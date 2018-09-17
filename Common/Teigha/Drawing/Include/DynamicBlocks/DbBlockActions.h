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

#ifndef _DbBlocksActions_h_Included_
#define _DbBlocksActions_h_Included_

#include "DbEvalGraph.h"
#include "Ge/GePoint2dArray.h"
#include "DbBlockElement.h"
#include "DbBlockParameter.h"

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockAction : public OdDbBlockElement
{
public:
  enum OdDbBlockActionXYType{ XY, X, Y }; // not sure if the enum is the same in scale and polar stretch actions
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockAction);
  virtual OdGePoint3d displayLocation() const;
  virtual void setDisplayLocation( const OdGePoint3d& );
  virtual OdDbEvalNodeId drivingParameterNodeId()const;
  OdDbObjectIdArray elementSelectionSet() const;

  virtual OdDbObjectIdArray selectionSet() const;
  virtual OdDbBlockParameterPtr getAssociatedParameter( OdDb::OpenMode mode = OdDb::kForRead ) const;
  //virtual void setSelectionSet( const OdDbObjectIdArray& );
};


/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockMoveAction : public OdDbBlockAction
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockMoveAction);
  OdDbBlockMoveAction();
  //double angleOffset();
  //double distanceMultiplier();
  //OdDbBlockActionXYType distanceType();
  //void setAngleOffset(double);
  //void setDistanceMultiplier(double);
  //void setDistanceType(OdDbBlockActionXYType);
};

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockStretchAction : public OdDbBlockAction
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockStretchAction);
  OdDbBlockStretchAction();
  //double angleOffset()const;
  //double distanceMultiplier()const;
  //OdDbBlockActionXYType distanceType()const;
  //void setAngleOffset(double);
  //void setDistanceMultiplier(double);
  //void setDistanceType(OdDbBlockActionXYType);
  //void setStretchFrame(const OdGePoint2dArray&);
  //OdGePoint2dArray stretchFrame() const;
};

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockActionWithBasePt : public OdDbBlockAction
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockActionWithBasePt);
  //OdGePoint3d basePoint() const;
  //OdGeVector3d basePointOffset() const;
  //bool dependent() const;
  //void setBasePoint(OdGePoint3d const &);
  //void setBasePointOffset(OdGeVector3d const &);
  //void setDependent(bool);
};

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockScaleAction : public OdDbBlockActionWithBasePt
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockScaleAction);
  OdDbBlockScaleAction();
  //OdDbBlockActionXYType scaleType();
  //void setScaleType( OdDbBlockActionXYType );
};

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockRotateAction : public OdDbBlockActionWithBasePt
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockRotateAction);
  OdDbBlockRotateAction();
};

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockFlipAction : public OdDbBlockAction
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockFlipAction);
  OdDbBlockFlipAction();
};

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockArrayAction : public OdDbBlockAction
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockArrayAction);
  OdDbBlockArrayAction();
  //double columnOffset() const;
  //double rowOffset() const;
  //void setColumnOffset(double);
  //void setRowOffset(double);
};

class OdDbDataTable;
typedef OdSmartPtr<OdDbDataTable>OdDbDataTablePtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockLookupAction : public OdDbBlockAction
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockLookupAction);
  OdDbBlockLookupAction();
  //bool duplicateCellsInLookupColumn(int,int *);
  //bool duplicateCellsInLookupColumn( const OdResBufArray&, const OdArray<void*>&,int,int *);
  //bool duplicateRowsOverInputColumns(int *);
  //bool duplicateRowsOverInputColumns( const OdResBufArray &, const OdArray<void*>&,int *);
  //OdDbDataTablePtr getLookupTable( OdArray<void*> & );

  void getLookupTable( OdResBufArray&,OdArray<void*>&);
  //bool nonSingletonRangeInInputColumns(int *);
  //bool nonSingletonRangeInInputColumns( const OdResBufArray &,const OdArray<void*>&,int *);
  //bool nullsInInputColumns(int *);
  //bool nullsInInputColumns( const OdResBufArray&, const OdArray<void*>&,int *);

  OdUInt32 numberOfInputColumns() const;
  OdUInt32 numberOfOutputColumns()const;
  OdUInt32 numberOfColumns() const;
  OdUInt32 numberOfRows()const;
  //void setLookupTable( const OdResBufArray &, OdArray<void*> const &);
  //void setLookupTable( const OdDbDataTable *, OdArray<void*>&);
};
typedef OdSmartPtr<OdDbBlockLookupAction>OdDbBlockLookupActionPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPolarStretchAction : public OdDbBlockAction
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockPolarStretchAction);
  OdDbBlockPolarStretchAction();
  //double angleOffset() const;
  //double distanceMultiplier() const;
  //OdDbObjectIdArray rotateOnlyElementSelectionSet();
  //OdDbObjectIdArray rotateOnlySelectionSet();
  //void setAngleOffset(double);
  //void setDistanceMultiplier(double);
  //void setRotateOnlySelectionSet(const OdDbObjectIdArray&);
  //void setStretchFrame(const OdGePoint2dArray &);
  //OdGePoint2dArray stretchFrame();
};

#endif // _DbBlocksActions_h_Included_
