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

#ifndef _DGN_IMPORTDIMASSOC_INCLUDED_ 
#define _DGN_IMPORTDIMASSOC_INCLUDED_

#include <DgDatabase.h>
#include <DbDimension.h>
#include "DgnImportContext.h"
#include "DbDimAssoc.h"
#include "DgAttributeLinkage.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DGN_IMPORT 
{
  void createLinearDimensionAssoc( OdDbDimensionPtr& pDim, const OdDgnImportDimAssocDescriptor& assocDiscr, OdDgDatabase* pDg );
  void createAngularDimensionAssoc( OdDbDimensionPtr& pDim, const OdDgnImportDimAssocDescriptor& assocDiscr, OdDgDatabase* pDg );
  void createRadialDimensionAssoc( OdDbDimensionPtr& pDim, const OdDgnImportDimAssocDescriptor& assocDiscr, OdDgDatabase* pDg );
  void createOrdinateDimensionAssoc( OdDbDimensionPtr& pDim, const OdDgnImportDimAssocDescriptor& assocDiscr, OdDgDatabase* pDg );

  bool addLinearPointAssociation( OdDbDimAssocPtr& pDimAssoc, const OdDgLinearAssociationPtr& pDgnAssoc, 
                                  const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, OdArray<OdDbObjectPtr>& arrObjects );

  bool addProjectrionPointAssociation( OdDbDimAssocPtr& pDimAssoc, const OdDgProjectionAssociationPtr& pDgnAssoc, 
                                  const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, OdArray<OdDbObjectPtr>& arrObjects );

  bool addArcPointAssociation( OdDbDimAssocPtr& pDimAssoc, const OdDgArcAssociationPtr& pDgnAssoc, 
                                const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, OdArray<OdDbObjectPtr>& arrObjects );

  bool addBSplineCurvePointAssociation( OdDbDimAssocPtr& pDimAssoc, const OdDgBSplineCurveAssociationPtr& pDgnAssoc, 
                                const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, OdArray<OdDbObjectPtr>& arrObjects );

  bool addOriginPointAssociation( OdDbDimAssocPtr& pDimAssoc, const OdDgOriginAssociationPtr& pDgnAssoc, 
                                const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, OdArray<OdDbObjectPtr>& arrObjects );

  bool addIntersect2PointAssociation( OdDbDimAssocPtr& pDimAssoc, const OdDgIntersect2AssociationPtr& pDgnAssoc, 
                                const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, const OdGePoint3d& ptDim, 
                                OdArray<OdDbObjectPtr>& arrObjects );

  bool addDimAssocPoint(OdDbDimAssocPtr& pDimAssoc, const OdDgAssocPointRootPtr& pDgnAssoc, 
                        const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, const OdGePoint3d& ptDim,
                        OdArray<OdDbObjectPtr>& arrObjects );

  bool assocDimPointToArc(  OdDbDimAssocPtr& pDimAssoc, const OdDgAssocPointRootPtr& pDgnAssoc, 
                            const OdDbDimAssoc::PointType& ptType, OdDgDatabase* pDg, const OdGePoint3d& ptDim,
                            OdArray<OdDbObjectPtr>& arrObjects );
}
#endif // _DGN_IMPORTDIMASSOC_INCLUDED_
