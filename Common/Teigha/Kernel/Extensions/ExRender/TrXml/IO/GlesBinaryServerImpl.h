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
// GlesBinaryServerImpl.h
//

#ifndef OG_GLES_BINARY_SERVER_IMPL_H_
#define OG_GLES_BINARY_SERVER_IMPL_H_

#include "CloudTools.h"
#include "GlesServerBaseImpl.h"
#include "UnivWriteFiler.h"

/** <group ExRender_Classes>
*/
class OdGlesBinaryServerImpl : public OdGlesServerBaseImpl
{
  OdString m_sPathNameFormat;
  OdUInt64 m_limitToSplit;
  OutputState m_state;
  int m_indexNextFree;
  OdSharedPtr<OdUniversalWriteFiler> m_pFiler;
  OdArray<OdAnsiString> m_stackPaths;

  //bool m_bSkipShaders,
  //     m_useFakeDbStub,
  //     m_bEncodeTextureHex;

public:
  OdGlesBinaryServerImpl(const OdDbBaseDatabase *pDb = NULL);
  ~OdGlesBinaryServerImpl();

  virtual void setOutput(OdStreamBuf * buf);
  // set output path name or format(if %d is present in path)
  virtual bool setOutPathName(const OdString& sPathName,
                              OdUInt64 limitToSplit = OdUInt64(), // if > OdUInt64() then willbe splited by next OnStateChanged
                              int indexNextFree = 0); 
  virtual int flushOut(); // return next free index

  bool wrPathKey(const char* pTagName);

  virtual void ident(const char* pTag, const char* pText);

  virtual void IncreaseNesting(const char* pLevelName);
  virtual void DecreaseNesting(const char* pLevelName);

  virtual void DropBoolean(const char* pTagName, bool data);
  //virtual void DropBooleanOpt(const char* pTagName, bool data) { if (data) DropBoolean(pTagName, data); }

  virtual void DropUInt8(const char* pTagName, OdUInt8 data);
  virtual void DropUInt16(const char* pTagName, OdUInt16 data);
  virtual void DropUInt32(const char* pTagName, OdUInt32 data);
  virtual void DropUInt64(const char* pTagName, OdUInt64 data);

  virtual void DropInt16(const char* pTagName, OdInt16 data);
  virtual void DropInt32(const char* pTagName, OdInt32 data);

  virtual void DropChars(const char* pTagName, const char* pStr);
  virtual void DropInts(const char* pTagName, OdUInt32 nData, const OdUInt16* pData);
  virtual void DropFloats(const char* pTagName, OdUInt32 nData, const float* pData);

  virtual void DropFloat(const char* pTagName, float data);
  virtual void DropFloat2(const char* pTagName, float data1, float data2);
  virtual void DropFloat3(const char* pTagName, float data1, float data2, float data3);
  virtual void DropFloat4(const char* pTagName, float data1, float data2, float data3, float data4);

  virtual void DropDouble(const char* pTagName, double data);
  virtual void DropDouble2(const char* pTagName, double data1, double data2);
  virtual void DropDouble3(const char* pTagName, double data1, double data2, double data3);
  virtual void DropDouble4(const char* pTagName, double data1, double data2, double data3, double data4);

  virtual void DropUInt8RGB(const char* pTagName, OdUInt8 data1, OdUInt8 data2, OdUInt8 data3, OdUInt8 data4);

  virtual void DropMatrix(const char* pTagName, const OdGeMatrix3d& data);

  virtual void DropBinaryStream(const char* pTagName, const OdUInt8* pData, OdUInt32 nData);

  virtual void DropDbStub(const char* pTagName, OdDbStub* pDbStub);

  virtual void DropId(const char* pTagName, OdUInt64 data, OdTrVisUniqueID* = NULL, OdInt32 = -1);

  virtual void OnStateChanged(OutputState newState);
  virtual bool UpdateClientDependentSectionOnly() const;
};

#endif // OG_GLES_BINARY_SERVER_IMPL_H_

