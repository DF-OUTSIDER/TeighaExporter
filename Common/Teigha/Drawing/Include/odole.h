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




//   OdOle.h
//

#ifndef _ODOLEAUTO_H_
#define _ODOLEAUTO_H_

#ifdef _MSC_VER
#include <wtypes.h>
#endif

#include "DbDatabase.h"
struct IUnknown;
struct IDispatch;

/** \details
    This class maintains a link between an object and its respective COM wrapper.
    Corresponding C++ library: TD_Db
    <group Other_Classes>
*/
class OdOxOleLinkManager
{
public:
  /** \details
    Returns the IUnknown of the COM wrapper for the specified database -resident object.
    
	\param pObject [in]  Pointer to the object.
    
	\remarks
    Returns NULL if no wrapper is found.
  */
  virtual IUnknown* GetIUnknown(OdDbObject* pObject) = 0;

  /** \details
    Sets the link between the specified database -resident object and the specified COM wrapper.
    
	\param pObject [in]  Pointer to the object.
    \param pIUnknown [in]  Pointer to the COM wrapper.
    
	\remarks
    If pIUnknown is NULL, the link is removed.
  */
  virtual bool SetIUnknown(OdDbObject* pObject, IUnknown* pIUnknown = 0) = 0;

  /** 
  */
   /** \details
    Returns the IUnknown of the COM wrapper for the specified database.
    
	\param pDb [in]  Pointer to the database.
    
	\remarks
    Returns NULL if no wrapper is found.
  */
  virtual IUnknown* GetIUnknown(OdDbDatabase * pDb) = 0;


  /** \details
    Sets the link between the specified database and the specified COM wrapper.
    
	\param pDb [in]  Pointer to the database.
    \param pIUnknown [in]  Pointer to the COM wrapper.
    
	\remarks
    If pIUnknown is NULL, the link is removed.
  */
  virtual bool SetIUnknown(OdDbDatabase * pDb, IUnknown* pIUnknown = 0) = 0;

  /** \details
    Return the IDispatch of the document to which the specified database belongs.
    \param pDb [in]  Pointer to the database.
    \remarks
    Returns NULL if the database does not belong to a document.
  */
  virtual IDispatch* GetDocIDispatch(OdDbDatabase * pDatabase) = 0;

  /** \details
    Sets the link between the specified database and the specified document to which it belongs..
    \param pDb [in]  Pointer to the database.
    \param pIDispatch [in]  Pointer to the IDispatch of the document..
    \remarks
    If IDispatch is NULL, the link is removed.
  */
  virtual bool SetDocIDispatch(OdDbDatabase * pDatabase, IDispatch* pIDispatch = 0) = 0;
};

DBENT_EXPORT OdOxOleLinkManager* OdOxGetOleLinkManager(void);

#endif // _ODOLEAUTO_H_


