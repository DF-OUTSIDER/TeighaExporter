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
#include "OdaCommon.h"
#include "RxModule.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "ZmqSocket.h"
#include "StaticRxObject.h"
#include "ZmqSocketFs.h"
#include "ZmqSocketLib.h"
#include "ZmqBaseIO.h"

DISABLE_THREAD_LIBRARY_CALLS()


void* ZmqSocketLib::zcontext = 0;

WSTR ZmqSocketLib::S_ADDRES;

OdConstString ZmqSocketLib::s_ZeroMQ(L"ZeroMQ");

ODRX_DEFINE_DYNAMIC_MODULE(ZmqSocketLib);

ODRX_DEFINE_CWSTR(s_Address, L"Address");

void ZmqSocketLib::BASEIO(CMDCTX* ctx) {
  OdEdUserIO& io = *ctx->userIO();
  OdSmartPtr<ZmqBaseIO> s = OdRxObjectImpl<ZmqBaseIO>::createObject();
  s->bind(io.getString("Bind address"));
  ctx->funcIO()->setResult(s);
}

void ZmqSocketLib::BASEIOCLIENT(CMDCTX* ctx) {
  OdEdUserIO& io = *ctx->userIO();
  ZmqSocketPtr s = ZmqSocket::createObject();
  s->connect(io.getString("Connection address"));
  ctx->funcIO()->setResult(s);
}

// LOADAPP ZeroMQ SOCKET Alias Binded tcp://127.0.0.1:5555/PAIR SOCKET Alias Connected Connect tcp://127.0.0.1:5555/PAIR SENDFILE A:/send.txt RECEIVEFILE Address Binded A:/receive.txt SOCKET Alias Connected Close SOCKET Alias Binded Close
// LOADAPP ZeroMQ SOCKET Alias Binded tcp://127.0.0.1:5555/PAIR SOCKET Alias Connected Connect tcp://127.0.0.1:5555/PAIR RECEIVEFILE Try Address Binded A:/receive.txt SOCKET Alias Connected Close SOCKET Alias Binded Close

// Test Socket FS
// LOADAPP WebMisc LOADAPP ZeroMQ SOCKET Alias Binded tcp://127.0.0.1:5555/PAIR SOCKET Alias Connected Connect tcp://127.0.0.1:5555/PAIR STATUS "Test string written through Socket FS" rx:/ZeroMQ/Connected/wr RECEIVEFILE Address Binded A:/receive.txt SOCKET Alias Connected Close SOCKET Alias Binded Close

void ZmqSocketLib::SOCKET(CMDCTX* ctx) {
  OdEdUserIO& io = *ctx->userIO();
  bool bBind = true;
  WSTR str;
  WSTR alias;
  for (;;) {
    try {
      ODRX_DEFINE_CWSTR(s_Binding_string, L"Binding string [Alias/Connect/Close]");
      ODRX_DEFINE_CWSTR(s_Connection_string, L"Connection string [Alias/Bind/Close]");
      ODRX_DEFINE_CWSTR(s_Alias_Connect_Close, L"Alias Connect Close");
      ODRX_DEFINE_CWSTR(s_Alias_Bind_Close, L"Alias Bind Close");

      str = io.getString(
        bBind ? ODRX_CWSTR(s_Binding_string) : ODRX_CWSTR(s_Connection_string), 0, WSTR::kEmpty,
        bBind ? ODRX_CWSTR(s_Alias_Connect_Close) : ODRX_CWSTR(s_Alias_Bind_Close));
    }
    catch (const OdEdKeyword& kw) {
      ODRX_DEFINE_CWSTR(s_Alias_name, L"Alias name");
      switch (kw.keywordIndex()) {
      case 0:
        alias = io.getString(ODRX_CWSTR(s_Alias_name), 0);
        break;
      case 1:
        bBind = !bBind;
        break;
      case 2:
      {
        if (alias.isEmpty())
          alias = io.getString(ODRX_CWSTR(s_Alias_name), OdEd::kGstNoEmpty);
        HRXDIC sockDic = HRXDIC(::odrxSysRegistry()).stepCr(s_ZeroMQ);
        ZmqSocketPtr socket = sockDic.getAt(alias);
        if (socket.isNull())
          throw OdError("Invalid socket alias (address)");
        socket->close();
        sockDic.remove(alias);
      }
      return;
      }
      continue;
    }
    break;
  }

  ZmqSocketPtr s = ZmqSocket::createObject();
  if (bBind)
    s->bind(str);
  else
    s->connect(str);

  if (alias.isEmpty())
    alias = str;

  HRXDIC sysReg = HRXDIC(::odrxSysRegistry());
  sysReg.stepCr(s_ZeroMQ).putAt(alias, s);

  S_ADDRES = alias;
}

void ZmqSocketLib::SENDFILE(CMDCTX* ctx) {
  OdEdUserIO& io = *ctx->userIO();
  WSTR alias = S_ADDRES;
  WSTR fname;
  bool bSendPart = false;
  io.putString(WSTR().format(L"Address: %ls", alias.c_str()));
  for (;;) {
    try {
      ODRX_DEFINE_CWSTR(s_File_name, L"File name [Address]");
      fname = io.getString(ODRX_CWSTR(s_File_name), 0, WSTR::kEmpty, ODRX_CWSTR(s_Address));
      //fname = io.getString("File name [Address Part]", 0, WSTR::kEmpty, "Address Part"); // multipart support not implemented yet
    }
    catch (const OdEdKeyword& kw) {
      switch (kw.keywordIndex()) {
      case 0:
        alias = io.getString(s_Address);
        break;
      case 1:
        bSendPart = !bSendPart;
        io.putString(bSendPart ? "send part" : "last or single part");
        break;
      }
      continue;
    }
    break;
  }
  STRMBFPTR file = ::odrxSystemServices()->createFile(fname);

  ZmqSocketPtr socket = HRXDIC(::odrxSysRegistry()).step(s_ZeroMQ).getAt(alias);

  socket->send(file, bSendPart ? ZmqSocket::kSendPart : 0);

  S_ADDRES = alias;
}

void ZmqSocketLib::RECEIVEFILE(CMDCTX* ctx) {
  OdEdUserIO& io = *ctx->userIO();
  WSTR alias = S_ADDRES;
  WSTR fname;
  bool bDontWait = false;
  io.putString(WSTR().format(L"Address: %ls", alias.c_str()));
  for (;;) {
    ODRX_DEFINE_CWSTR(File_name_1, L"File name [Address/Wait]");
    ODRX_DEFINE_CWSTR(File_name_2, L"File name [Address/Try receive]");
    ODRX_DEFINE_CWSTR(Address_Wait, L"Address Wait");
    ODRX_DEFINE_CWSTR(Address_Try, L"Address Try");
    try {
      fname = io.getString(
        bDontWait ? ODRX_CWSTR(File_name_1) : ODRX_CWSTR(File_name_2), 0, WSTR::kEmpty,
        bDontWait ? Address_Wait : Address_Try);
    }
    catch (const OdEdKeyword& kw) {
      switch (kw.keywordIndex()) {
      case 0:
        alias = io.getString(s_Address);
        break;
      case 1:
        bDontWait = !bDontWait;
        break;
      }
      continue;
    }
    break;
  }
  ZmqSocketPtr socket = HRXDIC(::odrxSysRegistry()).step(s_ZeroMQ).getAt(alias);

  STRMBFPTR data = socket->recieve(bDontWait ? ZmqSocket::kDontWait : 0);
  if (data.isNull()) {
    io.putString("No messages");
  }
  else {
    using namespace Oda;
    STRMBFPTR file = ::odrxSystemServices()->createFile(fname, kFileWrite, kShareDenyReadWrite, kCreateAlways);
    data->copyDataTo(file);
  }
  S_ADDRES = alias;
}

#if defined(_TOOLKIT_IN_DLL_)
ZmqSocketLib* ZmqSocketLib::singleton() { return static_cast<ZmqSocketLib*>(g_pSingletonModule); }
#else
ZmqSocketLib* ZmqSocketLib::singleton() { return static_cast<ZmqSocketLib*>(g_pSingletonModuleZmqSocketLib); }
#endif


void ZmqSocketLib::initApp() {
  ZmqSocketLib::zcontext = ::zmq_ctx_new();
  if (!ZmqSocketLib::zcontext)
    throw OdError("Can't initialize ZMQ library.");

  ZmqSocket::rxInit();

  static OdStaticRxObject<ZmqSocketFsPx> g_zmqRxFs;
  ZmqSocket::desc()->addX(OD::FilePx::desc(), &g_zmqRxFs);

  OdEdCommandStackPtr cmds = odedRegCmds();
  OdString name = "BASEIO";
  /*!
  \brief Creates server-side networking console I/O object and binds it to the specified address.
  \param[in] address Bind address
  */
  cmds->addCommand(s_ZeroMQ, name, name, OdEdCommand::kNoUndoMarker, &BASEIO);
  name = "BASEIOCLIENT";
  /*!
  \brief Creates client-side socket object and connects it to the specified server address. 
  \param[in] address Connection address
  */
  cmds->addCommand(s_ZeroMQ, name, name, OdEdCommand::kNoUndoMarker, &BASEIOCLIENT);
  name = "SOCKET";
  /*!
  \brief Creates, deletes and manipulates sokets 
  \param[in] alias Create alias to this socket
  \param[in] connect Connection string
  \param[in] bind Binding string
  */
  cmds->addCommand(s_ZeroMQ, name, name, OdEdCommand::kNoUndoMarker, &SOCKET);
  name = "SENDFILE";
  /*!
  \brief Sends a file to the last referenced socket or to the optionally specified socket. 
  \param[in] address Socket address or alias
  \param[in] path File path
  */
  cmds->addCommand(s_ZeroMQ, name, name, OdEdCommand::kNoUndoMarker, &SENDFILE);
  name = "RECEIVEFILE";
  /*!
  \brief Receives the data from the last referenced socket, or optionally from a specified one, and saves the data to a specified file. 
  \param[in] address Socket address or alias
  \param[in] try Address try
  \param[in] wait Address wait
  \param[in] path File to save
  */
  cmds->addCommand(s_ZeroMQ, name, name, OdEdCommand::kNoUndoMarker, &RECEIVEFILE);


}

void ZmqSocketLib::uninitApp() {
  odedRegCmds()->removeGroup(s_ZeroMQ);

  ZmqSocket::desc()->delX(OD::FilePx::desc());
  ZmqSocket::rxUninit();

  ODA_ASSERT(ZmqSocketLib::zcontext != 0);
  ODA_VERIFY(::zmq_ctx_destroy(ZmqSocketLib::zcontext) == 0);
  ZmqSocketLib::zcontext = 0;
}
