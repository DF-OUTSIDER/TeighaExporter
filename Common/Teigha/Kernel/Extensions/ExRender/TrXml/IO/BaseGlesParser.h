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
// BaseGlesParser.h
//

#ifndef OG_BASE_GLES_PARSER_H_
#define OG_BASE_GLES_PARSER_H_

/** <group ExRender_Classes>
*/
class OdBaseGlesParser
{
protected:
  OdBaseGlesParser() {}
public:
  virtual ~OdBaseGlesParser() {}

  virtual bool parse(const class OdString& sXmlFilePathName, class OdTrVisRendition* pRendition) = 0;
  virtual const OdString& errorMessage() const = 0; 
};

#endif // OG_BASE_GLES_PARSER_H_
