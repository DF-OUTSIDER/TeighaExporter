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




#ifndef _ODDBDATE_INCLUDED_
#define _ODDBDATE_INCLUDED_

#include "TD_PackPush.h"

#include "OdTimeStamp.h"

class OdDbDwgFiler;

/** \details
    This class represents Date objects in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbDate : public OdTimeStamp
{
public:
  /** \param tStamp [in]  TimeStamp.
    \param init [in]  Initial value.
    
    \remarks
    The default constructor initializes the Julian date and time to zero.
    
    init must be one of the following:
    
    <table>
    Name                 Value    Description
    kInitZero            1        Zero.
    kInitLocalTime       2        Workstation date in local time.
    kInitUniversalTime   3        Workstation date in Universal (Greenwich Mean) Time..
    </table>
  */
   OdDbDate();
  OdDbDate(
    const OdTimeStamp& tStamp);
  OdDbDate(
    InitialValue init);

  OdDbDate operator=(
    const OdTimeStamp& tStamp);

  /** \details
    Reads the .dwg file format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
  */
  void dwgIn(
    OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
  */
  void dwgOut(
    OdDbDwgFiler* pFiler) const;
};

#include "TD_PackPop.h"

#endif // _ODDBDATE_INCLUDED_

