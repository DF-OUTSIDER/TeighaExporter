//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//

#if !defined XVERSION_HEADER
#define XVERSION_HEADER

///
///\file        dwf/XAML/xversion.h
///

#include "resource.h"
#include "../Version.h"

// Define XAML toolkit version information
//
// These are used in XamlFile.h in toolkit_major_revision() and toolkit_minor_revision()
// in the WT_XAML_File object.  Also in file.h they are munged together and used as a
// single integer in toolkit_decimal_revision(), to be later disected and placed
// into a W2X file document element attribute.
#define WD_XAML_Toolkit_Major_Revision           7
#define WD_XAML_Toolkit_Minor_Revision           0

#define REVISION_WHEN_DWFX_FORMAT_BEGINS         700

//
// Define DLL version
//

#define _XAMLDLL_MAJOR_VERSION     7
#define _XAMLDLL_MINOR_VERSION     7
#define _XAMLDLL_EXTRA_VERSION     0
#define _XAMLDLL_BUILD_VERSION     _DWFTK_VERSION_BUILD

#define _XAMLTK_VERSION_ECHO( s )               #s
#define _XAMLTK_VERSION_STRING_( a, b, c, d )   _XAMLTK_VERSION_ECHO(a) "." _XAMLTK_VERSION_ECHO(b) "." _XAMLTK_VERSION_ECHO(c) "." _XAMLTK_VERSION_ECHO(d) "\0"
#define _XAMLTK_VERSION_STRING                  _XAMLTK_VERSION_STRING_( _XAMLDLL_MAJOR_VERSION, _XAMLDLL_MINOR_VERSION, _XAMLDLL_EXTRA_VERSION, _XAMLDLL_BUILD_VERSION )

// The WHIP_BUILD_STRING is used as the file version identifier on all
// the DLLs so whip.cab will download new versions as we update the DLLs.
// We put it in this form to allow it to be used in the Win32 versioning mechanism.
// This is also the product version of WHIP!. It is common to all unique components
// of WHIP!, i.e. not Heidi since Heidi has its own version.
//#define STRINGIZE_WHIP_BUILD(major, minor, extra, build) ECHO(major) "." ECHO(minor) "." ECHO(extra) "." ECHO(build) "\0"
//#define WHIP_BUILD_STRING                         STRINGIZE_WHIP_BUILD(WHIPDLL_MAJOR_VERSION, WHIPDLL_MINOR_VERSION, WHIPDLL_EXTRA_VERSION, WHIPDLL_BUILD_VERSION)
#define _XAML_BUILD_STRING   _XAMLTK_VERSION_STRING

#endif // XVERSION_HEADER
