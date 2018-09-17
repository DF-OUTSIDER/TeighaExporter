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


#ifndef _EX_HOSTAPPSERVICES_H_
#define _EX_HOSTAPPSERVICES_H_

#include "TD_PackPush.h"

#include "DbHostAppServices.h"
#include "HatchPatternManager.h"
#include "StaticRxObject.h"
#include "DbDatabaseReactor.h"
#include "Gi/TtfDescriptor.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include "ExPrintConsole.h"

typedef OdArray<OdTtfDescriptor> mapTrueTypeFont;

/** \details
  This class implements platform-dependent operations and progress metering.
  <group ExServices_Classes> 
  Library: Source code provided. 
*/
class ExHostAppServices : public OdDbHostAppServices2, public OdDbHostAppProgressMeter
{
  // MKU 06/17/05 - This optimization is obsolete. But it can be reason of bug 
  //                  when old database is found by name instead of creating anew.
  /*
  class DwgCollection : public OdStaticRxObject<OdDbDatabaseReactor>
  {
    std::map<OdString, OdDbDatabase*> m_loadedDwgs;
    void goodbye(const OdDbDatabase* pDb);
  public:
    OdDbDatabase* lookUp(const OdString& keyFileName);
    void add(OdDbDatabase* pDb);
  }         m_dwgCollection;
  */
  
  OdString  m_Prefix;
  long      m_MeterLimit;
  long      m_MeterCurrent;
  long      m_MeterOld;
  bool      m_disableOutput;
//   mapTrueTypeFont m_mapTTF;
//   OdMutex   m_TTFMapMutex;
//   bool      m_bSysFontCollected;

  OdHatchPatternManagerPtr m_patternManager;
public:
  ExHostAppServices();

  OdDbHostAppProgressMeter* newProgressMeter();

  void releaseProgressMeter(OdDbHostAppProgressMeter* pProgressMeter);

#ifdef SWIG
  void warning1(const OdString& ) { }
#else
  void warning(const char*, const OdString&) {}
  TD_USING(OdDbHostAppServices2::warning); 
#endif

  // OdDbHostAppProgressMeter functions

  void start(const OdString& displayString = OdString::kEmpty);

  void stop();

  void meterProgress();

  void setLimit(int max);

  //bool ttfFileNameByDescriptor(const OdTtfDescriptor& description, OdString& filename);

  /** \details
    Controls display of this ProgressMeter.
    \param disable [in]  Disables this ProgressMeter. 
  */
  void disableOutput(bool disable) { m_disableOutput = disable; }

  /** \details
    Sets the prefix for this ProgressMeter.
    \param prefix [in]  Prefix for this ProgressMeter.
  */
  void setPrefix(const OdString& prefix) { m_Prefix = prefix; }

  OdHatchPatternManager* patternManager();

  OdDbDatabasePtr readFile(const OdString& filename,
    bool allowCPConversion = false,
    bool partialLoad = false,
    Oda::FileShareMode shareMode = Oda::kShareDenyNo,
    const OdPassword& password = OdPassword());

  TD_USING(OdDbHostAppServices2::readFile);
};

#include "TD_PackPop.h"

#endif

