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

//OdCryptoServices - interface that provides services to enable developers to secure applications using cryptography

#ifndef _ODCRYPTOSERVICES_H__458F3BE0_8868_489B_B1D1_B1102D86667D__INCLUDED_
#define _ODCRYPTOSERVICES_H__458F3BE0_8868_489B_B1D1_B1102D86667D__INCLUDED_

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"
#include "OdArray.h"
#include "OdString.h"
#include "OdBinaryData.h"
#include "OdLinkedArray.h"

/** \details
	<group Other_Classes>
*/
class OdCertificateShortDesc
{
public:
  OdString m_CertSubject;
  OdString m_CertIssuer;
  OdString m_CertSerialNum;  
};

/** \details
	<group Other_Classes>
*/
class OdCertificateDescription : public OdCertificateShortDesc
{
public:
  OdString m_CertValidFrom;
  OdString m_CertValidTo;
};

/** \details
    This class defines the interface for certificate object.
	<group Other_Classes>
*/
class OdCertificateObject
{
public:
  virtual ~OdCertificateObject() {}

  /** \details
    Returns certificate description.    
  */
  virtual OdCertificateDescription getCertDescription() const = 0;

  /** \details
    Returns certificate trust status.
  */
  virtual bool isTrusted() const = 0;
};

typedef OdSharedPtr<OdCertificateObject> OdCertificateObjectPtr;

/** \details
    This class defines the interface for subject key identifier attribute.
	<group Other_Classes>
*/
class OdSubjectKeyIdAttrib
{
public:
  virtual ~OdSubjectKeyIdAttrib() {}

  /** \details
    Returns value of attribute as OdBinaryData.
    \param dataOfAttrValue [out]  attribute value.
  */
  virtual void getData(OdBinaryData& dataOfAttrValue) const = 0;
};

typedef OdSharedPtr<OdSubjectKeyIdAttrib> OdSubjectKeyIdAttribPtr;

/** \details
    This is a base class for data structure used to pass signature parameters 
    while signing with OdCryptoServices::generateDetachedSignature.
	<group Other_Classes>
*/
class OdCryptSignMessagePara
{
protected:
  OdCryptSignMessagePara() {}
public:
  virtual ~OdCryptSignMessagePara() {}
};

typedef OdSharedPtr<OdCryptSignMessagePara> OdCryptSignMessageParaPtr;

/** \details
    This class defines base signature description.
	<group Other_Classes>
*/
class OdSignatureBaseDescription
{
public:
  virtual ~OdSignatureBaseDescription() {}

  OdCertificateDescription m_certDesc;
};

/** \details
    This class defines the interface of crypto services.
	<group Other_Classes>
*/
class FIRSTDLL_EXPORT OdCryptoServices : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdCryptoServices);

  /** \details
    Fills an array of descriptions with data of all trusted certificates     
    presented in certificate store for which the private key is available.

    \param certificates [out]  descriptions array.

    \returns
    Returns number of certificates obtained.
  */
  virtual OdUInt32 getPersonalCertsWithTrustedStatus(OdArray<OdCertificateDescription>& certificates) const = 0;

  /** \details    
    \param certShortDesc [in]  certificate short description.

    \returns
    Returns certificate object corresponding to short description.
  */
  virtual OdCertificateObjectPtr getCertObjByShortDesc(const OdCertificateShortDesc& certShortDesc) const = 0;

  enum OdSignatureVerificationResult
  {
    kSuccess = 0,
    kHasNoSignature,
    kBadSignature,
    kCertificateChainProblem,
    kBadAlgId,
    kNoSigner,
    kUnexpectedMsgType,
    kInvalidArg,
    kUnknownError
  };

  /** \details
    Creates an attribute Subject Key Identifier using binary data as a value.

    \param dataOfAttrValue [in]  binary data for attribute value.

    \returns
    Returns created attribute.
  */
  virtual OdSubjectKeyIdAttribPtr newSubjectKeyIdentifierAttr(const OdBinaryData& dataOfAttrValue) = 0;

  /** \details
    Creates signature parameters object using certificate and attribute Subject Key Identifier.    

    \param pCertObj [in]  reference to certificate object pointer.
    \param pSubjectKeyIdAttr [in]  reference to attribute object pointer (pointer itself may be NULL due to attribute is optional).

    \returns
    Returns created signature parameters.
  */
  virtual OdCryptSignMessageParaPtr newCryptSignMessagePara(const OdCertificateObjectPtr& pCertObj,
    const OdSubjectKeyIdAttribPtr& pSubjectKeyIdAttr) = 0;

  /** \details
    Generates detached signature for the message using signature parameters passed.

    \param pSignPara [in]  signature parameters.
    \param message [in]  data to be signed.
    \param signature [out] generated signature.

    \returns
    Returns true if success, otherwise returns false.
  */
  virtual bool generateDetachedSignature(const OdCryptSignMessageParaPtr& pSignPara, 
    const OdLinkedArray<OdBinaryData>& message, OdBinaryData& signature) = 0;

  /** \details
    Verifies detached signature for the message.

    \param message [in]  message data.
    \param signature [in]  message signature.
    \param verificationResult [out] result of verification.
  */
  virtual void verifyDetachedSignature(const OdLinkedArray<OdBinaryData>& message, const OdBinaryData& signature, 
    OdSignatureVerificationResult& verificationResult) = 0;


  /** \details
    Obtains signing certificate from the signature data block.

    \param signature [in]  signature data block.

    \returns
    Returns certificate object.
  */
  virtual OdCertificateObjectPtr getCertFromDetachedSignature(const OdBinaryData& signature) = 0;

  /** \details
    Obtains an existing attribute Subject Key Identifier from the signature data block.

    \param signature [in]  signature data block.

    \returns
    Returns attribute object.
  */
  virtual OdSubjectKeyIdAttribPtr getSubjectKeyIdFromDetachedSignature(const OdBinaryData& signature) = 0;


  /** \details
    Initializes a session key structures to encrypt/decrypt data using key derived from password.

    \param passwordAsBinaryData [in]  password as binary data.
    \param nProvType [in]  crypto provider type.
    \param provName [in]  crypto provider name.
    \param nAlgId [in]  algorithm identifier.
    \param nKeyLength [in]  key length.

    \returns
    Returns true if success, otherwise returns false.
  */
  virtual bool initSessionKeyToEncryptData(const OdBinaryData& passwordAsBinaryData, 
    OdUInt32 nProvType, const OdString& provName, OdUInt32 nAlgId, OdUInt32 nKeyLength) = 0;

  /** \details
    Reset session key structures.
  */
  virtual void resetSessionKeyToEncryptData() = 0;

  /** \details
    Encrypts the specified buffer.
    
    \param bufferSize [in]  Number of bytes.
    \param buffer [in/out] Data to be encrypted.
    \returns
    Returns true if and only if successful.
  */
  virtual bool encryptData(OdUInt8* buffer, OdUInt32 bufferSize) = 0;

  /** \details
    Decrypts the specified buffer.
    
    \param bufferSize [in]  Number of bytes.
    \param buffer [in/out] Data to be decrypted.
    \returns
    Returns true if and only if successful.
  */
  virtual bool decryptData(OdUInt8* buffer, OdUInt32 bufferSize) = 0;
};

typedef OdSmartPtr<OdCryptoServices> OdCryptoServicesPtr;


#include "TD_PackPop.h"

#endif
