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
#ifndef __DG_BREPBUILDERINITIALDATA_H__
#define __DG_BREPBUILDERINITIALDATA_H__

#include "TD_PackPush.h"

#include "DgGraphicsElement.h"
#include "DgBSplineSurface.h"
#include "DgCone.h"
#include "BrepBuilderInitialData.h"

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineSurfaceBrepBuilderInitialDataPE : public OdRxObject
{
   ODRX_DECLARE_MEMBERS(OdDgBSplineSurfaceBrepBuilderInitialDataPE);
  long        m_references;     //used by each object separately
public:

  OdDgBSplineSurfaceBrepBuilderInitialDataPE();
  ~OdDgBSplineSurfaceBrepBuilderInitialDataPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  /** \details
    Allows to get initial data to construct brep by brep builder.
  */

  virtual bool getBrepBuilderInitialData(const OdDgBSplineSurface* obj, BrepBuilderInitialData& brepData, double& dScale ) const;

};

typedef OdSmartPtr<OdDgBSplineSurfaceBrepBuilderInitialDataPE> OdDgBSplineSurfaceBrepBuilderInitialDataPEPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgConeBrepBuilderInitialDataPE : public OdRxObject
{
   ODRX_DECLARE_MEMBERS(OdDgConeBrepBuilderInitialDataPE);
  long        m_references;     //used by each object separately
public:

  OdDgConeBrepBuilderInitialDataPE();
  ~OdDgConeBrepBuilderInitialDataPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  /** \details
    Allows to get initial data to construct brep by brep builder.
  */

  virtual bool getBrepBuilderInitialData(const OdDgCone* obj, BrepBuilderInitialData& brepData, double& dScale ) const;

};

typedef OdSmartPtr<OdDgConeBrepBuilderInitialDataPE> OdDgConeBrepBuilderInitialDataPEPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDg3dSolidBrepBuilderInitialDataPE : public OdRxObject
{
   ODRX_DECLARE_MEMBERS(OdDg3dSolidBrepBuilderInitialDataPE);
  long        m_references;     //used by each object separately
public:

  enum OdDg3dSolidBaseType
  {
    kExtrudeBased = 0,
    kRevolveBased = 1,
    kExtrudeAndRevolveBased = 2
  };
public:

  OdDg3dSolidBrepBuilderInitialDataPE();
  ~OdDg3dSolidBrepBuilderInitialDataPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  /** \details
    Allows to get initial data to construct brep by brep builder.
  */

  virtual bool getBrepBuilderInitialData( const OdDgElement* obj, BrepBuilderInitialData& brepData, double& dScale ) const;

  virtual OdDg3dSolidBaseType getDgnSolidBaseType( const OdDgElement* obj );

};

typedef OdSmartPtr<OdDg3dSolidBrepBuilderInitialDataPE> OdDg3dSolidBrepBuilderInitialDataPEPtr;
#include "TD_PackPop.h"

#endif // __DG_BREPBUILDERINITIALDATA_H__
