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
#ifndef OdDbAssocPersSubentIdPE_INCLUDED_
#define OdDbAssocPersSubentIdPE_INCLUDED_ /*!DOM*/

#include "DbAssocPersSubentId.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeCurve3d.h"
#include "DbImpAssocGeomDependency.h"
#include "DbAssocActionBody.h"
#include "DbCompoundObjectId.h"
#include "DbBody.h"
#include <ModelerGeometry.h>

#include "TD_PackPush.h"

/*
struct OdDbAssocTransInfo
{
  enum TransType
  {
    kGeneral = 0,
    kScaling,
    kRadius,
    kMajorRadius,
    kMinorRadius,
    kFixedRadius,
    kFixed,
    kFixWhenPossible,
    kNone,
  };

  OdDbAssocTransInfo() : mRadius(-1.0), mTransType(kGeneral)
  {}
  TransType mTransType;

  OdGeMatrix3d mTrans;

  double mRadius;
};


struct OdDbAssocSubentTransInfo : OdDbAssocTransInfo
{
  OdDbSubentId mSubentId;
};
*/

/** \details
  Declares the rigidset type.
*/
enum RigidSetType
{
  kNotRigidSet = 0,
  kScalableRigidSet,
  kNonScalableRigidSet
};


/** \details
  This class provides the base protocol extension class that declares the protocol for 
  obtaining a persistent subentity identifier from a transient subentity identifier.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocPersSubentIdPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocPersSubentIdPE);
  OdDbAssocPersSubentIdPE() 
  { 
    int y = 0;
  }
  //OdDbAssocPersSubentIdPE() { m_assocGeomDependency = NULL; }

  /** \details
    Creates new persistent subentity identifier.
  */
  virtual OdDbAssocPersSubentIdPtr createNewPersSubent(OdDbEntity* pEntity,
                                                     const class OdDbCompoundObjectId& compId,
                                                     const OdDbSubentId& subentId)
  {
      OdDbAssocSimplePersSubentIdPtr res = OdDbAssocSimplePersSubentId::createObject();
      res->init(subentId);
      return res;
  }

  /** \details
    Returns the transient subent ID of the dependent-on entity corresponding to the internally 
    held associated subentity. 
  */
  virtual OdResult getTransientSubentIds(const OdDbEntity* pEntity, 
                                         const OdDbAssocPersSubentId* pPerSubentId,
                                         OdArray<OdDbSubentId>& subents) const = 0;

  /** \details
    Gets all subentity identifiers for a given subentity type.
  */
  virtual OdResult getAllSubentities(const OdDbEntity* pEntity,
                                     OdDb::SubentType subentType,
                                     OdArray<OdDbSubentId>& allSubentIds)
  {
      return eNotImplemented;
  }

  /** \details
    Gets the vertex subentity identifier corresponding to the given edge subentity identifier. 
    This protocol provides the query of relations between the edge and vertex subentities.
  */
  virtual OdResult getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                            const OdDbSubentId&    edgeSubentId,
                                            OdDbSubentId&          startVertexSubentId,
                                            OdDbSubentId&          endVertexSubentId,
                                            OdArray<OdDbSubentId>& otherVertexSubentIds)
  {
      return eNotImplemented;
  }


  /** \details
    Gets the vertex subentity identifier corresponding to the given spline subentity identifier. 
    This protocol provides the query of relations between the spline edge and vertex subentities.
  */
  virtual OdResult getSplineEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                                  const OdDbSubentId&    edgeSubentId,
                                                  OdDbSubentId&          startVertexSubentId,
                                                  OdDbSubentId&          endVertexSubentId,
                                                  OdArray<OdDbSubentId>& controlPointSubentIds,
                                                  OdArray<OdDbSubentId>& fitPointSubentIds)
  {
      return eNotImplemented;
  }

  /** \details
    Gets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  virtual OdResult getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                              const OdDbSubentId& vertexSubentId,
                                              OdGePoint3d&        vertexPosition)
  {
      return eNotImplemented;
  }

  /** \details
    Gets the curve of an edge subentity corresponding to the associated subentity ID.
  */
  virtual OdResult getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                            const OdDbSubentId& edgeSubentId,
                                            OdGeCurve3d*&       pEdgeCurve)
  {
      return eNotImplemented;
  }

  virtual OdResult getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
    const OdDbSubentId& edgeSubentId,
    const OdDbImpAssocGeomDependency* m_assocGeomDependency,
    OdGeCurve3d*&       pEdgeCurve 
    )
  {
    return getEdgeSubentityGeometry(pEntity, edgeSubentId, pEdgeCurve);
  }

  virtual OdResult addSubentitySpecialSettings(OdDbEntityPtr ent, const OdDbSubentId subentId, OdUInt32 idx, OdDbAssocActionBodyPtr pActionBody ) { return eOk; };
  /** \details
    Gets the surface of a face subentity corresponding to the associated subentity ID.
  */
  virtual OdResult getFaceSubentityGeometry(const OdDbEntity*   pEntity,
                                            const OdDbSubentId& faceSubentId,
                                            OdGeSurface*&       pFaceSurface)
  {
      return eNotImplemented;
  }

  /** \details
    Sets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  virtual OdResult setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                              const OdDbSubentId& vertexSubentId,
                                              const OdGePoint3d&  newVertexPosition)
  {
      return eNotImplemented;
  }

  /** \details
    Sets the curve of an edge subentity corresponding to the associated subentity ID.
  */
  virtual OdResult setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                            const OdDbSubentId& edgeSubentId,
                                            const OdGeCurve3d*  pNewEdgeCurve)
  {
      return eNotImplemented;
  }

  /** \details
    Sets the surface of a face subentity corresponding to the associated subentity ID.
  */
  virtual OdResult setFaceSubentityGeometry(OdDbEntity*         pEntity,
                                            const OdDbSubentId& faceSubentId,
                                            const OdGeSurface*  pNewFaceSurface)
  {
      return eNotImplemented;
  }

  /** \details
    Modifiers the geometry of a subentity.
  */
  virtual OdResult setSubentityGeometry(OdDbEntity*         pEntity,
                                        const OdDbSubentId& subentId,
                                        const OdDbEntity*   pNewSubentityGeometry)
  {
      return eNotImplemented;
  }

  ///These methods are deprecated and should not be used.
  /*
  virtual OdResult getSubentTransFromStretchPoints(const OdDbEntity*   pEntity,
                                                   const OdDbIntArray& indices, 
                                                   const OdGeVector3d& offset,
                                                   OdArray<OdDbAssocSubentTransInfo>& aSubentTransInfos) const
  {
    return eNotImplemented;
  }

  virtual OdResult getSubentTransFromGripPoints(const OdDbEntity*   pEntity,
                                                         const OdDbIntArray& indices, 
                                                         const OdGeVector3d& offset,
                                                         OdArray<OdDbAssocSubentTransInfo>& aSubentTransInfos) const
  {
    return eNotImplemented;
  }

  virtual OdResult getSubentTransFromGripPoints(const OdDbEntity*       pEntity,
                                                         const OdDbVoidPtrArray& gripAppData, 
                                                         const OdGeVector3d&     offset,
                                                         OdArray<OdDbAssocSubentTransInfo>& aSubentTransInfos) const
  {
    return eNotImplemented;
  }
  */

  /** \details
    Obtains a transformation matrix for a given subentity within the block reference.
  */
  virtual OdResult getSubentGeometryXform(const OdDbEntity*        pEntity,
                                          const OdDbObjectIdArray& fullSubentPath,
                                          OdGeMatrix3d&            trans) const
  {
      trans.setToIdentity();
      return eOk;
  }

  /** \details
    Mirrors the specified persistent subentity identifier. This method modifies the identifier 
    so that it identifies the same subentity in the mirrored entity as it identified before 
    the entity was mirrored.
  */
  virtual OdResult mirrorPersSubent(const OdDbEntity*      pMirroredEntity,
                                    OdDbAssocPersSubentId& persSubentIdToMirror)
  {
      return eOk;
  }

  /** \details
    If the entity is a rigid set, this method returns kScalableRigidSet or kNonScalableRigidSet type.

    \remarks
    Returns kNotRigidSet in default implementation.
  */
  virtual int getRigidSetState(const OdDbEntity* pEntity)
  {
    return kNotRigidSet;
  }

  /** \details
    Gets the transformation matrix of the rigid set entity.
  */
  virtual OdResult getRigidSetTransform(const OdDbEntity* pEntity,
                                        OdGeMatrix3d&     trans)
  {
      return eNotApplicable;
  }

  /** \details
    Gets the transformation matrix for the rigid set entity.
  */
  virtual OdResult setRigidSetTransform(OdDbEntity*         pEntity, 
                                        const OdGeMatrix3d& trans)
  {
      return eNotApplicable;
  }

  virtual OdModelerGeometryPtr getModelerGeometry(const OdDbEntity* pEnt)
  {
    OdModelerGeometryPtr pRes;
    if (OdDb3dSolid* pSolid = OdDb3dSolid::cast(pEnt))
      pRes = (OdRxObject*)pSolid->body();
    else if (OdDbRegion* pReg = OdDbRegion::cast(pEnt))
      pRes = (OdRxObject*)pReg->body();
    else if (OdDbBody* pBody = OdDbBody::cast(pEnt))
      pRes = (OdRxObject*)pBody->body();
    else if (OdDbSurface* pSrf = OdDbSurface::cast(pEnt))
      pRes = (OdRxObject*)pSrf->body();
    return pRes;
  }
  //OdDbImpAssocGeomDependency* assocGeomDependency() const
  //{
  //  OdDbImpAssocGeomDependency* ret = m_assocGeomDependency;
  //  return ret;
  //}

//  void setAssocGeomDependency(const OdDbImpAssocGeomDependency* assocGeomDependency)
//  {
//    m_assocGeomDependency = (OdDbImpAssocGeomDependency*)assocGeomDependency;
//  }
//protected:
//
//  OdDbImpAssocGeomDependency* m_assocGeomDependency;
};

typedef OdSmartPtr<OdDbAssocPersSubentIdPE> OdDbAssocPersSubentIdPEPtr;


#include "TD_PackPop.h"
#endif
