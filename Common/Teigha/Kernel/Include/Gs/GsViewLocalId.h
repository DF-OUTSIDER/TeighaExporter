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
#ifndef __OD_GS_VIEW_LOCAL_ID__
#define __OD_GS_VIEW_LOCAL_ID__

#include "Gs/Gs.h"

#include "TD_PackPush.h"

class OdGsViewImpl;
class OdGsBaseModel;

/* Note:
   All GsNode's locally owned by only one GsModel, so GsNode metafiles is also owned by GsModel. GsModel could be shared
   between different rendering modules. But viewportId is creating from SlotManager owned by rendering device, so
   viewportId's for different devices will lie in same range, so couldn't be used as unique view identifier inside
   GsModel. To solve problem we create for each GsModel their own SlotManager which is used for generate unique
   viewportId for each view referenced by GsModel. This localViewportId is stored on GsModel side, view access to it
   and store it's own copy localViewportId for optimization purposes (this is not problem for MT because only one
   GsModel is used per parallel vectorization.
*/

/** \details
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseModelLocalIds : public OdRxObject
{
  /* Note: the initial registration will be done on addViewRef. But for example MT ghost views or views
           of layout helper will do registration on first update. In this case first of all we
           search exist registered view, if it is doesn't exist we search view with similar viewportId
           and similar device (case of MT ghost view and layout helper view). Composite device
           must register view ID manually.                                                               */
  public:
    // Call on view deletion to clear model view entry
    virtual void onViewDelete(OdGsViewImpl *pView) = 0;
    // Manually reset Id
    virtual void resetViewId(OdGsViewImpl *pView, OdUInt32 nId) = 0;
    // Get current view Id (or -1 if view doesn't registered)
    virtual OdUInt32 getViewId(OdGsViewImpl *pView) = 0;
    // Make view registration (you must check first does id is already registered)
    virtual OdUInt32 registerView(OdGsViewImpl *pView) = 0;
    // Get maximum registered Id
    virtual OdUInt32 getMaxId() const = 0;

    static OdSmartPtr<OdGsBaseModelLocalIds> createObject();
};
typedef OdSmartPtr<OdGsBaseModelLocalIds> OdGsBaseModelLocalIdsPtr;

/** \details
            
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
// Used in OdGsViewImpl
class GS_TOOLKIT_EXPORT OdGsViewLocalId
{
  protected:
    mutable const OdGsBaseModel *m_pLocalIdModel;
    mutable OdUInt32 m_localId;
    OdArray<OdGsBaseModelLocalIdsPtr> m_localIdRefs;
    OdGsViewImpl *m_pView;
  public:
    explicit OdGsViewLocalId(OdGsViewImpl *pView = NULL)
      : m_pLocalIdModel(NULL)
      , m_localId(0)
      , m_pView(pView)
    {
    }
    ~OdGsViewLocalId();

    void setView(OdGsViewImpl *pView)
    {
      m_pView = pView;
    }
    OdGsViewImpl *view() const
    {
      return m_pView;
    }

    // Gets local viewport Id optimized
    OdUInt32 localViewportId(const OdGsBaseModel *pModel) const
    {
      if (m_pLocalIdModel == pModel)
        return m_localId;
      m_pLocalIdModel = pModel;
      return m_localId = getLocalViewportId(pModel);
    }
    // Gets local viewport Id directly from GsModel
    OdUInt32 getLocalViewportId(const OdGsBaseModel *pModel) const;

    // Reset current state
    void reset()
    {
      m_pLocalIdModel = NULL;
      m_localId = 0;
    }

    OdArray<OdGsBaseModelLocalIdsPtr> &refs()
    {
      return m_localIdRefs;
    }
};

#define ODGS_LOCAL_IDS

#include "TD_PackPop.h"

#endif // __OD_GS_VIEW_LOCAL_ID__
