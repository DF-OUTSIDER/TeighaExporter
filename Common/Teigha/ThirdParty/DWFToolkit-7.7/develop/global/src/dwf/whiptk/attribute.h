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

#if !defined ATTRIBUTE_HEADER
#define ATTRIBUTE_HEADER

///
///\file        dwf/whiptk/attribute.h
///

#include "whiptk/whipcore.h"
#include "whiptk/object.h"

class WT_File;

/// Base class for attributes contained by WT_Rendition and WT_Rendering_Options.
/** \note When seriailzing (writing) data to a file, clients should always
 *  use the WT_File::desired_rendition() to set attributes.  When materializing
 *  (reading) data, clients should use the WT_File::rendition() to read attributes.
 *  \warning Clients should never call serialize() or sync() to write attributes manually,
 *  as the optimization of the resultant file will suffer.  Serialization will
 *  occur automatically through the framework.
 *  \sa WT_File::desired_rendition(), WT_File::rendition().
 */
class WHIPTK_API WT_Attribute : public WT_Object
{
	friend class WT_Class_Factory;
	friend class WT_DeprecatedClassFactory;
protected:
    /// Constructs a WT_Attribute object.
    WT_Attribute ()
    { }

    /// Destroys a WT_Attribute object.
    virtual ~WT_Attribute ()
    { }

public:
    /// Returns the object type WT_Type::Attribute.
    virtual WT_Type    object_type() const;

    /// Forces the serialization of the object to the file.
    /** \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     */
    virtual WT_Result        serialize (WT_File & file) const = 0;

    /// Serializes the attribute out to the file, if necessary.
    /** Only outputs relevant desired rendition changes to the file.
     *  \warning This is used by the framework and should not be called by client code.
     *  \retval WT_Result::Success The operation was successful.
     *  \sa WT_File::desired_rendition().
     */
    virtual WT_Result    sync(
        WT_File & file /**< File being written. */
        ) const = 0;

    /// Returns WD_True if the attributes are equal.
    virtual WT_Boolean operator==(WT_Attribute const & attrib) const = 0;

    /// Returns WD_True if the attributes are not equal.
    WT_Boolean operator!=(WT_Attribute const & attrib) const
    {
        return !(*this == attrib);
    }
};

#endif // ATTRIBUTE_HEADER
