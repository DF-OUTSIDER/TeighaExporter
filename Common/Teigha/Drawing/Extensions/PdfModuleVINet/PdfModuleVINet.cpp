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

#include "OdaCommon.h"
#include "PdfModuleVINet.h"
#include "RxDynamicModule.h"
#include <winver.h>
#include "PdfModuleVINetVersion.h"

ODRX_DEFINE_DYNAMIC_MODULE(PdfModuleVINet);

#ifdef PDFMODULEVI_PRODUCTION
#pragma comment(lib,"TD_PdfKey_VI.lib")
#include "PdfKey/PdfKey.h"
#endif // PDFMODULEVI_PRODUCTION

void PdfModuleVINet::initApp()
{
  OdStringArray appNamesArr;
  appNamesArr.append("\x004F\x0064ViewExMgd\x002E\x0065\x0078\x0065");
  appNamesArr.append("\x004F\x0064\x0061MfcApp\x002E\x0065\x0078\x0065");
  appNamesArr.append("QwikVue");
  appNamesArr.append("QwikVue.exe");
  appNamesArr.append("QWIKVUE.EXE");
  appNamesArr.append("AiMaker.exe");
  appNamesArr.append("AIMAKER.EXE");
  appNamesArr.append("OdaDwgAppSwigMgd.exe");
  appNamesArr.append("Solution_Documentation.exe");
  appNamesArr.append("ExcitechDOCS.Teigha.dll");
  appNamesArr.append("dllhost.exe");
  appNamesArr.append("prevhost.exe");

  OdStringArray copyRights;
  copyRights.append(COPYRIGHT);
  copyRights.append("Copyright © 2011-2014, Open Design Alliance");
  copyRights.append("Copyright © 2002-2015, Open Design Alliance");
  copyRights.append("Copyright © 2002-2016, Open Design Alliance");
  copyRights.append("Copyright © 2002-2017, Open Design Alliance");
  copyRights.append("Copyright © 2002-2018, Open Design Alliance");
  copyRights.append("Copyright © 2014 Logical Systems, LLC");
  copyRights.append("Copyright © 2015 Logical Systems, LLC");
  copyRights.append("Copyright © 2016 Logical Systems, LLC");
  copyRights.append("Copyright © 2017 Logical Systems, LLC");
  copyRights.append("Copyright © 2018 Logical Systems, LLC");
  copyRights.append("(c) Solution Suppliers Limited");
  copyRights.append("(©) Solution Suppliers Limited");
  copyRights.append("© Solution Suppliers Limited");
  copyRights.append("Copyright © Excitech 2017");
  copyRights.append("Copyright © Excitech 2018");
  copyRights.append("© Microsoft Corporation. All rights reserved.");


  PdfModuleVINetVerification(appNamesArr, copyRights);
}

void PdfModuleVINet::uninitApp()
{
#ifdef PDFMODULEVI_PRODUCTION
  uninitPdfSupport();
#endif
}
