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
//
// DbDiesel.h
//

// The original diesel engine was modified by ODA to be used inside DD

#ifndef OD_DB_DIESEL_H
#define OD_DB_DIESEL_H

#include "OdaCommon.h"
#include "DbDatabase.h"

#include "TD_PackPush.h"

/** \details

    \sa
    TD_Db

    <group Other_Classes> 
*/
class ODRX_ABSTRACT OdDieselService
{
public:
  virtual bool getSystemVariable(const OdString& sName, OdString& sValue) = 0;
  virtual void getUnits(int* pAngularUnits, // (0) // AUNITS // 0 Degrees
                                                             // 1 Degrees-Minutes-Seconds
                                                             // 2 Gradians
                                                             // 3 Radians
                                                             // 4 Surveyor Units
                        int* pAngularPrec, // (0) // AUPREC // number of decimal places of angular units
                        int* pLinearUnits, // (2) // LUNITS // 1 Scientific
                                                            // 2 Decimal
                                                            // 3 Engineering
                                                            // 4 Architectural
                                                            // 5 Fractional
                        int* pLinearPrec, // (4) // LUPREC // the number of decimal places of linear units
                        int* pDimzin, // (0)(8 - metric) // DIMZIN // zero suppression in linear dimensions :
                                                                   // 0 Suppress zero feet and exactly zero inches
                                                                   // 1 Include zero feet and exactly zero inches
                                                                   // 2 Include zero feet and suppress exactly zero inches
                                                                   // 3 Suppress zero feet and include exactly zero inches
                                                                   // 4 Suppress leading decimal zeros
                                                                   // 8 Suppress trailing decimal zeros
                                                                   // 12 Suppress leading and trailing decimal zeros
                        int* pUnitMode) = 0; // (0) // UNITMODE // unit display format for Architectural Units and Surveyor's Units
                                                                // 1 default
                                                                // 1 in a format suitable for input

  virtual bool getEnvironmentVariable(const OdString& sName, OdString& sValue) = 0;
  virtual bool getDrawingProperty(const OdString& sName, OdString& sValue) = 0;
};


DBENT_EXPORT int OdDieselEvaluate(const OdChar* in, OdChar* out, OdDieselService* pDieselService);
DBENT_EXPORT int OdDieselEvaluate(const OdChar* in, OdChar* out, OdDbDatabase* db);

#include "TD_PackPop.h"

#endif // OD_DB_DIESEL_H
