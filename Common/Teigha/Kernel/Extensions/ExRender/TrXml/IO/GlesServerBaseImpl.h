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
// GlesServerBaseImpl.h
//

#ifndef OG_GLES_SERVER_BASE_IMPL_H_
#define OG_GLES_SERVER_BASE_IMPL_H_

#include "CloudTools.h"
#include "XmlServerBaseImpl.h"

/** <group ExRender_Classes>
*/
class OdGlesServerBaseImpl : public OdGLES2XmlServerBaseImpl
{
protected:
  //OdStreamBufPtr m_pOutStream;
  bool m_bSkipShaders,
       m_useFakeDbStub,
       m_bEncodeTextureHex;
  std::map<OdDbStub*, OdAnsiString> m_mapHandles;

public:
  OdGlesServerBaseImpl(const OdDbBaseDatabase *pDb = NULL);

  // set output path name or format(if %d is present in path)
  virtual bool setOutPathName(const OdString& sPathName,
                              OdUInt64 limitToSplit = OdUInt64(), // if > OdUInt64() then willbe splited by next OnStateChanged
                              int indexNextFree = 0) = 0; 
  virtual int flushOut() = 0; // return next free index

  void setUseFakeDbStub(bool bVal);
  void setSkipShaders(bool bSkipShaders);
  void setEncodeTextureHex(bool bValue);

  OdStringArray getRenderHandles() const;

  // --- OdRxObject members ---

  virtual void release()
  {
    // should be an empty
    // to compensate changes type of OdGLES2XmlRendition::m_pXml for OdSmartPtr.
  }

  // --- OdGLES2XmlServer members ---

  //virtual void ident(const char* pTag, const char* pText);
  virtual void DropDbStub(const char* pTagName, OdDbStub* pDbStub);
  virtual void DropBinaryStream(const char* pTagName, const OdUInt8* pData, OdUInt32 nData);
};

#endif // OG_GLES_SERVER_BASE_IMPL_H_
