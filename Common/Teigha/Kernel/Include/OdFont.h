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




// OdFont.h: interface for the OdFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODFONT_H__0B76813A_DCFA_450E_8591_B6C6F1ED76EC__INCLUDED_)
#define AFX_ODFONT_H__0B76813A_DCFA_450E_8591_B6C6F1ED76EC__INCLUDED_

#include "RxObject.h"
#include "OdArray.h"
#include "OdCharMapper.h"

// For memset below
#include <memory.h>

class OdGiCommonDraw;
class OdGiConveyorGeometry;
class OdGePoint2d;
class OdGePoint3d;
class OdStreamBuf;

#include "TD_PackPush.h"

typedef enum {
  kFontTypeUnknown    = 0,  // Unknown.
  kFontTypeShx        = 1,  // SHX font.
  kFontTypeTrueType   = 2,  // TrueType font.
  kFontTypeShape      = 3,  // Shape file.
  kFontTypeBig        = 4,  // BigFont file.
  kFontTypeRsc        = 5   // MicroStation Recourse file.
} OdFontType;

/** \details
    This structure implements character properties in an OdGi context. 
    Corresponding C++ library: TD_Root
    <group !!RECORDS_tkernel_apiref>
*/
struct FIRSTDLL_EXPORT OdCharacterProperties
{
  OdCharacterProperties()
  { ::memset(this, 0, sizeof(*this)); }

  bool  bUnderlined;
  bool  bOverlined;
  bool  bStriked;
  bool  bLastChar;
  bool  bInBigFont;
  bool  bAsian;
  bool  bValid;
};

/** \details
    This structure implements text properties in an OdGi context.
    Corresponding C++ library: TD_Root
    <group !!RECORDS_tkernel_apiref>
*/
struct FIRSTDLL_EXPORT OdTextProperties
{
  OdUInt16 m_flags;
  double   m_trackingPercent;
  OdUInt16 m_textQuality;
  enum
  {
    kNormalText   = 0x01,
    kVerticalText = 0x02,
    kUnderlined   = 0x04,
    kOverlined    = 0x08,
    kLastChar     = 0x10,
    kInBigFont    = 0x20,
    kInclPenups   = 0x40,
    kZeroNormals  = 0x80,
    kBezierCurves = 0x100,
    kStriked      = 0x200,
    kLastPosOnly  = 0x400    // For internal use only
  };
  OdChar  m_prevChar;
public:
  OdTextProperties() : m_flags(0), m_trackingPercent(0.0), m_textQuality(50), m_prevChar(0) {}
  /** \details
    Returns true if and only if the Normal Text flag is set for this TextProperties object.
  */
  bool isNormalText() const { return GETBIT(m_flags, kNormalText); }
  /** \details
    Controls the Normal Text flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setNormalText(bool value) { SETBIT(m_flags, kNormalText, value); }
  /** \details
    Returns true if and only if the Vertical Text flag is set for this TextProperties object.
  */
  bool isVerticalText() const { return GETBIT(m_flags, kVerticalText); }
  /** \details
    Controls the Vertical Text flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setVerticalText(bool value) { SETBIT(m_flags, kVerticalText, value); }
  /** \details
    Returns true if and only if the Underlined flag is set for this TextProperties object.
  */
  bool isUnderlined() const { return GETBIT(m_flags, kUnderlined); }
  /** \details
    Controls the Underlined flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setUnderlined(bool value) { SETBIT(m_flags, kUnderlined, value); }
  /** \details
    Returns true if and only if the Overlined flag is set for this TextProperties object.
  */
  bool isOverlined() const { return GETBIT(m_flags, kOverlined); }
  /** \details
    Controls the Overlined flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setOverlined(bool value) { SETBIT(m_flags, kOverlined, value); }
  /** \details
    Returns true if and only if the Striked flag is set for this TextProperties object.
  */
  bool isStriked() const { return GETBIT(m_flags, kStriked); }
  /** \details
    Controls the Striked flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setStriked(bool value) { SETBIT(m_flags, kStriked, value); }
  /** \details
  Returns true if and only if the LastChar flag is set for this TextProperties object.
  */

  /** \details
    Returns true if and only if the LastChar flag is set for this TextProperties object.
  */
  bool isLastChar() const { return GETBIT(m_flags, kLastChar); }
  /** \details
    Controls the LastChar flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setLastChar(bool value) { SETBIT(m_flags, kLastChar, value); }
  /** \details
    Returns true if and only if the InBigFont flag is set for this TextProperties object.
  */
  bool isInBigFont() const { return GETBIT(m_flags, kInBigFont); }
  /** \details
    Controls the InBigFont flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setInBigFont(bool value) { SETBIT(m_flags, kInBigFont, value); }
  /** \details
    Returns true if and only if the IncludePenups flag is set for this TextProperties object.
  */
  bool isIncludePenups() const { return GETBIT(m_flags, kInclPenups); }
  /** \details
    Controls the IncludePenups flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setIncludePenups(bool value) { SETBIT(m_flags, kInclPenups, value); }
  /** \details
    Returns true if and only if the ZeroNormals flag is set for this TextProperties object.
  */
  bool isZeroNormals() const { return GETBIT(m_flags, kZeroNormals); }
  /** \details
    Controls the kZeroNormals flag for this TextProperties object.
    \param value [in]  Controls the flag.
  */
  void setZeroNormals(bool value) { SETBIT(m_flags, kZeroNormals, value); }
  /** \details
    Returns smooth TTF text contour output flag.
  */
  bool ttfPolyDraw() const { return GETBIT(m_flags, kBezierCurves); }
  /** \details
    Sets smooth TTF text contour output flag.

    bFlag (in) New flag value.
  */
  void setTtfPolyDraw(bool bFlag) { SETBIT(m_flags, kBezierCurves, bFlag); }
  /** \details
    Returns the tracking percent for this TextProperties object.
  */
  double trackingPercent() const {return m_trackingPercent; }
  /** \details
    Sets the tracking percent for this TextProperties object.
    \param trackingPercent [in]  Tracking percent.
  */
  void setTrackingPercent(double trackingPercent) { m_trackingPercent = trackingPercent; }

  /** \details
    Returns the text quality percent for this vectorization process.
    \returns
    Returns a value in the range [0,100]. 50 is the default.
  */
  OdUInt32 textQuality() const { return m_textQuality; }

  /** \details
    Returns the text quality percent for this vectorization process.
    \remarks
    Sets a value in the range [0,100]. 50 is the default.
  */
  void setTextQuality(OdUInt32 val) { m_textQuality = (OdUInt16)val; }

  // For internal use only
  bool isLastPosOnly() const { return GETBIT(m_flags, kLastPosOnly); }
  void setLastPosOnly(bool value) { SETBIT(m_flags, kLastPosOnly, value); }
};

typedef OdUInt32 OdFontSubType;

class OdGePoint2d;
class OdGiConveyorGeometry;
class OdTtfDescriptor;

/** \details
    This class is the base interface class for classes that implements font objects.
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdFont : public OdRxObject
{
  OdUInt32 m_Flags;

public:
  ODRX_DECLARE_MEMBERS(OdFont);

  OdFont() : m_Flags(0) {}

  enum
  {
    kBigFont10 = 0x0001,
    kUniFont10 = 0x0002,
    kFont10    = 0x0004,
    kFont11    = 0x0008,
    kFont10A   = 0x0010,
    kTrueType  = 0x0020,
    kFontGdt   = 0x0040,
    kFontSimplex6 = 0x0080,
    kShapes11   = 0x0100,
    kFontRsc    = 0x0200   // MicroStation Resource file.
  };

  /** \details
    Returns the font flags for this Font object.
    \remarks
    getFlags() returns a combination of zero or more of the following: 
    
    <table>
    Name              Value
    kBigFont10        0x0001
    kUniFont10        0x0002
    kFont10           0x0004
    kFont11           0x0008
    kFont10A          0x0010
    kTrueType         0x0020
    kFontGdt          0x0040
    kFontSimplex6     0x0080
    kShapes11         0x0100
    kFontRsc          0x0200
    </table>
  */
    OdUInt32 getFlags() const { return m_Flags; }
  /** \details
    Returns the font flags for this Font object.
    
    \remarks
    flags() returns a combination of zero or more of the following: 
    
    <table>
    Name              Value
    kBigFont10        0x0001
    kUniFont10        0x0002
    kFont10           0x0004
    kFont11           0x0008
    kFont10A          0x0010
    kTrueType         0x0020
    kFontGdt          0x0040
    kFontSimplex6     0x0080
    kShapes11         0x0100
    kFontRsc          0x0200
    </table>
  */
    OdUInt32 flags() { return m_Flags; }

  /** \details
    Sets the font flags for this Font object.
    \param fontFlags [in]  Font flags.

    \remarks
    fontFlags must be a combination of zero or more of the following:
    
    <table>
    Name              Value
    kBigFont10        0x0001
    kUniFont10        0x0002
    kFont10           0x0004
    kFont11           0x0008
    kFont10A          0x0010
    kTrueType         0x0020
    kFontGdt          0x0040
    kFontSimplex6     0x0080
    kShapes11         0x0100
    kFontRsc          0x0200
    </table>
  */
    void setFlags(OdUInt32 fontFlags) { m_Flags = fontFlags; }

  /** \details
    Adds the specified font flags to this Font object.
    \param fontFlags [in]  Font flags.

    \remarks
    fontFlags must be a combination of zero or more of the following:
     
    <table>
    Name              Value
    kBigFont10        0x0001
    kUniFont10        0x0002
    kFont10           0x0004
    kFont11           0x0008
    kFont10A          0x0010
    kTrueType         0x0020
    kFontGdt          0x0040
    kFontSimplex6     0x0080
    kShapes11         0x0100
    kFontRsc          0x0200
    </table>
  */
    void addFlag(OdUInt32 fontFlags) { m_Flags |= fontFlags; }

  /** \details
    Initializes this Font object to read font data from the specified StreamBuf object.

    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.    
  */
      virtual OdResult initialize(OdStreamBuf* pStreamBuf) = 0;
  /** \details
    Draws the specified character with this Font object.
    \param character [in]  Character to draw.
    \param advance [in]  Offset to next character.
    \param pWd [in/out] Draw object that can be used by this function to set the 
              necessary attributes and create the necessary geometry.
    \param textProperties [in] Text properties.
  */
  virtual OdResult     drawCharacter(OdChar character, OdGePoint2d& advance, OdGiCommonDraw* pWd,
                                     OdTextProperties& textProperties) = 0;

  /** \details
    Draws the specified character with this Font object.
    \param character [in]  Character to draw.
    \param advance [in]  Offset to next character.
	\param pGeometry [in] Pointer to the modeler geometry.
    \param textProperties [in] Text properties.
  */
  virtual OdResult     drawCharacter(OdChar character, OdGePoint2d& advance,
                                     OdGiConveyorGeometry* pGeometry,
                                     OdTextProperties& textProperties) = 0;
  /** \details
    Returns the height of this Font object above the baseline.
  */
  virtual double       getAbove() const = 0;
  /** \details
    Returns the depth of this font object below the baseline.
  */
  virtual double       getBelow() const = 0;
  /** \details
    Returns an array of the characters defined in this Font object.
    \param characters [out]  Receives the array of characters.
  */
  virtual OdUInt32     getAvailableChars(OdCharArray& characters) = 0;
  /** \details
    Returns true if and only if the specified character is defined in this Font object.
    \param character [in]  Character.
  */
  virtual bool        hasCharacter(OdChar character) = 0;
  
  /** \details
    Returns the height of this Font object.
  */
  virtual double      getHeight() const                                                   // MKU 20.02.2003
  {
    return getAbove() + getBelow();
  }
  /** \details
    Returns the internal leading of this Font object.
    \remarks
    The default implementation of this function always returns 0.
  */
  virtual double getInternalLeading() const
  {
    return 0;
  }


  //  removed here from GiContextForDbDatabase.cpp              // MKU 04.03.2003

  /** \details
    Returns the height of this Font object above the baseline.
    \remarks
    The default implementation of this function returns getAbove(), unless it is 0.0, in which case this function returns 1.0. 
  */
  double fontAbove() const  
  {
    double above = getAbove();
    if(OdZero(above))
    {
      above = 1.0;
    }
    return above;
  }
  /** \details
    Returns the underline position for this Font object at the specified text size.
    \param textSize [in]  Text size.
    \remarks
    The default implementation of this function returns -2.0 * textSize.
  */
  virtual double getUnderlinePos(double textSize) const
  {
    return -0.2 * textSize;
    
  }
  /** \details
    Returns the overline position for this Font object at the specified text size.
    \param textSize [in]  Text size.
    \remarks
    The default implementation of this function returns 1.2 * textSize.
  */
  virtual double getOverlinePos(double textSize) const
  {
    return 1.2 * textSize;
  }
  
  /** \details
    Returns true if and only if this Font object is an SHX font.
    \remarks
    The default implementation of this function always returns true.
  */
  virtual bool isShxFont()
  {
    return true;
  }
  /** \details
    Returns the average width of characters in this Font object.
    \remarks
    The default implementation of this function always returns 0.0.
  */
  virtual double getAverageWidth()
  {
    return 0.0;
  }

  /** \details
    Returns arrays of points over and under the baseline for the specified character in this OdFont object.
    \param character [in]  Character.
    \param advance [in]  Offset to next character.
    \param pointsOver [in]  Array of points over the baseline..
    \param pointsUnder [in]  Array of points under the baseline.
    \param textFlags [in] Text flags.

    \remarks
    textFlags must be a combination of one or more of the following:
 
    <table>
    Name                                Value
    OdTextProperties::kNormalText       0x01
    OdTextProperties::kVerticalText     0x02
    OdTextProperties::kUnderlined       0x04
    OdTextProperties::kOverlined        0x08
    OdTextProperties::kLastChar         0x10
    OdTextProperties::kInBigFont        0x20
    OdTextProperties::kInclPenups       0x40
    </table>
    
    \remarks
    The default implementation of this function does nothing but return.
    
  */
  virtual void getScore( OdUInt16 character, 
                         OdGePoint2d& advance, 
                         OdGePoint3d* pointsOver,
                         OdGePoint3d* pointsUnder,
                         const OdTextProperties& textFlags);

  /** \details
    Returns the font data for the specified character.
    \param dwTable [in]  Metric table to query.
    \param dwOffset [in]  Offset into table.
    \param pBuffer [in]  Pointer to to the buffer to receive the data.
    \param cbData [in]  ???.
    \returns
    Returns the number of bytes returned in buffer.
    
    \remarks
    The default implementation of this function does nothing but return -1.
  */
  virtual OdUInt32 getFontData(
    OdUInt32 dwTable,     // metric table to query
    OdUInt32 dwOffset,    // offset into table being queried
    void * pBuffer,  // pointer to buffer for returned data
    OdUInt32 cbData       ) const;

  /** \details
    Returns true if and only if the font supports vertical mode.
    
    \remarks
    The default implementation of this function returns false.
  */
  virtual bool supportsVerticalMode()
  {
    return false;
  }

  /** \details
    Returns font file name.

    \remarks
    The default implementation of this function returns empty string.
  */
  virtual OdString getFileName() const
  {
    return OdString();
  }
  virtual void getDescriptor(OdTtfDescriptor& descr)
  {
  }

  /** \details
    Returns the LOGFONT for this font

    \param lpLogFont [in] Pointer to the buffer to receive the data.
    \returns
    Returns the number of bytes returned in buffer. In case lpLogFont is NULL methods return
    number of bytes required in buffer. 
    
    \remarks
    The default implementation of this function does nothing but return -1.
  */
  virtual OdInt32 getLogFont(void* lpLogFont) const
  {
    return -1;
  }
};

inline void OdFont::getScore( OdUInt16 /*character*/, 
                         OdGePoint2d& /*advance*/, 
                         OdGePoint3d* /*pointsOver*/,
                         OdGePoint3d* /*pointsUnder*/,
                         const OdTextProperties& /*flags*/ )
  {}

inline OdUInt32 OdFont::getFontData(
    OdUInt32 /*dwTable*/,     // metric table to query
    OdUInt32 /*dwOffset*/,    // offset into table being queried
    void * /*lpvBuffer*/,  // pointer to buffer for returned data
    OdUInt32 /*cbData*/       ) const
  {
    return OdUInt32(-1);
  }


/** \details
    This template class is a specialization of the OdSmartPtr class for OdFont object pointers.
*/
typedef OdSmartPtr<OdFont> OdFontPtr;

/** \details
  This class is the base interface class for text iterators, to be used both by Teigha and DGNdirect clients.
  
  <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdBaseTextIterator : public OdRxObject
{
public:
  virtual OdChar nextChar() = 0;
  /** \details
  Returns the properties of the character at the current position of this TextIterator object.
  */
  virtual const OdCharacterProperties& currProperties() const = 0;

  /** \details
  Returns the character at the current position of this TextIterator object.
  */
  virtual const OdChar* currPos() const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdBaseTextIterator object pointers.
*/
typedef OdSmartPtr<OdBaseTextIterator> OdBaseTextIteratorPtr;

#include "TD_PackPop.h"

#endif // !defined(AFX_ODFONT_H__0B76813A_DCFA_450E_8591_B6C6F1ED76EC__INCLUDED_)

