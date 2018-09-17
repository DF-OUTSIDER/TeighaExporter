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
#ifndef BRMESHCONTROL_H_INCLUDED
#define BRMESHCONTROL_H_INCLUDED

#include "Br/BrEnums.h"

/** \details
  This class defines the interface base class for mesh controls.

  \remarks 
  This class is implemented only for Spatial modeler.

  \sa
  TD_Br
  
  <group OdBr_Classes>
  */
class ODBR_TOOLKIT_EXPORT OdBrMeshControl
{
  friend class OdBrEntityInternals;
public:

  /** \details
    Virtual destructor.
  */
  virtual ~OdBrMeshControl();

  /** \details
    Checks if content of the two mesh control objects is equal.

    \param other [in] Mesh control object.

    \returns Returns true if two mesh control objects are equal, 
    or returns false in the other case.
  */
  bool      isEqualTo       (const OdBrMeshControl* other) const;

  /** \details
    Sets the maximum number of subdivisions.

    \param maxSubs [in] Maximum number of subdivisions.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   setMaxSubdivisions(OdUInt32 maxSubs = 0);

  /** \details
    Gets the maximum number of subdivisions.

    \param maxSubs [out] Maximum number of subdivisions.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */  
  OdBrErrorStatus   getMaxSubdivisions(OdUInt32& maxSubs) const;

  /** \details
    Sets the maximum spacing between nodes in world coordinates.

    \param maxNodeSpace [in] Maximum node spacing.

    \returns Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus   setMaxNodeSpacing(double maxNodeSpace = 0);

  /** \details
    Returns the maximum spacing between nodes in world coordinates.

    \param maxNodeSpace [out] Maximum node spacing.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   getMaxNodeSpacing(double& maxNodeSpace) const;

  /** \details
    Sets the maximum angle tolerance between normals of any two nodes in 
    the mesh element.

    \param angTol [in] Maximum angle tolerance in radians.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   setAngTol    (double angTol = 0);

  /** \details
    Returns the maximum angle tolerance between normals of any two nodes in 
    the mesh element.

    \param angTol [out] Maximum angle tolerance in radians.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   getAngTol    (double& angTol) const;

  /** \details
    Sets the maximum distance tolerance in world coordinates between the original 
    geometry and the mesh element.

    \param distTol [in] Maximum distance tolerance.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   setDistTol    (double distTol = 0);

  /** \details
    Returns the maximum distance tolerance between the original geometry and the mesh element.

    \param distTol [out] Maximum distance tolerance.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   getDistTol    (double& distTol) const;

protected:
  /** \details
    Default constructor.
  */
  OdBrMeshControl();

  void* m_pImp;
};


#endif
