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


#ifndef _PDFPUBLISH_COMMON_INCLUDED_
#define _PDFPUBLISH_COMMON_INCLUDED_

#include "OdaCommon.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish {

  /** \details 
  Contains declarations representing the result of the publishing operations.
  Values declared within this enumeration are returned by <link !!OVERLOADED_Methods_exportPdf_OdPdfPublish::OdFile, exportPdf methods> 
  of the <link OdPdfPublish::OdFile,OdFile> class.
  */
  enum PdfPublishResult
  {
    eOk                       = 0,          /// Operation has been successfully finished.
    eInternalError            = 1,          /// An internal error occurred during the operation.
    eEmptyInputArray          = 2,          /// The input array is empty.
    eNotSortedArray           = 3,          /// The input arrya is not sorted.
    eDuplicatedElement        = 4,          /// A duplication of elements found.
    eNullDocument             = 5,          /// A try to proceed an operation with a null smart pointer to the Teigha Publish document.
    eKeyNotFound              = 6,          /// The specified key value is not found.
    eNullBaseName             = 7,          /// The name of the base object is null.
    eNotImplementedYet        = 8,          /// The feature currently is not implemented.
    exOdError                 = 0x00020000, /// ODA base class error occured during the operation.
    //DOM-IGNORE-BEGIN
    eLastErrorNum             = 0xFFFF  /// For internal use only.
    //DOM-IGNORE-END
  };

  /** \details 
  Contains declarations related with activation of an <link OdPdfPublish::OdAnnotation, annotation>.
  */
  namespace Activation {
    
    /** \details 
    Determines how an <link OdPdfPublish::OdAnnotation, annotation> can be activated.
    */
    enum When {
      kExplicit,  /// Annotation stays inactive until it is directly activated by a user or a script.
      kOpened,    /// Annotation is activated when its page is opened.
      kVisible    /// Annotation is activated when its page becomes visible.
    };
  }

  /** \details 
  Contains declarations related with deactivation of an <link OdPdfPublish::OdAnnotation, annotation>.
  */
  namespace Deactivation {
    
    /** \details 
    Determines how an <link OdPdfPublish::OdAnnotation, annotation> can be deactivated.
    */
    enum When {
      kExplicit,    /// Annotation stays active until it is directly deactivated by a user or a script.
      kClosed,      /// Annotation is deactivated when its page is closed.
      kNotVisible   /// Annotation is deactivated when its page becomes invisible.
    };
  }

  /** \details 
  Contains page settings for Teigha Publish documents.
  */
  namespace Page {
    
    /** \details 
    Page formats (sizes), supported by Teigha Publish. 
    */
    enum Format {
      kA4 = 0,  /// Format A4 (842 points width, 1190 points height).
      kA3,      /// Format A3 (595 points width, 842 points height).
      kP11x17, 
      kA5, 
      kB4JIS, 
      kB5JIS, 
      kExecutive, 
      kLegal, 
      kLetter, 
      kTabloid, 
      kB4ISO, 
      kB5ISO, 
      kCustom,
      //DOM-IGNORE-BEGIN
      kLastFormat /// For internal use only.
      //DOM-IGNORE-END
    };

    /** \details 
    Page orientations, supported by Teigha Publish. 
    */
    enum Orientation {
      kPortrait = 0,  /// Portrait orientation.
      kLandscape,     /// Landscape orientation.

      //DOM-IGNORE-BEGIN
      kLastOrientation /// For internal use only.
      //DOM-IGNORE-END
    };

    enum PaperUnits
    {
      kInches         = 0,   
      kMillimeters    = 1,   
      kPixels         = 2 
    };
  }

  /** \details 
  Contains camera settings for Teigha Publish documents.
  */
  namespace Camera {
    
    /** \details 
    Camera projection types supported by Teigha Publish. 
    */
    enum Projection {
      kOrthographic = 0,  /// Orthographic camera projection.
      kPerspective        /// Perspective camera projection.
    };
  }

  /** \details 
  Contains light settings for Teigha Publish documents.
  */
  namespace Lighting
  {
  
    /** \details 
    Light modes supported by Teigha Publish. 
    */
    enum Mode {
      kDefault = 0,   /// Default light mode. Currently is not applied.
      kArtwork,       /// Light mode is inherited from the annotation.
      kNone,          /// No light mode is used.
      kWhite,         /// White light mode is used.
      kDay,           /// Day light mode is used.
      kBright,        /// Bright light mode is used.
      kPrimaryColor,  /// Primary color light mode is used.
      kNight,         /// Night light mode is used.
      kBlue,          /// Blue light mode is used.
      kRed,           /// Red light mode is used.
      kCube,          /// Cube light mode is used.
      kCADOptimized,  /// Optimized light from CAD is used.
      kHeadlamp       /// Head lamp light mode is used.
    };
  }
  
  /** \details 
  Contains rendering settings for Teigha Publish documents.
  */
  namespace Rendering
  {
    /** \details 
    Rendering modes supported by Teigha Publish. 
    */
    enum Mode {
      kDefault = 0,                   /// Default rendering mode. Currently is not applied.
      kSolid,                         /// Textured and lighted faces are rendered.
      kSolidWireframe,                /// Textured, lighted faces and monochrome tessellation edges are rendered.
      kSolidOutline,                  /// Textured, lighted faces and monochrome silhouette edges are rendered.
      kBoundingBox,                   /// Monochrome bounding box edges are rendered.
      kTransparent,                   /// Textured and lighted transparent faces are rendered.
      kTransparentWireframe,          /// Textured, lighted transparent faces and monochrome tessellation edges are rendered.
      kTransparentBoundingBox,        /// Monochrome bounding box transparent faces are rendered.
      kTransparentBoundingBoxOutline, /// Monochrome edges and bounding box transparent faces are rendered. 
      kIllustration,                  /// Monochrome unlighted faces and silhouette edges are rendered. 
      kShadedIllustration,            /// Textured, lighted highly emissive faces and monochrome silhouette edges are rendered.
      kWireframe,                     /// Monochrome tessellation edges are rendered. 
      kShadedWireframe,               /// Interpolated and lighted tessellation edges are rendered.
      kHiddenWireframe,               /// Monochrome faces and tessellation edges are rendered. 
      kVertices,                      /// Monochrome vertices are rendered.
      kShadedVertices                 /// Colored and lighted vertices are rendered.
    };
  }

    /** \details 
     Contains declarations for .prc format support in Teigha Publish. 
    */
   namespace PRC {
   
    /** \details 
    Boundary representation compression levels. 
    */
     enum BRepCompression {
       kNone = 0,   /// Use uncompressed BRep data.
       kLow,        /// Use BRep data with low compression level.
       kMedium,     /// Use BRep data with medium compression level.
       kHigh        /// Use BRep data with high compression level.
     };
   }

    /** \details 
    Contains declarations for user's <link OdPdfPublish::OdArtwork, artworks> support in Teigha Publish. 
    */
  namespace Artwork {
  
    /** \details 
    An <link OdPdfPublish::OdArtwork, artwork> object animation styles. 
    */
    enum AnimationStyle {
      kNone,  /// No animation style. Application allows other ways of animation control, for example through JavaScript scenario.
      kLoop,  /// Animation loop. Application plays the animation in infinite loop.
      kBounce /// Animation forward and backward loop. In each loop iteration animation is played firstly  forward, then backward.
    };
    
    /** \details 
    An <link OdPdfPublish::OdArtwork, artwork> object default view option. 
    */
    enum ViewPreference {
      kNative,  /// Use default view from .prc data (if .prc data exists).
      kPDF      /// Use default view from .pdf data (if .pdf data exists).
    };
    
    /** \details 
    Options (such as background color, lighting and rendering modes) used for displaying views. 
    */
    enum DisplayPreference {
      kDefined, /// Use a view settings.
      kRuntime  /// Use application display runtime settings. In this mode settings are not applied for the default view.
    };
  }

    /** \details 
    Contains declarations for border options in Teigha Publish. 
    */
  namespace Border {
  
    /** \details 
    Border line thickness types. 
    */
    enum Thickness {
      kThin,    /// Border line is thin.
      kMedium,  /// Border line has medium thickness.
      kWide     /// Border line is wide.
    };
    
    /** \details 
    Border line styles. 
    */
    enum Style {
      kSolid,         /// The border is solid.
      kDashed,        /// The border is dashed.
      kBeveled,       /// The border is bevelled.
      kInset,         /// The border is inset.
      kUnderlined,    /// The border is underlined.
    };
  }

    /** \details 
    Contains declarations for text entity options in Teigha Publish. 
    */
  namespace Text {
  
    /** \details 
    Font styles. 
    */
    enum FontStyle {
      kRegular,     /// Normal font style. 
      kBold,        /// Text appears as bold. 
      kItalic,      /// Text appears as italic. 
      kBoldItalic   /// Text appears as bold and italic. 
    };
    
    /** \details 
    Text justification values. 
    */
    enum Justification {
      kLeft,    /// Left text justification.
      kRight,   /// Right text justification.
      kCenter   /// Center text justification.
    };
    
    /** \details 
    Text rotation options. 
    */
    enum Rotation {
      kNone,                        /// Horizontally allocated text, no rotation.
      kCounterClockwise90Degrees,   /// Counter-clockwise rotation for 90 degrees relative from horizontal position.
      kCounterClockwise180Degrees,  /// Counter-clockwise rotation for 180 degrees relative from horizontal position.
      kCounterClockwise270Degrees   /// Counter-clockwise rotation for 270 degrees relative from horizontal position.
    };
    
    /** \details 
    Built-in fonts supported by Teigha Publish. 
    */
    enum StandardFontsType 
    {
      kTimesRoman,            /// Normal Times Roman font.
      kHelvetica,             /// Normal Helvetica font.
      kCourier,               /// Normal Courier font.
      kSymbol,                /// Normal Symbol font.
      kTimesBold,             /// Bold Times Roman font.
      kHelveticaBold,         /// Bold Helvetica font.
      kCourierBold,           /// Bold Courier font.
      kZapfDingbats,          /// Normal ZapfDingbats font.
      kTimesItalic,           /// Italic Times Roman font.
      kHelveticaOblique,      /// Oblique Helvetica font.
      kCourierOblique,        /// Oblique Courier font.
      kTimesBoldItalic,       /// Bold and italic combination of the Times Roman font.
      kHelveticaBoldOblique,  /// Bold and oblique of the Helvetica font.
      kCourierBoldOblique     /// Bold and oblique of the Courier font.
    };
    enum StorageType
    {
      kBuiltIn,
      kExplicit
    };
    enum Language
    {
      kASCII,
      kEastEuropeanRoman,
      kCyrillic,
      kGreek,
      kTurkish,
      kHebrew,
      kArabic,
      kBaltic,
      kChineseTraditional,
      kChineseSimplified,
      kJapanese,
      kKorean,
      kDefault
    };
  }

  /** \details 
  Contains declarations for <link OdPdfPublish::OdText, label> behaviour options in Teigha Publish. 
  */
  namespace Label {
    
    /** \details 
    <link OdPdfPublish::OdText, Label> positions relative to the icon.
    */
    enum Position
    {
      kLabelOnly, /// Only label is displayed.
      kIconOnly,  /// Only label icon is displayed.
      kTop,       /// The label is positioned at the top of the icon associated with it (i.e above the icon).
      kBottom,    /// The label is positioned at the bottom of icon associated with it (i.e under the icon).
      kLeft,      /// The label is positioned at the left edge of the icon associated with it.
      kRight,     /// The label is positioned at the right edge of the icon associated with it.
      kOnTop      /// The label is positioned on the right edge of the icon associated with it.
    };
  }

  /** \details 
  Contains declarations for highlighting options in Teigha Publish. 
  */
  namespace Highlighting {
    
    /** \details 
    Highlighting modes, which can be applied to Teigha Publish objects. 
    Highlighting mode determines how a control appears after a user clicks it.
    */
    enum Mode
    {
      kNone,    /// No highlighting.
      kInvert,  /// Inverts highlighting colors within the control.
      kOutline, /// Creates an outline border around the control.
      kPush     /// Content area appears on the page.
    };
  }

  /** \details 
  Contains declarations for image options in Teigha Publish. 
  */
  namespace Image {
    
    /** \details 
    Image formats supported by Teigha Publish. 
    */
    enum Format
    {

      kUnknown  = -1, /// The format is unknown.
      kBMP,           /// A bitmap image.
      kICO,           /// An icon image.
      kJPEG,          /// A .jpeg image.
      kJNG,           /// A .jng image.
      kKOALA,         /// A .koala image.
      kLBM,           /// A .lbm image.
      kIFF,           /// An .iff image.
      kMNG,           /// A .mgn image.
      kPBM,           /// A .pbm image.
      kPBMRAW,        /// A raw .pbm image.
      kPCD,           /// A .pcd image.
      kPCX,           /// A .pcx image.
      kPGM,           /// A .pgm image.
      kPGMRAW,        /// A raw .pgm image.
      kPNG,           /// A .png image.
      kPPM,           /// A .ppm image.
      kPPMRAW,        /// A raw .ppm image.
      kRAS,           /// A .ras image.
      kTARGA,         /// A .targa image.
      kTIFF,          /// A .tiff image.
      kWBMP,          /// A .wbmp image.
      kPSD,           /// A .psd (Adobe Photoshop) image.
      kCUT,           /// A .cut image.
      kXBM,           /// A .xbm image.
      kXPM,           /// A .xpm image.
      kDDS,           /// A .dds image.
      kGIF,           /// A .gif image.
      kHDR,           /// A .hdr image.
      kFAXG3,         /// A .faxg3 image.
      kSGI,           /// A .sgi image.
      kEXR,           /// A .exr image.
      kJ2K,           /// A .j2k image.
      kJP2,           /// A .jp2 image.
      kPFM,           /// A .pfm image.
      kPICT,          /// A .pict image.
      kRAW            /// A raw image.
    };
  }

  namespace Action {
    
    enum Type
    {
      kCursorEnter    = 0,    
      kCursorExit     = 1,  
      kButtonPressed  = 2, 
      kButtonReleased = 3,
      kInputFocus     = 4,
      kLoseFocus      = 5,
      kPageOpened     = 6,
      kPageClosed     = 7,
      kPageVisible    = 8,
      kPageInvisible  = 9
    };
  }

  namespace Geometry {
    enum PDFLineCap
    {
      kLineCapNotSet = -1,
      kButtCap = 0,
      kRoundCap = 1,
      kProjectingSquareCap = 2
    };

    enum PDFLineJoin
    {
      kLineJoinNotSet = -1,
      kMiterJoin = 0,
      kRoundJoin = 1,
      kBevelJoin = 2
    };

    enum PDFFinishRule
    {
      kFillNotSet  = 0,
      kEnd         = 1, // End the path
      kStroke      = 2, // Stroke the path
      kClose       = 4, // Close the path
      kFillEvenOdd = 8, // Fill the path, using the even-odd rule to determine the region to fill
      kFillNonZero = 16 // Fill the path, using the nonzero winding number rule to determine the region to fill. Any subpaths that are open are implicitly closed before being filled.
    };
  }

  namespace Source {
  
    enum Type
    {
      kCode     = 0,
      kFileName = 1
    };
  }

  namespace CAD {

    enum ColorPolicy
    {
      kNoPolicy = 0,  // No color policy. A drawing is exported with its native color palette (as is).
      kMono = 1,      // Drawing is exported to a monochrome .pdf file.
      kGrayscale = 2  // Drawing is exported to a grayscale .pdf file. Native colors are converted into grayscale
    };
  }
}

#endif // _PDFPUBLISH_COMMON_INCLUDED_


