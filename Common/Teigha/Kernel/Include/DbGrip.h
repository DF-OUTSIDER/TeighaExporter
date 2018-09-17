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




#ifndef _ODDBGRIP_INCLUDED_
#define _ODDBGRIP_INCLUDED_ /*!DOM*/

#include "TD_PackPush.h"
#include "Ge/GePoint3d.h"
#include "SharedPtr.h"
#include "DbStubPtrArray.h"
#include "GsMarkerArray.h"

class OdDbGripData;
template <class T> class OdSharedPtr;
typedef OdSharedPtr<OdDbGripData> OdDbGripDataPtr;

/** \details
    This template class is a specialization of the OdArray class template for OdDbGripData smart pointers.
*/
typedef OdArray<OdDbGripDataPtr> OdDbGripDataPtrArray;

typedef OdArray<void*, OdMemoryAllocator<void*> > OdDbVoidPtrArray;

class OdDbDimData;
typedef OdArray<OdDbDimData*, OdMemoryAllocator<OdDbDimData*> > OdDbDimDataPtrArray;

class OdGiWorldDraw;
class OdGiViewportDraw;
class OdGiViewport;
class OdDbGripData;
class OdDbCustomOsnapMode;
typedef void* ODHMENU;

/** \details
    This template class is a specialization of the OdArray class template for OdDbGripData shared pointers.
*/
typedef OdArray<OdDbGripData> OdDbGripDataArray;

typedef OdSmartPtr<OdDbCustomOsnapMode> OdDbCustomOsnapModePtr;

/** \details
    This namespace contains functions for working with OdDbEntity.

    <group TD_Namespaces>
*/

namespace OdDb
{
  enum DragStat
  {
    kDragStart = 0,
    kDragEnd   = 1,
    kDragAbort = 2
  };

  enum GripStat
  {
    kGripsDone          = 0,
    kGripsToBeDeleted   = 1,
    kDimDataToBeDeleted = 2
  };
}

/** \details
    
    <group TD_Namespaces>
*/
namespace OdDbGripOperations
{
  enum DrawType
  {
    kWarmGrip = 0,
    kHoverGrip,
    kHotGrip,
    kDragImageGrip
  };

  enum GripStatus
  {
    kGripStart = 0,
    kGripEnd,
    kGripAbort,
    kStretch,
    kMove,
    kRotate,
    kScale,
    kMirror,
    kDimFocusChanged,
    kPopUpMenu
  };

  enum GripFlags
  {
    kSkipWhenShared           = 0x001,
    kDisableRubberBandLine    = 0x002,
    kDisableModeKeywords      = 0x004,
    kDrawAtDragImageGripPoint = 0x008,
    kTriggerGrip              = 0x010,
    kTurnOnForcedPick         = 0x020,
    kMapGripHotToRtClk        = 0x040,
    kGizmosEnabled            = 0x080,
    kGripIsPerViewport        = 0x100
  };

  enum GripContextFlags
  {
    kSharedGrip    = 0x01,
    kMultiHotGrip  = 0x02
  };

  enum GetGripPointsFlags
  {
    kGripPointsOnly    = 0x01,
    kCyclableGripsOnly = 0x02,
    kDynamicDimMode    = 0x04
  };

  enum MoveGripPointsFlags
  {
    kOsnapped  = 0x01,
    kPolar     = 0x02,
    kOtrack    = 0x04,
    kZdir      = 0x08,
    kKeyboard  = 0x10
  };
}

namespace OdDb
{
  enum OsnapMask
  {
    kOsMaskEnd          = 0x000001,
    kOsMaskMid          = 0x000002,
    kOsMaskCen          = 0x000004,
    kOsMaskNode         = 0x000008,
    kOsMaskQuad         = 0x000010,
    kOsMaskInt          = 0x000020,
    kOsMaskIns          = 0x000040,
    kOsMaskPerp         = 0x000080,
    kOsMaskTan          = 0x000100,
    kOsMaskNear         = 0x000200,
    kOsMaskQuick        = 0x000400,
    kOsMaskApint        = 0x000800,
    kOsMaskImmediate    = 0x010000,
    kOsMaskAllowTan     = 0x020000,
    kOsMaskDisablePerp  = 0x040000,
    kOsMaskRelCartesian = 0x080000,
    kOsMaskRelPolar     = 0x100000,
    kOsMaskNoneOverride = 0x200000
  };
}

typedef bool (*GripWorldDrawPtr)(
  OdDbGripData* pThis,
  OdGiWorldDraw* pWd,
  OdDbStub* entId,
  OdDbGripOperations::DrawType type,
  OdGePoint3d* imageGripPoint,
  double dGripSize );

typedef void (*GripViewportDrawPtr)(
  OdDbGripData* pThis,
  OdGiViewportDraw* pWd,
  OdDbStub* entId,
  OdDbGripOperations::DrawType type,
  OdGePoint3d* imageGripPoint,
  int gripSize );

typedef OdResult (*GripOperationPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  int iContextFlags );

typedef void (*GripOpStatusPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  OdDbGripOperations::GripStatus status );

typedef const OdChar* (*GripToolTipPtr)( OdDbGripData* pThis );

typedef void (*ContextMenuItemIndexPtr)( unsigned itemIndex );

typedef void (*GripDimensionPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  double dimScale,
  OdDbDimDataPtrArray& dimData );

typedef OdResult (*GripRtClkHandler)(
  OdDbGripDataArray& hotGrips,
  const OdDbStubPtrArray& ents, 
  OdString& menuName, ODHMENU& menu,
  ContextMenuItemIndexPtr& cb );

typedef OdResult (*GripInputPointPtr)(
  OdDbGripData* pThis, OdDbStub* entId, bool& changedPoint,
  OdGePoint3d& newPoint, const OdGiViewport& viewContext,
  OdDbStub* viewportId, bool pointComputed, int history,
  const OdGePoint3d& lastPoint, const OdGePoint3d& rawPoint,
  const OdGePoint3d& grippedPoint, const OdGePoint3d& cartesianSnappedPoint,
  const OdGePoint3d& osnappedPoint, OdDb::OsnapMask osnapMask,
  const OdArray<OdDbCustomOsnapModePtr>& customOsnapModes,
  OdDb::OsnapMask osnapOverrides,
  const OdArray<OdDbCustomOsnapModePtr>& customOsnapOverrides,
  const OdDbStubPtrArray& pickedEntities,
  const OdArray<OdDbStubPtrArray>& nestedPickedEntities,
  const OdGsMarkerArray& gsSelectionMark,
  const OdDbStubPtrArray& keyPointEntities,
  const OdArray<OdDbStubPtrArray>& nestedKeyPointEntities,
  const OdGsMarkerArray& keyPointGsSelectionMark,
  const OdArray<OdSharedPtr<OdGeCurve3d> >& alignmentPaths, const OdGePoint3d& computedPoint); 

/** \details
    This class passes parameters for the getGripPoints and moveGripPoints functions of OdDbEntity.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class OdDbGripData
{
public:
  ODRX_HEAP_OPERATORS();

  OdDbGripData();

  OdDbGripData(const OdDbGripData&);

  OdDbGripData(const OdGePoint3d& pt, void* AppData,
    GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
    GripRtClkHandler RtClk, GripWorldDrawPtr wd,
    GripViewportDrawPtr vd, GripOpStatusPtr stat,
    GripToolTipPtr tt, GripDimensionPtr hoverDim,
    GripDimensionPtr hotGripDim,
    unsigned bitFlags, OdGePoint3d* altBasePt,
    GripInputPointPtr inputPointFunc);

  OdDbGripData(const OdGePoint3d& pt, void* AppData,
    OdRxClass* pAppDataClass,
    GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
    GripRtClkHandler RtClk, GripWorldDrawPtr wd,
    GripViewportDrawPtr vd, GripOpStatusPtr stat,
    GripToolTipPtr tt, GripDimensionPtr hoverDim,
    GripDimensionPtr hotGripDim,
    unsigned bitFlags, OdGePoint3d* altBasePt,
    GripInputPointPtr inputPointFunc);

  void setAllData(const OdGePoint3d& pt, void* AppData,
    GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
    GripRtClkHandler RtClk, GripWorldDrawPtr wd,
    GripViewportDrawPtr vd, GripOpStatusPtr stat,
    GripToolTipPtr tt, GripDimensionPtr hoverDim,
    GripDimensionPtr hotGripDim,
    unsigned bitFlags, OdGePoint3d* altBasePt,
    GripInputPointPtr inputPointFunc, 
    OdRxClass* pAppDataClass);

  OdDbGripData& operator = (const OdDbGripData&);

  const OdGePoint3d& gripPoint() const;
  void setGripPoint( const OdGePoint3d& pt );

  void* appData() const;
  void setAppData( void* pAppData );

  OdRxClass* appDataOdRxClass() const;
  void setAppDataOdRxClass(OdRxClass* pClass);

  GripOperationPtr hotGripFunc() const;
  void setHotGripFunc( GripOperationPtr pf );

  GripOperationPtr hoverFunc() const;
  void setHoverFunc( GripOperationPtr pf );

  GripWorldDrawPtr worldDraw() const;
  void setWorldDraw( GripWorldDrawPtr pf );
  
  GripViewportDrawPtr viewportDraw() const;
  void setViewportDraw( GripViewportDrawPtr pf );
  
  GripOpStatusPtr gripOpStatFunc() const;
  void setGripOpStatFunc( GripOpStatusPtr pf );

  GripToolTipPtr toolTipFunc() const;
  void setToolTipFunc( GripToolTipPtr pf );

  OdGePoint3d* alternateBasePoint() const;
  void setAlternateBasePoint( OdGePoint3d* altBasePt );

  unsigned bitFlags() const;
  void setBitFlags( unsigned flags );

  bool skipWhenShared() const;
  void setSkipWhenShared( bool skip );

  bool isRubberBandLineDisabled() const;
  void disableRubberBandLine( bool disable );

  bool areModeKeywordsDisabled() const;
  void disableModeKeywords( bool disable );

  bool drawAtDragImageGripPoint() const;
  void setDrawAtDragImageGripPoint( bool atDragPoint );

  bool triggerGrip() const;
  void setTriggerGrip( bool trigger );

  bool forcedPickOn() const;
  void setForcedPickOn( bool on );

  bool mapGripHotToRtClk() const;
  void setMapGripHotToRtClk( bool on );

  bool gizmosEnabled() const;
  void setGizmosEnabled( bool on );

  bool gripIsPerViewport() const;
  void setGripIsPerViewport(bool on);

  GripDimensionPtr hoverDimensionFunc() const;
  void setHoverDimensionFunc( GripDimensionPtr pf );

  GripDimensionPtr hotGripDimensionFunc() const;
  void setHotGripDimensionFunc( GripDimensionPtr pf );

  GripRtClkHandler rtClk() const;
  void setRtClk( GripRtClkHandler pf );

  GripInputPointPtr inputPointFunc() const;
  void setInputPointFunc( GripInputPointPtr pf );

private:
  void*               m_pAppData;
  OdRxClass*          m_pAppDataClass;
  OdGePoint3d         m_gripPt;
  OdGePoint3d*        m_pAltBasePt;
  GripOperationPtr    m_pHotGripFunc;
  GripOperationPtr    m_pHoverFunc;
  GripWorldDrawPtr    m_pWorldDraw;
  GripViewportDrawPtr m_pViewportDraw;
  GripOpStatusPtr     m_pGripOpStatFunc;
  GripToolTipPtr      m_pToolTipFunc;
  unsigned        m_bitFlags;
  GripInputPointPtr   m_pInputPointFunc;
  GripRtClkHandler    m_pRtClk;
  GripDimensionPtr    m_pHoverDimensionFunc;
  GripDimensionPtr    m_pHotGripDimensionFunc;
};

//
// Inline methods
//

inline OdDbGripData::OdDbGripData()
  : m_pAppData(0)
  , m_pAppDataClass(0)
  , m_pAltBasePt(0)
  , m_pHotGripFunc(0)
  , m_pHoverFunc(0)
  , m_pWorldDraw(0)
  , m_pViewportDraw(0)
  , m_pGripOpStatFunc(0)
  , m_pToolTipFunc(0)
  , m_bitFlags(0)
  , m_pInputPointFunc(0)
  , m_pRtClk(0)
  , m_pHoverDimensionFunc(0)
  , m_pHotGripDimensionFunc(0)
{
}

inline OdDbGripData::OdDbGripData(const OdDbGripData& srcData)
{
  m_gripPt                = srcData.gripPoint();
  m_pAppData              = srcData.appData();
  m_pAppDataClass         = srcData.appDataOdRxClass();
  m_pHotGripFunc          = srcData.hotGripFunc();
  m_pHoverFunc            = srcData.hoverFunc();
  m_pRtClk                = srcData.rtClk();
  m_pWorldDraw            = srcData.worldDraw();
  m_pViewportDraw         = srcData.viewportDraw();
  m_pGripOpStatFunc       = srcData.gripOpStatFunc();
  m_pToolTipFunc          = srcData.toolTipFunc();
  m_pHoverDimensionFunc   = srcData.hoverDimensionFunc();
  m_pHotGripDimensionFunc = srcData.hotGripDimensionFunc();
  m_bitFlags              = srcData.bitFlags();
  m_pAltBasePt            = srcData.alternateBasePoint();
  m_pInputPointFunc       = srcData.inputPointFunc();
}

inline
OdDbGripData::OdDbGripData(const OdGePoint3d& pt, void* AppData,
                           GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
                           GripRtClkHandler RtClk, GripWorldDrawPtr wd,
                           GripViewportDrawPtr vd, GripOpStatusPtr stat,
                           GripToolTipPtr tt, GripDimensionPtr hoverDim,
                           GripDimensionPtr hotGripDim,
                           unsigned bitFlags, OdGePoint3d* altBasePt,
                           GripInputPointPtr inputPointFunc)
  : m_pAppData(AppData)
  , m_pAppDataClass(0)
  , m_gripPt(pt)
  , m_pAltBasePt(altBasePt)
  , m_pHotGripFunc(hotGrip)
  , m_pHoverFunc(hoverGrip)
  , m_pWorldDraw(wd)
  , m_pViewportDraw(vd)
  , m_pGripOpStatFunc(stat)
  , m_pToolTipFunc(tt)
  , m_bitFlags(bitFlags)
  , m_pInputPointFunc(inputPointFunc)
  , m_pRtClk(RtClk)
  , m_pHoverDimensionFunc(hoverDim)
  , m_pHotGripDimensionFunc(hotGripDim)
{
}

inline
OdDbGripData::OdDbGripData(const OdGePoint3d& pt, void* AppData,
                           OdRxClass* pAppDataClass,
                           GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
                           GripRtClkHandler RtClk, GripWorldDrawPtr wd,
                           GripViewportDrawPtr vd, GripOpStatusPtr stat,
                           GripToolTipPtr tt, GripDimensionPtr hoverDim,
                           GripDimensionPtr hotGripDim,
                           unsigned bitFlags, OdGePoint3d* altBasePt,
                           GripInputPointPtr inputPointFunc)
  : m_pAppData(AppData)
  , m_pAppDataClass(pAppDataClass)
  , m_gripPt(pt)
  , m_pAltBasePt(altBasePt)
  , m_pHotGripFunc(hotGrip)
  , m_pHoverFunc(hoverGrip)
  , m_pWorldDraw(wd)
  , m_pViewportDraw(vd)
  , m_pGripOpStatFunc(stat)
  , m_pToolTipFunc(tt)
  , m_bitFlags(bitFlags)
  , m_pInputPointFunc(inputPointFunc)
  , m_pRtClk(RtClk)
  , m_pHoverDimensionFunc(hoverDim)
  , m_pHotGripDimensionFunc(hotGripDim)
{
}

inline OdDbGripData& OdDbGripData::operator = (const OdDbGripData& rightSide)
{
  m_gripPt                = rightSide.gripPoint();
  m_pAppData              = rightSide.appData();
  m_pAppDataClass         = rightSide.appDataOdRxClass();
  m_pHotGripFunc          = rightSide.hotGripFunc();
  m_pHoverFunc            = rightSide.hoverFunc();
  m_pRtClk                = rightSide.rtClk();
  m_pWorldDraw            = rightSide.worldDraw();
  m_pViewportDraw         = rightSide.viewportDraw();
  m_pGripOpStatFunc       = rightSide.gripOpStatFunc();
  m_pToolTipFunc          = rightSide.toolTipFunc();
  m_pHoverDimensionFunc   = rightSide.hoverDimensionFunc();
  m_pHotGripDimensionFunc = rightSide.hotGripDimensionFunc();
  m_bitFlags              = rightSide.bitFlags();
  m_pAltBasePt            = rightSide.alternateBasePoint();
  m_pInputPointFunc       = rightSide.inputPointFunc();

  return *this;
}


inline void
OdDbGripData::setAllData(const OdGePoint3d& pt, void* pAppData,
                         GripOperationPtr hotGrip, GripOperationPtr hoverGrip,
                         GripRtClkHandler RtClk, GripWorldDrawPtr wd,
                         GripViewportDrawPtr vd, GripOpStatusPtr stat,
                         GripToolTipPtr tt, GripDimensionPtr hoverDim,
                         GripDimensionPtr hotGripDim,
                         unsigned bitFlags, OdGePoint3d* altBasePt,
                         GripInputPointPtr inputPointFunc, 
                         OdRxClass* pAppDataClass)
{
  m_gripPt = pt;
  m_pAppData = pAppData;
  m_pAppDataClass = pAppDataClass;
  m_pHotGripFunc = hotGrip;
  m_pHoverFunc = hoverGrip;
  m_pRtClk = RtClk;
  m_pWorldDraw = wd;
  m_pViewportDraw = vd;
  m_pGripOpStatFunc = stat;
  m_pToolTipFunc= tt;
  m_pHoverDimensionFunc = hoverDim;
  m_pHotGripDimensionFunc = hotGripDim;
  m_bitFlags = bitFlags;
  m_pAltBasePt = altBasePt;
  m_pInputPointFunc = inputPointFunc;
}

inline const OdGePoint3d& OdDbGripData::gripPoint() const
{
  return m_gripPt;
}

inline void OdDbGripData::setGripPoint( const OdGePoint3d& pt )
{
  m_gripPt = pt;
}

inline void* OdDbGripData::appData() const
{
  return m_pAppData;
}

inline void OdDbGripData::setAppData( void* pAppData )
{
  m_pAppData = pAppData;
}

inline OdRxClass* OdDbGripData::appDataOdRxClass() const
{
    return m_pAppDataClass;
}

inline void OdDbGripData::setAppDataOdRxClass(OdRxClass* appDataClass)
{
    m_pAppDataClass = appDataClass;
}

inline GripOperationPtr OdDbGripData::hotGripFunc() const
{
  return m_pHotGripFunc;
}

inline void OdDbGripData::setHotGripFunc( GripOperationPtr pf )
{
  m_pHotGripFunc = pf;
}

inline GripOperationPtr OdDbGripData::hoverFunc() const
{
  return m_pHoverFunc;
}

inline void OdDbGripData::setHoverFunc( GripOperationPtr pf )
{
  m_pHoverFunc = pf;
}

inline GripWorldDrawPtr OdDbGripData::worldDraw() const
{
  return m_pWorldDraw;
}

inline void OdDbGripData::setWorldDraw( GripWorldDrawPtr pf )
{
  m_pWorldDraw = pf;
}

inline GripViewportDrawPtr OdDbGripData::viewportDraw() const
{
  return m_pViewportDraw;
}

inline void OdDbGripData::setViewportDraw( GripViewportDrawPtr pf )
{
  m_pViewportDraw = pf;
}

inline GripOpStatusPtr OdDbGripData::gripOpStatFunc() const
{
  return m_pGripOpStatFunc;
}

inline void OdDbGripData::setGripOpStatFunc( GripOpStatusPtr pf )
{
  m_pGripOpStatFunc = pf;
}

inline GripToolTipPtr OdDbGripData::toolTipFunc() const
{
  return m_pToolTipFunc;
}

inline void OdDbGripData::setToolTipFunc( GripToolTipPtr pf )
{
  m_pToolTipFunc = pf;
}

inline OdGePoint3d* OdDbGripData::alternateBasePoint() const
{
  return m_pAltBasePt;
}

inline void OdDbGripData::setAlternateBasePoint( OdGePoint3d* altBasePt )
{
  m_pAltBasePt = altBasePt;
}

inline unsigned OdDbGripData::bitFlags() const
{
  return m_bitFlags;
}

inline void OdDbGripData::setBitFlags( unsigned flags )
{
  m_bitFlags = flags;
}

inline bool OdDbGripData::skipWhenShared() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kSkipWhenShared);
}

inline void OdDbGripData::setSkipWhenShared( bool skip )
{
  if (skip)
    m_bitFlags |= OdDbGripOperations::kSkipWhenShared;
  else
    m_bitFlags &= ~OdDbGripOperations::kSkipWhenShared;
}

inline bool OdDbGripData::isRubberBandLineDisabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDisableRubberBandLine);
}

inline void OdDbGripData::disableRubberBandLine( bool disable )
{
  if (disable)
    m_bitFlags |= OdDbGripOperations::kDisableRubberBandLine;
  else
    m_bitFlags &= ~OdDbGripOperations::kDisableRubberBandLine;
}

inline bool OdDbGripData::areModeKeywordsDisabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDisableModeKeywords);
}

inline void OdDbGripData::disableModeKeywords( bool disable )
{
  if (disable)
    m_bitFlags |= OdDbGripOperations::kDisableModeKeywords;
  else
    m_bitFlags &= ~OdDbGripOperations::kDisableModeKeywords;
}

inline bool OdDbGripData::drawAtDragImageGripPoint() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDrawAtDragImageGripPoint);
}

inline void OdDbGripData::setDrawAtDragImageGripPoint( bool atDragPoint )
{
  if (atDragPoint)
    m_bitFlags |= OdDbGripOperations::kDrawAtDragImageGripPoint;
  else
    m_bitFlags &= ~OdDbGripOperations::kDrawAtDragImageGripPoint;
}

inline bool OdDbGripData::triggerGrip() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kTriggerGrip);
}

inline void OdDbGripData::setTriggerGrip( bool trigger )
{
  if (trigger)
    m_bitFlags |= OdDbGripOperations::kTriggerGrip | OdDbGripOperations::kSkipWhenShared;
  else
    m_bitFlags &= ~(OdDbGripOperations::kTriggerGrip | OdDbGripOperations::kSkipWhenShared);
}

inline bool OdDbGripData::forcedPickOn() const
{
    return 0 != (m_bitFlags & OdDbGripOperations::kTurnOnForcedPick);
}

inline void OdDbGripData::setForcedPickOn( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kTurnOnForcedPick;
  else
    m_bitFlags &= ~OdDbGripOperations::kTurnOnForcedPick;
}

inline bool OdDbGripData::mapGripHotToRtClk() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kMapGripHotToRtClk);
}

inline void OdDbGripData::setMapGripHotToRtClk( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kMapGripHotToRtClk;
  else
    m_bitFlags &= ~OdDbGripOperations::kMapGripHotToRtClk;
}

inline bool OdDbGripData::gizmosEnabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kGizmosEnabled);
}

inline void OdDbGripData::setGizmosEnabled( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kGizmosEnabled;
  else
    m_bitFlags &= ~OdDbGripOperations::kGizmosEnabled;
}

inline bool OdDbGripData::gripIsPerViewport() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kGripIsPerViewport);
}

inline void OdDbGripData::setGripIsPerViewport(bool on)
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kGripIsPerViewport;
  else
    m_bitFlags &= ~OdDbGripOperations::kGripIsPerViewport;
}

inline GripDimensionPtr OdDbGripData::hoverDimensionFunc() const
{
  return m_pHoverDimensionFunc;
}

inline void OdDbGripData::setHoverDimensionFunc(GripDimensionPtr pf)
{
  m_pHoverDimensionFunc = pf;
}

inline GripDimensionPtr OdDbGripData::hotGripDimensionFunc() const
{
  return m_pHotGripDimensionFunc;
}

inline void OdDbGripData::setHotGripDimensionFunc(GripDimensionPtr pf)
{
  m_pHotGripDimensionFunc = pf;
}

inline GripRtClkHandler OdDbGripData::rtClk() const
{
  return m_pRtClk;
}

inline void OdDbGripData::setRtClk(GripRtClkHandler pf)
{
  m_pRtClk = pf;
}

inline GripInputPointPtr OdDbGripData::inputPointFunc() const
{
  return m_pInputPointFunc;
}

inline void OdDbGripData::setInputPointFunc(GripInputPointPtr pf)
{
  m_pInputPointFunc = pf;
}

#include "TD_PackPop.h"

#endif // _ODDBGRIP_INCLUDED_
