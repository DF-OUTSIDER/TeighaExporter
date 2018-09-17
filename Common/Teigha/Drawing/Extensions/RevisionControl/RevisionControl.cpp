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
#include <map>
#include "RevisionControl.h"

namespace revision_control {

RevisionControl::RevisionControl() {}

void RevisionControl::init() {
  if (!isInitialized())
    OdTfRevisionControl::import(tf_storage_ptr_, p_db_ptr_);
  else
    throw OdError("Revision control already is initialized");
}

bool RevisionControl::isInitialized() {
  std::map<OdString, OdTfDigest> branches;
  OdTfRevisionControl::getBranchList(tf_storage_ptr_, branches);
  return !branches.empty();
}

void RevisionControl::commit(OdTfCommitInfo& commit_message, OdTfDigest& revision_sha) {
  OdTfRevisionControl::commit(p_db_ptr_, commit_message);
  OdString current_branch;
  OdTfRevisionControl::getDatabaseStorage(p_db_ptr_, &current_branch);
  tf_storage_ptr_->getLabel(current_branch, revision_sha);
}

void RevisionControl::commit(OdTfCommitInfo& commit_message, OdTfDigest& revision_sha,
                             OdArray<OdDbHandle>& handles_to_commit) {
  if (handles_to_commit.empty())
    throw OdError("Handles list is empty");
  OdDbObjectIdArray objects_to_commit;
  for (OdArray<OdDbHandle>::iterator iter = handles_to_commit.begin() ; 
        iter != handles_to_commit.end() ; iter++)  {
      OdDbObjectId object_id = p_db_ptr_->getOdDbObjectId(*iter, false);
      if(object_id.isValid())
          objects_to_commit.append(object_id);
  }
  // TODO: do we need check here objects_to_commit.size == handles_to_commit.size???, 
  // just in case if some objects can be invalid
  OdTfRevisionControl::commit(p_db_ptr_, commit_message, OdString::kEmpty, 0, &objects_to_commit);
}

void RevisionControl::newBranch(const OdString& branch_name){
  if (branch_name.isEmpty())
    throw OdError("Branch is empty");
  OdString current_branch;
  OdTfDigest revision_sha;
  OdTfRevisionControl::getBranchTip(p_db_ptr_, current_branch, revision_sha);

  std::map<OdString, OdTfDigest> branches;
  OdTfRevisionControl::getBranchList(tf_storage_ptr_, branches);
  if (branches.find(branch_name) == branches.end())
    switchBranch(p_db_ptr_,  current_branch, branch_name);
  else 
    throw OdError("Branch name exists");
}

void RevisionControl::checkout(const OdString& branch_name) {
  OdString current_branch;
  OdTfRevisionControl::getDatabaseStorage(p_db_ptr_, &current_branch);
  if (current_branch.isEmpty())
    current_branch = L"master";
  OdTfRevisionControl::checkout(p_db_ptr_, tf_storage_ptr_, host_app_, branch_name, current_branch, true);
}

void RevisionControl::checkout(OdTfDigest& revision_sha, const OdString& branch_name){
  //TODO: make checking on possible errors: 1) no sha 
  OdString current_branch;
  OdTfStoragePtr tf_storage = OdTfRevisionControl::getDatabaseStorage(p_db_ptr_, &current_branch);
  OdTfRevisionControl::switchBranch(p_db_ptr_, current_branch, branch_name, &revision_sha);
}

void RevisionControl::diff(OdTfDigest& new_revision_sha, OdTfDigest& old_revision_sha,
                           std::map<OdDbObjectId, ChangeType>& new_changes_list,
                           std::map<OdDbObjectId, ChangeType>& old_changes_list) {
  std::map<OdDbHandle, ChangeType> all_changes;
  getRevisionDifference(tf_storage_ptr_, new_revision_sha, old_revision_sha, all_changes);

  //getting current SHA
  OdString current_branch;
  OdTfStoragePtr tf_storage = OdTfRevisionControl::getDatabaseStorage(p_db_ptr_, &current_branch);
  std::map<OdString, OdTfDigest> branches;
  OdTfRevisionControl::getBranchList(tf_storage_ptr_, branches);
  OdTfDigest current_digest = branches[current_branch];
  // TODO Using local database for fill object id map
  try {
    OdTfRevisionControl::switchBranch(p_db_ptr_, OdString::kEmpty, OdString::kEmpty, &new_revision_sha);
    fillObjectIdMapByHandles(p_db_ptr_, all_changes, new_changes_list);

    OdTfRevisionControl::switchBranch(p_db_ptr_, OdString::kEmpty, OdString::kEmpty, &old_revision_sha);
    fillObjectIdMapByHandles(p_db_ptr_, all_changes, old_changes_list);

    OdTfRevisionControl::switchBranch(p_db_ptr_, OdString::kEmpty, current_branch, &current_digest);
  }
  catch (OdError &err) {
    OdTfRevisionControl::switchBranch(p_db_ptr_, OdString::kEmpty, current_branch, &current_digest);
    // TODO message
    throw err;
  }
}

void RevisionControl::fillObjectIdMapByHandles(
  OdDbDatabasePtr pdb,
  std::map<OdDbHandle, ChangeType> all_changes,
  std::map<OdDbObjectId, ChangeType>& out_changes_list)
{
  for (std::map<OdDbHandle, ChangeType>::iterator it = all_changes.begin(); it != all_changes.end(); it++) {
    OdDbObjectId id = pdb->getOdDbObjectId(it->first);
    if (id.isValid()) {
      out_changes_list[id] = it->second;
    }
  }
}

void RevisionControl::getCommitMessage(OdTfDigest& sha, OdTfCommitInfo& commit_info){
  commit_info = OdTfRevisionControl::getCommitInfo(tf_storage_ptr_, sha);
}

void RevisionControl::getBranches(std::map<OdString, OdTfDigest>& branches){
  OdTfRevisionControl::getBranchList(tf_storage_ptr_, branches);
}

void RevisionControl::merge(const OdString& merge_from_branch, OdTfMergePolicy merge_policy, OdArray<OdDbObjectId>& conflicting_objects) {
  OdString current_branch;
  OdTfRevisionControl::getDatabaseStorage(p_db_ptr_, &current_branch);
  if (current_branch != merge_from_branch)
    throw OdError("Merge for the same branch");
  OdArray<OdDbHandle> conflicting_handles;
  OdTfRevisionControl::merge(p_db_ptr_, conflicting_handles, merge_policy, merge_from_branch, true);

  OdArray<OdDbHandle>::iterator it;
  for ( it = conflicting_handles.begin(); it != conflicting_handles.end(); it++ ) {
    OdDbObjectId id = p_db_ptr_->getOdDbObjectId(*it);
    if (id.isValid()) {
      conflicting_objects.push_back(id);
    }
  }
}

void RevisionControl::getCurrentBranch(OdString& current_branch) {
  OdTfRevisionControl::getDatabaseStorage(p_db_ptr_, &current_branch);
}

void RevisionControl::diff(const OdTfDigest& new_revision, const OdTfDigest& old_revision, ChangeList& changes_list) {
  OdTfRevisionControl::getRevisionDifference(tf_storage_ptr_, new_revision, old_revision, changes_list);
}

void RevisionControl::getCurrentChanges(OdDbObjectIdArray& changed_objects) {
  changed_objects = OdTfRevisionControl::getLocalChanges(p_db_ptr_);
}

RevisionControlPtr RevisionControl::create(OdDbHostAppServices& host_app, OdDbDatabasePtr p_db, const OdString& tigPath) {
  RevisionControlPtr ptr_rev_ctrl = OdRxObjectImpl<RevisionControl>::createObject();

  ptr_rev_ctrl->host_app_       = &host_app;
  ptr_rev_ctrl->p_db_ptr_       = p_db;
  ptr_rev_ctrl->tf_storage_ptr_ = OdTfRevisionControl::open(tigPath);

  return ptr_rev_ctrl;
}

RevisionControl* RevisionControl::cast(OdRxObject *obj) {
  return static_cast<RevisionControlPtr>(obj);
}

}
