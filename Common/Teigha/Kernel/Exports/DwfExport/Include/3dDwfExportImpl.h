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
//  3dDwfExportImpl.h : definition of the C3dDwfExportImpl class
//
///////////////////////////////////////////////////////////////////////////////
//
// MKU 08/16/05 - for exporting to 3D DWF using DWFToolkit v7.0
//

#ifndef DWF_3D_EXPORTIMPL_DEFINED
#define DWF_3D_EXPORTIMPL_DEFINED


#include "DwfExportParams.h"
#include "DwfExportImpl.h"

#include "dwf/publisher/model/Model.h"
#include "dwf/publisher/model/Segment.h" 

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DWF_EXPORT {

/** \details

   This class implements a 3D DWF export.
*/
class C3dDwfExportImpl : public CDwfExportMeter
{
  Dwf3dExportParams&      m_Params;
  DWFToolkit::DWFModel*   m_pModel;
  DWFToolkit::DWFSegment* m_pCurrentSegment;
  TK_Color_RGB*           m_pTKColorRGB;
  const ODCOLORREF*       m_pPalette;
public:
  C3dDwfExportImpl(Dwf3dExportParams& pParams);
  virtual ~C3dDwfExportImpl();

  // Implementation

  /** \details
    Runs the export to DWF file process for the current document.
  */
  bool run();
  
  /** \details
    Returns a pointer to the database object being exported.
  */
  virtual OdDbBaseDatabase* getDB()
  {
    return m_Params.database();
  }

  //  ----- C3dDwfExportImpl methods -----
  //
  
  /** \details
    Returns a pointer to the current segment object.
  */
  DWFToolkit::DWFSegment* getCurrentSegment()
  {
    return m_pCurrentSegment;
  }

  /** \details
    Returns a pointer to the Toolkit color.
  */
  TK_Color_RGB* getTKColorRGB()
  {
    return m_pTKColorRGB;
  }

};
}
#endif /* DWF_3D_EXPORTIMPL_DEFINED */
