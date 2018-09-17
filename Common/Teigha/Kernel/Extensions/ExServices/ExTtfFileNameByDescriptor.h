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

#ifndef ODEX_TTF_FILE_NAME_BY_DESCRIPTO_H_
#define ODEX_TTF_FILE_NAME_BY_DESCRIPTO_H_

#include "Gi/TtfDescriptor.h"

#define STL_USING_VECTOR
#include "OdaSTL.h"

inline bool exTtfFileNameByDescriptor(const OdTtfDescriptor& descr, OdString& fileName)
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
    if(nRes == ERROR_SUCCESS)
    {
      LPBYTE lpData = (LPBYTE)malloc(nDataSize);
      nRes = ::RegQueryValueEx(hFontSubstitutes, sValueName, NULL, NULL, lpData, &nDataSize);
      sFaceName = (LPCTSTR)lpData;
      free(lpData);
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
    nDataSize = odmax(fileName.getLength(), 20);
    int n_BT = sFaceName.replace(OD_T(" BT"), OD_T(" "));
    for(;;)
    {
      OdString sValueName;
      do
      {
        std::vector<TCHAR> lpValName(nValNameSize);
        std::vector<BYTE> lpData(nDataSize);
        nRes = ::RegEnumValue(hFonts, nIndex, &lpValName[0], &nValNameSize, NULL, NULL, &lpData[0], &nDataSize);
        if (nRes == ERROR_MORE_DATA)
        {
          nValNameSize += 20;
        }
        else if (nRes == ERROR_SUCCESS)
        {
          sValueName = &lpValName[0];
          fileName = (LPCTSTR)&lpData[0];
        }
        else 
        {
          sValueName.empty();
          fileName.empty();
        }
      }
      while (nRes == ERROR_MORE_DATA);

      if (nRes == ERROR_SUCCESS)
      {
        ++nIndex;
        nValNameSize = sValueName.getLength();
        nDataSize = fileName.getLength();
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
        nLen = ::GetWindowsDirectoryW((LPWSTR)szWinDir, nLen);
        testpath.releaseBuffer();

        testpath += OD_T("\\fonts\\");
        fileName = testpath + fileName;
      }
    }
  }
  if ( !fileName.isEmpty() )
    return true;
#endif
  return false;
}

#endif // ODEX_TTF_FILE_NAME_BY_DESCRIPTO_H_
