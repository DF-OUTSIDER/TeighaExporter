/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "OdDbCryptImpl.h"
#include "RxObjectImpl.h"

#include "StringArray.h"

#include "DynamicLinker.h"
#include "OdTimeProvider/OdTimeProvider.h"

ODRX_CONS_DEFINE_MEMBERS(OdDbCryptImpl, OdDbCrypt, RXIMPL_CONSTR);

OdDbCryptImpl::OdDbCryptImpl()
  : m_trustedCertIsFound(false)
{
  OdRxClassPtr pService = odrxServiceDictionary()->getAt(OD_T("OdCryptoServices"));
  if (!pService.isNull())
  {
    m_pCryptoServices = pService->create();
  }
}

OdDbCryptImpl::~OdDbCryptImpl()
{
  clear();
}

void OdDbCryptImpl::clear()
{
  if (!m_pCryptoServices.isNull())
    m_pCryptoServices->resetSessionKeyToEncryptData();

  m_trustedCertIsFound = false;
  m_dataBlocksArray.clear();
}

//Conversion between UTF16 and UTF32 functions:
OdUInt32 OD_TOOLKIT_IMPORT uint16StrLen(const OdUInt16* pString);

OdBinaryData OD_TOOLKIT_IMPORT getUTF16BinaryDataFromUTF32Str(const OdString& str);
OdString OD_TOOLKIT_IMPORT getUTF32StrFromUTF16BinaryData(const OdBinaryData& data);
//

bool OdDbCryptImpl::initialize(const OdSecurityParams& secParams)
{
  clear();

  if (m_pCryptoServices.isNull())
    return false;

  if ( ((secParams.nFlags & SECURITYPARAMS_ENCRYPT_DATA) ||
    (secParams.nFlags & SECURITYPARAMS_ENCRYPT_PROPS))
    &&
    (!secParams.password.isEmpty())
    )
  {
    OdBinaryData passwordBinData;
    //prepare password binary data without termination symbol:
    if (4 == sizeof(OdChar))
    {
      passwordBinData = getUTF16BinaryDataFromUTF32Str(secParams.password);
      OdUInt32 lenWithoutTerminationSymbol = uint16StrLen((const OdUInt16*)passwordBinData.asArrayPtr());
      passwordBinData.resize(lenWithoutTerminationSymbol * sizeof(OdUInt16));
    }
    else
    {
      OdUInt32 sizeInBytes = (secParams.password.getLength())*sizeof(OdChar);
      passwordBinData.resize(sizeInBytes);
      ::memcpy(passwordBinData.asArrayPtr(), secParams.password.c_str(), sizeInBytes);
    }

    if (!m_pCryptoServices->initSessionKeyToEncryptData(passwordBinData,
      secParams.nProvType, secParams.provName, secParams.nAlgId, secParams.nKeyLength))
      return false;
  }

  //Initialize to sign the data:
  if (secParams.nFlags & SECURITYPARAMS_SIGN_DATA)
  {
    OdCertificateShortDesc certShortDesc;
    certShortDesc.m_CertSubject = secParams.sCertSubject;
    certShortDesc.m_CertIssuer = secParams.sCertIssuer;
    certShortDesc.m_CertSerialNum = secParams.sCertSerialNum;

    OdCertificateObjectPtr pCertObj = m_pCryptoServices->getCertObjByShortDesc(certShortDesc);
    if ( (!pCertObj.isNull()) && (pCertObj->isTrusted()) )
      m_trustedCertIsFound = true;
  }
  
  return true;
}


// Decrypt data. 
bool OdDbCryptImpl::decryptData(OdUInt8* buffer, OdUInt32 nBufferSize)
{
  bool res = false;

  if (!m_pCryptoServices.isNull())
  {
    res = m_pCryptoServices->decryptData(buffer, nBufferSize);
  }

  return res;
}

// Encrypt data. 
bool OdDbCryptImpl::encryptData(OdUInt8* buffer, OdUInt32 nBufferSize)
{
  bool res = false;

  if (!m_pCryptoServices.isNull())
  {
    res = m_pCryptoServices->encryptData(buffer, nBufferSize);
  }

  return res;
}

bool OdDbCryptImpl::digitalSignatureModeLaunched()
{
  return m_trustedCertIsFound;
}

void OdDbCryptImpl::appendDataToProcessSignature(const OdBinaryData& dataBlock)
{
  m_dataBlocksArray.append(dataBlock);
}

OdBinaryData generateAttribDataBlock(const OdString& attribStringData)
{
  OdBinaryData attrDataBlock;

  OdUInt32 sizeOfOdChar = sizeof(OdChar);
  if (sizeOfOdChar == 4)
  {
    attrDataBlock = getUTF16BinaryDataFromUTF32Str(attribStringData);
  }
  else
  {
    OdUInt32 blockSizeInBytes = (attribStringData.getLength() + 1)*sizeof(OdChar);
    attrDataBlock.resize(blockSizeInBytes);
    ::memcpy(attrDataBlock.asArrayPtr(), attribStringData.c_str(), blockSizeInBytes);
  }

  return attrDataBlock;
}

OdSubjectKeyIdAttribPtr OdDbCryptImpl::generateSubjectKeyIdentifierAttr(const OdString& attribStringData)
{
  OdSubjectKeyIdAttribPtr pAttrib;

  OdBinaryData strValueAsBinData;

  strValueAsBinData = generateAttribDataBlock(attribStringData);

  pAttrib = m_pCryptoServices->newSubjectKeyIdentifierAttr(strValueAsBinData);

  return pAttrib;
}

bool OdDbCryptImpl::signData(const OdSecurityParams& securityParams, OdBinaryData& signatureBlock)
{
  bool res = false;

  if (!m_pCryptoServices.isNull())
  {
    OdCertificateObjectPtr pCertObj;
    OdCertificateShortDesc certShortDesc;
    certShortDesc.m_CertSubject = securityParams.sCertSubject;
    certShortDesc.m_CertIssuer = securityParams.sCertIssuer;
    certShortDesc.m_CertSerialNum = securityParams.sCertSerialNum;
    pCertObj = m_pCryptoServices->getCertObjByShortDesc(certShortDesc);

    OdSubjectKeyIdAttribPtr pAttrib;
    if ( (securityParams.nFlags & SECURITYPARAMS_ADD_TIMESTAMP) || (!securityParams.sComment.isEmpty()) )
    { 
      OdString attribStringData;

      if (securityParams.nFlags & SECURITYPARAMS_ADD_TIMESTAMP)
      {
        OdTimeProviderPtr pTimeProvider = odrxDynamicLinker()->loadApp(OdTimeProviderModuleName);
        if (!pTimeProvider.isNull())
        {
          OdTimeStamp timeStamp;
          pTimeProvider->getTimeFromServer(securityParams.sTimeServer, timeStamp);

          OdString timeStampString;
          OdString timeFromLocalComputerStr(OD_T("Time from local computer"));

#if defined(ODA_WINDOWS)
          timeStamp.strftime(OD_T("%#m/%#d/%Y/%#H/%#M/%#S/"), timeStampString);
#else
          short mon, day, year;
          timeStamp.getDate(mon, day, year);
          short hour, min, sec, msec;
          timeStamp.getTime(hour, min, sec, msec);
          timeStampString.format(OD_T("%d/%d/%d/%d/%d/%d/"), mon, day, year, hour, min, sec);
#endif

          timeStampString += (!securityParams.sTimeServer.isEmpty()) ? securityParams.sTimeServer :
            timeFromLocalComputerStr;
                    
          OdString timeStampAttribString;          
          timeStampAttribString.format(OD_T("%d;"), timeStampString.getLength());
          timeStampAttribString += timeStampString;

          attribStringData += timeStampAttribString;
        }
        else
        {
          attribStringData += OD_T("0;");
        }
      }
      else
      {
        attribStringData += OD_T("0;");
      }

      attribStringData += securityParams.sComment;

      OdBinaryData attribDataBlock = generateAttribDataBlock(attribStringData);
      m_dataBlocksArray.append(attribDataBlock);

      pAttrib = generateSubjectKeyIdentifierAttr(attribStringData);      
    }

    OdCryptSignMessageParaPtr pSignPara = m_pCryptoServices->newCryptSignMessagePara(pCertObj, pAttrib);

    if (m_pCryptoServices->generateDetachedSignature(pSignPara, m_dataBlocksArray, signatureBlock))
      res = true;
  }

  return res;
}

bool getStringFromSubjectKeyIdentifierAttr(OdSubjectKeyIdAttrib* pCryptAttrib, OdString& attribStringData)
{
  bool res = false;

  OdBinaryData data;
  pCryptAttrib->getData(data);
  if (data.size())
  {
    OdUInt32 sizeOfOdChar = sizeof(OdChar);
    if (sizeOfOdChar == 4)
    {
      attribStringData = getUTF32StrFromUTF16BinaryData(data);
    }
    else
    {
      const OdChar* pBuf = (const OdChar*)data.asArrayPtr();
      attribStringData = OdString(pBuf);
    }
    
    res = true;
  }

  return res;
}

void OdDbCryptImpl::getSignatureAttributes(const OdBinaryData& signatureBlock, 
  OdString& signatureTimestamp, OdString& signatureComment, OdBinaryData& attribBlock)
{
  OdSubjectKeyIdAttribPtr pAttr;
  pAttr = m_pCryptoServices->getSubjectKeyIdFromDetachedSignature(signatureBlock);
  if (!pAttr.isNull())
  {
    OdString attribStringData;
    if (getStringFromSubjectKeyIdentifierAttr(pAttr, attribStringData))
    {
      OdUInt32 delimiterPos = attribStringData.find(OD_T(";"));
      if (delimiterPos != -1)
      {
        OdString timeStampSymbolsCounterStr = attribStringData.left(delimiterPos);
        OdString rightString = attribStringData.right(attribStringData.getLength() - (delimiterPos + 1));
        OdUInt32 timeStampSymbolsCounter = odStrToUInt(timeStampSymbolsCounterStr);
        if (timeStampSymbolsCounter)
        {
          signatureTimestamp = rightString.left(timeStampSymbolsCounter);
          rightString = rightString.right(rightString.getLength() - timeStampSymbolsCounter);
        }
        if (rightString.getLength())
        {
          signatureComment = rightString;
        }
      }
      attribBlock = generateAttribDataBlock(attribStringData);
    }
  }
}

OdStringArray splitStringBySeparator(const OdString& src, const OdString& separator)
{
  OdStringArray res;

  OdString currentStr = src;
  int pos = 0;
  while ((pos = currentStr.find(separator)) != -1)
  {
    OdString subStr = currentStr.left(pos);
    res.push_back(subStr);

    currentStr = currentStr.right(currentStr.getLength() - (pos + 1));
  }
  res.push_back(currentStr);

  return res;
}

void splitDateTimeAndTimeService(const OdString& timeStampString, OdString& dateAndTimeString, OdString& timeServiceString)
{
  OdStringArray subStrArray = splitStringBySeparator(timeStampString, OD_T("/"));
  if (subStrArray.size() == 7)
  {
    OdTimeStamp timeStamp;

    timeStamp.setDate(odStrToUInt(subStrArray[0].c_str()), 
                      odStrToUInt(subStrArray[1].c_str()), 
                      odStrToUInt(subStrArray[2].c_str()));
    timeStamp.setTime(odStrToUInt(subStrArray[3].c_str()), 
                      odStrToUInt(subStrArray[4].c_str()), 
                      odStrToUInt(subStrArray[5].c_str()), 0);
    //timeStamp.universalToLocal();

#if defined(ODA_WINDOWS)
    timeStamp.universalToLocal();
    timeStamp.strftime(OD_T("%c %z"), dateAndTimeString);
#else
    timeStamp.ctime(dateAndTimeString);
#endif   
    
    timeServiceString = subStrArray[6].c_str();
  }
}

void OdDbCryptImpl::verifyDataSignature(const OdBinaryData& signatureBlock, 
  OdCryptoServices::OdSignatureVerificationResult& verificationResult, 
  OdSignatureDescription& signatureDesc)
{
  if (!m_pCryptoServices.isNull())
  {
    OdCertificateObjectPtr pCertObj = m_pCryptoServices->getCertFromDetachedSignature(signatureBlock);
    if (!pCertObj.isNull())
      signatureDesc.m_certDesc = pCertObj->getCertDescription();

    OdString signatureTimestamp;
    OdString signatureComment;
    OdBinaryData attribBlock;
    getSignatureAttributes(signatureBlock, signatureTimestamp, signatureComment, attribBlock);
    if (attribBlock.size())
    {
      m_dataBlocksArray.append(attribBlock);
      
      splitDateTimeAndTimeService(signatureTimestamp, 
        signatureDesc.m_signatureDateAndTime, signatureDesc.m_timeServiceUsed);

      signatureDesc.m_comment = signatureComment;
    }

    m_pCryptoServices->verifyDetachedSignature(m_dataBlocksArray, signatureBlock, verificationResult);
  }
}
