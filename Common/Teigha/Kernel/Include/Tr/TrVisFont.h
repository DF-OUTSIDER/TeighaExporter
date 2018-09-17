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
// GLES2 font

#ifndef ODTRVISFONT
#define ODTRVISFONT

#include "TD_PackPush.h"
#include "TrVisTexture.h"
class OdTrVisRendition;

class OdTrVisFont : public OdRxObject
{
protected:
  OdTrVisFont();
  void setup(const OdChar* fontName, OdUInt32 fontSize, OdUInt32 fontWeight, OdUInt8 flags = 0, OdTrVisRendition* pRendition = NULL);
public:

  struct UVRect
  {
    double m_minX;
    double m_minY;
    double m_maxX;
    double m_maxY;
    void setZero()
    {
      m_minX = m_minY = m_maxX = m_maxY = 0.0;
    }
  };
  enum OdTrVisFontFlag
  {
    kNone = 0,
    kBold = 1,
    kItalic = 1 << 1,
    kUnderline = 1 << 2,
    kStrikeout = 1 << 3
  };

  static OdSmartPtr<OdTrVisFont> createFont(const OdChar* fontName, OdUInt32 fontSize, OdUInt32 fontWeight, OdUInt8 flags = 0, OdTrVisRendition* pRendition = NULL);
  ~OdTrVisFont();

  OdTrVisTexturePtr getTextureForLetter( char c, UVRect& uvRect ) const;
  OdTrVisTexturePtr getTexture() const;
  OdString fontName() const { return m_fontName; }
  OdUInt32 fontSize() const { return m_fontSize; }
  OdUInt32 fontWeight() const { return m_fontWeight; }
  void getLetterSize( OdUInt32& w, OdUInt32& h ) const;

protected:
  OdString m_fontName;
  OdUInt32 m_fontSize;
  OdUInt32 m_fontWeight;
  OdUInt8  m_flags;
  OdTrVisRendition* m_pRendition;

  mutable OdTrVisTexturePtr m_pTexture;
};

typedef OdSmartPtr<OdTrVisFont> OdTrVisFontPtr;

#include "TD_PackPop.h"

#endif // ODTRVISFONT
