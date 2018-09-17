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
///\file        XAML/XamlMacro_Scale.h
///

#if !defined XAML_MACRO_SCALE_HEADER
#define XAML_MACRO_SCALE_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/macro_scale.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current Macro scale
/**
 *
 *  \sa WT_Rendition
 */
class XAMLTK_API WT_XAML_Macro_Scale : public WT_Macro_Scale
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_Macro_Scale object.
    WT_XAML_Macro_Scale()
        : WT_Macro_Scale()
    { }
    /// Constructs a WT_XAML_Macro_Scale object with the given size (in drawing units.)
    WT_XAML_Macro_Scale(WT_Integer32 macro_scale)
        : WT_Macro_Scale(macro_scale)
    { }

	///Copy Constructor
	WT_XAML_Macro_Scale(WT_Macro_Scale const& scale)
		:WT_Macro_Scale(scale)
	{}
	virtual ~WT_XAML_Macro_Scale()
	{}
public:
    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);

};
//@}

#endif //XAML_MACRO_SCALE_HEADER
