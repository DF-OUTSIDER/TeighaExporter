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
#ifndef ODZMQSOCKETLIB
#define ODZMQSOCKETLIB
#include <OdaCommon.h>
#include "RxModule.h"
#include "TxDefs.h"

using namespace Oda;

/** \details
  <group Extension_Classes>
*/
class ZmqSocketLib : public OdRxModule {
protected:
  static void* zcontext;

  void initApp();
  void uninitApp();
  static ZmqSocketLib* singleton();

  static WSTR S_ADDRES;
public:
  static OdConstString s_ZeroMQ;

  static void* zmqCtx() { return ZmqSocketLib::singleton()->zcontext; }


  /**
  <title BASEIO Command>
  <toctitle BASEIO Command>

  \syntax_cloud
  BASEIO&lt;bind address&gt;

  \details
  Creates server-side networking console I/O object and binds it to the specified address. Object is returned via OdEdCommandContext::setResult() and can be read by a caller via OdEdCommandContext::result(). This finction is being called from TxHost's EDIOBIND command implementation.

  \params_cloud
  <table>
  <b>Command keys and parameters</b>        <b>Description</b>
  &lt;bind address&gt;                      [in]  Address for bind I/O console
  </table>
  */
  static void BASEIO(CMDCTX* ctx);

  /**
  <title BASEIOCLIENT Command>
  <toctitle BASEIOCLIENT Command>

  \syntax_cloud
  BASEIOCLIENT&lt;connection address&gt;

  \details
  Creates client-side socket object and connects it to the specified server address. Object is returned via OdEdCommandContext::setResult() and can be read by a caller via OdEdCommandContext::result(). This finction is not being called in Teigha Cloud Viewer yet.

  \params_cloud
  <table>
  <b>Command keys and parameters</b>        <b>Description</b>
  &lt;connection address&gt;                [in]  Address to connect to I/O console.
  </table>
  */
  static void BASEIOCLIENT(CMDCTX* ctx);

  /**
  <title SOCKET Command>
  <toctitle SOCKET Command>

  \syntax_cloud
  SOCKET[Alias &lt;name&gt;][Connect[|Bind]]&lt;Bind[|Connect] string&gt;

  \details
  Creates, deletes and manipulates sockets

  \params_cloud
  <table>
  <b>Command keys and parameters</b>                <b>Description</b>
  Alias &lt;name&gt;                                [in]  Alias of socket
  Connect[|Bind]                                    [in]  Connect or bind
  &lt;Bind[|Connect] string&gt;                     [in]  Bind or connection address
  </table>
  */
  static void SOCKET(CMDCTX* ctx);

  /**
  <title SENDFILE Command>
  <toctitle SENDFILE Command>

  \syntax_cloud
  SENDFILE[Address &lt;socket address or alias name&gt;]&lt;file to send&gt;

  \details
  Experimental. For debugging. Sends a file to the last referenced socket or to the optionally specified socket.

  \params_cloud
  <table>
  <b>Command keys and parameters</b>                <b>Description</b>
  Address &lt;socket address or alias name&gt;      [in]  Address or alias for connect to socket
  &lt;file to send&gt;                              [in]  Path to file for send
  </table>
  */
  static void SENDFILE(CMDCTX* ctx);

  /**
  <title RECEIVEFILE Command>
  <toctitle RECEIVEFILE Command>

  \syntax_cloud
  RECEIVEFILE[Address &lt;socket address or alias name&gt;][Try][Wait]&lt;file to save&gt;

  \details
  Experimental. For debugging. Receives the data from the last referenced socket, or optionally from a specified one, and saves the data to a specified file.

  \params_cloud
  <table>
  <b>Command keys and parameters</b>                <b>Description</b>
  Address &lt;socket address or alias name&gt;      [in]  Address or alias for receive file
  Try                                               [in]  Try to receive file
  Wait                                              [in]  Wait while receive file
  &lt;file to save&gt;                              [in]  Path to save file
  </table>
  */
  static void RECEIVEFILE(CMDCTX* ctx);
};

#endif //ODZMQSOCKETLIB
