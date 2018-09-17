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
#ifndef GS_VECT_PERFORMANCE_H
#define GS_VECT_PERFORMANCE_H

#include "TD_PackPush.h"

/** \details
 <group OdGs_Classes> 
    
 Corresponding C++ library: TD_Gs
*/
class GsDevicePerformanceTm
{
public:
    GsDevicePerformanceTm() { reset(); }
    void reset()
    {
        m_update = 0;
        m_updateGeometry = 0;;
        m_updateScreen = 0;
        m_updateEntities = 0;
    }

public:
    double m_update;
    double m_updateGeometry;
    double m_updateScreen;
    double m_updateEntities;
};

enum EParallelVectOptions
{
    eEnableParallelVectorization = 0x00000001,
    eEnableParallelDisplay       = 0x00000002,
    eEnableOptimalThreadsNumber  = 0x00000004,
    eEnableSchedulerLogOutput    = 0x00000008,
    eEnablePerfMeasurements      = 0x00000010,
    eForcePartialUpdateForTest   = 0x00000020,
    eForceParallelVectorization  = 0x00000040
};

/** \details
  <group OdGs_Classes> 
    
  Corresponding C++ library: TD_Gs
*/
class GsVectPerformanceData
{
public:
    GsVectPerformanceData(): m_options(0), m_numVectUsedUpdateGeom(0),
        m_numVectUsedUpdateScr(0){}
    bool enableParallelVectorization() const
    { return GETBIT(m_options, eEnableParallelVectorization); }
    bool enableParallelDisplay() const
    { return GETBIT(m_options, eEnableParallelDisplay); }
    bool schedulerLogOutput() const
    { return GETBIT(m_options, eEnableSchedulerLogOutput); }
    bool optimalThreadsNumber() const
    { return GETBIT(m_options, eEnableOptimalThreadsNumber); }
    bool enablePerfMeasurements() const
    { return GETBIT(m_options, eEnablePerfMeasurements); }
    bool forcePartialUpdateForTest() const
    { return GETBIT(m_options, eForcePartialUpdateForTest); }
    bool forceParallelVectorization() const
    { return GETBIT(m_options, eForceParallelVectorization); }

public:
    OdUInt32 m_options; //bit flags, see EParallelVectOptions
    GsDevicePerformanceTm m_tm;
    OdUInt32 m_numVectUsedUpdateGeom;
    OdUInt32 m_numVectUsedUpdateScr;
};

#include "TD_PackPop.h"

#endif // GS_VECT_PERFORMANCE_H
