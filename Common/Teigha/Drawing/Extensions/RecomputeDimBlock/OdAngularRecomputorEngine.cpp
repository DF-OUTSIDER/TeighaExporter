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
#include "OdAngularRecomputorEngine.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "Ge/GeCircArc2d.h"

#ifdef _DEBUG
#define DEBUG_BUILD_ARC_DIM
#endif

double OdAngularRecomputorEngine::measurement()
{
  return 0;
}

void OdAngularRecomputorEngine::tranformBy(const OdGeMatrix3d& matr)
{
  OdRecomputorEngine::tranformBy(matr);
  m_centerPtArc.transformBy(matr);
  m_arcPt.transformBy(matr);
  m_centerPtArc.z = m_arcPt.z = 0.0;
}

void OdAngularRecomputorEngine::setAszParams(double asz1, double asz2, double asz, bool flipArrow1, bool flipArrow2)
{
  OdRecomputorEngine::setAszParams(asz1, asz2, asz, flipArrow1, flipArrow2);
  m_Arrow1Angle = chordAngle(m_asz1);
  m_Arrow2Angle = chordAngle(m_asz2);
}

void OdAngularRecomputorEngine::adjustTextAndArrowsPlace(OdUInt16 tfit, bool tix)
{
  m_textPosition = m_middleArcPt;

  if (m_just < 3)
  {
    if ( m_tad == 0 )
      fitTextAndArrows(true, true);
    else
      fitTextAndArrows(true, true, false);

    if ( tix )
    {
      m_bTextInside = true;
      m_bArrowsInside = m_isFitArrowsWithoutText;
    } else
    {
      applyDimTFit(tfit);
    }
    // applyDimTFit(tfit);

    if ( !m_bUseRotate )
    {
      if ( m_bTextInside && m_dimTih )
      {
        m_bTextHor = true;
      }
      if ( !m_bTextInside && m_dimToh )
      {
        m_bTextHor = true;
      }
      if ( m_bTextHor )
      {
        m_dTextRot = 0.0;
        m_textDirection = m_xAxis;
      }
    }
  } else
  {
    fitTextAndArrows(false, true);
    if ( !m_isFitArrowsWithoutText )
      m_bArrowsInside = false;
  }
}

void OdAngularRecomputorEngine::adjustTextLocation(OdInt16 dimMove, double dimExe)
{
  if ( m_bTextInside )
  {
    adjustTextByHor(dimExe);
    if ( m_bUseRotate || m_bTextHor )
    {
      OdGeVector3d v = m_xLine1Pt - m_xLine2Pt;
      bool bHor = v.isCodirectionalTo(m_xAxis) || v.isCodirectionalTo(-m_xAxis);

      if ( m_just > 2 || ( m_bTextHor && ( m_tad > 1 || (bHor && ( m_tad == 1 || m_tad == 4 )))))
        adjustTextByVert();
    } else
      adjustTextByVert();

    if ( m_just > 2 )
      m_bTextInside = false;
  } else 
  {
    m_bNeedCalcTextIntersection = false;

    if ( m_just < 3 && ( dimMove == 1 || dimMove == 2 ) )
    {
      applyDimMoveForDefaultPos(dimMove);
      m_bTextInside = true;
    } else
    {
      if ( !m_bArrowsInside )
        textAndArrowToOutside();
      else
        textOnlyToOutside();
    }
  }

  if ( m_bTextHor )
    m_textDirection = m_xAxis;
}

void OdAngularRecomputorEngine::applyDimMoveForDefaultPos(OdUInt16 dimMove)
{
  if ( dimMove == 1 )
  {
    m_textPosition = m_centerPtArc;
    m_textPosition += m_dirToMiddleArcPt*(m_radiusDimArc + 2.0*m_asz);
    double width = textWidth(false);
    double height = textHeight(false);

    if ( m_bTextHor )
    {
      m_bLeader = true;
      m_ldrPt2 = m_textPosition;
      OdGeVector3d v = m_xAxis;
      if ( m_dirToMiddleArcPt.x < 0  )
        v = -m_xAxis;

      if ( m_tad > 0 )
      {
        m_textPosition += v*(width/2.0 + m_gap);
        m_ldrPt1 = m_ldrPt2 + v*(width + m_gap);
        double gap = m_gap;
        if ( m_isBox )
          m_gap *= 2.0;
        if ( m_tad == 2 && ( m_radiusDimArc <= m_sourceArcRadius )  )
        {
          m_textPosition -= m_yAxis*(height/2.0 + gap);
        } else
        {
          m_textPosition += m_yAxis*(height/2.0 + gap);
        }
      } else
      {
        m_textPosition += v*m_asz;
        m_ldrPt1 = m_textPosition;
        m_textPosition += v*(width/2.0 + m_gap);
      }
    } else
    {
      m_bTextHasLine = true;
      m_textLinePt1 = m_middleArcPt;

      double maxDist = 0.0;
      maxDist = odmax(maxDist, m_centerPtArc.distanceTo(m_xLine1Pt));
      maxDist = odmax(maxDist, m_centerPtArc.distanceTo(m_xLine2Pt));
      maxDist = odmax(maxDist, m_centerPtArc.distanceTo(m_xLine1PtEnd));
      maxDist = odmax(maxDist, m_centerPtArc.distanceTo(m_xLine2PtEnd));

      if (maxDist > m_radiusDimArc)
      {
        m_textPosition += m_dirToMiddleArcPt*(maxDist - m_radiusDimArc);
      }
      m_textLinePt2 = m_textPosition;

      if ( !m_bUseRotate )
      {
        m_textDirection = m_dirToMiddleArcPt.crossProduct(OdGeVector3d::kZAxis).normalize();
        correctTextDir();
      }
      m_textPosition += m_dirToMiddleArcPt*(height/2.0 + m_gap);
    }
  } else if ( dimMove == 2 )
  {
    m_textPosition = m_middleArcPt;
    m_textPosition += m_dirToMiddleArcPt*(m_asz*2.0);
    if ( !m_bUseRotate && !m_bTextHor )
    {
      m_textDirection = m_dirToMiddleArcPt.crossProduct(OdGeVector3d::kZAxis).normalize();
      correctTextDir();
    }
  }
  return;
}

void OdAngularRecomputorEngine::applyFlipArrows()
{
  m_bArrow1Inside = m_bArrow2Inside = m_bArrowsInside;
  if ( m_bArrow1Flip )
  {
    m_bArrow1Inside = !m_bArrow1Inside;
  }
  if ( m_bArrow2Flip )
  {
    m_bArrow2Inside = !m_bArrow2Inside;
  }
}

void OdAngularRecomputorEngine::correctTextDir()
{
  if ( !m_bUseRotate && !m_bTextHor)
  {
    if ( m_just < 3 )
    {
      OdGeVector3d v = m_middleArcPt - m_centerDimPtArc;
      double d = v.angleTo(m_xAxis, OdGeVector3d::kZAxis);
      if ( d < OdaPI )
        m_textDirection *= -1.0;
    } else
    {
      double d = m_textDirection.angleTo(m_xAxis, OdGeVector3d::kZAxis);
      if ( d > OdaPI2 && d < OdaPI + OdaPI2 )
        m_textDirection *= -1.0;
    }
  } else if ( m_just > 2 )
  {
    double d = m_textDirection.angleTo(m_xAxis, OdGeVector3d::kZAxis);
    if ( d > OdaPI2 && d < OdaPI + OdaPI2 )
      m_textDirection *= -1.0;
  }
}

double OdAngularRecomputorEngine::chordAngle(double chord)
{
  return OdRecomputorEngine::chordAngle(chord, m_radiusDimArc);
}

//  Input:
//    m_xLine1PtEnd, m_xLine1PtEnd,  m_centerDimPtArc
//    m_Arrow1Angle, m_Arrow2Angle
//  Result:
//    m_dirArrow1, m_dirArrow2
void OdAngularRecomputorEngine::calcExtLineAngles()
{
  OdGeVector3d extLine1Dir = m_xLine1PtEnd - m_centerDimPtArc;
  OdGeVector3d extLine2Dir = m_xLine2PtEnd - m_centerDimPtArc;

  m_extLine1Angle = OdGeVector3d::kXAxis.angleTo(extLine1Dir, OdGeVector3d::kZAxis);
  m_extLine2Angle = OdGeVector3d::kXAxis.angleTo(extLine2Dir, OdGeVector3d::kZAxis);

  if ( m_middleArcPt == m_centerDimPtArc )
  {
    m_dirToMiddleArcPt = m_xLine1Dir + m_xLine2Dir;
    m_dirToMiddleArcPt *= -1.0;
  }
  else
    m_dirToMiddleArcPt = m_middleArcPt - m_centerDimPtArc;
  m_dirToMiddleArcPt.normalize();
}

//  Input:
//    m_xLine1PtEnd, m_xLine1PtEnd,  
//    m_centerDimPtArc, m_radiusDimArc
//    m_Arrow1Angle, m_Arrow2Angle, 
//    m_extLine1Angle, m_extLine2Angle
//    m_arrow1Pt, m_arrow2Pt
//  Result:
//    m_dirArrow1, m_dirArrow2
void OdAngularRecomputorEngine::calcDirectionArrows()
{
  double dir1 = 1.0, dir2 = 1.0;
  if ( !m_bArrow1Inside )
  {
    dir1 = -1.0;
  }
  if ( m_bArrow2Inside )
  {
    dir2 = -1.0;
  }

  m_dirArrow1 = dir1*m_xLine1Dir.perpVector();
  m_dirArrow1.rotateBy(dir1*m_Arrow1Angle/2.0, OdGeVector3d::kZAxis);

  m_dirArrow2 = dir2*m_xLine2Dir.perpVector();
  m_dirArrow2.rotateBy(dir2*m_Arrow2Angle/2.0, OdGeVector3d::kZAxis);

  if ( m_bArrow1Flip && OdZero(m_asz1) )
  {
    m_dirArrow1.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  }
  if ( m_bArrow2Flip && OdZero(m_asz2) )
  {
    m_dirArrow2.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  }
}
// It is used for user define position text and dimmove = 0
void OdAngularRecomputorEngine::calcDimArcPoint()
{
  testDimExtLineOrder();
  m_radiusDimArc = m_textPosition.distanceTo(m_centerPtArc);
  
  // Preliminary calculate m_arcPt
  OdGeVector3d v1 = m_xLine1Dir;
  OdGeVector3d v2 = m_xLine2Dir;
  double angle = v1.angleTo(v2, OdGeVector3d::kZAxis);
  angle /= 3.0;
  v1.rotateBy(angle, OdGeVector3d::kZAxis);
  v2.rotateBy(-angle, OdGeVector3d::kZAxis);
  OdGeVector3d v = m_textPosition - m_centerPtArc;
  double angle1 = v.angleTo(v1);
  double angle2 = v.angleTo(v2);
  if ( angle1 < angle2 )
  {
    m_arcPt = m_centerPtArc + v2*m_radiusDimArc;
  } else
  {
    m_arcPt = m_centerPtArc + v1*m_radiusDimArc;
  }
}

void OdAngularRecomputorEngine::correctDimArcPoint()
{
  double angle = m_angle / 3.0; 
  OdGePoint3d p1 = m_arrow1Pt;
  OdGePoint3d p2 = m_arrow2Pt;
  p1.rotateBy(angle, OdGeVector3d::kZAxis, m_centerDimPtArc);
  p2.rotateBy(-angle, OdGeVector3d::kZAxis, m_centerDimPtArc);
  double d1 = m_textPosition.distanceTo(p1);
  double d2 = m_textPosition.distanceTo(p2);
  if ( d1 > d2 || OdZero(d1-d2) )
    m_arcPt = p1;
  else
    m_arcPt = p2;
}

// Input:
//  m_xLine1PtEnd, m_xLine2PtEnd, m_textExtentsWithGap, m_dTextRot
//  m_textPosition
// Return:
//  if text is within 2 parallel lines through dimArcPt1 and dimArcPt2.
bool OdAngularRecomputorEngine::isTextIn(const OdGePoint3d dimArcPt1, const OdGePoint3d dimArcPt2)
{
  bool isIn = true;

  OdGeVector3d chorda = m_xLine1PtEnd-m_xLine2PtEnd;
  if ( OdZero(chorda.length()) )
    return false;

  OdGeVector3d perpChorda = chorda.perpVector();
  perpChorda *= m_radiusDimArc*2.0;

  OdGePoint3d minPt; //  = m_textExtentsWithGap.minPoint();
  OdGePoint3d maxPt; // = m_textExtentsWithGap.maxPoint();

  // ACAD uses aligned extents relative m_textPosition
  double extX2 = textWidth()/2.0; //maxPt.x - minPt.x) / 2.0;
  double extY2 = textHeight()/2.0; //maxPt.y - minPt.y) / 2.0;

  minPt.x = m_textPosition.x - extX2;
  maxPt.x = m_textPosition.x + extX2;

  minPt.y = m_textPosition.y - extY2;
  maxPt.y = m_textPosition.y + extY2;

  OdGeLineSeg2d diagonal1(OdGePoint2d(minPt.x, minPt.y),OdGePoint2d(maxPt.x, maxPt.y));
  OdGeLineSeg2d diagonal2(OdGePoint2d(minPt.x, maxPt.y),OdGePoint2d(maxPt.x, minPt.y));

  if (!m_dimTih )
  {
    OdGeMatrix2d matr = OdGeMatrix2d::rotation(m_dTextRot, OdGePoint2d(m_textPosition.x, m_textPosition.y));
    diagonal1.transformBy(matr);
    diagonal2.transformBy(matr);
  }
#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("----- isTextIn START\n");
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", diagonal1.startPoint().x, diagonal1.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", diagonal1.endPoint().x, diagonal1.endPoint().y);
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", diagonal2.startPoint().x, diagonal2.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", diagonal2.endPoint().x, diagonal2.endPoint().y);
#endif

  OdGePoint3d p1 = dimArcPt1 - perpChorda, p2= dimArcPt1 + perpChorda;
  OdGeLineSeg2d limitLine1(OdGePoint2d(p1.x, p1.y), OdGePoint2d(p2.x, p2.y));

#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", p1.x, p1.y);
  ODA_TRACE2("%f,%f,0.0\n\n", p2.x, p2.y);
#endif

  p1 = dimArcPt2 - perpChorda;
  p2 = dimArcPt2 + perpChorda;
  OdGeLineSeg2d limitLine2(OdGePoint2d(p1.x, p1.y), OdGePoint2d(p2.x, p2.y));

#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", p1.x, p1.y);
  ODA_TRACE2("%f,%f,0.0\n\n", p2.x, p2.y);
  ODA_TRACE0("----- isTextIn END\n");
#endif

  OdGePoint2d intersecPt;
  if ( limitLine1.intersectWith(diagonal1, intersecPt) )
  {
    isIn = false;
  } else if (limitLine1.intersectWith(diagonal2, intersecPt))
  {
    isIn = false;
  } else if (limitLine2.intersectWith(diagonal1, intersecPt))
  {
    isIn = false;
  }  else if (limitLine2.intersectWith(diagonal2, intersecPt))
  {
    isIn = false;
  }
  return isIn;
}

// Input:
//    m_xLine1PtEnd, m_xLine1PtEnd
//    m_xLine1Pt, m_xLine2Pt
//    m_centerDimPtArc
// Output:
//   m_xLine1PtEnd, m_xLine2PtEnd corrected (extented on text extents)

void OdAngularRecomputorEngine::addUnderline()
{
  // TODO: it need to be reversed
  if ( m_just == 3 ) 
  {
    OdGeVector3d v  = m_xLine1PtEnd - m_centerDimPtArc;
    OdGeVector3d v1 = m_xLine1PtEnd - m_xLine1Pt;
    v.normalize();
    bool bNeedLine = false;
    if ( v.isCodirectionalTo(v1) )
      bNeedLine = true;
    else
    {
      v *= -1.0;
      if ( v.isCodirectionalTo(v1) )
        bNeedLine = true;
    }
    if ( bNeedLine )
    {
      m_xLine1PtEnd += v*(textWidth(false) + m_gap);
    }
  }
  else if ( m_just == 4 )
  {
    OdGeVector3d v  = m_xLine2PtEnd - m_centerDimPtArc;
    OdGeVector3d v1 = m_xLine2PtEnd - m_xLine2Pt;
    v.normalize();
    bool bNeedLine = false;
    if ( v.isCodirectionalTo(v1) )
      bNeedLine = true;
    else
    {
      v *= -1.0;
      if ( v.isCodirectionalTo(v1) )
        bNeedLine = true;
    }
    if ( bNeedLine )
    {
      m_xLine2PtEnd += v*(textWidth(false) + m_gap);
    }
  }

}

void OdAngularRecomputorEngine::adjustTextByVert()
{
  // Function isn't run in case text with rotation
  switch (m_tad)
  {
  case 0: // Centers the dimension text between the extension lines.
    // Already in place. Calculate intersect with arc is nedded.
    m_bNeedCalcTextIntersection = true;
    m_bNeedCheckTextFit = true;
    break;
  case 4:
  case 1: // Places the dimension text above the dimension line except when the dimension line is 
    // not horizontal and text inside the extension lines is forced horizontal (DIMTIH = 1). 
    // The distance from the dimension line to the baseline of the lowest line of text is 
    // the current DIMGAP value.
    //
  case 3:
    {
      if ( m_bUseRotate && !m_bTextHor && m_just > 2 )
      {
        return;
      }
      double distance = 0.0;
      if ( m_isBox ) 
        distance += m_gap;
      distance += textHeight(false)/2.0 + m_gap;

      if ( m_just < 3 )
        m_perpTextDir = m_textDirection.perpVector();

      m_perpTextDir.normalize();

      if ( m_just > 2 )
      {
        OdGeVector3d v = m_middleArcPt - m_centerDimPtArc;
        if ( v.y > 0.0 )
          m_perpTextDir *= -1.0;
      }

      if ( m_just < 3 && m_bTextHor )
      {
        if ( m_tad == 3  )
        {
          OdGeVector3d v = m_middleArcPt - m_centerDimPtArc;
          if ( OdZero(v.y) )
          {
            if ( v.x > 0.0 )
              m_perpTextDir *= -1.0;
          } else if ( !OdPositive(v.y) )
          {
            m_perpTextDir *= -1.0;
          }
          distance = 0.0;
          if ( m_isBox ) 
            distance += m_gap;
          // arc sym is needed
          double ang = m_perpTextDir.angleTo(OdGeVector3d::kYAxis);
          double h = textHeight()/2.0;
          double w = textWidth()/2.0;
          distance += w*sin(ang) + h*cos(ang);
        // TODO: for just == 1 & 2
        } else if ( m_tad == 1 )
        {
          m_perpTextDir = m_yAxis;
        }
      }
      if ( m_tad == 4 )
        m_perpTextDir *= -1.0;
      m_textPosition += m_perpTextDir * distance;
      m_bNeedCalcTextIntersection = false;
      if ( m_bUseRotate && !m_bTextHor )
      {
        return;
      }
      addUnderline();
      break;
    }
  case 2:     // Places the dimension text on the side of the dimension line farthest away from the 
    // defining points.
    {
      if ( m_bUseRotate && !m_bTextHor && m_just > 2)
      {
        return;
      }
      
      if ( m_just < 3 )
      {
        if ( m_radiusDimArc <= m_sourceArcRadius  )
          m_perpTextDir *= -1.0;
      } else  
      {
        if ( m_radiusDimArc > m_sourceArcRadius )
          m_perpTextDir *= -1.0;
      }

      m_perpTextDir.normalize();
      double distance = 0.0;
      if ( m_isBox ) 
        distance += m_gap;
      distance += textHeight(false)/2.0 + m_gap;

      if ( m_just < 3 && m_bTextHor ) // m_bUseRotate && OdZero(m_dTextRot) ) // && m_tad > 1
      {
        distance = 0.0;
        if ( m_isBox ) 
          distance += m_gap;
        // arc sym is needed
        OdGeVector3d v = m_middleArcPt - m_centerDimPtArc;
        bool bCoDir = m_perpTextDir.isCodirectionalTo(OdGeVector3d::kXAxis); // Seems ACAD bug
        if ( bCoDir )
          m_perpTextDir *= -1.0;

        OdGeVector3d perpTextDir = m_perpTextDir;
        if ( !OdPositive(perpTextDir.y) )
          perpTextDir *= -1.0;

        double ang = perpTextDir.angleTo(OdGeVector3d::kYAxis);
        double h = textHeight()/2.0;
        double w = textWidth()/2.0;
        distance += w*sin(ang) + h*cos(ang);
        // TODO: for just == 1 & 2
      }

      m_textPosition += m_perpTextDir * distance;
      m_bNeedCalcTextIntersection = false;

      if ( m_bUseRotate && !m_bTextHor)
      {
        return;
      }
      addUnderline();
    }
    break;

    //      case 3: // Places the dimension text to conform to Japanese Industrial Standards (JIS).
    //              // In left dimention line
    //        {
    //        }
    //        break;
  }
} // End adjustTextByVert

// Calc text position and direction outside dimension ext lines.

void OdAngularRecomputorEngine::textToOutside(double asz, double angle)
{
  // Horizontal justify
  OdGePoint3d p;
  OdGeVector3d v;
  OdGeVector3d dirToText;
  v = m_dirToMiddleArcPt;
  if ( m_just == 0 || m_just == 2 )
  {
    v.rotateBy(-angle-m_angle/2.0, OdGeVector3d::kZAxis);
    dirToText = v.crossProduct(OdGeVector3d::kZAxis).normalize();
  } else
  {
    v.rotateBy(angle+m_angle/2.0, OdGeVector3d::kZAxis);
    dirToText = v.crossProduct(-OdGeVector3d::kZAxis).normalize();
  }

  v.normalize();
  m_textPosition = m_centerDimPtArc + v*m_radiusDimArc;
  p = m_textPosition;
  m_textPosition += dirToText*(textWidth(false)/2.0 + m_gap);

  m_textDirection = v.crossProduct(OdGeVector3d::kZAxis).normalize();
  m_perpTextDir = v;
  correctTextDir();

  if ( !m_bUseRotate && !m_bTextHor && m_tad > 0 )
  {
    adjustTextByVert();
    // line
    double distance = textWidth(false) + 2.0*m_gap;
    m_textLinePt1 = p;
    // m_textLinePt2 = p + v*distance;
    m_textLinePt2 = p + dirToText*distance;
    m_bTextHasLine = true;
    return;
  }

  if ( m_bUseRotate || m_bTextHor )
  {
    m_textLinePt1 = p;
    m_bTextHasLine = true;
    OdGeVector3d xVec = m_xAxis;
    OdGeVector3d vec = getMiddleVector();
    if ( m_just == 1 )
    {
      if ( vec.y > 0.0 )
        xVec *= -1.0;
    } else
    {
      if ( vec.y < 0.0 )
        xVec *= -1.0;
    }

    m_textPosition = p + xVec * (textWidth(false)/2.0 + m_gap + asz);
    m_textLinePt2 = p + xVec*asz;

    if ( /*OdZero(m_dTextRot)*/ m_bTextHor && m_tad > 0 )
    {
      // horizontal text
      double distance = 0.0;
      if ( m_isBox ) 
        distance += m_gap;
      distance += textHeight(false)/2.0 + m_gap;
      m_textPosition += m_yAxis*distance;
      m_textLinePt2 += xVec*(textWidth(false) + 2.0*m_gap);
    }
  }
} // End textAndArrowToOuside

void OdAngularRecomputorEngine::textOnlyToOutside()
{
  if ( m_just > 2 )
    return;

  textToOutside(odmax(m_asz1,m_asz2), chordAngle(m_asz)); // m_Arrow1Angle);
}

void OdAngularRecomputorEngine::textAndArrowToOutside()
{
  if ( m_just > 2 )
    return;

  textToOutside(odmax(m_asz1,m_asz2), chordAngle(m_asz)*2.0 ); //m_Arrow1Angle*2.0);
} // End textAndArrowToOuside()

void OdAngularRecomputorEngine::prepareDimArc()
{
  if ( !m_bArrow1Inside )
    m_dimArc1EndAngle = m_extLine1Angle;
  else
    m_dimArc1EndAngle = m_extLine1Angle+m_Arrow1Angle;

  if ( !m_bArrow2Inside )
    m_dimArc2EndAngle = m_extLine2Angle;
  else
    m_dimArc2EndAngle = m_extLine2Angle-m_Arrow2Angle;
}

void OdAngularRecomputorEngine::addTailDimLineArc(double asz, bool soxd)
{
  if ( !m_bArrowsInside )
  {
    if ( soxd ) 
    {
      if ( !m_bTextInside && !m_bUserDefPos )
      {
        if (m_just == 0 || m_just == 2)
        {
          if ( !m_bArrow1Inside )
          {
            m_addedArrow1Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
                                  m_radiusDimArc, m_extLine1Angle-chordAngle(asz)*2.0, m_extLine1Angle);
            m_bAddedArrow1Arc = true;
          }
        } else if ( m_just == 1 )
        {
          if ( !m_bArrow2Inside )
          {
            m_addedArrow2Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
                                  m_radiusDimArc, m_extLine2Angle, m_extLine2Angle+chordAngle(asz)*2.0);
            m_bAddedArrow2Arc = true;
          }
        }
      }
    } else 
    {
      if ( OdZero(m_asz1) && OdZero(m_asz2) )
      {
        return;
      }
      if ( !m_bArrow1Inside )
      {
        m_addedArrow1Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
                              m_radiusDimArc, m_extLine1Angle-m_Arrow1Angle-chordAngle(asz), m_extLine1Angle-m_Arrow1Angle);
        m_bAddedArrow1Arc = true;
      }
      if ( !m_bArrow2Inside )
      {
        m_addedArrow2Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
                              m_radiusDimArc, m_extLine2Angle+m_Arrow1Angle, m_extLine2Angle+chordAngle(asz)+m_Arrow1Angle);
        m_bAddedArrow2Arc = true;
      }
    }
  } else if ( !m_bTextInside && !m_bUserDefPos )
  {
    if (m_just == 0 || m_just == 2)
    {
      m_addedArrow1Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
                            m_radiusDimArc, m_extLine1Angle-chordAngle(asz), m_extLine1Angle);
      m_bAddedArrow1Arc = true;
    } else if ( m_just == 1 )
    {
      m_addedArrow2Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
                            m_radiusDimArc, m_extLine2Angle, m_extLine2Angle+chordAngle(asz));
      m_bAddedArrow2Arc = true;
    }
  } 
  if ( m_bArrow1Flip && !m_bArrow1Inside && OdNonZero(m_Arrow1Angle) )
  {
    m_addedArrow1Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
      m_radiusDimArc, m_extLine1Angle-m_Arrow1Angle-chordAngle(asz), m_extLine1Angle-m_Arrow1Angle);
    m_bAddedArrow1Arc = true;
  }
  if ( m_bArrow2Flip && !m_bArrow2Inside && OdNonZero(m_Arrow2Angle) )
  {
    m_addedArrow2Arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), 
      m_radiusDimArc, m_extLine2Angle+m_Arrow2Angle, m_extLine2Angle+chordAngle(asz)+m_Arrow2Angle);
    m_bAddedArrow2Arc = true;
  }
}

void OdAngularRecomputorEngine::calcTextAngles()
{
  if ( m_dimLines == 2 )
  {
    m_StartTextAngle = m_dimArc2StartAngle;
    m_EndTextAngle = m_dimArc1StartAngle;
  } else
  {
    double ang = OdGeVector3d::kXAxis.angleTo(m_textPosition - m_centerDimPtArc, OdGeVector3d::kZAxis);
    double hrAng = chordAngle(textWidth(false)/2.0);
    m_gapAngle = chordAngle(m_gap);
    m_StartTextAngle = ang + m_gapAngle + hrAng;
    m_EndTextAngle = ang - m_gapAngle - hrAng;;
  }
}

void OdAngularRecomputorEngine::adjustUserDefText0(double /*asz*/)
{
  // justify with dim line
  addGapToTextExtents();
  // TODO: check in/out text
  if ( m_dimTih ) // m_textPosition == m_centerDimPtArc )
    m_textDirection = m_xAxis;
  else
    m_textDirection = (m_textPosition - m_centerDimPtArc).crossProduct(OdGeVector3d::kZAxis).normalize();

  if ( m_tad == 2 )
  {
    m_perpTextDir = m_textDirection.perpVector();
    if ( m_textDirection.x < 0.0 )
      m_textDirection *= -1.0;
  } else
  {
    if ( m_textDirection.x < 0.0 )
      m_textDirection *= -1.0;
    m_perpTextDir = m_textDirection.perpVector();
  }

  if ( m_tad > 0 && !m_bUseRotate && !m_bTextHor )
  {
    double halfTextH = textHeight()/2.0;
    if ( m_isBox )
      halfTextH += m_gap;
    m_textPosition += m_perpTextDir*halfTextH;
    m_bNeedCalcTextIntersection = false; 
  }
  // if ( m_tad > 0 && !m_bUseRotate )
  //   m_bNeedCalcTextIntersection = false;
  OdGeVector3d v = m_textPosition - m_centerDimPtArc;
  double ang = OdGeVector3d::kXAxis.angleTo(v, OdGeVector3d::kZAxis);
  if ( ang > m_extLine1Angle && ang < m_extLine2Angle )
  {
    m_bTextInside = true;
  }
}

void OdAngularRecomputorEngine::adjustUserDefText1(double asz, double /*exe*/)
{
  // no justify   add leader
  if ( m_textExtents.isValidExtents() )
  {
    addGapToTextExtents();
  }
  m_bNeedCalcTextIntersection = false;
  m_textDirection = OdGeVector3d::kXAxis;
  m_bLeader = true;
  OdGeVector3d v = m_textPosition - m_centerDimPtArc;
  double delta = ::fabs(m_radiusDimArc - v.length());
  if ( delta < textHeight() ) 
  {
    m_bLeader = false;
    return;
  }

  OdGePoint3d minPt = m_textPosition;
  OdGePoint3d maxPt = m_textPosition;
  if ( m_textExtents.isValidExtents() )
  {
    minPt = m_textExtentsWithGap.minPoint();
    maxPt = m_textExtentsWithGap.maxPoint();
  } 

  if ( m_tad > 1 && !m_bUseRotate && !m_bTextHor )
  {
    m_ldrPt1.x = maxPt.x - m_gap;
    m_ldrPt1.y = minPt.y;
    m_ldrPt2.x = minPt.x;
  } else
  {
    m_ldrPt1.y = m_textPosition.y;
    if ( m_textPosition.x < m_middleArcPt.x )
    {
      m_ldrPt1.x = maxPt.x;
      m_ldrPt2.x = m_ldrPt1.x + asz;
    } else
    {
      m_ldrPt1.x = minPt.x;
      m_ldrPt2.x = m_ldrPt1.x - asz;
    }
  }
  m_ldrPt2.y = m_ldrPt1.y;

  m_ldrPt3 = m_middleArcPt;
}

void OdAngularRecomputorEngine::adjustUserDefText2(double /*exe*/)
{
  // justify with dim line  no add leader
  addGapToTextExtents();

  if ( m_dimTih || m_textPosition == m_centerDimPtArc )
  {
    m_textDirection = m_xAxis;
    m_bTextHor = true;
  }
  else
    m_textDirection = (m_textPosition - m_centerDimPtArc).crossProduct(OdGeVector3d::kZAxis).normalize();

  if ( !m_bUseRotate && !m_bTextHor )
  {
    if ( m_textDirection.x < 0.0 )
      m_textDirection *= -1.0;
  }
}

void OdAngularRecomputorEngine::calcDimExtLineDir()
{
}

// Input: 
//   m_xLine1Pt, m_xLine2Pt,  m_centerPtArc
// Result: 
//   m_xLine1Dir, m_xLine2Dir
//   m_centerDimPtArc, m_radiusDimArc

void OdAngularRecomputorEngine::testDimExtLineOrder()
{
  calcDimExtLineDir();
  OdGeVector3d pointOnArcDir = m_arcPt - m_centerPtArc;

  double tol(1.0e-10);
  if(!OdZero(m_xLine1Dir.angleTo(pointOnArcDir, OdGeVector3d::kZAxis) +
    pointOnArcDir.angleTo(m_xLine2Dir, OdGeVector3d::kZAxis) -
    m_xLine1Dir.angleTo(m_xLine2Dir, OdGeVector3d::kZAxis), tol))
  {
    std::swap(m_xLine1Pt, m_xLine2Pt);
    std::swap(m_xLine1Dir, m_xLine2Dir);
  }
  m_xLine1Dir.normalize();
  m_xLine2Dir.normalize();
  m_sourceArcRadius = m_centerPtArc.distanceTo(m_xLine1Pt);
  m_radiusDimArc = m_centerPtArc.distanceTo(m_arcPt);
}

void OdAngularRecomputorEngine::calcExtLinePoints()
{
}

//  Input:
//    m_xLine1PtEnd, m_xLine2PtEnd, m_centerPtArc, m_centerDimPtArc, m_radiusDimArc,
//    m_textExtentsWithGap
//  Output:
//    m_dimArc1StartAngle, m_dimArc2StartAngle
void OdAngularRecomputorEngine::intersectTextBoxWithDimArc(bool bAddArcSym) // double gap, 
{
  if ( !m_bNeedCalcTextIntersection )
  {
    m_dimArc1StartAngle = m_extLine2Angle;
    m_dimArc2StartAngle = m_extLine1Angle;
    m_dimLines = 1;
    return;
  }

  addGapToTextExtents(bAddArcSym);

  OdGeCircArc2d cicle;

  cicle.set(OdGePoint2d(m_centerDimPtArc.x,m_centerDimPtArc.y), m_radiusDimArc, m_extLine1Angle, m_extLine2Angle);
  int num = 0;
  OdGePoint2d p1, p2;
  OdGeLineSeg2d line;
  OdGePoint3d minPt = m_textExtentsWithGap.minPoint();
  OdGePoint3d maxPt = m_textExtentsWithGap.maxPoint();
  OdGeMatrix2d matr = OdGeMatrix2d::rotation(m_dTextRot, OdGePoint2d(m_textPosition.x, m_textPosition.y));

  OdGePoint3dArray intersections;
  line.set(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(minPt.x, maxPt.y));
#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("----- intersectTextBoxWithDimArc START\n");
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
#endif
  line.transformBy(matr);
#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
#endif
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
  line.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, maxPt.y));
  line.transformBy(matr);
#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
#endif
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
  line.set(OdGePoint2d(maxPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
  line.transformBy(matr);
#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
#endif
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
  line.set(OdGePoint2d(maxPt.x, minPt.y), OdGePoint2d(minPt.x, minPt.y));
  line.transformBy(matr);
#ifdef DEBUG_BUILD_ARC_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
  ODA_TRACE0("----- intersectTextBoxWithDimArc END\n");
#endif
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
  if ( intersections.size() == 2 )
  {
    OdGeVector3d dimLineStart1VectorAny = (intersections[0] - m_centerDimPtArc).normalize();
    OdGeVector3d dimLineStart2VectorAny = (intersections[1] - m_centerDimPtArc).normalize();
    double dist1 = 0.0, dist2 = 0.0;
    if(dimLineStart1VectorAny.crossProduct(dimLineStart2VectorAny).z < 0.0)
    {
      m_dimArc1StartAngle = OdGeVector3d::kXAxis.angleTo(dimLineStart2VectorAny, OdGeVector3d::kZAxis); 
      m_dimArc2StartAngle = OdGeVector3d::kXAxis.angleTo(dimLineStart1VectorAny, OdGeVector3d::kZAxis); 
      dist1 = m_xLine1PtEnd.distanceTo(intersections[1]);
      dist2 = m_xLine2PtEnd.distanceTo(intersections[0]);
    }
    else
    {
      m_dimArc1StartAngle = OdGeVector3d::kXAxis.angleTo(dimLineStart1VectorAny, OdGeVector3d::kZAxis); 
      m_dimArc2StartAngle = OdGeVector3d::kXAxis.angleTo(dimLineStart2VectorAny, OdGeVector3d::kZAxis); 
      dist1 = m_xLine1PtEnd.distanceTo(intersections[0]);
      dist2 = m_xLine2PtEnd.distanceTo(intersections[1]);
    }
    if ( dist1 < m_asz1 || dist2 < m_asz2)
    {
      m_bArrowsInside = false;
    }
  } else
  {
    // TODO: ...
    m_dimArc1StartAngle = m_extLine2Angle;
    m_dimArc2StartAngle = m_extLine1Angle;
    m_dimLines = 1;
  }
}
// Input:
//    m_Arrow1Angle, m_Arrow2Angle, m_centerDimPtArc, m_angle
//    m_xLine1PtEnd, m_xLine2PtEnd
// Output:
//    m_isFitTextAndArrows, m_isFitArrowsWithoutText, m_isFitTextWithoutArrows
// 
void OdAngularRecomputorEngine::fitTextAndArrows(bool bText, bool bArrow, bool bWithGap)
{
  m_isFitTextAndArrows = false;
  m_isFitArrowsWithoutText = false;
  m_isFitTextWithoutArrows = false;
  m_gapAngle = chordAngle(m_gap);
  double gapAngle = 0.0;
  if ( bWithGap ) 
  {
    gapAngle = m_gapAngle;
  }
  addGapToTextExtents();

  if ( bArrow && bText )
  {
    /* if ( m_angle > OdaToRadian(200) )
    {
    double arrowsAngle = m_Arrow1Angle+m_Arrow1Angle+m_Arrow2Angle+m_Arrow2Angle;

    double arrowsAngle1 = m_Arrow1Angle+m_Arrow2Angle+gapAngle+gapAngle;

    double extTextX = m_textExtentsWithGap.maxPoint().x - m_textExtentsWithGap.minPoint().x;
    double textArcEngle = extTextX/m_radiusDimArc;
    if ( arrowsAngle + textArcEngle < m_angle )
    {
    m_isFitTextAndArrows = true;
    m_isFitArrowsWithoutText = true;
    m_isFitTextWithoutArrows = true;
    return;
    } else
    {
    m_isFitTextAndArrows = false;
    } 
    } else */
    {
      if ( m_angle > m_Arrow1Angle + m_Arrow2Angle + 2*gapAngle )
      {
        OdGePoint3d dimArcPt1 = m_xLine1PtEnd;
        dimArcPt1.transformBy(OdGeMatrix3d::rotation(m_Arrow1Angle + gapAngle, OdGeVector3d::kZAxis, m_centerDimPtArc));
        OdGePoint3d dimArcPt2 = m_xLine2PtEnd;
        dimArcPt2.transformBy(OdGeMatrix3d::rotation(-(m_Arrow2Angle + gapAngle), OdGeVector3d::kZAxis, m_centerDimPtArc));
        if ( isTextIn(dimArcPt1, dimArcPt2) )
        {
          m_isFitTextAndArrows = true;
          m_isFitArrowsWithoutText = true;
          m_isFitTextWithoutArrows = true;
          return;
        } 
      }
    }
  }

  if ( bArrow )
  {
    // gap does't influence
    if ( m_Arrow1Angle + m_Arrow2Angle < m_angle )
      m_isFitArrowsWithoutText = true;
  }

  if ( bText )
  {
    /* if ( m_angle > OdaToRadian(200) )
    {
    // m_isFitTextWithoutArrows = true; // ???
    double extTextX = m_textExtentsWithGap.maxPoint().x - m_textExtentsWithGap.minPoint().x;
    double textArcEngle = extTextX/m_radiusDimArc;
    if ( textArcEngle < m_angle )
    m_isFitTextWithoutArrows = true; // ???
    } else */
    {
      OdGePoint3d dimArcPt1 = m_xLine1PtEnd;
      OdGePoint3d dimArcPt2 = m_xLine2PtEnd;

      // added gap into account
      dimArcPt1.transformBy(OdGeMatrix3d::rotation(gapAngle, OdGeVector3d::kZAxis, m_centerDimPtArc));
      dimArcPt2.transformBy(OdGeMatrix3d::rotation(-gapAngle, OdGeVector3d::kZAxis, m_centerDimPtArc));

      if ( isTextIn(dimArcPt1, dimArcPt2) )
      {
        m_isFitTextWithoutArrows = true;
      }

      // if ( 2.0 * m_asz + 2.0 * m_gap > widthText )
      //  m_isFitTextWithoutArrowsForBest = false;
      // else
      m_isFitTextWithoutArrowsForBest = true;

    }
  }
} // fitTextAndArrowsStep1

void OdAngularRecomputorEngine::adjustTextByHor(double dimExe)
{
  double halfTextLngth = textWidth()/2.0;
  switch(m_just) 
  {
  case 0: // Centered along the dimension line between the extension lines
    m_textPosition = m_middleArcPt;
    m_perpTextDir = (m_textPosition - m_centerDimPtArc).normalize();
    m_textDirection = (m_textPosition - m_centerDimPtArc).crossProduct(OdGeVector3d::kZAxis).normalize();
    break;
  case 1: // Next to the first extension line
  case 2: // Next to the second extension line 
    {
      double angle = 0.0;
      if ( m_bUseRotate || m_bTextHor )
        angle = m_Arrow2Angle + m_gap*2.0/m_radiusDimArc + chordAngle(widthRotText()/2.0);
      else
        angle = m_Arrow2Angle + m_gap*2.0/m_radiusDimArc + chordAngle(halfTextLngth);

      if ( m_tad > 0 && !m_bUseRotate && !m_bTextHor )
      {
        angle -= chordAngle(m_gap*2.0); // +
      }
      if ( m_just == 1 )
      {
        m_textPosition = m_xLine2PtEnd;
        angle = -angle;
      } else
      {
        m_textPosition = m_xLine1PtEnd;
      }
      m_textPosition.rotateBy(angle, OdGeVector3d::kZAxis, m_centerDimPtArc);
      m_perpTextDir = (m_textPosition - m_centerDimPtArc).normalize();
      m_textDirection = (m_textPosition - m_centerDimPtArc).crossProduct(OdGeVector3d::kZAxis).normalize();
    }
    break;
  case 3: // Above and aligned with the first extension line
  case 4: // Above and aligned with the second extension line
    {
      OdGeVector3d v;  // = m_xLine1PtEnd - m_centerDimPtArc;
      OdGeVector3d v1; // = m_xLine1Pt - m_centerDimPtArc;

      if ( m_just == 3 )
      {
        v = m_xLine1PtEnd - m_centerDimPtArc;
        v1 = m_xLine1Pt - m_centerDimPtArc;
        m_textPosition = m_xLine1PtEnd;
      } else
      {
        v = m_xLine2PtEnd - m_centerDimPtArc;
        v1 = m_xLine2Pt - m_centerDimPtArc;
        m_textPosition = m_xLine2PtEnd;
      }
      v.normalize();
      v *= (dimExe + halfTextLngth);
      m_perpTextDir = v.crossProduct(OdGeVector3d::kZAxis).normalize();
      if ( v1.length() > m_radiusDimArc )
        v *= -1.0;
      m_textPosition += v; 
      m_textDirection = v;
      m_dimLines = 1;
    }
    break;
  }

  m_textDirection.normalize();
  correctTextDir();
} // End  adjustTextByHor

//////////////////////////////////////////////////////
// Od3PointAngularRecomputorEngine start implementation
//////////////////////////////////////////////////////

void Od3PointAngularRecomputorEngine::calcDimExtLineDir()
{
  m_xLine1Dir = m_xLine1Pt - m_centerPtArc;
  OdGeVector3d pointOnArcDir = m_arcPt - m_centerPtArc;
  m_xLine2Dir = m_xLine2Pt - m_centerPtArc;

  double zeroTol(1e-10); //OdGeContext::gZeroTol.equalPoint());
  if ( m_xLine1Dir.isZeroLength(zeroTol) )
  {
    if ( !m_xLine2Dir.isZeroLength(zeroTol) && !pointOnArcDir.isZeroLength(zeroTol) )
    {
      double ang = m_xLine2Dir.angleTo(pointOnArcDir, OdGeVector3d::kZAxis);
      double ang2 = m_xLine2Dir.angleTo(pointOnArcDir, -OdGeVector3d::kZAxis);
      OdGeVector3d refVec = OdGeVector3d::kZAxis;
      if ( ang2 < ang)
      {
        ang = ang2;
        refVec = -refVec;
      }
      ang *= 3.0;
      m_xLine1Dir = m_xLine2Dir;
      m_xLine1Dir.rotateBy(ang, refVec); //OdGeVector3d::kZAxis);
    } else
    {
      // ODA_ASSERT(0);
      m_xLine1Dir = OdGeVector3d::kYAxis;
      m_xLine2Dir = OdGeVector3d::kYAxis;
    }
  } else  if ( m_xLine2Dir.isZeroLength(zeroTol) )
  {
    if ( !m_xLine1Dir.isZeroLength(zeroTol) && !pointOnArcDir.isZeroLength(zeroTol) )
    {
      double ang = m_xLine1Dir.angleTo(pointOnArcDir, OdGeVector3d::kZAxis);
      double ang2 = m_xLine1Dir.angleTo(pointOnArcDir, -OdGeVector3d::kZAxis);
      OdGeVector3d refVec = OdGeVector3d::kZAxis;
      if ( ang2 < ang)
      {
        ang = ang2;
        refVec = -refVec;
      }
      ang *= 3.0/2.0;
      m_xLine2Dir = m_xLine1Dir;
      m_xLine2Dir.rotateBy(ang, refVec); //OdGeVector3d::kZAxis);
    } else
    {
      // ODA_ASSERT(0);
      m_xLine1Dir = OdGeVector3d::kYAxis;
      m_xLine2Dir = OdGeVector3d::kYAxis;
    }
  }
  m_xLine1Dir.normalize();
  m_xLine2Dir.normalize();
}

void Od3PointAngularRecomputorEngine::adjustTextByHor(double dimExe)
{
  addGapToTextExtents();
  OdAngularRecomputorEngine::adjustTextByHor(dimExe);
}
// Input:
//   m_xLine1Pt, m_xLine2Pt
//   m_xLine1Dir, m_xLine2Dir
//   m_centerPtArc, m_arcPt
// Result:
//   m_xLine1PtEnd, m_xLine2PtEnd
//   m_arrow1Pt, m_arrow2Pt
//   m_middleArcPt

void Od3PointAngularRecomputorEngine::calcExtLinePoints()
{
  testDimExtLineOrder();
  // calc ref vector
  // Calculate extention line
  OdGeVector3d v1 = m_xLine1Dir;
  OdGeVector3d v2 = m_xLine2Dir;

  // v1 or v2 may be zero len
  m_angle = v1.angleTo(v2, OdGeVector3d::kZAxis);

  // double distArcToDim = m_radiusDimArc - m_sourceArcRadius;

  m_centerDimPtArc = m_centerPtArc;

  v1.normalize();
  v2.normalize();
  OdGeVector3d v = v1 + v2;
  if ( v.isZeroLength() )
    v = (m_xLine1Pt - m_xLine2Pt).perpVector();

  m_xLine1PtEnd = m_centerDimPtArc + v1*m_radiusDimArc;
  m_xLine2PtEnd = m_centerDimPtArc + v2*m_radiusDimArc;

  v.normalize();
  if ( m_angle > OdaPI )
    v *= -1.0;
  m_middleArcPt = m_centerDimPtArc + v * m_radiusDimArc;

  m_arrow1Pt = m_xLine1PtEnd;
  m_arrow2Pt = m_xLine2PtEnd;
}

double Od3PointAngularRecomputorEngine::measurement()
{
  testDimExtLineOrder();
  m_angle = m_xLine1Dir.angleTo(m_xLine2Dir, OdGeVector3d::kZAxis);
  return m_angle;
}

//////////////////////////////////////////////////////
// OdArcLenRecomputorEngine start implementation
//////////////////////////////////////////////////////

void OdArcLenRecomputorEngine::adjustTextByHor(double dimExe)
{
  bool bAddArcSym = true;

  if ( m_just == 3 || m_just == 4 )
    bAddArcSym = false;
  addGapToTextExtents(bAddArcSym);

  double halfTextLngth = textWidth()/2.0;
  if ( m_arcSym == 0 && isInspection() )
  {
    halfTextLngth += textHeight(false)*0.9;
  }
  if ( m_just == 1 || m_just == 2 )
  {
    double angle = 0.0;
    if ( m_bUseRotate || m_bTextHor )
      angle = m_Arrow2Angle + m_gap*2.0/m_radiusDimArc + chordAngle(widthRotText()/2.0);
    else
      angle = m_Arrow2Angle + m_gap*2.0/m_radiusDimArc + chordAngle(halfTextLngth);

    if ( m_tad > 0 && !m_bUseRotate && !m_bTextHor )
    {
      angle -= chordAngle(m_gap*2.0);
    }
    if ( m_just == 1 )
    {
      m_textPosition = m_xLine2PtEnd;
      angle = -angle;
    } else
    {
      m_textPosition = m_xLine1PtEnd;
    }
    m_textPosition.rotateBy(angle, OdGeVector3d::kZAxis, m_centerDimPtArc);
    m_perpTextDir = (m_textPosition - m_centerDimPtArc).normalize();
    m_textDirection = (m_textPosition - m_centerDimPtArc).crossProduct(OdGeVector3d::kZAxis).normalize();

    m_textDirection.normalize();
    correctTextDir();
    return;
  }

  OdAngularRecomputorEngine::adjustTextByHor(dimExe);
} // End  adjustTextByHor

void OdArcLenRecomputorEngine::addGapToTextExtents(bool bAddArcSym)
{
  // Input:
  //    m_textExtents, m_arcInText, there are horizontal text extents
  // Output:
  //    m_textExtentsWithGap
  // 
  if ( isInspection() )
  {
    OdAngularRecomputorEngine::addGapToTextExtents();
    return;
  }

  OdGePoint3d minPt = m_textExtents.minPoint();
  OdGePoint3d maxPt = m_textExtents.maxPoint();

  if ( bAddArcSym )
  {
    if ( m_arcSym == 0 )
    {
      minPt.x = m_arcInText.endPoint().x; //??? m_arcInText.center().x;
    } else if ( m_arcSym == 1 )
    {
      maxPt.y = m_arcInText.startPoint().y; 
    } 
  }
  minPt.x -= m_gap;
  minPt.y -= m_gap;
  maxPt.x += m_gap;
  maxPt.y += m_gap;
  m_textExtentsWithGap.set(minPt, maxPt);
}

double OdArcLenRecomputorEngine::measurement()
{
  testDimExtLineOrder();
  m_angle = m_xLine1Dir.angleTo(m_xLine2Dir, OdGeVector3d::kZAxis);
  return m_sourceArcRadius*m_angle;
}

void OdArcLenRecomputorEngine::calcExtLinePoints()
{
  testDimExtLineOrder();
  // calc ref vector
  // Calculate extention line
  OdGeVector3d v1 = m_xLine1Dir;
  OdGeVector3d v2 = m_xLine2Dir;

  // v1 or v2 may be zero len
  m_angle = v1.angleTo(v2, OdGeVector3d::kZAxis);
  double distArcToDim = m_radiusDimArc - m_sourceArcRadius;
  m_centerDimPtArc = m_centerPtArc;

  OdGeVector3d v = v1 + v2;
  if ( v.isZeroLength() )
    v = (m_xLine1Pt - m_xLine2Pt).perpVector();

  v.normalize();

  if ( m_angle < OdaPI2 )
  {
    OdGeCircArc2d cicle;

    OdGePoint2d p1, p2;
    int numInt;
    if ( distArcToDim < 0.0 )
    {
      OdGePoint3d p = m_centerPtArc - v; 
      OdGePoint3dArray intersect; 

      cicle.set(m_arcPt.convert2d(), m_sourceArcRadius);
      OdGeLine2d line;
      line.set(OdGePoint2d(m_centerPtArc.x, m_centerPtArc.y), OdGePoint2d(p.x, p.y));

      cicle.intersectWith(line,numInt, p1, p2);

      if ( m_centerPtArc.convert2d().distanceTo(p1) < m_centerPtArc.convert2d().distanceTo(p2) ) 
        m_centerDimPtArc.set(p1.x, p1.y, 0.0);
      else
        m_centerDimPtArc.set(p2.x, p2.y, 0.0);

      m_radiusDimArc = m_sourceArcRadius;
      distArcToDim = m_centerDimPtArc.distanceTo(m_centerPtArc);
    }
    v *= distArcToDim;

    m_xLine1PtEnd = m_xLine1Pt + v;
    m_xLine2PtEnd = m_xLine2Pt + v;

    cicle.set(m_centerDimPtArc.convert2d(), m_radiusDimArc);
    OdGeLine2d line;

    line.set(OdGePoint2d(m_xLine1Pt.x, m_xLine1Pt.y), OdGePoint2d(m_xLine1PtEnd.x, m_xLine1PtEnd.y));
    if ( cicle.intersectWith(line, numInt, p1, p2) )
    {
      if ( m_xLine1PtEnd.convert2d().distanceTo(p1) < m_xLine1PtEnd.convert2d().distanceTo(p2) ) 
        m_xLine1PtEnd.set(p1.x, p1.y, 0.0);
      else
        m_xLine1PtEnd.set(p2.x, p2.y, 0.0);
    }
    line.set(OdGePoint2d(m_xLine2Pt.x, m_xLine2Pt.y), OdGePoint2d(m_xLine2PtEnd.x, m_xLine2PtEnd.y));
    if ( cicle.intersectWith(line, numInt, p1, p2) )
    {
      if ( m_xLine2PtEnd.convert2d().distanceTo(p1) < m_xLine2PtEnd.convert2d().distanceTo(p2) ) 
        m_xLine2PtEnd.set(p1.x, p1.y, 0.0);
      else
        m_xLine2PtEnd.set(p2.x, p2.y, 0.0);
    }
    m_xLine1Dir = m_xLine1PtEnd - m_centerDimPtArc;
    m_xLine2Dir = m_xLine2PtEnd - m_centerDimPtArc;
    m_angle = m_xLine1Dir.angleTo(m_xLine2Dir, OdGeVector3d::kZAxis);

    m_middleArcPt = m_xLine1PtEnd;
    m_middleArcPt.rotateBy(m_angle/2.0, OdGeVector3d::kZAxis, m_centerDimPtArc);
  } else
  {
    m_xLine1PtEnd = m_centerDimPtArc + v1*m_radiusDimArc;
    m_xLine2PtEnd = m_centerDimPtArc + v2*m_radiusDimArc;

    v.normalize();
    if ( m_angle > OdaPI )
      v *= -1.0;
    m_middleArcPt = m_centerDimPtArc + v * m_radiusDimArc;
  }
  m_arrow1Pt = m_xLine1PtEnd;
  m_arrow2Pt = m_xLine2PtEnd;

  m_xLine1Dir.normalize();
  m_xLine2Dir.normalize();
}

OdGeVector3d OdArcLenRecomputorEngine::getMiddleVector()
{
  if ( m_angle < OdaPI2 )
  {
    OdGeVector3d vec = m_xLine1PtEnd - m_xLine1Pt;
    double d = vec.angleTo(m_dirToMiddleArcPt, OdGeVector3d::kZAxis);
    if ( d > OdaPI )
      d = Oda2PI - d;
    if ( d > OdaPI2 )
      vec *= -1.0;
    // vec.normalize();
    return vec;
  } else
  {
    return m_dirToMiddleArcPt;
  }
}

////
//  Input:
//    TextPosition, TextExtension(for horizontal text) 
//    Calculation with TextDirection = kXAxis
//  Result:
//    m_radiusArcSym, m_centerArcSym, m_startArcSym, m_endArcSym

void OdArcLenRecomputorEngine::applyArcParam()
{
  // If text empty no arc need? (possible it test need)
  if ( m_arcSym == 2 )
    return;

  OdGePoint3d minPt = m_textExtents.minPoint();
  OdGePoint3d maxPt = m_textExtents.maxPoint();
  double startArcSym = 0.0;
  double endArcSym = 0.0;
  OdGePoint2d centerArcSym; 
  double radiusArcSym = 0.0;

  double height = textHeight(false);
  double width = textWidth(false);
  if ( isInspection() )
    width -= height; 
  if ( m_arcSym == 0 )
  {
    // Arc radius = (text extent Y) * 4.0/5.0
    double extY5 = height*0.2; 
    if ( isInspection()  )
      extY5 = height*0.7;
    radiusArcSym = height*0.8;
    centerArcSym.x = m_textPosition.x - width/2.0 - extY5 - radiusArcSym;
    centerArcSym.y = m_textPosition.y - height/2.0 + (height - radiusArcSym)/2.0;
    // total angel 180
    startArcSym = 0;
    endArcSym = OdaPI;
  } else if ( m_arcSym == 1 )
  {
    // Arc radius = (text extent Y) * 1.0875 
    radiusArcSym = height*1.0875;
    // center X = text position X
    centerArcSym.x = m_textPosition.x;
    // center Y = ( text position Y ) + ( extent Y )/80.0;
    centerArcSym.y = m_textPosition.y + height/80.0;
    // Start 0.809784 End 2.33181
    // Total angle 87.205638
    // TODO: need to be corrected
    startArcSym = 0.809784;
    endArcSym = 2.33181;
  }
  m_arcInText.set(centerArcSym, radiusArcSym, startArcSym, endArcSym);
}

void OdArcLenRecomputorEngine::fitArcParam(OdDbMTextPtr pText)
{
  applyArcParam();
  OdGeMatrix2d matr;
  matr.setToRotation(pText->rotation(), OdGePoint2d(m_textPosition.x, m_textPosition.y) );
  // matr.setToRotation(pText->rotation(), OdGePoint2d(pText->location().x, pText->location().y) );
  m_arcInText.transformBy(matr);
}

void OdArcLenRecomputorEngine::calcTextExtents(OdDbMTextPtr& pText, bool bForce)
{
  OdAngularRecomputorEngine::calcTextExtents(pText, bForce);
  applyArcParam();
}

void OdArcLenRecomputorEngine::calcInspectionExtents(OdDbMTextPtr& pLabel, OdDbMTextPtr& pRate, bool bForce)
{
  OdRecomputorEngine::calcInspectionExtents(pLabel, pRate, bForce);
  applyArcParam();
}

void OdArcLenRecomputorEngine::calcDimExtLineDir()
{
  m_xLine1Dir = m_xLine1Pt - m_centerPtArc;
  OdGeVector3d pointOnArcDir = m_arcPt - m_centerPtArc;
  m_xLine2Dir = m_xLine2Pt - m_centerPtArc;

  double zeroTol(1e-10); //OdGeContext::gZeroTol.equalPoint());
  if ( m_xLine1Dir.isZeroLength(zeroTol) )
  {
    if ( !m_xLine2Dir.isZeroLength(zeroTol) && !pointOnArcDir.isZeroLength(zeroTol) )
    {
      double ang = m_xLine2Dir.angleTo(pointOnArcDir, OdGeVector3d::kZAxis);
      double ang2 = m_xLine2Dir.angleTo(pointOnArcDir, -OdGeVector3d::kZAxis);
      OdGeVector3d refVec = OdGeVector3d::kZAxis;
      if ( ang2 < ang)
      {
        ang = ang2;
        refVec = -refVec;
      }
      ang *= 3.0;
      m_xLine1Dir = m_xLine2Dir;
      m_xLine1Dir.rotateBy(ang, refVec); //OdGeVector3d::kZAxis);
    } else
    {
      // ODA_ASSERT(0);
      m_xLine1Dir = OdGeVector3d::kYAxis;
      m_xLine2Dir = OdGeVector3d::kYAxis;
    }
  } else  if ( m_xLine2Dir.isZeroLength(zeroTol) )
  {
    if ( !m_xLine1Dir.isZeroLength(zeroTol) && !pointOnArcDir.isZeroLength(zeroTol) )
    {
      double ang = m_xLine1Dir.angleTo(pointOnArcDir, OdGeVector3d::kZAxis);
      double ang2 = m_xLine1Dir.angleTo(pointOnArcDir, -OdGeVector3d::kZAxis);
      OdGeVector3d refVec = OdGeVector3d::kZAxis;
      if ( ang2 < ang)
      {
        ang = ang2;
        refVec = -refVec;
      }
      ang *= 3.0;
      m_xLine2Dir = m_xLine1Dir;
      m_xLine2Dir.rotateBy(ang, refVec); //OdGeVector3d::kZAxis);
    } else
    {
      // ODA_ASSERT(0);
      m_xLine1Dir = OdGeVector3d::kYAxis;
      m_xLine2Dir = OdGeVector3d::kYAxis;
    }
  }
  m_xLine1Dir.normalize();
  m_xLine2Dir.normalize();
}

bool OdArcLenRecomputorEngine::isUserDefTextInside()
{
  OdGeLineSeg2d ln;
  ln.set(OdGePoint2d(m_arrow1Pt.x, m_arrow1Pt.y), OdGePoint2d(m_arrow2Pt.x, m_arrow2Pt.y));
  OdGePoint2d p1, p2;
  OdGeLineSeg2d line;

  double extX2 = textWidth()/2.0; 
  double extY2 = textHeight()/2.0;
  OdGePoint3d minPt;
  OdGePoint3d maxPt;

  minPt.x = m_textPosition.x - extX2;
  maxPt.x = m_textPosition.x + extX2;

  minPt.y = m_textPosition.y - extY2;
  maxPt.y = m_textPosition.y + extY2;

  OdGeMatrix2d matr = OdGeMatrix2d::rotation(m_dTextRot, OdGePoint2d(m_textPosition.x, m_textPosition.y));

  line.set(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(minPt.x, maxPt.y));
  line.transformBy(matr);
  if ( ln.intersectWith(line, p1) )
  {
    return true;
  }
  line.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, maxPt.y));
  line.transformBy(matr);
  if ( ln.intersectWith(line, p1))
  {
    return true;
  }
  line.set(OdGePoint2d(maxPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
  line.transformBy(matr);
  if ( ln.intersectWith(line, p1))
  {
    return true;
  }
  line.set(OdGePoint2d(maxPt.x, minPt.y), OdGePoint2d(minPt.x, minPt.y));
  line.transformBy(matr);
  if ( ln.intersectWith(line, p1) )
  {
    return true;
  }
  // check diagonal
  line.set(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(maxPt.x, maxPt.y));
  line.transformBy(matr);
  if ( ln.intersectWith(line, p1) )
  {
    return true;
  }
  line.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
  line.transformBy(matr);
  if ( ln.intersectWith(line, p1) )
  {
    return true;
  }
  return false;
}

void OdArcLenRecomputorEngine::checkArrowsPlace()
{
  bool bInside = false;
  if ( m_angle < OdaPI2 )
  {
    double ang = chordAngle( m_arrow1Pt.distanceTo(m_arrow2Pt));
    if ( m_Arrow1Angle + m_Arrow2Angle < ang )
    {
      bInside = true;
    }
  } else if ( m_Arrow1Angle + m_Arrow2Angle < m_angle )
  {
    bInside = true;
  }
  if ( !bInside && isUserDefTextInside() )
  {
    m_bArrowsInside = !m_bArrowsInside;
  }
}

void Od2LineAngularRecomputorEngine::calcDimExtLineDir()
{
  calcDimPoints();
}

void Od2LineAngularRecomputorEngine::calcDimPoints()
{
  m_centerDimPtArc = m_centerPtArc;
  OdGeVector3d pointOnArcDir = m_arcPt - m_centerPtArc;

  double tol(1.0e-6);
  m_xLine1Dir = m_xLine1Pt - m_centerPtArc;
  if ( m_xLine1Dir.isZeroLength(tol) )
    m_xLine1Dir = m_xLine1PtEnd - m_centerPtArc;

  m_xLine2Dir = m_xLine2Pt - m_centerPtArc;
  if ( m_xLine2Dir.isZeroLength(tol) )
    m_xLine2Dir = m_xLine2PtEnd - m_centerPtArc;

  if(!OdZero(m_xLine1Dir.angleTo(pointOnArcDir, OdGeVector3d::kZAxis) +
    pointOnArcDir.angleTo(m_xLine2Dir, OdGeVector3d::kZAxis) -
    m_xLine1Dir.angleTo(m_xLine2Dir, OdGeVector3d::kZAxis), tol))
  {
    std::swap(m_xLine1Pt, m_xLine2Pt);
    std::swap(m_xLine1PtEnd, m_xLine2PtEnd);
    std::swap(m_xLine1Dir, m_xLine2Dir);
    std::swap(m_extLine1Angle, m_extLine2Angle);
    std::swap(m_bExtLine1Enabled, m_bExtLine2Enabled);
    m_bSwaped = true;
  }

  OdGeVector3d v1 = m_xLine1Dir;
  OdGeVector3d v2 = m_xLine2Dir;

  // v1 or v2 may be zero len
  m_angle = v1.angleTo(v2, OdGeVector3d::kZAxis);

  v1.normalize();
  v2.normalize();
  OdGeVector3d v = v1 + v2;
  if ( v.isZeroLength() )
  {
    // CR 3547
    v = (m_xLine1Pt - m_xLine2Pt);
    if ( v.isZeroLength() )
      v = (m_xLine1PtEnd - m_xLine2PtEnd);
    v = v.perpVector();
  }

  v.normalize();
  // if ( m_angle > OdaPI )
  //  v *= -1.0;
  m_middleArcPt = m_centerDimPtArc + v * m_radiusDimArc;

  m_arrow1Pt = m_xLine1PtEnd;
  m_arrow2Pt = m_xLine2PtEnd;

  m_sourceArcRadius = m_radiusDimArc/2.0; // it need for check  if ( m_radiusDimArc <= m_sourceArcRadius  ) for ArcDimension
  m_xLine1Dir.normalize();
  m_xLine2Dir.normalize();

  if ( m_middleArcPt == m_centerDimPtArc )
  {
    m_dirToMiddleArcPt = m_xLine1Dir + m_xLine2Dir;
    m_dirToMiddleArcPt *= -1.0;
  }
  else
    m_dirToMiddleArcPt = m_middleArcPt - m_centerDimPtArc;
  m_dirToMiddleArcPt.normalize();
}

void Od2LineAngularRecomputorEngine::adjustTextByHor(double dimExe)
{
  addGapToTextExtents();
  double halfTextLngth = textWidth()/2.0;
  switch(m_just) 
  {
  case 3: // Above and aligned with the first extension line
  case 4: // Above and aligned with the second extension line
    {
      OdGeVector3d v;  
      OdGeVector3d v1;

      if ( m_just == 3 )
      {
        v = m_xLine1PtEnd - m_centerDimPtArc;
        // v1 = m_xLine1Pt - m_centerDimPtArc;
        v1 = m_xLine2Pt - m_centerDimPtArc;
        m_textPosition = m_xLine1PtEnd;
      } else
      {
        v = m_xLine2PtEnd - m_centerDimPtArc;
        v1 = m_xLine2Pt - m_centerDimPtArc;
        m_textPosition = m_xLine2PtEnd;
      }
      v.normalize();
      v *= (dimExe + halfTextLngth);
      m_perpTextDir = v.crossProduct(OdGeVector3d::kZAxis).normalize();
      if ( m_just == 3 && v1.length() > m_radiusDimArc )
        v *= -1.0;
      m_textPosition += v; 
      m_textDirection = v;
      m_dimLines = 1;

      m_textDirection.normalize();
      correctTextDir();
    }
    break;
  default:
    Od3PointAngularRecomputorEngine::adjustTextByHor(dimExe);
  }

} // End  adjustTextByHor

// Od2LineAngularRecomputorEngine::measurement()

bool OdAngularRecomputorEngine::makeExtensionArc(OdUInt16 dimtmove)
{
  if ( !m_bUserDefPos || m_bLeader )
    return false;

  double dirRot = 1.0;
  double distance = 0.0;
  OdGePoint3d startPoint;
  OdGeVector3d v = m_textPosition - m_centerDimPtArc;
  v.normalize();
  OdGePoint3d arcPoint = m_centerDimPtArc + v*m_radiusDimArc;

  if ( dimtmove == 0 )
  {
    OdGeCircArc2d arc;
    arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), m_radiusDimArc, m_extLine1Angle, m_extLine2Angle);
    OdGePoint2d point(arcPoint.x, arcPoint.y);
    if ( arc.isOn(point) )
    {
      return false;
    }
    if ( arcPoint.distanceTo(m_arrow1Pt) < arcPoint.distanceTo(m_arrow2Pt) )
    { 
      distance = arcPoint.distanceTo(m_arrow1Pt);
      m_dStartExtAng = m_extLine1Angle;
      dirRot = -1.0;
      startPoint = m_arrow1Pt;
    } else
    {
      distance = arcPoint.distanceTo(m_arrow2Pt);
      m_dStartExtAng = m_extLine2Angle;
      startPoint = m_arrow2Pt;
    }
    double halfW = textWidth()/2.0;
    m_dEndExtAng = m_dStartExtAng + dirRot*chordAngle(distance) - dirRot*chordAngle(halfW);
    if ( m_tad > 0 && !m_bUseRotate && distance > halfW )
    {
      if ( m_dStartExtAng > m_dEndExtAng )
      {
        std::swap(m_dStartExtAng, m_dEndExtAng);
      }
      return true;
    }
  } else
  {
    double halfH = textHeight()/2.0;
    double delta = ::fabs(m_radiusDimArc - m_textPosition.distanceTo(m_centerDimPtArc));
    if ( delta > halfH ) 
      return false;

    OdGeCircArc2d arc;
    arc.set(OdGePoint2d(m_centerDimPtArc.x, m_centerDimPtArc.y), m_radiusDimArc, m_extLine1Angle, m_extLine2Angle);
    OdGePoint2d point(arcPoint.x, arcPoint.y);
    if ( arc.isOn(point) )
    {
      return false;
    }

    if ( arcPoint.distanceTo(m_arrow1Pt) < arcPoint.distanceTo(m_arrow2Pt) )
    { 
      distance = arcPoint.distanceTo(m_arrow1Pt);
      m_dStartExtAng = m_extLine1Angle;
      dirRot = -1.0;
      startPoint = m_arrow1Pt;
    } else
    {
      distance = arcPoint.distanceTo(m_arrow2Pt);
      m_dStartExtAng = m_extLine2Angle;
      startPoint = m_arrow2Pt;
    }

    m_dEndExtAng = m_dStartExtAng + dirRot*chordAngle(distance) - dirRot*chordAngle(halfH);
  }

  OdGePoint3dArray intersections;
  intersectTextBoxWithExtDimArc(intersections);
  if ( intersections.size() > 1 )
  {
    if ( startPoint.distanceTo(intersections[0]) < startPoint.distanceTo(intersections[1]) )
    {
      distance = startPoint.distanceTo(intersections[0]);
    } else
    {
      distance = startPoint.distanceTo(intersections[1]);
    }
    if ( intersections.size() > 2 ) 
    {
      if ( startPoint.distanceTo(intersections[2]) < distance )
      {
        distance = startPoint.distanceTo(intersections[2]);
      }
    }
    m_dEndExtAng = m_dStartExtAng + dirRot*chordAngle(distance);
  } else
  {
    return false;
  }

  if ( m_dStartExtAng > m_dEndExtAng )
  {
    std::swap(m_dStartExtAng, m_dEndExtAng);
  }
  return true;
}

void OdAngularRecomputorEngine::intersectTextBoxWithExtDimArc(OdGePoint3dArray& intersections)
{
  addGapToTextExtents();
  OdGeCircArc2d cicle;
  cicle.set(OdGePoint2d(m_centerDimPtArc.x,m_centerDimPtArc.y), m_radiusDimArc, m_extLine2Angle, m_extLine1Angle );
  int num = 0;
  OdGePoint2d p1, p2;
  OdGeLineSeg2d line;

  double extX2 = textWidth()/2.0; 
  double extY2 = textHeight()/2.0;
  OdGePoint3d minPt;
  OdGePoint3d maxPt;

  minPt.x = m_textPosition.x - extX2;
  maxPt.x = m_textPosition.x + extX2;

  minPt.y = m_textPosition.y - extY2;
  maxPt.y = m_textPosition.y + extY2;

  OdGeMatrix2d matr = OdGeMatrix2d::rotation(m_dTextRot, OdGePoint2d(m_textPosition.x, m_textPosition.y));

  // OdGePoint3dArray intersections;
  line.set(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(minPt.x, maxPt.y));
  line.transformBy(matr);
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
  line.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, maxPt.y));
  line.transformBy(matr);
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
  line.set(OdGePoint2d(maxPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
  line.transformBy(matr);
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
  line.set(OdGePoint2d(maxPt.x, minPt.y), OdGePoint2d(minPt.x, minPt.y));
  line.transformBy(matr);
  if ( cicle.intersectWith(line, num, p1, p2) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
    if ( num == 2 )
      intersections.push_back(OdGePoint3d(p2.x, p2.y, 0.0));
  }
}
