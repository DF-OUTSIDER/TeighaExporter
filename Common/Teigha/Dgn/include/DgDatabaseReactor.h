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
#ifndef __DG_DATABASEREACTOR_H__
#define __DG_DATABASEREACTOR_H__

#include "TD_PackPush.h"

#include "DgExport.h"
#include "RxObject.h"

/** \details
    This class is the base class for custom classes that receive notification
    of OdDgDatabase events.
    
    \remarks
    Events consist of the addition, modification, or deletion of objects 
    from an OdDgDatabase instance.
    
    \remarks
    The default implementations of all methods in this class do nothing.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDatabaseReactor : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDatabaseReactor);
  
  OdDgDatabaseReactor() {}
  
  /** \details
    Notification function called whenever an OdDgElement has been appended to an OdDgDatabase.
    
    \param pDb [in]  Pointer to the database being modified.
    \param pElement [in]  Pointer to the object being appended.
    
    \remarks
    This function is called after the operation.
  */
  virtual void elementAppended(
    const OdDgDatabase* pDb, 
    const OdDgElement* pElement);

  
  /** \details
    Notification function called whenever an OdDgElement has been unappended from an OdDgDatabase by an Undo operation.
    
    \param pDb [in]  Pointer to the database being modified.
    \param pElement [in]  Pointer to the object being unappended.

    \remarks
    This function is called after the operation.
  */
  virtual void elementUnAppended(
    const OdDgDatabase* pDb, 
    const OdDgElement* pElement);

  
  /** \details
    Notification function called whenever an OdDgElement has been reappended to an OdDgDatabase by a Redo operation.
    
    \param pDb [in]  Pointer to the database being modified.
    \param pElement [in]  Pointer to the object being reappended.

    \remarks
    This function is called after the operation.
  */
  virtual void elementReAppended(
    const OdDgDatabase* pDb, 
    const OdDgElement* pElement);

  
  /** \details
    Notification function called whenever an OdDgElement is about to be modified.
    
    \param pDb [in]  Pointer to the database being modified.
    \param pElement [in]  Pointer to the object being modified.
    
    \remarks
    This function is called before the operations.
    
    \sa
    objectModified
  */
  virtual void elementOpenedForModify(
    const OdDgDatabase* pDb, 
    const OdDgElement* pElement);

  
  /** \details
    Notification function called whenever an OdDgElement has been modified.
    
    \param pDb [in]  Pointer to the database being modified.
    \param pElement [in]  Pointer to the object being modified.
    
    \remarks
    This function is called after the operations.

    \sa
    objectOpenedForModify
  */
  virtual void elementModified(
    const OdDgDatabase* pDb, 
    const OdDgElement* pElement);
    
  
  /** \details
    Notification function called whenever an OdDgElement has been erased or unerased.
    
    \param pDb [in]  Pointer to the database being modified.
    \param pElement [in]  Pointer to the object being modified.
    \param erased [in]  True if and only if the object is being erased.
    
    \remarks
    This function is called after the operation.
  */
  virtual void elementErased(
    const OdDgDatabase* pDb, 
    const OdDgElement* pElement, 
    bool erased = true);

  /** \details
    Notification function called whenever a database -resident system variable is about to change.
    
    \param pDb [in]  Pointer to the database being modified.
    \param name [in]  Name of the system variable being modified.
    
    \remarks
    This function is called before the operation.
    
    \sa
    headerSysVarChanged
  */
  virtual void headerSysVarWillChange(
    const OdDgDatabase* pDb, 
    const OdString& name);

/**DOM-IGNORE-BEGIN*/
///** { Secret } */
//#define RO_VAR_DEF(type, name, d1, d2, r1, r2)
///** { Secret } */
//#define RO_VAR_DEF_H(type, name, d1, d2, r1, r2)
///** { Secret } */
//#define VAR_DEF(type, name, d1, d2, r1, r2)\
//  /** Description: Notification function called whenever the ##NAME system variable is about to change. \
//    Arguments: pDb (I) Pointer to the *database* being *modified*. \
//    Remarks: This function is called before the operation. \
//    Remarks: ##remarks \
//    Notes: ##notes \
//    See Also: ##see_also \
//  */\
//  virtual void headerSysVar_##name##_WillChange(const OdDgDatabase* pDb);
//
//#include "DgSysVarDefs.h"
//#undef VAR_DEF
//#undef RO_VAR_DEF
//#undef RO_VAR_DEF_H
//
///** { Secret } */
//#define VAR_DEF(type, name, dxf, d1, d2, r1, r2)\
//  /** Description: Notification function called whenever the DIM##NAME system variable is about to change. \
//    Arguments: pDb (I) Pointer to the *database* being *modified*. \
//    Remarks: This function is called before the operation. \
//    Remarks: ##remarks \
//    Notes: ##notes \
//    See Also: ##see_also \
//  */ \
//  virtual void headerSysVar_dim##name##_WillChange(const OdDgDatabase* pDb);
//
//#include "DgDimVarDefs.h"
//#undef VAR_DEF
//
//
/**DOM-IGNORE-END*/

  /** \details
    Notification function called whenever a database -resident system variable has changed.
    
    \param pDb [in]  Pointer to the database being modified.
    \param name [in]  Name of the system variable being modified.
    
    \remarks
    This function is called after the operation.
    
    \sa
    headerSysVarWillChange
  */
  virtual void headerSysVarChanged(
    const OdDgDatabase* pDb, 
    const OdString& name);

/**DOM-IGNORE-BEGIN*/
//
///** { Secret } */
//#define VAR_DEF(type, name, d1, d2, r1, r2)\
//  /** Description: Notification function called whenever the ##NAME system variable has been changed. \
//    Arguments: pDb (I) Pointer to the *database* being *modified*. \
//    Remarks: This function is called after the operation. \
//    Remarks: ##remarks \
//    Notes: ##notes \
//    See Also: ##see_also \
//  */ \
//  virtual void headerSysVar_##name##_Changed(const OdDgDatabase* pDb);
//
//#include "DgSysVarDefs.h"
//#undef VAR_DEF
//
///** { Secret } */
//#define VAR_DEF(type, name, dxf, d1, d2, r1, r2)\
//  /** Description: Notification function called whenever the DIM##NAME system variable has been changed. \
//    Arguments: pDb (I) Pointer to the *database* being *modified*. \
//    Remarks: This function is called after the operation. \
//    Remarks: ##remarks \
//    Notes: ##notes \
//    See Also: ##see_also \
//  */ \
//  virtual void headerSysVar_dim##name##_Changed(const OdDgDatabase* pDb);
//
//#include "DgDimVarDefs.h"
//#undef VAR_DEF
//
/**DOM-IGNORE-END*/

  /** \details
    Notification function called whenever a database is about to be deleted from memory.
    \param pDb [in]  Pointer to the database being deleted.

    \remarks
    This function is called before the operation.
  */
  virtual void goodbye(
    const OdDgDatabase* pDb);
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDatabaseReactor object pointers.
*/
typedef OdSmartPtr<OdDgDatabaseReactor> OdDgDatabaseReactorPtr;

#include "TD_PackPop.h"

#endif // __DG_DATABASEREACTOR_H__
