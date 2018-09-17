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
#ifndef BRELEMENT2D_H_INCLUDED
#define BRELEMENT2D_H_INCLUDED

#include "Br/BrElement.h"
#include "Ge/GeVector3d.h"
/** \details
  This interface class represents linear, two-dimensional first-order elements 
  in a mesh.

  \remarks 
  This class is implemented only for Spatial modeler. 
    
  \sa
  TD_Br
    
  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrElement2d : public OdBrElement
{
public:  
  /** \details
    Default constructor.
  */
  OdBrElement2d();
  
  /** \details
    Destructor.
  */
  ~OdBrElement2d();

  /** \details
    Returns the normal vector of normalized model space. The vector is computed 
    on the plane that is defined by the nodes of element traversed in a clockwise 
    direction.
    
    \param normal [out] Normal vector.

    \remarks
    In case of an error, the value of the normal vector passed as an 
    argument is unchanged.
  */
  OdBrErrorStatus getNormal (OdGeVector3d& normal) const;
};


#endif

