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

#if !defined FILE_STATS_HEADER
#define FILE_STATS_HEADER

///
///\file        dwf/whiptk/file_stats.h
///

#include "whiptk/whipcore.h"

/** \addtogroup groupFileIO
 *  @{
 */

/// Collects file usage statistics (during reading.)
class WHIPTK_API WT_File_Stats
{
private:

    long            m_last_opcode_start_position;
    int             m_last_opcode_index;
    WT_File const * m_main_file;
    long            m_opcode_count[WD_MAX_OPCODE_ID + 1];
    long            m_opcode_total_bytes[WD_MAX_OPCODE_ID + 1];

public:
    WT_File_Stats (WT_File_Stats const &)
      : m_last_opcode_start_position()
      , m_last_opcode_index()
      , m_main_file()
    {
        WD_Complain ("cannot copy WT_File_Stats");
    } // prohibited

    WT_File_Stats & operator= (WT_File_Stats const &)
    {
        WD_Complain ("cannot assign WT_File_Stats");
        return *this;
    } // prohibited

    inline void add_stat(long opcode_index)
    {
        // Finish off tabulating the previous opcode we were dealing with:
        // Find out how many butes the last opcode + operand accounted for and add it in...
        m_opcode_total_bytes[m_last_opcode_index] += m_main_file->this_opcode_start_position() - m_last_opcode_start_position;

        // Store away info for future use in computing this opcode's length:
        m_last_opcode_start_position = m_main_file->this_opcode_start_position();
        m_last_opcode_index = opcode_index;

        // Now record the basics for this opcode:
        WD_Assert(opcode_index <= WD_MAX_OPCODE_ID);
        m_opcode_count[opcode_index]++;
    }

    /// Constructs a WT_File_Stats object used when reading the given file.
    WT_File_Stats(WT_File const * file);

    /// Destroys a WT_File_Stats object.
    virtual ~WT_File_Stats()
    { }

    /// Returns the number of times the given opcode was found in the file.
    /** \warning May overflow an internal array.  Clients should be sure to only provide
     *  a valid opcode index that is <= WD_MAX_OPCODE_ID.
     */
    const long opcode_count(int opcode_index) const  { return  m_opcode_count[opcode_index]; }

    /// Returns the number of bytes occupied by the given opcode in the file.
    /** \warning May overflow an internal array.  Clients should be sure to only provide
     *  a valid opcode index that is <= WD_MAX_OPCODE_ID.
     */
    const long opcode_total_bytes(int opcode_index) const  { return  m_opcode_total_bytes[opcode_index]; }

    /// Returns a text description of the given opcode (or an empty string.)
    /** Defaults to the description of the last read opcode.
     *  \warning May overflow an internal array.  Clients should be sure to only provide
     *  a valid opcode index that is <= WD_MAX_OPCODE_ID.
     */
    char const * descriptions(int index=-1) const;

};

//@}

#endif // FILE_STATS_HEADER
