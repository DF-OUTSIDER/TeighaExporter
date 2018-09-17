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
// OdFakeDbStub.h

#ifndef _OD_FAKE_DBSTUB_INCLUDED_
#define _OD_FAKE_DBSTUB_INCLUDED_

// FakeDbStub redesigned to way of CspGsModel (to be compatible with current Core/Extensions/ExRender/GLES2/remote/C-API.cpp)
// see also  Core/Extensions/ExRender/GLES2/remote/CspGsModel.h
#ifndef _ODDB_DBSTUB_INCLUDED_

/** \details
    <group ExRender_Classes>
*/
class OdFakeDbStub {
public:
  OdFakeDbStub(OdUInt64 handle = 0, void* held = 0) 
  {
    this->handle = handle;
    this->held = held;
  }
  OdUInt64 handle;
  void* held;
};
#else
#  define OdFakeDbStub OdDbStub
#endif //_ODDB_DBSTUB_INCLUDED_

inline int _hexDigit(char c) 
{
  int res = (((c >= '0' && c <= '9') ? c - '0' 
                                     : (c >= 'A' && c <= 'F') ? c - 'A' + 10 
                                                              : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : -1));
  return res;
}

inline OdUInt64 odStrToUInt64(const char* hex) 
{
  OdUInt64 res = 0;
  for (;;)
  {
    int digit = _hexDigit(*hex++);
    if(digit<0)
      break;
    res = (res << 4) | digit;
  }
  return res;
}

#endif //_OD_FAKE_DBSTUB_INCLUDED_
