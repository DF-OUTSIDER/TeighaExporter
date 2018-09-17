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

#ifndef ODDB_DBSHCYLINDER_H
#define ODDB_DBSHCYLINDER_H

#include "TD_PackPush.h"
#include "DbShPrimitive.h"

/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbShCylinder : public OdDbShPrimitive
{
public:
  ODDB_DECLARE_MEMBERS(OdDbShCylinder);
  OdDbShCylinder();

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);  
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;  
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);  
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual double height() const;
  virtual double xRadius() const;
  virtual double yRadius() const;
  virtual double majorRadius() const;
  virtual double minorRadius() const;
  virtual OdResult setHeight(double dHeight);
  virtual OdResult setXRadius(double dXRadius);
  virtual OdResult setYRadius(double dXRadius);
  virtual OdResult setMajorRadius(double dMajorRadius);
  virtual OdResult setMinorRadius(double dMinorRadius);
  virtual bool isCylindrical() const;
  virtual bool isElliptical() const;
};


/** \details

    \sa
    TD_Db
    <group OdDbSh_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbShCone : public OdDbShPrimitive
{
public:
  ODDB_DECLARE_MEMBERS(OdDbShCone);
  OdDbShCone();

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  virtual double baseRadius() const;
  virtual double topMinorRadius() const;
  virtual double topMajorRadius() const;
  virtual double topXRadius() const;
  virtual OdResult setBaseRadius(double dHeight);
  virtual OdResult setTopMinorRadius(double dMinorRadius);
  virtual OdResult setTopMajorRadius(double dMajorRadius);
  virtual OdResult setTopXRadius(double dTopMinorRad);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbShCylinder object pointers.
*/
typedef OdSmartPtr<OdDbShCylinder> OdDbShCylinderPtr;
typedef OdSmartPtr<OdDbShCone> OdDbShConePtr;
#include "TD_PackPop.h"

#endif

