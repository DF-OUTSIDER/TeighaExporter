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

#ifndef _OD_DBNAMEDPATH_H_INCLUDED_
#define _OD_DBNAMEDPATH_H_INCLUDED_

#include "DbObject.h"

#include "TD_PackPush.h"

class OdGeCurve3d;

/** \details
    Corresponding C++ library: SCENEOE

    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbNamedPath : public OdDbObject
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbNamedPath);

    virtual ~OdDbNamedPath();

    virtual OdResult getGeometry(OdGeCurve3d *& pCurve) const = 0;
  private:
    OdDbNamedPath(); // noimpl
};

typedef OdSmartPtr<OdDbNamedPath> OdDbNamedPathPtr;

/** \details

    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbCurvePath : public OdDbNamedPath
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbCurvePath);

    OdDbCurvePath();
    virtual ~OdDbCurvePath();

    virtual OdResult getGeometry(OdGeCurve3d *& pCurve) const;

    void setGeometry(const OdDbObjectId &curveId);
    OdDbObjectId entityId() const;

    virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
    virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
    virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
    virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbCurvePath> OdDbCurvePathPtr;

/** \details

    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbPointPath : public OdDbNamedPath
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbPointPath);

    OdDbPointPath();
    virtual ~OdDbPointPath();

    virtual OdResult getGeometry(OdGeCurve3d *& pCurve) const;

    void setGeometry(const OdGePoint3d &point);
    OdGePoint3d point() const;

    virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
    virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
    virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
    virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbPointPath> OdDbPointPathPtr;

SCENEOE_EXPORT OdDbObjectId oddbGetNamedPathDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
SCENEOE_EXPORT OdDbDictionaryPtr oddbGetNamedPathDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);

#include "TD_PackPop.h"

#endif // _OD_DBNAMEDPATH_H_INCLUDED_
