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
#include "StaticRxObject.h"
#include "ExSystemServices.h"
#include "DynamicLinker.h"
#include "diagnostics.h"
#include "RxDynamicModule.h"

#include "OdDwfxSignatureHandler.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#ifndef _TOOLKIT_IN_DLL_
  #if defined(_MSC_VER) && (_MSC_VER > 1500)
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DwfxSignatureHandlerWinModule);

    ODRX_BEGIN_STATIC_MODULE_MAP()
      ODRX_DEFINE_STATIC_APPMODULE(DwfxSignatureHandlerWinModuleName, DwfxSignatureHandlerWinModule)
    ODRX_END_STATIC_MODULE_MAP()
  #else
    ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DwfxSignatureHandlerModule);

    ODRX_BEGIN_STATIC_MODULE_MAP()
      ODRX_DEFINE_STATIC_APPMODULE(DwfxSignatureHandlerModuleName, DwfxSignatureHandlerModule)
    ODRX_END_STATIC_MODULE_MAP()
  #endif
#endif

#define MAX_PATH_LENGTH 1024

void printCertData(const OdCertParameters& certData);

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  OdStaticRxObject<ExSystemServices> svcs;
  odrxInitialize(&svcs);
  
  try
  {
    OdDwfxSignatureHandlerModulePtr pSignatureHandlerModule;

#if defined(ODA_WINDOWS)
    pSignatureHandlerModule = odrxDynamicLinker()->loadApp(DwfxSignatureHandlerWinModuleName);
#else
    pSignatureHandlerModule = odrxDynamicLinker()->loadApp(DwfxSignatureHandlerModuleName);
#endif

    if (!pSignatureHandlerModule.isNull())
    {
      OdDwfxSignatureHandlerPtr pSignatureHandler;
      pSignatureHandler = pSignatureHandlerModule->getDwfxSignatureHandler();
      
      OdAnsiString filePath;
      OdPrintf("Input full path to the DWFx package and press Enter:\n");
      GETSS((char*)filePath.getBuffer(MAX_PATH_LENGTH), MAX_PATH_LENGTH);
      filePath.releaseBuffer();

      if (!svcs.accessFile(filePath, Oda::kFileRead))
      {
        OdPrintf("\nCan't access the file specified!\n");
      }
      else
      {
        OdAnsiString opNumber;
        int caseCode = 0;
        bool exitFlag = false;
        do {
          OdPrintf("\n\nInput the number of the operation you want to perform and press Enter:\n");
          OdPrintf("1 - to validate signature of the package;\n");
          OdPrintf("2 - to sign the package;\n");
          OdPrintf("3 - to remove existing signature of the package;\n");
          OdPrintf("4 - to exit;\n");
          GETSS((char*)opNumber.getBuffer(MAX_PATH_LENGTH), MAX_PATH_LENGTH);
          opNumber.releaseBuffer();
          caseCode = atol(opNumber.c_str());
          switch (caseCode)
          {
          case 1://VALIDATION:
            {
              OdCertParameters outCertParam;
              OdDwfxSignatureHandler::SignatureValidationResult validationRes = OdDwfxSignatureHandler::kNotSigned;
              if (pSignatureHandler->validatePackageSignature(filePath, validationRes, outCertParam))
              {
                switch (validationRes)
                {
                case OdDwfxSignatureHandler::kNotSigned:
                  OdPrintf("RESULT: The package has no signature.\n");
                  break;
                case OdDwfxSignatureHandler::kSuccess:
                  OdPrintf("RESULT: The signature is valid.\n");
                  OdPrintf("Certificate data:\n");
                  printCertData(outCertParam);
                  break;
                case OdDwfxSignatureHandler::kInvalidSignature:
                  OdPrintf("RESULT: Invalid signature!\n");
                  break;
                case OdDwfxSignatureHandler::kCertificateChainProblem:
                  OdPrintf("RESULT: Can't verify certificate trust status!\n");
                  OdPrintf("Certificate data:\n");
                  printCertData(outCertParam);
                  break;
                }
              }
              else
              {
                OdPrintf("ERROR while trying to validate signature!\n");
              }
            }          
            break;
          case 2://SIGNING:
            {
              OdArray<OdCertParameters> certificates;
              int certNumber = pSignatureHandlerModule->getSuitableCertificatesList(certificates);
              if (certNumber)
              {
                OdPrintf("Certificate list:\n");
                for (int i = 0; i < certNumber; i++)
                {
                  OdPrintf("[%d] ", i + 1);
                  printCertData(certificates[i]);
                }
                OdAnsiString certIndex;
                OdPrintf("Select a certificate by its number in the list:\n");
                GETSS((char*)certIndex.getBuffer(MAX_PATH_LENGTH), MAX_PATH_LENGTH);
                certIndex.releaseBuffer();
                int nSelectedCert = atol(certIndex.c_str());
                if ((nSelectedCert > 0)&&(nSelectedCert <= certNumber))
                {
                  pSignatureHandler->setCertParameters(certificates[nSelectedCert - 1]);
                  if (pSignatureHandler->signPackage(filePath))
                    OdPrintf("RESULT: The package successfully signed.\n");
                  else
                    OdPrintf("ERROR while trying to sign the package!\n");
                }
                else
                {
                  OdPrintf("Wrong input!\n");
                }
              }
              else
              {
                OdPrintf("No valid digital ID (Certificate) is available on your system!\n");
              }
            }
            break;
          case 3://REMOVE EXISTING SIGNATURE:
            if (pSignatureHandler->removeExistingSignature(filePath))          
              OdPrintf("RESULT: Signature successfully removed!\n");
            else
              OdPrintf("ERROR while trying to remove existing signature!\n");
            break;
          case 4://EXIT:
            exitFlag = true;
            break;
          default:
            OdPrintf("Wrong input!\n");
            break;
          }
        } while (!exitFlag);
      }      
    }
    else
    {
      OdPrintf("Can't load DWFx signature handler module!\n");
    }
  }
  catch (OdError& e)
  {
    OdPrintf("Exception (%ls) during the package processing!\n", e.description().c_str());    
  }
  catch (...)
  {
    OdPrintf("Unknown Exception during the package processing!\n");
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odrxUninitialize();

	return 0;
}

void printCertData(const OdCertParameters& certData)
{  
  OdPrintf("Subject = %ls, Issuer = %ls, Serial number = %ls, Expiration date = %ls\n", 
    certData.sCertSubject.c_str(), certData.sCertIssuer.c_str(), 
    certData.sCertSerialNum.c_str(), certData.m_CertValidTo.c_str());
}
