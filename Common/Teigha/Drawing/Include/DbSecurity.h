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

#ifndef _ODDBSECURITY_INCLUDED_
#define _ODDBSECURITY_INCLUDED_

#include "TD_PackPush.h"

#include "RxObject.h"
#include "OdArray.h"
#include "DbExport.h"
#include "OdString.h"
#include "OdBinaryData.h"

#include "OdCryptoServices/OdCryptoServices.h"

enum
{
  SECURITYPARAMS_ENCRYPT_DATA     = 0x00000001,
  SECURITYPARAMS_ENCRYPT_PROPS    = 0x00000002,

  SECURITYPARAMS_SIGN_DATA        = 0x00000010,
  SECURITYPARAMS_ADD_TIMESTAMP    = 0x00000020,

  SECURITYPARAMS_ALGID_RC4        = 0x00006801
};


typedef OdString OdPassword;

/** \details
    This class represents the security parameters used by OdDbCrypt classes.
    
    Corresponding C++ library: TD_Db
    <group Other_Classes>
*/
class OdSecurityParams
{
public:
  OdSecurityParams()
    : nFlags(0)
    , nProvType(0)
    , nAlgId (SECURITYPARAMS_ALGID_RC4)
    , nKeyLength(40)
  {}

  OdUInt32    nFlags;
  OdPassword  password;
  OdUInt32    nProvType;
  OdString   provName;
  OdUInt32    nAlgId;
  OdUInt32    nKeyLength;

  OdString   sCertSubject;
  OdString   sCertIssuer;
  OdString   sCertSerialNum;
  OdString   sComment;
  OdString   sTimeServer;
};

/** \details
  <group Other_Classes>
  This class defines signature description.
*/
class OdSignatureDescription : public OdSignatureBaseDescription
{
public:
  OdString m_signatureDateAndTime;
  OdString m_timeServiceUsed;

  OdString m_comment;
};

/** \details
    This class defines the interface for the 
    encription/decription of byte data.
    
    Corresponding C++ library: TD_Db
    <group Other_Classes>
*/
class TOOLKIT_EXPORT OdDbCrypt : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCrypt);

  /** \details
    Initializes this OdDbCrypt object with the specified security parameters.
    \param securityParams [in]  Security parameters. 
    \returns
    Returns true if and only if successful.
  */
  virtual bool initialize(const OdSecurityParams& securityParams) = 0;
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

  /** \details
     \returns
     Returns true if and only if trusted certificate was found during OdDbCrypt initialization.
     It means the signing is possible using the certificate specified by OdSecurityParams.
     The default implementation returns false. You should override this method when implementing the interface.
  */
  virtual bool digitalSignatureModeLaunched();

  /** \details
     Appends a data block to be signed/hashed.
     This method should be used to specify the data to be processed 
     by signData() or verifyDataSignature().

     \param dataBlock [in] Data block to be signed/hashed.
  */
  virtual void appendDataToProcessSignature(const OdBinaryData& dataBlock) = 0;

  /** \details
     Creates a hash of the data blocks appended by appendDataToProcessSignature() method, 
     signs the hash and returns the signed hash (detached signature).

     \param securityParams [in] Certificate and signature settings.
     \param signatureBlock [out] Signed hash.

     \returns
     Returns true if the function succeeds.
     If the function fails, the return value is false.
  */
  virtual bool signData(const OdSecurityParams& securityParams, OdBinaryData& signatureBlock) = 0;

  /** \details
     Verifies a detached signature using data blocks appended by appendDataToProcessSignature() method.

     \param signatureBlock [in] Detached signature.
     \param verificationResult [out] Verification result.
     \param signatureDesc [out] Structure into which signature description data will be placed.
  */
  virtual void verifyDataSignature(const OdBinaryData& signatureBlock, 
    OdCryptoServices::OdSignatureVerificationResult& verificationResult, 
    OdSignatureDescription& signatureDesc) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbCrypt object pointers.
*/
typedef OdSmartPtr<OdDbCrypt> OdDbCryptPtr;

/** \details
    This class implements Iterator objects that traverse entries in OdPwdCache objects.

    <group Other_Classes>
*/
class TOOLKIT_EXPORT OdPwdIterator : public OdRxObject
{
public:
  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() const = 0;
  /** \details
    Sets this Iterator object to reference the entry following the current entry.
  */
  virtual void next() = 0;
  /** \details
    Returns the Password object pointed to by this Iterator object. 

    \param password [out]  Receives the Password object.
  */
  virtual void get(OdPassword& password) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdPwdIterator object pointers.
*/
typedef OdSmartPtr<OdPwdIterator> OdPwdIteratorPtr;

/** \details
  This class implements and manages a Password Cache.
    <group Other_Classes>
*/
class TOOLKIT_EXPORT OdPwdCache : public OdRxObject
{
public:
  /** \details
    Adds the specified Password object to this Cache object.

    \param password [in]  Password object.
  */
  virtual void add(const OdPassword& password) = 0;
  /** \details
    Returns an Iterator object that can be 
    used to traverse the OdPassword objects in this Stack object.
  */
  virtual OdPwdIteratorPtr newIterator() = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdPwdCache object pointers.
*/
typedef OdSmartPtr<OdPwdCache> OdPwdCachePtr;

#include "TD_PackPop.h"

#endif  // _ODDBSECURITY_INCLUDED_
