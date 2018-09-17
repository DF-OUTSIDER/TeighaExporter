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

#ifndef _ODDG_MODELERGEOMETRY_INCLUDED_
#define _ODDG_MODELERGEOMETRY_INCLUDED_

#include "DgExport.h"
#include "RxObject.h"

class OdStreamBuf;
class OdBrBrep;
class OdGiCommonDraw;
class OdGeMatrix3d;

#include "TD_PackPush.h"


/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgModelerGeometry : public OdRxObject
{
public:
  enum
  {
    kUndefined     = 0x00000000,

    kTypeMask      = 0xff000000,
    kAcisMask      = 0xf0000000,
    kParasolidMask = 0x0f000000,
    kSat           = 0x10000000,
    kSab           = 0x20000000,
    kSatIndexed    = 0x30000000,
    kSabIndexed    = 0x40000000,
    kXT            = 0x01000000,
    kXB_Bare       = 0x02000000,
    kXB_Typed0     = 0x03000000,
    kXB_Typed1     = 0x04000000,
    kXB_Neutral    = 0x05000000,

    kVersionMask   = 0x0000ffff,
  //kPS_7_0        = 0x00000001,
  //kPS_7_1        = 0x00000002,
  //kPS_8_0        = 0x00000003,
    kPS_9_0        = 0x00000004,
  //kPS_9_1        = 0x00000005,
  //kPS_10_0        = 0x00000006,
  //kPS_10_1        = 0x00000007,
  //kPS_11_0        = 0x00000008,
  //kPS_11_1        = 0x00000009,
    kPS_12_0        = 0x0000000A,
  //kPS_12_1        = 0x0000000B,
  //kPS_13_0        = 0x0000000C,
  //kPS_13_1        = 0x0000000D,
  //kPS_14_0        = 0x0000000E,
  //kPS_14_1        = 0x0000000F,
  //kPS_15_0        = 0x00000010,
  //kPS_16_0        = 0x00000011,

    kAS_1_5         = 0x00000100,
    kAS_1_6         = 0x00000200,
    kAS_1_7         = 0x00000300,
    kAS_2_1         = 0x00000400,
    kAS_3_0         = 0x00000500,
    kAS_4_0         = 0x00000600,
    kAS_5_0         = 0x00000700,
  //kAS_5_2         = 0x00000800,
    kAS_6_0         = 0x00000900,
    kAS_7_0         = 0x00000A00,
    kAS_8_0         = 0x00000B00
  //kAS_9_0         = 0x00000C00,
  //kAS_10_0        = 0x00000D00,
  //kAS_11_0        = 0x00000E00,
  //kAS_12_0        = 0x00000F00,
  //kAS_13_0        = 0x00001000
  };
  typedef OdUInt32 OdDgModelerVersion;

protected:
  OdDgModelerGeometry() {}
public:  
  ODRX_DECLARE_MEMBERS(OdDgModelerGeometry);

  virtual OdResult in(OdStreamBuf* pStreamBuf) = 0;

  virtual OdResult comeFromVersion(OdDgModelerVersion &ver) const = 0;

  virtual OdResult out(OdStreamBuf* pStreamBuf, const OdDgModelerVersion& typeVer) const = 0;

  virtual OdResult brep(OdBrBrep& brBrep) const = 0;

  virtual OdResult render(OdGiCommonDraw* pWd) = 0;

  virtual OdResult getTransformation(OdGeMatrix3d& xfm) = 0;

  virtual OdResult transformBy( const OdGeMatrix3d& xfm ) = 0;

  virtual bool isEmpty( ) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgModelerGeometry object pointers.
*/
typedef OdSmartPtr<OdDgModelerGeometry> OdDgModelerGeometryPtr;

#include "TD_PackPop.h"

#endif // _ODDG_MODELERGEOMETRY_INCLUDED_
