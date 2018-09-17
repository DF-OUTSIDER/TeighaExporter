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
#if !defined XAML_GLYPHS
#define XAML_GLYPHS

#include "XAML/XamlObject.h"


//<Glyphs Fill="#ff000000" 
//        FontUri="/Documents/1/Resources/Fonts/98595B54-4534-483C-AE20-209D88A14C32.odttf" 
//        FontRenderingEmSize="10.6997" 
//        StyleSimulations="None" 
//        OriginX="144" 
//        OriginY="58.88" 
//        Indices="59,72;51,61;54,72;3,37;54,71;83,64;72,63;70,55;76,28;73,39;76,29;70,55;68,63;87,42;76,28;82,64;81,65;3,39;68,61;81,66;71,64;3,37;53,72;72,61;73,39;72,62;85,44;72,62;81,66;70,55;72,61;3,38;42,80;88,66;76,29;71,65;72,59" 
//        UnicodeString="XPS Specification and Reference Guide" />

class XamlGlyphs : public XamlDrawable
                 , public XamlDrawableAttributes::GlyphsAttributeConsumer
                 , public XamlDrawableAttributes::BidiLevel::Provider
                 , public XamlDrawableAttributes::CaretStops::Provider
                 , public XamlDrawableAttributes::DeviceFontName::Provider
                 , public XamlDrawableAttributes::Fill::Provider
                 , public XamlDrawableAttributes::FontRenderingEmSize::Provider
                 , public XamlDrawableAttributes::FontUri::Provider
                 , public XamlDrawableAttributes::OriginX::Provider
                 , public XamlDrawableAttributes::OriginY::Provider
                 , public XamlDrawableAttributes::Indices::Provider
                 , public XamlDrawableAttributes::IsSideways::Provider
                 , public XamlDrawableAttributes::UnicodeString::Provider
                 , public XamlDrawableAttributes::StyleSimulations::Provider
                 , public XamlDrawableAttributes::RenderTransform::Provider
                 , public XamlDrawableAttributes::Clip::Provider
                 , public XamlDrawableAttributes::Opacity::Provider
                 , public XamlDrawableAttributes::OpacityMask::Provider
                 , public XamlDrawableAttributes::Name::Provider
                 , public XamlDrawableAttributes::NavigateUri::Provider
{
public:
    class AttributeParser : public XamlDrawableAttributes::GlyphsAttributeProvider
                          , public XamlDrawableAttributes::BidiLevel::Provider
                          , public XamlDrawableAttributes::CaretStops::Provider
                          , public XamlDrawableAttributes::DeviceFontName::Provider
                          , public XamlDrawableAttributes::Fill::Provider
                          , public XamlDrawableAttributes::FontRenderingEmSize::Provider
                          , public XamlDrawableAttributes::FontUri::Provider
                          , public XamlDrawableAttributes::OriginX::Provider
                          , public XamlDrawableAttributes::OriginY::Provider
                          , public XamlDrawableAttributes::Indices::Provider
                          , public XamlDrawableAttributes::IsSideways::Provider
                          , public XamlDrawableAttributes::UnicodeString::Provider
                          , public XamlDrawableAttributes::StyleSimulations::Provider
                          , public XamlDrawableAttributes::RenderTransform::Provider
                          , public XamlDrawableAttributes::Clip::Provider
                          , public XamlDrawableAttributes::Opacity::Provider
                          , public XamlDrawableAttributes::OpacityMask::Provider
                          , public XamlDrawableAttributes::Name::Provider
                          , public XamlDrawableAttributes::NavigateUri::Provider
    {
    public:
        AttributeParser( XamlXML::tAttributeMap& rMap );
        WT_Result provideGlyphsAttributes(      XamlDrawableAttributes::GlyphsAttributeConsumer* p, WT_XAML_File& r) const;

    protected:
        WT_Result provideBidiLevel(             XamlDrawableAttributes::BidiLevel*& rp );
        WT_Result provideCaretStops(            XamlDrawableAttributes::CaretStops*& rp );
        WT_Result provideDeviceFontName(        XamlDrawableAttributes::DeviceFontName*& rp );
        WT_Result provideFill(                  XamlDrawableAttributes::Fill*& rp );
        WT_Result provideFontRenderingEmSize(   XamlDrawableAttributes::FontRenderingEmSize*& rp );
        WT_Result provideFontUri(               XamlDrawableAttributes::FontUri*& rp );
        WT_Result provideOriginX(               XamlDrawableAttributes::OriginX*& rp );
        WT_Result provideOriginY(               XamlDrawableAttributes::OriginY*& rp );
        WT_Result provideIndices(               XamlDrawableAttributes::Indices*& rp );
        WT_Result provideIsSideways(            XamlDrawableAttributes::IsSideways*& rp );
        WT_Result provideUnicodeString(         XamlDrawableAttributes::UnicodeString*& rp );
        WT_Result provideStyleSimulations(      XamlDrawableAttributes::StyleSimulations*& rp );
        WT_Result provideRenderTransform(       XamlDrawableAttributes::RenderTransform*& rp );
        WT_Result provideClip(                  XamlDrawableAttributes::Clip*& rp );
        WT_Result provideOpacity(               XamlDrawableAttributes::Opacity*& rp );
        WT_Result provideOpacityMask(           XamlDrawableAttributes::OpacityMask*& rp );
        WT_Result provideName(                  XamlDrawableAttributes::Name*& rp );
        WT_Result provideNavigateUri(           XamlDrawableAttributes::NavigateUri*& rp );
        
    private:
        AttributeParser& operator=( const AttributeParser& ) { return *this; }
        XamlXML::tAttributeMap& _rMap;
        WT_XAML_File * _pFile;
    };


public:
    XamlGlyphs();
    virtual ~XamlGlyphs(void);

    XamlGlyphs& operator=(const XamlGlyphs& r);

    virtual WTX_ID      xobject_id() const { return Glyphs_ID; }
    virtual WT_Result   serialize(WT_File & file) const;

public:
    const XamlDrawableAttributes::BidiLevel&            bidiLevel()             { return _oBidiLevel; }
    const XamlDrawableAttributes::CaretStops&           caretStops()            { return _oCaretStops; }
    const XamlDrawableAttributes::DeviceFontName&       deviceFontName()        { return _oDeviceFontName; }
    const XamlDrawableAttributes::Fill&                 fill()                  { return _oFill; }
    const XamlDrawableAttributes::FontRenderingEmSize&  fontRenderingEmSize()   { return _oFontRenderingEmSize; }
    const XamlDrawableAttributes::FontUri&              fontUri()               { return _oFontUri; }
    const XamlDrawableAttributes::OriginX&              originX()               { return _oOriginX; }
    const XamlDrawableAttributes::OriginY&              originY()               { return _oOriginY; }
    const XamlDrawableAttributes::IsSideways&           isSideways()            { return _oIsSideways; }
    const XamlDrawableAttributes::Indices&              indices()               { return _oIndices; }
    const XamlDrawableAttributes::UnicodeString&        unicodeString()         { return _oUnicodeString; }
    const XamlDrawableAttributes::StyleSimulations&     styleSimulations()      { return _oStyleSimulations; }
    const XamlDrawableAttributes::RenderTransform&      renderTransform()       { return _oRenderTransform; }
    const XamlDrawableAttributes::Clip&                 clip()                  { return _oClip; }
    const XamlDrawableAttributes::Opacity&              opacity()               { return _oOpacity; }
    const XamlDrawableAttributes::OpacityMask&          opacityMask()           { return _oOpacityMask; }
    const XamlDrawableAttributes::Name&                 name()                  { return _oName; }
    const XamlDrawableAttributes::NavigateUri&          navigateUri()           { return _oNavigateUri; }

protected:
    virtual WT_Result consumeBidiLevel          ( XamlDrawableAttributes::BidiLevel::Provider* );
    virtual WT_Result consumeCaretStops         ( XamlDrawableAttributes::CaretStops::Provider* );
    virtual WT_Result consumeDeviceFontName     ( XamlDrawableAttributes::DeviceFontName::Provider* );
    virtual WT_Result consumeFill               ( XamlDrawableAttributes::Fill::Provider* );
    virtual WT_Result consumeFontRenderingEmSize( XamlDrawableAttributes::FontRenderingEmSize::Provider* );
    virtual WT_Result consumeFontUri            ( XamlDrawableAttributes::FontUri::Provider* );
    virtual WT_Result consumeOriginX            ( XamlDrawableAttributes::OriginX::Provider* );
    virtual WT_Result consumeOriginY            ( XamlDrawableAttributes::OriginY::Provider* );
    virtual WT_Result consumeIndices            ( XamlDrawableAttributes::Indices::Provider* );
    virtual WT_Result consumeIsSideways         ( XamlDrawableAttributes::IsSideways::Provider* );
    virtual WT_Result consumeUnicodeString      ( XamlDrawableAttributes::UnicodeString::Provider* );
    virtual WT_Result consumeStyleSimulations   ( XamlDrawableAttributes::StyleSimulations::Provider* );
    virtual WT_Result consumeRenderTransform    ( XamlDrawableAttributes::RenderTransform::Provider* );
    virtual WT_Result consumeClip               ( XamlDrawableAttributes::Clip::Provider* );
    virtual WT_Result consumeOpacity            ( XamlDrawableAttributes::Opacity::Provider* );
    virtual WT_Result consumeOpacityMask        ( XamlDrawableAttributes::OpacityMask::Provider* );
    virtual WT_Result consumeName               ( XamlDrawableAttributes::Name::Provider* );
    virtual WT_Result consumeNavigateUri        ( XamlDrawableAttributes::NavigateUri::Provider* );

    virtual WT_Result provideBidiLevel          ( XamlDrawableAttributes::BidiLevel*& rp );
    virtual WT_Result provideCaretStops         ( XamlDrawableAttributes::CaretStops*& rp );
    virtual WT_Result provideDeviceFontName     ( XamlDrawableAttributes::DeviceFontName*& rp );
    virtual WT_Result provideFill               ( XamlDrawableAttributes::Fill*& rp );
    virtual WT_Result provideFontRenderingEmSize( XamlDrawableAttributes::FontRenderingEmSize*& rp );
    virtual WT_Result provideFontUri            ( XamlDrawableAttributes::FontUri*& rp );
    virtual WT_Result provideOriginX            ( XamlDrawableAttributes::OriginX*& rp );
    virtual WT_Result provideOriginY            ( XamlDrawableAttributes::OriginY*& rp );
    virtual WT_Result provideIndices            ( XamlDrawableAttributes::Indices*& rp );
    virtual WT_Result provideIsSideways         ( XamlDrawableAttributes::IsSideways*& rp );
    virtual WT_Result provideUnicodeString      ( XamlDrawableAttributes::UnicodeString*& rp );
    virtual WT_Result provideStyleSimulations   ( XamlDrawableAttributes::StyleSimulations*& rp );
    virtual WT_Result provideRenderTransform    ( XamlDrawableAttributes::RenderTransform*& rp );
    virtual WT_Result provideClip               ( XamlDrawableAttributes::Clip*& rp );
    virtual WT_Result provideOpacity            ( XamlDrawableAttributes::Opacity*& rp );
    virtual WT_Result provideOpacityMask        ( XamlDrawableAttributes::OpacityMask*& rp );
    virtual WT_Result provideName               ( XamlDrawableAttributes::Name*& rp );
    virtual WT_Result provideNavigateUri        ( XamlDrawableAttributes::NavigateUri*& rp );

    XamlDrawableAttributes::BidiLevel           _oBidiLevel;
    XamlDrawableAttributes::CaretStops          _oCaretStops;
    XamlDrawableAttributes::DeviceFontName      _oDeviceFontName;
    XamlDrawableAttributes::Fill                _oFill;
    XamlDrawableAttributes::FontRenderingEmSize _oFontRenderingEmSize;
    XamlDrawableAttributes::FontUri             _oFontUri;
    XamlDrawableAttributes::OriginX             _oOriginX;
    XamlDrawableAttributes::OriginY             _oOriginY;
    XamlDrawableAttributes::IsSideways          _oIsSideways;
    XamlDrawableAttributes::Indices             _oIndices;
    XamlDrawableAttributes::UnicodeString       _oUnicodeString;
    XamlDrawableAttributes::StyleSimulations    _oStyleSimulations;
    XamlDrawableAttributes::RenderTransform     _oRenderTransform;
    XamlDrawableAttributes::Clip                _oClip;
    XamlDrawableAttributes::Opacity             _oOpacity;
    XamlDrawableAttributes::OpacityMask         _oOpacityMask;
    XamlDrawableAttributes::Name                _oName;
    XamlDrawableAttributes::NavigateUri         _oNavigateUri;
};

#endif //XAML_GLYPHS
