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
// GLES2 device special display codes processing helpers

#ifndef ODTRVISDISPLAYCODEPROC
#define ODTRVISDISPLAYCODEPROC

#include "TD_PackPush.h"

#include "TrVisRendition.h"

/** \details
    <group ExRender_Classes>
*/
class OdTrVisDisplayCodeKeeper
{
  public:
    template <typename TArray>
    static void pushZero(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeZero);
    }
    template <typename TArray>
    static void pushHighlight(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeHighlight);
    }
    template <typename TArray>
    static void pushUnhighlight(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeUnhighlight);
    }
    template <typename TArray>
    static void pushHighlight(TArray &arry, bool bHighlight)
    {
      arry.push_back((bHighlight) ? OdTrVisRendition::kDisplayCodeHighlight : OdTrVisRendition::kDisplayCodeUnhighlight);
    }
    template <typename TArray>
    static void pushPushMatrix(TArray &arry, const OdGeMatrix3d &mtx)
    {
      typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodePushMatrix);
      const ValType *pMtxRepresentation = reinterpret_cast<const ValType*>(mtx.entry);
      arry.insert(arry.end(), pMtxRepresentation, pMtxRepresentation + (sizeof(OdGeMatrix3d) / sizeof(ValType)));
    }
    template <typename TArray>
    static void pushPopMatrix(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodePopMatrix);
    }
    template <typename TArray>
    static void pushSelMarkers(TArray &arry, const void *pHlBranch)
    {
      typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodeSelMarkers);
      arry.push_back((ValType)(OdIntPtr)pHlBranch);
    }
    template <typename TArray>
    static void pushDrawOrder(TArray &arry, double drawOrder)
    {
      typedef OD_TYPENAME TArray::value_type ValType;
      arry.push_back(OdTrVisRendition::kDisplayCodeDrawOrder);
      arry.push_back(*reinterpret_cast<ValType*>(&drawOrder));
    }
    template <typename TArray>
    static void pushMultiOrder(TArray &arry)
    {
      arry.push_back(OdTrVisRendition::kDisplayCodeMultiOrder);
    }
};

/** \details
    <group ExRender_Classes>
*/
class OdTrVisFastComparator
{
  public:
    template <typename TObj>
    static bool isEqual(const TObj &obj1, const TObj &obj2)
    {
      return !::memcmp(&obj1, &obj2, sizeof(TObj));
    }
    static bool isIdentity(const OdGeMatrix3d &mtx)
    {
      return isEqual(mtx, OdGeMatrix3d::kIdentity);
    }
};

/** \details
    <group ExRender_Classes>
*/
class OdTrVisDisplayListIterator
{
  OdTrVisDisplayId *m_pList;
  OdUInt32 m_nListData, m_nCaret;
  public:
    OdTrVisDisplayListIterator(const OdTrVisDisplayId *pList, OdUInt32 nData)
      : m_pList(const_cast<OdTrVisDisplayId*>(pList)), m_nListData(nData), m_nCaret(0)
    {
    }
    bool isEOF() const { return m_nCaret >= m_nListData; }
    bool isCode() const { return m_pList[m_nCaret] < OdTrVisRendition::kDisplayCodeRange; }
    OdTrVisRendition::DisplayIdCodes code() const { return (OdTrVisRendition::DisplayIdCodes)m_pList[m_nCaret]; }
    void skipCode() {
      OdUInt32 nSteps = 1;
      switch (code()) { case OdTrVisRendition::kDisplayCodePushMatrix: nSteps += sizeof(OdGeMatrix3d) / sizeof(double); break;
                        case OdTrVisRendition::kDisplayCodeSelMarkers:
                        case OdTrVisRendition::kDisplayCodeDrawOrder : nSteps += 1; break; default: break; }
      m_nCaret += nSteps;
    }
    OdTrVisDisplayId &cur() { return m_pList[m_nCaret]; }
    const OdTrVisDisplayId &cur() const { return m_pList[m_nCaret]; }
    OdTrVisDisplayId &next(OdUInt32 n = 1) { return m_pList[m_nCaret + n]; }
    const OdTrVisDisplayId &next(OdUInt32 n = 1) const { return m_pList[m_nCaret + n]; }
    bool step() { if (isCode()) skipCode(); else m_nCaret++; return isEOF(); }
    // Access extensions
    const double *codeAsDoublePtr(OdUInt32 n = 1) const { return reinterpret_cast<const double*>(m_pList + n); }
    const OdGeMatrix3d *codeAsMatrixPtr(OdUInt32 n = 1) const { return reinterpret_cast<const OdGeMatrix3d*>(m_pList + n); }
    const OdTrVisId *codeAsIdPtr(OdUInt32 n = 1) const { return reinterpret_cast<const OdTrVisId*>(m_pList + n); }
    const void *codeAsPtr(OdUInt32 n = 1) const { return OdTrVisIdToPtr(const void, m_pList[n]); }
};

#include "TD_PackPop.h"

#endif // ODTRVISDISPLAYCODEPROC
