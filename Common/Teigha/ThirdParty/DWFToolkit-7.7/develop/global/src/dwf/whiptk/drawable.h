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

#if !defined DRAWABLE_HEADER
#define DRAWABLE_HEADER

///
///\file        dwf/whiptk/drawable.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"
#include "whiptk/file.h"

/// Base class for all drawable objects.
class WHIPTK_API WT_Drawable : public WT_Object
{
	friend class WT_File;
	friend class WT_Class_Factory;

protected:
    WT_Logical_Box    m_bounds; /**< \brief The drawable bounds. */
    WT_Boolean        m_bounds_set; /**< \brief Flag to determine if bounds need to be calculated. */

protected:

    //@{ \name Construction / destruction.
    /// Constructs a WT_Drawable object.
    WT_Drawable ()
    : m_bounds_set(WD_False)
    {
    }

    /// Destroys a WT_Drawable object.
    virtual ~WT_Drawable ()
    { }
    //@}
public:
    /// Returns the approximate bounds of the drawable (in drawing units.)
    WT_Logical_Box  bounds(
        WT_File * file=NULL /**< Optional current file object (examines current rendition's lineweight if non-null.) */
        );

    //@{ \name WT_Drawable virtual methods
    /// Delays the serialization of this object.
    /** Stores the object for later serialization (in case the following object is coincident
     *  and can be merged with this object.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \retval WT_Result::Internal_Error The drawable object does not implement the delay() method.
     */
    virtual    WT_Result       delay(WT_File & file) const;
    /// Forces the serialization of the object to the file.
    /** \warning This is used by the framework and should not be called by client code.
     *  Clients should always call serialize() so as to take advantage of the
     *  optimizations gained through delayed serialization and merging.
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual    WT_Result       dump(
        WT_File & file /**< The file being written. */
        ) const;

    /// Returns the object type WT_Type::Drawable.
    virtual    WT_Type         object_type() const;

    /// Merge a same-class drawable with this one.
    /** \exception WT_Result::Internal_Error The drawable object does not implement the merge() method.
     *  \retval WD_True The drawable was merged. */
    virtual    WT_Boolean      merge(WT_Drawable const & current);
    /// Forces the drawable bounds to be updated.
    virtual    void            update_bounds(WT_File * file);
    //@}

protected:
    /// Given the point, update the drawable bounds to include the point.
    void update_bounds(const WT_Logical_Point& pt, WT_File * file=NULL);
    /// Given the point set, update the drawable bounds to include the points.
    void update_bounds(const WT_Point_Set_Data& set, WT_File * file=NULL);
    /// Given the point array, update the drawable bounds to include the points.
    void update_bounds(const WT_Logical_Point*, int count, WT_File * file=NULL);
};

#endif // DRAWABLE_HEADER
