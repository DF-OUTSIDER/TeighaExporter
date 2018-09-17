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

#ifndef OD_GENURBSF_H
#define OD_GENURBSF_H /*!DOM*/

#include "Ge/GeSurface.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeEllipCylinder.h"
#include "Ge/GeEllipCone.h"
#include "OdPlatformSettings.h"
#include "TD_PackPush.h"

class OdGeNurbCurve3d;

static const int derivArraySize = 3;
/**
    \details
    Defines VectorDerivArray type.
*/
typedef OdGeVector3d VectorDerivArray[derivArraySize][derivArraySize];
/**
    \details
    Defines the WDerivArray type.
*/
typedef double WDerivArray [derivArraySize][derivArraySize];

/** \details
    This class represents non-uniform, rational B-Spline (NURBS) surfaces.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeNurbSurface.html, Working with NURBS Surfaces>
*/
class GE_TOOLKIT_EXPORT OdGeNurbSurface : public OdGeSurface
{
public:


  /** \param degreeInU [in]  Degree in the U direction. 
    \param degreeInV [in]  Degree in the V direction.
    \param propsInU [in]  Properties in the U direction.
    \param propsInV [in]  Properties in the V direction.
    \param numControlPointsInU [in]  Number of control points in the U direction.
    \param numControlPointsInV [in]  Number of control points in the V direction.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of weights
    \param uKnots [in]  Knot vector in the U direction.
    \param vKnots [in]  Knot vector in the V direction.
    \param tol [in]  Geometric tolerance.
    \param source [in]  Object to be cloned.
   
    \remarks
    propsInU and propsInV utilize OdGe::NurbSurfaceProperties values.
  */
  OdGeNurbSurface();
  OdGeNurbSurface(
    int degreeInU, 
    int degreeInV, 
    int propsInU, 
    int propsInV,
    int numControlPointsInU, 
    int numControlPointsInV,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    const OdGeKnotVector& uKnots,
    const OdGeKnotVector& vKnots,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeNurbSurface(
    const OdGeNurbSurface& source);
  OdGeNurbSurface(const OdGeEllipCylinder& cylinder);
  OdGeNurbSurface(const OdGeEllipCone& cone);

  OdGeNurbSurface& operator =(
    const OdGeNurbSurface& nurb);

  
  /** \details
    Returns true if and only if this surface is rational in the U direction.

    \param tol [in]  Geometric tolerance.
  */
     bool isRationalInU() const;

  /** \details
    Returns true if and only if this surface is periodic in the U direction, and returns the period.

    \param period [in]  Period.
  */
     bool isPeriodicInU(double& period) const;

  /** \details
    Returns true if and only if this surface is rational in the V direction.

    \param tol [in]  Geometric tolerance.
  */
     bool isRationalInV() const;

  /** \details
    Returns true if and only if this surface is periodic in the U direction, and returns the period.

    \param period [in]  Period.
  */
     bool isPeriodicInV(double& period) const;
  
  /** \details
    Returns a description of the singularity in the U direction:
    
    \remarks
    singularityInU utilizes OdGe::NurbSurfaceProperties values.
  */
   int singularityInU() const;

  /** \details
    Returns a description of the singularity in the V direction:
    
    \remarks
    singularityInV utilizes OdGe::NurbSurfaceProperties values.
  */
   int singularityInV() const;

  /** Desctiption:
    Returns the degree in the U direction.
  */
   int degreeInU() const;

  /** \details
    Returns the number of control points in the U direction.
  */
   int numControlPointsInU() const;
  
  /** \details
    Returns the degree in the V direction.
  */
   int degreeInV() const;
  
  /** \details
    Returns the number of control points in the V direction.
  */
   int numControlPointsInV() const;

  /** \details
    Returns the array of control points.
    
    \param controlPoints [out] Receives an array of 3D control points.
  */
     void getControlPoints(
    OdGePoint3dArray& controlPoints) const;
    
  /** \details
    Returns the array of weights.
    
    \param weights [in]  Array of weights
  */
     bool getWeights(
    OdGeDoubleArray& weights) const;
  
  /** \details
    Returns the number of knots in the U direction.
  */
     int numKnotsInU() const;
  
  /** \details
    Returns the knot vector in the U direction.
    
    \param uKnots [in]  Knot vector in the U direction.
  */
   void getUKnots(
    OdGeKnotVector& uKnots) const;
  
  /** \details
    Returns the number of knots in the V direction.
  */
     int numKnotsInV() const;
  
  /** \details
    Returns the knot vector in the V direction.
    
    \param vKnots [in]  Knot vector in the V direction.
  */
   void getVKnots(
    OdGeKnotVector& vKnots) const;
    
  /** \details
    Returns the data used to define this surface.
    
    \param degreeInU [out]  Receives the Degree in the U direction. 
    \param degreeInV [out]  Receives the Degree in the V direction.
    \param propsInU [out]  Receives the properties in the U direction.
    \param propsInV [out]  Receives the properties in the V direction.
    \param numControlPointsInU [out]  Receives the number of control points in the U direction.
    \param numControlPointsInV [out]  Receives the number of control points in the V direction.
    \param controlPoints [out]  Receives an array of 3D control points.
    \param weights [out]  Receives an array of weights
    \param uKnots [out]  Receives the knot vector in the U direction.
    \param vKnots [out]  Receives the knot vector in the V direction.
   
    \remarks
    propsInU and propsInV utilize OdGe::NurbSurfaceProperties values.
  */
     void getDefinition(
    int& degreeInU, 
    int& degreeInV,
    int& propsInU, 
    int& propsInV,
    int& numControlPointsInU,
    int& numControlPointsInV,
    OdGePoint3dArray& controlPoints,
    OdGeDoubleArray& weights,
    OdGeKnotVector& uKnots,
    OdGeKnotVector& vKnots) const;

  /** \details
    Sets the parameters for this spline according to the arguments, 
    and returns a reference to this spline.

    \param degreeInU [in]  Degree in the U direction. 
    \param degreeInV [in]  Degree in the V direction.
    \param propsInU [in]  Properties in the U direction.
    \param propsInV [in]  Properties in the V direction.
    \param numControlPointsInU [in]  Number of control points in the U direction.
    \param numControlPointsInV [in]  Number of control points in the V direction.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of weights
    \param uKnots [in]  Knot vector in the U direction.
    \param vKnots [in]  Knot vector in the V direction.
    \param tol [in]  Geometric tolerance.
    \param source [in]  Object to be cloned.
   
    \remarks
    propsInU and propsInV utilize OdGe::NurbSurfaceProperties values.
  */
   OdGeNurbSurface& set(
    int degreeInU, 
    int degreeInV,
    int propsInU, 
    int propsInV,
    int numControlPointsInU,
    int numControlPointsInV,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    const OdGeKnotVector& uKnots,
    const OdGeKnotVector& vKnots,
    const OdGeTol& tol = OdGeContext::gTol);


    OdGeNurbSurface& setFitData(
      const OdGePoint3dArray& fitPoints,
      const OdGeVector3dArray& arrTangentsInU,
      const OdGeVector3dArray& arrTangentsInV, 
      const OdGeVector3dArray& arrMixedDerivs,
      const OdGeKnotVector& uKnots,
      const OdGeKnotVector& vKnots,
      const OdGeTol& tol);
  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  // computes isolines
  
  /** \details
    Returns the isoline for the specified value of V.
    Not implemented for third-party use only.

    \param V [in]  Value of V.
    \param isoline [out]  Receives the isoline for the specified value of V. 
  */
  void computeVIsoLine(
    double V, 
    OdGeNurbCurve3d& isoline) const;

  /** \details
    Returns the isoline for the specified value of U.
    Not implemented for third-party use only.

    \param V [in]  Value of U.
    \param isoline [out]  Receives the isoline for the specified value of U. 
  */
   void computeUIsoLine(
    double U, 
    OdGeNurbCurve3d& isoline) const;

     /** \details
    Returns the 2D pair of parameter values of a point on this surface.

    \param point [in]  Point to be evaluated.
    \param tol [in]  Geometric tolerance.

    \remarks
    The returned parameters specify a point within tol of point.
    If point is not on this surface, the results are unpredictable.
    If you are not sure the point is on this surface, use 
    isOn() instead of this function. This method is slower than paramOf, 
    but more accurate.
     */
   OdGePoint2d paramOfPrec(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;
   

  /** \details
    Returns the derivatives at the point specified by param.
	Not implemented for third-party use only.

    \param param [in]  Parameter to be evaluated.
    \param numDeriv [in]  Number of derivatives to be computed.
    \param derivatives [out]  Receives an array of derivatives at the point corresponding to param.
  */
  bool getDerivativesAt(
    const OdGePoint2d& param, 
    OdUInt32 numDeriv,
    VectorDerivArray derivatives) const;

  // the index in "m_controlPoints" of control *point* (i,j):

  /*!DOM*/
   int loc(
    int i, 
    int j) const;

  /** \details
    Converts source surface to spline with given tolerance and parametrization settings.
    Returns raw pointer to converted spline or null pointer if conversion failed.
    
    \param source [in]  Source surface to convert.
    \param tol [in]  Accuracy of conversion.
    \param sameParametrization [in]  Necessity of parametrization preserving during conversion.
    
    \remarks
    * If source surface already is spline, a copy is returned.
    * If provided null-pointer to source surface or surface has no analytic representation, a null-pointer is returned
  */
  static OdGeNurbSurface* convertFrom(const OdGeSurface* source, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);

protected:
  void calculateNURBSProperties();
};

#include "TD_PackPop.h"

#endif // OD_GENURBSF_H
