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

#ifndef __OdGsFilerV100Impl_H__
#define __OdGsFilerV100Impl_H__

#include "TD_PackPush.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"
#include "DbBaseDatabase.h"
#include "GsFiler.h"
#include "OdStack.h"
#include "RxObjectImpl.h"


// Database linker
class OdGsFilerDbLinker : public OdRxObject
{
public:
  OdGsFilerDbLinker() { }
  virtual ~OdGsFilerDbLinker() { }
public:
  // Get database hash
  virtual OdBinaryData getDbHash(const OdDbBaseDatabase *pDb) const;

  // Transform database handle
  virtual OdUInt64 stubToDbHandle(OdDbStub *pStub) const = 0;
  virtual OdDbStub *handleToDbStub(OdUInt64 nHandle) const = 0;

  // Util methods
  virtual OdDbBaseDatabase *getDatabase() const = 0;
  virtual OdDbBaseDatabase *getDatabase(OdDbStub *pStub) const = 0;
};

typedef OdSmartPtr<OdGsFilerDbLinker> OdGsFilerDbLinkerPtr;



#define STL_USING_MAP
#define STL_USING_SET
#include "OdaSTL.h"

// Implement substitutor
struct GS_TOOLKIT_EXPORT OdGsFiler_SubstitutorImpl : public OdRxObject, public OdGsFiler::Substitutor
{
  template<OdUInt32 nBytes> struct DataTyp
  {
    OdUInt8 m_data[nBytes];
    static int compare(const DataTyp &t1, const DataTyp &t2)
    {
      for (OdUInt32 nb = 0; nb < nBytes; nb++)
      {
        if (t1.m_data[nb] != t2.m_data[nb])
          return int(t1.m_data[nb]) - int(t2.m_data[nb]);
      }
      return 0;
    }
    bool operator ==(const DataTyp &t2) const { return compare(*this, t2) == 0; }
    bool operator !=(const DataTyp &t2) const { return compare(*this, t2) != 0; }
    bool operator <(const DataTyp &t2) const { return compare(*this, t2) < 0; }
    bool operator <=(const DataTyp &t2) const { return compare(*this, t2) <= 0; }
    bool operator >(const DataTyp &t2) const { return compare(*this, t2) > 0; }
    bool operator >=(const DataTyp &t2) const { return compare(*this, t2) >= 0; }

    operator const void*() const { return this; }
  };
  struct VoidsSet
  {
    void *m_pVoid;
    OdGsFiler::SubstitutionActuator *m_pAct;
  };
  struct VoidArry : public VoidsSet
  {
    OdInt32 m_nVoids;
    VoidArry()
      : m_nVoids(0)
    {
      m_pVoid = NULL;
      m_pAct = NULL;
    }
    ~VoidArry() { clear(); }
    VoidsSet *getSet() { return (VoidsSet*)m_pVoid; }
    VoidsSet *at(OdInt32 n) {
      if (m_nVoids == 1)
        return this;
      return getSet() + n;
    }
    bool contains(void *pVoid);
    void add(void *pVoid, OdGsFiler::SubstitutionActuator *pAct);
    void kill(void *pVoid);
    void clear();
  };

  struct AbstractTyp
  {
    void *m_pContainer;
    AbstractTyp() : m_pContainer(NULL) { }
  };
  typedef std::map<OdUInt32, AbstractTyp> RegsMap;
  typedef std::map<OdUInt32, AbstractTyp> SubstsMap;
  RegsMap m_regs;
  SubstsMap m_substs;

  template<OdUInt32 nBytes> struct Procs
  {
    typedef OdGsFiler_SubstitutorImpl::DataTyp<nBytes> ProcTyp;
    typedef std::map<ProcTyp, ProcTyp> RegMap;
    typedef std::map<ProcTyp, VoidArry> SubstMap;
    static void makeSubst(void *pPlace, const void *pSubstitution)
    {
      *(ProcTyp*)(pPlace) = *(const ProcTyp*)(pSubstitution);
    }
    static void runSubst(void *pPlace, const void *pSubstitution, OdGsFiler::SubstitutionActuator *pActuator)
    {
      ODA_ASSERT(pActuator);
      pActuator->applySubstitution(pPlace, pSubstitution, makeSubst);
    }
    static void subst(OdGsFiler_SubstitutorImpl &impl, const void *pValue, void *pPlace, OdGsFiler::SubstitutionActuator *pAct, const void *pSubstitution, bool bClear)
    {
      if (pPlace && pSubstitution)
      {
        runSubst(pPlace, pSubstitution, pAct);
        return;
      }
      if (pValue)
      {
        if (pPlace)
        {
          RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
          OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
          if (itReg != pRegMap->end())
          {
            runSubst(pPlace, itReg->second, pAct);
            if (bClear)
              pRegMap->erase(itReg);
          }
        }
        else if (pSubstitution)
        {
          SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
          OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(*(const ProcTyp*)pValue);
          if (itSub != pSubMap->end())
          {
            for (OdInt32 n = 0; n < itSub->second.m_nVoids; n++)
              runSubst(itSub->second.at(n)->m_pVoid, pSubstitution, itSub->second.at(n)->m_pAct);
            if (bClear)
              pSubMap->erase(itSub);
          }
        }
        else
        {
          RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
          SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
          if (pRegMap && pSubMap)
          {
            OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
            OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(*(const ProcTyp*)pValue);
            if ((itReg != pRegMap->end()) && (itSub != pSubMap->end()))
            {
              for (OdInt32 n = 0; n < itSub->second.m_nVoids; n++)
                runSubst(itSub->second.at(n)->m_pVoid, itReg->second, itSub->second.at(n)->m_pAct);
            }
            if (bClear)
            {
              if (itReg != pRegMap->end())
                pRegMap->erase(itReg);
              if (itSub != pSubMap->end())
                pSubMap->erase(itSub);
            }
          }
          else if (bClear)
          {
            if (pRegMap)
            {
              OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
              if (itReg != pRegMap->end())
                pRegMap->erase(itReg);
            }
            if (pSubMap)
            {
              OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(*(const ProcTyp*)pValue);
              if (itSub != pSubMap->end())
                pSubMap->erase(itSub);
            }
          }
        }
        return;
      }
      RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
      SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
      if (pRegMap && pSubMap)
      {
        OD_TYPENAME RegMap::iterator itReg = pRegMap->begin();
        while (itReg != pRegMap->end())
        {
          OD_TYPENAME SubstMap::iterator itSub = pSubMap->find(itReg->first);
          if (itSub != pSubMap->end())
          {
            for (OdInt32 n = 0; n < itSub->second.m_nVoids; n++)
              runSubst(itSub->second.at(n)->m_pVoid, itReg->second, itSub->second.at(n)->m_pAct);
          }
          itReg++;
        }
      }
      if (bClear)
      {
        if (pRegMap) { delete pRegMap; impl.m_regs[nBytes].m_pContainer = NULL; }
        if (pSubMap) { delete pSubMap; impl.m_substs[nBytes].m_pContainer = NULL; }
      }
    }
    static void reg(OdGsFiler_SubstitutorImpl &impl, const void *pValue, const void *pSubstitution, bool bRegister, bool bImmediate)
    {
      if (bRegister)
      {
        OdGsFiler_SubstitutorImpl::AbstractTyp &cont = impl.m_regs[nBytes];
        if (!cont.m_pContainer)
          cont.m_pContainer = new RegMap;
        RegMap &pMap = *(RegMap*)cont.m_pContainer;
        pMap[*(const ProcTyp*)pValue] = *(const ProcTyp*)pSubstitution;
      }
      if (bImmediate)
        subst(impl, pValue, NULL, NULL, pSubstitution, false);
    }
    static void req(OdGsFiler_SubstitutorImpl &impl, void *pPlace, OdGsFiler::SubstitutionActuator *pAct, const void *pValue, bool bRegister, bool bImmediate)
    {
      if (bRegister)
      {
        if (bImmediate)
        {
          RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
          if (pRegMap)
          {
            OD_TYPENAME RegMap::iterator itReg = pRegMap->find(*(const ProcTyp*)pValue);
            if (itReg != pRegMap->end())
            {
              runSubst(pPlace, itReg->second, pAct);
              return;
            }
          }
        }
        OdGsFiler_SubstitutorImpl::AbstractTyp &cont = impl.m_substs[nBytes];
        if (!cont.m_pContainer)
          cont.m_pContainer = new SubstMap;
        SubstMap &pMap = *(SubstMap*)cont.m_pContainer;
        pMap[*(const ProcTyp*)pValue].add(pPlace, pAct);
      }
      else if (bImmediate)
        subst(impl, pValue, pPlace, pAct, NULL, false);
    }
    static void clear(OdGsFiler_SubstitutorImpl &impl, const void *pValue)
    {
      RegMap *pRegMap = (RegMap*)impl.m_regs[nBytes].m_pContainer;
      SubstMap *pSubMap = (SubstMap*)impl.m_substs[nBytes].m_pContainer;
      if (pValue)
      {
        if (pRegMap)
        {
          OD_TYPENAME RegMap::iterator it = pRegMap->find(*(const ProcTyp*)pValue);
          if (it != pRegMap->end())
            pRegMap->erase(it);
        }
        if (pSubMap)
        {
          OD_TYPENAME SubstMap::iterator it = pSubMap->find(*(const ProcTyp*)pValue);
          if (it != pSubMap->end())
            pSubMap->erase(it);
        }
      }
      else
      {
        if (pRegMap) { delete pRegMap; impl.m_regs[nBytes].m_pContainer = NULL; }
        if (pSubMap) { delete pSubMap; impl.m_substs[nBytes].m_pContainer = NULL; }
      }
    }
  };

  static OdGsFiler::SubstitutionActuator g_defActuator;
  static struct OdRxObjectSubstitutionActuator : public OdGsFiler::SubstitutionActuator
  {
    OdRxObjectSubstitutionActuator() { }
    virtual void applySubstitution(void *pPlace, const void *pValue, SetPtrFunc pSetFunc)
    {
      OdGsFiler::SubstitutionActuator::applySubstitution(pPlace, pValue, pSetFunc);
      if (pPlace)
        (*reinterpret_cast<OdRxObjectPtr*>(pPlace))->addRef();
    }
  } g_rxObjectActuator;

  // Register known substitution
  virtual void registerSubstitution(const void *pValue, const void *pSubstitution, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = false);
  // Register substitution request
  virtual void requestSubstitution(void *pPlace, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true);
  // Register substitution request for smart pointer
  virtual void requestSubstitution(OdBaseObjectPtr *pPlace, const void *pValue, bool bRegister = true, bool bImmediate = true);
  // Register substitution with non-default behavior
  virtual void requestSubstitution(void *pPlace, OdGsFiler::SubstitutionActuator *pActuator, const void *pValue, OdUInt32 size = sizeof(OdIntPtr), bool bRegister = true, bool bImmediate = true);
  // Clear substitutions
  virtual void clearSubstitutions(const void *pValue = NULL, OdUInt32 size = 0);
  // Run substitutions
  virtual void runSubstitutions(const void *pValue = NULL, OdUInt32 size = 0, bool bClear = true);
#undef REG_FUNCCALL
  ~OdGsFiler_SubstitutorImpl() { clearSubstitutions(); }
};


// Implement V1 version filer
class GS_TOOLKIT_EXPORT OdGsFilerV100Impl : public OdGsFiler {
protected:
  enum Flags {
    kOpenedForWrite = (1 << 0),

    kLastFlag = kOpenedForWrite
  };
protected:
  mutable OdStreamBufPtr  m_pStream;
  OdGsFilerDbLinkerPtr    m_pLinker;
  OdUInt64                m_nSectionsToWrite;
  OdUInt64                m_nSectionsToRead;
  OdUInt32                m_nFlags;
  OdUInt32                m_nVersion;
  mutable Section         m_nCurSection;
  mutable OdUInt32        m_nCurSectionSize;
  struct WrSectionData {
    OdGsFiler::Section  m_type;
    OdUInt64            m_begin;
    WrSectionData()
      : m_type(OdGsFiler::kEOFSection)
      , m_begin(0)
    { }
    WrSectionData(OdGsFiler::Section section, OdUInt32 tell)
      : m_type(section)
      , m_begin(tell)
    { }
  };
  OdStack<WrSectionData> m_sectionStack;
  mutable OdSmartPtr<OdGsFiler_SubstitutorImpl> m_subst;
  typedef std::map<OdString, OdRxObjectPtr> ArbDataMap;
  ArbDataMap m_arbData;
  typedef std::set<const void *> RegPtrsSet;
  RegPtrsSet m_regPtrs;
protected:
  bool isWriting() const { return GETBIT(m_nFlags, kOpenedForWrite); }
  void setWriting(bool bSet) { SETBIT(m_nFlags, kOpenedForWrite, bSet); }

  void assertWr() const { if (!isWriting()) throw OdError(eNotOpenForWrite); }
  void assertRd() const { if (isWriting()) throw OdError(eNotOpenForRead); }
public:
  OdGsFilerV100Impl()
    : m_nSectionsToWrite(OdUInt64(-1))
    , m_nSectionsToRead(OdUInt64(-1))
    , m_nFlags(0)
    , m_nVersion(kV2)
    , m_nCurSection(kEOFSection)
    , m_nCurSectionSize(0)
  {
    m_subst = OdRxObjectImpl<OdGsFiler_SubstitutorImpl>::createObject();
  }
  ~OdGsFilerV100Impl()
  {
  }

  OdUInt32 headerTestChunk() const;
  bool rdHeaderSection();
  void wrHeaderSection();

  virtual bool setStream(OdStreamBuf *pStream, bool bWrite = false);
  virtual OdStreamBufPtr getStream() const;

  virtual void setDatabase(const OdDbBaseDatabase *pDb);
  virtual OdDbBaseDatabase *getDatabase() const;

  virtual void setVersion(OdUInt32 nVersion);
  virtual OdUInt32 version() const;

  virtual void wrDbHash(const OdDbBaseDatabase *pDb);
  virtual bool checkDbHash(const OdDbBaseDatabase *pDb);

  virtual void setWriteSections(OdUInt64 nSections);
  virtual void setWriteSection(Section section, bool bSet);
  virtual bool isWriteSection(Section section) const;

  virtual void setReadSections(OdUInt64 nSections);
  virtual void setReadSection(Section section, bool bSet);
  virtual bool isReadSection(Section section) const;

  virtual void wrSectionBegin(Section section);
  virtual void wrSectionEnd(Section section);
  virtual void wrEOFSection();

  virtual Section rdSection() const;
  virtual Section curSection() const;
  virtual void skipSection() const;
  virtual void rdBackSection() const;
  virtual bool checkEOF() const;

  virtual Substitutor *subst() const;
  virtual void makeSubstitutions(bool bClear = true) const;
  virtual OdRxObjectPtr getSubstitutor() const;
  virtual void setSubstitutor(OdRxObject *pSubst);

  virtual void setArbitraryData(const OdChar *pName, OdRxObject *pObject);
  virtual OdRxObjectPtr getArbitraryData(const OdChar *pName) const;
  virtual bool hasArbitraryData(const OdChar *pName) const;
  virtual void clearArbitraryData();

  virtual void registerPtr(const void *pPtr);
  virtual void unregisterPtr(const void *pPtr);
  virtual bool isPtrRegistered(const void *pPtr) const;
  virtual void clearRegisteredPtrs();

  virtual void wrHandle(OdDbStub *pHandle);
  virtual OdDbStub *rdHandle() const;

  virtual void wrClass(OdRxObject *pObj);
  virtual OdRxObjectPtr rdClass() const;

  virtual void wrRawData(const void *pData, OdUInt32 nDataSize);
  virtual void rdRawData(void *pData, OdUInt32 nDataSize) const;
};

#include "TD_PackPop.h"

#endif // __OdGsFilerV100Impl_H__
