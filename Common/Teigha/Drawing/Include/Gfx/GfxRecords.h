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

#ifndef _CLOUDGFXRECORD_H_INCLUDED_
#define _CLOUDGFXRECORD_H_INCLUDED_



namespace CloudGfxRecord {

#define CloudGfxRecordList()\
    DefCloudGfxRecord(GfxEntity)\
    DefCloudGfxRecord(GfxInvisibleEntity)\
    DefCloudGfxRecord(GfxContainer)\
    DefCloudGfxRecord(GfxViewExtents)\
    DefCloudGfxRecord(GfxLayer)\
    DefCloudGfxRecord(GfxMaterial)\
    DefCloudGfxRecord(GfxDistantLight)\
    DefCloudGfxRecord(GfxPointLight)\
    DefCloudGfxRecord(GfxWebLight)\
    DefCloudGfxRecord(GfxVisualStyle)\
    DefCloudGfxRecord(GfxSpotLight)\
    DefCloudGfxRecord(EraseGfx)\
    DefCloudGfxRecord(GfxView)\
    DefCloudGfxRecord(GfxSolidBackground)\
    DefCloudGfxRecord(GfxGradientBackground)\
    DefCloudGfxRecord(GfxImageBackground)\
    DefCloudGfxRecord(GfxGroundPlaneBackground)\
    DefCloudGfxRecord(GfxSkyBackground)\
    DefCloudGfxRecord(GfxBLBackground)\
    DefCloudGfxRecord(Reset   )\
    DefCloudGfxRecord(GfxPalette)\
    DefCloudGfxRecord(EmbeddedFile)\
    DefCloudGfxRecord(BeginUpdate)\
    DefCloudGfxRecord(EndUpdate)\
    DefCloudGfxRecord(EndUpdateGfxTruncated)\
    DefCloudGfxRecord(AddGfx)\
    DefCloudGfxRecord(EraseVpGfx)\

#define DefCloudGfxRecord(Name) Name,

  enum TopLevel {
    CloudGfxRecordList()

    LastTopLevelRecord
  };

#undef DefCloudGfxRecord


};



#endif //#ifndef _CLOUDGFXRECORD_H_INCLUDED_
