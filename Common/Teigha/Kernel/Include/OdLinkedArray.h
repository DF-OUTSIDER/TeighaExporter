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


#ifndef ODLINKEDARRAY_H_INCLUDED
#define ODLINKEDARRAY_H_INCLUDED

#include "OdArray.h" // to use allocators

template <class T, class A = OdObjectsAllocator<T> >
class OdLinkedArray
{
public:
  typedef typename A::size_type size_type;

  class PAGE
  {
  public:
    PAGE*     _next;
    PAGE*     _prev;
    size_type _size;

    // Page allocation/deallocation
    //
    static PAGE* allocate(size_type pageSize)
    {
      PAGE* page = (PAGE*)::odrxAlloc(sizeof(PAGE) + pageSize * sizeof(T));
      if (!page)
        throw OdError(eOutOfMemory);
      page->_prev = 0;
      page->_next = 0;
      page->_size = 0;
      return page;
    }
    void release()
    {
      A::destroy(items(), _size);
      ::odrxFree(this);
    }

    // Access to items on page
    //
    T& at(size_type index)
    {
      assertValid(index);
      return *(items(index));
    }
    const T& at(size_type index) const
    {
      assertValid(index);
      return *(items(index));
    }

    // Add items to page
    //
    size_type append(const T& value)
    {
      A::construct(items(_size), value);
      return ++_size;
    }
    size_type insertAt(size_type i, const T& value)
    {
      size_type moved = _size - i;
      ODA_ASSERT(moved<=_size);
      A::move(items(i+1), items(i), moved);
      A::construct(items(i), value);
      return ++_size;
    }
    void resize(size_type newSize, const T& value)
    {
      ODA_ASSERT(newSize>_size);
      A::constructn(items(_size), newSize-_size, value);
      _size = newSize;
    }
    void resize(size_type newSize)
    {
      if(newSize>_size) {
        A::constructn(items(_size), newSize-_size);
        _size = newSize;
      }
      else {
        A::destroy(items(0), _size - newSize);
        _size = newSize;
      }
    }
    void moveFrom(PAGE* src, size_type start)
    {
      ODA_ASSERT(src && (this!=src));
      ODA_ASSERT(start && start<src->_size);

      A::copy(items(_size), src->items(), start);
      _size += start;

      src->_size -= start;
      A::move(src->items(), src->items(start), src->_size);
    }
    void remove(size_type i)
    {
      A::destroy(items(i), 1);
      --_size;
      if ( i < _size )
        A::move(items(i), items(i+1), _size - i);
    }
    void removeLast()
    {
      A::destroy(items(_size--), 1);
    }

  private:
    T* items(size_type i = 0)
    { 
      return (T*)((OdUInt8*)this + sizeof(PAGE)) + i;
    }
    void assertValid(size_type index) const
    { 
      if (index >= _size) 
      {
        ODA_FAIL();
        throw OdError(eInvalidIndex);
      }
    }
  };

private:
  // Add/insert new page
  //
  PAGE* addPage()
  {
    PAGE* page = PAGE::allocate(_pageSize);

    if (!_last)
    {
      ODA_ASSERT(!_first);
      _first = page;
      _last = page;
    }
    else
    {
      ODA_ASSERT(_first);
      _last->_next = page;
      page->_prev = _last;
      _last = page;
    }

    ++_pageNums;
    return page;
  }

  PAGE* insertPage(PAGE* prev)
  {
    PAGE* page = PAGE::allocate(_pageSize);

    if (!_first)
    {
      ODA_ASSERT(!prev && !_last);
      _first = page;
      _last = page;
    }
    else if (!prev)
    {
      ODA_ASSERT(_first && _last);
      _first->_prev = page;
      page->_next = _first;
      _first = page;
    }
    else
    {
      ODA_ASSERT(_first && _last);
      if (prev == _last)
      {
        _last = page;
      }
      else
      {
        ODA_ASSERT(prev->_next);
        prev->_next->_prev = page;
        page->_next = prev->_next;
      }
      prev->_next = page;
      page->_prev = prev;
    }

    ++_pageNums;
    return page;
  }

  PAGE* _first;
  PAGE* _last;
  size_type _pageNums;
  size_type _pageSize;
  size_type _itemNums;

private:
  // Prohibit usage
  OdLinkedArray(const OdLinkedArray&);
  OdLinkedArray& operator = (const OdLinkedArray&);
  
  // Construction/destruction
  //
public:
  OdLinkedArray(OdUInt32 pageSize = 0x10)
    : _first(0)
    , _last(0)
    , _pageNums(0)
    , _pageSize(pageSize)
    , _itemNums(0)
  {
  }

   ~OdLinkedArray()
  {
    clear();
  }

  void reserve(size_type) {};   // For compatibility with other array types

  void clear()
  { 
    PAGE *next, *curr = _first;
    while (curr)
    {
      next = curr->_next;
      curr->release();
      curr = next;
    }
    _first = 0;
    _last = 0;
    _pageNums = 0;
    _itemNums = 0;
  }
  
  void freeExtra() {
    if(_last) {
      PAGE *curr = _last->_next;
      _last->_next = 0;
      while ( curr ) {
        PAGE *tmp = curr;
        ODA_ASSERT( curr->_size==0 );
        curr->release();
        curr = tmp;
      }
    }
  }
  
  T removeLast()
  {
    if (_itemNums == 0)
    {
      ODA_FAIL();
      return T();
    }
    T data = _last->at(_last->_size - 1);
    --_itemNums;
    _last->removeLast();
    if (!_last->_size)
    {
      --_pageNums;
      PAGE* p = _last->_prev;
      _last->release();
      _last = p;
      if (!_last)
        _first = 0;
      else
        _last->_next = 0;
    }
    return data;
  }

  // Iterators
  //
  class const_iterator
  {
  public:
    const_iterator()
      : _page(0)
      , _ind(0) {}

    const_iterator(OD_TYPENAME2 OD_LINKEDARRAY_SCOPE PAGE* page, unsigned ind = 0)
      : _page(page)
      , _ind(ind) {}

    const T& operator*() const
    {
      return _page->at(_ind);
    }

    operator const T*() const {
      return &_page->at(_ind);
    }

    const T* operator->() const
    {
      return &**this;
    }

    const_iterator& operator++()
    {
      next();
      return *this;
    }

    const_iterator operator++(int) 
    {
      const_iterator tmp = *this;
      ++*this;
      return tmp;
    }

    const_iterator& operator--()
    {
      prev();
      return *this;
    }

    const_iterator operator--(int)
    {
      const_iterator tmp = *this;
      --*this;
      return tmp;
    }

    const_iterator operator += (int n) {
      seek(n);
      return *this;
    }

    const_iterator operator -= (int n) {
      seek(-n);
      return *this;
    }

    bool operator==(const const_iterator& it) const
    {
      return (_page == it._page && _ind == it._ind);
    }

    bool operator!=(const const_iterator& it) const
    {
      return (!(*this == it));
    }

    bool done() const
    {
      return (!_page || (_ind >= _page->_size/* && !_page->_next*/));
    }
  protected:
    void seek(int n) {
      if(n > 0)
        next(n);
      else
        prev(-n);
    }

    void next(int n=1)
    {
      if (!_page)
      {
        ODA_FAIL();
        return;
      }

      _ind += n;
      while (_page && _page->_next && _ind>=_page->_size)
      {
        _ind -= _page->_size;
        _page = _page->_next;
      }
    }

    void prev(int n=1)
    {
      if (!_page)
      {
        ODA_FAIL();
        return;
      }

      while (_page && _page->_prev && _ind < (unsigned)n) {
        _page = _page->_prev;
        _ind += _page->_size;
      }
      _ind -= n;
    }

    OD_TYPENAME2 OD_LINKEDARRAY_SCOPE PAGE*    _page;
    unsigned _ind;

    friend class OdLinkedArray<T,A>;
  };

  class iterator : public OD_LINKEDARRAY_SCOPE const_iterator
  {
  public:
    iterator() {}

    iterator(OD_TYPENAME2 OD_LINKEDARRAY_SCOPE PAGE* page, unsigned ind = 0)
      : const_iterator(page, ind) {}

    T& operator*() const
    {
      return this->_page->at(this->_ind);
    }

    operator T*() {
      return &(this->_page->at(this->_ind));
    }

    T* operator->() {
      return &**this;
    }

    iterator& operator++()
    {
      this->next();
      return *this;
    }

    iterator operator++(int) 
    {
      iterator tmp = *this;
      ++*this;
      return tmp;
    }

    iterator operator += (int n) {
      this->seek(n);
      return *this;
    }

    iterator operator -= (int n) {
      this->seek(-n);
      return *this;
    }

    iterator& operator--()
    {
      this->prev();
      return *this;
    }

    iterator operator--(int)
    {
      iterator tmp = *this;
      --*this;
      return tmp;
    }
  };
 
  // Return iterator for beginning/end of sequence
  //
  iterator begin()
  { 
    return iterator(_first, 0);
  }
  const_iterator begin() const
  { 
    return const_iterator(_first, 0);
  }
  iterator end()
  { 
    return iterator(_last, (_last ? _last->_size : 0));
  }
  const_iterator end() const
  { 
    return const_iterator(_last, (_last ? _last->_size : 0));
  }

  // Find value in sequence
  //
  iterator find(const T& val)
  {
    iterator it = begin(),
             endIt = end();
    while (it != endIt && *it != val)
    {
      ++it;
    }
    return it;
  }
  const_iterator find(const T& val) const
  {
    const_iterator it = begin(),
                   endIt = end();
    while (it != endIt && *it != val)
    {
      ++it;
    }
    return it;
  }
  
  bool contains(const T& val) const
  {
    const_iterator  it = find(val);
    return !it.done();
  }

  // Add new items to array
  //   resize
  //   insert
  //   append
  //
  void resize(size_type newSize)
  {
    int d = newSize - size();
    if (d > 0)
    {
      if (_last) {
        // fill last existed page
        int d0;
        if(_last->_size < _pageSize) {
          d0 = odmin(d, (int)(_pageSize - _last->_size));
          _last->resize(_last->_size + d0);
          d -= d0;
        }
        // reuse clear pages
        while (d && _last->_next) {
          _last = _last->_next;
          ODA_ASSERT(_last->_size==0);
          d0 = odmin(d, (int)_pageSize);
          _last->resize(d0);
          d -= d0;
        }
      }
      // add new full pages
      while (d>=(int)_pageSize)
      {
        addPage()->resize(_pageSize);
        d -= _pageSize;
      }
      // add new partial page
      if (d>0)
      {
        addPage()->resize(d);
      }

      _itemNums = newSize;
    }
    else if (d < 0)
    {
      while ((size_type)-d > _last->_size) {
        d += _last->_size;
        PAGE* pToRemove = _last;
        _last = _last->_prev;
        if (_last)
          _last->_next = NULL;
        else
          _first = NULL;
        pToRemove->release();
        --_pageNums;
      }
      if (-d > 0) {
        d += _last->_size;
        _last->resize(d);
      }
      _itemNums = newSize;
    }
  }
  
  iterator insert(iterator before, const T& val = T())
  {
    if (before.done())
    {
      append(val);
      before = end();
      --before;
    }
    else
    {
      size_type curInd = before._ind;
      PAGE* curPage = before._page;
      ODA_ASSERT(curPage);

      if (curPage->_size < _pageSize)
      {
        curPage->insertAt(curInd, val); // there is free space in current page
      }
      else
      {
        PAGE* prevPage = curPage->_prev;
        if (!prevPage || (prevPage->_size + curInd) >= _pageSize)
        {
          prevPage = insertPage(prevPage); // there is not enough free space in previous page
        }
        if (curInd)
        {
          prevPage->moveFrom(curPage, curInd); // move part of data to previous page
        }
        ODA_ASSERT(prevPage);
        prevPage->append(val);
        before = iterator(prevPage, prevPage->_size-1);
      }

      ++_itemNums;
    }
    return before;
  }

  T remove(iterator at)
  {
    size_type curInd = at._ind;
    PAGE* curPage = at._page;
    if (curPage == _last && curInd == (_last->_size - 1))
      return removeLast();
    T data = curPage->at(curInd);
    if (curPage->_size == 1) // removing last page item
    {
      if (curPage->_prev)
        curPage->_prev->_next = curPage->_next;
      else
        this->_first = curPage->_next;
      if (curPage->_next)
        curPage->_next->_prev = curPage->_prev;
      curPage->release();
      --this->_pageNums;
    }
    else
    {
      curPage->remove(curInd);
    }
    --this->_itemNums;
    return data;
  }

  void append(const T& value)
  { 
    if (_last && _last->_size < _pageSize)
    {
      _last->append(value);
    }
    else
    {
      addPage()->append(value);
    }

    ++_itemNums;
  }

  iterator append() { 
    resize(size()+1);
    return --end();
  }

  T& first() {
    return *begin();
  }

  const T& first() const { 
    return *begin();
  }

  T& last() { 
    return *(--end());
  }

  const T& last() const { 
    return *(--end());
  }

  bool empty() const {
    return size()==0;
  }

  size_type size() const
  { 
#ifdef _DEBUG
    size_type itemNums = 0,
              pageNums = 0;
    PAGE* page = _first;
    while (page)
    {
      ++pageNums;
      itemNums += page->_size;
      page = page->_next;
    }
    ODA_ASSERT(_itemNums == itemNums);
    ODA_ASSERT(_pageNums == pageNums);
#endif
    return _itemNums;
  }
};

#endif // ODLINKEDARRAY_H_INCLUDED
