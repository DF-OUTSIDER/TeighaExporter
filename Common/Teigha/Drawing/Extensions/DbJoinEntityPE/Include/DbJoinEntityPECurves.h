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


#ifndef _DB_JOIN_ENTITY_PE_INCLUDED_
#define _DB_JOIN_ENTITY_PE_INCLUDED_

#include "DbJoinEntityPE.h"

#include "TD_PackPush.h"

/** \details
  This namespace provides the join functionality for entities and allows searching of
  intersections between entities and uniting them in a single curve. Join functionality 
  is implemented through a protocol extension mechanism and implements some classes
  by deriving them from one base, OdDbJoinEntityPE class, which declares the join PE interface. 
  Classes provide the join implementation of virtual methods declared in a base abstract
  class for corresponding types of entities.

  Library: Source code provided.
  <group Join_Classes> 
*/
namespace joinEntityPE
{

/** \details
  This protocol extension class implements the join operation for straight line entities.
  Library: Source code provided.
  <group Join_Classes> 
*/
class OdDbLineJoinEntityPE : public OdDbJoinEntityPE
{
  /** \details
    Performs the join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out] Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const;
};

/** \details
  The typified smart pointer for join operation of straight line entities. This  
  template class is a specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbLineJoinEntityPE> OdDbLineJoinEntityPEPtr;



/** \details
  This protocol extension class implements the join operation for circular arc entities.
  Library: Source code provided.
  <group Join_Classes> 
*/
class OdDbArcJoinEntityPE : public OdDbJoinEntityPE
{
  /** \details
    Performs the join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out]) Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const;
};

/** \details
  The typified smart pointer for a join operation of circular arc entities. This  
  template class is a specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbArcJoinEntityPE> OdDbArcJoinEntityPEPtr;



/** \details
  This protocol extension class implements the join operation for ellipse and elliptical arc entities.
  Library: Source code provided.
  <group Join_Classes> 
*/
class OdDbEllipseJoinEntityPE : public OdDbJoinEntityPE
{
  /** \details
    Performs the join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out] Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const;
};

/** \details
  The typified smart pointer for a join operation of ellipse and elliptical arc entities. This  
  template class is a specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbEllipseJoinEntityPE> OdDbEllipseJoinEntityPEPtr;



/** \details
  This protocol extension class implements the join operation for three-dimensional polyline entities.
  Library: Source code provided.
  <group Join_Classes> 
*/
class OdDb3dPolylineJoinEntityPE : public OdDbJoinEntityPE
{
  /** \details
    Performs the join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out] Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const;
};

/** \details
  The typified smart pointer for a join operation of three-dimensional polyline entities. This  
  template class is a specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDb3dPolylineJoinEntityPE> OdDb3dPolylineJoinEntityPEPtr;



/** \details
  This protocol extension class implements the join operation for lightweight polyline entities.
  Library: Source code provided.
  <group Join_Classes> 
*/
class OdDbPolylineJoinEntityPE : public OdDbJoinEntityPE
{
  /** \details
    Performs the join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out] Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const;
};

/** \details
  The typified smart pointer for a join operation of lightweight polyline entities. This  
  template class is a specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbPolylineJoinEntityPE> OdDbPolylineJoinEntityPEPtr;



/** \details
  This protocol extension class implements the join operation for two-dimensional polyline entities.
  Library: Source code provided.
  <group Join_Classes> 
*/
class OdDb2dPolylineJoinEntityPE : public OdDbJoinEntityPE
{
  /** \details
    Performs the join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out] Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const;
};

/** \details
  The typified smart pointer for a join operation of two-dimensional polyline entities. This  
  template class is a specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDb2dPolylineJoinEntityPE> OdDb2dPolylineJoinEntityPEPtr;



/** \details
  This protocol extension class implements the join operation for spline entities.
  Library: Source code provided.
  <group Join_Classes> 
*/
class OdDbSplineJoinEntityPE : public OdDbJoinEntityPE
{
  /** \details
    Performs the join operation between entities. Returns eOk if the join is successful, otherwise, eInvalidInput.
    
    \param primaryEntity [in]  Smart pointer to the primary entity that should be updated using 
                      parameters of other entities joined to it, when it is possible. 
    \param otherEntities [in] Array of smart pointers to other entities that are joined to the primary entity.
    \param joinedEntityIndices [out] Reference to an array of entity indices from the array of other entities 
                            (second argument) that were joined successfully. 
  */
  virtual OdResult joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const;
};

/** \details
  The typified smart pointer for a join operation of spline entities. This  
  template class is a specialization of the OdSmartPtr class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSplineJoinEntityPE> OdDbSplineJoinEntityPEPtr;
}

#endif // _DB_JOIN_ENTITY_PE_INCLUDED_

