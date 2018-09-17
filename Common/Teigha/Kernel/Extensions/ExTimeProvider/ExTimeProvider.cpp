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

#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "ExTimeProvider.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

ODRX_DEFINE_DYNAMIC_MODULE(ExTimeProvider);

void ExTimeProvider::initApp()
{ 
}

void ExTimeProvider::uninitApp()
{
}

OdTimeStamp getTimeStampFromRFC868TimeMessage(OdUInt32 timeMessage);

bool ExTimeProvider::getTimeFromServer(const OdString& serverName, OdTimeStamp& timeStamp)
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
    OdPrintf("\nSTART\n");

    int retVal = 0;
    struct addrinfo* pAddrInfo = NULL;
    retVal = getaddrinfo((const char*)serverName, "37", NULL, &pAddrInfo);
    if (retVal == 0)
    {
      OdPrintf("\nGETADDR OK\n");

      int connectSocket;
      connectSocket = socket(pAddrInfo->ai_family, SOCK_STREAM, IPPROTO_TCP);
      if (connectSocket != -1)
      {
        OdPrintf("\nSOCKET OK\n");

        struct timeval timeout;
        timeout.tv_sec = 1000;
        timeout.tv_usec = 0;
        if ((setsockopt(connectSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) != -1) &&
          (setsockopt(connectSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout)) != -1))
        {
          OdPrintf("\nSETSOCKOPT OK\n");

          int timeData = 0;

          if ((connect(connectSocket, pAddrInfo->ai_addr, (int)pAddrInfo->ai_addrlen) != -1) &&
            (recv(connectSocket, (char*)&timeData, sizeof(int), 0) != -1))
          {
            OdPrintf("\nCONNECT AND RECV OK\n");

            int timeDataInHostByteOrder;
#ifdef ODA_BIGENDIAN
            timeDataInHostByteOrder = timeData;
#else
            timeDataInHostByteOrder = ntohl(timeData);
#endif
            timeStamp = getTimeStampFromRFC868TimeMessage(timeDataInHostByteOrder);              
            res = true;
          }
        }
        close(connectSocket);
      }
      freeaddrinfo(pAddrInfo);
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
