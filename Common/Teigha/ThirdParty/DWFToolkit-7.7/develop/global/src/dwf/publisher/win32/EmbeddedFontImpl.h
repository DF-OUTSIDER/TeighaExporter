///
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

#ifndef _DWFTK_EMBEDDED_FONT_IMPL_WIN32_H
#define _DWFTK_EMBEDDED_FONT_IMPL_WIN32_H


///
///\file        dwf/publisher/win32/EmbeddedFontImpl.h
///\brief       This file contains the DWFEmbeddedFontImpl_Win32 class declaration.
///

#include "dwf/publisher/EmbeddedFont.h"


#ifdef  _DWFCORE_WIN32_SYSTEM

#include "dwf/publisher/win32/t2embapi.h"


#include "dwfcore/TempFile.h"
#include "dwfcore/BufferInputStream.h"
#include "dwfcore/BufferOutputStream.h"
using namespace DWFCore;


#include "dwf/package/FontResource.h"


namespace DWFToolkit
{

///
///\ingroup         dwfpublish
///
///\class           DWFEmbeddedFontImpl_Win32     dwf/publisher/win32/EmbeddedFont.h    "dwf/publisher/win32/EmbeddedFont.h"
///\brief           TrueType font embedding implementation for Microsoft Windows GDI Font Engine.
///\since           7.0.1
///
///                 This implementation uses the T2Embed API.
///
class DWFEmbeddedFontImpl_Win32 : public DWFEmbeddedFontImpl
                                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

#ifndef DWFTK_READ_ONLY

    ///
    ///             Constructor
    ///
    ///             Use this contructor when this implementation engine
    ///             will be used for generating embedded font data from a GDI font resource.
    ///
    ///\param       hFont           A GDI font handle.
    ///\param       nHeapLimit      In order to facilitate the fastest possible process,
    ///                             this implementation will always serialize the EFF from
    ///                             the MS API into a heap memory buffer.  When getInputStream()
    ///                             is called, this buffer will be flushed to a temporary disk
    ///                             file if it's size exceeds this limit value.  The rationale
    ///                             behind this mechanism is that if many fonts are in use it's
    ///                             possible that they could consume quite a bit of RAM.
    ///                             Further, and maybe more importantly, while in some (most?) cases
    ///                             getting the input stream is proceeded immediately by publishing it
    ///                             to a DWF file, it is not always true.  If this stream is instead 
    ///                             going to passed around - especially to a remote client - then you
    ///                             definitely don't want your memory tied up.  If you never want to
    ///                             go to disk, then simply make this value very large, like 512K or 1MB.
    ///                             \b DWFTK_EMBEDDED_FONT_HEAP_LIMIT is preferred to be set to hold
    ///                             a "usual" embedded font in memory - say 10k to 20k - so that no
    ///                             disk i/o becomes necessary at publish time.
    ///\throw       None
    ///
    _DWFTK_API
    DWFEmbeddedFontImpl_Win32( HFONT    hFont,
                               size_t   nHeapLimit = DWFTK_EMBEDDED_FONT_HEAP_LIMIT )
        throw();

#endif

    ///
    ///             Constructor
    ///
    ///             Use this contructor when this implementation engine
    ///             will be used for loading an embedded font resource into the GDI font engine.
    ///
    ///\param       rFontResource   A toolkit package object from which the embedded font data can be read.
    ///\throw       None
    ///
    _DWFTK_API
    DWFEmbeddedFontImpl_Win32( DWFFontResource& rFontResource )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFTK_API
    virtual ~DWFEmbeddedFontImpl_Win32()
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFEmbeddedFontImpl::embed
    ///
    _DWFTK_API
    void embed()
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::addCharacters
    ///
    _DWFTK_API
    void addCharacters( const DWFString& rCharacters )
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::getInputStream
    ///
    _DWFTK_API
    DWFInputStream* getInputStream()
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::getFaceName
    ///
    _DWFTK_API
    const DWFString& getFaceName()
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::getLogfontName
    ///
    _DWFTK_API
    const DWFString& getLogfontName()
        throw( DWFException );


#endif

    ///
    ///\copydoc DWFEmbeddedFontImpl::getRequest
    ///
    _DWFTK_API
    int getRequest()
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::getPrivilege
    ///
    _DWFTK_API
    DWFFontResource::tePrivilege getPrivilege()
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::getCharacterCode
    ///
    _DWFTK_API
    DWFFontResource::teCharacterCode getCharacterCode()
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::load
    ///
    _DWFTK_API
    void load( bool bForceRename = true )
        throw ( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontImpl::loadedFontName
    ///
    _DWFTK_API
    const DWFString& loadedFontName() const
        throw();

    ///
    ///\copydoc DWFEmbeddedFontImpl::unload
    ///
    _DWFTK_API
    void unload()
        throw ( DWFException );

private:

#ifndef DWFTK_READ_ONLY

    //
    // streams the EFF to either the heap buffer or temp file
    //
    void _serializeFont( HDC hDC )
        throw( DWFException );

    //
    // TTEmbedFont callback
    //
    static unsigned long _WriteFontBuffer( void*               pStream,
                                           const void*         pBuffer,
                                           const unsigned long nBytesToWrite )
        throw( DWFException );

#endif

    //
    // TTEmbedFont callback
    //
    static unsigned long _ReadFontBuffer( void*               pStream,
                                          void*               pBuffer,
                                          const unsigned long nBytesToRead )
        throw( DWFException );

    //
    // Loads the font embedding library 
    //
    static void _loadFontLibrary( void );

private:

#ifndef DWFTK_READ_ONLY
    size_t                  _nHeapLimit;

    HFONT                   _hFont;
    DWFString               _zFaceName;
    DWFString               _zLogfontName;
    DWFTempFile*            _pFontFile;

    bool                    _abCodePoints[65536];
    unsigned short          _nCodePoints;
    unsigned short*         _pSparseCharacters;
#endif

    unsigned long           _nPriv;
    unsigned long           _nRequest;
    unsigned long           _nCharacterCode;

    DWFBufferOutputStream*  _pFontOutputStream;
    DWFFontResource*        _pFontResource;
    DWFInputStream*         _pFontInputStream;
    DWFBufferInputStream*   _pReadAheadStream;
    HANDLE                  _hLoadedFont;
    DWFString               _zLoadedFontName;

    static HINSTANCE                _khLib;
    static int                      _knLibRef;
    static const char*              _kz_T2Embed;
    static fpTTEmbedFont            _kfpTTEmbedFont;
    static fpTTLoadEmbeddedFont     _kfpTTLoadEmbeddedFont;
    static fpTTDeleteEmbeddedFont   _kfpTTDeleteEmbeddedFont;

private:

    DWFEmbeddedFontImpl_Win32( const DWFEmbeddedFontImpl_Win32& );
    DWFEmbeddedFontImpl_Win32& operator=( const DWFEmbeddedFontImpl_Win32& );
};


    //
    // required for win32 dll external linkage
    //
#ifndef DWFTK_STATIC

_declspec(selectany) const char* DWFEmbeddedFontImpl_Win32::_kz_T2Embed = /*NOXLATE*/"t2embed.dll";

#endif

};


#else
#error  This is a Win32 header file and is incompatible with your current system configuration
#endif  

#endif

