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

#if defined(_MSC_VER) && defined(_WIN32)
#include <windows.h>
#endif

#define STL_USING_IOSTREAM
#define STL_USING_VECTOR
#include "OdaSTL.h"

#include "OdaCommon.h"
#include "OdToolKit.h"
#include "OdString.h"
#include "ExHostAppServices.h"
#include "Gs/Gs.h"
#include "DbAudit.h"
#include "Gi/TtfDescriptor.h"
#include "OdStreamBuf.h"
#include "RxDictionary.h"
#include "OdFontServices.h"
#include "OdCharMapper.h"
#include "ExTtfFileNameByDescriptor.h"

#define  STD(a)  std:: a

/*
// DN: temporary. To be removed...
#define ODRGB(r,g,b) {r,g,b,0}

const OdGsColor ExHostAppServices::dark_palette[] =
{
  ODRGB(  0,   0,   0),   // black //
  ODRGB(255,   0,   0),   // red //
  ODRGB(255, 255,   0),   // yellow //
  ODRGB(  0, 255,   0),   // green //
  ODRGB(  0, 255, 255),   // cyan //
  ODRGB(  0,   0, 255),   // blue //
  ODRGB(255,   0, 255),   // magenta //
  ODRGB(255, 255, 255),   // white //
  ODRGB(152, 152, 152),   // gray -- an educated guess //
  ODRGB(192, 192, 192),   // light gray -- an educated guess //
  ODRGB(255,   0,   0),ODRGB(255, 128, 128),ODRGB(166,   0,   0),
  ODRGB(166,  83,  83),ODRGB(128,   0,   0),ODRGB(128,  64,  64),ODRGB( 76,   0,   0),
  ODRGB( 76,  38,  38),ODRGB( 38,   0,   0),ODRGB( 38,  19,  19),ODRGB(255,  64,   0),
  ODRGB(255, 159, 128),ODRGB(166,  41,   0),ODRGB(166, 104,  83),ODRGB(128,  32,   0),
  ODRGB(128,  80,  64),ODRGB( 76,  19,   0),ODRGB( 76,  48,  38),ODRGB( 38,  10,   0),
  ODRGB( 38,  24,  19),ODRGB(255, 128,   0),ODRGB(255, 191, 128),ODRGB(166,  83,   0),
  ODRGB(166, 124,  83),ODRGB(128,  64,   0),ODRGB(128,  96,  64),ODRGB( 76,  38,   0),
  ODRGB( 76,  57,  38),ODRGB( 38,  19,   0),ODRGB( 38,  29,  19),ODRGB(255, 191,   0),
  ODRGB(255, 223, 128),ODRGB(166, 124,   0),ODRGB(166, 145,  83),ODRGB(128,  96,   0),
  ODRGB(128, 112,  64),ODRGB( 76,  57,   0),ODRGB( 76,  67,  38),ODRGB( 38,  29,   0),
  ODRGB( 38,  33,  19),ODRGB(255, 255,   0),ODRGB(255, 255, 128),ODRGB(166, 166,   0),
  ODRGB(166, 166,  83),ODRGB(128, 128,   0),ODRGB(128, 128,  64),ODRGB( 76,  76,   0),
  ODRGB( 76,  76,  38),ODRGB( 38,  38,   0),ODRGB( 38,  38,  19),ODRGB(191, 255,   0),
  ODRGB(223, 255, 128),ODRGB(124, 166,   0),ODRGB(145, 166,  83),ODRGB( 96, 128,   0),
  ODRGB(112, 128,  64),ODRGB( 57,  76,   0),ODRGB( 67,  76,  38),ODRGB( 29,  38,   0),
  ODRGB( 33,  38,  19),ODRGB(128, 255,   0),ODRGB(191, 255, 128),ODRGB( 83, 166,   0),
  ODRGB(124, 166,  83),ODRGB( 64, 128,   0),ODRGB( 96, 128,  64),ODRGB( 38,  76,   0),
  ODRGB( 57,  76,  38),ODRGB( 19,  38,   0),ODRGB( 29,  38,  19),ODRGB( 64, 255,   0),
  ODRGB(159, 255, 128),ODRGB( 41, 166,   0),ODRGB(104, 166,  83),ODRGB( 32, 128,   0),
  ODRGB( 80, 128,  64),ODRGB( 19,  76,   0),ODRGB( 48,  76,  38),ODRGB( 10,  38,   0),
  ODRGB( 24,  38,  19),ODRGB(  0, 255,   0),ODRGB(128, 255, 128),ODRGB(  0, 166,   0),
  ODRGB( 83, 166,  83),ODRGB(  0, 128,   0),ODRGB( 64, 128,  64),ODRGB(  0,  76,   0),
  ODRGB( 38,  76,  38),ODRGB(  0,  38,   0),ODRGB( 19,  38,  19),ODRGB(  0, 255,  64),
  ODRGB(128, 255, 159),ODRGB(  0, 166,  41),ODRGB( 83, 166, 104),ODRGB(  0, 128,  32),
  ODRGB( 64, 128,  80),ODRGB(  0,  76,  19),ODRGB( 38,  76,  48),ODRGB(  0,  38,  10),
  ODRGB( 19,  38,  24),ODRGB(  0, 255, 128),ODRGB(128, 255, 191),ODRGB(  0, 166,  83),
  ODRGB( 83, 166, 124),ODRGB(  0, 128,  64),ODRGB( 64, 128,  96),ODRGB(  0,  76,  38),
  ODRGB( 38,  76,  57),ODRGB(  0,  38,  19),ODRGB( 19,  38,  29),ODRGB(  0, 255, 191),
  ODRGB(128, 255, 223),ODRGB(  0, 166, 124),ODRGB( 83, 166, 145),ODRGB(  0, 128,  96),
  ODRGB( 64, 128, 112),ODRGB(  0,  76,  57),ODRGB( 38,  76,  67),ODRGB(  0,  38,  29),
  ODRGB( 19,  38,  33),ODRGB(  0, 255, 255),ODRGB(128, 255, 255),ODRGB(  0, 166, 166),
  ODRGB( 83, 166, 166),ODRGB(  0, 128, 128),ODRGB( 64, 128, 128),ODRGB(  0,  76,  76),
  ODRGB( 38,  76,  76),ODRGB(  0,  38,  38),ODRGB( 19,  38,  38),ODRGB(  0, 191, 255),
  ODRGB(128, 223, 255),ODRGB(  0, 124, 166),ODRGB( 83, 145, 166),ODRGB(  0,  96, 128),
  ODRGB( 64, 112, 128),ODRGB(  0,  57,  76),ODRGB( 38,  67,  76),ODRGB(  0,  29,  38),
  ODRGB( 19,  33,  38),ODRGB(  0, 128, 255),ODRGB(128, 191, 255),ODRGB(  0,  83, 166),
  ODRGB( 83, 124, 166),ODRGB(  0,  64, 128),ODRGB( 64,  96, 128),ODRGB(  0,  38,  76),
  ODRGB( 38,  57,  76),ODRGB(  0,  19,  38),ODRGB( 19,  29,  38),ODRGB(  0,  64, 255),
  ODRGB(128, 159, 255),ODRGB(  0,  41, 166),ODRGB( 83, 104, 166),ODRGB(  0,  32, 128),
  ODRGB( 64,  80, 128),ODRGB(  0,  19,  76),ODRGB( 38,  48,  76),ODRGB(  0,  10,  38),
  ODRGB( 19,  24,  38),ODRGB(  0,   0, 255),ODRGB(128, 128, 255),ODRGB(  0,   0, 166),
  ODRGB( 83,  83, 166),ODRGB(  0,   0, 128),ODRGB( 64,  64, 128),ODRGB(  0,   0,  76),
  ODRGB( 38,  38,  76),ODRGB(  0,   0,  38),ODRGB( 19,  19,  38),ODRGB( 64,   0, 255),
  ODRGB(159, 128, 255),ODRGB( 41,   0, 166),ODRGB(104,  83, 166),ODRGB( 32,   0, 128),
  ODRGB( 80,  64, 128),ODRGB( 19,   0,  76),ODRGB( 48,  38,  76),ODRGB( 10,   0,  38),
  ODRGB( 24,  19,  38),ODRGB(128,   0, 255),ODRGB(191, 128, 255),ODRGB( 83,   0, 166),
  ODRGB(124,  83, 166),ODRGB( 64,   0, 128),ODRGB( 96,  64, 128),ODRGB( 38,   0,  76),
  ODRGB( 57,  38,  76),ODRGB( 19,   0,  38),ODRGB( 29,  19,  38),ODRGB(191,   0, 255),
  ODRGB(223, 128, 255),ODRGB(124,   0, 166),ODRGB(145,  83, 166),ODRGB( 96,   0, 128),
  ODRGB(112,  64, 128),ODRGB( 57,   0,  76),ODRGB( 67,  38,  76),ODRGB( 29,   0,  38),
  ODRGB( 33,  19,  38),ODRGB(255,   0, 255),ODRGB(255, 128, 255),ODRGB(166,   0, 166),
  ODRGB(166,  83, 166),ODRGB(128,   0, 128),ODRGB(128,  64, 128),ODRGB( 76,   0,  76),
  ODRGB( 76,  38,  76),ODRGB( 38,   0,  38),ODRGB( 38,  19,  38),ODRGB(255,   0, 191),
  ODRGB(255, 128, 223),ODRGB(166,   0, 124),ODRGB(166,  83, 145),ODRGB(128,   0,  96),
  ODRGB(128,  64, 112),ODRGB( 76,   0,  57),ODRGB( 76,  38,  67),ODRGB( 38,   0,  29),
  ODRGB( 38,  19,  33),ODRGB(255,   0, 128),ODRGB(255, 128, 191),ODRGB(166,   0,  83),
  ODRGB(166,  83, 124),ODRGB(128,   0,  64),ODRGB(128,  64,  96),ODRGB( 76,   0,  38),
  ODRGB( 76,  38,  57),ODRGB( 38,   0,  19),ODRGB( 38,  19,  29),ODRGB(255,   0,  64),
  ODRGB(255, 128, 159),ODRGB(166,   0,  41),ODRGB(166,  83, 104),ODRGB(128,   0,  32),
  ODRGB(128,  64,  80),ODRGB( 76,   0,  19),ODRGB( 76,  38,  48),ODRGB( 38,   0,  10),
  ODRGB( 38,  19,  24),ODRGB( 84,  84,  84),ODRGB(118, 118, 118),ODRGB(152, 152, 152),
  ODRGB(187, 187, 187),ODRGB(221, 221, 221),ODRGB(255, 255, 255)
};
*/
ExHostAppServices::ExHostAppServices() 
                 : m_disableOutput(false)
//                 , m_bSysFontCollected(false)
{
}

OdHatchPatternManager* ExHostAppServices::patternManager()
{
  if(m_patternManager.isNull())
  {
    // dna: use odrxSafeCreateObject() for no linking to TD_DbFull
    m_patternManager = odrxSafeCreateObject<OdHatchPatternManager>("OdHatchPatternManager", OdDbEntitiesAppName);
    m_patternManager->setApplicationService(this);
  }
  return m_patternManager.get();
}

OdDbHostAppProgressMeter* ExHostAppServices::newProgressMeter()
{
  return this;
}

void ExHostAppServices::releaseProgressMeter(OdDbHostAppProgressMeter* /*pMeter*/)
{
}

// MKU 06/17/05 - This optimization is obsolete. But it can be reason of bug 
//                  when old database is found by name instead of creating anew.
/*
void ExHostAppServices::DwgCollection::goodbye(const OdDbDatabase* pDb)
{
  std::map<OdString, OdDbDatabase*>::iterator iter = m_loadedDwgs.begin(), end = m_loadedDwgs.end();
  while(iter!=end)
  {
    if(iter->second==pDb)
    {
      m_loadedDwgs.erase(iter);
      break;
    }
    ++iter;
  }
}

OdDbDatabase* ExHostAppServices::DwgCollection::lookUp(const OdString& keyFileName)
{
  std::map<OdString, OdDbDatabase*>::iterator iter = m_loadedDwgs.find(keyFileName);
  if(iter!=m_loadedDwgs.end())
  {
    return iter->second;
  }
  return 0;
}

void ExHostAppServices::DwgCollection::add(OdDbDatabase* pDb)
{
  ODA_ASSERT(lookUp(pDb->getFilename())==0);
  m_loadedDwgs.insert(std::map<OdString, OdDbDatabase*>::value_type(pDb->getFilename(), pDb));
  pDb->addReactor(this);
}
*/
OdDbDatabasePtr ExHostAppServices::readFile(const OdString& fileName,
    bool bAllowCPConversion, bool bPartial, Oda::FileShareMode shmode, const OdPassword& password)
{
  OdDbDatabasePtr pRes;// = m_dwgCollection.lookUp(fileName);
  //if(pRes.isNull())
  {
    pRes = OdDbHostAppServices2::readFile(fileName, bAllowCPConversion, bPartial, shmode, password);
    //m_dwgCollection.add(pRes.get());
  }
  return pRes;
}


void ExHostAppServices::start(const OdString& displayString)
{
  if (!m_disableOutput)
  {
    odPrintConsoleString(OD_T("%ls------- Started %ls\n"), m_Prefix.c_str(), displayString.c_str());
  }
}

void ExHostAppServices::stop()
{
  if (!m_disableOutput)
  {
    odPrintConsoleString(OD_T("%ls------- Stopped\n"), m_Prefix.c_str());
  }
}

void ExHostAppServices::meterProgress()
{
  m_MeterCurrent++;
  if (!m_disableOutput)
  {
    double  f1, f2;

    f1 = (double)m_MeterCurrent / m_MeterLimit * 100;
    f2 = (double)m_MeterOld / m_MeterLimit * 100;
    if ((f1 - f2) > 0.7)
    {
      odPrintConsoleString(OD_T("%lsProgress: %2.2lf%%\n"), m_Prefix.c_str(), f1);
      m_MeterOld = m_MeterCurrent;
    }
  }
}

void ExHostAppServices::setLimit(int max)
{
  m_MeterLimit = max;
  m_MeterCurrent = 0;
  m_MeterOld = 0;
  if (!m_disableOutput)
  {
    odPrintConsoleString(OD_T("%lsMeter Limit: %d\n"), m_Prefix.c_str(), max);
  }
}

#if 0
// Moved to OdDbBaseHostAppServices
bool ExHostAppServices::ttfFileNameByDescriptor(const OdTtfDescriptor& descr, OdString& fileName)
{
  OdFontServicesPtr fontSvc = odrxSysRegistry()->getAt(ODDB_FONT_SERVICES);
  if ( !fontSvc.isNull() )
  {
    return fontSvc->ttfFileNameByDescriptor(descr, fileName, this);
  }
  return false;

//#if 0
  // Implementation was moved into DbFontServices
  // Commented source text is stay as sample.
  if ( descr.typeface().isEmpty() )
  {
    TD_AUTOLOCK(m_TTFMapMutex);
    OdString fullFileName = findFile(fileName);
    if ( !fullFileName.isEmpty() )
    {
      OdFontServicesPtr fontSvc = odrxSysRegistry()->getAt(ODDB_FONT_SERVICES);
      if ( !fontSvc.isNull() )
      {
        OdTtfDescriptor _descr = descr;
        if ( fontSvc->getTTFParamFromFile(fullFileName, _descr) )
        { 
          _descr.setFileName(fullFileName);
          m_mapTTF.insertAt(0, _descr); // fonts in drawing folder have upper priority // m_mapTTF.append(_descr);
        }   
      }
      return false;
    }
  }

  OdInt32 szMap = m_mapTTF.size();
  if ( szMap > 0 )
  {
    TD_AUTOLOCK(m_TTFMapMutex);
    for ( OdInt32 i = 0; i < szMap; i++ )
    {
      OdString tpFace = m_mapTTF[i].typeface();
      if ( tpFace.find(L" & ") != -1 )
      {
        // for example by next : "Times New" may be found instead "Times" :
        // //if ( tpFace.find(descr.typeface()) != -1 )
        //
        tpFace = L" & " + tpFace + L" & ";
        OdString tpFaceToSearch = L" & " + descr.typeface() + L" & ";
        if (tpFace.makeLower().find(tpFaceToSearch.makeLower()) != -1)
        {
          fileName = m_mapTTF[i].fileName();
          return true;
        }
      } else    
      {
        if ( !descr.typeface().iCompare(m_mapTTF[i].typeface()) && 
          descr.isBold() ==  m_mapTTF[i].isBold() &&
          descr.isItalic() ==  m_mapTTF[i].isItalic() )
        {
          fileName = m_mapTTF[i].fileName();
          return true;
        }
      }
    }
    if ( descr.isBold() || descr.isItalic() )
    {
      for ( OdInt32 i = 0; i < szMap; i++ )
      {
        if ( !descr.typeface().iCompare( m_mapTTF[i].typeface()) )
        {
          fileName = m_mapTTF[i].fileName();
          return true;
        }
      }
    }
  }

#if defined(ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER)
  if ( !fileName.isEmpty() && fileName.right(3) == L"*.*" )
  {
    OdString sPath = fileName;

    OdStringArray aFiles;
    unsigned int index = 0;

    sPath.replace(L'\\', L'/');
    int pos = sPath.reverseFind(L'/');
    sPath = sPath.left(pos);

    collectFilePathsInDirectory(aFiles, sPath, L"*.ttc");
    collectFilePathsInDirectory(aFiles, sPath, L"*.otf");
    collectFilePathsInDirectory(aFiles, sPath, L"*.ttf");

    if (aFiles.size())
    {
      OdTtfDescriptor dscEmpty;
      for (index = 0; index < aFiles.size(); index++)
      {
        sPath = aFiles[index];
        ttfFileNameByDescriptor(dscEmpty, sPath); // now collect cache only
      }
    }
    return true;
  }

  // Collect system fonts
  if ( !m_bSysFontCollected )
  {
    OdStringArray aDirs;
    getSystemFontFolders(aDirs);

    OdStringArray aFiles;
    unsigned int index = 0;
    OdString sPath = aDirs[index];

    for (; index < aDirs.size(); index++)
    {
      sPath = aDirs[index];
      collectFilePathsInDirectory(aFiles, sPath, L"*.ttc");
      collectFilePathsInDirectory(aFiles, sPath, L"*.otf");
      collectFilePathsInDirectory(aFiles, sPath, L"*.ttf");
    }
    m_bSysFontCollected = true;
    if (aFiles.size())
    {
      OdTtfDescriptor dscEmpty;
      for (index = 0; index < aFiles.size(); index++)
      {
        sPath = aFiles[index];
        ttfFileNameByDescriptor(dscEmpty, sPath); // now collect cache only
      }
      return ttfFileNameByDescriptor(descr, fileName);
    }
  }
#endif

#if !defined(ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER)
  return exTtfFileNameByDescriptor(descr, fileName);
#else
  return false;
#endif
// was moved into exTtfFileNameByDescriptor.h
//#if defined(_MSC_VER) && defined(_WIN32) && !defined(_WIN32_WCE) && !defined(ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER) && !defined(_WINRT)
//  OdString sNonExactMatched;
//
//  OdString sFaceName = descr.typeface();
//
//  OSVERSIONINFO os;
//  os.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
//  ::GetVersionEx(&os);
//  OdString sName;
//  sName.format(OD_T("Software\\Microsoft\\Windows%ls\\CurrentVersion\\"),
//    (os.dwPlatformId & VER_PLATFORM_WIN32_NT)!=0 ? OD_T(" nt") : OD_T(""));
//
//  LONG nRes;
//  HKEY hFontSubstitutes;
//  
//  DWORD nDataSize;
//  nRes = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sName + OD_T("FontSubstitutes\\"), 0, KEY_READ, &hFontSubstitutes);
//  if (hFontSubstitutes)
//  {
//    OdString sValueName;
//    sValueName.format(OD_T("%ls,%d"), descr.typeface().c_str(), descr.charSet());
//    nRes = ::RegQueryValueEx(hFontSubstitutes, sValueName, NULL, NULL, NULL, &nDataSize);
//    if(nRes == ERROR_SUCCESS)
//    {
//      LPBYTE lpData = (LPBYTE)malloc(nDataSize);
//      nRes = ::RegQueryValueEx(hFontSubstitutes, sValueName, NULL, NULL, lpData, &nDataSize);
//      sFaceName = (LPCTSTR)lpData;
//      free(lpData);
//      int n = sFaceName.find(',');
//      if(n>0)
//      {
//        sFaceName = sFaceName.left(n);
//      }
//    }
//    nRes = ::RegCloseKey(hFontSubstitutes);
//  }
//
//  HKEY hFonts;
//  ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sName + OD_T("Fonts\\"), 0, KEY_READ, &hFonts);
//  if (hFonts)
//  {
//    DWORD nIndex = 0;
//    DWORD nValNameSize = 20;
//    nDataSize = odmax(fileName.getLength(), 20);
//    int n_BT = sFaceName.replace(OD_T(" BT"), OD_T(" "));
//    for(;;)
//    {
//      OdString sValueName;
//      do
//      {
//        std::vector<TCHAR> lpValName(nValNameSize);
//        std::vector<BYTE> lpData(nDataSize);
//        nRes = ::RegEnumValue(hFonts, nIndex, &lpValName[0], &nValNameSize, NULL, NULL, &lpData[0], &nDataSize);
//        if (nRes == ERROR_MORE_DATA)
//        {
//          nValNameSize += 20;
//        }
//        else if (nRes == ERROR_SUCCESS)
//        {
//          sValueName = &lpValName[0];
//          fileName = (LPCTSTR)&lpData[0];
//        }
//        else 
//        {
//          sValueName.empty();
//          fileName.empty();
//        }
//      }
//      while (nRes == ERROR_MORE_DATA);
//
//      if (nRes == ERROR_SUCCESS)
//      {
//        ++nIndex;
//        nValNameSize = sValueName.getLength();
//        nDataSize = fileName.getLength();
//        if(sValueName.replace(OD_T("(TrueType)"), OD_T(""))) // is TrueType font?
//        {
//          if(sValueName.replace(OD_T(" BT"), OD_T(""))==n_BT)
//          {
//            bool bBold = false;
//            if(n_BT)
//            {
//              sValueName.replace(OD_T(" Extra Bold "), OD_T(" XBd "));
//            }
//            else
//            {
//              if(sValueName.find(OD_T(" Extra Bold "))==-1)
//                bBold = (sValueName.replace(OD_T(" Bold "), OD_T(" "))!=0);
//            }
//            
//            bool bItalic = (sValueName.replace(OD_T(" Italic "), OD_T(" "))!=0);
//            sValueName.remove(' ');
//            sFaceName.remove(' ');
//            if(sValueName==sFaceName)
//            {
//              sNonExactMatched = fileName;
//              if(descr.isBold()==bBold && descr.isItalic()==bItalic)
//                break;
//            }
//            if ( sValueName.find((sFaceName+L"&").c_str()) != -1 ||
//              sValueName.find((L"&"+sFaceName).c_str()) != -1 )
//            {
//              sNonExactMatched = fileName;
//              break;
//            }
//          }
//        }
//      }
//      else
//      {
//        fileName = sNonExactMatched;
//        break;
//      }
//    }
//    ::RegCloseKey(hFonts);
//  }
//  if ( !fileName.isEmpty() )
//  {
//    if ( fileName.find(L'\\') == -1 )
//    {
//      OdString testpath;
//      int nLen = ::GetWindowsDirectory(NULL, 0);
//      if(nLen)
//      {
//        OdChar* szWinDir = testpath.getBufferSetLength(nLen);
//        nLen = ::GetWindowsDirectoryW((LPWSTR)szWinDir, nLen);
//        testpath.releaseBuffer();
//
//        testpath += OD_T("\\fonts\\");
//        fileName = testpath + fileName;
//      }
//    }
//  }
//  if ( !fileName.isEmpty() )
//    return true;
//#endif
//  return false;
}
#endif

//#include "ExPrintConsole.cpp"
//#if defined(ANDROID)
//#include <android/log.h>
//
//#define  LOG_TAG    "TeighaJni"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
//#endif
//
//#ifdef _WINRT
//
//static PrintConsoleInsideFunc g_printConsoleInsideFunc = wprintf;
//
//// Sets printConsoleString function
//void odExSetPrintConsoleInsideFunc(PrintConsoleInsideFunc printConsoleFunc)
//{
//    g_printConsoleInsideFunc = printConsoleFunc;
//}
//#endif //_WINRT
//void odPrintConsoleString(const wchar_t* fmt, ...) // to stdout
//{
//  va_list argList;
//  va_start(argList, fmt);
//  OdString s;
//  s.formatV(fmt, argList);
//  va_end(argList);
//#if (defined(_WIN32) || defined(_WIN64)) && !defined(_WIN32_WCE) && !defined(_WINRT)
//  DWORD dw;
//  HANDLE o = ::GetStdHandle(STD_OUTPUT_HANDLE);
//  if (o != NULL)
//  {
//    if (::GetFileType(o) == FILE_TYPE_CHAR)
//      ::WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
//    else
//      ::WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
//  }
//#elif defined(ANDROID)
//  printf("%s", (const char*)s);
//  LOGI("%s", (const char*)s);
//#elif defined(_WINRT)
//  g_printConsoleInsideFunc(L"%ls", s.c_str());
//#else
//  OdAnsiCharArray bf;
//  int lenStr = s.getLength();
//  bf.reserve(lenStr * 4 + 1);
//  OdCharMapper::unicodeToUtf8(s.c_str(), lenStr, bf);
//  printf(bf.asArrayPtr());
//#endif
//}
//
//void odPrintErrorString(const wchar_t* fmt, ...) // to stderr
//{
//  va_list argList;
//  va_start(argList, fmt);
//  OdString s;
//  s.formatV(fmt, argList);
//  va_end(argList);
//#if (defined(_WIN32) || defined(_WIN64)) && !defined(_WIN32_WCE) && !defined(_WINRT)
//  DWORD dw;
//  HANDLE o = ::GetStdHandle(STD_ERROR_HANDLE);
//  if (o != NULL)
//  {
//    if (::GetFileType(o) == FILE_TYPE_CHAR)
//      ::WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
//    else
//      ::WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
//  }
//#elif defined(ANDROID)
//  printf("%s", (const char*)s);
//  LOGI("%s", (const char*)s);
//#elif defined(_WINRT)
//  g_printConsoleInsideFunc(L"%ls", s.c_str());
//#else
//  OdAnsiCharArray bf;
//  int lenStr = s.getLength();
//  bf.reserve(lenStr * 4 + 1);
//  OdCharMapper::unicodeToUtf8(s.c_str(), lenStr, bf);
//  fprintf(stderr, bf.asArrayPtr());
//#endif
//}
