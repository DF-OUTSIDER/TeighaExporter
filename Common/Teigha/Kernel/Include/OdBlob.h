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

// OdBlob.h: interface for the OdBlob class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ODBLOB_H_INCLUDED_
#define _ODBLOB_H_INCLUDED_

#include "MemoryStreamImpl.h"
#include "StaticRxObject.h"
class OdDbStub;

/** \details
  <group OdDbRoot_Classes>
  Corresponding C++ library: TD_Root
*/
class OdBlob : public OdStaticRxObject<OdMemoryStreamImpl<OdMemoryStream> >
{
public:
  inline OdBlob(OdUInt32 nPageSize = 0x1000);

  inline bool rdBool();
  inline void wrBool(bool);
  inline OdInt8 rdInt8();
  inline void    wrInt8(OdInt8);
  inline OdInt16 rdInt16();
  inline void    wrInt16(OdInt16);
  inline OdInt32 rdInt32();
  inline void    wrInt32(OdInt32);
  inline OdInt64 rdInt64();
  inline void    wrInt64(OdInt64);
  inline double  rdDouble();
  inline void    wrDouble(double);
  inline OdGePoint3d rdPoint3d();
  inline void    wrPoint3d(const OdGePoint3d&);
  inline OdGeVector3d rdVector3d();
  inline void    wrVector3d(const OdGeVector3d&);
  inline void*   rdAddress();
  inline void    wrAddress(const void*);

  inline void rdBytes(void* buf, OdUInt32 len);
  inline void wrBytes(const void* buf, OdUInt32 len);

  inline void rdPoints2d(OdGePoint2d* points, OdUInt32 num);
  inline void wrPoints2d(const OdGePoint2d* points, OdUInt32 num);

  inline void rdPoints3d(OdGePoint3d* points, OdUInt32 num);
  inline void wrPoints3d(const OdGePoint3d* points, OdUInt32 num);

  inline void wrDoubles(const double* doubles, OdUInt32 num);
  inline void rdDoubles(double* doubles, OdUInt32 num);

  inline void rdObjectIds(OdDbStub** pRes, OdUInt32 num);
  inline OdDbStub* rdObjectId();
  inline void wrObjectId(const OdDbStub* id);
};

inline OdBlob::OdBlob(OdUInt32 nPageSize)
{
  setPageDataSize(nPageSize);
}

#define BLOB_DATA(type, name) \
inline type OdBlob::rd##name() \
{\
  type val;\
  getBytes(&val, sizeof(type));\
  return val;\
}\
inline void OdBlob::wr##name(type val)\
{  putBytes(&val, sizeof(type));}

BLOB_DATA(bool, Bool)
BLOB_DATA(OdInt8, Int8)
BLOB_DATA(OdInt16, Int16)
BLOB_DATA(OdInt32, Int32)
BLOB_DATA(OdInt64, Int64)
BLOB_DATA(double, Double)

inline OdGePoint3d OdBlob::rdPoint3d() 
{
  OdGePoint3d val;
  getBytes(&val, sizeof(val));
  return val;
}

inline void OdBlob::wrPoint3d(const OdGePoint3d& val)
{  putBytes(&val, sizeof(OdGePoint3d));}

inline OdGeVector3d OdBlob::rdVector3d() 
{
  OdGeVector3d val;
  getBytes(&val, sizeof(val));
  return val;
}

inline void OdBlob::wrVector3d(const OdGeVector3d& val)
{  putBytes(&val, sizeof(OdGeVector3d));}


inline void* OdBlob::rdAddress() 
{
  void* val;
  getBytes(&val, sizeof(void*));
  return val;
}

inline void OdBlob::wrAddress(const void* val)
{  putBytes(&val, sizeof(void*));}



inline void OdBlob::rdBytes(void* buf, OdUInt32 len)
{
  getBytes(buf, len);
}

inline void OdBlob::wrBytes(const void* buf, OdUInt32 len)
{
  putBytes(buf, len);
}

inline void OdBlob::rdPoints3d(OdGePoint3d* points, OdUInt32 num)
{
  getBytes(points, num * sizeof(OdGePoint3d));
}

inline void OdBlob::wrPoints3d(const OdGePoint3d* points, OdUInt32 num)
{
  putBytes(points, num * sizeof(OdGePoint3d));
}

inline void OdBlob::rdPoints2d(OdGePoint2d* points, OdUInt32 num)
{
  getBytes(points, num * sizeof(OdGePoint2d));
}

inline void OdBlob::wrPoints2d(const OdGePoint2d* points, OdUInt32 num)
{
  putBytes(points, num * sizeof(OdGePoint2d));
}

inline void OdBlob::wrDoubles(const double* doubles, OdUInt32 num)
{
  putBytes(doubles, num * sizeof(double));
}

inline void OdBlob::rdDoubles(double* doubles, OdUInt32 num)
{
  getBytes(doubles, num * sizeof(double));
}

inline void OdBlob::rdObjectIds(OdDbStub** pRes, OdUInt32 num)
{
  getBytes(pRes, num * sizeof(OdDbStub*));
}

inline OdDbStub* OdBlob::rdObjectId()
{
  OdDbStub* id;
  getBytes(&id, sizeof(id));
  return id;
}

inline void OdBlob::wrObjectId(const OdDbStub* id)
{
  putBytes(&id, sizeof(OdDbStub*));
}

#endif // #ifndef _ODBLOB_H_INCLUDED_
