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

#ifndef _ODDB_OBJECTCONTEXTCOLLECTION_H_INCLUDED_
#define _ODDB_OBJECTCONTEXTCOLLECTION_H_INCLUDED_

#include "DbObjectContext.h"
#include "TD_PackPush.h"

class OdDbObjectContextCollectionIterator;
typedef OdSmartPtr<OdDbObjectContextCollectionIterator> OdDbObjectContextCollectionIteratorPtr;

/** \details
    This class represents a collection of objects that have context items defined.

    \remarks
    Implement this interface and register it with OdDbObjectContextContextManager
	for each database that supports context items. A context collection can be made
	current, locked, or unlocked.
        
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT ODRX_ABSTRACT OdDbObjectContextCollection : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextCollection);

   /** \details
    Returns the name of the context collection.
  
	\remarks
	The context collection name is used by the context manager to identify the 
	collection and the context items that it contains.
   */
    virtual OdString name() const = 0;

   /** \details
    Returns a pointer to the current context of the context collection.
  
	\param pRequestingObject [in]  Optional pointer to the object for which you want
	the current context.
 
	\remarks
	If no current context exists, Null is returned.
   */
  virtual OdDbObjectContextPtr currentContext(
    const OdDbObject* pRequestingObject) const = 0;

   /** \details
    Sets the current context of the context collection.
  
	\param pContext [in]  Pointer to the context for which a uniqueIdentifier is defined
	to make current for the context collection.
   */
   virtual OdResult setCurrentContext(
    const OdDbObjectContext* pContext) = 0;

   /** \details
    Copies a context and adds the copy to the context collection.
  
	\param pContext [in]  Pointer to the context for which a uniqueIdentifier is defined
	to copy and add to the context collection.
 
	\remarks
	The new context is assigned a new uniqueIdentifier.
	Returns eOk if successful, or an appropriate error code if not.
   */
  virtual OdResult addContext(
    const OdDbObjectContext* pContext) = 0;

   /** \details
    Removes a context from the context collection.
  
	\param contextName [in]  Name of the context to delete from the context
	collection.
 
	\remarks
	To remove the current context from a collection, first make a different
	context current, then delete the desired context.
	Returns eOk if successful, or an appropriate error code if not.
   */
  virtual OdResult removeContext(
    const OdString& contextName) = 0;

   /** \details
    Internal use only.
   */
    virtual OdResult lockContext(
    const OdDbObjectContext* pContext) = 0;
 
   /** \details
    Internal use only.
   */
    virtual OdResult unlockContext() = 0;

   /** \details
    Internal use only.
   */
    virtual bool locked() const = 0;

   /** \details
    Returns a pointer to a copy of a named context in the collection.
  
	\param contextName [in]  Name of the context.
 
	\remarks
	If no context exists with the specified name, Null is returned.
   */
  virtual OdDbObjectContextPtr getContext(
    const OdString& contextName) const = 0;

  /** \details
    Returns whether a named context exists in the collection.
  
	\param contextName [in]  Name of the context.
 
	\returns
	Returns true if the named context exists in the collection, 
	or returns Null if it does not exist.
  */
  virtual bool hasContext(
    const OdString& contextName) const = 0;

   /** \details
    Returns an Iterator to the contexts in the collection.
   */
  virtual OdDbObjectContextCollectionIteratorPtr newIterator() const = 0;
};


////////////////////////////////////////////////////////////////////////
// class OdDbObjectContextCollectionIterator
////////////////////////////////////////////////////////////////////////

/** \details
    This class implements Iterators for objects that have context items defined and
	stored in a context collection (using the OdDbObjectContextCollections class).

    \remarks
    If an application supports context collections, you must implement this class. You
	can get an Iterator using OdDbObjectContextCollection::newIterator().
        
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT ODRX_ABSTRACT OdDbObjectContextCollectionIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextCollectionIterator);

   /** \details
    Moves the Iterator to the start of the collection.
 
	\returns
	Returns eOk if successful, or an appropriate error code if not.
   */
  virtual OdResult start() = 0;

   /** \details
    Moves the Iterator to the next context of the collection.
 
	\returns
	Returns true if successful, or returns false if the Iterator is
	at the end of the collection.
   */
  virtual bool next() = 0;

   /** \details
    Returns whether the Iterator is at the end of the collection.
 
	\returns
	Returns true if the Iterator is at the end of the collection, or
	returns false if it is not.
   */
  virtual bool done() const = 0;

   /** \details
    Returns a copy of the context located at the current position of the
	Iterator.
   */
  virtual OdDbObjectContextPtr getContext() const = 0;
};

#include "TD_PackPop.h"

#endif // _ODDB_OBJECTCONTEXTCOLLECTION_H_INCLUDED_
