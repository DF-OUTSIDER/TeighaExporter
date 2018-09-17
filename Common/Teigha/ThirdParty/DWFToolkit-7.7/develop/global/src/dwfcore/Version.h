//
//  Copyright (c) 2003-2008 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFCORE_VERSION_H
#define _DWFCORE_VERSION_H


#define _DWFCORE_VERSION_MAJOR      1
#define _DWFCORE_VERSION_MINOR      7
#define _DWFCORE_VERSION_EXTRA      0
#define _DWFCORE_VERSION_BUILD      19


#define _DWFCORE_VERSION_ECHO( s )                  #s
#define _DWFCORE_VERSION_STRING_( a, b, c, d )      _DWFCORE_VERSION_ECHO(a) "." _DWFCORE_VERSION_ECHO(b) "." _DWFCORE_VERSION_ECHO(c) "." _DWFCORE_VERSION_ECHO(d) "\0"
#define _DWFCORE_VERSION_STRING                     _DWFCORE_VERSION_STRING_( _DWFCORE_VERSION_MAJOR, _DWFCORE_VERSION_MINOR, _DWFCORE_VERSION_EXTRA, _DWFCORE_VERSION_BUILD )

#endif
