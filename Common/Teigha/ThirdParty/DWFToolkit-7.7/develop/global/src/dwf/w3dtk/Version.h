//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
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


#ifndef _W3DTK_VERSION_H
#define _W3DTK_VERSION_H

///
///\file        dwf/w3dtk/Version.h
///

#include "dwf/Version.h"


//
//
// W3D Toolkit Versioning
//
//

#define _W3DTK_VERSION_MAJOR                        1
#define _W3DTK_VERSION_MINOR                        7
#define _W3DTK_VERSION_HOOPS                        1555
#define _W3DTK_VERSION_BUILD                        _DWFTK_VERSION_BUILD

#define _W3DTK_VERSION_ECHO( s )                    #s
#define _W3DTK_VERSION_STRING_( a, b, c, d )        _W3DTK_VERSION_ECHO(a) "." _W3DTK_VERSION_ECHO(b) "." _W3DTK_VERSION_ECHO(c) "." _W3DTK_VERSION_ECHO(d) "\0"
#define _W3DTK_VERSION_STRING                       _W3DTK_VERSION_STRING_( _W3DTK_VERSION_MAJOR, _W3DTK_VERSION_MINOR, _W3DTK_VERSION_HOOPS, _W3DTK_VERSION_BUILD )

#define _W3D_FILE_VERSION_STRING                    "W3D V01.00"

#endif
