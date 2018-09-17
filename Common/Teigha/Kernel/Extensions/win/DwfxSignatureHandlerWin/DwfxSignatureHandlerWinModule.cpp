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
#include "RxDynamicModule.h"
#include "DwfxSignatureHandlerWinModule.h"
#include "OdDwfxSignatureHandlerWinImpl.h"

#include <WinCrypt.h>

ODRX_DEFINE_DYNAMIC_MODULE(DwfxSignatureHandlerWinModule);

static DwfxSignatureHandlerWinModule* g_pModule = 0;

void DwfxSignatureHandlerWinModule::initApp()
{
  g_pModule = this;
  m_pCOMInitMutex.attach();
}

void DwfxSignatureHandlerWinModule::uninitApp()
{
  m_pCOMInitMutex.detach();
  g_pModule = 0;
}

OdMutexPtr& DwfxSignatureHandlerWinModule::globalCOMInitMutex()
{
  if (!g_pModule || !g_pModule->m_pCOMInitMutex.m_pMutex)
    throw OdError(eNullPtr);
  return *(g_pModule->m_pCOMInitMutex.m_pMutex);
}

OdDwfxSignatureHandlerPtr DwfxSignatureHandlerWinModule::getDwfxSignatureHandler() const
{
  OdDwfxSignatureHandlerPtr pDwfxSignatureHandler = new OdDwfxSignatureHandlerWinImpl();

  return pDwfxSignatureHandler;
}

int DwfxSignatureHandlerWinModule::getSuitableCertificatesList(OdArray<OdCertParameters>& certificates) const
{
  int result = 0;

  HCERTSTORE hSysStore = NULL;
  hSysStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING), 
    NULL, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
  if (hSysStore)
  { 
    PCCERT_CONTEXT pCertContext = NULL;
    while (pCertContext = CertEnumCertificatesInStore(hSysStore, pCertContext))
    {
      HCRYPTPROV hCryptProv;
      DWORD dwKeySpec;
      if (CryptAcquireCertificatePrivateKey(pCertContext, 0, NULL, &hCryptProv, &dwKeySpec, NULL))
      {
        HCERTCHAINENGINE hChainEngine = NULL;
        CERT_CHAIN_ENGINE_CONFIG ChainConfig;

        ChainConfig.cbSize = sizeof(CERT_CHAIN_ENGINE_CONFIG);
        ChainConfig.hRestrictedRoot= NULL ;
        ChainConfig.hRestrictedTrust= NULL ;
        ChainConfig.hRestrictedOther= NULL ;
        ChainConfig.cAdditionalStore=0 ;
        ChainConfig.rghAdditionalStore = NULL ;
        ChainConfig.dwFlags = CERT_CHAIN_CACHE_END_CERT;
        ChainConfig.dwUrlRetrievalTimeout= 0 ;
        ChainConfig.MaximumCachedCertificates=0 ;
        ChainConfig.CycleDetectionModulus = 0;

#if (defined(NTDDI_VERSION) && defined(NTDDI_WIN7) && NTDDI_VERSION >= NTDDI_WIN7)
        ChainConfig.hExclusiveRoot = 0;
        ChainConfig.hExclusiveTrustedPeople = 0;
#endif

        if (CertCreateCertificateChainEngine(&ChainConfig, &hChainEngine))
        {
          PCCERT_CHAIN_CONTEXT pChainContext = NULL;

          CERT_ENHKEY_USAGE EnhkeyUsage;
          CERT_USAGE_MATCH  CertUsage;  
          CERT_CHAIN_PARA   ChainPara;
          EnhkeyUsage.cUsageIdentifier = 0;
          EnhkeyUsage.rgpszUsageIdentifier = NULL;
          CertUsage.dwType = USAGE_MATCH_TYPE_AND;
          CertUsage.Usage  = EnhkeyUsage;
          ChainPara.cbSize = sizeof(CERT_CHAIN_PARA);
          ChainPara.RequestedUsage = CertUsage;
          DWORD dwFlags=0;

          if (CertGetCertificateChain(hChainEngine, pCertContext, NULL, 
            pCertContext->hCertStore, &ChainPara, dwFlags, NULL, &pChainContext))
          {
            if (pChainContext->TrustStatus.dwErrorStatus == CERT_TRUST_NO_ERROR)
            {
              //get subject name:
              wchar_t subjectName[256];
              CertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, subjectName, 256);

              //get issuer name:
              wchar_t issuerName[256];
              CertGetNameStringW(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, issuerName, 256);

              //get serial number:
              wchar_t* serialNumberFromCert;
              int serialNumberStringLen = 0;
              int sizeOfSerialNumberBLOB = pCertContext->pCertInfo->SerialNumber.cbData;
              serialNumberStringLen = sizeOfSerialNumberBLOB * 2;
              serialNumberFromCert = new wchar_t[serialNumberStringLen + 1];
              serialNumberFromCert[serialNumberStringLen] = 0;
              int j = serialNumberStringLen - 1;
              for (int i = 0; i < sizeOfSerialNumberBLOB; i++)
              {
                wchar_t strRepresentationOfByte[3];
                strRepresentationOfByte[2] = 0;
                wsprintf(strRepresentationOfByte, L"%02x", pCertContext->pCertInfo->SerialNumber.pbData[i]);
                serialNumberFromCert[j] = strRepresentationOfByte[1];
                serialNumberFromCert[j - 1] = strRepresentationOfByte[0];
                j = j - 2;
              }

              //get expiration date:
              FILETIME ft = pCertContext->pCertInfo->NotAfter;
              SYSTEMTIME st;
              wchar_t localDate[255];

              FileTimeToLocalFileTime( &ft, &ft );
              FileTimeToSystemTime( &ft, &st );
              GetDateFormat( LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, NULL, localDate, 255 );
              OdString expirationDate(localDate);

              OdCertParameters currentCertParams;
              currentCertParams.sCertSubject = subjectName;
              currentCertParams.sCertIssuer = issuerName;
              currentCertParams.sCertSerialNum = serialNumberFromCert;
              currentCertParams.m_CertValidTo = expirationDate;

              certificates.push_back(currentCertParams);

              result++;

              delete[] serialNumberFromCert;
            }
          }
          if (pChainContext)
            CertFreeCertificateChain(pChainContext);
        }

        if (hChainEngine)
          CertFreeCertificateChainEngine(hChainEngine);
      }
    }

    if (pCertContext)
      CertFreeCertificateContext(pCertContext);
  }

  if (hSysStore)
    CertCloseStore(hSysStore, 0);

  return result;
}
