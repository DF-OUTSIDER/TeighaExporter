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


#ifndef _PDFEXPORTSERVICE_INCLUDED_
#define _PDFEXPORTSERVICE_INCLUDED_

#include "RxObject.h"
#include "Gi/GiDrawable.h"
#include "DbBaseDatabase.h"
#include "RxDictionary.h"

#include "TD_PackPush.h"


// forward declaration
struct PDF2PRCExportParams;

/** \details
    This is helper service for PdfExport to support export of 3d entities.
    
    \sa
    <group OdExport_Classes>
*/
class PdfExportServiceInterface
{
protected:
  PdfExportServiceInterface() {}
public:  
  virtual OdResult serialize(OdStreamBufPtr pBuffer, OdRxObjectPtr &context, const OdGiDrawable* pDrawable, const PDF2PRCExportParams *params, OdDbBaseDatabase *pDb) = 0;

  virtual ~PdfExportServiceInterface() {}
};


inline OdRxClassPtr getPdfExportService()
{
  return odrxServiceDictionary()->getAt(OD_T("PdfExportService"));
}

#include "TD_PackPop.h"

#endif // _PDFEXPORTSERVICE_INCLUDED_
