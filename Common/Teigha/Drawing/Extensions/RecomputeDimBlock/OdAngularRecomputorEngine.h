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

#ifndef __OD_DIM_ANGULARRECOMPUTOR_ENGINE__
#define __OD_DIM_ANGULARRECOMPUTOR_ENGINE__

#include "TD_PackPush.h"

#include "Ge/GeExtents3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeLineSeg2d.h"

#include "OdRecomputorEngine.h"

#include "DbMText.h"

/** \details
  This class is the base class for Angular RecomputorEngine classes.
  \remarks
  This class computes aligned and rotated dimension geometries. 
  
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdAngularRecomputorEngine : public OdRecomputorEngine
{

public:
  OdGePoint3d m_centerPtArc;        // Center of the Source Arc.
  OdGePoint3d m_centerDimPtArc;     // Center of the Dimension Arc.
  OdGePoint3d m_arcPt;              // Chord point of the Dimension Arc.
  OdGePoint3d m_middleArcPt;        // Midpoint of the Dimension Arc.

  OdGeVector3d m_dirToMiddleArcPt;  // Direction from the center of the Dimension Arc to its midpoint.

  double m_extLine1Angle;           // OCS angle of Extension Line 1.
  double m_extLine2Angle;           // OCS angle of Extension Line 2.

  double m_dimArc1StartAngle;       // OCS start angle of Dimension Arc 1.
  double m_dimArc2StartAngle;       // OCS start angle of Dimension Arc 2.

  double m_dimArc1EndAngle;         // OCS end angle of Dimension Arc 1.
  double m_dimArc2EndAngle;         // OCS end angle of Dimension Arc 2.

  double m_angle;                   // Angle from the Extension line 1 to Extension Line 2.
  double m_radiusDimArc;            // Radius of the Dimension Arc.
  double m_sourceArcRadius;         // Radius of the Source Arc.

  double m_StartTextAngle;          // Text start angle.
  double m_EndTextAngle;            // Text end angle.

  double m_Arrow1Angle;             // Angle subtended by Arrow 1
  double m_Arrow2Angle;             // Angle subtended by Arrow 2
  double m_gapAngle;                // Arrow subtended by Gap

  OdGeCircArc2d m_addedArrow1Arc;   // Arc added to Arrow 1.
  OdGeCircArc2d m_addedArrow2Arc;   // Arc added to Arrow 2.
  bool m_bAddedArrow1Arc;           // Arc has been added to Arrow 1.
  bool m_bAddedArrow2Arc;           // Arc has been added to Arrow 2.

  double m_dStartExtAng;
  double m_dEndExtAng;

  OdAngularRecomputorEngine() : OdRecomputorEngine(),
    m_extLine1Angle(0.0), m_extLine2Angle(0.0),
    m_dimArc1StartAngle(0.0), m_dimArc2StartAngle(0.0),
    m_dimArc1EndAngle(0.0), m_dimArc2EndAngle(0.0),
    m_angle(0), m_radiusDimArc(0.0), m_sourceArcRadius(0.0), 
    m_StartTextAngle(0.0), m_EndTextAngle(0.0),
    m_Arrow1Angle(0.0), m_Arrow2Angle(0.0), m_gapAngle(0.0),
    m_bAddedArrow1Arc(false), m_bAddedArrow2Arc(false),
    m_dStartExtAng(0.0), m_dEndExtAng(0.0)
  {
    m_dirToMiddleArcPt = OdGeVector3d::kYAxis;
  }

protected:
  /** \details
    Returns the angle subtended by a chord of the specified length.
    \param chordLength [in]  Chord length.
    \note
    All angles are expressed in radians.
  */
  double chordAngle(double chordLength);
  /** \details
    Corrects the text direction for this RecomputorEngine object.
  */
  void correctTextDir();
  
  virtual void adjustUserDefText0(double dimasz);
  virtual void adjustUserDefText1(double dimasz, double dimexe);
  virtual void adjustUserDefText2(double dimexe);
  /** \details
    Moves the text outside the extension lines for this RecomputorEngine object.
    \param dimasz [in]  DIMASZ value.
    \param rotAngle [in]  Rotation angle. 
  */
  void textToOutside(double dimasz, double rotAngle);
  /** \details
    Adds an underline to this RecomputorEngine object.
  */
  void addUnderline();
  
  /** \details
    Applies dimtmove to the default text for this RecomputorEngine object.
    \param dimtmove [in]  DIMTMOVE value.
  */
  void applyDimMoveForDefaultPos(OdUInt16 dimtmove);
  
  /** \details
    Returns the vector from the center of the Dimension Arc to its midpoint for this RecomputorEngine object.
  */
  virtual OdGeVector3d getMiddleVector()
  { 
    return m_dirToMiddleArcPt;
  }

  void intersectTextBoxWithExtDimArc(OdGePoint3dArray& intersections);

public:
  /** \details
    Swaps the extension lines if the chord point for RecomputorEngine object is outside the extension lines.
  */
  virtual void testDimExtLineOrder();
  /** \details
    Returns the measurement for this RecomputorEngine object.
  */
  virtual double measurement();

  void tranformBy(const OdGeMatrix3d& xfm);

  void setAszParams(double asz1, double asz2, double asz, bool flipArrow1, bool flipArrow2);
  
  /** \details
    Calculates the Dimension Arc Point for this RecomputorEngine object.
  */
  void calcDimArcPoint();
  /** \details
    Calculates the Extension Line Points for this RecomputorEngine object
  */
  virtual void calcExtLinePoints();
  /** \details
    Calculates the Extension Line Arrows for this RecomputorEngine object
  */
  void calcExtLineAngles();
  /** \details
    Calculates the Arrow directions for this RecomputorEngine object
  */
  void calcDirectionArrows();
  /** \details
    Calculates the Extension Line directions for this RecomputorEngine object
  */
  virtual void calcDimExtLineDir();

  /** \details
    Intersects the Dimension Arc with the text box for this RecomputorEngine object.
    \param addArcSym [in]  Add arc symbol to text.
  */
  void intersectTextBoxWithDimArc(bool addArcSym = true);
  /** \details
    Returns true if and only if the text box intersects the Dimension Arc between the
    specified points.
    \param dimArcPt1 [in]  Dimension Arc Point 1.
    \param dimArcPt2 [in]  Dimension Arc Point 2.
  */
  bool isTextIn(const OdGePoint3d dimArcPt1, const OdGePoint3d dimArcPt2);

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
    Moves the text and arrows outside the extension lines for this RecomputorEngine object.
  */
  void textAndArrowToOutside();

  /** \details
    Moves only the text outside extension lines for this RecomputorEngine object.
  */
  void textOnlyToOutside();

  /** \details
    Adds Dimension Arc Tails to this RecomputorEngine object.
    \param dimasz [in]  DIMASZ value.
    \param dimsoxd [in]  DIMSOXD value.
  */
  void addTailDimLineArc(double dimasz, bool dimsoxd);

  /** \details
    Calculates the text angles for this RecomputorEngine object.
  */
  void calcTextAngles();
  /** \details
    Adjusts the starting and ending dimension angles for the arrows for this RecomputorEngine object.
  */
  void prepareDimArc();
  /** \details
    Swaps the extension lines if the dimension arc would be clockwise for this RecomputorEngine object. 
  */
  void correctDimArcPoint();
  
  // void adjustUserDefText(OdUInt16 dimMove, double asz);

  /** \details
    Applies the flipArrows flag to this RecomputorEngine object.
  */
  void applyFlipArrows();
  /** \details
    Adjusts the text and arrows placement for this RecomputerEngine object.
    \param dimtfit [in]  DIMTFIT value.
    \param dimtix [in]  DIMTIX value.
  */
  void adjustTextAndArrowsPlace(OdUInt16 dimtfit, bool dimtix);
  /** \details
    Adjusts the text location for this RecompterEngine object.
    \param dimtmove [in]  DIMTMOVE value.
    \param dimexe [in]  DIMEXE value.
  */
  void adjustTextLocation(OdInt16 dimtmove, double dimexe);

  bool makeExtensionArc(OdUInt16 dimtmove);
};

/** \details
  This class implements a 3-Point Angular Recomputor Engine.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class Od3PointAngularRecomputorEngine : public OdAngularRecomputorEngine
{
public:

  Od3PointAngularRecomputorEngine() : OdAngularRecomputorEngine()
  {
  }
public:
  virtual void calcExtLinePoints();
  virtual void calcDimExtLineDir();
  void adjustTextByHor(double dimexe);
  virtual double measurement();
};

/** \details
  This class implements an Arc Length Dimension Recomputor Engine.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class OdArcLenRecomputorEngine : public OdAngularRecomputorEngine
{
public:
  OdUInt16 m_arcSym;           
  OdGeCircArc2d m_arcInText;

  OdArcLenRecomputorEngine() : OdAngularRecomputorEngine(),  m_arcSym(2)
  {
  }

protected:
  virtual OdGeVector3d getMiddleVector();

public:

  virtual void calcExtLinePoints();
  virtual void calcDimExtLineDir();
  virtual void adjustTextByHor(double dimexe);
  virtual void calcTextExtents(OdDbMTextPtr& pMText, bool force = false);
  virtual void calcInspectionExtents(OdDbMTextPtr& pLabel, OdDbMTextPtr& pRate, bool bForce = false);
  virtual void addGapToTextExtents(bool addArcSym = true);
  virtual double measurement();

  bool isUserDefTextInside();
  void checkArrowsPlace();
  /** \details
    Applies the Arc parameters to this RecomputorEngine object.
  */
  void applyArcParam();
  /** \details
    Fits the Arc parameters to this RecomputorEngine object.
    \param pMText [in]  Pointer the pMText object.
  */
  void fitArcParam(OdDbMTextPtr pMText);
};

/** \details
  This class implements a 2-Line Angular Dimension Recomputor Engine.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class Od2LineAngularRecomputorEngine : public Od3PointAngularRecomputorEngine
{
public:
  bool m_bExtLine1Enabled;
  bool m_bExtLine2Enabled;
  bool m_bSwaped;
  Od2LineAngularRecomputorEngine() : Od3PointAngularRecomputorEngine()
  {
    m_bExtLine1Enabled = true;
    m_bExtLine2Enabled = true;
    m_bSwaped = false;
  }

public:

  /** \details
    Calculates the dimension points for this RecomputorEngine object.
  */
  void calcDimPoints();
  void calcDimExtLineDir();
  void adjustTextByHor(double dimexe);
  // virtual double measurement();
};

#include "TD_PackPop.h"

#endif
