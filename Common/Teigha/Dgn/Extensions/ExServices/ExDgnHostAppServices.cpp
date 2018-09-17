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
#include "Gs/Gs.h"
#include "ExDgnHostAppServices.h"
#include "Gi/TtfDescriptor.h"
#include "RxObjectImpl.h"

//----------------------------------------------------------
//
// Support function
//
//----------------------------------------------------------
void oddgPrintConsoleString(const wchar_t* fmt, ...)
{
  va_list argList;
  va_start(argList, fmt);
  OdString s;
  s.formatV(fmt, argList);
  va_end(argList);
#if (defined(_WIN32) || defined(_WIN64)) && !defined(_WIN32_WCE) && !defined(_WINRT)
  DWORD dw;
  HANDLE o = ::GetStdHandle(STD_OUTPUT_HANDLE);
  if (::GetFileType(o) == FILE_TYPE_CHAR)
    WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
  else
    WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
#else
  printf("%ls", s.c_str());
#endif
}

//----------------------------------------------------------
//
// OdExDgnHostAppServices
//
//----------------------------------------------------------
OdExDgnHostAppServices::OdExDgnHostAppServices() 
{
}

bool OdExDgnHostAppServices::ttfFileNameByDescriptor(const OdTtfDescriptor& descr, OdString& fileName)
{
#if defined(_MSC_VER) && defined(_WIN32) && !defined(_WIN32_WCE) && !defined(_WINRT)
  OdString sNonExactMatched;

  OdString sFaceName = descr.typeface();

  OSVERSIONINFO os;
  os.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
  ::GetVersionEx(&os);
  OdString sName;
  sName.format(OD_T("Software\\Microsoft\\Windows%ls\\CurrentVersion\\"),
    (os.dwPlatformId & VER_PLATFORM_WIN32_NT)!=0 ? OD_T(" nt") : OD_T(""));

  LONG nRes;
  HKEY hFontSubstitutes;
  
  DWORD nDataSize;
  nRes = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sName + OD_T("FontSubstitutes\\"), 0, KEY_READ, &hFontSubstitutes);
  if (hFontSubstitutes)
  {
    OdString sValueName;
    sValueName.format(OD_T("%ls,%d"), descr.typeface().c_str(), descr.charSet());
    nRes = ::RegQueryValueEx(hFontSubstitutes, sValueName, NULL, NULL, NULL, &nDataSize);
    if(nRes == eOk)
    {
      char* lpData = new char[nDataSize];
      nRes = ::RegQueryValueEx(hFontSubstitutes, sValueName, NULL, NULL, (LPBYTE)lpData, &nDataSize);
      sFaceName = (LPCTSTR)lpData;
      delete [] lpData;
      int n = sFaceName.find(',');
      if(n>0)
      {
        sFaceName = sFaceName.left(n);
      }
    }
    nRes = ::RegCloseKey(hFontSubstitutes);
  }

  HKEY hFonts;
  ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sName + OD_T("Fonts\\"), 0, KEY_READ, &hFonts);
  if (hFonts)
  {
    DWORD nIndex = 0;
    DWORD nValNameSize = 20;
    nDataSize = odmax(fileName.getAllocLength(), 20);
    int n_BT = sFaceName.replace(OD_T(" BT"), OD_T(" "));
    for(;;)
    {
      OdString sValueName;
      do
      {
        TCHAR* lpValName = new TCHAR[nValNameSize];
        LPBYTE lpData = new BYTE[nDataSize];
        nRes = ::RegEnumValue(hFonts, nIndex, lpValName, &nValNameSize, NULL, NULL, lpData, &nDataSize);
        if (nRes == eInvalidInput)
        {
          nValNameSize += 20;
        }
        else if (nRes == eOk)
        {
          sValueName = lpValName;
          fileName = (LPCTSTR)lpData;
        }
        else 
        {
          sValueName.empty();
          fileName.empty();
        }
        delete [] lpValName;
        delete [] lpData;
      }
      while (nRes == eInvalidInput);

      if (nRes == eOk)
      {
        ++nIndex;
        nValNameSize = sValueName.getAllocLength();
        nDataSize = fileName.getAllocLength();
        if(sValueName.replace(OD_T("(TrueType)"), OD_T(""))) // is TrueType font?
        {
          if(sValueName.replace(OD_T(" BT"), OD_T(""))==n_BT)
          {
            bool bBold = false;
            if(n_BT)
            {
              sValueName.replace(OD_T(" Extra Bold "), OD_T(" XBd "));
            }
            else
            {
              if(sValueName.find(OD_T(" Extra Bold "))==-1)
                bBold = (sValueName.replace(OD_T(" Bold "), OD_T(" "))!=0);
            }
            
            bool bItalic = (sValueName.replace(OD_T(" Italic "), OD_T(" "))!=0);
            sValueName.remove(' ');
            sFaceName.remove(' ');
            if(sValueName==sFaceName)
            {
              sNonExactMatched = fileName;
              if(descr.isBold()==bBold && descr.isItalic()==bItalic)
                break;
            }
            if ( sValueName.find((sFaceName+L"&").c_str()) != -1 ||
              sValueName.find((L"&"+sFaceName).c_str()) != -1 )
            {
              sNonExactMatched = fileName;
              break;
            }
          }
        }
      }
      else
      {
        fileName = sNonExactMatched;
        break;
      }
    }
    ::RegCloseKey(hFonts);
  }
  if ( !fileName.isEmpty() )
  {
    if ( fileName.find(L'\\') == -1 )
    {
      OdString testpath;
      int nLen = ::GetWindowsDirectory(NULL, 0);
      if(nLen)
      {
        OdChar* szWinDir = testpath.getBufferSetLength(nLen);
        ::GetWindowsDirectoryW((LPWSTR)szWinDir, nLen);
        testpath.releaseBuffer();

        testpath += OD_T("\\fonts\\");
        fileName = testpath + fileName;
      }
    }
  }
  return (!fileName.isEmpty());
#else
  if( getDbHostAppServices() )
  {
    return getDbHostAppServices()->ttfFileNameByDescriptor(descr,fileName);
  }
  else
  {
    return false;
  }
#endif
}

OdDbHostAppProgressMeter* OdExDgnHostAppServices::newProgressMeter()
{
  return &m_progressMeter;
}

void OdExDgnHostAppServices::releaseProgressMeter(OdDbHostAppProgressMeter* pProgressMeter)
{
}

//----------------------------------------------------------
//
// OdExDgnHostAppServices
//
//----------------------------------------------------------
OdExDgnHostAppProgressMeter::OdExDgnHostAppProgressMeter()
  : m_MeterLimit(0)
  , m_MeterCurrent(0)
  , m_MeterOld(0)
  , m_disableOutput(false)
{
}

void OdExDgnHostAppProgressMeter::start(const OdString& displayString)
{
  if (!m_disableOutput)
  {
#ifndef _WIN32_WCE
    oddgPrintConsoleString(L"%ls------- Started %ls\n", m_Prefix.c_str(), displayString.c_str());
#endif
  }
}

void OdExDgnHostAppProgressMeter::stop()
{
  if (!m_disableOutput)
  {
#ifndef _WIN32_WCE
    oddgPrintConsoleString(L"%ls------- Stopped\n", m_Prefix.c_str());
#endif
  }
}

void OdExDgnHostAppProgressMeter::meterProgress()
{
  m_MeterCurrent++;
  if (!m_disableOutput)
  {
    double  f1, f2;

    f1 = (double)m_MeterCurrent / m_MeterLimit * 100;
    f2 = (double)m_MeterOld / m_MeterLimit * 100;
    if ((f1 - f2) > 0.7)
    {
#if !defined(_WIN32_WCE) && !defined(__BORLANDC__) // Causing access violation on BB
      oddgPrintConsoleString(L"%lsProgress: %2.2lf%%\n", m_Prefix.c_str(), f1);
#endif
	  m_MeterOld = m_MeterCurrent;
    }
  }
}

void OdExDgnHostAppProgressMeter::setLimit(int max)
{
  m_MeterLimit = max;
  m_MeterCurrent = 0;
  m_MeterOld = 0;
  if (!m_disableOutput)
  {
#ifndef _WIN32_WCE
    oddgPrintConsoleString(L"%lsMeter Limit: %d\n", m_Prefix.c_str(), max);
#endif
  }
}
