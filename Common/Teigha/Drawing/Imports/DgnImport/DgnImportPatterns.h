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

#ifndef _DGN_IMPORTPATTERNS_INCLUDED_ 
#define _DGN_IMPORTPATTERNS_INCLUDED_

#define STL_USING_MAP
#include <OdaSTL.h>
#include <DgElementId.h>
#include <DgHostAppServices.h>
#include <DgModel.h>
#include <DgLevelMask.h>
#include <DbHatch.h>

#include "Gi/GiBaseVectorizer.h"
#include "Gi/GiGeometrySimplifier.h"
#include "DgGiContext.h"

class OdDgDatabase;
class OdDbDatabase;
class OdDgLevelTableRecord;
class OdDbLayerTableRecord;
class OdDgElement;
class OdDgLineStyleResource;
class OdDgModel;


/** \details
  <group OdImport_Classes> 
*/
namespace TD_DGN_IMPORT {

void importClosedElementFill( OdDgElement* e, OdDbEntity* dbEll, OdDbBlockTableRecord* owner );

void importPatterns( OdDgGraphicsElementPtr pShape, const OdGePoint3dArray& arrVertices,
                      const OdDbObjectIdArray& arrElementIds, bool bUsePoints,
                      OdDbBlockTableRecord* owner, bool bAssoc = false,
                      OdDbObjectId idAssoc = OdDbObjectId()
                    );

void importSymbolPattern( OdDgElement* pElm, OdDbBlockTableRecord* owner );
OdDbEntityPtr createHatchEntity( const OdGePoint3dArray& arrPts );
OdDbEntityPtr createHatchEntity( const OdDbObjectIdArray& arrElements, bool bSplitContours = false );
void importGradient( OdDbHatchPtr& pHatch, OdDgGradientFillLinkagePtr& pGradientLinkage );
void importCrossPattern( OdDbHatchPtr& pHatch, OdDgElement* pElm, const OdDgCrossPatternLinkagePtr& pPaternLinkage,
                          double dUORsToModelScale );
void importLinearPattern( OdDbHatchPtr& pHatch, OdDgElement* pElm, const OdDgLinearPatternLinkagePtr& pPaternLinkage,
                          double dUORsToModelScale );
void importDwgBasedPattern( OdDbHatchPtr& pHatch, OdDgElement* pElm, const OdDgDWGPatternLinkagePtr& pPaternLinkage,
                           double dUORsToModelScale );
void importSymbolPattern( OdDbHatchPtr& pHatch, OdDgElement* pElm, const OdDgSymbolPatternLinkagePtr& pPaternLinkage, 
                         double dUORsToModelScale, OdDbBlockTableRecord* pOwner );
template <class T> void applyDgnPatternLineTypeAndWeightToHatch( OdDbHatchPtr& pHatch, OdDgDatabase* pDb,const T* pPaternLinkage );
template <class T> void applyDgnPatternSymbologyToHatch( OdDbHatchPtr& pHatch, OdDgDatabase* pDb, const T* pPaternLinkage );

//-----------------------------------------------------------------------------------------

class OdDgnImportSymbolToHatchConverter : public OdGiBaseVectorizer
                                        , public OdGiContextForDgDatabase
                                        , public OdGiGeometrySimplifier
{
public:
  OdDgnImportSymbolToHatchConverter();
  bool convertSymbolToHatch( const OdDgSharedCellDefinition* pCell, double dTolerance, OdUInt32 nMaxLineNumber, 
                             OdUInt32 nMinLineNumber, OdUInt32 nArcSegNum, double dRowSpacing, double dColSpacing,
                             OdHatchPattern& dwgPattern, double& dScaleFactor, OdUInt32& uHatchColorIndex );

  OdGiRegenType regenType() const;

  virtual void circleProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0);
  virtual void circleProc(const OdGePoint3d&, const OdGePoint3d&, const OdGePoint3d&, const OdGeVector3d* pExtrusion = 0);
  virtual void circularArcProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector, double sweepAngle, OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  virtual void circularArcProc(const OdGePoint3d& start, const OdGePoint3d& point, const OdGePoint3d& end, OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  virtual void polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
  virtual void polygonOut(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal = 0);
  virtual void nurbsProc( const OdGeNurbCurve3d& nurbsCurve );
  virtual void ellipArcProc( const OdGeEllipArc3d& ellipArc, const OdGePoint3d* endPointOverrides = 0, OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  TD_USING(OdGiGeometrySimplifier::polylineOut);
  TD_USING(OdGiGeometrySimplifier::polygonOut);
  TD_USING(OdGiGeometrySimplifier::circleProc);
  TD_USING(OdGiGeometrySimplifier::circularArcProc);
  TD_USING(OdGiGeometrySimplifier::nurbsProc);
  TD_USING(OdGiGeometrySimplifier::ellipArcProc);

private:
  void addHatchLine( OdGePoint2d ptStart, OdGePoint2d ptEnd );
  void addHatchPolyline(const OdGePoint3dArray& arrPts);
  void addHatchPolyline(const OdGePoint3d* arrPts, OdUInt32 uNumPts );
  void createYTopLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd );
  void createXTopLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd );
  void createYBottomLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd );
  void createXBottomLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd );

protected:
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

private:
  double         m_dSymbolWidth;
  double         m_dSymbolHeight;
  double         m_dBorderAngle;
  double         m_dScaleFactor;
  double         m_dTolerance;
  OdUInt32       m_maxLineNumber;
  OdUInt32       m_minLineNumber;
  OdUInt32       m_uSircleSegNum;
  OdGePoint3d    m_ptOffset;
  OdHatchPattern m_hatchPattern;
  OdArray<OdGePoint2d> m_arrBasePoints;
  OdArray<OdGePoint2d> m_arrCorrectedPoints;
};

//-----------------------------------------------------------------------------------------

}
#endif // _DGN_IMPORTPATTERNS_INCLUDED_
