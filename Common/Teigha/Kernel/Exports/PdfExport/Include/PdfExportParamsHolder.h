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


///////////////////////////////////////////////////////////////////////////////
//
//  PdfExportParamsHolder.h : definition of the PdfExportParamsHolder class
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PDF_EXPORT_PARAMS_HOLDER_DEFINED
#define PDF_EXPORT_PARAMS_HOLDER_DEFINED

#include "PdfExportParams.h"

#define STL_USING_MAP
#define STL_USING_STRING
#include "OdaSTL.h"

#include "PdfDocument.h"
#include "PdfIContentCommands.h"

#include "PdfFontOptimizer.h"
#include "PdfType3Optimizer.h"
#include "PdfPageDictionary.h"

using namespace TD_PDF;


/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

  typedef std::map<OdString, OdRxObjectPtr> FrozenLayerMap;
/** \details
  This class implements the PDF export.
*/
class PdfExportParamsHolder
{
  // Members
  //
  PDFDocument              &m_Document; // PDF document for export process implementation.
  PDFIContentCommands      *m_pContentCommands; // Pointer to the content commands for PDF document.
  PDFPageDictionary        *m_pCurPage;
  PDFExportParams           m_Params; // PDF export parameters.

  PDFFontOptimizer          &m_FontOptimizer; // Font optimization options for a PDF document.
  PDFType3Optimizer         &m_Type3Optimizer; // Type optimization options for a PDF document.
  OdArray<OdString>         m_OffLayers; // Names of all invisible layers contained in the PDF document
  FrozenLayerMap            m_FrozenLayers;

  /** \details
    Returns a /page parameters/ for a specified layout.

    \remarks
	Returns a /page parameters/ for a specified layout (pLayout parameter). Also returns /result of getting page parameters operation/ (instance of PDFResultEx class). Page params are written to the pageParams parameter (a reference to an object of OdGsPageParams class).
	getPageParamsFromLayout() is a static method.

    \param pLayout [in]  Pointer to the layout.
	\param pageParams [in]  Reference to the OdGsPageParams object where the page parameters are written after successful method call.
  */
  static PDFResultEx getPageParamsFromLayout(OdRxObject* pLayout, OdGsPageParams &pageParams);
  
protected:


public:
    // Constructor/destructor

  PdfExportParamsHolder(PDFDocument& document, PDFFontOptimizer& font_optimizer, PDFType3Optimizer& type_3_optimizer);
    virtual ~PdfExportParamsHolder();

    // Implementation

/** \details
    Initiates PDF export with specified parameters.

    \remarks
	 Initiates PDF export with specified parameters (passed in the pParams parameter). Returns an instance of the PDFResultEx class as the result of the initiantion. 

    \param pParams [in]  Reference to the PDFExportParams class instance containing export parameter values.
*/
    PDFResultEx init(const PDFExportParams &pParams);

    /** \details
    Shows all the layers of the exporting document.

    \remarks
    Layers are determined in m_Params member variable.
    */
    void showAllLayers();
    void frozenLayers(bool showUp);
	
public:

/** \details
    Returns a reference to a PDFDocument object.

    \remarks
    Returns a reference to a PDFDocument object (m_Document member variable), that is used for PDF export.
*/
    PDFDocument &document()
    {
      return m_Document;
    }

/** \details
    Returns a reference to a PDFFontOptimizer class instance.

    \remarks
    PDFFontOptimizer class instance is contained in the m_FontOptimizer member variable. 
*/
    PDFFontOptimizer &getFontOptimizer() { return m_FontOptimizer; }
	
/** \details
    Returns the m_Type3Optimizer property value.

    \remarks
     Returns reference to PDFType3Optimizer object (representing type optimization options for PDF export) that is stored in the m_Type3Optimizer property. 
*/
    PDFType3Optimizer &getType3Optimizer() { return m_Type3Optimizer; }

/** \details
    Returns a pointer to a PDFIContentCommands object.

    \remarks
     Returns a pointer to a PDFIContentCommands object that can be used for handling PDF document created as the result of the export process.
*/
	  PDFIContentCommands *ContentCommands()
    {
      return m_pContentCommands;
    }

    void setContentCommands(PDFIContentCommands * content_commands)
    {
      m_pContentCommands = content_commands;
    }

    void setCurrentPage(PDFPageDictionary * current_page)
    {
      m_pCurPage = current_page;
    }

    PDFPageDictionary *CurrentPage()
    {
      return m_pCurPage;
    }

    // Auxiliary methods
    //

/** \details
    Return parameters of the PDF export.

    \remarks
     Returns reference to PDFExportParams object (representing PDF export options) that is stored in the m_Params property. 
*/
    const PDFExportParams& getParams() const
    {
      return m_Params;
    }
    PDFExportParams& getParams()
    {
      return m_Params;
    }

/** \details
    Returns /colour palette/ used for PDF export.

    \remarks
     Palette is an array of ODCOLORREF values. Pointer to this array is returned. 
*/
    const ODCOLORREF* getPalette() const
    {
      return m_Params.palette();
    }

/** \details
    Returns if the layer with specified name is visible.

    \remarks
    Returns true if the layer with specified name (parameter layerName) is visible in origin PDF document and false in other case. If specified layout doesn't exist, returns false.

    \param layerName [in]  Layout name. 
*/
	  bool isLayerOriginallyVisible(const OdString &layerName) const;

    const FrozenLayerMap& getFrozenLayers()
    {
      return m_FrozenLayers;
    }
};

class PdfExportParamsHolderPointer
{
  PdfExportParamsHolder             *m_ParamsHolder;

public:
  PdfExportParamsHolder* safeGetPointer() const
  {
    if (NULL == m_ParamsHolder)
      throw OdError(::eNullPtr);
    return m_ParamsHolder;
  }
  PdfExportParamsHolderPointer(PdfExportParamsHolder* params_holder)
    : m_ParamsHolder(params_holder)
  {
  }
  ~PdfExportParamsHolderPointer()
  {

  }

  PdfExportParamsHolder* operator->() { return safeGetPointer(); }
  const PdfExportParamsHolder* operator->() const { return safeGetPointer(); }
  operator PdfExportParamsHolder*() { return safeGetPointer(); }
  operator const PdfExportParamsHolder*() const { return safeGetPointer(); }
};

}
#endif //PDF_EXPORT_PARAMS_HOLDER_DEFINED
