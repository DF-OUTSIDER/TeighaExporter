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

#ifndef __OD_GS_BASE_VECTORIZER_H_
#define __OD_GS_BASE_VECTORIZER_H_

#include "TD_PackPush.h"

#include "Gs/GsExport.h"
#include "Gs/GiBaseVectorizerImpl.h"
#include "Gs/GsExtAccum.h"
#include "Gs/GsModel.h"
#include "GsMarkerArray.h"
#include "Gs/GsHighlightData.h"

class OdGsBaseModel;
class OdGsLayerNode;
class OdGsUpdateContext;
class OdGiSelectProc;
class OdGiCollideProc;
class OdGsEntityNode;
class OdGsBlockReferenceNode;
class OdGsBlockReferenceNodeImpl;
struct OdGsGeomPortion;
class OdGsMaterialNode;
class OdGsBaseVectorizer;

class OdGsMtContext;
class OdGsUpdateState;
class OdGiSharedRefDesc;
class OdGiHistory;
class OdGiSectionGeometry;

class OdPerfTimerBase;

/** \details
            
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsWriter
{
public:
  OdGsWriter();

  /** \details
    Returns the accumulated extents when writing to the object.
  */
  OdGsExtAccum& extentsAccum()
  {
    return *m_pExtAccum.get();
  }
  OdGsBaseModel* gsModel() const { return m_pGsModel; }
  
  bool isRecordingMetafile() { return m_pGeomPortion != 0; }
  OdGsGeomPortion *currentGeomPortion() const { return m_pGeomPortion; }
  
  void beginMetafileRecording(OdGsGeomPortion* pGeomPortion) { m_pGeomPortion = pGeomPortion; }
  void endMetafileRecording();
  void onNestedDrawable(OdDbStub* layerId);
  void separateMetafile();

  bool isLayerFrozen(OdGsLayerNode *pLayerNode) const;
private:
  friend class OdGsBaseVectorizer;
  void set(OdGsBaseVectorizer* v) { m_vectorizer = v; }
  void onLayerModified(OdDbStub* layerId);
  void onLayerModified(OdDbStub* layerId, bool bFrozen);
  void onFrozenLayerModified(OdDbStub* layerId) const;

public:
  OdGsBaseModel* m_pGsModel;

protected:
  OdGsExtAccumPtr m_pExtAccum;
  OdGsGeomPortion* m_pGeomPortion;
  OdGsBaseVectorizer* m_vectorizer;
};

class OdGiPointLightTraitsData;
class OdGiSpotLightTraitsData;
class OdGiDistantLightTraitsData;
class OdGiWebLightTraitsData;

/** \details
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseVectorizer: public OdGiBaseVectorizerImpl
{
public:
  OdGsBaseVectorizer();

protected:
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

public:  
#ifdef SWIG
	virtual void draw(const OdGiDrawable* pDrawable) {draw(pDrawable);};
#endif
  /** OdGsBaseVectorizer methods *
  */
  OdGsWriter& gsWriter() { return m_gsWriter; }
  const OdGsWriter& gsWriter() const { return m_gsWriter; }
  OdGsExtAccum& gsExtentsAccum() { return gsWriter().extentsAccum(); }

  /** \details
    Creates a new GsMetafile (cache) object.
    \returns
    Returns a SmartPointer to the newly created object.
    
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to a display device. 

    \sa
    * beginMetafile
    * endMetafile
    * playMetafile    
  */
  virtual OdRxObjectPtr newGsMetafile();
  /** \details
    Begins vectorization to the specified GsMetafile (cache) object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \remarks
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to a display device. 
    
    \sa
    * endMetafile
    * newGsMetafile
    * playMetafile    
  */
  virtual void beginMetafile(OdRxObject* pMetafile);
  /** \details
    Ends vectorization to the specified GsMetafile (cache) object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \remarks
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to a display device. 
    
    \sa
    * beginMetafile    
    * newGsMetafile
    * playMetafile    
  */
  virtual void endMetafile(OdRxObject* pMetafile);

  /** \details
    Plays (redraws) the specified Metafile (cache) object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \remarks
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to a display device. 
    \sa
    * beginMetafile
    * endMetafile
    * newGsMetafile    
  */
  virtual void playMetafile(const OdRxObject* pMetafile);

  /** \details
    Stores the specified Metafile (cache) into OdGsFiler object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \param pFiler [in]  Pointer to the OdGsFiler object.
  */
  virtual bool saveMetafile(const OdRxObject* pMetafile, OdGsFiler *pFiler);
  /** \details
    Loads the Metafile (cache) from OdGsFiler object.
    \param pFiler [in]  Pointer to the OdGsFiler object.
  */
  virtual OdRxObjectPtr loadMetafile(OdGsFiler *pFiler);

  /** \details
	This method can be called right after endMetafile() to check whether
	last recorded metafile was empty and optimize stored data.
  */
  bool isMetafileEmpty() const { return m_nMetafileSize == 0; }
  
  /** \details
    This method can be called right after endMetafile() to obtain
    metafile size for Update Manager filtration
  */
  OdUInt64 lastMetafileSize() const 
  {
    if( m_nMetafileSize != OdUInt64( -1 ) ) return m_nMetafileSize;
    return 0;
  }

  /** \details
    Sets viewport's common data before display() call.
  */
  virtual void loadViewport();

  virtual bool forceMetafilesDependence() const;

  virtual bool isViewRegenerated() const;

  /** \details
    Draws the frame for the viewport.
  */
  virtual void drawViewportFrame();
  virtual void updateViewport();

  // Materials support
  virtual void processMaterialNode(OdDbStub *materialId, OdGsMaterialNode *pNode);
  virtual bool saveMaterialCache(const OdGsMaterialNode *pNode, OdGsFiler *pFiler);
  virtual bool loadMaterialCache(OdGsMaterialNode *pNode, OdGsFiler *pFiler);

  virtual void addPointLight(const OdGiPointLightTraitsData&) {}
  virtual void addSpotLight(const OdGiSpotLightTraitsData&) {}
  virtual void addDistantLight(const OdGiDistantLightTraitsData&) {}
  virtual void addWebLight(const OdGiWebLightTraitsData&); 
  // default implementation call's addPointLight

  ~OdGsBaseVectorizer();

  /** OdGiGeometry Overrides *
  */
  void pushModelTransform(const OdGeMatrix3d& xfm);
  void pushModelTransform(const OdGeVector3d& normal);
  void popModelTransform();

  void pushClipBoundary(OdGiClipBoundary* pBoundary);
  void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  void popClipBoundary();

  /** OdGiBaseVectorizer Overrides *
  */
  virtual void setEntityTraitsDataChanged();
  virtual void setEntityTraitsDataChanged(int bit, bool value = true);

  virtual OdGeMatrix3d objectToDeviceMatrix() const;

  /** OdGsBaseVectorizer methods *
  */
  bool disableInfiniteGeomExtents() const { return GETBIT(m_flags, kDisableInfiniteGeomExtents); }
  bool sectionableGeomExtentsOnly() const { return GETBIT(m_flags, kSectionableGeomExtentsOnly); }
  void setSectionableGeomExtentsOnly(bool bOn) { SETBIT(m_flags, kSectionableGeomExtentsOnly, bOn); }

  /** \details
	This method sets internal state flag indicating playing mode and calls
	playMetafile(). The flag can be accessed via isPlayingMfAsGeometry(),
	see below.
  */
  void playMetafileMode(const OdRxObject* pMetafile, EMetafilePlayMode eMode);
  /** \details
	This method returns internal state indicating metafile playing mode.
  */
  EMetafilePlayMode metafilePlayMode() const { return m_eMfPlayMode;  }

  enum MetafileTransformFlags
  {
    kSharedRefTransform = (1 << 0),
    kSharedRefUpdate    = (1 << 1),
    kSharedRefSelect    = (1 << 2)
  };
  virtual void pushMetafileTransform(const OdGeMatrix3d&, OdUInt32 = 0){}
  virtual void popMetafileTransform(OdUInt32 = 0){}

  virtual bool useSharedBlockReferences() const { return true; }
  virtual bool useMetafileAsGeometry() const { return false; }
  virtual OdGiConveyorOutput& outputForMetafileGeometry();
  virtual void setTransformForMetafileGeometry(const OdGeMatrix3d&) {}
  virtual OdGeMatrix3d getTransformForMetafileGeometry() const { return OdGeMatrix3d::kIdentity; }
  virtual void reportUpdateError(OdDbStub* /*entityId*/, const OdError& error)
  {
#ifdef ODA_DIAGNOSTICS
    throw error;
#else
    ODA_ASSERT(error); // prevent arg unused warning
#endif
  }

  OdGsLayerNode* gsLayerNode(OdDbStub* layerId, OdGsBaseModel* pModel);

  /** \details
	This method returns internal state indicating color fading mode.
  */
  inline bool isFaded() const;
  inline OdUInt32 fadingIntensity() const;

  inline bool isHighlighted() const;

  inline bool isSelecting() const;

  bool isDragging() const;

  virtual OdGiConveyorOutput& gsExtentsOutput() { return secondaryOutput(); }

  /** \details
    Controls analytic linetype support for circles and circular arcs in this VectorizeView object. 

    \param analytic [in]  Analytic linetype support.

    \sa
    Analytic Linetyping
  */
  virtual void setAnalyticLinetypingCircles(bool analytic);

  /** \details
    Returns true if and only if analytic linetype support for circles and circular arcs is
    enabled in this VectorizeView object.

    \sa
    Analytic Linetyping
  */
  virtual bool isAnalyticLinetypingCircles() const;
  
  /** \details
    Controls analytic linetype support for ellipses, elliptical curves, 
    and NURBS curves in this VectorizeView object. 

    \param analytic [in]  Analytic linetype support.
    \sa
    Analytic Linetyping
  */
  virtual void setAnalyticLinetypingComplexCurves(bool analytic);

  /** \details
    Returns true if and only if analytic linetype support for ellipses, elliptical curves, 
    and NURBS curves is enabled in this VectorizeView object.

    \sa
    Analytic Linetyping
  */
  virtual bool isAnalyticLinetypingComplexCurves() const;

//#ifdef GS_INEXACT_SELECTION
  void checkSelection();
//#endif

  virtual bool displayViewportProperties(OdGsPropertiesDirectRenderOutput *pdro = NULL,
      OdUInt32 incFlags = OdGsProperties::kAll);

  /** \details
    Returns the secondary output from this vectorizer object.
  */
  OdGiConveyorOutput& secondaryOutput();

  // OdGiBaseVectorizerImpl methods
  virtual void setUp(OdGsViewImpl& view);

  // OdGiBaseVectorizer methods
  virtual void beginViewVectorization();
  virtual void endViewVectorization();
  virtual void onTraitsModified();
  virtual bool regenAbort() const;
  virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);
  virtual void setSelectionMarker(OdGsMarker selectionMarker);
  virtual OdGiConveyorOutput& output();

  /** \details
    This method can be called on each Visual Style displaying. It may be used by inherited device
    for make own changes for Visual Style support.
    \param visualStyle [in]  Reference onto current Visual Style state.
  */
  virtual void setVisualStyle(const OdGiVisualStyle& visualStyle);

  const OdGsMarkerArray* highlightedSubentities() const;
  OdGsHlBranch* findHighlightedSubnodeBranch();
  OdUInt32 threadIndex() const;

  void highlight(bool bHighlight);
  /** \details
    Returns current highlighting data.
  */
  const OdGsHlBranch* currentHighlightBranch() const;

  OdGsLayerNode *activeLayerNode(bool bSync = false) const;

  OdGsModel::RenderType activeRenderType() const;
  OdGsOverlayId activeOverlay() const;
protected:
  void doSelect(const OdGePoint2d* aPtDc, int numPoints,
                OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode);
  bool doViewExtents(OdGeBoundBlock3d& extents);
  /** \details
    Paints the display with each of the drawable objects associated with this VectorizeView object.
    
    \param update [in]  If and only if true, the cache is updated before displaying the drawable.
     
    \remarks
    The display is painted for each drawable from one of the following methods in the following order.
    
    1.  Model cache. 
    2.  The metafile for the drawable.
    3.  The drawable itself.
  */
  virtual void display(bool bUpdate);
  /** \details
    Recomputes the extents for this VectorizeView object.
    
    \param buildCache [in]  Builds Metafile (cache) objects if and only if true.
  */
  virtual void updateExtents(bool bBuildCache);

  // OdGiBaseVectorizer methods
  virtual const OdGiLayerTraitsData& effectiveLayerTraits() const;
  virtual OdDbStub* switchLayer(OdDbStub* layerId) const;

  void selectionMarkerOnChange(OdGsMarker nSelectionMarker);

  void checkRenderType(const OdGsModel *pModel);
  virtual void renderTypeOnChange(OdGsModel::RenderType renderType);
  virtual void switchOverlay(OdGsOverlayId overlayId);

  void displayWithoutNesting(OdGsEntityNode *pNode);

public:
  void updateExtentsInThreadInit(OdGsMtContext& mtContext);

private:
  void setInitGsState(bool bOn);
  bool isRecordingHistory();
  void deleteHistory();
  void setThreadIndex(OdUInt32 idx) { m_threadIndex = idx; }
  bool drawSectionable(const OdGiDrawable& drawable, bool& bRes);
  bool drawSectionGeometry(OdGiSectionGeometry& geom, bool bVpModelTfAwareBefore);
  void setCurrentState(OdGsUpdateState& newState);
  OdGsUpdateState* currentState() { return m_curState; }
  const OdGsUpdateState* currentState() const { return m_curState; }
  void applyState(const OdGsUpdateState& s, const OdGsUpdateState& prev);

public:
  OdGsHlBranchPtr m_pCurHltBr;
protected:
  OdUInt64 m_nMetafileSize;
  OdGsWriter m_gsWriter;
  OdGiSelectProc* m_pSelectProc;
  OdGiCollideProc* m_pCollideProc;
  OdGiConveyorGeometry* m_pDetachedOutput;
  OdGiConveyorGeometry* m_pDetachedSecondaryOutput;
  EMetafilePlayMode m_eMfPlayMode;
  OdGsUpdateState* m_curState;
  OdGsModel::RenderType m_curRenderType;
  OdGsOverlayId m_curOverlay;
  OdIntPtr m_drawableFilterFunction;
  // Fading support
  enum FadingFlags
  {
    kFfLockedLayer       = (1 << 0),
    kFfXref              = (1 << 1),
    kFfRefEdit           = (1 << 2),
    kFfFlagsMask         = kFfLockedLayer  | kFfXref  | kFfRefEdit,
    kEFfLockedLayer      = (1 << 4),
    kEFfXref             = (1 << 5),
    kEFfRefEdit          = (1 << 6),
    kEFfFlagsMask        = kEFfLockedLayer | kEFfXref | kEFfRefEdit,
    kEFfFlagsOffset      = 4,
    kFvLockedLayerOffset = 8,
    kFvLockedLayerMask   = (0xFF << kFvLockedLayerOffset),
    kFvXrefOffset        = 16,
    kFvXrefMask          = (0xFF << kFvXrefOffset),
    kFvRefEditOffset     = 24,
    kFvRefEditMask       = (0xFF << kFvRefEditOffset)
  };
  OdUInt32 m_fadingFlags;
  template <OdGiContext::FadingType fadingType> friend class OdGsLockFadingFlag;

public:
  /** \details
    Sets Render Abort flag
    \param bSet [in]  Render Abort flag value
  */
  void setRenderAbort( bool bSet );
  /** \details
    Returns Render Abort flag value
  */
  virtual bool renderAbort();
protected:
  enum BaseVectorizerFlags
  {
    kRenderAbort            = OdGiBaseVectorizerImpl::kLastImplFlag << 1,
    kViewInteractive        = OdGiBaseVectorizerImpl::kLastImplFlag << 2,
    kLastBaseVectorizerFlag = kViewInteractive
  };
  double m_dRenderAbortCondition;
  OdPerfTimerBase* m_viewTimingProcessor;


private:
  mutable const OdGiLayerTraitsData* m_pCachedLayer;
  OdGsLayerNode* m_effectiveLayer;
  OdGsExtAccumPtr m_pOutputExtents;
  OdGiHistory* m_giHistory;
  OdUInt32 m_threadIndex;
  bool m_bTraitsDataChanged;
  bool m_bByBlockTraitsChanged;
  friend class OdGsUpdateContext;
  friend class OdGsDisplayContext;
  friend class OdGsBaseVectorizeView;
  friend class OdGsApplyModelOverrides;

protected:
  virtual void doCollide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor, OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0, const OdGsCollisionDetectionContext* pCtx = NULL );
  virtual void doCollideAll( OdGsCollisionDetectionReactor* pReactor, const OdGsCollisionDetectionContext* pCtx = NULL );
};

template <OdGiContext::FadingType fadingType>
class OdGsLockFadingFlag
{
  protected:
    OdUInt32 &m_lockedFlags;
    bool m_bSavedBit;
  public:
    OdGsLockFadingFlag(OdGsBaseVectorizer &vectorizer)
      : m_lockedFlags(vectorizer.m_fadingFlags)
      , m_bSavedBit(GETBIT(vectorizer.m_fadingFlags, 1 << fadingType))
    {}
    void set(bool bNewVal) { SETBIT(m_lockedFlags, 1 << fadingType, bNewVal); }
    OdGsLockFadingFlag(OdGsBaseVectorizer &vectorizer, bool bNewVal)
      : m_lockedFlags(vectorizer.m_fadingFlags)
      , m_bSavedBit(GETBIT(vectorizer.m_fadingFlags, 1 << fadingType))
    { set(bNewVal); }
    ~OdGsLockFadingFlag() { set(m_bSavedBit); }
};

inline bool OdGsBaseVectorizer::isFaded() const
{ // Returns true in case if any type of fading is 'true' and fading value is positive
  return !!((m_fadingFlags & kFfFlagsMask) & ((m_fadingFlags & kEFfFlagsMask) >> kEFfFlagsOffset));
}

inline OdUInt32 OdGsBaseVectorizer::fadingIntensity() const
{ // Returns percentage in [0-100] range
  OdUInt32 nIntensityMix = 100;
  if (GETBIT(m_fadingFlags, kFfLockedLayer) && GETBIT(m_fadingFlags, kEFfLockedLayer))
  {
    OdUInt32 mix = 100 - ((m_fadingFlags & kFvLockedLayerMask) >> kFvLockedLayerOffset);
    nIntensityMix = mix;
  }
  if (GETBIT(m_fadingFlags, kFfXref) && GETBIT(m_fadingFlags, kEFfXref))
  {
    OdUInt32 mix = 100 - ((m_fadingFlags & kFvXrefMask) >> kFvXrefOffset);
    nIntensityMix = odmin(nIntensityMix, mix);
  }
  if (GETBIT(m_fadingFlags, kFfRefEdit) && GETBIT(m_fadingFlags, kEFfRefEdit))
  {
    OdUInt32 mix = 100 - ((m_fadingFlags & kFvRefEditMask) >> kFvRefEditOffset);
    nIntensityMix = nIntensityMix * mix / 100;
  }
  nIntensityMix = odmax(10, nIntensityMix);
  return 100 - nIntensityMix;
}

inline const OdGsMarkerArray* OdGsBaseVectorizer::highlightedSubentities() const
{
  return m_pCurHltBr.get() && m_pCurHltBr->markers().size() ? &m_pCurHltBr->markers() : NULL;
}

inline OdUInt32 OdGsBaseVectorizer::threadIndex() const
{
  return m_threadIndex;
}

inline const OdGsHlBranch* OdGsBaseVectorizer::currentHighlightBranch() const
{
  return m_pCurHltBr;
}

inline OdGsLayerNode *OdGsBaseVectorizer::activeLayerNode(bool bSync) const
{
  if (bSync)
    updateLayerTraits(m_effectiveEntityTraitsData);
  return m_effectiveLayer;
}

inline OdGsModel::RenderType OdGsBaseVectorizer::activeRenderType() const
{
  return m_curRenderType;
}

inline OdGsOverlayId OdGsBaseVectorizer::activeOverlay() const
{
  return m_curOverlay;
}

inline bool OdGsBaseVectorizer::isSelecting() const
{
  return m_pSelectProc != 0;
}

inline bool OdGsBaseVectorizer::isHighlighted() const
{
  return GETBIT(m_flags, kHighlighted);
}

#include "TD_PackPop.h"

#endif // __OD_GS_BASE_VECTORIZER_H_
