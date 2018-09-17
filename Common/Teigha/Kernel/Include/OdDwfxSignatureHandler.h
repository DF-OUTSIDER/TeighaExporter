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

// OdDwfxSignatureHandler.h - interface of digital signature handler for .dwfx files

#ifndef _OD_DWFXSIGNATUREHANDLER_H__CCC79098_26F9_4DF3_B146_F698FBFEF213__INCLUDED_
#define _OD_DWFXSIGNATUREHANDLER_H__CCC79098_26F9_4DF3_B146_F698FBFEF213__INCLUDED_

#include "TD_PackPush.h"

#include "RxModule.h"
#include "RootExport.h"
#include "SharedPtr.h"
#include "OdArray.h"

/** \details
  <group OdDbRoot_Classes>
  Corresponding C++ library: TD_Root
*/
class OdCertParameters
{
public:
  OdString sCertSubject;
  OdString sCertIssuer;
  OdString sCertSerialNum;

  OdString m_CertValidFrom;
  OdString m_CertValidTo;
};

/** \details
  <group OdDbRoot_Classes>
  This class is the interface class for the DWFx signature handler object.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdDwfxSignatureHandler
{
public:
  virtual ~OdDwfxSignatureHandler(){}

  enum SignatureValidationResult
  {
    kSuccess = 0,
    kInvalidSignature,
    kCertificateChainProblem,
    kNotSigned
  };

  /** \details
    Method to set the certificate for signing the DWFx package.    
    \param certParams [in] Certificate description data.
  */
  virtual void setCertParameters(const OdCertParameters& certParams) = 0;

  /** \details
    Method to sign the DWFx package.
    \returns
    Returns true if succeeded. Otherwise returns false.
    \param filePath [in] Full path to the package.
  */
  virtual bool signPackage(const OdString& filePath) = 0;

  /** \details
    Method to remove existing signature of the package.
    \returns
    Returns true if succeeded. Otherwise returns false.
    \param filePath [in] Full path to the package.
  */
  virtual bool removeExistingSignature(const OdString& filePath) = 0;

  /** \details
    Method to validate signature.
    \returns
    Returns true if succeeded. Otherwise returns false.
    \param filePath [in] Full path to the package.
    \param validationResult [out] The result of signature validation if method succeeds.
    \param certParams [out] Description data of the certificate if obtained one during the validation.
  */
  virtual bool validatePackageSignature(const OdString& filePath, 
    SignatureValidationResult& validationResult, OdCertParameters& certParams) = 0;
};

typedef OdSharedPtr<OdDwfxSignatureHandler> OdDwfxSignatureHandlerPtr;

/** \details
  <group OdDbRoot_Classes>
  This class is the interface class for the module that provides digital signature handling for DWFx files.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdDwfxSignatureHandlerModule : public OdRxModule
{
public:
  /** \details
    Returns the pointer to the signature handler object.    
  */
  virtual OdDwfxSignatureHandlerPtr getDwfxSignatureHandler() const = 0;

  /** \details
    Fills an array using the data of certificates available on the system.
    \returns
    Returns the number of suitable certificates on the system.
    \param certificates [out] Result array containing the data of certificates.
  */
  virtual int getSuitableCertificatesList(OdArray<OdCertParameters>& certificates) const = 0;
};

typedef OdSmartPtr<OdDwfxSignatureHandlerModule> OdDwfxSignatureHandlerModulePtr;

#include "TD_PackPop.h"

#endif
