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

#ifndef _EX_DGNHOSTAPPSERVICES_H_
#define _EX_DGNHOSTAPPSERVICES_H_

#include "DgHostAppServices.h"

// Auxiliary function to display different messages
void oddgPrintConsoleString(const wchar_t* fmt, ...);


/** \details
  <group Dg_Extension_Classes>

  This class implements the progress meter for dgn application.  
*/
class OdExDgnHostAppProgressMeter : public OdDbHostAppProgressMeter
{
public:
  OdExDgnHostAppProgressMeter();

  // OdDbHostAppProgressMeter overridden
  virtual void start(const OdString& displayString = OdString::kEmpty);
  virtual void stop();
  virtual void meterProgress();
  virtual void setLimit(int max);

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

private:
  OdString  m_Prefix;
  long      m_MeterLimit;
  long      m_MeterCurrent;
  long      m_MeterOld;
  bool      m_disableOutput;
};

/** \details
  This class implements platform-dependent operations and progress metering.

  <group Dg_Extension_Classes> 
*/
class OdExDgnHostAppServices : public OdDgHostAppServices
{
public:
  OdExDgnHostAppServices();

  // OdDgHostAppServices overridden
  virtual bool ttfFileNameByDescriptor(const OdTtfDescriptor& description, OdString& filename);
  virtual OdDbHostAppProgressMeter* newProgressMeter();
  virtual void releaseProgressMeter(OdDbHostAppProgressMeter* pProgressMeter);

  /** \details
    Controls display of this ProgressMeter.
    \param disable [in]  Disables this ProgressMeter. 
  */
  void disableProgressMeterOutput(bool disable) { m_progressMeter.disableOutput(disable); }

private:
  OdExDgnHostAppProgressMeter m_progressMeter;
};

#endif  // _EX_DGNHOSTAPPSERVICES_H_

