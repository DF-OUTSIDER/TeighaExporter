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

#ifndef DbImpAssocNetwork_INCLUDED_
#define DbImpAssocNetwork_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbImpAssocAction.h"

#include "TD_PackPush.h"

/** \details
  This class represents the custom implementation of the network of associated actions.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocNetwork : public OdDbImpAssocAction
{
public: 
  /** \details
    Constructor creates an instance of this class.
  */
  explicit OdDbImpAssocNetwork();

  /** \details
    Gets the evaluation callback set, or NULL if no action evaluation is in progress.
  */
  OdDbObjectIdArray getActions() const;

  /** \details
    Adds the associated action to be owned by this network using its ID.
  */
  OdResult addAction(const OdDbObjectId networkId, const OdDbObjectId& actionId, bool alsoSetAsDatabaseOwner);

  /** \details
    Removes the associated action from this network using its ID.
  */
  OdResult removeAction(const OdDbObjectId& actionId, bool alsoEraseIt);

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
    Provides the post processing after deep clone.
  */
  virtual OdResult postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap); 

  /** \details
    Provides the post processing after deep clone cancel.
  */
  virtual OdResult postProcessAfterDeepCloneCancel(OdDbAssocAction *pAction, OdDbIdMapping& idMap);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Sets the status of the network of associated actions.
  */
  OdResult setStatus(OdDbAssocStatus newStatus,
                     bool notifyOwningNetwork,
                     bool setInOwnedActions);

  /** \details
    Evaluates the network of associated actions.
  */
  virtual void evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback);
  virtual void audit(OdDbObject* pObj, OdDbAuditInfo* pAuditInfo);

protected:
  /*!DOM*/
  OdDbObjectIdArray       m_idRefs;
  /*!DOM*/
  OdArray<bool>           m_bIsHard;
  /*!DOM*/
  OdUInt32 m_maxChildActionIdx;
  /*!DOM*/
  OdDbObjectIdArray       m_idRefs1;

};

#include "TD_PackPop.h"

#endif
