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

#ifndef OD_VIEWBORDER_H
#define OD_VIEWBORDER_H

#include "DbEntity.h"
#include "ModelDocObjExports.h"

#include "TD_PackPush.h"

/** \details
    This class represents the border entity that is used by model documentation views and defines the region for placing the view on the layout.

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewBorder : public OdDbEntity
{
  friend class OdDbAssocViewRepActionBody;
  friend class OdDbImpAssocViewRepActionBody;
  //friend class OdDbViewBorderImpl;
  //friend class OdDbViewSymbolImpl;
public:
  ODDB_DECLARE_MEMBERS(OdDbViewBorder);

  /** \details
    Default constructor. Creates an OdDbViewBorder instance.
  */
  OdDbViewBorder();
  virtual ~OdDbViewBorder();


  /** \details
    Defines view style types.
  */
  enum ViewStyleType 
  {
    kFromBase               = 0,  // The style defined by the base view.
    kWireframeVisibleEdges  = 1,  // Wireframe style with visible edges only.
    kWireframeHiddenEdges   = 2,  // Wireframe style with hidden edges displayed.
    kShadedVisibleEdges     = 3,  // Shaded style with visible edges only.
    kShadedHiddenEdges      = 4   // Shaded style with hidden edges displayed.
  };

  /** \details
    Defines the types of source data of the documentation view. Views can be created from model space or from external Autodesk Inventor or Inventor Fusion files. 
  */
  enum SourceType
  {
    kSourceNotDefined   = 0,  // Source type is not defined.
    kInventorSource     = 1,  // Source data is from an Autodesk Inventor file.
    kFusionSource       = 2,  // Source data is from an Autodesk Inventor Fusion file.
    kModelSpaceSource   = 3   // Source data is from model space.
  };

  /** \details
    Returns WCS coordinates of the center point of the view border entity in paper space. 
  */
  OdGePoint3d insertionPoint() const;

  /** \details
    Returns the height of the view border entity. 
  */
  double height() const;

  /** \details
    Returns the width of the view border entity. 
  */
  double width() const;

  /** \details
    Returns the source data type of the model documentation view as a SourceType enumeration value.
  */
  OdDbViewBorder::SourceType sourceType() const;

  /** \details
    Returns the full path to a referenced Autodesk Inventor file.

    \returns
    Returns a String object that contains the full path to the Inventor file.
    If the view source type isn't SourceType::kInventorSource, the method returns an empty string.
  */
  OdString inventorFileReference() const;

  /** \details
    Returns true value if the projection type is the first angle and false otherwise.
  */
  bool isFirstAngleProjection() const;

  /** \details
    Returns the rotation angle of the model documentation view around the line of sight 
    (vector perpendicular to the display, going from the front of display toward the back). 
    Zero angle is on the X-axis of the display coordinate system, and positive direction of rotation is counterclockwise.

    \returns
    Returns the view's rotation angle in radians as a double value.
  */
  double rotationAngle() const;


  /** \details
    Returns the view style type as a ViewStyleType enumeration value.
  */
  OdDbViewBorder::ViewStyleType viewStyleType() const;

  /** \details
    Returns the scale factor of the model documentation view. 
    The scale factor defines a ratio between paper space units and viewport units in model space.
    For example, if the scale factor is 2 that means 1 unit of the viewport maps to 2 units in paper space.  
  */
  double scale() const;

  /** \details
    Returns the DPI value for the views with the shaded view styles (ViewStyleType::kShadedVisibleEdges, ViewStyleType::kShadedHiddenEdges).  
  */
  OdUInt32 shadedDPI() const;

  /** \details
    Returns ObjectId of the associated OdDbViewport object.  
  */
  OdDbObjectId viewportId() const;
  OdResult setViewportId(const OdDbObjectId& viewportId);

protected:
  // OdGiDrawable methods :
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual void subHighlight(bool bDoIt = true, const OdDbFullSubentPath* pSubId = 0, bool highlightAll = false) const;

  //// OdDbObject methods :
  //virtual OdResult subErase(bool erasing);
  //virtual OdResult subDeepClone(OdDbObject* pOwnerObject, OdDbObject*& pClonedObject, 
  //                              OdDbIdMapping& idMap, bool isPrimary = true) const;
  //virtual OdResult subWblockClone(OdRxObject* pOwnerObject, OdDbObject*& pClonedObject, 
  //                                OdDbIdMapping& idMap, bool isPrimary = true) const;

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

public:
  //virtual void audit(OdDbAuditInfo* pAuditInfo);

  //virtual OdResult subGetStretchPoints(OdGePoint3dArray& stretchPoints) const;
  //virtual OdResult subMoveStretchPointsAt(const OdIntArray& indices, 
  //                                        const OdGeVector3d& offset);

  // OdDbEntity methods :
  //virtual OdResult subGetGripPoints(OdDbGripDataPtrArray& grips, 
  //                                  const double curViewUnitSize, 
  //                                  const int gripSize, 
  //                                  const OdGeVector3d& curViewDir, 
  //                                  const int bitflags) const;
  //virtual OdResult subGetGripPoints(OdGePoint3dArray& gripPoints, 
  //                                  OdIntArray& osnapModes, 
  //                                  OdIntArray& geomIds) const;
  //virtual OdResult subMoveGripPointsAt(const OdDbVoidPtrArray& gripAppData,
  //                                     const OdGeVector3d& offset, 
  //                                     const int bitflags);
  //virtual OdResult subMoveGripPointsAt(const OdIntArray& indices, 
  //                                     const OdGeVector3d& offset);
  //virtual void subGripStatus(const OdDb::GripStat status);

  //virtual OdResult subGetOsnapPoints(OdDb::OsnapMode osnapMode, 
  //                                   OdGsMarker gsSelectionMark, 
  //                                   const OdGePoint3d& pickPoint, 
  //                                   const OdGePoint3d& lastPoint, 
  //                                   const OdGeMatrix3d& viewXform, 
  //                                   OdGePoint3dArray& snapPoints, 
  //                                   OdIntArray& geomIds) const;
  //virtual OdResult subGetOsnapPoints(OdDb::OsnapMode osnapMode, 
  //                                   OdGsMarker gsSelectionMark, 
  //                                   const OdGePoint3d& pickPoint, 
  //                                   const OdGePoint3d& lastPoint, 
  //                                   const OdGeMatrix3d& viewXform, 
  //                                   OdGePoint3dArray& snapPoints, 
  //                                   OdIntArray& geomIds, 
  //                                   const OdGeMatrix3d& insertionMat) const;

  //virtual OdResult subIntersectWith(const OdDbEntity* pEnt, 
  //                                  OdDb::Intersect intType, 
  //                                  const OdGePlane& projPlane, 
  //                                  OdGePoint3dArray& points, 
  //                                  OdGsMarker thisGsMarker = 0, 
  //                                  OdGsMarker otherGsMarker = 0) const;

  //virtual OdResult subIntersectWith(const OdDbEntity* pEnt, 
  //                                  OdDb::Intersect intType, 
  //                                  OdGePoint3dArray& points, 
  //                                  OdGsMarker thisGsMarker = 0, 
  //                                  OdGsMarker otherGsMarker = 0) const;

  virtual OdResult subTransformBy(const OdGeMatrix3d& xform);

  //virtual OdResult subGetClassID(CLSID* pClsid) const;

  //virtual void subList() const;

  //virtual void subHighlight(bool bDoIt = true, 
  //                          const OdDbFullSubentPath* pSubId = 0, 
  //                          bool highlightAll = false) const;

  //virtual  OdResult  subGetSubentPathsAtGsMarker(OdDb::SubentType type,
  //                                               OdGsMarker gsMark, 
  //                                               const OdGePoint3d& pickPoint,
  //                                               const OdGeMatrix3d& viewXform, 
  //                                               int& numPaths,
  //                                               OdDbFullSubentPath*& subentPaths, 
  //                                               int numInserts = 0,
  //                                               OdDbObjectId* entAndInsertStack = NULL) const;

  //virtual  OdResult  subGetGsMarkersAtSubentPath(const OdDbFullSubentPath* pSubPath, 
  //                                               OdArray<OdGsMarker>& gsMarkers) const;

  //virtual  OdResult  subGetSubentClassId(const OdDbFullSubentPath* pSubPath, CLSID* clsId) const;

  //virtual void saveAs(OdGiWorldDraw* mode, OdDb::SaveType st);

  //virtual OdResult setLayer(const OdString& newVal,
  //                          bool doSubents = true,
  //                          bool allowHiddenLayer = false);
  //virtual OdResult setLayer(OdDbObjectId newVal,
  //                          bool doSubents = true,
  //                          bool allowHiddenLayer = false);

public:
  // Internal use only methods:
  //OdResult setViewportId(const OdDbObjectId& viewportId);
  OdGePoint2d centerPoint(bool exludeSketchEnts = true) const;
  OdGePoint3d centerPoint3d(bool exludeSketchEnts = true) const;
  OdResult getSize(OdGePoint3d& minPt, OdGePoint3d& maxPt) const;
  OdResult setSize(const OdGePoint3d& minPt, const OdGePoint3d& maxPt);
  void setScale(double newScale);
  //OdResult recalculateBorder();

  //void setTransientEnts(const OdArray<OdDbEntity*>& traientEnts);
  //void setDisableUnHighlight(bool bIsDisable);
  //bool hitTest(const OdGePoint3d& inputPt) const;

  // Internal use only methods: subentity related methods
  //OdResult getSubentColor(const OdDbSubentId& subentId, OdCmColor& color) const;
  //OdResult setSubentColor(const OdDbSubentId& subentId, const OdCmColor& color);

  //OdResult getSubentLayerId(const OdDbSubentId& subentId, OdDbObjectId& layerId) const;
  //OdResult setSubentLayer(const OdDbSubentId& subentId, OdDbObjectId newVal);

  //OdResult getSubentLinetypeId(const OdDbSubentId& subentId, OdDbObjectId& linetypeId) const;
  //OdResult setSubentLinetype(const OdDbSubentId& subentId, OdDbObjectId newVal);

  //OdResult getSubentLinetypeScale(const OdDbSubentId& subentId, double& linetypeScale) const;
  //OdResult setSubentLinetypeScale(const OdDbSubentId& subentId, double newVal);

  //const OdString& getSubentPlotStyleName(const OdDbSubentId& subentId) const;
  //OdResult setSubentPlotStyleName(const OdDbSubentId& subentId, const OdString& newName);

  //OdResult getSubentLineWeight(const OdDbSubentId& subentId, OdDb::LineWeight& lineweight) const;
  //OdResult setSubentLineWeight(const OdDbSubentId& subentId, OdDb::LineWeight newVal);

  //OdResult getSubentVisibility(const OdDbSubentId& subentId, OdDb::Visibility& visVal) const;
  //OdResult setSubentVisibility(const OdDbSubentId& subentId, OdDb::Visibility newVal);

  //OdResult subentIdToNestedEnts(const OdDbSubentId& subentId, OdDbFullSubentPathArray &nestedEnts) const;

 protected:

  OdDbObjectId getScaleId() const;
  void setScaleId( const OdDbObjectId scaleId );

//private:
//  OdGeMatrix3d transientXform() const;

//private:
//  static  double sViewBoderOffset;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewBorder object pointers.
*/
typedef OdSmartPtr<OdDbViewBorder> OdDbViewBorderPtr;

#include "TD_PackPop.h"

#endif // OD_VIEWBORDER_H
