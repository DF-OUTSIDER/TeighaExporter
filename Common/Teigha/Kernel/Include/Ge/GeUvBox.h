#ifndef _GE_UV_BOX_H_
#define _GE_UV_BOX_H_

#include "Ge/GeInterval.h"
#include "Ge/GeExport.h"

class OdGePoint2d;

/** \details
  Represents a box in UV-space of a surface. 

  Corresponding C++ library: TD_Ge
  
  <group OdGe_Classes>
*/
struct GE_TOOLKIT_EXPORT OdGeUvBox
{
  OdGeInterval intervals[2];

  OdGeUvBox();
  OdGeUvBox(const OdGeInterval& iIntervalU, const OdGeInterval& iIntervalV);

  const OdGeInterval& u() const { return intervals[0]; }
  const OdGeInterval& v() const { return intervals[1]; }
  OdGeInterval& u() { return intervals[0]; }
  OdGeInterval& v() { return intervals[1]; }

  const OdGeInterval& operator[](int idx) const { return intervals[idx]; }
  OdGeInterval& operator[](int idx) { return intervals[idx]; }

  OdGeUvBox& set();
  OdGeUvBox& set(const OdGeInterval& iIntervalU, const OdGeInterval& iIntervalV);
  OdGeUvBox& set(const OdGePoint2d& iLowerBound, const OdGePoint2d& iUpperBound);

  bool contains(double uparam, double vparam) const;
  bool contains(const OdGePoint2d& uvpoint) const;

  bool isBounded() const;

  /** \details
    Find finite sub-envelope belonging both to the specified range and this one
    \returns
    Returns true if and only if the specified uv-box intersects with this one, 
    and the resulting uv-box is bounded.

    \param range [in]  The uv-box to be intersected.
    \param result [out]  Receives the intersected uv-box.
  */
  bool finiteIntersectWith(const OdGeUvBox& range, OdGeUvBox& result) const;

  OdGePoint2d lowerBound() const;
  OdGePoint2d upperBound() const;

};

#endif //_GE_UV_BOX_H_
