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



//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

#ifndef _PDF_LINEARIZER_
#define _PDF_LINEARIZER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OdStreamBuf.h"

using namespace TD_PDF;

//////////////////////////////////////////////////////////////////////

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

/** \details
  Linearizer for PDF export.
*/
namespace PDFLinearizer
{
  void linearize(OdStreamBufPtr& inStream, OdStreamBufPtr& outStream);
};

}
#endif // #ifndef _PDF_LINEARIZER_
