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




#ifndef OD_DBIDBUFFER_H
#define OD_DBIDBUFFER_H

#include "TD_PackPush.h"

#include "DbObject.h"

/** \details
    This class is the base class for Iterator classes used to access the 
    Object ID's in an OdDbIdBuffer object.

    <group OdDb_Classes>
*/
class  OdDbIdBufferIterator : public OdRxObject
{
public:
  /** \details
      Sets this iterator to point to the first element in the associated container. 
  */
  virtual void start() = 0;

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() const = 0;

  /** \details
      Increments the current object for this iterator to the next object in the associated container.
  */
  virtual void next() = 0;

  /** \details
      Returns the Object ID of the current object referenced by this iterator.
  */
  virtual OdDbObjectId id() const = 0;

  /** \details
      If the specified Object ID is present in the associated container, makes this object the 
      current object referenced by this iterator. If the specified Object ID is not found, 
      the current object referenced by this iterator will be invalid, and done() returns true.
  */
  virtual bool seek(OdDbObjectId id) = 0;

  /** \details
      Removes the Object ID referenced by this iterator as the current element.  The element 
      immediately following the removed element becomes the current element for this iterator. 
  */
  virtual void removeCurrentId() = 0;
};


/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbIdBufferIterator object pointers.
*/
typedef OdSmartPtr<OdDbIdBufferIterator> OdDbIdBufferIteratorPtr;

/** \details
    Represents an ID buffer object in an OdDbDatabase instance.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbIdBuffer : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbIdBuffer);

  /** \details
      Constructor (no arguments).
  */
  OdDbIdBuffer();

  /** \details
      Returns an Iterator object that can be used to traverse the Object ID's in the OdDbIdBuffer object.

      \returns
      SmartPointer to the new iterator.
  */
  OdDbIdBufferIteratorPtr newIterator();

  /** \details
      Appends an Object ID onto this object's ID list.
  */
  void addId(const OdDbObjectId &id);

  /** \details
      Appends a set of Object IDs onto this object's ID list.
  */
  void addIds(const OdDbObjectIdArray& ids);

  /** \details
      Returns the number of Object ID's referenced by this OdDbIdBuffer object.
  */
  int numIds() const;

  /** \details
      Clears this object's Object ID list.
  */
  void removeAll();

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
};
 

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbIdBuffer object pointers.
*/
typedef OdSmartPtr<OdDbIdBuffer> OdDbIdBufferPtr;

#include "TD_PackPop.h"

#endif  // OD_DBIDBUFFER_H

