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
#ifndef OdDbAssocSplinePersSubentIdPE_INCLUDED_
#define OdDbAssocSplinePersSubentIdPE_INCLUDED_  /*!DOM*/

#include "OdDbAssocCurvePersSubentIdPE.h"

#include "TD_PackPush.h"

/** \details
  This class provides the base protocol extension class that declares the protocol for 
  working with spline persistent subentity identifier.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocSplinePersSubentIdPE : public OdDbAssocCurvePersSubentIdPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocSplinePersSubentIdPE);

  /** \details
    Creates new persistent subentity identifier.
  */
  virtual OdDbAssocPersSubentIdPtr createNewPersSubent(OdDbEntity* pEntity,
                                                     const class OdDbCompoundObjectId& compId,
                                                     const OdDbSubentId& subentId);

  /** \details
    Gets all subentity identifiers for a given subentity type.
  */
  virtual OdResult getAllSubentities(const OdDbEntity* pEntity,
                                     OdDb::SubentType subentType,
                                     OdArray<OdDbSubentId>& allSubentIds);

  /** \details
    Gets the vertex subentity identifier corresponding to the given edge subentity identifier. 
    This protocol provides the query of relations between the edge and vertex subentities.
  */
  virtual OdResult getEdgeVertexSubentities(const OdDbEntity*      pEntity,
                                            const OdDbSubentId&    edgeSubentId,
                                            OdDbSubentId&          startVertexSubentId,
                                            OdDbSubentId&          endVertexSubentId,
                                            OdArray<OdDbSubentId>& otherVertexSubentIds);

  /** \details
    Gets the curve of an edge subentity corresponding to the associated subentity ID.
  */
  virtual OdResult getEdgeSubentityGeometry(const OdDbEntity*   pEntity,
                                            const OdDbSubentId& edgeSubentId,
                                            OdGeCurve3d*&       pEdgeCurve);

  /** \details
    Sets the curve of an edge subentity corresponding to the associated subentity ID.
  */
  virtual OdResult setEdgeSubentityGeometry(OdDbEntity*         pEntity,
                                            const OdDbSubentId& edgeSubentId,
                                            const OdGeCurve3d*  pNewEdgeCurve);

  /** \details
    Gets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  virtual OdResult getVertexSubentityGeometry(const OdDbEntity*   pEntity,
                                              const OdDbSubentId& vertexSubentId,
                                              OdGePoint3d&        vertexPosition);

  /** \details
    Sets the coordinates of a vertex subentity corresponding to the associated subentity ID.
  */
  virtual OdResult setVertexSubentityGeometry(OdDbEntity*         pEntity,
                                              const OdDbSubentId& vertexSubentId,
                                              const OdGePoint3d&  newVertexPosition);

  /** \details
    Modifiers the geometry of a subentity.
  */
  virtual OdResult setSubentityGeometry(OdDbEntity*         pEntity,
                                        const OdDbSubentId& subentId,
                                        const OdDbEntity*   pNewSubentityGeometry);

protected:
  /** \details
    Fills the start vertex subentity identifiers.
  */
  virtual void fillStartVertexSubentId(const OdDbEntity*      pEntity,
                                     const OdDbSubentId&    edgeSubentId,
                                     OdDbSubentId& startVertexSubentId);

  /** \details
    Fills the end vertex subentity identifiers.
  */
  virtual void fillEndVertexSubentId(const OdDbEntity*      pEntity,
                                   const OdDbSubentId&    edgeSubentId,
                                   OdDbSubentId& startVertexSubentId);

  /** \details
    Gets other vertex subentity identifiers.
  */
  virtual void getOtherVertexSubentIds(const OdDbEntity*      pEntity,
                                       const OdDbSubentId&    edgeSubentId,
                                       OdArray<OdDbSubentId>& otherVertexSubentIds);
private:
  /** \details
    Checks whether subentity identifier for a given type.
  */
  bool checkSubentId(const OdDbEntity* pEntity, const OdDbSubentId& subentId, const OdDb::SubentType type) const;
};

#include "TD_PackPop.h"
#endif
