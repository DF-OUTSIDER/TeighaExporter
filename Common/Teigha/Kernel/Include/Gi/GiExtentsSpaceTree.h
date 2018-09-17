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

#ifndef __ODGIEXTENTS3DSPACETREE_H__
#define __ODGIEXTENTS3DSPACETREE_H__

#include "Ge/GeExtents3d.h"
#include "Ge/GeExtents2d.h"
#include "OdVector.h"
#define STL_USING_SET
#define STL_USING_MAP
#include "OdaSTL.h"
#include "OdList.h"

#ifdef _MSC_VER
#pragma warning(disable: 4244)  // 'conversion' conversion from 'type1' to 'type2', possible loss of data
#endif

#include "TD_PackPush.h"

/** \details
  .

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/

class OdGiExtentsSpaceObject
{
  // unique ID of the object
  //IMPORTANT: the user is responsible for the uniqueness of the ID
  OdUInt64    ID;

public:

  // constructor
  OdGiExtentsSpaceObject(OdUInt64 uniqueID)
  {
    ID = uniqueID;
  }

  //set/get ID
  OdUInt64  getID() const {return ID;}
  void      setID(OdUInt64 uniqueID){ID = uniqueID;}

  // pure virtual members
  virtual bool isInExtents(OdGeExtents3d& extents) const  = 0;
  virtual bool isInExtents(OdGeExtents2d& extents) const  = 0;
  virtual bool isEqual(OdGiExtentsSpaceObject* pObject, const OdGeTol& tol = OdGeContext::gTol) const = 0;
};


/** \details
  .

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/

template <class E, class O> class OdGiExtentsSpaceNode //here E should be OdGeExtents3d or OdGeExtents2d
{
  template <OdUInt32 NUM_AXIS, OdUInt32 MAX_DEPTH, OdUInt64 MAX_NODE_OBJECT, class EE, class P, class OO> friend class OdGiExtentsSpaceTree;

  // left child node (only link, not need to delete)
  OdGiExtentsSpaceNode*   m_pLeftChild;
  //right child node (only link, not need to delete)
  OdGiExtentsSpaceNode*   m_pRightChild;
  //parent node (only link, not need to delete)
  OdGiExtentsSpaceNode*   m_pParent;

  // extents of the node
  E                        m_extents;
  
  // array of vectors with objects in the tree
  std::map<int, OdVector<O*, OdMemoryAllocator<O*> >* >* m_pObjectPointers;
  int                                  m_iObjectsTypes;

  // special (the power of 8 or power of 4 ) depth of the node in the tree
  int                 m_iDepth;
public:

  //constructor
  OdGiExtentsSpaceNode(OdGiExtentsSpaceNode* parent, E& extents, int nDepth, int nTypeOfObjects)
  {
    m_pParent = parent;
    m_extents = extents;

    m_pLeftChild = NULL;
    m_pRightChild = NULL;

    m_iObjectsTypes = 0;
    m_pObjectPointers = NULL;
    if ( nTypeOfObjects > 0 )
      m_iObjectsTypes = nTypeOfObjects;

    m_iDepth = nDepth;
  }

  //destructor
  ~OdGiExtentsSpaceNode()
  {
    releaseObjectsStore();
  }

  // methods
  // check that the node is leave
  bool isLeave()
  {
    return (m_pLeftChild == NULL && m_pRightChild == NULL);
  }

  //get the depth of the ndoe
  int getDepth() {return m_iDepth;}


  // retrieve the pointer the stores objects list
  OdVector<O*, OdMemoryAllocator<O*> >* getObjectPointersPtr(int iType)
  {
    if ( m_pObjectPointers == NULL )
      return NULL;

    if ( iType < m_iObjectsTypes )
    {
      OD_TYPENAME std::map<int, OdVector<O*, OdMemoryAllocator<O*> >* >::iterator it = m_pObjectPointers->find(iType);
      if ( it != m_pObjectPointers->end() )
        return it->second;
    }

    return NULL;
  }

private:
  
  // initialize objects list
  OdVector<O*, OdMemoryAllocator<O*> >* initObjectList(int iType, int iMaxObjectsInNode)
  {
    if ( m_pObjectPointers == NULL )
      m_pObjectPointers = new std::map<int, OdVector<O*, OdMemoryAllocator<O*> >* >;

    OdVector<O*, OdMemoryAllocator<O*> >* pNewVector = new OdVector<O*, OdMemoryAllocator<O*> >; 
    (*m_pObjectPointers)[iType] = pNewVector;

    pNewVector->setPhysicalLength(10);
    pNewVector->setGrowLength(iMaxObjectsInNode);
    
    return pNewVector;
  }

  void releaseObjectsStore()
  {
    if ( m_pObjectPointers )
    {
      OD_TYPENAME std::map<int, OdVector<O*, OdMemoryAllocator<O*> >*>::iterator it = m_pObjectPointers->begin();
      OD_TYPENAME std::map<int, OdVector<O*, OdMemoryAllocator<O*> >*>::iterator itEnd = m_pObjectPointers->end();
      while ( it != itEnd )
      {
        if ( it->second )
          delete it->second;
        ++it;
      }
        
      m_pObjectPointers->clear();
      delete m_pObjectPointers;
      m_pObjectPointers = NULL;
    }
  }
};


typedef OdGiExtentsSpaceNode<OdGeExtents2d, OdGiExtentsSpaceObject> OdGiExtents2dSpaceNode;
typedef OdGiExtentsSpaceNode<OdGeExtents3d, OdGiExtentsSpaceObject> OdGiExtents3dSpaceNode;

/** \details
  .

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
template <class E, class O> 
class OdGiExtentsSpaceTreeCallback
{
  public:
    virtual bool notifyObjectPlacedAtNode(O* /*pObject*/, int /*objectType*/, OdGiExtentsSpaceNode<E,O>* pNode) = 0;
};



/** \details
  .

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
template <OdUInt32 NUM_AXIS, OdUInt32 MAX_DEPTH, OdUInt64 MAX_NODE_OBJECT, class E, class P, class O> class OdGiExtentsSpaceTree
{
  //root node
  OdGiExtentsSpaceNode<E, O>*  m_pRootNode;

  // list of all nodes
  OdList<OdGiExtentsSpaceNode<E, O>*> m_Nodes;

  // list of all leaves
  OdList<OdGiExtentsSpaceNode<E, O>*> m_Leaves;

  // pointer to the callback
  OdGiExtentsSpaceTreeCallback<E, O>* m_pCallback;

  //Is addaptive - means that started additioanl splitting when the number of objects starts to be greater than 'm_iMaxNodeObjects'
  bool              m_bIsAdaptive;
  OdUInt64          m_iMaxNodeObjects;

  // for storing the same objects during processing
  OdList<O*>     m_theSameObjects;

  // for storing the intersected leaves during processing
  OdList<OdGiExtentsSpaceNode<E, O>*>     m_theIntersectedLeaves;

 // for storing the leaves where we should add the object (delayed adding in the case of checking the same)
  OdList<OdGiExtentsSpaceNode<E, O>*>     m_theCandidateLeaves;

public:
  
  //constructor
  OdGiExtentsSpaceTree()
  {
    m_pRootNode = NULL;
    m_bIsAdaptive = false;
    m_iMaxNodeObjects = MAX_NODE_OBJECT;
    m_pCallback = NULL;
  }

  //destructor
  ~OdGiExtentsSpaceTree(){reset();}

  //set callback
  void setCallback(OdGiExtentsSpaceTreeCallback<E, O>* pCallback) {m_pCallback = pCallback;}

  //build the tree (the correctness of the incoming extents should check the caller)
   void buildTree(E& extents, int nTypeOfObjects, OdInt depth = 2)
   {   
     if ( depth > MAX_DEPTH )
      depth = MAX_DEPTH;
     else if ( depth < 0 )
      depth = 0;

     if ( m_pRootNode != NULL )
      reset();

     //1. Create root node
     m_pRootNode = new OdGiExtentsSpaceNode<E,O>(NULL, extents, 0, nTypeOfObjects);
     m_Nodes.push_back(m_pRootNode);

     //2. Call recursive method for making the tree
     constructChilds(m_pRootNode, NUM_AXIS, depth, nTypeOfObjects); 
  }

  // reset the tree
  void reset()
  {
    m_pRootNode = NULL;

    OD_TYPENAME OdList<OdGiExtentsSpaceNode<E,O>*>::iterator it = m_Nodes.begin();
    OD_TYPENAME OdList<OdGiExtentsSpaceNode<E,O>*>::iterator itEnd = m_Nodes.end();
    while ( it != itEnd )
    {
      OdGiExtentsSpaceNode<E,O>* pNode = *it;
      delete pNode;
      ++it;
    }

    m_Leaves.clear();
    m_Nodes.clear();

    m_theSameObjects.clear();
    m_theIntersectedLeaves.clear();
    m_theCandidateLeaves.clear();
  }

  // set that the tree should be adaptive or not
  void setAdaptive(bool bAdaptive) {m_bIsAdaptive = bAdaptive;}

  // set the maximal node objects (used only if 'm_bIsAdaptive' = true)
  void setMaxNodeObjects(OdUInt64 nObjects) { m_iMaxNodeObjects = nObjects <= 0 ? m_iMaxNodeObjects : nObjects; }

  //process object
  O* processObject(O* pObject, int iObjectType, bool bCheckTheSame = false, const OdGeTol& tol = OdGeContext::gTol)
  {
    m_theSameObjects.clear();
    m_theCandidateLeaves.clear();

    if ( m_pRootNode == NULL )
      return NULL;

    nodeProcessObject(m_pRootNode, pObject, iObjectType, bCheckTheSame, tol);
  
    if ( m_theSameObjects.size() == 0 )
    {
      if ( !m_theCandidateLeaves.empty() )
      {
        OD_TYPENAME OdList<OdGiExtentsSpaceNode<E,O>*>::iterator itNodes = m_theCandidateLeaves.begin();
        OD_TYPENAME OdList<OdGiExtentsSpaceNode<E,O>*>::iterator itNodesEnd = m_theCandidateLeaves.end();
        while ( itNodes != itNodesEnd )
        {
          OdGiExtentsSpaceNode<E, O>* pNode = *itNodes;
          if (pNode)
          {
            nodeProcessObject(pNode, pObject, iObjectType, false, tol);
          }
          ++itNodes;
        }
      }
      m_theCandidateLeaves.clear();
      return NULL;
    }

    m_theCandidateLeaves.clear();

    return m_theSameObjects.front();
  }

  // retrieve the leaves nodes
  OdList<OdGiExtentsSpaceNode<E,O>*>* retrieveLeaves(){ return &m_Leaves;}

   //retrieve the extents of the root node
  bool getRootExtents(E& exts)
  {
    if ( m_pRootNode == NULL )
      return false;

    if(m_pRootNode->m_extents.isValidExtents())
    {
      exts = m_pRootNode->m_extents;
      return true;
    }
    return false;
  }

  OdList<OdGiExtentsSpaceNode<E,O>*>* getExtentsLeaves(E& ext)
  {
    m_theIntersectedLeaves.clear();

    if ( m_pRootNode == NULL )
      return NULL;

    nodeProcessExtent(m_pRootNode, ext);
  
    if ( m_theIntersectedLeaves.size() == 0 )
      return NULL;

    return &m_theIntersectedLeaves;
  }

  OdList<OdGiExtentsSpaceNode<E,O>*>* getPointLeaves(const P& pt)
  {
    m_theIntersectedLeaves.clear();

    if ( m_pRootNode == NULL )
      return NULL;

    nodeProcessPoint(m_pRootNode, pt);
  
    if ( m_theIntersectedLeaves.size() == 0 )
      return NULL;

    return &m_theIntersectedLeaves;
  }

private:

  // internal recursive method for building the tree
  void constructChilds(OdGiExtentsSpaceNode<E,O>* pParentNode, int axislevel, int curDepth, int nTypeOfObjects)
  {
    if ( pParentNode == NULL )
      return;

    if ( curDepth == 0 )
    {
      // means we have a leave
      m_Leaves.push_back(pParentNode);
      return;
    }

    //define the left part extents
    E     leftChildExtents;

    P leftBounPoint = pParentNode->m_extents.maxPoint();
    switch ( axislevel )
    {
    case 3:
      middlez(pParentNode->m_extents.maxPoint(), pParentNode->m_extents.minPoint(), leftBounPoint);
      break;
    case 2:
      leftBounPoint.x = (pParentNode->m_extents.maxPoint().x + pParentNode->m_extents.minPoint().x)/2.;
      break;
    case 1:
      leftBounPoint.y = (pParentNode->m_extents.maxPoint().y + pParentNode->m_extents.minPoint().y)/2.;
      break;
    }
  
    leftChildExtents.set(pParentNode->m_extents.minPoint(), leftBounPoint);

    // create left child
    pParentNode->m_pLeftChild = new OdGiExtentsSpaceNode<E,O>(pParentNode, leftChildExtents, axislevel > 1 ? pParentNode->m_iDepth : pParentNode->m_iDepth + 1, nTypeOfObjects);
    m_Nodes.push_back(pParentNode->m_pLeftChild);

    // go deeper
    if ( axislevel > 1 )
      constructChilds(pParentNode->m_pLeftChild, axislevel-1, curDepth, nTypeOfObjects);
    else
      constructChilds(pParentNode->m_pLeftChild, NUM_AXIS, curDepth-1, nTypeOfObjects);

    //define the right part extents
    E     rightChildExtents;

    P rightBounPoint = pParentNode->m_extents.minPoint();
    switch ( axislevel )
    {
    case 3:
      middlez(pParentNode->m_extents.maxPoint(), pParentNode->m_extents.minPoint(), rightBounPoint);
      break;
    case 2:
      rightBounPoint.x = (pParentNode->m_extents.maxPoint().x + pParentNode->m_extents.minPoint().x)/2.;
      break;
    case 1:
      rightBounPoint.y = (pParentNode->m_extents.maxPoint().y + pParentNode->m_extents.minPoint().y)/2.;
      break;
    }
  
    rightChildExtents.set(rightBounPoint, pParentNode->m_extents.maxPoint());

    // create right child
    pParentNode->m_pRightChild = new OdGiExtentsSpaceNode<E,O>(pParentNode, rightChildExtents, axislevel > 1 ? pParentNode->m_iDepth : pParentNode->m_iDepth + 1, nTypeOfObjects);
    m_Nodes.push_back(pParentNode->m_pRightChild);

    // go deeper
    if ( axislevel > 1 )
      constructChilds(pParentNode->m_pRightChild, axislevel-1, curDepth, nTypeOfObjects);
    else
      constructChilds(pParentNode->m_pRightChild, NUM_AXIS, curDepth-1, nTypeOfObjects);

    return;
  }

  // internal recursive method for process objects
  void nodeProcessObject(OdGiExtentsSpaceNode<E,O>* pNode, O* pObject, int iObjectType, bool bCheckTheSame, const OdGeTol& tol = OdGeContext::gTol)
  {
    if ( pNode == NULL || pObject == NULL )
      return;

    bool bIntersect = pObject->isInExtents(pNode->m_extents);

    if ( bIntersect )
    {
      if ( pNode->isLeave() )
      {
        if ( m_pCallback )
        {
          if ( !m_pCallback->notifyObjectPlacedAtNode(pObject, iObjectType, pNode) )
            return;
        }

        OdVector<O*, OdMemoryAllocator<O*> >* pNodeObjects = pNode->getObjectPointersPtr(iObjectType);
        if ( pNodeObjects == NULL ) // initialize object list if need
        {
          if ( m_iMaxNodeObjects > 0 )
            pNodeObjects = pNode->initObjectList(iObjectType, m_iMaxNodeObjects+1);
          else
            pNodeObjects = pNode->initObjectList(iObjectType, 50);
        }

        if ( pNodeObjects )
        {
          if ( bCheckTheSame ) // check that we already have the same object
          {
            OD_TYPENAME OdVector<O*, OdMemoryAllocator<O*> >::iterator itNodes = pNodeObjects->begin();
            OD_TYPENAME OdVector<O*, OdMemoryAllocator<O*> >::iterator itNodesEnd = pNodeObjects->end();
            while ( itNodes != itNodesEnd )
            {
              O* pObjectInList = *itNodes;
              if ( pObjectInList )
              {
                if ( pObjectInList->isEqual(pObject, tol) )
                {
                  m_theSameObjects.push_back(pObjectInList);
                  return;
                }
              }
              ++itNodes;
            }
            m_theCandidateLeaves.push_back(pNode);
          }
          else // simply add
          {
            pNodeObjects->push_back( pObject );
          }
        }

        //adaptive case
        if ( m_bIsAdaptive && pNodeObjects->size() > m_iMaxNodeObjects && pNode->m_iDepth <= MAX_DEPTH )
        {
          // remove current node from leaves
          m_Leaves.remove(pNode);

          //try to make additional split
          constructChilds(pNode, NUM_AXIS, 1/*one step deeper*/, pNode->m_iObjectsTypes);

          //put down the objects
          for ( int i = 0; i < pNode->m_iObjectsTypes; i++)
          {
            pNodeObjects = pNode->getObjectPointersPtr(i);
            if ( pNodeObjects )
            {
              OD_TYPENAME OdVector<O*, OdMemoryAllocator<O*> >::iterator it = pNodeObjects->begin();
              OD_TYPENAME OdVector<O*, OdMemoryAllocator<O*> >::iterator itEnd = pNodeObjects->end();
              while ( it != itEnd )
              {
                O* pObjectInList = *it;
               
                if ( pObjectInList )
                {
                  nodeProcessObject(pNode->m_pLeftChild, pObjectInList, i, false, tol);
                  nodeProcessObject(pNode->m_pRightChild, pObjectInList, i, false, tol);
                }
            
                ++it;
              }
            }
          } //eo for...
          //remove information about the objects in the nodes
          pNode->releaseObjectsStore();
        }
      }
      else
      {
        nodeProcessObject(pNode->m_pLeftChild, pObject, iObjectType, bCheckTheSame, tol);
        nodeProcessObject(pNode->m_pRightChild, pObject, iObjectType, bCheckTheSame, tol);
      }
    }
  
    return;
  }

  // internal recursive method for process extents
  void nodeProcessExtent(OdGiExtentsSpaceNode<E,O>* pNode, E& ext)
  {
    if ( pNode == NULL || !ext.isValidExtents() )
      return;

    E isect;
    ext.intersectWith(pNode->m_extents, &isect);

    if ( isect.isValidExtents() )
    {
      if ( pNode->isLeave() )
      {
        m_theIntersectedLeaves.push_back(pNode);
      }
      else
      {
        nodeProcessExtent(pNode->m_pLeftChild, ext);
        nodeProcessExtent(pNode->m_pRightChild, ext);
      }
    }
  
    return;
  }

  // internal recursive method for process point
  void nodeProcessPoint(OdGiExtentsSpaceNode<E,O>* pNode, const P& pt)
  {
    if ( pNode == NULL )
      return;

    if ( pNode->m_extents.contains(pt) )
    {
      if ( pNode->isLeave() )
      {
        m_theIntersectedLeaves.push_back(pNode);
      }
      else
      {
        nodeProcessPoint(pNode->m_pLeftChild, pt);
        nodeProcessPoint(pNode->m_pRightChild, pt);
      }
    }
  
    return;
  }

  void middlez(const OdGePoint2d& pt1, const OdGePoint2d& pt2, OdGePoint2d& ptRes)
  {
    return;
  }


  void middlez(const OdGePoint3d& pt1, const OdGePoint3d& pt2, OdGePoint3d& ptRes)
  {
    ptRes.z = (pt1.z + pt2.z)/2.;
    return;
  }
};

typedef OdGiExtentsSpaceTree<2, 10, 10, OdGeExtents2d, OdGePoint2d, OdGiExtentsSpaceObject> OdGiExtents2dSpaceTree;
typedef OdGiExtentsSpaceTree<3, 5, 20, OdGeExtents3d, OdGePoint3d, OdGiExtentsSpaceObject>  OdGiExtents3dSpaceTree;


/** \details
  .

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/

class OdGiExtents3dSpacePoint : public OdGiExtentsSpaceObject
{
  friend class OdGiExtents3dSpaceEdge;

  // set of the IDs of edges which have this vertex
  std::set<OdUInt64> m_edges;

  // set of the IDs of invisible edges which have this vertex
  std::set<OdUInt64> m_invisibleedges;

  // need for constructing the chain polylines
  bool        m_bVisited;

public:

  //point
  OdGePoint3d  m_pt;

  //constructor
  OdGiExtents3dSpacePoint(const OdGePoint3d& pt, OdUInt64 uniqueID) : OdGiExtentsSpaceObject(uniqueID)
  {
    m_pt.set(pt.x, pt.y, pt.z);
    m_bVisited = false;
  }

  ~OdGiExtents3dSpacePoint()
  {
    m_edges.clear();
    m_invisibleedges.clear();
  }

  void addEdge(OdUInt64 ID)
  {
    m_edges.insert(ID);
  }

  void addInvisible(OdUInt64 ID)
  {
    m_invisibleedges.insert(ID);
  }

  void removeInvisible(OdUInt64 ID)
  {
    m_invisibleedges.erase(ID);
  }

  // check that the edge is in extents (currently not used)
  bool isInExtents(OdGeExtents3d& extents) const
  {
    OdGeTol  tol; 
    if (extents.contains( m_pt, tol ) )
      return true;

    return false;
  }

  bool isInExtents(OdGeExtents2d& extents) const {return false;}

  // check that objects are equal
  bool isEqual(OdGiExtentsSpaceObject* pObject, const OdGeTol& tol = OdGeContext::gTol) const
  {
    OdGiExtents3dSpacePoint* pObjectVertex = dynamic_cast<OdGiExtents3dSpacePoint*>(pObject);
    
    if ( pObjectVertex == NULL )
      return false;

    if ( pObjectVertex->m_pt.isEqualTo(m_pt, tol) )
      return true;

    return false;
  }

  OdUInt64 getPower()
  {
    return m_edges.size();
  }

  const std::set<OdUInt64>* getEdges(){return &m_edges;}
  const std::set<OdUInt64>* getInvisilbeEdges(){return & m_invisibleedges;}

  void setVisited(bool bVisit){m_bVisited = bVisit;}
  bool isVisited(){return m_bVisited;}

};

/** \details
  .

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/

class OdGiExtents3dSpaceEdge : public OdGiExtentsSpaceObject
{

public:
  //two vertices
  OdUInt64    m_iVert1;
  OdUInt64    m_iVert2;
  bool        m_bVisited; // need for constructing the pathes
  bool        m_bIsVisible; // means that the edge is not visible
  
  //constructor
  OdGiExtents3dSpaceEdge(OdGiExtents3dSpacePoint& pt1, OdGiExtents3dSpacePoint& pt2, OdInt64 uniqueID) : OdGiExtentsSpaceObject(uniqueID)
  {
    m_iVert1 = pt1.getID();
    m_iVert2 = pt2.getID();

    if ( uniqueID >= 0 )
    {
      pt1.m_edges.insert(uniqueID);
      pt2.m_edges.insert(uniqueID);
    }

    m_bVisited = false;
    m_bIsVisible = true;
  }

  void setVisited(bool bVisit){m_bVisited = bVisit;}
  bool isVisited(){return m_bVisited;}

  OdUInt64 getSecondVertex(OdUInt64 iDfirst)
  {
    if ( iDfirst == m_iVert1 )
      return m_iVert2;

    return m_iVert1;
  }

  bool isInExtents(OdGeExtents3d& extents) const {return false;}
  bool isInExtents(OdGeExtents2d& extents) const {return false;}
  bool isEqual(OdGiExtentsSpaceObject* pObject, const OdGeTol& tol = OdGeContext::gTol) const{return false;}
};

/** \details
  .

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/

class OdGiExtents3dSpaceChainPolyline : public OdGiExtentsSpaceObject
{
protected:

  OdList<OdGiExtents3dSpacePoint*> vertices;

public:

  //constructor
  OdGiExtents3dSpaceChainPolyline(OdInt64 uniqueID) : OdGiExtentsSpaceObject(uniqueID){}

  ~OdGiExtents3dSpaceChainPolyline(){vertices.clear();}

  virtual void addVertex(OdGiExtents3dSpacePoint* pVertex)
  {
    vertices.push_back(pVertex);
  }

  OdUInt64 getNumberOfVertices() {return vertices.size();}
  OdUInt64 getNumberOfVertices_closed()
  {
    OdGiExtents3dSpacePoint* ptFirst = vertices.front();
    OdGiExtents3dSpacePoint* ptSecond = vertices.back();
    if ( ptFirst == ptSecond && vertices.size() > 1)
      return vertices.size()-1;

    return vertices.size();
  }

  void getPoints(OdGePoint3d *pPoints)
  {
    if ( pPoints == NULL )
      return;

    OdList<OdGiExtents3dSpacePoint*>::iterator it = vertices.begin();
    OdList<OdGiExtents3dSpacePoint*>::iterator itEnd = vertices.end();
    OdUInt32 ind = 0;
    while ( it != itEnd )
    {
       OdGiExtents3dSpacePoint* pVertex = *it;
       if ( pVertex )
       {
         pPoints[ind].set(pVertex->m_pt.x, pVertex->m_pt.y, pVertex->m_pt.z); 
         ind++;
       }
       ++it;
    }
  }

  void getPoints_closed(OdGePoint3d *pPoints)
  {
    if ( pPoints == NULL )
      return;

    OdList<OdGiExtents3dSpacePoint*>::iterator it = vertices.begin();
    OdList<OdGiExtents3dSpacePoint*>::iterator itEnd = vertices.end();

    OdGiExtents3dSpacePoint* ptFirst = vertices.front();
    OdGiExtents3dSpacePoint* ptSecond = vertices.back();
    if ( ptFirst == ptSecond && vertices.size() > 1 )
      --itEnd;

    OdUInt32 ind = 0;
    while ( it != itEnd )
    {
       OdGiExtents3dSpacePoint* pVertex = *it;
       if ( pVertex )
       {
         pPoints[ind].set(pVertex->m_pt.x, pVertex->m_pt.y, pVertex->m_pt.z); 
         ind++;
       }
       ++it;
    }
  }

  void getPoints(OdGePoint3dVector& pts)
  {
    getPoints(pts.asArrayPtr());
  }

  void getPoints_closed(OdGePoint3dVector& pts)
  {
    getPoints_closed(pts.asArrayPtr());
  }

  bool isInExtents(OdGeExtents3d& extents) const {return false;}
  bool isInExtents(OdGeExtents2d& extents) const {return false;}
  bool isEqual(OdGiExtentsSpaceObject* pObject, const OdGeTol& tol = OdGeContext::gTol) const{return false;}
};


class OdGiExtents3dSpaceUtils
{
 // next start vertex for finding the path
 static OdGiExtents3dSpacePoint* getNextStartVertex(OdArray<OdGiExtents3dSpacePoint*>& points, OdList<OdGiExtents3dSpacePoint*>& leafs)
 {
   OdGiExtents3dSpacePoint* pStartVertex = NULL;
   
   //1. Try to find leafs
   if ( leafs.size() > 0 )
   {
      OdList<OdGiExtents3dSpacePoint*>::iterator itLeafs = leafs.begin();
      OdList<OdGiExtents3dSpacePoint*>::iterator itLeafsEnd = leafs.end();
      while ( itLeafs != itLeafsEnd )
      {
        OdGiExtents3dSpacePoint* pLeaf = *itLeafs;
        if ( pLeaf && !pLeaf->isVisited() )
        {
          pStartVertex = pLeaf;
          break;
        }
        ++itLeafs;
      }
   }

   //2. Try to find first not visited vertex
   if ( pStartVertex == NULL )
   {
     int iVertNumber = points.size();
     for (OdUInt64 i = 0; i < iVertNumber; i++)
     {
       OdGiExtents3dSpacePoint* pVert = points[i];
       if ( pVert && !pVert->isVisited() )
       {
         pStartVertex = pVert;
         break;
       }
     }
   }

   return pStartVertex;
 }

template <class E>
static OdGiExtents3dSpaceEdge* getFirstUnvisitedEdge(OdGiExtents3dSpacePoint* pVertex, OdArray<E*>& edges)
{
  if ( pVertex == NULL )
    return NULL;

  const std::set<OdUInt64>* pEdges = pVertex->getEdges();
  if ( pEdges == NULL )
    return NULL;

  std::set<OdUInt64>::const_iterator itEdge = pEdges->begin();
  std::set<OdUInt64>::const_iterator itEdgeEnd = pEdges->end();
  while ( itEdge != itEdgeEnd )
  {
    OdUInt64 edgeID = *itEdge;
    if ( edgeID < edges.size() )
    {
      OdGiExtents3dSpaceEdge* pEdge = dynamic_cast<OdGiExtents3dSpaceEdge*>(edges[edgeID]);
      if ( pEdge && !pEdge->isVisited() && pEdge->m_bIsVisible )
        return pEdge;
    }
   
    ++itEdge;
  }

  return NULL;
}

 // construct a path from start vertex
template <class T, class E>
static void constructPath(OdGiExtents3dSpacePoint* pStartVertex, OdArray<OdGiExtents3dSpacePoint*>& points,
                   OdArray<E*>& edges, OdList<T*>& polylines)
{
  if ( pStartVertex == NULL )
    return;
  
  OdGiExtents3dSpacePoint* pFirstVertex = pStartVertex;
 
  T *pPath = NULL;
  //loop
  while(pFirstVertex)
  { 
    //get unvisited edge
    OdGiExtents3dSpaceEdge* pFirstEdge = getFirstUnvisitedEdge(pFirstVertex, edges);
    if ( pFirstEdge == NULL )
    {
      pFirstVertex->setVisited(true);
      break;
    }

    // get the second vertex of the edge
    OdGiExtents3dSpacePoint* pNextVertex = points[pFirstEdge->getSecondVertex(pFirstVertex->getID())];
  
    //init the path
    if ( pPath == NULL )
    {
      pPath = new T(0);
      pPath->addVertex(pFirstVertex);
    }
    pPath->addVertex(pNextVertex);
    
    pFirstVertex->setVisited(true);
    pNextVertex->setVisited(true);
    pFirstEdge->setVisited(true);
  
    pFirstVertex = pNextVertex;
  }

  if ( pPath )
    polylines.push_back(pPath);

  return;
}

public:
template <class T, class E>
static OdInt64 calculateChainPolylinesFromEdges(OdArray<OdGiExtents3dSpacePoint*>& points, OdArray<E*>& edges, OdList<T*>& polylines )
{
  //1. Find the vertices with power 1 if exists
   OdList<OdGiExtents3dSpacePoint*> leafVertices;
   int iVertNumber = points.size();
   for (OdUInt64 i = 0; i < iVertNumber; i++)
   {
     OdGiExtents3dSpacePoint* pVert = points[i];
     if ( pVert )
     {
       OdUInt64 vPower = pVert->getPower();
       if ( vPower == 1 )
         leafVertices.push_back(pVert);
       else if ( vPower == 0 )
         pVert->setVisited(true);
     }
   }

  //2.  run loop for construction the pathes
  OdGiExtents3dSpacePoint* pStartVertex = getNextStartVertex(points, leafVertices);
  while ( pStartVertex )
  {
    constructPath(pStartVertex, points, edges, polylines);
    pStartVertex = getNextStartVertex(points, leafVertices);
  }

  return polylines.size();
}
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIEXTENTS3DSPACETREE_H__
