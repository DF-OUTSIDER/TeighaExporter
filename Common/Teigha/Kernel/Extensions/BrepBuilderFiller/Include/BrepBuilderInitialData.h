/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


#ifndef _BREPBUILDERINITIALDATA_INCLUDED_
#define _BREPBUILDERINITIALDATA_INCLUDED_

#include "BrepBuilder/BrepBuilder.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "CmColorBase.h"
#include "Gi/GiMaterial.h"

class OdBrEdge;
class OdBrFace;
class OdIMaterialAndColorHelper;

/** \details
    Corresponding C++ library: TD_BrepBuilderFiller
    <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialCoedge
{
  typedef OdGeCurve3dPtrArray::size_type EdgeIndex;
  EdgeIndex edgeIndex;
  OdBrepBuilder::EntityDirection direction;
  OdGeCurve2dPtr curve;

  BrepBuilderInitialCoedge()
    : curve(NULL)
    , direction(OdBrepBuilder::kForward)
    , edgeIndex(0)
  {}
  BrepBuilderInitialCoedge(
    OdGeCurve2dPtr curve,
    OdBrepBuilder::EntityDirection direction,
    BrepBuilderInitialCoedge::EdgeIndex edgeIndex
  )
    : curve(curve)
    , direction(direction)
    , edgeIndex(edgeIndex)
  {}
};
typedef OdArray<BrepBuilderInitialCoedge> BrepBuilderInitialCoedgeArray;

/** \details
    Corresponding C++ library: TD_BrepBuilderFiller
    <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialLoop
{
  BrepBuilderInitialCoedgeArray coedges;

  BrepBuilderInitialLoop() {}
  BrepBuilderInitialLoop(OdGeCurve2dPtr curve,
    BrepBuilderInitialCoedge::EdgeIndex edgeIndex,
    OdBrepBuilder::EntityDirection direction
  )
    : coedges(1)
  {
    coedges.append(BrepBuilderInitialCoedge(curve, direction, edgeIndex));
  }
};
typedef OdArray<BrepBuilderInitialLoop> BrepBuilderInitialLoopArray;

/** \details
    Corresponding C++ library: TD_BrepBuilderFiller
    <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialSurface
{
  OdGeSurfacePtr pSurf;
  OdBrepBuilder::EntityDirection direction;
  BrepBuilderInitialLoopArray loops;

  OdDbStub* material;
  bool hasMaterialMapping;
  OdGiMapper materialMapper;

  bool hasColor;
  OdCmEntityColor color;

  BrepBuilderInitialSurface()
    : pSurf(NULL)
    , direction(OdBrepBuilder::kForward)
    , material(NULL)
    , hasMaterialMapping(false)
    , hasColor(false)
    , color(OdCmEntityColor::kNone)
  {}

  OdResult setupVisualInfo(const OdBrFace& face, OdIMaterialAndColorHelper* pMaterialHelper);
};
typedef OdArray<BrepBuilderInitialSurface> BrepBuilderInitialSurfaceArray;

typedef OdArray<BrepBuilderInitialSurfaceArray> BrepBuilderShellsArray;

typedef OdArray<BrepBuilderShellsArray> BrepBuilderComplexArray;

/** \details
    Corresponding C++ library: TD_BrepBuilderFiller
    <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialEdge
{
  OdGeCurve3dPtr curve;
  bool hasColor;
  OdCmEntityColor color;

  BrepBuilderInitialEdge(OdGeCurve3dPtr edgeCurve = NULL)
    : curve(edgeCurve)
    , hasColor(false)
    , color(OdCmEntityColor::kNone)
  {}

  OdResult setupVisualInfo(const OdBrEdge& edge, OdIMaterialAndColorHelper* pMaterialHelper);
};
typedef OdArray<BrepBuilderInitialEdge> BrepBuilderInitialEdgeArray;

/** \details
Helper structure store data for initialize OdBrepBuilder.
    Corresponding C++ library: TD_BrepBuilderFiller
    <group !!RECORDS_tkernel_apiref>
*/
struct BrepBuilderInitialData
{
  BrepBuilderInitialEdgeArray edges;
  BrepBuilderComplexArray complexes;
};

#endif //_BREPBUILDERINITIALDATA_INCLUDED_
