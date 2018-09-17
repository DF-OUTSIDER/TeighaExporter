//  Copyright (c) 2001-2006 by Autodesk, Inc.
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




#if !defined XAML_CONTRAST_COLOR_HEADER
#define XAML_CONTRAST_COLOR_HEADER

///
///\file        dwf/XAML/XamlContrastColor.h
///

#include "XAML/XamlCore.h"

#include "whiptk/contrastcolor.h"


/** \addtogroup groupRenditionAttributes
 *  @{
 *  The BackgroundColor rendition attribute is a straightforward 32 bit color declaration 
 *  ( RGBA) that is applied to drawables that require a background color. 
 *  Note: Unlike the color attribute it does not work with a color map and requires
 *  that every definition of Background color have its entire RGBA (Red Green Blue Alpha)
 *  definition
 */


/// Attribute class for specifying a Background color 
class XAMLTK_API WT_XAML_Contrast_Color : public WT_Contrast_Color
                                        , public XamlDrawableAttributes::Stroke::Provider
                                        , public XamlDrawableAttributes::Fill::Provider
                                        , public XamlDrawableAttributes::Stroke::Consumer
                                        , public XamlDrawableAttributes::Fill::Consumer
                                        , public XamlDrawableAttributes::PathAttributeProvider
                                        , public XamlDrawableAttributes::GlyphsAttributeProvider
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:
	//@{ \name Construction
    /// Default Constructor 
    WT_XAML_Contrast_Color()
        : WT_Contrast_Color()
        , _pSerializeFile( NULL)
    {}
    /// Constructor with  WT_RGBA32 structure
    WT_XAML_Contrast_Color(WT_RGBA32 color)
        : WT_Contrast_Color(color)
        , _pSerializeFile( NULL)
    {}
    
    /// Constructor with individual color and alpha values
    WT_XAML_Contrast_Color(
        int red, /**< Red coponent of RGBA color. */
        int green, /**< Green coponent of RGBA color. */
        int blue, /**< Blue coponent of RGBA color. */
        int alpha = 255) /**< Alpha component of RGBA color (0 = transparent, 255 = opaque.) */
        : WT_Contrast_Color(red,green,blue,alpha)
        , _pSerializeFile( NULL)
    {}
    
	///Copy Consytructor
	WT_XAML_Contrast_Color(const WT_Contrast_Color& color)
		: WT_Contrast_Color(color)
        , _pSerializeFile( NULL)
	{}

	~WT_XAML_Contrast_Color()
	{}

public:
	 //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);

    //@}
protected:
    virtual WT_Result provideStroke(XamlDrawableAttributes::Stroke* & rpStroke);
    virtual WT_Result provideFill(XamlDrawableAttributes::Fill*& rpFill);
    virtual WT_Result provideGlyphsAttributes( XamlDrawableAttributes::GlyphsAttributeConsumer* p, WT_XAML_File& r) const;
    virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer*, WT_XAML_File& ) const;
    virtual WT_Result consumeStroke(XamlDrawableAttributes::Stroke::Provider* );
    virtual WT_Result consumeFill(XamlDrawableAttributes::Fill::Provider* );

private:
    WT_XAML_File *_pSerializeFile;
};
//@}

#endif //XAML_CONTRAST_COLOR_HEADER
