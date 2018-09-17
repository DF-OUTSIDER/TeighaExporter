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

#ifndef _ODDB_OBJECTCONTEXTMANAGER_H_INCLUDED_
#define _ODDB_OBJECTCONTEXTMANAGER_H_INCLUDED_

#include "RxObject.h"
#include "TD_PackPush.h"

class OdDbObjectContextCollection;
class OdDbObjectContext;
class OdDbObjectContextManagerImpl;

////////////////////////////////////////////////////////////////////////
// class OdDbObjectContextManager
////////////////////////////////////////////////////////////////////////

/** \details
    This class returns and manages the context objects that are available in a drawing.
    
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbObjectContextManager : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectContextManager);

  OdDbObjectContextManager(OdDbObjectContextManagerImpl*);

  virtual ~OdDbObjectContextManager();

  /** \details
    Registers a context collection with the context manager.
  
	\param collectionName [in]  Name of the context collection.
	\param pCollection [in]  Pointer to the context collection.
 
	\returns
	Returns eOk if successful, or an appropriate error code if not.
  */
     OdResult registerContextCollection(
    const OdString& collectionName,
    OdDbObjectContextCollection* pCollection);

  /** \details
    Unregisters a context collection from the context manager.
  
	\param collectionName [in]  Name of the context collection.
 
	\returns
	Returns eOk if successful, or an appropriate error code if not.
  */
      OdResult unregisterContextCollection(
    const OdString& collectionName);

   /** \details
    Returns a pointer to a context collection using its name as an identifier.
  
	\param collectionName [out]  Name of the context collection.
 
	\returns
	Returns a pointer to the registered context collection. 
	If no collection exists	with the specified name, Null is returned.
   */
     OdDbObjectContextCollection* contextCollection(
    const OdString& collectionName) const;

protected:
  OdDbObjectContextManagerImpl* m_pImpl;
};

#include "TD_PackPop.h"

#endif //_ODDB_OBJECTCONTEXTMANAGER_H_INCLUDED_
