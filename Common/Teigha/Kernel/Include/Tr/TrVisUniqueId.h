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
// GLES2 dynamic unique ID declaration

#ifndef ODTRVISUNIQUEID
#define ODTRVISUNIQUEID

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "OdAnsiString.h"
#include "ExGsHelpers.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisUniqueID
{
  public:
    enum IDType
    {
      kPersistentID = 0,
      kTransientID,
      kUtilitaryID
    };
  protected:
    OdUInt64 m_entryId;
    OdUInt64 m_entryData;
    OdUInt32 m_entryType;
  public:
    OdTrVisUniqueID()
      : m_entryId(0)
      , m_entryData(0)
      , m_entryType(0)
    {
    }
    OdTrVisUniqueID(const OdTrVisUniqueID &ui)
      : m_entryId(ui.m_entryId)
      , m_entryData(ui.m_entryData)
      , m_entryType(ui.m_entryType)
    {
    }
    OdTrVisUniqueID(OdUInt64 entryId, OdUInt32 entryType = kTransientID, OdUInt64 entryData = 0)
      : m_entryId(entryId), m_entryData(entryData), m_entryType(entryType)
    {
    }

    OdTrVisUniqueID &operator =(const OdTrVisUniqueID &ui) 
    { 
      if (this == &ui) 
        return *this;
      m_entryId = ui.m_entryId;
      m_entryData = ui.m_entryData;
      m_entryType = ui.m_entryType;
      return *this; 
    }

    bool operator ==(const OdTrVisUniqueID &ui) const
    {
      return (m_entryId == ui.m_entryId) &&
             (m_entryData == ui.m_entryData) &&
             (m_entryType == ui.m_entryType);
    }
    bool operator !=(const OdTrVisUniqueID &ui) const
    {
      return (m_entryId != ui.m_entryId) ||
             (m_entryData != ui.m_entryData) ||
             (m_entryType != ui.m_entryType);
    }

    bool isValid() const { return (m_entryId != 0) || (m_entryData != 0); }

    void setId(OdUInt64 nId) { m_entryId = nId; }
    void setId(const void *pId) { m_entryId = OdTrVisPtrToId(pId); }
    OdUInt64 getId() const { return m_entryId; }
    OdDbStub *persistentId() const { return OdTrVisIdToPtr(OdDbStub, m_entryId); }

    void setData(OdUInt64 nData) { m_entryData = nData; }
    OdUInt64 getData() const { return m_entryData; }

    void setPersistent() { m_entryType = kPersistentID; }
    bool isPersistent() const { return m_entryType == kPersistentID; }
    void setTransient() { m_entryType = kTransientID; }
    bool isTransient() const { return m_entryType == kTransientID; }
    void setUtilitary(OdUInt32 utCode = kUtilitaryID) { m_entryType = utCode; }
    bool isUtilitary() const { return m_entryType >= kUtilitaryID; }
    OdUInt32 getType() const { return m_entryType; }

    static int compare(const OdTrVisUniqueID &t1, const OdTrVisUniqueID &t2)
    {
      if (t1.m_entryId != t2.m_entryId)
        return (t1.m_entryId < t2.m_entryId) ? -1 : 1;
      if (t1.m_entryType != t2.m_entryType)
        return (t1.m_entryType < t2.m_entryType) ? -1 : 1;
      if (t1.m_entryData != t2.m_entryData)
        return (t1.m_entryData < t2.m_entryData) ? -1 : 1;
      return 0;
    }
    //bool operator ==(const OdTrVisUniqueID &t2) const { return compare(*this, t2) == 0; }
    //bool operator !=(const OdTrVisUniqueID &t2) const { return compare(*this, t2) != 0; }
    bool operator <(const OdTrVisUniqueID &t2) const { return compare(*this, t2) < 0; }
    bool operator <=(const OdTrVisUniqueID &t2) const { return compare(*this, t2) <= 0; }
    bool operator >(const OdTrVisUniqueID &t2) const { return compare(*this, t2) > 0; }
    bool operator >=(const OdTrVisUniqueID &t2) const { return compare(*this, t2) >= 0; }

    // OdTrVisUniqueID -> OdAnsiString transformers

    class UIDTransformer
    {
      public:
        virtual void append(OdAnsiString &to, OdDbStub* from) const = 0;
        virtual void append(OdAnsiString &to, OdUInt32 from) const = 0;
        virtual void append(OdAnsiString &to, OdUInt64 from) const = 0;
        virtual char delimiter() const { return '_'; }
    };
    class DefUIDTransformer : public UIDTransformer
    {
      public:
        virtual void append(OdAnsiString &to, OdDbStub* from) const
        {
          append(to, OdTrVisPtrToId(from));
        }
        virtual void append(OdAnsiString &to, OdUInt32 from) const
        {
          OdAnsiString formatter;
          formatter.format("%u", (unsigned)from);
          to += formatter;
        }
        virtual void append(OdAnsiString &to, OdUInt64 from) const
        {
          char fromChr[18];
          fromChr[::ExGsMakeHex(from, fromChr)] = 0;
          to += fromChr;
        }
        virtual char delimiter() const { return '-'; }
    };

    OdAnsiString transform(const UIDTransformer &transformer = DefUIDTransformer()) const
    {
      OdAnsiString formatter;
      if (isPersistent())
        transformer.append(formatter, persistentId());
      else
      {
        transformer.append(formatter, getId());
        formatter += transformer.delimiter();
        transformer.append(formatter, getType());
      }
      if (getData() || !isPersistent())
      {
        // Persistent ID can have single or two elements. Other types will have three elements always, elsewhere them cannot be unparsed.
        formatter += transformer.delimiter();
        transformer.append(formatter, getData());
      }
      return formatter;
    }

    class UIDUnTransformer
    {
    public:
      virtual OdDbStub *makeStub(const char *pStr) const = 0;
      virtual OdUInt32 makeUInt32(const char *pStr) const = 0;
      virtual OdUInt64 makeUInt64(const char *pStr) const = 0;
      virtual char delimiter() const { return '_'; }
    };
    class DefUIDUnTransformer : public UIDUnTransformer
    {
      public:
        virtual OdDbStub *makeStub(const char *pStr) const
        {
          return OdTrVisIdToPtr(OdDbStub, makeUInt64(pStr));
        }
        virtual OdUInt32 makeUInt32(const char *pStr) const
        {
          OdUInt32 nVal = 0;
          while ((*pStr >= '0') && (*pStr <= '9'))
          {
            nVal = nVal * 10 + OdUInt32(*pStr - '0');
            pStr++;
          }
          return nVal;
        }
        virtual OdUInt64 makeUInt64(const char *pStr) const
        {
          OdUInt64 nVal = 0;
          while (((*pStr >= '0') && (*pStr <= '9')) ||
                 ((*pStr >= 'a') && (*pStr <= 'f')) ||
                 ((*pStr >= 'A') && (*pStr <= 'F')))
          {
            OdUInt64 mx = 0;
            if ((*pStr >= '0') && (*pStr <= '9'))
              mx = OdUInt64(*pStr - '0');
            else if ((*pStr >= 'a') && (*pStr <= 'f'))
              mx = OdUInt64(*pStr - 'a') + 10;
            else if ((*pStr >= 'A') && (*pStr <= 'F'))
              mx = OdUInt64(*pStr - 'A') + 10;
            nVal = (nVal << 4) | mx;
            pStr++;
          }
          return nVal;
        }
        virtual char delimiter() const { return '-'; }
    };

    OdTrVisUniqueID &untransform(const char *pStr, const UIDUnTransformer &untransformer = DefUIDUnTransformer())
    {
      const char *pStr1 = pStr;
      const char *pStr2 = NULL, *pStr3 = NULL;
      while (*pStr && !pStr3)
      {
        if (*pStr == untransformer.delimiter())
        {
          if (!pStr2) pStr2 = pStr + 1;
          else pStr3 = pStr + 1;
        }
        pStr++;
      }
      if (!pStr3)
      { // Persistent id
        setId(untransformer.makeStub(pStr1));
        setPersistent();
        if (pStr2)
          setData(untransformer.makeUInt64(pStr2));
      }
      else
      { // Transient or utilitary
        setId(untransformer.makeUInt64(pStr1));
        setUtilitary(untransformer.makeUInt32(pStr2));
        setData(untransformer.makeUInt64(pStr3));
      }
      return *this;
    }

    OdTrVisUniqueID(const OdAnsiString& asValue, const UIDUnTransformer &untransformer = DefUIDUnTransformer())
      : m_entryId(0)
      , m_entryData(0)
      , m_entryType(0)
    {
      untransform(asValue, untransformer);
    }
};

#include "TD_PackPop.h"

#endif // ODTRVISUNIQUEID
