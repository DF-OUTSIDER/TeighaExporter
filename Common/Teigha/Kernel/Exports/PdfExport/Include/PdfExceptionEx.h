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

#ifndef __PDFEXCEPTIONEX_H__
#define __PDFEXCEPTIONEX_H__

#include "OdString.h"
#include "PdfResultEx.h"

#ifdef _DEBUG
    #define PDF_EXCEPTIONEX( x ) throw PdfExceptionEx( x, OD_T(__FILE__), __LINE__ )
#else
    #define PDF_EXCEPTIONEX( x ) throw PdfExceptionEx( x )
#endif

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

/** \details
  This class implements the PDF result exception.
*/
class PdfExceptionEx
{
public:
    PDFResultEx m_uErrorCode; // An object of /PDFResultEx/ class that implements a PDF export /error code/. 
public:
    PdfExceptionEx( PDFResultEx uErrorCode ) 
      : m_uErrorCode( uErrorCode )
    {
    }
    PdfExceptionEx( PDFResultEx uErrorCode, const OdChar* cpFile, int iLine)
    : m_uErrorCode( uErrorCode )
    {
#if defined(ODA_WINDOWS)
        OdString sMes;
        sMes.format(OD_T("PdfExceptionEx %d in %s %d\n"), uErrorCode, cpFile, iLine);
        OutputDebugString( sMes );
#endif
    }
};
}
#endif  //__PDFEXCEPTIONEX_H__
