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

#ifndef __OD_GI_METAFILER__
#define __OD_GI_METAFILER__

#include "Gi/GiConveyorNode.h"
#include "ChunkAllocator.h"
#include "Ge/GeDoubleArray.h"

class OdGiDeviation;

#include "TD_PackPush.h"

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiGeometryMetafile : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiGeometryMetafile);

  ODCA_HEAP_OPERATORS();

  class Record
  {
  protected:
    Record* m_pTail;
  public:
    ODRX_HEAP_OPERATORS();

    Record()
      : m_pTail(0)
    {}

    virtual ~Record() {}

    Record* tail()
    {
      return m_pTail;
    }

    const Record* tail() const
    {
      return m_pTail;
    }

    void setTail(Record* pTail)
    {
      m_pTail = pTail;
    }

    inline void deleteList()
    {
      Record* pCurr = this;
      while(pCurr)
      {
        Record* pTail = pCurr->m_pTail;
        delete pCurr;
        pCurr = pTail;
      }
    }

    virtual void play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const = 0;

    virtual OdUInt64 recordSize() const { return (OdUInt64)sizeof(*this); }
  };

private:
  Record*               m_pHead;

  void destroyRecords()
  {
    if(m_pHead)
    {
      m_pHead->deleteList();
      m_pHead = NULL;
    }
  }

public:

  OdGiGeometryMetafile();
  virtual ~OdGiGeometryMetafile();

  void clear();
  void play(OdGiConveyorGeometry* pGeom, OdGiConveyorContext* pCtx) const;
  virtual OdUInt64 metafileSize() const;
  void setRecords(Record* pRec)
  {
    ODA_ASSERT(m_pHead == 0);
    m_pHead = pRec;
  }
  bool isEmpty() const { return m_pHead == 0; }
  Record *firstRecord() const { return m_pHead; }
  Record *lastRecord() const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGeometryMetafile object pointers.
*/
typedef OdSmartPtr<OdGiGeometryMetafile> OdGiGeometryMetafilePtr;

/** \details

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiMetafiler : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiMetafiler);
  
  virtual void setMetafile(OdGiGeometryMetafile* pMetafile) = 0;
  virtual OdGiGeometryMetafile* metafile() = 0;

  virtual void exchangeMetafile(OdGiGeometryMetafile* pMetafile) = 0;

  /** Sets max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations) = 0;

  /** Sets deviation object to obtain max deviation for curve tesselation.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation) = 0;

  /** Sets the draw context object (to access to traits, etc).
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  enum CoordType
  {
    kUnknown = 0,
    kModel,
    kWorld,
    kEye,
    kDevice
  };

  /** Sets coordinates type for geometry stored by metafiler.
  */
  virtual void setCoordinatesType(CoordType ct) = 0;

  /** Returns coordinates type used by this metafiler object.
  */
  virtual CoordType coordinatesType() const = 0;

  /** Save traits into metafile. Optionally save ByBlock traits.
  */
  virtual bool saveTraits(const OdGiSubEntityTraitsData& entTraits, const OdGiSubEntityTraitsData *byBlockTraits = NULL) = 0;
  /** Save effective traits into metafile. Optionally save ByBlock traits.
  */
  virtual bool saveTraits(const OdGiSubEntityTraitsData *byBlockTraits) = 0;
  /** Save only effective traits into metafile.
  */
  virtual bool saveTraits() = 0;

  /** Flush all data to the metafile
  */
  virtual void flush(bool bForceTraits = true) = 0;

  /** Adds metafile record to the records list.
  */
  virtual void add(OdGiGeometryMetafile::Record* pRec) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMetafiler object pointers.
*/
typedef OdSmartPtr<OdGiMetafiler> OdGiMetafilerPtr;

#include "TD_PackPop.h"

#endif //#ifndef __OD_GI_METAFILER__
