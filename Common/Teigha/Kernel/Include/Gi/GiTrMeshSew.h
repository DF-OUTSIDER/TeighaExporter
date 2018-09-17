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



#ifndef __OD_GI_TRMESHSEW__
#define __OD_GI_TRMESHSEW__

#include "Ge/GeDoubleArray.h"
#include "Gi/GiConveyorNode.h"
#include "Si/SiSpatialIndex.h"
#include "Ge/GeTrMeshSimplification.h"
#include "OdLinkedArray.h"

#include "TD_PackPush.h"

namespace GiMesh
{

struct TrVertex : OdSiEntity
{
public:
  const OdGePoint3d*  pt;
  int index;

  void set(const OdGePoint3d* _pt, int _index) { pt = _pt; index = _index; }

  bool extents(OdGeExtents3d& extents) const
  {
    extents.set(*pt, *pt);
    return true;
  }
};

struct TrEdge : OdSiEntity
{
public:
  const OdGePoint3d*  pt1;
  const OdGePoint3d*  pt2;
  int indexTr;
  int indexEdge;

  void set(const OdGePoint3d* _pt1, const OdGePoint3d* _pt2, int iTr, int iEd) { pt1 = _pt1; pt2 = _pt2; indexTr = iTr; indexEdge = iEd; }

  bool extents(OdGeExtents3d& extents) const
  {
    extents.set(*pt1, *pt1);
    extents.addPoint(*pt2);
    return true;
  }
};

class ODGI_EXPORT TrMeshSew
{
public:
  TrMeshSew(GeMesh::OdGeTrMesh* pMesh, double tol = 1e-3);
  void joinVertexes();
  void sewVertexes();
  void sewEdges();

public:
  GeMesh::OdGeTrMesh* m_pMesh;
  OdLinkedArray<TrVertex> m_aVertexSI;
  OdLinkedArray<TrEdge> m_aEdgeSI;
  OdSiSpatialIndexPtr m_pSpatialIndexVx;
  OdSiSpatialIndexPtr m_pSpatialIndexEdge;

  // vector to assign vertices to triangles
  //OdArray<OdArray<GeMesh::intPair > > m_vxToTr;

  double tolSew;
};

}

#include "TD_PackPop.h"

#endif // __OD_GI_TRMESHSEW__
