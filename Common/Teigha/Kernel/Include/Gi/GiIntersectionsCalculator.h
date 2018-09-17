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

#ifndef __ODGIINTERSECTIONSCALCULATOR__
#define __ODGIINTERSECTIONSCALCULATOR__

#include "TD_PackPush.h"
#include "Ge/GeExtents3d.h"
#include "Gi/GiExtentsSpaceTree.h"

/** \details
  .
  Library: Source code provided.
*/
class OdGiTriangleForIntersectTest
{
  friend class OdGiIntersectionsCalculator;
  friend class OdGiCollisionDetector;
  friend class OdGiCollisionDetectorIntersectionsOnly;

  // unique ID of the object
  //IMPORTANT: the user is responsible for the uniqueness of the ID
  OdUInt64    ID;

  // extents of the triangle
  OdGeExtents3d       m_extents;

  // parameters of the triangle's plane
  OdGeVector3d        planeNorm;
  double              planeD;
  bool                m_bIsPlaneCalculated;

public:
  //three vertices
  OdGePoint3d         ABC[3];

  // default constructor
  OdGiTriangleForIntersectTest();

  //constructor
  OdGiTriangleForIntersectTest(const OdGePoint3d *points, OdUInt64 uniqueID);

  //set data
  void setData(const OdGePoint3d *points);

  // retrieve the plane's normal
  inline const OdGeVector3d*  getTrianglePlaneNorm();

  //retrieve the D
  inline double getPlaneD();

  //**********************************************************************//
  //********************METHODS FOR USING IN SPACE TREE*******************//
  //**********************************************************************//
  //set/get ID
  OdUInt64  getID() {return ID;}
  void      setID(OdUInt64 uniqueID){ID = uniqueID;}

  // check that the triangle is in extents
  inline bool isInExtents(OdGeExtents3d& extents) const;

  // not need for this class
  bool isInExtents(OdGeExtents2d& extents) const {return false;}

  // check that objects are equal
  bool isEqual(OdGiTriangleForIntersectTest* pObject, const OdGeTol& tol = OdGeContext::gTol) const
  {
    return false;
  }

private:
  
  // calculate the parameters of the plane
  inline void calculatePlaneParameters();
};

typedef OdVector<OdGiTriangleForIntersectTest, OdMemoryAllocator<OdGiTriangleForIntersectTest> > OdGiIntersectTrianglesVector;
typedef OdGiExtentsSpaceTree<3, 5, 20, OdGeExtents3d, OdGePoint3d, OdGiTriangleForIntersectTest>  OdGiExtents3dSpaceTree_Gi;
typedef OdGiExtentsSpaceNode<OdGeExtents3d, OdGiTriangleForIntersectTest> OdGiExtents3dSpaceNode_Gi;

/** \details
  .

  Library: Source code provided.
*/
class OdGiEdgeForIntersectTest : public OdGiExtents3dSpaceEdge
{
  public:  
  //means that this edge is an edge of touching of two triangles
  bool        m_bIsTouchingEdge;

  OdGiEdgeForIntersectTest(OdGiExtents3dSpacePoint& pt1, OdGiExtents3dSpacePoint& pt2, OdInt64 uniqueID) : OdGiExtents3dSpaceEdge(pt1, pt2, uniqueID)
  {
    m_bIsTouchingEdge = false;
  }
};

/** \details
  This class implements triangles-based intersection calculation.
  Usage:
  1) call initializeCalculations()
  2) add triangles:
    2.1) call appendTriangleContainer to obtain containerID
    2.2) call addTriangleToContainer with obtained ID
  3) specify containers that should be tested by calling addContainerToBeTested and call processPathCalculation for them
  4) call computeIntersections to calculate intersections between specified container and all "should be tested" containers
  5) use processPathCalculation() to obtain results

  Library: Source code provided.

  <group OdGi_Classes>
*/
class OdGiIntersectionsCalculator : public OdGiExtentsSpaceTreeCallback<OdGeExtents3d, OdGiTriangleForIntersectTest>
{
  protected:
    // tolerance for the intersection calculation
    OdGeTol m_tol;

    // cach data for the controlling already intersected triangles with the given triangle
    OdVector<bool, OdMemoryAllocator<bool> >                m_vectTestedTriangles;            
    OdUInt64Vector                                          m_vectTestTriangles_objBoundary;

    // data for controlling the objects with which will be intersected the given object
    OdUInt64Vector                                          m_vectToBeTestedObjects;

    // following parameter need for the callback method' notifyObjectPlacedAtNode'
    bool                                                    m_bSecondObjectProcessedInTree;

    // cach data for storing the triangles from all objects
    OdVector<OdGiIntersectTrianglesVector*>                 m_containerTriangles;
        
    // extents trees for the triangles and vertices
    OdGiExtents3dSpaceTree_Gi                               m_spaceTriangTree;
    OdGiExtents3dSpaceTree                                  m_spaceVertexTree;

    // arrays for storing the intersecting vertices and edges (this arrays will have the same size as 'm_vectToBeTestedObjects')
    OdArray<OdGiExtents3dSpacePoint*>*                      m_intersectVertices;
    OdArray<OdGiEdgeForIntersectTest*>*                     m_intersectEdges;

    // list for the storing final pathes for polyout
    OdList<OdGiExtents3dSpaceChainPolyline*>                m_finalIntersectPathes;
    bool                                                    m_b3dFacesActive;

    // method for the callback from the tree
    virtual bool notifyObjectPlacedAtNode(OdGiTriangleForIntersectTest* pTriang, int objectType, OdGiExtents3dSpaceNode_Gi* pNode);

    // calculate intersections of the triangles
    void intersectTriangles(OdUInt64 idObjA, OdUInt64 trIdA, OdUInt64 idObjB, OdUInt64 trIdB, OdUInt64 idObjTested, const OdGeTol& tol);

    //Public API
  public:
    OdGiIntersectionsCalculator();

    virtual ~OdGiIntersectionsCalculator(){}
    /** Appends specified triangle container to the containers list and returns it ID
    */
    OdInt64 appendTriangleContainer( OdGiIntersectTrianglesVector* pContainer );
    /** Adds triangle to the specified triangle container
    */
    OdInt64 addTriangleToContainer( OdInt64 containerID, const OdGePoint3d *pTriangle );
    /** Adds specified triangle container to the "containers to be tested" list
    */
    OdInt64 addContainerToBeTested( OdInt64 containerID );
    /** Computes intersections between specified triangle container and "containers to be tested" list
    */
    void computeIntersections( OdInt64 containerID, const OdGeExtents3d &extents );
    // initialize calculations
    virtual void initializeCalculations(OdGeExtents3d& ext, OdInt64 nObjects);
    // finalize calculations
    virtual void finalizeCalculations();
    // intermediate finalize calculations
    void intermediatefinalizeCalculations();

    //process triangles into tree
    void processTrianglesIntoSpaceTree(OdInt64 objID, bool bOtherObjectsProcessed);
    //process path calculation
    void processPathCalculation(OdUInt64 idObjTested);

  public:
    //Data accessors
    bool& facesActive() { return m_b3dFacesActive; }

    bool getSpaceTreeRootExtents( OdGeExtents3d& ext );

    OdUInt32 triangleContainerSize() const { return m_containerTriangles.size(); }

    OdUInt32 containerToBeTestedSize() const { return m_vectToBeTestedObjects.size(); }

    void removeEdgeInfo( OdInt64 objID );

    OdList<OdGiExtents3dSpaceChainPolyline*>& finalIntersectionPathes() { return m_finalIntersectPathes; }
    OdArray<OdGiExtents3dSpacePoint*>*  intersectionVertices() { return m_intersectVertices; }
    OdArray<OdGiEdgeForIntersectTest*>* intersectionEdges(){ return m_intersectEdges; }
};

class OdGiCollisionDetector : public OdGiIntersectionsCalculator
{
public:
  OdGiCollisionDetector();
  virtual ~OdGiCollisionDetector() {};

protected:
   // method for the callback from the tree
  virtual bool notifyObjectPlacedAtNode(OdGiTriangleForIntersectTest* pTriang, int objectType, OdGiExtents3dSpaceNode_Gi* pNode);

protected: //Hide OdGiIntersectionsCalculator methods
  void computeIntersections( OdInt64 containerID, const OdGeExtents3d &extents ) { OdGiIntersectionsCalculator::computeIntersections( containerID, extents ); }
  void processPathCalculation(OdUInt64 idObjTested) { OdGiIntersectionsCalculator::processPathCalculation( idObjTested ); }
  void removeEdgeInfo( OdInt64 objID ) { OdGiIntersectionsCalculator::removeEdgeInfo( objID ); }
  OdList<OdGiExtents3dSpaceChainPolyline*>& finalIntersectionPathes() { return m_finalIntersectPathes; }
  OdArray<OdGiExtents3dSpacePoint*>*  intersectionVertices() { return m_intersectVertices; }
  OdArray<OdGiEdgeForIntersectTest*>* intersectionEdges(){ return m_intersectEdges; }

public:
  void detectCollisions( OdInt64 containerID, const OdGeExtents3d &extents );
  void getCollisions( OdList< OdInt64 >& result );

  // finalize calculations
  virtual void finalizeCalculations();

  void clearContainersToBeTested();

protected:
  OdVector<bool, OdMemoryAllocator<bool> >                m_vectCollidedContainers;
  //Performance
  bool*                                                   m_vectCollidedContainersAsPtr;
  OdGePoint3d   m_ptA;
  OdGePoint3d   m_ptB;
};

//Collision detector that detect only intersections (no touching)
//It is separate class because additional checks in OdGiCollisionDetector::collideTriangles will slow down it
class OdGiCollisionDetectorIntersectionsOnly : public OdGiCollisionDetector
{
public:
  OdGiCollisionDetectorIntersectionsOnly() : OdGiCollisionDetector()
  {};
  virtual ~OdGiCollisionDetectorIntersectionsOnly()
  {};
protected:
   // method for the callback from the tree
  virtual bool notifyObjectPlacedAtNode(OdGiTriangleForIntersectTest* pTriang, int objectType, OdGiExtents3dSpaceNode_Gi* pNode);

  bool collideTriangles(OdUInt64 idObjA, OdUInt64 trIdA, OdUInt64 idObjB, OdUInt64 trIdB, OdUInt64 idObjTested, const OdGeTol& tol);
};


#include "TD_PackPop.h"

#endif //#ifndef __ODGIINTERSECTIONSCALCULATOR__
