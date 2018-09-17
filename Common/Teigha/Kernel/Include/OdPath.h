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

#ifndef _ODPATH_INCLUDED_
#define _ODPATH_INCLUDED_

#include "OdString.h"

namespace Oda
{

// Support methods operating with relative paths:
//   isRelativePath()
//   makeRelativePath()
//   adjustPath()
//
#ifdef ODA_UNIXOS
#define pathChar '/'
#define dirDiv L"/"
#define dirCur L"./"
#define dirUp L"../"
inline bool	isRelativePath(const OdChar* path)
{
  return path && path[0] != L'/';
}
#else
#define pathChar '\\'
#define dirDiv L"\\"
#define dirCur L".\\"
#define dirUp L"..\\"

inline bool	isUNCPath(const OdChar* path)
{
  return Od_wcslen(path) >= 4 && // "\\a" can't be a UNC path
    path[0] == pathChar && path[1] == pathChar && path[2] != pathChar;
}

inline bool	isRelativePath(const OdChar* path)
{
  return path && odWStrChr(path, ':') == 0 && !isUNCPath(path);
}
#endif

FIRSTDLL_EXPORT bool makeRelativePath(const OdString& from, const OdString& to, OdString& out, bool windowsMode = false);
FIRSTDLL_EXPORT bool adjustPath(OdString& path, bool windowsMode = false);

} // namespace Oda

#endif // _ODPATH_INCLUDED_
