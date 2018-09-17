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

#ifndef __ODGSHIGHLIGHTDATA_H__
#define __ODGSHIGHLIGHTDATA_H__

#include "GsMarkerArray.h"
#include "Gi/GiDrawable.h"
#include "Gs.h"
#include "TPtr.h"

#include "TD_PackPush.h"

class OdGsHlBranch;
typedef TPtr<OdGsHlBranch> OdGsHlBranchPtr;
typedef OdArray<OdGsHlBranchPtr> OdGsHlBranchPtrArray;

class OdGsHlBranchReactor;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsHlBranchReactor object pointers.
*/
typedef OdSmartPtr<OdGsHlBranchReactor> OdGsHlBranchReactorPtr;

/** \details
    This class is a service class to keep information about highlighted subentities and nested entities on OsGsEntityNode.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsHlBranch
{
  protected:
    ODRX_HEAP_OPERATORS();

    OdRefCounter m_nRefCounter;        // shared ownership
    const void* m_pDrawableId;         // persistent id or transient drawable pointer (depends from flag)
    bool m_bPersistentId;              // true if m_pDrawableId stores OdDbStub pointer
    OdGsHlBranchReactorPtr m_pReactor; // OdGsHlBranch reactor
    OdGsMarkerArray m_aMarkers;        // sorted array of markers
    OdGsHlBranchPtrArray m_aChild;     // sorted array of nested entities
    // the sorting rule: first are transient entities with id 0, sorted by drw() value,
    // next are persistent entities with id != 0, sorted by id()
  public:
    OdGsHlBranch(const OdDbStub* pDrawableId)
      : m_nRefCounter(1)
      , m_pDrawableId(pDrawableId)
      , m_bPersistentId(true)
      , m_aMarkers(0, 1)
      , m_aChild(0, 1)
    {
    }
    OdGsHlBranch(const OdGiDrawable* pTransDrawable)
      : m_nRefCounter(1)
      , m_pDrawableId(pTransDrawable)
      , m_bPersistentId(false)
      , m_aMarkers(0, 1)
      , m_aChild(0, 1)
    {
    }

    static OdGsHlBranchPtr create(OdDbStub* pDrawableId, OdGsMarker marker);
    static OdGsHlBranchPtr create(OdGiDrawable* pTransDrawable, OdGsMarker marker);

    void addRef()
    {
      ++m_nRefCounter;
    }
    void release()
    {
      ODA_ASSERT((m_nRefCounter > 0));
      if (!(--m_nRefCounter))
        delete this;
    }

    // access to nested entities
    const OdGsHlBranchPtrArray& aChild() const { return m_aChild; }
    // access to subentities
    const OdGsMarkerArray& markers() const { return m_aMarkers; }

    // add/remove/find nested entities info
    OdGsHlBranch* addChild(OdDbStub* pDrawableId);
    OdGsHlBranch* addChild(const OdGiDrawable* pTransDrawable);
    void removeChild(const OdGsHlBranch* pChild);
    OdGsHlBranch* findChild(const OdDbStub* pDrawableId); // search in a sorted array, fast
    OdGsHlBranch* findChild(const OdGiDrawable* pTransDrawable); // search in a sorted array, fast

    // add/remove/find subentities info
    bool addMarker(OdGsMarker marker);
    bool removeMarker(OdGsMarker marker);
    bool hasMarker(OdGsMarker marker) const; // search in a sorted array, fast

    // access to the OdGsEntityNode data
    const OdDbStub* id() const
    {
      return (m_bPersistentId) ? reinterpret_cast<const OdDbStub*>(m_pDrawableId) : NULL;
    }
    const OdGiDrawable* drw() const
    {
      return (!m_bPersistentId) ? reinterpret_cast<const OdGiDrawable*>(m_pDrawableId) : NULL;
    }
    bool isEmpty() const
    {
      return markers().isEmpty() && aChild().isEmpty();
    }

    // access flags
    bool isPersistentId() const { return m_bPersistentId; }

    // reactor
    void setReactor(OdGsHlBranchReactor *pReactor);
    OdGsHlBranchReactor *reactor();

    static inline bool isValidMarker(OdGsMarker marker)
    {
      return (marker != kNullSubentIndex);
    }
};

/** \details
    This class is a reactor for OdGsHlBranch class.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsHlBranchReactor : public OdRxObject
{
  public:
    virtual void onChildAdded(OdGsHlBranch *pHlBranch, OdGsHlBranch *pHlBranchAdded) = 0;
    virtual void onChildRemoved(OdGsHlBranch *pHlBranch, OdGsHlBranch *pHlBranchRemoved) = 0;
    virtual void onMarkerAdded(OdGsHlBranch *pHlBranch, OdGsMarker gsMarker) = 0;
    virtual void onMarkerRemoved(OdGsHlBranch *pHlBranch, OdGsMarker gsMarker) = 0;
};

// gcc3x template instantiation must be done after class declaration
inline void OdGsHlBranch::setReactor(OdGsHlBranchReactor *pReactor) { m_pReactor = pReactor; }
inline OdGsHlBranchReactor *OdGsHlBranch::reactor() { return m_pReactor.get(); }

/** \details
    This class is a multimodule reactor reactor implementation for OdGsHlBranch class.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsHlBranchMultimoduleReactor
{
  public:
    static void attach(OdGsHlBranch *pHlBranch, const void *pModule, OdGsHlBranchReactor *pReactor);
    static void detach(OdGsHlBranch *pHlBranch, const void *pModule);
    static OdGsHlBranchReactor *getReactor(OdGsHlBranch *pHlBranch, const void *pModule);
    static void setReactor(OdGsHlBranch *pHlBranch, const void *pModule, OdGsHlBranchReactor *pReactor);
};

#include "TD_PackPop.h"

#endif // __ODGSHIGHLIGHTDATA_H__
