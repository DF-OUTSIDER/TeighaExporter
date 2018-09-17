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




#ifndef _OD_DB_SUBDMESH
#define _OD_DB_SUBDMESH

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DoubleArray.h"
#include "DbSurface.h"
#include "Db3dSolid.h"
#include "CmEntityColorArray.h"

#include "Int32Array.h"
#include "Gi/GiGeometry.h"

class OdDbSurface;
class OdDb3dSolid;

/** \details
 This class represents SubDMesh entities in an OdDbDatabase instance.

 \sa
 TD_Db

 <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSubDMesh : public OdDbEntity
{
public:
 ODDB_DECLARE_MEMBERS(OdDbSubDMesh);
 
 OdDbSubDMesh();

 virtual bool subWorldDraw(
   OdGiWorldDraw* pWd) const;

 virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

 virtual OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver,
   OdDbObjectId& replaceId,
   bool& exchangeXData);

 virtual OdResult dwgInFields(
   OdDbDwgFiler* pFiler);

 virtual void dwgOutFields(
   OdDbDwgFiler* pFiler) const;

 virtual OdResult dxfInFields(
   OdDbDxfFiler* pFiler);

 virtual void dxfOutFields(
   OdDbDxfFiler* pFiler) const;

 virtual OdResult subTransformBy(
   const OdGeMatrix3d& xfm);

 OdResult subGetGeomExtents(
   OdGeExtents3d& extents) const;

 virtual OdResult subExplode(
   OdRxObjectPtrArray& entitySet) const;

 OdResult setSubDMesh (const OdGePoint3dArray& vertexArray,
   const OdInt32Array& faceArray,
   OdInt32 subDLevel);

 OdResult setSphere (double radius,
   OdInt32 divAxis,
   OdInt32 divHeight,
   OdInt32 subDLevel);

 OdResult setCylinder (double majorRadius,
   double minorRadius,
   double height,
   OdInt32 divAxis,
   OdInt32 divHeight,
   OdInt32 divCap,
   OdInt32 subDLevel);

 OdResult setCone (double majorRadius,
   double minorRadius,
   double height,
   OdInt32 divAxis,
   OdInt32 divHeight,
   OdInt32 divCap,
   double radiusRatio,
   OdInt32 subDLevel);

 OdResult setTorus (double majorRadius,
   OdInt32 divSection,
   OdInt32 divSweepPath,
   double sectionRadiusRatio,
   double sectionRotate,
   OdInt32 subDLevel);

 OdResult setBox (double xLen,
   double yLen,
   double zLen,
   OdInt32 divX,
   OdInt32 divY,
   OdInt32 divZ,
   OdInt32 subDLevel);

 OdResult setWedge (double xLen,
   double yLen,
   double zLen,
   OdInt32 divLength,
   OdInt32 divWidth,
   OdInt32 divHeight,
   OdInt32 divSlope,
   OdInt32 divCap,
   OdInt32 subDLevel);

 OdResult setPyramid (double radius,
   double height,
   OdInt32 divLength,
   OdInt32 divHeight,
   OdInt32 divCap,
   OdInt32 nSides,
   double radiusRatio,
   OdInt32 subDLevel);

 OdResult computeRayIntersection(const OdGePoint3d &rayStart,
   const OdGeVector3d &rayDir,
   OdArray<OdDbSubentId> &retSubents,
   OdArray<double> &retIntersectDist,
   OdGePoint3dArray& retIntersectPoint) const;

 OdResult subdDivideUp ();
 OdResult subdDivideDown ();
 OdResult subdRefine ();
 OdResult subdRefine (const OdDbFullSubentPathArray& subentPaths);
 OdResult subdLevel (OdInt32& result) const;

 OdResult splitFace (const OdDbSubentId& subentFaceId,
   const OdDbSubentId& subent0,
   const OdGePoint3d& point0,
   const OdDbSubentId& subent1,
   const OdGePoint3d& point1);

 OdResult extrudeFaces (const OdDbFullSubentPathArray& subentPaths,
   double length,
   const OdGeVector3d& dir,
   double taper);

 OdResult extrudeFaces (const OdDbFullSubentPathArray& subentPaths,
   const OdGePoint3dArray& alongPath,
   double taper);

 OdResult isWatertight (bool& result) const;

 OdResult numOfFaces (OdInt32& result) const;
 OdResult numOfSubDividedFaces (OdInt32& result) const;
 OdResult numOfSubDividedFacesAt (const OdDbFullSubentPathArray& subentPaths, OdInt32& result) const;
 OdResult numOfVertices (OdInt32& result) const;
 OdResult numOfSubDividedVertices (OdInt32& result) const;
 OdResult numOfEdges (OdInt32& result) const;

 OdResult getVertices (OdGePoint3dArray& vertexArray) const;
 OdResult getEdgeArray (OdInt32Array& edgeArray) const;
 OdResult getFaceArray (OdInt32Array& faceArray) const;
 OdResult getNormalArray (OdGeVector3dArray& normalArray) const;

 OdResult getSubDividedVertices (OdGePoint3dArray& vertexArray) const;
 OdResult getSubDividedFaceArray (OdInt32Array& faceArray) const;
 OdResult getSubDividedNormalArray(OdGeVector3dArray& normalArray) const;

 OdResult getVertexAt (OdInt32 nIndex, OdGePoint3d& vertex) const;
 OdResult setVertexAt (OdInt32 nIndex, const OdGePoint3d& vertex);
 OdResult getVertexAt (const OdDbSubentId& id, OdGePoint3d& vertex) const;
 OdResult setVertexAt (const OdDbSubentId& id, const OdGePoint3d& vertex);

 OdResult getSubDividedVertexAt (OdInt32 nIndex, OdGePoint3d& vertex) const;
 OdResult getSubDividedVertexAt (const OdDbSubentId& id, OdGePoint3d& vertex) const;

 OdResult setCrease (double creaseVal);
 OdResult setCrease (const OdDbFullSubentPathArray& subentPaths, double creaseVal);
 OdResult getCrease (const OdDbFullSubentPathArray& subentPaths, OdDoubleArray& result) const;
 OdResult getCrease (const OdDbSubentId& id, double& result) const;

 OdResult getAdjacentSubentPath (const OdDbFullSubentPath& path,
   OdDb::SubentType type,
   OdDbFullSubentPathArray& subentPaths) const;

 OdResult getSubentPath (OdInt32 nIndex,
   OdDb::SubentType type,
   OdDbFullSubentPathArray& subentPaths) const;

 OdResult convertToSurface (bool bConvertAsSmooth, const OdDbSubentId& id, OdDbSurfacePtr& pSurface) const;
 OdResult convertToSurface (bool bConvertAsSmooth, bool optimize, OdDbSurfacePtr& pSurface) const;
 OdResult convertToSolid (bool bConvertAsSmooth, bool optimize, OdDb3dSolidPtr& pSolid) const;

 OdResult getSubentColor (const OdDbSubentId& id, OdCmColor& color) const;
 OdResult setSubentColor (const OdDbSubentId& id, const OdCmColor& color);
 OdResult getSubentMaterial (const OdDbSubentId& id, OdDbObjectId& material) const;
 OdResult setSubentMaterial (const OdDbSubentId& id, const OdDbObjectId& material);
 OdResult getSubentMaterialMapper (const OdDbSubentId& id, OdGiMapper& mapper) const;
 OdResult setSubentMaterialMapper (const OdDbSubentId& id, const OdGiMapper& mapper);

 OdResult getFacePlane (const OdDbSubentId& id, OdGePlane& facePlane) const;

 OdResult computeVolume (double &retVolume) const;
 OdResult computeSurfaceArea (double &retSurfArea) const;



 OdResult getVertexNormalArray(OdGeVector3dArray& arrNorm);
 OdResult getVertexTextureArray(OdGePoint3dArray& arrPts);
 OdResult getVertexColorArray(OdCmEntityColorArray& arrColor);
 OdResult setVertexNormalArray(OdGeVector3dArray& arrNorm);
 OdResult setVertexTextureArray(OdGePoint3dArray& arrPts);
 OdResult setVertexColorArray(OdCmEntityColorArray& arrColor);
 OdResult clearVertexNormalArray();
 OdResult clearVertexTextureArray();
 OdResult clearVertexColorArray();

 OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type,
   OdGsMarker gsMark, 
   const OdGePoint3d& ,
   const OdGeMatrix3d& , 
   OdDbFullSubentPathArray& subentPaths, 
   const OdDbObjectIdArray* pEntAndInsertStack ) const;
 OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
   OdGsMarkerArray& gsMarkers) const;
};

/** \details
 This template class is a specialization of the OdSmartPtr class for OdDbSubDMesh object pointers.
*/
typedef OdSmartPtr<OdDbSubDMesh> OdDbSubDMeshPtr;

/** \details
  Corresponding C++ library: TD_Db
  <group !!RECORDS_tdrawings_apiref>
*/
struct MeshFaceterSettings
{
  double faceterDevSurface;
  double faceterDevNormal;
  double faceterGridRatio;
  double faceterMaxEdgeLength;
  OdUInt16 faceterMaxGrid;
  OdUInt16 faceterMinUGrid;
  OdUInt16 faceterMinVGrid;
  OdInt16 faceterMeshType;

  MeshFaceterSettings()
    : faceterDevSurface(0.)
    , faceterDevNormal(0.)
    , faceterGridRatio(0.)
    , faceterMaxEdgeLength(0)
    , faceterMaxGrid(0)
    , faceterMinUGrid(0)
    , faceterMinVGrid(0)
    , faceterMeshType(0)  {}
};

typedef struct MeshFaceterSettings  OdDbFaceterSettings;

DBENT_EXPORT OdResult oddbGetObjectMesh(OdDbObject *pObj, 
                                           const OdDbFaceterSettings *faceter,
                                           OdGePoint3dArray& vertexArray, 
                                           OdInt32Array& faceArray,
                                           OdGiFaceData*& faceData);


#include "TD_PackPop.h"

#endif
