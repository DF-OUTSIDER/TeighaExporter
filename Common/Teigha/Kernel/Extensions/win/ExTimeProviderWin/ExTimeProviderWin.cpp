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

#include <winsock2.h>
#include <ws2tcpip.h>

#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "ExTimeProviderWin.h"

ODRX_DEFINE_DYNAMIC_MODULE(ExTimeProviderWin);

void ExTimeProviderWin::initApp()
{ 
}

void ExTimeProviderWin::uninitApp()
{
}

OdTimeStamp getTimeStampFromRFC868TimeMessage(OdUInt32 timeMessage);

bool ExTimeProviderWin::getTimeFromServer(const OdString& serverName, OdTimeStamp& timeStamp)
{
  bool res = false;

  if (serverName.isEmpty())
  {
    OdTimeStamp resTimeStamp(OdTimeStamp::kInitUniversalTime);
    timeStamp = resTimeStamp;
    res = true;
  }
  else
  {
    WSADATA wsaData;
    int retVal = 0;
    retVal = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (retVal == 0)
    {
      PADDRINFOA pAddrInfo = NULL;
      retVal = getaddrinfo((const char*)serverName, "37", NULL, &pAddrInfo);
      if (retVal == 0)
      {
        SOCKET connectSocket = INVALID_SOCKET;
        connectSocket = socket(pAddrInfo->ai_family, SOCK_STREAM, IPPROTO_TCP);
        if (connectSocket != INVALID_SOCKET)
        {
          timeval timeout;
          ZeroMemory(&timeout,sizeof(timeval));
          timeout.tv_sec = 1000;          
          if ((setsockopt(connectSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout.tv_sec, sizeof(DWORD)) != SOCKET_ERROR) &&
              (setsockopt(connectSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout.tv_sec, sizeof(DWORD)) != SOCKET_ERROR))
          {
            DWORD timeData = 0;

            if ((connect(connectSocket, pAddrInfo->ai_addr, (int)pAddrInfo->ai_addrlen) != SOCKET_ERROR) &&
                (recv(connectSocket, (char*)&timeData, sizeof(DWORD), 0) != SOCKET_ERROR))
            {
              DWORD timeDataInHostByteOrder;
#ifdef ODA_BIGENDIAN
              timeDataInHostByteOrder = timeData;
#else
              timeDataInHostByteOrder = ntohl(timeData);
#endif
              timeStamp = getTimeStampFromRFC868TimeMessage(timeDataInHostByteOrder);              
              res = true;
            }
          }
          closesocket(connectSocket);
        }
        freeaddrinfo(pAddrInfo);
      }
      WSACleanup();
    }
  }

  return res;
}

OdTimeStamp getTimeStampFromRFC868TimeMessage(OdUInt32 timeMessage)
{
  OdTimeStamp res;

  OdUInt64 time64 = timeMessage - 2208988800;
  res.setPackedValue(time64);

  return res;
}
