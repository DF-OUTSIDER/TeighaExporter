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




#ifndef __ODGIDEVIATION_H__
#define __ODGIDEVIATION_H__


#include "Gi/GiCommonDraw.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint3d.h"

/** \details
    This class implements an interface that returns the deviation values used for anisotropic space 
    (perspective view).

    Corresponding C++ library: TD_Gi
    
    <group OdGi_Classes> 
*/
class OdGiDeviation
{
public:

  /** \details
    Returns the recommended maximum deviation of the current vectorization, for the specified point on the curve or surface being tesselated.
    \sa
    OdGiGeometrySimplifier::setDeviation
    \param deviationType [in]  Deviation type.
    \param pointOnCurve [in]  Point on the curve.
    \remarks
    deviationType must be one of the following:
    
    <table>
    Name                       Value
    kOdGiMaxDevForCircle       0      
    kOdGiMaxDevForCurve        1      
    kOdGiMaxDevForBoundary     2      
    kOdGiMaxDevForIsoline      3
    kOdGiMaxDevForFacet        4
    </table>
  */
  virtual double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const = 0;
};
/** \details
    Returns an array of the deviations for the specified Deviation object for the specified point on the curve.
    \param deviationObj [in]  Deviation object.
    \param pointOnCurve [in]  Point on the curve.
  
    \remarks
    Deviations are returned in the array in the following order:
    
    <table>
    Name                       Index
    kOdGiMaxDevForCircle       0      
    kOdGiMaxDevForCurve        1      
    kOdGiMaxDevForBoundary     2      
    kOdGiMaxDevForIsoline      3
    kOdGiMaxDevForFacet        4
    </table>
*/
inline OdGeDoubleArray odgiGetAllDeviations(const OdGiDeviation& deviationObj, const OdGePoint3d& pointOnCurve = OdGePoint3d::kOrigin)
{
  OdGeDoubleArray values(5);
  values.append(deviationObj.deviation(kOdGiMaxDevForCircle,   pointOnCurve));
  values.append(deviationObj.deviation(kOdGiMaxDevForCurve,    pointOnCurve));
  values.append(deviationObj.deviation(kOdGiMaxDevForBoundary, pointOnCurve));
  values.append(deviationObj.deviation(kOdGiMaxDevForIsoline,  pointOnCurve));
  values.append(deviationObj.deviation(kOdGiMaxDevForFacet,    pointOnCurve));
  return values;
}

#endif //#ifndef __ODGIDEVIATION_H__
