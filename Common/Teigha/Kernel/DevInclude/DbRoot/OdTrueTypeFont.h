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


#ifndef _OD_TRUETYPEFONT_INCLUDED_
#define _OD_TRUETYPEFONT_INCLUDED_

#define  STL_USING_MAP
#include "OdaSTL.h"

#include "OdTrueTypeFontBase.h"
#include "DbRootExport.h"
#include "Ge/GeBoundBlock3d.h"
#include "OdMutex.h"
#include "OdCodePage.h"
#include "OdStreamBuf.h"
#include "Gi/TtfDescriptor.h"
#include "Gi/OdPolyPolygon3d.h"
#include "IntArray.h"
#include "UInt8Array.h"

class OdGiConveyorGeometry;

class ODRX_ABSTRACT DBROOT_EXPORT OdTrueTypeFont : public OdTrueTypeFontBase
{
protected:
  OdMutex m_Mutex;
	void*   m_Data;
	double  m_Above;
	double  m_Below;          // it is kept with (-) sign
  double  m_AveWidth;
  double  m_Ascent;
	bool    m_bMultibyte;
  bool    m_bPseudoItalic;
  bool    m_bPseudoBold;
  double  m_Height;                                                                       // MKU 21.02.2003
  double  m_InternalLeading; 
  OdGeBoundBlock3d  m_BBox;
	OdStreamBufPtr    m_Io;
	OdUInt32          m_RawTTFlags;
	OdCodePageId      m_Codepage;
  OdUInt32  m_fontLanguageInfo;
  OdString       m_fileName;
  double  m_UnderlinePos;
  double  m_UnderlineThickness;
  double  m_MacAscent;
  double  m_CapEmHeight;
  double  m_UnitsPerEM;
  bool    m_bIsPostScript;  // it is true if PostScript, else font is TrueType

  void outCharacter(void *data, OdGiConveyorGeometry* pGeom, const OdTextProperties& textFlags, void* prevData = NULL);
	void drawScore(double advance, OdGiCommonDraw* pWd, OdGeBoundBlock3d& bbox, OdTextProperties& textFlags);
  void processContour(void* data);
  // vectorized glyph outline data from OdTtfInfo::m_outline to OdTtfInfo::m_Ppg;
  // if deviation is 0.0, then it calculates it with own algorithm
  // (OdGeNurbCurve3d::appendSamplePoints() does that)
  void vectorizeOutline(void *data, double deviation = 0.0);

public:
  ODRX_DECLARE_MEMBERS(OdTrueTypeFont);
  OdTrueTypeFont();

	void      setRawTTFlags(OdUInt32 rawFlags);
  bool      isMultiByte()   const { return m_bMultibyte; }
	OdUInt32  getRawTTFlags() const { return m_RawTTFlags; }

  OdCodePageId getCodepage() { return m_Codepage; }


	virtual OdResult  initialize(OdStreamBuf* io) = 0;
	virtual OdResult  initialize(const OdString& fileName, const OdTtfDescriptor& desc) = 0;
  virtual OdResult  drawCharacter(OdChar character,
		                              OdGePoint2d& advance,
																  OdGiCommonDraw* pWd,
																  OdTextProperties& textFlags) = 0;
  virtual OdResult  drawCharacter(OdChar character, OdGePoint2d& advance,
                                  OdGiConveyorGeometry* pGeom,
                                  OdTextProperties& textFlags) = 0;

  virtual double    getAbove() const 
  { 
    return m_Above; 
  }
  virtual double    getBelow() const 
  { 
    return m_Below; 
  }
  virtual double    getHeight() const                                                     // MKU 20.02.2003
  {
    return m_Height;
  }
  double getAscent() const
  {
    return m_Ascent;
  }
  virtual double getInternalLeading() const
  {
    return m_InternalLeading;
  }
  virtual OdUInt32  getAvailableChars(OdCharArray& retArray) = 0;
  virtual bool      hasCharacter(OdChar character);

  virtual double getUnderlinePos(double textSize) const
  {
    return m_Below * textSize/fontAbove();
  }
  virtual double getOverlinePos(double textSize) const
  {
    return m_Ascent * textSize/fontAbove();
  }
  virtual bool isShxFont()
  {
    return false;
  }
  virtual double getAverageWidth()
  {
    return m_AveWidth;
  }

  virtual void getScore( OdChar character, 
                         OdGePoint2d& advance, 
                         OdGePoint3d* pointsOver,
                         OdGePoint3d* pointsUnder,
                         const OdTextProperties& flags ) = 0;

  virtual OdUInt32 getFontData(
    OdUInt32 dwTable,     // metric table to query
    OdUInt32 dwOffset,    // offset into table being queried
    void *   lpvBuffer,   // pointer to buffer for returned data
    OdUInt32 cbData       ) const = 0;

  virtual void getGlyhpIndexes(const OdCharArray& srcArray, OdCharArray& glyphs, 
              OdIntArray* visualOrderFromLogical = NULL) = 0;
  virtual bool isSupportReorder() = 0;

  // Surrogate pairs support
  virtual bool isSupportSurrogatePairs() const { return false; }
  virtual OdResult drawCharacter(OdChar character1, OdChar /*character2*/,
                                 OdGePoint2d& advance,
                                 OdGiCommonDraw* pWd,
                                 OdTextProperties& textFlags) {
    return drawCharacter(character1, advance, pWd, textFlags);
  }
  virtual OdResult drawCharacter(OdChar character1, OdChar /*character2*/, OdGePoint2d& advance,
                                 OdGiConveyorGeometry* pGeom,
                                 OdTextProperties& textFlags) {
    return drawCharacter(character1, advance, pGeom, textFlags);
  }
  virtual bool hasCharacter(OdChar character1, OdChar /*character2*/) { return hasCharacter(character1); }
  //

  void setFileName(const OdString& fileName) { m_fileName = fileName; }
  OdString getFileName() const { return m_fileName; }
  virtual OdInt32 getLogFont(void* lpLogFont) const { return -1; }
  virtual OdTtfDescriptor ttfdescriptor() const
  {
    OdTtfDescriptor desc;
    desc.setFileName(m_fileName);
    return desc;
  }

  virtual OdString getSubstituteFontByChar(OdChar unicodeChar) const
  {
    return OdString::kEmpty;
  }

  double getUnderlinePosition() const
  {
    return m_UnderlinePos;
  }
  double getUnderlineThickness() const
  {
    return m_UnderlineThickness;
  }
  double getMacAscent() const
  {
    return m_MacAscent;
  }
  double getCapEmHeight() const
  {
    return m_CapEmHeight;
  }
  double getUnitsPerEM() const
  {
    return m_UnitsPerEM;
  }
  bool isPostScript() const
  {
    return m_bIsPostScript;
  }
};

typedef OdSmartPtr<OdTrueTypeFont> OdTrueTypeFontPtr;

typedef struct
{
  double x;
  double y;
  OdChar flag;
} DPOINT;

/** Description:
Representation of TTF glyph outlines, containing straight line segments and spline segments.
So points represent either polygon vertices or spline control points.

{group:Other_Classes}
*/
class OdPolyCurve3d : public OdPolyPolygon3d
{
  OdInt32Array m_curvePointsIndices; // indices of 1st control points for each spline segment
public:
  /** Description:
  Adds index of the last point to the control points indices array.
  */
  void addCurvePointIndex() { m_curvePointsIndices.append(points().size()-1); }
  /** Description:
  Returns the indices of curve (spline control) points.
  */
  const OdInt32Array& curvePointsIndices() const { return m_curvePointsIndices; }
};


//Stores information for a single TT character.
class OdTtfInfo
{
public:
  double            m_deviation; // deviation used for vectorizing m_outline to m_Ppg
  OdPolyCurve3d     m_outline;

  OdPolyPolygon3d   m_Ppg;
  OdInt32Array      m_Faces;
  OdInt32           m_TotalPoints;

  OdUInt8Array      m_polyFlags;

  OdGeBoundBlock3d  m_BoundingBox;
  OdGePoint2d       m_Advance;
  bool              m_HasPenStrokes;

  OdTtfInfo() : m_TotalPoints(0), m_deviation(0.), m_HasPenStrokes(true)
  {}
  ~OdTtfInfo() {}
};

typedef std::map<OdChar, OdTtfInfo*> OdaGlyphMap;

class ODRX_ABSTRACT DBROOT_EXPORT OdTrueTypeEngine : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdTrueTypeEngine);
  virtual OdTrueTypeFontPtr createFont(const OdString& fileName, OdTtfDescriptor& desc) = 0;
};

typedef OdSmartPtr<OdTrueTypeEngine> OdTrueTypeEnginePtr;

#ifdef OD_WINDOWS_DESKTOP
class OdTrueTypeEngineWin : public OdTrueTypeEngine
{
public:
  ODRX_DECLARE_MEMBERS(OdTrueTypeEngineWin);
  virtual OdTrueTypeFontPtr createFont(const OdString& fileName, OdTtfDescriptor& desc);
};

class OdTrueTypeFontWin : public OdTrueTypeFont
{
  ODRX_DECLARE_MEMBERS(OdTrueTypeFontWin);
  OdTrueTypeFontWin(){}
  ~OdTrueTypeFontWin();
protected:
  void initAbove(TextRenderingMode textRenderingMode);
  void outCharacter(void *data, OdGiCommonDraw* pWd);
  OdResult getBBox(OdChar character, OdGeBoundBlock3d& bbox, OdGePoint2d& advance);
  OdResult createOutlineAndDraw(OdChar character,
    OdTtfInfo*& pTtfInfo,
    OdGePoint2d& advance,
    OdGiCommonDraw* pWd,
    OdGeBoundBlock3d* bbox,
    OdTextProperties textFlags,
    bool bDraw = false);
public:
  virtual void getDescriptor(OdTtfDescriptor& descr);
  virtual OdResult  initialize(OdStreamBuf* io);
  virtual OdResult  initialize(const OdString& fileName, const OdTtfDescriptor& desc);
  virtual OdResult  drawCharacter(OdChar character,
    OdGePoint2d& advance,
    OdGiCommonDraw* pWd,
    OdTextProperties& textFlags);
  virtual OdResult  drawCharacter(OdChar character, OdGePoint2d& advance,
    OdGiConveyorGeometry* pGeom,
    OdTextProperties& textFlags);
  virtual OdUInt32  getAvailableChars(OdCharArray& retArray);
  virtual bool      hasCharacter(OdChar character);
  virtual void getScore( OdChar character, 
    OdGePoint2d& advance, 
    OdGePoint3d* pointsOver,
    OdGePoint3d* pointsUnder,
    const OdTextProperties& flags );

  virtual OdUInt32 getFontData(
    OdUInt32 dwTable,     // metric table to query
    OdUInt32 dwOffset,    // offset into table being queried
    void *   lpvBuffer,   // pointer to buffer for returned data
    OdUInt32 cbData       ) const;

  virtual void getGlyhpIndexes(const OdCharArray& srcArray, OdCharArray& glyphs,
          OdIntArray* visualOrderFromLogical = NULL);
  virtual bool isSupportReorder();
  virtual OdInt32 getLogFont(void* lpLogFont) const;

  virtual OdString getSubstituteFontByChar(OdChar unicodeChar) const;
};
#endif
class OdTrueTypeEngineFT : public OdTrueTypeEngine
{
public:
  ODRX_DECLARE_MEMBERS(OdTrueTypeEngineFT);
  OdTrueTypeEngineFT(){}
  virtual OdTrueTypeFontPtr createFont(const OdString& fileName, OdTtfDescriptor& desc);
};
class OdTrueTypeFontFT : public OdTrueTypeFont
{
public:
  ODRX_DECLARE_MEMBERS(OdTrueTypeFontFT);
  OdTrueTypeFontFT(){}
  ~OdTrueTypeFontFT();
protected:
  void initAbove(TextRenderingMode textRenderingMode);
  void outCharacter(void *data, OdGiCommonDraw* pWd);
  OdResult getBBox(OdChar character, OdGeBoundBlock3d& bbox, OdGePoint2d& advance);
  OdResult createOutlineAndDraw(OdChar character,
    OdGePoint2d& advance,
    OdGiCommonDraw* pWd,
    OdGeBoundBlock3d& bbox,
    OdTextProperties textFlags,
    bool bDraw = false);
public:
  virtual void getDescriptor(OdTtfDescriptor& descr);
  virtual OdResult  initialize(OdStreamBuf* io);
  virtual OdResult  initialize(const OdString& fileName, const OdTtfDescriptor& desc);
  virtual OdResult  drawCharacter(OdChar character,
    OdGePoint2d& advance,
    OdGiCommonDraw* pWd,
    OdTextProperties& textFlags);
  virtual OdResult  drawCharacter(OdChar character, OdGePoint2d& advance,
    OdGiConveyorGeometry* pGeom,
    OdTextProperties& textFlags);
  virtual OdUInt32  getAvailableChars(OdCharArray& retArray);
  virtual bool      hasCharacter(OdChar character);
  virtual void getScore( OdChar character, 
    OdGePoint2d& advance, 
    OdGePoint3d* pointsOver,
    OdGePoint3d* pointsUnder,
    const OdTextProperties& flags );

  virtual OdUInt32 getFontData(
    OdUInt32 dwTable,     // metric table to query
    OdUInt32 dwOffset,    // offset into table being queried
    void *   lpvBuffer,   // pointer to buffer for returned data
    OdUInt32 cbData       ) const;

  virtual void getGlyhpIndexes(const OdCharArray& srcArray, OdCharArray& glyphs, 
          OdIntArray* visualOrderFromLogical = NULL);
  virtual bool isSupportReorder();
};


#define FONT_ENGINE L"TT_FONT_ENGINE"

#endif // _OD_TRUETYPEFONT_INCLUDED_
