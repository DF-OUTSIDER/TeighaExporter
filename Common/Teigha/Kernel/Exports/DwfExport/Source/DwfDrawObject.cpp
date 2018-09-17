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



//////////////////////////////////////////////////////////////////////
//
// DwfDrawObject.cpp: implementation of the GsDwfFactory, OdDwfDevice, OdDwfView classes.
//
//////////////////////////////////////////////////////////////////////


#include "OdaCommon.h"

// to avoid STL warnings (if not to include STL here, it will include itself from whip tk)
#define STL_USING_VECTOR
#define STL_USING_LIST
#define STL_USING_MAP
#include "OdaSTL.h"

#ifdef _MSC_VER
#pragma warning(push)
//#include <yvals.h>              // warning numbers get enabled in yvals.h 
#pragma warning(disable: 4290)
#endif // _MSC_VER

#include "2dExportDevice.h"
#include "2dSupport.h"
#include "DwfDrawObject.h"
#include "DwfSupport.h"
#include "DwfExportImpl.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#include "RxObjectImpl.h"
#include "Ge/GeCircArc3d.h"
#include "ColorMapping.h"


#include "Gi/GiTextStyle.h"
//#include "OdFont.h"
#include "Ge/GeLineSeg2d.h"

#include "Gs/GsBMPDevice.h"
#include "RxVariantValue.h"

#include "Ge/GeEllipArc3d.h"
#include "Ge/GeLineSeg3d.h"

#include "StaticRxObject.h"

#include "DbHyperlink.h"
#include "XAML/XamlFile.h"

//////////////////////////////////////////////////////////////////////////////////
namespace TD_DWF_EXPORT {

typedef std::map<OdString, int> mapLayers;
static mapLayers mapLayer;

typedef std::map<OdString, int> mapURLs;
static mapURLs mapURL;

static bool g_bUrlTrigger;                  // if true - url list was changed

typedef OdArray<DwfUrl> vectorDwfUrls;
static vectorDwfUrls g_UrlList;            // List of urls witch are in force right now
const int iDwfUrlOff = (-1);

//static OdGiDrawable* p_Drawable;
static OdArray<OdGiDrawable*> g_Drawables;

const int nFontMultipliedByFactor (1024);         // Some Font's values may range from a value of one (1/1024 
                                                  //  of normal) to 65535 (64 times normal). 
const int    nWtAngleMaxValue  (65535);           // The max legal value of angle.
const double dWtAngleMeasureOfDegree (65535/360); // Measure in 360/65636ths of a degree.


//////////////////////////////////////////////////////////////////////////////////
// OdDwfDevice
//////////////////////////////////////////////////////////////////////////////////

OdDwfDevice::OdDwfDevice() : Od2dExportDevice(kDWFDevice)
  , m_dwf(NULL)
  , m_pWhipFactory(0)
  , m_bWasNrcViewportClip(false)
  , m_bKindOfRun(Main_run)
  , m_bTurnVisibilityOn(false)
  , m_dThickness(.0)
  , m_curRGB(0)
  , m_bDrawShellAsRaster(false)
{
  m_lineStyle.join = WT_Line_Style::Undefined_Joinstyle;
  m_lineStyle.startCap = WT_Line_Style::Undefined_Capstyle;
  m_lineStyle.endCap = WT_Line_Style::Undefined_Capstyle;
}

//====================================================================
//
OdGsDevicePtr OdDwfDevice::createObject()
{
  OdGsDevicePtr pRes = OdRxObjectImpl<OdDwfDevice, OdGsDevice>::createObject();
  return pRes;
}


//====================================================================
//
OdGsViewPtr OdDwfDevice::createView(const OdGsClientViewInfo* pViewInfo, bool bEnableLayVisPerView)
{
  OdGsViewPtr pView = OdDwfView::createObject();
  OdDwfView* pMyView = static_cast<OdDwfView*>(pView.get());

  pMyView->init(this, pViewInfo, bEnableLayVisPerView);

  pMyView->screenOutput().setDestGeometry(*this);

  return pMyView;
}


//====================================================================
//
void OdDwfDevice::setContext(CDwfExportImpl* dwf)
{
  m_dwf = dwf;

  // to init layout environment
  mapLayer.clear();
  mapURL.clear();
  g_UrlList.clear();

  DwfUrl dwfUrl;
  dwfUrl.iUrlNum = iDwfUrlOff;
  g_UrlList.push_back(dwfUrl);      // "URL Link" attribute is inactive


  g_bUrlTrigger = false;            // "URL Link" attribute is inactive 
                                    //  (no URL is attached to subsequent geometry)

  dc_urls_remove_all();
  dc_layers_remove_all();
}


//====================================================================
//
WT_File* OdDwfDevice::getWtFile()
{
  return getExportImpl()->getWtFile();
}

//====================================================================
//
void OdDwfDevice::update(OdGsDCRect* pUpdatedRect, KindOfRun bKindOfRun)
{
  // to begin paint

  setRun(bKindOfRun);
  getExportImpl()->getDevice()->update(pUpdatedRect);
}

int OdDwfDevice::numRenderDevices()
{
  return IsMainRun() ? Od2dExportDevice::numRenderDevices() : 0;
}

//====================================================================
//
void OdDwfDevice::onSize(const OdGsDCRect& outputRect)
{
  Od2dExportDevice::onSize(outputRect);
}


int OdDwfDevice::getSupportedColorBits(int nColorBits, bool bTransparency) const
{
  return ((bTransparency) 
              ? Od2dExportDevice::orgBGRA             // always 32 bits in case of transparency 
              : Od2dExportDevice::getSupportedColorBits(nColorBits, bTransparency)
         );
}


void OdDwfDevice::push_Url()
{
  if (g_bUrlTrigger)                       // Is url trigger on? 
  {                                   //  in this case to out the current url list
                                      //  Note: the current url list can be empty
      set_Urls();
      g_bUrlTrigger = false;
  }
}

void OdDwfDevice::set_Urls()
{
    dc_url_clear();       // clears the current url list

  // Iterator is used to loop through the vector.
  vectorDwfUrls::const_iterator theIterator;

  for ( theIterator = g_UrlList.begin(); 
        theIterator != g_UrlList.end();
        theIterator++ )
  {
    DwfUrl dwfUrl = *theIterator;

    if (dwfUrl.iUrlNum > 0)
    {
      WT_URL_Item wtUrlItem( dwfUrl.iUrlNum, OdW2US(dwfUrl.sName.c_str()), OdW2US(dwfUrl.sDescription.c_str()) );
      dc_url_item( wtUrlItem );
    }                                      
  }
}



//////////////////////////////////////////////////////////////////////////////////
// OdDwfView
//////////////////////////////////////////////////////////////////////////////////

inline OdGeMatrix3d normalizeCs(const OdGeMatrix3d& xCs)
{
  OdGeMatrix3d xRes;
  OdGeVector3d xAxiz = xCs.getCsXAxis(), yAxis =xCs.getCsYAxis();
  xRes.setCoordSystem(
    xCs.getCsOrigin(),
    xAxiz,
    yAxis,
    xAxiz.crossProduct(yAxis).normal(OdGeTol(10e-100)) * yAxis.length());
  return xRes;
}

void OdDwfView::initialize()
{
  device()->set_Color(device()->getBackgroundColor());
  device()->set_LineWeight(0.0);
  device()->setThickness(0.0);
  device()->setTurnVisibilityOn(true);
  device()->setWasNrcViewportClip(false);

  // LineStyle
  LineStyle lineStyle;
  lineStyle.join     = WT_Line_Style::Round_Join;
  lineStyle.startCap = WT_Line_Style::Round_Cap;
  lineStyle.endCap   = WT_Line_Style::Round_Cap;

  device()->set_LineStyle(lineStyle);
}

void OdDwfView::update()
{
  initialize();

  setViewportBorderVisibility(false);

  g_Drawables.clear();
  g_Drawables.push_back(NULL);
  
  int nClips = putViewport();           // Sets the active viewport for clipping of drawing 
                                        //  in order to get the same result as on the screen
  Od2dExportView::update();

  // Restore clipping
  while (nClips > 0)
  {
    device()->dc_popClip();
    nClips--;
  }

  g_Drawables.removeLast();
  ODA_ASSERT(  g_Drawables.empty() );
}

//====================================================================
//
void OdDwfDevice::onTraitsModified(const OdGiSubEntityTraitsData& currTraits)
{
  Od2dExportDevice::onTraitsModified(currTraits);
  // color
  if(currTraits.trueColor().isByColor())
  {
    set_Color(ODTOCOLORREF(currTraits.trueColor()));
  }
  else
  {
    set_ColorIndex(currTraits.color());
  }

  if(IsMainRun())
  {
    // LineStyle
    LineStyle lineStyle;
    lineStyle.join     = WT_Line_Style::Round_Join;
    lineStyle.startCap = WT_Line_Style::Round_Cap;
    lineStyle.endCap   = WT_Line_Style::Round_Cap;

    set_LineStyle(lineStyle);

    set_Layer(currTraits.layer());

    m_dThickness = currTraits.thickness();
  }
}

void OdDwfView::onTraitsModified()
{
  if (isOutputSuppressed())
  { //optimisation.
    OdGsBaseVectorizer::onTraitsModified();
    return;
  }

  Od2dExportView::onTraitsModified();

  const OdPsPlotStyleData& plotStyle = effectivePlotStyle();
  const OdGiSubEntityTraitsData& entityTraits = effectiveTraits();

  // color
  if (userGiContext()->isPlotGeneration())
  {
    if(plotStyle.color().isByColor())
    {
      device()->set_Color(ODTOCOLORREF(plotStyle.color()));
    }
    else
    {
      device()->set_ColorIndex(plotStyle.color().colorIndex());
    }
  }
  else
  {
    if (entityTraits.trueColor().isByColor())
      device()->set_Color(ODTOCOLORREF(entityTraits.trueColor()));
    else
      device()->set_ColorIndex(entityTraits.color());
  }

  device()->set_LineWeight(lineweightToPixels(plotStyle.lineweight()));
  device()->set_Layer(entityTraits.layer());
}

void OdDwfDevice::set_LineWeight(double lineWeight)
{
  if (IsNonprocessRun()) return;
  dc_lineWeight(RoundDoubleToWTInteger32(lineWeight * getExportImpl()->getParams().lineweightScale()));
}

void OdDwfDevice::setThickness(double thickness)
{
  m_dThickness = thickness;
}

void OdDwfDevice::setTurnVisibilityOn(bool turnVisibilityOn)
{
  m_bTurnVisibilityOn = turnVisibilityOn;
}

//====================================================================
//
void OdDwfDevice::set_Color(ODCOLORREF rgb)
{
  if(m_curRGB != rgb)
  {
    // If the colour was changed
    m_curRGB = rgb;

    if(IsNonprocessRun())
      getExportImpl()->colorPresetting(rgb);
    else
      dc_color(rgb);
  }
}


//====================================================================
//
void OdDwfDevice::set_ColorIndex(OdUInt16 colorIndex)
{
  set_Color(getColor(colorIndex));
}

//====================================================================
//
void OdDwfDevice::set_LineStyle(const LineStyle& lineStyle)
{
  if (IsNonprocessRun()) return;
  if (   m_lineStyle.join     != lineStyle.join
      || m_lineStyle.startCap != lineStyle.startCap
      || m_lineStyle.endCap   != lineStyle.endCap
     )
  {
    m_lineStyle.join     = lineStyle.join;
    m_lineStyle.startCap = lineStyle.startCap;
    m_lineStyle.endCap   = lineStyle.endCap;

    dc_lineStyle(lineStyle);
  }
}

//====================================================================
//
void OdDwfDevice::set_Layer(OdDbStub* layer)
{
  if (IsNonprocessRun()) return;
  // The Set Layer function is used to delimit a group of opcodes.
  if(!layer)
    return;

  //OdDbLayerTableRecordPtr pLayer;
  //pLayer = OdDbObjectId(layer).openObject();
  OdRxObjectPtr pL = (OdDbBaseDatabasePEPtr((getExportImpl()->getParams().database())))->openObject(layer);
  OdDbBaseLayerPEPtr pLayer(pL);
  if(pLayer.get())
  {
    //OdString strLayerName = pLayer->getName();
    OdString strLayerName = pLayer->name(pL.get());

    int layer_num;      // The layer number which will be assigned to the geometric primitives 
                        //  that are subsequently created. 
                        //  NOTE: Do not use layer numbers less than 1. 

    mapLayers::const_iterator it;
    it = mapLayer.find( strLayerName );       // If the Layer exists in the map...

    if (it != mapLayer.end())
    {
      layer_num = (*it).second;               // ... to get Layer number by Name
    }
    else
    {
      layer_num = (int)mapLayer.size() + 1;        // Otherwise to set number and to add into the map
      mapLayer[strLayerName] = layer_num;
    }

    ODA_ASSERT(layer_num >= 1);

    if(layer_num < 1) return;

    //Layer.set_visibility(!pLayer->m_Invisible);
    if(!getExportImpl()->getParams().skipLayerInfo()) // MKU 0703/05 - ti support this property
      dc_layer(layer_num, strLayerName.c_str());
  }
}

//====================================================================
//
// draw()
//
void OdDwfView::draw(const OdGiDrawable* pDrawable)
{
  OdString sName = pDrawable->isA()->name();
  bool bPushDrawable = sName.find(L"Text") >= 0; // = sName == L"AcDbText";
  if (bPushDrawable)
    g_Drawables.push_back(const_cast<OdGiDrawable*>(pDrawable)); // CORE-12622 CORE-12630
  else
    g_Drawables.push_back(NULL);

  process_Url(pDrawable);       // Checks url data existence
  Od2dExportView::draw(pDrawable);
  postprocess_Url();

  g_Drawables.removeLast();
}


//====================================================================
//
// putViewport()
//
// Sets the active viewport for clipping of drawing in order to get the same result as on the screen
//
int OdDwfView::putViewport()
{
  int nClips = 0;
  if(device()->IsNonprocessRun())
  {
    return nClips;
  }

  if(isNonRectClipped())
  {
    OdGsDCPointArray nrcPoints;
    OdIntArray nrcCounts;
    viewportClipRegion(nrcCounts, nrcPoints);
    device()->dc_pushClip(nrcCounts.size(), nrcCounts.getPtr(), nrcPoints);
    device()->setWasNrcViewportClip(true);
  }
  else
  {
    int counts = 4;
    OdGsDCPointArray points;
    points.resize(4);
    screenRect(points[0], points[2]);
    points[1] = OdGsDCPoint(points[2].x, points[0].y);
    points[3] = OdGsDCPoint(points[0].x, points[2].y);
    device()->dc_pushClip(1, &counts, points);
    device()->setWasNrcViewportClip(false);
  }
  nClips++;

  { // AMark : Add clipping of entire viewport for all available views inside layout
    OdGsDCPoint outMinPt(device()->outputRect().m_min);
    OdGsDCPoint outMaxPt(device()->outputRect().m_max);
    OdGsDCPoint inMinPt, inMaxPt;
    screenRect(inMinPt, inMaxPt);
    if (inMinPt.x < outMinPt.x || inMaxPt.x > outMaxPt.x ||
        inMaxPt.y < outMaxPt.y || inMinPt.y > outMinPt.y)
    {
      const int nrcCountsOut = 4;
      OdGsDCPointArray nrcPointsOut;
      nrcPointsOut.resize(4);
      nrcPointsOut[0] = outMinPt;
      nrcPointsOut[1].x = outMaxPt.x;
      nrcPointsOut[1].y = outMinPt.y;
      nrcPointsOut[2] = outMaxPt;
      nrcPointsOut[3].x = outMinPt.x;
      nrcPointsOut[3].y = outMaxPt.y;
      device()->dc_pushClip(1, &nrcCountsOut, nrcPointsOut);
      nClips++;
    }
  }

  double xform[16];
  convDeviceMatrixToWt(xform);

  device()->getWtFile()->desired_rendition().viewport().viewport_units().set_application_to_dwf_transform(WT_Matrix(xform));
  //device()->getWtFile()->desired_rendition().viewport().viewport_units().set_units(getUnitNameString(device()->getExportImpl()->getParams().pDb->getINSUNITS())); // before 05.01.2012
  device()->getWtFile()->desired_rendition().viewport().viewport_units().set_units(getUnitNameString((OdDbBaseDatabasePEPtr((device()->getExportImpl()->getParams().database())))->getUnits(device()->getExportImpl()->getParams().database())));  // dgn dwf export
  return nClips;
}



//====================================================================
//
void OdDwfView::convViewportToWt(WT_Logical_Point& wtMin, WT_Logical_Point& wtMax) const
{
  OdGsDCPoint min, max;
  screenRect(min, max);

  RoundPoint(wtMin, OdGePoint2d(min.x, min.y));
  RoundPoint(wtMax, OdGePoint2d(max.x, max.y));
}


//====================================================================
//
// convDeviceMatrixToWt()
//
//  Converts device matrix for view to Wt matrix
//
void OdDwfView::convDeviceMatrixToWt(double* xform) const
{
  OdGeMatrix3d matrix(worldToDeviceMatrix());
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      //transposed matrix
      xform[i * 4 + j] = matrix(j, i);
    }
  }
}


//====================================================================
//
// view space to normalized device space
//
OdGeMatrix3d OdDwfView::projectionMatrixMy() const
{
  OdGeMatrix3d res;

    res(0,0) = (dcUpperRight().x - dcLowerLeft().x) / fieldWidth();
    res(1,1) = (dcUpperRight().y - dcLowerLeft().y) / fieldHeight();
    res(2,2) = 1.0;
    res(0,3) = (dcUpperRight().x + dcLowerLeft().x) / 2.0;
    res(1,3) = (dcUpperRight().y + dcLowerLeft().y) / 2.0;
    res(2,3) = 0;  

  return res;
}

//==========================================================================================
//
// shxText()
//
void OdDwfDevice::shxText(const OdGePoint3d& position,
                          const OdGeVector3d& u, 
                          const OdGeVector3d& v,
                          const OdChar* msg, 
                          OdInt32 length, 
                          bool raw, 
                          const OdGiTextStyle* pStyle,
                          const OdGeVector3d* pExtrusion)
{
  if (IsNonprocessRun()) return;
  if (getExportImpl()->getParams().exportInvisibleText()  && getExportImpl()->getParams().format() != DW_XPS)
  {
    turnVisibilityOn(false);                      // turn visibility on
    
    bool bASCII(true);
    
    OdString sProcessedText;
    OdArray<OdUInt16>    arrayUnicode;
    
    if(!bASCII || !textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText, arrayUnicode, true))
    {
      bASCII = false;
      arrayUnicode.clear();
      
      textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText,
        arrayUnicode,
        false);
    }
    
    Od2dExportView* p2dExpView = static_cast<Od2dExportView*>(viewAt(0));
    p2dExpView->setText( position, u, v, 
      (bASCII) ? sProcessedText.c_str() : msg, 
      (bASCII) ? sProcessedText.getLength() : length, 
      raw, pStyle, false, NULL, 
      (bASCII) ? NULL : &arrayUnicode
      );
    
    turnVisibilityOn(true);                       // turn visibility off
  }

  OdGiGeometrySimplifier::textProc(position, u, v, msg, length, raw, pStyle, pExtrusion);
}


//====================================================================
//
//
// setText()
//

//void odExtentsBox(const OdGiTextStyle* pStyle, const OdChar* pStr, int nStrLen, OdUInt32 flags, OdDbDatabase* pDb, OdGePoint3d& min, OdGePoint3d& max, OdGePoint3d* pEndPos);
static bool hasInvalidCharacters(OdFont* f, const OdChar* msg, int length)
{
  OdCharArray chars;
  f->getAvailableChars(chars);
  if (length < 256)
  {
    for (int i = 0; i < length; ++i)
    {
      if (!chars.contains(msg[i]))
        return true;
    }
  }
  else
  {
    std::set<OdChar> s;
    for (int i = 0; i < length; ++i)
      s.insert(msg[i]);
    for (std::set<OdChar>::iterator pc = s.begin(); pc != s.end(); ++pc)
      if (chars.contains(*pc))
        return true;
  }
  return false;
}
void OdDwfView::setText(const OdGePoint3d& position,
                          const OdGeVector3d& u,      // normal
                          const OdGeVector3d& v,      // direction
                          const OdChar* msg, OdInt32 length, bool raw,
                          const OdGiTextStyle* pTextStyle,
                          bool  bBox,
                          const Od2dFont* pFont,
                          const OdArray<OdUInt16> *pUnicode)
{
  if (device()->IsNonprocessRun())
  {
    OdFont* f = pTextStyle->getFont();
    if (f->isShxFont() || hasInvalidCharacters(f, msg, length))
      return;
    OdTtfDescriptor descr;
    f->getDescriptor(descr);
    OdString typeface = descr.typeface();
    OdString fileName = descr.fileName();
    device()->getExportImpl()->collectSymbols(typeface, msg, length);
  }
  bool bASCII = !pUnicode || pUnicode->isEmpty();
  bool bInPlane(false);

  OdGeMatrix3d matrix = objectToDeviceMatrix();
  matrix.transposeIt();
  OdGeVector3d vZAxis = matrix.getCsZAxis();

  OdGiDrawable* pDrawable = g_Drawables.last();

  // if oblique to use Box as ACAD because oblique specificator (dwf format) dosn't work properly
  bool bBoxInCaseOfOblique(false);
  bool bBoxInCaseOfWidthScale(false);

  // 24.09.2004 GU changed cast from double to bool to comparing for the sake of
  // Borland portability. Although it is not very good to compare doubles in such manner.
  if (pFont && (!OdZero(pFont->m_oblique)) && (!OdZero(pFont->m_rotation)))
  {
    bBoxInCaseOfOblique = true;
  }
  if (pFont && (!OdZero(pFont->m_width_scale - 1)) && (!OdZero(pFont->m_rotation)))
  {
    bBoxInCaseOfWidthScale = true;
  }

  if (pDrawable && bBox && pFont)
  {
    // CORE-12622 CORE-12630 (u is normal to text direction in plane XY via vZAxis) //if (u.crossProduct(v).isParallelTo(vZAxis))  //if(u.isParallelTo(vZAxis))
      bInPlane = true;
    //else
    //{
    //  ODA_ASSERT_ONCE(false);
    //  bInPlane = false;
    //}
  }

  bool bSkip(false);

  OdGePoint3d ptMin, ptMax, pt1, pt3;

  // To transform the text insertion point to WT system
  OdGePoint3d ptPosition3d(position);
//  ptPosition3d.transformBy(objectToDeviceMatrix());

  WT_Logical_Point wtPosition;
  RoundPoint(wtPosition, OdGePoint2d(ptPosition3d.x, ptPosition3d.y));

  // There isn't a need of bounding box for shx or invisible text
  WT_Logical_Point* pBounds = NULL;
  WT_Logical_Point bounds[4];

  if (bBox && pFont)
  {
    OdFont* f = pTextStyle->getFont();
    int text_height = RoundDoubleToWTInteger32(pFont->m_height * (f->getHeight() - f->getInternalLeading()) / f->getAbove());
    if (text_height == 0)
      return;
    if (device()->getExportImpl()->getParams().format() == DW_XPS)
    {
      OdTtfDescriptor descr;
      if (f->isShxFont() || hasInvalidCharacters(f, msg, length) 
        || (f->getDescriptor(descr), !static_cast<WT_XAML_File*>(device()->getWtFile())->resolveFontUri(descr.typeface())))
      {
        device()->OdGiGeometrySimplifier::textProc( position, u, v, msg, length, raw, pTextStyle, 0);
        return;
      }
    }
    // if Text lies in the current view plane (2D) there is no need in the box.
    // Otherwise to put box for Text
    if (!bInPlane || bBoxInCaseOfOblique || bBoxInCaseOfWidthScale)
    {
      pBounds = bounds;
    }

    // Get text extents...
    context()->textExtentsBox( *pTextStyle, msg, length, raw ? kOdGiRawText : 0, ptMin, ptMax );

    if (!f->isShxFont() && !OdZero(pFont->m_rotation) && !OdZero(pFont->m_width_scale - 1))
    {
      // CORE-10942 vladimirv
      // When WT_Text class instance is created with bounding box, into which text must fit, then WT_Font::height() is ignored. 
      // textExtentsBox() returns bounding box around the text. But text can have different height depending on text (e.g. "a", "A", "g", "j").
      // Meanwhile DWF format demands bounding box which corresponds to string "Ag" (top bound corresponds to the height of capital letter, 
      // and bottom bound includes descent of letter).
      // In addition, the height of the bounding box should be scaled, as it was done for 'text_height' a little higher. 
      OdGePoint3d testPtMin, testPtMax;
      context()->textExtentsBox(*pTextStyle, L"Ag", 2, raw ? kOdGiRawText : 0, testPtMin, testPtMax);
      if (testPtMax.y > ptMax.y)
        ptMax.y = testPtMax.y;
      if (testPtMin.y < ptMin.y)
        ptMin.y = testPtMin.y;

      const double koeff1 = (f->getHeight() + f->getBelow()) / f->getAbove();
      const double koeff2 = (f->getHeight() - f->getInternalLeading() + f->getBelow()) / f->getAbove();
      ptMax.y *= koeff1;
      ptMin.y *= koeff2;
    }

    // ... Init text box...
    pt1.x = ptMax.x;
    pt1.y = ptMin.y;
    pt3.x = ptMin.x;
    pt3.y = ptMax.y;

    // ... Take into account an angle...
    OdGeMatrix3d xForm;
    xForm.setCoordSystem(position, u, v, u.crossProduct(v));
    ptMin.transformBy(xForm);
    ptMax.transformBy(xForm);
    pt1.transformBy(xForm);
    pt3.transformBy(xForm);

    // ... Transform text box to WT system...
    WT_Logical_Point wtMin;
    WT_Logical_Point wtMax;
    WT_Logical_Point wt1;
    WT_Logical_Point wt3;
    RoundPoint(wtMin, OdGePoint2d(ptMin.x, ptMin.y));
    RoundPoint(wtMax, OdGePoint2d(ptMax.x, ptMax.y));
    RoundPoint(wt1, OdGePoint2d(pt1.x, pt1.y));
    RoundPoint(wt3, OdGePoint2d(pt3.x, pt3.y));
  
    // ... and finally to get text bounds
    bounds[0].m_x = wtMin.m_x;
    bounds[0].m_y = wtMin.m_y;
    bounds[1].m_x = wt1.m_x;  
    bounds[1].m_y = wt1.m_y;  
    bounds[2].m_x = wtMax.m_x;
    bounds[2].m_y = wtMax.m_y;
    bounds[3].m_x = wt3.m_x;  
    bounds[3].m_y = wt3.m_y;  

    // Clip Text by View
    //  If any point of the text bounding box is in View clip rectangle
    //  then the text is inside a View clip rectangle partly at least.
    //  To skip only that text which completely lie out of this one.
    OdGePoint2d points[4];
    points[0] = OdGePoint2d(ptMin.x, ptMin.y);
    points[1] = OdGePoint2d(pt1.x, pt1.y);
    points[2] = OdGePoint2d(ptMax.x, ptMax.y);
    points[3] = OdGePoint2d(pt3.x, pt3.y);

    OdGsDCPoint ll, ur;
    screenRect(ll, ur);
    if (PolygonOutOfRect2d(4, points, toGePoint2d(ll), toGePoint2d(ur)))
    {
      bSkip = true;
    }
    else
    {
      std::auto_ptr<WT_Font> wtFont(device()->classFactory()->Create_Font());

      wtFont->set_fields_defined( WT_Font::FONT_ALL_FIELDS );

      WT_String fontName(OdW2US(pFont->m_font_name.c_str()));
      if (fontName.length() == 0)
      {
        return;       // Font Name must be not empty
      }
      wtFont->font_name().set( fontName );

          // pmk 08.30.2004 -- conversion from 'const int' to 'WT_Byte'
      wtFont->charset() = (WT_Byte) pFont->m_charset;
      wtFont->pitch()   = (WT_Byte) pFont->m_pitch;
      wtFont->family()  = (WT_Byte) pFont->m_family;

      wtFont->style().set_bold( pFont->m_bold );
      wtFont->style().set_italic( pFont->m_italic );
      wtFont->style().set_underlined( pFont->m_underline );

      double dWidthScaleFactor = pFont->m_width_scale;
      if (!OdZero(pFont->m_oblique) && !OdZero(dWidthScaleFactor - 1))
      {
        // CORE-12622  problem with oblique texts in TextTest.dwg
        //pFont->m_oblique = OdaPI2 - u.angleTo(v, OdGeVector3d::kZAxis);
        dWidthScaleFactor = u.length() / v.length();
        double dHeightCoeff = cos(pFont->m_oblique);
        dWidthScaleFactor /= dHeightCoeff;
        //const_cast<Od2dFont*>(pFont)->m_width_scale = dWidthScaleFactor;
        text_height = RoundDoubleToWTInteger32(  dHeightCoeff * v.length()
                                               * (f->getHeight() - f->getInternalLeading()) / f->getAbove());
        //const_cast<Od2dFont*>(pFont)->m_height = text_height;
      }
      wtFont->height() = text_height;

      // describes how the entire string is rotated from horizontal
      wtFont->rotation() = RoundDoubleToWTInteger16( pFont->m_rotation * dWtAngleMeasureOfDegree );
      
      wtFont->width_scale() = RoundDoubleToWTInteger16(dWidthScaleFactor * nFontMultipliedByFactor);

      // The desired rotation of the individual characters within the font
      wtFont->oblique() = RoundDoubleToWTInteger16( OdaToDegree( pFont->m_oblique ) * dWtAngleMeasureOfDegree );

      WT_Integer32 flags = pFont->m_flags;      // Flag bits reserved for the DWF generator’s use.
      wtFont->flags().set( flags );

      device()->dc_font( *wtFont );
    }
  }

  if (!bSkip)
  {
    if (bASCII)
    {
      std::auto_ptr<WT_Text> wtText(device()->classFactory()->Create_Text( 
        wtPosition, WT_String(OdW2US(msg)), pBounds, 0, 0, 0, 0 ));
      device()->dc_text( *wtText );
    }
    else
    {
      // Constructs a WT_String object from a null-terminated wide character string (i.e. Unicode.)
      WT_String wtUnicode(pUnicode->length(), pUnicode->asArrayPtr());
      std::auto_ptr<WT_Text> wtText(device()->classFactory()->Create_Text( 
        wtPosition, wtUnicode, pBounds, 0, 0, 0, 0 ));
      device()->dc_text( *wtText );
    }
  }
}


//====================================================================
//
//
// add_UrlToMap()
//
/*
void OdDwfView::add_UrlToMap(const OdString& sName, 
                       const OdString& sDescription, 
                       const OdString& sSubLocation)
*/
void OdDwfView::add_UrlToMap(DwfUrl& dwfUrl)
{
    OdString sKey( dwfUrl.sName + dwfUrl.sDescription );

    int url_num;        // The URL number which will be assigned to the geometric primitives 
                        //  that are subsequently created. 
                        //  NOTE: Do not use layer numbers less than 1. 

    mapURLs::const_iterator it;
    it = mapURL.find( sKey );               // If the URL exists in the map...

    if (it != mapURL.end())
    {
      url_num = (*it).second;               // ... to get Layer number by Name
    }
    else
    {
      url_num = (int)mapURL.size() + 1;          // Otherwise to set number and to add into the map
      mapURL[sKey] = url_num;
    }

    ODA_ASSERT(url_num >= 1);

    dwfUrl.iUrlNum = url_num;
}



//====================================================================
//
// process_Url()
//
//  Checks url data existence
//
void OdDwfView::process_Url(const OdGiDrawable* pDrawable)
{
  DwfUrl dwfUrl;

  dwfUrl.iUrlNum = (g_bUrlTrigger) ? 0 : iDwfUrlOff;

  OdDbEntityHyperlinkPEPtr hpe = pDrawable->queryX(OdDbEntityHyperlinkPE::desc());

  if (!hpe.isNull())
  {
    if (hpe->hasHyperlink(pDrawable))
    {
      OdDbHyperlinkCollectionPtr collection = hpe->getHyperlinkCollection(pDrawable, true);

      //OdDbHyperlink* pUrl = collection->item( collection->count() - 1 );
      OdDbHyperlink* pUrl = collection->item( 0 );

      dwfUrl.sName        = pUrl->name();
      dwfUrl.sDescription = pUrl->description();
      dwfUrl.sSubLocation = pUrl->subLocation();

      add_UrlToMap( dwfUrl );

      g_bUrlTrigger = true;     // to set on trigger because the current url list was changed
    }
  }

  g_UrlList.push_back(dwfUrl);
}


void OdDwfView::postprocess_Url()
{
  ODA_ASSERT( !g_UrlList.empty() );

  DwfUrl dwfUrl = g_UrlList.last();

  if (dwfUrl.iUrlNum > 0)
  {
    g_bUrlTrigger = true;  // url trigger was changed
  }

  g_UrlList.removeLast();

  ODA_ASSERT( !g_UrlList.empty() );
}
}
