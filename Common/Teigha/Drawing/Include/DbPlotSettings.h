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

#ifndef OD_DBPLOTSETTINGS_H
#define OD_DBPLOTSETTINGS_H

#include "TD_PackPush.h"

#include "DbObject.h"
#include "Ge/GePoint2d.h"

/** \details
    This class represents PlotSettings objects in an OdDbDatabase instance.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbPlotSettings : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbPlotSettings);

  OdDbPlotSettings();

  enum PlotPaperUnits
  {
    kInches         = 0,   // Inches
    kMillimeters    = 1,   // Millimeters
    kPixels         = 2    // Pixels
  };

  enum PlotRotation
  {
    k0degrees       = 0,   // No rotation
    k90degrees      = 1,   // 90° CCW
    k180degrees     = 2,   // Inverted
    k270degrees     = 3    // 90° CW
  };

  enum PlotType
  {
    kDisplay        = 0,   // Display
    kExtents        = 1,   // Extents
    kLimits         = 2,   // Limits
    kView           = 3,   // View
    kWindow         = 4,   // Window
    kLayout         = 5    // Layout
  };

  enum StdScaleType
  {
    kScaleToFit     = 0,   // Scaled to Fit
    k1_128in_1ft    = 1,   // 1/128" = 1'
    k1_64in_1ft     = 2,   // 1/64" = 1'
    k1_32in_1ft     = 3,   // 1/32" = 1'
    k1_16in_1ft     = 4,   // 1/16" = 1'
    k3_32in_1ft     = 5,   // 3/32" = 1'
    k1_8in_1ft      = 6,   // 1/8" = 1'
    k3_16in_1ft     = 7,   // 3/16" = 1'
    k1_4in_1ft      = 8,   // 1/4" = 1'
    k3_8in_1ft      = 9,   // 3/8" = 1'
    k1_2in_1ft      = 10,  // 1/2" = 1'
    k3_4in_1ft      = 11,  // 3/4" = 1'
    k1in_1ft        = 12,  // 1" = 1'
    k3in_1ft        = 13,  // 3" = 1'
    k6in_1ft        = 14,  // 6" = 1'
    k1ft_1ft        = 15,  // 1' = 1'
    k1_1            = 16,  // 1:1
    k1_2            = 17,  // 1:2
    k1_4            = 18,  // 1:4
    k1_5            = 19,  // 1:5
    k1_8            = 20,  // 1:8
    k1_10           = 21,  // 1:10
    k1_16           = 22,  // 1:16
    k1_20           = 23,  // 1:20
    k1_30           = 24,  // 1:30
    k1_40           = 25,  // 1:40
    k1_50           = 26,  // 1:50
    k1_100          = 27,  // 1:100
    k2_1            = 28,  // 2:1
    k4_1            = 29,  // 4:1
    k8_1            = 30,  // 8:1
    k10_1           = 31,  // 10:1
    k100_1          = 32,  // 100:1
    k1000_1         = 33,  // 1000:1
    k1and1_2in_1ft  = 34  // 1.5"= 1' 
  };

  enum ShadePlotType 
  {
    kAsDisplayed    = 0, // As displayed
    kWireframe      = 1, // Wireframe
    kHidden         = 2, // Hidden lines removed
    kRendered       = 3, // Rendered
    kVisualStyle    = 4, // Visual style
    kRenderPreset   = 5  // Render preset
  };

  enum ShadePlotResLevel 
  {
    kDraft          = 0, // Draft
    kPreview        = 1, // Preview
    kNormal         = 2, // Normal
    kPresentation   = 3, // Presentation
    kMaximum        = 4, // Maximum
    kCustom         = 5  // Custom
  };

  /** \details
    Adds this PlotSettings object to the PlotSettings dictionary in the specified database.

    \param pDb [in]  Pointer to the database.
  */
  void addToPlotSettingsDict(
    OdDbDatabase *pDb);

  /** \details
    Returns the name of this PlotSettings object (DXF 1).
  */
  OdString getPlotSettingsName() const;

  /** \details
    Sets the name of this PlotSettings object (DXF 1).
    
    \param plotSettingsName [in]  PlotSettings name.  
  */
  void setPlotSettingsName(
    const OdString& plotSettingsName);

  /** \details
    Returns the name of the system printer or plot configuration file for this PlotSettings object (DXF 2).
  */
  OdString getPlotCfgName() const;

  /** \details
    Returns the margins of the paper for this PlotSettings object.
    
    \param leftMargin [out]  Receives the left margin. 
    \param bottomMargin[out] Receives the bottom margin.
    \param rightMargin [out] Receives the right margin. 
    \param topMargin [out] Receives the top margin.
    
    \remarks
    Margins are measured from their respective edges to the plot area and are independent of plot rotation.
    All distances are returned in millimeters, regardless of the drawing units. 
  */
  void getPlotPaperMargins(
    double& leftMargin, 
    double& bottomMargin,
    double& rightMargin, 
    double& topMargin) const;

  /** \details
    Returns the size of the paper for this PlotSettings object (DXF 44, 45).

    \remarks
    Paper size is the physical paper size and includes margins.
    All distances are returned in millimeters, regardless of the drawing units. 
  */
  void getPlotPaperSize(
    double& paperWidth, 
    double& paperHeight) const;

  /** \details
    Returns the canonical media name for the paper id of this PlotSettings object (DXF 4).
    
    \sa
    OdDbPlotSettingsValidator::getLocaleMediaName()
  */
  OdString getCanonicalMediaName() const;

  /** \details
    Returns the plot origin offset for this PlotSettings object (DXF 46, 47).

    \param xCoordinate [out]  Receives the X-coordinate of the origin offset.
    \param yCoordinate [out]  Receives the Y-coordinate of the origin offset.
    
    \remarks
    The plot origin offset is measured with respect to the plot margin.
    All distances are returned in millimeters, regardless of the drawing units. 
  */
  void getPlotOrigin(
    double& xCoordinate, 
    double& yCoordinate) const;

  /** \details
    Returns the plot paper units used for margins, offsets, paper size, and drawing units of this PlotSettings object (DXF 72).

    \remarks
    plotPaperUnits() returns one of the following:
    
    <table>
    Name            Value   Description
    kInches         0       Inches
    kMillimeters    1       Millimeters
    kPixels         2       Pixels
    </table>
  */
  OdDbPlotSettings::PlotPaperUnits plotPaperUnits() const;

  /** \details
    Returns whether viewport borders are plotted for this PlotSettings object (DXF 70, bit 0x01).
    
    \returns
    Returns true if and only if viewport borders are to be plotted.
  */
  bool plotViewportBorders() const;

  /** \details
    Controls whether viewport borders are plotted for this PlotSettings object (DXF 70, bit 0x01).

    \param plotViewportBorders [in]  True if and only if viewport borders are to be plotted.
  */
  void setPlotViewportBorders(
    bool plotViewportBorders);

  /** \details
    Returns whether plotstyles are applied for this PlotSettings object (DXF 70, bit 0x20).
      
    \returns
    Returns true if and only if plot styles are to be plotted.
  */
  bool plotPlotStyles() const;

  /** \details
    Controls whether plotstyles are applied for this PlotSettings object (DXF 70, bit 0x20).
    
    \param plotPlotStyles [in]  True if and only if plot styles are to be plotted.
  */
  void setPlotPlotStyles(
    bool plotPlotStyles);

  /** \details
    Returns whether plotstyles display for this PlotSettings object during layout mode (DXF 70, bit 0x02).
  */
  bool showPlotStyles() const;

  /** \details
    Controls whether plotstyles display for this PlotSettings object during layout mode (DXF 70, bit 0x02).

    \param showPlotStyles [in]  True if and only if plot styles are to be shown.
  */
  void setShowPlotStyles(
    bool showPlotStyles);

  /** \details
    Returns the plot rotation for this PlotSettings object (DXF 73).
    
    \remarks
    plotRotation() returns one of the following:
    
    <table>
    Name         Value    Description
    k0degrees    0        No rotation
    k90degrees   1        90° CCW
    k180degrees  2        Inverted°
    k270degrees  3        90° CW
    </table>
  */
  OdDbPlotSettings::PlotRotation plotRotation() const;

  /** \details
    Returns true if an only if the plot is to be centered for this PlotSettings object (DXF 70, bit 0x04).
  */
  bool plotCentered() const;

  /** \details
    Returns true if and only if the hidden line removal algorithm 
    is to be applied to PaperSpace entities for this PlotSettings object (DXF 70, bit 0x08). 
  */
  bool plotHidden() const;

  /** \details
    Controls the application of the hidden line removal algorithm 
    to PaperSpace entities for this PlotSettings object (DXF 70, bit 0x08). 

    \param plotHidden [in]  True if and only hidden lines are to be removed.
  */
  void setPlotHidden(bool plotHidden);

  /** \details
    Returns the plot type for this PlotSettings Object (DXF 74).

    \remarks
    plotType() returns one of the following:
    
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
  OdDbPlotSettings::PlotType plotType() const;

  /** \details
    Returns the corners of the plot window area for this PlotSettings object (DXF 48, 49, 140, 141).

    \param xMin [in]  Receives the X-coordinate of the lower-left corner.      
    \param yMin [in]  Receives the Y-coordinate of the lower-left corner.      
    \param xMax [in]  Receives the X-coordinate of the upper-right corner.      
    \param yMax [in]  Receives the Y-coordinate of the upper-right corner.      
    
    \remarks
    The corners define the area to be plotted if and only if plotType == kWindow.
  */
  void getPlotWindowArea(
    double& xMin, 
    double& yMin,
    double& xMax, 
    double& yMax) const;

  /** \details
    Returns the plot view name for this PlotSettings object (DXF 6).

    \remarks
    This string specifies the named view to be plotted if and only if plotType == kView. 
  */
  OdString getPlotViewName() const;

  /** \details
    Returns true if and only if this PlotSettings object uses a standard scale (DXF 70, bit 0x10).
  */
  bool useStandardScale() const;

  /** \details
    Returns the custom print scale for this PlotSettings object (DXF 142, 143).
    
    \param numerator [in]  Receives PaperSpace units.
    \param denominator [in]  Receives media units.
    
    \remarks
    The custom print scale always reflects the scale that this PlotSettings object will use to plot.
  */
  void getCustomPrintScale(
    double& numerator, 
    double& denominator) const;

  /** \details
    Returns the current style sheet (DXF 7).
  */
  OdString getCurrentStyleSheet() const;

  /** \details
    Returns the standard scale type for this PlotSettings object (DXF 75).
    
    \remarks
    stdScaleType() returns one of the of the following
    
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
  OdDbPlotSettings::StdScaleType stdScaleType() const;


  /** \details
    Returns the standard scale, as a floating point value, for this PlotSettings object (DXF 147).
    
    \param standardScale [out]  Receives the standard scale.
  */
  void getStdScale(
    double& standardScale) const;

  /** \details
    Returns true if an only if lineweights are scaled for this PlotSettings object (DXF 70, bit 0x40).
  */
  bool scaleLineweights() const;

  /** \details
    Controls the scaling of lineweights for this PlotSettings object (DXF 70, bit 0x40).

    \param scaleLineweights [in]  True if and only if lineweights are to be scaled.
  */
  void setScaleLineweights(
    bool scaleLineweights);

  /** \details
    Returns true if an only if lineweights are printed for this PlotSettings object (DXF 70, bit 0x80).
  */
  bool printLineweights() const;

  /** \details
    Controls the printing of lineweights for this PlotSettings object (DXF 70, bit 0x80).

    \param printLineweights [in]  True if and only if lineweights are to be printed.
  */
  void setPrintLineweights(bool printLineweights);

  /** \details
      TBC.  
  bool textFill() const;

  */

  /** \details
      TBC.
  void setTextFill(bool textFill);
  */

  /** \details
      TBC.
  int getTextQuality() const;
  */

  /** \details
      TBC.
  void setTextQuality(int quality);
  */

  /** \details
    Returns true if and only if viewports are to be plotted before other objects in PaperSpace
    for this PlotSettings object (DXF 70, bit 0x200).
  */
  bool drawViewportsFirst() const;

  /** \details
    Controls the plotting of viewports before other objects in PaperSpace
    for this PlotSettings object (DXF 70, bit 0x200).
    
    \param drawViewportsFirst [in]  True if and only if viewports are to be plotted first.
  */
  void setDrawViewportsFirst(
    bool drawViewportsFirst);

  /** \details
    Returns true if and only if this PlotSettings object is of ModelSpace type (DXF 70, bit 0x400).
    
    \remarks
    A PlotSettings object is of either ModelSpace type or PaperSpace type.
  */
  bool modelType() const;

  /** \details
    Controls the ModelSpace type of this PlotSettings object (DXF 70, bit 0x400).
    
    \param modelType [in]  True if and only if it is of ModelSpace type.
    
    \remarks
    A PlotSettings object is of either ModelSpace type or PaperSpace type.
  */
  void setModelType(bool modelType);

  /** \details
    Returns the top margin of the paper for this PlotSettings object (DXF 43).
  */
  double getTopMargin() const;

  /** \details
    Returns the right margin of the paper for this PlotSettings object (DXF 42).
  */
  double getRightMargin() const;

  /** \details
    Returns the bottom margin of the paper for this PlotSettings object (DXF 41).
  */
  double getBottomMargin() const;

  /** \details
    Returns the left margin of the paper for this PlotSettings object (DXF 40).
  */
  double getLeftMargin() const;

  /** \details
    Returns the paper image origin of the paper for this PlotSettings object (DXF 148, 149).
  */
  OdGePoint2d getPaperImageOrigin() const;

  /** \details
    Sets the paper image origin (DXF 148, 149).
    \param paperImageOrigin [in]  Paper image origin. 
  */
  void setPaperImageOrigin(OdGePoint2d paperImageOrigin);

  // New to 2004 api

  /** \details
    Returns the shade plot type for this PlotSettings object (DXF 76).
    
    \remarks
    shadePlotType() returns one of the following:
    
    <table>
    Name           Value  Description
    kAsDisplayed   0      As displayed
    kWireframe     1      Wireframe
    kHidden        2      Hidden lines removed
    kRendered      3      Rendered
    </table>
    
  */
  OdDbPlotSettings::ShadePlotType shadePlot() const;

  /** \details
    Sets the shade plot type for this PlotSettings object (DXF 76).
    
    \param shadePlot [in]  Shade Plot type.

    \remarks
    shadePlot must be one of the following:
    
    <table>
    Name           Value  Description
    kAsDisplayed   0      As displayed
    kWireframe     1      Wireframe
    kHidden        2      Hidden lines removed
    kRendered      3      Rendered
    </table>
    
  */
  OdResult setShadePlot(
    OdDbPlotSettings::ShadePlotType shadePlot);

  /** \details
    Returns the shade plot resolution level for this PlotSettings object.
    
    \remarks
    shadePlotResLevel controls the resolution at which shaded and 
    vectorized viewports will plot.
    
    shadePlotResLevel() returns one of the following:
    
    <table>
    Name            Value   Description
    kDraft          0       Draft
    kPreview        1       Preview
    kNormal         2       Normal
    kPresentation   3       Presentation
    kMaximum        4       Maximum
    kCustom         5       Custom
    </table>

  */
  OdDbPlotSettings::ShadePlotResLevel shadePlotResLevel() const;

  /** \details
    Sets the shade plot resolution level for this PlotSettings object.
    
    \param shadePlotResLevel [in]  Shade plot resolution level.
    
    \remarks
    shadePlotResLevel controls the resolution at which shaded and 
    vectorized viewports will plot.

    shadePlotResLevel must be one of the following:
    
    <table>
    Name            Value   Description
    kDraft          0       Draft
    kPreview        1       Preview
    kNormal         2       Normal
    kPresentation   3       Presentation
    kMaximum        4       Maximum
    kCustom         5       Custom
    </table>

  */
  void setShadePlotResLevel(
    OdDbPlotSettings::ShadePlotResLevel shadePlotResLevel);

  /** \details
    Returns the shade plot custom DPI for this PlotSettings object.
    
    \remarks
    shadePlotCustomDPI specifies the custom resolution at which shaded and 
    vectorized viewports will plot if shadePlotResLevel == ShadePlotResLevel::kCustom.
    
    \remarks
    shadePlotCustomDPI should be used only if the shade plot resolution level 
    is set to ShadePlotResLevel::kCustom.
  */
  OdInt16 shadePlotCustomDPI() const;

  /** \details
    Sets the shade plot custom DPI for this PlotSettings object.
    
    \remarks
    shadePlotCustomDPI specifies the custom resolution at which shaded and 
    vectorized viewports will plot if shadePlotResLevel == ShadePlotResLevel::kCustom.
    
    \remarks
    shadePlotCustomDPI should be used only if the shade plot resolution level 
    is set to ShadePlotResLevel::kCustom.
  
    \param shadePlotCustomDPI [in]  Shade plot custom DPI.
  */
  void setShadePlotCustomDPI(
    OdInt16 shadePlotCustomDPI);

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual void subClose();

  virtual OdResult subErase(
    bool erasing);
  
  virtual void subHandOverTo(
    OdDbObject* pNewObject);

  virtual void copyFrom(
    const OdRxObject* pSource);

// New in AC2007

// Returns the OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object
// referenced by this object.

  /** \details
    Returns the OdDbObjectId of the OdDbVisualStyle or OdDbRenderSettings object
    referenced by this object.
  */
  OdDbObjectId    shadePlotId() const;

  /** \details
    Sets the shade plot type for this PlotSettings object (DXF 76).
    
    \param type [in]  Shade Plot type.

    \remarks
    type must be one of the following:
    
    <table>
    Name           Value  Description
    kAsDisplayed   0      As displayed
    kWireframe     1      Wireframe
    kHidden        2      Hidden lines removed
    kRendered      3      Rendered
    </table>
    
  */
  OdResult        setShadePlot(OdDbPlotSettings::ShadePlotType type,
                               OdDbObjectId shadePlotId);
  /** \details
    Controls whether the output is vector-based when using shade plot settings of a viewport.
  */
  bool plotWireframe() const;
  /** \details
    Controls whether the output is raster-based when using shade plot settings of a viewport.
  */
  bool plotAsRaster () const;

  /** \details
    Returns true if and only if transparencies plotting is enabled for this OdDbRenderSettings object.
  */
  bool plotTransparency() const;
  /** \details
    Enables or disables transparencies plotting for this OdDbRenderSettings object.

    \param bSet [in]  New transparencies plotting setting.
  */
  void setPlotTransparency(bool bSet);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbPlotSettings object pointers.
*/
typedef OdSmartPtr<OdDbPlotSettings> OdDbPlotSettingsPtr;

#include "TD_PackPop.h"

#endif

