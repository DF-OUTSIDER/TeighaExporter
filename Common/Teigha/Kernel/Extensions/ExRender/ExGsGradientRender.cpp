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
#include "OdError.h"
#include "OdRound.h"

#include "Ge\GePoint2d.h"
#include "Gs\GsDefs.h"

#include "ExGsGradientRender.h"


ExGsGradientRender::ExGsGradientRender()
{
  m_nOriginX = 0;
  m_nOriginY = 0;
  m_nMinCX = 0;
  m_nMinCY = 0;
  m_nMaxCX = 0;
  m_nMaxCY = 0;
  m_pBuffer = NULL;
  m_nBufferSize = 0;
  m_nLength = 0;
  m_nHeight = 0;

  m_bIs32bpp = false;
  m_nAlpha = 255;
  m_clBackground = ODRGB( 0,0,0 );

  m_bIsInited = false;
}


ExGsGradientRender::~ExGsGradientRender()
{
}


bool ExGsGradientRender::setupContext32BPP( OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt8 *pBuffer, OdUInt8 alpha )
{
  m_bIsInited = false;
  if( !pBuffer ) return false;
  m_bIs32bpp = true;
  m_nAlpha = alpha;
  m_nLength = bufferWidth;
  m_nHeight = bufferHeight;
  m_pBuffer = pBuffer;
  //Set default clip rect
  m_nOriginX = 0;
  m_nOriginY = 0;
  m_nMinCX = 0;
  m_nMinCY = 0;
  m_nMaxCX = m_nLength;
  m_nMaxCY = m_nHeight;
  m_bIsInited = true;
  return true;
}


bool ExGsGradientRender::setupContext24BPP( OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt8 *pBuffer, ODCOLORREF backgroundColor )
{
  m_bIsInited = false;
  if( !pBuffer ) return false;
  m_bIs32bpp = false;
  m_clBackground = backgroundColor;
  m_nLength = bufferWidth;
  m_nHeight = bufferHeight;
  m_pBuffer = pBuffer;
  //Set default clip rect
  m_nOriginX = 0;
  m_nOriginY = 0;
  m_nMinCX = 0;
  m_nMinCY = 0;
  m_nMaxCX = m_nLength;
  m_nMaxCY = m_nHeight;
  m_bIsInited = true;
  return true;
}


void ExGsGradientRender::setOrigin( const OdGePoint2d& origin )
{
  m_nOriginX = origin.x;
  m_nOriginY = origin.y;
  OdUInt32 w = m_nMaxCX - m_nMinCX;
  OdUInt32 h = m_nMaxCY - m_nMinCY;
  m_nMinCX = m_nOriginX;
  m_nMaxCX = m_nOriginX + w;
  m_nMinCY = m_nOriginY;
  m_nMaxCY = m_nOriginY + h;
}


bool ExGsGradientRender::setClipRect( const OdGsDCRect& rect )
{
  if( rect.m_min.x > rect.m_max.x || rect.m_min.y > rect.m_max.y ) 
  {
    return false;
  }
  OdUInt32 w = rect.m_max.x - rect.m_min.x;
  OdUInt32 h = rect.m_max.y - rect.m_min.y;
  if( w > m_nLength ) 
  {
    w = m_nLength;
  }
  if( h > m_nHeight )
  {
    h = m_nHeight;
  }
  m_nMinCX = m_nOriginX;
  m_nMaxCX = m_nOriginX + w;
  m_nMinCY = m_nOriginY;
  m_nMaxCY = m_nOriginY + h;
  return true;
}


void ExGsGradientRender::eraseBackground()
{
  if( !m_bIsInited ) return;
  if( m_bIs32bpp )
  {
    memset( m_pBuffer, 0, m_nLength * m_nHeight * 4 );
  }
  else
  {
    OdUInt32 nPixels = m_nLength * m_nHeight;
    for( OdUInt32 pos = 0; pos < nPixels; ++pos )
    {
      m_pBuffer[ 3*pos ] = ODGETBLUE( m_clBackground );
      m_pBuffer[ 3*pos + 1 ] = ODGETGREEN( m_clBackground );
      m_pBuffer[ 3*pos + 2 ] = ODGETRED( m_clBackground );
    }
  }
}


bool ExGsGradientRender::renderTriangles( TRIVERTEX* verts, OdUInt32 nVerts, GRADIENT_TRIANGLE* triangles, OdUInt32 nTriangles )
{
  if( !m_bIsInited ) return false;
  if( !verts || !triangles ) return false;
  if( nVerts < 2 ) return false;
  if( nTriangles == 0 ) return false;

  for( OdUInt32 i = 0; i < nTriangles; ++i )
  {
    Edge edges[ 3 ] = 
    {
      Edge( verts[ triangles[ i ].Vertex1 ].x, verts[ triangles[ i ].Vertex1 ].y, verts[ triangles[ i ].Vertex1 ].Red, verts[ triangles[ i ].Vertex1 ].Green, verts[ triangles[ i ].Vertex1 ].Blue,
        verts[ triangles[ i ].Vertex2 ].x, verts[ triangles[ i ].Vertex2 ].y, verts[ triangles[ i ].Vertex2 ].Red, verts[ triangles[ i ].Vertex2 ].Green, verts[ triangles[ i ].Vertex2 ].Blue ),
      Edge( verts[ triangles[ i ].Vertex2 ].x, verts[ triangles[ i ].Vertex2 ].y, verts[ triangles[ i ].Vertex2 ].Red, verts[ triangles[ i ].Vertex2 ].Green, verts[ triangles[ i ].Vertex2 ].Blue,
        verts[ triangles[ i ].Vertex3 ].x, verts[ triangles[ i ].Vertex3 ].y, verts[ triangles[ i ].Vertex3 ].Red, verts[ triangles[ i ].Vertex3 ].Green, verts[ triangles[ i ].Vertex3 ].Blue ),
      Edge( verts[ triangles[ i ].Vertex3 ].x, verts[ triangles[ i ].Vertex3 ].y, verts[ triangles[ i ].Vertex3 ].Red, verts[ triangles[ i ].Vertex3 ].Green, verts[ triangles[ i ].Vertex3 ].Blue,
        verts[ triangles[ i ].Vertex1 ].x, verts[ triangles[ i ].Vertex1 ].y, verts[ triangles[ i ].Vertex1 ].Red, verts[ triangles[ i ].Vertex1 ].Green, verts[ triangles[ i ].Vertex1 ].Blue )
    };
    drawTriangle( edges );
  }
  return true;
}


void ExGsGradientRender::drawTriangle( Edge* edges )
{
  if( !edges ) return;
  OdInt32 maxLength = 0;
  OdInt8 longestEdge = -1;
  for( OdInt8 i = 0; i < 3; ++i )
  {
    OdInt32 l = edges[ i ].Y2 - edges[ i ].Y1;
    if( l > maxLength )
    {
      maxLength = l;
      longestEdge = i;
    }
  }

  if( longestEdge < 0 || longestEdge > 2 ) return;

  drawSpans( edges[ longestEdge ], edges[ (longestEdge + 1) % 3 ] );
  drawSpans( edges[ longestEdge ], edges[ (longestEdge + 2) % 3 ] );
}


void ExGsGradientRender::drawSpans( const Edge& e1, const Edge& e2 )
{
  OdInt32 diffYE1 = e1.Y2 - e1.Y1;
  if( diffYE1 == 0 ) return; 
  OdInt32 diffYE2 = e2.Y2 - e2.Y1;
  if( diffYE2 == 0 ) return;

  OdInt32 diffXE1 = e1.X2 - e1.X1;
  OdInt32 diffXE2 = e2.X2 - e2.X1;

  double diffColorE1R = e1.color2R - e1.color1R;
  double diffColorE1G = e1.color2G - e1.color1G;
  double diffColorE1B = e1.color2B - e1.color1B;

  double diffColorE2R = e2.color2R - e2.color1R;
  double diffColorE2G = e2.color2G - e2.color1G;
  double diffColorE2B = e2.color2B - e2.color1B;

  double factor1 = (double)( e2.Y1 - e1.Y1 ) / (double) diffYE1;
  double factor1Step = 1.0 / (double) diffYE1;
  double factor2 = 0.0;
  double factor2Step = 1.0 / (double) diffYE2;

  for( OdInt32 y = e2.Y1; y <= e2.Y2; ++y )
  {
    if( y < m_nMinCY || y > m_nMaxCY )
    {
      factor1 += factor1Step;
      factor2 += factor2Step;
      continue;
    }
    Span span( e1.X1 + OdInt32( OdTruncateToLong( diffXE1 * factor1 ) ), 
      e1.color1R + (diffColorE1R * factor1),
      e1.color1G + (diffColorE1G * factor1),
      e1.color1B + (diffColorE1B * factor1),
      e2.X1 + OdInt32( OdTruncateToLong( diffXE2 * factor2 ) ), 
      e2.color1R + (diffColorE2R * factor2),
      e2.color1G + (diffColorE2G * factor2),
      e2.color1B + (diffColorE2B * factor2) );

    drawSpan( span, y );

    factor1 += factor1Step;
    factor2 += factor2Step;
  }
}


void ExGsGradientRender::drawSpan( const Span& span, OdInt32 y )
{
  OdInt32 diffX = span.X2 - span.X1;
  if( diffX == 0 ) return;
  double diffColorR = span.color2R - span.color1R;
  double diffColorG = span.color2G - span.color1G;
  double diffColorB = span.color2B - span.color1B;

  double factor = 0.0;
  double factorStep = 1.0 / (double) diffX;

  for( OdInt32 x = span.X1; x <= span.X2; ++x )
  {
    if( x < m_nMinCX || x > m_nMaxCX )
    {
      factor += factorStep;
      continue;
    }
    OdUInt8 R, G, B;
    double tmp = span.color1R + (diffColorR * factor);
    if( tmp < 0 ) tmp = 0;
    if( tmp > 1.0 ) tmp = 1.0;
    R = OdUInt8( tmp * 255.0 );

    tmp = span.color1G + (diffColorG * factor);
    if( tmp < 0 ) tmp = 0;
    if( tmp > 1.0 ) tmp = 1.0;
    G = OdUInt8( tmp * 255.0 );

    tmp = span.color1B + (diffColorB * factor);
    if( tmp < 0 ) tmp = 0;
    if( tmp > 1.0 ) tmp = 1.0;
    B = OdUInt8( tmp * 255.0 );

    setPixel( x, y, R, G, B );

    factor += factorStep;
  }
}


void ExGsGradientRender::setPixel( OdInt32 x, OdInt32 y, OdUInt8 R, OdUInt8 G, OdUInt8 B )
{
  if( m_bIs32bpp )
  {
    OdUInt32 pos = ( y - m_nMinCY ) * ( m_nLength ) + x - m_nMinCX;

    pos *= 4;
    m_pBuffer[ pos++ ] = B;
    m_pBuffer[ pos++ ] = G;
    m_pBuffer[ pos++ ] = R;
    m_pBuffer[ pos ] = m_nAlpha;
  }
  else
  {

    OdUInt32 pos = ( y - m_nMinCY ) * ( m_nLength ) + x - m_nMinCX;

    pos *= 3;
    m_pBuffer[ pos++ ] = B;
    m_pBuffer[ pos++ ] = G;
    m_pBuffer[ pos ] = R;
  }
}
