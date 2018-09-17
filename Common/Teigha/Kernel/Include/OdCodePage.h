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




#ifndef _ODCODE_PAGE_INCLUDED_
#define _ODCODE_PAGE_INCLUDED_

typedef enum 
{
  CP_UNDEFINED = 0,   // undefined
  CP_ASCII     = 1,   // ascii
  CP_8859_1    = 2,   // iso8859-1
  CP_8859_2    = 3,   // iso8859-2
  CP_8859_3    = 4,   // iso8859-3   
  CP_8859_4    = 5,   // iso8859-4   
  CP_8859_5    = 6,   // iso8859-5
  CP_8859_6    = 7,   // iso8859-6
  CP_8859_7    = 8,   // iso8859-7
  CP_8859_8    = 9,   // iso8859-8
  CP_8859_9    = 10,  // iso8859-9
  CP_DOS437    = 11,  // dos437 
  CP_DOS850    = 12,  // dos850 
  CP_DOS852    = 13,  // dos852 
  CP_DOS855    = 14,  // dos855 
  CP_DOS857    = 15,  // dos857 
  CP_DOS860    = 16,  // dos860 
  CP_DOS861    = 17,  // dos861 
  CP_DOS863    = 18,  // dos863 
  CP_DOS864    = 19,  // dos864 
  CP_DOS865    = 20,  // dos865 
  CP_DOS869    = 21,  // dos869 
  CP_DOS932    = 22,  // dos932 
  CP_MACINTOSH = 23,  // mac-roman
  CP_BIG5      = 24,  // big5
  CP_KSC5601   = 25,  // ksc5601   
  CP_JOHAB     = 26,  // johab     
  CP_DOS866    = 27,  // dos866    
  CP_ANSI_1250 = 28,  // ansi 1250 
  CP_ANSI_1251 = 29,  // ansi 1251 
  CP_ANSI_1252 = 30,  // ansi 1252 
  CP_GB2312    = 31,  // gb2312    
  CP_ANSI_1253 = 32,  // ansi 1253 
  CP_ANSI_1254 = 33,  // ansi 1254 
  CP_ANSI_1255 = 34,  // ansi 1255 
  CP_ANSI_1256 = 35,  // ansi 1256 
  CP_ANSI_1257 = 36,  // ansi 1257 
  CP_ANSI_874  = 37,  // ansi 874  
  CP_ANSI_932  = 38,  // ansi 932  
  CP_ANSI_936  = 39,  // ansi 936  
  CP_ANSI_949  = 40,  // ansi 949  
  CP_ANSI_950  = 41,  // ansi 950  
  CP_ANSI_1361 = 42,  // ansi 1361 
  CP_ANSI_1200 = 43,  // ansi 1200 
  CP_ANSI_1258 = 44,  // ansi 1258 
  CP_UTF_8     = 45,
  CP_CNT       = 46   // Current ( returned by OdRxSystemServices::systemCodePage() )
} OdCodePageId;


#endif // _ODCODE_PAGE_INCLUDED_

