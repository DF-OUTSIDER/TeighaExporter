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


#if __cplusplus>=199711L
#include "OdaCommon.h"
#include "RevisionSerialization.h"
#include "CloudTools.h"
//#include "../Kernel/Extensions/ExRender/TrXml/IO/CloudTools.h"


namespace revision_control {
void RevisionSerializationModel::addBranchToModel(const OdTfDigest &head, const OdString& branch) {
  std::deque<OdTfDigest> queue;
  OdTfCommitInfo         commit_info;
  OdTfDigest             id;

  queue.push_back(head);

  while (!queue.empty()) {
    id = queue.front();
    
    revision_control_->getCommitMessage(id, commit_info);
    // check duplicates
    if (checkDuplicates(id)) {
      addToModel(RevisonNode(id, commit_info));
    }

    for (unsigned int i = 0; i < commit_info.parents.size(); i++) {
      queue.push_back(commit_info.parents[i]);
    }

    queue.pop_front();
  }

  branches_.push_back(branch);
}

void RevisionSerializationModel::sortModelByDate() {
  std::sort(revisions_.begin(), revisions_.end(), revisonNodeComparator);
}

bool RevisionSerializationModel::checkDuplicates(const OdTfDigest &digest) {
  return added_.find(digest) == added_.end();
}

void RevisionSerializationModel::addToModel(const RevisonNode &node) {
  added_.insert(node.digest);
  revisions_.push_back(node);
}

void RevisionSerializationModel::createModel(const OdTfDigest &top) {
  if (revision_control_.isNull()) throw "revision_control_ is null \n";

  OdString currnet_branch;
  revision_control_->getCurrentBranch(currnet_branch);

  clear();
  addBranchToModel(top, currnet_branch);
  sortModelByDate();
}

void RevisionSerializationModel::createModel(const std::map<OdString, OdTfDigest> &branches) {
  if (revision_control_.isNull()) throw "revision_control_ is null \n";

  clear();
  for (std::map<OdString, OdTfDigest>::const_iterator itr = branches.begin(); itr != branches.end(); itr++) {
    addBranchToModel(itr->second, itr->first);
  }
  sortModelByDate();
}

void RevisionSerializationModel::serialize(RevisionSerializable *impl) {
  impl->beginSerialize();
  impl->serializeRevisions(revisions_);
  impl->serializeBranches(branches_);
  impl->endSerialize();
}

void RevisionSerializationModel::clear() {
  added_.clear();
  revisions_.clear();
  branches_.clear();
}

void RevisionSerializableJSONImpl::clear() {
  buffer_.Clear();
  writer_.Reset(buffer_);
}

void RevisionSerializableJSONImpl::writeDigestBase64(const OdTfDigest& digest)
{
  OdAnsiString str;
  encodeToBase64((const unsigned char *)digest.data, sizeof(digest.data) / sizeof(digest.data[0]), str);
  writer_.String(str);
}

void RevisionSerializableJSONImpl::serializeRevision(const RevisonNode &rev)
{
  writer_.StartObject();

  writer_.String("revision");
  writeDigestBase64(rev.digest);

  writer_.String("author");
  writer_.String(rev.info.author);

  writer_.String("message");
  writer_.String(rev.info.message);

  writer_.String("timestamp");
  writer_.Uint64(rev.info.date.packedValue() * 1000);

  writer_.String("parents");
  writer_.StartArray();
  for (int i = 0; i < rev.info.parents.size(); i++)
    writeDigestBase64(rev.info.parents[i]);
  writer_.EndArray();
  writer_.EndObject();
}

void RevisionSerializableJSONImpl::beginSerialize()
{
  clear();
  writer_.StartObject();
}

void RevisionSerializableJSONImpl::serializeRevisions(const vector<RevisonNode> &revisions)
{
  writer_.String("revisions");
  writer_.StartArray();
  for (vector<RevisonNode>::const_iterator itr = revisions.begin(); itr != revisions.end(); itr++)
    serializeRevision(*itr);
  writer_.EndArray();
}

void RevisionSerializableJSONImpl::serializeBranches(const vector<OdString> &branches)
{
  writer_.String("branches");
  writer_.StartArray();
  for (vector<OdString>::const_iterator itr = branches.begin(); itr != branches.end(); itr++)
    writer_.String(*itr);
  writer_.EndArray();
}

void RevisionSerializableJSONImpl::endSerialize()  {
  writer_.EndObject();
}

void RevisionSerializableJSONImpl::getResult(string& res) {
  res = buffer_.GetString();
}

}
#endif
