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



#ifndef OD_DBDATALINK_H
#define OD_DBDATALINK_H

#include "DbObject.h"
#include "OdValue.h"
#include "StringArray.h"

#include "TD_PackPush.h"

namespace OdDb
{
  enum DataLinkOption
  { 
    kDataLinkOptionNone                         = 0,
    kDataLinkOptionAnonymous                    = 0x1,
    kDataLinkOptionPersistCache                 = 0x2,
    kDataLinkOptionDisableInLongTransaction     = 0x4,
    kDataLinkHasCustomData                      = 0x8
  };

  enum PathOption
  { 
    kPathOptionNone = 1,
    kPathOptionRelative,
    kPathOptionAbsolute,
    kPathOptionPathAndFile
  };

  enum UpdateDirection   
  { 
    kUpdateDirectionSourceToData   = 0x1,
    kUpdateDirectionDataToSource   = 0x2
  };

  enum UpdateOption 
  { 
    kUpdateOptionNone                                  = 0,
    kUpdateOptionSkipFormat                            = 0x20000,
    kUpdateOptionUpdateRowHeight                       = 0x40000,
    kUpdateOptionUpdateColumnWidth                     = 0x80000,
    kUpdateOptionAllowSourceUpdate                     = 0x100000,
    kUpdateOptionForceFullSourceUpdate                 = 0x200000,
    kUpdateOptionOverwriteContentModifiedAfterUpdate   = 0x400000,
    kUpdateOptionOverwriteFormatModifiedAfterUpdate    = 0x800000,
    kUpdateOptionForPreview                            = 0x1000000,
    kUpdateOptionIncludeXrefs                          = 0x2000000,
    kUpdateOptionSkipFormatAfterFirstUpdate            = 0x4000000
  };

  enum DataLinkGetSourceContext
  { 
    kDataLinkGetSourceContextUnknown,
    kDataLinkGetSourceContextEtransmit,
    kDataLinkGetSourceContextXrefManager,
    kDataLinkGetSourceContextFileWatcher,
    kDataLinkGetSourceContextOther
  };
};

/** \details
    This class links a table to data in another file.

    \sa
    TD_Db
    
    <group OdDb_Classes> 

*/
class DBENT_EXPORT OdDbDataLink : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDataLink);

  OdDbDataLink(void);
  
  OdString name (void) const;
  void setName (const OdString& sName);
  OdString description (void) const;
  void setDescription (const OdString& sDescription);
  OdString getToolTip (void) const;
  void setToolTip (const OdString& sToolTip);
  OdString  dataAdapterId (void) const;
  void setDataAdapterId (const OdString& sAdapterId);
  OdString  connectionString (void) const;
  void setConnectionString(const OdString& sConnectionString);
  OdDb::DataLinkOption option (void) const;
  void setOption (OdDb::DataLinkOption nOption);
  OdInt32 updateOption (void) const;
  void setUpdateOption(OdInt32 nOption);
  void update (OdDb::UpdateDirection nDir, OdDb::UpdateOption nOption);
  void getUpdateStatus(OdDb::UpdateDirection* pDir, OdTimeStamp* pTime, OdString* pErrMessage) const;
  void setUpdateStatus(const OdString& sErrMessage);
  bool isValid (void) const;
  OdInt32 getTargets (OdDbObjectIdArray& targetIds) const;
  void getSourceFiles(OdDb::DataLinkGetSourceContext nContext, OdStringArray& files) const;
  void repathSourceFiles(OdString& sBasePath, OdDb::PathOption nOption);
  OdDbObjectId contentId() const;
  OdValue getCustomData(const OdString& sKey) const;
  void setCustomData(const OdString& sKey, const OdValue* pData);

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};

typedef OdArray<OdDbDataLink*> OdDbDataLinkArray;

typedef OdSmartPtr<OdDbDataLink> OdDbDataLinkPtr;


//*************************************************************************
// OdDbDataLinkManager
//*************************************************************************
class OdDbDataLinkManagerImpl;

/** \details
    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbDataLinkManager : public OdStaticRxObject<OdRxObject>
{
public:
  ODRX_HEAP_OPERATORS();

  ODRX_DECLARE_MEMBERS(OdDbDataLinkManager);
  
  OdDbDataLinkManager(void);
  ~OdDbDataLinkManager(void);
  OdDbObjectId createDataLink(const OdString& adapterId, const OdString& name,
    const OdString& description, const OdString& connectionString);

  OdDbObjectId getDataLink(const OdString& name) const;

  OdDbDataLinkPtr getDataLink(const OdString& name, OdDb::OpenMode mode) const;

  int   dataLinkCount(void) const;

  int   getDataLink(OdDbObjectIdArray& dataLinks) const;

  OdDbObjectId addDataLink(OdDbDataLink* pDataLink);

  void removeDataLink(const OdString& sKey, OdDbObjectId& idDataLink);

  void removeDataLink(const OdDbObjectId& idDataLink);

  void update(const OdDbObjectIdArray& dataIds, OdDb::UpdateDirection nDir, OdDb::UpdateOption nOption);
  void update(OdDb::UpdateDirection nDir, OdDb::UpdateOption nOption);

  void setDatabase(OdDbDatabase* pDb);
protected:
  OdDbDataLinkManager(OdDbDataLinkManagerImpl* pValImpl);
  OdDbDataLinkManagerImpl* m_pImpl;
  friend class OdDbSystemInternals;
};

typedef OdSmartPtr<OdDbDataLinkManager> OdDbDataLinkManagerPtr;

class DBDIM_EXPORT OdDbDataLinkUpdatePE : public OdRxObject
{
public:
  virtual OdError dataLinkUpdate(OdDbDataLink* pDataLink, OdDb::UpdateDirection nDir, OdDb::UpdateOption nOption) = 0;
  virtual OdError repathSourceFiles(OdString& sBasePath, OdDb::PathOption nOptionm, OdString& path) = 0;

  ODRX_DECLARE_MEMBERS(OdDbDataLinkUpdatePE);
};

typedef OdSmartPtr<OdDbDataLinkUpdatePE> OdDbDataLinkUpdatePEPtr;

#include "TD_PackPop.h"

#endif // OD_DBDATALINK_H
