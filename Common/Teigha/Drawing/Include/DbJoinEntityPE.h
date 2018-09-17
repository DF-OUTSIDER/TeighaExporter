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

#ifndef OD_DBJOINENTITYPE_H
#define OD_DBJOINENTITYPE_H

#include "DbEntity.h"

#include "TD_PackPush.h"

/** \details
  This abstract class declares the base JOIN functionality for entities that allows searching 
  intersections between them and uniting them in a single curve using the protocol extension mechanism.

  Corresponding C++ library: TD_Db    
  <group Join_Classes>
*/
class DBENT_EXPORT OdDbJoinEntityPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbJoinEntityPE);

  /** \details
    This pure virtual method that must be implemented in derived class to provide the 
    join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out] Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const = 0;

  /** \details
    This virtual method that must be implemented in derived class to provide the join operation of 
    two supported entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    Some implementations can require the same type for primary and secondary entities.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of the secondary entity joined to it, when it is possible. 
    \param secondaryEntity [in] Smart pointer to the secondary entity that represents own parameters 
                        for the primary entity joined to it.
  */
  virtual OdResult joinEntity(OdDbEntityPtr primaryEntity, OdDbEntityPtr secondaryEntity) const;
};

/** \details
  The typified smart pointer for join operation of entities. This  
  template class is specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbJoinEntityPE> OdDbJoinEntityPEPtr;

#include "TD_PackPop.h"

#endif // OD_DBJOINENTITYPE_H

