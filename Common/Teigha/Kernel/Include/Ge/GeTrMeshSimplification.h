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

#ifndef OD_GETRSIMPLIFICATION_H
#define OD_GETRSIMPLIFICATION_H


#include "OdPlatformSettings.h"
#include "GePoint3dArray.h"
#include "GeExtents3d.h"
#include "Ge/GePoint2dArray.h"
#include <set>
#include <vector>

namespace GeMesh
{
typedef std::pair<int, int> intPair;

struct int3
{
  int3() { val[0] = val[1] = val[2] = 0; }
  void set(int a) { val[0] = val[1] = val[2] = a; }
  int operator [](unsigned int i) const
  {
    return val[i];
  }
  int& operator [](unsigned int i)
  {
    return val[i];
  }
  int val[3];
};

struct GE_TOOLKIT_EXPORT OdGeTr
{
  OdGeTr() : tagFace(-1) { nb.set(-1); tagEdge.set(-1); }
  void replaceNb(int nbOld, int nbNew);

  int3 tr;
  int3 nb;
  int tagFace;
  int3 tagEdge;
};

class GE_TOOLKIT_EXPORT OdGeMesh
{
public:
  OdGeExtents3d getExtents() const;
  virtual ~OdGeMesh() {}

  virtual double distanceTo(const OdGePoint3d& pt, OdGePoint3d& ptClosest, bool bPrecise) const = 0;
  virtual int checkMeshMismatch(const OdGeMesh& mesh, OdGePoint3dArray& aPtMismatch, double tol) const = 0;
  
public:
  OdGePoint3dArray m_aVx;
  OdArray<int> m_aVxTag;
};

class GE_TOOLKIT_EXPORT OdGeTrMesh : public OdGeMesh
{
public:
  virtual double distanceTo(const OdGePoint3d& pt, OdGePoint3d& ptClosest, bool bPrecise) const;
  virtual int checkMeshMismatch(const OdGeMesh& mesh, OdGePoint3dArray& aPtMismatch, double tol) const;

  OdGeVector3d trNormal(int t, double* area = NULL) const;
  OdGeVector3d vxNormal(int v) const;
  bool walkNextTr(int& iTr, int& w, bool dir) const;
  bool walkAroundVertex(int& iTr, int& w, int3& aux) const;
  int getCoEdge(int t, int e) const;
  int fillNbLinks();
  void removeDegenerateTriangles(double tol);
  void clear();
  void fillVxToTr();

  void dump(FILE* f) const;
public:
  OdArray<OdGeTr> m_aTr;

  // vector to assign vertices to triangles
  OdArray<OdArray<intPair > > m_vxToTr;
};

struct VertexPair
{
  VertexPair() : m_cost(1e300) {}
  VertexPair(int a, int b) { m_edge.first = a; m_edge.second = b; }
  // is this point present in pair?
  bool isPresent(int a) { return m_edge.first == a || m_edge.second == a; }
  // is equal?
  bool equal(VertexPair& pair) { return isPresent(pair.m_edge.first) && isPresent(pair.m_edge.second); }
  void replace(int a, int b);
  int nb(int a);

  // cost (error) of contraction this pair
  double m_cost;
  // indices of 2 vertices
  intPair m_edge;
  // position of new point (after contraction) which minimizes error
  OdGePoint3d m_newVx;
};

struct VertexPairKey
{
    double m_cost;
    int m_ind;
    VertexPairKey(double cost, int ind) {m_cost = cost; m_ind = ind;}
};

class GeSimplificationBase
{
protected:
    GeSimplificationBase();
    void reset(OdGePoint3dArray& vx);
    void computeErrorOnPair(int iPair);
    virtual bool computeQ(int iVt) = 0;

protected:
    // vertices
    OdGePoint3dArray* m_pVx;
    // tolerance ^2
    double m_eps2;
    // max edge length
    double m_maxEdLen2;
    // quadrics to approximate error at each vertex
    OdArray<OdGeMatrix3d> m_Q;
    // contraction candidates
    OdArray<VertexPair> m_pair;
    // vector to assign vertices to pairs
    OdArray<OdArray<int> > m_vxToPair;
    // map to keep pairs sorted by cost
    std::set<VertexPairKey> m_pairMap;
};

class GE_TOOLKIT_EXPORT GeTrngSimplification : public GeSimplificationBase
{
public:
    GeTrngSimplification();
    void reset(OdGeTrMesh& trng);
    void algo(double simpPercent);

private:
    virtual bool computeQ(int iVt);
    bool isEdgeValid(int t, int v);
    bool checkEdge(int t, int v);
    bool checkOverlap(int v1, int v2);
    int selectPairs();
    int contractPair();
    void removeGarbage();
#ifdef _DEBUG
    bool checkVxToTr(int iVt);
#endif

private:
    // mesh to simplificate
    OdGeTrMesh* m_pTrng;
    // determines the vertex's fan type (0 - closed, 1 - open, 2 - corner)
    OdArray<int> m_fanType;
    // vector to assign vertices to triangles
    //OdArray<OdArray<intPair > > m_vxToTr;
};

GE_TOOLKIT_EXPORT bool triangulateProfile(const OdGePoint2dArray& vertexSource, const std::vector<OdInt32>& inFaceData, OdArray<OdInt32>& vecTriangles);
GE_TOOLKIT_EXPORT bool compareMeshes(const OdGeTrMesh& meshTemplate, const OdGeTrMesh& mesh, double tolCoef, bool bDebugOutput);
}

#endif // OD_GETRSIMPLIFICATION_H

