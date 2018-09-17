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
///\file        dwf/whiptk/lz77decp.h
///

#if !defined LZ77DECP_HEADER
#define LZ77DECP_HEADER

#include "whiptk/whipcore.h"
#include "whiptk/fifo.h"
#include "whiptk/file.h"
#include "whiptk/lzdefs.h"

#ifdef  DWFCORE_BUILD_ZLIB
#include "dwfcore/zlib/zlib.h"
#else
#ifdef ANDROID
#include <ZLib/zlib.h>
#else
#include <zlib.h>
#endif
#endif

/// A Ziv-Lempel decompressor.
/** \note Internal use only - not intended for direct use by client code. */
class WT_LZ_DeCompressor : public WT_DeCompressor
{
private:
    enum Decompression_State
    {
        Literal_Data,
        Compression_Code,
        Process_Compression_Length_Code,
        Extended_Literal_Length_Code,
        Extended_Compression_Length_Code,
        First_Offset_Code_Byte,
        Second_Offset_Code_Byte,
        Eating_Recall_Buffer_Data
    };

    WT_File &                   m_file;
    Decompression_State         m_next_byte;
    WT_Integer32                m_compression_run;
    WT_Integer32                m_literal_data_run;
    WT_Integer32                m_offset;
    WT_FIFO<WT_Byte>            m_recall_buffer;

    void preload_recall_buffer();

  //  WT_LZ_DeCompressor (WT_LZ_DeCompressor const & other)
  //      : WT_DeCompressor()
		//, m_file(other.m_file) //_WT_File_g_none)
  //      , m_next_byte(Compression_Code)
  //      , m_compression_run(0)
  //      , m_literal_data_run(0)
  //      , m_offset(0)
  //  {
  //      WD_Complain ("cannot copy WT_LZ_DeCompressor");
  //  } // prohibited


    WT_LZ_DeCompressor & operator= (WT_LZ_DeCompressor const &)
    {
        WD_Complain ("cannot assign WT_LZ_DeCompressor");
        return *this;
    } // prohibited

public:

    // Constructors, Destructors

    WT_LZ_DeCompressor(WT_File & file)
        : m_file(file)
        , m_next_byte(Compression_Code)
        , m_compression_run(0)
        , m_literal_data_run(0)
        , m_offset(0)
    { }

    ~WT_LZ_DeCompressor()
    { }

    WT_Result    start()
    {
        preload_recall_buffer();
        return WT_Result::Success;
    }

    WT_Result    decompress(int desired, int & actual, void * buffer);
};


/// A Zlib decompressor.
/** \note Internal use only - not intended for direct use by client code. */
class WT_ZLib_DeCompressor : public WT_DeCompressor
{
private:

    WT_File &                   m_file;
    z_stream                    m_zlib_stream; /* Zlib's decompression stream */
    WT_Byte                     m_compressed_data_buffer[WD_ZLIB_DECOMPRESSION_INPUT_BUFFER_SIZE];

    //WT_ZLib_DeCompressor (WT_ZLib_DeCompressor const & other)
    //    : WT_DeCompressor()
    //    , m_file(other.m_file) //_WT_File_g_none)
    //    , m_zlib_stream()
    //{
    //    WD_Complain ("cannot copy WT_ZLib_DeCompressor");
    //} // prohibited


    WT_ZLib_DeCompressor & operator= (WT_ZLib_DeCompressor const &)
    {
        WD_Complain ("cannot assign WT_ZLib_DeCompressor");
        return *this;
    } // prohibited

public:

    // Constructors, Destructors

    WT_ZLib_DeCompressor(WT_File & file)
        : m_file(file)
        , m_zlib_stream()
    { }

    ~WT_ZLib_DeCompressor()
    { }

    WT_Result    start();
    WT_Result    decompress(int desired, int & actual, void * buffer);
};


#endif // LZ77DECP_HEADER
