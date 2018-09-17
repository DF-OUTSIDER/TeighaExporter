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

#ifndef ODA_NEXT_CODE_H_
#define ODA_NEXT_CODE_H_

#define NEXT_CODE(code)           \
  if (pFiler->nextItem() != code) \
  {                               \
    ODA_FAIL_ONCE();              \
    return eMakeMeProxy;          \
  }

#define NEXT_UINT64(code, ref64)                 \
  switch (pFiler->nextItem())                    \
  {                                              \
  case code:                                     \
    ref64 = pFiler->rdUInt64();                  \
    break;                                       \
  case (91 + code - 160): /* hi */               \
    ref64 = pFiler->rdUInt32();                  \
    ref64  <<= 32;                               \
    if (pFiler->nextItem() != (91 + code - 160)) \
    { /* low */                                  \
      ref64 += pFiler->rdUInt32();               \
      break;                                     \
    }                                            \
  default:                                       \
    ODA_FAIL_ONCE();                             \
    return eMakeMeProxy;                         \
  }

#define DXF_OUT_UINT64(code, ref64)                                       \
  if (pFiler->dwgVersion() > OdDb::vAC21)                                 \
    pFiler->wrUInt64(code, ref64);                                        \
  else                                                                    \
  {                                                                       \
    pFiler->wrUInt32(91 + code - 160, (OdUInt32) (ref64 >> 32)); /* hi */ \
    pFiler->wrUInt32(92 + code - 160, (OdInt32) ref64); /* low */         \
  }

#define DWG_IN_UINT64(ref64)                          \
  if (pFiler->dwgVersion() > OdDb::vAC21)             \
    ref64 = pFiler->rdInt64();                        \
  else                                                \
  {                                                   \
    ref64 = pFiler->rdInt32(); /* hi */               \
    ref64  <<= 32;                                    \
    ref64 += (OdUInt32) pFiler->rdInt32(); /* low */  \
  }

#define DWG_OUT_UINT64(ref64)                            \
  if (pFiler->dwgVersion() > OdDb::vAC21)                \
    pFiler->wrInt64(ref64);                              \
  else                                                   \
  {                                                      \
    pFiler->wrInt32((OdInt32) (ref64 >> 32)); /* hi */   \
    pFiler->wrInt32((OdInt32) ref64); /* low */          \
  }

#endif // ODA_NEXT_CODE_H_
