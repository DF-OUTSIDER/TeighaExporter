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
#ifndef BRMESH2DELEMENT2DTRAVERSER_H_INCLUDED
#define BRMESH2DELEMENT2DTRAVERSER_H_INCLUDED

#include "Br/BrTraverser.h"
#include "Br/BrEnums.h"
#include "Br/BrElement2d.h"
#include "Br/BrMesh2d.h"

/** \details
  This interface class represents mesh element traversers.

  \remarks 
  This class is implemented only for Spatial modeler.

  \sa
  TD_Br

  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrMesh2dElement2dTraverser : public OdBrTraverser
{
public:
  /** \details
    Default constructor.
  */
  OdBrMesh2dElement2dTraverser();

  /** \details
    Destructor.
  */
  ~OdBrMesh2dElement2dTraverser();

  /** \details
    Sets this traverser to a specific mesh element list and start position.
    
    \param element2d [in] Reference to a 2D element object.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   setMeshAndElement(const OdBrElement2d& element2d);
  
  /** \details
    Sets this traverser to a specific mesh element list, 
    starting with the first list's element.
    
    \param mesh2d [in] Reference to a mesh object.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   setMesh         (const OdBrMesh2d& mesh2d);
  
  /** \details
    Returns the owner of the mesh element list.

    \param mesh2d [in/out] Reference to a mesh object.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   getMesh         (OdBrMesh2d& mesh2d) const;

  /** \details
    Sets the starting position of this traverser to a specific element in the mesh element list.

    \param element2d [in] Reference to an element object.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   setElement    (const OdBrElement2d& element2d);

  /** \details
    Returns the current element in the mesh element list.

    \param element2d [in/out] Reference to an element object.

    \returns Returns odbrOK if successful, or an appropriate error code if not.  
  */
  OdBrErrorStatus   getElement    (OdBrElement2d& element2d) const;
};


#endif

