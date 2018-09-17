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


#ifndef __ODCHARCONVERTER_H__
#define __ODCHARCONVERTER_H__

#include "OdaCommon.h"
#include "OdCharMapper.h"
#include "OdArray.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace OdCharConverter
{
  template <class CharType> inline bool isDigit(CharType ch)
  {
    return (ch >= '0' && ch <= '9');
  }

  template <class CharType> inline bool isHexDigit(CharType ch)
  {
    return (ch >= '0' && ch <= '9') ||
          (ch >= 'a' && ch <= 'f') ||
          (ch >= 'A' && ch <= 'F');
  }

  template <class CharType> inline bool checkDigits(const CharType* ptr, int numDigits, bool hex = true)
  {
    for (int i = 0; i < numDigits; i++)
    {
      if ( !(hex ? isHexDigit(ptr[i]) : isDigit(ptr[i])) )
      {
        return false;
      }
    }
    return true;
  }

  template <class CharType> inline char isCIForMIF(const CharType* ptr)
  {
    if (*ptr == '\\')
    {
      CharType ch2 = *++ptr;
      if (*ptr++ == '+')
      {
        if ((ch2 == 'U' || ch2 == 'u') && checkDigits(ptr, 4))
          return 'u';
        
        if ((ch2 == 'M' || ch2 == 'm') && (*ptr >= '1' && *ptr <= '5') && checkDigits(++ptr, 4))
          return 'm';
      }
    }
    return '\0';
  }

  template <class CharType> inline bool isCIF(const CharType* ptr)
  {
    if ( !ptr )
      return false;
    return (ptr[0] == '\\' && ( ptr[1] == 'U' || ptr[1] == 'u' ) && ptr[2] == '+' && 
      checkDigits(&ptr[3], 4) );
  }

  template <class CharType> inline bool isMIF(const CharType* ptr)
  {
    if ( !ptr )
      return false;
    return (ptr[0] == '\\' && ( ptr[1] == 'M' || ptr[1] == 'm' ) && ptr[2] == '+' && 
      checkDigits(&ptr[4], 4) && ptr[3] >= '1' && ptr[3] <= '5');
  }

  template <class CharType> inline unsigned int hexValue(const CharType c)
  {
    if (c >= '0' && c <= '9') return(c - '0');
    if (c >= 'A' && c <= 'F') return(c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return(c - 'a' + 10);
    return(0);
  }

  inline int getMIFIndex(OdCodePageId id )
  {
    switch(id)
    {
      case CP_ANSI_932:
      case CP_DOS932:      return '1';
      case CP_ANSI_950:    
      case CP_BIG5:        return '2';
      case CP_ANSI_949:    
      case CP_KSC5601:     return '3';
      case CP_ANSI_1361:   
      case CP_JOHAB:       return '4';
      case CP_ANSI_936:    
      case CP_GB2312:      return '5';
      default:             return '0';
    }
  }

  template <class CharType> inline int getMIFString(OdChar val, OdCodePageId cp, CharType* dstBuf, int size)
  {
    if ( size < 8 )
      return 0;  
    dstBuf[0] = '\\';
    dstBuf[1] = 'M';
    dstBuf[2] = '+';
    dstBuf[3] = (CharType)getMIFIndex(cp);
    for( int i = 0; i < 4; i++ )
    {
      CharType byteVal = (CharType)(val & 0x0f);
      dstBuf[7 - i] = CharType((byteVal <= 9) ? ('0' + byteVal) : ('A' - 10 + byteVal));
      val >>= 4;
    }
    return 8;
  }

  template <class CharType> inline int getCIFString(OdUInt16 val, CharType* dstBuf, int size)
  {
    if ( size < 7 )
      return 0;  
    dstBuf[0] = '\\';
    dstBuf[1] = 'U';
    dstBuf[2] = '+';
    for( int i = 0; i < 4; i++ )
    {
      CharType byteVal = (CharType)(val & 0x0f);
      dstBuf[6 - i] = CharType((byteVal <= 9) ? ('0' + byteVal) : ('A' - 10 + byteVal));
      val >>= 4;
    }
    return 7;
  }

  template <class CharType> inline OdCodePageId getMIFCodepage(CharType cp)
  {
    switch(cp)
    {
      case '1':    return CP_ANSI_932;
      case '2':    return CP_ANSI_950;
      case '3':    return CP_ANSI_949;
      case '4':    return CP_ANSI_1361;
      case '5':    return CP_ANSI_936;
      default:     return CP_UNDEFINED;
    }
  }

  template <class CharType> inline  bool parseMIFString(CharType* srcBuf, OdChar& val, OdCodePageId& cp)
  {
    if ( !isMIF<CharType>(srcBuf) )
      return false;

    cp = getMIFCodepage(srcBuf[3]);
    val = OdChar(
        (hexValue(srcBuf[4]) << 12)
      + (hexValue(srcBuf[5]) << 8)
      + (hexValue(srcBuf[6]) << 4)
      + hexValue(srcBuf[7]));
    return true;
  }

  template <class CharType> inline bool parseCIFString(CharType* srcBuf, OdChar& val)
  {
    if ( !isCIF<CharType>(srcBuf) )
      return false;

    val = OdChar(
        (hexValue(srcBuf[3]) << 12)
      + (hexValue(srcBuf[4]) << 8)
      + (hexValue(srcBuf[5]) << 4)
      + hexValue(srcBuf[6]));
    return true;
  }

  inline bool isMBCBCodepage(OdCodePageId id)
  {
    return id == CP_ANSI_932 ||
      id == CP_ANSI_936 ||
      id == CP_ANSI_949 ||
      id == CP_ANSI_950 ||
      id == CP_ANSI_1361 ||
      id == CP_BIG5 ||
      id == CP_JOHAB ||
      id == CP_GB2312 ||
      id == CP_KSC5601 ||
      id == CP_DOS932;
  }

  inline bool isMIFCodepage(OdCodePageId id)
  {
    return isMBCBCodepage(id);
  }


  inline OdCodePageId checkTheSameCP(OdCodePageId cp)
  {
    switch (cp)
    {
      case CP_DOS932:  return CP_ANSI_932;
      case CP_BIG5:    return CP_ANSI_950;
      case CP_KSC5601: return CP_ANSI_949;
      case CP_GB2312:  return CP_ANSI_936;
      case CP_JOHAB:   return CP_ANSI_1361;
      default:break;
    }
    return cp;
  }
}

#endif // __ODCHARCONVERTER_H__
