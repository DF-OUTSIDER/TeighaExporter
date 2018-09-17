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

#if !defined XAMLCORE_HEADER
#define XAMLCORE_HEADER

///
///\file        dwf/XAML/XamlCore.h
///

//////////////////////////////////////////////////////////////////////////
//
// Recognize the newer DWF Toolkit pre-processor macros
// 
//
#ifdef  DWFTK_STATIC
#ifndef XAML_STATIC_LIB
#define XAML_STATIC_LIB
#endif
#endif

#ifdef  DWFTK_EXPORT_API
#undef  XAMLTOOL_CLIENT
#undef  XAML_STATIC_LIB
#endif

#if defined   _MSC_VER || defined __BORLANDC__ || (defined(_WIN32) && defined(__GNUC__))
    #ifndef XAML_STATIC_LIB
        #if defined XAMLTOOL_CLIENT
            #ifdef XAMLTOOL_CLIENT_NODECLSPEC
                  #define         XAMLTK_API
            #pragma message ("Xaml client (no declspec) ")
            #else
                  #define         XAMLTK_API  __declspec(dllimport)
            #pragma message ("Xaml client (dllimport) ")
            #endif
        #else
            #define         XAMLTK_API  __declspec(dllexport)
            #pragma message ("Xaml server (dllexport) ")
        #endif
    #else
        #define XAMLTK_API
        #pragma message ("XAML static library ")
    #endif
#endif

#if !defined XAMLTK_API
    #define     XAMLTK_API
#endif

#endif

