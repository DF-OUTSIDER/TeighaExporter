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

#include "OdaCommon.h"
#include "DbAssocNetwork.h"
#include "DbFiler.h"
#include "DbDictionary.h"
#include "DbImpAssocNetwork.h"

ODRX_DEFINE_MEMBERS_EX(OdDbAssocNetwork,     // ClassName
                       OdDbAssocAction,      // ParentClass
                       DBOBJECT_CONSTR,      // DOCREATE
                       OdDb::kDHL_1021,      // DwgVer
                       OdDb::kMRelease6,     // MaintVer
                       1025,                 // nProxyFlags (kEraseAllowed | kDisableProxyWarning)
                       L"AcDbAssocNetwork",  // DWG class name
                       L"ACDBASSOCNETWORK",  // DxfName
                       L"ObjectDBX Classes", // AppName
                       OdRx::kMTLoading | OdRx::kHistoryAware);

OdDbAssocNetwork::OdDbAssocNetwork(): OdDbAssocAction(false)
{
  m_pImpObj = new OdDbImpAssocNetwork();
}

OdDbObjectIdArray OdDbAssocNetwork::getActions() const
{
  assertReadEnabled();
  return ((OdDbImpAssocNetwork*)m_pImpObj)->getActions();
}

OdResult OdDbAssocNetwork::addAction( const OdDbObjectId& actionId, bool alsoSetAsDatabaseOwner )
{
  assertWriteEnabled();
  return ((OdDbImpAssocNetwork*)m_pImpObj)->addAction(objectId(), actionId, alsoSetAsDatabaseOwner);
}
OdResult OdDbAssocNetwork::removeAction(const OdDbObjectId& actionId, bool alsoEraseIt)
{
  assertWriteEnabled();
  return ((OdDbImpAssocNetwork*)m_pImpObj)->removeAction(actionId, alsoEraseIt);
}
#if 0
    OdResult addActions(const OdDbObjectIdArray& actionIds, bool alsoSetAsDatabaseOwner);

    OdResult removeAllActions(bool alsoEraseThem);

    OdResult ownedActionStatusChanged(OdDbAssocAction* pOwnedAction, AcDbAssocStatus  previousStatus);
#endif

void OdDbAssocNetwork::composeForLoad(OdDb::SaveType format, 
                                      OdDb::DwgVersion version, 
                                      OdDbAuditInfo* pAuditInfo)
{
  OdDbAssocAction::composeForLoad(format, version, pAuditInfo);

  if ( format == OdDb::kDwg && OdDb::kSTMode == database()->multiThreadedMode() )
    return;

  OdDbObjectIdArray ids = getActions();
  for (OdUInt32 index = 0; index < ids.size(); index++)
  {
    OdDbObjectPtr pChild = ids[index].openObject();
    if (pChild.isNull() || pChild->ownerId() != objectId() )
      continue;

    pChild->composeForLoad(format, version, pAuditInfo);
  }
}

OdDbObjectPtr OdDbAssocNetwork::decomposeForSave(OdDb::DwgVersion ver,
                                                 OdDbObjectId& replaceId, 
                                                 bool& exchangeXData)
{
  OdDbObjectPtr pObj = OdDbAssocAction::decomposeForSave(ver, replaceId, exchangeXData);
  OdDbObjectIdArray ids = getActions();
  for (OdUInt32 index = 0; index < ids.size(); index++)
  {
    OdDbObjectPtr pChild = ids[index].openObject();
    if (pChild.isNull())
      continue;

    if (pChild->decomposeForSave(ver, replaceId, exchangeXData).get())
    {	
      ODA_FAIL_ONCE();
	  }
  }
  return pObj;
}

OdDbObjectId OdDbAssocNetwork::getInstanceFromDatabase(OdDbDatabase*   pDatabase,
                                                       bool            createIfDoesNotExist,
                                                       const OdString& dictionaryKey)
{
  if (!pDatabase)
    return OdDbObjectId::kNull;
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectId dictId = pDatabase->getNamedObjectsDictionaryId();
  OdDbObjectId networkDict;

  OdDbDictionaryPtr dict = dictId.safeOpenObject();
  if ( !dict.isNull() )
    networkDict = dict->getAt(dictName);
  else
    return OdDbObjectId::kNull;

  bool createDict = false;

  OdDbDictionaryPtr pNetworkDict;
  OdDbObjectId res;

  if ( !networkDict.isNull() )
  {
    pNetworkDict = networkDict.openObject(OdDb::kForRead);
    res = pNetworkDict->getAt(dictName);

    if ( !res.isNull() )
      return res;

  }
  else
  {
    createDict = true;
  }

  if ( createIfDoesNotExist )
  {
    if ( createDict )
    {
      dict->upgradeOpen();
      pNetworkDict = OdDbDictionary::createObject();
      networkDict = dict->setAt(dictName, pNetworkDict);
    }
    
    if ( !pNetworkDict->isWriteEnabled() )
      pNetworkDict->upgradeOpen();

    OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::createObject();
    res = pNetworkDict->setAt(dictName, pNetwork);
    
    return res;
  }

  return OdDbObjectId::kNull;

}
OdDbObjectId OdDbAssocNetwork::getInstanceFromObject(const OdDbObjectId& owningObjectId,
                                          bool                createIfDoesNotExist,
                                          bool                addToTopLevelNetwork,
                                          const OdString&     dictionaryKey)
{
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectId res = OdDbObjectId::kNull;

  OdDbObjectPtr pOwningObjectId = owningObjectId.safeOpenObject();
  OdDbObjectId extDictId = pOwningObjectId->extensionDictionary();

  OdDbDictionaryPtr pDict;

  OdDbDictionaryPtr pNetworkDict;

  bool createExtDict = false;
  bool createDict = false;
  bool clearTopLevelNetwork = false;

  if (extDictId.isValid())
  {
    pDict = extDictId.safeOpenObject();
    OdDbObjectId networkDict = pDict->getAt(dictName);

    if (networkDict.isValid())
    {
      pNetworkDict = networkDict.safeOpenObject();
      res = pNetworkDict->getAt(dictName);

      if ( !res.isNull() )
      {
        OdDbObjectPtr network = res.openObject();
        if (OdDbAssocNetwork::cast(network).isNull()) { // it actually might be proxy
          if (!createIfDoesNotExist)
            return OdDbObjectId::kNull;
          network->upgradeOpen();
          network->erase();
          res.setNull();
          clearTopLevelNetwork = true;
          createIfDoesNotExist = false;
        } else
          return res;
      }
    }
    else
    {
      createDict = true;
    }
  }
  else
  {
    createExtDict = true;
    createDict = true;
  }

  if ( createIfDoesNotExist )
  {
    pOwningObjectId->upgradeOpen();

    if ( createExtDict )
    {
      pOwningObjectId->createExtensionDictionary();
      extDictId = pOwningObjectId->extensionDictionary();
      pDict = extDictId.openObject(OdDb::kForWrite);
    }
    else
      pDict->upgradeOpen();

    if ( createDict )
    {
      pNetworkDict = OdDbDictionary::createObject();
      pDict->setAt(dictName, pNetworkDict);
    }

    if ( !pNetworkDict->isWriteEnabled() )
      pNetworkDict->upgradeOpen();

    OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::createObject();
    res = pNetworkDict->setAt(dictName, pNetwork);
  }

  if ( (addToTopLevelNetwork && createIfDoesNotExist) || clearTopLevelNetwork )
  {
    OdDbObjectId topLevelNetId = getInstanceFromDatabase(pOwningObjectId->database(), true, dictionaryKey);
    OdDbAssocNetworkPtr pTopLevelNet = topLevelNetId.openObject(OdDb::kForWrite);
    if (clearTopLevelNetwork)
      pTopLevelNet->removeAction(res, false);
    else
	    pTopLevelNet->addAction(res, false);
  }

  return res;
}

OdResult OdDbAssocNetwork::removeInstanceFromObject(
  const OdDbObjectId& owningObjectId,
  bool                alsoEraseIt,
  const OdString&     dictionaryKey)
{
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectPtr pOwningObject = owningObjectId.safeOpenObject();
  OdDbObjectId extDictId = pOwningObject->extensionDictionary();

  // Get network id to erase network
  OdDbObjectId networkId = getInstanceFromObject(owningObjectId, false, false, dictionaryKey);

  OdDbDictionaryPtr pDict = OdDbDictionary::cast(extDictId.openObject(OdDb::kForWrite));
  if (!pDict.isNull())
  {
    OdDbDictionaryPtr pNetworkDict = pDict->getAt(dictName, OdDb::kForWrite);
    pDict->remove(dictName);
    if (!pNetworkDict.isNull())
    {
      // Under this name there is network itself
      pNetworkDict->remove(dictName);
      pNetworkDict->erase();
      pNetworkDict = 0;
    }
    pDict = 0;
  }

  // will be released if empty
  pOwningObject->releaseExtensionDictionary();
  pOwningObject = 0;

  if (alsoEraseIt)
  {
    OdDbAssocNetworkPtr pNetwork = networkId.openObject();
    if (!pNetwork.isNull())
    {
      OdDbAssocNetworkPtr pOwningNetwork  = pNetwork->owningNetwork().openObject(OdDb::kForWrite);
      if (pOwningNetwork.isNull())
      {
        pNetwork->erase();
        pNetwork = 0;
      }
      else
      {
        pNetwork = 0;
        pOwningNetwork->removeAction(networkId, true);
      }
    }
  }

  return eOk;
}

OdResult OdDbAssocNetwork::removeInstanceFromDatabase(
  OdDbDatabase*   pDatabase,
  bool            alsoEraseIt,
  const OdString& dictionaryKey)
{
  OdString dictName = dictionaryKey.isEmpty() ? OdString(OD_T("ACAD_ASSOCNETWORK")) : dictionaryKey;

  OdDbObjectId dictId = pDatabase->getNamedObjectsDictionaryId();
  OdDbDictionaryPtr dict = dictId.safeOpenObject();

  // Get network id to erase network
  OdDbObjectId networkId = getInstanceFromDatabase(pDatabase, false, dictionaryKey);

  OdDbDictionaryPtr pDict = OdDbDictionary::cast(dictId.openObject(OdDb::kForWrite));
  if (!pDict.isNull())
  {
    OdDbDictionaryPtr pNetworkDict = pDict->getAt(dictName, OdDb::kForWrite);
    pDict->remove(dictName);
    if (!pNetworkDict.isNull())
    {
      // Under this name there is network itself
      pNetworkDict->remove(dictName);
      pNetworkDict->erase();
      pNetworkDict = 0;
    }   
    pDict = 0;
  }

  if (alsoEraseIt)
  {
    OdDbAssocNetworkPtr pNetwork = networkId.openObject(OdDb::kForWrite);
    if (!pNetwork.isNull())
      pNetwork->erase();
    pNetwork = 0; 
  }

  return eOk;
}
