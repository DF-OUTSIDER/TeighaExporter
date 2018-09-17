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

#include "OdaCommon.h"
#include "Gs/GsFiler.h"
#include "ExOpenGLMetafileStream.h"
#include "PseudoGLDefs.h"

#ifdef _DEBUG
#define STL_USING_LIMITS
#include "OdaSTL.h"
inline OdGLfloat dbl2float(double value)
{
  // Test if value denormalized
  if (value < double(std::numeric_limits<float>::min()) && value > -double(std::numeric_limits<float>::min()))
    return 0.0f;
  // Test if value to big
  if (value > double(std::numeric_limits<float>::max()))
    return std::numeric_limits<float>::max();
  // Test if value to small
  if (value < double(-std::numeric_limits<float>::max()))
    return -std::numeric_limits<float>::max();
  return (OdGLfloat)value;
}
#else
#ifndef dbl2float
#define dbl2float(x) (OdGLfloat)(x)
#endif
#endif

// OdOpenGLArrayWrapper

void OdOpenGLArrayWrapper::setArray(const void *pPtr, OdUInt32 size)
{
  clearArray();
  m_pData = ::odrxAlloc(size);
  if (m_pData == NULL)
    throw OdError(eOutOfMemory);
  ::memcpy(m_pData, pPtr, size);
  m_uSize = size;
}

void OdOpenGLArrayWrapper::clearArray()
{
  if (m_pData)
  {
    ::odrxFree(m_pData);
    m_pData = NULL;
    m_uSize = 0;
  }
}

void OdOpenGLArrayWrapper::concatArray(const void *pPtr, OdUInt32 size)
{
  if (!m_pData)
  {
    setArray(pPtr, size);
  }
  else
  {
    m_pData = ::odrxRealloc(m_pData, m_uSize + size, m_uSize);
    if (m_pData == NULL)
      throw OdError(eOutOfMemory);
    ::memcpy((void*)(((OdUInt8*)m_pData) + m_uSize), pPtr, size);
    m_uSize += size;
  }
}

#ifndef OD_OGL_ENABLEFLOATCONVERSION
void OdOpenGLArrayWrapper::setArray_dbl(const double *pPtr, OdUInt32 size)
#else
void OdOpenGLArrayWrapper::setArray_dbl(const double *pPtr, OdUInt32 size)
#endif
{
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  setArray(pPtr, size * sizeof(double));
#else
  clearArray();
  m_pData = ::odrxAlloc(size * sizeof(OdGLfloat));
  if (m_pData == NULL)
    throw OdError(eOutOfMemory);
  OdGLfloat *pData = (OdGLfloat*)m_pData;
  m_uSize = size * sizeof(OdGLfloat);
  while (size)
  {
    *pData = dbl2float(*pPtr);
    pData++; pPtr++;
    size--;
  }
#endif
}

void OdOpenGLArrayWrapper::concatArray_dbl(const double *pPtr, OdUInt32 size)
{
#ifndef OD_OGL_ENABLEFLOATCONVERSION
  concatArray(pPtr, size * sizeof(double));
#else
  if (!m_pData)
  {
    setArray_dbl(pPtr, size);
  }
  else
  {
    m_pData = ::odrxRealloc(m_pData, m_uSize + size * sizeof(OdGLfloat), m_uSize);
    if (m_pData == NULL)
      throw OdError(eOutOfMemory);
    OdGLfloat *pData = (OdGLfloat*)(((OdUInt8*)m_pData) + m_uSize);
    m_uSize += size * sizeof(OdGLfloat);
    while (size)
    {
      *pData = dbl2float(*pPtr);
      pData++; pPtr++;
      size--;
    }
  }
#endif
}

void OdOpenGLArrayWrapper::setArray_ush(const OdInt32 *pPtr, OdUInt32 size)
{
  clearArray();
  m_pData = ::odrxAlloc(size * sizeof(OdGLushort));
  if (m_pData == NULL)
    throw OdError(eOutOfMemory);
  OdGLushort *pData = (OdGLushort*)m_pData;
  m_uSize = size * sizeof(OdGLushort);
  while (size)
  {
    *pData = OdGLushort(*pPtr);
    pData++; pPtr++;
    size--;
  }
}

// OdOpenGLFlatMetafileContainer

OdOpenGLFlatMetafileContainer::OdOpenGLFlatMetafileContainer() : OdFlatMetafileContainer() { }

OdOpenGLFlatMetafileContainer::~OdOpenGLFlatMetafileContainer()
{
  clearOpenGLFlatContainer();
}

void OdOpenGLFlatMetafileContainer::clearOpenGLFlatContainer()
{
  for (OdUInt32 i = 0; i < m_ArrayElements.size(); i++)
  {
    m_ArrayElements.at(i).clearArray();
  }
}

void OdOpenGLFlatMetafileContainer::optimizeMemory(OdUInt32 uSize)
{
  OdFlatMetafileContainer::optimizeMemory(uSize);
  m_ArrayElements.setPhysicalLength(m_ArrayElements.size());
  m_RxObjects.setPhysicalLength(m_RxObjects.size());
}

OdBaseMetafileContainerReader *OdOpenGLFlatMetafileContainer::createReader() const
{
  return new OdOpenGLFlatMetafileContainerReader();
}

OdBaseMetafileContainerWriter *OdOpenGLFlatMetafileContainer::createWriter() const
{
  return new OdOpenGLFlatMetafileContainerWriter();
}

// Helper functions

const OdChar *chunkDecode(OdOpenGLMetaRecType rt)
{
  switch (rt)
  {
    case OdOpenGLMetaRecType_Empty:              return OD_T("OdOpenGLMetaRecType_Empty");              break;
    case OdOpenGLMetaRecType_EnableOpt:          return OD_T("OdOpenGLMetaRecType_EnableOpt");          break;
    case OdOpenGLMetaRecType_DisableOpt:         return OD_T("OdOpenGLMetaRecType_DisableOpt");         break;
    case OdOpenGLMetaRecType_Color:              return OD_T("OdOpenGLMetaRecType_Color");              break;
    case OdOpenGLMetaRecType_EnableArray:        return OD_T("OdOpenGLMetaRecType_EnableArray");        break;
    case OdOpenGLMetaRecType_DisableArray:       return OD_T("OdOpenGLMetaRecType_DisableArray");       break;
    case OdOpenGLMetaRecType_DrawArrays:         return OD_T("OdOpenGLMetaRecType_DrawArrays");         break;
    case OdOpenGLMetaRecType_DrawElements:       return OD_T("OdOpenGLMetaRecType_DrawElements");       break;
    case OdOpenGLMetaRecType_CullFace:           return OD_T("OdOpenGLMetaRecType_CullFace");           break;
    case OdOpenGLMetaRecType_LStipple:           return OD_T("OdOpenGLMetaRecType_LStipple");           break;
    case OdOpenGLMetaRecType_PStipple:           return OD_T("OdOpenGLMetaRecType_PStipple");           break;
    case OdOpenGLMetaRecType_VPoint:             return OD_T("OdOpenGLMetaRecType_VPoint");             break;
    case OdOpenGLMetaRecType_VLine:              return OD_T("OdOpenGLMetaRecType_VLine");              break;
    case OdOpenGLMetaRecType_IPoint:             return OD_T("OdOpenGLMetaRecType_IPoint");             break;
    case OdOpenGLMetaRecType_ILine:              return OD_T("OdOpenGLMetaRecType_ILine");              break;
    case OdOpenGLMetaRecType_HLRStencil:         return OD_T("OdOpenGLMetaRecType_HLRStencil");         break;
    case OdOpenGLMetaRecType_EnableShading:      return OD_T("OdOpenGLMetaRecType_EnableShading");      break;
    case OdOpenGLMetaRecType_DisableShading:     return OD_T("OdOpenGLMetaRecType_DisableShading");     break;
    case OdOpenGLMetaRecType_Material:           return OD_T("OdOpenGLMetaRecType_Material");           break;
    case OdOpenGLMetaRecType_UserEntry:          return OD_T("OdOpenGLMetaRecType_UserEntry");          break;
    case OdOpenGLMetaRecType_InitTexture:        return OD_T("OdOpenGLMetaRecType_InitTexture");        break;
    case OdOpenGLMetaRecType_UninitTexture:      return OD_T("OdOpenGLMetaRecType_UninitTexture");      break;
    case OdOpenGLMetaRecType_SelectionMarker:    return OD_T("OdOpenGLMetaRecType_SelectionMarker");    break;
    case OdOpenGLMetaRecType_EnableMarkerArray:  return OD_T("OdOpenGLMetaRecType_EnableMarkerArray");  break;
    case OdOpenGLMetaRecType_DisableMarkerArray: return OD_T("OdOpenGLMetaRecType_DisableMarkerArray"); break;
    case OdOpenGLMetaRecType_SelectionFlags:     return OD_T("OdOpenGLMetaRecType_SelectionFlags");     break;
    case OdOpenGLMetaRecType_Lineweight:         return OD_T("OdOpenGLMetaRecType_Lineweight");         break;
    case OdOpenGLMetaRecType_Linestyle:          return OD_T("OdOpenGLMetaRecType_Linestyle");          break;
    case OdOpenGLMetaRecType_TtfText:            return OD_T("OdOpenGLMetaRecType_TtfText");            break;
    default:                                     return OD_T("Unknown");
  }
}

//
