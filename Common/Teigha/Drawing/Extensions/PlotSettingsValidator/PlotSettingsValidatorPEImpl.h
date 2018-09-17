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




#ifndef OD_DBPLOTSETVALPEIMPL_H
#define OD_DBPLOTSETVALPEIMPL_H

#include "TD_PackPush.h"
#include "DbPlotSettingsValidatorPE.h"
#include "PlotSettingsValidatorCustomMediaPE.h"

/** \details
This class is the Plot Settings Validator Protocol Extension class implementation for custom paper sizes.
library: Source code provided.
<group Win_Classes>
*/
class OdPlotSettingsValidatorCustomMediaPEImpl : public OdDbPlotSettingsValidatorCustomMediaPE
{
public:
  virtual OdResult addMedia(const OdDbPlotSettingsValidatorPE::psvPaperInfo& media);
  virtual OdResult editMedia(const OdDbPlotSettingsValidatorPE::psvPaperInfo& media);
  virtual OdResult removeMedia(const OdString& canonicalName);
  virtual OdResult getMedia(const OdString& canonicalName, OdDbPlotSettingsValidatorPE::psvPaperInfo& media);
  virtual OdResult getMedia(OdUInt16 index, OdDbPlotSettingsValidatorPE::psvPaperInfo& media);
  virtual OdResult clear();
  virtual OdUInt16 size();
};

/** \details
    This class is the Plot Settings Validator Protocol Extension class implementation.
    library: Source code provided. 
    <group Win_Classes> 
*/
class OdPlotSettingsValidatorPEImpl : public OdDbPlotSettingsValidatorPE
{
public:

  /** \details
    Returns a list of plot device names for this ValidatorPE object.
    \param deviceList [out]  Receives the list of device names.
  */
  virtual OdResult getDeviceList(OdArray<OdString> &deviceList);
  /** \details
    Returns a list of media names for the specified device name for this ValidatorPE object.
    \param mediaList [out]  Receives the list of media names.
    \param deviceName [in]  Device name.
  */
  virtual OdResult getMediaList(const OdString &deviceName, OdArray<psvPaperInfo> &mediaList, bool bUpdateMediaMargins);
  /** \details
    Returns a list of plot style sheet names for this ValidatorPE object.
    \param plotStyleSheetList [out]  Receives the list of plot style sheet names.
    \param pHostApp [in]  Application services with implemented getEnv().
    Note : if pHostApp is NULL, OdRxSystemServices::getEnvVar() will be called.
  */
  virtual OdResult getPlotStyleSheetList(OdArray<OdString> &plotStyleSheetList, OdDbBaseHostAppServices *pHostApp);

  /** \details
  Returns a default media name for the specified device name for this ValidatorPE object.
  \param sDefaultMedia [out]  Default media name.
  \param sDeviceName [in]  Device name.
  Note : sDefaultMedia is local (not canonical) name.
  */
  virtual OdResult getDefaultMedia(const OdString &deviceName, OdString &defaultMedia);

  virtual OdResult getMediaMargins(const OdString &deviceName, psvPaperInfo &pMediaInfo);
};

#include "TD_PackPop.h"

#endif // OD_DBPLOTSETVALPEIMPL_H
