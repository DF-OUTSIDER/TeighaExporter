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



#ifndef _INC_DDBREDGELOOPTRAVERSER_3F82E2750148_INCLUDED
#define _INC_DDBREDGELOOPTRAVERSER_3F82E2750148_INCLUDED

#include "Br/BrTraverser.h"

class OdBrEdge;
class OdBrLoop;
class OdBrLoopEdgeTraverser;

#include "TD_PackPush.h"


/** \details
    This class is the interface class for edge loop traversers.

    \sa
    TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrEdgeLoopTraverser : public OdBrTraverser
{
public:
  /** \details
    Sets this Traverser object to a specific edge loop list.
    \param edge [in]  Owner of the loop list.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setEdge(const OdBrEdge& edge);

  /** \details
    Sets the starting position of this Traverser object.
    \param loop [in]  Defines the starting position.
  */
  OdBrErrorStatus setLoop(const OdBrLoop& loop);

  /** \details
    Sets this Traverser object to a specific edge loop list
    and starting position.
    \param loopEdge [in]  Defines the edge loop list and starting position.
  */
  OdBrErrorStatus setEdgeAndLoop(const OdBrLoopEdgeTraverser& loopEdge);

  /** \details
    Returns the loop object at the current Traverser position.
  */
  OdBrLoop getLoop() const;

  /** \details
    Returns the owner of the loop list associated with this Traverser object.
  */
  OdBrEdge getEdge() const;

  OdBrEdgeLoopTraverser();
};

#include "TD_PackPop.h"

#endif /* _INC_DDBREDGELOOPTRAVERSER_3F82E2750148_INCLUDED */

