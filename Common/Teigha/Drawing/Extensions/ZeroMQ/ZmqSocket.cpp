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


//#include "zmq.h"
#include "ZmqSocket.h"
#include "Ed/EdUserIO.h"
#include "RxDictionary.h"
#include "Ed/EdCommandStack.h"
#include "Error_InvalidInput.h"

OdConstString sZeroMQ(L"ZeroMQ");

ODRX_CONS_DEFINE_MEMBERS(ZmqSocket, OdRxObject, RXIMPL_CONSTR);

using namespace Oda;

ZmqSocket::~ZmqSocket() {
  close();
}

STRMBFPTR ZmqSocket::recieve(int options) {
  if(inpBuf.isNull() || inpBuf->numRefs()>1)
    inpBuf = OdRxObjectImpl<InBuf>::createObject();
  throwIfFailed( ::zmq_msg_close( &inpBuf->buf ) );
  throwIfFailed( ::zmq_msg_init( &inpBuf->buf ) );

  int msglen = ::zmq_msg_recv( &inpBuf->buf, zsocket, options );
  if(msglen == -1) {
    int err = ::zmq_errno();
    if(err==EAGAIN && GETBIT(options, kDontWait))
      return STRMBFPTR();
    throw EXCEP( ::zmq_strerror( err ) );
  }

  inpBuf->init( ::zmq_msg_data( &inpBuf->buf ), msglen, 0 );
  return inpBuf;
}

bool ZmqSocket::moreParts() {
  OdInt64 bMore;
  size_t more_size = sizeof(bMore);
  throwIfFailed( ::zmq_getsockopt(zsocket, ZMQ_RCVMORE, &bMore, &more_size) );
  return bMore!=0;
}

bool ZmqSocket::send(STRMBF* data, int options) {
  int rc1 = 0, rc2 = 0;
  if(data) {
    zmq_msg_t out_buf;
    UI64 len = data->length();
    throwIfFailed( ::zmq_msg_init_size( &out_buf, len ) );
    if(len) {
      data->rewind();
      data->getBytes( ::zmq_msg_data( &out_buf ), len );
    }
    rc1 = ::zmq_sendmsg( zsocket, &out_buf, options );
    rc2 = ::zmq_msg_close( &out_buf );
  } else { // close ZMQ_STREAM accepted connection
    rc1 = ::zmq_send( zsocket, 0, 0, options );
  }

  if(rc1==-1) {
    int err = ::zmq_errno();
    if(err==EAGAIN && GETBIT(options, kDontWait))
      return false;
    throw EXCEP( ::zmq_strerror( err ) );
  }
  throwIfFailed( rc1 );
  throwIfFailed( rc2 );
  return true;
}

bool ZmqSocket::sendBytes(const void* data, OdUInt32 len, int options) {
  int rc1 = 0, rc2 = 0;
  if(data) {
    zmq_msg_t out_buf;
    throwIfFailed( ::zmq_msg_init_size( &out_buf, len ) );
    ::memcpy( ::zmq_msg_data( &out_buf ), data, len );
    rc1 = ::zmq_sendmsg( zsocket, &out_buf, options );
    rc2 = ::zmq_msg_close( &out_buf );
  } else { // close ZMQ_STREAM accepted connection
    rc1 = ::zmq_send( zsocket, 0, 0, options );
  }

  if(rc1==-1) {
    int err = ::zmq_errno();
    if(err==EAGAIN && GETBIT(options, kDontWait))
      return false;
    throw EXCEP( ::zmq_strerror( err ) );
  }
  throwIfFailed( rc1 );
  throwIfFailed( rc2 );
  return true;
}

class Sock2File : public OdStreamBuf {
  ZmqSocketPtr socket;
public:
  Sock2File* init(int options, const PATH& path) {
    HRXDIC sockDict = HRXDIC(::odrxSysRegistry()).walk(path, GETBIT(options, kFileWrite));
    return init(options, (ZmqSocket*)sockDict.getAt(WSTR(sZeroMQ)).get());
  }
  Sock2File* init(int options, ZmqSocket* socket) {
    this->socket = socket;
    return this;
  }
  bool sendFile(int options) { return socket->send(this, options); }
  STRMBFPTR receiveFile(int options) { return socket->recieve(options); }

};

OdStreamBufPtr ZmqSocket::asFile(int options /*= 0*/) {
  return OdRxObjectImpl<Sock2File>::createObject()->init(options, this);
}

OdString parseOption( OdString& conn ) {
  conn.trimRight(L"/");
  int i = conn.reverseFind(L'/');
  if(i<=0)
    throw OdError(eInvalidInput);
  OdString res = conn.right(conn.getLength()-i-1);
  conn = conn.left(i);
  return res;
}

int parseType( const OdString& sType ) {
#define ZMQ_SOCKET_TYPE(TYPE)\
  if(sType.iCompare(#TYPE)==0) return ZMQ_##TYPE;

  ZMQ_SOCKET_TYPE( PAIR   );
  ZMQ_SOCKET_TYPE( PUB    );
  ZMQ_SOCKET_TYPE( SUB    );
  ZMQ_SOCKET_TYPE( REQ    );
  ZMQ_SOCKET_TYPE( REP    );
  ZMQ_SOCKET_TYPE( DEALER );
  ZMQ_SOCKET_TYPE( ROUTER );
  ZMQ_SOCKET_TYPE( PULL   );
  ZMQ_SOCKET_TYPE( PUSH   );
  ZMQ_SOCKET_TYPE( XPUB   );
  ZMQ_SOCKET_TYPE( XSUB   );
  ZMQ_SOCKET_TYPE( STREAM );

  throw OdError_InvalidInput(OdString("Socket ")+sType);
#undef GET_TYPE
}

int ZmqSocket::socketOption(int optName) const {
  int res = 0;
  size_t sz = sizeof(res);
  throwIfFailed(::zmq_getsockopt(zsocket, optName, &res, &sz));
  return res;
}

void ZmqSocket::parseSocketString(OdString& conn) {
  int type = parseType( parseOption( conn ) );
  zsocket = ::zmq_socket( context(), type );
  throwZmqErrorIf( !zsocket );
}

void ZmqSocket::bind( const OdString& connString ) {
  OdString conn = connString;
  parseSocketString(conn);
  throwIfFailed( zmq_bind(zsocket, conn) );
}

void ZmqSocket::connect( const OdString& connString ) {
  OdString conn = connString;
  parseSocketString(conn);
  throwIfFailed( zmq_connect(zsocket, conn) );
}

void ZmqSocket::close() {
  if( zsocket ) {
    throwIfFailed( zmq_close(zsocket) );
    zsocket = 0;
  }
}

