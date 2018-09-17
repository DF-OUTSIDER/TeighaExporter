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

#ifndef _ODDBCRYPTIMPL_H_
#define _ODDBCRYPTIMPL_H_

#include "TD_PackPush.h"

#include "DbSecurity.h"
#include "RxDictionary.h"
#include "OdString.h"
#include "OdBinaryData.h"
#include "OdLinkedArray.h"

/**
*/
class OdDbCryptImpl : public OdDbCrypt
{
  OdCryptoServicesPtr m_pCryptoServices;
  bool                m_trustedCertIsFound;

  OdLinkedArray<OdBinaryData> m_dataBlocksArray;

public:
  ODRX_DECLARE_MEMBERS(OdDbCryptImpl);

  OdDbCryptImpl();
  ~OdDbCryptImpl();
 /** \details
    Clears this OdDbCrypt object.
 */
  void clear();

  bool initialize(const OdSecurityParams& securityParams);
  bool decryptData(OdUInt8* buffer, OdUInt32 bufferSize);
  bool encryptData(OdUInt8* buffer, OdUInt32 bufferSize);

  bool digitalSignatureModeLaunched();
  void appendDataToProcessSignature(const OdBinaryData& dataBlock);

  bool signData(const OdSecurityParams& securityParams, OdBinaryData& signatureBlock);

  void verifyDataSignature(const OdBinaryData& signatureBlock, 
    OdCryptoServices::OdSignatureVerificationResult& verificationResult, 
    OdSignatureDescription& signatureDesc);

protected:
  OdSubjectKeyIdAttribPtr generateSubjectKeyIdentifierAttr(const OdString& attribStringData);
  void getSignatureAttributes(const OdBinaryData& signatureBlock, 
    OdString& signatureTimestamp, OdString& signatureComment, OdBinaryData& attribBlock);
};

#include "TD_PackPop.h"

#endif //#ifndef _ODDBCRYPTIMPL_H_
