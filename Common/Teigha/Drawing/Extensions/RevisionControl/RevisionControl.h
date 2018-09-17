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

#ifndef REVISION_CONTROL_
#define REVISION_CONTROL_

#include <Tf/Tf.h>
#include <Tf/TfIO.h>
#include "Tf/TfRevisionControl.h"

using namespace OdTfRevisionControl;

/** \details
  <group TD_Namespaces>
*/
namespace revision_control {

class RevisionControl;
typedef OdSmartPtr<RevisionControl> RevisionControlPtr;

/** \details
  This class provides API for revision control
  Library: Source provided.
  <group RevisionControl_Classes>
*/
class RevisionControl : public OdRxObject {
protected:
  RevisionControl();

  bool isInitialized();

  void fillObjectIdMapByHandles(
      OdDbDatabasePtr pdb,
      std::map<OdDbHandle, ChangeType> all_changes,
      std::map<OdDbObjectId, ChangeType>& out_changes_list);

public:
  /** \details
  Initialization of revision control API, this method will create tf storage to
  save revision history
  \note
  */
  void init();
  /** \details
  Stores the current workspace changes in a new commit along with a
  log message from the user describing the changes.
  \param commit_message [in]  message for revision (autor, log)
  \param revision_sha [out]  sha of just created revision
  \note
  */
  void commit(OdTfCommitInfo& commit_message, OdTfDigest& revision_sha);
  /** \details
  Record ceratin changes to tf storage
  \param commit_message [in]  message for revision (autor, message)
  \param revision_sha [out]  sha of just created revision
  \param handles_to_commit [in]  array of db handles to commit
  \note
  */
  void commit(OdTfCommitInfo& commit_message,
              OdTfDigest& revision_sha,
              OdArray<OdDbHandle>& handles_to_commit);
  /** \details
  Create new branch
  \param branch_name [in]  branch name
  \note
  */
  void newBranch(const OdString& branch_name);
  /** \details
  Set branch as current and set HEAD on the last revison of selected branch
  \param branch_name [in]  branch name to set as current
  \note
  */
  void checkout(const OdString& branch_name);
  /** \details
  Set branch as current and set HEAD to special revision on this branch
  \param branch_name [in]  branch name to set as current
  \param revision_sha [in]  revision to set as current
  \note
  */
  void checkout(OdTfDigest& revision_sha, const OdString& branch_name);
  /** \details
  Incorporates changes from the certain branch into the current branch. If there
  are conflicts conflicting_objects will contain list of conflicting objects
  \param merge_from_branch [in]  source branch name
  \param merge_policy [in]  merge policy to handle conflict situations
  \param conflicting_objects [out]  array of objects id which is conflicting
  \note
  */
  void merge(const OdString& merge_from_branch,
             OdTfMergePolicy merge_policy,
             OdArray<OdDbObjectId>& conflicting_objects);
  /** \details
  Gets difference between two revisions and returns changed objects with their
  original properties
  \param new_revision_sha [in]  sha for first revision to compare
  \param old_revision_sha [in]  sha for second revision to compare
  \param new_changes_list [out]  map of objects id from first revision and their
  changes type
  \param old_changes_list [out]  map of objects id from second revision and their
  changes type
  \note
  */
  void diff(OdTfDigest& new_revision_sha,
            OdTfDigest& old_revision_sha,
            std::map<OdDbObjectId, ChangeType>& new_changes_list,
            std::map<OdDbObjectId, ChangeType>& old_changes_list);
  /** \details
  Gets commit message related to given sha
  \param sha [in]  sha to get information
  \param commit_info [out]  commit info related to given sha
  \note
  */
  void getCommitMessage(OdTfDigest& sha, OdTfCommitInfo& commit_info);
  /** \details
  Gets map of all branches and their heads
  \param branches [out] map of branches and top revisions sha
  \note
  */
  void getBranches(std::map<OdString, OdTfDigest>& branches);
  /** \details
  Get current changes in workspace compared to last revision
  \param changed_objects [out] array of new/deleted/changed objects id
  \note
  */
  void getCurrentChanges(OdDbObjectIdArray& changed_objects);
  /** \details
  Get current name of branch
  \param current_branch [out] current branch name
  \note
  */
  void getCurrentBranch(OdString& current_branch);
  /** \details
  Get changestype between two revisions. It works faster than diff which returns
  two lists but it returns only all Handles of objects with their changes types in map
  \param new_revision_sha [in]  sha for first revision to compare
  \param old_revision_sha [in]  sha for second revision to compare
  \param changes_list [out]  map of handles and their changes type
  \note
  */
  void diff(const OdTfDigest& new_revision_sha,
            const OdTfDigest& old_revision_sha,
            ChangeList& changes_list);
  /** \details
  Factory class method to create RevisionControl object
  \param host_app [in]  host app service
  \param p_db [in]  pointer to database
  \param tf_path [in]  path to tf storage
  \note
  */
  static RevisionControlPtr create(OdDbHostAppServices& host_app,
                                   OdDbDatabasePtr p_db,
                                   const OdString& tf_path);
  static RevisionControl* cast(OdRxObject* obj);

private:
  OdDbDatabasePtr p_db_ptr_;
  OdTfStoragePtr tf_storage_ptr_;
  OdDbHostAppServices* host_app_;
};
}

#endif // REVISION_CONTROL_
