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

#include "OdaCommon.h"
#include "GsOpenGLStreamVectorizer.h"
#include "OpenGLExtensions.h"
#include "Gi/GiRasterImage.h"
#include "RxObjectImpl.h"
#include "Gs/GsBaseModel.h"
#include "Gs/GsFiler.h"
#include "ExGsHelpers.h"

// OdOpenGLFragmentProgram

#include "OpenGL/FpPaletteShader.h"

bool OdOpenGLFragmentProgram::initShad(OdGsOpenGLVectorizeDevice *pDevice, const char *pAsm)
{
  if (m_bTried)
    return m_bTried > 1;
  m_bTried++;
  ((OdGLFn_GenPrograms_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_GenPrograms))
    (1, &m_progBind);
  if (m_progBind == 0) return false;
  ((OdGLFn_BindProgram_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_BindProgram))
    (GL_FRAGMENT_PROGRAM_ARB, m_progBind);
  ((OdGLFn_ProgramString_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_ProgramString))
    (GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)odStrLenA(pAsm), (const GLubyte *)pAsm);
  GLint errorPos = 0;
  ::glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorPos);
  if ((::glGetError() != GL_NO_ERROR) || (errorPos != -1))
  {
#ifdef ODA_DIAGNOSTICS
    ODA_TRACE(L"Fragment Program error at position %d:\n%s", errorPos, OdString((char*)::glGetString(GL_PROGRAM_ERROR_STRING_ARB)).c_str());
#endif
    ((OdGLFn_BindProgram_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_BindProgram))
      (GL_FRAGMENT_PROGRAM_ARB, 0);
    uninitShad(pDevice);
    m_bTried++;
    return false;
  }
  ((OdGLFn_BindProgram_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_BindProgram))
    (GL_FRAGMENT_PROGRAM_ARB, 0);
  m_bTried++;
  return true;
}

void OdOpenGLFragmentProgram::uninitShad(OdGsOpenGLVectorizeDevice *pDevice)
{
  if (m_bTried)
  {
    if (m_progBind)
      ((OdGLFn_DeletePrograms_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_DeletePrograms))
        (1, &m_progBind);
    m_progBind = 0;
    m_bTried = 0;
  }
}

void OdOpenGLFragmentProgram::bind(OdGsOpenGLVectorizeDevice *pDevice) const
{
  ((OdGLFn_BindProgram_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_BindProgram))
    (GL_FRAGMENT_PROGRAM_ARB, m_progBind);
  ::glEnable(GL_FRAGMENT_PROGRAM_ARB);
}

void OdOpenGLFragmentProgram::unbind(OdGsOpenGLVectorizeDevice *pDevice) const
{
  ((OdGLFn_BindProgram_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_BindProgram))
    (GL_FRAGMENT_PROGRAM_ARB, 0);
  ::glDisable(GL_FRAGMENT_PROGRAM_ARB);
}

void OdOpenGLFragmentProgram::setParam(OdGsOpenGLVectorizeDevice *pDevice, GLuint nParam, GLfloat p1, GLfloat p2, GLfloat p3, GLfloat p4) const
{
  ((OdGLFn_ProgramLocalParameter4f_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_ProgramLocalParameter4f))
    (GL_FRAGMENT_PROGRAM_ARB, nParam, p1, p2, p3, p4);
}

void OdOpenGLFragmentProgram::setParam(OdGsOpenGLVectorizeDevice *pDevice, GLuint nParam, const GLfloat *ps) const
{
  ((OdGLFn_ProgramLocalParameter4fv_ARB)pDevice->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_ProgramLocalParameter4fv))
    (GL_FRAGMENT_PROGRAM_ARB, nParam, ps);
}

// OdGsOpenGLStreamVectorizeDevice

OdGsOpenGLStreamVectorizeDevice::OdGsOpenGLStreamVectorizeDevice()
  : OdGsOpenGLVectorizeDevice()
{
  SETBIT_1(m_flags, kDynamicHighlight);
#ifdef OD_OGL_DYNAMICSUBENTHLT // for compatibility with previous behavior (enable dynamicSubEntHlt via OD_OGL_DYNAMICSUBENTHLT preprocessor definition)
  SETBIT_1(m_flags, kDynamicSubhighlight);
#endif
#ifdef OD_OGL_TEXTURESHARING
  m_sharingProv.setDevice(this);
#endif
  m_ttfFontsCache = OdTtfFontsCache::createObject(this);
}

OdGsOpenGLStreamVectorizeDevice::~OdGsOpenGLStreamVectorizeDevice()
{
  clearRasterImageCache();
}

void OdGsOpenGLStreamVectorizeDevice::clearRasterImageCache()
{
  OdUInt32 i;
  for (i = 0; i < m_rasterImageCache.size(); i++)
  {
    RasterImageCacheEntry *&ricEntry = m_rasterImageCache.at(i);
#ifndef OD_OGL_ENABLETEXBINDING
#ifndef OD_OGL_TEXTURESHARING
    ricEntry->m_texData.clearArray();
    ricEntry->m_palData.clearArray();
#endif
    // bindings will be deleted with context?
#else
    ::glDeleteTextures(1, &ricEntry->m_uBinding);
    if (ricEntry->m_uLutSize)
      ::glDeleteTextures(1, &ricEntry->m_uPalBinding);
#endif
    for (OdUInt32 j = 0; j < ricEntry->m_callbacks.size(); j++)
    {
      ricEntry->m_callbacks.at(j)->rasterImageCacheDestroyed();
    }
    delete ricEntry;
  }
  for (i = 0; i < m_resourceCallbacks.size(); i++)
    m_resourceCallbacks.at(i)->rasterImageCacheDestroyed();
  m_rasterImageCache.setPhysicalLength(0);
  m_resourceCallbacks.setPhysicalLength(0);
}

OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *OdGsOpenGLStreamVectorizeDevice::findRasterImage(const OdGsOpenGLStreamVectorizeDevice::RasterImageInfo &riInfo) const
{
  for (OdUInt32 i = 0; i < m_rasterImageCache.size(); i++)
  {
    RasterImageCacheEntry *ricEntry = m_rasterImageCache.at(i);
    if (*(RasterImageInfo*)ricEntry == riInfo)
      return ricEntry;
  }
  return NULL;
}

#ifndef OD_OGL_ENABLETEXBINDING
OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *OdGsOpenGLStreamVectorizeDevice::addRasterImage(const OdGsOpenGLStreamVectorizeDevice::RasterImageInfo &riInfo, const void *pData, OdUInt32 uSize,
                                                                                                        const void *pPalData, OdUInt32 uLutSize)
#else
#ifndef OD_OGL_TEXTURESHARING
OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *OdGsOpenGLStreamVectorizeDevice::addRasterImage(const OdGsOpenGLStreamVectorizeDevice::RasterImageInfo &riInfo, GLuint uBinding,
                                                                                                        OdUInt32 uLutSize, GLuint uPalBinding)
#else
OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *OdGsOpenGLStreamVectorizeDevice::addRasterImage(const RasterImageInfo &riInfo, const void *pData, OdUInt32 uSize, GLuint uBinding,
                                                                                                        const void *pPalData, OdUInt32 uLutSize, GLuint uPalBinding)
#endif
#endif
{
  RasterImageCacheEntry *ricEntry = new RasterImageCacheEntry;
  *(RasterImageInfo*)ricEntry = riInfo;
#ifdef OD_OGL_TEXTURESHARING
  ricEntry->m_texData = OdOpenGLArrayWrapperShared::createSharedArray();
  ricEntry->m_texData->setArray(pData, uSize);
  if (uLutSize)
  {
    ricEntry->m_palData = OdOpenGLArrayWrapperShared::createSharedArray();
    ricEntry->m_palData->setArray(pPalData, uLutSize * 4);
  }
#endif
#ifndef OD_OGL_ENABLETEXBINDING
#ifndef OD_OGL_TEXTURESHARING
  ricEntry->m_texData.setArray(OdOpenGLArrayWrapper::Type_Invalid, pData, uSize);
  if (m_uLutSize)
    ricEntry->m_palData.setArray(OdOpenGLArrayWrapper::Type_Invalid, pPalData, uLutSize * 4);
#endif
#else
  ricEntry->m_uBinding = uBinding;
  ricEntry->m_uPalBinding = uPalBinding;
#endif
  ricEntry->m_uLutSize = uLutSize;
  ricEntry->m_uRefCounter = 0;
  m_rasterImageCache.append(ricEntry);
  return ricEntry;
}

void OdGsOpenGLStreamVectorizeDevice::incrementRasterImageRef(OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *pEntry, OdGsOpenGLStreamVectorizeDevice::RasterImageCacheCallback *pCallback)
{
  pEntry->m_uRefCounter++;
  pEntry->m_callbacks.append(pCallback);
}

void OdGsOpenGLStreamVectorizeDevice::decrementRasterImageRef(OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *pEntry, OdGsOpenGLStreamVectorizeDevice::RasterImageCacheCallback *pCallback)
{
  pEntry->m_uRefCounter--;
  if (pEntry->m_uRefCounter == 0)
  {
#ifndef OD_OGL_ENABLETEXBINDING
#ifndef OD_OGL_TEXTURESHARING
    pEntry->m_texData.clearArray();
    pEntry->m_palData.clearArray();
#endif
#else
    OdOpenGLWrapResourceRelease _wResDel(this);
    ::glDeleteTextures(1, &pEntry->m_uBinding);
    if (pEntry->m_uLutSize)
      ::glDeleteTextures(1, &pEntry->m_uPalBinding);
#endif
    delete pEntry;
    OdArray<RasterImageCacheEntry*>::size_type nt;
    if (m_rasterImageCache.find(pEntry, nt))
      m_rasterImageCache.removeAt(nt);
  }
  else
  {
    pEntry->m_callbacks.remove(pCallback);
  }
}

void OdGsOpenGLStreamVectorizeDevice::addResourceCallback(RasterImageCacheCallback *pCallback)
{
  OdGsBaseDeviceMTSyncHelper __sync(mtDeviceSync(), OdGsBaseDeviceMTHelpers::kSyncTextureCache);
  m_resourceCallbacks.append(pCallback);
}

void OdGsOpenGLStreamVectorizeDevice::removeResourceCallback(RasterImageCacheCallback *pCallback)
{
  m_resourceCallbacks.remove(pCallback);
}

void OdGsOpenGLStreamVectorizeDevice::onContextDelete()
{
  clearRasterImageCache();
#ifdef OD_OGL_TEXTURESHARING
  m_sharingProv.onManagerDestroy();
#endif
  for (OdUInt32 nFP = 0; nFP < NFragmentPrograms; nFP++)
  {
    if (m_fragmentProgram[nFP].isInited())
      m_fragmentProgram[nFP].uninitShad(this);
  }
  OdGsOpenGLVectorizeDevice::onContextDelete();
}

// OdGsOpenGLStreamVectorizeView

OdGsOpenGLStreamVectorizeView::OdGsOpenGLStreamVectorizeView()
  : OdGsOpenGLVectorizeView()
  , m_bIsVertsColors(false)
  , m_bDynamicSubEntHlt(false)
  , m_bSecondaryBinding(false)
#ifdef OD_OGL_ES
  , m_metaWrite(true)
#endif
{
  SETBIT(m_uMaterialViewFlags, kEnableDelayCache, true);
  if (m_pSelectXfm.isNull())
  {
    m_pSelectXfm = OdGiXform::createObject();
    m_pSelectXfm->input().addSourceNode(m_mfEntryPoint);
  }
}

class OdOpenGLStreamList : public OdRxObject, public OdOpenGLFlatMetafileContainer
{
public:
  inline OdOpenGLStreamList() {}

  ~OdOpenGLStreamList()
  {
  }

  inline void begin()
  {
  }

  inline void end()
  {
  }

  inline void play(OdGsOpenGLStreamVectorizeView *pOGLView) const
  {
    switch (pOGLView->metafilePlayMode())
    {
      case kMfDisplay:
        {
          const bool bHighlighted = pOGLView->isHighlighted();
          const bool bCheckMark = (pOGLView->dynamicSubEntHlt()) ? pOGLView->metafileNeedCheckMarker() : false;
          pOGLView->m_metaRead.play(this, bHighlighted, bCheckMark);
        }
      break;
      case kMfSelect:
        pOGLView->m_metaRead.play4Select(this, &pOGLView->metafileEntryPoint(), pOGLView->OdGiBaseVectorizer::drawContext());
      break;
      case kMfExtents:
        pOGLView->m_metaRead.play4Select(this, &pOGLView->metafileEntryPoint(), pOGLView->OdGiBaseVectorizer::drawContext(), false);
      break;
      default:
        ODA_FAIL();
    }
  }

  inline void clear()
  {
    clearOpenGLFlatContainer();
    clearFlatContainer();
  }
};

void OdGsOpenGLStreamVectorizeView::shell(OdInt32 nVertices,
                                          const OdGePoint3d* vertexList,
                                          OdInt32 faceListSize,
                                          const OdInt32* faceList,
                                          const OdGiEdgeData* edgeData,
                                          const OdGiFaceData* faceData,
                                          const OdGiVertexData* vertexData)
{
  if (m_pCurMetafile == NULL)
    onTraitsModified(); // AMark fix #3736: Support for discardBackFaces in OpenGL GS is broken
  if (m_metaWrite.isActive())
  {
    if ((view().mode() >= kFlatShaded) && ::odExDiscardBackfaces(*this, device()->discardBackFaces()) &&
#ifndef OD_OGL_ALTERNATIVEHLT
        (forceAlternativeHlt() || (!currentHighlightBranch() || device()->discardBackFaces())) &&
#endif
        ((faceData && faceData->normals()) || (vertexData && vertexData->normals())) )
      m_bBackfacesDiscarded = true;
    OdGsBaseVectorizer::shell(nVertices, vertexList, faceListSize, faceList, edgeData, faceData, vertexData);
    m_bBackfacesDiscarded = false;
  }
  else
    OdGsOpenGLVectorizeView::shell(nVertices, vertexList, faceListSize, faceList, edgeData,
                                   faceData, vertexData);
}

void OdGsOpenGLStreamVectorizeView::polypointProc(OdInt32 numPoints,
                                                  const OdGePoint3d* vertexList,
                                                  const OdCmEntityColor* pColors,
                                                  const OdCmTransparency* pTransparency,
                                                  const OdGeVector3d* pNormals,
                                                  const OdGeVector3d* pExtrusions,
                                                  const OdGsMarker* pSubEntMarkers,
                                                  OdInt32 nPointSize)
{
  if (m_metaWrite.isActive() && (numPoints >= OD_OGL_MINPOLYPOINTWORK))
  {
    const bool bMarkers = pSubEntMarkers && m_bDynamicSubEntHlt && !(isNestedPersistent(currentGiPath()) && !device()->supportBlocks());
    if (pTransparency) // Force blending for entire primitive
      m_metaWrite.glTransparency(0);
    if (nPointSize >= 1)
      draw_lineweight(nPointSize, false);
    for (OdInt32 nPt = 0; nPt < numPoints; nPt++)
    {
      if (bMarkers)
        m_metaWrite.glSelectionMarker(pSubEntMarkers[nPt]);
      if (!pColors && !pTransparency)
      {
        if (!pExtrusions)
          m_metaWrite.glPackPoint(vertexList + nPt);
        else
        {
          const OdGePoint3d line[2] = { vertexList[nPt], vertexList[nPt] + pExtrusions[nPt] };
          m_metaWrite.glPackLine(2, line);
        }
      }
      else
      {
        const OdCmEntityColor clr = (pColors) ? pColors[nPt] : ODTOCMCOLOR(m_metaWrite.glTransparency(pTransparency[nPt].alpha(), false));
        if (pTransparency && pColors)
          m_metaWrite.glTransparency(pTransparency[nPt].alpha(), false);
        if (!pExtrusions)
          m_metaWrite.glPackCPoint(vertexList + nPt, &clr);
        else
        {
          const OdGePoint3d line[2] = { vertexList[nPt], vertexList[nPt] + pExtrusions[nPt] };
          const OdCmEntityColor clrs[2] = { clr, clr };
          m_metaWrite.glPackCLine(2, line, clrs);
        }
      }
    }
    if (nPointSize >= 1)
      draw_lineweight(nPointSize, true);
    if (pTransparency) // Restore primitive transparency
      m_metaWrite.glTransparency(effectiveTraits().transparency().alpha());
  }
  else
    OdGsOpenGLVectorizeView::polypointProc(numPoints, vertexList, pColors, pTransparency, pNormals, pExtrusions, pSubEntMarkers, nPointSize);
}

void OdGsOpenGLStreamVectorizeView::onTraitsModified()
{
  OdGsOpenGLVectorizeView::onTraitsModified();
  if (m_bDynamicSubEntHlt && m_metaWrite.isActive())
  {
    const OdGiSubEntityTraitsData &pTraits = effectiveTraits();
    m_metaWrite.glSelectionFlags((~(OdUInt8)pTraits.geomVisibilityFlags()) & OdOpenGLSelFlag_FlagsMask);
    if (gsWriter().gsModel())
    {
      OdGsMarker gsMarker = selectionMarker();
      if (m_metaWrite.prevMarker() != gsMarker)
      {
        if (isNestedPersistent(currentGiPath()) && !device()->supportBlocks())
          gsMarker = kNullSubentIndex;
        m_metaWrite.glSelectionMarker(gsMarker);
      }
    }
  }
}

bool OdGsOpenGLStreamVectorizeView::metafileNeedCheckMarker() const
{
  return (currentHighlightBranch() != NULL) && !currentHighlightBranch()->markers().isEmpty();
}

bool OdGsOpenGLStreamVectorizeView::metafileCheckMarker(OdOpenGLGsMarker gsMarker, bool bPrevState)
{
  const OdGsHlBranch* pBranch = currentHighlightBranch();
  if (pBranch && !pBranch->markers().isEmpty() && OdGsHlBranch::isValidMarker((OdGsMarker) gsMarker))
  {
    return pBranch->hasMarker((OdGsMarker)gsMarker);
  }
  return bPrevState;
}

void OdGsOpenGLStreamVectorizeView::metafileSelectMarker(OdOpenGLGsMarker gsMarker)
{
  setSelectionMarker((OdGsMarker)gsMarker);
}

OdGiConveyorOutput& OdGsOpenGLStreamVectorizeView::outputForMetafileGeometry()
{
  if (!m_bDynamicSubEntHlt)
    return OdGsBaseVectorizer::outputForMetafileGeometry();
  else
    return m_pSelectXfm->output();
}

void OdGsOpenGLStreamVectorizeView::setTransformForMetafileGeometry(const OdGeMatrix3d& mtx)
{
  if (!metafileTransformOverrided())
  {
    mfTransformStack().setOutputTransform(mtx);
    m_pSelectXfm->setTransform(mtx);
  }
  else
  {
    // Remove mf sub
    OdGeMatrix3d fixed = mtx * metafileToWorldMatrix();
    mfTransformStack().setOutputTransform(fixed);
    m_pSelectXfm->setTransform(fixed);
  }
}

OdGeMatrix3d OdGsOpenGLStreamVectorizeView::getTransformForMetafileGeometry() const
{
  return mfTransformStack().outputTransform();
}

void OdGsOpenGLStreamVectorizeView::pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags)
{
  if (!m_bDynamicSubEntHlt)
    OdGsOpenGLVectorizeView::pushMetafileTransform(mtx, uFlags);
  else
  {
    if (m_pSelectProc || (&(m_pSelectXfm->output().destGeometry()) != &(OdGiEmptyGeometry::kVoid)))
    {
      if (pushMetafileTransformForSelect(*this, mtx, uFlags))
        m_pSelectXfm->setTransform(mfTransformStack().getCompositeTransform());
    } else
      OdGsOpenGLVectorizeView::pushMetafileTransform(mtx, uFlags);
  }
}

void OdGsOpenGLStreamVectorizeView::popMetafileTransform(OdUInt32 uFlags)
{
  if (!m_bDynamicSubEntHlt)
    OdGsOpenGLVectorizeView::popMetafileTransform(uFlags);
  else
  {
    if (m_pSelectProc || (&(m_pSelectXfm->output().destGeometry()) != &(OdGiEmptyGeometry::kVoid)))
    {
      if (popMetafileTransformForSelect(*this, uFlags))
        m_pSelectXfm->setTransform(mfTransformStack().getCompositeTransform());
    } else
      OdGsOpenGLVectorizeView::popMetafileTransform(uFlags);
  }
}

void OdGsOpenGLStreamVectorizeView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode)
{
  OdGsBaseVectorizeView::select(pts, nPoints, pReactor, mode);
}

OdRxObjectPtr OdGsOpenGLStreamVectorizeView::newGsMetafile()
{
  return OdRxObjectImpl<OdOpenGLStreamList>::createObject();
}

void OdGsOpenGLStreamVectorizeView::beginMetafile(OdRxObject* pMetafile)
{
  OdGsBaseMaterialVectorizer::beginMetafile(pMetafile);
  static_cast<OdOpenGLStreamList*>(pMetafile)->begin();
  if (!m_pCurMetafile)
  {
    m_metaWrite.beginWriting(static_cast<OdOpenGLStreamList*>(pMetafile));
    m_metaWrite.setPointMode(OdOpenGLMetafileWriter::PointMode_Sep);
    m_pCurMetafile = pMetafile;
    if (isHighlighted() && (!currentHighlightBranch() || currentHighlightBranch()->markers().isEmpty()))
    { // Always pack as non-highlighted
      m_metaWrite.glDropLineStipple(OdOpenGLLinePatt::kLnPattSel, 1);
      m_metaWrite.glDropPolygonStipple(OdOpenGLFillPatt::kFillPattSel, 1);
    }
  }
  else
  {
    m_metaWrite.flushPackager(OdOpenGLMetafileWriter::PackageType_Null);
    m_metaWrite.containerWriter()->completeWriting();
    m_metaWrite.containerIO()->setContainer(static_cast<OdOpenGLStreamList*>(pMetafile));
  }
}

void OdGsOpenGLStreamVectorizeView::endMetafile(OdRxObject* pMetafile)
{
  if (m_metaWrite.isCurrent(static_cast<OdOpenGLStreamList*>(m_pCurMetafile)))
  {
    m_pCurMetafile = NULL;
    m_metaWrite.completeWriting();
  }
  else
  {
    m_metaWrite.flushPackager(OdOpenGLMetafileWriter::PackageType_Null);
    m_metaWrite.OdBaseMetafileOutputStream::completeWriting();
    m_metaWrite.containerIO()->setContainer(static_cast<OdOpenGLStreamList*>(m_pCurMetafile));
    m_metaWrite.containerWriter()->seek(m_metaWrite.containerIO()->size());
  }
  static_cast<OdOpenGLStreamList*>(pMetafile)->end();
  OdGsBaseMaterialVectorizer::endMetafile(pMetafile);
  if (!validateMetafileExtents())
    static_cast<OdOpenGLStreamList*>(pMetafile)->clear();
  if( device()->updateManager() )
  {
    OdOpenGLStreamList* file = static_cast<OdOpenGLStreamList*>(pMetafile);
    m_nMetafileSize = file->size();
    for( OdUInt32 i = 0; i < file->arrayElementsSize(); ++i )
    {
      m_nMetafileSize += file->arrayElement( i ).m_uSize;
    }
  }
}

void OdGsOpenGLStreamVectorizeView::playMetafile(const OdRxObject* pMetafile)
{
  resetFading(isFaded());
  static_cast<const OdOpenGLStreamList*>(pMetafile)->play(this);
}

OdGsOpenGLStreamVectorizeView::~OdGsOpenGLStreamVectorizeView()
{
}

void OdGsOpenGLStreamVectorizeView::beginViewVectorization()
{
  OdGsOpenGLVectorizeView::beginViewVectorization();
  m_metaRead.setCallback(this);
  OdOpenGLFlatMetafileContainerWriter *pCont = m_metaWrite.glContainerIO();
  // #6996 : Optimize rendering for disabled GsModel mode. (10.5 -> 2.8 sec on test file)
  if (isModelCacheEnabled())
  {
    pCont->setInitSize(OD_FLATMETAFILECONTAINERWRITER_DEF_INITSIZE);
    pCont->setGrowSize(OD_FLATMETAFILECONTAINERWRITER_DEF_GROWSIZE);
    m_metaWrite.setupPackagerGrowOptions(-100, 256);
  }
  else
  {
    pCont->setInitSize(02000000); // 0.5Mb
    pCont->setGrowSize(02000000); // 0.5Mb
    m_metaWrite.setupPackagerGrowOptions(02000000 / 2, 02000000 / 2);
  }
  m_metaWrite.setLinesRepMode(device()->isLinesRepModeEnabled());
  m_metaLwd.m_bPrecomputed = false;
  // Initialize lineweight styles
  m_styleLwd.m_bPointsLwd = giContext().lineWeightConfiguration(OdGiContext::kPointLineWeight) != 0;
  m_styleLwd.m_curLesStyle = m_styleLwd.m_defLesStyle = (OdPs::LineEndStyle)giContext().lineWeightConfiguration(OdGiContext::kLineCapStyle);
  m_styleLwd.m_curLjsStyle = m_styleLwd.m_defLjsStyle = (OdPs::LineJoinStyle)giContext().lineWeightConfiguration(OdGiContext::kLineJoinStyle);
  m_styleLwd.m_bCurStyleDef = m_styleLwd.m_bDefStyleDef = (m_styleLwd.m_defLesStyle == OdPs::kLesRound) && (m_styleLwd.m_defLjsStyle == OdPs::kLjsRound);
  m_styleLwd.m_bCurUsesDef = true;
  // Initialize anti-aliasing
  m_metaRead.enableLinesAntiAliasing(GETBIT(giContext().antiAliasingMode(), 1) && (view().mode() == k2DOptimized));
  // Dynamic highlighting
  m_bDynamicSubEntHlt = baseDevice()->supportDynamicSubhighlight();
  // TtfFonts cache
  if (device()->isTTFCacheEnabled()) // If TtfFonts cache enabled
    setDrawContextFlags(kSpatialFilterSimplText | kEmbranchmentSimplText, false);
}

void OdGsOpenGLStreamVectorizeView::loadViewport()
{
  OdGsOpenGLVectorizeView::loadViewport();

#if defined(OD_OGL_SUPPORTINDEXEDMODE) && defined(OD_OGL_ENABLEINDEXEDFACENORMALS)
  RenderMode rm = mode();

  // This change can't made result worse, but need only for indexed mode.
  if (rm == kFlatShaded || rm == kFlatShadedWithWireframe)
    ::glShadeModel(GL_FLAT);
  else
    ::glShadeModel(GL_SMOOTH);
#endif

  ::glDisable(GL_BLEND); // #9643 : enabled option causes performance regression. use only if need.

  m_metaRead.beginPlaying();
}

void OdGsOpenGLStreamVectorizeView::drawViewportFrame()
{
  if (m_metaRead.isLinesAntiAliasingEnabled())
  {
    m_metaRead.enableLinesAntiAliasing(false);
    OdGsOpenGLVectorizeView::drawViewportFrame();
    m_metaRead.enableLinesAntiAliasing(true);
  }
  else
    OdGsOpenGLVectorizeView::drawViewportFrame();
}

void OdGsOpenGLStreamVectorizeView::endViewVectorization()
{
  m_metaRead.enableLinesAntiAliasing(false);
  OdGsOpenGLVectorizeView::endViewVectorization();
}

void OdGsOpenGLStreamVectorizeView::draw_color(ODCOLORREF color)
{
  if (m_metaWrite.isActive())
  {
    m_metaWrite.glColor(color);
    if (m_bAcceptColorChange)
      m_cCurColor = color;
  }
  else
  {
    OdGsOpenGLVectorizeView::draw_color(color);
  }
}

void OdGsOpenGLStreamVectorizeView::draw_noColorMod(bool bState)
{
  if (m_metaWrite.isActive())
  {
    if (bState)
      m_metaWrite.glEnableShadingOpt(OdOpenGLShading_NoColorOverride);
    else
      m_metaWrite.glDisableShadingOpt(OdOpenGLShading_NoColorOverride);
  }
  OdGsOpenGLVectorizeView::draw_noColorMod(bState);
}

void OdGsOpenGLStreamVectorizeView::draw_linetype(OdOpenGLLinePatt::Entry entry, OdInt32 state, bool isAdaptive, double scaleFactor)
{
  if (m_metaWrite.isActive())
  {
    if (entry == OdOpenGLLinePatt::kLnPattLinetype)
    {
      if (state >= OdPs::kLtpUseObject)
        state = OdPs::kLtpSolid;
    }
    m_metaWrite.glLineStipple(entry, (OdUInt8)state);
  }
  else
  {
    OdGsOpenGLVectorizeView::draw_linetype(entry, state, isAdaptive, scaleFactor);
  }
}

void OdGsOpenGLStreamVectorizeView::draw_fillStyle(OdOpenGLFillPatt::Entry entry, OdInt32 state)
{
  if (m_metaWrite.isActive())
  {
    if (entry == OdOpenGLFillPatt::kFillPattFillstyle)
    {
      m_bFill = (state != OdPs::kFsUseObject);
      if (state >= OdPs::kFsUseObject || m_bProcessingTTF)
        state = OdPs::kFsSolid;
      state -= OdPs::kFsSolid;
    }
    m_metaWrite.glPolygonStipple(entry, (OdUInt8)state);
  }
  else
  {
    OdGsOpenGLVectorizeView::draw_fillStyle(entry, state);
  }
}

void OdGsOpenGLStreamVectorizeView::metafileLineStipple(OdOpenGLLinePatt::Entry entry, OdUInt8 state)
{
  OdGsOpenGLVectorizeView::draw_linetype(entry, state);
}

void OdGsOpenGLStreamVectorizeView::metafileFillStipple(OdOpenGLFillPatt::Entry entry, OdUInt8 state)
{
  if (entry == OdOpenGLFillPatt::kFillPattAll)
  {
    if (m_fillPattState.data.named.reg_transparency)
      OdGsOpenGLVectorizeView::draw_fillStyle(OdOpenGLFillPatt::kFillPattTransparency, 0);
    if (m_fillPattState.data.named.reg_fillstyle)
      OdGsOpenGLVectorizeView::draw_fillStyle(OdOpenGLFillPatt::kFillPattFillstyle, OdPs::kFsSolid);
    if (m_fillPattState.data.named.reg_sel)
      OdGsOpenGLVectorizeView::draw_fillStyle(OdOpenGLFillPatt::kFillPattSel, 0);
  }
  else
  {
    if (entry == OdOpenGLFillPatt::kFillPattFillstyle)
      state += OdPs::kFsSolid;
    OdGsOpenGLVectorizeView::draw_fillStyle(entry, state);
  }
}

const OdOpenGLLinePatt *OdGsOpenGLStreamVectorizeView::metafileGetLinePattPtr() const
{
  return &m_lnPattState;
}

const OdOpenGLFillPatt *OdGsOpenGLStreamVectorizeView::metafileGetFillPattPtr() const
{
  return &m_fillPattState;
}

void OdGsOpenGLStreamVectorizeView::draw_lineweight(OdDb::LineWeight lwLineWeight, GLfloat pixels)
{
  if (m_metaWrite.isActive() && m_metaWrite.isCurrent(static_cast<OdOpenGLStreamList*>(m_pCurMetafile)))
  {
    if (!hasLineweightOverride())
      m_metaWrite.glLineweight(lwLineWeight);
    else
      m_metaWrite.glLineweight(lineweightToPixelsOverride(lwLineWeight, true), true);
  }
  OdGsOpenGLVectorizeView::draw_lineweight(lwLineWeight, pixels);
}

void OdGsOpenGLStreamVectorizeView::draw_lineweight(double dLineWeight, GLfloat pixels)
{
  if (m_metaWrite.isActive() && m_metaWrite.isCurrent(static_cast<OdOpenGLStreamList*>(m_pCurMetafile)))
  {
    if (!hasLineweightOverride())
      m_metaWrite.glLineweight(dLineWeight);
    else
      m_metaWrite.glLineweight(lineweightToPixelsOverride(dLineWeight, true), true);
  }
  OdGsOpenGLVectorizeView::draw_lineweight(dLineWeight, pixels);
}

void OdGsOpenGLStreamVectorizeView::draw_lineweight(OdInt32 nLineWeight, bool bReset)
{
  if (m_metaWrite.isActive())
    m_metaWrite.glLineweight((bReset) ? OdInt32(0) : nLineWeight);
  OdGsOpenGLVectorizeView::draw_lineweight(nLineWeight, bReset);
}

double OdGsOpenGLStreamVectorizeView::computeLineweightCft(const OdGePoint3d *pOrigin)
{
  double dLwdCoef = 0.0;
  if (pOrigin)
  {
    if (m_metaLwd.m_bPerspective)
    {
      OdGePoint3d pt1 = *pOrigin, pt2 = pt1 + m_metaLwd.m_xfmBasis[1];
      pt1.transformBy(m_metaLwd.m_xfm); pt2.transformBy(m_metaLwd.m_xfm);
      dLwdCoef = (pt2 - pt1).length();
    }
    else
      return m_metaLwd.m_dPreCft;
  }
  else
  {
    if (!m_metaLwd.m_bPrecomputed)
    {
      if (isModelCacheEnabled())
      {
        m_metaLwd.m_xfm = view().worldToDeviceMatrix() * metafileToWorldMatrix();
        m_metaLwd.m_xfmBasis[1] = m_metaLwd.m_basis[1] = view().upVector();
        m_metaLwd.m_xfmBasis[2] = m_metaLwd.m_basis[2] = view().eyeVector();
        m_metaLwd.m_xfmBasis[0] = m_metaLwd.m_basis[0] = m_metaLwd.m_basis[1].crossProduct(m_metaLwd.m_basis[2]);
        if (metafileTransformOverrided())
        {
          m_metaLwd.m_xfmBasis[0].transformBy(worldToMetafileMatrix());
          m_metaLwd.m_xfmBasis[1].transformBy(worldToMetafileMatrix());
          m_metaLwd.m_xfmBasis[2].transformBy(worldToMetafileMatrix());
        }
      }
      else
      {
        m_metaLwd.m_xfm = view().eyeToScreenMatrix();
        m_metaLwd.m_xfmBasis[1] = m_metaLwd.m_basis[1] = OdGeVector3d::kYAxis;
        m_metaLwd.m_xfmBasis[2] = m_metaLwd.m_basis[2] = OdGeVector3d::kZAxis;
        m_metaLwd.m_xfmBasis[0] = m_metaLwd.m_basis[0] = OdGeVector3d::kXAxis;
      }
      if (!m_metaLwd.m_bPerspective)
        dLwdCoef = (m_metaLwd.m_xfm * m_metaLwd.m_xfmBasis[1]).length();
      m_metaLwd.m_bPrecomputed = true;
    }
    else
      return m_metaLwd.m_dPreCft;
  }
  if (OdNonZero(dLwdCoef))
    return 0.5 / dLwdCoef;
  return 0.0;
}

bool OdGsOpenGLStreamVectorizeView::metafileProcessLineweight(const OdOpenGLLwdSetting &lwd)
{
  m_metaLwd.m_nPix = lwdSettingToPixels(lwd);
  bool bPrevNotExtended = m_metaLwd.m_bEnabled && (!m_metaLwd.m_bExtendPoints || !m_metaLwd.m_bExtendLines);
  if (m_metaLwd.m_nPix > 1)
  {
    // Enable lineweights
    m_metaLwd.m_bEnabled = true;
    m_metaLwd.m_bForced = lwd.isForcedLineweight();
    m_metaLwd.m_bExtendPoints = (m_metaLwd.m_nPix > (int)m_fMaxLineWidth) || !device()->isPointLineweightOptimizationEnabled();
    m_metaLwd.m_bExtendLines  = (m_metaLwd.m_nPix > (int)m_fMaxLineWidth) || !device()->isLineLineweightOptimizationEnabled();
    if (!m_metaLwd.m_bExtendPoints || !m_metaLwd.m_bExtendLines)
    {
      ::glLineWidth((GLfloat)m_metaLwd.m_nPix);
      ::glPointSize((GLfloat)m_metaLwd.m_nPix);
      ::glEnable(GL_LINE_SMOOTH);
      ::glEnable(GL_POINT_SMOOTH);
      bPrevNotExtended = false;
    }
    m_metaLwd.m_bPerspective = view().isPerspective();
    m_metaLwd.m_dPreCft = computeLineweightCft(NULL);
  } else
    m_metaLwd.m_bEnabled = false;
  if (bPrevNotExtended)
  {
    ::glDisable(GL_LINE_SMOOTH);
    ::glDisable(GL_POINT_SMOOTH);
    ::glLineWidth(1.0f);
    ::glPointSize(1.0f);
  }
  return m_metaLwd.m_bEnabled;
}

bool OdGsOpenGLStreamVectorizeView::metafilePointsLineweight() const
{
  return m_styleLwd.m_bPointsLwd || m_metaLwd.m_bForced;
}

bool OdGsOpenGLStreamVectorizeView::metafileDrawExtendedPointLineweight() const
{
  return m_metaLwd.m_bExtendPoints;
}

bool OdGsOpenGLStreamVectorizeView::metafileDrawExtendedLineLineweight() const
{
  return m_metaLwd.m_bExtendLines;
}

void OdGsOpenGLStreamVectorizeView::metafileDrawExtendedLine(const OdGePoint3d *origin, const OdUInt8 *color)
{
  OdGeVector3d pathVec1 = origin[1] - origin[0], pathVec; // screen-plane projection
  if (!m_metaLwd.m_bPerspective)
    pathVec = pathVec1.orthoProject(m_metaLwd.m_xfmBasis[2]);
  else
    pathVec = pathVec1; // @@@TODO: solve it for perspective projection (check perspective correction to fix direction)
  OdGe::ErrorCondition err;
  pathVec.normalize(OdGeContext::gTol, err);
  if (err != OdGe::kOk)
  {
    metafileDrawExtendedPoint(origin, color);
    return;
  }
  OdGeVector3d sideVec = pathVec.crossProduct(m_metaLwd.m_xfmBasis[2]);
  sideVec.normalize(OdGeContext::gTol, err);
  if (err != OdGe::kOk)
  {
    metafileDrawExtendedPoint(origin, color);
    return;
  }
  double lwdCft = computeLineweightCft(origin) * m_metaLwd.m_nPix;
  renderLineCap(m_metaLwd.m_nPix, origin, pathVec * lwdCft, sideVec * lwdCft, color, OD_OGL_NCOLORCOMPONENTS, (color) ? (color + OD_OGL_NCOLORCOMPONENTS) : NULL);
}

void OdGsOpenGLStreamVectorizeView::metafileDrawExtendedPoint(const OdGePoint3d *origin, const OdUInt8 *color)
{
  OdGePoint3d copyPoint[2] = { *origin, *origin };
  double lwdCft = computeLineweightCft(origin) * m_metaLwd.m_nPix;
  renderLineCap(m_metaLwd.m_nPix, copyPoint, m_metaLwd.m_xfmBasis[1] * lwdCft, m_metaLwd.m_xfmBasis[0] * lwdCft, color, OD_OGL_NCOLORCOMPONENTS);
}

#ifdef OD_OGL_ES
void oglEsDrawPlain(OdInt32 nPoints, const OdGePoint3d* pPoints, GLenum mode);
#endif

void OdGsOpenGLStreamVectorizeView::metafileDrawExtendedLineStrip(const OdGePoint3d *pPoints, OdUInt32 nPoints, const OdUInt8 *color)
{
  class OdExOpenGLExtendedLineWeightStyleClient : public OdExExtendedLineWeightStyleClient
  {
    OdOpenGLMetafileReaderCallback *pCallback;
    const OdUInt8 *pColors;
    public:
      OdExOpenGLExtendedLineWeightStyleClient(const OdGeVector3d &axis, double lwd, OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle,
                                              OdOpenGLMetafileReaderCallback *pCbk, const OdUInt8 *colors)
        : OdExExtendedLineWeightStyleClient(axis, lwd, capStyle, joinStyle), pCallback(pCbk), pColors(colors)
      {
      }
      virtual bool elsPolygonOut(const OdGePoint3d *pPoints, OdUInt32 nPoints)
      {
#ifndef OD_OGL_ES
        ::glBegin((nPoints == 4) ? GL_QUADS : GL_TRIANGLES);
          if (pColors)
          {
#if (OD_OGL_NCOLORCOMPONENTS == 4)
            ::glColor4ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(0));
#else
            ::glColor3ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(0));
#endif
          }
          ::glVertex3dv((const GLdouble*)pPoints);
          if (pColors)
          {
#if (OD_OGL_NCOLORCOMPONENTS == 4)
            ::glColor4ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(1));
#else
            ::glColor3ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(1));
#endif
          }
          ::glVertex3dv((const GLdouble*)(pPoints + 1));
          if (pColors)
          {
#if (OD_OGL_NCOLORCOMPONENTS == 4)
            ::glColor4ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(2));
#else
            ::glColor3ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(2));
#endif
          }
          ::glVertex3dv((const GLdouble*)(pPoints + 2));
          if (nPoints == 4)
          {
            if (pColors)
            {
#if (OD_OGL_NCOLORCOMPONENTS == 4)
              ::glColor4ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(3));
#else
              ::glColor3ubv(pColors + OD_OGL_NCOLORCOMPONENTS * polygonVertexIndex(3));
#endif
            }
            ::glVertex3dv((const GLdouble*)(pPoints + 3));
          }
        ::glEnd();
#else
        // @@@TODO: support colors
        ::oglEsDrawPlain((OdInt32)nPoints, pPoints, GL_TRIANGLE_FAN);
#endif
        return true;
      }
      virtual bool elsCircleOut(const OdGePoint3d &pPoint, const OdGeVector3d & /*normal*/, double /*radius*/)
      {
        if (!pColors)
          pCallback->metafileDrawExtendedPoint(&pPoint);
        else
          pCallback->metafileDrawExtendedPoint(&pPoint, pColors + OD_OGL_NCOLORCOMPONENTS * circleVertexIndex());
        return true;
      }
  } _exlwd_renderer(m_metaLwd.m_xfmBasis[2], (computeLineweightCft(pPoints) * m_metaLwd.m_nPix) * 2,
                    m_styleLwd.m_curLesStyle, m_styleLwd.m_curLjsStyle, this, color);
  _exlwd_renderer.process(pPoints, nPoints);
}

void OdGsOpenGLStreamVectorizeView::metafileGetBackgroundColor(ODCOLORREF &bkgndColor)
{
  bkgndColor = baseDevice()->getPaletteBackground();
}

OdColorConverterCallback *OdGsOpenGLStreamVectorizeView::metafileColorConverter()
{
  return device()->getColorConverter();
}

void OdGsOpenGLStreamVectorizeView::draw_lineStyle(bool bByDefault, OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle)
{
  if (hasLinestyleOverride())
  {
    bByDefault = false;
    capStyle  = currentLineweightOverride()->endStyleOverride();
    joinStyle = currentLineweightOverride()->joinStyleOverride();
  }
  if (m_metaWrite.isActive())
    m_metaWrite.glLineStyle(bByDefault, capStyle, joinStyle);
  else
    metafileLineweightStyle(bByDefault, capStyle, joinStyle);
}

void OdGsOpenGLStreamVectorizeView::metafileLineweightStyle(bool bByDefault, OdPs::LineEndStyle lesStyle, OdPs::LineJoinStyle ljsStyle)
{
  if (bByDefault)
  {
    m_styleLwd.m_bCurUsesDef = true;
    m_styleLwd.m_bCurStyleDef = m_styleLwd.m_bDefStyleDef;
    m_styleLwd.m_curLesStyle = m_styleLwd.m_defLesStyle;
    m_styleLwd.m_curLjsStyle = m_styleLwd.m_defLjsStyle;
  }
  else
  {
    m_styleLwd.m_bCurUsesDef = (lesStyle == OdPs::kLesUseObject) && (ljsStyle == OdPs::kLjsUseObject);
    m_styleLwd.m_curLesStyle = (lesStyle != OdPs::kLesUseObject) ? lesStyle : m_styleLwd.m_defLesStyle;
    m_styleLwd.m_curLjsStyle = (ljsStyle != OdPs::kLjsUseObject) ? ljsStyle : m_styleLwd.m_defLjsStyle;
    m_styleLwd.m_bCurStyleDef = (m_styleLwd.m_curLesStyle == OdPs::kLesRound) && (m_styleLwd.m_curLjsStyle == OdPs::kLjsRound);
  }
}

const OdOpenGLLwdStyle *OdGsOpenGLStreamVectorizeView::metafileGetLwdStylePtr() const
{
  return &m_styleLwd;
}

inline int isLineParallelDegenerate(const OdGePoint3d &pt1, const OdGePoint3d &pt2,
                                    const OdGeVector3d &xVec, const OdGeVector3d &yVec, const OdGeVector3d &zVec,
                                    bool bLinesRepModeEnabled = false)
{
  if (bLinesRepModeEnabled)
    return (pt1.isEqualTo(pt2)) ? 2 : 0;
  OdGeVector3d line(pt2 - pt1);
  OdGeVector2d projLine(xVec.dotProduct(line), yVec.dotProduct(line));
  if (projLine.isZeroLength())
  { // Means that projected line is invisible in current view
    // Check does line haven't length by z direction
    const double zLen = zVec.dotProduct(line);
    if (OdZero(zLen))
      return 2; // Completely degenerate
    return 1; // Partially degenerate (in plane)
  }
  return 0; // Non degenerate
}

#define TD_OGL_REALLOC_TRY try {
#define TD_OGL_REALLOC_CATCH \
  } \
  catch (const OdError &error) { \
    if (error.code() == eOutOfMemory) \
      m_metaWrite.clearArrays(); \
    throw; \
  }

void OdGsOpenGLStreamVectorizeView::polylineOut(OdInt32 nPoints, const OdInt32* pVertexIndexList)
{
  if (m_metaWrite.isActive())
  {
    TD_OGL_REALLOC_TRY
    if (!m_metaWrite.indexedMode())
      OdGiGeometrySimplifier::polylineOut(nPoints, pVertexIndexList);
    else
    {
      m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Lighting, 0);
      m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Texture2D, 0);
#if 0 // Haven't sense since in any case primitive become linear, so will be not rendered
      int lineTopology = 0;
      switch (nPoints)
      {
        case 1: lineTopology = 2; break;
        case 2: lineTopology = isLineParallelDegenerate(vertexDataList()[pVertexIndexList[0]], vertexDataList()[pVertexIndexList[1]],
                                                        xVector(), getUpVector(), eyeVector(), m_metaWrite.isLinesRepModeEnabled()); break;
      }
      switch (lineTopology)
      {
        case 1: case 2:
        {
          OdInt32 pIndices[2] = { pVertexIndexList[0], pVertexIndexList[0] };
          m_metaWrite.glPackLine(2, pIndices);
          if (lineTopology == 2) break;
        }
        default:
          m_metaWrite.glPackLine(nPoints, pVertexIndexList);
      }
#else
      if (nPoints == 1)
      {
        const OdInt32 pIndices[2] = { pVertexIndexList[0], pVertexIndexList[0] };
        m_metaWrite.glPackLine(2, pIndices);
      }
      else
        m_metaWrite.glPackLine(nPoints, pVertexIndexList);
#endif
    }
    TD_OGL_REALLOC_CATCH
  }
  else
  {
    // Old branch haven't all functionality which we need, so reuse it partially
    ::glDisable(GL_LIGHTING);
    ::glDisable(GL_TEXTURE_2D);
    int lineTopology = 0;
    switch (nPoints)
    {
      case 1: lineTopology = 2; break;
      case 2: lineTopology = isLineParallelDegenerate(vertexDataList()[pVertexIndexList[0]], vertexDataList()[pVertexIndexList[1]], xVector(), getUpVector(), eyeVector()); break;
    }
    switch (lineTopology)
    {
      case 1: case 2:
      {
        if (m_styleLwd.m_bPointsLwd || (m_lineWeight.isForcedLineweight() && m_lineWeight.hasLineweight()))
          metafileDrawExtendedPoint(vertexDataList() + pVertexIndexList[0]);
        else
          OdGsOpenGLVectorizeView::polylineOut(nPoints, pVertexIndexList);
        if (lineTopology == 2) break;
      }
      default:
        if (!m_lineWeight.hasLineweight() ||
            (!isModelCacheEnabled() && (m_fCurLw < 2.0f))) // Optimize if GsModel disabled
        {
          OdGsOpenGLVectorizeView::polylineOut(nPoints, pVertexIndexList);
        }
        else
        {
          if (!m_styleLwd.m_bCurStyleDef)
          {
            OdGePoint3dArray &tmpBuf = m_metaRead.getTmpBuffer();
            tmpBuf.resize(nPoints);
            OdGePoint3d *pBuf = tmpBuf.asArrayPtr();
            for (OdInt32 nPt = 0; nPt < nPoints; nPt++)
              pBuf[nPt] = vertexDataList()[pVertexIndexList[nPt]];
            metafileDrawExtendedLineStrip(pBuf, (OdUInt32)nPoints);
            if (tmpBuf.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT)
              tmpBuf.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT);
          }
          else
            OdGsOpenGLVectorizeView::polylineOut(nPoints, pVertexIndexList);
        }
    }
  }
}

void OdGsOpenGLStreamVectorizeView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  if (m_metaWrite.isActive())
  {
    TD_OGL_REALLOC_TRY
    //m_metaWrite.glDisableOpt(OdOpenGLAttribute_Lighting);
    //m_metaWrite.glDisableOpt(OdOpenGLAttribute_Texture2D);
    m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Lighting, 0);
    m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Texture2D, 0);
    int lineTopology = 0;
    switch (nPoints)
    {
      case 1: lineTopology = 2; break;
      case 2: lineTopology = isLineParallelDegenerate(pPoints[0], pPoints[1], xVector(), getUpVector(), eyeVector(),
                                                      m_metaWrite.isLinesRepModeEnabled()); break;
    }
    switch (lineTopology)
    {
      case 1: case 2:
      {
        //m_metaWrite.glPoint(*pPoints);
        m_metaWrite.glPackPoint(pPoints);
        if (lineTopology == 2) break;
      }
      default:
        m_metaWrite.glPackLine(nPoints, pPoints);
    }
    TD_OGL_REALLOC_CATCH
  }
  else
  {
    // Old branch haven't all functionality which we need, so reuse it partially
    ::glDisable(GL_LIGHTING);
    ::glDisable(GL_TEXTURE_2D);
    int lineTopology = 0;
    switch (nPoints)
    {
      case 1: lineTopology = 2; break;
      case 2: lineTopology = isLineParallelDegenerate(pPoints[0], pPoints[1], xVector(), getUpVector(), eyeVector()); break;
    }
    switch (lineTopology)
    {
      case 1: case 2:
      {
        if (m_styleLwd.m_bPointsLwd || (m_lineWeight.isForcedLineweight() && m_lineWeight.hasLineweight()))
          metafileDrawExtendedPoint(pPoints);
        else
          OdGsOpenGLVectorizeView::polylineOut(nPoints, pPoints);
        if (lineTopology == 2) break;
      }
      default:
        if (!m_lineWeight.hasLineweight() ||
            (!isModelCacheEnabled() && (m_fCurLw < 2.0f))) // Optimize if GsModel disabled
        {
          OdGsOpenGLVectorizeView::polylineOut(nPoints, pPoints);
        }
        else
        {
          if (!m_styleLwd.m_bCurStyleDef)
            metafileDrawExtendedLineStrip(pPoints, (OdUInt32)nPoints);
          else
            OdGsOpenGLVectorizeView::polylineOut(nPoints, pPoints);
        }
    }
  }
}

void OdGsOpenGLStreamVectorizeView::polygonOut(OdInt32 nPoints, const OdGePoint3d* pPoints, const OdGeVector3d* pNormal)
{
  if (!m_bFill && m_metaWrite.isActive())
  {
    TD_OGL_REALLOC_TRY
    m_metaWrite.flushPackager(OdOpenGLMetafileWriter::PackageType_Null);
    m_metaWrite.glDisableOpt(OdOpenGLAttribute_Lighting);
    m_metaWrite.glDisableOpt(OdOpenGLAttribute_Texture2D);
    m_metaWrite.glEnableArray(OdOpenGLMetaRecArrayType_Vertex, 0);
    m_metaWrite.glAppendArray_dbl((const double*)pPoints, nPoints * 3);
    m_metaWrite.glDrawArrays(m_bFill ? GL_POLYGON : GL_LINE_LOOP, 0, nPoints);
    m_metaWrite.glDisableArray(OdOpenGLMetaRecArrayType_Vertex);
    TD_OGL_REALLOC_CATCH
  }
  else
  {
    OdGsOpenGLVectorizeView::polygonOut(nPoints, pPoints, pNormal);
  }
}

void OdGsOpenGLStreamVectorizeView::pushTriangle(double *texCoords, const OdInt32* indices, const OdGeVector3d *n1, const OdGeVector3d *n2, const OdGeVector3d *n3)
{
  if (m_metaWrite.indexedMode())
  {
    if (m_bIsVertsColors)
      m_metaWrite.glSetPackITriColors(indices,
      vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2]);
    m_metaWrite.glSetPackITriNormals(indices,
      n1, n2, n3);
    if (m_bTexCoords)
    {
      const OdGePoint2d *pTPt = m_texCoordsArray.getPtr();
      m_metaWrite.glSetPackITriTexCoords(indices,
                                         (const double*)(pTPt + indices[0]),
                                         (const double*)(pTPt + indices[1]),
                                         (const double*)(pTPt + indices[2]),
                                         isMappingDelayed());
    }
    m_metaWrite.glPackITriangle(indices);
  }
  else
  {
    if (m_bTexCoords)
    {
      const OdGePoint2d *pTPt = m_texCoordsArray.getPtr();
      if (m_bIsVertsColors)
        m_metaWrite.glPackCNTTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
        vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2],
        n1, n2, n3,
        (const double*)(pTPt + indices[0]),
        (const double*)(pTPt + indices[1]),
        (const double*)(pTPt + indices[2]),
        isMappingDelayed());
      else
        m_metaWrite.glPackNTTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
        n1, n2, n3,
        (const double*)(pTPt + indices[0]),
        (const double*)(pTPt + indices[1]),
        (const double*)(pTPt + indices[2]),
        isMappingDelayed());
    }
    else
    {
      if (m_bIsVertsColors)
        m_metaWrite.glPackCNTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
        vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2],
        n1, n2, n3);
      else
        m_metaWrite.glPackNTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
        n1, n2, n3);
    }
  }
}

void OdGsOpenGLStreamVectorizeView::triangleOut(const OdInt32* indices, const OdGeVector3d* pNormal)
{
  if (m_metaWrite.isActive())
  {
    TD_OGL_REALLOC_TRY
    if((renderMode() <= OdGsView::kHiddenLine) || m_bDontShade)
    {
      if (m_metaWrite.indexedMode())
      {
        if (m_bIsVertsColors)
          m_metaWrite.glSetPackITriColors(indices,
                                          vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2]);
        m_metaWrite.glPackITriangle(indices);
      }
      else
      {
        if (m_bIsVertsColors)
          m_metaWrite.glPackCTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
                                      vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2]);
        else
          m_metaWrite.glPackTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2]);
      }
    }
    else // normals are necessary
    {
      if(m_bIsVertsNormals)
      {
        OdGiOrientationType orientation = vertexData()->orientationFlag();

        if(orientation == kOdGiCounterClockwise)
        {
          const OdGeVector3d* pNormals = vertexData()->normals();
          pushTriangle(NULL, indices, &pNormals[indices[0]], &pNormals[indices[1]], &pNormals[indices[2]]);
        }
        else if(orientation == kOdGiClockwise)
        {
          OdGeVector3d normals[3] = 
          {
            -vertexData()->normals()[indices[0]],
            -vertexData()->normals()[indices[1]],
            -vertexData()->normals()[indices[2]]
          };

          pushTriangle(NULL, indices, &normals[0], &normals[1], &normals[2]);
        }
        else // no orientation -- we must calculate it
        {
          OdGeVector3d normals[3];

          for (int i = 0; i < 3; i++)
          {
            normals[i] = vertexData()->normals()[indices[i]];
            if(normals[i].dotProduct(*pNormal) < 0.0)
              normals[i] *= -1.0;
          }

          pushTriangle(NULL, indices, &normals[0], &normals[1], &normals[2]);
        }
      }
      else
      {
        if (m_metaWrite.indexedMode())
        {
          if (m_bIsVertsColors)
            m_metaWrite.glSetPackITriColors(indices,
                                            vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2]);
          m_metaWrite.glSetPackITriNormals(indices,
                                           pNormal, pNormal, pNormal);
          if (m_bTexCoords)
          {
            const OdGePoint2d *pTPt = m_texCoordsArray.getPtr();
            m_metaWrite.glSetPackITriTexCoords(indices,
                                               (const double*)(pTPt + indices[0]),
                                               (const double*)(pTPt + indices[1]),
                                               (const double*)(pTPt + indices[2]),
                                               isMappingDelayed());
          }
          m_metaWrite.glPackITriangle(indices);
        }
        else
        {
          if (m_bTexCoords)
          {
            const OdGePoint2d *pTPt = m_texCoordsArray.getPtr();
            if (m_bIsVertsColors)
              m_metaWrite.glPackCNTTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
                                            vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2],
                                            pNormal, pNormal, pNormal,
                                            (const double*)(pTPt + indices[0]),
                                            (const double*)(pTPt + indices[1]),
                                            (const double*)(pTPt + indices[2]),
                                            isMappingDelayed());
            else
              m_metaWrite.glPackNTTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
                                           pNormal, pNormal, pNormal,
                                           (const double*)(pTPt + indices[0]),
                                           (const double*)(pTPt + indices[1]),
                                           (const double*)(pTPt + indices[2]),
                                           isMappingDelayed());
          }
          else
          {
            if (m_bIsVertsColors)
              m_metaWrite.glPackCNTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
                                           vertexData()->trueColors() + indices[0], vertexData()->trueColors() + indices[1], vertexData()->trueColors() + indices[2],
                                           pNormal, pNormal, pNormal);
            else
              m_metaWrite.glPackNTriangle(vertexDataList() + indices[0], vertexDataList() + indices[1], vertexDataList() + indices[2],
                                          pNormal, pNormal, pNormal);
          }
        }
      }
    }
    TD_OGL_REALLOC_CATCH
  }
  else
  {
    OdGsOpenGLVectorizeView::triangleOut(indices, pNormal);
  }
}

void OdGsOpenGLStreamVectorizeView::prepareShMFacesGeneration(const OdGiFaceData* pFaceData, OdGsView::RenderMode rm)
{
  if(vertexData() && vertexData()->trueColors())
    m_bIsVertsColors = true;
  else
    m_bIsVertsColors = false;

  // Ignore normals generation for flat shaded modes. This mode can be emulated using OpenGL.
  if (m_metaWrite.isActive() && m_metaWrite.indexedMode())
  {
    if (rm == OdGsView::kFlatShaded)
      rm = OdGsView::kGouraudShaded;
    else if (rm == OdGsView::kFlatShadedWithWireframe)
      rm = OdGsView::kGouraudShadedWithWireframe;
  }

  OdGsOpenGLVectorizeView::prepareShMFacesGeneration(pFaceData, rm);
}

void OdGsOpenGLStreamVectorizeView::beginFaces(GLenum mode, OdGsView::RenderMode rm)
{
  if (m_metaWrite.isActive())
  {
    if (m_bBackfacesDiscarded)
    {
      //m_metaWrite.glEnableOpt(OdOpenGLAttribute_CullFace);
      //m_metaWrite.glCullFace(GL_BACK);
      m_metaWrite.glEnableOptToTraits(OdOpenGLAttribute_CullFace, 1);
      m_metaWrite.glCullFaceToTraits(OdOpenGLCullFace_Back, 1);
    }
    else
      m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_CullFace, 1);
    if (rm == OdGsView::k2DOptimized)
    {
      // Optimizes at least one record flushing
      m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Lighting, 1);
      m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Texture2D, 1);
    }
    if (rm == kHiddenLine)
    {
      m_metaWrite.setHLRColor((!m_bRasterImage && !m_bProcessingTTF) ? OD_OGL_OPAQUE(device()->getPaletteBackground()) : m_cCurColor);
      m_metaWrite.glHLRStencilToTraits(true, 1);
      if (!m_bRasterImage && !m_bProcessingTTF)
        m_bAcceptColorChange = false;
    }
    else if(rm>kHiddenLine)
    {
      if(m_bDontShade)
      {
        m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Lighting, 1);
        m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Texture2D, 1);
      }
      else
      {
        if (m_bRasterImage)
          m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Lighting, 1);
        else
          m_metaWrite.glEnableOptToTraits(OdOpenGLAttribute_Lighting, 1);
        if (isMaterialAvailable() && currentMaterial()->haveDiffuseTexture())
        {
          m_metaWrite.glEnableOptToTraits(OdOpenGLAttribute_Texture2D, 1);
        }
        m_metaWrite.setInsertMode((rm == kFlatShadedWithWireframe) || (rm == kGouraudShadedWithWireframe));
      }
    }
#ifdef OD_OGL_ES
    if (m_bRasterImage)
      OdGiGeometrySimplifier::m_renderMode = OdGsView::kFlatShaded;
#endif
  }
  else
  {
    OdGsOpenGLVectorizeView::beginFaces(mode, rm);
  }
}

void OdGsOpenGLStreamVectorizeView::endFaces(OdGsView::RenderMode rm)
{
  if (m_metaWrite.isActive())
  {
#ifdef OD_OGL_ES
    if (m_bRasterImage)
      OdGiGeometrySimplifier::m_renderMode = rm;
#endif
    if (rm == kHiddenLine)
    {
      m_metaWrite.glHLRStencilToTraits(false, 1, false);
      if (!m_bRasterImage && !m_bProcessingTTF)
        m_bAcceptColorChange = true;
    }
    else if (rm > kHiddenLine)
      m_metaWrite.setInsertMode(false);
    if (m_bBackfacesDiscarded)
    {
      //m_metaWrite.glDisableOpt(OdOpenGLAttribute_CullFace);
      m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_CullFace, 1, false);
    }
  }
  else
  {
    OdGsOpenGLVectorizeView::endFaces(rm);
  }
}

bool OdGsOpenGLStreamVectorizeView::isSMIndexable(const OdGiEdgeData* edgeData, const OdGiFaceData* faceData,
                                                  const OdGiVertexData* vertexData, bool bShell)
{
#ifndef OD_OGL_ENABLEINDEXEDTEXCOORDS
  if (isTextureCoordinatesEnabled(renderMode()))
    return false;
#endif
  // The gradient hatch is not shaded in Acad, it can contain indexed triangles with
  // different normals. Here I go by another way - don't use indexation in this case.
  //if (bShell && (vertexData != NULL && vertexData->trueColors() != NULL && vertexData->normals() == NULL))
  //  return false;
  if (bShell && GETBIT(effectiveTraits().drawFlags(), OdGiSubEntityTraits::kDrawGradientFill | OdGiSubEntityTraits::kDrawContourFill))
    return false;
  // Don't use indexation if layers can be changed. In this case different metafiles will be created for
  // different layers.
  if (faceData != NULL && faceData->layerIds() != NULL)
    return false;
#ifndef OD_OGL_ENABLEINDEXEDFACENORMALS
  if ((renderMode() == OdGsView::kFlatShaded || renderMode() == OdGsView::kFlatShadedWithWireframe) &&
       vertexData != NULL && vertexData->normals() != NULL)
    return false;
#endif
  return true;
}

void OdGsOpenGLStreamVectorizeView::shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                                              OdInt32 faceListSize, const OdInt32* faceList,
                                              const OdGiEdgeData* edgeData,
                                              const OdGiFaceData* faceData,
                                              const OdGiVertexData* vertexData)
{
#ifdef OD_OGL_ES
  if (numVertices > OdInt32(0xFFFF))
  {
    subdivideShellByVertexLimit(numVertices, vertexList, faceListSize, faceList, edgeData, faceData, vertexData, 0xFFFF);
    return;
  }
#endif
  if (m_metaWrite.isActive())
  {
#ifdef OD_OGL_SUPPORTINDEXEDMODE
    if (m_metaWrite.indexedMode() || numVertices > OD_OGL_MINSHELLINDEXED)
    {
      if (!isSMIndexable(edgeData, faceData, vertexData, true))
        m_metaWrite.flushOnIndexedMode(false);
      else
        m_metaWrite.beginIndexedMode(vertexData != NULL && vertexData->trueColors() != NULL,
                                     renderMode() > OdGsView::kHiddenLine,
#ifdef OD_OGL_ENABLEINDEXEDTEXCOORDS
                                     isTextureCoordinatesEnabled(renderMode()),
#else
                                     false,
#endif
                                     numVertices, vertexList);
    }
#endif
    if (!m_metaWrite.indexedMode())
    {
      m_metaWrite.reserveBuffers(numVertices);
    }
    OdGiGeometrySimplifier::shellProc(numVertices, vertexList, faceListSize, faceList, edgeData, faceData, vertexData);
  }
  else
  {
    OdGsOpenGLVectorizeView::shellProc(numVertices, vertexList, faceListSize, faceList, edgeData, faceData, vertexData);
  }
}

void OdGsOpenGLStreamVectorizeView::meshProc(OdInt32 numRows, 
                                             OdInt32 numColumns,
                                             const OdGePoint3d* vertexList,
                                             const OdGiEdgeData* edgeData,
                                             const OdGiFaceData* faceData,
                                             const OdGiVertexData* vertexData)
{
  if (m_metaWrite.isActive())
  {
#ifdef OD_OGL_SUPPORTINDEXEDMODE
    if (m_metaWrite.indexedMode() || numRows * numColumns > OD_OGL_MINSHELLINDEXED)
    {
      if (!isSMIndexable(edgeData, faceData, vertexData, false))
        m_metaWrite.flushOnIndexedMode(false);
      else
        m_metaWrite.beginIndexedMode(vertexData != NULL && vertexData->trueColors() != NULL,
                                     renderMode() > OdGsView::kHiddenLine,
#ifdef OD_OGL_ENABLEINDEXEDTEXCOORDS
                                     isTextureCoordinatesEnabled(renderMode()),
#else
                                     false,
#endif
                                     numRows * numColumns, vertexList);
    }
#endif
    if (!m_metaWrite.indexedMode())
    {
      m_metaWrite.reserveBuffers(numRows * numColumns);
    }
    OdGiGeometrySimplifier::meshProc(numRows, numColumns, vertexList, edgeData, faceData, vertexData);
  }
  else
  {
    OdGsOpenGLVectorizeView::meshProc(numRows, numColumns, vertexList, edgeData, faceData, vertexData);
  }
}

class OdOpenGLRasterRef : public OdRxObject, public OdGsOpenGLStreamVectorizeDevice::RasterImageCacheCallback
#ifdef OD_OGL_TEXTURESHARING
                        , public OdOpenGLResourceSharingProvider::OdOpenGLResourceShareRef
#endif
{
  public:
    OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *m_pEntry;
    OdGsOpenGLStreamVectorizeDevice *m_pDevice;

    void rasterImageCacheDestroyed() { m_pDevice = NULL; }

    OdOpenGLRasterRef() : m_pEntry(NULL), m_pDevice(NULL) { }
    ~OdOpenGLRasterRef()
    {
      if (m_pDevice)
      {
        m_pDevice->decrementRasterImageRef(m_pEntry, this);
      }
    }

#ifdef OD_OGL_TEXTURESHARING
    const void* rsrAccessPtr(int n) const
    {
      if (m_pDevice)
      {
        if (!n && !m_pEntry->m_texData.isNull()) return m_pEntry->m_texData->m_pData;
        else if (n && !m_pEntry->m_palData.isNull()) return m_pEntry->m_palData->m_pData;
        return NULL;
      }
      else
      {
        ODA_ASSERT(!m_sharedDefs.empty());
        return (!n) ? m_sharedDefs.begin()->second->m_pBasePtr : m_sharedDefs.begin()->second->m_pSecondPtr;
      }
    }
    OdRxObject *rsrAccessObj(int n) const
    {
      if (m_pDevice)
      {
        return (!n) ? m_pEntry->m_texData : m_pEntry->m_palData;
      }
      else
      {
        ODA_ASSERT(!m_sharedDefs.empty());
        return (!n) ? m_sharedDefs.begin()->second->m_pLock : m_sharedDefs.begin()->second->m_pSecondLock;
      }
    }
#ifdef OD_OGL_ENABLETEXBINDING
    bool m_bBindReqCheck;
    GLint m_secBind;
    void rsrRenderToBind(const void* /*pPtr*/, const void* /*pSecPtr*/, GLint secBind)
    {
      m_bBindReqCheck = true;
      m_secBind = secBind;
    }
#endif
#endif
};

bool OdGsOpenGLStreamVectorizeView::isTextureLoaded(const OdGiRasterImage* pImage, 
                                                    bool transparency,
                                                    ODCOLORREF fgColor, // only if transparency enabled
                                                    bool secColor,
                                                    ODCOLORREF bkColor, // only if secColor enabled
                                                    double brightness,
                                                    double contrast,
                                                    double fade,
                                                    const OdUInt8 *&/*pData*/,
                                                    OdUInt32 &/*bufSize*/,
                                                    const OdUInt8 *&/*pPalData*/,
                                                    OdUInt32 &lutSize)
{
  if (m_metaWrite.isActive())
  {
    OdGsOpenGLStreamVectorizeDevice::RasterImageInfo riInfo;
    riInfo.m_pImg = pImage;
    riInfo.m_bTransparency = transparency;
    riInfo.m_fgColor = fgColor;
    riInfo.m_bSecColor = secColor;
    riInfo.m_bkColor = bkColor;
    riInfo.m_fBrightness = brightness;
    riInfo.m_fContrast = contrast;
    riInfo.m_fFade = fade;
    OdGsOpenGLStreamVectorizeDevice *pDevice = static_cast<OdGsOpenGLStreamVectorizeDevice *>(device());
    OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *ricEntry = (pImage->imageSource() == OdGiRasterImage::kFromFile) ? pDevice->findRasterImage(riInfo) : NULL;
    if (ricEntry)
    {
      OdSmartPtr<OdOpenGLRasterRef> pRef = OdRxObjectImpl<OdOpenGLRasterRef>::createObject();
      pRef->m_pEntry = ricEntry;
      pRef->m_pDevice = pDevice;
      m_pTexObject = pRef;
      lutSize = ricEntry->m_uLutSize;
      return true;
    }
    else
    {
      m_procTexInfo = riInfo;
    }
  }
  return false;
}

void OdGsOpenGLStreamVectorizeView::glInitTexture(OdOpenGLAlphaTestType bEnableAlphaTest, GLint glFormat, GLsizei pxWidth, GLsizei pxHeight,
                                                  const void *pTexture, GLint glFilter, GLint glEnvMode,
                                                  const OdGeQuaternion &glSPlane, const OdGeQuaternion &glTPlane,
                                                  OdUInt32 bufSize, bool bBitonal, const void *pPalTexture, OdUInt32 lutSize)
{
  if (m_metaWrite.isActive())
  {
    OdOpenGLMetafileReaderCallback::RasterImageEntrySet dataSet;
    OdSmartPtr<OdOpenGLRasterRef> pRef = m_pTexObject;
    dataSet.bEnableAlphaTest = bEnableAlphaTest;
    dataSet.glFormat = glFormat;
    dataSet.pxWidth = pxWidth;
    dataSet.pxHeight = pxHeight;
    dataSet.pTexture = pTexture;
    dataSet.glFilter = glFilter;
    dataSet.glEnvMode = glEnvMode;
    dataSet.glSPlane = glSPlane;
    dataSet.glTPlane = glTPlane;
    dataSet.bufSize = bufSize;
    dataSet.bBitonal = bBitonal;
    dataSet.pPalTexture = pPalTexture;
    dataSet.lutSize = lutSize;
    OdGsOpenGLStreamVectorizeDevice *pDevice = static_cast<OdGsOpenGLStreamVectorizeDevice *>(device());
    if (pRef.isNull())
    {
#ifndef OD_OGL_ENABLETEXBINDING
      OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *ricEntry = pDevice->addRasterImage(m_procTexInfo, pTexture, bufSize, pPalTexture, lutSize);
#else
      GLuint gtt = 0xFFFFFFFF, gtt2 = 0xFFFFFFFF;
      if (!isMTRegen())
      {
        ::glEnable(GL_TEXTURE_2D);
        ::glGenTextures(1, &gtt);
        ::glBindTexture(GL_TEXTURE_2D, gtt);
        OdGsOpenGLVectorizeView::glInitTexture(bEnableAlphaTest, glFormat, pxWidth, pxHeight, pTexture, glFilter, glEnvMode,
                                               glSPlane, glTPlane, bufSize, bBitonal, NULL, 0);
        ::glBindTexture(GL_TEXTURE_2D, 0);
        ::glDisable(GL_TEXTURE_2D);
        ::glDisable(GL_ALPHA_TEST);
        ::glDisable(GL_BLEND);
        if (lutSize)
        {
          ::glEnable(GL_TEXTURE_2D);
          ::glGenTextures(1, &gtt2);
          ::glBindTexture(GL_TEXTURE_2D, gtt2);
          OdGsOpenGLVectorizeView::glInitLutTexture(pPalTexture, lutSize);
          ::glBindTexture(GL_TEXTURE_2D, 0);
          ::glDisable(GL_TEXTURE_2D);
        }
      } // init on-demand for mt views
#ifndef OD_OGL_TEXTURESHARING
      OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *ricEntry = pDevice->addRasterImage(m_procTexInfo, gtt, lutSize, gtt2);
#else
      OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *ricEntry = pDevice->addRasterImage(m_procTexInfo, pTexture, bufSize, gtt, pPalTexture, lutSize, gtt2);
#endif
#endif
      pRef = OdRxObjectImpl<OdOpenGLRasterRef>::createObject();
      pRef->m_pEntry = ricEntry;
      pRef->m_pDevice = pDevice;
    }
    else
    {
      m_pTexObject.release();
    }
    pDevice->incrementRasterImageRef(pRef->m_pEntry, pRef.get());
    m_metaWrite.glInitTexture(&dataSet, sizeof(OdOpenGLMetafileReaderCallback::RasterImageEntry), pRef,
                              dataSet.pTexture, dataSet.bufSize, dataSet.pPalTexture, dataSet.lutSize * 4);
#ifdef OD_OGL_ES
    glInitTextureTransform(bEnableAlphaTest, bBitonal, glEnvMode, glSPlane, glTPlane);
#endif
  }
  else
  {
    OdGsOpenGLVectorizeView::glInitTexture(bEnableAlphaTest, glFormat, pxWidth, pxHeight, pTexture, glFilter, glEnvMode,
                                           glSPlane, glTPlane, bufSize, bBitonal, pPalTexture, lutSize);
  }
}

bool OdGsOpenGLStreamVectorizeView::canUseLut() const
{
#if defined(GL_ARB_fragment_program) && defined(OD_OGL_ENABLETEXBINDING)
  return const_cast<OdGsOpenGLStreamVectorizeView*>(this)->device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_ARB_FragProg) &&
         m_metaWrite.isActive(); // We can't prepare shader in case if no metafile processing
#else
  return false;
#endif
}

void OdGsOpenGLStreamVectorizeView::glBindSecondary(GLint secondBinding, OdUInt32 lutSize)
{
#ifdef OD_OGL_ENABLETEXBINDING
   ((OdGLFn_ActiveTexture_ARB)device()->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_ActiveTexture))(GL_TEXTURE0 + 1);
   ::glBindTexture(GL_TEXTURE_2D, secondBinding);
   ((OdGLFn_ActiveTexture_ARB)device()->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_ARB_ActiveTexture))(GL_TEXTURE0);
   if (!m_bSecondaryBinding && secondBinding)
   {
     OdOpenGLFragmentProgram &pProg = static_cast<OdGsOpenGLStreamVectorizeDevice *>(device())->
                                      fragmentProgram(OdGsOpenGLStreamVectorizeDevice::kPaletteShader);
     if (!pProg.isInited())
       pProg.initShad(device(), g_fp10_PaletteShader_main);
     if (pProg.canBind())
     {
      pProg.bind(device());
      pProg.setParam(device(), 1, (GLfloat)lutSize, 0.0f, ::glIsEnabled(GL_FOG) ? 0.5f : 0.0f, 1.0f);
     }
   }
   else if (m_bSecondaryBinding && !secondBinding)
   {
     OdOpenGLFragmentProgram &pProg = static_cast<OdGsOpenGLStreamVectorizeDevice *>(device())->
                                      fragmentProgram(OdGsOpenGLStreamVectorizeDevice::kPaletteShader);
     if (pProg.canBind())
      pProg.unbind(device());
   }
#endif
   m_bSecondaryBinding = (secondBinding != 0);
}

void OdGsOpenGLStreamVectorizeView::uninitTexture()
{
  if (m_metaWrite.isActive())
  {
    m_bRasterImage = false;
    m_metaWrite.glUninitTexture();
    m_metaWrite.glDropOpt(OdOpenGLAttribute_Texture2D);
  }
  else
  {
    OdGsOpenGLVectorizeView::uninitTexture();
    ::glDisable(GL_BLEND);
  }
}

void OdGsOpenGLStreamVectorizeView::metafileProcessInitTexture(const OdOpenGLMetafileReaderCallback::RasterImageEntrySet &dataSet, OdRxObject *pData)
{
  if (!pData)
  {
    this->glInitTexture(dataSet.bEnableAlphaTest, dataSet.glFormat, dataSet.pxWidth, dataSet.pxHeight,
                        dataSet.pTexture, dataSet.glFilter, dataSet.glEnvMode,
                        dataSet.glSPlane, dataSet.glTPlane, dataSet.bufSize, dataSet.bBitonal,
                        dataSet.pPalTexture, dataSet.lutSize);
  }
  else
  {
    OdOpenGLRasterRef *pRef = static_cast<OdOpenGLRasterRef*>(pData);
#ifdef OD_OGL_TEXTURESHARING
    if (pRef->m_pDevice == device())
    {
      OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *ricEntry = pRef->m_pEntry;
#ifndef OD_OGL_ENABLETEXBINDING
      this->glInitTexture(dataSet.bEnableAlphaTest, dataSet.glFormat, dataSet.pxWidth, dataSet.pxHeight,
                          ricEntry->m_texData->m_pData, dataSet.glFilter, dataSet.glEnvMode,
                          dataSet.glSPlane, dataSet.glTPlane, ricEntry->m_texData->m_uSize, dataSet.bBitonal,
                          ricEntry->m_palData->m_pData, ricEntry->m_uLutSize);
#else
      if (ricEntry->m_uBinding == 0xFFFFFFFF)
      {
        // We doesn't initialized yet, so initialize on-demand
        ::glEnable(GL_TEXTURE_2D);
        GLuint gtt = 0;
        ::glGenTextures(1, &gtt);
        ::glBindTexture(GL_TEXTURE_2D, gtt);
        OdGsOpenGLVectorizeView::glInitTexture(dataSet.bEnableAlphaTest, dataSet.glFormat, dataSet.pxWidth, dataSet.pxHeight,
          pRef->rsrAccessPtr(0), dataSet.glFilter, dataSet.glEnvMode, dataSet.glSPlane, dataSet.glTPlane,
          static_cast<OdOpenGLArrayWrapperShared*>(pRef->rsrAccessObj(0))->m_uSize, dataSet.bBitonal, NULL, 0);
        ricEntry->m_uBinding = gtt;
        if (ricEntry->m_uLutSize)
        {
          ::glGenTextures(1, &gtt);
          ::glBindTexture(GL_TEXTURE_2D, gtt);
          OdGsOpenGLVectorizeView::glInitLutTexture(pRef->rsrAccessPtr(1), ricEntry->m_uLutSize);
          ricEntry->m_uPalBinding = gtt;
        }
        ::glBindTexture(GL_TEXTURE_2D, 0);
      }
      ::glEnable(GL_TEXTURE_2D);
      ::glBindTexture(GL_TEXTURE_2D, ricEntry->m_uBinding);
      glInitTextureTransform(dataSet.bEnableAlphaTest, dataSet.bBitonal, dataSet.glEnvMode, dataSet.glSPlane, dataSet.glTPlane);
      if (ricEntry->m_uLutSize)
        glBindSecondary(ricEntry->m_uPalBinding, ricEntry->m_uLutSize);
#endif
    }
    else
    {
      OdOpenGLResourceSharingProvider::OdOpenGLResourceShareEntry *pEnt = pRef->getShared(device());
      if (!pEnt)
      {
#ifdef OD_OGL_ENABLETEXBINDING
        pRef->m_bBindReqCheck = false;
#endif
        pEnt = pRef->createShared(device(), &static_cast<OdGsOpenGLStreamVectorizeDevice*>(device())->sharingProvider());
#ifdef OD_OGL_ENABLETEXBINDING
        if (pRef->m_bBindReqCheck)
        {
          OdGsOpenGLVectorizeView::glInitTexture(dataSet.bEnableAlphaTest, dataSet.glFormat, dataSet.pxWidth, dataSet.pxHeight,
            pRef->rsrAccessPtr(0), dataSet.glFilter, dataSet.glEnvMode, dataSet.glSPlane, dataSet.glTPlane,
            static_cast<OdOpenGLArrayWrapperShared*>(pRef->rsrAccessObj(0))->m_uSize, dataSet.bBitonal, NULL, 0);
            if (dataSet.lutSize)
            {
              ::glBindTexture(GL_TEXTURE_2D, pRef->m_secBind);
              glInitLutTexture(pRef->rsrAccessPtr(1), dataSet.lutSize);
            }
          ::glBindTexture(GL_TEXTURE_2D, 0);
        }
#endif
      }
#ifndef OD_OGL_ENABLETEXBINDING
      this->glInitTexture(dataSet.bEnableAlphaTest, dataSet.glFormat, dataSet.pxWidth, dataSet.pxHeight,
        pEnt->m_pBasePtr, dataSet.glFilter, dataSet.glEnvMode,
        dataSet.glSPlane, dataSet.glTPlane, static_cast<OdOpenGLArrayWrapperShared*>(pEnt->m_pLock.get())->m_uSize, dataSet.bBitonal,
        pEnt->m_pSecondPtr, dataSet.lutSize);
#else
      ::glEnable(GL_TEXTURE_2D);
      ::glBindTexture(GL_TEXTURE_2D, pEnt->m_uBinding);
      glInitTextureTransform(dataSet.bEnableAlphaTest, dataSet.bBitonal, dataSet.glEnvMode, dataSet.glSPlane, dataSet.glTPlane);
      if (dataSet.lutSize)
        glBindSecondary(pEnt->m_uSecondBinding, dataSet.lutSize);
#endif
    }
#else
    OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *ricEntry = pRef->m_pEntry;
#ifndef OD_OGL_ENABLETEXBINDING
    this->glInitTexture(dataSet.bEnableAlphaTest, dataSet.glFormat, dataSet.pxWidth, dataSet.pxHeight,
                        ricEntry->m_texData.m_pData, dataSet.glFilter, dataSet.glEnvMode,
                        dataSet.glSPlane, dataSet.glTPlane, ricEntry->m_texData.m_uSize, dataSet.bBitonal,
                        ricEntry->m_palData.m_pData, dataSet.lutSize);
#else
    ::glEnable(GL_TEXTURE_2D);
    ::glBindTexture(GL_TEXTURE_2D, ricEntry->m_uBinding);
    glInitTextureTransform(dataSet.bEnableAlphaTest, dataSet.bBitonal, dataSet.glEnvMode, dataSet.glSPlane, dataSet.glTPlane);
    if (dataSet.lutSize)
      glBindSecondary(pEnt->m_uPalBinding, dataSet.lutSize);
#endif
#endif
  }
}

void OdGsOpenGLStreamVectorizeView::metafileProcessUninitTexture()
{
#ifdef OD_OGL_ENABLETEXBINDING
  ::glBindTexture(GL_TEXTURE_2D, 0);
  if (m_bSecondaryBinding)
    glBindSecondary(0, 0);
#endif
  uninitTexture();
  m_metaRead.restoreOption(OdOpenGLAttribute_Texture2D);
}

#ifdef OD_OGL_ES
void OdGsOpenGLStreamVectorizeView::metafilePushStencilAttrib()
{
  m_esStencilSaver.pushAttrib();
}
void OdGsOpenGLStreamVectorizeView::metafilePopStencilAttrib()
{
  m_esStencilSaver.popAttrib();
}
#endif

bool OdGsOpenGLStreamVectorizeView::isMappingDelayed() const
{
#ifdef OD_OGL_ES
  if (m_bRasterImage)
    return false;
#endif
  if (!isTextureCoordinatesPredefined())
    return OdGsBaseMaterialVectorizer::isMappingDelayed();
  return false;
}

// Materials support

void OdGsOpenGLStreamVectorizeView::renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId)
{
  if (m_metaWrite.isActive())
  {
    if (m_bCheckFaceMaterials) checkFaceMaterialChange();
    m_metaWrite.glAppendMaterial((OdRxObjectPtr)pCache);
    m_metaWrite.glDropOpt(OdOpenGLAttribute_Texture2D);
  }
  else
  {
    OdGsOpenGLVectorizeView::renderMaterialCache(pCache, materialId);
  }
}

void OdGsOpenGLStreamVectorizeView::metafileProcessMaterial(OdRxObject *pMetafile)
{
  renderMaterial(static_cast<OdGiMaterialItem*>(pMetafile));
  // Restore affected options state
  m_metaRead.restoreOption(OdOpenGLAttribute_Texture2D);
}

class OdGsOpenGLDelayedMappingEntry : public OdGsBaseMaterialView::DelayCacheEntry
                                    , public OdOpenGLDelayedMappingEntry
{
  public:
    OdGsOpenGLDelayedMappingEntry() : OdGsBaseMaterialView::DelayCacheEntry()
                                    , OdOpenGLDelayedMappingEntry()
    {
    }
};

void OdGsOpenGLStreamVectorizeView::mapperChangedForDelayCache()
{
  OdGsBaseMaterialVectorizer::mapperChangedForDelayCache();
  if (m_metaWrite.isActive())
  {
    OdGsOpenGLDelayedMappingEntry *pEntry = new OdGsOpenGLDelayedMappingEntry();
    appendDelayCacheEntry(pEntry);
    m_metaWrite.startDelayMappingEntry(pEntry);
  }
}

void OdGsOpenGLStreamVectorizeView::playDelayCacheEntry(const DelayCacheEntry *pEntry)
{
  const OdGsOpenGLDelayedMappingEntry *pMapEntry = static_cast<const OdGsOpenGLDelayedMappingEntry*>(pEntry);
  OdOpenGLFlatMetafileContainer *pContainer = static_cast<OdOpenGLFlatMetafileContainer*>(pMapEntry->pContainer);
#ifdef OD_OGL_ENABLEFLOATCONVERSION
  typedef GLfloat _itl;
#else
  typedef double _itl;
#endif
  OdGePoint2d texCoords[3];
  for (OdUInt32 nElem = 0; nElem < pMapEntry->m_Elements.size(); nElem++)
  {
    const OdOpenGLDelayedMappingEntry::ArrayElement &elem = pMapEntry->m_Elements.at(nElem);
    currentMapper()->diffuseMapper()->mapCoords(elem.verts, elem.normal, texCoords);
    if (pMapEntry->m_texIds.size() <= elem.texId)
    {
      // The array is not flushed currently. We could set coordinates into intermediate buffer.
      m_metaWrite.glResetTexCoords(elem.crdIds, texCoords);
    }
    else
    {
      _itl *pArray = (_itl*)pContainer->m_ArrayElements.at(pMapEntry->m_texIds.at(elem.texId)).m_pData;
      _itl *pTexCoord;
      for (int x = 0; x < 3; x++)
      {
        pTexCoord = pArray + (elem.crdIds[x] << 1);
        pTexCoord[0] = (_itl)texCoords[x].x;
        pTexCoord[1] = (_itl)texCoords[x].y;
      }
    }
  }
}

void OdGsOpenGLStreamVectorizeView::delayCacheProcessed(const OdGiDrawable *pDrawable)
{
  OdGsBaseMaterialVectorizer::delayCacheProcessed(pDrawable);
  if (m_metaWrite.isActive())
  {
    m_metaWrite.clearDelayMappingCache();
  }
}

//

void OdGsOpenGLStreamVectorizeView::display(bool update)
{
  // For future modifications
  OdGsOpenGLVectorizeView::display(update);
}

// TtfFonts cache support

OdRxObjectPtr OdGsOpenGLStreamVectorizeDevice::tfcNewMetafile(void *pSessionId)
{
  OdGsOpenGLStreamVectorizeView *pView = reinterpret_cast<OdGsOpenGLStreamVectorizeView*>(pSessionId);
  return pView->tfcNewMetafile();
}

OdGiConveyorGeometry *OdGsOpenGLStreamVectorizeDevice::tfcBeginMetafile(OdRxObject *pMetafile, void *pSessionId)
{
  OdGsOpenGLStreamVectorizeView *pView = reinterpret_cast<OdGsOpenGLStreamVectorizeView*>(pSessionId);
  return pView->tfcBeginMetafile(pMetafile);
}

void OdGsOpenGLStreamVectorizeDevice::tfcFinalizeMetafile(OdRxObject *pMetafile, void *pSessionId)
{
  OdGsOpenGLStreamVectorizeView *pView = reinterpret_cast<OdGsOpenGLStreamVectorizeView*>(pSessionId);
  return pView->tfcFinalizeMetafile(pMetafile);
}

void OdGsOpenGLStreamVectorizeDevice::tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile, void *pSessionId) const
{
  OdGsOpenGLStreamVectorizeView *pView = reinterpret_cast<OdGsOpenGLStreamVectorizeView*>(pSessionId);
  return pView->tfcPlayMetafile(xForm, pMetafile);
}

bool OdGsOpenGLStreamVectorizeDevice::tfcSaveMetafile(const OdRxObject* pMetafile, OdGsFiler *pFiler)
{
  return VectorizerAutoPtr(*static_cast<OdGsBaseVectorizeView*>(viewAt(0)), true)->saveMetafile(pMetafile, pFiler);
}

OdRxObjectPtr OdGsOpenGLStreamVectorizeDevice::tfcLoadMetafile(OdGsFiler *pFiler)
{
  return VectorizerAutoPtr(*static_cast<OdGsBaseVectorizeView*>(viewAt(0)), true)->loadMetafile(pFiler);
}

OdRxObjectPtr OdGsOpenGLStreamVectorizeView::tfcNewMetafile()
{
  return newGsMetafile();
}

OdGiConveyorGeometry *OdGsOpenGLStreamVectorizeView::tfcBeginMetafile(OdRxObject *pMetafile)
{
  beginMetafile(pMetafile);
  return this;
}

void OdGsOpenGLStreamVectorizeView::tfcFinalizeMetafile(OdRxObject *pMetafile)
{
  endMetafile(pMetafile);
}

void OdGsOpenGLStreamVectorizeView::tfcPlayMetafile(const OdGeMatrix3d &xForm, const OdRxObject *pMetafile) const
{
  OdGsOpenGLStreamVectorizeView* pThis = const_cast<OdGsOpenGLStreamVectorizeView*>(this);
  pThis->pushMetafileTransform(xForm, kSharedRefTransform);
  static_cast<const OdOpenGLStreamList*>(pMetafile)->play(pThis);
  pThis->popMetafileTransform(kSharedRefTransform);
}

void OdGsOpenGLStreamVectorizeView::metafileTtfText(OdUInt64 fontKeyA, OdUInt64 fontKeyB, const OdUInt32 *pText, OdUInt32 nText,
                                                    const OdGeMatrix3d &baseTransform, const OdGeVector3d &stepDir)
{
  static_cast<OdGsOpenGLStreamVectorizeDevice*>(device())->ttfFontsCache()->playText(OdTtfFontsCache::FontKey(fontKeyA, fontKeyB),
                                                                                     pText, nText, baseTransform, stepDir, this);
}

void OdGsOpenGLStreamVectorizeView::textProc(const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
                                             const OdChar* msg, OdInt32 nLength, bool raw, const OdGiTextStyle* pStyle, const OdGeVector3d* pExtrusion)
{
  OdTtfFontsCache::TextInfo ttfInfo;
  if (pExtrusion || !m_metaWrite.isActive() ||
      (renderMode() == OdGsView::k2DOptimized && giCtx().quickTextMode()))
  {
    OdGiGeometrySimplifier::textProc(position, u, v, msg, nLength, raw, pStyle, pExtrusion);
    return;
  }
  OdGiFillType fill;
  { // Emulate GeometrySimplifier behavior
    m_bProcessingTTF = m_bDontShade = true;
    fill = m_pTraits->fillType();
    m_pTraits->setFillType(giCtx().fillTtf() ? kOdGiFillAlways : kOdGiFillNever);
    m_pTraits->setDrawFlags(m_pTraits->drawFlags() | OdGiSubEntityTraits::kDrawPolygonFill);
    m_pDrawCtx->onTraitsModified();
  }
  // Do that generateShellFaces done, to setup all properties once on metafile beginning
  m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Lighting, 1);
  m_metaWrite.glDisableOptToTraits(OdOpenGLAttribute_Texture2D, 1);
  if (renderMode() == OdGsView::k2DOptimized)
    m_metaWrite.glLineweight(OdDb::kLnWt000);
  // Process text string
  {
    OdGePoint3d posPt = position;
    OdGeVector3d uVec = u, vVec = v;
    if (metafileTransformOverrided())
    {
      const OdGeMatrix3d &xForm = metafileToWorldMatrix();
      posPt.transformBy(xForm);
      uVec.transformBy(xForm);
      vVec.transformBy(xForm);
    }
    if (!static_cast<OdGsOpenGLStreamVectorizeDevice*>(device())->ttfFontsCache()->processText(posPt, uVec, vVec,
         msg, nLength, raw, pStyle, this, ttfInfo, this))
    {
      OdGiGeometrySimplifier::textProc(position, u, v, msg, nLength, raw, pStyle, pExtrusion);
      return;
    }
  }
  // Pack text
  m_metaWrite.glTtfText(ttfInfo.m_fontKey.first, ttfInfo.m_fontKey.second, ttfInfo.m_textString.getPtr(),
                        ttfInfo.m_textString.size(), ttfInfo.m_baseTransform, ttfInfo.m_stepDir);
  { // Restore changed traits
    m_pTraits->setFillType(fill);
    m_pTraits->setDrawFlags(m_pTraits->drawFlags() & ~OdGiSubEntityTraits::kDrawPolygonFill);
    m_pDrawCtx->onTraitsModified();
    m_bProcessingTTF = m_bDontShade = false;
  }
}

// Binding of material textures

#ifdef OD_OGL_ENABLETEXBINDING

// Texture data container
class OdGiOpenGLBindableMaterialTextureData : public OdGiOpenGLMaterialTextureData,
                                              protected OdGsOpenGLStreamVectorizeDevice::RasterImageCacheCallback
#ifdef OD_OGL_TEXTURESHARING
                                            , protected OdOpenGLResourceSharingProvider::OdOpenGLResourceShareRef
#endif
{
  protected:
    GLuint m_uPrevTexBind;
    GLuint m_uTexBind;
    OdGsOpenGLStreamVectorizeDevice *m_pDevice;
  public:
    OdGiOpenGLBindableMaterialTextureData()
      : OdGiOpenGLMaterialTextureData()
      , m_uPrevTexBind(0xFFFFFFFF)
      , m_uTexBind(0xFFFFFFFF)
      , m_pDevice(NULL)
    {
    }
    ~OdGiOpenGLBindableMaterialTextureData()
    {
      unbindTexture();
      if (m_pDevice)
        m_pDevice->removeResourceCallback(this);
    }

    void unbindTexture()
    {
      if (m_uPrevTexBind != 0xFFFFFFFF)
        m_uTexBind = m_uPrevTexBind;
      if (m_uTexBind != 0xFFFFFFFF)
      {
        OdOpenGLWrapResourceRelease _wResDel(m_pDevice);
        ::glBindTexture(GL_TEXTURE_2D, 0);
        ::glDeleteTextures(1, &m_uTexBind);
        m_uPrevTexBind = m_uTexBind = 0xFFFFFFFF;
      }
    }

    void rasterImageCacheDestroyed()
    {
      unbindTexture();
      m_pDevice = NULL;
    }

    void updateOnCreate(OdGiMaterialTextureData::DevDataVariant pDeviceInfo)
    {
      m_uTexBind = 0xFFFFFFFF;
      if (m_pDevice == NULL)
      {
        m_pDevice = (OdGsOpenGLStreamVectorizeDevice*)pDeviceInfo.getPtr();
        m_pDevice->addResourceCallback(this);
      }
    }

    void setTextureData(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, const OdGiImageBGRA32 &image)
    {
      OdGiOpenGLMaterialTextureData::setTextureData(pDeviceInfo, image);
      updateOnCreate(pDeviceInfo);
    }

#ifdef OD_OGL_TEXTURESHARING
    const void* rsrAccessPtr(int n) const { return (!n) ? m_pxArray.getPtr() : NULL; }
    void rsrRenderToBind(const void* /*pPtr*/, const void* /*pSecPtr*/, GLint /*secBind*/)
    {
      OdGiOpenGLMaterialTextureData::renderTexture(NULL);
      ::glBindTexture(GL_TEXTURE_2D, 0);
    }
#endif

    void renderTexture(OdGsOpenGLVectorizeDevice *pDevice)
    {
      if (m_pxOpacity.size() == 0)
      {
#ifdef OD_OGL_TEXTURESHARING
        if (pDevice == m_pDevice)
        {
#endif
        if (m_uTexBind == 0xFFFFFFFF)
        {
          unbindTexture();
          ::glBindTexture(GL_TEXTURE_2D, 0);
          ::glGenTextures(1, &m_uTexBind);
          ::glBindTexture(GL_TEXTURE_2D, m_uTexBind);
          m_uPrevTexBind = m_uTexBind;
          OdGiOpenGLMaterialTextureData::renderTexture(pDevice);
          //::glBindTexture(GL_TEXTURE_2D, 0); // Don't unbind here
        }
        else
        {
          ::glBindTexture(GL_TEXTURE_2D, m_uTexBind);
        }
#ifdef OD_OGL_TEXTURESHARING
        }
        else
        {
          OdOpenGLResourceSharingProvider::OdOpenGLResourceShareEntry *pEnt = getShared(pDevice);
          if (pEnt == NULL)
            pEnt = createShared(pDevice, &static_cast<OdGsOpenGLStreamVectorizeDevice*>(pDevice)->sharingProvider());
          ::glBindTexture(GL_TEXTURE_2D, pEnt->m_uBinding);
        }
#endif
      }
      else
      {
        OdGiOpenGLMaterialTextureData::renderTexture(pDevice);
      }
    }

    // RxClass
    class Desc2 : public OdGiOpenGLMaterialTextureData::Desc
    {
      public:
        OdRxObjectPtr create() const
        {
          return OdRxObjectPtr(OdRxObjectImpl<OdGiOpenGLBindableMaterialTextureData>::createObject());
        }
    };
};

OdRxClassPtr OdGsOpenGLStreamVectorizeView::createMaterialDesc() const
{
  return (OdRxClassPtr)OdRxObjectPtr(OdRxObjectImpl<OdGiOpenGLBindableMaterialTextureData::Desc2>::createObject());
}

#endif

#ifdef OD_OGL_TEXTURESHARING

// Sharing provider

OdSmartPtr<OdOpenGLArrayWrapperShared> OdOpenGLArrayWrapperShared::createSharedArray()
{
  return OdRxObjectImpl<OdOpenGLArrayWrapperShared>::createObject();
}

OdOpenGLResourceSharingProvider::OdOpenGLResourceShareEntry::~OdOpenGLResourceShareEntry()
{
#ifdef OD_OGL_ENABLETEXBINDING
  if (m_uBinding != 0xFFFFFFFF)
  {
    ::glBindTexture(GL_TEXTURE_2D, 0);
    ::glDeleteTextures(1, &m_uBinding);
  }
  if (m_uSecondBinding != 0xFFFFFFFF)
  {
    ::glBindTexture(GL_TEXTURE_2D, 0);
    ::glDeleteTextures(1, &m_uSecondBinding);
  }
#endif
}

OdOpenGLResourceSharingProvider::OdOpenGLResourceShareEntry *OdOpenGLResourceSharingProvider::OdOpenGLResourceShareRef::getShared(OdGsOpenGLVectorizeDevice* pCurDevice) const
{
  sharedDefs::iterator it = m_sharedDefs.find(pCurDevice);
  if (it != m_sharedDefs.end())
    return it->second;
  return NULL;
}

OdOpenGLResourceSharingProvider::OdOpenGLResourceShareEntry *OdOpenGLResourceSharingProvider::OdOpenGLResourceShareRef::createShared(OdGsOpenGLVectorizeDevice* pCurDevice, /*this*/ OdOpenGLResourceSharingProvider *pProv) const
{
  OdOpenGLResourceShareEntry *pEntry = pProv->appendEntry(const_cast<OdOpenGLResourceShareRef*>(this), pCurDevice);
  m_sharedDefs[pCurDevice] = pEntry;
  return pEntry;
}

void OdOpenGLResourceSharingProvider::OdOpenGLResourceShareRef::unlinkShared(OdGsOpenGLVectorizeDevice* pDevice)
{
  sharedDefs::iterator it = m_sharedDefs.find(pDevice);
  if (it != m_sharedDefs.end())
  {
    it->second->m_pProv->unlinkEntry(this, pDevice, it->second);
    m_sharedDefs.erase(it);
  }
}

void OdOpenGLResourceSharingProvider::OdOpenGLResourceShareRef::unlinkShared()
{
  sharedDefs::iterator it = m_sharedDefs.begin();
  while (it != m_sharedDefs.end())
  {
    it->second->m_pProv->unlinkEntry(this, it->first, it->second);
    it++;
  }
  m_sharedDefs.clear();
}

void OdOpenGLResourceSharingProvider::onManagerDestroy()
{
  // Detach all provided resources
  shareEntries::iterator it = m_sharedEntries.begin();
  while (it != m_sharedEntries.end())
  {
    OdList<OdOpenGLResourceShareRef*>::iterator it2 = it->m_refs.begin();
    while (it2 != it->m_refs.end())
    {
      (*it2)->m_sharedDefs.erase(m_pDevice);
      it2++;
    }
    it++;
  }
  m_sharedEntries.clear();
}

OdOpenGLResourceSharingProvider::OdOpenGLResourceShareEntry *OdOpenGLResourceSharingProvider::appendEntry(OdOpenGLResourceShareRef *pRef, OdGsOpenGLVectorizeDevice* pCurDevice /*this*/)
{
  const void *pBaseResourcePtr = pRef->rsrAccessPtr(0);
  shareEntries::iterator it = m_sharedEntries.begin();
  while (it != m_sharedEntries.end())
  {
    if (it->m_pBasePtr == pBaseResourcePtr)
      break;
    it++;
  }
  if (it == m_sharedEntries.end())
  {
    it = m_sharedEntries.append();
    it->m_pBasePtr = pBaseResourcePtr;
    it->m_pLock = pRef->rsrAccessObj(0);
    it->m_pSecondPtr = pRef->rsrAccessPtr(1);
    if (it->m_pSecondPtr)
    {
      it->m_pSecondLock = pRef->rsrAccessObj(1);
#ifdef OD_OGL_ENABLETEXBINDING
      ::glGenTextures(1, &(it->m_uSecondBinding));
#endif
    }
#ifdef OD_OGL_ENABLETEXBINDING
    { // Make binding
      ::glBindTexture(GL_TEXTURE_2D, 0);
      ::glGenTextures(1, &(it->m_uBinding));
      ::glBindTexture(GL_TEXTURE_2D, it->m_uBinding);
      pRef->rsrRenderToBind(pBaseResourcePtr, it->m_pSecondPtr, it->m_uSecondBinding);
    }
#endif
    it->m_pProv = this;
  }
  *(it->m_refs.append()) = pRef;
  return &*it;
}

void OdOpenGLResourceSharingProvider::unlinkEntry(OdOpenGLResourceShareRef *pRef, OdGsOpenGLVectorizeDevice* /*pCurDevice*/, OdOpenGLResourceShareEntry *pEntry)
{
  pEntry->m_refs.remove(pRef);
  if (pEntry->m_refs.empty())
    m_sharedEntries.remove(*pEntry);
}

#endif

// OdGsFiler compatibility

bool OdGsOpenGLStreamVectorizeView::saveMetafile(const OdRxObject* pMetafile, OdGsFiler *pFiler)
{
  if (!pMetafile)
    return false;
  const OdOpenGLStreamList *pMf = static_cast<const OdOpenGLStreamList*>(pMetafile);
  pFiler->wrUInt32(pMf->arrayElementsSize());
  for (OdUInt32 nArray = 0; nArray < pMf->arrayElementsSize(); nArray++)
  {
    pFiler->wrUInt32(pMf->arrayElement(nArray).m_uSize);
    pFiler->wrRawData(pMf->arrayElement(nArray).m_pData, pMf->arrayElement(nArray).m_uSize);
  }
  pFiler->wrUInt32(pMf->m_RxObjects.size());
  if (!pMf->m_RxObjects.isEmpty())
  {
    for (OdUInt32 nObj = 0; nObj < pMf->m_RxObjects.size(); nObj++)
    {
      const OdRxObject *pObj = pMf->m_RxObjects[nObj].get();
      // Can be OdGiMaterialItem or RasterImageRef
      if (pObj->isKindOf(OdGiMaterialItem::desc()))
      {
        pFiler->wrBool(true);
        pFiler->wrPtr(pObj);
      }
      else
      {
        pFiler->wrBool(false);
        const OdOpenGLRasterRef *pRef = static_cast<const OdOpenGLRasterRef*>(pObj);
        pFiler->wrPtr(pRef->m_pEntry);
      }
    }
  }
  pFiler->wrUInt32(pMf->size());
  if (pMf->size())
    pFiler->wrRawData(pMf->memoryPtr(), pMf->size());
  return true;
}

OdRxObjectPtr OdGsOpenGLStreamVectorizeView::loadMetafile(OdGsFiler *pFiler)
{
  OdRxObjectPtr pMetafile = newGsMetafile();
  OdOpenGLStreamList *pMf = static_cast<OdOpenGLStreamList*>(pMetafile.get());
  OdUInt32 nArrays = pFiler->rdUInt32(), nArray;
  OdUInt8Vector rda;
  for (nArray = 0; nArray < nArrays; nArray++)
  {
    OdOpenGLArrayWrapper aw;
    rda.resize(pFiler->rdUInt32());
    pFiler->rdRawData(rda.asArrayPtr(), rda.size());
    aw.setArray(rda.getPtr(), rda.size());
    pMf->appendArrayElement(aw);
  }
  nArrays = pFiler->rdUInt32();
  if (nArrays)
  {
    pMf->m_RxObjects.resize(nArrays);
    for (nArray = 0; nArray < nArrays; nArray++)
    {
      bool bMaterial = pFiler->rdBool();
      OdIntPtr pPtr = pFiler->rdIntPtr();
      if (pPtr)
      {
        if (bMaterial)
          pFiler->subst()->requestSubstitution(&pMf->m_RxObjects[nArray], &pPtr);
        else
        {
          OdGsOpenGLStreamVectorizeDevice *pDevice = static_cast<OdGsOpenGLStreamVectorizeDevice*>(device());
          OdGsOpenGLStreamVectorizeDevice::RasterImageCacheEntry *pEntry = NULL;
          if (pPtr) pFiler->subst()->requestSubstitution(&pEntry, &pPtr, sizeof(OdIntPtr), false);
          OdSmartPtr<OdOpenGLRasterRef> pRef = OdRxObjectImpl<OdOpenGLRasterRef>::createObject();
          pRef->m_pEntry = pEntry;
          pRef->m_pDevice = pDevice;
          pDevice->incrementRasterImageRef(pEntry, pRef);
          pMf->m_RxObjects[nArray] = pRef;
        }
      }
    }
  }
  rda.resize(pFiler->rdUInt32());
  if (!rda.isEmpty())
  {
    pFiler->rdRawData(rda.asArrayPtr(), rda.size());
    pMf->inflateBuffer(rda.size());
    pMf->writeTo(0, rda.getPtr(), rda.size());
  }
  return pMetafile;
}

bool OdGsOpenGLStreamVectorizeView::saveClientMaterialCache(const OdRxObject* pMtl, OdGsFiler *pFiler)
{
  if (!pMtl) return false;
  const OdOpenGLMaterialDataContainer *pMatContainer = static_cast<const OdOpenGLMaterialDataContainer*>(pMtl);
  pFiler->wrRawData(&pMatContainer->data(), sizeof(OdOpenGLMaterialData));
  return true;
}

OdRxObjectPtr OdGsOpenGLStreamVectorizeView::loadClientMaterialCache(OdGsFiler *pFiler, OdGiMaterialItem * /*pMatItem*/)
{
  OdSmartPtr<OdOpenGLMaterialDataContainer> pMatContainer = OdRxObjectPtr(OdRxObjectImpl<OdOpenGLMaterialDataContainer>::createObject());
  pFiler->rdRawData(&pMatContainer->data(), sizeof(OdOpenGLMaterialData));
  return pMatContainer.get();
}

bool OdGsOpenGLStreamVectorizeView::saveClientMaterialTextureData(const OdGiMaterialTextureData *pTextureData, OdGsFiler *pFiler)
{
  bool bPresentInManager = device()->materialTextureManager()->isAvailable(pTextureData);
  pFiler->wrBool(bPresentInManager);
  if (bPresentInManager)
    pFiler->wrPtr(pTextureData);
  else
    return static_cast<OdGsOpenGLStreamVectorizeDevice *>(device())->saveClientMaterialTextureData(pTextureData, pFiler);
  return true;
}

OdGiMaterialTextureDataPtr OdGsOpenGLStreamVectorizeView::loadClientMaterialTextureData(OdGsFiler *pFiler)
{
  bool bPresentInManager = pFiler->rdBool();
  OdGiMaterialTextureDataPtr pTextureData;
  if (bPresentInManager)
  {
    OdIntPtr pPtr = pFiler->rdIntPtr();
    if (pPtr) pFiler->subst()->requestSubstitution(&pTextureData, &pPtr, false);
  }
  else
    pTextureData = static_cast<OdGsOpenGLStreamVectorizeDevice *>(device())->loadClientMaterialTextureData(pFiler);
  return pTextureData;
}

class OdGiOpenGLMaterialTextureDataFiler :
#ifdef OD_OGL_ENABLETEXBINDING
  public OdGiOpenGLBindableMaterialTextureData
#else
  public OdGiOpenGLMaterialTextureData
#endif
{
  public:
    OdGiOpenGLMaterialTextureDataFiler() { }

#ifndef OD_OGL_ENABLETEXBINDING
    void updateOnCreate(OdGiMaterialTextureData::DevDataVariant /*pDeviceInfo*/) { }
#endif

    static bool save(const OdGiMaterialTextureData *pTextureData, OdGsFiler *pFiler)
    {
      const OdGiOpenGLMaterialTextureDataFiler *pData = static_cast<const OdGiOpenGLMaterialTextureDataFiler*>(pTextureData);
      OdGsFilerPtr_wrArrayRaw(pFiler, pData->m_pxArray, sizeof(OdGiPixelBGRA32));
      pFiler->wrUInt32(pData->m_uWidth); pFiler->wrUInt32(pData->m_uHeight);
      pFiler->wrBool(pData->m_bInBGRA);
      pFiler->wrInt((int)pData->m_nBorder);
      OdGsFilerPtr_wrArrayRaw(pFiler, pData->m_pxOpacity, sizeof(OdUInt8));
      return true;
    }
    static bool load(OdGiMaterialTextureData *pTextureData, OdGsFiler *pFiler, OdGsOpenGLStreamVectorizeDevice *pDevice)
    {
      OdGiOpenGLMaterialTextureDataFiler *pData = static_cast<OdGiOpenGLMaterialTextureDataFiler*>(pTextureData);
      OdGsFilerPtr_rdArrayRaw(pFiler, pData->m_pxArray, sizeof(OdGiPixelBGRA32));
      pData->m_uWidth = pFiler->rdUInt32(); pData->m_uHeight = pFiler->rdUInt32();
      pData->m_bInBGRA = pFiler->rdBool();
      pData->m_nBorder = (GLint)pFiler->rdInt();
      OdGsFilerPtr_rdArrayRaw(pFiler, pData->m_pxOpacity, sizeof(OdUInt8));
      pData->updateOnCreate(OdGiMaterialTextureData::DevDataVariant((void*)pDevice));
      return true;
    }
};

bool OdGsOpenGLStreamVectorizeDevice::saveClientMaterialTextureData(const OdGiMaterialTextureData *pTextureData, OdGsFiler *pFiler)
{
  pFiler->wrPtr(pTextureData);
  return OdGiOpenGLMaterialTextureDataFiler::save(pTextureData, pFiler);
}

OdGiMaterialTextureDataPtr OdGsOpenGLStreamVectorizeDevice::loadClientMaterialTextureData(OdGsFiler *pFiler)
{
#ifdef OD_OGL_ENABLETEXBINDING
  OdGiMaterialTextureDataPtr pTextureData = OdRxObjectImpl<OdGiOpenGLBindableMaterialTextureData, OdGiMaterialTextureData>::createObject();
#else
  OdGiMaterialTextureDataPtr pTextureData = OdRxObjectImpl<OdGiOpenGLMaterialTextureData, OdGiMaterialTextureData>::createObject();
#endif
  OdIntPtr pPtr = pFiler->rdIntPtr();
  if (OdGiOpenGLMaterialTextureDataFiler::load(pTextureData, pFiler, this))
  {
    pFiler->subst()->registerSubstitution(&pPtr, &pTextureData);
    return pTextureData;
  }
  return OdGiMaterialTextureDataPtr();
}

bool OdGsOpenGLStreamVectorizeDevice::isSupportDeviceStateSaving() const
{
  return true;
}

bool OdGsOpenGLStreamVectorizeDevice::saveClientDeviceState(OdGsFiler *pFiler) const
{
  OdGsOpenGLStreamVectorizeDevice *pDevice = const_cast<OdGsOpenGLStreamVectorizeDevice*>(this);
  // Save raster image cache
  const OdUInt32 nRasters = m_rasterImageCache.size();
  pFiler->wrUInt32(nRasters);
  if (nRasters)
  {
    RasterImageCacheEntry * const *pRasters = m_rasterImageCache.getPtr();
    for (OdUInt32 nRaster = 0; nRaster < nRasters; nRaster++)
    {
      RasterImageInfo *pRI = pRasters[nRaster];
      pFiler->wrPtr(pRasters[nRaster]);
      pFiler->wrRawData(pRI, sizeof(RasterImageInfo));
#ifdef OD_OGL_TEXTURESHARING
      OdOpenGLArrayWrapper *pTexData = pRasters[nRaster]->m_texData.get();
      OdOpenGLArrayWrapper *pPalData = pRasters[nRaster]->m_palData.get();
#else
#ifndef OD_OGL_ENABLETEXBINDING
      OdOpenGLArrayWrapper *pTexData = &pRasters[nRaster]->m_texData;
      OdOpenGLArrayWrapper *pPalData = &pRasters[nRaster]->m_palData;
#else
#pragma MARKMESSAGE(@@@TODO: restoring from GLContext);
      return false;
#endif
#endif
      if (pTexData && pTexData->m_uSize)
      {
        pFiler->wrUInt32(pTexData->m_uSize);
        pFiler->wrRawData(pTexData->m_pData, pTexData->m_uSize);
      }
      else
        pFiler->wrUInt32(0);
      if (pPalData && pPalData->m_uSize)
      {
        pFiler->wrUInt32(pPalData->m_uSize);
        pFiler->wrRawData(pPalData->m_pData, pPalData->m_uSize);
      }
      else
        pFiler->wrUInt32(0);
      pFiler->wrUInt32(pRasters[nRaster]->m_uLutSize);
    }
  }
  // Save material textures manager
  if (!OdGsBaseMaterialVectorizer::saveMaterialTextureManager(pDevice->materialTextureManager(), pFiler, pDevice))
    return false;
  if (!m_ttfFontsCache->saveFontCache(pFiler))
    return false;
  return true;
}

bool OdGsOpenGLStreamVectorizeDevice::loadClientDeviceState(OdGsFiler *pFiler)
{
  // Load raster images cache
  const OdUInt32 nRasters = pFiler->rdUInt32();
  if (nRasters)
  {
    for (OdUInt32 nRaster = 0; nRaster < nRasters; nRaster++)
    {
      OdIntPtr pPtr = pFiler->rdIntPtr();
      RasterImageInfo ri;
      OdUInt8Array pTex, pPal;
      pFiler->rdRawData(&ri, sizeof(RasterImageInfo));
      pTex.resize(pFiler->rdUInt32());
      if (!pTex.isEmpty())
        pFiler->rdRawData(pTex.asArrayPtr(), pTex.size());
      pPal.resize(pFiler->rdUInt32());
      if (!pPal.isEmpty())
        pFiler->rdRawData(pPal.asArrayPtr(), pPal.size());
      OdUInt32 nLutSize = pFiler->rdUInt32();
      RasterImageCacheEntry *pEntryPtr = addRasterImage(ri, pTex.getPtr(), pTex.size(), 0xFFFFFFFF, pPal.getPtr(), nLutSize, 0xFFFFFFFF);
      if (pPtr) pFiler->subst()->registerSubstitution(&pPtr, &pEntryPtr);
    }
  }
  // Load material textures amanager
  if (!OdGsBaseMaterialVectorizer::loadMaterialTextureManager(materialTextureManager(), pFiler, this))
    return false;
  if (!m_ttfFontsCache->loadFontCache(pFiler, userGiContext()->database()))
    return false;
  return true;
}

bool OdGsOpenGLStreamVectorizeView::saveClientViewState(OdGsFiler *pFiler) const
{
  // Save FPPrecisionFix
  pFiler->wrUInt32((OdUInt32)m_coordType);
  pFiler->wrMatrix3d(m_outputMtx);
  pFiler->wrMatrix3d(m_blocksScale);
  return true;
}

bool OdGsOpenGLStreamVectorizeView::loadClientViewState(OdGsFiler *pFiler)
{
  // Load FPPrecisionFix
  m_coordType = (MetaCoordType)pFiler->rdUInt32();
  pFiler->rdMatrix3d(m_outputMtx);
  pFiler->rdMatrix3d(m_blocksScale);
  return true;
}

//
