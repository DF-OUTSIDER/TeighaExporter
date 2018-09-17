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

#ifndef _EXGSGRADIENTRENDER_H_INCLUDED_
#define _EXGSGRADIENTRENDER_H_INCLUDED_

class OdGsDCRect;

#if !defined(ODA_WINDOWS)

typedef struct _TRIVERTEX
{
    OdInt32   x;
    OdInt32   y;
    OdUInt16  Red;
    OdUInt16  Green;
    OdUInt16  Blue;
    OdUInt16  Alpha;
}TRIVERTEX,*PTRIVERTEX,*LPTRIVERTEX;

typedef struct _GRADIENT_TRIANGLE
{
    OdUInt32 Vertex1;
    OdUInt32 Vertex2;
    OdUInt32 Vertex3;
} GRADIENT_TRIANGLE,*PGRADIENT_TRIANGLE,*LPGRADIENT_TRIANGLE;

#endif
/** \details
  This class implements Geometry Gradient Triangles Render.
  <group ExRender_Classes> 
*/
class ExGsGradientRender
{
public:
  ExGsGradientRender();
  ~ExGsGradientRender();

  /** \details
    Sets the 32 bits per pixel draw context.
    \param bufferWidth [in]  Buffer width (in pixels).
    \param bufferHeight [in]  Buffer height (in pixels).
    \param pBuffer [in]  Pointer to the buffer.
    \param alpha [in]  Alpha channel value.
  */
  bool setupContext32BPP( OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt8 *pBuffer, OdUInt8 alpha = 255 );

  /** \details
    Sets the 24 bits per pixel draw context.
    \param bufferWidth [in]  Buffer width (in pixels).
    \param bufferHeight [in]  Buffer height (in pixels).
    \param pBuffer [in]  Pointer to the buffer.
    \param backgroundColor [in]  Background color.
  */
  bool setupContext24BPP( OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt8 *pBuffer, ODCOLORREF backgroundColor );

  /** \details
    Maps the buffer point (0,0) to the origin.
    \param origin [in]  Origin point.
  */
  void setOrigin( const OdGePoint2d& origin );
  
  /** \details
    Sets the clipping rectangle.
    \param clipRect [in]  Clipping rectangle.
    \remarks
    If rectangle is bigger than buffer it will be reduced to appropriate size.
  */
  bool setClipRect( const OdGsDCRect& clipRect );

  /** \details
    Clears background.
    \param clipRect [in]  Clipping rectangle.
    \remarks
    In 32-bpp case it fills all bytes by 0.
    In 24-bpp case if fills background by background color.
  */
  void eraseBackground();

  /** \details
    Render gradient triangles to the context.
    \param verts [in]  Pointer to vertices.
    \param nVerts [in]  Vertices count.
    \param triangles [in]  Pointer to triangles.
    \param nTriangles [in]  Triangles count.
    \remarks
    All trivial triangles will be skipped.
  */
  bool renderTriangles( TRIVERTEX* verts, OdUInt32 nVerts, GRADIENT_TRIANGLE* triangles, OdUInt32 nTriangles );

private:

  OdInt32  m_nOriginX;
  OdInt32  m_nOriginY;
  OdInt32  m_nMinCX;
  OdInt32  m_nMinCY;
  OdInt32  m_nMaxCX;
  OdInt32  m_nMaxCY;
  OdUInt8*  m_pBuffer;
  OdUInt32  m_nBufferSize;
  OdUInt32  m_nLength;
  OdUInt32  m_nHeight;

  bool        m_bIs32bpp;
  OdUInt8     m_nAlpha;
  ODCOLORREF  m_clBackground;


  bool      m_bIsInited;

  void setPixel( OdInt32 x, OdInt32 y, OdUInt8 R, OdUInt8 G, OdUInt8 B );

  struct Edge
  {
    double color1R, color1G, color1B;
    double color2R, color2G, color2B;

    OdInt32 X1, Y1, X2, Y2;

    Edge( OdInt32 x1, OdInt32 y1, OdUInt16 Color1R, OdUInt16 Color1G, OdUInt16 Color1B,
      OdInt32 x2, OdInt32 y2, OdUInt16 Color2R, OdUInt16 Color2G, OdUInt16 Color2B )
    {
      if( y1 < y2 )
      {
        color1R = (double)Color1R / (double)( 0xffff );
        color1G = (double)Color1G / (double)( 0xffff );
        color1B = (double)Color1B / (double)( 0xffff );
        X1 = x1;
        Y1 = y1;
        color2R = (double)Color2R / (double)( 0xffff );
        color2G = (double)Color2G / (double)( 0xffff );
        color2B = (double)Color2B / (double)( 0xffff );
        X2 = x2;
        Y2 = y2;
      }
      else
      {
        color1R = (double)Color2R / (double)( 0xffff );
        color1G = (double)Color2G / (double)( 0xffff );
        color1B = (double)Color2B / (double)( 0xffff );
        X1 = x2;
        Y1 = y2;
        color2R = (double)Color1R / (double)( 0xffff );
        color2G = (double)Color1G / (double)( 0xffff );
        color2B = (double)Color1B / (double)( 0xffff );
        X2 = x1;
        Y2 = y1;
      }
    }
  };

  void drawTriangle( Edge* edges );
  void drawSpans( const Edge& e1, const Edge& e2 );

  struct Span
  {
    double color1R, color1G, color1B;
    double color2R, color2G, color2B;

    OdInt32 X1, X2;

    Span( OdInt32 x1, double Color1R, double Color1G, double Color1B,
      OdInt32 x2, double Color2R, double Color2G, double Color2B )
    {
      if( x1 < x2 )
      {
        color1R = Color1R;
        color1G = Color1G;
        color1B = Color1B;
        X1 = x1;
        color2R = Color2R;
        color2G = Color2G;
        color2B = Color2B;
        X2 = x2;
      }
      else
      {
        color1R = Color2R;
        color1G = Color2G;
        color1B = Color2B;
        X1 = x2;
        color2R = Color1R;
        color2G = Color1G;
        color2B = Color1B;
        X2 = x1;
      }
    }

  };

  void drawSpan( const Span& span, OdInt32 y );

};

#endif
