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
// GLES2 device local resource sharing provider

#ifndef ODGLES2LOCALSHARINGPROVIDER
#define ODGLES2LOCALSHARINGPROVIDER

#include "TD_PackPush.h"

#include "TrVecResourceSharingProvider.h"

class OdTrGL2LocalRenditionHost;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrGL2LocalSharingProvider : public OdTrVecResourceSharingProvider
{
  public:
    OdTrGL2LocalSharingProvider();
    ~OdTrGL2LocalSharingProvider();

    // Share metafile with secondary device (should be overrided by client, do nothing by default)
    virtual bool shareMetafile(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                               OdTrVisMetafileId metafileId, void *pInternalDataPtr, void *pOriginatorPtr);
    // Share texture with secondary device (should be overrided by client, do nothing by default)
    virtual bool shareTexture(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                              OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr);
    // Share material with secondary device (should be overrided by client, do nothing by default)
    virtual bool shareMaterial(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                               OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr);
  protected:
    OdTrGL2LocalRenditionHost *renditionHost(OdTrVecDevice *pRequestFor);
};

#include "TD_PackPop.h"

#endif // ODGLES2LOCALSHARINGPROVIDER
