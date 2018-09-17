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




#ifndef _ODAXWRAP_H_INCLUDED_
#define _ODAXWRAP_H_INCLUDED_

#if defined(_MSC_VER) && (_MSC_VER > 1310) && defined(_TOOLKIT_IN_DLL_) && !defined(_WIN32_WCE)
#include "TD_PackPush.h"

#define _INC_MALLOC
#include <comdef.h>

#include "oaidl.h"
#include "OdaX.h"
class OdDbObjectId;
class OdGePoint3d;
class OdGeVector3d;
class OdGePoint2d;
class OdGeVector2d;
class OdDbHostAppServices;

interface __declspec(uuid("C9E0781D-BA3D-4224-9FA4-58ECEA2BC559")) 
IAcadBaseObject : public IUnknown
{
  // IAcadBaseObject methods
  virtual HRESULT STDMETHODCALLTYPE SetObjectId(OdDbObjectId& objId, 
    OdDbObjectId ownerId = OdDbObjectId::kNull, TCHAR* keyName = 0) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetObjectId(OdDbObjectId* objId) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(OdDbObjectId ownerId, LPUNKNOWN* pUnkClone) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID& clsid) = 0;
  virtual HRESULT STDMETHODCALLTYPE NullObjectId() = 0;
  virtual HRESULT STDMETHODCALLTYPE OnModified() = 0;
};

typedef IAcadBaseObject* LPACADBASEOBJECT;


// Definition of interface: IRetrieveApplication
interface __declspec(uuid("0E25DE83-2257-4b6d-B73B-33F1D21FFD8D"))
IRetrieveHostAppServices : IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE GetHostAppServices(OdDbHostAppServices** ppHostAppServices) = 0;
};

_COM_SMARTPTR_TYPEDEF(IRetrieveHostAppServices, __uuidof(IRetrieveHostAppServices));

// Definition of interface: IRetrieveApplication
interface __declspec(uuid("765B4640-664A-11cf-93F3-0800099EB3B7")) 
IRetrieveApplication : public IUnknown
{
  // IRetrieveApplication methods
  virtual HRESULT STDMETHODCALLTYPE SetApplicationObject(LPDISPATCH pAppDisp) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetApplicationObject(LPDISPATCH* pAppDisp) = 0;
};

typedef IRetrieveApplication* LPRETRIEVEAPPLICATION;

// Definition of interface: IAcadBaseDatabase
interface __declspec(uuid("CD3EB5B8-F3FC-48c2-84EE-954EFC4D4208")) 
IAcadBaseDatabase : public IUnknown
{
  // IAcadBaseObject methods
  virtual HRESULT STDMETHODCALLTYPE SetDatabase(OdDbDatabase*& pDb) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetDatabase(OdDbDatabase** pDb) = 0;
  virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID& clsid) = 0;
};

typedef IAcadBaseDatabase* LPACADBASEDATABASE;


_COM_SMARTPTR_TYPEDEF(IAcadBaseObject, __uuidof(IAcadBaseObject));
_COM_SMARTPTR_TYPEDEF(IRetrieveApplication, __uuidof(IRetrieveApplication));
_COM_SMARTPTR_TYPEDEF(IAcadBaseDatabase, __uuidof(IAcadBaseDatabase));


// {4D07FC10-F931-11ce-B001-00AA006884E5}
DEFINE_GUID(IID_ICategorizeProperties, 0x4d07fc10, 0xf931, 0x11ce, 0xb0, 0x1, 0x0, 0xaa, 0x0, 0x68, 0x84, 0xe5);

#include "OdaxCategoryProperties.h"


typedef int PROPCAT;

#ifndef __OBJEXT_H
interface __declspec(uuid("4D07FC10-F931-11ce-B001-00AA006884E5"))
ICategorizeProperties : public IUnknown
{
  // Return a property category for the specified property.
  virtual HRESULT STDMETHODCALLTYPE MapPropertyToCategory(
    /*[in]*/ DISPID dispid, /*[out]*/ PROPCAT* ppropcat) = 0;

  // Return the name associated with the specified category ID, as a BSTR.
  virtual HRESULT STDMETHODCALLTYPE GetCategoryName(
    /*[in]*/ PROPCAT propcat, /*[in]*/ LCID lcid, /*[out]*/BSTR* pbstrName) = 0;
};

typedef ICategorizeProperties FAR* LPCATEGORIZEPROPERTIES;
#endif

//
// Utility functions
//
DBENT_EXPORT IUnknown* OdOxGetIUnknownOfObject(OdDbObjectId objId, LPDISPATCH pApp);
//DBENT_EXPORT IUnknown* OdOxGetIUnknownOfObject(OdDbObject* pObj, LPDISPATCH pApp);
DBENT_EXPORT IUnknown* OdOxGetIUnknownOfDatabase(OdDbDatabase* pDb, LPDISPATCH pApp);


#define FACILITY_ODA                  32

#define OdHresultFromOdResult(res)    MAKE_HRESULT(res!=eOk ? 3 : 0, FACILITY_ODA, res)

#include "TD_PackPop.h"
#endif //_WIN32

#if defined(_MSC_VER) && defined(_TOOLKIT_IN_DLL_) && (_MSC_VER > 1310) && !defined(_WIN32_WCE)
#define  _DEFINE_GET_CLASS_ID(Class, ID) \
OdResult Class::subGetClassID(void* pClsid) const { *(CLSID*)pClsid = ID; return eOk;}
#else
#define  _DEFINE_GET_CLASS_ID(Class, CLSID) \
OdResult Class::subGetClassID(void* pClsid) const { return eNotImplemented; }
#endif // _TOOLKIT_IN_DLL_

#endif // _ODAXWRAP_H_INCLUDED_

