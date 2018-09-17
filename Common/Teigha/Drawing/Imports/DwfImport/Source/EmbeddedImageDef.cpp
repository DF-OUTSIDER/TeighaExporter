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

#include "StdAfx.h"
#include "OdaCommon.h"
#include "DbObject.h"
#include "EmbeddedImageDef.h"
#include "DbSystemServices.h"

namespace TD_DWF_IMPORT {

EmbeddedImageDef::EmbeddedImageDef()
{
}

EmbeddedImageDef::~EmbeddedImageDef()
{
}

void EmbeddedImageDef::beginSave(const OdString& pIntendedName)
{
  if (pIntendedName.isEmpty())
    return;
  // extract base path
  OdString path = pIntendedName;
  OdString shortFileName;
  // under windows in the path there may be both '\' and '/' separators
  {
    int i1 = path.reverseFind('\\');
    int i2 = path.reverseFind('/');
    int i = odmax(i1, i2);
    shortFileName = path.mid(i+1);
    path = path.left(i+1);
    i = shortFileName.find(L'.');
    if (i != -1)
      shortFileName = shortFileName.left(i);
  }
  // materialize image to the file by the path
  OdChar filename[1024];
  if (_name.isEmpty())
  {
    int imageNum = 0;
    for (;;)
    {
      odSprintf(filename, 1024, OD_T("%simage%d%ls"), (const OdChar*)path, imageNum++, (const OdChar*)_ext);
      if (!odSystemServices()->accessFile(filename, 0))
        break;
    }
  }
  else
  {
    odSprintf(filename, 1024, OD_T("%ls%ls_%ls%ls"), (const OdChar*)path, (const OdChar*)shortFileName, (const OdChar*)_name, (const OdChar*)_ext);
  }
  try
  {
    OdStreamBufPtr f = odSystemServices()->createFile(
      filename, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways );
    if (!f.isNull())
    {
      _data->rewind();
      _data->copyDataTo(f);

      // set this materialized file path to OdDbRasterImageDef
      upgradeOpen();
      setSourceFileName(filename);
      if (image().isNull() || image()->scanLineSize() == 0)
      {
        OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
        if (!pRasSvcs.isNull())
        {
          OdGiRasterImagePtr img = pRasSvcs->loadRasterImage(filename);
          if (!img.isNull())
            setImage(img);
        }
      }
      downgradeOpen();
    }
  }
  catch (OdError&)
  {
  }
}
}
