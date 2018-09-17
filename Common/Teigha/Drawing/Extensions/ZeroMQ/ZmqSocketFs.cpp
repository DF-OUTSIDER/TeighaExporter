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

#include "ZmqSocketFs.h"
#include <OdaCommon.h>
#pragma MARKMESSAGE(DNA: Workaround: If "zmq.h" is included after <OdaCommon.h> then MSVC10 failed to build ZmqSocket.cpp in RELEASE...)

using namespace OD;

OdRxObjectPtr ZmqSocketFsPx::open(OdRxObject* obj, const char* path, File::Access am, File::Share sm, File::Creation dispos) {
  Path filePath(path);
  if (filePath.first() == L"wr") {
    return ZmqSocketOut::createObject(ZmqSocketPtr(obj));
  }
  throw OdError_FileNotFound(path);
}

void ZmqSocketVFile::init(ZmqSocket* socket) {
  this->socket = socket;
  buffer = OdMemoryStream::createNew(0x100000); // 1 Mb page
}

void ZmqSocketVFile::release() {
  if (numRefs()==1)
    onFinalRelease();
  OdRxObjectImpl<OdStreamBuf>::release();
}

bool ZmqSocketVFile::isEof() {
  return buffer->isEof();
}

OdUInt64 ZmqSocketVFile::tell() {
  return buffer->tell();
}

OdUInt64 ZmqSocketVFile::length() {
  return buffer->length();
}

void ZmqSocketVFile::truncate() {
  return buffer->truncate();
}

void ZmqSocketVFile::rewind() {
  return buffer->rewind();
}

OdUInt64 ZmqSocketVFile::seek(OdInt64 offset, OdDb::FilerSeekType seekType) {
  return buffer->seek(offset, seekType);
}

void ZmqSocketVFile::getBytes(void* bytes, OdUInt32 numBytes) {
  return buffer->getBytes(bytes, numBytes);
}

void ZmqSocketVFile::putBytes(const void* bytes, OdUInt32 numBytes) {
  return buffer->putBytes(bytes, numBytes);
}

void ZmqSocketVFile::copyDataTo(OdStreamBuf* pDestination, OdUInt64 sourceStart /*= 0*/, OdUInt64 sourceEnd /*= 0*/) {
  return buffer->copyDataTo(pDestination, sourceStart, sourceEnd);
}


void ZmqSocketIn::onFinalRelease() {
}

void ZmqSocketOut::onFinalRelease() {
  socket->send(buffer);
}

OdStreamBufPtr ZmqSocketOut::createObject(ZmqSocket* socket) {
  OdSmartPtr<ZmqSocketOut> sockFile(new ZmqSocketOut, kOdRxObjAttach);
  sockFile->init(socket);
  return sockFile;
}
