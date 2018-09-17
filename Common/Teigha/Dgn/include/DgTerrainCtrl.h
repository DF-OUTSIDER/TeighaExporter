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
#ifndef __DG_TERRAINCTRL_H__
#define __DG_TERRAINCTRL_H__

#include "TD_PackPush.h"

#include "DgElement.h"
#include "Ge/GeVector3dArray.h"

//-------------------------------------------------------------------
// OdDgTerrainControlElement
//-------------------------------------------------------------------

// type of Extended control element, subtype Terrain Control Element

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/

//-----------------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgTerrainFeature
{
public:
  enum OdDgTerrainFeatureType
  {
    kUnknown     = 0,
    kDataPoints  = 1,
    kIsland      = 2,
    kHole        = 3,
    kVoid        = 4,
    kContour     = 5,
    kBreakLine   = 6,
    kOuterBoundary = 7
  };

public:
  OdDgTerrainFeature();
  virtual ~OdDgTerrainFeature();

  OdUInt32               getFeatureNumber() const;
  void                   setFeatureNumber( OdUInt32 uNumber );

  OdDgTerrainFeatureType getFeatureType() const;
  void                   setFeatureType( OdDgTerrainFeatureType uType );

  bool                   getCorruptedFeatureTypeFlag() const;
  void                   setCorruptedFeatureTypeFlag( bool bSet, const OdGePoint3dArray& arrTerrainVertices );

  OdUInt32Array          getFeatureVertexIndexes() const;
  void                   setFeatureVertexIndexes( const OdUInt32Array& arrVertexIndexes );

  OdGePoint3dArray       getCorruptedFeaturePoints() const;
  void                   setCorruptedFeaturePoints( const OdGePoint3dArray& arrCorruptedPts );

private:
  bool                    m_bCorruptedFeature;
  OdUInt32                m_uFeatureNumber;
  OdDgTerrainFeatureType  m_uFeatureType;
  OdUInt32Array           m_arrFeaturePointIndex;
  OdGePoint3dArray        m_arrCorruptedFeaturePoints;
};

//-----------------------------------------------------------------------------------------------------------

struct OdDgTerrainTriangle
{
  OdUInt32 m_uFirstIndex;
  OdUInt32 m_uSecondIndex;
  OdUInt32 m_uThirdIndex;
  bool     m_bVisible;
};

//-----------------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgTerrainControlElement : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgTerrainControlElement );
public:
  enum OdDgTerrainEdgeMethod
  {
    kNone    = 1,
    kSlivers = 2,
    kMaxTriangleLength = 3
  };

public:

  // Name
  virtual OdString getName() const;
  virtual void     setName( const OdString& strName );

  //Transformation
  virtual OdGeMatrix3d getTransformation() const;
  virtual void         setTransformation( const OdGeMatrix3d& matTransform );

  //Boundary
  virtual OdUInt32Array getBoundary() const;

  //Terrain Graphics Element Reference
  virtual OdDgElementId getTerrainElementId() const;

  //Vertices
  virtual void                      getTerrainVertices( OdGePoint3dArray& arrVertices ) const;
  virtual const OdGePoint3dArray&   getTerrainVertices() const;
  virtual void                      setTerrainVertices( const OdGePoint3dArray& arrVertices );
  virtual void                      getTerrainNormals( OdGeVector3dArray& arrNormals ) const;

  //Calculated Features
  virtual OdGePoint3dArray          getTerrainHighPoints() const;
  virtual OdGePoint3dArray          getTerrainLowPoints( double dMinDepth ) const;
  virtual OdGePoint3dArray          getFeatureSourceVertices() const;
  virtual OdArray<OdGePoint3dArray> getTerrainMajorContourPoints( const OdGePoint3dArray& arrTransformedPts, OdBoolArray& arrContourDepression, double dInterval ) const;
  virtual OdArray<OdGePoint3dArray> getTerrainMinorContourPoints( const OdGePoint3dArray& arrTransformedPts, OdBoolArray& arrContourDepression, double dInterval, double dMajorInterval ) const;

  //Features
  virtual OdUInt32 getFeatureCount() const;
  virtual OdDgTerrainFeature getFeature( OdUInt32 uFeatureIndex ) const;
  virtual void setFeature( OdUInt32 uFeatureIndex, const OdDgTerrainFeature& featureDescription );
  virtual void addFeature( const OdDgTerrainFeature& featureDescription );
  virtual void deleteFeature( OdUInt32 uFeatureIndex );  

  //Triangles
  virtual OdUInt32 getTriangleCount() const;
  virtual OdDgTerrainTriangle getTriangle( OdUInt32 uTriangleIndex ) const;
  virtual void setTriangle( OdUInt32 uTriangleIndex, const OdDgTerrainTriangle& terrainTriangle );
  virtual void addTriangle( const OdDgTerrainTriangle& terrainTriangle );
  virtual void deleteTriangle( OdUInt32 uTriangleIndex );

  virtual OdDgTerrainEdgeMethod getEdgeMethod() const;
  virtual void setEdgeMethod( OdDgTerrainEdgeMethod uMethod );
  virtual double getMaxTriangleLength() const;
  virtual void setMaxTriangleLength( double dLength );

  //overrides for OdGiDrawable
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

typedef OdSmartPtr< OdDgTerrainControlElement > OdDgTerrainControlElementPtr;

#include "TD_PackPop.h"

#endif // __DG_TERRAINCTRL_H__
