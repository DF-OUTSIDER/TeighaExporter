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

#ifndef __OD_GI_SECTION_SEOMETRY_MAP_H__
#define __OD_GI_SECTION_SEOMETRY_MAP_H__

#include "Gi/GiExport.h"
#include "Ge/GeMatrix3d.h"
#include "Gi/GiPathNode.h"
#include "Gi/GiDrawable.h"
#include "RxObject.h"

typedef OdArray<OdGiDrawablePtr> OdGiDrawablePtrArray;

/** \details
  <group OdGi_Classes>

  This class defines the selection geometry.

  Corresponding C++ library: TD_Gi
*/
class ODGI_EXPORT OdGiSectionGeometry
{
  public:
    OdGiSectionGeometry():
      m_foregroundFaceTransparency(50),
      m_foregroundEdgeTransparency(50){}

    const OdGiDrawablePtrArray& intBoundaryEnts() const { return m_intBoundaryEnts; }
    const OdGiDrawablePtrArray& intFillEnts() const { return m_intFillEnts; }
    const OdGiDrawablePtrArray& backgroundEnts() const { return m_backgroundEnts; }
    const OdGiDrawablePtrArray& foregroundEnts() const { return m_foregroundEnts; }

    int foregroundFaceTransparency() const { return m_foregroundFaceTransparency; }
    int foregroundEdgeTransparency() const { return m_foregroundEdgeTransparency; }

    OdGiDrawablePtrArray& intBoundaryEnts() { return m_intBoundaryEnts; }
    OdGiDrawablePtrArray& intFillEnts() { return m_intFillEnts; }
    OdGiDrawablePtrArray& backgroundEnts() { return m_backgroundEnts; }
    OdGiDrawablePtrArray& foregroundEnts() { return m_foregroundEnts; }

    void setForegroundTransparency(int faceTransp, int edgeTransp)
    {
      m_foregroundFaceTransparency = faceTransp;
      m_foregroundEdgeTransparency = edgeTransp;
    }

  protected:
    OdGiDrawablePtrArray m_intBoundaryEnts;
    OdGiDrawablePtrArray m_intFillEnts;
    OdGiDrawablePtrArray m_backgroundEnts;
    OdGiDrawablePtrArray m_foregroundEnts;
    int m_foregroundFaceTransparency;
    int m_foregroundEdgeTransparency;
};

/** \details
  <group OdGi_Classes>

  This class defines the selection geometry map.

  Corresponding C++ library: TD_Gi
*/
class ODGI_EXPORT OdGiSectionGeometryMap : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiSectionGeometryMap);
    virtual ~OdGiSectionGeometryMap(){}
    virtual OdGiSectionGeometry* getAt(OdGiDrawable& section, const OdGiPathNode* path,
                                       const OdGiDrawable& drawable, const OdGeMatrix3d& tf) = 0;
    virtual void clear() = 0;
};

/** \details
  The typified smart pointer for the selection geometry map object. This  
  template class is specialization of the OdSmartPtr class for the OdGiSectionGeometryMap class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdGiSectionGeometryMap> OdGiSectionGeometryMapPtr;

/** \details
  <group OdGi_Classes>

  This class defines the section geometry manager.

  Corresponding C++ library: TD_Gi
*/
class ODGI_EXPORT OdGiSectionGeometryManager : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiSectionGeometryManager);
    virtual OdDbStub* getLiveSection(const OdDbStub* layoutId, OdDbStub*& sectionSettingsId) const = 0;
    virtual OdGiDrawablePtr createLiveSection(OdDbBaseDatabase *pDb, OdDbStub *visualStyleId,
                                              const OdGePoint3dArray &points, const OdGeVector3d &verticalDir,
                                              const double *dTop = NULL, const double *dBottom = NULL) const = 0;
    virtual bool isLiveSection(const OdGiDrawable* pDrawable) const = 0;
    virtual bool generateSectionGeometry(OdGiDrawable& section,
                                         const OdGiDrawable& drawable,
                                         const OdGeMatrix3d& xform,
                                         OdGiSectionGeometry& geom,
                                         bool* bHasForeground) const = 0;
    virtual OdGiSectionGeometryMapPtr createSectionGeometryMap() = 0;
    virtual int classifyExtentsRelativelySection(const OdGiDrawable* pDrawable, const OdGeExtents3d& ext) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiSectionGeometryManager object pointers.
*/
typedef OdSmartPtr<OdGiSectionGeometryManager> OdGiSectionGeometryManagerPtr;

#endif // __OD_GI_SECTION_SEOMETRY_MAP_H__
