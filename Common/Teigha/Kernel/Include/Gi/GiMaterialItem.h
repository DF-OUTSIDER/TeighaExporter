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

// Material data entry for Gs material cache and textures processing.

#ifndef __ODGIMATERIALITEM_H__
#define __ODGIMATERIALITEM_H__

#include "GiExport.h"
#include "GiImage.h"
#include "GiMaterial.h"
#include "GiCommonDraw.h"

#include "RxObject.h"

#include "TD_PackPush.h"

class OdDbStub;

/** \details
    Internal texture data for device support.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureData : public OdRxObject
{
  public:
    // Simple variant type for external device information

    /** \details
      Simple variant type for storing external device information.
    */
    struct DevDataVariant
    {
      enum DevDataVariantType
      {
        DevDataVariantType_Void = 0,
        DevDataVariantType_Ptr,
        DevDataVariantType_RxObject
      };

      DevDataVariant() : m_ddvType(DevDataVariantType_Void), m_pVoid(NULL)
      {
      }
      DevDataVariant(void *ptr) : m_ddvType(DevDataVariantType_Ptr), m_pVoid(ptr)
      {
        if (m_pVoid == NULL)
          m_ddvType = DevDataVariantType_Void;
      }
      DevDataVariant(OdRxObjectPtr ptr) : m_ddvType(DevDataVariantType_RxObject), m_pVoid(NULL)
      {
        m_pRxObject = ptr;
      }

      DevDataVariantType type() const
      {
        return m_ddvType;
      }
      void *getPtr() const
      {
        return m_pVoid;
      }
      OdRxObjectPtr getRxObject() const
      {
        return m_pRxObject;
      }

      DevDataVariant &operator =(void *ptr)
      {
        return setPtr(ptr);
      }
      DevDataVariant &operator =(OdRxObjectPtr ptr)
      {
        return setRxObject(ptr);
      }

      DevDataVariant &setPtr(void *ptr)
      {
        clear();
        if (ptr != NULL)
        {
          m_ddvType = DevDataVariantType_Ptr;
          m_pVoid = ptr;
        }
        return *this;
      }
      DevDataVariant &setRxObject(OdRxObjectPtr ptr)
      {
        clear();
        m_ddvType = DevDataVariantType_RxObject;
        m_pRxObject = ptr;
        return *this;
      }

      void clear()
      {
        m_ddvType = DevDataVariantType_Void;
        m_pVoid = NULL;
        if (m_pRxObject.isNull())
          m_pRxObject.release();
      }

    protected:
      DevDataVariantType m_ddvType;
      void *m_pVoid;
      OdRxObjectPtr m_pRxObject;
    };

  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureData);
    friend class OdGiModuleObject;
    
    /** \details
      Set texture pixels from RGBA array.
      \param pDeviceInfo [in]  Optional external device information.
      \param image [in]  Input BGRA image.
    */
    virtual void setTextureData(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, const OdGiImageBGRA32 &image) = 0;
    /** \details
      Get texture pixels as array of RGBA colors.
      \param data [out]  Output BGRA colors array.
      \param width [out]  Width of texture.
      \param height [out]  Height of texture.
    */
    virtual void textureData(OdGiPixelBGRA32Array &data, OdUInt32 &width, OdUInt32 &height) const = 0;
    
    /** \details
      Return true if texture data was previously initialized.
    */
    virtual bool haveData() const = 0;

    /** \details
      Create default material texture data implementation that stores color array.
    */
    static OdSmartPtr<OdGiMaterialTextureData> createDefaultTextureDataImplementation();
    /** \details
      Return description of default material texture data implementation.
    */
    static OdRxClass *defaultTextureDataImplementationDesc();

  private:
    /*!DOM*/
    static void rxInitDefaultTextureDataImplementation();
    /*!DOM*/
    static void rxUninitDefaultTextureDataImplementation();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureData object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureData> OdGiMaterialTextureDataPtr;

/** \details
    Protocol extension for OdGiMaterialTextureEntry texture loading information.

    \sa
    TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureLoadPE : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureLoadPE);

    OdGiMaterialTextureLoadPE();

    /** \details
      Called by texture loader before file loading.
      Can be used for file name correction before texture loading.
    */
    virtual void startTextureLoading(OdString &fileName, OdDbBaseDatabase* pDb) = 0;

    /** \details
      Called by texture loader after file loading, only if texture loading success.
    */
    virtual void textureLoaded(const OdString &fileName, OdDbBaseDatabase* pDb) = 0;
    /** \details
      Called by texture loader after file loading, only if texture loading failed.
    */
    virtual void textureLoadingFailed(const OdString &fileName, OdDbBaseDatabase* pDb) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureLoadPE object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureLoadPE> OdGiMaterialTextureLoadPEPtr;

class OdGiMaterialTextureEntry;

/** \details
    Extends device by texture loading functionality.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureLoaderExt : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureLoaderExt);

    /** \details
      Returns true if device supports loading of this kind of texture.
      \param pTexture [in]  Pointer to material texture.
    */
    virtual bool allowTextureLoading(OdGiMaterialTexturePtr pTexture) = 0;

    /** \details
      Loads material texture.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param pTexture [in]  Pointer to material texture.
    */
    virtual OdGiMaterialTextureDataPtr loadTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, OdGiMaterialTexturePtr pTexture) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureLoaderExt object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureLoaderExt> OdGiMaterialTextureLoaderExtPtr;

/** \details
    Manage scene textures.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureManager : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureManager);

    /** \details
        Management type.
    */
    enum ManageType
    {
      kFileTexturesOnly = 0, // @def
      kFileAndProceduralTextures, // not efficient for search
      kDisable // only loading from LoaderExt will be active
    };

    /** \details
      Sets type of textures management.
      \param type [in]  New management type.
    */
    virtual void setManageType(ManageType type) = 0;
    /** \details
      Returns current management type.
    */
    virtual ManageType manageType() const = 0;

    /** \details
      Sets texture loading extension.
      \param pExt [in]  New extentsion pointer.
    */
    virtual void setMaterialLoaderExt(OdGiMaterialTextureLoaderExt *pExt) = 0;
    /** \details
      Returns current texture loading extension.
    */
    virtual OdGiMaterialTextureLoaderExtPtr materialLoaderExt() const = 0;

    /** \details
      Search texture in cache.
      \param matMap [in]  Material map.
    */
    virtual OdGiMaterialTextureDataPtr searchTexture(const OdGiMaterialMap &matMap) = 0;
    /** \details
      Search texture in cache.
      \param pTexture [in]  Material texture pointer.
    */
    virtual OdGiMaterialTextureDataPtr searchTexture(OdGiMaterialTexturePtr pTexture) = 0;
    /** \details
      Search texture in cache.
      \param fileName [in]  Texture file name.
    */
    virtual OdGiMaterialTextureDataPtr searchTexture(const OdString &fileName) = 0;

    /** \details
      Try to load texture if it is isn't available in cache.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param matMap [in]  Material map.
    */
    virtual OdGiMaterialTextureDataPtr tryToLoad(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, const OdGiMaterialMap &matMap) = 0;
    /** \details
      Try to load texture if it is isn't available in cache.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param pTexture [in]  Material texture pointer.
    */
    virtual OdGiMaterialTextureDataPtr tryToLoad(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, OdGiMaterialTexturePtr pTexture) = 0;
    /** \details
      Try to load texture if it is isn't available in cache.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation.
      \param giCtx [in]  Gi context.
      \param pEntry [in]  Texture entry.
      \param fileName [in]  Texture file name.
    */
    virtual OdGiMaterialTextureDataPtr tryToLoad(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTextureEntry *pEntry, const OdString &fileName) = 0;

    /** \details
      Links texture to current cache.
      \param matMap [in]  Material map.
      \param pData [in]  Texture data pointer
    */
    virtual void linkTexture(const OdGiMaterialMap &matMap, OdGiMaterialTextureDataPtr pData) = 0;
    /** \details
      Links texture to current cache.
      \param pTexture [in]  Material texture pointer.
      \param pData [in]  Texture data pointer
    */
    virtual void linkTexture(OdGiMaterialTexturePtr pTexture, OdGiMaterialTextureDataPtr pData) = 0;
    /** \details
      Links texture to current cache.
      \param fileName [in]  Texture file name.
      \param pData [in]  Texture data pointer
    */
    virtual void linkTexture(const OdString &fileName, OdGiMaterialTextureDataPtr pData) = 0;

    /** \details
      Returns true if specified texture data is already available in cache.
      \param pData [in]  Texture data pointer
    */
    virtual bool isAvailable(const OdGiMaterialTextureData *pData) const = 0;

    /** \details
      Removes specified texture data from cache.
      \param pData [in]  Texture data pointer
    */
    virtual void unlinkTexture(OdGiMaterialTextureData *pData) = 0;

    /** \details
      Clears cache.
    */
    virtual void clear() = 0;

    /** \details
      Returns number of texture linkages.
    */
    virtual OdUInt32 numTextureEntries() const = 0;

    /** \details
      Returns texture linkage by index.
      \param nIndex [in]  Texture linkage index.
    */
    virtual OdGiMaterialTexturePtr textureEntryKey(OdUInt32 nIndex) const = 0;

    /** \details
      Returns texture linkage data by index.
      \param nIndex [in]  Texture linkage index.
    */
    virtual OdGiMaterialTextureDataPtr textureEntryData(OdUInt32 nIndex) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureManager object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureManager> OdGiMaterialTextureManagerPtr;

/** \details
    Material texture entry that contains, generates, and loads texture data.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialTextureEntry : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialTextureEntry);

    /** \details
      Return pointer to device-dependent texture data container.
    */
    virtual OdGiMaterialTextureDataPtr textureData() const = 0;
    /** \details
      Return true if texture data is initialized.
    */
    virtual bool isTextureInitialized() const = 0;

    /** \details
      Return width of procedurally generated textures.
    */
    virtual OdUInt32 proceduralTextureWidth() const = 0;
    /** \details
      Set width of procedurally generated textures.
    */
    virtual void setProceduralTextureWidth(OdUInt32 width) = 0;
    /** \details
      Return height of procedurally generated textures.
    */
    virtual OdUInt32 proceduralTextureHeight() const = 0;
    /** \details
      Set height of procedurally generated textures.
    */
    virtual void setProceduralTextureHeight(OdUInt32 height) = 0;
    /** \details
      Set width and height of procedurally generated textures.
    */
    virtual void setProceduralTextureResolution(OdUInt32 width, OdUInt32 height) = 0;
    
    /** \details
      Return procedural textures quality coefficient.
    */
    virtual double proceduralTextureQuality() const = 0;
    /** \details
      Set procedural textures quality coefficient.
    */
    virtual void setProceduralTextureQuality(double coef) = 0;

    /** \details
      Load texture from material map.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL)
      \param giCtx [in]  Gi context.
      \param matMap [in]  Input material map reference.
      \param pManager [in]  Optional textures manager.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, const OdGiMaterialMap &matMap, OdGiMaterialTextureManager *pManager = NULL) = 0;

    /** \details
      Load texture from material texture pointer.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL).
      \param giCtx [in]  Gi context.
      \param pTexture [in]  Pointer to material texture.
      \param pManager [in]  Optional textures manager.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, OdGiMaterialTexturePtr pTexture, OdGiMaterialTextureManager *pManager = NULL) = 0;
    // Load texture from raster file
    /** \details
      Load texture from raster file.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL).
      \param giCtx [in]  Gi context.
      \param fileName [in]  Name of file that contains raster image.
      \param pManager [in]  Optional textures manager.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, const OdString &fileName, OdGiMaterialTextureManager *pManager = NULL) = 0;
    /** \details
      Generate texture from material opacity value.
      \param pDeviceInfo [in]  Optional external device information.
      \param pTexDataImpl [in]  Description of texture data class implementation (must not be NULL).
      \param giCtx [in]  Gi context.
      \param opacity [in]  Material opacity value (0.0 - 1.0).
      \param pManager [in]  Optional textures manager.
    */
    virtual bool setGiMaterialTexture(OdGiMaterialTextureData::DevDataVariant pDeviceInfo, OdRxClass *pTexDataImpl, const OdGiContext &giCtx, double opacity, OdGiMaterialTextureManager *pManager = NULL) = 0;

    /** \details
      Sets texture data explicitly.
      \param pTextureData [in]  Pointer to texture data.
      \param pManager [in]  Optional textures manager.
    */
    virtual void setTextureData(OdGiMaterialTextureData *pTextureData, OdGiMaterialTextureManager *pManager = NULL) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialTextureEntry object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialTextureEntry> OdGiMaterialTextureEntryPtr;

/** \details
    Container item for material data storage.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialItem : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialItem);

    /** \details
      Return diffuse texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr diffuseTexture() const = 0;
    /** \details
      Return diffuse texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr diffuseTexture() = 0;
    /** \details
      Create and return diffuse texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createDiffuseTexture() = 0;
    /** \details
      Destroy diffuse texture.
    */
    virtual void removeDiffuseTexture() = 0;
    /** \details
      Return true if diffuse texture is created.
    */
    virtual bool haveDiffuseTexture() const = 0;

    // Container data

    /** \details
      Return pointer to cached data object.
    */
    virtual OdRxObjectPtr cachedData() const = 0;
    /** \details
      Set pointer to cached data object.
    */
    virtual void setCachedData(OdRxObjectPtr data) = 0;

    /** \details
      Return material ID.
    */
        virtual const OdDbStub *materialId() const = 0;
    /** \details
      Set material ID.
    */
    virtual void setMaterialId(const OdDbStub *matId = NULL) = 0;
    /** \details
      Test whether stored material ID is equal to matId.
    */
    virtual bool isMaterialIdValid(const OdDbStub *matId) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialItem object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialItem> OdGiMaterialItemPtr;

/** \details
    Container item for material data store (for render devices).

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMaterialRenderItem : public OdGiMaterialItem
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMaterialRenderItem);

    /** \details
      Return specular texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr specularTexture() const = 0;
    /** \details
      Return specular texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr specularTexture() = 0;
    /** \details
      Create and return specular texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createSpecularTexture() = 0;
    /** \details
      Destroy specular texture.
    */
    virtual void removeSpecularTexture() = 0;
    /** \details
      Return true if specular texture created.
    */
    virtual bool haveSpecularTexture() const = 0;

    /** \details
      Return reflection texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr reflectionTexture() const = 0;
    /** \details
      Return reflection texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr reflectionTexture() = 0;
    /** \details
      Create and return reflection texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createReflectionTexture() = 0;
    /** \details
      Destroy reflection texture.
    */
    virtual void removeReflectionTexture() = 0;
    /** \details
      Return true if reflection texture created.
    */
    virtual bool haveReflectionTexture() const = 0;

    /** \details
      Return opacity texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr opacityTexture() const = 0;
    /** \details
      Return opacity texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr opacityTexture() = 0;
    /** \details
      Create and return opacity texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createOpacityTexture() = 0;
    /** \details
      Destroy opacity texture.
    */
    virtual void removeOpacityTexture() = 0;
    /** \details
      Return true if opacity texture created.
    */
    virtual bool haveOpacityTexture() const = 0;

    /** \details
      Return bump texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr bumpTexture() const = 0;
    /** \details
      Return bump texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr bumpTexture() = 0;
    /** \details
      Create and return bump texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createBumpTexture() = 0;
    /** \details
      Destroy bump texture.
    */
    virtual void removeBumpTexture() = 0;
    /** \details
      Return true if bump texture created.
    */
    virtual bool haveBumpTexture() const = 0;

    /** \details
      Return refraction texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr refractionTexture() const = 0;
    /** \details
      Return refraction texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr refractionTexture() = 0;
    /** \details
      Create and return refraction texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createRefractionTexture() = 0;
    /** \details
      Destroy refraction texture.
    */
    virtual void removeRefractionTexture() = 0;
    /** \details
      Return true if refraction texture created.
    */
    virtual bool haveRefractionTexture() const = 0;

    /** \details
      Return normal map texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr normalMapTexture() const = 0;
    /** \details
      Return normal map texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr normalMapTexture() = 0;
    /** \details
      Create and return normal map texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createNormalMapTexture() = 0;
    /** \details
      Destroy normal map texture.
    */
    virtual void removeNormalMapTexture() = 0;
    /** \details
      Return true if normal map texture created.
    */
    virtual bool haveNormalMapTexture() const = 0;

    /** \details
      Return emission texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr emissionTexture() const = 0;
    /** \details
      Return emission texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr emissionTexture() = 0;
    /** \details
      Create and return emisiion texture pointer.
    */
    virtual OdGiMaterialTextureEntryPtr createEmissionTexture() = 0;
    /** \details
      Destroy emission texture.
    */
    virtual void removeEmissionTexture() = 0;
    /** \details
      Return true if emission texture created.
    */
    virtual bool haveEmissionTexture() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMaterialRenderItem object pointers.
		
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMaterialRenderItem> OdGiMaterialRenderItemPtr;

#include "TD_PackPop.h"

#endif // __ODGIMATERIALITEM_H__
