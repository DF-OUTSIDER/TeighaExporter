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
/*
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4018) // signed/unsigned mismatch
#pragma warning(disable : 4146) // unary minus operator applied to unsigned type, result still unsigned

#if !defined(_WIN32_WCE) && !defined(ODA_MAC_OSX) && !defined(__MWERKS__)
// Use standard ftime() function available for all systems except WinCE
#include <sys/timeb.h>
#define ODA_USE_FTIME

#include <sys/types.h>
#endif

#ifdef WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif

#ifndef _WIN32_WCE
#include <io.h>
#endif

#pragma warning(pop)
#endif
*/

#ifdef TG_SystemServices

#ifndef _WIN32_WCE
#include <sys/stat.h>
#endif

#include "OdaCommon.h"
#include "ExDgnServices.h"
#include "../Extensions/ExServices/OdFileBuf.h"
#include "FlatMemStream.h"

OdStreamBufPtr OdExDgnSystemServices::createFile( const OdString& pcFilename,
                                                  Oda::FileAccessMode nDesiredAccess,
                                                  Oda::FileShareMode nShareMode,
                                                  Oda::FileCreationDisposition nCreationDisposition
                                                )

{
  OdSmartPtr<OdBaseFileBuf> pFile;

  if(!pcFilename.isEmpty() && pcFilename[0])
  {
    if (nDesiredAccess == Oda::kFileRead)
    {
      pFile = OdRdFileBuf::createObject();
    }
    else
    {
      pFile = OdWrFileBuf::createObject();
    }

    pFile->open(pcFilename, nShareMode, nDesiredAccess, nCreationDisposition);
  }
  else
  {
    throw OdError(eNoFileName);
  }
  return OdStreamBufPtr(pFile);
}

bool OdExDgnSystemServices::accessFile( const OdString& pcFilename, int mode )
{
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(_WINRT)
  // SetErrorMode() function is used to avoid the message box
  // if there is no floppy disk in the floppy drive (CR 2122).
  int oldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
  bool res = (_waccess(pcFilename,
    (GETBIT(mode, Oda::kFileRead)  ? 0x04 : 0x00) | 
    (GETBIT(mode, Oda::kFileWrite) ? 0x02 : 0x00)) == 0);
  SetErrorMode(oldErrorMode);
  if (res)
  {
    struct _stat st;
    _wstat(pcFilename,&st);
    if (st.st_mode & _S_IFDIR)
      return false;
  }
  return res;
#elif defined(sgi)
  // Use this version, since try/catch below results in memory leaks on sgi (OdError destructor doesn't get called).
  OdString  sMode = (mode == Oda::kFileRead) ? OD_T("rb") : OD_T("wb");
  FILE* fp = fopen((const char*)pcFilename, (const char*)sMode);
  if (fp)
  {
    fclose(fp);
    return true;
  }
  return false;
#else
  try
  {
    createFile(pcFilename, (Oda::FileAccessMode)mode);
    return true;
  }
  catch(...) 
  {
    return false; // Do NOT remove this, or else some compilers (e.g. cw7 mac) will optimize out the catch! 
  }
  return false;
#endif
}

OdString OdExDgnSystemServices::formatMessage(unsigned int code, va_list* argList)
{
  static const OdChar* message[] =
  {
#define OD_ERROR_DEF(cod, desc)  desc,
#include "ErrorDefs.h"
#undef OD_ERROR_DEF

#define OD_MESSAGE_DEF(cod, desc) desc,
#include "MessageDefs.h"
#undef OD_MESSAGE_DEF
    OD_T("")// DummyLastMassage
  };

  OdString msg;
  if (code >= sidDummyLastMessage)
  {
    msg.format(OD_T("Unknown message (code: %d)"), code);
    return msg;
  }

  if (argList)
  {
    msg.formatV(message[code], *argList);
  }
  else msg = message[code];

  return msg;
}

OdRxClass* OdExDgnSystemServices::isA() const 
{ 
	if (!OdExDgnSystemServices::g_pDesc) 
	{                                                                         
		OdExDgnSystemServices::g_pDesc = ::newOdRxClass(OD_T("OdExDgnSystemServices"),OdRxObject::desc(),0,0,0,0,OdString::kEmpty,OdString::kEmpty);                                   
	} 
	else 
	{                                                                                           
		ODA_ASSERT(("Class OdExDgnSystemServices is already initialized.",0));                                  
		throw OdError(eExtendedError);                                                                   
	}                                                                                                  
	return g_pDesc; 
}                                                

OdRxClass* OdExDgnSystemServices::g_pDesc = (OdRxClass*)0;
#endif
