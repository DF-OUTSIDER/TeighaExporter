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

#ifndef _OD_FONT_TABLE_H_INCLUDED_
#define _OD_FONT_TABLE_H_INCLUDED_

#include "OdMutex.h"
#include "RxObject.h"
#include "DbRootExport.h"
#include "OdString.h"
#include "OdFont.h"
#include "Gi/TtfDescriptor.h"

#define  STL_USING_MAP
#include "OdaSTL.h"

class OdFontStore
{
public:
  OdString  m_fileName;
  OdFontPtr m_font;
};

typedef std::map<OdString, OdFontStore> OdFontMap;
class OdFontTable;
typedef OdSmartPtr<OdFontTable> OdFontTablePtr;

class DBROOT_EXPORT OdFontTable : public OdMutex, public OdRxObject
{
  OdFontPtr m_DefaultShxFont;
  OdFontPtr m_DefaultGdtFont;
  OdFontPtr m_DefaultShapeFont;

  static bool createShxFontKey(const OdString& fileName, OdString& resKey);

  void _setDefaultShxFont(OdFont* pFont);

  void _setDefaultGdtFont(OdFont* pFont);

  OdFontPtr lookUp(const OdString& uniqueName, OdString& fileName);

  void addFont(const OdString& uniqueName, OdFont* pFont, const OdString& fileName);

  OdFontPtr createTtf(const OdString& fileName, const OdTtfDescriptor& descr);

  OdFontPtr createShx(const OdString& fileName);

  OdFontPtr _getFont(const OdTtfDescriptor& descr, const OdString& fileName);

  OdFontPtr _getFont(const OdString& fileName);

  OdFontPtr _getFontAt(OdUInt32 nPos);

  void _removeFontAt(OdUInt32 nPos);

private:
  static OdFontTablePtr s_FontTable;
protected:
  OdFontMap m_FontMap;
public:
  OdFontTable();
  ~OdFontTable();
  
  static OdFontTable* fontTable();
  static void init();
  static void uninit();
  void clear();
  OdFontPtr defaultFont()
  {
    TD_AUTOLOCK(*this);
    OdFontPtr pRes = m_DefaultShxFont;
    return pRes;
  }
  OdFontPtr defaultGdtFont()
  {
    TD_AUTOLOCK(*this);
    OdFontPtr pRes = m_DefaultGdtFont;
    return pRes;
  }
  OdFontPtr defaultShapeFont()
  {
    TD_AUTOLOCK(*this);
    OdFontPtr pRes = m_DefaultShapeFont;
    return pRes;
  }
  static void setDefaultShxFont(OdFont* pFont);

	static void setDefaultGdtFont(OdFont* pFont);

  static OdFontPtr getFont(const OdString& fileName);

  static OdFontPtr getFont(const OdTtfDescriptor& descr, const OdString& fileName);

  static OdString getFontKey(const OdTtfDescriptor& descr);

  static OdFontPtr getFontByKey(const OdString& fontKey, OdString& fileName);

  static OdFontPtr getFontAt(OdUInt32 nPos);

  static void removeFontAt(OdUInt32 nPos);

  static void addFontByKey(const OdString& fontKey, OdFont* pFont, const OdString& fileName);
};

typedef OdSmartPtr<OdFontTable> OdFontTablePtr;

#endif //_OD_FONT_TABLE_H_INCLUDED_
