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


#ifndef _TFREVISIONCONTROL_H_INCLUDED_
#define _TFREVISIONCONTROL_H_INCLUDED_

#include <Tf/Tf.h>
#include <Tf/TfIO.h>
#include "TfSchemaManager.h"
//#include "OdTfStorage.h"
#include "DbObjectIterator.h"

#define STL_USING_MAP
#include "OdaSTL.h"

#include <stdexcept>

// Merge policy determines conflict resolution action, when automatic resolution fails
// "manual" assumes some kind of user interaction - not implemented yet
enum OdTfMergePolicy
{
  kOdTfMergePreferTheirs,
  kOdTfMergePreferMine,
  kOdTfMergeManual
};

/** \details
  <group !!RECORDS_tdrawings_apiref>
*/
struct OdTfStorageInfo; // opaque pointer

/** \details
  <group !!RECORDS_tdrawings_apiref>
  Describes a commit in the revision control database
*/
struct TFDLL_EXPORT OdTfCommitInfo
{
  OdTimeStamp date; // Commit timestamp
  OdString message; // Commit message
  OdString author; // Commit author
  OdArray<OdTfDigest> parents; // Parent commits (one for usual commit, several for merge (first is from the branch _into_ which the merge was done))
  OdStreamBufPtr userData;
  OdTfCommitInfo(const OdString& author = L"", const OdString& message = L"", const OdTimeStamp& date = OdTimeStamp(OdTimeStamp::kInitUniversalTime), OdStreamBuf* arbitraryUserData = 0);
};

typedef OdArray<OdDbHandle> OdDbHandleArray;

/** \details
    <group TD_Namespaces>
*/
namespace OdTfRevisionControl
{
  // Open the revision control database or create an empty one 
  TFDLL_EXPORT OdTfStoragePtr open(const OdString& path, const OdString& strgClassName = L"TFSQLStorage", const OdString& appToLoad = L"TfSQLStorage");

  // Without optional parameters this function is expected to be used for the initial import (automatically switches to "master" branch)
  // Extended version imports given database as if its current state is the next revision of the given branch
  // (expected to be used when integrating changes made in third-party editors)
  TFDLL_EXPORT void import(OdTfStorage* strg, OdDbDatabase* pDb, const OdString& branch = L"master", const OdTfCommitInfo& message = OdTfCommitInfo());
  TFDLL_EXPORT void import(OdStreamBuf* pStream, OdDbDatabase* pDb, const OdString& branch = L"master", const OdTfCommitInfo& message = OdTfCommitInfo());

  // checkout: read database from storage "sourceBranch" setting "localBranch" as current for commit
  // source branch will be saved as default merge source (like remote tracking, for 'pull')
  // if 'partial' is false, all the objects are loaded in memory. If they will be eventually loaded anyway, it is much faster not to use 'partial' flag
  TFDLL_EXPORT OdDbDatabasePtr checkout(OdTfStorage* strg, OdDbHostAppServices* pHostApp, const OdString& localBranch, const OdString& sourceBranch = L"master", bool partial = false);
  TFDLL_EXPORT OdDbDatabasePtr checkout(OdStreamBuf* strm, OdDbHostAppServices* pHostApp, const OdString& localBranch, const OdString& sourceBranch = L"master", bool partial = false);

  // checkout: create a link between an empty database and storage. Update database from storage "sourceBranch" setting "localBranch" as current for commit.
  // if 'partial' is false, all the objects are loaded in memory. If they will be eventually loaded anyway, it is much faster not to use 'partial' flag
  // Note: database must be empty(created by default) 
  // (this function does essentially the same as previous)
  TFDLL_EXPORT void checkout(OdDbDatabase* pDb, OdTfStorage* strg, OdDbHostAppServices* pHostApp, const OdString& localBranch, const OdString& sourceBranch = L"master", bool partial = false);
  TFDLL_EXPORT void checkout(OdDbDatabase* pDb, OdStreamBuf* strg, OdDbHostAppServices* pHostApp, const OdString& localBranch, const OdString& sourceBranch = L"master", bool partial = false);

  // checkout from the storage of source database into the newly created database and return it 
  TFDLL_EXPORT OdDbDatabasePtr checkoutInDatabase(const OdDbDatabase * pSourceDb, OdDbHostAppServices* pHostApp, const OdString & localBranch, const OdString & sourceBranch, bool partial = false);

  // commit: save changes in the database to the storage, create commit object and shift current branch
  // Optional parameters (except the last) are for the merge commits (record the result of a merge operation)
  // The last parameter is an optional list of objects to save (partial commit)
  TFDLL_EXPORT void commit(OdDbDatabase* pDb, const OdTfCommitInfo& message, const OdString& mergeCommitOtherBranch = OdString::kEmpty, OdTfDigest* mergeCommitOtherParent = 0, const OdDbObjectIdArray* objectsToSave = 0);

  // Merge changes from the branch to the database.
  // Empty branch name means "merge from the branch that this database was checked out from" (like 'git pull')
  // returns 'false' if merge was fast forward (no merge commit necessary)
  // merge does not automatically commit changes, only runtime database is changed
  // if 'noFastForward' flag is true, and the remote branch is directly above or below this one in commit graph, then do nothing 
  TFDLL_EXPORT bool merge(OdDbDatabase* pDb, OdDbHandleArray& conflicts, OdTfMergePolicy mp = kOdTfMergePreferTheirs, const OdString& branch = OdString::kEmpty, bool noFastForward = false);

  // returns the commit and digest of the commit pointed by the branch
  TFDLL_EXPORT void getBranchTip(OdDbDatabase* strg, const OdString& branch, OdTfDigest&, OdTfCommitInfo* commit = 0);
  TFDLL_EXPORT void getBranchTip(OdTfStorage* strg, const OdString& branch, OdTfDigest&, OdTfCommitInfo* commit = 0);

  TFDLL_EXPORT OdTfCommitInfo getCommitInfo(OdDbDatabase* strg, const OdTfDigest&);
  TFDLL_EXPORT OdTfCommitInfo getCommitInfo(OdTfStorage* strg, const OdTfDigest&);
  // Collect all the changes made from one revision to another.
  // First revision is supposed to be the ancestor of a second (not necessarily direct)
  // This function is called on local repository to implement "git push"-like functionality
  //   together with the following "applyPatch" function.
  //   In the terms of push: "from" revision is the tip of a remote branch and "to" is a tip of a local remote tracking branch
  //   This function does not automatically fast forward local replica of a remote branch (if there is any)
  TFDLL_EXPORT OdStreamBufPtr makePatch(OdTfStorage* strg, const OdTfDigest& from, const OdTfDigest& to);
  
  // apply on remote repository patch received from local repository (created by makePatch() above)
  // "branch" is a name of a branch to fast forward
  // applyPatch will fail if the branch tip is not at the same revision as the patch start (makePatch "from" parameter)
  TFDLL_EXPORT void applyPatch(OdTfStorage* strg, OdStreamBuf* s, const OdString& branch);
  
  // If the database was checked out from revision control storage - return it, otherwise return 0
  // optionally return the database current branch (commit target)
  TFDLL_EXPORT OdTfStorage* getDatabaseStorage(OdDbDatabase* pDb, OdString* currentBranch = 0);
  // more universal version of the function, supporting binary format
  TFDLL_EXPORT OdTfStorageInfo* getDatabaseStorage2(OdDbDatabase* pDb, OdString* currentBranch = 0);

  // Set branch tip to the given hash. If no other branch points to the current top, and you don't have it saved, it will be inaccessible.
  // If no branch with this name existed, it will be created
  TFDLL_EXPORT void resetBranch(OdDbDatabase* strg, const OdString& branch, const OdTfDigest&);
  TFDLL_EXPORT void resetBranch(OdTfStorage* strg, const OdString& branch, const OdTfDigest&);

  // Switch database to specific revision, or unrelated branch (like 'git reset <commit>')
  // Source branch is a branch we are switching to (where the data will be taken from)
  // Local branch is a branch that we will commit to from now on (usually is equal to the source branch, unless you want to create a new one at this point)
  // if no revision is specified, top revision of the source branch is used
  // if no source branch is specified, the initial checkout branch is reused (consider using 'merge' instead)
  // if local branch parameter is empty it is set to be equal to the sourceBranch
  // if "hard" is false and there are local (non-committed) changes, operation will fail, otherwise local changes will be silently overwritten
  TFDLL_EXPORT void switchBranch(OdDbDatabase* pDb, const OdString& sourceBranch = OdString::kEmpty, const OdString& localBranch = OdString::kEmpty, OdTfDigest* revision = 0, bool hard = true);

  // get the list of objects changed since the last commit
  TFDLL_EXPORT OdDbObjectIdArray getLocalChanges(OdDbDatabase* pDb);
  
  // these simple wrappers over OdDbDatabase::writeFile preserve database connection to the Teigha file storage
  // (simply calling writeFile() would sever that connection)
  TFDLL_EXPORT void exportDwg(OdDbDatabase* pDb, OdStreamBuf* pStreamBuf, OdDb::DwgVersion fileVersion, bool saveThumbnailImage = false);
  TFDLL_EXPORT void exportDxf(OdDbDatabase* pDb, OdStreamBuf* pStreamBuf, OdDb::DwgVersion fileVersion, bool saveThumbnailImage = false, int dxfPrecision = 16);

  enum ChangeType
  {
    kObjectAdded = 1,
    kObjectDeleted,
    kObjectModified
  };
  typedef std::map<OdDbHandle, ChangeType> ChangeList;
  // enumerate the handles of the differing objects between revisions (revisions may actually be unrelated)
  TFDLL_EXPORT void getRevisionDifference(OdTfStorage* pTf, const OdTfDigest& newRevision, const OdTfDigest& oldRevision, ChangeList& result);
  
  // List of the branches in the revision control database
  TFDLL_EXPORT void getBranchList(OdTfStorage* pTf, std::map<OdString, OdTfDigest>&);
  TFDLL_EXPORT void getBranchList(OdDbDatabase* pTf, std::map<OdString, OdTfDigest>&);


  //TFDLL_EXPORT OdStreamBufPtr convertToActual(OdStreamBuf* inputStream, OdStreamBuf* schemaStream, OdTfBinarySchemaIteratorImpl* pActualSchemaIt);

  /** \details
    Replace 
  */
  TFDLL_EXPORT void compressRevisionData(OdTfStorage* strg, OdUInt16 referenceDistance = 10, const bool auditCompressedData = true);
  TFDLL_EXPORT void compressRevisionData(OdDbDatabase* pDb, OdUInt16 referenceDistance = 10, const bool auditCompressedData = true);

  /** \details
    Load unloaded objects in pObjects from currently loaded revision of storage
  */
  TFDLL_EXPORT void loadObjects(OdDbDatabase* pDb, OdDbObjectIteratorPtr pObjects);
};

#endif //_TFREVISIONCONTROL_H_INCLUDED_
