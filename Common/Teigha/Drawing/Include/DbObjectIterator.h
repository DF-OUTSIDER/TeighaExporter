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




#ifndef _ODDBOBJECTITERATOR_INCLUDED_
#define _ODDBOBJECTITERATOR_INCLUDED_

#include "RxObject.h"
#include "DbObjectId.h"

class OdDbEntity;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbEntity object pointers.
*/
typedef OdSmartPtr<OdDbEntity> OdDbEntityPtr;

/** \details
    This class defines bidirectional Iterator objects that traverse entities contained in complex entities.
    
    
    OdDbBlock Table Records in an OdDbDatabase instance.

    \sa
    TD_Db
    
    \remarks
    Complex entities include the following:
    
    <table>
    Entity               Iterated entities
    OdDbBlockReference   All
    OdDb2dPolyline       Vertex
    OdDb3dPolyline       Vertex
    OdDbPolyFaceMesh     Vertex
    OdDbPolygonMesh      Vertex
    </table>

    This class cannot directly instantiated, but must be instantiated with the
    iterator creation methods of the class to be iterated through.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObjectIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbObjectIterator);

  OdDbObjectIterator() {}

  /** \details
    Sets this Iterator object to reference the entity that it would normally return first.
     
    \remarks
    Allows multiple traversals of the iterator list.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipErased [in]  If and only if true, erased records are skipped.
  */
    virtual void start(
    bool atBeginning = true, 
    bool skipErased = true) = 0;

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() const = 0;

  /** \details
    Returns the Object ID of the entity currently referenced by this Iterator object.
  */
  virtual OdDbObjectId objectId() const = 0;

  /** \details
    Opens and returns the entity currently referenced by this Iterator object.

    \param openMode [in]  Mode in which to open the entity.
    \param openErasedEntity [in]  If and only if true, erased records will be opened or retrieved.

    \returns
    Returns a SmartPointer to the opened entity if successful, otherwise a null SmartPointer.
  */
  virtual OdDbEntityPtr entity(
    OdDb::OpenMode openMode = OdDb::kForRead, 
    bool openErasedEntity = false) = 0;

  /** \details
    Steps this Iterator object.

    \param forward [in]  True to step forward, false to step backward.
    \param skipErased [in]  If and only if true, erased records are skipped.
  */
  virtual void step(
    bool forward = true, 
    bool skipErased = true) = 0;

  /** \details
    Positions this Iterator object at the specified record.
    \param objectId [in]  Object ID of the entity.
  */
    virtual bool seek(
    OdDbObjectId objectId) = 0;
    
  /** \details
    Positions this Iterator object at the specified record.
    \param pEntity [in] Pointer to the entity.
  */
  virtual bool seek(
    const OdDbEntity* pEntity) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbObjectIterator object pointers.
*/
typedef OdSmartPtr<OdDbObjectIterator> OdDbObjectIteratorPtr;

#endif //_ODDBOBJECTITERATOR_INCLUDED_

