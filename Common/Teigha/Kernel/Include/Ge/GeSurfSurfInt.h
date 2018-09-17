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

#ifndef OD_GESSINT_H
#define OD_GESSINT_H /*!DOM*/

#include "Ge/GeEntity3d.h"

class OdGeCurve3d;
class OdGeCurve2d;
class OdGeSurface;

#include "TD_PackPush.h"

/** \details
    This class holds the intersection data of two surfaces.

    \remarks
    * This object contains references to surface objects but does not own them.
    * Actual intersection calculations are not performed until a query function is called.
    * Any returned objects other than pointers to the referenced surfaces are owned by the called.
      
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSurfSurfInt  : public OdGeEntity3d
{
public:


  /** \param srf1 [in]  First surface.
    \param srf2 [in]  Second surface.
    \param tol [in]  Geometric tolerance.
    \param source [in]  Object to be cloned.
  */
  OdGeSurfSurfInt();

  OdGeSurfSurfInt(
    const OdGeSurface& srf1,
    const OdGeSurface& srf2,
    const OdGeTol& tol = OdGeContext::gTol) ;
  OdGeSurfSurfInt(
    const OdGeSurfSurfInt& source);

  /** \details
    Returns the first surface.
  */
   const OdGeSurface* surface1() const;

  /** \details
    Returns the second surface.
  */
   const OdGeSurface* surface2() const;

  /** \details
    Returns the tolerance for determining intersections.
  */
   OdGeTol tolerance() const;

  /** \details
    Returns the number of intersections between the two surfaces,
    and the status of the intersections.

    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   int numResults(
    OdGeIntersectError& status) const;

  /** \details
    Returns the 3D curve representing the specified intersection
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param isExternal [in]  Unknown.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   OdGeCurve3d* intCurve(
    int intNum, 
    bool isExternal, 
    OdGeIntersectError& status) const; 

  /** \details
    Returns the 2D curve representing the specified intersection
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param isExternal [in]  Unknown.
    \param isFirst [in]  If true, returns the curve on the first surface, otherwise it the curve on the second surface.
    \param status [out]  Receives the status of the intersection.

    \remarks
    If the intersection is not a 2d curve, returns 0.

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   OdGeCurve2d* intParamCurve(
    int intNum, 
    bool isExternal, 
    bool isFirst, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the 3d point representing the specified intersection,
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param status [out]  Receives the status of the intersection.

    \remarks
    If the intersection is not a 3d point, returns 0.

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   OdGePoint3d intPoint(
    int intNum, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the parameter pairs for the specified intersection point with respect to each surface,
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param param1 [out]  Receives the parameter pair of the intersection point with respect to the first curve.
    \param param2 [out]  Receives the parameter pair of the intersection point with respect to the second curve.
    \param status [out]  Receives the status of the intersection.

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   void getIntPointParams(
    int intNum,
    OdGePoint2d& param1, 
    OdGePoint2d& param2, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the configurations on either side of the intersection each surface.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param surf1Left [out]  Receives the configuration for surface 1 on the left side of the intersection.
    \param surf1Right [out]  Receives the configuration for surface 1 on the right side of the intersection.
    \param surf2Left [out]  Receives the configuration for surface 2 on the left side of the intersection.
    \param surf2Right [out]  Receives the configuration for surface 2 on the right side of the intersection.
    \param status [out]  Receives the status of the intersection.
    
    \remarks
    Possible values for surf1Left, surf1Right, surf2Left, and surf2Right are as follows:

    @untitled table
    kSSIUnknown
    kSSIOut            Neighborhood is outside this surface.
    kSSIIn             Neighborhood is inside this surface.
    kSSICoincident     Non-zero area intersection.

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown 

    \remarks
    Currently not implemented.
  */
   void getIntConfigs(
    int intNum, 
    OdGe::ssiConfig& surf1Left,  
    OdGe::ssiConfig& surf1Right,
    OdGe::ssiConfig& surf2Left,  
    OdGe::ssiConfig& surf2Right,  
    OdGe::ssiType& intType, 
    int& dim, 
    OdGeIntersectError& status) const; 

  /** \details
    Description Pending.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   int getDimension(
    int intNum, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the type of the specified intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible return values for the configuratons are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
   OdGe::ssiType getType(
    int intNum, 
    OdGeIntersectError& status) const;

  /** \details
    Sets the surfaces and tolerances whose intersection data is to be determines.
    Returns a reference to this SurfSurfInt.

    \param srf1 [in]  First surface.
    \param srf2 [in]  Second surface.
    \param tol [in]  Geometric tolerance.
  */
   OdGeSurfSurfInt& set(
    const OdGeSurface& srf1,
    const OdGeSurface& srf2,
    const OdGeTol& tol = OdGeContext::gTol);

  OdGeSurfSurfInt& operator =(
    const OdGeSurfSurfInt& surfSurfInt);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif // OD_GESSINT_H
