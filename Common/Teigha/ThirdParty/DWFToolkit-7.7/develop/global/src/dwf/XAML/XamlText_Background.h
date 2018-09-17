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

#if !defined XAML_TEXT_BACKGROUND_HEADER
#define XAML_TEXT_BACKGROUND_HEADER

///
///\file        XAML/XamlText_Background.h
///

#include "XAML/XamlCore.h"

#include "whiptk/text_background.h"
#include "XAML/XamlXML.h"

// Attribute class for specifying text background
class XAMLTK_API WT_XAML_Text_Background : public WT_Text_Background
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:
    /// constructs a WT_XAML_Text_Background object
    WT_XAML_Text_Background()
		: WT_Text_Background()
	{}

    /// constructs a WT_XAML_Text_Background object with the given background style and color
    WT_XAML_Text_Background(eBackground background, WT_Integer32 offset)
		: WT_Text_Background(background,offset)
	{}

	///Copy Constructor
	WT_XAML_Text_Background(WT_Text_Background const& backgrnd)
		:WT_Text_Background(backgrnd)
	{}
    /// destructor
    virtual ~WT_XAML_Text_Background()
	{}

public:
	/// WT_Object virtual methods
    WT_Result      serialize(WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result      parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    
};

#endif //XAML_TEXT_BACKGROUND_HEADER
