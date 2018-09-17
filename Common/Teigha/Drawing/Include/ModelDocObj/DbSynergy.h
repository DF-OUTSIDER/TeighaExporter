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

#ifndef OD_SYNERGY_H
#define OD_SYNERGY_H

#include "DbObject.h"
#include "ModelDocObjExports.h"

#include "TD_PackPush.h"

//namespace OdSynergyObj
//{
//  enum ViewEdgeLayerStyleType {};
//  enum ViewEdgeType {};
//  enum ViewRepZoneType {};
//}

//class MODELDOCOBJ_EXPORT OdDbSynergyLayerMapping : public OdDbObject
//{
//public:
//  ODDB_DECLARE_MEMBERS(OdDbSynergyLayerMapping);
//  OdDbSynergyLayerMapping();
//  virtual ~OdDbSynergyLayerMapping();
//
//  //void layerIds(OdArray<OdDbObjectId>&) const;
//  //void removeLayerId(enum OdSynergyObj::ViewEdgeLayerStyleType);
//  //void addLayerId(enum OdSynergyObj::ViewEdgeLayerStyleType, OdDbObjectId);
//  //bool layerId(enum OdSynergyObj::ViewEdgeLayerStyleType, OdDbObjectId&) const;
//
//protected:
//  // OdDbObject methods :
//  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
//  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
//  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
//  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
//};

//class MODELDOCOBJ_EXPORT OdDbIdwViewMapping : public OdDbObject
//{
//public:
//  ODDB_DECLARE_MEMBERS(OdDbIdwViewMapping);
//  OdDbIdwViewMapping();
//  virtual ~OdDbIdwViewMapping();
//
//  void setViewRepId(OdDbObjectId);
//  OdDbObjectId viewRepId() const;
//  void setViewportId(OdDbObjectId);
//  OdDbObjectId viewportId() const;
//  void setInventorViewId(OdUInt64);
//  OdUInt64 inventorViewId() const;
//  
//protected:
//  // OdDbObject methods :
//  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
//  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
//  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
//  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
//};

//class MODELDOCOBJ_EXPORT OdDbIdwSheetMapping : public OdDbObject
//{
//public:
//  ODDB_DECLARE_MEMBERS(OdDbIdwSheetMapping);
//  OdDbIdwSheetMapping();
//  virtual ~OdDbIdwSheetMapping();
//
//  //bool hasInventorModelDocViews() const;
//  //OdResult invalidIds(OdArray<OdUInt64>&) const;
//  //OdResult update(OdArray<OdUInt64>&);
//  //bool eraseViewMappingByViewRepId(OdDbObjectId);
//  //bool eraseViewMappingByViewportId(OdDbObjectId);
//  //bool eraseViewMappingByViewId(OdUInt64);
//  //OdDbIdwViewMapping* addIdwViewMapping(OdUInt64, OdDbObjectId, OdDbObjectId);
//  //const OdDbIdwViewMapping* idwViewMappingByViewRepId(OdDbObjectId) const;
//  //const OdDbIdwViewMapping* idwViewMappingByViewportId(OdDbObjectId) const;
//  //const OdDbIdwViewMapping* idwViewMappingByViewId(OdUInt64) const;
//  //OdDbIdwViewMapping* idwViewMappingByViewRepId(OdDbObjectId);
//  //OdDbIdwViewMapping* idwViewMappingByViewportId(OdDbObjectId);
//  //OdDbIdwViewMapping* idwViewMappingByViewId(OdUInt64);
//  //void setLayoutId(OdDbObjectId);
//  //OdDbObjectId layoutId() const;
//  //void setIdwSheetId(unsigned long);
//  //unsigned long idwSheetId() const;
//
//protected:
//  // OdDbObject methods :
//  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
//  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
//  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
//  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
//
////private: 
////  void eraseViewMappings();
//};

/** \details
    This class and its functions are not currently implemented.

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbSynergyMappings : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSynergyMappings);
  OdDbSynergyMappings();
  virtual ~OdDbSynergyMappings();

  //bool hasInventorModelDocViews(class OdDbDatabase *) const;
  //OdResult allLayoutIds(OdArray<OdDbObjectId>&) const;
  //OdResult invalidIds(OdArray<unsigned long>&, OdArray<OdUInt64>&) const;
  //const OdDbSynergyLayerMapping& layerMapping() const;
  //OdDbSynergyLayerMapping& layerMapping();
  //OdResult update(OdArray<unsigned long>&, OdArray<OdUInt64>&);
  //bool eraseAllIdwSheetMappings();
  //bool eraseIdwSheetMappingByLayoutId(OdDbObjectId);
  //bool eraseIdwSheetMappingBySheetId(unsigned long);
  //OdDbIdwSheetMapping* addIdwSheetMapping(unsigned long,OdDbObjectId);
  //const OdDbIdwSheetMapping* idwSheetMappingByLayoutId(OdDbObjectId) const;
  //const OdDbIdwSheetMapping* idwSheetMappingBySheetId(unsigned long) const;
  //OdDbIdwSheetMapping* idwSheetMappingByLayoutId(OdDbObjectId);
  //OdDbIdwSheetMapping* idwSheetMappingBySheetId(unsigned long);

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  // virtual subErase

//private:
//  void eraseIdwSheetMappings();
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSynergyMappings object pointers.
*/
typedef OdSmartPtr<OdDbSynergyMappings> OdDbSynergyMappingsPtr;

/** \details
    This class and its functions are not currently implemented.

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
//class MODELDOCOBJ_EXPORT OdDbSynergyBulletin : public OdDbObject
//{
//public:
//  ODDB_DECLARE_MEMBERS(OdDbSynergyBulletin);
//  OdDbSynergyBulletin();
//  virtual ~OdDbSynergyBulletin();
//
//protected:
//  // OdDbObject methods :
//  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
//  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
//  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
//  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
//};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSynergyBulletin object pointers.
*/
//typedef OdSmartPtr<OdDbSynergyBulletin> OdDbSynergyBulletinPtr;

#include "TD_PackPop.h"

#endif // OD_SYNERGY_H
