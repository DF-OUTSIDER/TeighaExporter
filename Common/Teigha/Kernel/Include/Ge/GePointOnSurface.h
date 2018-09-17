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

#ifndef OD_GEPONSRF_H
#define OD_GEPONSRF_H /*!DOM*/

#include "Ge/GePointEnt3d.h"

class OdGeSurface;

#include "TD_PackPush.h"

/** \details
    This class represents points on surfaces.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \remarks
    The point representation includes its parameter value, its coordinates, and
    the derivatives and normal of the surface at that point.

    \sa
    <link ge_Evaluators.html, Working with Evaluator Classes>
*/
class GE_TOOLKIT_EXPORT OdGePointOnSurface : public OdGePointEnt3d
{
public:


  /** \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    \param source [in]  Object to be cloned.

    \remarks
    The default constructor constructs a point on the surface OdGePlane::kXYPlane with a parameter value of (0,0).
  */
  OdGePointOnSurface();
  OdGePointOnSurface(
    const OdGePointOnSurface& source);
  OdGePointOnSurface(
    const OdGeSurface& surface);
  OdGePointOnSurface(
    const OdGeSurface& surface, 
    const OdGePoint2d& param);

  // Assignment operator.
  
  OdGePointOnSurface& operator =(
    const OdGePointOnSurface& pntOnSurface);

  /** \details
    Returns the surface.
  */
   const OdGeSurface* surface() const;
  
  /** \details
    Returns the parameter value on the surface corresponding to the point.
  */
   OdGePoint2d parameter() const;

  /** \details
    Returns the point on the surface corresponding to the parameter.
  */
   OdGePoint3d point() const;
  
  /** \details
    Returns the point on the surface corresponding to the parameter.
    
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying param updates the underlying value.
  */
  OdGePoint3d point(
    const OdGePoint2d& param);
  
  /** \details
    Returns the point on the surface corresponding to the parameter.
    
    \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying surface or param updates the underlying value.
  */
  OdGePoint3d point(
    const OdGeSurface& surface,
    const OdGePoint2d& param);
  
  /** \details
    Returns the normal to the surface at the point corresponding to the parameter.

    \remarks
    Currently not implemented.
  */
   OdGeVector3d normal() const;
  
  /** \details
    Returns the normal to the surface at the point corresponding to the parameter.

    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying surface updates the underlying value.

    \remarks
    Currently not implemented.
  */
  OdGeVector3d normal(
    const OdGePoint2d& param);
  
  /** \details
    Returns the normal to the surface at the point corresponding to the parameter.
    
    \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying surface or param updates the underlying value.

    \remarks
    Currently not implemented.
  */
  OdGeVector3d normal(
    const OdGeSurface& surface,
    const OdGePoint2d& param);

  /** \details
    Returns the specified derivative of the surface in the U direction  
    at the point corresponding to the parameter.
    
    \param order [in]  Specifies the order of the derivitive [1-2].
        
    \remarks
    Currently not implemented.
  */
   OdGeVector3d uDeriv(
    int order) const;
 
  /** \details
    Returns the specified derivative of the surface in the U direction  
    at the point corresponding to the parameter.
    
    \param order [in]  Specifies the order of the derivitive [1-2].
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying param updates the underlying value.

    \remarks
    Currently not implemented.
  */
  OdGeVector3d uDeriv(
    int order, 
    const OdGePoint2d& param);

  /** \details
    Returns the specified derivative of the surface in the U direction  
    at the point corresponding to the parameter.
    
    \param order [in]  Specifies the order of the derivitive [1-2].
    \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying surface or param updates the underlying value.

    \remarks
    Currently not implemented.
  */
  OdGeVector3d uDeriv(
    int order, const OdGeSurface& surface,
    const OdGePoint2d& param);

  /** \details
    Returns the specified derivative of the surface in the V direction 
    at the point corresponding to the parameter.
    
    \param order [in]  Specifies the order of the derivitive [1-2].
    
    \remarks
    Currently not implemented.
  */
   OdGeVector3d vDeriv(
    int order) const;

  /** \details
    Returns the specified derivative of the surface in the V direction 
    at the point corresponding to the parameter.
    
    \param order [in]  Specifies the order of the derivitive [1-2].
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying param updates the underlying value.

    \remarks
    Currently not implemented.
  */
    OdGeVector3d vDeriv(
    int order, 
    const OdGePoint2d& param);

  /** \details
    Returns the specified derivative of the surface in the V direction 
    at the point corresponding to the parameter.
    
    \param order [in]  Specifies the order of the derivitive [1-2].
    \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying surface or param updates the underlying value.

    \remarks
    Currently not implemented.
  */
    OdGeVector3d vDeriv(
    int order, 
    const OdGeSurface& surface,
    const OdGePoint2d& param);

  /** \details
    Returns the mixed partial derivative of the surface 
    at the point corresponding to the parameter.

    \remarks
    Currently not implemented.
  */
   OdGeVector3d mixedPartial() const;

  /** \details
    Returns the mixed partial derivative of the surface 
    at the point corresponding to the parameter.
    
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying surface or param updates the underlying value.

    \remarks
    Currently not implemented.
  */
   OdGeVector3d mixedPartial(
    const OdGePoint2d& param);

  /** \details
    Returns the mixed partial derivative of the surface 
    at the point corresponding to the parameter.
    
    \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    
    \remarks
    Specifying surface or param updates the underlying value.

    \remarks
    Currently not implemented.
  */
  OdGeVector3d mixedPartial(
    const OdGeSurface& surface,
    const OdGePoint2d& param);

  /** \details
    Maps the specified vector to the tangent plane of the surface 
    at the point corresponding to the parameter
    
    \param vect [in]  Any 2D vector.
    
    \remarks
    The tangent vector is mapped as follows:
    
            tangentVector = uDeriv(1) * vect.x + vDeriv(1) * vect.y

    \remarks
    Currently not implemented.
  */
   OdGeVector3d tangentVector(
    const OdGeVector2d& vect) const;

  /** \details
    Maps the specified vector to the tangent plane of the surface 
    at the point corresponding to the parameter
    
    \param param [in]  Parameter to specify a point on surface.
    \param vect [in]  Any 2D vector.
    
    \remarks
    Specifying param updates the underlying value.
    
    The tangent vector is mapped as follows:
    
            tangentVector = uDeriv(1) * vect.x + vDeriv(1) * vect.y

    \remarks
    Currently not implemented.
  */
  OdGeVector3d tangentVector(
    const OdGeVector2d& vect,
    const OdGePoint2d& param);

  /** \details
    Maps the specified vector to the tangent plane of the surface 
    at the point corresponding to the parameter
    
    \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    \param vect [in]  Any 2D vector.
    
    \remarks
    Specifying surface or param updates the underlying value.
    
    The tangent vector is mapped as follows:
    
            tangentVector = uDeriv(1) * vect.x + vDeriv(1) * vect.y

    \remarks
    Currently not implemented.
  */
  OdGeVector3d tangentVector(
    const OdGeVector2d& vect,
    const OdGeSurface& surface,
    const OdGePoint2d& param);

  /** \details
    Projects the specified vector to the tangent plane of the surface 
    at the point corresponding to the parameter.
    
    \param vect [in]  Any 3D vector.

    \remarks
    Currently not implemented.
  */
   OdGeVector2d inverseTangentVector(
    const OdGeVector3d& vect) const;

  /** \details
    Projects the specified vector to the tangent plane of the surface 
    at the point corresponding to the parameter.
    
    \param param [in]  Parameter to specify a point on surface.
    \param vect [in]  Any 3D vector.

    \remarks
    Currently not implemented.
  */
  OdGeVector2d inverseTangentVector(
    const OdGeVector3d& vect,
    const OdGePoint2d& param);
  
  /** \details
    Projects the specified vector to the tangent plane of the surface 
    at the point corresponding to the parameter
    
    \param surface [in]  Any surface.
    \param param [in]  Parameter to specify a point on surface.
    \param vect [in]  Any 3D vector.

    \remarks
    Currently not implemented.
  */
    OdGeVector2d inverseTangentVector(
    const OdGeVector3d& vect,
    const OdGeSurface& surface,
    const OdGePoint2d& param);

  /** \details
    Sets the current surface.

    \param OdGeSurface [in]  Any surface.
  */
    OdGePointOnSurface& setSurface(
    const OdGeSurface& surface);
    
  /** \details
    Sets the current parameter.

    \param param [in]  Sets the current parameter.
  */
   OdGePointOnSurface& setParameter(
    const OdGePoint2d& param);

  ///////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif // OD_GEPONSRF_H
