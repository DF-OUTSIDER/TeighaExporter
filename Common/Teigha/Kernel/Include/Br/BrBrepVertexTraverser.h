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



#ifndef _INC_DDBRBREPVERTEXTRAVERSER_3F83F4F5003E_INCLUDED
#define _INC_DDBRBREPVERTEXTRAVERSER_3F83F4F5003E_INCLUDED

#include "Br/BrVertex.h"
#include "Br/BrBrep.h"
#include "Br/BrTraverser.h"


#include "TD_PackPush.h"

/** \details
    This class is the interface class for BREP vertex traversers.
    
    \sa
    TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrBrepVertexTraverser : public OdBrTraverser
{
public:
  /** \details
    Sets this Traverser object to a specific BREP vertex list.
    \param brep [in]  Owner of the vertex list.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setBrep(const OdBrBrep& brep);

  /** \details
    Sets this Traverser object to a specific BREP vertex list
    and starting position.
    \param vertex [in]  Defines the vertex list and starting position.
  */
  OdBrErrorStatus setBrepAndVertex(const OdBrVertex& vertex);

  /** \details
    Sets the starting position of this Traverser object.
    \param vertex [in]  Defines the starting position.
  */
  OdBrErrorStatus setVertex(const OdBrVertex& vertex);

  /** \details
    Returns the vertex object at the current Traverser position.
  */
  OdBrVertex getVertex() const;
  /** \details
    Returns the owner of the vertex list associated with this Traverser object.
  */
  OdBrBrep getBrep() const;

  OdBrBrepVertexTraverser();
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRBREPVERTEXTRAVERSER_3F83F4F5003E_INCLUDED */


