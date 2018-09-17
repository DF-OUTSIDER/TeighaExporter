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


#ifndef _OD_TF_SCHEMAMANAGERNTERFACE_Included_
#define _OD_TF_SCHEMAMANAGERNTERFACE_Included_


#include "TD_PackPush.h"

namespace OdTf
{
  enum SaveType
  {
    kTxds = 0,//xml schema file type
    kTsbf = 1//binary schema file type
  };
}


class OdTfSchemaManagerInterface : public OdRxObject
{
public:
  /** \details
  Try to register the xml scheme or binary schema.
  \param schemeName [in] xml/binary schema name.
  Returns true if the schema is registered. Return false if the schema was registered early.
  \note Name of schema must have one of types (.tsbf | .txds)
  */
  virtual bool registerSchema(const OdString& schemeName) = 0;
  /** \details
  Try to register the xml scheme or binary schema.
  \param schemeName [in] xml/binary schema name.
  Returns true if the schema is registered. Return false if the schema was registered early.
  */
  virtual bool registerSchema(const OdString& schemaName, OdTf::SaveType fileType) = 0;
  /** \details
  Convert the xml scheme to binary schema.
  \param schemeName [in] xml scheme name.
  Note: binary scheme will be save in the same folder where situated xml schema, but with file type .tsbf
  */
  virtual void convertSchema(const OdString& schemeName) = 0;
  OdTfSchemaManagerInterface() {};
  virtual ~OdTfSchemaManagerInterface() {};
};

#include "TD_PackPop.h"

#endif // _OD_TF_SCHEMAMANAGERNTERFACE_Included_
