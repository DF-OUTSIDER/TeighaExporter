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



#ifndef _OD_DB_ARCDIMENSION_DIMENSION_
#define _OD_DB_ARCDIMENSION_DIMENSION_

#include "TD_PackPush.h"

#include "DbDimension.h"


/** \details
    This class represents Arc Dimension entities in an OdDbDatabase instance.
    
    \remarks
    An Arc Dimension entity dimensions the length of an arc.
    
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DBDIM_EXPORT OdDbArcDimension : public OdDbDimension
{
public:
  ODDB_DECLARE_MEMBERS(OdDbArcDimension);

  OdDbArcDimension();

  /** \details
    Returns the WCS point defining the location of dimension arc for this Dimension entity.
  */
  OdGePoint3d         arcPoint() const;

  /** \details
    Sets the WCS point defining the location of dimension arc for this Dimension entity.
    \param arcPoint [in]  Arc point.
  */
  void setArcPoint(const OdGePoint3d& arcPoint);

  /** \details
    Returns the WCS start point of the first extension line of this Dimension entity.
      
    \remarks
    This point and the centerPoint define one side of the Arc being dimensioned.
  */
  OdGePoint3d xLine1Point() const;

  /** \details
    Sets the WCS start point of the first extension line of this Dimension entity.
  
    \param xLine1Point [in]  Start point.
        
    \remarks
    This point and the centerPoint define one side of the arc being dimensioned.
  */
  void   setXLine1Point(const OdGePoint3d & xLine1Pt);

  /** \details
    Returns the WCS start point of the second extension line of this Dimension entity.
      
    \remarks
    This point and the centerPoint define one side of the Arc being dimensioned.
  */
  OdGePoint3d xLine2Point() const;

  /** \details
    Sets the WCS start point of the second extension line of this Dimension entity.
  
    \param xLine2Point [in]  Start point.
        
    \remarks
    This point and the centerPoint define one side of the arc being dimensioned.
  */
  void setXLine2Point(const OdGePoint3d& xLine2Pt);

  /** \details
    Returns the WCS vertex of the arc being dimensioned by this Dimension entity.
    \remarks
    The vertex of the arc being dimensioned is the center of the dimension arc.
  */
  OdGePoint3d centerPoint() const;

  /** \details
    Sets the WCS vertex of the arc being dimensioned by this Dimension entity.
    \param centerPoint [in]  Center point.
    \remarks
    The vertex of the angle being dimensioned is the center of the dimension arc.
  */
  void setCenterPoint(const OdGePoint3d& centerPoint);

  /** \details
    Returns true if and only if this Dimension entity was specified using two points along the arc.
  */
  bool isPartial() const;
  /** \details
    Controls the specification of this Dimension entity with two points along the arc.
    \param partial [in]  Specifies partial dimension.
  */
  void setIsPartial(const bool partial);

  /** \details
    Returns the arc parameter of this Dimension entitiy's first definition point.
  */
  double arcStartParam() const;

  /** \details
    Sets the arc parameter of this Dimension entity's first definition point.
    \param arcStartParam [in]  Arc start parameter.
  */
  void setArcStartParam(const double arcStartParam);


  /** \details
    Returns the arc parameter of this Dimension entity's second definition point.
  */
  double arcEndParam() const;

  /** \details
    Sets the arc parameter of this Dimension entity's second definition point.
    \param arcEndParam [in]  Arc end parameter.
  */
  void setArcEndParam(const double arcEndParam);


  /** \details
    Returns true if and only if this Dimension entity has a leader drawn to resolve ambiguity.
  */
  bool hasLeader() const;
  /** \details
    Determines if this Dimension entity has a leader drawn to resolve ambiguity.
    \param hasLeader [in]  Controls leader.
  */
  void setHasLeader(const bool hasLeader);

  /** \details
    Returns the start point of the extra leader of this Dimension entity.
  */
  OdGePoint3d leader1Point() const;
  /** \details
    Sets the start point of the extra leader of this Dimension entity.
    \param leader1Point [in]  Start point of leader.
  */
  void setLeader1Point(const OdGePoint3d& leader1Point);

  /** \details
    Returns the end point of the extra leader of this Dimension entity.
  */
  OdGePoint3d leader2Point() const;
  /** \details
    Sets the end point of the extra leader of this Dimension entity.
    \param leader2Point [in]  End point of leader.
  */
  void   setLeader2Point(const OdGePoint3d& leader2Point);

  /** \details
    Returns the type of arc symbol used within the dimension text of this Dimension entity.
    
    \remarks
    arcSymbolType() returns one of the following:
    
    <table>
    Value   Description
    0       Arc symbol precedes text.
    1       Arc symbol is above text.
    2       No arc symbol.
    </table>
  */
  OdInt16 arcSymbolType() const;

  /** \details
    Sets the type of arc symbol used within the dimension text of this Dimension entity.
    
    \remarks
    arcSymbolType must be one of the following:
    
    <table>
    Value   Description
    0       Arc symbol precedes text.
    1       Arc symbol is above text.
    2       No arc symbol.
    </table>
    
    \remarks
    Overrides the value in the dimension style.
  */
  void setArcSymbolType(const OdInt16 arcSymbolType);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  /*
  OdDbArcDimension(
  const OdGePoint3d& centerPoint,
  const OdGePoint3d& xLine1Point,
  const OdGePoint3d& xLine2Point,
  const OdGePoint3d& arcPoint,
  const OdString* dimText = NULL,
  OdDbObjectId dimStyle = OdDbObjectId::kNull );
  */
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbArcDimension object pointers.
*/
typedef OdSmartPtr<OdDbArcDimension> OdDbArcDimensionPtr;

#include "TD_PackPop.h"

#endif
