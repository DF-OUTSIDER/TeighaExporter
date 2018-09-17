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


#ifndef OD_GE_INTERVAL_H
#define OD_GE_INTERVAL_H /*!DOM*/

#include "Ge/GeExport.h"
#include "TD_PackPush.h"

/** \details
    This class represents a finite, infinite, or semi-infinite
    interval as the real axis.
  
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeInterval
{
public:
  /** \param lower [in]  Lower bound.
    \param upper [in]  Upper bound.
    \param tol [in]  Boundary tolerance.
    \param boundedBelow [in]  Determines if the interval is bounded below.
    \param bound [in]  Specifies a lower and upper bounds if boundedBelow is true, or only an upper
              bound if it is false.     

    \remarks
    With no arguments other than tol, creates an unbounded interval.
  */
  OdGeInterval(
    double tol = 1.e-12);
  OdGeInterval(
    double lower,
    double upper,
    double tol = 1.e-12);
  OdGeInterval(
    bool boundedBelow,
    double bound,
    double tol = 1.e-12);

  ~OdGeInterval();

  /** \details
    Returns the lower bound of this interval.
    
    \remarks
    This value is meaningful if and only if the interval has a lower bound.
  */
  double lowerBound() const;

  /** \details
    Returns the upper bound of this interval.
    
    \remarks
    This value is meaningful if and only if the interval has an upper bound.
  */
  double upperBound() const;

  /** \details
    Returns a point on this interval.
    
    \remarks
    <table>
    Bounded Above   Bounded Below          Returns
    Yes             ---                    Upper Bound
    No              Yes                    Lower Bound
    No              No                     0.0
    </table>

  */
    double element() const;

  /** \details
    Returns the bounds of this interval.
    
    \param lower [out]  Receives the Lower bound.
    \param upper [out]  Receives the Upper bound.
  */
  void getBounds(
    double& lower,
    double& upper) const;
    
  /** \details
  Returns the parameter of interval's end.

  \param index  Must be 0 (interval start) or 1 (interval end), otherwise behavior is undefined.
  */
  double getEnd(
    int index) const;

  /** \details
    Returns the length of this interval.
    
    \returns
    Returns -1.0 if this interval is unbounded above or below.
  */
  double length() const;
  
  /** \details
    Returns the boundary tolerance for this interval.
  */
  double tolerance() const;

  /** \details
  Returns parameter L + (R-L) * t for given ratio t.
  Note: if the interval is unbounded, behavior is undefined!

  \param ratio  Arbitrary number
  */
  double eval(
    double ratio) const;

  /** \details
    Sets the parameters for this interval according to the arguments.

    \param lower [in]  Lower bound.
    \param upper [in]  Upper bound.
              
    \returns
    Returns a reference to this interval. 
  */
  OdGeInterval& set(
    double lower,
    double upper);
  
  /** \details
    Sets the parameters for this interval according to the arguments.

    \param boundedBelow [in]  Determines if bound specifies a lower or an upper bound.
    \param bound [in]  Specifies a lower bound if boundedBelow is true, or an upper
              bound if it is false.
              
    \returns
    Returns a reference to this interval.   
  */
  OdGeInterval& set(
    bool boundedBelow,
    double bound);
  
  /** \details
    Unbounds this interval.
    
    \returns
    Returns a reference to this interval.   
  */
  OdGeInterval& set();

  /** \details
    Sets the upper bound for this interval.

    \param upper [in]  Upper bound.
    
  */
  OdGeInterval& setUpper(
    double upper);
    
  /** \details
    Sets the lower bound for this interval.

    \param lower [in] Lower bound.
    \returns
    Returns a reference to this interval.
  */
  OdGeInterval& setLower(
    double lower);

  /** \details
    Sets the boundary tolerance for this interval.

    \param tol [in]  Boundary tolerance.

    \returns
    Returns a reference to this interval.
  */
  OdGeInterval& setTolerance(
    double tol);

  /** \details
    Returns the smallest interval containing both the specified interval and this interval.

    \param otherInterval [in]  The interval to be merged with this one.
    \param result [out]  Receives the merged interval.
  */
  void getMerge(
    const OdGeInterval& otherInterval,
    OdGeInterval& result) const;
    
  /** \details
    Subtracts the specified interval from this interval.
    \returns
    Returns the number of intervals, and the intervals, resulting from the subtraction

    \param otherInterval [in]  The interval to be subtracted from this one.
    \param lInterval [out]  Receives the Left (or only) interval.
    \param rInterval [out]  Receives the right interval.
    
    \remarks
    <table>
    return value   Results
    0              Empty result
    1              Single interval in lInterval
    2              Left in lInterval, Right in rInterval.
    </table>
  */
  int subtract(
    const OdGeInterval& otherInterval,
    OdGeInterval& lInterval,
    OdGeInterval& rInterval) const;

  /** \details
    Intersects the specified interval from this one.
    \returns
    Returns true if and only this specified interval intersects with this one, 
    and the interval of intersection.

    \param otherInterval [in]  The interval to be intersected.
    \param result [out]  Receives the intersected interval.
  */
  bool intersectWith(
    const OdGeInterval& otherInterval,
    OdGeInterval& result) const;

  /** \details
    Find finite sub-interval belonging both to the specified range and this one
    \returns
    Returns true if and only if the specified interval intersects with this one, 
    and the resulting interval is bounded.

    \param range [in]  The interval to be intersected.
    \param result [out]  Receives the intersected interval.
  */
  bool finiteIntersectWith(const OdGeInterval& range, OdGeInterval& result) const;
    
  /** \details
    Returns true if and only if this interval is bounded above and below.
  */
  bool isBounded() const;
    
  /** \details
    Returns true if and only if this interval is bounded above.
  */
  bool isBoundedAbove() const;
    
  /** \details
    Returns true if and only if this interval is bounded below.
  */
  bool isBoundedBelow() const;

  /** \details
    Returns true if and only if this interval is unbounded above or below.
  */
  bool isUnBounded() const;
  
  /** \details
    Returns true if and only if this interval object is a singleton.
    \remarks
    Returns true if and only if this interval is bounded, and the upper and lower bounds are equal within tolerance().
  */
  bool isSingleton() const;

  /** \details
    Returns true if and only if the specified interval is disjoint from this interval.
    \remarks
    Returns true if and only if this interval does not intersect otherInterval within tolerance().
    
    \param otherInterval [in]  The interval to be tested.
  */
  bool isDisjoint(
    const OdGeInterval& otherInterval) const;

  /** \details
    Returns true if and only if this interval contains the specified value or Interval.
    \remarks
    Returns true if and only if this Interval contains value or otherInterval within tolerance().
    
    \param otherInterval [in]  The Interval to be tested.
    \param value [in]  The value to be tested.
  */
  bool contains(
    const OdGeInterval& otherInterval) const;
  bool contains(
    double value) const;

  /** \details
  Corrects the parameter so that it belongs to the interval.
  If parameter is already within interval, then it is returned as is.
  Otherwise parameter is moved to closest of endpoints.

  \param value [in]  The parameter to be corrected.
  */
  double clamp(
    double value) const;

  /** \details
    Returns true if and only if the specified Interval is continuous with the upper bound of this Interval.
    \remarks
    Returns true if and only if this Interval is bounded above, otherInterval is bounded below, 
    and the upper bound of this Interval is equal to the lower bound of otherInterval within tolerance().
    
    \param otherInterval [in]  The Interval to be tested.
  */
  bool isContinuousAtUpper(
    const OdGeInterval& otherInterval) const;

  /** \details
    Returns true if and only if the specified Interval and this Interval overlap at the upper bound of this Interval.
    \remarks
    Returns true if and only if
     
    1.  This Interval is bounded above, otherInterval is bounded below. 
    2.  Neither Interval is a singleton.
    3.  The Intervals Intersect.
    4.  otherInterval does not contain this one.
    5.  The upper bound of this Interval is contained in otherInterval.
    6.  The lower bound of otherInterval is contained within this one. 
    
    If all these conditions are met, this function returns the intersection of the Intervals.
    
    \param otherInterval [in]  The Interval to be tested.
    \param overlap [out]  Receives the overlap of the Intervals.
  */
  bool isOverlapAtUpper(
    const OdGeInterval& otherInterval,
    OdGeInterval& overlap) const;

  bool operator ==(
    const OdGeInterval& otherInterval) const;
  bool operator !=(
    const OdGeInterval& otherInterval) const;
    
  /** \details
    Returns true if and only if upper bounds of the specified Interval and this Interval are equal.
    \remarks
    
    Returns true if and only if
    
    1. Both this Interval and otherInterval are unbounded above or bounded 
       above with their upper bounds equal within tolerance().

    \param otherInterval [in]  The Interval to be tested.
  */
  bool isEqualAtUpper(
    const OdGeInterval& otherInterval) const;

  /** \details
    Returns true if and only if upper bounds of the specified Interval and this Interval are equal.
    \remarks
    
    Returns true if and only if

    1. This Interval is bounded above, and the upper bound is equal to value within tolerance(). 

    \param value [in]  The value to be tested.
  */
  bool isEqualAtUpper(
    double value) const;
    
  /** \details
    Returns true if and only if lower bounds of the specified Interval and this Interval are equal.
    \remarks
    Returns true if and only if
    
    1. Both this Interval and otherInterval are unbounded below or bounded 
       below with their lower bounds equal within tolerance.
    
    \param otherInterval [in]  The Interval to be tested.
  */
  bool isEqualAtLower(
    const OdGeInterval& otherInterval) const;
  
  /** \details
    Returns true if and only if lower bounds of the specified Interval and this Interval are equal.
    \remarks
    Returns true if and only if
    
    1. This Interval is bounded below, and the lower bound is equal to value within tolerance. 

    \param value [in]  The value to be tested.
  */
  bool isEqualAtLower(
    double value) const;

  /** \details
    Returns true if and only if the specified value is periodically on this Interval.
    \remarks
    Returns true if and only if there is a positive integer N such that
    
            value + N * period
            
    is on this Interval. If so, returns that value.         
    
    \param period [in]  Period.
    \param value [in/out] Value.
  */
  bool isPeriodicallyOn(
    double period,
    double& value);

  friend 
    bool operator > (
    double value,
    const OdGeInterval& interval);
  bool operator > (
    double value) const;
  bool operator > (
    const OdGeInterval& otherInterval) const;
  friend 
    bool operator >= (
    double value,
    const OdGeInterval& interval);
  bool operator >= (
    double value) const;
  bool operator >= (
    const OdGeInterval& otherInterval) const;
  friend 
    bool operator < (
    double value,
    const OdGeInterval& interval);
  bool operator < (
    double value) const;
  bool operator < (
    const OdGeInterval& otherInterval) const;
  friend 
    bool operator <= (
    double value,
    const OdGeInterval& interval);
  bool operator <= (
    double value) const;
  bool operator <= (
    const OdGeInterval& otherInterval) const;

private:
  double m_Tol;
  double m_UpperParam;
  double m_LowerParam;
  bool m_bBoundedAbove;
  bool m_bBoundedBelow;
};

#include "TD_PackPop.h"
#endif // OD_GE_INTERVAL_H

