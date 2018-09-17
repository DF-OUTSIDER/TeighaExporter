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

#if !defined XAML_TEXT_HEADER
#define XAML_TEXT_HEADER

///
///\file        XAML/XamlText.h
///

#include "XAML/XamlCore.h"

#include "whiptk/text.h"
#include "XAML/XamlXML.h"


/** \addtogroup groupDrawable
 *  @{
 */

class XAMLTK_API WT_XAML_Text 
    : public WT_Text
    , public XamlDrawableAttributes::OriginX::Provider
    , public XamlDrawableAttributes::OriginY::Provider
    , public XamlDrawableAttributes::RenderTransform::Provider
    , public XamlDrawableAttributes::Indices::Provider
    , public XamlDrawableAttributes::UnicodeString::Provider
    , public XamlDrawableAttributes::GlyphsAttributeProvider
{
	friend class WT_XAML_Class_Factory;

public:
    enum teGhostPosition
    {
        Undefined,
        None,
        Left,
        Top,
        Right,
        Bottom
    };

protected:
    /// Constructs a WT_XAML_Text object.
    WT_XAML_Text()
		: WT_Text()
        , _fOriginX(0.0)
        , _fOriginY(0.0)
        , _fWidth(0.0)
        , _fHeight(0.0)
        , _fBase(0.0)
        , _fAscent(0.0)
        , _bIsRelativized(false)
        , _pSerializeFile(NULL)
        , _bHaveFontMetrics(false)
        , _eGhostPosition(None)
	{}

    /// Overrides default copy constructor
    WT_XAML_Text (WT_XAML_Text const & text)
		: WT_Text(static_cast<WT_Text const &>(text))
        , _fOriginX(0.0)
        , _fOriginY(0.0)
        , _fWidth(0.0)
        , _fHeight(0.0)
        , _fBase(0.0)
        , _fAscent(0.0)
        , _bIsRelativized(false)
        , _pSerializeFile(NULL)
        , _bHaveFontMetrics(false)
        , _eGhostPosition(None)
	{}   


    /// Constructs a WT_XAML_Text object with the given data.
    WT_XAML_Text(
        WT_Logical_Point const &        position, /**< Insertion point at the baseline of the text (lower left point.) */
        WT_String const &               string, /**< Text string. */
        WT_Logical_Point const *        bbox, /**< Bounding box into which text must fit (the lower left point should equal the \a position argument.) */
        WT_Unsigned_Integer16           overscore_count, /**< The number of overscore position indices in the following array argument. */
        WT_Unsigned_Integer16 const *   overscore_pos, /**< An array containing the position indicies of characters in the string which should receive overscores. */
        WT_Unsigned_Integer16           underscore_count, /**< The number of underscore position indices in the following array argument. */
        WT_Unsigned_Integer16 const *   underscore_pos /**< An array containing the position indicies of characters in the string which should receive underscores. */
        )
		: WT_Text(position,string,bbox,overscore_count,overscore_pos,underscore_count,underscore_pos)
        , _fOriginX(0.0)
        , _fOriginY(0.0)
        , _fWidth(0.0)
        , _fHeight(0.0)
        , _fBase(0.0)
        , _fAscent(0.0)
        , _bIsRelativized(false)
        , _pSerializeFile(NULL)
        , _bHaveFontMetrics(false)
        , _eGhostPosition(None)
	{}

    /// Constructs a WT_XAML_Text object with the given data.
    WT_XAML_Text(
        WT_Logical_Point const &        position, /**< Insertion point at the baseline of the text (lower left point.) */
        WT_String const &               string /**< Text string. */
        )
		: WT_Text(position,string)
        , _fOriginX(0.0)
        , _fOriginY(0.0)
        , _fWidth(0.0)
        , _fHeight(0.0)
        , _fBase(0.0)
        , _fAscent(0.0)
        , _bIsRelativized(false)
        , _pSerializeFile(NULL)
        , _bHaveFontMetrics(false)
        , _eGhostPosition(None)
	{}

    /// Destroys a WT_Text object.
    virtual ~WT_XAML_Text()
	{}

public:
	/// Overrides default assignment operator
    WT_XAML_Text & operator= (WT_Text const & other)
	{
        _fOriginX = 0.0;
        _fOriginY = 0.0;
        _fWidth = 0.0;
        _fHeight = 0.0;
        _fBase = 0.0;
        _fAscent = 0.0;
        _bIsRelativized = false;
        _pSerializeFile = NULL;
        _bHaveFontMetrics = false;
        _eGhostPosition = None;
		WT_Text::operator= (other);
		return *this;
    }
    
    /// Overrides default assignment operator
    WT_XAML_Text & operator= (WT_XAML_Text const & other)
	{
        _fOriginX = other._fOriginX;
        _fOriginY = other._fOriginY;
        _fWidth = other._fWidth;
        _fHeight = other._fHeight;
        _fBase = other._fBase;
        _fAscent = other._fAscent;
        _bIsRelativized = other._bIsRelativized;
        _pSerializeFile = other._pSerializeFile;
        _bHaveFontMetrics = other._bHaveFontMetrics;
        _eGhostPosition = other._eGhostPosition;
		WT_Text::operator= (other);
		return *this;
	}

	 //@{ \name WT_Object virtual methods
    virtual WT_Result  serialize(WT_File & /*file*/) const;
	/// partial materialization of shell from W2X
	WT_Result		 parseAttributeList(XamlXML::tAttributeMap& rMap, WT_XAML_File& rFile);
    //@}

    const teGhostPosition& ghostPosition() const { return _eGhostPosition; }
          teGhostPosition& ghostPosition()       { return _eGhostPosition; }

protected:
    virtual WT_Result provideOriginX( XamlDrawableAttributes::OriginX *& rpOriginX );
    virtual WT_Result provideOriginY( XamlDrawableAttributes::OriginY *& rpOriginY );
    virtual WT_Result provideIndices( XamlDrawableAttributes::Indices *& rpIndices  );
    virtual WT_Result provideUnicodeString( XamlDrawableAttributes::UnicodeString *& rpUnicodeString );
    virtual WT_Result provideRenderTransform( XamlDrawableAttributes::RenderTransform *& rpRenderTransform );
    virtual WT_Result provideGlyphsAttributes( XamlDrawableAttributes::GlyphsAttributeConsumer* p, WT_XAML_File& r) const;
    virtual WT_Result relativizePosition(void);


private:
    WT_Result _getFontMetrics( const WT_XAML_File &rFile ) const;
    bool _needCanvasWrapper( WT_XAML_File &rFile ) const;

private:
    float _fOriginX, _fOriginY;
    double _fWidth, _fHeight, _fBase, _fAscent;
    bool _bIsRelativized;
    WT_XAML_File *_pSerializeFile;
    bool _bHaveFontMetrics;
    teGhostPosition _eGhostPosition;
};
//@}

#endif //XAML_TEXT_HEADER
