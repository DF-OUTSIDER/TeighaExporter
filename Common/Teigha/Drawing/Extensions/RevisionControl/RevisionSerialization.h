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

/* Use example
  RevisionSerializationModel model(rev_ctrl);
  model.createModel(head_branch);
  model.serialize(out);
*/

#ifndef REVISION_SERIALIZATION_
#define REVISION_SERIALIZATION_

#if __cplusplus>=199711L

#include <algorithm>
#include <deque>
#include <set>
#include <vector>

#include "OdaCommon.h"
#include "RevisionControl.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

namespace revision_control {
using namespace std;
using namespace rapidjson;

struct RevisonNode {
  OdTfDigest     digest;
  OdTfCommitInfo info;

  RevisonNode(OdTfDigest digest, OdTfCommitInfo info) 
    : digest(digest), info(info) {}
};

struct RevisionSerializable {
  virtual void beginSerialize() = 0;

  virtual void serializeRevisions(const vector<RevisonNode> &revisions) = 0;

  virtual void serializeBranches(const vector<OdString> &branches) = 0;

  virtual void endSerialize() = 0;
};

class RevisionSerializationModel {
protected:
  struct RevisonNodeComparator {
    bool operator() (const RevisonNode &obj1, const RevisonNode &obj2) const {
      return obj1.info.date > obj2.info.date;
    }
  } revisonNodeComparator;

private:
  void addBranchToModel(const OdTfDigest &head, const OdString& branch);

  void sortModelByDate();

  bool checkDuplicates(const OdTfDigest &digest);

  void addToModel(const RevisonNode &node);

public:
  RevisionSerializationModel(RevisionControlPtr rev_ctrl) 
    : revision_control_(rev_ctrl) {}

  void createModel(const OdTfDigest &top);

  void createModel(const std::map<OdString, OdTfDigest> &branches);

  void serialize(RevisionSerializable *impl);

  void clear();

private:
  set<OdTfDigest>     added_;
  vector<RevisonNode> revisions_;
  vector<OdString>    branches_;
  RevisionControlPtr  revision_control_;
};

class RevisionSerializableJSONImpl : public RevisionSerializable {
protected:
  void clear();

  void writeDigestBase64(const OdTfDigest& digest);

  void serializeRevision(const RevisonNode &rev);;
public:

  /** \details
    Prepare and and open stream for serialization
    \note
    */
  virtual void beginSerialize();

  /** \details
  Serialize Revisions
  \param revisions [in] list of revisions
  \note
  */
  virtual void serializeRevisions(const vector<RevisonNode> &revisions);

  /** \details
  Serialize Branches
  \param branches [in] list of branch names
  \note
  */
  virtual void serializeBranches(const vector<OdString> &branches);

  /** \details
  Close stream for serialization
  \note
  */
  virtual void endSerialize();

  /** \details
  Get result of serialisation as string
  \param res [branch] output string
  \note
  */
  void getResult(string& res);

private:
  StringBuffer buffer_;
  PrettyWriter<StringBuffer> writer_;
};
}
#endif

#endif // REVISION_SERIALIZATION_
