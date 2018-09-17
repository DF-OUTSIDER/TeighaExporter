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
// GLES2 device rendition Id registrator implementation

#ifndef ODGLES2REGISTRATORIMPL
#define ODGLES2REGISTRATORIMPL

#include "TD_PackPush.h"

#include "TrVisRendition.h"
#include "TrVisUniqueId.h"

#define STL_USING_MAP
#define STL_USING_SET
#include "OdaSTL.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdGLES2RegistratorImpl : public OdTrVisRendition::IdRegistrator
{
  protected:
    typedef std::map<OdTrVisId, OdTrVisUniqueID> RegMap;
    mutable RegMap m_regMap[OdTrVisRendition::kNDataTypes];
    mutable OdTrVisUniqueID m_tmpId;
    struct RegType
    {
      OdUInt64 m_entryId;
      OdUInt32 m_entryType;

      RegType(const OdTrVisUniqueID& id)
        : m_entryId(id.getId())
        , m_entryType(id.getType())
      {}

      bool operator <(const RegType &r2) const
      {
        if (m_entryType != r2.m_entryType)
          return m_entryType < r2.m_entryType;
        return m_entryId < r2.m_entryId;
      }

      OdTrVisUniqueID asUID() const
      {
        return OdTrVisUniqueID(m_entryId, m_entryType);
      }
    };
    typedef std::set<OdUInt64> IdSet;
    struct RegLink
    {
      OdUInt64 m_nLowFreeId;
      IdSet m_linkIds;
      RegLink()
        : m_nLowFreeId(0)
      {
      }
      bool isEmpty() const { return m_linkIds.empty(); }
    };
    typedef std::map<RegType, RegLink> LinkMap;
    mutable LinkMap m_linkMap[OdTrVisRendition::kNDataTypes];
  public:
    struct Traverser
    {
      virtual bool idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId) = 0;
    };
  public:
    OdGLES2RegistratorImpl();
    ~OdGLES2RegistratorImpl();

    // Registrator methods

    virtual void reg(OdTrVisRendition::DataType type, OdTrVisId id, OdDbStub *pPersId);
    virtual void reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdGiDrawable *pTransId);
    virtual void reg(OdTrVisRendition::DataType type, OdTrVisId id, const OdTrVisUniqueID &uId);
    virtual void kill(OdTrVisRendition::DataType type, OdTrVisId id);
    virtual OdTrVisUniqueID *get(OdTrVisRendition::DataType type, OdTrVisId id, bool bKill = false) const;

    void killImpl(OdTrVisRendition::DataType type, OdTrVisId id) const;
    void killAll();

    void traverse(Traverser *pTraverser);

    bool save(OdGsFiler *pFiler) const;
    bool load(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap);
};

#include "TD_PackPop.h"

#endif // ODGLES2REGISTRATORIMPL
