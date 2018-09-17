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

#ifndef _ODGIFASTEXTCALC_INCLUDED_
#define _ODGIFASTEXTCALC_INCLUDED_

#include "Gi/GiBaseVectorizer.h"
#include "Gi/GiTransformed.h"
#include "Gi/GiDummyGeometry.h"
#include "OdStack.h"

#include "TD_PackPush.h"

/** \details This class was used to resolve compiling error with VS2010.
    \remarks https://support.microsoft.com/en-us/kb/127900
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class OdGiFastExtCalcViewportDrawImpl : public OdGiDummyViewportDraw<OdGiDummyViewportGeometry<OdGiViewportDraw_> > { };
/** \details Base class typedef for OdGiFastExtCalc.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
typedef OdGiTransformed<OdGiJoinCommonDraw<OdGiWorldDrawImpl, OdGiFastExtCalcViewportDrawImpl> > OdGiFastExtCalcBase;

/** \details
    This class is an implementation of the subWorldDraw() framework to quickly calculate the WCS extents of an OdGiDrawable object.
    \remarks
    This class is the same as OdGiExtCalc, but calculates the extents with lower precision to improve speed.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT ODRX_ABSTRACT OdGiFastExtCalc : public OdGiFastExtCalcBase
{
  typedef OdGeExtents3d Extents;
  OdGeExtents3d         m_worldExt;
  OdStack<Extents>      m_extStack;
  OdGeExtents3d*        m_pCurrExt;

  enum Flags
  {
    kSetExtentsCalledFlag     = 1,
    kDrawInvisiblesFlag       = 2,
    kDrawInvisiblesNestedFlag = 4,
    kDrawingInitiatedFlag     = 8,
    kViewportDrawEnabledFlag  = 16
  };
  OdUInt8               m_flags;

  void setSetExtentsCalled(bool bSet);
  bool isSetExtentsCalled() const;

  void setDrawingInitiated(bool bSet);
  bool isDrawingInitiated() const;

  void addTextExtents(
    const OdGePoint3d& locExtMin,
    const OdGePoint3d& locExtMax,
    const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction);
protected:
  ODRX_USING_HEAP_OPERATORS(OdGiFastExtCalcBase);

  OdGiFastExtCalc();
public:

  /** \details
    Resets the accumulated extents for the OdGiDrawable object associated with this Vectorization object.
  */
  void resetExtents();
  
  /** \details
    Returns the accumulated extents for the OdGiDrawable object associated with this Vectorization object.
    \param extents [out]  Receives the extents.
  */
  void getExtents(OdGeExtents3d& extents) const;

  /** \details
    Sets whether invisible entities will be drawn.
    \param bSet [in]  true - to draw invisible entities, false - do not draw.
  */
  void setDrawInvisible(bool bSet);

  /** \details
    Returns whether invisible entities are to be drawn.
  */
  bool isDrawInvisible() const;

  /** \details
    Sets whether invisible nested entities will be drawn.
    \param bSet [in]  true - to draw invisible nested entities, false - do not draw.
  */
  void setDrawInvisibleNested(bool bSet);

  /** \details
    Returns whether invisible nested entities are to be drawn.
  */
  bool isDrawInvisibleNested() const;

  /** \details
    Resets flag which is set after first draw call. This flag informs extents calculator that nested
    entities draw.
  */
  void resetFirstDrawFlag();

  /** \details
    Sets whether viewportDraw method will be called after worldDraw method.
    \param bSet [in]  true - to enable viewportDraw call, false - to disable viewportDraw call.
  */
  void setViewportDrawEnabled(bool bSet);

  /** \details
    Returns whether viewportDraw method will be called after worldDraw method.
  */
  bool isViewportDrawEnabled() const;

  /**\details
    Returns pointer onto OdGiWorldDraw interface.
  */
  OdGiWorldDraw *getWorldDraw() const;

  /**\details
    Returns pointer onto OdGiViewportDraw interface.
  */
  OdGiViewportDraw *getViewportDraw() const;

  /**\details
    Returns pointer onto OdGiWorldGeometry interface.
  */
  OdGiWorldGeometry *getWorldGeometry() const;

  /**\details
    Returns pointer onto OdGiViewportGeometry interface.
  */
  OdGiViewportGeometry *getViewportGeometry() const;

  /////////////////////////////////////////////////////////////////////////////
  // OdGiCommonDraw Overrides

  bool regenAbort() const;
  double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;
  OdGiRegenType regenType() const;

   /////////////////////////////////////////////////////////////////////////////
  // OdGiGeometry Overrides

  void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);

  void circle(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint);

  void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple);

  void circularArc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple);

  void polyline(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    OdGsMarker baseSubEntMarker = -1);

  void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList);

  void pline(const OdGiPolyline& polyline, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);


  /** \details
    Introduces a shape into this vectorization context.
    
    \param position [in]  Position of the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
    \param direction [in]  Baseline direction for the shape.
    \param normal [in]  Normal vector.
  */
  void shape(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    int shapeNumber, const OdGiTextStyle* pTextStyle);

  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    double height, double width, double oblique, const OdString& msg);

  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle);


  void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);

  void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint );

  void nurbs(const OdGeNurbCurve3d& nurbsCurve);

  void ellipArc(const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple);

  void mesh(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void worldLine(const OdGePoint3d points[2]);

  void image(const OdGiImageBGRA32& img,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit);

  void edge(const OdGiEdge2dArray& edges);

  void polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                 const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize);

  void rowOfDots(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  void setExtents(const OdGePoint3d *newExtents);

  void pushClipBoundary(OdGiClipBoundary* pBoundary);
  void popClipBoundary();

  void pushModelTransform(const OdGeMatrix3d& xfm);
  void popModelTransform();

  void draw(const OdGiDrawable* pDrawable);
};

inline void OdGiFastExtCalc::setSetExtentsCalled(bool bSet)
{
  SETBIT(m_flags, kSetExtentsCalledFlag, bSet);
}

inline bool OdGiFastExtCalc::isSetExtentsCalled() const
{
  return GETBIT(m_flags, kSetExtentsCalledFlag);
}

inline void OdGiFastExtCalc::setDrawingInitiated(bool bSet)
{
  SETBIT(m_flags, kDrawingInitiatedFlag, bSet);
}

inline bool OdGiFastExtCalc::isDrawingInitiated() const
{
  return GETBIT(m_flags, kDrawingInitiatedFlag);
}

inline void OdGiFastExtCalc::setDrawInvisible(bool bSet)
{
  SETBIT(m_flags, kDrawInvisiblesFlag, bSet);
}

inline bool OdGiFastExtCalc::isDrawInvisible() const
{
  return GETBIT(m_flags, kDrawInvisiblesFlag);
}

inline void OdGiFastExtCalc::setDrawInvisibleNested(bool bSet)
{
  SETBIT(m_flags, kDrawInvisiblesNestedFlag, bSet);
}

inline bool OdGiFastExtCalc::isDrawInvisibleNested() const
{
  return GETBIT(m_flags, kDrawInvisiblesNestedFlag);
}

inline void OdGiFastExtCalc::resetFirstDrawFlag()
{
  m_flags &= ~kDrawingInitiatedFlag;
}

inline void OdGiFastExtCalc::setViewportDrawEnabled(bool bSet)
{
  SETBIT(m_flags, kViewportDrawEnabledFlag, bSet);
}

inline bool OdGiFastExtCalc::isViewportDrawEnabled() const
{
  return GETBIT(m_flags, kViewportDrawEnabledFlag);
}

#include "TD_PackPop.h"

#endif // #ifndef _ODGIFASTEXTCALC_INCLUDED_
