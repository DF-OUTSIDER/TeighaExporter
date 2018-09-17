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

#if !defined XAML_URL_HEADER
#define XAML_URL_HEADER

///
///\file        XAML/XamlURL.h
///

#include "XAML/XamlCore.h"

#include "whiptk/url.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

/** \addtogroup groupRenditionAttributes
 *  @{
 */

/// Rendition attribute describing the current single or multiple-target hyperlink to be applied to subsequent geometry.
/** \note Clients should call clear() when done adding linked geometry.
 *  \sa WT_Rendition
 *
 */
class XAMLTK_API WT_XAML_URL : public WT_URL
							 , public XamlDrawableAttributes::NavigateUri::Provider
							 , public XamlDrawableAttributes::PathAttributeProvider
							 , public XamlDrawableAttributes::GlyphsAttributeProvider
{
	friend class WT_XAML_Class_Factory;
	friend class WT_XAML_Rendition;

protected:

    //@{ \name Construction
    /// Constructs a WT_XAML_URL object.
    WT_XAML_URL()
      : WT_URL()
      , _bXamlUrlEnabled( true )
     { }

    /// Constructs a WT_XAML_URL object with the given initial URL item.
    WT_XAML_URL(WT_URL_Item  &item)
		: WT_URL(item)
        , _bXamlUrlEnabled( true )
	{}

    /// Constructs a WT_XAML_URL object with the given initial hyperlink data.
    WT_XAML_URL(
        WT_Integer32 index, /**< Unique index for this hyperlink. */
        WT_Unsigned_Integer16 const * address, /**< Machine-readable target for this hyperlink. */
        WT_Unsigned_Integer16 const * friendly_name /**< Human-readable friendly name for this hyperlink. */
        )
		: WT_URL(index,address,friendly_name)
        , _bXamlUrlEnabled( true )
	{}

	///Copy Constructor
	WT_XAML_URL(WT_URL const& url)
		:WT_URL(url)
        , _bXamlUrlEnabled( true )
	{}

	///Copy Constructor
	WT_XAML_URL(WT_XAML_URL const& url)
        : _bXamlUrlEnabled( true )
	{
        *this = url;
    }

	virtual ~WT_XAML_URL()
	{}
    //@}

	WT_Result serializeUrlList(WT_XAML_File& rFile);

public:
	  /// Assignment operator.  Copies the contents of the given WT_URL object to this one.
    WT_XAML_URL const & operator=(WT_XAML_URL const & url)
	{
        _szXamlNavigateUri = url._szXamlNavigateUri;
        _bXamlUrlEnabled = url._bXamlUrlEnabled;
		WT_URL::operator= (url);
		return *this;
	}

    DWFString const& xamlURL() const
    {   return _szXamlNavigateUri;   }

    DWFString&       xamlURL()
    {   return _szXamlNavigateUri;   }

    const bool& xamlUrlEnabled() const
    { return _bXamlUrlEnabled; }

    bool& xamlUrlEnabled()
    { return _bXamlUrlEnabled; }

    virtual void clear()
    {
        if(_szXamlNavigateUri.bytes())
            _szXamlNavigateUri.destroy();

        WT_URL::clear();
    }

    virtual WT_Result providePathAttributes( XamlDrawableAttributes::PathAttributeConsumer*, WT_XAML_File& ) const;
	virtual WT_Result provideGlyphsAttributes( XamlDrawableAttributes::GlyphsAttributeConsumer* pConsumer, WT_XAML_File& r) const;

	virtual WT_Result provideNavigateUri(XamlDrawableAttributes::NavigateUri* & rpNavigateUri);

	//@{ \name WT_Object and WT_Attribute virtual methods
    WT_Result        serialize (WT_File & file) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}
private:
    DWFString       _szXamlNavigateUri;
    bool            _bXamlUrlEnabled;
};
//@}

#endif //XAML_URL_HEADER
