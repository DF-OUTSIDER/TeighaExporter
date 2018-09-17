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

#ifndef ODZMQSOCKETFS
#define ODZMQSOCKETFS

#include "ZmqSocket.h"
#include "RxFS.h"
#include "RxObjectImpl.h"
#include "MemoryStream.h"

#include "TD_PackPush.h"


/** \details
  <group Extension_Classes>
*/
class ODRX_ABSTRACT ZmqSocketFsPx : public OD::FilePx {
public:
   OdRxObjectPtr open(
     OdRxObject* obj,
     const char* path,
     OD::File::Access am = OD::File::Read,
     OD::File::Share sm = OD::File::DenyNo,
     OD::File::Creation dispos = OD::File::OpenExisting
     );
};

/** \details
  <group Extension_Classes>
*/
class ZmqSocketVFile : public OdRxObjectImpl<OdStreamBuf> {
  using OdRxObjectImpl<OdStreamBuf>::createObject;
protected:
  OdMemoryStreamPtr buffer;
  ZmqSocketPtr      socket;
  virtual void onFinalRelease() = 0;

  void init(ZmqSocket* socket);
public:
  void release();

  virtual bool isEof();

  virtual OdUInt64 tell();

  virtual OdUInt64 length();

  virtual void truncate();

  virtual void rewind();

  virtual OdUInt64 seek(OdInt64 offset, OdDb::FilerSeekType seekType);

  virtual void getBytes(void* buffer, OdUInt32 numBytes);

  virtual void putBytes(const void* buffer, OdUInt32 numBytes);

  virtual void copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart = 0, OdUInt64 sourceEnd = 0);
};

/** \details
  <group Extension_Classes>
*/
class ZmqSocketIn : public ZmqSocketVFile {
protected:
  virtual void onFinalRelease();
public:
};

/** \details
  <group Extension_Classes>
*/
class ZmqSocketOut : public ZmqSocketVFile {
protected:
  virtual void onFinalRelease();
public:
  static OdStreamBufPtr createObject(ZmqSocket* socket);
};


#include "TD_PackPop.h"

#endif //ODZMQSOCKETFS
