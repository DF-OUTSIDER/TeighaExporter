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

#if !defined XAML_PROJECTION_HEADER
#define XAML_PROJECTION_HEADER

///
///\file        XAML/XamlProjection.h
///

#include "XAML/XamlCore.h"

#include "whiptk/projection.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the indicates what portion of the drawing space is to be rendered.
/** \sa WT_Rendition */
class XAMLTK_API WT_XAML_Projection : public WT_Projection 
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction
    /// Creates a WT_Projection object.
    WT_XAML_Projection()
    : WT_Projection()
    {}
    /// Creates a WT_Projection object with the given projection type.
    WT_XAML_Projection(
        const WT_Projection_Type projection_type /** The projection type, one of \ref WT_Projection_Type "enum WT_Projection_Type". */
        )
		: WT_Projection(projection_type)
    {}

	WT_XAML_Projection(WT_Projection const& projection)
		:WT_Projection(projection)
	{}
	virtual ~WT_XAML_Projection()
	{}
    //@}
public:
	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}

};
//@}

#endif //XAML_PROJECTION_HEADER
