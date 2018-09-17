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

#ifndef _DWFUGEOMCALLBACKS_INCLUDED_
#define _DWFUGEOMCALLBACKS_INCLUDED_

#include "DwfUGeomBase.h"

namespace DWFCore
{
  class DWFString;
  class DWFInputStream;
}

class WT_File;
class WT_Rendition;
class OdDbStub;

class DwfUGeomCallbacks : public DwfUGeomBase
{
protected:
  virtual OdResult setCallbacks(WT_File& wtFile);

  virtual OdResult proceed6stream(DWFCore::DWFInputStream& stream);
  virtual OdResult proceedXstream(DWFCore::DWFInputStream& stream, WT_File&);
  virtual OdResult proceed55stream(DWFCore::DWFInputStream& stream);

public:
  DwfUGeomCallbacks(OdString password, OdString layoutName);

  virtual ~DwfUGeomCallbacks() {}
};

class OdDbStub* getCurrentLineStyle(DwfUUserData* pUserData, WT_Rendition& rendition);
OdDb::LineWeight getCurrentLineWeight(double w);
OdDb::LineWeight getCurrentLineWeight(WT_Rendition& rendition);

#endif // _DWFUGEOMCALLBACKS_INCLUDED_
