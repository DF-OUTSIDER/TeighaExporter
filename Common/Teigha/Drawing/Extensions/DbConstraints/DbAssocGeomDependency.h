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

#ifndef DbAssocGeomDependency_INCLUDED_
#define DbAssocGeomDependency_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocDependency.h"
#include "DbAssocPersSubentId.h"
#include "Ge/GePoint3dArray.h"
#include "DbBlockReference.h"
#include "DbViewport.h"

class OdGeCurve3d;
class OdGeSurface;

#include "TD_PackPush.h"

enum PointPositionOnBlockLine
{
  kUndefined = 0,
  kStartPoint = 1,
  kEndPoint = 2,
  kOnLine = 3
};
/** \details
  This class implements a dependency on a subentity (faceedgevertex) of a geometric object. 
  It stores an subent ID which persistently identifies the subentity and provides a protocol 
  to get/set the referenced subentity geometry. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocGeomDependency : public OdDbAssocDependency
{
public: 
  ODRX_DECLARE_MEMBERS(OdDbAssocGeomDependency);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocGeomDependency(bool createImp = true);

  static OdSmartPtr<OdDbAssocGeomDependency> createObject( OdDbObjectId objId, OdDbSubentId subId );

  /** \details
    Returns a smart pointer to the internally held subent ID which identifies a subentity of 
    the dependent-on entity. The returned pointer refers to an internal data owned by this class.
  */
  OdDbAssocPersSubentIdPtr persistentSubentId() const;

  /** \details
    Returns the transient subent ID of the dependent-on entity corresponding to the internally 
    held associated subentity. 
  */
  OdResult getTransientSubentIds(OdArray<OdDbSubentId>& transientSubentIds) const;

  /** \details
    Sets the internally held associated subentity ID to persistently identify the given subentity. 
  */
  OdResult setSubentity(const OdDbSubentId& transientSubentId);

  /** \details
    Returns the type of the persistently referenced subentity. The method returns kNull if 
    no subentity is referenced. 
  */
  OdDb::SubentType subentType() const;

  /** \details
    Returns the number of transient subentity corresponding to the internally held associated subentity ID.
  */
  int transientSubentCount() const;

  /** \details
    Checks whether the dependency caches the referenced subentity geometry.
  */
  bool isCachingSubentityGeometry() const;

  /** \details
    Switches the dependency caching of the referenced subentity geometry.
  */
  void setCachingSubentityGeometry(bool bCaching);

  /** \details
    Gets the coordinates of vertex subentities corresponding to the internally held associated 
    subentity ID which must identify a vertex subentity type.
  */
  OdResult getVertexSubentityGeometry(OdGePoint3dArray& vertexPositions) const;

  OdResult getVertexSubentityGeometryFromPersSubentiy(OdGeCurve3d* & geometry, OdGePoint3dArray& vertexPositions) const;

  /** \details
    Gets the curves of edge subentities corresponding to the internally held associated 
    subentity ID which must identify an edge subentity type.
  */
  OdResult getEdgeSubentityGeometry(OdArray<OdGeCurve3d*>& edgeCurves) const;

  /** \details
    Gets the surfaces of face subentities corresponding to the internally held associated 
    subentity ID which must identify a face subentity type.
  */
  OdResult getFaceSubentityGeometry(OdArray<OdGeSurface*>& faceSurfaces) const;

  /** \details
    Sets the coordinates of vertex subentities corresponding to the internally held associated 
    subentity ID which must identify a vertex subentity type.
  */
  OdResult setVertexSubentityGeometry(const OdGePoint3dArray& newVertexPositions);

  /** \details
    Sets the curves of edge subentities corresponding to the internally held associated 
    subentity ID which must identify an edge subentity type.
  */
  OdResult setEdgeSubentityGeometry(const OdArray<const OdGeCurve3d*>& newEdgeCurves);

  /** \details
    Sets the surfaces of face subentities corresponding to the internally held associated 
    subentity ID which must identify a face subentity type.
  */
  OdResult setFaceSubentityGeometry(const OdArray<const OdGeSurface*>& newFaceSurfaces);

  /** \details
    This method is called from the client code to inform the dependency that the object, 
    whose subentity depends on it, has been mirrored.
  */
  OdResult dependentOnObjectMirrored();

  /** \details
    Gets all geometry dependencies on the old object that refers to the given old subentity 
    of old object and redirects them to refer to new subentity of new object.

    Returns True when the dependency holds the cached geometry of the referenced subentity.
  */
  static OdResult redirectToAnotherSubentity(const OdDbObjectId& oldObjectId,
                                             const OdDbSubentId& oldSubentId,
                                             const OdDbObjectId& newObjectId,
                                             const OdDbSubentId& newSubentId);
  /** \details
  Returns True when the SubEntity create successfully
  */
  OdResult createNewSingleEdgePersSubent(const OdDbObjectId lineID);
  OdResult createNewEdgePersSubent(OdDbEntityPtr pEntity, OdDbAssocActionPtr pAction, OdUInt32& subentIndex);// , bool bNewPersSubentIndex);
  OdResult createNewAssocSimplePersSubent(OdDbObjectId lineID, OdDb::SubentType type, PointPositionOnBlockLine isStartPoint);

  //void getViewPortOrBlRefFromFullSubentPath(OdDbViewportPtr& viewPort, OdDbBlockReferencePtr& blRef);
  //virtual void modifiedGraphics(const OdDbObject* pObject);
};
/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocGeomDependency class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocGeomDependency> OdDbAssocGeomDependencyPtr;

#include "TD_PackPop.h"

#endif
