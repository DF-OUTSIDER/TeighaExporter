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
#include "OdRecomputorEngine.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

#ifdef _DEBUG
#define DEBUG_RECOMPUTOR
#endif

void OdRecomputorEngine::setTextParams(OdUInt16 just, OdUInt16 tad, double gap)
{
  m_just = just;
  m_tad = tad; 
  m_isBox = ( gap < 0.0 ) ? true : false;
  m_gap = ::fabs(gap);
}

void OdRecomputorEngine::setInspection(bool bLabel, bool bRate, bool bFrame)
{
  m_isBox = bFrame || m_isBox;
  m_bInspectionFrame = bFrame;
  m_bInspectionLabel = bLabel;
  m_bInspectionRate = bRate;
}

void OdRecomputorEngine::tranformBy(const OdGeMatrix3d& matr)
{
  m_xLine1Pt.transformBy(matr);
  m_xLine2Pt.transformBy(matr);
  m_xLine1Pt.z = m_xLine2Pt.z = 0.0;
}

void OdRecomputorEngine::setAszParams(double asz1, double asz2, double asz, bool flipArrow1, bool flipArrow2)
{
  m_asz1 = asz1;
  m_asz2 = asz2;
  m_asz = asz;
  m_bArrow1Flip = flipArrow1;
  m_bArrow2Flip = flipArrow2;
}

void OdRecomputorEngine::calcTextExtents(OdDbMTextPtr& pText, bool bForce)
{
  //  Result: 
  //    m_textExtents of horizontal pText
  if ( !bForce && m_textExtents.isValidExtents() )
    return;

  if ( pText.isNull() )
  {
    m_textExtents.set(m_textPosition, m_textPosition);
    return;
  }

  OdGeVector3d dir = pText->direction();
  pText->setDirection(OdGeVector3d::kXAxis);
  pText->getGeomExtents(m_textExtents);
  pText->setDirection(dir);
}

void OdRecomputorEngine::calcInspectionExtents(OdDbMTextPtr& pLabel, OdDbMTextPtr& pRate, bool bForce)
{
  if ( !bForce && m_labelExtents.isValidExtents() )
    return;

  if ( pLabel.isNull() )
  {
    m_labelExtents.set(m_textPosition, m_textPosition);
  } else 
  {
    OdGeVector3d dir = pLabel->direction();
    pLabel->setDirection(OdGeVector3d::kXAxis);
    pLabel->getGeomExtents(m_labelExtents);
    pLabel->setDirection(dir);
  }

  if ( !bForce && m_rateExtents.isValidExtents() )
    return;

  if ( pRate.isNull() )
  {
    m_rateExtents.set(m_textPosition, m_textPosition);
  } else 
  {
    OdGeVector3d dir = pRate->direction();
    pRate->setDirection(OdGeVector3d::kXAxis);
    pRate->getGeomExtents(m_rateExtents);
    pRate->setDirection(dir);
  }
}

void OdRecomputorEngine::addGapToTextExtents(bool /*bAddArcSym*/)
{
  // Input:
  //    m_textExtents, m_arcInText, there are horizontal text extents
  // Output:
  //    m_textExtentsWithGap
  // 
  OdGePoint3d minPt = m_textPosition;
  OdGePoint3d maxPt = m_textPosition;
  if ( m_bInspectionLabel || m_bInspectionRate )
  {
    double width = overallWidth();

    double height = overallHeight();

    minPt.x -= (width/2.0 + m_gap);
    minPt.y -= (height/2.0 + m_gap);

    maxPt.x += (width/2.0 + m_gap);
    maxPt.y += (height/2.0 + m_gap);

    if ( m_bInspectionFrame )
    {
      minPt.x -= height/2.0 ;
      maxPt.x += height/2.0;
    }

    m_textExtentsWithGap.set(minPt, maxPt);
    return;
  }

  minPt = m_textExtents.minPoint();
  maxPt = m_textExtents.maxPoint();

  minPt.x -= m_gap;
  minPt.y -= m_gap;
  maxPt.x += m_gap;
  maxPt.y += m_gap;
  m_textExtentsWithGap.set(minPt, maxPt);
}

void OdRecomputorEngine::applyDimTFit(OdUInt32 Dimatfit)
{
  m_bArrowsInside = true;
  m_bTextInside = true;
  if(!m_isFitTextAndArrows)
  {
    switch(Dimatfit)
    {
    case 0: // Places both text and arrows outside extension lines
      m_bArrowsInside = false;
      m_bTextInside = false;
      break;
    case 1: // Moves arrows first, then text
      if ( m_isFitArrowsWithoutText && !m_isFitTextWithoutArrows )
      {
        m_bTextInside = false;
      }

      if ( !m_isFitArrowsWithoutText && m_isFitTextWithoutArrows )
      {
        m_bArrowsInside = false;
      }

      if ( !m_isFitArrowsWithoutText && !m_isFitTextWithoutArrows )
      {
        m_bArrowsInside = false;
        m_bTextInside = false;
      }

      if ( (m_isFitArrowsWithoutText && m_isFitTextWithoutArrows) || !m_bTextInside)
      {
        m_bArrowsInside = false;
      }
      break;
    case 2: // Moves text first, then arrows
      if ( m_isFitArrowsWithoutText && !m_isFitTextWithoutArrows )
      {
        m_bTextInside = false;
      }
      if ( !m_isFitArrowsWithoutText && m_isFitTextWithoutArrows )
      {
        m_bArrowsInside = false;
      }
      if ( !m_isFitArrowsWithoutText && !m_isFitTextWithoutArrows )
      {
        m_bArrowsInside = false;
        m_bTextInside = false;
      }
      if ( (m_isFitArrowsWithoutText && m_isFitTextWithoutArrows) || !m_bArrowsInside )
      {
        m_bTextInside = false;
      }
      break;
    case 3: // Moves either text or arrows, whichever fits best
      if ( m_isFitArrowsWithoutText && !m_isFitTextWithoutArrows )
      {
        m_bTextInside = false;
      }
      if ( !m_isFitArrowsWithoutText && m_isFitTextWithoutArrows )
      {
        m_bArrowsInside = false;
      }
      if ( !m_isFitArrowsWithoutText && !m_isFitTextWithoutArrows )
      {
        m_bArrowsInside = false;
        m_bTextInside = false;
      }

      if ( m_isFitArrowsWithoutText && m_isFitTextWithoutArrows )
      {
        if ( m_isFitTextWithoutArrowsForBest )
        {
          m_bArrowsInside = false;
          m_bTextInside = true;
        } else
        {
          m_bArrowsInside = true;
          m_bTextInside = false;
        }
        // What is OD_T("Fit best")????
        /* if(2.0 * arrowAng > dimLineEnd1Vector.angleTo(dimLineEnd2Vector))
        {
        textOutside = true;
        }
        else
        {
        arrowsOut = true;
        }
        */
      }
      break;
    }
  }
  if ( !m_bTextInside )
    m_dimLines = 1;

} // End fitTextAndArrows

double OdRecomputorEngine::widthRotText()
{
  OdGePoint3d maxPt = m_textExtentsWithGap.maxPoint();
  OdGePoint3d minPt = m_textExtentsWithGap.minPoint();

  OdGePoint2d p1(minPt.x, minPt.y);
  OdGePoint2d p2(maxPt.x, minPt.y);
  OdGePoint2d p3(minPt.x, maxPt.y);
  OdGePoint2d p4(maxPt.x, maxPt.y);

  OdGeVector3d chorda = m_xLine1PtEnd-m_xLine2PtEnd;
  double angle = chorda.angleTo(OdGeVector3d::kXAxis, OdGeVector3d::kZAxis);
  angle += m_dTextRot;

  p1.rotateBy(angle, OdGePoint2d(m_textPosition.x, m_textPosition.y));
  p2.rotateBy(angle, OdGePoint2d(m_textPosition.x, m_textPosition.y));
  p3.rotateBy(angle, OdGePoint2d(m_textPosition.x, m_textPosition.y));
  p4.rotateBy(angle, OdGePoint2d(m_textPosition.x, m_textPosition.y));

#ifdef DEBUG_RECOMPUTOR
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", p1.x, p1.y);
  ODA_TRACE2("%f,%f,0.0\n\n", p2.x, p2.y);
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", p3.x, p3.y);
  ODA_TRACE2("%f,%f,0.0\n\n", p4.x, p4.y);
#endif

  double minX, maxX;
  minX = odmin(odmin(odmin(p1.x, p2.x), p3.x), p4.x);
  maxX = odmax(odmax(odmax(p1.x, p2.x), p3.x), p4.x);
  return maxX - minX;
}

void OdRecomputorEngine::adjustUserDefText(OdUInt16 dimMove, double asz, double exe)
{
  m_dimMove = dimMove;
  switch ( dimMove )
  {
  case 0: // 0  Moves the dimension line with dimension text
    adjustUserDefText0(asz);
    break;
  case 1: // 1  Adds a leader when dimension text is moved
    adjustUserDefText1(asz, exe);
    break;
  case 2: // 2  Allows text to be moved freely without a leader
    adjustUserDefText2(exe);
    break;
  }
}

double OdRecomputorEngine::textHeight(bool withGap)
{
  if ( m_bUseXText )
  {
    if ( withGap )
      return m_textHeight + 2.0*m_gap;
    else
      return m_textHeight;
  }

  if ( m_bInspectionLabel || m_bInspectionRate || m_bInspectionFrame )
  {
    if ( withGap )
      return overallHeight() + 2.0*m_gap;
    else
      return overallHeight();
  } else if ( m_textExtents.isValidExtents() )
  {
    if ( withGap )
      return (m_textExtents.maxPoint().y - m_textExtents.minPoint().y) + 2.0*m_gap;
    else
      return (m_textExtents.maxPoint().y - m_textExtents.minPoint().y);
  }
  return 0.0;

}

double OdRecomputorEngine::textWidth(bool withGap)
{
  if ( m_bUseXText )
  {
    if ( withGap )
      return m_textWidth + 2.0*m_gap;
    else
      return m_textWidth;
  }

  if ( m_bInspectionLabel || m_bInspectionRate || m_bInspectionFrame )
  {
    if ( m_bInspectionFrame )
      return overallWidth() + overallHeight() + ( withGap ? 2.0*m_gap : 0.0);
    else 
      return overallWidth() + ( withGap ? 2.0*m_gap : 0.0);
  } else if ( m_textExtents.isValidExtents() )
  {
    if ( withGap )
    {
      if ( m_textExtentsWithGap.isValidExtents() )
        return m_textExtentsWithGap.maxPoint().x - m_textExtentsWithGap.minPoint().x; 
      else
        return (m_textExtents.maxPoint().x - m_textExtents.minPoint().x) + 2.0*m_gap;
    } else
      return (m_textExtents.maxPoint().x - m_textExtents.minPoint().x);
  }
  return 0.0;
}

void OdRecomputorEngine::adjustUserDefText0(double /* dimasz */){}
void OdRecomputorEngine::adjustUserDefText1(double /* dimasz */, double /* dimexe */){}
void OdRecomputorEngine::adjustUserDefText2(double /* dimexe */){}

double OdRecomputorEngine::overallWidth()
{
  double textWidth = m_textExtents.maxPoint().x - m_textExtents.minPoint().x;
  double labelWidth = m_labelExtents.maxPoint().x - m_labelExtents.minPoint().x;
  double rateWidth = m_rateExtents.maxPoint().x - m_rateExtents.minPoint().x;

  double overallWidth = 0.0;
  if ( m_textExtents.isValidExtents() )
  {
    overallWidth += textWidth;
  }
  if ( m_labelExtents.isValidExtents() )
  {
    overallWidth += labelWidth;
  }
  if ( m_rateExtents.isValidExtents() )
  {
    overallWidth += rateWidth;
  }
  if ( m_bInspectionLabel )
  {
    overallWidth += m_gap*2.0;
  }
  if ( m_bInspectionRate )
  {
    overallWidth += m_gap*2.0;
  }
  return overallWidth;
}

double OdRecomputorEngine::overallHeight()
{
  double textHeight = m_textExtents.maxPoint().y - m_textExtents.minPoint().y;
  double labelHeight = m_labelExtents.maxPoint().y - m_labelExtents.minPoint().y;
  double rateHeight = m_rateExtents.maxPoint().y - m_rateExtents.minPoint().y;

  return (odmax(textHeight, odmax(labelHeight, rateHeight)));
}

double OdRecomputorEngine::chordAngle(double chord, double radius)
{
  if ( OdZero(radius) ) // prevent division by zero
    return OdaPI;
  double acosArg  = (1.0 - chord*chord / (2.0 * radius * radius) );
  // these are incorrect cases - arrows are larger then radius
  if(acosArg > 1.0)
  {
    acosArg = 1.0 - 1.0e-10;
  }
  if(acosArg < -1.0)
  {
    acosArg = -1.0 + 1.0e-10;
  }
  return OD_ACOS(acosArg);
}

OdDbMTextPtr OdRecomputorEngine::createXText(OdDbMTextPtr pText, OdInt16 dimtmove, bool bUserDefPos, double dTextRot)
{
  OdDbMTextPtr pAddedText;
  OdString text = pText->contents();
  int xPos = -1;
  int cPos = -1;
  do
  {
    cPos = text.find(OD_T("\\X"), xPos + 1);
    if (cPos != -1)
      xPos = cPos;
  } while (cPos != -1);
  if (xPos != -1)
  {
    if ( !bUserDefPos && OdZero(dTextRot) )
    {
      OdString tmpStr = text;
      OdString addedText;
      int len = text.getLength();
      if (len > xPos+2 && text.getAt(xPos + 2) == ';') // End of tolerance
      {
        text = text.left(xPos);
        text += ';';
        addedText = tmpStr.right(tmpStr.getLength() - xPos - 3);
      }
      else
      {
        text = text.left(xPos);
        addedText = tmpStr.right(tmpStr.getLength() - xPos - 2);
      }
      addedText.trimLeft();
      text.replace(OD_T("\\X"), OD_T(""));
      pText->setContents(text);
      if (!addedText.isEmpty())
      {
        pAddedText = OdDbMText::createObject();
        pAddedText->copyFrom(pText);
        pAddedText->setContents(addedText);
        m_bUseXText = true;
        m_bAddedXText = true;
      }
    }
    else
    {
      int len = text.getLength();
      if (len > xPos+2 && text.getAt(xPos + 2) == ';') // End of tolerance
      {
        OdString tText = text;
        text = tText.left(xPos);
        text += OdString(OD_T(";\\P"));
        text += tText.right(tText.getLength() - xPos - 3);
      }
      else
      {
        text.setAt(xPos + 1, 'P');
      }
      text.replace(OD_T("\\X"), OD_T(""));
      pText->setContents(text);
      if (bUserDefPos && dimtmove == 0)
        m_bUseXText = true;
    }
  }
  return pAddedText;
}

void OdRecomputorEngine::applyXText(OdDbMTextPtr pText, OdArray<OdGePoint3d>& points)
{
  OdGeVector3d v = m_textDirection.perpVector();
  v.normalize();
  v *= -1.0;

  double h1 = textHeight(false) / 2.0;

  OdGeExtents3d exts;
  pText->setDirection(OdGeVector3d::kXAxis);
  pText->getGeomExtents(exts);

  double h2 = (exts.maxPoint().y - exts.minPoint().y) / 2.0;
  double dXTextWidth = (exts.maxPoint().x - exts.minPoint().x);
  OdGePoint3d txtPos = m_textPosition;
  // double added = m_tad == 0 ? m_gap*1.5 : m_gap*2.0;
  txtPos += v*(h1 + h2 + m_gap*2.0);
  if (m_bLeader && m_textWidth > dXTextWidth)
  {
    txtPos += m_textDirection * (dXTextWidth / 2.0 - m_textWidth / 2.0);
  }

  pText->setLocation(txtPos);
  pText->setDirection(m_textDirection);

  if (m_isBox)
  {
    points.resize(4);

    points[0] = txtPos + v*(h2 + m_gap);
    points[0] -= m_textDirection*textWidth() / 2.0;

    points[1] = points[0] - v*((h1 + h2 + m_gap*2.0)*2.0);

    points[2] = points[1] + m_textDirection*textWidth();
    points[3] = points[0] + m_textDirection*textWidth();
  }
}
