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

#ifndef DbAssocNetworkDeepCloneAux_INCLUDED_
#define DbAssocNetworkDeepCloneAux_INCLUDED_ /*!DOM*/

#include "TD_PackPush.h"

#define STL_USING_MAP
#define STL_USING_SET
#include "OdaSTL.h"

#include "DbIdMapping.h"
#include "DbObjectId.h"

class OdDbAssoc2dConstraintGroup;
class OdDbAssocAction;

/** \details
  This class implements the auxiliary deep clone reactor.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocNetworkDeepCloneAux
{
public:

  /** \details
    Registries the deep clone reactor.
  */
  static void createAssocNetworkCloneReactors();

  /** \details
    Unregistries the deep clone reactor.
  */
  static void deleteAssocNetworkCloneReactors();

  /** \details
    Utility method.
  */
  static OdString getTempAssocDictionaryName();

  /** \details
    Provides src - dest block id mapping for current deep clone of the associated network.
  */
  static std::map<OdDbObjectId, OdDbObjectId> * srcToDestIdMap(OdDbIdMapping& idMap);

  /** \details
  Cleans internal data associated with given idMap
  */
  static void cleanData(OdDbIdMapping& idMap);

  /** \details
    Moves the complete action to the destination network.
  */
  static void moveCompleteActionToDestNetwork(OdDbAssocAction *pAction, OdDbObjectId destBlockId, OdDb::DeepCloneType deepCloneType);

  /** \details
    Removes constraints from pDestGroup if they are incomplete - if some of geometries were not cloned.
  */
  static void removeIncompleteConstraints(OdDbObjectId srcGroupId, OdDbIdMapping& idMap);

  /** \details
    This group-dependent method is placed to aux due to its size.
  */
  static void moveDepFromSrcGroup(OdDbAssoc2dConstraintGroup * pSrcGroup, OdDbAssoc2dConstraintGroup * pDestGroup);

  /** \details
  Removes incomplete expression from variable
  */
  static void clearIncompleteExpressions(const OdDbObjectIdArray& aSrcActions, int copyMode, OdDbIdMapping& idMap);

  /** Description:
  Returns all connected AssocActions which need to be copied
  */
  static OdDbObjectIdArray getUsedActions(const OdDbObjectIdArray& aSrcActions, int copyMode);

  /** Description:
  Resolves variables names collision according PARAMETERCOPYMODE value
  */
  static void mergeVariables(const OdDbObjectIdArray& aSrcActions,
    int copyMode,
    OdDbIdMapping& idMap,
    OdDbObjectId destNetworkId);
};

#include "TD_PackPop.h"

#endif
