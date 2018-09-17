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


/////////////////////////////////////////////////////////////////////
//
// HoopCommon.cpp: implementation of hoops export functions.
//
//////////////////////////////////////////////////////////////////////

#include "HoopsCommon.h"
#include "Ge/GeKnotVector.h"

namespace TD_HOOPS_COMMON_EXPORT {

#define BUFFER_SIZE_FOR_SEARCH 256
#define PIXEL_PER_MM        5

//===================================================================\\
//      implementation of class CGlyph2dGeometry                     \\
//===================================================================\\

CGlyph2dGeometry::CGlyph2dGeometry()
{
  m_arrVerticesData.clear();
  m_iMaxSize = 100;
  m_dXMax = -1e30;
  m_dXMin = 1e30;
  m_dYMax = -1e30;
  m_dYMin = 1e30;
  m_ptCenter.x = 0;
  m_ptCenter.y = 0;
  m_bFixedCenter = false;
}

//===================================================================\\

CGlyph2dGeometry::~CGlyph2dGeometry()
{
  m_arrVerticesData.clear();
}

//===================================================================\\

void CGlyph2dGeometry::setMaxSize( OdInt32 iSize )
{
  m_iMaxSize = iSize;
}

//===================================================================\\

void CGlyph2dGeometry::fixGlyphCenter()
{
  OdGePoint2d ptMin;
  OdGePoint2d ptMax;
  getMinMax( ptMin, ptMax );
  m_ptCenter.x = (ptMax.x + ptMin.x) / 2;
  m_ptCenter.y = (ptMax.y + ptMin.y) / 2;
  m_bFixedCenter = true;
}

//===================================================================\\

OdGePoint2d CGlyph2dGeometry::getGlyphCenter()
{
  return m_ptCenter;
}

//===================================================================\\

OdInt32 CGlyph2dGeometry::maxSize()
{
  return m_iMaxSize;
}

//===================================================================\\

double CGlyph2dGeometry::getRegionLength()
{
  return m_dXMax - m_dXMin;
}

//===================================================================\\

double CGlyph2dGeometry::getRegionHeight()
{
  return m_dYMax - m_dYMin;
}

//===================================================================\\

double CGlyph2dGeometry::getMaxY()
{
  return m_dYMax;
}

//===================================================================\\

double CGlyph2dGeometry::getMinY()
{
  return m_dYMin;
}
//===================================================================\\

void CGlyph2dGeometry::setLength( double dXMin, double dXMax )
{
  m_dXMax = dXMax;
  m_dXMin = dXMin;
}

//===================================================================\\

void CGlyph2dGeometry::setHeight( double dYMin, double dYMax )
{
  m_dYMax = dYMax;
  m_dYMin = dYMin;
}

//===================================================================\\

void CGlyph2dGeometry::Add3dGeometryItem( CGlyphGeometryInfo pInfo, bool bKeepXY )
{
  OdGePoint2dArray tmp2dPts;
  Convert3dPtArrayForGlyphDefine( pInfo.pVertices, tmp2dPts, bKeepXY );

  double dYMax = -1e30;
  double dYMin = 1e30;
  double dXMax = -1e30;
  double dXMin = 1e30;

  if( pInfo.bFillMode )
  {
    CGlyph2dGeometryInfo tmpRecord;
    tmpRecord.bFillMode = true;

    if( (OdInt32)tmp2dPts.size() > m_iMaxSize )
      reducePolylinePoints( tmp2dPts );

    for( OdUInt32 i = 0; i < tmp2dPts.size(); i++ )
    {
      if( tmp2dPts[i].y > dYMax )
        dYMax = tmp2dPts[i].y;

      if( tmp2dPts[i].y < dYMin )
        dYMin = tmp2dPts[i].y;

      if( tmp2dPts[i].x > dXMax )
        dXMax = tmp2dPts[i].x;

      if( tmp2dPts[i].x < dXMin )
        dXMin = tmp2dPts[i].x;

      tmpRecord.pVertices.push_back( tmp2dPts[i] );
    }

    if( tmpRecord.pVertices.size() > 1 )
      m_arrVerticesData.push_back( tmpRecord );
  }
  else
  {
    CGlyph2dGeometryInfo tmpRecord;
    tmpRecord.bFillMode = false;

    OdInt32 iCount = 0;

    for( OdUInt32 i = 0; i < tmp2dPts.size(); i++ )
    {
      if( tmp2dPts[i].y > dYMax )
        dYMax = tmp2dPts[i].y;

      if( tmp2dPts[i].y < dYMin )
        dYMin = tmp2dPts[i].y;

      if( tmp2dPts[i].x > dXMax )
        dXMax = tmp2dPts[i].x;

      if( tmp2dPts[i].x < dXMin )
        dXMin = tmp2dPts[i].x;

      tmpRecord.pVertices.push_back( tmp2dPts[i] );
      iCount++;

      if( iCount == m_iMaxSize )
      {
        m_arrVerticesData.push_back( tmpRecord );
        tmpRecord.pVertices.clear();
        tmpRecord.pVertices.push_back(tmp2dPts[i]);
        iCount = 1;
      }
    }

    if( tmpRecord.pVertices.size() > 1 )
      m_arrVerticesData.push_back( tmpRecord );
  }

  if( dYMin > 0 )
    dYMin = 0;

  if( dXMin > 0 )
    dXMin = 0;

  if( dYMax > m_dYMax )
    m_dYMax = dYMax;

  if( dYMin < m_dYMin)
    m_dYMin = dYMin;

  if( dXMax > m_dXMax )
    m_dXMax = dXMax;

  if( dXMin < m_dXMin)
    m_dXMin = dXMin;
}

//===================================================================\\

void CGlyph2dGeometry::getMinMax( OdGePoint2d& ptMin, OdGePoint2d& ptMax )
{
  bool bInitMinMax = false;

  for( OdUInt32 i = 0; i < m_arrVerticesData.size(); i++ )
  {
    for( OdUInt32 j = 0; j < m_arrVerticesData[i].pVertices.size(); j++ )
    {
      if( !bInitMinMax )
      {
        bInitMinMax = true;
        ptMin = m_arrVerticesData[i].pVertices[j];
        ptMax = m_arrVerticesData[i].pVertices[j];
      }
      else
      {
        if( m_arrVerticesData[i].pVertices[j].x > ptMax.x )
          ptMax.x = m_arrVerticesData[i].pVertices[j].x;
        else if( m_arrVerticesData[i].pVertices[j].x < ptMin.x )
          ptMin.x = m_arrVerticesData[i].pVertices[j].x;

        if( m_arrVerticesData[i].pVertices[j].y > ptMax.y )
          ptMax.y = m_arrVerticesData[i].pVertices[j].y;
        else if( m_arrVerticesData[i].pVertices[j].y < ptMin.y )
          ptMin.y = m_arrVerticesData[i].pVertices[j].y;
      }
    }
  }

  if( ptMin.y > 0 )
    ptMin.y = 0;

  if( ptMin.x > 0 )
    ptMin.x = 0;
}

//===================================================================\\

void CGlyph2dGeometry::rotateByAngle( double dAngle, bool bUpdateExtents )
{
  if( OdZero(dAngle) )
    return;

  OdGePoint2d ptMin;
  OdGePoint2d ptMax;

  if( bUpdateExtents )
    getMinMax(ptMin, ptMax);
  else
  {
    ptMax.x = m_dXMax;
    ptMin.x = m_dXMin;
    ptMax.y = m_dYMax;
    ptMin.y = m_dYMin;
  }

  OdGePoint2d ptCenter(0,0);
  OdGePoint2d ptZero(0,0);
  ptCenter.x = (ptMin.x + ptMax.x) / 2.0;
  ptCenter.y = (ptMin.y + ptMax.y) / 2.0;

  for( OdUInt32 i = 0; i < m_arrVerticesData.size(); i++ )
  {
    for( OdUInt32 j = 0; j < m_arrVerticesData[i].pVertices.size(); j++ )
    {
      m_arrVerticesData[i].pVertices[j].x -= ptCenter.x;
      m_arrVerticesData[i].pVertices[j].y -= ptCenter.y;

      m_arrVerticesData[i].pVertices[j] = m_arrVerticesData[i].pVertices[j].rotateBy( dAngle, ptZero );

      m_arrVerticesData[i].pVertices[j].x += ptCenter.x;
      m_arrVerticesData[i].pVertices[j].y += ptCenter.y;
    }
  }

  getMinMax(ptMin, ptMax);

  m_dXMax = ptMax.x;
  m_dXMin = ptMin.x;
  m_dYMax = ptMax.y;
  m_dYMin = ptMin.y;

  if( m_bFixedCenter )
    m_ptCenter.rotateBy( dAngle, ptZero );
}

//===================================================================\\

double CGlyph2dGeometry::ClipToRegion( double dSize, bool bMoveCenterToZero, bool bUpdateExtents )
{
  OdGePoint2d ptMin;
  OdGePoint2d ptMax;

  if( bUpdateExtents )
    getMinMax(ptMin, ptMax);
  else
  {
    ptMax.x = m_dXMax;
    ptMin.x = m_dXMin;
    ptMax.y = m_dYMax;
    ptMin.y = m_dYMin;
  }

  m_dXMax = ptMax.x;
  m_dXMin = ptMin.x;
  m_dYMax = ptMax.y;
  m_dYMin = ptMin.y;

  double m_dXLength = ptMax.x - ptMin.x;
  double m_dYLength = ptMax.y - ptMin.y;

  double dMaxLength = m_dXLength > m_dYLength ? m_dXLength : m_dYLength;

  OdGePoint2d ptStart = ptMin;

  double dScale = dSize / dMaxLength;

  if( bMoveCenterToZero )
  {
    ptStart.x = (ptMax.x + ptMin.x) / 2.0;
    ptStart.y = (ptMax.y + ptMin.y) / 2.0;
    dScale = dSize*2 / dMaxLength;
  }

  for( OdUInt32 i = 0; i < m_arrVerticesData.size(); i++ )
  {
    for( OdUInt32 j = 0; j < m_arrVerticesData[i].pVertices.size(); j++ )
    {
      m_arrVerticesData[i].pVertices[j].x -= ptStart.x;
      m_arrVerticesData[i].pVertices[j].y -= ptStart.y;
      m_arrVerticesData[i].pVertices[j].x *= dScale;
      m_arrVerticesData[i].pVertices[j].y *= dScale;
    }
  }

  return dScale;
}

//===================================================================\\

OdInt32 CGlyph2dGeometry::getnItems()
{
  return m_arrVerticesData.size();
}

//===================================================================\\

OdUInt32 CGlyph2dGeometry::getnCharsForGlyphDefine()
{
  OdUInt32 uiRet = 1;

  for( OdUInt32 i = 0; i < m_arrVerticesData.size(); i++ )
  {
    if( m_arrVerticesData[i].bFillMode )
      uiRet+= 2;

    uiRet += m_arrVerticesData[i].pVertices.size() * 2 + 1;
  }

  return uiRet;
}

//===================================================================\\

bool CGlyph2dGeometry::getItem( OdUInt32 iNum, CGlyph2dGeometryInfo& p2dInfo )
{
  p2dInfo.bFillMode = false;
  p2dInfo.pVertices.clear();

  if( iNum < m_arrVerticesData.size() )
  {
    p2dInfo.bFillMode = m_arrVerticesData[iNum].bFillMode;

    for( OdUInt32 i = 0; i < m_arrVerticesData[iNum].pVertices.size(); i++ )
      p2dInfo.pVertices.push_back( m_arrVerticesData[iNum].pVertices[i] );

    return true;
  }
  else
    return false;
}

//===================================================================\\

bool getPlanarEntity( OdGePoint3dArray GlyphLinePts, OdGePlane& PlaneEnt , bool bKeepXY)
{
  bool bRet = true;

  OdGePoint3d ptsPlane[3];
  ptsPlane[0] = GlyphLinePts[0];

  OdUInt32 iSecondPt = 1;
  bool bFindSecondPoint = false;
  bool bFindThirdPoint = false;

  for( OdUInt32 i = 1; i < GlyphLinePts.size(); i++ )
  {
    if( ptsPlane[0].distanceTo(GlyphLinePts[i]) > 1e-10 )
    {
      bFindSecondPoint = true;
      ptsPlane[1] = GlyphLinePts[i];
      iSecondPt = i;
      break;
    }
  }

  if( bFindSecondPoint )
  {
    for( OdUInt32 i = iSecondPt+1; i < GlyphLinePts.size(); i++ )
    {
      OdGeLine3d lineCheck( ptsPlane[0], ptsPlane[1] );

      if( !lineCheck.isOn( GlyphLinePts[i]) )
      {
        bFindThirdPoint = true;
        ptsPlane[2] = GlyphLinePts[i];
        break;
      }
    }
  }

  if( bFindThirdPoint )
  {
    double X1=ptsPlane[0].x;
    double X2=ptsPlane[1].x;
    double X3=ptsPlane[2].x;

    double Y1=ptsPlane[0].y;
    double Y2=ptsPlane[1].y;
    double Y3=ptsPlane[2].y;

    double Z1=ptsPlane[0].z;
    double Z2=ptsPlane[1].z;
    double Z3=ptsPlane[2].z;

    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;

    double M1=(Y2-Y1)*(Z3-Z1)-(Y3-Y1)*(Z2-Z1);
    double M2=(X2-X1)*(Z3-Z1)-(X3-X1)*(Z2-Z1);
    double M3=(X2-X1)*(Y3-Y1)-(X3-X1)*(Y2-Y1);

    if( OdZero(fabs( X1 - X3 )) && OdZero(fabs( X1 - X2 )) )
    {
      if( M1 >= 0 )
        a = 1;
      else
        a = -1;

      b = 0;
      c = 0;
      d = X1;
    }
    else if( OdZero(fabs( Y1 - Y3 )) && OdZero(fabs( Y1 - Y2 )) )
    {
      a = 0;

      if( M2 > 0 )
        b = -1;
      else
        b = 1;

      c = 0;
      d = Y1;
    }
    else if( OdZero(fabs( Z1 - Z3 )) && OdZero(fabs( Z1 - Z2 )) )
    {
      a = 0;
      b = 0;

      if( M3 > 0 || bKeepXY )
        c = 1;
      else
        c = -1;

      d = Z1;
    }
    else
    {
      a = M1;
      b = -1*M2;
      c = M3;
      d = Y1*M2-X1*M1-Z1*M3;
    }

    double dNorm = sqrt(a*a+b*b+c*c);
    PlaneEnt.set( a/dNorm, b/dNorm, c/dNorm, d/dNorm );
  }
  else
    bRet = false;

  return bRet;
}

//===================================================================\\

void CGlyph2dGeometry::Convert3dPtArrayForGlyphDefine( OdGePoint3dArray arr3dPts, OdGePoint2dArray& arr2dPts, bool bKeepXY )
{
  if( arr3dPts.size() == 1 )
    arr2dPts.push_back( OdGePoint2d( arr3dPts[0].x, arr3dPts[0].y ) );
  else if( arr3dPts.size() == 2 )
  {
    arr2dPts.push_back( arr3dPts[0].convert2d() );
    arr2dPts.push_back( arr3dPts[1].convert2d() );
  }
  else
  {
    OdGePlane ptsPlane;
    bool bCreatePlane = getPlanarEntity( arr3dPts, ptsPlane, bKeepXY );

    for( OdUInt32 i = 0; i < arr3dPts.size(); i++ )
    {
      if( bCreatePlane )
        arr2dPts.push_back( arr3dPts[i].convert2d(ptsPlane) );
      else
        arr2dPts.push_back( arr3dPts[i].convert2d() );
    }
  }
}

//===================================================================\\

void CGlyph2dGeometry::reducePolylinePoints( OdGePoint2dArray& arr2dPts )
{
  if( m_iMaxSize < 3)
    return;

  while( (OdInt32)arr2dPts.size() > m_iMaxSize )
  {
    double dMinLength = 1e30;
    OdInt32 iIndex    = -1;

    for( OdUInt32 i = 1; i < arr2dPts.size() - 1 ; i++ )
    {
      double dCurLen = arr2dPts[i-1].distanceTo( arr2dPts[i] ) + 
        arr2dPts[i].distanceTo( arr2dPts[i+1] ) +
        arr2dPts[i-1].distanceTo( arr2dPts[i+1] );

      if( dCurLen < dMinLength )
      {
        dMinLength = dCurLen;
        iIndex     = i;
      }
    }

    arr2dPts.erase( arr2dPts.begin() + iIndex, arr2dPts.begin() + iIndex + 1);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

char* OdStringToPChar( OdString strData )
{
  char* pchData = new char[ strData.getLength() + 1 ];
  Od_wcstombs( pchData, strData.c_str(), strData.getLength() );
  pchData[ strData.getLength() ] = '\0';
  return pchData;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsTextSize( double dTextSize )
{
  OdString tmpStr;
  tmpStr.format(L_FONT_SIZE_FORMAT, dTextSize);

  char* pchHoopsFontSize = OdStringToPChar( tmpStr );
  HC_Set_Text_Font( pchHoopsFontSize );
  delete[] pchHoopsFontSize;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
double getTtfScale( const OdFont* pFont )
{
  return (pFont->getHeight()) / ( pFont->fontAbove() - pFont->getBelow() + pFont->getInternalLeading());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsDefaultAlignment()
{
  HC_Set_Text_Alignment(HOOPS_TEXT_DEFAULT_ALIGN);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setText3dMode( bool bText3d )
{
  if( bText3d )
    HC_Set_Text_Font( HOOPS_SET_TEXT_3D );
  else
    HC_Set_Text_Font( HOOPS_SET_TEXT_2D );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsFontName( OdString strFontName, bool bTTF )
{
  if( !bTTF )
  {
    OdString strSetFont = "";
    strSetFont.format(L_SET_FONT_NAME_FORMAT, strFontName.c_str());

    char* pchHoopsFontName = OdStringToPChar( strSetFont );
    HC_Set_Text_Font( pchHoopsFontName );
    delete[] pchHoopsFontName;

    HC_Set_Text_Font( HOOPS_FONT_SET_DEFINED );
  }
  else
  {
    OdString strTTFName = "";
    OdString strTTFDir  = "";

    if( strFontName.reverseFind('/') != -1 && strFontName.reverseFind('\\') != -1 )
    {
      if( strFontName.reverseFind('/') > strFontName.reverseFind('\\') )
      {
        strTTFName = strFontName.right( strFontName.getLength() - strFontName.reverseFind('/') - 1);
        strTTFDir  = strFontName.left( strFontName.reverseFind('/') );
      }
      else
      {
        strTTFName = strFontName.right( strFontName.getLength() - strFontName.reverseFind('\\') - 1);
        strTTFDir  = strFontName.left( strFontName.reverseFind('\\') );
      }
    }
    else if( strFontName.reverseFind('/') != -1 )
    {
      strTTFName = strFontName.right( strFontName.getLength() - strFontName.reverseFind('/') - 1);
      strTTFDir  = strFontName.left( strFontName.reverseFind('/') );
    }
    else if( strFontName.reverseFind('\\') != -1 )
    {
      strTTFName = strFontName.right( strFontName.getLength() - strFontName.reverseFind('\\') - 1);
      strTTFDir  = strFontName.left( strFontName.reverseFind('\\') );
    }
    else
      strTTFName = strFontName;

    if( strTTFName != "")
    {
      OdString strSetFont = "";
      strSetFont.format(L_SET_FONT_NAME_FORMAT, strTTFName.c_str());

      char* pchHoopsFontName = OdStringToPChar( strSetFont );
      HC_Set_Text_Font( pchHoopsFontName );
      delete[] pchHoopsFontName;

      HC_Set_Text_Font( HOOPS_FONT_SET_TTF );
    }

    if( strTTFDir != "" )
    {
      OdString strSetFontDir = "";
      strSetFontDir.format(L_SET_FONT_DIR_FORMAT, strTTFDir.c_str());

      char* pchHoopsFontDir = OdStringToPChar( strSetFontDir );
      HC_Define_System_Options( pchHoopsFontDir );
      delete[] pchHoopsFontDir;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsLineWeight( double dLineWeight )
{
   HC_Set_Line_Weight( dLineWeight / 100.0 * PIXEL_PER_MM );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsLineType( OdString strLineType )
{
  if( strLineType == "" )
    strLineType = L_HOOPS_SOLID_LINE;
  
  char* pchHoopsLineType = OdStringToPChar( strLineType );
  HC_Set_Line_Pattern( pchHoopsLineType );
  delete[] pchHoopsLineType;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsObliquingAngle( double dAngle )
{
  OdString strFontAngle;
  strFontAngle.format(L_FONT_ANGLE_FORMAT, dAngle * 180 / OdaPI);

  char* pchHoopsFontAngle = OdStringToPChar( strFontAngle );
  HC_Set_Text_Font( pchHoopsFontAngle );
  delete[] pchHoopsFontAngle;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsFontBold( bool bBold )
{
  if( bBold )
    HC_Set_Text_Font( HOOPS_SET_BOLD );
  else
    HC_Set_Text_Font( HOOPS_UNSET_BOLD );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsFontItalic( bool bItalic )
{
  if( bItalic )
    HC_Set_Text_Font( HOOPS_SET_ITALIC );
  else
    HC_Set_Text_Font( HOOPS_UNSET_ITALIC );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsFontUnderlined( bool bUnderlined )
{
  if( bUnderlined )
    HC_Set_Text_Font( HOOPS_SET_UNDERLINED );
  else
    HC_Set_Text_Font( HOOPS_UNSET_UNDERLINED );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsFontOverlined( bool bOverlined )
{
  if( bOverlined )
    HC_Set_Text_Font( HOOPS_SET_OVERLINED );
  else
    HC_Set_Text_Font( HOOPS_UNSET_OVERLINED );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsOrientationByNormal( HC_KEY iKey, OdGeVector3d vrNorm )
{
  OdString strHoopsOrient;
  strHoopsOrient.format( L_HOOPS_LINE_ORIENTATION, vrNorm.x, vrNorm.y, vrNorm.z );
  char* pchHoopsOrient = OdStringToPChar( strHoopsOrient );
  HC_Edit_Geometry_Options( iKey, pchHoopsOrient );
  delete[] pchHoopsOrient;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsLineOrientation( HC_KEY iKey, OdGePoint3dArray ptsArr )
{
  if( ptsArr.size() < 2 )
    return;

  OdGeVector3d vrNorm(0,0,1);

  bool bUse2pts = false;

  if( ptsArr.size() < 3 )
    bUse2pts = true;

  if( !bUse2pts )
  {
    OdGePlane        planeEnt;

    if( getPlanarEntity(ptsArr, planeEnt) )
    {
      vrNorm = planeEnt.normal();
      vrNorm.normalize();
    }
    else
      bUse2pts = true;
  }

  if( bUse2pts )
  {
    if( !OdZero(ptsArr[1].distanceTo( ptsArr[0] ) ) )
    {
      OdGeVector3d OrdX(1,0,0);
      OdGeVector3d OrdZ(0,0,1);
      OdGeVector3d NewDir = ptsArr[1] - ptsArr[0];
      NewDir.normalize();
      OdGeVector3d XZProj( NewDir.x, 0, NewDir.z);

      double dYAngle = 0;

      if( !OdZero(XZProj.length()) )
      {
        XZProj.normalize();

        dYAngle = XZProj.angleTo( OrdX );
        OrdX.rotateBy( dYAngle, OdGeVector3d(0,1,0));

        if( !OdZero(OrdX.angleTo( XZProj ), 1e-4) )
          dYAngle = -dYAngle;
      }
      else
        dYAngle = 0;

      OrdZ.rotateBy( dYAngle, OdGeVector3d(0,1,0));

      vrNorm = OrdZ;
    }
  }

  setHoopsOrientationByNormal( iKey, vrNorm );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsFontWidthFactor( double dScale )
{
  OdString strWidthScale;
  strWidthScale.format(L_FONT_WIDTH_SCALE, dScale);

  char* pchHoopsFontWidthScale = OdStringToPChar( strWidthScale );
  HC_Set_Text_Font( pchHoopsFontWidthScale );
  delete[] pchHoopsFontWidthScale;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  OpenHoopsSegment( OdString strSegment )
{
  char* pchLineSegmentName = OdStringToPChar( strSegment );
  HC_Open_Segment( pchLineSegmentName );
  delete[] pchLineSegmentName;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool  isHoopsGlyphExists( HC_KEY iKey, OdString strGlyphName )
{
  bool bRet = false;

  char* pchGlyphName = new char[BUFFER_SIZE_FOR_SEARCH];
  pchGlyphName[0] = '\0';

  HC_Open_Segment_By_Key( iKey );
 
  HC_Begin_Glyph_Search(); 

  while( HC_Find_Glyph( pchGlyphName ) )
  {
    if( OdString(pchGlyphName) == strGlyphName )
    {
      bRet = true;
      break;
    }
  }

  HC_End_Glyph_Search();

  HC_Close_Segment();

  delete[] pchGlyphName;

  return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool  isHoopsLTypeExists( HC_KEY iKey, OdString strLTypeName )
{
  bool bRet = false;

  char* pchLTypeName = new char[BUFFER_SIZE_FOR_SEARCH];
  pchLTypeName[0] = '\0';

  HC_Open_Segment_By_Key( iKey );

  HC_Begin_Line_Style_Search(); 

  while( HC_Find_Line_Style( pchLTypeName ) )
  {
    if( OdString(pchLTypeName) == strLTypeName )
    {
      bRet = true;
      break;
    }
  }

  HC_End_Line_Style_Search();

  HC_Close_Segment();

  delete[] pchLTypeName;

  return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool  isHoopsSegmentExists( HC_KEY iKey, OdString strSegmentName )
{
  bool bRet = false;

  char* pchSegmentName = new char[BUFFER_SIZE_FOR_SEARCH];
  pchSegmentName[0] = '\0';

  char* pchStartSegmentName = new char[BUFFER_SIZE_FOR_SEARCH];
  pchStartSegmentName[0] = '\0';

  HC_Show_Segment( iKey, pchStartSegmentName );

  HC_Begin_Segment_Search( pchStartSegmentName ); 

  while( HC_Find_Segment( pchSegmentName ) )
  {
    if( OdString(pchSegmentName) == strSegmentName )
    {
      bRet = true;
      break;
    }
  }

  HC_End_Segment_Search();

  delete[] pchSegmentName;
  delete[] pchStartSegmentName;

  return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool  isHoopsTextureExists( HC_KEY iKey, OdString strTextureName )
{
  bool bRet = false;

  char* pchTextureName = new char[BUFFER_SIZE_FOR_SEARCH];
  pchTextureName[0] = '\0';

  HC_Open_Segment_By_Key( iKey );

  HC_Begin_Texture_Search(); 

  while( HC_Find_Texture( pchTextureName ) )
  {
    if( OdString(pchTextureName) == strTextureName )
    {
      bRet = true;
      break;
    }
  }

  HC_End_Texture_Search();

  HC_Close_Segment();

  delete[] pchTextureName;

  return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool  isHoopsFontExists( HC_KEY iKey, OdString strFontName )
{
  bool bRet = false;

  char* pchFontName = new char[BUFFER_SIZE_FOR_SEARCH];
  char* pchSegmentName = new char[BUFFER_SIZE_FOR_SEARCH];

  pchFontName[0] = '\0';

  HC_Show_Segment( iKey, pchSegmentName );

  HC_Begin_Font_Search(pchSegmentName, HOOPS_FONT_STYLE ); 

  while( HC_Find_Font( pchFontName ) )
  {
    if( OdString(pchFontName) == strFontName )
    {
      bRet = true;
      break;
    }
  }

  HC_End_Font_Search();

  delete[] pchFontName;
  delete[] pchSegmentName;

  return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void HoopsExportCircleArc( OdGeCircArc3d circArc, bool bExportGeometryOnly, bool bSkipTransform )
{
  double       dRadius        = circArc.radius();
  OdGeVector3d vrNormal       = circArc.normal();
  OdGeVector3d vrStart        = circArc.startPoint() - circArc.center();
  OdGePoint3d  ptArcCenter    = circArc.center();
  double       dSweepAngle    = circArc.endAng() - circArc.startAng();

  if( OdZero(fabs(dSweepAngle)) || OdZero(fabs( Oda2PI - dSweepAngle)) )
  { 
    ODA_ASSERT(0);
  }
  else
  {
    OdGeVector3d Wx(1, 0, 0);
    OdGeVector3d Wy(0, 1, 0);
    OdGeVector3d Wz(0, 0, 1);

    OdGeVector3d Ax,Ay,Az;

    Ax = vrStart;
    Ax.normalize();
    Ay = vrNormal.crossProduct(vrStart);
    Ay.normalize();
    Az = vrNormal;
    Az.normalize();

    double cos_X_alpha = Wx.dotProduct(Ax);
    double cos_X_beta  = Wy.dotProduct(Ax);
    double cos_X_theta = Wz.dotProduct(Ax);
    double cos_Y_alpha = Wx.dotProduct(Ay);
    double cos_Y_beta  = Wy.dotProduct(Ay);
    double cos_Y_theta = Wz.dotProduct(Ay);

    if( !bExportGeometryOnly )
    {
      if( !bSkipTransform )
      {
        HC_Translate_Object( ptArcCenter.x, ptArcCenter.y, ptArcCenter.z );
      }

      ptArcCenter.set(0,0,0);
    }

    HoopsPoint ptCenter;
    ptCenter.x = (float)(ptArcCenter.x);
    ptCenter.y = (float)(ptArcCenter.y);
    ptCenter.z = (float)(ptArcCenter.z);

    HoopsPoint ptMajor;
    ptMajor.x = (float)(ptCenter.x + dRadius*cos_X_alpha);
    ptMajor.y = (float)(ptCenter.y + dRadius*cos_X_beta);
    ptMajor.z = (float)(ptCenter.z + dRadius*cos_X_theta);

    HoopsPoint ptMinor;
    ptMinor.x = (float)(ptCenter.x + dRadius*cos_Y_alpha);
    ptMinor.y = (float)(ptCenter.y + dRadius*cos_Y_beta);
    ptMinor.z = (float)(ptCenter.z + dRadius*cos_Y_theta);

    double dStartAngle = 0.0;
    double dEndAngle = dSweepAngle/Oda2PI;

    if( (OdZero(ptCenter.x - ptMajor.x) && OdZero(ptCenter.y - ptMajor.y) && OdZero(ptCenter.z - ptMajor.z)) ||
      (OdZero(ptCenter.x - ptMinor.x) && OdZero(ptCenter.y - ptMinor.y) && OdZero(ptCenter.z - ptMinor.z)) ||
      (OdZero(ptMajor.x - ptMinor.x) && OdZero(ptMajor.y - ptMinor.y) && OdZero(ptMajor.z - ptMinor.z))
      )
    {
      if( !OdZero(ptMinor.x - ptMajor.x) || !OdZero(ptMinor.y - ptMajor.y) || !OdZero(!ptMinor.z - ptMajor.z) ) 
      {
        HoopsPoint ptsPline[2];
        ptsPline[0] = ptMinor;
        ptsPline[1] = ptMajor;
        HC_Insert_Polyline( 2, ptsPline);
      }
      else if( !OdZero(ptCenter.x - ptMajor.x) || !OdZero(ptCenter.y - ptMajor.y) || !OdZero(!ptCenter.z - ptMajor.z) ) 
      {
        HoopsPoint ptsPline[2];
        ptsPline[0] = ptCenter;
        ptsPline[1] = ptMajor;
        HC_Insert_Polyline( 2, ptsPline);
      }
    }
    else
    {
      HC_KInsert_Elliptical_Arc( &ptCenter,&ptMajor,&ptMinor, dStartAngle,dEndAngle );
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void HoopsExportEllipArc( OdGeEllipArc3d ellipArc, bool bExportGeometryOnly, bool bSkipTransform )
{
  OdGeVector3d vrMajorAxis = ellipArc.majorAxis();
  vrMajorAxis.normalize();

  OdGeVector3d vrMinorAxis = ellipArc.minorAxis();
  vrMinorAxis.normalize();

  double dMajorRadius = ellipArc.majorRadius();
  double dMinorRadius = ellipArc.minorRadius();

  HoopsPoint ptCenter;

  if( !bExportGeometryOnly )
  {
    if( !bSkipTransform )
    {
      HC_Translate_Object( ellipArc.center().x, ellipArc.center().y, ellipArc.center().z );
    }

    ptCenter.x = (float)(0);
    ptCenter.y = (float)(0);
    ptCenter.z = (float)(0);
  }
  else
  {
    ptCenter.x = (float)(ellipArc.center().x);
    ptCenter.y = (float)(ellipArc.center().y);
    ptCenter.z = (float)(ellipArc.center().z);
  }

  HoopsPoint ptMajor;
  ptMajor.x = (float)(ptCenter.x + vrMajorAxis.x * dMajorRadius);
  ptMajor.y = (float)(ptCenter.y + vrMajorAxis.y * dMajorRadius);
  ptMajor.z = (float)(ptCenter.z + vrMajorAxis.z * dMajorRadius);

  HoopsPoint ptMinor;
  ptMinor.x = (float)(ptCenter.x + vrMinorAxis.x * dMinorRadius);
  ptMinor.y = (float)(ptCenter.y + vrMinorAxis.y * dMinorRadius);
  ptMinor.z = (float)(ptCenter.z + vrMinorAxis.z * dMinorRadius);

  double dStartAngle = ellipArc.startAng()/Oda2PI;
  double dEndAngle   = ellipArc.endAng()/Oda2PI;

  if( dStartAngle > 1 )
    dStartAngle -= 1;
  else if( dStartAngle < -1 )
    dStartAngle += 1;

  if( dStartAngle < 0 )
    dStartAngle += 1;

  if( dEndAngle > 1 )
    dEndAngle -= 1;
  else if( dEndAngle < -1 )
    dEndAngle += 1;

  if( dEndAngle < 0 )
    dEndAngle += 1;

  if( (OdZero(ptCenter.x - ptMajor.x) && OdZero(ptCenter.y - ptMajor.y) && OdZero(ptCenter.z - ptMajor.z)) ||
        (OdZero(ptCenter.x - ptMinor.x) && OdZero(ptCenter.y - ptMinor.y) && OdZero(ptCenter.z - ptMinor.z)) ||
        (OdZero(ptMajor.x - ptMinor.x) && OdZero(ptMajor.y - ptMinor.y) && OdZero(ptMajor.z - ptMinor.z))
      )
  {
    if( !OdZero(ptMinor.x - ptMajor.x) || !OdZero(ptMinor.y - ptMajor.y) || !OdZero(!ptMinor.z - ptMajor.z) ) 
    {
      HoopsPoint ptsPline[2];
      ptsPline[0] = ptMinor;
      ptsPline[1] = ptMajor;
      HC_Insert_Polyline( 2, ptsPline);
    }
    else if( !OdZero(ptCenter.x - ptMajor.x) || !OdZero(ptCenter.y - ptMajor.y) || !OdZero(!ptCenter.z - ptMajor.z) ) 
    {
      HoopsPoint ptsPline[2];
      ptsPline[0] = ptCenter;
      ptsPline[1] = ptMajor;
      HC_Insert_Polyline( 2, ptsPline);
    }
  }
  else
  {
    HC_KInsert_Elliptical_Arc( &ptCenter,&ptMajor,&ptMinor, dStartAngle,dEndAngle );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void HoopsExportNurbs( OdGeNurbCurve3d nurbsCurve )
{
  int iDegree = 3;
  bool bRational = true;
  bool bPeriodic = true;
  OdGeKnotVector   knots;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray  weights;

  nurbsCurve.getDefinitionData(iDegree, bRational, bPeriodic, knots, controlPoints, weights);

  HoopsPoint* ptsArr     = new HoopsPoint[ controlPoints.size() ];
  float*      knotsArr   = new float[ knots.length() ];
  float*      weightsArr = new float[ controlPoints.size() ];

  for( OdUInt32 i = 0; i < controlPoints.size(); i++ )
  {
    ptsArr[i].x = (float)(controlPoints[i].x);
    ptsArr[i].y = (float)(controlPoints[i].y);
    ptsArr[i].z = (float)(controlPoints[i].z);

    if( weights.size() > i )
      weightsArr[i] = (float)weights[i];
    else
      weightsArr[i] = 1;
  }

  double dStartParam = nurbsCurve.startParam();
  double dEndParam = nurbsCurve.endParam();

  if( (dStartParam >= 0) && (dEndParam > 0) )
  {
    for( OdInt32 j = 0; j < knots.length(); j++ )
      knotsArr[j] = (float)(knots[j] / dEndParam);
  }
  else
  {
    knots.setRange(0,1);

    for( OdInt32 j = 0; j < knots.length(); j++ )
      knotsArr[j] = (float)(knots[j]);
  }

  HC_KInsert_NURBS_Curve(iDegree,controlPoints.size(), ptsArr, weightsArr, 
                         knotsArr, 0, 1 );

  delete[] ptsArr;
  delete[] knotsArr;
  delete[] weightsArr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void CreateRasterImage(
  OdString strShellName, 
  OdString strImageName, 
  OdString strTextureName,
  double   dWidth,
  double   dHeight,
  OdGePoint3dArray ptsShellArr, 
  OdGePoint2dArray ptsTextureArr )
{
  ODA_ASSERT( ptsTextureArr.size() == ptsShellArr.size() );

  HoopsPoint*   ptsHoopsShell   = new HoopsPoint[ ptsShellArr.size() ];
  Hoops2dPoint*  ptsHoopsTexture = new Hoops2dPoint[ ptsTextureArr.size() ];
  int*           iHoopsShellConnects = new int [ ptsShellArr.size() + 1];

  iHoopsShellConnects[0] = ptsShellArr.size();

  for( OdUInt32 i = 0; i < ptsShellArr.size(); i++ )
  {
    ptsHoopsShell[i].x = (float)ptsShellArr[i].x;
    ptsHoopsShell[i].y = (float)ptsShellArr[i].y;
    ptsHoopsShell[i].z = (float)ptsShellArr[i].z;

    ptsHoopsTexture[i].x = (float)(ptsTextureArr[i].x);
    ptsHoopsTexture[i].y = (float)(ptsTextureArr[i].y);

    iHoopsShellConnects[i+1] = i;
  }

  OdString strTextureOptions = "";
  strTextureOptions.format( L_RASTER_TEXTURE_OPTIONS, strImageName.c_str() );

  OdString strSetTexture = "";
  strSetTexture.format( L_SET_TEXTURE_FORMAT, strTextureName.c_str(), strTextureName.c_str());

  char* pchTextureName    = OdStringToPChar( strTextureName );
  char* pchTextureOptions = OdStringToPChar( strTextureOptions );
  char* pchSetTexture     = OdStringToPChar( strSetTexture );

  OpenHoopsSegment( strShellName );

  HC_Define_Local_Texture( pchTextureName, pchTextureOptions );

  HC_KEY iKey = HC_KInsert_Shell( ptsShellArr.size(), ptsHoopsShell, ptsShellArr.size() + 1, iHoopsShellConnects );

  HC_Scale_Texture( dWidth, dHeight, 1 );
  HC_Set_Color(pchSetTexture);
  HC_Set_Visibility( HOOPS_IMAGE_VISIBILITY_OPTIONS );
  HC_Set_Rendering_Options(HOOPS_IMAGE_RENDERING_OPTIONS);

  HC_Open_Geometry( iKey );

  for( OdUInt32 j = 0; j < ptsShellArr.size(); j++ )
  {
    HC_Open_Vertex( j );
    HC_Set_Parameter( 2, (float*)(ptsHoopsTexture + j) );
    HC_Close_Vertex();
  }

  HC_Close_Geometry();

  HC_Close_Segment();

  delete[] pchTextureName;
  delete[] pchTextureOptions;
  delete[] pchSetTexture;

  delete[] ptsHoopsShell;
  delete[] ptsHoopsTexture;
  delete[] iHoopsShellConnects;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

HC_KEY HoopsExportPolygon( OdInt32 numPoints, HoopsPoint* pPointArr )
{
  int* iHoopsShellConnects = new int [ numPoints + 1];

  iHoopsShellConnects[0] = numPoints;

  for( OdInt32 i = 0; i < numPoints; i++ )
    iHoopsShellConnects[i+1] = i;

  HC_KEY iKey = HC_KInsert_Shell( numPoints, pPointArr, numPoints + 1, iHoopsShellConnects );

  delete[] iHoopsShellConnects;

  return iKey;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

HC_KEY HoopsExportShell( OdInt32 numPoints, 
                         HoopsPoint* pPointArr,  
                         OdInt32 faceListSize, 
                         const OdInt32* faceList )
{
  int* piFaceList = new int[ faceListSize ];

  for( OdInt32 i = 0; i < faceListSize; i++ )
    piFaceList[i] = abs(faceList[i]);

  HC_KEY iKey = HC_KInsert_Shell( numPoints, pPointArr, faceListSize, piFaceList );

  delete[] piFaceList;

  return iKey;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

HC_KEY HoopsExportMesh( OdInt32 numRows, OdInt32 numColumns, const HoopsPoint* pPointArr )
{
  HC_KEY iKey = HC_KInsert_Mesh( numRows, numColumns, pPointArr );

  return iKey;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void ResetHoops()
{
  HC_Reset_System();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void HoopsSetClipRegion( OdGePoint3dArray ptsClipBoundary)
{
  HoopsPoint* ptsHoops = new HoopsPoint [ ptsClipBoundary.size() ];

  for( OdUInt32 i = 0; i < ptsClipBoundary.size(); i++ )
  {
    ptsHoops[i].x = (float)(ptsClipBoundary[i].x);
    ptsHoops[i].y = (float)(ptsClipBoundary[i].y);
    ptsHoops[i].z = (float)(ptsClipBoundary[i].z);
  }

  if( ptsClipBoundary.size() > 2)
  {
    HC_Set_Polygonal_Clip_Region( ptsClipBoundary.size(), ptsHoops, HOOPS_WORLD_CLIPPING );
  }

  delete[] ptsHoops;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void HoopsSetCamera( 
  OdGePoint3d ptCameraPos, 
  OdGePoint3d ptCameraTarget, 
  OdGeVector3d vrCameraUp, 
  double dCameraWidth, 
  double dCameraHeight,
  bool bPerspective,
  bool bStereoView,
  double /*dStereoDist*/, 
  double /*dStereoAngle*/)
{
  HoopsPoint ptPos;
  ptPos.x = (float)ptCameraPos.x;
  ptPos.y = (float)ptCameraPos.y;
  ptPos.z = (float)ptCameraPos.z;

  HoopsPoint ptTarget;
  ptTarget.x = (float)ptCameraTarget.x;
  ptTarget.y = (float)ptCameraTarget.y;
  ptTarget.z = (float)ptCameraTarget.z;

  HoopsPoint vrUpVector;
  vrUpVector.x = (float)vrCameraUp.x;
  vrUpVector.y = (float)vrCameraUp.y;
  vrUpVector.z = (float)vrCameraUp.z;

  if( bPerspective )
  {
    if( !bStereoView )
      HC_Set_Camera( &ptPos, &ptTarget, &vrUpVector, dCameraWidth, dCameraHeight, PERSPECTIVE_MODE);
    else
      HC_Set_Camera( &ptPos, &ptTarget, &vrUpVector, dCameraWidth, dCameraHeight, PERSPECTIVE_STEREO_MODE);
  }
  else
  {
    if( !bStereoView )
      HC_Set_Camera( &ptPos, &ptTarget, &vrUpVector, dCameraWidth, dCameraHeight, PARALLEL_MODE);
    else
      HC_Set_Camera( &ptPos, &ptTarget, &vrUpVector, dCameraWidth, dCameraHeight, PARALLEL_STEREO_MODE);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsVertexNormal( OdGeVector3d normal )
{
  if( normal.length() )
  {
    normal.normalize();
    HC_Set_Normal( normal.x, normal.y, normal.z );
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsEdgeVisibility( bool bShow )
{
  if( bShow )
    ;//HC_Set_Visibility( HOOPS_SHOW_EDGES );
  else
    HC_Set_Visibility( HOOPS_HIDE_EDGES );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsFaceVisibility( bool bShow )
{
  if( bShow )
    ;//HC_Set_Visibility( HOOPS_SHOW_FACES );
  else
    HC_Set_Visibility( HOOPS_HIDE_FACES );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void  setHoopsFaceNormal( OdGeVector3d normal )
{
  if( normal.length() )
    normal.normalize();

  HC_Set_Normal( normal.x, normal.y, normal.z );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void  UpdateShellFaceList( OdInt32* faceList, OdInt32 faceListSize, const OdGePoint3d* vertexList, OdInt32 )
{
  OdGeVector3d baseNormal;
  bool         bInitBaseNormal = false;
  OdInt32Array iIndexToChangeDirArr;
  OdGePoint3dArray ptsFace;
  OdGePlane        CheckPlane;

  OdInt32 iItemCount = 0;
  OdInt32 iFaceCount = 0;

  bool bUpdateFaceList = true;

  while( iItemCount < faceListSize )
  {
    OdInt32 nFacePts = abs(faceList[ iItemCount++ ]);

    if( nFacePts < 3 )
    {
      iItemCount += nFacePts;
      iFaceCount ++;
      continue;
    }
    else
    {
      ptsFace.clear();
      
      for( OdInt32 i = 0; i < nFacePts; i++ )
        ptsFace.push_back( vertexList[ faceList[ iItemCount++ ] ] );

      if( getPlanarEntity( ptsFace, CheckPlane ) )
      {
        if( !bInitBaseNormal )
        {
          bInitBaseNormal = true;
          baseNormal = CheckPlane.normal();
        }
        else
        {
          OdGeVector3d curNormal = CheckPlane.normal();

          if( !OdZero( curNormal.angleTo( baseNormal) ) )
          {
            if( OdZero( fabs(curNormal.angleTo( baseNormal)) - OdaPI ) )
              iIndexToChangeDirArr.push_back( iFaceCount );
            else
              bUpdateFaceList = false;
          }
        }
      }

      iFaceCount++;
    }

    if( !bUpdateFaceList )
      break;
  }

  iItemCount = 0;
  iFaceCount = 0;
  OdUInt32 iCurArrIndex = 0;

  if( bUpdateFaceList && iIndexToChangeDirArr.size() )
  {
    while( iItemCount < faceListSize )
    {
      OdInt32 nFacePts = abs(faceList[ iItemCount++ ]);

      if( nFacePts < 3 )
      {
        iItemCount += nFacePts;
        iFaceCount ++;
        continue;
      }
      else
      {
        if( iIndexToChangeDirArr[iCurArrIndex] == iFaceCount )
        {
          iCurArrIndex++;

          for( OdInt32 k = 0; k < (OdInt32)(nFacePts/2); k++ )
          {
            OdInt32 iTmp = faceList[ iItemCount + k ];
            faceList[ iItemCount + k ] = faceList[ iItemCount + nFacePts - 1 -  k ];
            faceList[ iItemCount + nFacePts - 1 -  k ] = iTmp;
          }
        }

        iItemCount += nFacePts;
      
        iFaceCount++;
      }

      if( iCurArrIndex >= iIndexToChangeDirArr.size() )
        break;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsAmbientColor( ODCOLORREF clrAmbient )
{
  double dRed   = ODGETRED( clrAmbient ) / 255.0;
  double dGreen = ODGETGREEN( clrAmbient ) / 255.0;
  double dBlue  = ODGETBLUE( clrAmbient ) / 255.0;

  OdString strColorOpts = "";
  strColorOpts.format( L_SET_AMBIENT_FORMAT, dRed, dGreen, dBlue );
  char* pchColorOpts = OdStringToPChar( strColorOpts );
  HC_Set_Color( pchColorOpts );
  delete[] pchColorOpts;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsDiffuseColor( ODCOLORREF clrDiffuse )
{
  double dRed   = ODGETRED( clrDiffuse ) / 255.0;
  double dGreen = ODGETGREEN( clrDiffuse ) / 255.0;
  double dBlue  = ODGETBLUE( clrDiffuse ) / 255.0;

  OdString strColorOpts = "";
  strColorOpts.format( L_SET_DIFFUSE_FORMAT, dRed, dGreen, dBlue );
  char* pchColorOpts = OdStringToPChar( strColorOpts );
  HC_Set_Color( pchColorOpts );
  delete[] pchColorOpts;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsSpecularColor( ODCOLORREF clrSpecular )
{
  double dRed   = ODGETRED( clrSpecular ) / 255.0;
  double dGreen = ODGETGREEN( clrSpecular ) / 255.0;
  double dBlue  = ODGETBLUE( clrSpecular ) / 255.0;

  OdString strColorOpts = "";
  strColorOpts.format( L_SET_SPECULAR_FORMAT, dRed, dGreen, dBlue );
  char* pchColorOpts = OdStringToPChar( strColorOpts );
  HC_Set_Color( pchColorOpts );
  delete[] pchColorOpts;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsGloss( double dGloss )
{
  OdString strColorOpts = "";
  strColorOpts.format( L_SET_GLOSS_FORMAT, dGloss * 29 + 1 );
  char* pchColorOpts = OdStringToPChar( strColorOpts );
  HC_Set_Color( pchColorOpts );
  delete[] pchColorOpts;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

bool InsertHoopsImage( OdGiRasterImagePtr pImage, 
                       OdString strImageName, 
                       bool bSetRealSize,
                       bool bForseBGR)
{
  if( pImage.isNull() )
    return false;

  OdGiRasterImage::PixelFormatInfo infoPix = pImage->pixelFormat();

  bool bBGR = bForseBGR;

  if( infoPix.isBGR() || infoPix.isBGRA() )
  {
    bBGR = true;
  }

  OdUInt32 uColorDepth = pImage->colorDepth();

  OdUInt32 nBytesPerPixel = (int)(uColorDepth/8);
  OdUInt32 nDataBytesForLine = pImage->scanLineSize();
  OdUInt32 nDataBytesPerLine = pImage->pixelWidth() * uColorDepth/8;
  OdUInt32 nDataBytes = nDataBytesPerLine*pImage->pixelHeight();

  OdUInt8* pImageData = new OdUInt8[ nDataBytes ];
  OdUInt8* pLineData  = new OdUInt8[ nDataBytesForLine ];

  OdInt32 iImageDataSize = 0;

  for( OdUInt32 i = 0; i < pImage->pixelHeight(); i++ )
  {
    pImage->scanLines( pLineData, i, 1);

    if( bBGR )
    {
      for( OdUInt32 j = 0; j < nDataBytesPerLine; j += nBytesPerPixel )
      {
        if( ( j + 2 ) < nDataBytesPerLine )
        {
          OdInt8 tmpByte = pLineData[j];
          pLineData[j] = pLineData[j + 2];
          pLineData[j + 2] = tmpByte;
        }
      }
    }

    memcpy( pImageData + iImageDataSize, pLineData, nDataBytesPerLine * sizeof( OdUInt8 ) );
    iImageDataSize += nDataBytesPerLine;
  }

  OdString strInsertImageFormat = "";

  if( bSetRealSize )
  {
    if( nBytesPerPixel > 3 )
      strInsertImageFormat.format(L_RGBA_SIZE_IMAGE_FORMAT, pImage->pixelWidth(), pImage->pixelHeight(), strImageName.c_str());
    else
      strInsertImageFormat.format(L_RGB_SIZE_IMAGE_FORMAT, pImage->pixelWidth(), pImage->pixelHeight(), strImageName.c_str());
  }
  else
  {
    if( nBytesPerPixel > 3 )
      strInsertImageFormat.format(L_RGBA_IMAGE_FORMAT, strImageName.c_str());
    else
      strInsertImageFormat.format(L_RGB_IMAGE_FORMAT, strImageName.c_str());
  }

  char* pchInsertImageFormat = OdStringToPChar( strInsertImageFormat );

  HC_Open_Segment( HOOPS_IMAGE_SEGMENT );

  HC_Insert_Image( 0,0,0, pchInsertImageFormat, pImage->pixelWidth(), pImage->pixelHeight(), pImageData );
  HC_Set_Visibility( HOOPS_HIDE_GEOMETRY );
  HC_Set_Heuristics( HOOPS_EXCLUDE_BOUNDING );

  HC_Close_Segment();

  delete[] pchInsertImageFormat;
  delete[] pLineData;
  delete[] pImageData;

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsTransparency( double dTransFactor )
{
  if( OdZero( dTransFactor ) )
    return;

  OdString strTransparent;
  strTransparent.format(L_SET_TRANSPARENT_FORMAT, dTransFactor,dTransFactor,dTransFactor);
  char* pchTransparent = OdStringToPChar( strTransparent );
  HC_Set_Color( pchTransparent );
  delete[] pchTransparent;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void setHoopsBgColor( ODCOLORREF clrBackground )
{
  OdString strBG;
  strBG.format(L_SET_BGCOLOR_FORMAT, ODGETRED(clrBackground) / 255.0, 
                                     ODGETGREEN(clrBackground) / 255.0,
                                     ODGETBLUE(clrBackground) / 255.0);
  char* pchBG = OdStringToPChar( strBG );
  HC_Set_Color( pchBG );
  delete[] pchBG;
}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
