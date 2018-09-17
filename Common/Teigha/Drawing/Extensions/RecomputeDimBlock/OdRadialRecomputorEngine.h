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

#ifndef __OD_DIM_RADIALRECOMPUTOR_ENGINE__
#define __OD_DIM_RADIALRECOMPUTOR_ENGINE__

#include "TD_PackPush.h"

#include "OdRecomputorEngine.h"

/** \details
  This class implements a Radial Recomputor Engine.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class  OdRadialRecomputorEngine : public OdRecomputorEngine
{
public:
  OdGePoint3d m_centerPt;         // Center point
  double m_radius;                // Radius
  OdGePoint3d m_dogLegPt;         // Dogleg point
  bool m_bRadial;                 // Is radial
  OdGePoint3d m_tail1StartPt;     // Tail 1 startpoint
  OdGePoint3d m_tail1EndPt;       // Tail 1 endpoint
  bool m_bTail1;                  // Has Tail 1.
  bool m_bFullDimLine;            // Full dimension line
  double m_dLeaderLen;            // Leader length
  OdGePoint3d m_tail2StartPt;     // Tail 2 startpoint
  OdGePoint3d m_tail2EndPt;       // Tail 2 endpoint
  bool m_bTail2;                  // Has Tail 2
  
  OdGePoint3d m_flipedTailStartPt;     // Tail 2 startpoint
  OdGePoint3d m_flipedTailEndPt;       // Tail 2 endpoint
  bool m_bflipedTail;                  // Has Tail 2

  OdGeVector3d m_dimLineDir;      // Dimension line direction.
  bool m_bTextUpperDimLine;       // Text is above dimension line.
  bool m_bArrow2Present;          // Arrow 2 present.
  bool m_bFarOutsideText;         // Text is far outside extension lines.

  OdUInt16 m_atfit;               // dimatfit variable
  bool m_tix;                     // dimtix variable
  OdUInt16 m_dimMove;             // dimtmove variable

  double m_dStartExtAng;
  double m_dEndExtAng;

  bool m_bDogLegInside;

  OdRadialRecomputorEngine() : OdRecomputorEngine()
    , m_radius(0.0)
    , m_bRadial(true)
    , m_bTail1(false)
    , m_bFullDimLine(false)
    , m_dLeaderLen(0.0)
    , m_bTail2(false)
    , m_bflipedTail(false)
    , m_bTextUpperDimLine(false)
    , m_bArrow2Present(false)
    , m_bFarOutsideText(false)
    , m_atfit(3)
    , m_tix(false)
    , m_dimMove(0)
    , m_dStartExtAng(0.0)
    , m_dEndExtAng(0.0)
    , m_bDogLegInside(false)
  {
  }
  /** \param dimtmove [in]  DIMTMOVE value.
    \param radial [in]  True if and only if this a radial dimension.
  */
  void preprocessDimPoints(OdUInt16 dimtmove);
  /** \details
    Calculates the direction of the dimension arrows for this RecomputorEngine object.
  */
  void calcDirectionArrows();
  /** \details
    Returns true if and only if a center mark is to be drawn for this RecomputorEngine object.
  */
  bool isCenterMark();

  /** \details
    Calculates the text direction for this RecomputorEngine object.
  */
  void calcTextDirection();
  
  /** \details
    Computes the endpoints of lines for this RecomputorEngine object.
    \param dimtofl [in]  DIMTOFL value.
  */
  void makeLines(OdInt16 dimtofl);
  /** \details
    Returns true if and only if the dimension line is horizontal for this RecomputorEngine object.
  */
  bool isHorDimLine();
  /** \details
    Adjusts the text location for this RecompterEngine object.
    \param dimtmove [in]  DIMTMOVE value.
    \param dimexe [in]  DIMEXE value.
  */
  void adjustTextLocation(OdInt16 dimtmove, double dimexe);
  
  /** \details
    Adjusts the text and arrows placement for this RecomputorEngine object.
    \param dimtmove [in]  DIMTMOVE value.
    \param dimtfit [in]  DIMTFIT value.
    \param dimtix [in]  DIMTIX value.
    \param dimtofl [in]  DIMTOFL value.
  */
  void adjustTextAndArrowsPlace(OdInt16 dimtmove, OdInt16 dimtfit, OdInt16 dimtix, bool dimtofl);

  /** \details
    Adjusts the text vertical position for this RecomputorEngine object.
  */
  void adjustTextByVert();

  void adjustUserDefText(OdUInt16 dimtmove, double dimasz, double dimexe);
  /** \param dimexe [in]  DIMEXE value.
  */
  void adjustUserDefText0(double dimexe);
  /** \param dimexe [in]  DIMEXE value.
  */
  void adjustUserDefText2(double dimexe);
  /** \details
    Computes the intersections between the specified line and the sides of the text box for this RecomputorEngine object. 
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param useDimLine [in]  Use dimension line.
    \remarks
    Returns the number of intersections.

    If useDimLine == true, the extension line will be used for the computations. If useDimLine == false,
    a line connection the extension line origins will be used.
  */
  OdUInt32 getIntersectionPoints(OdGePoint3d& p1, OdGePoint3d& p2, bool useDimLine);
 
  /** \details
    Returns the length of the text on the dimension line for this RecomputorEngine object.
  */
  double lengthTextOnDimLine();
  /** \details
    Adjusts the arrow and placement for this RecomputerEngine object.
    \param dimtofl [in]  DIMTOFL value.
  */
  void adjustArrowAndLines(bool dimtofl, bool bExtArc, double startAngle, double endAngle);
  /** \details
    Calculates the center point for this RecomputerEngine object.
  */
  void calcCenterPoint()
  {
    if ( m_bRadial  )
      m_centerPt = m_xLine1Pt;
    else
      m_centerPt = m_xLine1Pt + (m_xLine2Pt.asVector() - m_xLine1Pt.asVector())/2.0;
  }
  /** \details
    Calculates the radius for this RecomputerEngine object.
  */
  void calcRadius()
  {
    m_radius = m_centerPt.distanceTo(m_xLine2Pt);
  }

  double distanceToHorText();
  bool isTextMovedUp();
  void textLocation(OdUInt16 dimMove);
  void dimMove2();
  bool adjustArrowPlace2();
  void makeLines2();
  OdGeVector3d dirToText();

  void makeDimLine();
  void makeArrowTails();
  void makeTextLine();
  bool calcExtensionArc(double startAngle, double endAngle, double dimExe, double dimExo);
  void applyLeaderLen();
  void adjustFlipArrows();
};

#include "TD_PackPop.h"

#endif
