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

#ifndef _DBOBJECTCONTEXTINTERFACE_H_INCLUDED_
#define _DBOBJECTCONTEXTINTERFACE_H_INCLUDED_

#include "RxObject.h"

class OdDbObjectContext;
class OdDbObject;

/** \details
    This class is the Protocol Extension class for OdDbObjectContext objects.
    
    \remarks
    Implement this interface to use OdDbObjectContext objects, which can determine 
	the properties and behaviors that are available for custom objects.
        
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbObjectContextInterface : public OdRxObject
{
public:

  ODRX_DECLARE_MEMBERS(OdDbObjectContextInterface);
 
   /** \details
    Returns whether a context collection is supported by a specified object.
  
	\param pObject [in]  Pointer to the object.
	\param collectionName [in]  Name of the context collection.
 
	\returns
	Returns true if the object supports the named context collection, or returns
	false if it does not.
   */
      virtual bool supportsCollection( 
    const OdDbObject* pObject,
    const OdString&   collectionName ) const = 0;

   /** \details
    Returns whether a context is used by a specified object.
  
	\param pObject [in]  Pointer to the object.
	\param context [in]  Name of the context.
 
	\returns
	Returns true if the object uses the named context, or returns
	false if it does not.
   */
      virtual bool hasContext ( 
    const OdDbObject*        pObject,
    const OdDbObjectContext& context ) const = 0;

   /** \details
    Adds a context to the specified object.
  
	\param pObject [in]  Pointer to the object.
	\param context [in]  Name of the context to add.
 
	\returns
	Returns eOk if successful, or an appropriate error code if not.
   */
      virtual OdResult addContext ( 
    OdDbObject*              pObject,
    const OdDbObjectContext& context ) const = 0;

  /** \details
    Removes a context from the specified object.
  
	\param pObject [in]  Pointer to the object.
	\param context [in]  Name of the context to remove.
 
	\returns
	Returns eOk if successful, or an appropriate error code if not.
  */
      virtual OdResult removeContext ( 
    OdDbObject*              pObject,
    const OdDbObjectContext& context ) const = 0;
};

typedef OdSmartPtr<OdDbObjectContextInterface> OdDbObjectContextInterfacePtr;

#endif //_DBOBJECTCONTEXTINTERFACE_H_INCLUDED_
