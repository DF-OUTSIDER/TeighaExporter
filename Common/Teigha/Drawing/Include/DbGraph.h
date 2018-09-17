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




#ifndef _ODDBGRAPH_H_INCLUDED_
#define _ODDBGRAPH_H_INCLUDED_

#include "TD_PackPush.h"

#include "OdaDefs.h"
#include "DbExport.h"
#include "RxObject.h"
#include "OdArray.h"

class OdDbGraph;
class OdDbGraphNode;

/** \details
    This template class is a specialization of the OdArray class for OdDbGraphNode object pointers.
*/
typedef OdArray<OdDbGraphNode*, OdMemoryAllocator<OdDbGraphNode*> > OdDbGraphNodeArray;

/** \details
    This class implements generic node objects for generic graphs.
    
    \remarks
    A graph consists of a collection of nodes bi-directionally linked by directional edges.
    
    An edge connected to a node is represented as a pointer or reference to the node at the other
    end of the edge.
 
    References are classified as either incoming or outgoing. Every incoming reference has
    a corresponding outgoing reference and vice versa.

    Each GraphNode object can have any number of references associated with it, enabling the
    implementation of any graph structure.

    \sa
    
    * OdDbGraph
    * OdDbGraphStack
    * OdDbXrefGraph
    * OdDbXrefGraphNode
    
    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGraphNode : public OdRxObject
{
  friend class OdDbGraph;
public:
  OdDbGraphNode() : m_pData(0), m_flags(0), m_pOwner(0) {}

  ODRX_DECLARE_MEMBERS(OdDbGraphNode);

  virtual ~OdDbGraphNode();
  
  enum Flags
  {
    kNone         = 0x00, // None.
    kVisited      = 0x01, // Used internally by OdDbGraph::findCycles() and while traversing a graphs with cycles.
    kOutsideRefed = 0x02, // Used internally by Xref detach.
    kSelected     = 0x04, // User selection. Set by getOutgoing.
    kInList       = 0x08, // Is *in* list. Set by getOutgoing.
    kListAll      = 0x0E, // Used to *clear* kSelected, kInList, and kOutsideRefed.
    kFirstLevel   = 0x10, // The *node* is connected to the root *node*. Read Only.
    kUnresTree    = 0x20, // The tree is unresolved.
    kAll          = 0x2F  // Used to *clear* all but kFirstLevel.
  };
  
  /** \details
    Returns the data associated with this GraphNode object.
  */
  void* data() const { return m_pData; }

  /** \details
    Sets the data associated with this GraphNode object.
    
    \param pData [in] Pointer to the data.
  */
  void setData(
    void* pData) { m_pData = pData; }
  
  /** \details
    Return the number of outgoing references associated with this GraphNode object.
  */
  int numOut() const { return m_outgoing.size(); }
  
  /** \details
    Return the number of incoming references associated with this GraphNode object.
  */
  int numIn() const { return m_incoming.size(); } 
   
  /** \details
    Returns the specified incoming reference of this GraphNode object.
    
    \param refIndex [in]  Reference index. 
    
    \remarks
    Returns a null pointer if and only if the index is not valid.
  */
  OdDbGraphNode* in(
    int refIndex) const { return m_incoming.at(refIndex); }

  /** \details
    Returns the specified outgoing reference of this GraphNode object.
    
    \param refIndex [in]  Reference index. 
    \remarks
    Returns a null pointer if and only if the index is not valid.
  */
  OdDbGraphNode* out(
    int refIndex) const { return m_outgoing.at(refIndex); }
  
  /** \details
    Creates an outgoing reference, in this GraphNode object, to the specified GraphNode object.  
    
    \param pTo [in]  Pointer to the outgoing GraphNode.

    \remarks
    Creates an incoming reference to this GraphNode object in *pTo.
    
    Throws:
    OdError(eInvalidOwnerObject) if the specified GraphNode object are not in the same Graph object as this
    GraphNode object.
    
    \sa
    OdDbGraph::addEdge
  */
  void addRefTo(
    OdDbGraphNode* pTo);
  
  /** \details
    Removes the outgoing reference, in this GraphNode object, to the specified GraphNode object.  
    
    \param pTo [in]  Pointer to the outgoing GraphNode.

    \remarks
    Removes the incoming reference to this GraphNode object in the outgoing GraphNode object.
    
    The specified GraphNode object must be part of the same graph as this GraphNode object.
  */
  void removeRefTo(
    OdDbGraphNode* pNode);

  /** \details
    Removes all references in and to this GraphNode object.  
    
    \remarks
    Always called by ~OdDbGraphNode().
  */
  void disconnectAll();
  
  /** \details
    Returns the Graph object to which this GraphNode object is attached.
  */
  OdDbGraph* owner() const { return m_pOwner; }
  
  /** \details
    Returns true if and only if the all the specified set flag bits are set in this GraphNode object.
    
    \param flags [in]  Flag bits.
    
    \remarks
    flags must be a combination of one or more of the following:
    
    <table>
    Name              Value    Description
    kNone             0x00     None.
    kVisited          0x01     Used internally by OdDbGraph::findCycles() and while traversing a graphs with cycles.
    kOutsideRefed     0x02     Used internally by Xref detach.
    kSelected         0x04     User selection. Set by getOutgoing.
    kInList           0x08     Is in list. Set by getOutgoing.
    kListAll          0x0E     Used to clear kSelected, kInList, kOutsideRefed.
    kFirstLevel       0x10     The node is connected to the root node. Read Only.
    kUnresTree        0x20     The tree is unresolved.
    kAll              0x2F     Used to clear all but kFirstLevel.
    </table>
  */
  bool isMarkedAs(
    OdUInt8 flags) const { return ((m_flags & flags)==flags); }
  
  /** \details
    Sets the specified set flag bits in this GraphNode object.
    
    \param flags [in]  Flag bits.
    
    \remarks
    flags must be a combination of one or more of the following:
    
    <table>
    Name              Value    Description
    kNone             0x00     None.
    kVisited          0x01     Used internally by OdDbGraph::findCycles() and while traversing a graphs with cycles.
    kOutsideRefed     0x02     Used internally by Xref detach.
    kSelected         0x04     User selection. Set by getOutgoing.
    kInList           0x08     Is in list. Set by getOutgoing.
    kListAll          0x0E     Used to clear kSelected, kInList, kOutsideRefed.
    kFirstLevel       0x10     The node is connected to the root node. Read Only.
    kUnresTree        0x20     The tree is unresolved.
    kAll              0x2F     Used to clear all but kFirstLevel.
    </table>
  */
  void markAs(
    OdUInt8 flags)
  {
    if(!GETBIT(flags, kFirstLevel))
    {
      m_flags |= flags;
    }
    else
    {
      throw OdError(eInvalidInput);
    }
  }

  /** \details
    Clears the specified set flag bits in this GraphNode object.
    
    \param flags [in]  Flag bits.
    
    \remarks
    flags must be a combination of one or more of the following:
    
    <table>
    Name              Value    Description
    kNone             0x00     None.
    kVisited          0x01     Used internally by OdDbGraph::findCycles() and while traversing a graphs with cycles.
    kOutsideRefed     0x02     Used internally by Xref detach.
    kSelected         0x04     User selection. Set by getOutgoing.
    kInList           0x08     Is in list. Set by getOutgoing.
    kListAll          0x0E     Used to clear kSelected, kInList, kOutsideRefed.
    kFirstLevel       0x10     The node is connected to the root node. Read Only.
    kUnresTree        0x20     The tree is unresolved.
    kAll              0x2F     Used to clear all but kFirstLevel.
    </table>
  */
  void clear(
    OdUInt8 flags)
  {
    if(!GETBIT(flags, kFirstLevel))
    {
      m_flags &= (~flags);
    }
    else
    {
      throw OdError(eInvalidInput);
    }
  }

    
  /** \details
    Marks this GraphNode object and all nested outgoing GraphNode objects with the specified flags.
    
    \param flags [in]  Flag bits.
    \param pNodeArray [in]  Pointer to a GraphNode array.  

    \remarks
    If pNodeArray is specified, this function appends this GraphNode object and all 
    nested outgoing GraphNode objects, to the specified array. While tranversing a branch,
    if any GraphNode object already has the flag bits set, the branch is no longer followed.

    The user must clear the flags with OdDbGraph::clearAll() or OdDbGraphNode::clear() when done with them.
    
    flags must be a combination of one or more of the following:
    
    <table>
    Name              Value    Description
    kNone             0x00     None.
    kVisited          0x01     Used internally by OdDbGraph::findCycles() and while traversing a graphs with cycles.
    kOutsideRefed     0x02     Used internally by Xref detach.
    kSelected         0x04     User selection. Set by getOutgoing.
    kInList           0x08     Is in list. Set by getOutgoing.
    kListAll          0x0E     Used to clear kSelected, kInList, kOutsideRefed.
    kFirstLevel       0x10     The node is connected to the root node. Read Only.
    kUnresTree        0x20     The tree is unresolved.
    kAll              0x2F     Used to clear all but kFirstLevel.
    </table>
    
    \remarks
    This function is not implemented, and will generate a link error if you reference it.
  */
  void markTree(
    OdUInt8 flags, 
    OdDbGraphNodeArray* pNodeArray = 0);
  
  // Circularity detection methods
  
  /** \details
    Returns the number of outgoing cyclical references associated with this GraphNode object.
    
    \remarks
    This function returns valid results only if OdDbGraph::findCycles() has been called
    with no intervening changes to the graph.
  */
  int numCycleOut() const { return m_cycleOut.size(); }

  /** \details
    Returns the number of incoming cyclical references associated with this GraphNode object.
    
    \remarks
    This function returns valid results only if OdDbGraph::findCycles() has been called
    with no intervening changes to the graph.
  */
  int numCycleIn() const { return m_cycleIn.size(); }
  
  /** \details
    Returns the specified incoming cyclical reference of this GraphNode object.
    
    \param refIndex [in]  Reference index. 

    \returns
    Returns a null pointer if and only if the index is not valid.

    \remarks
    This function returns valid results only if OdDbGraph::findCycles() has been called
    with no intervening changes to the graph.
  */
  OdDbGraphNode* cycleIn(
    int refIndex) const { return m_cycleIn[refIndex]; }

  /** \details
    Returns the specified outgoing cyclical reference of this GraphNode object.
    
    \param refIndex [in]  Reference index. 

    \returns
    Returns a null pointer if and only if the index is not valid.

    \remarks
    This function returns valid results only if OdDbGraph::findCycles() has been called
    with no intervening changes to the graph.
  */
  OdDbGraphNode* cycleOut(
    int refIndex) const  { return m_cycleOut[refIndex]; }
  
  /** \details
    Returns the next outgoing cyclical reference of this GraphNode object.
    
    \returns
    This function returns cycleOut(0). Returns a null pointer if cycleOut(0) == 0.

    \remarks
    This function returns valid results only if OdDbGraph::findCycles() has been called
    with no intervening changes to the graph.
  */
  OdDbGraphNode* nextCycleNode() const { return cycleOut(0); }
  
  /** \details
    Returns true if and only if this GraphNode object is part of a cyclical reference.

    \remarks
    This function returns valid results only if OdDbGraph::findCycles() has been called
    with no intervening changes to the graph.
  */
  bool isCycleNode() const { return (numCycleOut() != 0 || numCycleIn() != 0); }
  
private:
  void setOwner(
    OdDbGraph* pGraph) 
  {
    if(m_pOwner)
    {
      ODA_FAIL();
      throw OdError(eInvalidOwnerObject);
    }
    m_pOwner = pGraph;
  }
  
  friend struct if_leaf_push_to;
  friend struct clear_cycles;
  friend void break_edge(OdDbGraphNode* , OdDbGraphNode* );
  
  void*               m_pData;
  OdUInt8             m_flags;
  OdDbGraphNodeArray  m_outgoing;
  OdDbGraphNodeArray  m_incoming;
  OdDbGraph*          m_pOwner;
  OdDbGraphNodeArray  m_cycleOut;
  OdDbGraphNodeArray  m_cycleIn;
};
/** \details
    This template class is a specialization of the OdSmartPtr class template for OdDbGraphNode object pointers.
*/
typedef OdSmartPtr<OdDbGraphNode> OdDbGraphNodePtr;

/** \details
    This class implements stacks for OdDbGraphNode object pointers.
    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class OdDbGraphStack
{
public:
  /** \param initPhysicalLength [in]  Initial Physical Length.
      \param initGrowLength [in]  Initial Grow Length.


    \remarks
    Physical Length is the maximum number of entries in this Stack object before it automatically grows.
    
    Grow Length is the number of entries by which the Physical Length will grow as required.
  */
  OdDbGraphStack(
    int initPhysicalLength = 0, 
    int initGrowLength = 8)
    : m_stack(initPhysicalLength, initGrowLength) {}

  ~OdDbGraphStack() {}

  /** \details
    Pushes the specified GraphNode onto this Stack object.
    
    \param pNode [in]  Pointer to the node to be pushed.
  */
  void push(
    OdDbGraphNode* pNode) { m_stack.push_back(pNode); }

  /** \details
    Pops and returns the GraphNode at the top of this Stack object.
    
    \remarks
    Returns a null pointer if this Stack object is empty.
  */
  OdDbGraphNode* pop()
  {
    if(!isEmpty())
    {
      OdDbGraphNode* pRes = top();
      m_stack.removeLast();
      return pRes;
    }
    return 0;
  }


  /** \details
    Returns the OdDbGraphNode at the top of this Stack object.

    \remarks
    Returns a null pointer if this Stack object is empty.
  */
  OdDbGraphNode* top() const { return isEmpty() ? 0 : m_stack.last(); }

  /** \details
    Returns true if and only if this Stack object is empty.
  */
  bool isEmpty() const { return m_stack.empty(); }
private:
  OdDbGraphNodeArray  m_stack;
};

/** \details
    This class implements generic graph objects.
    
    \remarks
    A graph consists of a collection of nodes bi-directionally linked by directional edges.
    
    An edge connected to a node is represented as a pointer or reference to the node at the other
    end of the edge.
 
    References are classified as either incoming or outgoing. Every incoming reference has
    a corresponding outgoing reference and vice versa.

    Each GraphNode object can have any number of references associated with it, enabling the
    implementation of any graph structure.

    \sa
    * OdDbGraphNode
    * OdDbGraphStack
    * OdDbXrefGraph
    * OdDbXrefGraphNode
    
    Corresponding C++ library: TD_Db
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGraph
{
  friend class OdDbGraphNode;
public:
  OdDbGraph() : m_bDirty(false), m_nNonCycleNodes(0) {}
  virtual ~OdDbGraph();
  
  /** \details
    Returns the specified GraphNode object of this Graph object.
    
    \param nodeIndex [in]  Node index. 
  */
  OdDbGraphNode* node(
    int nodeIndex) const { return m_nodes.at(nodeIndex); }
  
  /** \details
    Returns the root (first) GraphNode object of this Graph object.
    
    \remarks
    Returns a null pointer if isEmpty().
  */
  OdDbGraphNode* rootNode() const;
  
  /** \details
    Returns the specified incoming reference of this Graph object.
    
    \param refIndex [in]  Reference index. 
  */
  int numNodes() const { return m_nodes.size(); }

  /** \details
    Returns true if and only if this Graph object is empty.
  */
  bool isEmpty() const { return numNodes() == 0; }
  
  /** \details
    Adds the specifed GraphNode object to this Graph object.
    
    \param pNode [in]  Pointer to the GraphNode object.
    
    \remarks
    *pNode must be created with new(), and should not be deleted once added to this Graph object; 
    This Graph object will delete the GraphNode when it is no longer needed.
    
    Throws:
    OdError(eInvalidOwnerObject) if the specified GraphNode object already has an owner.
  */
    void addNode(
    OdDbGraphNode* pNode);
  
  /** \details
    Adds the specified edge to this Graph object.

    \param pFrom [in]  Pointer to the GraphNode at the start of the edge.
    \param pTo [in]  Pointer to the GraphNode at the end of the edge.
    
    \remarks
    Creates an outgoing reference to *pTo in *pFrom, and an incoming reference to *pFrom in *pTo. 
        
    Throws:
    OdError(eInvalidOwnerObject) if the specified GraphNode objects are not in the same Graph object.
  */
    void addEdge(
    OdDbGraphNode* pFrom, 
    OdDbGraphNode* pTo);
  
  /** \details
    Removes the specified node from this GraphNode object, and all references to it. 

    \param pNode [in]  Pointer to the GraphNode object.
  */
    void delNode(
    OdDbGraphNode* pNode);
  
  /** \details
    Removes all nodes and cycle nodes from this Graph object.

    \remarks
    Always called by ~OdDbGraph().  
  */
  void reset();

  /** \details
    Clears the specified set flag bits in the GraphNode objects of this Graph object.
    
    \param flags [in]  Flag bits.
    
    \remarks
    flags must be a combination of one or more of the following:
    
    <table>
    Name                             Value    Description
    OdDbGraphNode::kNone             0x00     None.
    OdDbGraphNode::kVisited          0x01     Used internally by OdDbGraph::findCycles() and while traversing a graphs with cycles.
    OdDbGraphNode::kOutsideRefed     0x02     Used internally by Xref detach.
    OdDbGraphNode::kSelected         0x04     User selection. Set by getOutgoing.
    OdDbGraphNode::kInList           0x08     Is in list. Set by getOutgoing.
    OdDbGraphNode::kListAll          0x0E     Used to clear kSelected, kInList, kOutsideRefed.
    OdDbGraphNode::kFirstLevel       0x10     The node is connected to the root node. Read Only.
    OdDbGraphNode::kUnresTree        0x20     The tree is unresolved.
    OdDbGraphNode::kAll              0x2F     Used to clear all but kFirstLevel.
    </table>

    OdDbGraphNode::kListAll and OdDbGraphNode::kAll are intended to be used with this function.
  */
  void clearAll(
    OdUInt8 flags);
  
  /** \details
    Adds to the specified array, the nested outgoing GraphNode objects from the specified GraphNode objects.
    \param outgoing [in/out] Array of GraphNode objects. 

    \remarks
    The user must clear the kListAll flags with OdDbGraph::clearAll() or OdDbGraphNode::clear() when done with them.
  */
  void getOutgoing(
    OdDbGraphNodeArray& outgoing);
  
  /** \details
    Finds the cyclical nodes for this Graph object.
    \param pStart [in]  Pointer to the starting node from which to search. Usually defaulted.

    \returns
    Returns true if and only if there are any cyclical GraphNodes.
    
    \remarks
    This function must be called with no intervening changes to the graph, prior to querying cycle information.
  */
  virtual bool findCycles(
    OdDbGraphNode* pStart = 0);
  /** \remarks
    Removes the specified edge, and updates the cyclical information for this Graph object.

    \param pFrom [in]  Pointer to the GraphNode at the start of the edge.
    \param pTo [in]  Pointer to the GraphNode at the end of the edge.
  */
  void breakCycleEdge(
    OdDbGraphNode* pFrom, 
    OdDbGraphNode* pTo);
  
protected:
  /** \details
    Removes all cyclical information determined by findCycles.
    
    \ewmarks
    This function does not remove cyclical edges, and is intended
    only for error cleanup.
    
    \sa
    breakCycleEdge
  */
  void clearAllCycles();
private:
  OdDbGraph(
    const OdDbGraph&);
  OdDbGraph& operator =(
    const OdDbGraph&);
  
  void removeLeaves(
    OdDbGraphStack& stack);
  void setDirty() { m_bDirty = true; }
  bool isDirty() const { return m_bDirty; }
  bool m_bDirty;
  OdDbGraphNodeArray::size_type m_nNonCycleNodes;
  OdDbGraphNodeArray m_nodes;
};

#include "TD_PackPop.h"

#endif // _ODDBGRAPH_H_INCLUDED_
