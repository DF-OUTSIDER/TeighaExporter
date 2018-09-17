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

#ifndef _DbBlocksGrips_h_Included_
#define _DbBlocksGrips_h_Included_

#include "DbEvalGraph.h"
#include "DbBlockParameter.h"
#include "DbBlockElement.h"
#include "DbBlockFlipParameter.h"
#include "DbBlockLookUpParameter.h"

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockGripExpr : public OdDbEvalExpr
{
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockGripExpr);
  OdDbBlockGripExpr();
};
typedef OdSmartPtr<OdDbBlockGripExpr> OdDbBlockGripExprPtr;

class OdDbGripData;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockGrip : public OdDbBlockElement
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockGrip);
  OdDbBlockGrip();
  virtual void addedToGraph( OdDbEvalGraph* );
  virtual void removedFromGraph( OdDbEvalGraph* );
  OdGeVector3d definitionGripOffset() const;
  OdGePoint3d displayLocation() const;
  void setDisplayLocation(class OdGePoint3d const &);
  OdDbBlockParameterPtr getAssociatedParameter(OdDb::OpenMode mode = OdDb::kForRead) const;
  OdDbBlockParameter::ParameterComponent getAssociatedParameterComponent() const;
  OdGeVector3d gripOffset();
  bool insertionCycling() const;
  void setInsertionCycling(bool);
  OdInt32 insertionCyclingWeight() const;
  void setInsertionCyclingWeight(OdInt32);
  OdGePoint3d location() const;
  OdGePoint3d paramBasePoint() const;
  void setUpdatedLocation(class OdGePoint3d const &);
  OdGePoint3d updatedLocation();
};
typedef OdSmartPtr<OdDbBlockGrip> OdDbBlockGripPtr;

//class OdDbBlockPointParameter;
//typedef OdSmartPtr<OdDbBlockPointParameter>OdDbBlockPointParameterPtr;
//class OdDbBlockXYParameter
//typedef OdSmartPtr<OdDbBlockXYParameter>OdDbBlockXYParameterPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockXYGrip : public OdDbBlockGrip
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlockXYGrip);
  OdDbBlockXYGrip();
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  //OdDbBlockPointParameterPtr getAssociatedPointParameter( OdDb::OpenMode )
  //OdDbBlockXYParameterPtr getAssociatedXYParameter( OdDb::OpenMode )
};
typedef OdSmartPtr<OdDbBlockXYGrip> OdDbBlockXYGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPolarGrip : public OdDbBlockGrip
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlockPolarGrip);
  OdDbBlockPolarGrip();
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  //OdDbBlockPolarParameterPtr getAssociatedPolarParameter( OdDb::OpenMode );
};
typedef OdSmartPtr<OdDbBlockPolarGrip> OdDbBlockPolarGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockRotationGrip : public OdDbBlockGrip
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlockRotationGrip);
  OdDbBlockRotationGrip();
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  //AcDbBlockRotationParameterPtr AcDbBlockRotationGrip::getAssociatedRotationParameter( OdDb::OpenMode );
};
typedef OdSmartPtr<OdDbBlockRotationGrip> OdDbBlockRotationGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockLinearGrip : public OdDbBlockGrip
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockLinearGrip);
  OdDbBlockLinearGrip();
  //OdDbBlockLinearParameterPtr getAssociatedLinearParameter( OdDb::OpenMode );
  OdGeVector3d orientation() const;
  void setOrientation( const OdGeVector3d& );
};
typedef OdSmartPtr<OdDbBlockLinearGrip> OdDbBlockLinearGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockFlipGrip : public OdDbBlockGrip
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockFlipGrip);
  OdDbBlockFlipGrip();
  //void cycleFlipState();
  OdDbBlockFlipParameter::FlipState flipState() const;
  void setFlipState( OdDbBlockFlipParameter::FlipState );
  void setUpdatedFlipState( OdDbBlockFlipParameter::FlipState );
  OdDbBlockFlipParameter::FlipState updatedFlipState();
  //OdDbBlockFlipParameterPtr getAssociatedFlipParameter( OdDb::OpenMode );
  OdGeVector3d orientation();
  void setOrientation( const OdGeVector3d& );
  virtual void graphEvalEnd(bool);
};
typedef OdSmartPtr<OdDbBlockFlipGrip> OdDbBlockFlipGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockLookupGrip : public OdDbBlockGrip
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlockLookupGrip);
  OdDbBlockLookupGrip();
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  //OdDbBlockLookUpParameterPtr getAssociatedLookupParameter( OdDb::OpenMode );
  //void getValueList( OdResBufArray& );
  //bool showGrip() const;
};
typedef OdSmartPtr<OdDbBlockLookupGrip> OdDbBlockLookupGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockAlignmentGrip : public OdDbBlockGrip
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockAlignmentGrip);
  OdDbBlockAlignmentGrip();
  //OdDbBlockAlignmentParameterPtr getAssociatedAlignmentParameter( OdDb::OpenMode );
  OdGeVector3d orientation();
  void setOrientation(const OdGeVector3d&);
};
typedef OdSmartPtr<OdDbBlockAlignmentGrip> OdDbBlockAlignmentGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockVisibilityGrip : public OdDbBlockGrip
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlockVisibilityGrip);
  OdDbBlockVisibilityGrip();
  OdResult dxfInFields(OdDbDxfFiler* pFiler);
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  virtual void setMemberOfCurrentVisibilitySet(bool) ODRX_OVERRIDE;
  //void getValueList( OdResBufArray& );
  //bool showGrip() const;
};

typedef OdSmartPtr<OdDbBlockVisibilityGrip> OdDbBlockVisibilityGripPtr;

#endif // _DbBlocksGrips_h_Included_
