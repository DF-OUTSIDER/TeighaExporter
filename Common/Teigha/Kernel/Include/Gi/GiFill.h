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

#ifndef _OD_GIFILL_H_
#define _OD_GIFILL_H_

#include "RxObject.h"
#include "HatchPattern.h"
#include "UInt8Array.h"

#include "TD_PackPush.h"

/** \details
    This class is a base class for boundary filling definitions.

    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiFill : public OdRxObject
{
  protected:
    double m_dDeviation;
  public:
    ODRX_DECLARE_MEMBERS(OdGiFill);
    OdGiFill();

    double deviation() const;
    void setDeviation(double dDeviation);

    virtual void copyFrom(const OdRxObject* pSource);

    virtual bool operator ==(const OdGiFill& fill) const;
    virtual bool operator !=(const OdGiFill& fill) const;

    virtual void saveBytes(OdUInt8Array &bytes) const;
    virtual void loadBytes(const OdUInt8 *pBytes);

    static OdSmartPtr<OdGiFill> loadFill(const OdUInt8 *pBytes);
  protected:
    void pushDbl(OdUInt8Array &bytes, double dDbl) const;
    void pushInt(OdUInt8Array &bytes, OdUInt32 nInt) const;
    void popDbl(const OdUInt8 *&pBytes, double &dDbl) const;
    void popInt(const OdUInt8 *&pBytes, OdUInt32 &nInt) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiFill object pointers.
*/
typedef OdSmartPtr<OdGiFill> OdGiFillPtr;

inline
OdGiFill::OdGiFill()
  : m_dDeviation(0.0)
{ }

inline
double OdGiFill::deviation() const
{
  return m_dDeviation;
}

inline
void OdGiFill::setDeviation(double dDeviation)
{
  m_dDeviation = dDeviation;
}

inline
void OdGiFill::copyFrom(const OdRxObject* pSource)
{
  OdGiFillPtr pSrcFill = OdGiFill::cast(pSource);
  if (!pSrcFill.isNull())
    setDeviation(pSrcFill->deviation());
  else
    throw OdError(eNotApplicable);
}

inline
bool OdGiFill::operator ==(const OdGiFill& fill) const
{
  return (isA() == fill.isA()) && OdEqual(deviation(), fill.deviation());
}

inline
bool OdGiFill::operator !=(const OdGiFill& fill) const
{
  return (isA() != fill.isA()) || !OdEqual(deviation(), fill.deviation());
}

inline
void OdGiFill::saveBytes(OdUInt8Array &bytes) const
{
  bytes.push_back(0);
  pushDbl(bytes, m_dDeviation);
}

inline
void OdGiFill::loadBytes(const OdUInt8 *pBytes)
{
  if (*pBytes++ != 0)
    throw OdError(eInvalidInput);
  popDbl(pBytes, m_dDeviation);
}

inline
void OdGiFill::pushDbl(OdUInt8Array &bytes, double dDbl) const
{
  OdUInt8 nBytes[sizeof(double)];
  ::memcpy(nBytes, &dDbl, sizeof(double));
  bytes.insert(bytes.end(), nBytes, nBytes + sizeof(double));
}

inline
void OdGiFill::pushInt(OdUInt8Array &bytes, OdUInt32 nInt) const
{
  OdUInt8 nBytes[sizeof(OdUInt32)];
  ::memcpy(nBytes, &nInt, sizeof(OdUInt32));
  bytes.insert(bytes.end(), nBytes, nBytes + sizeof(OdUInt32));
}

inline
void OdGiFill::popDbl(const OdUInt8 *&pBytes, double &dDbl) const
{
  ::memcpy(&dDbl, pBytes, sizeof(double));
  pBytes += sizeof(double);
}

inline
void OdGiFill::popInt(const OdUInt8 *&pBytes, OdUInt32 &nInt) const
{
  ::memcpy(&nInt, pBytes, sizeof(OdUInt32));
  pBytes += sizeof(OdUInt32);
}

/** \details
    This class is a specialtion of OdGiFill for hatch pattern type of filling.

    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiHatchPattern : public OdGiFill
{
  protected:
    OdHatchPattern m_aHatchPattern;
    bool   m_isDraft;
    
  public:
    ODRX_DECLARE_MEMBERS(OdGiHatchPattern);
    OdGiHatchPattern();

    const OdHatchPattern &patternLines() const;
    OdHatchPattern &patternLines();
    void setPatternLines(const OdHatchPattern &aHatchPattern);

    virtual void copyFrom(const OdRxObject* pSource);

    virtual bool operator ==(const OdGiFill& fill) const;
    virtual bool operator !=(const OdGiFill& fill) const;

    virtual void saveBytes(OdUInt8Array &bytes) const;
    virtual void loadBytes(const OdUInt8 *pBytes);

    bool isDraft() const;
    void setDraft(bool draft);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiHatchPattern object pointers.
*/
typedef OdSmartPtr<OdGiHatchPattern> OdGiHatchPatternPtr;

inline
OdGiHatchPattern::OdGiHatchPattern() :
  OdGiFill(),
  m_isDraft(false)
{ }

inline
const OdHatchPattern &OdGiHatchPattern::patternLines() const
{
  return m_aHatchPattern;
}

inline
OdHatchPattern &OdGiHatchPattern::patternLines()
{
  return m_aHatchPattern;
}

inline
void OdGiHatchPattern::setPatternLines(const OdHatchPattern &aHatchPattern)
{
  m_aHatchPattern = aHatchPattern;
}

inline
void OdGiHatchPattern::copyFrom(const OdRxObject* pSource)
{
  OdGiHatchPatternPtr pSrcFill = OdGiHatchPattern::cast(pSource);
  if (!pSrcFill.isNull())
  {
    m_isDraft = pSrcFill->m_isDraft;
    setPatternLines(pSrcFill->patternLines());
    OdGiFill::copyFrom(pSource);
  }
  else
    throw OdError(eNotApplicable);
}

inline
bool OdGiHatchPattern::operator ==(const OdGiFill& fill) const
{
  if (isA() == fill.isA())
  {
    if (OdEqual(deviation(), fill.deviation()))
    {
      const OdGiHatchPattern *pSecond = static_cast<const OdGiHatchPattern*>(&fill);
      if (patternLines().size() == pSecond->patternLines().size() &&
          m_isDraft == pSecond->m_isDraft)
      {
        for (OdUInt32 nPat = 0; nPat < patternLines().size(); nPat++)
        {
          const OdHatchPatternLine &pl1 = patternLines().getPtr()[nPat];
          const OdHatchPatternLine &pl2 = pSecond->patternLines().getPtr()[nPat];
          if (!OdEqual(pl1.m_dLineAngle, pl2.m_dLineAngle) ||
              !OdEqual(pl1.m_basePoint.x, pl2.m_basePoint.x) || !OdEqual(pl1.m_basePoint.y, pl2.m_basePoint.y) ||
              !OdEqual(pl1.m_patternOffset.x, pl2.m_patternOffset.x) || !OdEqual(pl1.m_patternOffset.y, pl2.m_patternOffset.y) ||
              pl1.m_dashes.size() != pl2.m_dashes.size())
            return false;
          for (OdUInt32 nDash = 0; nDash < pl1.m_dashes.size(); nDash++)
          {
            if (!OdEqual(pl1.m_dashes.getPtr()[nDash], pl2.m_dashes.getPtr()[nDash]))
              return false;
          }
        }
        return true;
      }
    }
  }
  return false;
}

inline
bool OdGiHatchPattern::operator !=(const OdGiFill& fill) const
{
  return !operator ==(fill);
}

inline
void OdGiHatchPattern::saveBytes(OdUInt8Array &bytes) const
{
  bytes.push_back(1);
  pushInt(bytes, m_isDraft ? 1 : 0);
  pushInt(bytes, m_aHatchPattern.size());
  for (OdUInt32 nPat = 0; nPat < m_aHatchPattern.size(); nPat++)
  {
    const OdHatchPatternLine &pl = m_aHatchPattern.getPtr()[nPat];
    pushDbl(bytes, pl.m_dLineAngle);
    pushDbl(bytes, pl.m_basePoint.x); pushDbl(bytes, pl.m_basePoint.y);
    pushDbl(bytes, pl.m_patternOffset.x); pushDbl(bytes, pl.m_patternOffset.y);
    pushInt(bytes, pl.m_dashes.size());
    for (OdUInt32 nDash = 0; nDash < pl.m_dashes.size(); nDash++)
      pushDbl(bytes, pl.m_dashes.getPtr()[nDash]);
  }
  OdGiFill::saveBytes(bytes);
}

inline
void OdGiHatchPattern::loadBytes(const OdUInt8 *pBytes)
{
  if (*pBytes++ != 1)
    throw OdError(eInvalidInput);
  OdUInt32 isDraft; popInt(pBytes, isDraft); m_isDraft = (isDraft == 1);
  OdUInt32 nPats = 0; popInt(pBytes, nPats);
  m_aHatchPattern.resize(nPats);
  for (OdUInt32 nPat = 0; nPat < nPats; nPat++)
  {
    OdHatchPatternLine &pl = m_aHatchPattern[nPat];
    popDbl(pBytes, pl.m_dLineAngle);
    popDbl(pBytes, pl.m_basePoint.x); popDbl(pBytes, pl.m_basePoint.y);
    popDbl(pBytes, pl.m_patternOffset.x); popDbl(pBytes, pl.m_patternOffset.y);
    OdUInt32 nDashes = 0; popInt(pBytes, nDashes);
    pl.m_dashes.resize(nDashes);
    for (OdUInt32 nDash = 0; nDash < nDashes; nDash++)
      popDbl(pBytes, pl.m_dashes[nDash]);
  }
  OdGiFill::loadBytes(pBytes);
}

inline
OdSmartPtr<OdGiFill> OdGiFill::loadFill(const OdUInt8 *pBytes)
{
  OdGiFillPtr pObj;
  switch (*pBytes)
  {
    case 0: pObj = OdGiFill::createObject(); break;
    case 1: pObj = OdGiHatchPattern::createObject(); break;
  }
  if (!pObj.isNull())
    pObj->loadBytes(pBytes);
  return pObj;
}

inline
bool OdGiHatchPattern::isDraft() const
{
  return m_isDraft;
}

inline
void OdGiHatchPattern::setDraft(bool draft)
{
  m_isDraft = draft;
}


#include "TD_PackPop.h"

#endif //_OD_GIFILL_H_
