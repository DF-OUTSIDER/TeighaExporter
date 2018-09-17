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




#ifndef _OD_CHARMAPPER_H_
#define _OD_CHARMAPPER_H_

#include "TD_PackPush.h"

#include "OdCodePage.h"
#include "OdError.h"
#include "OdArray.h"

class OdStreamBuf;
class OdString;
typedef OdArray <char, OdMemoryAllocator<char> > OdAnsiCharArray;
typedef OdArray<OdChar, OdMemoryAllocator<OdChar> > OdCharArray;


/** \details
    This class implements character mapping.
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdCharMapper
{
  static OdString m_MapFile;
private:
	OdCharMapper();
public:
//	static OdResult initialize(OdStreamBuf* pIo);

  /** \details
    Initializes this CharMapper object from the specified mapping file.
    \param filename [in]  File name.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult initialize(const OdString& filename);

  /** \details
    Maps the specified Unicode character to the specified code page.
    \param sourceChar [in]  Unicode source character.
    \param codepageId [in]  Object ID of the code page.
    \param codepageChar [out]  Receives the code page character.
    \param bTryToUseSystemCP [in]   Try to use default ANSI code page and OEM  code page in case conversion with codepageId is not OK
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult unicodeToCodepage(OdChar sourceChar,
    OdCodePageId codepageId,
    OdChar& codepageChar,
    bool bTryToUseSystemCP = true );

  /** \details
  Maps the specified Unicode character to the specified code page using IMLangFontLink2 interface.
  \param sourceChar [in]  Unicode source character.
  \param codepageId [in]  Object ID of the code page.
  \param codepageChar [out]  Receives the code page character.
  \returns
  Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult unicodeToCodepage2(OdChar sourceChar,
    OdCodePageId codepageId,
    OdChar& codepageChar);

  /** \details
    Maps the specified code page character to Unicode.
    \param sourceChar [in]  Code page source character.
    \param codepageId [in]  Object ID of the code page.
    \param unicodeChar [out]  Receives the Unicode character.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult codepageToUnicode(OdChar sourceChar,
    OdCodePageId codepageId,
    OdChar& unicodeChar);

  /** \details
    Returns true if and only if the specified byte is one of the leading bytes of the specified code page.
    \param testByte [in]  Byte to test.
    \param codepageId [in]  Object ID of the code page.
  */
  static bool isLeadByte(OdUInt8 testByte, OdCodePageId codepageId);

  /** \details
    Returns the code page with the specified description.
    \param description [in]  Description.  
    \param codepageId [out]  Receives the object ID of the code page.  
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult codepageDescToId(const OdString& description, OdCodePageId& codepageId);

  /** \details
    Returns the description for the specified code page.
    \param description [out]  Receives the description.  
    \param codepageId [in]  Object ID of the code page.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  static OdResult codepageIdToDesc(OdCodePageId codepageId, OdString& description);

  /** \details
    Returns the number of valid code pages for this CharMapper object.
  */
  static OdUInt32 numValidCodepages();

  /** \details
    Returns the code page corresponding to the specified ANSI code page.
    \param ansiCodePage [in]  ANSI code page.
  */
  static OdCodePageId ansiCpToAcadCp(OdUInt32 ansiCodePage);

  /** \details
    Returns the ANSI code page corresponding to the specified code page.
    \param acadCodePageId [in]  code page.
  */
  static OdUInt32 acadCpToAnsiCp(OdCodePageId acadCodePageId);

  /** \details
    Returns the code page corresponding to the specified ANSI character set.
    \param ansiCharacterSet [in]  Character Set.
  */
   static OdCodePageId getCodepageByCharset(OdUInt16 ansiCharacterSet);

   static OdUInt16 getReorderCharsetByChar(OdChar ch);

   /** \details
   Returns true if and only if the specified code page supports conversion.
   \param codepageId [in]  Object ID of the code page.
   */
   static bool isConversionSupported(OdCodePageId codepageId);

   static void wideCharToMultiByte(OdCodePageId codePage, const OdChar* srcBuf, int srcSize, OdAnsiCharArray& dstBuf);
   // Returns number of characters (! not dstBuf length)
   static void multiByteToWideChar(OdCodePageId codePage, const char* srcBuf, int srcSize, OdCharArray& dstBuf);

   // This is actually CESU-8 encoding used in DXF (http://www.unicode.org/reports/tr26/)
   static void utf8ToUnicode(const char* srcBuf, int srcSize, OdCharArray& dstBuf);
   static void unicodeToUtf8(const OdChar* srcBuf, int srcSize, OdAnsiCharArray& dstBuf);

   /** \details
     Adds the bigFont to map.
     \param bigFont [in]  big font file name.  
     \param cpIndex [in]  code page index.
     
     \remarks
     cpIndex must be one of the following:
     
     <table>
     Value    Description
     1        CP_ANSI_932
     2        CP_ANSI_950
     3        CP_ANSI_949
     5        CP_ANSI_936
     </table>

     Returns eOk if successful, or an appropriate error code if not.
   */
   static OdResult addBigFontWithIndex(const OdString& bigFont, OdInt32 cpIndex);

   /** \details
     Adds the bigFont to map.
     \param bigFont [in]  big font file name.  
     \param codePageId [in]  CodePage ID.

     \remarks
     codePageId must be one of the following:
     
     @untitled table
     CP_ANSI_932
     CP_ANSI_950
     CP_ANSI_949
     CP_ANSI_936

     Returns eOk if successful, or an appropriate error code if not.
   */
   static OdResult addBigFontWithCodepage(const OdString& bigFont, OdCodePageId codePageId);

   /** \details
     Adds the bigfont list from io stream to map.
     \param io [in]  stream with next format.

     Anything after a '#' character is a comment (and ignored)
     The ';' character is used to separate the file name from the code page index
  
     <BIGFONTFILENAME.SHX>;<CODE PAGE INDEX>
   
     There should be no leading or trailing spaces for the filename
    
     JAPANESE_CODEPAGE_INDEX = 1,
     TRADITIONAL_CHINESE_CODEPAGE_INDEX = 2,
     KOREAN_CODEPAGE_INDEX = 3,
     KOREAN_JOHAB_CODEPAGE_INDEX = 4,
     SIMPLIFIED_CHINESE_CODEPAGE_INDEX = 5
    
     Returns eOk if successful, or an appropriate error code if not.
   */
   static OdResult addBigFonts(OdStreamBuf* io);

   /** \details
     Returns the OdCodePageId.
     \remarks
     getCpByBigFont returns one of the following:
     
     @untitled table
     CP_ANSI_932
     CP_ANSI_950
     CP_ANSI_949
     CP_ANSI_936
     \param bigFont [in]  big font file name.  
   */

   static OdCodePageId getCpByBigFont(const OdString& bigFont);
   /** \details
     Returns the code page index.
     \remarks
     getCpIndexByBigFont returns one of the following:
     
     <table>
     Value     Description
     1         CP_ANSI_932
     2         CP_ANSI_950
     3         CP_ANSI_949
     5         CP_ANSI_936
     \param bigFont [in]  big font file name.  
     </table>
   */
   static OdInt32 getCpIndexByBigFont(const OdString& bigFont);

   /** \details
     Returns the check sum for an ANSI string.
     \param str [in]  String for calculating check sum.  
   */
   static double getCheckSumAnsi(OdAnsiString str);

   /** \details
     Returns the check sum for a unicode string.
     \param str [in]  String for calculating check sum.  
   */
   static double getCheckSumUnicode(OdString str);

   static OdString convertCIFcoding(const OdString& strText);
};

// Surrogate pairs support
template<class T> inline bool odIsLeadSurrogate(T chr) {
  return (chr >= 0xD800 && chr <= 0xDFFF);
}
template<class T> inline bool odIsTailSurrogate(T chr) {
  return (chr >= 0xDC00 && chr <= 0xDFFF);
}
template<class T1, class T2> inline T1 odToCodepoint(T1 chr, T2 chr2) {
  return (chr << 10) + chr2 + (0x10000 - (0xD800 << 10) - 0xDC00);
}
template<class T> inline OdUInt32 odToCodeValue(T chr, T chr2) {
  return ((OdUInt32)chr << 10) + chr2 + (0x10000 - (0xD800 << 10) - 0xDC00);
}
template<class T> inline bool odIsCodepoint(T chr) {
  return (chr >= 0x10000 && chr <= 0x10FFFF);
}
template<class T> inline T odToLeadSurrogate(T chr) {
  return ((0xD800 - (0x10000 >> 10)) + (chr >> 10));
}
template<class T> inline T odToTailSurrogate(T chr) {
  return (0xDC00 + (chr & 0x3FF));
}

enum OdAsianCpIndex
{
  JAPANESE_CP_INDEX = 1,              // 932
  TRADITIONAL_CHINESE_CP_INDEX = 2,   // 950
  KOREAN_WANSUNG_CP_INDEX = 3,        // 949
  KOREAN_JOHAB_CP_INDEX = 4,          // 1361
  SIMPLIFIED_CHINESE_CP_INDEX = 5     // 936
};

/** \details
  Returns the specified text converted from one code page to another.
  \param source [in]  Source text.
  \param sourceId [in]  Object ID of source code page.
  \param destId [in]  Object ID of destination code page.
*/

#include "TD_PackPop.h"

#endif

