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
#include "AssetExporter.h"

namespace TD_COLLADA_EXPORT
{
  AssetExporter::AssetExporter(COLLADASW::StreamWriter * streamWriter, OdDbBaseDatabase *pDb):COLLADASW::Asset ( streamWriter )
  {
    getContributor().mAuthor = COLLADASW::String ( "Open Design Alliance" );

    int nMeasurement = 1;
    OdDbBaseDatabasePEPtr pDbPE(pDb);

    nMeasurement = pDbPE->getMeasurement(pDb);
    if (nMeasurement == 0)
    {
      setUnit("inches", 1 * 0.0254f);
    }
    else
    {
      setUnit("millimeter", 1 * 0.001f);
    }
    setUpAxisType ( COLLADASW::Asset::Z_UP );

    add();
  }
}
