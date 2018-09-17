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

#ifndef DbGeomRef_INCLUDED_
#define DbGeomRef_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h" 
#include "DbSubentId.h"
#include "DbCompoundObjectId.h"
#include "Ge/GeCurve3d.h"
#include "RxObjectImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents a reference to a geometry object. It is the base class for references.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbGeomRef : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeomRef);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbGeomRef();

  /** \details
    Resets this geometry reference.
  */
  virtual void reset();

  /** \details
    Checks whether this geometry reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this geometry reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Creates an antity using this geometry reference.
  */
  virtual OdDbEntity* createEntity() const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult  evaluateAndCacheGeometry();
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbGeomRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbGeomRef> OdDbGeomRefPtr;


/** \details
  This class represents a reference to a subent object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbSubentRef : public OdDbGeomRef
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSubentRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbSubentRef();
  OdDbSubentRef(const OdDbCompoundObjectId&, 
                const OdDbSubentId& = OdDbSubentId()); 

  static OdSmartPtr<OdDbSubentRef> createObject( const OdDbCompoundObjectId*,
                const OdDbSubentId& = OdDbSubentId() );

  OdDbSubentRef& operator =(const OdDbSubentRef&);
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this subent reference.
  */
  virtual void reset();

  /** \details
    Checks whether this subent reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this subent reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Returns an object ID of the compounded entity.
  */
  virtual const OdDbCompoundObjectId& entity() const;

  /** \details
    Returns an subent ID.
  */
  virtual OdDbSubentId subentId() const;

  /** \details
    Creates an entity using this subent reference.
  */
  virtual OdDbEntity* createEntity() const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult evaluateAndCacheGeometry(); 

protected:
  /** \details
    Sets the compounded entity using an object ID.
  */
  void setEntity(const OdDbCompoundObjectId& entityId);
 
  /** \details
    Sets an subent ID.
  */
  void setSubent(const OdDbSubentId& subentId);

  /** \details
    Sets an subent ID using the full subent path.
  */
  void setFromSubentPath(const OdDbFullSubentPath&);

private:
  /*!DOM*/
  OdDbCompoundObjectIdPtr m_pEntityId;
  /*!DOM*/
  OdDbSubentId m_SubentId;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbSubentRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSubentRef> OdDbSubentRefPtr;


/** \details
  This class represents a reference to a face object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbFaceRef : public OdRxObjectImpl<OdDbSubentRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbFaceRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbFaceRef();
  OdDbFaceRef(const OdDbCompoundObjectId&, 
              const OdDbSubentId& = OdDbSubentId(), 
              class BODY* = NULL); 
  OdDbFaceRef(const OdDbFaceRef&);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbFaceRef();

  /** \details
    Copies a given fase reference to this reference.
  */
  OdDbFaceRef& operator =(const OdDbFaceRef&);

  /** \details
    Initializes this reference using a given object.
  */
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this fase reference.
  */
  virtual void reset();

  /** \details
    Checks whether this face reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this face reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Returns the ASM BODY object which this face reference may hold.
  */
  class BODY* asmBody() const;

  /** \details
    Creates an entity using this face reference.
  */
  virtual OdDbEntity* createEntity() const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  class BODY* m_pAsmBody;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbFaceRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbFaceRef> OdDbFaceRefPtr;


/** \details
  This class represents a reference to an endge object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbEdgeRef : public OdRxObjectImpl<OdDbSubentRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbEdgeRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbEdgeRef();
  OdDbEdgeRef(const OdDbFullSubentPath&);
  OdDbEdgeRef(const OdDbCompoundObjectId&,
              const OdDbSubentId& edgeSubentId = OdDbSubentId(),
              const OdDbSubentId& faceSubentId = OdDbSubentId(),
              const OdGeCurve3d* pCurve = NULL);
  OdDbEdgeRef(const OdDbEntity* pEntity);  
  OdDbEdgeRef(const OdGeCurve3d* pGeCurve);
  OdDbEdgeRef(const OdDbEdgeRef&);

  static OdSmartPtr<OdDbEdgeRef> createObject( const OdDbCompoundObjectIdPtr &, const OdGeCurve3d* pCurve );
  static OdSmartPtr<OdDbEdgeRef> createObject( const OdGeCurve3d* pCurve );

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbEdgeRef();

  /** \details
    Copies a given edge reference to this reference.
  */
  OdDbEdgeRef& operator =(const OdDbEdgeRef&);

  /** \details
    Initializes this reference using a given object.
  */
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this edge reference.
  */
  virtual void reset();

  /** \details
    Checks whether this edge reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this edge reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    The edge reference can use the subent ID of a face subentity. The face entity is used to restrict
    the referenced edge subentity.
  */
  void setFaceSubentity(const OdDbSubentId& faceSubentId);

  /** \details
    Returns the face subent ID of one of the faces adjacent to the referenced edge.
  */
  OdDbSubentId faceSubentId() const;

  /** \details
    Returns the constant three-dimensional curve which the edge reference can hold.
  */
  const OdGeCurve3d* curve() const;

  /** \details
    Evaluates the curve.
  */
  virtual OdResult evaluateCurve(OdGeCurve3d*&) const;

  /** \details
    Creates an entity using this edge reference.
  */
  virtual OdDbEntity* createEntity() const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult evaluateAndCacheGeometry(); 

  
  /** \details
  Sets the constant three-dimensional curve to be hold by this edge reference.
  */
  void setCurve(const OdGeCurve3d* pCurve);

private:
  ///** \details
  //  Sets the constant three-dimensional curve to be hold by this edge reference.
  //*/
  //void setCurve(const OdGeCurve3d* pCurve);
  /*!DOM*/
  OdDbSubentId m_FaceSubentId;
  /*!DOM*/
  OdGeCurve3dPtr m_pCurve;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbEdgeRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbEdgeRef> OdDbEdgeRefPtr;


/** \details
  This class represents a reference to a vertex object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbVertexRef : public OdRxObjectImpl<OdDbSubentRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbVertexRef);

  /** \details
    Declares the implied types.
  */
  enum ImpliedType
  {
    kExplicitVertex, // directly specified vertex
    kUnknownType,
    kEdgeStart,
    kEdgeEnd,
    kEdgeMid,
    kEdgeCenter,
    kEdgeSplineControlPoint,
    kEdgeSplineFitPoint,
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbVertexRef();
  OdDbVertexRef(const OdDbCompoundObjectId&, 
                const OdDbSubentId& = OdDbSubentId(), 
                const OdGePoint3d& = OdGePoint3d::kOrigin);
  OdDbVertexRef(const OdDbFullSubentPath&);
  OdDbVertexRef(const OdDbEntity*);
  OdDbVertexRef(const OdGePoint3d&);
  OdDbVertexRef(ImpliedType, 
                const OdDbEdgeRef& edgeRef, 
                int controlOrFitPointIndex = 0,
                const OdGePoint3d& = OdGePoint3d::kOrigin);
  OdDbVertexRef(const OdDbVertexRef&);

  static OdSmartPtr<OdDbVertexRef> createObject( const OdGePoint3d& );
  static OdSmartPtr<OdDbVertexRef> createObject( const OdDbObjectId entId, const OdDbSubentId subId, const OdGePoint3d& point );

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbVertexRef();

  /** \details
    Copies a given vertex reference to this reference.
  */
  OdDbVertexRef& operator =(const OdDbVertexRef&);

  /** \details
    Initializes this reference using a given object.
  */
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this vertex reference.
  */
  virtual void reset();

  /** \details
    Checks whether this vertex reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this vertex reference is empty.
  */
  virtual const OdDbCompoundObjectId& entity() const;

  /** \details
    Returns the subent ID.
  */
  virtual OdDbSubentId subentId() const;

  /** \details
    Returns the implied type.
  */
  ImpliedType impliedType() const;

  /** \details
    Returns the geometry reference referenced by this vertex reference.
  */
  const OdDbGeomRef* referencedRef() const;

  /** \details
    Returns the vertex index.
  */
  int index() const;

  /** \details
    Returns the three-dimensional point directly held by this vertex reference.
  */
  OdGePoint3d point() const;

  /** \details
    Evaluate the point.
  */
  virtual OdResult evaluatePoint(OdGePoint3d&) const;

  /** \details
    Creates an entity using this vertex reference.
  */
  class OdDbEntity* createEntity()const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult evaluateAndCacheGeometry(); 

private:
  /** \details
    Sets the geometry reference to be hold by this vertex reference.
  */
  void setReferencedRef(OdDbGeomRefPtr pReferencedRef);

  /*!DOM*/
  ImpliedType m_ImpliedType;
  /*!DOM*/
  OdDbGeomRefPtr m_pReferencedRef;
  /*!DOM*/
  int m_Index;
  /*!DOM*/
  OdGePoint3d m_Point; 
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbVertexRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbVertexRef> OdDbVertexRefPtr;


/** \details
  This class represents a reference to a path object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbPathRef : public OdRxObjectImpl<OdDbGeomRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPathRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbPathRef();
  OdDbPathRef(const OdArray<OdDbEdgeRef>&);
  OdDbPathRef(const OdDbFullSubentPathArray& edgeSubentPathArr,
              const OdDbFullSubentPathArray& faceSubentPathArr);
  OdDbPathRef(const OdDbPathRef&);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbPathRef();

  /** \details
    Copies a given path reference to this reference.
  */
  OdDbPathRef& operator =(const OdDbPathRef&);

  /** \details
    Initializes this reference using a given object.
  */
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this path reference.
  */
  virtual void reset();

  /** \details
    Checks whether this path reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this path reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Sets the path reference from a sequence of edge references.
  */
  void setEdgeRefs(const OdArray<OdDbEdgeRef>& edges);

  /** \details
    Returns an array of entities created from the edge references in the path. 
    If the second argument is True, it tries to concatenate them.
  */
  OdResult getEntityArray(OdArray<OdDbEntity*>& entities, bool concatenate) const;

  /** \details
    Returns the array of edge references stored in the path reference.
  */
  const OdArray<OdDbEdgeRef>& edgeRefs() const;

  /** \details
    Checks whether the geometry of the path is equal to the geometry of a given curve.
  */
  bool isEqualTo(const OdGeCurve3d*);

  /** \details
    Checks whether all elements of this path reference are the references to the entities.
  */
  bool isReferencePath();

  /** \details
    Creates an entity using this path reference.
  */
  virtual OdDbEntity* createEntity() const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  OdArray<OdDbEdgeRef> m_EdgeRefs;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbPathRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbPathRef> OdDbPathRefPtr;


/** \details
  This class represents a reference to a vector object.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbVectorRef : public OdRxObjectImpl<OdDbGeomRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbVectorRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbVectorRef();
  OdDbVectorRef(const OdGeVector3d&);
  OdDbVectorRef(const OdDbVectorRef&);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbVectorRef();

  /** \details
    Copies a given vector reference to this reference.
  */
  OdDbVectorRef& operator =(const OdDbVectorRef&);

  /** \details
    Initializes this reference using a given object.
  */
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this vector reference.
  */
  virtual void reset();

  /** \details
    Checks whether this vector reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this vector reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Returns a three-dimensional vector stored by this vector reference.
  */
  const OdGeVector3d vector() const;

  /** \details
    Sets a three-dimensional vector for this vector reference.
  */
  void set(const OdGeVector3d&);

  /** \details
    Creates an entity using this vector reference.
  */
  virtual OdDbEntity* createEntity() const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  OdGeVector3d m_Vector;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbVectorRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbVectorRef> OdDbVectorRefPtr;


/** \details
  This class represents a reference to a coordinate system.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbCoordSystemRef : public OdRxObjectImpl<OdDbGeomRef>
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCoordSystemRef);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbCoordSystemRef();
  OdDbCoordSystemRef(const OdGeMatrix3d&);
  OdDbCoordSystemRef(const OdDbCompoundObjectId&, const OdGeMatrix3d&); 
  OdDbCoordSystemRef(const OdArray<OdDbSubentRef*>&);
  OdDbCoordSystemRef(const OdDbCoordSystemRef&);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbCoordSystemRef();

  /** \details
    Copies a given coordinate system reference to this reference.
  */
  OdDbCoordSystemRef& operator = (const OdDbCoordSystemRef&);

  /** \details
    Initializes this reference using a given object.
  */
  virtual void copyFrom(const OdRxObject*);

  /** \details
    Resets this coordinate system reference.
  */
  virtual void reset();

  /** \details
    Checks whether this coordinate system reference is valid.
  */
  virtual bool isValid() const;

  /** \details
    Checks whether this coordinate system reference is empty.
  */
  virtual bool isEmpty() const;

  /** \details
    Returns the transformation matrix of the coordinate system held by this reference.
  */
  const OdGeMatrix3d& coordSystem() const;

  /** \details
    Returns an entity ID held by this reference.
  */
  const OdDbCompoundObjectId& entityId() const;

  /** \details
    Returns an array of subent references held by this reference.
  */
  const OdArray<OdDbSubentRef*>& subentRefs() const;

  /** \details
    Sets this reference using a transformation matrix.
  */
  void set(const OdGeMatrix3d&);
  
  /** \details
    Sets this reference using an entity ID.
  */
  void set(const OdDbCompoundObjectId&);
  
  /** \details
    Sets this reference using an array of subent references held.
  */
  void set(const OdArray<OdDbSubentRef*>&);

  /** \details
    Creates an entity using this coordinate system reference.
  */
  virtual OdDbEntity* createEntity() const;

  /** \details
    Evaluates and caches the geometry.
  */
  virtual OdResult evaluateAndCacheGeometry();

private:
  /*!DOM*/
  OdGeMatrix3d m_CoordSystem;
  /*!DOM*/
  OdDbCompoundObjectIdPtr m_pEntityId;
  /*!DOM*/
  OdArray<OdDbSubentRef*> m_SubentRefs;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbCoordSystemRef class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbCoordSystemRef> OdDbCoordSystemRefPtr;

#include "TD_PackPop.h"

#endif // DbGeomRef_INCLUDED_
