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

#ifndef OD_GE_COMPOSITE_CURVE_2D_H
#define OD_GE_COMPOSITE_CURVE_2D_H /*!DOM*/

#include "Ge/GeCurve2d.h"
#include "Ge/GeCurve2dPtrArray.h"
#include "Ge/GeIntArray.h"
#include "Ge/GeIntervalArray.h"

#include "TD_PackPush.h"

/** \details
    This class represents composite curves in 2D space.
    
    \remarks
    Composite curves consists of pointers to any number of subcurves that
    are joined end to end. Each subcurve must be bounded.

    The parameter at the start of the composite curve is 0.0. The parameter at any
    point along the composite curve is the approximate length of the
    composite curve up to that point.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
    
    \sa
    <link ge_OdGeCompositeCurve3d.html, Working with Composite Curves>
*/
class GE_TOOLKIT_EXPORT OdGeCompositeCurve2d : public OdGeCurve2d
{
public:
  

  /** \param curveList [in]  Array of pointers to subcurves comprising the composite curve.
    \param source [in]  Object to be cloned.

    \remarks
    The default constructor creates a composite curve that consists 
    of a single subcurve: a line segment from (0,0) to (1,0). 
    
  */
  OdGeCompositeCurve2d();
  OdGeCompositeCurve2d(const OdGeCompositeCurve2d& source);
  OdGeCompositeCurve2d(const OdGeCurve2dPtrArray& curveList);

  /** \details
    Returns an array of pointers to subcurves comprising the composite curve.

    \param curvelist [out]  Receives an array of pointers to subcurves comprising the composite curve.
    
  */
  void getCurveList(OdGeCurve2dPtrArray& curveList) const;

  /** \details
    Sets the curve list of the composite curve.

    \param curvelist [in]  Array of pointers to subcurves comprising the composite curve.
  */
  OdGeCompositeCurve2d& setCurveList(const OdGeCurve2dPtrArray& curveList);

  /** \details
    Returns the parameter on a subcurve, and the index of that subcurve,
    corresponding to the specified parameter on the composite curve.

    \param param [in]  Parameter value on composite curve.
    \param crvNum [out]  Receives the curve number of the subcurve.
    
  */
  double globalToLocalParam(double param, 
                                    int& crvNum) const; 

  /** \details
    Returns the parameter on the composite curve, corresponding
    to the specified parameter on the specifed subcurve curve.

    \param param [in]  Parameter value on the subcurve.
    \param crvNum [in]  Curve number of the subcurve.

  */
  double localToGlobalParam(double param, 
                                    int crvNum) const; 

  /** \remarks
    All of the subcurves of the input curve are copied.         
  */
  OdGeCompositeCurve2d& operator =(const OdGeCompositeCurve2d& compCurve);
};

#include "TD_PackPop.h"

#endif // OD_GE_COMPOSITE_CURVE_2D_H

