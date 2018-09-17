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

#ifndef DbAssocNetworkDeepCloneReactor_INCLUDED_
#define DbAssocNetworkDeepCloneReactor_INCLUDED_ /*!DOM*/

#include "DbDatabase.h"
#include "RxEvent.h"
#include "StaticRxObject.h"
#include "SharedPtr.h"

#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
<group Contraint_Classes>
*/
class OdDbAssocNetworkDeepCloneReactorData
{
  /** \details
  This class implements the deep clone mapping.

  Library: Source code provided.
  <group Contraint_Classes>
  */
  class OdDbAssocNetworkDeepCloneMapping
  {
  public:
    /** \details
    Constructor creates an instance of this class.
    */
    OdDbAssocNetworkDeepCloneMapping();

    /** \details
    Clears the map.
    */
    void clear(bool setFailure = false);
    bool isFailure() const { return m_failure; }

    /** \details
    Initializes the map.
    */
    void init(OdDbIdMapping& idMap, bool iInitOnlySrc);

  private:
    // Access to this mapping should be done via OdDbAssocNetworkDeepCloneAux::srcToDestIdMap();
    // Because reactor is a thing-in-a-self and aux is set of methods seen everywhere from deep clone of network

    /*!DOM*/
    friend class OdDbAssocNetworkDeepCloneAux;
    bool m_failure;

    /*!DOM*/
    std::map<OdDbObjectId, OdDbObjectId> m_deepCloneMapping;
  };

  /*!DOM*/
  friend class OdDbAssocNetworkDeepCloneAux;
  friend class OdDbAssocNetworkDeepCloneReactor;
  /*!DOM*/
  OdDbAssocNetworkDeepCloneMapping m_dcMapping;

  // All the ids involved in clone

  /*!DOM*/
  OdDbObjectIdArray m_aAllTheSrcAssocIds;

  // Cloning dimensions doesn't make them to be shown at proper position

  /*!DOM*/
  OdDbObjectIdArray m_aSrcDimensionsCloned;
};

typedef OdSharedPtr<OdDbAssocNetworkDeepCloneReactorData> OdDbAssocNetworkDeepCloneReactorDataPtr;

/** \details
  This class implements the deep clone reactor.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocNetworkDeepCloneReactor: public OdStaticRxObject<OdRxEventReactor>
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocNetworkDeepCloneReactor();

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbAssocNetworkDeepCloneReactor();

  /** \details
    Adds the reference to the deep clone reactor.
  */
  void addRefReactor();

  /** \details
    Releases the deep clone reactor.
  */
  void releaseReactor();

  // OdRxEventReactor methods

  /** \details
    Begins the deep clone.
  */
  void beginDeepCloneXlation(OdDbIdMapping& idMap);

  /** \details
    Aborts the deep clone.
  */
  void abortDeepClone(OdDbIdMapping& idMap);

  /** \details
    Ends the deep clone.
  */
  void endDeepClone(OdDbIdMapping& idMap);

private:

  void makeEmergencyClear(OdDbIdMapping& idMap);

  /** \details
    Removes the destination from the map if it is erased.
  */
  void removeDestFromMapIfErased(OdDbObjectId srcObjectId, OdDbIdMapping& idMap);

  /** \details
    Provides src - dest block id mapping for current deep clone of the associated network.
  */
  void collectSrcActionsAndDependencies(OdDbObjectId srcActionId, OdDbIdMapping& idMap);

  /** \details
    Clears the erased actions and dependencies from the map.
  */
  void clearErasedActionsAndDependenciesFromMap(OdDbIdMapping& idMap);

  /** \details
    Deletes the temporary dictionary.
  */
  void deleteTempDict(OdDbDatabasePtr pDb, const bool bDeleteActions);

  /** \details
    Checks whether transaction is long.
  */
  bool isInLongTransaction(OdDbIdMapping & idMap);

  friend class OdDbAssocNetworkDeepCloneAux;
  OdDbAssocNetworkDeepCloneReactorDataPtr getData(OdDbIdMapping & idMap);
  void deleteData(OdDbIdMapping & idMap);

private:

  typedef std::map<void *, OdDbAssocNetworkDeepCloneReactorDataPtr> IdMappingToInternalDataMap;
  IdMappingToInternalDataMap m_idMapToData;
  /*!DOM*/
  int m_Counter;
};

#endif
