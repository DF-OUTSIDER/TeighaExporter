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

#ifndef RXSYSTEMSERVICESIMPL_DEFINED
#define RXSYSTEMSERVICESIMPL_DEFINED

#include "TD_PackPush.h"

#include "RxSystemServices.h"

/** \details
  This class implements platform-dependent file operations for Teigha.
  <group ExServices_Classes> 
  Library: Source code provided. 
*/
class RxSystemServicesImpl : public OdRxSystemServices
{
public:
  RxSystemServicesImpl();

  OdStreamBufPtr createFile(
    const OdString& filename,                            
    Oda::FileAccessMode accessMode = Oda::kFileRead,  
    Oda::FileShareMode shareMode = Oda::kShareDenyNo, 
    Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting);

  bool accessFile(const OdString& filename, int accessMode);

  OdInt64 getFileCTime(const OdString& filename);                 
  OdInt64 getFileMTime(const OdString& filename);                 
  OdInt64 getFileSize(const OdString& filename);

  OdString formatMessage(unsigned int formatId, va_list* argList = 0);
  
  /** \details
    Returns the system code page.
    
    \remarks
    On Windows platforms system code page is initialized based on computer's
    Regional Settings. On other platforms with CP_UNDEFINED.
    It can be altered by setSystemCodePage()
  */
  OdCodePageId systemCodePage() const;
  
  /** \details
    Sets the system code page.
    \param codePage [in]  CodePage.
  */
  void setSystemCodePage(OdCodePageId id);

  /** \details
  Get environment variable. If varName doesn't exist, returns eKeyNotFound.

  */
  virtual OdResult getEnvVar(const OdString &varName, OdString &value);

  /** \details
  Set environment variable with new value.

  */
  virtual OdResult setEnvVar(const OdString &varName, const OdString &newValue);

protected:
  OdCodePageId m_CodePageId;
};

#include "TD_PackPop.h"

#endif	// RXSYSTEMSERVICESIMPL_DEFINED
