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

#ifndef DbAssocPersSubentId_INCLUDED_
#define DbAssocPersSubentId_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbSubentId.h"
#include "SmartPtr.h"

class OdDbEntity;
class OdDbDwgFiler;
class OdDbDxfFiler;
class OdDbAuditInfo;

class OdDbAssocPersSubentId;
typedef OdSmartPtr<OdDbAssocPersSubentId> OdDbAssocPersSubentIdPtr;

class OdDbAssocSimplePersSubentId;
typedef OdSmartPtr<OdDbAssocSimplePersSubentId> OdDbAssocSimplePersSubentIdPtr;

class OdDbAssocSingleEdgePersSubentId;
typedef OdSmartPtr<OdDbAssocSingleEdgePersSubentId> OdDbAssocSingleEdgePersSubentIdPtr;

class OdDbAssocIndexPersSubentId;
typedef OdSmartPtr<OdDbAssocIndexPersSubentId> OdDbAssocIndexPersSubentIdPtr;

class OdDbAssocAsmBasedEntityPersSubentId;
typedef OdSmartPtr<OdDbAssocAsmBasedEntityPersSubentId> OdDbAssocAsmBasedEntityPersSubentIdPtr;

#include "TD_PackPush.h"

/** \details
  This is the abstract base class that represents the persistent subentity identifier.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocPersSubentId : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocPersSubentId);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocPersSubentId() {}

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocPersSubentId() {}

  /** \details
    Gets all subentity identifiers corresponding to this associated persistent subentity identifier.
  */
  virtual OdResult getTransientSubentIds(const OdDbEntity*      pEntity, 
                                         OdArray<OdDbSubentId>& subents) const;

  /** \details
    Declares the pure method that returns the subentity type and must be overridden in derived classes.
  */
  virtual OdDb::SubentType subentType(const OdDbEntity* pEntity) const = 0;

  /** \details
    Returns the number of transient subentity identifiers corresponding to this associated 
    persistent subentity identifier.
  */
  virtual int transientSubentCount(const OdDbEntity* pEntity) const;

  /** \details
    Declares the pure method that checks whether this identifier is kNull and must be overridden in derived classes.
  */
  virtual bool isNull() const = 0;

  /** \details
    Checks whether this identifier is equal to the given identifier.
  */
  virtual bool isEqualTo(const OdDbEntity* pEntity, const OdDbAssocPersSubentId* pOther) const
  {
    return (this == pOther) || ((pEntity != NULL) && (pOther != NULL) && (isA() == pOther->isA()));
  }

  /** \details
    Notifies this associated persistent subentity identifier that the entity has been mirrored and 
    that the associated persistent subentity identifier possibly should be updated itself to identify 
    the same subentity in the mirrored entity.
  */
  virtual OdResult mirror(const OdDbEntity* pMirroredEntity);

  /** \details
    Reads the class identification of the actual associated persistent subentity identifier of derived class, 
    creates an instance for this derived class and calls dwgInFields() on it.
  */
  static OdResult createObjectAndDwgInFields(OdDbDwgFiler* pFiler, OdDbAssocPersSubentIdPtr& pCreatedPersSubentId);

  /** \details
    Reads the class identification of the actual associated persistent subentity identifier of derived class, 
    creates an instance for this derived class and calls dxfInFields() on it.
  */
  static OdResult createObjectAndDxfInFields(OdDbDxfFiler* pFiler, OdDbAssocPersSubentIdPtr& pCreatedPersSubentId);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Provides the standard audit protocol.
  */
  virtual void audit(OdDbAuditInfo* /*pAuditInfo*/) {}
};

//----------------------------------------------------------------------------

/** \details
  This class represents the single edge persistent subentity identifier.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocSingleEdgePersSubentId : public OdDbAssocPersSubentId
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocSingleEdgePersSubentId);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocSingleEdgePersSubentId();

  /** \details
    Returns the number of transient subentity identifiers corresponding to this associated 
    persistent subentity identifier for the given entity.
  */
  virtual int transientSubentCount(const OdDbEntity* /*pEntity*/) const { return 1; }

  /** \details
    Returns the subentity type for the given entity.
  */
  virtual OdDb::SubentType subentType(const OdDbEntity* /*pEntity*/) const { return OdDb::kEdgeSubentType; }

  /** \details
    Checks whether this identifier is kNull (always False).
  */
  virtual bool isNull() const { return false; }

  /** \details
    Checks whether this identifier is equal to the given identifier.
  */
  virtual bool isEqualTo(const OdDbEntity* pEntity, const OdDbAssocPersSubentId* pOther) const
  {
    return OdDbAssocPersSubentId::isEqualTo(pEntity, pOther);
  }

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

protected:
  /*!DOM*/
  bool m_Unk_Bool_1;
};

//----------------------------------------------------------------------------

/** \details
  This class represents the simple persistent subentity identifier.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocSimplePersSubentId : public OdDbAssocPersSubentId
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocSimplePersSubentId);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocSimplePersSubentId();

  /** \details
    Initializes this instance using the specified ID instance.
  */
  void init(const OdDbSubentId& subentId);

  /** \details
    Returns the number of transient subentity identifiers corresponding to this associated 
    persistent subentity identifier for the given entity.
  */
  virtual int transientSubentCount(const OdDbEntity* /*pEntity*/) const { return !isNull() ? 1 : 0; }

  /** \details
    Returns the subentity type for the given entity.
  */
  virtual OdDb::SubentType subentType(const OdDbEntity* /*pEntity*/) const { return m_SubentId.type(); }

  /** \details
    Returns the subentity ID for the given entity.
  */
  virtual OdDbSubentId subentId(const OdDbEntity* /*pEntity*/) const { return m_SubentId; }

  /** \details
    Checks whether this identifier is kNull.
  */
  virtual bool isNull() const
  {
    return m_SubentId.type() == OdDb::kNullSubentType;
  }

  /** \details
    Checks whether this identifier is equal to the given identifier.
  */
  virtual bool isEqualTo(const OdDbEntity* pEntity, const OdDbAssocPersSubentId* pOther) const
  {
    if (!OdDbAssocPersSubentId::isEqualTo(pEntity, pOther))
      return false;

    OdDbAssocSimplePersSubentIdPtr pOtherSimple = pOther;
    return m_SubentId == pOtherSimple->m_SubentId;
  }

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Provides the standard audit protocol.
  */
  virtual void audit(OdDbAuditInfo* pAuditInfo);

protected:
  /*!DOM*/
  bool m_Unk_Bool_1;
private:
  /*!DOM*/
  OdDbSubentId m_SubentId;
};

//----------------------------------------------------------------------------

/** \details
  This class represents the index persistent subentity identifier.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocIndexPersSubentId : public OdDbAssocPersSubentId
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocIndexPersSubentId);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocIndexPersSubentId();

  /** \details
    Initializes this instance using the specified ID instance.
  */
  void init(const OdDbSubentId& subentId);

  /** \details
    Returns the number of transient subentity identifiers corresponding to this associated 
    persistent subentity identifier for the given entity.
  */
  virtual int transientSubentCount(const OdDbEntity* /*pEntity*/) const { return !isNull() ? 1 : 0; }

  /** \details
    Returns the subentity type for the given entity.
  */
  virtual OdDb::SubentType subentType(const OdDbEntity* /*pEntity*/) const { return m_SubentId.type(); }

  /** \details
    Returns the subentity ID for the given entity.
  */
  virtual OdDbSubentId subentId(const OdDbEntity* /*pEntity*/) const { return m_SubentId; }

  /** \details
    Checks whether this identifier is kNull.
  */
  virtual bool isNull() const
  {
    return m_SubentId.type() == OdDb::kNullSubentType;
  }

  /** \details
    Checks whether this identifier is equal to the given identifier.
  */
  virtual bool isEqualTo(const OdDbEntity* pEntity, const OdDbAssocPersSubentId* pOther) const
  {
    if (!OdDbAssocPersSubentId::isEqualTo(pEntity, pOther))
      return false;

    OdDbAssocIndexPersSubentIdPtr pOtherIndex = pOther;
    return m_SubentId == pOtherIndex->m_SubentId;
  }

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Provides the standard audit protocol.
  */
  virtual void audit(OdDbAuditInfo* pAuditInfo);

protected:
  /*!DOM*/
  bool m_Unk_Bool_1;
private:
  /*!DOM*/
  OdDbSubentId m_SubentId;
};

/** \details
This class represents the modeler entity persistent subentity identifier.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocAsmBasedEntityPersSubentId : public OdDbAssocPersSubentId
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocAsmBasedEntityPersSubentId);

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbAssocAsmBasedEntityPersSubentId();

  /** \details
  Initializes this instance using the specified ID instance.
  */
  void init(const OdDbSubentId& subentId);

  /** \details
  Returns the number of transient subentity identifiers corresponding to this associated
  persistent subentity identifier for the given entity.
  */
  virtual int transientSubentCount(const OdDbEntity* /*pEntity*/) const { return !isNull() ? 1 : 0; }

  /** \details
  Returns the subentity type for the given entity.
  */
  virtual OdDb::SubentType subentType(const OdDbEntity* /*pEntity*/) const { return m_SubentId.type(); }

  /** \details
  Returns the subentity ID for the given entity.
  */
  virtual OdDbSubentId subentId(const OdDbEntity* /*pEntity*/) const { return m_SubentId; }

  /** \details
  Checks whether this identifier is kNull.
  */
  virtual bool isNull() const
  {
    return m_SubentId.type() == OdDb::kNullSubentType;
  }

  /** \details
  Checks whether this identifier is equal to the given identifier.
  */
  virtual bool isEqualTo(const OdDbEntity* pEntity, const OdDbAssocPersSubentId* pOther) const
  {
    if (!OdDbAssocPersSubentId::isEqualTo(pEntity, pOther))
      return false;

    OdDbAssocAsmBasedEntityPersSubentIdPtr pOtherIndex = pOther;
    return m_SubentId == pOtherIndex->m_SubentId;
  }

  /** \details
  Reads the .dwg file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
  Writes the .dwg file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
  Reads the .dxf file data of this object.

  \param pFiler [in]  Filer object from which data are read.

  \remarks
  Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
  Writes the .dxf file data of this object.

  \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
  Provides the standard audit protocol.
  */
  virtual void audit(OdDbAuditInfo* pAuditInfo);

protected:
  /*!DOM*/
  bool m_Unk_Bool_1;
private:
  /*!DOM*/
  OdDbSubentId m_SubentId;
};


#include "TD_PackPop.h"

#endif


