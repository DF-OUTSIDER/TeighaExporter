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
#ifndef _ODTFSCHEMAMANAGERN_H_INCLUDED_
#define _ODTFSCHEMAMANAGERN_H_INCLUDED_


#include "OdaCommon.h"
#include "OdPlatformStreamer.h"
#include "OdaDefs.h"
#include <Tf/Tf.h>
#include <Tf/TfIO.h>

#include "FlatMemStream.h"
#include "../../ThirdParty/tinyxml/tinyxml.h"

#define STL_USING_SET
#include "OdaSTL.h"
#include "DbHostAppServices.h"
#include "RxDynamicModule.h"

class OdTfBinarySchemaIteratorImpl;

/** \details
<group OdTf_Classes>
*/
class TFDLL_EXPORT OdTfSchemaManager : public OdTfSchemaManagerInterface
{
public:
  ODRX_DECLARE_MEMBERS(OdTfSchemaManager);
  OdTfSchemaManager() : m_appServices(NULL) {};
  virtual ~OdTfSchemaManager() {};

  /** \details
  Try to register the xml scheme or binary schema.
  \param schemeName [in] xml/binary schema name.
  Returns true if the schema is registered. Return false if the schema was registered early.
  \note Name of schema must have one of types (.tsbf | .txds)
  */
  bool registerSchema(const OdString& schemeName);
  /** \details
  Try to register the xml scheme or binary schema.
  \param schemeName [in] xml/binary schema name.
  Returns true if the schema is registered. Return false if the schema was registered early.
  */
  virtual bool registerSchema(const OdString& schemaName, OdTf::SaveType fileType);
  /** \details
  Convert the xml scheme to binary schema.
  \param schemeName [in] xml scheme name.
  \note binary scheme will be save in the same folder where situated xml schema, but with file type .tsbf
  */
  virtual void convertSchema(const OdString& schemeName);
  /** \details
  Set HostAppServices that will be used bu OdTfSchemaManager.
  \param pHostAppServices [in] pointer to OdDbHostAppServices.
  \note setAppServices used in ::odTfInitialize(pHostApp);
  */
  void setAppServices(OdDbHostAppServices* pHostAppServices);
protected:
  void mergeSchema(const OdString& schemeName);
  void mergeSchemaBin(const OdString& schemeName);

  //To avoid registration of already registered schemes, we store the names
  std::set<OdString> m_setNames;
  OdDbHostAppServices* m_appServices;
};

typedef OdSmartPtr<OdTfSchemaManager> OdTfSchemaManagerPtr;

/** \details
Used to register basic(core) scheme.
\param pAppServices [in] OdDbHostAppServices.
\note pAppServices can't be NULL
*/
TFDLL_EXPORT bool odTfInitialize(OdDbHostAppServices* pAppServices);

/** \details
Try to register the scheme.
\param pathSchema [in] path to xml scheme file.
\param schemeName [in] scheme name. Need to avoid repeated registering of scheme.
\param pAppServices [in] OdDbHostAppServices.
Returns true if the schema is registered. Return false if the schema was registered early.
\note pAppServices can't be NULL
*/
TFDLL_EXPORT bool odrxRegisterSchema(const OdString& schemeName);

/** \details
Convert the xml scheme to binary schema.
\param schemeName [in] xml scheme name.
\note binary scheme will be save in the same folder where situated xml schema, but with file type .tsbf
*/
TFDLL_EXPORT void odrxConvertSchema(const OdString& schemeName);


#endif //_ODTFSCHEMAMANAGERN_H_INCLUDED_
