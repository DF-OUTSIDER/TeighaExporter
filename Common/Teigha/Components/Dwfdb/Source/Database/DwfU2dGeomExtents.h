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

#ifndef _DWFU2DGEOMEXTENTS_INCLUDED_
#define _DWFU2DGEOMEXTENTS_INCLUDED_

#include "DwfU2dGeomDraw.h"

namespace DWFCore
{
  class DWFString;
  class DWFInputStream;
}

class WT_File;
class OdGiViewportDraw;
struct OdDbUnderlayDrawContext;

class DwfU2dGeomExtents : public DwfU2dGeomDraw
{
protected:
  virtual OdResult setCallbacks(WT_File& wtFile);

public:
  DwfU2dGeomExtents(OdString password, OdString layoutName, OdGiViewportDraw &pVd, const OdDbUnderlayDrawContext& context);

  virtual ~DwfU2dGeomExtents() {}
};

#endif // _DWFU2DGEOMEXTENTS_INCLUDED_
