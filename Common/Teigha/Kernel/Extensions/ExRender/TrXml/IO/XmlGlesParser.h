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
// XmlGlesParser.h

#ifndef OD_XML_GLES_PARSER_H_
#define OD_XML_GLES_PARSER_H_

#include "TD_PackPush.h"

#include "SharedPtr.h"
#include "BaseGlesParser.h"
#include "XmlGlesLoader.h"

typedef bool (*TXmlGlesParserFunc)(class OdXmlGlesParser* pThis, 
                                   class TiXmlElement* elem, 
                                   OdEnPathKey pk, const OdAnsiString& asPath);

/** <group ExRender_Classes>
*/
class OdXmlGlesParser : public OdBaseGlesParser
{
public: // TODO protected:
  OdSharedPtr<OdXmlGlesLoader> m_pLoader;
  OdString m_sError;
  TXmlGlesParserFunc m_funcArray;
  OdXmlGlesLoaderIdMapper* m_pIdMapper;
  bool parse(class TiXmlElement* elem, const OdAnsiString& asPath);

//public:
  OdXmlGlesParser( OdXmlGlesLoaderIdMapper* pIdMapper = NULL );
  virtual bool parse(const OdString& sXmlFilePathName, OdTrVisRendition* pRendition);
  virtual const OdString& errorMessage() const; 

//#ifdef EMCC // compiler to javascript
//  bool parseString(const char* pcszXmlContent, OdTrVisRendition* pRendition); // usable for emscripten
//#endif
};

#include "TD_PackPop.h"

#endif // OD_XML_GLES_PARSER_H_
