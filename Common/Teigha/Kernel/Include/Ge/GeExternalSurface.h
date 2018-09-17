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

#ifndef OD_GEEXTSF_H
#define OD_GEEXTSF_H /*!DOM*/

#include "Ge/GeSurface.h"

#include "TD_PackPush.h"

class OdGePlane;
class OdGeCylinder;
class OdGeCone;
class OdGeSphere;
class OdGeTorus;
class OdGeNurbSurface;
class OdGeEllipCone;            // AE 03.09.2003 
class OdGeEllipCylinder;    // AE 09.09.2003    

/** \details
    This class represents unbounded surfaces, whose definitions are external to the OdGe library, as OdGeSurface.
    
    \remarks
    OdGeExternalSurface instances can be treated as any other OdGeSurface. 
    
    You can access the OdGeExternalSurface as a corresponding native OdGeSurface, if such a corresponding
    surface exists, or you may (more efficiently) access the external data in its native form.
    
    One example of using OdGeExternalSurface is to access an ACIS surface. 
    
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeExternalSurface : public OdGeSurface
{
public:


  /** \param pSurfaceDef [in]  Pointer to the surface definition.
    \param surfaceKind [in]  Information about system that created the surface.
    \param makeCopy [in]  Makes a copy of the surface definition.
    \param source [in]  Object to be cloned.
    
    \remarks
    Without arguments, the constructor creates an empty instance.

    Possible values for surfaceKind:
    
    @untitled table
    kAcisEntity
    kExternalEntityUndefined
  */
  OdGeExternalSurface();
  OdGeExternalSurface(
    void* pSurfaceDef, 
    OdGe::ExternalEntityKind surfaceKind,
    bool makeCopy = true);
  OdGeExternalSurface(
    const OdGeExternalSurface& source);

  /** \details
    Returns a copy of the raw surface definition.

    \param pSurfaceDef [out]  Receives a pointer to a copy of the raw surface definition.

    \remarks
    It is up to the caller to delete the memory allocated.
  */
   void getExternalSurface(
    void*& pSurfaceDef) const;

  // Type of the external surface.
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
    Return true if and only if the surface can be represented as an OdGePlanarEnt. 
  */
   bool isPlane() const;

  /** \details
    Return true if and only if the external surface can be represented as an OdGeSphere. 
  */
   bool isSphere() const;

  /** \details
    Return true if and only if the external surface can be represented as an OdGeCylinder. 
  */
   bool isCylinder() const;

  /** \details
    Return true if and only if the external surface can be represented as an OdGeCone. 
  */
   bool isCone() const;

  /** \details
    Return true if and only if the external surface can be represented as an OdGeEllipCylinder. 
  */
   bool isEllipCylinder() const;

  /** \details
    Return true if and only if the external surface can be represented as an OdGeEllipCone. 
  */
   bool isEllipCone() const;

  /** \details
    Return true if and only if the external surface can be represented as an OdGeTorus. 
  */
   bool isTorus() const;
  
  /** \details
    Return true if and only if the external surface can be represented as an OdGeNurbSurface. 
  */
   bool isNurbSurface() const;

  /** \details
    Returns true if and only if the surface is defined (not an empty instance).
  */
   bool isDefined() const;

  /** \details
    Return true if and only if the external surface can be represented as a native OdGeSurface,
    and returns a pointer to an instance of that native surface.

    \param nativeSurface [out]  Receives the native surface.

    \remarks
    The returned object is created with the new operator, and it is the responsibility of the caller to delete it.
  */
   bool isNativeSurface(
    OdGeSurface*& nativeSurface) const;

  // Assignment operator.
  //

  OdGeExternalSurface& operator =(
    const OdGeExternalSurface& extSurf);

  // Reset surface
  //

  /** \details
    Sets the parameters for this external surface according to the arguments, 
    and returns a reference to this external surface.

    \param pSurfaceDef [in]  Pointer to the surface definition.
    \param surfaceKind [in]  Information about system that created the surface.
    \param makeCopy [in]  If true, makes a copy of the surface definition.
  */
   OdGeExternalSurface& set(
    void* pSurfaceDef,
    OdGe::ExternalEntityKind surfaceKind,
    bool makeCopy = true);

  /** \details
    Returns true if and only if the external surface owns the data.

    \remarks
    If the external surface owns the data, it will be destroyed when
    the surface is destroyed.
  */
   bool isOwnerOfSurface() const;
  
  /** \details
    Forces this external surface to own the data, and returns a reference to this external surface.

    \remarks
    If the external surface owns the data, it will be destroyed when
    the external surface is destroyed.
  */
   OdGeExternalSurface& setToOwnSurface();

  //////////////////////////////////////////////////////////////////////////
};

#include "TD_PackPop.h"

#endif // OD_GEEXTSF_H

