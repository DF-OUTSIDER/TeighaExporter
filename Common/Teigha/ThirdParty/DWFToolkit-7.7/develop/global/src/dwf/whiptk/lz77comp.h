//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

///
///\file        dwf/whiptk/lz77comp.h
///

#if !defined LZ77COMP_HEADER
#define LZ77COMP_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/fifo.h"
#include "whiptk/file.h"
#include "whiptk/lzdefs.h"

#ifdef    DWFCORE_BUILD_ZLIB
#include "dwfcore/zlib/zlib.h"
#else
#ifdef ANDROID
#include <ZLib/zlib.h>
#else
#include <zlib.h>
#endif
#endif

#define WD_LZ_BYTES_USED_FOR_HASH                4
#define WD_MAX_HASH_ENTRIES_TO_SEARCH            64
#define WD_LZ_HASH_TABLE_SIZE                    65536
#define WD_MAXIMUM_MATCH_LENGTH                  (255 + 15 + WD_LZ_COMPRESSION_AND_OFFSET_CODE_BYTES)
#define WD_MAX_LITERAL_DATA_STREAM               (255 + 15)

/// For use by the compression classes.
/** \note Internal use only - not intended for direct use by client code. */
class WT_History_Item
{
private:
    WT_Byte                 m_value;
    WT_History_Item *       m_next;
    WT_History_Item **      m_backlink;

/* FIXME:  The copy constructor and assignment operators should not be public, but private!
   Unfortunately, when private, we generate the following error:

    ../../inc\whiptk\fifo.h(155) : error C2248: 'WT_History_Item::operator`='' : cannot access private member declared in class 'WT_History_Item'
            ../../inc\whiptk\lz77comp.h(56) : see declaration of 'WT_History_Item::operator`=''
            ../../inc\whiptk\lz77comp.h(36) : see declaration of 'WT_History_Item'
            ../../inc\whiptk\fifo.h(54) : while compiling class-template member function 'WT_Result WT_FIFO<_ItemType>::add(int,const _ItemType *)'
            with
            [
                _ItemType=WT_History_Item
            ]
            ../../inc\whiptk\lz77comp.h(103) : see reference to class template instantiation 'WT_FIFO<_ItemType>' being compiled
            with
            [
                _ItemType=WT_History_Item
            ]
    Alternatively, a proper definition of these members could be supplied, although it's difficult to ascertain what that should be.
*/
public:
    WT_History_Item (WT_History_Item const &)
        : m_value(0)
        , m_next(WD_Null)
        , m_backlink(WD_Null)
    {
        WD_Complain ("cannot copy WT_History_Item");
    } // prohibited


    WT_History_Item & operator= (WT_History_Item const &)
    {
        WD_Complain ("cannot assign WT_History_Item");
        return *this;
    } // prohibited

public:
    // Constructors, Destructors

    WT_History_Item()
        : m_value(0)
        , m_next(WD_Null)
        , m_backlink(WD_Null)
    { }

    ~WT_History_Item()
    { }

    void unlink()
    {
        if ((*m_backlink = m_next) != WD_Null)
            m_next->set_backlink(m_backlink);
    }

    void set_value(WT_Byte data)
    {    m_value = data;    }
    WT_Byte    value() const
    {    return m_value;    }

    void set_next(WT_History_Item * next)
    {    m_next = next;    }
    WT_History_Item * & next()
    {    return m_next;    }

    void set_backlink(WT_History_Item ** backlink)
    {    m_backlink = backlink;    }
    WT_History_Item ** & backlink()
    {    return m_backlink;    }
};


/// A Ziv-Lempel compressor.
/** \note Internal use only - not intended for direct use by client code. */
class WT_LZ_Compressor : public WT_Compressor
{
private:
    WT_FIFO<WT_Byte>            m_candidate;
    WT_FIFO<WT_History_Item>    m_history;
    WT_History_Item *           m_hash_table[WD_LZ_HASH_TABLE_SIZE];
    WT_Boolean                  m_at_least_one_extendible;
    WT_FIFO<WT_Byte>            m_literal_output_buf;
    WT_File &                   m_file;
    WT_Boolean                  m_compression_started;
    int                         m_best_match_length;
    WT_History_Item *           m_best_match;
    WT_Boolean                  m_best_match_extendible;
    int                         m_hash_entries_searched;

    void    preload_history_buffer();

#ifdef DEBUG_ASSERTIONS
    int                            m_total_literal_bytes;
    int                            m_total_literal_strings;
    int                            m_num_literal_string_overflows;
    int                            m_total_compressed_bytes;
    int                            m_total_compression_codes;
    int                            m_biggest_compressed_string;
    int                            m_hash_misses;
    int                            m_hash_hits;
#endif

//    WT_LZ_Compressor(WT_LZ_Compressor const & other)
//        : WT_Compressor()
//        , m_candidate()
//        , m_history()
//        , m_at_least_one_extendible()
//        , m_literal_output_buf()
//        , m_file(other.m_file) //_WT_File_g_none)
//        , m_compression_started()
//        , m_best_match_length()
//        , m_best_match()
//        , m_best_match_extendible()
//        , m_hash_entries_searched()
//#ifdef DEBUG_ASSERTIONS
//        , m_total_literal_bytes()
//        , m_total_literal_strings()
//        , m_num_literal_string_overflows()
//        , m_total_compressed_bytes()
//        , m_total_compression_codes()
//        , m_biggest_compressed_string()
//        , m_hash_misses()
//        , m_hash_hits()
//#endif
//    {
//        WD_Complain ("cannot copy WT_LZ_Compressor");
//    } // prohibited

    //WT_LZ_Compressor operator= (WT_LZ_Compressor const &)
    //{
    //    WD_Complain ("cannot assign WT_LZ_Compressor");
    //    return *this;
    //} // prohibited

public:

    // Constructors, Destructors

    WT_LZ_Compressor(WT_File & file)
        : m_history(WD_LZ_HISTORY_BUFFER_SIZE)
        , m_at_least_one_extendible(WD_False)
        , m_literal_output_buf()
        , m_file(file)
        , m_compression_started(WD_False)
        , m_best_match_length(WD_LZ_COMPRESSION_AND_OFFSET_CODE_BYTES)
        , m_best_match(WD_Null)
        , m_best_match_extendible(WD_False)
        , m_hash_entries_searched(0)

#ifdef DEBUG_ASSERTIONS
        , m_total_literal_bytes(0)
        , m_total_literal_strings(0)
        , m_num_literal_string_overflows(0)
        , m_total_compressed_bytes(0)
        , m_total_compression_codes(0)
        , m_biggest_compressed_string(0)
        , m_hash_misses(0)
        , m_hash_hits(0)
#endif
    { }

    ~WT_LZ_Compressor()
    { }

    WT_Result    compress(int in_size, void const * in_buf);
             WT_Result    output_match();
             void        extend_best_match_length();
             void        find_better_match(WT_History_Item * history_item);
    WT_Result    start();
    WT_Boolean   is_compression_started() { return m_compression_started; }
    WT_Result    stop();
    WT_Result    add_to_history_buffer(WT_Byte a_byte, WT_Boolean output);
};


/// A Zlib compressor.
/** \note Internal use only - not intended for direct use by client code. */
class WT_ZLib_Compressor : public WT_Compressor
{
private:
    WT_File &                   m_file;
    WT_Boolean                  m_compression_started;
    z_stream                    m_zlib_stream; /* compression stream */
    WT_Byte                     m_zlib_output_buffer[WD_ZLIB_COMPRESSION_OUTPUT_BUFFER_SIZE];

    WT_Result   preload_history_buffer();

    //WT_ZLib_Compressor(WT_ZLib_Compressor const & other)
    //    : WT_Compressor()
    //    , m_file(other.m_file)//_WT_File_g_none)
    //    , m_compression_started(WD_False)
    //    , m_zlib_stream()
    //{
    //    WD_Complain ("cannot copy WT_ZLib_Compressor");
    //} // prohibited

    WT_ZLib_Compressor & operator= (WT_ZLib_Compressor const &)
    {
        WD_Complain ("cannot assign WT_ZLib_Compressor");
        return *this;
    } // prohibited


public:

    // Constructors, Destructors

    WT_ZLib_Compressor(WT_File & file)
        : m_file(file)
        , m_compression_started(WD_False)
    { }

    ~WT_ZLib_Compressor()
    { }

    WT_Result    start();
    WT_Boolean   is_compression_started() { return m_compression_started; }
    WT_Result    stop();
    WT_Result    compress(int in_size, void const * in_buf);
};

#endif // LZ77COMP_HEADER
