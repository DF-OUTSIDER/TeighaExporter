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

/************************************************************************/
/* World Geometry Sample classes                                        */
/* As implemented, these methods do nothing but announce their          */
/* invocation and return                                                */
/************************************************************************/
#ifndef _WORLDGEOMSAMPLE_H_
#define _WORLDGEOMSAMPLE_H_

#include "OdaCommon.h"
#include "CmColorBase.h"

#include "Gi/GiGeometry.h"
#include "Gi/GiCommonDraw.h"
#include "StaticRxObject.h"
#include "Gi/GiWorldGeometry.h"
#include "Gi/GiWorldDraw.h"

#include "Gi/GiViewportDraw.h"
#include "Gi/GiViewportGeometry.h"
#include "Gi/GiDummyGeometry.h"
#include "Ge/GeMatrix3d.h"

#define WC_CALLED(a) printf("%s\n", #a);

OdGiSubEntityTraits_Stub g_Traits;

template<class T> class OdGiCommonDraw_ : public T
{
public:
  virtual OdGiRegenType regenType() const              { return eOdGiRegenTypeInvalid; }
  virtual bool regenAbort() const                      { return false; }
  virtual OdGiSubEntityTraits& subEntityTraits() const { return g_Traits; }
  virtual bool isDragging() const                      { return false; }
  virtual OdGiContext* context() const                 { return NULL; }

  virtual OdUInt32 numberOfIsolines() const
  { 
    const int numIsol = 6;
    printf("numberOfIsolines = %d\n", numIsol);
    return numIsol; 
  }

  virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const { return 0.; }
};

template<class T> class OdGiGeometry_ : public T
{
public:
  virtual OdGeMatrix3d getModelToWorldTransform() const
  {
    WC_CALLED(getModelToWorldTransform);
    return OdGeMatrix3d();
  }
  virtual OdGeMatrix3d getWorldToModelTransform() const
  {
    WC_CALLED(getWorldToModelTransform);
    return OdGeMatrix3d();
  }
  
  virtual void pushModelTransform(const OdGeVector3d& vNormal)
  {
    WC_CALLED(pushModelTransform);
  }

  virtual void pushModelTransform(const OdGeMatrix3d& xMat)
  {
    WC_CALLED(pushModelTransform);
  }

  virtual void popModelTransform()
  {
    WC_CALLED(popModelTransform);
  }
   
  virtual void pline(const OdGiPolyline& , OdUInt32 , OdUInt32 )
  {
    WC_CALLED(pline);
  }
  
  // For drawing various primitives.
  //
  virtual void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
  {
    WC_CALLED(circle);
  }
  
  virtual void circle(const OdGePoint3d& p1, const OdGePoint3d& p2, const OdGePoint3d& p3)
  {
    WC_CALLED(circle);
  }
  
  virtual void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    WC_CALLED(circleArc);
  }
  
  virtual void circularArc(const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    WC_CALLED(circleArc);
  }
  
  virtual void polyline(OdInt32 nbPoints,
    const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = NULL,
    OdGsMarker lBaseSubEntMarker = -1)
  {
    WC_CALLED(polyline);
  }
  
  virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
  {
    WC_CALLED(polygon);
  }
  
  virtual void mesh(OdInt32 rows,
    OdInt32 columns,
    const OdGePoint3d* pVertexList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL)
  {
    WC_CALLED(mesh);
  }
  
  virtual void shell(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL)
  {
    WC_CALLED(shell);
  }
  
  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    double height, double width, double oblique, const OdString& msg)
  {
    WC_CALLED(text);
  }
  
  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle)
  {
    WC_CALLED(text);
  }

  void ray(const OdGePoint3d& first, const OdGePoint3d& second)
  {
    WC_CALLED(ray);
  }
  
  virtual void nurbs(const OdGeNurbCurve3d& nurbs)
  {
    WC_CALLED(nurbs);
  }

  virtual void xline(const OdGePoint3d& p1, const OdGePoint3d& p2)
  {
    WC_CALLED(xline);
  }
  
  virtual void ellipArc(const OdGeEllipArc3d& arc,
    const OdGePoint3d* pEndPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    WC_CALLED(ellipArc);
  }

  virtual void draw(const OdGiDrawable* pD)
  {
    WC_CALLED(draw);
  }
  
  // If you push a clip boundary onto the stack you must pop it;
  //
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary)
  {
    WC_CALLED(pushClipBoundary);
  }

  virtual void popClipBoundary()
  {
    WC_CALLED(popClipBoundary);
  }
  
  virtual void worldLine(const OdGePoint3d pnts[2])
  {
    WC_CALLED(worldLine);
  }

  virtual void image(
    const OdGiImageBGRA32& img,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit)
  {
    WC_CALLED(image);
  }

  virtual void edge(
    const OdGiEdge2dArray& edges)
  {
    WC_CALLED(edge);
  }
};

class OdSampleWorldGeometry : public OdStaticRxObject<OdGiGeometry_<OdGiWorldGeometry> >
                            , public OdStaticRxObject<OdGiCommonDraw_<OdGiWorldDraw> >
{
  ODRX_USING_HEAP_OPERATORS(OdStaticRxObject<OdGiGeometry_<OdGiWorldGeometry> >);

public:
  virtual OdGiGeometry& rawGeometry() const            { return (OdGiGeometry&)*this;      }
	virtual OdGiWorldGeometry& geometry() const          { return (OdGiWorldGeometry&)*this; }

  virtual void setExtents(const OdGePoint3d *pNewExtents) { }
};

class OdSampleViewportDraw : public OdStaticRxObject<OdGiGeometry_<OdGiViewportGeometry> >
                           , public OdStaticRxObject<OdGiCommonDraw_<OdGiViewportDraw> >
                           , public OdStaticRxObject<OdGiViewport>
{
  ODRX_USING_HEAP_OPERATORS(OdStaticRxObject<OdGiGeometry_<OdGiViewportGeometry> >);
public:

  virtual OdGiGeometry& rawGeometry() const            { return (OdGiGeometry&)*this;      }
	virtual OdGiViewportGeometry& geometry() const          { return (OdGiViewportGeometry&)*this; }

  virtual OdGiViewport&         viewport() const { return (OdGiViewport&)*this; }
  virtual OdUInt32              sequenceNumber() const { return 0; }
  virtual bool                  isValidId(const OdUInt32 acgiId) const { return true; }
  virtual OdDbStub*             viewportObjectId() const { return NULL; }

  virtual void polylineEye(OdUInt32 nbPoints, const OdGePoint3d* pPoints) {}
	virtual void polygonEye(OdUInt32 nbPoints, const OdGePoint3d* pPoints) {}
	virtual void polylineDc(OdUInt32 nbPoints, const OdGePoint3d* pPoints) {}
  virtual void polygonDc(OdUInt32 nbPoints, const OdGePoint3d* pPoints) {}
	virtual void rasterImageDc(
		const OdGePoint3d& origin,
		const OdGeVector3d& u,
		const OdGeVector3d& v,
    const OdGiRasterImage* pImg, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0) {}
	virtual void metafileDc(
    const OdGePoint3d& origin,
		const OdGeVector3d& u,
		const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool bDcAligned = true,
    bool bAllowClipping = false) {}
  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool bDcAligned,
    bool bAllowClipping) {}

  virtual OdGeMatrix3d getModelToEyeTransform() const { return OdGeMatrix3d(); }
  virtual OdGeMatrix3d getEyeToModelTransform() const { return OdGeMatrix3d(); }
  virtual OdGeMatrix3d getWorldToEyeTransform() const { return OdGeMatrix3d(); }
  virtual OdGeMatrix3d getEyeToWorldTransform() const { return OdGeMatrix3d(); }
  virtual bool isPerspective() const { return false; }
  virtual bool doPerspective(OdGePoint3d& p) const { return false; }
  virtual bool doInversePerspective(OdGePoint3d&) const  { return false; }
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt, OdGePoint2d& pixelArea, bool includePerspective = true) const {  }
  virtual OdGePoint3d getCameraLocation() const { return OdGePoint3d(); }
  virtual OdGePoint3d getCameraTarget() const { return OdGePoint3d(0,0,1); }
  virtual OdGeVector3d getCameraUpVector() const { return OdGeVector3d(0,1,0); }
  virtual OdGeVector3d viewDir() const { return getCameraTarget() - getCameraLocation();  }
  virtual OdUInt32 viewportId() const { return 0; }
  virtual OdInt16  acadWindowId() const { return 0; }
  virtual void getViewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const { lower_left.set(0,0); upper_right.set(100,100); }
  virtual bool getFrontAndBackClipValues(bool& clip_front, bool& clip_back, double& front, double& back) const { clip_front = clip_back = false; return false; }
  virtual double linetypeScaleMultiplier() const { return 1.; }
  virtual double linetypeGenerationCriteria() const { return 1.; }
  virtual bool layerVisible(OdDbStub* idLayer) const { return true; }
};

#endif // _WORLDGEOMSAMPLE_H_
