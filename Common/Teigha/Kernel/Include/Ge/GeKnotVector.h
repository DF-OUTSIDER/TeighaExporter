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


#ifndef OD_GE_KNOT_VECTOR
#define OD_GE_KNOT_VECTOR /*!DOM*/

#include "Ge/GeExport.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GeIntArray.h"

class OdGeInterval;

#include "TD_PackPush.h"

/** \details
    This class represents an ordered series of monotonically increasing doubles used by spline entities.
 
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeKnotVector
{
public:
  /** \param tol [in]  Knot equality tolerance.
    \param size [in]  Length of vector.
    \param growSize [in]  Length of future vector size increments.
    \param plusMult [in]  Multiplicity increment for each knot.
    \param source [in]  Object to be cloned.

  */
  OdGeKnotVector(
    double tol = 1.e-9); 
  OdGeKnotVector(
    int size, 
    int growSize, 
    double tol = 1.e-9);
  OdGeKnotVector(
    int size, 
    const double source[], 
    double tol = 1.e-9);
  OdGeKnotVector(
    int plusMult, 
    const OdGeKnotVector& source);
  OdGeKnotVector(
    const OdGeKnotVector& source);
  OdGeKnotVector(
    const OdGeDoubleArray& source, 
    double tol = 1.e-9);
  ~OdGeKnotVector();

  OdGeKnotVector& operator =(
      const OdGeKnotVector& knotVector);
  OdGeKnotVector& operator =(
      const OdGeDoubleArray& dblArray);

  /** \param i [in]  Index of knot.
    
    \returns
    Returns or references the ith knot of the knot vector.
  */
  double& operator [](
    int i);
  double operator [](
    int i) const;
   
  /** \details
    Returns true if and only if knotVector is identical to this one.
    
    \param knotVector [in]  Knot vector. 
  */
  bool isEqualTo(
    const OdGeKnotVector& knotVector) const;

  /** \details
    Returns the first knot value of this vector.
  */
  double startParam() const;

  /** \details
    Returns the last knot value of this vector.
  */
  double endParam() const;
  
  /** \details
    Returns knot multiplicity (repetitions) of the knot value at the specified index.
    
    \param knotIndex [in]  Knot index.
    
    \remarks
    If consecutive knots are within the knot equality tolerance,
    the knots are considered identical, and their multiplicities combined.
  */
  int multiplicityAt(
    int knotIndex) const;
    
  /** \details
    Returns the number of intervals between distinct knots. 

    \remarks
    Consecutive knots are considered distinct if and only if
    they are not within the knot equality tolerance.
  */
  int numIntervals() const;
  
  /** \details
    Returns the knot interval, and the index of the knot interval, containing the point specified by param. 

    \param order [in]  The order of the spline.
    \param param [in]  Parameter to specify a point on the vector.
    \param interval [out]  Receives the interval containing the point specified by param.
    \remarks
    param must lie between the knot values indexed by order -1 and length() - order, where order is the order of the spline. 
  */
  int getInterval(
    int order, 
    double param, 
    OdGeInterval& interval) const;

  /** \details
    Returns the number of distinct knots. 
    
    \param knots [in]  Array of knots.
    \param multiplicity [out]  Receives an array of multiplicities (repetitions) of each knot.

    \remarks
    If consecutive knots are within the knot equality tolerance,
    the knots are considered identical, and their multiplicities combined.
  */
      void getDistinctKnots(
    OdGeDoubleArray& knots, 
    OdGeIntArray *multiplicity = NULL) const;

  /** \details
    Returns true if an only if the specified parameter is
    between the first and last knots.
    
    \param param [in]  Parameter to be tested. 
  */
  bool contains(
    double param) const;

  /** \details
    Returns true if and only if knot is a member of this vector within the knot equality tolerance.
  */
  bool isOn(
    double knot) const;

  /** \details
    Reverses the order of this vector, and returns a reference to this vector.
  */
  OdGeKnotVector& reverse();

  /** \details
    Removes the specified knot from this vector, and returns a reference to this vector.
    
    \param knotIndex [in]  Knot index.
  */
  OdGeKnotVector& removeAt(
    int knotIndex);

  /** \details
    Removes the specified range of knots from this vector, and returns a reference to this vector.
    
    \param startIndex [in]  Index of the first knot to be removed.
    \param endIndex [in]  Index of the last knot to be removed.
  */
  OdGeKnotVector& removeSubVector(
    int startIndex, 
    int endIndex);

  /** \details
    Inserts the specified knot the specified number of times at the specified index, and returns a reference to 
    this vector.
    
    \param knotIndex [in]  Knot index.
    \param knot [in]  Value to be inserted
    \param multiplicity [in]  Number ot times to insert the knot.
  */
  OdGeKnotVector& insertAt(
    int knotIndex, 
    double knot, 
    int multiplicity = 1);

  /** \details
    Inserts a knot in the appropriate knot interval as specified
    by param, and returns a reference to this vector.
    
    \param param [in]  Parameter to specify a point on the vector.
    
    \remarks
    If the specified point is within the knot equality tolerance of another knot,
    said knot's multiplicity is incremented.
  */
    OdGeKnotVector& insert(
    double param);

  /** \details
    Appends a single knot to this vector, and returns the value of the single knot. 
   
    \param knot [in]  New last knot value.
  */
  int append(
    double knot);

  /** \details
    Appends a vector to this vector, and returns a reference to this vector. 
   
    \param tail [in]  Knot vector to be appended.
    \param knotRatio [in]  Knot ratio.
    
    \remarks
    If knotRatio > 0, append performs a linear transformations on this vector and on tail,
    such that the ratio of their lengths is equal to knotRatio, and that tail immediately follows
    this vector. tail is modified by this operation.
  */
  OdGeKnotVector& append(
    OdGeKnotVector& tail, 
    double knotRatio = 0.0);

  /** \details
    
    Splits this vector at the point corresponding to param.
    
    \param param [in]  Parameter to specify a point on the vector.
    \param pKnotHead [out]  Receives the head portion of the split.
    \param multLast [in]  Multiplicity of the last knot in the head portion.
    \param pKnotTail [out]  Receives the tail portion of the split.
    \param multFirst [in]  Multiplicity of the first knot in the tail portion.
  */
  int split(
    double param,
    OdGeKnotVector* pKnotHead,
    int multLast,
    OdGeKnotVector* pKnotTail,
    int multFirst) const;

  /** \details
    Transforms this vector such that the first knot has a value
    of lower, and the last knot has a value of upper, and 
    returns a reference to this vector. 
  
    \param lower [in]  New lower knot.
    \param upper [in]  New upper knot.
  */
  OdGeKnotVector& setRange(
    double lower, 
    double upper);

  /** \details
    Returns the knot equality tolerance.
  */
  double tolerance() const;
  
  /** \details
    Sets the knot equality tolerance for this vector, 
    and returns a reference to this vector.

     \param tol [in]  Knot equality tolerance.
  */
  OdGeKnotVector& setTolerance(
    double tol); 

  /** \details
    Returns the length of this vector.
  */
  int length() const;

  /** \details
    Returns true if and only if length() == 0.
  */
  bool isEmpty() const;
  
  /** \details
    Returns the logical length of this vector.
    
    \remarks
    The logical length is the number of elements in the array returned
    by asArrayPtr() and getPtr().
  */
    int logicalLength() const;

  /** \details
    Sets the logical length of this vector, 
    and returns a reference to this vector.
    
    \param size [in]  Logical length of vector.
    
    \remarks
    The logical length is the number of elements in the array returned
    by asArrayPtr() and getPtr().
  */
    OdGeKnotVector& setLogicalLength(
    int size);
    
  int physicalLength() const;
  OdGeKnotVector& setPhysicalLength(int);

  /** \details
    Returns the length by which the array is set to grow automatically.
  */
  int growLength() const;

  /** \details
    Set the length by which the array will grow automatically.

    \param rowLength [in]  The length for the array automatic resizing.
  */
  OdGeKnotVector& setGrowLength(int rowLength);

  /** \details
    Returns this vector as an array of doubles.
    
    \remarks
    The number of elements in this array is returned by
    logicalLength(), and set by setLogicalLength().
  */
  const double* getPtr() const;

  /** \details
    Returns this vector as an array of doubles.
    
    \remarks
    The number of elements in this array is returned by
    logicalLength(), and set by setLogicalLength().
  */
  const double* asArrayPtr() const;
  
  /** \details
    Returns this vector as an array of doubles.
    
    \remarks
    The number of elements in this array is returned by
    logicalLength(), and set by setLogicalLength().
  */
  double* asArrayPtr();

  /** \details
    Sets the parameters for this vector according to the arguments, 
    and returns a reference to this vector.

    \param tol [in]  Knot equality tolerance.
    \param size [in]  Length of vector.
  */
  OdGeKnotVector& set(
    int size, 
    const double source[], 
    double tol = 1.e-9);

  int multiplicityAt(
    double param) const;

  OdGeDoubleArray& getArray();
  const OdGeDoubleArray& getArray() const;

protected:
  /** \details
    Returns true if and only if index < length()
    
    \param knotIndex [in]  Knot index.
  */
   bool isValid(
    OdUInt32 knotIndex) const;

   OdGeDoubleArray m_Data;
   double m_Tolerance;
};

#include "TD_PackPop.h"

#endif // OD_GE_KNOT_VECTOR

