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

#ifndef ODRXSKETCHOBJECT_H_
#define ODRXSKETCHOBJECT_H_

//#include "RxObject.h"

//class OdRxSketchManager;
#include "ModelDocObjExports.h"

/** <group OdModelDocObj_Classes>
*/
class MODELDOCOBJ_EXPORT OdRxSketchObject : public OdRxObject
{
protected:
  OdRxSketchObject();
public:
  ODRX_USING_HEAP_OPERATORS(OdRxObject);
  ODRX_DECLARE_MEMBERS(OdRxSketchObject);
  virtual ~OdRxSketchObject();

  //enum OdSySketchObjValidateStatus {}; 
  //OdResult restoreEntity() const;
  //OdDbObjectId getConstraintGroup() const;
  //OdResult exportGeometry(OdArray<OdGeCurve3d*>&) const;
  //OdResult getGeometry(OdArray<OdGeCurve3d*>&) const;
  //bool hasEntity(OdDbObjectId) const;
  //void addEntities(const OdArray<OdDbObjectId> &);
  //OdResult getEntities(OdArray<OdDbObjectId>&) const;
  //bool repair();
  //enum OdRxSketchObject::OdSySketchObjValidateStatus validate() const;
  //OdRxSketchManager* owningSketchManager();
  //const OdRxSketchManager* owningSketchManager() const;
  //void setConstraintGroupWorkplane(const OdGePlane&);
  //void transform(const OdGeMatrix3d&);
  //OdDbObjectId sketchBlkRefId() const;
  unsigned short sketchTag() const;

  OdDbObjectId getViewRepId() const;
  OdDbObjectId getSecSymbolId() const;

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

protected:
  //void deActivate();
  //void activate();
  //void cleanUp();
  //void setOwner(OdRxSketchManager*);

  class OdRxSketchObjectImpl* m_pImpObj;
};

/** <group OdModelDocObj_Classes>
*/
class MODELDOCOBJ_EXPORT OdRxFreeSketch : public OdRxSketchObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxFreeSketch);

  OdRxFreeSketch();
  virtual ~OdRxFreeSketch();

//protected:
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewRepSource object pointers.
*/
typedef OdSmartPtr<OdRxSketchObject> OdRxSketchObjectPtr;

/** <group OdModelDocObj_Classes>
*/
class MODELDOCOBJ_EXPORT OdRxSymbolSketch : public OdRxSketchObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxSymbolSketch);

  OdRxSymbolSketch();
  virtual ~OdRxSymbolSketch();

  //protected:
};

/** <group OdModelDocObj_Classes>
*/
class MODELDOCOBJ_EXPORT OdRxSectionSketch : public OdRxSketchObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxSectionSketch);

  OdRxSectionSketch();
  virtual ~OdRxSectionSketch();

  //protected:
};

/** <group OdModelDocObj_Classes>
*/
class MODELDOCOBJ_EXPORT OdRxDetailSketch : public OdRxSketchObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxDetailSketch);

  OdRxDetailSketch();
  virtual ~OdRxDetailSketch();

  //protected:
};

#endif // ODRXSKETCHOBJECT_H_
