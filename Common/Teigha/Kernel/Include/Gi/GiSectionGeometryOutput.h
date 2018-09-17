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

#ifndef __ODGISECTIONGEOMETRYOUTPUT_H__
#define __ODGISECTIONGEOMETRYOUTPUT_H__

#include "Gi/GiEmptyGeometry.h"
#include "Gi/GiDummyGeometry.h"
#include "RxObjectImpl.h"

#include "TD_PackPush.h"

/** \details
    This class is an default implementation of OdGiConveyorGeometry that provides output of section geometry.

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiSectionGeometryOutput : public OdRxObject, public OdGiEmptyGeometry
{
  public:
    /** \details
        This class provides data for OdGiSectionGeometryOutput and inherited classes.

        <group OdGi_Classes>
    */
    struct ODGI_EXPORT SectionGeometryOutputInterface
    {
      /** \details
          Returns destination geometry interface.
      */
      virtual OdGiConveyorGeometry &destinationGeometry() = 0;
      /** \details
          Returns conveyor context interface.
      */
      virtual OdGiConveyorContext &conveyorContext() = 0;
    };
  protected:
    enum Flags
    {
      kOutputClosedPrimitives      = 1,
      kOutputOpenedPrimitives      = 2,
      kClosedPrimitivesAsPolylines = 4,
      kNonSectionableGeometryClip  = 8,
      kToleranceOverrideEnabled    = 16
    };
  protected:
    SectionGeometryOutputInterface *m_pIface;        // Section geometry interface.
    OdUInt32                        m_flags;         // Section geometry output flags.
    OdUInt32                        m_overrideFlags; // Traits overrides flags
    OdGiSubEntityTraitsData         m_overrides;     // Traits overrides
    OdGiSubEntityTraitsData         m_traits;        // Actual traits storage
    double                          m_tolOverride;   // Section tolerance override.
    OdGePoint3dArray                m_tmpPoints;     // Temporary vertexes array
  public:
    OdGiSectionGeometryOutput() : m_pIface(NULL), m_flags(kOutputClosedPrimitives | kNonSectionableGeometryClip)
                                , m_overrideFlags(0), m_tolOverride(1.e-10) {}

    /** \details
        Create new OdGiSectionGeometryOutput object.
    */
    static OdSmartPtr<OdGiSectionGeometryOutput> createObject() { return OdRxObjectImpl<OdGiSectionGeometryOutput>::createObject(); }

    /** \details
        Returns section geometry interface.
    */
    SectionGeometryOutputInterface &getInterface() { return *m_pIface; }
    /** \details
        Sets section geometry interface.
        \param pInterface [in]  Pointer onto section geometry interface.
        \remarks
        Called internally by OdGiOrthoClipperEx conveyor node.
    */
    void setInterface(SectionGeometryOutputInterface *pInterface) { m_pIface = pInterface; }

    /** \details
        Returns true if closed sections output enabled.
    */
    bool isClosedSectionsOutputEnabled() const { return GETBIT(m_flags, kOutputClosedPrimitives); }
    /** \details
        Enables or disables closed sections output.
        \param bSet [in]  New state of closed sections output.
    */
    void enableClosedSectionsOutput(bool bSet) { SETBIT(m_flags, kOutputClosedPrimitives, bSet); }

    /** \details
        Returns true if opened sections output enabled.
    */
    bool isOpenedSectionsOutputEnabled() const { return GETBIT(m_flags, kOutputOpenedPrimitives); }
    /** \details
        Enables or disables opened sections output.
        \param bSet [in]  New state of opened sections output.
    */
    void enableOpenedSectionsOutput(bool bSet) { SETBIT(m_flags, kOutputOpenedPrimitives, bSet); }

    /** \details
        Returns true if enabled output of closed sections as polyline primitives.
    */
    bool isOutputOfClosedSectionsAsPolylinesEnabled() const { return GETBIT(m_flags, kClosedPrimitivesAsPolylines); }
    /** \details
        Enables or disables output of closed sections as polyline primitives.
        \param bSet [in]  New state of closed sections output mode.
    */
    void enableOutputOfClosedSectionsAsPolylines(bool bSet) { SETBIT(m_flags, kClosedPrimitivesAsPolylines, bSet); }

    /** \details
        Returns true if sections generation tolerance override enabled.
    */
    bool isSectionToleranceOverrideEnabled() const { return GETBIT(m_flags, kToleranceOverrideEnabled); }

    /** \details
        Returns sections generation tolerance override value.
    */
    double sectionToleranceOverride() const { return m_tolOverride; }

    /** \details
        Enables sections generation tolerance override and sets it's value.
    */
    void setSectionToleranceOverride(double tolOverride) { SETBIT_1(m_flags, kToleranceOverrideEnabled); m_tolOverride = tolOverride; }

    /** \details
        Disables sections generation tolerance override.
    */
    void resetSectionToleranceOverride() { SETBIT_0(m_flags, kToleranceOverrideEnabled); }

    /** \details
        Returns true if clipping of non-sectionable geometry enabled.
    */
    bool isNonSectionableGeometryClipping() const { return GETBIT(m_flags, kNonSectionableGeometryClip); }
    /** \details
        Enables or disables clipping of non-sectionable geometry.
        \param bSet [in]  New state of non-sectionable geometry clipping.
    */
    void setNonSectionableGeometryClipping(bool bSet) { SETBIT(m_flags, kNonSectionableGeometryClip, bSet); }

    /** \details
        Setup traits override flags.
        \param nFlags [in]  Traits override flags.
    */
    void setTraitsOverrideFlags(OdUInt32 nFlags)
    {
      m_overrideFlags = nFlags;
    }
    /** \details
        Returns traits override flags.
    */
    OdUInt32 traitsOverrideFlags() const
    {
      return m_overrideFlags;
    }

    /** \details
        Setup traits overrides.
        \param pData [in]  Traits overrides.
    */
    void setTraitsOverrides(const OdGiSubEntityTraitsData &pData)
    {
      m_overrides = pData;
    }
    /** \details
        Returns traits overrides (for changing).
    */
    OdGiSubEntityTraitsData &traitsOverrides()
    {
      return m_overrides;
    }
    /** \details
        Returns traits overrides.
    */
    const OdGiSubEntityTraitsData &traitsOverrides() const
    {
      return m_overrides;
    }

    /** \details
        Called to setup section geometry traits.
        \param context [in]  Geometry conveyor context.
        \remarks
        Override to implement own traits processing.
    */
    virtual void setupTraits(OdGiConveyorContext &context)
    {
      if (m_overrideFlags)
      {
        // Copy traits settings from current state
        OdGiSubEntityTraitsToData dataAdaptor(m_traits);
        OdGiSubEntityTraitsToData::copyTraits(context.subEntityTraits(), dataAdaptor, m_overrideFlags);
        // Setup traits overrides
        dataAdaptor.setTraits(m_overrides);
        OdGiSubEntityTraitsToData::copyTraits(dataAdaptor, context.subEntityTraits(), m_overrideFlags);
        context.onTraitsModified();
      }
    }

    /** \details
        Called to release section geometry traits.
        \param context [in]  Geometry conveyor context.
        \remarks
        Override to implement own traits processing.
    */
    virtual void releaseTraits(OdGiConveyorContext &context)
    {
      if (m_overrideFlags)
      {
        // Copy modified traits back to current state
        OdGiSubEntityTraitsToData dataAdaptor(m_traits);
        OdGiSubEntityTraitsToData::copyTraits(dataAdaptor, context.subEntityTraits(), m_overrideFlags);
        //context.onTraitsModified(); // Don't need since we completed with section geometry
      }
    }

    /** \details
        Called for opened sections.
        \remarks
        Redirects to destination geometry in default implementation.
    */
    void polylineProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal = 0,
                      const OdGeVector3d* pExtrusion = 0, OdGsMarker baseSubEntMarker = -1)
    {
      setupTraits(getInterface().conveyorContext());
      getInterface().destinationGeometry().polylineProc(numPoints, vertexList, pNormal, pExtrusion, baseSubEntMarker);
      releaseTraits(getInterface().conveyorContext());
    }

    /** \details
        Called for closed sections.
        \remarks
        Redirects to destination geometry in default implementation.
    */
    void shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList,
                   const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0)
    {
      setupTraits(getInterface().conveyorContext());
      if (!isOutputOfClosedSectionsAsPolylinesEnabled())
        getInterface().destinationGeometry().shellProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
      else
        shellProcAsPolylineProc(numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData);
      releaseTraits(getInterface().conveyorContext());
    }
  protected:
    void shellProcAsPolylineProc(OdInt32 /*numVertices*/, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList,
                                 const OdGiEdgeData* /*pEdgeData*/, const OdGiFaceData* pFaceData, const OdGiVertexData* /*pVertexData*/)
    {
      OdInt32 nFace = 0, nFaceList = 0;
      OdGeVector3d extrusion;
      const bool bExtrusion = OdNonZero(getInterface().conveyorContext().subEntityTraits().thickness());
      while (nFaceList < faceListSize)
      {
        const OdInt32 numVerts = Od_abs(faceList[nFaceList++]);
        const OdGeVector3d *pNormal = (pFaceData && pFaceData->normals()) ? (pFaceData->normals() + nFace) : NULL;
        if (pNormal && bExtrusion)
          extrusion = *pNormal * getInterface().conveyorContext().subEntityTraits().thickness();
        m_tmpPoints.resize(numVerts + 1);
        OdGePoint3d *tmpPoints = m_tmpPoints.asArrayPtr();
        for (OdInt32 nVert = 0; nVert < numVerts; nVert++)
          tmpPoints[nVert] = vertexList[faceList[nFaceList++]];
        tmpPoints[numVerts] = tmpPoints[0];
        getInterface().destinationGeometry().polylineProc(numVerts + 1, tmpPoints, pNormal, (pNormal && bExtrusion) ? &extrusion : NULL);
        nFace++;
      }
    }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiSectionGeometryOutput object pointers.
*/
typedef OdSmartPtr<OdGiSectionGeometryOutput> OdGiSectionGeometryOutputPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGISECTIONGEOMETRYOUTPUT_H__
