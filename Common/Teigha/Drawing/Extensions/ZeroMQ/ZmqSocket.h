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

#ifndef ODZMQSOCKET
#define ODZMQSOCKET


#include "zmq.h"
#include <OdaCommon.h>
#pragma MARKMESSAGE(DNA: Workaround: If "zmq.h" is included after <OdaCommon.h> then MSVC10 failed to build ZmqSocket.cpp in RELEASE...)
#include "FlatMemStream.h"
#include "TxDefs.h"
#include "MemoryStream.h"
#include "MemoryStreamImpl.h"
#include "ZmqSocketLib.h"
using namespace Oda;

#include "TD_PackPush.h"


/** \details
  "Thread safety
  ZMQ sockets are not thread safe. Applications MUST NOT use a socket from multiple threads
  except after migrating a socket from one thread to another with a "full fence" memory barrier."
  http://api.zeromq.org/2-1:zmq-socket
  <group Extension_Classes>
*/
class ODRX_ABSTRACT ZmqSocket : public OdRxObject {
protected:
  static inline void throwZmqErrorIf(bool condition);
  static inline void throwIfFailed(int zmqResult);
  void parseSocketString(OdString& connString);

  struct InBuf : OdFlatMemStream {
    zmq_msg_t buf;
    InBuf();
    ~InBuf();
  };
  friend struct InBuf;

  void *zsocket;
  OdSmartPtr<InBuf> inpBuf;

  static void* context() { return ZmqSocketLib::zmqCtx(); }
  int socketType() const;

  int socketOption(int optName) const;
public:
  ODRX_DECLARE_MEMBERS(ZmqSocket);

  ZmqSocket();
  ~ZmqSocket();

  virtual void connect( WSTRREF connString );
  virtual void bind( WSTRREF connString );

  bool canSend() const { return (socketOption(ZMQ_EVENTS) & ZMQ_POLLOUT)!=0; }
  bool canReceive() const { return (socketOption(ZMQ_EVENTS) & ZMQ_POLLIN)!=0; }
  bool canSendOrReceive() const { return (socketOption(ZMQ_EVENTS) & (ZMQ_POLLIN|ZMQ_POLLOUT))==(ZMQ_POLLIN|ZMQ_POLLOUT); }

  enum {
    kDontWait     = 1,
    kSendPart     = 2,

    kRead         = 4,
    kWrite        = 8,
    kReadWrite    = kRead|kWrite,
  };
  virtual OdStreamBufPtr recieve(int options = 0);
  virtual bool moreParts();
  virtual bool send(OdStreamBuf* buf, int options = 0);
  virtual bool sendBytes(const void* data, OdUInt32 bytes, int options = 0);

  /** \details
    Returns Virtual File Interface input of a string.
 
    \param options [in]  combination of ZmqSocket::enum options.
    
    \remarks
    If returns 0 if there is no message in the input queue or sending to a socket is not possible and kDontWait option is selected
    It returns Read, Write or Read-Write stream that is flushed to to this socket once returned pointer was released
    See also ZeroMQ.SENDFILE and ZeroMQ.RECEIVEFILE commands
  */
  virtual OdStreamBufPtr asFile(int options = 0);

  static void SOCKET( CMDCTX* ctx );

  virtual void close();

  static void initZmqLib();
  static void uninitZmqLib();
};

typedef OdSmartPtr<ZmqSocket> ZmqSocketPtr;



inline ZmqSocket::ZmqSocket() : zsocket(0) {}

inline void ZmqSocket::throwZmqErrorIf(bool condition) {
  if(condition)
    throw OdError( ::zmq_strerror(zmq_errno()) );
}

inline void ZmqSocket::throwIfFailed(int zmqResult) {
  if(zmqResult==-1) {
    int err = ::zmq_errno();
    switch( err ) {
    case EFSM:
      throw OdError( eFileAccessErr );
    default:
      throw OdError( ::zmq_strerror( err ) );
    }
  }
}

inline ZmqSocket::InBuf::InBuf() {
  ZmqSocket::throwIfFailed( ::zmq_msg_init( &buf ) );
}

inline ZmqSocket::InBuf::~InBuf() {
  ODA_VERIFY( ::zmq_msg_close( &buf )==0 );
}

#define ODRX_DEFINE_CWSTR(name, string) static OdConstString name(string)
#define ODRX_CWSTR(name) name


#include "TD_PackPop.h"

#endif //ODZMQSOCKET
