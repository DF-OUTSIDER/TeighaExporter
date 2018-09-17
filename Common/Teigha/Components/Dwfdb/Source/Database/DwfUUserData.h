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

#ifndef DWFUUSERDATA_H 
#define DWFUUSERDATA_H 

#include "DwfDbItemImpl.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GePoint2dArray.h"
#define STL_USING_MAP
#include "OdaSTL.h"

#ifndef GEPOINT2DARRAYARRAY_INCLUDED // TODO #include "Ge/GePoint2dArrayArray.h"
  typedef OdArray<OdGePoint2dArray> OdGePoint2dArrayArray;
#endif

class WT_File;
class WT_Rendition;

OdString WTS2ODS(const class WT_String& s);

class DwfUUserData
{
protected:
  DwfUUserData()
  {
  }

public:
  ~DwfUUserData()
  {
  }

  virtual void setDrawContext(class OdGiViewportDraw* p) { throw OdError(eNotImplementedYet); }
  virtual void setUDrawContext(const OdDbUnderlayDrawContext* context) { throw OdError(eNotImplementedYet); }

  virtual void setClipState(bool val) = 0;
  virtual void setTransformState(bool val) = 0;
  virtual void setUnits(OdDb::UnitsValue val) = 0;
  virtual void setLayers(OdArray<OdUnderlayLayer> *p) = 0;
  virtual void setTransform(const OdGeMatrix3d &mx) = 0;

  virtual OdDbBaseDatabase* baseDatabase() = 0;
  virtual void pushModelTransform(const OdGeMatrix3d& mx) = 0;
  virtual void popModelTransform() = 0;
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary) = 0;
  virtual void popClipBoundary() = 0;
  virtual bool applyTraits(WT_Rendition& rendition, const class WT_Object* pObj) = 0;
  virtual void setFillAlways() = 0;
  virtual void drawPolyline(WT_Rendition& rendition, const OdGePoint3d *pPoints, OdUInt32 nPoints) = 0;
  virtual void drawEllipArc(WT_Rendition& rendition, const OdGeEllipArc3d &ell2) = 0;
  virtual bool drawHatch(WT_Rendition& rendition,
                         const OdGePoint2dArrayArray& vertexPtsArray, 
                         const OdGeDoubleArray& vertexBulges) = 0; // for vertexPtsArray[0] only
  virtual void drawImage(WT_Rendition& rendition,
                         const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                         OdGiRasterImagePtr pImage, const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts) = 0;
  virtual void drawPolygon(WT_Rendition& rendition,
                           OdInt32 numVertices, const OdGePoint3d* pVertexList) = 0;
  virtual void drawShell(WT_Rendition& rendition,
                         OdInt32 numVertices, const OdGePoint3d* vertexList, 
                         OdInt32 faceListSize, const OdInt32* faceList) = 0;
  virtual bool drawText(WT_Rendition& rendition, WT_Text& text) = 0;

  //virtual OdGiViewportDraw* getDrawContext() { throw OdError(eNotImplementedYet); }
  //virtual const OdDbUnderlayDrawContext* getUDrawContext()  { throw OdError(eNotImplementedYet); }
  virtual bool getClipState()  { throw OdError(eNotImplementedYet); }
  virtual bool getTransformState()  { throw OdError(eNotImplementedYet); }
  virtual OdDb::UnitsValue getUnits()  { throw OdError(eNotImplementedYet); }
  virtual OdArray<OdUnderlayLayer>& getLayers()  { throw OdError(eNotImplementedYet); }
  //virtual std::map<int, int>& getLayerCache()  { throw OdError(eNotImplementedYet); }
  virtual std::map<int, OdDbStub*>& getLineTypeCache()  { throw OdError(eNotImplementedYet); }
  virtual const OdGeMatrix3d& getTransform()  { throw OdError(eNotImplementedYet); }

  static OdDbBaseDatabase* baseDatabase(WT_File& file);
  static void pushModelTransform(WT_File& file, const OdGeMatrix3d& mx);
  static void popModelTransform(WT_File& file);
  static void pushClipBoundary(WT_File& file, OdGiClipBoundary* pBoundary);
  static void popClipBoundary(WT_File& file);
  static bool applyTraits(WT_File& file);
  static void setFillAlways(WT_File& file);
  static void drawPolyline(WT_File& file, const OdGePoint3d *pPoints, OdUInt32 nPoints);
  static void drawEllipArc(WT_File& file, const OdGeEllipArc3d &ell2);
  static bool drawHatch(WT_File& file, 
                        const OdGePoint2dArrayArray& vertexPtsArray, 
                        const OdGeDoubleArray& vertexBulges); // for vertexPtsArray[0] only
  static void drawImage(WT_File& file,
                        const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                        OdGiRasterImagePtr pImage, const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts);
  static void drawPolygon(WT_File& file, OdInt32 numVertices, const OdGePoint3d* pVertexList);
  static void drawShell(WT_File& file,
                        OdInt32 numVertices, const OdGePoint3d* vertexList, 
                        OdInt32 faceListSize, const OdInt32* faceList);
  static bool drawText(WT_File& file, WT_Text& text);

  //static void setDrawContext(WT_File& file, OdGiViewportDraw* p);
  //static void setUDrawContext(WT_File& file, const OdDbUnderlayDrawContext* context);
  static void setClipState(WT_File& file, bool val);
  static void setTransformState(WT_File& file, bool val);
  static void setUnits(WT_File& file, OdDb::UnitsValue val);
  static void setLayers(WT_File& file, OdArray<OdUnderlayLayer> *p);
  static void setTransform(WT_File& file, const OdGeMatrix3d &mx);

  //static OdGiViewportDraw* getDrawContext(WT_File& file);
  //static const OdDbUnderlayDrawContext* getUDrawContext(WT_File& file);
  static bool getClipState(WT_File& file);
  static bool getTransformState(WT_File& file);
  static OdDb::UnitsValue getUnits(WT_File& file);
  static OdArray<OdUnderlayLayer>& getLayers(WT_File& file);
  //static std::map<int, int>& getLayerCache(WT_File& file);
  static std::map<int, OdDbStub*>& getLineTypeCache(WT_File& file);
  static const OdGeMatrix3d& getTransform(WT_File& file);
};

class DwfUUserDataImpl : public DwfUUserData
{
public:
  OdArray<OdUnderlayLayer> *layers;
  std::map<int, int> layercache;
  std::map<int, OdDbStub*> lineTypecache;

  const OdDbUnderlayDrawContext* pContext;
  OdGiViewportDraw* pVd;
  bool ClipPushed;
  bool transformPushed;
  OdGeMatrix3d mx;

  OdDb::UnitsValue dbu;

  DwfUUserDataImpl()
    : pVd(0)
    , ClipPushed(false)
    , transformPushed(false)
    , dbu(OdDb::kUnitsUndefined)
    , layers(0)
    , pContext(0)
  {
  }

  ~DwfUUserDataImpl()
  {
    int t=0;
  }

  virtual OdDbBaseDatabase* baseDatabase();
  virtual void pushModelTransform(const OdGeMatrix3d& mx);
  virtual void popModelTransform();
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary);
  virtual void popClipBoundary();
  virtual bool applyTraits(WT_Rendition& rendition, const WT_Object* pObj);
  virtual void setFillAlways();
  virtual void drawPolyline(WT_Rendition& rendition, const OdGePoint3d *pPoints, OdUInt32 nPoints);
  virtual void drawEllipArc(WT_Rendition& rendition, const OdGeEllipArc3d &ell2);
  virtual bool drawHatch(WT_Rendition& rendition, 
                         const OdGePoint2dArrayArray& vertexPtsArray, 
                         const OdGeDoubleArray& vertexBulges); // for vertexPtsArray[0] only
  virtual void drawImage(WT_Rendition& rendition,
                         const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                         OdGiRasterImagePtr pImage, const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts);
  virtual void drawPolygon(WT_Rendition& rendition,
                           OdInt32 numVertices, const OdGePoint3d* pVertexList);
  virtual void drawShell(WT_Rendition& rendition,
                         OdInt32 numVertices, const OdGePoint3d* vertexList, 
                         OdInt32 faceListSize, const OdInt32* faceList);
  virtual bool drawText(WT_Rendition& rendition, WT_Text& text);

  virtual void setDrawContext(OdGiViewportDraw* p);
  virtual OdGiViewportDraw* getDrawContext();
  virtual void setUDrawContext(const OdDbUnderlayDrawContext* context);
  virtual const OdDbUnderlayDrawContext* getUDrawContext();

  virtual void setClipState(bool val);
  virtual bool getClipState();
  virtual void setTransformState(bool val);
  virtual bool getTransformState();
  virtual void setTransform(const OdGeMatrix3d &mx);
  virtual const OdGeMatrix3d& getTransform();
  virtual void setUnits(OdDb::UnitsValue val);
  virtual OdDb::UnitsValue getUnits();
  virtual void setLayers(OdArray<OdUnderlayLayer> *p);
  virtual OdArray<OdUnderlayLayer>& getLayers();
  virtual std::map<int, int>& getLayerCache();
  virtual std::map<int, OdDbStub*>& getLineTypeCache();
};

#endif// DWFUUSERDATA_H
