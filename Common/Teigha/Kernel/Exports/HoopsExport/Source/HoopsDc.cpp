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


//////////////////////////////////////////////////////////////////////
//
// HoopsDc.cpp: implementation of the GsHoopsFactory, GsHoopsDevice, GsHoopsView classes.
//
//////////////////////////////////////////////////////////////////////


#include "HoopsExportCommon.h"
#include "HoopsExportDevice.h"
#include "HoopsExportView.h"
#include "HoopsCommon.h"

#include "Gi/GiRasterImage.h"
#include "Gi/GiRasterWrappers.h"
#include "Gi/GiRasterImageLoader.h"

namespace TD_HOOPS_COMMON_EXPORT {

#define GLIPH_POLYLINE_MAX  125
#define GLYPH_DEFAULT_SIZE  62
#define TEXT_RECT_SIZE      62


struct CDwgCharInfo
{
    OdChar            chCode;
    double            chWidth;
    double            chAbove;
    CGlyph2dGeometry  chGeometry;
    bool              bBigFont;
};

#ifndef ODA_WINDOWS
typedef unsigned short      WORD;
#endif

//===================================================================\\
//      implementation of OdHoopsExportDevice class                  \\
//===================================================================\\

void OdHoopsExportDevice::dc_init()
{
  if( m_pHoops && m_pHoops->m_Params.iStartKey == 0 )
  {
    ResetHoops();
    m_pHoops->m_Params.iStartKey = HC_KOpen_Segment( DRAWING_SEGMENT );
    HC_Close_Segment();
  }

  HC_Open_Segment_By_Key( m_pHoops->m_Params.iStartKey );
  HC_Set_Visibility (HOOPS_SHOW_GEOMETRY);
  HC_Set_Handedness(HOOPS_RIGHT_CS);
  setHoopsBgColor( m_pHoops->getParams().background );
  HC_Close_Segment();
}

//===================================================================\\

OdString OdHoopsExportDevice::DefineHoopsLineType( OdGiLinetype& curLS, 
                                                    double dLTypeScale, 
                                                      bool& bShapesInLT )
{
  OdString strRet = OdString::kEmpty;

  // Export shapes

  for( OdInt32 iCurDash = 0; iCurDash < curLS.numDashes(); iCurDash++ )
  {
    OdGiLinetypeDash curDash;
    curLS.dashAt(iCurDash, curDash);

    if( curDash.styleId )
    {
      AddGlyph( curDash );
      bShapesInLT = true;
    }
  }

  // Create line style definition

  OdString strLineTypeDefine = OdString::kEmpty;

  bool bContinuousLineType = true;

  for( OdInt32 iDash = 0; iDash < curLS.numDashes(); iDash++ )
  {
    bContinuousLineType = false;

    OdGiLinetypeDash curDash;
    curLS.dashAt(iDash, curDash);

    OdDbStub    *pShapeStyle     = curDash.styleId;
    double       dDashLength     = curDash.length*dLTypeScale;

    if( pShapeStyle )
    {
      bool bShapeUSCOrient = curDash.isRotationAbsolute();

      OdString strBufer = OdString::kEmpty;

      HoopsGlyphsPair glyphInfo;

      if( GetHoopsGlyph( curDash, glyphInfo ) )
      {
        if( dDashLength < 0 )
          strBufer.format(L"%f oru %s", fabs(dDashLength), L_BLANK_LINETYPE_ITEM );
        else
          strBufer.format(L"%f oru %s", dDashLength, L_DASH_LINETYPE_ITEM );

        if( strLineTypeDefine != OdString::kEmpty )
          strBufer = ", "+strBufer;

        strLineTypeDefine += strBufer;

        strBufer = OdString::kEmpty;

        double dGlyphLen = glyphInfo.dRealLength*curDash.shapeScale*dLTypeScale;

        if( OdZero(dGlyphLen) )
          dGlyphLen = 1;

        if( bShapeUSCOrient )
          strBufer.format(L_FIXED_GLYPH_LINETYPE_ITEM,dGlyphLen,glyphInfo.strHoopsGlyphName.c_str() );
        else
          strBufer.format(L_GLYPH_LINETYPE_ITEM,dGlyphLen,glyphInfo.strHoopsGlyphName.c_str() );
      }
      else
        strBufer.format(L"%f oru %s", fabs(dDashLength), L_BLANK_LINETYPE_ITEM );

      if( strLineTypeDefine != OdString::kEmpty )
        strBufer = ", "+strBufer;

      strLineTypeDefine += strBufer;
    }
    else
    {
      OdString strBufer = OdString::kEmpty;

      if( dDashLength < 0 )
        strBufer.format(L"%f oru %s", fabs(dDashLength), L_BLANK_LINETYPE_ITEM );
      else if( dDashLength == 0 )
        strBufer = L_POINT_LINETYPE_ITEM;
      else
        strBufer.format(L"%f oru %s", dDashLength, L_DASH_LINETYPE_ITEM );

      if( strLineTypeDefine != OdString::kEmpty && strBufer != OdString::kEmpty )
        strBufer = ", "+strBufer;

      strLineTypeDefine += strBufer;
    }
  }

  if( !bContinuousLineType )
  {
    // Generate hoops line style name

    OdString strHoopsLSName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsLType );

    // Export line style

    char* pchHoopsLSNameStr = OdStringToPChar( strHoopsLSName );
    char* pchHoopsLSDefStr  = OdStringToPChar( strLineTypeDefine );

    HC_Open_Segment_By_Key(m_pHoops->m_iRealStartKey);
    HC_Define_Line_Style( pchHoopsLSNameStr, pchHoopsLSDefStr );
    HC_Close_Segment();

    strRet = strHoopsLSName;

    delete[] pchHoopsLSNameStr;
    delete[] pchHoopsLSDefStr;
  }
  else
    strRet = OdString::kEmpty;

  return strRet;
}

//===================================================================\\

bool OdHoopsExportDevice::DefineHoopsGlyph( 
                      OdGiTextStyle curGlyphInfo,
                      OdGiLinetypeDash curDash,
                      OdString& strGlyphName,
                      double& dLength
                      )
{
  bool bRet = true;

  OdGePoint3dArray GlyphLinePts;

  // Get glyph geometry

  OdGePoint2d advance;

  CHoopsGlyphWriter gwHoops;
  gwHoops.setDrawContext(drawContext());
  gwHoops.bTTF = curGlyphInfo.isTtfFont();

  curGlyphInfo.setTextSize( curDash.shapeScale );

  OdString strGlyphText = OdString::kEmpty;

  if( curDash.textString != OdString::kEmpty )
    strGlyphText = curDash.textString;
  else if( curDash.shapeNumber )
    strGlyphText += OdChar( curDash.shapeNumber );

  
  OdDbBaseDatabasePEPtr pDbPE (m_pHoops->getParams().pDb);
  OdBaseTextIteratorPtr pIter = pDbPE->createTextIterator(m_pHoops->getParams().pDb, strGlyphText, strGlyphText.getLength(), false, &curGlyphInfo);
  //OdDbTextIteratorPtr pIter = OdDbTextIterator::createObject(strGlyphText, strGlyphText.getLength(), false, curGlyphInfo.getCodePage(),
  //&curGlyphInfo );
  for(;;)
  {
    OdChar ch = pIter->nextChar();
    OdCharacterProperties props = pIter->currProperties();

    OdTextProperties textFlags;
    textFlags.setVerticalText(false);
    textFlags.setTrackingPercent(1);
    textFlags.setIncludePenups(false);
    textFlags.setLastChar( false );

    if(ch && props.bValid)
    {
      OdFont* pDrawFont;

      if( props.bInBigFont && curGlyphInfo.getBigFont())
        pDrawFont = curGlyphInfo.getBigFont();
      else
        pDrawFont = curGlyphInfo.getFont();

      gwHoops.dScale = 1.0 / pDrawFont->getAbove() * curDash.shapeScale;

      pDrawFont->drawCharacter(ch, advance, &gwHoops, textFlags);

      gwHoops.dXOffset += advance.x * gwHoops.dScale;
      gwHoops.dYOffset += advance.y * gwHoops.dScale;
    }
    else if ( !ch )
      break;
    if ( props.bLastChar )
      break;
  }

  // Convert glyph geometry and define glyph

  if( gwHoops.arrGlyphGeometry.size() )
  {
    CGlyph2dGeometry GlyphGeometry;
    GlyphGeometry.setMaxSize(GLIPH_POLYLINE_MAX);

    for( OdUInt32 i = 0; i < gwHoops.arrGlyphGeometry.size(); i++ )
      GlyphGeometry.Add3dGeometryItem( gwHoops.arrGlyphGeometry[i]);

    GlyphGeometry.fixGlyphCenter();

    if( curDash.shapeRotation )
      GlyphGeometry.rotateByAngle( curDash.shapeRotation );

    double dGlyphLength = GlyphGeometry.getRegionLength();
    double dGlyphHeight = GlyphGeometry.getRegionHeight();

    OdGePoint2d tpGlyphCenter = GlyphGeometry.getGlyphCenter();

    double dScale = GlyphGeometry.ClipToRegion( GLYPH_DEFAULT_SIZE * 1.0 );

    OdUInt32 nChars = GlyphGeometry.getnCharsForGlyphDefine() + 3;

    if( dGlyphLength > dGlyphHeight )
      dLength = dGlyphLength / curDash.shapeScale;
    else
      dLength = dGlyphHeight / curDash.shapeScale;

    char* pchGlyphData = new char[ nChars];

    double dShapeOffsetX = ( -tpGlyphCenter.x - curDash.shapeOffset.x ) * dScale;
    double dShapeOffsetY = ( -tpGlyphCenter.y - curDash.shapeOffset.y ) * dScale;

    double maxLimit = GLYPH_DEFAULT_SIZE * 2.0;

    if( dShapeOffsetX > maxLimit )
    {
      while( dShapeOffsetX > maxLimit )
        dShapeOffsetX -= maxLimit;
    }

    if( dShapeOffsetX < -maxLimit )
    {
      while( dShapeOffsetX < -maxLimit )
        dShapeOffsetX += maxLimit;
    }

    if( dShapeOffsetY > maxLimit )
    {
      while( dShapeOffsetY > maxLimit )
        dShapeOffsetY -= maxLimit;
    }

    if( dShapeOffsetY < -maxLimit )
    {
      while( dShapeOffsetY < -maxLimit )
        dShapeOffsetY += maxLimit;
    }

    pchGlyphData[0] = (char)(GLYPH_DEFAULT_SIZE);               // Glyph size
    pchGlyphData[1] = (char)(dShapeOffsetX);                    // Glyph offset X
    pchGlyphData[2] = (char)(dShapeOffsetY);                    // Glyph offset Y

    OdUInt32 iStart = 3;

    for( OdInt32 iPLine = 0; iPLine < GlyphGeometry.getnItems(); iPLine++ )
    {
      CGlyph2dGeometryInfo curItem;
      GlyphGeometry.getItem( iPLine, curItem );

      if( curItem.bFillMode )
        pchGlyphData[iStart++] = (char)(-1);                      // Start Fill

      OdUInt32 iPts = curItem.pVertices.size();

      pchGlyphData[iStart++] = (char)(iPts);                      // Polyline

      for(OdUInt32 i = 0; i < iPts; i++ )
      {
        pchGlyphData[iStart++] = (char)(curItem.pVertices[i].x);  // xCoord
        pchGlyphData[iStart++] = (char)(curItem.pVertices[i].y);  // yCoord
      }

      if( curItem.bFillMode )
        pchGlyphData[iStart++] = (char)(-3);                      // End Fill
    }

    pchGlyphData[iStart++] = '\0';                                // End Glyph

    strGlyphName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsShape );

    char* pchGlyphName = OdStringToPChar( strGlyphName );

    HC_Open_Segment_By_Key(m_pHoops->m_iRealStartKey);
    HC_Define_Glyph( pchGlyphName, iStart,  pchGlyphData);
    HC_Close_Segment();

    delete[] pchGlyphData;
    delete[] pchGlyphName;
  }
  else
    bRet = false;

  return bRet;
}

//==========================================================================================//

OdString OdHoopsExportDevice::getHoopsLineType( OdDbStub* pLineType, double dLTypeScale, OdDbStub* pLayer, bool& bShapes )
{
  OdString strRet = OdString::kEmpty;

  OdDbBaseDatabasePEPtr pDbPE(m_pHoops->getParams().pDb);

  OdGiLinetype curLineType;

  bool bUseLS = pDbPE->getLineTypeById( m_pHoops->getParams().pDb, pLineType, curLineType );

  if( bUseLS && curLineType.isByLayer() )
  {
    OdRxObjectPtr pLay(userGiContext()->openDrawable(pLayer));

    if( !pLay.isNull() )
    {
      OdDbBaseLayerPEPtr lpe(pLay);

      bUseLS = lpe->getLineType( pLay, curLineType );
    }
  }

  bool bAddLineType = true;

  if( pLineType != 0 )
  {
    for( OdUInt32 i = 0; i < m_pHoops->arrLineTypePairs.size(); i++ )
    {
      if( pLineType == m_pHoops->arrLineTypePairs[i].idDwgLineType &&
          fabs(m_pHoops->arrLineTypePairs[i].dLength - curLineType.patternLength() ) < 1e-8 &&
          fabs(m_pHoops->arrLineTypePairs[i].dScale - dLTypeScale) < 1e-8 )
      {
        strRet = m_pHoops->arrLineTypePairs[i].strHoopsLineType;
        bShapes = m_pHoops->arrLineTypePairs[i].bShapes;
        bAddLineType = false;
        break;
      }
    }
  }
  else
  {
    bAddLineType = false;
    strRet = OdString::kEmpty;
  }

  if( bAddLineType )
  {
    HoopsLineTypePair tmpPair;
    tmpPair.idDwgLineType    = pLineType;
    tmpPair.dScale           = dLTypeScale;
    tmpPair.dLength          = curLineType.patternLength();
    tmpPair.strHoopsLineType = DefineHoopsLineType( curLineType, dLTypeScale, tmpPair.bShapes );

    if( tmpPair.strHoopsLineType != OdString::kEmpty )
      m_pHoops->arrLineTypePairs.push_back(tmpPair);

    strRet = tmpPair.strHoopsLineType;
    bShapes = tmpPair.bShapes;
  }

  return strRet;
}

//===================================================================\\

void OdHoopsExportDevice::AddGlyph( OdGiLinetypeDash curDash )
{
  bool bAddGlyph = true;

  if( curDash.styleId != 0 )
  {
    for( OdUInt32 i = 0; i < m_pHoops->arrGlyphsPairs.size(); i++ )
    {
      if( curDash.styleId == m_pHoops->arrGlyphsPairs[i].idGlyphDwg &&
          fabs(m_pHoops->arrGlyphsPairs[i].dYOffset - curDash.shapeOffset.y) < 1e-8 &&
          fabs(m_pHoops->arrGlyphsPairs[i].dXOffset - curDash.shapeOffset.x) < 1e-8 &&
          curDash.textString == m_pHoops->arrGlyphsPairs[i].strText &&
          fabs(m_pHoops->arrGlyphsPairs[i].dAngle - curDash.shapeRotation) < 1e-8 &&
          curDash.shapeNumber == m_pHoops->arrGlyphsPairs[i].iNumber &&
          fabs(m_pHoops->arrGlyphsPairs[i].dScale  - curDash.shapeScale) < 1e-8 &&
          fabs(m_pHoops->arrGlyphsPairs[i].dLength - curDash.length) < 1e-8
        )
      {
        bAddGlyph = false;
        break;
      }
    }
  }
  else
    bAddGlyph = false;

  if( bAddGlyph )
  {
    HoopsGlyphsPair tmpPair;
    tmpPair.idGlyphDwg   = curDash.styleId;
    tmpPair.dXOffset     = curDash.shapeOffset.x;
    tmpPair.dYOffset     = curDash.shapeOffset.y;
    tmpPair.dAngle       = curDash.shapeRotation;
    tmpPair.iNumber      = curDash.shapeNumber;
    tmpPair.strText      = curDash.textString;
    tmpPair.dLength      = curDash.length;
    tmpPair.dScale       = curDash.shapeScale;

    OdDbBaseDatabasePEPtr pDbPE(m_pHoops->getParams().pDb);

    OdGiTextStyle curGlyphInfo;

    bool bGetInfo = pDbPE->getTextStyleById( m_pHoops->getParams().pDb, curDash.styleId, curGlyphInfo );

    if( bGetInfo && DefineHoopsGlyph( curGlyphInfo, curDash, tmpPair.strHoopsGlyphName, 
                                      tmpPair.dRealLength) )
      m_pHoops->arrGlyphsPairs.push_back(tmpPair);
  }
}

//===================================================================\\

bool OdHoopsExportDevice::GetHoopsGlyph( OdGiLinetypeDash curDash, HoopsGlyphsPair& glyphInfo )
{
  bool bRet = false;

  if( curDash.styleId != 0 )
  {
    for( OdUInt32 i = 0; i < m_pHoops->arrGlyphsPairs.size(); i++ )
    {
      if( curDash.styleId == m_pHoops->arrGlyphsPairs[i].idGlyphDwg &&
        fabs(m_pHoops->arrGlyphsPairs[i].dYOffset - curDash.shapeOffset.y) < 1e-8 &&
        fabs(m_pHoops->arrGlyphsPairs[i].dXOffset - curDash.shapeOffset.x) < 1e-8 &&
        curDash.textString == m_pHoops->arrGlyphsPairs[i].strText &&
        fabs(m_pHoops->arrGlyphsPairs[i].dAngle - curDash.shapeRotation) < 1e-8 &&
        curDash.shapeNumber == m_pHoops->arrGlyphsPairs[i].iNumber &&
        fabs(m_pHoops->arrGlyphsPairs[i].dScale  - curDash.shapeScale) < 1e-8 &&
        fabs(m_pHoops->arrGlyphsPairs[i].dLength - curDash.length) < 1e-8
        )
      {
        bRet = true;
        glyphInfo = m_pHoops->arrGlyphsPairs[i];
        break;
      }
    }
  }

  return bRet;
}

//===================================================================\\

void OdHoopsExportDevice::dc_polyline(OdUInt32 nPts, const OdGePoint3d* pPoints)
{
  if( !m_bRecording || m_bSkipLines )
    return;

  HoopsPoint* pPointArr = new HoopsPoint[nPts];

  for( OdUInt32 i = 0; i < nPts; i++ )
  {
    pPointArr[i].x = (float)pPoints[i].x;
    pPointArr[i].y = (float)pPoints[i].y;
    pPointArr[i].z = (float)pPoints[i].z;
  }

  if( !m_bExportGeometryOnly )
  {
    OdString strLineSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsLine );

    OdGiSubEntityTraitsData curTraits = m_curTraits;

    bool bShapes = false;
    OdString strHoopsLineType = getHoopsLineType( curTraits.lineType(), curTraits.lineTypeScale(), curTraits.layer(), bShapes );

    OpenHoopsSegment( strLineSegmentName );

    setHoopsLineColor( curTraits.trueColor() );
    setHoopsLineWeight(  curTraits.lineWeight() );
    setHoopsLineType( strHoopsLineType );

    if( !IsLayerVisible(curTraits.layer()) )
      HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);
  }

  HC_KEY iKey = HC_KInsert_Polyline( nPts, pPointArr );

  bool bInitOrientByPoints = true;

  if( m_bInitNormal )
  {
    bInitOrientByPoints = false;

    if( nPts == 2 )
    {
      OdGeVector3d vrLine = pPoints[1] - pPoints[0];

      if( OdZero(vrLine.angleTo( m_vrNormal), 1e-4) || OdZero( vrLine.angleTo(m_vrNormal) - OdaPI, 1e-4) )
        bInitOrientByPoints = true;
    }

    if( !bInitOrientByPoints )
      setHoopsOrientationByNormal( iKey, m_vrNormal );
  }
  
  if( bInitOrientByPoints )
  {
    OdGePoint3dArray ptsArr;

    for( OdUInt32 i = 0; i < nPts; i++ )
      ptsArr.push_back( pPoints[i] );

    setHoopsLineOrientation( iKey, ptsArr );
  }

  if( !m_bExportGeometryOnly )
  {
    HC_Close_Segment();
  }

  delete[] pPointArr;
}

//===================================================================\\

void OdHoopsExportDevice::dc_xline( const OdGePoint3d& p1, const OdGePoint3d& p2 )
{
  if( !m_bRecording || m_bSkipLines )
    return;

  OdString strLineSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsXLine );

  OdGiSubEntityTraitsData curTraits = m_curTraits;

  bool bShapes = false;
  OdString strHoopsLineType = getHoopsLineType( curTraits.lineType(), curTraits.lineTypeScale(), curTraits.layer(), bShapes );

  OpenHoopsSegment( strLineSegmentName );

  setHoopsLineColor( curTraits.trueColor() );
  setHoopsLineWeight(  curTraits.lineWeight() );
  setHoopsLineType( strHoopsLineType );

  if( !IsLayerVisible(curTraits.layer()) )
    HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);

  HC_KEY iKey = HC_KInsert_Infinite_Line( p1.x, p1.y, p1.z, p2.x, p2.y, p2.z );

  bool bInitOrientByPoints = true;

  HC_Close_Segment();
}

//===================================================================\\

void OdHoopsExportDevice::dc_ray( const OdGePoint3d& p1, const OdGePoint3d& p2 )
{
  if( !m_bRecording || m_bSkipLines )
    return;

  OdString strLineSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsRay );

  OdGiSubEntityTraitsData curTraits = m_curTraits;

  bool bShapes = false;
  OdString strHoopsLineType = getHoopsLineType( curTraits.lineType(), curTraits.lineTypeScale(), curTraits.layer(), bShapes );

  OpenHoopsSegment( strLineSegmentName );

  setHoopsLineColor( curTraits.trueColor() );
  setHoopsLineWeight(  curTraits.lineWeight() );
  setHoopsLineType( strHoopsLineType );

  if( !IsLayerVisible(curTraits.layer()) )
    HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);

  HC_KEY iKey = HC_KInsert_Infinite_Ray( p1.x, p1.y, p1.z, p2.x, p2.y, p2.z );

  bool bInitOrientByPoints = true;

  HC_Close_Segment();
}

//===================================================================\\

void OdHoopsExportDevice::dc_circleArc( const OdGeCircArc3d& circArc )
{
  if( !m_bRecording || m_bSkipLines )
    return;

  if( circArc.radius() < 1e-8 )
    return;

  bool bShapes = false;

  if( !m_bExportGeometryOnly )
  {
    OdString strLineSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsCircle );

    OdGiSubEntityTraitsData curTraits = m_curTraits;

    OdString strHoopsLineType = getHoopsLineType( curTraits.lineType(), curTraits.lineTypeScale(), curTraits.layer(), bShapes );

    OpenHoopsSegment( strLineSegmentName );

    setHoopsLineColor( curTraits.trueColor() );
    setHoopsLineWeight(  curTraits.lineWeight() );
    setHoopsLineType( strHoopsLineType );

    if( !IsLayerVisible(curTraits.layer()) )
      HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);
  }

  if( !bShapes )
  {
    double       dSweepAngle    = circArc.endAng() - circArc.startAng();

    if( OdZero(dSweepAngle) || OdZero(fabs( Oda2PI - dSweepAngle)) )
    {
      if( OdZero(dSweepAngle) )
        dSweepAngle = Oda2PI;
      else if( dSweepAngle < 0 )
        dSweepAngle = fabs( dSweepAngle);

      OdGeVector3d startVector = circArc.center() - circArc.startPoint();

      OdGeCircArc3d circArc1(circArc.center(), circArc.normal(), 
                           startVector.normal(), circArc.radius(), 0., dSweepAngle/2.0);
      OdGeCircArc3d circArc2(circArc.center(), circArc.normal(), startVector.normal(), 
                           circArc.radius(), dSweepAngle/2.0, dSweepAngle);

      HoopsExportCircleArc( circArc1, m_bExportGeometryOnly );
      HoopsExportCircleArc( circArc2, m_bExportGeometryOnly, true );
    }
    else
      HoopsExportCircleArc( circArc, m_bExportGeometryOnly );
  }
  else
  {
    OdGeInterval interval;
    circArc.getInterval(interval);
    OdGePoint3dArray points;
    circArc.appendSamplePoints(interval.lowerBound(), interval.upperBound(),
      deviation(kOdGiMaxDevForCircle, circArc.center()), points);
    points.first() = circArc.startPoint();
    points.last() = circArc.endPoint();
    m_vrNormal = circArc.normal();

    m_bExportGeometryOnly = true;
    dc_polyline( points.size(), points.asArrayPtr() );
    m_bExportGeometryOnly = false;
  }

  if( !m_bExportGeometryOnly )
  {
    HC_Close_Segment();
  }
}

//===================================================================\\

void OdHoopsExportDevice::dc_ellipArc( const OdGeEllipArc3d& ellipArc )
{
  if( !m_bRecording || m_bSkipLines )
    return;

  if( ellipArc.majorRadius() < 1e-8 && ellipArc.minorRadius() < 1e-8 )
    return;

  bool bShapes = false;

  if( !m_bExportGeometryOnly )
  {
    OdString strLineSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsEllipse );

    OdGiSubEntityTraitsData curTraits = m_curTraits;

    OdString strHoopsLineType = getHoopsLineType( curTraits.lineType(), curTraits.lineTypeScale(), curTraits.layer(), bShapes );

    OpenHoopsSegment( strLineSegmentName );

    setHoopsLineColor( curTraits.trueColor() );
    setHoopsLineWeight(  curTraits.lineWeight() );
    setHoopsLineType( strHoopsLineType );

    if( !IsLayerVisible(curTraits.layer()) )
      HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);
  }

  if( !bShapes )
  {
    double dSweepAngle    = ellipArc.endAng() - ellipArc.startAng();

    if( OdZero(dSweepAngle) || OdZero(fabs( Oda2PI - dSweepAngle)) )
    {
      if( OdZero(dSweepAngle) )
        dSweepAngle = Oda2PI;
      else if( dSweepAngle < 0 )
        dSweepAngle = fabs( dSweepAngle);

      OdGeEllipArc3d ellipArc1(ellipArc.center(), ellipArc.majorAxis(),ellipArc.minorAxis(), 
        ellipArc.majorRadius(), ellipArc.minorRadius(), 0., dSweepAngle/2.0);
      OdGeEllipArc3d ellipArc2(ellipArc.center(), ellipArc.majorAxis(),ellipArc.minorAxis(), 
        ellipArc.majorRadius(), ellipArc.minorRadius(), dSweepAngle/2.0, dSweepAngle);

      HoopsExportEllipArc( ellipArc1, m_bExportGeometryOnly );
      HoopsExportEllipArc( ellipArc2, m_bExportGeometryOnly, true );
    }
    else
      HoopsExportEllipArc( ellipArc, m_bExportGeometryOnly );
  }
  else
  {
    OdGeInterval interval;
    ellipArc.getInterval(interval);
    OdGePoint3dArray points;
    ellipArc.appendSamplePoints(interval.lowerBound(), interval.upperBound(),
      deviation(kOdGiMaxDevForCircle, ellipArc.center()), points);
    points.first() = ellipArc.startPoint();
    points.last() = ellipArc.endPoint();
    m_vrNormal = ellipArc.normal();

    m_bExportGeometryOnly = true;
    dc_polyline( points.size(), points.asArrayPtr() );
    m_bExportGeometryOnly = false;
  }

  if( !m_bExportGeometryOnly )
  {
    HC_Close_Segment();
  }
}

//===================================================================\\

void OdHoopsExportDevice::dc_nurbs( const OdGeNurbCurve3d& nurbCurve )
{
  if( !m_bRecording || m_bSkipLines )
    return;

  bool bShapes = false;

  if( !m_bExportGeometryOnly )
  {
    OdString strLineSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsNurbs );

    OdGiSubEntityTraitsData curTraits = m_curTraits;

    OdString strHoopsLineType = getHoopsLineType( curTraits.lineType(), curTraits.lineTypeScale(), curTraits.layer(), bShapes );

    OpenHoopsSegment( strLineSegmentName );

    setHoopsLineColor( curTraits.trueColor() );
    setHoopsLineWeight(  curTraits.lineWeight() );
    setHoopsLineType( strHoopsLineType );

    if( !IsLayerVisible(curTraits.layer()) )
      HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);
  }

  if( !bShapes )
    HoopsExportNurbs( nurbCurve );
  else
  {
    if(nurbCurve.numControlPoints())
    {
      OdGePoint3dArray points;
      nurbCurve.appendSamplePoints(nurbCurve.startParam(), nurbCurve.endParam(),
        deviation(kOdGiMaxDevForCurve, nurbCurve.controlPointAt(0)), points);

      if( points.size() )
      {
        m_bExportGeometryOnly = true;
        dc_polyline( points.size(), points.asArrayPtr() );
        m_bExportGeometryOnly = false;
      }
    }
  }

  if( !m_bExportGeometryOnly )
  {
    HC_Close_Segment();
  }
}

//===================================================================

OdString OdHoopsExportDevice::DefineHoopsFontStyle( 
  const OdGiTextStyle* pTextStyle, 
  bool bRaw,
  double& dScaleFactor
  )
{

  OdFont* pCurFont    = pTextStyle->getFont();
  OdFont* pCurBigFont = pTextStyle->getBigFont();

  OdCharArray arrCharsForFont;
  OdCharArray arrCharsForBigFont;
  OdArray< CDwgCharInfo > arrCharsData;

  if( pCurFont )
    pCurFont->getAvailableChars( arrCharsForFont );

  if( pCurBigFont )
    pCurBigFont->getAvailableChars( arrCharsForBigFont );

  OdString strAllChars = "A";

  for( OdUInt32 i = 0; i < arrCharsForFont.size(); i++ )
  {
    if( arrCharsForFont[i] )
      strAllChars = strAllChars + arrCharsForFont[i];
  }

  int iBigCharsStart = strAllChars.getLength();

  for( OdUInt32 j = 0; j < arrCharsForBigFont.size(); j++ )
  {
    if( !arrCharsForBigFont[j] )
      continue;

    OdString curBigChar = OdString::kEmpty;
    curBigChar = curBigChar + arrCharsForBigFont[j];

    if( strAllChars.findOneOf(curBigChar) == -1 )
      strAllChars = strAllChars + arrCharsForBigFont[j];
  }

  strAllChars = strAllChars + L'\0';

  OdGePoint2d advance;

  CDwgCharInfo chBase;

  CHoopsGlyphWriter gwHoops;
  gwHoops.setDrawContext(drawContext());
  gwHoops.bTTF = false;
  bool bSaveExtents = false;

  int iCharCount = 0;
  double dBigFontScale = 1;

  if( pCurBigFont )
    dBigFontScale = pCurFont->getAbove() / pCurBigFont->getAbove();

  while( strAllChars[iCharCount] != 0 )
  {
    gwHoops.reset();

    OdChar ch = strAllChars[iCharCount];

    OdTextProperties textFlags;
    textFlags.setVerticalText(false);
    textFlags.setTrackingPercent(1);
    textFlags.setIncludePenups(false);
    textFlags.setLastChar( false );

    OdFont* pDrawFont;

    bool bBigFont = false;

    if( iCharCount >= iBigCharsStart && pTextStyle->getBigFont() )
    {
      pDrawFont = pTextStyle->getBigFont();
      bBigFont  = true;
    }
    else
      pDrawFont = pTextStyle->getFont();

    CDwgCharInfo tmpRecord;

    tmpRecord.chCode = ch;
    gwHoops.dScale = 1.0;

    if( bBigFont )
      gwHoops.dScale = dBigFontScale;

    pDrawFont->drawCharacter(ch, advance, &gwHoops, textFlags);

    if( bBigFont )
      tmpRecord.chWidth  = advance.x * dBigFontScale;
    else
      tmpRecord.chWidth  = advance.x;

    tmpRecord.chAbove  = pCurFont->getAbove();
    tmpRecord.chGeometry.setMaxSize(GLIPH_POLYLINE_MAX);
    tmpRecord.bBigFont = bBigFont;

    for( OdUInt32 i = 0; i < gwHoops.arrGlyphGeometry.size(); i++ )
      tmpRecord.chGeometry.Add3dGeometryItem( gwHoops.arrGlyphGeometry[i], true);

    if( bSaveExtents )
      arrCharsData.push_back( tmpRecord );
    else
    {
      chBase = tmpRecord;
      bSaveExtents = true;
    }

    iCharCount++;
  }

  double dFontMaxX         = -1;
  double dBigFontMaxX      = -1;
  double dMaxFontY = -1e30;
  double dMaxBigFontY = -1e30;
  OdInt32 nCharsForDefinition = 0;
  nCharsForDefinition += 5;

  for( OdUInt32 iRecord = 0; iRecord < arrCharsData.size(); iRecord++ )
  {
    nCharsForDefinition += 8 + arrCharsData[iRecord].chGeometry.getnCharsForGlyphDefine();

    double dCurMaxY =  arrCharsData[iRecord].chGeometry.getMaxY();

    if( !arrCharsData[iRecord].chGeometry.getnItems())
      dCurMaxY = 0;

    if( arrCharsData[iRecord].bBigFont )
    {
      if( arrCharsData[iRecord].chWidth > dBigFontMaxX )
        dBigFontMaxX = arrCharsData[iRecord].chWidth;

      if( dCurMaxY > dMaxBigFontY )
        dMaxBigFontY = dCurMaxY;

      continue;
    }
    else
    {
      if( arrCharsData[iRecord].chWidth > dFontMaxX )
        dFontMaxX = arrCharsData[iRecord].chWidth;

      if( dCurMaxY > dMaxFontY )
        dMaxFontY = dCurMaxY;
    }
  }

  double dMaxX = dFontMaxX;
  double dCharsMaxY = dMaxFontY;

  if( dMaxX < 0 )
    dMaxX = dBigFontMaxX;

  if( dCharsMaxY < 0 )
    dCharsMaxY = dMaxBigFontY; 

  char* pchFontDefine = new char[nCharsForDefinition];
  pchFontDefine[0] = '\3';

  if( dCharsMaxY > dMaxX  )
  {
    pchFontDefine[1] = (char)(TEXT_RECT_SIZE);
    pchFontDefine[2] = (char)( dMaxX / dCharsMaxY * TEXT_RECT_SIZE );
  }
  else
  {
    pchFontDefine[1] = (char)( dCharsMaxY / dMaxX * TEXT_RECT_SIZE );
    pchFontDefine[2] = (char)(TEXT_RECT_SIZE );
  }

  pchFontDefine[3] = '\0';

  OdUInt32 iCurPos = 4;

  for( OdUInt32 iRec = 0; iRec < arrCharsData.size(); iRec++ )
  {
    double dMaxY =  arrCharsData[iRec].chGeometry.getMaxY();
    double dMinY =  arrCharsData[iRec].chGeometry.getMinY();

    if( !arrCharsData[iRec].chGeometry.getnItems())
    {
      dMaxY = 0;
      dMinY = 0;
    }

    arrCharsData[iRec].chGeometry.setLength( 0, dMaxX );
    arrCharsData[iRec].chGeometry.setHeight( 0, dCharsMaxY );

    double dScale = arrCharsData[iRec].chGeometry.ClipToRegion( TEXT_RECT_SIZE, false, false);

    WORD wCode = arrCharsData[iRec].chCode;
    pchFontDefine[iCurPos++] = (char)(wCode>>8);
    pchFontDefine[iCurPos++] = (char)(wCode & 0x00FF );
    pchFontDefine[iCurPos++] = (char)(arrCharsData[iRec].chWidth * dScale);
    pchFontDefine[iCurPos++] = '\0';
    pchFontDefine[iCurPos++] = '\0';
    pchFontDefine[iCurPos++] = (char)(dMaxY * dScale);
    pchFontDefine[iCurPos++] = (char)(dMinY * dScale);

    for( OdInt32 j = 0; j < arrCharsData[iRec].chGeometry.getnItems(); j++ )
    {
      CGlyph2dGeometryInfo tmpRecord;
      arrCharsData[iRec].chGeometry.getItem(j, tmpRecord);
      pchFontDefine[iCurPos++] = (char)(tmpRecord.pVertices.size());

      for( OdUInt32 k = 0; k < tmpRecord.pVertices.size(); k++ )
      {
        pchFontDefine[iCurPos++] = (char)(tmpRecord.pVertices[k].x);
        pchFontDefine[iCurPos++] = (char)(tmpRecord.pVertices[k].y);
      }
    }
    pchFontDefine[iCurPos++] = '\0';
  }

  HoopsFontTypePair tmpPair;
  tmpPair.bRaw       = bRaw;
  tmpPair.idCodePage = pTextStyle->getCodePage();
  tmpPair.pFont      = pTextStyle->getFont();
  tmpPair.pBigFont   = pTextStyle->getBigFont();
  tmpPair.dScaleFactor = dMaxFontY/chBase.chAbove;

  OdString strName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsFontStyle, false );

  tmpPair.strHoopsFontName = strName;

  char* pchFontName = OdStringToPChar( strName );

  HC_Define_Font( pchFontName, HOOPS_FONT_OPTIONS, iCurPos, pchFontDefine );

  dScaleFactor = tmpPair.dScaleFactor;

  m_pHoops->arrFontPairs.push_back( tmpPair );

  delete[] pchFontName;
  delete[] pchFontDefine;

  return strName;
}

//===================================================================\\

OdString OdHoopsExportDevice::GetHoopsFontStyle( const OdGiTextStyle* pTextStyle, bool bRaw, double& dScaleFactor )
{
  OdString strRet = OdString::kEmpty;

  bool bDefineNewFont = true;

  if( pTextStyle )
  {
    for( OdUInt32 i = 0; i < m_pHoops->arrFontPairs.size(); i++ )
    {
      if( m_pHoops->arrFontPairs[i].bRaw == bRaw && 
        m_pHoops->arrFontPairs[i].idCodePage == pTextStyle->getCodePage() &&
        m_pHoops->arrFontPairs[i].pFont == pTextStyle->getFont() &&
        m_pHoops->arrFontPairs[i].pBigFont == pTextStyle->getBigFont()
        )
      {
        bDefineNewFont = false;
        strRet = m_pHoops->arrFontPairs[i].strHoopsFontName;
        dScaleFactor = m_pHoops->arrFontPairs[i].dScaleFactor;
        break;
      }
    }
  }

  if( bDefineNewFont )
    strRet = DefineHoopsFontStyle( pTextStyle, bRaw, dScaleFactor );

  return strRet;
}

//===================================================================\\

void OdHoopsExportDevice::dc_text( 
                                  const OdGePoint3d& position, 
                                  const OdGeVector3d& u, 
                                  const OdGeVector3d& v,
                                  const OdChar* msg, 
                                  OdInt32 length, 
                                  bool raw, 
                                  const OdGiTextStyle* pTextStyle,
                                  const OdGeVector3d* pExtrusion)
{
  if( !m_bRecording )
    return;

  OdGiSubEntityTraitsData curTraits = m_curTraits;

  OdString strTextSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsText );

  bool bExportTextAsLines = pTextStyle->isVertical();

  if( length == -1 )
    length = (OdInt32)Od_wcslen( msg );

  if( length != -1 )
  {
    OdString checkStr( msg );

    if( checkStr.getLength() != length || (checkStr.find(L"%%") != -1 ) )
      bExportTextAsLines = true;

    if( pTextStyle->isTtfFont() && ( m_pHoops->m_Params.flags & TTF_TEXT_AS_GEOMETRY ) )
      bExportTextAsLines = true;

    if( !pTextStyle->isTtfFont() && ( m_pHoops->m_Params.flags & SHX_TEXT_AS_GEOMETRY ) )
      bExportTextAsLines = true;

    if( pTextStyle->getBigFont() )
    {
      bExportTextAsLines = true;
    }
  }

  if( bExportTextAsLines )
  {
    OpenHoopsSegment( strTextSegmentName );

    setHoopsLineColor( curTraits.trueColor() );
    setHoopsLineWeight(0);
    setHoopsLineType(OdString::kEmpty);

    m_bExportGeometryOnly = true;

    OdGiGeometrySimplifier::textProc( position, u, v, msg, length, raw, pTextStyle, pExtrusion );

    m_bExportGeometryOnly = false;

    HC_Close_Segment();

    return;
  }

  OdString strFontName = OdString::kEmpty;

  bool bUseTTF = true;

  double dScaleFontSizeFactor = 1;

  if( pTextStyle->isTtfFont() )
    strFontName = pTextStyle->getFontFilePath( m_pHoops->getParams().pDb );
  else
  {
    bUseTTF     = false;
    strFontName = GetHoopsFontStyle( pTextStyle, raw, dScaleFontSizeFactor );
  }

  OpenHoopsSegment( strTextSegmentName );

  if( strFontName != OdString::kEmpty )
    setHoopsFontName( strFontName, bUseTTF );

  double dAngle = OdaPI2 - u.angleTo( v );
  setHoopsTextColor( curTraits.trueColor() );
  setHoopsDefaultAlignment();
  setHoopsObliquingAngle( dAngle );
  setHoopsFontWidthFactor( pTextStyle->xScale() );
  setText3dMode( true );

  bool bBold   = false;
  bool bItalic = false;

  if( pTextStyle->isTtfFont() )
  {
    bBold   = pTextStyle->ttfdescriptor().isBold();
    bItalic = pTextStyle->ttfdescriptor().isItalic();
  }

  setHoopsFontBold( bBold );
  setHoopsFontItalic( bItalic );
  setHoopsFontUnderlined(pTextStyle->isUnderlined());
  setHoopsFontOverlined(pTextStyle->isOverlined());
  setHoopsFontWidthFactor( u.length() / v.length() );

  OdChar* textMsg = NULL;
  textMsg = new OdChar[ length+1 ];

  if( !bUseTTF && pTextStyle->getBigFont() )
  {
    OdDbBaseDatabasePEPtr pDbPE(m_pHoops->getParams().pDb);
    OdBaseTextIteratorPtr pIter = pDbPE->createTextIterator(m_pHoops->getParams().pDb, msg, length, false, pTextStyle);
    //OdDbTextIteratorPtr pIter = OdDbTextIterator::createObject(msg, length, false, pTextStyle->getCodePage(), pTextStyle);

     int iCharCount = 0;

     for(;;)
     {
       OdChar ch = pIter->nextChar();

       textMsg[iCharCount] = ch;

       iCharCount++;

       if( !ch || iCharCount > length )
       {
         if( ch != 0 )
           textMsg[iCharCount-1] = 0;

         break;
       }
     }
  }
  else
  {
    if( length > 0 )
      memcpy(textMsg,msg,length*sizeof(OdChar));

    textMsg[length]=0;
  }

  OdInt64 lTextKey = HC_KInsert_Text_With_Encoding( position.x, position.y,position.z, HOOPS_WCS_TEXT_MODE, (char const*)textMsg);

  delete[] textMsg;

  OdGePlane pl( position, u, v );
  OdGeVector3d norm = pl.normal();
  OdGeVector3d vPerp = v;
  vPerp.rotateBy( dAngle, norm);

  if( u.angleTo(vPerp) != OdaPI2 )
    vPerp.rotateBy( -2*dAngle, norm);

  vPerp = vPerp * cos(dAngle);

  setHoopsTextExtents( lTextKey, pTextStyle, vPerp.length(), msg, length, position, u, vPerp, dScaleFontSizeFactor, dAngle );

  if( !IsLayerVisible(curTraits.layer()) )
    HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);

  HC_Close_Segment();
}

//===================================================================\\

void   OdHoopsExportDevice::setHoopsTextExtents( 
  OdInt64 TextKey, 
  const OdGiTextStyle* pTextStyle, 
  double dBaseSize, 
  const OdChar* msg, 
  OdInt32 length,
  OdGePoint3d ptInsert,
  OdGeVector3d u,
  OdGeVector3d v,
  double dScaleFactor,
  double dAngle
  )
{
  OdGePoint3d minPt, maxPt;

  OdGiTextStyle tmpTextStyle( *pTextStyle );
  tmpTextStyle.setTextSize(dBaseSize);
  tmpTextStyle.setVertical( false );
  tmpTextStyle.setXScale( u.length() / v.length() );
  tmpTextStyle.setObliquingAngle(0);
  drawContext()->giContext().textExtentsBox( tmpTextStyle, msg, length, 0, minPt, maxPt );

  double dHoopsSize = dBaseSize;

  if( pTextStyle->isTtfFont() )
    dHoopsSize *= getTtfScale( pTextStyle->getFont() );
  else
    dHoopsSize *= dScaleFactor;

  double dHeight = dHoopsSize;

  if( !OdZero(dAngle) )
  {
    double dX = tan( dAngle ) * dHeight;

    if( dX > 0 )
      maxPt.x += dX;
    else
    {
      ptInsert.x += u.x / u.length() * dX;
      ptInsert.y += u.y / u.length() * dX;
      ptInsert.z += u.z / u.length() * dX;
    }
  }

  double dLength = maxPt.x - minPt.x;

  if( dLength == 0 )
    dLength = 0.001;

  HC_Open_Geometry( TextKey );
  HoopsPoint ptsRgn[3];
  ptsRgn[0].x = (float) ptInsert.x;
  ptsRgn[0].y = (float) ptInsert.y;
  ptsRgn[0].z = (float) ptInsert.z;

  ptsRgn[1].x = (float) ( ptInsert.x + u.x / u.length() * dLength);
  ptsRgn[1].y = (float) ( ptInsert.y + u.y / u.length() * dLength);
  ptsRgn[1].z = (float) ( ptInsert.z + u.z / u.length() * dLength);

  ptsRgn[2].x = (float) ( ptInsert.x + v.x / u.length() * dHeight);
  ptsRgn[2].y = (float) ( ptInsert.y + v.y / u.length() * dHeight);
  ptsRgn[2].z = (float) ( ptInsert.z + v.z / u.length() * dHeight);

  HC_Set_Text_Region(3,ptsRgn,OdString::kEmpty);
  HC_Close_Geometry();
  setHoopsTextSize( dHoopsSize );
}

//===================================================================\\

void OdHoopsExportDevice::dc_polygon( 
  OdInt32 numPoints, 
  const OdGePoint3d* vertexList, 
  const OdGeVector3d* pNormal )
{
  if( !m_bRecording )
    return;

  OdGiSubEntityTraitsData curTraits = m_curTraits;

  if( !fillMode() )
  {
    if( pNormal )
    {
      m_vrNormal = *pNormal;
      m_bInitNormal = true;
    }
    else
      m_bInitNormal = false;

    if( OdZero(vertexList[0].distanceTo(vertexList[numPoints - 1])) )
      dc_polyline( numPoints, vertexList );
    else
    {
      OdGePoint3d* ptsLine = new OdGePoint3d[ numPoints + 1 ];

      for( OdInt32 i = 0; i < numPoints; i++ )
        ptsLine[i] = vertexList[i];

      ptsLine[numPoints] = ptsLine[0];

      dc_polyline( numPoints + 1, ptsLine );

      delete[] ptsLine;
    }

    return;
  }

  if( numPoints < 4 )
  {
    if( m_bTriangleOut &&
       (OdZero(vertexList[0].distanceTo(vertexList[1])) ||
        OdZero(vertexList[0].distanceTo(vertexList[2])) ||
        OdZero(vertexList[1].distanceTo(vertexList[2]))
       )
      )
        return;
  }

  OdGiMapperItemEntryPtr pCurMapper;

  if( m_MaterialInfo.bSetMaterial && m_MaterialInfo.strTextureFilename != OdString::kEmpty )
  {
    switch( m_MaterialInfo.iTextureType )
    {
    case DIFFUSE_TEXTURE :
      pCurMapper = ((OdGiMapperRenderItemPtr)m_pCurMapperItem)->diffuseMapper(); break;
    case SPECULAR_TEXTURE :
      pCurMapper = ((OdGiMapperRenderItemPtr)m_pCurMapperItem)->specularMapper(); break;
    case BUMP_TEXTURE :
      pCurMapper = ((OdGiMapperRenderItemPtr)m_pCurMapperItem)->bumpMapper(); break;
    case REFLECTION_TEXTURE :
      pCurMapper = ((OdGiMapperRenderItemPtr)m_pCurMapperItem)->reflectionMapper(); break;
    }
  }

  Hoops2dPoint* ptsHoopsTexture = NULL;

  if( !pCurMapper.isNull() && numPoints > 2)
  {
    ptsHoopsTexture   = new Hoops2dPoint[ numPoints ];

    double      ptsInUse[9];
    double      ptsTexture[6];
    OdInt32     iIndexesInUse[3];

    OdInt32     iIndexForUpdate = 0;

    for( OdInt32 i = 0; i < numPoints; i++ )
    {
      ptsInUse[3*iIndexForUpdate] = vertexList[i].x;
      ptsInUse[3*iIndexForUpdate + 1] = vertexList[i].y;
      ptsInUse[3*iIndexForUpdate + 2] = vertexList[i].z;
      iIndexesInUse[iIndexForUpdate] = i;

      iIndexForUpdate++;

      if( iIndexForUpdate == 3 )
      {
        OdGeVector3d  vrNormal;
        OdGeVector3d* pCurNormal = NULL;

        if( pNormal )
        {
          vrNormal = *pNormal;
          pCurNormal = &vrNormal;
        }
        else if( m_bTriangleOut && m_pShellVertexData && m_pShellVertexData->normals() )
        {
          vrNormal = m_pShellVertexData->normals()[m_iTriangleIndexes[1]];
          pCurNormal = &vrNormal;
        }

        if( pCurNormal )
          pCurMapper->mapCoords( ptsInUse, (double*)pCurNormal, ptsTexture );
        else
          pCurMapper->mapCoords( ptsInUse, ptsTexture );

        for( OdInt32 k = 0; k < 3; k++ )
        {
          ptsHoopsTexture[iIndexesInUse[k]].x = (float)(ptsTexture[2*k]);
          ptsHoopsTexture[iIndexesInUse[k]].y = (float)(ptsTexture[2*k+1]);
        }

        iIndexForUpdate = 0;
      }
    }

    if( iIndexForUpdate != 0 )
    {
      OdGeVector3d  vrNormal;
      OdGeVector3d* pCurNormal = NULL;

     
      if( pNormal )
      {
        vrNormal = *pNormal;
        pCurNormal = &vrNormal;
      }
      else if( m_bTriangleOut && m_pShellVertexData && m_pShellVertexData->normals() )
      {
        vrNormal = m_pShellVertexData->normals()[m_iTriangleIndexes[1]];
        pCurNormal = &vrNormal;
      }

      if( pCurNormal)
        pCurMapper->mapCoords( ptsInUse, (double*)pCurNormal, ptsTexture );
      else
        pCurMapper->mapCoords( ptsInUse, ptsTexture );

      for( OdInt32 k = 0; k < 3; k++ )
      {
        ptsHoopsTexture[iIndexesInUse[k]].x = (float)(ptsTexture[2*k]);
        ptsHoopsTexture[iIndexesInUse[k]].y = (float)(ptsTexture[2*k+1]);
      }
    }
  }

  HoopsPoint* pPointArr = new HoopsPoint[numPoints];

  for( OdInt32 i = 0; i < numPoints; i++ )
  {
    pPointArr[i].x = (float)vertexList[i].x;
    pPointArr[i].y = (float)vertexList[i].y;
    pPointArr[i].z = (float)vertexList[i].z;
  }

  if( !m_bExportGeometryOnly )
  {
    OdString strPolygonSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsPolygon );

    OpenHoopsSegment( strPolygonSegmentName );
    setHoopsMaterial( curTraits, m_pHoops->getParams().iExportFileType != DWG_FILE_TYPE );
  }

  HC_KEY iKey = HoopsExportPolygon( numPoints, pPointArr );

  if( !m_bExportGeometryOnly )
  {
    if( !IsLayerVisible(curTraits.layer()) )
      HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);
  }

  if( !pCurMapper.isNull() )
  {
    HC_Open_Geometry( iKey );

    for( OdInt32 i = 0; i < numPoints; i++ )
    {
      HC_Open_Vertex(i);
      HC_Set_Parameter(2, (float*)(ptsHoopsTexture + i) );

      if( m_bTriangleOut && m_pShellVertexData && i < 3 )
      {
        if( m_pShellVertexData->normals() )
          setHoopsVertexNormal( m_pShellVertexData->normals()[m_iTriangleIndexes[i]] );

        if( m_pShellVertexData->trueColors() )
          setHoopsVertexColor( m_pShellVertexData->trueColors()[m_iTriangleIndexes[i]] );
      }

      HC_Close_Vertex();
    }

    HC_Close_Geometry();
  }

  if( !m_bExportGeometryOnly )  
    HC_Close_Segment();

  delete[] pPointArr;

  if( ptsHoopsTexture )
    delete[] ptsHoopsTexture;
}

//===================================================================\\

void OdHoopsExportDevice::dc_image( 
  const OdGePoint3d& origin, 
  const OdGeVector3d& u, 
  const OdGeVector3d& v,
  const OdGiRasterImage* pImage, 
  const OdGePoint2d* uvBoundary, 
  OdUInt32 numBoundPts,
  bool transparency,
  double brightness, 
  double contrast, 
  double fade )
{
  if( !m_bRecording)
    return;

  HoopsImagePair imageInfo;
  imageInfo.pImage        = pImage;
  imageInfo.dBrightness   = brightness;
  imageInfo.dContrast     = contrast;
  imageInfo.dFade         = fade;
  imageInfo.bTransparency = transparency;

  if( getHoopsImage( imageInfo ) )
  {
    OdGePoint2dArray ptsTextureArr;
    OdGePoint3dArray ptsShellArr;

    if( numBoundPts < 4)
    {
      OdGePoint2d ptTexCoord;

      ptTexCoord.x = imageInfo.iImageWidth;
      ptTexCoord.y = imageInfo.iImageHeight;
      ptsTextureArr.push_back( ptTexCoord );
      ptTexCoord.x = 0;
      ptTexCoord.y = imageInfo.iImageHeight;
      ptsTextureArr.push_back( ptTexCoord );
      ptTexCoord.x = 0;
      ptTexCoord.y = 0;
      ptsTextureArr.push_back( ptTexCoord );
      ptTexCoord.x = imageInfo.iImageWidth;
      ptTexCoord.y = 0;
      ptsTextureArr.push_back( ptTexCoord );

      OdGePoint3d ptShellCoord;

      ptShellCoord = origin;
      ptsShellArr.push_back( ptShellCoord );
      ptShellCoord = origin + u * imageInfo.iImageWidth;
      ptsShellArr.push_back( ptShellCoord );
      ptShellCoord = origin + u * imageInfo.iImageWidth + 
                              v * imageInfo.iImageHeight;
      ptsShellArr.push_back( ptShellCoord );
      ptShellCoord = origin + v * imageInfo.iImageHeight;
      ptsShellArr.push_back( ptShellCoord );
    }
    else
    {
      OdGePlane imagePlane( origin, u, v );
      OdGeMatrix3d TransMat;
      TransMat.setToPlaneToWorld( imagePlane );

      OdUInt32 nPts = numBoundPts;

      if( OdZero(uvBoundary[0].distanceTo(uvBoundary[numBoundPts-1])) )
        nPts--;

      for( OdUInt32 i = 0; i < nPts; i++ )
      {
        ptsTextureArr.push_back( uvBoundary[i] );

        OdGePoint3d ptShellCoord;
        ptShellCoord.x = uvBoundary[i].x;
        ptShellCoord.y = imageInfo.iImageHeight-uvBoundary[i].y;
        ptShellCoord.z = 0;
        ptShellCoord.transformBy( TransMat );
        ptsShellArr.push_back( ptShellCoord );
      }
    }

    OdString strImageShellName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsImage );
    OdString strTextureName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsTexture );

    CreateRasterImage(strImageShellName, imageInfo.strHoopsImageName, strTextureName, 
                      imageInfo.iImageWidth, imageInfo.iImageHeight,
                      ptsShellArr, ptsTextureArr );
  }
}

//===================================================================\\

bool  OdHoopsExportDevice::getHoopsImage( HoopsImagePair& imageInfo )
{
  bool bFindImage = false;

  for( OdUInt32 i = 0; i < m_pHoops->arrImagePairs.size(); i++ )
  {
    if( m_pHoops->arrImagePairs[i].pImage == imageInfo.pImage &&
        m_pHoops->arrImagePairs[i].bTransparency == imageInfo.bTransparency &&
        m_pHoops->arrImagePairs[i].dBrightness == imageInfo.dBrightness &&
        m_pHoops->arrImagePairs[i].dContrast == imageInfo.dContrast &&
        m_pHoops->arrImagePairs[i].dFade == imageInfo.dFade
      )
    {
      imageInfo.strHoopsImageName = m_pHoops->arrImagePairs[i].strHoopsImageName;
      imageInfo.iBitPerPixel      = m_pHoops->arrImagePairs[i].iBitPerPixel;
      imageInfo.iImageHeight      = m_pHoops->arrImagePairs[i].iImageHeight;
      imageInfo.iImageWidth       = m_pHoops->arrImagePairs[i].iImageWidth;
      bFindImage = true;
      break;
    }
  }

  if( !bFindImage )
  {
    HoopsImagePair tmpPair;
    tmpPair.pImage = imageInfo.pImage;
    tmpPair.bTransparency = imageInfo.bTransparency;
    tmpPair.dBrightness = imageInfo.dBrightness;
    tmpPair.dContrast = imageInfo.dContrast;
    tmpPair.dFade = imageInfo.dFade;

    bFindImage = DefineHoopsImage( tmpPair );

    if( bFindImage )
    {
      m_pHoops->arrImagePairs.push_back( tmpPair );

      imageInfo.iImageWidth  = tmpPair.iImageWidth;
      imageInfo.iImageHeight = tmpPair.iImageHeight;
      imageInfo.iBitPerPixel = tmpPair.iBitPerPixel;
      imageInfo.strHoopsImageName = tmpPair.strHoopsImageName;
    }
  }

  return bFindImage;
}

//===================================================================\\

bool OdHoopsExportDevice::DefineHoopsImage( HoopsImagePair& newImagePair )
{
  if( newImagePair.pImage == NULL )
    return false;

  OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
  pDesc->setPixelWidth(newImagePair.pImage->pixelWidth());
  pDesc->setPixelHeight(newImagePair.pImage->pixelHeight());
  pDesc->setColorDepth(32);

  if( newImagePair.pImage->scanLinesAlignment() == 4 )
  {
    pDesc->setScanLinesAlignment(4);
  }

  pDesc->pixelFormat().setBGRA();

  OdGiRasterImagePtr pImg;

  if( newImagePair.pImage->numColors() == 2 )
  {
    OdSmartPtr<OdGiBitonalRasterTransformer> pBitTrans;

    OdGiSubEntityTraitsData curTraits = m_curTraits;

    pBitTrans = OdRxObjectImpl<OdGiBitonalRasterTransformer>::createObject();

    OdCmEntityColor fgTrueColor = curTraits.trueColor();

    OdUInt8 uRed;
    OdUInt8 uGreen;
    OdUInt8 uBlue;

    if( fgTrueColor.colorMethod() == OdCmEntityColor::kByACI || 
        fgTrueColor.colorMethod() == OdCmEntityColor::kByDgnIndex )
    {
      if( m_logPalette.size() > (OdUInt16)fgTrueColor.colorIndex() ) 
      {
        ODCOLORREF trueColor = getColor( fgTrueColor.colorIndex() );

        uRed   = ODGETRED(trueColor);
        uGreen = ODGETGREEN(trueColor);
        uBlue  = ODGETBLUE(trueColor);
      }
      else
      {
        uRed   = OdCmEntityColor::mLUT[ fgTrueColor.colorIndex() ][0];
        uGreen = OdCmEntityColor::mLUT[ fgTrueColor.colorIndex() ][1];
        uBlue  = OdCmEntityColor::mLUT[ fgTrueColor.colorIndex() ][2];
      }
    }
    else
    {
      uRed = fgTrueColor.red();
      uGreen = fgTrueColor.green();
      uBlue = fgTrueColor.blue();
    }

    ODCOLORREF fgColor = ODRGBA( uRed, uGreen, uBlue, 0 );

    pBitTrans->setOriginal( newImagePair.pImage, fgColor, ODRGBA(0,0,0,255), newImagePair.bTransparency );

    pImg = pBitTrans->convert( true, newImagePair.dBrightness, newImagePair.dContrast,
                               newImagePair.dFade, 0, false, false,false, pDesc, newImagePair.bTransparency );
  }
  else
  {
    pImg = newImagePair.pImage->convert( true, newImagePair.dBrightness, newImagePair.dContrast,
                                         newImagePair.dFade, 0, false, false,false, pDesc, newImagePair.bTransparency );
  }

  if( pImg.isNull() )
    return false;

  OdString strImageName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsImageDef, false );

  if( InsertHoopsImage(pImg, strImageName ) )
  {
    newImagePair.strHoopsImageName = strImageName;
    newImagePair.iImageWidth  = pImg->pixelWidth();
    newImagePair.iImageHeight = pImg->pixelHeight();
    newImagePair.iBitPerPixel = pImg->colorDepth();
    return true;
  }

  return false;
}

//===================================================================\\

OdString OdHoopsExportDevice::defineHoopsObjectName( hoopsObjectType iObjType, bool bSkipCheck )
{
  OdString  strFormat = OdString::kEmpty;
  OdUInt32* iCount = NULL;
  HC_KEY    iBaseSegmentKey = m_pHoops->m_iCurKey;

  switch( iObjType )
  {
    case OdHoopsExportDevice::kHoopsLine :
    {
      strFormat = L_PLINE_NAME_FORMAT;
      iCount = &m_pHoops->m_iLineCount;
    }; break;

    case OdHoopsExportDevice::kHoopsXLine :
    {
      strFormat = L_XLINE_NAME_FORMAT;
      iCount = &m_pHoops->m_iXLineCount;
    }; break;

    case OdHoopsExportDevice::kHoopsRay :
    {
      strFormat = L_RAY_NAME_FORMAT;
      iCount = &m_pHoops->m_iRayCount;
    }; break;

    case OdHoopsExportDevice::kHoopsShape :
    {
      strFormat = L_GLYPH_NAME_FORMAT;
      iCount = &m_pHoops->m_iShapeCount;
    }; break;

    case OdHoopsExportDevice::kHoopsLType :
    {
      strFormat = L_LTYPE_NAME_FORMAT;
      iCount = &m_pHoops->m_iLTypeCount;
    }; break;

    case OdHoopsExportDevice::kHoopsCircle :
    {
      strFormat = L_CIRCLE_NAME_FORMAT;
      iCount = &m_pHoops->m_iCircleCount;
    }; break;

    case OdHoopsExportDevice::kHoopsEllipse :
    {
      strFormat = L_ELLIPSE_NAME_FORMAT;
      iCount = &m_pHoops->m_iEllipseCount;
    }; break;

    case OdHoopsExportDevice::kHoopsNurbs :
    {
      strFormat = L_NURBS_NAME_FORMAT;
      iCount = &m_pHoops->m_iNurbsCount;
    }; break;

    case OdHoopsExportDevice::kHoopsText :
    {
      strFormat = L_TEXT_NAME_FORMAT;
      iCount = &m_pHoops->m_iTextCount;
    }; break;

    case OdHoopsExportDevice::kHoopsFontStyle :
    {
      strFormat = L_FONT_NAME_FORMAT;
      iCount = &m_pHoops->m_iFontStyleCount;
    }; break;

    case OdHoopsExportDevice::kHoopsPolygon :
    {
      strFormat = L_POLYGON_NAME_FORMAT;
      iCount = &m_pHoops->m_iPolygonCount;
    }; break;

    case OdHoopsExportDevice::kHoopsImageDef :
    {
      strFormat = L_IMAGE_NAME_FORMAT;
      iCount = &m_pHoops->m_iImageCount;
      iBaseSegmentKey = HC_KOpen_Segment(HOOPS_IMAGE_SEGMENT);
      HC_Close_Segment();
    }; break;

    case OdHoopsExportDevice::kHoopsImage :
    {
      strFormat = L_IMAGESHELL_NAME_FORMAT;
      iCount = &m_pHoops->m_iImageShellCount;
    }; break;

    case OdHoopsExportDevice::kHoopsTexture :
    {
      strFormat = L_TEXTURE_NAME_FORMAT;
      iCount = &m_pHoops->m_iTextureCount;
    }; break;

    case OdHoopsExportDevice::kHoopsMesh :
    {
      strFormat = L_MESH_NAME_FORMAT;
      iCount = &m_pHoops->m_iMeshCount;
    }; break;

    case OdHoopsExportDevice::kHoopsShell :
    {
      strFormat = L_SHELL_NAME_FORMAT;
      iCount = &m_pHoops->m_iShellCount;
    }; break;

    case OdHoopsExportDevice::kHoolsLight :
    {
      strFormat = L_LS_NAME_FORMAT;
      iCount = &m_pHoops->m_iLSCount;
    }; break;

    case OdHoopsExportDevice::kHoopsViewport :
    {
      strFormat = L_VIEWPORT_NAME_FORMAT;
      iCount = &m_pHoops->m_iViewportCount;
    }; break;

    default:
    {
      strFormat = L_OBJECT_NAME_FORMAT;
      iCount = &m_pHoops->m_iObjectCount;
    }; break;
  };

  OdString strName;

  bool bGoodName = false;

  while( !bGoodName )
  {
    strName.format( strFormat.c_str(), *iCount );
    (*iCount)++;

    if( bSkipCheck )
      bGoodName = true;
    else
    {
      switch( iObjType )
      {
        case OdHoopsExportDevice::kHoopsLine :
        case OdHoopsExportDevice::kHoopsXLine :
        case OdHoopsExportDevice::kHoopsRay :
        case OdHoopsExportDevice::kHoopsCircle :
        case OdHoopsExportDevice::kHoopsEllipse :
        case OdHoopsExportDevice::kHoopsNurbs :
        case OdHoopsExportDevice::kHoopsText :
        case OdHoopsExportDevice::kHoopsPolygon :
        case OdHoopsExportDevice::kHoopsImage :
        case OdHoopsExportDevice::kHoopsMesh :
        case OdHoopsExportDevice::kHoopsShell :
        case OdHoopsExportDevice::kHoolsLight :
        case OdHoopsExportDevice::kHoopsViewport :
        {
          bGoodName = !isHoopsSegmentExists( iBaseSegmentKey, strName );
        } break;

        case OdHoopsExportDevice::kHoopsShape :
        {
          bGoodName  = !isHoopsGlyphExists( iBaseSegmentKey, strName );
        } break;

        case OdHoopsExportDevice::kHoopsLType :
        {
          bGoodName = !isHoopsLTypeExists( iBaseSegmentKey, strName );
        } break;
       
        case OdHoopsExportDevice::kHoopsFontStyle :
        {
          bGoodName = !isHoopsFontExists( iBaseSegmentKey, strName );
        } break;

        case OdHoopsExportDevice::kHoopsImageDef :
        case OdHoopsExportDevice::kHoopsTexture :
        {
          bGoodName = !isHoopsTextureExists( iBaseSegmentKey, strName );
        } break;
      }
    }
  }

  return strName;
}

//===================================================================\\

void OdHoopsExportDevice::dc_shell( 
  OdInt32 numVertices, 
  const OdGePoint3d* vertexList, 
  OdInt32 faceListSize,
  const OdInt32* faceList, 
  const OdGiEdgeData* pEdgeData, 
  const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData )
{
  if( !m_bRecording )
    return;

  OdInt32 *newFaceList = new OdInt32 [ faceListSize ];

  memcpy( newFaceList, faceList, faceListSize * sizeof(OdInt32) );

  bool bTexture = false;

  if( m_MaterialInfo.bSetMaterial && m_MaterialInfo.strTextureFilename != OdString::kEmpty )
    bTexture = true;
  else
    UpdateShellFaceList( newFaceList, faceListSize, vertexList, numVertices );

  HoopsPoint* pPointArr = new HoopsPoint[numVertices];

  for( OdInt32 i = 0; i < numVertices; i++ )
  {
    pPointArr[i].x = (float)vertexList[i].x;
    pPointArr[i].y = (float)vertexList[i].y;
    pPointArr[i].z = (float)vertexList[i].z;
  }

  OdGiSubEntityTraitsData curTraits = m_curTraits;

  OdString strShellSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsShell );

  OpenHoopsSegment( strShellSegmentName );

  HC_KEY iKey = 0;

  setHoopsMaterial( curTraits );

  if( bTexture && m_MaterialInfo.strTextureFilename.isEmpty() )
    bTexture = false;

  bool bHasHoles = false;

  OdInt32 iItemCount = 0;

  while( iItemCount < faceListSize )
  {
    OdInt32 nVerticesInFace = faceList[ iItemCount++ ];

    if( nVerticesInFace < 0 )
    {
      bHasHoles = true;
      break;
    }

    iItemCount += abs(nVerticesInFace);
  }

  if( bTexture || bHasHoles )
  {
    bool bDrawEdges = false;
    OdGiFillType oldFillType = curTraits.fillType();

    if( shmFillMode(bDrawEdges) )
    {
      drawContext()->subEntityTraits().setFillType( kOdGiFillAlways );
    }

    m_bExportGeometryOnly = true;
    m_bSkipLines = true;
    OdGiGeometrySimplifier::shellProc( numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData );
    m_bSkipLines = false;
    m_bExportGeometryOnly = false;
    drawContext()->subEntityTraits().setFillType( kOdGiFillAlways );
  }
  else
   iKey = HoopsExportShell( numVertices, pPointArr, faceListSize, newFaceList );

  if( !bTexture && !bHasHoles )
  {
    HC_Open_Geometry( iKey );

    if( pVertexData )
    {
      for( OdInt32 i = 0; i < numVertices; i++ )
      {
        HC_Open_Vertex( i );

        if( pVertexData->trueColors() )
          setHoopsVertexColor( pVertexData->trueColors()[i] );

        if( pVertexData->normals() )
          setHoopsVertexNormal( pVertexData->normals()[i] );

        HC_Close_Vertex();
      }
    }

    if( pEdgeData )
    {
      OdInt32 iItemCount = 0;
      OdInt32 iEdgeCount = 0;

      while( iItemCount < faceListSize )
      {
        OdInt32 nVerticesInFace = faceList[ iItemCount++ ];
        OdInt32 iFirstIndex     = faceList[ iItemCount ];
        OdInt32 iStartIndex     = 0;
        OdInt32 iEndIndex       = 0;

        for( OdInt32 j = 0; j < nVerticesInFace; j++ )
        {
          if( j < nVerticesInFace - 1 )
          {
            iStartIndex = faceList[ iItemCount++ ];
            iEndIndex   = faceList[ iItemCount ];
          }
          else
          {
            iStartIndex = faceList[ iItemCount++ ];
            iEndIndex   = iFirstIndex;
          }

          if( iStartIndex != iEndIndex )
          {
            HC_Open_Edge( iStartIndex, iEndIndex );

            if( pEdgeData->trueColors() )
              setHoopsEdgeColor( pEdgeData->trueColors()[iEdgeCount] );

            if( pEdgeData->visibility() )
              setHoopsEdgeVisibility( !(pEdgeData->visibility()[iEdgeCount] == kOdGiInvisible ) );

            HC_Close_Edge();
          }

          iEdgeCount++;
        }
      }
    }

    if( pFaceData )
    {
      OdInt32 iItemCount = 0;
      OdInt32 iFaceCount = 0;

      while( iItemCount < faceListSize )
      {
        OdInt32 nVerticesInFace = faceList[ iItemCount++ ];

        if( nVerticesInFace > 0 )
        {
          HC_Open_Face( iFaceCount );

          if( pFaceData->trueColors() )
            setHoopsFaceColor( pFaceData->trueColors()[iFaceCount] );

          if( pFaceData->normals() )
            setHoopsFaceNormal( pFaceData->normals()[iFaceCount] );

          if( pFaceData->visibility() )
            setHoopsFaceVisibility( !(pFaceData->visibility()[iFaceCount] == kOdGiInvisible ) );

          HC_Close_Face();

          iFaceCount++;
        }

        iItemCount += abs(nVerticesInFace);
      }
    }

    HC_Close_Geometry();
  }

  if( !IsLayerVisible(curTraits.layer()) )
    HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);

  HC_Close_Segment();

  if( newFaceList )
    delete[] newFaceList;

  delete[] pPointArr;
}

//===================================================================\\

void OdHoopsExportDevice::dc_mesh( 
  OdInt32 numRows, 
  OdInt32 numColumns, 
  const OdGePoint3d* vertexList, 
  const OdGiEdgeData* pEdgeData, 
  const OdGiFaceData* pFaceData, 
  const OdGiVertexData* pVertexData )
{
  if( !m_bRecording )
    return;

  OdGiSubEntityTraitsData curTraits = m_curTraits;

  if( !fillMode() && numRows == 2 && numColumns == 2)
  {
    m_bInitNormal = false;

    OdInt32 numPoints = 5;

    OdGePoint3d ptsLine[5];

    ptsLine[0] = vertexList[0];
    ptsLine[1] = vertexList[1];
    ptsLine[2] = vertexList[3];
    ptsLine[3] = vertexList[2];
    ptsLine[4] = vertexList[0];

    dc_polyline( numPoints, ptsLine );

    return;
  }

  bool bTexture = false;

  if( m_MaterialInfo.bSetMaterial && m_MaterialInfo.strTextureFilename != OdString::kEmpty )
    bTexture = true;

  HoopsPoint* pPointArr = new HoopsPoint[numRows*numColumns];

  for( OdInt32 i = 0; i < numRows; i++ )
    for( OdInt32 j = 0; j < numColumns; j++ )
  {
    pPointArr[i*numColumns +j].x = (float)vertexList[i*numColumns + j].x;
    pPointArr[i*numColumns +j].y = (float)vertexList[i*numColumns + j].y;
    pPointArr[i*numColumns +j].z = (float)vertexList[i*numColumns + j].z;
  }

  OdString strMeshSegmentName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsMesh );

  OpenHoopsSegment( strMeshSegmentName );

  HC_KEY iKey = 0;

  setHoopsMaterial( curTraits );

  if( bTexture && m_MaterialInfo.strTextureFilename.isEmpty() )
    bTexture = false;

  if( bTexture )
  {
    m_bExportGeometryOnly = true;
    m_bSkipLines = true;
    OdGiGeometrySimplifier::meshProc( numRows, numColumns, vertexList, pEdgeData, pFaceData, pVertexData );
    m_bSkipLines = false;
    m_bExportGeometryOnly = false;
  }
  else
    iKey = HoopsExportMesh( numRows, numColumns, pPointArr );

  if( !bTexture )
  {
    HC_Open_Geometry( iKey );
    
    if( pVertexData )
    {
      for( OdInt32 i = 0; i < numRows; i++ )
        for( OdInt32 j = 0; j < numColumns; j++ )
        {
          HC_Open_Vertex( i * numColumns + j );

          if( pVertexData->trueColors() )
            setHoopsVertexColor( pVertexData->trueColors()[i * numColumns + j] );

          if( pVertexData->normals() )
            setHoopsVertexNormal( pVertexData->normals()[i * numColumns + j] );

          HC_Close_Vertex();
        }
    }

    if( pEdgeData )
    {
      for( OdInt32 i = 0; i < numRows; i++ )
        for( OdInt32 j = 0; j < numColumns - 1; j++ )
        {
          HC_Open_Edge( i * numColumns + j, i * numColumns + j + 1 );

          if( pEdgeData->trueColors() )
            setHoopsEdgeColor( pEdgeData->trueColors()[i * (numColumns - 1) + j] );

          if( pEdgeData->visibility() )
            setHoopsEdgeVisibility( !(pEdgeData->visibility()[i * (numColumns - 1) + j] == kOdGiInvisible ) );

          HC_Close_Edge();
        }

      for( OdInt32 j = 0; j < numColumns; j++ )
        for( OdInt32 i = 0; i < numRows - 1; i++ )
          {
            HC_Open_Edge( i * numColumns + j, (i + 1) * numColumns + j );

            if( pEdgeData->trueColors() )
              setHoopsEdgeColor( pEdgeData->trueColors()[ numRows * (numColumns - 1) + j*(numRows - 1) + i] );

            if( pEdgeData->visibility() )
              setHoopsEdgeVisibility( !(pEdgeData->visibility()[numRows * (numColumns - 1) + j*(numRows - 1) + i] == kOdGiInvisible ) );

            HC_Close_Edge();
          }
    }

    if( pFaceData )
    {
      for( OdInt32 i = 0; i < numRows - 1; i++ )
        for( OdInt32 j = 0; j < numColumns - 1; j++ )
        {
          HC_Open_Face( 2*( i * (numColumns - 1) + j ) );

          if( pFaceData->trueColors() )
            setHoopsFaceColor( pFaceData->trueColors()[i * (numColumns - 1) + j] );

          if( pFaceData->normals() )
            setHoopsFaceNormal( pFaceData->normals()[i * (numColumns - 1) + j] );

          if( pFaceData->visibility() )
            setHoopsFaceVisibility( !(pFaceData->visibility()[i * (numColumns - 1) + j] == kOdGiInvisible ) );

          HC_Close_Face();

          HC_Open_Face( 2*( i * (numColumns - 1) + j ) + 1 );

          if( pFaceData->trueColors() )
            setHoopsFaceColor( pFaceData->trueColors()[i * (numColumns - 1) + j] );

          if( pFaceData->normals() )
            setHoopsFaceNormal( pFaceData->normals()[i * (numColumns - 1) + j] );

          if( pFaceData->visibility() )
            setHoopsFaceVisibility( !(pFaceData->visibility()[i * (numColumns - 1) + j] == kOdGiInvisible ) );

          HC_Close_Face();
        }
    }

    HC_Close_Geometry();

  }

  if( !IsLayerVisible(curTraits.layer()) )
    HC_Set_Visibility(HOOPS_HIDE_GEOMETRY);

  HC_Close_Segment();

  delete[] pPointArr;
}

//===================================================================\\

void OdHoopsExportDevice::dc_open_segment( HC_KEY iSegmentKey )
{
  HC_Open_Segment_By_Key( iSegmentKey );
}

//===================================================================\\

HC_KEY OdHoopsExportDevice::dc_open_segment( OdString strSegmentName )
{
  char* pchSegmentName = OdStringToPChar(strSegmentName);
  HC_KEY iKey = HC_KOpen_Segment( pchSegmentName );
  delete[] pchSegmentName;

  return iKey;
}

//===================================================================\\

void OdHoopsExportDevice::dc_close_segment()
{
  HC_Close_Segment();
}

//===================================================================\\

void  OdHoopsExportDevice::setHoopsMaterial(const OdGiSubEntityTraitsData& Traits, bool b3dObjectMaterial  )
{
  HC_Set_Visibility( HOOPS_3DOBJECT_VISIBILITY_OPTIONS );

  if( m_MaterialInfo.bSetMaterial )
  {
    OdCmEntityColor clrEntity = Traits.trueColor();

    OdUInt8 uRed, uGreen, uBlue;

    if( clrEntity.colorMethod() == OdCmEntityColor::kByACI || 
        clrEntity.colorMethod() == OdCmEntityColor::kByDgnIndex )
    {
      if( m_logPalette.size() > (OdUInt16)clrEntity.colorIndex() ) 
      {
        ODCOLORREF trueColor = getColor( clrEntity.colorIndex() );

        uRed   = ODGETRED(trueColor);
        uGreen = ODGETGREEN(trueColor);
        uBlue  = ODGETBLUE(trueColor);
      }
      else
      {
        uRed   = OdCmEntityColor::mLUT[ clrEntity.colorIndex() ][0];
        uGreen = OdCmEntityColor::mLUT[ clrEntity.colorIndex() ][1];
        uBlue  = OdCmEntityColor::mLUT[ clrEntity.colorIndex() ][2];
      }
    }
    else
    {
      uRed = clrEntity.red();
      uGreen = clrEntity.green();
      uBlue = clrEntity.blue();
    }

    ODCOLORREF clrAmbient  = 0;
    ODCOLORREF clrDiffuse  = ODRGB( uRed, uGreen, uBlue );
    ODCOLORREF clrSpecular = clrDiffuse;

    if( m_MaterialInfo.bUseAmbient )
      clrAmbient = m_MaterialInfo.clrAmbient;
      
    if( m_MaterialInfo.bUseDiffuse )
      clrDiffuse = m_MaterialInfo.clrDiffuse;

    if( m_MaterialInfo.bUseSpecular )
      clrSpecular = m_MaterialInfo.clrSpecular;

    if( m_MaterialInfo.strTextureFilename != OdString::kEmpty &&
        m_MaterialInfo.iTextureType == DIFFUSE_TEXTURE 
      )
    {
      OdUInt8 newR = (OdUInt8)(ODGETRED(clrDiffuse) + (255.0 - ODGETRED(clrDiffuse))*m_MaterialInfo.dTextureBrightness);
      OdUInt8 newG = (OdUInt8)(ODGETGREEN(clrDiffuse) + (255.0 - ODGETGREEN(clrDiffuse))*m_MaterialInfo.dTextureBrightness);
      OdUInt8 newB = (OdUInt8)(ODGETBLUE(clrDiffuse) + (255.0 - ODGETBLUE(clrDiffuse))*m_MaterialInfo.dTextureBrightness);
      clrDiffuse = ODRGB( newR, newG, newB );
    }
    
    setHoopsDiffuseColor( clrDiffuse );
    setHoopsSpecularColor( clrSpecular );
    setHoopsGloss( m_MaterialInfo.dGlossFactor );
    setHoopsAmbientColor( clrAmbient );
    setHoopsTransparency( m_MaterialInfo.dTranslucence );

    if( m_MaterialInfo.strTextureFilename != OdString::kEmpty )
    {
      OdString strTextureName = CreateHoopsTexture();
      OdString strColorOptions = OdString::kEmpty;

      if( strTextureName != OdString::kEmpty )
      {
        switch( m_MaterialInfo.iTextureType )
        {
          case DIFFUSE_TEXTURE:
            strColorOptions.format( L_SET_DIFFUSE_TEXTURE, strTextureName.c_str() ); break;
          case BUMP_TEXTURE:
            strColorOptions.format( L_SET_BUMP_TEXTURE, strTextureName.c_str() ); break;
          case REFLECTION_TEXTURE:
            strColorOptions.format( L_SET_REFLECTION_TEXTURE, strTextureName.c_str() ); break;
          case SPECULAR_TEXTURE:
            strColorOptions.format( L_SET_SPECULAR_TEXTURE, strTextureName.c_str() ); break;
        }

        if( strColorOptions != OdString::kEmpty )
        {
          char* pchColorOptions = OdStringToPChar( strColorOptions );
          HC_Set_Color( pchColorOptions );
          delete[] pchColorOptions;
        }
      }
      else
        m_MaterialInfo.strTextureFilename.empty();
    }
  }
  else
  {
    setHoopsPolygonColor( Traits.trueColor() );

    if( !b3dObjectMaterial || m_MaterialInfo.bTTFProcessing )
    {
      HC_Set_Visibility( HOOPS_IMAGE_VISIBILITY_OPTIONS );
      HC_Set_Rendering_Options(HOOPS_IMAGE_RENDERING_OPTIONS);
    }
  }
}

//===================================================================\\

OdString OdHoopsExportDevice::CreateHoopsTexture()
{
  OdString strRet = OdString::kEmpty;

  OdString strSourceName = getHoopsTextureImage( m_MaterialInfo.strTextureFilename, (1.0 - m_MaterialInfo.dTextureBrightness) * 100 );

  if( strSourceName != OdString::kEmpty )
  {
    OdString strTextureName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsTexture );
    OdString strTextureOptions = OdString::kEmpty;

    strTextureOptions.format(L_TEXTURE_SOURCE_FORMAT, strSourceName.c_str());

    strTextureOptions = strTextureOptions + "," + L_MODULATE;
    strTextureOptions = strTextureOptions + "," + L_TEXTURE_UV;

    if( m_MaterialInfo.uTiling == OdGiMapper::kClamp )
      strTextureOptions = strTextureOptions + "," + L_TILING_CLAMP;
    else if( m_MaterialInfo.uTiling == OdGiMapper::kMirror )
      strTextureOptions = strTextureOptions + "," + L_TILING_MIRROR;
    else
      strTextureOptions = strTextureOptions + "," + L_TILING_REPEAT;

    char* pchTextureName = OdStringToPChar( strTextureName );
    char* pchTextureOptn = OdStringToPChar( strTextureOptions );

    HC_Define_Local_Texture( pchTextureName, pchTextureOptn );

    strRet = strTextureName;

    delete[] pchTextureName;
    delete[] pchTextureOptn;
  }

  return strRet;
}

//===================================================================\\

OdString OdHoopsExportDevice::getHoopsTextureImage( OdString strFileName, double dFade )
{
  OdString strRes = OdString::kEmpty;

  bool bCreateImage = true;

  for( OdUInt32 i = 0; i < m_pHoops->arrTexturePairs.size(); i++ )
  {
    if( strFileName == m_pHoops->arrTexturePairs[i].strFileName &&
        OdZero( dFade - m_pHoops->arrTexturePairs[i].dFade )
      )
    {
      bCreateImage = false;
      strRes = m_pHoops->arrTexturePairs[i].strHoopsName;
      break;
    }
  }

  if( bCreateImage )
  {
    OdString strTextureName = defineHoopsObjectName( OdHoopsExportDevice::kHoopsImage, false );
    HoopsTextureImagePair tmpPair;
    tmpPair.strFileName  = strFileName;
    tmpPair.dFade        = dFade;
    tmpPair.strHoopsName = strTextureName;

    if( strTextureName != OdString::kEmpty && CreateHoopsTextureImage( strFileName, strTextureName, dFade) )
    {
      m_pHoops->arrTexturePairs.push_back( tmpPair );
      strRes = strTextureName;
    }
  }

  return strRes;
}

//===================================================================\\

bool OdHoopsExportDevice::CreateHoopsTextureImage( OdString strFileName, OdString strTextureName, double dFade )
{
  OdGiRasterImagePtr pRasterBase = OdGiRasterImageLoader::createObject()->loadRasterImage(m_MaterialInfo.strTextureFilename
                                                                                            , giCtx(), OdDbBaseHostAppServices::kEmbeddedImageFile);
  if( !pRasterBase.isNull() )
  {
    OdGiRasterImagePtr pRasterMain = pRasterBase->convert( true, 50.0, 50.0, dFade, 0, false, true);
    return InsertHoopsImage( pRasterMain, strTextureName, false, true );
  }
  else
    return false;
}

//===================================================================\\

bool OdHoopsExportDevice::IsLayerVisible( OdDbStub* layerId )
{
  OdRxObjectPtr pLayer(userGiContext()->openDrawable(layerId));

  if( pLayer.isNull() )
    return true;

  OdDbBaseLayerPEPtr lpe(pLayer);

  if( lpe.isNull() )
    return true;
  else if( lpe->isOff(pLayer) || lpe->isFrozen(pLayer) )
    return false;
  else
    return true;
}

//===================================================================\\

HC_KEY OdHoopsExportDevice::createActiveLayoutSegment( OdString strName )
{
  strName.replace(L'(',L'-');
  strName.replace(L')',L'-');
  strName.replace(L'=',L'-');
  HC_KEY lRet = dc_open_segment( strName );
  HC_Set_Visibility( HOOPS_SHOW_GEOMETRY );
  dc_close_segment();
  return lRet;
}

//===================================================================\\

HC_KEY OdHoopsExportDevice::createInvisibleLayoutSegment( OdString strName )
{
  strName.replace(L'(',L'-');
  strName.replace(L')',L'-');
  strName.replace(L'=',L'-');
  HC_KEY lRet = dc_open_segment( strName );
  HC_Set_Visibility( HOOPS_HIDE_GEOMETRY );
  dc_close_segment();
  return lRet;
}

//===================================================================\\

void  OdHoopsExportDevice::setHoopsPolygonColor( const OdCmEntityColor& color )
{
  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  setHoopsAmbientColor( 0 );
  setHoopsDiffuseColor( ODRGB( uRed, uGreen, uBlue) );
  setHoopsSpecularColor( ODRGB( uRed, uGreen, uBlue) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  OdHoopsExportDevice::setHoopsLineColor( const OdCmEntityColor& color )
{
  bool bChangeBlackWhite = false;

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  double rColor = uRed/255.0;
  double gColor = uGreen/255.0;
  double bColor = uBlue/255.0;

  if( bChangeBlackWhite && uRed == 255 && uGreen == 255 && uBlue == 255 )
    rColor = gColor = bColor = 0;

  HC_Set_Color_By_Value( LINE_COLOR_TYPE,HOOPS_COLORSPACE, rColor, gColor, bColor );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  OdHoopsExportDevice::setHoopsTextColor( const OdCmEntityColor& color )
{
  bool bChangeBlackWhite = false;

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  double rColor = uRed/255.0;
  double gColor = uGreen/255.0;
  double bColor = uBlue/255.0;

  if( bChangeBlackWhite && uRed == 255 && uGreen == 255 && uBlue == 255 )
    rColor = gColor = bColor = 0;

  HC_Set_Color_By_Value( LINE_COLOR_TYPE,HOOPS_COLORSPACE, rColor, gColor, bColor );
  HC_Set_Color_By_Value( FACE_COLOR_TYPE,HOOPS_COLORSPACE, rColor, gColor, bColor );
  HC_Set_Color_By_Value( TEXT_COLOR_TYPE,HOOPS_COLORSPACE, rColor, gColor, bColor );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void  OdHoopsExportDevice::setHoopsVertexColor( const OdCmEntityColor& color )
{
  bool bChangeBlackWhite = false;

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  double rColor = uRed/255.0;
  double gColor = uGreen/255.0;
  double bColor = uBlue/255.0;

  if( bChangeBlackWhite && uRed == 255 && uGreen == 255 && uBlue == 255 )
    rColor = gColor = bColor = 0;

  HC_Set_Color_By_Value( "front", HOOPS_COLORSPACE, rColor, gColor, bColor );
  HC_Set_Color_By_Value( "back", HOOPS_COLORSPACE, rColor, gColor, bColor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void  OdHoopsExportDevice::setHoopsEdgeColor( const OdCmEntityColor& color )
{
  bool bChangeBlackWhite = false;

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)(color.colorIndex()) ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  double rColor = uRed/255.0;
  double gColor = uGreen/255.0;
  double bColor = uBlue/255.0;

  if( bChangeBlackWhite && uRed == 255 && uGreen == 255 && uBlue == 255 )
    rColor = gColor = bColor = 0;

  HC_Set_Color_By_Value( ALL_COLOR_TYPE,HOOPS_COLORSPACE, rColor, gColor, bColor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void  OdHoopsExportDevice::setHoopsFaceColor( const OdCmEntityColor& color )
{
  bool bChangeBlackWhite = false;

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  double rColor = uRed/255.0;
  double gColor = uGreen/255.0;
  double bColor = uBlue/255.0;

  if( bChangeBlackWhite && uRed == 255 && uGreen == 255 && uBlue == 255 )
    rColor = gColor = bColor = 0;

  HC_Set_Color_By_Value( ALL_COLOR_TYPE,HOOPS_COLORSPACE, rColor, gColor, bColor );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void OdHoopsExportDevice::addHoopsPointLight( const OdString& strSegName, 
                        OdGePoint3d ptPos, 
                        OdCmEntityColor color, 
                        double dIntensity, 
                        bool bOn )
{
  OpenHoopsSegment( strSegName );

  HC_Insert_Local_Light( ptPos.x, ptPos.y, ptPos.z );

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  double rColor = uRed/255.0 * dIntensity;

  if( rColor > 1 )
    rColor = 1;

  double gColor = uGreen/255.0 * dIntensity;

  if( gColor > 1 )
    gColor = 1;

  double bColor = uBlue/255.0 * dIntensity;

  if( bColor > 1 )
    bColor = 1;

  OdString strColorOpts;
  strColorOpts.format( L_SET_POINTLIGHT_FORMAT, rColor, gColor, bColor );
  char* pchColorOpts = OdStringToPChar( strColorOpts );
  HC_Set_Color( pchColorOpts );
  delete[] pchColorOpts;

  if( bOn )
    HC_Set_Visibility( HOOPS_LIGHT_ON );
  else
    HC_Set_Visibility( HOOPS_LIGHT_OFF );

  HC_Close_Segment();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void OdHoopsExportDevice::addHoopsSpotLight( const OdString& strSegName, 
                       OdGePoint3d ptPos, 
                       OdGePoint3d ptTarget, 
                       double dHotSpot, 
                       double dFallOff, 
                       OdCmEntityColor color, 
                       double dIntensity, 
                       bool bOn )
{
  OpenHoopsSegment( strSegName );

  OdString strSpotOptions;
  strSpotOptions.format( L_SPOT_LIGHT_FORMAT, dFallOff*180.0/OdaPI, dHotSpot*180.0/OdaPI );
  char* pchSpotOptions = OdStringToPChar( strSpotOptions );

  HoopsPoint ptHoopsPos, ptHoopsTarget;
  ptHoopsPos.x = (float)ptPos.x;
  ptHoopsPos.y = (float)ptPos.y;
  ptHoopsPos.z = (float)ptPos.z;
  ptHoopsTarget.x = (float)ptTarget.x;
  ptHoopsTarget.y = (float)ptTarget.y;
  ptHoopsTarget.z = (float)ptTarget.z;

  HC_Insert_Spot_Light(&ptHoopsPos, &ptHoopsTarget, pchSpotOptions );

  delete[] pchSpotOptions;

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }

  double rColor = uRed/255.0 * dIntensity;

  if( rColor > 1 )
    rColor = 1;

  double gColor = uGreen/255.0 * dIntensity;

  if( gColor > 1 )
    gColor = 1;

  double bColor = uBlue/255.0 * dIntensity;

  if( bColor > 1 )
    bColor = 1;

  OdString strColorOpts;
  strColorOpts.format( L_SET_POINTLIGHT_FORMAT, rColor, gColor, bColor );
  char* pchColorOpts = OdStringToPChar( strColorOpts );
  HC_Set_Color( pchColorOpts );
  delete[] pchColorOpts;

  if( bOn )
    HC_Set_Visibility( HOOPS_LIGHT_ON );
  else
    HC_Set_Visibility( HOOPS_LIGHT_OFF );

  HC_Close_Segment();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void OdHoopsExportDevice::addHoopsDistantLight( const OdString& strSegName, 
                          OdGeVector3d vrDir,
                          OdCmEntityColor color, 
                          double dIntensity, 
                          bool bOn )
{
  OpenHoopsSegment( strSegName );

  HC_Insert_Distant_Light( vrDir.x, vrDir.y, vrDir.z );

  OdUInt8 uRed, uGreen, uBlue;

  if( color.colorMethod() == OdCmEntityColor::kByACI || 
    color.colorMethod() == OdCmEntityColor::kByDgnIndex )
  {
    if( m_logPalette.size() > (OdUInt16)color.colorIndex() ) 
    {
      ODCOLORREF trueColor = getColor( color.colorIndex() );

      uRed   = ODGETRED(trueColor);
      uGreen = ODGETGREEN(trueColor);
      uBlue  = ODGETBLUE(trueColor);
    }
    else
    {
      uRed   = OdCmEntityColor::mLUT[ color.colorIndex() ][0];
      uGreen = OdCmEntityColor::mLUT[ color.colorIndex() ][1];
      uBlue  = OdCmEntityColor::mLUT[ color.colorIndex() ][2];
    }
  }
  else
  {
    uRed = color.red();
    uGreen = color.green();
    uBlue = color.blue();
  }


  double rColor = uRed/255.0 * dIntensity;

  if( rColor > 1 )
    rColor = 1;

  double gColor = uGreen/255.0 * dIntensity;

  if( gColor > 1 )
    gColor = 1;

  double bColor = uBlue/255.0 * dIntensity;

  if( bColor > 1 )
    bColor = 1;

  OdString strColorOpts;
  strColorOpts.format( L_SET_POINTLIGHT_FORMAT, rColor, gColor, bColor );
  char* pchColorOpts = OdStringToPChar( strColorOpts );
  HC_Set_Color( pchColorOpts );
  delete[] pchColorOpts;

  if( bOn )
    HC_Set_Visibility( HOOPS_LIGHT_ON );
  else
    HC_Set_Visibility( HOOPS_LIGHT_OFF );

  HC_Close_Segment();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

OdUInt32 OdHoopsExportDevice::getPaletteSize() const
{
  return m_logPalette.size();
}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
