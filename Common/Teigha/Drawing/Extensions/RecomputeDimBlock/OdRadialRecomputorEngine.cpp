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
#include "OdRadialRecomputorEngine.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeRay2d.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

void OdRadialRecomputorEngine::preprocessDimPoints(OdUInt16 dimMove)
{
  textLocation(dimMove);

  if ( dimMove == 2 || 
    m_atfit < 3 && m_bTextInside && dimMove != 1 ) 
  {
    dimMove2();
    return;
  }

  if ( !m_textExtents.isValidExtents() )
  {
    return;
  }

  double checkDist = 0.0 ;
  if ( m_dimMove == 0 && m_atfit == 2 && !m_bTextInside )
  {
    checkDist = m_asz;
  } else
  {
    checkDist = 2.0 * m_asz;
  }
  if ( !m_bTextHor && !m_bUseRotate )
  {
    OdGeLine2d radialLine(m_xLine1Pt.convert2d(), m_xLine2Pt.convert2d());
    OdGeLine2d perpLine(m_dimOcsTextPosition.convert2d(), radialLine.direction().perpVector());
    OdGePoint2d intPt;

    if(perpLine.intersectWith(radialLine, intPt))
    {
      checkDist += textWidth(false) /2.0 + m_gap;
      if ( intPt.distanceTo(m_xLine2Pt.convert2d()) <  checkDist 
        && intPt.distanceTo(m_xLine2Pt.convert2d()) < intPt.distanceTo(m_xLine1Pt.convert2d()) )
      {
        OdGeVector3d e_c;
        e_c = m_dimLineDir;
        e_c *= (m_bTextInside ? -1.0 : 1.0);
        m_dimOcsTextPosition = m_xLine2Pt + e_c*(checkDist);
      } else
      {
        m_dimOcsTextPosition.x = intPt.x;
        m_dimOcsTextPosition.y = intPt.y;
      }
      m_dogLegPt = m_dimOcsTextPosition;
    }
  } else // horizontal or rotated text
  {
    OdUInt16 dm = dimMove;
    if ( dimMove == 0 )
    {
      // Check angle
      if ( isHorDimLine() ) 
        dm = 1;
    }
    OdGeLine2d radialLine(m_xLine1Pt.convert2d(), m_xLine2Pt.convert2d());
    OdGeLine2d textLine(m_dimOcsTextPosition.convert2d(), m_xAxis.convert2d());
    OdGePoint2d intPt;
    if(textLine.intersectWith(radialLine, intPt))
    {
       m_dogLegPt.x = intPt.x;
       m_dogLegPt.y = intPt.y;
       double lenDimLine = m_xLine1Pt.distanceTo(m_xLine2Pt);

       if ( OdZero(lenDimLine - intPt.distanceTo(m_xLine1Pt.convert2d()) - intPt.distanceTo(m_xLine2Pt.convert2d())) )
         m_bDogLegInside = true;

      double dist = intPt.distanceTo(m_xLine2Pt.convert2d());
      double checkHorDist = textWidth(false)/2.0 + m_gap;

      if ( m_tad == 0 || ( m_tad == 1 && m_bDogLegInside ) )
        checkHorDist += m_asz;

      OdGeVector3d e_c;
      e_c = m_dimLineDir;
      e_c *= (m_bTextInside ? -1.0 : 1.0);
      double angl = e_c.angleTo(m_xAxis);
      double dir = !OdNegative(angl - OdaPI2) ? -1.0 : 1.0;

      if ( !m_bTextInside  )
      {
        if ( m_dimOcsTextPosition.distanceTo(m_xLine1Pt) < m_dimOcsTextPosition.distanceTo(m_xLine2Pt) )
          dir *= -1.0;
      }

      if ( dm == 0 &&  intPt.distanceTo(m_dimOcsTextPosition.convert2d()) < checkHorDist )
      {
        m_dimOcsTextPosition = OdGePoint3d(intPt.x, intPt.y, 0.0) + m_xAxis * dir * checkHorDist;
      }
      if ( intPt.distanceTo(m_xLine2Pt.convert2d()) < checkDist )
      {
        OdGePoint3d pt = m_xLine2Pt + e_c*(checkDist);
        m_dimOcsTextPosition += e_c*(checkDist - dist);
        m_dogLegPt += e_c*(checkDist - dist);
      }
    } else
    {
      OdGeLine2d radialLine(m_xLine1Pt.convert2d(), m_xLine2Pt.convert2d());
      OdGeVector2d dir = radialLine.direction();
      dir.rotateBy(OdaPI2);
      OdGeLine2d perpLine(m_dimOcsTextPosition.convert2d(), dir);
      OdGePoint2d intPt;
      perpLine.intersectWith(radialLine, intPt);

      m_dogLegPt.x = intPt.x;
      m_dogLegPt.y = intPt.y;
      m_dimOcsTextPosition = m_dogLegPt;
      OdGeVector3d e_c;
      e_c = m_dimLineDir;
      e_c *= (m_bTextInside ? 1.0 : -1.0);
      double dist = textWidth(false)/2.0 + m_gap;
      m_dogLegPt += e_c*dist;
    }
  } 
}

void OdRadialRecomputorEngine::calcTextDirection()
{
  if ( m_bTextHor )
  {
    m_textDirection = m_xAxis;
  } else if ( m_bUseRotate )
  {
    m_textDirection = m_xAxis;
    m_textDirection.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
  } else
  {
    m_textDirection = m_dimLineDir;
    if ( m_textDirection.x < 0.0 )
      m_textDirection *= -1.0;
    m_textDirection.normalize();
    m_dTextRot = m_xAxis.angleTo(m_textDirection, OdGeVector3d::kZAxis);
  }
}


bool OdRadialRecomputorEngine::isHorDimLine()
{
  double angl = m_dimLineDir.angleTo(OdGeVector3d::kXAxis);
  angl = ( angl > OdaPI2 ? OdaPI - angl : angl );
  if ( angl < OdaPI/12.0 )
  {
    return true;
  } 
  return false;
}

void OdRadialRecomputorEngine::calcDirectionArrows()
{
  m_dirArrow1 = m_dimLineDir;
  if ( !m_dirArrow1.isZeroLength() )
  {
    m_dirArrow1.normalize();
  }

  if ( m_bArrowsInside )
    m_dirArrow1 *= -1.0;

  m_dirArrow2 = -m_dirArrow1;
  if ( m_bArrow1Flip )
  {
    m_dirArrow1 *= -1.0;
    if ( OdZero(m_asz2) )
      m_dirArrow1.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  }
  if ( m_bArrow2Flip)
  {
    m_dirArrow2 *= -1.0;
    if ( OdZero(m_asz1) )
      m_dirArrow2.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  }
}

bool OdRadialRecomputorEngine::isCenterMark()
{
  if ( m_bArrowsInside )
    return false;
  return true;
}

void OdRadialRecomputorEngine::adjustArrowAndLines(bool tofl, bool bExtArc, double startAngle, double endAngle)
{
  if ( !m_bRadial )
  {
    if ( !m_bArrowsInside  )
    {
      if ( !tofl )
      {
        // m_bTail1 = false;
        m_bTail2 = false;
        m_bArrow2Present = false;
      } else
        m_bArrow2Present = true;
    }  else if ( (m_bTextInside && m_atfit == 3 ) || m_bFarOutsideText )
    {
      m_bArrow2Present = false;
    } 
  } else
  {
    m_bArrow2Present = false;
    if ( bExtArc && m_bTail1 )
    {
      OdGeCircArc2d arc;
      arc.set(OdGePoint2d(m_centerPt.x, m_centerPt.y), m_radius, startAngle, endAngle);
      OdGeRay2d line; 
      OdGeVector3d v = m_centerPt - m_arrow1Pt;
      line.set(OdGePoint2d(m_arrow1Pt.x, m_arrow1Pt.y), OdGeVector2d(v.x, v.y));

      OdGePoint2d p1, p2;
      int numInt;
      bool b = arc.intersectWith(line, numInt, p1, p2);
      if ( b && numInt == 1  )
      {
        if ( !p1.isEqualTo(OdGePoint2d(m_arrow1Pt.x, m_arrow1Pt.y)) )
        {
          v.normalize();
          if ( m_bTextHasLine )
            m_tail1EndPt = m_tail1EndPt + v*(m_radius*2);
          else
            m_tail1StartPt = m_tail1StartPt + v*(m_radius*2);

          m_arrow1Pt += v*(m_radius*2);
          m_bArrowsInside = true;
        }
      }
    }
  }
}

void OdRadialRecomputorEngine::adjustTextAndArrowsPlace(OdInt16 dimMove, OdInt16 fit, OdInt16 tix, bool tofl)
{
  // FIT 0 & 1 the same behavior (arrow only and text with arrow)
  // Best fit 3 text and arrow out
  // 2 - text only
  if ( fit == 3  )
  {
    m_bTextInside = false;
    if ( m_dimToh )
      m_bTextHor = true;
    m_bArrowsInside = m_bTextInside;
  } else
  {
    if ( dimMove == 1 )
    {
      {
        m_bTextInside = (m_dLeaderLen < 0); // ????

        if ( !m_bUseRotate )
        {
          if ( fit != 3 )
          {
            if ( (tix && m_dimTih) || (!tix && m_dimToh) ) 
              m_bTextHor = true;
          } else
          {
            if ( m_dimTih && m_bTextInside )
              m_bTextHor = true;
            else if ( m_dimToh && !m_bTextInside )
              m_bTextHor = true;
          }
        }
      }
      m_bArrowsInside = m_bTextInside;
    } else
    {
      double h = textHeight();
      double w = textWidth();
      double ang = m_dimLineDir.angleTo(OdGeVector3d::kXAxis);
      double dist = w;
      bool bHorDim = OdZero(ang, 1.e-5) || OdZero(OdaPI-ang, 1.e-5);
      bool bCheckArrow = false;
      if ( !bHorDim && ( m_dimTih || m_bUseRotate ) )
      {
        dist = h*sin(ang) + w*cos(ang);
        dist = ::fabs(dist);
        bCheckArrow = true;
      }

      if ( tix )
      {
        m_bTextInside = true;
        // TODO: diametric
        // if ( !m_bTextInside )
        //  m_bArrowsInside = false;
        // else 
        if ( dist > m_radius || ( bCheckArrow && dist + m_asz > m_radius ))
          m_bArrowsInside = false;
        // else if ( m_asz > m_radius )
         // m_bArrowsInside = false;

      } else if ( !m_bRadial )
      {
        m_bArrowsInside = true;
        m_bTextInside = true;
        double w = textWidth(true) + 2.0*m_gap + 2.0*m_asz;
        if ( w > m_radius*2.0 )
        {
          m_bArrowsInside = false;
          m_bTextInside = false;
        }
      } else if ( fit < 3 )
      {
        if ( dist < m_radius )
        {
          m_bArrowsInside = true;
          m_bTextInside = true;
          if ( bCheckArrow && dist+m_asz > m_radius )
            m_bArrowsInside = false;
        } else
        {
          m_bArrowsInside = false;
          m_bTextInside = false;
          if ( fit == 2 && m_asz < m_radius )
            m_bArrowsInside = true;
        }
      }

      if ( OdZero(m_dTextRot) )
      {
        if ( m_bTextInside && m_dimTih )
          m_bTextHor = true;

        if ( !m_bTextInside && m_dimToh )
          m_bTextHor = true;
      }
      //??? if ( m_bTextHor && m_tad == 1 )
      //???  m_tad = 0;
    }
  }

  if ( !m_bRadial )
  {
    if ( ( m_bTextInside && m_bArrowsInside ) || tofl )
      m_bArrow2Present = true;
  }
}

void OdRadialRecomputorEngine::adjustTextLocation(OdInt16 /*dimMove*/, double /*dimExe*/)
{
  calcTextDirection();

  m_dimLines = 0;
  m_arrow1Pt = m_xLine2Pt;
  m_arrow2Pt = m_xLine1Pt;

  OdGeVector3d v = m_dimLineDir;

  if ( m_bTextInside )
  {
    OdGePoint3d p = m_xLine2Pt;
    if ( m_bTextHor || m_bUseRotate && !m_bArrowsInside )
    {
      double dist = m_xLine2Pt.distanceTo(m_xLine1Pt) - m_asz1;
      if ( m_bArrow2Present )
        dist -= m_asz2;
      p -= m_dimLineDir*(dist/2.0 + m_asz1);
    } else
    {
      double dist = m_xLine2Pt.distanceTo(m_xLine1Pt) - ( m_bArrowsInside ? m_asz1 : 0.0);
      if ( m_bArrow2Present )
        dist -= m_asz2;
      p -= m_dimLineDir*(dist/2.0 + ( m_bArrowsInside ? m_asz1 : 0.0));
    }
    m_textPosition = p;
  } else
  {
    double leaderLen = m_dLeaderLen;
    if ( m_dLeaderLen < 2.0 * m_asz ) 
    {
      if ( m_bArrowsInside && !m_bTextInside)
        leaderLen = m_asz;
      else
        leaderLen = 2.0 * m_asz;
      if(OdNonZero(m_dLeaderLen)) 
        leaderLen *= m_dLeaderLen / fabs(m_dLeaderLen); //???
    }

    if ( m_bTextHor || m_bUseRotate )
    {
      OdGeVector3d vDir = m_dimLineDir;
      OdGeMatrix3d m;
      m.setCoordSystem(OdGePoint3d::kOrigin, m_xAxis, m_yAxis, OdGeVector3d::kZAxis);
      vDir.transformBy(m);
      if (m_tad > 0 && OdNegative(vDir.y)) // OdNegative(m_dimLineDir.y)) // < 0.0 )
        leaderLen += textHeight(false);
      m_dogLegPt = m_xLine2Pt + leaderLen * m_dimLineDir; // leaderLen
      m_textPosition = m_dogLegPt;
      double dist = textWidth(false)/2.0 + m_gap;
      if ( !isTextMovedUp() ) // if ( m_tad == 0 || m_bUseRotate )
      {
        dist += m_asz;
      }
      OdGeVector3d dir = m_xAxis;
      if ( OdNonZero(vDir.x) && vDir.x < 0.0 || 
        OdZero(vDir.x) && vDir.y < 0.0 ) // < 0.0 )
        dir *= -1.0;
#if 0
      if ( OdNonZero(m_dogLegPt.x > m_centerPt.x) )
        dir *= (m_dogLegPt.x > m_centerPt.x ? 1.0 : -1.0);
      dir *= (m_dLeaderLen < 0 ? -1.0 : 1.0);
#endif      
      m_textPosition += dir*dist;
    } else
    {
      if(leaderLen > 0.0)
        m_dogLegPt = m_xLine2Pt + (leaderLen + (textWidth(false))/2.0 + m_gap)*v;  
      else
        m_dogLegPt = m_xLine2Pt + (leaderLen - (2.0 * m_asz + 4.0 * m_gap)) * v; // ?
      m_textPosition = m_dogLegPt;
    }
  }

  adjustTextByVert();
  if ( m_bTextInside && m_tad == 0 )
  {
    bool b = m_bArrowsInside;
    if ( adjustArrowPlace2() )
      m_bArrowsInside = b;  
  }
  calcDirectionArrows();
  m_dimOcsTextPosition = m_textPosition;
}

bool OdRadialRecomputorEngine::isTextMovedUp()
{
  if (m_bUseXText && !m_bAddedXText)
    return false;
  if (m_bUseXText && m_tad == 0 && m_bTextHor )
    return true;
  if ( ( m_tix && !m_bTextInside && m_bTextHor && m_tad == 1 && m_dimTih && !m_bUserDefPos) )
    return false;
  if ( (m_tad == 0 && OdZero(m_dimTvp, 1.e-5) ) || m_bUseRotate || ( m_bUserDefPos && m_dimMove == 2 ) )
    return false;
  if ( !m_bTextInside && m_bTextHor && m_tad > 0 )
    return true;
  if ( !m_bTextHor && m_tad > 0 )
    return true;
  if ( m_bTextHor && m_dimMove != 2 && m_tad < 2 && m_atfit == 3 )
    return false;
  if ( m_bTextHor && m_tad > 1 && m_atfit > 1 )
    return true;

  if ( m_bTextHor && m_tad == 1 ) // && m_atfit > 1
  {
    double ang = m_dimLineDir.angleTo(OdGeVector3d::kXAxis);

    if (OdZero(ang, 1.e-5) || OdZero(OdaPI-ang, 1.e-5) )
      return true;
  }

  if ( m_tad > 1 )
    return true;
  if ( (m_tad == 0 && OdNonZero(m_dimTvp, 1.e-5) ) )
    return true;
  return false;
}

double OdRadialRecomputorEngine::distanceToHorText()
{
  double distance;

  m_perpTextDir = (m_xLine2Pt - m_xLine1Pt).perpVector(); // m_dirDimLine.perpVector();
  if ( m_perpTextDir.y < 0.0 || OdZero(m_perpTextDir.y) )
    m_perpTextDir *= -1.0;

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

void OdRadialRecomputorEngine::adjustTextByVert()
{
  // if ( !m_bUseRotate && m_tad > 0 )
  if ( isTextMovedUp() )
  {
    if ( m_bTextHor && m_bTextInside && m_atfit != 3 )
    {
      OdGeVector3d vert = m_dimLineDir.perpVector();
      if ( vert.y < 0.0 ) 
        vert *= -1.0;
      if ( m_tad == 4)
        vert *= -1.0;
      m_textPosition += vert*distanceToHorText(); // distance;
    } else
    {
      OdGeVector3d vert = m_textDirection.perpVector();
      double dist = 0.0;
      if ( OdNonZero(m_dimTvp) && m_tad == 0 )
      {
        dist = textHeight(false)*m_dimTvp;
      } else
      {
        dist = textHeight(false)/2.0 + m_gap;
        if ( m_isBox )
          dist += m_gap;
      }
      if ( m_tad == 4)
        vert *= -1.0;
      m_textPosition += vert*dist;
    }
    m_bTextUpperDimLine = true;
  }
}

void OdRadialRecomputorEngine::makeLines(OdInt16 tofl)
{
  addGapToTextExtents();
  if ( m_bTextInside )
  {
    if ( m_bArrowsInside && m_bTextUpperDimLine || OdZero(textWidth()) )
    {
      m_dimLines++;
      m_xLine1PtEnd = m_xLine1Pt;
      if ( !m_bArrowsInside )
        m_xLine1Pt = m_arrow1Pt;
      else
        m_xLine1Pt = m_xLine2Pt - m_dimLineDir*m_asz2;
    } else
    {
      m_bNeedCalcTextIntersection = true;
      if ( m_bArrowsInside || tofl )
        makeDimLine();
    }
    if ( !m_bArrowsInside )
    {
      m_tail1StartPt = m_arrow1Pt + m_dimLineDir*m_asz1;
      m_tail1EndPt = m_tail1StartPt + m_dimLineDir*m_asz;
      m_bTail1 = true;
    }
  } else
  {
    // TODO: optimization
    double leaderLen = m_asz;
    OdGeVector3d vDir = m_dimLineDir;
    OdGeMatrix3d m;
    m.setCoordSystem(OdGePoint3d::kOrigin, m_xAxis, m_yAxis, OdGeVector3d::kZAxis);
    vDir.transformBy(m);

    if (m_tad > 0 && OdNegative(vDir.y)) // m_dimLineDir.y < 0.0)
      leaderLen += textHeight(false);

    if ( m_bUseRotate || !m_bTextUpperDimLine )
    {
      m_tail1StartPt = m_arrow1Pt + m_dimLineDir*m_asz1;
      m_tail1EndPt = m_tail1StartPt + m_dimLineDir*leaderLen;
      m_bTail1 = true;

      if ( m_bTextHor || m_bUseRotate )
      {
        m_textLinePt1 = m_tail1EndPt;
        if (vDir.x < 0.0  && OdNonZero(vDir.x) ||
          OdZero(vDir.x) && vDir.y < 0.0) // < 0.0 )
          m_textLinePt2 = m_textLinePt1 - m_xAxis*m_asz;
        else
          m_textLinePt2 = m_textLinePt1 + m_xAxis*m_asz;
        m_bTextHasLine = true;
      }
    } else if ( m_bTextHor )
    {
      if ( OdZero(vDir.y) ) // OdZero(m_dimLineDir.y) )
      {
        if ( m_bArrowsInside && !m_bTextInside)
        {
          m_textLinePt1 = m_arrow1Pt;

          m_dimLines++;
          m_xLine1PtEnd = m_xLine1Pt;
          m_xLine1Pt = m_xLine2Pt - m_dimLineDir*m_asz1;
        } else
          m_textLinePt1 = m_arrow1Pt + m_dimLineDir*m_asz1;
        double dist = textWidth(false)+m_gap*2.0+m_asz;
        if (vDir.x < 0.0)
          m_textLinePt2 = m_textLinePt1 - m_xAxis*dist;
        else
          m_textLinePt2 = m_textLinePt1 + m_xAxis*dist;
        m_bTextHasLine = true;
      } else
      {
        if ( m_bArrowsInside && !m_bTextInside)
        {
          m_tail1StartPt = m_arrow1Pt;

          m_dimLines++;
          m_xLine1PtEnd = m_xLine1Pt;
          m_xLine1Pt = m_xLine2Pt - m_dimLineDir*m_asz1;
        } else
          m_tail1StartPt = m_arrow1Pt + m_dimLineDir*m_asz1;
        m_tail1EndPt = m_tail1StartPt + m_dimLineDir*leaderLen; 
        m_bTail1 = true;

        m_textLinePt1 = m_tail1EndPt;
        double dist = textWidth(false)+m_gap;
        if ( m_isBox )
          dist += m_gap;
        if (vDir.x < 0.0  && OdNonZero(vDir.x) ||
          OdZero(vDir.x) && vDir.y < 0.0) // < 0.0 )
          m_textLinePt2 = m_textLinePt1 - m_xAxis*dist;
        else
          m_textLinePt2 = m_textLinePt1 + m_xAxis*dist;
        m_bTextHasLine = true;
      }
    } else // aligned along line
    {
      if ( OdZero(m_asz2) || m_bArrow1Flip )
      {
        m_textLinePt1 = m_arrow1Pt;
        m_textLinePt2 = m_textLinePt1 + m_dimLineDir*(textWidth()+2*m_asz);
      } else
      {
        m_textLinePt1 = m_arrow1Pt + m_dimLineDir*m_asz;
        m_textLinePt2 = m_textLinePt1 + m_dimLineDir*(textWidth()+m_asz);
      }
      m_bTextHasLine = true;

      if ( m_tad == 0 && OdNonZero(m_dimTvp) && OdPositive(0.7 - m_dimTvp) )
      {
        m_tail1StartPt = m_arrow1Pt + m_dimLineDir*m_asz1;
        m_tail1EndPt = m_tail1StartPt + m_dimLineDir*leaderLen;
        m_bTail1 = true;
        m_bTextHasLine = false;
      }
    }
  }
  if ( OdZero(textWidth(false)) )
  {
    if ( m_bTextHasLine && ! m_bTail1)
    {
      m_tail1StartPt = m_arrow1Pt + m_dimLineDir*m_asz1;
      m_tail1EndPt = m_tail1StartPt + m_dimLineDir*m_asz;
      m_bTail1 = true;
    }
    m_bTextHasLine = false;
  }

  if ( m_bArrow2Present && !m_bArrowsInside )
  {
    if ( OdNonZero(m_asz1) )
    {
      m_tail2StartPt = m_arrow2Pt - m_dimLineDir*m_asz2;
      m_tail2EndPt = m_tail2StartPt - m_dimLineDir*m_asz;
      m_bTail2 = true;
    }
  }
}

OdUInt32 OdRadialRecomputorEngine::getIntersectionPoints(OdGePoint3d& p1, OdGePoint3d& p2, bool useDimLine)
{
  OdGePoint3d minPt = m_textExtentsWithGap.minPoint();
  OdGePoint3d maxPt = m_textExtentsWithGap.maxPoint();

  // ACAD uses aligned extents relative m_textPosition
  double extX2 = textWidth()/2.0;
  double extY2 = textHeight()/2.0;

  minPt.x = m_textPosition.x - extX2;
  maxPt.x = m_textPosition.x + extX2;

  minPt.y = m_textPosition.y - extY2;
  maxPt.y = m_textPosition.y + extY2;


  OdGeMatrix2d matr = OdGeMatrix2d::rotation(m_dTextRot, OdGePoint2d(m_textPosition.x, m_textPosition.y));

  OdGePoint3dArray intersections;

  OdGePoint3d pt1, pt2;

  OdGeVector3d v = m_xLine1Pt - m_xLine2Pt;
  if ( useDimLine )
  {
    pt1 = m_arrow1Pt;
    pt2 = m_arrow2Pt;
  } else
  {
    pt1 = m_textPosition - v*(maxPt.x - minPt.x);
    pt2 = m_textPosition + v*(maxPt.x - minPt.x);
  }

  // OdGeLineSeg2d dimLine1(OdGePoint2d(pt1.x, pt1.y), OdGePoint2d(pt2.x, pt2.y));
  OdGeLine2d dimLine1(OdGePoint2d(pt1.x, pt1.y), OdGePoint2d(pt2.x, pt2.y));

  OdGePoint2d intersecPt;

  OdGeLineSeg2d line(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(minPt.x, maxPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }

  line.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, maxPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }

  line.set(OdGePoint2d(maxPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }

  line.set(OdGePoint2d(maxPt.x, minPt.y), OdGePoint2d(minPt.x, minPt.y));
  line.transformBy(matr);
  if ( dimLine1.intersectWith(line, intersecPt) )
  {
    intersections.push_back(OdGePoint3d(intersecPt.x, intersecPt.y, 0.0));
  }
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

double OdRadialRecomputorEngine::lengthTextOnDimLine()
{
  double textLenOnDimLine;
  addGapToTextExtents();

  if ( !m_bTextHor && !m_bUseRotate )
    return textWidth();

  OdGePoint3d p1, p2;
  OdUInt32 numPt = getIntersectionPoints(p1, p2, false);

  if ( numPt == 2 )
  {
    textLenOnDimLine = p1.distanceTo(p2);
  } else
  {
    textLenOnDimLine = 0.0;
  }
  return textLenOnDimLine;
}

void OdRadialRecomputorEngine::adjustUserDefText(OdUInt16 dimMove, double asz, double /*exe*/)
{
  if ( m_atfit < 3 && m_bTextInside && dimMove != 1 )
  {
    return;
  }

  calcTextDirection();

  m_textPosition = m_dimOcsTextPosition;
  m_arrow1Pt = m_xLine2Pt;
  m_arrow2Pt = m_xLine1Pt;

  if ( !m_textExtents.isValidExtents() )
  {
    m_bArrowsInside = m_bTextInside;
    m_dogLegPt = m_textPosition;
    calcDirectionArrows();
    adjustUserDefText0(0.0);
    m_dimOcsTextPosition = m_textPosition;
    return;
  }

  if ( !m_bRadial && !m_bTextInside )
    m_bArrow2Present = true;

  OdUInt16 usedDimMove = dimMove;
  bool bNeedDogLeg = false;

  if ( m_bTextHor || m_bUseRotate )
  {
    if ( dimMove == 1 )
    {
      m_dimOcsTextPosition = m_dogLegPt;
    }
    if ( isHorDimLine() )
    {
      usedDimMove = 1;
    } else
      bNeedDogLeg = true;
  }

  // Correct text position
  OdGeVector3d e_c = m_dimLineDir; 
  e_c = m_dimLineDir;
  e_c *= (m_bTextInside ? -1.0 : 1.0);
  double angl = e_c.angleTo(m_xAxis);
  double dir = !OdNegative(angl - OdaPI2) ? -1.0 : 1.0;
  m_bFarOutsideText = false;
  if ( !m_bTextInside  )
  {
    if ( m_dimOcsTextPosition.distanceTo(m_xLine1Pt) < m_dimOcsTextPosition.distanceTo(m_xLine2Pt) )
    {
      m_bFarOutsideText = true;
      dir *= -1.0;
    }
  }
  if ( usedDimMove == 1 )
  {
    if ( m_bTextHor || m_bUseRotate )
    {
      double distToText = textWidth(false) / 2.0 + m_gap;
      if ( bNeedDogLeg && ( m_tad == 0 || m_bUseRotate || (dimMove == 1 && m_tad == 1 && m_bTextInside)) )
        distToText += m_asz;
      m_textPosition = m_dogLegPt + m_xAxis*dir*distToText;
    }
  } /* else
  {
    if ( m_bTextHor || m_bUseRotate )
    {
      double horDist = textWidth(false) / 2.0 + m_gap;
      if ( bNeedDogLeg && ( m_tad == 0 || m_bUseRotate || (dimMove == 1 && m_tad == 1 && m_bTextInside)) )
        horDist += m_asz;
    }
  } */

  bool bLongDogLeg = false;
  if (isTextMovedUp() || (m_bUseXText && !m_bAddedXText && m_bTextHor && !m_bTextInside)) // m_tad > 0 && !m_bUseRotate && !(m_tad == 1 && m_bTextInside && m_bTextHor) )
  {
    adjustTextByVert();
    // need long dogleg
    if ( m_bTextHor && m_tad != 0 ) // || m_bUseXText )
      bLongDogLeg = true;
    m_bTextUpperDimLine = true;
  }

  double horDist = textWidth(false);
  if ( usedDimMove == 0 )
    horDist /= 2.0;  
  else
    horDist += m_gap;

  if ( m_isBox )
    horDist += m_gap;

  if ( bLongDogLeg )
  {
    m_bTextHasLine = true;
    m_textLinePt1 = m_dogLegPt;
    m_textLinePt2 = m_dimOcsTextPosition + m_xAxis*dir*horDist;
  } else if ( bNeedDogLeg)
  {
    m_bTextHasLine = true;
    m_textLinePt1 = m_dogLegPt;

    if ( usedDimMove == 1 )
    {
      m_textLinePt2 = m_dimOcsTextPosition + m_xAxis*dir*m_asz;

    } else
    {
      double horDist1 = textWidth(false) /2.0 + m_gap;
      if ( m_isBox )
        horDist += m_gap;

      OdGeVector3d v = m_dimOcsTextPosition - m_textLinePt1;
      if ( m_xAxis.isCodirectionalTo(v) )
        m_textLinePt2 = m_dimOcsTextPosition - m_xAxis*horDist1; //  + m_xAxis*dir*m_asz;
      else
        m_textLinePt2 = m_dimOcsTextPosition + m_xAxis*horDist1; //  + m_xAxis*dir*m_asz;

    }
  }

  if ( m_atfit == 2 && dimMove == 0 )
  {
    m_bArrowsInside = false;
    if ( asz < m_radius )
    {
      m_bArrowsInside = true;
    }
  } else
  {
    m_bArrowsInside = m_bTextInside || m_bFarOutsideText || m_bDogLegInside;
  }

  calcDirectionArrows();
  adjustUserDefText0(0.0);
}

void OdRadialRecomputorEngine::adjustUserDefText0(double /*exe*/)
{
  m_dimLines = 0;
  m_arrow1Pt = m_xLine2Pt;
  m_arrow2Pt = m_xLine1Pt;
  if ( !m_textExtents.isValidExtents() )
  {
    m_bTextHasLine = true;
    m_textLinePt1 = m_xLine2Pt + m_dimLineDir*m_asz2;
    m_textLinePt2 = m_dimOcsTextPosition;
    return;
  }

  if ( m_bArrow2Present && !m_bArrowsInside )
  {
    m_tail2StartPt = m_arrow2Pt + m_dirArrow2*m_asz1;
    m_tail2EndPt = m_tail2StartPt + m_dirArrow2*m_asz1;
    m_bTail2 = true;
  }

  if ( m_bTextHasLine )
  {
    m_bTail1 = true;
    m_tail1StartPt = m_textLinePt1;
    if ( m_bTextInside != m_bArrowsInside && !m_bFarOutsideText && !m_bDogLegInside )
    {
      m_tail1EndPt = m_xLine2Pt;
      m_dimLines++;
      m_xLine1PtEnd = m_xLine1Pt;
      m_xLine1Pt = m_xLine2Pt + m_dirArrow1*m_asz2;
    } else
    {
      m_tail1EndPt = m_xLine2Pt + m_dirArrow1*m_asz2;
    }
    return;
  }

  OdGePoint3d point;
  OdGeVector3d vec = m_dimLineDir;
  if ( m_bTextInside || m_bFarOutsideText )
  {
    vec *= -1.0;
  }

  bool bNeedDimLine = false;

  if ( m_bTextInside != m_bArrowsInside )
  {
    m_tail1StartPt = m_xLine2Pt;
    if ( m_dimOcsTextPosition.distanceTo(m_xLine2Pt) < m_dimOcsTextPosition.distanceTo(m_xLine1Pt) )
      bNeedDimLine = true;
  } else
  {
    if ( m_bArrow1Flip )
      m_tail1StartPt = m_xLine2Pt;
    else   
      m_tail1StartPt = m_xLine2Pt + vec*m_asz2;
  }
  m_bTail1 = true;

  double halfWidthText = lengthTextOnDimLine()/2.0;
  if ( m_bTextUpperDimLine || ( m_bUseXText && !m_bAddedXText ) )
  {
    if (isHorDimLine() && (m_tad == 0 || m_tad == 1 && m_bTextInside && m_bTextHor ) )
      m_tail1EndPt = m_dimOcsTextPosition;
    else
    {
      if ( m_tad == 0 && OdNonZero(m_dimTvp) && OdPositive(0.7-m_dimTvp) )
        m_tail1EndPt = m_dimOcsTextPosition - vec*halfWidthText; // m_tail1StartPt + vec*m_asz1;
      else
        m_tail1EndPt = m_dimOcsTextPosition + vec*halfWidthText;
    }
  } else
  {
    m_tail1EndPt = m_dogLegPt; // m_dimOcsTextPosition;
    if ( !m_bTextHor )
      m_tail1EndPt -= vec*halfWidthText;
  }
  
  if ( bNeedDimLine )
  {
    m_dimLines++;
    m_xLine1PtEnd = m_xLine1Pt;
    m_xLine1Pt = m_xLine2Pt + m_dirArrow1*m_asz2;
  }
}

void OdRadialRecomputorEngine::adjustUserDefText2(double /*exe*/)
{
  // width text on dim line
  double halfWidthText = lengthTextOnDimLine()/2.0;
  double distToArrowPt = m_textPosition.distanceTo(m_xLine2Pt);
  double distToCenterPt = m_textPosition.distanceTo(m_xLine1Pt);
  m_arrow1Pt = m_xLine2Pt;
  m_arrow2Pt = m_xLine1Pt;

  m_dimLines = 0;
  if ( !m_textExtents.isValidExtents() )
  {
    if ( !m_bArrowsInside )
    {

      m_tail1StartPt = m_arrow1Pt + m_dimLineDir*m_asz2;
      m_tail1EndPt = m_tail1StartPt + m_dimLineDir*m_asz2;
      m_bTail1 = true;
      if ( m_bArrow2Present )
      {
        m_tail2StartPt = m_arrow2Pt - m_dimLineDir*m_asz1;
        m_tail2EndPt = m_tail2StartPt - m_dimLineDir*m_asz1;
        m_bTail2 = true;
      }
    } else
    {
      m_xLine1PtEnd = m_xLine2Pt - m_dimLineDir*m_asz2;
      if ( m_bArrow2Present )
        m_xLine1Pt += m_dimLineDir*m_asz1;
      m_dimLines++;
    }
    return;
  }

  m_bArrowsInside = true;
  bool bDimLineInsideText = false;
  OdGePoint3d p1;
  OdGePoint3d p2;
  OdUInt32 numItersect = 0;

  if (m_bTextInside)
  {
    numItersect = getIntersectionPoints(p1, p2, true);
    if ( numItersect )
    {

      if ( distToArrowPt < halfWidthText || distToCenterPt < halfWidthText+m_asz )
      {
        m_bNeedCalcTextIntersection = false;
        if ( distToArrowPt < halfWidthText && distToCenterPt < halfWidthText )
        {
          m_bArrowsInside = true;
          bDimLineInsideText = true;
        }
        else
          m_bArrowsInside = false;
      }
    } else
      m_bArrowsInside = true;

  } else
  {
    if ( distToArrowPt < halfWidthText || distToCenterPt < halfWidthText )
    {
      m_bNeedCalcTextIntersection = false;
      if ( distToArrowPt < halfWidthText && distToCenterPt < halfWidthText )
      {
        m_bArrowsInside = true;
        bDimLineInsideText = true;
      }
      else
        m_bArrowsInside = false;
    } 
  }
  if ( bDimLineInsideText )
    m_bNeedCalcTextIntersection = false;
  else
  {
    if ( m_bTextInside )
    {
      if ( m_bArrowsInside )
        m_bNeedCalcTextIntersection = true;
      else
        m_bNeedCalcTextIntersection = false;
    }
    else
      m_bNeedCalcTextIntersection = false;
  }

  calcDirectionArrows();

  if ( m_bArrowsInside )
  {
    bool bFarOutside = false;
    if ( distToArrowPt > m_radius*2.0 )
      bFarOutside = true;
    makeDimLine();
  }  else
  {
    m_tail1StartPt = m_xLine2Pt + m_dirArrow1*m_asz2;
    m_tail1EndPt = m_tail1StartPt + m_dirArrow1*m_asz2;
    m_bTail1 = 1;
    // TODO: diametric
  }
}

void OdRadialRecomputorEngine::textLocation(OdUInt16 dimMove)
{
  m_bTextHor = false;

  OdGeLineSeg2d radialLine(m_xLine1Pt.convert2d(), m_xLine2Pt.convert2d());
  OdGeVector2d perpDimLine = radialLine.direction();
  perpDimLine.rotateBy(OdaPI2);
  OdGeLine2d perpLine(m_dimOcsTextPosition.convert2d(), perpDimLine);

  OdGePoint2d intRerpPt;
  m_bTextInside = perpLine.intersectWith(radialLine,intRerpPt);

  if ( !m_bUseRotate )
  {
    if ( dimMove == 2 ) 
    {
      if ( m_dimTih )
      {
        m_bTextHor = true;
      }
    } else if ( (m_bTextInside && m_dimTih) || (!m_bTextInside && m_dimToh) )
      m_bTextHor = true;
  }
}

OdGeVector3d OdRadialRecomputorEngine::dirToText()
{
  OdGeVector3d e_c;
  e_c = m_dimLineDir;
  e_c *= (m_bTextInside ? -1.0 : 1.0);
  if ( m_bTextHor || m_bUseRotate )
  {
    double angl = e_c.angleTo(m_xAxis);
    double dir = !OdNegative(angl - OdaPI2) ? -1.0 : 1.0;
    e_c *= dir;
  }
  return e_c;
}

void OdRadialRecomputorEngine::dimMove2()
{
  m_arrow1Pt = m_xLine2Pt;
  m_arrow2Pt = m_xLine1Pt;
  calcTextDirection();
  bool bFullDimLine = true;
  if ( !m_textExtents.isValidExtents() )
  {
    m_bArrowsInside = m_bTextInside;
    if ( m_bArrowsInside && m_asz > m_radius || ( !m_bRadial && m_asz*2.0 > m_radius*2.0 ) )
    {
      m_bArrowsInside = false;
    }
    if ( !m_bRadial )
      m_bArrow2Present = true;
    if ( !m_bArrowsInside )
    {
      m_bTail1 = true;
      if ( m_bArrow2Present )
      {
        m_bTail2 = true;
      }
      m_bFullDimLine = false;
    } else
    {
      m_bFullDimLine = true;
    }
    m_bNeedCalcTextIntersection = false;
  } else
  {
    addGapToTextExtents();
    m_textPosition = m_dimOcsTextPosition;
    adjustTextByVert();
    bFullDimLine = true;
    if ( !isTextMovedUp() )
      bFullDimLine = adjustArrowPlace2();

    m_bNeedCalcTextIntersection = true;
    if ( m_bArrowsInside && m_asz > m_radius || ( !m_bRadial && m_asz*2.0 > m_radius*2.0 ) )
    {
      m_bArrowsInside = false;
    }
    if ( !m_bArrowsInside )
    {
      m_bTail1 = true;
      m_bNeedCalcTextIntersection = false;
    } else if ( !m_bTextInside || bFullDimLine )
    {
      m_bFullDimLine = bFullDimLine;
      if ( !m_bTextInside )
        m_bFullDimLine = true;
      m_bNeedCalcTextIntersection = false;
    }
    if ( !m_bRadial ) 
    {
      m_bTail2 = m_bTail1;
      m_bArrow2Present = true;
    }
  }
  calcDirectionArrows();
  makeLines2();
}

bool OdRadialRecomputorEngine::adjustArrowPlace2()
{
  OdGePoint3d p1;
  OdGePoint3d p2;
  OdInt32 numItersect = getIntersectionPoints(p1, p2, true);

  m_bArrowsInside = true;
  if ( numItersect != 2 )
  {
    return true; // full dim line
  }

  double d1_1 = p1.distanceTo(m_xLine1Pt);
  double d2_1 = p2.distanceTo(m_xLine1Pt);

  double d1_2 = p1.distanceTo(m_xLine2Pt);
  double d2_2 = p2.distanceTo(m_xLine2Pt);

  double p1_p2 = p1.distanceTo(p2);
  if ( OdZero(p1_p2 - d1_1 - d2_1) )
  {
    if ( OdZero(p1_p2 - d1_2 - d2_2) )
      return true; // full dim line
    m_bArrowsInside = false;
    return false;
  }
  if ( OdZero(p1_p2 - d1_2 - d2_2) )
  {
    m_bArrowsInside = false;
    return false;
  }
  if ( m_bTextInside ) 
  {
    if ( odmin(d1_1, d2_1) < m_asz )
    {
      m_bArrowsInside = false;
      return false;
    }
    if ( !m_bRadial && odmin(d1_2, d2_2) < m_asz )
    {
      m_bArrowsInside = false;
      return false;
    }
    if ( m_bRadial ) // && odmin(d1_2, d2_2) < m_asz )
    {
       OdGeVector3d v1 = p1 - m_xLine1Pt;
       OdGeVector3d v2 = p1 - m_xLine2Pt;
       if ( v1.isCodirectionalTo(v2) )
       {
         m_bTextInside = false;
         return false;
       }
    }
  }
  return false;
}

void OdRadialRecomputorEngine::makeDimLine()
{
  if ( m_bNeedCalcTextIntersection )
  {
    OdGePoint3d p1;
    OdGePoint3d p2;
    OdInt32 numItersect = getIntersectionPoints(p1, p2, true);
    if ( !numItersect )
    {
      m_xLine1PtEnd = m_xLine2Pt + m_dirArrow1*m_asz2;
      if ( m_bArrow2Present )
        m_xLine1Pt -= m_dirArrow1*m_asz1;
      m_dimLines++;
    } else
    {
      if ( p2.distanceTo(m_xLine1Pt) > p1.distanceTo(m_xLine1Pt) )
      {
        std::swap(p2, p1);
      }
      if ( m_bRadial || p2.distanceTo(m_xLine1Pt) > m_asz )
      {
        m_xLine1PtEnd = p2;
        if ( m_bArrow2Present )
          m_xLine1Pt += m_dimLineDir*m_asz1;
        m_dimLines++;
      }
      if ( !m_bArrowsInside || p1.distanceTo(m_xLine2Pt) > m_asz )
      {
        m_xLine2PtEnd = p1;
        if ( m_bArrowsInside )
        {
          m_xLine2Pt -= m_dimLineDir*m_asz2;
        }
        m_dimLines++;
        if ( m_dimLines == 1 )
        {
           m_xLine1PtEnd = m_xLine2PtEnd;
           m_xLine1Pt = m_xLine2Pt;
        }
      }
    }
  } else
  {
    if ( m_bFullDimLine )
    {
      m_xLine1PtEnd = m_xLine2Pt + m_dirArrow1*m_asz2;
      if ( m_bArrow2Present )
        m_xLine1Pt -= m_dirArrow1*m_asz1;
      m_dimLines++;
    }
  }
}

void OdRadialRecomputorEngine::makeArrowTails()
{
  if ( m_bTail1 )
  {
    if ( OdNonZero(m_asz2) )
    {
      m_tail1StartPt = m_xLine2Pt + m_dirArrow1*m_asz2;
      m_tail1EndPt = m_tail1StartPt + m_dirArrow1*m_asz2;
    } else
    {
      m_bTail1 = false;
    }
  }
  if ( m_bTail2 )
  {
    if ( OdNonZero(m_asz1) )
    {
      m_tail2StartPt = m_arrow2Pt - m_dimLineDir*m_asz1;
      m_tail2EndPt = m_tail2StartPt - m_dimLineDir*m_asz1;
    } else
    {
      m_bTail2 = false;
    }
  }
}

void OdRadialRecomputorEngine::makeLines2()
{
  m_dimLines = 0;
  makeDimLine();
  makeArrowTails();
}

void OdRadialRecomputorEngine::makeTextLine()
{
  m_bTextHasLine = true;
  double widthText = textWidth(false)/2.0;
  if ( m_isBox ) 
    widthText += m_gap;

  m_textLinePt1 = m_xLine2Pt;
  if ( !m_bArrowsInside )
    m_textLinePt1 += m_dimLineDir*m_asz1;

  OdGePoint3d point = m_textPosition;
  OdGeVector3d dir = m_dimLineDir;
  if ( m_bTextHor || m_bUseRotate )
  {
    OdGeLine2d line1(m_dogLegPt.convert2d(), m_xAxis.convert2d());
    OdGeLine2d line2(m_textPosition.convert2d(), m_yAxis.convert2d());
    OdGePoint2d pt;
    line1.intersectWith(line2, pt);
    point.x = pt.x;
    point.y = pt.y;
    dir = point - m_dogLegPt;
    dir.normalize();
  } else
  {
    if ( m_bTextUpperDimLine )
      point = m_dimOcsTextPosition;
  }

  if ( m_bTextUpperDimLine )
  {
    dir *= -1.0;
  }

  if ( (m_bTextHor && isHorDimLine() ) || (!m_bTextHor && !m_bUseRotate && !m_bTextUpperDimLine) )
  {
    m_textLinePt2 = point - dir*widthText;
  }  else if ( m_bTextUpperDimLine && !m_bTextHor )
  {
    m_textLinePt2 = m_dimOcsTextPosition + m_dimLineDir*widthText;
  } else
  {
    m_bTail1 = true;
    m_tail1StartPt = m_xLine2Pt;
    m_tail1EndPt = m_dogLegPt;

    m_textLinePt1 = m_dogLegPt;
    m_textLinePt2 = point - dir*widthText;
  }
}


bool OdRadialRecomputorEngine::calcExtensionArc(double startAngle, double endAngle,
                                                double dimExe, double dimExo)
{
  if ( OdZero(endAngle-startAngle) )
    return false;
  OdGeCircArc2d arc;
  arc.set(OdGePoint2d(m_centerPt.x, m_centerPt.y), m_radius, startAngle, endAngle);
  OdGePoint2d point(m_arrow1Pt.x, m_arrow1Pt.y);
  if ( !arc.isOn(point) )
  {
    OdGePoint2d p1 = arc.startPoint();
    OdGePoint2d p2 = arc.endPoint();

    double exeAng = chordAngle(dimExe, m_radius);
    double exoAng = chordAngle(dimExo, m_radius);
    double distance = 0.0;

    double dirRot = 1.0;

    if ( point.distanceTo(p1) < point.distanceTo(p2) )
    { 
      m_dStartExtAng = arc.startAng();
      distance = point.distanceTo(p1);
      dirRot = -1.0;
    } else
    {
      m_dStartExtAng = arc.endAng();
      distance = point.distanceTo(p2);
    }
    
    m_dEndExtAng = m_dStartExtAng + dirRot*chordAngle(distance, m_radius) + dirRot*exeAng;

    if ( distance > dimExo )
    {
      m_dStartExtAng += dirRot*exoAng;
    }

    if ( m_dStartExtAng > m_dEndExtAng )
    {
      std::swap(m_dStartExtAng, m_dEndExtAng);
    }
    return true;
  }
  return false;
}


void OdRadialRecomputorEngine::applyLeaderLen() // OdUInt16 dimMove)
{
  if ( OdZero(m_dLeaderLen ) )
  {
    return;
  }

  if ( !m_textExtents.isValidExtents() )
  {
    return;
  }

  OdGeVector3d e_c;
  e_c = m_dimLineDir;
  // e_c *= (m_bTextInside ? -1.0 : 1.0); // Text must be outside
  m_dimOcsTextPosition = m_xLine2Pt + e_c*(::fabs(m_dLeaderLen));
  m_dogLegPt = m_dimOcsTextPosition;

  if ( !isHorDimLine() ) 
  {
    double dir = !OdNegative(e_c.angleTo(m_xAxis) - OdaPI2) ? -1.0 : 1.0;
    m_dimOcsTextPosition += m_xAxis*dir*(textWidth()/2.0);
  }
  m_textPosition = m_dimOcsTextPosition;
  m_dogLegPt = m_dimOcsTextPosition;
  m_dLeaderLen = 0.0;
}

void OdRadialRecomputorEngine::adjustFlipArrows()
{
  if ( !m_bArrow1Flip && ( !m_bArrow2Flip || !m_bArrow2Present ) )
    return;
  
  if ( m_bArrow1Flip )
  {
    if ( m_bArrowsInside == m_bTextInside )
    {
      if ( OdNonZero(m_asz1))
      {
        m_flipedTailStartPt = m_arrow1Pt + m_dirArrow1*m_asz1;
        m_flipedTailEndPt = m_flipedTailStartPt + m_dirArrow1*m_asz;
        m_bflipedTail = true;
      }
    }
  }

  if ( m_bArrow2Present && m_bArrow2Flip )
  {
    if ( m_bArrowsInside == m_bTextInside ) 
    {
      if ( OdNonZero(m_asz1))
      {
        m_bflipedTail = true; // no need first tail
        m_flipedTailStartPt = m_arrow2Pt;
        m_flipedTailEndPt = m_flipedTailStartPt - m_dirArrow2*2.0*m_asz;
      }
    }
  }
}
