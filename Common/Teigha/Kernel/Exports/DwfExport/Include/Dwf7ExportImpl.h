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


///////////////////////////////////////////////////////////////////////////////
//
//  DwfExportImpl.h : definition of the CDwfExportImpl class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DWF_7_EXPORTIMPL_DEFINED
#define DWF_7_EXPORTIMPL_DEFINED


#include "OdaCommon.h"

#include "Gs/Gs.h"

#include "DwfExport.h"
#include "DwfExportParams.h"

#ifdef OD_HAVE_IO_FILE
#include <io.h>    // _access
#endif

// MKU 06/16/05 - for converting to DWF Toolkit v7.0
//

#include "dwfcore/Core.h"
#include "dwf/Toolkit.h"
#include "whiptk/whip_toolkit.h"
#include "dwfcore/MIME.h"

#define STL_USING_MAP
#define STL_USING_STRING
#include "OdaSTL.h"

#include "2dExportDevice.h"
#include "dwf/package/Paper.h"

typedef std::map<ODCOLORREF, int> colorIndexMap;

class OdDbHostAppProgressMeter;

/*!DOM*/
namespace DWFToolkit
{  
  class DWFEPlotSection;  
  class DWFGraphicResource;
}

/*!DOM*/
namespace DWFCore
{
  class DWFString;
}

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DWF_EXPORT {
//class GsDwfDevice;
class OdDwfDevice;

///////////////////////////////////////////////////////////////////////////////
// Auxiliary methods
///////////////////////////////////////////////////////////////////////////////

bool SpecifyImageMimeType(DWFCore::DWFString& zMimeType, const OdChar* pImage);

///////////////////////////////////////////////////////////
// Classes & structers
///////////////////////////////////////////////////////////

/** \details
   This structure stores the layout plot parameters for DWF export.
*/
struct DwfLayoutPlotInfo
{
  enum OdDwfExportPlotPaperUnits
  {
    kInches         = 0,   // Inches
    kMillimeters    = 1,   // Millimeters
    kPixels         = 2    // Pixels
  };

  OdDb::MeasurementValue    m_Measurement;              // Measurement value (english or metric).
  OdDwfExportPlotPaperUnits m_PlotPaperUnit;            // Plot paper units (inches, millimeters or pixels).
  double                    m_dPaperWidth;              // Paper width.
  double                    m_dPaperHeight;             // Paper height.
  double                    m_dPrintableXmin;           // X-value of the minimum point to be printed.
  double                    m_dPrintableYmin;           // Y-value of the minimum point to be printed.
  double                    m_dPrintableXmax;           // X-value of the maximum point to be printed.
  double                    m_dPrintableYmax;           // Y-value of the maximum point to be printed.
  double                    m_dScaleUnit;               // Scale factor.
  bool m_bUninitializedLayout;                          // True if layout will be uninitialized.
  DwfLayoutPlotInfo()
  {
    m_bUninitializedLayout = false;
    m_Measurement = OdDb::kMetric;
    m_PlotPaperUnit = kMillimeters;
    m_dScaleUnit = 1.0;
  }
};

/** \details
   This structure stores the coordinates of the sub-rectangle.
*/
typedef struct SUB_RECT { 
  long left; 
  long top; 
  long right; 
  long bottom; 
} SubRECT;

struct DataForDWFPlotting;


/** \details

   This is a progress meter interface for DWF export.
*/
class CDwfExportMeter
{
  // Members
  //
  OdDbHostAppProgressMeter*   m_pProgressMeter;

public:
  // Constructor/destructor

  CDwfExportMeter();
  virtual ~CDwfExportMeter()
  {
  }

  // Implementation
  
  /** \details
    Initializes the progress meter.
  */
  void pmInit();
  
  /** \details
    Stops the progress meter.
  */
  void pmRelease();
  
  /** \details
    Starts the progress meter.
  */
  void pmStart(const OdChar* displayString);
  
  /** \details
    Pauses the progress meter.
  */
  void pmStop();
  
  /** \details
    Increments the progress meter.
  */
  void pmMeterProgress();
   
  /** \details
   Sets the limit of the progress meter.
   
   \param max [in] Input limit.
  */
  void pmSetLimit(int max);

  //virtual OdDbDatabase* getDB() = 0;
  
  /** \details
    Returns a pointer to the database object to be exported.
  */
  virtual OdDbBaseDatabase* getDB() = 0;
};

/** \details
   This structure stores the paper parameters for DWF export.
*/
struct DataForDWFPlotting
{
  double            m_dWidth;       // The paper width in eUnits.
  double            m_dHeight;      // The paper height in eUnits.
  DWFToolkit::DWFPaper::teUnits m_eUnits;       // The units in which the paper dimensions apply.
  unsigned int      m_nColorARGB;   // The paper color (as a 32-bit value).
  double           m_pPaperClip[4];        // A clipping path to be applied to the paper graphics.
  bool              m_bShow;        // Indicates whether or not the paper should be shown under the graphics.
  double           m_pTransform[16];        // A clipping path to be applied to the paper graphics.
  double           m_pClip[4];        // A clipping path to be applied to the paper graphics.
  OdGePoint2d      xps_page_offset;  // An XPS page offset.
  double           xps_scale;        // An XPS scale factor.

  /** \details
    Sets the clipping region. 
	
	\param paperClip [in] Input region defined by points.
  */
  void SetClip(double paperClip[]);
  
  /** \details
    Sets the paper extents. 
	
	\param extents [in] Input extents defined by points.
  */
  void SetExtents(double extents[]);
  
  /** \details
    Sets the transformation matrix. 
	
	\param dwfToPaper [in] Input transformation matrix.
  */
  void SetTransform(double dwfToPaper[]);
  
  /** \details
    Sets the color to the paper. 
	
	\param color [in] Input color.
  */
  void SetColor( ODCOLORREF color);
  
  /** \details
    Sets the units in which the paper dimensions apply. 
	
	\param units [in] Input units.
  */
  void SetUnits( DWFToolkit::DWFPaper::teUnits units )
  {
    m_eUnits = units;
  }
  
  /** \details
    Sets the paper height. 
	
	\param dHeight [in] Input paper height.
  */
  void SetHeight( double dHeight )
  {
    m_dHeight = dHeight;
  }
  
  /** \details
    Sets the paper width. 
	
	\param dWidth [in] Input paper width.
  */
  void SetWidth( double dWidth )
  {
    m_dWidth = dWidth;
  }
  
  /** \details
    Returns the paper height. 
  */
  double GetHeight()
  {
    return m_dHeight;
  }
  
  /** \details
    Returns the paper width. 
  */
  double GetWidth()
  {
    return m_dWidth;
  }
};

/** \details
   This structure stores information about layouts for DWF export.
*/
struct DwfLayoutInfo
{
  //  corresponding to layout
  OdString          m_sLayoutName;    // Layout name.
  OdDbStub*         m_idLayout;       // ID of layout (dgn, dwf export).
  DwfLayoutPlotInfo m_PlotInfo;       // Information about plots.
  int               m_iParamIndex;    // Storage of the indexes of params corresponding to
  //  initialized layouts.
  DataForDWFPlotting m_PlottingData;  // Paper parameters for DWF export.
  DWFOutputStream* pOutStream;        // Output stream.
  DWFOutputStream* pOutStreamExt;     // Output stream extension.
  DwfLayoutInfo()
  {
    pOutStream = 0;
    pOutStreamExt = 0;
    m_idLayout = 0;
    m_iParamIndex = 0;
  }
};

///////////////////////////////////////////////////////////
//
// CDwfExportImpl
//
///////////////////////////////////////////////////////////

/** \details

   This is a main implementation class for DWF export.
*/
class CDwfExportImpl : public CDwfExportMeter
{
  DwExportParams&         m_Params;
  WT_File*                m_wtFile;
  const ODCOLORREF*       m_pPalette;       // Palette to be used. 
  colorIndexMap           m_colorIndexMap;    // Keeps RGB colors from the selected palette (Default 
                                              //  or Set by user) and corresponding color index.
  bool                    m_bWtDefaulPalette; // True - if WHIPTK Default color map is enough // MKU 1/21/2004
                                              //  when displaying images;
                                              //  false - otherwise.
  OdArray<WT_RGBA32>      m_pColors;          //  it will be used for new color map to set up a list of 
                                              //  colors (a map) to be used when displaying images.
  OdGsDevicePtr           m_pDevice;
  OdDwfDevice*            m_pDeviceDwf;
  SubRECT                 m_rectViewport;
  SubRECT                 m_rectOutput;
  typedef std::map<OdString, std::set<OdChar> > FontSubsetContainer;
  FontSubsetContainer m_xpsFontMap; // maps fontName -> (character set), for font subsetting in XPS
  DWFToolkit::DWFEPlotSection* createPlotSection(DwfLayoutInfo& layoutInfo);
protected:
  bool runExport();

  void initializeLayoutList(OdArray<DwfLayoutInfo>& arrayLayoutInfo);

  void showAllLayers();

public:
    // Constructor/destructor

    CDwfExportImpl(DwExportParams& pParams);
    virtual ~CDwfExportImpl();

    // Implementation

	/** \details
      Runs the DWF export.
    */
    bool run();

    // Accessors

	/** \details
      Returns the pointer to the WT file (for internal use).
    */
    WT_File* getWtFile()
    {
      return m_wtFile;
    }

//protected:

    /** \details
      This method is called to initialize the environment, to open the WtFile, and to write the WtFile Header data.
    */
    void begin();               // To initialize environment, to open WtFile and to write WtFile Header data
    
	/** \details
      This method is called to terminate actions.
    */
	void end();                 // Termination
	
	/** \details
      This method is called to export the data.
	  
	  \param DwfLayoutInfo [in]  Input layout information.
    */
    void draw(const DwfLayoutInfo&);                // To export data

	/** \details
      This method is called to initialize the environment.
    */
    void initContext();         // To initialize environment
	
	/** \details
      This method is called to initialize the plotting space.
	  
	  \param layoutInfo [in]  Input layout information.
    */
    void initPlotting(DwfLayoutInfo& layoutInfo);   // To initialize the plotting space
	
	/** \details
      This method is called to initialize the color engine.
    */
    void initColors();          // To initialize the color engine

	/** \details
      Opens the WT file.
	  
	  \param layoutInfo [in]  Input layout information.
    */
    void openWtFile(const DwfLayoutInfo& layoutInfo);

	/** \details
      Sets the 2D compression.
    */
    void setW2dCompression();

	/** \details
      Creates the graphical device.
    */
    void createDevice();

	/** \details
      Sets the plotting space.
	  
	  param layoutInfo [in]  Input layout information.
    */
    bool setPlottingSpace(DwfLayoutInfo& layoutInfo);
	
	/** \details
      Sets plot settings to the current layout.
    */
    void setPlotSettingsToLayout();

	/** \details
      Adds a new raster graphic resource. 
	
	  \param pageData [in] Input dwf page data.
	  \param dwf_zSectionTitle [in] Input title of the dwf z-section.
	  \param pPage [in] Input pointer to the plot section.
	  \param p2Dgfx [in] Input pointer to the 2D graphic resource.
    */
    void addThumbnail( DwfPageData &pageData, const DWFCore::DWFString& dwf_zSectionTitle, DWFToolkit::DWFEPlotSection* pPage, DWFToolkit::DWFGraphicResource* p2Dgfx);
    
	/** \details
      Adds a new preview. 
	
	  \param pageData [in] Input dwf page data.
	  \param dwf_zSectionTitle [in] Input title of the dwf z-section.
	  \param pPage [in] Input pointer to the plot section.
	  \param p2Dgfx [in] Input pointer to the 2D graphic resource.
    */
	void addPreview( DwfPageData &pageData, const DWFCore::DWFString& dwf_zSectionTitle, DWFToolkit::DWFEPlotSection* pPage, DWFToolkit::DWFGraphicResource* p2Dgfx );
    
	/** \details
      Adds new metadata. 
	
	  \param bPageDataExists [in] Input flag that determines whether the page data exists.
	  \param pageData [in] Input page data.
	  \param pPage [in] Input pointer to the plot section.
    */
	void addMetadata(bool bPageDataExists, DwfPageData &pageData, DWFToolkit::DWFEPlotSection* pPage);
	
	/** \details
      Adds fonts. 
	
	  \param pageData [in] Input page data.
	  \param pPage [in] Input pointer to the plot section.
    */
    void addFonts( const DwfPageData* pageData, DWFToolkit::DWFEPlotSection* pPage);
	
	/** \details
      Creates a basic DWF 6 file.
    */
    void writingDwf6Files();    // Creating a basic DWF 6 file
	
	/** \details
      Returns true if the DWF version is 06.00 or greater.
    */
    bool isDwfVersion6();       // Checks if the DWF version is 06.00 and greater.
	
	/** \details
      This method is called to optimize a presetting by nonprocess run (draw).
    */
    void nonprocessRun(const DwfLayoutInfo& li);       // To optimize a presetting by nonprocess run (draw)
    
	/** \details
      Puts the inked area into the file.
	  
	  \param li [in]  Input layout information.
    */
	void putInkedArea(const DwfLayoutInfo& li);
	
	/** \details
      Puts the drawing information into the file.
    */
    void putDrawingInfo();      // Put into the file Drawing Info
	
	/** \details
      Defines the drawing units.
    */
    void putDrawingUnits();     // Define Drawing Units 
	
	/** \details
      Embeds the source file.
    */
    void putEmbed();            // Embed Source File 
	
	/** \details
       Puts the view information into the file.
	   
	   \param li [in]  Input layout information.
    */
    void putViewInfo(const DwfLayoutInfo& li);
	
	/** \details
       Puts the plot information into the file.
	   
	   \param li [in]  Input layout information.
    */
    void putPlotInfo(const DwfLayoutInfo& li);
	
	/** \details
      Defines the drawing background.
    */
    void putBackground();       // Define Drawing Background
	
	/** \details
      Puts the color map into the file.
    */
    void putColorMap(); // todo

	/** \details
       Puts the view into the file.
	   
	   \param gsView [in]  Input view.
	   \param Name [in]  Input view name.
    */
    void putView(OdGsView* gsView, const OdChar* Name = NULL);
	
	/** \details
       Puts the view into the file.
	   
	   \param Name [in]  Input view name.
	   \param min [in]  Input minimum point of the view.
	   \param max [in]  Input maximum point of the view.
    */
    void putView(const OdChar* Name, const OdGePoint3d& min, const OdGePoint3d& max);
	
	/** \details
       Sets the device scale.
	   
	   \param li [in]  Input layout information.
    */
    void setDeviceScale(const DwfLayoutInfo& li);
public:
    // Auxiliary methods
    //

	/** \details
       Returns the DWF export parameters.
    */
    const DwExportParams& getParams() const
    {
      return m_Params;
    }

	/** \details
       Returns the graphical device.
    */
    OdGsDevicePtr getDevice()
    {
      return m_pDevice;
    }

    //int getWtColorIndex(const ODCOLORREF rgb) const;
	
	/** \details
       Returns the color index.
	   
	   \param rgb [in]  Input color.
	   \param max [out]  Output color index.
    */
    bool getWtColorIndex(const ODCOLORREF rgb, int& color_index) const;
	
	/** \details
       Returns the default color index.
    */
    int getWtDefaultColorIndex() const
    {
      return 1;   // todo // set color index opposite background color as default
    }
	
	/** \details
       This method is called for color presetting.
	   
	   \param color [in]  Input color.
    */
    void colorPresetting(ODCOLORREF color);

	/** \details
      Returns the palette that is used during the export.
    */
    const ODCOLORREF* getPalette() const
    {
      return m_pPalette;
    }

    // get plot paper size for current view
	
	/** \details
      Returns the plot paper size for the current view.
	  
	  \param li [in]  Input layout information.
	  \param plotWidth [out]  Output plot width.
	  \param plotHeight [out]  Output plot height.
    */
    void getPlotPaperSize(const DwfLayoutInfo& li, double& plotWidth, double& plotHeight );

    // CDwfExportMeter overwriting
    //
    //virtual OdDbDatabase* getDB()
	
	/** \details
      Returns the pointer to the database object to be exported.
    */
    virtual OdDbBaseDatabase* getDB()
    {
      //return m_Params.pDb; // before 21.12.2011
      //return (OdDbDatabase*)m_Params.pDb; // dgn dwf export
      return m_Params.database(); // dgn dwf export
    }
	
	/** \details
      Registers the font for XPS.
	   
	  \param msg [in]  Input message string.
	  \param fontName [in]  Input font name.
    */
    WT_Result registerFontForXPS(const OdChar* msg, const OdString& fontName);
	
	/** \details
      Collects the symbols from the message.
	   
	  \param typeface [in]  Input typeface.
	  \param msg [in]  Input message.
	  \param length [in]  Input message length.
    */
    void collectSymbols(const OdString typeface, const OdChar* msg, int length);
};
}
#endif /* DWF_7_EXPORTIMPL_DEFINED */
