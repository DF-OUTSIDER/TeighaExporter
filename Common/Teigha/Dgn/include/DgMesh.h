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
#ifndef __DG_MESH_H__
#define __DG_MESH_H__


#include "TD_PackPush.h"
#include "DgGraphicsElement.h"
#include "Int32Array.h"


/** \details
Common interface for auxiliary data (normals, colors, etc) that can be linked either to faces or to indices

\remarks

In common, any data is identified by a pair of indices. It is done, for example, to identify a data linked to points of a quad grid.
If a data is linked to some linear type (like point cloud), then the second index should be zero.

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData
{
  enum DataTypes
  {
    kTypeNormal,
    kTypeParameter,
    kTypeDoubleColor,
    kTypeIntegerColor,
    kTypeTableColor,
    kTypeDensity,
    kTypePressure,
    kTypeTemperature,
    kTypeDisplacement,
    kTypeVelocity
  };

  enum LinkTypes
  {
    kLinkToVertices,
    kLinkToFaces
  };

  virtual ~OdDgMeshAuxiliaryData();

  virtual DataTypes getDataType() const = 0;
  LinkTypes getLinkType() const;
};

/** \details
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_Normal : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  OdGeVector3d get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( const OdGeVector3d& value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_Parameter : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  OdGePoint2d get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( const OdGePoint2d& value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_DoubleColor : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  OdGePoint3d get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( const OdGePoint3d& value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_IntegerColor : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  OdUInt32 get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( OdUInt32 value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_TableColor : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  OdUInt32 get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( OdUInt32 value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details 
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_Density : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  double get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( double value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details 
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_Pressure : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  double get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( double value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details 
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_Temperature : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  double get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( double value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details 
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_Displacement : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  OdGeVector3d get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( const OdGeVector3d& value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details 
One of the available types of auxiliary data.

<group OdDg_Classes>
*/
struct TG_EXPORT OdDgMeshAuxiliaryData_Velocity : public OdDgMeshAuxiliaryData
{
  //overrides for OdDgMeshAuxiliaryData
  DataTypes getType() const;

  //access to the data
  OdGeVector3d get( OdUInt32 row, OdUInt32 column = 0 ) const;
  void set( const OdGeVector3d& value, OdUInt32 row, OdUInt32 column = 0 );
};

/** \details

Common interface for meshes.

Auxiliary data (color, normals, etc) is available from here.
Different styles use their specific ways to manipulate geometric data (vertices and faces).

    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgMesh : public OdDgGraphicsElement
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMesh );
public:

  //styles of meshes
  enum StyleValues
  {
    kFaceLoops = 1,
    kPointCloud,
    kTriangleList,
    kQuadList,
    kTriangleGrid,
    kQuadGrid
  };

  //the style
  StyleValues getStyle();

  //
  //manipulation with auxiliary data
  //note: it is under construction, so no data is available
  //

  //get a pointer to a specified auxiliary data (or return zero if such data is not defined)
  OdDgMeshAuxiliaryData* getAuxiliaryData( OdDgMeshAuxiliaryData::DataTypes type, OdDgMeshAuxiliaryData::LinkTypes linkedTo );
  const OdDgMeshAuxiliaryData* getAuxiliaryData( OdDgMeshAuxiliaryData::DataTypes type, OdDgMeshAuxiliaryData::LinkTypes linkedTo ) const;

  //define a specified data (do nothing if it exists already)
  void defineAuxiliaryData( OdDgMeshAuxiliaryData::DataTypes type, OdDgMeshAuxiliaryData::LinkTypes linkedTo );

  //remove a specified data (do nothing if it does not exist)
  void removeAuxiliaryData( OdDgMeshAuxiliaryData::DataTypes type, OdDgMeshAuxiliaryData::LinkTypes linkedTo );

  //overrides from OdDgElement
  OdResult subOpen( OdDg::OpenMode mode );
  OdResult subClose();

  virtual OdRxObjectPtr clone() const;
  
};

typedef OdSmartPtr< OdDgMesh > OdDgMeshPtr;

/** \details

Mesh of style 1: indexed face loops

It consists of:

1. A number of vertices
2. A number of faces that are specified as indices of vertices they are bounded by
3. Each edge of a face can be either visible or not that is determined by the m_impliesVisibleEdge flag of the point that edge starts from

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshFaceLoops : public OdDgMesh
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMeshFaceLoops );
public:

  //
  // Faces
  //

  struct TG_EXPORT FacePoint
  {
    FacePoint();

    OdInt32   m_pointIndex;         // index of a point of points array
    OdInt32   m_colorIndex;         // index of a color table record for a point
    double    m_dColorRGB[3];       // double values of red, green and blue color ( 0...1 )
    OdGeVector3d m_vrNormal;        // normal to the face in a point
    OdGePoint2d  m_texCoords;       // texture coordinates of a point ( x and y values are integer )
    bool      m_impliesVisibleEdge; // if true (default), then it is the visible edge that starts at this point
  };

  typedef OdArray< FacePoint > VerticesArray;

  OdUInt32 getFacesNumber() const;
  void setFacesNumber( OdUInt32 number );
  void removeFace( OdUInt32 index );
  void removeAllFaces();

  //returns an index of the new face
  OdUInt32 addFace( const VerticesArray& vertices );
  void getFace( OdUInt32 index, VerticesArray& vertices ) const;
  void setFace( OdUInt32 index, const VerticesArray& vertices );

  bool getUseColorTableIndexesFlag() const;
  bool getUseDoubleColorsFlag() const;
  bool getUseNormalsFlag() const;
  bool getUseTextureCoordinatesFlag() const;
  void setUseColorTableIndexesFlag( bool bSet );
  void setUseDoubleColorsFlag( bool bSet );
  void setUseNormalsFlag( bool bSet );
  void setUseTextureCoordinatesFlag( bool bSet );


  //
  // Points
  //

  OdUInt32 getPointsNumber() const;
  void setPointsNumber( OdUInt32 number );
  void removePoint( OdUInt32 index );
  void removeAllPoints();

  //return an index of the new point
  OdUInt32 addPoint( const OdGePoint3d& point );
  OdGePoint3d getPoint( OdUInt32 index ) const;
  void setPoint( OdUInt32 index, const OdGePoint3d& point );

  //overrides for OdDgGraphicsElement
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgMeshFaceLoops > OdDgMeshFaceLoopsPtr;

/** \details

Mesh of style 2: point cloud

It is a set of points

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshPointCloud : public OdDgMesh
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMeshPointCloud );
public:

  OdUInt32 getPointsNumber() const;
  void setPointsNumber( OdUInt32 number );
  void removePoint( OdUInt32 index );
  void removeAllPoints();

  //return an index of the new point
  OdUInt32 addPoint( const OdGePoint3d& point );
  OdGePoint3d getPoint( OdUInt32 index ) const;
  void setPoint( OdUInt32 index, const OdGePoint3d& point );

  //overrides for OdDgGraphicsElement
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
  OdResult subGetGeomExtents(const OdDgElementId& idView, OdGeExtents3d& extents) const;
};

typedef OdSmartPtr< OdDgMeshPointCloud > OdDgMeshPointCloudPtr;

/** \details

Mesh of style 3: triangle list

It is a set of triangles defined by their 3 points

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshTriangleList : public OdDgMesh
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMeshTriangleList );
public:

  struct Triangle
  {
    OdGePoint3d m_points[ 3 ];
  };

  OdUInt32 getTrianglesNumber() const;
  void setTrianglesNumber( OdUInt32 number );
  void removeTriangle( OdUInt32 index );
  void removeAllTriangles();

  //returns an index of the new triangle
  OdUInt32 addTriangle( const OdDgMeshTriangleList::Triangle& triangle );
  void getTriangle( OdUInt32 index, OdDgMeshTriangleList::Triangle& triangle ) const;
  void setTriangle( OdUInt32 index, const OdDgMeshTriangleList::Triangle& triangle );

  //overrides for OdDgGraphicsElement
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgMeshTriangleList > OdDgMeshTriangleListPtr;

/** \details

Mesh of style 4: quad list

It is a set of quads defined by their 4 points

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshQuadList : public OdDgMesh
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMeshQuadList );
public:

  struct Quad
  {
    OdGePoint3d m_points[ 4 ];
  };

  OdUInt32 getQuadsNumber() const;
  void setQuadsNumber( OdUInt32 number );
  void removeQuad( OdUInt32 index );
  void removeAllQuads();

  //returns an index of the new quad
  OdUInt32 addQuad( const OdDgMeshQuadList::Quad& quad );
  void getQuad( OdUInt32 index, OdDgMeshQuadList::Quad& quad ) const;
  void setQuad( OdUInt32 index, const OdDgMeshQuadList::Quad& quad );

  //overrides for OdDgGraphicsElement
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgMeshQuadList > OdDgMeshQuadListPtr;

/** \details
Mesh of style 5: triangle grid
It is a grid R[M][N] of points that determines (M-1)*(N-1)*2 triangles bounded by points:

1. R[i][j], R[i+1][j], R[i+1][j+1]
2. R[i][j], R[i+1][j+1], R[i][j+1]

for each 0&lt;=i&lt;M-1, 0&lt;=j&lt;N-1

Corresponding C++ library: TG_Db 

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshTriangleGrid : public OdDgMesh
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMeshTriangleGrid );
public:

  //rows
  OdUInt32 getRowsNumber() const;
  void setRowsNumber( OdUInt32 number );
  OdUInt32 addRow();
  void removeRow( OdUInt32 index );

  //columns
  OdUInt32 getColumnsNumber() const;
  void setColumnsNumber( OdUInt32 number );
  OdUInt32 addColumn();
  void removeColumn( OdUInt32 index );

  //all together
  void setGridSize( OdUInt32 rows, OdUInt32 columns );
  void removeAllPoints();

  //points
  OdGePoint3d getPoint( OdUInt32 row, OdUInt32 column ) const;
  void setPoint( OdUInt32 row, OdUInt32 column, const OdGePoint3d& point );

  //overrides for OdDgGraphicsElement
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgMeshTriangleGrid > OdDgMeshTriangleGridPtr;

/** \details
<group OdDg_Classes>
Mesh of style 6: quad grid

It is a grid R[M][N] of points that determines (M-1)*(N-1) quads bounded by points:

R[i][j], R[i+1][j], R[i+1][j+1], R[i][j+1] for each 0&lt;=i&lt;M-1, 0&lt;=j&lt;N-1

Corresponding C++ library: TG_Db 
*/
class TG_EXPORT OdDgMeshQuadGrid : public OdDgMesh
{

  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgMeshQuadGrid );
public:

  //rows
  OdUInt32 getRowsNumber() const;
  void setRowsNumber( OdUInt32 number );
  OdUInt32 addRow();
  void removeRow( OdUInt32 index );

  //columns
  OdUInt32 getColumnsNumber() const;
  void setColumnsNumber( OdUInt32 number );
  OdUInt32 addColumn();
  void removeColumn( OdUInt32 index );

  //all together
  void setGridSize( OdUInt32 rows, OdUInt32 columns );
  void removeAllPoints();

  //points
  OdGePoint3d getPoint( OdUInt32 row, OdUInt32 column ) const;
  void setPoint( OdUInt32 row, OdUInt32 column, const OdGePoint3d& point );

  //overrides for OdDgGraphicsElement
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual OdResult subExplode( OdRxObjectPtrArray& entitySet ) const;
};

typedef OdSmartPtr< OdDgMeshQuadGrid > OdDgMeshQuadGridPtr;

#include "TD_PackPop.h"

#endif // __DG_MESH_H__
