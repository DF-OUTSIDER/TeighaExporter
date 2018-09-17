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
#include "OdAlignedRecomputorEngine.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

#ifdef _DEBUG
// #define DEBUG_ALIGNED_DIM
#endif

#define ZERO_TOLERANCE  1.0e-10

void OdAlignedRecomputorEngine::tranformBy(const OdGeMatrix3d& matr)
{
  m_xLine1Pt.transformBy(matr);
  m_xLine2Pt.transformBy(matr);
  m_dimLineDefPt.transformBy(matr);

  zCoord = m_xLine1Pt.z;
  m_dimLineDefPt.z = m_xLine1Pt.z = m_xLine2Pt.z = 0.0;
}

void OdAlignedRecomputorEngine::preprocessData(double oblique, bool bDimRot, double rotAngle )
{
  OdGeVector3d zAxis = OdGeVector3d::kZAxis; // m_xAxis.crossProduct(m_yAxis);
  if (bDimRot )
  {
    m_dirExtLine = OdGeVector3d::kYAxis; // m_yAxis;
    if(OdZero(oblique))
    {
      m_dirExtLine.rotateBy(rotAngle, zAxis);
    }
    else
    {
      m_dirExtLine.rotateBy(rotAngle-(OdaPI2-oblique), zAxis);
    }

    m_dirDimLine = OdGeVector3d::kXAxis; //m_xAxis; 
    m_dirDimLine.rotateBy(rotAngle, zAxis);

    OdGeLine3d perpLine(m_xLine2Pt, m_dirExtLine);
    OdGeLine3d dimLine(m_dimLineDefPt, m_dirDimLine); 

    dimLine.intersectWith(perpLine, m_dimLineDefPt);

    perpLine.set(m_xLine1Pt, m_dirExtLine);
    dimLine.intersectWith(perpLine, m_xLine1PtEnd);
    m_xLine2PtEnd = m_dimLineDefPt;

    if ( m_xLine1PtEnd != m_xLine2PtEnd )
    {
      m_dirDimLine = m_xLine2PtEnd - m_xLine1PtEnd;
    } 
    if ( m_xLine1PtEnd != m_xLine1Pt )
    {
      m_dirExtLine = m_xLine1PtEnd - m_xLine1Pt;
    }

  } else
  {
    if ( m_xLine1Pt != m_xLine2Pt )
    {
      m_dirDimLine = m_xLine2Pt - m_xLine1Pt;
      m_dirExtLine = OdGeVector3d::kZAxis.crossProduct(m_dirDimLine);
      if(OdNonZero(oblique))
      {
        m_dirExtLine.rotateBy(oblique-OdaPI2, zAxis);
      }

      OdGeLine3d perpLine(m_xLine2Pt, m_dirExtLine);
      OdGeLine3d dimLine(m_dimLineDefPt, m_dirDimLine); 
      perpLine.intersectWith(dimLine, m_dimLineDefPt);
    } else
    {
      m_dirDimLine = OdGeVector3d::kXAxis; // m_xAxis;
      m_dirExtLine = OdGeVector3d::kYAxis; // m_yAxis;
      if ( OdNonZero(oblique) )
      {
        double dOblique = OdaPI2 - oblique;
        m_dirExtLine.rotateBy(-dOblique, zAxis);
      }
    }
    m_xLine2PtEnd = m_dimLineDefPt;
    m_xLine1PtEnd = m_xLine1Pt + (m_xLine2PtEnd.asVector() - m_xLine2Pt.asVector());
    if ( m_xLine1PtEnd != m_xLine1Pt )
      m_dirExtLine = m_xLine1PtEnd - m_xLine1Pt;
  }

  m_arrow1Pt = m_xLine1PtEnd;
  m_arrow2Pt = m_xLine2PtEnd;

  m_middleDimLinePt.x = (m_xLine2PtEnd.x + m_xLine1PtEnd.x)/2.0;
  m_middleDimLinePt.y = (m_xLine2PtEnd.y + m_xLine1PtEnd.y)/2.0;

  m_dirDimLine.normalize();
  m_dirExtLine.normalize();

  // m_bDimLineIsHor = m_dirDimLine.isCodirectionalTo(OdGeVector3d::kXAxis) || m_dirDimLine.isCodirectionalTo(-OdGeVector3d::kXAxis);
  m_bDimLineIsHor = m_dirDimLine.isCodirectionalTo(m_xAxis) || m_dirDimLine.isCodirectionalTo(-m_xAxis);

  /*
  if ( m_xLine2PtEnd == m_xLine1PtEnd )
  {
    m_dirDimLine = m_xAxis;
    if ( bDimRot )
    {
      OdGeVector3d zAxis = m_xAxis.crossProduct(m_yAxis);
      m_dirDimLine.rotateBy(rotAngle, zAxis);
    }
  }
  else
    m_dirDimLine = (m_xLine2PtEnd - m_xLine1PtEnd);
  m_dirDimLine.normalize();

  if ( m_xLine1PtEnd == m_xLine1Pt )
  {
    m_dirExtLine = OdGeVector3d::kZAxis.crossProduct(m_dirDimLine);
    if ( OdNonZero(oblique) )
    {
      double dOblique = OdaPI2 - oblique;
      m_dirExtLine.rotateBy(-dOblique, OdGeVector3d::kZAxis);
    }
  }
  else
    m_dirExtLine = m_xLine1PtEnd - m_xLine1Pt;
  */
}

double OdAlignedRecomputorEngine::verticalDistToText()
{
  double distance = 0.0;
  if ( m_tad == 0 && OdNonZero(m_dimTvp, 1.e-5) )
  {
    // if ( !m_bUseRotate )
    distance = textHeight(false)*m_dimTvp;
    return distance;
  } 

  if ( m_tad == 0 )
    return distance;

  if ( OdNonZero(m_dTextRot) )
    return distance;

  if ( m_bUserDefPos )
  {
    if ( m_bTextHor && m_bTextInside )
    {
      bool bNeedCalcDist = ((m_tad == 1 || m_tad == 4 ) && m_bDimLineIsHor);
      if ( m_tad > 1 || bNeedCalcDist )
        distance = distanceToHorText();
    } else
    {
      distance = textHeight()/2.0;
      if ( m_isBox ) 
        distance += m_gap;
    }
  } else
  {
    if ( m_bTextHor && m_just < 3 )
      distance = distanceToHorText();
    else
    {
      distance = textHeight()/2.0;
      if ( m_isBox ) 
        distance += m_gap;
    }
  }
  return distance;
}

void OdAlignedRecomputorEngine::adjustTextAndArrowsPlace(OdUInt16 tfit, bool tix)
{
  if ( m_dimTih && !m_bDimLineIsHor && m_just < 3 )
     m_bUseRotate = true;

  if ( m_just > 2 )
  {
    fitTextAndArrows(false, true);
    if ( !m_isFitArrowsWithoutText )
      m_bArrowsInside = false;
    return;
  }

  bool withGap = true;

  if ( m_tad == 0 && OdNonZero(m_dimTvp, 1.e-5) )
  {
    if ( !OdNegative( m_dimTvp - 0.7) )
      withGap = false;
  }

  if ( m_tad == 2 || m_tad == 3 || ( ( m_tad == 1 || m_tad == 4 ) && !m_bUseRotate ) )
    withGap = false;

  if ( OdNonZero(m_dTextRot) || (m_tad < 2 && m_dimTih && !m_bDimLineIsHor ) )
  {
    if ( m_dimTvp <= 0.7 )
      withGap = true;
  }

  if ( withGap )
  {
    fitTextAndArrows(true, true);
  } else
  {
    fitTextAndArrows(true, true, false);
    m_isFitTextAndArrows = m_isFitArrowsWithoutText & m_isFitTextWithoutArrows;
  }

  if ( tix )
  {
    m_bTextInside = true;
    m_bArrowsInside = m_isFitTextAndArrows;
  } else
  {
    applyDimTFit(tfit);
  }

  if ( OdZero(m_dTextRot) )
  {
    if ( m_bTextInside )
    {
      if ( m_dimTih )
        m_bTextHor = true;
      else
 	      m_bUseRotate = false;
    }

    if ( !m_bTextInside )
    {
      if ( m_dimToh )
        m_bTextHor = true;
 	    else 
 	      m_bUseRotate = false;
    }
  }
}

void OdAlignedRecomputorEngine::adjustTextLocation(OdInt16 dimMove, double dimExe)
{
  lengthTextOnDimLine();

  if ( m_bTextInside )
  {
    adjustTextByHor(dimExe);
    bool needVertCorrect = true;
    if ( m_tad == 0 )
    {
      if ( OdZero(m_dimTvp, 1.e-5) )
      {
        needVertCorrect = false;
      } else
      {
        if ( !m_bDimLineIsHor && m_bUseRotate )
          needVertCorrect = false;
      }
    }
    if ( m_tad > 0 )
    {
      if ( m_bUseRotate && !m_bTextHor )
        needVertCorrect = false;
      if ( ( m_tad == 1 || m_tad == 4 ) && m_bTextHor && m_bUseRotate )
        needVertCorrect = false;
    }    
    if ( needVertCorrect )
      adjustTextByVert();
  
    if ( m_just > 2 )
       m_bTextInside = false;
    else 
    {    
      if ( m_tad == 0 )
      {
        if ( m_dimTvp <= 0.7 )
          m_bNeedCalcTextIntersection = true;
        else
          m_bNeedCalcTextIntersection = false;
      }
      if ( m_tad > 0 && m_just < 3 )
      {
        if ( m_bUseRotate && !needVertCorrect )
          m_bNeedCalcTextIntersection = true;
        else
          m_bNeedCalcTextIntersection = false;
      }
    }
    m_dimOcsTextPosition = m_textPosition;
  } else 
  {
    m_bNeedCalcTextIntersection = false;
    if ( m_just < 3 && ( dimMove == 1 || dimMove == 2 ) )
    {
      applyDimMoveForDefaultPos(dimMove, dimExe);
      if ( m_bTextHor )
        m_textDirection = m_xAxis;
    } else
    {
      if ( m_bArrowsInside )
        textAndArrowToOutside();
      else
        textOnlyToOutside();
      m_dimOcsTextPosition = m_textPosition;
    }  
  }
}

void OdAlignedRecomputorEngine::fitTextAndArrows(bool bText, bool bArrow, bool bWithGap)
{
  // m_isFitTextAndArrows = false;
  // m_isFitArrowsWithoutText = false;
  // m_isFitTextWithoutArrows = false;

  addGapToTextExtents();

  double gap = 0.0;
  if ( bWithGap ) 
  {
    gap = m_gap;
  }

  double widthText = textWidth();

  double distance = m_xLine2PtEnd.distanceTo(m_xLine1PtEnd);
  if ( bArrow && bText )
  {
    if ( m_bUseRotate )
      widthText = lengthTextOnDimLine();
    if ( m_asz1 + m_asz2 + widthText + gap*2.0 < distance  )
      m_isFitTextAndArrows = true;
    else
      m_bFitTextByHor = false;
  }

  if ( bArrow )
  {
    if ( m_asz1 + m_asz2 + m_gap*2.0 <= distance )
      m_isFitArrowsWithoutText = true;
  }

  if ( bText )
  {
    if ( m_bUseRotate )
      widthText = widthRotText();
    if ( widthText < distance )
    {
      m_isFitTextWithoutArrows = true;
      if ( 2.0 * m_asz + 2.0 * m_gap > widthText )
        m_isFitTextWithoutArrowsForBest = false;
      else
        m_isFitTextWithoutArrowsForBest = true;
    }
  }
}

void OdAlignedRecomputorEngine::correctTextDirection()
{
  OdGeVector3d vv = m_textDirection;
  OdGeMatrix3d matr;
  matr.setCoordSystem(OdGePoint3d::kOrigin, m_xAxis, m_yAxis, OdGeVector3d::kZAxis);
  matr.invert();
  vv.transformBy(matr);
  if ( OdNonZero(vv.x, 1.e-5) )
  {
    if ( vv.x < 0.0 )
      m_textDirection *= -1.0;
  } else
  {
    if ( vv.y < 0.0 )
      m_textDirection *= -1.0;
  }
}

void OdAlignedRecomputorEngine::adjustTextByHor(double dimExe)
{
  double halfTextLngth = textWidth()/2.0;
  OdUInt16 just = m_just;
  if ( !m_bFitTextByHor  && (m_just == 1 || m_just == 2) ) // !m_bArrowsInside
    just = 0;
  switch(just) 
  {
  case 0: // Centered along the dimension line between the extension lines
    if ( OdNonZero(m_asz1 - m_asz2) )
    {
      if ( OdZero(m_asz1) )
      {
        m_textPosition -= m_asz/2.0*m_dirDimLine;
      } else 
      {
        m_textPosition += m_asz/2.0*m_dirDimLine;
      }
    } else
    {
      m_textPosition = m_middleDimLinePt;
    }
    m_textDirection = m_dirDimLine;
    break;
  case 1: // Next to the first extension line
  case 2: // Next to the second extension line 
    {
      // TODO: if len text with gap greater than distance between arrows - text centred
      if ( m_bUseRotate )
        halfTextLngth = lengthTextOnDimLine()/2.0;

      OdGeVector3d v;

      if ( m_just == 1 )
      {
        v = m_dirDimLine;
        m_textPosition = m_xLine1PtEnd;
      } else
      {
        v = -m_dirDimLine;
        m_textPosition = m_xLine2PtEnd;
      }
      m_textDirection = m_dirDimLine;
      // v.normalize();
      if ( m_bSuppressDimLine && m_tad > 0 )
        m_textPosition += v*(m_asz+halfTextLngth);
      else
        m_textPosition += v*(m_asz+m_gap+halfTextLngth);
    }
    break;
  case 3: // Above and aligned with the first extension line
  case 4: // Above and aligned with the second extension line
    {
      OdGeVector3d v;  
      if ( m_just == 3 )
      {
        v = m_xLine1PtEnd - m_xLine1Pt;
        if ( OdZero(v.length()) )
          v = m_dirDimLine.perpVector();
        m_textPosition = m_xLine1PtEnd;
      } else
      {
        v = m_xLine2PtEnd - m_xLine2Pt;
        if ( OdZero(v.length()) )
          v = m_dirDimLine.perpVector();
        m_textPosition = m_xLine2PtEnd;
      }

      v.normalize();
      v *= (dimExe + halfTextLngth);
      m_textPosition += v; 
      m_textDirection = v;
      m_dimLines = 1;
    }
    break;
  }
  if ( m_bTextHor )
  {
    m_textDirection = m_xAxis;
  } else
  {
    correctTextDirection();
  }

  m_textDirection.normalize();
} // End  adjustTextByHor

void OdAlignedRecomputorEngine::adjustTextByVert()
{
  // Function isn't run in case text with rotation
  // OdUInt16 tad = m_tad;
  if ( m_bUseRotate && OdZero(m_dTextRot) && m_tad == 1  )
  {
    OdGeVector3d v = m_dirDimLine;
    if ( !v.isCodirectionalTo(m_textDirection) && !v.isCodirectionalTo(-m_textDirection) )
    {
      m_bNeedCalcTextIntersection = true;
      m_bNeedCheckTextFit = true;
      return;
    }
  }
  switch (m_tad)
  {
  case 0: // Centers the dimension text between the extension lines.
    // Already in place. Calculate intersect with arc is nedded.
    m_bNeedCalcTextIntersection = true;
    m_bNeedCheckTextFit = true;
    if ( OdNonZero(m_dimTvp, 1.e-5 ) ) // && !m_bUseRotate )
    {
      calcVerticalDirection();
      m_textPosition += m_perpTextDir * verticalDistToText();
      expandExtLine();
      if ( m_dimTvp > 0.7 || m_just > 2 )
      {
        m_bNeedCalcTextIntersection = false;
      }
    }
    break;
  case 1: // Places the dimension text above the dimension line except when the dimension line is 
    // not horizontal and text inside the extension lines is forced horizontal (DIMTIH = 1). 
    // The distance from the dimension line to the baseline of the lowest line of text is 
    // the current DIMGAP value.
    //
  case 4:
  case 3:
    {
      if ( m_bUseRotate && OdNonZero(m_dTextRot) && m_just > 2 )
      {
        return;
      }
      calcVerticalDirection();
      if ( m_tad == 4 )
        m_textPosition -= m_perpTextDir * verticalDistToText();
      else
        m_textPosition += m_perpTextDir * verticalDistToText();
      m_bNeedCalcTextIntersection = false;
      if ( m_bUseRotate && OdNonZero(m_dTextRot))
      {
        return;
      }
      expandExtLine();
      break;
    }
  case 2:     // Places the dimension text on the side of the dimension line farthest away from the 
    // defining points.
    {
      if ( m_bUseRotate && OdNonZero(m_dTextRot) && m_just > 2 )
      {
        return;
      }
      
      calcVerticalDirection();
      m_textPosition += m_perpTextDir * verticalDistToText();
      m_bNeedCalcTextIntersection = false;

      if ( m_bUseRotate && OdNonZero(m_dTextRot))
      {
        return;
      }
      expandExtLine();
    }
    break;

    //      case 3: // Places the dimension text to conform to Japanese Industrial Standards (JIS).
    //              // In left dimention line
    //        {
    //        }
    //        break;
  }

}

double OdAlignedRecomputorEngine::distanceToHorText()
{
  double distance;

  double ang = m_perpTextDir.angleTo(m_xAxis);
  double h, w;
  h = textHeight()/2.0;
  w = textWidth()/2.0;
  double angDiag = ::atan(h/w);
  double diag = ::sqrt(h*h+w*w);
  ang = ang > OdaPI2 ? OdaPI - ang : ang;
  ang -= angDiag; 
  distance = diag*::cos(ang);
  if ( m_isBox ) 
    distance += m_gap;
  return distance;
}

void OdAlignedRecomputorEngine::expandExtLine()
{
  // TODO: it need to be reversed in case m_tad == 2
  if ( m_bUseRotate )
    return;

  if ( m_just == 3 ) 
  {
    OdGeVector3d v  = m_xLine1PtEnd - m_xLine1Pt;
    if ( OdZero(v.length()) )
      v = m_dirDimLine.perpVector();
    v.normalize();
    m_xLine1PtEnd += v*textWidth();
  }
  else if ( m_just == 4 )
  {
    OdGeVector3d v = m_xLine2PtEnd - m_xLine2Pt;
    if ( OdZero(v.length()) )
      v = m_dirDimLine.perpVector();
    v.normalize();
    m_xLine2PtEnd += v*textWidth();
  }
}

void OdAlignedRecomputorEngine::calcDirectionArrows()
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

  if ( m_bArrow1Inside )
    m_dirArrow1 = m_dirDimLine;
  else
    m_dirArrow1 = -m_dirDimLine;

  if ( m_bArrow2Inside )
    m_dirArrow2 = -m_dirDimLine;
  else
    m_dirArrow2 = m_dirDimLine;

  if ( m_bArrow1Flip && OdZero(m_asz1) )
  {
    m_dirArrow1.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  }
  if ( m_bArrow2Flip && OdZero(m_asz2) )
  {
    m_dirArrow2.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  }
}

void OdAlignedRecomputorEngine::calcDimLinesPoints()
{
  OdGeVector3d v(m_dirDimLine);
  if ( m_dimLines == 2 )
  {
    if ( m_bArrow1Inside )
    {
      m_dimLineEnd1Pt = m_arrow1Pt + v*( m_asz1 > 0.0 ? m_asz : 0.0 );
    } else
    {
      m_dimLineEnd1Pt = m_arrow1Pt;
    }
    if ( m_bArrow2Inside )
    {
      m_dimLineEnd2Pt = m_arrow2Pt - v*( m_asz2 > 0.0 ? m_asz : 0.0 );
    } else
    {
      m_dimLineEnd2Pt = m_arrow2Pt;
    }
  } else if ( m_dimLines == 1 )
  {
    if ( m_bArrow1Inside )
    {
      m_dimLineEnd1Pt = m_arrow1Pt + v*( m_asz1 > 0.0 ? m_asz : 0.0 );
    } else
    {
      m_dimLineEnd1Pt = m_arrow1Pt;
    }
    if ( m_bArrow2Inside )
    {
      m_dimLineStart1Pt = m_arrow2Pt - v*( m_asz2 > 0.0 ? m_asz : 0.0 );
    } else
    {
      m_dimLineStart1Pt = m_arrow2Pt;
    }
    m_dimLineEnd2Pt = m_dimLineStart1Pt;
    m_dimLineStart2Pt = m_dimLineEnd1Pt;
  }
}

void OdAlignedRecomputorEngine::correctDimLinesPoints()
{
  if ( m_dimLines == 1 )
  {
    OdGeVector3d xDir = m_dirDimLine;

    OdGeVector3d yUnit = OdGeVector3d::kZAxis.crossProduct(xDir).normalize();
    OdGeMatrix3d cordSys;
    cordSys.setCoordSystem(m_middleDimLinePt, xDir, yUnit, OdGeVector3d::kZAxis);
    OdGeMatrix3d invCordSys = cordSys;
    invCordSys.invert();

    OdGePoint3d testPt = m_textPosition;
    testPt.transformBy(invCordSys);

    double dist = textWidth() / 2.0;

    m_dimLineStart1Pt.transformBy(invCordSys);
    m_dimLineStart1Pt.x = testPt.x - dist;
    m_dimLineStart1Pt.transformBy(cordSys);

    m_dimLineStart2Pt.transformBy(invCordSys);
    m_dimLineStart2Pt.x = testPt.x + dist;
    m_dimLineStart2Pt.transformBy(cordSys);

  }
}

void OdAlignedRecomputorEngine::addDimLineTails(bool soxd, double dle)
{
  bool bAsz1 = OdZero(m_asz1) | m_bArrow1Flip, bAsz2 = OdZero(m_asz2) | m_bArrow2Flip;

  if ( !m_bArrowsInside )
  {
    // if ( !soxd ) 
    // {
    if ( !m_bTextInside && !m_bUserDefPos )
    {
      if (m_just == 0 || m_just == 2)
      {
        OdGeVector3d v = -m_dirDimLine;
        m_tail1StartPt = m_arrow1Pt;
        if ( !bAsz1)
          m_tail1StartPt += v*m_asz;
        if (m_bArrow1Flip)
          m_tail1EndPt = m_tail1StartPt + v*2.0*m_asz;
        else
          m_tail1EndPt = m_tail1StartPt + v*m_asz;
        m_bTail1 = true;
        bAsz1 = false;
      } else if ( m_just == 1 )
      {
        OdGeVector3d v = m_dirDimLine;
        m_tail2StartPt = m_arrow2Pt;
        if ( !bAsz2)
          m_tail2StartPt += v*m_asz;
        m_tail2EndPt = m_tail2StartPt + v*m_asz;
        m_bTail2 = true;
        bAsz2 = false;
        bAsz1 = false; // ACAD bug !!!
      } else
      {
        OdGeVector3d v(m_dirDimLine);
        if ( OdNonZero(m_asz1) )
        {
          m_tail1StartPt = m_arrow1Pt - v*m_asz;
          m_tail1EndPt = m_tail1StartPt - v*m_asz;
          m_bTail1 = true;
        }
        if ( OdNonZero(m_asz2) )
        {
          m_tail2StartPt = m_arrow2Pt + v*m_asz;
          m_tail2EndPt = m_tail2StartPt + v*m_asz;
          m_bTail2 = true;
        }
      }
      // }
    } else 
    {
      // ????
      OdGeVector3d v(m_dirDimLine);
      if ( m_dimLines == 0 && !m_bLeader && !soxd )
      {
        if ( bAsz1 )
        {
          m_tail1StartPt = m_arrow1Pt;
          m_tail1EndPt = m_tail1StartPt - v*m_asz;
        } else
        {
          m_tail1StartPt = m_arrow1Pt - v*m_asz;
          m_tail1EndPt = m_tail1StartPt - v*m_asz;
        }
        m_bTail1 = true;
        if ( bAsz2 )
        {
          m_tail2StartPt = m_arrow2Pt;
          m_tail2EndPt = m_tail2StartPt + v*m_asz;
        } else
        {
          m_tail2StartPt = m_arrow2Pt + v*m_asz;
          m_tail2EndPt = m_tail2StartPt + v*m_asz;
        }
        m_bTail2 = true;
      } else if ( !(m_bTextInside && soxd && !m_bUserDefPos) )
      {
        bool bAsz11 = bAsz1, bAsz22 = bAsz2;
        if ( ( !bAsz11 || !bAsz22 ) && m_bUserDefPos && m_bLeader )
        {
          if ( !m_middleDimLinePt.isEqualTo(m_ldrPt3) )
          {
            // minimize amount of lines
            if ( m_textPosition.distanceTo(m_arrow1Pt) > m_textPosition.distanceTo(m_arrow2Pt) )
            {
              bAsz22 = true;
            } else
            {
              bAsz11 = true;
            }
          }
        }
        if ( !bAsz11 )
        {
          m_tail1StartPt = m_arrow1Pt - v*m_asz;
          m_tail1EndPt = m_tail1StartPt - v*m_asz;
          m_bTail1 = true;
        }
        if ( !bAsz22 )
        {
          m_tail2StartPt = m_arrow2Pt + v*m_asz;
          m_tail2EndPt = m_tail2StartPt + v*m_asz;
          m_bTail2 = true;
        }
      }
    }
  } else
  {
    // ???
  }
  // TODO: add to account result m_dimLines
  if ( OdNonZero(dle) && m_dimLines != 0 )
  {
    if ( m_bTextInside || m_just > 2 || m_bUserDefPos )
    {
      if ( bAsz1 )
      {
        m_dimLineEnd1Pt -= m_dirDimLine*dle;
      } 
      if ( bAsz2 )
      {
        m_dimLineEnd2Pt += m_dirDimLine*dle;
      } 
    } else
    {
      if (m_just == 0 || m_just == 2)
      {
        if ( bAsz1 )
        {
          m_dimLineEnd1Pt -= m_dirDimLine*dle;
        } 
      } else if ( m_just == 1 )
      {
        /*  ACAD bug
        if ( bAsz2 )
        {
          m_dimLineEnd2Pt += m_dirDimLine*dle;
        } 
        */
      }
    }
  }

  if ( m_bArrow1Flip && !m_bArrow1Inside && m_asz1 > 0.0 )
  {
    m_tail1StartPt = m_arrow1Pt -m_dirDimLine*m_asz;
    m_tail1EndPt = m_tail1StartPt -m_dirDimLine*m_asz;
    m_bTail1 = true;
  }
  if ( m_bArrow2Flip && !m_bArrow2Inside && m_asz2 > 0.0 )
  {
    m_tail2StartPt = m_arrow2Pt + m_dirDimLine*m_asz;
    m_tail2EndPt = m_tail2StartPt + m_dirDimLine*m_asz;
    m_bTail2 = true;
  }

  /* if ( !m_bTextInside && !m_bUserDefPos )
  {
  } */
}

OdUInt32 OdAlignedRecomputorEngine::getIntersectionPoints(OdGePoint3d& p1, OdGePoint3d& p2, bool useDimLine)
{
  double extX2 = textWidth()/2.0; // = (maxPt.x - minPt.x) / 2.0;
  double extY2 = textHeight()/2.0; // = (maxPt.y - minPt.y) / 2.0;
  OdGePoint3d minPt;
  OdGePoint3d maxPt;
  /*
  if ( m_bUseXText || m_bInspectionFrame )
  {
     extX2 = textWidth()/2.0;
     extY2 = textHeight()/2.0;
  } else
  {
    minPt = m_textExtentsWithGap.minPoint();
    maxPt = m_textExtentsWithGap.maxPoint();

    // ACAD uses aligned extents relative m_textPosition
    extX2 = (maxPt.x - minPt.x) / 2.0;
    extY2 = (maxPt.y - minPt.y) / 2.0;
  }
  */

  minPt.x = m_textPosition.x - extX2;
  maxPt.x = m_textPosition.x + extX2;

  minPt.y = m_textPosition.y - extY2;
  maxPt.y = m_textPosition.y + extY2;



  OdGeMatrix2d matr = OdGeMatrix2d::rotation(m_dTextRot, OdGePoint2d(m_textPosition.x, m_textPosition.y));

  OdGePoint3dArray intersections;

  OdGePoint3d pt1, pt2;

  OdGeVector3d v = m_dirDimLine;
  if ( useDimLine )
  {
    pt1 = m_arrow1Pt;
    pt2 = m_arrow2Pt;
  } else
  {
    pt1 = m_textPosition - v*(maxPt.x - minPt.x);
    pt2 = m_textPosition + v*(maxPt.x - minPt.x);
  }

  OdGeLineSeg2d dimLine1(OdGePoint2d(pt1.x, pt1.y), OdGePoint2d(pt2.x, pt2.y));

  OdGePoint2d intersecPt;

  OdGeLineSeg2d line(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(minPt.x, maxPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }

#ifdef DEBUG_ALIGNED_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);

  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", dimLine1.startPoint().x, dimLine1.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", dimLine1.endPoint().x, dimLine1.endPoint().y);
#endif

  line.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, maxPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }
#ifdef DEBUG_ALIGNED_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
#endif

  line.set(OdGePoint2d(maxPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }
#ifdef DEBUG_ALIGNED_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
#endif

  line.set(OdGePoint2d(maxPt.x, minPt.y), OdGePoint2d(minPt.x, minPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }
#ifdef DEBUG_ALIGNED_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line.startPoint().x, line.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line.endPoint().x, line.endPoint().y);
#endif

  if ( intersections.size() == 2 )
  {
    p1 = intersections[0];
    p2 = intersections[1];
    return 2;
  } else if ( intersections.size() == 1 )
  {
    p1 = intersections[0];
    return 1;
  } else if ( intersections.size() == 4 )
  {
    // exclude same points
    if ( intersections[0] == intersections[1] )
    {
      p1 = intersections[0];
      p2 = intersections[2];
    } else
    {
      p1 = intersections[0];
      p2 = intersections[1];
    }
    return 2;
  }
  return 0;
}

void OdAlignedRecomputorEngine::intersectDimLineWithText()
{
  if ( !m_bNeedCalcTextIntersection )
  {
    m_dimLines = 1;
    return;
  }

  bool bOscPos = false;
  OdGePoint3d txtPos = m_textPosition;
  if (m_bUserDefPos && m_dimMove != 2 && OdNonZero(m_dimTvp) && m_bUseRotate && m_tad == 0 && m_bTextInside)
  {
    m_textPosition = m_dimOcsTextPosition;
    bOscPos = true;
  }

  addGapToTextExtents();

  OdGePoint3d p1, p2;
  OdUInt32 numPt = getIntersectionPoints(p1, p2, true);

  if ( numPt == 2 )
  {
    if ( m_arrow1Pt.distanceTo(p1) < m_arrow1Pt.distanceTo(p2) )
    {
      m_dimLineStart1Pt = p1;
      m_dimLineStart2Pt = p2;
    } else
    {
      m_dimLineStart1Pt = p2;
      m_dimLineStart2Pt = p1;
    }
    m_dimLines = 2;
  } else
  {
    m_dimLines = 1;
  }
  if ( bOscPos )
    m_textPosition = txtPos;
}

void OdAlignedRecomputorEngine::textOnlyToOutside()
{
  if ( m_just > 2 )
    return;

  if ( m_dimToh && !m_bDimLineIsHor )
    rotTextToOutside();
  else
    textToOutside(odmax(m_asz1,m_asz2));
}

void OdAlignedRecomputorEngine::textAndArrowToOutside()
{
  if ( m_just > 2 )
    return;

  if ( m_dimToh && !m_bDimLineIsHor )
    rotTextToOutside();
  else
    textToOutside(odmax(m_asz1,m_asz2));
} // End textAndArrowToOuside()


void OdAlignedRecomputorEngine::rotTextToOutside()
{
  bool bIsTextRotated = OdNonZero(m_dTextRot);

  double ang = m_dirDimLine.angleTo(m_xAxis);
  OdGeVector3d horDir = m_xAxis;

  if ( ( OdZero(OdaPI2 - ang) && m_dirDimLine.y < 0.0 ) ||
    OdNegative(OdaPI2 - ang) )
  {
    ang = OdaPI - ang;
    horDir *= -1.0;
  }

  bool bLeader = false;
  double addedGap = 0.0;
  if ( ang > OdaPI/12.0 && !m_bSwapTextPos )
  {
    bLeader = true;
    if ( m_tad == 0 )
      addedGap = m_asz;
  }

  double widthText;
  double addedAsz = 0.0;
  if ( bIsTextRotated )
  {
    if ( bLeader )
    {
      m_bCalcTextLenOnDimLine = false;
      widthText = lengthTextOnXDir()/2.0;
      addedGap = m_asz;
    } else
      widthText = lengthTextOnDimLine()/2.0;
  }
  else
  {
     widthText = textWidth()/2.0;
  }
  if (!m_bArrowsInside)
    addedAsz = m_asz;

  OdGePoint3d point;
  if ( m_just == 0 || m_just == 2 )
  {
    if ( m_bSwapTextPos )
    {
      m_textPosition = m_arrow1Pt - m_dirDimLine*(m_asz + addedAsz);
      m_textPosition -= horDir*(widthText + addedGap);
    } else
    {
      m_textPosition = m_arrow2Pt + m_dirDimLine*(m_asz + addedAsz);
      m_textPosition += horDir*(widthText + addedGap);
    }
    m_tail2StartPt = m_arrow2Pt;
    if (!m_bArrowsInside)
    {
       m_tail2StartPt += m_dirDimLine*m_asz2;
    }
    m_tail2EndPt = m_tail2StartPt + m_dirDimLine*m_asz;
    m_bTail2 = true;
    point = m_tail2EndPt;
  } else
  {
    m_textPosition = m_arrow1Pt - m_dirDimLine*(m_asz + addedAsz);
    m_textPosition -= horDir*(widthText + addedGap);

    m_tail1StartPt = m_arrow1Pt;
    if (!m_bArrowsInside)
    {
      m_tail1StartPt -= m_dirDimLine*m_asz1;
    }
    m_tail1EndPt = m_tail1StartPt - m_dirDimLine*m_asz;
    m_bTail1 = true;
    point = m_tail1EndPt;
    horDir *= -1.0;
  }
  if ( bLeader )
  {
    m_textLinePt1 = point;
    if ( m_tad > 0 && !bIsTextRotated )
    {
      double distance = textWidth(false)+m_gap;
      m_textLinePt2 = m_textLinePt1 + horDir*distance;
    } else
    {
      m_textLinePt2 = m_textLinePt1 + horDir*m_asz;
    }
    m_bTextHasLine = true;
  }

  if ( m_tad > 0 && !bIsTextRotated)
  {
    double distance = verticalDistToText(); /* textHeight()/2.0;
    if ( m_isBox )
      distance += m_gap; */
    if ( m_tad == 4 )
      m_textPosition -= m_yAxis*distance;
    else
      m_textPosition += m_yAxis*distance;
  }
}

void OdAlignedRecomputorEngine::textToOutside(double asz)
{

  // Horizontal justify
  OdGePoint3d p;
  OdGeVector3d v;
  double addedAsz = 0.0;
  if (!m_bArrowsInside)
    addedAsz = m_asz;

  double widthText;
  if ( m_bUseRotate )
    widthText = lengthTextOnDimLine()/2.0;
  else
    widthText = textWidth()/2.0;

  double gapFromArrowPt = 0.0;
  double addToDistance = 0.0;
  if ( m_just == 0 || m_just == 2 )
  {
    if ( OdNonZero(m_asz2) && !m_bArrow2Flip )
      gapFromArrowPt = addedAsz;
    else if ( m_bArrow2Flip )
      addToDistance = addedAsz;
    if ( m_bSwapTextPos )
    {
      p = m_arrow1Pt;
      v = -1.0 * m_dirDimLine;
    } else
    {
      p = m_arrow2Pt;
      v = m_dirDimLine;
    }
    m_textPosition = p + v*(widthText + m_asz + addedAsz);
  } else
  {
    p = m_arrow1Pt;
    if ( OdNonZero(m_asz1) && !m_bArrow1Flip )
      gapFromArrowPt = addedAsz;
    else if ( m_bArrow1Flip )
      addToDistance = addedAsz;
    v = -m_dirDimLine;
    m_textPosition = p + v*(widthText + m_asz + addedAsz);
  }

  m_textDirection = v;
  correctTextDirection();

  if ( !m_bUseRotate && m_tad > 0 )
  {
    calcVerticalDirection();
    adjustTextByVert();
    // line
    double distance = textWidth() + m_asz + addToDistance;
    m_textLinePt1 = p + v*gapFromArrowPt;
    m_textLinePt2 = m_textLinePt1 + v*distance;
    m_bTextHasLine = true;
    return;
  }

  if ( m_bUseRotate || m_tad == 0 )
  {
    m_textLinePt1 = p;
    m_bTextHasLine = true;
    if ( m_just == 0 || m_just == 2 )
    {
      if ( !m_bArrowsInside )
        m_textLinePt1 += m_dirDimLine*asz;
      m_textLinePt2 = m_textLinePt1 + m_dirDimLine*m_asz;
    } else
    {
      if ( !m_bArrowsInside )
        m_textLinePt1 -= m_dirDimLine*asz;
      m_textLinePt2 = m_textLinePt1 - m_dirDimLine*m_asz;
    }
  }
} // End textAndArrowToOuside

double OdAlignedRecomputorEngine::lengthTextOnDimLine()
{
  if ( m_bCalcTextLenOnDimLine )
    return m_textLenOnDimLine;

  addGapToTextExtents();

  OdGePoint3d p1, p2;
  OdUInt32 numPt = getIntersectionPoints(p1, p2, false);

  if ( numPt == 2 )
  {
    m_textLenOnDimLine = p1.distanceTo(p2);
  } else
  {
    m_textLenOnDimLine = 0.0;
  }
  m_bCalcTextLenOnDimLine = true;
  return m_textLenOnDimLine;
}

double OdAlignedRecomputorEngine::lengthTextOnXDir()
{
  OdGeVector3d v = m_dirDimLine;
  m_dirDimLine = OdGeVector3d::kXAxis;
  double len = lengthTextOnDimLine();
  m_dirDimLine = v;
  return len;
}

void OdAlignedRecomputorEngine::correctExtLinePoints(const OdGePoint3d& oscTextPos)
{
  if ( m_bTextInside || ( !m_bTextInside && !(m_bTextHor || m_bUseRotate) ) )
  {
    OdGeLine3d extLine1(m_xLine1Pt, m_dirExtLine);
    OdGeLine3d textLine(oscTextPos, m_dirDimLine);
    OdGeLine3d extLine2(m_xLine2Pt, m_dirExtLine);

    OdGePoint3d pt1, pt2;
    textLine.intersectWith(extLine1, pt1);
    textLine.intersectWith(extLine2, pt2);
    m_arrow1Pt = m_xLine1PtEnd = pt1;
    m_arrow2Pt = m_xLine2PtEnd = pt2;
    m_dimOcsTextPosition = oscTextPos;
    if ( m_bTextInside )
      return;
  }

  OdGePoint3d textPt = oscTextPos;

  double halfTextWidth;
  if ( m_bUseRotate )
  {
    halfTextWidth = lengthTextOnXDir()/2.0;
  } else
  {
    halfTextWidth = textWidth()/2.0;
  }
  double addedAsz = 0.0;
  if ( m_tad == 0 &&  !m_bDimLineIsHor )
  {
    addedAsz = m_asz;
  }

  double distToText = m_asz;
  if ( !m_bArrowsInside )
    distToText *= 2.0;

  if ( m_bTextHor || m_bUseRotate )
  {
    OdGeLine3d extLine1(m_xLine1Pt, m_dirExtLine);
    OdGeLine3d textLine(textPt, m_dirDimLine);
    OdGePoint3d pt;
    extLine1.intersectWith(textLine, pt);
    OdGeVector3d vecToText = textPt - pt;

    OdGePoint3d controlPt;
    OdGeVector3d dirToControlPt = m_xAxis;
    if ( OdZero(vecToText.x) )
    {
      if ( vecToText.y >= 0.0 )
        dirToControlPt = -m_xAxis;
    } else
    {
      if ( vecToText.x > 0.0 )
        dirToControlPt = -m_xAxis;
    }

    if ( m_bTextHor )
    {
      controlPt = textPt + dirToControlPt*(halfTextWidth+addedAsz);
    } else
    {
      if ( m_dimToh )
      {
        controlPt = textPt + dirToControlPt*(halfTextWidth+addedAsz+m_asz);
      } else
      {
        return;
      }
    }
    textLine.set(controlPt, m_dirDimLine);
    OdGeLine3d extLine2(m_xLine2Pt, m_dirExtLine);

    OdGePoint3d pt1, pt2;
    textLine.intersectWith(extLine1, pt1);
    textLine.intersectWith(extLine2, pt2);
    m_arrow1Pt = m_xLine1PtEnd = pt1;
    m_arrow2Pt = m_xLine2PtEnd = pt2;

    OdGePoint3d dimPoint = m_arrow2Pt;
    double dir = 1.0;
    if ( textPt.distanceTo(m_arrow1Pt) < textPt.distanceTo(m_arrow2Pt) )
    {
      dimPoint = m_arrow1Pt;
      dir = -1.0;
    }

    if ( dimPoint.distanceTo(controlPt) < distToText 
      && !controlPt.isEqualTo(dimPoint)
      )
    {
      OdGeVector3d v = m_dirDimLine*dir;
      v.normalize();
      OdGeVector3d testV = textPt - dimPoint;
      m_dimOcsTextPosition = dimPoint + v*distToText;
      // if ( m_bDimLineIsHor )
      //  addedAsz = 0.0;
      if ( testV.x > 0.0 )
        m_dimOcsTextPosition += m_xAxis*(halfTextWidth+addedAsz);
      else
        m_dimOcsTextPosition -= m_xAxis*(halfTextWidth+addedAsz);
      m_textPosition = m_dimOcsTextPosition;
    }
  } else // text aligned
  {
    OdGePoint3d dimPoint = m_arrow2Pt;
    double dir = 1.0;
    if ( textPt.distanceTo(m_arrow1Pt) < textPt.distanceTo(m_arrow2Pt) )
    {
      dimPoint = m_arrow1Pt;
      dir = -1.0;
    }
    distToText += halfTextWidth;
    if ( dimPoint.distanceTo(textPt) < distToText )
    {
      OdGeVector3d v = textPt - dimPoint;
      if ( OdNonZero(v.length()) )
        v.normalize();
      else
        v = m_dirDimLine*dir;
      m_dimOcsTextPosition = dimPoint + v*distToText;
      m_textPosition = m_dimOcsTextPosition;
    }
  }
}

void OdAlignedRecomputorEngine::adjustUserDefText0(double asz)
{
  // justify with dim line
  addGapToTextExtents();

  OdGePoint3d oscTextPos = m_textPosition;
  m_dimOcsTextPosition = oscTextPos;
  m_textDirection = m_dirDimLine;
  
  testTextLocation0();

  if ( m_bTextHor )
  {
    m_textDirection = m_xAxis;
  } else
  {
    correctTextDirection();
  }

  m_textDirection.normalize();

  bool bNeedVertOffset = true;

  if ( m_bUseRotate || ( m_bTextInside && m_bTextHor && !m_bDimLineIsHor) )
  {
    bNeedVertOffset = false;
  } else if ( m_tad < 1 || m_bUseXText )
  {
    bNeedVertOffset = false;
  }
  if ( m_tad == 0 && OdNonZero(m_dimTvp, 1.e-5) )
    bNeedVertOffset = true;

  /* # 5291  Condition is not reproduced. Possible where is other situation.
  There is no such dimensions in test set
  else if ( m_tad == 1 && m_bTextHor && m_bTextInside )
  {
    bNeedVertOffset = false;
  } 
  */
  
  correctExtLinePoints(oscTextPos);
  oscTextPos = m_dimOcsTextPosition;
  if ( bNeedVertOffset )
  {
    calcVerticalDirection();
    double distance = verticalDistToText();
    if ( m_tad == 4 )
      m_textPosition -= m_perpTextDir*distance;
    else
      m_textPosition += m_perpTextDir*distance;
    m_bNeedCalcTextIntersection = false;
    m_dimLines = 1;
    if (m_tad == 0 && OdNonZero(m_dimTvp, 1.e-5))
      m_bNeedCalcTextIntersection = true;
  }

  // Add text line or leader
  if ( !m_bTextInside )
  {
    double halfTextWidth;
    if ( m_bUseRotate )
      halfTextWidth = lengthTextOnXDir()/2.0;
    else
      halfTextWidth = textWidth()/2.0;

    double addedAsz = 0.0;
    if ( m_tad == 0 )
      addedAsz = asz;

    bool b  = m_dirDimLine.isCodirectionalTo(m_textDirection) || m_dirDimLine.isCodirectionalTo(-m_textDirection);
    if ( ( !b &&  m_bTextHor ) || ( !m_bDimLineIsHor && m_bUseRotate && m_dimToh ) )
    {
      if ( m_tad > 0 && !m_bUseRotate )
      {
        m_bLeader = true;
        if ( oscTextPos.distanceTo(m_arrow1Pt) < oscTextPos.distanceTo(m_arrow2Pt) )
        {
          m_ldrPt3 = m_arrow1Pt;
        } else
        {
          m_ldrPt3 = m_arrow2Pt;
        }
        OdGeVector3d v = oscTextPos - m_ldrPt3;
        if ( v.x > 0.0 )
        {
          m_ldrPt1 = oscTextPos + m_xAxis*(halfTextWidth-m_gap);
          m_ldrPt2 = oscTextPos - m_xAxis*(halfTextWidth);
        } else
        {
          m_ldrPt1 = oscTextPos - m_xAxis*(halfTextWidth-m_gap);
          m_ldrPt2 = oscTextPos + m_xAxis*(halfTextWidth);
        }
      } else
      {
        if ( oscTextPos.distanceTo(m_arrow1Pt) < oscTextPos.distanceTo(m_arrow2Pt) )
        {
          m_ldrPt3 = m_arrow1Pt;
        } else
        {
          m_ldrPt3 = m_arrow2Pt;
        }
        OdGeVector3d v = m_ldrPt3 - oscTextPos;
        if ( v.x > 0.0 )
        {
          m_ldrPt1 = oscTextPos + m_xAxis*(halfTextWidth);
          m_ldrPt2 = m_ldrPt1 + m_xAxis*asz;
        } else
        {
          m_ldrPt1 = oscTextPos - m_xAxis*(halfTextWidth);
          m_ldrPt2 = m_ldrPt1 - m_xAxis*asz;
        }
        m_bLeader = true;
      }
      // TODO: if codirectional text and dim line  m_bTextHasLine = true;
    } else
    {
      if ( m_tad > 0 && !m_bUseRotate )
      {
        m_bTextHasLine = true;
        OdGeVector3d v;
        if ( oscTextPos.distanceTo(m_arrow1Pt) < oscTextPos.distanceTo(m_arrow2Pt) )
        {
          m_textLinePt1 = m_arrow1Pt;
          v = -m_dirDimLine;
        }
        else  
        {
          m_textLinePt1 = m_arrow2Pt;
          v = m_dirDimLine;
        }
        m_textLinePt2 = oscTextPos + v*(halfTextWidth); 
      } else
      {
        if (m_bUseRotate && !m_dimToh && !OdNonZero(m_dimTvp, 1.e-5))
          return;
        m_bTextHasLine = true;
        OdGeVector3d v;
        if ( oscTextPos.distanceTo(m_arrow1Pt) < oscTextPos.distanceTo(m_arrow2Pt) )
        {
          m_textLinePt1 = m_arrow1Pt;
          v = m_dirDimLine;
        }
        else  
        {
          m_textLinePt1 = m_arrow2Pt;
          v = -m_dirDimLine;
        }
        if (!m_bUseRotate && OdNonZero(m_dimTvp, 1.e-5))
          v *= -1.0;
        m_textLinePt2 = oscTextPos + v*(halfTextWidth);
      }
    }
  }
}

void OdAlignedRecomputorEngine::testTextLocation0()
{
  OdGeVector3d xAxis  = m_dirDimLine;
  xAxis.normalize();

  OdGeVector3d yUnit = OdGeVector3d::kZAxis.crossProduct(xAxis).normalize();
  OdGeMatrix3d cordSys;
  cordSys.setCoordSystem(m_middleDimLinePt, xAxis, yUnit, OdGeVector3d::kZAxis);
  cordSys.invert();

  OdGePoint3d testPt = m_textPosition;
  testPt.transformBy(cordSys);

  OdGePoint3d point1 = m_arrow1Pt;
  OdGePoint3d point2 = m_arrow2Pt;
  point1.transformBy(cordSys);
  point2.transformBy(cordSys);

  if ( testPt.x > point1.x && testPt.x < point2.x )
  {
    m_bTextInside = true;
  } else
  {
    m_bTextInside = false;
  }

  if ( !m_bUseRotate )
  {
    if ( m_dimTih && m_bTextInside )
    {
      m_bTextHor = true;
    }
    if ( m_dimToh && !m_bTextInside )
    {
      m_bTextHor = true;
    }
  }
/*
  if ( m_bTextHor && (m_xAxis == m_dirDimLine || m_xAxis == -m_dirDimLine) ) 
  {
    m_bTextHor = false;
  }
*/
  m_bDimLineIsHor = m_dirDimLine.isCodirectionalTo(m_xAxis) || m_dirDimLine.isCodirectionalTo(-m_xAxis);

  bool doContinue = false;

  if ( m_bTextInside )
  {
    if ( m_bUseRotate )
    {
      doContinue = true;
    } else if ( m_tad < 1 )
    {
      doContinue = true;
    } else if ( m_bTextHor && m_tad < 2 && !m_bDimLineIsHor)
    {
      doContinue = true;
    }
  } else
  {
    doContinue = false;
  }

  if ( !doContinue )
  {
    if  ( m_dimatfit < 2 )
    {
      m_bArrowsInside = m_bTextInside;
    } else
    {
      fitTextAndArrows(false, true);
      m_bArrowsInside = m_isFitArrowsWithoutText;
    }
    m_bNeedCalcTextIntersection = false;
    m_dimLines = 1;
    return;
  }

  double halfTextWidth;
  if ( m_bTextHor || m_bUseRotate )
  {
    halfTextWidth = lengthTextOnDimLine()/2.0;
  } else
  {
    halfTextWidth = textWidth()/2.0; // + m_gap;
  }

  if ( point1.x + m_asz1 + halfTextWidth > testPt.x  || point2.x - m_asz2 - halfTextWidth < testPt.x )
  {
    if (testPt.x - halfTextWidth < point1.x && testPt.x + halfTextWidth > point2.x && m_asz1 + m_asz2 + m_gap * 2 < point2.x - point1.x)
      m_bArrowsInside = true;
    else
      m_bArrowsInside = false;
  } else
  {
    if ( point1.x + halfTextWidth > testPt.x  || point2.x - halfTextWidth < testPt.x )
    {
      m_bNeedCalcTextIntersection = false;
      m_dimLines = 1;
    } else
    {
      m_bNeedCalcTextIntersection = true;
      m_dimLines = 2;
    } 
  }
}


void OdAlignedRecomputorEngine::adjustUserDefText1(double /*asz*/, double exe)
{
  // no justify   add leader
  // check leader
  OdGePoint3d oscTextPosition = m_textPosition;
  m_dimOcsTextPosition = oscTextPosition;

  m_textDirection = m_dirDimLine;
  m_textDirection.normalize();

  m_bNeedCalcTextIntersection = false;
  m_dimLines = 1;

  bool bLeaderOnDimLine = testTextLocation1(exe);

  bool bTvp = ( m_tad == 0 && OdNonZero(m_dimTvp, 1.e-5));
  if (!bTvp && ( bLeaderOnDimLine || m_bTextHasLine))
  {
    correctTextDirection();
    return;
  }

  if (m_bTextHor)
  {
    OdInt16 tempTad = m_tad;
    m_textDirection = m_xAxis;
    if ( m_tad == 2 && m_bLeader )
      m_tad = 1;

    calcVerticalDirection();
    if ( m_bLeader )
    {
      m_textPosition += m_leaderDir*(textWidth(false)/2.0);
    } 
    double vDist = verticalDistToText();
    m_textPosition += m_perpTextDir*vDist;

    if ( m_bTextInside && m_tad < 2 && OdZero(vDist) ) // TODO: 
    {
      m_bNeedCalcTextIntersection = true;
      m_dimLines = 2;
    }
    m_tad = tempTad;
  } else if ( !m_bUseRotate || bTvp )
  {
    correctTextDirection();
    double vDist = verticalDistToText();
    calcVerticalDirection();
    if ( m_bLeader )
    {
      m_textPosition += m_leaderDir*(textWidth(false)/2.0); 
    }
    m_textPosition += m_perpTextDir*verticalDistToText(); 
    if ( m_bTextInside && m_tad == 0 && OdZero(vDist) &&  m_dimLines != 0 )
    {
      m_bNeedCalcTextIntersection = true;
      m_dimLines = 2;
    } else if (m_tad == 1 && m_dimLines != 0 )
    {
      m_bNeedCalcTextIntersection = false;
      m_dimLines = 1;
    }
    if (bTvp && !m_bTextInside && !m_bUseRotate)
      m_bLeader = true;
  } else
  {
    // TODO: calc text pos
  }

  if ( m_bLeader )
    makeLeader(oscTextPosition, m_leaderDir, bTvp);

}

void OdAlignedRecomputorEngine::makeLeader(const OdGePoint3d& oscTextPosition, const OdGeVector3d& leaderDir, bool bTvp)
{
  if (bTvp)
  {
    m_ldrPt1 = m_ldrPt2 = oscTextPosition;
    if (!m_isTxtPosFarFromDimLine)
    {
      m_ldrPt1 += leaderDir*(textWidth(false) / 2.0 + m_gap);
      m_ldrPt2 -= leaderDir*(textWidth(false) / 2.0 + m_gap);
    } else
    {
      m_ldrPt1 += leaderDir*(textWidth(false) + m_gap);
    }
  }
  else if (m_tad > 0 && !m_bUseRotate)
  {
    m_ldrPt1 = oscTextPosition;
    m_ldrPt1 += leaderDir*(textWidth(false)+m_gap);
    m_ldrPt2 = oscTextPosition;
    m_ldrPt2 -= leaderDir*m_gap;
  } else if ( !bTvp )
  {
    m_ldrPt1 = oscTextPosition;
    m_ldrPt1 -= leaderDir*m_gap;
    m_ldrPt2 = m_ldrPt1;
    m_ldrPt2 -= leaderDir*2.0*m_gap;
  }
  m_ldrPt3 = m_middleDimLinePt;
}

void OdAlignedRecomputorEngine::makeLeaderAsTextLine(const OdGePoint3d& pt, double distToText, double /*dVert*/)
{
  m_bTextHasLine = true;
  m_textLinePt1 = pt;

  m_textLinePt2 = m_textLinePt1 + m_dirDimLine*distToText;

  correctTextDirection();
  OdGeVector3d perp = m_textDirection.perpVector();
  perp.normalize();
  if ( m_tad > 0 )
  {
    // Above text case
    if (m_tad == 1)
    {
      double angle = perp.angleTo(OdGeVector3d::kYAxis);
      if ((angle > OdaPI2) && (angle < (OdaPI + OdaPI2)))
        perp *= -1.;
    }

    if ( m_isBox )
      m_textPosition += perp*(textHeight()/2.0+m_gap);
    else
      m_textPosition += perp*textHeight()/2.0;
    if ( distToText < 0.0 )
      m_textLinePt2 -= m_dirDimLine*textWidth();
    else
      m_textLinePt2 += m_dirDimLine*textWidth();
  }
}

void OdAlignedRecomputorEngine::makeLeaderOnDimLine(const OdGePoint3d& startPoint, const OdGePoint3d& secondPoint)
{
  // need calc text position (m_asz + halfTextWidht)
  m_ldrPt3 = startPoint;
  m_ldrPt2 = secondPoint;

  m_bLeader = true;
  double halfTextHeight = textHeight()/2.0;
  double halfTextWidth = textWidth()/2.0;

  double distanceToTextPos = secondPoint.distanceTo(m_textPosition);

  // OdGeVector3d perp = m_yAxis;
  m_ldrPt1 = secondPoint;
  if ( m_asz + halfTextWidth > distanceToTextPos )
  {
    // correct text pos
    m_textPosition = secondPoint + m_leaderDir*(m_asz + halfTextWidth);
    if ( m_bUseRotate || m_tad == 0 )
    {
      m_ldrPt1 += m_leaderDir*m_asz;
    } else
    {
      m_ldrPt1 += m_leaderDir*(m_asz+halfTextWidth*2.0);
      if ( m_isBox )
        m_textPosition += m_yAxis*(halfTextHeight+m_gap);
      else
        m_textPosition += m_yAxis*halfTextHeight;

    }
  } else
  {
    if ( m_bUseRotate || m_tad == 0 )
    {
      m_ldrPt1 += m_leaderDir*(distanceToTextPos-halfTextWidth);
    } else
    {
      m_ldrPt1 += m_leaderDir*(distanceToTextPos+halfTextWidth);
      if ( m_isBox )
        m_textPosition += m_yAxis*(halfTextHeight+m_gap);
      else
        m_textPosition += m_yAxis*halfTextHeight;
    }
  }
}

bool OdAlignedRecomputorEngine::testTextLocation1(double exe)
{
  bool bLeaderOnDimLine = false;

  OdGePoint3d oscTextPosition = m_textPosition;
  double halfTextHeight = textHeight()/2.0;
  double halfTextWidth = textWidth()/2.0;

  OdGeVector3d xAxis  = m_dirDimLine;
  xAxis.normalize();

  OdGeVector3d yUnit = OdGeVector3d::kZAxis.crossProduct(xAxis).normalize();
  OdGeMatrix3d cordSys;
  cordSys.setCoordSystem(m_middleDimLinePt, xAxis, yUnit, OdGeVector3d::kZAxis);
  cordSys.invert();

  OdGePoint3d testPt = oscTextPosition;
  testPt.transformBy(cordSys);

  OdGePoint3d point1 = m_arrow1Pt;
  OdGePoint3d point2 = m_arrow2Pt;
  point1.transformBy(cordSys);
  point2.transformBy(cordSys);

  double distFromTxt2DimLine = ::fabs(testPt.y);
  m_leaderDir = m_textDirection;

  bool isTxtPosBetweenExtLine  = false;
  bool isTxtPosFarFromDimLine  = false;
  m_isTxtPosFarFromArrowPoint = false;

  if ( distFromTxt2DimLine - exe - halfTextHeight > 0.0 )
  {
    isTxtPosFarFromDimLine = true;
  }
  if ( testPt.x > point1.x && testPt.x < point2.x )
  {
    isTxtPosBetweenExtLine = true;
  } else if ( (testPt.x > 0.0 && point1.x > 0.0) || (testPt.x < 0.0 && point1.x < 0.0))
  {
    if ( ::fabs(testPt.x - point1.x) > halfTextWidth + 2*m_asz )
      m_isTxtPosFarFromArrowPoint = true;
  } else
  {
    if ( ::fabs(testPt.x - point2.x) > halfTextWidth + 2*m_asz )
      m_isTxtPosFarFromArrowPoint = true;
  }
  m_isTxtPosFarFromDimLine = isTxtPosFarFromDimLine;
  if ( isTxtPosFarFromDimLine )
  {
    m_bTextInside = false;
  } else if ( isTxtPosBetweenExtLine ) 
  {
    m_bTextInside = true;
  } else
  {
    m_bTextInside = false;
  }

  if ( !m_bUseRotate )
  {
    if ( m_dimTih && m_bTextInside )
    {
      m_bTextHor = true;
    }
    if ( m_dimToh && !m_bTextInside )
    {
      m_bTextHor = true;
    }
  }

  if ( m_bTextHor )
  {
    m_textDirection = m_xAxis;
    if ( oscTextPosition.x < m_middleDimLinePt.x )
      m_leaderDir = -m_xAxis;
    else
      m_leaderDir = m_xAxis;
  }

  if ( !m_bTextInside )
  {
    if ( !m_bTextHor && !m_bUseRotate && !isTxtPosFarFromDimLine )
    {
      // If text near extension line
      m_bLeader = false;
      if ( testPt.x + halfTextWidth + m_asz < point1.x )
      {
        m_textPosition -= yUnit*testPt.y;
        makeLeaderAsTextLine(m_arrow1Pt, testPt.x + halfTextWidth - point1.x, testPt.y );
      } else if ( testPt.x - halfTextWidth - m_asz > point2.x )
      {
        m_textPosition -= yUnit*testPt.y;
        makeLeaderAsTextLine(m_arrow2Pt, testPt.x - halfTextWidth - point2.x, -testPt.y);
      } else
      {
        m_bLeader = false;
      }
      fitTextAndArrows(false, true);
      m_bArrowsInside = m_isFitArrowsWithoutText; // && isTxtPosBetweenExtLine;
      if (m_dimatfit < 2 && m_isFitArrowsWithoutText)
        m_bArrowsInside = isTxtPosBetweenExtLine;
     
      return bLeaderOnDimLine;
    } /* else
      {
      m_bLeader = true;
      } */
  } else
  {
    m_bLeader = false;
    if ( !isTxtPosFarFromDimLine && isTxtPosBetweenExtLine )
    {
      if ( !OdNegative(distFromTxt2DimLine - halfTextHeight) )
      {
        m_bNeedCalcTextIntersection = false;
        m_dimLines = 1;
        if ( m_bTextHor || m_bUseRotate )
        {
          if ( point1.x + m_asz1 + halfTextWidth > testPt.x  || point2.x - m_asz2 - halfTextWidth < testPt.x )
          {
            m_bArrowsInside = false;
          } else
          {
            m_bArrowsInside = true;
            m_bNeedCalcTextIntersection = true;
            m_dimLines = 2;
          }
        }
      } else
      {
        m_bNeedCalcTextIntersection = true;
        m_dimLines = 2;
        if ( m_asz1 + m_asz2 + 2*m_gap > point2.x - point1.x )
        {
          m_bArrowsInside = false;
          m_bNeedCalcTextIntersection = false;
          m_dimLines = 1;
        } else if ( m_tad == 0 && (( testPt.x - halfTextWidth - m_asz < point1.x ) || 
          ( testPt.x + halfTextWidth + m_asz > point2.x )) )
        {
          // Text near arrow points
          m_bNeedCalcTextIntersection = false;
          m_bArrowsInside = false;
          m_dimLines = 0;
        } 
      }
    } 
    return bLeaderOnDimLine;
  }

  // where leader will be
  if ( !m_bTextHor && !m_bUseRotate )
  {
    m_bLeader = true;
    m_leaderDir = m_dirDimLine;
    m_leaderDir.normalize();
    if ( testPt.x < 0 )
      m_leaderDir *= -1.0;
    else
      m_leaderDir *= 1.0;

    fitTextAndArrows(false, true);
    m_bArrowsInside = m_isFitArrowsWithoutText;
    if (m_dimatfit < 2 && m_isFitArrowsWithoutText)
      m_bArrowsInside = isTxtPosBetweenExtLine;

    return bLeaderOnDimLine;
  }

  // here is horizontal or rotated text with leader
  if ( isTxtPosBetweenExtLine )
  {
    m_bLeader = true;
    fitTextAndArrows(false, true);
    m_bArrowsInside = m_isFitArrowsWithoutText; // && isTxtPosBetweenExtLine;
    return bLeaderOnDimLine;
  }

  // here is horizontal or rotated text only with leader outside ext lines
  // if ( m_bTextHor || m_bUseRotate )
  const OdGeVector2d dimLineDir(m_dirDimLine.x, m_dirDimLine.y);
  const OdGeVector2d leaderLineDir(m_xAxis.x, m_xAxis.y);
  const OdGeLine2d dimLine(m_middleDimLinePt.convert2d(), dimLineDir);
  const OdGeLine2d leaderLine(oscTextPosition.convert2d(), leaderLineDir);

  OdGePoint2d point;
  bool isIntersect = dimLine.intersectWith(leaderLine, point);

  bool b = (dimLineDir.isParallelTo(leaderLineDir) && dimLineDir.dotProduct(leaderLineDir) >= 0.0) || !isIntersect;

#if 0
  OdGeLine2d dimLine( OdGePoint2d(m_middleDimLinePt.x,m_middleDimLinePt.y), OdGeVector2d(m_dirDimLine.x,m_dirDimLine.y));
  OdGeLine2d leaderLine(OdGePoint2d(oscTextPosition.x, oscTextPosition.y), OdGeVector2d(m_xAxis.x, m_xAxis.y) );
  bool b = dimLine.isColinearTo(leaderLine);
#endif
  if ( b )
  {
    if ( isTxtPosFarFromDimLine )
    {
      m_bLeader = true;
    } else
    {
      // m_bLeaderOnDimLine = true;
      if ( testPt.x + halfTextWidth + m_asz < point1.x )
      {
        m_textPosition -= yUnit*testPt.y;
        makeLeaderAsTextLine(m_arrow1Pt, testPt.x + halfTextWidth - point1.x, testPt.y );
      } else if ( testPt.x - halfTextWidth - m_asz > point2.x )
      {
        m_textPosition -= yUnit*testPt.y;
        makeLeaderAsTextLine(m_arrow2Pt, testPt.x - halfTextWidth - point2.x, testPt.y);
      } 
    }
    fitTextAndArrows(false, true);
    m_bArrowsInside = m_isFitArrowsWithoutText;
    if (m_dimatfit < 2 && m_isFitArrowsWithoutText)
      m_bArrowsInside = isTxtPosBetweenExtLine;
    return bLeaderOnDimLine;
  }

  OdGePoint3d pt(point.x, point.y, 0.0);
  pt.transformBy(cordSys);

  if ( pt.x > point1.x && pt.x < point2.x )
  {
    // point is between extension line
    m_bLeader = true;
    if ( oscTextPosition.x < m_middleDimLinePt.x )
      m_leaderDir = -m_xAxis;
    else
      m_leaderDir = m_xAxis;
    fitTextAndArrows(false, true);
    m_bArrowsInside = m_isFitArrowsWithoutText;
    if (m_dimatfit < 2 && m_isFitArrowsWithoutText)
      m_bArrowsInside = isTxtPosBetweenExtLine;
    return bLeaderOnDimLine;
  }

  if ( ( testPt.x > 0.0 && pt.x < 0.0 ) ||
    ( testPt.x < 0.0 && pt.x > 0.0 ) )
  {
    m_bLeader = true;
    fitTextAndArrows(false, true);
    m_bArrowsInside = m_isFitArrowsWithoutText;
    if (m_dimatfit < 2 && m_isFitArrowsWithoutText)
      m_bArrowsInside = isTxtPosBetweenExtLine;
    return bLeaderOnDimLine;
  }

  double halfmeasurement = ::fabs(point1.x);
  OdGePoint2d p;
  OdGeLine2d lineToText(OdGePoint2d(0.0, 0.0), OdGePoint2d(testPt.x, testPt.y));
  OdGeLine2d line;
  if ( pt.x > point1.x )
  {
    line.set(OdGePoint2d(point1.x*2/3, 0.0), OdGeVector2d(0.0, 1.0));
  } else
  {
    line.set(OdGePoint2d(point2.x*2/3, 0.0), OdGeVector2d(0.0, 1.0));
  }
  b = line.intersectWith(lineToText, p);

  if ( b && ::fabs(testPt.y) > halfmeasurement + ::fabs(p.y) )
  {
    m_bLeader = true;
  } else 
  {
    m_leaderDir = m_textPosition - OdGePoint3d(point.x, point.y, 0.0);
    // TODO //if (!m_leaderDir.isZeroLength()) // CORE-12888
    {
      m_leaderDir.normalize();
      bLeaderOnDimLine = true;
      if (pt.x > point1.x)
      {
        makeLeaderOnDimLine(m_arrow2Pt, OdGePoint3d(point.x, point.y, 0.0));
        // need calc text position (m_asz + halfTextWidht)
      }
      else
      {
        makeLeaderOnDimLine(m_arrow1Pt, OdGePoint3d(point.x, point.y, 0.0));
        // m_leaderDir = -m_xAxis;
      }
    }
  }
  fitTextAndArrows(false, true);
  m_bArrowsInside = m_isFitArrowsWithoutText;
  if (m_dimatfit < 2 && m_isFitArrowsWithoutText)
    m_bArrowsInside = isTxtPosBetweenExtLine;
  return bLeaderOnDimLine;
}

void OdAlignedRecomputorEngine::adjustUserDefText2(double exe)
{
  // justify with dim line  no add leader
  // TODO:
  //  need check text inside or outside
  //  apply  m_dimTih || m_dimToh
  // 
  addGapToTextExtents();
  testTextLocation2(exe);
  if ( m_bTextHor )
    m_textDirection = m_xAxis;
  else
    m_textDirection = m_dirDimLine;
  if ( !m_bUseRotate )
  {
    correctTextDirection();
  }
}

void OdAlignedRecomputorEngine::testTextLocation2(double exe)
{
  OdGePoint3d oscTextPosition = m_textPosition;
  double halfTextHeight = textHeight()/2.0;

  OdGeVector3d xAxis  = m_dirDimLine;
  xAxis.normalize();

  OdGeVector3d yUnit = OdGeVector3d::kZAxis.crossProduct(xAxis).normalize();
  OdGeMatrix3d cordSys;
  cordSys.setCoordSystem(m_middleDimLinePt, xAxis, yUnit, OdGeVector3d::kZAxis);
  cordSys.invert();

  OdGePoint3d testPt = oscTextPosition;
  testPt.transformBy(cordSys);

  double distFromTxt2DimLine = ::fabs(testPt.y);

  bool isTxtPosBetweenExtLine = false;
  OdGePoint3d point1 = m_arrow1Pt;
  OdGePoint3d point2 = m_arrow2Pt;
  point1.transformBy(cordSys);
  point2.transformBy(cordSys);

  if (testPt.x > point1.x && testPt.x < point2.x)
  {
    isTxtPosBetweenExtLine = true;
  }

  if ( distFromTxt2DimLine - exe - halfTextHeight > 0.0 )
  {
    m_bTextInside = false;
    if ( m_dimToh )
      m_bTextHor = true;
    m_bNeedCalcTextIntersection = false;
    m_dimLines = 1;
    fitTextAndArrows(false, true);
    m_bArrowsInside = m_isFitArrowsWithoutText;
    if (m_dimatfit < 2 )
      m_bArrowsInside = isTxtPosBetweenExtLine;
    return;
  }

  if ( testPt.x > point1.x && testPt.x < point2.x )
  {
    m_bTextInside = true;
    if ( m_dimTih )
      m_bTextHor = true;
  } else
  {
    m_bTextInside = false;
    if ( m_dimToh )
      m_bTextHor = true;
  }

  if ( m_bTextInside )
  {
    double halfTextWidth;
    if ( m_bTextHor || m_bUseRotate )
    {
      halfTextWidth = lengthTextOnDimLine()/2.0;
    } else
    {
      halfTextWidth = textWidth()/2.0;
    }

    if ( !OdNegative(distFromTxt2DimLine - halfTextHeight) )
    {
      m_bNeedCalcTextIntersection = false;
      m_dimLines = 1;
      if ( m_bTextHor || m_bUseRotate )
      {
        if ( point1.x + m_asz1 + halfTextWidth > testPt.x  || point2.x - m_asz2 - halfTextWidth < testPt.x )
        {
          m_bArrowsInside = false;
        } else
        {
          m_bArrowsInside = true;
          m_bNeedCalcTextIntersection = true;
          m_dimLines = 2;
        }
      }
    } else
    {
      m_bNeedCalcTextIntersection = true;
      m_dimLines = 2;
      if ( (m_asz1 + m_asz2 + 2*m_gap > point2.x - point1.x) )
      {
        m_bArrowsInside = false;
        m_bNeedCalcTextIntersection = false;
        m_dimLines = 1;
      } 
      if ( ( testPt.x - halfTextWidth - m_asz < point1.x ) || 
        ( testPt.x + halfTextWidth + m_asz > point2.x ) )
      {
        // Text near arrow points
        m_bNeedCalcTextIntersection = false;
        m_bArrowsInside = false;
        m_dimLines = 0;
      }
    }
    fitTextAndArrows(false, true);
    if (m_dimLines != 0)
    {
      if (m_dimatfit < 2)
        m_bArrowsInside = isTxtPosBetweenExtLine;
      else
        m_bArrowsInside = m_isFitArrowsWithoutText;
    }
  } else
  {
    fitTextAndArrows(false, true);
    if (m_dimatfit < 2)
      m_bArrowsInside = isTxtPosBetweenExtLine;
    else
      m_bArrowsInside = m_isFitArrowsWithoutText;

    m_bNeedCalcTextIntersection = false;
    m_dimLines = 1;
  }
}

void OdAlignedRecomputorEngine::applyDimMoveForDefaultPos(OdInt16 dimMove, double dimExe)
{
  if ( dimMove != 1 && dimMove != 2  )
    return;

  OdGeVector3d v = m_dirExtLine; // m_xLine1PtEnd.asVector() - m_xLine1Pt.asVector();
  // v.normalize();
  OdGePoint3d oscTextPosition;

  if ( dimMove == 1 )
  {
    OdUInt16 tad = m_tad;
    bool useRot = m_bUseRotate;

    OdGeVector3d vectToText = m_dirDimLine;
    if ( OdNonZero(m_dTextRot) ) // rotated text - ACAD has bug
      m_tad = 0;
    if ( m_bUseRotate || m_bTextHor )
    {
      m_textDirection = m_xAxis;
      vectToText = m_xAxis;
      OdGeVector3d dirExtLine = m_xLine2PtEnd - m_xLine2Pt;
      if ( OdNonZero(dirExtLine.length() ))
        dirExtLine.normalize();
      v = dirExtLine;
      if ( !OdNegative( dirExtLine.x) ) // >= 0.0 )
        vectToText = -m_xAxis;
    }
    else
    {
      m_textDirection = m_dirDimLine;

      if ( m_textDirection.x > 0.0 )
        vectToText *= -1.0;

      correctTextDirection();
    }

    double halfTextHeight = textHeight(false)/2.0;
    double vertDistToText = 0.0;

    if ( m_bUseXText )
    {
      vertDistToText = dimExe + 2.5*m_gap + m_dXTextHeight/2.0 + halfTextHeight;
    } else
    {
      if ( m_bTextHor || m_bUseRotate )
      {
        if ( vectToText.isCodirectionalTo(-v) )
        {
          m_isTxtPosFarFromArrowPoint = true;
          vertDistToText = dimExe + 0.5*m_gap + textHeight(false)*2.0;
        } else
        {
          vertDistToText = dimExe + 0.5*m_gap + textHeight(false);
        }
      } else
      {
        vertDistToText = 1.5*m_gap + dimExe + halfTextHeight;
      }
    }

    double addedDist = halfTextHeight*2.0;
    if ( m_bUseXText )
    {
      addedDist += (m_dXTextHeight + 2*m_gap);
    }

    if ( m_bTextHor || m_bUseRotate ) 
    {
      if ( m_dirExtLine.y < 0.0 ) 
        vertDistToText += addedDist;
    } else if ( m_dirExtLine.y <= 0 ) //  m_textDirection.y > 0 )
    {
      vertDistToText += addedDist;
    }
    double horDistToText = 0.0; 
    if ( !m_bUseRotate && !m_bTextHor)
    {
      horDistToText = vertDistToText*::tan(0.291457);
    }
    if ( m_tad > 0 )
    {
      horDistToText += m_gap;
    } else
    {
      horDistToText += 3.0*m_gap;
    }

    oscTextPosition = m_middleDimLinePt + v*vertDistToText;
    oscTextPosition -= vectToText*horDistToText;

    if ( !m_bTextInside && vertDistToText-m_gap >= m_asz )
    {
      if ( OdZero(m_dTextRot) )
        m_bUseRotate = false;

      makeLeader(oscTextPosition, -vectToText);
      m_bLeader = true;
      horDistToText = textWidth(false)/2.0;
    } else
    {
      horDistToText = 0.0;
    }

    if ( m_tad > 0 )
    {
      vertDistToText = halfTextHeight;
      vertDistToText += m_gap;
      if ( m_isBox )
        vertDistToText += m_gap;
    } else
    {
      vertDistToText = 0.0;
    }
    OdGeVector3d perpVect = m_textDirection.perpVector().normalize();
    m_textPosition = oscTextPosition - vectToText*horDistToText + perpVect*vertDistToText;
    
    m_dimOcsTextPosition = m_textPosition;
    if ( m_bLeader )
      m_dimOcsTextPosition += vectToText*textWidth(false)/2.0;
    m_tad = tad;
    m_bUseRotate = useRot;
  } else 
  {
    double dist = dimExe;
    double halfTextHeight = textHeight(false)/2.0;

    OdGePoint3d p =  m_middleDimLinePt + v*(dist+halfTextHeight+1.5*m_gap);

    if ( v.y < 0.0 )
      p += v*halfTextHeight*2.0;

    m_textPosition = p;
    m_textDirection = m_dirDimLine;
    correctTextDirection();
    m_dimOcsTextPosition = m_textPosition;
  }
  m_bTextInside = true;
  m_bNeedCalcTextIntersection = false;
  m_dimLines = 1;
}

// calculation m_perpTextDir
// m_dirDimLine && m_textDirection are used 
void OdAlignedRecomputorEngine::calcVerticalDirection()
{
  OdGeVector3d dirDimLine = m_dirDimLine;
  if ( m_dirDimLine == OdGeVector3d::kYAxis )
    dirDimLine *= -1.0;

  if ( m_tad == 0 && OdNonZero(m_dimTvp, 1.e-5 ) )
  {
    if ( m_just < 3 )
    {
      m_perpTextDir = dirDimLine.perpVector(); // m_dirDimLine.perpVector();
      OdGeVector3d vv = dirDimLine;
      OdGeMatrix3d matr;
      matr.setCoordSystem(OdGePoint3d::kOrigin, m_xAxis, m_yAxis, OdGeVector3d::kZAxis);
      matr.invert();
      vv.transformBy(matr);
      if (!OdNegative(vv.y, 1e-12))
      {
        if (OdNegative(vv.x, 1e-12))
          m_perpTextDir *= -1.0;
      }
      else
      {
        if (OdPositive(vv.x, 1e-12))
          m_perpTextDir *= -1.0;
      }
      if (m_perpTextDir.y < 0.0 || OdZero(m_perpTextDir.y))
        m_perpTextDir *= -1.0;
    }
    else
    {
      if ( m_textDirection.y > 0.0 )
        m_perpTextDir = m_textDirection.perpVector();
      else
        m_perpTextDir = -m_textDirection.perpVector();
    }
    return;
  }

  if ( m_tad == 1 || m_tad == 3 || m_tad == 4 )
  {
    if ( m_just < 3 || m_bUserDefPos )
    {
      if ( m_bTextHor && !m_bTextInside )
      {
        m_perpTextDir = m_yAxis;
      } else
      {
        if ( m_bTextHor )
        {
          m_perpTextDir = dirDimLine.perpVector(); // m_dirDimLine.perpVector();
          if ( OdZero(m_perpTextDir.y) )
            m_perpTextDir = -m_xAxis;
          else if ( m_perpTextDir.y < 0.0 )
            m_perpTextDir *= -1.0;
        } else
        {
          m_perpTextDir = m_textDirection.perpVector();
        }
      }
    } else
    {
      OdGeVector3d vv = m_textDirection;
      OdGeMatrix3d matr;
      matr.setCoordSystem(OdGePoint3d::kOrigin, m_xAxis, m_yAxis, OdGeVector3d::kZAxis);
      matr.invert();
      vv.transformBy(matr);

      m_perpTextDir = m_textDirection.perpVector();

      if ( OdPositive(vv.y, 1e-12) )
      {
        if ( OdNegative(vv.x, 1e-12) ) 
          m_perpTextDir *= -1.0;
      } else
      {
        if ( OdPositive(vv.x, 1e-12) )
          m_perpTextDir *= -1.0; 
      }
    }
  }

  if ( m_tad == 2 )
  {
    m_perpTextDir = dirDimLine.perpVector(); // m_dirDimLine.perpVector();
    double dotProd   = m_dirExtLine.dotProduct(m_perpTextDir);
    if ( dotProd < 0.0 )
    {
      m_perpTextDir *= -1.0;
    }
    if ( !m_bTextInside && m_bTextHor )
    {
      double dotProd1  = m_yAxis.dotProduct(m_perpTextDir);
      if ( dotProd1 > 0.0 )
        m_perpTextDir = m_yAxis;
      else
        m_perpTextDir = -m_yAxis;
    }
    if (m_just > 2 && !m_bUserDefPos )
      m_perpTextDir = m_perpTextDir.perpVector();
  }
}
bool OdAlignedRecomputorEngine::makeJogSymbol(OdGePoint3d& point, double size, OdGePoint3dArray& jogPoints, OdDbMTextPtr addedText, bool bDimLine1, bool bDimLine2)
{
  double sizeJog = size* m_textHeight;
  OdGePoint3d textPosition = m_textPosition;

//   ACAd changed behavior start 2011
//   if ( m_bUseXText )  
//   {
//     OdGeVector3d v = m_textDirection.perpVector();
//     v.normalize();
//     v *= -1.0;
// 
//     double h1 = textHeight(false);
// 
//     OdGeExtents3d exts;
//     addedText->setDirection(OdGeVector3d::kXAxis);
//     addedText->getGeomExtents(exts);
// 
//     double h2 = (exts.maxPoint().y - exts.minPoint().y);
//     sizeJog = size*(h1+h2+m_gap*2.0);
// 
//     textPosition += v*(h1/2.0 + m_gap);
//   }

  OdGePoint3d jogPoint = point; // TODO: to plane

  OdGeVector3d perp = m_dirDimLine.perpVector();
  OdGeLine3d perpLine(jogPoint, perp);
  OdGeLine3d dimLine(m_dimLineDefPt, m_dirDimLine); 
  perpLine.intersectWith(dimLine, jogPoint);

  double extLineDist = m_xLine2PtEnd.distanceTo(m_xLine1PtEnd);
  double toExtLine2  = jogPoint.distanceTo(m_xLine2PtEnd);
  double toExtLine1  = jogPoint.distanceTo(m_xLine1PtEnd);
  // Is jogPoint between ext lines
  if ( OdNonZero(extLineDist - toExtLine2 - toExtLine1 ) ||
    extLineDist - 2*m_asz < sizeJog )
  {
    return false;
  }

  if ( odmin(toExtLine1, toExtLine2) < sizeJog/2.0 + m_asz )
  {
    return false;
  }

  if ( !bDimLine1 || !bDimLine2  )
  {
    if ( jogPoint.distanceTo(m_dimLineStart1Pt) > jogPoint.distanceTo(m_dimLineStart2Pt) )
    {
      if ( bDimLine2 )
        return false;
    } else
    {
      if ( bDimLine1 )
        return false;
    }
  }

  OdGePoint3d p1, p2, p3, p4;

  p1 = jogPoint + m_dirDimLine*sizeJog/2.0;
  p2 = p1 - m_dirDimLine*sizeJog/3.0;
  p2 -= perp*sizeJog/2.0;
  p4 = jogPoint - m_dirDimLine*sizeJog/2.0;
  p3 = p4 + m_dirDimLine*sizeJog/3.0;
  p3 += perp*sizeJog/2.0;

  if ( m_dimLines == 1 )
  {
    if ( !bDimLine1 || !bDimLine2 )
    {
      if ( !bDimLine1 )
      {
        m_AddedDimLineStart = m_dimLineStart2Pt;
        m_AddedDimLineEnd = p1;
        m_dimLineStart2Pt = p4;
      } else
      {
        m_AddedDimLineStart = m_dimLineStart1Pt;
        m_AddedDimLineEnd = p4;
        m_dimLineStart1Pt = p1;
      }
      m_bAddedDimLine = true;
    } else
    {
      m_dimLineStart1Pt = p4;
      m_dimLineStart2Pt = p1;
    }
    // Text is placed above
  } else if ( m_dimLines == 2 )
  {
    // Text is placed center 
    bool bNeedAddedDimLine = true;
    double toText = jogPoint.distanceTo(textPosition);
    double textDist = m_dimLineStart2Pt.distanceTo(m_dimLineStart1Pt);
    if ( toText < textDist/2.0+sizeJog/2.0 )
    {
      if ( toText > textDist/2.0 && toExtLine2 < toExtLine1 )
      {
        jogPoint = m_dimLineStart2Pt + m_dirDimLine*sizeJog/2.0;

      } else
      {
        // new caclulated jogPoint
        jogPoint = m_dimLineStart1Pt - m_dirDimLine*sizeJog/2.0;
      }
      toExtLine2  = jogPoint.distanceTo(m_xLine2PtEnd);
      toExtLine1  = jogPoint.distanceTo(m_xLine1PtEnd);
      if ( odmin(toExtLine1, toExtLine2) < sizeJog/2.0 + m_asz  )
        return false;

      p1 = jogPoint + m_dirDimLine*sizeJog/2.0;
      p2 = p1 - m_dirDimLine*sizeJog/3.0;
      p2 -= perp*sizeJog/2.0;
      p4 = jogPoint - m_dirDimLine*sizeJog/2.0;
      p3 = p4 + m_dirDimLine*sizeJog/3.0;
      p3 += perp*sizeJog/2.0;
      bNeedAddedDimLine = false;
    }
    if ( jogPoint.distanceTo(m_dimLineStart1Pt) < jogPoint.distanceTo(m_dimLineStart2Pt) )
    {
      m_AddedDimLineStart = m_dimLineStart1Pt;
      m_AddedDimLineEnd = p1;
      m_dimLineStart1Pt = p4;
    } else
    {
      m_AddedDimLineStart = m_dimLineStart2Pt;
      m_AddedDimLineEnd = p4; 
      m_dimLineStart2Pt = p1;
    }
    m_bAddedDimLine = bNeedAddedDimLine;
  }
  point = jogPoint;
  jogPoints.push_back(p1);
  jogPoints.push_back(p2);
  jogPoints.push_back(p3);
  jogPoints.push_back(p4);
  return true;
}
