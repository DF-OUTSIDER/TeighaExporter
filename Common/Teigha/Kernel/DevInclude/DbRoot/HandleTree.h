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

// OdHandleTree.h: interface for the OdHandleTree class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ODDB_HANDLETREE_INCLUDED_
#define _ODDB_HANDLETREE_INCLUDED_

#include "OdDbStub.h"
#include "SlotManager.h"
#define STL_USING_STACK
#include "OdaSTL.h"


//#define TREE_AS_ARRAY
#ifdef TREE_AS_ARRAY

class OdHandleTreeItemCmp
{
public:
  bool operator ()(const OdDbStub* item, const OdDbHandle& key)
  {
    return (item->getHandle() < key);
  }

  bool operator ()(const OdDbStub* item1, const OdDbStub* item2)
  {
    return (item1->getHandle() < item2->getHandle());
  }

  bool operator ()(const OdDbHandle& key, const OdDbStub* item)
  {
    return (key < item->getHandle());
  }
};

class DBROOT_EXPORT OdHandleTree : public OdArray<OdDbStub*>
{
	OdHandleTree(const OdHandleTree&);
  OdHandleTree& operator = (const OdHandleTree& );
  mutable bool m_bSorted;
  OdHandleTreeItemCmp& cmp() const
  {
    static OdHandleTreeItemCmp cmp;
    return cmp;
  }
public:
  void sort() const
  {
    if(!m_bSorted)
    {
      std::sort(((OdHandleTree*)this)->begin(), ((OdHandleTree*)this)->end(), cmp());
      m_bSorted = true;
    }
  }

  iterator lower_bound(const OdDbHandle& h, bool* pbFound = NULL)
  {
    sort();
    iterator res = std::lower_bound(begin(), end(), h, cmp());
    if(pbFound)
    {
      *pbFound = (res != end() && !cmp()(h, *res));
    }
    return res;
  }

  const_iterator lower_bound(const OdDbHandle& h, bool* pbFound = NULL) const
  {
    sort();
    const_iterator res = std::lower_bound(begin(), end(), h, cmp());
    if(pbFound)
    {
      *pbFound = (res != end() && !cmp()(h, *res));
    }
    return res;
  }

  OdDbHandle getNextAvailableHandle() const
  {
		if (size())
		{
			return OdDbHandle((OdUInt64)last()->getHandle() + 1);
		}
		else
		{
			return OdDbHandle(1);
		}
  }

  OdHandleTree(OdDbStubFactory* pStubFactory);
  ~OdHandleTree();

  OdDbStub* addObject(OdDbBaseDatabase* pDb, const OdDbHandle& h);
	OdDbStub* getObjectId(const OdDbHandle& h) const;

  OdDbStubFactoryPtr m_pStubFactory;
  OdSlotManager m_objectIdXdataSlotManager;
};

#else


namespace OdStubBTree
{
  class Node;
}


class DBROOT_EXPORT OdHandleTree
{
public:
  OdHandleTree(OdDbStubFactory* pStubFactory);
  virtual ~OdHandleTree();
  OdDbHandle getNextAvailableHandle() const
  {
    return m_NextHandle;
  }
  virtual OdDbStub* addObject(OdDbBaseDatabase* pDb, const OdDbHandle& h);
	OdDbStub* getObjectId(const OdDbHandle& h) const;
  int size() { return m_size; }
  void reserve(int) { }
protected:
  OdDbHandle         m_NextHandle;
private:
  OdStubBTree::Node* m_pRoot;
  int                m_size;
  OdDbStubFactoryPtr m_pStubFactory;

  OdSlotManager m_objectIdXdataSlotManager;
  friend class OdHandleTreeIterator;
  friend DBROOT_EXPORT void oddbSwapHandles(OdHandleTree*, const OdDbHandle& h1, const OdDbHandle& h2);
};

class DBROOT_EXPORT OdHandleTreeIterator
{
  OdStubBTree::Node* _pCurrent;
  std::stack<std::pair<OdStubBTree::Node*, int> > _stack;
  int _index;
public:
  OdHandleTreeIterator(const OdHandleTree*);
  OdDbStub* getCurrentObjectId();
  bool done();
  void step();
};

#endif // TREE_AS_ARRAY

#endif // _ODDB_HANDLETREE_INCLUDED_
