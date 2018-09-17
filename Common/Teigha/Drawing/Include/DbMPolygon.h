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

#ifndef _DBMPOLYGON_H_INCLUDED_
#define _DBMPOLYGON_H_INCLUDED_

#include "DbHatch.h"

class OdDbCircle;
class OdDb2dPolyline;
class OdDbPolyline;
class OdDbMPolygonCrossing;
typedef OdArray<OdDbMPolygonCrossing,OdPlainObjectsAllocator<OdDbMPolygonCrossing> > OdDbMPolygonCrossingArray;
class OdDbMPolygonNode;

const double OdDbMPolygonCrossingFuzz = 1E-6;

#ifdef MPOLYGON_EXPORTS
# define MPOLYGON_EXPORT            OD_TOOLKIT_EXPORT
#else
# define MPOLYGON_EXPORT            OD_TOOLKIT_IMPORT
#endif

/** \details
    This class processes one or more closed polygons as a single polygon.
    Corresponding C++ library: AcMPolygonObj15
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class MPOLYGON_EXPORT OdDbMPolygon : public OdDbEntity {
public:
  OdDbMPolygon();
  virtual ~OdDbMPolygon();

  ODDB_DECLARE_MEMBERS(OdDbMPolygon);

  virtual OdDbHatchPtr hatch();
  virtual OdDbHatchPtr hatch() const;

  virtual double elevation() const;
  virtual void setElevation(double elevation);

  virtual OdGeVector3d normal() const;
  virtual void setNormal(const OdGeVector3d& normal);

  virtual OdResult evaluateHatch(bool bUnderestimateNumLines = false);

  virtual OdDbHatch::HatchPatternType patternType() const;

  virtual OdString patternName() const;

  virtual void setPattern(OdDbHatch::HatchPatternType patType,
    const OdString& patName);

  virtual double patternAngle() const;
  virtual void setPatternAngle(double angle);

  virtual double patternSpace() const;
  virtual void setPatternSpace(double space);

  virtual double patternScale() const;
  virtual void setPatternScale(double scale);

  virtual bool patternDouble() const;
  virtual void setPatternDouble(bool isDouble);

  virtual int numPatternDefinitions() const;
  virtual void getPatternDefinitionAt(int index, 
    double& angle,
    double& baseX,
    double& baseY,
    double& offsetX,
    double& offsetY,
    OdGeDoubleArray& dashes)
    const;

  void setGradientAngle(double angle);
  void setGradientShift( float shiftValue );
  void setGradientOneColorMode( bool oneColorMode );
  void setGradientColors( unsigned int count, 
    const OdCmColor* colors,
    const double* values );
  void setGradient(OdDbHatch::GradientPatternType gradType,
    const OdString& gradName);


  virtual OdCmColor patternColor() const;
  virtual void setPatternColor(const OdCmColor& pc);

  /** \details
    Returns the area of this entity.

    \param area [out]  Receives the area.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getArea(double& area) const;

  virtual OdGeVector2d getOffsetVector() const;

  virtual OdResult appendLoopFromBoundary(const OdDbCircle* pCircle,
    bool excludeCrossing = true, double tol = OdDbMPolygonCrossingFuzz);
  virtual OdResult appendLoopFromBoundary(const OdDbPolyline* pPoly,
    bool excludeCrossing = true, double tol = OdDbMPolygonCrossingFuzz);
  virtual OdResult appendLoopFromBoundary(const OdDb2dPolyline* pPoly,
    bool excludeCrossing = true, double tol = OdDbMPolygonCrossingFuzz);

  /** \details
    Returns the number of loops in the closed polygon.
  */
  virtual int numMPolygonLoops() const;

  virtual OdResult getMPolygonLoopAt(int loopIndex, OdGePoint2dArray& vertices,
    OdGeDoubleArray& bulges) const;

  virtual OdResult appendMPolygonLoop(const OdGePoint2dArray& vertices,
    const OdGeDoubleArray& bulges, bool excludeCrossing = true,
    double tol = OdDbMPolygonCrossingFuzz);

  virtual OdResult insertMPolygonLoopAt(int loopIndex, 
    const OdGePoint2dArray& vertices, const OdGeDoubleArray& bulges, 
    bool excludeCrossing = true, double tol = OdDbMPolygonCrossingFuzz);

  virtual OdResult removeMPolygonLoopAt(int loopIndex);

  /** \details
    Declares the loop directions.
  */
  enum loopDir {
    kExterior   = 0,   // Loop direction is counter-clockwise
    kInterior   = 1,   // Loop direction is clockwise
    kAnnotation = 2    // Text
  };

  /** \details
    Gets the direction of the specified loop in the closed polygon.
    Returns eOk if successful, or eInvalidInput if loop index is out of range.

    \param lindex [in]  Loop index as an Ingeter value in the range 0 to numMPolygonLoops()-1.
    \param dir [out]  Reference to a variable in which the loop direction must be saved.
  */
  virtual OdResult getLoopDirection(int lindex, OdDbMPolygon::loopDir& dir) const;

  /** \details
    Sets the direction for the specified loop in the closed polygon.
    Returns eOk if successful, or eInvalidInput if loop index is out of range.

    \param lindex [in]  Loop index as an Ingeter value in range 0 to numMPolygonLoops()-1.
    \param dir [in]  Loop direction.
  */
  virtual OdResult setLoopDirection(int lindex, OdDbMPolygon::loopDir dir);

  virtual bool isPointOnLoopBoundary(const OdGePoint3d& worldPt, int loop,
    double tol = OdDbMPolygonCrossingFuzz) const;
  virtual int isPointInsideMPolygon(const OdGePoint3d& worldPt, OdGeIntArray& loopsArray,
    double tol = OdDbMPolygonCrossingFuzz) const;
  /* Not implemented


  virtual OdResult getMPolygonTree(OdDbMPolygonNode*& loopNode) const;
  virtual void deleteMPolygonTree(OdDbMPolygonNode* loopNode) const;

  virtual double getPerimeter() const;
  virtual bool isBalanced() const;

  virtual OdResult balanceTree();
  virtual OdResult balanceDisplay();

  //virtual OdResult getLoopAtGsMarker(OdGsMarker gsMark, int &loop) const;
  virtual void getChildLoops(int curLoop, OdGeIntArray& selectedLoopIndexes) const;
  virtual int getParentLoop(int curLoop) const;
  virtual int getClosestLoopTo(const OdGePoint3d& worldPt) const;

  // Mpolyon loop crossing and inside methods.
  //

  virtual OdResult loopCrossesItself(bool& crosses, bool findAll,
    OdDbMPolygonCrossingArray& crossingsArray,
    const OdGePoint2dArray& vertexPts, 
    const OdGeDoubleArray& vertexBulges, 
    double tol = OdDbMPolygonCrossingFuzz) const;

  virtual bool loopCrossesItself(const OdGePoint2dArray& vertexPts, 
    const OdGeDoubleArray& vertexBulges, 
    double tol = OdDbMPolygonCrossingFuzz) const;

  virtual OdResult loopCrossesMPolygon(bool& crosses, bool findAll,
    OdDbMPolygonCrossingArray& crossingsArray,
    const OdGePoint2dArray& testVertexPts, 
    const OdGeDoubleArray& testVertexBulges, 
    double tol = OdDbMPolygonCrossingFuzz) const;

  virtual bool selfCrosses(const OdGePoint2dArray& vertexPts, 
    const OdGeDoubleArray& vertexBulges,
    double tol = OdDbMPolygonCrossingFuzz) const;

  virtual bool includesTouchingLoops(double tol = OdDbMPolygonCrossingFuzz) const;

  //virtual OdResult subGetClassID(void* pClsid) const;

  virtual OdResult   intersectWith(
    const OdDbEntity*   ent,
    OdDb::Intersect     intType,
    OdGePoint3dArray&   points,
    OdGsMarker            thisGsMarker  = 0,
    OdGsMarker            otherGsMarker = 0)
    const;

  virtual OdResult   intersectWith(
    const OdDbEntity*   ent,
    OdDb::Intersect     intType,
    const OdGePlane&    projPlane,
    OdGePoint3dArray&   points,
    OdGsMarker            thisGsMarker  = 0,
    OdGsMarker            otherGsMarker = 0)
    const;

  virtual OdResult   getOsnapPoints(
    OdDb::OsnapMode       osnapMode,
    OdGsMarker              gsSelectionMark,
    const OdGePoint3d&    pickPoint,
    const OdGePoint3d&    lastPoint,
    const OdGeMatrix3d&   viewXform,
    OdGePoint3dArray&        snapPoints,
    OdDbIntArray&     geomIds) const;

  virtual OdResult getGripPoints(OdGePoint3dArray&     gripPoints,
    OdDbIntArray&  osnapModes,
    OdDbIntArray&  geomIds) const;

  virtual OdResult moveGripPointsAt(const OdDbIntArray& indices,
    const OdGeVector3d& offset);

  virtual OdResult getStretchPoints(OdGePoint3dArray& stretchPoints) const;

  virtual OdResult moveStretchPointsAt( const OdDbIntArray& indices,
    const OdGeVector3d& offset);

  virtual OdResult createLoopsFromBoundaries (const OdDbObjectIdArray &ids,
    OdIntArray &rejectedObjs,
    bool excludeCrossing = true, double tol = OdDbMPolygonCrossingFuzz);
  
  virtual OdResult createLoops (const OdGePoint2dArrayArray &vertices,
    const OdArray<OdGeDoubleArray> &bulges, OdIntArray &rejectedObjs,
    bool excludeCrossing = true, double tol = OdDbMPolygonCrossingFuzz);
  */
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  virtual OdResult subTransformBy( 
    const OdGeMatrix3d& xfn );

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual void subSetDatabaseDefaults(
    OdDbDatabase* pDb, 
    bool doSubents);

  virtual void saveAs(
    OdGiWorldDraw* pWd,
    OdDb::DwgVersion ver ) const;
};

typedef OdSmartPtr<OdDbMPolygon> OdDbMPolygonPtr;

#endif // _DBMPOLYGON_H_INCLUDED_
