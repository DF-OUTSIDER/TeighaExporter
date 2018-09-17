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

#ifndef _ODRXOBJECT_INCLUDED_
#define _ODRXOBJECT_INCLUDED_ /* { Secret } *// 

#include "TD_PackPush.h"
#include "RootExport.h"
#include "OdHeap.h"
#include "OdArrayPreDef.h"
#include "OdString.h"
#include "BaseObjectPtr.h"

class OdRxClass;
class OdRxModule;

/** \details
    Callback function providing ability for custom class to alter its application name
    depending on file format version drawing is saved to.
    
    \remarks
    Effects only application name saved to file.
*/
typedef void (*AppNameChangeFuncPtr)(const OdRxClass* classObj, OdString& newAppName, int saveVer);

/** \details
    Suppresses the incrementing of the reference counter.
    
    \remarks
    The option is used for creating of smart pointers that refer to the classes derived from OdRxObject.
    The option can be specified as an argument for the constructor of the OdSmartPtr class or OdRxObjectPtr class.

    \sa
    <link smart_pointers_definition.html, Implementing Smart Pointers> 
*/
enum OdRxObjMod { kOdRxObjAttach };

#define ODRX_DECLARE_MEMBERS_GENERIC(ClassType, ClassName)\
public:                                                                                              \
                                                                                                     \
  /** Casts the specified pointer to an ClassName SmartPointer. **/                      \
  static OdSmartPtr<ClassName> cast(const OdRxObject* pObj)                                          \
  {                                                                                                  \
    if (pObj)                                                                                        \
      return OdSmartPtr<ClassName>(((ClassName*)pObj->queryX(ClassName::desc())), kOdRxObjAttach);   \
    return (ClassName*)0;                                                                            \
  }                                                                                                  \
                                                                                                     \
  static ClassType* g_pDesc;                                                                         \
                                                                                                     \
  /** Returns the static ClassType description object associated with this object. **/  \
  /** This function is for use only when the class type of this object is known.          **/  \
  /** If the class type of this object is unknown, use instead isA().                           **/  \
  static ClassType* desc();                                                                   \
                                                                                                     \
  /** Returns the ClassType description instance associated with this object.      **/  \
  /** This function is for use only when the class type of this object is unknown.        **/  \
  /** If the class type of this object is known, use instead desc().                            **/  \
  virtual ClassType* isA() const;                                                                    \
                                                                                                     \
  /** Returns the Protocol Extension object for this object.        **/                 \
  /** Return null if there is no Protocol Extension object is found.    **/                 \
  virtual OdRxObject* queryX(const OdRxClass* protocolClass) const;                                  \
                                                                                                     \
  /** Creates a new instance of this object type.                   **/                 \
  /** Returns a SmartPointer to the new instance.                       **/                 \
  static OdRxObjectPtr pseudoConstructor();                                                          \
                                                                                                     \
  /** Creates a new instance of this object type.                   **/                 \
  /** Returns a SmartPointer to the new instance.                       **/                 \
   static OdSmartPtr<ClassName> createObject()                                                       \
  { if (!desc()) throw OdError(eNotInitializedYet); return desc()->create(); }                       \
                                                                                                     \
  /** Registers ClassName with Teigha.                           **/                 \
  static void rxInit();                                                                              \
  static void rxInit(AppNameChangeFuncPtr appNameChangeFunc);                                        \
                                                                                                     \
/* Unregisters ClassName with Teigha.                          **/                 \
  static void rxUninit()


/** \details
    Declares the RunTime Type Identification methods required by OdRxObject objects.
    
    \remarks
    Any class derived from OdRxObject should include this macro in its class declarations,
    passing its own name.

    \sa
    <link rtti_class_implementing.html, Implementing RTTI Methods for the Derived Class> 
*/
#define ODRX_DECLARE_MEMBERS(ClassName)\
    ODRX_DECLARE_MEMBERS_GENERIC(OdRxClass, ClassName)
  
/** \details
    Defines the empty constructor and casts a null pointer for the specified class.

    \sa
    <link rtti_pseudoconstructor.html, Understanding Pseudo-Constructors> 
*/
#define EMPTY_CONSTR(ClassName)(ClassName*)0

/** \details
    Defines the pseudo-constructor for the specified class that creates a new instance, creates a smart pointer to it,
    does not increment its reference counter, and returns a smart pointer to the instance.
    
    \remarks
    The reference counter of a new instance is set to a default value.

    \sa
    <link rtti_pseudoconstructor.html, Understanding Pseudo-Constructors> 
*/
#define NEWOBJ_CONSTR(ClassName) OdSmartPtr<ClassName>(new ClassName, kOdRxObjAttach)

#define ODRX_DEFINE_RTTI_MEMBERS_GENERIC(ClassType, ClassName, ParentClass) \
                                                                            \
  ClassType* ClassName::g_pDesc = 0;                                        \
  ClassType* ClassName::desc() { return g_pDesc; }                          \
  ClassType* ClassName::isA() const { return g_pDesc; }                     \
                                                                            \
  OdRxObject* ClassName::queryX(const OdRxClass* pClass) const {            \
    return ::odQueryXImpl<ClassName, ParentClass>(this, pClass);            \
  }

/** \details
    Defines Runtime Type Identification functions for OdRxObject objects.

    \sa
    <link rtti_class_implementing.html, Implementing RTTI Methods for the Derived Class> 
*/
#define ODRX_DEFINE_RTTI_MEMBERS(ClassName, ParentClass) \
  ODRX_DEFINE_RTTI_MEMBERS_GENERIC(OdRxClass, ClassName, ParentClass)

#define ODRX_DEFINE_INIT_MEMBERS_GENERIC(ClassName, CREATE_CLASS_INSTANCE, CREATE_CLASS_INSTANCE2)   \
                                                                                                     \
/* Registers this class with Teigha.                                         */                      \
void ClassName::rxInit()                                                                             \
{                                                                                                    \
  if (!ClassName::g_pDesc) {                                                                         \
    ClassName::g_pDesc = CREATE_CLASS_INSTANCE;                                                      \
  } else {                                                                                           \
    ODA_ASSERT(("Class ["#ClassName"] is already initialized.",0));                                  \
    throw OdError(eExtendedError);                                                                   \
  }                                                                                                  \
}                                                                                                    \
void ClassName::rxInit(AppNameChangeFuncPtr pAppNameChangeCallback)                                  \
{                                                                                                    \
  if (!ClassName::g_pDesc) {                                                                         \
    ClassName::g_pDesc = CREATE_CLASS_INSTANCE2;                                                     \
  } else {                                                                                           \
    ODA_ASSERT(("Class ["#ClassName"] is already initialized.",0));                                  \
    throw OdError(eExtendedError);                                                                   \
  }                                                                                                  \
}                                                                                                    \
                                                                                                     \
/* Unregisters this class with Teigha.                                       */                      \
void ClassName::rxUninit()                                                                           \
{                                                                                                    \
  if (ClassName::g_pDesc) {                                                                          \
    ::deleteOdRxClass(ClassName::g_pDesc);                                                           \
    ClassName::g_pDesc = 0;                                                                          \
  } else {                                                                                           \
    ODA_ASSERT(("Class ["#ClassName"] is not initialized yet.",0));                                  \
    throw OdError(eNotInitializedYet);                                                               \
  }                                                                                                  \
}

/** \details
   Defines the registering and unregistering functions.

   \sa
   <link rtti_class_implementing.html, Implementing RTTI Methods for the Derived Class> 

   <link rtti_class_registering.html, Registering and Unregistering Classes in a Program> 
*/
#define ODRX_DEFINE_INIT_MEMBERS(ClassName, ParentClass, pseudoConsFn, DwgVer,                \
                                 MaintVer, nProxyFlags, szDWGClassName,                       \
                                 szDxfName, szAppName, nCustomFlags)                          \
  ODRX_DEFINE_INIT_MEMBERS_GENERIC(                                                           \
      ClassName,                                                                              \
      (::newOdRxClass(szDWGClassName, ParentClass::desc(), pseudoConsFn, DwgVer,              \
        MaintVer, nProxyFlags, szDxfName, szAppName, NULL, nCustomFlags)),                    \
      (::newOdRxClass(szDWGClassName, ParentClass::desc(), pseudoConsFn, DwgVer,              \
        MaintVer, nProxyFlags, szDxfName, szAppName, pAppNameChangeCallback, nCustomFlags)))


/** \details
    Defines a pseudo-constructor for the specified class name.

    \sa
    <link rtti_pseudoconstructor.html, Understanding Pseudo-Constructors> 
*/
#define ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName,DOCREATE)                                            \
                                                                                                     \
OdRxObjectPtr ClassName::pseudoConstructor() { return OdRxObjectPtr(DOCREATE(ClassName)); }

/** \details
    Defines Runtime Type Identification and initialization functions for OdRxObject objects.

    \sa
    <link rtti_class_implementing.html, Implementing RTTI Methods for the Derived Class> 
*/
#define ODRX_DEFINE_MEMBERS2(ClassName,ParentClass,pseudoConsFn,DwgVer,MaintVer,nProxyFlags,szDWGClassName,szDxfName,szAppName,nCustomFlags) \
                                                                                                     \
ODRX_DEFINE_RTTI_MEMBERS(ClassName,ParentClass)                                                      \
                                                                                                     \
ODRX_DEFINE_INIT_MEMBERS(ClassName,ParentClass,pseudoConsFn,                                         \
    DwgVer,MaintVer,nProxyFlags,szDWGClassName,szDxfName,szAppName,nCustomFlags)
    
#define ODRX_DEFINE_MEMBERS(ClassName,ParentClass,DOCREATE,DwgVer,MaintVer,nProxyFlags,szDWGClassName,szDxfName,szAppName)\
                                                                                                     \
ODRX_DEFINE_MEMBERS2(ClassName,ParentClass,ClassName::pseudoConstructor,                             \
  DwgVer,MaintVer,nProxyFlags,szDWGClassName,szDxfName,szAppName,0)                                  \
                                                                                                     \
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName,DOCREATE)

#define ODRX_DEFINE_MEMBERS_EX(ClassName,ParentClass,DOCREATE,DwgVer,MaintVer,nProxyFlags,szDWGClassName,szDxfName,szAppName,nCustomFlags)\
                                                                                                     \
ODRX_DEFINE_MEMBERS2(ClassName,ParentClass,ClassName::pseudoConstructor,                             \
  DwgVer,MaintVer,nProxyFlags,szDWGClassName,szDxfName,szAppName,nCustomFlags)                       \
                                                                                                     \
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName,DOCREATE)


#define ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(ClassName,ParentClass,szClassName)                       \
                                                                                                     \
ODRX_DEFINE_MEMBERS2(ClassName,ParentClass,0,0,0,0,szClassName,OdString::kEmpty,OdString::kEmpty,0)                          \
                                                                                                     \
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName,EMPTY_CONSTR)



#define ODRX_NO_CONS_DEFINE_MEMBERS(ClassName,ParentClass)                                           \
                                                                                                     \
ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(ClassName,ParentClass,OD_T(#ClassName))



#define ODRX_CONS_DEFINE_MEMBERS_ALTNAME(ClassName,ParentClass,szClassName,DOCREATE)                 \
                                                                                                     \
ODRX_DEFINE_MEMBERS2(ClassName,ParentClass,ClassName::pseudoConstructor,0,0,0,szClassName,OdString::kEmpty,OdString::kEmpty,0) \
                                                                                                     \
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName,DOCREATE)



#define ODRX_CONS_DEFINE_MEMBERS(ClassName,ParentClass,DOCREATE)                                     \
                                                                                                     \
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(ClassName,ParentClass,OD_T(#ClassName),DOCREATE)



#define ODRX_DXF_DEFINE_MEMBERS(ClassName,ParentClass,DOCREATE,DwgVer,MaintVer, nProxyFlags,DxfName,AppName)    \
                                                                                                                \
ODRX_DEFINE_MEMBERS(ClassName,ParentClass,DOCREATE,                                                             \
    DwgVer,MaintVer,nProxyFlags,OD_T(#ClassName),OD_T(#DxfName),OD_T(#AppName))



#define ODRX_DXF_CONS_DEFINE_MEMBERS(ClassName,ParentClass,DwgVer,MaintVer,nProxyFlags,DxfName,AppName)    \
                                                                                                           \
ODRX_DEFINE_MEMBERS2(ClassName,ParentClass,ClassName::pseudoConstructor,                                   \
    DwgVer,MaintVer,nProxyFlags,OD_T(#ClassName),OD_T(#DxfName),OD_T(#AppName),0)                          \
                                                                                                           \
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName,NEWOBJ_CONSTR)


/** \details

    <group TD_Namespaces>
*/
namespace OdRx
{
  enum Ordering
  {
    kLessThan     =-1, // This object < Specified Object
    kEqual        = 0, // This object == Specified Object
    kGreaterThan  = 1, // This object > Specified Object
    kNotOrderable = 2  // These objects cannot be ordered.
  };

  enum MTSupport
  {
    kMTRender         = 0x01, // worldDraw() or viewportDraw() calls from different threads are allowed
                              // for highest level (MS or PS), elsewhere they should always happen
                              // in one dedicated thread.
    kMTRenderInBlock  = 0x02, // Calls to worldDraw() or viewportDraw() methods from different threads are allowed
                              // if the drawable is located inside a block (compound object).
    kMTRenderNested   = 0x04, // worldDraw() or viewportDraw() from different threads are allowed for
                              // nested drawables of this compound object.
    kMTRenderReserved = 0x08, // Reserved for future use.
    kMTRenderMask     = 0x0f, // Combination of all MTRender-dependent flags.
    kMTLoading        = 0x80, // Multithread loading is supported for this object.
    kHistoryAware     = 0x100 // The information written by the object to the undo filer can be saved in the history file (is valid between sessions)
  };
}

/** \details
    <group OdRx_Classes> 

    This class provides the non-typified implementation of smart pointers for instances of classes derived from the OdRxObject class.

    \sa
    TD_Db
    
    \remarks
    SmartPointers relieve the programmer of having to determine when objects are no longer needed, or having to delete
    them at that time.

    Each object referenced by a SmartPointer (henceforth 'referenced object') 
    maintains a reference count; i.e., how many SmartPointers are referencing it.     
    When the reference counter reaches zero, the referenced object is deleted.

     \sa
    <link smart_pointers_example_refexchange.html, Example of Assigning Smart Pointers>

    <link smart_pointers_example_nontypified.html, Example of a Non-Typified Smart Pointer>

    \sa
    <link smart_pointers_overview.html, Overview of Smart Pointer Classes>

    The OdSmartPtr class
*/
class OdRxObjectPtr : public OdBaseObjectPtr
{
  /** \details
    Increments the reference counter of the referenced object.
  */
  inline void internalAddRef();

  /** \details
    Assigns the pointer of the specifed object to the smart pointer.  
      
    \remarks
    The reference counter of the specified object is incremented.

    If this SmartPointer is currently referencing another object, that object 
    is released prior to the assignment.  

    \param pObject [in]  Pointer to the specified object to be assigned.
  */
  void assign(
    const OdRxObject* pObject)
  {
    if (m_pObject != pObject)  // To prevent reference counter dec/inc which
    {                          // may cause object destruction or subClose
      release();
      m_pObject = (OdRxObject*)pObject;
      internalAddRef();
    }
  }

public:

  /** \details
    Constructors for the non-typified smart pointer.

    \param pSource [in]  Pointer to the specified object to be assigned for the new SmartPointer object.

    \remarks
    If pSourceis specified, the smart pointer is set to the address of the specified object. If pSource is not specified, the smart pointer is set to a NULL.
    
    \remarks
    If OdRxObjMod is specified, the reference counter of the specified object 
    is *not* incremented. 
  */
  OdRxObjectPtr() : OdBaseObjectPtr() { }
  OdRxObjectPtr(
    const OdRxObject* pSource, 
    OdRxObjMod)
    : OdBaseObjectPtr(pSource)
  {
  }

  OdRxObjectPtr(
    const OdRxObject* pSource)
    : OdBaseObjectPtr(pSource)
  {
    internalAddRef();
  }

  OdRxObjectPtr(
    const OdRxObjectPtr& pSource)
    : OdBaseObjectPtr(pSource.get())
  {
    internalAddRef();
  }

  OdRxObjectPtr(
    const OdBaseObjectPtr& pSource)
    : OdBaseObjectPtr(pSource.get())
  {
    internalAddRef();
  }

  /** \details
    Assigns the pointer of the specifed object to the smart pointer without incrementing its reference counter.  
      
    \param pObject [in]  Pointer to the specified object to be assigned.

    \remarks
    The reference count of the specified object is not incremented.

    If this SmartPointer is currently referencing another object, that object 
    is released prior to the assignment.

    \sa
    <link smart_pointers_example_nontypified.html, Example of a Non-Typified Smart Pointer> 
  */
  void attach(
    const OdRxObject* pObject) { release(); m_pObject = (OdRxObject*)pObject; }

  /** \details
    Destructor for the non-typified smart pointer.

    \remarks
    Decrements the reference counter of the referenced object. When the reference count reaches zero, the referenced object is deleted.
  */
  ~OdRxObjectPtr()
  { 
    release(); 
  }

  /** \details
    Releases the smart pointer reference to the referenced object. 
    
    \remarks
    Decrements the reference counter of the referenced object.  When the reference count reaches zero, 
    the referenced object is deleted. Smart pointer is set to a NULL.

    \sa
    <link smart_pointers_example_nontypified.html, Example of a Non-Typified Smart Pointer> 
  */
  inline void release();

  /** \details
    Releases the smart pointer reference to the referenced object without decrementing of the reference counter. 
      
    \returns
    Returns the raw pointer to the referenced object. The reference counter is not modified 
    for the referenced object. Smart pointer is set to a NULL.

    \sa
    <link smart_pointers_example_nontypified.html, Example of a Non-Typified Smart Pointer>
  */
  OdRxObject* detach()
  {
    OdRxObject* res = m_pObject;
    m_pObject = 0;
    return res;
  }

  /** \remarks
    The assignment operator increments the reference counter of the referenced object and saves its address in the smart pointer.
     If the smart pointer is currently referencing to another object, that assignment operator releases the referenced object prior 
     to the assignment.  

    \sa
   <link smart_pointers_example_nontypified.html, Example of a Non-Typified Smart Pointer>
  */
  OdRxObjectPtr& operator = (
    const OdRxObjectPtr& source)
  { assign(source); return *this; }

  OdRxObjectPtr& operator = (
    const OdRxObject* source)
  { assign(source); return *this; }

  OdRxObjectPtr& operator = (
    const OdBaseObjectPtr& source)
  { assign(source.get()); return *this; }

  /** \details
    Returns the raw pointer to the referenced object.

    \remarks
    The smart pointer maintains its reference to the referenced object.
    The reference counter of the referenced object is not changed.
 
    \sa
    <link smart_pointers_example_nontypified.html, Example of a Non-Typified Smart Pointer> 
  */
  OdRxObject* get() { return m_pObject; }
  const OdRxObject* get() const { return m_pObject; }

  /** \details
    Returns the raw pointer to the referenced object.
      
    \remarks
    The reference counter of the referenced object is not changed.

    \sa
    <link smart_pointers_example_nontypified.html, Example of a Non-Typified Smart Pointer> 
  */
  OdRxObject* operator ->() { return m_pObject; }
  const OdRxObject* operator ->() const { return m_pObject; }

#ifdef ODA_GCC_2_95
  /** \details
    Returns the raw pointer to the referenced object.
    
    \remarks
    The class maintains the reference to the referenced object.
    The reference counter of the referenced object is not changed.

    This non-standard form is used to eliminate a large number of compiler
    warnings produced by GCC 2.95.X (GCC 3.X no longer produces these warnings).
  */
  operator OdRxObject*() const { return m_pObject; }
#else
  /** \details
    Returns the raw pointer of the referenced object.
    
    \remarks
    The reference counter of the referenced object is not changed.
  */
  operator OdRxObject*() { return m_pObject; }

  operator const OdRxObject*() const { return m_pObject; }
#endif

  bool operator==(
    const OdRxObject* pObject) const { return (m_pObject==pObject); }

  bool operator!=(
    const OdRxObject* pObject) const { return (m_pObject!=pObject); }
};


/** \details
   <group OdRx_Classes> 

   This class is the base class for all OdRxObject objects that 
   require Runtime Type Identification.

   \sa
   <link smart_pointers_overview.html, Overview of Smart Pointer Classes>

   <link rtti.html, RTTI Technology>

   <link rx_object_compare.html, Comparing Rx-Objects>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxObject
{

  /** \details
      Constructor for this class that assigns another Rx-object.
  */
  OdRxObject(const OdRxObject&);

  /** \details
      Assignment operator prohibited.
  */
  OdRxObject& operator = (const OdRxObject&);

protected:
  /** \details
      Protects derived OdRxObject objects from direct use of new and delete operators. 
  */
  ODRX_HEAP_OPERATORS();

public:
  /** \details
      Default constructor for this class.
  */
  OdRxObject() { }

  /** \details
      The virtual destructor for this class.
  */
  virtual ~OdRxObject();

  /** \details
    Checks whether the object belongs to the specified class and returns a pointer to 
    this object if this object can be converted to the specified class.

    \param pClass [in]  Raw pointer to the class describing instance to which the object must be converted.
      
    \remarks
    If the object cannot be converted, the method returns a Null.
    The program must release this object, if the returned pointer is not Null.

    \sa
    <link rtti_sample_belonging.html, Example of Belonging to an Instance of a Class>

    \sa
    <link rtti_class_belonging.html, Belonging to an Instance of a Class>
  */
  virtual OdRxObject* queryX(
    const OdRxClass* pClass) const;

  /** \details
   Checks whether the object belongs to the specified class and returns a pointer to 
   this object if this object can be converted to the specified class, or throws an exception if it cannot.

    \param pClass [in]  Raw pointer to the class describing instance to which the object must be converted.

    \remarks
    This method generates the eNotThatKindOfClass exception when the object can be not converted to the specified class.
    This method generates the eInvalidInput exception when the argument pClass is Null. This method uses the queryX() method.

    \sa
    <link rtti_sample_belonging.html, Example of Belonging to an Instance of a Class>

    \sa
    <link rtti_class_belonging.html, Belonging to an Instance of a Class>
  */
  virtual OdRxObject* x(
    const OdRxClass* pClass) const;

  /** \details
    Casts the specified raw pointer to the smart pointer.

    \param pointer [in]  Raw pointer to be cast.
  */
  static OdRxObjectPtr cast(
    const OdRxObject* pointer)
  {
    OdRxObjectPtr pRes;
    if (pointer)
      pRes.attach(pointer->queryX(desc()));
    return pRes;
  }

  /** \details
    Returns the raw pointer to the class describing instance statically associated with this object
    as the OdRxClass object.

    \remarks
    This function is for use only when the class type of this object is known. 
    If the class type of this object is unknown, use isA() method instead.

    \sa
    <link rtti_sample_identifying.html, Example of Identifying Classes>

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>
  */
  static OdRxClass* desc();


  /** \details
    Returns the raw pointer to the class describing instance dynamically associated with this object
    as the OdRxClass object.
 
    \remarks
    This function is for use only when the class type of this object is unknown. 
    If the class type of this object is known, use desc() method instead.

    \sa
    <link rtti_sample_identifying.html, Example of Identifying Classes>

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>
  */
    virtual OdRxClass* isA() const;

  /** \details
    Declares the method incrementing the reference counter in objects derived from this class.

    \remarks
    It is the pure virtual method that does not have an implementation. You must redefine it in the derived class.

    \sa
    <link smart_pointers_overview.html, Overview of Smart Pointer Classes>
  */
  virtual void addRef() = 0;

  /** \details
    Declares the method decrementing the reference counter in objects derived from this class.
    
    \remarks
    It is the pure virtual method that does not have an implementation. You must redefine it in the derived class.

    \sa
    <link smart_pointers_overview.html, Overview of Smart Pointer Classes>
  */
  virtual void release() = 0;

  /** \details
    Returns the value of the reference counter, that is, the number of references to an instance of this class.

    \remarks
    This method has the default implementation that returns 1. You shloud redefine it in the derived class.

    \sa
    <link smart_pointers_overview.html, Overview of Smart Pointer Classes>
  */
  virtual long numRefs() const;

   /** \details
    Checks whether the object belongs to the specified class and
    returns true if and only if this object is an instance of the specified class or
    a subclass of the specified class.

     \param pClass [in]  Raw pointer to the class describing instance to be checked.

     \sa
     <link rtti_sample_belonging.html, Example of Belonging to an Instance of a Class>

     \sa
     <link rtti_class_belonging.html, Belonging to an Instance of a Class>
   */
  bool isKindOf(
    const OdRxClass* pClass) const
  {
    OdRxObjectPtr pRes;
    pRes.attach(queryX(pClass));
    return (!pRes.isNull());
  }

  /** \details
    Creates a *clone* of this object, and returns a pointer to the *clone*.
  */
  virtual OdRxObjectPtr clone() const;
  
  /** details
    Copies the contents of the specified object into this object.
    
    \param pSource [in] Pointer to the source object.
    
    \remarks
    The default implementation of this function does nothing but throw an eNotApplicable exception. 
    
    Throws:
    eNotApplicable Exception 
    
   
 */
  virtual void copyFrom(
    const OdRxObject* pSource);

  /** \details
    Compares another object to this one in a some meaning. 

    \param pOther [in]  Raw pointer to the object to be compared.
    
    \remarks
    This method returns one of the following:

    <table>
    Name             Value    Description  
    _kLessThan_     -1       This object < Other Object.  
    _kEqual_              0       This object = Other Object.  
    _kGreaterThan_     1       This object > Other Object.  
    _kNotOrderable_     2       This class is not orderable.  
    </table>


    \remarks
    The default implementation of this method returns _kNotOrderable_.

    \sa
    <link rx_object_compare_moreless.html, Comparing the Value of Rx Objects>

    \sa
    <link rx_object_compare.html, Comparing Rx-Objects>
  */
  virtual OdRx::Ordering comparedTo(
    const OdRxObject* pOther) const;

  /** \details
    Returns true if and only if the specified object is equal to this one in a some meaning.

    \param pOther [in]  Raw pointer to the object to be compared.

    \sa
    <link rx_object_compare_equivalence.html, Comparing the Equivalence of Rx Objects>

    \sa
    <link rx_object_compare.html, Comparing Rx-Objects>
  */
  virtual bool isEqualTo(
    const OdRxObject* pOther) const;
};



void OdRxObjectPtr::internalAddRef()
{
  if(m_pObject) { m_pObject->addRef(); }
}
void OdRxObjectPtr::release()
{
  if (m_pObject)
  {
    m_pObject->release();
    m_pObject = NULL;
  }
}

#include "SmartPtr.h"


enum OverrulingType
  {
    kDrawableOverrule = 0,
    kObjectOverrule,
    kPropertiesOverrule,
    kGeometryOverrule,
    kHighlightOverrule,
    kSubentityOverrule,
    kGripOverrule,
    kTransformOverrule,
    kOsnapOverrule,
    kVisibilityOverrule,
    kTotalOverrules
  };

class OdRxOverruleIterator;

/** \details
    Defines OdPseudoConstructorType as a function pointer type.
    
    \remarks
    Functions pointed to by OdPseudoConstructorType 
    take no arguments and construct instances of
    classes derived from OdRxObject;
    hence the name PseudoConstructorType.
*/
typedef OdRxObjectPtr (*OdPseudoConstructorType)();

/** \details
  <group OdRx_Classes> 
  Contains runtime information about a Rx-Object.  
    
  \remarks
  A pointer to an OdRxClass instance can be obtained for an OdRxObject class by calling 
  the OdRxObject::desc() function.

  \sa
  <link rtti.html, RTTI Technology>

  <link rtti_class_describing.html, Implementing the Class Describing Structure>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxClass : public OdRxObject
{
protected:
  OdRxClass();
public:
  ODRX_DECLARE_MEMBERS(OdRxClass);
 
  /** \details
    Creates the Protocol Extension for the specified OdRxClass.

    \param pProtocolClass [in]  Pointer to the Protocol Extension class.
    \param pProtocolObject [in]  Pointer to the Protocol Extension Object.

    \returns
    Returns a SmartPointer to the Protocol Extension.

    \remarks
    The default implementation of  this object does nothing but return a
    null pointer, and asserts in debug mode. It will be
    fully implemented in a future *release*.
  */
  virtual OdRxObjectPtr addX(
    OdRxClass* pProtocolClass, 
    OdRxObject* pProtocolObject);

  /** \details
    Returns the Protocol Extension for the specified
    Protocol extension class.

    \param pProtocolClass [in]  Pointer to the Protocol Extension class.

    \returns
    Returns a SmartPointer to the Protocol Extension.

    \remarks
    The default implementation of this object does nothing but
    assert in debug mode, and return a null pointer.
    It will be fully implemented in a future *release*.
  */
  virtual OdRxObjectPtr getX(
    const OdRxClass* pProtocolClass);

  /** \details
    Deletes the Protocol Extension for the specified Protocol Extension class.

    \returns
    Returns the Protocol Extension Object.

    \param pProtocolClass [in]  Pointer to the Protocol Extension class.

    \remarks
    As currently implemented, this function does nothing but
    assert in debug mode, and return a null pointer.
    It will be fully implemented in a future *release*.
  */
  virtual OdRxObjectPtr delX(
    OdRxClass* pProtocolClass);
  
  /** \details
    Creates a new instance of the class associated with the class describing instance
    using the pseudo-constructor of the class.

    \returns
    Returns a smart pointer to the newly created instance.

    \sa
    <link rtti_class_constructors.html, Functionality of Pseudo-Constructors>

    \sa
    <link rtti_pseudoconstructor.html, Understanding Pseudo-Constructors>
  */
  virtual OdRxObjectPtr create() const = 0;

  /** \details
    Returns the application class name for the class describing instance represented by this 
    OdRxClass object. Custom class can alter its application name depending on file format version 
    it's being saved to.

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual const OdString appName() const = 0;

  /** \details
    Returns a pointer to the callback function which is called when a drawing is to be saved and 
    the logical application class name represented by the class description instance to be written 
    out to file for the custom class, or NULL, if the callback function is not assigned.

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual AppNameChangeFuncPtr appNameCallbackPtr() const = 0;

  /** \details
    Returns the DXF class name for the class describing instance represented by this OdRxClass object.

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual const OdString dxfName() const = 0;

  /** \details
    Returns the class name for the class describing instance represented by this OdRxClass object.

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual const OdString name() const = 0;

  /** \details
    Returns the .dwg file version and maintenance version for the class describing instance 
    represented by this OdRxClass object.

    \param pMaintReleaseVer [in]  Raw pointer to the variable in which the maintenance version number
    must be saved. This variable must have the OdDb::MaintReleaseVer type.

    \remarks
    The .dwg file version number is returned value of the method that must have the OdDb::DwgVersion type.
    This method returns the maintenance release version if and only if its argument is not Null. 

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual OdDb::DwgVersion getClassVersion(
    OdDb::MaintReleaseVer* pMaintReleaseVer = 0) const = 0;

  /** \details
    Returns the /proxy flags/ for the class describing instance represented by this OdRxClass object.

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual OdUInt32 proxyFlags() const = 0;
  
  /** \details
   Checks whether the class defined by this class describing instance belongs to or
   derived from the specified class. The method returns true if and only if this object is
   an instance of the specified class or a subclass of the specified class.

    \param pClass [in]  Raw pointer to the class describing instance to be checked. 

    \sa
    <link rtti_sample_belonging.html, Example of Belonging to an Instance of a Class> 

    \sa
    <link rtti_class_belonging.html, Belonging to an Instance of a Class> 
  */
  virtual bool isDerivedFrom(
    const OdRxClass* pClass) const;

  /** \details
    Returns the raw pointer to the class describing instance which describes 
    the parent class for this class as the OdRxClass object.

   \sa
   <link rtti_class_functionality.html, Functionality of RTTI>

   \sa
   <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual OdRxClass* myParent() const = 0;

  /** \details
    Returns the OdRxModule object in which the class is resided.

    \remarks
    The default implementation of this function returns a Null pointer. 

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual OdRxModule* module() const = 0;

  /** \details
    Returns the raw pointer to the function that is the pseudo-constructor for this class.

    \sa
    <link rtti_class_constructors.html, Functionality of Pseudo-Constructors>

    \sa
    <link rtti_pseudoconstructor.html, Understanding Pseudo-Constructors>
  */
  virtual OdPseudoConstructorType constructor();

  /** \details
    Sets the user-defined function as the pseudo-constructor for this class.

    \param pseudoConstr [in]  Raw pointer to the user-defined function that does not have arguments
    and returns the smart pointer to a new instance of this class.

    \sa
    <link rtti_class_constructors.html, Functionality of Pseudo-Constructors>

    \sa
    <link rtti_pseudoconstructor.html, Understanding Pseudo-Constructors>
  */
  virtual void setConstructor(
    OdPseudoConstructorType psuedoConstr) = 0;

  /** \details
    Returns the custom flags for the class describing instance represented by this OdRxClass object.

    \sa
    <link rtti_class_functionality.html, Functionality of RTTI>

    \sa
    <link rtti_class_describing.html, Implementing the Class Describing Structure>
  */
  virtual OdUInt32 customFlags() const = 0;

  virtual OdRxOverruleIterator* getOverrule(OverrulingType t) const = 0;
  virtual void setOverrule(OdRxOverruleIterator* pIt, OverrulingType t) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class template for OdRxClass object pointers.

    \sa
    <link rtti_class_belonging.html, Belonging to an Instance of a Class>
*/
typedef OdSmartPtr<OdRxClass> OdRxClassPtr;

template <class Class, class Parent>
OdRxObject* odQueryXImpl(const Class* pThis, const OdRxClass* pClass)
{
  OdRxObject* pObj = 0;
  if (!pClass) // requested class is not registred
    return pObj;
  if(pClass == Class::desc())
  {
    pObj = (OdRxObject*)pThis;
    pObj->addRef();
  }
  else
  {
    pObj = Class::desc()->getX(pClass).detach();
    if(!pObj)
      pObj = pThis->Parent::queryX(pClass);
  }
  return pObj;
}

/** \details
    This external function creates a new instance of the class specified by the argument as a string
    and returns a /non-typified smart pointer/ to it.

    \param sClassName [in]  Custom class name as a string.
*/
FIRSTDLL_EXPORT OdRxObjectPtr odrxCreateObject(
    const OdString& sClassName);

/** \details
    This external function creates a new class description instance with specified parameters and 
    returns a raw pointer to it.

    \param sClassName [in]  Custom class name as a string.
    \param pBaseClass [in]  A pointer to an existing class description instance which is the parent class.
    \param pConstr [in]  A pointer to the pseudo-constructor function.
    \param DwgVer [in]  dwg-version number.
    \param MaintVer [in]  Maintain version number.
    \param nProxyFlags [in]  Mask of the proxy flags.
    \param sDxfName [in]  dxf-class name.
    \param sAppName [in]  Application class name.
    \param fNameChangeFunc [in]  A pointer to the callback function which is called when the registered object is to be saved.
    \param nCustomFlags [in]  Mask of the custom flags.

    \sa
    <link rtti_class_registering.html, Registering and Unregistering Classes in a Program>
*/
FIRSTDLL_EXPORT OdRxClass* newOdRxClass(
    const OdString& sClassName,
    OdRxClass* pBaseClass,
    OdPseudoConstructorType pConstr = 0,
    int DwgVer = 0,
    int MaintVer = 0,
    int nProxyFlags = 0,
    const OdString& sDxfName = OdString::kEmpty,
    const OdString& sAppName = OdString::kEmpty,
    AppNameChangeFuncPtr fNameChangeFunc = NULL,
    int nCustomFlags = 0
  );

/** \details
    This external function deletes a class description instance.

    \param pClass [in]  Raw pointer to an existing class description instance.
*/
FIRSTDLL_EXPORT void deleteOdRxClass(
  OdRxClass* pClass);

/** \details
    This template class is a specialization of the OdArray class template for OdRx object pointers.
*/
typedef OdArray<OdRxObjectPtr> OdRxObjectPtrArray;

/** \details
    This registration allows you to catch the invoking of each RxObject destructor.
*/
typedef void (*ODRXOBJECT_CALLBACK)(const OdRxObject*);
FIRSTDLL_EXPORT void odrxRegisterDestructorCallback(const ODRXOBJECT_CALLBACK callbackFunc);
FIRSTDLL_EXPORT void odrxUnregisterDestructorCallback();

#include "TD_PackPop.h"

#endif //_ODRXOBJECT_INCLUDED_

