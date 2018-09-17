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
#ifndef BRELEMENT_H_INCLUDED
#define BRELEMENT_H_INCLUDED

#include "Br/BrMeshEntity.h"
/** \details
  This class defines the interface base-class for mesh elements. Each element is
  represented by an ordered, minimal closed subset of connected nodes in a mesh.
 
  \remarks 
  This class is implemented only for Spatial modeler. 
  
  \sa
  TD_Br

  <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrElement : public OdBrMeshEntity
{
public:
  /** \details
    Virtual destructor.
  */
  virtual ~OdBrElement();

protected:
  /** \details
    Default constructor.
  */
  OdBrElement();
  
  /** \details
    Copy constructor.
  */
  OdBrElement(const OdBrElement& src);
  
  /** \details
    Assignment operator.
  */
  OdBrElement&    operator =      (const OdBrElement& src);
};


#endif

