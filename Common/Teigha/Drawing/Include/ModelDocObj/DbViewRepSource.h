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

#ifndef OD_VIEWREPSOURCE_H
#define OD_VIEWREPSOURCE_H

#include "DbObject.h"
#include "ModelDocObjExports.h"
#include "DbViewRep.h"

#include "TD_PackPush.h"

/** \details

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewRepSource : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbViewRepSource);
  //OdDbViewRepSource();
  virtual ~OdDbViewRepSource();

  virtual enum OdDbViewRep::SourceType type() const;

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  //virtual subErase
  //virtual decomposeForSave
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewRepSource object pointers.
*/
typedef OdSmartPtr<OdDbViewRepSource> OdDbViewRepSourcePtr;

//class OdDbViewRepModelSpaceOccurrence;
//class OdDbViewRepModelSpaceBodyPath;
//class OdDbViewRepModelSpaceXRefFileRecord;

/** \details
    This class and its functions are not currently implemented.

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
//class MODELDOCOBJ_EXPORT OdDbViewRepModelSpaceBodyRecord : public OdDbObject
//{
//public:
//  ODDB_DECLARE_MEMBERS(OdDbViewRepModelSpaceBodyRecord);
//  //OdDbViewRepModelSpaceBodyRecord(const OdDbObjectId&, const struct _GUID&, const OdDbObjectId&);
//  //OdDbViewRepModelSpaceBodyRecord(const OdDbObjectId&, const struct _GUID&);
//  OdDbViewRepModelSpaceBodyRecord();
//  virtual ~OdDbViewRepModelSpaceBodyRecord();
//
//  //OdResult transfer2DWGBodyRecord(class aciv::DWGBodyRecord*) const;
//  //OdResult fixObjId();
//  //void setBoundingBox(const OdGePoint3d&, const OdGePoint3d&);
//  //void boundingBox(OdGePoint3d&, OdGePoint3d&) const;
//  //void setBlockDefObjId(const OdDbObjectId&);
//  //OdDbObjectId blockDefObjId() const;
//  //bool isXRef() const;
//  //void setBodyVerGuid(const struct _GUID&);
//  //struct _GUID& bodyVerGuid();
//  //const struct _GUID& bodyVerGuid() const;
//  //void setBodyHandle(const OdDbHandle&);
//  //OdDbHandle bodyHandle() const;
//  //void setBodyObjId(const OdDbObjectId&);
//  //OdDbObjectId bodyObjId() const;
//
//protected:
//  // OdDbObject methods :
//  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
//  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
//  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
//  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
//
////private: 
////  OdResult dwgInFieldsMigrate(OdDbDwgFiler*);
////  void dwgOutFieldsDowngrade(OdDbDwgFiler*) const;
////  OdResult dxfInFieldsMigrate(OdDbDxfFiler*);
////  void dxfOutFieldsDowngrade(OdDbDxfFiler*) const;
//};

/** \details
    This class and its functions are not currently implemented.

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
//class MODELDOCOBJ_EXPORT OdDbViewRepModelSpaceBodyOccurrenceSnapshot : public OdDbObject
//{
//public:
//  ODDB_DECLARE_MEMBERS(OdDbViewRepModelSpaceBodyOccurrenceSnapshot);
//  OdDbViewRepModelSpaceBodyOccurrenceSnapshot();
//  virtual ~OdDbViewRepModelSpaceBodyOccurrenceSnapshot();
//
//  //const OdDbViewRepModelSpaceOccurrence* findOccurrence(const OdDbViewRepModelSpaceBodyPath&,
//  //                                                      const OdDbObjectId&) const;
//  //OdResult getSubOccurrences(const OdArray<OdDbObjectId>&, 
//  //                           OdArray<OdDbViewRepModelSpaceOccurrence>&) const;
//  //OdResult fixObjId();
//  //bool isEqualTo(const OdDbViewRepModelSpaceBodyOccurrenceSnapshot&,
//  //               const OdArray<OdDbObjectId>&) const;
//  //bool isEqualTo(const OdDbViewRepModelSpaceBodyOccurrenceSnapshot&) const;
//  //bool isEqual(const OdArray<OdDbViewRepModelSpaceOccurrence>&,
//  //             const OdArray<OdDbViewRepModelSpaceBodyRecord>&,
//  //             const OdArray<OdDbViewRepModelSpaceXRefFileRecord>&,
//  //             const OdArray<OdDbViewRepModelSpaceOccurrence>&);
//  //unsigned short getVersionNum() const;
//
//protected:
//  // OdDbObject methods :
//  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
//  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
//  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
//  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
//};

/** \details
    This class and its functions are not currently implemented.

    Corresponding C++ library: AcModelDocObj

    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewRepFusionSource : public OdDbViewRepSource
{
public:
  ODDB_DECLARE_MEMBERS(OdDbViewRepFusionSource);
  OdDbViewRepFusionSource();
  virtual ~OdDbViewRepFusionSource();

  virtual enum OdDbViewRep::SourceType type() const;

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewRepFusionSource object pointers.
*/
typedef OdSmartPtr<OdDbViewRepFusionSource> OdDbViewRepFusionSourcePtr;

#include "TD_PackPop.h"

#endif // OD_VIEWREPSOURCE_H
