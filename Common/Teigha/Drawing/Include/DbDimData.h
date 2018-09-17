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

#ifndef ODDBDIMDATA_H
#define ODDBDIMDATA_H

#include "TD_PackPush.h"

#include "DbDimension.h"

class OdDbDimData;

typedef OdArray<OdDbDimData> OdDbDimDataArray;
typedef OdArray<OdDbDimData*, OdMemoryAllocator<OdDbDimData*> > OdDbDimDataPtrArray;
typedef OdGeVector3d (*DimDataSetValueFuncPtr) (OdDbDimData* pThis,
                                                OdDbEntity* pEnt, 
                                                double newValue,
                                                const OdGeVector3d& offset);

typedef OdGeVector3d (*DimDataSetCustomStringFuncPtr) (OdDbDimData* pThis,
                                                       OdDbEntity* pEnt, 
                                                       const OdString& sCustomString,
                                                       const OdGeVector3d& offset);
/** \details
    This class passes parameters for the getGripPoints and moveGripPoints functions of OdDbEntity.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class OdDbDimData
{
public:
  enum DimDataFlags
  {
    kDimEditable           = 0x0001,
    kDimInvisible          = 0x0002,
    kDimFocal              = 0x0004,
    kDimHideIfValueIsZero  = 0x0008,
    kDimEmptyData          = 0x0010,
    kDimResultantLength    = 0x0020,
    kDimDeltaLength        = 0x0040,
    kDimResultantAngle     = 0x0080,
    kDimDeltaAngle         = 0x0100,
    kDimRadius             = 0x0200,
    kDimCustomValue        = 0x0400,
    kDimConstrained        = 0x0800,
    kDimCustomString       = 0x1000
  };

  OdDbDimData();

  OdDbDimData(OdDbDimension* pDim, 
    DimDataSetValueFuncPtr setDimFunc = 0,
    unsigned int bitFlags = 0,
    void* appData = 0,
    DimDataSetCustomStringFuncPtr setCustomStringFunc = 0);

  ~OdDbDimData();

  OdDbDimData(const OdDbDimData&);
  OdDbDimData&  operator = (const OdDbDimData&);
  OdDbDimensionPtr dimension() const;
  void setDimension(OdDbDimension* pDim);
  OdDbObjectId ownerId() const;
  void setOwnerId(const OdDbObjectId& objId);
  unsigned int bitFlags() const;
  void setBitFlags(unsigned int flags);
  bool isDimFocal() const;
  void setDimFocal(bool focal);
  bool isDimEditable() const;
  void setDimEditable(bool editable);
  bool isDimInvisible() const;
  void setDimInvisible(bool invisible);
  bool isDimHideIfValueIsZero() const;
  void setDimHideIfValueIsZero(bool hide);
  void *appData() const;
  void setAppData(void* appData);
  DimDataSetValueFuncPtr dimValueFunc() const;
  void setDimValueFunc(DimDataSetValueFuncPtr funcPtr);
  DimDataSetCustomStringFuncPtr customStringFunc() const;
  void setCustomStringFunc(DimDataSetCustomStringFuncPtr funcPtr);
  bool isDimResultantLength() const;
  void setDimResultantLength(bool bValue);
  bool isDimDeltaLength() const;
  void setDimDeltaLength(bool bValue);
  bool isDimResultantAngle() const;
  void setDimResultantAngle(bool bValue);
  bool isDimDeltaAngle() const;
  void setDimDeltaAngle(bool bValue);
  bool isDimRadius() const;
  void setDimRadius(bool bValue);
  bool isCustomDimValue() const;
  void setCustomDimValue(bool custom);
  bool isConstrained() const;
  void setConstrain(bool bValue);
  bool isCustomString() const;
  void setCustomString(bool bValue);

private:
  OdDbDimensionPtr              m_pDim;
  OdDbObjectId                  m_ownerId;
  unsigned int                  m_bitFlags;
  void*                         m_pAppData;
  DimDataSetValueFuncPtr        m_pSetDimValueFunc;
  DimDataSetCustomStringFuncPtr m_pSetCustomStringFunc;
};

inline OdDbDimData::OdDbDimData()
  : m_bitFlags(0)
  , m_pAppData (0)
  , m_pSetDimValueFunc(0)
  , m_pSetCustomStringFunc(0)
{
}

inline
OdDbDimData::OdDbDimData(OdDbDimension* pDim, 
                         DimDataSetValueFuncPtr setValueFunc,
                         unsigned int bitFlags,
                         void* appData,
                         DimDataSetCustomStringFuncPtr setCustomStringFunc)
  : m_pDim(pDim)
  , m_bitFlags(bitFlags)
  , m_pAppData(appData)
  , m_pSetDimValueFunc(setValueFunc)
  , m_pSetCustomStringFunc(setCustomStringFunc)
{
}

inline OdDbDimData::~OdDbDimData()
{
}

inline OdDbDimensionPtr
OdDbDimData::dimension() const
{
  return m_pDim;
}

inline void
OdDbDimData::setDimension(OdDbDimension* pDim)
{
  m_pDim = pDim;
}

inline OdDbObjectId
OdDbDimData::ownerId() const
{
  return m_ownerId;
}

inline void
OdDbDimData::setOwnerId(const OdDbObjectId& objId)
{
  m_ownerId = objId;
}

inline unsigned int 
OdDbDimData::bitFlags() const
{
  return m_bitFlags;
}

inline void
OdDbDimData::setBitFlags(unsigned int flags)
{
  m_bitFlags = flags;
}

inline bool
OdDbDimData::isDimFocal() const
{
  return 0 != (m_bitFlags & kDimFocal);
}

inline void
OdDbDimData::setDimFocal(bool focal)
{
  if (focal)
    m_bitFlags |= kDimFocal;
  else
    m_bitFlags &= ~kDimFocal;
}

inline bool
OdDbDimData::isDimEditable() const
{
  return 0 != (m_bitFlags & kDimEditable);
}

inline void
OdDbDimData::setDimEditable(bool editable)
{
  if (editable)
    m_bitFlags |= kDimEditable;
  else
    m_bitFlags &= ~kDimEditable;
}

inline bool
OdDbDimData::isDimInvisible() const
{
  return 0 != (m_bitFlags & kDimInvisible);
}

inline void
OdDbDimData::setDimInvisible(bool invisible)
{
  if (invisible)
    m_bitFlags |= kDimInvisible;
  else
    m_bitFlags &= ~kDimInvisible;
}

inline bool
OdDbDimData::isDimHideIfValueIsZero() const
{
  return 0 != (m_bitFlags & kDimHideIfValueIsZero);
}

inline void
OdDbDimData::setDimHideIfValueIsZero(bool hide)
{
  if (hide)
    m_bitFlags |= kDimHideIfValueIsZero;
  else
    m_bitFlags &= ~kDimHideIfValueIsZero;
}

inline bool
OdDbDimData::isDimResultantLength() const
{
  return 0 != (m_bitFlags & kDimResultantLength);
}

inline void
OdDbDimData::setDimResultantLength(bool bValue)
{
  if (bValue)
    m_bitFlags |= kDimResultantLength;
  else
    m_bitFlags &= ~kDimResultantLength;
}

inline bool
OdDbDimData::isDimDeltaLength() const
{
  return 0 != (m_bitFlags & kDimDeltaLength);
}

inline void
OdDbDimData::setDimDeltaLength(bool bValue)
{
  if (bValue)
    m_bitFlags |= kDimDeltaLength;
  else
    m_bitFlags &= ~kDimDeltaLength;
}

inline bool
OdDbDimData::isDimResultantAngle() const
{
  return 0 != (m_bitFlags & kDimResultantAngle);
}

inline void
OdDbDimData::setDimResultantAngle(bool bValue)
{
  if (bValue)
    m_bitFlags |= kDimResultantAngle;
  else
    m_bitFlags &= ~kDimResultantAngle;
}

inline bool
OdDbDimData::isDimDeltaAngle() const
{
  return 0 != (m_bitFlags & kDimDeltaAngle);
}

inline void
OdDbDimData::setDimDeltaAngle(bool bValue)
{
  if (bValue)
    m_bitFlags |= kDimDeltaAngle;
  else
    m_bitFlags &= ~kDimDeltaAngle;
}

inline bool
OdDbDimData::isDimRadius() const
{
    return 0 != (m_bitFlags & kDimRadius);
}

inline void
OdDbDimData::setDimRadius(bool bValue)
{
  if (bValue)
    m_bitFlags |= kDimRadius;
  else
    m_bitFlags &= ~kDimRadius;
}

inline bool
OdDbDimData::isCustomDimValue() const
{
  return 0 != (m_bitFlags & kDimCustomValue);
}

inline void
OdDbDimData::setCustomDimValue(bool custom)
{
  if (custom)
    m_bitFlags |= kDimCustomValue;
  else
    m_bitFlags &= ~kDimCustomValue;
}

inline bool
OdDbDimData::isConstrained() const
{
  return 0 != (m_bitFlags & kDimConstrained);
}

inline void
OdDbDimData::setConstrain(bool bValue)
{
  if (bValue)
    m_bitFlags |= kDimConstrained;
  else
    m_bitFlags &= ~kDimConstrained;
}

inline bool
OdDbDimData::isCustomString() const
{
  return 0 != (m_bitFlags & kDimCustomString);
}

inline void
OdDbDimData::setCustomString(bool bValue)
{
  if (bValue)
    m_bitFlags |= kDimCustomString;
  else
    m_bitFlags &= ~kDimCustomString;
}

inline void*
OdDbDimData::appData() const
{
  return m_pAppData;
}

inline void
OdDbDimData::setAppData(void* appData)
{
  m_pAppData = appData;
}

inline DimDataSetValueFuncPtr
OdDbDimData::dimValueFunc() const
{
  return m_pSetDimValueFunc;
}

inline void
OdDbDimData::setDimValueFunc(DimDataSetValueFuncPtr funcPtr)
{
  m_pSetDimValueFunc = funcPtr;
}

inline DimDataSetCustomStringFuncPtr
OdDbDimData::customStringFunc() const
{
  return m_pSetCustomStringFunc;
}

inline void
OdDbDimData::setCustomStringFunc(DimDataSetCustomStringFuncPtr funcPtr)
{
  m_pSetCustomStringFunc = funcPtr;
}

inline OdDbDimData& OdDbDimData::operator = (const OdDbDimData& src)
{
  if (this == &src)
    return *this;
  
  m_pDim = src.dimension()->clone();
  m_ownerId = src.ownerId();
  m_pSetDimValueFunc = src.dimValueFunc();
  m_pSetCustomStringFunc = src.customStringFunc();
  m_bitFlags = src.bitFlags();
  m_pAppData = src.appData();
  return *this;
}

inline OdDbDimData::OdDbDimData(const OdDbDimData& src)
{
  *this = src;
}

#include "TD_PackPop.h"

#endif // ODDBDIMDATA_H
