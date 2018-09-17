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
#ifndef OdDbImpAssocGeomDependency_INCLUDED_
#define OdDbImpAssocGeomDependency_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbImpAssocDependency.h"
#include "DbAssocPersSubentId.h"
#include "DbAssocEdgePersSubentId.h"
#include "Ge/GePoint3dArray.h"
#include "DbAssocGeomDependency.h"

class OdGeCurve3d;
class OdGeSurface;

#include "TD_PackPush.h"

/** \details
  This class represents the custom implementation of the dependency on a subentity 
  (faceedgevertex) of a geometric object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocGeomDependency : public OdDbImpAssocDependency
{
public: 
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocGeomDependency();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocGeomDependency();

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
  OdInt32 transientSubentCount() const;

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

  /** \details
    Gets the curves of edge subentities corresponding to the internally held associated 
    subentity ID which must identify an edge subentity type.
  */
  OdResult getEdgeSubentityGeometry(OdArray<OdGeCurve3d*>& edgeCurves) const;
  
  /** \details
  Gets the coordinates of vertex subentities corresponding to the internally held associated
  subentity ID which must identify a vertex subentity type.
  */
  OdResult getVertexSubentityGeometryFromPersSubentiy(OdGeCurve3d* & geometry, OdGePoint3dArray& vertexPositions) const;

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
    Updates the object depends on the dependency, based on the information obtained from the owning 
    action.
  */
  virtual OdResult updateDependentOnObject(OdDbAssocDependency * pDep);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Notification callback for OdDbObject::erased(). This method is called when the dependency is erased.
  */
  virtual void erased(const OdDbObject* dbObj, OdDbAssocDependency *pDep, bool isErasing = true);

  /** \details
    Notification callback for OdDbObject::modified(). This method is called when the dependency is modified.
  */
  virtual void modified(const OdDbObject* dbObj, OdDbAssocDependency *pDep);

  /** \details
    Notification callback for OdDbObject::copied(). This method is called when the dependency is copied.
  */
  virtual void copied(const OdDbObject* pDbObj, const OdDbObject* pNewObj, OdDbAssocDependency *pDep);

  /** \details
    Sets the owning action for this dependency using the given object ID.
  */
  virtual OdResult setOwningAction(OdDbObjectId actionId, OdDbAssocDependency *pDep);

  /** \details
    Checks whether the changes are relevant to the depended-on object for this dependency type.
  */
  virtual bool isRelevantChange(const OdDbAssocDependency *pDep) const;

  /** \details
  Returns True when the SubEntity create successfully
  */
  OdResult createNewSingleEdgePersSubent(const OdDbObjectId lineID);
  OdResult createNewEdgePersSubent(OdDbEntityPtr pEntity, OdDbAssocActionPtr pAction, OdUInt32& subentIndex);// , bool bNewPersSubentIndex);
  virtual OdResult createNewAssocSimplePersSubent(OdDbObjectId lineID, OdDb::SubentType type, PointPositionOnBlockLine isStartPoint);
  
  virtual OdStringArray compareWith(const OdDbImpAssocDependency *depIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
protected:
  OdResult getBaseEdgeSubentityGeometry(OdArray<OdGeCurve3d*>& edgeCurves) const;
  /*!DOM*/
  OdDbAssocPersSubentIdPtr  m_pPersSubent;
  //OdDbAssocSingleEdgePersSubentIdPtr  m_pPersSubent;
  /*!DOM*/
  bool                      m_bCachingGeometry;
  /*!DOM*/
  OdGePoint3d m_subentPt;
  /*!DOM*/
  OdGeCurve3d *m_pSubentCurve;

  bool        m_bPolylineGeometry;

};

#include "TD_PackPop.h"

#endif
