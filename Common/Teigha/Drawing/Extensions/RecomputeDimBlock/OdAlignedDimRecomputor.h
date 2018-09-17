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

#ifndef __OD_ALIGNED_DIM_RECOMPUTOR__
#define __OD_ALIGNED_DIM_RECOMPUTOR__

#include "TD_PackPush.h"

#include "OdDimRecomputor.h"

/** \details
  This class implements an Aligned Dimension Block Recomputor.
  Library: Source provided.
  <group RecomputeDimBlock_Classes>
*/
class OdAlignedDimRecomputor : public OdDimRecomputor
{
public:
  void getDimParams(const OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx);
  void setDimParams(OdDbDimension* pDimension, OdDbDimensionObjectContextData* ctx) const;

  void buildDimension(OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);
  void preprocessDimPoints(OdDbDimStyleTableRecord* pDimVars);
  void updateMeasurement(OdDbDimStyleTableRecord* pDimVars);
  void formatMeasurement(OdString& formattedMeasurement, OdDbDimStyleTableRecord* pDimVars);

protected:
  OdAlignedDimRecomputor();
  virtual ~OdAlignedDimRecomputor();

  OdGePoint3d m_DefPoint1, m_DefPoint2;
  double m_dOblique;
  // jog
  bool m_bJogSymbolOn;
  OdGePoint3d m_pJogPosition; 
  double  m_JogSymbolHeight;
  bool m_JogSymbolUserDefPos;

  virtual OdUInt8 getDimTypeFlag();

  /** \details
    Returns the extension lines and definition points of the dimension block.
    \param extLine1Start [out]  Receives the startpoint of the first extension line.
    \param extLine1End [out]  Receives the endpoint of the first extension line.
    \param extLine2Start [out]  Receives the startpoint of the second extension line.
    \param extLine2End [out]  Receives the endpoint of the second extension line.
    \param dimLineDefPoint [out]  Receives the dimension line definition point.
    \param defPoint1 [out]  Receives the first definition point.
    \param defPoint2 [out]  Receives the second definition point.
  */
  virtual void calculateExtLinePoints(OdGePoint3d& extLine1Start, OdGePoint3d& extLine1End,
    OdGePoint3d& extLine2Start, OdGePoint3d& extLine2End,
    const OdGePoint3d& dimLineDefPt, const OdGePoint3d& defPoint1, const OdGePoint3d& defPoint2);

  /** \details
    Builds the first dimension line of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param startPoint [in] Startpoint of the extension line.
    \param endPoint [in] Endpoint of the extension line.

    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
  */
  void buildDimLine1(const OdGePoint3d& startPoint, const OdGePoint3d& endPoint, OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);

  /** \details
    Builds the second dimension line of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param startPoint [in] Startpoint of the extension line.
    \param endPoint [in] Endpoint of the extension line.

    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
  */
  void buildDimLine2(const OdGePoint3d& startPoint, const OdGePoint3d& endPoint, OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);

  /** \details
    Builds the specified dimension line of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param startPoint [in] Startpoint of the extension line.
    \param endPoint [in] Endpoint of the extension line.

    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
    
    This function is called by buildDimLine1() and buildDimLine2().
  */
  void buildDimLine (const OdGePoint3d& startPoint, const OdGePoint3d& endPoint, OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);
  
  /** \details
    Builds the single dimension line of the dimension block using the specified dimension variables.
    \param pDimVars [in]  Pointer to a non- database -resident dimension style record.
    \param dimBlkEntities [in/out] Dimension Block entities.
    \param startPoint [in] Startpoint of the extension line.
    \param endPoint [in] Endpoint of the extension line.

    \remarks
    SmartPointers to the entities comprising the dimension block are appended to the specified array.
    
    This function is called by buildDimLine1() and buildDimLine2().
  */
  void buildSingleDimLine(const OdGePoint3d& startPoint, const OdGePoint3d& endPoint, OdArray<OdDbEntityPtr>& dimBlkEntities, OdDbDimStyleTableRecord* pDimVars);
};

/** \details
  This class implements an Aligned Dimension Block Recomputor Protocol Extension.
  Library: Source provided.
  <group RecomputeDimBlock_Classes> 
*/
class OdDbAlignedDimRecomputePEImpl : public OdDbDimRecomputePEImpl
{
protected:
  virtual OdDimRecomputorPtr createRecomputor();
};

#include "TD_PackPop.h"

#endif // __OD_ALIGNED_DIM_RECOMPUTOR__
