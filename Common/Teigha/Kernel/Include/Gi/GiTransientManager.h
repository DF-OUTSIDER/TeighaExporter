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

#ifndef __ODGITRANSIENTMANAGER_H__
#define __ODGITRANSIENTMANAGER_H__

#include <RxObject.h>
#include <Gi/GiExport.h>
#include <UInt32Array.h>

#include "TD_PackPush.h"

class OdGiDrawable;

/** \details
    This class provides transient drawables manager functionality.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiTransientManager : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiTransientManager);

    enum OdGiTransientDrawingMode
    {
      kOdGiMain = 0,
      kOdGiSprite,
      kOdGiDirectShortTerm,
      kOdGiHighlight,
      kOdGiDirectTopmost,
      kOdGiContrast,
      kOdGiDrawingModeCount
    };

    /** \details
        Adds new drawable into transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param mode [in]  Transient drawable mode.
        \param subMode [in]  Draw order index.
        \param viewportIds [in]  List of viewport indices.
    */
    virtual bool addTransient(OdGiDrawable *pDrawable,
                              OdGiTransientDrawingMode mode,
                              OdInt32 subMode,
                              const OdUInt32Array &viewportIds) = 0;

    /** \details
        Erases drawable from transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param viewportIds [in]  List of viewport indices.
    */
    virtual bool eraseTransient(OdGiDrawable *pDrawable,
                                const OdUInt32Array &viewportIds) = 0;

    /** \details
        Erases all drawables from specified drawable mode and order.
        \param mode [in]  Transient drawable mode.
        \param subMode [in]  Draw order index.
        \param viewportIds [in]  List of viewport indices.
    */
    virtual bool eraseTransients(OdGiTransientDrawingMode mode,
                                 OdInt32 subMode,
                                 const OdUInt32Array &viewportIds) = 0;

    /** \details
        Updates drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param viewportIds [in]  List of viewport indices.
    */
    virtual void updateTransient(OdGiDrawable *pDrawable,
                                 const OdUInt32Array &viewportIds) const = 0;

    /** \details
        Appends child drawable to parent drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param pParent [in]  Parent drawable pointer.
    */
    virtual bool addChildTransient(OdGiDrawable *pDrawable, OdGiDrawable *pParent) = 0;

    /** \details
        Erases child drawable from parent drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param pParent [in]  Parent drawable pointer.
    */
    virtual bool eraseChildTransient(OdGiDrawable *pDrawable, OdGiDrawable *pParent) = 0;

    /** \details
        Updates child drawable of parent drawable in the transient drawables manager.
        \param pDrawable [in]  Drawable pointer.
        \param pParent [in]  Parent drawable pointer.
    */
    virtual void updateChildTransient(OdGiDrawable *pDrawable, OdGiDrawable *pParent) const = 0;

    /** \details
        Finds free order for specified drawable mode.
        \param mode [in]  Transient drawable mode.
        \param subMode [out]  Draw order index.
        \param viewportIds [in]  List of viewport indices.
        \returns
        Returns 0 if specified input draw order isn't free and transient manager couldn't find free draw order.
        Returns 1 if specified input draw order is free.
        Returns 2 if specified input draw order isn't free but transient manager found free draw order.
    */
    virtual OdInt32 getFreeSubDrawingMode(OdGiTransientDrawingMode mode,
                                          OdInt32 &subMode,
                                          const OdUInt32Array &viewportIds) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiTransientManager object pointers.
*/
typedef OdSmartPtr<OdGiTransientManager> OdGiTransientManagerPtr;

/** \details
    Extracts transient manager using transient manager protocol extension from specified object.
*/
ODGI_EXPORT OdGiTransientManager *odgiGetTransientManager(const OdRxObject *pObject);
/** \details
    Sets transient manager using transient manager protocol extension for specified object.
*/
ODGI_EXPORT void odgiSetTransientManager(OdGiTransientManager *pManager, OdRxObject *pObject);

/** \details
    This class provides transient drawables manager protocol extension.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiTransientManagerPE : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiTransientManagerPE);

    /** \details
        Returns transient manager for specified object.
        \param pThis [in]  Pointer to object for which this procol extension was specified.
    */
    virtual OdGiTransientManager *transientManager(const OdRxObject *pThis) const = 0;
    /** \details
        Sets transient manager to specified object.
        \param pThis [in]  Pointer to object for which this protocol extension was specified.
        \param pManager [in]  Pointer to transient drawables manager.
    */
    virtual void setTransientManager(OdRxObject *pThis, OdGiTransientManager *pManager) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiTransientManagerPE object pointers.
*/
typedef OdSmartPtr<OdGiTransientManagerPE> OdGiTransientManagerPEPtr;

#include "TD_PackPop.h"

#endif  // __ODGITRANSIENTMANAGER_H__
