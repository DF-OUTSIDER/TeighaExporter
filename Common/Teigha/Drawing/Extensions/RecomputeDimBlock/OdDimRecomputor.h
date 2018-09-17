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


#ifndef __OD_DIM_RECOMPUTOR__
#define __OD_DIM_RECOMPUTOR__


#include "DbDimension.h"
#include "OdSetLocale.h"
#include "Ge/GePoint2dArray.h"

#include "TD_PackPush.h"
class OdRecomputorEngine;
/** \details
  This class is the base class for Dimension Block Recomputor classes.

  \remarks
  The entities comprising a dimension block are constructed
  from the parameters of a dimension and a set of dimension variables.  
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDimRecomputor : public OdRxObject
{
public:
  /** \details
    Retrieves the parameters for this DimRecomputer object from the specified Dimension entity.
    \param pDimension [in]  Pointer to the dimension entity.
  */
  virtual void getDimParams(const OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);
  /** \details
    Updates the parameters of the specified Dimension entity from this DimRecomputer object.
    \param pDimension [in]  Pointer to the dimension entity.
  */
  virtual void setDimParams(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) const;

  /** \details
    Builds a dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.

    \note
    This function is called by recomputeDimBlock().
  */
  virtual void buildDimension(OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars) = 0;

  /** \details
    Preprocesses the dimension points using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \note
    This function is called by recomputeDimBlock().
  */
  virtual void preprocessDimPoints(OdDbDimStyleTableRecord* pDimVars) = 0;

  /** \details
    Updates the measurement value using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \note
    This function is called by recomputeDimBlock().
  */
  virtual void updateMeasurement(OdDbDimStyleTableRecord* pDimVars) = 0;
  /** \details
    Formats the specified measurement using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param formattedMeasurement [out]  Receives the formatted measurement.
  */
  virtual void formatMeasurement(OdString& formattedMeasurement, OdDbDimStyleTableRecord* pDimVars);

  bool setUseDimLineIndex(OdDbDimension* pDim);
  void setDimLineIndex(OdDbEntityPtr& entity, OdInt32 index);

  double            m_dMeasurement;
  OdString          m_strDimText;

protected:
  OdDimRecomputor();
  virtual ~OdDimRecomputor();

  // Entity property
  
  OdDbObjectId      m_linetypeId;
  double            m_dLinetypeScale;
  OdInt16           m_nLineweight;
  OdCmTransparency  m_transparency;
  ///
  
  OdGeVector3d      m_vNormal;
  OdGePoint3d       m_DimLineDefPt;
  bool              m_bUserDefinedTextPosition;
  OdGePoint3d       m_ocsDimTextMidPt; // OCS - z coordinate is elevation
  double            m_dUcsXAngle;
  double            m_dTextRot;
  OdDbDatabase*     m_pDb;
  double            m_dimlfac;
  bool              m_dimtaln;

  OdInt32           m_LinespacingStyle;
  double            m_dLinespacingFactor;
  OdInt32           m_AttachPt;

  OdUInt16          m_BgrndTxtFlag;
  OdCmColor         m_BgrndTxtColor;
  bool              m_ExtLineFixLenEnable;
  double            m_ExtLineFixLen;

  OdDbObjectId      m_DimLinetypeId;
  OdDbObjectId      m_DimExt1LinetypeId;
  OdDbObjectId      m_DimExt2LinetypeId;

  bool              m_bArrowFirstIsFlipped;
  bool              m_bArrowSecondIsFlipped;

  // diminspect
  bool              m_bInspection;
  int               m_nInspectionFrame;
  OdString          m_sInspectionLabel;
  OdString          m_sInspectionRate;
  ///
  bool              m_bAnnotative;
  bool              m_bUseDimLineIndex;
  double            m_dTextWidth;
  double            m_dTextHeight;

  enum DimType
  {
    DimRotated       = 0,
    DimAligned       = 1,
    Dim2LineAngular  = 2,
    DimDiametric     = 3,
    DimRadial        = 4,
    Dim3PointAngular = 5,
    DimOrdinate      = 6,
    DimUnknown       = 7,
    DimArcLen        = 8,
    DimRadialLarge   = 9
  };
  /** \details
    Returns the Dimension type.
    
    \remarks
    getDimTypeFlag() will return one of the following:
    
    <table>
    Name                 Value
    DimRotated           0
    DimAligned           1
    Dim2LineAngular      2
    DimDiametric         3
    DimRadial            4
    Dim3PointAngular     5
    DimOrdinate          6
    DimUnknown           7
    DimArcLen            8
    DimRadialLarge       9
    </table>
  */
  virtual OdUInt8 getDimTypeFlag() = 0;

  /** \details
    Returns the database associated with the Dimension entity associated with this DimRecomputor object. 
  */
  OdDbDatabase* database() { return m_pDb; }
  
  /** \details
    Returns the WCS normal to the plane of the Dimension entity associated with this DimRecomputor object.
  */
  OdGeVector3d normal() const { return m_vNormal; }

/**DOM-IGNORE-BEGIN*/  
#define VAR_DEF(type, name, dxf, def_value, metric_def_value, reserve1, reserve2) \
  /** Description: Returns the DIM##NAME value from the specified dimension variables.  \
    Remarks: ##remarks \
    \
    Arguments: \
    pDimVars (I) Pointer to a non- database -resident dimension style record. \
    Notes: ##notes \
    See Also: ##see_also \
  */ \
  type getDim##name(OdDbDimStyleTableRecord* pDimVars);

#include "DimVarDefs.h"
#undef  VAR_DEF
/**DOM-IGNORE-END*/ 
  
  ///
  // last parameter is the text extents' length (along X axis, in OCS units)
  // it is needed in those cases where dimjust is 3 or 4 && dimtad = 1.

  /** \details
    Builds the first extension line of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param startPoint [in] Startpoint of the extension line.
    \param endPoint [in] Endpoint of the extension line.
    \param textLength [in] Text length.
    
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.

    textLength is the length of the text extents along the OCS X-axis. It is needed only when
    
            ((dimjust == 3) || (dimjust == 4)) && (dimtad == 1)
    
    \remarks
    This function is called by buildDimension().
  */
  bool buildExtLine1(const OdGePoint3d& startPoint, const OdGePoint3d& endPoint, 
    OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars,
    double textLength, const OdGeVector3d dir = OdGeVector3d::kYAxis);

  /** \details
    Builds the second extension line of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param startPoint [in] Startpoint of the extension line.
    \param endPoint [in] Endpoint of the extension line.
    \param textLength [in] Text length.
    
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.

    textLength is the length of the text extents along the OCS X-axis. It is needed only when
    
            ((dimjust == 3) || (dimjust == 4)) && (dimtad == 1)
    
    \remarks
    This function is called by buildDimension().
  */
  bool buildExtLine2(const OdGePoint3d& startPoint, const OdGePoint3d& endPoint, 
    OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars,
    double textLength, const OdGeVector3d dir = OdGeVector3d::kYAxis);

  // in this function dimjust == 3 or 4 is already taken into account
  // (textLength must be taken into account as is)

  /** \details
    Builds an extension line of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    dimBlkEntities [in/out] Dimension Block entities.
    startPoint [in] Startpoint of the extension line.
    endPoint [in] Endpoint of the extension line.
    textLength [in] Text length.
    direction [in] Extension line direction.
    oblique [in] Obliquing angle.
    
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.

    textLength is the length of the text extents along the OCS X-axis. It is needed only when
    
            ((dimjust == 3) || (dimjust == 4)) && (dimtad == 1)
    
    \remarks
    All angles are expressed in radians.
    
    This function is called by buildDimension().
  */
  void buildExtLine (const OdGePoint3d& startPoint, const OdGePoint3d& endPoint, 
    OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars,
    double textLength, const OdGeVector3d direction = OdGeVector3d::kYAxis, double oblique = 0.0 );

  /** \details
    Builds the first dimension arrow of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param origin [in] Tip of the dimension arrow.
    \param oppDir [in] Vector from tip to tail of dimension arrow
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
    
    This function is called by buildDimension().
  */
  void buildArrow1(const OdGePoint3d& origin, const OdGeVector3d& oppDir, OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);
  /** \details
    Builds the second dimension arrow of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param origin [in] Tip of the dimension arrow.
    \param oppDir [in] Vector from tip to tail of dimension arrow
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
    
    This function is called by buildDimension().
  */
  void buildArrow2(const OdGePoint3d& origin, const OdGeVector3d& oppDir, OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);

  /** \details
    Builds a dimension arrow of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param origin [in] Tip of the dimension arrow.
    \param oppDir [in] Vector from tip to tail of dimension arrow.
    \param arrowId [in] Object ID of the arrowhead block.
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
    
    This function is called by buildDimension().
  */
  void buildArrow (const OdGePoint3d& origin, const OdGeVector3d& oppDir, OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars, OdDbObjectId arrowId);

  /** \details
    Returns the intersections of the specified line with the specified extents.
    \param startPoint [in]  Startpoint of line.
    \param endPoint [in]  Endpoint of line.
    \param extents [in]  Extents.
    \param intersections [in]  Array of intersection points.
    \remarks
    This function is called by buildDimension().
  */
  void intersectLineAndExts(const OdGePoint3d& startPoint, const OdGePoint3d& endPoint,
    const OdGeExtents3d& extents, OdGePoint2dArray& intersections);

  /** \details
    Returns true if and only if, for the specified arrowhead, 
    the dimension line must be extended beyond the extension line.
    \remarks
    This function is called by buildDimension().
  */
  bool toExtend(OdDbObjectId arrowId);

  // Formatting text strings for dimensions functions

  /** \details
    Returns a formatted Decimal Units Dimension string with the specified parameters.
    \param formattedString [out]  Receives the formatted string.
    \param suppressLeading [in]  Suppress leading zeros.
    \param suppressTrailing [in]  Suppress trailing zeros.
    \param precision [in]  Number of decimal places.
    \param value [in]  Value to be formatted.
    \param roundOff [in]  Rounding value (0.0 for no rounding).
    \param separator [in]  Decimal separator.
    \remarks
    This function is called by formatDimStr().
  */
  void formatDimDecStr(OdString& formattedString, bool suppressLeading, bool suppressTrailing, OdInt16 precision,
    double value, double roundOff, OdChar separator);

  /** \details
    Returns a formatted Architectural Units Dimension string with the specified parameters.
    \param formattedString [out]  Receives the formatted string.
    \param precision [in]  Number of decimal places.
    \param measurement [in]  Measurement to be formatted.
    \param roundOff [in]  Rounding value (0.0 for no rounding).
    \param fracFormat [in]  Fractional format.
    \param suppressZeroFeet [in]  Suppress zero feet.
    \param suppressZeroInches [in]  Suppress zero inches.
    \param forLimits [in]  Generate dimension text for limits.
    \param dimtfac [in]  DIMTFAC value.
    \remarks
    fracFormat will be one of the following:
    
    <table>
    Value   Description 
    0       Horizontal Stacked 
    1       Diagonal Stacked 
    2       Not Stacked 
    </table>

    \remarks
    This function is called by formatDimStr().
  */
  void formatDimArcStr(OdString& formattedString, OdInt16 precision, double measurement, double roundOff,
    OdInt16 fracFormat, bool forLimits, bool forTolerances, bool suppressFeet, bool suppressZeroInches, double dimtfac);

  /** \details
    Returns a formatted Scientific Units Dimension string with the specified parameters.
    \param formattedString [out]  Receives the formatted string.
    \param precision [in]  Number of decimal places.
    \param measurement [in]  Measurement to be formatted.
    \param roundOff [in]  Rounding value (0.0 for no rounding).
    \remarks
    This function is called by formatDimStr().
  */
  void formatDimScientStr(OdString& formattedString, OdInt16 precision, double measurement, double roundOff);

  /** \details
    Returns a formatted Fractional Units Dimension string with the specified parameters.
    \param formattedString [out]  Receives the formatted string.
    \param precision [in]  Number of decimal places.
    \param measurement [in]  Measurement to be formatted.
    \param roundOff [in]  Rounding value (0.0 for no rounding).
    \param fracFormat [in]  Fractional format.
    \param forLimits [in]  Generate dimension text for limits.
    \param forTolerances [in]  Generate dimension text for tolerances.
    \param dimtfac [in]  DIMTFAC value.
    \remarks
    fracFormat will be one of the following:
    
    <table>
    Value   Description 
    0       Horizontal Stacked 
    1       Diagonal Stacked 
    2       Not Stacked 
    </table>

    \remarks
    This function is called by formatDimStr().
  */
  void formatDimFracStr(OdString& formattedString, OdInt16 precision, double measurement, double roundOff, OdInt16 fracFormat,
    bool forLimits, bool forTolerances, double dimtfac);

  /** \details
    Returns a formatted Dimension string with limits and tolerances with the specified parameters.
    \param formattedString [out]  Receives the formatted string.
    \param measurement [in]  Measurement to be formatted.
    \param units [in]  Drawing units.
    \param precision [in]  Number of decimal places.
    \param roundOff [in]  Rounding value (0.0 for no rounding).
    \param tzin [in]  Zero suppression.
    \param separator [in]  Decimal separator.
    \param fracFormat [in]  Fractional format.
    \param forLimits [in]  Generate dimension text for limits.
    \param forTolerances [in]  Generate dimension text for tolerances.
    \param dimtfac [in]  DIMTFAC value.
    \remarks
    units will be one of the following:
    
    <table>
    Value   Description
    1       Scientific 
    2       Decimal 
    3       Engineering 
    4       Architectural (stacked) 
    5       Fractional (stacked) 
    6       Architectural 
    7       Fractional 
    8       Microsoft Windows Desktop 
    </table>
    
    tzin will be one of the following:
    
    <table>
    Value   Description
    0       Suppress zero feet and exactly zero inches. 
    1       Include zero feet and exactly zero inches. 
    2       Include zero feet and suppress exactly zero inches. 
    3       Suppress zero feet and include exactly zero inches. 
    4       Suppress leading decimal zeros. 
    8       Suppress trailing decimal zeros. 
    12      Suppress leading and trailing decimal zeros. 
    </table>

    Values 0-3 affect only feet-and-inch dimensions, and may be combined with values 4-12.
    
    fracFormat will be one of the following:
    
    <table>
    Value   Description 
    0       Horizontal Stacked 
    1       Diagonal Stacked 
    2       Not Stacked 
    </table>

  */
  virtual void formatDimStr(OdString& formattedString, double measurement, OdInt16 units, OdInt16 precision, double roundOff,
    OdInt16 tzin, OdChar separator, OdInt16 fracFormat, bool forLimits, bool forTolerances, double dimtfac = 1.0);

  /** \details
    Returns a formatted Dimension string with the specified parameters.
    \param formattedString [out]  Receives the formatted string.
    \param hasLimits [in]  Controls the generation of limits.
    \param hasTols [in]  Controls the generation of tolerances.
    \param measurement [in]  Measurement to be formatted.
    \param dimlunit [in]  DIMLUNIT value.
    \param dimtm [in]  DIMTM value.
    \param dimtp [in]  DIMTP value.
    \param dimtzin [in]  DIMTZIN value.
    \param dimtfac [in]  DIMTFAC value.
    \param dimtdec [in]  DIMTDEC value.
    \param dimrnd [in]  DIMRND value.
    \param dimdsep [in]  DIMDSEP value. 
    \param dimfrac [in]  DIMFRAC value.
    \param dimtxt [in]  DIMTXT value.
    \param dimzin [in]  DIMZIN value.
    \param dimdec [in]  DIMDEC value.
    \param dimpost [in]  DIMPOST value.
  */
  virtual void formatDimStrWithLimsAndTols(OdString& formattedString, bool hasLimits, bool hasTols,
    double measurement, OdInt16 dimlunit,
    double dimtm, double dimtp, OdInt16 dimtzin, double dimtfac,
    OdInt16 dimtdec, double dimrnd, OdChar dimdsep,
    OdInt16 dimfrac, double dimtxt, OdInt16 dimzin, OdInt16 dimdec,
    OdString dimpost, bool bPrimaryUnits);

  // "the commonest" dimension text formatting functions

  /** \details
    Returns a formatted Dimension string using the specified dimension variables.
    \param formattedString [out]  Receives the formatted string.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
  */
  void formatDimStrWithAltUnits(OdString& formattedString, OdDbDimStyleTableRecord* pDimVars);

  /** \details
    Creates a formatted MText entity using the specified dimension variables.

    \param pMText [out] Receives a SmartPointer to the newly created MText entity.
    \param pDimVars [in] Pointer to a non- database -resident dimension style record.
  */
  void createTextEntityForDimGraphic(OdDbMTextPtr& pMText, OdDbDimStyleTableRecord* pDimVars);

  /** \details
    Creates a formatted MText entity using the specified dimension variables.

    \param pLabel [out] Receives a SmartPointer to the newly created MText entity.
    \param pRate [out] Receives a SmartPointer to the newly created MText entity.
    \param pDimVars [in] Pointer to a non- database -resident dimension style record.
  */
  void createInspectionEntitisForDimGraphic(OdDbMTextPtr& pLabel, OdDbMTextPtr& pRate,
    OdDbDimStyleTableRecord* pDimVars);

  /** \details
    Returns the object ID of the Defpoints layer.
  */
  OdDbObjectId getLayerDefpointsId();

  /** \details
    Returns the intersection of two lines assuming z == 0.
    \param intersectionPoint [out]  Receives the intersection point.
    \param firstLinePoint [in]  Point on the first line.
    \param secondLinePoint [in]  Point on the second line.
    \param firstLineVector [in]  Direction of first line.
    \param secondLineVector [in]  Direction of second line.
  */
  void intersectLines(OdGePoint3d& intersectionPoint, const OdGePoint3d& firstLinePoint,
    const OdGePoint3d& secondLinePoint, const OdGeVector3d& firstLineVector,
    const OdGeVector3d& secondLineVector);

  /** \details
    Returns the OCS xAxis and OCS yAxis of the current UCS.
    \note
    This is a bad function in need of redesign. 
  */
  void getUCS(OdGeVector3d& xAxis, OdGeVector3d& yAxis);

  // Builds boundary box about the text, if it is necessary
  
  /** \details
    Builds a boundary box, if necessary, around the specified MText entity using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param pMText [in] Pointer to the MText entity.
  
    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
    This function is called by recomputeDimBlock().
  */
  void buildBoundaryBox(OdDbDimStyleTableRecord* pDimVars, OdDbMText* pMText, OdArray<OdDbEntityPtr>& dimBlkEntities);

  void setTextProperty(OdDbMTextPtr& pText, OdDbDimStyleTableRecord* pDimVars);

  void makeInspectionFrame(OdDbDimStyleTableRecord* pDimVars, OdRecomputorEngine* pEngine, OdArray<OdDbEntityPtr>& dimBlkEntities);
};


/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDimRecomputer object pointers.
*/
typedef OdSmartPtr<OdDimRecomputor> OdDimRecomputorPtr;


/** \details
  This class is the base class for Dimension Recomputor Protocol Extensions.

  \remarks
  The entities comprising a dimension block are constructed
  from the parameters of a dimension and a set of dimension variables.  
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDbDimRecomputePEImpl : public OdDbDimensionRecomputePE
{
protected:
  /** \details
    Creates an instance of this DimRecomputor object.
    \remarks
    Returns a SmartPointer to the newly created object.
  */
  virtual OdDimRecomputorPtr createRecomputor() = 0;

public:
  /** \details
    Recomputes the measurement for the specified Dimension entity.
  */
  void recomputeDimMeasurement(OdDbDimension* pDimension);

  /** \details
    Recomputes the dimension block for the specified Dimension entity.
  */
  void recomputeDimBlock(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);

  /** \details
    Formats the measurement for the specified Dimension entity.
    \param pDimension [in]  Pointer to the dimension entity.
    \param formattedMeasurement [out]  Receives the formatted measurement.
    \param measurementValue [in]  Measurement to be formatted.
    \param dimensionText [in]  Dimension text.
  */
  void formatMeasurement(OdDbDimension* pDimension, OdString& formattedMeasurement, 
    double measurementValue, const OdString& dimensionText);

};

#define EXT_LINE_1_INDEX   0
#define EXT_LINE_2_INDEX   1
#define DIM_LINE_ORD_INDEX 2
#define DIM_LINE_INDEX     3

OdInt32 dimLineIndex(OdDbEntityPtr& entity);
void removeDimLineIndex(OdDbEntityPtr& entity);

#include "TD_PackPop.h"

#endif // __OD_DIM_RECOMPUTOR__
