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

#if !defined XAMLCODE_PAGE_HEADER
#define XAMLCODE_PAGE_HEADER

///
///\file        XAML/XamlCode_Page.h
///

#include "XAML/XamlCore.h"

#include "whiptk/code_page.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute specifying the current code page (can be to assist with displaying localized strings.)
/** \sa WT_Rendition */
class XAMLTK_API WT_XAML_Code_Page : public WT_Code_Page
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:
	/// Constructs a WT_Code_Page object.
    WT_XAML_Code_Page()
    : WT_Code_Page()
    {}

    /// Constructs a WT_Code_Page object with the given page number.
    WT_XAML_Code_Page( WT_Integer32 page_number )
		: WT_Code_Page(page_number)
    {}

	//Copy Constructor
	WT_XAML_Code_Page(const WT_Code_Page& code_page)
		: WT_Code_Page(code_page)
	{}

	virtual ~WT_XAML_Code_Page()
	{}

public:
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
};
//@}

#endif //XAMLCODE_PAGE_HEADER
