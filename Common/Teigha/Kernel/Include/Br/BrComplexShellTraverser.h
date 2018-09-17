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



#ifndef _INC_DDBRCOMPLEXSHELLTRAVERSER_3F83F6550148_INCLUDED
#define _INC_DDBRCOMPLEXSHELLTRAVERSER_3F83F6550148_INCLUDED

#include "Br/BrComplex.h"
#include "Br/BrShell.h"
#include "Br/BrTraverser.h"

#include "TD_PackPush.h"


/** \details
    This class is the interface class for complex shell traversers.

    \sa
    TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrComplexShellTraverser : public OdBrTraverser
{
public:
  OdBrComplexShellTraverser();

  /** \details
    Sets this Traverser object to a specific complex shell list.
    \param complex [in]  Owner of the shell list.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setComplex(const OdBrComplex& complex);

  /** \details
    Sets the starting position of this Traverser object.
    \param shell [in]  Defines the starting position.
  */
  OdBrErrorStatus setShell(const OdBrShell& shell);

  /** \details
    Sets this Traverser object to a specific complex shell list
    and starting position.
    \param shell [in]  Defines the shell list and starting position.
  */
  OdBrErrorStatus setComplexAndShell(const OdBrShell& shell);

  /** \details
    Returns the shell object at the current Traverser position.
  */
  OdBrShell getShell() const;

  /** \details
    Returns the owner of the shell list associated with this Traverser object.
  */
  OdBrComplex getComplex() const;

};

#include "TD_PackPop.h"

#endif /* _INC_DDBRCOMPLEXSHELLTRAVERSER_3F83F6550148_INCLUDED */

