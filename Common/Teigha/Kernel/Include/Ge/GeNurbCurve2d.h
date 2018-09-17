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


#ifndef OD_GENURB2d_H
#define OD_GENURB2d_H /*!DOM*/

#include "Ge/GeSplineEnt2d.h"
#include "OdPlatformSettings.h"

class OdGeNurbCurve2dImpl;
class OdGePolyline2d;
class OdGeEllipArc2d;
class OdGeLineSeg2d;
class OdGeKnotVector;

#include "TD_PackPush.h"

/** \details
    This class represents non-uniform rational B-splines (NURBS) in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGeNurbCurve3d.html, Working with NURBS curves>
*/
class GE_TOOLKIT_EXPORT OdGeNurbCurve2d : public OdGeSplineEnt2d
{
public:
  

  /** \param degree [in]  Degree of spline.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of 2D control points.
    \param weights [in]  Array of weights
    \param isPeriodic [in]  True if and only if the spline is to be periodic.
    \param source [in]  Object to be cloned.
    
    \remarks
    * If weights is not specified, a non-rational spline is constructed. 
    * If weights is specified, then a rational spline is constructed, and controlPoints.length() must equal weights.length()
    * If isPeriodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    * If isPeriodic is true, then knots.length() must equal controlPoints.length(), the first and last controlPoints must be equal
      and the first and last weights (if provided) must be equal.
  */
  OdGeNurbCurve2d();
  OdGeNurbCurve2d(
    const OdGeNurbCurve2d& source);
  OdGeNurbCurve2d(
    int degree, 
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& controlPoints,
    bool isPeriodic = false);
  OdGeNurbCurve2d(
    int degree, 
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& controlPoints,
    const OdGeDoubleArray& weights,
    bool isPeriodic = false);
  OdGeNurbCurve2d(
    int degree, 
    const OdGePolyline2d& fitPolyline,
    bool isPeriodic = false);
  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    bool startTangentDefined = true,
    bool endTangentDefined = true,
    const OdGeTol& fitTolerance = OdGeContext::gTol);
  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints,
    const OdGeTol& fitTolerance = OdGeContext::gTol);
  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2dArray& fitTangents,
    const OdGeTol& fitTolerance = OdGeContext::gTol,
    bool isPeriodic = false);
  OdGeNurbCurve2d(
    const OdGeEllipArc2d& ellipse);
  OdGeNurbCurve2d(
    const OdGeLineSeg2d& linSeg);

  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints, 
    const OdGeVector2d& startTangent, 
    const OdGeVector2d& endTangent,
    bool startTangentDefined,
    bool endTangentDefined,
    OdGeKnotParameterization knotParam,
    const OdGeTol& fitTol = OdGeContext::gTol);


  int numFitPoints() const;

  /** \details
    Returns true if and only if  0 <= index < numFitPoints(),
    and returns the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point. 
  */
  bool getFitPointAt(
    int index, 
    OdGePoint2d& point) const;

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
    OdGeVector2d& startTangent,
    OdGeVector2d& endTangent) const;

  /** \details
    Returns true if and only if this spline was constructed with fit data,
    and returns all the fit data used to construct this spline.
    
    \param fitPoints [out]  Receives the interpolation points.
    \param fitTolerance [out]  Receives the geometric tolerance.
    \param tangentsExist [out]  Receives true if and only if tangents were used in constructing this spline.
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
 
    \remarks
    Currently not implemented.
  */
  bool getFitData(
    OdGePoint2dArray& fitPoints,
    OdGeTol& fitTolerance,
    bool& tangentsExist,
    OdGeVector2d& startTangent,
    OdGeVector2d& endTangent) const;

  /** \details
    Returns true if and only if this spline was constructed with fit data,
    and returns all the fit data used to construct this spline.
    
    \param fitPoints [out]  Receives the interpolation points.
    \param fitTolerance [out]  Receives the geometric tolerance.
    \param tangentsExist [out]  Receives true if and only if tangents were used in constructing this spline.
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
    \param knotParam [out]  Receives the knot parameterization.
 
    \remarks
    startTangent and endTangent are meaningful if and only if tangentsExist == true.

    \remarks
    Currently not implemented.
  */
  bool getFitData(
    OdGePoint2dArray& fitPoints,
    OdGeTol& fitTolerance,
    bool& tangentsExist,
    OdGeVector2d& startTangent, 
    OdGeVector2d& endTangent,
    OdGeKnotParameterization& knotParam) const;

  /** \details
    Gets the knot parametrization of the spline created with fit points and returns true if successful.

    \param knotParam [out]  Receives the knot parametrization.

    \remarks
    Currently not implemented.
  */
   bool getFitKnotParameterization(OdGeKnotParameterization& knotParam) const;
  
  /** \details
    Returns all the data that define the spline.
    
    \param degree [out]  Receives the degree of spline.
    \param knots [out]  Receives the knot vector.
    \param controlPoints [out]  Receives an array of 2D control points.
    \param weights [out]  Receives an array of weights.
    \param rational [out]  Receives true if and only if the spline is rational.
    \param periodic [out]  Receives true true if and only if the spline is periodic.

    \remarks
    The weights array will be empty if the spline is not rational.
  */
  void getDefinitionData(
    int& degree, 
    bool& rational,
    bool& periodic,
    OdGeKnotVector& knots,
    OdGePoint2dArray& controlPoints,
    OdGeDoubleArray& weights) const;

  /** \details
    Returns the number of weights in the spline.
    
    \remarks
    Feturns numControlPoints() if this spline is rational, 0 if it is not.
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

  /** \details
    Sets the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point. 
  */
  bool setFitPointAt(
    int index, 
    const OdGePoint2d& point);
  
  /** \details
    Returns true if and only if this spline was created with
    fit data, and inserts the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point. 
  */
  bool addFitPointAt(
    int index, 
    const OdGePoint2d& point);

  /** \details
    Removes a fit point with the specified index. The index must be greater than or equal to 0 and less than or equal to the number of fit points.

    \param index [in]  The index of fit point that is to be removed.
            
    \returns
    Returns true if the fit point is successfully deleted and false otherwise.
  */
  bool deleteFitPointAt(
    int index);

  /** \details
    Sets the fit tolerance that is used for interpolating fit points.

    \param fitTol [in]  Geometric tolerance.
            
    \returns
    Returns true if the tolerance value is successfully set. false is returned if the spline doesn't contain fit data.
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
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent);

  /** \details
    Sets the specified knot parameterization if the spline is created by fit points and returns true if the knot parameterization is successfully set.

    \param knotParam [in]  The knot parameterization to be set.
            
    \remarks
    Currently not implemented.
  */
  bool setFitKnotParameterization(OdGeKnotParameterization knotParam);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
  */
  OdGeNurbCurve2d& setFitData(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
  */
  OdGeNurbCurve2d& setFitData(
    const OdGeKnotVector& fitKnots,
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol,
    bool isPeriodic = false);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
  */
  OdGeNurbCurve2d& setFitData(
    int degree,
    const OdGePoint2dArray& fitPoints,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
  */
  OdGeNurbCurve2d& setFitData(
    const OdGePoint2dArray& fitPoints,                                             
    const OdGeVector2d& startTangent, 
    const OdGeVector2d& endTangent,
    OdGeKnotParameterization knotParam,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Deletes all the fit data used to construct this spline. 
    Returns true if and only if the spline was constructed with fit data.

    \remarks
    The definition of the spline is unchanged. 
  */
  bool purgeFitData();

  /** \details
    Adds new knot to the spline knot array and returns reference to this spline object.

    \param newKnot [in]  New knot.
  */
  OdGeNurbCurve2d& addKnot(
    double newKnot);

  /** \details
    Inserts new knot to the spline knot array and returns reference to this spline object. If there is already such knot, then the multiplicity of the knot is increased by 1.

    \param newKnot [in]  New knot.
  */
  OdGeNurbCurve2d& insertKnot(
    double newKnot);

  /** \details
    Sets the weight at the specified control point.
    
    \param controlPointIndex [in]  Control point index.
    \param weight [in]  The weight to be assigned to that control point.
  */
  OdGeSplineEnt2d& setWeightAt(
    int controlPointIndex, 
    double weight);

  /** \details
    Sets the evaluation mode.

    \remarks
    Currently not implemented.
  */
  OdGeNurbCurve2d& setEvalMode(
    bool evalMode = false);

  /** \details
    Joins the specified spline with this spline and returns a reference to the recomputed spline. 
    Both splines must be open and the start point of the input spline must be equal to the endpoint of the initial spline.
    
    \param curve [in]  Spline to be joined with this spline.  
  */
  OdGeNurbCurve2d& joinWith(
    const OdGeNurbCurve2d& curve);

  /** \details
    Trims the spline by the new start and end parameters, and returns a reference to the recalculated spline. 
    
    \param newStartParam [in]  New start parameter.  
    \param newEndParam [in]  New end parameter.
  */
  OdGeNurbCurve2d& hardTrimByParams(
    double newStartParam,
    double newEndParam);

  /** \details
    Makes this spline rational.
    
    \param weight [in]  The weight to be assigned to each control point.
    
    \remarks
    When called for a non-rational spline, this spline is made rational, and the specified weight is assigned to each control point.
    
    When called for a rational spline, the weight at each control point is multiplied by the specified weight. 
  */
  OdGeNurbCurve2d& makeRational(
    double weight = 1.0);

  /** \details
    Makes the opened spline closed, and returns a reference to this spline. Closed spline is made by creating a smooth spline between the first and the end points and joining it with the initial spline. 
    
    \remarks
    If this spline is already closed, then the method does nothing.
  */
  OdGeNurbCurve2d& makeClosed();

  /** \details
    Makes non-periodic spline periodic. 
    
    \remarks
    Currently not implemented.
  */
  OdGeNurbCurve2d& makePeriodic();

  /** \details
    Makes periodic spline non-periodic. 
    
    \remarks
    Currently not implemented.
  */
  OdGeNurbCurve2d& makeNonPeriodic();

  /** \details
    Makes closed spline open. 
    
    \remarks
    Currently not implemented.
  */
  OdGeNurbCurve2d& makeOpen();

  /** \details
    Increases spline degree by the specified value and returns a reference to this spline object.
    
    \param plusDegree [in]  Degree increment value.
        
  */
  OdGeNurbCurve2d& elevateDegree(
    int plusDegree);

  OdGeNurbCurve2d& operator =(
    const OdGeNurbCurve2d& spline);

  /** \details
    Adds a control point at the specified knot.
    
    \param newKnot [in]  The knot parameter to add the control point at.
    \param point [in]  Coordinates of the control point to be added.
    \param weight [in] The weight to be assigned to the control point.
    
    \remarks
    When called for a non-rational spline, weight is ignored.

    \remarks
    Currently not implemented.
  */
  bool addControlPointAt(double newKnot, const OdGePoint2d& point, double weight = 1.0);

  /** \details
    Deletes a control point at the specified index.
    
    \param index [in]  The index of the control point to be deleted.
        
    \remarks
    Currently not implemented.
  */
  bool deleteControlPointAt(int index);


  //////////////////////////////////////////////////////////////////////////
  // Overrides :

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
    Gets the intersection points well as the parameters of this spline said points.
        
    \returns
    Returns true if and only if this spline intersects with the specified linear entity.  

    \param line [in]  Any 2D linear entity.
    \param pnts2D [out]  Receives an array of 2D points.
    \param tol [in]  Geometric tolerance.
    \param params [out]  Receives an array of parameters.
  */
  bool intersectWith(
    const OdGeLine2d &line2d, 
    OdGePoint2dArray &pnts2d, 
    const OdGeTol& tol = OdGeContext::gTol, 
    OdGeDoubleArray *params = NULL) const;

  /** \details
  Sets the parameters for this spline according to the arguments, 
  and returns a reference to this spline.

  \param degree [in]  Degree of spline.
  \param knots [in]  Knot vector.
  \param controlPoints [in]  Array of 2D control points.
  \param weights [in]  Array of weights
  \param isPeriodic [in]  True if and only if the spline is to be periodic (closed).

  \remarks
  * If weights is not specified, a non-rational spline is constructed. 
  * If weights is specified, then a rational spline is constructed, and controlPoints.length() must equal weights.length()
  * If isPeriodic is false, then knots.length() must equal controlPoints.length() + degree + 1
  * If isPeriodic is true, then knots.length() must equal controlPoints.length(), the first and last controlPoints must 
  be equal, and the first and last weights (if provided) must be equal.
  */
  void set(
    int degree, 
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& controlPoints, 
    const OdGeDoubleArray& weights, 
    bool isPeriodic = false);
  
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
  static OdGeNurbCurve2d* convertFrom(const OdGeCurve2d* source, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);

protected:
  friend class OdDbGeEdgesDwgIO;
  friend class OdDbGeEdgesDxfIO;

  void set(int degree, const OdGeKnotVector& knots,
    const OdGePoint2dArray& cntrlPnts,  const OdGeDoubleArray& weights, bool isPeriodic, 
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent);
};

#include "TD_PackPop.h"

#endif

