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

#ifndef OD_GE_EXTERNAL_CURVE_2D_H
#define OD_GE_EXTERNAL_CURVE_2D_H /*!DOM*/

#include "TD_PackPush.h"
#include "Ge/GeCurve2d.h"

class OdGeNurbCurve2d;

/** \details
    This class represents, as OdGeCurve2d curves, 2D curves whose definitions are external to the OdGe library.
    
    \remarks
    OdGeExternalCurve2d curve instances can be treated as any other OdCurve2d. 
    
    You can access the OdGeExternalCurve2d as a corresponding native OdGeCurve2d curve, if such a corresponding
    curve exists, or you may (more efficiently) access the external data in its native form.
    
    One example of using OdGeOdGeExternalCurve2d is to represent an ACIS curve. 
    
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeExternalCurve2d : public OdGeCurve2d
{
public:
  

  /** \param pCurveDef [in]  Pointer to the curve definition.
    \param curveKind [in]  Information about system that created the curve.
    \param makeCopy [in]  Makes a copy of the curve definition.
    \param source [in]  Object to be cloned.

    \remarks
    Without arguments, the constructor creates an empty instance.

    Possible values for curveKind:

    @untitled table
    kAcisEntity
    kExternalEntityUndefined
  */
  OdGeExternalCurve2d();
  OdGeExternalCurve2d(
    const OdGeExternalCurve2d& source);
  OdGeExternalCurve2d(
    void* pCurveDef, 
    OdGe::ExternalEntityKind curveKind,
    bool makeCopy = true);

  /** \details
    Returns true if and only if the curve can be represented as an OdGeNurbCurve2d curve.
  */
  bool isNurbCurve() const;
  
  /** \details
    Returns true, and an OdGeNurbCurve2d curve,
    if and only if the curve can be represented as an OdGeNurbCurve2d curve.

    \param nurbCurve [out]  Receives an OdGeNurbCurve2d internal representation of the external curve.

    \remarks
    nurbCurve is created with the new method, and must be deleted by the caller.
  */
  bool isNurbCurve(
    OdGeNurbCurve2d& nurbCurve) const;

  /** \details
    Returns true if and only if the curve is defined (not an empty instance).
  */
  bool isDefined() const;

  /** \details
    Returns a copy of the raw curve definition.

    \param pCurveDef [out]  Receives a pointer to a copy of the raw curve definition.

    \remarks
    It is up to the caller to delete the memory allocated.
  */
  void getExternalCurve(
    void*& pCurveDef) const;

  /** \details
    Returns information about the system that created the curve.

    \remarks
    Possible values for curveKind:

    @untitled table
    kAcisEntity
    kExternalEntityUndefined
  */
  OdGe::ExternalEntityKind externalCurveKind() const;

  /** \details
    Sets the parameters for this external curve according to the arguments, 
    and returns a reference to this external curve.

    \param pCurveDef [in]  Pointer to the curve definition.
    \param curveKind [in]  Information about system that created the curve.
    \param makeCopy [in]  If true, makes a copy of of the curve definition.
  */
  OdGeExternalCurve2d& set(
    void* pCurveDef, 
    OdGe::ExternalEntityKind curveKind,
    bool makeCopy = true);

  /** \details
    Returns true if and only if if the external curve owns the data.

    \remarks
    If the external curve owns the data, it will be destroyed when
    the curve is destroyed.
  */
  bool isOwnerOfCurve() const;

  /** \details
    Forces this external curve to own the data, and returns a pointer to this curve.

    \remarks
    If the external curve owns the data, it will be destroyed when
    the curve is destroyed.
  */
  OdGeExternalCurve2d& setToOwnCurve();

  OdGeExternalCurve2d& operator =(
    const OdGeExternalCurve2d& extCurve);

};

#include "TD_PackPop.h"

#endif // OD_GE_EXTERNAL_CURVE_2D_H
