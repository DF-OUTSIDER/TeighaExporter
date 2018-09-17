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
// XmlGlesLoader.h

#ifndef OD_XML_GLES_LOADER_H_
#define OD_XML_GLES_LOADER_H_

#include "TD_PackPush.h"

#include "Ge/GeMatrix3d.h"
#include "OdBinaryData.h"
#include "TrVisRendition.h"
#include "TrVisUniqueIdMapping.h"
#include "OdFakeDbStub.h"

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdGLES2MetafileOrderDef
{ 
  OdTrVisViewportId m_viewportId;
  OdTrVisOverlayId m_overlayId;
  OdUInt32 m_nInvalidFrom;
  OdUInt32 m_nInvalidLen;
  OdUInt64Vector m_metafileOrder;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
    m_nInvalidFrom = 0;
    m_nInvalidLen = 0;
    m_metafileOrder.clear();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdGLES2LightsListDef
{ 
  OdTrVisViewportId m_viewportId;
  float m_ambientLight[4];
  bool m_bDefaultLightsOn;
  OdArray<OdTrVisLightDef, OdMemoryAllocator<OdTrVisLightDef> > m_Defs;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_ambientLight[0] = 0.0f;
    m_ambientLight[1] = 0.0f;
    m_ambientLight[2] = 0.0f;
    m_ambientLight[3] = 0.0f;
    m_bDefaultLightsOn = true;
    m_Defs.clear();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdGLES2BackgroundDef
{ 
  OdTrVisViewportId m_viewportId;
  OdTrVisFlatMetafileContainerPtr m_pStream;
  OdTrVisProgramId m_baseProgramId;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_baseProgramId = kTrVisNegativeId;
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdGLES2ExtentsChangedDef : public OdTrVisExtentsDef
{
  OdTrVisViewportId m_viewportId;
  OdTrVisOverlayId  m_overlayId;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
    OdTrVisExtentsDef::setDefault();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdGLES2OverlayViewParamsOverrideDef
{
  OdTrVisViewportId m_viewportId;
  OdTrVisOverlayId m_overlayId;
  bool m_bOverride;
  OdTrVisViewParamsDef m_viewParams;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
    m_bOverride = false;
    m_viewParams.setDefault();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdGLES2MetafileOrderInheritanceDef
{
  OdTrVisViewportId m_viewportId1;
  OdTrVisOverlayId m_overlayId;
  OdTrVisViewportId m_viewportId2;

  void setDefault()
  {
    m_viewportId1 = OdTrVisViewportId();
    m_viewportId2 = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
  }
};
/** <group !!RECORDS_tkernel_apiref>
*/#define PATH_ENTRY(path, enKey, func) pk_##enKey,
typedef enum {
  pk_None = 0,
# include "PathEntryDefs.h"
  pk_UpperBound,
  pk_Count = pk_UpperBound - 1
} OdEnPathKey;
/** \details
  This interface should provide Id's map sharing between different OdXmlGlesLoader instances
  <group ExRender_Classes>
*/
class OdXmlGlesLoaderIdMapper
{
public:
  /** \details
  Obtain exist or generate new ID
  */
  virtual OdTrVisId getId( const OdAnsiString&, OdTrVisRendition::DataType type ) const = 0;
  /** \details
  Check is ID for specified key exist
  */
  virtual bool hasId( const OdAnsiString&, OdTrVisRendition::DataType type ) const = 0;
  /** \details
  Set special ID value by specified key
  */
  virtual void setSpecialId( const OdAnsiString&, OdTrVisId, OdTrVisRendition::DataType type ) = 0;
  /** \details
  Clear all amps
  */
  virtual void reset() = 0;
  /** \details
  Returns pointer to map <ID>-<OdFakeDbStub>
  */
  virtual OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > getMapFakeDbStub() = 0;
};
/** \details
  Simple implementation of OdXmlGlesLoaderIdMapper interface using OdTrVisUniqueIDMappingImpl
  <group ExRender_Classes>
*/
class OdXmlGesIdMapper : public OdXmlGlesLoaderIdMapper
{
protected:
  OdTrVisUniqueIDMappingImpl< OdAnsiString > m_mappingImpl;
  OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > m_pMapFakeDbStub;
public:
  OdXmlGesIdMapper()
  {
    m_pMapFakeDbStub = new std::map<OdAnsiString, OdSharedPtr<OdFakeDbStub> >();
  }

  ~OdXmlGesIdMapper(){};

  virtual OdTrVisId getId( const OdAnsiString& key, OdTrVisRendition::DataType type ) const
  {
    return m_mappingImpl.getTrVisId( key, type );
  }
  virtual bool hasId( const OdAnsiString& key, OdTrVisRendition::DataType type ) const
  {
    return m_mappingImpl.hasUID( key, type );
  }
  virtual void reset()
  {
    m_mappingImpl.clean();
    m_pMapFakeDbStub->clear();
  }
  virtual void setSpecialId( const OdAnsiString& key, OdTrVisId id, OdTrVisRendition::DataType type )
  {
    m_mappingImpl.setSpecialId( key, id, type );
  }
  virtual OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > getMapFakeDbStub()
  {
    return m_pMapFakeDbStub;
  }
};


/** <group ExRender_Classes>
*/
class OdXmlGlesLoader
{
protected:
  OdTrVisRendition* m_pRendition;

  OdTrVisId m_id; // current Internal ID
  OdXmlGlesLoaderIdMapper* m_pIdMapper;

  //OdSharedPtr<OdGLES2ProgramDef> m_pProgramDef;
  OdSharedPtr<OdTrVisMaterialDef> m_pMaterialDef;
  struct TextureDescription
  {
    OdTrVisTexture::Format fmt;
    OdUInt32 nWidth, nHeight, nAlign, nScanLineLength;
    bool isValid;
    OdTrVisTexture::Format pal_fmt;
    OdUInt32 pal_nWidth, pal_nAlign, pal_nScanLineLength;
  };
  TextureDescription m_curTexture;
  OdSharedPtr<OdTrVisTextureDef> m_pTextureDef;
  OdSharedPtr<OdTrVisViewportDef> m_pViewportDef;
  OdUInt32 m_kindOfModViewportModified;
  OdInt32  m_nViewportInsertedIndex;
  OdSharedPtr<OdTrVisOverlayDef> m_pOverlayDef;
  OdSharedPtr<OdGLES2OverlayViewParamsOverrideDef> m_pOverlayViewParamsOverrideDef;
  OdSharedPtr<OdTrVisViewParamsDef> m_pViewParamsDef;
  OdSharedPtr<OdTrVisMetafileDef> m_pMetafileDef;
  OdArray<float, OdMemoryAllocator<float> > m_ArrayOffset;
  OdSharedPtr<OdTrVisFlatMetafileContainerWriter> m_pWriter;
  OdBaseMetafileOutputStream m_streamWriter;
  OdSharedPtr<OdTrVisSurfaceDef> m_pSurfaceDef;
  OdSharedPtr<OdGLES2ExtentsChangedDef> m_pExtentsDef;
  OdSharedPtr<OdGLES2MetafileOrderDef> m_pMetafileOrderDef;
  OdSharedPtr<OdGLES2LightsListDef> m_pLightsListDef;
  OdSharedPtr<OdGLES2BackgroundDef> m_pBackgroundDef;
  OdSharedPtr<OdTrVisVisualStyle> m_pVisualStyleDef;
  OdSharedPtr<OdGLES2MetafileOrderInheritanceDef> m_pMetafileOrderInheritance;
  OdTrVisVisualStyleProperty* m_pVisualStyleProperty;
  OdGiVariant::VariantType m_typeVisualStyle;
  OdSharedPtr<OdTrVisLayerDef> m_pLayerDef;
  OdUInt8 m_metafileChangedFlag;
  OdUInt32 m_mfUserEntryNumData;

  // cache of fake DbStub
  OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > m_pMapFakeDbStub;

  OdTrVisMetafileContainer* metafile();
  enum OdTrVisArrayWrapper::Type m_typeElement;
  OdUInt32 m_indexElement,
           m_numDataElement;
  OdArray<OdTrVisDefProcMark, OdMemoryAllocator<OdTrVisDefProcMark> > m_aMarkers;
  OdUInt32 m_uaMarkerIndex;

  OdSharedPtr<OdTrVisHlBranchDef> m_pHlBranchDef;
  OdTrVisId m_HlBranchId;

  OdTrVisId                   m_overrideLayerId;
  bool                        m_bOverrideLayer;
  OdTrVisLayerProps           m_overrideLayerProps;

  bool m_bDestroyIdMapped;
public:

  OdXmlGlesLoader( OdTrVisRendition* pRendition, OdXmlGlesLoaderIdMapper* pIdMapper = NULL);
  virtual ~OdXmlGlesLoader();

  bool DropBegin(OdEnPathKey pk);
  virtual bool DropEnd(OdEnPathKey pk);

  bool DropBoolean(OdEnPathKey pk, bool data);

  bool DropUInt8RGB(OdEnPathKey pk, const OdBinaryData& data);
  bool DropUInt8(OdEnPathKey pk, OdUInt8 data);
  bool DropUInt16(OdEnPathKey pk, OdUInt16 data);
  bool DropUInts16(OdEnPathKey pk, const OdArray<OdUInt16, OdMemoryAllocator<OdUInt16> >& data);
  bool DropUInt32(OdEnPathKey pk, OdUInt32 data);
  bool DropUInt64(OdEnPathKey pk, OdUInt64 data);
  bool DropBinaryStream(OdEnPathKey pk, const OdBinaryData& data);

  bool DropInt16(OdEnPathKey pk, OdInt16 data);
  bool DropInt32(OdEnPathKey pk, OdInt32 data);
  bool DropInts(OdEnPathKey pk, const OdIntArray& data);

  bool DropFloat(OdEnPathKey pk, float data);
  bool DropFloats(OdEnPathKey pk, const OdArray<float, OdMemoryAllocator<float> >& data);
  bool DropDouble(OdEnPathKey pk, double data);
  bool DropDoubles(OdEnPathKey pk, const OdArray<double, OdMemoryAllocator<double> >& data);
  bool DropMatrix(OdEnPathKey pk, const OdGeMatrix3d& data,
                  const OdArray<float, OdMemoryAllocator<float> >* pDataFloats = NULL);

//  bool DropStaticId(OdEnPathKey pk, OdTrVisId idValue); // prog
  virtual bool DropId(OdEnPathKey pk, const OdAnsiString& asValue);
  bool DropDbStub(OdEnPathKey pk, const OdAnsiString& asValue);

  bool DropChars(OdEnPathKey pk, const OdAnsiString& asValue);
//  bool DropShareableClientData(OdEnPathKey pk, const OdAnsiString& asValue);
};

#include "TD_PackPop.h"

#endif // OD_XML_GLES_LOADER_H_
