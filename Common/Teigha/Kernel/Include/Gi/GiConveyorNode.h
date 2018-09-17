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




#ifndef __ODGICONVEYORNODE_H__
#define __ODGICONVEYORNODE_H__


#include "Gi/GiConveyorGeometry.h"
#include "Gi/GiExport.h"

#include "TD_PackPush.h"

class OdGiConveyorOutput;

/** \details
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiConveyorInput
{
public:
  /** \details
    Adds the specified source node to this Conveyor object.
    \param sourceNode [in] Source node to add.
  */
  virtual void addSourceNode(OdGiConveyorOutput& sourceNode) = 0;
  /** \details
    Removes the specified source node from this Conveyor object.
    
    \param sourceNode [in] Source node to remove.
  */
  virtual void removeSourceNode(OdGiConveyorOutput& sourceNode) = 0;
};

/** \details
    Corresponding C++ library: TD_Gi

    <group OdGi_Classes> 
*/
class OdGiConveyorOutput
{
public:
  /** \details
    Sets the destination geometry for this Conveyor object.
    
    \param destGeometry [in] Destination geometry.
  */
  virtual void setDestGeometry(OdGiConveyorGeometry& destGeometry) = 0;
  /** \details
    Returns the destination geometry for this Conveyor object.
  */
  virtual OdGiConveyorGeometry& destGeometry() const = 0;
};

/** \details

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiConveyorNode : public OdRxObject
{
protected:
  OdGiConveyorNode();
public:
  ODRX_DECLARE_MEMBERS(OdGiConveyorNode);
  
  /** \details
    Returns the input from this ConveyorNode object.
  */
  virtual OdGiConveyorInput& input() = 0;
  /** \details
    Returns the output from this ConveyorNode object.
  */
  virtual OdGiConveyorOutput& output() = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiConveyorNode object pointers.
*/
typedef OdSmartPtr<OdGiConveyorNode> OdGiConveyorNodePtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGICONVEYORNODE_H__
