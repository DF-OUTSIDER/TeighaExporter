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

#if !defined WVERSION_HEADER
#define WVERSION_HEADER

///
///\file        dwf/whiptk/wversion.h
///

#include "dwf/Version.h"


// Define DWF toolkit version information
//
// These are used in file.h in toolkit_major_revision() and toolkit_minor_revision()
// in the WT_File object.  Also in file.h they are munged together and used as a
// single integer in toolkit_decimal_revision(), to be later disected and placed
// into the DWF file header.
#define WD_Toolkit_Major_Revision           6
#define WD_Toolkit_Minor_Revision           1

//
// Define DLL version
//
// 01/21/05 - The module versioning has gotten a bit out of date
// and is now going to be changed.  Previously, the major version
// reflected the Heidi major version and the minor version
// reflected the stream major version. The previous values were:
//
//#define WHIPDLL_MAJOR_VERSION     7
//#define WHIPDLL_MINOR_VERSION     WD_Toolkit_Major_Revision
//#define WHIPDLL_EXTRA_VERSION     1
//#define WHIPDLL_BUILD_VERSION     23
//
// From now on, the versioning will be independently adjusted to
// reflect modifications to the API and improvements in the code.
// However, the third field (the extra version) will reflect the
// current version of the W2D graphics stream format, similar to
// how the W3D Toolkit version reflects the version of the HSF
// stream toolkit it uses.
//
#define WHIPDLL_MAJOR_VERSION     7
#define WHIPDLL_MINOR_VERSION     13
#define WHIPDLL_EXTRA_VERSION     601
#define WHIPDLL_BUILD_VERSION     _DWFTK_VERSION_BUILD


//The below #defines indicate versions (WD_Toolkit_Major_Revision*100 + WD_Toolkit_Minor_Revision)
//when certain major stream events occurred, to be used in backwards-compatability code
#define WHIP05_DWF_FILE_VERSION                                 17
#define REVISION_WHEN_HISTORY_BUFFER_PRELOADED                  23
#define WHIP10_DWF_FILE_VERSION                                 25
#define WHIP20_DWF_FILE_VERSION                                 30
#define REVISION_WHEN_SET_FONT_STRING_FIXED                     31
#define REVISION_WHEN_DRAW_TEXT_ATTRIBUTES_REMOVED              32
#define REVISION_WHEN_BINARY_COLOR_CHANGED_TO_RGBA_FROM_BGRA    33
#define WHIP30_DWF_FILE_VERSION                                 34
#define REVISION_WHEN_COMMA_FOR_DECIMAL_CORRECTED               35
#define REVISION_WHEN_UNDERLINING_WAS_ADDED                     35
#define HOMER_DWF_FILE_VERSION                                  35
#define REVISION_WHEN_STRINGS_CHANGED_TO_UNICODE_FROM_ASCII     36
#define WHIP31_DWF_FILE_VERSION                                 36
#define REVISION_WHEN_HEIDI_DRAWING_CHANGED                     37
#define REVISION_WHEN_WIDE_LINES_ADDED                          37
#define REVISION_WHEN_MULTI_URLS_WERE_SUPPORTED                 37
#define REVISION_WHEN_DEFAULT_COLORMAP_WAS_CHANGED              38
#define REVISION_WHEN_ZLIB_COMPRESSION_IS_SUPPORTED             39
#define REVISION_WHEN_LANDSCAPE_FLIP_ADDED_TO_PLOTINFO          40
#define REVISION_WHEN_ROTATION_ADDED_TO_PLOTINFO                41
#define REVISION_WHEN_CONTOUR_SET_USED_FOR_VIEWPORTS            42
#define WHIP40_DWF_FILE_VERSION                                 42
#define WHIP50_DWF_FILE_VERSION                                 50
#define REVISION_WHEN_READ_WRITE_MODE_WAS_SUPPORTED             55
#define WHIP52_DWF_FILE_VERSION                                 55
//Note, this is correct.  Version 600 is the first "major revision" the format has undergone.
#define REVISION_WHEN_PACKAGE_FORMAT_BEGINS                    600

// The following also defines when Polymarker is deprecated from the API
#define REVISION_WHEN_MACRO_IS_SUPPORTED                       601

#define REVISION_WHEN_TEXT_ALIGNMENT_IS_SUPPORTED              601

#define _WHIPTK_VERSION_ECHO( s )               #s
#define _WHIPTK_VERSION_STRING_( a, b, c, d )   _WHIPTK_VERSION_ECHO(a) "." _WHIPTK_VERSION_ECHO(b) "." _WHIPTK_VERSION_ECHO(c) "." _WHIPTK_VERSION_ECHO(d) "\0"
#define _WHIPTK_VERSION_STRING                  _WHIPTK_VERSION_STRING_( WHIPDLL_MAJOR_VERSION, WHIPDLL_MINOR_VERSION, WHIPDLL_EXTRA_VERSION, WHIPDLL_BUILD_VERSION )

// Define resource strings
#define ECHO(a)                        #a

// The WHIP_VERSION_STRING can be used in an "About WHIP!..." box
#define STRINGIZE_WHIP_VERSION(major, minor, build) "Release " ECHO(major) "." ECHO(minor) "." ECHO(extra) "." ECHO(build) "\0"
#define WHIP_VERSION_STRING                         STRINGIZE_WHIP_VERSION(WHIPDLL_MAJOR_VERSION, WHIPDLL_MINOR_VERSION, WHIPDLL_EXTRA_VERSION, WHIPDLL_BUILD_VERSION)

// The WHIP_BUILD_STRING is used as the file version identifier on all
// the DLLs so whip.cab will download new versions as we update the DLLs.
// We put it in this form to allow it to be used in the Win32 versioning mechanism.
// This is also the product version of WHIP!. It is common to all unique components
// of WHIP!, i.e. not Heidi since Heidi has its own version.
//#define STRINGIZE_WHIP_BUILD(major, minor, extra, build) ECHO(major) "." ECHO(minor) "." ECHO(extra) "." ECHO(build) "\0"
//#define WHIP_BUILD_STRING                         STRINGIZE_WHIP_BUILD(WHIPDLL_MAJOR_VERSION, WHIPDLL_MINOR_VERSION, WHIPDLL_EXTRA_VERSION, WHIPDLL_BUILD_VERSION)
#define WHIP_BUILD_STRING   _WHIPTK_VERSION_STRING


#endif // WVERSION_HEADER
