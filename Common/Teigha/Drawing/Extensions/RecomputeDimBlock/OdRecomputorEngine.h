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

#ifndef __OD_DIM_RECOMPUTOR_ENGINE__
#define __OD_DIM_RECOMPUTOR_ENGINE__

#include "TD_PackPush.h"

#include "Ge/GeExtents3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeLineSeg2d.h"
#include "DbMText.h"

/** \details
  This class is the base class for Dimension Recomputor Engine classes.
  \remarks
  These classes compute dimension geometries. They are used by the OdDimRecomputor classes.
  
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdRecomputorEngine
{
protected:
  OdUInt16 m_just;
  OdUInt16 m_tad;
  OdUInt16 m_dimMove;
  double m_gap;
  bool m_isBox;
  double m_asz1;
  double m_asz2;
  double m_asz;
  bool m_bArrow1Flip;
  bool m_bArrow2Flip;
  bool m_bArrow1Inside;
  bool m_bArrow2Inside;

  bool m_bInspectionLabel;
  bool m_bInspectionRate;
  bool m_bInspectionFrame;

public:
  OdGePoint3d m_xLine1Pt;                 // Extension Line 1 startpoint.
  OdGePoint3d m_xLine2Pt;                 // Extension Line 2 startpoint.

  OdGePoint3d m_xLine1PtEnd;              // Extension Line 1 endpoint.
  OdGePoint3d m_xLine2PtEnd;              // Extension Line 2 endpoint.

  OdGeVector3d m_xLine1Dir;               // Extension Line 1 direction.
  OdGeVector3d m_xLine2Dir;               // Extension Line 2 direction.

  OdGeVector3d m_dirArrow1;               // Dimension Arrow 1 direction.
  OdGeVector3d m_dirArrow2;               // Dimension Arrow 2 direction.

  OdGePoint3d m_arrow1Pt;                 // Dimension Arrow 1 insertion point.
  OdGePoint3d m_arrow2Pt;                 // Dimension Arrow 2 insertion point. 

  bool m_bArrowsInside;                   // Force Arrows inside extension Lines
  bool m_bTextInside;                     // Force Text inside extension Lines

  bool m_isFitTextAndArrows;              // Text and Arrows fit between extension lines.
  bool m_isFitTextWithoutArrows;          // Text without Arrows fit between extension lines.
  bool m_isFitTextWithoutArrowsForBest;   // Text without Arrows fit between extension lines for best fit.
  bool m_isFitArrowsWithoutText;          // Arrows without Text fit between extension lines.

  OdGePoint3d  m_dimOcsTextPosition;      // Initial dimension OCS text position.
  OdGePoint3d  m_textPosition;            // Current dimension OCS text position.
  OdGeVector3d m_textDirection;           // Text direction.
  double m_dTextRot;                      // Text rotation.
  bool m_bUseRotate;                      // Use text rotation.
  OdGeVector3d m_perpTextDir;             // Text direction + 90°. 

  OdGePoint3d m_labelPosition;
  OdGePoint3d m_ratePosition;

  bool m_bNeedCalcTextIntersection;       // Must Check for Text Intersections.
  bool m_bNeedCheckTextFit;               // Must Check Text Fit.

  OdGeExtents3d m_textExtents;            // Text extents.
  OdGeExtents3d m_textExtentsWithGap;     // Text extents  with DIMGAP.

  OdGeExtents3d m_labelExtents;           // Inspection label extents.
  OdGeExtents3d m_rateExtents;            // Inspection rate  extents.

  bool m_bUserDefPos;                     // User-defined Text position.
  OdGePoint3d m_textLinePt1;              // First point of text line.
  OdGePoint3d m_textLinePt2;              // Second point of text line.
  bool m_bTextHasLine;                    // Text has line.

  OdUInt16 m_dimLines;                    // Number of dimension lines.

  OdGePoint3d m_ldrPt1;                   // Text leader point 1
  OdGePoint3d m_ldrPt2;                   // Text leader point 2
  OdGePoint3d m_ldrPt3;                   // Text leader point 3

  bool m_bLeader;                         // Text has leader.
  OdGeVector3d m_xAxis;                   // WCS dimension X-axis
  OdGeVector3d m_yAxis;                   // WCS dimension Y-axis

  bool m_dimTih;                          // DIMTIH value.
  bool m_dimToh;                          // DIMTOH value.
  double m_dimTvp;                        // DIMTVP value.
  bool m_bTextHor;                        // Text is Horizontal.

  bool m_bUseXText;                       // True to use m_textHeight and m_textWidth instead of m_textEntents
                                          // in textHeight() and textWidth().
  bool m_bAddedXText;
  double m_dXTextHeight;
  double m_textHeight;                    // Text height.
  double m_textWidth;                     // Text width.

  OdRecomputorEngine() :
    m_just(0), m_tad(0), m_gap(0.0), m_isBox(false),
    m_asz1(0.0), m_asz2(0.0), m_asz(0.0),
    m_bArrow1Flip(false), m_bArrow2Flip(false),
    m_bArrow1Inside(true), m_bArrow2Inside(true),
    m_bInspectionLabel(false), m_bInspectionRate(false), m_bInspectionFrame(false),
    m_bArrowsInside(true), m_bTextInside(true),
    m_isFitTextAndArrows(false), m_isFitTextWithoutArrows(false), 
    m_isFitTextWithoutArrowsForBest(true), m_isFitArrowsWithoutText(false),
    m_dTextRot(0.0), m_bUseRotate(false),
    m_bNeedCalcTextIntersection(true), m_bNeedCheckTextFit(true),
    m_bUserDefPos(false), m_bTextHasLine(false), m_dimLines(2), m_bLeader(false), 
    m_dimTih(false), m_dimToh(false),
    m_dimTvp(0.0), m_bTextHor(false), m_bUseXText(false), m_bAddedXText(false),
    m_dXTextHeight(0), m_textHeight(0.0), m_textWidth(0.0), m_dimMove(0)
  {
    m_dirArrow1 = OdGeVector3d::kXAxis;
    m_dirArrow2 = OdGeVector3d::kXAxis;
    m_xLine1Dir = OdGeVector3d::kXAxis;
    m_xLine2Dir = OdGeVector3d::kXAxis;
    m_textDirection = OdGeVector3d::kXAxis;
    m_perpTextDir = OdGeVector3d::kYAxis;
    m_xAxis = OdGeVector3d::kXAxis;
    m_yAxis = OdGeVector3d::kYAxis;
  }
  
  /** \details
    Applies the specified transformation matrix to this RecomputorEngine object.
    \param xfm [in]  3D Transformation matrix.
  */
  virtual void tranformBy(const OdGeMatrix3d& xfm);
  
  /** \details
    Sets the specified Arrow Size parameters.
    \param asz1 [in]  First Arrow size.
    \param asz2 [in]  Second Arrow size.
    \param asz [in] Arrow size.
    \param flipArrows [in] Flip Arrow flags.
    
    flipArrows will be a combination of one or more of the following.
    
    <table>
    Value  Description
    0      Flip none.
    1      Flip Arrow 1.
    2      Flip Arrow 2.
    </table>
  */
  virtual void setAszParams(double asz1, double asz2, double asz, bool flipArrow1, bool flipArrow2);
  
  /** \details
    Calculates the extents of the specified MText entity for this RecomputorEngine object.
    
    \param pMText [in]  Pointer to the MText entity.
    \param force [in]  True to force recomputation of the extents.
    
    \remarks
    The extents are computed with the text temporarily aligned with the X-axis.
  */
  virtual void calcTextExtents(OdDbMTextPtr& pMText, bool force = false);

  /** \details
    Calculates the extents of the specified MText entities for this RecomputorEngine object.

    \param pLabel [in]  Pointer to the MText entity.
    \param pRate [in] Pointer to the MText entity.
    \param force [in] True to force recomputation of the extents.

    \remarks
    The extents are computed with the text temporarily aligned with the X-axis.
  */
  virtual void calcInspectionExtents(OdDbMTextPtr& pLabel, OdDbMTextPtr& pRate, bool bForce = false);

  /** \details
    Computes m_textExtentsWithGap for this RecomputorEngine object.
    
    \param addArcSym [in]  If and only if true, adds an Arc Symbol to the extents.
    \remarks
    The default implementation of this function ignores addArcSym.
  */
  virtual void addGapToTextExtents(bool addArcSym = true);

  /** \details
    Returns the width of the rotated text of this RecomputorEngine object.
  */
  virtual double widthRotText();
  /** \details
    Sets the text parameters of this RecomputorEngine object.
    \param dimjust [in]  DIMJUST value.
    \param dimtad [in]  DIMTAD value.
    \param dimgap [in]  DIMGAP value.
  */
  void setTextParams(OdUInt16 dimjust, OdUInt16 dimtad, double dimgap);

  void setInspection(bool bLabel, bool bRate, bool bFrame);

  /** \details
    Applies the specified DIMATFIT value to this RecomputorEngine object.
    \param dimatfit [in]  DIMATFIT value.
  */
  void applyDimTFit(OdUInt32 dimatfit);

  /** \details
    Returns the text height of this RecomputorEngine object.
    \param withGap [in]  If and only if true, adds 2 x DIMGAP to the text height.
  */
  virtual double textHeight(bool withGap = true);

  /** \details
    Returns the text width of this RecomputorEngine object.
    \param withGap [in]  If and only if true, adds 2 x DIMGAP to the text width.
  */
  virtual double textWidth(bool withGap = true);
  
  /** \details
    Adjusts the User-defined Text of this RecomputorEngine object per the specified parameters.
    \param dimtmove [in]  DIMTMOVE value.
    \param dimasz [in]  DIMASZ value.
    \param dimexe [in]  DIMEXE value.
  */
  virtual void adjustUserDefText(OdUInt16 dimtmove, double dimasz, double dimexe);

  /** \details
    Returns the text width of this RecomputorEngine object with inspection.
  */
  double overallWidth();

  /** \details
    Returns the text height of this RecomputorEngine object with inspection.
  */
  double overallHeight();

  bool isBox() { return m_isBox; }
  
  double chordAngle(double chord, double radius);

  OdDbMTextPtr createXText(OdDbMTextPtr pText, OdInt16 dimtmove, bool bUserDefPos, double dTextRot);

  /** \details
    Applies added text to this RecomputorEngine object.
    \param pMText [in]  Pointer to the MText entity.
    \param boundingBox [out]  Receives the corners of the bounding box if and only if dimgap < 0.0
  */
  virtual void applyXText(OdDbMTextPtr pMText, OdArray<OdGePoint3d>& boundingBox);

protected:
  /** \details
    Moves the dimension line with the dimension text.
    \param dimasz [in]  DIMASZ value.
    \remarks
    This function is called by adjustUserDefText() when dimtmove == 0.
    The default implementation of this function does nothing but return.
  */
  virtual void adjustUserDefText0(double dimasz);
  /** \details
    Adds a leader when the dimension text is moved.
    \param dimasz [in]  DIMASZ value.
    \param dimexe [in]  DIMEXE value.
    \remarks
    This function is called by adjustUserDefText() when dimtmove == 1.
    The default implementation of this function does nothing but return.
  */
  virtual void adjustUserDefText1(double dimasz, double dimexe);
  /** \details
    Allows text to be moved freely without a leader.
    \param dimexe [in]  DIMEXE value.
    \remarks
    This function is called by adjustUserDefText() when dimtmove == 2.
    The default implementation of this function does nothing but return.
  */
  virtual void adjustUserDefText2(double dimexe);
  bool isInspection()
  { 
    return m_bInspectionLabel | m_bInspectionRate;
  }
};

#include "TD_PackPop.h"

#endif
