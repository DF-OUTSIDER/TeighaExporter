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

#ifndef _OD_DBMOTIONPATH_H_INCLUDED_
#define _OD_DBMOTIONPATH_H_INCLUDED_

#include "DbObject.h"

#include "TD_PackPush.h"

/** \details
    Corresponding C++ library: SCENEOE

    <group OdDb_Classes>
*/
class SCENEOE_EXPORT OdDbMotionPath : public OdDbObject
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbMotionPath);

    OdDbMotionPath();
    virtual ~OdDbMotionPath();

    void setCameraPath(const OdDbObjectId &pathId);
    OdDbObjectId cameraPath() const;

    void setTargetPath(const OdDbObjectId &pathId);
    OdDbObjectId targetPath() const;

    void setViewTableRecordId(const OdDbObjectId &viewId);
    OdDbObjectId viewTableRecordId() const;

    void setFrames(OdUInt32 nFrames);
    OdUInt32 frames() const;

    void setFrameRate(OdUInt32 nRate);
    OdUInt32 frameRate() const;

    void setCornerDecel(bool bCornerDecel);
    bool cornerDecel() const;

    virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
    virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
    virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
    virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdSmartPtr<OdDbMotionPath> OdDbMotionPathPtr;

SCENEOE_EXPORT OdDbObjectId oddbGetMotionPathDictionaryId(OdDbDatabase* pDb, bool createIfNotFound = false);
SCENEOE_EXPORT OdDbDictionaryPtr oddbGetMotionPathDictionary(OdDbDatabase* pDb, OdDb::OpenMode mode, bool createIfNotFound = false);

#include "TD_PackPop.h"

#endif // _OD_DBMOTIONPATH_H_INCLUDED_
