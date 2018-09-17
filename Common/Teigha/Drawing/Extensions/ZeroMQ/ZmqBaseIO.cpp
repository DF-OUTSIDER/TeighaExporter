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
#include "ZmqBaseIO.h"
#include "TxDefs.h"


using namespace Oda;


OdAnsiString ZmqBaseIO::getRequest() {
  OdStreamBufPtr input = socket.recieve();
  OdUInt32 len = input->length();
  OdAnsiString ansi;
  input->getBytes(ansi.getBuffer(len), len);
  ansi.releaseBuffer(len);
  return ansi;
}

OdString ZmqBaseIO::getString(int options) {
  if (inbuf.isEof()) {
    OdAnsiString rcvd = getRequest().c_str();
    if (rcvd.isEmpty())
      return OdString::kEmpty;
    inbuf.reset(toUtf16(rcvd));
  }
  return inbuf.getString(OdString::kEmpty, options);
}

OdString ZmqBaseIO::getString(const OdString& prompt, int options, OdEdStringTracker*) {
  sendReplay(prompt, !inbuf.isEof());
  OdString s = getString(options);
  if (s.iCompare(L"^C") == 0)
    throw OdEdCancel();
  return s;
}

void ZmqBaseIO::sendReplay(const OdString& repl, bool more) {
  OdAnsiString ansi = ::toUtf8(repl);
  socket.sendBytes(ansi.c_str(), ansi.getLength(), more ? ZmqSocket::kSendPart : 0);
}

void ZmqBaseIO::putString(const OdString& string) {
  sendReplay(string, true);

}

void ZmqBaseIO::bind(const OdString& addres) {
  socket.bind(OdString().format(L"tcp://%ls/REP", addres.c_str()));
  // Web server's REQ socket is requesting
  // this REP socket is replaying

  OdAnsiString sHandshake = getRequest(); // DNA handshake REQ from server, REPlay will be sent when getString() will be called
  if (sHandshake != "handshake")
    throw OdError("HANDSHAKE failed, server requested: " + sHandshake);
}
