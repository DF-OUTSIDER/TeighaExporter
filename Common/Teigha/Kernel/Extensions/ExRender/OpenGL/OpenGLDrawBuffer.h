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
// OpenGL active Read and Draw Buffer helpers

#ifndef ODOPENGLDRAWBUFFER
#define ODOPENGLDRAWBUFFER

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLResetDrawBuffer
{
  OdOpenGLResetDrawBuffer() { }
  virtual ~OdOpenGLResetDrawBuffer() { }
};

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLResetReadBuffer
{
  OdOpenGLResetReadBuffer() { }
  virtual ~OdOpenGLResetReadBuffer() { }
};

#ifdef GL_VERSION_1_1
// Desktop OpenGL implementations

/** \details
  <group ExRender_Classes>
*/
class OdOpenGLResetDrawBufferImpl : public OdOpenGLResetDrawBuffer
{
  GLint m_nPrevDraw, m_nNewDraw;
  public:
    OdOpenGLResetDrawBufferImpl(GLenum nNewDraw)
      : m_nPrevDraw(GL_BACK), m_nNewDraw(nNewDraw)
    {
      ::glGetIntegerv(GL_DRAW_BUFFER, &m_nPrevDraw);
      if (m_nNewDraw != m_nPrevDraw)
        ::glDrawBuffer(m_nNewDraw);
    }
    ~OdOpenGLResetDrawBufferImpl()
    {
      if (m_nPrevDraw != m_nNewDraw)
        ::glDrawBuffer(m_nPrevDraw);
    }
};
/** \details
  <group ExRender_Classes>
*/
class OdOpenGLResetReadBufferImpl : public OdOpenGLResetReadBuffer
{
  GLint m_nPrevRead, m_nNewRead;
  public:
    OdOpenGLResetReadBufferImpl(GLenum nNewRead)
      : m_nPrevRead(GL_FRONT), m_nNewRead(nNewRead)
    {
      ::glGetIntegerv(GL_READ_BUFFER, &m_nPrevRead);
      if (m_nNewRead != m_nPrevRead)
        ::glReadBuffer(m_nNewRead);
    }
    ~OdOpenGLResetReadBufferImpl()
    {
      if (m_nPrevRead != m_nNewRead)
        ::glReadBuffer(m_nPrevRead);
    }
};
/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLFrontReadBuffer : public OdOpenGLResetReadBufferImpl { OdOpenGLFrontReadBuffer() : OdOpenGLResetReadBufferImpl(GL_FRONT) { } };

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLBackReadBuffer : public OdOpenGLResetReadBufferImpl { OdOpenGLBackReadBuffer() : OdOpenGLResetReadBufferImpl(GL_BACK) { } };

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLNoneReadBuffer : public OdOpenGLResetReadBufferImpl { OdOpenGLNoneReadBuffer() : OdOpenGLResetReadBufferImpl(GL_NONE) { } };
#ifdef GL_COLOR_ATTACHMENT0

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLFrameReadBuffer : public OdOpenGLResetReadBufferImpl { OdOpenGLFrameReadBuffer() : OdOpenGLResetReadBufferImpl(GL_COLOR_ATTACHMENT0) { } };
#endif // GL_COLOR_ATTACHMENT0

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLFrontDrawBuffer : public OdOpenGLResetDrawBufferImpl { OdOpenGLFrontDrawBuffer() : OdOpenGLResetDrawBufferImpl(GL_FRONT) { } };

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLBackDrawBuffer : public OdOpenGLResetDrawBufferImpl { OdOpenGLBackDrawBuffer() : OdOpenGLResetDrawBufferImpl(GL_BACK) { } };

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLNoneDrawBuffer : public OdOpenGLResetDrawBufferImpl { OdOpenGLNoneDrawBuffer() : OdOpenGLResetDrawBufferImpl(GL_NONE) { } };
#ifdef GL_COLOR_ATTACHMENT0

/** \details
    <group ExRender_Classes>
*/
struct OdOpenGLFrameDrawBuffer : public OdOpenGLResetDrawBufferImpl { OdOpenGLFrameDrawBuffer() : OdOpenGLResetDrawBufferImpl(GL_COLOR_ATTACHMENT0) { } };
#endif // GL_COLOR_ATTACHMENT0

#else
// OpenGL ES implementations

typedef OdOpenGLResetReadBuffer OdOpenGLFrontReadBuffer;
typedef OdOpenGLResetReadBuffer OdOpenGLBackReadBuffer;
typedef OdOpenGLResetReadBuffer OdOpenGLNoneReadBuffer;
typedef OdOpenGLResetReadBuffer OdOpenGLFrameReadBuffer;

typedef OdOpenGLResetDrawBuffer OdOpenGLFrontDrawBuffer;
typedef OdOpenGLResetDrawBuffer OdOpenGLBackDrawBuffer;
typedef OdOpenGLResetDrawBuffer OdOpenGLNoneDrawBuffer;
typedef OdOpenGLResetDrawBuffer OdOpenGLFrameDrawBuffer;

#endif

#endif // ODOPENGLDRAWBUFFER
