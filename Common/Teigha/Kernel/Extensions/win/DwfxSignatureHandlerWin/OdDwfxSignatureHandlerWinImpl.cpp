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

#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0601 // to enable Windows 7 Packaging API
#endif

#define CRYPT_OID_INFO_HAS_EXTRA_FIELDS

#include "OdaCommon.h"
#include "OdDwfxSignatureHandlerWinImpl.h"
#include "DwfxSignatureHandlerWinModule.h"
#include <msopc.h>
#include <WinCrypt.h>
#include <cryptxml.h>

#define STL_USING_LIST
#include <OdaSTL.h>

OdDwfxSignatureHandlerWinImpl::OdDwfxSignatureHandlerWinImpl()
{
}

OdDwfxSignatureHandlerWinImpl::~OdDwfxSignatureHandlerWinImpl()
{
}

void OdDwfxSignatureHandlerWinImpl::setCertParameters(const OdCertParameters& certParams)
{
  m_certParams = certParams;
}

PCCERT_CONTEXT getCertIfDataIsValid(HCERTSTORE hSysStore, 
  const OdString& subjStr, const OdString& issuerStr, const OdString& serialNumStr);
HRESULT ReadPackageFromFile(LPCWSTR filename, IOpcFactory* opcFactory, IOpcPackage** opcPackage);
BOOL SupportsSignatureAlgorithm(LPCWSTR signingMethodToCheck, PCCERT_CONTEXT certificateToCheck);
OdString getSignatureMethodFromCert(PCCERT_CONTEXT cert);
HRESULT PrepareForSigning(IOpcFactory* opcFactory, IOpcPackage* opcPackage, 
  IOpcSigningOptions* signingOptions, IOpcDigitalSignatureManager* opcDigSigManager, PCCERT_CONTEXT cert);
HRESULT WritePackageToFile(LPCWSTR filename, IOpcPackage* opcPackage, IOpcFactory* opcFactory);
void RemoveAllSignatures(IOpcPackage* opcPackage, IOpcDigitalSignatureManager* opcDigSigManager);
HRESULT ValidateSignature(IOpcDigitalSignatureManager* opcDigSigManager,
  IOpcDigitalSignature* signature, BOOL* isValid, PCCERT_CONTEXT* signerCert);
void getResultDataForCert(PCCERT_CONTEXT pCert, OdCertParameters& certParams, BOOL& isTrusted);

bool OdDwfxSignatureHandlerWinImpl::signPackage(const OdString& filePath)
{
  bool res = false;

  HCERTSTORE hSysStore = NULL;
  PCCERT_CONTEXT pDesiredCert = NULL;
  hSysStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING), 
    NULL, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
  if (hSysStore)
  {
    pDesiredCert = getCertIfDataIsValid(hSysStore, 
      m_certParams.sCertSubject, m_certParams.sCertIssuer, m_certParams.sCertSerialNum);
    if (pDesiredCert)
    {
      COMInit ialize;

      //Initialize an OPC factory:
      IOpcFactory* opcFactory = NULL;
      HRESULT hr = CoCreateInstance(__uuidof(OpcFactory), NULL, 
        CLSCTX_INPROC_SERVER, __uuidof(IOpcFactory), (LPVOID*)&opcFactory);

      if (SUCCEEDED(hr))
      {
        //Process the package:

        IOpcPackage* opcPackage = NULL;
        IOpcDigitalSignatureManager* opcDigSigManager = NULL;
        IOpcDigitalSignatureEnumerator* signatureEnumerator = NULL;

        HRESULT hr = ReadPackageFromFile(filePath.c_str(), opcFactory, &opcPackage);
        if (SUCCEEDED(hr))
        {
          hr = opcFactory->CreateDigitalSignatureManager(opcPackage, &opcDigSigManager);
        }
        if (SUCCEEDED(hr))
        {
          hr = opcDigSigManager->GetSignatureEnumerator(&signatureEnumerator);
        }
        if (SUCCEEDED(hr))
        {
          // Check for an existing signature.
          BOOL hasNext = FALSE;
          hr = signatureEnumerator->MoveNext(&hasNext);
          if (SUCCEEDED(hr) && hasNext)
          {
            //Remove an existing signature:
            RemoveAllSignatures(opcPackage, opcDigSigManager);
          }
        }

        IOpcSigningOptions* signingOptions = NULL;
        IOpcDigitalSignature* signature = NULL;
        if (SUCCEEDED(hr))
        {
          hr = opcDigSigManager->CreateSigningOptions(&signingOptions);
        }
        if (SUCCEEDED(hr))
        {
          // Configure signing options, adding references to parts and relationships
          // that need to be signed.
          hr = PrepareForSigning(opcFactory, opcPackage, signingOptions, opcDigSigManager, pDesiredCert);
        }
        if (SUCCEEDED(hr))
        {
          //Sign the package:
          hr = opcDigSigManager->Sign(pDesiredCert, signingOptions, &signature);
        }

        OdString tempTargetFilePath = filePath + OD_T(".tmp");
        if (SUCCEEDED(hr))
        { 
          hr = WritePackageToFile(tempTargetFilePath.c_str(), opcPackage, opcFactory);
        }

        //Release all of the package related resources in reverse order
        //to close read-only file stream before overwriting the original file:
        if (signature)
        {
          signature->Release();
          signature = NULL;
        }
        if (signingOptions)
        {
          signingOptions->Release();
          signingOptions = NULL;
        }
        if (signatureEnumerator)
        {
          signatureEnumerator->Release();
          signatureEnumerator = NULL;
        }
        if (opcDigSigManager)
        {
          opcDigSigManager->Release();
          opcDigSigManager = NULL;
        }
        if (opcPackage)
        {
          ULONG releaseResForPackage = opcPackage->Release();
          ODA_ASSERT(releaseResForPackage == 0);
          opcPackage = NULL;
        }
        
        //Replace the original file with signed one:
        if (SUCCEEDED(hr))
        {
          res = CopyFile(tempTargetFilePath.c_str(), filePath.c_str(), FALSE);          
        }
        DeleteFile(tempTargetFilePath.c_str());
      }

      //Release an OPC factory:
      if (opcFactory)
      {
        opcFactory->Release();
        opcFactory = NULL;
      }
    }
  }

  if (pDesiredCert)
    CertFreeCertificateContext(pDesiredCert);
  if (hSysStore)
    CertCloseStore(hSysStore, 0);
  
  return res;
}

HRESULT ReadPackageFromFile(LPCWSTR filename, IOpcFactory* opcFactory, IOpcPackage** opcPackage)
{
  IStream* fileStream = NULL;

  // Create a stream over the package file.
  HRESULT hr = opcFactory->CreateStreamOnFile(
    filename,
    OPC_STREAM_IO_READ, 
    NULL,                   // Use default security attribute.
    FILE_ATTRIBUTE_NORMAL,  // dwFlagsAndAttributes.
    &fileStream             // The new stream over the file,
    );

  // Read the package from the stream. 
  if (SUCCEEDED(hr))
  {
    hr = opcFactory->ReadPackageFromStream(
      fileStream, 
      OPC_READ_DEFAULT,
      opcPackage
      );
  }

  // Release resources.
  if (fileStream)
  {
    fileStream->Release();
    fileStream = NULL;
  }

  return hr;
}

PCCERT_CONTEXT getCertIfDataIsValid(HCERTSTORE hSysStore, 
  const OdString& subjStr, const OdString& issuerStr, const OdString& serialNumStr)
{
  PCCERT_CONTEXT  pDesiredCert = NULL;

  pDesiredCert = CertFindCertificateInStore(hSysStore, (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING), 0, 
    CERT_FIND_SUBJECT_STR, (const wchar_t*)subjStr, NULL);

  if (!pDesiredCert)
    return NULL;

  wchar_t issuerName[256];
  if (!CertGetNameStringW(pDesiredCert, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, issuerName, 256))
    return NULL;

  wchar_t* serialNumberFromCert;
  int serialNumberStringLen = 0;
  int sizeOfSerialNumberBLOB = pDesiredCert->pCertInfo->SerialNumber.cbData;
  serialNumberStringLen = sizeOfSerialNumberBLOB * 2;
  serialNumberFromCert = new wchar_t[serialNumberStringLen + 1];
  serialNumberFromCert[serialNumberStringLen] = 0;
  int j = serialNumberStringLen - 1;
  for (int i = 0; i < sizeOfSerialNumberBLOB; i++)
  {
    wchar_t strRepresentationOfByte[3];
    strRepresentationOfByte[2] = 0;
    odSprintf(strRepresentationOfByte, 3, L"%02x", pDesiredCert->pCertInfo->SerialNumber.pbData[i]);
    serialNumberFromCert[j] = strRepresentationOfByte[1];
    serialNumberFromCert[j - 1] = strRepresentationOfByte[0];
    j = j - 2;
  }

  bool issuerStrIsOk = (wcscmp(issuerStr, issuerName) == 0);
  bool serialNumStrIsOk = (wcscmp(serialNumStr, serialNumberFromCert) == 0);

  delete[] serialNumberFromCert;

  if (!issuerStrIsOk || !serialNumStrIsOk)
    return NULL;

  return pDesiredCert;
}

static const WCHAR g_defaultDigestMethod[] = L"http://www.w3.org/2000/09/xmldsig#sha1";
static const WCHAR g_signatureOriginPartName[] = L"/package/services/digital-signature/origin.psdsor";

HRESULT PrepareForSigning(IOpcFactory* opcFactory, IOpcPackage* opcPackage,
  IOpcSigningOptions* signingOptions, IOpcDigitalSignatureManager* opcDigSigManager,
  PCCERT_CONTEXT cert)
{
  HRESULT hr = signingOptions->SetDefaultDigestMethod(g_defaultDigestMethod);
  if (SUCCEEDED(hr))
  {
    OdString signatureMethodFromCert;
    signatureMethodFromCert = getSignatureMethodFromCert(cert);
    if (!signatureMethodFromCert.isEmpty())
    {
      hr = signingOptions->SetSignatureMethod(signatureMethodFromCert.c_str());
    }
    else
    {
      hr = E_FAIL;
      return hr;
    }
  }

  IOpcPartUri * signatureOriginPartUri = NULL;
  if (SUCCEEDED(hr))
  {
    hr = opcFactory->CreatePartUri(g_signatureOriginPartName, &signatureOriginPartUri);
  }
  if (SUCCEEDED(hr))
  {
    hr = opcDigSigManager->SetSignatureOriginPartName(signatureOriginPartUri);
  }
  if (SUCCEEDED(hr))
  {
    hr = signingOptions->SetCertificateEmbeddingOption(OPC_CERTIFICATE_IN_CERTIFICATE_PART);
  }

  IOpcSignaturePartReferenceSet * partReferenceSet = NULL;
  IOpcSignatureRelationshipReferenceSet * relsReferenceSet = NULL;
  if (SUCCEEDED(hr))    
  {
    hr = signingOptions->GetSignaturePartReferenceSet(&partReferenceSet);
  }  
  if (SUCCEEDED(hr))
  {
    hr = signingOptions->GetSignatureRelationshipReferenceSet(&relsReferenceSet);
  }

  IOpcPartSet * partSet = NULL;
  if (SUCCEEDED(hr))
  {
    hr = opcPackage->GetPartSet(&partSet);
  }
  IOpcPartEnumerator* partEnumerator = NULL;
  BOOL hasNext = FALSE;
  if (SUCCEEDED(hr))
  {
    hr = partSet->GetEnumerator(&partEnumerator);
  }
  while (SUCCEEDED(hr) && SUCCEEDED(hr = partEnumerator->MoveNext(&hasNext)) && hasNext)
  { 
    IOpcPart* currentPart = NULL;
    IOpcPartUri* partUri = NULL;

    IOpcRelationshipSet* relSet = NULL;
    IOpcRelationshipEnumerator* relEnumerator = NULL;
    BOOL hasNextRel = FALSE;

    hr = partEnumerator->GetCurrent(&currentPart);
    if (SUCCEEDED(hr))
    {
      hr = currentPart->GetName(&partUri);
      if (SUCCEEDED(hr))
      {
        hr = partReferenceSet->Create(partUri, NULL, OPC_CANONICALIZATION_NONE, NULL);
      }
      if (SUCCEEDED(hr))
      {
        hr = currentPart->GetRelationshipSet(&relSet);
      }
      if (SUCCEEDED(hr))
      {
        hr = relSet->GetEnumerator(&relEnumerator);

        while (SUCCEEDED(hr) && SUCCEEDED(hr = relEnumerator->MoveNext(&hasNextRel)) && hasNextRel)
        {
          IOpcRelationship* currentRel = NULL;
          IOpcUri* sourceUri = NULL;

          hr = relEnumerator->GetCurrent(&currentRel);
          if (SUCCEEDED(hr))
          {
            hr = currentRel->GetSourceUri(&sourceUri);
          }
          if (SUCCEEDED(hr))
          {
            relsReferenceSet->Create(sourceUri, 
              NULL, OPC_RELATIONSHIP_SIGN_PART, 
              NULL, OPC_CANONICALIZATION_C14N, 
              NULL);
          }

          if (sourceUri)
          {
            sourceUri->Release();
            sourceUri = NULL;
          }
          if (currentRel)
          {
            currentRel->Release();
            currentRel = NULL;
          }
        }
      }
    }

    if (relEnumerator)
    {
      relEnumerator->Release();
      relEnumerator = NULL;
    }
    if (relSet)
    {
      relSet->Release();
      relSet = NULL;
    }
    if (currentPart)
    {
      currentPart->Release();
      currentPart = NULL;
    }
    if (partUri)
    {
      partUri->Release();
      partUri = NULL;
    }
  }

  if (signatureOriginPartUri)
  {
    signatureOriginPartUri->Release();
    signatureOriginPartUri = NULL;
  }
  if (partEnumerator)
  {
    partEnumerator->Release();
    partEnumerator = NULL;
  }
  if (partSet)
  {
    partSet->Release();
    partSet = NULL;
  }
  if (relsReferenceSet)
  {
    relsReferenceSet->Release();
    relsReferenceSet = NULL;
  }
  if (partReferenceSet)
  {
    partReferenceSet->Release();
    partReferenceSet = NULL;
  }

  return hr;
}


HRESULT WritePackageToFile(LPCWSTR filename, IOpcPackage* opcPackage, IOpcFactory* opcFactory)
{
  IStream * fileStream = NULL;
  HRESULT hr = opcFactory->CreateStreamOnFile(
    filename,               // Output file.
    OPC_STREAM_IO_WRITE,
    NULL,                   // Use default security attribute.
    FILE_ATTRIBUTE_NORMAL,  // dwFlagsAndAttributes.
    &fileStream             // The new stream over the file.
    );

  // Write the package to the stream. 
  if (SUCCEEDED(hr))
  {
    hr = opcFactory->WritePackageToStream(
      opcPackage,
      OPC_WRITE_FORCE_ZIP32,
      fileStream
      );
  }

  // Release resources.
  if (fileStream)
  {
    fileStream->Release();
    fileStream = NULL;
  }

  return hr;
}

void RemoveAllSignatures(IOpcPackage* opcPackage, IOpcDigitalSignatureManager* opcDigSigManager)
{
  IOpcDigitalSignatureEnumerator* signatureEnumerator = NULL;
  HRESULT hr = opcDigSigManager->GetSignatureEnumerator(&signatureEnumerator);  
  if (SUCCEEDED(hr))
  {
    BOOL hasNext = FALSE;
    std::list<IOpcDigitalSignature*> signatureList;
    while (SUCCEEDED(hr) && SUCCEEDED(hr = signatureEnumerator->MoveNext(&hasNext)) && hasNext)
    {
      IOpcDigitalSignature* currentSignature = NULL;
      hr = signatureEnumerator->GetCurrent(&currentSignature);
      if (SUCCEEDED(hr))
      {
        signatureList.push_back(currentSignature);
      }      
    }
    std::list<IOpcDigitalSignature*>::iterator sigIt = signatureList.begin();
    for (; sigIt != signatureList.end(); ++sigIt)
    {
      IOpcPartUri* signaturePartName = NULL;
      hr = (*sigIt)->GetSignaturePartName(&signaturePartName);
      if (SUCCEEDED(hr))
      {
        hr = opcDigSigManager->RemoveSignature(signaturePartName);
      }
      if (signaturePartName)
      {
        signaturePartName->Release();
        signaturePartName = NULL;
      }
      if (*sigIt)
      {
        (*sigIt)->Release();
        (*sigIt) = NULL;
      }
    }
  }
  if (signatureEnumerator)
  {
    signatureEnumerator->Release();
    signatureEnumerator = NULL;
  }
}

bool OdDwfxSignatureHandlerWinImpl::removeExistingSignature(const OdString& filePath)
{
  bool res = false;

  COMInit ialize;

  //Initialize an OPC factory:
  IOpcFactory* opcFactory = NULL;
  HRESULT hr = CoCreateInstance(__uuidof(OpcFactory), NULL, 
    CLSCTX_INPROC_SERVER, __uuidof(IOpcFactory), (LPVOID*)&opcFactory);

  if (SUCCEEDED(hr))
  {
    //Process the package:
    bool packageWasSigned = false;

    IOpcPackage* opcPackage = NULL;
    IOpcDigitalSignatureManager* opcDigSigManager = NULL;
    IOpcDigitalSignatureEnumerator* signatureEnumerator = NULL;

    HRESULT hr = ReadPackageFromFile(filePath.c_str(), opcFactory, &opcPackage);
    if (SUCCEEDED(hr))
    {
      hr = opcFactory->CreateDigitalSignatureManager(opcPackage, &opcDigSigManager);
    }
    if (SUCCEEDED(hr))
    {
      hr = opcDigSigManager->GetSignatureEnumerator(&signatureEnumerator);
    }
    if (SUCCEEDED(hr))
    {
      // Check for an existing signature.
      BOOL hasNext = FALSE;
      hr = signatureEnumerator->MoveNext(&hasNext);
      if (SUCCEEDED(hr) && hasNext)
      {
        packageWasSigned = true;

        //Remove an existing signature:
        RemoveAllSignatures(opcPackage, opcDigSigManager);
      }
    }

    OdString tempTargetFilePath = filePath + OD_T(".tmp");
    //write package to temp file if needed:
    if (packageWasSigned && SUCCEEDED(hr))
    {      
      hr = WritePackageToFile(tempTargetFilePath.c_str(), opcPackage, opcFactory);
    }

    //Release all of the package related resources in reverse order
    //to close read-only file stream before overwriting the original file:    
    if (signatureEnumerator)
    {
      signatureEnumerator->Release();
      signatureEnumerator = NULL;
    }
    if (opcDigSigManager)
    {
      opcDigSigManager->Release();
      opcDigSigManager = NULL;
    }
    if (opcPackage)
    {
      ULONG releaseResForPackage = opcPackage->Release();
      ODA_ASSERT(releaseResForPackage == 0);
      opcPackage = NULL;
    }

    //Replace the original file if needed:
    if (packageWasSigned && SUCCEEDED(hr))
    {
      res = CopyFile(tempTargetFilePath.c_str(), filePath.c_str(), FALSE);          
    }
    DeleteFile(tempTargetFilePath.c_str());
  }

  //Release an OPC factory:
  if (opcFactory)
  {
    opcFactory->Release();
    opcFactory = NULL;
  }

  return res;
}

bool OdDwfxSignatureHandlerWinImpl::validatePackageSignature(const OdString& filePath, 
  SignatureValidationResult& validationResult, OdCertParameters& certParams)
{
  bool res = false;

  COMInit ialize;

  //Initialize an OPC factory:
  IOpcFactory* opcFactory = NULL;
  HRESULT hr = CoCreateInstance(__uuidof(OpcFactory), NULL, 
    CLSCTX_INPROC_SERVER, __uuidof(IOpcFactory), (LPVOID*)&opcFactory);

  if (SUCCEEDED(hr))
  {
    //Process the package:

    IOpcPackage* opcPackage = NULL;
    IOpcDigitalSignatureManager* opcDigSigManager = NULL;
    IOpcDigitalSignatureEnumerator* signatureEnumerator = NULL;

    HRESULT hr = ReadPackageFromFile(filePath.c_str(), opcFactory, &opcPackage);
    if (SUCCEEDED(hr))
    {
      hr = opcFactory->CreateDigitalSignatureManager(opcPackage, &opcDigSigManager);
    }
    if (SUCCEEDED(hr))
    {
      hr = opcDigSigManager->GetSignatureEnumerator(&signatureEnumerator);
    }

    // Check for an existing signature.
    BOOL hasNext = FALSE;
    if (SUCCEEDED(hr))
    {
      hr = signatureEnumerator->MoveNext(&hasNext);
    }    
    if (SUCCEEDED(hr) && hasNext)
    {
      BOOL hasPrev = FALSE;
      signatureEnumerator->MovePrevious(&hasPrev);

      //Validate signatures:
      BOOL isValid = FALSE;
      IOpcDigitalSignature* signature = NULL;
      while (SUCCEEDED(hr) && SUCCEEDED(hr = signatureEnumerator->MoveNext(&hasNext)) && hasNext)
      {
        PCCERT_CONTEXT signerCert = NULL;

        hr = signatureEnumerator->GetCurrent(&signature);
        if (SUCCEEDED(hr))
        {
          hr = ValidateSignature(opcDigSigManager, signature, &isValid, &signerCert);
          if (SUCCEEDED(hr))
          {
            if (isValid)
            {
              BOOL isTrusted = FALSE;
              getResultDataForCert(signerCert, certParams, isTrusted);
              if (isTrusted)
                validationResult = kSuccess;
              else
                validationResult = kCertificateChainProblem;

              res = true;
              CertFreeCertificateContext(signerCert);
            }
            else
            {
              validationResult = kInvalidSignature;
              res = true;
              break;
            }
          }
        }
      }
      if (signature)
      {
        signature->Release();
        signature = NULL;
      }
    }
    else
    {
      validationResult = kNotSigned;
      res = true;
    }

    if (signatureEnumerator)
    {
      signatureEnumerator->Release();
      signatureEnumerator = NULL;
    }
    if (opcDigSigManager)
    {
      opcDigSigManager->Release();
      opcDigSigManager = NULL;
    }
    if (opcPackage)
    {
      ULONG releaseResForPackage = opcPackage->Release();
      ODA_ASSERT(releaseResForPackage == 0);
      opcPackage = NULL;
    }
  }

  //Release an OPC factory:
  if (opcFactory)
  {
    opcFactory->Release();
    opcFactory = NULL;
  }

  return res;
}

struct SignatureMethodData
{
  LPCWSTR             userSignatureAlgorithmToCheck; 
  PCCRYPT_OID_INFO    certificateAlgorithmInfo; 
  BOOL                userSignatureAlgorithmSupported; 
};

BOOL WINAPI EnumSignatureMethodCallback (const CRYPT_XML_ALGORITHM_INFO *certMethodInfo, void *userArg)
{
  SignatureMethodData *certificateAlgorithmData = NULL;

  if (NULL != userArg) {
    // Assign user data to local data structure
    certificateAlgorithmData = (SignatureMethodData*)userArg;
  } else {
    // Unable to continue this enumeration 
    //   without data from calling method.
    return FALSE;
  }

  int cmpResult = 0;
  cmpResult = wcscmp(certMethodInfo->wszAlgorithmURI, 
    certificateAlgorithmData->userSignatureAlgorithmToCheck);
  if ( 0 == cmpResult )
  {
    cmpResult = wcscmp(certMethodInfo->wszCNGExtraAlgid, 
      certificateAlgorithmData->certificateAlgorithmInfo->pwszCNGAlgid);
    if ( 0 == cmpResult )
    {
      certificateAlgorithmData->userSignatureAlgorithmSupported = TRUE;      
      return FALSE;
    }
  }
  // The enumeration stops when the callback method returns FALSE. 
  //   If here, then return TRUE because a matching algorithm has
  //   not been found.
  return TRUE;
}

BOOL SupportsSignatureAlgorithm (LPCWSTR signingMethodToCheck, PCCERT_CONTEXT certificateToCheck)
{
  HRESULT hr = S_OK;

  // Initialize the structure that contains the   
  //  information about the signature algorithm to check
  SignatureMethodData certificateAlgorithmData;

  certificateAlgorithmData.userSignatureAlgorithmSupported = FALSE;
  certificateAlgorithmData.userSignatureAlgorithmToCheck = signingMethodToCheck;

  // Call the crypt API to get information about the algorithms
  //   that are supported by the certificate and initialize 
  //   certificateAlgorithmData
  certificateAlgorithmData.certificateAlgorithmInfo = CryptFindOIDInfo(
    CRYPT_OID_INFO_OID_KEY,
    certificateToCheck->pCertInfo->SubjectPublicKeyInfo.Algorithm.pszObjId,
    CRYPT_PUBKEY_ALG_OID_GROUP_ID|CRYPT_OID_PREFER_CNG_ALGID_FLAG);

  if (certificateAlgorithmData.certificateAlgorithmInfo != NULL)
  {
    // Enumerate the algorithms to determine if
    //   the user supplied signature algorithm is supported by 
    //     the certificate.
    //
    // Note that CRYPT_XML_GROUP_ID_SIGN is used to enumerate
    //  the signature methods
    hr = CryptXmlEnumAlgorithmInfo(
      CRYPT_XML_GROUP_ID_SIGN,  // NOTE: CRYPT_XML_GROUP_ID_SIGN
      CRYPT_XML_FLAG_DISABLE_EXTENSIONS,
      (void*)&certificateAlgorithmData,
      EnumSignatureMethodCallback);
    // when the enumeration has returned successfully, 
    //  certificateAlgorithmData.userSignatureAlgorithmSupported
    //  will be TRUE if the signing method is supported by
    //  the certificate
  }
  return certificateAlgorithmData.userSignatureAlgorithmSupported;
}

OdString getSignatureMethodFromCert(PCCERT_CONTEXT cert)
{
  OdString res;

  //Check the URIs natively supported by CryptXML
  //(see https://technet.microsoft.com/en-us/security/dd979768(v=vs.80)):

  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_RSA_SHA1, cert))
    return wszURI_XMLNS_DIGSIG_RSA_SHA1;//wszURI_XMLNS_DIGSIG_RSA_SHA1
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_DSA_SHA1, cert))
    return wszURI_XMLNS_DIGSIG_DSA_SHA1;//wszURI_XMLNS_DIGSIG_DSA_SHA1
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_RSA_SHA256, cert))
    return wszURI_XMLNS_DIGSIG_RSA_SHA256;//wszURI_XMLNS_DIGSIG_RSA_SHA256
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_RSA_SHA384, cert))
    return wszURI_XMLNS_DIGSIG_RSA_SHA384;//wszURI_XMLNS_DIGSIG_RSA_SHA384
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_RSA_SHA512, cert))
    return wszURI_XMLNS_DIGSIG_RSA_SHA512;//wszURI_XMLNS_DIGSIG_RSA_SHA512
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_ECDSA_SHA1, cert))
    return wszURI_XMLNS_DIGSIG_ECDSA_SHA1;//wszURI_XMLNS_DIGSIG_ECDSA_SHA1
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_ECDSA_SHA256, cert))
    return wszURI_XMLNS_DIGSIG_ECDSA_SHA256;//wszURI_XMLNS_DIGSIG_ECDSA_SHA256
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_ECDSA_SHA384, cert))
    return wszURI_XMLNS_DIGSIG_ECDSA_SHA384;//wszURI_XMLNS_DIGSIG_ECDSA_SHA384
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_ECDSA_SHA512, cert))
    return wszURI_XMLNS_DIGSIG_ECDSA_SHA512;//wszURI_XMLNS_DIGSIG_ECDSA_SHA512
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_HMAC_SHA1, cert))
    return wszURI_XMLNS_DIGSIG_HMAC_SHA1;//wszURI_XMLNS_DIGSIG_HMAC_SHA1
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_HMAC_SHA256, cert))
    return wszURI_XMLNS_DIGSIG_HMAC_SHA256;//wszURI_XMLNS_DIGSIG_HMAC_SHA256
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_HMAC_SHA384, cert))
    return wszURI_XMLNS_DIGSIG_HMAC_SHA384;//wszURI_XMLNS_DIGSIG_HMAC_SHA384
  if (SupportsSignatureAlgorithm(wszURI_XMLNS_DIGSIG_HMAC_SHA512, cert))
    return wszURI_XMLNS_DIGSIG_HMAC_SHA512;//wszURI_XMLNS_DIGSIG_HMAC_SHA512  
  
  return res;
}

HRESULT ValidateSignature(IOpcDigitalSignatureManager* opcDigSigManager,
  IOpcDigitalSignature* signature, BOOL* isValid, PCCERT_CONTEXT* signerCert)
{
  *isValid = FALSE;
  *signerCert = NULL;
  HRESULT hr = S_OK;
  IOpcCertificateEnumerator * certEnumerator = NULL;

  // Get an enumerator of certificates related to the signature.
  hr = signature->GetCertificateEnumerator(&certEnumerator);

  // There may be multiple certificates related to a signature, but only one
  // is the certificate that was used to generate the signature (the signer
  // certificate). Package signature validation requires this signer
  // certificate; therefore, we will enumerate the certificates related to
  // the signature and try to validate the signature with each certificate.
  // When validation succeeds, the certificate that was used must be the
  // signer certificate.  
  UINT32 certCount = 0;
  BOOL hasNext = TRUE;
  while (SUCCEEDED(hr) && SUCCEEDED(hr = certEnumerator->MoveNext(&hasNext)) && hasNext)
  {
    OPC_SIGNATURE_VALIDATION_RESULT result;
    PCCERT_CONTEXT cert = NULL;

    certCount++;

    // The contract of GetCurrent ensures that if the method fails,
    // it will not return a valid CERT_CONTEXT. If it succeeds,
    // CertFreeCertificateContext is called to free the CERT_CONTEXT;
    // unless it is being returned to the caller in signerCert.
    hr = certEnumerator->GetCurrent(&cert);

    if (FAILED(hr))
    {
      // Filter with known possible errors, so that we can still continue
      // safely.
      UINT32 facilityCode = HRESULT_FACILITY(hr);

      if (facilityCode == FACILITY_SECURITY || facilityCode == FACILITY_OPC)
      {
        // The error may be due to a corrupted certificate, a bad
        // certificate relationship, or a missing certificate part.        

        hr = S_OK;  // Reset hr to continue with the loop.
        continue;
      }
      else
      {
        // Do not continue for unanticipated or critical errors, such
        // as E_OUTOFMEMORY.
        break;
      }
    }

    // Verify that the signature is valid, meaning that no signed content 
    // has been modified.
    hr = opcDigSigManager->Validate(signature, cert, &result);

    if (SUCCEEDED(hr))
    {
      if (result == OPC_SIGNATURE_VALID)
      {
        // Found the signer certificate and confirmed that the
        // signature is valid.
        *isValid = TRUE;
        *signerCert = cert;
        break;
      }
    }
    else
    {
      // The HRESULT from IOpcDigitalSignatureManager::Validate()
      // describes why validation failed. 
      UINT32 facilityCode = HRESULT_FACILITY(hr);

      if (facilityCode == FACILITY_SECURITY)
      {
        // The error may have been caused by attempting to validate
        // the signature with a certificate that is not the signer
        // certificate. Try the next available certificate.

        hr = S_OK;  // Reset hr to continue with the loop.
      }
      // Do not continue for unanticipated or critical errors, such
      // as E_OUTOFMEMORY.
    }
    // Free the CERT_CONTEXT that was obtained from the certificate enumerator.
    CertFreeCertificateContext(cert);
  }

  // Release resources
  if (certEnumerator)
  {
    certEnumerator->Release();
    certEnumerator = NULL;
  }

  return hr;
}

bool getObtainedCertTrustStatus(PCCERT_CONTEXT pDesiredCert, 
  PCERT_TRUST_STATUS pCertTrustStatus)
{
  bool resultCode = false;

  HCERTCHAINENGINE hChainEngine = NULL;
  CERT_CHAIN_ENGINE_CONFIG ChainConfig;

  HCERTSTORE hSysStore = NULL;  
  PCCERT_CHAIN_CONTEXT pChainContext = NULL;

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
    hSysStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING), 
      NULL, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
    if (hSysStore)
    {
      if (pDesiredCert)
      {
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

        if (CertGetCertificateChain(hChainEngine, pDesiredCert, NULL, 
          pDesiredCert->hCertStore, &ChainPara, dwFlags, NULL, &pChainContext))
        {
          *pCertTrustStatus = pChainContext->TrustStatus;
          resultCode = true;
        }
      }
    }
  }

  if (hChainEngine)
    CertFreeCertificateChainEngine(hChainEngine);
  if (pChainContext)
    CertFreeCertificateChain(pChainContext);  
  if (hSysStore)
    CertCloseStore(hSysStore, 0);

  return resultCode;
}

void getResultDataForCert(PCCERT_CONTEXT pCert, OdCertParameters& certParams, BOOL& isTrusted)
{
  if (pCert)
  {
    //get subject name:
    wchar_t subjectName[256];
    CertGetNameStringW(pCert, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, subjectName, 256);

    //get issuer name:
    wchar_t issuerName[256];
    CertGetNameStringW(pCert, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, issuerName, 256);

    //get serial number:
    wchar_t* serialNumberFromCert;
    int serialNumberStringLen = 0;
    int sizeOfSerialNumberBLOB = pCert->pCertInfo->SerialNumber.cbData;
    serialNumberStringLen = sizeOfSerialNumberBLOB * 2;
    serialNumberFromCert = new wchar_t[serialNumberStringLen + 1];
    serialNumberFromCert[serialNumberStringLen] = 0;
    int j = serialNumberStringLen - 1;
    for (int i = 0; i < sizeOfSerialNumberBLOB; i++)
    {
      wchar_t strRepresentationOfByte[3];
      strRepresentationOfByte[2] = 0;
      odSprintf(strRepresentationOfByte, 3, L"%02x", pCert->pCertInfo->SerialNumber.pbData[i]);
      serialNumberFromCert[j] = strRepresentationOfByte[1];
      serialNumberFromCert[j - 1] = strRepresentationOfByte[0];
      j = j - 2;
    }

    //get "Valid from" date:
    FILETIME ft = pCert->pCertInfo->NotBefore;
    SYSTEMTIME st;
    wchar_t validFrom[255];
    FileTimeToLocalFileTime( &ft, &ft );
    FileTimeToSystemTime( &ft, &st );
    GetDateFormatW( LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, NULL, validFrom, 255 );
    OdString validFromDate(validFrom);
    //get "Valid to" date:
    ft = pCert->pCertInfo->NotAfter;
    wchar_t validTo[255];
    FileTimeToLocalFileTime( &ft, &ft );
    FileTimeToSystemTime( &ft, &st );
    GetDateFormatW( LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, NULL, validTo, 255 );
    OdString validToDate(validTo);

    //ret cert params:
    certParams.sCertSubject = subjectName;
    certParams.sCertIssuer = issuerName;
    certParams.sCertSerialNum = serialNumberFromCert;
    certParams.m_CertValidFrom = validFromDate;
    certParams.m_CertValidTo = validToDate;

    delete[] serialNumberFromCert;
  }

  //check the certification path:
  bool bNoErrors = false;
  CERT_TRUST_STATUS certTrustStatus;
  bNoErrors = getObtainedCertTrustStatus(pCert, &certTrustStatus);
  if ((bNoErrors) && (certTrustStatus.dwErrorStatus == CERT_TRUST_NO_ERROR))
    isTrusted = TRUE;
  else
    isTrusted = FALSE;
}

