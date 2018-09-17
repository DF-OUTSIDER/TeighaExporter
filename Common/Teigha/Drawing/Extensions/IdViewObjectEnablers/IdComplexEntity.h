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

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of the "Object enabler" for AcIdComplexEntity ("Id" - short for "Inventor Drawing")

#ifndef IDCOMPLEXENTITY_INCLUDED
#define IDCOMPLEXENTITY_INCLUDED

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DbFiler.h"
#include "DbBlockTableRecord.h"
#include "Gi/GiViewportDraw.h"
#include "IdViewExport.h"

/** \details
    This structure stores data for lines exported from Autodesk® Inventor®.

    <group !!RECORDS_tdrawings_apiref>
*/
struct  _LineData
{
  OdGePoint3d   pnt1;
  OdGePoint3d   pnt2;
  OdDbObjectId  unknownId;

  _LineData() {}
};
typedef OdArray<_LineData>       CLineData;

/** \details
    This structure stores data for arcs exported from Autodesk® Inventor®.

    <group !!RECORDS_tdrawings_apiref>
*/
struct  _ArcData
{
  OdGePoint3d   pnt;
  OdGeVector3d  vec1;
  double        dbl1;
  OdGeVector3d  vec2;
  double        dbl2;

  _ArcData() : 
      dbl1( 0.0 ),
      dbl2( 0.0 ) {}
};
typedef OdArray<_ArcData>       CArcData;

/** \details
    This structure stores data for circles exported from Autodesk® Inventor®.

    <group !!RECORDS_tdrawings_apiref>
*/
struct  _CircleData
{
  OdGePoint3d   pnt;
  OdGeVector3d  vec;
  double        dbl;

  _CircleData() : dbl( 0.0 ) {}
};
typedef OdArray<_CircleData>       CCircleData;

/** \details
    This class implements the object enabler for complex entities exported from 
	drawings created with Autodesk® Inventor®.

    Library: Id
    <group OdId_Classes> 
*/
class IDVIEW_EXPORT OdIdComplexEntity : public OdDbEntity
{
  OdDbObjectId      m_pVportId;

  CLineData         m_LineData;
  OdUInt32          m_LogLineData;

  CArcData          m_ArcData;
  OdUInt32          m_LogArcData;

  CCircleData       m_CircleData;
  OdUInt32          m_LogCircleData;
public:
  ODDB_DECLARE_MEMBERS(OdIdComplexEntity);
  OdIdComplexEntity() : m_LogLineData(0), m_LogArcData(0), m_LogCircleData(0) {}

  OdDbObjectId vportId()  const     { return m_pVportId; }
  void setVportId(OdDbObjectId id)  { m_pVportId = id; }

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  void  addGraphicalArc(OdGePoint3d const &point,OdGeVector3d const &v1,double const &dbl1,OdGeVector3d const &v2,double const &dbl2);
  void  addGraphicalCircle(OdGePoint3d const &point,OdGeVector3d const &v,double const &dbl);
  void  addGraphicalLine(OdGePoint3d const *ppoint);
  void  addLogicalArc(OdGePoint3d const &,OdGeVector3d const &v1,double const &dbl1,OdGeVector3d const &v2,double const &dbl2);
  void  addLogicalCircle(OdGePoint3d const &,OdGeVector3d const &v,double const &dbl);
  void  addLogicalLine(OdGePoint3d const *ppoint);

  bool subWorldDraw(OdGiWorldDraw* pWd) const;

  OdResult subExplode(OdRxObjectPtrArray& entitySet) const;
};

#include "TD_PackPop.h"

#endif  // IDCOMPLEXENTITY_INCLUDED

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

