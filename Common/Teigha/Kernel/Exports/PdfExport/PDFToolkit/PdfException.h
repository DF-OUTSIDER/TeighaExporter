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

#ifndef __PDFEXCEPTION_H__
#define __PDFEXCEPTION_H__

#include "PdfResult.h"
#include "OdString.h"

namespace TD_PDF
{

#ifdef _DEBUG
    #define PDF_EXCEPTION( x ) throw PdfException( x, OD_T(__FILE__), __LINE__ )
#else
    #define PDF_EXCEPTION( x ) throw PdfException( x )
#endif

class PDFEXPORT_TOOLKIT PdfException
{
public:
    PDFResult m_uErrorCode;
public:
    PdfException()
      : m_uErrorCode(eOk)
    {
    }

    PdfException( PDFResult uErrorCode ) 
      : m_uErrorCode( uErrorCode )
    {
    }

    PdfException( PDFResult uErrorCode, const OdChar* cpFile, int iLine)
    : m_uErrorCode( uErrorCode )
    {
#ifdef OD_WINDOWS_DESKTOP
        OdString s;
        s.format(L"PdfException %d in %s %d\n", uErrorCode, cpFile, iLine);
        
        DWORD dw;
        HANDLE o = ::GetStdHandle(STD_OUTPUT_HANDLE);
        if (::GetFileType(o) == FILE_TYPE_CHAR)
          WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
        else
          WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
#endif
    }

    ~PdfException() {}
};

}

#endif  //__PDFEXCEPTION_H__
