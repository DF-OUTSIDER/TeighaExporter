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

#ifndef _DBSSET_H_
#define _DBSSET_H_

#include "Ge/GePoint3d.h"
#include "DbDatabase.h"
#include "DbObjectId.h"
#include "DbObject.h"
#include "DbObjectIterator.h"
#include "ResBuf.h"
#include "DbSubentId.h"
#include "SSet.h"

//class OdDbSelectionSet;
//class OdDbBlockTableRecord;


/** \details
    This class is the abstract base class for all selection filters.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT ODRX_ABSTRACT OdDbSelectionFilter : public OdRxObject
{
protected:
  OdDbSelectionFilter();
public:
  ODRX_DECLARE_MEMBERS(OdDbSelectionFilter);

  virtual bool accept(const OdDbObjectId& entityId) const = 0;
};

typedef OdSmartPtr<OdDbSelectionFilter> OdDbSelectionFilterPtr;


class OdDbParametrizedSF;
typedef OdSmartPtr<OdDbParametrizedSF> OdDbParametrizedSFPtr;

/** \details
    This class represents selection filter defined by ResBuf chain.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT ODRX_ABSTRACT OdDbParametrizedSF : public OdDbSelectionFilter
{
protected:
  OdDbParametrizedSF();
public:
  static OdDbParametrizedSFPtr createObject(const OdResBuf* pSpec, const OdDbDatabase* pDb);

  virtual void setSpecification(const OdResBuf* /*pSpec*/, const OdDbDatabase* /*pDb*/ = 0) {}
  virtual OdResBufPtr specification() const { return OdResBufPtr(); }
  virtual const OdDbDatabase* database() const { return 0; }
};


/** \details
  This class implements iterators for OdDbSelectionSet objects.

  \remarks

  Instances are returned by OdDbSelectionSet::newIterator().

  <group OdDb_Classes>
*/
//class ODRX_ABSTRACT TOOLKIT_EXPORT OdDbSelectionSetIterator : public OdRxObject
class ODRX_ABSTRACT TOOLKIT_EXPORT OdDbSelectionSetIterator : public OdSelectionSetIterator
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSelectionSetIterator);

  /** \details
      Returns the Object ID of the entity at the current position specified by
      this Iterator object.
  */
  virtual OdDbObjectId objectId() const = 0;
  
protected:
  OdDbSelectionSetIterator();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSelectionSetIterator object pointers.
*/
typedef OdSmartPtr<OdDbSelectionSetIterator>  OdDbSelectionSetIteratorPtr;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSelectionSet object pointers.
*/
typedef OdSmartPtr<OdDbSelectionSet>  OdDbSelectionSetPtr;

/** \details
    This class represents Selection Sets in an Teigha application.

    \remarks
    Only entities from an OdDbDatabase object's ModelSpace or PaperSpace may be
    added to a Selection Set.

    <group OdDb_Classes>
*/
class ODRX_ABSTRACT TOOLKIT_EXPORT OdDbSelectionSet : public OdSelectionSet
{
protected:
  OdDbSelectionSet();
public:
  ODRX_DECLARE_MEMBERS(OdDbSelectionSet);

  /** \details
    Creates a SelectionSet object for the specified database. 
    \param pDb [in]  Pointer to the database object for which to create the SelectionSet object.
    \returns
    Returns a SmartPointer to the new SelectionSet object.
  */
  static OdDbSelectionSetPtr createObject(const OdDbDatabase *pDb);
  
  virtual const OdDbDatabase* database() const = 0;

  /** \details
      Returns an Iterator object that provides access to the entities in this Selection Set.
  */
  //virtual OdSelectionSetIteratorPtr newIterator() const = 0; 

  virtual OdDbObjectIdArray objectIdArray() const = 0;

  // operations on the selection set 

  /** \details
    Filter selection only.  

    \param pDb [in]  Pointer to the database object for which to create the SelectionSet object.
    \param pFilter [in] object of type OdDbSelectionFilter or pointer to a OdResBuf chain defining the filter.
  */
  static OdDbSelectionSetPtr select(const OdDbDatabase *pDb, const OdRxObject* pFilter = 0);

  /** \details
    Filter selection only.  

    \param vpId [in]  id of OdDbViewportTableRecord  record or OdDbViewport entity in which to select.
    \param nPoints [in]  Number of points into selection shape.
    \param wcsPts [in]  Selection shape as set of World Coordinate Space points.
    \param mode [in]  Selection mode.
    \param sm [in]  Subentities selection mode.
    \param pFilter [in] object of type OdDbSelectionFilter or pointer to a OdResBuf chain defining the filter.
  */
  static OdDbSelectionSetPtr select(
    const OdDbObjectId& vpId,
    int nPoints,
    const OdGePoint3d* wcsPts,
    OdDbVisualSelection::Mode mode = OdDbVisualSelection::kCrossing,
    OdUInt32 sm = OdDbVisualSelection::kDisableSubents,
    const OdRxObject* pFilter = 0);

  /** \details
    Returns the number of entities in this group.
  */
  //virtual OdUInt32 numEntities() const = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDbEntity. 
    \param method [in] Selection method to associate with object
  */
  TD_USING(OdSelectionSet::append);
  //virtual void append(OdDbStub* entityId, OdDbSelectionMethod* pMethod = 0) = 0;
  
  virtual void append(const OdDbObjectId& entityId, OdDbSelectionMethod* pMethod = 0) = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param entityIds [in]  Array of Object IDs of the OdDbEntity's.
    \param pMethod [in] Selection method to associate with objects.
  */
  virtual void append(const OdDbObjectIdArray& entityIds, OdDbSelectionMethod* pMethod = 0);


  //virtual void append(const OdDbBaseFullSubentPath& subent, OdDbSelectionMethod* pMethod = 0) = 0;
  
  /** \details
    Appends the specified subentity to this SelectionSet object.  

    \param subent [in]  Subentity path to append.
    \param pMethod [in] Selection method to associate with objects  
  */
  virtual void append(const OdDbFullSubentPath& subent, OdDbSelectionMethod* pMethod = 0) = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param pSSet [in]  Other selection set to copy entities from. 
  */
  virtual void append(const OdSelectionSet* pSSet);

  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDbEntity. 
  */
  TD_USING(OdSelectionSet::remove);
  //virtual void remove(OdDbStub* entityId) = 0;
  
  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDbEntity. 
  */
  virtual void remove(const OdDbObjectId& entityId) = 0;

  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param entityIds [in]  Array Object ID of the OdDbEntity. 
  */
  virtual void remove(const OdDbObjectIdArray& entityIds);


  //virtual void remove(const OdDbBaseFullSubentPath& subent) = 0;
  
  /** \details
  Removes the specified subentity from this SelectionSet object.  

  \param subent [in]  Subentity path to remove. 
  */
  virtual void remove(const OdDbFullSubentPath& subent) = 0;

  /** \details
  Removes the specified entity from this SelectionSet object.  

  \param pSSet [in]  Object ID of the OdDbEntity. 
  */
  virtual void remove(const OdSelectionSet* pSSet);

  /** \details
    Returns true if and only if the specified entity is a member
    of this SelectionSet object.
    \param entityId [in]  Object ID of the OdDbEntity. 
  */
  TD_USING(OdSelectionSet::isMember);
  //virtual bool isMember(OdDbStub* entityId) const = 0;
  
  /** \details
    Returns true if and only if the specified subentity is a member
    of this SelectionSet object.
    \param entityId [in]  Subentity path. 
  */
  virtual bool isMember(const OdDbObjectId& entityId) const = 0;


  //virtual bool isMember(const OdDbBaseFullSubentPath& subent) const = 0;
  
  /** \details
    Returns true if and only if the specified subentity is a member
    of this SelectionSet object.
    \param entityId [in]  Subentity path. 
  */
  virtual bool isMember(const OdDbFullSubentPath& subent) const = 0;

  TD_USING(OdSelectionSet::method);
  //virtual OdDbSelectionMethodPtr method(OdDbStub* entityId) const = 0;
  
  /** \details
    Returns the selection method for the given subentity.
    
    \param entityId [in]  Subentity path.
  */
  virtual OdDbSelectionMethodPtr method(const OdDbObjectId& entityId) const = 0;

  /** \details
  Returns the number of subentities selected inside the entity with the given id.
  (analogous to acedSSSubentLength)
  \param entityId [in]  entity id for the parent of the subentity

  \remarks
  Returns true if and only if not at the end of the list.
  */
  virtual OdUInt32 subentCount(const OdDbObjectId& entityId) const = 0;

  /** \details
  Returns the given subentity path inside the entity with the given id.
  (analogous to acedSSSubentName)
  \param entityId [in]  entity id for the parent of the subentity
  \param i [in]  index of the subentity.
  \param path [out]  Path to the subentity.

  \returns
  Returns true if and only if "i" is valid index in the subentities array.
  */
  virtual bool getSubentity(const OdDbObjectId& entityId, OdUInt32 i, OdDbFullSubentPath& path) const = 0;
  
  /** \details
    Returns the selection method for the given subentity.
  */
  virtual OdDbSelectionMethodPtr method(const OdDbFullSubentPath& subent) const = 0;
};

#endif //_DBSSET_H_
