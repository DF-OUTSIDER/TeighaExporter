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
#if !defined XAML_PATH
#define XAML_PATH

#include "XAML/XamlDrawable.h"
#include "XAML/XamlAttribute.h"
#include "XAML/XamlDrawableAttributes.h"
#include "XAML/XamlXML.h"

// absolute paths:
// <Path Data="F1 M 136.16,978.08 L 136.8,978.08 136.8,978.72 136.16,978.72 z" Fill="#ff999999" />

// <Path Data="M 144,120.8 L 719.36,120.8 719.36,910.4 144,910.4 z">
//      <Path.Fill>
//          <ImageBrush ImageSource="/Documents/1/Resources/Images/10.PNG" Viewbox="0,0,799.099,1096.56" TileMode="None" ViewboxUnits="Absolute" ViewportUnits="Absolute" Viewport="144,120.8,575.36,789.6" />
//      </Path.Fill>
// </Path>

// relative paths:

class XamlGraphicsObject;

class XamlPath : public XamlDrawable
               , public XamlDrawableAttributes::PathAttributeConsumer
               , public XamlDrawableAttributes::Data::Provider
               , public XamlDrawableAttributes::Name::Provider
               , public XamlDrawableAttributes::Fill::Provider
               , public XamlDrawableAttributes::RenderTransform::Provider
               , public XamlDrawableAttributes::Clip::Provider
               , public XamlDrawableAttributes::Opacity::Provider
               , public XamlDrawableAttributes::OpacityMask::Provider
               , public XamlDrawableAttributes::Stroke::Provider
               , public XamlDrawableAttributes::StrokeDashArray::Provider
               , public XamlDrawableAttributes::StrokeDashCap::Provider
               , public XamlDrawableAttributes::StrokeDashOffset::Provider
               , public XamlDrawableAttributes::StrokeEndLineCap::Provider
               , public XamlDrawableAttributes::StrokeStartLineCap::Provider
               , public XamlDrawableAttributes::StrokeLineJoin::Provider
               , public XamlDrawableAttributes::StrokeMiterLimit::Provider
               , public XamlDrawableAttributes::StrokeThickness::Provider
               , public XamlDrawableAttributes::NavigateUri::Provider
{
public:

    class AttributeParser : public XamlDrawableAttributes::PathAttributeProvider
                          , public XamlDrawableAttributes::Data::Provider
                          , public XamlDrawableAttributes::Name::Provider
                          , public XamlDrawableAttributes::Fill::Provider
                          , public XamlDrawableAttributes::RenderTransform::Provider
                          , public XamlDrawableAttributes::Clip::Provider
                          , public XamlDrawableAttributes::Opacity::Provider
                          , public XamlDrawableAttributes::OpacityMask::Provider
                          , public XamlDrawableAttributes::Stroke::Provider
                          , public XamlDrawableAttributes::StrokeDashArray::Provider
                          , public XamlDrawableAttributes::StrokeDashCap::Provider
                          , public XamlDrawableAttributes::StrokeDashOffset::Provider
                          , public XamlDrawableAttributes::StrokeEndLineCap::Provider
                          , public XamlDrawableAttributes::StrokeStartLineCap::Provider
                          , public XamlDrawableAttributes::StrokeLineJoin::Provider
                          , public XamlDrawableAttributes::StrokeMiterLimit::Provider
                          , public XamlDrawableAttributes::StrokeThickness::Provider
                          , public XamlDrawableAttributes::NavigateUri::Provider
    {
    public:
        AttributeParser( XamlXML::tAttributeMap& rMap );
        WT_Result providePathAttributes(    XamlDrawableAttributes::PathAttributeConsumer* p, WT_XAML_File& r) const;

    protected:
        WT_Result provideData(              XamlDrawableAttributes::Data*& rp );
        WT_Result provideName(              XamlDrawableAttributes::Name*& rp );
        WT_Result provideFill(              XamlDrawableAttributes::Fill*& rp );
        WT_Result provideRenderTransform(   XamlDrawableAttributes::RenderTransform*& rp );
        WT_Result provideClip(              XamlDrawableAttributes::Clip*& rp );
        WT_Result provideOpacity(           XamlDrawableAttributes::Opacity*& rp );
        WT_Result provideOpacityMask(       XamlDrawableAttributes::OpacityMask*& rp );
        WT_Result provideStroke(            XamlDrawableAttributes::Stroke*& rp );
        WT_Result provideStrokeDashArray(   XamlDrawableAttributes::StrokeDashArray*& rp );
        WT_Result provideStrokeDashCap(     XamlDrawableAttributes::StrokeDashCap*& rp );
        WT_Result provideStrokeDashOffset(  XamlDrawableAttributes::StrokeDashOffset*& rp );
        WT_Result provideStrokeEndLineCap(  XamlDrawableAttributes::StrokeEndLineCap*& rp );
        WT_Result provideStrokeStartLineCap(XamlDrawableAttributes::StrokeStartLineCap*& rp );
        WT_Result provideStrokeLineJoin(    XamlDrawableAttributes::StrokeLineJoin*& rp );
        WT_Result provideStrokeMiterLimit(  XamlDrawableAttributes::StrokeMiterLimit*& rp );
        WT_Result provideStrokeThickness(   XamlDrawableAttributes::StrokeThickness*& rp );
        WT_Result provideNavigateUri(       XamlDrawableAttributes::NavigateUri*& rp );
        
    private:
        AttributeParser& operator=( const AttributeParser& ) { return *this; }
        XamlXML::tAttributeMap& _rMap;
        WT_XAML_File * _pFile;
    };
		
public:
    XamlPath();
    XamlPath(const XamlPath& s);
    virtual ~XamlPath(void);
    
    XamlPath& operator=(const XamlPath& s);

    virtual WTX_ID     xobject_id() const { return Path_ID; }

    void addDrawable(XamlGraphicsObject* pXamlDrawable);

    virtual WT_Result   serialize(WT_File & file) const;
    virtual WT_Result   delay(WT_File & file) const;
    virtual WT_Result   dump(WT_File & file) const;
    virtual bool  merge(XamlPath const & new_one);

    //Path.Data
    //     Fill                  (fill, fill pattern, hatch pattern, color)
    //     RenderTransform       (viewport)
    //     Clip                  (viewport) 
    //     Opacity               (visibility, pen pattern)
    //     OpacityMask           (pen pattern)
    //     Stroke                (color / color map)
    //     StrokeDashArray       (dash pattern, line pattern)
    //     StrokeDashCap         (line style)
    //     StrokeDashOffset      (line style)
    //     StrokeEndLineCap      (line style)
    //     StrokeStartLineCap    (line style)
    //     StrokeLineJoin        (line style)
    //     StrokeMiterLimit      (line style)
    //     StrokeThickness       (line weight)
    //     FixedPage.NavigateUri (URL)

public:
    const XamlDrawableAttributes::Data&                 data()                  { return _oData; }
    const XamlDrawableAttributes::Name&                 name()                  { return _oName; }
    const XamlDrawableAttributes::Fill&                 fill()                  { return _oFill; }
    const XamlDrawableAttributes::RenderTransform&      renderTransform()       { return _oRenderTransform; }
    const XamlDrawableAttributes::Clip&                 clip()                  { return _oClip; }
    const XamlDrawableAttributes::Opacity&              opacity()               { return _oOpacity; }
    const XamlDrawableAttributes::OpacityMask&          opacityMask()           { return _oOpacityMask; }
    const XamlDrawableAttributes::Stroke&               stroke()                { return _oStroke; }
    const XamlDrawableAttributes::StrokeDashArray&      strokeDashArray()       { return _oStrokeDashArray; }
    const XamlDrawableAttributes::StrokeDashCap&        strokeDashCap()         { return _oStrokeDashCap; }
    const XamlDrawableAttributes::StrokeDashOffset&     strokeDashOffset()      { return _oStrokeDashOffset; }
    const XamlDrawableAttributes::StrokeEndLineCap&     strokeEndLineCap()      { return _oStrokeEndLineCap; }
    const XamlDrawableAttributes::StrokeStartLineCap&   strokeStartLineCap()    { return _oStrokeStartLineCap; }
    const XamlDrawableAttributes::StrokeLineJoin&       strokeLineJoin()        { return _oStrokeLineJoin; }
    const XamlDrawableAttributes::StrokeMiterLimit&     strokeMiterLimit()      { return _oStrokeMiterLimit; }
    const XamlDrawableAttributes::StrokeThickness&      strokeThickness()       { return _oStrokeThickness; }
    const XamlDrawableAttributes::NavigateUri&          navigateUri()           { return _oNavigateUri; }

protected:

    virtual WT_Result consumeData(              XamlDrawableAttributes::Data::Provider *p );
    virtual WT_Result consumeName(              XamlDrawableAttributes::Name::Provider *p );
    virtual WT_Result consumeFill(              XamlDrawableAttributes::Fill::Provider *p );
    virtual WT_Result consumeRenderTransform(   XamlDrawableAttributes::RenderTransform::Provider *p );
    virtual WT_Result consumeClip(              XamlDrawableAttributes::Clip::Provider *p );
    virtual WT_Result consumeOpacity(           XamlDrawableAttributes::Opacity::Provider *p );
    virtual WT_Result consumeOpacityMask(       XamlDrawableAttributes::OpacityMask::Provider *p );
    virtual WT_Result consumeStroke(            XamlDrawableAttributes::Stroke::Provider *p );
    virtual WT_Result consumeStrokeDashArray(   XamlDrawableAttributes::StrokeDashArray::Provider *p );
    virtual WT_Result consumeStrokeDashCap(     XamlDrawableAttributes::StrokeDashCap::Provider *p );
    virtual WT_Result consumeStrokeDashOffset(  XamlDrawableAttributes::StrokeDashOffset::Provider *p );
    virtual WT_Result consumeStrokeEndLineCap(  XamlDrawableAttributes::StrokeEndLineCap::Provider *p );
    virtual WT_Result consumeStrokeStartLineCap( XamlDrawableAttributes::StrokeStartLineCap::Provider *p );
    virtual WT_Result consumeStrokeLineJoin(    XamlDrawableAttributes::StrokeLineJoin::Provider *p );
    virtual WT_Result consumeStrokeMiterLimit(  XamlDrawableAttributes::StrokeMiterLimit::Provider *p );
    virtual WT_Result consumeStrokeThickness(   XamlDrawableAttributes::StrokeThickness::Provider *p );
    virtual WT_Result consumeNavigateUri(       XamlDrawableAttributes::NavigateUri::Provider *p );

    virtual WT_Result provideData(              XamlDrawableAttributes::Data*& rp );
    virtual WT_Result provideName(              XamlDrawableAttributes::Name*& rp );
    virtual WT_Result provideFill(              XamlDrawableAttributes::Fill*& rp );
    virtual WT_Result provideRenderTransform(   XamlDrawableAttributes::RenderTransform*& rp );
    virtual WT_Result provideClip(              XamlDrawableAttributes::Clip*& rp );
    virtual WT_Result provideOpacity(           XamlDrawableAttributes::Opacity*& rp );
    virtual WT_Result provideOpacityMask(       XamlDrawableAttributes::OpacityMask*& rp );
    virtual WT_Result provideStroke(            XamlDrawableAttributes::Stroke*& rp );
    virtual WT_Result provideStrokeDashArray(   XamlDrawableAttributes::StrokeDashArray*& rp );
    virtual WT_Result provideStrokeDashCap(     XamlDrawableAttributes::StrokeDashCap*& rp );
    virtual WT_Result provideStrokeDashOffset(  XamlDrawableAttributes::StrokeDashOffset*& rp );
    virtual WT_Result provideStrokeEndLineCap(  XamlDrawableAttributes::StrokeEndLineCap*& rp );
    virtual WT_Result provideStrokeStartLineCap(XamlDrawableAttributes::StrokeStartLineCap*& rp );
    virtual WT_Result provideStrokeLineJoin(    XamlDrawableAttributes::StrokeLineJoin*& rp );
    virtual WT_Result provideStrokeMiterLimit(  XamlDrawableAttributes::StrokeMiterLimit*& rp );
    virtual WT_Result provideStrokeThickness(   XamlDrawableAttributes::StrokeThickness*& rp );
    virtual WT_Result provideNavigateUri(       XamlDrawableAttributes::NavigateUri*& rp );

protected:
	
	XamlDrawableAttributes::Data                _oData;
	XamlDrawableAttributes::Name                _oName;
    XamlDrawableAttributes::Fill                _oFill;
    XamlDrawableAttributes::RenderTransform     _oRenderTransform;
    XamlDrawableAttributes::Clip                _oClip;
    XamlDrawableAttributes::Opacity             _oOpacity;
    XamlDrawableAttributes::OpacityMask         _oOpacityMask;
    XamlDrawableAttributes::Stroke              _oStroke;
    XamlDrawableAttributes::StrokeDashArray     _oStrokeDashArray;
    XamlDrawableAttributes::StrokeDashCap       _oStrokeDashCap;
    XamlDrawableAttributes::StrokeDashOffset    _oStrokeDashOffset;
    XamlDrawableAttributes::StrokeEndLineCap    _oStrokeEndLineCap;
    XamlDrawableAttributes::StrokeStartLineCap  _oStrokeStartLineCap;
    XamlDrawableAttributes::StrokeLineJoin      _oStrokeLineJoin;
    XamlDrawableAttributes::StrokeMiterLimit    _oStrokeMiterLimit;
    XamlDrawableAttributes::StrokeThickness     _oStrokeThickness;
    XamlDrawableAttributes::NavigateUri         _oNavigateUri;
};


#endif //XAML_PATH
