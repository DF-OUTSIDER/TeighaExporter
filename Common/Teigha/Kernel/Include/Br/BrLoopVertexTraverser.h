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



#ifndef _INC_DDBRLOOPVERTEXTRAVERSER_3F82E3080167_INCLUDED
#define _INC_DDBRLOOPVERTEXTRAVERSER_3F82E3080167_INCLUDED

#include "Br/BrTraverser.h"

class OdBrVertexLoopTraverser;
class OdBrVertex;
class OdBrLoop;
class OdGePoint2d;

#include "TD_PackPush.h"


/** \details
    This class is the interface class for loop vertex traversers.

    \sa
    TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrLoopVertexTraverser : public OdBrTraverser
{
public:
  OdBrLoopVertexTraverser();

  /** \details
    Returns the loop vertex.
  */
  OdBrVertex getVertex() const;

 /** \details
    Returns the owner of the vertex list associated with this Traverser object.
 */
  OdBrLoop getLoop() const;

  /** \details
    Sets this Traverser object to a specific loop vertex list.
    \param loop [in]  Owner of the vertex list.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setLoop(const OdBrLoop& loop);
  /** \details
    Sets this Traverser object to a specific loop vertex list
    and starting position.
    \param vertexLoop [in]  Defines the loop vertex list and starting position.
  */
  OdBrErrorStatus setLoopAndVertex(const OdBrVertexLoopTraverser& vertexLoop);
  
  /** \details
    Sets the starting position of this Traverser object.
    \param vertex [in]  Defines the starting position.
  */
  OdBrErrorStatus setVertex(const OdBrVertex& vertex);

  /** \details
    Returns the parameter space point at the current Traverser position.
    \param ppoint [out]  Parameter point.
    
    \returns
    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus getParamPoint(OdGePoint2d &point) const;
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRLOOPVERTEXTRAVERSER_3F82E3080167_INCLUDED */

