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

#if !defined XAML_FONT_EXTENSION_HEADER
#define XAML_FONT_EXTENSION_HEADER

#include "XAML/XamlCore.h"

#include "whiptk/font.h"
#include "XAML/XamlXML.h"


/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current text font.
/** \sa WT_Rendition \sa WT_Text \sa \ref groupFontOptions "Font options"
 *
 *
 *  \sa See the embedded fonts example provided in the sample app \em W2DTest accompanying the WHIP!
 *  Toolkit source distribution.
 */
class XAMLTK_API WT_XAML_Font_Extension
    : public WT_Font_Extension
    , public XamlDrawableAttributes::FontUri::Provider
    , public XamlDrawableAttributes::GlyphsAttributeProvider
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

    WT_XAML_File* _pSerializeFile;
protected:
    //@{ \name Construction
    /// Constructs a WT_XAML_Font object.
    WT_XAML_Font_Extension()
        : WT_Font_Extension( ) 
        , _pSerializeFile( NULL )
	{}

    /// Constructs a WT_Font_Extension object with the given data.
    WT_XAML_Font_Extension(
        WT_String logfont_name, /**< The font name as it would appear in a LOGFONT structure. */
        WT_String cannonical_name /**< The cannonical font name.*/
        )
        : WT_Font_Extension( logfont_name, cannonical_name )
        , _pSerializeFile( NULL )
    {}

	virtual ~WT_XAML_Font_Extension()
	{}
    //@}
public:

	 /// Assignment operator.  Copies the given font settings to this one.
    WT_XAML_Font_Extension const & operator=  (WT_XAML_Font_Extension const & font)
	{
		WT_Font_Extension::operator = (font);
		return *this;
	}

	 //@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);

protected:

    virtual WT_Result provideFontUri( XamlDrawableAttributes::FontUri*&  pUri);
    virtual WT_Result provideGlyphsAttributes( XamlDrawableAttributes::GlyphsAttributeConsumer* p, WT_XAML_File& r) const;
};
//@}

#endif //XAML_FONT_EXTENSION_HEADER
