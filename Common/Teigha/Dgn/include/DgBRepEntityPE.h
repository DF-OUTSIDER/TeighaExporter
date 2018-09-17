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
#ifndef __DG_BREPENTITYPE_H__
#define __DG_BREPENTITYPE_H__

#include "TD_PackPush.h"

class OdDgCellHeader3d;
class OdStreamBuf;

/** \details
   
    Corresponding C++ library: DG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgBRepEntityPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS( OdDgBRepEntityPE );

  long        m_references;     //used by each object separately
public:

  OdDgBRepEntityPE();
  ~OdDgBRepEntityPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  // BRep support methods
  enum BRepEntityType
  {
    eNotBRepEntity = 0,
    eSmartSolid,
    eSmartSurface,
    eFeatureSolid
  };

  enum OdDgBRepEntityDataType
  {
    kUnknownData   = 0,
    kParasolidData = 1,
    kAcisData      = 2
  };

  virtual BRepEntityType type(const OdDgCellHeader3d *pCell) const;

  virtual OdDgBRepEntityDataType getDataType(const OdDgCellHeader3d *pCell) const; 

  /* output - stream version, see OdDgModelerGeometry class for available values. */
  virtual OdUInt32 brepVersion(const OdDgCellHeader3d *pCell) const;
  
  /* The output stream is binary data stored inside entity. Without any parsing\resaving.*/
  virtual OdResult brepOutNative(const OdDgCellHeader3d *pCell, OdStreamBuf &binData) const;

  /* The output stream is binary data returned by parser.*/
  virtual OdResult brepOut(const OdDgCellHeader3d *pCell, OdUInt32 nVer, OdStreamBuf &binData) const;

  // p3d must be DBRO entity.
  virtual void fillSmartSolid(OdDgCellHeader3d& p3d, OdStreamBuf &binData) const;
  void fillSmartSolid(OdDgCellHeader3d& p3d, OdStreamBuf &binData, bool bBrepIsAcis, double dScale = 0) const;

  // for internal use only
  virtual double getInternalScale(const OdDgCellHeader3d& p3d, double dScale) const; 

  // for Feature Solids only. Return true if extents of Brep cache is around extents of subelements.
  virtual bool isBrepCacheSynchronized(const OdDgCellHeader3d& p3d, const OdGeMatrix3d& mx, OdDgViewPtr& pView) const;
};

typedef OdSmartPtr<OdDgBRepEntityPE> OdDgBRepEntityPEPtr;


#include "TD_PackPop.h"

#endif // __DG_BREPENTITYPE_H__
