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

// GiBaseVectorizer.h: interface for the OdGiBaseVectorizer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ODGIBASEVECTORIZER_INCLUDED_
#define _ODGIBASEVECTORIZER_INCLUDED_

#include "Gi/GiDrawImpl.h"
#include "Gs/Gs.h"
#include "Ge/GeExtents3d.h"
#include "Gi/GiViewport.h"
#include "Gi/GiXform.h"
#include "Gi/GiModelToViewProc.h"
#include "Gi/GiConveyorEntryPoint.h"
#include "Gi/GiConveyorConnector.h"
#include "Gi/GiDeviation.h"
#include "Ge/GePlane.h"
#include "Gi/GiDgLinetype.h"
#include "Gi/GiOrthoClipper.h"
#include "Gi/GiExtAccum.h"
#include "Gi/GiSubEntityTraitsData.h"
#include "Gi/GiTextStyle.h"
#include "Gi/GiLayerTraitsData.h"
#include "Gi/GiMaterialTraitsData.h"
#include "Gi/GiTraitsCache.h"
#include "Gi/GiDummyGeometry.h"

class OdGiVisualStyle;

class OdGiBaseVectorizer;

class OdDbStub;

typedef OdSmartPtr<OdGiFill> OdGiFillPtr;

#include "TD_PackPush.h"

// Enable traits optimization
#define ENABLE_GITRAITSOPTIMIZATION

/** \details
    This class provides an implementation of the OdGiWorldDraw::geometry() function.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT ODRX_ABSTRACT OdGiWorldDrawImpl : public OdGiDummyCommonDraw<OdGiWorldDraw_>
                                                  , public OdGiSubEntityTraits
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdGiDummyCommonDraw<OdGiWorldDraw_>);

  OdGiSubEntityTraitsData m_entityTraitsData;
  OdGiContext*            m_pContext;

  OdGiWorldDrawImpl();
public:
  /** \details
    Sets the OdGiContext instance associated with this object.

    \param pUserContext [in]  Pointer to the user context.
  */
  virtual void setContext(OdGiContext* pUserContext);
  static OdGiContext* dummyGiContext();

  OdUInt16 color() const;
  OdCmEntityColor trueColor() const;
  OdDbStub* layer() const;
  OdDbStub* lineType() const;
  OdGiFillType fillType() const;
  OdDb::LineWeight lineWeight() const;
  double lineTypeScale() const;
  double thickness() const;
  OdDb::PlotStyleNameType plotStyleNameType() const;
  OdDbStub* plotStyleNameId() const;
  OdDbStub* material() const;
  const OdGiMapper* mapper() const;
  OdDbStub* visualStyle() const;
  OdCmTransparency transparency() const;
  OdUInt32 drawFlags() const;
  bool selectionGeom() const;
  ShadowFlags shadowFlags() const;
  bool sectionable() const;
  SelectionFlags selectionFlags() const;
  OdCmEntityColor secondaryTrueColor() const;
  const OdGiDgLinetypeModifiers* lineStyleModifiers() const;
  const OdGiFill* fill() const;

  void setTrueColor(const OdCmEntityColor& color);
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId = 0);
  void setColor(OdUInt16 color);
  void setLayer(OdDbStub* layerId);
  void setLineType(OdDbStub* lineTypeId);
  void setFillType(OdGiFillType fillType);
  void setLineWeight(OdDb::LineWeight lineWeight);
  void setLineTypeScale(double lineTypeScale);
  void setThickness(double thickness);
  void setSelectionMarker(OdGsMarker selectionMarker);
  void setMaterial(OdDbStub* pMaterialId);
  void setMapper(const OdGiMapper *pMapper);
  void setVisualStyle(const OdDbStub* visualStyleId);
  void setTransparency(const OdCmTransparency &transparency);
  void setDrawFlags(OdUInt32 drawFlags);
  void setSelectionGeom(bool bSelectionFlag);
  void setShadowFlags(ShadowFlags shadowFlags);
  void setSectionable(bool bSectionableFlag);
  void setSelectionFlags(SelectionFlags selectionFlags);
  void setSecondaryTrueColor(const OdCmEntityColor& color);
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod);
  void setFill(const OdGiFill* pFill);

  /////////////////////////////////////////////////////////////////////////////
  // OdGiCommonDraw Overrides

  OdGiContext* context() const;

  OdGiSubEntityTraits& subEntityTraits() const;

  OdGiGeometry& rawGeometry() const;

  OdUInt32 numberOfIsolines() const;
};

/** \details
    This class is a base implementation of the OdGiWorldDraw and OdGiViewportDraw interfaces.
    Corresponding C++ library: TD_Gi    
  
    \remarks
    This class takes a drawable graphics and passes it into OdGiConveyorGeometry.
    
    It is the base class for specialized frameworks such as OdGiExtCalc.
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiBaseVectorizer : public OdGiWorldDrawImpl
                                     , public OdGiViewportDraw_
                                     , public OdGiConveyorContext
                                     , public OdGiDeviation 
                                     , public OdGiSubEntityTraitsChangedFlags
{
protected:
  ODRX_HEAP_OPERATORS();

  mutable OdGiSubEntityTraitsData m_effectiveEntityTraitsData;

  ODGI_EXPORT_STATIC static const OdGiSubEntityTraitsData g_entityTraitsDataReset;

  mutable OdDbStub*               m_layerId;
  mutable OdGiLayerTraitsTaker    m_effectiveLayerTraits;
  struct LayerTraitsCache : public OdGiLayerTraitsData
  { OdDbStub* m_effectiveLayerId;
    LayerTraitsCache() : m_effectiveLayerId(NULL) {} };
  typedef std::map<OdDbStub*, LayerTraitsCache> LayerTraitsCacheMap;
  mutable LayerTraitsCacheMap m_layerTraitsCache;

  typedef OdGiTraitsCache<OdGiMaterialTraitsData, OdGiMaterialTraitsTaker, OdGiMaterialTraits::kByBlock | OdGiMaterialTraits::kByLayer> OdGiMaterialTraitsCache;
  mutable OdDbStub*               m_materialId;
  mutable OdDbStub*               m_byLayerMaterialId;
  mutable OdDbStub*               m_byBlockMaterialId;
  mutable OdUInt32                m_materialAttributes;
  mutable OdGiMaterialTraitsCache m_effectiveMaterialTraitsData;
  mutable OdDbStub*               m_effectiveMaterialId;
  void actualizeMaterialAttributes(OdDbStub* materialId) const;

  virtual OdDbStub*               switchLayer(OdDbStub* layerId) const;
  virtual const OdGiLayerTraitsData& effectiveLayerTraits() const;
  mutable OdDbStub*               m_effectiveLayerId;

  OdGsMarker                      m_nSelectionMarker;
  OdGsMarker                      m_nPrevSelectionMarker;

  OdGiMapper                      m_materialMapper;
  OdGiMapper*                     m_pCurMaterialMapper;

  OdGiDgLinetypeModifiers         m_lsModifiers;
  OdGiDgLinetypeModifiers*        m_pCurLSModifiers;

  OdGiFillPtr                     m_pFill;

  virtual bool layerVisible(OdDbStub* layerId) const;
  /** \details
    Updates the layer traits for this Vectorizer object from the specified subentity traits.
    \param subEntityTraits [in]  Subentity traits.
  */
  void updateLayerTraits(OdGiSubEntityTraitsData& subEntityTraits) const;
  
public:
  /** \details
    Returns true if and only if the EntityTraitsData for this Vectorizer object have been changed.
  */
  bool isEntityTraitsDataChanged(int bit = kSomeChanged) const { return GETBIT(m_entityTraitsDataChanged, bit); }

  /** \details
    Marks as changed the EntityTraitsData data for this Vectorizer object.
  */
  virtual void setEntityTraitsDataChanged() { m_entityTraitsDataChanged = kAllChanged; }
  /** \details
    Marks as changed the EntityTraitsData data for this Vectorizer object.
  */
  virtual void setEntityTraitsDataChanged(int bit, bool value = true) { SETBIT(m_entityTraitsDataChanged, bit, value); }
  
  /** \details
    Marks as unchanged the EntityTraitsData data for this Vectorizer object.
  */
  void clearEntityTraitsDataChanged() const { m_entityTraitsDataChanged = 0; }

  /** \details
    Returns true if and only if the layer of this Vectorizer object is effectively visible.
    
    \remarks
    This function returns:
    
                    (GETBIT(m_flags, kDrawLayerOff   ) || !traits.isLayerOff()) &&
                    (GETBIT(m_flags, kDrawLayerFrozen) || !traits.isLayerFrozen())
  */
  bool effectivelyVisible() const
  {
    const OdGiSubEntityTraitsData& traits = effectiveTraits();
    return (
      (GETBIT(m_flags, kDrawLayerOff   ) || !traits.isLayerOff()) &&
      (GETBIT(m_flags, kDrawLayerFrozen) || !traits.isLayerFrozen())
    );
  }  

public:
  /////////////////////////////////////////////////////////////////////////////
  // OdGiConveyorContext Overrides

  OdGiContext& giContext() const;
  const OdGiDrawableDesc* currentDrawableDesc() const;
  const OdGiDrawable* currentDrawable() const;
  const OdGiViewport* giViewport() const;
  const OdGsView* gsView() const;
  
  /////////////////////////////////////////////////////////////////////////////

private:
  // Data Members

  OdGiConveyorEntryPoint          m_dcEntryPoint;
  OdGiTextStyle                   m_textStyle;
  OdGePlane                       m_primitivePlane;
  OdGeVector3d                    m_extrusion;
  OdGiXformPtr                    m_pDcInputToOutput;
#ifdef ENABLE_GITRAITSOPTIMIZATION
  bool                            m_bResetEntityTraitsData; //reset them before setAttributes()
#endif
protected:
  OdRxObjectPtr                   m_pGiReserved;
  mutable OdUInt32                m_entityTraitsDataChanged;
  OdGiConveyorConnector           m_eyeEntryPoint;

  /** \details
    Returns an extrusion vector normal to the specified plane.
    
    \param firstPoint [in]  First point in the plane.
    \param secondPoint [in]  Second point in the plane.
    \param thirdPoint [in]  Third point in the plane.
    \param origin [in]  Origin of the plane.
    \param u [in]  U-vector in the plane.
    \param v [in]  V-vector in the plane.
    \param normal [in]  Normal to the plane.
    \param pNormal [in]  Pointer to the normal to the plane.
    \remarks
    The magnitude of the extrusion vector is effectiveTraits().thickness().
  */
  const OdGeVector3d* extrusion(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint);
  const OdGeVector3d* extrusion(const OdGeVector3d& normal);
  const OdGeVector3d* extrusion(const OdGeVector3d* pNormal)
  {
    if(pNormal)
      return extrusion(*pNormal);
    return 0;
  }
  const OdGeVector3d* extrusion(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v);

  OdGiConveyorConnector           m_modelEntryPoint;
  OdGiConveyorConnector*          m_pActiveEntryPoint;

  OdGiModelToViewProcPtr          m_pModelToEyeProc;
  OdGiConveyorConnector           m_output;

public:
  OdGiDrawableDesc*               m_pDrawableDesc;
  OdUInt32                        m_nDrawableAttributes;
  const OdGiDrawableDesc *drawableDesc() const { return m_pDrawableDesc; }
protected:

  enum Flags
  {
    kFirstFlag            = 1,

    kSuppressViewportDraw = kFirstFlag << 0,
    kDrawInvisibleEnts    = kFirstFlag << 1,
    kDrawLayerOff         = kFirstFlag << 2,
    kDrawLayerFrozen      = kFirstFlag << 3,
    kIgnoreFillPlane      = kFirstFlag << 4,
    kSelectionMode        = kFirstFlag << 5,
    kAttributesSegment    = kFirstFlag << 6,
    kXrefOverride         = kFirstFlag << 7,

    kLastFlag             = kXrefOverride
  };
  mutable OdUInt32                m_flags;
  OdUInt32                        m_drawContextFlags;

  const OdGiSubEntityTraitsData*  m_pByBlock;

  /** \details
    Sets the specfied EntityTraitsData data from the specified EntityTraitsData.
    
    \param pSource [in]  Pointer to the source SubEntityTraitsData.
    \param destination [out]  Receives the SubEntityTraitsData.
    
    \remarks
    If pSource is NULL, default SubEntityTraitsData is used.
    
  */
  virtual void affectTraits(const OdGiSubEntityTraitsData* pSource, OdGiSubEntityTraitsData& destination) const;

  /** \details
    Returns the dash length below which linetypes are not generated by this Vectorizer object.
  */
  virtual double linetypeGenerationCriteria() const;

#ifdef ENABLE_GITRAITSOPTIMIZATION
  inline void resetEntityTraitsData();
#endif
public:
  /////////////////////////////////////////////////////////////////////////////
  // OdGiConveyorContext Override

  /** \details
    Returns the EntityTraitsData in effect for this Vectorization object.
  */
  virtual const OdGiSubEntityTraitsData& effectiveTraits() const;

  virtual void setEffectiveTraits(const OdGiSubEntityTraitsData& traits,
                                  const OdGeVector3d* fillNormal = NULL);

  /////////////////////////////////////////////////////////////////////////////
protected:

  /** \details
    Returns the a pointer to the OdRxClass object that contains a description
    of this object type, for objects of known class type.

    \remarks
    This function is for use only when the class type of this object is known.
    If the class type of this object is unknown, use instead isA().
  */
  static OdRxClass* desc() { return ::OdRxObject::desc(); }

  /** \details
    Called when selection marker is being to be changed.

    \param nSelectionMarker [in]  New selection marker.

    \remarks
    Previous selection marker accessible through selectionMarker method inside override.
    This method should be called for base class from override to change selection marker.
  */
  virtual void selectionMarkerOnChange(OdGsMarker nSelectionMarker);

public:
  OdGiBaseVectorizer();
  virtual ~OdGiBaseVectorizer();

  /** \details
    Returns the a pointer to the OdRxClass object that contains a description
    of this object type, for objects of unknown class type.
    
    \remarks
    This function may be used only when the class type of this object is unknown. 
    If the class type of this object is known, use instead desc().
  */
  OdRxClass* isA() const;

  /** \details
     Returns the Protocol Extension object for this object.
     \remarks
     Returns NULL if no protocol extension class exists.
  */
  OdRxObject* queryX(const OdRxClass* pProtocolClass) const;

  /** \details
    Returns a reference to the output of this Vectorizer object.
  */
  virtual OdGiConveyorOutput& output();

  /** \details
    Sets the Eye to Output transformation matrix for this Vectorizer object.
    \param xfm [in]  Transformation matrix.
  */
  void setEyeToOutputTransform(const OdGeMatrix3d& xfm);

  /** \details
    Returns the Eye to Output transformation matrix for this Vectorizer object.
  */
  const OdGeMatrix3d& eyeToOutputTransform() const;
  
  /** \details
    Returns the Eye to Output transformation matrix for this Vectorizer object.
    \param xfm [in]  Receives the transformation matrix.
  */
  void eyeToOutputTransform(OdGeMatrix3d& xfm) const;

  /** \details
    Returns the OdGiConveyorContext associated with this object.

    \remarks
    Since OdGiConveyorContext is a parent class, this function always returns "this".
  */
  OdGiConveyorContext* drawContext() { return this; }

  const OdGiConveyorContext* drawContext() const { return this; }

  /** \details
    Begins the ViewVectorization process for this Vectorizer object.
  */
  virtual void beginViewVectorization();

  /** \details
    Ends the ViewVectorization process for this Vectorizer object.
  */
  virtual void endViewVectorization();

  /////////////////////////////////////////////////////////////////////////////
  // OdGiSubEntityTraits Overrides

  void setTrueColor(const OdCmEntityColor& trueColor);
  /** \details
    Sets the Plot Style Name Type for this Vectorizer object.
    \param plotStyleNameType [in]  Plot Style Name Type.
    
    \remarks
    plotStyleNameType must be one of the following:
    
    <table>
    Name                                 Value                         
    OdDb::kPlotStyleNameByLayer          0
    OdDb::kPlotStyleNameByBlock          1
    OdDb::kPlotStyleNameIsDictDefault    2
    OdDb::kPlotStyleNameById             3
    </table>
  */
  void setPlotStyleNameType(OdDb::PlotStyleNameType plotStyleNameType);
  /** \details
    Sets the PlotStyleName for this Vectorizer object.
    \param plotStyleNameId [in]  Pointer to the Object ID of the plot style name.
  */
  void setPlotStyleNameId(OdDbStub* plotStyleNameId);
  void setColor(OdUInt16 color);
  void setLayer(OdDbStub* layerId);
  void setLineType(OdDbStub* lineTypeId);
  void setFillType(OdGiFillType fillType);
  void setLineWeight(OdDb::LineWeight lineWeight);
  void setLineTypeScale(double lineTypeScale);
  void setThickness(double thickness);

  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId= 0);
  void setSelectionMarker(OdGsMarker selectionMarker);
  void setMaterial(OdDbStub* pMaterialId);
  void setMapper(const OdGiMapper *pMapper);
  void setVisualStyle(const OdDbStub* pVisualStyleId);
  void setTransparency(const OdCmTransparency& transparency);
  void setDrawFlags(OdUInt32 drawFlags);
  void setSelectionGeom(bool bSelectionFlag);
  void setShadowFlags(ShadowFlags shadowFlags);
  void setSectionable(bool bSectionableFlag);
  void setSelectionFlags(SelectionFlags selectionFlags);

  void setSecondaryTrueColor(const OdCmEntityColor& color);
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod);

  void setFill(const OdGiFill* pFill);

  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // OdGiCommonDraw Overrides

  OdGiContext* context() const;
  bool regenAbort() const;
  OdGiSubEntityTraits& subEntityTraits() const;
  double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;
  OdGiRegenType regenType() const;
  OdUInt32 numberOfIsolines() const;
  OdGiGeometry& rawGeometry() const;
  bool isDragging() const;

  /////////////////////////////////////////////////////////////////////////////

  OdUInt32 drawContextFlags() const;
  void setDrawContextFlags(OdUInt32 flags, bool bFlag);

  /////////////////////////////////////////////////////////////////////////////
  // OdGiViewportDraw Override
  //

  /** \remarks
      The default implementation of this function always returns 0.
  */
  OdUInt32  sequenceNumber() const;

  /** \remarks
      The default implementation of this function always returns false.
  */
  bool isValidId(const OdUInt32 viewportId) const;

  /** \remarks
      The default implementation of this function always returns 0.
  */
  OdDbStub* viewportObjectId() const;

  OdGiViewport& viewport() const;

  /////////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////////
  // OdGiGeometry Overrides

  void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);

  void circle(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint);

  void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple);

  void circularArc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple);

  void polyline(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    OdGsMarker baseSubEntMarker = -1);

  void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList);
  void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal);

  void pline(const OdGiPolyline& polyline, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);

  /** \details
    Introduces a shape into this vectorization context.
    
    \param position [in]  Position of the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
    \param direction [in]  Baseline direction for the shape.
    \param normal [in]  Normal vector.
  */
  void shape(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    int shapeNumber, const OdGiTextStyle* pTextStyle);

  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    double height, double width, double oblique, const OdString& msg);

  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle);

  void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);

  void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint );

  void nurbs(const OdGeNurbCurve3d& nurbsCurve);

  void ellipArc(const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple);

  void mesh(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void worldLine(const OdGePoint3d points[2]);

  void image(const OdGiImageBGRA32& img,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit);

  void edge(const OdGiEdge2dArray& edges);

  void polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                 const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize);

  void rowOfDots(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  void pushClipBoundary(OdGiClipBoundary* pBoundary);
  void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  void drawClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  void popClipBoundary();
  bool isClipping() const;

  void pushModelTransform(const OdGeMatrix3d& xfm);
  void pushModelTransform(const OdGeVector3d& normal);
  void popModelTransform();

  OdGeMatrix3d getModelToWorldTransform() const;
  OdGeMatrix3d getWorldToModelTransform() const;

  bool isXrefOverride() const { return GETBIT(m_flags, kXrefOverride); }
  void setXrefOverride(bool bSet) { SETBIT(m_flags, kXrefOverride, bSet); }

  void draw(const OdGiDrawable* pDrawable);

  const OdGiPathNode* currentGiPath() const;

  /////////////////////////////////////////////////////////////////////////////

  /** \details
    Sets the default drawable traits for this Vectorizer object,
    and sets the drawable flags for the specified OdGiDrawable object.
    
    \param pDrawable [in]  Pointer to the Drawable object.
  */
  virtual OdUInt32 setAttributes(const OdGiDrawable* pDrawable);

  /** \details
    Returns the  drawable flags of the last vectorized OdGiDrawable object.
  */
  OdUInt32 drawableAttributes() const { return m_nDrawableAttributes; }

  /** \details
    Returnes true if and only the Drawable object associated with
    this Vectorizer is visible.
    \param drawableFlags [in]  Drawable flags.
    \remarks
    Called by draw().
  */
  bool needDraw(OdUInt32 drawableFlags);

  /** \details
    Vectorizes the specified OdGiDrawable object.
    \param drawableFlags [in]  Drawable flags.
    \param pDrawable [in]  Pointer to the Drawable object.
    
    \remarks
    Called by draw() after subSetAttributes().
    
    Returns pDrawable->subWorldDraw(this). If pDrawable->subWorldDraw returns false, 
    and kSuppressViewportDraw is not set, this function evokes pDrawable->subViewportDraw(this).
  */
  virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);

  /////////////////////////////////////////////////////////////////////////////
  // OdGiWorldGeometry Override

  void setExtents(const OdGePoint3d *newExtents);
  void startAttributesSegment();
  bool isAttributesSegmentEnabled();

  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // OdGiViewportGeometry

  void rasterImageDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage,
    const OdGePoint2d* uvBoundary,
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  void metafileDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool bDcAligned = true,
    bool bAllowClipping = false);

  void polylineEye(OdUInt32 numVertices, const OdGePoint3d* vertexList);
  void polygonEye(OdUInt32 numVertices, const OdGePoint3d* vertexList);

  void polylineDc(OdUInt32 numVertices, const OdGePoint3d* vertexList);
  void polygonDc(OdUInt32 numVertices, const OdGePoint3d* vertexList);


  /////////////////////////////////////////////////////////////////////////////
  // OdGiConveyorContext Overrides

  virtual void onTraitsModified();

  /////////////////////////////////////////////////////////////////////////////

  /** \details
    Returns the maximum ModelSpace (UCS) deviation for this VectorizeView object.
  */
  const OdGiDeviation& modelDeviation() const;
  /** \details
    Returns the maximum world (WCS) deviation for this VectorizeView object.
  */
  const OdGiDeviation& worldDeviation() const;
  /** \details
    Returns the maximum EyeSpace (ECS) deviation for this VectorizeView object.
  */
  const OdGiDeviation& eyeDeviation() const;

  // materials
  const OdGiMaterialTraitsData &effectiveMaterialTraitsData() const;
  void effectiveMaterialTraitsData(OdGiMaterialTraitsData &data) const;
  void setEffectiveMaterialTraitsData(OdDbStub *materialId, const OdGiMaterialTraitsData &data, bool bForce = false);

  /** \details
    Returns current selection marker state.
  */
  OdGsMarker selectionMarker() const;

  const OdGiSubEntityTraitsData *byBlockTraits() const { return m_pByBlock; }
  void setByBlockTraits(const OdGiSubEntityTraitsData *pByBlock) { m_pByBlock = pByBlock; }
  OdRxObjectPtr saveByBlockTraits();
};

/** \details
    This class is an implementation of the subWorldDraw() framework to calculate the WCS extents of an OdGiDrawable object.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiExtCalc : public OdGiBaseVectorizer
{
protected:
  bool            m_BBoxSet;
  OdGiExtAccumPtr m_pExtAccum;

public:

  OdGiExtCalc();
  virtual ~OdGiExtCalc();

  OdGiRegenType regenType() const;
  bool regenAbort() const;
  void draw(const OdGiDrawable* pDrawable);
  void setExtents(const OdGePoint3d *newExtents);
  /** \details
    Resets the accumulated extents for the OdGiDrawable object associated with this Vectorization object.
  */
  void resetExtents();

  /** \details
    Returns the accumulated extents for the OdGiDrawable object associated with this Vectorization object.
    \param extents [out]  Receives the extents.
  */
  void getExtents(OdGeExtents3d& extents) const;

  void setContext(OdGiContext* pUserContext);

  // optimization

  const OdGiSubEntityTraitsData& effectiveTraits() const;
  void affectTraits(const OdGiSubEntityTraitsData* pSource, OdGiSubEntityTraitsData& destination) const;
};

#ifdef ENABLE_GITRAITSOPTIMIZATION
inline void OdGiBaseVectorizer::resetEntityTraitsData()
{
  m_bResetEntityTraitsData = false;
  m_entityTraitsData = g_entityTraitsDataReset;
  setEntityTraitsDataChanged();
}
#endif

ODGI_EXPORT void odgiCalculateTextBasis(OdGeVector3d& u, OdGeVector3d& v,
                                        const OdGeVector3d& normal,
                                        const OdGeVector3d& direction,
                                        double height,
                                        double width,
                                        double oblique,
                                        bool bMirrorX = false,
                                        bool bMirrorY = false);
ODGI_EXPORT const OdGiTextStyle* odgiPrepareTextStyle(const OdGiTextStyle* pStyle, OdGiTextStyle& res);

#include "TD_PackPop.h"

#endif // #ifndef _ODGIBASEVECTORIZER_INCLUDED_
