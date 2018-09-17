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

#ifndef _ODGIDUMMYGEOM_INCLUDED_
#define _ODGIDUMMYGEOM_INCLUDED_

#include "Gi/Gi.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiSubEntityTraitsData.h"
#include "OdStack.h"
#include "StaticRxObject.h"

#include "TD_PackPush.h"

/** \details
This class is a dummy template for OdGiGeometry.
Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperGeometry : public T
{
  OdGiGeometry        *m_Geom;
protected:
  void setGeom(OdGiGeometry &Geom) { m_Geom = &Geom; }
  OdGiGeometry &getGeom() { return *m_Geom; }
public:
  virtual void pline(const OdGiPolyline& p, OdUInt32 i, OdUInt32 n)
  {
    m_Geom->pline(p, i, n);
  }

  virtual OdGeMatrix3d getModelToWorldTransform() const
  {
    return m_Geom->getModelToWorldTransform();
  }
  virtual OdGeMatrix3d getWorldToModelTransform() const
  {
    return m_Geom->getWorldToModelTransform();
  }

  virtual void pushModelTransform(const OdGeVector3d& vNormal)
  {
    m_Geom->pushModelTransform(vNormal);
  }
  virtual void pushModelTransform(const OdGeMatrix3d& xMat)
  {
    m_Geom->pushModelTransform(xMat);
  }
  virtual void popModelTransform()
  {
    m_Geom->popModelTransform();
  }

  // For drawing various primitives.
  //
  virtual void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
  {
    m_Geom->circle(center, radius, normal);
  }

  virtual void circle(const OdGePoint3d& p1, const OdGePoint3d& p2, const OdGePoint3d& p3)
  {
    m_Geom->circle(p1, p2, p3);
  }

  virtual void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    m_Geom->circularArc(center, radius, normal, startVector, sweepAngle, arcType);
  }

  virtual void circularArc(const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    m_Geom->circularArc(start, point, end, arcType);
  }

  virtual void polyline(OdInt32 nbPoints,
    const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = NULL,
    OdGsMarker lBaseSubEntMarker = -1)
  {
    m_Geom->polyline(nbPoints, pVertexList, pNormal, lBaseSubEntMarker);
  }

  virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
  {
    m_Geom->polygon(nbPoints, pVertexList);
  }
  virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal)
  {
    m_Geom->polygon(nbPoints, pVertexList, pNormal);
  }

  virtual void mesh(OdInt32 rows,
    OdInt32 columns,
    const OdGePoint3d* pVertexList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL)
  {
    m_Geom->mesh(rows, columns, pVertexList,  pEdgeData, pFaceData, pVertexData);
  }

  virtual void shell(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL)
  {
    m_Geom->shell(nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
  }

  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    double height, double width, double oblique, const OdString& msg)
  {
    m_Geom->text(position, normal, direction, height, width, oblique, msg);
  }

  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle)
  {
    m_Geom->text(position, normal, direction, msg, length, raw, pTextStyle);
  }

  void ray(const OdGePoint3d& first, const OdGePoint3d& second)
  {
    m_Geom->ray(first, second);
  }

  virtual void nurbs(const OdGeNurbCurve3d& nurbs)
  {
    m_Geom->nurbs(nurbs);
  }

  virtual void xline(const OdGePoint3d& p1, const OdGePoint3d& p2)
  {
    m_Geom->xline(p1, p2);
  }

  virtual void ellipArc(const OdGeEllipArc3d& arc,
    const OdGePoint3d* pEndPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    m_Geom->ellipArc(arc, pEndPointsOverrides, arcType);
  }

  virtual void draw(const OdGiDrawable* pD)
  {
    m_Geom->draw(pD);
  }

  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary)
  {
    m_Geom->pushClipBoundary(pBoundary);
  }
  virtual void popClipBoundary()
  {
    m_Geom->popClipBoundary();
  }

  virtual void worldLine(const OdGePoint3d pnts[2])
  {
    m_Geom->worldLine(pnts);
  }

  virtual void image(const OdGiImageBGRA32& img,
                     const OdGePoint3d& origin,
                     const OdGeVector3d& uVec,
                     const OdGeVector3d& vVec,
                     OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit)
  {
    m_Geom->image(img, origin, uVec, vVec, trpMode);
  }

  virtual void edge(const OdGiEdge2dArray& edges)
  {
    m_Geom->edge(edges);
  }

  virtual void polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                         const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize)
  {
    m_Geom->polypoint(numPoints, vertexList, pColors, pTransparency, pNormals, pSubEntMarkers, nPointSize);
  }

  virtual void rowOfDots(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint)
  {
    m_Geom->rowOfDots(numPoints, startPoint, dirToNextPoint);
  }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperWorldGeometry : public OdGiWrapperGeometry< T >
{
public:
  virtual void setExtents(const OdGePoint3d *newExtents) { ((OdGiWorldGeometry&)(OdGiWrapperGeometry< T >::getGeom())).setExtents(newExtents); }
  virtual void startAttributesSegment() { ((OdGiWorldGeometry&)(OdGiWrapperGeometry< T >::getGeom())).startAttributesSegment(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperViewportGeometry : public OdGiWrapperGeometry< T >
{
public:
  virtual void polylineEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polylineEye(numVertices, vertexList); }

  virtual void polygonEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polygonEye(numVertices, vertexList); }

  virtual void polylineDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polylineDc(numVertices, vertexList); }

  virtual void polygonDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polygonDc(numVertices, vertexList); }

  virtual void rasterImageDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).rasterImageDc(origin,u,v,pImage, uvBoundary, numBoundPts,transparency,brightness,contrast,fade); }

  virtual void  metafileDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,
    bool allowClipping = false) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).metafileDc(origin,u,v,pMetafile,dcAligned,allowClipping); }

  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool dcAligned = true,
    bool allowClipping = false) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).ownerDrawDc(origin,u,v,pDrawable,dcAligned,allowClipping); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class TInterface, class TData>
class OdGiSubEntityTraitsToDataWrapper : public TInterface
{
  protected:
    TData *m_pTraits;
  public:
    OdGiSubEntityTraitsToDataWrapper() : m_pTraits(NULL) {}

    void setTraits(TData &pTraits) { m_pTraits = &pTraits; }
    TData &getTraits() const { return *m_pTraits; }

    void setColor(OdUInt16 n) { m_pTraits->setColor(n); }
    void setTrueColor(const OdCmEntityColor& n) { m_pTraits->setTrueColor(n); }
    void setLayer(OdDbStub* n) { m_pTraits->setLayer(n); }
    void setLineType(OdDbStub* n) { m_pTraits->setLineType(n); }
    void setSelectionMarker(OdGsMarker /*n*/) { } // Stub
    void setFillType(OdGiFillType n) { m_pTraits->setFillType(n); }
    void setFillPlane(const OdGeVector3d* /*n*/ = 0) { } // Stub
    void setLineWeight(OdDb::LineWeight n) { m_pTraits->setLineWeight(n); }
    void setLineTypeScale(double n = 1.0) { m_pTraits->setLineTypeScale(n); }
    void setThickness(double n) { m_pTraits->setThickness(n); }
    void setPlotStyleName(OdDb::PlotStyleNameType n, OdDbStub* m = 0) { m_pTraits->setPlotStyleName(n, m); }
    void setMaterial(OdDbStub* n) { m_pTraits->setMaterial(n); }
    void setMapper(const OdGiMapper * n) { m_pTraits->setMapper(n); }
    void setVisualStyle(const OdDbStub* n) { m_pTraits->setVisualStyle(const_cast<OdDbStub*>(n)); }
    void setTransparency(const OdCmTransparency& transparency) { m_pTraits->setTransparency(transparency); }
    void setDrawFlags(OdUInt32 drawFlags) { m_pTraits->setDrawFlags(drawFlags); }
    void setSelectionGeom(bool bSelectionFlag) { m_pTraits->setSelectionGeom(bSelectionFlag); }
    void setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags) { m_pTraits->setShadowFlags(shadowFlags); }
    void setSectionable(bool bSectionableFlag) { m_pTraits->setSectionable(bSectionableFlag); }
    void setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags) { m_pTraits->setSelectionFlags(selectionFlags); }
    void setSecondaryTrueColor(const OdCmEntityColor& n) { m_pTraits->setSecondaryTrueColor(n); }
    void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod) { m_pTraits->setLineStyleModifiers(pLSMod); }
    void setFill(const OdGiFill* pFill) { m_pTraits->setFill(pFill); }

    OdUInt16 color() const { return m_pTraits->color(); }
    OdCmEntityColor trueColor() const { return m_pTraits->trueColor(); }
    OdDbStub* layer() const { return m_pTraits->layer(); }
    OdDbStub* lineType() const { return m_pTraits->lineType(); }
    OdGiFillType fillType() const { return m_pTraits->fillType(); }
    bool fillPlane(OdGeVector3d& /*n*/) { return false; } // Stub
    OdDb::LineWeight lineWeight() const { return m_pTraits->lineWeight(); }
    double lineTypeScale() const { return m_pTraits->lineTypeScale(); }
    double thickness() const { return m_pTraits->thickness(); }
    OdDb::PlotStyleNameType plotStyleNameType() const { return m_pTraits->plotStyleNameType(); }
    OdDbStub* plotStyleNameId() const { return  m_pTraits->plotStyleNameId(); }
    OdDbStub* material() const { return m_pTraits->material(); }
    const OdGiMapper* mapper() const { return m_pTraits->mapper(); }
    OdDbStub* visualStyle() const { return m_pTraits->visualStyle(); }
    OdCmTransparency transparency() const { return m_pTraits->transparency(); }
    OdUInt32 drawFlags() const { return m_pTraits->drawFlags(); }
    bool selectionGeom() const { return m_pTraits->selectionGeom(); }
    OdGiSubEntityTraits::ShadowFlags shadowFlags() const { return m_pTraits->shadowFlags(); }
    bool sectionable() const { return m_pTraits->sectionable(); }
    OdGiSubEntityTraits::SelectionFlags selectionFlags() const { return m_pTraits->selectionFlags(); }
    OdCmEntityColor secondaryTrueColor() const { return m_pTraits->secondaryTrueColor(); }
    const OdGiDgLinetypeModifiers* lineStyleModifiers() const { return m_pTraits->lineStyleModifiers(); }
    const OdGiFill* fill() const { return m_pTraits->fill(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiSubEntityTraitsToData : public OdGiSubEntityTraitsToDataWrapper<OdStaticRxObject<OdGiSubEntityTraits>, OdGiSubEntityTraitsData>
                                , public OdGiSubEntityTraitsChangedFlags
{
  public:
    OdGiSubEntityTraitsToData() {}
    OdGiSubEntityTraitsToData(OdGiSubEntityTraitsData &pData) { setTraits(pData); }

    static void copyTraits(OdGiSubEntityTraits &pFrom, OdGiSubEntityTraits &pTo, OdUInt32 nFlags = kAllChanged)
    {
      if (GETBIT(nFlags, kColorChanged)) pTo.setTrueColor(pFrom.trueColor());
      if (GETBIT(nFlags, kLayerChanged)) pTo.setLayer(pFrom.layer());
      if (GETBIT(nFlags, kLineTypeChanged)) pTo.setLineType(pFrom.lineType());
      if (GETBIT(nFlags, kFillTypeChanged)) pTo.setFillType(pFrom.fillType());
      if (GETBIT(nFlags, kLineWeightChanged)) pTo.setLineWeight(pFrom.lineWeight());
      if (GETBIT(nFlags, kLineTypeScaleChanged)) pTo.setLineTypeScale(pFrom.lineTypeScale());
      if (GETBIT(nFlags, kThicknessChanged)) pTo.setThickness(pFrom.thickness());
      if (GETBIT(nFlags, kPlotStyleChanged)) pTo.setPlotStyleName(pFrom.plotStyleNameType(), pFrom.plotStyleNameId());
      if (GETBIT(nFlags, kMaterialChanged)) pTo.setMaterial(pFrom.material());
      if (GETBIT(nFlags, kMapperChanged)) pTo.setMapper(pFrom.mapper());
      if (GETBIT(nFlags, kVisualStyleChanged)) pTo.setVisualStyle(pFrom.visualStyle());
      if (GETBIT(nFlags, kTransparencyChanged)) pTo.setTransparency(pFrom.transparency());
      if (GETBIT(nFlags, kDrawFlagsChanged)) pTo.setDrawFlags(pFrom.drawFlags());
      if (GETBIT(nFlags, kSelectionGeomChanged)) pTo.setSelectionGeom(pFrom.selectionGeom());
      if (GETBIT(nFlags, kShadowFlagsChanged)) pTo.setShadowFlags(pFrom.shadowFlags());
      if (GETBIT(nFlags, kSectionableChanged)) pTo.setSectionable(pFrom.sectionable());
      if (GETBIT(nFlags, kSelectionFlagsChanged)) pTo.setSelectionFlags(pFrom.selectionFlags());
      if (GETBIT(nFlags, kSecColorChanged)) pTo.setSecondaryTrueColor(pFrom.secondaryTrueColor());
      if (GETBIT(nFlags, kLSModifiersChanged)) pTo.setLineStyleModifiers(pFrom.lineStyleModifiers());
      if (GETBIT(nFlags, kFillChanged)) pTo.setFill(pFrom.fill());
    }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiSubEntityTraitsWrapper : public OdGiSubEntityTraitsToDataWrapper<T, OdGiSubEntityTraits>
{
private:
  OdGiSubEntityTraits *&baseTraits() { return OdGiSubEntityTraitsToDataWrapper<T, OdGiSubEntityTraits>::m_pTraits; }
protected:
  void setTraits(OdGiSubEntityTraits &pTraits) { baseTraits() = &pTraits; }
  OdGiSubEntityTraits &getTraits() { return *baseTraits(); }
public:
  OdGiSubEntityTraitsWrapper() { }

  // no refcounting
  void addRef() { baseTraits()->addRef(); }
  // no refcounting
  void release() { baseTraits()->release(); }

  void setSelectionMarker(OdGsMarker n) { baseTraits()->setSelectionMarker(n); }
  void setFillPlane(const OdGeVector3d* n = 0) { baseTraits()->setFillPlane(n); }

  bool fillPlane(OdGeVector3d& n) { return baseTraits()->fillPlane(n); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiWrapperCommonDraw : public T
{
  OdGiCommonDraw *m_pD;
protected:
  void setDraw(OdGiCommonDraw &pD) { m_pD = &pD; }
  OdGiCommonDraw *getDraw() const { return m_pD; }
public:
  OdGiWrapperCommonDraw() : m_pD(0) {}
  virtual OdGiGeometry& rawGeometry() const            { return m_pD->rawGeometry(); }
  virtual OdGiRegenType regenType() const              { return m_pD->regenType(); }
  virtual bool regenAbort() const                      { return m_pD->regenAbort(); }
  virtual OdGiSubEntityTraits& subEntityTraits() const { return m_pD->subEntityTraits(); }
  virtual bool isDragging() const                      { return m_pD->isDragging(); }
  virtual OdGiContext* context() const                 { return m_pD->context(); }
  virtual OdUInt32 numberOfIsolines() const            { return m_pD->numberOfIsolines(); }
  virtual const OdGiPathNode* currentGiPath() const    { return m_pD->currentGiPath(); }
  virtual bool displayModelerSilhouettes() const       { return m_pD->displayModelerSilhouettes(); }
  virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const { return m_pD->deviation(t, p); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiWrapperViewportDraw : public OdGiWrapperCommonDraw< T >
{
public:
  virtual OdGiViewport& viewport() const { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->viewport(); }
  virtual OdGiViewportGeometry& geometry() const  { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->geometry(); }
  virtual OdUInt32 sequenceNumber() const  { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->sequenceNumber(); }
  virtual bool isValidId(const OdUInt32 viewportId) const { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->isValidId(viewportId); }
  virtual OdDbStub* viewportObjectId() const { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->viewportObjectId(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiWrapperWorldDraw : public OdGiWrapperCommonDraw< T >
{
public:
  virtual OdGiWorldGeometry& geometry() const  { return ((OdGiWorldDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->geometry(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiViewportDraw_Stub : public OdGiWrapperViewportDraw<OdGiViewportDraw>
                            , public OdGiWrapperViewportGeometry<OdGiViewportGeometry>
                            , public OdGiSubEntityTraitsWrapper<OdGiSubEntityTraits>
{
public:
  ODRX_USING_HEAP_OPERATORS( OdGiWrapperViewportDraw<OdGiViewportDraw> );

  OdGiViewportDraw_Stub(OdGiViewportDraw &pVD, OdGiViewportGeometry &pVG, OdGiSubEntityTraits &pST)
  {
    setDraw(pVD);
    setGeom(pVG);
    setTraits(pST);
  }

  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiViewportDraw_Stub*>(this)); }
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiViewportDraw_Stub*>(this)); }
  virtual OdGiViewportGeometry& geometry() const  { return *static_cast<OdGiViewportGeometry*>(const_cast<OdGiViewportDraw_Stub*>(this)); }

  void addRef() { }
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiWorldDraw_Stub : public OdGiWrapperWorldDraw<OdGiWorldDraw>
                         , public OdGiWrapperWorldGeometry<OdGiWorldGeometry>
                         , public OdGiSubEntityTraitsWrapper<OdGiSubEntityTraits>
{
public:
  ODRX_USING_HEAP_OPERATORS( OdGiWrapperWorldDraw<OdGiWorldDraw> );

  OdGiWorldDraw_Stub(OdGiWorldDraw &pVD, OdGiWorldGeometry &pVG, OdGiSubEntityTraits &pST)
  {
    setDraw(pVD);
    setGeom(pVG);
    setTraits(pST);
  }

  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiWorldDraw_Stub*>(this)); }
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiWorldDraw_Stub*>(this)); }
  virtual OdGiWorldGeometry& geometry() const  { return *static_cast<OdGiWorldGeometry*>(const_cast<OdGiWorldDraw_Stub*>(this)); }

  void addRef() { }
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyGeometry : public T
{
public:
  void circle(const OdGePoint3d& /*center*/, double /*radius*/, const OdGeVector3d& /*normal*/) {}

  void circle(const OdGePoint3d& /*firstPoint*/, const OdGePoint3d& /*secondPoint*/, const OdGePoint3d& /*thirdPoint*/) {}

  void circularArc(const OdGePoint3d& /*center*/,
    double /*radius*/,
    const OdGeVector3d& /*normal*/,
    const OdGeVector3d& /*startVector*/,
    double /*sweepAngle*/,
    OdGiArcType /*arcType*/ = kOdGiArcSimple) {}

  void circularArc(const OdGePoint3d& /*firstPoint*/,
    const OdGePoint3d& /*secondPoint*/,
    const OdGePoint3d& /*thirdPoint*/,
    OdGiArcType /*arcType */= kOdGiArcSimple) {}

  void polyline(OdInt32 /*numVertices*/,
    const OdGePoint3d* /*vertexList*/,
    const OdGeVector3d* /*pNormal */= 0,
     OdGsMarker /*baseSubEntMarker */= -1) {}

  void polygon(OdInt32 /*numVertices*/, const OdGePoint3d* /*vertexList*/) {}
  void polygon(OdInt32 /*numVertices*/, const OdGePoint3d* /*vertexList*/, const OdGeVector3d* /*pNormal*/) {}

  void pline(const OdGiPolyline& /*polylPine*/, OdUInt32 /*fromIndex */= 0, OdUInt32 /*numSegs */= 0) {}

  void shape(const OdGePoint3d& /*position*/,
    const OdGeVector3d& /*normal*/, const OdGeVector3d& /*direction*/,
    int /*shapeNumber*/, const OdGiTextStyle* /*pTextStyle*/) {}

  void text(const OdGePoint3d& /*position*/,
    const OdGeVector3d& /*normal*/, const OdGeVector3d& /*direction*/,
    double /*height*/, double /*width*/, double /*oblique*/, const OdString& /*msg*/) {}

  void text(const OdGePoint3d& /*position*/,
    const OdGeVector3d& /*normal*/, const OdGeVector3d& /*direction*/,
    const OdChar* /*msg*/, OdInt32 /*length*/, bool /*raw*/, const OdGiTextStyle* /*pTextStyle*/) {}


  void xline(const OdGePoint3d& /*firstPoint*/, const OdGePoint3d& /*secondPoint*/) {}

  void ray(const OdGePoint3d& /*basePoint*/, const OdGePoint3d& /*throughPoint */) {}

  void nurbs(const OdGeNurbCurve3d& /*nurbsCurve*/) {}

  void ellipArc(const OdGeEllipArc3d& /*ellipArc*/,
    const OdGePoint3d* /*endPointsOverrides */= 0,
    OdGiArcType /*arcType */= kOdGiArcSimple) {}

  void mesh(OdInt32 /*numRows*/,
    OdInt32 /*numColumns*/,
    const OdGePoint3d* /*vertexList*/,
    const OdGiEdgeData* /*pEdgeData */= 0,
    const OdGiFaceData* /*pFaceData */= 0,
    const OdGiVertexData* /*pVertexData */= 0) {}

  void shell(OdInt32 /*numVertices*/,
    const OdGePoint3d* /*vertexList*/,
    OdInt32 /*faceListSize*/,
    const OdInt32* /*faceList*/,
    const OdGiEdgeData* /*pEdgeData */= 0,
    const OdGiFaceData* /*pFaceData */= 0,
    const OdGiVertexData* /*pVertexData */= 0) {}

  void worldLine(const OdGePoint3d /*points*/[2]) {}

  void image(const OdGiImageBGRA32& /*img*/,
    const OdGePoint3d& /*origin*/,
    const OdGeVector3d& /*uVec*/,
    const OdGeVector3d& /*vVec*/,
    OdGiRasterImage::TransparencyMode /*trpMode*/ = OdGiRasterImage::kTransparency8Bit) {}

  virtual void edge(const OdGiEdge2dArray& /*edges*/) {}

  void polypoint(OdInt32 /*numPoints*/, const OdGePoint3d* /*vertexList*/, const OdCmEntityColor* /*pColors*/, const OdCmTransparency* /*pTransparency*/,
                 const OdGeVector3d* /*pNormals*/, const OdGsMarker* /*pSubEntMarkers*/, OdInt32 /*nPointSize*/) {}

  void rowOfDots(OdInt32 /*numPoints*/, const OdGePoint3d& /*startPoint*/, const OdGeVector3d& /*dirToNextPoint*/) {}

  void setExtents(const OdGePoint3d * /*newExtents*/) {}

  void pushClipBoundary(OdGiClipBoundary* /*pBoundary*/) {}
  void popClipBoundary() {}
  void draw(const OdGiDrawable* /*pDrawable*/) {}

  void pushModelTransform(const OdGeMatrix3d& /*xMat*/) { }

  void pushModelTransform(const OdGeVector3d& /*normal*/) { }

  void popModelTransform() { }

  OdGeMatrix3d getModelToWorldTransform() const { return OdGeMatrix3d::kIdentity; }

  OdGeMatrix3d getWorldToModelTransform() const { return OdGeMatrix3d::kIdentity; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyWorldGeometry : public OdGiDummyGeometry< T >
{
public:
  virtual void setExtents(
    const OdGePoint3d * /*newExtents*/) { }

  virtual void startAttributesSegment() {}
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyViewportGeometry : public OdGiDummyGeometry< T >
{
public:
  virtual void polylineEye(
    OdUInt32 , 
    const OdGePoint3d* ) { }

  virtual void polygonEye(
    OdUInt32 , 
    const OdGePoint3d* ) {  }

  virtual void polylineDc(
    OdUInt32 , 
    const OdGePoint3d* ) {  }

  virtual void polygonDc(
    OdUInt32 , 
    const OdGePoint3d* ) {  }

  virtual void rasterImageDc(
    const OdGePoint3d& ,
    const OdGeVector3d& ,
    const OdGeVector3d& ,
    const OdGiRasterImage* , 
    const OdGePoint2d* , 
    OdUInt32 ,
    bool  = false,
    double  = 50.0,
    double  = 50.0,
    double  = 0.0) {  }

  virtual void  metafileDc(
    const OdGePoint3d& ,
    const OdGeVector3d& ,
    const OdGeVector3d& ,
    const OdGiMetafile* ,
    bool  = true,
    bool  = false) {  }

  virtual void ownerDrawDc(
    const OdGePoint3d& ,
    const OdGeVector3d& ,
    const OdGeVector3d& ,
    const OdGiSelfGdiDrawable* ,
    bool  = true,
    bool  = false) {  }
};

// OdGiSubEntityTraits dummy template

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummySubEntityTraits : public T
{
public:
  void setColor(OdUInt16 ) { }
  void setTrueColor(const OdCmEntityColor& ) { }
  void setLayer(OdDbStub* ) { }
  void setLineType(OdDbStub* ) { }
  void setSelectionMarker(OdGsMarker ) { }
  void setFillType(OdGiFillType ) { }
  void setFillPlane(const OdGeVector3d* = 0) {}
  void setLineWeight(OdDb::LineWeight ) { }
  void setLineTypeScale(double  = 1.0) { }
  void setThickness(double ) { }
  void setPlotStyleName(OdDb::PlotStyleNameType, OdDbStub* = 0) { }
  void setMaterial(OdDbStub* ) { }
  void setMapper(const OdGiMapper * /*pMapper*/) { }
  void setVisualStyle(const OdDbStub* /*visualStyleId*/) { }
  void setTransparency(const OdCmTransparency& /*transparency*/) { }
  void setDrawFlags(OdUInt32 /*drawFlags*/) { }
  void setSelectionGeom(bool /*bSelectionFlag*/) { }
  void setShadowFlags(OdGiSubEntityTraits::ShadowFlags /*shadowFlags*/) { }
  void setSectionable(bool /*bSectionableFlag*/) { }
  void setSelectionFlags(OdGiSubEntityTraits::SelectionFlags /*selectionFlags*/) { }

  OdUInt16 color() const { return 0; }
  OdCmEntityColor trueColor() const { return OdCmEntityColor(); }
  OdDbStub* layer() const { return NULL; }
  OdDbStub* lineType() const { return NULL; }
  OdGiFillType fillType() const { return kOdGiFillNever; }
  bool fillPlane(OdGeVector3d& /*normal*/) { return false; }
  OdDb::LineWeight lineWeight() const { return OdDb::kLnWt000; }
  double lineTypeScale() const { return 0.0; }
  double thickness() const { return 0.0; }
  OdDb::PlotStyleNameType plotStyleNameType() const { return OdDb::kPlotStyleNameByBlock; }
  OdDbStub* plotStyleNameId() const { return NULL; }
  OdDbStub* material() const { return NULL; }
  const OdGiMapper* mapper() const { return NULL; }
  void addLight(const OdDbStub* /*lightId*/) { }
  OdCmTransparency transparency() const { return OdCmTransparency(); }
  OdUInt32 drawFlags() const { return 0; }
  bool selectionGeom() const { return false; }
  OdGiSubEntityTraits::ShadowFlags shadowFlags() const { return OdGiSubEntityTraits::kShadowsCastAndReceive; }
  bool sectionable() const { return false; }
  OdGiSubEntityTraits::SelectionFlags selectionFlags() const { return OdGiSubEntityTraits::kNoSelectionFlags; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyViewport : public T
{
public:
  virtual OdGeMatrix3d getModelToEyeTransform() const { return OdGeMatrix3d::kIdentity; }
  virtual OdGeMatrix3d getEyeToModelTransform() const { return OdGeMatrix3d::kIdentity; }
  virtual OdGeMatrix3d getWorldToEyeTransform() const { return OdGeMatrix3d::kIdentity; }
  virtual OdGeMatrix3d getEyeToWorldTransform() const { return OdGeMatrix3d::kIdentity; }
  virtual bool isPerspective() const { return false; }
  virtual bool doPerspective(OdGePoint3d& /*p*/) const { return false; }
  virtual bool doInversePerspective(OdGePoint3d& /*p*/) const  { return false; }
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& /*givenWorldpt*/, OdGePoint2d& /*pixelArea*/, bool /*includePerspective*/ = true) const {  }
  virtual OdGePoint3d getCameraLocation() const { return OdGePoint3d::kOrigin; }
  virtual OdGePoint3d getCameraTarget() const { return OdGePoint3d::kOrigin + OdGeVector3d::kZAxis; }
  virtual OdGeVector3d getCameraUpVector() const { return OdGeVector3d::kYAxis; }
  virtual OdGeVector3d viewDir() const { return getCameraTarget() - getCameraLocation();  }
  virtual OdUInt32 viewportId() const { return 0xFFFFFFFF; }
  virtual OdInt16  acadWindowId() const { return -1; }
  virtual void getViewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const { lower_left.set(0,0); upper_right.set(100,100); }
  virtual bool getFrontAndBackClipValues(bool& clip_front, bool& clip_back, double& /*front*/, double& /*back*/) const { clip_front = clip_back = false; return false; }
  virtual double linetypeScaleMultiplier() const { return 1.0; }
  virtual double linetypeGenerationCriteria() const { return 1.0; }
  virtual bool layerVisible(OdDbStub* /*idLayer*/) const { return true; }
};

/** \details
This class is a dummy template for OdGiViewport.
Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperViewport : public T
{
  const OdGiViewport* m_ref;
protected:
  void set(const OdGiViewport& ref) { m_ref = &ref; }
  const OdGiViewport &get() const { return *m_ref; }
public:
  virtual OdGeMatrix3d getModelToEyeTransform() const { return get().getModelToEyeTransform(); }
  virtual OdGeMatrix3d getEyeToModelTransform() const { return get().getEyeToModelTransform(); }
  virtual OdGeMatrix3d getWorldToEyeTransform() const { return get().getWorldToEyeTransform(); }
  virtual OdGeMatrix3d getEyeToWorldTransform() const { return get().getEyeToWorldTransform(); }
  virtual bool isPerspective() const { return get().isPerspective(); }
  virtual bool doPerspective(OdGePoint3d& p) const { return get().doPerspective(p); }
  virtual bool doInversePerspective(OdGePoint3d& p) const  { return get().doInversePerspective(p); }
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt,
    OdGePoint2d& pixelArea, bool includePerspective = true) const
  {
    get().getNumPixelsInUnitSquare(givenWorldpt, pixelArea, includePerspective);
  }
  virtual OdGePoint3d getCameraLocation() const { return get().getCameraLocation(); }
  virtual OdGePoint3d getCameraTarget() const { return get().getCameraTarget(); }
  virtual OdGeVector3d getCameraUpVector() const { return get().getCameraUpVector(); }
  virtual OdGeVector3d viewDir() const { return get().viewDir();  }
  virtual OdUInt32 viewportId() const { return get().viewportId(); }
  virtual OdInt16  acadWindowId() const { return get().acadWindowId(); }
  virtual void getViewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const
  { get().getViewportDcCorners(lower_left, upper_right); }
  virtual bool getFrontAndBackClipValues(bool& clip_front, bool& clip_back, double& front, double& back) const
  { return get().getFrontAndBackClipValues(clip_front, clip_back, front, back); }
  virtual double linetypeScaleMultiplier() const { return get().linetypeScaleMultiplier(); }
  virtual double linetypeGenerationCriteria() const { return get().linetypeGenerationCriteria(); }
  virtual bool layerVisible(OdDbStub* idLayer) const { return get().layerVisible(idLayer); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T1, class T2> class OdGiJoinCommonDraw : public T1, public T2
{
public:
  ODRX_USING_HEAP_OPERATORS(T1);
  virtual OdGiGeometry& rawGeometry() const            { return T1::rawGeometry(); }
  virtual OdGiRegenType regenType() const              { return T1::regenType(); } // dont change from kOdGiStandardDisplay
  virtual bool regenAbort() const                      { return T1::regenAbort(); }
  virtual OdGiSubEntityTraits& subEntityTraits() const { return T1::subEntityTraits(); }
  virtual bool isDragging() const                      { return T1::isDragging(); }
  virtual OdGiContext* context() const                 { return T1::context(); }
  virtual OdUInt32 numberOfIsolines() const            { return T1::numberOfIsolines(); }
  virtual const OdGiPathNode* currentGiPath() const    { return T1::currentGiPath(); }
  virtual bool displayModelerSilhouettes() const       { return T1::displayModelerSilhouettes(); }
  virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const { return T1::deviation(t, p); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiDummyCommonDraw : public T
{
public:
  virtual OdGiRegenType regenType() const              { return kOdGiStandardDisplay; } // dont change from kOdGiStandardDisplay
  virtual bool regenAbort() const                      { return false; }
  virtual bool isDragging() const                      { return false; }
  virtual OdUInt32 numberOfIsolines() const { return 4; }
  virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const { return 0.; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T, class TVp = OdGiViewport> class OdGiDummyViewportDraw : public OdGiDummyCommonDraw< T >
{
private:
  // This class was moved from multiple inheritance to member, since it is crash VS2010 IntelliSense
  mutable OdGiDummyViewport< OdStaticRxObject<TVp> > m_dummyViewport;
public:
  virtual OdGiViewport& viewport() const { return m_dummyViewport; }
  virtual OdUInt32 sequenceNumber() const  { return 0; }
  virtual bool isValidId(const OdUInt32 viewportId) const { return false; }
  virtual OdDbStub* viewportObjectId() const { return 0; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyWorldDraw : public OdGiDummyCommonDraw< T >
{
public:
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyContext : public T
{
public:
  virtual OdDbBaseDatabase* database() const { return 0;}
  virtual OdGiDrawablePtr openDrawable(OdDbStub* ) { return OdGiDrawablePtr(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiViewportDraw_Dummy : public OdGiDummySubEntityTraits< OdGiSubEntityTraits >
                             , public OdGiDummyViewportDraw< OdGiViewportDraw >
                             , public OdGiDummyViewportGeometry< OdGiViewportGeometry >
                             , public OdGiDummyContext< OdGiContext >
{
  OdDbBaseDatabase *m_pDb;
public:
  ODRX_USING_HEAP_OPERATORS( OdGiDummySubEntityTraits< OdGiSubEntityTraits > );

  OdGiViewportDraw_Dummy(OdDbBaseDatabase *pDb = 0) : m_pDb(pDb) { }
  virtual OdDbBaseDatabase* database() const { return m_pDb; }

  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }
  virtual OdGiViewportGeometry& geometry() const  { return *static_cast<OdGiViewportGeometry*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }
  virtual OdGiContext* context() const            { return static_cast<OdGiContext*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }

  void addRef() { }
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiWorldDraw_Dummy : public OdGiDummySubEntityTraits< OdGiSubEntityTraits >
  , public OdGiDummyWorldDraw< OdGiWorldDraw >
  , public OdGiDummyWorldGeometry< OdGiWorldGeometry >
  , public OdGiDummyContext< OdGiContext >
{
  OdDbBaseDatabase *m_pDb;
public:
  ODRX_USING_HEAP_OPERATORS( OdGiDummySubEntityTraits< OdGiSubEntityTraits > );

  OdGiWorldDraw_Dummy(OdDbBaseDatabase *pDb = 0) : m_pDb(pDb) { }
  virtual OdDbBaseDatabase* database() const { return m_pDb; }

  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }
  virtual OdGiWorldGeometry& geometry() const  { return *static_cast<OdGiWorldGeometry*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }
  virtual OdGiContext* context() const            { return static_cast<OdGiContext*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }

  void addRef() { }
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
typedef OdStaticRxObject< OdGiDummySubEntityTraits< OdGiSubEntityTraits > > OdGiSubEntityTraits_Stub;

#include "TD_PackPop.h"

#endif // #ifndef _ODGIDUMMYGEOM_INCLUDED_
