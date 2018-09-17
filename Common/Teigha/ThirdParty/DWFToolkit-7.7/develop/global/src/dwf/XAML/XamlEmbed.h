//
//  Copyright (c) 2006 by Autodesk, Inc.
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

#if !defined XAML_EMBED_HEADER
#define XAML_EMBED_HEADER

///
///\file        XAML/XamlEmbed.h
///

#include "XAML/XamlCore.h"

#include "whiptk/embed.h"


class XAMLTK_API WT_XAML_Embed 
    : public WT_Embed
{
	friend class WT_XAML_Class_Factory;

protected:
    /// Constructs a WT_XAML_Embed object.
    WT_XAML_Embed()
		: WT_Embed()
	{}

    /// Overrides default copy constructor
    WT_XAML_Embed (WT_XAML_Embed const & embed)
		: WT_Embed(static_cast<WT_Embed const &>(embed))
	{}   

    /// Conversion constructor
    WT_XAML_Embed (WT_Embed const & embed)
		: WT_Embed(embed)
	{}   


    /// Destroys a WT_XAML_Embed object.
    virtual ~WT_XAML_Embed()
	{}

public:
	/// Overrides default assignment operator
    WT_XAML_Embed & operator= (WT_XAML_Embed const & other)
	{
		WT_Embed::operator= (other);
		return *this;
	}

	 //@{ \name WT_Object virtual methods
    virtual WT_Result  serialize(WT_File & /*file*/) const;
    /// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}

protected:

};

#endif //XAML_EMBED_HEADER
