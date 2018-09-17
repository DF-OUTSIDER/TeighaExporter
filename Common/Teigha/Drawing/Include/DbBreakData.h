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

#ifndef _DbBreakData_h_Included_
#define _DbBreakData_h_Included_

#include "DbObject.h"
#include "OdArray.h"
#include "SmartPtr.h"
#include "DbDimAssoc.h"

class OdDbBreakPointRef;
typedef OdSmartPtr<OdDbBreakPointRef> OdDbBreakPointRefPtr;
typedef OdArray<OdDbBreakPointRefPtr> OdDbBreakPointRefPtrArray;

/** \details 
  This class represents break data for dimensions.

    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbBreakData : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBreakData);

  OdDbObjectId dimObjId() const;
  void setDimObjId(const OdDbObjectId& dimId);

  void addBreakPointRef(OdDbBreakPointRef* ptRef);
  void getBreakPointRef(int lineIndex, OdDbBreakPointRefPtrArray& ptRefs) const;
  void getBreakPointRef(OdDbBreakPointRefPtrArray& ptRefs) const;
  void deleteAllBreakPointRefs();
  OdResult removeAssociativity(bool force = true);
  //OdResult postToDb(OdDbObjectId& breakObjId);
  //OdResult eraseFromDb(); 

  virtual OdResult dwgInFields (OdDbDwgFiler*);
  virtual void dwgOutFields(OdDbDwgFiler*) const;
  virtual OdResult dxfInFields (OdDbDxfFiler*);
  virtual void dxfOutFields(OdDbDxfFiler*) const;
  virtual void modifiedGraphics(const OdDbEntity* pEnt);
  virtual void erased(const OdDbObject* pObj, bool erasing);
  virtual void modified(const OdDbObject* pObj);

  OdResult updateAssociativity(const OdDbObjectIdArray& ids, int cmdType);
};

typedef OdSmartPtr<OdDbBreakData> OdDbBreakDataPtr;

/** \details 
  This class represents break points for dimensions.

    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbBreakPointRef : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBreakPointRef);

  enum BreakPointType { 
    kDynamic           = 0,
    kStatic            = 1,
    kStatic2Point      = 2
  };

  OdDbBreakPointRef::BreakPointType pointType() const;
  void setPointType(OdDbBreakPointRef::BreakPointType newVal);
  OdInt32 lineIndex() const;
  void setLineIndex(OdInt32 index);
  OdGePoint3d breakPoint() const;
  void setBreakPoint(const OdGePoint3d& pt);
  OdGePoint3d breakPoint2() const;
  void setBreakPoint2(const OdGePoint3d& pt);
  void getBreakPointId(OdDbXrefFullSubentPath& idPath) const;
  void setBreakPointId(const OdDbXrefFullSubentPath& idPath);
  OdDbEntityPtr subentPtr() const;
  OdResult intersectWith(const OdDbEntity* pEnt, OdGePoint3dArray& points, OdGePoint3dArray& textBoxPts);

  virtual OdResult dwgInFields (OdDbDwgFiler*);
  virtual void dwgOutFields(OdDbDwgFiler*) const;
  virtual OdResult dxfInFields (OdDbDxfFiler*);
  virtual void dxfOutFields(OdDbDxfFiler*) const;
};

#endif

