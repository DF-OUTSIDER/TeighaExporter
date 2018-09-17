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

#ifndef __TF_OBJECT_H__
#define __TF_OBJECT_H__

#include "TD_PackPush.h"

#include "RxObject.h"
#include "RxIterator.h"
#include "TfVariant.h"
#include "TfObjectReactor.h"
#include "DbStubPtrArray.h"
#include "Gi/GiDrawable.h"

#define ODTF_CREATE_ATTRIBUTE(Name, value) \
  reuslt.push_back(::newOdTfAttribute(OD_T(#Name), pObject, value));

#define ODTF_DEFINE_ATTRIBUTES_CONSTRUCTOR(Name, ATTRIBUTES_LIST) \
  static OdArray<OdTfAttribute*> Name##AttributesConstructor(     \
      OdRxObject* pObject) {                                      \
    OdArray<OdTfAttribute*> result;                               \
    ATTRIBUTES_LIST(ODTF_CREATE_ATTRIBUTE)                        \
    return result;                                                \
  }

#define ODTF_CREATE_PROPERTY_NO_ATTRS(PropertyName, type, gettterFn, setterFn, \
                                      defaultItemFn)                           \
  result.push_back(::newOdTfProperty(                                          \
      OD_T(#PropertyName), pClass, static_cast<OdTfVariant::Type>(type), NULL, \
      defaultItemFn, gettterFn, setterFn));

#define ODTF_DEFINE_PROPERTIES_CONSTRUCTOR(ClassName, PROPERTIES_LIST) \
  static OdArray<OdTfProperty*> ClassName##propertiesConstructor(      \
      OdTfClass* pClass) {                                             \
    OdArray<OdTfProperty*> result;                                     \
    PROPERTIES_LIST(ODTF_CREATE_PROPERTY_NO_ATTRS);                    \
    return result;                                                     \
  }

#define ODTF_DECLARE_MEMBERS(ClassName)\
    ODRX_DECLARE_MEMBERS_GENERIC(OdTfClass, ClassName)

#define ODTF_DEFINE_RTTI_MEMBERS(ClassName, ParentClass) \
  ODRX_DEFINE_RTTI_MEMBERS_GENERIC(OdTfClass, ClassName, ParentClass)

#define ODTF_DEFINE_INIT_MEMBERS(ClassName, ParentClass, attrsConsFn,      \
                                 propsConsFn, pseudoConsFn, szClassName)   \
  ODRX_DEFINE_INIT_MEMBERS_GENERIC(                                        \
      ClassName, (::newOdTfClass(szClassName, ParentClass::desc(),         \
                                 attrsConsFn, propsConsFn, pseudoConsFn)), \
      (::newOdTfClass(szClassName, ParentClass::desc(), attrsConsFn,       \
                      propsConsFn, pseudoConsFn)))

#define ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, attrsConsFn, propsConsFn, \
                             pseudoConsFn, szClassName)                        \
                                                                               \
  ODTF_DEFINE_RTTI_MEMBERS(ClassName, ParentClass)                             \
  ODTF_DEFINE_INIT_MEMBERS(ClassName, ParentClass, attrsConsFn, propsConsFn,   \
                           pseudoConsFn, szClassName)

#define ODTF_NO_CONS_NO_ATTRS_NO_PROPS_DEFINE_MEMBERS(ClassName, ParentClass) \
                                                                              \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL, NULL, NULL,              \
                       OD_T(#ClassName))                                      \
                                                                              \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, EMPTY_CONSTR)

#define ODTF_NO_CONS_NO_ATTRS_PROPS_DEFINE_MEMBERS(ClassName, ParentClass, \
                                                   PROPERTIES_LIST)        \
  ODTF_DEFINE_PROPERTIES_CONSTRUCTOR(ClassName, PROPERTIES_LIST)           \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL,                       \
                       ClassName##propertiesConstructor, NULL,             \
                       OD_T(#ClassName))                                   \
                                                                           \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, EMPTY_CONSTR)

#define ODTF_CONS_NO_ATTRS_PROPS_DEFINE_MEMBERS(ClassName, ParentClass,    \
                                                PROPERTIES_LIST, DOCREATE) \
  ODTF_DEFINE_PROPERTIES_CONSTRUCTOR(ClassName, PROPERTIES_LIST)           \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL,                       \
                       ClassName##propertiesConstructor,                   \
                       ClassName::pseudoConstructor, OD_T(#ClassName))     \
                                                                           \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, DOCREATE)

#define ODTF_CONS_NO_ATTRS_NO_PROPS_DEFINE_MEMBERS(ClassName, ParentClass, \
                                                   DOCREATE)               \
  ODTF_DEFINE_MEMBERS2(ClassName, ParentClass, NULL, NULL,                 \
                       ClassName::pseudoConstructor, OD_T(#ClassName))     \
                                                                           \
  ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName, DOCREATE)

class OdTfClass;
class OdTfProperty;
class OdTfAttribute;

class ODRX_ABSTRACT TFCORE_EXPORT OdTfAttributesContainer : public OdRxObject {
  ODRX_DECLARE_MEMBERS(OdTfAttributesContainer);
public:
  virtual OdRxObject* owner() const = 0;
  virtual const OdTfAttribute* find(const OdString& name) const = 0;
};
typedef OdSmartPtr<OdTfAttributesContainer> OdTfAttributesContainerPtr;


class ODRX_ABSTRACT TFCORE_EXPORT OdTfPropertiesContainer : public OdRxObject {
  ODRX_DECLARE_MEMBERS(OdTfPropertiesContainer);
public:
  typedef OdInt32 size_type;

  virtual OdTfClass* owner() const = 0;
  virtual size_type size() const = 0;
  virtual bool contains(const OdTfProperty* pProp) const = 0;
  virtual const OdTfProperty* find(size_type index) const = 0;
  virtual const OdTfProperty* find(const OdString& name) const = 0;
  virtual const OdTfProperty* find(const OdString& name,
                                   const OdTfVariant::Type type) const = 0;
  virtual size_type index(const OdTfProperty* pProp) const = 0;
  virtual bool unique(const OdString& name) const = 0;
  virtual bool unique(const OdString& name,
                      const OdTfVariant::Type type) const = 0;
  virtual OdRxIteratorPtr iterator() const = 0;
};
typedef OdSmartPtr<OdTfPropertiesContainer> OdTfPropertiesContainerPtr;

/** \details
    Corresponding C++ library: Od_Tf
    <group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfClass : public OdRxClass {
  ODRX_DECLARE_MEMBERS(OdTfClass);
public:
  virtual const OdTfAttributesContainerPtr attributes() const = 0;
  virtual const OdTfPropertiesContainerPtr properties() const = 0;
  virtual const OdTfPropertiesContainerPtr allProperties() const = 0;
  virtual OdTfClass* tfParent() const = 0;
};
typedef OdSmartPtr<OdTfClass> OdTfClassPtr;

/** \details
    Corresponding C++ library: Od_Tf
    <group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfProperty : public OdRxObject {
  ODRX_DECLARE_MEMBERS(OdTfProperty);
public:
  virtual OdTfClass* owner() const = 0;
  virtual OdTfVariant::Type type() const = 0;
  virtual OdString name() const = 0;
  virtual OdTfAttributesContainerPtr attributes() const = 0;
  /*
    Returns default value, that should be used for this property instead of 
    OdTfVariant::kEmpty.
    For single property it returns value, that should be used in place of whole
    property.
    For array property it returns value, to replace empty items.
  */
  virtual OdTfVariant defaultItemValue() const = 0;
  virtual OdResult getValue(const OdTfObject* pObject,
                                     OdTfVariant& value) const = 0;
  virtual OdResult setValue(OdTfObject* pObject,
                                     const OdTfVariant& value) const = 0;
};
typedef OdSmartPtr<OdTfProperty> OdTfPropertyPtr;

/** \details
    Corresponding C++ library: Od_Tf
    <group OdTf_Classes>
*/
class ODRX_ABSTRACT TFCORE_EXPORT OdTfAttribute : public OdRxObject {
  ODRX_DECLARE_MEMBERS(OdTfAttribute);
public:
  virtual OdRxObject* owner() const = 0;
  virtual OdString name() const = 0;
  virtual OdTfVariant value() const = 0;
};
typedef OdSmartPtr<OdTfAttribute> OdTfAttributePtr;

/** \details
    Corresponding C++ library: Od_Tf
    <group OdTf_Classes>
*/
class TFCORE_EXPORT OdTfObject : public OdGiDrawable {
  ODTF_DECLARE_MEMBERS(OdTfObject);

public:
  typedef OdTfPropertiesContainer::size_type PropertyIndexType;
  typedef OdTfVariantArray::size_type ItemIndexType;

  /** \details
    Gets the object's owner
  */
  virtual const OdTfObject* getOwner() const = 0;

  virtual OdResult getProperty(
      const OdTfProperty* pPI, OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) const = 0;

  virtual OdResult setProperty(
      const OdTfProperty* pPI, const OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) = 0;

  /** \details
    Gets the specified property value.

    \param name [in]  Property name.
    \param value [out]  Property value.
    \param arrayIndex [in]  if a property is of an array type, get the value at
    the given index.
  */
  virtual OdResult getProperty(
      const OdString& name, OdTfVariant& value,
      const ItemIndexType arrayIndex = -1,
      const OdTfClass* pClass = NULL) const = 0;
  /** \details
    Sets the specified property value.

    \param name [in]  Property name.
    \param value [in]  New property value.
    \param arrayIndex [in]  if a property is of an array type, set the value at
    the given index
  */
  virtual OdResult setProperty(const OdString& name,
                                        const OdTfVariant& value,
                                        const ItemIndexType arrayIndex = -1,
                                        const OdTfClass* pClass = NULL) = 0;
  /** \details
    Gets the specified property value.

    \param propertyIndex [in]  Index of a property in a class field array.
    \param value [out]  Property value.
    \param arrayIndex [in]  if a property is of an array type, get the value at
    the given index.
  */
  virtual OdResult getProperty(
      const PropertyIndexType propertyIndex, OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) const = 0;
  /** \details
    Sets the specified property value.

    \param propertyIndex [in]  Index of a property in a class field array.
    \param value [in]  New property value.
    \param arrayIndex [in]  if a property is of an array type, set the value at
    the given index
  */
  virtual OdResult setProperty(
      const PropertyIndexType propertyIndex, const OdTfVariant& value,
      const ItemIndexType arrayIndex = -1) = 0;

  /** \details
    Adds the specified transient reactor to this element's reactor list.

    \param pReactor [in]  Pointer to the transient reactor object.

    \remarks
    An element must be open either OdBm::kForRead or OdBm::kForWrite in order to
    add a transient reactor.
  */
  virtual void addReactor(OdTfObjectReactor* pReactor) = 0;

  /** \details
    Removes the specified transient reactor from this element's reactor list.

    \param pReactor [in]  Pointer to the transient reactor object.

    \remarks
    An element must be open either OdBm::kForRead or OdBm::kForWrite in order to
    remove a transient reactor.
  */
  virtual void removeReactor(OdTfObjectReactor* pReactor) = 0;

  /** \details
    Adds the specified persistent reactor to this element's reactor list.

    \param elmId [in]  Element ID of the persistent reactor.

    \remarks
    An element must be open OdBm::kForWrite in order to add a persistent
    reactor.

    If the persistent reactor does not have an owner,
    it isn't saved with the drawing.
  */
  virtual void addPersistentReactor(OdTfPersistentReactor*) = 0;

  /** \details
    Removes the specified persistent reactor from this element's reactor list.

    \param elmId [in]  Element ID of the persistent reactor.

    \remarks
    An element must be open OdBm::kForWrite in order to remove one of its
    persistent reactors.
  */
  virtual void removePersistentReactor(OdTfPersistentReactor*) = 0;

  /** \details
    This method returns true if elmId is the Element ID of a reactor attached to
    this element. Otherwise, it returns false.

    \param elmId [in]  Element ID of the persistent reactor.
  */
  virtual bool hasPersistentReactor(OdTfPersistentReactor*) const = 0;

  /** \details
    Returns this element's persistent reactors.

    \param elmIds [out]  Receives Element IDs of this element's persistent
    reactors.
  */
  virtual void getPersistentReactors(OdTfPersistentReactorArray& elmIds) = 0;

  /** \details
    Returns this element's transient reactors.

    \param reactors [out]  Receives this element's transient reactors.
  */
  virtual void getTransientReactors(OdTfObjectReactorArray& reactors) = 0;
};
typedef OdSmartPtr<OdTfObject> OdTfObjectPtr;

//
//
//

typedef OdArray<OdTfProperty*>(*OdTfPropertiesConstructorType)(OdTfClass*);
typedef OdArray<OdTfAttribute*>(*OdTfAttributesConstructorType)(OdRxObject*);
typedef OdResult (*OdTfPropertyGetterType)(const OdTfObject*,
                                                    OdTfVariant&);
typedef OdResult (*OdTfPropertySetterType)(OdTfObject*,
                                                    const OdTfVariant&);
typedef OdTfVariant (*OdTfVariantConstructorType)();

TFCORE_EXPORT OdTfClass* newOdTfClass(
    const OdString& name, OdRxClass* pBaseClass,
    OdTfAttributesConstructorType pAttrsConstr,
    OdTfPropertiesConstructorType pPropsConstr, OdPseudoConstructorType pConstr,
    bool bRegister = true);

TFCORE_EXPORT OdTfClass* createOdTfClass();

TFCORE_EXPORT OdTfClass* initOdTfClass(
    OdTfClass* pClass, const OdString& name, OdRxClass* pBaseClass,
    const OdArray<OdTfAttribute*>& attributes,
    const OdArray<OdTfProperty*>& properties, OdPseudoConstructorType pConstr,
    bool bRegister = true);

TFCORE_EXPORT OdTfProperty* newOdTfProperty(
    const OdString& name, OdTfClass* pOwner, OdTfVariant::Type type,
    OdTfAttributesConstructorType pAttrsConstr,
    OdTfVariantConstructorType pDefaultValueConstructor,
    OdTfPropertyGetterType pGetter, OdTfPropertySetterType pSetter);

TFCORE_EXPORT OdTfProperty* createOdTfProperty();

TFCORE_EXPORT OdTfProperty* initOdTfProperty(
    OdTfProperty* pProp, const OdString& name, OdTfClass* pOwner,
    OdTfVariant::Type type, const OdArray<OdTfAttribute*>& attributes,
    OdTfVariantConstructorType pDefaultValueConstructor,
    OdTfPropertyGetterType pGetter, OdTfPropertySetterType pSetter);

TFCORE_EXPORT OdTfAttribute* newOdTfAttribute(const OdString& name,
                                              OdRxObject* pOwner,
                                              const OdTfVariant& value);

#endif  // __TF_OBJECT_H__
