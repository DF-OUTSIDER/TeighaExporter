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


#ifndef _OD_TEXTITERATOR_INCLUDED_
#define _OD_TEXTITERATOR_INCLUDED_


#include "DbRootExport.h"
#include "OdString.h"
#include "OdCodePage.h"
#include "OdFont.h"
#include "Gi/GiTextStyle.h"

class OdGeVector3d;

class DBROOT_EXPORT OdTextIterator
{
  const OdChar*         m_Text;
  int                   m_nLen;
  OdCharacterProperties m_CurrProps;
  OdCodePageId          m_Codepage;
  OdCodePageId          m_localCodepage;
  OdChar                m_CurrentChar;
  bool                  m_CurrentValid;
  bool                  m_CurrIsToleranceDivider;

  OdChar appendDigits(int numDigits, bool hex = true);

  OdUInt16 countDigits();
  OdChar defaultNextChar();

protected:
  OdFont*               m_pBigFont;
  OdFont*               m_pFont;
  const OdChar*         m_pCurr;
  bool                  m_DoParsePercentPercent;
  bool                  m_bProcessToleranceDivider;
  bool                  m_bProcessMIF;
  OdChar                m_UnicodeCurrChar;

  virtual OdChar nextCharAsIs()
  {
    OdChar ch = *m_pCurr;
    m_CurrentChar = *m_pCurr++;
    return ch; //m_CurrentChar;
  }

#ifdef TD_UNICODE
  OdString degree_ucode()
  {
    return OdString(0x00B0, 1);
  }
  OdString plus_minus_ucode()
  {
    return OdString(0x00B1, 1);
  }
  OdString diameter_ucode()
  {
    return OdString(0x00D8, 1);
  }
#else
  OdString degree_ucode()
  {
    return OdString(OD_T("\\U+00B0"));
  }
  OdString plus_minus_ucode()
  {
    return OdString(OD_T("\\U+00B1"));
  }
  OdString diameter_ucode()
  {
    return OdString(OD_T("\\U+00D8"));
  }
#endif

public:
  const OdChar* currPos() const { return m_pCurr; }

  OdTextIterator() {}
	OdTextIterator(const OdChar* str, int nLen, bool bRaw, OdCodePageId codepage, OdFont* pFont, OdFont* pBigFont);
	virtual ~OdTextIterator();

  OdChar operator *();
  const OdCharacterProperties& currProperties() const { return m_CurrProps; }
  OdChar nextChar();
  bool currIsToleranceDivider() const { return m_CurrIsToleranceDivider; }
  void setProcessToleranceDivider(bool b = true)
  {
    m_bProcessToleranceDivider = b;
  }
  void getTextAsDByte(OdCharArray& retArray);
  int breakSafely(int len, OdString& s);
  void setProcessMIF(bool b)
  {
    m_bProcessMIF = b;
  }

#ifdef TD_UNICODE
  OdAnsiString convertToCodePage(OdCodePageId bigFontCP, OdCodePageId localCp);
#else
  OdString convertToCodePage(OdCodePageId codepage);
#endif
};

/*
inline bool OdIsdigit(OdChar c)
{
  return (c >= '0' && c <= '9');
}
*/
#ifdef TD_UNICODE
inline OdString convertTextToCodePage(const OdString& source, OdCodePageId /*from*/, OdCodePageId /*to*/)
{
  return source;
}
#else
inline OdString convertTextToCodePage(const OdString& source, OdCodePageId from, OdCodePageId to)
{
  if ( from != CP_UNDEFINED && to != CP_UNDEFINED && from != to )
  {
    OdTextIterator iter(source, -1, false, from, NULL, NULL);
    return iter.convertToCodePage(to);
  }
  return source;
}
#endif

class DBROOT_EXPORT OdAnsiTextIterator
{
  const char*         m_Text;
  const char*         m_pCurr;
  const char*         m_pEnd;
  int                 m_nLen;
  OdCodePageId        m_Codepage;
  bool                m_bMultibyteCP;
  OdChar              m_CurrentChar;
  const char*         m_pHoldCharPos;

  OdChar appendDigitsNoCheck(int numDigits, bool hex = true);
  OdChar appendDigits(int numDigits, bool hex = true);
  void init(const OdAnsiString& str, OdCodePageId codepage);

private:
  // To protect from usage
  OdAnsiTextIterator() {}

public:
  static bool OdAnsiTextIteratorNotRequired(const OdAnsiString& str, OdCodePageId codepage);

  OdAnsiTextIterator(const OdAnsiString& str, OdCodePageId codepage);
  ~OdAnsiTextIterator();

  const char* currPos() const { return m_pCurr; }
  OdChar   operator *();
  OdChar   nextChar();
  int breakSafely(int len, OdAnsiString& s);

};

class DBROOT_EXPORT LineStats {
public:
  double m_Height;

  double m_Width;
  double m_MinY;
  double m_MaxY;
  double m_MinX;
  double m_MaxX;
  double m_TextSizeMaxY;

  double m_MaxTextSize;
  double m_StackedHeight;

  double m_LastWordHeight;
  double m_TextSizeForLinespace;

  LineStats(double h = 0.0, double mts = 0.0, double w = 0.0) :
    m_Height(h), m_Width(w), m_MaxTextSize(mts), m_MinY(0.0), m_MaxY(0.0),
    m_MinX(0.0), m_MaxX(0.0),
    m_StackedHeight(0.0), m_LastWordHeight(0.0), m_TextSizeForLinespace(0.0),
    m_TextSizeMaxY(0.0){}

  double getHeightForAligned(int linespacingStyle);
  double caclulateLineSpace(double linespacingFactor);
};

DBROOT_EXPORT OdCodePageId mapCodepage(OdInt16 cp);
DBROOT_EXPORT OdInt16 reMapCodepage(OdCodePageId id);
DBROOT_EXPORT bool isAsianCodepage(OdCodePageId codePage);
DBROOT_EXPORT OdCodePageId GetCPageIdByName(const OdString& CodePage);
DBROOT_EXPORT const OdString getCodePageStr(int index);
DBROOT_EXPORT bool IsMultiByteCodePage(OdCodePageId id);
DBROOT_EXPORT OdString oddbConvertTextToCodePage(const OdString& source, OdCodePageId sourceId, OdCodePageId destId);
DBROOT_EXPORT OdChar checkSpecialSymbol(OdFont* pFont, OdChar sym, bool& isSpecSym );

#endif // _OD_TEXTITERATOR_INCLUDED_
