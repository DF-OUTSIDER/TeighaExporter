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
#ifndef __DG_DICTIONARYTABLERECORD_H__
#define __DG_DICTIONARYTABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"

/** \details
    This class represents Dictionary records in the OdDgDictionaryTable in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDictionaryTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDictionaryTableRecord);
public:

  OdDgElementId getItemId() const;
  void setItemId(const OdDgElementId& elementId);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDictionaryTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgDictionaryTableRecord> OdDgDictionaryTableRecordPtr;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDictionaryIterator object pointers.
*/

class OdDgDictionaryIterator;
typedef OdSmartPtr<OdDgDictionaryIterator> OdDgDictionaryIteratorPtr;

/** \details
    This class represents Dgn Dictionary iterator.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDictionaryIterator : public OdRxIterator
{
ODRX_DECLARE_MEMBERS(OdDgDictionaryIterator);
  
public:

  OdDgDictionaryIterator() {}

  /** \details
    Returns true if and only if the traversal by this iterator object is completed.
  */
    virtual bool done() const = 0;

  /** \details
    Sets the iterator object to the next element of the container.

    \remarks
    Returns true if and only if not at the end of the list.
  */
  virtual bool next() = 0;

  /** \details
    Returns the non-typified smart pointer to the object on which the iterator object refers. 
  */
  /** \details
      Returns the name (key) of the dictionary entry currently referenced
      by this Iterator object.
  */
  virtual OdString name() const = 0;

  /** \details
      Opens the dictionary entry currently referenced by this Iterator object.    

      \param mode [in]  Mode in which to open the element.

      \returns
      Returns a SmartPointer to the opened element.
  */
  virtual OdDgElementPtr getElement(
    OdDg::OpenMode openMode = OdDg::kForRead) = 0;

  /** \details
      Returns the Element ID of the dictionary entry currently referenced by
      this Iterator object.
  */
  virtual OdDgElementId elementId() const = 0;

  /** \details
      Opens the dictionary entry currently referenced by this Iterator object in OdDb::kForRead mode.

      \returns
      Returns a SmartPointer to the opened object.
  */
  virtual OdRxObjectPtr object() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDictionary object pointers.
*/

class OdDgDictionary;
typedef OdSmartPtr<OdDgDictionary> OdDgDictionaryPtr;

/** \details
    This class represents Dgn Dictionary in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDictionary : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgDictionary);
public:

  virtual OdString getName() const;
  virtual bool setName(const OdString& name);

  virtual OdDgElementId getItemId() const;
  virtual void setItemId(const OdDgElementId& elementId);

  virtual OdDgElementId getDefaultId() const;
  virtual void setDefaultId(const OdDgElementId& elementId);

  virtual OdUInt16  getOwnerFlags() const;
  virtual void      setOwnerFlags( OdUInt16 uFlags );

  virtual OdUInt16  getCloneFlags() const;
  virtual void      setCloneFlags( OdUInt16 uFlags );

  virtual bool      hasSubDictionaries() const;
  virtual bool      isRootDictionary() const;

  virtual OdDgElementId getAt( const OdString& recordName, bool getErasedRecord = false) const;
  virtual bool has( const OdDgElementId& elementId ) const;
  virtual void add( OdDgDictionaryPtr pDict, bool bCheckData = true );
  virtual OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;
  virtual OdDgDictionaryIteratorPtr newIterator( bool atBeginning = true, bool skipDeleted = true ) const;
  virtual void remove( const OdDgElementId& elementId );

  virtual OdRxObjectPtr clone() const;
};


#include "TD_PackPop.h"

#endif // __DG_DICTIONARYTABLERECORD_H__
