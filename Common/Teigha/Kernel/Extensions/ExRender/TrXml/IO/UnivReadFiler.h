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
//
// UnivReadFiler.h
//

#ifndef __OD_UNIVERSAL_READ_FILER_H__
#define __OD_UNIVERSAL_READ_FILER_H__

#include "TD_PackPush.h"

#include "OdStreamBuf.h"

/** <group ExRender_Classes>
*/
class OdUniversalReadFiler
{
protected:
  mutable OdStreamBufPtr m_pStream;
  OdUInt32 m_nVersion;
  bool m_bBigEndian;

  void assertRd() const;

public:
  OdUniversalReadFiler(const OdString& sFilePathName, 
                        OdUInt32 nVersion = 100,
                        const char* pMagic = "TGS");
  OdUniversalReadFiler(OdStreamBuf* pStream = NULL, 
                        OdUInt32 nVersion = 100,
                        const char* pMagic = "TGS");
  virtual ~OdUniversalReadFiler();

  // Attach stream and initiate read process
  bool setStream(OdStreamBuf* pStream, 
                 OdUInt32 nVersion = 100,
                 const char* pMagic = "TGS");
  OdStreamBufPtr stream() const;
  OdUInt32 version() const;

  bool isEof() const;

  bool isBigEndian() const;
  void setBigEndian(bool bValue);
  void setBigEndian(); // auto

  // Process raw data
  void rdRawData(void* pData, OdUInt32 nDataSize) const;
  void rd2BytesNumber(void* pData) const;
  void rd4BytesNumber(void* pData) const;
  void rd4Bytes(void *pData) const;
  void rd8Bytes(void* pData) const;

  // Process handles
  //class OdDbStub *rdHandle() const;

  // Process data primitives
  bool rdBool() const;
  int rdInt() const;
  //unsigned int rdUInt() const;
  //char rdChar() const;
  OdUInt8 rdUInt8() const;
  OdInt16 rdInt16() const;
  OdUInt16 rdUInt16() const;
  OdInt32 rdInt32() const;
  OdUInt32 rdUInt32() const;
  OdInt64 rdInt64() const;
  OdUInt64 rdUInt64() const;
  //ODCOLORREF rdCOLORREF() const;
  float rdFloat() const;
  double rdDouble() const;
  void rdMatrix3d(class OdGeMatrix3d& mat) const;
  void rdAnsiString(OdAnsiString& str) const;
};

#include "TD_PackPop.h"

#endif // __OD_UNIVERSAL_READ_FILER_H__
