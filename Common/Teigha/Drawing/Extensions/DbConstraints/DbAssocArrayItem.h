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

#ifndef DbAssocArrayItem_INCLUDED_
#define DbAssocArrayItem_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h" 
#include "RxObjectImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents the item locator for an array of associated parameters.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbItemLocator
{
public:
  /** \details
    Declares the index type.
  */
  enum IndexType 
  { 
      kItemIndex
    , kRowIndex
    , kLevelIndex
  };
  
  /** \details
    Constructors create an instance of this class.
  */
  OdDbItemLocator();
  
  OdDbItemLocator(int itemIndex, int rowIndex, int levelIndex);
  
  OdDbItemLocator(const OdDbItemLocator& other);

  /** \details
    Returns an item for a given index.
  */
  int operator [](IndexType i) const;
  
  /** \details
    Assigns a given locator for this locator.
  */
  OdDbItemLocator& operator =(const OdDbItemLocator& other);
  
  /** \details
    Substracts a given locator from this locator.
  */
  OdDbItemLocator  operator -(const OdDbItemLocator& other) const;
  
  /** \details
    Adds a given locator to this locator.
  */
  OdDbItemLocator  operator +(const OdDbItemLocator& other) const;
  
  /** \details
    Checks whether this locator equals to a given locator.
  */
  bool operator ==(const OdDbItemLocator& other) const;
  
  /** \details
    Checks whether this locator unequals to a given locator.
  */
  bool operator !=(const OdDbItemLocator& other) const;
  
  /** \details
    Checks whether this locator less than a given locator.
  */
  bool operator <(const OdDbItemLocator& other) const;

  /** \details
    Checks whether this locator more than a given locator.
  */
  bool operator >(const OdDbItemLocator& other) const;

  /** \details
    Compares two locators.
  */
  int compareWith(const OdDbItemLocator& other) const;

private:
  /*!DOM*/
  int m_Indices[3];
};

/** \details
  This class represents an item of the array of associated parameters.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocArrayItem : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocArrayItem);

  /** \details
    Constructors create an instance of this class.
  */
  OdDbAssocArrayItem();
  OdDbAssocArrayItem(OdDbItemLocator& locator, const OdGeMatrix3d& transform);
  static OdSmartPtr<OdDbAssocArrayItem> createObject(OdDbItemLocator& locator, const OdGeMatrix3d& transform);
  static OdSmartPtr<OdDbAssocArrayItem> createObject(const OdDbAssocArrayItem *pItem, OdDbObjectId modifyBodyId);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocArrayItem();

  /** \details
    Gets the transformation matrix.
  */
  OdResult getTransform(OdGeMatrix3d& transform, bool bCompounded = true) const;
  
  /** \details
    Checks whether the transformation is relative.
  */
  bool hasRelativeTransform() const;

  /** \details
    Transforms the item.
  */
  OdResult transformBy(const OdGeMatrix3d& transform);

  /** \details
    Sets the default transformation matrix.
  */
  OdResult setDefaultTransform(const OdGeMatrix3d& transform);

  /** \details
    Sets the erase status.
  */
  OdResult setErased(bool bErased);

  /** \details
    Checks the erase status.
  */
  bool isErased() const;

  /** \details
    Returns the item locator.
  */
  const OdDbItemLocator& locator() const;

  /** \details
    Returns an object ID of the entity provider.
  */
  const OdDbObjectId& entityProvider() const;

  /** \details
    Sets the entity provider using a given object ID.
  */
  void setEntityProvider(OdDbObjectId provider);

  /** \details
    Copies the content of a given object to one.
  */
  virtual void copyFrom(const OdRxObject* other);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

private:
  /*!DOM*/
  OdDbAssocArrayItem(const OdDbAssocArrayItem& other); // disabled Copy constructor

protected:
  friend class OdDbImpAssocArrayItem;
  class OdDbImpAssocArrayItem* m_pImpl;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocArrayItem class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocArrayItem> OdDbAssocArrayItemPtr;

#include "TD_PackPop.h"

#endif
