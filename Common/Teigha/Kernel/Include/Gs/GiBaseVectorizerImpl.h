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

#ifndef __OD_GI_BASE_VECTORIZER_IMPL_H_
#define __OD_GI_BASE_VECTORIZER_IMPL_H_

#include "TD_PackPush.h"

#include "Gs/GsExport.h"
#include "Gi/GiBaseVectorizer.h"
#include "Gi/GiConveyorEmbranchment.h"
#include "Gi/GiLinetypeRedir.h"
#include "Gi/GiLineweightOverride.h"
#include "Gi/GiStack.h"
#include "Gi/GiPalette.h"
#include "Ps/PlotStyles.h"
#include "Gs/GsViewPropsDef.h"
#include "Gs/GsViewImpl.h"

/** \details
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGiBaseVectorizerImpl: public OdGiViewport, public OdGiBaseVectorizer
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

public:
  OdGiBaseVectorizerImpl();

  /** \details
    This method is to be called when setting up the vectorizer to work
    on the given view.
  **/
  virtual void setUp(OdGsViewImpl& view);

  OdGsViewImpl& view() const { ODA_ASSERT(m_view); return *m_view; }
  /** \details
    Returns the Aware flags for portion of geometry that had been vectorized
    since last call resetAwareFlags().

    \remarks
    awareFlags() returns a combination of zero or more of the following:

    <table>
    Name                    Value         Geometry_depends_on
    kVpID                   0x00000001    viewport Id (OdGiViewport::viewportId()).
    kVpRegenType            0x00000002    regeneration type (OdGiCommonDraw::regenType()).
    kVpRenderMode           0x00000004    render mode (OdGsView::mode()).
    kVpWorldToEye           0x00000008    description to come...
    kVpPerspective          0x00000010    description to come...
    kVpResolution           0x00000020    description to come...
    kVpMaxDevForCircle      0x00000040    description to come...
    kVpMaxDevForCurve       0x00000080    description to come...
    kVpMaxDevForBoundary    0x00000100    description to come...
    kVpMaxDevForIsoline     0x00000200    description to come...
    kVpMaxDevForFacet       0x00000400    description to come...
    kVpCamLocation          0x00000800    description to come...
    kVpCamTarget            0x00001000    description to come...
    kVpCamUpVector          0x00002000    description to come...
    kVpCamViewDir           0x00004000    description to come...
    kVpViewport             0x00008000    description to come...
    kVpFrontBack            0x00010000    description to come...
    kVpFrozenLayers         0x00020000    description to come...
    kVpLtypeScaleMult       0x00040000    description to come...
    kEntityRegenDraw        0x00080000    description to come...
    kVpDisableLayoutCache   0x00100000    description to come...
    kVpModelTf              0x00200000    description to come...
    kVpExcludeFromExtMinMax 0x00400000    description to come...
    kSectionable            0x00800000    description to come...
    kVpRenderModule         0x01000000    description to come...
    kVpAnnoScale            0x02000000    description to come...
    kVpFilterFunction       0x04000000    description to come...
    kVpVisualStyle          0x08000000    description to come...
    kVpHighlightingGeom     0x10000000    description to come...
    </table>

    \sa

    * resetAwareFlags
  */
  OdUInt32 awareFlags() const { return m_nAwareFlags; }
  
  /** \details
    Returns the Aware flags for portion of geometry that had been vectorized
    since last call resetAwareFlags().

    \remarks
    awareFlags() returns a combination of zero or more of the following:

    <table>
    Name                    Value         Geometry_depends_on
    kVpID                   0x00000001    viewport Id (OdGiViewport::viewportId()).
    kVpRegenType            0x00000002    regeneration type (OdGiCommonDraw::regenType()).
    kVpRenderMode           0x00000004    render mode (OdGsView::mode()).
    kVpWorldToEye           0x00000008    description to come...
    kVpPerspective          0x00000010    description to come...
    kVpResolution           0x00000020    description to come...
    kVpMaxDevForCircle      0x00000040    description to come...
    kVpMaxDevForCurve       0x00000080    description to come...
    kVpMaxDevForBoundary    0x00000100    description to come...
    kVpMaxDevForIsoline     0x00000200    description to come...
    kVpMaxDevForFacet       0x00000400    description to come...
    kVpCamLocation          0x00000800    description to come...
    kVpCamTarget            0x00001000    description to come...
    kVpCamUpVector          0x00002000    description to come...
    kVpCamViewDir           0x00004000    description to come...
    kVpViewport             0x00008000    description to come...
    kVpFrontBack            0x00010000    description to come...
    kVpFrozenLayers         0x00020000    description to come...
    kVpLtypeScaleMult       0x00040000    description to come...
    kEntityRegenDraw        0x00080000    description to come...
    kVpDisableLayoutCache   0x00100000    description to come...
    kVpModelTf              0x00200000    description to come...
    kVpExcludeFromExtMinMax 0x00400000    description to come...
    kSectionable            0x00800000    description to come...
    kVpRenderModule         0x01000000    description to come...
    kVpAnnoScale            0x02000000    description to come...
    kVpFilterFunction       0x04000000    description to come...
    kVpVisualStyle          0x08000000    description to come...
    kVpHighlightingGeom     0x10000000    description to come...
    </table>

    \sa

    * resetAwareFlags
  */
  OdUInt32 &awareFlags() { return m_nAwareFlags; }

  /** \details
    Resets the Aware flags.

    \sa
    awareFlags
  */
  void resetAwareFlags()
  {
    //NS: traits processing was optimized, but it is not correct
    //Aware flags can be changed when calulating for effective traits in affectTraits(),
    //but on the other hand the flags are reset independently.
    //As a result it is possible that aware flags have no kVpFrozenLayers flag set, but
    //the traits are based on the layer (effective traits are used, unchanged from 
    //the previously processed entity.
#ifdef ENABLE_GITRAITSOPTIMIZATION
    if(GETBIT(m_nAwareFlags, kVpFrozenLayers))
      resetEntityTraitsData(); //force resetting traits
#endif
    m_nAwareFlags = 0;
  }
  /** \details
    Returns the effective plot style for this vectorizer object.
  */
  const OdPsPlotStyleData& effectivePlotStyle() const;

  OdGsView::RenderMode getRenderMode() const;

  void clearLinetypeCache();

  void setLinetypesEnabled(bool bEnable);
  bool isLinetypesEnabled() const;

  void setModelSection(OdGiDrawable* pSection, bool bUseSectionGeometryMap);
  const OdGiDrawable* liveSection() const { return m_liveSection; }

  OdGeMatrix3d modelToWorldTransform() const;
  OdGeMatrix3d worldToModelTransform() const;

  virtual OdGeMatrix3d objectToDeviceMatrix() const;

  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  int lineweightToPixels(OdDb::LineWeight lineweight) const;
  
  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  double lineweightToPixels(double lineweight) const;

  bool hasLineweightOverride() const;
  bool hasLinestyleOverride() const;
  const OdGiLineweightOverride *currentLineweightOverride() const;

  ODCOLORREF paletteColor(OdUInt16 colorIndex) const;
  ODCOLORREF paletteColor(OdInt16 colorIndex) const { return paletteColor(OdUInt16(colorIndex)); }
  const ODCOLORREF *getPalette(int *pNumColors = NULL) const;
  bool hasPaletteOverride() const { return !m_paletteOverrideStack.empty(); }

  /** OdGiViewport methods *
  */
  virtual OdGeMatrix3d getModelToEyeTransform() const;
  virtual OdGeMatrix3d getEyeToModelTransform() const;
  virtual OdGeMatrix3d getWorldToEyeTransform() const;
  virtual OdGeMatrix3d getEyeToWorldTransform() const;

  virtual OdGeMatrix3d getWorldToModelTransform() const;
  virtual OdGeMatrix3d getModelToWorldTransform() const;

  virtual bool isPerspective() const;
  virtual bool doPerspective(OdGePoint3d& point) const;
  virtual bool doInversePerspective(OdGePoint3d& point) const;
  virtual void getNumPixelsInUnitSquare(
    const OdGePoint3d& point, 
    OdGePoint2d& pixelDensity,
    bool includePerspective = true) const;

  virtual OdGePoint3d getCameraLocation() const;
  virtual OdGePoint3d getCameraTarget() const;
  virtual OdGeVector3d getCameraUpVector() const;
  virtual OdGeVector3d viewDir() const;
  virtual OdUInt32 viewportId() const;
  virtual OdInt16 acadWindowId() const;
  virtual void getViewportDcCorners(
    OdGePoint2d& lowerLeft, 
    OdGePoint2d& upperRight) const;
  virtual bool getFrontAndBackClipValues(
    bool& clipFront, 
    bool& clipBack, 
    double& front, 
    double& back) const;
  virtual double linetypeScaleMultiplier() const;
  virtual double linetypeGenerationCriteria() const;
  virtual bool layerVisible(
    OdDbStub* layerId) const;  

  const OdGiContextualColors *contextualColors() const;

  /** OdGiGeometry methods *
  */
  virtual void pushModelTransform(const OdGeVector3d& normal);
  virtual void pushModelTransform(const OdGeMatrix3d & xfm);
  virtual void popModelTransform();
  virtual void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
  virtual void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);
  virtual void shell(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList,
                     const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0);
  virtual void mesh(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList,
                    const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0);

  /** OdGiWorldGeometry methods *
  */
  virtual void setExtents(const OdGePoint3d *newExtents);

  /** OdGiCommonDraw methods *
  */
  virtual double deviation(const OdGiDeviationType deviationType,
      const OdGePoint3d& pointOnCurve) const;
  virtual OdGiRegenType regenType() const;
  
  /** OdGiViewportDraw methods *
  */
  virtual OdUInt32 sequenceNumber() const;
  virtual bool isValidId(const OdUInt32 viewportId) const;
  virtual OdDbStub* viewportObjectId() const;
  virtual OdGiViewport& viewport() const;

  /** OdGiSubEntityTraits methods *
  */
  virtual void setFillPlane(const OdGeVector3d* pNormal = 0);
  virtual void setFillType(OdGiFillType fillType);
  virtual OdUInt32 setupForEntity() const;
  virtual bool pushLineweightOverride(const OdGiLineweightOverride *pOverride);
  virtual void popLineweightOverride();
  virtual bool pushPaletteOverride(const OdGiPalette* pOverride);
  virtual void popPaletteOverride();

  /** OdGiConveyorContext methods *
  */
  virtual const OdGiViewport* giViewport() const;
  virtual const OdGsView* gsView() const;
  virtual double annotationScale() const;

  /** OdGiBaseVectorizer methods *
  */
  virtual void beginViewVectorization();
  virtual void endViewVectorization();
  virtual void onTraitsModified();

protected:
  /** \details
    Returns true if and only if this VectorizeView object is configured to use a fill plane.
  */
  bool useFillPlane() const;

  inline bool isOutputSuppressed() const;

  /** \details
    Returns true if and this if the effective linetype generation is continuous for vectorizer object.
  */
  bool isEffectiveLinetypeContinuous() const;

  bool isSupportPlotStyles() const;

  /** OdGiBaseVectorizer methods *
  */
  virtual void affectTraits(const OdGiSubEntityTraitsData* pSource,
      OdGiSubEntityTraitsData& destination) const;

  double lineweightToPixelsOverride(OdDb::LineWeight lineweight, bool bAbsolute = false) const;
  int lineweightToPixelsOverrideInt(OdDb::LineWeight lineweight) const;
  double lineweightToPixelsOverride(double lineweight, bool bAbsolute = false) const;

  virtual void updateLineweightOverride(const OdGiLineweightOverride &lwdOverride);
  virtual void updatePaletteOverride(const OdGiPalette *pPalette);
private:
  const OdPsPlotStyleData& plotStyle() const;
  void affect2dTraits(const OdGiSubEntityTraitsData* pFrom, OdGiSubEntityTraitsData& to) const;
  /** \details
    Updates the Xline and Ray clipper prior to rendering each Xline and Ray.
    \remarks
    Called by ray() and xline().
  */
  void updateXlineNRayClipper();
  void onModelToEyeChanged();

protected:
  mutable OdUInt32            m_nAwareFlags;
  OdUInt32                    m_implFlags;
  OdGeVector3d                m_fillPlane;
  OdGiContext::PStyleType     m_pstype;
  mutable OdPsPlotStyleData   m_plotStyle;
  OdUInt32                    m_setAttributesHints;
  OdGiOrthoClipperPtr         m_pXlineNRayClipper;
  OdGiConveyorEntryPoint      m_xlineNRayEntryPoint;
  OdGiLinetypeRedirPtr        m_pLinetyper;
  OdGiConveyorEmbranchmentPtr m_pOutputBranch;
  OdRxObjectPtr               m_pGsReserved; // Reserved pointer for internal use (binary incompatible changes), do not delete.
  OdGiDrawablePtr             m_liveSection;
  OdRxObjectPtr               m_sectionGeometryMap;
  OdCmTransparency            m_sectForegroundFace;
  OdCmTransparency            m_sectForegroundEdge;

  OdGiXformPtr                m_pXToLtp;

private:
  OdGsViewImpl*               m_view;
  OdGiConveyorConnector       m_ltpEntryPoint;

  mutable int                 m_nPenIndex;
  mutable OdDbStub*           m_psnId;
  mutable OdPsPlotStyleData   m_effectivePlotStyle;

  OdArray<OdGiLineweightOverride, OdMemoryAllocator<OdGiLineweightOverride> > m_lwdOverrideStack; // Lineweight overrides stack
  OdGiStack<OdGiPalettePtr>   m_paletteOverrideStack; // Palette overrides stack
protected:
  mutable OdGeMatrix3d        m_objectToDeviceMatrix;
protected:
  enum Flags
  {
    kFillPlaneSet               = OdGiBaseVectorizer::kLastFlag <<  1,
    kLinetypeContinuous         = OdGiBaseVectorizer::kLastFlag <<  2,
    kModelCache                 = OdGiBaseVectorizer::kLastFlag <<  3,
    kHighlighted                = OdGiBaseVectorizer::kLastFlag <<  4,
    kDontAbortRegen             = OdGiBaseVectorizer::kLastFlag <<  5,
    kCheckLinePattern           = OdGiBaseVectorizer::kLastFlag <<  6,
    kBreakSelection             = OdGiBaseVectorizer::kLastFlag <<  7,
    kSuppressOutput             = OdGiBaseVectorizer::kLastFlag <<  8,
    kSelectionGeometry          = OdGiBaseVectorizer::kLastFlag <<  9,  // Processing geometry which is intended only for selection
    kSelectionDisabled          = OdGiBaseVectorizer::kLastFlag <<  10, // Processing geometry which shouldn't be used for selection
    kObjectToDeviceValid        = OdGiBaseVectorizer::kLastFlag <<  11,
    kDisableInfiniteGeomExtents = OdGiBaseVectorizer::kLastFlag <<  12,
    kSectionableGeomExtentsOnly = OdGiBaseVectorizer::kLastFlag <<  13,
    kDisableSection             = OdGiBaseVectorizer::kLastFlag <<  14,
    kSectionForegroundGeometry  = OdGiBaseVectorizer::kLastFlag <<  15,
    kInitGsState                = OdGiBaseVectorizer::kLastFlag <<  16,
    kSetVpLtypeScaleMult        = OdGiBaseVectorizer::kLastFlag <<  17,
    kSetVpFilterFunction        = OdGiBaseVectorizer::kLastFlag <<  18,
    kLineweightOverride         = OdGiBaseVectorizer::kLastFlag <<  19,
    kLinestyleOverride          = OdGiBaseVectorizer::kLastFlag <<  20,
    kLastOptionFlag             = kLinestyleOverride
    // Note: m_flags and Flags enum bits is nearly complete utilization. Please don't add new flags here.
    //       use m_implFlags and ImplFlags enum instead.
  };
  enum ImplFlags
  {
    kFirstImplFlag              = 1,
    kSectionableGeomEnabled     = kFirstImplFlag << 0,
    kLastImplFlag               = kSectionableGeomEnabled
  };
};

inline bool OdGiBaseVectorizerImpl::isOutputSuppressed() const
{
  return GETBIT(m_flags, kSuppressOutput);
}

inline void OdGiBaseVectorizerImpl::setLinetypesEnabled(bool bEnable)
{
  if (bEnable) m_pLinetyper->enable();
  else         m_pLinetyper->disable();
}

inline bool OdGiBaseVectorizerImpl::isLinetypesEnabled() const
{
  return m_pLinetyper->enabled();
}

inline bool OdGiBaseVectorizerImpl::isSupportPlotStyles() const
{
  return (m_pstype != OdGiContext::kPsNone) && GETBIT(m_setAttributesHints, kHint2DAttributes);
}

inline int OdGiBaseVectorizerImpl::lineweightToPixels(OdDb::LineWeight lineweight) const
{
  if (!GETBIT(m_flags, OdGiBaseVectorizerImpl::kLineweightOverride))
    return view().lineweightToPixels(lineweight);
  return lineweightToPixelsOverrideInt(lineweight);
}

inline double OdGiBaseVectorizerImpl::lineweightToPixels(double lineweight) const
{
  if (!GETBIT(m_flags, OdGiBaseVectorizerImpl::kLineweightOverride))
    return view().lineweightToPixels(lineweight);
  return lineweightToPixelsOverride(lineweight);
}

inline double OdGiBaseVectorizerImpl::lineweightToPixelsOverride(OdDb::LineWeight lineweight, bool bAbsolute) const
{
  return lineweightToPixelsOverride(double(lineweight) / 100.0, bAbsolute);
}

inline const OdGiLineweightOverride *OdGiBaseVectorizerImpl::currentLineweightOverride() const
{
  if (GETBIT(m_flags, kLineweightOverride | kLinestyleOverride))
    return  &m_lwdOverrideStack.last();
  return NULL;
}

inline bool OdGiBaseVectorizerImpl::hasLineweightOverride() const
{
  return GETBIT(m_flags, OdGiBaseVectorizerImpl::kLineweightOverride);
}

inline bool OdGiBaseVectorizerImpl::hasLinestyleOverride() const
{
  return GETBIT(m_flags, OdGiBaseVectorizerImpl::kLinestyleOverride);
}

#include "TD_PackPop.h"

#endif // __OD_GI_BASE_VECTORIZER_IMPL_H_
