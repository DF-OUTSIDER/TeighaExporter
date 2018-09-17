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

#ifndef OD_VIEWREP_H
#define OD_VIEWREP_H

#include "DbObject.h"
#include "ModelDocObjExports.h"

#include "TD_PackPush.h"

/** \details

    Corresponding C++ library: AcModelDocObj
    <group OdModelDocObj_Classes> 
*/
class MODELDOCOBJ_EXPORT OdDbViewRep : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbViewRep);
  OdDbViewRep();
  virtual ~OdDbViewRep();

  enum ViewStyleType 
  {
    kVisibleLines                = 0,
    kVisibleAndHiddenLines       = 1,
    kShadedVisibleLines          = 2,
    kShadedVisibleAndHiddenLines = 3
  };

  enum SourceType
  {
    kSourceNotDefined   = 0,
    kInventorSource     = 1,
    kFusionSource       = 2,
    kModelSpaceSource   = 3
  };

  enum ViewType
  {
    kBaseFront = 0,
    kBaseTop = 1,
    kBaseRight = 2,
    kBaseBack = 3,
    kBaseBottom = 4,
    kBaseLeft = 5,
    kBaseNEIsometric = 6,
    kBaseNWIsometric = 7,
    kBaseSEIsometric = 8,
    kBaseSWIsometric = 9,
    kBaseCurrent = 10,
    kDetail = 13,
    kSection = 14,
    kProjected = 15
  };

  enum
  {
    kShaded = 1,
    kLabelVisibility = 1 << 1,
    kScaleFromParent = 1 << 6
  };

  OdResult sketchModelSpaceToModelSpaceXform( OdGeMatrix3d &mat ) const;
  OdResult sketchModelSpaceToPaperSpaceXform( OdGeMatrix3d &mat ) const;

  OdResult viewModelSpaceToModelSpaceXform( OdGeMatrix3d &mat ) const;
  OdResult viewModelSpaceToPaperSpaceXform( OdGeMatrix3d &mat ) const;

  OdDbObjectId selSetObjId() const;
  bool isPartialMSView() const;
  unsigned long viewIndex() const;

  void setInventorDrawingViewId(OdUInt64 invDVid);
  OdUInt64 inventorDrawingViewId() const;
  OdDbObjectId layoutId() const;

  const class OdRxSketchManager* getSketchManager() const;
  OdRxSketchManager* getSketchManager();

  OdDbObjectId viewBlockReference() const;
  OdDbObjectId blockReference() const;
  OdDbObjectId viewBorderId() const;
  OdDbObjectId viewportId() const;

  enum OdDbViewRep::SourceType sourceType();
  const OdString& label();
  bool labelVisibility();
  double scale();
  
  bool isShaded();
  OdUInt32 shadeResolution() const;
  enum OdDbViewRep::ViewStyleType viewStyleType();
  enum OdDbViewRep::ViewType viewType();
  void camera( OdGePoint3d &eye, OdGePoint3d &target, OdGeVector3d &upVector );
  double rotationAngle() const;
  OdGePoint2d viewBasePoint() const;
  OdGePoint3d center();
  OdResult updateViewportScale( double scale );
  bool isScaleFromParent() const;
  void setScaleFromParent( bool fromParent );

protected:
  // OdDbObject methods :
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  friend class OdDbImpAssocViewRepActionBody;

private:
  void setRotationAngle(double angle);

};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbViewRep object pointers.
*/
typedef OdSmartPtr<OdDbViewRep> OdDbViewRepPtr;

#include "TD_PackPop.h"

#endif // OD_VIEWREP_H
