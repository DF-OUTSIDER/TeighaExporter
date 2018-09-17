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

#ifndef TD_VERSION_DEFINED
#define TD_VERSION_DEFINED

#define TD_COMPANY_NAME_S   "Open Design Alliance"
#define TD_COMPANY_NAME_W  L"Open Design Alliance"

// The string below need to be defined in quotes else compiler complains in illegal characters
#define TD_COPYRIGHT_START_S            "Copyright\xA9 "
#define TD_COPYRIGHT_START_YEAR_S       "2002"
#define TD_COPYRIGHT_END_S              "-2018, Open Design Alliance"
#define TD_COPYRIGHT_START_W           L"Copyright \x00A9 "
#define TD_COPYRIGHT_START_YEAR_W      L"2002"
#define TD_COPYRIGHT_END_W             L"-2018, Open Design Alliance"
#define TDNET_C_COPYRIGHT_START_YEAR_W TD_COPYRIGHT_START_YEAR_W
#define TDNET_COPYRIGHT_START_YEAR_W   TD_COPYRIGHT_START_YEAR_W

#define TD_COPYRIGHT_S       TD_COPYRIGHT_START_S TD_COPYRIGHT_START_YEAR_S TD_COPYRIGHT_END_S
#define TD_COPYRIGHT_W       TD_COPYRIGHT_START_W TD_COPYRIGHT_START_YEAR_W TD_COPYRIGHT_END_W

#define TD_PRODUCT_NAME_START_S          "ODA"
#define TD_PRODUCT_NAME_ADDITION_S       ""
#define TDX_PRODUCT_NAME_ADDITION_S      "X"
#define TD_PRODUCT_NAME_END_S            "SDK"
#define TD_PRODUCT_NAME_START_W         L"ODA"
#define TD_PRODUCT_NAME_ADDITION_W      L""
#define TDX_PRODUCT_NAME_ADDITION_W     L"X"
#define TDNET_PRODUCT_NAME_ADDITION_W   L".NET"
#define TDNET_C_PRODUCT_NAME_ADDITION_W L".NET Classic"
#define TD_PRODUCT_NAME_END_W           L"SDK"

#define TD_PRODUCT_NAME_S TD_PRODUCT_NAME_START_S " " TD_PRODUCT_NAME_END_S
#define TD_PRODUCT_NAME_W TD_PRODUCT_NAME_START_W L" " TD_PRODUCT_NAME_END_W

#define DEFAULT_MODULE_EXTENSION_S             "tx"
#define DEFAULT_MODULE_EXTENSION_DOT_S        ".tx"
#define DEFAULT_MODULE_EXTENSION_W            L"tx"
#define DEFAULT_MODULE_EXTENSION_DOT_W       L".tx"
#define VECTORIZATION_MODULE_EXTENSION_S       "txv"
#define VECTORIZATION_MODULE_EXTENSION_DOT_S  ".txv"
#define VECTORIZATION_MODULE_EXTENSION_W      L"txv"
#define VECTORIZATION_MODULE_EXTENSION_DOT_W L".txv"


// Full Product version is:
//   TD_MAJOR_VERSION.TD_MINOR_VERSION.TD_MAJOR_BUILD.TD_MINOR_BUILD
// Example: 1.11.00.00 (initial 1.11 release).
// OpenDesign maintenance updates to 1.11 will increase the major build number,
// for example, 1.11.01.00, 1.11.02.00, etc.  Minor build number is reserved for
// client builds.

/* When Incrementing these be sure to make the appropriate changes
 * to the Version string and build comments.
 */
#define TD_MAJOR_VERSION         19
#define TD_MINOR_VERSION          8
#define TD_MAJOR_BUILD            0
#define TD_MINOR_BUILD            0

// Numeric representation of the TD version to be used in preprocessor definitions
// like #if (TD_VERSION > 30500) ...
// (we never use minor build number, and it is not included in version.
// If you are building your custom version of Teigha, you may define an alternative TD_VERSION_
// or TD_VERSION_FULL, to distinguish between minor builds if necessary)
#define TD_VERSION  (TD_MAJOR_BUILD + TD_MINOR_VERSION*100 + TD_MAJOR_VERSION*10000)

#define TD_SHORT_STRING_VER_S   "19.8"
#define TD_SHORT_STRING_VER_W  L"19.8"

#define TD_PRODUCT_VER_STR_S   "19.8.0.0"
#define TD_PRODUCT_VER_STR_W  L"19.8.0.0"

//Changed in VersionInfo to TD_PRODUCT_NAME_S " " TD_PRODUCT_VER_STR_S
//That need for NET and ActiveX
#define TD_BUILD_COMMENTS_S     TD_PRODUCT_NAME_S " " TD_PRODUCT_VER_STR_S
#define TD_BUILD_COMMENTS_W     TD_PRODUCT_NAME_W L" " TD_PRODUCT_VER_STR_W


#define TD_TYPELIB_VER(MAJ,MIN) version(##MAJ##.##MIN##)

#define TEIGHA_BUILD_NUMBER 0
#define TEIGHA_MAIN_SVN_REVISION 0
#define TEIGHA_CORE_SVN_REVISION 0
#define TEIGHA_DGN_SVN_REVISION 0
#define TEIGHA_ARCHITECTURE_SVN_REVISION 0
#define TEIGHA_CIVIL_SVN_REVISION 0
#define TEIGHA_NET_SVN_REVISION 0
#define TEIGHA_JAVA_SVN_REVISION 0
#define TEIGHA_MECHANICAL_SVN_REVISION 0
#define TEIGHA_PRC_SVN_REVISION 0
#define TEIGHA_BIM_SVN_REVISION 0
#define TEIGHA_CLOUD_SVN_REVISION 0

#if defined(_TOOLKIT_IN_DLL_) && defined(_MSC_VER) // second condition for RC
#define TD_STRINGIZE2( s )      L##s
#define TD_STRINGIZE( s )       TD_STRINGIZE2( s )

#if _MSC_VER == 1200
// src is default part of suffix
// fme is registered for Safe Software Inc. www.safe.com

#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_6"
#elif _MSC_VER == 1310
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_7"
#elif _MSC_VER == 1400
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_8"
#elif _MSC_VER == 1500
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_9"
#elif _MSC_VER == 1600
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_10"
#elif _MSC_VER == 1700
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_11"
#elif _MSC_VER == 1800
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_12"
#elif _MSC_VER == 1900
#define TD_DLL_VERSION_SUFFIX_STR L"_" TD_STRINGIZE(TD_SHORT_STRING_VER_S) L"_14"
#elif _MSC_VER >= 1910
#define TD_DLL_VERSION_SUFFIX_STR L"_" TD_STRINGIZE(TD_SHORT_STRING_VER_S) L"_15"
#else
#error "Unsupported compiler for suffixed version"
#endif
#endif


#if defined(_TOOLKIT_IN_DLL_) && defined(__BORLANDC__)
#define TD_STRINGIZE2( s )      L""##s
#define TD_STRINGIZE( s )       TD_STRINGIZE2( s )

#if __BORLANDC__ >= 0x0580 && __BORLANDC__ <= 0x0589
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_b6"
#elif __BORLANDC__ >= 0x0610 && __BORLANDC__ <= 0x0619
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_b9"
#elif __BORLANDC__ >= 0x0620 && __BORLANDC__ <= 0x0629
#define TD_DLL_VERSION_SUFFIX_STR L"_"##TD_STRINGIZE(TD_SHORT_STRING_VER_S)##L"_b10"
#else
#error "Unsupported compiler for suffixed version"
#endif
#endif

#if  defined(_TOOLKIT_IN_DLL_) && defined(_WIN32) && defined(__GNUC__)
#define TD_DLL_VERSION_SUFFIX_STR L""
#endif

#endif  // ODA_VERSION_DEFINED

