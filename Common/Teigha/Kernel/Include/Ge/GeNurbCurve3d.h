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

#ifndef OD_GENURB3D_H
#define OD_GENURB3D_H /*!DOM*/

class OdGeEllipArc3d;
class OdGeLineSeg3d;
class OdGePolyline3d;
class OdGeKnotVector;
class OdGeNurbCurve3dImpl;

#include "Ge/GeSplineEnt3d.h"
#include "OdPlatformSettings.h"

#include "TD_PackPush.h"

/** \details
    This class represents non-uniform rational B-splines (NURBS) in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeNurbCurve3d.html, Working with NURBS Curves>
*/
class GE_TOOLKIT_EXPORT OdGeNurbCurve3d : public OdGeSplineEnt3d
{
public:


  /** \param degree [in]  Degree of spline.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of weights
    \param isPeriodic [in]  True if and only if this spline is to be periodic.
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param ellipse [in]  Any elliptical arc.
    \param numSpans [in]  Number of knot spans in the curve.
    \param lineSeg [in]  Any 3D line segment.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
    \param startTangentDefined [in]  If true, startTangent is used.
    \param endTangentDefined [in]  If true, endTangent is used.
    \param source [in]  Object to be cloned.
    \param numControlPoints [in]  Number of control points.
    \param numWeights [in]  Number of weights.
    
    \remarks
    * If weights is not specified, a non-rational spline is constructed. 
    * If weights is specified, then a rational spline is constructed, and controlPoints.length() must equal weights.length()
    * If isPeriodic is true, the constructor forces makeClosed functionality(if the curve is open, it will be closed inside constructor)
      and the first and last weights (if provided) must be equal.
    * If ellipse is specified, a curve identical to the ellipse is created.
    * If lineSeg is specified, a curve identical to the line segment is created.
  */
  OdGeNurbCurve3d();
  OdGeNurbCurve3d(
    const OdGeNurbCurve3d& source);
  OdGeNurbCurve3d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    bool isPeriodic = false);
  OdGeNurbCurve3d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    bool isPeriodic = false);
  // TD Special
  OdGeNurbCurve3d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3d* controlPoints,
    OdUInt32 numControlPoints,
    const double* weights,
    OdUInt32 numWeights,
    bool isPeriodic = false);
  //
  OdGeNurbCurve3d(
    int degree, 
    const OdGePolyline3d& fitPolyline,
    bool isPeriodic = false);
  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    bool startTangentDefined = true,
    bool endTangentDefined = true,
    const OdGeTol& fitTol = OdGeContext::gTol);
  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints,
    const OdGeTol& fitTolerance = OdGeContext::gTol);
  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3dArray& fitTangents,
    const OdGeTol& fitTolerance = OdGeContext::gTol,
    bool isPeriodic = false);

  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints, 
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent,
    bool startTangentDefined,
    bool endTangentDefined,
    OdGeKnotParameterization knotParam,
    const OdGeTol& fitTolerance = OdGeContext::gTol);
  // numSpans - the number of knot spans in nurbs curve
  // if numSpans == 0 (default) it is automatically calculated from 
  // ellipse domain


  OdGeNurbCurve3d(
    const OdGeEllipArc3d& ellipse, 
    int numSpans = 0);

  OdGeNurbCurve3d(
    const OdGeLineSeg3d& lineSeg);

  /** \details
    Returns the number of fit points.
  */
   int numFitPoints() const;

  /** \details
    Gets the knot parametrization of the spline created with fit points and returns true if successful.

    \param knotParam [out]  Receives the knot parametrization.
  */
   bool getFitKnotParameterization(OdGeKnotParameterization& knotParam) const;

  /** \details
    Returns true if and only if  0 <= index < numFitPoints(),
    and returns the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point. 
  */
   bool getFitPointAt(
    int fitPointIndex, 
    OdGePoint3d& point) const;

  /** \details
    Returns true if and only if this spline was constructed
    with fit points, and gets the fit tolerance that is
    used to construct this spline.
    
    \param fitTolerance [out]  Recieves the geometric tolerance.
  */
   bool getFitTolerance(
    OdGeTol& fitTolerance) const;

  /** Descripton:
    Returns true if and only if this spline was constructed with fit data, and
    returns the starting and ending tangents.
    
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
  */
   bool getFitTangents(
    OdGeVector3d& startTangent,
    OdGeVector3d& endTangent) const;
 
  /** Descripton:
    Returns true if and only if this spline was constructed with fit data, and
    returns the starting and ending tangents.
    
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
    \param startTangentDefined [out]  Receives the if true, startTangent was used.
    \param endTangentDefined [out]  Receives the if true, endTangent was used.
  */
  bool getFitTangents(
    OdGeVector3d& startTangent,
    OdGeVector3d& endTangent,
    bool& startTangentDefined,
    bool& endTangentDefined) const;

  /** \details
    Returns true if and only if this spline was constructed with fit data,
    and returns all the fit data used to construct this spline.
    
    \param fitPoints [out]  Receives the onterpolation points.
    \param fitTolerance [out]  Receives the geometric tolerance.
    \param tangentsExist [out]  Receives true if and only if tangents were used in constructing this spline.
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
 
    \remarks
    startTangent and endTangent are meaningful if and only if tangentsExist == true.
  */
   bool getFitData(
    OdGePoint3dArray& fitPoints,
    OdGeTol& fitTolerance,
    bool& tangentsExist,
    OdGeVector3d& startTangent,
    OdGeVector3d& endTangent) const;

  /** \details
    Returns true if and only if this spline was constructed with fit data,
    and returns all the fit data used to construct this spline.
    
    \param fitPoints [out]  Receives the onterpolation points.
    \param fitTolerance [out]  Receives the geometric tolerance.
    \param tangentsExist [out]  Receives true if and only if tangents were used in constructing this spline.
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
    \param knotParam [out]  Receives the knot parameterization.
 
    \remarks
    startTangent and endTangent are meaningful if and only if tangentsExist == true.
  */
   bool  getFitData(
     OdGePoint3dArray& fitPoints,
     OdGeTol& fitTolerance,
     bool& tangentsExist,
     OdGeVector3d& startTangent, 
     OdGeVector3d& endTangent,
     OdGeKnotParameterization& knotParam) const;

  // NURBS data query functions

  /** \details
    Returns the data used to define this spline.

    \param degree [out]  Receives the degree of spline.
    \param periodic [out]  Receives true if and only if this spline is periodic.
    \param rational [out]  Receives true if and only if this spline is rational.
    \param knots [out]  Receives the knot vector.
    \param controlPoints [out]  Receives an array of 3D control points.
    \param weights [out]  Receives an array of weights
    
    \remarks
    The weights array will be empty if the spline is not rational.
  */
   void getDefinitionData(
    int& degree,
    bool& rational,
    bool& periodic,
    OdGeKnotVector& knots,
    OdGePoint3dArray& controlPoints,
    OdGeDoubleArray& weights) const;

  /** \details
    Returns the number of weights in the spline.
    
    \remarks
    Returns numCurves() if this spline is rational, 0 if it is not.
  */
   int numWeights() const;

  /** \details
    Returns the weight at the specified index.

    \param weightIndex [in]  Weight index.
  */
   double weightAt(
    int weightIndex) const;

  /** \details
    Returns the current evaluation mode.

    \remarks
    Currently not implemented.
  */
   bool evalMode() const;

   /** \remarks
    Currently not implemented.
   */
   bool getParamsOfC1Discontinuity(
    OdGeDoubleArray& params,
    const OdGeTol& tol = OdGeContext::gTol) const;

   /** \remarks
    Currently not implemented.
   */
   bool getParamsOfG1Discontinuity(
    OdGeDoubleArray& params,
    const OdGeTol& tol = OdGeContext::gTol) const;

  // Fit data edit functions

  /** \details
    Sets the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point. 
	
    \returns
    Returns true if the fit point has successfully been set and false otherwise.
  */
   bool setFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);

  /** \details
    Returns true if and only if this spline was created with
    fit data, and inserts the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point. 
  */
   bool addFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);

  /** \details
    Adds a control point at the specified knot.
    
    \param newKnot [in]  The knot parameter to add the control point at.
    \param point [in]  Coordinates of the control point to be added.
    \param weight [in] The weight to be assigned to the control point.
    
    \remarks
    When called for a non-rational spline, weight is ignored.
	
	  \returns
    Returns true if the control point has successfully been added and false otherwise.
  */
   bool addControlPointAt(double newKnot, const OdGePoint3d& point, double weight = 1.0);

  /** \details
    Deletes a control point at the specified index.
    
    \param index [in]  The index of the control point to be deleted.
	
	  \returns
    Returns true if the control point has successfully been deleted and false otherwise.
  */
   bool deleteControlPointAt(int index);

  /** \details
    Removes a fit point with the specified index. The index must be greater than or equal to 0 and less than or equal to the number of fit points.

    \param index [in]  The index of fit point that is to be removed.
            
    \returns
    Returns true if the fit point is successfully deleted and false otherwise.
  */
   bool deleteFitPointAt(
    int fitPointIndex);

  /** \details
    Sets the specified knot parameterization if the spline is created by fit points and returns true if the knot parameterization has successfully been set.

    \param knotParam [in]  The knot parameterization to be set.
  */
   bool setFitKnotParameterization(OdGeKnotParameterization knotParam);

  /** \details
    Sets the fit tolerance that is used for interpolating fit points.

    \param fitTol [in]  Geometric tolerance.
            
    \returns
    Returns true if and only if this spline was constructed with fit data.
  */
   bool setFitTolerance(
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit tangents for this spline.
    
     \param startTangent [in]  Starting tangent.
     \param endTangent [in]  Ending tangent.

    \returns
    Returns true if and only if this spline was constructed with fit data. 
  */
   bool setFitTangents(
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent);
 
  /** \details
    Sets the fit tangents for this spline.
    
     \param startTangent [in]  Starting tangent.
     \param endTangent [in]  Ending tangent.
     \param startTangentDefined [in]  Defines if the specified value of starting tangent is to be used.
     \param endTangentDefined [in]  Defines if the specified value of starting tangent is to be used.

    \returns
    Returns true if and only if this spline was constructed with fit data. 
  */  
  bool setFitTangents(
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    bool startTangentDefined,
    bool endTangentDefined);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
  */
  OdGeNurbCurve3d& setFitData(
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
    \param fitKnots [in] Fit knots.

    \remarks
    The signature with fitKnots parameter currently is not implemented.
  */  
  OdGeNurbCurve3d& setFitData(
    const OdGeKnotVector& fitKnots,
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol,
    bool isPeriodic = false);

    
  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
  */
   OdGeNurbCurve3d& setFitData(
    int degree,
    const OdGePoint3dArray& fitPoints,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
  */
   OdGeNurbCurve3d& setFitData(
     const OdGePoint3dArray& fitPoints,                                             
     const OdGeVector3d& startTangent, 
     const OdGeVector3d& endTangent,
     OdGeKnotParameterization knotParam,
     const OdGeTol& fitTol = OdGeContext::gTol);

  
  /** \details
    Purges the fit data defining this spline.
    
    \remarks
    The fit data consists of the fit points, fit tolerance,
    start tangent, and end tangent.
  */
   bool purgeFitData();

  /** \details
    Adds new knot to the spline knot array and returns reference to this spline object.

    \param newKnot [in]  New knot.
  */
   OdGeNurbCurve3d& addKnot(double newKnot);


  /** \details
    Inserts new knot to the spline knot array and returns reference to this spline object. If there is already such knot, then the multiplicity of the knot is increased by 1.

    \param newKnot [in]  New knot.
  */
   OdGeNurbCurve3d& insertKnot(double newKnot);

  /** \details
    Sets the weight at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param weight [in]  Weight at the specified fit point. 
  */
   OdGeSplineEnt3d& setWeightAt(
    int fitPointIndex, 
    double weight);

  /** \details
    Sets the evaluation mode.

    \remarks
    Currently not implemented.
  */
   OdGeNurbCurve3d& setEvalMode(
    bool evalMode = false);

  /** \details
    Joins the specified spline with this spline and returns a reference to the recomputed spline. 
    Both splines must be open and the start point of the input spline must be equal to the endpoint of the initial spline.
    
    \param curve [in]  Spline to be joined with this spline.  
  */
   OdGeNurbCurve3d& joinWith(
    const OdGeNurbCurve3d& curve);

  /** \details
    Trims the spline by the new start and end parameters, and returns a reference to the recalculated spline. 
    
    \param newStartParam [in]  New start parameter.  
    \param newEndParam [in]  New end parameter.
  */
   OdGeNurbCurve3d& hardTrimByParams(
    double newStartParam,
    double newEndParam);

  /** \details
    Makes this spline rational (if it is not), and returns a reference to this spline.
    
    \param weight [in]  Weight to be applied to each control point. 
    
    \remarks
    If this spline was already rational, the weight at each control point is multiplied by the
    specified weight.
  */
   OdGeNurbCurve3d& makeRational(
    double weight = 1.0);

  /** \details
    Makes the opened spline closed, and returns a reference to this spline. Closed spline is made by creating a smooth spline between the first and the end points and joining it with the initial spline. 
    
    \remarks
    If this spline is already closed, then the method does nothing.
  */
   OdGeNurbCurve3d& makeClosed();

  /** \details
    Makes non-periodic spline periodic. 
    
    \remarks
    Currently not implemented.
  */
   OdGeNurbCurve3d& makePeriodic();

  /** \details
    Makes periodic spline non-periodic. 
    
    \remarks
    Currently not implemented.
  */
   OdGeNurbCurve3d& makeNonPeriodic();

  /** \details
    Makes closed spline open. 
    
    \remarks
    Currently not implemented.
  */
   OdGeNurbCurve3d& makeOpen();

   /** \details
    Increases spline degree by the specified value and returns a reference to this spline object.
    
    \param plusDegree [in]  Degree increment value.
   */
  OdGeNurbCurve3d& elevateDegree(int plusDegree);

  // Assignment operator.
  OdGeNurbCurve3d& operator =(
    const OdGeNurbCurve3d& spline);

  ///////////////////////////////////////////////////////////////////////////
  // TD Special :

  TD_USING(OdGeSplineEnt3d::evalPoint);

  // This is only for internal use
  /*!DOM*/
   OdGePoint3d evalPoint(
    double param, 
    int hint) const;  

  // This is need in surface (for rational case only)
  /*!DOM*/
  void evalPointDivider(
    double param, 
    OdGePoint3d& point, 
    double& divider, 
    int hint) const;

  // NURBS data edit functions
  
  /** \details
    Sets the parameters for this spline according to the arguments. 

    \param degree [in]  Degree of spline.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of weights
    \param isPeriodic [in]  True if and only if this spline is to be periodic.
    
    \returns
    Returns a reference to this spline.
    
    * A rational spline is constructed, and controlPoints.length() must equal weights.length()
    * If isPeriodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    * If isPeriodic is true, then knots.length() must equal controlPoints.length(), the first and last controlPoints must be equal
      and the first and last weights (if provided) must be equal.
  */
   void set(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    bool isPeriodic = false);

   /** \details
    Calculates fit data for the spline constructed by control points. 
        
    \remarks
    Currently the method calculates fit data only for NURB curves of degree less than or equal to three.  

    \returns
    Returns true if and only if fit data have successfully been calculated.
   */
   bool buildFitData(OdGeKnotParameterization knotParam = OdGe::kChord);

  /** \details
    Converts source curve to spline with given tolerance and parametrization settings.
    Returns raw pointer to converted spline or null pointer if conversion failed.
    
    \param source [in]  Source curve to convert.
    \param tol [in]  Accuracy of conversion.
    \param sameParametrization [in]  Necessity of parametrization preserving during conversion.
    
    \remarks
    * If source curve already is spline, a copy is returned.
    * If provided null-pointer to source curve or curve has no analytic representation, a null-pointer is returned
  */
  static OdGeNurbCurve3d* convertFrom(const OdGeCurve3d* source, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);
   
protected:
   /** \details
    Modifies the position and tangent of the point with a specified parameter.

    \param param [in]  Parameter value of the point.
    \param point [in]  New position of the point.
    \param deriv [in]  New value of the tangent vector at the point.
           
    \returns
    Returns true if and only if the point has successfully been adjusted.
   */
   bool modifyPositionAndTangent(double param, const OdGePoint3d& point, const OdGeVector3d* deriv);
   void makeClosedFit();
   
protected:

  void setData(int degree, const OdGePoint3dArray& controlPoints, const OdGeKnotVector& knots, const OdGeDoubleArray& weights
    , const OdGePoint3dArray& fitPoints, const OdGeVector3d& startTangent, const OdGeVector3d& endTangent
    , bool isStartTangentDefined, bool isEndTangentDefined, bool isPeriodic, const OdGeTol& fitTol);

   void setKnotTolerance(double tol);
};

#include "TD_PackPop.h"

#endif // OD_GENURB3D_H

