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

#ifndef DBASSOCPERSSUBENTMANAGER_INCLUDED_
#define DBASSOCPERSSUBENTMANAGER_INCLUDED_ /*!DOM*/

#include "TD_PackPush.h"
#include "DbObject.h"
#include "ShHistory/DbManagerSubentData.h"
#include "TD_3DSolidHistoryDef.h"
#include "ModelerDefs.h"


using namespace OdDbHistoryInternal;
/** \details
  This class represents the persistent subentity manager.

  Corresponding C++ library: Source code provided.
  <group Contraint_Classes> 
*/
class DB3DSOLIDHISTORY_EXPORT OdDbAssocPersSubentManager : public OdDbObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocPersSubentManager);
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocPersSubentManager();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocPersSubentManager();

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \returns
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \returns
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  //test method
  OdResult parseSubentData(SubentDataArray& pData);
  //test method
  OdUInt32 lastStepId();  
  OdArray <OdUInt32> infoAcisDataIndexes(OdUInt32 depIndex);
  static OdDbObjectId getInstanceFromDatabase(OdDbDatabase const * pDatabase);
  static OdDbObjectId createIfItDoesNotExist(OdDbDatabase* pDatabase);
  OdResult addSubentRWData(AcisDataType dt, OdUInt32 idx, OdArray<OdUInt32> arr);

  //OdUInt32 getStepIdForObject(class OdDbObject* pObj/*, bool, bool*/);

  OdUInt32 createNewStep(); 

 // virtual OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver, OdDbObjectId& replaceId, bool& exchangeXData);
};


/** \details
  The typified smart pointer for the manager object. This template class 
  is specialization of the OdSmartPtr class for the OdDbAssocPersSubentManager class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocPersSubentManager> OdDbAssocPersSubentManagerPtr;

#include "TD_PackPop.h"

#endif //DBASSOCPERSSUBENTMANAGER_INCLUDED_
