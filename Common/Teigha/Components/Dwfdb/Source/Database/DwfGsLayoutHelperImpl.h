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

#ifndef __ODDWFGSLAYOUTHELPERIMPL_H__
#define __ODDWFGSLAYOUTHELPERIMPL_H__

#include "DwfGsLayoutHelper.h"
#include "GsBaseLayoutHelper.h"
#include "DoubleArray.h"
#include "Int32Array.h"
#include "DwfObjectReactor.h"

#include "TD_PackPush.h"

class OdDwfGsLayoutHelperInt : public OdGsBaseLayoutHelperInt<OdDwfDatabase, OdDwfObject>, OdStaticRxObject<OdDwfDatabaseReactor>
{
  static OdGiDrawablePtr openDrawable(OdDbStub* id);

  // OdDgDatabaseReactor overridden
  virtual void elementAppended(const OdDwfDatabase* pDb, const OdDwfObject* pElm);
  virtual void elementModified(const OdDwfDatabase* pDb, const OdDwfObject* pElm);
  virtual void elementErased(const OdDwfDatabase* pDb, const OdDwfObject* pElm, bool bErased);
  virtual void goodbye(const OdDwfDatabase* pDb);

public:
  ODRX_HEAP_OPERATORS();

  void init(OdGsDevice* pUnderlyingDevice, const OdDwfObjectId& layoutId);

  OdDwfGsLayoutHelperInt();
  ~OdDwfGsLayoutHelperInt() {
    if (this->m_pDb) {
      this->m_pDb->removeReactor(this);
    }
  }
};

template <class TBase>
class OdDwfGsLayoutHelperImpl : public OdGsBaseLayoutHelperImpl<TBase, OdDwfGsLayoutHelperInt>
{
protected:
  ODRX_HEAP_OPERATORS();

public:
  OdDwfObjectId layoutId() const
  {
    return this->getLayoutId();
  }

  OdDwfObjectId getActiveViewportId()
  {
    return this->getActiveViewportStub();
  }

  bool setActiveViewport(const OdDwfObjectId& id)
  {
    return this->setActiveViewportStub(id);
  }
  typedef OdGsBaseLayoutHelperImpl<TBase, OdDwfGsLayoutHelperInt> OdGsBaseLayoutHelperImplDef;
  TD_USING(OdGsBaseLayoutHelperImplDef::setActiveViewport);
};

class ExDwfGsLayoutHelperImpl : public OdDwfGsLayoutHelperImpl<OdDwfGsLayoutHelper>
{
  OdDoubleArray m_zoom;
  OdDoubleArray m_scale;
  OdInt32Array m_projection;

  struct ViewInfo
  {
    ViewInfo() : isPsCalculated(false) {}

    OdGePoint2d           lowerLeft;
    OdGePoint2d           upperRight;

    OdGeVector3d          zoomingOffset;

    bool                  isPsCalculated;

    OdGePoint3d           position;
    OdGePoint3d           target;
    OdGeVector3d          upVector;
    double                scale;
    OdGePoint3d           offset;
    OdGsView::Projection  projection;

    OdGePoint3d           min;
    OdGePoint3d           max;
  };
  OdArray<ViewInfo>   m_viewInfos;
protected:
  ExDwfGsLayoutHelperImpl();
public:
  ~ExDwfGsLayoutHelperImpl();
  ODRX_DECLARE_MEMBERS(ExDwfGsLayoutHelperImpl);
  static OdDwfGsLayoutHelperPtr createObject(OdGsDevice* pUnderlyingDevice, const OdDwfObjectId& idLayout);

  void viewPsCoordinates(int viewNo, OdGePoint2d& lowerLeft, OdGePoint2d& upperRight);
  void setViewPsCoordinates(int viewNo, const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight);
  //static void setViewportClipRegion(const OdDwfViewer* pViewer, OdGsView* pGsView);
  void setViewParameters(int viewNo, const OdGePoint3d& position, const OdGePoint3d& target,
    const OdGeVector3d& upVector, const double scale, const OdGePoint3d& offset,
    const OdGsView::Projection projection);
  void setViewMinMax(int viewNo, const OdGePoint3d& min, const OdGePoint3d& max);
  //void initGsView(OdGsView* pView, const OdDwfViewport* pVp);
  //void initGsView(int vpNo, const OdDwfViewport* pVp);

  void update(OdGsDCRect* pUpdatedRect = NULL);
  void addView(OdGsView* pView);
  void insertView(int viewIndex, OdGsView* pView);

  void onSize(const OdGsDCRect& outputRect);

  bool setActiveViewport(const OdGePoint2d& screenPt);

  //virtual OdGsViewPtr addViewport(const OdDwfViewport* pVp);
  //virtual OdGsViewPtr insertViewport(int viewIndex, const OdDwfViewport* pVp);
  OdGsViewPtr overallView();

  void setViewsParams(OdGiContextForDwfDatabase* pGiCtx, OdGsView* pSheetView);

  void setDisplayStyle(OdGsView* pView, const class OdDwfView* pDwfView);

  static double calcLwScale(const OdGeMatrix3d& worldToScreen,
                            const class OdDwfLayoutView* pLayout, 
                            bool isPlotting,
                            double dCoeff = 0.0, // no 
                            bool checkUnits = false);
};

typedef OdSmartPtr<ExDwfGsLayoutHelperImpl> ExDwfGsLayoutHelperImplPtr;

#include "TD_PackPop.h"

#endif // __ODDWFGSLAYOUTHELPERIMPL_H__

