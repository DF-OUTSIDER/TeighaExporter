/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SvgExport.h"
#include "SvgWrappers.h"
#include "SvgShxEmbedding.h"
#include "Gi/GiSpatialFilter.h"
#include "Gi/GiHLRemover.h"
#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbBaseDatabase.h"
#include "../../2dExport/Include/2dExportDevice.h"
#include "DbHyperlink.h"
#include "Gi/GiRasterWrappers.h"
#include "DynamicLinker.h"

#define STL_USING_STACK
#include "OdaSTL.h"

using namespace TD_2D_EXPORT;
namespace TD_SVG_EXPORT {

using namespace xml;

// for old compilers having problems with Koenig namespace lookup
#if !defined(_MSC_VER) && !defined (__BCPLUSPLUS__) && !defined(ODA_GCC)
using namespace xml;
#endif
namespace xml
{
  int Attribute::_doublePrecision = 6;
}
class OdSvgDevice;

class OdSvgView : public Od2dExportView
{
  virtual void beginViewVectorization();
  virtual void endViewVectorization();
public:
  OdSvgView()
  {
  }

  OdSvgDevice* device();

  void setOuterClipPath();

  static OdGsViewPtr createObject()
  {
    return OdRxObjectImpl<OdSvgView, OdGsView>::createObject();
  }

  virtual void onTraitsModified();
  void draw(const OdGiDrawable* pDrawable);

  TD_USING(Od2dExportView::update);
  virtual void shell(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData) ODRX_OVERRIDE;
};

class SvgProperties;
typedef OdSmartPtr<SvgProperties> SvgPropertiesPtr;

class SvgProperties : public OdRxDispatchImpl<>
{
  OdStreamBufPtr _stream;
  double _tolerance;
  double _lwScale;
  OdString _imageBasePath;
  OdString _imageURL;
  double _shxLineWeight;
  OdString _defImageExt;
  ODCOLORREF _bkColor;
  OdString _genericFontFamily;
  bool _bUseHLR;
  bool _bEnableGouraudShading;
  bool _bExplodeShxFonts;
  bool _bRemoveFirstSpacesOfText;
public:
  SvgProperties() 
    : _tolerance( 0.5 )
    , _lwScale( 0.5 )
    , _shxLineWeight( 0.05 )
    , _defImageExt( OD_T("png") )
    , _bkColor( ODRGB( 0xff, 0xff, 0xff ) )
    , _genericFontFamily( OD_T("sans-serif") )
    , _bUseHLR(false)
    , _bEnableGouraudShading(true)
    , _bExplodeShxFonts(true)
    , _bRemoveFirstSpacesOfText(true)
  {}
  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(SvgProperties);
  static SvgPropertiesPtr createObject()
  {
    return OdRxObjectImpl<SvgProperties, OdRxDictionary>::createObject();
  }
  OdInt32 get_Precision() const { return xml::Attribute::_doublePrecision; }
  void put_Precision( OdInt32 p )
  {
    if ( p < 1 || p > 20 ) 
      p = 6;
    xml::Attribute::_doublePrecision = p;
  }
  double get_Tolerance() const { return _tolerance; }
  void put_Tolerance( double tol ){ _tolerance = tol; }
  OdRxObjectPtr get_Output() const { return OdRxObject::cast(_stream); }
  void put_Output( OdRxObject* obj ){ _stream = obj; }
  double get_LineWeightScale() const { return _lwScale; }
  void put_LineWeightScale( double lws ){ _lwScale = lws; }
  const OdString& get_ImageBase() const { return _imageBasePath; }
  void put_ImageBase( const OdString& path ) 
  { 
    _imageBasePath = path; 
    int n = _imageBasePath.getLength();
    OdChar last = n ? _imageBasePath[ n - 1 ] : 0;
    if ( last != L'\\' && last != L'/' ) _imageBasePath += L'/';
  }
  const OdString& get_ImageUrl() const { return _imageURL; }
  void put_ImageUrl( const OdString& url ) { _imageURL = url; }
  double get_ShxLineWeight(){ return _shxLineWeight; }
  void put_ShxLineWeight( double lw ) { _shxLineWeight = lw; }
  const OdString& get_DefaultImageExt() const { return _defImageExt; }
  void put_DefaultImageExt( const OdString& ext )
  { 
    if ( ext[0] != '.' ) _defImageExt = OdString(OD_T(".")) + ext;
    else _defImageExt = ext;
  }
  ODCOLORREF get_BackGround() const { return _bkColor; }
  void put_BackGround( ODCOLORREF clr ) { _bkColor = clr; }
  OdString get_GenericFontFamily() const { return _genericFontFamily; }
  void put_GenericFontFamily( const OdString& s ){ _genericFontFamily = s;}
  bool get_UseHLR() const { return _bUseHLR; }
  void put_UseHLR(bool bUseHLR) { _bUseHLR = bUseHLR; }
  bool get_EnableGouraudShading(){ return _bEnableGouraudShading; }
  void put_EnableGouraudShading(bool b){ _bEnableGouraudShading = b; }
  bool get_ExplodeShxTexts(){ return _bExplodeShxFonts; }
  void put_ExplodeShxTexts(bool b){ _bExplodeShxFonts = b; }
  bool get_RemoveFirstSpacesOfText(){ return _bRemoveFirstSpacesOfText; }
  void put_RemoveFirstSpacesOfText(bool b){ _bRemoveFirstSpacesOfText = b; }
};

ODRX_DECLARE_PROPERTY(DefaultImageExt)
ODRX_DECLARE_PROPERTY(ShxLineWeight)
ODRX_DECLARE_PROPERTY(Tolerance)
ODRX_DECLARE_PROPERTY(LineWeightScale)
ODRX_DECLARE_PROPERTY(ImageBase)
ODRX_DECLARE_PROPERTY(ImageUrl)
ODRX_DECLARE_PROPERTY(Output)
ODRX_DECLARE_PROPERTY(BackGround)
ODRX_DECLARE_PROPERTY(Precision)
ODRX_DECLARE_PROPERTY(GenericFontFamily)
ODRX_DECLARE_PROPERTY(UseHLR)
ODRX_DECLARE_PROPERTY(EnableGouraudShading)
ODRX_DECLARE_PROPERTY(ExplodeShxTexts)
ODRX_DECLARE_PROPERTY(RemoveFirstSpacesOfText)

ODRX_DEFINE_PROPERTY(DefaultImageExt, SvgProperties, getString)
ODRX_DEFINE_PROPERTY(ShxLineWeight, SvgProperties, getDouble)
ODRX_DEFINE_PROPERTY(Tolerance, SvgProperties, getDouble)
ODRX_DEFINE_PROPERTY(LineWeightScale, SvgProperties, getDouble)
ODRX_DEFINE_PROPERTY(ImageBase, SvgProperties, getString)
ODRX_DEFINE_PROPERTY(ImageUrl, SvgProperties, getString)
ODRX_DEFINE_PROPERTY_OBJECT(Output, SvgProperties,  get_Output, put_Output, OdStreamBuf)
ODRX_DEFINE_PROPERTY(BackGround, SvgProperties, getInt32)
ODRX_DEFINE_PROPERTY(Precision, SvgProperties, getInt32)
ODRX_DEFINE_PROPERTY(GenericFontFamily, SvgProperties, getString)
ODRX_DEFINE_PROPERTY(UseHLR, SvgProperties, getBool)
ODRX_DEFINE_PROPERTY(EnableGouraudShading, SvgProperties, getBool)
ODRX_DEFINE_PROPERTY(ExplodeShxTexts, SvgProperties, getBool)
ODRX_DEFINE_PROPERTY(RemoveFirstSpacesOfText, SvgProperties, getBool)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP( SvgProperties );
  ODRX_GENERATE_PROPERTY(DefaultImageExt)
  ODRX_GENERATE_PROPERTY(ShxLineWeight)
  ODRX_GENERATE_PROPERTY(Tolerance)
  ODRX_GENERATE_PROPERTY(LineWeightScale)
  ODRX_GENERATE_PROPERTY(ImageBase)
  ODRX_GENERATE_PROPERTY(ImageUrl)
  ODRX_GENERATE_PROPERTY(Output);
  ODRX_GENERATE_PROPERTY(BackGround)
  ODRX_GENERATE_PROPERTY(Precision)
  ODRX_GENERATE_PROPERTY(GenericFontFamily)
  ODRX_GENERATE_PROPERTY(UseHLR)
  ODRX_GENERATE_PROPERTY(EnableGouraudShading)
  ODRX_GENERATE_PROPERTY(ExplodeShxTexts)
  ODRX_GENERATE_PROPERTY(RemoveFirstSpacesOfText)
ODRX_END_DYNAMIC_PROPERTY_MAP( SvgProperties );

namespace svg
{
  // Check if image has valid size
  //
  bool isValidImage( const OdGiRasterImage* pImg ) 
  {
    return pImg->pixelWidth() != 0 && pImg->pixelHeight() != 0;
  }
  // Apply gamma correction to filter.
  //
  inline static void applyGamma( xml::Node* filter, double gamma )
  {
    ODA_ASSERT( gamma > 0 && gamma < 100 );
    xml::Node* feComponentTransfer = filter->addChild( OD_T("feComponentTransfer") );
    xml::Node* ff[3] = { feComponentTransfer->addChild( OD_T("feFuncR") ), 
      feComponentTransfer->addChild( OD_T("feFuncG") ), 
      feComponentTransfer->addChild( OD_T("feFuncB") ) };
    double ex = log( .5 ) / log( 1. - gamma / 100 );
    for ( int i = 0; i < 3; i++ )
    {
      ff[i]->addAttribute( OD_T("type"),OD_T("gamma") );
      ff[i]->addAttribute( OD_T("exponent"), ex );
      ff[i]->addAttribute( OD_T("amplitude"), 1. );
    }
  }
  
  // Apply contrast correction to filter.
  //
  inline static void applyContrast( xml::Node* filter, double contrast )
  {
    ODA_ASSERT( contrast > 0 && contrast < 100 );
    xml::Node* feComponentTransfer = filter->addChild( OD_T("feComponentTransfer") );
    xml::Node* ff[3] = { new xml::Node( OD_T("feFuncR") ), 
      new xml::Node( OD_T("feFuncG") ), 
      new xml::Node( OD_T("feFuncB") ) };
    for ( int i = 0; i < 3; i++ )
    {
      feComponentTransfer->addChild( ff[i] );
      ff[i]->addAttribute( OD_T("type"),OD_T("linear") );
      ff[i]->addAttribute( OD_T("slope"), contrast / 50 );
      ff[i]->addAttribute( OD_T("intercept"), 0.5 * (1. - contrast / 50) );
    }
  }
  
  // Format transform to this coord. system
  //
  inline static OdString formatTransformMatrix( const OdGePoint3d& position,
    const OdGeVector3d& u, const OdGeVector3d& v )
  {
    OdGeMatrix2d m; m.setCoordSystem( OdGePoint2d( position.x, position.y ),
      OdGeVector2d( u.x, u.y), OdGeVector2d( -v.x, -v.y ) );
    return formatMatrix(m);
  }
}

struct OdSvgDashArrayDescription
{
  bool            m_bUseDashArray;
  double          m_dDashArrayOffset;
  OdArray<double> m_dDashArray;

  OdSvgDashArrayDescription()
  {
    m_bUseDashArray = false;
    m_dDashArrayOffset = 0;
  }
};

// Main exporting device, 
// implements OdGiConveyorGeometry through OdGiGeometrySimplifier.
//
class OdSvgDevice : public Od2dExportDevice
{
  friend class OdSvgView;
  xml::Node* _svgRoot;
  xml::Node* _currentNode;
  xml::Node* _defs;
  ODCOLORREF _color, _color2;
  OdUInt8 _alpha;
  bool _fill, _fillContour;
  double _currentLineWeight;
  OdSvgView* _currentView;
  typedef std::map<OdString, svg::Font*> FontMap;
  FontMap _fontMap;
  SvgPropertiesPtr _properties;
  OdString _clipId;
  std::stack<OdString> _clipsStack;
  int _filterCount;
  int _clipCount;
  int _imageNum;
  int _gradientCount;
  int _pathCount;
  const OdGiDrawable* _currentDrawable;
  OdString _hyperlink;

  std::map<const OdDbStub*, OdSvgDashArrayDescription> m_dashArrayMap;
  OdSvgDashArrayDescription m_curDashArray;
  bool _bShellMode;
public:
  bool _plotLineweights;
  OdSvgDevice() : Od2dExportDevice(DeviceType(kSupport2dPolyline | kSupport2dCircle | kSupport2dEllipse | kSupportNrcClip | kSupportContourFill))
    , _svgRoot( 0 )
    , _currentNode( 0 )
    , _defs( 0 )
    , _color( 0 )
    , _color2( 0 )
    , _alpha( 255 )
    , _fill( false )
    , _fillContour( false )
    , _currentLineWeight( 1 )
    , _currentView( 0 )
    , _properties( SvgProperties::createObject() )
    , _filterCount( 0 )
    , _clipCount( 0 )
    , _imageNum( 0 )
    , _gradientCount( 0 )
    , _pathCount( 0 )
    , _currentDrawable(0)
    , _bShellMode(false)
    , _plotLineweights(true)
  {
  }

  ODRX_DECLARE_MEMBERS( OdSvgDevice );
  
  ~OdSvgDevice()
  {
  }

  OdRxDictionaryPtr properties() { return _properties; }
  
  bool isUseHLR() { return _properties->get_UseHLR(); }

  virtual void update( OdGsDCRect* pUpdatedRect = 0 )
  {
    OdGeDoubleArray devs;
    devs.insert(devs.end(), 5, _properties->get_Tolerance());
    setDeviation(devs);
    ODA_ASSERT( !m_logPalette.isEmpty() );
    if ( m_logPalette.isEmpty() )
      setLogicalPalette( odcmAcadLightPalette(), 256 );
    double lws = _properties->get_LineWeightScale();
    // set default lw scaling
    if ( lws == 0 ) _properties->put_LineWeightScale( 
      (m_outputRect.m_max.x - m_outputRect.m_min.x) / 800 );
    _svgRoot = new svg::RootNode( m_outputRect, _properties->get_BackGround() );

    if( userGiContext() )
    {
      if( userGiContext()->lineWeightConfiguration(OdGiContext::kLineCapStyle) == (OdUInt32)OdPs::kLesButt)
      {
        _svgRoot->removeAttribute( OD_T("stroke-linecap"));
        _svgRoot->addAttribute( OD_T("stroke-linecap"),   OD_T("butt") );
      }

      if( userGiContext()->lineWeightConfiguration(OdGiContext::kLineJoinStyle) == (OdUInt32)OdPs::kLjsMiter)
      {
        _svgRoot->removeAttribute( OD_T("stroke-linejoin"));
        _svgRoot->addAttribute( OD_T("stroke-linecap"),   OD_T("miter") );
      }
      else if( userGiContext()->lineWeightConfiguration(OdGiContext::kLineJoinStyle) == (OdUInt32)OdPs::kLjsBevel)
      {
        _svgRoot->removeAttribute( OD_T("stroke-linejoin"));
        _svgRoot->addAttribute( OD_T("stroke-linecap"),   OD_T("bevel") );
      }
    }

    _currentNode = _svgRoot;
    Od2dExportDevice::update( pUpdatedRect );
    flushFonts();
    ODA_ASSERT( _properties->get_Output() ); 
    // "Output" property must be set before exporting
    OdStreamBufPtr stream = _properties->get_Output();
    writeFile( *stream.get() );
    delete _svgRoot;
    // Clear all settings to be safe for initiate secondary vectorization
    _currentNode = _svgRoot = _defs = 0;
    _color = _color2 = 0;
    _alpha = 255;
    _fill = _fillContour = false;
    _currentLineWeight = 1.0;
    _filterCount = _clipCount = _imageNum = _gradientCount = _pathCount = 0;
  }
  
  void flushFonts()
  {
    for ( FontMap::iterator i = _fontMap.begin(); i != _fontMap.end(); ++i )
    {
      addDefs();
      i->second->handOverTo( _defs );
      delete i->second;
    }
    _fontMap.clear();
  }

  void writeFile( OdStreamBuf& stream )
  {
    stream << OdString( OD_T("<?xml version=\"1.0\" standalone=\"no\"?>\r\n") );
    stream << *_svgRoot;
  }
  void addDefs()
  {
    if ( _defs ) return;
    _defs = new xml::Node( OD_T("defs") );
    _svgRoot->_children.insert( _svgRoot->_children.begin(), _defs );
  }

  OdGsViewPtr createView( const OdGsClientViewInfo* pViewInfo = 0, bool bEnableLayerVisibilityPerView = false )
  {
    OdGsViewPtr v = OdSvgView::createObject();
    static_cast<OdSvgView*>( v.get() )->init( this, pViewInfo, bEnableLayerVisibilityPerView );
    ((OdSvgView*)(OdGsView*)v)->screenOutput().setDestGeometry(*this);
    return v;
  }
 
  OdString createClipPath( int rings, const int* counts, const OdGePoint2d* pp);
  xml::Node* createGradient(OdString& id, const OdCmEntityColor& col1, const OdCmEntityColor& col2, const OdGePoint3d& p1, const OdGePoint3d& p2);

  void setDashArrayData(const  OdSvgDashArrayDescription& dashArray )
  {
    m_curDashArray = dashArray;
  }

  void setLineTypeDashArray( const OdDbStub* pLineStyle, const OdSvgDashArrayDescription& lStyleDashes )
  {
    m_dashArrayMap[pLineStyle] = lStyleDashes;
  }

  bool getLineTypeDashArray( const OdDbStub* pLineStyle, OdSvgDashArrayDescription& lStyleDashes )
  {
    bool bRet = false;

    std::map<const OdDbStub*, OdSvgDashArrayDescription>::iterator pIter = m_dashArrayMap.find( pLineStyle );

    if( pIter != m_dashArrayMap.end() )
    {
      lStyleDashes = pIter->second;
      bRet = true;
    }

    return bRet;
  }

  void setClipRect(const OdGsDCPoint& p1, const OdGsDCPoint& p2 )
  {
    OdGePoint2d clipPoints[ 4 ] =
    {
      OdGePoint2d( p1.x, p1.y ),
      OdGePoint2d( p2.x, p1.y ),
      OdGePoint2d( p2.x, p2.y ),
      OdGePoint2d( p1.x, p2.y ) 
    };
    int size = 4;
    _clipId = createClipPath(1, &size, clipPoints );
    setCurrTraits( _fill, kTxtNone, true );
  }
  
  void setClipPath( int rings, const int* counts, const OdGePoint2d* pp )
  {
    _clipId = createClipPath( rings, counts, pp );
    setCurrTraits(_fill, kTxtNone, true);
  }
  
  // Od2dExportDevice interface
  void dc_pushClip(int rings, const int* counts, const OdGsDCPointArray &nrcPoints)
  {
    _clipsStack.push(_clipId); // save old clip id
    
    OdGePoint2dArray clip; // temporary fix - dc_pushClip should be changed in Od2dExportDevice, to avoid double conversion
    int cur = 0;
    for ( int i = 0; i < rings; i++)
    {
      for ( int j = 0; j < counts[i]; j++ )
      {
        const OdGsDCPoint& p = nrcPoints[cur++];
        clip.append( OdGePoint2d( p.x, p.y ) );
      }
    }
    setClipPath(rings, counts, clip.asArrayPtr());
  }

  void dc_popClip()
  {
    if(_clipsStack.empty())
      throw OdError(eNotApplicable);
    _clipId = _clipsStack.top();
    _clipsStack.pop();
    _currentNode = _svgRoot->addChild( OD_T("g") );
    if (!_clipId.isEmpty())
      _currentNode->addAttribute(OD_T("clip-path"), OdString(OD_T("url(#")) + _clipId + OD_T(")"));
  }

  // usually traits are set in View::OnTraitsModified(), 
  // but here it would be more clear to set them manually,
  // due to nested structure of svg object tree
  enum TextMode { kTxtNone, kTxtShx, kTxtTtf };
  void setCurrTraits(bool fill, TextMode textMode = kTxtNone, bool forceCreateGroup = false)
  {
    OdGiSubEntityTraitsData traits;
    bool addHyperlink = false;
    if (drawContext()) // may be absent in case nrc-shaded viewport
    {
      traits = drawContext()->effectiveTraits();
      const OdGiDrawable* d = drawContext()->currentDrawable();
      if (d != _currentDrawable)
      {
        _currentDrawable = d;
        if (d)
        {
          OdDbHyperlinkCollectionPtr hColl;
          OdDbEntityHyperlinkPEPtr phPE = OdDbEntityHyperlinkPE::cast(d);
          if (!phPE.isNull())
            hColl = phPE->getHyperlinkCollection(d, true);
          if (!hColl.isNull() && hColl->count() > 0)
          {
            OdString h = hColl->item(0)->name();
            if (!h.isEmpty())
              addHyperlink = true;
            if (_hyperlink != h)
            {
              _hyperlink = h;
              if (!_hyperlink.isEmpty())
                forceCreateGroup = true;
            }
          }
        }
      }
    }
    ODCOLORREF color, color2 = 0;
    OdUInt8 alpha = traits.transparency().alpha();
    if (_bShellMode)
      alpha = 255;
    double lw = 0.0;
    if (_currentView && userGiContext()->isPlotGeneration())
    {
      const OdPsPlotStyleData& plotStyle = _currentView->effectivePlotStyle();
      color = plotStyle.color().isByColor() ?
        ODTOCOLORREF(plotStyle.color()) : m_logPalette[plotStyle.color().colorIndex()];
      if (textMode == kTxtNone)
      {
        if (!_currentView->hasLineweightOverride())
          lw = _properties->get_LineWeightScale() * plotStyle.lineweight();
        else
          lw = _currentView->currentLineweightOverride()->scaleOverride() * plotStyle.lineweight();
      }
    }
    else
    {
      color = traits.trueColor().isByColor() ?
        ODTOCOLORREF(traits.trueColor()) : m_logPalette[traits.color()];
      if (textMode == kTxtNone)
      {
        if (!_currentView || !_currentView->hasLineweightOverride())
          lw = _properties->get_LineWeightScale() / 100 * (int)traits.lineWeight();
        else
          lw = _currentView->currentLineweightOverride()->scaleOverride() / 100 * (int)traits.lineWeight();
      }
    }
    if (textMode == kTxtShx)
      lw = _properties->get_ShxLineWeight() * _properties->get_LineWeightScale();

    if (!_plotLineweights || (lw == 0 && textMode != kTxtTtf))
      lw = 0.1 * _properties->get_LineWeightScale();

    if( traits.lineStyleModifiers() )
    {
      if( traits.lineStyleModifiers()->getWidthMode() == OdGiDgLinetypeModifiers::kLsWidthConstant )
      {
        lw = traits.lineStyleModifiers()->getWidth();
      }
    }
    
    bool fillContour = GETBIT(traits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill);
    if (fill && fillContour)
    {
      OdCmEntityColor secColor = traits.secondaryTrueColor();
      color2 = secColor.isByColor() ?
        ODTOCOLORREF(secColor) : m_logPalette[secColor.colorIndex()];
    }

    if ( forceCreateGroup || _color != color || fill != _fill || lw != _currentLineWeight || _alpha != alpha ||
         (fill && ((fillContour != _fillContour) || (fillContour && _color2 != color2))) )
    {
      _color = color;
      _alpha = alpha;
      _fill = fill;
      _currentLineWeight = lw;
      if (addHyperlink)
      {
        _currentNode = _svgRoot->addChild( OD_T("a") );
        _currentNode->addAttribute( OD_T("xlink:href"), _hyperlink );
        _currentNode = _currentNode->addChild( OD_T("g") );
      }
      else
        _currentNode = _svgRoot->addChild( OD_T("g") );
      if (!_clipId.isEmpty())
        _currentNode->addAttribute( OD_T("clip-path"), OdString(OD_T("url(#")) + _clipId + OD_T(")"));
      double alphaDbl = (_alpha == 255) ? 1.0 : traits.transparency().alphaPercent();
      if ( _fill ) 
      {
        if ( fillContour )
        {
          _fillContour = fillContour;
          _color2 = color2;
          _currentNode->addAttribute( svg::Fill( _color2 ) );
          _currentNode->addAttribute( svg::Stroke( _color ) );
          _currentNode->addAttribute( OD_T("stroke-width"), _currentLineWeight );
        }
        else
        {
          _currentNode->addAttribute( svg::Fill( _color ) );
          _currentNode->addAttribute( svg::Stroke( _color ) );
          _currentNode->addAttribute(OD_T("stroke-width"), _bShellMode ? 1.0 : _currentLineWeight); // 1 pixel border masks gaps between fills
        }
        if (_alpha != 255)
        {
          _currentNode->addAttribute(OD_T("fill-opacity"), alphaDbl);
          _currentNode->addAttribute(OD_T("stroke-opacity"), alphaDbl);
        }
      }
      else
      {
        _currentNode->addAttribute( svg::Fill() );
        _currentNode->addAttribute( svg::Stroke( _color ) );
        if (_alpha != 255) _currentNode->addAttribute( OD_T("stroke-opacity"), alphaDbl );
        _currentNode->addAttribute( OD_T("stroke-width"), _currentLineWeight );
      }
    }
  }
  
  virtual void polylineOut( OdInt32 nbPoints, const OdGePoint3d* pVertexList )
  {
    setCurrTraits( false );
    if (xml::Node* pPolylineNode = _currentNode->addChild(new xml::Node(OD_T("polyline"))))
    {
      pPolylineNode->addAttribute(svg::Points(nbPoints, pVertexList));
      if (m_curDashArray.m_bUseDashArray)
      {
        if (!OdZero(m_curDashArray.m_dDashArrayOffset))
        {
          pPolylineNode->addAttribute(OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }
        pPolylineNode->addAttribute(svg::DashArray(m_curDashArray.m_dDashArray));
      }
    }
  }
  
  virtual void polygonOut( OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* )
  {
    setCurrTraits( true );
    if (xml::Node* node = _currentNode->addChild(new xml::Node(OD_T("polygon"))))
    {
      node->addAttribute(svg::Points(nbPoints, pVertexList));
      if (m_curDashArray.m_bUseDashArray)
      {
        if (!OdZero(m_curDashArray.m_dDashArrayOffset))
        {
          node->addAttribute(OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }
        node->addAttribute(svg::DashArray(m_curDashArray.m_dDashArray));
      }
    }
  }
  void dc_polygon(OdUInt32 nbPoints, const OdGePoint2d* pVertexList)
  {
    setCurrTraits( true );
    if (xml::Node* node = _currentNode->addChild(new xml::Node(OD_T("polygon"))))
    {
      node->addAttribute(svg::Points(nbPoints, pVertexList));
      if (m_curDashArray.m_bUseDashArray)
      {
        if (!OdZero(m_curDashArray.m_dDashArrayOffset))
        {
          node->addAttribute(OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }
        node->addAttribute(svg::DashArray(m_curDashArray.m_dDashArray));
      }
    }
  }
  virtual void shellFaceOut( OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* normals)
  {
    if (!vertexData() || !vertexData()->trueColors())
    {
      setCurrTraits( true );
      svg::Path* path = new svg::Path;
      while ( faceListSize )
      {
        OdGiShellFaceIterator iter( vertexDataList(), pFaceList + 1 );
        int n = Od_abs( (int)*pFaceList );

        OdGePoint3dArray points;
        for ( int i = 0; i < n; ++i, ++iter )
          points.append( *iter );
        path->addLoop( n, points.getPtr() );

        pFaceList += n + 1;
        faceListSize -= n + 1;
      }
      _currentNode->addChild( path );
    }
    else
    {
      Od2dExportDevice::shellFaceOut(faceListSize, pFaceList, normals);
    }
  }
  // get a point projected onto opposite triangle side
  OdGePoint3d getTriangleProjectedPoint(OdGePoint3d* pts, int N)
  {
    OdGeVector3d v, pv;
    switch(N)
    {
    case 0:
      v = pts[2] - pts[1];
      pv = pts[2] - pts[0];
      break;
    case 1:
      v = pts[2] - pts[0];
      pv = pts[2] - pts[1];
      break;
    case 2:
      v = pts[1] - pts[0];
      pv = pts[1] - pts[2];
      break;
    }
    v.normalize();
    pv -= v * v.dotProduct(pv);
    return pts[N] + pv;
  }
  void linear_gradient_triangle( OdGePoint3d* pts, OdCmEntityColor* cols, int gradient_start)
  {
    OdString id;
    OdGePoint3d p = getTriangleProjectedPoint(pts, gradient_start);
    createGradient(id, cols[gradient_start], cols[gradient_start==0?1:0], pts[gradient_start], p );
    svg::Path* path = new svg::Path();
    path->addAttribute(L"fill", OdString(L"url(#") + id + L')');
    path->addLoop( 3, pts );
    _currentNode->addChild(path);
  }
  void gouraud_triangle(OdGePoint3d* pts, OdCmEntityColor* cols)
  {
    svg::Path* path = new svg::Path();
    _defs->addChild(path);
    OdString pathId;
    pathId.format(L"t_%d", _pathCount++);
    path->addAttribute(L"id", pathId);
    path->addLoop( 3, pts );
    OdString pathRef = L'#' + pathId;
    
    if (!_properties->get_EnableGouraudShading())
    {
      // mean color triangle
      xml::Node* use = _currentNode->addChild(L"use");
      use->addAttribute(L"xlink:href", pathRef);
      OdString meanColor;
      meanColor.format(L"rgb(%d,%d,%d)", int((cols[0].red()+cols[1].red()+cols[2].red())/3),
        int((cols[0].green()+cols[1].green()+cols[2].green())/3), int((cols[0].blue()+cols[1].blue()+cols[2].blue())/3));
      use->addAttribute(L"fill", meanColor);
    }
    else
    {
      xml::Node* g = _currentNode->addChild(L"g");
      g->addAttribute(L"filter", L"url(#A)");
      for (int i = 0; i < 3; ++i)
      {
        OdString id;
        createGradient(id, cols[i], cols[i], pts[i], getTriangleProjectedPoint(pts, i) );
        xml::Node* use = g->addChild(L"use");
        use->addAttribute(L"xlink:href", pathRef);
        use->addAttribute(L"fill", OdString(L"url(#") + id + L')');
        if (i > 0)
          use->addAttribute(L"filter", L"url(#colorAdd)");
      }
    }
  }
  virtual void dc_gouraud_polytriangle(
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdCmEntityColor* pColors )
  {
    setCurrTraits(true);
    for (int i = 0; i < faceListSize;)
    {
      ODA_VERIFY(pFaceList[i++] == 3);
      OdGePoint3d pts[3];
      OdCmEntityColor cols[3];
      for(int k = 0; k < 3; ++k)
      {
        pts[k] = pVertexList[pFaceList[i]];
        cols[k] = pColors[pFaceList[i++]];
      }
      if (pts[0] == pts[1] || pts[1] == pts[2] || pts[0] == pts[2])
        continue;
      if (cols[0] == cols[1])
        linear_gradient_triangle(pts, cols, 2);
      else if (cols[1] == cols[2])
        linear_gradient_triangle(pts, cols, 0);
      else if (cols[0] == cols[2])
        linear_gradient_triangle(pts, cols, 1);
      else
        gouraud_triangle(pts, cols);
    }
  }

  bool arcFillMode( OdGiArcType arcType )
  {
    if ( arcType == kOdGiArcSimple )
      return false;
    return circleArcFillMode();
  }

  virtual void circleProc( const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0)
  {
    setCurrTraits( arcFillMode( kOdGiArcChord ) );
    if ( pExtrusion && !pExtrusion->isZeroLength() )
      OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
    else
    {
      xml::Node* pCircleNode = _currentNode->addChild( new svg::Circle( center, radius ) );

      if( pCircleNode && m_curDashArray.m_bUseDashArray )
      {
        if( !OdZero(m_curDashArray.m_dDashArrayOffset) )
        {
          pCircleNode->addAttribute( OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }

        pCircleNode->addAttribute( svg::DashArray(m_curDashArray.m_dDashArray) );
      }
    }
  }
  
  virtual void circleProc( const OdGePoint3d& p1, const OdGePoint3d& p2, const OdGePoint3d& p3, const OdGeVector3d* pExtrusion = 0)
  {
    OdGeCircArc3d arc( p1, p2, p3 );
    circleProc( arc.center(), arc.radius(), OdGeVector3d::kZAxis, pExtrusion );
  }
  
  static void setSvgArcType( svg::Path* path, OdGiArcType arcType, const OdGePoint3d& center )
  {
    if ( arcType == kOdGiArcSector )
    {
      OdString s = OD_T(" L");
      s += svg::Points::formatPoint( center );
      s += OD_T(" z");
      path->getAttribute( OD_T("d") )->_value += s;
    }
    else if ( arcType == kOdGiArcChord )
      path->getAttribute( OD_T("d") )->_value += OD_T(" z");
  }
  
  static svg::Path* formatArcPath( const OdGePoint3d& start, const OdGePoint3d& end, 
    double r1, double r2, double angle, bool largeArc , bool reversed )
  {
      svg::Path* path = new svg::Path;
      OdString s = OD_T("M"); // "M%g,%g A%g,%g %g %d,%d %g,%g",
      s += svg::Points::formatPoint( start );
      s += OD_T(" A");
      s += xml::Attribute::formatDouble( r1 );
      s += ',';
      s += xml::Attribute::formatDouble( r2 );
      s += ' ';
      s += xml::Attribute::formatDouble( angle );
      s += largeArc ? OD_T(" 1,") : OD_T(" 0,");
      s += reversed ? OD_T("0 ") : OD_T("1 ");
      s += svg::Points::formatPoint( end );
      path->addAttribute( OD_T("d"), s );
      return path;
  }
  
  virtual void circularArc( const OdGeCircArc3d& arc, OdGiArcType arcType = kOdGiArcSimple )
  {
    setCurrTraits( arcFillMode( arcType ) );
    if ( !arc.startPoint().isEqualTo( arc.endPoint() ) )
    {
      double a = arc.endAng() - arc.startAng();
      if ( a < 0 ) a += Oda2PI;
      svg::Path* path = formatArcPath( arc.startPoint(), arc.endPoint(),
        arc.radius(), arc.radius(), 0, a > OdaPI, arc.normal().z < 0 );
      setSvgArcType( path, arcType, arc.center() );
      xml::Node* pPathNode = _currentNode->addChild( path );

      if( pPathNode && m_curDashArray.m_bUseDashArray )
      {
        if( !OdZero(m_curDashArray.m_dDashArrayOffset) )
        {
          pPathNode->addAttribute( OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }

        pPathNode->addAttribute( svg::DashArray(m_curDashArray.m_dDashArray) );
      }
    }
    else
    {
      xml::Node* pCircleNode = _currentNode->addChild( new svg::Circle( arc.center(), arc.radius() ) );

      if( pCircleNode && m_curDashArray.m_bUseDashArray )
      {
        if( !OdZero(m_curDashArray.m_dDashArrayOffset) )
        {
          pCircleNode->addAttribute( OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }

        pCircleNode->addAttribute( svg::DashArray(m_curDashArray.m_dDashArray) );
      }
    }
  }
  
  virtual void circularArcProc( const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0)
  {
    if ( pExtrusion && !pExtrusion->isZeroLength() ) 
      OdGiGeometrySimplifier::circularArcProc( center, radius, normal, startVector, sweepAngle, arcType, pExtrusion );
    else circularArc( OdGeCircArc3d( 
      center, normal, startVector, radius, 0., sweepAngle ), arcType);
  }
  
  virtual void circularArcProc( const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0)
  {
    if ( pExtrusion && !pExtrusion->isZeroLength() )
    {
      OdGiGeometrySimplifier::circularArcProc( start, point, end, arcType, pExtrusion );
    }
    else
    {
      OdGeCircArc3d arc;
      OdGeError status;
      arc.set(start, point, end, status);
      if (status == OdGe::kOk)
      {
        circularArc( arc, arcType );
      }
      else
      {
        OdGePoint3d pp[2] = {start, end};
        polylineOut(2, pp);
      }
    } 
  }
  
  virtual void ellipArcProc(const OdGeEllipArc3d& arc,
    const OdGePoint3d* pEndPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0)
  {
    if ( pExtrusion || OdZero( arc.minorRadius() ) || OdZero( arc.majorRadius() )
      || arc.majorAxis().isParallelTo( arc.minorAxis() ) ) 
    {
      OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
      return;
    }
    setCurrTraits( arcFillMode( arcType ) );
    OdGeEllipArc3d correctArc( arc );
    OdGe::ErrorCondition ec;
    correctArc.orthogonolizeAxes( OdGeContext::gTol, ec );
    if ( ec != OdGe::kOk 
      || correctArc.minorAxis().isZeroLength() 
      || correctArc.majorAxis().isZeroLength() 
      || correctArc.minorRadius() < _properties->get_Tolerance() )
    {
      OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
      return;
    }
    double angle = correctArc.majorAxis().x == 0 ? 90 
      : atan( correctArc.majorAxis().y / correctArc.majorAxis().x ) * 180 / OdaPI;
    // bug in adobe viewer workaround
    const double MIN_ADOBE_ROT_ANGLE = 1E-10;
    if ( fabs( angle ) < MIN_ADOBE_ROT_ANGLE ) angle = 0; 
    if ( !correctArc.endPoint().isEqualTo( correctArc.startPoint() ) )
    {
      double a = correctArc.endAng() - correctArc.startAng();
      if ( a < 0 ) a += Oda2PI;
      
      svg::Path* path = formatArcPath( 
        pEndPointsOverrides ? pEndPointsOverrides[0] : correctArc.startPoint(),
        pEndPointsOverrides ? pEndPointsOverrides[1] : correctArc.endPoint(),
        correctArc.majorRadius(), correctArc.minorRadius(), angle, a > OdaPI, 
        correctArc.majorAxis().crossProduct( correctArc.minorAxis() ).z < 0 );
      
      setSvgArcType( path, arcType, correctArc.center() );
      xml::Node* pPathNode = _currentNode->addChild( path );

      if( pPathNode && m_curDashArray.m_bUseDashArray )
      {
        if( !OdZero(m_curDashArray.m_dDashArrayOffset) )
        {
          pPathNode->addAttribute( OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }

        pPathNode->addAttribute( svg::DashArray(m_curDashArray.m_dDashArray) );
      }
    }
    else 
    {
      xml::Node* pEllipseNode = _currentNode->addChild( 
        new svg::Ellipse( correctArc.center(), correctArc.majorRadius(), correctArc.minorRadius(), angle ) );

      if( pEllipseNode && m_curDashArray.m_bUseDashArray )
      {
        if( !OdZero(m_curDashArray.m_dDashArrayOffset) )
        {
          pEllipseNode->addAttribute( OD_T("stroke-dashoffset"), m_curDashArray.m_dDashArrayOffset);
        }

        pEllipseNode->addAttribute( svg::DashArray(m_curDashArray.m_dDashArray) );
      }
    }
  }

  OdBaseTextIteratorPtr createTextIterator( OdString& text, bool raw, const OdGiTextStyle* pTextStyle )
  {
    OdDbBaseDatabase* pDb = drawContext()->giContext().database();
    return OdDbBaseDatabasePEPtr(pDb)->createTextIterator(pDb, text.c_str(), text.getLength(), raw, pTextStyle );
  }
  
  svg::Font* getBigFont( const OdGiTextStyle* pTextStyle )
  {
    if ( !pTextStyle->getBigFont() ) return 0;
    return getFont( pTextStyle->bigFontFileName());
  }

  svg::Font* getFont( const OdGiTextStyle* pTextStyle )
  {
    return getFont( pTextStyle->ttfdescriptor().fileName() );
  }

  svg::Font* getFont( const OdString& face)
  {
    OdString localFace = face.isEmpty() ? OD_T("simplex.shx") : face.c_str();
    FontMap::iterator fi = _fontMap.find( localFace );
    if ( fi == _fontMap.end() )
    {
      svg::Font* font = new svg::Font( localFace );
      _fontMap[ localFace ] = font;
      return font;
    }
    else return fi->second;
  }
  
  virtual void textProc( const OdGePoint3d& position,
    const OdGeVector3d& u, const OdGeVector3d& v,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0)
  {
    OdString text = length > 0 ? OdString( msg, length ) : OdString( msg );
    if( pExtrusion || !svg::isUnicodeConvertible( text ) || 
       ( _properties->get_ExplodeShxTexts() && pTextStyle->getFont() && pTextStyle->getFont()->isShxFont() ) 
      )
    {
      OdGiGeometrySimplifier::textProc( position, u, v, msg, length, raw, pTextStyle, pExtrusion );
      return;
    }

// Remove spaces at first of message. RAV #DGN-1656
    OdInt32  uTextOffset  = 0;
    double   dSpaceOffset = 0;

    if( _properties->get_RemoveFirstSpacesOfText() && drawContext() )
    {
      while( (uTextOffset < length) && (text.getAt(uTextOffset) == L' ') )
      {
        if( OdZero(dSpaceOffset) )
        {
          OdGePoint3d ptMin, ptMax, ptEndPos;
          OdChar chSpace = L' ';
          drawContext()->giContext().textExtentsBox(*pTextStyle, &chSpace, 1, 0, ptMin, ptMax, &ptEndPos );
          dSpaceOffset = ptEndPos.x;
        }
        uTextOffset++;
      }
    }

    OdGeVector3d vrPositionOffset = u*dSpaceOffset*uTextOffset;

    text = text.right( text.getLength() - uTextOffset );

    if( text.isEmpty() )
      return;
//
    xml::Node* node = new xml::Node( OD_T("text") );
    node->addAttribute( OD_T("transform"), svg::formatTransformMatrix( position + vrPositionOffset, u, v ) );
    bool bFillText = false;

    if (pTextStyle->isTtfFont())
    {
      setCurrTraits( true, kTxtTtf );
      OdString ff; 
      if ( pTextStyle->ttfdescriptor().typeface() == OD_T("SansSerif") ) // Adobe specific workaround
        ff = OD_T("sans-serif");
      else if ( !_properties->get_GenericFontFamily().isEmpty() ) 
        ff.format( OD_T("%ls,\'%ls\'"), pTextStyle->ttfdescriptor().typeface().c_str(), _properties->get_GenericFontFamily().c_str() );
      else if (pTextStyle->ttfdescriptor().typeface().isEmpty())
        ff = pTextStyle->ttfdescriptor().fileName();
      else 
        ff = pTextStyle->ttfdescriptor().typeface();
      node->addAttribute( OD_T("font-family"), ff );
      if ( pTextStyle->ttfdescriptor().isItalic())
        node->addAttribute( OD_T("font-style"), OD_T("italic") );
      if ( pTextStyle->ttfdescriptor().isBold() )
        node->addAttribute( OD_T("font-weight"), OD_T("bold") );
    }
    else
    {
      setCurrTraits( false, kTxtShx );
      OdBaseTextIteratorPtr it = createTextIterator( text, raw, pTextStyle);
      svg::embedCharacters( 
        getFont( pTextStyle ), getBigFont(pTextStyle), it, pTextStyle, drawContext(), bFillText );
    }
    svg::setTextAttributes( node, pTextStyle, bFillText, _color );
    
    svg::replaceXmlSpecials( text );
    OdBaseTextIteratorPtr it = createTextIterator( text, raw, pTextStyle);
    svg::setContents( node, it, pTextStyle );
    
    if ( node->_contents.isEmpty() && node->_children.empty() )
      delete node;
    else
      _currentNode->addChild( node );
  }
  
  virtual void xlineProc( const OdGePoint3d& p1, const OdGePoint3d& p2 )
  {
    ODA_ASSERT(p1.isEqualTo( p2 ));
    OdGePoint3d pp[2] = { p1, p2 };
    polylineOut( 2, pp );
  }
  
  virtual void rayProc( const OdGePoint3d& p1, const OdGePoint3d& p2 )
  {
    ODA_ASSERT(p1.isEqualTo( p2 ));
    OdGePoint3d pp[2] = { p1, p2 };
    polylineOut( 2, pp );
  }
  
  virtual void nurbsProc( const OdGeNurbCurve3d& nurbs)
  {
    setCurrTraits( false );
    OdGePoint3dArray points;
    nurbs.getSamplePoints( nurbs.startParam(), nurbs.endParam(), 
      _properties->get_Tolerance(), points );
    polylineOut( points.size(), points.getPtr() );
  }

  // Generate unique file name in given directory.
  //
  OdString generateFileName( const OdString& path, const OdString& ext )
  {
    OdString s;
    for (;;)
    {
       s.format( OD_T("image%d%ls"), _imageNum++, ext.c_str() );
       if ( !odrxSystemServices()->accessFile( path + s, 0 ) ) break;
    }
    return s;
  }

  static OdString createImageTransform( const OdGiRasterImage* pImg 
    ,const OdGePoint3d& origin
    ,const OdGeVector3d& u
    ,const OdGeVector3d& v
    )
  {
    OdUInt32 width(pImg->pixelWidth()), height(pImg->pixelHeight());
    OdGeVector2d u2( u.x * width, u.y * width );
    OdGeVector2d v2( -v.x * height, -v.y * height ); // image in svg is upside-down
    OdGePoint2d o2( origin.x, origin.y );
    OdGeMatrix2d m; m.setCoordSystem( o2 - v2, u2, v2 );
    return svg::formatMatrix(m);
  }
  
  OdString createImageClipPath( const OdGiRasterImage* pImg,
    int numBoundPts, const OdGePoint2d* uvBoundary )
  {
    OdGePoint2dArray clipPoints( numBoundPts );
    OdUInt32 width(pImg->pixelWidth()), height(pImg->pixelHeight());
    for ( int i = 0 ; i < numBoundPts; i++ )
      clipPoints.push_back(OdGePoint2d( uvBoundary[i].x / width, uvBoundary[i].y / height ));
    return createClipPath( 1, &numBoundPts, clipPoints.getPtr() );
  }
  
  xml::Node* createFilter( OdString& id );

  OdString copyImage( const OdGiRasterImage* pImg, const OdString& path, bool transparency )
  {
    try
    {
      OdStreamBufPtr inFile = odrxSystemServices()->createFile( path );
      if ( inFile.isNull() ) return path;
      OdString newPath = _properties->get_ImageBase();
      int slash = odmax( path.reverseFind( '/' ), path.reverseFind( '\\' ) );
      OdString newName = path.right( path.getLength() - slash - 1 );
      // svg does not support bmp images
      OdString ext = path.right(4);
      ext.makeLower();
      bool convert = false;
      if ( ext == OD_T(".bmp") )
      {
        convert = true;
        ext = transparency ? L".gif" : L".png";
        newName.deleteChars( newName.getLength() - 4 , 4 );
        newName += ext;
      }
      
      // if such a file exist
      if ( odrxSystemServices()->accessFile( newPath + newName, 0 ) )
        newName = generateFileName( _properties->get_ImageBase(), ext );

      if ( convert )
      {
        OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
        if (pRasSvcs.get())
        {
          if (transparency && pImg->numColors() == 2)
          {
            OdUInt32 flags[2] = { OdRxRasterServices::kTransparentColor, 1 };
            if (pRasSvcs->saveRasterImage(pImg, newPath + newName, transparency ? OdRxRasterServices::kGIF : OdRxRasterServices::kPNG, flags))
              return _properties->get_ImageUrl() + newName;
          }
          else if (pRasSvcs->saveRasterImage(pImg, newPath + newName, transparency ? OdRxRasterServices::kGIF : OdRxRasterServices::kPNG))
            return _properties->get_ImageUrl() + newName;
        }
        else
        {
          newName.deleteChars(newName.getLength() - 4);
          newName += OD_T(".bmp");
        }
      }
      // copy to new path
      OdStreamBufPtr outFile = odrxSystemServices()->createFile(newPath + newName, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kOpenAlways);
      inFile->copyDataTo(outFile);
      return _properties->get_ImageUrl() + newName;
    }
    catch( const OdError_CantOpenFile& ex )
    {
      OdString s(ex.description()); // prevent warnings in release build
      ODA_TRACE1("%ls", s.c_str());
      return path;
    }
    catch(...)
    {
      ODA_FAIL();
      return path;
    }
  }

  OdString putRasterImage(const OdGiRasterImage* pImg)
  {
    OdString path = _properties->get_ImageBase();
    OdString ext  = _properties->get_DefaultImageExt();
    OdString name = generateFileName(path, ext);
    OdString fullName = path+name;
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if(!pRasSvcs.get())
      throw OdError(eNotApplicable); // RX_RASTER_SERVICES_APPNAME must be available in case we reached this point
    pRasSvcs->saveRasterImage(pImg, fullName);
    return name;
  }

  void dc_raster_image(OdGiRasterImagePtr pImg, const OdGeExtents2d& exts)
  {
    OdString path = putRasterImage(pImg);
    xml::Node* imgNode = _currentNode->addChild( new svg::Image( path ) );
    OdGePoint3d origin = OdGePoint3d(exts.minPoint().x, exts.maxPoint().y, 0.0);
    OdGeVector3d u, v;
    u = OdGeVector3d::kXAxis*(exts.maxPoint().x-exts.minPoint().x)/pImg->pixelWidth();
    v = -OdGeVector3d::kYAxis*(exts.maxPoint().y-exts.minPoint().y)/pImg->pixelHeight();
    imgNode->addAttribute( OD_T("transform"), createImageTransform( pImg, origin, u, v ) );
  }

  OdSmartPtr<OdGiBitonalRasterTransformer> process2ColorsImage(const OdGiRasterImage* pImg)
  {
    ODA_ASSERT( pImg->numColors() == 2 );

    OdSmartPtr<OdGiBitonalRasterTransformer> pBitTrans = OdRxObjectImpl<OdGiBitonalRasterTransformer>::createObject();

    OdGiSubEntityTraitsData traits = drawContext()->effectiveTraits();
    OdCmEntityColor primaryColor = traits.trueColor();
    OdCmEntityColor secondaryColor = (traits.drawFlags() & OdGiSubEntityTraits::kDrawContourFill) 
                                    ? traits.secondaryTrueColor()
                                    : OdCmEntityColor(0,0,0);
    bool bTransparency = (traits.drawFlags() & OdGiSubEntityTraits::kDrawContourFill) ? true : false;

    pBitTrans->setOriginal( pImg, 
                            m_logPalette[ primaryColor.colorIndex() ], 
                            m_logPalette[ secondaryColor.colorIndex() ], 
                            bTransparency );
    
    return pBitTrans;
  }

  virtual void rasterImageProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImg, // image object
    const OdGePoint2d* uvBoundary, // may not be null
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0 )
  {
    if ( !svg::isValidImage( pImg ) ) return;

    OdDbBaseDatabase* pDb = drawContext()->giContext().database();
    OdDbBaseDatabasePEPtr pDbPE( pDb );

    OdString path;
    if ((pImg->imageSource() == OdGiRasterImage::kFromFile) && !pImg->sourceFileName().isEmpty() && !_properties->get_ImageBase().isEmpty())
    {
      path = copyImage(pImg, path = pImg->sourceFileName(), transparency);
    }
    else
    {
      if ( _properties->get_ImageBase().isEmpty() )
        return;
      path = generateFileName( _properties->get_ImageBase(), _properties->get_DefaultImageExt() );
      OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);

      OdSmartPtr<OdGiBitonalRasterTransformer> pImg2;
      bool bImg2colors(false);
      if (pImg->numColors() == 2)
      {
        pImg2 = process2ColorsImage(pImg);
        bImg2colors = true;
      }

      if ( !pRasSvcs.get() || !pRasSvcs->saveRasterImage( (bImg2colors) ? pImg2.get() : pImg, _properties->get_ImageBase() + path) ) 
        return;
    }

    xml::Node* imgNode = _currentNode->addChild( new svg::Image( path ) );
    imgNode->addAttribute( OD_T("transform"), createImageTransform( pImg, origin, u, v ) );
    if ( fade > 0 ) imgNode->addAttribute( OD_T("opacity"), 1 - fade / 100 );
    if ( uvBoundary && numBoundPts > 2 )
    {
      OdString clipId = createImageClipPath( pImg, numBoundPts, uvBoundary );
      imgNode->addAttribute( OD_T("clip-path"), OdString(OD_T("url(#")) + clipId + OD_T(")") );
    }
    if ( brightness != 50 || contrast != 50 )
    {
      OdString id;
      xml::Node* filter = createFilter( id );
      if ( brightness != 50 ) svg::applyGamma( filter, brightness );
      if ( contrast != 50 ) svg::applyContrast( filter, contrast );
      imgNode->addAttribute( OD_T("filter"), OdString(OD_T("url(#")) + id + OD_T(")") );
    }
  }

  TD_USING(Od2dExportDevice::polylineOut);
};

ODRX_CONS_DEFINE_MEMBERS(OdSvgDevice,OdGsBaseVectorizeDevice,RXIMPL_CONSTR);

inline OdSvgDevice* OdSvgView::device() { return static_cast<OdSvgDevice*>(OdGsBaseVectorizeView::device()); }

void OdSvgView::onTraitsModified()
{
  m_pLinetyper->enable();

  Od2dExportView::onTraitsModified();

  OdDbBaseDatabase* pDb = device()->drawContext()->giContext().database();

  if( pDb )
  {
    OdDbBaseDatabasePEPtr pDbPE( pDb );

    const OdGiSubEntityTraitsData& entityTraits = effectiveTraits();

    double       dCurLineScale = entityTraits.lineTypeScale();
    OdDbStub*    pLineStyle    = entityTraits.lineType();

    OdSvgDashArrayDescription lStyleDashes;

    bool bUseLS = false;

    if( !device()->getLineTypeDashArray( pLineStyle, lStyleDashes ) )
    {
      OdGiLinetype curLineType;

      bUseLS = pDbPE->getLineTypeById( pDb, pLineStyle, curLineType );

      if( bUseLS )
      {
        OdGiLinetypeDashArray arrDashes;
        curLineType.dashes(arrDashes);

        if( arrDashes.size() > 1 )
        {
          lStyleDashes.m_bUseDashArray = true;
        }

        for( OdUInt32 j = 0; j < arrDashes.size(); j++ )
        {
          if( (arrDashes[j].shapeNumber != 0) || !arrDashes[j].textString.isEmpty() )
          {
            lStyleDashes.m_bUseDashArray = false;
            lStyleDashes.m_dDashArray.clear();
            lStyleDashes.m_dDashArrayOffset = 0;
            break;
          }

          double dLength = arrDashes[j].length;

          if( OdZero(dLength) )
          {
            dLength += curLineType.patternLength()*0.00001;
          }

          if( (j == 0) && (dLength < 0) )
          {
            lStyleDashes.m_dDashArrayOffset = fabs( dLength );
          }
          else
          {
            lStyleDashes.m_dDashArray.push_back( fabs( dLength ) );
          }
        }
      }
      else
      {
        lStyleDashes.m_bUseDashArray = false;
      }

      device()->setLineTypeDashArray( pLineStyle, lStyleDashes );
    }

    if( lStyleDashes.m_bUseDashArray )
    {
      if( device()->_currentView ) 
      {
        OdGsView* pCurView = dynamic_cast<OdGsView*>(device()->_currentView);

        OdGePoint2d ptScale;
        pCurView->getNumPixelsInUnitSquare( OdGePoint3d(0,0,0), ptScale, false);

        dCurLineScale *= ptScale.x;
      }

      lStyleDashes.m_dDashArrayOffset *= dCurLineScale;

      for( OdUInt32 k = 0; k < lStyleDashes.m_dDashArray.size(); k++ )
      {
        lStyleDashes.m_dDashArray[k] *= dCurLineScale;
      }

      m_pLinetyper->disable();
    }

    device()->setDashArrayData( lStyleDashes );
  }
}

void OdSvgView::draw(const OdGiDrawable* pDrawable)
{
  m_pLinetyper->enable();

  OdGiBaseVectorizer::draw(pDrawable);
}

// Inlined functions containing static variables cause warnings on the mac (bad semantics), 
// so move createClipPath and createFilter down to here.

OdString OdSvgDevice::createClipPath( int rings, const int* counts, const OdGePoint2d* pp )
{
  addDefs();
  OdString clipId; clipId.format( OD_T("clipId%d"), _clipCount++ );
  xml::Node* clipPath = _defs->addChild( new xml::Node( OD_T("clipPath") ) );
  clipPath->addAttribute( OD_T("id"), clipId );
  svg::Path* path = new svg::Path;
  clipPath->addChild( path );
  int currentPos = 0;
  for ( int i = 0; i < rings; i++ )
  {
    path->addLoop( counts[i], pp + currentPos );
    currentPos += counts[i];
  }
  return clipId;
}

xml::Node* OdSvgDevice::createFilter( OdString& id )
{
  addDefs();
  xml::Node* filter = _defs->addChild( OD_T("filter") );
  id.format( OD_T("filterId%d"), _filterCount++ );
  filter->addAttribute( OD_T("id"), id );
  return filter;
}

xml::Node* OdSvgDevice::createGradient(OdString& id, const OdCmEntityColor& col1, const OdCmEntityColor& col2, const OdGePoint3d& p1, const OdGePoint3d& p2)
{
  addDefs();
  if (_gradientCount == 0)
  {
    xml::Node* filter = _defs->addChild( L"filter" );
    filter->addAttribute( L"id", L"A" );
    xml::Node* cm = filter->addChild(L"feColorMatrix");
    cm->addAttribute(L"type", L"matrix");
    cm->addAttribute(L"values", L"\n1 0 0 0 0\n0 1 0 0 0\n0 0 1 0 0\n1 1 1 1 0\n0 0 0 0 1\n");

    xml::Node* filter1 = _defs->addChild( L"filter" );
    filter1->addAttribute(L"id", L"colorAdd");
    xml::Node* comp = filter1->addChild(L"feComposite");
    comp->addAttribute(L"in", L"SourceGraphic");
    comp->addAttribute(L"in2", L"BackgroundImage");
    comp->addAttribute(L"operator", L"arithmetic");
    comp->addAttribute(L"k1", L"0");
    comp->addAttribute(L"k2", L"1");
    comp->addAttribute(L"k3", L"1");
    comp->addAttribute(L"k4", L"0");
  }
  xml::Node* grad = _defs->addChild( L"linearGradient" );
  id.format( L"g_%d", _gradientCount++ );
  grad->addAttribute( L"id", id );
  grad->addAttribute( L"gradientUnits", L"userSpaceOnUse");
  grad->addAttribute( L"x1", p1.x);
  grad->addAttribute( L"y1", p1.y);
  grad->addAttribute( L"x2", p2.x);
  grad->addAttribute( L"y2", p2.y);
  xml::Node* stop1 = grad->addChild(L"stop");
  stop1->addAttribute(L"offset", L"0");
  stop1->addAttribute(L"stop-color", svg::formatColor(col1));
  stop1->addAttribute(L"stop-opacity", L"1");
  xml::Node* stop2 = grad->addChild(L"stop");
  stop2->addAttribute(L"offset", L"1");
  stop2->addAttribute(L"stop-color", svg::formatColor(col2));
  stop2->addAttribute(L"stop-opacity", (col1 == col2) ? L"0" : L"1");
  return grad;
}

void OdSvgView::setOuterClipPath()
{
  if ( isNonRectClipped() )
  {
    OdGePoint2dArray nrcPoints;
    OdIntArray nrcCounts;
    viewportClipRegion(nrcCounts, nrcPoints);
    device()->setClipPath(
      nrcCounts.size(), nrcCounts.getPtr(), nrcPoints.getPtr() );
  }
  else
  {
    OdGsDCPoint p1, p2; screenRect( p1, p2 );
    device()->setClipRect( p1, p2 );
  }
}

void OdSvgView::beginViewVectorization()
{
  Od2dExportView::beginViewVectorization();
  if (device()->_svgRoot) // if we were called for real rendering
    setOuterClipPath();
  device()->_currentView = this;
}
void OdSvgView::endViewVectorization()
{
  Od2dExportView::endViewVectorization();
  device()->_currentView = 0;
}

void OdSvgView::shell(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
{
  if (const OdGiDrawable* d = device()->drawContext()->currentDrawable())
    device()->_bShellMode = (d->isA()->name() == L"AcDbHatch" || d->isA()->name() == L"AcDbMPolygon");
  Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
  device()->_bShellMode = false;
}

class SvgExportCommand : public OdEdCommand
{
public:
  const OdString groupName() const { return OD_T("EXPORTS"); }
  const OdString globalName() const { return OD_T("svgout"); };
  void execute(OdEdCommandContext* pCmdCtx)
  {
    bool fileExisted = true;
    try
    {
      OdString path = pCmdCtx->userIO()->getFilePath( OD_T("Enter file name:"), OdEd::kGfpForSave, OD_T("Svg file to save"),
        OD_T("svg"), OdString::kEmpty, OD_T("Scalable Vector Graphics (*.svg)|*.svg||"));

      fileExisted = odrxSystemServices()->accessFile(path, 0);
      // Create output file
      OdStreamBufPtr file = odrxSystemServices()->createFile( path, Oda::kFileWrite, Oda::kShareDenyReadWrite, Oda::kCreateAlways);
      if ( file.isNull() ) return;
      // Create export device
      OdGsDevicePtr dev = OdSvgDevice::createObject();
      // Setup output stream
      dev->properties()->putAt( OD_T("Output"), file.get() );
      // size of pixel in device units
      double LineWeightScale = pCmdCtx->userIO()->getReal( OD_T("LineWeight scale <1.0>:"), OdEd::kInpDefault, 1.0 );
      dev->properties()->putAt( OD_T("LineWeightScale"), OdRxVariantValue( LineWeightScale ) );
      // number of digits in float values (6 by default) 
      OdInt32 Precision = pCmdCtx->userIO()->getInt( OD_T("Floating point precision <6>:"), OdEd::kInpDefault, 6 );
      dev->properties()->putAt( OD_T("Precision"), OdRxVariantValue( Precision ) );
      // where copy images
      OdString base = path,
               fmt = OD_T("ImageBase:");
      int slash1 = base.reverseFind(L'/');
      int slash2 = base.reverseFind(L'\\');
      base = base.left( odmax(slash1,slash2) );
      if (!base.isEmpty())
        fmt.format( OD_T("ImageBase <%ls>:"), base.c_str() );
      base = pCmdCtx->userIO()->getString( fmt, OdEd::kGstAllowSpaces, base );
      dev->properties()->putAt( OD_T("ImageBase"), OdRxVariantValue( base ) );
      // prefix to prepend to image name
      OdString ImageUrl = pCmdCtx->userIO()->getString( OD_T("ImageUrl <./>:"), 0, OD_T("./") );
      dev->properties()->putAt( OD_T("ImageUrl"), OdRxVariantValue( ImageUrl ) );
      // default image format
      OdString DefaultImageExt = pCmdCtx->userIO()->getString( OD_T("Default image extension <.png>:"), 0, OD_T(".png") );
      dev->properties()->putAt( OD_T("DefaultImageExt"), OdRxVariantValue( DefaultImageExt ) );
      // generic font family
      OdString GenericFontFamily = pCmdCtx->userIO()->getString( OD_T("GenericFontFamily <sans-serif>:"), 0, OD_T("sans-serif") );
      dev->properties()->putAt( OD_T("GenericFontFamily"), OdRxVariantValue( GenericFontFamily ) );
      // Set active palette
      OdRxVariantValue tmpVar = (OdRxVariantValue)dev->properties()->getAt("BackGround");
      ODCOLORREF bg = tmpVar.get()->getInt32();
      dev->setBackgroundColor(bg);
      dev->setLogicalPalette( odcmAcadLightPalette(), 256 );
      // Prepare database context for device
      OdDbBaseDatabasePEPtr db = pCmdCtx->baseDatabase();
      OdGiDefaultContextPtr pDwgContext = db->createGiContext(pCmdCtx->baseDatabase());
      // do not render paper
      pDwgContext->setPlotGeneration( true );
      pDwgContext->setPaletteBackground(bg);

      pDwgContext->setHatchAsPolygon(OdGiDefaultContext::kHatchPolygon);

      db->loadPlotstyleTableForActiveLayout(pDwgContext, pCmdCtx->baseDatabase());

// vladimirv, reported: OdaJira:CORE-12436
      double h = 768, w = 1024;
      double dwgUnitsIn1SVGPixel = 
          pCmdCtx->userIO()->getReal(OD_T("Scale factor (Number of drawing units in 1 SVG pixel) <0.0 (Fit-to-page)>:"), OdEd::kInpNonNeg, 0.0);
      if (!OdPositive(dwgUnitsIn1SVGPixel))
      {
        // Setup device coordinate space
        h = pCmdCtx->userIO()->getReal(OD_T("Output height (in pixels) <768>:"), OdEd::kInpDefault, 768);
        w = pCmdCtx->userIO()->getReal(OD_T("Output width (in pixels) <1024>:"), OdEd::kInpDefault, 1024);
      }

      // whether turn on hidden line removing
      bool bUseSoftHLR =
        (pCmdCtx->userIO()->getKeyword(L"Use hidden line removing for according viewports <YES>:", L"NO YES", 1) != 0);
      dev->properties()->putAt(L"UseHLR", OdRxVariantValue(bUseSoftHLR));

      bool bEnableGouraudShading =
        (pCmdCtx->userIO()->getKeyword(L"Use blended gradients for complex gradient fill <YES>:", L"NO YES", 1) != 0);
      dev->properties()->putAt(L"EnableGouraudShading", OdRxVariantValue(bEnableGouraudShading));

      // Prepare the device to render the active layout in this database.
      OdGsDevicePtr wrapper = db->setupActiveLayoutViews(dev, pDwgContext);
      // creating render device for rendering the shaded viewports
      OdSvgDevice* pDeviceSvg = (OdSvgDevice*)dev.get();
      pDeviceSvg->setRenderDevice();
      OdRxObjectPtr pLayout = OdDbBaseDatabasePEPtr(pCmdCtx->baseDatabase())->currentLayout(pCmdCtx->baseDatabase());
      pDeviceSvg->_plotLineweights = OdDbBaseLayoutPEPtr(pLayout)->printLineweights(pLayout);
      for (int nDevice = 0; nDevice < pDeviceSvg->numRenderDevices(); nDevice++)
      {
        OdGsDevice* pRenderDevice = pDeviceSvg->renderDeviceAt(nDevice);

        pRenderDevice->setBackgroundColor(bg);
        pRenderDevice->setLogicalPalette(odcmAcadLightPalette(), 256);
        OdGsDevicePtr pLayoutHelper = db->setupActiveLayoutViews(pRenderDevice, pDwgContext);
        pDeviceSvg->setRenderLayoutHelperAt(nDevice, pLayoutHelper.get());
      }

// vladimirv, reported: OdaJira:CORE-12436
      if (OdPositive(dwgUnitsIn1SVGPixel))
      {
        OdGePoint2d pixelDensity;
        const OdGsView* pGsView = pDeviceSvg->viewAt(0);
        pGsView->getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, pixelDensity, false);

        if (OdPositive(pixelDensity.x) && OdPositive(pixelDensity.y))
        {
          OdGsDCRect rectDc;
          pGsView->getViewport(rectDc);
          if (rectDc.m_max.y < rectDc.m_min.y)
            std::swap(rectDc.m_max.y, rectDc.m_min.y);

          const double dwgUnitsInDisplayPixel = 1.0 / pixelDensity.x;
          const double widthDbl = (rectDc.m_max.x - rectDc.m_min.x) * dwgUnitsInDisplayPixel / dwgUnitsIn1SVGPixel;
          const double heightDbl = (rectDc.m_max.y - rectDc.m_min.y) * dwgUnitsInDisplayPixel / dwgUnitsIn1SVGPixel;
          if (widthDbl <= 2. || heightDbl <= 2.)
              pCmdCtx->userIO()->putString(OD_T("\nCalculated SVG page size is too small for this scale. Default page size (1024x768) is used."));
          else if (widthDbl > LONG_MAX || heightDbl > LONG_MAX)
              pCmdCtx->userIO()->putString(OD_T("\nCalculated SVG page size is too large for this scale. Default page size (1024x768) is used."));
          else
          {
            w = widthDbl;
            h = heightDbl;
          }
        }
      }

      wrapper->onSize( OdGsDCRectDouble( 0, w, h, 0 ) );
      // Initiate rendering.
      wrapper->update( 0 );
    }
    catch( const OdEdCancel )
    {
      if ( !fileExisted )
      {
        // here file should be deleted
      }
    }
  }
};

class SvgExportModule : public OdGsBaseModule
{
  OdStaticRxObject<SvgExportCommand> m_Command;
public:
  virtual OdSmartPtr<OdGsBaseVectorizeDevice> createDeviceObject()
  {
    return OdSvgDevice::createObject();
  }
  virtual OdSmartPtr<OdGsViewImpl> createViewObject()
  {
    return OdSvgView::createObject();
  }
  virtual OdSmartPtr<OdGsBaseVectorizeDevice> createBitmapDeviceObject()
  {
    return (OdGsBaseVectorizeDevice*)0;
  }
  virtual OdSmartPtr<OdGsViewImpl> createBitmapViewObject()
  {
    return (OdGsViewImpl*)0;
  }
  virtual void initApp()
  {
    OdGsBaseModule::initApp();
    OdSvgDevice::rxInit();
    OdEdCommandStackPtr pCommands = odedRegCmds();
    pCommands->addCommand( &m_Command );
  }
  virtual void uninitApp()
  {
    OdEdCommandStackPtr pCommands = odedRegCmds();
    pCommands->removeCmd( m_Command.groupName(), m_Command.globalName() );
    OdSvgDevice::rxUninit();
    OdGsBaseModule::uninitApp();
  }
};
}
using namespace TD_SVG_EXPORT;
ODRX_DEFINE_DYNAMIC_MODULE(SvgExportModule)

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) //  MacOS X dynamic library loading. //#ifdef _TOOLKIT_IN_DLL_

int __stdcall DllMain( void*, unsigned long, void* ){ return 1; }

#endif //_TOOLKIT_IN_DLL_ //  MacOS X dynamic library loading.
