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
#if !defined XAML_CANVAS
#define XAML_CANVAS

#include "XAML/XamlDrawable.h"
#include "XAML/XamlAttribute.h"
#include "XAML/XamlDrawableAttributes.h"


// <Canvas Opacity="0.5" RenderTransform="0.75,0,0,0.75,25,46">
// <!-- contents -->
// </Canvas>

class XamlCanvas : public XamlDrawable
                 , public XamlDrawableAttributes::CanvasAttributeConsumer
                 , public XamlDrawableAttributes::Name::Provider
                 , public XamlDrawableAttributes::RenderTransform::Provider
                 , public XamlDrawableAttributes::Clip::Provider
                 , public XamlDrawableAttributes::Opacity::Provider
                 , public XamlDrawableAttributes::OpacityMask::Provider
                 , public XamlDrawableAttributes::NavigateUri::Provider
                 , public XamlDrawableAttributes::Resources::Provider
{
public:
    class AttributeParser : public XamlDrawableAttributes::CanvasAttributeProvider
                          , public XamlDrawableAttributes::Name::Provider
                          , public XamlDrawableAttributes::RenderTransform::Provider
                          , public XamlDrawableAttributes::Clip::Provider
                          , public XamlDrawableAttributes::Opacity::Provider
                          , public XamlDrawableAttributes::OpacityMask::Provider
                          , public XamlDrawableAttributes::NavigateUri::Provider
                          , public XamlDrawableAttributes::Resources::Provider
    {
    public:
        AttributeParser( XamlXML::tAttributeMap& rMap );
        WT_Result provideCanvasAttributes(  XamlDrawableAttributes::CanvasAttributeConsumer* p, WT_XAML_File& r) const;

    protected:
        WT_Result provideName(              XamlDrawableAttributes::Name*& rp );
        WT_Result provideRenderTransform(   XamlDrawableAttributes::RenderTransform*& rp );
        WT_Result provideClip(              XamlDrawableAttributes::Clip*& rp );
        WT_Result provideOpacity(           XamlDrawableAttributes::Opacity*& rp );
        WT_Result provideOpacityMask(       XamlDrawableAttributes::OpacityMask*& rp );
        WT_Result provideNavigateUri(       XamlDrawableAttributes::NavigateUri*& rp );
        WT_Result provideResources(         XamlDrawableAttributes::Resources*& rp );
        
    private:
        AttributeParser& operator=( const AttributeParser& ) { return *this; }
        XamlXML::tAttributeMap& _rMap;
        WT_XAML_File * _pFile;
    };

public:

    //
    // \param bNested Indicates that serializing this canvas should not close a previous canvas
    //
    XamlCanvas( bool bNesting=true );
    virtual ~XamlCanvas();

    virtual WTX_ID xobject_id() const { return Canvas_ID; }

    virtual WT_Result serialize(WT_File &) const;
    virtual WT_Result dump(WT_File &) const;

public:
    const XamlDrawableAttributes::Name&             name()          { return _oName; }
    const XamlDrawableAttributes::RenderTransform&  renderTransform()   { return _oRenderTransform; }
    const XamlDrawableAttributes::Clip&             clip()          { return _oClip; }
    const XamlDrawableAttributes::Opacity&          opacity()       { return _oOpacity; }
    const XamlDrawableAttributes::OpacityMask&      opacityMask()   { return _oOpacityMask; }
    const XamlDrawableAttributes::NavigateUri&      navigateUri()   { return _oNavigateUri; }
    const XamlDrawableAttributes::Resources&        resources()     { return _oResources; }

protected :

    virtual WT_Result consumeName(              XamlDrawableAttributes::Name::Provider *);
    virtual WT_Result consumeRenderTransform(   XamlDrawableAttributes::RenderTransform::Provider *);
    virtual WT_Result consumeClip(              XamlDrawableAttributes::Clip::Provider *);
    virtual WT_Result consumeOpacity(           XamlDrawableAttributes::Opacity::Provider *);
    virtual WT_Result consumeOpacityMask(       XamlDrawableAttributes::OpacityMask::Provider *);
    virtual WT_Result consumeNavigateUri(       XamlDrawableAttributes::NavigateUri::Provider *);
    virtual WT_Result consumeResources(         XamlDrawableAttributes::Resources::Provider *);

    virtual WT_Result provideName(              XamlDrawableAttributes::Name*& rp );
    virtual WT_Result provideRenderTransform(   XamlDrawableAttributes::RenderTransform*& rp );
    virtual WT_Result provideClip(              XamlDrawableAttributes::Clip*& rp );
    virtual WT_Result provideOpacity(           XamlDrawableAttributes::Opacity*& rp );
    virtual WT_Result provideOpacityMask(       XamlDrawableAttributes::OpacityMask*& rp );
    virtual WT_Result provideNavigateUri(       XamlDrawableAttributes::NavigateUri*& rp );
    virtual WT_Result provideResources(         XamlDrawableAttributes::Resources*& rp );

protected :

    XamlDrawableAttributes::Name                _oName;
    XamlDrawableAttributes::RenderTransform     _oRenderTransform;
    XamlDrawableAttributes::Clip                _oClip;
    XamlDrawableAttributes::Opacity             _oOpacity;
    XamlDrawableAttributes::OpacityMask         _oOpacityMask;
    XamlDrawableAttributes::NavigateUri         _oNavigateUri;
    XamlDrawableAttributes::Resources           _oResources;
    bool                                        _bNesting;
};

#endif //XAML_CANVAS
