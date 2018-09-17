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
// DwfDrawObject.h: definition of the GsDwfFactory, OdDwfDevice, OdDwfView classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DWFDRAWOBJECT_INCLUDED_
#define _DWFDRAWOBJECT_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "2dExportDevice.h"
#include "OdPlatform.h"
#include "RxObjectImpl.h"

#include "Ge/avgpc.h"

// workaround for bug in certain versions of HP's aCC compiler
#ifdef OD_REDEFINE_STD
namespace std {} using namespace std;
#endif
#include <memory>

#define STL_USING_STACK
#include "OdaSTL.h"

#include "whiptk/whip_toolkit.h"
#include "whiptk/class_factory.h"
using namespace TD_2D_EXPORT;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DWF_EXPORT {

class CDwfExportImpl;

/** \details
  This structure implements the linestyle. 
*/
struct LineStyle
{
  WT_Line_Style::WT_Joinstyle_ID  join;
  WT_Line_Style::WT_Capstyle_ID   startCap;
  WT_Line_Style::WT_Capstyle_ID   endCap;
};

/** \details
  This structure implements the URL. 
*/
struct DwfUrl
{
  int       iUrlNum;
  OdString  sName;
  OdString  sDescription;
  OdString  sSubLocation;
};

//////////////////////////////////////////////////////////////////////

/** \details
  This class implements the device for DWF export.
*/
class OdDwfDevice : public Od2dExportDevice
{
  // Members

  CDwfExportImpl*   m_dwf;

  // Stack of viewport clips
  std::stack<WT_Viewport*> m_viewportClips;
  bool m_bDrawShellAsRaster;
  struct NrcClipEntry
  {
    OdIntArray m_nrcContours;
    OdGsDCPointArray m_nrcPoints;
    bool m_bLastNrcState;
  };
  std::stack<NrcClipEntry> m_nrcClips;
  WT_Class_Factory* m_pWhipFactory;

  // Stack of viewport clips as polygons-with-islands structure
  // the top of the stack is intersection of all previous polygons with islands
  // this member necessary to handle clipped images, since WT_Viewport doesn't
  // support NRC clipping of raster images
  std::stack<gpc_polygon> m_softClips;
  bool                    m_bTurnVisibilityOn;

  ODCOLORREF              m_curRGB;
  LineStyle               m_lineStyle;
  double                  m_dThickness;
  bool                    m_bWasNrcViewportClip;

public:

  /** \details
    Returns the class factory.
  */
  WT_Class_Factory* classFactory() { return m_pWhipFactory; }
  
  /** \details
    Sets a class factory.
    
    \param f [in]  Pointer to a class factory.
  */
  void setClassFactory(WT_Class_Factory* f) { m_pWhipFactory = f; }
  
  /** \details
    Sets a boolean flag that indicates that a viewport clipping is non-rectangular.
    
    \param val [in]  Boolean value to set.
  */
  void setWasNrcViewportClip(bool val) { m_bWasNrcViewportClip = val; }

  /** \details
    Specifies a processing type.
  */
  enum KindOfRun 
  {
    Nonprocess_run = 0 ,      // It can be used for presetting something (for instance, ColorMap)
    Main_run
  };

  /** \details
    Current processing type.
  */
  KindOfRun         m_bKindOfRun;

  // Constructor
  /** \details
    Default constructor.
  */
  OdDwfDevice();
  
  virtual ~OdDwfDevice();

  // Set the target data stream and the type.  
  //static OdGsDevicePtr createObject(DeviceType type = /*Od2dExportDevice::*/k3dDevice);
  
  /** \details
    Pseudo-constructor for device. Returns a smart pointer to the device instance.
  */
  static OdGsDevicePtr createObject();

  /** \details
    Updates the current view.
    
    \param pUpdatedRect [in]  Updated 2D device coordinate rectangle.
    \param bKindOfRun [in]  Processing type.
  */
  void update(OdGsDCRect* pUpdatedRect, KindOfRun bKindOfRun);

  TD_USING(Od2dExportDevice::update);

  // Called by the rendering framework to create an instance 
  // of the desired OdGsView subclass.
  
  /** \details
    Creates an OdGsView object.
    
    \param pInfo [in]  Structure that contains the viewport data.
    \param bEnableLayerVisibilityPerView [in]  Enables or disables a layer's visibility.
    
    \remarks
    Called by the rendering framework to create an instance of the desired OdGsView subclass.
  */   
  OdGsViewPtr createView(const OdGsClientViewInfo* pInfo = 0, 
                         bool bEnableLayerVisibilityPerView = false);


  //  ----- Od2dExportDevice methods -----

  // writing process
  
  /** \details
    Pushes a clip to the export device.
    
    \param nrcContours [in]  Number of non-rectangular contours.
    \param pNrcCounts [in]  Array of integers specifying the number of points in each non-rectangular contour.
    \param nrcPoints [in]  Array of points defining the non-rectangular clipping boundary.  
  */
  virtual void dc_pushClip(int nrcContours, const int* pNrcCounts, const OdGsDCPointArray &nrcPoints) ODRX_OVERRIDE;
  
  /** \details
    Pops a clip from the export device.
  */
  virtual void dc_popClip() ODRX_OVERRIDE;
    
  /** \details
    Outputs a polyline to the export device.
    
    \param nPts [in]  Number of points.
    \param points [in]  Array of points defining the polyline. 
  */
  virtual void dc_polyline( OdUInt32 nPts, const OdGePoint2d* points ) ODRX_OVERRIDE;
  
  /** \details
    Outputs a polygon to the export device.
    
    \param nPts [in]  Number of points.
    \param points [in]  Array of points defining the polygon. 
  */
  virtual void dc_polygon(OdUInt32 nPts, const OdGePoint2d* points) ODRX_OVERRIDE;
  
  /** \details
    Outputs a circle to the export device.
    
    \param ellipse [in]  A circle to be outputted.
  */
  virtual void dc_circle(const OdGeEllipArc3d& ellipse) ODRX_OVERRIDE;
  
  /** \details
    Outputs an ellipse to the export device.
    
    \param ellipse [in]  An ellipse to be outputted.
  */
  virtual void dc_ellipse ( const OdGeEllipArc3d& ellipse ) ODRX_OVERRIDE;
  
  /** \details
    Outputs a polytriangle to the export device.
    
    \param pVertexList [in]   List of vertices.
    \param faceListSize [in]  Number of faces.
    \param pFaceList [in]  Face list.
    \param pColors [in]  Array of face colors.
  */
  virtual void dc_gouraud_polytriangle(
                        const OdGePoint3d* pVertexList,
                        OdInt32 faceListSize,
                        const OdInt32* pFaceList,
                        const OdCmEntityColor* pColors ) ODRX_OVERRIDE;

  /** \details
    Outputs a raster image to the export device.
    
    \param pRaster [in]  Pointer to a raster image.
    \param pMask [in]  Pointer to a mask.
    \param imgExts [in]  Image extents.
    \param bBitonalTransp [in]  Indicates that the raster image is transparent.
  */                        
  void dc_raster_image( OdGiRasterImagePtr pRaster, 
                        OdGiRasterImagePtr pMask,
                        const OdGeExtents2d& imgExts,
                        bool bBitonalTransp);

  // imgExts are in screen coordinates.
  // clipBoundary (dc_pushClip) must be applied to this primitive
  
  /** \details
    Outputs a raster image to the export device.
    
    \param pRaster [in]  Pointer to a raster image.
    \param imgExts [in]  Image extents.

    \remarks
    imgExts are in screen coordinates.
  */ 
  virtual void dc_raster_image( OdGiRasterImagePtr pRaster, 
    const OdGeExtents2d& imgExts) ODRX_OVERRIDE;
/*
  virtual void dc_text( const OdGePoint3d& position,
                        const OdGeVector3d& normal,
                        const OdGeVector3d& direction,
                        const OdChar* msg, OdInt32 length, bool raw,
                        const OdGiTextStyle* pTextStyle,
                        bool  bBox,
                        const WT_Font* pWtFont );
*/

  /** \details
    Outputs a color to the export device.
    
    \param rgb [in]  RGB color.
  */
  virtual void dc_color(ODCOLORREF rgb) ODRX_OVERRIDE;
  
  /** \details
    Outputs a layer to the export device.
    
    \param layer_num [in]  Layer number.
    \param layer_name [in]  Layer name.
  */
  virtual void dc_layer(int layer_num, const OdString& layer_name) ODRX_OVERRIDE;

  /** \details
    Pushes a soft clip to the export device.
    
    \param nrcContours [in]  Number of non-rectangular contours.
    \param pNrcCounts [in]  Array of integers specifying the number of points in each non-rectangular contour.
    \param nrcPoints [in]  Array of points defining the non-rectangular clipping boundary.  
  */
  void pushSoftClip(int nrcContours, const int* pNrcCounts, const OdGsDCPointArray &nrcPoints);
  
  /** \details
    Pops a soft clip from the device.
  */
  void popSoftClip();
  
  /** \details
    Returns a stack of viewport clips as polygons-with-islands structures.
  */ 
  const std::stack<gpc_polygon>& getSoftClips() const { return m_softClips; }

  /** \details
    Returns a supported color format: bits, mapped, RGB or BGRA.
    
    \param nColorBits [in]  Number of color bits.
    \param bTransparency [in]  Transparency flag.
  */ 
  virtual int getSupportedColorBits(int nColorBits, bool bTransparency) const;

  //  ----- OdDwfDevice methods -----
  //

  // Attention: there is a need to call own device methods for setting of context 
  //            before setupActiveLayoutViews() or setupLayoutViews() not after! 
  //            Only base interface methods can be called after them.

  /** \details
    Sets a special OdDwfDevice context.
    
    \param dwf [in]  DWF export data.
  */
  void setContext(CDwfExportImpl* dwf);

  /** \details
    Returns a special OdDwfDevice context.
  */
  CDwfExportImpl* getExportImpl()
  {
    return m_dwf;
  }

  /** \details
    Returns a pointer to a .dwf file.
  */
  WT_File* getWtFile();

  // To set\check a type of processing (with\without geometry)
  //
 
  /** \details
    Sets the type of processing.
    
    \param bKindOfRun [in]  Processing type.
  */
  void setRun(KindOfRun bKindOfRun)                 
  {
    m_bKindOfRun = bKindOfRun;
  }

  /** \details
    Returns true if the type of processing is Nonprocess_run. Returns false in the other case.
  */
  bool IsNonprocessRun() const
  {
    return (m_bKindOfRun == Nonprocess_run) ? true : false;
  }

  /** \details
    Returns true if the type of processing is Main_run. Returns false in the other case.
  */
  bool IsMainRun() const
  {
    return (m_bKindOfRun == Main_run) ? true : false;
  }


  // ----- Writer -----
  
  /** \details
    Outputs a font to the export device.
  */
  void dc_font(const WT_Font& wtFont);
  
  /** \details
    Outputs text to the export device.
  */
  void dc_text(const WT_Text& wtText);
  
  /** \details
    Outputs a URL item to the export device.
  */
  void dc_url_item(WT_URL_Item&  wtUrlItem);
  
  /** \details
    Clears the current URL list.
  */
  void dc_url_clear();
  
  /** \details
    Pushes the current URL list.
  */  
  void push_Url();
  
  /** \details
    Creates a list of URLs that are in force right now.
  */ 
  void set_Urls();

  // to set off the rendition
  
  /** \details
    Removes all URLs from the list of URLs that are in force right now.
  */ 
  void dc_urls_remove_all();
  
  /** \details
    Removes all layers.
  */ 
  void dc_layers_remove_all();

  /** \details
    Outputs a gouraud polytriangle to the export device.
  */
  void dc_gouraud_polytriangle(const WT_Gouraud_Polytriangle& wtColoredPoly);

  //void dc_lineWeight(OdDb::LineWeight lineWeight);
  
  /** \details
    Sets the lineweight for the export device.
    
    \param lineWeight [in]  Lineweight.
  */
  void dc_lineWeight(WT_Integer32 lineWeight);
  
  /** \details
    Sets the linestyle for the export device.
    
    \param lineStyle [in]  Linestyle.
  */
  void dc_lineStyle(const LineStyle& lineStyle);

  /** \details
    Sets the visibility.
    
    \param bVisibility [in]  Boolean value to set.
  */
  void dc_visibility(bool bVisibility)
  {
    std::auto_ptr<WT_Visibility> visibility(classFactory()->Create_Visibility(bVisibility));
    visibility->serialize(*getWtFile());
  }
  
  /** \details
    Switches the fill mode on or off.
    
    \param bFill [in]  Boolean value to set.
  */
  void dc_fill(bool bFill)                          
  {
    std::auto_ptr<WT_Fill> wtFill(classFactory()->Create_Fill(bFill));
    getWtFile()->desired_rendition().fill() = *wtFill;  
  }

  /** \details
    Notification function called whenever the size of the GUI window for this device object has changed. 
    
    \param outputRect [in]  New size of the output rectangle in device coordinates (pixels).
    
    \remarks
    This function should not be called if the device object has a width and/or height of 0.
  */
  void onSize(const OdGsDCRect& outputRect);

  /** \details
    Processes a shape file's index text for the export device. 
    
    \param position [in]  Starting point.
    \param u [in]  X-axis for the export device.
    \param v [in]  Y-axis for the export device.
    \param msg [in]  Text of the message to output.  
    \param length [in]  Length of the message.
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
    \param pStyle [in]  Text style.
    \param pExtrusion [in]  Text extrusion.
        
    \remarks
    Processes the shape file's index text from the specified point using two OdGeVector instances as X-axis and Y-axis according to other specified parameters. 
  */
  void shxText( 
                const OdGePoint3d& position,
                const OdGeVector3d& u, 
                const OdGeVector3d& v,
                const OdChar* msg, 
                OdInt32 length, 
                bool raw, 
                const OdGiTextStyle* pStyle,
                const OdGeVector3d* pExtrusion );

  /** \details
    Changes the current visibility value. 
    
    \param flag [in]  Boolean value to set.
  */
  void turnVisibilityOn(bool flag)
  {
    if(m_bTurnVisibilityOn != flag)
    {
      m_bTurnVisibilityOn = flag;
      dc_visibility(flag);
    }
  }

  // --------  OdGiConveyorGeometry  --------
  
  /** \details
    Processes raster image data.
    
    \param origin [in]  Lower-left corner.
    \param u [in]  Image width vector.
    \param v [in]  Image height vector.
    \param pImg [in]  Pointer to the RasterImage object.
    \param uvBoundary [in]  Array of image boundary points (cannot be null).
    \param numBoundPts [in]  Number of boundary points.
    \param transparency [in]  True if and only if image transparency is on.
    \param brightness [in]  Image brightness [0.0 .. 100.0].
    \param contrast [in]  Image contrast [0.0 .. 100.0].
    \param fade [in]  Image fade value [0.0 .. 100.0].
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

  // traits
  
  /** \details
    Sets an RGB color.
    
    \param rgb [in]  RGB color to set.
  */
  void set_Color(ODCOLORREF rgb);
  
  /** \details
    Sets a color index for the export device.
    
    \param colorIndex [in]  Color index to set.
  */
  void set_ColorIndex(OdUInt16 colorIndex);
  
  /** \details
    Sets a lineweight for the export device.
    
    \param lineWeight [in]  Lineweight to set.
  */
  void set_LineWeight(double lineWeight);
  
  /** \details
    Sets a linestyle for the export device.
    
    \param lineStyle [in]  Linestyle to set.
  */
  void set_LineStyle(const LineStyle& lineStyle);
  
  /** \details
    Sets a layer for the export device.
    
    \param layer [in]  Layer to set.
  */
  void set_Layer(OdDbStub* layer);

  /** \details
    Sets the current thickness.
    
    \param thickness [in]  Thickness.
  */
  void setThickness(double thickness);
  
  /** \details
    Changes the current visibility value. 
    
    \param turnVisibilityOn [in]  Boolean value to set.
  */
  void setTurnVisibilityOn(bool turnVisibilityOn);

  /** \details
    Notification function called by the vectorization framework
    whenever the rendering attributes have changed.

    \param traits [in]  Traits data for processing by the export device.
  */  
  virtual void onTraitsModified(const OdGiSubEntityTraitsData& traits) ODRX_OVERRIDE;
  
  /** \details
    Returns the number of render devices.
  */
  virtual int numRenderDevices() ODRX_OVERRIDE;

  /** \details
    Creates a shell raster image. Used to export gradient hatches.
  */
  OdGiRasterImagePtr createShellRaster(bool bMask, OdGiDrawable* pShell, double dVpWidth, double dVpHeight, const OdGePoint3d& ptVpCenter, const OdGsDCRect& gsRect);

  /** \details
    Processes shell data.
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices. 
    \param faceListSize [in]  Number of entries in faceList.  
    \param faceList [in]  Array of numbers that define the faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data. 
    \param pVertexData [in]  Pointer to additional vertex data. 
  */
  virtual void shellProc(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData)ODRX_OVERRIDE;
  friend class OdDwfView;
};


//////////////////////////////////////////////////////////////////////

/** \details
  This class implements the view for DWF export.
*/
class OdDwfView : public Od2dExportView
{
public:
  /** \details
    Returns the ExGsSimpleDevice instance that owns this view.
  */
  OdDwfDevice* device()
  { 
    return (OdDwfDevice*)Od2dExportView::device();
  }

  /** \details
    Default constructor.
  */
  OdDwfView() 
  {}

  /** \details
    Pseudo-constructor for a view. Returns a smart pointer to the view instance.
  */
  static OdGsViewPtr createObject()
  {
    return OdRxObjectImpl<OdDwfView, OdGsView>::createObject();
  }

  /** \details
    Called by the Teigha rendering framework to render each entity in this view.  
    This override allows a client application to examine each entity before it is rendered.
    
    \remarks
    The override should call the parent version of this function, OdGsBaseView::draw().
  */
  void draw(const OdGiDrawable*);

  // --------  Od2dExportView  --------

  virtual void update() ODRX_OVERRIDE;

  /** \details
    Introduces a shell into this view.
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices. 
    \param faceListSize [in]  Number of entries in faceList.  
    \param faceList [in]  Array of numbers that define the faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data. 
    \param pVertexData [in]  Pointer to additional vertex data. 
  */
  void shell(OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData) ODRX_OVERRIDE;

  /** \details
    Sets text.
    
    \param position [in]  Text position.
    \param u [in]  Normal. 
    \param v [in]  Direction.  
    \param msg [in]  Text of the message to output.
    \param length [in]  Length of the message.
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters. 
    \param pTextStyle [in]  Text style.
    \param pFont [in]  Font. 
    \param pUnicode [in]  Unicode text message.
  */    
  void setText(const OdGePoint3d& position,
                const OdGeVector3d& u,
                const OdGeVector3d& v,
                const OdChar* msg, OdInt32 length, bool raw,
                const OdGiTextStyle* pTextStyle,
                bool  bBox,
                const Od2dFont* pFont = NULL,
                const OdArray<OdUInt16> *pUnicode = NULL )ODRX_OVERRIDE;


  // --------  OdDwfView  --------
  /** \details
    Initializes an export device.
  */ 
  void initialize();
  
  /** \details
    Sets the active viewport for clipping of the drawing in order to get the same result as on the screen.
  */ 
  int putViewport();

  /** \details
    Converts a world-to-device matrix of the view to a Wt matrix.
    
    \param xform [out]  Transposed matrix.
  */ 
  void convDeviceMatrixToWt(double* xform) const;
  
  /** \details
    Views space as normalized device space.
  */ 
  OdGeMatrix3d projectionMatrixMy() const;

  /** \details
    Converts a viewport of the view to a Wt point.
    
    \param wtMin [out]  Wt minimum point.
    \param wtMax [out]  Wt maximum point.
  */
  void convViewportToWt(WT_Logical_Point& wtMin, WT_Logical_Point& wtMax) const;
  
  /** \details
    Checks for URL data existence.
    
    \param pDrawable [in]  Graphical object.
  */  
  void process_Url(const OdGiDrawable* pDrawable);  
  
  /** \details
    Removes the last URL from the URL list.
  */ 
  void postprocess_Url();
  
  /** \details
    Adds a URL into the map.
    
    \param dwfUrl [in]  URL data.
  */  
  void add_UrlToMap(DwfUrl& dwfUrl);
  

  virtual void onTraitsModified() ODRX_OVERRIDE;
  friend class OdDwfDevice;

};  // end OdDwfView 
}
#endif // #ifndef _DWFDRAWOBJECT_INCLUDED_
