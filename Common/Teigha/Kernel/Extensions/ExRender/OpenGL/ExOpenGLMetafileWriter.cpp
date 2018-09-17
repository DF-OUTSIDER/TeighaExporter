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
#include "ExOpenGLMetafileWriter.h"

#define OD_OGL_PACKRESERVE_MINARRAYSIZE m_nPackReserve // Minimum count of elements in array
// elem.setPhysicalLength(0); // Use this if don't want store unused memory 
#define OD_OGL_PACKRESERVE(elem) \
  { \
    if (elem.capacity() > OD_OGL_PACKRESERVE_MINARRAYSIZE) \
      elem.setPhysicalLength(OD_OGL_PACKRESERVE_MINARRAYSIZE); \
    elem.setLogicalLength(0); \
  }

#define OD_OGL_STREAM_NORMALIZESTRICTALIGN(nAlign, nPre) \
  if (m_bStrictAlignment) \
  { \
    OdUInt32 nRem = (containerWriter()->position() + nPre) % nAlign; \
    if (nRem) \
    { \
      nRem = nAlign - nRem; \
      do \
      { \
        writeChunk(OdOpenGLMetaRecType_Empty); \
      } \
      while (--nRem); \
    } \
  }

#ifdef _DEBUG
#define STL_USING_LIMITS
#include "OdaSTL.h"
inline OdGLfloat dbl2float(double value)
{
  // Test if value denormalized
  if (value < double(std::numeric_limits<float>::min()) && value > -double(std::numeric_limits<float>::min()))
    return 0.0f;
  // Test if value to big
  if (value > double(std::numeric_limits<float>::max()))
    return std::numeric_limits<float>::max();
  // Test if value to small
  if (value < double(-std::numeric_limits<float>::max()))
    return -std::numeric_limits<float>::max();
  return (OdGLfloat)value;
}
#else
#ifndef dbl2float
#define dbl2float(x) (OdGLfloat)(x)
#endif
#endif

// OdOpenGLMetafileWriter::TraitsOptions

void OdOpenGLMetafileWriter::TraitsOptions::setDefaults()
{
  ::memset(this, 0xFF, sizeof(TraitsOptions));
}

// OdOpenGLMetafileWriter::PrevState

void OdOpenGLMetafileWriter::PrevState::setDefaults()
{
  // Init to the state, which is incrorrect always
  TraitsOptions::setDefaults();
  bOptFlagsState.regs.attributes = (bOptFlagsTouch.regs.attributes ^= (1 << OdOpenGLAttribute_CullFace) |
                                                                      (1 << OdOpenGLAttribute_Blend)); // #9643 : enable from initialized state, to prevent recording
  bOptFlagsState.regs.shading = (bOptFlagsTouch.regs.shading ^= (1 << OdOpenGLShading_NoColorOverride));
  // Initialize all members for PrevState
  // Set colors to complete zero (so alpha disabled)
  crColor = 0;
  gsMarker = kNullSubentIndex;
  lwd.m_type = OdOpenGLLwdSetting::kDatabase;
  lwd.m_val.m_lwLineWeight = OdDb::kLnWt000;
  lnPatt.data.summ = 0; lnPatt.data.named.reg_linetype = 0xFF;
  fillPatt.data.summ = 0; fillPatt.data.named.reg_fillstyle = 0xFF;
  lwdStyle.m_bCurStyleDef = lwdStyle.m_bCurUsesDef = true;
  //
  nSelectionFlags = 0;
}

// OdOpenGLMetafileWriter inlines

namespace Classify
{
  inline bool isPts(OdOpenGLMetafileWriter::PackageType packType) { return (packType & OdOpenGLMetafileWriter::PackageType_IsTypeMask) == OdOpenGLMetafileWriter::PackageType_IsPoints; }
  inline bool isLin(OdOpenGLMetafileWriter::PackageType packType) { return (packType & OdOpenGLMetafileWriter::PackageType_IsTypeMask) == OdOpenGLMetafileWriter::PackageType_IsLines; }
  inline bool isTri(OdOpenGLMetafileWriter::PackageType packType) { return (packType & OdOpenGLMetafileWriter::PackageType_IsTypeMask) == OdOpenGLMetafileWriter::PackageType_IsTriangles; }
  inline bool isIndexed(OdOpenGLMetafileWriter::PackageType packType) { return GETBIT(packType, OdOpenGLMetafileWriter::PackageType_IsIndexed); }
  inline bool isColors(OdOpenGLMetafileWriter::PackageType packType) { return GETBIT(packType, OdOpenGLMetafileWriter::PackageType_IsColors); }
  inline bool isNormals(OdOpenGLMetafileWriter::PackageType packType) { return GETBIT(packType, OdOpenGLMetafileWriter::PackageType_IsNormals); }
  inline bool isTextured(OdOpenGLMetafileWriter::PackageType packType) { return GETBIT(packType, OdOpenGLMetafileWriter::PackageType_IsTextured); }

  inline OdOpenGLMetafileWriter::PackageType type(OdOpenGLMetafileWriter::PackageType packType)
  { return (OdOpenGLMetafileWriter::PackageType)(packType & OdOpenGLMetafileWriter::PackageType_IsTypeMask); }
  inline OdUInt32 glType(OdOpenGLMetafileWriter::PackageType packType)
  {
    switch (packType & OdOpenGLMetafileWriter::PackageType_IsTypeMask)
    {
      case OdOpenGLMetafileWriter::PackageType_IsLines:     return GL_LINES;
      case OdOpenGLMetafileWriter::PackageType_IsTriangles: return GL_TRIANGLES;
      case OdOpenGLMetafileWriter::PackageType_IsPoints:    return GL_POINTS;
    }
    return 0;
  }

  // Check that line type compatible with triangles type
  inline bool isLineTypeCompat(OdOpenGLMetafileWriter::PackageType packTypeTrg, OdOpenGLMetafileWriter::PackageType packTypeLin)
  {
    if (!GETBIT(packTypeTrg, OdOpenGLMetafileWriter::PackageType_IsColors)   && GETBIT(packTypeLin, OdOpenGLMetafileWriter::PackageType_IsColors))   return false;
    if (!GETBIT(packTypeTrg, OdOpenGLMetafileWriter::PackageType_IsNormals)  && GETBIT(packTypeLin, OdOpenGLMetafileWriter::PackageType_IsNormals))  return false;
    if (!GETBIT(packTypeTrg, OdOpenGLMetafileWriter::PackageType_IsTextured) && GETBIT(packTypeLin, OdOpenGLMetafileWriter::PackageType_IsTextured)) return false;
    return true;
  }
}

// OdOpenGLMetafileWriter

void OdOpenGLMetafileWriter::beginWriting(OdBaseMetafileContainer *pContainer)
{
  containerIO()->setContainer(pContainer);
  prevState.setDefaults();
  m_uAffects = 0;
  m_settings.resetFlags();
  m_uIndexIncr = m_uMaxId = 0;
  m_uReserve[0] = m_uReserve[1] = 0;
  m_prevChunk = OdOpenGLMetaRecType_Empty;
  m_pointMode = PointMode_Unopt;
  m_bPrevPoint = 0;
}

// The parts of optimization processing:
// completeWriting() - postprocess data structure.
// flushTraitsOptions() - sets painting flags only when they really changed.
// resolveTraitsConflicts() - can be used to optimize attributes output, if one attribute sections changed two times.
// flushPackager() - outputs collected data.
// flushOnIndexedMode() - enables indexed data output.
// For further rendering optimizations in future can be used following techniques:
// - if primitive collection type not changed after color or attributes changing, previous arrays can be used instead of
//   new arrays creation (can't be used in 2DOptimized mode).
// - in FlatShaded and GouraudShaded modes (without edges) native OpenGL call lists is more fast.
//   Can be used mixed output in this case or... Attributes can be skipped in some cases.
//   Colors can be packed in separate array. And so on...
// - For modes instead 2DOptimized can be used following technique: Pack lines and triangles in
//   separate arrays. Connect arrays in playMetafile(). Flush all data as two arrays to GPU on
//   endVectorization().

void OdOpenGLMetafileWriter::completeWriting()
{
  flushPackager(PackageType_Null);
  // Simple options check to reduce calls
  if (prevState.gsMarker != kNullSubentIndex)
    glSelectionMarker(kNullSubentIndex); // Make unselection if markers available
  // Hard options check
  if (prevState.lwd.isForcedLineweight())
    glLineweight(OdInt32(0)); // Reset lineweight if it is forced
  glLineweight(OdDb::kLnWt000); // Disable lineweights if enabled (prevent lineweight usage on next metafile)
  glLineStyle(true, OdPs::kLesRound, OdPs::kLjsRound); // Set to default
  glDisableOpt(OdOpenGLAttribute_Blend); // #9643
  glDisableShadingOpt(OdOpenGLShading_NoColorOverride);
  OdBaseMetafileOutputStream::completeWriting();
  if (m_uAffects != 0 && (m_uAffects & OdOpenGLAffection_Geometry) == 0)
  {
    OdOpenGLFlatMetafileContainer *pCont = glContainer();
    if ((m_uAffects & OdOpenGLAffection_Arrays) != 0)
      pCont->clearOpenGLFlatContainer();
    pCont->clearFlatContainer();
  }
  containerIO()->setContainer(NULL);
  m_delEntry.setPhysicalLength(0);
}

void OdOpenGLMetafileWriter::clearArrays()
{
  for (OdUInt32 nPkg = 0; nPkg < 2; nPkg++)
  {
    m_packEntry[nPkg].packType = PackageType_Null;
    m_packEntry[nPkg].m_vertArray.setPhysicalLength(0);
    m_packEntry[nPkg].m_clrArray.setPhysicalLength(0);
    m_packEntry[nPkg].m_normArray.setPhysicalLength(0);
    m_packEntry[nPkg].m_texArray.setPhysicalLength(0);
    m_packEntry[nPkg].m_selMarkArray.setPhysicalLength(0);
    m_packEntry[nPkg].m_indices.setPhysicalLength(0);
    m_packEntry[nPkg].m_inputTraits.setDefaults();
    m_packEntry[nPkg].m_outputTraits.setDefaults();
  }
  m_settings.resetFlushing();
  m_pointEntry.m_vertArray.setPhysicalLength(0);
  m_pointEntry.m_clrArray.setPhysicalLength(0);
  m_pointEntry.m_texArray.setPhysicalLength(0);
  m_pointEntry.m_selMarkArray.setPhysicalLength(0);
  m_delEntry.setPhysicalLength(0);
}

void OdOpenGLMetafileWriter::flushTraitsOptions(TraitsOptions &options)
{
  if (!options.bOptFlagsTouch.getAttributeMaskAll())
  { for (long i = 0; i < OdOpenGLAttribute_NTypes; i++)
    { const OdOpenGLAttribute attrib = (OdOpenGLAttribute)i;
      if (!options.bOptFlagsTouch.getAttribute(attrib))
      { if (options.bOptFlagsState.getAttribute(attrib)) this->glEnableOpt(attrib);
        else                                             this->glDisableOpt(attrib);
      }
    }
  }
  if (!options.bOptFlagsTouch.getShadingMaskAll())
  { for (long i = 0; i < OdOpenGLShading_NTypes; i++)
    { const OdOpenGLShading shad = (OdOpenGLShading)i;
      if (!options.bOptFlagsTouch.getShading(shad))
      { if (options.bOptFlagsState.getShading(shad)) this->glEnableShadingOpt(shad);
        else                                         this->glDisableShadingOpt(shad);
      }
    }
  }
  if (options.eCullMode != 0xFF)
    this->glCullFace(options.cullMode());
  if (options.bHLR != 0xFF)
  {
    if (options.bHLR == 0)
    {
      this->glHLRStencil(false);
      this->glColor(m_crPrevColor);
    }
    else if (options.bHLR == 1)
    {
      this->glHLRStencil(true);
      m_crPrevColor = prevState.crColor;
      this->glColor(m_crHLR);
    }
  }
}

void OdOpenGLMetafileWriter::resolveTraitsConflicts(TraitsOptions &prev, const TraitsOptions &next)
{
  prev.bOptFlagsTouch.flags |= ~(next.bOptFlagsTouch.flags | prev.bOptFlagsTouch.flags);
  if (next.eCullMode != 0xFF && prev.eCullMode != 0xFF)
    prev.eCullMode = 0xFF;
  if (next.bHLR != 0xFF && prev.bHLR != 0xFF)
    prev.bHLR = 0xFF;
}

bool OdOpenGLMetafileWriter::validateGsMarkers(int nPackEntry)
{ // -1 for points entry, 0-1 for pack entry layers
  OdOpenGLSelMarkArray &selMarks = (nPackEntry >= 0) ? m_packEntry[nPackEntry].m_selMarkArray : m_pointEntry.m_selMarkArray;
  if (nPackEntry >= 0)
  {
    if (m_settings.isIndexedMode())
    {
      if (selMarks.last().uFrom != m_packEntry[nPackEntry].m_indices.size())
        return true;
    }
    else
    {
      if (selMarks.last().uFrom != m_packEntry[nPackEntry].m_vertArray.size())
        return true;
    }
  }
  else
  {
    if (selMarks.last().uFrom != m_pointEntry.m_vertArray.size())
      return true;
  }
  selMarks.removeLast();
  m_settings.setRestoreGsMarkers();
  return !selMarks.empty();
}

void OdOpenGLMetafileWriter::flushPackager(PackageType type, OdUInt32 apprLayer)
{
  if (!m_settings.isFlushing() &&
      ((apprLayer != 0xFFFFFFFF && m_packEntry[apprLayer].packType != type) ||
      (apprLayer == 0xFFFFFFFF && (type != m_packEntry[0].packType || type != m_packEntry[1].packType))))
  {
    if (m_settings.isIndexedMode())
    {
      if (apprLayer == 0xFFFFFFFF || (m_packEntry[apprLayer].packType != type && m_packEntry[apprLayer].packType != PackageType_Null))
      {
        m_settings.setFlushing();
        const bool bLines = !m_packEntry[0].m_indices.empty();
        const bool bTris  = !m_packEntry[1].m_indices.empty();
        if (bTris)
        {
          flushTraitsOptions(m_packEntry[1].m_inputTraits);
          this->glEnableArray(OdOpenGLMetaRecArrayType_Vertex, 0);
          this->glAppendArray_dbl((const double*)m_packEntry[1].m_vertArray.getPtr(), m_packEntry[1].m_vertArray.size() * 3);
          if (Classify::isColors(m_packEntry[1].packType))
          {
            this->glEnableArray(OdOpenGLMetaRecArrayType_Color, 0);
            this->glAppendArray(m_packEntry[1].m_clrArray.getPtr(), m_packEntry[1].m_clrArray.size() * sizeof(ColorTriplet));
          }
          if (Classify::isNormals(m_packEntry[1].packType))
          {
            this->glEnableArray(OdOpenGLMetaRecArrayType_Normal, 0);
            this->glAppendArray_dbl((const double*)m_packEntry[1].m_normArray.getPtr(), m_packEntry[1].m_normArray.size() * 3);
          }
          if (Classify::isTextured(m_packEntry[1].packType))
          {
            this->glEnableArray(OdOpenGLMetaRecArrayType_TexCoord, 0);
            flushTriDelayCache(this->glAppendArray_dbl((const double*)m_packEntry[1].m_texArray.getPtr(), m_packEntry[1].m_texArray.size() * 2));
          }
          if (!m_packEntry[1].m_selMarkArray.empty() && validateGsMarkers(1))
          {
            this->glEnableSelMarkArray();
            this->glAppendArray(m_packEntry[1].m_selMarkArray.getPtr(), m_packEntry[1].m_selMarkArray.size() * sizeof(OdOpenGLSelMark));
          }
          this->glDrawElements(GL_TRIANGLES, m_packEntry[1].m_indices.size(), m_packEntry[1].m_indices.getPtr());
          if (!m_packEntry[1].m_selMarkArray.empty())
          {
            this->glDisableSelMarkArray();
            OD_OGL_PACKRESERVE(m_packEntry[1].m_selMarkArray);
          }
          if (Classify::isTextured(m_packEntry[1].packType) && (!bLines || !Classify::isTextured(m_packEntry[0].packType)))
          {
            this->glDisableArray(OdOpenGLMetaRecArrayType_TexCoord);
            OD_OGL_PACKRESERVE(m_packEntry[1].m_texArray);
          }
          if (Classify::isNormals(m_packEntry[1].packType) && (!bLines || !Classify::isNormals(m_packEntry[0].packType)))
          {
            this->glDisableArray(OdOpenGLMetaRecArrayType_Normal);
            OD_OGL_PACKRESERVE(m_packEntry[1].m_normArray);
          }
          if (Classify::isColors(m_packEntry[1].packType) && (!bLines || !Classify::isColors(m_packEntry[0].packType)))
          {
            this->glDisableArray(OdOpenGLMetaRecArrayType_Color);
            OD_OGL_PACKRESERVE(m_packEntry[1].m_clrArray);
            // Restore previous color for next calls
            if (bLines || type != PackageType_Null)
            {
              ODCOLORREF cRest = prevState.crColor;
              prevState.crColor = 0;
              this->glColor(cRest);
            }
            else
            {
              prevState.crColor = 0;
            }
          }
          if (bLines)
            resolveTraitsConflicts(m_packEntry[1].m_outputTraits, m_packEntry[0].m_inputTraits);
          flushTraitsOptions(m_packEntry[1].m_outputTraits);
          OD_OGL_PACKRESERVE(m_packEntry[1].m_indices);
          m_packEntry[1].m_inputTraits.setDefaults();
          m_packEntry[1].m_outputTraits.setDefaults();
        }
        else if (bLines)
        {
          this->glEnableArray(OdOpenGLMetaRecArrayType_Vertex, 0);
          this->glAppendArray_dbl((const double*)m_packEntry[1].m_vertArray.getPtr(), m_packEntry[1].m_vertArray.size() * 3);
          if (Classify::isColors(m_packEntry[0].packType))
          {
            this->glEnableArray(OdOpenGLMetaRecArrayType_Color, 0);
            this->glAppendArray(m_packEntry[1].m_clrArray.getPtr(), m_packEntry[1].m_clrArray.size() * sizeof(ColorTriplet));
          }
          if (Classify::isNormals(m_packEntry[0].packType))
          {
            this->glEnableArray(OdOpenGLMetaRecArrayType_Normal, 0);
            this->glAppendArray_dbl((const double*)m_packEntry[1].m_normArray.getPtr(), m_packEntry[1].m_normArray.size() * 3);
          }
          if (Classify::isTextured(m_packEntry[0].packType))
          {
            this->glEnableArray(OdOpenGLMetaRecArrayType_TexCoord, 0);
            flushTriDelayCache(this->glAppendArray_dbl((const double*)m_packEntry[1].m_texArray.getPtr(), m_packEntry[1].m_texArray.size() * 2));
          }
        }
        if (bLines)
        {
          flushTraitsOptions(m_packEntry[0].m_inputTraits);
          if (!m_packEntry[0].m_selMarkArray.empty() && validateGsMarkers(0))
          {
            this->glEnableSelMarkArray();
            this->glAppendArray(m_packEntry[0].m_selMarkArray.getPtr(), m_packEntry[0].m_selMarkArray.size() * sizeof(OdOpenGLSelMark));
          }
          this->glDrawElements(GL_LINES, m_packEntry[0].m_indices.size(), m_packEntry[0].m_indices.getPtr());
          if (!m_packEntry[0].m_selMarkArray.empty())
          {
            this->glDisableSelMarkArray();
            OD_OGL_PACKRESERVE(m_packEntry[0].m_selMarkArray);
          }
          if (Classify::isTextured(m_packEntry[0].packType))
          {
            this->glDisableArray(OdOpenGLMetaRecArrayType_TexCoord);
            OD_OGL_PACKRESERVE(m_packEntry[1].m_texArray);
          }
          if (Classify::isNormals(m_packEntry[0].packType))
          {
            this->glDisableArray(OdOpenGLMetaRecArrayType_Normal);
            OD_OGL_PACKRESERVE(m_packEntry[1].m_normArray);
          }
          if (Classify::isColors(m_packEntry[0].packType))
          {
            this->glDisableArray(OdOpenGLMetaRecArrayType_Color);
            OD_OGL_PACKRESERVE(m_packEntry[1].m_clrArray);
          }
          OD_OGL_PACKRESERVE(m_packEntry[0].m_indices);
          flushTraitsOptions(m_packEntry[0].m_outputTraits);
          m_packEntry[0].m_inputTraits.setDefaults();
          m_packEntry[0].m_outputTraits.setDefaults();
        }
        if (bLines || bTris)
        {
          this->glDisableArray(OdOpenGLMetaRecArrayType_Vertex);
          if( bTris )
          {
            OD_OGL_PACKRESERVE(m_packEntry[1].m_vertArray);
          }
        }
        if (!bTris && !m_packEntry[1].m_vertArray.empty())
        {
          // This situation maybe if shell drawing started, but all faces is invisible
          OD_OGL_PACKRESERVE(m_packEntry[1].m_vertArray);
          if (Classify::isColors(m_packEntry[1].packType))
          {
            OD_OGL_PACKRESERVE(m_packEntry[1].m_clrArray);
          }
          if (Classify::isNormals(m_packEntry[1].packType))
          {
            OD_OGL_PACKRESERVE(m_packEntry[1].m_normArray);
          }
          if (Classify::isTextured(m_packEntry[1].packType))
          {
            OD_OGL_PACKRESERVE(m_packEntry[1].m_texArray);
          }
          if (!m_packEntry[0].m_selMarkArray.empty())
          {
            OD_OGL_PACKRESERVE(m_packEntry[0].m_selMarkArray);
          }
          if (!m_packEntry[1].m_selMarkArray.empty())
          {
            OD_OGL_PACKRESERVE(m_packEntry[1].m_selMarkArray);
          }
        }
        m_settings.resetFlushing();
        if (apprLayer != 0xFFFFFFFF)
        {
          m_packEntry[0].packType = PackageType_Null;
          m_settings.resetIndexedMode();
        }
      }
    }
    else
    {
      for (int iLayer = 1; iLayer >= 0; iLayer--) // Triangles must be rendered before lines.
      {
        if (m_packEntry[iLayer].packType != PackageType_Null &&
            (apprLayer == OdUInt32(iLayer) || apprLayer == 0xFFFFFFFF))
        {
          m_settings.setFlushing();
          switch (m_packEntry[iLayer].packType)
          {
            case PackageType_VPoints:
              if (m_packEntry[iLayer].m_vertArray.size() == 1 && m_packEntry[iLayer].m_selMarkArray.empty())
              {
                flushTraitsOptions(m_packEntry[iLayer].m_inputTraits);
                glPoint(*(m_packEntry[iLayer].m_vertArray.getPtr()));
                OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_vertArray);
                flushTraitsOptions(m_packEntry[iLayer].m_outputTraits);
                m_packEntry[iLayer].m_inputTraits.setDefaults();
                m_packEntry[iLayer].m_outputTraits.setDefaults();
                break;
              }
            case PackageType_VLines:
              if (m_packEntry[iLayer].m_vertArray.size() == 2 &&
                  m_packEntry[iLayer].packType == PackageType_VLines &&
                  m_packEntry[iLayer].m_selMarkArray.empty() &&
                  m_pointEntry.m_vertArray.size() <= 1)
              {
                // Single line can be flushed without vertex array. This is more optimal I think.
                flushTraitsOptions(m_packEntry[iLayer].m_inputTraits);
                glLine((m_packEntry[iLayer].m_vertArray.getPtr())[0], (m_packEntry[iLayer].m_vertArray.getPtr())[1]);
                OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_vertArray);
                if (m_pointEntry.hasData())
                {
                  glPoint(*(m_pointEntry.m_vertArray.getPtr()));
                  OD_OGL_PACKRESERVE(m_pointEntry.m_vertArray);
                }
                flushTraitsOptions(m_packEntry[iLayer].m_outputTraits);
                m_packEntry[iLayer].m_inputTraits.setDefaults();
                m_packEntry[iLayer].m_outputTraits.setDefaults();
                break;
              }
            default:
              if (iLayer == 1 && m_packEntry[0].packType != PackageType_Null &&
                  apprLayer == 0xFFFFFFFF && Classify::isLineTypeCompat(m_packEntry[iLayer].packType, m_packEntry[0].packType))
              {
                // Optimization for all modes. Pack triangles and polylines into one array.
                flushTraitsOptions(m_packEntry[iLayer].m_inputTraits);
                this->glEnableArray(OdOpenGLMetaRecArrayType_Vertex, 0);
                OdUInt32 id = this->glAppendArray_dbl((const double*)m_packEntry[iLayer].m_vertArray.getPtr(), m_packEntry[iLayer].m_vertArray.size() * 3);
                this->glConcatArray_dbl(id, (const double*)m_packEntry[0].m_vertArray.getPtr(), m_packEntry[0].m_vertArray.size() * 3);
                if (m_pointEntry.hasData())
                  this->glConcatArray_dbl(id, (const double*)m_pointEntry.m_vertArray.getPtr(), m_pointEntry.m_vertArray.size() * 3);
                if (Classify::isColors(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_Color, 0);
                  id = this->glAppendArray(m_packEntry[iLayer].m_clrArray.getPtr(), m_packEntry[iLayer].m_vertArray.size() * sizeof(ColorTriplet));
                  if (Classify::isColors(m_packEntry[0].packType))
                  {
                    this->glConcatArray(id, m_packEntry[0].m_clrArray.getPtr(), m_packEntry[0].m_clrArray.size() * sizeof(ColorTriplet));
                    if (m_pointEntry.hasData())
                      this->glConcatArray(id, m_pointEntry.m_clrArray.getPtr(), m_pointEntry.m_clrArray.size() * sizeof(ColorTriplet));
                  }
                }
                if (Classify::isNormals(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_Normal, 0);
                  id = this->glAppendArray_dbl((const double*)m_packEntry[iLayer].m_normArray.getPtr(), m_packEntry[iLayer].m_normArray.size() * 3);
                  if (Classify::isNormals(m_packEntry[0].packType))
                    this->glConcatArray_dbl(id, (const double*)m_packEntry[0].m_normArray.getPtr(), m_packEntry[0].m_normArray.size() * 3);
                }
                if (Classify::isTextured(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_TexCoord, 0);
                  id = this->glAppendArray_dbl((const double*)m_packEntry[iLayer].m_texArray.getPtr(), m_packEntry[iLayer].m_texArray.size() * 2);
                  if (Classify::isTextured(m_packEntry[0].packType))
                  {
                    this->glConcatArray_dbl(id, (const double*)m_packEntry[0].m_texArray.getPtr(), m_packEntry[0].m_texArray.size() * 2);
                    if (m_pointEntry.hasData())
                      this->glConcatArray_dbl(id, (const double*)m_pointEntry.m_texArray.getPtr(), m_pointEntry.m_texArray.size() * 2);
                  }
                  flushTriDelayCache(id);
                }
                if (!m_packEntry[iLayer].m_selMarkArray.empty() && validateGsMarkers(iLayer))
                {
                  this->glEnableSelMarkArray();
                  this->glAppendArray(m_packEntry[iLayer].m_selMarkArray.getPtr(), m_packEntry[iLayer].m_selMarkArray.size() * sizeof(OdOpenGLSelMark));
                }
                this->glDrawArrays(GL_TRIANGLES, 0, m_packEntry[iLayer].m_vertArray.size());
                if (!m_packEntry[iLayer].m_selMarkArray.empty())
                {
                  this->glDisableSelMarkArray();
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_selMarkArray);
                }
                if (Classify::isTextured(m_packEntry[iLayer].packType))
                {
                  if (!Classify::isTextured(m_packEntry[0].packType))
                    this->glDisableArray(OdOpenGLMetaRecArrayType_TexCoord);
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_texArray);
                }
                if (Classify::isNormals(m_packEntry[iLayer].packType))
                {
                  if (!Classify::isNormals(m_packEntry[0].packType))
                    this->glDisableArray(OdOpenGLMetaRecArrayType_Normal);
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_normArray);
                }
                if (Classify::isColors(m_packEntry[iLayer].packType))
                {
                  if (!Classify::isColors(m_packEntry[0].packType))
                    this->glDisableArray(OdOpenGLMetaRecArrayType_Color);
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_clrArray);
                  if (!Classify::isColors(m_packEntry[0].packType))
                  {
                    // Restore previous color for next calls
                    ODCOLORREF cRest = prevState.crColor;
                    prevState.crColor = 0;
                    this->glColor(cRest);
                  }
                }
                resolveTraitsConflicts(m_packEntry[iLayer].m_outputTraits, m_packEntry[0].m_inputTraits);
                flushTraitsOptions(m_packEntry[iLayer].m_outputTraits);
                flushTraitsOptions(m_packEntry[0].m_inputTraits);
                if (Classify::isColors(m_packEntry[0].packType) && !Classify::isColors(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_Color, 0);
                  id = this->glAppendArray(m_packEntry[0].m_clrArray.getPtr(), m_packEntry[0].m_vertArray.size() * sizeof(ColorTriplet));
                  if (m_pointEntry.hasData())
                    this->glConcatArray(id, m_pointEntry.m_clrArray.getPtr(), m_pointEntry.m_clrArray.size() * sizeof(ColorTriplet));
                }
                if (Classify::isNormals(m_packEntry[0].packType) && !Classify::isNormals(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_Normal, 0);
                  this->glAppendArray_dbl((const double*)m_packEntry[0].m_normArray.getPtr(), m_packEntry[0].m_normArray.size() * 3);
                }
                if (Classify::isTextured(m_packEntry[0].packType) && !Classify::isTextured(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_TexCoord, 0);
                  id = this->glAppendArray_dbl((const double*)m_packEntry[0].m_texArray.getPtr(), m_packEntry[0].m_texArray.size() * 2);
                  if (m_pointEntry.hasData())
                    this->glConcatArray_dbl(id, (const double*)m_pointEntry.m_texArray.getPtr(), m_pointEntry.m_texArray.size() * 2);
                  flushTriDelayCache(id);
                }
                if (!m_packEntry[0].m_selMarkArray.empty() && validateGsMarkers(0))
                {
                  this->glEnableSelMarkArray();
                  OdUInt32 nMarks = m_packEntry[0].m_selMarkArray.size();
                  OdOpenGLSelMark *pMarks = m_packEntry[0].m_selMarkArray.asArrayPtr();
                  OdUInt32 icSize = m_packEntry[iLayer].m_vertArray.size();
                  for (OdUInt32 nMark = 0; nMark < nMarks; nMark++)
                    pMarks[nMark].uFrom += icSize;
                  this->glAppendArray(pMarks, nMarks * sizeof(OdOpenGLSelMark));
                }
                this->glDrawArrays(Classify::isLin(m_packEntry[0].packType) ? GL_LINES : GL_POINTS,
                                   m_packEntry[iLayer].m_vertArray.size(), m_packEntry[0].m_vertArray.size());
                if (!m_packEntry[0].m_selMarkArray.empty())
                {
                  this->glDisableSelMarkArray();
                  OD_OGL_PACKRESERVE(m_packEntry[0].m_selMarkArray);
                }
                if (Classify::isNormals(m_packEntry[0].packType))
                {
                  this->glDisableArray(OdOpenGLMetaRecArrayType_Normal);
                  OD_OGL_PACKRESERVE(m_packEntry[0].m_normArray);
                }
                if (m_pointEntry.hasData())
                {
                  if (!m_pointEntry.m_selMarkArray.empty() && validateGsMarkers(-1))
                  {
                    this->glEnableSelMarkArray();
                    OdUInt32 nMarks = m_pointEntry.m_selMarkArray.size();
                    OdOpenGLSelMark *pMarks = m_pointEntry.m_selMarkArray.asArrayPtr();
                    OdUInt32 icSize = m_packEntry[iLayer].m_vertArray.size() + m_packEntry[0].m_vertArray.size();
                    for (OdUInt32 nMark = 0; nMark < nMarks; nMark++)
                      pMarks[nMark].uFrom += icSize;
                    this->glAppendArray(pMarks, nMarks * sizeof(OdOpenGLSelMark));
                  }
                  this->glDrawArrays(GL_POINTS,
                                     m_packEntry[iLayer].m_vertArray.size() + m_packEntry[0].m_vertArray.size(),
                                     m_pointEntry.m_vertArray.size());
                  if (!m_pointEntry.m_selMarkArray.empty())
                  {
                    this->glDisableSelMarkArray();
                    OD_OGL_PACKRESERVE(m_pointEntry.m_selMarkArray);
                  }
                }
                if (Classify::isTextured(m_packEntry[0].packType))
                {
                  this->glDisableArray(OdOpenGLMetaRecArrayType_TexCoord);
                  OD_OGL_PACKRESERVE(m_packEntry[0].m_texArray);
                  if (m_pointEntry.hasData())
                  {
                    OD_OGL_PACKRESERVE(m_pointEntry.m_texArray);
                  }
                }
                if (Classify::isColors(m_packEntry[0].packType))
                {
                  this->glDisableArray(OdOpenGLMetaRecArrayType_Color);
                  OD_OGL_PACKRESERVE(m_packEntry[0].m_clrArray);
                  if (m_pointEntry.hasData())
                  {
                    OD_OGL_PACKRESERVE(m_pointEntry.m_clrArray);
                  }
                }
                this->glDisableArray(OdOpenGLMetaRecArrayType_Vertex);
                OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_vertArray);
                OD_OGL_PACKRESERVE(m_packEntry[0].m_vertArray);
                if (m_pointEntry.hasData())
                {
                  OD_OGL_PACKRESERVE(m_pointEntry.m_vertArray);
                }
                if (m_settings.isInsertMode())
                {
                  // Solves following situation: we start rendering of new faces and set some options. start
                  // new face and reset something which causes flush. Now traits will be setupped for lines
                  // rendering and previously set params will be lost. So we need restore them here for next
                  // face processing. For example Lighting attribute could be lost here.
                  resolveTraitsConflicts(m_packEntry[0].m_outputTraits, m_packEntry[iLayer].m_inputTraits);
                  flushTraitsOptions(m_packEntry[0].m_outputTraits);
                  flushTraitsOptions(m_packEntry[iLayer].m_inputTraits);
                }
                else
                  flushTraitsOptions(m_packEntry[0].m_outputTraits);
                m_packEntry[0].m_inputTraits.setDefaults();
                m_packEntry[0].m_outputTraits.setDefaults();
                m_packEntry[iLayer].m_inputTraits.setDefaults();
                m_packEntry[iLayer].m_outputTraits.setDefaults();
                iLayer = 0;
              }
              else
              {
                flushTraitsOptions(m_packEntry[iLayer].m_inputTraits);
                this->glEnableArray(OdOpenGLMetaRecArrayType_Vertex, 0);
                OdUInt32 id = this->glAppendArray_dbl((const double*)m_packEntry[iLayer].m_vertArray.getPtr(), m_packEntry[iLayer].m_vertArray.size() * 3);
                if (m_pointEntry.hasData() && (iLayer == 0))
                  this->glConcatArray_dbl(id, (const double*)m_pointEntry.m_vertArray.getPtr(), m_pointEntry.m_vertArray.size() * 3);
                if (Classify::isColors(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_Color, 0);
                  id = this->glAppendArray(m_packEntry[iLayer].m_clrArray.getPtr(), m_packEntry[iLayer].m_clrArray.size() * sizeof(ColorTriplet));
                  if (m_pointEntry.hasData() && (iLayer == 0))
                    this->glConcatArray(id, m_pointEntry.m_clrArray.getPtr(), m_pointEntry.m_clrArray.size() * sizeof(ColorTriplet));
                }
                if (Classify::isNormals(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_Normal, 0);
                  this->glAppendArray_dbl((const double*)m_packEntry[iLayer].m_normArray.getPtr(), m_packEntry[iLayer].m_normArray.size() * 3);
                }
                if (Classify::isTextured(m_packEntry[iLayer].packType))
                {
                  this->glEnableArray(OdOpenGLMetaRecArrayType_TexCoord, 0);
                  id = this->glAppendArray_dbl((const double*)m_packEntry[iLayer].m_texArray.getPtr(), m_packEntry[iLayer].m_texArray.size() * 2);
                  if (m_pointEntry.hasData() && (iLayer == 0))
                    this->glConcatArray_dbl(id, (const double*)m_pointEntry.m_texArray.getPtr(), m_pointEntry.m_texArray.size() * 2);
                  flushTriDelayCache(id);
                }
                if (!m_packEntry[iLayer].m_selMarkArray.empty() && validateGsMarkers(iLayer))
                {
                  this->glEnableSelMarkArray();
                  this->glAppendArray(m_packEntry[iLayer].m_selMarkArray.getPtr(), m_packEntry[iLayer].m_selMarkArray.size() * sizeof(OdOpenGLSelMark));
                }
                // Draw
                this->glDrawArrays(Classify::glType(m_packEntry[iLayer].packType),
                                   0, m_packEntry[iLayer].m_vertArray.size());
                if (!m_packEntry[iLayer].m_selMarkArray.empty())
                {
                  this->glDisableSelMarkArray();
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_selMarkArray);
                }
                if (m_pointEntry.hasData() && (iLayer == 0))
                {
                  if (!m_pointEntry.m_selMarkArray.empty() && validateGsMarkers(-1))
                  {
                    this->glEnableSelMarkArray();
                    OdUInt32 nMarks = m_pointEntry.m_selMarkArray.size();
                    OdOpenGLSelMark *pMarks = m_pointEntry.m_selMarkArray.asArrayPtr();
                    OdUInt32 icSize = m_packEntry[iLayer].m_vertArray.size();
                    for (OdUInt32 nMark = 0; nMark < nMarks; nMark++)
                      pMarks[nMark].uFrom += icSize;
                    this->glAppendArray(pMarks, nMarks * sizeof(OdOpenGLSelMark));
                  }
                  this->glDrawArrays(GL_POINTS,
                    m_packEntry[iLayer].m_vertArray.size(),
                    m_pointEntry.m_vertArray.size());
                  if (!m_pointEntry.m_selMarkArray.empty())
                  {
                    this->glDisableSelMarkArray();
                    OD_OGL_PACKRESERVE(m_pointEntry.m_selMarkArray);
                  }
                }
                if (Classify::isTextured(m_packEntry[iLayer].packType))
                {
                  this->glDisableArray(OdOpenGLMetaRecArrayType_TexCoord);
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_texArray);
                  if (m_pointEntry.hasData() && (iLayer == 0))
                  {
                    OD_OGL_PACKRESERVE(m_pointEntry.m_texArray);
                  }
                }
                if (Classify::isNormals(m_packEntry[iLayer].packType))
                {
                  this->glDisableArray(OdOpenGLMetaRecArrayType_Normal);
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_normArray);
                }
                if (Classify::isColors(m_packEntry[iLayer].packType))
                {
                  this->glDisableArray(OdOpenGLMetaRecArrayType_Color);
                  OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_clrArray);
                  if (m_pointEntry.hasData() && (iLayer == 0))
                  {
                    OD_OGL_PACKRESERVE(m_pointEntry.m_clrArray);
                  }
                  // Restore previous color for next calls
                  if ((type == PackageType_Null) && (!m_pointEntry.hasData() || (iLayer == 0)))
                  {
                    // Typically call from glColor or flush
                    prevState.crColor = 0;
                  }
                  else
                  {
                    // Typically call from another primititve, so color is already setupped
                    ODCOLORREF cRest = prevState.crColor;
                    prevState.crColor = 0;
                    this->glColor(cRest);
                  }
                }
                this->glDisableArray(OdOpenGLMetaRecArrayType_Vertex);
                OD_OGL_PACKRESERVE(m_packEntry[iLayer].m_vertArray);
                if (m_pointEntry.hasData() && (iLayer == 0))
                {
                  OD_OGL_PACKRESERVE(m_pointEntry.m_vertArray);
                }
                flushTraitsOptions(m_packEntry[iLayer].m_outputTraits);
                m_packEntry[iLayer].m_inputTraits.setDefaults();
                m_packEntry[iLayer].m_outputTraits.setDefaults();
              }
            break;
          }
          m_settings.resetFlushing();
        }
      }
    }
    if (apprLayer != 0xFFFFFFFF)
    {
      m_packEntry[apprLayer].packType = type;
      if (m_uReserve[apprLayer] != 0)
      {
        if (type != PackageType_Null)
        {
          if (Classify::isIndexed(type))
          {
            m_packEntry[apprLayer].m_indices.reserve(m_packEntry[apprLayer].m_indices.size() + m_uReserve[apprLayer]);
            m_settings.setIndexedMode(); // could be reseted by previous indexed mode flush
          }
          else
          {
            m_packEntry[apprLayer].m_vertArray.reserve(m_packEntry[apprLayer].m_vertArray.size() + m_uReserve[apprLayer]);
            if (Classify::isColors(type))
              m_packEntry[apprLayer].m_clrArray.reserve(m_packEntry[apprLayer].m_clrArray.size() + m_uReserve[apprLayer]);
            if (Classify::isNormals(type))
              m_packEntry[apprLayer].m_normArray.reserve(m_packEntry[apprLayer].m_normArray.size() + m_uReserve[apprLayer]);
            if (Classify::isTextured(type))
              m_packEntry[apprLayer].m_texArray.reserve(m_packEntry[apprLayer].m_texArray.size() + m_uReserve[apprLayer]);
          }
        }
        m_uReserve[apprLayer] = 0;
      }
    }
    else
    {
      m_packEntry[0].packType = m_packEntry[1].packType = type;
      m_settings.resetIndexedMode();
      m_uReserve[0] = m_uReserve[1] = 0;
    }
    if (m_settings.isRestoreGsMarkers())
    {
      prevState.gsMarker = ~prevState.gsMarker;
      glSelectionMarker(~prevState.gsMarker);
      m_settings.resetRestoreGsMarkers();
    }
  }
}

bool OdOpenGLMetafileWriter::flushOnIndexedMode(bool newMode)
{
  if (m_settings.isFlushing())
    return true;
  if (m_settings.isIndexedMode() != newMode)
  {
    flushPackager(PackageType_Null);
    m_settings.setIndexedMode(newMode);
    if (newMode)
    {
      m_uIndexIncr = m_uMaxId = 0;
    }
    return true;
  }
  return false;
}

void OdOpenGLMetafileWriter::beginIndexedMode(bool bColors, bool bNormals, bool bTexCoords,
                                              OdUInt32 nPoints, const OdGePoint3d *pPoints,
                                              bool bColoredLines, bool bTexturedLines)
{
  // beginIndexedMode guarantees the flags will be not reseted in shell/mesh calls if type is not changed
  flushOnIndexedMode(true);
  PackageType tType = PackageType_ITri;
  PackageType lType = PackageType_ILines;
  if ((bColoredLines && !bColors) || (bTexturedLines && !bTexCoords))
  {
    ODA_FAIL(); // Unsupported packaging configuration
  }
  if (bColors)
  {
    tType = (PackageType)(tType | PackageType_IsColors);
    if (bColoredLines) lType = (PackageType)(lType | PackageType_IsColors);
  }
  if (bNormals)
  {
    tType = (PackageType)(tType | PackageType_IsNormals);
  }
  if (bTexCoords)
  {
    tType = (PackageType)(tType | PackageType_IsTextured);
    if (bTexturedLines) lType = (PackageType)(lType | PackageType_IsTextured);
  }
  if (isES() && ((m_uMaxId + nPoints) > 0xFFFF)) // #15301 : check does id's fit for 16bit indexation
  {
    flushPackager(PackageType_Null);
    m_uIndexIncr = m_uMaxId = 0;
  }
  reserveBuffers(nPoints);
  flushPackager(tType, 1);
  m_uIndexIncr = m_uMaxId;
  m_uMaxId += nPoints;
  m_packEntry[1].m_vertArray.resize(m_uMaxId);
  ::memcpy(m_packEntry[1].m_vertArray.asArrayPtr() + m_uIndexIncr, pPoints, nPoints * sizeof(OdGePoint3d));
  if (bColors)
    m_packEntry[1].m_clrArray.resize(m_uMaxId);
  if (bNormals)
    m_packEntry[1].m_normArray.resize(m_uMaxId);
  if (bTexCoords)
    m_packEntry[1].m_texArray.resize(m_uMaxId);
  m_packEntry[0].packType = lType; // We don't support points here
}

void OdOpenGLMetafileWriter::glEnableOpt(OdOpenGLAttribute attrib)
{
  if (prevState.getCombinedAttribute(attrib) ^ 1)
  {
    flushPackager(PackageType_Null);
    prevState.setCombinedAttribute(attrib, 1);
    writeChunk(OdOpenGLMetaRecType_EnableOpt);
    writeUInt8((OdUInt8)attrib);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
}

void OdOpenGLMetafileWriter::glDisableOpt(OdOpenGLAttribute attrib)
{
  if (prevState.getCombinedAttribute(attrib))
  {
    if (m_packEntry[0].packType != PackageType_Null || m_packEntry[1].packType != PackageType_Null)
      flushPackager(PackageType_Null);
    prevState.setCombinedAttribute(attrib, 0);
    writeChunk(OdOpenGLMetaRecType_DisableOpt);
    writeUInt8((OdUInt8)attrib);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
}

template <typename TraitsOptions>
struct TraitsStorage : public TraitsOptions {
  TraitsOptions& opts;
  TraitsStorage(TraitsOptions& _opts) : TraitsOptions(_opts), opts(_opts) {}
  ~TraitsStorage() { opts = static_cast<TraitsOptions&>(*this); }
};

void OdOpenGLMetafileWriter::glEnableOptToTraits(OdOpenGLAttribute attrib, OdUInt32 trait, bool input)
{
  TraitsOptions &opts = getTraitsOptions(trait, input);
  if (opts.getCombinedAttribute(attrib) == 0)
  {
    TraitsStorage<TraitsOptions> _storage(opts);
    flushPackager(PackageType_Null);
  }
  opts.setCombinedAttribute(attrib, 1);
}

void OdOpenGLMetafileWriter::glDisableOptToTraits(OdOpenGLAttribute attrib, OdUInt32 trait, bool input)
{
  TraitsOptions &opts = getTraitsOptions(trait, input);
  if (opts.getCombinedAttribute(attrib) == 1)
  {
    TraitsStorage<TraitsOptions> _storage(opts);
    flushPackager(PackageType_Null);
  }
  opts.setCombinedAttribute(attrib, 0);
}

void OdOpenGLMetafileWriter::glUnsetOptToTraits(OdOpenGLAttribute attrib, OdUInt32 trait, bool input)
{
  TraitsOptions &opts = getTraitsOptions(trait, input);
  if (opts.getCombinedAttribute(attrib) <= 1)
  {
    TraitsStorage<TraitsOptions> _storage(opts);
    flushPackager(PackageType_Null);
  }
  opts.setCombinedAttribute(attrib, 3);
}

void OdOpenGLMetafileWriter::glDropOpt(OdOpenGLAttribute attrib)
{
  prevState.bOptFlagsTouch.setAttribute(attrib);
}

void OdOpenGLMetafileWriter::glEnableShadingOpt(OdOpenGLShading shad)
{
  if (prevState.getCombinedShading(shad) ^ 1)
  {
    flushPackager(PackageType_Null);
    prevState.setCombinedShading(shad, 1);
    writeChunk(OdOpenGLMetaRecType_EnableShading);
    writeUInt8((OdUInt8)shad);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
}

void OdOpenGLMetafileWriter::glDisableShadingOpt(OdOpenGLShading shad)
{
  if (prevState.getCombinedShading(shad))
  {
    if (m_packEntry[0].packType != PackageType_Null || m_packEntry[1].packType != PackageType_Null)
      flushPackager(PackageType_Null);
    prevState.setCombinedShading(shad, 0);
    writeChunk(OdOpenGLMetaRecType_DisableShading);
    writeUInt8((OdUInt8)shad);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
}

void OdOpenGLMetafileWriter::glEnableShadingOptToTraits(OdOpenGLShading shad, OdUInt32 trait, bool input)
{
  TraitsOptions &opts = getTraitsOptions(trait, input);
  if (opts.getCombinedShading(shad) == 0)
  {
    TraitsStorage<TraitsOptions> _storage(opts);
    flushPackager(PackageType_Null);
  }
  opts.setCombinedShading(shad, 1);
}

void OdOpenGLMetafileWriter::glDisableShadingOptToTraits(OdOpenGLShading shad, OdUInt32 trait, bool input)
{
  TraitsOptions &opts = getTraitsOptions(trait, input);
  if (opts.getCombinedShading(shad) == 1)
  {
    TraitsStorage<TraitsOptions> _storage(opts);
    flushPackager(PackageType_Null);
  }
  opts.setCombinedShading(shad, 0);
}

void OdOpenGLMetafileWriter::glUnsetShadingOptToTraits(OdOpenGLShading shad, OdUInt32 trait, bool input)
{
  TraitsOptions &opts = getTraitsOptions(trait, input);
  if (opts.getCombinedShading(shad) <= 1)
  {
    TraitsStorage<TraitsOptions> _storage(opts);
    flushPackager(PackageType_Null);
  }
  opts.setCombinedShading(shad, 3);
}

void OdOpenGLMetafileWriter::glDropShadingOpt(OdOpenGLShading shad)
{
  prevState.bOptFlagsTouch.setShading(shad);
}

void OdOpenGLMetafileWriter::glColor(ODCOLORREF color)
{
  if (prevState.crColor != color)
  {
    flushPackager(PackageType_Null);
    // #9643 : do blending option enable here
    if (ODGETALPHA(color) == 255)
      glDisableOpt(OdOpenGLAttribute_Blend);
    else
      glEnableOpt(OdOpenGLAttribute_Blend);
    // eof@9643
    prevState.crColor = color;
    const OdGLubyte clrbits[4] = { ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color), ODGETALPHA(color) };
    if (m_prevChunk != OdOpenGLMetaRecType_Color)
    {
      writeChunk(OdOpenGLMetaRecType_Color);
      m_uAffects |= OdOpenGLAffection_Colors;
    }
    else
    {
      seek(4, false);
    }
    writeGLubyteArray(clrbits, 4);
  }
}

ODCOLORREF OdOpenGLMetafileWriter::glTransparency(OdUInt8 alpha, bool bUpdateBlending)
{
  if (bUpdateBlending)
  {
    if (alpha == 255)
      glDisableOpt(OdOpenGLAttribute_Blend);
    else
      glEnableOpt(OdOpenGLAttribute_Blend);
  }
  return prevState.crColor = (prevState.crColor & 0xFFFFFF) | (ODCOLORREF(alpha) << 24);
}

void OdOpenGLMetafileWriter::glSelectionMarker(OdGsMarker gsMarker)
{
  if (prevState.gsMarker != gsMarker)
  {
    if (m_settings.isFlushing() || ((m_packEntry[0].packType == PackageType_Null) && (m_packEntry[1].packType == PackageType_Null)))
    {
      // Make stand-alone marker in case no packaging anything currently
      flushPackager(PackageType_Null);
      if (m_prevChunk != OdOpenGLMetaRecType_SelectionMarker)
      {
        OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdOpenGLGsMarker), 1)
        writeChunk(OdOpenGLMetaRecType_SelectionMarker);
        m_uAffects |= OdOpenGLAffection_Attributes;
      }
      else
      {
        seek(sizeof(OdOpenGLGsMarker), false);
      }
      writeUInt64((OdUInt64)gsMarker);
    }
    prevState.gsMarker = gsMarker;
    if (!m_settings.isFlushing())
    {
      // Pack marker with geometry for increase effectivity of buffered geometry playing
      for (int i = 0; i < 2; i++)
      {
        //if (m_packEntry[i].packType != PackageType_Null)
        {
          OdOpenGLSelMark selMark;
          selMark.gsMark = (OdOpenGLGsMarker)gsMarker;
          if (m_settings.isIndexedMode())
            selMark.uFrom = m_packEntry[i].m_indices.size();
          else
            selMark.uFrom = m_packEntry[i].m_vertArray.size();
          if (m_packEntry[i].m_selMarkArray.empty() ||
              m_packEntry[i].m_selMarkArray.last().uFrom != selMark.uFrom)
            m_packEntry[i].m_selMarkArray.append(selMark);
          else
            m_packEntry[i].m_selMarkArray.last() = selMark;
        }
      }
      if (m_pointMode == PointMode_Sep && !m_settings.isIndexedMode())
      {
        OdOpenGLSelMark selMark;
        selMark.gsMark = (OdOpenGLGsMarker)gsMarker;
        selMark.uFrom = m_pointEntry.m_vertArray.size();
        if (m_pointEntry.m_selMarkArray.empty() ||
            m_pointEntry.m_selMarkArray.last().uFrom != selMark.uFrom)
          m_pointEntry.m_selMarkArray.append(selMark);
        else
          m_pointEntry.m_selMarkArray.last() = selMark;
      }
    }
  }
}

void OdOpenGLMetafileWriter::glSelectionFlags(OdUInt8 selFlags)
{
  if (prevState.nSelectionFlags != selFlags)
  {
    flushPackager(PackageType_Null);
    prevState.nSelectionFlags = selFlags;
    if (m_prevChunk != OdOpenGLMetaRecType_SelectionFlags)
    {
      writeChunk(OdOpenGLMetaRecType_SelectionFlags);
      m_uAffects |= OdOpenGLAffection_Attributes;
    }
    else
      seek(1, false);
    writeUInt8(selFlags);
  }
}

void OdOpenGLMetafileWriter::glEnableSelMarkArray()
{
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdUInt32), 1)
  writeChunk(OdOpenGLMetaRecType_EnableMarkerArray);
  m_uAffects |= OdOpenGLAffection_Arrays;
}

void OdOpenGLMetafileWriter::glDisableSelMarkArray()
{
  writeChunk(OdOpenGLMetaRecType_DisableMarkerArray);
}

void OdOpenGLMetafileWriter::glLineweight(OdDb::LineWeight lwd)
{
  if (!prevState.lwd.isForcedLineweight() && ((prevState.lwd.m_type != OdOpenGLLwdSetting::kDatabase) || (prevState.lwd.m_val.m_lwLineWeight != lwd)))
  {
    flushPackager(PackageType_Null);
    const bool bPrevIsPs = prevState.lwd.isPs();
    prevState.lwd.m_type = OdOpenGLLwdSetting::kDatabase;
    prevState.lwd.m_val.m_lwLineWeight = lwd;
    if ((m_prevChunk != OdOpenGLMetaRecType_Lineweight) || bPrevIsPs)
    {
      OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdInt16), 2)
      writeChunk(OdOpenGLMetaRecType_Lineweight);
      m_uAffects |= OdOpenGLAffection_Attributes;
    }
    else
    {
      seek(sizeof(OdUInt8) + sizeof(OdInt16), false);
    }
    writeUInt8(0);
    writeInt16((OdInt16)lwd);
  }
}

void OdOpenGLMetafileWriter::glLineweight(double lwd, bool bAbsolute)
{
  const OdOpenGLLwdSetting::LwdType type = (bAbsolute) ? OdOpenGLLwdSetting::kAbsolute : OdOpenGLLwdSetting::kPlotStyle;
  if (!prevState.lwd.isForcedLineweight() && ((prevState.lwd.m_type != type) || !OdEqual(prevState.lwd.m_val.m_dLineWeight, lwd)))
  {
    flushPackager(PackageType_Null);
    const bool bPrevIsPs = prevState.lwd.isPs();
    prevState.lwd.m_type = type;
    prevState.lwd.m_val.m_dLineWeight = lwd;
    if ((m_prevChunk != OdOpenGLMetaRecType_Lineweight) || !bPrevIsPs)
    {
      OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(double), 2)
      writeChunk(OdOpenGLMetaRecType_Lineweight);
      m_uAffects |= OdOpenGLAffection_Attributes;
    }
    else
    {
      seek(sizeof(OdUInt8) + sizeof(double), false);
    }
    writeUInt8((OdUInt8)type);
    writeDouble(lwd);
  }
}

void OdOpenGLMetafileWriter::glLineweight(OdInt32 lwd)
{
  if (!lwd)
  {
    prevState.lwd.m_val.m_pixLineWeight = 0;
  }
  else if ((prevState.lwd.m_type != OdOpenGLLwdSetting::kPixels) || (prevState.lwd.m_val.m_pixLineWeight != lwd))
  {
    flushPackager(PackageType_Null);
    const bool bPrevIsPs = prevState.lwd.isPs();
    prevState.lwd.m_type = OdOpenGLLwdSetting::kPixels;
    prevState.lwd.m_val.m_pixLineWeight = lwd;
    if ((m_prevChunk != OdOpenGLMetaRecType_Lineweight) || bPrevIsPs)
    {
      OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdInt16), 2)
      writeChunk(OdOpenGLMetaRecType_Lineweight);
      m_uAffects |= OdOpenGLAffection_Attributes;
    }
    else
    {
      seek(sizeof(OdUInt8) + sizeof(OdInt16), false);
    }
    writeUInt8(2);
    writeInt16((OdInt16)lwd);
  }
}

void OdOpenGLMetafileWriter::glLineStyle(bool bByDefault, OdPs::LineEndStyle lesStyle, OdPs::LineJoinStyle ljsStyle)
{
  if (bByDefault)
  {
    if (!prevState.lwdStyle.m_bCurUsesDef)
    {
      flushPackager(PackageType_Null);
      prevState.lwdStyle.m_bCurUsesDef = true;
      writeChunk(OdOpenGLMetaRecType_Linestyle);
      m_uAffects |= OdOpenGLAffection_Attributes;
      writeUInt8(1);
    }
  }
  else
  {
    if (prevState.lwdStyle.m_bCurUsesDef || prevState.lwdStyle.m_curLesStyle != lesStyle || prevState.lwdStyle.m_curLjsStyle != ljsStyle)
    {
      flushPackager(PackageType_Null);
      prevState.lwdStyle.m_bCurUsesDef = false;
      prevState.lwdStyle.m_curLesStyle = lesStyle;
      prevState.lwdStyle.m_curLjsStyle = ljsStyle;
      writeChunk(OdOpenGLMetaRecType_Linestyle);
      m_uAffects |= OdOpenGLAffection_Attributes;
      writeUInt8(0);
      writeUInt8((OdUInt8)lesStyle);
      writeUInt8((OdUInt8)ljsStyle);
    }
  }
}

void OdOpenGLMetafileWriter::glCullFace(OdOpenGLFaceCulling mode)
{
  if (prevState.cullMode() != mode)
  {
    flushPackager(PackageType_Null);
    prevState.setCullMode(mode);
    writeChunk(OdOpenGLMetaRecType_CullFace);
    writeUInt8((OdUInt8)mode);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
}

void OdOpenGLMetafileWriter::glCullFaceToTraits(OdOpenGLFaceCulling mode, OdUInt32 trait, bool input)
{
  TraitsOptions &options = getTraitsOptions(trait, input);
  if (((options.eCullMode != 0xFF) || (isPackaging(trait) && input && hasGeometry(trait))) && options.cullMode() != mode)
  {
    TraitsStorage<TraitsOptions> _storage(options);
    flushPackager(PackageType_Null);
  }
  options.setCullMode(mode);
}

void OdOpenGLMetafileWriter::glUnsetCullFaceToTraits(OdUInt32 trait, bool input)
{
  TraitsOptions &options = getTraitsOptions(trait, input);
  if (options.eCullMode != 0xFF)
  {
    TraitsStorage<TraitsOptions> _storage(options);
    flushPackager(PackageType_Null);
  }
  options.eCullMode = 0xFF;
}

void OdOpenGLMetafileWriter::glLineStipple(OdOpenGLLinePatt::Entry entry, OdUInt8 state)
{
  if (prevState.lnPatt.data.named.reg_linetype == 0xFF)
  { // Initialize input metafile
    flushPackager(PackageType_Null);
    prevState.lnPatt.data.named.reg_linetype = 0;
    writeChunk(OdOpenGLMetaRecType_LStipple);
    writeUInt8((OdUInt8)OdOpenGLLinePatt::kLnPattAll);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
  if (((entry == OdOpenGLLinePatt::kLnPattAll) && (prevState.lnPatt.data.summ != 0)) ||
      ((entry != OdOpenGLLinePatt::kLnPattAll) && (prevState.lnPatt.data.reg[entry] != state)))
  {
    flushPackager(PackageType_Null);
    glDropLineStipple(entry, state);
    writeChunk(OdOpenGLMetaRecType_LStipple);
    writeUInt8((OdUInt8)entry);
    if (entry != OdOpenGLLinePatt::kLnPattAll)
      writeUInt8(state);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
}

void OdOpenGLMetafileWriter::glDropLineStipple(OdOpenGLLinePatt::Entry entry, OdUInt8 state)
{
  if (entry == OdOpenGLLinePatt::kLnPattAll)
    prevState.lnPatt.data.summ = 0;
  else
    prevState.lnPatt.data.reg[entry] = state;
}

void OdOpenGLMetafileWriter::glPolygonStipple(OdOpenGLFillPatt::Entry entry, OdUInt8 state)
{
  if (prevState.fillPatt.data.named.reg_fillstyle == 0xFF)
  { // Initialize input metafile
    flushPackager(PackageType_Null);
    prevState.fillPatt.data.named.reg_fillstyle = 0;
    writeChunk(OdOpenGLMetaRecType_PStipple);
    writeUInt8((OdUInt8)OdOpenGLFillPatt::kFillPattAll);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
  if (((entry == OdOpenGLFillPatt::kFillPattAll) && (prevState.fillPatt.data.summ != 0)) ||
      ((entry != OdOpenGLFillPatt::kFillPattAll) && (prevState.fillPatt.data.reg[entry] != state)))
  {
    flushPackager(PackageType_Null);
    glDropPolygonStipple(entry, state);
    writeChunk(OdOpenGLMetaRecType_PStipple);
    writeUInt8((OdUInt8)entry);
    if (entry != OdOpenGLFillPatt::kFillPattAll)
      writeUInt8(state);
    m_uAffects |= OdOpenGLAffection_Attributes;
  }
}

void OdOpenGLMetafileWriter::glDropPolygonStipple(OdOpenGLFillPatt::Entry entry, OdUInt8 state)
{
  if (entry == OdOpenGLFillPatt::kFillPattAll)
    prevState.fillPatt.data.summ = 0;
  else
    prevState.fillPatt.data.reg[entry] = state;
}

void OdOpenGLMetafileWriter::glEnableArray(OdOpenGLMetaRecArrayType type, OdGLint /*stride*/ /* 0 for default */)
{
  flushPackager(PackageType_Null);
  //prevState.bClientState[type] = 1;
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdUInt32), 2)
  writeChunk(OdOpenGLMetaRecType_EnableArray);
  writeUInt8((OdUInt8)type);
  //if (stride == 0)
  //{
  //  writeUInt8(0);
  //}
  //else
  //{
  //  writeUInt8(1);
  //  writeUInt8((OdUInt8)stride); // can't be more 256 I think
  //}
  // Next must be pointer to data saved manually
  m_uAffects |= OdOpenGLAffection_Arrays;
}

void OdOpenGLMetafileWriter::glDisableArray(OdOpenGLMetaRecArrayType type)
{
  //if (prevState.bClientState[type] == 1)
  {
    flushPackager(PackageType_Null);
    writeChunk(OdOpenGLMetaRecType_DisableArray);
    writeUInt8((OdUInt8)type);
    m_uAffects |= OdOpenGLAffection_Arrays;
  }
}

void OdOpenGLMetafileWriter::glPoint(const OdGePoint3d &point)
{
  flushPackager(PackageType_Null);
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(double), 1)
#else
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdGLfloat), 1)
#endif
  writeChunk(OdOpenGLMetaRecType_VPoint);
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  writePoint3d(point);
#else
  const OdGLfloat tmp[3] = { dbl2float(point.x), dbl2float(point.y), dbl2float(point.z) };
  writeGLfloatArray(tmp, 3);
#endif
  m_uAffects |= OdOpenGLAffection_Geometry;
}

void OdOpenGLMetafileWriter::glLine(const OdGePoint3d &point1, const OdGePoint3d &point2)
{
  flushPackager(PackageType_Null);
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(double), 1)
#else
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdGLfloat), 1)
#endif
  writeChunk(OdOpenGLMetaRecType_VLine);
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  writePoint3d(point1);
  writePoint3d(point2);
#else
  const OdGLfloat tmp[6] = { dbl2float(point1.x), dbl2float(point1.y), dbl2float(point1.z),
                             dbl2float(point2.x), dbl2float(point2.y), dbl2float(point2.z) };
  writeGLfloatArray(tmp, 6);
#endif
  m_uAffects |= OdOpenGLAffection_Geometry;
  if (m_settings.isLinesRepMode())
    this->glPoint(point1); // One point for single line is enough
}

void OdOpenGLMetafileWriter::glPoint(OdInt32 id)
{
  flushPackager(PackageType_Null);
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdInt32), 1)
  writeChunk(OdOpenGLMetaRecType_IPoint);
  writeInt32(id);
  m_uAffects |= OdOpenGLAffection_Geometry;
}

void OdOpenGLMetafileWriter::glLine(OdInt32 id1, OdInt32 id2)
{
  flushPackager(PackageType_Null);
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdInt32), 1)
  writeChunk(OdOpenGLMetaRecType_ILine);
  writeInt32(id1);
  writeInt32(id2);
  m_uAffects |= OdOpenGLAffection_Geometry;
  if (m_settings.isLinesRepMode())
    this->glPoint(id1); // One point for single line is enough
}

void OdOpenGLMetafileWriter::glDrawArrays(OdGLenum mode, OdGLint first, OdGLsizei count)
{
  flushPackager(PackageType_Null);
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdGLint), 2)
  writeChunk(OdOpenGLMetaRecType_DrawArrays);
  writeUInt8((OdUInt8)mode);
  writeGLint(first);
  writeGLint(count);
  m_uAffects |= OdOpenGLAffection_Geometry;
  if (m_settings.isLinesRepMode() && (mode >= GL_LINES) && (mode <= GL_LINE_STRIP))
    this->glDrawArrays(GL_POINTS, first, count);
}

void OdOpenGLMetafileWriter::glDrawElements(OdGLenum mode, OdGLsizei count, const OdInt32 *indices)
{
  flushPackager(PackageType_Null);
  // Always int's
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdGLint), 2)
  writeChunk(OdOpenGLMetaRecType_DrawElements);
  writeUInt8((OdUInt8)mode);
  writeGLint(count);
  if (!isES())
    glAppendArray(indices, sizeof(OdInt32) * count);
  else
  {
    OdOpenGLArrayWrapper newArray;
    newArray.setArray_ush(indices, count);
    writeUInt32(glContainer()->appendArrayElement(newArray));
  }
  m_uAffects |= OdOpenGLAffection_Geometry;
  if (m_settings.isLinesRepMode() && (mode >= GL_LINES) && (mode <= GL_LINE_STRIP))
  {
    OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdGLint), 2)
    writeChunk(OdOpenGLMetaRecType_DrawElements);
    writeUInt8((OdUInt8)GL_POINTS);
    writeGLint(count);
    writeUInt32(glContainer()->lastArrayElement());
  }
}

void OdOpenGLMetafileWriter::glPackLine(OdInt32 nPoints, const OdGePoint3d *pPoints)
{
  bool bStrip = false;
  if (m_packEntry[0].packType == PackageType_VLines)
    bStrip = nPoints > OD_OGL_MINLINESTRIPWORK;
  else
    bStrip = nPoints > OD_OGL_MINLINESTRIP;
  if (m_pointMode == PointMode_Opt)
  {
    // This point may be skipped, because point displayed as line is invisible
    m_bPrevPoint = 0;
  }
  if (bStrip)
  {
    flushPackager(PackageType_Null);
    glDisableOpt(OdOpenGLAttribute_Lighting);
    glDisableOpt(OdOpenGLAttribute_Texture2D);
    glEnableArray(OdOpenGLMetaRecArrayType_Vertex, 0);
    glAppendArray_dbl((const double*)pPoints, nPoints * 3);
    glDrawArrays(GL_LINE_STRIP, 0, nPoints);
    glDisableArray(OdOpenGLMetaRecArrayType_Vertex);
    return;
  }
  if (!flushOnIndexedMode(false))
  {
    if ((m_packEntry[0].packType == PackageType_VPoints) && (m_pointMode == PointMode_Sep))
    { // We able to mutate points into lines for this mode
      m_pointEntry.m_vertArray.insert(m_pointEntry.m_vertArray.end(), m_packEntry[0].m_vertArray.begin(), m_packEntry[0].m_vertArray.end());
      m_packEntry[0].m_vertArray.clear();
      if (!m_packEntry[0].m_selMarkArray.isEmpty())
      {
        m_pointEntry.m_selMarkArray.insert(m_pointEntry.m_selMarkArray.end(), m_packEntry[0].m_selMarkArray.begin(), m_packEntry[0].m_selMarkArray.end());
        m_packEntry[0].m_selMarkArray.clear();
      }
      m_packEntry[0].packType = PackageType_VLines;
    }
    else
      flushPackager(PackageType_VLines, 0);
  }
  else
    m_packEntry[0].packType = PackageType_VLines;

  const OdInt32 nCount = (nPoints - 1) * 2;
  m_packEntry[0].m_vertArray.resize(m_packEntry[0].m_vertArray.size() + nCount);
  OdGePoint3d *pArray = m_packEntry[0].m_vertArray.asArrayPtr() + m_packEntry[0].m_vertArray.size() - nCount;
  for (OdInt32 i = 1; i < nPoints; i++, pArray += 2)
  {
    pArray[0] = pPoints[i - 1];
    pArray[1] = pPoints[i];
  }
}

void OdOpenGLMetafileWriter::glPackCLine(OdInt32 nPoints, const OdGePoint3d *pPoints, const OdCmEntityColor *pColors)
{
  if (m_pointMode == PointMode_Opt)
  {
    // This point may be skipped, because point displayed as line is invisible
    m_bPrevPoint = 0;
  }
  if (!flushOnIndexedMode(false))
  {
    if ((m_packEntry[0].packType == PackageType_VCPoints) && (m_pointMode == PointMode_Sep))
    { // We able to mutate points into lines for this mode
      m_pointEntry.m_vertArray.insert(m_pointEntry.m_vertArray.end(), m_packEntry[0].m_vertArray.begin(), m_packEntry[0].m_vertArray.end());
      m_packEntry[0].m_vertArray.clear();
      if (!m_packEntry[0].m_selMarkArray.isEmpty())
      {
        m_pointEntry.m_selMarkArray.insert(m_pointEntry.m_selMarkArray.end(), m_packEntry[0].m_selMarkArray.begin(), m_packEntry[0].m_selMarkArray.end());
        m_packEntry[0].m_selMarkArray.clear();
      }
      if (!m_packEntry[0].m_clrArray.isEmpty())
      {
        m_pointEntry.m_clrArray.insert(m_pointEntry.m_clrArray.end(), m_packEntry[0].m_clrArray.begin(), m_packEntry[0].m_clrArray.end());
        m_packEntry[0].m_clrArray.clear();
      }
      m_packEntry[0].packType = PackageType_VCLines;
    }
    else
      flushPackager(PackageType_VCLines, 0);
  }
  else
    m_packEntry[0].packType = PackageType_VCLines;

  const OdInt32 nCount = (nPoints - 1) * 2;
  m_packEntry[0].m_vertArray.resize(m_packEntry[0].m_vertArray.size() + nCount);
  m_packEntry[0].m_clrArray.resize(m_packEntry[0].m_clrArray.size() + nCount);
  OdGePoint3d *pArray = m_packEntry[0].m_vertArray.asArrayPtr() + m_packEntry[0].m_vertArray.size() - nCount;
  ColorTriplet *pClrs = m_packEntry[0].m_clrArray.asArrayPtr() + m_packEntry[0].m_clrArray.size() - nCount;
  for (OdInt32 i = 1; i < nPoints; i++, pArray += 2, pClrs += 2)
  {
    pArray[0] = pPoints[i - 1];
    pArray[1] = pPoints[i];
    pClrs[0].r = pColors[i - 1].red(); pClrs[0].g = pColors[i - 1].green(); pClrs[0].b = pColors[i - 1].blue();
    pClrs[1].r = pColors[i].red(); pClrs[1].g = pColors[i].green(); pClrs[1].b = pColors[i].blue();
#if (OD_OGL_NCOLORCOMPONENTS != 3)
    pClrs[1].a = pClrs[0].a = ODGETALPHA(prevState.crColor);
#endif
  }
}

void OdOpenGLMetafileWriter::glPackPoint(const OdGePoint3d *pPt)
{
  if (m_packEntry[0].packType == PackageType_VLines && m_pointMode != PointMode_Unopt)
  {
    switch (m_pointMode)
    {
      case PointMode_Opt:
        if (m_bPrevPoint == 0)
        {
          m_bPrevPoint = 1;
          m_prevPoint = *pPt;
          return;
        }
        else if (m_bPrevPoint == 1)
        {
          m_bPrevPoint = 2;
          glPackPoint(&m_prevPoint);
        }
      break;
      case PointMode_High:
        {
          const OdGePoint3d aPts[2] = { *pPt, *pPt };
          glPackLine(2, aPts);
        }
      return;
      case PointMode_Sep:
        m_pointEntry.m_vertArray.append(*pPt);
      return;
      default: break;
    }
  }
  if (!flushOnIndexedMode(false))
    flushPackager(PackageType_VPoints, 0);
  else
    m_packEntry[0].packType = PackageType_VPoints;
  m_packEntry[0].m_vertArray.append(*pPt);
}

void OdOpenGLMetafileWriter::glPackCPoint(const OdGePoint3d *pPt, const OdCmEntityColor *color)
{
  if (m_packEntry[0].packType == PackageType_VLines && m_pointMode != PointMode_Unopt)
  {
    switch (m_pointMode)
    {
      // We don't support "Opt" mode for color points
      case PointMode_High:
      {
        const OdGePoint3d aPts[2] = { *pPt, *pPt };
        const OdCmEntityColor aCls[2] = { *color, *color };
        glPackCLine(2, aPts, aCls);
      }
      return;
      case PointMode_Sep:
      {
        ColorTriplet clr;
        m_pointEntry.m_vertArray.append(*pPt);
        clr.r = color->red(); clr.g = color->green(); clr.b = color->blue();
#if (OD_OGL_NCOLORCOMPONENTS != 3)
        clr.a = ODGETALPHA(prevState.crColor);
#endif
        m_pointEntry.m_clrArray.append(clr);
      }
      return;
      default: break;
    }
  }
  if (!flushOnIndexedMode(false))
    flushPackager(PackageType_VCPoints, 0);
  else
    m_packEntry[0].packType = PackageType_VPoints;
  m_packEntry[0].m_vertArray.append(*pPt);
  ColorTriplet clr;
  clr.r = color->red(); clr.g = color->green(); clr.b = color->blue();
#if (OD_OGL_NCOLORCOMPONENTS != 3)
  clr.a = ODGETALPHA(prevState.crColor);
#endif
  m_packEntry[0].m_clrArray.append(clr);
}

void OdOpenGLMetafileWriter::glPackLine(OdInt32 nPoints, const OdInt32 *pPoints)
{
  //if (!flushOnIndexedMode(true))
  //  flushPackager(PackageType_ILines, 0);
  ODA_ASSERT(Classify::isLin(m_packEntry[0].packType));
  //m_packEntry[0].packType = PackageType_ILines; // Do not overwrite packaging type
  OdInt32 nCount = (nPoints - 1) * 2;
  m_packEntry[0].m_indices.resize(m_packEntry[0].m_indices.size() + nCount);
  OdInt32 *pArray = m_packEntry[0].m_indices.asArrayPtr() + m_packEntry[0].m_indices.size() - nCount;
  for (OdInt32 i = 1; i < nPoints; i++, pArray += 2)
  {
    pArray[0] = pPoints[i - 1] + m_uIndexIncr;
    pArray[1] = pPoints[i] + m_uIndexIncr;
  }
}

void OdOpenGLMetafileWriter::glSetPackILineColors(const OdInt32 *ids, const OdCmEntityColor *color1, const OdCmEntityColor *color2)
{
  ODA_ASSERT(Classify::isLin(m_packEntry[0].packType) && Classify::isColors(m_packEntry[0].packType));
  ColorTriplet ct;
#if (OD_OGL_NCOLORCOMPONENTS == 3)
  ct.r = color1->red(); ct.g = color1->green(); ct.b = color1->blue();
  m_packEntry[0].m_clrArray[ids[0] + m_uIndexIncr] = ct;
  ct.r = color2->red(); ct.g = color2->green(); ct.b = color2->blue();
  m_packEntry[0].m_clrArray[ids[1] + m_uIndexIncr] = ct;
#else
  const OdUInt8 a = ODGETALPHA(prevState.crColor);
  ct.r = color1->red(); ct.g = color1->green(); ct.b = color1->blue(); ct.a = a;
  m_packEntry[0].m_clrArray[ids[0] + m_uIndexIncr] = ct;
  ct.r = color2->red(); ct.g = color2->green(); ct.b = color2->blue(); ct.a = a;
  m_packEntry[0].m_clrArray[ids[1] + m_uIndexIncr] = ct;
#endif
}

// @@@TODO: that's very unoptimal way for triangles data packaging. Optimize later.
// place something to reserve approximated count of vertices in shell/mesh calls.

inline void packTriangleVertices(OdOpenGLMetafileWriter::FriendPEntry &pe,
                                 const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3)
{
  //OdUInt32 uSize = pe.m_vertArray.size();
  //pe.m_vertArray.resize(uSize + 3);
  pe.m_vertArray.append(*pPt1);
  pe.m_vertArray.append(*pPt2);
  pe.m_vertArray.append(*pPt3);
}

inline void packTriangleColors(OdOpenGLMetafileWriter::FriendPEntry &pe,
                               const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3,
#if (OD_OGL_NCOLORCOMPONENTS == 4)
                               OdUInt8 a)
#else
                               OdUInt8 /*a*/)
#endif
{
  OdOpenGLMetafileWriter::ColorTriplet ct;
#if (OD_OGL_NCOLORCOMPONENTS == 3)
  ct.r = color1->red(); ct.g = color1->green(); ct.b = color1->blue();
  pe.m_clrArray.append(ct);
  ct.r = color2->red(); ct.g = color2->green(); ct.b = color2->blue();
  pe.m_clrArray.append(ct);
  ct.r = color3->red(); ct.g = color3->green(); ct.b = color3->blue();
  pe.m_clrArray.append(ct);
#else
  ct.r = color1->red(); ct.g = color1->green(); ct.b = color1->blue(); ct.a = a;
  pe.m_clrArray.append(ct);
  ct.r = color2->red(); ct.g = color2->green(); ct.b = color2->blue(); ct.a = a;
  pe.m_clrArray.append(ct);
  ct.r = color3->red(); ct.g = color3->green(); ct.b = color3->blue(); ct.a = a;
  pe.m_clrArray.append(ct);
#endif
}

inline void packTriangleNormals(OdOpenGLMetafileWriter::FriendPEntry &pe,
                                const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3)
{
  pe.m_normArray.append(*pNr1);
  pe.m_normArray.append(*pNr2);
  pe.m_normArray.append(*pNr3);
}

inline void packTriangleTexCoords(OdOpenGLMetafileWriter::FriendPEntry &pe,
                                  const double *pTC1, const double *pTC2, const double *pTC3)
{
  OdOpenGLMetafileWriter::TextureCoord tc;
  tc.s = pTC1[0]; tc.t = pTC1[1];
  pe.m_texArray.append(tc);
  tc.s = pTC2[0]; tc.t = pTC2[1];
  pe.m_texArray.append(tc);
  tc.s = pTC3[0]; tc.t = pTC3[1];
  pe.m_texArray.append(tc);
}

void OdOpenGLMetafileWriter::reserveBuffers(OdUInt32 nPoints)
{
  if (nPoints >= 3)
    reserveBuffers(nPoints * 2, (nPoints - 2) * 3);
  else
    reserveBuffers(0, 0);
}

void OdOpenGLMetafileWriter::reserveBuffers(OdUInt32 valIL, OdUInt32 valTR)
{
  m_uReserve[0] = valIL;
  m_uReserve[1] = valTR;
}

#define OD_OGL_INITIATETRI(TriType) \
  if (!flushOnIndexedMode(false)) \
    flushPackager(TriType, 1); \
  else \
    m_packEntry[1].packType = TriType

void OdOpenGLMetafileWriter::glPackTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3)
{
  OD_OGL_INITIATETRI(PackageType_VTri);
  packTriangleVertices(m_packEntry[1], pPt1, pPt2, pPt3);
}

void OdOpenGLMetafileWriter::glPackCTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                                             const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3)
{
  OD_OGL_INITIATETRI(PackageType_VCTri);
  packTriangleVertices(m_packEntry[1], pPt1, pPt2, pPt3);
  packTriangleColors  (m_packEntry[1], color1, color2, color3, ODGETALPHA(prevState.crColor));
}

void OdOpenGLMetafileWriter::glPackNTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                                             const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3)
{
  OD_OGL_INITIATETRI(PackageType_VNTri);
  packTriangleVertices(m_packEntry[1], pPt1, pPt2, pPt3);
  packTriangleNormals (m_packEntry[1], pNr1, pNr2, pNr3);
}

void OdOpenGLMetafileWriter::glPackCNTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                                              const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3,
                                              const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3)
{
  OD_OGL_INITIATETRI(PackageType_VCNTri);
  packTriangleVertices(m_packEntry[1], pPt1, pPt2, pPt3);
  packTriangleColors  (m_packEntry[1], color1, color2, color3, ODGETALPHA(prevState.crColor));
  packTriangleNormals (m_packEntry[1], pNr1, pNr2, pNr3);
}

#define OD_OGL_DELAYMAPPINGTRI() \
  if (bDelayMapping) \
  { \
    OdOpenGLDelayedMappingEntry::ArrayElement nElem; \
    nElem.texId = m_delEntry.last()->m_texIds.size(); \
    nElem.verts[0] = *pPt1; nElem.verts[1] = *pPt2; nElem.verts[2] = *pPt3; \
    nElem.normal = *pNr2; \
    nElem.crdIds[0] = m_packEntry[1].m_texArray.size() - 3; \
    nElem.crdIds[1] = m_packEntry[1].m_texArray.size() - 2; \
    nElem.crdIds[2] = m_packEntry[1].m_texArray.size() - 1; \
    m_delEntry.last()->m_Elements.append(nElem); \
    m_delEntry.last()->bFlushed = false; \
  }

void OdOpenGLMetafileWriter::glPackNTTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                                              const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3,
                                              const double *pTC1, const double *pTC2, const double *pTC3, bool bDelayMapping)
{
  OD_OGL_INITIATETRI(PackageType_VNTTri);
  packTriangleVertices (m_packEntry[1], pPt1, pPt2, pPt3);
  packTriangleNormals  (m_packEntry[1], pNr1, pNr2, pNr3);
  packTriangleTexCoords(m_packEntry[1], pTC1, pTC2, pTC3);
  OD_OGL_DELAYMAPPINGTRI()
}

void OdOpenGLMetafileWriter::glPackCNTTriangle(const OdGePoint3d *pPt1, const OdGePoint3d *pPt2, const OdGePoint3d *pPt3,
                                               const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3,
                                               const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3,
                                               const double *pTC1, const double *pTC2, const double *pTC3, bool bDelayMapping)
{
  OD_OGL_INITIATETRI(PackageType_VCNTTri);
  packTriangleVertices (m_packEntry[1], pPt1, pPt2, pPt3);
  packTriangleColors   (m_packEntry[1], color1, color2, color3, ODGETALPHA(prevState.crColor));
  packTriangleNormals  (m_packEntry[1], pNr1, pNr2, pNr3);
  packTriangleTexCoords(m_packEntry[1], pTC1, pTC2, pTC3);
  OD_OGL_DELAYMAPPINGTRI()
}

#undef OD_OGL_DELAYMAPPINGTRI
#undef OD_OGL_INITIATETRI

void OdOpenGLMetafileWriter::glPackITriangle(const OdInt32 *ids)
{
  OdInt32Vector::size_type sz = m_packEntry[1].m_indices.size();
  m_packEntry[1].m_indices.resize(sz + 3);

  // hack - resize should separate referenced array, getPtr will work with non referenced, so
  // there is no reason to use asArrayPtr with time consuming check of copy_if_referenced.
  OdInt32* pIndexes = (OdInt32*)(m_packEntry[1].m_indices.getPtr() + sz);
  pIndexes[0] = ids[0] + m_uIndexIncr;
  pIndexes[1] = ids[1] + m_uIndexIncr;
  pIndexes[2] = ids[2] + m_uIndexIncr;
}

void OdOpenGLMetafileWriter::glSetPackITriColors(const OdInt32 *ids,
                                                 const OdCmEntityColor *color1, const OdCmEntityColor *color2, const OdCmEntityColor *color3)
{
  ColorTriplet ct;
#if (OD_OGL_NCOLORCOMPONENTS == 3)
  ct.r = color1->red(); ct.g = color1->green(); ct.b = color1->blue();
  m_packEntry[1].m_clrArray[ids[0] + m_uIndexIncr] = ct;
  ct.r = color2->red(); ct.g = color2->green(); ct.b = color2->blue();
  m_packEntry[1].m_clrArray[ids[1] + m_uIndexIncr] = ct;
  ct.r = color3->red(); ct.g = color3->green(); ct.b = color3->blue();
  m_packEntry[1].m_clrArray[ids[2] + m_uIndexIncr] = ct;
#else
  const OdUInt8 a = ODGETALPHA(prevState.crColor);
  ct.r = color1->red(); ct.g = color1->green(); ct.b = color1->blue(); ct.a = a;
  m_packEntry[1].m_clrArray[ids[0] + m_uIndexIncr] = ct;
  ct.r = color2->red(); ct.g = color2->green(); ct.b = color2->blue(); ct.a = a;
  m_packEntry[1].m_clrArray[ids[1] + m_uIndexIncr] = ct;
  ct.r = color3->red(); ct.g = color3->green(); ct.b = color3->blue(); ct.a = a;
  m_packEntry[1].m_clrArray[ids[2] + m_uIndexIncr] = ct;
#endif
}

void OdOpenGLMetafileWriter::glSetPackITriNormals(const OdInt32 *ids,
                                                  const OdGeVector3d *pNr1, const OdGeVector3d *pNr2, const OdGeVector3d *pNr3)
{
  OdGeVector3d *pNormals = m_packEntry[1].m_normArray.asArrayPtr() + m_uIndexIncr;

  pNormals[ids[0]] = *pNr1;
  pNormals[ids[1]] = *pNr2;
  pNormals[ids[2]] = *pNr3;
}

void OdOpenGLMetafileWriter::glSetPackITriTexCoords(const OdInt32 *ids,
                                                    const double *pTC1, const double *pTC2, const double *pTC3,
                                                    bool bDelayMapping)
{
  TextureCoord tc;
  tc.s = pTC1[0]; tc.t = pTC1[1];
  m_packEntry[1].m_texArray[ids[0] + m_uIndexIncr] = tc;
  tc.s = pTC2[0]; tc.t = pTC2[1];
  m_packEntry[1].m_texArray[ids[1] + m_uIndexIncr] = tc;
  tc.s = pTC3[0]; tc.t = pTC3[1];
  m_packEntry[1].m_texArray[ids[2] + m_uIndexIncr] = tc;
  if (bDelayMapping)
  {
    OdOpenGLDelayedMappingEntry::ArrayElement nElem;
    nElem.texId = m_delEntry.last()->m_texIds.size();
    nElem.verts[0] = m_packEntry[1].m_vertArray.at(ids[0] + m_uIndexIncr);
    nElem.verts[1] = m_packEntry[1].m_vertArray.at(ids[1] + m_uIndexIncr);
    nElem.verts[2] = m_packEntry[1].m_vertArray.at(ids[2] + m_uIndexIncr);
    nElem.normal = m_packEntry[1].m_normArray.at(ids[1] + m_uIndexIncr);
    nElem.crdIds[0] = ids[0] + m_uIndexIncr;
    nElem.crdIds[1] = ids[1] + m_uIndexIncr;
    nElem.crdIds[2] = ids[2] + m_uIndexIncr;
    m_delEntry.last()->m_Elements.append(nElem);
    m_delEntry.last()->bFlushed = false;
  }
}

// @@@

OdUInt32 OdOpenGLMetafileWriter::glAppendArray(const void *pPtr, OdUInt32 size, bool writeId)
{
  // No flushing here
  OdOpenGLArrayWrapper newArray;
  newArray.setArray(pPtr, size);
  OdUInt32 id = glContainer()->appendArrayElement(newArray);
  if (writeId)
    writeUInt32(id);
  return id;
}

void OdOpenGLMetafileWriter::glConcatArray(OdUInt32 id, const void *pPtr, OdUInt32 size, bool writeId)
{
  // No flushing here
  glContainer()->arrayElement(id).concatArray(pPtr, size);
  if (writeId)
    writeUInt32(id);
}

OdUInt32 OdOpenGLMetafileWriter::glAppendArray_dbl(const double *pPtr, OdUInt32 size, bool writeId)
{
  // No flushing here
  OdOpenGLArrayWrapper newArray;
  newArray.setArray_dbl(pPtr, size);
  OdUInt32 id = glContainer()->appendArrayElement(newArray);
  if (writeId)
    writeUInt32(id);
  return id;
}

void OdOpenGLMetafileWriter::glConcatArray_dbl(OdUInt32 id, const double *pPtr, OdUInt32 size, bool writeId)
{
  // No flushing here
  glContainer()->arrayElement(id).concatArray_dbl(pPtr, size);
  if (writeId)
    writeUInt32(id);
}

OdUInt32 OdOpenGLMetafileWriter::glAppendRxObject(OdRxObjectPtr pObject, bool writeId)
{
  // No flushing here
  OdUInt32 id;
  if (pObject.isNull())
  {
    id = 0xFFFFFFFF;
  }
  else
  {
    OdOpenGLFlatMetafileContainerWriter *pCont = glContainerIO();
    pCont->glContainer()->m_RxObjects.append(pObject);
    id = pCont->glContainer()->m_RxObjects.size() - 1;
  }
  if (writeId)
    writeUInt32(id);
  return id;
}

void OdOpenGLMetafileWriter::glAppendMaterial(OdRxObjectPtr pMaterial)
{
  flushPackager(PackageType_Null, 1);
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdUInt32), 1)
  writeChunk(OdOpenGLMetaRecType_Material);
  this->glAppendRxObject(pMaterial);
  m_uAffects |= OdOpenGLAffection_Materials;
}

void OdOpenGLMetafileWriter::glAppendUserEntry()
{
  flushPackager(PackageType_Null);
  writeChunk(OdOpenGLMetaRecType_UserEntry);
}

void OdOpenGLMetafileWriter::glAppendUserEntry(const void *pMem, OdUInt32 length)
{
  this->glAppendUserEntry();
  write(pMem, length);
}

void OdOpenGLMetafileWriter::glInitTexture(const void *dataSet, OdUInt32 sizeofDataSet, OdRxObject *pData,
                                           const void *subDataSet, OdUInt32 sizeofSubDataSet,
                                           const void *subDataSet2, OdUInt32 sizeofSubDataSet2)
{
  flushPackager(PackageType_Null);
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdUInt32), 1) // to max struct element
  writeChunk(OdOpenGLMetaRecType_InitTexture);
  write(dataSet, sizeofDataSet);
  if (!pData)
  {
    writeBool(false);
    this->glAppendArray(subDataSet, sizeofSubDataSet);
    writeBool(sizeofSubDataSet2 != 0);
    if (sizeofSubDataSet2)
      this->glAppendArray(subDataSet2, sizeofSubDataSet2);
  }
  else
  {
    writeBool(true);
    this->glAppendRxObject(pData);
  }
}

void OdOpenGLMetafileWriter::glUninitTexture()
{
  flushPackager(PackageType_Null);
  writeChunk(OdOpenGLMetaRecType_UninitTexture);
}

OdUInt32 OdOpenGLMetafileWriter::glAppendArray(const ColorTriplet *pPtr, OdUInt32 size, bool writeId)
{
  return glAppendArray((const void*)pPtr, size, writeId);
}

void OdOpenGLMetafileWriter::glHLRStencil(bool beginFlag)
{
  if ((beginFlag && prevState.bHLR != 1) ||
     (!beginFlag && prevState.bHLR != 0))
  {
    flushPackager(PackageType_Null);
    prevState.bHLR = (beginFlag) ? 1 : 0;
    if (m_prevChunk != OdOpenGLMetaRecType_HLRStencil)
    {
      writeChunk(OdOpenGLMetaRecType_HLRStencil);
      m_uAffects |= OdOpenGLAffection_Attributes;
    }
    else
      seek(1, false);
    writeUInt8((OdUInt8)prevState.bHLR);
  }
}

void OdOpenGLMetafileWriter::glHLRStencilToTraits(bool beginFlag, OdUInt32 trait, bool input)
{
  OdUInt8 mode = (beginFlag) ? 1 : 0;
  TraitsOptions &options = getTraitsOptions(trait, input);
  if ((options.bHLR != 0xFF || (isPackaging(trait) && input && hasGeometry(trait))) && options.bHLR != mode)
  {
    TraitsStorage<TraitsOptions> _storage(options);
    flushPackager(PackageType_Null);
  }
  options.bHLR = mode;
}

void OdOpenGLMetafileWriter::glUnsetHLRStencilToTraits(OdUInt32 trait, bool input)
{
  TraitsOptions &options = getTraitsOptions(trait, input);
  if (options.bHLR != 0xFF)
  {
    TraitsStorage<TraitsOptions> _storage(options);
    flushPackager(PackageType_Null);
  }
  options.bHLR = 0xFF;
}

void OdOpenGLMetafileWriter::glTtfText(OdUInt64 fontKeyA, OdUInt64 fontKeyB, const OdUInt32 *pText, OdUInt32 nText,
                                       const OdGeMatrix3d &baseTransform, const OdGeVector3d &stepDir)
{
  flushPackager(PackageType_Null);
  OD_OGL_STREAM_NORMALIZESTRICTALIGN(sizeof(OdUInt64), 1)
  writeChunk(OdOpenGLMetaRecType_TtfText);
  writeUInt64(fontKeyA);
  writeUInt64(fontKeyB);
  writeMatrix3d(baseTransform);
  writeVector3d(stepDir);
  writeUInt32(nText);
  writeUInt32Array(pText, nText);
  m_uAffects |= OdOpenGLAffection_Geometry;
}

void OdOpenGLMetafileWriter::startDelayMappingEntry(OdOpenGLDelayedMappingEntry *pEntry)
{
  m_delEntry.push_back(pEntry);
  pEntry->pContainer = containerIO()->container();
}

void OdOpenGLMetafileWriter::clearDelayMappingCache()
{
  m_delEntry.setPhysicalLength(0);
}

void OdOpenGLMetafileWriter::glResetTexCoords(const OdUInt32 ids[3], const OdGePoint2d coords[3])
{
  for (int x = 0; x < 3; x++)
  {
    TextureCoord &coord = m_packEntry[1].m_texArray.at(ids[x]);
    coord.s = coords[x].x;
    coord.t = coords[x].y;
  }
}

void OdOpenGLMetafileWriter::flushTriDelayCache(OdUInt32 nArrayId)
{
  for (OdUInt32 i = m_delEntry.size(); i > 0; i--)
  {
    if (m_delEntry.at(i - 1)->bFlushed)
      break;
    m_delEntry.at(i - 1)->m_texIds.append(nArrayId);
    m_delEntry.at(i - 1)->bFlushed = true;
  }
}

void OdOpenGLMetafileWriter::setupPackagerGrowOptions(OdInt32 newGrowSet, OdUInt32 nPackReserve)
{
  for (OdUInt8 nEnt = 0; nEnt < 2; nEnt++)
  {
    PackageEntry &entry = m_packEntry[nEnt];
    entry.m_vertArray.setGrowLength(newGrowSet);
    entry.m_clrArray.setGrowLength(newGrowSet);
    entry.m_normArray.setGrowLength(newGrowSet);
    entry.m_texArray.setGrowLength(newGrowSet);
    entry.m_selMarkArray.setGrowLength(newGrowSet);
    entry.m_indices.setGrowLength(newGrowSet);
  }
  m_pointEntry.m_vertArray.setGrowLength(newGrowSet);
  m_pointEntry.m_selMarkArray.setGrowLength(newGrowSet);
  m_nPackReserve = nPackReserve;
}

//
