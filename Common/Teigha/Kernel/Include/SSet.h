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

#ifndef _SSET_H_
#define _SSET_H_

#include "OdPlatform.h"

#include "Ge/GePoint3d.h"
#include "DbBaseSubentId.h"

/** \details
    This class is the abstract base class for all selection methods.

    <group OdDb_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdDbSelectionMethod : public OdRxObject
{
protected:
  OdDbSelectionMethod();
public:
  ODRX_DECLARE_MEMBERS(OdDbSelectionMethod);
};

typedef OdSmartPtr<OdDbSelectionMethod> OdDbSelectionMethodPtr;

class OdGiViewport;

/** \details

    <group OdDb_Classes>
*/
class FIRSTDLL_EXPORT OdDbVisualSelection : public OdDbSelectionMethod
{
protected:
  OdDbVisualSelection();
public:
  ODRX_DECLARE_MEMBERS(OdDbVisualSelection);

  enum Mode
  {
    kPoint        = 0, // Selection by single point. Returns single object: top for 3D render modes or first for 2D render mode.
    kBox          = 1, // Selection by two points (represents rectangle) and choose between kWindow and kCrossing modes using direction vector between points.
    kWindow       = 2, // Selection by two points (represents rectangle). Select only objects which is entirely inside selection area.
    kCrossing     = 3, // Selection by two points (represents rectangle). Select objects which is entirely inside or intersects selection area.
    kFence        = 4, // Selection by two or more points (represents polyline). Select object which is intersects selection polyline.
    kWPoly        = 5, // Selection by three or more points (represents polygon). Select only objects which is entirely inside selection area.
    kCPoly        = 6  // Selection by three or more points (represents polygon). Select objects which is entirely inside or intersects selection area.
  };

  enum SubentSelectionMode
  {
    kDisableSubents  = 0,
    kEnableSubents   = 1,
    // #CORE-13269 : include ModelSpace viewport into selection path
    kIncludeViewport = 2
  };

  virtual OdDbStub* viewport() const = 0;
  virtual OdGePoint3dArray points() const = 0;
  virtual Mode mode() const = 0;
};

typedef OdSmartPtr<OdDbVisualSelection> OdDbVisualSelectionPtr;


/** \details
  This class implements iterators for OdSelectionSet objects.

  \remarks

  Instances are returned by OdSelectionSet::newIterator().

  <group OdDb_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdSelectionSetIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdSelectionSetIterator);
  
  virtual OdDbSelectionMethodPtr method() const = 0;

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() const = 0;

  /** \details
    Sets this Iterator object to reference the entry following the current entry.

    \returns
    Returns true if and only if not at the end of the list.
  */
  virtual bool next() = 0;

  /** \details
    Returns the number of subentities selected inside the current entity.
    (analogous to acedSSSubentLength)

    \returns
    Returns true if and only if not at the end of the list.
  */
  virtual OdUInt32 subentCount() = 0;

  /** \details
    Returns the given subentity path inside the current entity.
    (analogous to acedSSSubentName)
    \param i [in]  index of the subentity.
    \param path [out]  Path to the subentity.

    \returns
    Returns true if and only if "i" is valid index in the subentities array.
  */
  virtual bool getSubentity(OdUInt32 i, OdDbBaseFullSubentPath& path) = 0;

  /** \details
      Returns the GiDrawable ID at the current position specified by
      this Iterator object.
  */
  virtual OdDbStub* id() const = 0;

protected:
  OdSelectionSetIterator();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdSelectionSetIterator object pointers.
*/
typedef OdSmartPtr<OdSelectionSetIterator>  OdSelectionSetIteratorPtr;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdSelectionSet object pointers.
*/
typedef OdSmartPtr<class OdSelectionSet>  OdSelectionSetPtr;

/** \details
    This class represents Selection Sets in an Teigha application.

    \remarks
    Only entities from an OdDbDatabase object's ModelSpace or PaperSpace may be
    added to a Selection Set.

    <group OdDb_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdSelectionSet : public OdRxObject
{
protected:
  OdSelectionSet();
public:
  ODRX_DECLARE_MEMBERS(OdSelectionSet);

  virtual const OdRxObject* baseDatabase() const = 0;

  /** \details
    Returns an Iterator object that provides access to the entities in this Selection Set.
  */
  virtual OdSelectionSetIteratorPtr newIterator() const = 0;

  // operations on the selection set 

  /** \details
    Returns the number of entities in this group.
  */
  virtual OdUInt32 numEntities() const = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDbEntity. 
    \param method [in] Selection method to associate with object
  */
  virtual void append(OdDbStub* entityId, OdDbSelectionMethod* pMethod = 0) = 0;

  /** \details
    Appends the specified subentity to this SelectionSet object.  

    \param subent [in]  Subentity path to append. 
  */
  virtual void append(const OdDbBaseFullSubentPath& subent, OdDbSelectionMethod* pMethod = 0) = 0;

  /** \details
    Appends the specified entity to this SelectionSet object.  

    \param pSSet [in]  Other selection set to copy entities from. 
  */
  virtual void append(const OdSelectionSet* pSSet) = 0;

  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param entityId [in]  Object ID of the OdDbEntity. 
  */
  virtual void remove(OdDbStub* entityId) = 0;

  /** \details
    Removes the specified subentity from this SelectionSet object.  

    \param subent [in]  Subentity path  to remove. 
  */
  virtual void remove(const OdDbBaseFullSubentPath& subent) = 0;

  /** \details
    Removes the specified entity from this SelectionSet object.  

    \param pSSet [in]  Object ID of the OdDbEntity. 
  */
  virtual void remove(const OdSelectionSet* pSSet) = 0;

  /** \details
    Returns true if and only if the specified entity is a member
    of this SelectionSet object.
    \param entityId [in]  Object ID of the OdDbEntity. 
  */
  virtual bool isMember(OdDbStub* entityId) const = 0;

  /** \details
    Returns true if and only if the specified subentity is a member
    of this SelectionSet object.
    \param entityId [in]  Subentity path. 
  */
  virtual bool isMember(const OdDbBaseFullSubentPath& subent) const = 0;

  virtual OdDbSelectionMethodPtr method(OdDbStub* entityId) const = 0;

  /** \details
    Removes all members of this SelectionSet object.
  */
  virtual void clear() = 0;
};

#endif //_SSET_H_
