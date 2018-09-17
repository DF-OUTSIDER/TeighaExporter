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

#ifndef DbAssocViewRepActionBody_INCLUDED_
#define DbAssocViewRepActionBody_INCLUDED_ /*!DOM*/

#include "SynergyObjDPWExports.h" 
#include "DbAssocParamBasedActionBody.h"
#include "DbModelDocViewStyle.h"

#include "TD_PackPush.h"

//enum OdDbModelDocViewStyle::AttachmentPoint;


/** \details
  This class represents the view action body object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class SYNERGYOBJDPW_EXPORT OdDbAssocViewRepActionBody : public OdDbAssocParamBasedActionBody
{
public:     
  ODRX_DECLARE_MEMBERS(OdDbAssocViewRepActionBody);

  enum AlignmentTypeEnum
  {
    kNotAligned,
    kAligned,
    kVerticalAligned
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocViewRepActionBody(bool createImpObj = true);

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbAssocViewRepActionBody();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  void composeForLoad( OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo );

  //virtual OdResult subErase(int);
  //virtual OdResult dragStatusOverride(enum OdDb::DragStat);
  //virtual OdResult postProcessAfterDeepCloneOverride(class OdDbIdMapping &);
  //virtual OdResult addMoreObjectsToDeepCloneOverride(class OdDbIdMapping &,OdArray<class OdDbObjectId,OdArrayMemCopyReallocator<class OdDbObjectId> > &) const;
  //virtual OdResult evaluateDependencyOverride(class OdDbAssocDependency *);

  /** \details
    Overrides the evalute action.
  */
  virtual void evaluateOverride();

  /** \details
    Sets the specified ID instance for this instance.
  */
  OdResult setViewRepId(const OdDbObjectId&);

  /** \details
    Gets the associated ID instance.
  */
  OdDbObjectId viewRepId();

  //void setDirtyFlag(enum OdDbAssocViewRepActionBody::DirtyFlag);
  //void clearDirtyFlags();
  void setNeedBeErased( bool erased );
  bool isNeedBeErased() const;
  void setShowLabelOverride( bool bShowLabelOverride );
  bool isShowLabelOverriden() const;
  OdResult setShowHatch(bool,bool);
  OdResult getShowHatch(bool &show) const;
  OdResult attachHatchManager( const OdDbObjectId& hatchManagerId );
  OdDbObjectId hatchManager() const;
  OdResult setShowViewLabel(bool bShow, bool bUnknown);
  OdResult getShowViewLabel(bool &bShow) const;
  OdResult setViewLabelPosition( OdDbModelDocViewStyle::AttachmentPoint labelPosType );
  OdResult getViewLabelPosition( OdDbModelDocViewStyle::AttachmentPoint& labelPosType ) const;
  OdResult setViewLabelOffset( const OdGeVector2d& offset, bool Unknown );
  OdResult getViewLabelOffset( OdGeVector2d& offset, bool Unknown ) const;
  enum OdDbAssocViewRepActionBody::AlignmentTypeEnum getOriginalAlignmentType() const;
  //void notifyAlignmentChange();
  //void updateViewLabel(enum OdDbAssocDraggingState, bool);
  //void updateConstraintType(bool, double);
  //void cleanupTransientEnts();
  //bool processTransientsForViewBorder(bool, bool);
  //void createTransientEnts(bool);
  bool isHorizontalAligned() const;
  bool isAligned() const;
  bool canBreakOrRepairAlignment() const;
  OdDbObjectId parentViewBorder() const;
  OdDbObjectId parentViewport() const;
  void viewRepHatches( OdArray<OdDbObjectId> &hatchIds ) const;
  void viewSymbols(OdArray<OdDbObjectId> &symbolIds, bool bIncludeDetailViewSymol) const;
  OdDbObjectId viewSymbol() const;
  OdDbObjectId viewStyle() const;
  OdDbObjectId viewLabel() const;
  OdDbObjectId currentViewBorder() const;
  OdDbObjectId currentViewport() const;
  OdResult removeDefinitionParameters(unsigned short tag);
  OdResult setDefinitionId(const OdDbObjectId& objId, unsigned short tag);
  OdResult removeViewSymbol(const OdDbObjectId& symbolId);
  OdResult removeViewSymbolParam(const OdDbObjectId& symbolParamId);
  OdResult setViewStyle( const OdDbObjectId& idViewStyle );
  OdResult setViewSymbol(const OdDbObjectId& idViewSymbol);
  OdResult setViewLabel(const OdDbObjectId& idViewLabel);
  //OdResult resetTopLevelCompoundId();
  //OdResult setCurrentAndParentViewport( OdDbObjectId idCurrentViewPort, OdDbObjectId idParentViewPort );
  bool breakAlignment();
  void repairAlignment();

protected:
  /*  {secret}  */
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocViewRepActionBody class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocViewRepActionBody> OdDbAssocViewRepActionBodyPtr;

#include "TD_PackPop.h"

#endif // DbAssocViewRepActionBody_INCLUDED_
