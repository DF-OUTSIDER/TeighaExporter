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
#ifndef _TF_IO_H_DEFINED_
#define _TF_IO_H_DEFINED_

#include <OdStreamBuf.h>
#define STL_USING_MAP
#include <OdaSTL.h>

struct OdTfDigest
{
  char data[20];
  void write(OdStreamBuf* pBuf) const { pBuf->putBytes(data, 20); }
  void read(OdStreamBuf* pBuf) { pBuf->getBytes(data, 20); }
  bool operator < (const OdTfDigest& other) const
  {
    return memcmp(data, other.data, 20) < 0;
  }
  bool operator == (const OdTfDigest& other) const
  {
    return memcmp(data, other.data, 20) == 0;
  }
  bool operator != (const OdTfDigest& other) const
  {
    return memcmp(data, other.data, 20) != 0;
  }
  static OdTfDigest kNull() 
  {
    OdTfDigest res; 
    memset(res.data, 0, 20); 
    return res;
  }
  bool isNull() const
  {
    char test[20] = {0};
    return memcmp(test, data, 20) == 0;
  }
};
struct OdTfThreadContext {};
class OdDbDatabase;

class FIRSTDLL_EXPORT OdTfStorage : public OdRxObject {
public:
  ODRX_DECLARE_MEMBERS(OdTfStorage);
  virtual void open(const OdString& sPath) = 0;
  virtual OdString path() const = 0;
  virtual OdStreamBufPtr read(const OdTfDigest& sha1, OdTfThreadContext* ctx = 0) = 0;
  virtual bool getLabel(const OdString& label, OdTfDigest& sha1) = 0;
  virtual void write(OdStreamBuf* data, OdTfDigest& sha1) = 0;
  virtual bool setLabel(const OdString& label, const OdTfDigest& sha1) = 0;
  virtual OdTfThreadContext* createThreadContext() = 0;
  virtual void deleteThreadContext(OdTfThreadContext*) = 0;
  virtual void getExclusiveLock() = 0;
  virtual void releaseExclusiveLock() = 0;
  virtual void getBranchList(std::map<OdString, OdTfDigest>&) = 0;
  virtual OdStreamBufPtr getDiff(const OdTfDigest& sha1) = 0;
  virtual void writeDiff(const OdTfDigest& sha1, OdStreamBuf* data) = 0;
  virtual void deleteKey(const OdTfDigest& sha1) = 0;
  virtual void vacuum() = 0;
};
typedef OdSmartPtr<OdTfStorage> OdTfStoragePtr;


#endif // _TF_IO_H_DEFINED_
