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




#ifndef OD_DBDIMASSOC_H
#define OD_DBDIMASSOC_H

#include "TD_PackPush.h"

#include "DbSubentId.h"
#include "DbHandle.h"
#include "DbObject.h"

class OdDbDwgFiler;
class OdDbDxfFiler;
class OdGePoint3d;
class OdDbIdMapping;


/** \details
    This class uniquely defines Xref subentities within an OdDbDatabase instance.  

    \remarks
    Each OdDbXrefFullSubentPath object consists of a OdDbSubentId object,
    an ordered array of Object IDs, and an ordered array of entity handles. 
    The SubentId consists the index and subentType
    of the object. The array of Object IDs, and the array of handles, define the path to the 
    subentity from the outermost entity (in PaperSpace or ModelSpace) 
    to the entity containing the subentity.
    
    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbXrefFullSubentPath : public OdDbFullSubentPath
{
public:
  /** \details
    Returns a reference to the embedded OdHandleArray object in this XrefFullSubentPath object.
    
    \param objectIds [out]  Receives the array reference.
  */
  OdHandleArray& xrefObjHandles();
  const OdHandleArray& xrefObjHandles()const;

  /** \details
    Reads the .dwg file format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
  */
  void dwgIn(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    
  */
  void dwgOut(OdDbDwgFiler* pFiler) const;

  /** \details
    Writes the DXF format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
    \param groupCodeOffset [in]  Group code offset.
  */
  void dxfOut(OdDbDxfFiler* pFiler, OdInt groupCodeOffset = 0) const;
private:
  OdHandleArray m_XrefObjHandles;
};

class OdDbOsnapPointRef;
class OdDbViewport;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbOsnapPointRef object pointers.
*/
typedef OdSmartPtr<OdDbOsnapPointRef> OdDbOsnapPointRefPtr;
//typedef OdArray<OdDbFullSubentPath> OdDbFullSubentPathArray;

/** \details
    This class defines a point reference between a dimension and
	its corresponding object.

    Corresponding C++ library: TD_Db
    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbPointRef : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbPointRef);

  virtual OdResult evalPoint(OdGePoint3d& pnt_wcs) = 0;
  virtual OdResult getEntities(OdDbFullSubentPathArray& ents, bool getLastPtRef = true) const = 0;
  virtual bool isGeomErased() const = 0;
  virtual bool isXrefObj(OdDbObjectIdArray& ids1, OdDbObjectIdArray& ids2, bool isMainObj = true) const = 0;
  virtual OdResult updateXrefSubentPath() = 0;
  virtual OdResult updateSubentPath(OdDbIdMapping& idMap) = 0;

  virtual void dwgOutFields(OdDbDwgFiler* filer) const = 0;
  virtual void dwgInFields(OdDbDwgFiler* filer) = 0;
  virtual void dxfOutFields(OdDbDxfFiler* filer) const = 0;
  virtual OdResult dxfInFields(OdDbDxfFiler* filer) = 0;

  virtual void updateDueToMirror(bool inMirror = false) = 0;

/** \details
  Calculates transformation combined from OdDbViewport's MS2PS and BlockReference transformations
*/
  static bool calcTransform(const OdDbObjectIdArray& ids, OdGeMatrix3d& A_Ecs2Wcs);

  /** \details
  Returns matrix to convert MS coordinates to PS for given OdDbViewport entity
  */
  static OdGeMatrix3d mswcsToPswcs(const OdDbViewport* pVPort);
/*
  static OdResult  dwgInFields(OdDbDwgFiler* filer, OdDbDatabase* pDb, OdDbPointRef*& pPointRef);
  static OdResult  dwgOutFields(OdDbDwgFiler* filer, OdDbDatabase* pDb, const OdDbPointRef* pPointRef);
  static OdResult  dxfInFields(OdDbDxfFiler* filer, OdDbDatabase* pDb, OdDbPointRef*& pPointRef);
  static OdResult  dxfOutFields(OdDbDxfFiler* filer, OdDbDatabase* pDb, const OdDbPointRef* pPointRef);
  static OdDbEntityPtr subentPtr(const OdDbFullSubentPath& objPath, bool& isCloned, bool& reverseArc, bool& isStandAloneArc);
  static OdGeMatrix3d wcsToDcsMatrix(const OdDbViewport* pVPort);
*/
};

/** \details

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbOsnapPointRef : public OdDbPointRef
{
public:
  OdDbOsnapPointRef();

  ODRX_DECLARE_MEMBERS(OdDbOsnapPointRef);

  /** \details
    Returns the object snap mode for this OsnapPointRef object.
    \remarks
    osnapMode() returns one of the following:
    
    <table>
    Name                      Value   Description 
    OdDb::kOsModeEnd          1       End Point
    OdDb::kOsModeMid          2       Mid Point
    OdDb::kOsModeCen          3       Center
    OdDb::kOsModeNode         4       Node
    OdDb::kOsModeQuad         5       Quadrant
    OdDb::kOsModeIntersec     6       Intersection
    OdDb::kOsModeIns          7       Insertion point
    OdDb::kOsModePerp         8       Perpendicular
    OdDb::kOsModeTan          9       Tangent
    OdDb::kOsModeNear         10      Nearest
    OdDb::kOsModeApint        11      Apparent intersection
    OdDb::kOsModePar          12      Parallel
    OdDb::kOsModeStart        13      Start Point 
    </table>
 
  */
  OdDb::OsnapMode osnapType() const;
  /** \details
    Sets the object snap mode for this OsnapPointRef object.
 
    \remarks
    osnapMode must be one of the following:
    
    <table>
    Name                      Value   Description 
    OdDb::kOsModeEnd          1       End Point
    OdDb::kOsModeMid          2       Mid Point
    OdDb::kOsModeCen          3       Center
    OdDb::kOsModeNode         4       Node
    OdDb::kOsModeQuad         5       Quadrant
    OdDb::kOsModeIntersec     6       Intersection
    OdDb::kOsModeIns          7       Insertion point
    OdDb::kOsModePerp         8       Perpendicular
    OdDb::kOsModeTan          9       Tangent
    OdDb::kOsModeNear         10      Nearest
    OdDb::kOsModeApint        11      Apparent intersection
    OdDb::kOsModePar          12      Parallel
    OdDb::kOsModeStart        13      Start Point
    </table>
  */
  void setOsnapType(OdDb::OsnapMode osnapMode);

  /** \details
    Returns a reference to the OdDbXrefFullSubentPath of the main entity for the OsnapPointRef object. 
  */
  OdDbXrefFullSubentPath& mainEntity();
  /** \details
    Returns a reference to the OdDbXrefFullSubentPath of the intersecting entity for the OsnapPointRef object. 
  */
  OdDbXrefFullSubentPath& intersectEntity();

  void getIdPath(OdDbFullSubentPath& idPath) const;
  void setIdPath(const OdDbFullSubentPath& idPath);

  void getIntIdPath(OdDbFullSubentPath& intIdPath) const;
  void setIntIdPath(const OdDbFullSubentPath& intIdPath);

  /** \details
    Returns the near Osnap value for this OsnapPointRef object.
  */
  double nearPointParam() const;

  /** \details
    Returns the near Osnap value for this OsnapPointRef object.
    \param nearOsnap [in]  Near Osnap value.
  */
  void setNearPointParam(double nearOsnap);

  /** \details
    Returns a reference to the Osnap point for this OsnapPointRef object.
  */
  OdGePoint3d point() const;
  void setPoint(const OdGePoint3d& pt);

  /** \details
    Returns the last OsnapPointRef for this OsnapPointRef object.
  */
  const OdDbOsnapPointRef* lastPointRef() const;
  
  /** \details
    Returns the last OsnapPointRef for this OsnapPointRef object.
  */
  OdDbOsnapPointRef* lastPointRef();

  /** \details
    Sets the last OsnapPointRef for this OsnapPointRef object.
    \param pOsnapPointRef [in]  Pointer to the last OsnapPointRef object.
  */
  void setLastPointRef(OdDbOsnapPointRefPtr pOsnapPointRef);

  virtual bool isGeomErased() const;
  virtual bool isXrefObj(OdDbObjectIdArray& ids1, OdDbObjectIdArray& ids2, bool isMainObj = true) const;
  virtual OdResult updateXrefSubentPath();
  virtual OdResult updateSubentPath(OdDbIdMapping& idMap);
  virtual OdResult evalPoint(OdGePoint3d &pt);
  virtual OdResult getEntities(OdDbFullSubentPathArray& ents, bool getLastPtRef = true) const;
  virtual void updateDueToMirror(bool inMirror = false);

  void getXrefHandles(OdHandleArray& xrefHandles) const;
  void setXrefHandles(const OdHandleArray& xrefHandles);
  void getXrefIntHandles(OdHandleArray& xrefHandles) const;
  void setXrefIntHandles(const OdHandleArray& xrefHandles);

  /** \details
    Reads the .dwg file format data of this object from the specified file.
       
    \param pFiler [in]  Pointer to the filer from which the data are to be read.
  */
  void dwgInFields(OdDbDwgFiler* pFiler);
  /** \details
    Writes the .dwg file format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
  */
  void dwgOutFields(OdDbDwgFiler* pFiler) const;
  /** \details
    Writes the DXF format data of this object to the specified filer. 
    
    \param pFiler [in]  Pointer to the filer to which the data are to be written.
  */
  void dxfOutFields(OdDbDxfFiler* pFiler) const;
  OdResult dxfInFields(OdDbDxfFiler* filer);
private:
  OdDb::OsnapMode        m_OsnapMode;
  OdDbXrefFullSubentPath m_MainEntity;
  OdDbXrefFullSubentPath m_IntersectEntity;
  double                 m_dNearOsnap;
  OdGePoint3d            m_OsnapPoint;
  OdDbOsnapPointRefPtr   m_pLastPointRef;
  bool                   m_bUpdateDueToMirror;
};

/** \details
    This class represents Associative Dimension objects in an OdDbDatabase instance.

    Corresponding C++ library: TD_Db
    
    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbDimAssoc : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDimAssoc);

  OdDbDimAssoc();

  enum { kMaxPointRefs = 4 };
  
  enum PointType 
  {
    kXline1Point          = 0,
    kXline2Point          = 1,
    kOriginPoint          = 0,
    kDefiningPoint        = 1,
    kXline1Start          = 0,
    kXline1End            = 1,
    kXline2Start          = 2,
    kXline2End            = 3,
    kVertexPoint          = 2,
    kChordPoint           = 0,
    kCenterPoint          = 1,
    kFarChordPoint        = 1,
    kOverrideCenterPoint  = 2,
    kAngLineStart         = 2,
    kJogPoint             = 3,
    kAngLineEnd           = 3,
    kLeaderPoint          = 0
  };

  enum RotatedDimType
  {
    kUnknown       = 0,
    kParallel      = 1,
    kPerpendicular = 2
  };

  enum AssocFlags 
  {
    kFirstPointRef     = (1<<0),
    kSecondPointRef    = (1<<1),
    kThirdPointRef     = (1<<2),
    kFourthPointRef    = (1<<3)
  };

  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler) ODRX_OVERRIDE;
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler) ODRX_OVERRIDE;
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;

  /** \details
    Returns the Object ID of the Dimension entity associated with this Associative Dimension object (DXF 330).
  */
  OdDbObjectId dimObjId() const;

  /** \details
    Sets the Object ID of the Dimension entity associated with this Associative Dimension object (DXF 330).
    \param dimId [in]  Dimension Object ID.
  */
  void setDimObjId(const OdDbObjectId& dimId);

  /** \details
    Returns the associativity flag of this Associative Dimension object (DXF 90).
  */
  OdInt32 assocFlag() const;

  /** \details
    Returns the associativity flag of the specified Osnap Point Reference for this Associative Dimension object.
  */
  bool assocFlag(int pointIndex) const;

  void setAssocFlag(int assocFlag);
  // Sets the index of point reference being used and establishes or clears the association flag. 
  void setAssocFlag(int pointIndex, bool flagValue);

  /** \details
    Returns the specified Osnap Point Reference for this Associative Dimension object.
    
    \param pointIndex [in]  Point index. The value should be 0, 1, 2, 3 or from the PointType enum 
    
    \remarks
    pointIndex must be one of the following:
   
  */
  OdDbOsnapPointRefPtr pointRef(int pointIndex) const;

  /** \details
    Sets the specified Osnap Point Reference for this Associative Dimension object.
    
    \param pointIndex [in]  Point index. The value should be 0, 1, 2, 3 or from the PointType enum 
    \param pOsnapPointRef [in]  SmartPointer to the Osnap Point Reference.
  */
  void setPointRef(int pointIndex, OdDbOsnapPointRefPtr pOsnapPointRef);

  /** \details
  Returns the rotated dimension type of this Associative Dimension object (DXF 71).

  \remarks
  rotatedDimType() returns one of the following:

  <table>
  Name              Value
  kParallel         1
  kPerpendicular    2
  </table>
  */
  RotatedDimType rotatedDimType() const;

  /** \details
  Sets the rotated dimension type of this Associative Dimension object (DXF 71).

  \remarks
  rotatedDimType must be one of the following:

  <table>
  Name              Value
  kParallel         1
  kPerpendicular    2
  </table>
  */
  void setRotatedDimType(RotatedDimType dimType);

  void addToPointRefReactor();
  void addToDimensionReactor(bool isAdd = true);

  OdResult removePointRef(int ptType);
  OdResult updateDimension(bool update = true, bool skipReactors = false);
  void removeAssociativity(bool force = true);
  
  /** \details
  Returns the trans-space flag of this Associative Dimension object (DXF 70).
  */
  bool isTransSpatial() const;
  /** \details
  Sets the trans-space flag of this Associative Dimension object (DXF 70).
  \param transSpace [out]  Trans-space flag.
  */
  void setTransSpatial(bool value);
  

  OdResult post(OdDbObjectId dimId, OdDbObjectId& dimAssocId, bool isActive = true);
  OdResult getDimAssocGeomIds(OdDbObjectIdArray& geomIds) const;
  bool isAllGeomErased() const;
  
  virtual void modifiedGraphics(const OdDbObject* pObj) ODRX_OVERRIDE;
  virtual void copied(const OdDbObject* pObj, const OdDbObject* pNewObj) ODRX_OVERRIDE;
  virtual void erased(const OdDbObject *pObj, bool erasing = true) ODRX_OVERRIDE;
  virtual void openedForModify(const OdDbObject* pObject) ODRX_OVERRIDE;

  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& idMap, OdDbObject* pOwner, bool bPrimary) const ODRX_OVERRIDE;
  virtual OdDbObjectPtr subDeepClone(OdDbIdMapping& idMap, OdDbObject* pOwner, bool bPrimary) const ODRX_OVERRIDE;

protected:
  OdResult setPointRefReactor(int ptType, bool isAdd = true);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbDimAssoc object pointers.
*/
typedef OdSmartPtr<OdDbDimAssoc> OdDbDimAssocPtr;

//
// Inlines
//

inline
OdHandleArray& OdDbXrefFullSubentPath::xrefObjHandles()
{ 
  return m_XrefObjHandles; 
}

inline
const OdHandleArray& OdDbXrefFullSubentPath::xrefObjHandles() const
{ 
  return m_XrefObjHandles; 
}

inline
OdDb::OsnapMode OdDbOsnapPointRef::osnapType() const
{
  return m_OsnapMode;
}

inline
void OdDbOsnapPointRef::setOsnapType(OdDb::OsnapMode osnapMode)
{
  m_OsnapMode = osnapMode;
}

inline
OdDbXrefFullSubentPath& OdDbOsnapPointRef::mainEntity()
{
  return m_MainEntity;
}

inline
OdDbXrefFullSubentPath& OdDbOsnapPointRef::intersectEntity()
{
  return m_IntersectEntity;
}

inline
double OdDbOsnapPointRef::nearPointParam() const
{
  return m_dNearOsnap;
}

inline
void OdDbOsnapPointRef::setNearPointParam(double nearOsnap)
{
  m_dNearOsnap = nearOsnap;
}

inline
OdGePoint3d OdDbOsnapPointRef::point() const
{
  return m_OsnapPoint;
}
inline
void OdDbOsnapPointRef::setPoint(const OdGePoint3d& pt)
{
  m_OsnapPoint = pt;
}

inline
const OdDbOsnapPointRef* OdDbOsnapPointRef::lastPointRef() const
{
  return m_pLastPointRef;
}
inline
OdDbOsnapPointRef* OdDbOsnapPointRef::lastPointRef()
{
  return m_pLastPointRef;
}

inline
void OdDbOsnapPointRef::setLastPointRef(OdDbOsnapPointRefPtr pOsnapPointRef)
{
  m_pLastPointRef = pOsnapPointRef;
}

#include "TD_PackPop.h"

#endif // OD_DBDIMASSOC_H

