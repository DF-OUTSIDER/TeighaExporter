//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFCORE_TIMER_H
#define _DWFCORE_TIMER_H

///
///\file        dwfcore/Timer.h
///\brief       This header contains the declaration for the DWFTimer class.
///

#include "dwfcore/Core.h"


namespace DWFCore
{

///
///\class           DWFTimer   dwfcore/Timer.h  "dwfcore/Timer.h"
///\brief           This class provides a high resolution hardware-based timer.
///\since           1.0.1
///\note            This class has platform-specific implementations.
///\ingroup         CrossPlatformImpl
///
///                 This class most basically provides access to a raw cpu tick count.
///                 Using this 64-bit value, it then provides simple stopwatch-like
///                 functionality for measuring code execution.
///                 Since the class declaration is the same for all platforms,
///                 it (and the values it provides) can be used in portable
///                 code.  Further, these values are more accurate for measuring
///                 events that occur under the millisecond threshhold where using
///                 certain OS-specific APIs would not suffice.
///                 It is preferrable to use these numbers and convert them into milliseconds
///                 (or some other unit) if the need arises.  The following sample is one
///                 such method:
///
///                 Example - convert high resultion cpu ticks into milliseconds:
///                 \code
///                         cout << "Calibrating...";
///
///                 #ifdef  _DWFCORE_WIN32_SYSTEM
///
///                         unsigned long ms = ::GetTickCount();
///                         unsigned long long ts = DWFTimer::Tick64();
///                         ::Sleep( 1000 );
///                         ts = DWFTimer::Tick64() - ts;
///                         ms = ::GetTickCount() - ms;
///
///                         double tpms = ts/(double)(ms);
///                 #else
///
///                         struct timeval tv;
///                         struct timeval tv2;
///                         gettimeofday( &tv, NULL );
///                         unsigned long long ts = DWFTimer::Tick64();
///                         usleep( 10000 );
///                         ts = DWFTimer::Tick64() - ts;
///                         gettimeofday( &tv2, NULL );
///
///                         time_t sec = tv2.tv_sec - tv.tv_sec;
///                         suseconds_t usec = tv2.tv_usec - tv.tv_usec;
///
///                         double tpms = ts/( (sec*1000) + (usec/1000) );
///                #endif
///
///                         cout << "using " << tpms << " ticks per ms" << endl << endl;
///                 \endcode
///
class DWFTimer : virtual public DWFCoreMemory
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFTimer()
        throw();

    ///
    ///             Copy Constructor
    ///
    ///\param       rTimer  The other timer from which to copy the tick value.
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFTimer( const DWFTimer& rTimer )
        throw();

    ///
    ///             Assignment Operator
    ///
    ///\param       rTimer  The other timer from which to copy the tick value.
    ///\throw       None
    ///
    _DWFCORE_API 
    DWFTimer& operator=( const DWFTimer& rTimer )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    ~DWFTimer()
        throw();

    ///
    ///             Returns the lower 32-bits of the cpu tick.
    ///
    ///\return      The cpu tick value.
    ///\throw       None
    ///
    _DWFCORE_API 
    static unsigned long Tick32()
        throw();

    ///
    ///             Returns the full 64-bits of the cpu tick.
    ///
    ///\return      The cpu tick value.
    ///\throw       None
    ///
    _DWFCORE_API 
    static uint64_t Tick64()
        throw();

    ///
    ///             Begins the timer/stopwatch feature of the class.
    ///             This method should be can in conjuction with \a stop(),
    ///             \a tick32(), \a tick64(), \a timer32() and \a timer64().
    ///
    ///             Stopwatch Example:
    ///             \code
    ///             DWFTimer oStopwatch;
    ///             uint64_t nLastLap = 0;
    ///             
    ///             oStopwatch.start();
    ///
    ///             for(int i = 0; i < LAPS; i++)
    ///             {
    ///                 ... do work ...
    ///             
    ///                 uint64_t nLap = oStopwatch.tick64();
    ///                 record_lap_time( i, nLap - nLastLap );
    ///                 nLastLap = nLap;
    ///             }
    ///
    ///             oStopwatch.stop();
    ///             record_total_time( oStopwatch.timer64() );
    ///             \endcode
    ///
    ///\throw       None
    ///
    _DWFCORE_API 
    void start()
        throw();

    ///
    ///             Returns the lower 32-bit difference from the last \a start() call.
    ///
    ///\return      The difference in cpu tick values.
    ///\throw       None
    ///
    _DWFCORE_API 
    unsigned long tick32()
        throw();

    ///
    ///             Returns the full 64-bit difference from the last \a start() call.
    ///
    ///\return      The difference in cpu tick values.
    ///\throw       None
    ///
    _DWFCORE_API 
    uint64_t tick64()
        throw();

    ///
    ///             Stops the timer and stores the elapsed difference from the last \a start() call.
    ///\throw       None
    ///
    _DWFCORE_API 
    void stop()
        throw();

    ///
    ///             Returns the lower 32-bits of the ticks elapsed between \a start() and \a stop() calls.
    ///
    ///\return      The difference in cpu tick values.
    ///\throw       None
    ///
    _DWFCORE_API 
    unsigned long timer32()
        throw();

    ///
    ///             Returns the full 64-bits of the ticks elapsed between \a start() and \a stop() calls.
    ///
    ///\return      The difference in cpu tick values.
    ///\throw       None
    ///
    _DWFCORE_API 
    uint64_t timer64()
        throw();

private:

    uint64_t _nTick;
};


}


#endif


