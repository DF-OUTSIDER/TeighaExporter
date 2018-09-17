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




#ifndef _RXDICTIONARY_INC_
#define _RXDICTIONARY_INC_


#include "RxObject.h"
#include "RxIterator.h"
#include "RxDefs.h"
#include "RxNames.h"

class OdString;

#include "TD_PackPush.h"

/** \details
    <group OdRx_Classes>

    This class implements Iterator objects that traverse entries in OdRxDictionary objects in an OdDbDatabase instance.
    
    Corresponding C++ library: TD_Db
    
    \remarks
    An OdRxDictionaryIterator maintains a "current position" within the entries
    of the associated dictionary, and can provide access to the key value and object at the
    current position.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link iterator_overview.html, Overview of Classes that Implement an Iterator>

    <link dictionary_iterating.html, Iterating through Items of a Dictionary>
*/
class FIRSTDLL_EXPORT OdRxDictionaryIterator : public OdRxIterator
{
public:
  ODRX_DECLARE_MEMBERS(OdRxDictionaryIterator);

  /** \details
     Returns the keyword associated with the item of the dictionary on which the iterator refers

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>
  */
  virtual OdString getKey() const = 0;

  /** \details
    Returns the ID of the item in the dictionary on which the iterator refers

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>
  */
  virtual OdUInt32 id() const = 0;
};

/** \details
  The typified smart pointer for the dictionary iterator. It is the template class created by the OdSmartPtr class.
*/
typedef OdSmartPtr<OdRxDictionaryIterator> OdRxDictionaryIteratorPtr;


/** \details
    <group OdRx_Classes> 
    This class implements the Dictionary object.
    
    \remarks
    * Each instance of OdRxbDictionary is a single container object, in which items are searched, added, modified, and deleted.  
    * Each dictionary item is associated with an unique textual string named keyword and 32-Bit ID with an unique OdDbObject.
    * Anonymous names are signified by specifying a name starting with an asterisk; e.g., *U. 
       A unique name (also starting with an asterisk) will be constructed for the entry.
    * Entry names honor the rules of Symbol names
        * Names may be any length.
        * Names are case-insensitve
        * Names may not contain any of the following characters: | * \ : ; < > ? " , equals

    \sa
    OdRxDictionaryIterator
    
    Corresponding C++ library: TD_Db
    
    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary.html, Working with Dictionaries>
*/
class FIRSTDLL_EXPORT OdRxDictionary : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxDictionary);
  
  /** \details
    Allocates storage for the specified number of items in this dictionary object.

    \param minSize [in]  Minimum number of items for allocating. 

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual void reserve(
    OdUInt32 minSize);

  /** \details
    Returns the /non-typified smart pointer/ to the instance that is associated with the dictionary item 
    specified by the keyword.

    \param key [in]  Item keyword as a string value.

    \remarks
    Returns a Null if the specified item is not found.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdRxObjectPtr getAt(
    const OdString& key) const = 0;
    
  /** \details
    Returns the /non-typified smart pointer/ to the instance that is associated with the dictionary item 
    specified by the ID.

    \param id [in]  Item ID as an integer value.

    \remarks
    Returns a Null if the specified item is not found.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdRxObjectPtr getAt(
    OdUInt32 id) const = 0;

  /** \details
    Puts the instance specified by the smart pointer into the dictionary object and associates it with the specified keyword.
    
    \param key [in]  Keyword as a string value.
    \param pObject [in]  Pointer to the instance to be put.
    \param pRetId [in]  Pointer to an OdUInt32 to receive the Entry ID of the entry.
    
    \returns
    Returns a smart pointer to the object at this entry prior to the call.
    \remarks
    Keyword and smart pointer must not be Null.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdRxObjectPtr putAt(
    const OdString& key, 
    OdRxObject* pObject, 
    OdUInt32* pRetId = 0) = 0;
    
  /** \details
    Puts the instance specified by the smart pointer into the dictionary object and associates it with the specified keyword.
    
    \param id [in]  ID as an integer value.
    \param pObject [in]  Pointer to the instance to be put.
    
    \returns
    Returns a smart pointer to the object at this entry prior to the call.
    \remarks
    Smart pointer must not be Null, and id > 0.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdRxObjectPtr putAt(
    OdUInt32 id, 
    OdRxObject* pObject) = 0;
  
  /** \details
    Sets the keyword for the item specified by ID for the dictionary object.
    
    \param id [in]   ID as an integer value.
    \param newKey [in]  New keyword as a string value.
    
    \returns
    Returns true if and only if successful.    
 
    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual bool resetKey(
    OdUInt32 id, 
    const OdString& newKey) = 0;

  /** \details
    Removes the item specified by keyword from the dictionary object.
    
    \param key [in]  Keyword as a string value.
    
    \remarks
    This method does not delete the instance associated with the removed item 
    and returns a smart pointer to the instance referenced by the item.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdRxObjectPtr remove(
    const OdString& key) = 0;
    
  /** \details
    Removes the item specified by keyword or ID from the dictionary object.
    
    \param id [in]  ID as an integer value.
    
    \remarks
    This method does not delete the instance associated with the removed item 
    and returns a smart pointer to the instance referenced by the item.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdRxObjectPtr remove(
    OdUInt32 id) = 0;

  void removeAll() { OdUInt32 n = numEntries();  while(n--) remove(n); }

  /** \details
    Returns true if and only if the dictionary object contains the item with the specified keyword.

    \param key [in]  Keyword as a string value.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual bool has(
    const OdString& key) const = 0;
    
  /** \details
    Returns true if and only if the dictionary object contains the item with the specified ID.

    \param id [in]  ID as an integer value.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual bool has(
    OdUInt32 id) const = 0;


  /** \details
    Returns the ID of the item specified by the keyword in the dictionary object.

    \param key [in]  Keyword as a string value.
    
    \remarks
    Returns (-1) if the keyword is not found in the dictionary.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdUInt32 idAt(
    const OdString& key) const = 0;

  /** \details
    Returns the keyword of the item specified by the ID in the dictionary object.

    \param id [in]  Entry ID.
    
    \remarks
    Returns an empty string if ID is not found in the dictionary.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
   virtual OdString keyAt(
    OdUInt32 id) const = 0;

  /** \details
    Returns the number of items in the dictionary object.

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual OdUInt32 numEntries() const = 0;

  /** \details
    Returns a new interator that can be used to traverse through items of the dictionary object.

    \param iterType [in]  Type of iterator.

    \remarks
    The iterator type can be one of the following:

    <table>
    Name                      Description
    OdRx::kDictCollated       Traverses the entries in the order they were added to the dictionary.
    OdRx::kDictSorted         Traverses the entries in alphabetical order by key value.
    </table>

    \sa
    <link dictionary_sample.html, Example of Working with a Dictionary>

    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>

    <link dictionary_iterating.html, Iterating through Items of a Dictionary>

    <link iterator_concept.html, Concept of Iterators>
  */
  virtual OdRxDictionaryIteratorPtr newIterator(
    OdRx::DictIterType iterType = OdRx::kDictCollated) = 0;

  /** \details
    Returns true if and only if keywords of the dictionary object are case-sensitive, 
    or false if keywords are case-insensitive.
    
    \sa
    <link dictionary_functionality.html, Functionality of Dictionaries>
  */
  virtual bool isCaseSensitive() const = 0;

  inline OdSmartPtr<OdRxDictionary> subDict(const OdString* path, int numSteps, bool createIfNotFound);

  inline OdSmartPtr<OdRxDictionary> subDictCr(const OdString* path, int numSteps);
};

/** \details
  The typified smart pointer for the dictionary object. This template class is a specialization 
   of the OdSmartPtr class for OdRxDictionary object.
*/
typedef OdSmartPtr<OdRxDictionary> OdRxDictionaryPtr;


/** \details
   Returns the raw pointer to the <link dictionaty_root_runtime.html, Root Run-Time Dictionary>.
*/
FIRSTDLL_EXPORT OdRxDictionary* odrxSysRegistry();


/** \details
   Returns the smart pointer to the <link rtti_class_dictionary.html, Dictionary of Registered Classes>.

    \sa
    <link rtti_concept.html, Concept of RTTI Technology>

    <link rtti_class_registering.html, Registering and Unregistering of Classes>
*/
FIRSTDLL_EXPORT OdRxDictionaryPtr odrxClassDictionary();

inline OdRxClass* odrxGetClassDesc(const OdString& className) {
  return (OdRxClass*)(odrxClassDictionary()->getAt(className).get());
}

inline OdRxClass* odrxSafeGetClassDesc(const OdString& className) {
  OdRxClass* cls = (OdRxClass*)(odrxClassDictionary()->getAt(className).get());
  if (cls)
    return cls;
  throw OdError(className + " class not registered");
}

template <class TClass>
inline OdSmartPtr<TClass> odrxCastByClassName(const OdRxObject* obj, const OdString& className) {
  if(!obj || obj->isKindOf(odrxGetClassDesc(className)))
    return (TClass*)obj;
  return OdSmartPtr<TClass>();
}

template <class TClass>
inline OdSmartPtr<TClass> odrxSafeCastByClassName(const OdRxObject* obj, const OdString& className) {
  if(!obj || obj->isKindOf(odrxSafeGetClassDesc(className)))
    return (TClass*)obj;
  return OdSmartPtr<TClass>();
}

/** \details
   Returns the smart pointer to the Dictionary of Registered Services.
*/
FIRSTDLL_EXPORT OdRxDictionaryPtr odrxServiceDictionary();


/** \details
   Creates a new dictionary object that can be modified only from a single thread and
   returns the smart pointer to it.
*/
FIRSTDLL_EXPORT OdRxDictionaryPtr odrxCreateRxDictionary();


/** \details
   Creates a new dictionary object that can be modified from multiple threads and
   returns the smart pointer to it.
*/
FIRSTDLL_EXPORT OdRxDictionaryPtr odrxCreateSyncRxDictionary();


inline OdRxDictionaryPtr OdRxDictionary::subDict(const OdString* path, int numSteps, bool createIfNotFound) {
  OdRxDictionaryPtr dir(this);
  for(int i=0; dir.get() && i<numSteps; ++i)
    dir = dir->getAt(path[i]);
  return dir;
}
inline OdRxDictionaryPtr OdRxDictionary::subDictCr(const OdString* path, int numSteps) {
  return subDict(path, numSteps, true);
}

#include "TD_PackPop.h"

#endif // _RXDICTIONARY_INC_

