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

#ifndef _ODA_HASH_MAP_H_
#define _ODA_HASH_MAP_H_

/* hash_map isn't included into STL standart initially, so implementations is
   available under different namespaces and could have implementation
   differences.                                                               */

/* Mostly STL implementations contain hash_map which uses separate hasher and predicate
   template arguments. In this implementations predicate defined as std::equal_to.
   Now this type of hash maps included in C++11 standart as unordered_map.
   This implementation contained by all compilers, which uses SGI STL, but
   Visual Studio contains it only since 2015 (previous versions contain Dinkumware hash_map
   implementation, which invokes std::less predicate, so we can't emulate unordered_map
   through this container.                                                                  */
// #ifdef UNORDEREDMAP_SUPPORTED
// Syntax: OdUnorderedMap<KeyType, ValueType, HashFunction, EqualityPredicate, Allocator>

#if defined(__GNUC__)
// GCC
#define _GLIBCXX_PERMIT_BACKWARD_HASH
#include <ext/hash_map>
#define UNORDEREDMAP_SUPPORTED
// This is for GCC-3.1+, GCC 3.0.x puts hash_map in the std namespace.
// Older compilers such as GCC before version 3.0 do not keep
// extensions in the `ext' directory, and ignore the `std' namespace.
#define OdUnorderedMap __gnu_cxx::hash_map
#define OdUnorderedMap_DefaultHasher __gnu_cxx::hash
//for x86
#if OD_SIZEOF_LONG == 4
namespace __gnu_cxx {
  template<>
  struct hash<OdUInt64>
  {
    size_t operator()(const OdUInt64& handle) const { return handle; }
  };
}
#endif
#elif defined(__sun)
// sparc on Sun Studio compiler, solaris
// SUNpro uses Cstd by default which is haven't hash_map. But SUNpro ships with -library=stlport4
// which is could be enabled by compiler and linker options.
// LIBSTLPORT = -library=no%Cstd -L../../lib -lstlport_sunpro64
// First of all include <utility> to detect which STL is actually configured to use:
#include <utility>
// Enable hash_map only if stlport is used:
#ifdef _STLPORT_VERSION
#include <hash_map>
#define UNORDEREDMAP_SUPPORTED
#define OdUnorderedMap std::hash_map
#define OdUnorderedMap_DefaultHasher std::hash
#endif // _STLPORT_VERSION
#elif defined(_MSC_VER) || defined(__BORLANDC__)
// MSVC, BB (Dinkumware)
#if (_MSC_VER >= 1900)
#include <unordered_map>
#define UNORDEREDMAP_SUPPORTED
#define OdUnorderedMap std::unordered_map
#define OdUnorderedMap_DefaultHasher std::hash
#endif
#elif defined(sgi)
// SGI (StlPort)
#include <hash_map>
#define UNORDEREDMAP_SUPPORTED
#define OdUnorderedMap std::hash_map
#define OdUnorderedMap_DefaultHasher std::hash
#elif defined(__hpux)
// HPUX
// HP aCC doesn't include an SGI-like hash_map. For this platform (or
// any others using Rogue Wave Software's Tools.h++ library), we wrap
// around them in std::
#include <rw/stdex/hashmap.h>
#if 1 // to wrap in std if necessary
//#ifndef RW_ALLOCATOR_PLAIN
namespace std {
  template <class DataType> struct hash {
    size_t operator()(const DataType& x) const { return static_cast<size_t>(x); }
  };
  template < class KeyVal,
             class Value,
             class _HashFcn = hash<KeyVal>,
             class Pred = equal_to<KeyVal>,
             class Alloc = allocator <pair <const KeyVal, Value> > >
  class hash_map : public rw_hashmap < KeyVal, Value, _HashFcn, Pred, Alloc > { };
}
#define UNORDEREDMAP_SUPPORTED
#define OdUnorderedMap std::hash_map
#define OdUnorderedMap_DefaultHasher std::hash
//#endif
#else // clear way without wrapping
#define UNORDEREDMAP_SUPPORTED
#define OdUnorderedMap rw_hashmap
#define OdUnorderedMap_DefaultHasher rw_hash
#endif
#else
// List of known platforms which isn't support hash_map:
// MSVC6 and earlier
// solaris w/o stlport
#error Unknown platform... check hash_map support
#endif

/* Dinkumware hash_map implementation require following specification of hasher and predicate:
  struct Hasher {
    enum { bucket_size = 4, min_buckets = 8 };
    size_t operator()(const Key &key) const {
      return hash(key);
    }
    bool operator()(const Key &ls, const Key &rs) const {
      return ls < rs;
    }
  };
  It is combines hash function and predicate. We can use this specification as client for
  OdUnorderedMap implementation too using two comparisons through less predicate:
  equal_to == !(l < r) && !(r < l). Of course this solution can cause perfomance loss
  if keys comparison isn't simple, but at least it will work onto all platforms.          */
// #ifdef HASHMAP_SUPPORTED
// Syntax: OdHashMapLs<KeyType, ValType, HashWithLessPredicate, Allocator>

#ifdef UNORDEREDMAP_SUPPORTED
#define HASHMAP_SUPPORTED
template <class Key> struct od_std_hash_map_def_func : public OdUnorderedMap_DefaultHasher<Key>, public std::less<Key> {};
template <class Key, class _HashFcn>
struct od_std_hash_map_generalized_equal_to : protected _HashFcn
{ bool operator()(const Key& leftArg, const Key& rightArg) const {
    return !_HashFcn::operator()(leftArg, rightArg) && !_HashFcn::operator()(rightArg, leftArg); } };
template <class Key, class Value, class _HashFcn = od_std_hash_map_def_func<Key>, class Alloc = std::allocator<std::pair<const Key, Value> > >
class OdHashMapLs : public OdUnorderedMap<Key, Value, _HashFcn, od_std_hash_map_generalized_equal_to<Key, _HashFcn>, Alloc> {};
#elif defined(_MSC_VER) || defined(__BORLANDC__)
// MSVC (this container available only since vc7), BB
#if (_MSC_VER >= 1300) || defined(__BORLANDC__)
#include <hash_map>
#define HASHMAP_SUPPORTED
#define OdHashMapLs stdext::hash_map
#define OdHashMap_DefaultHasher stdext::hash_compare
#endif
#endif

/* Typically default implementation of predicate is enough for our needs.
   OdHashMap template doesn't require specification of predicate and uses default
   which is depends from hash_map implementation.                                 */
// #ifdef HASHMAP_SUPPORTED
// Syntax: OdHashMap<KeyType, ValType, HashFunction, Allocator>
#ifdef UNORDEREDMAP_SUPPORTED
template <class Key, class Value, class HashFcn = OdUnorderedMap_DefaultHasher<Key>, class Alloc = std::allocator<std::pair<const Key, Value> > >
class OdHashMap : public OdUnorderedMap<Key, Value, HashFcn, std::equal_to<Key>, Alloc> {};
#elif defined(HASHMAP_SUPPORTED)
template <class Key, class HashFcn>
struct od_std_hash_map_add_less_predicate : public HashFcn, public std::less<Key> {
  size_t operator()(const Key &key) const { return HashFcn::operator()(key); }
  bool operator()(const Key &ls, const Key &rs) const { return std::less<Key>::operator()(ls, rs); } };
template <class Key, class Value, class HashFcn = OdHashMap_DefaultHasher<Key>, class Alloc = std::allocator<std::pair<const Key, Value> > >
class OdHashMap : public OdHashMapLs<Key, Value, od_std_hash_map_add_less_predicate<Key, HashFcn>, Alloc> {};
#endif

#endif //_ODA_HASH_MAP_H_
