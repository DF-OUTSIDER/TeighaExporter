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

#ifndef __OD_GS_BASE_MATERIAL_VIEW__
#define __OD_GS_BASE_MATERIAL_VIEW__

#include "Gs/GsBaseInclude.h"
#include "Gs/GsExtAccum.h"
#include "Gi/GiMapperItem.h"
#include "Gi/GiMaterialItem.h"

#include "TD_PackPush.h"

class OdGsMaterialNode;
/** <group !!RECORDS_TD_APIRef>
*/
struct OdGsMaterialTextureDataFiler
{
  virtual bool saveClientMaterialTextureData(const OdGiMaterialTextureData * /*pTextureData*/, OdGsFiler * /*pFiler*/)
  {
    return false;
  }
  virtual OdGiMaterialTextureDataPtr loadClientMaterialTextureData(OdGsFiler * /*pFiler*/)
  {
    return OdGiMaterialTextureDataPtr();
  }
};

/** \details
    This class provides material support for OdGsBaseVectorizeView.

    \remarks
    OdGsBaseMaterialView works only with diffuse material/mapping channel and not applicable for
    complete rendering devices.
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialVectorizer : public OdGsBaseVectorizer, public OdGsMaterialTextureDataFiler
{
  public:
    enum MaterialViewFlags
    {
      kProcessMappers               = 1,        // Process material mappers
      kProcessMaterials             = (1 << 1), // Process materials

      kProcessMappersAndMaterials   = 3,        // Process both material mappers and materials

      kMappersForRender             = (1 << 2), // OdGiMapperRenderItem instead of OdGiMapperItem
      kMaterialsForRender           = (1 << 3), // OdGiMaterialRenderItem instead of OdGiMaterialItem

      kMappersAndMaterialsForRender = 12,       // kMappersForRender | kMaterialsForRender

      kProcessForRender             = 15,       // kProcessMappersAndMaterials | kMappersAndMaterialsForRender

      // #5127 fix. Delay cache implementation.
      kEnableDelayCache             = (1 << 4)  // Enable cache, which drop extents data after drawable extents computation
    };

    // Cache structures
    enum DelayCacheEntryType
    {
      kInternal = 0,
      kUser
    };
    struct DelayCacheEntry
    {
      DelayCacheEntry() : m_pNext(NULL) { }
      virtual ~DelayCacheEntry() { }

      DelayCacheEntry *m_pNext;

      // Don't return kInternal here if you want get callback
      virtual DelayCacheEntryType internalType() const { return kUser; }

      DelayCacheEntry *nextEntry() { return m_pNext; }
      void setNextEntry(DelayCacheEntry *pNext) { m_pNext = pNext; }

      virtual void play(OdGsBaseMaterialVectorizer &mView) const
      {
        mView.playDelayCacheEntry(this);
      }
    };
    friend class OdDefDelayCacheEntryMap;
    friend class OdManageMViewPerDrawableData;
  private:
    bool                m_bMaterialCommited;    // Material already emitted by GsModel
    struct ExtentsContainer
    {
      const OdGiDrawable *m_pCurrentDrawable;
      OdGeExtents3d       m_Extents;
      enum Flags
      {
        kExtentsValid        = (1 << 0),
        kExtentsAwaitingCalc = (1 << 1),
        kLastFlag            = kExtentsAwaitingCalc
      };
      ExtentsContainer() : m_pCurrentDrawable(NULL) {}
      void clear() { m_pCurrentDrawable = NULL; }
    };
    struct DelayCache
    {
      const OdGiMapper *m_pCurMapper,   *m_pCopyMapper;
      const OdDbStub   *m_pCurMaterial, *m_pCopyMaterial;
      DelayCacheEntry   m_Cache;
      DelayCacheEntry  *m_pTail;

      DelayCache() : m_pCurMapper(NULL), m_pCopyMapper(NULL)
                   , m_pCurMaterial(NULL), m_pCopyMaterial(NULL)
                   , m_pTail(&m_Cache)
      {
      }

      void clear();
    };
    struct MViewPerDrawableData : public ExtentsContainer
    {
      DelayCache   m_DelayCache;
      OdGeMatrix3d m_ModelTransform;
      OdUInt32     m_flags;
      enum Flags
      {
        kMapperNeedExtents = (ExtentsContainer::kLastFlag << 1), // Mapper awaiting current drawable extents
        kBaseLevel         = (ExtentsContainer::kLastFlag << 2),
        kHasModelTransform = (ExtentsContainer::kLastFlag << 3)
      };
      MViewPerDrawableData()
        : m_flags(0)
      {
      }
      ~MViewPerDrawableData()
      {
      }
      void clear()
      {
        ExtentsContainer::clear();
        SETBIT_0(m_flags, ~kBaseLevel);
      }
    } *m_pCurMvDrawableData, m_mvBaseDrawableData, m_mvGlobalDrawableDataInitializer;
  protected:
    OdUInt32            m_uMaterialViewFlags;   // Set of material view flags
    OdGiMaterialItemPtr m_pCurrentMaterialItem; // Current material
    OdGiMapperItemPtr   m_pCurrentMapperItem;   // Current mapper
    OdGiMaterialTraitsData m_nullMaterialTraitsData;
    OdGiMaterialItemPtr m_pNullMaterialItem;
  public:
    explicit OdGsBaseMaterialVectorizer(OdUInt32 uMaterialViewFlags = kProcessMappersAndMaterials);
    ~OdGsBaseMaterialVectorizer();

    // OdGsBaseVectorizerView overrides

    virtual void onTraitsModified();

    virtual void beginMetafile(OdRxObject* pMetafile);
    virtual void endMetafile(OdRxObject* pMetafile);
    virtual void beginViewVectorization();
    virtual void endViewVectorization();

    virtual void processMaterialNode(OdDbStub *materialId, OdGsMaterialNode *node);
    virtual bool saveMaterialCache(const OdGsMaterialNode *pNode, OdGsFiler *pFiler);
    virtual bool loadMaterialCache(OdGsMaterialNode *pNode, OdGsFiler *pFiler);

    virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable *pDrawable);

    // OdGsBaseMaterialView methods

    /** \details
      Returns current material item.
    */
    OdGiMaterialItemPtr currentMaterial() const;
    /** \details
      Sets current material item.
      \param pMaterial [in]  New material item.
    */
    void resetCurrentMaterial(OdGiMaterialItemPtr pMaterial);
    /** \details
      Returns true if and only if materials support enabled.
    */
    bool isMaterialEnabled() const;
    /** \details
      Returns true if and only if current material available.
    */
    bool isMaterialAvailable() const;
    /** \details
      Removes current material item.
    */
    void resetCurrentMaterial();
    /** \details
      Returns current material mapper item.
      \param bForCoords [in]  Set to true if mapper item pointer will be used for texture coordinates calculations.
    */
    OdGiMapperItemPtr currentMapper(bool bForCoords = true) const;
    /** \details
      Sets current material mapper item.
      \param pMapper [in]  New material mapper item.
    */
    void resetCurrentMapper(OdGiMapperItemPtr pMapper);
    /** \details
      Returns true if and only if mappers support enabled.
    */
    bool isMapperEnabled() const;
    /** \details
      Returns true if and only if current mapper item available.
    */
    bool isMapperAvailable() const;

    /** \details
      Returns true if and only if delay cache is enabled and drawable extents is not available.
    */
    bool isMappingDelayed() const;

    // Output callbacks

    /** \details
      Can be overrided by device for setup additional transform matrix to material mapper.
      \param dm [out]  Transform matrix.
      \remarks
      Return true if transform matrix initialized.
    */
    virtual bool getDeviceMapperMatrix(OdGeMatrix3d &dm) const;
    /** \details
      Test does current material need to be processed.
      \param materialId [in]  Material id.
      \remarks
      Return true material need to be skipped.
      As default returns true for 2d optimized, wireframe and hidden render modes and if materialId null.
    */
    virtual bool skipMaterialProcess(OdDbStub *materialId) const;
    /** \details
      Can be overrided by device for process null materials.
    */
    virtual void nullMaterialStub();
    /** \details
      Called by OdGsBaseMaterialView for initialize/reinitialize material cache.
      \param prevCache [in]  Previous cache, if available.
      \param materialId [in]  Material id.
      \param materialData [in]  Current material traits.
      \remarks
      As default returns null pointer.
    */
    virtual OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData);
    /** \details
      Called by OdGsBaseMaterialView for render material cache.
      \param pCache [in]  Material cache.
      \param materialId [in]  Material id.
      \remarks
      As default does nothing.
    */
    virtual void renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId);

    /** \details
      Called by delay cache entry as default handler.
      \param pEntry [in]  Pointer to caller entry.
    */
    virtual void playDelayCacheEntry(const DelayCacheEntry *pEntry);
    /** \details
      Sets user entry to delay cache.
      \param pEntry [in]  Pointer to newly created cache entry.
    */
    void appendDelayCacheEntry(DelayCacheEntry *pEntry);
    /** \details
      Called by delay cache writer if mapping entry was changed.
      \remarks
      Can be used for link cache markers with texture coordinates which must be recalculated after extents
      will be calculated.
    */
    virtual void mapperChangedForDelayCache();
    /** \details
      Called by delay cache writer after execution of all cache entries.
      \remarks
      Can be overrided for clear device's internal caches. If you want clear caches before execution,
      you could override computeDelayedExtents method instead of delayCacheProcessed.
    */
    virtual void delayCacheProcessed(const OdGiDrawable *pDrawable);

    bool saveMaterialItem(const OdGiMaterialItem *pMatItem, OdGsFiler *pFiler);
    OdGiMaterialItemPtr loadMaterialItem(OdGsFiler *pFiler);
    static bool saveMaterialTexture(const OdGiMaterialTexture *pTexture, OdGsFiler *pFiler);
    static OdGiMaterialTexturePtr loadMaterialTexture(OdGsFiler *pFiler);
    static bool saveMaterialTextureManager(const OdGiMaterialTextureManager *pManager, OdGsFiler *pFiler, OdGsMaterialTextureDataFiler *pSaver);
    static bool loadMaterialTextureManager(OdGiMaterialTextureManager *pManager, OdGsFiler *pFiler, OdGsMaterialTextureDataFiler *pSaver);
    virtual bool saveClientMaterialCache(const OdRxObject* pMtl, OdGsFiler *pFiler);
    virtual OdRxObjectPtr loadClientMaterialCache(OdGsFiler *pFiler, OdGiMaterialItem *pMatItem);
  protected:
    /** \details
      Called internally if extents needed for current drawable (delay cache disabled).
      \param pDrawable [in]  Pointer to current drawable.
      \param extents [out]  Computed extents.
      \remarks
      Returns true if and only if computed extents valid.
      Can be overrided by device if it needs more fast implementation.
    */
    virtual bool computeDrawableExtents(const OdGiDrawable *pDrawable, OdGeExtents3d &extents);
    /** \details
      Called internally if extents needed for current drawable (if delay cache enabled).
      \param pDrawable [in]  Pointer to current drawable.
      \param extents [out]  Computed extents.
      \remarks
      Returns true if and only if computed extents valid.
      Can be overrided by device if it needs more fast implementation.
    */
    virtual bool computeDelayedExtents(const OdGiDrawable *pDrawable, OdGeExtents3d &extents);
  private:
    void updateMViewModelTransform();
    void computeDrawableExtents();
    bool tryGsModelExtents(OdGeExtents3d &extents) const;
  protected:
    OdGiMapperItemPtr _itlCreateMapperItem() const;
    OdGiMaterialItemPtr _itlCreateMaterialItem() const;
};

/** \details
    Implements OdGsBaseMaterialVectorizer pseudo constructor.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialViewPC : public OdGsBaseMaterialVectorizer
{
  protected:
    void resetConstructorFlags(OdUInt32 uMaterialViewFlags);
  public:
    OdGsBaseMaterialViewPC() { }
};

/** \details
    Joins OdGsBaseVectorizeView and OdGsBaseMaterialVectorizer classes with default behavior.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialView : public OdGsBaseVectorizeViewJoin<OdGsBaseMaterialViewPC>
{
  public:
    explicit OdGsBaseMaterialView(OdUInt32 uMaterialViewFlags = kProcessMappersAndMaterials)
    {
      resetConstructorFlags(uMaterialViewFlags);
    }
};

/** \details
    Joins OdGsBaseVectorizeView and OdGsBaseMaterialVectorizer classes with multithread behavior.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialViewMT : public OdGsBaseVectorizeViewJoinMT<OdGsBaseMaterialViewPC>
{
  public:
    explicit OdGsBaseMaterialViewMT(OdUInt32 uMaterialViewFlags = kProcessMappersAndMaterials)
    {
      resetConstructorFlags(uMaterialViewFlags);
    }

    bool isMTView() const
    {
      return (&view() != this);
    }
};

#include "TD_PackPop.h"

#endif // __OD_GS_BASE_MATERIAL_VIEW__
