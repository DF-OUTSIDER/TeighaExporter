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

#ifndef _ODGSDBROOTLINKAGE_H_INCLUDED_
#define _ODGSDBROOTLINKAGE_H_INCLUDED_

#include "OdaCommon.h"
#include "RxObject.h"
#include "Gs/GsExport.h"
#include "Gi/GiDrawable.h"
#include "DbBaseHostAppServices.h"
#include "DbBaseDatabase.h"
#include "AbstractViewPE.h"

#include "TD_PackPush.h"

/** \details
    Provides connection between Gs module and DbRoot module export classes.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsDbRootLinkage
{
  public:
    OdGsDbRootLinkage() {}

    /** \details
        Initialize DbRoot link classes.
    */
    static void initialize();
    /** \details
        Uninitialize DbRoot link classes.
    */
    static void uninitialize();
    /** \details
        Returns true if and only if DbRoot link classes initialized.
    */
    static bool isInitialized();
    /** \details
        Checks does initialized at least one DbRoot link class.
    */
    static bool isInitializedAny();

    /** \details
        Returns OdDbBaseDatabasePE class descriptor.
    */
    static OdRxClass *getDbBaseDatabasePEClass();
    /** \details
        Returns OdDbBaseDatabasePE protocol extension linked with database object.
        \param pDb [in]  Database object pointer.
    */
    static OdDbBaseDatabasePE *getDbBaseDatabasePE(const OdDbBaseDatabase *pDb);
    /** \details
        Returns OdDbBaseDatabasePE protocol extension linked with drawable database object.
        \param pDrw [in]  Drawable pointer.
    */
    static OdDbBaseDatabasePE *getDbBaseDatabasePEForDrawable(const OdGiDrawable *pDrw, OdDbBaseDatabase *&pDb);

    /** \details
        Returns OdDbBaseHostAppServices class descriptor.
    */
    static OdRxClass *getDbBaseHostAppServicesClass();
    /** \details
        Returns OdDbBaseHostAppServices class pointer linked with host application services object.
        \param pServices [in]  Host application services object pointer.
    */
    static OdDbBaseHostAppServices *getDbBaseHostAppServices(const OdRxObject *pServices);
    /** \details
        Returns OdDbBaseHostAppServices class pointer linked with database object.
        \param pDb [in]  Database object pointer.
    */
    static OdDbBaseHostAppServices *getDatabaseDbBaseHostAppServices(const OdDbBaseDatabase *pDb);
    /** \details
        Displays warning message for specified database object.
        \param pDb [in]  Database object pointer.
        \param message [in]  Warning message.
    */
    static bool displayWarning(const OdDbBaseDatabase *pDb, const OdString &message);

    /** \details
        Returns OdDbBaseLayerPE class descriptor.
    */
    static OdRxClass *getDbBaseLayerPEClass();
    /** \details
        Returns OdDbBaseLayerPE protocol extension linked with layer object.
        \param pLayer [in]  Layer object pointer.
    */
    static OdDbBaseLayerPE *getDbBaseLayerPE(const OdRxObject *pLayer);

    /** \details
        Returns OdDbBaseVisualStylePE class descriptor.
    */
    static OdRxClass *getDbBaseVisualStylePEClass();
    /** \details
        Returns OdDbBaseVisualStylePE protocol extension linked with visual style object.
        \param pVisualStyle [in]  Visual Style object pointer.
    */
    static OdDbBaseVisualStylePE *getDbBaseVisualStylePE(const OdRxObject *pVisualStyle);


    /** \details
        Returns OdDbBaseAnnotationScalePE class descriptor.
    */
    static OdRxClass *getDbBaseAnnotationScalePEClass();
    /** \details
        Returns OdDbBaseAnnotationScalePE protocol extension linked with DbView object.
        \param pView [in]  view pointer.
    */
    static OdDbBaseAnnotationScalePE *getDbBaseAnnotationScalePE(const OdRxObject *pView);

    /** \details
        Returns OdDbBaseMaterialPE class descriptor.
    */
    static OdRxClass *getDbBaseMaterialPEClass();
    /** \details
        Returns OdDbBaseMaterialPE protocol extension linked with material object.
        \param pMaterial [in]  Material object pointer.
    */
    static OdDbBaseMaterialPE *getDbBaseMaterialPE(const OdRxObject *pMaterial);

    /** \details
        Returns OdDbBaseLayoutPE class descriptor.
    */
    static OdRxClass *getDbBaseLayoutPEClass();
    /** \details
        Returns OdDbBaseLayoutPE protocol extension linked with layout object.
        \param pLayout [in]  Layout object pointer.
    */
    static OdDbBaseLayoutPE *getDbBaseLayoutPE(const OdRxObject *pLayout);
    /** \details
        Checks that specified object is a paper layout drawable which is not combined with block table.
        \param pLayout [in]  Layout object pointer.
    */
    static bool isLayoutDrawable(const OdGiDrawable *pLayout);

    /** \details
        Returns OdAbstractViewPE class descriptor.
    */
    static OdRxClass *getAbstractViewPEClass();
    /** \details
        Returns OdAbstractViewPE protocol extension linked with layout object.
        \param pViewport [in]  Viewport object pointer.
    */
    static OdAbstractViewPE *getAbstractViewPE(const OdRxObject *pViewport);

    /** \details
        Returns OdDbBaseBlockPE class descriptor.
    */
    static OdRxClass *getDbBaseBlockPEClass();
    /** \details
        Returns OdDbBaseBlockPE protocol extension linked with block object.
        \param pBlock [in]  Block object pointer.
    */
    static OdDbBaseBlockPE *getDbBaseBlockPE(const OdRxObject *pBlock);

    /** \details
        Returns OdDbBaseBlockRefPE class descriptor.
    */
    static OdRxClass *getDbBaseBlockRefPEClass();
    /** \details
        Returns OdDbBaseBlockRefPE protocol extension linked with block reference object.
        \param pBlockRef [in]  Block reference object pointer.
    */
    static OdDbBaseBlockRefPE *getDbBaseBlockRefPE(const OdRxObject *pBlockRef);
    /** \details
        Returns true if drawable is a block reference.
        \param pBlockRef [in]  Block reference object pointer.
        \param bMInsert [out]  Set to true if drawable is MInsert.
    */
    static bool isBlockRefDrawable(const OdGiDrawable *pBlockRef, bool *bMInsert = NULL);

    /** \details
        Returns OdDbBaseSortEntsPE class descriptor.
    */
    static OdRxClass *getDbBaseSortEntsPEClass();
    /** \details
        Returns OdDbBaseSortEntsPE protocol extension linked with entity sorting object.
        \param pSortEnts [in]  Entity sorting object pointer.
    */
    static OdDbBaseSortEntsPE *getDbBaseSortEntsPE(const OdRxObject *pSortents);

    /** \details
        Returns OdDbBaseLongTransactionPE class descriptor.
    */
    static OdRxClass *getDbBaseLongTransactionPEClass();
    /** \details
        Returns OdDbBaseLongTransactionPE protocol extension linked with long transaction object.
        \param pLT [in]  Long transaction object pointer.
    */
    static OdDbBaseLongTransactionPE *getDbBaseLongTransactionPE(const OdRxObject *pLT);

    /** \details
    Returns OdDbBaseHatchPE class descriptor.
    */
    static OdRxClass *getDbBaseHatchPEClass();
    /** \details
    Returns OdDbBaseHatchPE protocol extension linked with hatch object.
    \param pHatch [in]  hatch object pointer.
    */
    static OdDbBaseHatchPE *getDbBaseHatchPE(const OdRxObject *pHatch);
    /** \details
    Returns true if drawable is a hatch.
    \param pHatch [in]  Hatch object pointer.
    */
    static bool isHatchDrawable(const OdGiDrawable *pHatch);
};

// OdDbStub helpers

/** \details
    Returns database for Object ID.
    \param pId [in]  Object persistent ID.
*/
GS_TOOLKIT_EXPORT OdDbBaseDatabase *odgsDbGetDatabase(OdDbStub *pId);

/** \details
    Returns owner for Object ID.
    \param pId [in]  Object persistent ID.
*/
GS_TOOLKIT_EXPORT OdDbStub *odgsDbGetOwner(OdDbStub *pId);

/** \details
    Returns redirected Object ID.
    \param pId [out]  Object persistent ID.
*/
GS_TOOLKIT_EXPORT OdDbStub *odgsDbRedirectID(OdDbStub *&pId);

/** \details
    Checks does Object ID is redirected.
    \param pId [in]  Object persistent ID.
*/
GS_TOOLKIT_EXPORT bool odgsDbObjectIDRedirected(OdDbStub *pId);

/** \details
    Resets Object ID redirection state.
    \param pId [in]  Object persistent ID.
    \param bSet [in]  New redirection state.
*/
GS_TOOLKIT_EXPORT void odgsDbObjectIDSetRedirected(OdDbStub *pId, bool bSet);

/** \details
    Resets Object ID locking state.
    \param pId [in]  Object persistent ID.
    \param bSet [in]  New locking state.
*/
GS_TOOLKIT_EXPORT void odgsDbObjectIDSetLockingFlag(OdDbStub *pId, bool bSet);

/** \details
    Returns database for redirected Object ID.
    \param pId [in]  Object persistent ID.
*/
GS_TOOLKIT_EXPORT OdDbBaseDatabase *odgsDbObjectIDRedirectedDatabase(OdDbStub *pId);

/** \details
    Checks that Object ID is erased.
    \param pId [in]  Object persistent ID.
*/
GS_TOOLKIT_EXPORT bool odgsDbObjectIDErased(OdDbStub *pId);

#include "TD_PackPop.h"

#endif // _ODGSDBROOTLINKAGE_H_INCLUDED_
