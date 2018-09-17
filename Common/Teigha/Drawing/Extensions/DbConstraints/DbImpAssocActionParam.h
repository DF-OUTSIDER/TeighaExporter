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

#ifndef DbImpAssocActionParam_INCLUDED_
#define DbImpAssocActionParam_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbGeomRef.h"
//#include "DbEntity.h"

#include "TD_PackPush.h"

/** \details
  This class represents the custom implementation of the associated action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocActionParam
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocActionParam();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocActionParam();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  virtual OdStringArray compareWith(OdDbImpAssocActionParam *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT)  const;
protected:
  /*!DOM*/
  friend class OdDbAssocActionParam;

  /*!DOM*/
  OdString m_sName;
  /*!DOM*/
  OdDbAssocStatus m_status;
};

//////////////////////////////////////////////////////////////////////////

/** \details
  This class represents the custom implementation of the associated single dependency action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocSingleDependencyActionParam : public OdDbImpAssocActionParam
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocSingleDependencyActionParam();
  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocSingleDependencyActionParam();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  virtual OdDbObjectId dependentOnObjectId() const;

  OdResult detachDependencies( OdDbObjectId PerentActionId );

  /** \details
  Gets the compounded object.
  */
  virtual OdResult getObject(OdDbCompoundObjectId* object) const;

  /** \details
  Sets the compounded object.
  */
  virtual OdResult setObject(OdDbObjectId actionId,
    const OdDbCompoundObjectId* object,
    bool isReadDependency = true,
    bool isWriteDependency = false);
  /** \details
  Sets the compounded object. Without creating additional dependency (see OdDbImpAssocSingleDependencyActionParam::setObject)
  */
  OdResult setObjectOnly(OdDbObjectId actionId,
    const OdDbCompoundObjectId* object,
    bool isReadDependency /*= true*/,
    bool isWriteDependency /*= false*/);

  virtual OdStringArray compareWith(OdDbImpAssocActionParam *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT)  const;
protected:
  /*!DOM*/
  friend class OdDbAssocActionParam;
  /*!DOM*/
  OdDbObjectId m_DependencyId;
};

//////////////////////////////////////////////////////////////////////////

/** \details
  This class represents the custom implementation of the associated compounded action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocCompoundActionParam : public OdDbImpAssocActionParam
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocCompoundActionParam();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocCompoundActionParam();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;
  /** \details
  Creates and adds a new parameter.
  */
  OdResult addChildParam(const OdDbObjectId actionId, const OdDbObjectId thisObjectId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex);
  /** \details
  Returns an array of object IDs that refer to all owned action parameters with the given name.
  */
  OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

  /** \details
  Returns an owned action parameter specified by its name and optional index for parameters with the same name.
  */
  OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;

  virtual OdStringArray compareWith(OdDbImpAssocActionParam *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT)  const;
protected:
  /*!DOM*/
  friend class OdDbAssocCompoundActionParam;
  /*!DOM*/
  OdUInt16 m_Unknown1;                          // 0
  OdUInt16 m_Unknown2;                          // 0
  OdUInt16 m_UseAdditionalAnchors;                          // 0
  /*!DOM*/
  OdDbObjectIdArray m_ActionParams;
  bool m_usingInheritor;
  OdDbObjectId m_AnchorId1;
  OdDbObjectId m_AnchorId2;
  OdUInt16 m_Unknown3;
  OdDbObjectId m_AnchorId3;
};

// TODO
// class OdDbImpAssocBlendSurfaceActionBody
// class OdDbImpAssocBoolOperActionBody
// class OdDbImpAssocCloneAndPositionEntityActionBody
// class OdDbImpAssocEdgeFilletChamferActionBody
// class OdDbImpAssocExtendSurfaceActionBody
// class OdDbImpAssocExtrudedSurfaceActionBody
// class OdDbImpAssocFilletSurfaceActionBody
// class OdDbImpAssocImpliedSurfaceOrSolidActionBody
// class OdDbImpAssocLegacySurfaceActionBody
// class OdDbImpAssocLoftedSurfaceActionBody
// class OdDbImpAssocLoftedSurfaceBaseActionBody
// class OdDbImpAssocNetworkSurfaceActionBody
// class OdDbImpAssocOffsetSurfaceActionBody
// class OdDbImpAssocPatchSurfaceActionBody
// class OdDbImpAssocPathBasedSurfaceActionBody
// class OdDbImpAssocPlaneSurfaceActionBody
// class OdDbImpAssocPositionEntityActionBody
// class OdDbImpAssocRevolvedSurfaceActionBody
// class OdDbImpAssocSetObjectPropertyActionBody
// class OdDbImpAssocSurfaceActionBody
// class OdDbImpAssocSweptSurfaceActionBody
// class OdDbImpAssocTrimSurfaceActionBody

//////////////////////////////////////////////////////////////////////////

/** \details
  This class represents the custom implementation of the associated object of single dependency action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocObjectActionParam : public OdDbImpAssocSingleDependencyActionParam
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocObjectActionParam();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocObjectActionParam();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;
protected:
  /*!DOM*/
  friend class OdDbAssocObjectActionParam;
  /*!DOM*/
  bool m_isBase;
};

//////////////////////////////////////////////////////////////////////////

/** \details
  This class represents the custom implementation of the associated vertex of single dependency action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocVertexActionParam : public OdDbImpAssocSingleDependencyActionParam
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocVertexActionParam();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocVertexActionParam();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;


  OdResult getVertexRef(OdArray<OdDbVertexRef>& vertexRefs) const;
  OdResult getVertexRef(OdDbVertexRef& vertexRef) const;

  virtual OdResult setVertexRef( const OdDbVertexRef& vertexRef, bool isReadDependency, bool isWriteDependency, int dependencyOrder, OdDbObjectId actionId);

protected:
  /*!DOM*/
  friend class OdDbAssocVertexActionParam;
  /*!DOM*/
  OdDbVertexRef m_vertexRef;
};

//////////////////////////////////////////////////////////////////////////

/** \details
  This class represents the custom implementation of the associated edge of single dependency action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocEdgeActionParam : public OdDbImpAssocSingleDependencyActionParam
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocEdgeActionParam();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocEdgeActionParam();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  OdResult getEdgeRef(OdArray<OdDbEdgeRef>& edgeRefs) const;

  OdResult setEdgeRef( const OdDbEdgeRef& edgeRef, bool isReadDependency, /* = true */ bool isWriteDependency, /* = false */ int dependencyOrder, OdDbObjectId actionId); // = 0

  OdResult setEdgeSubentityGeometry(const OdGeCurve3d* pNewEdgeCurve);

  OdResult getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const;

  /** \details
  Gets the compounded object.
  */
  virtual OdResult getObject(OdDbCompoundObjectId* object) const;

  /** \details
  Sets the compounded object.
  */
  virtual OdResult setObject(OdDbObjectId actionId,
    const OdDbCompoundObjectId* object,
    bool isReadDependency = true,
    bool isWriteDependency = false);

  virtual OdStringArray compareWith(OdDbImpAssocActionParam *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT)  const;
protected:
  /*!DOM*/
  friend class OdDbAssocEdgeActionParam;
  friend class OdDbAssocViewSymbolActionParam;
  /*!DOM*/
  OdDbEdgeRef m_edgeRef;
  OdDbObjectId m_UnkownId;
};

//////////////////////////////////////////////////////////////////////////

/** \details
This class represents the custom implementation of the associated compounded action parameter.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocPointRefActionParam : public OdDbImpAssocCompoundActionParam
{
public:
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpAssocPointRefActionParam();

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocPointRefActionParam();
};


//////////////////////////////////////////////////////////////////////////

/** \details
This class represents the custom implementation of the associated compounded action parameter.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocOsnapPointRefActionParam : public OdDbImpAssocPointRefActionParam
{
public:
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpAssocOsnapPointRefActionParam();

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocOsnapPointRefActionParam();
  /** \details
  Reads the .dwg file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
  Writes the .dwg file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
  Reads the .dxf file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
  Writes the .dxf file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  void setOsnapMode(OdDb::OsnapMode mode)
  {
    m_OsnapMode = mode;
  }

  void setParam(double param)
  {
    m_NearPointParam = param;
  }

  virtual OdStringArray compareWith(OdDbImpAssocActionParam *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT)  const;

private:
  OdInt32 m_UnknownParam1;                          // 0
  OdDb::OsnapMode m_OsnapMode;                          // 0
  double m_NearPointParam;
};
//////////////////////////////////////////////////////////////////////////

/** \details
This class represents the custom implementation of the associated compounded action parameter.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocPathActionParam : OdDbImpAssocCompoundActionParam
{
public:
	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbImpAssocPathActionParam();

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpAssocPathActionParam();

	/** \details
	Reads the .dwg file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dwg file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

	/** \details
	Reads the .dxf file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dxf file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;
	/** \details
	Creates and adds a new parameter.
	*/
	//OdResult addChildParam(const OdDbObjectId actionId, const OdDbObjectId thisObjectId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex);
	///** \details
	//Returns an array of object IDs that refer to all owned action parameters with the given name.
	//*/
	//OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

	///** \details
	//Returns an owned action parameter specified by its name and optional index for parameters with the same name.
	//*/
	//OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;
  virtual OdStringArray compareWith(OdDbImpAssocActionParam *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT)  const;
protected:
	/*!DOM*/
	friend class OdDbAssocPathActionParam;
	/*!DOM*/
	OdUInt32 m_UnknownPathActionParam1;                          // 0
};


//////////////////////////////////////////////////////////////////////////

/** \details
This class represents the custom implementation of the associated compounded action parameter.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocAsmBodyActionParam : OdDbImpAssocActionParam
{
public:
	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbImpAssocAsmBodyActionParam();
	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpAssocAsmBodyActionParam();

	/** \details
	Reads the .dwg file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dwg file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

	/** \details
	Reads the .dxf file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dxf file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;
	/** \details
	Creates and adds a new parameter.
	*/
	//OdResult addChildParam(const OdDbObjectId actionId, const OdDbObjectId thisObjectId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex);
	///** \details
	//Returns an array of object IDs that refer to all owned action parameters with the given name.
	//*/
	//OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

	///** \details
	//Returns an owned action parameter specified by its name and optional index for parameters with the same name.
	//*/
	//OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;
protected:
	/*!DOM*/
	friend class OdDbAssocAsmBodyActionParam;
private:
	/*!DOM*/
	OdUInt32 m_UnknownPathActionParam1;
	bool m_UnknownBool;
	OdDbImpAssocSingleDependencyActionParam *m_pSingleDependencyActionParam;
};

/** \details
This class represents the custom implementation of the associated compounded action parameter.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocFaceActionParam : public OdDbImpAssocActionParam
{
public:
	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbImpAssocFaceActionParam();

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpAssocFaceActionParam();

	/** \details
	Reads the .dwg file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dwg file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

	/** \details
	Reads the .dxf file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dxf file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;
	/** \details
	Creates and adds a new parameter.
	*/
	//OdResult addChildParam(const OdDbObjectId actionId, const OdDbObjectId thisObjectId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex);
	///** \details
	//Returns an array of object IDs that refer to all owned action parameters with the given name.
	//*/
	//OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

	///** \details
	//Returns an owned action parameter specified by its name and optional index for parameters with the same name.
	//*/
	//OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;
protected:
	/*!DOM*/
	friend class OdDbAssocFaceActionParam;
	/*!DOM*/
private:
	OdUInt32 m_UnknownPathActionParam1;                 
	OdUInt32 m_UnknownPathActionParam2;
	OdDbImpAssocAsmBodyActionParam *m_pAsmBodyActionParam;
};

/** \details
This class represents the custom implementation of the associated compounded action parameter.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocTrimmingBodyActionParam : public OdDbImpAssocFaceActionParam
{
public:
	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbImpAssocTrimmingBodyActionParam();

	/** \details
	Destructor destroys an instance of this class.
	*/
	virtual ~OdDbImpAssocTrimmingBodyActionParam();

	/** \details
	Reads the .dwg file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dwg file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

	/** \details
	Reads the .dxf file data of this object.

	\param pFiler [in]  Filer object from which data are read.

	\remarks
	Returns the filer status.
	*/
	virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

	/** \details
	Writes the .dxf file data of this object.

	\param pFiler [in]  Pointer to the filer to which data are written.
	*/
	virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;
	/** \details
	Creates and adds a new parameter.
	*/
	//OdResult addChildParam(const OdDbObjectId actionId, const OdDbObjectId thisObjectId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex);
	///** \details
	//Returns an array of object IDs that refer to all owned action parameters with the given name.
	//*/
	//OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

	///** \details
	//Returns an owned action parameter specified by its name and optional index for parameters with the same name.
	//*/
	//OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;
protected:
	/*!DOM*/
	friend class OdDbAssocTrimmingBodyActionParam;
	/*!DOM*/
private:
	OdUInt16 m_unknownTrimmingBodyParam1;
	OdInt8 m_unknownTrimmingBodyParam2;
};
#include "TD_PackPop.h"

#endif // DbImpAssocActionParam_INCLUDED_
