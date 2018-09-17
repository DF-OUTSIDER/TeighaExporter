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
// UnivWriteFiler.h
//

#ifndef __OD_UNIVERSAL_WRITE_FILER_H__
#define __OD_UNIVERSAL_WRITE_FILER_H__

#include "TD_PackPush.h"

#include "OdStreamBuf.h"

/** <group ExRender_Classes>
*/
class OdUniversalWriteFiler
{
protected:
  mutable OdStreamBufPtr m_pStream;
  OdUInt32 m_nVersion;

  void assertWr() const; 

public:
  OdUniversalWriteFiler(const OdString& sFilePathName, 
                        OdUInt32 nVersion = 100,
                        const char* pMagic = "TGS");
  OdUniversalWriteFiler(OdStreamBuf* pStream = NULL,
                        OdUInt32 nVersion = 100,
                        const char* pMagic = "TGS");
  ~OdUniversalWriteFiler();

  // Attach stream and initiate write process
  bool setStream(OdStreamBuf* pStream, 
                 OdUInt32 nVersion = 100,
                 const char* pMagic = "TGS");
  OdStreamBufPtr stream() const;
  OdUInt32 version() const;

  // Process raw data
  void wrRawData(const void* pData, OdUInt32 nDataSize);
  void wr2BytesNumber(void* pData);
  void wr4BytesNumber(void* pData);
  void wr4Bytes(void* pData);
  void wr8Bytes(void* pData);

  // Process handles
  //void wrHandle(class OdDbStub *pHandle);

  // Process data primitives
  void wrBool(bool bVal);
  void wrInt(int val);
  //void wrUInt(unsigned int val);
  //void wrChar(char val);
  void wrUInt8(OdUInt8 val);
  void wrInt16(OdInt16 val);
  void wrUInt16(OdUInt16 val);
  void wrInt32(OdInt32 val);
  void wrUInt32(OdUInt32 val);
  void wrInt64(OdInt64 val);
  void wrUInt64(OdUInt64 val);
  //void wrCOLORREF(ODCOLORREF val);
  void wrFloat(float val);
  void wrDouble(double val);
  void wrMatrix3d(const class OdGeMatrix3d &mat);
  void wrAnsiString(const OdAnsiString &str);
};

#include "TD_PackPop.h"

#endif // __OD_UNIVERSAL_WRITE_FILER_H__
