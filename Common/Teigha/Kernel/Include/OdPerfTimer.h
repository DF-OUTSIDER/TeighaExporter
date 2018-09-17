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

// AMark : PerfTest timing classes

#ifndef _OdPerfTimer_INCLUDED_
#define _OdPerfTimer_INCLUDED_


#include "OdPlatformSettings.h"
#include "TD_PackPush.h"

/** \details
    This interface represents Performance Timer objects.
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdPerfTimerBase
{
  protected:
    struct InternalData
    {
      OdInt64 started;
      OdInt64 stopped;
      OdInt64 freq;
      OdUInt8 flags[4];
      InternalData()
        : started(0)
        , stopped(0)
        , freq(0)
      {
        flags[0] = flags[1] = flags[2] = flags[3] = 0;
      }
    } gData;
    InternalData *data;
  public:
    OdPerfTimerBase() : data(&gData) { }

    virtual ~OdPerfTimerBase() { }
    virtual void initialize() = 0;
    virtual void destroy() = 0;
    /** \details
    Clears timer data
    */
    virtual void clear() = 0;
    /** \details
    Starts timer
    */
    virtual void start() = 0;
    /** \details
    Stops timer
    */
    virtual void stop() = 0;
    /** \details
    Returns time from start() to stop() in seconds
    */
    virtual double countedSec() = 0;
    /** \details
    Returns time from start() to stop() in miliseconds
    */
    virtual OdUInt32 countedMSec() = 0;
    /** \details
    Returns time from start() to current in seconds
    */
    virtual double permanentSec() = 0;
    /** \details
    Returns time from start() to current in miliseconds
    */
    virtual OdUInt32 permanentMSec() = 0;
    /** \details
    Returns true if timer is running
    */
    virtual bool isStarted() = 0;

    static OdPerfTimerBase *createTiming();
    static void destroyTiming(OdPerfTimerBase *ptr);
};

/** \details
    This class wraps Performance Timer objects.
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdPerfTimerWrapper
{
  OdPerfTimerWrapper( const OdPerfTimerWrapper& );
  OdPerfTimerWrapper& operator=(const OdPerfTimerWrapper& );
  OdPerfTimerBase* m_pTimer;
public:

  OdPerfTimerWrapper()
  {
    m_pTimer = OdPerfTimerBase::createTiming();
  }

  ~OdPerfTimerWrapper()
  {
    OdPerfTimerBase::destroyTiming( m_pTimer );
  }

  OdPerfTimerBase* getTimer()
  {
    return m_pTimer;
  }
};

#include "TD_PackPop.h"

#endif
