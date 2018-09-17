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
// DwfWriter.cpp: implementation of the GsDwfFactory, GsDwfDevice, GsDwfView classes.
//
//////////////////////////////////////////////////////////////////////


#include "OdaCommon.h"

// to avoid STL warnings (if not to include STL here, it will include itself from whip tk)
#define STL_USING_VECTOR
#define STL_USING_LIST
#define STL_USING_MAP
#include "OdaSTL.h"

#include "OdBinaryData.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "DbBaseHostAppServices.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeLineSeg3d.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiRasterWrappers.h"
#include "Gi/GiDrawableImpl.h"

#include "RxRasterServices.h"
#include "MemoryStream.h"
#include "StaticRxObject.h"
#include "DynamicLinker.h"

//-----------------------------
// MKU 12/08/2005
//  This is a temporary fix to transform name to Unicode.
//  It is done by 'past&copy' method.
//  There is no access to OdNameIterator because "OdNameIterator.h" is localed in Source not Include

#include "OdString.h"
#include "OdCodePage.h"
#include "OdCharMapper.h"

#ifdef _MSC_VER
#pragma warning(push)
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#pragma warning(disable: 4290)
#pragma warning(disable: 4244)
#endif // _MSC_VER

#include "DwfDrawObject.h"
#include "2dSupport.h"
#include "DwfSupport.h"
#include "DwfExportImpl.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

namespace TD_DWF_EXPORT {

const int nFontMultipliedByFactor (1024);     // Some Font's values may range from a value of one (1/1024 
                                              //  of normal) to 65535 (64 times normal). 
const WT_Unsigned_Integer16 nWtAngleMaxValue  (65535);  // The max legal value of angle.
const double dWtAngleMeasureOfDegree (65535/360);       // Measure in 360/65636ths of a degree.

//====================================================================
//

OdDwfDevice::~OdDwfDevice()
{
  delete m_pWhipFactory;
  while(!m_softClips.empty())
    popSoftClip();
}

#ifndef WD_MAXIMUM_POINT_SET_SIZE
#define WD_MAXIMUM_POINT_SET_SIZE    (255 + 65535)
#endif

void OdDwfDevice::dc_polyline(OdUInt32 nPts, const OdGePoint2d* pPoints)
{
  if (IsNonprocessRun()) return;
  if (nPts == 0)
    return;
  push_Url();
  if (nPts == 1)
  {
    WT_Logical_Point pWtPoints[2];
    RoundPoint(pWtPoints[0], pPoints[0]);
    pWtPoints[1] = pWtPoints[0];
    std::auto_ptr<WT_Polyline> line(m_pWhipFactory->Create_Polyline(2, pWtPoints, WD_False));
    line->serialize(*getWtFile());
  }
  else
  {
    WT_Logical_Point * pWtPoints = new WT_Logical_Point[nPts];
    for (OdUInt32 i = 0; i < nPts; i++)
    {
      RoundPoint(pWtPoints[i], pPoints[i]);
    }
    int N = (nPts / WD_MAXIMUM_POINT_SET_SIZE);
    for (int j = 0; j <= N; ++j)
    {
      int K = WD_MAXIMUM_POINT_SET_SIZE;
      if (j == N)
        K = nPts % WD_MAXIMUM_POINT_SET_SIZE;
      std::auto_ptr<WT_Polyline> line(m_pWhipFactory->Create_Polyline(K, pWtPoints + (j * WD_MAXIMUM_POINT_SET_SIZE), WD_False));
      line->serialize(*getWtFile());
    }
    delete [] pWtPoints;
  }
}


//====================================================================
//
void OdDwfDevice::dc_polygon(OdUInt32 nPoints, const OdGePoint2d* pPoints)
{
  if (IsNonprocessRun()) return;
  push_Url();
  dc_fill(true);

  WT_Logical_Point * pWtPoints = new WT_Logical_Point[nPoints];

  for (OdUInt32 i = 0; i < nPoints; i++)
  {
    RoundPoint(pWtPoints[i], pPoints[i]);
  }

  std::auto_ptr<WT_Polygon> polygon(m_pWhipFactory->Create_Polygon(nPoints, pWtPoints, WD_False));
  polygon->serialize(*getWtFile());

  delete [] pWtPoints;
}


//====================================================================
//

// This is temporary iterative fix, used locally for DWF (#4886)
// Precision of iterative method is 360.0 / 65536 (15 iterations) normal for DWF, but not
// for Ge. Really this calculation must be implemented correctly in OdGeEllipArc3d::
// ::orthogonalizeAxes(). This solution must be completely rewrited as analytic method
// (for optimize speed) and placed in OdGeEllipArc3d in future.
void correctEllipseAxesForDWF(const OdGeEllipArc3d &inArc, OdGeEllipArc3d &outArc)
{
  // Find major axis using optimized iterative method
  double in_angle1 = 0.0, in_angle5 = 180.0, out_angle = 0.0;
  // in_angle5 - in_angle1 > 0.1 --> 11 iterations
  // DWF angle precision:
  // in_angle5 - in_angle1 > 360.0 / 65536 --> 15 iterations
  // (75 evals calls per ellipse --> 150 sincoses ~ 1755 floating point operations --> 585 fp_muldivs)
  const double min_angle = 360.0 / 65536;
  while (in_angle5 - in_angle1 > min_angle)
  {
    int i;
    double rad[5];
    double in_angle3 = in_angle1 + (in_angle5 - in_angle1) / 2;
    double in_angle2 = in_angle1 + (in_angle3 - in_angle1) / 2;
    double in_angle4 = in_angle3 + (in_angle5 - in_angle3) / 2;
    OdGeVector3d v[5];
    v[0] = inArc.evalPoint(OdaToRadian(in_angle1)) - inArc.center();
    v[1] = inArc.evalPoint(OdaToRadian(in_angle2)) - inArc.center();
    v[2] = inArc.evalPoint(OdaToRadian(in_angle3)) - inArc.center();
    v[3] = inArc.evalPoint(OdaToRadian(in_angle4)) - inArc.center();
    v[4] = inArc.evalPoint(OdaToRadian(in_angle5)) - inArc.center();
    for (i = 0; i < 5; i++)
    {
      rad[i] = v[i].length();
    }
    int max_rad = 0;
    for (i = 1; i < 5; i++)
    {
      if (rad[i] > rad[max_rad])
      {
        max_rad = i;
      }
    }
    switch (max_rad)
    {
      case 0:
        out_angle = in_angle1;
        in_angle1 = in_angle1 - (in_angle2 - in_angle1);
        in_angle5 = in_angle2;
      break;
      case 1:
        out_angle = in_angle2;
        // in_angle1 = in_angle1;
        in_angle5 = in_angle3;
      break;
      case 2:
        out_angle = in_angle3;
        in_angle1 = in_angle2;
        in_angle5 = in_angle4;
      break;
      case 3:
        out_angle = in_angle4;
        in_angle1 = in_angle3;
        // in_angle5 = in_angle5;
      break;
      case 4:
        out_angle = in_angle5;
        in_angle1 = in_angle4;
        in_angle5 = in_angle5 + (in_angle5 - in_angle4);
      break;
    };
  }
  OdGeVector3d new_max_axis = inArc.evalPoint(OdaToRadian(out_angle)) - inArc.center();
  OdGeVector3d new_min_axis = inArc.evalPoint(OdaToRadian(out_angle + 90)) - inArc.center();
  if (inArc.isClosed())
  {
    outArc.set(inArc.center(), new_max_axis.normal(), new_min_axis.normal(), new_max_axis.length(), new_min_axis.length());
  }
  else
  {
    // Recompute params
    outArc.set(inArc.center(), new_max_axis.normal(), new_min_axis.normal(), new_max_axis.length(), new_min_axis.length());
    // Code from OdGeEllipArc::orthogonalizeAxes
    OdGeMatrix3d x;
    x.setCoordSystem(outArc.center(), new_max_axis, new_min_axis, outArc.normal()).invert();
    OdGeVector3d rvecSt = (x * inArc.startPoint()).asVector();
    OdGeVector3d rvecEn = (x * inArc.endPoint()).asVector();
    double startParam = OdGeVector3d::kXAxis.angleTo(rvecSt, OdGeVector3d::kZAxis);
    double endParam = OdGeVector3d::kXAxis.angleTo(rvecEn, OdGeVector3d::kZAxis);
    if (startParam >= Oda2PI)
    {
      startParam -= Oda2PI;
    }
    if(startParam < 0.0)
    {
      startParam += Oda2PI;
      endParam += Oda2PI;
    }
    if(endParam < startParam)
      endParam += Oda2PI;
    ODA_ASSERT(startParam >= 0);
    ODA_ASSERT(startParam < Oda2PI);
    outArc.setAngles(startParam, endParam);
  }
}

void OdDwfDevice::dc_circle(const OdGeEllipArc3d& ellipse)
{
  if (IsNonprocessRun()) return;
  push_Url();

  OdGeEllipArc3d convEllipse(ellipse);
  bool bNormalize = convEllipse.majorAxis().isPerpendicularTo(convEllipse.minorAxis());
  if (!bNormalize)
  {
    correctEllipseAxesForDWF(ellipse, convEllipse);
  }

  OdGePoint3d ptCenter3d(convEllipse.center());

  WT_Logical_Point wtCenter;
  RoundPoint(wtCenter, OdGePoint2d(ptCenter3d.x, ptCenter3d.y));

  OdGeVector3d vMajor = convEllipse.majorRadius() * convEllipse.majorAxis();
  OdGeVector3d vMinor = convEllipse.minorRadius() * convEllipse.minorAxis();

  WT_Integer32 wt_iMajor = RoundDoubleToWTInteger32(vMajor.length());
  WT_Integer32 wt_iMinor = RoundDoubleToWTInteger32(vMinor.length());

  WT_Unsigned_Integer16 wtStart = 0;
  WT_Unsigned_Integer16 wtEnd = 0;      // Because it is not an Arc. The result is that 'R' opcode 
                                        //  instead of 'Circle' will be written into file
  // The effect of Tilt is to rotate the ellipse counter-clockwise 
  //  about its center point in 360/65,536ths of a degree.
  double dVal = OdaToDegree( OdGeVector3d::kXAxis.angleTo( convEllipse.majorAxis(), OdGeVector3d::kZAxis ) );
  while (dVal > 360.0) dVal -= 360;
  while (dVal < 0.0) dVal += 360;
  WT_Unsigned_Integer16 wtTilt = RoundDoubleToWTInteger16( dVal * dWtAngleMeasureOfDegree );

  std::auto_ptr<WT_Outline_Ellipse> wtEllipse(m_pWhipFactory->Create_Outline_Ellipse(wtCenter, wt_iMajor, wt_iMinor, wtStart, wtEnd, wtTilt));
  wtEllipse->serialize(*getWtFile());
}

void OdDwfDevice::dc_ellipse(const OdGeEllipArc3d& ellipse)
{
  if (IsNonprocessRun()) return;
  if (m_dwf->getParams().format() == DW_XPS) // temporary workaround
  {
    OdGiGeometrySimplifier::ellipArcProc(ellipse);
    return;
  }
  push_Url();

  OdGeEllipArc3d convEllipse(ellipse);
  bool bNormalize = convEllipse.majorAxis().isPerpendicularTo(convEllipse.minorAxis());
  if (!bNormalize)
  {
    correctEllipseAxesForDWF(ellipse, convEllipse);
  }

  OdGePoint3d ptCenter3d(convEllipse.center());

  WT_Logical_Point wtCenter;
  RoundPoint(wtCenter, OdGePoint2d(ptCenter3d.x, ptCenter3d.y));

  OdGeVector3d vMajor = convEllipse.majorRadius() * convEllipse.majorAxis();
  OdGeVector3d vMinor = convEllipse.minorRadius() * convEllipse.minorAxis();

  WT_Integer32 wt_iMajor = RoundDoubleToWTInteger32(convEllipse.majorRadius());
  WT_Integer32 wt_iMinor = RoundDoubleToWTInteger32(convEllipse.minorRadius());

  double dVal = OdaToDegree( OdGeVector3d::kXAxis.angleTo( convEllipse.majorAxis(), OdGeVector3d::kZAxis ));
  // Normalize angle (it may overflow uword)
  while (dVal > 360.0) dVal -= 360;
  while (dVal < 0.0) dVal += 360;
  WT_Unsigned_Integer16 wt_iValue 
              = RoundDoubleToWTUInteger16(
                            dVal * dWtAngleMeasureOfDegree );
                                        // The effect of Tilt is to rotate the ellipse counter-clockwise 
                                        //  about its center point in 360/65,536ths of a degree.
  WT_Unsigned_Integer16 wtTilt = (wt_iValue < nWtAngleMaxValue) ? wt_iValue : nWtAngleMaxValue;

  bool bArc(false);                     // in order to keep Arc as Arc even at very small angular value
  if (!OdEqual(convEllipse.endAng(),convEllipse.startAng()))
  {
    bArc = true;
  }

  double dStart = OdaToDegree( convEllipse.startAng() );
  while (dStart > 360.0) dStart -= 360;
  while (dStart < 0.0) dStart += 360;
  double dEnd = OdaToDegree( convEllipse.endAng() );
  while (dEnd > 360.0) dEnd -= 360;
  while (dEnd < 0.0) dEnd += 360;

  wt_iValue = RoundDoubleToWTUInteger16( dStart * dWtAngleMeasureOfDegree );
  WT_Unsigned_Integer16 wtStart = (wt_iValue < nWtAngleMaxValue) ? wt_iValue : nWtAngleMaxValue;

  wt_iValue = RoundDoubleToWTUInteger16( dEnd * dWtAngleMeasureOfDegree );
  WT_Unsigned_Integer16 wtEnd = (wt_iValue < nWtAngleMaxValue) ? wt_iValue : nWtAngleMaxValue;

  if (bArc && OdZero(wtEnd - wtStart) && !convEllipse.isClosed())
  {
    wtEnd += 1;   // CircleArc should be on its own
  }

  if (bArc && wt_iMajor != 0 && (Od_abs(wtEnd - wtStart) / (double)wt_iMajor) < 0.001) //  too small arcs should be splined
  {
    OdGiGeometrySimplifier::ellipArcProc(ellipse);
  }
  else
  {
    std::auto_ptr<WT_Outline_Ellipse> wtEllipse(m_pWhipFactory->Create_Outline_Ellipse(wtCenter, wt_iMajor, wt_iMinor, wtStart, wtEnd, wtTilt));
    wtEllipse->serialize(*getWtFile());
  }
}


//====================================================================
//
void OdDwfDevice::dc_lineWeight(WT_Integer32 lineWeight/*OdDb::LineWeight lineWeight*/)
{
  if (IsNonprocessRun()) return;
  getWtFile()->desired_rendition().line_weight() = lineWeight;
}


//====================================================================
//
void OdDwfDevice::dc_layer(int layer_num, const OdString& layer_name)
{
  if (IsNonprocessRun()) return;
  WT_Layer Layer(*getWtFile(), layer_num, OdW2US(layer_name.c_str()));
  getWtFile()->desired_rendition().layer() = Layer;
}

//====================================================================
//
void OdDwfDevice::dc_color(ODCOLORREF rgb)
{
  if (IsNonprocessRun()) return;
  int colorIndex;
  bool bIndex = getExportImpl()->getWtColorIndex(rgb, colorIndex);
  getWtFile()->desired_rendition().color() 
    = (bIndex) ? WT_Color(colorIndex, getWtFile()->desired_rendition().color_map())
               : WT_Color(ODGETRED(rgb), ODGETGREEN(rgb), ODGETBLUE(rgb));
}

//====================================================================
//
void OdDwfDevice::dc_lineStyle(const LineStyle& lineStyle)
{  
  if (IsNonprocessRun()) return;
  WT_Line_Style wtLineStyle;

  wtLineStyle.line_join() = lineStyle.join;
  wtLineStyle.line_start_cap() = lineStyle.startCap;
  wtLineStyle.line_end_cap() = lineStyle.endCap;

  getWtFile()->desired_rendition().line_style() = wtLineStyle;
}

//====================================================================
//
void OdDwfDevice::dc_gouraud_polytriangle( const OdGePoint3d* pVertexList,
                                           OdInt32 faceListSize,
                                           const OdInt32* pFaceList,
                                           const OdCmEntityColor* pColors)
{
  if (IsNonprocessRun()) return;
  push_Url();

  // it is shell with gradient fill
  for(int i = 0; i < faceListSize;)
  {
    ODA_ASSERT(pFaceList[i] == 3);
    ++i;
    
    WT_Logical_Point vP[3];
    WT_RGBA32        rgba[3];
    for(OdInt32 k = 0; k < 3; k ++)
    {
      OdGePoint3d pt(pVertexList[pFaceList[i]]);
      OdCmEntityColor col(pColors[pFaceList[i]]);
      i ++;

      // converting to...
      RoundPoint(vP[k], OdGePoint2d(pt.x, pt.y));
      rgba[k] = WT_RGBA32(col.red(), col.green(), col.blue(), 255);
    }
    std::auto_ptr<WT_Gouraud_Polytriangle> gpt(m_pWhipFactory->Create_Gouraud_Polytriangle(3, vP, rgba, WD_True));
    dc_gouraud_polytriangle(*gpt);
  }
}

void OdDwfDevice::dc_gouraud_polytriangle(const WT_Gouraud_Polytriangle& wtColoredPoly)
{
  if (IsNonprocessRun()) return;
  push_Url();

  wtColoredPoly.serialize(*getWtFile());
}

//====================================================================
//
// rasterImageDc()
//

class DwfDrawableImpl : public OdStaticRxObject<OdGiDrawableImpl<> >
{
public:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits*) const
  {
    return kGeometry;
  }

  virtual bool subWorldDraw(OdGiWorldDraw*) const
  {
    return false;
  }
};

class ImageDrawable : public DwfDrawableImpl
{
  const OdGePoint3d&      m_origin;
  const OdGeVector3d&     m_u;
  const OdGeVector3d&     m_v;
  const OdGiRasterImage*  m_pImage;       // image object
  const OdGePoint2d*      m_uvBoundary;
  OdUInt32                m_numBoundPts;
  bool                    m_transparency;
  double                  m_brightness;
  double                  m_contrast;
  double                  m_fade;
  OdCmEntityColor         m_traitsCol;
  OdUInt32                m_drawFlags;
  OdCmEntityColor         m_secCol;

public:
  ImageDrawable( const OdGePoint3d& origin,
                 const OdGeVector3d& u,
                 const OdGeVector3d& v,
                 const OdGiRasterImage* pImage, // image object
                 const OdGePoint2d* uvBoundary, // may not be null
                 OdUInt32 numBoundPts,
                 bool transparency,
                 double brightness,
                 double contrast,
                 double fade,
                 const OdCmEntityColor& traitsCol,
                 OdUInt32 drawFlags,
                 const OdCmEntityColor& secCol
               )
      : m_origin( origin )
      , m_u( u )
      , m_v( v )
      , m_pImage( pImage )
      , m_uvBoundary( uvBoundary )
      , m_numBoundPts( numBoundPts )
      , m_transparency( transparency )
      , m_brightness( brightness )
      , m_contrast( contrast )
      , m_fade( fade )
      , m_traitsCol( traitsCol )
      , m_drawFlags( drawFlags )
      , m_secCol( secCol )
    {
    }

  virtual void subViewportDraw(OdGiViewportDraw* vd) const
  {
    vd->subEntityTraits().setDrawFlags(m_drawFlags);
    vd->subEntityTraits().setTrueColor(m_traitsCol);
    vd->subEntityTraits().setSecondaryTrueColor(m_secCol);
    vd->geometry().rasterImageDc( m_origin,
                                  m_u,
                                  m_v,
                                  m_pImage,
                                  m_uvBoundary,
                                  m_numBoundPts,
                                  m_transparency,
                                  m_brightness,
                                  m_contrast,
                                  m_fade
                                );
  }
};

class PolygonDrawable : public DwfDrawableImpl
{
  const OdGePoint3d*  m_pPts;
  OdUInt32            m_ptsNum;

public:
  PolygonDrawable(const OdGePoint3d* pPts, OdUInt32 ptsNum)
    : m_pPts(pPts)
    , m_ptsNum(ptsNum)
  {}

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    pWd->subEntityTraits().setTrueColor(OdCmEntityColor(255, 255, 255));
    pWd->subEntityTraits().setFillType(kOdGiFillAlways);
    pWd->geometry().polygon(m_ptsNum, m_pPts);
    return true;
  }
};

class ShellDrawable : public DwfDrawableImpl
{
protected:
  OdInt32 m_numVertices;
  const OdGePoint3d* m_vertexList;
  OdInt32 m_faceListSize;
  const OdInt32* m_faceList;
public:
  ShellDrawable(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList)
    : m_numVertices(numVertices)
    , m_vertexList(vertexList)
    , m_faceListSize(faceListSize)
    , m_faceList(faceList)
  {
  }

  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    pWd->subEntityTraits().setTrueColor(OdCmEntityColor(255, 255, 255));
    pWd->subEntityTraits().setFillType(kOdGiFillAlways);
    pWd->geometry().shell(m_numVertices, m_vertexList, m_faceListSize, m_faceList);
    return true;
  }
};

class ShellDrawableEx : public ShellDrawable
{
  const OdGiEdgeData* m_pEdgeData;
  const OdGiFaceData* m_pFaceData;
  const OdGiVertexData* m_pVertexData;
public:
  ShellDrawableEx(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData) :
    ShellDrawable(numVertices, vertexList, faceListSize, faceList), m_pEdgeData(pEdgeData), m_pFaceData(pFaceData), m_pVertexData(pVertexData){}
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    pWd->subEntityTraits().setFillType(kOdGiFillAlways);
    pWd->geometry().shell(m_numVertices, m_vertexList, m_faceListSize, m_faceList, m_pEdgeData, m_pFaceData, m_pVertexData);
    return true;
  }
};

OdGiRasterImagePtr createMaskDIB(const OdGePoint3dArray &devBound, 
                                 const OdGePoint3d &ptVpCenter, 
                                 const OdGeVector3d &viewDir, 
                                 const OdGeVector3d &upDir, 
                                 double dVpWidth, 
                                 double dVpHeight, 
                                 const OdGsDCRect &gsRect,
                                 const std::stack<gpc_polygon>& softClips,
                                 OdGiContext* pUserGiContext)
{
  // This is a case when the image is rotated and the transparency is necessary
  
  // PolygonDrawable drawPolygon(worldBound.asArrayPtr(), worldBound.size());
  
  // First, clipping source boundary with software clip
  gpc_polygon result;
  bool bFreeResult(true);
  
  if(!devBound.empty())
  {
    result.num_contours = 1;
    result.hole = ::gpc_alloc<int>(1);
    result.hole[0] = 0;
    result.contour = ::gpc_alloc<gpc_vertex_list>(1);
    result.contour[0].num_vertices = devBound.size();
    result.contour[0].vertex = ::gpc_alloc<gpc_vertex>((int)devBound.size());
    OdUInt32 i;
    for(i = 0; i < devBound.size(); i ++)
    {
      OdGePoint3d pt(devBound[i]);
      result.contour[0].vertex[i].x = pt.x;
      result.contour[0].vertex[i].y = pt.y;
    }
    if(!softClips.empty())
    {
      gpc_polygon resPol;
      gpc_polygon_clip(GPC_INT, &result, (gpc_polygon*)&softClips.top(), &resPol);
      gpc_free_polygon(&result);
      result = resPol;
    }

    // We got it as result.
  }
  else if(!softClips.empty())
  {
    result = softClips.top();
    bFreeResult = false;
  }
  else
    throw OdError(eInvalidInput);

  // Second, turning result to shell primitive data
  OdGePoint3dArray vertexList(0, -100);
  OdInt32Array faceList(0, -100); // grow by 100% of current size()

  {
    int numVertsPushed(0), i;
    for(i = 0; i < result.num_contours; i ++) // NB: GPC may return data in incorrect order
                                              // (shell primitive requires all holes to follow its contour)
                                              // but WinGDI device don't depend on order (a little weak point)
    {
      int numVts = result.contour[i].num_vertices;
      faceList.push_back(numVts);
      if(result.hole[i])
        faceList.last() *= -1;
      int j;
      for(j = 0; j < numVts; j ++)
      {
        gpc_vertex vert = result.contour[i].vertex[j];
        OdGePoint3d pt(vert.x, vert.y, 0.0);
        // pt.transformBy(d2w);
        vertexList.push_back(pt);
        faceList.push_back(numVertsPushed);
        numVertsPushed ++;
      }
    }
  }

  if(bFreeResult)
    gpc_free_polygon(&result);

  ShellDrawable drawShell(vertexList.size(), vertexList.asArrayPtr(), faceList.size(), faceList.asArrayPtr());
  
  // Device for mask
  //
  OdDbBaseDatabasePEPtr pDb(pUserGiContext->database());
  OdGsDevicePtr pMaskDevice = pDb->appServices(pUserGiContext->database())->gsBitmapDevice(NULL, pUserGiContext->database(), OdDbBaseHostAppServices::kFor2dExportRender);

  if (!pMaskDevice.get())
  {
    throw OdError(OD_T("Can't create Bitmap Device for exporting RasterImage to DWF."));
  }
  
  pMaskDevice->setUserGiContext(pUserGiContext);
  
  //MKU 05/24/05  #2848 - to create Veiw anew
  OdGsViewPtr pViewMask = pMaskDevice->createView();
  
  pViewMask->setView( ptVpCenter + viewDir, ptVpCenter, 
    upDir, dVpWidth, dVpHeight);        // copy current view settings
  
  pViewMask->setViewport(gsRect);
  pViewMask->setViewportBorderVisibility(false);
  
  pMaskDevice->addView(pViewMask);
  pMaskDevice->setBackgroundColor(ODRGB(0,0,0));
  pMaskDevice->onSize(gsRect);
  
  pMaskDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(1)));
  
  ODCOLORREF imgPalette[2] = {0, ODRGB(255, 255, 255)};
  pMaskDevice->setLogicalPalette(imgPalette, 2);
  pViewMask->add(&drawShell, 0);
  pMaskDevice->update();
  
  OdGiRasterImagePtr pMask;
  pMask = pMaskDevice->properties()->getAt(OD_T("RasterImage"));
  
  pViewMask->eraseAll();
  
  return pMask;
}

class AlphaComponentDetector : public OdGiRasterImageWrapper
{
  OdUInt8Array m_prevData;
  mutable OdUInt8Array m_mergeData;
  OdGiRasterImagePtr m_pOrig2;
public:
  AlphaComponentDetector() { }
  virtual ~AlphaComponentDetector() { }

  void setOriginal(const OdGiRasterImage* pOrig)
  {
    OdGiRasterImageWrapper::setOriginal(pOrig);
    m_pOrig2 = pOrig;
    ODA_ASSERT(pOrig->colorDepth() == 24);
    m_prevData.resize(pOrig->scanLineSize() * pOrig->pixelHeight());
    pOrig->scanLines(m_prevData.asArrayPtr(), 0, pOrig->pixelHeight());
  }
  PixelFormatInfo pixelFormat() const
  {
    PixelFormatInfo res;
    res.setBGRA();
    return res;
  }
  OdUInt32 colorDepth() const
  {
    return 32;
  }
  OdUInt32 scanLineSize() const
  {
    return 4 * m_pOrig2->pixelWidth();
  }
  void scanLines(OdUInt8* scnLines, OdUInt32 firstScanline, OdUInt32 numLines) const
  {
    memcpy(scnLines, scanLines() + firstScanline * scanLineSize(), numLines * scanLineSize());
  }
  const OdUInt8* scanLines() const
  {
    if (m_mergeData.empty())
    {
      m_mergeData.resize(m_pOrig2->pixelHeight() * scanLineSize());
      OdUInt32 w = m_pOrig2->pixelWidth();
      OdUInt32 h = m_pOrig2->pixelHeight();
      OdUInt8 *pMerge = m_mergeData.asArrayPtr();
      OdUInt8Array m_curData;
      m_curData.resize(m_pOrig2->scanLineSize());
      const OdUInt8 *pPrev, *pCur;
      for (OdUInt32 y = 0; y < h; y++)
      {
        m_pOrig2->scanLines(m_curData.asArrayPtr(), y);
        pCur = m_curData.asArrayPtr();
        pPrev = m_prevData.asArrayPtr() + m_pOrig2->scanLineSize() * y;
        for (OdUInt32 x = 0; x < w; x++, pCur += 3, pPrev += 3, pMerge += 4)
        {
          pMerge[0] = pPrev[0];
          pMerge[1] = pPrev[1];
          pMerge[2] = pPrev[2];
          pMerge[3] = (pPrev[0] != pCur[0] || pPrev[1] != pCur[1] || pPrev[2] != pCur[2]) ? 0 : 255;
        }
      }
    }
    return m_mergeData.asArrayPtr();
  }
  OdUInt32 scanLinesAlignment() const
  {
    return 4;
  }
};

void OdDwfDevice::rasterImageProc(const OdGePoint3d& origin,
                                  const OdGeVector3d& u,
                                  const OdGeVector3d& v, // now it is in screen coordinates
                                  const OdGiRasterImage* pImage, // image object
                                  const OdGePoint2d* uvBoundary, // may not be null
                                  OdUInt32 numBoundPts,
                                  bool transparency,
                                  double brightness,
                                  double contrast,
                                  double fade)
{
  if (IsNonprocessRun()) return;
  OdUInt32 i;

  if(!pImage)
  {
    return;
  }

  OdCmEntityColor entCol, secCol;
  entCol = m_effTraits.trueColor();
  secCol = m_effTraits.secondaryTrueColor();

  // entCol must be true color because image device may have different 
  // palette with dwf device
  if(entCol.colorMethod() == OdCmEntityColor::kByACI
     || entCol.colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    ODCOLORREF colRef = getColor(entCol.colorIndex());
    entCol = OdCmEntityColor(ODGETRED(colRef), ODGETGREEN(colRef), ODGETBLUE(colRef));
  }
  if(secCol.colorMethod() == OdCmEntityColor::kByACI
     || secCol.colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    ODCOLORREF colRef = getColor(secCol.colorIndex());
    secCol = OdCmEntityColor(ODGETRED(colRef), ODGETGREEN(colRef), ODGETBLUE(colRef));
  }

  ImageDrawable drawImage(origin,
                          u,
                          v,
                          pImage, // image object
                          uvBoundary, // may not be null
                          numBoundPts,
                          (transparency && pImage->colorDepth() == 32), // GU, cr2394, 17.12.2004
                          brightness,
                          contrast,
                          fade,
                          entCol,
                          m_effTraits.drawFlags(),
                          secCol); // traits color need for bitonal images

  // If the image is rotated the transparency is necessary
  //  Note: we can receive another number of bits per pixel in this case on the destination image 

  //OdDbDatabase* pDb = ((OdGiContextForDbDatabase*)&giCtx())->getDatabase();
  OdDbBaseDatabasePEPtr pDb(getExportImpl()->getParams().database());
  OdGsDevicePtr pDevice = pDb->appServices(getExportImpl()->getParams().database())->gsBitmapDevice(NULL, getExportImpl()->getParams().database(), OdDbBaseHostAppServices::kFor2dExportRender);

  if (pDevice.isNull())
    throw OdError(OD_T("Can't create Bitmap Device for exporting RasterImage to DWF."));

  pDevice->setUserGiContext(userGiContext());
  
  OdUInt32 pixelWidth  = pImage->pixelWidth();
  OdUInt32 pixelHeight = pImage->pixelHeight();
  int colorDepth = pImage->colorDepth();

  if (pixelWidth == 0 || pixelHeight == 0 || colorDepth == 0)
    return;
  
  // clipping boundary
  const OdGePoint2d* pClpBnd = uvBoundary;
  OdGePoint2dArray uvBndBuf;
  OdUInt32 effNumBndPts(numBoundPts);
  if(!pClpBnd)
  {
    uvBndBuf.resize(2);
    uvBndBuf[0] = OdGePoint2d::kOrigin - OdGeVector2d(0.5, 0.5);
    uvBndBuf[1].set(pixelWidth - 0.5, pixelHeight - 0.5);
    pClpBnd = uvBndBuf.asArrayPtr();
    effNumBndPts = 2;
  }
  
  OdGePoint2dArray uvBndBuf1;
  if(effNumBndPts == 2)
  {
    uvBndBuf1.resize(4);
    uvBndBuf1[0] = pClpBnd[0];
    uvBndBuf1[1] = OdGePoint2d(pClpBnd[0].x, pClpBnd[1].y);
    uvBndBuf1[2] = pClpBnd[1];
    uvBndBuf1[3] = OdGePoint2d(pClpBnd[1].x, pClpBnd[0].y);
    pClpBnd = uvBndBuf1.asArrayPtr();
    effNumBndPts = 4;
  }

  // is transparency need
  bool bIsTransp(false);
  if(numBoundPts > 3)
    bIsTransp = true; // non-rectangular clip
  else if((numBoundPts == 2) && (!uvBoundary[0].isEqualTo(OdGePoint2d(-0.5, -0.5)) || 
    !uvBoundary[1].isEqualTo(OdGePoint2d(pixelWidth - 0.5, pixelHeight - 0.5))))
    bIsTransp = true; // rectangular clip, but clips out part of image

  if(m_bWasNrcViewportClip)
    bIsTransp = true; // nrc viewport clip requires mask, WT_Viewport can't clip by NRC contour

  if(!(u.isParallelTo(OdGeVector3d::kXAxis) && v.isParallelTo(OdGeVector3d::kYAxis)) && 
     !(u.isParallelTo(OdGeVector3d::kYAxis) && v.isParallelTo(OdGeVector3d::kXAxis)))
    bIsTransp = true; // not aligned with CS

  if(((colorDepth==1) && transparency)/* || ((colorDepth==32) && transparency)*/)
    bIsTransp = false; // in this case need not any masks
                       // (one of the images' colors can be done transparent)

  OdGeMatrix3d uv2World;
  uv2World.setCoordSystem(origin + 0.5*u - 0.5*v + pixelHeight*v, 
    u, -v, OdGeVector3d());

  OdGeExtents2d imgExts; // in WCS
  OdGePoint3dArray worldBound;
  worldBound.resize(effNumBndPts);
  for(i = 0; i < effNumBndPts; i ++)
  {
    worldBound[i] = OdGePoint3d(pClpBnd[i].x, pClpBnd[i].y, 0.0);
    worldBound[i].transformBy(uv2World);
    imgExts.addPoint((OdGePoint2d&)worldBound[i]);
  }
  
  // Create View containing only one (Image) object.
  //  Therefore to set a Viewport for the Image.
  //
  double dVpWidth  = imgExts.maxPoint().x - imgExts.minPoint().x;
  double dVpHeight = imgExts.maxPoint().y - imgExts.minPoint().y;
  OdGePoint3d ptVpCenter( (imgExts.maxPoint().x + imgExts.minPoint().x) / 2,
                          (imgExts.maxPoint().y + imgExts.minPoint().y) / 2,
                          0.0
                        );

  double dU = u.length();       // size of one pixel in the World Coo
  double dV = v.length();
  double dW = odmin(dU, dV);
  OdUInt32 pixelWidthNew  = RoundDblToUInt32(dVpWidth / dW);
  OdUInt32 pixelHeightNew = RoundDblToUInt32(dVpHeight / dW);

  // the numbers are average, the idea is to avoid too large image size
  while (pixelWidthNew > 800 || pixelHeightNew > 800)
  {
    pixelWidthNew /= 2;
    pixelHeightNew /= 2;
  }

  // Set the Bitmap Device 
  //  and in our case to create View containing only Image. 
  //  New View should have the same settings as the current View.
  //  Note: Our Device will have only one View because only one Viewport will be set
  //
  OdGsViewPtr pView = pDevice->createView();
  pDevice->addView(pView);

  OdGsDCRect   gsRect(0, pixelWidthNew, pixelHeightNew, 0);   // in pixels

  OdGeVector3d viewDir(OdGeVector3d::kZAxis);
  OdGeVector3d upDir(OdGeVector3d::kYAxis);
  pView->setView(ptVpCenter + viewDir, ptVpCenter, 
    upDir, dVpWidth, dVpHeight);  // copy current view settings

  pView->setViewport(gsRect);
  pView->setViewportBorderVisibility(false);

  pView->add(&drawImage, 0);

  pDevice->setBackgroundColor(ODRGB(0,0,0));
  
  OdUInt32 numCols = pImage->numColors();
  OdUInt8Array imgPalette;
  imgPalette.resize(pImage->paletteDataSize());

  if(numCols != 2)
  {
    pImage->paletteData(imgPalette.asArrayPtr());
  }
  else
  {
    // for bitonal images, palette contains current bg color 
    // and color that is currently set in traits
    ODCOLORREF* pCol = (ODCOLORREF*)imgPalette.asArrayPtr();
    pCol[0] = getBackgroundColor();
    if (GETBIT(m_effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill))
    {
      OdCmEntityColor secCol = m_effTraits.secondaryTrueColor();
      if(secCol.isByColor())
        pCol[0] = ODTOCOLORREF(secCol);
      else
        pCol[0] = getColor(secCol.colorIndex());
    }
    OdCmEntityColor col = m_effTraits.trueColor();
    if(col.isByColor())
      pCol[1] = ODTOCOLORREF(col);
    else
      pCol[1] = getColor(col.colorIndex());
  }
  if(numCols)
    pDevice->setLogicalPalette((const ODCOLORREF*)imgPalette.asArrayPtr(), numCols);
  else // NB "getPaletteColorNumbers" is not correct name
    pDevice->setLogicalPalette(m_logPalette.asArrayPtr(), m_logPalette.size());

  pDevice->onSize(gsRect);

  if (colorDepth == 32)
    colorDepth = 24;
  pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(colorDepth)));
  OdGiDefaultContextPtr dbctx = pDevice->userGiContext();
  dbctx->setPlotGeneration(false);
  pDevice->update();
  dbctx->setPlotGeneration(true);
  
  OdGiRasterImagePtr pRaster = pDevice->properties()->getAt(OD_T("RasterImage"));

  if (transparency && pImage->colorDepth() == 32)
  {
    // We need create additional image with negative background to detect image alpha pixels
    OdSmartPtr<AlphaComponentDetector> pDet = OdRxObjectImpl<AlphaComponentDetector>::createObject();
    pDet->setOriginal(pRaster);
    pDevice->setBackgroundColor(ODRGB(255,255,255));
    dbctx->setPlotGeneration(false);
    pDevice->update();
    dbctx->setPlotGeneration(true);
    pRaster = pDet;
  }
  
  // Creating the mask
  OdGiRasterImagePtr pMask;

  if(bIsTransp)
  {
    pMask = createMaskDIB(worldBound, ptVpCenter, viewDir, upDir, dVpWidth, dVpHeight, gsRect, getSoftClips(), userGiContext());
  }

  dc_raster_image(pRaster, pMask, imgExts, transparency);

  pView->eraseAll(); // to avoid pure virtual function call
}


// imgExts are in screen coordinates.
// clipBoundary (dc_pushClip) must be applied to this primitive
void OdDwfDevice::dc_raster_image(OdGiRasterImagePtr pRaster, const OdGeExtents2d& imgExts)
{
  if (IsNonprocessRun()) return;
  if(!numViews())
    throw OdError(eNotApplicable);

  double dVpWidth  = imgExts.maxPoint().x - imgExts.minPoint().x;
  double dVpHeight = imgExts.maxPoint().y - imgExts.minPoint().y;
  OdGePoint3d ptVpCenter( (imgExts.maxPoint().x + imgExts.minPoint().x) / 2,
                          (imgExts.maxPoint().y + imgExts.minPoint().y) / 2,
                          0.0
                        );
  OdGsDCRect   gsRect(0, pRaster->pixelWidth(), pRaster->pixelHeight(), 0);   // in pixels
  OdGeVector3d viewDir(OdGeVector3d::kZAxis);
  OdGeVector3d upDir(OdGeVector3d::kYAxis);
  const std::stack<gpc_polygon>& softClips = getSoftClips();
  OdGiRasterImagePtr pMask;
  if(!softClips.empty())
    pMask = createMaskDIB(OdGePoint3dArray(), ptVpCenter, viewDir, upDir, dVpWidth, 
    dVpHeight, gsRect, getSoftClips(), viewAt(0)->userGiContext());
  
  dc_raster_image(pRaster, pMask, imgExts, true);
}

static WT_Image::WT_Image_Format getWTImageFormat(int nColorBits, bool bBitonalTransp, OdGiRasterImage* pMask, int& outClrDepth)
{
  if (pMask)
  {
    if (pMask->colorDepth() != 1)
      throw OdError(eInvalidInput);
    outClrDepth = 32;
    return WT_Image::RGBA;
  }
  switch (nColorBits)
  {
  case 1:
    outClrDepth = 1;
    return WT_Image::Bitonal_Mapped;
  case 2:
  case 4:
  case 8:
    outClrDepth = 8;
    return WT_Image::Mapped;
  case 16:
  case 24:
    outClrDepth = 24;
    return WT_Image::RGB;
  case 32:
    if (bBitonalTransp)
    {
      outClrDepth = 32;
      return WT_Image::RGBA;
    }
    else
    {
      outClrDepth = 24;
      return WT_Image::RGB;
    }
  default:
    ODA_FAIL();
    throw OdError(eInvalidInput);
  }
}

static WT_Color_Map* getWTColorMap(int outClrDepth, OdGiRasterImage* pRaster, WT_Color_Map& wtColorMap, bool bBitonalTransp, WT_File& wtfile)
{
  if (outClrDepth == 32)
    return WD_Null;
  WT_RGBA32 pMap[256];
  OdUInt32 numColors = pRaster->numColors();
  for (OdUInt32 i = 0; i < numColors ; i++)
  {
    ODCOLORREF col = pRaster->color(i);
    pMap[i] = WT_RGBA32(ODGETRED(col), ODGETGREEN(col), 
      ODGETBLUE(col), (i==0 && bBitonalTransp) ? 0 : 255);       // Add new color
  }
   wtColorMap.set(numColors == 2 ? 2 : 256, pMap, wtfile);
   return &wtColorMap;
}

static OdUInt32 getBGRA(OdGiRasterImage* pRaster, OdGiRasterImage* pMask, int outClrDepth, WT_Image::WT_Image_Format imageOutFormat, bool invert, OdUInt8Array& outScLns)
{
  int nColorBits = pRaster->colorDepth();
  OdUInt32 imageHeight = pRaster->pixelHeight();
  OdUInt32 imageWidth  = pRaster->pixelWidth();
  OdUInt32 scanlineWidth = pRaster->scanLineSize();
  OdBinaryData scanLineBuf;
  scanLineBuf.resize(scanlineWidth);
  OdUInt8* scanLine = scanLineBuf.asArrayPtr();
  OdBinaryData alphaScanLineBuf;
  OdUInt32 alphaScLnWdth(0);
  OdUInt8* alphaScanLine = NULL;
  if (pMask)
  {
    alphaScLnWdth = pMask->scanLineSize();
    alphaScanLineBuf.resize(alphaScLnWdth);
    alphaScanLine = alphaScanLineBuf.asArrayPtr();
  }
  OdBinaryData palette;
  palette.resize(pRaster->paletteDataSize());
  OdUInt8* pPal = palette.asArrayPtr();
  pRaster->paletteData(pPal);

  OdUInt32 outScLnSz = outClrDepth * imageWidth / 8;
  if(outClrDepth * imageWidth % 8)
    ++outScLnSz;
  outScLns.resize(outScLnSz * imageHeight);
  memset(outScLns.asArrayPtr(), 0, outScLnSz * imageHeight);
  for (OdUInt32 indexHeight = 0; indexHeight < imageHeight; ++indexHeight)
  {
    int slIndex = invert ? (imageHeight - indexHeight - 1) : indexHeight; // to W2D image is saved upside-down
    pRaster->scanLines(scanLine, slIndex);
    if (pMask)
      pMask->scanLines(alphaScanLine, slIndex);
    OdUInt8* pDestCurScLn = outScLns.asArrayPtr() + outScLnSz*indexHeight;

    for (OdUInt32 i = 0; i < imageWidth; i ++)
    {
      OdUInt32 index(0);
      OdUInt8 red(0), green(0), blue(0), alpha(255);

      const OdUInt8* ptr = scanLine + i * nColorBits / 8;
      if(nColorBits < 16)
      {
        index = 
          (ptr[0] >> 
          (
          ((8 / nColorBits) - 1 - (i % (8 / nColorBits)))
          * nColorBits
          )
          ) 
          & 
          ((1 << nColorBits) - 1);
      }
      else if(nColorBits == 16)
      {
        OdUInt32 val = *((OdUInt32*)ptr);
        blue  = (OdUInt8)(( val        & 0x1f)<<3);
        green = (OdUInt8)(((val >> 5)  & 0x1f)<<3);
        red   = (OdUInt8)(((val >> 10) & 0x1f)<<3);
      }
      else if(nColorBits == 24)
      {
        red   = ptr[2];
        green = ptr[1];
        blue  = ptr[0];
      }
      else if(nColorBits == 32)
      {
        red   = ptr[2];
        green = ptr[1];
        blue  = ptr[0];
        alpha = (ptr[3] == 0) ? 0 : 255;
      }

      if ((imageOutFormat == WT_Image::RGB || imageOutFormat == WT_Image::RGBA) && (nColorBits < 16))
      {
        red   = pPal[index*4+2];
        green = pPal[index*4+1];
        blue  = pPal[index*4];
      }

      if (imageOutFormat == WT_Image::Mapped || imageOutFormat == WT_Image::Bitonal_Mapped)
      {
        OdUInt8* dstPtr = pDestCurScLn + i*outClrDepth/8;
        OdUInt8 bitPerCol = (OdUInt8)(8/outClrDepth);
        dstPtr[0] &= (0xFF - (((1<<outClrDepth)-1) << ((bitPerCol-1-i%bitPerCol)*outClrDepth)));
        dstPtr[0] |= index << ((bitPerCol-1-i%bitPerCol)*outClrDepth);
      }
      else
      {
        OdUInt8* dstPtr = pDestCurScLn + i*outClrDepth/8;
        if (invert) // RGB(A) to be saved to W2D
        {
          dstPtr[0] = red;
          dstPtr[1] = green;
          dstPtr[2] = blue;
        }
        else // BGRA (to be converted to PNG)
        {
          dstPtr[0] = blue;
          dstPtr[1] = green;
          dstPtr[2] = red;
        }
        if (imageOutFormat == WT_Image::RGBA)
        {
          if (pMask)
          {
            const OdUInt8* ptr = alphaScanLine + i / 8;
            alpha &= (OdUInt8)(((ptr[0] >> (7 - i%8)) & 1)*255);
          }
          dstPtr[3] = alpha;
        }
      }
    }
  }
  return outScLnSz;
}
////////////////////
// bBitonalTransp - true if transparency is enabled for bitonal images
// in this case rendering may be optimized
////////////////////
void OdDwfDevice::dc_raster_image( OdGiRasterImagePtr pRaster, 
                                   OdGiRasterImagePtr pMask,
                                   const OdGeExtents2d& imgExts,
                                   bool bBitonalTransp )
{
  if (IsNonprocessRun()) return;
  push_Url();
  OdUInt32 imageHeight = pRaster->pixelHeight();
  OdUInt32 imageWidth  = pRaster->pixelWidth();
  int outClrDepth = 0;
  WT_Image::WT_Image_Format imageOutFormat = getWTImageFormat(pRaster->colorDepth(), bBitonalTransp, pMask, outClrDepth);
  WT_Logical_Point wtMin;
  WT_Logical_Point wtMax;
  RoundPoint(wtMin, imgExts.minPoint());
  RoundPoint(wtMax, imgExts.maxPoint());

  // An attempt to save JPEG images in DWF file rather than RGB
  if (getExportImpl()->getParams().RGBToJpeg() && imageOutFormat == WT_Image::RGB)
  {
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (!pRasSvcs.isNull())
    {
      OdMemoryStreamPtr pStreamBuf = OdMemoryStream::createNew();
      if (pRasSvcs->convertRasterImage( pRaster, OdRxRasterServices::kJPEG, pStreamBuf))
      {
        pStreamBuf->rewind();
        OdUInt32 nJpegSize = (OdUInt32)pStreamBuf->length();
        OdUInt8Array dataJPEG;
        dataJPEG.resize( nJpegSize );
        pStreamBuf->getBytes( dataJPEG.asArrayPtr(), nJpegSize );
        std::auto_ptr<WT_Image> wtImage(m_pWhipFactory->Create_Image((WT_Unsigned_Integer16)imageHeight,(WT_Unsigned_Integer16)imageWidth,        
          WT_Image::JPEG, 0, WD_Null, nJpegSize, dataJPEG.asArrayPtr(), wtMin, wtMax, WD_False));
        wtImage->serialize(*getWtFile());
      }
    }
  }
  // image with transparency is converted to PNG
  else if (imageOutFormat == WT_Image::RGBA && getExportImpl()->getParams().format() == DW_XPS)
  {
    OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    if (!pRasSvcs.isNull())
    {
      OdUInt8Array outScLns;
      OdUInt32 outScLnSz = getBGRA(pRaster, pMask, outClrDepth, imageOutFormat, false, outScLns);
      OdGiImageBGRA32 bgraData(imageWidth, imageHeight, (OdGiPixelBGRA32*)outScLns.asArrayPtr());
      OdGiRasterImagePtr bgraImage = OdGiRasterImageBGRA32::createObject(&bgraData);
      OdMemoryStreamPtr pStreamBuf = OdMemoryStream::createNew();
      if (pRasSvcs->convertRasterImage( bgraImage, OdRxRasterServices::kPNG, pStreamBuf))
      {
        pStreamBuf->rewind();
        OdUInt32 nSize = (OdUInt32)pStreamBuf->length();
        OdUInt8Array data;
        data.resize(nSize);
        pStreamBuf->getBytes( data.asArrayPtr(), nSize );
        std::auto_ptr<WT_PNG_Group4_Image> wtImage(m_pWhipFactory->Create_PNG_Group4_Image((WT_Unsigned_Integer16)imageHeight, (WT_Unsigned_Integer16)imageWidth,     
          WT_PNG_Group4_Image::PNG, 0, WD_Null, nSize, data.asArrayPtr(), wtMin, wtMax, WD_False));
        wtImage->serialize(*getWtFile());
      }
    }
  }
  else
  {
    WT_Color_Map wtColorMap;
    WT_Color_Map* pWtColorMap = getWTColorMap(outClrDepth, pRaster, wtColorMap, bBitonalTransp, *getWtFile());
    OdUInt8Array outScLns;
    OdUInt32 outScLnSz = getBGRA(pRaster, pMask, outClrDepth, imageOutFormat, true, outScLns);
    std::auto_ptr<WT_Image> wtImage(m_pWhipFactory->Create_Image((WT_Unsigned_Integer16)imageHeight, (WT_Unsigned_Integer16)imageWidth,     
      imageOutFormat, 0, pWtColorMap, imageHeight * outScLnSz, outScLns.asArrayPtr(), wtMin, wtMax, WD_False));
    wtImage->serialize(*getWtFile());
  }
}



//====================================================================
//
void OdDwfDevice::dc_text(const WT_Text& wtText)
{
  if (IsNonprocessRun()) return;
  push_Url();

  wtText.serialize( *getWtFile() );
}


//====================================================================
//
void OdDwfDevice::dc_font(const WT_Font& wtFont)
{
  if (IsNonprocessRun()) return;
  getWtFile()->desired_rendition().font() = wtFont;
}

//====================================================================
//
void OdDwfDevice::dc_url_item(WT_URL_Item& url_item)
{
  getWtFile()->desired_rendition().url().add_url_optimized( url_item, *getWtFile());
}

void OdDwfDevice::dc_url_clear()
{
  getWtFile()->desired_rendition().url().clear();
}

void OdDwfDevice::dc_urls_remove_all()
{
  getWtFile()->desired_rendition().url_lookup_list().remove_all();
}

void OdDwfDevice::dc_layers_remove_all()
{
  getWtFile()->layer_list().remove_all();
}

static int mergeNrcClips(int nrcContours1, const int* pNrcCounts1, const OdGsDCPointArray &nrcPoints1,
                         int nrcContours2, const int* pNrcCounts2, const OdGsDCPointArray &nrcPoints2,
                         OdIntArray &nrcCounts, OdGsDCPointArray &nrcPoints);

void OdDwfDevice::dc_pushClip(int nrcContours, const int* nrcCounts, const OdGsDCPointArray &nrcPointsIn)
{
  if (IsNonprocessRun()) return;
  // save old viewport
  m_viewportClips.push(classFactory()->Create_Viewport(getWtFile()->desired_rendition().viewport()));

  // probably intersecting with prev. clip necessary - todo - AMark : 17.02.2009 : implemented
  if (!m_nrcClips.empty())
  {
    // merge nrc clips
    const NrcClipEntry &topEntry = m_nrcClips.top();
    OdIntArray resClipContours; OdGsDCPointArray resClipPoints;
    mergeNrcClips((int)topEntry.m_nrcContours.size(), topEntry.m_nrcContours.asArrayPtr(), topEntry.m_nrcPoints,
                  nrcContours, nrcCounts, nrcPointsIn,
                  resClipContours, resClipPoints);
    NrcClipEntry newEntry;
    newEntry.m_nrcContours = resClipContours;
    newEntry.m_nrcPoints = resClipPoints;
    newEntry.m_bLastNrcState = m_bWasNrcViewportClip;
    m_nrcClips.push(newEntry);
    m_bWasNrcViewportClip = true;
  }
  else
  {
    // save old nrc clip
    NrcClipEntry newEntry;
    newEntry.m_nrcContours.resize((OdUInt32)nrcContours);
    memcpy(newEntry.m_nrcContours.asArrayPtr(), nrcCounts, sizeof(int) * nrcContours);
    newEntry.m_nrcPoints = nrcPointsIn;
    newEntry.m_bLastNrcState = false;
    m_nrcClips.push(newEntry);
  }
  const NrcClipEntry &nrcEntry = m_nrcClips.top();
  nrcContours = (int)nrcEntry.m_nrcContours.size();
  nrcCounts = nrcEntry.m_nrcContours.asArrayPtr();
  const OdGsDCPointArray &nrcPoints = nrcEntry.m_nrcPoints;

  int nTotalPoints = 0; // The total number of points in the contour set (should equal the sum of the counts.)
  int i, v;
  for(i = 0; i < nrcContours; i++)
    nTotalPoints += nrcCounts[i];   // To add the number of points in the ith contour
  
  OdArray<WT_Logical_Point, OdMemoryAllocator<WT_Logical_Point> > pWtPoints;
  pWtPoints.resize(nTotalPoints);
  // The array of points used by the contours
  OdArray<WT_Integer32, OdMemoryAllocator<WT_Integer32> > pContourCounts;
  pContourCounts.resize(nrcContours);

  // The array of counts (each array item indicates the number of points in the respective contour)  
  for (i = 0, v = 0; i < nrcContours; i++)
  {
    int nPoints = nrcCounts[i]; // The number of points in the ith contour
    for (int j = 0; j < nPoints; j++, v++)
      RoundPoint(pWtPoints[v], OdGePoint2d(nrcPoints[v].x, nrcPoints[v].y));
    pContourCounts[i] = nPoints;
  }
  
  // Create the contour set.
  WT_Contour_Set contour_set(*(getWtFile()), nrcContours, pContourCounts.asArrayPtr(), nTotalPoints, 
    pWtPoints.asArrayPtr(), WD_True);
  
  /* WT_Contour_Set wtContourSet;
  wtContourSet.set(0xFFFFFFFFU, contour_set.contours(), contour_set.counts(), contour_set.total_points(), 
    contour_set.points(), WD_True);*/

  std::auto_ptr<WT_Viewport> viewPort(classFactory()->Create_Viewport(*getWtFile(), "", contour_set));
 
  getWtFile()->desired_rendition().viewport() = *viewPort;

  pushSoftClip(nrcContours, nrcCounts, nrcPoints);
}

void OdDwfDevice::dc_popClip()
{
  if (IsNonprocessRun()) return;
  if (m_viewportClips.empty())
    throw OdError(eNotApplicable);
  getWtFile()->desired_rendition().viewport() = *m_viewportClips.top();
  delete m_viewportClips.top();
  m_viewportClips.pop();
  m_bWasNrcViewportClip = m_nrcClips.top().m_bLastNrcState;
  m_nrcClips.pop();

  popSoftClip();
}

// caller responsible for memory of returned polygon
static gpc_polygon getNrcClipAsGpcPolygon(int nrcContours, const int* pNrcCounts, const OdGsDCPointArray &nrcPoints)
{
  gpc_polygon result;
  result.num_contours = 0;
  result.contour      = 0;
  result.hole         = 0;
  
  if(nrcContours < 1)
    return result;

  result.num_contours = 1;
  result.hole = ::gpc_alloc<int>(1);
  result.hole[0] = 0;
  result.contour = ::gpc_alloc<gpc_vertex_list>(1);
  result.contour[0].num_vertices = pNrcCounts[0];
  result.contour[0].vertex = ::gpc_alloc<gpc_vertex>(pNrcCounts[0]);
  int i;
  for(i = 0; i < pNrcCounts[0]; i ++)
  {
    result.contour[0].vertex[i].x = nrcPoints[i].x;
    result.contour[0].vertex[i].y = nrcPoints[i].y;
  } // NB: this value of i will be used in the next block

  int j;
  for(j = 1; j < nrcContours; j ++)
  {
    int zero = 0;

    gpc_polygon next;
    next.num_contours = 1;
    next.hole = &zero;
    gpc_vertex_list vList;
    next.contour = &vList;
    vList.num_vertices = pNrcCounts[j];
    OdGePoint2dArray vertices;
    vertices.resize(vList.num_vertices);
    vList.vertex = (gpc_vertex*)vertices.asArrayPtr();
    int k;
    for(k = 0; k < vList.num_vertices; k ++, i ++)
    {
      vList.vertex[k].x = nrcPoints[i].x;
      vList.vertex[k].y = nrcPoints[i].y;
    }

    gpc_polygon res;
    gpc_polygon_clip(GPC_XOR, &result, &next, &res);
    gpc_free_polygon(&result);
    result = res;
  }

  return result;
}

// convert gpc polygon to nrc clip (retval: nrcContours)
static int getGpcPolygonAsNrcClip(const gpc_polygon *poly, OdIntArray &nrcCounts, OdGsDCPointArray &nrcPoints)
{
  nrcCounts.resize((OdUInt32)poly->num_contours);
  int nVerts = 0, i;
  for (i = 0; i < poly->num_contours; i++)
  {
    nVerts += poly->contour[i].num_vertices;
  }
  nrcPoints.resize((OdUInt32)nVerts);
  int *pCounts = nrcCounts.asArrayPtr();
  OdGsDCPoint *pPoints = nrcPoints.asArrayPtr();
  for (i = 0; i < poly->num_contours; i++, pCounts++)
  {
    *pCounts = poly->contour[i].num_vertices;
    for (int j = 0; j < poly->contour[i].num_vertices; j++, pPoints++)
    {
      pPoints->x = OdRoundToLong(poly->contour[i].vertex[j].x);
      pPoints->y = OdRoundToLong(poly->contour[i].vertex[j].y);
    }
  }
  return poly->num_contours;
}

// use gpc to merge two nrc clip's (retval: nrcContours)
static int mergeNrcClips(int nrcContours1, const int* pNrcCounts1, const OdGsDCPointArray &nrcPoints1,
                         int nrcContours2, const int* pNrcCounts2, const OdGsDCPointArray &nrcPoints2,
                         OdIntArray &nrcCounts, OdGsDCPointArray &nrcPoints)
{
  gpc_polygon clip1 = getNrcClipAsGpcPolygon(nrcContours1, pNrcCounts1, nrcPoints1);
  gpc_polygon clip2 = getNrcClipAsGpcPolygon(nrcContours2, pNrcCounts2, nrcPoints2);
  gpc_polygon res;
  gpc_polygon_clip(GPC_INT, &clip1, &clip2, &res);
  gpc_free_polygon(&clip2);
  gpc_free_polygon(&clip1);
  int nrcContours = getGpcPolygonAsNrcClip(&res, nrcCounts, nrcPoints);
  gpc_free_polygon(&res);
  return nrcContours;
}

void OdDwfDevice::pushSoftClip(int nrcContours, const int* pNrcCounts, const OdGsDCPointArray &nrcPoints)
{
  if (IsNonprocessRun()) return;
  gpc_polygon clip = getNrcClipAsGpcPolygon(nrcContours, pNrcCounts, nrcPoints);
  if(!m_softClips.empty())
  {
    gpc_polygon res;
    gpc_polygon_clip(GPC_INT, &clip, &m_softClips.top(), &res);
    gpc_free_polygon(&clip);
    clip = res;
  }
  m_softClips.push(clip);
}

void OdDwfDevice::popSoftClip()
{
  if (IsNonprocessRun()) return;
  gpc_free_polygon(&m_softClips.top());
  m_softClips.pop();
}

OdGiRasterImagePtr OdDwfDevice::createShellRaster(bool bMask, OdGiDrawable* pShell, double dVpWidth, double dVpHeight, const OdGePoint3d& ptVpCenter, const OdGsDCRect& gsRect)
{
  OdDbBaseDatabasePEPtr pDb(userGiContext()->database());
  OdGsDevicePtr pMaskDevice = pDb->appServices(userGiContext()->database())->gsBitmapDevice(NULL, userGiContext()->database(), OdDbBaseHostAppServices::kFor2dExportRender);
  if (!pMaskDevice.get())
  {
    throw OdError(OD_T("Can't create Bitmap Device for exporting gradient hatch to DWF."));
  }
  OdRxDictionaryPtr pDevProps = pMaskDevice->properties();
  if (!pDevProps.isNull())
  {
    if (pDevProps->has(OD_T("PrecisionMode")))
      pDevProps->putAt(OD_T("PrecisionMode"), OdRxVariantValue(true));
  }
  pMaskDevice->setUserGiContext(userGiContext());
  OdGsViewPtr pViewMask = pMaskDevice->createView();
  pViewMask->setView(ptVpCenter + OdGeVector3d::kZAxis, ptVpCenter, OdGeVector3d::kYAxis, dVpWidth, dVpHeight);

  OdGsViewPtr pView = pMaskDevice->createView();
  pMaskDevice->addView(pView);
  pViewMask->setViewport(gsRect);
  pViewMask->setViewportBorderVisibility(false);

  pMaskDevice->addView(pViewMask);
  if (bMask)
    pMaskDevice->setBackgroundColor(ODRGB(0, 0, 0));
  pMaskDevice->onSize(gsRect);

  if (bMask)
    pMaskDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(1)));
  else
    pMaskDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));

  ODCOLORREF imgPalette[2] = { 0, ODRGB(255, 255, 255) };
  if (bMask)
    pMaskDevice->setLogicalPalette(imgPalette, 2);
  else
    pMaskDevice->setLogicalPalette(m_logPalette.asArrayPtr(), m_logPalette.size());
  pViewMask->add(pShell, 0);
  pMaskDevice->update();

  OdGiRasterImagePtr pMask = pMaskDevice->properties()->getAt(OD_T("RasterImage"));
  pViewMask->eraseAll();
  return pMask;
}

void OdDwfDevice::shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                      OdInt32 faceListSize, const OdInt32* faceList,
                      const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
{
  if (IsNonprocessRun()) return;
  if (m_bDrawShellAsRaster)
  {
    OdGeExtents3d exts;
    for (int i = 0; i < numVertices; ++i)
      exts.addPoint(vertexList[i]);
    OdGePoint3d ptVpCenter = exts.center();
    double dVpWidth = exts.maxPoint().x - exts.minPoint().x;
    double dVpHeight = exts.maxPoint().y - exts.minPoint().y;
    OdUInt32 pixelWidthNew = (OdUInt32)dVpWidth;
    OdUInt32 pixelHeightNew = (OdUInt32)dVpHeight;
    if (pixelWidthNew < 40 || pixelHeightNew < 40)
    {
      pixelWidthNew = 400;
      pixelHeightNew = (OdUInt32)(400 * dVpHeight / dVpWidth);
    }
    while (pixelWidthNew > 800 || pixelHeightNew > 800)
    {
      pixelWidthNew /= 2;
      pixelHeightNew /= 2;
    }
    OdGsDCRect gsRect(0, pixelWidthNew, pixelHeightNew, 0);

    ShellDrawable drawShellMask(numVertices, vertexList, faceListSize, faceList);
    OdGiRasterImagePtr pMask = createShellRaster(true, &drawShellMask, dVpWidth, dVpHeight, ptVpCenter, gsRect);
    ShellDrawableEx drawShell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
    OdGiRasterImagePtr pRaster = createShellRaster(false, &drawShell, dVpWidth, dVpHeight, ptVpCenter, gsRect);
    OdGeExtents2d ext2d;
    exts.convert2d(ext2d);
    dc_raster_image(pRaster, pMask, ext2d, false);
  }
  else
    Od2dExportDevice::shellProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
}
void OdDwfView::shell(OdInt32 numVertices, const OdGePoint3d* vertexList,
  OdInt32 faceListSize, const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData)
{
  if (const OdGiDrawable* pDrawable = currentDrawable())
  {
    OdDbBaseHatchPEPtr pHatchPE = OdDbBaseHatchPE::cast(pDrawable);

    if(!pHatchPE.isNull())
    {
      if( pHatchPE->isSolidFill(pDrawable) && pHatchPE->isGradient(pDrawable) && device()->getExportImpl()->getParams().exportGradientAsRaster())
        device()->m_bDrawShellAsRaster = true;
    }
  }
  Od2dExportView::shell(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
  device()->m_bDrawShellAsRaster = false;
}
}
