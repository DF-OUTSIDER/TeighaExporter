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


#ifndef _OD_RXDYNAMICMODULE_H_
#define _OD_RXDYNAMICMODULE_H_

#include "RxModule.h"
#include "TDVersion.h"
#include "OdString.h"
#include "OdAllocOp.h"
#include "Tf/TfSchemaManagerInterface.h"

#include "TD_PackPush.h"

/** \details
  <group OdRx_Classes> 

  This template class provides the implementation of static rx-modules in multi-threaded (MT) mode.

  \sa
  OdRxModule, OdRefCounter classes
*/
template<class T, class TInterface = T>
class OdRxStaticModule : public T
{
  ODRX_HEAP_OPERATORS();

  OdRefCounter m_nLockCount;
  OdString     m_sName;

  OdRxStaticModule(const OdString& sName)
    : m_nLockCount(0)
    , m_sName(sName)
  { }
  void addRef() 
  { 
    ++m_nLockCount; 
  }
  void release() 
  { 
    ODA_ASSERT(m_nLockCount); 
    --m_nLockCount; 
  }
  long numRefs() const { return m_nLockCount; }
public:
  /** \details
    Returns the system data for the existing static rx-module instance.

    \remarks
    The default implementation does nothing and returns a null pointer.
  */
  void* sysData() { return 0; }
  
  /** \details
    Creates a new static rx-module instance with the specified module name and 
    returns a raw pointer to it.

    \param name [in]  module name as a non-empty string value.
  */
  static OdRxModule* createModule(const OdString& name)
  {
    return new OdRxStaticModule(name);
  }

  /** \details
    Deletes this static rx-module object.
  */
  void deleteModule() { delete this; }

  /** \details
    Returns the module name for the static rx-module instance.
  */
  OdString moduleName() const { return m_sName; }
};

typedef OdRxModule* (*StaticModuleEntryPoint)(const OdString& szModuleName);

#define ODRX_STATIC_MODULE_ENTRY_POINT(_UserModuleClass) odrxCreateModuleObject_For_##_UserModuleClass

/** \details
    This macro is used to create an entry point function for a custom application which 
    are linked in statically to a Teigha client application.
*/
#define ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(_UserModuleClass) \
OdRxModule* ODRX_STATIC_MODULE_ENTRY_POINT(_UserModuleClass)(const OdString& szModuleName)

/** \details
  <group !!RECORDS_TD_APIRef>
*/
      struct STATIC_MODULE_DESC
{
  const wchar_t*           szAppName;
  StaticModuleEntryPoint  entryPoint;
};

/** \details
  Defines the start of the static module map, which contains entries for all custom Teigha modules 
  that are linked in statically to a Teigha client application.
*/
#define ODRX_BEGIN_STATIC_MODULE_MAP() \
STATIC_MODULE_DESC g_ODRX_STATIC_MODULE_MAP[] = {

/** \details
  Defines the entry in the static module map, which contains entries for all custom Teigha modules 
  that are linked in statically to a Teigha client application.

  \param AppName [in]  Registered application name, that can be later passed to 
              OdRxDynamicLinker::loadModule to load the static application.
  \param ModuleClassName [in] Name of the C++ class derived from OdRxModule, that 
                      implements support for this module.
*/
#define ODRX_DEFINE_STATIC_APPLICATION(AppName, ModuleClassName) \
{ AppName, ODRX_STATIC_MODULE_ENTRY_POINT(ModuleClassName) },

/** \details
*/
#define ODRX_DEFINE_STATIC_APPMODULE(moduleName, ModuleClassName) \
ODRX_DEFINE_STATIC_APPLICATION(moduleName, ModuleClassName)

/** \details
  Defines the end of the static module map, which contains entries for all custom Teigha modules 
  that are linked in statically to a Teigha client application.
*/
#define ODRX_END_STATIC_MODULE_MAP() \
{ 0, 0 } };

FIRSTDLL_EXPORT void odrxInitStaticModuleMap(STATIC_MODULE_DESC* pMap);

#define ODRX_INIT_STATIC_MODULE_MAP() odrxInitStaticModuleMap(g_ODRX_STATIC_MODULE_MAP)

#define ODRX_DEFINE_STATIC_MODULE(_UserModuleClass)\
\
  static OdRxModule* g_pSingletonModule##_UserModuleClass = 0;\
\
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(_UserModuleClass)\
{\
  return OdRxStaticModule<_UserModuleClass >::createModule(szModuleName);\
}

/** \details
  <group OdRx_Classes> 

  This template class provides the implementation of pseudo static rx-modules in multi-threaded (MT) mode.

  \remarks
  For internal use only

  \sa
  OdRxModule, OdRefCounter classes
*/
template<class T, class TInterface = T>
class OdRxPseudoStaticModule : public T
{
  ODRX_HEAP_OPERATORS();

  OdRefCounter m_nLockCount;
  OdString     m_sName;

  OdRxPseudoStaticModule(const OdString& sName)
    : m_nLockCount(0)
    , m_sName(sName)
  { }
  void addRef() 
  { 
    ++m_nLockCount; 
  }
  void release() 
  { 
    ODA_ASSERT(m_nLockCount); 
    --m_nLockCount; 
    if (m_nLockCount == 0){
      delete this;
    }
  }
  long numRefs() const { return m_nLockCount; }
public:
  /** \details
    Returns the system data for the existing pseudo static rx-module instance.

    \remarks
    The default implementation does nothing and returns a null pointer.
  */
  void* sysData() { return 0; }

  /** \details
    Creates a new pseudo static rx-module instance with the specified module name and
    returns a raw pointer to it.

    \param name [in]  Module name as non-empty string value.
  */
  static OdRxModule* createModule(const OdString& name)
  {
    return new OdRxPseudoStaticModule(name);
  }

  /** \details
    Deletes this pseudo static rx-module object.
  */
  void deleteModule() { delete this; }

  /** \details
    Returns the module name for the pseudo static rx-module instance.
  */
  OdString moduleName() const { return m_sName; }
};

// For internal use only
#define ODRX_DEFINE_PSEUDO_STATIC_MODULE(_UserModuleClass)\
  ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(_UserModuleClass)\
{\
  return OdRxPseudoStaticModule<_UserModuleClass >::createModule(szModuleName);\
}

#if defined(_TOOLKIT_IN_DLL_) && !defined(__MWERKS__) ////////////////////////////////////////////////////////////////////////////////

//IR.111706. MacOS X dynamic library loading.
#if defined(__GNUC__) && defined(__APPLE__) // GNU compiler MacOS X

#define ODRX_STATIC_MODULE_PATH "{5CEAD1EF-4D33-48fe-99E4-E09176BCF088}/"

#define ODRX_STATIC_APP_MODULE_NAME(AppName) (OdString(ODRX_STATIC_MODULE_PATH) + AppName + DEFAULT_MODULE_EXTENSION_DOT_W)

void deleteModuleObject();

/** \details
  <group OdRx_Classes> 

  This template class provides the implementation of Mac rx-modules in multi-threaded (MT) mode.

  \sa
  OdRxModule, OdRefCounter classes
*/
template<class T, class TInterface = T>
class OdRxMacModule : public T
{
  OdRefCounter   m_nLockCount;
  void          *m_hModule;
  const OdString m_name;

  OdRxMacModule(void *hModule, const OdString name)
    : m_nLockCount(0)
    , m_hModule(hModule)
    , m_name(name)
  {}
  void addRef() 
  { 
    ++m_nLockCount; 
  }
  void release() 
  { 
    ODA_ASSERT(m_nLockCount); 
    --m_nLockCount; 
  }
  long numRefs() const { return m_nLockCount; }
  ODRX_HEAP_OPERATORS();
public:
  /** \details
    Returns the system data for the existing Mac rx-module instance.
  */
  void* sysData() { return m_hModule; }

  /** \details
    Creates a new Mac rx-module instance with the specified module name and
    returns a raw pointer to it.

    \param hModule [in]  Module handle.
    \param name [in]  module name as non-empty string value.
  */
  static OdRxMacModule* createModule(void *hModule, const OdString name) 
  { 
    return new OdRxMacModule(hModule, name); 
  }

  /** \details
    Deletes this Mac rx-module object.
  */
  void deleteModule()
  {
    deleteModuleObject();
    delete this;
  }

  /** \details
    Returns the module name for the Mac rx-module instance.
  */
  OdString moduleName() const
  {
    return m_name;
  }
};

/** \details
  Creates the entry point function for a Teigha custom application.
*/
#define ODRX_DEFINE_DYNAMIC_MODULE(_UserModuleClass)\
ODRX_ALLOC_OPERATORS()\
  static OdRxModule* g_pSingletonModule = 0;\
  extern "C" { \
  __attribute__ ((visibility("default"))) \
  OdRxModule* odrxCreateModuleObject(void *hModule, OdChar *name)\
{\
  if(!g_pSingletonModule)\
{\
  g_pSingletonModule = OdRxMacModule<_UserModuleClass >::createModule(hModule, name);\
  hModule = NULL;\
}\
  return g_pSingletonModule;\
}\
  void odrxGetAPIVersion(int& nMajorVersion, int& nMinorVersion,\
  int& nMajorBuildVersion, int& nMinorBuildVersion)\
{\
  nMajorVersion       = TD_MAJOR_VERSION;\
  nMinorVersion       = TD_MINOR_VERSION;\
  nMajorBuildVersion  = TD_MAJOR_BUILD;\
  nMinorBuildVersion  = TD_MINOR_BUILD;\
}\
}\
  void deleteModuleObject()\
{\
  g_pSingletonModule = NULL;\
}

#elif defined(__GNUC__) && !defined(_WIN32)

#define ODRX_STATIC_MODULE_PATH "{5CEAD1EF-4D33-48fe-99E4-E09176BCF088}/"

#define ODRX_STATIC_APP_MODULE_NAME(AppName) (OdString(ODRX_STATIC_MODULE_PATH) + AppName + DEFAULT_MODULE_EXTENSION_DOT_W)

void deleteModuleObject();

/** \details
  <group OdRx_Classes> 

  This template class provides the implementation of Unix rx-modules in multi-threaded (MT) mode.

  \sa
  OdRxModule, OdRefCounter classes
*/
template<class T, class TInterface = T>
class OdRxUnixModule : public T
{
  OdRefCounter   m_nLockCount;
  void          *m_hModule;
  const OdString m_name;
protected:
  OdRxUnixModule(void *hModule, const OdString name)
    : m_nLockCount(0)
    , m_hModule(hModule)
    , m_name(name)
  {}
private:
  void addRef() 
  { 
    ++m_nLockCount; 
  }
  void release() 
  { 
    ODA_ASSERT(m_nLockCount); 
    --m_nLockCount; 
  }
  long numRefs() const { return m_nLockCount; }
  ODRX_HEAP_OPERATORS();
public:
  /** \details
    Returns the system data for the existing Unix rx-module instance.
  */
  void* sysData() { return m_hModule; }
  
  /** \details
    Creates a new Unix rx-module instance with the specified module name and 
    returns a raw pointer to it.

    \param hModule [in]  Module handle.
    \param name [in]  module name as non-empty string value.
  */
  static OdRxUnixModule* createModule(void *hModule, const OdString name) 
  { 
    return new OdRxUnixModule(hModule, name); 
  }

  /** \details
    Deletes this Unix rx-module object.
  */
  void deleteModule()
  {
    deleteModuleObject();
    delete this;
  }

  /** \details
    Returns the module name for the Unix rx-module instance.
  */
  OdString moduleName() const
  {
    return m_name;
  }
};

/** \details
  Creates the entry point function for a Teigha custom application.
*/

#define ODRX_DEFINE_DYNAMIC_MODULE(_UserModuleClass)\
ODRX_ALLOC_OPERATORS()\
  static OdRxModule* g_pSingletonModule = 0;\
\
class OdRxUnixModule##_UserModuleClass : public OdRxUnixModule<_UserModuleClass>\
{\
  ODRX_HEAP_OPERATORS();\
  OdRxUnixModule##_UserModuleClass(void *hModule, const OdString name)\
    : OdRxUnixModule<_UserModuleClass>(hModule, name)\
  {\
  }\
public:\
  static _UserModuleClass* createModule(void *hModule, const OdString name) \
  { \
    return new OdRxUnixModule##_UserModuleClass(hModule, name);\
  }\
\
  virtual void deleteModule()\
  {\
    g_pSingletonModule = 0;\
    delete this;\
  }\
};\
\
  extern "C" { \
  __attribute__ ((visibility("default"))) \
  OdRxModule* odrxCreateModuleObject(void *hModule, OdChar *name)\
{\
  if(!g_pSingletonModule)\
{\
  g_pSingletonModule = OdRxUnixModule##_UserModuleClass::createModule(hModule, name);\
  hModule = NULL;\
}\
  return g_pSingletonModule;\
}\
  __attribute__ ((visibility("default"))) \
  void odrxGetAPIVersion(int& nMajorVersion, int& nMinorVersion,\
  int& nMajorBuildVersion, int& nMinorBuildVersion)\
{\
  nMajorVersion       = TD_MAJOR_VERSION;\
  nMinorVersion       = TD_MINOR_VERSION;\
  nMajorBuildVersion  = TD_MAJOR_BUILD;\
  nMinorBuildVersion  = TD_MINOR_BUILD;\
}\
}\
  void deleteModuleObject()\
{\
  g_pSingletonModule = NULL;\
}\

#else //defined(__GNUC__)

#define ODRX_STATIC_MODULE_PATH "{5CEAD1EF-4D33-48fe-99E4-E09176BCF088}/"

#define ODRX_STATIC_APP_MODULE_NAME(AppName) (OdString(ODRX_STATIC_MODULE_PATH) + AppName + DEFAULT_MODULE_EXTENSION_DOT_W)

#ifdef __BORLANDC__


/** \details
  <group OdRx_Classes> 

  This template class provides the implementation of Win32 rx-modules in multi-threaded (MT) mode.

  \sa
  OdRxModule, OdRefCounter classes
*/
template<class T, class TInterface = T>
class OdRxWin32Module : public T
{
  OdRefCounter   m_nLockCount;
  HMODULE        m_hModule;
  const OdString m_sModuleName;
  OdRxWin32Module() : m_nLockCount(0) { }
  void addRef()
  {
	++m_nLockCount;
  }
  void release()
  {
	ODA_ASSERT(m_nLockCount);
	--m_nLockCount;
  }
  long numRefs() const { return m_nLockCount; }
  OdRxWin32Module(HMODULE hModule , const OdString name )
	: m_nLockCount(0)
	, m_hModule(hModule)
	, m_sModuleName(name)
  {}
  ODRX_HEAP_OPERATORS();
public:
  /** \details
    Returns the system data for the existing Win32 rx-module instance.
  */
  void* sysData() { return reinterpret_cast<void*>(m_hModule); }

  /** \details
    Creates a new Win32 rx-module instance with the specified module name and 
    returns a raw pointer to it.

    \param hModule [in]  Module handle.
    \param name [in]  module name as non-empty string value.
  */
  static OdRxWin32Module* createModule(HMODULE hModule, const OdString name ) { return new OdRxWin32Module(hModule,name); }

  /** \details
    Deletes this Win32 rx-module object.
  */
  void deleteModule()
  {
     g_pSingletonModule = 0;
     delete this;
  }

  /** \details
    Returns the module name for the Win32 rx-module instance.
  */
  OdString moduleName() const
  {
     return m_sModuleName;
  }
};

/** \details
  Creates the entry point function for a Teigha custom application.
*/
#define ODRX_DEFINE_DYNAMIC_MODULE(_UserModuleClass)\
ODRX_ALLOC_OPERATORS()\
static OdRxModule* g_pSingletonModule = 0;\
  extern "C" {\
__declspec(dllexport) OdRxModule* odrxCreateModuleObject(HMODULE& hModule, const OdChar *name)\
{\
  if(!g_pSingletonModule)\
  {\
    g_pSingletonModule = OdRxWin32Module<_UserModuleClass >::createModule(hModule, name);\
    hModule = 0;\
  }\
  return g_pSingletonModule;\
}\
__declspec(dllexport) void odrxGetAPIVersion(int& nMajorVersion, int& nMinorVersion,\
                                             int& nMajorBuildVersion, int& nMinorBuildVersion)\
{\
  nMajorVersion       = TD_MAJOR_VERSION;\
  nMinorVersion       = TD_MINOR_VERSION;\
  nMajorBuildVersion  = TD_MAJOR_BUILD;\
  nMinorBuildVersion  = TD_MINOR_BUILD;\
}\
}\

#else //__BORLANDC__

void deleteModuleObject();

/** \details
  <group OdRx_Classes> 

  This template class provides the implementation of Win32 rx-modules in multi-threaded (MT) mode.

  \sa
  OdRxModule, OdRefCounter classes
*/
template<class T, class TInterface = T>
class OdRxWin32Module : public T
{
  OdRefCounter m_nLockCount;
  HMODULE      m_hModule;
  OdRxWin32Module() : m_nLockCount(0) { }
  void addRef()
  {
	++m_nLockCount;
  }
  void release()
  {
	ODA_ASSERT(m_nLockCount);
	--m_nLockCount;
  }
  long numRefs() const { return m_nLockCount; }
  OdRxWin32Module(HMODULE hModule )
	: m_nLockCount(0)
	, m_hModule(hModule)
  {}
  ODRX_HEAP_OPERATORS();
public:
  /** \details
    Returns the system data for the existing Win32 rx-module instance.
  */
  void* sysData() { return reinterpret_cast<void*>(m_hModule); }

  /** \details
    Creates a new Win32 rx-module instance with the specified module name and 
    returns a raw pointer to it.

    \param hModule [in]  Module handle.
    \param name [in]  module name as non-empty string value.
  */
  static OdRxWin32Module* createModule(HMODULE hModule) { return new OdRxWin32Module(hModule); }

  /** \details
    Deletes this Win32 rx-module object.
  */
  void deleteModule()
  {
     deleteModuleObject();
     delete this;
  }

  /** \details
    Returns the module name for the Win32 rx-module instance.
  */
  OdString moduleName() const
  {
     wchar_t buffer[_MAX_PATH];
     ::GetModuleFileNameW(m_hModule, buffer, _MAX_PATH );
     return buffer;
  }
};


/** \details
  Creates the entry point function for a Teigha custom application.
*/
#define ODRX_DEFINE_DYNAMIC_MODULE(_UserModuleClass)\
ODRX_ALLOC_OPERATORS()\
static OdRxModule* g_pSingletonModule = 0;\
  extern "C" {\
__declspec(dllexport) OdRxModule* odrxCreateModuleObject(HMODULE& hModule)\
{\
  if(!g_pSingletonModule)\
  {\
    g_pSingletonModule = OdRxWin32Module<_UserModuleClass >::createModule(hModule);\
    hModule = 0;\
  }\
  return g_pSingletonModule;\
}\
__declspec(dllexport) void odrxGetAPIVersion(int& nMajorVersion, int& nMinorVersion,\
                                             int& nMajorBuildVersion, int& nMinorBuildVersion)\
{\
  nMajorVersion       = TD_MAJOR_VERSION;\
  nMinorVersion       = TD_MINOR_VERSION;\
  nMajorBuildVersion  = TD_MAJOR_BUILD;\
  nMinorBuildVersion  = TD_MINOR_BUILD;\
}\
}\
  void deleteModuleObject()\
{\
  g_pSingletonModule = NULL;\
}

#endif //__BORLANDC__



#endif //defined(__GNUC__) && defined(__APPLE__)

# define ODRX_GET_MODULE_OBJECT(_UserModuleClass) g_pSingletonModule

#else //#ifdef _TOOLKIT_IN_DLL_ ////////////////////////////////////////////////////////////////////////////////

#define ODRX_STATIC_MODULE_PATH ""

#define ODRX_STATIC_APP_MODULE_NAME(AppName) AppName

#define ODRX_DEFINE_DYNAMIC_MODULE(_UserModuleClass) ODRX_DEFINE_STATIC_MODULE(_UserModuleClass)

# define ODRX_GET_MODULE_OBJECT(_UserModuleClass) g_pSingletonModule##_UserModuleClass


#endif //#ifdef _TOOLKIT_IN_DLL_ ////////////////////////////////////////////////////////////////////////////////



#if defined(_TOOLKIT_IN_DLL_) && (defined(_MSC_VER) || (defined(_WIN32) && defined(__GNUC__)))

/** \details
  Speeds up DLL loading on Windows. Refer to Win SDK documentation on DllMain() for more details.
*/
#define DISABLE_THREAD_LIBRARY_CALLS()\
extern "C" int APIENTRY DllMain(HINSTANCE h, DWORD reason, LPVOID ) {\
  if(reason==DLL_PROCESS_ATTACH)\
    DisableThreadLibraryCalls((HMODULE)h);\
  return TRUE;\
}

#else

#define DISABLE_THREAD_LIBRARY_CALLS()

#endif //#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER)


#include "TD_PackPop.h"

#endif // _OD_RXDYNAMICMODULE_H_

