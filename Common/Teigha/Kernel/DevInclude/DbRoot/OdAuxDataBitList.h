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

// OdAuxDataBitList.h: interface for the OdAuxDataBitList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ODAUXDATABITSET_INCLUDED_
#define _ODAUXDATABITSET_INCLUDED_

//----------------------------------------------------------
//
// OdAuxData
//
//----------------------------------------------------------
class OdAuxData
{
public:
  union Data
  {
    unsigned _n;
    void*    _p;
  };

  OdAuxData() { _data._n = 0; _data._p = 0; }

  void set(const OdAuxData& src) { _data = src._data; }
  void set(unsigned n) { _data._n = n; }
  template <class T>
    void set(T* p) { _data._p = (void*)p; }

  void get(OdAuxData& dst) { dst._data = _data; }
  void get(unsigned long& n) const { n = _data._n; }
  void get(unsigned& n) const { n = _data._n; }
  template <class T>
    void get(T*& p) const { p = static_cast<T*>(_data._p); }
  void* getPtr() { return _data._p;  }
  const void* getPtr() const { return _data._p;  }

private:
  Data _data;
};

//----------------------------------------------------------
//
// OdAuxDataBitList 
// Requires TBitSet interface:
//   integer flags() const;
//   void setFlags(integer flags, integer mask);
//
// Application is responsible for freeing memory used by XData
//
//----------------------------------------------------------
template <class TBitVal,
          class TBitSet,
          TBitVal kControlBit,
          TBitVal kFirstBit,
          TBitVal kLastBit
         >
class OdAuxDataBitList : public TBitSet
{
public:

  class Node : public OdAuxData
  {
  public:
    // Constructor.
    //
    template <class T>
    static Node* create(T data)
    { 
      Node* pNode = new Node(); pNode->set(data); return pNode;
    }
    // Destructor.
    //
    void release() { delete this; }
    // Accessors.
    //
    Node* next() { return _pNext; }
    const Node* next() const { return _pNext; }
    void setNext(Node* p) { _pNext = p; }

  private:
    Node() : _pNext(0) { }
    Node* _pNext;
  };

  OdAuxDataBitList() {}
  ~OdAuxDataBitList()
  {
    if (!(this->flags() & kControlBit))
    {
      // Free nodes if there is a chain of nodes.
      Node *iter = begin(),
           *curr = 0;
      while (iter)
      {
        curr = iter;
        iter = iter->next();
        curr->release();
      }
    }
  }

  template <class T>
  void getData(OdUInt32 id, T& res, bool bRemove = false)
  {
    ODA_ASSERT(id>=kFirstBit && id<=kLastBit);
    
    if (!(id & this->flags()))
    {
      // There is not such Data in the stub
      res = (T)0;
      return;
    }

    if (this->flags() & kControlBit)
    {
      // There is a single Xdata in stub
      _root.get(res);
      if (bRemove)
      {
        this->setFlags(0, id);
        this->setFlags(0, kControlBit);
        _root.set((void*)0);
      }
    }
    else
    {
      // There are a quantity of Xdata in stub, so iterate thru list
      Node* pNode = find(id);
      ODA_ASSERT(pNode);
      pNode->get(res);
      if (bRemove)
      {
        // Reset the flag and relink the list
        this->setFlags(0,id);
        remove(pNode);
      }
    }
  }

  template <class T>
  void setData(OdUInt32 id, T val)
  {
    ODA_ASSERT(id>=kFirstBit && id<=kLastBit);

    if (this->flags() & id)
    {
      // There are such data already -> replace
      if (this->flags() & kControlBit)
      {
        _root.set(val);
      }
      else
      {
        Node* pNode = find(id);
        pNode->set(val);
      }
    }
    else
    {
      if (!(this->flags() & kControlBit) && _root.getPtr() == 0)
      {
        // There is nothing yet -> assign
        _root.set(val);
        this->setFlags(kControlBit, kControlBit);
      }
      else
      {
        if (this->flags() & kControlBit)
        {
          // There is only a single data -> transform it to Node
          _root.set(Node::create(_root)); 
          this->setFlags(0, kControlBit);
        }

        // Find an appropriate place in the list and insert a new Node
        Node* pNode = Node::create(val);
        Node* pBefore = find(id);
        insert(pNode, pBefore);
      }
      this->setFlags(id, id);
    }
  }

private:
  Node* begin()
  {
    ODA_ASSERT(!(this->flags() & kControlBit));
    return static_cast<Node*>(_root.getPtr());
  }

  void insert(Node* pNode, Node* pBefore)
  {
    if (!pBefore)
    {
      pNode->setNext(begin());
      _root.set(pNode);
    }
    else
    {
      pNode->setNext(pBefore->next());
      pBefore->setNext(pNode);
    }
  }

  void remove(Node* pNode)
  {
    if (begin() == pNode)
    {
      _root.set(pNode->next());
    }
    else
    {
      Node* pBefore = begin();
      while (pNode != pBefore->next())
      {
        pBefore = pBefore->next();
      }
      pBefore->setNext(pNode->next());
    }
    pNode->release();

    // Transform single node to simple value (pointer or integer).
    if (begin() && begin()->next() == 0)
    {
      pNode = begin();
      _root = *pNode;
      pNode->release();
      this->setFlags(kControlBit, kControlBit);
    }
  }

  Node* find(OdUInt32 id)
  {
    ODA_ASSERT(id>=kFirstBit && id<=kLastBit);
    OdUInt32 mask = kFirstBit;
    Node *iter = begin(),
         *curr = 0;
    while (mask < id)
    {
      if (this->flags() & mask)
      {
        ODA_ASSERT(iter);
        curr = iter;
        iter = iter->next();
      }
      mask <<= 1;
    }

    ODA_ASSERT((mask^id) == 0);
    if (this->flags() & mask)
    {
      curr = iter;
    }
    return curr;
  }

private:
  OdAuxData _root;
};

#endif // _ODAUXDATABITSET_INCLUDED_
