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

#ifndef _DWFOBJECTIMPL_H_
#define _DWFOBJECTIMPL_H_

#include "Ge/GePoint3d.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeShell.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GePoint2dArray.h"
#include "DwfObject.h"
#include "DwfObjectContainer.h"
#include "Gi/GiMaterialTraitsData.h"
#include "Gi/GiTextStyle.h"
#include "Gi/GiClipBoundary.h"
#include "OdBinaryData.h"

#include "DbUnderlayItem.h"
#define STL_USING_MAP
#include "OdaSTL.h"

#ifndef GEPOINT2DARRAYARRAY_INCLUDED // TODO #include "Ge/GePoint2dArrayArray.h"
  typedef OdArray<OdGePoint2dArray> OdGePoint2dArrayArray;
#endif

class WT_Rendition;
class WT_Text;
class OdDwfViewLoadContext;
///////////////////////////////////////////////////////////////////////////////

class OdDwfObjectImpl : public OdDwfObject
{
protected:
  friend class DwfDatabaseImpl;
  OdDwfObjectId m_id;
  OdGsCache* m_pGsNode;
  //DwfObjectReactorArray m_reactors; //possible TODO 

  OdDwfObjectImpl();
public:
  ~OdDwfObjectImpl();
  ODRX_DECLARE_MEMBERS(OdDwfObjectImpl);

  // OdDwfObject members:
  virtual OdDwfObjectId objectId() const;
  virtual OdDbStub* ownerId() const;
  virtual void addReactor(OdDwfObjectReactor* pReactor);
  virtual void removeReactor(OdDwfObjectReactor* pReactor);

  // OdGiDrawable members:
  virtual bool isPersistent() const;
  virtual OdDbStub* id() const;
  virtual void setGsNode(OdGsCache* pGsNode);
  virtual OdGsCache* gsNode() const;
protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
  virtual void subViewportDraw(OdGiViewportDraw* vd) const;
};

typedef OdSmartPtr<OdDwfObjectImpl> OdDwfObjectImplPtr;

class DWFDBEXPORT OdDwfMaterial : public OdDwfObjectImpl
                                , public OdGiMaterialTraitsData
{
protected:
  double m_specularGloss; // like OdDbMaterialImpl::m_SpecularGloss(0.5)

  OdDwfMaterial();
public:
  ODRX_DECLARE_MEMBERS(OdDwfMaterial);

protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
};

typedef OdSmartPtr<OdDwfMaterial> OdDwfMaterialPtr;

class DWFDBEXPORT OdDwfLinetype : public OdDwfObjectImpl
                                , public OdGiLinetype
{
protected:
  double m_scale;

  OdDwfLinetype();
public:
  ODRX_DECLARE_MEMBERS(OdDwfLinetype);

  bool setLinePattern(const short* pattern, int length, double scale);
  double scale() const;
  void setScale(double patternLength);

protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
};

typedef OdSmartPtr<OdDwfLinetype> OdDwfLinetypePtr;

class DWFDBEXPORT OdDwfEntity : public OdDwfObjectImpl
{
protected:
  int m_tagIndex; // for 3d only (set of entities vam hav common tag)

  OdDwfEntity();
public:
  ODRX_DECLARE_MEMBERS(OdDwfEntity);

  void setTagIndex(int tagIndex) { m_tagIndex = tagIndex; }
  int tagIndex() const { return m_tagIndex; }
protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
};

typedef OdSmartPtr<OdDwfEntity> OdDwfEntityPtr;

class DWFDBEXPORT OdDwfView : public OdDwfObjectImpl
{
protected:
  class OdGsView* m_pGsView;
  OdString m_sName;
  friend class OdDwfViewLoadContext;
  mutable OdDwfViewLoadContext* m_pCtxToLoad;
  friend class DwfDatabaseImpl;

  OdDwfView();
  virtual bool loadPaperImage(OdStreamBuf* pStream);
public:
  ODRX_DECLARE_MEMBERS(OdDwfView);
  ~OdDwfView();

  virtual bool isModelLayout() const = 0;
  virtual bool getThumbnail(OdBinaryData& data) const;

  virtual OdString getName() const;
  void getViewports(OdDwfObjectIdArray& idsViewports) const;
  bool checkLoad() const;

  OdGsView* gsView() const;
  void setGsView(OdGsView* pGsView);
};

typedef OdSmartPtr<OdDwfView> OdDwfViewPtr;

class DWFDBEXPORT OdDwfModelView : public OdDwfView // for 3d DWF_TK_Camera
{
protected:
  friend class DWF_TK_Camera; friend class OdDwfGsLayoutHelper;
  OdGePoint3d m_ptCameraTarget;
  OdGeVector3d m_vecCameraDir, m_vecCameraUpVector;
  double m_withField, m_heightField;
  bool m_isPerspective;
  std::map<OdString, OdString> m_properties, // CORE-12865
                               m_propNames;
  OdGeMatrix3d m_matrix; friend class OdDwfViewLoadContext;
  class OdDwfObjectContainer m_idsSegments, 
                             m_idsIncludeLibrarySegments,
                             m_idsStyleLibrarySegments;
  OdDwfModelView();
public:
  ODRX_DECLARE_MEMBERS(OdDwfModelView);

  virtual bool isModelLayout() const;
  virtual bool getThumbnail(OdBinaryData& data) const;

  inline OdStringArray propertyKeys() const
  {
    OdStringArray keys;
    for (std::map<OdString, OdString>::const_iterator itr = m_properties.begin(); itr != m_properties.end(); itr++)
      keys.push_back(itr->first);
    return keys;
  }
  // use "_UnitAngular", "_UnitArea", "_UnitLinear" & "_UnitVolume" keys to get unit properties 
  inline OdString property(const OdString& key, const OdString& defVal = OdString::kEmpty) const
  {
    std::map<OdString, OdString>::const_iterator itr = m_properties.find(key);
    return itr != m_properties.end() ? itr->second : defVal;
  }
  inline OdString propertyName(const OdString& key) const
  {
    std::map<OdString, OdString>::const_iterator itr = m_propNames.find(key);
    return itr != m_propNames.end() ? itr->second : key;
  }

protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
  virtual void subViewportDraw(OdGiViewportDraw* vd) const;
};

typedef OdSmartPtr<OdDwfModelView> OdDwfModelViewPtr;

class DWFDBEXPORT OdDwfLayoutView : public OdDwfView // for OdDbBaseLayoutPE
{
protected:
  friend class OdDwfViewLoadContext;
  OdSmartPtr<class DwfDbItemImpl> m_pItem; // OdDbUnderlayItem // GiDrawable 
  friend class DwfDatabaseImpl;
  OdDb::UnitsValue m_PaperUnits;
  mutable double m_PaperWidth, m_PaperHeight, m_PaperHeightForSetupLayoutView;
  mutable OdGePoint3d m_ptPaperCenter,
                      m_ptPaperLowerLeft,
                      m_ptPaperUpperRight;
  ODCOLORREF m_PaperColor;
  OdGiRasterImagePtr m_pPaperImage;
  OdGeExtents3d m_bounds; // should be transformed

  friend class DwfStreamParserW2D;
  mutable std::map<int, OdSharedPtr<OdUnderlayLayer> > m_layers; // by m_layerKey
  std::map<int, OdDbStub*> m_lineTypecache;
  OdDb::UnitsValue m_units;
  mutable OdGeExtents3d m_Extents;

  OdGeMatrix3d m_matrix;

  OdDwfObjectContainer m_idsViewports;

  mutable const OdDbUnderlayDrawContext* m_pUnderlayDrawContext;
  mutable ODCOLORREF m_paletteBackgroundColor;

  OdDwfLayoutView();
public:
  ODRX_DECLARE_MEMBERS(OdDwfLayoutView);

  virtual OdString getName() const;
  virtual bool isModelLayout() const;
  virtual bool getThumbnail(OdBinaryData& data) const;
  virtual OdGiRasterImagePtr getThumbnail() const;

  OdDbUnderlayItemPtr getItem() const;

  void getViewports(OdDwfObjectIdArray& idsViewports) const;
  void getPlotPaperSize(double& width, double& height, OdGePoint3d* pCenter = NULL, bool bForSetupLayoutView = false) const;

  OdDb::UnitsValue getUnits(OdDb::UnitsValue unitDef = OdDb::kUnitsUndefined) const;
  OdDb::UnitsValue getPaperUnits() const;
  OdUInt32 underlayLayerCount() const;
  OdUnderlayLayer* underlayLayer(int index) const;
  OdResult getUnderlayLayer(int idx, OdUnderlayLayer& layer) const;
  void getExtents(OdGePoint2d& min, OdGePoint2d& max) const; // via bounds (a bit more than real)
  const OdGeMatrix3d& getMatrix() const;

  void viewportDraw(OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const;
  ODCOLORREF computeShadedColor(ODCOLORREF color) const;

protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
  virtual void subViewportDraw(OdGiViewportDraw* vd) const;
  
  virtual bool loadPaperImage(OdStreamBuf* pStream);
  void drawPaperImage(OdGiViewportDraw* pVd) const; // draw in underlay too
  void drawPaper(OdGiCommonDraw* pCd) const;
};

typedef OdSmartPtr<OdDwfLayoutView> OdDwfLayoutViewPtr;

class DWFDBEXPORT OdDwfSegment : public OdDwfObjectImpl
{
protected:
  OdString m_sName; friend class DWF_TK_Open_Segment;
  class OdDwfObjectContainer m_idsEntities; friend class OdDwfViewLoadContext;
  // OdGiMaterialColor    OdGiMaterialTraitsTaker : public OdGiMaterialTraits  contains struct OdGiMaterialTraitsData
  OdGiMaterialTraitsData m_dataMaterial; friend class DWF_TK_Color;
  mutable OdDwfObjectId m_idMaterial; // via all m_dataMaterial in database
  OdCmEntityColor m_color; friend class DWF_TK_Color_RGB;
  //int m_colorGeometryMask;
  enum OdDb::LineWeight m_lw; friend class DWF_TK_Size;
  OdGeMatrix3d m_matrix; friend class DWF_TK_Matrix;
  OdGiTextStyle m_textStyle; friend class DWF_TK_Text_Font; friend class DWF_TK_Text;
  OdDwfObjectId m_idParent;

  OdDwfSegment();
public:
  ODRX_DECLARE_MEMBERS(OdDwfSegment);

  OdString getName() const;
  OdDwfObjectId getMaterialId() const;
protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const; 
};

typedef OdSmartPtr<OdDwfSegment> OdDwfSegmentPtr;

class DWFDBEXPORT OdDwfSegmentReference : public OdDwfEntity
{
protected:
  OdDwfObjectId m_idSegment; friend class DWF_TK_Referenced_Segment;

  OdDwfSegmentReference();
public:
  ODRX_DECLARE_MEMBERS(OdDwfSegmentReference);

protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfSegmentReference> OdDwfSegmentReferencePtr;

class DWFDBEXPORT OdDwfShell : public OdDwfEntity
{
protected:
  mutable OdGeShell m_shell; friend class DWF_TK_Shell;

  OdDwfShell();
public:
  ODRX_DECLARE_MEMBERS(OdDwfShell);

protected:
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfShell> OdDwfShellPtr;

class DWFDBEXPORT OdDwfLine : public OdDwfEntity
{
protected:
  mutable OdGeLineSeg3d m_line; friend class DWF_TK_Line;

  OdDwfLine();
public:
  ODRX_DECLARE_MEMBERS(OdDwfLine);

protected:
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfLine> OdDwfLinePtr;

class DWFDBEXPORT OdDwfPolypoint : public OdDwfEntity
{
protected:
  //mutable OdGePolyline3d m_polyline; friend class DWF_TK_Polypoint;
  OdGePoint3dArray m_points; friend class DWF_TK_Polypoint;
  bool m_isPolygon;

  OdDwfPolypoint();
public:
  ODRX_DECLARE_MEMBERS(OdDwfPolypoint);

protected:
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfPolypoint> OdDwfPolypointPtr;


class DWFDBEXPORT OdDwfCircArc : public OdDwfEntity
{
protected:
  mutable OdGeCircArc3d m_arc; friend class DWF_TK_Circle;

  OdDwfCircArc();
public:
  ODRX_DECLARE_MEMBERS(OdDwfCircArc);

protected:
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfCircArc> OdDwfCircArcPtr;

class DWFDBEXPORT OdDwfNurbCurve : public OdDwfEntity
{
protected:
  mutable OdGeNurbCurve3d m_nurbsCurve; friend class DWF_TK_NURBS_Curve;

  OdDwfNurbCurve();
public:
  ODRX_DECLARE_MEMBERS(OdDwfNurbCurve);

protected:
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfNurbCurve> OdDwfNurbCurvePtr;

class DWFDBEXPORT OdDwfEllipse : public OdDwfEntity
{
protected:
  mutable OdGeEllipArc3d m_ellipse; friend class DWF_TK_Ellipse;

  OdDwfEllipse();
public:
  ODRX_DECLARE_MEMBERS(OdDwfEllipse);

protected:
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfEllipse> OdDwfEllipsePtr;

class DWFDBEXPORT OdDwfText : public OdDwfEntity
{
protected:
  mutable OdGiTextStyle m_textStyle; friend class DWF_TK_Text;
  OdGePoint3d m_position;
  OdString m_strText;

  OdDwfText();
public:
  ODRX_DECLARE_MEMBERS(OdDwfText);

protected:
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfText> OdDwfTextPtr;

class OdDwfViewport : public OdDwfEntity
{
protected:
  OdGiClipBoundary m_clipBoundary; friend class DwfStreamParserW2D;
  OdDwfObjectContainer m_idsEntities;

  OdGsView* m_pGsView; //friend class OdDwfLayoutView;
  OdDwfViewport();
public:
  ODRX_DECLARE_MEMBERS(OdDwfViewport);

  virtual OdDwfObjectId getDbViewId() const;

  OdGsView* gsView() const;
  void setGsView(OdGsView* pGsView);

  virtual void subViewportDraw(OdGiViewportDraw* vd) const;
protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
};

typedef OdSmartPtr<OdDwfViewport> OdDwfViewportPtr;

class DWFDBEXPORT OdDwfEntity2d : public OdDwfEntity
{
public:
  typedef enum {
    kPolyline,
    kPolygon,
    kEllipArc,
    kHatch,
    kImage,
    kShell,
    kText
  } TDwfGeomType2d;
  class DwfGeom2d
  {
  public:
    virtual ~DwfGeom2d() {} // fix memleak 
    virtual TDwfGeomType2d type() const = 0;
    virtual void subViewportDraw(OdGiViewportDraw* vd, const OdDwfEntity2d* pEnt2d) const = 0;
  };
protected:
  ODCOLORREF m_color; friend class DwfStreamParserW2D;
  OdDb::LineWeight m_lineWeight;
  OdDwfObjectId m_lineTypeId;
  OdGeExtents3d m_bounds; // should be transformed

  OdInt32 m_layerKey;
  std::map<int, OdSharedPtr<OdUnderlayLayer> >* m_pLayers; // by m_layerKey
  //mutable OdDwfObjectId m_layerId; 

  OdArray<OdSharedPtr<DwfGeom2d> > m_geoms;

  OdDwfEntity2d();
public:
  ODRX_DECLARE_MEMBERS(OdDwfEntity2d);

  bool isVisibleByLayer() const;
  ODCOLORREF computeShadedColor(ODCOLORREF color) const;

protected:
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* traits) const;
  virtual bool subWorldDraw(OdGiWorldDraw* wd) const;
  virtual void subViewportDraw(OdGiViewportDraw* vd) const;

  void addPolyline(WT_Rendition& rendition, const OdGePoint3d *pPoints, OdUInt32 nPoints);
  void addEllipArc(WT_Rendition& rendition, const OdGeEllipArc3d& ellipse);
  bool addHatch(WT_Rendition& rendition, 
                const OdGePoint2dArrayArray& vertexPtsArray, 
                const OdGeDoubleArray& vertexBulges); // for vertexPtsArray[0] only
  void addImage(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                OdGiRasterImagePtr pImage, const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts);
  void addPolygon(OdInt32 numVertices, const OdGePoint3d* pVertexList);
  void addShell(WT_Rendition& rendition,
                OdInt32 numVertices, const OdGePoint3d* vertexList, 
                OdInt32 faceListSize, const OdInt32* faceList);
  bool addText(WT_Rendition& rendition, WT_Text& text);
};

typedef OdSmartPtr<OdDwfEntity2d> OdDwfEntity2dPtr;

#endif // _DWFOBJECTIMPL_H_
