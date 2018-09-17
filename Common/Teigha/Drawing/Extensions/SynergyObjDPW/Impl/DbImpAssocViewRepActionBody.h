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

#ifndef DbImpAssocViewRepActionBody_INCLUDED_
#define DbImpAssocViewRepActionBody_INCLUDED_ /*!DOM*/

#include "DbAssocViewRepActionBody.h"
#include "DbImpAssocActionBody.h"
#include "DbModelDocViewStyle.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include "TD_PackPush.h"

class OdDbAssocAction;

/** \details
  This class implements the view action body.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class SYNERGYOBJDPW_EXPORT OdDbImpAssocViewRepActionBody : public OdDbImpAssocParamBasedActionBody
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocViewRepActionBody();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocViewRepActionBody();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  void composeForLoad( OdDbObject *pObj, OdDb::SaveType format, OdDb::DwgVersion version );

protected:

  enum
  {
    kBreakAlignment = 1,
    kNotAligned = 1 << 2,
    kErased = 1 << 3,
    kShowLabelOwerride = 1 << 4
  };

  void evaluateOverride( const OdDbObjectId parentActionId );

  

  bool isAligned() const;
  bool isHorizontalAligned() const;
  bool breakAlignment();
  void setNeedBeErased( bool erased );
  bool isNeedBeErased() const;
  OdResult attachHatchManager( const OdDbObjectId& hatchManagerId, OdDbObjectId idParentAction );
  OdDbObjectId hatchManager( const OdDbObjectId parentActionId ) const;
  OdDbObjectId currentViewBorder( OdDbObjectId idParentAction ) const;
  OdDbObjectId currentViewport( OdDbObjectId idParentAction ) const;
  OdResult getShowHatch( bool &bShow, OdDbObjectId idParentAction ) const;
  OdResult setShowHatch( bool bShow, bool Unknown, OdDbObjectId idParentAction ) const;
  OdResult getShowViewLabel( bool &bShow, OdDbObjectId idParentAction ) const;
  OdResult setShowViewLabel( bool bShow, bool bUnknown, OdDbObjectId idParentAction );
  OdDbObjectId viewLabel( OdDbObjectId idParentAction ) const;
  OdResult setViewLabel( const OdDbObjectId& idViewLabel, OdDbObjectId idParentAction );
  OdDbObjectId viewSymbol( OdDbObjectId idParentAction ) const;
  OdResult getViewLabelOffset( OdGeVector2d& offset, bool Unknown, OdDbObjectId idParentAction ) const;
  OdResult setViewLabelOffset( const OdGeVector2d& offset, bool Unknown, OdDbObjectId idParentAction ) const;
  OdDbObjectId parentViewBorder( OdDbObjectId idParentAction ) const;
  OdDbObjectId parentViewport( OdDbObjectId idParentAction ) const;
  void viewSymbols( OdArray<OdDbObjectId> &symbolIds, bool bIncludeDetailViewSymol, OdDbObjectId idParentAction ) const;
  OdDbObjectId viewStyle( OdDbObjectId idParentAction ) const;
  void viewRepHatches( OdArray<OdDbObjectId> &hatchIds, OdDbObjectId idParentAction ) const;
  void setShowLabelOverride( bool bShowLabelOverride );
  bool isShowLabelOverriden() const;
  void repairAlignment();
  bool canBreakOrRepairAlignment() const;
  OdResult getViewLabelPosition( OdDbModelDocViewStyle::AttachmentPoint& labelPosType, OdDbObjectId idParentAction ) const;
  OdResult setViewLabelPosition( enum OdDbModelDocViewStyle::AttachmentPoint labelPosType, OdDbObjectId idParentAction );
  enum OdDbAssocViewRepActionBody::AlignmentTypeEnum getOriginalAlignmentType() const;
  OdResult removeViewSymbol( const OdDbObjectId& symbolId, OdDbObjectId idParentAction );
  OdResult removeViewSymbolParam( const OdDbObjectId& symbolParamId, OdDbObjectId idParentAction );
  OdResult setViewSymbol( const OdDbObjectId& idViewSymbol, OdDbObjectId idParentAction );
  OdResult setViewStyle( const OdDbObjectId& idViewStyle, OdDbObjectId idParentAction );
  OdResult setDefinitionId( const OdDbObjectId& objId, unsigned short tag, OdDbObjectId idParentAction );
  OdResult removeDefinitionParameters( unsigned short tag, OdDbObjectId idParentAction );

  /*  {secret}  */
  friend class OdDbAssocViewRepActionBody;
  /*  {secret}  */
  OdDbObjectId m_viewRepId; // AcDbViewRep x29B
  /*  {secret}  */
  OdUInt32 m_flags;
  /*  {secret}  */
  double m_Unknown1;

private:

  std::map<OdDbObjectId, OdGePoint3d> m_HatchExtCenter;

  OdGePoint2d m_CenterBeforeTransform;

  OdGeVector2d m_vectorCenterToCenterBeforeTransform;
  OdGeMatrix3d m_ViewBorderTransformMatrix;
  double m_scale;

  void savePosition( OdDbObjectId parenActId );

  void erase( OdDbObjectIdArray &objIdsDepCurrentAction, OdDbAssocActionPtr pParentAction, bool borderErase, bool mainIsErased );

  void UpdateSymbol( OdDbAssocActionPtr pParentAction );

  void calculateHatchAndSymbolTransMatrix();

  void UpdateLabel( OdDbAssocActionPtr pParentAction );

  void UpdateHatch( OdDbAssocActionPtr pParentAction );

  void CalculateBorderPos( OdDbAssocAction *pBaseAction, bool DirectlyChange );

  void removeSymbolInParenView( OdDbAssocAction *pBaseAction );

  bool UpdateScale(OdDbAssocActionPtr pParentAction, OdDbAssocActionPtr pBaseAction);

  void UpdateRotation( OdDbAssocAction *pParentAction );

  bool UpdateGeom(OdDbAssocAction* pParentAction, OdDbAssocAction* pBaseAction, bool DirectlyChange); // returns isScaleUpdated
};

#include "TD_PackPop.h"

#endif // DbImpAssocViewRepActionBody_INCLUDED_
