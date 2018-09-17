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



#ifndef _ODDBHYPERLINK_INCLUDED_
#define _ODDBHYPERLINK_INCLUDED_

#include "TD_PackPush.h"
#include "RxObject.h"
#include "DbStubPtrArray.h"

/** \details
    This class is the base class for Hyperlink objects.
    
    Corresponding C++ library: TD_Db
    
    \remarks
    OdDbHyperlink objects contain the following data:
    
    <table>
    Name               Description 
    Hyperlink Name     A filename or URL.
    Sublocation        A named view, range of cells, etc.
    Description        A display name for the Hyperlink.
    </table>
    
    
    \sa
    * OdDbEntityHyperlinkPE
    * OdDbHyperlink
    * OdDbHyperlinkCollection

    <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbHyperlink
{
public:
  /** \details
  Flags that can be set on the hyperlink object
  */
  enum HLinkFlags 
  {
    kUndefined = 0,
    kConvertDwgToDwf = 0x1
  };

  OdDbHyperlink() {};
  virtual ~OdDbHyperlink(){};
  
  /** \details
    Returns the name associated with this Hyperlink object.
  */
  virtual const OdString name() const = 0;   

  /** \details
    Sets the name associated with this Hyperlink object.
    
    \param name [in]  Name for this Hyperlink object.
  */
  virtual void setName(
    const OdString& name) = 0; 
  
  /** \details
    Returns the description associated with this Hyperlink object.
  */
  virtual const OdString description() const = 0;  

  /** \details
    Sets the description associated with this Hyperlink object.
    
    \param description [in]  Description for this Hyperlink object.

    \remarks
    "<Name> - <SubLocation>" will be returned by getDisplayString() if description is null.
  */
  virtual void setDescription(
    const OdString& description) = 0;
  
  /** \details
    Returns the sublocation associated with this Hyperlink object.
  */
  virtual const OdString subLocation() const = 0;  

  /** \details
    Sets the sublocation associated with this Hyperlink object.
    
    \param subLocation [in]  Sublocation for this Hyperlink object.
  */
  virtual void setSubLocation(
    const OdString& subLocation) = 0;
  
  /** \details
    Returns the display string associated with this Hyperlink object.

    \remarks
    Returns "<Name> - <SubLocation>" if description() is null.
  */
  virtual const OdString getDisplayString() const = 0;  
  
  /** \details
    Returns true if and only if this Hyperlink object is associated with the outermost container.
  */
  virtual bool isOutermostContainer() const = 0;
  
  /** \details
    Returns the nesting level of this Hyperlink object.
  */

  virtual int getNestedLevel() const = 0;

  /** \details
  Get flags that can be set on the hyperlink object
  */
  virtual OdInt32 flags() const  = 0;

  /** \details
  Set flags that can be set on the hyperlink object
  */
  virtual void setFlags(OdInt32 lFlags) = 0;

};

/** \details
    This class is a container object for OdDbHyperlink objects.
    
    \sa
    * OdDbEntityHyperlinkPE
    * OdDbHyperlink
    * OdDbHyperlinkCollection

    Corresponding C++ library: TD_Db
    
    <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbHyperlinkCollection : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbHyperlinkCollection);
  OdDbHyperlinkCollection() {};
  virtual ~OdDbHyperlinkCollection(){};
  
  /** \details
    Creates a HyperLink object, and adds it to the start of this HyperlinkCollection object.

    \param name [in]  Name for the Hyperlink object.
    \param description [in]  Description for the Hyperlink object.
    \param subLocation [in]  Sublocation for the Hyperlink object.
  */
  virtual void addHead(
    const OdString& name, 
    const OdString& description, 
    const OdString& subLocation = OdString::kEmpty) = 0;

  /** \details
    Creates a HyperLink object, and adds it to the end of this HyperlinkCollection object.

    \param name [in]  Name for the Hyperlink object.
    \param description [in]  Description for the Hyperlink object.
    \param subLocation [in]  Sublocation for the Hyperlink object.
  */
  virtual void addTail(
    const OdString& name, 
    const OdString& description, 
    const OdString& subLocation = OdString::kEmpty) = 0;

  /** \details
    Creates a HyperLink object, and inserts it into this HyperlinkCollection object at the specified index.


    \param name [in]  Name for the Hyperlink object.
    \param description [in]  Description for the Hyperlink object.
    \param subLocation [in]  Sublocation for the Hyperlink object.
    \param hyperlinkIndex [in]  Hyperlink index.
    
    \remarks
    Objects at or after index are moved toward the tail.
  */
  virtual void addAt(
    const int hyperlinkIndex,
    const OdString& name, 
    const OdString& description, 
    const OdString& subLocation = OdString::kEmpty) = 0;

  /** \details
    Removes the first HyperLink object in this HyperlinkCollection object
  */
  virtual void removeHead() = 0;

  /** \details
    Removes the last HyperLink object in this HyperlinkCollection object
  */
  virtual void removeTail() = 0;

  /** \details
    Removes the HyperLink object at the specified location in this HyperlinkCollection object.

    \param hyperlinkIndex [in]  Hyperlink index.
  */
  virtual void removeAt(
    const int hyperlinkIndex) = 0;
  
  /** \details
    Returns the number of HyperLink objects in this HyperlinkCollection object.
  */
  virtual int count() const = 0;
  
  
  /** \details
    Returns the HyperLink object at the specified location in this HyperlinkCollection object.

    \param hyperlinkIndex [in]  Hyperlink index.
  */
  virtual OdDbHyperlink * item(
    const int hyperlinkIndex) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbHyperlinkCollection object pointers.
*/
typedef OdSmartPtr<OdDbHyperlinkCollection> OdDbHyperlinkCollectionPtr;


/** \details
    This class is the Hyperlink Protocol Extension class.

    \remarks
    This class provides access to the Hyperlink objects
    associated with OdDbDatabase objects.
    
    \sa
    * OdDbEntityHyperlinkPE
    * OdDbHyperlink
    * OdDbHyperlinkCollection

    Corresponding C++ library: TD_Db
    <group OdDb_Classes>
*/
class DBROOT_EXPORT OdDbEntityHyperlinkPE : public OdRxObject
{
public:
  
  OdDbEntityHyperlinkPE();
  virtual ~OdDbEntityHyperlinkPE();

  ODRX_DECLARE_MEMBERS(OdDbEntityHyperlinkPE);
  
  /** \details
    Creates a HyperlinkCollection object containing the Hyperlink objects
    associated with the specified objects.
    
    \param pObject [in]  Pointer to the object.
    \param oneOnly [in]  If and only if true, returns only the first Hyperlink.  
    \param ignoreBlockDefinition [in]  If and only if true, ignores the Hyperlink objects associated with
                              block definitions.
    \returns
    Returns the HyperlinkColection object.                          

    \remarks
    As implemented, ignoreBlockDefinition is ignored.
    It will be fully implemented in a future release.
  */
  virtual OdDbHyperlinkCollectionPtr getHyperlinkCollection(
    const OdRxObject* pObject, 
    bool oneOnly = false, 
    bool ignoreBlockDefinition = true) = 0;

  /** \details
    Creates a HyperlinkCollection object containing the Hyperlink objects
    associated with the specified objects.
    
    \param objectIds [in]  Array of Object IDs.
    \param oneOnly [in]  If and only if true, returns only the first Hyperlink.  
    \param ignoreBlockDefinition [in]  If and only if true, ignores the Hyperlink objects associated with
                              block definitions.
    \returns
    Returns the HyperlinkColection object.                          

    \remarks
    As implemented, ignoreBlockDefinition is ignored.
    It will be fully implemented in a future release.
  */
  virtual OdDbHyperlinkCollectionPtr getHyperlinkCollection(
    const OdDbStubPtrArray*& objectIds,
    bool oneOnly = false,                                               
    bool ignoreBlockDefinition = true) = 0;

  /** \details
    Associates the specified HyperlinkCollection object with the specified object.
    
    \param pObject [in]  Pointer to the target object.
    \param pHCO [in]  Pointer to the HyperlinkCollection object.
    
    \remarks Hyperlink objects with null name, description, or sublocation will
    not be added to the object. 
  */
  virtual void setHyperlinkCollection(
    const OdRxObject * pObject, 
    OdDbHyperlinkCollection * pHCO) = 0;
  
  /** \details
    Returns the number of Hyperlink objects associated with the specified objects.

    \param pObject [in]  Pointer to the object.
    \param ignoreBlockDefinition [in]  If and only if true, ignores the Hyperlink objects associated with
                              block definitions.

    \remarks
    As implemented, ignoreBlockDefinition is ignored.
    It will be fully implemented in a future release.
  */
  virtual unsigned int getHyperlinkCount(
    const OdRxObject * pObject, 
    bool ignoreBlockDefinition = true) = 0;
 
  /** \details
    Returns the number of Hyperlink objects associated with the specified objects.

    \param ignoreBlockDefinition [in]  If and only if true, ignores the Hyperlink objects associated with
                              block definitions.

    \remarks
    As implemented, ignoreBlockDefinition is ignored.
    It will be fully implemented in a future release.
  */ 
  virtual unsigned int getHyperlinkCount(
    const OdDbStubPtrArray *& idContainers, 
    bool ignoreBlockDefinition = true) = 0;
  
  /** \details
    Returns true if and only if the specified object has 
    Hyperlink objects associated with it.

    \param pObject [in]  Pointer to the object.
    \param ignoreBlockDefinition [in]  If and only if true, ignores the Hyperlink objects associated with
                              block definition.

    \remarks
    As implemented, ignoreBlockDefinition is ignored.
    It will be fully implemented in a future release.
  */
  virtual bool hasHyperlink(
    const OdRxObject * pObject, 
    bool ignoreBlockDefinition = true) = 0;
  
  
  /** \details
    Returns true if and only if one or more of the specified objects has 
    Hyperlink objects associated with them.

    \param objectIds [in]  Array of Object IDs.
    \param ignoreBlockDefinition [in]  If and only if true, ignores the Hyperlink objects associated with
                              block definition.

    \remarks
    As implemented, ignoreBlockDefinition is ignored.
    It will be fully implemented in a future release.
  */  
  virtual bool hasHyperlink(
    const OdDbStubPtrArray *& objectIds, 
    bool ignoreBlockDefinition = true) = 0;
  
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbEntityHyperlinkPE object pointers.
*/
typedef OdSmartPtr<OdDbEntityHyperlinkPE> OdDbEntityHyperlinkPEPtr;

/** \details
    This registration allows you to catch the invoking of each hyperlink destructor.
*/
typedef void (*ODDBHYPERLINK_CALLBACK)(const OdDbHyperlink*);
DBROOT_EXPORT void odrxRegisterHyperlinkDestructorCallback(ODDBHYPERLINK_CALLBACK callbackFunc);
DBROOT_EXPORT void odrxUnregisterHyperlinkDestructorCallback();
#include "TD_PackPop.h"
#endif // _ODDBHYPERLINK_INCLUDED_
