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



#ifndef _INC_DDBRSHELLFACETRAVERSER_3F83F80B005D_INCLUDED
#define _INC_DDBRSHELLFACETRAVERSER_3F83F80B005D_INCLUDED

#include "Br/BrShell.h"

#include "Br/BrFace.h"

#include "Br/BrTraverser.h"

#include "TD_PackPush.h"


/** \details
    This class is the interface class for shell face traversers.
    
    \sa
    TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrShellFaceTraverser : public OdBrTraverser
{
public:

  /** \details
    Returns the face object at the current Traverser position.
  */
  OdBrFace getFace() const;

  /** \details
    Returns the owner of the face list associated with this Traverser object.
  */
  OdBrShell getShell() const;

  /** \details
    Sets this Traverser object to a specific shell face list.
    \param shell [in]  Owner of the face list.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setShell(const OdBrShell& shell);

  /** \details
    Sets the starting position of this Traverser object.
    \param face [in]  Defines the starting position.
  */
  OdBrErrorStatus setFace(const OdBrFace& face);

  /** \details
    Sets this Traverser object to a specific shell face list
    and starting position.
    \param face [in]  Defines the face list and starting position.
  */
  OdBrErrorStatus setShellAndFace(const OdBrFace& face);

  OdBrShellFaceTraverser();
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRSHELLFACETRAVERSER_3F83F80B005D_INCLUDED */

