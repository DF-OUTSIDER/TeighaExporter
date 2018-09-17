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
#ifndef __DG_APPLICATIONDATA_H__
#define __DG_APPLICATIONDATA_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "OdBinaryData.h"

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgApplicationData : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgApplicationData);
public:
  virtual OdUInt16 getSignature() const;
  virtual void setSignature(OdUInt16 type);

  virtual void getData(OdBinaryData& data) const;
  virtual void setData(const OdBinaryData& data);
};

typedef OdSmartPtr<OdDgApplicationData> OdDgApplicationDataPtr;

//================================================================================================

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgElementIdPath : public OdDgApplicationData
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgElementIdPath);
public:

  enum
  {
    kSignature = 0x56E6
  };

public:
  virtual OdUInt16 getSignature() const;
  virtual void setSignature(OdUInt16 type);

  virtual OdUInt32 getPathLength() const;

  virtual OdUInt64 getPathItem( OdUInt32 nItem ) const;
  virtual void     setPathItem( OdUInt64 uPathElementId, OdUInt32 nItem );
  virtual void     deletePathItem( OdUInt32 nItem );
  virtual void     addPathItem( OdUInt64 uPathElementId );
};

typedef OdSmartPtr<OdDgElementIdPath> OdDgElementIdPathPtr;

//================================================================================================

#include "TD_PackPop.h"

#endif // __DG_APPLICATIONDATA_H__
