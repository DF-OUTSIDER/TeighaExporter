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

#ifndef _ODGILINEWEIGHTOVERRIDE_INCLUDED_
#define _ODGILINEWEIGHTOVERRIDE_INCLUDED_

#include "Gi/Gi.h"
#include "Ps/PlotStyles.h"

#include "TD_PackPush.h"

/** \details
    This class provides an interface to override OdGsView lineweight settings.
  
    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
struct OdGiLineweightOverride
{
  protected:
    enum Flags
    {
      kLwdOverrideScale = (1 << 0),
      kLwdOverrideStyle = (1 << 1)
    };
    OdUInt32            m_uFlags;
    double              m_dLwdScale;
    double              m_dPixScale;
    OdPs::LineEndStyle  m_lineCapStyle;
    OdPs::LineJoinStyle m_lineJoinStyle;
  public:
    OdGiLineweightOverride()
      : m_uFlags(0)
      , m_dLwdScale(1.0)
      , m_dPixScale(1.0)
      , m_lineCapStyle(OdPs::kLesRound)
      , m_lineJoinStyle(OdPs::kLjsRound)
    { }
    // Scale override
    void setScaleOverride(double dLwdScale)
    {
      SETBIT_1(m_uFlags, kLwdOverrideScale);
      m_dLwdScale = m_dPixScale = dLwdScale;
    }
    void resetScaleOverride()
    {
      SETBIT_0(m_uFlags, kLwdOverrideScale);
      m_dLwdScale = m_dPixScale = 1.0;
    }
    bool hasScaleOverride() const
    {
      return GETBIT(m_uFlags, kLwdOverrideScale);
    }
    double scaleOverride() const
    {
      return m_dLwdScale;
    }
    // Helpers
    void setPixelScale(double dPixScale)
    {
      m_dPixScale = dPixScale;
    }
    double pixelScale() const
    {
      return m_dPixScale;
    }
    // Style override
    void setStyleOverride(OdPs::LineEndStyle lineCapStyle, OdPs::LineJoinStyle lineJoinStyle)
    {
      SETBIT_1(m_uFlags, kLwdOverrideStyle);
      m_lineCapStyle = lineCapStyle;
      m_lineJoinStyle = lineJoinStyle;
    }
    void resetStyleOverride()
    {
      SETBIT_0(m_uFlags, kLwdOverrideStyle);
      m_lineCapStyle = OdPs::kLesRound;
      m_lineJoinStyle = OdPs::kLjsRound;
    }
    bool hasStyleOverride() const
    {
      return GETBIT(m_uFlags, kLwdOverrideStyle);
    }
    OdPs::LineEndStyle endStyleOverride() const
    {
      return m_lineCapStyle;
    }
    OdPs::LineJoinStyle joinStyleOverride() const
    {
      return m_lineJoinStyle;
    }
    // Helpers
    bool hasOverrides() const
    {
      return m_uFlags != 0;
    }
    bool operator ==(const OdGiLineweightOverride &lwdO2) const
    {
      if (lwdO2.m_uFlags != m_uFlags)
        return false;
      if (hasScaleOverride())
      {
        if (!OdEqual(lwdO2.m_dLwdScale, m_dLwdScale))
          return false;
      }
      if (hasStyleOverride())
      {
        if (lwdO2.m_lineCapStyle != m_lineCapStyle ||
            lwdO2.m_lineJoinStyle != m_lineJoinStyle)
          return false;
      }
      return true;
    }
    bool operator !=(const OdGiLineweightOverride &lwdO2) const
    {
      if (lwdO2.m_uFlags != m_uFlags)
        return true;
      if (hasScaleOverride())
      {
        if (!OdEqual(lwdO2.m_dLwdScale, m_dLwdScale))
          return true;
      }
      if (hasStyleOverride())
      {
        if (lwdO2.m_lineCapStyle != m_lineCapStyle ||
            lwdO2.m_lineJoinStyle != m_lineJoinStyle)
          return true;
      }
      return false;
    }
};

/** \details
    Helper class to simplify lineweight override setting.
  
    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiLineweightOverrideHelper
{
  protected:
    OdGiSubEntityTraits *m_pTraits;
  public:
    OdGiLineweightOverrideHelper() : m_pTraits(NULL) {}
    OdGiLineweightOverrideHelper(OdGiSubEntityTraits &pTraits, const OdGiLineweightOverride &pOverride) : m_pTraits(NULL)
    { setLineweightOverride(pTraits, pOverride); }
    ~OdGiLineweightOverrideHelper()
    {
      if (m_pTraits)
        m_pTraits->popLineweightOverride();
    }
    void setLineweightOverride(OdGiSubEntityTraits &pTraits, const OdGiLineweightOverride &pOverride)
    {
      m_pTraits = &pTraits;
      m_pTraits->pushLineweightOverride(&pOverride);
    }
};

#include "TD_PackPop.h"

#endif //_ODGILINEWEIGHTOVERRIDE_INCLUDED_
