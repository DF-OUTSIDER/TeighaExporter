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




#ifndef _ODDBOBJECTIDGRAPH_H_INCLUDED_
#define _ODDBOBJECTIDGRAPH_H_INCLUDED_

#include "TD_PackPush.h"

#include "OdaDefs.h"
#include "DbGraph.h"

/** <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObjectIdGraphNode : public OdDbGraphNode
{
  OdDbObjectId m_Id;
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectIdGraphNode);
  OdDbObjectIdGraphNode() : OdDbGraphNode(){}
  virtual ~OdDbObjectIdGraphNode();
  OdDbObjectId id() const { return m_Id; }
  void setId(OdDbObjectId id){ m_Id = id;}
};

/** <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObjectIdGraph : public OdDbGraph
{
public:
  OdDbObjectIdGraph(){}
  virtual ~OdDbObjectIdGraph();
  OdDbObjectIdGraphNode* findNode(OdDbObjectId) const;
  OdDbObjectIdGraphNode* idNode(int idx) const { return (OdDbObjectIdGraphNode*)node(idx); }
};
#include "TD_PackPop.h"

#endif // _ODDBOBJECTIDGRAPH_H_INCLUDED_
