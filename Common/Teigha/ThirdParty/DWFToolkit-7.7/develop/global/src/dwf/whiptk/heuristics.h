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

#if !defined HEURISTICS_HEADER
#define HEURISTICS_HEADER

///
///\file        dwf/whiptk/heuristics.h
///

#include "whiptk/whipcore.h"

/** \addtogroup groupFileIO
 *  @{
 */

/// Contains settings used during file I/O operations.
/** \sa WT_File */
class WHIPTK_API   WT_File_Heuristics
{
    friend class WT_Plot_Info;
    friend class WT_Informational;
    friend class WT_Opcode;
    friend class WT_DWF_Header;
	friend class WT_File;
public:
    /// Constructs a WT_File_Heuristics object for the optional WT_File object.
    WT_File_Heuristics();
    /// Destroys a WT_File_Heuristics object.
    virtual ~WT_File_Heuristics();
    //@{ \name Heuristic set methods
    /// Sets whether the file should be opened for writing as a binary file (defaults to WD_True.)
    /** \attention Set this AFTER calling WT_File::open().
     *  \note This is only used when writing to a file.  There is not applicable in a read mode.
     */
    void                 set_allow_binary_data (WT_Boolean binary) {   m_allow_binary_data = binary;    }
    /// Sets whether the file should be opened for writing as a compressed file (defaults to WD_False.)
    /** \attention Set this AFTER calling WT_File::open().
     *  \note This is only used when writing to a file.  There is not applicable in a read mode.
     */
    void                 set_allow_data_compression(WT_Boolean allowed) {   m_allow_data_compression = allowed;    }
    /// Sets whether the file should, whenever possible, merge drawables to save space (defaults to WD_True.)
    /** \note This is only used when writing to a file.  There is not applicable in a read mode. */
    void                 set_allow_drawable_merging (WT_Boolean merging) {   m_allow_drawable_merging = merging;    }
    /// Sets whether the file should, whenever possible, use colors indexes (in the color map) to save space (defaults to WD_True.)
    /** \note This is only used when writing to a file.  There is not applicable in a read mode. */
    void                 set_allow_indexed_colors(WT_Boolean indexed) {   m_allow_indexed_colors = indexed; }
    /// Sets whether the file should apply a transform to geometry (defaults to WD_False.)
    /** \note This is only used when writing to a file.  There is not applicable in a read mode. */
    void                 set_apply_transform(WT_Boolean apply)  {    m_apply_transform = apply;    }
    /// Sets whether the file should apply a transform to the units matrix (defaults to WD_True.)
    /** \note This is only used when writing to a file.  There is not applicable in a read mode. */
    void                 set_apply_transform_to_units_matrix(WT_Boolean apply) {    m_apply_transform_to_units_matrix = apply;    }

    /// Sets the deferred delete state (client code is responsible for deleting objects created by the toolkit).
    /** \warning When using deferred delete, be very careful when using
     *  WT_File::get_next_object_shell().  The deferred delete
     *  processing assumes that the file pointer has been moved.
     *  This happens when the object has been materialized, or
     *  the operand has been skipped.  Normally, in a non-deferred
     *  state, if the object hasn't been materialized,
     *  WT_File::get_next_object_shell() takes care of skipping the current
     *  (non-materialized) object's operand and thus advancing the
     *  file pointer, by using WT_Object::skip_operand().  However,
     *  in the deferred delete state, it's possible that the client
     *  has already deleted the current object before
     *  get_next_object_shell() is called, so we avoid calling
     *  the methods on the current object to discover whether or not
     *  it is materialized.  Thus, the file pointer will not
     *  automatically get advanced in that case.  The suggested usage
     *  by clients using deferred_delete is to either always use
     *  WT_File::get_next_object(), which always materializes the
     *  object and thus advances the file pointer - or to make sure
     *  the operand is skipped if the object is not materialized.
     *  \sa WT_File::get_next_object_shell(), WT_Object::skip_operand().
     */
    void                 set_deferred_delete(WT_Boolean state)  { m_deferred_delete = state;    }
    /// Sets the target version which should be used when creating this file.
    /** The version is calculated as follows: (major_version * 100) + minor_version.
     *  For example, if the desired file version is 00.55, the target version should be set to 55;
     *  for version 06.00, the target version should be set to 600.
     *  \note This is only used when writing to a file.  This method is not applicable in a read mode.
     *  When reading a file, clients should call %WT_File::rendition().drawing_info().decimal_revision()
     *  to retrieve the revision of the file being read.
     *  \attention Set this before calling WT_File::open().
     */
    void                 set_target_version(int target, WT_File * file = WD_Null);
    /// Sets a transform to be applied to geometry (and potentially the units matrix).
    /** The use of the transform is goverened by the apply_transform and apply_transform_to_units_matrix
     *  flags set by set_apply_transform() and set_apply_transform_to_units_matrix(), respectively.
     *  \note This is only used when writing to a file.  There is not applicable in a read mode.
     */
    void                 set_transform(WT_Transform const & transform) {    m_transform = transform;    }
    /// Sets a pointer to a user-defined data item.
    void                 set_user_data(void * user_data) {    m_user_data = user_data;}
    //@}

    //@{ \name Heuristic get methods
    /// Returns WD_True if the file was opened for writing as a binary file.
    WT_Boolean           allow_binary_data () const {   return m_allow_binary_data;      }
    /// Returns WD_True if the file was be opened for writing as a compressed file.
    WT_Boolean           allow_data_compression() const {   return m_allow_data_compression;   }
    /// Returns WD_True if the file is set to merge drawables to save space.
    WT_Boolean           allow_drawable_merging () const {   return m_allow_drawable_merging;      }
    /// Returns WD_True if the file is set to use colors indexes (in the color map) to save space.
    WT_Boolean           allow_indexed_colors() const {   return m_allow_indexed_colors;  }
    /// Returns WD_True if the file is set to apply a transform to geometry.
    /** \note This is only used when writing to a file.  There is not applicable in a read mode. */
    WT_Boolean           apply_transform() const {    return m_apply_transform;    }
    /// Returns WD_True if the file is set to apply a transform to the units matrix
    /** \note This is only used when writing to a file.  There is not applicable in a read mode.
     *  \sa WT_Drawing_Info.
     */
    WT_Boolean           apply_transform_to_units_matrix() const {    return m_apply_transform_to_units_matrix;    }

    /// Returns WD_True if objects that are created by the toolkit will be deleted by the client code.
    /** \note This is only used when reading a file.  There is not applicable in a write mode. \sa set_deferred_delete(). */
    WT_Boolean           deferred_delete() const   { return m_deferred_delete;    }
    /// Returns the desired target version.
    /** \note This is only used when writing to a file.  This method is not applicable in a read mode.
     *  When reading a file, clients should call %WT_File::rendition().drawing_info().decimal_revision()
     *  to retrieve the revision of the file being read.
     *  \sa WT_Drawing_Info::decimal_revision()
     */
    int                  target_version() const {   return m_target_version;    }
    /// Returns the desired transform to be applied to geometry (and possibly the units matrix) when writing a file.
    /** \note This is only used when writing to a file.  There is not applicable in a read mode. */
    WT_Transform const & transform() const {    return m_transform;    }
    WT_Transform &       transform()       {    return m_transform;    }
    /// Returns the pointer to a user-defined data item.
    void *               user_data() {    return m_user_data;        }
    //@}

private:
    void                 set_broken_plotinfo(WT_Boolean state) { m_broken_plotinfo = state;    }
    WT_Boolean           broken_plotinfo() const               { return m_broken_plotinfo;    }

    WT_Boolean			m_allow_data_compression;
    WT_Boolean			m_allow_indexed_colors;
    WT_Boolean			m_allow_binary_data;
    WT_Boolean			m_allow_drawable_merging;
    WT_Boolean			m_apply_transform;
    WT_Boolean			m_apply_transform_to_units_matrix;
    WT_Transform		m_transform;
    int					m_target_version;
    void *				m_user_data;
    WT_Boolean			m_deferred_delete;
    WT_Boolean			m_broken_plotinfo;
    WT_Boolean			m_w2d_channel;

    WT_File_Heuristics (WT_File_Heuristics const &)
      : m_allow_data_compression()
      , m_allow_indexed_colors()
      , m_allow_binary_data()
      , m_allow_drawable_merging()
      , m_apply_transform()
      , m_apply_transform_to_units_matrix()
      , m_transform()
      , m_target_version()
      , m_user_data()
      , m_deferred_delete()
      , m_broken_plotinfo()
      , m_w2d_channel()
    {
        WD_Complain ("cannot copy WT_File_Heuristics");
    } // prohibited

    WT_File_Heuristics & operator= (WT_File_Heuristics const &)
    {
        WD_Complain ("cannot assign WT_File_Heuristics");
        return *this;
    } // prohibited
};

//@}

#endif // HEURISTICS_HEADER
