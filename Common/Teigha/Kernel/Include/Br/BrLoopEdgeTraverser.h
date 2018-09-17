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



#ifndef _INC_DDBRLOOPEDGETRAVERSER_3F82E1FB0177_INCLUDED
#define _INC_DDBRLOOPEDGETRAVERSER_3F82E1FB0177_INCLUDED

#include "Br/BrTraverser.h"

class OdBrEdgeLoopTraverser;
class OdGeNurbCurve3d;
class OdGeNurbCurve2d;
class OdGeCurve3d;
class OdGeCurve2d;
class OdBrLoop;
class OdBrEdge;


#include "TD_PackPush.h"

/** \details
    This class is the interface class for loop edge traversers.

    \sa
    TD_Br

    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrLoopEdgeTraverser : public OdBrTraverser
{
public:

  /** \details
    Returns the underlying curve of the edge at the current Traverser position as an oriented OdGeCurve3d object.
  */
  OdGeCurve3d *getOrientedCurve() const;
  
  /** \details
    Returns the underlying curve of the edge at the current Traverser position as an OdGeNurbCurve3d object.

    \param nurb [out]  Receives the NURBS curve.
    
    \returns
    Returns true if and only if successful.
  */
  bool getOrientedCurveAsNurb(OdGeNurbCurve3d& nurb) const;

  /** \details
    Returns the underlying curve of the edge at the current Traverser position as a parametric OdGeCurve2d object.
  */
  OdGeCurve2d* getParamCurve() const;

  /** \details
    Returns the underlying curve of the edge at the current Traverser position as a parametric OdGeNurbCurve2d object.

    \param nurb [out]  Receives the NURBS curve.
    
    \returns
    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus getParamCurveAsNurb( OdGeNurbCurve2d& nurb ) const;

  /** \details
    Returns true if and only if the orientation of the edge from vertex 1 to vertex 2 is the 
    same as the orientation of the loop parameterization.
  */
  bool getEdgeOrientToLoop() const;

  /** \details
    Returns the edge object at the current Traverser position.
  */
  OdBrEdge getEdge() const;
  
 /** \details
    Returns the loop of the edge list associated with this Traverser object.
 */
  OdBrLoop getLoop() const;


  /** \details
    Sets the starting position of this Traverser object.
    \param edge [in]  Defines the starting position.
  */
  OdBrErrorStatus setEdge( const OdBrEdge& edge );
  
  /** \details
    Sets this Traverser object to a specific loop edge list.
    \param loop [in]  Owner of the edge list.
    
    \remarks
    This Traverser object is set to the first element in the list.

    Returns odbrOK if successful, or an appropriate error code if not.
  */
  OdBrErrorStatus setLoop( const OdBrLoop& loop );
  
  /** \details
    Sets this Traverser object to a specific edge loop list
    and starting position.
    \param edgeLoop [in]  Defines the edge list and starting position.
  */
   OdBrErrorStatus setLoopAndEdge( const OdBrEdgeLoopTraverser &edgeLoop );

  OdBrLoopEdgeTraverser();
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRLOOPEDGETRAVERSER_3F82E1FB0177_INCLUDED */

