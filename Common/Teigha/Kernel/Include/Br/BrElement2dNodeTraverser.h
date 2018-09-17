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
#ifndef BRELEMENT2DNODETRAVERSER_H_INCLUDED
#define BRELEMENT2DNODETRAVERSER_H_INCLUDED

#include "Br/BrTraverser.h"
#include "Br/BrEnums.h"
#include "Br/BrMesh2dElement2dTraverser.h"
#include "Br/BrNode.h"
#include "Ge/GePoint2d.h"
/** \details
  This interface class represents element node traversers. 
  It is used to access node data, such as surface normals
  and pairs of uv parameters.
  
  \remarks 
  This class is implemented only for Spatial modeler.

  \sa
  TD_Br

  <group OdBr_Classes>
  */
class ODBR_TOOLKIT_EXPORT OdBrElement2dNodeTraverser : public OdBrTraverser
{
public:
  /** \details
    Default constructor.
  */
  OdBrElement2dNodeTraverser();
  
  /** \details
    Destructor.
  */
  ~OdBrElement2dNodeTraverser();

  /** \details
    Sets the specific element node list to this traverser.
    
    \param mesh2dElement2dTraverser [in] Defines the topological adjacent list of nodes 
    owned by the 2D element at the current position in this OdBrMesh2dElement2dTraverser,
    starting at the first node in the list.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
    
    \remarks
    In case of an error, the value of this traverser's element node list is unchanged.
  */
  OdBrErrorStatus   setElement    (const OdBrMesh2dElement2dTraverser& mesh2dElement2dTraverser);
  
  /** \details
    Sets the specific element node list to this traverser.
    
    \param element [in] Defines the 2D element with a topological adjacent list of nodes,
    starting at the first node in the list.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
      
    \remarks
    In case of an error, the value of this traverser's element node list is unchanged.
  */
  OdBrErrorStatus   setElement    (const OdBrElement2d& element);
  
  /** \details
    Returns the element that owns the element node list. 
    
    \param element [out] Defines the 2D element object, passed by reference.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
    
    \remarks
    In case of an error, the element reference object passed as an argument is unchanged.
  */
  OdBrErrorStatus   getElement    (OdBrElement2d& element) const;
  
  /** \details
    Sets the starting position of this traverser to a specific node in the element node list. 
    
    \param node [in] Node at which the traverser's starting position is set.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
    
    \remarks
    In case of an error, the traverser's element node list is unchanged.
  */
  OdBrErrorStatus   setNode         (const OdBrNode& node);
  
  /** \details
    Returns the node at the current position in the element node list. 
    
    \param node [out] Node at the current position.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
    
    \remarks
    In case of an error, the node reference object that is passed as an argument is unchanged.
  */
  OdBrErrorStatus   getNode         (OdBrNode& node) const;

  /** \details
    Returns the normal vector computed on the original surface at the current node.
    
    \param vector [out] Normal vector at the current node.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
    
    \remarks
    In case of an error, the normal vector reference argument's value is unchanged.
  */
  OdBrErrorStatus   getSurfaceNormal(OdGeVector3d& vector) const;
  
  /** \details
    Returns the 2D element node's underlying parameter point.
    
    \param point [out] Parameter point.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.
    
    \remarks
    In case of an error, the parameter point reference object is unchanged.
  */
  OdBrErrorStatus   getParamPoint  (OdGePoint2d& point) const;
};


#endif

