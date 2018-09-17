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
// Basic draw order support for devices based on own display lists

#ifndef ODEXDRAWORDERBASE
#define ODEXDRAWORDERBASE

#include "TD_PackPush.h"

#include "Gs/GsBaseInclude.h"
#include "Gs/GsBaseMaterialView.h"
#include "Gs/GsBaseModel.h"
#include "OdVector.h"
#define STL_USING_MAP
#define STL_USING_SET
#include "OdaSTL.h"

class OdDrawOrderBaseDevice;

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdDrawOrderBaseDeviceMCCallback
{
  typedef void (*onDeleteRequestProc)(OdDrawOrderBaseDevice *pDevice, void *pData);

  virtual void onDeleteRequest(onDeleteRequestProc pReqProc, void *pData) = 0;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdDrawOrderBaseDevice
  : public OdGsBaseVectorizeDevice
  , protected OdDrawOrderBaseDeviceMCCallback
{
  public:
    struct MetafileContext : public OdRxObject
    {
      OdDrawOrderBaseDeviceMCCallback *m_pDevice;
    };
    typedef OdSmartPtr<MetafileContext> MetafileContextPtr;
  protected:
    MetafileContextPtr m_pMetafileContext;
  public:
    ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);

    OdDrawOrderBaseDevice();
    ~OdDrawOrderBaseDevice();

    MetafileContext *metafileContext() { return m_pMetafileContext.get(); }

    virtual void update(OdGsDCRect* pUpdatedRect = 0);

    virtual bool drawOrderForPartialUpdate() const { return false; }

    // Callbacks

    virtual void onMetafileAdded(OdRxObject *pMetafile, bool bVisible, bool bHighlighted) { }
    virtual void onMetafileDeleted(OdRxObject *pMetafile) { }
    virtual void onMetafileHighlightingChanged(OdRxObject *pMetafile, bool bSet) { }
    virtual void onMetafileVisibilityChanged(OdRxObject *pMetafile, bool bSet) { }
    virtual void onMetafileFadingChanged(OdRxObject *pMetafile, bool bSet) { }

    // Default callback impl

    virtual void onDeleteRequest(onDeleteRequestProc pReqProc, void *pData)
    {
      pReqProc(this, pData);
    }
  protected:
    // Process synchronization with all dependent devices/views
    virtual void sync();
    // Preprocess synchronization for all dependent views
    virtual void preprocess();

    virtual void syncView(int id);
    virtual void preprocessView(int id);
    //SEA Draw order inheritance
    enum
    {
      kFirstFlag = OdGsBaseVectorizeDevice::kLastFlag,
      kCheckDrawOrderInheritanceFlag = kFirstFlag << 1
    };
public:
    void setCheckDrawOrderInheritance( bool bSet ) { SETBIT( m_flags, kCheckDrawOrderInheritanceFlag, bSet ); }
    bool isCheckDrawOrderInheritance() const { return GETBIT( m_flags, kCheckDrawOrderInheritanceFlag ); }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdDrawOrderBaseView
  : public OdGsBaseVectorizeView
  , protected OdGsBaseModelReactor
{
  protected:
    enum DrawablesFlags
    {
      kHighlightModified   = (1 << 0), // display list doesn't changed, required only highlighting check
      kPartialRegeneration = (1 << 1), // require partial comparison of display list
      kFullRegeneration    = (1 << 2)  // require full display list rebuild
    };

    struct DrawableInfo
    {
      enum DrawableType
      {
        kTransient = 0, // No cache
        kContainer, // GsContainerNode
        kPersistent // GsEntityNode
      } m_type;
      // Need to check correctness
      OdDbStub* m_drawableId;
      const OdGiDrawable *m_pDrawable;
      const OdGsBaseModel *m_pGsModel;
      // Flags
      OdUInt32 m_flags;
      // Positions in display list
      OdUInt32 m_nFrom, m_nLen;
      // Link index with view drawables holder
      OdUInt32 m_nLink;
    };

    struct ViewInfo
    {
      OdGsView::RenderMode m_mode;
      OdGiRegenType m_regenType;
      OdDbStub* m_annoScale;

      ViewInfo();
    };
    ViewInfo m_viewInfo;

    struct DrawOrderOverlayData : public GsViewOverlayData
    {
      OdArray<DrawableInfo, OdMemoryAllocator<DrawableInfo> > m_drawableInfo;
      OdUInt64Vector m_metafileOrder;
      OdUInt64Vector m_metafileOrderAccum;
      bool m_bMetafileOrderValid;
      OdUInt32 m_nInvalidFrom, m_nInvalidLen;
      OdUInt32 m_nValidFrom, m_nValidLen;
      const OdDrawOrderBaseView* m_pConnectedView;
      bool m_bBreakInheritance;
      DrawOrderOverlayData() : m_bMetafileOrderValid( false ), m_nInvalidFrom(0), m_nInvalidLen( 0 ),
        m_nValidFrom( 0 ), m_nValidLen( 0 ), m_pConnectedView( NULL ), m_bBreakInheritance( false )
      {}
    };
    struct DrawOrderOverlayDataAllocator : public GsViewOverlayDataContainer::Allocator
#ifdef __hpux
    , public GsViewOverlayData
#endif
    {
      GsViewOverlayData *createData() { return new DrawOrderOverlayData(); }
      void deleteData(GsViewOverlayData *pData) { delete static_cast<DrawOrderOverlayData*>(pData); }
    } m_overlayAlloc;
    DrawOrderOverlayData *getOverlayData(OdGsOverlayId nOverlay)
    {
      return static_cast<DrawOrderOverlayData*>(m_overlayData.getOverlayData(nOverlay));
    }

    struct ModelRef { OdUInt32 nRefs; bool bHighlighted; ModelRef() : nRefs(0), bHighlighted(false) { } };
    std::map<OdGsModel*, ModelRef> m_refModels;
  public:
    ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeView);

    OdDrawOrderBaseView();
    ~OdDrawOrderBaseView();

    OdDrawOrderBaseDevice* device();

    OdUInt64Vector *drawOrder(OdGsOverlayId nOverlay)
    {
      DrawOrderOverlayData *pData = getOverlayData(nOverlay);
      return (pData) ? &pData->m_metafileOrder : NULL;
    }
    OdUInt64Vector *drawOrderAccum(OdGsOverlayId nOverlay)
    {
      DrawOrderOverlayData *pData = getOverlayData(nOverlay);
      return (pData) ? &pData->m_metafileOrderAccum : NULL;
    }

    virtual void drawOrderInvalidate();
    void clearDrawOrderAccums();

    virtual void sync();
    virtual void preprocess();

    virtual void extractLights();
    virtual void displayLights(OdGsBaseVectorizer &vec);

    virtual bool add(OdGiDrawable* pSceneGraph, OdGsModel* pModel);
    virtual bool erase(OdGiDrawable* sceneGraph);
    virtual void eraseAll();

    virtual bool onHighlight(OdGsModel *pModel, const OdGiPathNode& path, bool bDoIt, const OdGsView* pView);
    virtual bool onErased(OdGsModel *pModel, OdGiDrawable *pErased, OdGiDrawable *pParent);
    virtual bool onErased(OdGsModel *pModel, OdGiDrawable *pErased, OdDbStub *parentID);

    // Callbacks

    virtual void onMetafileOrderChanged(OdGsOverlayId nOverlay, const OdUInt64 *pFullDrawOrder, OdUInt32 nFullDrawOrder) { }
    virtual void onMetafileOrderChanged(OdGsOverlayId nOverlay, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdUInt64 *pValidOrderPart, OdUInt32 nValidLen) { }
    virtual void onMetafileOrderInheritance( OdGsOverlayId overlayId, const OdGsView* viewportId2 ) {}

  protected:
    bool viewChangesAndUpdate();
    //SEA List cmp
    enum DiffAction
    {
      kNoChanges = 0,
      kAdd, 
      kRemove
    };

    class ActionSequence
    {
    public:
      struct ConvertorAction
      {
        DiffAction m_action;
        OdUInt32 m_nCount;
        ConvertorAction* m_pNext;
        ConvertorAction* m_pPrev;
        ConvertorAction() : m_pNext( NULL ), m_pPrev( NULL )
        {
          m_nCount = 0;
          m_action = kNoChanges;
        }
      };
    public:
      ActionSequence();
      ~ActionSequence();
      void addAction( DiffAction action, OdUInt32 count = 1 );
      ConvertorAction* top() { return m_pFirst; }
      void clear();
    protected:
      ConvertorAction* m_pFirst;
      ConvertorAction* m_pLast;
    };

    class MapBasedComparer
    {
    public:
      MapBasedComparer();
      ~MapBasedComparer();
      void setOriginal( const OdUInt64* pList, OdUInt32 size );

      void diff( const OdUInt64* pList, OdUInt32 size, ActionSequence& as );

    protected:
      struct Options
      {
        OdUInt32 m_pos;
        bool m_bTouched;
        Options( OdUInt32 pos )
        {
          m_pos = pos;
          m_bTouched = false;
        };
        Options()
        {
        };
      };
      std::map< OdUInt64, Options > m_map;
      std::set< OdUInt32 > m_removeSet;
    };
    MapBasedComparer m_mbCmp;
    ActionSequence m_diffActions;
    void listCmp( OdUInt64Vector& originalList,  OdUInt64Vector& newList, OdUInt32 nDrawInfo, DrawableInfo* drawInfo, ActionSequence& result );
  protected:
    //SEA
    virtual bool isContentCompatibleOverlay( const OdDrawOrderBaseView* pView, const DrawOrderOverlayData* pOverlayData, OdGsOverlayId overId ) const;
    virtual bool isMetafilesCompatibleOverlays( OdGsOverlayId overId1, const OdDrawOrderBaseView* pView2 );
  public:
    virtual void breakOverlayCompatibility();
    virtual void breakOverlayCompatibility( OdGsOverlayId nOverlay );
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdDrawOrderBaseVectorizer
  : public OdGsBaseMaterialVectorizer
{
  protected:
    OdUInt64Vector *m_pCurDrawOrderAccum;
    OdRxObjectPtr m_curRecordingMetafile;

    bool m_bDisableNesting, m_bDisableOffLayersVisibility;
  public:
    OdDrawOrderBaseVectorizer();
    ~OdDrawOrderBaseVectorizer();

    OdDrawOrderBaseDevice* device();
    OdDrawOrderBaseView& view() const;

    virtual void drawOrderQuery(bool bContainer, bool bDisableNested = false);

    OdUInt64Vector *drawOrderAccum(OdGsOverlayId nOverlay) { return view().drawOrderAccum(nOverlay); }
    OdUInt64Vector *activeDrawOrderAccum() { return m_pCurDrawOrderAccum; }

    virtual void displayLights();

    // Vectorizer overrides

    virtual void display(bool update);

    virtual void switchOverlay(OdGsOverlayId overlayId);

    // Metafiles

    virtual OdRxObjectPtr newGsMetafile();
    virtual void beginMetafile(OdRxObject* pMetafile);
    virtual void endMetafile(OdRxObject* pMetafile);
    virtual void playMetafile(const OdRxObject* pMetafile);

    virtual void onMetafileAdded(OdRxObject *pMetafile, bool bVisible, bool bHighlighted);
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdDrawOrderMetafileBase : public OdRxObject
{
  enum MetafileFlagsBase
  {
    kReserved = (1 << 0),

    kLastFlag = (kReserved << 1)
  };

  OdIntPtr m_pPtr;
  OdUInt8 m_flags;
  OdUInt8 m_reserved; // At least 2byte padding which could be useful somewhere

  OdDrawOrderMetafileBase()
    : m_pPtr(0)
    , m_flags(0)
  { }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdDrawOrderBaseMetafile : public OdDrawOrderMetafileBase
{
  enum MetafileFlags
  {
    kFilled      = (OdDrawOrderMetafileBase::kLastFlag << 0), // Marks non-empty metafile
    kVisible     = (OdDrawOrderMetafileBase::kLastFlag << 1), // Marks metafiles on enabled layer
    kHighlighted = (OdDrawOrderMetafileBase::kLastFlag << 2), // Marks completely highlighted metafiles
    kNested      = (OdDrawOrderMetafileBase::kLastFlag << 3), // Marks nested metafiles
    kFaded       = (OdDrawOrderMetafileBase::kLastFlag << 4), // Marks faded metafiles

    kLastFlag    = (kFaded << 1)
  };

  OdDrawOrderBaseDevice::MetafileContextPtr m_pContext;
  OdUInt64 m_visualId;
  OdGsLayerNode *m_pLayer;

  OdDrawOrderBaseMetafile();
  ~OdDrawOrderBaseMetafile();

  bool isFilled() const { return GETBIT(m_flags, kFilled); }
  void setFilled() { SETBIT_1(m_flags, kFilled); }

  bool isVisible() const { return GETBIT(m_flags, kVisible); }
  void setVisible(bool bSet) { SETBIT(m_flags, kVisible, bSet); }

  bool isHighlighted() const { return GETBIT(m_flags, kHighlighted); }
  void setHighlighted(bool bSet) { SETBIT(m_flags, kHighlighted, bSet); }

  bool isNested() const { return GETBIT(m_flags, kNested); }
  void setNested() { SETBIT_1(m_flags, kNested); }

  bool isFaded() const { return GETBIT(m_flags, kFaded); }
  void setFaded(bool bSet) { SETBIT(m_flags, kFaded, bSet); }

  static void onMetafileDeleteRequestProc(OdDrawOrderBaseDevice *pDevice, void *pData);

  static OdRxObjectPtr createObject(OdDrawOrderBaseDevice::MetafileContext *pContext);
};

#include "TD_PackPop.h"

#endif // ODEXDRAWORDERBASE
