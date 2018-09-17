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



#ifndef _INC_DDBREDGE_3F82D9C2030D_INCLUDED
#define _INC_DDBREDGE_3F82D9C2030D_INCLUDED

#include "Br/BrEntity.h"
#include "Br/BrVertex.h"

#include "Br/BrEnums.h"

class OdGeNurbCurve3d;
class OdGeCurve3d;
class OdCmEntityColor;

#include "TD_PackPush.h"

/** \details
    This class is the interface class for BREP edges.

    \sa
    TD_Br
    
    \remarks
    Edges are primary BREP elements with associated geometries: curves. 
    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrEdge : public OdBrEntity
{
public:
  /** \details
    Returns the end vertex of this Edge entity.
    \param vertex2 [out]  Receives the end vertex.
  */
  bool getVertex2(OdBrVertex &vertex2) const;

  /** \details
    Returns the start vertex of this Edge entity.
    \param vertex1 [out]  Receives the start vertex.
  */
  bool getVertex1(OdBrVertex &vertex1) const;

  /** \details
    Returns the best match of the underlying curve of this Edge entity to curves supported by OdGe.
  */
  OdGe::EntityId getCurveType() const;

  /** \details
    Returns the underlying curve of this Edge entity as an OdGeCurve3d object.
  */
  OdGeCurve3d* getCurve() const;

  /** \details
  Returns the underlying curve of this Edge entity as an OdGeCurve3d object.
  */
  OdBrErrorStatus getCurve(OdGeCurve3d* &pCurve) const;
  
  /** \details
    Returns the underlying curve of this Edge entity as an OdGeNurbCurve3d object.
    
    \param nurb [out]  Receives the NURBS curve.
    
    \returns
    Returns odbrOK if successful, or an appropriate error code if not.
  */
  bool getCurveAsNurb(OdGeNurbCurve3d& nurb) const;

  /** \details
    Returns true if and only if the orientation of the edge from vertex 1 to vertex 2 is the 
    same as the orientation of the curve parameterization.
    
    \remarks
    The orientation of the underlying curve may be modified with OdGeCurve3d::reverseParam().  
  */
  bool getOrientToCurve() const;
  /** \details
    Returns the color of this Edge entity.
   
    \returns
    Returns true if and only if this Edge entity has a color associated with it.
    \param color [out]  Receives the color.
  */
  bool getColor(OdCmEntityColor &color) const;

  OdBrEdge();

  ~OdBrEdge();
};

#include "TD_PackPop.h"

#endif /* _INC_DDBREDGE_3F82D9C2030D_INCLUDED */

