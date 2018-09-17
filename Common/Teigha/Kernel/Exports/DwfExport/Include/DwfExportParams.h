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
// DwfExportParams.h - Open Design DWF Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _DWF_EXPORTPARAMS_INCLUDED_
#define _DWF_EXPORTPARAMS_INCLUDED_

///////////////////////////////////////////////////////////////////////////////

//#if defined(ODA_WINDOWS)
//  #include <tchar.h>
//#endif

#include "DwfExportDef.h"

#include "DbBaseDatabase.h"

//class OdDbDatabase;
//class OdDbLayout;

// Output format types
//

/** \details
  DWF export content.

  <group TD_Namespaces> 
*/
namespace TD_DWF_EXPORT {

enum DW_FORMAT {
  DW_COMPRESSED_BINARY = 0,     // NOTE: only for the DWF v5.5 (and earlier)
  DW_UNCOMPRESSED_BINARY,
  DW_ASCII,
  DW_XPS
};


const OdUInt32 lMaxDwfResolution = 0x7FFFFFFE;  // Max X and Y size

// The version is calculated as follows: (major_version * 100) + minor_version.
enum DwfVersion {
  nDwf_v55 = 55,            // DWF v5.5
  nDwf_v42 = 42,            // DWF v4.2
  nDwf_v60 = 600,           // DWF v6.0
  nDwf_v6_unknown = 602     // unknown version
};


//
// DW_EXPORT_PARAMS - Export parameters
//

/** \details
  The ImageResource element is used to describe an image (raster) file resource.
*/
struct ImageResource
{
  ImageResource()
    : sFileName(OD_T(""))
    , ColorDepth(0)
    , Width(0)
    , Height(0)
  {
  };

  ImageResource(const ImageResource& data)
  {
    Copy(data);
  }

  /** \details
    Copies raster image data from the file resource. 
	
	\param from [in] Input image file resource.
  */
  void Copy(const ImageResource& from)
  {
    sFileName = from.sFileName;
    ColorDepth = from.ColorDepth;
    Width = from.Width;
    Height = from.Height;
  }

  OdString  sFileName;        // File name 
  OdInt32   ColorDepth;       // The number of colors used in the image
  OdInt32   Width;            // Image width 
  OdInt32   Height;           // Image height
};

/** \details
  This structure stores the page parameters for DWF export.
*/
class DwfPageData
{
public:
  DwfPageData()
    : sLayout(OD_T(""))
    , sPageAuthor(OD_T(""))
    , sPageTitle(OD_T(""))
    , sPageSubject(OD_T(""))
    , sPageCompany(OD_T(""))
    , sPageComments(OD_T(""))
    , sPageReviewers(OD_T(""))
    , sPageKeywords(OD_T(""))
    , sPageDescription(OD_T(""))
    , sPageCopyright(OD_T(""))
  {
  }
  ~DwfPageData()
  {
  }

  DwfPageData(const DwfPageData& data)
  {
    *this = data;
  }

  DwfPageData& operator=(const DwfPageData& from)
  {
    sLayout         = from.sLayout;
    sPageAuthor     = from.sPageAuthor;
    sPageTitle      = from.sPageTitle;
    sPageSubject    = from.sPageSubject;
    sPageCompany    = from.sPageCompany;
    sPageComments   = from.sPageComments;
    sPageReviewers  = from.sPageReviewers;
    sPageKeywords   = from.sPageKeywords;
    sPageDescription= from.sPageDescription;
    sPageCopyright  = from.sPageCopyright;

    thumbnail = from.thumbnail;
    preview   = from.preview;
    arFonts   = from.arFonts;

    return *this;
  }

  //DwfPageData(const DwfPageData& data)
  //{
  //  Copy(data);
  //}

  //void Copy(const DwfPageData& from)
  //{
  //  sLayout         = from.sLayout;
  //  sPageAuthor     = from.sPageAuthor;
  //  sPageTitle      = from.sPageTitle;
  //  sPageSubject    = from.sPageSubject;
  //  sPageCompany    = from.sPageCompany;
  //  sPageComments   = from.sPageComments;
  //  sPageReviewers  = from.sPageReviewers;
  //  sPageKeywords   = from.sPageKeywords;
  //  sPageDescription= from.sPageDescription;
  //  sPageCopyright  = from.sPageCopyright;
  //
  //  thumbnail = from.thumbnail;
  //  preview   = from.preview;
  //  arFonts   = from.arFonts;
  //}

  OdString  sLayout;

  // list of Page (section) data
  
  OdString  sPageAuthor;           // Page author.
  OdString  sPageTitle;            // Page title.
  OdString  sPageSubject;          // Page subject.
  OdString  sPageCompany;          // Page company.
  OdString  sPageComments;         // Page comments.
  OdString  sPageReviewers;        // Page reviewers.
  OdString  sPageKeywords;         // Page keywords.
  OdString  sPageDescription;      // Page description.
  OdString  sPageCopyright;        // Page copyright.

  ImageResource         thumbnail;  // Raster Graphics (e.g. .jpeg, .png).
  ImageResource         preview;    // Preview image resource.

  OdStringArray arFonts;    // The list of the embedded fonts.
};

/** \details

   This class implements the parameters for DWF export.
*/
class DwExportParams
{
public:

  DwExportParams()
    : m_pDb(NULL)
    , m_sDwfFileName(OD_T(""))
    , m_xSize(36000)
    , m_ySize(24000)
    , m_Format(DW_ASCII)
    , m_Version(nDwf_v55)
    , m_background(ODRGB(255, 255, 255))
    , m_pPalette(NULL)
    , m_bExportInvisibleLayers(false)
    , m_bForceInitialViewToExtents(false)
    , m_bSkipLayerInfo(false)
    , m_bSkipNamedViewsInfo(false)
    , m_bInkedArea(false)                                                                   // MKU 1/21/2004
    , m_bColorMapOptimize(false)                                                            // MKU 1/21/2004
   , m_bRGBToJpeg(true)
    , m_bUseHLR(true)
    , m_nMaxPointsInPolygon(0)
    , m_nMaxRasterResolution(0)
    , m_bExportInvisibleText(true)
    , m_bEmbedAllFonts(false)
    , m_bExportGradientAsRaster(true)
    , m_dLineweightScale(1.0)
  {
  }

  ~DwExportParams()
  {
  }

  /** \details
    Returns a pointer to the database object to be exported.
  */
  OdDbBaseDatabase* database() const { return m_pDb; }
  
  /** \details
    Sets a pointer to the database to be exported. 
	
	\param pDb [in] Input pointer to the database.
  */
  void setDatabase(OdDbBaseDatabase *pDb) { m_pDb = pDb; }

  /** \details
    Sets the name of the .dwf file. 
	
	\param sDwfFileName [in] Input .dwf file name.
  */
  void setDwfFileName(const OdString& sDwfFileName) { m_sDwfFileName = sDwfFileName; }
  
  /** \details
    Returns the .dwf file name.
  */
  OdString dwfFileName() const { return m_sDwfFileName; }

  /** \details
    Sets the horizontal size of the output screen. 
	
	\param iValue [in] Input horizontal size.
  */
  void  setXSize(OdInt32 iValue)
  {
      m_xSize = iValue;
  }
  
  /** \details
    Returns the horizontal size of the output screen.
  */
  OdInt32 xSize() const
  {
      return m_xSize;
  }

  /** \details
    Sets the vertical size of the output screen. 
	
	\param iValue [in] Input vertical size.
  */
  void  setYSize(OdInt32 iValue)
  {
      m_ySize = iValue;
  }
  
  /** \details
    Returns the vertical size of the output screen.
  */
  OdInt32 ySize() const
  {
      return m_ySize;
  }

  /** \details
    Sets the format of the .dwf file. 
	
	\param value [in] Input .dwf file format.
  */
  void  setFormat(DW_FORMAT value)
  {
      m_Format = value;
  }
  
  /** \details
    Returns the format of the .dwf file.
  */
  DW_FORMAT format() const
  {
      return m_Format;
  }

  /** \details
    Sets the version of the .dwf file. 
	
	\param value [in] Input .dwf file version.
  */
  void  setVersion(DwfVersion value)
  {
      m_Version = value;
  }
  
  /** \details
    Returns the version of the .dwf file.
  */
  DwfVersion version() const
  {
      return m_Version;
  }

  /** \details
    Sets the background color of the output. 
	
	\param background [in] Input background color.
  */
  void setBackground(ODCOLORREF background) { m_background = background; }
  
  /** \details
    Returns the background color of the output.
  */
  ODCOLORREF background() const { return m_background; }

  /** \details
    Sets a new palette to be used during the export. 
	
	\param pPalette [in] Input palette to be set.
  */
  void setPalette(const ODCOLORREF* pPalette) { m_pPalette = pPalette; }
  
  /** \details
    Returns the palette that is used during the export.
  */
  const ODCOLORREF* palette() const { return m_pPalette; }

  /** \details
    Sets or clears the flag that determines whether to export invisible layers. 
	
	\param bFlag [in] Input flag.
  */
  void  setExportInvisibleLayers(bool bFlag) { m_bExportInvisibleLayers = bFlag; }
  
  /** \details
    Returns the flag that determines whether to export invisible layers.
  */
  bool  exportInvisibleLayers() const { return m_bExportInvisibleLayers; }

  /** \details
    Sets or clears the flag that determines whether to force the initial view to extents. 
	
	\param bFlag [in] Input flag.
  */
  void  setForceInitialViewToExtents(bool bFlag) { m_bForceInitialViewToExtents = bFlag; }
  
  /** \details
    Returns the flag that determines whether to force the initial view to extents.
  */
  bool  forceInitialViewToExtents() const { return m_bForceInitialViewToExtents; }

  /** \details
    Sets or clears the flag that determines whether to skip layer information. 
	
	\param bFlag [in] Input flag.
  */
  void  setSkipLayerInfo(bool bFlag)
  {
      m_bSkipLayerInfo = bFlag;
  }
  
  /** \details
    Returns the flag that determines whether to skip layer information.
  */
  bool  skipLayerInfo() const
  {
      return m_bSkipLayerInfo;
  }

  /** \details
    Sets or clears the flag that determines whether to skip named views information. 
	
	\param bFlag [in] Input flag.
  */
  void  setSkipNamedViewsInfo(bool bFlag)
  {
      m_bSkipNamedViewsInfo = bFlag;
  }
  
  /** \details
    Returns the flag that determines whether to skip named views information.
  */
  bool  skipNamedViewsInfo() const
  {
      return m_bSkipNamedViewsInfo;
  }

  /** \details
    Sets or clears the "inked area" option. 
	
	\param bFlag [in] Input "inked area" option.
  */
  void  setInkedArea(bool bFlag)
  {
      m_bInkedArea = bFlag;
  }
  
  /** \details
    Returns the "inked area" option.
  */
  bool  inkedArea() const
  {
      return m_bInkedArea;
  }

  /** \details
    Sets or clears the color map optimization. 
	
	\param bFlag [in] Input flag.
  */
  void  setColorMapOptimize(bool bFlag)
  {
      m_bColorMapOptimize = bFlag;
  }
  
  /** \details
    Returns the color map optimization status.
  */
  bool  colorMapOptimize() const
  {
      return m_bColorMapOptimize;
  }

  /** \details
    Sets or clears the flag that determines whether to save JPEG images in the DWF file rather than RGB. 
	
	\param bFlag [in] Input flag.
  */
  void  setRGBToJpeg(bool bFlag)
  {
      m_bRGBToJpeg = bFlag;
  }
  
  /** \details
    Returns the flag that determines whether to save JPEG images in the DWF file rather than RGB.
  */
  bool  RGBToJpeg() const
  {
      return m_bRGBToJpeg;
  }

  /** \details
    Sets or clears the flag that determines whether the software's vector hidden-line removal is used for the corresponding viewports. 
	
	\param bFlag [in] Input flag.
  */
  void  setUseHLR(bool bFlag)
  {
      m_bUseHLR = bFlag;
  }
  
  /** \details
    Returns the flag that determines whether the software's vector hidden-line removal is used for the corresponding viewports.
  */
  bool  useHLR() const
  {
      return m_bUseHLR;
  }

  /** \details
    Sets or clears the flag that determines whether to save a gradient hatch as a raster image. 
	
	\param bFlag [in] Input flag.
  */
  void  setExportGradientAsRaster(bool bFlag)
  {
      m_bExportGradientAsRaster = bFlag;
  }
  
  /** \details
    Returns the flag that determines whether to save a gradient hatch as a raster image.
  */
  bool  exportGradientAsRaster() const
  {
      return m_bExportGradientAsRaster;
  }

  /** \details
    Sets the maximum number of points in a polygon. 
	
	\param uValue [in] Input number of maximum points in a polygon.
  */
  void  setMaxPointsInPolygon(OdUInt32 uValue)
  {
      m_nMaxPointsInPolygon = uValue;
  }
  
  /** \details
    Returns the maximum number of points in a polygon.
  */
  OdUInt32 maxPointsInPolygon() const
  {
      return m_nMaxPointsInPolygon;
  }

  /** \details
    Sets the maximum raster resolution. 
	
	\param uValue [in] Input maximum raster resolution.
  */
  void  setMaxRasterResolution(OdUInt16 uValue) { m_nMaxRasterResolution = uValue; }
  
  /** \details
    Returns the maximum raster resolution.
  */
  OdUInt16 maxRasterResolution() const { return m_nMaxRasterResolution; }

  /** \details
    Sets the password of the .dwf file. 
	
	\param sPassword [in] Input .dwf file password.
  */
  void setPassword(const OdString& sPassword) { m_sPassword = sPassword; }
  
  /** \details
    Returns the password of the .dwf file.
  */
  OdString password() const { return m_sPassword; }

  /** \details
    Sets the publisher name. 
	
	\param sPublisher [in] Input publisher name.
  */
  void setPublisher(const OdString& sPublisher) { m_sPublisher = sPublisher; }
  
  /** \details
    Returns the publisher name.
  */
  OdString publisher() const { return m_sPublisher; }

  /** \details
    Sets the wide comments. 
	
	\param sWideComments [in] Input wide comments.
  */
  void setWideComments(const OdString& sWideComments) { m_sWideComments = sWideComments; }
  
  /** \details
    Returns the wide comments.
  */
  OdString wideComments() const { return m_sWideComments; }

  /** \details
    Sets the source product name. 
	
	\param sSourceProductName [in] Input source product name.
  */
  void setSourceProductName(const OdString& sSourceProductName) { m_sSourceProductName = sSourceProductName; }
  
  /** \details
    Returns the source product name.
  */
  OdString sourceProductName() const { return m_sSourceProductName; }

  /** \details
    Sets the page data. 
	
	\param pageData [in] Input page data.
  */
  void setPageData(const OdArray<DwfPageData>& pageData ) 
  { 
    m_arPageData = pageData; 
  }
  
  /** \details
    Returns the page data.
  */
  OdArray<DwfPageData>& pageData() 
  { 
    return m_arPageData; 
  }

  /** \details
    Sets or clears the flag that determines whether to export invisible text. 
	
	\param bFlag [in] Input flag.
  */
  void  setExportInvisibleText(bool bFlag) { m_bExportInvisibleText = bFlag; }
  
  /** \details
    Returns the flag that determines whether to export invisible text.
  */
  bool  exportInvisibleText() const { return m_bExportInvisibleText; }

  /** \details
    Sets or clears the flag that determines whether to embed all of the fonts encountered in the document to the .dwf file. 
	
	\param bFlag [in] Input flag.
  */
  void  setEmbedAllFonts(bool bFlag) { m_bEmbedAllFonts = bFlag; }
  
  /** \details
    Returns the flag that determines whether to embed all of the fonts encountered in the document to the .dwf file.
  */
  bool  embedAllFonts() const { return m_bEmbedAllFonts; }
  
  /** \details
    Sets the lineweight scale. 
	
	\param s [in] Input lineweight scale.
  */
  void  setLineweightScale(double s) { m_dLineweightScale = s; }
  
  /** \details
    Returns the lineweight scale.
  */
  double lineweightScale() const { return m_dLineweightScale; }

private:
  OdDbBaseDatabase* m_pDb;
  OdString          m_sDwfFileName;

  OdInt32           m_xSize;        // DWF file resolution (less resolution - less file size
  OdInt32           m_ySize;        //

  // Optional parameters. May be zeroed to use defaults
  //
  DW_FORMAT         m_Format;       // Output format (compressed, binary, ASCII)
  DwfVersion        m_Version;      // DWF version (currently 42 or 55)

  ODCOLORREF        m_background;   // Background color. Default - white
  const ODCOLORREF* m_pPalette;     // Palette to be used. It's size depends on the highest
                                    //  index in ad3parm.colors array.
                                    //  If NULL - one of two default palettes
                                    //  will be used depending on background color

  bool  m_bExportInvisibleLayers;   // If non-zero invisible and frozen layers will be exported
                                    //  Note: not supported
  bool  m_bForceInitialViewToExtents; // If non-zero Initial view will be set
                                    //  to extents else - active view saved in drawing
                                    //  Note: for Model Space only
  bool  m_bSkipLayerInfo;           // Don't save layer info
  bool  m_bSkipNamedViewsInfo;      // Don't save named views
  bool  m_bInkedArea;               // Sets on/off the Inked Area
                                    //  The Inked Area opcode specifies a rectangle, in logical
                                    //  coordinates, in which all of the .dwf fileӳ geometric
                                    //  primitives fit tightly inside.
                                    // ATTENTION: The slowing-down of performance is possible
                                    //            in case that this option is set on.
  bool  m_bColorMapOptimize;        // True - to set on the color map optimization;
                                    //  as a result this one keeps the actual colors only.
                                    //  false - to set off the optimization
                                    //  and to export whole color map always
                                    // ATTENTION: The slowing-down of performance is possible
                                    //            in case that this option is set on.
  bool  m_bRGBToJpeg;               // If it is true an attempt to save JPEG images in DWF file
                                    //  rather than RGB will be made - less output size.
                                    // The result is depend on RX_RASTER_SERVICES implementation.
  bool  m_bUseHLR;                  // Is software vector hidden-line removal used for
                                    // corresponding viewports
  bool  m_bExportGradientAsRaster;  // Save gradient hatch as raster image. For complex hatches 
                                    // this both decreases file size and improves visual quality

  /*
   * DWF format describes polygon entity with
   * great (>65000) number of vertices. It's the most compact representation of
   * filled areas (more compact that polytriangles).
   * But AutoDesk's viewer which probably will be used to display DWF files
   * has problems with rendering polygons.
   * First it does not render correctly polygons with number of vertices > 64.
   * It often crashes while zooming polygons with more than 32 vertices.
   * BTW AutoCAD 2000 does not export filled polygons to DWF (tristrips only)
   */
  OdUInt32  m_nMaxPointsInPolygon;  // Max number of points in polygon
                                    //  Default is 0 - polygons will be triangulated to tristrips.
  OdUInt16  m_nMaxRasterResolution; // If non zero sets the limit of raster image resolution

  OdString  m_sPassword;
  // dwf-wide metadata
  OdString  m_sPublisher;
  OdString  m_sWideComments;
  OdString  m_sSourceProductName;   // Brief description of the application from
                                    //  which the source data was created
  OdArray<DwfPageData>  m_arPageData;
  
  // AutoCAD 2004 and earlier exported invisible text along with vectorized SHX text, 
  // to allow text search in DWF. In version 2005 this feature was removed.
  // This flags controls the behaviour of our export.
  //
  bool  m_bExportInvisibleText;
  bool  m_bEmbedAllFonts;           // embed all the fonts encountered in the document to DWFx file
  double m_dLineweightScale;        // additional lineweight scale, default is 1 (no scaling)
}; // DwExportParams

/** \details

   This class implements the parameters for 3D DWF export.
*/
class Dwf3dExportParams 
{
public:

  Dwf3dExportParams()
    : m_pDb(NULL)
    , m_xSize(1024)
    , m_ySize(768)
    , m_background(ODRGB(255, 255, 255))
    , m_pPalette(NULL)
  {
  }

  ~Dwf3dExportParams()
  {
  }

  /** \details
    Returns a pointer to the database object to be exported.
  */
  OdDbBaseDatabase* database() const { return m_pDb; }
  
  /** \details
    Sets a pointer to the database to be exported. 
	
	\param pDb [in] Input pointer to the database.
  */
  void setDatabase(OdDbBaseDatabase *pDb) { m_pDb = pDb; }

  /** \details
    Sets the name of the .dwf file. 
	
	\param sDwfFileName [in] Input .dwf file name.
  */
  void setDwfFileName(const OdString& sDwfFileName) { m_sDwfFileName = sDwfFileName; }
  
  /** \details
    Returns the .dwf file name.
  */
  OdString dwfFileName() const { return m_sDwfFileName; }

  /** \details
    Sets the horizontal size of the output screen. 
	
	\param iValue [in] Input horizontal size.
  */
  void  setXSize(OdInt32 iValue)
  {
      m_xSize = iValue;
  }
  
  /** \details
    Returns the horizontal size of the output screen.
  */
  OdInt32 xSize() const
  {
      return m_xSize;
  }

  /** \details
    Sets the vertical size of the output screen. 
	
	\param iValue [in] Input vertical size.
  */
  void  setYSize(OdInt32 iValue)
  {
      m_ySize = iValue;
  }
  
  /** \details
    Returns the vertical size of the output screen.
  */
  OdInt32 ySize() const
  {
      return m_ySize;
  }

  /** \details
    Sets the background color for the output. 
	
	\param background [in] Input background color.
  */
  void setBackground(ODCOLORREF background) { m_background = background; }
  
  /** \details
    Returns the background color for the output.
  */
  ODCOLORREF background() const { return m_background; }

  /** \details
    Sets a new palette to be used during the export. 
	
	\param pPalette [in] Input palette to be set.
  */
  void setPalette(const ODCOLORREF* pPalette) { m_pPalette = pPalette; }
  
  /** \details
    Returns palette that is used during the export.
  */
  const ODCOLORREF* palette() const { return m_pPalette; }

  /** \details
    Sets a new EModel section title. 
	
	\param sTitle [in] Input title.
  */
  void setTitle(const OdString& sTitle) { m_sTitle = sTitle; }
  
  /** \details
    Returns the EModel section title.
  */
  OdString title() const { return m_sTitle; }

  /** \details
    Sets a new thumbnail. 
	
	\param sTitle [in] Input title.
  */
  void setThumbnail(ImageResource thumbnail) { m_thumbnail = thumbnail; }
  
  /** \details
    Returns the thumbnail.
  */
  ImageResource thumbnail() const { return m_thumbnail; }

private:
  OdDbBaseDatabase* m_pDb;
  OdString          m_sDwfFileName;


  OdInt32     m_xSize;
  OdInt32     m_ySize;

  // Optional parameters. May be zeroed to use defaults
  //
  ODCOLORREF  m_background;       // Background color. Default - white
  const ODCOLORREF* m_pPalette;   // Palette to be used. It's size depends on the highest
                                  //  index in ad3parm.colors array.
                                  //  If NULL - one of two default palettes
                                  //  will be used depending on background color

  // Properties
  OdString      m_sTitle;         // Corresponds to the EModel section title
  ImageResource m_thumbnail;      // Raster Graphics (e.g. .jpeg, .png)

}; // 3dDwfExportParams
}

#endif // _DWF_EXPORTPARAMS_INCLUDED_
