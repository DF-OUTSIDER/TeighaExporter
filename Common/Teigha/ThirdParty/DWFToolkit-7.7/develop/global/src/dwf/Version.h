//
//  Copyright (c) 1996-2008 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFTK_VERSION_H
#define _DWFTK_VERSION_H

//DNT_Start

///
///\file        dwf/Version.h
///\brief       This file contains DWF file format version information and macros 
///             for use in toolkit client code.
///



//
// IMPORTANT: Toolkit client code should NOT use these internal macros.
//
// Doxygen should skip these.
//

#define _DWF_FORMAT_VERSION_MAJOR                   6       // Internal Use Only
#define _DWF_FORMAT_VERSION_MINOR                  21       // Internal Use Only
#define _DWF_FORMAT_VERSION_EXTRA                   0       // Internal Use Only -- Reserved



///
///\defgroup    DWFFileFormat       DWF File Format
///             These macros are available for referencing the current and
///             historical revisions of the DWF File Format.
///
///\note        The DWF File Format is different from the DWF Toolkit version.
///

///
///\ingroup     DWFFileFormat
///\brief       The current major revision of the DWF File Format.
///
///             This macro should be used by client code wishing to reference
///             the current major version of the DWF file format.
///
///             A major version change indicates a significant structural
///             and or content shift in the file.  Generally speaking,
///             while the toolkit will always attempt to provide as much
///             backwards compatability as possible, it may do so, in order
///             to account for such changes, with a new collection of data and API
///             objects.  A major version change will almost definitely result in
///             a port of the toolkit client code.
///
#define _DWF_FORMAT_VERSION_CURRENT_MAJOR           _DWF_FORMAT_VERSION_MAJOR
///
///\ingroup     DWFFileFormat
///\brief       The current minor revision of the DWF File Format.
///
///             This macro should be used by client code wishing to reference
///             the current minor version of the DWF file format.
///
///             A minor version change usually indicates that some content or structure
///             was adding or modified in a way that would not impair existing DWF processors.
///             Extensions or additions to schemas, introductions of new section types and
///             document and elements are all examples of minor revision triggers.
///             
///             Toolkit client code should always perform a minor version check to 
///             ensure 100% compatability and in the case that an increment occurs
///             at least notify the end user that some new data may exist that the
///             current product may not be able to handle/display/etc.
///
#define _DWF_FORMAT_VERSION_CURRENT_MINOR           _DWF_FORMAT_VERSION_MINOR
///
///\ingroup     DWFFileFormat
///\brief       The current DWF File Format version number as a composite integer value.
///
///             The current value of this macro is <b>620</b>.
///
#define _DWF_FORMAT_VERSION_CURRENT                 ((_DWF_FORMAT_VERSION_CURRENT_MAJOR*100) + _DWF_FORMAT_VERSION_CURRENT_MINOR)
///
///\ingroup     DWFFileFormat
///\brief       The current DWF File Format version number as a composite floating point value.
///
///             The current value of this macro is <b>6.20</b>.
///
#define _DWF_FORMAT_VERSION_CURRENT_FLOAT           (float)(_DWF_FORMAT_VERSION_CURRENT_MAJOR + (_DWF_FORMAT_VERSION_CURRENT_MINOR/100.0))
///
///\ingroup     DWFFileFormat
///\brief       The current DWF File Format version number as an ASCII character string.
///
#define _DWF_FORMAT_VERSION_CURRENT_STRING          "6.20"
///
///\ingroup     DWFFileFormat
///\brief       The current DWF File Format version number as a wide character string.
///
#define _DWF_FORMAT_VERSION_CURRENT_WIDE_STRING     L"6.20"


///
///\defgroup    DWFFileFormatEModelMarkup     EModel Markup Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the version
///             of the file format that introduced dwfs that contain
///             graphical markup of EModel sections.
///             

///
///\ingroup     DWFFileFormatEModelMarkup
///\brief       The major revision of the DWF File Format introducing EModel Markup.
///
#define _DWF_FORMAT_VERSION_EMODEL_MARKUP_MAJOR          6
///
///\ingroup     DWFFileFormatEModelMarkup
///\brief       The minor revision of the DWF File Format introducing EModel Markup.
///
#define _DWF_FORMAT_VERSION_EMODEL_MARKUP_MINOR          21
///
///\ingroup     DWFFileFormatEModelMarkup
///\brief       The version number of the DWF File Format introducing EModel Markup as a composite integer value.
///
///             The value of this macro is <b>621</b>.
///
#define _DWF_FORMAT_VERSION_EMODEL_MARKUP                ((_DWF_FORMAT_VERSION_EMODEL_MARKUP_MAJOR*100) + _DWF_FORMAT_VERSION_EMODEL_MARKUP_MINOR)
///
///\ingroup     DWFFileFormatEModelMarkup
///\brief       The version number of the DWF File Format introducing EModel Markup as a composite floating point value.
///
///             The value of this macro is <b>6.21</b>.
///
#define _DWF_FORMAT_VERSION_EMODEL_MARKUP_FLOAT          (float)(_DWF_FORMAT_VERSION_EMODEL_MARKUP_MAJOR + (_DWF_FORMAT_VERSION_EMODEL_MARKUP_MINOR/100.0))
///
///\ingroup     DWFFileFormatEModelMarkup
///\brief       The version number of the DWF File Format introducing EModel Markup as an ASCII character string.
///
#define _DWF_FORMAT_VERSION_EMODEL_MARKUP_STRING         "6.21"
///
///\ingroup     DWFFileFormatEModelMarkup
///\brief       The version number of the DWF File Format introducing EModel Markup as a wide character string.
///
#define _DWF_FORMAT_VERSION_EMODEL_MARKUP_WIDE_STRING    L"6.21"



///
///\defgroup    DWFFileFormatCD     Content Definition Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the version
///             of the file format that introduced dwfs that contain content
///             definition information.
///             

///
///\ingroup     DWFFileFormatCD
///\brief       The major revision of the DWF File Format introducing content definition.
///
#define _DWF_FORMAT_VERSION_CONTENT_DEFINITION_MAJOR          6
///
///\ingroup     DWFFileFormatCD
///\brief       The minor revision of the DWF File Format introducing content definition.
///
#define _DWF_FORMAT_VERSION_CONTENT_DEFINITION_MINOR          20
///
///\ingroup     DWFFileFormatCD
///\brief       The version number of the DWF File Format introducing content definition as a composite integer value.
///
///             The value of this macro is <b>620</b>.
///
#define _DWF_FORMAT_VERSION_CONTENT_DEFINITION                ((_DWF_FORMAT_VERSION_CONTENT_DEFINITION_MAJOR*100) + _DWF_FORMAT_VERSION_CONTENT_DEFINITION_MINOR)
///
///\ingroup     DWFFileFormatCD
///\brief       The version number of the DWF File Format introducing content definition as a composite floating point value.
///
///             The value of this macro is <b>6.20</b>.
///
#define _DWF_FORMAT_VERSION_CONTENT_DEFINITION_FLOAT          (float)(_DWF_FORMAT_VERSION_CONTENT_DEFINITION_MAJOR + (_DWF_FORMAT_VERSION_CONTENT_DEFINITION_MINOR/100.0))
///
///\ingroup     DWFFileFormatCD
///\brief       The version number of the DWF File Format introducing content definition as an ASCII character string.
///
#define _DWF_FORMAT_VERSION_CONTENT_DEFINITION_STRING         "6.20"
///
///\ingroup     DWFFileFormatCD
///\brief       The version number of the DWF File Format introducing content definition as a wide character string.
///
#define _DWF_FORMAT_VERSION_CONTENT_DEFINITION_WIDE_STRING    L"6.20"


///
///\defgroup    DWFFileFormatEComposite     EComposite Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the version
///             of the file format that introduced dwfs that contain both ePlot 
///             and eModel sections
///             

///
///\ingroup     DWFFileFormatEComposite
///\brief       The major revision of the DWF File Format introducing eComposite dwfs.
///
#define _DWF_FORMAT_VERSION_ECOMPOSITE_MAJOR          6
///
///\ingroup     DWFFileFormatEComposite
///\brief       The minor revision of the DWF File Format introducing eComposite dwfs.
///
#define _DWF_FORMAT_VERSION_ECOMPOSITE_MINOR          11
///
///\ingroup     DWFFileFormatEComposite
///\brief       The version number of the DWF File Format introducing eComposite dwfs as a composite integer value.
///
///             The value of this macro is <b>611</b>.
///
#define _DWF_FORMAT_VERSION_ECOMPOSITE                ((_DWF_FORMAT_VERSION_ECOMPOSITE_MAJOR*100) + _DWF_FORMAT_VERSION_ECOMPOSITE_MINOR)
///
///\ingroup     DWFFileFormatEComposite
///\brief       The version number of the DWF File Format introducing eComposite dwfs as a composite floating point value.
///
///             The value of this macro is <b>6.11</b>.
///
#define _DWF_FORMAT_VERSION_ECOMPOSITE_FLOAT          (float)(_DWF_FORMAT_VERSION_ECOMPOSITE_MAJOR + (_DWF_FORMAT_VERSION_ECOMPOSITE_MINOR/100.0))
///
///\ingroup     DWFFileFormatEComposite
///\brief       The version number of the DWF File Format introducing eComposite dwfs as an ASCII character string.
///
#define _DWF_FORMAT_VERSION_ECOMPOSITE_STRING         "6.11"
///
///\ingroup     DWFFileFormatEComposite
///\brief       The version number of the DWF File Format introducing eComposite dwfs as a wide character string.
///
#define _DWF_FORMAT_VERSION_ECOMPOSITE_WIDE_STRING    L"6.11"


///
///\defgroup    DWFFileFormatTI     TYPEINFO Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the version
///             of the file format that introduced the TYPEINFO file.
///             
///             The TYPEINFO file must be placed as the very first file
///             in the DWF package archive.  It's presence can be detected
///             in the binary stream directly after the header for immediate
///             recognition and processing.  It must be named the same as the
///             DWFInterface identifier that it represents.
///

///
///\ingroup     DWFFileFormatTI
///\brief       The major revision of the DWF File Format introducing the TYPEINFO file.
///
#define _DWF_FORMAT_VERSION_TYPEINFO_MAJOR          6
///
///\ingroup     DWFFileFormatTI
///\brief       The minor revision of the DWF File Format introducing the TYPEINFO file.
///
#define _DWF_FORMAT_VERSION_TYPEINFO_MINOR          1
///
///\ingroup     DWFFileFormatTI
///\brief       The version number of the DWF File Format introducing the TYPEINFO file as a composite integer value.
///
///             The value of this macro is <b>601</b>.
///
#define _DWF_FORMAT_VERSION_TYPEINFO                ((_DWF_FORMAT_VERSION_TYPEINFO_MAJOR*100) + _DWF_FORMAT_VERSION_TYPEINFO_MINOR)
///
///\ingroup     DWFFileFormatTI
///\brief       The version number of the DWF File Format introducing the TYPEINFO file as a composite floating point value.
///
///             The value of this macro is <b>6.01</b>.
///
#define _DWF_FORMAT_VERSION_TYPEINFO_FLOAT          (float)(_DWF_FORMAT_VERSION_TYPEINFO_MAJOR + (_DWF_FORMAT_VERSION_TYPEINFO_MINOR/100.0))
///
///\ingroup     DWFFileFormatTI
///\brief       The version number of the DWF File Format introducing the TYPEINFO file as an ASCII character string.
///
#define _DWF_FORMAT_VERSION_TYPEINFO_STRING         "6.01"
///
///\ingroup     DWFFileFormatTI
///\brief       The version number of the DWF File Format introducing the TYPEINFO file as a wide character string.
///
#define _DWF_FORMAT_VERSION_TYPEINFO_WIDE_STRING    L"6.01"


///
///\defgroup    DWFFileFormatPkg    Package Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the version
///             of the file format that introduced the zip package file format.
///             

///
///\ingroup     DWFFileFormatPkg
///\brief       The major revision of the DWF File Format introducing the zip package structure.
///
#define _DWF_FORMAT_VERSION_PACKAGE_MAJOR           6
///
///\ingroup     DWFFileFormatPkg
///\brief       The minor revision of the DWF File Format introducing the zip package structure.
///
#define _DWF_FORMAT_VERSION_PACKAGE_MINOR           0
///
///\ingroup     DWFFileFormatPkg
///\brief       The version number of the DWF File Format introducing the zip package structure as a composite integer value.
///
///             The value of this macro is <b>600</b>.
///
#define _DWF_FORMAT_VERSION_PACKAGE                 ((_DWF_FORMAT_VERSION_PACKAGE_MAJOR*100) + _DWF_FORMAT_VERSION_PACKAGE_MINOR)
///
///\ingroup     DWFFileFormatPkg
///\brief       The version number of the DWF File Format introducing the zip package structure as a composite floating point value.
///
///             The value of this macro is <b>6.0</b>.
///
#define _DWF_FORMAT_VERSION_PACKAGE_FLOAT           (float)(_DWF_FORMAT_VERSION_PACKAGE_MAJOR + (_DWF_FORMAT_VERSION_PACKAGE_MINOR/100.0))
///
///\ingroup     DWFFileFormatPkg
///\brief       The version number of the DWF File Format introducing the zip package structure as an ASCII character string.
///
#define _DWF_FORMAT_VERSION_PACKAGE_STRING          "6.0"
///
///\ingroup     DWFFileFormatPkg
///\brief       The version number of the DWF File Format introducing the zip package structure as a wide character string.
///
#define _DWF_FORMAT_VERSION_PACKAGE_WIDE_STRING     L"6.0"

///
///\defgroup    DWFFileFormatAlias  Version Aliases
///\ingroup     DWFFileFormat
///             These macros are available for referencing versions
///             of the file format that coincide with other introductory
///             features.
///             

///
///\ingroup     DWFFileFormatAlias
///\brief       The version number of the DWF File Format introducing OpenHSF animations.
///
#define _DWF_FORMAT_VERSION_INTRO_FEATURE_CONTROL               _DWF_FORMAT_VERSION_CONTENT_DEFINITION

///
///\ingroup     DWFFileFormatAlias
///\brief       The version number of the DWF File Format introducing strong passwords.
///
#define _DWF_FORMAT_VERSION_INTRO_STRONG_PASSWORDS              _DWF_FORMAT_VERSION_CONTENT_DEFINITION

///
///\ingroup     DWFFileFormatAlias
///\brief       The version number of the DWF File Format introducing OpenHSF animations.
///
#define _DWF_FORMAT_VERSION_INTRO_OPENHSF_ANIMATIONS            _DWF_FORMAT_VERSION_CONTENT_DEFINITION

///
///\ingroup     DWFFileFormatAlias
///\brief       The version number of the DWF File Format introducing content presentation.
///
#define _DWF_FORMAT_VERSION_INTRO_CONTENT_PRESENTATION          _DWF_FORMAT_VERSION_CONTENT_DEFINITION

///
///\ingroup     DWFFileFormatAlias
///\brief       The version number of the DWF File Format introducing 3D graphics streams.
///
#define _DWF_FORMAT_VERSION_INTRO_3D                            _DWF_FORMAT_VERSION_TYPEINFO



///
///\defgroup    DWFManifestFormat   Package Manifest Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the manifest document.
///  
    
///
///\ingroup     DWFManifestFormat
///\brief       The major revision of the DWF package manifest document schema.
///
#define _DWF_FORMAT_MANIFEST_VERSION_MAJOR              6
///
///\ingroup     DWFManifestFormat
///\brief       The minor revision of the DWF package manifest document schema.
///
#define _DWF_FORMAT_MANIFEST_VERSION_MINOR              0
///
///\ingroup     DWFManifestFormat
///\brief       The current version of the DWF package manifest document schema as a composite integer value.
///
///             The current value of this macro is <b>600</b>.
///
#define _DWF_FORMAT_MANIFEST_VERSION_CURRENT            ((_DWF_FORMAT_MANIFEST_VERSION_MAJOR*100) + _DWF_FORMAT_MANIFEST_VERSION_MINOR)
///
///\ingroup     DWFManifestFormat
///\brief       The current version of the DWF package manifest document schema as a composite floating point value.
///
///             The current value of this macro is <b>6.0</b>.
///
#define _DWF_FORMAT_MANIFEST_VERSION_CURRENT_FLOAT      (float)(_DWF_FORMAT_MANIFEST_VERSION_MAJOR + (_DWF_FORMAT_MANIFEST_VERSION_MINOR/100.0))
///
///\ingroup     DWFManifestFormat
///\brief       The current version of the DWF package manifest document schema as an ASCII character string.
///
#define _DWF_FORMAT_MANIFEST_VERSION_CURRENT_STRING     "6.0"

///
///\defgroup    DWFEPlotFormat      2D EPlot Section Descriptor Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the EPlot section descriptor documents.
/// 

///
///\ingroup     DWFEPlotFormat
///\brief       The major revision of the EPlot section descriptor document schema.
///
#define _DWF_FORMAT_EPLOT_VERSION_MAJOR                 1
///
///\ingroup     DWFEPlotFormat
///\brief       The minor revision of the EPlot section descriptor document schema.
///
#define _DWF_FORMAT_EPLOT_VERSION_MINOR                 21
///
///\ingroup     DWFEPlotFormat
///\brief       The current version of the EPlot section descriptor document schema as a composite integer value.
///
///             The current value of this macro is <b>121</b>.
///
#define _DWF_FORMAT_EPLOT_VERSION_CURRENT               ((_DWF_FORMAT_EPLOT_VERSION_MAJOR*100) + _DWF_FORMAT_EPLOT_VERSION_MINOR)
///
///\ingroup     DWFEPlotFormat
///\brief       The current version of the EPlot section descriptor document schema as a composite floating point value.
///
///             The current value of this macro is <b>1.21</b>.
///
#define _DWF_FORMAT_EPLOT_VERSION_CURRENT_FLOAT         (float)(_DWF_FORMAT_EPLOT_VERSION_MAJOR + (_DWF_FORMAT_EPLOT_VERSION_MINOR/100.0))
///
///\ingroup     DWFEPlotFormat
///\brief       The current version of the EPlot section descriptor document schema as an ASCII character string.
///
#define _DWF_FORMAT_EPLOT_VERSION_CURRENT_STRING        "1.21"
///
///\ingroup     DWFEPlotFormat
///\brief       The EPlot section type as an ASCII character string.
///
#define _DWF_FORMAT_EPLOT_TYPE_STRING                   "com.autodesk.dwf.ePlot"
///
///\ingroup     DWFEPlotFormat
///\brief       The EPlot section type as a wide character string.
///
#define _DWF_FORMAT_EPLOT_TYPE_WIDE_STRING             L"com.autodesk.dwf.ePlot"
///
///\ingroup     DWFEPlotFormat
///\brief       The Global EPlot section type as an ASCII character string.
///
#define _DWF_FORMAT_EPLOT_GLOBAL_TYPE_STRING            "com.autodesk.dwf.ePlotGlobal"
///
///\ingroup     DWFEPlotFormat
///\brief       The Global EPlot section type as a wide character string.
///
#define _DWF_FORMAT_EPLOT_GLOBAL_TYPE_WIDE_STRING      L"com.autodesk.dwf.ePlotGlobal"

///
///\defgroup    DWFEModelFormat      3D EModel Section Descriptor Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the EModel section descriptor documents.
/// 

///
///\ingroup     DWFEModelFormat
///\brief       The major revision of the EModel section descriptor document schema.
///
#define _DWF_FORMAT_EMODEL_VERSION_MAJOR                1
///
///\ingroup     DWFEModelFormat
///\brief       The minor revision of the EModel section descriptor document schema.
///
#define _DWF_FORMAT_EMODEL_VERSION_MINOR                1
///
///\ingroup     DWFEModelFormat
///\brief       The current version of the EModel section descriptor document schema as a composite integer value.
///
///             The current value of this macro is <b>101</b>.
///
#define _DWF_FORMAT_EMODEL_VERSION_CURRENT              ((_DWF_FORMAT_EMODEL_VERSION_MAJOR*100) + _DWF_FORMAT_EMODEL_VERSION_MINOR)
///
///\ingroup     DWFEModelFormat
///\brief       The current version of the EModel section descriptor document schema as a composite floating point value.
///
///             The current value of this macro is <b>1.01</b>.
///
#define _DWF_FORMAT_EMODEL_VERSION_CURRENT_FLOAT        (float)(_DWF_FORMAT_EMODEL_VERSION_MAJOR + (_DWF_FORMAT_EMODEL_VERSION_MINOR/100.0))
///
///\ingroup     DWFEModelFormat
///\brief       The current version of the EModel section descriptor document schema as an ASCII character string.
///
#define _DWF_FORMAT_EMODEL_VERSION_CURRENT_STRING       "1.01"
///
///\ingroup     DWFEModelFormat
///\brief       The EModel section type as an ASCII character string.
///
#define _DWF_FORMAT_EMODEL_TYPE_STRING                  "com.autodesk.dwf.eModel"
///
///\ingroup     DWFEModelFormat
///\brief       The EModel section type as a wide character string.
///
#define _DWF_FORMAT_EMODEL_TYPE_WIDE_STRING            L"com.autodesk.dwf.eModel"
///
///\ingroup     DWFEModelFormat
///\brief       The Global EModel section type as an ASCII character string.
///
#define _DWF_FORMAT_EMODEL_GLOBAL_TYPE_STRING           "com.autodesk.dwf.eModelGlobal"
///
///\ingroup     DWFEModelFormat
///\brief       The Global EModel section type as a wide character string.
///
#define _DWF_FORMAT_EMODEL_GLOBAL_TYPE_WIDE_STRING     L"com.autodesk.dwf.eModelGlobal"

///
///\defgroup    DWFDataFormat      Data Section Descriptor Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the data section descriptor documents.
/// 

///
///\ingroup     DWFDataFormat
///\brief       The major revision of the data section descriptor document schema.
///
#define _DWF_FORMAT_DATA_VERSION_MAJOR                1
///
///\ingroup     DWFDataFormat
///\brief       The minor revision of the data section descriptor document schema.
///
#define _DWF_FORMAT_DATA_VERSION_MINOR                0
///
///\ingroup     DWFDataFormat
///\brief       The current version of the data section descriptor document schema as a composite integer value.
///
///             The current value of this macro is <b>100</b>.
///
#define _DWF_FORMAT_DATA_VERSION_CURRENT              ((_DWF_FORMAT_DATA_VERSION_MAJOR*100) + _DWF_FORMAT_DATA_VERSION_MINOR)
///
///\ingroup     DWFDataFormat
///\brief       The current version of the Data section descriptor document schema as a composite floating point value.
///
///             The current value of this macro is <b>1.0</b>.
///
#define _DWF_FORMAT_DATA_VERSION_CURRENT_FLOAT        (float)(_DWF_FORMAT_DATA_VERSION_MAJOR + (_DWF_FORMAT_DATA_VERSION_MINOR/100.0))
///
///\ingroup     DWFDataFormat
///\brief       The current version of the Data section descriptor document schema as an ASCII character string.
///
#define _DWF_FORMAT_DATA_VERSION_CURRENT_STRING       "1.0"
///
///\ingroup     DWFDataFormat
///\brief       The Data section type as an ASCII character string.
///
#define _DWF_FORMAT_DATA_TYPE_STRING                  "com.autodesk.dwf.Data"
///
///\ingroup     DWFDataFormat
///\brief       The Data section type as a wide character string.
///
#define _DWF_FORMAT_DATA_TYPE_WIDE_STRING            L"com.autodesk.dwf.Data"
///
///\ingroup     DWFDataFormat
///\brief       The Global Data section type as an ASCII character string.
///
#define _DWF_FORMAT_DATA_GLOBAL_TYPE_STRING           "com.autodesk.dwf.DataGlobal"
///
///\ingroup     DWFDataFormat
///\brief       The Global Data section type as a wide character string.
///
#define _DWF_FORMAT_DATA_GLOBAL_TYPE_WIDE_STRING     L"com.autodesk.dwf.DataGlobal"


// ...
///
///\defgroup    DWFSignatureFormat      Signature Section Descriptor Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the Signature section descriptor documents.
/// 

///
///\ingroup     DWFSignatureFormat
///\brief       The major revision of the Signature section descriptor document schema.
///
#define _DWF_FORMAT_SIGNATURE_SECTION_VERSION_MAJOR                1
///
///\ingroup     DWFSignatureFormat
///\brief       The minor revision of the Signature section descriptor document schema.
///
#define _DWF_FORMAT_SIGNATURE_SECTION_VERSION_MINOR                0
///
///\ingroup     DWFSignatureFormat
///\brief       The current version of the Signature section descriptor document schema as a composite integer value.
///
///             The current value of this macro is <b>100</b>.
///
#define _DWF_FORMAT_SIGNATURE_SECTION_VERSION_CURRENT              ((_DWF_FORMAT_SIGNATURE_SECTION_VERSION_MAJOR*100) + _DWF_FORMAT_SIGNATURE_SECTION_VERSION_MINOR)
///
///\ingroup     DWFSignatureFormat
///\brief       The current version of the Signature section descriptor document schema as a composite floating point value.
///
///             The current value of this macro is <b>1.0</b>.
///
#define _DWF_FORMAT_SIGNATURE_SECTION_VERSION_CURRENT_FLOAT        (float)(_DWF_FORMAT_SIGNATURE_SECTION_VERSION_MAJOR + (_DWF_FORMAT_SIGNATURE_SECTION_VERSION_MINOR/100.0))
///
///\ingroup     DWFSignatureFormat
///\brief       The current version of the Signature section descriptor document schema as an ASCII character string.
///
#define _DWF_FORMAT_SIGNATURE_SECTION_VERSION_CURRENT_STRING       "1.0"
///
///\ingroup     DWFSignatureFormat
///\brief       The Signature section type as an ASCII character string.
///
#define _DWF_FORMAT_SIGNATURE_SECTION_TYPE_STRING                  "com.autodesk.dwf.signatures"
///
///\ingroup     DWFSignatureFormat
///\brief       The Signature section type as a wide character string.
///
#define _DWF_FORMAT_SIGNATURE_SECTION_TYPE_WIDE_STRING              L"com.autodesk.dwf.signatures"


///
///\defgroup    DWFContentFormat   Package Content Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the content documents.
///  
    
///
///\ingroup     DWFContentFormat
///\brief       The major revision of the content document schema.
///
#define _DWF_FORMAT_CONTENT_VERSION_MAJOR                   1
///
///\ingroup     DWFContentFormat
///\brief       The minor revision of the content document schema.
///
#define _DWF_FORMAT_CONTENT_VERSION_MINOR                   0
///
///\ingroup     DWFContentFormat
///\brief       The current version of the content document document schema as a composite integer value.
///
///             The current value of this macro is <b>100</b>.
///
#define _DWF_FORMAT_CONTENT_VERSION_CURRENT                 ((_DWF_FORMAT_CONTENT_VERSION_MAJOR*100) + _DWF_FORMAT_CONTENT_VERSION_MINOR)
///
///\ingroup     DWFContentFormat
///\brief       The current version of the content document document schema as a composite floating point value.
///
///             The current value of this macro is <b>1.0</b>.
///
#define _DWF_FORMAT_CONTENT_VERSION_CURRENT_FLOAT           (float)(_DWF_FORMAT_CONTENT_VERSION_MAJOR + (_DWF_FORMAT_CONTENT_VERSION_MINOR/100.0))
///
///\ingroup     DWFContentFormat
///\brief       The current version of the content document document schema as an ASCII character string.
///
#define _DWF_FORMAT_CONTENT_VERSION_CURRENT_STRING          "1.0"
///
///\ingroup     DWFContentFormat
///\brief       The content document schema location as an ASCII character string.
///
#define _DWF_FORMAT_CONTENT_SCHEMA_LOCATION_STRING          "http://autodesk.com/global/dwf/content.xsd"
///
///\ingroup     DWFContentFormat
///\brief       The content document schema location as a wide character string.
///
#define _DWF_FORMAT_CONTENT_SCHEMA_LOCATION_WIDE_STRING     L"http://autodesk.com/global/dwf/content.xsd"



///
///\defgroup    DWFSectionContentFormat   Section Content Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the section content documents.
///  
    
///
///\ingroup     DWFSectionContentFormat
///\brief       The major revision of the section content document schema.
///
#define _DWF_FORMAT_SECTION_CONTENT_VERSION_MAJOR                   1
///
///\ingroup     DWFSectionContentFormat
///\brief       The minor revision of the section content document schema.
///
#define _DWF_FORMAT_SECTION_CONTENT_VERSION_MINOR                   0
///
///\ingroup     DWFSectionContentFormat
///\brief       The current version of the section content document document schema as a composite integer value.
///
///             The current value of this macro is <b>100</b>.
///
#define _DWF_FORMAT_SECTION_CONTENT_VERSION_CURRENT                 ((_DWF_FORMAT_SECTION_CONTENT_VERSION_MAJOR*100) + _DWF_FORMAT_SECTION_CONTENT_VERSION_MINOR)
///
///\ingroup     DWFSectionContentFormat
///\brief       The current version of the section content document document schema as a composite floating point value.
///
///             The current value of this macro is <b>1.0</b>.
///
#define _DWF_FORMAT_SECTION_CONTENT_VERSION_CURRENT_FLOAT           (float)(_DWF_FORMAT_SECTION_CONTENT_VERSION_MAJOR + (_DWF_FORMAT_SECTION_CONTENT_VERSION_MINOR/100.0))
///
///\ingroup     DWFSectionContentFormat
///\brief       The current version of the section content document document schema as an ASCII character string.
///
#define _DWF_FORMAT_SECTION_CONTENT_VERSION_CURRENT_STRING          "1.0"
///
///\ingroup     DWFSectionContentFormat
///\brief       The section content document schema location as an ASCII character string.
///
#define _DWF_FORMAT_SECTION_CONTENT_SCHEMA_LOCATION_STRING          "http://autodesk.com/global/dwf/sectioncontent.xsd"
///
///\ingroup     DWFSectionContentFormat
///\brief       The section content document schema location as a wide character string.
///
#define _DWF_FORMAT_SECTION_CONTENT_SCHEMA_LOCATION_WIDE_STRING     L"http://autodesk.com/global/dwf/sectioncontent.xsd"


///
///\defgroup    DWFContentPresentationFormat   Package Content Presentation Schema Version
///\ingroup     DWFFileFormat
///             These macros are available for referencing the current version
///             of the schema for the content presentation documents.
///  
    
///
///\ingroup     DWFContentPresentationFormat
///\brief       The major revision of the content presentation schema.
///
#define _DWF_FORMAT_CONTENT_PRESENTATION_VERSION_MAJOR                   1
///
///\ingroup     DWFContentPresentationFormat
///\brief       The minor revision of the content presentation schema.
///
#define _DWF_FORMAT_CONTENT_PRESENTATION_VERSION_MINOR                   0
///
///\ingroup     DWFContentPresentationFormat
///\brief       The current version of the content presentation document schema as a composite integer value.
///
///             The current value of this macro is <b>100</b>.
///
#define _DWF_FORMAT_CONTENT_PRESENTATION_VERSION_CURRENT                 ((_DWF_FORMAT_CONTENT_PRESENTATION_VERSION_MAJOR*100) + _DWF_FORMAT_CONTENT_PRESENTATION_VERSION_MINOR)
///
///\ingroup     DWFContentPresentationFormat
///\brief       The current version of the content presentation document schema as a composite floating point value.
///
///             The current value of this macro is <b>1.0</b>.
///
#define _DWF_FORMAT_CONTENT_PRESENTATION_VERSION_CURRENT_FLOAT           (float)(_DWF_FORMAT_CONTENT_PRESENTATION_VERSION_MAJOR + (_DWF_FORMAT_CONTENT_PRESENTATION_VERSION_MINOR/100.0))
///
///\ingroup     DWFContentPresentationFormat
///\brief       The current version of the content presentation document schema as an ASCII character string.
///
#define _DWF_FORMAT_CONTENT_PRESENTATION_VERSION_CURRENT_STRING          "1.0"
///
///\ingroup     DWFContentPresentationFormat
///\brief       The content presentation document schema location as an ASCII character string.
///
#define _DWF_FORMAT_CONTENT_PRESENTATION_SCHEMA_LOCATION_STRING          "http://autodesk.com/global/dwf/contentpresentation.xsd"
///
///\ingroup     DWFContentPresentationFormat
///\brief       The content presentation document schema location as a wide character string.
///
#define _DWF_FORMAT_CONTENT_PRESENTATION_SCHEMA_LOCATION_WIDE_STRING     L"http://autodesk.com/global/dwf/contentpresentation.xsd"


///
///\brief       The major release version for the DWF Toolkit library module.
///
///             A major library release will almost always indicate a fundamental programming
///             model modification or change in design.  Major releases are not required to be
///             backwards compatable with previous versions.
///
#define _DWFTK_VERSION_MAJOR                        7
///
///\brief       The minor release version for the DWF Toolkit library module.
///
///             A minor release usually indicates a new development stream that has been
///             branched from a previous release to accomodate addition functionality,
///             feature support, bug fixing and performance optimizations.
///             While minor releases are also not required to
///             maintain interface compatability, it is strongly encouraged.  Ideally,
///             objects and interfaces could be added or extended but never removed or modified
///             between minor revisions.
///
#define _DWFTK_VERSION_MINOR                        7
///
///\brief       The age of the release version for the DWF Toolkit library module.
///
///             This concept is taken from the GNU Libtool version numbering system,
///             where the age of the library interface is incremented as internal
///             bug fixes and improvements are made under the hood, so to speak.
///             The entire interface to the module is frozen as the age increases.
///             Therefore, developers are guaranteed that code written against
///             the first release candidate, for example, version 7.0.1 will compile
///             cleanly against a future release candidate (should one become necessary).
///
#define _DWFTK_VERSION_AGE                          0
///
///\brief       An internal build difference.  
///
///\internal    This value is not required to make sense to anything but the internal build system.
///
#define _DWFTK_VERSION_BUILD                        19

#define _DWFTK_VERSION_ECHO( s )                    #s
#define _DWFTK_VERSION_STRING_( a, b, c, d )        _DWFTK_VERSION_ECHO(a) "." _DWFTK_VERSION_ECHO(b) "." _DWFTK_VERSION_ECHO(c) "." _DWFTK_VERSION_ECHO(d) "\0"
#define _DWFTK_VERSION_STRING                       _DWFTK_VERSION_STRING_( _DWFTK_VERSION_MAJOR, _DWFTK_VERSION_MINOR, _DWFTK_VERSION_AGE, _DWFTK_VERSION_BUILD )


///
///\brief		DWFx Format Version
///
///\internal
///

#define _DWFX_FORMAT_VERSION_MAJOR                   7       // Internal Use Only
#define _DWFX_FORMAT_VERSION_MINOR                   10       // Internal Use Only
#define _DWFX_FORMAT_VERSION_EXTRA                   0       // Internal Use Only -- Reserved



///
///\defgroup    DWFxFileFormat       DWFx File Format
///             These macros are available for referencing the current and
///             historical revisions of the DWFx File Format.
///
///\note        The DWFx File Format is different from the XAML Toolkit version.
///

///
///\ingroup     DWFxFileFormat
///\brief       The current major revision of the DWFx File Format.
///
///             This macro should be used by client code wishing to reference
///             the current major version of the DWFx file format.
///
///             A major version change indicates a significant structural
///             and or content shift in the file.  Generally speaking,
///             while the toolkit will always attempt to provide as much
///             backwards compatability as possible, it may do so, in order
///             to account for such changes, with a new collection of data and API
///             objects.  A major version change will almost definitely result in
///             a port of the toolkit client code.
///
#define _DWFX_FORMAT_VERSION_CURRENT_MAJOR           _DWFX_FORMAT_VERSION_MAJOR
///
///\ingroup     DWFxFileFormat
///\brief       The current minor revision of the DWFx File Format.
///
///             This macro should be used by client code wishing to reference
///             the current minor version of the DWFx file format.
///
///             A minor version change usually indicates that some content or structure
///             was adding or modified in a way that would not impair existing DWFx processors.
///             Extensions or additions to schemas, introductions of new section types and
///             document and elements are all examples of minor revision triggers.
///             
///             Toolkit client code should always perform a minor version check to 
///             ensure 100% compatability and in the case that an increment occurs
///             at least notify the end user that some new data may exist that the
///             current product may not be able to handle/display/etc.
///
#define _DWFX_FORMAT_VERSION_CURRENT_MINOR           _DWFX_FORMAT_VERSION_MINOR
///
///\ingroup     DWFxFileFormat
///\brief       The current DWFx File Format version number as a composite integer value.
///
///             The current value of this macro is <b>700</b>.
///
#define _DWFX_FORMAT_VERSION_CURRENT                 ((_DWFX_FORMAT_VERSION_CURRENT_MAJOR*100) + _DWFX_FORMAT_VERSION_CURRENT_MINOR)
///
///\ingroup     DWFxFileFormat
///\brief       The current DWFx File Format version number as a composite floating point value.
///
///             The current value of this macro is <b>7.00</b>.
///
#define _DWFX_FORMAT_VERSION_CURRENT_FLOAT           (float)(_DWFX_FORMAT_VERSION_CURRENT_MAJOR + (_DWFX_FORMAT_VERSION_CURRENT_MINOR/100.0))
///
///\ingroup     DWFxFileFormat
///\brief       The current DWFx File Format version number as an ASCII character string.
///
#define _DWFX_FORMAT_VERSION_CURRENT_STRING          "7.10"
///
///\ingroup     DWFxFileFormat
///\brief       The current DWFx File Format version number as a wide character string.
///
#define _DWFX_FORMAT_VERSION_CURRENT_WIDE_STRING     L"7.10"


#define _DWFX_FORMAT_DEFAULT_VERSION   700
///
///\ingroup     Default DWFxFileFormat
///\brief       The current DWFx File Format version number as an ASCII character string.
///
#define _DWFX_FORMAT_DEFAULT_VERSION_STRING "7.00"
///
///\ingroup     Default DWFxFileFormat
///\brief       The current DWFx File Format version number as a wide character string.
///
#define _DWFX_FORMAT_DEFAULT_VERSION_WIDE_STRING L"7.00"


//DNT_End

#endif
