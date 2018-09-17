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

#ifndef _ProgressMonitor_h_Included_
#define _ProgressMonitor_h_Included_

#include "dwfcore/MonitoredInputStream.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT {

/** \details
  This class implements the import progress meter.
*/
class DWFImportProgressMeter
{
  OdDbHostAppProgressMeter* m_pOuterMeter;
  int _current;
  OdDbDatabase* _db;
public:
  DWFImportProgressMeter(OdDbDatabase* db) : _current(0), _db(db)
  {
    m_pOuterMeter = db->appServices()->newProgressMeter();
    m_pOuterMeter->setLimit(100);
    m_pOuterMeter->start();
  }
  ~DWFImportProgressMeter()
  {
    _db->appServices()->releaseProgressMeter(m_pOuterMeter);
  }
  int current(){return _current;}
  void step()
  {
    m_pOuterMeter->meterProgress();
    ++_current;
  }
};

/** \details
  This class implements the progress monitor.
*/
class ProgressMonitor : public DWFMonitoredInputStream::Monitor
{
  size_t _total;
  size_t _current;
  double _percents;
  DWFImportProgressMeter& _pm;
  int _start;
public:
  ProgressMonitor(size_t total, DWFImportProgressMeter& pm, double percents) :
      _total(total), _current(0), _percents(percents), _pm(pm)
  {
    _start = _pm.current();
  }
  virtual void notify( const void* const /*pBuffer*/, size_t /*nBytesRequested*/, size_t nBytesRead ) throw( DWFException )
  {
    _current += nBytesRead;
    int cur = int((_percents * _current) / _total);
    while(_pm.current() < (cur + _start))
      _pm.step();
  }
};
}
#endif //_ProgressMonitor_h_Included_
