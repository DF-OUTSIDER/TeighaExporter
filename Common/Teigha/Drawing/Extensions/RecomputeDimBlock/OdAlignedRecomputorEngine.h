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

#ifndef __OD_DIM_ALIGNEDRECOMPUTOR_ENGINE__
#define __OD_DIM_ALIGNEDRECOMPUTOR_ENGINE__

#include "TD_PackPush.h"

#include "OdRecomputorEngine.h"

/** \details
  This class implements an Aligned Dimension Recomputor Engine.
  \remarks
  This class computes aligned and rotated dimension geometries. 
  It is used by the OdAlignedDimRecomputor and OdRotatedDimRecomputor classes.
  
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class  OdAlignedRecomputorEngine : public OdRecomputorEngine
{
  double m_textLenOnDimLine;             
  bool m_bCalcTextLenOnDimLine;          

  OdGeVector3d m_leaderDir;              
  OdGePoint3d m_leaderPtOnDimLine;       
  bool m_bDimLineIsHor;                   
  void expandExtLine();
  OdUInt32 getIntersectionPoints(OdGePoint3d& p1, OdGePoint3d& p2, bool useDimLine);
  void makeLeaderAsTextLine(const OdGePoint3d& pt, double distToText, double vertDist);
  void makeLeaderOnDimLine(const OdGePoint3d& startPoint, const OdGePoint3d& secondPoint);
  void calcVerticalDirection();

public:
  OdGePoint3d m_dimLineDefPt;           // Dimension line definition point.
  OdGePoint3d m_middleDimLinePt;        // Dimension line midpoint.

  OdGePoint3d m_dimLineEnd1Pt;          // Dimension line 1 endpoint.
  OdGePoint3d m_dimLineEnd2Pt;          // Dimension line 2 endpoint.

  OdGePoint3d m_dimLineStart1Pt;        // Dimension line 1 start point.
  OdGePoint3d m_dimLineStart2Pt;        // Dimension line 2 start point.

  OdGeVector3d m_dirDimLine;            // Dimension line direction.
  OdGeVector3d m_dirExtLine;            // Extension line direction.

  OdGePoint3d m_tail1StartPt;           // Tail 1 startpoint.
  OdGePoint3d m_tail1EndPt;             // Tail 1 endpoint.
  bool m_bTail1;                        // Enable tail 1

  OdGePoint3d m_tail2StartPt;           // Tail 2 startpoint.
  OdGePoint3d m_tail2EndPt;             // Tail 2 endpoint.
  bool m_bTail2;                        // Enable tail 2

  bool m_bSuppressDimLine;              // Suppress dimension line.
  bool m_bFitTextByHor;                 // Fit text by horizontal 
  double zCoord;                        // Z-coordinate of dimension

  bool m_bAddedDimLine;
  OdGePoint3d m_AddedDimLineStart;
  OdGePoint3d m_AddedDimLineEnd;

  bool m_isTxtPosFarFromArrowPoint;
  bool m_isTxtPosFarFromDimLine;
  OdUInt32 m_dimatfit;
  bool m_bSwapTextPos;

  OdAlignedRecomputorEngine() : OdRecomputorEngine()
    , m_textLenOnDimLine(0.0)
    , m_bCalcTextLenOnDimLine(false)
    , m_bDimLineIsHor(false)
    , m_bTail1(false)
    , m_bTail2(false) 
    , m_bSuppressDimLine(false)
    , m_bFitTextByHor(true)
    , zCoord(0.0)
    , m_bAddedDimLine(false)
    , m_isTxtPosFarFromArrowPoint(false)
    , m_isTxtPosFarFromDimLine(false)
    , m_dimatfit(0)
    , m_bSwapTextPos(false)
  {
    m_leaderDir = OdGeVector3d::kXAxis;
    m_dirDimLine = OdGeVector3d::kXAxis;
    m_dirExtLine = OdGeVector3d::kYAxis;
  }

  void tranformBy(const OdGeMatrix3d& xfm);

  /** \details
    Sets the specified dimension parameters for this RecomputorEngine object.
    \param oblique [in]  Obliquing angle.
    \param dimRotated [in]  True for rotated dimensions, false for aligned.
    \param rotAngle  [in] Rotation angle.
    \remarks
    All angles are expressed in radians.    
  */
  void preprocessData(double oblique, bool dimRotated = false, double rotAngle = 0.0 );

  /** \details
    Determines if the text and/or arrows fit between extension lines for this RecomputorEngine object.
    \param useText [in]  Use text.
    \param useArrows [in]  Use arrows.
    \param useGap [in]  Use DIMGAP.
  */
  void fitTextAndArrows(bool useText, bool useArrows, bool useGap = true);
  /** \details
    Adjusts the text horizontal position for this RecomputorEngine object.
    \param dimexe [in]  DIMEXE value.
  */
  virtual void adjustTextByHor(double dimexe);
  /** \details
    Adjusts the text vertical position for this RecomputorEngine object.
  */
  virtual void adjustTextByVert();
  /** \details
    Calculates the direction of the dimension arrows  for this RecomputorEngine object.
  */
  void calcDirectionArrows();
  /** \details
    Calculates the dimension line points for this RecomputorEngine object.
  */
  void calcDimLinesPoints();
  
  /** \details
    Adds dimension line tails to this RecomputorEngine object.
    \param dimsoxd [in]  DIMSOXD value.
    \param dimdle [in]  DIMDLE value.
  */
  void addDimLineTails(bool dimsoxd, double dimdle);
  /** \details
    Intersects the dimension line with the text box for this RecomputorEngine object.
  */
  void intersectDimLineWithText();
  /** \details
    Moves the text outside the extension lines for this RecomputorEngine object.
    \param dimasz [in]  DIMASZ value.
  */
  void textToOutside(double dimasz);
  /** \details
    Moves the text and arrows outside the extension lines for this RecomputorEngine object.
  */
  void textAndArrowToOutside();
  /** \details
    Moves only the text outside extension lines for this RecomputorEngine object.
  */
  void textOnlyToOutside();
  
  /** \details
    Returns the length of the text on the dimension line for this RecomputorEngine object.
  */
  double lengthTextOnDimLine();
  /** \details
    Returns the length of the text on the dimension line for this RecomputorEngine object.
  */
  double lengthTextOnXDir();
  
  /** \details
    Returns the horizontal distance from the lower-left corner to the middle of the text for this RecomputorEngine object.
  */
  double distanceToHorText();
  /** \details
    Corrects the dimension line points for this RecomputorEngine object.
  */
  void correctDimLinesPoints();
  /** \details
    Applies dimtmove to the default text for this RecomputorEngine object.
    \param dimtmove [in]  DIMTMOVE value.
    \param dimexe [in]  DIMEXE value.
  */
  void applyDimMoveForDefaultPos(OdInt16 dimtmove, double dimexe);

  /** \details
    Adjusts the text and arrows placement for this RecomputorEngine object.
    \param dimtfit [in]  DIMTFIT value.
    \param dimexe [in]  DIMEXE value.
  */
  void adjustTextAndArrowsPlace(OdUInt16 dimtfit, bool dimtix);

  /** \details
    Adjusts the text and arrows placement for this RecomputorEngine object.
    \param dimtmove [in]  DIMTMOVE value.
    \param dimtix [in]  DIMTIX value.
  */
  void adjustTextLocation(OdInt16 dimtmove, double dimexe);
  
  bool makeJogSymbol(OdGePoint3d& jogPoint, double size, OdGePoint3dArray& jogPoints, OdDbMTextPtr addedText, bool bDimLine1 = true, bool bDimLine2 = true );

protected:
  virtual void adjustUserDefText0(double dimasz);
  virtual void adjustUserDefText1(double dimasz, double dimexe);
  virtual void adjustUserDefText2(double dimexe);

  /** \details
    Updates the text parameters for this RecomputorEngine object.
  */
  void testTextLocation0();
  /** \details
    Updates the text parameters for this RecomputorEngine object.
    \param dimexe [in]  DIMEXE value.
    \remarks
    Returns true if and only if there is a leader on the dimension line.
  */
  bool testTextLocation1(double dimexe);

  /** \details
    Updates the text parameters for this RecomputorEngine object.
    \param dimexe [in]  DIMEXE value.
  */
  void testTextLocation2(double dimexe);
  /** \details
    Computes the leader points for this RecomputorEngine object.
    \param ocsTextPosition [in]  OCS text position.
    \param leaderDirection [in]  Leader direction.
  */
   void makeLeader(const OdGePoint3d& ocsTextPosition, const OdGeVector3d& leaderDirection, bool bTvp = false);
  /** \details
    Moves rotated text outside the extension lines of this RecomputorEngine object. 
  */
  void rotTextToOutside();
  /** \details
    Returns the vertical OCS distance of the text from the dimension line for this RecomputorEngine object.
  */
  double verticalDistToText();
  /** \details
    Corrects the extension line points for this RecomputorEngine object.
    \param ocsTextPosition [in]  OCS text position.
  */
  void correctExtLinePoints(const OdGePoint3d& ocsTextPosition);
  /** \details
    Corrects the text direction for this RecomputorEngine object.
  */
  void correctTextDirection();

};

#include "TD_PackPop.h"

#endif
