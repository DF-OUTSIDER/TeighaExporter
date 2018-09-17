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




#ifndef OD_DBSETPLOTSETPE_H
#define OD_DBSETPLOTSETPE_H

#include "RxObject.h"
#include "DbPlotSettings.h"

/** \details
This class is a plot settings protocol extension class. The methods of the OdDbSetPlotSettingsPE provide the direct access to OdDbPlotSettings fields. 
Unlike the OdDbPlotSettingsValidator class, which can also be used for defining plot settings, OdDbSetPlotSettingsPE allows you to set data without validation.
<group OdDb_Classes> 

*/
class TOOLKIT_EXPORT OdDbSetPlotSettingsPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbSetPlotSettingsPE);
  //DOM-IGNORE-END

  /** \details
    Sets the plot device name for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotCfgName [in]  Name of the system printer or plot configuration PC3 file.

	\returns Returns eOk if successful or an appropriate error code otherwise. 
  */
  virtual OdResult setPlotCfgName(OdDbPlotSettings *pPlotSettings, const OdString &plotCfgName);

  /** \details
    Sets plot layout flags for the specified PlotSettings object.

    \param pPlotSettings [in] Pointer to the PlotSettings object.
    \param flags [in] Plot layout flags to be set.

	\returns Returns eOk if successful or an appropriate error code otherwise. 
  */
  virtual OdResult setPlotLayoutFlags(OdDbPlotSettings *pPlotSettings, OdInt16 flags);

  /** \details
    Sets the margins of the paper for the specified PlotSettings object.
    
	\param pPlotSettings [in] Pointer to the PlotSettings object.
    \param left [in] Sets the left margin. 
    \param Bottom [in] Sets the bottom margin.
    \param right [in] Sets the right margin. 
    \param top [in] Sets the top margin.
    
    \remarks
    Margins are measured from their respective edges to the plot area and are independent of plot rotation.
    All distances are returned in millimeters, regardless of the drawing units. 

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setMargins(OdDbPlotSettings *pPlotSettings, double left, double Bottom, double right, double top);

  /** \details
    Sets the size of the paper for the specified PlotSettings object.
	
	\param pPlotSettings [in] Pointer to the PlotSettings object.
    \param paperWidth [in] Sets the paper width. 
    \param paperHeight [in] Sets the paper height.

    \remarks
    Paper size is the physical paper size and includes margins.
    All distances are returned in millimeters, regardless of the drawing units. 

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotPaperSize(OdDbPlotSettings *pPlotSettings, double paperWidth, double paperHeight);
    
  /** \details
    Sets the canonical (locale-independent) media name for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param mediaName [in]  Canonical media name (case-sensitive).
    
    \remarks
    The canonical media name is locale-independent.
    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setMediaName(OdDbPlotSettings *pPlotSettings, OdString mediaName);

  /** \details
    Sets the plot origin for the specified PlotSettings object.

	\param pPlotSettings [in] Pointer to the PlotSettings object.
    \param origin [in]  Plot origin.
        
    \remarks
    The plot origin is measured with respect to the plot margin. All distances are returned in millimeters, regardless of the drawing units.

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setOrigin(OdDbPlotSettings *pPlotSettings, const OdGePoint2d &origin);

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

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotPaperUnits(OdDbPlotSettings *pPlotSettings, OdDbPlotSettings::PlotPaperUnits units);

  /** \details
    Sets the plot rotation for the specified PlotSettings object.

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

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotRotation(OdDbPlotSettings *pPlotSettings, OdDbPlotSettings::PlotRotation plotRotation);

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

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotType(OdDbPlotSettings *pPlotSettings, OdDbPlotSettings::PlotType plotType);

  /** \details
    Sets the corners of the plot window area for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param xMin [in]  X-coordinate of the lower-left corner.      
    \param yMin [in]  Y-coordinate of the lower-left corner.      
    \param xMax [in]  X-coordinate of the upper-right corner.      
    \param yMax [in]  Y-coordinate of the upper-right corner.      
    
    \remarks
    The corners define the area to be plotted if and only if plotType == kWindow.

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotWindowArea(OdDbPlotSettings *pPlotSettings, double xmin, double ymin, double xmax, double ymax);

  /** \details
    Sets the plot view for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotViewId [in]  Plot view Object Id to set.
  
    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotView(OdDbPlotSettings* pPlotSettings, OdDbObjectId plotViewId);

  /** \details
    Sets the custom print scale for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param numerator [in]  PaperSpace units.
    \param denominator [in]  Media units.
    
    \remarks
    The custom print scale always reflects the scale that this PlotSettings object will use to plot.

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPrintScale(OdDbPlotSettings* pPlotSettings, double numerator, double denominator);

  /** \details
    Sets the current style sheet for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param styleSheet [in]  Name of current style sheet.

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setStyleSheet(OdDbPlotSettings* pPlotSettings, OdString styleSheet);

  /** \details
    Sets the standard scale type for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param scaleType [in]  Standard scale type to be set.

    \remarks
    scaleType must be one of the following:
    
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

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setScaleType(OdDbPlotSettings* pPlotSettings, OdDbPlotSettings::StdScaleType scaleType);

  /** \details
    Sets the scale factor, as a floating point value, for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param dScaleFactor [in]  Scale factor.
  */
  virtual OdResult setScaleFactor(OdDbPlotSettings* pPlotSettings, double dScaleFactor);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSetPlotSettingsPE object pointers.
*/
typedef OdSmartPtr<OdDbSetPlotSettingsPE> OdDbSetPlotSettingsPEPtr;

#endif // OD_DBSETPLOTSETPE_H

