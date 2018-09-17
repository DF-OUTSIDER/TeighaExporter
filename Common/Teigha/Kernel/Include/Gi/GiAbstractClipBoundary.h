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

#ifndef _GI_ABSTRACT_CLIP_BOUNDARY_H_INCLUDED_
#define _GI_ABSTRACT_CLIP_BOUNDARY_H_INCLUDED_

#include "TD_PackPush.h"

#include "GiClipBoundary.h"
#include "GiSectionGeometryOutput.h"
#include "IntArray.h"

/** \details
    Base class for pass additional clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiAbstractClipBoundary
{
  public:
    enum BoundaryType
    {
      kNormal = 0, // Default clip boundary (not pass additional data, clip outside contour)
      kInverted,   // Inverted clip boundary (pass inverted clip array, clip inside contour)
      kExtended,   // Extended clip boundary (pass clipping contours, clip by sorted contours array)
      kComplex,    // Complex clip boundary (extended clip boundary which requires additional preprocessing)
      kPlanar      // Planar clip boundary (clipping by set of 3d planes with ability to output geometry sections)
    };
  public:
    ODRX_HEAP_OPERATORS();

    OdGiAbstractClipBoundary() { }
    virtual ~OdGiAbstractClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
    */
    virtual BoundaryType type() const { return kNormal; }

    /** \details
        Returns cloned object.
    */
    virtual OdGiAbstractClipBoundary *clone() const { return new OdGiAbstractClipBoundary(); }
};

/** \details
    Class to pass inverted clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiInvertedClipBoundary : public OdGiAbstractClipBoundary
{
  protected:
    OdGePoint2dArray m_Points; // Array of points that defines the inverted clip boundary.
  public:
    OdGiInvertedClipBoundary() { }
    ~OdGiInvertedClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
    */
    BoundaryType type() const { return kInverted; }

    /** \details
        Returns inverted clipping boundary.
    */
    const OdGePoint2dArray &invertedClipBoundary() const { return m_Points; }
    /** \details
        Setup inverted clipping boundary.
        \param pPoints [in]  Input points array.
    */
    void setInvertedClipBoundary(const OdGePoint2dArray &pPoints) { m_Points = pPoints; }

    /** \details
        Returns cloned object.
    */
    OdGiAbstractClipBoundary *clone() const {
      OdGiInvertedClipBoundary *pNew = new OdGiInvertedClipBoundary();
      pNew->setInvertedClipBoundary(invertedClipBoundary());
      return pNew;
    }
};

/** \details
    Class to pass extended clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiExtendedClipBoundary : public OdGiAbstractClipBoundary
{
  protected:
    OdIntArray       m_Counts; // Array of contour points counts.
    OdGePoint2dArray m_Points; // Array of points that defines the extended clip boundary contours.
  public:
    OdGiExtendedClipBoundary() { }
    ~OdGiExtendedClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
    */
    BoundaryType type() const { return kExtended; }

    /** \details
        Returns extended clipping boundary contour vertices counts.
    */
    const OdIntArray &clipBoundaryCounts() const { return m_Counts; }
    /** \details
        Setup extended boundary contour vertices counts.
        \param pCounts [in]  Input counts array.
    */
    void setClipBoundaryCounts(const OdIntArray &pCounts) { m_Counts = pCounts; }

    /** \details
        Returns extended clipping boundary points.
    */
    const OdGePoint2dArray &clipBoundaryPoints() const { return m_Points; }
    /** \details
        Setup extended clipping boundary points.
        \param pPoints [in]  Input points array.
    */
    void setClipBoundaryPoints(const OdGePoint2dArray &pPoints) { m_Points = pPoints; }

    /** \details
        Returns cloned object.
    */
    OdGiAbstractClipBoundary *clone() const { return copyExtendedData(new OdGiExtendedClipBoundary()); }
  protected:
    OdGiAbstractClipBoundary *copyExtendedData(OdGiExtendedClipBoundary *pNew) const {
      pNew->setClipBoundaryCounts(clipBoundaryCounts());
      pNew->setClipBoundaryPoints(clipBoundaryPoints());
      return pNew;
    }
};

/** \details
    Class to pass extended clip boundary information which requires preprocessing from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiComplexClipBoundary : public OdGiExtendedClipBoundary
{
  public:
    OdGiComplexClipBoundary() { }
    ~OdGiComplexClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
    */
    BoundaryType type() const { return kComplex; }

    /** \details
        Returns cloned object.
    */
    OdGiAbstractClipBoundary *clone() const { return copyExtendedData(new OdGiComplexClipBoundary()); }
};

/** \details
    Class to pass planar clip boundary information from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiPlanarClipBoundary : public OdGiAbstractClipBoundary
{
  public:
    /** \details
      Describes a clipping plane.
    */
    struct ClipPlane
    {
      /**Plane's origin point.*/
      OdGePoint3d m_origin;
      /**Plane's normal vector.*/
      OdGeVector3d m_normal;

      ClipPlane() {}
      ClipPlane(const OdGePoint3d &origin, const OdGeVector3d &normal)
        : m_origin(origin), m_normal(normal) {}
      ClipPlane &setOrigin(const OdGePoint3d &origin) { m_origin = origin; return *this; }
      const OdGePoint3d &origin() const { return m_origin; }
      ClipPlane &setNormal(const OdGeVector3d &normal) { m_normal = normal; return *this; }
      const OdGeVector3d &normal() const { return m_normal; }
    };
    /** \details
      Defines the type of array of clipping planes.
    */
    typedef OdArray<ClipPlane> ClipPlaneArray;
  protected:
    ClipPlaneArray                       m_ClipPlanes;     // Array of clipping planes.
    mutable OdGiSectionGeometryOutputPtr m_pSectionOutput; // Optional geometry sections output.
  public:
    OdGiPlanarClipBoundary() { }
    ~OdGiPlanarClipBoundary() { }

    /** \details
        Returns type of the inherited boundary.
    */
    BoundaryType type() const { return kPlanar; }

    /** \details
        Returns array of clipping planes.
    */
    const ClipPlaneArray &clipPlanes() const { return m_ClipPlanes; }
    /** \details
        Setup array of clipping planes.
        \param pClipPlanes [in]  Input array of clipping planes.
    */
    void setClipPlanes(const ClipPlaneArray &pClipPlanes) { m_ClipPlanes = pClipPlanes; }

    /** \details
        Returns optional clipping section geometry output.
    */
    OdGiSectionGeometryOutput *sectionGeometryOutput() const { return m_pSectionOutput; }
    /** \details
        Setup optional clipping section geometry output.
        \param pSectionOutput [in]  Input pointer onto section geometry output.
    */
    void setSectionGeometryOutput(OdGiSectionGeometryOutput *pSectionOutput) { m_pSectionOutput = pSectionOutput; }

    /** \details
        Returns cloned object.
    */
    OdGiAbstractClipBoundary *clone() const {
      OdGiPlanarClipBoundary *pNew = new OdGiPlanarClipBoundary();
      pNew->setClipPlanes(clipPlanes());
      pNew->setSectionGeometryOutput(sectionGeometryOutput());
      return pNew;
    }
};

/** \details
    Container for OdGiClipBoundary and OdGiAbstractClipBoundary.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
struct OdGiClipBoundaryWithAbstractData : public OdGiClipBoundary
{
  OdGiAbstractClipBoundary *m_pAbstractData;

  OdGiClipBoundaryWithAbstractData() : m_pAbstractData(NULL) { }
  OdGiClipBoundaryWithAbstractData(const OdGiClipBoundary &pBoundary, const OdGiAbstractClipBoundary *pAbsData = NULL)
    : OdGiClipBoundary(pBoundary), m_pAbstractData(NULL)
  {
    if (pAbsData) m_pAbstractData = pAbsData->clone();
  }
  OdGiClipBoundaryWithAbstractData(const OdGiClipBoundaryWithAbstractData &pBoundary)
    : OdGiClipBoundary(pBoundary), m_pAbstractData(NULL)
  {
    if (pBoundary.m_pAbstractData) m_pAbstractData = pBoundary.m_pAbstractData->clone();
  }
  ~OdGiClipBoundaryWithAbstractData() {
    if (m_pAbstractData) delete m_pAbstractData;
  }
};

#include "TD_PackPop.h"

#endif // _GI_ABSTRACT_CLIP_BOUNDARY_H_INCLUDED_
