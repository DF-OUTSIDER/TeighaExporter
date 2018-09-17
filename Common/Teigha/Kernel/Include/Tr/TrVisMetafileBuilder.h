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
// GLES2 device metafile builder

#ifndef ODTRVISMETAFILEBUILDER
#define ODTRVISMETAFILEBUILDER

#include "TD_PackPush.h"
#include "TrVisMetafileDef.h"
#include "TrVisCamera.h"
#include "Ge/GeCurve2d.h"
#include "Ge/GeCurve3d.h"

class OdTrVisMetafileWriter;
struct OdTrVisMetafileContainer;
class OdTrVisFont;

enum OdTrVisEnabledVertexData
{
  kVDNone = 0,
  kVDColor = 1,
  kVDNormal1 = 2,
  kVDNormal2 = 4,
  kVDTexCoord = 8,
  kVDDepth = 16
};

struct OdTrVisVertexData
{
  OdTrVisVertexData();
  OdTrVisVertexData( OdTrVisColorRGBA*, OdTrVisColorRGBA::ColorRep = OdTrVisColorRGBA::kRGBA, OdGeVector3d* = NULL,
                     OdGeVector3d* = NULL, OdGePoint2d* = NULL, double* = NULL );
  ~OdTrVisVertexData();

  OdTrVisColorRGBA* arrayColor;
  OdTrVisColorRGBA::ColorRep colorRep;
  OdGeVector3d* arrayNormal1;
  OdGeVector3d* arrayNormal2;
  OdGePoint2d*  arrayTexCoord;
  double*       arrayDepth;
};

class OdTrVisGeometryDraw : public OdRxObject
{
protected:
  OdTrVisGeometryDraw() {};
public:
  enum LinesPrimType
  {
    kLines,
    kLineLoop,
    kLineStrip
  };
  enum TrianglesPrimType
  {
    kTriangles,
    kTriangleStrip,
    kTriangleFan
  };

  virtual ~OdTrVisGeometryDraw() {};

  virtual void polypoint(OdUInt32 nPoints, const OdGePoint3d *pPoints) = 0;
  virtual void polypoint(OdUInt32 nPoints, const OdGePoint3d *pPoints, const OdTrVisVertexData& data ) = 0;

  virtual void polyline(OdUInt32 nPoints, const OdGePoint3d *pPoints, LinesPrimType type ) = 0;
  virtual void polyline(OdUInt32 nPoints, const OdGePoint3d *pPoints, LinesPrimType type, const OdTrVisVertexData& data ) = 0;

  virtual void triangle( OdUInt32 nPoints, const OdGePoint3d *pPoints, TrianglesPrimType type ) = 0;
  virtual void triangle( OdUInt32 nPoints, const OdGePoint3d *pPoints, TrianglesPrimType type, const OdTrVisVertexData& data ) = 0;

  virtual void polygon(OdUInt32 nPoints, const OdGePoint3d *pPoints) = 0;
  virtual void polygon(OdUInt32 nPoints, const OdGePoint3d *pPoints, const OdTrVisVertexData& data) = 0;

  virtual void arc(const OdGePoint3d &center, double radius, const OdGeVector3d &normal = OdGeVector3d::kZAxis, OdUInt32 nSegs = 16,
                     const OdGeVector3d &start = OdGeVector3d::kYAxis, double angleFrom = 0.0, double angleTo = Oda2PI) = 0;
  virtual void circle( const OdGePoint3d &center, double radius, const OdGeVector3d &normal, OdUInt32 nSegs ) = 0;
  virtual void curve2d( const OdGeCurve2d& curve ) = 0;
  virtual void curve3d( const OdGeCurve3d& curve ) = 0;

  virtual void setDeviation( double dev ) = 0;
  virtual double deviation() const = 0;

  //Indexed mode
  virtual void beginIndexedMode(OdUInt32 nPoints, const OdGePoint3d* pPoints, const OdTrVisVertexData& data = OdTrVisVertexData() ) = 0;
  virtual void endIndexedMode() = 0;

  virtual void indexedPolypoint( OdUInt32 nPoints, const OdInt32* pIndeces ) = 0;
  virtual void indexedPolypoint( OdUInt32 nPoints, const OdInt32* pIndeces, OdUInt8 nEnabledVertexData ) = 0;

  virtual void indexedPolyline( OdUInt32 nPoints, const OdInt32* pIndeces, LinesPrimType type ) = 0;
  virtual void indexedPolyline( OdUInt32 nPoints, const OdInt32* pIndeces, LinesPrimType type, OdUInt8 nEnabledVertexData ) = 0;

  virtual void indexedTriangle( OdUInt32 nPoints, const OdInt32* pIndeces, TrianglesPrimType type ) = 0;
  virtual void indexedTriangle( OdUInt32 nPoints, const OdInt32* pIndeces, TrianglesPrimType type, OdUInt8 nEnabledVertexData ) = 0;

  virtual void indexedPolygon( OdUInt32 nPoints, const OdInt32* pIndeces ) = 0;
  virtual void indexedPolygon( OdUInt32 nPoints, const OdInt32* pIndeces, OdUInt8 nEnabledVertexData ) = 0;

  virtual void text( const OdGePoint3d& pt, const OdGeVector3d& direction, const OdGeVector3d& normal, const char* str, const OdTrVisFont* pFont ) = 0;
};

typedef OdSmartPtr< OdTrVisGeometryDraw > OdTrVisGeometryDrawPtr;

class OdTrVisMetafileBuilder : public OdRxObject
{
protected:
  OdTrVisMetafileBuilder(){}
public:
  virtual ~OdTrVisMetafileBuilder(){}

  virtual void beginMetafileBuilding(const OdTrVisCamera* pCamera, OdTrVisViewportId refVp, OdTrVisOverlayId refOr = kTrVisMainOverlayId, bool b2dModel = false) = 0;
  virtual void beginMetafileBuilding(OdTrVisMetafileWriter* pWriter, const OdTrVisCamera* pCamera, OdTrVisViewportId refVp, OdTrVisOverlayId refOr = kTrVisMainOverlayId, bool b2dModel = false) = 0;
  virtual void endMetafileBuilding(OdTrVisMetafileDef &mfDef, bool bFin = false) = 0;

  virtual void rollback(bool bFin = true) = 0;

  virtual void writeAttribute(OdTrVisAttribute attrName, bool bEnable = true) = 0;
  virtual void writeShading( OdTrVisShading shadName, bool bEnable = true ) = 0;
  
  // @@@TODO: reimplement this using TransformStack
  virtual void pushTransform(const OdGeMatrix3d &tf) = 0;
  virtual void popTransform() = 0;

  virtual void addMetafile(OdTrVisMetafileId mfId) = 0;

  virtual void setGeometryMarker(OdTrVisGeomType geomType) = 0;
  virtual void setMaterial( OdTrVisMaterialId matId ) = 0;

  virtual OdTrVisGeometryDraw* wcsGeometry() = 0;
  virtual OdTrVisGeometryDraw* ecsGeometry() = 0;
  virtual OdTrVisGeometryDraw* scsGeometry() = 0;

  virtual void setColor( const OdTrVisColorRGBA& ) = 0;

   // @@@TODO: eliminate this, but now ExClip uses it
  virtual OdTrVisViewportId viewportId() const = 0;
  virtual OdTrVisOverlayId overlayId() const = 0;

  static OdSmartPtr< OdTrVisMetafileBuilder > createObject();
};

typedef OdSmartPtr< OdTrVisMetafileBuilder > OdTrVisMetafileBuilderPtr;

#include "TD_PackPop.h"

#endif // ODTRVISMETAFILEBUILDER
