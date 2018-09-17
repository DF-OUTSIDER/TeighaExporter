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

#ifndef _Path_Util_H_
#define _Path_Util_H_


#include "OdString.h"
#include "OdArray.h"
#include <string.h>


namespace OD {
  class Path;

  class Path : public OdArray<OdString> {
    Path& parse(const OdAnsiString& name, const char* delimeters = "/\\");
    Path& parse(const OdString& name, const OdChar* delimeters = L"/\\");
    Path& add(const Path& path);
    Path& add(const OdAnsiString& part);
    void append(const OdAnsiString& part);
    void append(const Path& path);
  public:
    Path() {}
    Path(const OdAnsiString& path);
    Path(const OdString& path);
    Path(const OdChar* path);
    Path(const char* path);
    static const char* Walk(const char* szPath, const char* delimeters = "/\\");
    Path  operator +  (const OdAnsiString& path) const;
    Path  operator +  (const OdString& path) const;
    Path  operator +  (const char* path) const;
    Path  operator +  (const OdChar* path) const;
    Path  operator +  (const Path& path) const;
    Path& operator += (const OdAnsiString& path);
    Path& operator += (const OdString& path);
    Path& operator += (const char* path);
    Path& operator += (const OdChar* path);
    Path& operator += (const Path& path);
    OdString asStr(bool asdir = false, char sepch = '/') const;
  };


  inline void Path::append(const OdAnsiString & part) { 
    if(size()>0 && part=="..") {
      removeLast();
    } else {
      OdArray<OdString>::append(part);
    }
  }
  inline void Path::append(const Path & path) { 
    for(size_type i=0; i<path.size(); ++i) {
      append(path[i]);
    }
  }
  inline const char* Path::Walk(const char* path, const char* delimeters) {
    char ch[] = " ";
    while((ch[0] = *path)) {
      if(::strpbrk(delimeters, ch))
        break;
      ++path;
    }
    return path;
  }
  inline Path& Path::parse(const OdAnsiString& path, const char* delimeters) {
    OdAnsiString astr(path);
    for(int i=0; !astr.isEmpty(); i=0) {
      i = astr.findOneOf(delimeters);
      if(i==0) {
        astr = astr.right(astr.getLength()-1);
        continue;
      }
      if(i==-1) {
        append(astr);
        break;
      } else {
        append(astr.left(i));
        ++i;
        astr = astr.right(astr.getLength()-i);
      }
    } 
    return *this;
  }
  inline Path& Path::parse(const OdString& path, const OdChar* delimeters) {
    return Path::parse(OdAnsiString(OdString(path)), OdAnsiString(OdString(delimeters)));
  }
  inline Path::Path(const OdChar* name) { parse(OdString(name)); }
  inline Path::Path(const char* name) { parse(OdAnsiString(name)); }
  inline Path::Path(const OdString& path) { parse(path); }
  inline Path::Path(const OdAnsiString& path) { parse(path); }

  inline Path& Path::add(const OdAnsiString& part) { append(part); return *this; }
  inline Path& Path::add(const Path& path) { append(path); return *this; }

  inline Path  Path::operator + (const OdAnsiString& path) const { return Path(*this).add( Path(path) ); }
  inline Path  Path::operator + (const OdString& path) const { return Path(*this).add( Path(path) ); }
  inline Path  Path::operator + (const char* path) const { return Path(*this).add( Path(path) ); }
  inline Path  Path::operator + (const OdChar* path) const { return Path(*this).add( Path(path) ); }
  inline Path  Path::operator + (const Path& path) const { return Path(*this).add( path ); }
  inline Path& Path::operator +=(const OdAnsiString& path) { return add( Path(path) ); }
  inline Path& Path::operator +=(const OdString& path) { return add( Path(path) ); }
  inline Path& Path::operator +=(const char* path) { return add( Path(path) ); }
  inline Path& Path::operator +=(const OdChar* path) { return add( Path(path) ); }
  inline Path& Path::operator +=(const Path& path) { return add( path ); }

  inline OdString Path::asStr(bool asdir, char sepch) const {
    OdString res, sep(sepch, 1);
    if(empty())
      return res;
    res = at(0);
    for(size_type i=1; i<size(); ++i) {
      res += (sep + at(i));
    }
    if(asdir)
      res += sep;
    return res;
  }
} //namespace OD



#endif //#ifndef _Path_Util_H_
