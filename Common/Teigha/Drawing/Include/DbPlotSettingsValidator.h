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




#ifndef OD_DBPLOTSETVAL_H
#define OD_DBPLOTSETVAL_H

#include "RxObject.h"
#include "DbPlotSettings.h"
#include "OdArray.h"

class OdDbPlotSettings;

typedef OdDbPlotSettings::PlotPaperUnits PlotPaperUnits;
typedef OdDbPlotSettings::PlotRotation   PlotRotation;
typedef OdDbPlotSettings::PlotType       PlotType;
typedef OdDbPlotSettings::StdScaleType   StdScaleType;

/** \details
    This class defines access methods that validate data for
    OdDbPlotSettings objects prior to setting the data on the
    objects.
    
    \sa
    TD_Db
    
    \remarks
    Plot device and style lists should be refreshed with refreshLists() prior to
    using the other methods in this class, in case any plot devices, PC3 files, or plot style tables were changed
    after starting your application.
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbPlotSettingsValidator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPlotSettingsValidator);

  /** \details
    Sets the plot device and media names for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotDeviceName [in]  Name of the system printer or PC3 file.
    \param mediaName [in]  Media name.
    
    \remarks
    (plotDeviceName == none_device) requires (mediaName == none_user_media)
  */
  virtual OdResult setPlotCfgName(
    OdDbPlotSettings* pPlotSettings,
    const OdString& plotDeviceName,
    const OdString& mediaName = OdString::kEmpty) = 0;

  /** \details
    Sets the canonical (locale-independent) media name for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param mediaName [in]  Canonical media name.
    
    \remarks
    The canonical media name is locale-independent.
    
    \sa
    getLocaleMediaName()

    \remarks
    Canonical media names are case-sensitive.
  */
  virtual OdResult setCanonicalMediaName(
    OdDbPlotSettings* pPlotSettings,
    const OdString& mediaName) = 0;

  /** \details
    Sets the plot origin offset for the specified PlotSettings object.

    \param xCoordinate [out]  X-coordinate of the origin offset.
    \param yCoordinate [out]  Y-coordinate of the origin offset.
    
    \remarks
    The plot origin offset is measured with respect to the plot margin.
     
    \remarks
    All distances are returned in millimeters, regardless of the drawing units. 
  */
  virtual OdResult setPlotOrigin(
    OdDbPlotSettings *pPlotSettings,
    double xCoordinate,
    double yCoordinate) = 0;


  /** \details
    Sets the plot paper units for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotPaperUnits [in]  Plot paper units.
        
    \remarks
    The plot paper units determine the units of the margins, offsets, paper size, and drawing units.
    
    plotPaperUnits must be one of the following:
    
    <table>
    Name            Value   Description
    kInches         0       Inches
    kMillimeters    1       Millimeters
    kPixels         2       Pixels
    </table>
  */
  virtual OdResult setPlotPaperUnits(
    OdDbPlotSettings* pPlotSettings,
    const PlotPaperUnits plotPaperUnits) = 0;


  /** \details
    Sets the plot rotation for specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotRotation [in]  Plot rotation.
    
    \remarks
    plotRotation must be one of the following:
    
    <table>
    Name         Value    Description
    k0degrees    0        No rotation
    k90degrees   1        90° CCW
    k180degrees  2        Inverted°
    k270degrees  3        90° CW
    </table>
  */
  virtual OdResult setPlotRotation(
    OdDbPlotSettings* pPlotSettings,
    const PlotRotation plotRotation) = 0;

  /** \details
    Controls the centering of the plot for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotCentered [in]  Plot centered.
  */
  virtual OdResult setPlotCentered(
    OdDbPlotSettings* pPlotSettings,
    bool plotCentered) = 0;

  /** \details
    Sets the plot type for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotType [in]  Plot type.

    \remarks
    plotType must be one of the following:
    
    <table>
    Name        Value   Description
    kDisplay    0       Display
    kExtents    1       Extents
    kLimits     2       Limits
    kView       3       View
    kWindow     4       Window
    kLayout     5       Layout
    </table>
  */
  virtual OdResult setPlotType(
    OdDbPlotSettings* pPlotSettings,
    const PlotType plotType) = 0;

  /** \details
    Sets the corners of the plot window area for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param xMin [in]  X-coordinate of the lower-left corner.      
    \param yMin [in]  Y-coordinate of the lower-left corner.      
    \param xMax [in]  X-coordinate of the upper-right corner.      
    \param yMax [in]  Y-coordinate of the upper-right corner.      
    
    \remarks
    The corners define the area to be plotted if and only if plotType == kWindow.
  */
  virtual OdResult setPlotWindowArea(
    OdDbPlotSettings* pPlotSettings,
    double xMin,
    double yMin,
    double xMax,
    double yMax) = 0;

  /** \details
    Sets the plot view name for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotViewName [in]  Plot view name.
  
    \remarks
    This string specifies the named view to be plotted if and only if plotType == kView. 
  */
  virtual OdResult setPlotViewName(
    OdDbPlotSettings* pPlotSettings,
    const OdString& plotViewName) = 0;

  /** \details
    Controls the use of a standard scale for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param useStandardScale [in]  Use standard scale.
  */
  virtual OdResult setUseStandardScale(
    OdDbPlotSettings* pPlotSettings,
    bool useStandardScale) = 0;

  /** \details
    Sets the custom print scale for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param numerator [in]  Receives the PaperSpace units.
    \param denominator [in]  Receives the media units.
    
    \remarks
    The custom print scale always reflects the scale that this PlotSettings object will use to plot.
  */
  virtual OdResult setCustomPrintScale(
    OdDbPlotSettings* pPlotSettings,
    double numerator,
    double denominator) = 0;

  /** \details
    Sets the current style sheet for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param currentStyleSheet [in]  Name of current style sheet.
  */
  virtual OdResult setCurrentStyleSheet(
    OdDbPlotSettings* pPlotSettings,
    const OdString& currentStyleSheet) = 0;

  /** \details
    Sets the standard scale type for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param stdScaleType [in]  Name of current style sheet.

    \remarks
    stdScaleType must be one of the of the following
    
    <table>
    Name            Value   Scale
    kScaleToFit     0       Scaled to Fit
    k1_128in_1ft    1       1/128=1'
    k1_64in_1ft     2       1/64=1'
    k1_32in_1ft     3       1/32=1'
    k1_16in_1ft     4       1/16=1'
    k3_32in_1ft     5       3/32=1'
    k1_8in_1ft      6       1/8=1'
    k3_16in_1ft     7       3/16=1'
    k1_4in_1ft      8       1/4=1'
    k3_8in_1ft      9       3/8=1'
    k1_2in_1ft      10      1/2=1'
    k3_4in_1ft      11      3/4=1'
    k1in_1ft        12      1=1'
    k3in_1ft        13      3=1'
    k6in_1ft        14      6=1'
    k1ft_1ft        15      1'=1'
    k1_1            16      1:1
    k1_2            17      1:2
    k1_4            18      1:4
    k1_5            19      1:5
    k1_8            20      1:8
    k1_10           21      1:10
    k1_16           22      1:16
    k1_20           23      1:20
    k1_30           24      1:30
    k1_40           25      1:40
    k1_50           26      1:50
    k1_100          27      1:100
    k2_1            28      2:1
    k4_1            29      4:1
    k8_1            30      8:1
    k10_1           31      10:1
    k100_1          32      100:1
    k1000_1         33      1000:1
    k1and1_2in_1ft  34      1.5"=1'
    </table>
  */
  virtual OdResult setStdScaleType(
    OdDbPlotSettings* pPlotSettings,
    const StdScaleType stdScaleType) = 0;

  /** \details
    Sets the standard scale, as a floating point value, for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param standardScale [in]  Standard scale.
  */
  virtual OdResult setStdScale(
    OdDbPlotSettings* pPlotSettings,
    double standardScale) = 0;
  
  /** \details
    Returns an array of all available system printers and PC3 files.
    
    \remarks
    Plot device and style lists should be refreshed with refreshLists() prior to
    using the other methods in this class, in case any plot devices, PC3 files, or plot style tables were changed
    after starting your application.
    
    \param deviceList [out]  Receives the plot device list.
  */
  virtual void plotDeviceList(
    OdArray<const OdChar*> & deviceList) = 0;

  /** \details
    Returns an array canonical (locale-independent) media names for the specified
    PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param mediaList [out]  Receives the media name list.

    \remarks
    Plot device and style lists should be refreshed with refreshLists() prior to
    using the other methods in this class, in case any plot devices, PC3 files, or plot style tables were changed
    after starting your application.
  */
  virtual OdResult canonicalMediaNameList(
    OdDbPlotSettings* pPlotSettings,
    OdArray<const OdChar*> & mediaList) = 0;

  /** \details
    Returns the locale-dependent media name for the specified PlotSettings object and canonical media name or index.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param canonicalName [out]  Locale-independent media name.
    
    \remarks
    Plot device and style lists should be refreshed with refreshLists() prior to
    using the other methods in this class, in case any plot devices were added or removed
    after starting your application.
  */
  virtual OdString getLocaleMediaName(
    OdDbPlotSettings *pPlotSettings,
    const OdString&  canonicalName) = 0;

  /** \details
    Returns the locale-dependent media name for the specified PlotSettings object and index.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param mediaIndex [in]  Media index.
  */
  virtual OdString getLocaleMediaName(
    OdDbPlotSettings *pPlotSettings,
    int mediaIndex) = 0;

  /** \details
    Sets the media name closest to the specified parameters for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param paperWidth [in]  Paper width.
    \param paperHeight [in]  Paper height.
    \param plotPaperUnits [in]  Plot paper units.
    \param matchPrintableArea [in]  True to match against printable area or physical media size.

    \remarks
    The plot paper units determine the units of the margins, offsets, paper size, and drawing units.

    plotPaperUnits must be one of the following:
    
    <table>
    Name            Value   Description
    kInches         0       Inches
    kMillimeters    1       Millimeters
    kPixels         2       Pixels
    </table>
  */
   virtual OdResult setClosestMediaName(
    OdDbPlotSettings* pPlotSettings,
    double paperWidth,
    double paperHeight,
    const PlotPaperUnits plotPaperUnits,
    bool matchPrintableArea) = 0;

  /** \details
    Returns an array of all available plot style tables.
    
    \remarks
    Plot device and style lists should be refreshed with refreshLists() prior to
    using the other methods in this class, in case any plot devices, PC3 files, or plot style tables were changed
    after starting your application.
    
    \param styleList [out]  Receives the plot style list.
  */
  virtual OdResult plotStyleSheetList(
    OdArray<const OdChar*> & styleList) = 0;

  /** \details
    Refreshes the plot device and style lists for the specified PlotSettings object.
    
    \remarks
    Plot device and style lists should be refreshed with refreshLists() prior to
    using the other methods in this class, in case any plot devices, PC3 files, or plot style tables were changed
    after starting your application.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
  */
  virtual void refreshLists(
    OdDbPlotSettings* pPlotSettings) = 0;


  /** \details
    Controls the Zoom to Paper on Update setting for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param zoomToPaperOnUpdate [in]  Zoom to Paper on Update.
  */
  virtual OdResult setZoomToPaperOnUpdate(
    OdDbPlotSettings* pPlotSettings,
    bool zoomToPaperOnUpdate) = 0;

  /** \details
    Applies the default plot configuration settings to the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
  */
  virtual OdResult setDefaultPlotConfig(
    OdDbPlotSettings* pPlotSettings) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbPlotSettingsValidator object pointers.
*/
typedef OdSmartPtr<OdDbPlotSettingsValidator> OdDbPlotSettingsValidatorPtr;

#endif // OD_DBPLOTSETVAL_H

