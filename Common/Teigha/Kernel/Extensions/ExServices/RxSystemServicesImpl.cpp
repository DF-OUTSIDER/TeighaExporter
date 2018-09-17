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

#include "StdAfx.h"
#if defined(ODA_WINDOWS)
#pragma warning(push)
#pragma warning(disable : 4018) // signed/unsigned mismatch
#pragma warning(disable : 4146) // unary minus operator applied to unsigned type, result still unsigned

#ifdef ODA_USE_FTIME
// Use standard ftime() function available for all systems except WinCE
#include <sys/timeb.h>
#endif

#ifdef OD_HAVE_SYS_TYPES_FILE
#include <sys/types.h>
#endif

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#  include <windows.h>
#endif

#ifdef OD_HAVE_IO_FILE
#include <io.h>
#endif

#include <tchar.h>

#pragma warning(pop)
#endif

#ifdef OD_HAVE_SYS_STAT_FILE
#include <sys/stat.h>
#endif

#ifdef OD_HAVE_UNISTD_FILE
#include <unistd.h>
#endif

#include "OdaCommon.h"
#include "RxSystemServicesImpl.h"
#include "OdFileBuf.h"
#include "RxObjectImpl.h"
#include "RxDefs.h"
#include "RxDictionary.h"
#include "RxModule.h"
#include "ExGiRasterImage.h"
#include "FlatMemStream.h"
#include "ColorMapping.h"
#include "Gs/GsBMPDevice.h"
#include "Gs/Gs.h"
#include "OdCharMapper.h"

#ifdef ODA_LINKED_WITH_FRAMEWORK_FONDATION // gcc ... -framework Foundation
#include <CoreFoundation/CoreFoundation.h>
#endif

#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "DynamicLinker.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"


bool IsCompound(OdStreamBuf *pBuf);

RxSystemServicesImpl::RxSystemServicesImpl()
  : m_CodePageId(CP_UNDEFINED)
{
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(_WINRT)
  switch(::GetACP())
  {
  case 874:
    m_CodePageId = CP_ANSI_874; // Thai
    break;
  case 932:
    m_CodePageId = CP_ANSI_932; // Japanese
    break;
  case 936:
    m_CodePageId = CP_ANSI_936; // Chinese Simplified
    break;
  case 949:
    m_CodePageId = CP_ANSI_949; // Korean 
    break;
  case 950:
    m_CodePageId = CP_ANSI_950; // Chinese Traditional
    break;
  case 1200:
    m_CodePageId = CP_ANSI_1200; // CP_UNDEFINED
    break;
  case 1250:
    m_CodePageId = CP_ANSI_1250; // Eastern European
    break;
  case 1251:
    m_CodePageId = CP_ANSI_1251; // Cyrillic
    break;
  case 1252:
    m_CodePageId = CP_ANSI_1252; // US, Western Europe
    break;
  case 1253:
    m_CodePageId = CP_ANSI_1253; // Greek
    break;
  case 1254:
    m_CodePageId = CP_ANSI_1254; // Turkish
    break;
  case 1255:
    m_CodePageId = CP_ANSI_1255; // Hebrew
    break;
  case 1256:
    m_CodePageId = CP_ANSI_1256; // Arabic
    break;
  case 1257:
    m_CodePageId = CP_ANSI_1257; // Baltic - Estonian, Latvian and Lithuanian
    break;
  }
#elif defined(__APPLE__)
  m_CodePageId = CP_MACINTOSH; 
 #if defined(ODA_LINKED_WITH_FRAMEWORK_FONDATION)
  //#include <CoreFoundation/CoreFoundation.h> 
  //
  // via Carbon - use first language code in list
  // attention : linking with with Foundation framework is needed for it:
  //   gcc ... -framework Foundation
  //
  // http://davidtse916.wordpress.com/2007/11/19/changing-the-system-language-in-mac-os-x/     Changing the system language in Mac OS X
  // ? http://msdn.microsoft.com/en-us/library/system.text.encodinginfo.codepage.aspx#Y0
  
  CFArrayRef langs = CFLocaleCopyPreferredLanguages();
  ODA_ASSERT_ONCE(CFArrayGetCount(langs) > 0);
  CFStringRef langCode = static_cast<CFStringRef>(CFArrayGetValueAtIndex(langs, 0));
  // TODO CFRelease((CFTypeRef) ...);
  // next is localized by first language :
  //CFStringRef langName = CFLocaleCopyDisplayNameForPropertyValue(CFLocaleCopyCurrent(), kCFLocaleLanguageCode, langCode); 
  CFIndex length = ::CFStringGetLength(langCode);
  ODA_ASSERT_ONCE(length);
  if (!length)
    return;
  OdString sLangCode;
  // it works if differ to CFStringGetCharactersPtr and CFStringGetCharacters
  for (int idx = 0; idx < length; idx++) 
    sLangCode += (OdChar) CFStringGetCharacterAtIndex(langCode, idx);
  sLangCode.makeLower();
  if (sLangCode == L"en") // US, Western Europe
  {
    m_CodePageId = CP_ANSI_1252;
  }
  else if (sLangCode == L"ru") // Cyrillic 
    m_CodePageId = CP_ANSI_1251;
  else if (sLangCode == L"ja") // Japanese
    m_CodePageId = CP_ANSI_932;
  else if (   sLangCode == L"fr" // "Fr" 
           || sLangCode == L"de" // "Ge" German
           || sLangCode == L"es" // "Sp" 
           || sLangCode == L"it" // "It" 
           || sLangCode == L"pt-PT" // "Portu" 
           || sLangCode == L"sv" // "Sw" 
           || sLangCode == L"nb" // "No" 
           || sLangCode == L"da" // "Da" 
           || sLangCode == L"fi" // "Fi" 
           || sLangCode == L"nl") // "Du" Holland 
    m_CodePageId = CP_ANSI_1252; // Central European
  else if (   sLangCode == L"pl" // "Po" Polish
           || sLangCode == L"pt" // "Po" 
         //|| sLangCode == L"de" // "Ge" German
          )
    // Polish, Czech, Slovak, Hungarian, Slovene, Bosnian, Croatian, Serbian (Latin script), Romanian and Albanian
    // may also be used with the German
    m_CodePageId = CP_ANSI_1250;  // Eastern European
  else if (sLangCode == L"zh-Hans") // Chinese Simplified
    m_CodePageId = CP_ANSI_936;
  else if (sLangCode == L"zh-Hant") // Chinese Traditional
    m_CodePageId = CP_ANSI_950;
  else if (sLangCode == L"ko") // Korean 
    m_CodePageId = CP_ANSI_949; // CP_ANSI_1361 // CP_JOHAB
  else
  {
    ODA_FAIL_ONCE(); // TODO
  }
 #endif
#endif
}


OdStreamBufPtr RxSystemServicesImpl::createFile(
    const OdString& path,
    Oda::FileAccessMode access,
    Oda::FileShareMode share,
    Oda::FileCreationDisposition dispos)
{
  OdSmartPtr<OdBaseFileBuf> pFile = OdRxSystemServices::createFile(path, access, share, dispos);
  if(pFile.isNull()) {
    // MKU 19.02.2004   It was corrected after debugging with BORLANDC. It hangs on open() statement 
    //                  in case of using conditional operator: ? :.
    //
    // typedef OdSmartPtr<OdBaseFileBuf> OdBaseFileBufPtr;
    // OdBaseFileBufPtr pFile;
    // pFile = (nDesiredAccess == Oda::kFileRead ?
    //   OdBaseFileBufPtr(OdRdFileBuf::createObject()) :
    //   OdBaseFileBufPtr(OdWrFileBuf::createObject()));

    if(!path.isEmpty() && path[0])
    {
      if ((access & Oda::kFileWrite) != 0)
      {
        pFile = OdWrFileBuf::createObject();
      }
      else
      {
        pFile = OdRdFileBuf::createObject();
      }
      pFile->open(path, share, access, dispos);
    }
    else
    {
      throw OdError(eNoFileName);
    }
  }
  return OdStreamBufPtr(pFile);
}


template <class TChar>
inline bool isRxFSPath(const TChar* s) { return (s[2]==':' && s[0]=='r' && s[1]=='x'); }

struct OdString_Access : OdString {
  inline static bool isConvertedToWide(const OdString& ws) { return !((OdString_Access&)ws).isUnicodeNotInSync(); }
};

inline bool isRxFSPath(const OdString& path) {
  if(OdString_Access::isConvertedToWide(path))
    return isRxFSPath<OdChar>(path);
  return isRxFSPath<char>(path);
}

bool RxSystemServicesImpl::accessFile(const OdString& pcFilename, int mode)
{
  if(isRxFSPath(pcFilename))
    return OdRxSystemServices::accessFile(pcFilename, mode);

#if defined(ODA_WINDOWS) && !defined(OD_HAVE_GETFILEATTRIBUTES_FUNC)
  // SetErrorMode() function is used to avoid the message box
  // if there is no floppy disk in the floppy drive (CR 2122).
#if !defined(_WINRT)
  int oldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
#endif
  bool res = (_waccess(pcFilename,
    (GETBIT(mode, Oda::kFileRead)  ? 0x04 : 0x00) | 
    (GETBIT(mode, Oda::kFileWrite) ? 0x02 : 0x00)) == 0);
#if !defined(_WINRT)
  SetErrorMode(oldErrorMode);
#endif
  if (res)
  {
    struct _stat st = {0};
    if (_wstat(pcFilename,&st)==-1)
      return false;
    if (st.st_mode & _S_IFDIR)
      return false;
  }
  return res;
#else
#ifdef OD_CONVERT_UNICODETOUTF8
  OdAnsiCharArray dstBuf;
  int lenStr = pcFilename.getLength();
  dstBuf.reserve(lenStr*2);
  OdCharMapper::unicodeToUtf8(pcFilename.c_str(), lenStr, dstBuf);
  
  const char* fName = (const char*)dstBuf.asArrayPtr();
#else
  
  const char* fName = (const char*)pcFilename;
#endif

  bool res(false);

#ifdef OD_HAVE_GETFILEATTRIBUTES_FUNC
  DWORD ftyp = GetFileAttributes( (LPCWSTR)pcFilename.c_str() );
  if (ftyp != 0xFFFFFFFF)
  {
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
      res = false;
    else
      res = true;
  }

#else
  res = (access(fName,
    (GETBIT(mode, Oda::kFileRead)  ? 0x04 : 0x00) | 
    (GETBIT(mode, Oda::kFileWrite) ? 0x02 : 0x00)) == 0);
  if (res)
  {
    struct stat st = {0};
    if (stat(fName, &st)==-1)
      return false;
    if (S_ISDIR(st.st_mode))
      return false;
  }

#endif
  return res;

#endif
}


#ifdef OD_HAVE_WSTAT_FUNC

OdInt64 RxSystemServicesImpl::getFileCTime(const OdString& name)
{
  struct _stat st;
  if (_wstat(name, &st)) return -1;
  return st.st_ctime;
}

OdInt64 RxSystemServicesImpl::getFileMTime(const OdString& name)
{
  struct _stat st;
  if (_wstat(name, &st)) return -1;
  return st.st_mtime;
}

#else // OD_HAVE_WSTAT_FUNC
OdInt64 RxSystemServicesImpl::getFileCTime(const OdString&)
{
  return (-1);
}

OdInt64 RxSystemServicesImpl::getFileMTime(const OdString&)
{
  return (-1);
}
#endif  // OD_HAVE_WSTAT_FUNC

OdInt64 RxSystemServicesImpl::getFileSize(const OdString& name)
{
#ifdef OD_HAVE_FINDFIRSTFILE_FUNC
  WIN32_FIND_DATA fd;
  if (HANDLE hf = ::FindFirstFile(OdString(name), &fd))
  {
    FindClose(hf);
    return (OdInt64(fd.nFileSizeLow) | (OdInt64(fd.nFileSizeHigh) << 32));
  }
  return OdInt64(-1);
#else
  struct stat st;
  OdString tmp(name);
  if (stat(static_cast<const char*>(tmp), &st)) return OdInt64(-1);
  return OdInt64(st.st_size);
#endif
}

bool IsCompound(OdStreamBuf *pBuf)
{
  static OdUInt8 docID[8]={ 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1 };  
  OdUInt8  b8[8];
  int   i;

  pBuf->seek(128, OdDb::kSeekFromStart);
  pBuf->getBytes(b8, 8);
  pBuf->seek(0, OdDb::kSeekFromStart);
  for (i=0; i<8; i++)
  {
    if (b8[i] != docID[i]) 
    {
      return false;
    }
  }
  return true;
}


OdString RxSystemServicesImpl::formatMessage(unsigned int code, va_list* argList)
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

OdCodePageId RxSystemServicesImpl::systemCodePage() const
{
  return m_CodePageId;
}

void RxSystemServicesImpl::setSystemCodePage(OdCodePageId id)
{
  m_CodePageId = id;
}

OdResult RxSystemServicesImpl::getEnvVar(const OdString &varName, OdString &value)
{
#ifndef OD_NO_GETENV

#ifdef _WINRT
  return eNotImplemented;
#endif

#if defined(ODA_WINDOWS)

  OdChar *pStrRes = (OdChar*)_tgetenv(varName);
  if (pStrRes == NULL)
  {
    return eKeyNotFound;
  }

  value = pStrRes;

#else

  int lenStr = varName.getLength();
  OdAnsiCharArray dstBuf;
  dstBuf.reserve(lenStr * 4); // UTF-8 can use up to 4 bytes per character
  OdCharMapper::unicodeToUtf8(varName.c_str(), lenStr, dstBuf);

  char *envVal = getenv(dstBuf.asArrayPtr());
  if (envVal == NULL)
  {
    return eKeyNotFound;
  }

  OdAnsiString asCmd(envVal);
  OdCharArray buf;
  OdCharMapper::utf8ToUnicode(asCmd.c_str(), asCmd.getLength(), buf);
  value = OdString(buf.getPtr(), buf.size() - 1);

#endif

  return eOk;

#else
  return eDisabledInConfig;
#endif
}

OdResult RxSystemServicesImpl::setEnvVar(const OdString &varName, const OdString &newValue)
{
#ifndef OD_NO_GETENV

#ifdef _WINRT
  return eNotImplemented;
#endif

  OdString strVal = varName + OD_T("=") + newValue;

#if defined(ODA_WINDOWS)

  if (_tputenv(strVal) != 0)
  {
    return eCantSetEnvVar;
  }

#else

  int lenStr = strVal.getLength();
  OdAnsiCharArray dstBuf;
  dstBuf.reserve(lenStr * 4); // UTF-8 can use up to 4 bytes per character
  OdCharMapper::unicodeToUtf8(strVal.c_str(), lenStr, dstBuf);
  char* mStrVal = new char[lenStr * 4]; // according to description of putenv() this array shouldn't be deleted as it becomes env var.
  memcpy(mStrVal, (char*)dstBuf.asArrayPtr(), lenStr * 4);
  if (putenv(mStrVal) != 0)
  {
    return eCantSetEnvVar;
  }

#endif

  return eOk;

#else
  return eDisabledInConfig;
#endif
}
