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
///\file        XAML/XamlMacro_Index.h
///

#if !defined XAML_MACRO_INDEX_HEADER
#define XAML_MACRO_INDEX_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/macro_index.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current macro to be used

class XAMLTK_API WT_XAML_Macro_Index : public WT_Macro_Index
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Constructors
    /// Constructs a WT_XAML_Macro_Index object.
    WT_XAML_Macro_Index()
        : WT_Macro_Index()
    { }

    /// Constructs a WT_XAML_Macro_Index object using the Macro index. if the macro dosent exist
    /// it will be ignored
    WT_XAML_Macro_Index(WT_Integer32 macro)
        : WT_Macro_Index(macro)
    { }

	///Copy Constructor
	WT_XAML_Macro_Index(WT_Macro_Index const& index)
		:WT_Macro_Index(index)
	{ }

	virtual ~WT_XAML_Macro_Index()
	{}
    //@}
public:
   
    //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};
//@}

#endif //XAML_MACRO_INDEX_HEADER
