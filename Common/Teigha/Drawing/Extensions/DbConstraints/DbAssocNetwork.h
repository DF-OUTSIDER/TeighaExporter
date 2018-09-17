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

#ifndef DbAssocNetwork_INCLUDED_
#define DbAssocNetwork_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocAction.h"

#include "TD_PackPush.h"

/** \details
  This class represents the network of associated actions.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocNetwork : public OdDbAssocAction
{
public: 
    ODRX_DECLARE_MEMBERS(OdDbAssocNetwork);

    /** \details
      Constructor creates an instance of this class.
    */
    explicit OdDbAssocNetwork();

    /** \details
      Gets the evaluation callback set, or NULL if no action evaluation is in progress.
    */
    OdDbObjectIdArray getActions() const;

    /** \details
      Adds the associated action to be owned by this network using its ID.
    */
    OdResult addAction(const OdDbObjectId& actionId, bool alsoSetAsDatabaseOwner);

    /** \details
      Removes the associated action from this network using its ID.
    */
    OdResult removeAction(const OdDbObjectId& actionId, bool alsoEraseIt);

    /** \details
      Composes this associated network for loading.
    */
    virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);

    /** \details
      Decomposes this associated network for saving.
    */
    virtual OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver, OdDbObjectId& replaceId, bool& exchangeXData);

#if 0 // TODO: Nothing complex but needs time

    /** \details
      Adds an array of associated actions to be owned by this network.
    */
    OdResult addActions(const OdDbObjectIdArray& actionIds, bool alsoSetAsDatabaseOwner);

    /** \details
      Removes an array of associated actions from this network.
    */
    OdResult removeAllActions(bool alsoEraseThem);

    /** \details
      This method is called from the setStatus(..., true) method of the associated action and 
      notifies the network owing the action that the status has just been changed.
    */
    OdResult ownedActionStatusChanged(OdDbAssocAction* pOwnedAction, AcDbAssocStatus  previousStatus);
#endif
    /** \details
      Returns an object ID of the associated network owned by the given database. 
      It optionally creates a new instance if it does not exist yet. 
    */
    static OdDbObjectId getInstanceFromDatabase(OdDbDatabase*   pDatabase,
                                                bool            createIfDoesNotExist,
                                                const OdString& dictionaryKey = L"");

    /** \details
      Returns an object ID of the associated network owned by the extension dictionary 
      of the given object. It optionally creates a new sub-dictionary and a new network 
      if they do not exist yet.
    */
    static OdDbObjectId getInstanceFromObject(const OdDbObjectId& owningObjectId,
                                              bool                createIfDoesNotExist,
                                              bool                addToTopLevelNetwork = true,
                                              const OdString&     dictionaryKey        = L"");

    /** \details
      Removes the network and sub-dictionary which owns it from the extension dictionary of the object. 
    */
    static OdResult removeInstanceFromObject(const OdDbObjectId& owningObjectId,
                                             bool                alsoEraseIt,
                                             const OdString&     dictionaryKey = L"");

    /** \details
      Removes the network and sub-dictionary which owns it from the named object dictionary of the database. 
    */
    static OdResult removeInstanceFromDatabase(OdDbDatabase*   pDatabase,
                                               bool            alsoEraseIt,
                                               const OdString& dictionaryKey = L"");
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocNetwork class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocNetwork> OdDbAssocNetworkPtr;

#include "TD_PackPop.h"

#endif
