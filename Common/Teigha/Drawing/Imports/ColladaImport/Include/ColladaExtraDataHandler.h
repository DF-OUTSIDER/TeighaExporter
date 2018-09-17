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
#ifndef __COLLADA_EXTRADATAHANDLER_H__
#define __COLLADA_EXTRADATAHANDLER_H__
#pragma once

#include "COLLADASaxFWLIExtraDataCallbackHandler.h"
#include "ColladaImporterBase.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  /** \details
    This class implements the extra data handler.
  */
  class ExtraDataHandler : public COLLADASaxFWL::IExtraDataCallbackHandler, public ImporterBase
  {
  public:
    /** Constructor. 
    */
    ExtraDataHandler(DocumentImporter* colladaImporter);
    /** Destructor. 
    */
    ~ExtraDataHandler();
    bool elementBegin( const COLLADASaxFWL::ParserChar* elementName, const GeneratedSaxParser::xmlChar** attributes);
    bool elementEnd(const COLLADASaxFWL::ParserChar* elementName );
    bool textData(const COLLADASaxFWL::ParserChar* text, size_t textLength);

    /** Method to ask, if the current callback handler want to read the data of the given extra element. 
    */
    bool parseElement( const COLLADASaxFWL::ParserChar* profileName, const COLLADASaxFWL::StringHash& elementHash, const COLLADAFW::UniqueId& uniqueId );
  };
}

#endif //__COLLADA_EXTRADATAHANDLER_H__
