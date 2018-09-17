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

#ifndef _DWFExtentManager_Included_
#define _DWFExtentManager_Included_

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT{

class DwfImporter;

/** \details
  This class implements the extent, scale, paper management.
*/
class DwfExtentManager
{
  DwfImporter* _importer;
  // current layout paper desc
  friend class DwfImporter; friend struct DwfCallbackManager;
  struct PaperInfo
  {
    OdDbPlotSettings::PlotPaperUnits _units;
    double _width;
    double _height;
    OdGePoint3d _center; // is not OdGePoint3d(0,0,0) if known (also _units != kPixels for it)
    bool _isMissing; // more convinient to have it obvious than use _center == OdGePoint3d(0,0,0) or _units != kPixels
    OdDbObjectId _idLayoutWithCheckOrigin;
  }
  _paperInfo;
  // transformation: DWF -> DWG
  struct ScaleInfo
  {
    double _xOffset;
    double _yOffset;
    double _scale;
  }
  _scaleInfo;
  // center plot on paper (like viewers do)
  void centerPlot(double paperWidth, double paperHeight, int extHeight, int extWidth);
  // current clipping rect (used while calculating extents)
  WT_Logical_Box _clipRect;
public:
  OdGeMatrix3d _unitsPrimary; // the transformation from w2d coordinates of the first section (layout) resource to the original model
  OdGeMatrix3d _units; // transformation to the model coordinates, for each resource, with global per resource transforms taken into account
  bool _useUnits; // used in initial model transform extraction
  OdGeMatrix3d m_matrix; // use as m_matrix * _units // CORE-10319
  DwfExtentManager(DwfImporter* importer);
  // logical extent of the current W2D
  WT_Logical_Box _extent;
  // first pass W2D reading -> calculate extents
  bool _collectBounds;
  // update _extent (used in first pass DWF reading,
  // after the first pass, extent includes all drawables & viewports)
  void updateBounds(WT_Drawable& d, WT_File& file);
  // store dwf paper size
  void setPaperInfo(double wPaper, double hPaper, const OdGePoint3d& ptCenter, OdDbPlotSettings::PlotPaperUnits units, bool bKeepIsMissing = false);
  static void adjustParerUnits(OdDbPlotSettings::PlotPaperUnits& units, double& dPaperWidth, double& dPaperHeight, OdGePoint3d* pCenter = NULL);
  // set current layout paper
  void setPlotSettings(double wPaper, double hPaper, 
    OdDbPlotSettings::PlotPaperUnits units = OdDbPlotSettings::kMillimeters);
  // calculate scale & translation based on paper size & DWF extent
  void calculateScale();
  // transform size from logical DWF coords to DWG
  double transformSize(double);
  // transform DWF point to DWG
  OdGePoint3d transformPoint(const WT_Logical_Point& wtPoint);
  //
  void clear();
  //
  void setClipRect(const WT_Logical_Box& clipRect);
  //
  bool isValid() const;
};
}
#endif
