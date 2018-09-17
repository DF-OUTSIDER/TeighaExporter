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




#ifndef _ODDB_HOSTAPPPROGRESSMETER_INCLUDED_
#define _ODDB_HOSTAPPPROGRESSMETER_INCLUDED_


#include "TD_PackPush.h"


#include "DbRootExport.h"
#include "OdString.h"


/** \details
    This class is the base class for platform specific progress metering
    within Teigha.

    Corresponding C++ library: TD_Db

    \remarks
    This class receives progress notifications during various database operations such
    as loading or saving a file.

    Calls to an instance of this class will always come in the following order:

    1.  setLimit (called once).
    2.  start (called once).
    3.  meterProgress (called repeatedly).
    4.  stop (called once).

    <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbHostAppProgressMeter
{
public:
  virtual ~OdDbHostAppProgressMeter();

    /** \details
    Notification function called to initialize this ProgressMeter object.
    \param displayString [in]  String to be displayed.
    */
  virtual void start(
    const OdString& displayString = OdString::kEmpty) = 0;

  /** \details
    Notification function called to terminate this ProgressMeter object.
  */
  virtual void stop() = 0;

  /** \details
    Notification function called to increment this ProgressMeter object.

    \remarks
    The completion percentage may be calculated by dividing the number of times
    this function is called by the value set by setLimit.

    \remarks
    Throwing an exception indicates that the operation associated with this ProgressMeter object
    should be halted.
  */
  virtual void meterProgress() = 0;

  /** \details
    Notification function called to specify the maximum number of times this
    ProgressMeter object will be incremented.

    \param max [in]  Maximum meterProgress calls.
  */
  virtual void setLimit(
    int max) = 0;
};


#include "TD_PackPop.h"

#endif // _ODDB_HOSTAPPPROGRESSMETER_INCLUDED_
