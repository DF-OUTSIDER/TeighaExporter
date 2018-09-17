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



//////////////////////////////////////////////////////////////////////
//
// PdfDrawObject.h: definition of the GsPdfFactory, PDF2dExportDevice, OdPdfView classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PDF_2D_EXPORT_DEVICE_INCLUDED_
#define _PDF_2D_EXPORT_DEVICE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2dExportDevice.h"
#include "OdPlatform.h"
#include "Gi/GiPlotGenerator.h"

#include "PdfExportParamsHolder.h"
#include "PdfFont.h"
#include "PdfExportService.h"
#include "PdfAnnotationDictionary.h"
#include "Pdf2PrcExportParams.h"

//////////////////////////////////////////////////////////////////////
using namespace TD_2D_EXPORT;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

#define PDF_FROZEN_LAYER "||frozen"
#define PDF_FROZEN_LAYER_SIZE 8

/** \details
  This structure implements the device point.
*/
struct PDFDevicePoint {
  double x; // X coordinate of the device point
  double y; // Y coordinate of the device point
};

struct ViewExtents {
  int m_viewIdx;
  OdGeExtents3d m_extents;
};

/** \details
  This class implements the two-dimensional export device.
*/
class PDF2dExportDevice : public Od2dExportDevice, public OdGiPlotGenerator::EllipseOut
{
  // Members
  //
private:

  PdfExportParamsHolderPointer       m_pParamsHolder; // Reference to the /PdfExportParamsHolder/ class instance, used for creating PDF document.
  PDFIContentCommands               *m_OutputCCStream; // Pointer to the /content commands stream/ for the PDF document
  static PDFDummyContentStream       m_DummyCCStream; // Pointer to the /content stream/ for the PDF document
  PDFPageDictionary                 *m_currentPdfPage;
public:
  bool                               m_bRasterClipping; // Added to unconditional clipping of 3d solids which cannot be exported via vectorizer
  OdGePoint2dArray                   m_clipPoints;  // Clipping area based on device restrictions
  OdGeExtents2d                      m_paperClip; // Calculated from m_clipPoints
  double                             m_LwToDcScale; //lineweight multiplier for Z2E mode
  std::map<OdUInt32, std::pair<OdRxObjectPtr, ViewExtents> >  m_prc_context; // prc export context

/** \details
    Returns the pointer to the content commands stream for the export device.
*/
  PDFIContentCommands *cc() { return m_OutputCCStream; }

  PDFPageDictionary *currentPdfPage() { return m_currentPdfPage; }

/** \details
    Returns reference to PDF document associated with export device.

    \remarks
	PDF document reference returns from CPdfExportImpl class instance incapsulated into the export device.
*/
    PDFDocument &document() { return m_pParamsHolder->document(); }
  
  /** \details
    Returns reference to PDFType3Optimizer object associated with the export device.

    \remarks
    Type optimization options for PDF export are stored in the CPdfExportImpl class instance incapsulated into the export device.
  */
  PDFType3Optimizer &getFontType3Optimizer() { return m_pParamsHolder->getType3Optimizer(); }

  virtual void shellFaceOut( OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal );

  virtual double renderAreaMultiplier(OdUInt32 width, OdUInt32 height) const;

  OdGiRasterImagePtr cropImage(const OdGiRasterImage* pImg, const OdGeVector2d& u, const OdGeVector2d& v, OdGeExtents2d& cropArea, OdGePoint2d& originPoint);
  void setPaperBox(const OdGsDCRect& box);
public:

  /** \details
    Returns reference to PDFFontOptimizer object associated with the export device.

    \remarks
    Font optimizer object is stored in the CPdfExportImpl class instance incapsulated into the export device.
  */
  PDFFontOptimizer &getFontOptimizer() { return m_pParamsHolder->getFontOptimizer(); }

  /** \details
    Enumeration for describing acceptable polygon fill rules.
  */
  enum PolygonFillRules
  {
    kFillNonzeroRule     = 0x0000, // Default value. Fill the path, using the nonzero winding number rule to determine the region to fill
                                   // Any subpaths that are open are implicitly closed before being filled.
    kFillEvenOddRule     = 0x0001, // Even-odd fill rule - "f*" Fill the path, using the even-odd rule to determine the region to fill
    kNoFill              = 0x0002, // No fill
    kFillStrokeCloseRule = 0x0004  // Close, fill, and then stroke the path, using the nonzero winding number rule to determine the region to fill.
  };
private:
  bool              m_bGraphStateFixed; // Shows if the graph state is fixed.

  bool              m_bOnePointPoly; // Shows if the polygon consists of one point.
  PDFDevicePoint    m_LastPoint; // Last point processed by the export device.
  OdUInt32          m_num; // Points count.
  PDFDevicePoint    m_points[6]; // Static array of the points which the polygon consists of

  PDFNamePtr        DWG_PALETTE_NAME; // Name of the pallete currently used by the export device.
  PDFNamePtr        PDF_OC_NAME; // Name of the PDF document used by the export device.
  bool              m_bDwgPaletteNeeded;//shows if indexed dwg palette is necessary

  bool              m_bRGBActive; // Shows if the RGB mode is active.
  ODCOLORREF        m_curRGB; // Contains the current RGB scheme used for export device.
  OdUInt16          m_curColorIndex; // Current colour index used for export device.
  ODCOLORREF        m_curSecondaryColorRGB; // Contains the current secondary color RGB scheme used for export device.
  OdUInt16          m_curSecondaryColorIndex; // Current secondary color index used for export device.

  OdUInt16          m_hatchDPI;       // Export device resolution for hatch exported as bitmap(dot per inch).
  OdUInt16          m_bwBitmapDPI;    // Export device resolution for bw bitmap(dot per inch).
  OdUInt16          m_colorBitmapDPI; // Export device resolution for color bitmap(dot per inch).
  
  // layers support functionality
  OdString          m_curLayerName; // Name of the current layer used by the export device.
  bool              m_LayerOpen; // Shows if the current layer is open.

  bool              m_stubLayerCreated;//create stub layer if pdf has hyperlinks to avoid incorrect work of Adobe Reader (workaround)

  double            m_lineWeight; // Weight of the line used for painting by the export device.
  PDFLineCap        m_CapStyle;   // Line cap style
  PDFLineJoin       m_JoinStyle;  // Line join style

  PolygonFillRules  m_fillRule; // Polygon fill rules used for painting by the export device.
  FrozenLayerMap m_frozenLayers;

  int m_stateDrawingContour; // the contour drawing is in progress, don't finish each curve with 'S' command

  bool              m_bHatchDrawingMode; //set for draw hatch, to avoid downscaling if hatchDPI > bitmapDPI
  OdAnsiString      m_currentTransparencyState;//current transparency graphic state
  bool              m_bTransparencyGroup255;//flag to create graphic state for non-transparent objects
  OdGsDCRect        m_paperBox; //size of the paper (now using for filling pdf page background as application background);
  bool              m_bRecordingEnabled;
protected:

  bool applyLayerToAnnotation(const OdString& layer, PDFAnnotationDictionaryPtr pDict);

/** \details
    Returns if it is needed to start new polyline for rendering polygone.

    \remarks
    Returns true if new polyline is needed, false if the other case.
*/
  bool needNewPolyline() const;
  
/** \details
    Determines whether the /segment of polygone/ represented by the specified point is duplicate.

    \remarks
    Returns true if the segment is dublicated and false in the other case.

    \param LastPoint [in]  A reference to the PDFDevicePoint class instance representing the point.
*/
  bool isDublicatedSegment(const PDFDevicePoint &LastPoint) const;
  
/** \details
    Continues the polyline starting from the specified point.

    \remarks
    Polyline is continued from the specified instance of the PDFDevicePoint class.

    \param LastPoint [in]  A point from which the polyline should be continued.
*/
  void ContinuePolyline(const PDFDevicePoint &LastPoint);
  
/** \details
    Saves the current state of the graph.

    \remarks
    Parameter value determines to what point the graph should be saved.

    \param LastPoint [in]  A point for saving.
*/
  void saveGraphState(const PDFDevicePoint& LastPoint);
  
  /** \details
    Saves last point of the polyline.

    \param LastPoint [in]  A point to save.
  */
  void saveLastPoint(const PDFDevicePoint& LastPoint);

  /** \details
  Converts color to grayscale.

  \param rgb [in]  color to convert.
  */
  void convertColorToGrayscale(ODCOLORREF& rgb);
  
/** \details
    Processing the changing of the graph state.

    \remarks
    Returns true if and only if successful.
    The /dictionary key/ is the string that is the combination of the /book name and colour name/ joined by the dollar '$' symbol

    \param dictionaryKey [in]  Dictionary keyword.   
*/
    void GraphStateChanged();
  
/** \details
    Sets the /book name and colour name/ for the /dictionary keyword/.

    \remarks
    Returns true if and only if successful.
    The /dictionary key/ is the string that is the combination of the /book name and colour name/ joined by the dollar '$' symbol

    \param dictionaryKey [in]  Dictionary keyword.   
*/
    void StartNewPolyline(const PDFDevicePoint &LastPoint);
  
/** \details
    Sets the /book name and color name/ for the /dictionary keyword/.

    \remarks
    Returns true if and only if successful.
    The /dictionary key/ is the string that is the combination of the /book name and colour name/ joined by the dollar '$' symbol

    \param dictionaryKey [in]  Dictionary keyword.   
*/
    void ClosePolyline();

/** \details
    Transoforms clip of the raster image into the instance of the OdGeMatrix2d class.

    \remarks
    Transforms specified raster image starting from the origin point using two vectors, representing X-axis and Y-axis of the coordinate system. Transformation can be translation, rotation and scaling. 

    \param pImg [in]  Pointer to raster image (OdGiRasterImage) that should be transformed.
	\param origin [in]  Point from which transformation starts.
	\param u [in]  Vector, representing X-axis.
	\param v [in]  Vector, representing X-axis.
*/
  OdGeMatrix2d createImageClipTransform( const OdGiRasterImage* pImg 
    ,const OdGePoint3d& origin
    ,const OdGeVector3d& u
    ,const OdGeVector3d& v
    );
	
/** \details
    Transforms the raster image into the instance of the OdGeMatrix2d class.

    \remarks
    Transforms specified raster image starting from the origin point using two vectors, representing X-axis and Y-axis of the coordinate system. Transformation can be translation, rotation and scaling. 

    \param pImg [in]  Pointer to raster image (OdGiRasterImage) that should be transformed.
	\param origin [in]  Point from which transformation starts.
	\param u [in]  Vector, representing X-axis.
	\param v [in]  Vector, representing X-axis.
*/
  OdGeMatrix2d createImageTransform( const OdGiRasterImage* pImg 
    ,const OdGePoint3d& origin
    ,const OdGeVector3d& u
    ,const OdGeVector3d& v
    );

/** \details
    Create a PDF layer with specified name.

    \remarks
    Returns a pointer to an instance of PDFName class.

    \param layer_name [in]  Name of the layer.
*/
	  PDFNamePtr CreateOC4Layer(const OdString &layer_name, bool bLocked = false);

/** \details
    Open /layer's bracket/ with the specified name.

    \param layer_name [in]  Name of the layer.
*/
	    void OpenLayerBracket(const OdString &layer_name);
  
/** \details
    Close /layer's bracket/, previously opened with the /OpenLayerBracket()/.
*/
	      void CloseLayerBracket();

    void finishPRCVectorization();

    PDFResourceDictionaryPtr getResourceDictionary();

public:
  void dc_mx();

  void open_Frozen_Layer(const OdString &layer_name);
  void close_Frozen_Layer();

public:

  // Constructor

  PDF2dExportDevice();
  virtual ~PDF2dExportDevice() 
  {}

/** \details
    Updates the export device at the specified rectangle.

    \remarks
	Repaints associated with the export device PDF document inside the borders of the specified rectangle.

    \param pUpdatedRect [in]  Pointer to the rectangle that needs to be updated.
*/
  virtual void update(OdGsDCRect* pUpdatedRect);

/** \details
    Enable\disable output methods

    \remarks
    It uses to prevent duplication of traits data in case of zoom2extents call and other.

    \param bEnable [in]  Determines if the output methods are enabled (if true) or disabled (if false). By default the value is true.
    \param bZ2E    [in]  Determines if the method call for Z2E case
*/
  void enableRecording(bool bEnable = true, bool bZ2E = true);

  bool recordingEnabled();

/** \details
    Sets the /image resolution/ for hatches exported as bitmap.

    \remarks
    Resolution is set in /pixels per inch/ (dpi).

    \param dpi [in]  Resolution value.
*/
  void setHatchDPI(OdUInt16 dpi) {
    m_hatchDPI = dpi;
  }

  void setHatchDrawingMode(bool bMode) {
    m_bHatchDrawingMode = bMode;
  }

  /** \details
  Sets the /image resolution/ for bw bitmap.

  \remarks
  Resolution is set in /pixels per inch/ (dpi).

  \param dpi [in]  Resolution value.
  */
  void setBwBitmapDPI(OdUInt16 dpi) {
    m_bwBitmapDPI = dpi;
  }

  /** \details
  Sets the /image resolution/ for color bitmap.

  \remarks
  Resolution is set in /pixels per inch/ (dpi).

  \param dpi [in]  Resolution value.
  */
  void setColorBitmapDPI(OdUInt16 dpi) {
    m_colorBitmapDPI = dpi;
  }

  bool isFrozenLayer(const OdString &layerName) const;

  void setFrozenLayers(const FrozenLayerMap &frozenLayers) {
    m_frozenLayers = frozenLayers;
  }

/** \details
    Returns the /image resolution/ for hatches exported as bitmap.

    \remarks
    Resolution is set in /pixels per inch/ (dpi).
*/
  OdUInt16 getHatchDPI() const {
    return m_hatchDPI;
  }

  /** \details
  Returns the /image resolution/ for bw bitmap.

  \remarks
  Resolution is set in /pixels per inch/ (dpi).
  */
  OdUInt16 getBwBimapDPI() const {
    return m_bwBitmapDPI;
  }

  /** \details
  Returns the /image resolution/ for color bitmap.

  \remarks
  Resolution is set in /pixels per inch/ (dpi).
  */
  OdUInt16 getColorBimapDPI() const {
    return m_colorBitmapDPI;
  }

/** \details
    Returns PDF /export parameters/.

    \remarks
    Parameters are realized as an instance of the PDFExportParams class;
*/
  const PDFExportParams& exportParams() const
  {
    return m_pParamsHolder->getParams();
  }
  PDFExportParams& exportParams()
  {
    return m_pParamsHolder->getParams();
  }

  // pdf input flags auxiliary methods
/** \details
    Determines if PDF document allows embedded /TTF fonts/.

    \remarks
    Returns true if document associated with export device supports embedded /True Type fonts/ and false in the other case.
*/
  bool AllowEmbededFonts() const 
  {
    return !isTTFTextGeometry() && GETBIT(m_pParamsHolder->getParams().exportFlags(), PDFExportParams::kEmbededTTF);
  }

/** \details
    Determines if PDF document allows optimized embedded /TTF fonts/.

    \remarks
    Returns true if document associated with export device supports optimized embedded /True Type fonts/ and false in the other case.
*/
  bool AllowEmbededOptimizedFonts() const 
  {
    return !isTTFTextGeometry() && GETBIT(m_pParamsHolder->getParams().exportFlags(), PDFExportParams::kEmbededOptimizedTTF);
  }

/** \details
    Determines if PDF document dissolves the text with /True Type fonts/ into simple geometry primitives.

    \remarks
    Returns true if the document associated with export device dissolves the text with /True Type fonts/ into simple geometry primitives and false in the other case.
*/
  bool isTTFTextGeometry() const
  {
    return GETBIT(m_pParamsHolder->getParams().exportFlags(), PDFExportParams::kTTFTextAsGeometry);
  }

/** \details
    Determines if PDF document dissolves the text with /Shape file Index  fonts/ into simple geometry primitives.

    \remarks
    Returns true if the document associated with export device dissolves the text with /Shape file Index fonts/ into simple geometry primitives and false in the other case.
*/
  bool isSHXTextGeometry() const
  {
    return GETBIT(m_pParamsHolder->getParams().exportFlags(), PDFExportParams::kSHXTextAsGeometry);
  }
  
/** \details
    Determines if the geometry is simplified to reduce file size:

    \remarks
    Returns true if export device optimizes geometry and false in the other case.
*/
  bool isSimpleGeomOptimizationEnabled() const
  {
    return GETBIT(m_pParamsHolder->getParams().exportFlags(), PDFExportParams::kSimpleGeomOptimization);
  }

  bool frozenLayerSimulationEnabled() const
  {
    return m_frozenLayers.size() != 0;
  }

/** \details
    Determines if layers are supported by the export device.

    \remarks
    Returns true if layers are supported by the export device or false in the other case.
*/
  bool isLayersSupported() const
  {
    return GETBIT(m_pParamsHolder->getParams().exportFlags(), PDFExportParams::kEnableLayers);
  }

/** \details
    Determines if not active /("off") layers/ are supported by export device.

    \remarks
    Returns true if the /off layers/ are supported and false in the other case.
*/
  bool isOffLayersEnabled() const
  {
    return GETBIT(m_pParamsHolder->getParams().exportFlags(), PDFExportParams::kIncludeOffLayers);
  }

  /** \details
  Determines if the "global" indexed palette is required in pdf.

  \remarks
  Returns true if the "global" indexed palette is required in pdf and false in the other case.
  */
  bool isDWGPaletteNeeded() const
  {
    return m_bDwgPaletteNeeded;
  }

  // Set the target data stream and the type.  
  //static OdGsDevicePtr createObject(DeviceType type = /*Od2dExportDevice::*/k3dDevice);
/** \details
    Creates the target data stream for the export device.
	
	\remarks
	Returns a smart pointer to OdGsDevice class instance if target data stream was successfully created.
*/
  static OdGsDevicePtr createObject();

  //  to create an instance 
  // of the desired OdGsView subclass. 
/** \details
    Creates an instance of the OdGsView subclass.

    \remarks
    Called by the rendering framework. Return a smart pointer to OdGsView class instance if view was successfully created.

    \param pInfo [in]  Pointer to OdGsClientViewInfo class instance.
	\param bEnableLayerVisibilityPerView [in]  Determines if layer visibility is switched on (true)  or off (false) for the created view. Default value is false.
*/
  OdGsViewPtr createView(const OdGsClientViewInfo* pInfo = 0, 
                         bool bEnableLayerVisibilityPerView = false);


  //  ----- Od2dExportDevice methods -----

  // writing process

  void dc_pushClip(int nrcContours, const int* pNrcCounts, const OdGsDCPointArray &nrcPoints);
  void dc_popClip();
  void dc_polyline( OdUInt32 nPts, const OdGePoint2d* points );
  void dc_polygon ( OdUInt32 nPts, const OdGePoint2d* points );
  void dc_fill();
  void dc_circle  ( const OdGeEllipArc3d& ellipse );
  void dc_ellipse ( const OdGeEllipArc3d& ellipse );
  
/** \details
    Outputs a non-uniform rational B-splines (NURBS) into the export device.

    \remarks
    NURBS for outputting to the export device is passed with the method parameter.

    \param nurb [in]  OdGeNurbCurve2d class instance implementing NURBS for output.
*/
    void dc_nurb(const OdGeNurbCurve2d& nurb );
  void dc_gouraud_polytriangle(
                        const OdGePoint3d* pVertexList,
                        OdInt32 faceListSize,
                        const OdInt32* pFaceList,
                        const OdCmEntityColor* pColors );

  void dc_raster_image( 
          const OdGePoint3d& origin,
          const OdGeVector3d& u,
          const OdGeVector3d& v,
          const OdGiRasterImage* pImg, // image object
          const OdGePoint2d* uvBoundary, // may not be null
          OdUInt32 numBoundPts,
          bool transparency,
          double brightness,
          double contrast,
          double fade,
          ODCOLORREF entityColor);
    
  void dc_raster_image( OdGiRasterImagePtr pRaster, 
                        const OdGeExtents2d& imgExts);

/** \details
    Outputs a line into the export device.

    \remarks
    Width of the line is passed to the method as a parameter.

    \param lineWeight [in]  Width of the ouputted line.
*/
  void dc_lineWeight (double lineWeight);
  void dc_color(ODCOLORREF rgb);
  void dc_color(OdUInt16 index);
  void dc_layer(const OdString &layer_name, bool bClose);

  // OdAnsiString dc_shx_font(const OdGiTextStyle& pTextStyle, PDFFontPtr *pOutFont);
  OdAnsiString dc_truetype_font(const OdGiTextStyle& pTextStyle, PDFFontPtr *pOutFont);
  
/** \details
    Outputs a text into the export device

    \remarks
    Draws specified text into the export device according with passed parameters.

    \param fontName [in]  Name of the font, used for drawing text.
	\param position [in]  Position from which drawing text starts.
	\param u [in]  Vector representing X-axis.
	\param v [in]  Vector representing Y-axis.
	\param msg [in]  Drawing text message.
	\param length [in]  Length of the drawing message.
	\param pTextStyle [in]  Text style of the drawing message.
*/
    void dc_drawtext(const OdAnsiString &fontName, const OdGePoint2d& position, const OdGeVector2d& u, const OdGeVector2d& v, const char& msg, OdInt32 length, const OdGiTextStyle& pTextStyle, const OdDoubleArray& charSpaces);
  
/** \details
    Outputs a Unicode text into the export device

    \remarks
    Draws specified Unicode text into the export device according with passed parameters.

    \param fontName [in]  Name of the font, used for drawing text.
	\param pos [in]  Position from which drawing text starts.
	\param u [in]  Vector representing X-axis.
	\param v [in]  Vector representing Y-axis.
	\param pUnicode [in]  Drawing Unicode text message.
	\param pTextStyle [in]  Text style of the drawing message.
*/
  void dc_drawunicodetext(const OdAnsiString &fontName, const OdGePoint2d& pos, const OdGeVector2d& u, const OdGeVector2d& v, OdArray<OdUInt16>* pUnicode, const OdGiTextStyle& pTextStyle, bool raw);

  /** \details
    Sets the line cap and line join styles for the export device.

    \param linecap [in]  Line cap style to be set.
	\param linejoin [in]  Line join style to be set.
  */
  void setCapJoin(PDFLineCap linecap, PDFLineJoin linejoin);

  //  ----- PDF2dExportDevice methods -----

  /** \details
    Sets a spetial PDF2dExportDevice context

    \remarks
	There is a need to call own device methods for setting of context before setupActiveLayoutViews() or setupLayoutViews() not after! Only base interface methords could be called after them.

    \param pdf [in]  A pointer to an CPdfExportImpl class instance used to install the context.
  */
  void setContext(PdfExportParamsHolderPointer params_holder);

  /** \details
    Starts optimization of the graphical primitives for the export device.
  */
  virtual void StartOptimizator();

/** \details
    Stops optimization of the graphical primitives for the export device.
*/
    virtual void StopOptimizator();

/** \details
	Traits modifying handler for the export device.

    \remarks
    Process modified traits with specified data.

    \param traits [in]  Traits data for processing by the export device.
*/
  virtual void onTraitsModified(const OdGiSubEntityTraitsData& traits);
  
/** \details
    Sets the secondary colour to the export device.

    \remarks
    Secondary colour is passed as a value of ODCOLORREF type.

    \param rgb [in]  ODCOLORREF value to set to the export device.
*/
  
  void set_SecondaryColor(ODCOLORREF rgb);
  
/** \details
    Sets the secondary /colour index/ to the export device.

    \param colorIndex [in]  Colour index to set.
*/
  void set_SecondaryColorIndex(OdUInt16 colorIndex);

  /** \details
    Sets the colour to the export device.

    \remarks
    Colour is passed as a value of ODCOLORREF type.

    \param rgb [in]  ODCOLORREF value to set to the export device.
*/
  
  void set_Color(ODCOLORREF rgb);
  
/** \details
    Sets the /colour index/ to the export device.

    \param colorIndex [in]  Colour index to set.
*/
  void set_ColorIndex(OdUInt16 colorIndex);
  
/** \details
    Sets the line weight for the export device.

    \param lineWeight [in]  Line weight.
*/
  void set_LineWeight(double lineWeight);
  
/** \details
    Sets the layer for the export device.

    \param layer [in]  Layer to be set.
*/
  void set_Layer(OdDbStub* layer);

  /** \details
  Returns the layer name from id

  \param layer [in]  Layer to be set.
  */
  OdString layerName(OdDbStub* layer);

/** \details
    Processing the specified /raster image/ by the export device.

    \remarks
	Processing the /raster image/ from the specified point, using two OdGeVector instancies as X-axis and Y-axis according with other specified parameters.

    \param origin [in]  Starting point.
	\param u [in]  X-axis vector.
	\param v [in]  Y-axis vector.
	\param pImg [in]  Pointer to the raster image object.
	\param uvBoundary [in]  Boundary of the processing area (may not be null).
	\param numBoundPts [in]  Bound pointers count.
	\param transparency [in]  Image transparency (by default image is not transparent, i.e. the value is false).
	\param brightness [in]  Brightness of the image (by default the value is equal to 50.0).
	\param contrast [in]  Contrast of the image (by default the value is equal to 50.0)
	\param fade [in]  Fading level of the image (by default the value is equal to 0.0)
*/
  void rasterImageProc(
            const OdGePoint3d& origin,
            const OdGeVector3d& u,
            const OdGeVector3d& v,
            const OdGiRasterImage* pImg, 
            const OdGePoint2d* uvBoundary, 
            OdUInt32 numBoundPts,
            bool transparency = false,
            double brightness = 50.0,
            double contrast = 50.0,
            double fade = 0.0);

/** \details
    Processing the specified /meta file/ by the export device.

    \remarks
    Processing the /meta file/ from the specified point, using two OdGeVector instancies as X-axis and Y-axis according with other specified parameters.

	\param origin [in]  Starting point.
	\param u [in]  X-axis vector.
	\param v [in]  Y-axis vector.
	\param pMetafile [in]  Pointer to the meta file object.
	\param bDcAligned [in]  Reserved.
	\param bAllowClipping [in]  Reserved.
*/
			  void metafileProc(
            const OdGePoint3d& origin,
            const OdGeVector3d& u,
            const OdGeVector3d& v,
            const OdGiMetafile* pMetafile,
            bool bDcAligned = true,       // reserved
            bool bAllowClipping = false); // reserved

/** \details
    Processing the specified text by the export device.

    \remarks
	Processing the text from the specified point, using two OdGeVector instancies as X-axis and Y-axis according with other specified parameters.

	\param position [in]  Starting point.
	\param u [in]  X-axis vector.
	\param v [in]  Y-axis vector.
	\param msg [in]  Text to output.
	\param length [in]  Text message length.
	\param raw [in]  
	\param pTextStyle [in]  Text style.
	\param pExtrusion [in]  Text extrusion.
*/
  virtual void textProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool raw, 
            const OdGiTextStyle* pTextStyle,
            const OdGeVector3d* pExtrusion = 0);

/** \details
    Processing a non-uniform rational B-splines (NURBS) to the export device.

    \remarks
	Processing the specified NURBS to the export device. NURBS represented by the OdGeNurbCurve class instance.

    \param nurbsCurve [in]  NURBS instance to process by the export device.
*/
  virtual void nurbsProc(const OdGeNurbCurve3d& nurbsCurve);

/** \details
    Processing a Shape file Index text by the export device.

    \remarks
	Processing the /Shape file Index text/ from the specified point, using two OdGeVector instancies as X-axis and Y-axis according with other specified parameters.

    \param position [in]  Starting point.
	\param u [in]  X-axis for the export device.
	\param v [in]  Y-axis for the export device.
	\param msg [in]  Text of the message to output.
	\param length [in]  Length of the message.
	\param raw [in] 
	\param pStyle [in]  Text style.
	\param pExtrusion [in]  Text extrusion.
*/
  virtual void shxText( const OdGePoint3d& /*position*/,
                        const OdGeVector3d& /*u*/,
                        const OdGeVector3d& /*v*/,
                        const OdChar* /*msg*/,
                        OdInt32 /*length*/,
                        bool /*raw*/,
                        const OdGiTextStyle* /*pStyle*/,
                        const OdGeVector3d* /*pExtrusion*/ );
						
/** \details
    Processing a Shape file Index text by the export device.

    \remarks
	Processing the /Shape file Index text/ from the specified point, using two OdGeVector instancies as X-axis and Y-axis according with other specified parameters.

    \param position [in]  Starting point.
	\param u [in]  X-axis for the export device.
	\param v [in]  Y-axis for the export device.
	\param msg [in]  Text of the message to output.
	\param length [in]  Length of the message.
	\param raw [in] 
	\param pStyle [in]  Text style.
	\param pUnicode [in]  Unicode text of the message.
	\param isInBigFont [in]  
	\param Advances [in]  
*/
  virtual void setSHXText(const OdGePoint3d& position,
                          const OdGeVector3d& u,      // normal
                          const OdGeVector3d& v,      // direction
                          const OdChar* msg, OdInt32 length, bool /*raw*/,
                          const OdGiTextStyle* pTextStyle,
                          const OdString &pUnicode,
                          const OdBoolArray &isInBigFont,
                          const OdGePoint2dArray &Advances);

/** \details
    Outputs the polygon to the export device.

    \param nbPoints [in]  Polygon points count.
	\param pVertexList [in]  Vertex list of the polygon.
	\param pNormal [in]  Normal list of the polygon.
*/
  virtual void polygonOut( OdInt32 /*nbPoints*/, 
                   const OdGePoint3d* /*pVertexList*/, 
                   const OdGeVector3d* /*pNormal*/ = NULL);

/** \details
    Sets the /fill rule/ for the polygons that can be output to the export device.

    \param fillRule [in]  Polygon fill rule to set.
*/
  PolygonFillRules setFillRule(PolygonFillRules fillRule) 
  {
    PolygonFillRules tmp = m_fillRule;
    m_fillRule = fillRule;
    return tmp;
  }

#ifdef BR_CR_10507
  virtual void shellProc(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);
#endif

  virtual bool ellipArcProc(const OdGeEllipArc3d& ellipArc, double width);
  virtual bool plotGeneratorEllipseOut(const OdGeEllipArc3d& arc, double width);

  OdResult dc_prc(OdStreamBuf *pOut, 
    const OdGiDrawable* pDrawable, 
    const PDF2PRCExportParams &params);

  OdResult dc_prc(OdStreamBuf *pOut, 
    const OdGeExtents2d &extents2d, 
    const OdGeVector3d &center, 
    const OdGeVector3d &x, 
    const OdGeVector3d &y, 
    const OdGeVector3d &z, 
    double distanceToTarget, 
    double scale, 
    OdGeMatrix3d *pMatr = NULL);

  void dc_hyperlink(const OdGeExtents2d &extents2d, const OdString &link, const OdString &layer);
  void dc_textAnnot(const OdGeExtents2d &extents2d, const OdString &title, const OdString &content, const OdString &layer);
  void dc_transparency(double alphaPercent, OdUInt8 alpha, bool mergeLines);
  void dc_mergeLines();

  void startDrawingContour(void);
  void endDrawingContour(void);
};
}
#endif // #ifndef _PDF_2D_EXPORT_DEVICE_INCLUDED_
