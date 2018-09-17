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
//
// BinaryGlesParser.h
//

#ifndef OD_BINARY_GLES_PARSER_H_
#define OD_BINARY_GLES_PARSER_H_

#include "TD_PackPush.h"

#include "SharedPtr.h"
#include "BaseGlesParser.h"
#include "XmlGlesLoader.h"
#include "UnivReadFiler.h"

typedef bool (*TBinaryGlesParserFunc)(class OdBinaryGlesParser* pThis, 
                                      OdUniversalReadFiler* pFiler, 
                                      OdEnPathKey pk);

/** <group ExRender_Classes>
*/
class OdBinaryGlesParser : public OdBaseGlesParser
{
public: // TODO protected:
  OdSharedPtr<OdXmlGlesLoader> m_pLoader;
  OdString m_sError;
  TBinaryGlesParserFunc m_funcArray;
  OdXmlGlesLoaderIdMapper* m_pIdMapper;
  bool parse(OdUniversalReadFiler* pFiler);
//public:
  OdBinaryGlesParser(OdXmlGlesLoaderIdMapper* pIdMapper = NULL);

  virtual bool parse(const OdString& sXmlFilePathName, OdTrVisRendition* pRendition);
  virtual const OdString& errorMessage() const; 
  // EMSCRIPTEN way
  typedef enum { enAuto, enBigEndian, enLittleEndian } TCodingType;
  virtual bool parse(OdStreamBuf* pStream, //OdUInt8* buffer, OdUInt32 numBytes, 
                     OdTrVisRendition* pRendition, TCodingType type = enAuto);
};

#include "TD_PackPop.h"

#endif // OD_BINARY_GLES_PARSER_H_
