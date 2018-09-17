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


#ifndef _ODBREPBUILDERFILLERMODULEIMPL_INCLUDED_
#define _ODBREPBUILDERFILLERMODULEIMPL_INCLUDED_

#include "BrepBuilderFillerDef.h"
#include "RxDynamicModule.h"

#include "Ge/GeCurve2dPtrArray.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "Ge/GeSurfacePtrArray.h"
#include "Ge/GeCurve2d.h"
#include "Ge/GeCurve3d.h"
#include "Ge/GeSurface.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "BrepBuilderInitialParams.h"

class OdBrepBuilder;
class OdBrBrep;
struct BrepBuilderInitialData;
class OdIMaterialAndColorHelper;

/** \details
  This class implements the module for BrepBuilderFiller.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group Extension_Classes>
*/
class BREPBUILDERFILLER_DLL OdBrepBuilderFillerModule : public OdRxModule
{
protected:
  /** \details
    Initializes the BrepBuilderFiller module.
  */
  void initApp();

  /** \details
    Uninitializes the BrepBuilderFiller module.
  */
  void uninitApp();

public:
  OdResult initFromNURBSingleFace(OdBrepBuilder& builder, const OdBrBrep& brep, OdGeCurve3dPtrArray& arrEdges, OdGeCurve2dPtrArray& arrCoedges, OdGeSurfacePtr &surface);

  /** \details
  Get data from OdBrBrep for later initialisation OdBrepBuilder.
  */
  OdResult getDataFrom(const OdBrBrep& brep,
    OdIMaterialAndColorHelper* pMaterialHelper,
    const OdBrepBuilderFillerParams& params,
    BrepBuilderInitialData& data);
  /** \details
  Init the OdBrepBuilder with data gotten from OdBrBrep.
  */
  OdResult initFrom(OdBrepBuilder& builder, const BrepBuilderInitialData& data,
    OdGeCurve3dPtrArray& arrEdges, OdGeCurve2dPtrArray& arrCoedges, OdGeSurfacePtrArray& arrSurfaces);

  /** \details
  Init the OdBrepBuilder with geometries from OdBrBrep.
  \param builder [out] BrepBuilder that will be inited.
  \param brep [in] B-Rep which will be used to collect data.
  \param arrEdges [inout] Edge curves. Its lifetime should be equal or more then lifetime of brep builder.
  \param arrCoedges [inout] Coedge curves. Its lifetime should be equal or more then lifetime of brep builder.
  \param arrSurfaces [inout] Boundary surfaces. Its lifetime should be equal or more then lifetime of brep builder.
  \param pMaterialHelper [inout] Helper class to receive visual information. Should be valid while calling initFrom.
  \param params [in] Optinal parameters to reset default behavior.
  */
  OdResult initFrom(OdBrepBuilder& builder, const OdBrBrep& brep,
    OdGeCurve3dPtrArray& arrEdges, OdGeCurve2dPtrArray& arrCoedges, OdGeSurfacePtrArray& arrSurfaces,
    OdIMaterialAndColorHelper* pMaterialHelper = NULL,
    const OdBrepBuilderFillerParams& params = OdBrepBuilderFillerParams());
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdBrepBuilderFillerModule object pointers.
*/
typedef OdSmartPtr<OdBrepBuilderFillerModule> OdBrepBuilderFillerModulePtr;

#endif //_ODBREPBUILDERFILLERMODULEIMPL_INCLUDED_
