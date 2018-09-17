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
#include "ExOpenGLMetafileReader.h"
#include "Gi/GiMaterialItem.h"
#include "SaveState.h"
#define ODGLLOCALGLDEFINITIONS_ASSUME_GL
#include "PseudoGLDefs.h" // Check that metafiles data compatible

//#define OD_OGL_TRACERECS

// Global arrays

GLint OdOpenGLMetafileReader::gDefArrayString[OdOpenGLMetaRecArrayType_NTypes] =
{
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  sizeof(GLdouble) * 3, // glVertexPointer
  OD_OGL_NCOLORCOMPONENTS, // glColorPointer
  sizeof(GLdouble) * 2, // glTexCoordPointer
  sizeof(GLdouble) * 3  // glNormalPointer
#else
  sizeof(GLfloat) * 3, // glVertexPointer
  OD_OGL_NCOLORCOMPONENTS, // glColorPointer
  sizeof(GLfloat) * 2, // glTexCoordPointer
  sizeof(GLfloat) * 3  // glNormalPointer
#endif
};

GLenum OdOpenGLMetafileReader::gGLArrayString[OdOpenGLMetaRecArrayType_NTypes] =
{
  GL_VERTEX_ARRAY,        // glVertexPointer
  GL_COLOR_ARRAY,         // glColorPointer
  GL_TEXTURE_COORD_ARRAY, // glTexCoordPointer
  GL_NORMAL_ARRAY         // glNormalPointer
};

#define OD_OGL_PTRWRAP(funcName) \
  static void funcName##Wrap(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) \
  { \
    ::funcName(size, type, stride, pointer); \
  }
OD_OGL_PTRWRAP(glVertexPointer);
OD_OGL_PTRWRAP(glColorPointer);
OD_OGL_PTRWRAP(glTexCoordPointer);
static void glNormalPointerWrap(GLint /*size*/, GLenum type, GLsizei stride, const GLvoid *pointer)
{
  ::glNormalPointer(type, stride, pointer);
}
#undef OD_OGL_PTRWRAP

OdOpenGLMetafileReader::glPointerProto OdOpenGLMetafileReader::gGLArrayProto[OdOpenGLMetaRecArrayType_NTypes] =
{
  ::glVertexPointerWrap,
  ::glColorPointerWrap,
  ::glTexCoordPointerWrap,
  ::glNormalPointerWrap
};

GLint OdOpenGLMetafileReader::gGLArrayNVals[OdOpenGLMetaRecArrayType_NTypes] =
{
  3, OD_OGL_NCOLORCOMPONENTS, 2, 3
};

GLenum OdOpenGLMetafileReader::gGLArrayType[OdOpenGLMetaRecArrayType_NTypes] =
{
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  GL_DOUBLE, GL_UNSIGNED_BYTE, GL_DOUBLE, GL_DOUBLE
#else
  GL_FLOAT, GL_UNSIGNED_BYTE, GL_FLOAT, GL_FLOAT
#endif
};

GLenum OdOpenGLMetafileReader::gGLAttributeString[OdOpenGLAttribute_NTypes] =
{
  GL_CULL_FACE,
  GL_BLEND,
  GL_LIGHTING,
  GL_TEXTURE_2D
};

//OdOpenGLMetafileReader::glReadFunc OdOpenGLMetafileReader::gReadFunc[OdOpenGLMetaRecType_NTypes] =
//{
//  &OdOpenGLMetafileReader::readRecType_Empty,
//  &OdOpenGLMetafileReader::readRecType_Enable,
//  &OdOpenGLMetafileReader::readRecType_Disable,
//  &OdOpenGLMetafileReader::readRecType_Color,
//  &OdOpenGLMetafileReader::readRecType_CullFace,
//  &OdOpenGLMetafileReader::readRecType_LStipple,
//  &OdOpenGLMetafileReader::readRecType_PStipple,
//  &OdOpenGLMetafileReader::readRecType_EnableArray,
//  &OdOpenGLMetafileReader::readRecType_DisableArray,
//  &OdOpenGLMetafileReader::readRecType_VPoint,
//  &OdOpenGLMetafileReader::readRecType_VLine,
//  &OdOpenGLMetafileReader::readRecType_IPoint,
//  &OdOpenGLMetafileReader::readRecType_ILine,
//  &OdOpenGLMetafileReader::readRecType_DrawArrays,
//  &OdOpenGLMetafileReader::readRecType_DrawElements,
//  &OdOpenGLMetafileReader::readRecType_EnableOpt,
//  &OdOpenGLMetafileReader::readRecType_DisableOpt
//};

// OdOpenGLMetafileReader::PrevState

void OdOpenGLMetafileReader::PrevState::setDefaults()
{
  //for (OdUInt32 i = 0; i < OdOpenGLAttribute_NTypes; i++)
  //  bOptAttrib[i] = (::glIsEnabled(gGLAttributeString[i])) ? true : false;
  { // #9643 optimization (loop was opened, some properties doesn't require direct initialization)
    bOptAttrib[OdOpenGLAttribute_CullFace] = false; // initially disabled
    bOptAttrib[OdOpenGLAttribute_Blend] = false; // initially disabled
    bOptAttrib[OdOpenGLAttribute_Lighting] = (::glIsEnabled(gGLAttributeString[OdOpenGLAttribute_Lighting])) ? true : false;
    bOptAttrib[OdOpenGLAttribute_Texture2D] = (::glIsEnabled(gGLAttributeString[OdOpenGLAttribute_Texture2D])) ? true : false;
    bNoColorModState = false;
  }
  GLint glInt;
  ::glGetIntegerv(GL_CULL_FACE_MODE, &glInt);
  eCullMode = (GLenum)glInt;
  pMaterial = (OdDbStub*)OdIntToPtr(-1);
}

void OdOpenGLMetafileReader::PrevState::restoreOption(OdOpenGLAttribute attrib)
{
  bOptAttrib[attrib] = (::glIsEnabled(gGLAttributeString[attrib])) ? true : false;
}

// OdOpenGLMetafileReader

#ifdef OD_OGL_ES
void oglEsDrawBuffer(GLint size, const GLvoid *pData, GLenum mode, GLsizei count);
#endif

inline bool glIsLineOrPtMode(GLenum mode)
{
  return mode < GL_TRIANGLES;
}
inline bool glIsPtMode(GLenum mode)
{
  return mode < GL_LINES;
}

void OdOpenGLMetafileReader::play(const OdBaseMetafileContainer *pContainer, bool bHighlighted, bool bCheckMark)
{
  //containerIO()->setContainer(const_cast<OdBaseMetafileContainer*>(pContainer)); // rewind
  const OdOpenGLFlatMetafileContainer *pDataCont = static_cast<const OdOpenGLFlatMetafileContainer*>(pContainer);
  OdUInt32 uSize = pDataCont->size();
  if (uSize == 0)
    return;
  const OdUInt8 *pMemPtr = pDataCont->memoryPtr();
  const OdUInt8 *pMemPtrReadFor = pMemPtr + uSize;
  InprocRegisters inprocCtx = m_inprocContext;
  SETBIT(inprocCtx.bFlags, kHighlightInit, bHighlighted);
#ifdef OD_OGL_TRACERECS
  ODA_TRACE0("Start . . .\n");
#endif
  while (pMemPtr < pMemPtrReadFor)
  {
    OdOpenGLMetaRecType recType = OD_OGL_RDR_READVALRETYPE(OdOpenGLMetaRecType, OdUInt8, pMemPtr);
    OD_OGL_RDR_SEEKINC(pMemPtr);
#ifdef OD_OGL_TRACERECS
    ODA_TRACE(L"Chunk: %X (%s)\n", recType, chunkDecode(recType));
#endif
#if 0
    ODA_ASSERT(rectype < OdOpenGLMetaRecType_NTypes);
    (this->*gReadFunc[recType])(pMemPtr);
#else
    // After several tests switch version is more fast than using of function pointers
    switch (recType)
    {
      case OdOpenGLMetaRecType_EnableOpt:
        {
          OdUInt8 mode = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
          OD_OGL_RDR_SEEKINC(pMemPtr);
          if (!prevState.bOptAttrib[mode])
          {
            ::glEnable(gGLAttributeString[mode]);
            prevState.bOptAttrib[mode] = true;
          }
        }
      break;
      case OdOpenGLMetaRecType_DisableOpt:
        {
          OdUInt8 mode = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
          OD_OGL_RDR_SEEKINC(pMemPtr);
          if (prevState.bOptAttrib[mode])
          {
            ::glDisable(gGLAttributeString[mode]);
            prevState.bOptAttrib[mode] = false;
          }
        }
      break;
      case OdOpenGLMetaRecType_Color:
        if (prevState.m_pColorConverter && (prevState.m_bProcessBackgroundColors || !prevState.bNoColorModState))
        {
          const ODCOLORREF color = prevState.m_pColorConverter->convert(ODRGBA(pMemPtr[0], pMemPtr[1], pMemPtr[2], pMemPtr[3]));
          ::glColor4ub(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color), ODGETALPHA(color));
        }
        else
          ::glColor4ubv((const GLubyte*)pMemPtr);
        OD_OGL_RDR_SEEKFWD(pMemPtr, 4);
        //{
        //  GLubyte clrbits[4];
        //  readGLubyteArray(clrbits, 4);
        //  ::glColor4ubv(clrbits);
        //}
      break;
      case OdOpenGLMetaRecType_EnableArray:
        {
          OdOpenGLMetaRecArrayType type = OD_OGL_RDR_READVALRETYPE(OdOpenGLMetaRecArrayType, OdUInt8, pMemPtr);
          //GLint strideSize = 0;
          //OdUInt8 strideAvail = OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, 1);
          //if (strideAvail)
          //{
          //  strideSize = OD_OGL_RDR_READVALOFFSETRETYPE(GLint, OdUInt8, pMemPtr, 2);
          //  OD_OGL_RDR_SEEKFWD(pMemPtr, 3);
          //}
          //else
          //{
            GLint strideSize = gDefArrayString[type];
            //OD_OGL_RDR_SEEKFWD(pMemPtr, 2);
            OD_OGL_RDR_SEEKINC(pMemPtr);
          //}
          //OdUInt32 lSkip = OD_OGL_RDR_READVAL(OdUInt32, pMemPtr);
          OdUInt32 nArray = OD_OGL_RDR_READVAL(OdUInt32, pMemPtr);
          gGLArrayProto[type](gGLArrayNVals[type], gGLArrayType[type],
                              strideSize, /*pMemPtr + sizeof(OdUInt32)*/
                              pDataCont->m_ArrayElements.getPtr()[nArray].m_pData);
          ::glEnableClientState(gGLArrayString[type]);
          //OD_OGL_RDR_SEEKFWD(pMemPtr, lSkip + sizeof(OdUInt32));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_DisableArray:
        ::glDisableClientState(gGLArrayString[OD_OGL_RDR_READVAL(OdUInt8, pMemPtr)]);
        OD_OGL_RDR_SEEKINC(pMemPtr);
      break;
      case OdOpenGLMetaRecType_DrawArrays:
        {
          if (GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLint) << 1) + 1);
            break;
          }
          if (!inprocCtx.pMarks)
          {
            if (!GETBIT(inprocCtx.bFlags, kLineweightProc | kLinesAntiAlias) || !m_pCallback)
              glSafeDrawArrays(OD_OGL_RDR_READVALRETYPE(GLenum, OdUInt8, pMemPtr), OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1), OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1 + sizeof(GLint)));
            else
            {
              if (GETBIT(inprocCtx.bFlags, kLineweightProc))
                drawArrayLwd(OD_OGL_RDR_READVALRETYPE(GLenum, OdUInt8, pMemPtr), OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1), OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1 + sizeof(GLint)), bHighlighted);
              else
                drawArrayAntiAlias(OD_OGL_RDR_READVALRETYPE(GLenum, OdUInt8, pMemPtr), OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1), OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1 + sizeof(GLint)));
            }
          }
          else
          {
#define OGL_CHECK_MARK(getMarker) \
          bool bNewState = m_pCallback->metafileCheckMarker(getMarker, bHighlighted); \
          if (bNewState != bHighlighted) \
          { \
            bHighlighted = bNewState; \
            m_pCallback->metafileLineStipple(OdOpenGLLinePatt::kLnPattSel, (bHighlighted) ? 1 : 0); \
            m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattSel, (bHighlighted) ? 1 : 0); \
          }
            GLenum mode = OD_OGL_RDR_READVALRETYPE(GLenum, OdUInt8, pMemPtr);
            GLint first = OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1);
            GLint count = OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1 + sizeof(GLint)) + first;
            GLint ctDiff;
            do
            {
              if (inprocCtx.nMarks)
                ctDiff = inprocCtx.pMarks->uFrom - first;
              else
                ctDiff = count - first;
              if (ctDiff)
              {
                if (!GETBIT(inprocCtx.bFlags, kLineweightProc | kLinesAntiAlias) || !m_pCallback)
                  glSafeDrawArrays(mode, first, ctDiff);
                else
                {
                  if (GETBIT(inprocCtx.bFlags, kLineweightProc))
                    drawArrayLwd(mode, first, ctDiff, bHighlighted);
                  else
                    drawArrayAntiAlias(mode, first, ctDiff);
                }
              }
              if (inprocCtx.nMarks)
              {
                OGL_CHECK_MARK(inprocCtx.pMarks->gsMark)
                first += ctDiff;
                inprocCtx.pMarks++;
              }
              inprocCtx.nMarks--;
            }
            while (inprocCtx.nMarks != (OdUInt32)-1);
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLint) << 1) + 1);
        }
      break;
      case OdOpenGLMetaRecType_DrawElements:
        {
          if (GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(GLint) + 1 + sizeof(OdUInt32));
            break;
          }
#ifndef OD_OGL_ES
#define OD_GL_INDEX_TYPE GL_UNSIGNED_INT
#else
#define OD_GL_INDEX_TYPE GL_UNSIGNED_SHORT
#endif
          GLenum mode = OD_OGL_RDR_READVALRETYPE(GLenum, OdUInt8, pMemPtr);
          GLint count = OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1);
          if (!inprocCtx.pMarks)
          {
            if (!GETBIT(inprocCtx.bFlags, kLineweightProc | kLinesAntiAlias) || !m_pCallback)
              ::glDrawElements(mode, count, OD_GL_INDEX_TYPE, pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(GLint))].m_pData);
            else
            {
              if (GETBIT(inprocCtx.bFlags, kLineweightProc))
                drawElementsLwd(mode, count, OD_GL_INDEX_TYPE, bHighlighted, pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(GLint))].m_pData);
              else
                drawElementsAntiAlias(mode, count, OD_GL_INDEX_TYPE, pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(GLint))].m_pData);
            }
          }
          else
          {
            GLint first = 0;
#ifndef OD_OGL_ES
            GLint *pBufPtr = (GLint*)pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(GLint))].m_pData;
#else
            GLushort *pBufPtr = (GLushort*)pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(GLint))].m_pData;
#endif
            GLint ctDiff;
            do
            {
              if (inprocCtx.nMarks)
                ctDiff = inprocCtx.pMarks->uFrom - first;
              else
                ctDiff = count - first;
              if (ctDiff)
              {
                if (!GETBIT(inprocCtx.bFlags, kLineweightProc | kLinesAntiAlias) || !m_pCallback)
                  ::glDrawElements(mode, ctDiff, OD_GL_INDEX_TYPE, pBufPtr + first);
                else
                {
                  if (GETBIT(inprocCtx.bFlags, kLineweightProc))
                    drawElementsLwd(mode, ctDiff, OD_GL_INDEX_TYPE, bHighlighted, pBufPtr + first);
                  else
                    drawElementsAntiAlias(mode, ctDiff, OD_GL_INDEX_TYPE, pBufPtr + first);
                }
              }
              if (inprocCtx.nMarks)
              {
                OGL_CHECK_MARK(inprocCtx.pMarks->gsMark)
                first += ctDiff;
                inprocCtx.pMarks++;
              }
              inprocCtx.nMarks--;
            }
            while (inprocCtx.nMarks != (OdUInt32)-1);
          }
#undef OD_GL_INDEX_TYPE
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(GLint) + 1 + sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_CullFace:
        {
          GLenum mode = 0;
          switch (OD_OGL_RDR_READVALRETYPE(OdOpenGLFaceCulling, OdUInt8, pMemPtr))
          {
            case OdOpenGLCullFace_Back:  mode = GL_BACK;  break;
            case OdOpenGLCullFace_Front: mode = GL_FRONT; break;
            default: break;
          }
          if (mode && (mode != prevState.eCullMode))
          {
            ::glCullFace(mode);
            prevState.eCullMode = mode;
          }
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdOpenGLMetaRecType_LStipple:
        {
          OdOpenGLLinePatt::Entry entry = OD_OGL_RDR_READVALRETYPE(OdOpenGLLinePatt::Entry, OdUInt8, pMemPtr);
          if (entry == OdOpenGLLinePatt::kLnPattAll)
          {
            if (m_pCallback)
            {
              // Static definitions for optimization purposes (minimize count of callbacks)
#ifndef ODA_BIGENDIAN
              static const OdUInt16 gOptAllBits_l = 0xFFFF;
              static const OdUInt16 gOptAllBitsSel_l = 0x0100;
#else
              static const OdUInt16 gOptAllBits_l = 0xFFFF;
              static const OdUInt16 gOptAllBitsSel_l = 0x0001;
#endif
              if (!bHighlighted)
              {
                if ((prevState.pLnPatt->data.summ & gOptAllBits_l) != 0)
                  m_pCallback->metafileLineStipple(entry, 0);
              }
              else
              {
                if ((prevState.pLnPatt->data.summ & gOptAllBits_l) != gOptAllBitsSel_l)
                {
                  if (prevState.pLnPatt->data.named.reg_linetype)
                    m_pCallback->metafileLineStipple(OdOpenGLLinePatt::kLnPattLinetype, 0);
                  if (!prevState.pLnPatt->data.named.reg_sel)
                    m_pCallback->metafileLineStipple(OdOpenGLLinePatt::kLnPattSel, 1);
                }
              }
            }
            OD_OGL_RDR_SEEKINC(pMemPtr);
          }
          else
          {
            if (m_pCallback)
            {
              if (bHighlighted && entry == OdOpenGLLinePatt::kLnPattSel)
              {
                if (!prevState.pLnPatt->data.named.reg_sel)
                  m_pCallback->metafileLineStipple(entry, 1);
              }
              else
              {
                OdUInt8 data = OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, sizeof(OdUInt8));
                if (prevState.pLnPatt->data.reg[entry] != data)
                  m_pCallback->metafileLineStipple(entry, data);
              }
            }
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 2);
          }
        }
      break;
      case OdOpenGLMetaRecType_PStipple:
        {
          OdOpenGLFillPatt::Entry entry = OD_OGL_RDR_READVALRETYPE(OdOpenGLFillPatt::Entry, OdUInt8, pMemPtr);
          if (entry == OdOpenGLFillPatt::kFillPattAll)
          {
            if (m_pCallback)
            {
              // Static definitions for optimization purposes (minimize count of callbacks)
#ifndef ODA_BIGENDIAN
              static const OdUInt32 gOptAllBits_p = 0xFFFF00FF;
              static const OdUInt32 gOptAllBitsSel_p = 0x01000000;
#else
              static const OdUInt32 gOptAllBits_p = 0xFF00FFFF;
              static const OdUInt32 gOptAllBitsSel_p = 0x00000001;
#endif
              if (!bHighlighted)
              {
                if ((prevState.pFillPatt->data.summ & gOptAllBits_p) != 0)
                  m_pCallback->metafileFillStipple(entry, 0);
              }
              else
              {
                if ((prevState.pFillPatt->data.summ & gOptAllBits_p) != gOptAllBitsSel_p)
                {
                  if (!prevState.pFillPatt->data.named.reg_sel)
                  {
                    m_pCallback->metafileFillStipple(entry, 0);
                    m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattSel, 1);
                  }
                  else
                  {
                    if (prevState.pFillPatt->data.named.reg_transparency)
                      m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattTransparency, 0);
                    if (prevState.pFillPatt->data.named.reg_fillstyle)
                      m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattFillstyle, 0);
                  }
                }
              }
            }
            OD_OGL_RDR_SEEKINC(pMemPtr);
          }
          else
          {
            if (m_pCallback)
            {
              if (bHighlighted && entry == OdOpenGLFillPatt::kFillPattSel)
              {
                if (!prevState.pFillPatt->data.named.reg_sel)
                  m_pCallback->metafileFillStipple(entry, 1);
              }
              else
              {
                OdUInt8 data = OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, sizeof(OdUInt8));
                if (prevState.pFillPatt->data.reg[entry] != data)
                  m_pCallback->metafileFillStipple(entry, data);
              }
            }
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 2);
          }
        }
      break;
      case OdOpenGLMetaRecType_VPoint:
        if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
        {
          if (!GETBIT(inprocCtx.bFlags, kLineweightProc) || !m_pCallback)
          {
#ifndef OD_OGL_ES
            ::glBegin(GL_POINTS);
#ifndef OD_OGL_ENABLEFLOATCONVERSION
            ::glVertex3dv(OD_OGL_RDR_GETPTRNATIVE(const GLdouble, pMemPtr));
#else
            ::glVertex3fv(OD_OGL_RDR_GETPTRNATIVE(const GLfloat, pMemPtr));
#endif
            ::glEnd();
#else
            // This works faster than pack metafile in regular mode
            ::oglEsDrawBuffer(3, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr), GL_POINTS, 1);
#endif
          }
          else
          {
            drawArrayLwd(GL_POINTS, 0, 1, bHighlighted, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr));
          }
        }
#ifndef OD_OGL_ENABLEFLOATCONVERSION
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGePoint3d));
#else
        OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLfloat) << 1) + sizeof(GLfloat));
#endif
      break;
      case OdOpenGLMetaRecType_VLine:
        if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
        {
          if (!GETBIT(inprocCtx.bFlags, kLineweightProc | kLinesAntiAlias) || !m_pCallback)
          {
#ifndef OD_OGL_ES
            ::glBegin(GL_LINES);
#ifndef OD_OGL_ENABLEFLOATCONVERSION
            ::glVertex3dv(OD_OGL_RDR_GETPTRNATIVE(const GLdouble, pMemPtr));
            ::glVertex3dv(OD_OGL_RDR_GETPTRNATIVEOFFSET(const GLdouble, pMemPtr, sizeof(OdGePoint3d)));
#else
            ::glVertex3fv(OD_OGL_RDR_GETPTRNATIVE(const GLfloat, pMemPtr));
            ::glVertex3fv(OD_OGL_RDR_GETPTRNATIVEOFFSET(const GLfloat, pMemPtr, (sizeof(GLfloat) << 1) + sizeof(GLfloat)));
#endif
            ::glEnd();
#else
            // This works faster than pack metafile in regular mode
            ::oglEsDrawBuffer(3, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr), GL_LINES, 2);
#endif
          }
          else
          {
            if (GETBIT(inprocCtx.bFlags, kLineweightProc))
              drawArrayLwd(GL_LINES, 0, 2, bHighlighted, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr));
            else
              drawArrayAntiAlias(GL_LINES, 0, 2, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr));
          }
        }
#ifndef OD_OGL_ENABLEFLOATCONVERSION
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGePoint3d) << 1);
#else
        OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLfloat) << 2) + (sizeof(GLfloat) << 1));
#endif
      break;
      case OdOpenGLMetaRecType_IPoint:
        if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
        {
          if (!GETBIT(inprocCtx.bFlags, kLineweightProc) || !m_pCallback)
          {
#ifndef OD_OGL_ES
            ::glBegin(GL_POINTS);
            ::glArrayElement(OD_OGL_RDR_READVAL(OdInt32, pMemPtr));
            ::glEnd();
#else
            GLushort arrayVals = OD_OGL_RDR_READVALRETYPE(GLushort, OdInt32, pMemPtr);
            ::glDrawElements(GL_POINTS, 1, GL_UNSIGNED_SHORT, &arrayVals);
#endif
          }
          else
          {
#ifndef OD_OGL_ES
            drawElementsLwd(GL_POINTS, 1, GL_UNSIGNED_INT, bHighlighted, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr));
#else
            GLushort arrayVals = OD_OGL_RDR_READVALRETYPE(GLushort, OdInt32, pMemPtr);
            drawElementsLwd(GL_POINTS, 1, GL_UNSIGNED_SHORT, bHighlighted, &arrayVals);
#endif
          }
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32));
      break;
      case OdOpenGLMetaRecType_ILine:
        if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
        {
          if (!GETBIT(inprocCtx.bFlags, kLineweightProc | kLinesAntiAlias) || !m_pCallback)
          {
#ifndef OD_OGL_ES
            ::glBegin(GL_LINES);
            ::glArrayElement(OD_OGL_RDR_READVAL(OdInt32, pMemPtr));
            ::glArrayElement(OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, sizeof(OdInt32)));
            ::glEnd();
#else
            GLushort arrayVals[2] = { OD_OGL_RDR_READVALRETYPE(GLushort, OdInt32, pMemPtr),
                                      OD_OGL_RDR_READVALOFFSETRETYPE(GLushort, OdInt32, pMemPtr, sizeof(OdInt32)) };
            ::glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, arrayVals);
#endif
          }
          else
          {
            if (GETBIT(inprocCtx.bFlags, kLineweightProc))
            {
#ifndef OD_OGL_ES
              drawElementsLwd(GL_LINES, 2, GL_UNSIGNED_INT, bHighlighted, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr));
#else
              GLushort arrayVals[2] = { OD_OGL_RDR_READVALRETYPE(GLushort, OdInt32, pMemPtr),
                                        OD_OGL_RDR_READVALOFFSETRETYPE(GLushort, OdInt32, pMemPtr, sizeof(OdInt32)) };
              drawElementsLwd(GL_LINES, 2, GL_UNSIGNED_SHORT, bHighlighted, arrayVals);
#endif
            }
            else
            {
#ifndef OD_OGL_ES
              drawElementsAntiAlias(GL_LINES, 2, GL_UNSIGNED_INT, OD_OGL_RDR_GETPTRNATIVE(const GLvoid, pMemPtr));
#else
              GLushort arrayVals[2] = { OD_OGL_RDR_READVALRETYPE(GLushort, OdInt32, pMemPtr),
                                        OD_OGL_RDR_READVALOFFSETRETYPE(GLushort, OdInt32, pMemPtr, sizeof(OdInt32)) };
              drawElementsAntiAlias(GL_LINES, 2, GL_UNSIGNED_SHORT, arrayVals);
#endif
            }
          }
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) * 2);
      break;
      case OdOpenGLMetaRecType_HLRStencil:
        if (OD_OGL_RDR_READVAL(OdUInt8, pMemPtr))
        {
          // Begin HLR faces
#ifndef OD_OGL_ES
          ::glPushAttrib(GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT);
#else
          if (m_pCallback)
            m_pCallback->metafilePushStencilAttrib();
#endif
          ::glStencilFunc(GL_LEQUAL, 1, 0xFFFF);
          ::glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
          if (bHighlighted && m_pCallback)
            m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattSel, 0);
        }
        else
        {
          // End HLR faces
#ifndef OD_OGL_ES
          ::glPopAttrib();
#else
          if (m_pCallback)
            m_pCallback->metafilePopStencilAttrib();
#endif
          if (bHighlighted && m_pCallback)
            m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattSel, 1);
        }
        OD_OGL_RDR_SEEKINC(pMemPtr);
      break;
      case OdOpenGLMetaRecType_EnableShading:
        if (OD_OGL_RDR_READVALRETYPE(OdOpenGLShading, OdUInt8, pMemPtr) == OdOpenGLShading_NoColorOverride)
          prevState.bNoColorModState = true;
        OD_OGL_RDR_SEEKINC(pMemPtr);
      break;
      case OdOpenGLMetaRecType_DisableShading:
        if (OD_OGL_RDR_READVALRETYPE(OdOpenGLShading, OdUInt8, pMemPtr) == OdOpenGLShading_NoColorOverride)
          prevState.bNoColorModState = false;
        OD_OGL_RDR_SEEKINC(pMemPtr);
      break;
      case OdOpenGLMetaRecType_Material:
        {
          OdUInt32 id = OD_OGL_RDR_READVAL(OdUInt32, pMemPtr);
          if (m_pCallback)
          {
            if (id != 0xFFFFFFFF)
            {
              const OdRxObjectPtr &pObj = pDataCont->m_RxObjects.at(id);
              const OdDbStub *pMtl = static_cast<const OdGiMaterialItem*>(pObj.get())->materialId();
              if (prevState.pMaterial != pMtl)
              {
                prevState.pMaterial = pMtl;
                m_pCallback->metafileProcessMaterial(const_cast<OdRxObject*>(pObj.get()));
              }
            }
            else
            {
              if (prevState.pMaterial != NULL)
              {
                prevState.pMaterial = NULL;
                m_pCallback->metafileProcessMaterial(NULL);
              }
            }
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_UserEntry:
        if (m_pCallback)
          m_pCallback->metafileProcessUserEntry(pDataCont, pMemPtr);
      break;
      case OdOpenGLMetaRecType_InitTexture:
        {
          OdOpenGLMetafileReaderCallback::RasterImageEntrySet dataSet;
          ::memcpy(&dataSet, pMemPtr, sizeof(OdOpenGLMetafileReaderCallback::RasterImageEntry));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdOpenGLMetafileReaderCallback::RasterImageEntry));
          bool bRx = OD_OGL_RDR_READVAL(bool, pMemPtr);
          OdRxObjectPtr pData;
          dataSet.bufSize = 0;
          dataSet.pTexture = NULL;
          dataSet.pPalTexture = NULL;
          if (!bRx)
          {
            dataSet.pTexture = pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(bool))].m_pData;
            if (OD_OGL_RDR_READVALOFFSET(bool, pMemPtr, sizeof(bool) + sizeof(OdUInt32)))
            {
              dataSet.pPalTexture = pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(bool) * 2 + sizeof(OdUInt32))].m_pData;
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
            }
            OD_OGL_RDR_SEEKINC(pMemPtr);
          }
          else
            pData = pDataCont->m_RxObjects.at(OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(bool)));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32) + sizeof(bool));
          if (m_pCallback)
          {
            m_pCallback->metafileProcessInitTexture(dataSet, pData);
#ifndef OD_OGL_ALTERNATIVEHLT
            if (bHighlighted && !prevState.bForceAlternativeHlt)
              m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattSel, 0);
#endif
          }
        }
      break;
      case OdOpenGLMetaRecType_UninitTexture:
        if (m_pCallback)
        {
          m_pCallback->metafileProcessUninitTexture();
#ifndef OD_OGL_ALTERNATIVEHLT
          if (bHighlighted && !prevState.bForceAlternativeHlt)
            m_pCallback->metafileFillStipple(OdOpenGLFillPatt::kFillPattSel, 1);
#endif
        }
      break;
      case OdOpenGLMetaRecType_SelectionMarker:
        if (bCheckMark && m_pCallback)
        {
          OGL_CHECK_MARK(OD_OGL_RDR_READVAL(OdOpenGLGsMarker, pMemPtr))
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdOpenGLGsMarker));
      break;
      case OdOpenGLMetaRecType_EnableMarkerArray:
        {
          if (bCheckMark)
          {
            OdUInt32 nArray = OD_OGL_RDR_READVAL(OdUInt32, pMemPtr);
            inprocCtx.pMarks = (OdOpenGLSelMark*)pDataCont->m_ArrayElements.getPtr()[nArray].m_pData;
            inprocCtx.nMarks = pDataCont->m_ArrayElements.getPtr()[nArray].m_uSize / sizeof(OdOpenGLSelMark);
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_DisableMarkerArray:
        inprocCtx.pMarks = NULL;
      break;
      case OdOpenGLMetaRecType_SelectionFlags:
        {
          const OdUInt8 selFlags = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
          OD_OGL_RDR_SEEKINC(pMemPtr);
          SETBIT(inprocCtx.bFlags, kSuppressGeom, (!GETBIT(inprocCtx.bFlags, kHighlightInit) && GETBIT(selFlags, OdOpenGLSelFlag_DontDisplayUnhighlighted)) ||
                                                   (GETBIT(inprocCtx.bFlags, kHighlightInit) && GETBIT(selFlags, OdOpenGLSelFlag_DontDisplayHighlighted)));
        }
      break;
      case OdOpenGLMetaRecType_Lineweight:
        {
          OdOpenGLLwdSetting lwdSet(OD_OGL_RDR_READVALRETYPE(OdOpenGLLwdSetting::LwdType, OdUInt8, pMemPtr));
          switch (lwdSet.m_type)
          {
            case OdOpenGLLwdSetting::kDatabase:
              lwdSet.m_val.m_lwLineWeight = OD_OGL_RDR_READVALOFFSETRETYPE(OdDb::LineWeight, OdInt16, pMemPtr, sizeof(OdUInt8));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(OdInt16));
            break;
            case OdOpenGLLwdSetting::kPlotStyle:
            case OdOpenGLLwdSetting::kAbsolute:
              lwdSet.m_val.m_dLineWeight = OD_OGL_RDR_READVALOFFSET(double, pMemPtr, sizeof(OdUInt8));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(double));
            break;
            default:
              lwdSet.m_val.m_pixLineWeight = OD_OGL_RDR_READVALOFFSETRETYPE(OdInt32, OdInt16, pMemPtr, sizeof(OdUInt8));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(OdInt16));
          }
          if (m_pCallback)
            SETBIT(inprocCtx.bFlags, kLineweightProc, m_pCallback->metafileProcessLineweight(lwdSet));
        }
      break;
      case OdOpenGLMetaRecType_Linestyle:
        {
          const OdUInt8 defStyle = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
          if (defStyle)
          {
            if (m_pCallback)
              m_pCallback->metafileLineweightStyle(true);
            OD_OGL_RDR_SEEKINC(pMemPtr);
          }
          else
          {
            if (m_pCallback)
              m_pCallback->metafileLineweightStyle(false,
              OD_OGL_RDR_READVALOFFSETRETYPE(OdPs::LineEndStyle, OdUInt8, pMemPtr, sizeof(OdUInt8)),
              OD_OGL_RDR_READVALOFFSETRETYPE(OdPs::LineJoinStyle, OdUInt8, pMemPtr, sizeof(OdUInt8) * 2));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 3);
          }
        }
      break;
      case OdOpenGLMetaRecType_TtfText:
        {
          if (m_pCallback && !GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OdUInt64 fontKeyA = OD_OGL_RDR_READVAL(OdUInt64, pMemPtr);
            OdUInt64 fontKeyB = OD_OGL_RDR_READVALOFFSET(OdUInt64, pMemPtr, sizeof(OdUInt64));
            const OdGeMatrix3d *pMtx = (const OdGeMatrix3d*)(pMemPtr + sizeof(OdUInt64) * 2);
            const OdGeVector3d *pVec = (const OdGeVector3d*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d));
            OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
            const OdUInt32 *pText = (const OdUInt32*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32));
            OdSaveState<InprocRegisters> _ss(m_inprocContext, inprocCtx);
            m_pCallback->metafileTtfText(fontKeyA, fontKeyB, pText, nText, *pMtx, *pVec);
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32) + sizeof(OdUInt32) * nText);
          }
          else
          {
            OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32) + sizeof(OdUInt32) * nText);
          }
        }
      break;
      case OdOpenGLMetaRecType_Empty:
        // Skip
      break;
      default:
        ODA_FAIL();
    }
#endif
  }
#ifdef OD_OGL_TRACERECS
  ODA_TRACE0("End . . .\n");
#endif
}

class OdOpenGLMfSelectorPresets
{
  OdGiSubEntityTraits *m_pTraits;
  OdGiFillType m_prevFillType;
  OdUInt32 m_prevDrawFlags;
  public:
    OdOpenGLMfSelectorPresets(OdGiConveyorContext *pCtx)
    {
      if (pCtx)
      {
        m_pTraits = &pCtx->subEntityTraits();
        if (m_pTraits)
        {
          // Store previous
          m_prevFillType = m_pTraits->fillType();
          m_prevDrawFlags = m_pTraits->drawFlags();
          // Modify
          if (m_prevFillType != kOdGiFillAlways)
            m_pTraits->setFillType(kOdGiFillAlways);
          if (!GETBIT(m_prevDrawFlags, OdGiSubEntityTraits::kDrawSolidFill))
            m_pTraits->setDrawFlags(m_prevDrawFlags | OdGiSubEntityTraits::kDrawSolidFill);
        }
      }
    }
    ~OdOpenGLMfSelectorPresets()
    {
      if (m_pTraits)
      {
        if (m_prevFillType != kOdGiFillAlways)
          m_pTraits->setFillType(m_prevFillType);
        if (!GETBIT(m_prevDrawFlags, OdGiSubEntityTraits::kDrawSolidFill))
          m_pTraits->setDrawFlags(m_prevDrawFlags);
      }
    }
};

void OdOpenGLMetafileReader::play4Select(const OdBaseMetafileContainer *pContainer, OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx, bool bCheckMark)
{
  const OdOpenGLFlatMetafileContainer *pDataCont = static_cast<const OdOpenGLFlatMetafileContainer*>(pContainer);
  OdUInt32 uSize = pDataCont->size();
  if (uSize == 0)
    return;
  OdOpenGLMfSelectorPresets _mfsp(pCtx);
  const OdUInt8 *pMemPtr = pDataCont->memoryPtr();
  const OdUInt8 *pMemPtrReadFor = pMemPtr + uSize;
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  typedef GLdouble vtxType;
#else
  typedef GLfloat vtxType;
#endif
  typedef vtxType vtxTypeArr[3];
  const vtxTypeArr *pVtxData = NULL;
  InprocRegisters inprocCtx = m_inprocContext;
  OdGePoint3d vertices[4];
  while (pMemPtr < pMemPtrReadFor)
  {
    OdOpenGLMetaRecType recType = OD_OGL_RDR_READVALRETYPE(OdOpenGLMetaRecType, OdUInt8, pMemPtr);
    OD_OGL_RDR_SEEKINC(pMemPtr);
    switch (recType)
    {
      case OdOpenGLMetaRecType_EnableOpt:
        {
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdOpenGLMetaRecType_DisableOpt:
        {
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdOpenGLMetaRecType_Color:
        {
          OD_OGL_RDR_SEEKFWD(pMemPtr, 4);
        }
      break;
      case OdOpenGLMetaRecType_EnableArray:
        {
          OdOpenGLMetaRecArrayType type = OD_OGL_RDR_READVALRETYPE(OdOpenGLMetaRecArrayType, OdUInt8, pMemPtr);
          OD_OGL_RDR_SEEKINC(pMemPtr);
          if (type == OdOpenGLMetaRecArrayType_Vertex)
          {
            pVtxData = (vtxTypeArr*)pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVAL(OdUInt32, pMemPtr)].m_pData;
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_DisableArray:
        {
          OdOpenGLMetaRecArrayType type = OD_OGL_RDR_READVALRETYPE(OdOpenGLMetaRecArrayType, OdUInt8, pMemPtr);
          if (type == OdOpenGLMetaRecArrayType_Vertex)
          {
            pVtxData = NULL;
          }
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdOpenGLMetaRecType_DrawArrays:
        {
          if (GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLint) << 1) + 1);
            break;
          }
          GLenum mode = OD_OGL_RDR_READVALRETYPE(GLenum, OdUInt8, pMemPtr);
          GLint first = OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1);
          GLint count = OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1 + sizeof(GLint)) + first;
          GLint ctr;
          GLint ctDiff;
          do
          {
            if (inprocCtx.nMarks)
              ctDiff = inprocCtx.pMarks->uFrom - first;
            else
              ctDiff = count - first;
            if (ctDiff)
            {
              switch (mode)
              {
                case GL_POINTS:
                {
                  for (ctr = 0; ctr < ctDiff; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + first + ctr;
                    vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[1] = vertices[0];
                    pGeom->polylineProc(2, vertices);
                  }
                }
                break;
                case GL_LINES:
                {
                  for (ctr = 0; ctr < ctDiff >> 1; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + first + (ctr << 1);
                    vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[1].set(pData[1][0], pData[1][1], pData[1][2]);
                    pGeom->polylineProc(2, vertices);
                  }
                }
                break;
                case GL_LINE_LOOP:
                {
                  const vtxTypeArr *pData1 = pVtxData + first, *pData2 = pVtxData + first + ctDiff - 1;
                  vertices[0].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                  vertices[1].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                  pGeom->polylineProc(2, vertices);
                }
                case GL_LINE_STRIP:
                {
                  for (ctr = 0; ctr < ctDiff - 1; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + first + ctr;
                    vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[1].set(pData[1][0], pData[1][1], pData[1][2]);
                    pGeom->polylineProc(2, vertices);
                  }
                }
                break;
                case GL_TRIANGLES:
                  for (ctr = 0; ctr < ctDiff / 3; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + first + ctr * 3;
                    vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[1].set(pData[1][0], pData[1][1], pData[1][2]);
                    vertices[2].set(pData[2][0], pData[2][1], pData[2][2]);
                    pGeom->polygonProc(3, vertices);
                  }
                break;
                case GL_TRIANGLE_STRIP:
                  for (ctr = 0; ctr < ctDiff - 2; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + first + ctr;
                    if ((ctr & 1) == 1)
                    {
                      vertices[0].set(pData[1][0], pData[1][1], pData[1][2]);
                      vertices[1].set(pData[0][0], pData[0][1], pData[0][2]);
                      vertices[2].set(pData[2][0], pData[2][1], pData[2][2]);
                    }
                    else
                    {
                      vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                      vertices[1].set(pData[1][0], pData[1][1], pData[1][2]);
                      vertices[2].set(pData[2][0], pData[2][1], pData[2][2]);
                    }
                    pGeom->polygonProc(3, vertices);
                  }
                break;
                case GL_TRIANGLE_FAN:
#ifndef OD_OGL_ES // Prevent redefinition
                case GL_POLYGON:
#endif
                  vertices[0].set(pVtxData[first][0], pVtxData[first][1], pVtxData[first][2]);
                  for (ctr = 1; ctr < ctDiff - 1; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + first + ctr;
                    vertices[1].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[2].set(pData[1][0], pData[1][1], pData[1][2]);
                    pGeom->polygonProc(3, vertices);
                  }
                break;
#ifndef OD_OGL_ES
                case GL_QUADS:
                  for (ctr = 0; ctr < ctDiff >> 2; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + first + (ctr << 2);
                    vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[1].set(pData[1][0], pData[1][1], pData[1][2]);
                    vertices[2].set(pData[2][0], pData[2][1], pData[2][2]);
                    vertices[3].set(pData[3][0], pData[3][1], pData[3][2]);
                    pGeom->polygonProc(4, vertices);
                  }
                break;
                case GL_QUAD_STRIP:
                  for (ctr = 0; ctr < ctDiff - 2; ctr += 2)
                  {
                    const vtxTypeArr *pData = pVtxData + first + ctr;
                    vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[1].set(pData[1][0], pData[1][1], pData[1][2]);
                    vertices[2].set(pData[3][0], pData[3][1], pData[3][2]);
                    vertices[3].set(pData[2][0], pData[2][1], pData[2][2]);
                    pGeom->polygonProc(4, vertices);
                  }
                break;
#endif
                default:
                  ODA_FAIL();
              }
            }
            if (inprocCtx.nMarks)
            {
              if (pCtx->regenAbort())
                return;
              if (m_pCallback)
                m_pCallback->metafileSelectMarker(inprocCtx.pMarks->gsMark);
              first += ctDiff;
              inprocCtx.pMarks++;
            }
            inprocCtx.nMarks--;
          }
          while (inprocCtx.nMarks != (OdUInt32)-1);
          inprocCtx.nMarks = 0;
          OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLint) << 1) + 1);
        }
      break;
      case OdOpenGLMetaRecType_DrawElements:
        {
          if (GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(GLint) + 1 + sizeof(OdUInt32));
            break;
          }
          GLenum mode = OD_OGL_RDR_READVALRETYPE(GLenum, OdUInt8, pMemPtr);
          GLint count = OD_OGL_RDR_READVALOFFSET(GLint, pMemPtr, 1);
          GLint first = 0;
#ifndef OD_OGL_ES
          GLint *pBufPtr = (GLint*)pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(GLint))].m_pData;
#else
          GLushort *pBufPtr = (GLushort*)pDataCont->m_ArrayElements.getPtr()[OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(GLint))].m_pData;
#endif
          GLint ctDiff, ctr;
          do
          {
            if (inprocCtx.nMarks)
              ctDiff = inprocCtx.pMarks->uFrom - first;
            else
              ctDiff = count - first;
            if (ctDiff)
            {
              switch (mode)
              {
                case GL_POINTS:
                {
                  for (ctr = 0; ctr < ctDiff; ctr++)
                  {
                    const vtxTypeArr *pData = pVtxData + pBufPtr[first + ctr];
                    vertices[0].set(pData[0][0], pData[0][1], pData[0][2]);
                    vertices[1] = vertices[0];
                    pGeom->polylineProc(2, vertices);
                  }
                }
                break;
                case GL_LINES:
                {
                  for (ctr = 0; ctr < ctDiff >> 1; ctr++)
                  {
                    const vtxTypeArr *pData1 = pVtxData + pBufPtr[first + (ctr << 1)],
                                     *pData2 = pVtxData + pBufPtr[first + (ctr << 1) + 1];
                    vertices[0].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                    vertices[1].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                    pGeom->polylineProc(2, vertices);
                  }
                }
                break;
                case GL_LINE_LOOP:
                {
                  const vtxTypeArr *pDataN1 = pVtxData + pBufPtr[first],
                                   *pDataN2 = pVtxData + pBufPtr[first + ctDiff - 1];
                  vertices[0].set(pDataN1[0][0], pDataN1[0][1], pDataN1[0][2]);
                  vertices[1].set(pDataN2[0][0], pDataN2[0][1], pDataN2[0][2]);
                  pGeom->polylineProc(2, vertices);
                }
                case GL_LINE_STRIP:
                {
                  for (ctr = 0; ctr < ctDiff - 1; ctr++)
                  {
                    const vtxTypeArr *pData1 = pVtxData + pBufPtr[first + ctr],
                                     *pData2 = pVtxData + pBufPtr[first + ctr + 1];
                    vertices[0].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                    vertices[1].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                    pGeom->polylineProc(2, vertices);
                  }
                }
                break;
                case GL_TRIANGLES:
                  for (ctr = 0; ctr < ctDiff / 3; ctr++)
                  {
                    const vtxTypeArr *pData1 = pVtxData + pBufPtr[first + ctr * 3],
                                     *pData2 = pVtxData + pBufPtr[first + ctr * 3 + 1],
                                     *pData3 = pVtxData + pBufPtr[first + ctr * 3 + 2];
                    vertices[0].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                    vertices[1].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                    vertices[2].set(pData3[0][0], pData3[0][1], pData3[0][2]);
                    pGeom->polygonProc(3, vertices);
                  }
                break;
                case GL_TRIANGLE_STRIP:
                  for (ctr = 0; ctr < ctDiff - 2; ctr++)
                  {
                    const vtxTypeArr *pData1 = pVtxData + pBufPtr[first + ctr * 3],
                                     *pData2 = pVtxData + pBufPtr[first + ctr * 3 + 1],
                                     *pData3 = pVtxData + pBufPtr[first + ctr * 3 + 2];
                    if ((ctr & 1) == 1)
                    {
                      vertices[0].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                      vertices[1].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                      vertices[2].set(pData3[0][0], pData3[0][1], pData3[0][2]);
                    }
                    else
                    {
                      vertices[0].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                      vertices[1].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                      vertices[2].set(pData3[0][0], pData3[0][1], pData3[0][2]);
                    }
                    pGeom->polygonProc(3, vertices);
                  }
                break;
                case GL_TRIANGLE_FAN:
#ifndef OD_OGL_ES // Prevent redefinition
                case GL_POLYGON:
#endif
                {
                  const vtxTypeArr *pData0 = pVtxData + pBufPtr[first];
                  vertices[0].set(pData0[0][0], pData0[0][1], pData0[0][2]);
                  for (ctr = 1; ctr < ctDiff - 1; ctr++)
                  {
                    const vtxTypeArr *pData1 = pVtxData + pBufPtr[first + ctr],
                                     *pData2 = pVtxData + pBufPtr[first + ctr + 1];
                    vertices[1].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                    vertices[2].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                    pGeom->polygonProc(3, vertices);
                  }
                }
                break;
#ifndef OD_OGL_ES
                case GL_QUADS:
                  for (ctr = 0; ctr < ctDiff >> 2; ctr++)
                  {
                    const vtxTypeArr *pData1 = pVtxData + pBufPtr[first + (ctr << 2)],
                                     *pData2 = pVtxData + pBufPtr[first + (ctr << 2) + 1],
                                     *pData3 = pVtxData + pBufPtr[first + (ctr << 2) + 2],
                                     *pData4 = pVtxData + pBufPtr[first + (ctr << 2) + 2];
                    vertices[0].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                    vertices[1].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                    vertices[2].set(pData3[0][0], pData3[0][1], pData3[0][2]);
                    vertices[3].set(pData4[0][0], pData4[0][1], pData4[0][2]);
                    pGeom->polygonProc(4, vertices);
                  }
                break;
                case GL_QUAD_STRIP:
                  for (ctr = 0; ctr < ctDiff - 2; ctr += 2)
                  {
                    const vtxTypeArr *pData1 = pVtxData + pBufPtr[first + (ctr << 2)],
                                     *pData2 = pVtxData + pBufPtr[first + (ctr << 2) + 1],
                                     *pData3 = pVtxData + pBufPtr[first + (ctr << 2) + 2],
                                     *pData4 = pVtxData + pBufPtr[first + (ctr << 2) + 2];
                    vertices[0].set(pData1[0][0], pData1[0][1], pData1[0][2]);
                    vertices[1].set(pData2[0][0], pData2[0][1], pData2[0][2]);
                    vertices[2].set(pData4[0][0], pData4[0][1], pData4[0][2]);
                    vertices[3].set(pData3[0][0], pData3[0][1], pData3[0][2]);
                    pGeom->polygonProc(4, vertices);
                  }
                break;
#endif
                default:
                  ODA_FAIL();
              }
            }
            if (inprocCtx.nMarks)
            {
              if (pCtx->regenAbort())
                return;
              if (m_pCallback)
                m_pCallback->metafileSelectMarker(inprocCtx.pMarks->gsMark);
              first += ctDiff;
              inprocCtx.pMarks++;
            }
            inprocCtx.nMarks--;
          }
          while (inprocCtx.nMarks != (OdUInt32)-1);
          inprocCtx.nMarks = 0;
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(GLint) + 1 + sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_CullFace:
        {
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdOpenGLMetaRecType_LStipple:
        {
          if (OD_OGL_RDR_READVALRETYPE(OdOpenGLLinePatt::Entry, OdUInt8, pMemPtr) == OdOpenGLLinePatt::kLnPattAll)
            OD_OGL_RDR_SEEKINC(pMemPtr);
          else
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 2);
        }
      break;
      case OdOpenGLMetaRecType_PStipple:
        {
          if (OD_OGL_RDR_READVALRETYPE(OdOpenGLFillPatt::Entry, OdUInt8, pMemPtr) == OdOpenGLFillPatt::kFillPattAll)
            OD_OGL_RDR_SEEKINC(pMemPtr);
          else
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 2);
        }
      break;
      case OdOpenGLMetaRecType_VPoint:
        {
          if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            const vtxTypeArr *pArr = OD_OGL_RDR_GETPTRNATIVE(const vtxTypeArr, pMemPtr);
            vertices[0].set(pArr[0][0], pArr[0][1], pArr[0][2]);
            vertices[1] = vertices[0];
            pGeom->polylineProc(2, vertices);
          }
#ifndef OD_OGL_ENABLEFLOATCONVERSION
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGePoint3d));
#else
          OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLfloat) << 1) + sizeof(GLfloat));
#endif
        }
      break;
      case OdOpenGLMetaRecType_VLine:
        {
          if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            const vtxTypeArr *pArr = OD_OGL_RDR_GETPTRNATIVE(const vtxTypeArr, pMemPtr);
            vertices[0].set(pArr[0][0], pArr[0][1], pArr[0][2]);
            vertices[1].set(pArr[1][0], pArr[1][1], pArr[1][2]);
            pGeom->polylineProc(2, vertices);
          }
#ifndef OD_OGL_ENABLEFLOATCONVERSION
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGePoint3d) << 1);
#else
          OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(GLfloat) << 2) + (sizeof(GLfloat) << 1));
#endif
        }
      break;
      case OdOpenGLMetaRecType_IPoint:
        {
          if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OdInt32 idx = OD_OGL_RDR_READVAL(OdInt32, pMemPtr);
            vertices[0].set(pVtxData[idx][0], pVtxData[idx][1], pVtxData[idx][2]);
            vertices[1] = vertices[0];
            pGeom->polylineProc(2, vertices);
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32));
        }
      break;
      case OdOpenGLMetaRecType_ILine:
        {
          if (!GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OdInt32 idx1 = OD_OGL_RDR_READVAL(OdInt32, pMemPtr);
            OdInt32 idx2 = OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, sizeof(OdInt32));
            vertices[0].set(pVtxData[idx1][0], pVtxData[idx1][1], pVtxData[idx1][2]);
            vertices[1].set(pVtxData[idx2][0], pVtxData[idx2][1], pVtxData[idx2][2]);
            pGeom->polylineProc(2, vertices);
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) * 2);
        }
      break;
      case OdOpenGLMetaRecType_HLRStencil:
      case OdOpenGLMetaRecType_EnableShading:
      case OdOpenGLMetaRecType_DisableShading:
        {
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdOpenGLMetaRecType_Material:
        {
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_UserEntry:
        {
          if (m_pCallback)
            m_pCallback->metafileProcessUserEntry(pDataCont, pMemPtr);
        }
      break;
      case OdOpenGLMetaRecType_InitTexture:
        {
          if (!OD_OGL_RDR_READVALOFFSET(bool, pMemPtr, sizeof(OdOpenGLMetafileReaderCallback::RasterImageEntry)))
          {
            if (OD_OGL_RDR_READVALOFFSET(bool, pMemPtr, sizeof(OdOpenGLMetafileReaderCallback::RasterImageEntry) + sizeof(bool) + sizeof(OdUInt32)))
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
            OD_OGL_RDR_SEEKINC(pMemPtr);
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdOpenGLMetafileReaderCallback::RasterImageEntry) + sizeof(OdUInt32) + sizeof(bool));
        }
      break;
      case OdOpenGLMetaRecType_UninitTexture:
      break;
      case OdOpenGLMetaRecType_SelectionMarker:
        {
          if (m_pCallback && bCheckMark)
            m_pCallback->metafileSelectMarker(OD_OGL_RDR_READVAL(OdOpenGLGsMarker, pMemPtr));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdOpenGLGsMarker));
        }
      break;
      case OdOpenGLMetaRecType_EnableMarkerArray:
        {
          if (bCheckMark)
          {
            OdUInt32 nArray = OD_OGL_RDR_READVAL(OdUInt32, pMemPtr);
            inprocCtx.pMarks = (const OdOpenGLSelMark*)pDataCont->m_ArrayElements.getPtr()[nArray].m_pData;
            inprocCtx.nMarks = pDataCont->m_ArrayElements.getPtr()[nArray].m_uSize / sizeof(OdOpenGLSelMark);
          }
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdOpenGLMetaRecType_DisableMarkerArray:
        inprocCtx.pMarks = NULL;
        inprocCtx.nMarks = 0;
      break;
      case OdOpenGLMetaRecType_SelectionFlags:
        {
          const OdUInt8 selFlags = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
          OD_OGL_RDR_SEEKINC(pMemPtr);
          if (bCheckMark)
            SETBIT(inprocCtx.bFlags, kSuppressGeom, /*(!bHighlighted &&*/ GETBIT(selFlags, OdOpenGLSelFlag_DontSelectUnhighlighted)/*) ||
                                                     (bHighlighted && GETBIT(selFlags, OdOpenGLSelFlag_DontSelectHighlighted))*/);
        }
      break;
      case OdOpenGLMetaRecType_Lineweight:
        if (OdOpenGLLwdSetting::isPs(OD_OGL_RDR_READVALRETYPE(OdOpenGLLwdSetting::LwdType, OdUInt8, pMemPtr)))
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(double));
        else
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(OdInt16));
      break;
      case OdOpenGLMetaRecType_Linestyle:
        {
          OdUInt8 defStyle = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
          if (defStyle)
            OD_OGL_RDR_SEEKINC(pMemPtr);
          else
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 3);
        }
      break;
      case OdOpenGLMetaRecType_TtfText:
        {
          if (m_pCallback && !GETBIT(inprocCtx.bFlags, kSuppressGeom))
          {
            OdUInt64 fontKeyA = OD_OGL_RDR_READVAL(OdUInt64, pMemPtr);
            OdUInt64 fontKeyB = OD_OGL_RDR_READVALOFFSET(OdUInt64, pMemPtr, sizeof(OdUInt64));
            const OdGeMatrix3d *pMtx = (const OdGeMatrix3d*)(pMemPtr + sizeof(OdUInt64) * 2);
            const OdGeVector3d *pVec = (const OdGeVector3d*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d));
            OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
            const OdUInt32 *pText = (const OdUInt32*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32));
            OdSaveState<InprocRegisters> _ss(m_inprocContext, inprocCtx);
            m_pCallback->metafileTtfText(fontKeyA, fontKeyB, pText, nText, *pMtx, *pVec);
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32) + sizeof(OdUInt32) * nText);
          }
          else
          {
            OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32) + sizeof(OdUInt32) * nText);
          }
        }
      break;
      case OdOpenGLMetaRecType_Empty:
      break;
      default:
        ODA_FAIL();
    }
  }
}

void OdOpenGLMetafileReader::drawArrayAntiAlias(GLenum mode, GLint first, GLsizei count, const GLvoid *pArray)
{
  if ((mode >= GL_LINES) && (mode <= GL_LINE_STRIP))
  {
    GLboolean lSmooth = ::glIsEnabled(GL_LINE_SMOOTH);
    GLboolean bBlend = ::glIsEnabled(GL_BLEND);
    if (!lSmooth)
    {
      ::glEnable(GL_LINE_SMOOTH);
      ::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }
    if (!bBlend)
    {
      ::glEnable(GL_BLEND);
      ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    drawArrayAsIs(mode, first, count, pArray, false, true);
    if (!bBlend)
      ::glDisable(GL_BLEND);
    if (!lSmooth)
      ::glDisable(GL_LINE_SMOOTH);
  }
  else
    drawArrayAsIs(mode, first, count, pArray, false, false);
}

void OdOpenGLMetafileReader::drawArrayAsIs(GLenum mode, GLint first, GLsizei count, const GLvoid *pArray, bool bPoint, bool bLine)
{
  GLfloat prevPtSet = 1.0f;
  bool bRestorePt = false;
  if (bPoint)
  {
    bRestorePt = m_pCallback && (!m_pCallback->metafileDrawExtendedPointLineweight() && !m_pCallback->metafilePointsLineweight());
    if (bRestorePt)
    {
      ::glDisable(GL_POINT_SMOOTH);
      ::glGetFloatv(GL_POINT_SIZE, &prevPtSet);
      ::glPointSize(1.0f);
    }
#ifndef OD_OGL_ES
    if (pArray && count == 1)
    {
      ::glBegin(GL_POINTS);
#ifdef OD_OGL_ENABLEFLOATCONVERSION
        ::glVertex3fv((const GLfloat*)pArray);
#else
        ::glVertex3dv((const GLdouble*)pArray);
#endif
      ::glEnd();
      if (bRestorePt)
      {
        ::glPointSize(prevPtSet);
        ::glEnable(GL_POINT_SMOOTH);
      }
      return;
    }
#endif
  }
#ifndef OD_OGL_ES
  else if (bLine)
  {
    if (pArray && count == 2)
    {
      ::glBegin(GL_LINES);
#ifdef OD_OGL_ENABLEFLOATCONVERSION
        ::glVertex3fv((const GLfloat*)pArray);
        ::glVertex3fv(((const GLfloat*)pArray) + 3);
#else
        ::glVertex3dv((const GLdouble*)pArray);
        ::glVertex3dv(((const GLdouble*)pArray) + 3);
#endif
      ::glEnd();
      return;
    }
  }
#endif
  if (pArray)
  {
    ::glVertexPointer(gGLArrayNVals[OdOpenGLMetaRecArrayType_Vertex], gGLArrayType[OdOpenGLMetaRecArrayType_Vertex],
                      gDefArrayString[OdOpenGLMetaRecArrayType_Vertex], pArray);
    ::glEnableClientState(GL_VERTEX_ARRAY);
  }
  glSafeDrawArrays(mode, first, count);
  if (pArray)
    ::glDisableClientState(GL_VERTEX_ARRAY);
  if (bRestorePt)
  {
    ::glPointSize(prevPtSet);
    ::glEnable(GL_POINT_SMOOTH);
  }
}

#ifndef OD_OGL_ALTERNATIVEHLT

inline void glInverseStipplePat()
{
  GLint factor, pat;
  ::glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &factor);
  ::glGetIntegerv(GL_LINE_STIPPLE_PATTERN, &pat);
  ::glLineStipple(factor, ~(GLushort)pat);
}

#endif // OD_OGL_ALTERNATIVEHLT

void OdOpenGLMetafileReader::drawArrayLwd(GLenum mode, GLint first, GLsizei count, bool bHighlighted, const GLvoid *pArray)
{
  bool bIsLine = glIsLineOrPtMode(mode), bIsPoint = false;
  if (bIsLine && glIsPtMode(mode))
  {
    bIsLine = false;
    bIsPoint = true;
  }
  if ((!bIsLine && !bIsPoint) || !m_pCallback || (bIsPoint && !m_pCallback->metafilePointsLineweight()))
  {
    drawArrayAsIs(mode, first, count, pArray, bIsPoint, bIsLine);
  }
  else
  {
    const bool bClientArray = (pArray == NULL),
               bExtended = ((bIsPoint) ? m_pCallback->metafileDrawExtendedPointLineweight() : m_pCallback->metafileDrawExtendedLineLineweight()) ||
                           (!prevState.pLwdStyle->m_bCurStyleDef && !bIsPoint);
    const GLboolean lnStpl = ::glIsEnabled(GL_LINE_STIPPLE);
    const GLboolean plStpl = ::glIsEnabled(GL_POLYGON_STIPPLE);
    const GLboolean hasColors = ::glIsEnabled(GL_COLOR_ARRAY);
    GLvoid *pColors = NULL;
    if (lnStpl) ::glDisable(GL_LINE_STIPPLE);
    if (plStpl) ::glDisable(GL_POLYGON_STIPPLE);
    if (bExtended)
    {
      if (bClientArray)
      {
        GLvoid *pNonConst = NULL;
        ::glGetPointerv(GL_VERTEX_ARRAY_POINTER, &pNonConst);
        pArray = pNonConst;
        ::glDisableClientState(GL_VERTEX_ARRAY);
      }
      if (hasColors)
      {
        ::glGetPointerv(GL_COLOR_ARRAY_POINTER, &pColors);
        ::glDisableClientState(GL_COLOR_ARRAY);
      }
#ifdef OD_OGL_ENABLEFLOATCONVERSION
      typedef GLfloat vtxType[3];
#else
      typedef GLdouble vtxType[3];
#endif
      const vtxType *pArrayVtx = (const vtxType*)pArray;
      const OdUInt8 *pColorsVtx = (const OdUInt8*)pColors;
      // Draw in software mode
      if (bIsPoint)
      {
        OdGePoint3d pt;
        for (GLsizei nPt = first; nPt < (count + first); nPt++)
        {
          pt.set(pArrayVtx[nPt][0], pArrayVtx[nPt][1], pArrayVtx[nPt][2]);
          if (!pColorsVtx)
            m_pCallback->metafileDrawExtendedPoint(&pt);
          else
            m_pCallback->metafileDrawExtendedPoint(&pt, pColorsVtx + (nPt * OD_OGL_NCOLORCOMPONENTS));
        }
      }
      else
      {
        OdGePoint3d pts[2];
        if (mode == GL_LINES)
        {
          if (prevState.pLwdStyle->m_bCurStyleDef)
          {
            // Doesn't need intermediate array for round joints and cap styles
            for (GLsizei nPt = first; nPt < (count + first); nPt += 2)
            {
              pts[0].set(pArrayVtx[nPt][0], pArrayVtx[nPt][1], pArrayVtx[nPt][2]);
              pts[1].set(pArrayVtx[nPt + 1][0], pArrayVtx[nPt + 1][1], pArrayVtx[nPt + 1][2]);
              if (!pColorsVtx)
                m_pCallback->metafileDrawExtendedLine(pts);
              else
                m_pCallback->metafileDrawExtendedLine(pts, pColorsVtx + (nPt * OD_OGL_NCOLORCOMPONENTS));
            }
          }
          else
          {
            // Accumulate linked line lists and drop as line strip
            OdUInt32 counter = 0;
            for (GLsizei nPt = first; nPt < (count + first); nPt += 2)
            {
              pts[0].set(pArrayVtx[nPt][0], pArrayVtx[nPt][1], pArrayVtx[nPt][2]);
              pts[1].set(pArrayVtx[nPt + 1][0], pArrayVtx[nPt + 1][1], pArrayVtx[nPt + 1][2]);
              if (counter == 0)
              {
                m_buffer.insert(m_buffer.end(), pts, pts + 2);
                if (pColorsVtx)
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (nPt * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((nPt + 2) * OD_OGL_NCOLORCOMPONENTS));
                counter += 2;
              }
              else if (m_buffer.last().isEqualTo(pts[0]))
              {
                m_buffer.push_back(pts[1]);
                if (pColorsVtx)
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + ((nPt + 1) * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((nPt + 2) * OD_OGL_NCOLORCOMPONENTS));
                counter++;
              }
              else
              {
                if (!pColorsVtx)
                  m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter);
                else
                {
                  m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter, m_clrBuffer.getPtr());
                  m_clrBuffer.clear();
                }
                m_buffer.clear(); 
                m_buffer.insert(m_buffer.end(), pts, pts + 2);
                if (pColorsVtx)
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (nPt * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((nPt + 2) * OD_OGL_NCOLORCOMPONENTS));
                counter = 2;
              }
            }
            // flush
            if (!pColorsVtx)
              m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter);
            else
            {
              m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter, m_clrBuffer.getPtr());
              m_clrBuffer.clear();
              if (m_clrBuffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS)
                m_clrBuffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS);
            }
            m_buffer.clear();
            if (m_buffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT)
              m_buffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT);
          }
        }
        else
        {
          if (prevState.pLwdStyle->m_bCurStyleDef)
          {
            // Doesn't need intermediate array for round joints and cap styles
            for (GLsizei nPt = first + 1; nPt < (count + first); nPt++)
            {
              pts[0].set(pArrayVtx[nPt - 1][0], pArrayVtx[nPt - 1][1], pArrayVtx[nPt - 1][2]);
              pts[1].set(pArrayVtx[nPt][0], pArrayVtx[nPt][1], pArrayVtx[nPt][2]);
              if (!pColorsVtx)
                m_pCallback->metafileDrawExtendedLine(pts);
              else
                m_pCallback->metafileDrawExtendedLine(pts, pColorsVtx + ((nPt - 1) * OD_OGL_NCOLORCOMPONENTS));
            }
            if (mode == GL_LINE_LOOP)
            {
              pts[0].set(pArrayVtx[first + count - 1][0], pArrayVtx[first + count - 1][1], pArrayVtx[first + count - 1][2]);
              pts[1].set(pArrayVtx[first][0], pArrayVtx[first][1], pArrayVtx[first][2]);
              if (!pColorsVtx)
                m_pCallback->metafileDrawExtendedLine(pts);
              else
              {
                m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + ((first + count - 1) * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((first + count) * OD_OGL_NCOLORCOMPONENTS));
                m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + first * OD_OGL_NCOLORCOMPONENTS, pColorsVtx + (first + 1) * OD_OGL_NCOLORCOMPONENTS);
                m_pCallback->metafileDrawExtendedLine(pts, m_clrBuffer.getPtr());
                m_clrBuffer.clear();
              }
            }
          }
          else
          {
            // Drop through intermediate array
            m_buffer.resize(count);
            OdGePoint3d *pBuf = m_buffer.asArrayPtr();
            for (GLsizei nPt = first; nPt < (count + first); nPt++)
              pBuf[nPt - first].set(pArrayVtx[nPt][0], pArrayVtx[nPt][1], pArrayVtx[nPt][2]);
            if (pColorsVtx)
            {
              m_clrBuffer.resize(count * OD_OGL_NCOLORCOMPONENTS);
              OdUInt8 *pBuf = m_clrBuffer.asArrayPtr();
              ::memcpy(pBuf, pColorsVtx + first, count * OD_OGL_NCOLORCOMPONENTS);
            }
            if (mode == GL_LINE_LOOP)
            {
              pts[1].set(pArrayVtx[first][0], pArrayVtx[first][1], pArrayVtx[first][2]);
              m_buffer.push_back(pts[1]);
              if (pColorsVtx)
              {
                m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + first * OD_OGL_NCOLORCOMPONENTS, pColorsVtx + (first + 1) * OD_OGL_NCOLORCOMPONENTS);
                m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), count + 1, m_clrBuffer.getPtr());
              }
              else
                m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), count + 1);
            }
            else
            {
              if (!pColorsVtx)
                m_pCallback->metafileDrawExtendedLineStrip(pBuf, count);
              else
                m_pCallback->metafileDrawExtendedLineStrip(pBuf, count, m_clrBuffer.getPtr());
            }
            m_buffer.clear();
            if (m_buffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT)
              m_buffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT);
            if (pColorsVtx)
            {
              m_clrBuffer.clear();
              if (m_clrBuffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS)
                m_clrBuffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS);
            }
          }
        }
      }
      if (bClientArray)
      {
        ::glVertexPointer(gGLArrayNVals[OdOpenGLMetaRecArrayType_Vertex], gGLArrayType[OdOpenGLMetaRecArrayType_Vertex],
                          gDefArrayString[OdOpenGLMetaRecArrayType_Vertex], pArray);
        ::glEnableClientState(GL_VERTEX_ARRAY);
        pArray = NULL;
      }
      if (hasColors)
      {
        ::glColorPointer(gGLArrayNVals[OdOpenGLMetaRecArrayType_Color], gGLArrayType[OdOpenGLMetaRecArrayType_Color],
                         gDefArrayString[OdOpenGLMetaRecArrayType_Color], pColors);
        ::glEnableClientState(GL_COLOR_ARRAY);
        pColors = NULL;
      }
    }
    else
    {
      // All will be rendered by hardware
      drawArrayAsIs(mode, first, count, pArray, bIsPoint, bIsLine);
      if (bIsLine)
        drawArrayAsIs(GL_POINTS, first, count, pArray, false, false);
    }
    if (lnStpl) ::glEnable(GL_LINE_STIPPLE);
    if (lnStpl) ::glEnable(GL_POLYGON_STIPPLE);
#ifndef OD_OGL_ALTERNATIVEHLT
    if (((bHighlighted || lnStpl) && bIsLine) && !prevState.bForceAlternativeHlt)
    {
      ODCOLORREF bkgndColor = 0;
      m_pCallback->metafileGetBackgroundColor(bkgndColor);
      if (prevState.m_pColorConverter && prevState.m_bProcessBackgroundColors)
        bkgndColor = prevState.m_pColorConverter->convert(bkgndColor);
      GLfloat curColor[4];
      ::glGetFloatv(GL_CURRENT_COLOR, curColor);
      ::glColor3ub(ODGETRED(bkgndColor), ODGETGREEN(bkgndColor), ODGETBLUE(bkgndColor));
#ifndef OD_OGL_ALTERNATIVEHLT
      ::glInverseStipplePat();
#endif
      GLfloat lineWidth;
      if (!bExtended)
      {
        ::glGetFloatv(GL_LINE_WIDTH, &lineWidth);
        ::glDisable(GL_LINE_SMOOTH);
        ::glLineWidth(1.0f);
      }
      if (hasColors) ::glDisableClientState(GL_COLOR_ARRAY);
      drawArrayAsIs(mode, first, count, pArray, false, true);
      if (hasColors) ::glEnableClientState(GL_COLOR_ARRAY);
      if (!bExtended)
      {
        ::glLineWidth(lineWidth);
        ::glEnable(GL_LINE_SMOOTH);
      }
#ifndef OD_OGL_ALTERNATIVEHLT
      ::glInverseStipplePat();
#endif
      ::glColor4fv(curColor);
    }
#endif
  }
}

void OdOpenGLMetafileReader::drawElementsAntiAlias(GLenum mode, GLsizei count, GLenum type, const GLvoid *pIds)
{
  if ((mode >= GL_LINES) && (mode <= GL_LINE_STRIP))
  {
    GLboolean lSmooth = ::glIsEnabled(GL_LINE_SMOOTH);
    GLboolean bBlend = ::glIsEnabled(GL_BLEND);
    if (!lSmooth)
    {
      ::glEnable(GL_LINE_SMOOTH);
      ::glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }
    if (!bBlend)
    {
      ::glEnable(GL_BLEND);
      ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    drawElementsAsIs(mode, count, type, pIds, NULL, false, true);
    if (!bBlend)
      ::glDisable(GL_BLEND);
    if (!lSmooth)
      ::glDisable(GL_LINE_SMOOTH);
  }
  else
    drawElementsAsIs(mode, count, type, pIds, NULL, false, false);
}

void OdOpenGLMetafileReader::drawElementsAsIs(GLenum mode, GLsizei count, GLenum type, const GLvoid *pIds, const GLvoid *pArray, bool bPoint, bool bLine)
{
  GLfloat prevPtSet = 1.0f;
  bool bRestorePt = false;
  if (bPoint)
  {
    bRestorePt = m_pCallback && (!m_pCallback->metafileDrawExtendedPointLineweight() && !m_pCallback->metafilePointsLineweight());
    if (bRestorePt)
    {
      ::glDisable(GL_POINT_SMOOTH);
      ::glGetFloatv(GL_POINT_SIZE, &prevPtSet);
      ::glPointSize(1.0f);
    }
#ifndef OD_OGL_ES
    if (!pArray && count == 1)
    {
      ::glBegin(GL_POINTS);
        ::glArrayElement(*(const GLint*)pIds);
      ::glEnd();
      if (bRestorePt)
      {
        ::glPointSize(prevPtSet);
        ::glEnable(GL_POINT_SMOOTH);
      }
      return;
    }
#endif
  }
#ifndef OD_OGL_ES
  else if (bLine)
  {
    if (!pArray && count == 2)
    {
      ::glBegin(GL_LINES);
        ::glArrayElement(*(const GLint*)pIds);
        ::glArrayElement(*((const GLint*)pIds + 1));
      ::glEnd();
      return;
    }
  }
#endif
  if (pArray)
  {
    ::glVertexPointer(gGLArrayNVals[OdOpenGLMetaRecArrayType_Vertex], gGLArrayType[OdOpenGLMetaRecArrayType_Vertex],
                      gDefArrayString[OdOpenGLMetaRecArrayType_Vertex], pArray);
    ::glEnableClientState(GL_VERTEX_ARRAY);
  }
  ::glDrawElements(mode, count, type, pIds);
  if (pArray)
    ::glDisableClientState(GL_VERTEX_ARRAY);
  if (bRestorePt)
  {
    ::glPointSize(prevPtSet);
    ::glEnable(GL_POINT_SMOOTH);
  }
}

struct CombinedLwColors
{
  OdUInt8 m_colors[OD_OGL_NCOLORCOMPONENTS * 2];
  CombinedLwColors() { }
  const OdUInt8 *combine(const OdUInt8 *pClr1, const OdUInt8 *pClr2)
  {
    for (int n = 0; n < OD_OGL_NCOLORCOMPONENTS; n++)
    {
      m_colors[n] = pClr1[n]; m_colors[OD_OGL_NCOLORCOMPONENTS + n] = pClr2[n];
    }
    return m_colors;
  }
};

void OdOpenGLMetafileReader::drawElementsLwd(GLenum mode, GLsizei count, GLenum type, bool bHighlighted, const GLvoid *pIds, const GLvoid *pArray)
{
  bool bIsLine = glIsLineOrPtMode(mode), bIsPoint = false;
  if (bIsLine && glIsPtMode(mode))
  {
    bIsLine = false;
    bIsPoint = true;
  }
  if ((!bIsLine && !bIsPoint) || !m_pCallback || (bIsPoint && !m_pCallback->metafilePointsLineweight()))
  {
    drawElementsAsIs(mode, count, type, pIds, pArray, bIsPoint, bIsLine);
  }
  else
  {
    const bool bClientArray = (pArray == NULL),
               bExtended = ((bIsPoint) ? m_pCallback->metafileDrawExtendedPointLineweight() : m_pCallback->metafileDrawExtendedLineLineweight()) ||
                           (!prevState.pLwdStyle->m_bCurStyleDef && !bIsPoint);
    GLboolean lnStpl = ::glIsEnabled(GL_LINE_STIPPLE);
    GLboolean plStpl = ::glIsEnabled(GL_POLYGON_STIPPLE);
    const GLboolean hasColors = ::glIsEnabled(GL_COLOR_ARRAY);
    GLvoid *pColors = NULL;
    if (lnStpl) ::glDisable(GL_LINE_STIPPLE);
    if (plStpl) ::glDisable(GL_POLYGON_STIPPLE);
    if (bExtended)
    {
      if (bClientArray)
      {
        GLvoid *pNonConst = NULL;
        ::glGetPointerv(GL_VERTEX_ARRAY_POINTER, &pNonConst);
        pArray = pNonConst;
        ::glDisableClientState(GL_VERTEX_ARRAY);
      }
      if (hasColors)
      {
        ::glGetPointerv(GL_COLOR_ARRAY_POINTER, &pColors);
        ::glDisableClientState(GL_COLOR_ARRAY);
      }
#ifdef OD_OGL_ENABLEFLOATCONVERSION
      typedef GLfloat vtxType[3];
#else
      typedef GLdouble vtxType[3];
#endif
#ifdef OD_OGL_ES
      typedef GLushort idType;
#else
      typedef GLint idType;
#endif
      const idType *pIdsVtx = (const idType*)pIds;
      const vtxType *pArrayVtx = (const vtxType*)pArray;
      const OdUInt8 *pColorsVtx = (const OdUInt8*)pColors;
      // Draw in software mode
      if (bIsPoint)
      {
        OdGePoint3d pt;
        for (GLsizei nPt = 0; nPt < count; nPt++)
        {
          pt.set(pArrayVtx[pIdsVtx[nPt]][0], pArrayVtx[pIdsVtx[nPt]][1], pArrayVtx[pIdsVtx[nPt]][2]);
          if (!pColorsVtx)
            m_pCallback->metafileDrawExtendedPoint(&pt);
          else
            m_pCallback->metafileDrawExtendedPoint(&pt, pColorsVtx + (pIdsVtx[nPt] * OD_OGL_NCOLORCOMPONENTS));
        }
      }
      else
      {
        OdGePoint3d pts[2];
        if (mode == GL_LINES)
        {
          if (prevState.pLwdStyle->m_bCurStyleDef)
          {
            // Doesn't need intermediate array for round joints and cap styles
            for (GLsizei nPt = 0; nPt < count; nPt += 2)
            {
              pts[0].set(pArrayVtx[pIdsVtx[nPt]][0], pArrayVtx[pIdsVtx[nPt]][1], pArrayVtx[pIdsVtx[nPt]][2]);
              pts[1].set(pArrayVtx[pIdsVtx[nPt + 1]][0], pArrayVtx[pIdsVtx[nPt + 1]][1], pArrayVtx[pIdsVtx[nPt + 1]][2]);
              if (!pColorsVtx)
                m_pCallback->metafileDrawExtendedLine(pts);
              else
                m_pCallback->metafileDrawExtendedLine(pts,
                  CombinedLwColors().combine(pColorsVtx + (pIdsVtx[nPt] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + (pIdsVtx[nPt + 1] * OD_OGL_NCOLORCOMPONENTS)));
            }
          }
          else
          {
            // Accumulate linked line lists and drop as line strip
            OdUInt32 counter = 0;
            for (GLsizei nPt = 0; nPt < count; nPt += 2)
            {
              pts[0].set(pArrayVtx[pIdsVtx[nPt]][0], pArrayVtx[pIdsVtx[nPt]][1], pArrayVtx[pIdsVtx[nPt]][2]);
              pts[1].set(pArrayVtx[pIdsVtx[nPt + 1]][0], pArrayVtx[pIdsVtx[nPt + 1]][1], pArrayVtx[pIdsVtx[nPt + 1]][2]);
              if (counter == 0)
              {
                m_buffer.insert(m_buffer.end(), pts, pts + 2);
                if (pColorsVtx)
                {
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (pIdsVtx[nPt] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((pIdsVtx[nPt] + 1) * OD_OGL_NCOLORCOMPONENTS));
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (pIdsVtx[nPt + 1] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((pIdsVtx[nPt + 1] + 1) * OD_OGL_NCOLORCOMPONENTS));
                }
                counter += 2;
              }
              else if (m_buffer.last().isEqualTo(pts[0]))
              {
                m_buffer.push_back(pts[1]);
                if (pColorsVtx)
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (pIdsVtx[nPt + 1] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((pIdsVtx[nPt + 1] + 1) * OD_OGL_NCOLORCOMPONENTS));
                counter++;
              }
              else
              {
                if (!pColorsVtx)
                  m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter);
                else
                {
                  m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter, m_clrBuffer.getPtr());
                  m_clrBuffer.clear();
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (pIdsVtx[nPt] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((pIdsVtx[nPt] + 1) * OD_OGL_NCOLORCOMPONENTS));
                  m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (pIdsVtx[nPt + 1] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((pIdsVtx[nPt + 1] + 1) * OD_OGL_NCOLORCOMPONENTS));
                }
                m_buffer.clear();
                m_buffer.insert(m_buffer.end(), pts, pts + 2);
                counter = 2;
              }
            }
            // flush
            if (!pColorsVtx)
              m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter);
            else
            {
              m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), counter, m_clrBuffer.getPtr());
              m_clrBuffer.clear();
              if (m_clrBuffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS)
                m_clrBuffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS);
            }
            m_buffer.clear();
            if (m_buffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT)
              m_buffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT);
          }
        }
        else
        {
          if (prevState.pLwdStyle->m_bCurStyleDef)
          {
            // Doesn't need intermediate array for round joints and cap styles
            for (GLsizei nPt = 1; nPt < count; nPt++)
            {
              pts[0].set(pArrayVtx[pIdsVtx[nPt - 1]][0], pArrayVtx[pIdsVtx[nPt - 1]][1], pArrayVtx[pIdsVtx[nPt - 1]][2]);
              pts[1].set(pArrayVtx[pIdsVtx[nPt]][0], pArrayVtx[pIdsVtx[nPt]][1], pArrayVtx[pIdsVtx[nPt]][2]);
              if (!pColorsVtx)
                m_pCallback->metafileDrawExtendedLine(pts);
              else
                m_pCallback->metafileDrawExtendedLine(pts,
                  CombinedLwColors().combine(pColorsVtx + (pIdsVtx[nPt - 1] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + (pIdsVtx[nPt] * OD_OGL_NCOLORCOMPONENTS)));
            }
            if (mode == GL_LINE_LOOP)
            {
              pts[0].set(pArrayVtx[pIdsVtx[count - 1]][0], pArrayVtx[pIdsVtx[count - 1]][1], pArrayVtx[pIdsVtx[count - 1]][2]);
              pts[1].set(pArrayVtx[pIdsVtx[0]][0], pArrayVtx[pIdsVtx[0]][1], pArrayVtx[pIdsVtx[0]][2]);
              if (!pColorsVtx)
                m_pCallback->metafileDrawExtendedLine(pts);
              else
                m_pCallback->metafileDrawExtendedLine(pts,
                  CombinedLwColors().combine(pColorsVtx + (pIdsVtx[count - 1] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + (pIdsVtx[0] * OD_OGL_NCOLORCOMPONENTS)));
            }
          }
          else
          {
            // Drop through intermediate array
            m_buffer.resize(count);
            OdGePoint3d *pBuf = m_buffer.asArrayPtr();
            for (GLsizei nPt = 0; nPt < count; nPt++)
              pBuf[nPt].set(pArrayVtx[pIdsVtx[nPt]][0], pArrayVtx[pIdsVtx[nPt]][1], pArrayVtx[pIdsVtx[nPt]][2]);
            if (pColorsVtx)
            {
              m_clrBuffer.resize(count * OD_OGL_NCOLORCOMPONENTS);
              OdUInt8 *pClrs = m_clrBuffer.asArrayPtr();
              for (GLsizei nPt = 0; nPt < count; nPt++)
              {
                const OdUInt8 *pColors = pColorsVtx + pIdsVtx[nPt] * OD_OGL_NCOLORCOMPONENTS;
                for (int i = 0; i < OD_OGL_NCOLORCOMPONENTS; i++)
                  pClrs[nPt + i] = pColors[i];
              }
            }
            if (mode == GL_LINE_LOOP)
            {
              pts[1].set(pArrayVtx[pIdsVtx[0]][0], pArrayVtx[pIdsVtx[0]][1], pArrayVtx[pIdsVtx[0]][2]);
              m_buffer.push_back(pts[1]);
              if (pColorsVtx)
              {
                m_clrBuffer.insert(m_clrBuffer.end(), pColorsVtx + (pIdsVtx[0] * OD_OGL_NCOLORCOMPONENTS), pColorsVtx + ((pIdsVtx[0] + 1) * OD_OGL_NCOLORCOMPONENTS));
                m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), count + 1, m_clrBuffer.getPtr());
              }
              else
                m_pCallback->metafileDrawExtendedLineStrip(m_buffer.getPtr(), count + 1);
            }
            else
            {
              if (pColorsVtx)
                m_pCallback->metafileDrawExtendedLineStrip(pBuf, count, m_clrBuffer.getPtr());
              else
                m_pCallback->metafileDrawExtendedLineStrip(pBuf, count);
            }
            m_buffer.clear();
            if (m_buffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT)
              m_buffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT);
            if (pColorsVtx)
            {
              m_clrBuffer.clear();
              if (m_clrBuffer.physicalLength() > OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS)
                m_clrBuffer.setPhysicalLength(OD_OGL_LWDBUFKEEPLIMIT * OD_OGL_NCOLORCOMPONENTS);
            }
          }
        }
      }
      if (bClientArray)
      {
        ::glVertexPointer(gGLArrayNVals[OdOpenGLMetaRecArrayType_Vertex], gGLArrayType[OdOpenGLMetaRecArrayType_Vertex],
                          gDefArrayString[OdOpenGLMetaRecArrayType_Vertex], pArray);
        ::glEnableClientState(GL_VERTEX_ARRAY);
        pArray = NULL;
      }
      if (hasColors)
      {
        ::glColorPointer(gGLArrayNVals[OdOpenGLMetaRecArrayType_Color], gGLArrayType[OdOpenGLMetaRecArrayType_Color],
                         gDefArrayString[OdOpenGLMetaRecArrayType_Color], pColors);
        ::glEnableClientState(GL_COLOR_ARRAY);
        pColors = NULL;
      }
    }
    else
    {
      // All will be rendered by hardware
      drawElementsAsIs(mode, count, type, pIds, pArray, bIsPoint, bIsLine);
      if (bIsLine)
        drawElementsAsIs(GL_POINTS, count, type, pIds, pArray, false, false);
    }
    if (lnStpl) ::glEnable(GL_LINE_STIPPLE);
    if (plStpl) ::glEnable(GL_POLYGON_STIPPLE);
#ifndef OD_OGL_ALTERNATIVEHLT
    if (((bHighlighted || lnStpl) && bIsLine) && !prevState.bForceAlternativeHlt)
    {
      ODCOLORREF bkgndColor = 0;
      m_pCallback->metafileGetBackgroundColor(bkgndColor);
      if (prevState.m_pColorConverter && prevState.m_bProcessBackgroundColors)
        bkgndColor = prevState.m_pColorConverter->convert(bkgndColor);
      GLfloat curColor[4];
      ::glGetFloatv(GL_CURRENT_COLOR, curColor);
      ::glColor3ub(ODGETRED(bkgndColor), ODGETGREEN(bkgndColor), ODGETBLUE(bkgndColor));
      ::glInverseStipplePat();
      GLfloat lineWidth;
      if (!bExtended)
      {
        ::glGetFloatv(GL_LINE_WIDTH, &lineWidth);
        ::glDisable(GL_LINE_SMOOTH);
        ::glLineWidth(1.0f);
      }
      if (hasColors) ::glDisableClientState(GL_COLOR_ARRAY);
      drawElementsAsIs(mode, count, type, pIds, pArray, false, true);
      if (hasColors) ::glEnableClientState(GL_COLOR_ARRAY);
      if (!bExtended)
      {
        ::glLineWidth(lineWidth);
        ::glEnable(GL_LINE_SMOOTH);
      }
      ::glInverseStipplePat();
      ::glColor4fv(curColor);
    }
#endif
  }
}

//
