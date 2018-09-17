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

#ifndef OD_DBABSTRACTPLOTDATAFORDBVIEWPORT_H
#define OD_DBABSTRACTPLOTDATAFORDBVIEWPORT_H

#include "DbAbstractPlotData.h"

#include "TD_PackPush.h"

/** \details
    This class is the Protocol Extension class for OdDbViewport objects.
    
    Corresponding C++ library: TD_Db

    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbAbstractPlotDataForDbViewport : public OdDbAbstractPlotData
{
  public:
    ODRX_DECLARE_MEMBERS(OdDbAbstractPlotDataForDbViewport);

    // Scale properties

    /** \details
      Returns true if and only if this Plot object uses a standard scale.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    bool useStandardScale(const OdRxObject* pPlotObj) const;

    /** \details
      Returns the custom print scale for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    double customScale(const OdRxObject* pPlotObj) const;

    /** \details
      Sets the custom scale of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param customScale [in]  Custom scale.
    */
    void setCustomScale(OdRxObject* pPlotObj, double customScale) const;

    /** \details
      Returns the standard scale type of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    OdDbPlotSettings::StdScaleType standardScale(const OdRxObject* pPlotObj) const;

    /** \details
      Sets the standard scale type of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param standardScale [in]  Standard scale type.
    */
    void setStandardScale(OdRxObject* pPlotObj, const OdDbPlotSettings::StdScaleType standardScale) const;

    // Plot style sheet

    /** \details
      Returns the current style sheet.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    OdString styleSheet(const OdRxObject* pPlotObj) const;

    /** \details
      Sets the plot style sheet name associated with this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param styleSheetName [in]  Style sheet name.
    */
    void setStyleSheet(OdRxObject* pPlotObj, const OdString& styleSheetName) const;

    // Plot shade

    /** \details
      Returns the shade plot type for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    OdDbPlotSettings::ShadePlotType shadePlot(const OdRxObject* pPlotObj) const;

    /** \details
      Sets the shade plot type for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param shadePlotSet [in]  New shade plot setting.
    */
    void setShadePlot(OdRxObject* pPlotObj, OdDbPlotSettings::ShadePlotType shadePlotSet) const;

    /** \details
      Returns the OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object referenced by this object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    OdDbObjectId shadePlotId(const OdRxObject* pPlotObj) const;

    /** \details
      Sets the shade plot type and OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param type [in]  New shade plot setting.
      \param shadePlotId [in]  OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object for this Plot object.
    */
    void setShadePlot(OdRxObject* pPlotObj, OdDbPlotSettings::ShadePlotType type, OdDbObjectId shadePlotId) const;

    // Shade modes

    /** \details
      Returns true if and only if the hidden line removal algorithm is to be applied to entities for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    bool plotHiddenLines(const OdRxObject* pPlotObj) const;

    /** \details
      Controls the application of the hidden line removal algorithm to entities for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param plotHidden [in]  True if and only hidden lines are to be removed.
    */
    void setPlotHiddenLines(OdRxObject* pPlotObj, bool plotHidden) const;

    /** \details
      Controls whether the output is vector-based when using shade plot settings of a viewport.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    bool plotWireframe(const OdRxObject* pPlotObj) const;

    /** \details
      Controls whether the output is raster-based when using shade plot settings of a viewport.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    bool plotAsRaster(const OdRxObject* pPlotObj) const;

    // Plotting transparency

    /** \details
      Returns true if and only if transparencies plotting enabled for this Plot object.
    */
    bool plotTransparency(const OdRxObject* pPlotObj) const;

    /** \details
      Controls transparencies plotting for this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param bEnable [in]  New transparencies plotting setting.
    */
    void setPlotTransparency(OdRxObject* pPlotObj, bool bEnable) const;

    // Helpers

    /** \details
      Returns true if and only if this Plot object is of ModelSpace type.
      \param pPlotObj [in]  Pointer to the Plot object.
    */
    bool modelType(const OdRxObject* pPlotObj) const;

    /** \details
      Controls the ModelSpace type of this Plot object.
      \param pPlotObj [in]  Pointer to the Plot object.
      \param modelTypeSet [in]  True if and only if it is of ModelSpace type.
    */
    void setModelType(OdRxObject* pPlotObj, bool modelTypeSet);
};

#include "TD_PackPop.h"

#endif //#ifndef OD_DBABSTRACTPLOTDATAFORDBVIEWPORT_H
