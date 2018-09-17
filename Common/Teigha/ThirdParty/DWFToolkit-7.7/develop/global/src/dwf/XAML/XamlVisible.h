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

#if !defined XAML_VISIBLE_HEADER
#define XAML_VISIBLE_HEADER

///
///\file        XAML/XamlVisible.h
///

#include "XAML/XamlCore.h"

#include "whiptk/visible.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute which indicates whether subsequent graphics are to be visible.
/** \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_Visibility : public WT_Visibility
                                    , public XamlDrawableAttributes::PathAttributeProvider
                                    , public XamlDrawableAttributes::GlyphsAttributeProvider
                                    , public XamlDrawableAttributes::Opacity::Provider
                                    , public XamlDrawableAttributes::Opacity::Consumer
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_Visibility object.
    WT_XAML_Visibility()
        : WT_Visibility()
    { }
    /// Constructs a WT_XAML_Visibility object with the given value.
    WT_XAML_Visibility(WT_Boolean visible)
        : WT_Visibility(visible)
    { }

	///Copy Constructor
	WT_XAML_Visibility(WT_Visibility const& visibility)
		:WT_Visibility(visibility)
	{}
	virtual ~WT_XAML_Visibility()
	{}
    //@}

public:

	WT_XAML_Visibility &   operator=(WT_XAML_Visibility const & visibility)
    {
		WT_Visibility::operator = (visibility);
        return *this;

    }
    
    //
    // from XamlDrawableAttributes::PathAttributeProvider
    //
    virtual WT_Result providePathAttributes(
        XamlDrawableAttributes::PathAttributeConsumer*, // consumer, != 0
        WT_XAML_File& ) const;                          // current file

    //
    // from XamlDrawableAttributes::GlyphsAttributeProvider
    //
    virtual WT_Result provideGlyphsAttributes(
        XamlDrawableAttributes::GlyphsAttributeConsumer *pConsumer,
        WT_XAML_File &) const;

    //
    // from XamlDrawableAttributes::Opacity::Provider
    //
    virtual WT_Result provideOpacity(
        XamlDrawableAttributes::Opacity *&);            // memento, may be null

    //
    // from XamlDrawableAttributes::Opacity::Consumer
    //
    virtual WT_Result consumeOpacity( XamlDrawableAttributes::Opacity::Provider * );
    
	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
   
    virtual WT_XAML_Visibility &   operator=(WT_Visibility const & visibility)
    {
		WT_Visibility::operator= (visibility);
        return *this;
    }
    //@}

};
//@}

#endif //XAML_VISIBLE_HEADER

