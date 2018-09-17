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
#include "GsOpenGLVectorizer.h"
#include "OpenGLExtensions.h"
#include "ExGsHelpers.h"
#include "RxObjectImpl.h"
#include "OdAnsiString.h"
#include "Ge/GePlane.h"
#include "Ge/GeScale3d.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeQuaternion.h"
#include "StaticRxObject.h"
#include "Gs/GsLight.h"
#include "Gs/GsViewportProperties.h"
#include "Gs/LineWtIndex.h"
#include "Gs/GsDbRootLinkage.h"
#include "Gi/GiUtils.h"
#include "Gi/GiShellFaceIterator.h"
#include "Gi/GiFastExtCalc.h"
#include "Gi/GiRasterWrappers.h"
#include "Gi/GiLightTraitsData.h"
#include "Gi/GiMaterialItem.h"
#include "Gi/GiMapperItem.h"
#include "Gi/GiViewportGeometry.h"
#include "Gs/GsMaterialNode.h"
#include "UInt8Array.h"
#include "Int32Array.h"
#include "SaveState.h"
#ifdef GL_EXT_texture_compression_s3tc
#include "mono_dxt1_compressor.h"
#endif

ODA_ASSUME(sizeof(GLushort) >= 2);

#include "GsOpenGLInclude.h"

// OLE support
#include "../ExOwnerDrawDc.cpp" // Kernel/Extensions/ExRender/ExOwnerDrawDc.cpp

#if defined(ANDROID)
static OdRefCounter flashCounter = 0;
void glSafeDrawArrays(GLenum mode, GLint first, GLsizei count)
{
  ::glDrawArrays(mode, first, count);
  if (flashCounter < MAX_COUNTER_BEFORE_FLASH)
  {
    ++flashCounter;
    return;
  }
  flashCounter = 0;
  ::glFlush();
}
#endif

// Maximum vertices for line cap
#define OD_OGL_MAXLINECAPVERTS   15

#ifdef OD_OGL_ES // OpenGL ES specific helpers
// not static. maybe usable from another translation element.
void oglEsDrawBuffer(GLint size, const GLvoid *pData, GLenum mode, GLsizei count)
{
  ::glVertexPointer(size, GL_FLOAT, sizeof(GLfloat) * size, pData);
  ::glEnableClientState(GL_VERTEX_ARRAY);
  glSafeDrawArrays(mode, 0, count);
  ::glDisableClientState(GL_VERTEX_ARRAY);
}
void oglEsDrawPlain(OdInt32 nPoints, const OdGePoint3d* pPoints, GLenum mode)
{
  OdArray<GLfloat> tmpVerts(nPoints * 3, 1);
  tmpVerts.resize(nPoints * 3);
  GLfloat *pBuf = tmpVerts.asArrayPtr();
  while(nPoints--)
  {
    pBuf[0] = (GLfloat)pPoints->x;
    pBuf[1] = (GLfloat)pPoints->y;
    pBuf[2] = (GLfloat)pPoints->z;
    pPoints++;
    pBuf += 3;
  }
  ::oglEsDrawBuffer(3, tmpVerts.getPtr(), mode, tmpVerts.size()/3);
}
void oglEsDrawIndexed(OdInt32 nInd, const OdInt32 *pInd, const OdGePoint3d *pPoints, GLenum mode)
{
  OdArray<GLfloat> tmpVerts(nInd * 3, 1);
  tmpVerts.resize(nInd * 3);
  GLfloat *pBuf = tmpVerts.asArrayPtr();
  while(nInd--)
  {
    pBuf[0] = (GLfloat)pPoints[*pInd].x;
    pBuf[1] = (GLfloat)pPoints[*pInd].y;
    pBuf[2] = (GLfloat)pPoints[*pInd].z;
    pInd++;
    pBuf += 3;
  }
  ::oglEsDrawBuffer(3, tmpVerts.getPtr(), mode, tmpVerts.size()/3);
}
void oglEsDrawIndexedTrg(const OdInt32 *pInd, const OdGePoint3d *pPoints,
                         const OdGeVector3d *pNormals, const OdGePoint2d *pTexCoords, const OdCmEntityColor *pColors, OdUInt8 alpha = 255)
{
  GLfloat verts[3 * 3];
  GLfloat norms[3 * 3];
  GLfloat texCrds[3 * 2];
  GLubyte clrs[3 * 4];
  int i;
  for (i = 0; i < 3; i++)
  {
    verts[i * 3 + 0] = (GLfloat)pPoints[pInd[i]].x;
    verts[i * 3 + 1] = (GLfloat)pPoints[pInd[i]].y;
    verts[i * 3 + 2] = (GLfloat)pPoints[pInd[i]].z;
  }
  ::glVertexPointer(3, GL_FLOAT, sizeof(GLfloat) * 3, verts);
  ::glEnableClientState(GL_VERTEX_ARRAY);
  if (pNormals)
  {
    for (i = 0; i < 3; i++)
    {
      norms[i * 3 + 0] = (GLfloat)pNormals[i].x;
      norms[i * 3 + 1] = (GLfloat)pNormals[i].y;
      norms[i * 3 + 2] = (GLfloat)pNormals[i].z;
    }
    ::glNormalPointer(GL_FLOAT, sizeof(GLfloat) * 3, verts);
    ::glEnableClientState(GL_NORMAL_ARRAY);
  }
  if (pTexCoords)
  {
    for (i = 0; i < 3; i++)
    {
      texCrds[i * 2 + 0] = (GLfloat)pTexCoords[pInd[i]].x;
      texCrds[i * 2 + 1] = (GLfloat)pTexCoords[pInd[i]].y;
    }
    ::glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, texCrds);
    ::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if (pColors)
  {
    for (i = 0; i < 3; i++)
    {
      clrs[i * 4 + 0] = (GLubyte)pColors[pInd[i]].red();
      clrs[i * 4 + 1] = (GLubyte)pColors[pInd[i]].green();
      clrs[i * 4 + 2] = (GLubyte)pColors[pInd[i]].blue();
      clrs[i * 4 + 3] = (GLubyte)alpha;
    }
    ::glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(GLubyte) * 4, clrs);
    ::glEnableClientState(GL_COLOR_ARRAY);
  }
  glSafeDrawArrays(GL_TRIANGLES, 0, 3);
  if (pColors)
    ::glDisableClientState(GL_COLOR_ARRAY);
  if (pTexCoords)
    ::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  if (pNormals)
    ::glDisableClientState(GL_NORMAL_ARRAY);
  ::glDisableClientState(GL_VERTEX_ARRAY);
}
void OdGlEsStencilBitSaver::pushAttrib()
{
  Entry ent;
  ent.m_enabled = ::glIsEnabled(GL_STENCIL_TEST);
  ::glGetIntegerv(GL_STENCIL_FUNC, (GLint*)&(ent.m_func));
  ::glGetIntegerv(GL_STENCIL_REF, (GLint*)&(ent.m_ref));
  ::glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint*)&(ent.m_mask));
  ::glGetIntegerv(GL_STENCIL_FAIL, (GLint*)&(ent.m_fail));
  ::glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint*)&(ent.m_zfail));
  ::glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint*)&(ent.m_zpass));
  ::glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint*)&(ent.m_writemask));
  m_stack.push(ent);
}
void OdGlEsStencilBitSaver::popAttrib()
{
  if (!m_stack.empty())
  {
    Entry ent;
    m_stack.pop(ent);
    if (ent.m_enabled)
      ::glEnable(GL_STENCIL_TEST);
    else
      ::glDisable(GL_STENCIL_TEST);
    ::glStencilFunc(ent.m_func, ent.m_ref, ent.m_mask);
    ::glStencilOp(ent.m_fail, ent.m_zfail, ent.m_zpass);
    ::glStencilMask(ent.m_writemask);
  }
}
#endif

OdGsOpenGLVectorizeDevice::OdGsOpenGLVectorizeDevice()
  : m_bDiscardBackFaces(false)
  , m_bClearScreen(true)
#ifndef OD_OGL_ALTERNATIVEHLT
  , m_bForceAlternativeHlt(false)
#else
  , m_bForceAlternativeHlt(true)
#endif
  , m_alternativeHltColor(ODRGBA(255, 162, 0, 128))
  , m_bUseTTFCache(false)
  , m_bUseLutPalette(3)
  , m_bLinesRepMode(false)
  , m_lineweightOptimization(3)
  , m_maxTextureSize(1024)
{
  setSortRenderTypes(true);
  m_pTexManager = OdGiMaterialTextureManager::createObject();
  // Init all extensions with undetermined state
  OdInt32 nExt;
  for (nExt = 0; nExt < (OdInt32)OpenGLExtension_NExtensions; nExt++)
    m_extAvail[nExt] = kExtSupportUndetermined;
  for (nExt = 0; nExt < (OdInt32)OpenGLExtFunc_NFuncs; nExt++)
    m_extFuncs[nExt] = NULL;
}

OdGsOpenGLVectorizeDevice::~OdGsOpenGLVectorizeDevice()
{
}

void OdGsOpenGLVectorizeDevice::clear()
{
  // OpenGL Normalized device coordinates: lower-left(-1,-1):upper-right(1,1)
  ::glViewport(0, 0, width(), height());

  if (!isClearScreenEnabled())
    return;

  const OdGsDCRectArray& invRects = invalidRects();
  int n = invRects.size();
  ODCOLORREF bckgnd(colorConvert(getBackgroundColor(), true));
  // for debugging
  //bckgnd = RGB(rand() * 255 / RAND_MAX, rand() * 255 / RAND_MAX, rand() * 255 / RAND_MAX);

  if(supportPartialUpdate() && n)
  {
    /*
    // for debugging
    ::glClearColor(GLclampf(float(ODGETRED(bckgnd))  /255.),
                   GLclampf(float(ODGETGREEN(bckgnd))/255.),
                   GLclampf(float(ODGETBLUE(bckgnd)) /255.),
                   GLclampf(.0f));
    ::glClear(GL_COLOR_BUFFER_BIT);
    bckgnd = RGB(rand() * 255 / RAND_MAX, rand() * 255 / RAND_MAX, rand() * 255 / RAND_MAX);
    */

    ::glColor3ub(ODGETRED(bckgnd), ODGETGREEN(bckgnd), ODGETBLUE(bckgnd));
    ::glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    OdGeMatrix3d xToGL;
    OdGsDCRect screenRect = outputRect();

    xToGL(0,0) = (screenRect.m_max.x - screenRect.m_min.x);
    xToGL(1,1) = (screenRect.m_max.y - screenRect.m_min.y);
    xToGL(0,3) = screenRect.m_min.x;
    xToGL(1,3) = screenRect.m_min.y;
    xToGL.invert();
    xToGL.preMultBy(OdGeMatrix3d::scaling(OdGeScale3d(2., 2., 2.), OdGePoint3d(1.,1.,1.)));

    ::glMatrixMode(GL_PROJECTION);
    glLoadTransposedMatrix(xToGL);

    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();  

    ::glDisable(GL_STENCIL_TEST);
    ::glDisable(GL_DEPTH_TEST);
    ::glDisable(GL_LIGHTING);
    ::glDisable(GL_POLYGON_STIPPLE);
#ifndef OD_OGL_ALTERNATIVEHLT
    if (isAlternativeHltEnabled())
#endif
      ::glDisable(GL_FOG);
    ::glDisable(GL_TEXTURE_2D);

    const OdGsDCRect* pInvRect = invRects.getPtr();
    OdGePoint3d pt;
    do
    {
#ifndef OD_OGL_ES
      ::glBegin(GL_TRIANGLE_FAN);
        ::glVertex2d(pInvRect->m_min.x, pInvRect->m_min.y);
        ::glVertex2d(pInvRect->m_min.x, pInvRect->m_max.y);
        ::glVertex2d(pInvRect->m_max.x, pInvRect->m_max.y);
        ::glVertex2d(pInvRect->m_max.x, pInvRect->m_min.y);
      ::glEnd();
#else
      const GLfloat invVerts[4 * 2] =
      {
        pInvRect->m_min.x, pInvRect->m_min.y,
        pInvRect->m_min.x, pInvRect->m_max.y,
        pInvRect->m_max.x, pInvRect->m_max.y,
        pInvRect->m_max.x, pInvRect->m_min.y
      };
      ::oglEsDrawBuffer(2, invVerts, GL_TRIANGLE_FAN, 4);
#endif
      ++pInvRect;
    }
    while(--n);
  }
  else
  {
    ::glClearColor(GLclampf(float(ODGETRED(bckgnd))  /255.),
                   GLclampf(float(ODGETGREEN(bckgnd))/255.),
                   GLclampf(float(ODGETBLUE(bckgnd)) /255.),
                   GLclampf(.0f));
    ::glClear(GL_COLOR_BUFFER_BIT);
  }
}

void OdGsOpenGLVectorizeDevice::updateScreen()
{
  clear();

  OdGsBaseVectorizeDevice::updateScreen();

  ::glFinish();
}

OdColorConverterCallback *OdGsOpenGLVectorizeDevice::getColorConverter()
{
  if (!properties().isNull() && properties()->has(OD_T("ColorConverter")))
    return OdColorConverterCallbackPtr(properties()->getAt(OD_T("ColorConverter")));
  return NULL;
}

ODCOLORREF OdGsOpenGLVectorizeDevice::colorConvert(ODCOLORREF color, bool bBackground)
{
  OdColorConverterCallback *pColorConverter = getColorConverter();
  if (pColorConverter && (!bBackground || pColorConverter->convertBackgroundColors()))
    return pColorConverter->convert(color);
  return color;
}

bool OpenGLParseExtension_base(const char *pExtName, const char *pExtensions)
{
  if (pExtName == NULL)
    return false;
  OdAnsiString extension(pExtName);
  if (extension.getLength() == 0 || extension.find(' ') != -1)
    return false;
  OdAnsiString extensions((!pExtensions) ? ((const char*)::glGetString(GL_EXTENSIONS)) : pExtensions);
  int start = 0, cur, end;
  for (;;)
  {
    cur = extensions.find(extension.c_str(), start);
    if (cur == -1)
      break;
    end = extension.getLength() + cur;
    if (cur == 0 || extensions[cur - 1] == ' ')
    {
      if (end == extensions.getLength() || extensions[end] == ' ')
      {
        return true;
      }
    }
    start = end;
  }
  return false;
}

// provide support for old accessors
bool OpenGLParseExtension(const char *pExtName)
{
  return OpenGLParseExtension_base(pExtName);
}

OdGsOpenGLVectorizeDevice::OpenGLExtSupport OdGsOpenGLVectorizeDevice::checkExtensionSupport(const char *pExtName, OpenGLExtension extIdx) const
{
  if (OpenGLParseExtension(pExtName))
  {
    return checkExtensionFuncSupport(extIdx) ? kExtSupported : kExtNotSupported;
  }
  return kExtNotSupported;
}

const char *OdGsOpenGLVectorizeDevice::getExtensionNameFromList(OdGsOpenGLVectorizeDevice::OpenGLExtension extIdx) const
{
  /* GL_EXT_bgra extension available in gl.h file in version 1.1, so we can don't care about
     additional and platform-dependent headers here. If anybody want to add own specific extension
     which is not a part of platform-specific headers, extension headers could be downloaded from
     OpenGL extensions registry site: http://www.opengl.org/registry/
     TD_GL.h maybe modified by adding this headers inside later.
  */
  switch (extIdx)
  {
    case OpenGLExtension_EXT_bgra:
#ifndef OD_OGL_ES
    return "GL_EXT_bgra";
#else
    /* GL_EXT_bgra extension is used not completely onto mobile platforms. It doesn't mean BGRA texture format compatibility,
       but frame buffer format. BGR doesn't supported correctly on my test environments. Unstable result on emulator and
       unable to create BGR texture on device. Look for texture loading code - additional fix there.                          */
    /* iOS-specific: GL_IMG_texture_format_BGRA8888 available instead on iOS-simulator. Probably this is incorectness.        */
#ifdef ANDROID
    return "GL_EXT_texture_format_BGRA8888";
#else
    return "GL_IMG_texture_format_BGRA8888";
#endif
#endif
    case OpenGLExtension_ARB_NPOT:
    return "GL_ARB_texture_non_power_of_two";
    case OpenGLExtension_ARB_Mirror:
    return "GL_ARB_texture_mirrored_repeat";
    case OpenGLExtension_ARB_Transpose:
    return "GL_ARB_transpose_matrix";
    case OpenGLExtension_EXT_BlendClr:
    return "GL_EXT_blend_color";
    case OpenGLExtension_EXT_Dxt:
    return "GL_EXT_texture_compression_s3tc";
    case OpenGLExtension_EXT_EdgeClamp:
    return "GL_EXT_texture_edge_clamp";
    case OpenGLExtension_ARB_VertexBuf:
    return "GL_ARB_vertex_buffer_object";
    case OpenGLExtension_ARB_PixelBuf:
    return "GL_ARB_pixel_buffer_object";
    case OpenGLExtension_ARB_FragProg:
    return "GL_ARB_fragment_program";
#if defined(ODA_WINDOWS)
    case OpenGLExtension_ARB_PixelFormat:
    return "WGL_ARB_pixel_format";
    case OpenGLExtension_ARB_PBuffer:
    return "WGL_ARB_pbuffer";
#endif
    default:
    return NULL;
  }
}

bool OdGsOpenGLVectorizeDevice::checkExtensionFuncSupport(OpenGLExtension extIdx) const
{
  switch (extIdx)
  {
    // No need funcs
    case OpenGLExtension_EXT_bgra:
    case OpenGLExtension_ARB_NPOT:
    case OpenGLExtension_ARB_Mirror:
    case OpenGLExtension_EXT_EdgeClamp:
    case OpenGLExtension_ARB_PixelBuf:
    return true;
    default: return false;
  }
}

void *OdGsOpenGLVectorizeDevice::getExtensionFuncPtrFromList(OpenGLExtFunc funcIdx) const
{
  const char *pName = NULL;
  switch (funcIdx)
  {
    case OpenGLExtFunc_ARB_LoadTransposeMatrixf:
      pName = "glLoadTransposeMatrixfARB";
    break;
    case OpenGLExtFunc_ARB_LoadTransposeMatrixd:
      pName = "glLoadTransposeMatrixdARB";
    break;
    case OpenGLExtFunc_ARB_MultTransposeMatrixf:
      pName = "glMultTransposeMatrixfARB";
    break;
    case OpenGLExtFunc_ARB_MultTransposeMatrixd:
      pName = "glMultTransposeMatrixdARB";
    break;
    case OpenGLExtFunc_EXT_BlendColor:
      pName = "glBlendColorEXT";
    break;
    case OpenGLExtFunc_EXT_CompressedTexImage2D:
      pName = "glCompressedTexImage2DARB";
    break;
    case OpenGLExtFunc_ARB_BindBuffer:
      pName = "glBindBufferARB";
    break;
    case OpenGLExtFunc_ARB_DeleteBuffers:
      pName = "glDeleteBuffersARB";
    break;
    case OpenGLExtFunc_ARB_GenBuffers:
      pName = "glGenBuffersARB";
    break;
    case OpenGLExtFunc_ARB_IsBuffer:
      pName = "glIsBufferARB";
    break;
    case OpenGLExtFunc_ARB_BufferData:
      pName = "glBufferDataARB";
    break;
    case OpenGLExtFunc_ARB_BufferSubData:
      pName = "glBufferSubDataARB";
    break;
    case OpenGLExtFunc_ARB_GetBufferSubData:
      pName = "glGetBufferSubDataARB";
    break;
    case OpenGLExtFunc_ARB_MapBuffer:
      pName = "glMapBufferARB";
    break;
    case OpenGLExtFunc_ARB_UnmapBuffer:
      pName = "glUnmapBufferARB";
    break;
    case OpenGLExtFunc_ARB_GetBufferParameteriv:
      pName = "glGetBufferParameterivARB";
    break;
    case OpenGLExtFunc_ARB_GetBufferPointerv:
      pName = "glGetBufferPointervARB";
    break;
    case OpenGLExtFunc_ARB_ActiveTexture:
      pName = "glActiveTextureARB";
    break;
    case OpenGLExtFunc_ARB_ProgramString:
      pName = "glProgramStringARB";
    break;
    case OpenGLExtFunc_ARB_BindProgram:
      pName = "glBindProgramARB";
    break;
    case OpenGLExtFunc_ARB_DeletePrograms:
      pName = "glDeleteProgramsARB";
    break;
    case OpenGLExtFunc_ARB_GenPrograms:
      pName = "glGenProgramsARB";
    break;
    case OpenGLExtFunc_ARB_ProgramLocalParameter4f:
      pName = "glProgramLocalParameter4fARB";
    break;
    case OpenGLExtFunc_ARB_ProgramLocalParameter4fv:
      pName = "glProgramLocalParameter4fvARB";
    break;
#if defined(ODA_WINDOWS)
    case OpenGLExtFunc_ARB_GetPixelFormatAttribiv:
      pName = "wglGetPixelFormatAttribivARB";
    break;
    case OpenGLExtFunc_ARB_GetPixelFormatAttribfv:
      pName = "wglGetPixelFormatAttribfvARB";
    break;
    case OpenGLExtFunc_ARB_ChoosePixelFormat:
      pName = "wglChoosePixelFormatARB";
    break;
    case OpenGLExtFunc_ARB_CreatePBuffer:
      pName = "wglCreatePbufferARB";
    break;
    case OpenGLExtFunc_ARB_GetPBufferDC:
      pName = "wglGetPbufferDCARB";
    break;
    case OpenGLExtFunc_ARB_ReleasePBufferDC:
      pName = "wglReleasePbufferDCARB";
    break;
    case OpenGLExtFunc_ARB_DestroyPBuffer:
      pName = "wglDestroyPbufferARB";
    break;
    case OpenGLExtFunc_ARB_QueryPBuffer:
      pName = "wglQueryPbufferARB";
    break;
#endif
    default: return NULL;
  }
  if (pName)
    return getExtensionFuncPtrFromListInt(pName);
  return NULL;
}

void *OdGsOpenGLVectorizeDevice::getExtensionFuncPtrFromListInt(const char * /*pFuncName*/) const
{
  return NULL;
}

bool OdGsOpenGLVectorizeDevice::isExtensionSupported(OpenGLExtension extIdx) const
{
  ODA_ASSERT(extIdx < OpenGLExtension_NExtensions);
  if (m_extAvail[extIdx] == kExtSupportUndetermined)
    m_extAvail[extIdx] = checkExtensionSupport(getExtensionNameFromList(extIdx), extIdx);
  return m_extAvail[extIdx] == kExtSupported;
}

void *OdGsOpenGLVectorizeDevice::getExtensionFunc(OpenGLExtFunc funcIdx) const
{
  ODA_ASSERT(funcIdx < OpenGLExtFunc_NFuncs);
  if (m_extFuncs[funcIdx] == NULL)
    m_extFuncs[funcIdx] = getExtensionFuncPtrFromList(funcIdx);
  return m_extFuncs[funcIdx];
}

double OdGsOpenGLVectorizeDevice::getRegenCoef() const
{
  return ExFPPrecisionFix::getRegenCoef(const_cast<OdGsOpenGLVectorizeDevice&>(*this),
                                        ExFPPrecisionFix::GsViewImplToFPFixCvtImpl<OdGsOpenGLVectorizeView>());
}

void OdGsOpenGLVectorizeDevice::onContextCreate()
{
  // If multithreading enabled we must pre-init all extensions which could be used during update process.
  if (supportMultithreading())
  {
    isExtensionSupported(OpenGLExtension_EXT_bgra);
    isExtensionSupported(OpenGLExtension_ARB_NPOT);
    isExtensionSupported(OpenGLExtension_ARB_Mirror);
    isExtensionSupported(OpenGLExtension_EXT_Dxt);
    isExtensionSupported(OpenGLExtension_ARB_FragProg);
  }
  // Aquire driver parameters
  ::glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize);
}

void OdGsOpenGLVectorizeDevice::onContextDelete()
{
  m_pTexManager->clear();
}

OdGiMaterialTextureManager *OdGsOpenGLVectorizeDevice::materialTextureManager()
{
  return m_pTexManager.get();
}

void OdGsOpenGLVectorizeDevice::glLoadTransposedMatrix(OdGeMatrix3d &x)
{
#ifndef OD_OGL_ES
#ifdef GL_ARB_transpose_matrix
  if (isExtensionSupported(OpenGLExtension_ARB_Transpose))
  {
    ((OdGLFn_LoadTransposeMatrixd_ARB)getExtensionFunc(OpenGLExtFunc_ARB_LoadTransposeMatrixd))((const GLdouble*)&x);
  }
  else
  {
#endif
    ::glLoadMatrixd((const GLdouble*)&x.transposeIt());
#ifdef GL_ARB_transpose_matrix
  }
#endif
#else
  GLfloat fltMat[4][4];
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      fltMat[i][j] = (GLfloat)x(j, i);
    }
  }
  ::glLoadMatrixf((const GLfloat*)fltMat);
#endif
}

typedef void (TD_STD_CALL * GluTessCallbackFn)();

OdGsOpenGLVectorizeView::OdGsOpenGLVectorizeView()
  : m_bFill(false)
  , m_bTexCoords(false)
  , m_bIsVertsNormals(false)
  , m_bFaceNormalsRequired(false)
  , m_bAcceptColorChange(true)
  , m_bRasterImage(false)
  , m_bForceAlternativeHlt(false)
  , m_bProcessingTTF(false)
  , m_bDontShade(false)
  , m_bFading(false)
  , m_nCurFading(0)
  , m_bBackfacesDiscarded(false)
  , m_bDrawTransparency(true)
  , m_bNoColorMod(false)
  , m_bCheckFaceMaterials(false)
  , m_numLights(0)
  , m_pCurMetafile(NULL)
  , m_pColorConverter(NULL)
  , m_bConvertBackgroundColor(true)
  , m_fMaxLineWidth(1.0f)
  , m_psLinetypeDefs(::odgiGetPsLinetypesManager(OdGiPsLinetypes::kOpenGLDefs).getOpenGLDefinitions())
  , m_psFillstyleDefs(::odgiGetPsFillstylesManager().getOpenGLDefinitions())
{
  OdGiGeometrySimplifier::setDrawContext(OdGsBaseVectorizer::drawContext());
}

// AMark fix #5167
class OdOpenGLDynamicSubList
{
  // Full list features
  OdGeVector3d m_viewDirection; // Differ instructs to recalc line cache
  double m_lwScaleCoef;
  OdUInt32 m_uFlags;
  // Cache for each polyline
  struct PrimaryCacheEntry
  {
    PrimaryCacheEntry()
      : m_nPxSize(-1)
      , m_lwCoef(0.0)
    {
    }

    OdOpenGLLwdSetting m_lineWeight;
    OdInt32 m_nPxSize;
    double m_dSize; // Size in depth / 2
    double m_dSize2; // Size in depth
    double m_lwCoef; // Used to detect MS or PS lineweights vectorization style
    OdUInt8 m_pColor[4]; // R, G, B, A
    bool m_bHighlight;
    OdGsMarker m_gsMark;
    bool m_bSelGeom, m_bSelIgnore;
    OdUInt32 m_uFrom, m_uNLines; // ID in line cache
  };
  OdArray<PrimaryCacheEntry> m_primCache;
  // Cache for each polyline segment
  struct LineCacheEntry
  {
    OdGePoint3d m_points[2];
    //double len; // Size in depth
    OdGeVector3d dir; // Direction * len
    OdGeVector3d side; // Side * len
  };
  OdArray<LineCacheEntry> m_lineCache;
public:
  OdOpenGLDynamicSubList()
    : m_lwScaleCoef(1.0)
    , m_uFlags(0)
  {
  }
  ~OdOpenGLDynamicSubList()
  {
  }

  inline void appendDynamicLwLines(const OdGePoint3d *pPoints, OdInt32 nPoints, const OdOpenGLLwdSetting &lineWeight, ODCOLORREF cColor, OdGsMarker gsMark, bool bSelGeom, bool bSelIgnore, bool bHighlight)
  {
    // appendDynamicLwLinesMacro
#define appendDynamicLwLinesMacro \
    if (nPoints < 2) \
      return; \
    bool prevPrim = false; \
    if (!m_primCache.isEmpty()) \
    { \
      PrimaryCacheEntry &lastPrim = m_primCache.last(); \
      if (lastPrim.m_lineWeight == lineWeight) \
      { \
        prevPrim = (ODRGBA(lastPrim.m_pColor[0], lastPrim.m_pColor[1], lastPrim.m_pColor[2], lastPrim.m_pColor[3]) == cColor) && (lastPrim.m_bHighlight == bHighlight) && (lastPrim.m_gsMark == gsMark) && \
                    (lastPrim.m_bSelGeom == bSelGeom) && (lastPrim.m_bSelIgnore == bSelIgnore); \
      } \
    } \
    OdUInt32 primId; \
    if (prevPrim) \
    { \
      primId = m_primCache.size() - 1; \
      m_primCache.last().m_uNLines += nPoints - 1; \
    } \
    else \
    { \
      PrimaryCacheEntry primCE; \
      primCE.m_lineWeight = lineWeight; \
      primCE.m_bHighlight = bHighlight; \
      primCE.m_gsMark = gsMark; \
      primCE.m_pColor[0] = ODGETRED(cColor); \
      primCE.m_pColor[1] = ODGETGREEN(cColor); \
      primCE.m_pColor[2] = ODGETBLUE(cColor); \
      primCE.m_pColor[3] = ODGETALPHA(cColor); \
      primCE.m_bSelGeom = bSelGeom; \
      primCE.m_bSelIgnore = bSelIgnore; \
      primId = m_primCache.size(); \
      primCE.m_uFrom = m_lineCache.size(); \
      primCE.m_uNLines = OdUInt32(nPoints) - 1; \
      m_primCache.append(primCE); \
    } \
    OdUInt32 uDataFrom = m_lineCache.size(); \
    m_lineCache.resize(m_lineCache.size() + nPoints - 1); \
    LineCacheEntry *lineCE = m_lineCache.asArrayPtr() + uDataFrom;
    // appendDynamicLwLinesMacro end
    appendDynamicLwLinesMacro // See code above
    for (OdInt32 i = 0; i < nPoints - 1; i++, lineCE++)
    {
      lineCE->m_points[0] = pPoints[i];
      lineCE->m_points[1] = pPoints[i + 1];
    }
  }

  inline void appendDynamicLwLines(const OdGePoint3d *pPoints, const OdInt32* vertexIndexList, OdInt32 nPoints, const OdOpenGLLwdSetting &lineWeight, ODCOLORREF cColor, OdGsMarker gsMark, bool bSelGeom, bool bSelIgnore, bool bHighlight)
  {
    appendDynamicLwLinesMacro // See code in previous function
    for (OdInt32 i = 0; i < nPoints - 1; i++, lineCE++)
    {
      lineCE->m_points[0] = pPoints[vertexIndexList[i]];
      lineCE->m_points[1] = pPoints[vertexIndexList[i + 1]];
    }
  }

  inline void play(OdGsOpenGLVectorizeView *pOGLView, bool bHighlighted, OdOpenGLCheckMarkIface *pCheckMark)
  {
    if (m_primCache.isEmpty())
      return; // [no data]
    GLboolean bLighting = ::glIsEnabled(GL_LIGHTING);
    if (bLighting)
      ::glDisable(GL_LIGHTING);

    // m_uFlags:
    // 1 - PS
    // 2 - PxSize changed
    // 4 - Lwd scale coef changed
    // 8 - Target changed
    // 16 - lwCoef changed
    // 64 - scale coef and target already calculated
    // 128 - Cache is not calculated
    if ((m_uFlags & 1) == 1)
      m_uFlags = 1;
    else
      m_uFlags = 0;
    OdUInt32 iflags;
    pOGLView->highlight(bHighlighted);
    pOGLView->onHighlightModified(false, true);
    for (OdUInt32 ip = 0; ip < m_primCache.size(); ip++)
    {
      PrimaryCacheEntry &pce = m_primCache.at(ip);
      if (pce.m_bSelGeom) continue;
      iflags = m_uFlags;
      OdInt32 pxSize = pOGLView->lwdSettingToPixels(pce.m_lineWeight);
      if (!bHighlighted && pce.m_bHighlight != pOGLView->isHighlighted())
      {
        pOGLView->highlight(pce.m_bHighlight);
        pOGLView->onHighlightModified(false, true);
      }
      if (pCheckMark)
      {
        bHighlighted = pCheckMark->metafileCheckMarker((OdOpenGLGsMarker)pce.m_gsMark, bHighlighted);
        if (bHighlighted != pOGLView->isHighlighted())
        {
          pOGLView->highlight(bHighlighted);
          pOGLView->onHighlightModified(false, true);
        }
      }
      if (pxSize < 2)
      {
        // The line can be drawed without width and caps
        ::glColor4ubv(pce.m_pColor);
        /*::glBegin(GL_LINE_STRIP);
          const LineCacheEntry *lce = m_lineCache.asArrayPtr() + pce.m_uFrom;
          ::glVertex3dv((const GLdouble*)lce->m_points);
          ::glVertex3dv((const GLdouble*)(lce->m_points + 1));
          for (OdUInt32 il = pce.m_uFrom + 1; il < pce.m_uFrom + pce.m_uNLines; il++)
          {
            lce = m_lineCache.asArrayPtr() + il;
            ::glVertex3dv((const GLdouble*)(lce->m_points + 1));
          }
        ::glEnd();*/
#ifndef OD_OGL_ES
        ::glBegin(GL_LINES);
          const LineCacheEntry *lce = m_lineCache.asArrayPtr() + pce.m_uFrom;
          for (OdUInt32 il = pce.m_uFrom; il < pce.m_uFrom + pce.m_uNLines; il++, lce++)
          {
            ::glVertex3dv((const GLdouble*)(lce->m_points));
            ::glVertex3dv((const GLdouble*)(lce->m_points + 1));
          }
        ::glEnd();
#else
        const LineCacheEntry *lce = m_lineCache.asArrayPtr() + pce.m_uFrom;
        OdArray<GLfloat> fltVerts(pce.m_uNLines * 3 * 2, 1);
        fltVerts.resize(pce.m_uNLines * 3 * 2);
        GLfloat *pOut = fltVerts.asArrayPtr();
        for (OdUInt32 il = pce.m_uFrom; il < pce.m_uFrom + pce.m_uNLines; il++, lce++, pOut += 6)
        {
          pOut[0] = (GLfloat)lce->m_points[0].x;
          pOut[1] = (GLfloat)lce->m_points[0].y;
          pOut[2] = (GLfloat)lce->m_points[0].z;
          pOut[3] = (GLfloat)lce->m_points[1].x;
          pOut[4] = (GLfloat)lce->m_points[1].y;
          pOut[5] = (GLfloat)lce->m_points[1].z;
        }
        ::oglEsDrawBuffer(3, fltVerts.getPtr(), GL_LINES, pce.m_uNLines * 2);
#endif
      }
      else
      {
        if (pce.m_nPxSize == -1)
        {
          iflags |= 128;
          pce.m_nPxSize = pxSize;
        }
        else if (pce.m_nPxSize != pxSize)
        {
          iflags |= 2;
          pce.m_nPxSize = pxSize;
        }
        // Test cache is need to be rebuilded
        if ((iflags & 64) == 0)
        {
          OdGeVector3d target = pOGLView->target() - pOGLView->position();
          double scaleCoef = pOGLView->lineweightToDcScale();
          if (scaleCoef != m_lwScaleCoef)
          {
            iflags |= 4; m_uFlags |= 4;
            m_lwScaleCoef = scaleCoef;
          }
          if (target != m_viewDirection)
          {
            iflags |= 8; m_uFlags |= 8;
            m_viewDirection = target;
          }
          m_uFlags |= 64;
        }
        if ((iflags & (2 | 4)) == (2 | 4))
        {
          iflags |= 1; m_uFlags |= 1;
        }
        if ((!((iflags & 1) == 1 && (iflags & 8) == 0) && // No PS 
            ((iflags & (2 | 4)) == 0)) || ((iflags & 128) == 128)) // MS
        {
          // Regen cache
          OdGeMatrix3d m3d;
          OdGeVector3d vmu, vmn;
          if (pOGLView->isModelCacheEnabled())
          {
            m3d = pOGLView->worldToDeviceMatrix();
            vmu = pOGLView->getEyeToWorldTransform().getCsYAxis().normal();
            vmn = pOGLView->getEyeToWorldTransform().getCsZAxis().normal();
          }
          else
          {
            m3d = pOGLView->eyeToScreenMatrix();
            vmu = OdGeVector3d::kYAxis;
            vmn = OdGeVector3d::kZAxis;
          }
          LineCacheEntry *pPt = m_lineCache.asArrayPtr() + pce.m_uFrom;
          OdGePoint3d pt1 = pPt->m_points[0], pt2 = pt1 + /*pOGLView->upVector()*/vmu;
          pt1.transformBy(m3d); pt2.transformBy(m3d);
          double lwCft = (pt2 - pt1).length();
          if (lwCft != pce.m_lwCoef)
          {
            iflags |= 16;
            pce.m_lwCoef = lwCft;
          }
          if (!((iflags & (8 | 16)) == 0))
          {
            lwCft = double(pce.m_nPxSize) / lwCft;
            pce.m_dSize2 = lwCft;
            pce.m_dSize = pce.m_dSize2 * 0.5;
            OdGeVector3d vcn = vmn/*m_viewDirection*/;
            if (pOGLView->isModelCacheEnabled() && pOGLView->metafileTransformOverrided())
            {
              vcn.transformBy(pOGLView->worldToMetafileMatrix());
            }
            for (OdUInt32 i = 0; i < pce.m_uNLines; i++, pPt++)
            {
              OdGePoint3d pv1(pPt->m_points[0]), pv2(pPt->m_points[1]);
              OdGeVector3d vn = (pv1 != pv2) ? ((pv2 - pv1).normal()) : (OdGeVector3d(1.0, 0.0, 0.0)), vu;
              if (vcn.isZeroLength())
              {
                vu.set(-vn.y, vn.x, 0.0);
              }
              else
              {
                if (-vn != vcn)
                {
                  vu = vn.crossProduct(vcn.normal());
                  if (vu.isZeroLength())
                    vu.set(-vn.y, vn.x, 0.0);
                  else
                    vu = vu.normal();
                }
                else
                {
                  vu.set(-vn.y, vn.x, 0.0);
                }
              }
              pPt->dir = vn * pce.m_dSize;
              pPt->side = vu * pce.m_dSize;
            }
          }
        }
        OdUInt32 nrs = pce.m_uNLines;
        const LineCacheEntry *pRl = m_lineCache.asArrayPtr() + pce.m_uFrom;
        while (nrs > 0)
        {
          pOGLView->renderLineCap(pce.m_nPxSize, pRl->m_points, pRl->dir, pRl->side, pce.m_pColor);
          nrs--;
          pRl++;
        }
#ifndef OD_OGL_ALTERNATIVEHLT
        if ((bHighlighted || pce.m_bHighlight) && !pOGLView->forceAlternativeHlt())
        {
          ODCOLORREF bkgndColor(pOGLView->baseDevice()->getPaletteBackground());
          ::glColor4ub(ODGETRED(bkgndColor), ODGETGREEN(bkgndColor), ODGETBLUE(bkgndColor), 0xFF);
          ::glBegin(GL_LINES);
            const LineCacheEntry *lce = m_lineCache.asArrayPtr() + pce.m_uFrom;
            for (OdUInt32 il = pce.m_uFrom; il < pce.m_uFrom + pce.m_uNLines; il++, lce++)
            {
              ::glVertex3dv((const GLdouble*)(lce->m_points));
              ::glVertex3dv((const GLdouble*)(lce->m_points + 1));
            }
          ::glEnd();
        }
#endif
      }
    }
    if (bLighting)
      ::glEnable(GL_LIGHTING);
  }

  void play4Select(OdGsOpenGLVectorizeView * /*pOGLView*/, OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx, OdOpenGLCheckMarkIface *pCheckMark) const
  {
    if (m_primCache.isEmpty())
      return; // [no data]
    OdGsMarker prevMark = kNullSubentIndex;
    for (OdUInt32 ip = 0; ip < m_primCache.size() && !pCtx->regenAbort(); ip++)
    {
      const PrimaryCacheEntry &pce = m_primCache.at(ip);
      if (pCheckMark)
      {
        if (pce.m_bSelIgnore) continue;
        if (prevMark != pce.m_gsMark)
        {
          prevMark = pce.m_gsMark;
          pCheckMark->metafileSelectMarker((OdOpenGLGsMarker)prevMark);
        }
      }
      const LineCacheEntry *lce = m_lineCache.asArrayPtr() + pce.m_uFrom;
      for (OdUInt32 il = pce.m_uFrom; il < pce.m_uFrom + pce.m_uNLines; il++, lce++)
      {
        pGeom->polylineProc(2, lce->m_points);
      }
    }
  }

  void finalize()
  {
    m_primCache.setPhysicalLength(m_primCache.size());
    m_lineCache.setPhysicalLength(m_lineCache.size());
  }
};

OdOpenGLDynamicSubListProc::OdOpenGLDynamicSubListProc()
  : m_pDynSubList(NULL)
{
}

OdOpenGLDynamicSubListProc::~OdOpenGLDynamicSubListProc()
{
  clear();
}

void OdOpenGLDynamicSubListProc::clear()
{
  if (m_pDynSubList)
  {
    delete m_pDynSubList;
    m_pDynSubList = NULL;
  }
}

/*inline*/ void OdOpenGLDynamicSubListProc::end()
{
  if (m_pDynSubList)
    m_pDynSubList->finalize();
}

/*inline*/ void OdOpenGLDynamicSubListProc::play(OdGsOpenGLVectorizeView *pOGLView, bool bHighlighted, OdOpenGLCheckMarkIface *pCheckMark) const
{
  if (m_pDynSubList)
    m_pDynSubList->play(pOGLView, bHighlighted, pCheckMark);
}

/*inline*/ void OdOpenGLDynamicSubListProc::play4Select(OdGsOpenGLVectorizeView *pOGLView, OdGiConveyorGeometry *pGeom, OdGiConveyorContext *pCtx, OdOpenGLCheckMarkIface *pCheckMark) const
{
  if (m_pDynSubList)
    m_pDynSubList->play4Select(pOGLView, pGeom, pCtx, pCheckMark);
}

/*inline*/ void OdOpenGLDynamicSubListProc::appendDynamicLwLines(const OdGePoint3d *pPoints, OdInt32 nPoints, const OdOpenGLLwdSetting &lineWeight, ODCOLORREF cColor, OdGsMarker gsMark, bool bSelGeom, bool bSelIgnore, bool bHighlight)
{
  if (m_pDynSubList == NULL)
    m_pDynSubList = new OdOpenGLDynamicSubList();
  m_pDynSubList->appendDynamicLwLines(pPoints, nPoints, lineWeight, cColor, gsMark, bSelGeom, bSelIgnore, bHighlight);
}

/*inline*/ void OdOpenGLDynamicSubListProc::appendDynamicLwLines(const OdGePoint3d *pPoints, const OdInt32* vertexIndexList, OdInt32 nPoints, const OdOpenGLLwdSetting &lineWeight, ODCOLORREF cColor, OdGsMarker gsMark, bool bSelGeom, bool bSelIgnore, bool bHighlight)
{
  if (m_pDynSubList == NULL)
    m_pDynSubList = new OdOpenGLDynamicSubList();
  m_pDynSubList->appendDynamicLwLines(pPoints, vertexIndexList, nPoints, lineWeight, cColor, gsMark, bSelGeom, bSelIgnore, bHighlight);
}
// end of #5167

class OdOpenGLList : public OdRxObject, public OdOpenGLDynamicSubListProc
{
  GLint m_list;
public:
  inline OdOpenGLList() : m_list(0) {}

  ~OdOpenGLList()
  {
    clear();
  }

  inline void clear()
  {
    if (m_list)
    {
      ::glDeleteLists(m_list, 1);
      m_list = 0;
    }
  }
  inline void clearWSL()
  {
    clear();
    OdOpenGLDynamicSubListProc::clear();
  }

  inline void glNewList()
  {
    if(!m_list)
      m_list = ::glGenLists(1);
    ::glNewList(m_list, GL_COMPILE);
#ifdef OD_OGL_ES
    throw OdError(eInvalidInput);
#endif
  }

  inline void begin()
  {
    glNewList();
  }

  inline void end()
  {
    ::glEndList();
    OdOpenGLDynamicSubListProc::end();
  }

  inline void play(OdGsOpenGLVectorizeView *pOGLView) const
  {
    ::glCallList(m_list);
    OdOpenGLDynamicSubListProc::play(pOGLView);
  }
};

void OdGsOpenGLVectorizeView::shell(OdInt32 nVertices,
                                    const OdGePoint3d* vertexList,
                                    OdInt32 faceListSize,
                                    const OdInt32* faceList,
                                    const OdGiEdgeData* edgeData,
                                    const OdGiFaceData* faceData,
                                    const OdGiVertexData* vertexData)
{
  bool bBackfacesDiscarded = false;
  if ((view().mode() >= kFlatShaded) && ::odExDiscardBackfaces(*this, device()->discardBackFaces()) &&
#ifndef OD_OGL_ALTERNATIVEHLT
      (forceAlternativeHlt() || (!currentHighlightBranch() || device()->discardBackFaces())) &&
#endif
      ((faceData && faceData->normals()) || (vertexData && vertexData->normals())) && !isOutputSuppressed())
  {
    if (m_pCurMetafile == NULL)
      onTraitsModified(); // AMark fix #3736: Support for discardBackFaces in OpenGL GS is broken
    // ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    ::glEnable(GL_CULL_FACE);
    ::glCullFace(GL_BACK);
    bBackfacesDiscarded = true;
  }
  OdGsBaseVectorizer::shell(nVertices, vertexList, faceListSize, faceList, edgeData, faceData, vertexData);
  if(bBackfacesDiscarded)
  {
    // ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    ::glDisable(GL_CULL_FACE);
  }
}

void OdGsOpenGLVectorizeView::polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                                            const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                                            const OdGeVector3d* pNormals, const OdGeVector3d* pExtrusions,
                                            const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize)
{
  if (nPointSize >= 1)
    draw_lineweight(nPointSize, false);
  OdGiGeometrySimplifier::polypointProc(numPoints, vertexList, pColors, pTransparency, pNormals, pExtrusions, pSubEntMarkers, nPointSize);
  if (nPointSize >= 1)
    draw_lineweight(nPointSize, true);
}

OdRxObjectPtr OdGsOpenGLVectorizeView::newGsMetafile()
{
  return OdRxObjectPtr(OdRxObjectImpl<OdOpenGLList>::createObject());
}

void OdGsOpenGLVectorizeView::beginMetafile(OdRxObject* pMetafile)
{
  OdGsBaseMaterialVectorizer::beginMetafile(pMetafile);
  static_cast<OdOpenGLList*>(pMetafile)->begin();
  m_pCurMetafile = pMetafile;
}

void OdGsOpenGLVectorizeView::endMetafile(OdRxObject* pMetafile)
{
  m_pCurMetafile = NULL;
  static_cast<OdOpenGLList*>(pMetafile)->end();
  OdGsBaseMaterialVectorizer::endMetafile(pMetafile);
  if (!validateMetafileExtents())
    static_cast<OdOpenGLList*>(pMetafile)->clearWSL();
}

void OdGsOpenGLVectorizeView::playMetafile(const OdRxObject* pMetafile)
{
  resetFading(isFaded());
  static_cast<const OdOpenGLList*>(pMetafile)->play(this);
}

void OdGsOpenGLVectorizeView::pushMetafileTransform(const OdGeMatrix3d& mtx, OdUInt32 uFlags)
{
  pushMetafileTransformForUpdate(*this, mtx, uFlags);
  if (pushMetafileTransformForDisplay(*this, mtx, uFlags))
  {
    ::glMatrixMode(GL_MODELVIEW);
    OdGeMatrix3d x = mfTransformStack().getCompositeTransform();
    device()->glLoadTransposedMatrix(x);
  }
}

void OdGsOpenGLVectorizeView::popMetafileTransform(OdUInt32 uFlags)
{
  popMetafileTransformForUpdate(*this, uFlags);
  if (popMetafileTransformForDisplay(*this, uFlags))
  {
    ::glMatrixMode(GL_MODELVIEW);
    OdGeMatrix3d x = mfTransformStack().getCompositeTransform();
    device()->glLoadTransposedMatrix(x);
  }
}

bool OdGsOpenGLVectorizeView::isModelCacheEnabled() const
{
  return view().cachedDrawables() > 0;
}

void OdGsOpenGLVectorizeView::resetFading(bool bFlag)
{
  if ((m_bFading != bFlag) || (bFlag && (m_nCurFading != OdUInt8(fadingIntensity()))))
  {
    m_bFading = bFlag;
    // This is good idea, but it can't live with local transparency
//#ifdef GL_EXT_blend_color
//    if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_BlendClr))
//    {
//      if (bFlag)
//      {
//        ODCOLORREF bkgndColor = device()->getBackgroundColor();
//        ::glEnable(GL_BLEND);
//        ((OdGLFn_BlendColor_EXT)device()->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_EXT_BlendColor))
//          ((GLclampf)ODGETRED(bkgndColor) / 255, (GLclampf)ODGETGREEN(bkgndColor) / 255, (GLclampf)ODGETBLUE(bkgndColor) / 255,
//           (GLclampf)(100 - fadingIntensity()) * 0.01f);
//        ::glBlendFunc(CONSTANT_ALPHA_EXT, ONE_MINUS_CONSTANT_ALPHA_EXT);
//      }
//      else
//      {
//        ::glDisable(GL_BLEND);
//      }
//    }
//#endif
#ifndef OD_OGL_ALTERNATIVEHLT
    if (forceAlternativeHlt())
      return;
    if (bFlag)
    {
      OdGeExtents3d we;
      GsViewImplHelper::getExtents(*this, kGsMainOverlay, we); // in output coordinates - i. e. world
      double zNear, zFar;
      if (!we.isValidExtents())
        zNear = zFar = 0.0;
      else
      {
        we.transformBy(worldToEyeMatrix());
        zNear = we.minPoint().z; zFar = we.maxPoint().z;
      }
      if (zFar < zNear)
        std::swap(zNear, zFar);
      if (OdEqual(zNear, zFar))
        zNear -= 1.0, zFar += 1.0;
      if (zNear < -1e20) zNear = -1e20;
      if (zFar > 1e20) zFar = 1e20;
      double zScale = zFar - zNear;
      //double zMid = zNear + zScale * 0.5;
      double coef = (m_nCurFading = (OdUInt8)fadingIntensity()) * 0.01; // [0-1]
      double fogNear = zNear - zScale * coef * 255;
      double fogFar = zFar + zScale * (1.0 - coef) * 255;
      if (fogFar < 0.0)
        fogNear = -fogNear, fogFar = -fogFar;
      ODCOLORREF bkgndColor = device()->getPaletteBackground();
      ::glEnable(GL_FOG);
      ::glFogi(GL_FOG_MODE, GL_LINEAR);
      ::glFogf(GL_FOG_START, ((fogNear < -1e20f) ? -1e20f : ((GLfloat)fogNear)));
      ::glFogf(GL_FOG_END, ((fogFar > 1e20f) ? 1e20f : ((GLfloat)fogFar)));
      const GLfloat mult = 1.0f / 255;
      GLfloat fogColor[4] = { mult * ODGETRED(bkgndColor), mult * ODGETGREEN(bkgndColor), mult * ODGETBLUE(bkgndColor), 1.0f };
      ::glFogfv(GL_FOG_COLOR, fogColor);
    }
    else
    {
      ::glDisable(GL_FOG);
      m_nCurFading = 0;
    }
#endif
  }
}

void OdGsOpenGLVectorizeView::alternativeHighlight(bool bHLT)
{
  bool curState = ::glIsEnabled(GL_FOG) != GL_FALSE;
  if (curState != bHLT)
  {
    if (bHLT)
    {
      ::glEnable(GL_FOG);
      ::glFogi(GL_FOG_MODE, GL_LINEAR);
      ::glFogf(GL_FOG_START, -1e20f);
      ::glFogf(GL_FOG_END, 1e20f);
      const ODCOLORREF hltColor = (ODCOLORREF)device()->alternativeHltColor();
      const GLfloat fogColor[4] = { (GLfloat)ODGETRED(hltColor) / 255, (GLfloat)ODGETGREEN(hltColor) / 255,
                                    (GLfloat)ODGETBLUE(hltColor) / 255, 1.0f };
      ::glFogfv(GL_FOG_COLOR, fogColor);
    }
    else
    {
      ::glDisable(GL_FOG);
    }
  }
}

OdGsOpenGLVectorizeView::~OdGsOpenGLVectorizeView()
{
}

// Returns transform from DC divided coordinates m_dcLowerLeft:m_dcUpperRight to 0,0:1,1 DC
inline OdGeMatrix3d OdGsOpenGLVectorizeView::dividedDcToNormalizedDc() const
{
  return OdGeMatrix3d::scaling(OdGeScale3d(1./(view().dcUpperRight().x-view().dcLowerLeft().x), 1./(view().dcUpperRight().y-view().dcLowerLeft().y),1.))
    * OdGeMatrix3d::translation(OdGeVector3d(-view().dcLowerLeft().x, -view().dcLowerLeft().y, 0.));
}

inline void OdGsOpenGLVectorizeView::getGLScreenRect(double& left, double& bottom, double& width, double& height)
{
  left   = view().dcScreenMin().x;
  bottom = view().dcScreenMax().y;
  width  = (view().dcScreenMax().x - view().dcScreenMin().x); // user top   - user bottom
  height = (view().dcScreenMin().y - view().dcScreenMax().y); // user right - user left

  left   = left   + view().dcLowerLeft().x * width;
  bottom = bottom + view().dcLowerLeft().y * height;
  width  = (view().dcUpperRight().x - view().dcLowerLeft().x) * width;
  height = (view().dcUpperRight().y - view().dcLowerLeft().y) * height;

  // get client screen rect (take into account DC subdividing)
  OdGePoint3d ll, ur;
  view().getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
  OdGeMatrix3d x = view().screenMatrix();
  ll.transformBy(x);
  ur.transformBy(x);

  // In OpenGL - viewport origin is lower-left corner Y goes UP;
  // in windows - window origin is upper-left corner Y goes DOWN;
  // so flip Y (relative to overall device position/size)
  const OdGsDCRect& deviceRc = device()->m_outputRect;
  OdGePoint3d center((deviceRc.m_max.x + deviceRc.m_min.x) / 2., (deviceRc.m_max.y + deviceRc.m_min.y) / 2., 0.);

  x.setToScaling(OdGeScale3d(1.,-1.,1.), center);
  ll.transformBy(x);
  ur.transformBy(x);

  left   = ll.x;
  bottom = ll.y;
  width  = ur.x - ll.x;
  height = ur.y - ll.y;
}

#define ROUND(X) long(X+.5)

bool OdGsOpenGLVectorizeView::usesDept(double* pMinDeptSupported, double* pMaxDeptSupported) const
{
  if(pMinDeptSupported)
    *pMinDeptSupported = -1e20;
  if(pMaxDeptSupported)
    *pMaxDeptSupported =  1e20;
  return true;
}

bool OdGsOpenGLVectorizeView::sceneDept(double& zNear, double& zFar) const
{
  if(((view().mode() == k2DOptimized) && (view().isFrontClipped()==view().isBackClipped())))
  {
    zFar  = -1e20;
    zNear =  1e20;
    return true;
  }
#ifdef EXPERIMENTAL_VIEWIMPLCHG
  bool bRVal;
  if (isModelCacheEnabled())
    bRVal = OdGsViewImpl::sceneDept(zNear, zFar);
  else
    bRVal = OdGsBaseVectorizeView::sceneDept(zNear, zFar);
  if (bRVal)
  {
    zNear += targetBalanceLength();
    zFar += targetBalanceLength();
  }
  return bRVal;
#else
  if (isModelCacheEnabled())
    return OdGsViewImpl::sceneDept(zNear, zFar);
  else
    return OdGsBaseVectorizeView::sceneDept(zNear, zFar);
#endif
}

OdGeMatrix3d OdGsOpenGLVectorizeView::projectionMatrixGL(bool bNoDept)
{
  OdGeMatrix3d mx = projectionCorrection() * view().projectionMatrix((bNoDept) ? kProjectionIncludeAllNoDept : kProjectionIncludeAll);
  return mx;
}

bool clip1d(double& start, double& len, const double& clip_len, double& scale, double& trans)
{
  double c0 = 0.;
  double c1 = clip_len;
  ODA_ASSERT(c0 < c1);
  double x0;
  double x1;
  if(len > 0.)
  {
    x0 = start;
    x1 = start + len;
  }
  else
  {
    x0 = start + len;
    x1 = start;
  }
  if(x1 <= c0 || x0 >= c1)
  {
    // clipped out
    return false;
  }
  else if(x0 < c0 || x1 > c1)
  {
    // clipped
    double cx0 = odmax(c0, x0);
    double cx1 = odmin(c1, x1);

    // then correct projection:
    scale = len/(cx1-cx0);
    if(len > 0.)
      trans = (x0-cx0)/len;
    else
      trans = (x1-cx1)/len;

    start = cx0;
    len   = cx1-cx0;
  }
  else
  {
    // not clipped
    scale = len/(x1-x0);
    trans = 0.;

    start = x0;
    len   = x1-x0;
  }
  return true;
}

//static void initArrays(OdInt32 nbVertex,
//                       const OdGePoint3d* pVertexList,
//                       const OdGiVertexData* pVertexData,
//                       OdUInt8Array &colors);

void OdGsOpenGLVectorizeView::loadViewport()
{
  // Prepare info required for render
  GLint lwRange[4] = { 1, 1, 1, 1 };
  ::glGetIntegerv(GL_SMOOTH_LINE_WIDTH_RANGE, lwRange);
  ::glGetIntegerv(GL_SMOOTH_POINT_SIZE_RANGE, lwRange + 2);
  m_fMaxLineWidth = (GLfloat)(odmin(lwRange[1], lwRange[3]));

  double left, bottom, width, height;
  getGLScreenRect(left, bottom, width, height);

  OdGeScale3d scale(2., 2., 2.);
  /*
  GLint nDeptBits = -1;
  ::glGetIntegerv(GL_DEPTH_BITS, &nDeptBits);
  if(nDeptBits > 0)
  {
    scale.sz += (1. / double(pow(2, nDeptBits)));
  }
  */

  // Transformation GS's normalized coordinates 0,0:1,1 to GL's normalized coordinates -1,-1:1,1
  // scale.sz serves to move far clipping plane just a little away to include objects on that plane.
  m_correction = OdGeMatrix3d::scaling(scale, OdGePoint3d(1.,1.,1.));

  double windowWidth = device()->width();
  double windowHeight = device()->height();

  double sx, tx, sy, ty;
  bool bFrontFaceCW = false;
  if(clip1d(left, width, windowWidth, sx, tx) && clip1d(bottom, height, windowHeight, sy, ty))
  {
    if(sx < 0. || sy < 0.)
    {
      if(sx < 0.)
      {
        scale.sx = -1.0;
        sx = -sx;
        bFrontFaceCW = !bFrontFaceCW;
      }
      else
      {
        scale.sx = 1.0;
      }
      if(sy < 0.)
      {
        scale.sy = -1.0;
        sy = -sy;
        bFrontFaceCW = !bFrontFaceCW;
      }
      else
      {
        scale.sy = 1.0;
      }
      scale.sz = 1.0;
      m_correction.preMultBy(scale);
    }

    m_correction *= scale.set(sx, sy, 1.);
    m_correction *= OdGeMatrix3d::translation(OdGeVector3d(tx, ty, 0.));
  }
  else
  {
    return;
  }

  m_correction *= dividedDcToNormalizedDc();

  OdGeMatrix3d x;
  OdGsDCRectArray invRects = view().invalidRects();
  OdGePoint2dArray m_nrcPoints;
  OdIntArray m_nrcCounts;
  view().viewportClipRegion(m_nrcCounts, m_nrcPoints);
  unsigned int n = invRects.size();
  unsigned int m = m_nrcCounts.size();

  if(n || m) // Non rectangular viewport clipping or invalid area clipping
  {
    // screen to GL:
    ::glViewport(0, 0, baseDevice()->width(), baseDevice()->height());

    OdGsDCRect dcrc = baseDevice()->outputRect();
    //x(0,0) = (dcrc.m_max.x - dcrc.m_min.x);
    //x(1,1) = (dcrc.m_max.y - dcrc.m_min.y);
    //x(0,3) = dcrc.m_min.x;
    //x(1,3) = dcrc.m_min.y;
    // #17083 : inversion will be included in screenMatrix, so we must skip it here
    x(0, 0) = odmax(dcrc.m_min.x, dcrc.m_max.x) - odmin(dcrc.m_min.x, dcrc.m_max.x);
    x(1, 1) = odmin(dcrc.m_min.y, dcrc.m_max.y) - odmax(dcrc.m_min.y, dcrc.m_max.y);
    x(0, 3) = odmin(dcrc.m_min.x, dcrc.m_max.x);
    x(1, 3) = odmax(dcrc.m_min.y, dcrc.m_max.y);
    // eof #17083
    x(2,2) = 2.0; // Z-mapping
    x.invert();
    x.preMultBy(OdGeMatrix3d::scaling(OdGeScale3d(2., 2., 2.), OdGePoint3d(1.,1.,1.)));
    
    ::glMatrixMode(GL_PROJECTION);
    device()->glLoadTransposedMatrix(x);

    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();  

    ::glDisable(GL_LIGHTING);
    ::glDisable(GL_POLYGON_STIPPLE);
    ::glDisable(GL_DEPTH_TEST);
#ifndef OD_OGL_ALTERNATIVEHLT
    if (forceAlternativeHlt())
#endif
      alternativeHighlight(false);
    ::glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // prepare to draw into stencil buffer
    ::glClearStencil(0);
    ::glClear(GL_STENCIL_BUFFER_BIT);
    ::glEnable(GL_STENCIL_TEST);
    ::glStencilFunc(GL_ALWAYS, 1, 1);
    ::glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT); // xor operation for overlapped contours

    // for debugging
    //::glColor3ub(255, 0, 0);

    unsigned int i,j;
    if(m)
    {
      OdGePoint3dArray points(n);
      OdInt32Array face(n + 1);

      OdGePoint2d* dcpt = m_nrcPoints.asArrayPtr();
      for( i = 0; i < m; ++i )
      {
        unsigned int n = m_nrcCounts[i];
        face.clear();
        face.reserve(n + 1);
        face.append(n); // counter
        points.resize(n);
        OdGePoint3d* pt = points.asArrayPtr();
        for(j=0; j<n; ++j)
        {
          pt->set(dcpt->x, dcpt->y, 0.0);
          ++dcpt;
          ++pt;
          face.append(j);
        }
        setVertexData(n, points.getPtr(), NULL);
        prepareShMFacesGeneration(NULL, renderMode());
        initArrays(n, points.getPtr(), NULL);
        ::glBegin(GL_TRIANGLES);
          OdGiGeometrySimplifier::shellFaceOut(face.size(), face.getPtr(), &OdGeVector3d::kZAxis);
        ::glEnd();
        uninitArrays(false);
      }
    }

    if(n)
    {
      if(m) // use dept buffer to combine viewport clipping region and invalid area
      {
        ::glStencilFunc(GL_EQUAL, 1, 1);
        ::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        ::glClearDepth(0.);
        ::glClear(GL_DEPTH_BUFFER_BIT);
        ::glDepthFunc(GL_GREATER);
        ::glEnable(GL_DEPTH_TEST);
      }
      else // otherwise use stencil buffer exclusively for invalid area clipping
      {
        ::glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // and operation for overlapped rectangles
      }

      const OdGsDCRect* pInvRect = invRects.getPtr();
      for( i = 0; i < n; ++i )
      {
#ifndef OD_OGL_ES
        ::glBegin(GL_TRIANGLE_FAN);
          ::glVertex3d(pInvRect->m_min.x, pInvRect->m_min.y, 1.9);
          ::glVertex3d(pInvRect->m_min.x, pInvRect->m_max.y, 1.9);
          ::glVertex3d(pInvRect->m_max.x, pInvRect->m_max.y, 1.9);
          ::glVertex3d(pInvRect->m_max.x, pInvRect->m_min.y, 1.9);
        ::glEnd();
#else
        const GLfloat fltVerts[4 * 3] =
        {
          pInvRect->m_min.x, pInvRect->m_min.y, 1.9f,
          pInvRect->m_min.x, pInvRect->m_max.y, 1.9f,
          pInvRect->m_max.x, pInvRect->m_max.y, 1.9f,
          pInvRect->m_max.x, pInvRect->m_min.y, 1.9f
        };
        ::oglEsDrawBuffer(3, fltVerts, GL_TRIANGLE_FAN, 4);
#endif
        ++pInvRect;
      }

      if(m) // combine stencil and dept buffers to stencil buffer
      {
        ::glDepthFunc(GL_LESS);
        ::glStencilOp(GL_KEEP, GL_ZERO, GL_KEEP);

        OdGsDCPoint lowerLeft, upperRight;
        view().screenRect(lowerLeft, upperRight);

#ifndef OD_OGL_ES
        ::glBegin(GL_POLYGON);
          ::glVertex3d(lowerLeft.x,  lowerLeft.y,  0.0);
          ::glVertex3d(lowerLeft.x,  upperRight.y, 0.0);
          ::glVertex3d(upperRight.x, upperRight.y, 0.0);
          ::glVertex3d(upperRight.x, lowerLeft.y,  0.0);
        ::glEnd();
#else
        const GLfloat fltVerts[4 * 3] =
        {
          lowerLeft.x, lowerLeft.y, 0.0f,
          lowerLeft.x, upperRight.y, 0.0f,
          upperRight.x, upperRight.y, 0.0f,
          upperRight.x, lowerLeft.y, 0.0f
        };
        ::oglEsDrawBuffer(3, fltVerts, GL_TRIANGLE_FAN, 4);
#endif
      }
    }

    ::glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    ::glStencilFunc(GL_EQUAL, 1, 1);
    ::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  }
  else if (view().mode() == kHiddenLine)
  {
    // We need clear stencil buffer for correct background rendering
    ::glClearStencil(1);
    ::glClear(GL_STENCIL_BUFFER_BIT);
    ::glEnable(GL_STENCIL_TEST);
  }
  // Setup clipping for hidden line
  if (view().mode() == kHiddenLine)
  {
    ::glStencilFunc(GL_LEQUAL, 1, 0xFFFF);
    ::glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  }
  //

  ::glViewport(ROUND(left), ROUND(bottom), ROUND(width), ROUND(height));

  x = projectionMatrixGL(false);

  ::glMatrixMode( GL_PROJECTION );
  device()->glLoadTransposedMatrix(x);

  RenderMode rm = view().mode();
  if (rm > k2DOptimized)
  {
    ::glLineWidth(1);
    ::glEnable(GL_DEPTH_TEST);
    ::glDepthFunc(GL_LEQUAL);
    if (device()->isClearScreenEnabled())
    {
      ::glClearDepth(1.);
      ::glClear(GL_DEPTH_BUFFER_BIT);
    }

    if (rm > kHiddenLine)
    {
      ::glFrontFace(bFrontFaceCW ? GL_CW : GL_CCW);
      ::glEnable(GL_LIGHTING);
      ::glEnable(GL_NORMALIZE);
      updateLights();
    }
    else
    {
      ::glDisable(GL_LIGHTING);
      ::glDisable(GL_NORMALIZE);
    }

    if(rm==kHiddenLine || rm==kFlatShadedWithWireframe || rm==kGouraudShadedWithWireframe)
    {
      ::glEnable(GL_POLYGON_OFFSET_FILL);
      ::glPolygonOffset(1.0, 2.0); // how to calculate it ???
    }
    else
    {
      ::glDisable(GL_POLYGON_OFFSET_FILL);
    }
    ::glDisable(GL_BLEND);
  }
  else
  {
    ::glDisable(GL_DEPTH_TEST);
    ::glDisable(GL_LIGHTING);
    ::glDisable(GL_NORMALIZE);
    ::glDisable(GL_POLYGON_OFFSET_FILL);
    ::glEnable(GL_BLEND);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  ::glMatrixMode( GL_MODELVIEW );
  if(view().cachedDrawables())
  {
    x = viewportToOutputMatrix();
    mfTransformStack().setOutputTransform(x);
    device()->glLoadTransposedMatrix(x);
  }
  else
  {
    ::glLoadIdentity();
    mfTransformStack().setOutputTransform(OdGeMatrix3d::kIdentity);
  }
}

void OdGsOpenGLVectorizeView::draw_color(const OdCmEntityColor& color)
{
  if (color.isByACI() || color.isByDgnIndex())
    draw_color(OdUInt16(color.colorIndex()));
  else // setting alpha to 255    
    draw_color(ODRGBA(color.red(), color.green(), color.blue(), 255)); // 0 is transparent, 255 is opaque
}

void OdGsOpenGLVectorizeView::draw_color(ODCOLORREF color)
{
  if (m_pColorConverter && (m_bConvertBackgroundColor || !m_bNoColorMod))
    color = m_pColorConverter->convert(color);
  ::glColor4ub(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color), ODGETALPHA(color));
  if (m_bAcceptColorChange)
    m_cCurColor = color;
}

void OdGsOpenGLVectorizeView::draw_color(OdUInt16 colorIndex)
{
  // setting alpha to 255
  // 0 is transparent, 255 is opaque
  draw_color(paletteColor(colorIndex) | 0xFF000000);
}

void OdGsOpenGLVectorizeView::draw_color(const OdCmEntityColor& color, const OdCmTransparency& transparency)
{
  if (color.isByACI() || color.isByDgnIndex())
    draw_color(OdUInt16(color.colorIndex()), transparency);
  else
    draw_color(ODRGBA(color.red(), color.green(), color.blue(), transparency.alpha()));
}

void OdGsOpenGLVectorizeView::draw_color(OdUInt16 colorIndex, const OdCmTransparency& transparency)
{
  draw_color((paletteColor(colorIndex) & 0x00FFFFFF) | (((ODCOLORREF)transparency.alpha()) << 24));
}

void OdGsOpenGLVectorizeView::draw_noColorMod(bool bState)
{
  m_bNoColorMod = bState;
}

#if 0
void OdGsOpenGLVectorizeView::draw_lineweight(GLfloat pixels)
{
  /*if(pixels < 1.0)
    pixels = 1.0;
  if(pixels > m_maxLw)
    pixels = m_maxLw;
  m_fCurLw = pixels;
  ::glLineWidth(pixels);*/
  if(pixels < 1.0)
    pixels = 1.0;
  m_fCurLw = pixels;
  ::glLineWidth(1.0f);

  /*pixels/=GLfloat(1.4);
  if(pixels < 1.0)
    pixels = 1.0;
  ::glPointSize(pixels);*/
  ::glPointSize(1.0f);
}
#endif

void OdGsOpenGLVectorizeView::draw_lineweight(OdDb::LineWeight lwLineWeight, GLfloat pixels)
{
  if (m_lineWeight.isForcedLineweight())
    return;
  if (!hasLineweightOverride())
  {
    m_lineWeight.m_type = OdOpenGLLwdSetting::kDatabase;
    m_lineWeight.m_val.m_lwLineWeight = lwLineWeight;
  }
  else
  {
    m_lineWeight.m_type = OdOpenGLLwdSetting::kAbsolute;
    m_lineWeight.m_val.m_dLineWeight = lineweightToPixelsOverride(lwLineWeight, true);
  }
  if (pixels < 1.0f)
    pixels = 1.0f;
  m_fCurLw = pixels;
  //::glLineWidth(1.0f);
  //::glPointSize(1.0f);
}

void OdGsOpenGLVectorizeView::draw_lineweight(double dLineWeight, GLfloat pixels)
{
  if (m_lineWeight.isForcedLineweight())
    return;
  if (!hasLineweightOverride())
  {
    m_lineWeight.m_type = OdOpenGLLwdSetting::kPlotStyle;
    m_lineWeight.m_val.m_dLineWeight = dLineWeight;
  }
  else
  {
    m_lineWeight.m_type = OdOpenGLLwdSetting::kAbsolute;
    m_lineWeight.m_val.m_dLineWeight = lineweightToPixelsOverride(dLineWeight, true);
  }
  if (pixels < 1.0f)
    pixels = 1.0f;
  m_fCurLw = pixels;
  //::glLineWidth(1.0f);
  //::glPointSize(1.0f);
}

void OdGsOpenGLVectorizeView::draw_lineweight(OdInt32 nLineWeight, bool bReset)
{
  if (!bReset)
  {
    m_lineWeight.m_type = OdOpenGLLwdSetting::kPixels;
    m_lineWeight.m_val.m_pixLineWeight = nLineWeight;
  }
  else
  {
    m_lineWeight.m_type = OdOpenGLLwdSetting::kDatabase;
    m_lineWeight.m_val.m_lwLineWeight = OdDb::kLnWt000;
  }
  m_fCurLw = (GLfloat)nLineWeight;
  if (m_fCurLw < 1.0f)
    m_fCurLw = 1.0f;
  //::glLineWidth(1.0f);
  //::glPointSize(1.0f);
}

OdInt32 OdGsOpenGLVectorizeView::lwdSettingToPixels(const OdOpenGLLwdSetting &lineWeight) const
{
  switch (lineWeight.m_type)
  {
    case OdOpenGLLwdSetting::kDatabase:  return (lineWeight.m_val.m_lwLineWeight == OdDb::kLnWt000) ? 1 :
                                                lineweightToPixels(lineWeight.m_val.m_lwLineWeight);
    case OdOpenGLLwdSetting::kPlotStyle: return OdInt32(OdTruncateToLong(lineweightToPixels(lineWeight.m_val.m_dLineWeight)));
    case OdOpenGLLwdSetting::kPixels:    return lineWeight.m_val.m_pixLineWeight;
    case OdOpenGLLwdSetting::kAbsolute: { OdGePoint2d pt; OdGiBaseVectorizerImpl::getNumPixelsInUnitSquare(OdGePoint3d::kOrigin, pt, false);
                                          return OdInt32(OdTruncateToLong(lineWeight.m_val.m_dLineWeight * pt.y)); }
  }
  return 1;
}

void OdGsOpenGLVectorizeView::draw_linetype(OdOpenGLLinePatt::Entry entry,
                                            OdInt32 state,
                                            bool /*isAdaptive*/, // not supported yet
                                            double /*linePatSz*/) // not supported yet
{
  switch (entry)
  {
    case OdOpenGLLinePatt::kLnPattLinetype:
      if (state >= OdPs::kLtpUseObject)
        state = OdPs::kLtpSolid;
    case OdOpenGLLinePatt::kLnPattSel:
      m_lnPattState.data.reg[entry] = (OdUInt8)state;
    break;
    case OdOpenGLLinePatt::kLnPattAll:
      m_lnPattState.data.summ = 0;
    break;
  }
  if (m_lnPattState.data.summ != m_lnPattPrevState.data.summ)
    updateLinePattState();
}

void OdGsOpenGLVectorizeView::updateLinePattState()
{
  m_lnPattPrevState.data.summ = m_lnPattState.data.summ;
#ifndef OD_OGL_ALTERNATIVEHLT
  if (m_lnPattState.data.named.reg_sel && !forceAlternativeHlt())
  {
    ::glEnable(GL_LINE_STIPPLE);
    ::glLineStipple(3, 0x5555);
    return;
  }
  else if (forceAlternativeHlt())
#endif
    alternativeHighlight(m_lnPattState.data.named.reg_sel || m_fillPattState.data.named.reg_sel);
  if (m_lnPattState.data.named.reg_linetype)
  {
    ::glEnable(GL_LINE_STIPPLE);
    ::glLineStipple((GLshort)m_psLinetypeDefs[m_lnPattState.data.named.reg_linetype].m_patternLength,
                    (GLshort)m_psLinetypeDefs[m_lnPattState.data.named.reg_linetype].m_pattern);
    return;
  }
  ::glDisable(GL_LINE_STIPPLE);
}

const GLubyte OdGsOpenGLVectorizeView::g_selPatt[128] =
{
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55, 
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
  0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55
};

void OdGsOpenGLVectorizeView::draw_fillStyle(OdOpenGLFillPatt::Entry entry, OdInt32 state)
{
  switch (entry)
  {
    case OdOpenGLFillPatt::kFillPattFillstyle:
      m_bFill = (state != OdPs::kFsUseObject);
      if (state >= OdPs::kFsUseObject || m_bProcessingTTF)
        state = OdPs::kFsSolid;
      state -= OdPs::kFsSolid;
    case OdOpenGLFillPatt::kFillPattTransparency:
    case OdOpenGLFillPatt::kFillPattMaterial:
    case OdOpenGLFillPatt::kFillPattSel:
      m_fillPattState.data.reg[entry] = (OdUInt8)state;
    break;
    case OdOpenGLFillPatt::kFillPattAll:
      m_fillPattState.data.summ = 0;
    break;
  }
  if (m_fillPattState.data.summ != m_fillPattPrevState.data.summ)
    updateFillPattState();
}

void OdGsOpenGLVectorizeView::updateFillPattState()
{
  m_fillPattPrevState.data.summ = m_fillPattState.data.summ;
#ifndef OD_OGL_ALTERNATIVEHLT
  if (m_fillPattState.data.named.reg_sel && !forceAlternativeHlt())
  {
    ::glEnable(GL_POLYGON_STIPPLE);
    ::glPolygonStipple(g_selPatt);
    return;
  }
  else if (forceAlternativeHlt())
#endif
    alternativeHighlight(m_lnPattState.data.named.reg_sel || m_fillPattState.data.named.reg_sel);
  if (m_fillPattState.data.named.reg_fillstyle)
  {
    ::glEnable(GL_POLYGON_STIPPLE);
    ::glPolygonStipple(m_psFillstyleDefs[m_fillPattState.data.named.reg_fillstyle]);
    return;
  }
  if (m_fillPattState.data.named.reg_transparency || m_fillPattState.data.named.reg_material)
  {
    // Transparencies here is inverted alpha values: 0 means opaque. So we need inverted summ.
    //OdUInt8 summ = (OdUInt8)((((OdUInt16)m_fillPattState.data.named.reg_transparency) * ((OdUInt16)m_fillPattState.data.named.reg_material)) >> 8);
    //OdUInt8 summ = (OdUInt8)((((OdUInt16)m_fillPattState.data.named.reg_transparency) + ((OdUInt16)m_fillPattState.data.named.reg_material)) >> 1);
    OdUInt8 summ = odmax(m_fillPattState.data.named.reg_transparency, m_fillPattState.data.named.reg_material);
    ::glEnable(GL_POLYGON_STIPPLE);
    ::glPolygonStipple(renderTransparency(255 - summ));
    return;
  }
  ::glDisable(GL_POLYGON_STIPPLE);
}

void OdGsOpenGLVectorizeView::onTraitsModified()
{
  OdGsBaseMaterialVectorizer::onTraitsModified();
  if ((!m_pCurMetafile && isMTRegen()) || isOutputSuppressed())
    return;
  if (!isSupportPlotStyles())
  {
    const OdGsView::RenderMode rm = renderMode();
    const OdGiSubEntityTraitsData& entTraits = effectiveTraits();
    const OdUInt32 drawFlags = entTraits.drawFlags();
    if (m_bNoColorMod != GETBIT(drawFlags, OdGiSubEntityTraits::kDrawNoPlotstyle))
      draw_noColorMod(GETBIT(drawFlags, OdGiSubEntityTraits::kDrawNoPlotstyle));
    if (m_bAcceptColorChange)
    {
      if (m_bDrawTransparency && (rm == k2DOptimized))
        draw_color(entTraits.trueColor(), entTraits.transparency());
      else
        draw_color(entTraits.trueColor());
    }
    bool bLwDisplay = view().hasLweights() || view().lineweightToDcScale() != 0.0 || hasLineweightOverride();
    if (bLwDisplay)
      draw_lineweight(entTraits.lineWeight(), GLfloat(lineweightToPixels(entTraits.lineWeight())));
    else
      draw_lineweight(OdDb::kLnWt000, 1.0f); // Avoid dynamic sublists creation if 'lwdisplay' disabled, but now
                                             // regeneration needed if 'lwdisplay' sysvar changed.
    draw_linetype(OdOpenGLLinePatt::kLnPattLinetype, OdPs::kLtpSolid);
    draw_fillStyle(OdOpenGLFillPatt::kFillPattFillstyle, (entTraits.fillType() == kOdGiFillAlways) ? OdPs::kFsSolid : OdPs::kFsUseObject);
    if (m_bDrawTransparency && (rm != k2DOptimized) && (rm != kHiddenLine))
      draw_fillStyle(OdOpenGLFillPatt::kFillPattTransparency, 255 - entTraits.transparency().alpha());
    draw_lineStyle(true);
    m_bProcessingTTF = GETBIT(drawFlags, OdGiSubEntityTraits::kDrawPolygonFill);
    if (rm > OdGsView::kHiddenLine)
      m_bDontShade = GETBIT(drawFlags, OdGiSubEntityTraits::kDrawSolidFill) || m_bProcessingTTF;
  }
  else
  {
    const OdPsPlotStyleData& plotStyle = effectivePlotStyle();
    if (m_bAcceptColorChange)
    {
      if (m_bDrawTransparency)
        draw_color(plotStyle.color(), effectiveTraits().transparency());
      else
        draw_color(plotStyle.color());
    }
    draw_lineweight(plotStyle.lineweight(), GLfloat(lineweightToPixels(plotStyle.lineweight())));
    draw_fillStyle(OdOpenGLFillPatt::kFillPattFillstyle, plotStyle.fillStyle());
    draw_linetype(OdOpenGLLinePatt::kLnPattLinetype, plotStyle.linetype(), plotStyle.isAdaptiveLinetype(), plotStyle.linePatternSize());
    draw_lineStyle(false, plotStyle.endStyle(), plotStyle.joinStyle());
  }
  onHighlightModified();
}

void OdGsOpenGLVectorizeView::onHighlightModified(bool bDisableLine, bool bDisableFill)
{
  OdGsView::RenderMode rm = renderMode();
  OdUInt8 bHighlighted = (isHighlighted()) ? 1 : 0;
  if (rm != kHiddenLine)
    draw_fillStyle(OdOpenGLFillPatt::kFillPattSel, bHighlighted && !bDisableFill);
  draw_linetype(OdOpenGLLinePatt::kLnPattSel, bHighlighted && !bDisableLine);
}

void OdGsOpenGLVectorizeView::renderTypeOnChange(OdGsModel::RenderType renderType)
{
  OdGsBaseVectorizer::renderTypeOnChange(renderType);
  if (renderMode() != k2DOptimized) // 2D mode have always disabled Z-buffer
  {
    const OdUInt32 overlayFlags = OdGsOverlayMapping::overlayFlags(
      OdGsOverlayMapping::overlayRenderingOrderToIndex(device()->renderTypeWeight(renderType)));
    if (!GETBIT(overlayFlags, OdGsOverlayMapping::kNoDepth))
    {
      ::glEnable(GL_DEPTH_TEST);
      if (renderMode() > kHiddenLine && m_numLights > 0)
      {
        ::glDisable(GL_LIGHT0);
        for (OdUInt32 i = 1; i <= m_numLights; i++)
          ::glEnable(GL_LIGHT0 + i);
      }
    }
    else
    {
      ::glDisable(GL_DEPTH_TEST);
      if (renderMode() > kHiddenLine && m_numLights > 0)
      {
        for (OdUInt32 i = m_numLights; i > 0; --i)
          ::glDisable(GL_LIGHT0 + i);
        ::glEnable(GL_LIGHT0);
      }
    }
  }
}

/*bool OdGsOpenGLVectorizeView::setCurrentTraits()
{
  const OdGiSubEntityTraitsData& traits = OdGsBaseVectorizeView::effectiveTraits();
  if(traits.visibility())
  {
    m_bFill = fillMode();
    if(traits.trueColor().isByColor())
    {
      device()->draw_color(ODTOCOLORREF(traits.trueColor()));
    }
    else
    {
      device()->draw_color(traits.color());
    }
    if(renderMode()==k2DOptimized)
    {
      float lw = float(traits.lineWeightPix());
      if(lw < 1.)
        lw = 1.;
      float oldlw;
      ::glGetFloatv(GL_POINT_SIZE, &oldlw);
      if(oldlw!=lw)
      {
        ::glLineWidth(lw);
        ::glPointSize(lw);
      }
    }
    return true;
  }
  return false;
}*/

void OdGsOpenGLVectorizeView::beginViewVectorization()
{
  OdGsBaseMaterialVectorizer::beginViewVectorization();

  if (isOutputSuppressed())
    return;

  setupForFPVectorization(*this, *this, !view().cachedDrawables(), false,
                          (isMTView()) ? static_cast<OdGsOpenGLVectorizeView*>(&view()) : NULL);

  // initialize OdGiGeometrySimplifier with current eye deviation:
  OdGiGeometrySimplifier::setDeviation(&m_pModelToEyeProc->eyeDeviation());
  OdGiGeometrySimplifier::setDrawContext(OdGsBaseVectorizer::drawContext());
  setDrawContextFlags(drawContextFlags() | kForceMarkersOnModified, true);

  if (view().cachedDrawables())
    setEyeToOutputTransform( getEyeToWorldTransform() );
  else
    setEyeToOutputTransform( OdGeMatrix3d::kIdentity );

  m_bProcessingTTF = m_bDontShade = false;

  m_bForceAlternativeHlt = (device() && device()->isAlternativeHltEnabled());
  m_bDrawTransparency = view().isPlotTransparency() || !giContext().isPlotGeneration();

  m_pColorConverter = device()->getColorConverter();
  if (m_pColorConverter)
    m_bConvertBackgroundColor = m_pColorConverter->convertBackgroundColors();

  draw_linetype(OdOpenGLLinePatt::kLnPattAll, 0);
  draw_fillStyle(OdOpenGLFillPatt::kFillPattAll, 0);
  if (!isMTRegen())
  {
    updateLinePattState(); updateFillPattState();
  }
}

void OdGsOpenGLVectorizeView::endViewVectorization()
{
  resetFading(false);
  if (!isOutputSuppressed())
  {
    finalizeFPVectorization(*this);
    if (!isMTRegen())
    {
      ::glDisable(GL_STENCIL_TEST);
      ::glDisable(GL_BLEND);
      ::glDisable(GL_TEXTURE_2D); // #11389
    }
    draw_linetype(OdOpenGLLinePatt::kLnPattAll, 0);
    draw_fillStyle(OdOpenGLFillPatt::kFillPattAll, 0);
  }
  OdGsBaseMaterialVectorizer::endViewVectorization();
}

bool OdGsOpenGLVectorizeView::forceMetafilesDependence() const
{
  return OdGsBaseVectorizer::forceMetafilesDependence() || metafileTransformOverrided();
}

double OdGsOpenGLVectorizeView::regenCoef()
{
  return ExFPPrecisionFix::regenCoef(view());
}

void OdGsOpenGLVectorizeView::initLights(OdRxIterator* pIt)
{
  m_lights.clear();
  if (pIt)
  {
    for (; !pIt->done(); pIt->next())
    {
      OdGsLightPtr pLight = pIt->object();
      if (!pLight.isNull())
      {
        m_lights.append(pLight.get());
      }
    }
  }
}

OdUInt32 OdGsOpenGLVectorizeView::addLight(const OdGiLightTraitsData& traits)
{
  ODA_ASSERT(view().mode() > kHiddenLine);
  ::glDisable(GL_LIGHT0); // turn off default light
  m_numLights ++;
  GLenum nLightId = GL_LIGHT0 + m_numLights;
  
  OdCmEntityColor color = traits.color();
  float col[4];
  if(color.isByACI() || color.isByDgnIndex())
  {
    ODCOLORREF clrref = paletteColor(color.colorIndex());
    col[0] = (float)ODGETRED(clrref)   / 255.0f;
    col[1] = (float)ODGETGREEN(clrref) / 255.0f;
    col[2] = (float)ODGETBLUE(clrref)  / 255.0f;
    col[3] = 1.0f;
  }
  else
  {
    col[0] = (float)color.red()   / 255.0f;
    col[1] = (float)color.green() / 255.0f;
    col[2] = (float)color.blue()  / 255.0f;
    col[3] = 1.0f;
  }

  //for(int i = 0; i < 3; i ++)
  //  col[i] *= (float)traits.intensity();

  ::glLightfv(nLightId, GL_DIFFUSE, col);
  col[0] = 0.0;
  col[1] = 0.0;
  col[2] = 0.0;
  ::glLightfv(nLightId, GL_SPECULAR, col);
  // #SRC-883 : reset cutoff angle since it is could be set in this slot by spot light previously
  ::glLightf(nLightId, GL_SPOT_CUTOFF, 180.0f);

  return nLightId;
}

static void setAttenuation(OdUInt32 nLightId, const OdGiLightAttenuation& atten, double intensity)
{
  if(atten.attenuationType() == OdGiLightAttenuation::kInverseLinear)
  {
    ::glLightf(nLightId, GL_CONSTANT_ATTENUATION, (float)0.0);
    ::glLightf(nLightId, GL_LINEAR_ATTENUATION, (float)(1.0/intensity));
    ::glLightf(nLightId, GL_QUADRATIC_ATTENUATION, (float)0.0);
  }
  else if(atten.attenuationType() == OdGiLightAttenuation::kInverseSquare)
  {
    ::glLightf(nLightId, GL_CONSTANT_ATTENUATION, (float)0.0);
    ::glLightf(nLightId, GL_LINEAR_ATTENUATION, (float)0.0);
    ::glLightf(nLightId, GL_QUADRATIC_ATTENUATION, (float)(1.0/intensity));
  }
  else
  {
    ::glLightf(nLightId, GL_CONSTANT_ATTENUATION, (float)(1.0/intensity));
    ::glLightf(nLightId, GL_LINEAR_ATTENUATION, (float)0.0);
    ::glLightf(nLightId, GL_QUADRATIC_ATTENUATION, (float)0.0);
  }
}

void OdGsOpenGLVectorizeView::addPointLight(const OdGiPointLightTraitsData& traits)
{
  GLenum nLightId = addLight(traits);

  float pos[4];
  pos[0] = (float)traits.position().x;
  pos[1] = (float)traits.position().y;
  pos[2] = (float)traits.position().z;
  pos[3] = 1.0f;
  ::glLightfv(nLightId, GL_POSITION, pos);

  setAttenuation(nLightId, traits.attenuation(), traits.intensity());

  ::glEnable(nLightId);
}

void OdGsOpenGLVectorizeView::addSpotLight(const OdGiSpotLightTraitsData& traits)
{
  GLenum nLightId = addLight(traits);

  float pos[4];
  pos[0] = (float)traits.position().x;
  pos[1] = (float)traits.position().y;
  pos[2] = (float)traits.position().z;
  pos[3] = 1.0f;
  ::glLightfv(nLightId, GL_POSITION, pos);

  setAttenuation(nLightId, traits.attenuation(), traits.intensity());

  float dir[4];
  OdGeVector3d direction = traits.target() - traits.position();
  direction.normalizeGetLength();
  dir[0] = (float)direction.x;
  dir[1] = (float)direction.y;
  dir[2] = (float)direction.z;
  dir[3] = 1.0;
  ::glLightfv(nLightId, GL_SPOT_DIRECTION, dir);

  ::glLightf(nLightId, GL_SPOT_CUTOFF, (float)OdaToDegree(traits.falloff()/2.0));

  ::glEnable(nLightId);
}

void OdGsOpenGLVectorizeView::addDistantLight(const OdGiDistantLightTraitsData& traits)
{
  GLenum nLightId = addLight(traits);

  OdGeVector3d dir = -traits.direction();
  dir.normalizeGetLength(); // zero vector is ok
  GLfloat direct[4];
  direct[0] = (float)dir.x;
  direct[1] = (float)dir.y;
  direct[2] = (float)dir.z;
  direct[3] = 0.0f;
  ::glLightfv(nLightId, GL_POSITION, direct);

  // Directional lights doesn't apply any attenuation paramaters
  //OdGiLightAttenuation atten;
  //atten.setAttenuationType(OdGiLightAttenuation::kNone);
  //setAttenuation(nLightId, atten, traits.intensity());
  // Use color modification instead
  ::glGetLightfv(nLightId, GL_DIFFUSE, direct);
  for(int i = 0; i < 3; i ++)
    direct[i] *= (float)traits.intensity();
  ::glLightfv(nLightId, GL_DIFFUSE, direct);

  ::glEnable(nLightId);
}

void OdGsOpenGLVectorizeView::updateLights()
{
  if(view().mode() > kHiddenLine)
  {
    ::glMatrixMode( GL_MODELVIEW );
    ::glPushMatrix();
    ::glLoadIdentity();

    GLint nMaxLights = GL_LIGHT1;
    ::glGetIntegerv(GL_MAX_LIGHTS, &nMaxLights);
    
    GLenum nLightId = GL_LIGHT0; 
    GLenum nMaxLightsId = GL_LIGHT0 + nMaxLights; 

    GLfloat ldata[4];
    OdGeVector3d dir;
    //if (!m_lights.size())
    {
      //
      // default "over-the-shoulder" distant light
      //
      ::glEnable(nLightId);
      dir.set(-1., 1., 2.).normalize();
      ldata[0] = (float)dir.x;
      ldata[1] = (float)dir.y;
      ldata[2] = (float)dir.z;
      ldata[3] = 0.0f;
      ::glLightfv(nLightId, GL_POSITION, ldata);

      nLightId++;

      while( nLightId < nMaxLightsId )
        ::glDisable(nLightId++);

      m_numLights = 0;
    }
    /*
    else
    {
      OdGeMatrix3d xW2E = getWorldToEyeTransform();

      OdRxObjectPtrArray::iterator pIt = m_lights.begin();
      OdRxObjectPtrArray::iterator pItEnd = m_lights.end();

      while( nLightId < nMaxLightsId )
      {
        if(pIt != pItEnd)
        {
          OdGsLightPtr pLight = *pIt;
          if (!pLight.isNull() && pLight->type() != OdGsLight::kSpotlight)
          {
            ::glEnable(nLightId);
            if (pLight->type() == OdGsLight::kPointLight)
            {
              OdGePoint3d pt = xW2E * pLight->position();
              ldata[0] = (float)pt.x;
              ldata[1] = (float)pt.y;
              ldata[2] = (float)pt.z;
              ldata[3] = 1.0f;
            }
            else
            {
              dir = xW2E * pLight->direction().negate();
              ldata[0] = (float)dir.x;
              ldata[1] = (float)dir.y;
              ldata[2] = (float)dir.z;
              ldata[3] = 0.0f;
            }
            glLightfv(nLightId, GL_POSITION, ldata);
            
            OdUInt32 rgb = pLight->color();
            ldata[0] = float(ODGETRED(rgb)   / 255. * pLight->intensity());
            ldata[1] = float(ODGETGREEN(rgb) / 255. * pLight->intensity());
            ldata[2] = float(ODGETBLUE(rgb)  / 255. * pLight->intensity());
            ldata[3] = 1.0f;
            
            //glLightfv(nLightId, GL_AMBIENT, ldata);
            ::glLightfv(nLightId, GL_DIFFUSE, ldata);
            ::glLightfv(nLightId, GL_SPECULAR, ldata);
            
            switch(pLight->attenuation())
            {
            case OdGsLight::kAttenNone:
              break;
            case OdGsLight::kAttenInverseLinear:
              break;
            case OdGsLight::kAttenInverseSquare:
              break;
            };

            ++nLightId;
          }
          ++pIt;
        }
        else
        {
          ::glDisable(nLightId++);
        }
      }
    }
    */

    ::glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

    ldata[0] = 1.0f;
    ldata[1] = 1.0f;
    ldata[2] = 1.0f;
    ldata[3] = 1.0f;
    ::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ldata);

    ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    ::glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    ::glEnable(GL_COLOR_MATERIAL);

    ::glPopMatrix();
  }
}

#ifndef OD_OGL_ES
ODA_ASSUME(sizeof(GLdouble)==sizeof(double));
#endif

//inline void
//pointsToGL(GLenum type, OdInt32 nPoints, const OdGePoint3d* pPoints)
//{
//  ::glBegin(type);
//  while(nPoints--)
//  {
//    ::glVertex3dv((const GLdouble*)pPoints);
//    ++pPoints;
//  }
//  ::glEnd();
//}
//
//inline void
//pointsToGL(GLenum type, OdInt32 nPoints, const OdGePoint3d* pPoints, const OdGeVector3d& normal)
//{
//  glNormal3dv((const GLdouble*)&normal);
//  pointsToGL(type, nPoints, pPoints);
//}

void OdGsOpenGLVectorizeView::polylineOut(OdInt32 nPoints, const OdInt32* pVertexIndexList)
{
  ::glDisable(GL_LIGHTING);
  ::glDisable(GL_TEXTURE_2D);

  if ((nPoints == 1) || ((nPoints == 2) && vertexDataList()[pVertexIndexList[0]].isEqualTo(vertexDataList()[pVertexIndexList[1]])))
  {
#ifndef TD_OPENGL_ES
    ::glBegin(GL_POINTS);
      ::glArrayElement(*pVertexIndexList);
    ::glEnd();
#else
    ::oglEsDrawIndexed(1, pVertexIndexList, vertexDataList(), GL_POINTS);
#endif
  }
  else if (!m_lineWeight.hasLineweight() ||
           (!isModelCacheEnabled() && (m_fCurLw < 2.0f))) // Optimize if GsModel disabled
  {
#ifndef TD_OPENGL_ES
    ::glBegin(GL_LINE_STRIP);
      while(nPoints--)
      {
        ::glArrayElement(*pVertexIndexList);
        ++pVertexIndexList;
      }
    ::glEnd();
#else
    ::oglEsDrawIndexed(nPoints, pVertexIndexList, vertexDataList(), GL_LINE_STRIP);
#endif
  }
  else
  {
    if (m_pCurMetafile != NULL)
    {
      static_cast<OdOpenGLList*>(m_pCurMetafile)->appendDynamicLwLines(vertexDataList(), pVertexIndexList, nPoints, m_lineWeight, m_cCurColor, -1, false, false, isHighlighted());
    }
    else
    {
      // drawCappedLineNoMeta()
      OdOpenGLDynamicSubListProc imedProc;
      imedProc.appendDynamicLwLines(vertexDataList(), pVertexIndexList, nPoints, m_lineWeight, m_cCurColor, -1, false, false, isHighlighted());
      imedProc.end();
      imedProc.play(this);
    }
  }
}

void OdGsOpenGLVectorizeView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  ::glDisable(GL_LIGHTING);
  ::glDisable(GL_TEXTURE_2D);

  if ((nPoints == 1) || ((nPoints == 2) && pPoints[0].isEqualTo(pPoints[1])))
  {
#ifndef TD_OPENGL_ES
    ::glBegin(GL_POINTS);
      ::glVertex3dv((const GLdouble*)pPoints);
    ::glEnd();
#else
    const GLfloat tmpPt[3] = { (GLfloat)pPoints->x, (GLfloat)pPoints->y, (GLfloat)pPoints->z };
    ::oglEsDrawBuffer(3, tmpPt, GL_POINTS, 1);
#endif
  }
  else if (!m_lineWeight.hasLineweight() ||
           (!isModelCacheEnabled() && (m_fCurLw < 2.0f))) // Optimize if GsModel disabled
  {
#ifndef TD_OPENGL_ES
    ::glBegin(GL_LINE_STRIP);
      while(nPoints--)
      {
        ::glVertex3dv((const GLdouble*)pPoints);
        ++pPoints;
      }
    ::glEnd();
#else
    ::oglEsDrawPlain(nPoints, pPoints, GL_LINE_STRIP);
#endif
  }
  else
  {
    if (m_pCurMetafile != NULL)
    {
      static_cast<OdOpenGLList*>(m_pCurMetafile)->appendDynamicLwLines(pPoints, nPoints, m_lineWeight, m_cCurColor, -1, false, false, isHighlighted());
    }
    else
    {
      // drawCappedLineNoMeta()
      OdOpenGLDynamicSubListProc imedProc;
      imedProc.appendDynamicLwLines(pPoints, nPoints, m_lineWeight, m_cCurColor, -1, false, false, isHighlighted());
      imedProc.end();
      imedProc.play(this);
    }
  }
}

void OdGsOpenGLVectorizeView::polygonOut(OdInt32 nPoints, const OdGePoint3d* pPoints, const OdGeVector3d* pNormal)
{
  if(!m_bFill)
  {
    ::glDisable(GL_LIGHTING);
    ::glDisable(GL_TEXTURE_2D);

#ifndef TD_OPENGL_ES
    ::glBegin(m_bFill ? GL_POLYGON : GL_LINE_LOOP);
      while(nPoints--)
      {
        ::glVertex3dv((const GLdouble*)pPoints);
        ++pPoints;
      }
    ::glEnd();
#else
    ::oglEsDrawPlain(nPoints, pPoints, GL_LINE_LOOP);
#endif
  }
  else
  {
    OdGiGeometrySimplifier::polygonOut(nPoints, pPoints, pNormal);
  }
}

void OdGsOpenGLVectorizeView::triangleOut( const OdInt32* indices, const OdGeVector3d* pNormal )
{
  // normals are unnecessary
  // vertex colors are in array
  // face colors are set up by onTraitsModified()
  if((renderMode() <= OdGsView::kHiddenLine) || m_bDontShade)
  {
#ifndef OD_OGL_ES
    ::glArrayElement(*indices++);
    ::glArrayElement(*indices++);
    ::glArrayElement(*indices++);
#else
    ::oglEsDrawIndexedTrg(indices, vertexDataList(), NULL, NULL, (vertexData()) ? vertexData()->trueColors() : NULL, ODGETALPHA(m_cCurColor));
#endif
  }
  else // normals are necessary
  {
    if(m_bIsVertsNormals)
    {
      OdGiOrientationType orientation = vertexData()->orientationFlag();
#ifndef OD_OGL_ES
      for(int i = 0; i < 3; i ++)
      {
        if(orientation == kOdGiCounterClockwise)
        {
          ::glNormal3dv((const GLdouble*)(vertexData()->normals() + indices[i]));
        }
        else if(orientation == kOdGiClockwise)
        {
          OdGeVector3d normal = -vertexData()->normals()[indices[i]];
          ::glNormal3dv((const GLdouble*)&normal);
        }
        else // no orientation -- we must calculate it
        {
          OdGeVector3d normal = vertexData()->normals()[indices[i]];
          if(normal.dotProduct(*pNormal) < 0.0)
            normal *= -1.0;
          ::glNormal3dv((const GLdouble*)&normal);
        }
        if (m_bTexCoords)
        {
          ::glTexCoord2dv((const GLdouble*)(m_texCoordsArray.getPtr() + indices[i]));
        }
        ::glArrayElement(indices[i]);
      }
#else
      OdGeVector3d norms[3];
      for (int i = 0; i < 3; i++)
      {
        if(orientation == kOdGiCounterClockwise)
        {
          norms[i] = vertexData()->normals()[indices[i]];
        }
        else if(orientation == kOdGiClockwise)
        {
          norms[i] = -vertexData()->normals()[indices[i]];
        }
        else // no orientation -- we must calculate it
        {
          OdGeVector3d normal = vertexData()->normals()[indices[i]];
          if(normal.dotProduct(*pNormal) < 0.0)
            normal *= -1.0;
          norms[i] = normal;
        }
      }
      ::oglEsDrawIndexedTrg(indices, vertexDataList(), norms, (m_bTexCoords) ? m_texCoordsArray.getPtr() : NULL,
                            (vertexData()) ? vertexData()->trueColors() : NULL, ODGETALPHA(m_cCurColor));
#endif
    }
    else
    {
#ifndef OD_OGL_ES
      ::glNormal3dv((const GLdouble*)pNormal);
      if (m_bTexCoords)
      {
        const OdGePoint2d *pTPt = m_texCoordsArray.getPtr();
        ::glTexCoord2dv((const GLdouble*)(pTPt + *indices));
        ::glArrayElement(*indices++);
        ::glTexCoord2dv((const GLdouble*)(pTPt + *indices));
        ::glArrayElement(*indices++);
        ::glTexCoord2dv((const GLdouble*)(pTPt + *indices));
        ::glArrayElement(*indices++);
      }
      else
      {
        ::glArrayElement(*indices++);
        ::glArrayElement(*indices++);
        ::glArrayElement(*indices++);
      }
#else
      ::glNormal3f((GLfloat)pNormal->x, (GLfloat)pNormal->y, (GLfloat)pNormal->z);
      ::oglEsDrawIndexedTrg(indices, vertexDataList(), NULL, (m_bTexCoords) ? m_texCoordsArray.getPtr() : NULL,
                            (vertexData()) ? vertexData()->trueColors() : NULL, ODGETALPHA(m_cCurColor));
#endif
    }
  }
}

void OdGsOpenGLVectorizeView::text(const OdGePoint3d& position,
                                   const OdGeVector3d& normal,
                                   const OdGeVector3d& direction,
                                   const OdChar* msg,
                                   OdInt32 length,
                                   bool raw,
                                   const OdGiTextStyle* pTextStyle)
{
  m_bProcessingTTF = m_bDontShade = pTextStyle->isTtfFont();
  OdGsBaseVectorizer::text(position, normal, direction, msg, length, raw, pTextStyle);
  m_bProcessingTTF = m_bDontShade = false;
}

void OdGsOpenGLVectorizeView::initArrays(OdInt32 nbVertex,
                                         const OdGePoint3d* pVertexList,
                                         const OdGiVertexData* pVertexData)
{
#ifndef OD_OGL_ES
  glVertexPointer(3, GL_DOUBLE, sizeof(double)*3, (const GLvoid*)pVertexList);
  glEnableClientState(GL_VERTEX_ARRAY);
  if(pVertexData)
  {
    /* Device now should process normals
    const GLvoid* pointer = (const GLvoid*)pVertexData->normals();
    if(pointer)
    {
      glNormalPointer(GL_DOUBLE, sizeof(double)*3, pointer);
      glEnableClientState(GL_NORMAL_ARRAY);
    }
    else
    {
      glDisableClientState(GL_NORMAL_ARRAY);
    } */
    const OdCmEntityColor* pColors = pVertexData->trueColors();
    if(pColors)
    {
      //m_colors.clear();
      const OdUInt8 clrAlpha = ODGETALPHA(m_cCurColor);
      const OdInt16 clrNComponents = (clrAlpha == 255) ? 3 : 4;
      m_colors.reserve(nbVertex * clrNComponents);
      for(int i=0; i<nbVertex; ++i)
      {
        m_colors.append(pColors->red());
        m_colors.append(pColors->green());
        m_colors.append(pColors->blue());
        if (clrNComponents == 4)
          m_colors.append(clrAlpha);
        ++pColors;
      }
      glColorPointer(clrNComponents, GL_UNSIGNED_BYTE, clrNComponents, m_colors.getPtr());
      glEnableClientState(GL_COLOR_ARRAY);
    }
    else
    {
      glDisableClientState(GL_COLOR_ARRAY);
    }
  }
  else
  {
    // glDisableClientState(GL_NORMAL_ARRAY); now it is always disabled
    glDisableClientState(GL_COLOR_ARRAY);
  }
#endif
}

void OdGsOpenGLVectorizeView::uninitArrays(bool bVertexData)
{
#ifndef OD_OGL_ES
  ::glDisableClientState(GL_VERTEX_ARRAY);
  if (bVertexData)
  {
    glDisableClientState(GL_COLOR_ARRAY);
    m_colors.setPhysicalLength(0); // bad for performance, but elsewhere that array may be very large.
  }
#endif
}

void OdGsOpenGLVectorizeView::prepareShMFacesGeneration(const OdGiFaceData* /*pFaceData*/, OdGsView::RenderMode rm)
{
  if(vertexData() && vertexData()->normals() && 
    (rm == OdGsView::kGouraudShaded || rm == OdGsView::kGouraudShadedWithWireframe))
    m_bIsVertsNormals = true;
  else
    m_bIsVertsNormals = false;

  if(
    (rm==kFlatShaded) ||
    (rm == kFlatShadedWithWireframe) ||
    ((rm == kGouraudShaded || rm == kGouraudShadedWithWireframe) && (!m_bIsVertsNormals || vertexData()->orientationFlag() == kOdGiNoOrientation))
    )
    m_bFaceNormalsRequired = true;
  else
    m_bFaceNormalsRequired = false;

  m_bTexCoords = isTextureCoordinatesEnabled(rm);

  if (m_bTexCoords)
  {
    if (m_texCoordsArray.size() < (OdUInt32)vertexDataCount())
      m_texCoordsArray.resize((OdUInt32)vertexDataCount());
    if (isTextureCoordinatesPredefined())
    {
      const OdGePoint3d *pInTexCoords = vertexData()->mappingCoords(OdGiVertexData::kAllChannels);
      OdGePoint2d *pOutTexCoords = m_texCoordsArray.asArrayPtr();
      currentMapper(false)->diffuseMapper()->mapPredefinedCoords(pInTexCoords, pOutTexCoords, (OdUInt32)vertexDataCount());
    }
  }
#ifdef OD_OGL_ES
  else if (m_bRasterImage)
  {
    // OpenGL ES doesn't supports glTexGen, so generate texture coordinates manually
    m_bTexCoords = true;
    OdUInt32 nDataCount = (OdUInt32)vertexDataCount();
    const OdGePoint3d *pData = vertexDataList();
    if (m_texCoordsArray.size() < nDataCount)
      m_texCoordsArray.resize(nDataCount);
    OdGePoint2d *pTexCoord = m_texCoordsArray.asArrayPtr();
    for (OdUInt32 nVert = 0; nVert < nDataCount; nVert++)
    {
      pTexCoord[nVert].x = pData[nVert].x * m_esTexGenSaver.sPlane[0] + pData[nVert].y * m_esTexGenSaver.sPlane[1] +
                           pData[nVert].z * m_esTexGenSaver.sPlane[2] + m_esTexGenSaver.sPlane[3];
      pTexCoord[nVert].y = pData[nVert].x * m_esTexGenSaver.tPlane[0] + pData[nVert].y * m_esTexGenSaver.tPlane[1] +
                           pData[nVert].z * m_esTexGenSaver.tPlane[2] + m_esTexGenSaver.tPlane[3];
    }
  }
#endif
}

bool OdGsOpenGLVectorizeView::isTextureCoordinatesEnabled(OdGsView::RenderMode rm) const
{
  if (!((rm <= OdGsView::kHiddenLine) || m_bDontShade))
  {
    if (isMaterialAvailable() &&
        currentMaterial()->haveDiffuseTexture())
    {
      return true;
    }
  }
  return false;
}

void OdGsOpenGLVectorizeView::beginFaces(GLenum mode, OdGsView::RenderMode rm)
{
  if(rm==kHiddenLine)
  {
    //::glPushAttrib(GL_CURRENT_BIT);
    // Increment stencil area for repaint background data
#ifndef OD_OGL_ES
    ::glPushAttrib(GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT);
#else
    m_esStencilSaver.pushAttrib();
#endif
    ::glStencilFunc(GL_LEQUAL, 1, 0xFFFF);
    ::glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    //
    if (!m_bRasterImage && !m_bProcessingTTF)
    {
      m_bAcceptColorChange = false;
      draw_color(OD_OGL_OPAQUE(device()->getPaletteBackground()));
    }
  }
  else if(rm>kHiddenLine)
  {
    if(m_bDontShade)
    {
      ::glDisable(GL_LIGHTING);
      ::glDisable(GL_TEXTURE_2D);
    }
    else
    {
      if (m_bRasterImage)
        ::glDisable(GL_LIGHTING);
      else
        ::glEnable(GL_LIGHTING);
      if (isMaterialAvailable() && currentMaterial()->haveDiffuseTexture())
      {
        ::glEnable(GL_TEXTURE_2D);
      }
    }
  }
#ifndef OD_OGL_ES
  ::glBegin(mode);
#else
  if (m_bRasterImage)
    OdGiGeometrySimplifier::m_renderMode = OdGsView::kFlatShaded;
#endif
}

void OdGsOpenGLVectorizeView::endFaces(OdGsView::RenderMode rm)
{
#ifndef OD_OGL_ES
  ::glEnd();
#else
  if (m_bRasterImage)
    OdGiGeometrySimplifier::m_renderMode = rm;
#endif
  if(rm==kHiddenLine)
  {
#ifndef OD_OGL_ES
    ::glPopAttrib();
#else
    m_esStencilSaver.popAttrib();
#endif
    if (!m_bRasterImage && !m_bProcessingTTF)
      m_bAcceptColorChange = true;
  }
}

bool OdGsOpenGLVectorizeView::generateShellFill(OdGiHatchPatternPtr pHatch,
                                                OdDouble& fillDensity,
                                                const OdGePoint3d* pVertexList,
                                                OdInt32 faceListSize,
                                                const OdInt32* pFaceList,
                                                const OdGiFaceData* pFaceData,
                                                OdGiMapperItemEntry* pMapper // = 0
                                                )
{
  return OdGiGeometrySimplifier::generateShellFill(pHatch, fillDensity, pVertexList, faceListSize, pFaceList, pFaceData, (currentMapper().isNull() ? (OdGiMapperItemEntry*)0 : currentMapper()->diffuseMapper().get()));
}

void OdGsOpenGLVectorizeView::generateShellFaces(OdInt32 faceListSize, const OdInt32* pFaceList,
                                                 const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData)
{
  prepareShMFacesGeneration(pFaceData, renderMode());

  RenderMode rm = view().mode();
  if (pFaceData) m_bCheckFaceMaterials = pFaceData->materials() != NULL; // #CORE-12062 : handle switch between textured and non-textured materials
  beginFaces(GL_TRIANGLES, rm);
    OdGiGeometrySimplifier::generateShellFaces(faceListSize, pFaceList, pEdgeData, pFaceData);
  endFaces(rm); m_bCheckFaceMaterials = false;

  // To make visible when very zoomed out
  if((rm==k2DOptimized /*|| (rm==kFlatShaded || rm==kGouraudShaded) && m_bProcessingTTF*/) &&
      effectivePlotStyle().fillStyle()==OdPs::kFsSolid &&
      !(vertexData() && vertexData()->trueColors()) )
  {
    OdDb::LineWeight lw = lineWeight();
    if(lw!=OdDb::kLnWt000)
    {
      setLineWeight(OdDb::kLnWt000);
      onTraitsModified();
    }
    generateShellWires(faceListSize, pFaceList, pEdgeData, pFaceData);
    if(lw!=OdDb::kLnWt000)
    {
      setLineWeight(lw);
      onTraitsModified();
    }
  }
}

void OdGsOpenGLVectorizeView::generateMeshFaces(OdInt32 rows, OdInt32 columns, const OdGiFaceData* pFaceData)
{
  prepareShMFacesGeneration(pFaceData, renderMode());

  RenderMode rm = view().mode();
  if (pFaceData) m_bCheckFaceMaterials = pFaceData->materials() != NULL; // #CORE-12062 : handle switch between textured and non-textured materials
  beginFaces(GL_TRIANGLES, rm);
    OdGiGeometrySimplifier::generateMeshFaces(rows, columns, pFaceData);
  endFaces(rm); m_bCheckFaceMaterials = false;

  // To make visible when very zoomed out
  if((rm==k2DOptimized /*|| (rm==kFlatShaded || rm==kGouraudShaded) && m_bProcessingTTF*/) &&
      effectivePlotStyle().fillStyle()==OdPs::kFsSolid &&
      !(vertexData() && vertexData()->trueColors()) )
  {
    generateMeshWires(rows, columns, 0, pFaceData);
  }
}

void OdGsOpenGLVectorizeView::shellProc(OdInt32 numVertices, const OdGePoint3d* vertexList,
                                        OdInt32 faceListSize, const OdInt32* faceList,
                                        const OdGiEdgeData* edgeData,
                                        const OdGiFaceData* faceData,
                                        const OdGiVertexData* vertexData)
{
  initArrays(numVertices, vertexList, vertexData);
  OdGiGeometrySimplifier::shellProc(numVertices, vertexList, faceListSize, faceList, edgeData, faceData, vertexData);
  uninitArrays(vertexData != NULL);
}

void OdGsOpenGLVectorizeView::meshProc(OdInt32 numRows, 
                                       OdInt32 numColumns,
                                       const OdGePoint3d* vertexList,
                                       const OdGiEdgeData* edgeData,
                                       const OdGiFaceData* faceData,
                                       const OdGiVertexData* vertexData)
{
  initArrays(numRows * numColumns, vertexList, vertexData);
  OdGiGeometrySimplifier::meshProc(numRows, numColumns, vertexList, edgeData, faceData, vertexData);
  uninitArrays(vertexData != NULL);
}

// shellFaceOut and meshFaceOut calculates face normal (suitable for OpenGL) in case it is necessary

void OdGsOpenGLVectorizeView::meshFaceOut(const OdInt32* faceList,
                                          const OdGeVector3d* pNormal)
{
  OdGeVector3d normal;
  if(m_bFaceNormalsRequired)
  {
    // correct face normal is necessary
    OdGePoint3d bndPts[4];
    bndPts[0] = vertexDataList()[faceList[0]];
    bndPts[1] = vertexDataList()[faceList[1]];
    bndPts[2] = vertexDataList()[faceList[3]];
    bndPts[3] = vertexDataList()[faceList[2]];
    normal = -::odgiFaceNormal(4, bndPts);
    if(pNormal)
    {
      if(pNormal->dotProduct(normal) < 0.0)
      {
        normal = -*pNormal;
        pNormal = &normal;
      }
    }
    else
    {
      pNormal = &normal;
    }
  } // else correct face normal isn't necessary
  if (m_bTexCoords && !m_bRasterImage)
  {
    if (!isTextureCoordinatesPredefined())
      currentMapper()->diffuseMapper()->mapCoords((OdUInt32)vertexDataCount(), vertexDataList(), m_texCoordsArray.asArrayPtr(),
        4, faceList, OdGiMapperItemEntry::kPolyIt, pNormal, NULL, (vertexData()) ? vertexData()->normals() : NULL,
        (vertexData()) ? vertexData()->orientationFlag() : kOdGiNoOrientation);
  }
  triangleOut(faceList, pNormal);
  OdInt32 vrts[4];
  vrts[0] = faceList[3];
  vrts[1] = faceList[2];
  vrts[2] = faceList[1];
  triangleOut(vrts, pNormal);
}

void OdGsOpenGLVectorizeView::shellFaceOut(OdInt32 faceListSize,
                                           const OdInt32* pFaceList,
                                           const OdGeVector3d* pNormal)
{
  OdGeVector3d normal;
  if(m_bFaceNormalsRequired)
  {
    // correct face normal is necessary
    OdGiShellFaceIterator pVertices(vertexDataList(), pFaceList+1);
    normal = -::odgiFaceNormal(*pFaceList, pVertices);
    if(pNormal)
    {
      if(pNormal->dotProduct(normal) < 0.0)
      {
        normal = -*pNormal;
        pNormal = &normal;
      }
    }
    else
    {
      pNormal = &normal;
    }
  } // else correct face normal isn't necessary
  if (m_bTexCoords && !m_bRasterImage)
  {
    if (!isTextureCoordinatesPredefined())
    {
      OdInt32 nVertexes = (OdInt32)Od_abs(*pFaceList);
      if (nVertexes == faceListSize - 1)
        currentMapper()->diffuseMapper()->mapCoords((OdUInt32)vertexDataCount(), vertexDataList(), m_texCoordsArray.asArrayPtr(),
          (OdUInt32)nVertexes, pFaceList + 1, OdGiMapperItemEntry::kPolyIt, pNormal, NULL, (vertexData()) ? vertexData()->normals() : NULL,
          (vertexData()) ? vertexData()->orientationFlag() : kOdGiNoOrientation);
      else
        currentMapper()->diffuseMapper()->mapCoords((OdUInt32)vertexDataCount(), vertexDataList(), m_texCoordsArray.asArrayPtr(),
          (OdUInt32)faceListSize, pFaceList, OdGiMapperItemEntry::kShellMapIt, pNormal, NULL, (vertexData()) ? vertexData()->normals() : NULL,
          (vertexData()) ? vertexData()->orientationFlag() : kOdGiNoOrientation);
    }
  }
  OdGiGeometrySimplifier::shellFaceOut(faceListSize, pFaceList, pNormal);
}

void OdGsOpenGLVectorizeView::rasterImageDc(const OdGePoint3d& origin,
                                            const OdGeVector3d& u,
                                            const OdGeVector3d& v,
                                            const OdGiRasterImage* pImg,
                                            const OdGePoint2d* uvBoundary,
                                            OdUInt32 numBoundPts,
                                            bool transparency,
                                            double brightness,
                                            double contrast,
                                            double fade)
{
  if (baseDevice()->supportDynamicHighlight())
  {
    OdGsBaseVectorizer::rasterImageDc(origin, u, v, pImg, uvBoundary, 
      numBoundPts, transparency, brightness, contrast, fade);
  }
  else
  {
    OdSaveState<OdUInt32> ssHLT(m_flags);
    highlight(false);
    OdGsBaseVectorizer::rasterImageDc(origin, u, v, pImg, uvBoundary, 
      numBoundPts, transparency, brightness, contrast, fade);
  }
}

void OdGsOpenGLVectorizeView::glInitTextureTransform(OdOpenGLAlphaTestType bEnableAlphaTest, bool /*bBitonal*/, GLint glEnvMode,
                                                     const OdGeQuaternion &glSPlane, const OdGeQuaternion &glTPlane)
{
  switch (bEnableAlphaTest)
  {
    case OdOpenGLAlphaTestType_Bitonal:
      ::glEnable(GL_ALPHA_TEST);
      ::glAlphaFunc(GL_GREATER, 0.5);
    break;
    case OdOpenGLAlphaTestType_0:
      ::glEnable(GL_ALPHA_TEST);
      ::glAlphaFunc(GL_GREATER, 0.0);
    break;
    case OdOpenGLAlphaTestType_255:
      ::glEnable(GL_ALPHA_TEST);
      ::glAlphaFunc(GL_EQUAL, 1.0);
    break;
    case OdOpenGLAlphaTestType_Blend:
      ::glEnable(GL_BLEND);
      ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    break;
    default: break;
  }

  ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, glEnvMode);

#ifndef OD_OGL_ES
  ::glEnable(GL_TEXTURE_GEN_S);
  ::glEnable(GL_TEXTURE_GEN_T);
  ::glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); // horizontal texture coordinate
  ::glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); // vertical texture coordinate

  glTexGendv(GL_S, GL_OBJECT_PLANE, (GLdouble*)&glSPlane);
  glTexGendv(GL_T, GL_OBJECT_PLANE, (GLdouble*)&glTPlane);
#else
  m_esTexGenSaver.sPlane[0] = glSPlane.w; m_esTexGenSaver.sPlane[1] = glSPlane.x; m_esTexGenSaver.sPlane[2] = glSPlane.y; m_esTexGenSaver.sPlane[3] = glSPlane.z;
  m_esTexGenSaver.tPlane[0] = glTPlane.w; m_esTexGenSaver.tPlane[1] = glTPlane.x; m_esTexGenSaver.tPlane[2] = glTPlane.y; m_esTexGenSaver.tPlane[3] = glTPlane.z;
#endif
}

void OdGsOpenGLVectorizeView::glInitTexture(OdOpenGLAlphaTestType bEnableAlphaTest, GLint glFormat, GLsizei pxWidth, GLsizei pxHeight,
                                            const void *pTexture, GLint glFilter, GLint glEnvMode,
                                            const OdGeQuaternion &glSPlane, const OdGeQuaternion &glTPlane,
                                            OdUInt32 /*bufSize*/, bool bBitonal, const void * /*pPalTexture*/, OdUInt32 /*lutSize*/)
{
  ::glEnable(GL_TEXTURE_2D);

  ::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  ::glPixelStorei(GL_PACK_ALIGNMENT, 4);

  GLint internalformat = glFormat;
#ifdef GL_EXT_bgra
  if (internalformat == GL_BGR_EXT) internalformat = GL_RGB;
  else if (internalformat == GL_BGRA_EXT) internalformat = GL_RGBA;
#endif

#if 1
#ifdef GL_EXT_texture_compression_s3tc
  if ((internalformat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT) || (internalformat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT))
    ((OdGLFn_CompressedTexImage2D_EXT)device()->getExtensionFunc(OdGsOpenGLVectorizeDevice::OpenGLExtFunc_EXT_CompressedTexImage2D))(
      GL_TEXTURE_2D, 0, internalformat, pxWidth, pxHeight, 0,
      (GLsizei)odComputeDxt1CompressedBufferSize(pxWidth, pxHeight), pTexture);
  else
#endif
  ::glTexImage2D(GL_TEXTURE_2D, 0, internalformat, pxWidth, pxHeight, 0, (GLenum)glFormat, GL_UNSIGNED_BYTE, pTexture);
#else
  // #6795 : possible workaround for old buggy ATI drivers. If doesn't help, try to disable non-pow2 extension.
  ::glTexImage2D(GL_TEXTURE_2D, 0, internalformat, pxWidth, pxHeight, 0, (GLenum)glFormat, GL_UNSIGNED_BYTE, NULL);
  ::glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pxWidth, pxHeight, (GLenum)glFormat, GL_UNSIGNED_BYTE, pTexture);
#endif

  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

#ifdef GL_EXT_texture_edge_clamp
  if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_EdgeClamp))
  {
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  else
#endif
  {
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  }

  glInitTextureTransform(bEnableAlphaTest, bBitonal, glEnvMode, glSPlane, glTPlane);
}

void OdGsOpenGLVectorizeView::glInitLutTexture(const void *pPalTexture, OdUInt32 lutSize)
{
  ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lutSize, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPalTexture);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#ifdef GL_EXT_texture_edge_clamp
  if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_EdgeClamp))
  {
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  else
#endif
  {
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  }
}

#include "lut_helpers.h"

void OdGsOpenGLVectorizeView::initTexture(const OdGePoint3d& origin,
                                          const OdGeVector3d& u,
                                          const OdGeVector3d& v,
                                          const OdGiRasterImage* pImage, // image object
                                          bool transparency,
                                          double brightness,
                                          double contrast,
                                          double fade)
{
  OdGsOpenGLVectorizeDevice *pDevice = device();
  OdGsBaseDeviceMTSyncHelper __sync(pDevice->mtDeviceSync(), OdGsBaseDeviceMTHelpers::kSyncRasterCache);

  const OdUInt32 srcColDepth = pImage->colorDepth();

  // recomputing width and height, resample image(need for quite large images)  
  const OdUInt32 imgWidth(pImage->pixelWidth()), imgHeight(pImage->pixelHeight());
  OdUInt32 newWidth(imgWidth), newHeight(imgHeight);

  // resampling to get sizes like 2^n+2 (for some integer n)
  GLint maxTexSz = pDevice->glMaxTextureSize();

  // resampling...
  if (!pDevice->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_ARB_NPOT))
  {
    OdUInt32 n = (OdUInt32)ceil(log((double)newWidth)/log(2.0));
    newWidth  = (1 << n);
    if((GLint)newWidth > maxTexSz)
      newWidth = (OdUInt32)maxTexSz;
    n = (OdUInt32)ceil(log((double)newHeight)/log(2.0));
    newHeight = (1 << n);
    if((GLint)newHeight > maxTexSz)
      newHeight = (OdUInt32)maxTexSz;
  }
  else
  {
    if (newWidth > (OdUInt32)maxTexSz)
      newWidth = (OdUInt32)maxTexSz;
    if (newHeight > (OdUInt32)maxTexSz)
      newHeight = (OdUInt32)maxTexSz;
  }

  GLint internalformat;
  GLenum format;
  GLenum formatType = GL_UNSIGNED_BYTE;
  bool bKeepBGRA = false;
#ifdef GL_ARB_fragment_program
  if (srcColDepth <= 8 && device()->canUseLutPalette(srcColDepth) && canUseLut())
  {
    internalformat = format = GL_LUMINANCE;
  }
  else
#endif
  if (srcColDepth == 1)
  {
#ifdef GL_EXT_texture_compression_s3tc
    if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_Dxt))
    {
      /* This extension is available since very old S3 Trio video adapter, so I assume that it is available in any
         video card now. Provide 1:8 compression for RGBA data. ATI Radeon driver compressor is very ineffective and buggy,
         so our own compressor was used which is optimized for monochrome data processing. DXT compression is lossy, but
         it is lossless for monochrome data, so this is good choise to use it. Moreover we could apply resize during
         compression step to increase effectivity. Actually Dxt1 is 4:1 with luminance textures, but luminance textures is
         deprecated in OpenGL 4 specification (moreover solution with luminance textures require 2 textures, so loss in 2:1,
         but win in performance since rendering require one step). Dxt compression is available on any platform include
         mobile because textures compression is very actual for them.                                                        */
      internalformat = (transparency) ? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT : GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
      odNormalizeDimensionsForDxt1Compression(newWidth, newHeight);
    }
    else
#endif
    internalformat = (transparency) ? GL_RGBA : GL_RGB;
    format = (transparency) ? GL_RGBA : GL_RGB;
  }
  else if (srcColDepth == 32)
  {
    internalformat = (transparency) ? GL_RGBA : GL_RGB;
#ifdef GL_EXT_bgra
    if (pImage->pixelFormat().isBGRA() && device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_bgra))
    {
      format = (transparency) ? GL_BGRA_EXT : GL_BGR_EXT;
      bKeepBGRA = true;
    }
    else
#endif
    {
      format = (transparency) ? GL_RGBA : GL_RGB;
    }
  }
  else if (srcColDepth == 24)
  {
    internalformat = GL_RGB;
#ifdef GL_EXT_bgra
    if (pImage->pixelFormat().isBGR() && device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_bgra))
    {
      format = GL_BGR_EXT;
      bKeepBGRA = true;
    }
    else
#endif
    {
      format = GL_RGB;
    }
  }
  else
  {
    internalformat = GL_RGB;
    format = GL_RGB;
  }

#ifdef OD_OGL_ES
  if (format == GL_BGR_EXT)
  {
    // Look comments for OpenGLExtension_EXT_bgra
    format = GL_RGB;
    bKeepBGRA = false;
  }
#endif

  // http://www.opengl.org/resources/faq/technical/texture.htm :
  // 21.130 What's the maximum size texture map my device will render hardware accelerated?
  // If your texture isn't hardware accelerated, but still within the size restrictions returned
  // by GL_MAX_TEXTURE_SIZE, it should still render correctly.
  // This is only an estimate, because the glGet*() function doesn't know what format, internalformat,
  // type, and other parameters you'll be using for any given texture. OpenGL 1.1 and greater solves
  // this problem by allowing texture proxy.
#if defined(GL_VERSION_1_1) && !defined(OD_OGL_ES)
  // Make proxy test, is this maxTexSz allowed in that format
  if (!isMTRegen())
  {
    maxTexSz = odmax(newWidth, newHeight);
    while (maxTexSz > 1)
    {
      ::glTexImage2D(GL_PROXY_TEXTURE_2D, 0, internalformat,
                     newWidth, newHeight, 0, format,
                     formatType, NULL);
      GLint testVal = 0;
      ::glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &testVal);
      if (testVal == 0)
      {
        maxTexSz >>= 1;
        if (newWidth > (OdUInt32)maxTexSz)
          newWidth = (OdUInt32)maxTexSz;
        if (newHeight > (OdUInt32)maxTexSz)
          newHeight = (OdUInt32)maxTexSz;
      }
      else
      {
        break;
      }
    }
  }
  // On my ATI Radeon HD 2600 PRO GL_MAX_TEXTURE_SIZE always reported as 8192. But texture processing is incorect with >= 4096.
  // This looks like as driver bug and proxy test don't solves the problem. I've use special test, but results are same.
  // I'm don't see any way how to solve this problem for all hardware :( Hardcode something like 2048 isn't good solution.
#endif

  ODCOLORREF traitsCol = 0;
  bool bSecColor = false;
  ODCOLORREF secCol = 0;

  if (srcColDepth == 1)
  {
    const OdGiSubEntityTraitsData& traits = OdGsBaseVectorizer::effectiveTraits();
    if(traits.trueColor().isByColor())
      traitsCol = ODTOCOLORREF(traits.trueColor());
    else
      traitsCol = paletteColor(traits.color());
    bSecColor = !transparency && GETBIT(traits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill);
    if (bSecColor)
    {
      if(traits.secondaryTrueColor().isByColor())
        secCol = ODTOCOLORREF(traits.secondaryTrueColor());
      else
        secCol = paletteColor(traits.secondaryTrueColor().colorIndex());
    }
  }

  bool bTransparentBitonal = false;
  bool bTransparent32      = false;
  const OdUInt8* pScanLines = NULL;
  OdUInt32 bufSize = 0;
  const OdUInt8* pPalData = NULL;
  OdUInt32 lutSize = 0;

  OdGiRasterImagePtr pCopyImg;
  OdUInt8Array scLinesBuf, palBuf;
  if (!isTextureLoaded(pImage, transparency, traitsCol, bSecColor, secCol, brightness, contrast, fade, pScanLines, bufSize, pPalData, lutSize))
  {
    OdGiRasterImagePtr pBitonalImg;
    if(srcColDepth == 1)
    {
      if(transparency)
        bTransparentBitonal = true;
#ifdef GL_EXT_texture_compression_s3tc
      if ((internalformat != GL_COMPRESSED_RGB_S3TC_DXT1_EXT) && (internalformat != GL_COMPRESSED_RGBA_S3TC_DXT1_EXT))
      {
#endif
      OdSmartPtr<OdGiBitonalRasterTransformer> pBitRast = OdRxObjectImpl<OdGiBitonalRasterTransformer>::createObject();
      if (!bSecColor)
        pBitRast->setOriginal(pImage, traitsCol, device()->getPaletteBackground(), bTransparentBitonal);
      else
        pBitRast->setOriginal(pImage, traitsCol, secCol, bTransparentBitonal);
      pBitonalImg = pBitRast;
      pImage = pBitonalImg;
#ifdef GL_EXT_texture_compression_s3tc
      }
      else
      {
        // Get input pixels
        OdUInt8Array intermedBuf;
        const OdUInt8 *pIntermedBuf = pImage->scanLines();
        if (!pIntermedBuf)
        {
          intermedBuf.resize(pImage->scanLineSize() * imgHeight);
          pIntermedBuf = intermedBuf.asArrayPtr();
          pImage->scanLines(intermedBuf.asArrayPtr(), 0, imgHeight);
        }
        // Prepare buffer for output pixels
        scLinesBuf.resize(odComputeDxt1CompressedBufferSize(newWidth, newHeight));
        pScanLines = scLinesBuf.asArrayPtr();
        bufSize = scLinesBuf.size();
        // Get colors
        OdUInt8 monoColor1[4] = { 0, 0, 0, 0 },
                monoColor2[4] = { 255, 255, 255, 255 };
        {
          OdSmartPtr<OdGiBitonalRasterTransformer> pBitRast = OdRxObjectImpl<OdGiBitonalRasterTransformer>::createObject();
          if (!bSecColor)
            pBitRast->setOriginal(pImage, traitsCol, device()->getPaletteBackground(), bTransparentBitonal);
          else
            pBitRast->setOriginal(pImage, traitsCol, secCol, bTransparentBitonal);
          monoColor1[0] = ODGETRED(pBitRast->color(0));
          monoColor1[1] = ODGETGREEN(pBitRast->color(0));
          monoColor1[2] = ODGETBLUE(pBitRast->color(0));
          monoColor1[3] = (pBitRast->transparentColor() == 0) ? 0 : 255;
          monoColor2[0] = ODGETRED(pBitRast->color(1));
          monoColor2[1] = ODGETGREEN(pBitRast->color(1));
          monoColor2[2] = ODGETBLUE(pBitRast->color(1));
          monoColor2[3] = (pBitRast->transparentColor() == 1) ? 0 : 255;
        }
        // Compress texture
        if (internalformat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
          odMonoDxt1RGBCompress(pIntermedBuf, imgWidth, imgHeight, pImage->scanLineSize(),
            monoColor1, monoColor2, scLinesBuf.asArrayPtr(), newWidth, newHeight);
        else
          odMonoDxt1RGBACompress(pIntermedBuf, imgWidth, imgHeight, pImage->scanLineSize(),
            monoColor1, monoColor2, scLinesBuf.asArrayPtr(), newWidth, newHeight);
        format = internalformat;
      }
#endif
    }
    else if(srcColDepth == 32 && transparency)
    {
      bTransparent32 = true;
    }
#ifdef GL_ARB_fragment_program
    if (internalformat == GL_LUMINANCE)
    {
      // Get input pixels
      OdUInt8Array intermedBuf;
      const OdUInt8 *pIntermedBuf = pImage->scanLines();
      if (!pIntermedBuf)
      {
        intermedBuf.resize(pImage->scanLineSize() * imgHeight);
        pIntermedBuf = intermedBuf.asArrayPtr();
        pImage->scanLines(intermedBuf.asArrayPtr(), 0, imgHeight);
      }
      // Prepare buffer for output pixels
      scLinesBuf.resize(::computeLutScanlineLength(newWidth) * newHeight);
      pScanLines = scLinesBuf.asArrayPtr();
      bufSize = scLinesBuf.size();
      ::makeLutTexturePixels(imgWidth, imgHeight, srcColDepth, pImage->scanLineSize(), pIntermedBuf,
                             newWidth, newHeight, ::computeLutScanlineLength(newWidth), scLinesBuf.asArrayPtr());
      // Prepare palette texture
      palBuf.resize(::computeLutPaletteLength(pImage));
      pPalData = palBuf.getPtr();
      lutSize = palBuf.size() / 4;
      ::makeLutPalettePixels(pImage, palBuf.asArrayPtr());
    }
    else
    {
#endif
#ifdef GL_EXT_texture_compression_s3tc
    if ((internalformat != GL_COMPRESSED_RGB_S3TC_DXT1_EXT) && (internalformat != GL_COMPRESSED_RGBA_S3TC_DXT1_EXT))
    {
#endif
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
    pDesc->setPixelWidth(newWidth);
    pDesc->setPixelHeight(newHeight);
    pDesc->setScanLinesAlignment(4);
    if(bTransparentBitonal || bTransparent32)
    {
      if (pImage->scanLinesAlignment() == 1)
        pDesc->setScanLinesAlignment(1); // not principle in that format
      pDesc->setColorDepth(32);
      if (bKeepBGRA)
        pDesc->pixelFormat().setBGRA();
      else
        pDesc->pixelFormat().setRGBA();
    }
    else
    {
      pDesc->setColorDepth(24);
      if (bKeepBGRA)
        pDesc->pixelFormat().setBGR();
      else
        pDesc->pixelFormat().setRGB();
    }

    pCopyImg = pImage->convert(true, brightness, contrast, fade, device()->getPaletteBackground(), false, false, false, pDesc);
    pImage = pCopyImg;

    pScanLines = pImage->scanLines();
    if(!pScanLines)
    {
      scLinesBuf.resize(pImage->scanLineSize() * newHeight);
      pScanLines = scLinesBuf.asArrayPtr();
      pImage->scanLines(scLinesBuf.asArrayPtr(), 0, newHeight);
    }
    bufSize = pImage->scanLineSize() * newHeight;
#ifdef GL_EXT_texture_compression_s3tc
    }
#endif
#ifdef GL_ARB_fragment_program
    }
#endif
  }
  else
  {
    bTransparentBitonal = (srcColDepth == 1 ) && transparency;
    bTransparent32      = (srcColDepth == 32) && transparency;
  }

#if 0

  ::glEnable(GL_TEXTURE_2D);

  if((srcColDepth == 1) && transparency)
  {
    ::glEnable(GL_ALPHA_TEST);
    ::glAlphaFunc(GL_GREATER, 0.5);
  }

  ::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  ::glPixelStorei(GL_PACK_ALIGNMENT, 4);

  if(bTransparentBitonal)
    ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newWidth, newHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pScanLines);
  else
    ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pScanLines);
  GLint filter = (srcColDepth == 1 && renderMode() == OdGsView::k2DOptimized) ? GL_NEAREST : GL_LINEAR;
  if (!giContext().isPlotGeneration() && giContext().imageQuality() == OdGiContext::kImageQualityDraft)
    filter = GL_NEAREST;
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  
  ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, bTransparentBitonal ? GL_MODULATE : GL_DECAL);

  ::glEnable(GL_TEXTURE_GEN_S);
  ::glEnable(GL_TEXTURE_GEN_T);
  ::glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); // horizontal texture coordinate
  ::glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); // vertical texture coordinate

  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  OdGeMatrix3d xWorld2UV;
  xWorld2UV.setCoordSystem(origin, u, v, u.crossProduct(v).normal()).invert();

  double coords[4];
  coords[0] = xWorld2UV[0][0] / imgWidth;
  coords[1] = xWorld2UV[0][1] / imgWidth;
  coords[2] = xWorld2UV[0][2] / imgWidth;
  coords[3] = xWorld2UV[0][3] / imgWidth;
  glTexGendv(GL_S, GL_OBJECT_PLANE, coords);

  coords[0] = xWorld2UV[1][0] / imgHeight;
  coords[1] = xWorld2UV[1][1] / imgHeight;
  coords[2] = xWorld2UV[1][2] / imgHeight;
  coords[3] = xWorld2UV[1][3] / imgHeight;
  glTexGendv(GL_T, GL_OBJECT_PLANE, coords);

#else

  OdGeMatrix3d xWorld2UV;
  xWorld2UV.setCoordSystem(origin, u, v, u.crossProduct(v).normal()).invert();

  OdGeQuaternion sPlane(xWorld2UV[0][0] / imgWidth, xWorld2UV[0][1] / imgWidth,
                        xWorld2UV[0][2] / imgWidth, xWorld2UV[0][3] / imgWidth);
  OdGeQuaternion tPlane(xWorld2UV[1][0] / imgHeight, xWorld2UV[1][1] / imgHeight,
                        xWorld2UV[1][2] / imgHeight, xWorld2UV[1][3] / imgHeight);

  GLint glFilter = (srcColDepth == 1 && (renderMode() == OdGsView::k2DOptimized)) ? GL_NEAREST : GL_LINEAR;
  if ((internalformat == GL_LUMINANCE) ||
      (!giContext().isPlotGeneration() && (renderMode() == OdGsView::k2DOptimized) && (giContext().imageQuality() == OdGiContext::kImageQualityDraft)) ||
      (pImage->imageSource() == OdGiRasterImage::kFromOleObject))
    glFilter = GL_NEAREST;
  if (bTransparent32)
  {
    if (pImage->transparencyMode() == OdGiRasterImage::kTransparencyOff)
      bTransparent32 = false;
    else if (pImage->transparencyMode() != OdGiRasterImage::kTransparency8Bit)
      glFilter = GL_NEAREST; // to prevent bilinear artefacts
  }
  OdOpenGLAlphaTestType transpType = OdOpenGLAlphaTestType_Disable;
  if (bTransparentBitonal)
    transpType = OdOpenGLAlphaTestType_Bitonal;
  else if (bTransparent32)
  {
    switch (pImage->transparencyMode())
    {
      case OdGiRasterImage::kTransparencyDef: transpType = OdOpenGLAlphaTestType_0; break;
      case OdGiRasterImage::kTransparency1Bit: transpType = OdOpenGLAlphaTestType_255; break;
      case OdGiRasterImage::kTransparency8Bit: transpType = OdOpenGLAlphaTestType_Blend; break;
      default: break;
    }
  }

  this->glInitTexture(transpType, (GLint)format,
                      newWidth, newHeight, pScanLines, glFilter,
                      (bTransparentBitonal || bTransparent32) ? GL_MODULATE : GL_DECAL, sPlane, tPlane, bufSize, srcColDepth == 1,
                      pPalData, lutSize);

  m_bRasterImage = true;

#endif
}

void OdGsOpenGLVectorizeView::uninitTexture()
{
  m_bRasterImage = false;
  ::glDisable(GL_TEXTURE_2D);
  ::glDisable(GL_ALPHA_TEST);
  if (renderMode() != k2DOptimized)
    ::glDisable(GL_BLEND);
}

bool OdGsOpenGLVectorizeView::isTextureLoaded(const OdGiRasterImage* /*pImage*/,
                                              bool /*transparency*/,
                                              ODCOLORREF /*fgColor*/, // only if transparency enabled
                                              bool /*secColor*/,
                                              ODCOLORREF /*bkColor*/, // only if secColor enabled
                                              double /*brightness*/,
                                              double /*contrast*/,
                                              double /*fade*/,
                                              const OdUInt8 *&/*pData*/,
                                              OdUInt32 &/*bufSize*/,
                                              const OdUInt8 *&/*pPalData*/,
                                              OdUInt32 &/*lutSize*/)
{
  return false;
}

/*
void OdGsOpenGLVectorizeView::meshProc(OdInt32 rows,
                                       OdInt32 columns,
                                       const OdGePoint3d* pVertexList,
                                       const OdGiEdgeData* pEdgeData,
                                       const OdGiFaceData* pFaceData,
                                       const OdGiVertexData* pVertexData)
{
}

void OdGsOpenGLVectorizeView::polylineProc(OdInt32 nPoints, const OdGePoint3d* pPoints,
                                         const OdGeVector3d* pNormal,
                                         const OdGeVector3d* pExtrusion, OdGsMarker bBaseMarker)
{
}

void OdGsOpenGLVectorizeView::polygonProc(OdInt32 nPoints, const OdGePoint3d* pPoints,
                                        const OdGeVector3d* pNormal,
                                        const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::circleProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::circleProc(const OdGePoint3d&, const OdGePoint3d&, const OdGePoint3d&, const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::circularArcProc(const OdGePoint3d& center,
                                            double radius,
                                            const OdGeVector3d& normal,
                                            const OdGeVector3d& startVector,
                                            double sweepAngle,
                                            OdGiArcType arcType, const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::circularArcProc(const OdGePoint3d& start,
                                            const OdGePoint3d& point,
                                            const OdGePoint3d& end,
                                            OdGiArcType arcType,
                                            const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::nurbsProc(const OdGeNurbCurve3d& nurbs)
{
}

void OdGsOpenGLVectorizeView::ellipArcProc(const OdGeEllipArc3d& arc,
                                         OdGiArcType arcType, const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::ellipArcProc(const OdGePoint3d& center,
                                         const OdGeVector3d& majorRadius,
                                         const OdGeVector3d& minorRadius,
                                         const OdGePoint3d& startPoint,
                                         const OdGePoint3d& endPoint,
                                         OdGiArcType arcType,
                                         const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::textProc(const OdGePoint3d& position,
                                     const OdGeVector3d& u, const OdGeVector3d& v,
                                     const OdChar* msg, OdInt32 length, bool raw,
                                     const OdGiTextStyle* pTextStyle,
                                     const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::shapeProc(const OdGePoint3d& position,
                                      const OdGeVector3d& u, const OdGeVector3d& v,
                                      int shapeNo, const OdGiTextStyle* pStyle,
                                      const OdGeVector3d* pExtrusion)
{
}

void OdGsOpenGLVectorizeView::xlineProc(const OdGePoint3d&, const OdGePoint3d&)
{
}

void OdGsOpenGLVectorizeView::rayProc(const OdGePoint3d&, const OdGePoint3d&)
{
}

void OdGsOpenGLVectorizeView::metafileProc(const OdGePoint3d& origin,
                                           const OdGeVector3d& u,
                                           const OdGeVector3d& v,
                                           const OdGiMetafile* pMetafile,
                                           bool bDcAligned,
                                           bool bAllowClipping)
{
}
*/

void OdGsOpenGLVectorizeView::ownerDrawDc(const OdGePoint3d& origin, 
                                          const OdGeVector3d& u, const OdGeVector3d& v,
                                          const OdGiSelfGdiDrawable* pDrawable, 
                                          bool dcAligned, // = true
                                          bool allowClipping) // = false
{
  // CORE-11600 Ole object is not drawn on Mac
  ::exOwnerDrawDc(origin, u, v, pDrawable, (getRenderMode() == k2DOptimized) && dcAligned, allowClipping, *this,
                  GETBIT(m_flags, kModelCache), GETBIT(m_flags, kModelCache));
}

// Materials support

// Texture data container
OdGiOpenGLMaterialTextureData::OdGiOpenGLMaterialTextureData()
  : m_uWidth(0)
  , m_uHeight(0)
  , m_bInBGRA(false)
  , m_nBorder(0)
{
}

OdGiOpenGLMaterialTextureData::~OdGiOpenGLMaterialTextureData()
{
}

void OdGiOpenGLMaterialTextureData::setTextureData(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, const OdGiImageBGRA32 &image)
{
  const OdGiPixelBGRA32 *arrayBufferIn = image.image();
  // Rescale image to power of two here
  OdGsOpenGLVectorizeDevice *pDevice = ((OdGsOpenGLVectorizeDevice*)pDeviceInfo.getPtr());
  GLint maxTextureSize = pDevice->glMaxTextureSize();
  OdUInt32 destWidth = image.width(), destHeight = image.height();
  if (!pDevice->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_ARB_NPOT))
  {
    destWidth = 1;
    while (destWidth < image.width())
    {
      destWidth <<= 1;
      if (destWidth == OdUInt32(maxTextureSize))
      {
        break;
      }
    }
    destHeight = 1;
    while (destHeight < image.height())
    {
      destHeight <<= 1;
      if (destHeight == OdUInt32(maxTextureSize))
      {
        break;
      }
    }
  }
  else
  {
    if (destWidth > (OdUInt32)maxTextureSize)
      destWidth = (OdUInt32)maxTextureSize;
    if (destHeight > (OdUInt32)maxTextureSize)
      destHeight = (OdUInt32)maxTextureSize;
  }
  m_bInBGRA = false;
#ifdef GL_EXT_bgra
  if (pDevice->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_bgra))
  {
    m_bInBGRA = true;
  }
#endif
  if (destWidth == image.width() && destHeight == image.height())
  {
    // Copy without rescale
    OdUInt32 fullSize = image.width() * image.height();
    m_pxArray.resize(fullSize);
    OdGiPixelBGRA32 *arrayBufferOut = m_pxArray.asArrayPtr();
    if (m_bInBGRA)
    {
      memcpy(arrayBufferOut, arrayBufferIn, sizeof(OdGiPixelBGRA32) * fullSize);
    }
    else
    {
      for (OdUInt32 i = 0; i < fullSize; i++, arrayBufferIn++, arrayBufferOut++)
      {
        arrayBufferOut->setBGRA(arrayBufferIn->getRGBA());
      }
    }
    m_uWidth = image.width();
    m_uHeight = image.height();
  }
  else
  {
    // Rescale image
    m_pxArray.resize(destWidth * destHeight);
    OdGiPixelBGRA32 *arrayBufferOut = m_pxArray.asArrayPtr();
    double aspectX = double(image.width() - 1) / (destWidth - 1),
           aspectY = double(image.height() - 1) / (destHeight - 1);
    for (OdUInt32 iy = 0; iy < destHeight; iy++)
    {
      for (OdUInt32 ix = 0; ix < destWidth; ix++)
      {
        const OdGiPixelBGRA32 *pPtr = arrayBufferIn + OdUInt32(aspectY * iy) * image.width() + OdUInt32(aspectX * ix);
        if (m_bInBGRA)
          *arrayBufferOut = *pPtr;
        else
          arrayBufferOut->setBGRA(pPtr->getRGBA());
        ++arrayBufferOut;
      }
    }
    m_uWidth = destWidth;
    m_uHeight = destHeight;
  }
}

void OdGiOpenGLMaterialTextureData::textureData(OdGiPixelBGRA32Array &data, OdUInt32 &width, OdUInt32 &height) const
{
  data.clear();
  width = m_uWidth;
  height = m_uHeight;
  if (m_bInBGRA)
    data.append(m_pxArray);
  else if (m_pxArray.size())
  {
    data.resize(m_pxArray.size());
    OdGiPixelBGRA32 *pOut = data.asArrayPtr();
    const OdGiPixelBGRA32 *pIn = m_pxArray.asArrayPtr();
    OdUInt32 count = m_pxArray.size();
    while (count)
    {
      pOut->setBGRA(pIn->getRGBA());
      count--;
      pOut++;
      pIn++;
    }
  }
}

bool OdGiOpenGLMaterialTextureData::haveData() const
{
  return m_pxArray.size() > 0;
}

const OdGiPixelBGRA32 *OdGiOpenGLMaterialTextureData::deviceTexture() const
{
  return m_pxArray.asArrayPtr();
}

void OdGiOpenGLMaterialTextureData::renderTexture(OdGsOpenGLVectorizeDevice * /*pDevice*/)
{
  // Bind
  if (m_pxOpacity.size() == 0)
  {
    ::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    ::glPixelStorei(GL_PACK_ALIGNMENT, 1);
#ifdef GL_EXT_bgra
    ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, m_nBorder, (m_bInBGRA) ? GL_BGRA_EXT : GL_RGBA, GL_UNSIGNED_BYTE, deviceTexture());
#else
    ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_uWidth, m_uHeight, m_nBorder, GL_RGBA, GL_UNSIGNED_BYTE, deviceTexture());
#endif
  }
  else
  {
    ::glDisable(GL_POLYGON_STIPPLE);
    ::glPolygonStipple((const GLubyte*)m_pxOpacity.asArrayPtr());
    ::glEnable(GL_POLYGON_STIPPLE);
  }
}

void OdGiOpenGLMaterialTextureData::setBorder(GLint nBorder)
{
  m_nBorder = nBorder;
}

void OdGiOpenGLMaterialTextureData::convertToOpacityPattern()
{
  m_pxOpacity.resize(128);
  int nBt = 0;
  OdUInt8 bt;
  OdUInt32 uY, i;
  while (nBt < 128)
  {
    bt = 0;
    uY = nBt / 4 % 8;
    for (i = 0; i < 8; i++)
    {
      if (m_pxArray.at(uY * 8 + i).alpha() > 0)
        bt |= 1 << i;
    }
    for (i = 0; i < 4; i++, nBt++)
    {
      m_pxOpacity.at(nBt) = bt;
    }
  }
  m_pxArray.clear();
  m_uWidth = m_uHeight = 0;
}

void OdGiOpenGLMaterialTextureData::adjustAlphaComponent(double fValue)
{
  OdUInt8 iValue = (OdUInt8)OdRound(fValue * 255);
  OdUInt32 size = m_uWidth * m_uHeight;
  OdGiPixelBGRA32 *pixel = m_pxArray.asArrayPtr(); 
  for (OdUInt32 i = 0; i < size; i++, pixel++)
  {
    pixel->setAlpha(iValue);
  }
}

// RxClass
OdRxObjectPtr OdGiOpenGLMaterialTextureData::Desc::create() const
{
  return OdRxObjectPtr(OdRxObjectImpl<OdGiOpenGLMaterialTextureData>::createObject());
}
const OdString OdGiOpenGLMaterialTextureData::Desc::appName() const
{
  return OdString::kEmpty;
}
AppNameChangeFuncPtr OdGiOpenGLMaterialTextureData::Desc::appNameCallbackPtr() const
{
  return NULL;
}
const OdString OdGiOpenGLMaterialTextureData::Desc::dxfName() const
{
  return OdString::kEmpty;
}
const OdString OdGiOpenGLMaterialTextureData::Desc::name() const
{
  return OdString::kEmpty;
}
OdDb::DwgVersion OdGiOpenGLMaterialTextureData::Desc::getClassVersion(OdDb::MaintReleaseVer*) const
{
  return OdDb::kDHL_CURRENT;
}
OdUInt32 OdGiOpenGLMaterialTextureData::Desc::proxyFlags() const
{
  return 0;
}
OdRxClass* OdGiOpenGLMaterialTextureData::Desc::myParent() const
{
  return OdRxClass::desc();
}
OdRxModule* OdGiOpenGLMaterialTextureData::Desc::module() const
{
  return NULL;
}
void OdGiOpenGLMaterialTextureData::Desc::setConstructor(OdPseudoConstructorType)
{
}
OdUInt32 OdGiOpenGLMaterialTextureData::Desc::customFlags() const
{
  return 0;
}
OdRxOverruleIterator* OdGiOpenGLMaterialTextureData::Desc::getOverrule(OverrulingType t) const
{
	return NULL;
}
void OdGiOpenGLMaterialTextureData::Desc::setOverrule(OdRxOverruleIterator* pIt, OverrulingType t)
{
}
//

static void setupDefaultMaterial()
{
  const GLfloat sColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  const GLfloat zColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  ::glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
  ::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sColor);
  ::glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, zColor);
  ::glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  ::glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  ::glEnable(GL_COLOR_MATERIAL);
  ::glDisable(GL_TEXTURE_2D);
}

bool OdGsOpenGLVectorizeView::getDeviceMapperMatrix(OdGeMatrix3d &dm) const
{
  if (!view().cachedDrawables())
  {
    dm = getWorldToEyeTransform();
    return true;
  }
  return false;
}

bool OdGsOpenGLVectorizeView::skipMaterialProcess(OdDbStub *materialId) const
{
  return OdGsBaseMaterialVectorizer::skipMaterialProcess(materialId) || m_bDontShade;
}

void OdGsOpenGLVectorizeView::nullMaterialStub()
{
  renderMaterial(OdGiMaterialItemPtr());
}

//view.device()->getBackgroundColor()

OdGiMaterialItemPtr OdGsOpenGLVectorizeView::fillMaterialCache(OdGiMaterialItemPtr /*prevCache*/, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData)
{
  OdGiMaterialItemPtr pMatItem;
  pMatItem = OdGiMaterialItem::createObject();
  if (!materialId)
    return pMatItem;
  OdOpenGLMaterialData mData;
  //
  OdGiMaterialColor diffuseColor; OdGiMaterialMap diffuseMap;
  OdGiMaterialColor ambientColor; OdGiMaterialMap ambientMap;
  OdGiMaterialColor specularColor; OdGiMaterialMap specularMap; double glossFactor;
  double opacityPercentage; OdGiMaterialMap opacityMap; OdGiMaterialColor emissionColor;
  ODCOLORREF colorDiffuse(0), colorAmbient(0), colorSpecular(0), colorEmission(0);
  materialData.diffuse(diffuseColor, diffuseMap);
  materialData.ambient(ambientColor);
  materialData.specular(specularColor, specularMap, glossFactor);
  materialData.opacity(opacityPercentage, opacityMap);
  if (diffuseColor.color().colorMethod() == OdCmEntityColor::kByColor)
  {
    colorDiffuse = ODTOCOLORREF(diffuseColor.color());
  }
  else if (diffuseColor.color().colorMethod() == OdCmEntityColor::kByACI
           || diffuseColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    colorDiffuse = OdCmEntityColor::lookUpRGB((OdUInt8)diffuseColor.color().colorIndex());
  }
  if (ambientColor.color().colorMethod() == OdCmEntityColor::kByColor)
  {
    colorAmbient = ODTOCOLORREF(ambientColor.color());
  }
  else if (ambientColor.color().colorMethod() == OdCmEntityColor::kByACI
           || ambientColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    colorAmbient = OdCmEntityColor::lookUpRGB((OdUInt8)ambientColor.color().colorIndex());
  }
  if (specularColor.color().colorMethod() == OdCmEntityColor::kByColor)
  {
    colorSpecular = ODTOCOLORREF(specularColor.color());
  }
  else if (specularColor.color().colorMethod() == OdCmEntityColor::kByACI
           || specularColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    colorSpecular = OdCmEntityColor::lookUpRGB((OdUInt8)specularColor.color().colorIndex());
  }
  if (materialData.luminanceMode() == OdGiMaterialTraits::kEmissionColor)
  {
    OdGiMaterialMap emissionMap;
    materialData.emission(emissionColor, emissionMap);
    if (emissionColor.color().colorMethod() == OdCmEntityColor::kByColor)
    {
      colorEmission = ODTOCOLORREF(emissionColor.color());
    }
    else if (emissionColor.color().colorMethod() == OdCmEntityColor::kByACI
             || emissionColor.color().colorMethod() == OdCmEntityColor::kByDgnIndex)
    {
      colorEmission = OdCmEntityColor::lookUpRGB((OdUInt8)emissionColor.color().colorIndex());
    }
  }
  //
  mData.m_paAmbient[0] = GLfloat((double)ODGETRED(colorAmbient) / 255 * ambientColor.factor());
  mData.m_paAmbient[1] = GLfloat((double)ODGETGREEN(colorAmbient) / 255 * ambientColor.factor());
  mData.m_paAmbient[2] = GLfloat((double)ODGETBLUE(colorAmbient) / 255 * ambientColor.factor());
  mData.m_paAmbient[3] = 1.0f;
  mData.m_paDiffuse[0] = GLfloat((double)ODGETRED(colorDiffuse) / 255 * diffuseColor.factor());
  mData.m_paDiffuse[1] = GLfloat((double)ODGETGREEN(colorDiffuse) / 255 * diffuseColor.factor());
  mData.m_paDiffuse[2] = GLfloat((double)ODGETBLUE(colorDiffuse) / 255 * diffuseColor.factor());
  mData.m_paDiffuse[3] = 1.0f;
  mData.m_paSpecular[0] = GLfloat((double)ODGETRED(colorSpecular) / 255 * specularColor.factor());
  mData.m_paSpecular[1] = GLfloat((double)ODGETGREEN(colorSpecular) / 255 * specularColor.factor());
  mData.m_paSpecular[2] = GLfloat((double)ODGETBLUE(colorSpecular) / 255 * specularColor.factor());
  mData.m_paSpecular[3] = 1.0f;
  mData.m_fSpecularPower = (1.0f - GLfloat(glossFactor)) * 128;
  mData.m_paEmission[0] = GLfloat((double)ODGETRED(colorEmission) / 255 * emissionColor.factor());
  mData.m_paEmission[1] = GLfloat((double)ODGETGREEN(colorEmission) / 255 * emissionColor.factor());
  mData.m_paEmission[2] = GLfloat((double)ODGETBLUE(colorEmission) / 255 * emissionColor.factor());
  mData.m_paEmission[3] = 1.0f;
  mData.m_bUseAmbientMat = ambientColor.method() == OdGiMaterialColor::kOverride;
  mData.m_bUseDiffuseMat = diffuseColor.method() == OdGiMaterialColor::kOverride;
  mData.m_bUseSpecularMat = specularColor.method() == OdGiMaterialColor::kOverride;
  // OpenGL not support complete per-channel color overriding
  {
    mData.m_nUseChannel = 0;
    if (!mData.m_bUseAmbientMat && !mData.m_bUseDiffuseMat)
    {
      mData.m_bUseSpecularMat = true;
      mData.m_nUseChannel = GL_AMBIENT_AND_DIFFUSE;
    }
    else if (!mData.m_bUseDiffuseMat)
    {
      mData.m_bUseAmbientMat = mData.m_bUseSpecularMat = true;
      mData.m_nUseChannel = GL_DIFFUSE;
    }
    else if (!mData.m_bUseSpecularMat)
    {
      mData.m_bUseAmbientMat = mData.m_bUseDiffuseMat = true;
      mData.m_nUseChannel = GL_SPECULAR;
    }
    else if (!mData.m_bUseAmbientMat)
    {
      mData.m_bUseDiffuseMat = mData.m_bUseSpecularMat = true;
      mData.m_nUseChannel = GL_AMBIENT;
    }
  }
  // #15298 : fill completely black surfaces by specular highlighting
  // This is correct for "Shaded" visual styles, but invalid for "Realistic".
  // So, make more analysis to keep old behavior in cases when this is possible.
  if (mData.m_bUseSpecularMat && OdZero(mData.m_paSpecular[0] + mData.m_paSpecular[1] + mData.m_paSpecular[2]) &&
      (!mData.m_bUseDiffuseMat || OdZero(mData.m_paDiffuse[0] + mData.m_paDiffuse[1] + mData.m_paDiffuse[2])) &&
      isLightColor(device()->getPaletteBackground()))
    mData.m_paSpecular[0] = mData.m_paSpecular[1] = mData.m_paSpecular[2] = 1.0f, mData.m_fSpecularPower = 10.0f;
  //
  mData.m_fOpacityPercentage = opacityPercentage;
  mData.m_fDiffuseBlendFactor = 0.0;
  mData.m_uTiling = OdGiMapper::kTile;
  mData.m_vTiling = OdGiMapper::kTile;
  // Texture data, if available
  if (!GETBIT(materialData.channelFlags(), OdGiMaterialTraits::kUseDiffuse) ||
      !pMatItem->createDiffuseTexture()->setGiMaterialTexture((void*)device(), createMaterialDesc(),
                                                              giCtx(), diffuseMap, device()->materialTextureManager()))
  {
    pMatItem->removeDiffuseTexture();
  }
  else
  {
    mData.m_fDiffuseBlendFactor = diffuseMap.blendFactor();
    mData.m_uTiling = currentMapper(false)->diffuseMapper()->mapper().uTiling();
    mData.m_vTiling = currentMapper(false)->diffuseMapper()->mapper().vTiling();
#if 0
    /* Rendering become extremely slow and incorrect in case if texture borders used. Looks like my ATI Radeon HD 4550
       runs entirely through software pipeline in this case. GL_ARB_texture_border_clamp is supported, but this is
       doesn't mean that it is correctly implemented. Required alternative solution.                                    */
    if (currentMapper(false)->diffuseMapper()->mapper().uTiling() == OdGiMapper::kCrop ||
        currentMapper(false)->diffuseMapper()->mapper().vTiling() == OdGiMapper::kCrop)
      ((OdSmartPtr<OdGiOpenGLMaterialTextureData>)pMatItem->diffuseTexture()->textureData())->setBorder(1);
#endif
    if (mData.m_fDiffuseBlendFactor < 1.0)
    {
      ((OdSmartPtr<OdGiOpenGLMaterialTextureData>)pMatItem->diffuseTexture()->textureData())->adjustAlphaComponent(mData.m_fDiffuseBlendFactor);
    }
    else
    {
      // Set default material properties for texture modulation
      mData.m_paAmbient[0] = mData.m_paAmbient[1] = mData.m_paAmbient[2] = mData.m_paAmbient[3] = 1.0f;
      mData.m_paDiffuse[0] = mData.m_paDiffuse[1] = mData.m_paDiffuse[2] = mData.m_paDiffuse[3] = 1.0f;
      // Specular component loaded from material
      mData.m_bUseAmbientMat = mData.m_bUseDiffuseMat = true;
      mData.m_nUseChannel = 0;
    }
    mData.m_fSpecularPower = 1.0f;
  }
  OdSmartPtr<OdOpenGLMaterialDataContainer> pMatContainer = OdRxObjectPtr(OdRxObjectImpl<OdOpenGLMaterialDataContainer>::createObject());
  pMatContainer->setData(mData);
  pMatItem->setCachedData((OdRxObjectPtr)pMatContainer); // Don't forgot set cache to item
  return pMatItem;
}

void OdGsOpenGLVectorizeView::checkFaceMaterialChange()
{
  if (m_bTexCoords != isTextureCoordinatesEnabled(renderMode()))
  {
    endFaces(renderMode()); m_bCheckFaceMaterials = true;
    prepareShMFacesGeneration(NULL, renderMode());
    beginFaces(GL_TRIANGLES, renderMode());
  }
}

void OdGsOpenGLVectorizeView::renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub * /*materialId*/)
{
  if (m_bCheckFaceMaterials) checkFaceMaterialChange();
  renderMaterial(pCache);
}

void OdGsOpenGLVectorizeView::renderMaterial(OdGiMaterialItemPtr pMaterialItem)
{
  if (pMaterialItem.isNull())
  {
    if (isMaterialAvailable())
    {
      resetCurrentMaterial();
      setupDefaultMaterial();
      draw_fillStyle(OdOpenGLFillPatt::kFillPattMaterial, 0);
      return;
    }
    return;
  }
  if (pMaterialItem->materialId() == NULL)
  {
    setupDefaultMaterial();
    return;
  }
  OdSmartPtr<OdOpenGLMaterialDataContainer> pMatDataCont = pMaterialItem->cachedData();
  const OdOpenGLMaterialData &matData = pMatDataCont->data();
  // Render
  if (matData.m_nUseChannel == 0)
  {
    ::glDisable(GL_COLOR_MATERIAL);
  }
  else
  {
    ::glEnable(GL_COLOR_MATERIAL);
    ::glColorMaterial(GL_FRONT_AND_BACK, matData.m_nUseChannel);
  }
  if (matData.m_bUseAmbientMat)
    ::glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matData.m_paAmbient);
  if (matData.m_bUseDiffuseMat)
    ::glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matData.m_paDiffuse);
  if (matData.m_bUseSpecularMat)
    ::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matData.m_paSpecular);
  ::glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matData.m_fSpecularPower);
  ::glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matData.m_paEmission);
  // Texture
  if (pMaterialItem->haveDiffuseTexture() && matData.m_fDiffuseBlendFactor > 0.0)
  {
    ::glEnable(GL_TEXTURE_2D);
    OdSmartPtr<OdGiOpenGLMaterialTextureData> pTexture = pMaterialItem->diffuseTexture()->textureData();
    pTexture->renderTexture(device());
    if (matData.m_fDiffuseBlendFactor < 1.0)
    {
      ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    }
    else
    {
      ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    ::glDisable(GL_ALPHA_TEST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifndef OD_OGL_ES
    ::glDisable(GL_TEXTURE_GEN_S);
    ::glDisable(GL_TEXTURE_GEN_T);
#endif

    // Set addressing mode here
    switch (matData.m_uTiling)
    {
      case OdGiMapper::kCrop:
#ifndef OD_OGL_ES
        {
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
          GLint borderColor[4] = { 0, 0, 0, 0 }; // @@@TODO: GL_CLAMP willn't work I think. Should be changed to GL_CLAMP_TO_BORDER.
          ::glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
      break;
#endif
      case OdGiMapper::kClamp:
#ifdef GL_EXT_texture_edge_clamp
        if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_EdgeClamp))
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        else
#endif
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      break;
      case OdGiMapper::kMirror:
#ifdef GL_ARB_texture_mirrored_repeat
        if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_ARB_Mirror))
        {
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_ARB);
          break;
        }
#endif
      default:
        ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    };
    switch (matData.m_vTiling)
    {
      case OdGiMapper::kCrop:
#ifndef OD_OGL_ES
        {
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
          GLint borderColor[4] = { 0, 0, 0, 0 }; // @@@TODO: GL_CLAMP willn't work I think. Should be changed to GL_CLAMP_TO_BORDER.
          ::glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }
      break;
#endif
      case OdGiMapper::kClamp:
#ifdef GL_EXT_texture_edge_clamp
        if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_EdgeClamp))
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        else
#endif
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      break;
      case OdGiMapper::kMirror:
#ifdef GL_ARB_texture_mirrored_repeat
        if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_ARB_Mirror))
        {
          ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT_ARB);
          break;
        }
#endif
      default:
        ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    };
  }
  else
  {
    ::glDisable(GL_TEXTURE_2D);
  }
  draw_fillStyle(OdOpenGLFillPatt::kFillPattMaterial, 255 - ((OdInt32)(matData.m_fOpacityPercentage * 255)));
}

OdRxClassPtr OdGsOpenGLVectorizeView::createMaterialDesc() const
{
  return (OdRxClassPtr)OdRxObjectPtr(OdRxObjectImpl<OdGiOpenGLMaterialTextureData::Desc>::createObject());
}

void OdGsOpenGLVectorizeView::drawViewportFrame()
{
  if (isViewportBorderVisible())
  {
    // Disable previously set modes
    ::glViewport(0, 0, baseDevice()->width(), baseDevice()->height());
    ::glDisable(GL_LIGHTING);
    ::glDisable(GL_TEXTURE_2D);
    ::glDisable(GL_LINE_STIPPLE);
#ifndef OD_OGL_ALTERNATIVEHLT
    if (forceAlternativeHlt())
#endif
      alternativeHighlight(false);
    ::glDisable(GL_STENCIL_TEST);
    ::glDisable(GL_DEPTH_TEST);
    resetFading(false);

    bool bNrcClip = isNonRectClipped();

    // Setup transforms
    OdGeMatrix3d x;
    OdGeMatrix3d vptMtx;
    {
      OdGsDCRect dcrc = baseDevice()->outputRect();
      x(0, 0) = (dcrc.m_max.x - dcrc.m_min.x);
      x(1, 1) = (dcrc.m_max.y - dcrc.m_min.y);
      x(0, 3) = dcrc.m_min.x;
      x(1, 3) = dcrc.m_min.y;
      x.invert();

      OdGePoint2d lowerLeft, upperRight;
      getViewport(lowerLeft, upperRight);
      vptMtx(0, 0) = (upperRight.x - lowerLeft.x);
      vptMtx(1, 1) = (upperRight.y - lowerLeft.y);
      vptMtx(0, 3) = lowerLeft.x;
      vptMtx(1, 3) = lowerLeft.y;
      x.preMultBy(vptMtx);
    }
    x.preMultBy(OdGeMatrix3d::scaling(OdGeScale3d(2., 2., 1.), OdGePoint3d(1., 1., 0.)));

    ::glMatrixMode(GL_PROJECTION);
    device()->glLoadTransposedMatrix(x);

    ::glMatrixMode(GL_MODELVIEW);
    ::glLoadIdentity();

    // Load viewports
    OdIntArray nrcLoops;
    OdGePoint2dArray nrcPoints;
    if (bNrcClip)
    {
      viewportClipRegion(nrcLoops, nrcPoints);
    }
    else
    {
      nrcLoops.resize(1);
      nrcLoops[0] = 5;
      nrcPoints.resize(5);
      {
        OdGsDCRectDouble dcRect;
        getViewport(dcRect);
        nrcPoints[0] = dcRect.m_min; nrcPoints[2] = dcRect.m_max;
      }
      nrcPoints[1].set(nrcPoints[2].x, nrcPoints[0].y);
      nrcPoints[3].set(nrcPoints[0].x, nrcPoints[2].y);
      nrcPoints[4] = nrcPoints[0];
    }

    // Get border properties
    ODCOLORREF borderColor; int borderWidth;
    getViewportBorderProperties(borderColor, borderWidth);
    borderColor = device()->colorConvert(borderColor);

    // Render loops
    if (borderWidth <= 1 || (((int)m_fMaxLineWidth >= borderWidth) && device()->isLineLineweightOptimizationEnabled()))
    {
#ifndef OD_OGL_ES
      ::glColor3ub(ODGETRED(borderColor), ODGETGREEN(borderColor), ODGETBLUE(borderColor));
      ::glEnableClientState(GL_VERTEX_ARRAY);
      ::glVertexPointer(2, GL_DOUBLE, sizeof(OdGePoint2d), nrcPoints.getPtr());
#else
      ::glColor4ub(ODGETRED(borderColor), ODGETGREEN(borderColor), ODGETBLUE(borderColor), 255);
      OdArray<GLfloat> tmpBuf(nrcPoints.size() * 2, 1);
      GLfloat *pBuf = tmpBuf.asArrayPtr();
      for (OdUInt32 fltConv = 0; fltConv < nrcPoints.size(); fltConv++)
      {
        pBuf[fltConv * 2 + 0] = (GLfloat)nrcPoints[fltConv].x;
        pBuf[fltConv * 2 + 1] = (GLfloat)nrcPoints[fltConv].y;
      }
      ::glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, pBuf);
      ::glEnableClientState(GL_VERTEX_ARRAY);
#endif
      if (borderWidth > 1)
      {
        ::glLineWidth((GLfloat)borderWidth);
        ::glEnable(GL_LINE_SMOOTH);
        ::glPointSize((GLfloat)borderWidth);
        ::glEnable(GL_POINT_SMOOTH);
      }
      OdUInt32 prevFirst = 0, nLoops = nrcLoops.size();
      for (OdUInt32 nLoop = 0; nLoop < nLoops; prevFirst += nrcLoops[nLoop++])
      {
        glSafeDrawArrays(GL_LINE_STRIP, (GLint)prevFirst, (GLsizei)nrcLoops[nLoop]);
        if (borderWidth > 2)
          glSafeDrawArrays(GL_POINTS, (GLint)prevFirst, (GLsizei)nrcLoops[nLoop]);
      }
      ::glDisableClientState(GL_VERTEX_ARRAY);
      if (borderWidth > 1)
      {
        ::glLineWidth(1.0f);
        ::glPointSize(1.0f);
        ::glDisable(GL_LINE_SMOOTH);
        ::glDisable(GL_POINT_SMOOTH);
      }
    }
    else
    {
      ::glDisable(GL_POLYGON_STIPPLE);
      // For drivers which doesn't supports line width
      const OdGePoint2d *pPoints = nrcPoints.getPtr();
      OdUInt32 prevFirst = 0, nLoops = nrcLoops.size(), nLines;
      double vecLen = 0.5 * borderWidth;
      vptMtx.invert();
      OdUInt8 colorVec[4] = { ODGETRED(borderColor), ODGETGREEN(borderColor), ODGETBLUE(borderColor), 255 };
      for (OdUInt32 nLoop = 0; nLoop < nLoops; prevFirst += nrcLoops[nLoop++])
      {
        nLines = (OdUInt32)nrcLoops[nLoop];
        for (OdUInt32 nLine = prevFirst + 1; nLine < prevFirst + nLines; nLine++)
        {
          OdGePoint3d pt[2] = { OdGePoint3d(pPoints[nLine - 1].x, pPoints[nLine - 1].y, 0.0), OdGePoint3d(pPoints[nLine].x, pPoints[nLine].y, 0.0) };
          OdGeVector3d dirVec(pt[1] - pt[0]);
          if (!dirVec.isZeroLength())
          {
            dirVec.normalize();
            OdGeVector3d sideVec = dirVec.crossProduct(OdGeVector3d::kZAxis);
            renderLineCap(borderWidth, pt, (dirVec * vecLen).transformBy(vptMtx), (sideVec * vecLen).transformBy(vptMtx), colorVec);
          }
        }
      }
    }
  }
}

// Background rendering

void OdGsOpenGLVectorizeView::display(bool update)
{
  renderBackground();
  OdGsBaseVectorizer::display(update);
  // Rerender background if renderer in HidedLine state
  if (mode() == kHiddenLine)
  {
#ifndef OD_OGL_ES
    ::glPushAttrib(GL_STENCIL_BUFFER_BIT);
#else
    m_esStencilSaver.pushAttrib();
#endif
    ::glStencilFunc(GL_LESS, 1, 0xFFFF);
    ::glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    renderBackground(true);
#ifndef OD_OGL_ES
    ::glPopAttrib();
#else
    m_esStencilSaver.popAttrib();
#endif
  }
  //
}

void OdGsOpenGLVectorizeView::renderBackground(bool secondPass)
{
  // Draw viewport background and etc.
  if (viewportObjectId() != NULL)
  {
    // Prepare for background drawing
    GLboolean zState = ::glIsEnabled(GL_DEPTH_TEST);
    ::glDisable(GL_DEPTH_TEST);
    GLboolean lState = ::glIsEnabled(GL_LIGHTING);
    ::glDisable(GL_LIGHTING);
    OdGiFillType ftState = subEntityTraits().fillType();
    subEntityTraits().setFillType(kOdGiFillAlways);
    OdDbStub *layerState = subEntityTraits().layer();
    subEntityTraits().setLayer(NULL);
    RenderMode rmState = mode(); bool bLegacyHidden = isLegacyHiddenMode(), bLegacyWire = isLegacyWireframeMode();
    setMode(k2DOptimized);
    // Additional transformations if model cache disabled
    if (!cachedDrawables())
    {
      setEyeToOutputTransform(getEyeToWorldTransform());
      ::glMatrixMode(GL_MODELVIEW);
      OdGeMatrix3d mx(viewportToOutputMatrix());
      device()->glLoadTransposedMatrix(mx);
      if (metafileTransformOverrided())
        resetOutputXform(worldToMetafileMatrix());
    }
    OdGiGeometrySimplifier::setDrawContext(OdGsBaseVectorizer::drawContext());
    // AMark : #5365 : Skip dept at background vectorizing
    OdGeMatrix3d prjMx = projectionMatrixGL(true);
    GLdouble prevPrjMx[16];
    ::glGetDoublev(GL_PROJECTION_MATRIX, prevPrjMx);
    ::glMatrixMode(GL_PROJECTION);
    device()->glLoadTransposedMatrix(prjMx);
    //
    if (!secondPass)
    {
      updateViewportProperties(OdGsProperties::kDeviceNormal);
    }
    if (rmState != OdGsView::k2DOptimized)
    {
      OdGiDrawFlagsHelper saveDrawFlag(subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
#ifdef GL_VERSION_1_1
      displayViewportProperties(this, OdGsProperties::kDeviceNormal);
#else
      displayViewportProperties(NULL, OdGsProperties::kDeviceNormal);
#endif
      // Check if no background rendered - remove all background cache elements
#ifdef GL_VERSION_1_1
      OdGsPropertiesPtr props = getViewportPropertiesForType(OdGsProperties::kBackground);
      if (props.isNull())
      {
        for (OdUInt32 i = 0; i < m_driCache.size(); i++)
        {
          const DRICacheElement &elem = m_driCache.at(i);
          if (elem.isInitialized() && elem.isBackground())
          {
            m_driCache.removeAt(i);
          }
        }
      }
#endif
    }
    // Resore render settings
    // AMark : #5365 : Restore dept after background vectorizing
    ::glMatrixMode(GL_PROJECTION);
    ::glLoadMatrixd(prevPrjMx);
    //
    if (!cachedDrawables())
    {
      ::glMatrixMode(GL_MODELVIEW);
      ::glLoadIdentity(); 
      setEyeToOutputTransform(OdGeMatrix3d::kIdentity);
      if (metafileTransformOverrided())
        resetOutputXform(OdGeMatrix3d::kIdentity);
    }
    setMode(rmState); setLegacyHiddenMode(bLegacyHidden); setLegacyWireframeMode(bLegacyWire);
    OdGiGeometrySimplifier::setDrawContext(OdGsBaseVectorizer::drawContext());
    subEntityTraits().setFillType(ftState);
    subEntityTraits().setLayer(layerState);
    if (lState == GL_TRUE) glEnable(GL_LIGHTING);
    if (zState == GL_TRUE) glEnable(GL_DEPTH_TEST);
    //
  }
}

#ifdef GL_VERSION_1_1
void OdGsOpenGLVectorizeView::directRenderOutputImage(const OdGePoint3d *pRect, const OdGiRasterImage *pImage,
                                                      const OdGsPropertiesDirectRenderOutput::DirectRenderImageParams &driParams)
{
  ODA_ASSERT(driParams.pDrawable != NULL && pRect != NULL && pImage != NULL);
  // Search in cache for already cached texture
  DRICacheElement *elem = NULL;
  OdGiDrawable::DrawableType dType = driParams.pDrawable->drawableType();
  bool background = (dType == OdGiDrawable::kSolidBackground) || (dType == OdGiDrawable::kGradientBackground) ||
                    (dType == OdGiDrawable::kImageBackground) || (dType == OdGiDrawable::kGroundPlaneBackground) ||
                    (dType == OdGiDrawable::kSkyBackground);
  for (OdUInt32 i = 0; i < m_driCache.size(); i++)
  {
    const DRICacheElement &cem = m_driCache.at(i);
    if (cem.isInitialized())
    {
      if (cem.isEqual(driParams.pDrawable, pImage))
      {
        elem = &m_driCache.at(i);
      }
      else if (background && cem.isBackground())
      {
        m_driCache.removeAt(i);
      }
    }
  }
  if (elem == NULL)
  {
    elem = new DRICacheElement;
    elem->initialize(driParams.pDrawable, pImage, *this);
    m_driCache.push_back(*elem);
    delete elem;
    elem = &m_driCache.last();
  }
  // Render texture
  ::glEnable(GL_TEXTURE_2D);
  ::glBindTexture(GL_TEXTURE_2D, elem->uTexture);
  ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  ::glDisable(GL_TEXTURE_GEN_S);
  ::glDisable(GL_TEXTURE_GEN_T);

  OdGePoint3d tmp[4];
  if (metafileTransformOverrided())
  {
    const OdGeMatrix3d &pMat = worldToMetafileMatrix();
    for (int i = 0; i < 4; i++)
      tmp[i] = pMat * pRect[i];
    pRect = tmp;
  }

  ::glColor3ub(255, 255, 255);
  if (driParams.uvCoords == NULL)
  {
    // Render one copy of texture
#ifdef GL_EXT_texture_edge_clamp
    if (device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_EXT_EdgeClamp))
    {
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
#endif
    {
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
#ifndef OD_OGL_ES
    ::glBegin(GL_QUADS);
      ::glTexCoord2s(0, 0);
      ::glVertex3dv((const GLdouble*)(pRect + 0));
      ::glTexCoord2s(1, 0);
      ::glVertex3dv((const GLdouble*)(pRect + 1));
      ::glTexCoord2s(1, 1);
      ::glVertex3dv((const GLdouble*)(pRect + 2));
      ::glTexCoord2s(0, 1);
      ::glVertex3dv((const GLdouble*)(pRect + 3));
    ::glEnd();
#else
    const GLfloat uvs[8] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    const GLfloat vertxs[12] = { (GLfloat)pRect[0].x, (GLfloat)pRect[0].y, (GLfloat)pRect[0].z,
                                 (GLfloat)pRect[1].x, (GLfloat)pRect[1].y, (GLfloat)pRect[1].z,
                                 (GLfloat)pRect[2].x, (GLfloat)pRect[2].y, (GLfloat)pRect[2].z,
                                 (GLfloat)pRect[3].x, (GLfloat)pRect[3].y, (GLfloat)pRect[3].z };
    ::glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, uvs);
    ::glEnableClientState(GL_COLOR_ARRAY);
    ::oglEsDrawBuffer(3, vertxs, GL_TRIANGLE_FAN, 4);
    ::glDisableClientState(GL_COLOR_ARRAY);
#endif
  }
  else
  {
    // Repeat texture rendering
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
#ifndef OD_OGL_ES
    ::glBegin(GL_QUADS);
      ::glTexCoord2dv((const GLdouble*)(driParams.uvCoords + 0));
      ::glVertex3dv((const GLdouble*)(pRect + 0));
      ::glTexCoord2dv((const GLdouble*)(driParams.uvCoords + 1));
      ::glVertex3dv((const GLdouble*)(pRect + 1));
      ::glTexCoord2dv((const GLdouble*)(driParams.uvCoords + 2));
      ::glVertex3dv((const GLdouble*)(pRect + 2));
      ::glTexCoord2dv((const GLdouble*)(driParams.uvCoords + 3));
      ::glVertex3dv((const GLdouble*)(pRect + 3));
    ::glEnd();
#else
    const GLfloat uvs[8] = { (GLfloat)driParams.uvCoords[0].u, (GLfloat)driParams.uvCoords[0].v,
                             (GLfloat)driParams.uvCoords[1].u, (GLfloat)driParams.uvCoords[1].v,
                             (GLfloat)driParams.uvCoords[2].u, (GLfloat)driParams.uvCoords[2].v,
                             (GLfloat)driParams.uvCoords[3].u, (GLfloat)driParams.uvCoords[3].v };
    const GLfloat vertxs[12] = { (GLfloat)pRect[0].x, (GLfloat)pRect[0].y, (GLfloat)pRect[0].z,
                                 (GLfloat)pRect[1].x, (GLfloat)pRect[1].y, (GLfloat)pRect[1].z,
                                 (GLfloat)pRect[2].x, (GLfloat)pRect[2].y, (GLfloat)pRect[2].z,
                                 (GLfloat)pRect[3].x, (GLfloat)pRect[3].y, (GLfloat)pRect[3].z };
    ::glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, uvs);
    ::glEnableClientState(GL_COLOR_ARRAY);
    ::oglEsDrawBuffer(3, vertxs, GL_TRIANGLE_FAN, 4);
    ::glDisableClientState(GL_COLOR_ARRAY);
#endif
  }
  ::glBindTexture(GL_TEXTURE_2D, 0);
  ::glDisable(GL_TEXTURE_2D);
}

OdUInt32 OdGsOpenGLVectorizeView::directRenderOutputFlags() const
{
  // Background image must be rendered directly for this device, for more optimized rendering way,
  // caching of D3D textures and optimal textures wrapping.
  return DirectRender_Image;
}

OdGsOpenGLVectorizeView::DRICacheElement::DRICacheElement()
  : pDrawable(NULL)
  , pImage(NULL)
  , dType(OdGiDrawable::kViewport)
  , uTexture(0)
  , nRefs(0)
{
}

OdGsOpenGLVectorizeView::DRICacheElement::DRICacheElement(const OdGsOpenGLVectorizeView::DRICacheElement &celm)
  : pDrawable(celm.pDrawable)
  , pImage(celm.pImage)
  , dType(celm.dType)
  , uTexture(celm.uTexture)
  , nRefs(celm.nRefs)
{
  celm.nRefs++;
}

OdGsOpenGLVectorizeView::DRICacheElement::~DRICacheElement()
{
  destroy();
}

const OdGsOpenGLVectorizeView::DRICacheElement &OdGsOpenGLVectorizeView::DRICacheElement::operator =(const OdGsOpenGLVectorizeView::DRICacheElement &celm)
{
  pDrawable = celm.pDrawable;
  pImage = celm.pImage;
  dType = celm.dType;
  uTexture = celm.uTexture;
  nRefs = celm.nRefs;
  celm.nRefs++;
  return *this;
}

void OdGsOpenGLVectorizeView::DRICacheElement::initialize(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage, OdGsOpenGLVectorizeView &view)
{
  ODA_ASSERT(pDrawable != NULL && pImage != NULL && !isInitialized());
  this->pDrawable = pDrawable;
  this->pImage = pImage;
  dType = pDrawable->drawableType();
  OdUInt32 srcWidth  = pImage->pixelWidth();
  OdUInt32 srcHeight = pImage->pixelHeight();
  OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
  pDesc->setPixelWidth(srcWidth);
  pDesc->setPixelHeight(srcHeight);
  pDesc->setColorDepth(24); // Disable transparency for bkgnd
  pDesc->pixelFormat().setRGB();
  pDesc->setScanLinesAlignment(4);
  OdGiRasterImagePtr pCopyImg;
  pCopyImg = pImage->convert(true, 50.0, 50.0, 0.0, view.device()->getBackgroundColor(), false, false, false, pDesc);
  pImage = pCopyImg;
  // Create texture
  // Image must be in power-of-two
  GLint maxTextureSize = 1024;
  ::glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
  OdUInt32 destWidth = srcWidth, destHeight = srcHeight;
  if (!view.device()->isExtensionSupported(OdGsOpenGLVectorizeDevice::OpenGLExtension_ARB_NPOT))
  {
    destWidth = 1;
    while (destWidth < srcWidth)
    {
      destWidth <<= 1;
      if (destWidth == OdUInt32(maxTextureSize))
      {
        break;
      }
    }
    destHeight = 1;
    while (destHeight < srcHeight)
    {
      destHeight <<= 1;
      if (destHeight == OdUInt32(maxTextureSize))
      {
        break;
      }
    }
  }
  else
  {
    if (destWidth > (OdUInt32)maxTextureSize)
      destWidth = (OdUInt32)maxTextureSize;
    if (destHeight > (OdUInt32)maxTextureSize)
      destHeight = (OdUInt32)maxTextureSize;
  }

  const OdUInt8* pScanLines = NULL;
  OdUInt8Array aScanLinesExt;
  GLint packLength = 4;
  if (destWidth == srcWidth && destHeight == srcHeight)
  {
    pScanLines = pImage->scanLines();
    if (!pScanLines)
    {
      aScanLinesExt.resize(pImage->scanLineSize() * destHeight);
      pScanLines = aScanLinesExt.asArrayPtr();
      pImage->scanLines(aScanLinesExt.asArrayPtr(), 0, destHeight);
    }
  }
  else
  {
    // Rescale texture
    aScanLinesExt.resize(destWidth * destHeight * 3);
    OdUInt8 *pBits = aScanLinesExt.asArrayPtr();
    pScanLines = pBits;
    const OdUInt8 *pBitsTmp = pImage->scanLines();
    OdUInt8Array aBitsTmpBuf;
    if (!pBitsTmp)
    {
      aBitsTmpBuf.resize(pImage->scanLineSize() * destHeight);
      pBitsTmp = aBitsTmpBuf.asArrayPtr();
      pImage->scanLines(aBitsTmpBuf.asArrayPtr(), 0, destHeight);
    }
    OdUInt32 sLineSize = pImage->scanLineSize();
    double aspectX = double(srcWidth - 1) / (destWidth - 1),
           aspectY = double(srcHeight - 1) / (destHeight - 1);
    for (OdUInt32 iy = 0; iy < destHeight; iy++)
    {
      for (OdUInt32 ix = 0; ix < destWidth; ix++)
      {
        const OdUInt8 *pPtr = pBitsTmp + OdUInt32(aspectY * iy) * sLineSize + OdUInt32(aspectX * ix) * 3;
        *pBits = *pPtr; pBits++; pPtr++;
        *pBits = *pPtr; pBits++; pPtr++;
        *pBits = *pPtr; pBits++;
      }
    }
    packLength = 1; // Disable alignment
  }
  ODA_ASSERT(pScanLines != NULL);
  // Create texture on device
  ::glEnable(GL_TEXTURE_2D);
  GLuint gtt = 0;
  ::glGenTextures(1, &gtt);
  uTexture = (OdUInt32)gtt;
  ::glBindTexture(GL_TEXTURE_2D, gtt);
  ::glPixelStorei(GL_UNPACK_ALIGNMENT, packLength);
  ::glPixelStorei(GL_PACK_ALIGNMENT, packLength);
  ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, destWidth, destHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pScanLines);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  ::glDisable(GL_TEXTURE_GEN_S);
  ::glDisable(GL_TEXTURE_GEN_T);
  ::glBindTexture(GL_TEXTURE_2D, 0);
  ::glDisable(GL_TEXTURE_2D);
  nRefs++;
}

void OdGsOpenGLVectorizeView::DRICacheElement::destroy()
{
  if (isInitialized())
  {
    pDrawable = NULL;
    pImage = NULL;
    dType = OdGiDrawable::kViewport;
    if (--nRefs == 0)
    {
      ::glBindTexture(GL_TEXTURE_2D, 0);
      GLuint gtt = GLuint(uTexture);
      ::glDeleteTextures(1, &gtt);
      uTexture = 0;
    }
  }
}

bool OdGsOpenGLVectorizeView::DRICacheElement::isBackground() const
{
  return (dType == OdGiDrawable::kSolidBackground) || (dType == OdGiDrawable::kGradientBackground) ||
         (dType == OdGiDrawable::kImageBackground) || (dType == OdGiDrawable::kGroundPlaneBackground) ||
         (dType == OdGiDrawable::kSkyBackground);
}

bool OdGsOpenGLVectorizeView::DRICacheElement::isInitialized() const
{
  return (pDrawable != NULL) && (pImage != NULL) && (dType != OdGiDrawable::kViewport);
}

bool OdGsOpenGLVectorizeView::DRICacheElement::isEqual(const OdGiDrawable *pDrawable, const OdGiRasterImage *pImage) const
{
  return (this->pDrawable == pDrawable) && (this->pImage == pImage) && (dType == pDrawable->drawableType());
}

#endif // GL_VERSION_1_1

void OdGsOpenGLVectorizeView::renderLineCap(int iPx, const OdGePoint3d *origin, const OdGeVector3d &fwd, const OdGeVector3d &side,
                                            const OdUInt8 *color, OdUInt8 nColorComponents, const OdUInt8 *color2) const
{
  OdGeVector3d fwdVec(fwd), sideVec(side);
  if (mfTransformStack().hasStack())
  {
    fwdVec.transformBy(mfTransformStack().inverseMetafileTransform());
    sideVec.transformBy(mfTransformStack().inverseMetafileTransform());
  }
  if (m_lwdCaps.isEmpty())
  {
    OdGeVector3dArray tmpArray;
    m_lwdCaps.resize(OD_OGL_MAXLINECAPVERTS, tmpArray);
  }
  if (iPx < 3) iPx = 3;
  else if (iPx >= OD_OGL_MAXLINECAPVERTS) iPx = OD_OGL_MAXLINECAPVERTS - 1;
  if (m_lwdCaps[iPx].isEmpty())
  {
    OdGeVector3dArray &ca = m_lwdCaps[iPx];
    ca.resize(iPx << 1);
    double delta = OdaPI / (iPx - 1);
    int i;
    for (i = 0; i < iPx; i++)
    {
      ca[i].x = sin(delta * i);
      ca[i].y = cos(delta * i);
      ca[i].z = 0.0f;
    }
    for (i = 0; i < iPx; i++)
    {
      ca[i + iPx].x = -ca[i].x;
      ca[i + iPx].y = -ca[i].y;
      ca[i + iPx].z = 0.0f;
    }
  }
  const OdGeVector3d *inc = m_lwdCaps[iPx].asArrayPtr();
  int i;
#ifndef OD_OGL_ES
  ::glBegin(GL_TRIANGLE_FAN);
    if (color)
    {
      if (nColorComponents == 4)
        ::glColor4ubv(color);
      else
        ::glColor3ubv(color);
    }
    for (i = 0; i < iPx; i++)
    {
      ::glVertex3d(origin[0].x - fwdVec.x * inc[i].x + sideVec.x * inc[i].y,
                   origin[0].y - fwdVec.y * inc[i].x + sideVec.y * inc[i].y,
                   origin[0].z - fwdVec.z * inc[i].x + sideVec.z * inc[i].y);
    }
    if (color2)
    {
      if (nColorComponents == 4)
        ::glColor4ubv(color2);
      else
        ::glColor3ubv(color2);
    }
    for (; i < (iPx << 1); i++)
    {
      ::glVertex3d(origin[1].x - fwdVec.x * inc[i].x + sideVec.x * inc[i].y,
                   origin[1].y - fwdVec.y * inc[i].x + sideVec.y * inc[i].y,
                   origin[1].z - fwdVec.z * inc[i].x + sideVec.z * inc[i].y);
    }
  ::glEnd();
#else
  OdArray<GLfloat> tmpVerts((iPx << 1) * 3, 1);
  tmpVerts.resize((iPx << 1) * 3);
  GLfloat *pBuf = tmpVerts.asArrayPtr();
  int j = 0;
  if (color)
  {
    if (nColorComponents == 4)
      ::glColor4ub(color[0], color[1], color[2], color[3]);
    else
      ::glColor3ub(color[0], color[1], color[2]);
  }
  // @@@TODO: support gradient colors (if color2 specified)
  else if (color2)
  {
    if (nColorComponents == 4)
      ::glColor4ub(color2[0], color2[1], color2[2], color2[3]);
    else
      ::glColor3ub(color2[0], color2[1], color2[2]);
  }
  for (i = 0; i < iPx; i++, j += 3)
  {
    pBuf[j + 0] = (GLfloat)(origin[0].x - fwdVec.x * inc[i].x + sideVec.x * inc[i].y);
    pBuf[j + 1] = (GLfloat)(origin[0].y - fwdVec.y * inc[i].x + sideVec.y * inc[i].y);
    pBuf[j + 2] = (GLfloat)(origin[0].z - fwdVec.z * inc[i].x + sideVec.z * inc[i].y);
  }
  for ( ; i < (iPx << 1); i++, j += 3)
  {
    pBuf[j + 0] = (GLfloat)(origin[1].x - fwdVec.x * inc[i].x + sideVec.x * inc[i].y);
    pBuf[j + 1] = (GLfloat)(origin[1].y - fwdVec.y * inc[i].x + sideVec.y * inc[i].y);
    pBuf[j + 2] = (GLfloat)(origin[1].z - fwdVec.z * inc[i].x + sideVec.z * inc[i].y);
  }
  ::oglEsDrawBuffer(3, pBuf, GL_TRIANGLE_FAN, tmpVerts.size() / 3);
#endif
}

void OdGsOpenGLVectorizeView::clearLineCaps() const
{
  m_lwdCaps.clear();
}

const OdUInt8 *OdGsOpenGLVectorizeView::renderTransparency(OdUInt8 transp) const
{
  if (transp >= m_trpPats.size())
    m_trpPats.resize(((OdUInt32)transp) + 1);
  OdUInt8Array &patArray = m_trpPats[transp];
  if (patArray.empty())
  {
    OdGiMaterialTextureEntryPtr pTextureEntry = OdGiMaterialTextureEntry::createObject();
    pTextureEntry->setGiMaterialTexture((void*)const_cast<OdGsOpenGLVectorizeView*>(this)->device(), createMaterialDesc(), giCtx(), double(transp) / 255);
    static_cast<OdGiOpenGLMaterialTextureData*>(pTextureEntry->textureData().get())->convertToOpacityPattern();
    patArray = static_cast<OdGiOpenGLMaterialTextureData*>(pTextureEntry->textureData().get())->accessOpacityPattern();
  }
  return patArray.getPtr();
}

void OdGsOpenGLVectorizeView::clearTransparencies() const
{
  m_trpPats.clear();
}

// Snapshot support

void OdGsOpenGLVectorizeView::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  device()->getSnapShot(pImage, region, this);
}

void OdGsOpenGLVectorizeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  getSnapShot(pImage, region, NULL);
}

#include "SnapshotSupport.h"
#include "Gi/GiRasterWrappers.h"

void OdGsOpenGLVectorizeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView)
{
  struct SnapshotCallback : public OdGiSnapshotImageCallback
  {
    OdGiRasterImagePtr snapshotImageParams(OdGsBaseVectorizeDevice &runDevice) const
    {
      return static_cast<OdGsOpenGLVectorizeDevice&>(runDevice).snapshotImageParams();
    }
    OdGiRasterImagePtr snapshotRegion(OdGsBaseVectorizeDevice &runDevice, const OdGsDCRect &area, bool bCrop) const
    {
      return static_cast<OdGsOpenGLVectorizeDevice&>(runDevice).snapshotRegion(area, bCrop);
    }
  } _cbk;
  OdGiSnapshotImageImpl::getSnapshot(*this, &_cbk, pImage, region, pView);
}

OdGiRasterImagePtr OdGsOpenGLVectorizeDevice::snapshotImageParams() const
{
  OdSmartPtr<OdGiRasterImageDesc> pInfo = OdGiRasterImageDesc::createObject((OdUInt32)width(), (OdUInt32)height(), 24);
  pInfo->pixelFormat().setRGB();
  pInfo->setScanLinesAlignment(4);
  return pInfo;
}

OdGiRasterImagePtr OdGsOpenGLVectorizeDevice::snapshotRegion(const OdGsDCRect &area, bool /*bCrop*/) const
{
  if (!width() || !height())
    return OdGiRasterImagePtr();
  OdSmartPtr<OdGiSnapshotImageImpl> pImage = OdRxObjectImpl<OdGiSnapshotImageImpl>::createObject();
  pImage->pixelFormat().setRGB();
  pImage->setAlignment(4);
  pImage->setupSurface(OdUInt32(area.m_max.x - area.m_min.x), OdUInt32(area.m_max.y - area.m_min.y), 24);
  ::glPixelStorei(GL_PACK_ALIGNMENT, 4);
  ::glReadPixels(area.m_min.x, area.m_min.y, pImage->pixelWidth(), pImage->pixelHeight(), GL_RGB, GL_UNSIGNED_BYTE, pImage->scanLines());
  return pImage;
}

#define OD_OGL_DISPLAYWARN

bool OdGsOpenGLVectorizeView::validateMetafileExtents() const
{
  if (gsWriter().gsModel())
  { // Avoid caching of object with invalid extents: #6193
    OdGeExtents3d extents;
    const double dLimit = 1.e30;
    if (const_cast<OdGsOpenGLVectorizeView*>(this)->gsExtentsAccum().getExtents(extents))
    {
      // m_pOutputExtents->getExtents(extents); // if model disabled
      extents.transformBy(worldToMetafileMatrix());
      double diaLen = (extents.maxPoint() - extents.minPoint()).lengthSqrd();
      if (diaLen > dLimit * dLimit)
      {
#ifdef OD_OGL_DISPLAYWARN
        static bool bWarned = false;
        if (!bWarned)
        {
          bWarned = true;
          OdString warnStr;
          if (currentDrawable() && currentDrawable()->isPersistent())
            warnStr.format(OD_T("One of scene objects contains too large extents to display (class: %s)."), currentDrawable()->isA()->name().c_str());
          else
            warnStr.format(OD_T("One of scene objects contains too large extents to display. Invalid geometry removed."));
          OdGsDbRootLinkage::displayWarning(giContext().database(), warnStr);
        }
#endif
        return false;
      }
    }
  }
  return true;
}

//
