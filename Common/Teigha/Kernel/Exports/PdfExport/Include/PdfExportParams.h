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

#ifndef _PDFEXPORTPARAMS_INCLUDED_
#define _PDFEXPORTPARAMS_INCLUDED_ /*!DOM*/ 

#include "PdfExportDef.h"

#include "Gs/GsPageParams.h"

#include "DbBaseDatabase.h"
#include "OdStreamBuf.h"
#include "PrcContextForPdfExport.h"
#include "Pdf/Pdf3dEnums.h"

/** \details
  <group TD_Namespaces>
*/
namespace TD_PDF_2D_EXPORT {

/** \details
  This class implements the PDF export parameters.
  
  <group OdExport_Classes> 
*/
class PDFExportParams
{
public:

  /** \details
  PDF export flags enumeration that can be used for handling the export process.
  */
  enum PDFExportFlags
  {
    kZeroFlag = 0,                  // All flags disabled.
    kEmbededTTF = 1,                // Enable embedding of True Type font program to PDF file. The whole True Type font file will be embedded (huge .pdf size).
    kTTFTextAsGeometry = 2,         // Enable True Type font text to geometry conversion.
    kSHXTextAsGeometry = 4,         // Enable SHX font text to geometry conversion.
    kSimpleGeomOptimization = 8,    // Enable simple geometry optimization (separated segments to one polyline, Bezier curve control points).
    kEnableLayers = 16,             // If enabled, layer support will be added to .pdf file (1.5 version required).
    kIncludeOffLayers = 32,         // If enabled, invisible layers will be added to .pdf file (1.5 version required).
    kEmbededOptimizedTTF = 64,      // Enable embedding of True Type font program to .pdf file. The True Type font will be reduced by removing unused parts (smaller .pdf file size, but limited editing).
    kUseHLR = 128,                  // Determines whether software uses vector hidden-line removal for corresponding viewports.
    kFlateCompression = 256,        // Enable compression of internal PDF streams.
    kASCIIHexEncoding = 512,        // Enable ASCIIHex encoding of internal PDF streams.
    kExportHyperlinks = 1024,       // Enable hyperlinks export
    kZoomToExtentsMode = 2048,      // If enabled, layout geometry will be zoomed to paper size, else layout settings will be applied.
    //Note: Z2E mode works like Layout settings: "Plot Area" - "Extents", "Fit to Paper", "Center the plot" applied to specified paper size and orientation,
    //but some settings like "Plot lineweight", "Plot style" are still applying from Layout settings
    kLinearized = 4096,             // Create linearized .pdf file
    kMergeLines = 8192,             // Enable merging the colors of crossing lines
    kMeasuring = 16384,             // Enable measuring scale in pdf

    kDefault = kTTFTextAsGeometry | kSHXTextAsGeometry | kFlateCompression | kASCIIHexEncoding | kZoomToExtentsMode// default flag combination
  };

  /** \details 
  PDF/A format mode enumeration. 
  */
  enum PDF_A_mode
  {
    /** PDF/A format is not supported*/
    kPDFA_None = 0,
    /**PDF/A-1b version*/
    kPDFA_1b,
    /**PDF/A-2b version*/
    kPDFA_2b
  };

  /** \details
  PRC export support enumeration.
  */
  enum PRCSupport
  {
    kDisabled = 0, // Export to PRC is disabled.
    kAsBrep   = 1, // Exports PRC is supported as Boundary representation.
    kAsMesh   = 2  // Exports PRC is supported as Mesh representation.
  };

  /** \details
  Hatches export type. 
  */
  enum ExportHatchesType
  {
    kBitmap =   0, //Exports hatches as a bitmap.
    kDrawing =  1, //Exports hatches as a drawing (vectorizer).
    kPdfPaths = 2  //Exports hatches as a PDF path.
  };

  /** \details
  Searchable text type.
  */
  enum SearchableTextType
  {
    kNoSearch = 0,  // No searchable text.
    kSHX =  1,      // Only text displayed with SHX font is searchable.
    kTTF =  2       // Only text displayed with TTF font is searchable.
  };

  /** \details
  Export color policy. Determines how colors form an original drawing are exported to .pdf file.
  */
  enum ColorPolicy
  {
    kNoPolicy = 0,  // No color policy. A drawing is exported with its native color palette (as is).
    kMono = 1,      // Drawing is exported to a monochrome .pdf file.
    kGrayscale = 2  // Drawing is exported to a grayscale .pdf file. Native colors are converted into graysca
  };

  /** \details
	PDF format versions enumeration, used during the export process.
  */
  enum PDFExportVersions
  {
    kPDFv1_0 = 10,
    kPDFv1_1 = 11,
    kPDFv1_2 = 12,
    kPDFv1_3 = 13,
    kPDFv1_4 = 14,
    kPDFv1_5 = 15,
    kPDFv1_6 = 16,
    kPDFv1_7 = 17,
    kLast    = kPDFv1_7
  };

  /** \details
  Creates a new set of export parameters with default values. 
  */
  PDFExportParams()
    : m_pDb(0)
    , m_ver(kPDFv1_6)
    , m_background(ODRGB(255, 255, 255))
    , m_pPalette(0)
    , m_flags(kDefault)
    , m_hatchDPI(72)
    , m_solidHatchesExportType(kBitmap)
    , m_otherHatchesExportType(kBitmap)
    , m_gradientHatchesExportType(kBitmap)
    , m_VectorResolution(600)
    , m_colorImagesDPI(400)
    , m_bwImagesDPI(400)
    , m_PRCMode(kDisabled)
    , m_PRCContext()
    , m_prcHasBackground(false)
    , m_prcBackground(ODRGB(255, 255, 255))
    , m_PRCRenderMode(PDF3D_ENUMS::kDefaultMode)
    , m_searchableTextType(kNoSearch)
    , m_colorPolicy(kNoPolicy)
    , m_bCropImages(true)
    , m_bDCTCompression(false)
    , m_DCTQuality(50)
    , m_PRCCompressionLevel(0)
    , m_useViewExtents(false)
    , m_pdfAMode(kPDFA_None)
    , m_reserved1(0)
    , m_reserved2(0)
    , m_reserved3(0)
    , m_reserved4(0)
    , m_Export2XObject(false)
  {
  }

  /** \details
  Destroys the set of export parameters. 
  */
  ~PDFExportParams() {}

  /** \details
  Sets export to PRC flag value.
  
  \param flags [in]  A new flag value to be set.
  */
  void setPRCMode(PRCSupport flags)
  {
    if (kDisabled != flags)
    {
      m_flags = (PDFExportFlags)(m_flags & ~kUseHLR);
      m_ver = kPDFv1_6;
    }
    m_PRCMode = flags;
  }
  
  /** \details
  Returns the export to PRC flag value.
  */
  PRCSupport getPRCMode(void) const { return m_PRCMode; }

  /** \details
  Returns the pointer to the prc export context.
  */
  OdPrcContextForPdfExport* getPRCContext(void)  { return (OdPrcContextForPdfExport*)m_PRCContext.get(); }
  const OdPrcContextForPdfExport* getPRCContext(void) const { return (OdPrcContextForPdfExport*)m_PRCContext.get(); }
  
  /** \details
  Sets prc export context

  \param pContext [in] A context that will be used during the export of prc file
  */
  void setPRCContext(OdRxObject *pContext) { m_PRCContext = pContext; }

  /** \details
  Return true if prc background is set.
  */
  bool hasPrcBackground(void) const { return m_prcHasBackground; }

  /** \details
  Return color of prc background is set.
  */
  ODCOLORREF getPrcBackground(void) const { return m_prcBackground; }

  /** \details
  Sets prc background

  \param bacgr [in] A color of default background in created prc file
  */
  void setPrcBackground(ODCOLORREF bacgr) { m_prcHasBackground = true; m_prcBackground = bacgr; }

  /** \details
  Clear prc background.
  */
  void clearPrcBackground() { m_prcHasBackground = false; }

  /** \details
  Returns the current value of the default PRC rendering mode.
  */
  PDF3D_ENUMS::PRCRenderingMode getPrcRenderingMode() { return m_PRCRenderMode; }

  /** \details
  Sets prc rendering mode

  \param renderMode [in] A new default rendering mode value
  */
  void setPrcRenderingMode(PDF3D_ENUMS::PRCRenderingMode renderMode) { m_PRCRenderMode = renderMode; }

  /** \details
  Return true if prc BREP compression is set.

  \param compressionLev [out] Receives the PRCCompressionLevel parameter.
  */
  bool hasPrcBrepCompression(PDF3D_ENUMS::PRCCompressionLevel &compressionLev) const
  {
    if ((m_PRCCompressionLevel & 0x00000004) != 0)
    {
      // apply mask
      compressionLev = (PDF3D_ENUMS::PRCCompressionLevel)(m_PRCCompressionLevel & 0x00000003);
      return true;
    }
    return false;
  }

  /** \details
  Return true if prc tessellation compression is set.
  */
  bool hasPrcTessellationCompression() const { return ((m_PRCCompressionLevel & 0x00000008) != 0); }

  /** \details
  Sets a pointer to a database to be exported.
  
  \param pDb [in]  A new pointer value to be set.
  */
  void setDatabase(OdDbBaseDatabase *pDb) { m_pDb = pDb; }
  
  /** \details
  Returns a pointer to database object to be exported.
  */
  OdDbBaseDatabase* database() const { return m_pDb; }

  /** \details
  Sets a new value for the version parameter of the output PDF file.
  
  \param version [in]  A new value of the PDF version parameter.
  */
  void setVersion(PDFExportVersions version) { m_ver = version; }

  /** \details
  Returns the current value of the version parameter for the output PDF file. 
  */
  PDFExportVersions version() const { return m_ver; }

  /** \details
  Sets a new output stream object for writing PDF file.
  
  \param output [in]  A reference to an OdStreamBuff pointer, representing output stream object to be used for writing data into the PDF file.
  */
  void setOutput(const OdStreamBufPtr& output) { m_outputStream = output; }

  /** \details
  Returns an output stream object for writing PDF file.
  */
    OdStreamBufPtr output() const { return m_outputStream; }

  /** \details
  Sets a new export flags value.
  
  \param flags [in]  An instance of PDF export flags to be set.
  */
  void setExportFlags(PDFExportFlags flags) { m_flags = flags; }
  
  /** \details
  Returns the current value of PDF export flags.
  */
  PDFExportFlags exportFlags() const { return m_flags; }

  /** \details
  Sets a new PDF/A mode value.

  \param mode [in]  An instance of PDF/A mode to be set.
  */
  void setArchived(PDF_A_mode mode) { m_pdfAMode = mode; }

  /** \details
  Returns the current value of PDF/A mode.
  */
  PDF_A_mode archived() const { return m_pdfAMode; }

  /** \details
  Sets a new searchable text type value.

  \param flags [in]  An instance of PDF export flags to be set.
  */
  void setSearchableTextType(SearchableTextType type) { m_searchableTextType = type; }

  /** \details
  Returns the current value of searchable text type.
  */
  SearchableTextType searchableTextType() const { return m_searchableTextType; }

  /** \details
  Sets a new export color policy value.

  \param policy [in] color policy to be set.
  */
  void setColorPolicy(ColorPolicy policy) { m_colorPolicy = policy; }

  /** \details
  Returns the current value of color policy.
  */
  ColorPolicy colorPolicy() const { return m_colorPolicy; }

  /** \details
  Sets an array of drawing layouts to be exported to PDF.
  
  \param layouts [in]  A new array of layouts to be exported.
  
  \remarks
  If array is zero size, only active layout is exported.
  */
  void setLayouts(const OdArray<OdString>& layouts) { m_layouts = layouts; }

  /** \details
  Returns an array of drawing layouts to be exported to PDF.
  
  \remarks
  If array is zero size, only active layout is exported.
  */
  OdArray<OdString>& layouts() { return m_layouts; }

  /** \details
  Sets a new array of page parameters to be used during the exporting.
  
  \param pageParams [in]  A new page parameters array to be set.
  
  \remarks
  Page parameters contain such options as: paper size, margins. All the values are measured in mm.
  */
  void setPageParams(const OdArray<OdGsPageParams>& pageParams) { m_pageParams = pageParams; }
  
  /** \details
  Returns an array of page parameters to be used during the exporting.
  
  \remarks
  Page parameters contain such options as: paper size, margins. All the values are measured in mm.
  */
  OdArray<OdGsPageParams>& pageParams() { return m_pageParams; }

  /** \details
  Sets a new value of the background color for the output PDF document.
  
  \param background [in]  A new value for background color.
  
  \remarks
  Default background color is white.
  */
  void setBackground(ODCOLORREF background) { m_background = background; }
  
  /** \details
  Returns the current value of the background color for the output PDF document.
  
  \remarks
  Default background color is white.
  */
  ODCOLORREF background() const { return m_background; }

  /** \details
  Sets a new palette to be used during the exporting.
  
  \param pPalette [in]  A new palette to be set.
  
  \remarks
  If pPalette is NULL, one of two default palettes will be used depending on background color value.
  */
  void setPalette(const ODCOLORREF* pPalette) { m_pPalette = pPalette; }
  
  /** \details
  Returns the palette to be used during the exporting.
  
  \remarks
  If pPalette is NULL, one of two default palettes will be used depending on background color value.
  */
  const ODCOLORREF* palette() const { return m_pPalette; }

  /** \details
  Sets a new value for vector resolution.
  
  \param dpi [in]  A new value for vector resolution to be set (in dots per inch, dpi).
  
  \remarks
  Standard values are: 72, 150, 200, 300, 400, 600, 720, 1200, 2400 and 4800 DPI. 
  Default value is 600 DPI.
  */
  void     setGeomDPI(OdUInt16 dpi) { if (dpi >= 72 && dpi <= 40000) m_VectorResolution = dpi; };
  
    /** \details
  Returns a new value for vector resolution (in dots per inch, dpi).
  
  \remarks
  Standard values are: 72, 150, 200, 300, 400, 600, 720, 1200, 2400 and 4800 DPI. 
  Default value is 600 DPI.
    */
  OdUInt16 getGeomDPI() const       { return m_VectorResolution; };

  /** \details
  Sets a new value of the resolution for images created as a result of hatch export.
  
  \param dpi [in]  A new value of the resolution to be set (in dots per inch, dpi).
  
  \remarks
  Hatch resolution value is used when hatch entity is exported as a bitmap.
  */
  void setHatchDPI(OdUInt16 dpi) { m_hatchDPI = dpi; }
  
  /** \details
  Returns the current value of the resolution for images created as a result of hatch export  (in dots per inch, dpi).
  
  \remarks
  Hatch resolution value is used when hatch entity is exported as a bitmap.
  */
  OdUInt16 hatchDPI() const      { return m_hatchDPI; }

  /** \details
  Sets a new value of the resolution for color and grayscale images.
  
  \param dpi [in]  A new value of the resolution to be set  (in dots per inch, dpi).
  
  \remarks
  Cannot exceed vector resolution value.
  */
  void setColorImagesDPI(OdUInt16 dpi) { if (dpi >= 72 && dpi <= m_VectorResolution) m_colorImagesDPI = dpi; }
  
  /** \details
  Returns the current value of the resolution for color and grayscale images  (in dots per inch, dpi).
  
  \remarks
  Cannot exceed vector resolution value.
  */
  OdUInt16 colorImagesDPI() const      { return m_colorImagesDPI; }

  /** \details
  Sets a new value of the resolution for monochrome images in the output PDF document.
  
  \param dpi [in]  A new value of the resolution for monochrome images (in dots per inch, dpi).
  
  \remarks
  Cannot exceed vector resolution value.
  */
  void setBWImagesDPI(OdUInt16 dpi) { if (dpi >= 72 && dpi <= m_VectorResolution) m_bwImagesDPI = dpi; }

  /** \details
  Returns the current value of the resolution for monochrome images in the output PDF document (in dots per inch, dpi).
  
  \remarks
  Cannot exceed vector resolution value.
  */
    OdUInt16 bwImagesDPI() const      { return m_bwImagesDPI; }

  /** \details
  Sets a new title for the output PDF document.
  
  \param sTitle [in]  A new value for the output document's title.
  */
  void setTitle(const OdString& sTitle) { m_Title = sTitle; }
  
  /** \details
  Returns a title for the output PDF document.
  */
    OdString title() const { return m_Title; }

  /** \details
  Sets a new value of the author of the output PDF document.
  
  \param sAuthor [in]  A new value of the author of the output document.
  */
  void setAuthor(const OdString& sAuthor) { m_Author = sAuthor; }
  
  /** \details
  Returns an author of the output PDF document.
  */
  OdString author() const { return m_Author; }

  /** \details
  Sets a new value of the output document's subject.
  
  \param sSubject [in]  A new value for the subject.
  */
  void setSubject(const OdString& sSubject) { m_Subject = sSubject; }
  
  /** \details
  Returns a value of the output document's subject.
  */
  OdString subject() const { return m_Subject; }

  /** \details
  Sets keywords associated with the output PDF document.
  
  \param sKeywords [in]  A new keywords string to be set.
  */
  void setKeywords(const OdString& sKeywords) { m_Keywords = sKeywords; }
  
  /** \details
  Returns keywords associated with the output PDF document.
  */
  OdString keywords() const { return m_Keywords; }

  /** \details
  Sets a new value of the output document's creator.
  
  \param sCreator [in]  A new value of the creator.
  
  \remarks
  If the output document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker) that created the original document from which it was converted.
  */
  void setCreator(const OdString& sCreator) { m_Creator = sCreator; }
  
  /** \details
  Returns the output document's creator.
  
  \remarks
  If the output document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker) that created the original document from which it was converted.
  */
  OdString creator() const { return m_Creator; }

  /** \details
  Sets a new value of the document's producer.
  
  \param sProducer [in]  A new value of the producer to be set.
  
  \remarks
  If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF.
  If sProducer is null, it will be "ODA PDF Export vX.X".
  */
  void setProducer(const OdString& sProducer) { m_Producer = sProducer; }
  
  /** \details
  Returns the output document's producer.
  
  \remarks
  If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF.
  If producer string is null, it will be "ODA PDF Export vX.X".
  */
  OdString producer() const { return m_Producer; }

  /** \details
  Sets the type of solid hatches export.
  
  \param type [in]  A new value of the solid hatches export type.
  
  \remarks
  There are 3 types of exporting to PDF for solid hatches:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  * PDF path (type = kPdfPaths);
  */
  void setSolidHatchesExportType(ExportHatchesType type) { m_solidHatchesExportType = type; }
  
  /** \details
  Returns the type of solid hatches export.
  
  \remarks
  There are 3 types of exporting to PDF for solid hatches:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  * PDF path (type = kPdfPaths);
  */
  ExportHatchesType solidHatchesExportType() const { return m_solidHatchesExportType; }

  /** \details
  Sets the type of other (non solid) hatches export.
  
  \param type [in]  A new value of the non solid hatches export type.
  
  \remarks
  For non solid hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  void setOtherHatchesExportType(ExportHatchesType type)
  {
    m_otherHatchesExportType = type == kBitmap ? kBitmap : kDrawing;
  }
  
  /** \details
  Returns the type of other (non solid) hatches export.
  
  \param type [in]  A new value of the non solid hatches export type.
  
  \remarks
  For non solid hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  ExportHatchesType otherHatchesExportType() const { return m_otherHatchesExportType; }

  /** \details
  Sets the type of gradient hatches export.

  \param type [in]  A new value of the gradient hatches export type.

  \remarks
  For gradient hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  void setGradientHatchesExportType(ExportHatchesType type)
  {
    m_gradientHatchesExportType = type == kBitmap ? kBitmap : kDrawing;
  }

  /** \details
  Returns the type of gradient hatches export.

  \remarks
  For gradient hatches, the following types of PDF export are available:
  * Bitmap (type = kBitmap);
  * Vectorizer (type = kDrawing);
  */
  ExportHatchesType gradientHatchesExportType() const { return m_gradientHatchesExportType; }

  /** \details
  Returns the image cropping option
  */
  bool imageCropping() const { return m_bCropImages; }

  /** \details
  Set the image cropping option
  */
  void setImageCropping(bool bEnable) { m_bCropImages = bEnable; }

  /** \details
  Returns the quality of DCT compression
  */
  OdUInt16 dctQuality() const { return m_DCTQuality; }

  /** \details
  Set the quality of DCT compression
  */
  void setDCTQuality(OdUInt16 quality)
  {
    if (quality > 100)
      m_DCTQuality = 100;
    else if (quality < 10)
      m_DCTQuality = 10;
    else
      m_DCTQuality = quality;
  }

  /** \details
  Returns the extents calculation mode
  */
  bool useViewExtents() const { return m_useViewExtents; }

  /** \details
  Set the extents calculation mode
  */
  void setUseViewExtents(bool bViewExtents) { m_useViewExtents = bViewExtents; }

  /** \details
  Returns the DCT compression option
  */
  bool dctCompression() const { return m_bDCTCompression; }

  /** \details
  Set the DCT compression option
  */
  void setDCTCompression(bool bEnable) { m_bDCTCompression = bEnable; }


  /** \details
  Set the prc objects compression option
  \param compressionLevel       [in] Determines what compression level (depends on used algorithm) is applied during the export process.
  \param bCompressBrep          [in] A Brep data compression flag. If the value is equal to true, the Brep data is compressed.
  \param bCompressTessellation  [in] A tesselation data compression flag. If the value is equal to true, the tesselation data is compressed.
  */
  void setPRCCompression(PDF3D_ENUMS::PRCCompressionLevel compressionLevel, bool bCompressBrep, bool bCompressTessellation)
  {
    if ((!bCompressBrep) && (!bCompressTessellation))
    {
      SETBIT_0(m_PRCCompressionLevel, 0x00000004);
      SETBIT_0(m_PRCCompressionLevel, 0x00000008);
      return;
    }

    m_PRCCompressionLevel = compressionLevel;
    SETBIT(m_PRCCompressionLevel, 0x00000004, bCompressBrep);
    SETBIT(m_PRCCompressionLevel, 0x00000008, bCompressTessellation);
  }

  bool export2XObject() const { return m_Export2XObject; }

  void setExport2XObject(bool bEnable) { m_Export2XObject = bEnable; }

private:
  OdDbBaseDatabase *m_pDb;            // Pointer to the database for export.
  PDFExportVersions m_ver;           // Only PDF 1.4 supported & 1.5 for layers functionality.
  OdStreamBufPtr    m_outputStream;  // Pointer to the stream buffer for PDF export implementation.

  OdArray<OdString>      m_layouts;       // Layouts to export. If zero size array, active layout will be exported.
  OdArray<OdGsPageParams> m_pageParams;    // Page parameters: paper size, margins, in mm.

  ODCOLORREF        m_background;    // Background color. Default: white.
  const ODCOLORREF* m_pPalette;      // Palette to be used. If NULL, one of two default palettes will be used depending on background color.

  OdUInt16          m_hatchDPI; // PdfExport exports hatch entity as bitmap. This value controls DPI resolution for images created during hatch export.
  //OdUInt16          view3dDPI; // 3d viewports exported as images if bUseHLR set to true. This value controls DPI of such inages.
  OdUInt16          m_VectorResolution; // Used for vector resolution. Standard values are 72, 150, 200, 300, 400, 600, 720, 1200, 2400 and 4800 DPI. Default value is 600 DPI
  OdUInt16          m_colorImagesDPI;//Used for control resolution for color and grayscale images. Cannot exceed m_VectorResolution value
  OdUInt16          m_bwImagesDPI;//Used for control resolution for monochrome images. Cannot exceed m_VectorResolution value

  ExportHatchesType m_solidHatchesExportType;
  ExportHatchesType m_gradientHatchesExportType;
  ExportHatchesType m_otherHatchesExportType;

  PDFExportFlags m_flags; // PDF export flag combination used for export.

  SearchableTextType m_searchableTextType;//can be used if SHX or TTF text exported as geometry
  ColorPolicy        m_colorPolicy;

  // All next fields are optional (can be ""). If Producer is null, it will be "ODA PDF Export vX.X".
  OdString m_Title;    // The document title.
  OdString m_Author;   // The name of the person who created the document.
  OdString m_Subject;  // The subject of the document.
  OdString m_Keywords; // Keywords associated with the document.
  OdString m_Creator;  // If the document was converted to PDF from another format, the name of the application (for example, Adobe FrameMaker) that created the original document from which it was converted.
  OdString m_Producer; // If the document was converted to PDF from another format, the name of the application (for example, Acrobat Distiller) that converted it to PDF.

  bool                  m_bCropImages;     // Enable bitmap cropping(crop invisible parts of bitmaps)
  bool                  m_bDCTCompression; // DCT compression for raster images
  OdUInt16              m_DCTQuality;      // Quality of DCT compression

  bool                  m_useViewExtents;  // Use view extents instead of plot extents (default) for calculation of drawing extents

  PDF_A_mode            m_pdfAMode;  //PDF/A mode - default value None
  //PDF/A is not compatible with PRC at all (by ISO standard) so the error will be returned if both modes are set
  //Optimized TTF fonts aren't supported yet, so this flag will be unset if PDF/A mode is chosen
  //For more details, see documentation

  //PRC related params
  PRCSupport                    m_PRCMode;
  OdUInt32                      m_PRCCompressionLevel;  //PRCCompressionLevel and 0x00000004 is bCompressBrep and 0x00000008 is bCompressTessellation
  OdRxObjectPtr                 m_PRCContext; // user-defined PRC context
  bool                          m_prcHasBackground; // True => Prc backgrounds should be enabled
  ODCOLORREF                    m_prcBackground;    // Default background color for PRC. Default: white.
  PDF3D_ENUMS::PRCRenderingMode m_PRCRenderMode;
  bool                          m_Export2XObject;
public:
  //new reserved parameters
  OdUInt16          m_reserved1;  //bit 1 - ZoomToExtents auxiliary flag                               - default value 0 [ExactExtents]
  OdUInt16          m_reserved2;
  OdUInt32          m_reserved3;
  OdUInt32          m_reserved4;
};
}
#endif //_PDFEXPORTPARAMS_INCLUDED_

