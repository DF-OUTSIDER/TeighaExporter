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

#ifndef OD_DBABSTRACTPLOTDATA_H
#define OD_DBABSTRACTPLOTDATA_H

#include "DbPlotSettings.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for Protocol Extension classes for OdDbViewport and OdDbPlotSettings objects.
    
    Corresponding C++ library: TD_Db
    
    \remarks
    Only the following objects are supported:
    
    *  OdDbViewport
    *  OdDbPlotSettings
    *  Derivatives of one of the above.

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbAbstractPlotData : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdDbAbstractPlotData);

    /** \details
      Sets the parameters for the specified Plot object according to the arguments.
      \param pDstPlotObj [in]  Pointer to the destination Plot object
      \param pSrcPlotObj [in]  Pointer to the source Plot object.
    */
    virtual void setProps(OdRxObject* pDstPlotObj, const OdRxObject* pSrcPlotObj) const;

    // Scale properties

    /** \details
      Returns true if and only if this Plot object uses a standard scale.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual bool useStandardScale(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Returns the custom print scale for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual double customScale(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Sets the custom scale of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param customScale [in]  Custom scale.
    */
    virtual void setCustomScale(OdRxObject* pPlotObj, double customScale) const = 0;

    /** \details
      Returns the standard scale type of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual OdDbPlotSettings::StdScaleType standardScale(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Sets the standard scale type of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param standardScale [in]  Standard scale type.
    */
    virtual void setStandardScale(OdRxObject* pPlotObj, const OdDbPlotSettings::StdScaleType standardScale) const = 0;

    // Plot style sheet

    /** \details
      Returns the current style sheet.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual OdString styleSheet(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Sets the plot style sheet name associated with this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param styleSheetName [in]  Style sheet name.
    */
    virtual void setStyleSheet(OdRxObject* pPlotObj, const OdString& styleSheetName) const = 0;

    // Plot shade

    /** \details
      Returns the shade plot type for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual OdDbPlotSettings::ShadePlotType shadePlot(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Sets the shade plot type for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param shadePlotSet [in]  New shade plot setting.
    */
    virtual void setShadePlot(OdRxObject* pPlotObj, OdDbPlotSettings::ShadePlotType shadePlotSet) const = 0;

    /** \details
      Returns the OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object referenced by this object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual OdDbObjectId shadePlotId(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Sets the shade plot type and OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param type [in]  New shade plot setting.
      \param shadePlotId [in]  OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object for this Plot object.
    */
    virtual void setShadePlot(OdRxObject* pPlotObj, OdDbPlotSettings::ShadePlotType type, OdDbObjectId shadePlotId) const = 0;

    // Shade modes

    /** \details
      Returns true if and only if the hidden line removal algorithm is to be applied to entities for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual bool plotHiddenLines(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Controls the application of the hidden line removal algorithm to entities for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param plotHidden [in]  True if and only hidden lines are to be removed.
    */
    virtual void setPlotHiddenLines(OdRxObject* pPlotObj, bool plotHidden) const = 0;

    /** \details
      Controls whether the output is vector-based when using shade plot settings of a viewport.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual bool plotWireframe(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Controls whether the output is raster-based when using shade plot settings of a viewport.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual bool plotAsRaster(const OdRxObject* pPlotObj) const = 0;

    // Plotting transparency

    /** \details
      Returns true if and only if transparencies plotting enabled for this Plot object.
    */
    virtual bool plotTransparency(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Controls transparencies plotting for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param bEnable [in]  New transparencies plotting setting.
    */
    virtual void setPlotTransparency(OdRxObject* pPlotObj, bool bEnable) const = 0;

    // Helpers

    /** \details
      Returns true if and only if this Plot object is of ModelSpace type.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    virtual bool modelType(const OdRxObject* pPlotObj) const = 0;

    /** \details
      Controls the ModelSpace type of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param modelTypeSet [in]  True if and only if it is of ModelSpace type.
    */
    virtual void setModelType(OdRxObject* pPlotObj, bool modelTypeSet) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbAbstractPlotData object pointers.
*/
typedef OdSmartPtr<OdDbAbstractPlotData> OdDbAbstractPlotDataPtr;

#include "TD_PackPop.h"

#endif //#ifndef OD_DBABSTRACTPLOTDATA_H
