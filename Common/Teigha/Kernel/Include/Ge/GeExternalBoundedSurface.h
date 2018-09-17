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

#ifndef OD_GEXBNDSF_H
#define OD_GEXBNDSF_H /*!DOM*/

class OdGeExternalSurface;
class OdGeCurveBoundary;
#include "Ge/GeSurface.h"

#include "TD_PackPush.h"

/** \details
    This class represents bounded surfaces, whose definitions are external to the OdGe library.
    
    \remarks
    Each instance of ns OdGeExternalBoundedSurface is comprosed of an instance
    of an OdExternalSurface and a collection of instances of OdGeCurveBoundary.
    
    OdGeExternalBoundedSurface instances can be treated as any other OdGeSurface. 
    
    You can access the OdGeExternalBoundedSurface as a corresponding native OdGeSurface, if such a corresponding
    surface exists, or you may (more efficiently) access the external data in its native form.
    
    One example of using OdGeExternalBoundedSurface is to access an ACIS surface. 
   
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeExternalBoundedSurface : public OdGeSurface
{
public:


  /** \param pSurfaceDef [in]  Pointer to the surface definition.
    \param surfaceKind [in]  Information about system that created the surface.
    \param makeCopy [in]  Makes a copy of curveDef.
    \param source [in]  Object to be cloned.
    
    \remarks
    Without arguments, the constructor creates an empty instance.

    Possible values for surfaceKind:
    
    @untitled table
    kAcisEntity
    kExternalEntityUndefined
  */
  OdGeExternalBoundedSurface();
  OdGeExternalBoundedSurface(
    void* pSurfaceDef, 
    OdGe::ExternalEntityKind surfaceKind, 
    bool makeCopy = true);
  OdGeExternalBoundedSurface(
    const OdGeExternalBoundedSurface& source);

  // Surface data.
  //

  /** \details
    Rerurns information about the system that created the surface.

    \remarks
    Possible values for externalSurfaceKind:

    @untitled table
    kAcisEntity
    kExternalEntityUndefined
  */
   OdGe::ExternalEntityKind externalSurfaceKind() const;
  
  /** \details
    Returns true if and only if the surface is defined (not an empty instance).
  */
   bool isDefined() const;

  /** \details
    Returns a copy of the raw surface definition.

    \param curveDef [out]  Receives a pointer to a copy of the raw surface definition.

    \remarks
    It is up to the caller to delete the memory allocated.
  */
   void getExternalSurface(
    void*& pSurfaceDef) const;

  // Odcess to unbounded surface.
  //

  /** \details
    Returns the unbounded surface.
    
    \param pSurfaceDef [out]  Receives a pointer to the unbounded surface definition.
    
    \remarks
    If called with a pointer to OdGeSurface, the OdExternalSurface
    will be converted to a native OdGeSurface, if possible.
  */
      void getBaseSurface(
    OdGeSurface*& pSurfaceDef) const;

  /** \details
    Returns the unbounded surface.
    
    \param unboundedSurfaceDef [out]  Receives the unbounded surface definition.
  */
    void getBaseSurface(
    OdGeExternalSurface& unboundedSurfaceDef) const;

  // Type queries on the unbounded base surface.

  /** \details
    Return true if and only if the unbounded base surface can be represented as an OdGePlanarEnt. 
  */
   bool isPlane() const;

  /** \details
    Return true if and only if the unbounded base surface can be represented as an OdGeSphere. 
  */
   bool isSphere() const;

  /** \details
    Return true if and only if the unbounded base surface can be represented as an OdGeCylinder. 
  */
   bool isCylinder() const;

  /** \details
    Return true if and only if the unbounded base surface can be represented as an OdGeCone. 
  */
   bool isCone() const;

   /** \details
   Return true if and only if the unbounded base surface can be represented as an OdGeEllipCylinder. 
   */
   bool isEllipCylinder() const;

   /** \details
   Return true if and only if the unbounded base surface can be represented as an OdGeEllipCone. 
   */
   bool isEllipCone() const;

  
  /** \details
    Return true if and only if the unbounded base surface can be represented as an OdGeTorus. 
  */
   bool isTorus() const;
  
  /** \details
    Return true if and only if the unbounded base surface can be represented as an OdGeNurbSurface. 
  */
   bool isNurbs() const;

  /** \details
    Return true if and only if the unbounded base surface cannot be represented as an native OdGeSurface. 
  */
   bool isExternalSurface() const;

  // Odcess to the boundary data.
  //
  
  /** \details
    Returns the number of contours on this surface.  
  */
   int numContours() const;

  /** \details
    Returns an array of the contours on this surface.

    \param numContours [out]  Receives the number of contours on this surface.
    \param contours [out]  Receives the array of contours. 
  */
   void getContours(
    int& numContours, 
    OdGeCurveBoundary*& contours) const;

  // Set methods
  //
  
  /** \details
    Sets the parameters for this external bound surface according to the arguments, 
    and returns a reference to this external surface.

    \param pSurfaceDef [in]  Pointer to the surface definition.
    \param surfaceKind [in]  Information about system that created the surface.
    \param makeCopy [in]  If true, makes a copy of the surface definition.
  */
   OdGeExternalBoundedSurface& set(
    void* pSurfaceDef,
    OdGe::ExternalEntityKind surfaceKind, 
    bool makeCopy = true);

  // Surface ownership.
  //
  
  /** \details
    Returns true if and only if the external surface owns the data.

    \remarks
    If the external surface owns the data, it will be destroyed when
    the curve is destroyed.
  */
   bool isOwnerOfSurface() const;

  /** \details
    Forces this external surface to own the data, and returns a reference to this external surface.

    \remarks
    If the external surface owns the data, it will be destroyed when
    the external surface is destroyed.
  */
   OdGeExternalBoundedSurface& setToOwnSurface();

  // Assignment operator.
  //
  OdGeExternalBoundedSurface& operator =(
    const OdGeExternalBoundedSurface& extBoundSurf);

};

#include "TD_PackPop.h"

#endif // OD_GEXBNDSF_H
