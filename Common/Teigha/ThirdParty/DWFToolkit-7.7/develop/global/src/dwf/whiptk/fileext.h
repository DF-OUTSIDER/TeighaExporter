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

#if !defined FILEEXT_HEADER
#define FILEEXT_HEADER

///
///\file        dwf/whiptk/fileext.h
///

#include "whiptk/whip_toolkit.h"
#include "whiptk/compdata.h"
#include "whiptk/gouraud_polyline.h"
#include "whiptk/gouraud_polytri.h"
#include "whiptk/unknown.h"

/** \addtogroup groupFileIO
 *  @{
 */

/// Inherits from WT_File and facilitates subclassing the IO overrides.
/** This is basically a WT_File with the various global IO callbacks
 *  routed back into protected instance methods within the class. The instance
 *  callback methods do the same default processing as would a normal WT_File
 *  hence behavior is business as usual except for those callback methods
 *  an inheriting class overides.
 */
class WHIPTK_API WT_FileExt : public WT_File
{
private:
    // Class action methods
    #define CALLBACK_MACRO(Upper, Lower)\
    static WT_Result process_##Lower (WT_##Upper & Lower, WT_File& file)\
        { return ((WT_FileExt&)file).process_##Lower(Lower); }
    CALLBACK_LIST
    #undef CALLBACK_MACRO

    // Class stream methods
    static WT_Result process_stream_open (WT_File& file)
        { return ((WT_FileExt&)file).process_stream_open(); }
    static WT_Result process_stream_close (WT_File& file)
        { return ((WT_FileExt&)file).process_stream_close(); }
    static WT_Result process_stream_read (WT_File& file, int desired_bytes, int& bytes_read, void* buffer)
        { return ((WT_FileExt&)file).process_stream_read(desired_bytes, bytes_read, buffer); }
    static WT_Result process_stream_write(WT_File& file, int size, void const* buffer)
        { return ((WT_FileExt&)file).process_stream_write(size, buffer); }
    static WT_Result process_stream_seek (WT_File& file, int distance, int& amount_seeked)
        { return ((WT_FileExt&)file).process_stream_seek(distance, amount_seeked); }

    static WT_Result process_stream_end_seek (WT_File& file)
        { return ((WT_FileExt&)file).process_stream_end_seek(); }
    static WT_Result process_stream_tell (WT_File& file, unsigned long *current_file_pointer_position)
        { return ((WT_FileExt&)file).process_stream_tell(current_file_pointer_position); }


protected:
    // Instance action methods
#ifndef DOXYGEN_EXCLUDE
    #define CALLBACK_MACRO(Upper, Lower)\
    virtual WT_Result process_##Lower (WT_##Upper & Lower)\
        { return WT_##Upper::default_process(Lower, *this); }
    CALLBACK_LIST
    #undef CALLBACK_MACRO
#endif

    /** \name I/O customization set/get methods
     *  These methods allow client code to override how low-level file I/O works
     *  by allowing client code to selectively implement various I/O procedures.
     *  @{
     */
    virtual WT_Result process_stream_close (void) { return default_close(*this); }
    virtual WT_Result process_stream_end_seek () { return default_end_seek(*this); }
    virtual WT_Result process_stream_open (void)  { return default_open(*this); }
    virtual WT_Result process_stream_read (int desired_bytes, int& bytes_read, void* buffer) { return default_read(*this, desired_bytes, bytes_read, buffer); }
    virtual WT_Result process_stream_seek (int distance, int& amount_seeked) { return default_seek(*this, distance, amount_seeked); }
    virtual WT_Result process_stream_tell (unsigned long *current_file_pointer_position) { return default_tell(*this, current_file_pointer_position); }
    virtual WT_Result process_stream_write(int size, void const* buffer) { return default_write(*this, size, buffer); }
    //@}

protected:
    /// Constructs a WT_FileExt object.
    WT_FileExt (void)
    {
        // set the action methods
        #define CALLBACK_MACRO(Upper, Lower)\
        m_##Lower##_action = process_##Lower;
        CALLBACK_LIST
        #undef CALLBACK_MACRO

        // set the stream methods
        m_stream_open_action  = process_stream_open;
        m_stream_close_action = process_stream_close;
        m_stream_read_action  = process_stream_read;
        m_stream_write_action = process_stream_write;
        m_stream_seek_action  = process_stream_seek;
        m_stream_end_seek_action    = process_stream_end_seek;
        m_stream_tell_action        = process_stream_tell;
    }

    // Deconstructs a WT_FileExt object.
    virtual ~WT_FileExt()
    {
        ;
    }
};

//@}

#endif // FILEEXT_HEADER
