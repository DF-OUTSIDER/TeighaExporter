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

#ifndef OD_ASSOCPROJECTEDENTITYPERSSUBENTIDHOLDER_H
#define OD_ASSOCPROJECTEDENTITYPERSSUBENTIDHOLDER_H

#include "ModelDocObjExports.h"
#include "DbEntity.h"

#include "TD_PackPush.h"

/** \details

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbAssocProjectedEntityPersSubentIdHolder : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS(OdDbAssocProjectedEntityPersSubentIdHolder);

  OdDbAssocProjectedEntityPersSubentIdHolder();
  virtual ~OdDbAssocProjectedEntityPersSubentIdHolder();

  //virtual void copied(const OdDbObject*,const OdDbObject*);
  //virtual void modified(const OdDbObject*);
  //virtual OdResult getPlane(OdGePlane& gePlane, OdDb::Planarity& gePlanarity) const;
  //virtual OdResult subErase(int);
  //virtual OdResult getSubentityGeometry(const OdDbSubentId&,OdGePoint3d &,OdGeCurve3d const * &,OdGeSurface const * &) const;
  //virtual OdResult getEdgeVertexSubentities(const OdDbSubentId&,OdDbSubentId&,OdDbSubentId&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&);
  //virtual OdResult getEdgeVertexSubentities(const OdDbSubentId&,OdDbSubentId&,OdDbSubentId&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&);
  //virtual OdResult getAllSubentities(unsigned long,OdArray<OdDbSubentId>&);
  //virtual OdResult getAllSubentities(const OdDbSourceEdgeRef&,unsigned long,OdArray<OdDbSubentId>&);
  //virtual OdResult getTransientSubentIds(const OdDbAssocIndexPersSubentId&,OdArray<OdDbSubentId>&) const;
  //OdResult releasePersSubent(const OdDbAssocIndexPersSubentId&);
  //virtual OdDbAssocIndexPersSubentId createNewPersSubent(const OdDbSubentId&);
  //virtual OdResult getSourceSubentity3d(const OdDbSubentId&,OdDbSubentRef*&,int*);
  //virtual OdResult syncUpWithSourceEntity3d(bool);
  //virtual bool isInSyncWithSourceEntity3d(int) const;
  //virtual void enableSourceEntity3dChangeTracking(bool,int);
  //virtual bool isTrackingSourceEntity3dChanges(int) const;
  //virtual OdResult setTransform(OdGeMatrix3d const &);
  //virtual OdGeMatrix3d const & transform() const;
  //virtual OdResult setTransform3d(OdGeMatrix3d const &);
  //virtual OdGeMatrix3d const & transform3d() const;
  //virtual OdResult setProjectionProvider(const OdDbObjectId&);
  //virtual OdDbObjectId projectionProvider() const;
  //OdResult setCurrentSourceId(int);
  //virtual OdResult fixupSourceEntity3d(const OdDbCompoundObjectId&,int);
  //int currentSourceId() const;
  //OdArray<int> sourceIds() const;
  //virtual OdResult setSourceEntity3d(const OdDbCompoundObjectId&,int);
  //virtual const OdDbCompoundObjectId& sourceEntity3d(bool,int) const;
  //virtual OdResult resumeAndSyncUpWithSourceEntity3d();
  //virtual OdResult visit(const OdDbAssocProjectedEntityPersSubentIdVisitor*);
  //virtual bool suspendPersIdNodes(OdDbAssocPersSubentIdSuspendCallback*);
  //virtual void removeAllTransientSubentIds();
  //virtual void reset(bool);
  //virtual bool contains(const OdDbSourceEdgeRef&) const;
  //virtual bool isEmpty() const;
  //virtual OdResult getEdgeVertexSubentities(const OdDbSubentId&,OdDbSubentId&,OdDbSubentId&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&);
  //virtual OdResult getEdgeVertexSubentities(const OdDbSubentId&,OdDbSubentId&,OdDbSubentId&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&);
  //virtual OdResult getEdgeVertexSubentities(const OdDbSubentId&,OdDbSubentId&,OdDbSubentId&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&,OdArray<OdDbSubentId>&);

  //static const int kCurrentSourceId;
  ////static void attachPEs();
  //static void uninit();
  //static void init();

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  //virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  //virtual void subViewportDraw(OdGiViewportDraw* pVd) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbAssocProjectedEntityPersSubentIdHolder object pointers.
*/
typedef OdSmartPtr<OdDbAssocProjectedEntityPersSubentIdHolder> OdDbAssocProjectedEntityPersSubentIdHolderPtr;

#include "TD_PackPop.h"

#endif // OD_ASSOCPROJECTEDENTITYPERSSUBENTIDHOLDER_H
