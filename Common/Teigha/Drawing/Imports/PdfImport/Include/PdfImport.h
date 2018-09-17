/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDFIMPORT_INCLUDED_
#define _PDFIMPORT_INCLUDED_

#include "RxModule.h"
#include "RxDictionary.h"
#include "DynamicLinker.h"

//////////////////////////////////////////////////////////////////////////

#include "OdPlatformSettings.h"

#ifdef PDFIMPORT_EXPORTS
  #define PDFIMPORT_TOOLKIT         OD_TOOLKIT_EXPORT
  #define PDFIMPORT_TOOLKIT_STATIC  OD_STATIC_EXPORT
#else                               
  #define PDFIMPORT_TOOLKIT         OD_TOOLKIT_IMPORT
  #define PDFIMPORT_TOOLKIT_STATIC  OD_STATIC_IMPORT
#endif

//////////////////////////////////////////////////////////////////////////

class OdDbDatabase;

//DD:EXPORT_ON

/** \details
  <group OdPdfImport_Classes>

  This class implements a .pdf importer functionality.
*/
class OdPdfImport : public OdRxObject
{
public:
  /** \details Contains the list of import result, which are handled with importing module.
  */
  enum ImportResult 
  {
    success,              //!< The import process has been successfully finished.
    fail,                 //!< The import process has been failed.
    bad_password,         //!< The import process has been failed because of incorrect password.
    bad_file,             //!< The import process has been failed because of incorrect input .pdf file.
    bad_database,         //!< The import process has been failed because of incorrect database object to be imported to.
    encrypted_file,       //!< The import process has been failed because of encrypted input .pdf file.
    invalid_page_number,  //!< The import process has been failed because of invalid page number of the input .pdf file.
    image_file_error      //!< The import process has been failed because of it is impossible to save the image file 
                          // (invalid directory for saving is possible) when importing  pdf.
  };
  
  /** \details A pure virtual function to be reimplemented for providing import functionality.
  \returns Returns the result of import operation represented with a value of ImportResult enumeration.
  */
  virtual ImportResult import() = 0;

  /** \details Returns a dictionary that contains properties used for the importing a drawing from a .pdf file. 
  \remarks 
  Implemented properies are: 
  * "Database"  An OdDbDatabase object where the contents of a .pdf file should be imported.
  * "PdfPath"   A full path to the imported .pdf file.
  * "Password"  A password for the input .pdf file. By default this property is empty.
  * "PageNumber"  A page number to import in input .pdf file. Starts from 1. By default this property is 1.
  * "LayersUseType"  A method to apply for assigning imported objects to layers. By default this property is 1.
       0 - Use PDF Layers
         in this case, layers are created of the PDF_ <Layer name from the PDF document>.The visibility of the layers is determined by their
         visibility in the pdf document. If the object is not on any of the layers, then it is installed on layers similar to case 1.
         If the object in the pdf document is on more than one layer, then when it is imported, it is set to the "inner" layer of those on which it seted
      1 - Create object layers
         Creating object layers(PDF_Geometry, PDF_Solid Fills, PDF_Images, and PDF_Text etc and PDF_Geometry_Invisible, PDF_Solid Fills_Invisible,
         PDF_Images_Invisible, and PDF_Text _Invisible for invisible objects)
      2 - Use current layer
         Importing all specified PDF objects to the current layer. (For invisible objects - invisible layers : PDF_Geometry_Invisible,
         PDF_Solid Fills_Invisible, PDF_Images_Invisible, and PDF_Text _Invisible)
  * "ImportVectorGeometry"  Import PDF geometric data types include linear paths, Bezier curves, and solid-filled areas, which are imported as 
         polylines, and 2D solids or solid-filled hatches. By default this property is true.
  * "ImportSolidFills"  Import all solid-filled areas. By default this property is true.
  * "ImportTrueTypeText"  Import text objects that use TrueType fonts. By default this property is true.
  * "ImportRasterImages"  Imports raster images by saving them as PNG files and attaching them to the current drawing. By default this property is true.
         The directory for saving is specified by the system variable PDFIMPORTIMAGEPATH. If this variable isn't specified, 
         the images are saved in the application temporary directory.
  * "ImportGradientFills"  Imports shading objects as images by saving them as PNG files and attaching them to the current drawing. 
         By default this property is true.
  * "ImportAsBlock"  Imports the PDF file as a block rather than as separate objects. By default this property is false.
  * "JoinLineAndArcSegments"  Joins contiguous segments into a polyline where possible. By default this property is true.
  * "ConvertSolidsToHatches"  Converts 2D solid objects into solid-filled hatches. By default this property is true.
  * "ApplyLineweight"  Retains or ignores the lineweight properties of the imported objects. By default this property is true.
  * "ImportTrueTypeTextAsGeometry"  Import text objects that use TrueType fonts as polylines and solid-filled hatches. By default this property is false.
  * "Scaling"  Specify a different import scaling factor for imported objects. By default this property is 1.0.
  * "Rotation"  Rotation for imported objects. By default this property is 0.0.
  * "InsertionPointX"  A X-coord of location of the imported PDF relative to the location of the current UCS. By default this property is 0.0.
  * "InsertionPointY"  A Y-coord of location of the imported PDF relative to the location of the current UCS. By default this property is 0.0.
  */

  virtual OdRxDictionaryPtr properties() = 0;
};

/** \details
  <group OdPdfImport_Classes>
  Smart pointer to a .pdf importer object.
*/
typedef OdSmartPtr<OdPdfImport> OdPdfImportPtr;

/** \details
  <group OdPdfImport_Classes> 

  This class implements the .pdf import module.
*/
class OdPdfImportModule : public OdRxModule
{
public:
    
    /** \details Creates an instance of .pdf import module.
    */
    virtual OdPdfImportPtr create () = 0;
    
};

/** \details
  <group OdPdfImport_Classes> 
  Smart pointer to a .pdf import module object.
*/
typedef OdSmartPtr<OdPdfImportModule> OdPdfImportModulePtr;


/** \details A stand alone function that creates an instance of .pdf import module and returns a smart point on it. 
<group OdPdfImport_Classes>
*/
inline OdPdfImportPtr createImporter()
{
     OdPdfImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdPdfImportModuleName);
     if ( !pModule.isNull() ) return pModule->create ();
     return (OdPdfImport*)0;
}

//DD:EXPORT_OFF

#endif // _PDFIMPORT_INCLUDED_
