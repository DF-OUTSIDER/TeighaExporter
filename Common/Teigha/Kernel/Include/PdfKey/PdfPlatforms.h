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

#ifndef PDFPLATFORMS_H
#define PDFPLATFORMS_H

// Auxiliary define to check supported platforms for PdfModuleVI.

// WIN32 DLL 71, 8, 9, 2010
#if !defined(_WIN32_WCE) && defined(_WIN32) && !defined(_WIN64) && defined(_TOOLKIT_IN_DLL_) && (_MSC_VER == 1310 || _MSC_VER == 1400 || _MSC_VER == 1500 || _MSC_VER == 1600)
#define PDF_VI_MODULE_PLATFORMS
#endif

//or all unix 

#endif
