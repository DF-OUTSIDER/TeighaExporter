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

#ifndef _DbEvalGraph_h_Included_
#define _DbEvalGraph_h_Included_

#include "SmartPtr.h"
#include "DbObject.h"

#include "TD_PackPush.h"

// Forward class declarations

class  OdDbEvalGraph;
class  OdDbEvalEdgeInfo;
class  OdDbEvalExpr;
class  OdDbEvalConnectable;
class  OdDbEvalContext;
class  OdDbEvalContextPair;
class  OdDbEvalContextIterator;
class  OdResBuf;
class  OdDbImpEvalContext;
class  OdDbImpEvalContextIterator;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbEvalGraph object pointers.
*/
typedef OdSmartPtr<OdDbEvalGraph> OdDbEvalGraphPtr;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbEvalContextIterator object pointers.
*/
typedef OdSmartPtr<OdDbEvalContextIterator> OdDbEvalContextIteratorPtr;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbEvalExpr object pointers.
*/
typedef OdSmartPtr<OdDbEvalExpr> OdDbEvalExprPtr;

typedef unsigned long OdDbEvalNodeId;

/** \details
    This template class is a specialization of the OdArray class for OdDbEvalNodeId objects.
*/
typedef OdArray<OdDbEvalNodeId, OdMemoryAllocator<OdDbEvalNodeId> > OdDbEvalNodeIdArray;
/** \details
    This template class is a specialization of the OdArray class for OdDbEvalEdgeInfo objects.
*/
typedef OdArray<OdDbEvalEdgeInfo> OdDbEvalEdgeInfoArray;
/** \details
    This template class is a specialization of the OdArray class for OdDbEvalExprPtr object SmartPointers.
*/
typedef OdArray<OdDbEvalExprPtr> OdDbEvalExprPtrArray;
/** \details
    This template class is a specialization of the OdArray class for OdResBufPtr object SmartPointers.
*/
typedef OdArray<OdResBufPtr> OdResBufArray;

#include "OdPlatformSettings.h"
#include "StringArray.h"

#ifdef DYNBLOCKS_DLL_EXPORTS
#ifndef SWIG
#define DYNBLOCKS_EXPORT        OD_TOOLKIT_EXPORT
#else
#define DYNBLOCKS_EXPORT        
#endif
#define DYNBLOCKS_EXPORT_STATIC OD_STATIC_EXPORT
#else       
#ifndef SWIG
#define DYNBLOCKS_EXPORT        OD_TOOLKIT_IMPORT
#else
#define DYNBLOCKS_EXPORT        
#endif
#define DYNBLOCKS_EXPORT_STATIC OD_STATIC_IMPORT
#endif

/** \details
    This class implements the behavior of dynamic blocks with a network of interrelated
    elements.

    \remarks
    Each element (OdDbEvalExpr object) depends on zero or more other elements. The
    dependencies are represented as edges to dependent nodes from the nodes
    on which they depend.

    In response to OdDbEvalGraph::evaluate(), this class traverses the graph and invokes
    OdDbEvalExpr::evaluate() for each of the elements in the graph in the proper order.

    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>

    \sa
    * OdDbEvalConnectable
    * OdDbEvalContext
    * OdDbEvalContextIterator
    * OdDbEvalContextPair
    * OdDbEvalEdgeInfo
    * OdDbEvalExpr
*/
class DYNBLOCKS_EXPORT OdDbEvalGraph : public OdDbObject
{
public:

  enum NodeId {
    kNullNodeId = 0
  };

  ODDB_DECLARE_MEMBERS(OdDbEvalGraph);

  OdDbEvalGraph();
  virtual ~OdDbEvalGraph();

  /* Management */

  /** \details
    Returns true if and only there is a graph with the specified key on the specified object.
    \param pObj [in]  Pointer to the object.
    \param key [in]  Graph key.
  */
  static bool hasGraph(const OdDbObject* pObj,
    const OdString& key);

  /** \details
    Returns the Graph object with the specified key on the specified object.
    \param pObj [in]  Pointer to the object.
    \param key [in]  Graph key.
    \param openMode [in]  Mode in which to open the Graph object.
  */
  static OdDbEvalGraphPtr getGraph(const OdDbObject* pObj,
    const OdString& pKey,
    const OdDb::OpenMode openMode = OdDb::kForRead );


  /** \details
    Creates a graph with the specified key on the specified object.
    
    \param pObj [in]  Pointer to the object.
    \param key [in]  Graph key.
    
    \returns
    Returns true if and only if successful.
  */
  static bool createGraph(OdDbObject* pObj,
    const OdString& key);


  /** \details
    Removes the graph with the specified key on the specified object.
    \param pObj [in]  Pointer to the object.
    \param key [in]  Graph key.
    \returns
    Returns true if and only if there was a graph with the specified key.
  */
  static bool removeGraph(OdDbObject* pObj,
    const OdString& pKey);

  /** \details
    Returns true if and only there is a graph with the specified key on the specified database.
    \param pDb [in]  Pointer to the database.
    \param key [in]  Graph key.
  */
  static bool hasGraph(OdDbDatabase* pDb,
    const OdString& key);

  /** \details
    Returns the Graph object with the specified key on the specified database.

    \param pDb [in]  Pointer to the database.
    \param key [in]  Graph key.
    \param openMode [in]  Mode in which to open the Graph object.
  */
  static OdDbEvalGraphPtr getGraph(OdDbDatabase* pDb,
    const OdString& key,
    const OdDb::OpenMode openMode = OdDb::kForRead );

  /** \details
    Creates a graph with the specified key on the specified database.
    
    \param pDb [in]  Pointer to the database.
    \param key [in]  Graph key.
    
    \returns
    Returns true if and only if there was a graph with the specified key.
  */
  static bool createGraph(OdDbDatabase* pDb,
    const OdString& key);

  /** \details
    Removes the graph with the specified key on the specified database. 
    \param pDb [in]  Pointer to the database.
    \param key [in]  Graph key.
    \returns
    Returns true if and only if there was a graph with the specified key.
  */
  static bool removeGraph(OdDbDatabase* pDb,
    const OdString& key);

  /** \details
    Adds a node to this Graph object.
    \param pNode [in]  Pointer to the GsNode object.
    \returns
    Returns the node ID of the newly added node.

    \remarks
    For database resident Graphs, the user is expected to close returned nodes
    when done for them. This includes the pNode argument.
  */
  virtual OdDbEvalNodeId addNode(OdDbEvalExpr* pNode);

  /** \details
    Removes a node from this Graph object.
    
    \param nodeId [in]  Node ID.
    \returns
    Returns true if and only if the node existed.
  */
  virtual bool removeNode  (const OdDbEvalNodeId& nodeId);

  /** \details
    Removes a node from this Graph object.
    \param pNode [in]  Pointer to the GsNode object.

    \remarks
    If pNode is specified, it is assumed the node is open for read or write.
  */
  virtual bool removeNode (OdDbEvalExpr* pNode);

  /** \details
    Returns an array of the node IDs of all nodes in this Graph object.
    \param allNodes [out]  Receives an array of node IDs.
  */
  virtual void getAllNodes (OdDbEvalNodeIdArray& allNodes) const;

  /** \details
    Opens and returns a SmartPointer to the specified node in this Graph object.
    \param nodeId [in]  Node ID.
    \param openMode [in]  Mode in which to open the Node object.
  */
  virtual OdDbEvalExprPtr getNode( const OdDbEvalNodeId& nodeId, OdDb::OpenMode openMode = OdDb::kForRead ) const;


  /** \details
    Adds the specified edge to this Graph object.

    \param idFrom [in]  Node ID of the node at the start of the edge.
    \param idTo [in]  Node ID of the node at the end of the edge.

    \remarks
    Creates an outgoing reference to idTo in idFrom, and an incoming reference to idFrom in idTo.

    Throws:
    OdError(eInvalidOwnerObject) if the specified GraphNode objects are not in the same Graph object.
  */
  virtual bool addEdge(const OdDbEvalNodeId& idFrom,
    const OdDbEvalNodeId& idTo);

  /** \details
    Adds the specified edge to this Graph object.
    
    \param idFrom [in]  Node ID of the node at the start of the edge.
    \param idTo [in]  Node ID of the node at the end of the edge.
    \param invertible [in]  True if and only if the edge is invertible.

    \remarks
    An invertible edge can have its originating and terminating nodes swapped
    based on which of the nodes is activated.
  */
  virtual bool addEdge(const OdDbEvalNodeId& idFrom,
    const OdDbEvalNodeId& idTo,
    bool  invertible);


  /** \details
    Removes the specified edge from this Graph object.

    \param idFrom [in]  Node ID of the node at the start of the edge.
    \param idTo [in]  Node ID of the node at the end of the edge.

    \remarks
    Creates an outgoing reference to idTo in idFrom, and an incoming reference to idFrom in idTo.

    Throws:
    OdError(eInvalidOwnerObject) if the specified GraphNode objects are not in the same Graph object.
  */
  virtual bool removeEdge(const OdDbEvalNodeId& idFrom,
    const OdDbEvalNodeId& idTo);

  /** \details
    Returns an array of the incoming edges for the specified node of this Graph object.

    \param nodeId [in]  Node ID.
    \param edges [in]  Receives an array of edges.
  */
  virtual void getIncomingEdges(const OdDbEvalNodeId& nodeId,
    OdDbEvalEdgeInfoArray& edges) const;

  /** \details
    Returns an array of the outgoing edges for the specified node of this Graph object.

    \param nodeId [in]  Node ID.
    \param edges [in]  Receives an array of edges.
  */
  virtual void getOutgoingEdges(const OdDbEvalNodeId& nodeId,
    OdDbEvalEdgeInfoArray& edges) const;


  /** \details
    Returns the edge information for the specified edge of this Graph object.
    \param idFrom [in]  Node ID of the node at the start of the edge.
    \param idTo [in]  Node ID of the node at the end of the edge.
    \param edgeInfo [out]  Receives the edge information.
    \returns
    Returns true if and only if successful.
  */
  virtual bool getEdgeInfo(const OdDbEvalNodeId& idFrom,
    const OdDbEvalNodeId& idTo, OdDbEvalEdgeInfo& edgeInfo) const;


  /** \details
    Moves the nodes and edges from the specified Graph object to this Graph object.
    \param pGraphToAdd [in]  Pointer to the Graph object to add.
  */
  virtual void addGraph(OdDbEvalGraph* pGraphToAdd);

  /* Evaluation */

  /** \details
    Evaluates this Graph object.

    \remarks
    Evaluation is accomplished by traversing the activated nodes on the Graph,
    invoking OdDbEvalExpr::evaluate() on each of the visited nodes.

    Nodes may be activated with either the activate() method, or by
    supplying an activatedNodes argument.

    \remarks
    Nodes not activated are deactivated.

    \returns
    Returns true if and only if successful.
  */
  virtual bool evaluate() const;

  /** \details
    Evaluates this Graph object.

    \param pContext [in]  Pointer to the Context object for the evaluation.
  */
  virtual bool evaluate(
    const OdDbEvalContext* pContext) const;

  /** \details
    Evaluates this Graph object.
    
    \param pContext [in]  Pointer to the Context object for the evaluation.
    \param pActivateNodes [in]  Pointer to an array of node IDs to activate.
  */
  virtual bool evaluate(
    const OdDbEvalContext* pContext,
    const OdDbEvalNodeIdArray* pActivateNodes) const;


  /** \details
    Activates the specified nodes in this Graph object.

    \param activatedNodes [in]  Array of node IDs to activate.

    \remarks
    Nodes not activated are deactivated.

    \returns
    Returns true if and only if the graph is non-cyclic.
  */
  virtual bool activate(
    const OdDbEvalNodeIdArray& activatedNodes) const;


  /** \details
    Activates the specified nodes in this Graph object.
    
    \param activatedNodes [in]  Array of node IDs to activate.
    \param pActiveSubgraph [in]  Pointer to an array to receive the node IDs that would visited, given the activated nodes.
  */
  virtual bool activate(
    const OdDbEvalNodeIdArray& activatedNodes,
    OdDbEvalNodeIdArray* pActiveSubgraph) const;

  /** \details
    Activates the specified nodes in this Graph object.
    
    \param pCycleNodes [in]  Pointer to an array to receive the node IDs that would be cyclic, given the activated nodes.
  */
  virtual bool activate(
    const OdDbEvalNodeIdArray& activatedNodes,
    OdDbEvalNodeIdArray* pActiveSubgraph,
    OdDbEvalNodeIdArray* pCycleNodes) const;


  /** \details
    Returns true if and only if the specified node in this Graph object is active.

    \param nodeId [in]  Node ID.
    \param isActive [in]  True if and only if node is active.

    \remarks
    The return value of this function is always true. The active state of the node is returned via isActive.
  */
  virtual bool getIsActive(const OdDbEvalNodeId& nodeId,
    bool& isActive) const;

  /** \details
    Returns true if and only if the specified Graph object is equal to this one.

    \param pOther [in]  Pointer to the other Graph object.

    \returns
    Returns true if and only if

                this->subgraph(pOther) && pOther->subgraph(this)
  */
  virtual bool equals(const OdDbEvalGraph* pOther) const;


  /** \details
    Returns true if and only if the specified Graph object is a subgraph of this one.

    \param pOther [in]  Pointer to the other Graph object.
  */
  virtual bool isSubgraphOf(const OdDbEvalGraph* pOther) const;

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler) ODRX_OVERRIDE;
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler) ODRX_OVERRIDE;
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;
  virtual void applyPartialUndo(OdDbDwgFiler* pFiler, OdRxClass* pClassObj) ODRX_OVERRIDE;
  virtual void copyFrom(const OdRxObject* p) ODRX_OVERRIDE;
  virtual OdResult postInDatabase(OdDbDatabase*);
  virtual void subHandOverTo(
    OdDbObject* pNewObject) ODRX_OVERRIDE;
};


/** \details
    This class holds the directed edge information between pairs of nodes in an OdDbEvalGraph instance.

    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>

    \sa
    * OdDbEvalConnectable
    * OdDbEvalContext
    * OdDbEvalContextIterator
    * OdDbEvalContextPair
    * OdDbEvalExpr
    * OdDbEvalGraph
*/
class DYNBLOCKS_EXPORT OdDbEvalEdgeInfo
{
public:
  OdDbEvalEdgeInfo()
    : mFlags(0), mRefCount(0)
  {
    mIdFrom = OdDbEvalGraph::kNullNodeId;
    mIdTo = OdDbEvalGraph::kNullNodeId;
  }

  /** \param fromId [in]  Node Id of the "from" Node object sharing the edge.
    \param toId [in]  Node Id of the "to" Node object sharing the edge.
    \param flags [in]  For internal use only.
    \param count [in]  Number of times this edge was added to the graph.
  */
  OdDbEvalEdgeInfo(OdDbEvalNodeId fromId,
    OdDbEvalNodeId toId,
    long           flags,
    unsigned long  count)
    : mIdFrom(fromId), mIdTo(toId), mFlags(flags), mRefCount(count)
  {}

  /** \details
    Returns the Object ID of the originating node of this edge.
  */
  OdDbEvalNodeId from () const;

  /** \details
    Returns the Object ID of the terminating node of this edge.
  */
  OdDbEvalNodeId to () const;
  /** \details
    Returns the number of times this edge was added to the graph.
  */
  unsigned long refCount() const;

  /** \details
    Returns true if and only if this edge is invertible.
    \remarks
    An invertible edge can have its originating and terminating nodes swapped
    based on which of the nodes is activated.
  */
  bool isInvertible () const;
  /** \details
    Returns true if and only if this edge is suppressed.
    \remarks
    As suppressed edge is temporarily removed from the graph during activation or evaluation. This applies only to
    invertible pairs of edges.
  */
  bool isSuppressed  () const;

  bool operator == (const OdDbEvalEdgeInfo& other) const;

private:
  OdDbEvalNodeId mIdFrom;
  OdDbEvalNodeId mIdTo;
  long mFlags;
  unsigned long mRefCount;
};


/** \details
    This class represents single nodes in OdDbEvalGraph objects.

    \remarks
    Each node reprsents an action or expression. The OdDbEvalGraph object calls each owned node's
    evaluate() method while traversing the graph within the OdDbEvalGraph::evaluate() method.

    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>

    \sa
    * OdDbEvalConnectable
    * OdDbEvalContext
    * OdDbEvalContextIterator
    * OdDbEvalContextPair
    * OdDbEvalEdgeInfo
    * OdDbEvalGraph
*/
class DYNBLOCKS_EXPORT OdDbEvalExpr : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbEvalExpr);

  OdDbEvalExpr();
  virtual ~OdDbEvalExpr();

  /** \details
    Opens the OdDbEvalGraph object that owns this Node object.

    \param openMode [in]  Mode in which to open the Graph object.

    \returns
    Returns a SmartPointer to the opened OdDbEvalGraph if successful, otherwise a null SmartPointer.
  */
  OdDbEvalGraphPtr getGraph( OdDb::OpenMode openMode = OdDb::kForRead ) const;

  /** \details
    Returns the node ID of this Node object.

    \remarks
    An unique ID is assigned when a Node object it added to a Graph.

    Returns OdDbGraph::kNullId if this Node object is not graph-resident.
  */
  OdDbEvalNodeId nodeId() const;

  /** \details
    Notification function called whenever this Node object has been added to a Graph object.

    \param pGraph [in]  Pointer to the Graph object.

    \remarks
    Called by OdDbEvalGraph::addNode().

    This function is called after the operation.
  */
  virtual void addedToGraph (OdDbEvalGraph* pGraph);

  /** \details
    Notification function called whenever this Node object has been removed a Graph object.

    \param pGraph [in]  Pointer to the Graph object.

    \remarks
    Called by OdDbEvalGraph::removeNode().

    This function is called after the operation.
  */
  virtual void removedFromGraph (OdDbEvalGraph* pGraph);


  /** \details
    Notification function called whenever a Node object with a shared edge
    to this Node object has been removed a Graph object.

    \param adjNodeId [in]  Node Id of the adjacent Node object.

    \remarks
    Called by OdDbEvalGraph::removeNode().

    This function is called after the operation.
  */
  virtual void adjacentNodeRemoved (const OdDbEvalNodeId& adjNodeId);


  /** \details
    Notification function called whenever an edge of this Node object has been removed.

    \param adjEdgeNodeId [in]  Node Id of the Node object sharing the edge.

    \remarks
    Called by OdDbEvalGraph::removeEdge().

    This function is called after the operation.
  */
  virtual void adjacentEdgeRemoved (const OdDbEvalNodeId& adjEdgeNodeId);


  /** \details
    Notification function called whenever an edge has been added the Graph object.

    \param fromId [in]  Node Id of the "from" Node object sharing the edge.
    \param toId [in]  Node Id of the "to" Node object sharing the edge.
    \param isInvertible [in]  True if and only if the edge the edge is invertible.
    \remarks
    Called by OdDbEvalGraph::addEdge().

    When the edge is invertible, this function is called twice.

    This function is called after the operation.
  */
  virtual void adjacentEdgeAdded (const OdDbEvalNodeId& fromId,
    const OdDbEvalNodeId& toId,
    bool isInvertible);


  /** \details
    Notification function called whenever this Node object
    is about to be moved to another Graph object.

    \param pFromGraph [in]  Pointer to the Graph object from which this Node object will be moved.

    \remarks
    Called by OdDbEvalGraph::addGraph().

    This function is called before the operation.
  */
  virtual void movedFromGraph (OdDbEvalGraph* pFromGraph);

  /** \details
    Notification function called whenever this Node object
    has been moved to another Graph object.

    \param pIntoGraph [in]  Pointer to the Graph object into which this Node object was moved.

    \remarks
    Called by OdDbEvalGraph::addGraph().

    This function is called after the operation.
  */
  virtual void movedIntoGraph (OdDbEvalGraph* pIntoGraph);



  /** \details
    Notification function called whenever a Node object
    has been copied to another Graph object.

    \param pIntoGraph [in]  Pointer to the Graph object into which a Node object was copied.

    \remarks
    Called by OdDbEvalGraph::copyFrom().

    This function is called after the operation.
  */
  virtual void copiedIntoGraph(OdDbEvalGraph* pIntoGraph);

  /** \details
    Returns true if and only if this Node object can be activated.

    \remarks
    The default implementation of this function always returns true.
  */
  virtual bool isActivatable  () const;

  /** \details
    Notification function called whenever this Node object has been activated.

    \param argumentActivateList [in]  Array of node IDs being activated.

    \remarks
    Called by OdDbEvalGraph::activate() or by OdDbEvalGraph::evaluate().

    When the edge is invertible, this function is called twice.

    This function is called after the operation.
  */
  virtual void activated           (OdDbEvalNodeIdArray& argumentActiveList);

  /** \details
    Notification function called whenever the Graph object is about to be traversed.

    \param nodeIsActive [in]  True if and only if this Node object has been activated.

    \remarks
    Called by OdDbEvalGraph::evaluate().

    This function is called before the operation.
  */
  virtual void graphEvalStart (bool nodeIsActive);


  /** \details
    Notification function called whenever the Graph object has been traversed.

    \param nodeIsActive [in]  True if and only if this Node object has been activated.

    \remarks
    Called by OdDbEvalGraph::evaluate().

    This function is called after the operation.
  */
  virtual void graphEvalEnd(bool nodeIsActive);

  /** \details
    Notification function called whenever the Graph object travesal is been terminated.

    \param nodeIsActive [in]  True if and only if this Node object has been activated.

    \remarks
    Called by OdDbEvalGraph::evaluate().

    This function is called after the operation.
  */
  virtual void graphEvalAbort(bool nodeIsActive);

  /** \details
    Evaluates the expression represented by this Node object.
    \param evalContext [in]  Evaluation context.

    \remarks
    Called by OdDbEvalGraph::evaluate().

    Returns true if and only if successful. Returning false terminates the Traversal.

    evalContext may be NULL.
  */
  virtual bool evaluate(const OdDbEvalContext* evalContext);

  /** \details
    Returns true if and only if the specified Node object is equal to this Node object.

    \param pOther [in]  Pointer to the other Node object.
    \remarks
    The default implementation of this function does nothing but
    throw an eNotImplemented error.
  */
  virtual bool equals(const OdDbEvalExpr* pOther) const;

  /** \details
    Returns the value of the evaluated expression of this Node object.
    \remarks
    The value is updated with a call to OdDbEvalExpr::evaluate().
  */
  OdResBufPtr value() const;

  OdResult dwgInFields(OdDbDwgFiler* pFiler);
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  virtual OdResult postInDatabase(OdDbDatabase*);

protected:

  OdResBufPtr m_lastValue;
  friend class OdDbEvalExprImpl;
};


/** \details
    This class implements a runtime inspection interface for OdDbEvalExpr objects.

    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>

    \sa
    * OdDbEvalContext
    * OdDbEvalContextIterator
    * OdDbEvalContextPair
    * OdDbEvalEdgeInfo
    * OdDbEvalExpr
    * OdDbEvalGraph
*/
class DYNBLOCKS_EXPORT OdDbEvalConnectable : public OdDbEvalExpr
{
public:
  ODDB_DECLARE_MEMBERS(OdDbEvalConnectable);

  virtual ~OdDbEvalConnectable();

  /** \details
    Returns an array of Connection names for this Connectable object.
    \param connectionNames [out]  Receives an array of Connection names.
  */
  virtual void getConnectionNames (OdStringArray& connectionNames) const;

  /** \details
    Returns true if and only if this Connectable object has a connection with the specified name.
    \param connectionName [in]  Connection name.
  */
  virtual bool hasConnectionNamed (const OdString& connectionName) const;


  /** \details
    Returns the type of the specified connection for this Connectable object.
    \param connectionName [in]  Connection name.
    \param connectionType [out]  Receives the connection type.
    \returns
    Returns true if and only if a connection with the specified name exists.
  */
  virtual bool getConnectionType  (
    const OdString& connectionName,
    OdResBuf::ValueType& connectionType) const;


  /** \details
    Returns the value of the specified connection for this Connectable object.
    \param connectionName [in]  Connection name.
    \remarks
    Returns a NULL SmartPointer if a connection with the specified name does not exist.
  */
  virtual OdResBufPtr getConnectionValue( const OdString& connectionName) const;


  /** \details
    Sets the value of the specified connection for this Connectable object.
    \param connectionName [in]  Connection name.
    \param pValue [in]  Pointer to the value.
    \returns
    Returns true if and only if a connection with the specified name exists.
  */
  virtual bool setConnectionValue ( const OdString& connectionName, const OdResBuf* pValue);


  /** \details
    Connects this Connectable object to the specified Connectable object.

    \remarks
    Both objects must graph-resident.

    \param thisConnectionName [in]  Connection name on this object.
    \param sourceConnectableId [in]  Object ID of the other Connectable object.
    \param sourceConnectionName [in]  Connection name on the other Connectable object.
    \returns
    Returns true if and only if successful.
  */
  virtual bool connectTo (
    const OdString&      thisConnectionName,
    OdDbEvalNodeId sourceConnectableId,
    const OdString&      sourceConnectionName);


  /** \details
    Disconnects this Connectable object from the specified Connectable object.

    \remarks
    Both objects must graph-resident.

    \param thisConnectionName [in]  Connection name on this object.
    \param sourceConnectableId [in]  Object ID of the other Connectable object.
    \param sourceConnectionName [in]  Connection name on the other Connectable object.
    \returns
    Returns true if and only if successful.
  */
  virtual bool disconnectFrom (
    const OdString&      thisConnectionName,
    OdDbEvalNodeId sourceConnectableId,
    const OdString&      sourceConnectionName);

  /** \details
    Returns true if and only if a connection is allowed from this Connectable object
    to the specified Connectable object.

    \remarks
    Both objects must graph-resident.

    \param thisConnectionName [in]  Connection name on this object.
    \param sourceConnectableId [in]  Object ID of the other Connectable object.
    \param sourceConnectionName [in]  Connection name on the other Connectable object.
  */
  virtual bool connectionAllowed (
    const OdString&      thisConnectionName,
    OdDbEvalNodeId sourceConnectableId,
    const OdString&      sourceConnectionName) const;


  /** \details
    Returns an array of Connectable objects to which this Connectable object
    is connected.

    \param thisConnectionName [in]  Connection name on this object.
    \param connectedObjects [out]  Receives an array of object IDs.
    \returns
    Returns true if and only if successful.
  */
  virtual bool getConnectedObjects (
    const OdString&      thisConnectionName,
    OdDbEvalNodeIdArray& connectedObjects) const;

  /** \details
    Returns an array Connection names shared between this OdDbEvalConnectable object and another.

    \param thisConnectionName [in]  Connection name on this object.
    \param sourceConnectableId [in]  ID of the other Connectable object.
    \param sourceConnectionNames [out]  Receives an array of connection names.

    \returns
    Returns true if and only if successful.
  */
  virtual bool getConnectedNames (
    const OdString&      thisConnectionName,
    OdDbEvalNodeId sourceConnectableId,
    OdStringArray&       sourceConnectionNames) const;
protected:

  OdDbEvalConnectable();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbEvalConnectable object pointers.
*/
typedef OdSmartPtr<OdDbEvalConnectable> OdDbEvalConnectablePtr;

/** \details
    This class represents application data used during the evaluation of an OdDbEvalGraph object.

    \remarks
    The graph passes the OdDbEvalContext object passed to OdDbEvalGraph::evaluate() to each node
    in the graph traversed as a result of calling OdDbEvalExpr::evaluate().

    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>

    \sa
    * OdDbEvalConnectable
    * OdDbEvalContextIterator
    * OdDbEvalContextPair
    * OdDbEvalEdgeInfo
    * OdDbEvalExpr
    * OdDbEvalGraph
*/
class DYNBLOCKS_EXPORT OdDbEvalContext : public OdRxObject
{
public:

  OdDbEvalContext();


  virtual ~OdDbEvalContext();

  ODRX_DECLARE_MEMBERS(OdDbEvalContext);


  /** \details
    Inserts the specified ContextPair into this Context object.
    \param pair [in]  ContextPair to insert.
    \remarks
    Replaces any ContextPair with the same key.
  */
  virtual void insertAt (const OdDbEvalContextPair& pair);


  /** \details
    Removes the ContextPair with the specified key from this Context object.
    \param key [in]  Key of ContextPair to remove.
  */
  virtual void removeAt (const OdString& key);


  /** \details
    Returns the ContextPair with the specified key from this Context object.

    pair (I/0) Specifies / receives the ContextPair.

    \remarks
    pair is initialized with the key of the ContextPair to return. If the key exists in this Context object,
    its associated value replaces the value in the specified ContextPair.

    Returns true if and only if the pari exists.
  */
  virtual bool getAt (OdDbEvalContextPair& pair) const;


  /** \details
    Returns an Iterator object that can be used to traverse this Context object.
  */
  virtual OdDbEvalContextIteratorPtr newIterator() const;

private:
  OdDbImpEvalContext* m_pImp;
  friend class OdDbImpEvalContextIterator;
};

/** \details
    This class represents key - value pairs stored in an OdDbEvalContext object.

    \remarks
    Values stored are void pointers. Neither OdDbEvalContext nor OdDbEvalContextPair
    manage the memory allocated to the values.

    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>

    \sa
    * OdDbEvalConnectable
    * OdDbEvalContext
    * OdDbEvalContextIterator
    * OdDbEvalEdgeInfo
    * OdDbEvalExpr
    * OdDbEvalGraph
*/
class DYNBLOCKS_EXPORT OdDbEvalContextPair
{
public:

  OdDbEvalContextPair();

  /** \param key [in]  ContextPair key.
    \param value [in]  ContextPair value.
  */
  OdDbEvalContextPair(const OdString& key, void *value);

  virtual ~OdDbEvalContextPair();

  /** \details
    Sets the key of this ContextPair object.
    \param key [in]  ContextPair key.
  */
  void setKey (const OdString& key);

  /** \details
    Returns the key of this ContextPair object.
    \param key [in]  ContextPair key.
  */
  const OdString key () const;

  /** \details
    Sets the value of this ContextPair object.
    \param value [in]  ContextPair value.

    \remarks
    It is up to the caller to allocate and free memory pointed to by value.
  */
  void setValue (void* value);

  /** \details
    Returns the value of this ContextPair object.
    \param value [in]  ContextPair value.
  */
  void*       value    () const;

private:
  OdString mKey;
  void* mValue;
};


/** \details
    This class implements iterators for OdDbEvalContextPair objects in an OdDbEvalContext instance.

    \remarks
    Instances are returned by OdDbEvalContext::newIterator()

    Corresponding C++ library: TD_DynBlocks
    <group OdDb_Classes>

    \sa
    * OdDbEvalConnectable
    * OdDbEvalContext
    * OdDbEvalContextPair
    * OdDbEvalEdgeInfo
    * OdDbEvalExpr
    * OdDbEvalGraph
*/
class DYNBLOCKS_EXPORT OdDbEvalContextIterator : public OdRxObject
{
public:
  OdDbEvalContextIterator();

  virtual ~OdDbEvalContextIterator();

  ODRX_DECLARE_MEMBERS(OdDbEvalContextIterator);

  /** \details
    Sets this Iterator object to reference the EvalContextPair that it would normally return first.

    \remarks
    Allows multiple traversals of the Iterator list.
  */
  virtual void start();

  /** \details
    Increments this Iterator object.
  */
  virtual bool   next();

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
  virtual bool   done() const;

  /** \details
    Returns the ContextPair pointed to by this Iterator Object.
  */
  virtual OdDbEvalContextPair contextPair() const;

private:
  OdDbImpEvalContextIterator* m_pImp;
  friend class OdDbImpEvalContext;
};

#include "TD_PackPop.h"

#endif
