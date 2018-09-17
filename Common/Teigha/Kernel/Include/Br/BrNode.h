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
#ifndef BRNODE_H_INCLUDED
#define BRNODE_H_INCLUDED

#include "Br/BrMeshEntity.h"
#include "Br/BrEnums.h"
/** \details
  This interface class represents nodes in a mesh.

  \remarks 
  This class is implemented only for Spatial modeler.

  \sa
  TD_Br

  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrNode : public OdBrMeshEntity
{
public:
  /** \details
    Default constructor.
  */
  OdBrNode();

  /** \details
    Destructor.
  */
  ~OdBrNode();

  /** \details
    Returns the base point of this node.
    
    \param point [out] Base point.
    
    \returns Returns odbrOK if successful, or an appropriate error code if not.   
  */
  OdBrErrorStatus getPoint(OdGePoint3d& point) const;
};

#endif

