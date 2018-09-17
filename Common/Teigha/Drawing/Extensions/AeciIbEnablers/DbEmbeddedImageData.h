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


#ifndef _AECI_DB_EMBEDDEDIMAGEDATA_H_
#define _AECI_DB_EMBEDDEDIMAGEDATA_H_

#include "TD_PackPush.h"

#include "AeciIbExport.h"
#include "DbObject.h"
#include "RxObjectImpl.h"
#include "OdBinaryData.h"
#include "Ge/GePoint3d.h"
#include "OdTimeStamp.h"

/** \details
    
    \sa
    TD_AeciIb

    \remarks

    <group AeciIb_Classes>
*/
class AECIIB_EXPORT AECISourceCorrelationInfo
{
public:
  AECISourceCorrelationInfo()
    : m_units(1)
    , m_scale(1.)
    , m_rotation(0.)
    , m_density(1/300.)
  {}

  void read(const OdUInt8* data);
  void write(OdUInt8* data) const;

  OdUInt32    m_units;
  double      m_scale;
  double      m_rotation;
  double      m_density;
  OdGePoint3d m_origin;
};

/** \details
    
    \sa
    TD_AeciIb

    \remarks

    <group AeciIb_Classes>
*/
class AECIIB_EXPORT AECIImageProperties
{
public:
  AECIImageProperties()
    : m_width(3000)
    , m_height(3000)
    , m_value3(0)
    , m_value4(0x10e)
    , m_value5(0x12c)
  {}

  void read(const OdUInt8* data);
  void write(OdUInt8* data) const;

  OdUInt32    m_width;
  OdUInt32    m_height;

  OdUInt32    m_value3;
  OdUInt32    m_value4;
  OdUInt32    m_value5;
};

/** \details

    \sa
    TD_AeciIb

    \remarks

    <group AeciIb_Classes>
*/
class AECIIB_EXPORT AECIDbEmbeddedImageData : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(AECIDbEmbeddedImageData);

  AECIDbEmbeddedImageData();

  OdStreamBufPtr getRasterImage() const;

  void setRasterImage(
    OdStreamBufPtr pRasterBuf);

  OdTimeStamp dataCreatedTime() const;

  OdTimeStamp dataModifiedTime() const;

  OdUInt32 imageRawDataSize() const;

  const OdBitBinaryData& imageRawData() const;

  const AECIImageProperties& imageProperties() const;

  const AECISourceCorrelationInfo& sourceCorrelation() const;

  void setSourceCorrelation(
    const AECISourceCorrelationInfo& corrInfo);

  OdString originalFilePath() const;

  void setOriginalFilePath(
    const OdString& filePath);

  OdString originalFormat() const;

  void setOriginalFormat(
    const OdString& fileFormat);

  void originalFileDetails(
    OdTimeStamp& origCreatedTime,
    OdTimeStamp& origModifiedTime,
    OdUInt32& origSize) const;

  void setOriginalFileDetails(
    OdTimeStamp origCreatedTime,
    OdTimeStamp origModifiedTime,
    OdUInt32 origSize);

  static OdDbObjectId imageDataDictionary(
    OdDbDatabase* pDb,
    bool createIfNotFound);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

private:
  static const OdInt32 kCurrentClassVersion;

  OdInt64 m_createdTime;
  OdInt64 m_modifiedTime;

  OdString m_originalPath;
  OdString m_originalFormat;
  OdInt64 m_originalCreatedTime;
  OdInt64 m_originalModifiedTime;
  OdInt32 m_originalSize;

  OdInt32 m_unknown2;
  OdInt32 m_unknown3;

  OdUInt32 m_imageUnknown;
  AECISourceCorrelationInfo m_corrInfo;
  AECIImageProperties m_imageProp;
  OdBitBinaryData m_imageData;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for AECIDbEmbeddedImageData object pointers.
*/
typedef OdSmartPtr<AECIDbEmbeddedImageData> AECIDbEmbeddedImageDataPtr;

#include "TD_PackPop.h"

#endif // _AECI_DB_EMBEDDEDIMAGEDATA_H_
