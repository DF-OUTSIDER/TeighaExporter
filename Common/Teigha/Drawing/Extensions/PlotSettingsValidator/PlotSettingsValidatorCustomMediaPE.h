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


#ifndef __OD_PS_VALIDATOR_MODULE2__
#define __OD_PS_VALIDATOR_MODULE2__

#include "StaticRxObject.h"
#include "DbPlotSettingsValidatorPE.h"
#include "PlotSettingsDefs.h"

#include "TD_PackPush.h"

#define CUSTOM_DEVICE_NAME "Custom Device"

/** <group Extension_Classes>
*/

class PLOTSETTINGS_DLL OdDbPlotSettingsValidatorCustomMediaPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPlotSettingsValidatorCustomMediaPE);

  /** \details
  Adds new media to list or replaces existing media with the same canonical name in list

  \param media new media to add.
  */
  virtual OdResult addMedia(const OdDbPlotSettingsValidatorPE::psvPaperInfo& media) = 0;

  /** \details
  Replaces existing media with the same canonical name in list
  
  \param media media to replace.
  */
  virtual OdResult editMedia(const OdDbPlotSettingsValidatorPE::psvPaperInfo& media) = 0;

  /** \details
  Removes media with the canonicalName in list
  
  \param canonicalName name of media to remove.
  */
  virtual OdResult removeMedia(const OdString& canonicalName) = 0;

  /** \details
  Gets media params with the canonicalName 
  
  \param canonicalName param name
  \param media media to get.
  */
  virtual OdResult getMedia(const OdString& canonicalName, OdDbPlotSettingsValidatorPE::psvPaperInfo& media) = 0;

  /** \details
  Gets media params with the index

  \param index index of media to get.
  */
  virtual OdResult getMedia(OdUInt16 index, OdDbPlotSettingsValidatorPE::psvPaperInfo& media) = 0;

  /** \details
  Clears the media list
  */
  virtual OdResult clear() = 0;

  /** \details
  Gets size of the media list
  */
  virtual OdUInt16 size() = 0;
};

typedef OdSmartPtr<OdDbPlotSettingsValidatorCustomMediaPE> OdDbPlotSettingsValidatorCustomMediaPEPtr;

#include "TD_PackPop.h"

#endif // __OD_PS_VALIDATOR_MODULE2__
