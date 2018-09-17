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

#ifndef _PDF_IMPORTER_INCLUDED_ 
#define _PDF_IMPORTER_INCLUDED_

#include "PdfImport.h"
#include "public/fpdfview.h"
#include <map>
#include "core/fxcrt/bytestring.h"
#include "Ge/GePoint2dArray.h"

class PdfProperties;
typedef OdSmartPtr<PdfProperties> PdfPropertiesPtr;

/** \details
  <group OdPdfImport_Classes> 

  This class implements the upper level import manager.
*/
class PdfImporter : public OdPdfImport
{
  PdfPropertiesPtr             m_pProperties;
  FPDF_DOCUMENT                m_pDocument;
  FPDF_PAGE                    m_Page;
  OdInt32                      m_PageNum;
  OdGePoint2dArray             m_ClipBoundary;
  bool                         m_isClipBoundaryInverted;
  std::map<ByteString, bool>   m_LayersInfo;

public:
  /** Constructor. 
  */
  PdfImporter();

  ~PdfImporter();

  /** \details
    Returns the propertities.
  */
  OdRxDictionaryPtr properties();

  /** \details
    Imports the data.
  */
  ImportResult import();

  ImportResult load();

  bool isLoaded() const;

  double getMeasureDictInfo();

  OdPdfImport::ImportResult fillLayersInfo(std::map<ByteString, bool>& layers_info);

  double getPageWidth() const;

  double getPageHeight() const;

  void setClipBoundary(const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted);

  void setLayersInfo(std::map<ByteString, bool>& layers_info);

  void ClearUnderlayInfo();

private:
	

};

typedef OdSmartPtr<PdfImporter> PdfImporterPtr;

#endif // _PDF_IMPORTER_INCLUDED_
