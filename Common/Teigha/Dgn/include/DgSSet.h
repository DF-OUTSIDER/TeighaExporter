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


#ifndef _DG_SSET_H_
#define _DG_SSET_H_

#include "SSet.h"
#include "DgElementId.h"
#include "DgElementIdArray.h"
#include "Gs/Gs.h"

//#include "Ge/GePoint3d.h"
//#include "DbDatabase.h"
//#include "DbObjectId.h"
//#include "DbObject.h"
//#include "DbObjectIterator.h"
//#include "ResBuf.h"
//#include "DbSubentId.h"

class OdDgDatabase;
class OdDgSelectionSet;
class OdDgFullSubentPath;
//class OdDgBlockTableRecord;


/** \details
    This class is the abstract base class for all selection filters.

    <group OdDg_Classes>
*/
class TG_EXPORT ODRX_ABSTRACT OdDgSelectionFilter : public OdRxObject
{
protected:
  OdDgSelectionFilter();
public:
  ODRX_DECLARE_MEMBERS(OdDgSelectionFilter);

  virtual bool accept(const OdDgElementId& entityId) const = 0;
};

typedef OdSmartPtr<OdDgSelectionFilter> OdDgSelectionFilterPtr;


class OdDgParametrizedSF;
typedef OdSmartPtr<OdDgParametrizedSF> OdDgParametrizedSFPtr;

///** Description:
//    This class represents selection filter defined by ResBuf chain.
//
//    {group:OdDg_Classes}
//*/
//class TG_EXPORT ODRX_ABSTRACT OdDgParametrizedSF : public OdDgSelectionFilter
//{
//protected:
//  OdDgParametrizedSF();
//public:
//  static OdDgParametrizedSFPtr createObject(const OdResBuf* pSpec, const OdDgDatabase* pDb);
//
//  virtual void setSpecification(const OdResBuf* /*pSpec*/, const OdDgDatabase* /*pDb*/ = 0) {}
//  virtual OdResBufPtr specification() const { return OdResBufPtr(); }
//  virtual const OdDgDatabase* database() const { return 0; }
//};


/** \details
  This class implements iterators for OdDgSelectionSet objects.

  \remarks

  Instances are returned by OdDgSelectionSet::newIterator().

  <group OdDg_Classes>
*/
class ODRX_ABSTRACT TG_EXPORT OdDgSelectionSetIterator : public OdSelectionSetIterator
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSelectionSetIterator);

  /** \details
      Returns the Object ID of the entity at the current position specified by
      this Iterator object.
  */
  virtual OdDgElementId objectId() const = 0;
  
protected:
  OdDgSelectionSetIterator();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgSelectionSetIterator object pointers.
*/
typedef OdSmartPtr<OdDgSelectionSetIterator>  OdDgSelectionSetIteratorPtr;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgSelectionSet object pointers.
*/
typedef OdSmartPtr<OdDgSelectionSet>  OdDgSelectionSetPtr;

/** \details
    This class represents Selection Sets in an Teigha application.

    \remarks
    Only entities from an OdDgDatabase object's ModelSpace or PaperSpace may be
    added to a Selection Set.

    <group OdDg_Classes>
*/
class ODRX_ABSTRACT TG_EXPORT OdDgSelectionSet : public OdSelectionSet
{
protected:
  OdDgSelectionSet();
public:
  ODRX_DECLARE_MEMBERS(OdDgSelectionSet);

  /** \details
    Creates a SelectionSet object for the specified database. 
    \param pDb [in]  Pointer to the database object for which to create the SelectionSet object.
    \returns
    Returns a SmartPointer to the new SelectionSet object.
  */
  static OdDgSelectionSetPtr createObject(const OdDgDatabase *pDb);
  
  virtual const OdDgDatabase* database() const = 0;

  /** \details
      Returns an Iterator object that provides access to the entities in this Selection Set.
  */
  //virtual OdSelectionSetIteratorPtr newIterator() const = 0;

  /** \details
  */
  virtual OdDgElementIdArray elementIdArray() const;

  // operations on the selection set 

  /** \details
    Filter selection only.  

    \param pDb [in]  Pointer to the database object for which to create the SelectionSet object.
    \param pFilter [in] object of type OdDgSelectionFilter or pointer to a OdResBuf chain defining the filter.
  */
  //static OdDgSelectionSetPtr select(const OdDgDatabase *pDb, const OdRxObject* pFilter = 0);

  /** \details
    Filter selection only.  

    \param vpId [in]  id of OdDgViewportTableRecord  record or OdDgViewport entity in which to select.
    \param nPoints [in] 
    \param wcsPts [in] 
    \param mode [in] 
    \param pFilter [in] object of type OdDgSelectionFilter or pointer to a OdResBuf chain defining the filter.
  */
  static OdDgSelectionSetPtr select(
    const OdDgElementId& vpId,
    int nPoints,
    const OdGePoint3d* wcsPts,
    OdDbVisualSelection::Mode mode = OdDbVisualSelection::kCrossing,
    OdDbVisualSelection::SubentSelectionMode sm = OdDbVisualSelection::kDisableSubents,
    const OdRxObject* pFilter = 0);

  /** \details
    Filter selection only.  

    \param nPoints [in] 
    \param wcsPts [in] 
    \param mode [in] 
    \param pFilter [in] object of type OdDgSelectionFilter or pointer to a OdResBuf chain defining the filter.
  */
  static OdDgSelectionSetPtr select(
    const OdDgDatabase* pDg,
    OdGsView* gsView,
    int nPoints,
    const OdGePoint3d* wcsPts,
    OdDbVisualSelection::Mode mode = OdDbVisualSelection::kCrossing,
    OdDbVisualSelection::SubentSelectionMode sm = OdDbVisualSelection::kDisableSubents,
    const OdRxObject* pFilter = 0);

  /** \details
    Returns the number of entities in this group.
  */
  virtual OdUInt32 numEntities() const = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDgEntity. 
    \param method [in] Selection method to associate with object
  */
  TD_USING(OdSelectionSet::append);
  
  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDgEntity. 
    \param pMethod [in] Selection method to associate with object.
  */
  virtual void append(const OdDgElementId& entityId, OdDbSelectionMethod* pMethod = 0) = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param entityIds [in]  Array of Object IDs of the OdDgEntity's.
    \param pMethod [in] Selection method to associate with objects.
  */
  virtual void append(const OdDgElementIdArray& entityIds, OdDbSelectionMethod* pMethod = 0);

  /** \details
    Appends the specified subentity to this SelectionSet object.  

    \param subent [in]  Subentity path to append. 
    \param pMethod [in] Selection method to associate with objects.
  */
  virtual void append(const OdDgFullSubentPath& subent, OdDbSelectionMethod* pMethod = 0) = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param pSSet [in]  Other selection set to copy entities from. 
  */
  virtual void append(const OdSelectionSet* pSSet);

  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDgEntity. 
  */
  TD_USING(OdSelectionSet::remove);
  
  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDgEntity. 
  */
  virtual void remove(const OdDgElementId& entityId) = 0;

  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param entityIds [in]  Array Object ID of the OdDgEntity. 
  */
  virtual void remove(const OdDgElementIdArray& entityIds);

  /** \details
  Removes the specified subentity from this SelectionSet object.  

  \param subent [in]  Subentity path  to remove. 
  */
  virtual void remove(const OdDgFullSubentPath& subent) = 0;

  /** \details
  Removes the specified entity from this SelectionSet object.  

  \param pSSet [in]  Object ID of the OdDgEntity. 
  */
  virtual void remove(const OdSelectionSet* pSSet);

  /** \details
    Returns true if and only if the specified entity is a member
    of this SelectionSet object.
    \param entityId [in]  Object ID of the OdDgEntity. 
  */
  TD_USING(OdSelectionSet::isMember);
  
  /** \details
    Returns true if and only if the specified entity is a member
    of this SelectionSet object.
    \param entityId [in]  Object ID of the OdDgEntity. 
  */
  virtual bool isMember(const OdDgElementId& entityId) const = 0;

  /** \details
    Returns true if and only if the specified subentity is a member
    of this SelectionSet object.
    \param subent [in]  Subentity path. 
  */
  virtual bool isMember(const OdDgFullSubentPath& subent) const = 0;

  TD_USING(OdSelectionSet::method);
  
  /** \details
    Returns the selection method for the given entity.
    
    \param entityId [in]  Object ID of the OdDgEntity. 
  */
  virtual OdDbSelectionMethodPtr method(const OdDgElementId& entityId) const = 0;

  /** \details
  Returns the number of subentities selected inside the entity with the given id.
  (analogous to acedSSSubentLength)
  \param entityId [in]  entity id for the parent of the subentity

  \returns
  Returns true if and only if not at the end of the list.
  */
  virtual OdUInt32 subentCount(const OdDgElementId& entityId) const;

  /** \details
  Returns the given subentity path inside the entity with the given id.
  (analogous to acedSSSubentName)
  \param entityId [in]  entity id for the parent of the subentity
  \param i [in]  index of the subentity.
  \param path [out]  Path to the subentity.

  \returns
  Returns true if and only if "i" is valid index in the subentities array.
  */
  virtual bool getSubentity(const OdDgElementId& entityId, OdUInt32 i, 
                            OdDgFullSubentPath& path) const;
  
  /** \details
    Returns the selection method for the given subentity.
  */
  virtual OdDbSelectionMethodPtr method(const OdDgFullSubentPath& subent) const;
};

#endif //_DG_SSET_H_
