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
#ifndef BRMESH2DCONTROL_H_INCLUDED
#define BRMESH2DCONTROL_H_INCLUDED

#include "Br/BrMeshControl.h"
#include "Br/BrEnums.h"

/** \details
  This interface class represents 2D mesh controls. It defines specific functions
  used to set controls for generating 2D meshes.

  \remarks 
  This class is implemented only for Spatial modeler.

  \sa
  TD_Br

  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrMesh2dControl : public OdBrMeshControl
{
public:
  /** \details
    Default constructor.
  */
  OdBrMesh2dControl();

  /** \details
    Copy constructor.
  */
  OdBrMesh2dControl(const OdBrMesh2dControl& src);

  /** \details
    Destructor.
  */
  ~OdBrMesh2dControl();

  /** \details
    Assignment operator.
  */
  OdBrMesh2dControl&  operator =      (const OdBrMesh2dControl& src);

  /** \details
    Sets the maximum aspect ratio between width and height for a 2D mesh element.

    \param maxAspectRatio [in] The value of the ratio.

    \returns Returns odbrOK if successful, or an appropriate error code if not.

    \remarks
    If the argument value is between zero and one, the aspect ratio relationship is inverted to a height-to-width ratio.  
  */
  OdBrErrorStatus   setMaxAspectRatio(double maxAspectRatio = 0);
  
  /** \details
    Returns the maximum aspect ratio between width and height for a 2D mesh element.
    
    \param maxAspectRatio [out] Maximum aspect ratio.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus   getMaxAspectRatio(double& maxAspectRatio) const;
  
  /** \details
    Sets the element shape criteria.

    \param elementShape [in] Shape of the mesh.

    \returns Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus   setElementShape  (Element2dShape elementShape = kDefault);
  
  /** \details
    Returns the element shape criteria.

    \param elementShape [out] Shape of the mesh.

    \returns Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus   getElementShape  (Element2dShape& elementShape) const;

  static const ODBR_TOOLKIT_EXPORT_STATIC OdBrMesh2dControl OdBrMesh2dControlDefault;
};


#endif
