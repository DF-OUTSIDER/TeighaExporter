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

#ifndef _DWFTK_EMBEDDED_FONT_H
#define _DWFTK_EMBEDDED_FONT_H


///
///\file        dwf/publisher/EmbeddedFont.h
///\brief       This file contains the DWFEmbeddedFont class and DWFEmbeddedFontImpl interface declarations.
///


#include "dwfcore/STL.h"
#include "dwfcore/Core.h"
#include "dwfcore/String.h"
#include "dwfcore/InputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/FontResource.h"



    //
    // initial amount of memory we want to commit
    // towards the EFF stream buffer
    //
#ifndef DWFTK_EMBEDDED_FONT_HEAP_INIT
#define DWFTK_EMBEDDED_FONT_HEAP_INIT       4192
#endif

    //
    // maximum amount of memory we want to commit
    // towards this particular EFF (embedded font file)
    // if the actual EFF exceeds this limit, 
    // a temporary disk file will be used instead
    //
#ifndef DWFTK_EMBEDDED_FONT_HEAP_LIMIT
#define DWFTK_EMBEDDED_FONT_HEAP_LIMIT      65536
#endif



namespace DWFToolkit
{

///
///\ingroup         dwfpublish
///
///\interface       DWFEmbeddedFontImpl     dwf/publisher/EmbeddedFont.h    "dwf/publisher/EmbeddedFont.h"
///\brief           Interface for TrueType font embedding engines.
///\since           7.0.1
///
///                 As of the 7.0.1 release, font embedding is only available on Microsoft Windows
///                 though the use of the T2Embed API (redistributable DLL required.)
///                 This non-final implementation pattern keeps the publishing API independent
///                 of platform-specific font embedding technology.
///
class DWFEmbeddedFontImpl _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFEmbeddedFontImpl()
        throw()
    {;}

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFEmbeddedFont::addCharacters
    ///
    _DWFTK_API
    virtual void addCharacters( const DWFString& rCharacters )
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::embed
    ///
    _DWFTK_API
    virtual void embed()
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::getInputStream
    ///
    _DWFTK_API
    virtual DWFInputStream* getInputStream()
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::getFaceName
    ///
    _DWFTK_API
    virtual const DWFString& getFaceName()
    throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::getLogfontName
    ///
    _DWFTK_API
    virtual const DWFString& getLogfontName()
    throw( DWFException ) = 0;


#endif

    ///
    ///\copydoc DWFEmbeddedFont::getRequest
    ///
    _DWFTK_API
    virtual int getRequest()
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::getPrivilege
    ///
    _DWFTK_API
    virtual DWFFontResource::tePrivilege getPrivilege()
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::getCharacterCode
    ///
    _DWFTK_API
    virtual DWFFontResource::teCharacterCode getCharacterCode()
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::load
    ///
    _DWFTK_API
    virtual void load( bool bForceRename = true )
        throw ( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::loadedFontName
    ///
    _DWFTK_API
    virtual const DWFString& loadedFontName() const
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFEmbeddedFont::unload
    ///
    _DWFTK_API
    virtual void unload()
        throw ( DWFException ) = 0;

protected:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFEmbeddedFontImpl()
        throw()
    {;}

private:

    DWFEmbeddedFontImpl( const DWFEmbeddedFontImpl& );
    DWFEmbeddedFontImpl& operator=( const DWFEmbeddedFontImpl& );
};



///
///\ingroup         dwfpublish
///
///\interface       DWFEmbeddedFont     dwf/publisher/EmbeddedFont.h    "dwf/publisher/EmbeddedFont.h"
///\brief           A general representation of a TrueType font that can be published.
///\since           7.0.1
///
///                 This object should remain platform-independent and delegate any specialized
///                 embedding technology to the implementation engines.
///
///                 This class can be used both to generate an embedded font representation
///                 as well as realize that representation in the font engine [provided by the 
///                 implementation.]  
///
class DWFEmbeddedFont       _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///\brief   This type defines a list of DWFEmbeddedFont pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFEmbeddedFont*) tList;

public:

    ///
    ///             Constructor
    ///
    ///\param       pImpl       A pointer to a font embedding engine.
    ///                         This pointer will be deleted with \b DWFCORE_FREE_OBJECT.
    ///\throw       None
    ///
    _DWFTK_API
    DWFEmbeddedFont( DWFEmbeddedFontImpl* pImpl )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFTK_API
    ~DWFEmbeddedFont()
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///             Identifies which character glyphs to embed.
    ///
    ///             This is an additive operation.  Generally,
    ///             an embedded font object is created for each
    ///             TrueType font that should be published.
    ///             For every string (or character) that is added to the 
    ///             scene that must be rendered in an embedded font,
    ///             that string (or character) is sent to this method.
    ///             If possible, only those data for the final character subset
    ///             should be embedded.
    ///
    ///             The following are equivalent:
    ///             -#
    ///             \code
    ///             pFont->addCharacters( L"This is some text" );
    ///             pFont->addCharacters( L"Doors and Windows" );
    ///             \endcode
    ///             -#
    ///             \code
    ///             pFont->addCharacters( L"adehimnorstwxDTW" );
    ///             \endcode
    ///
    ///\param       rCharacters     The Unicode character string.
    ///\throw       DWFException
    ///
    _DWFTK_API
    void addCharacters( const DWFString& rCharacters )
        throw( DWFException );

    ///
    ///             Captures the embeddable font data.
    ///
    ///             This method should be invoked once after \a addCharacters()
    ///             to use the engine implementation to perform its work and 
    ///             extract, compress, etc. for the font data.  The next call to
    ///             \a getInputStream() will provide the data generated from this operation.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void embed()
        throw( DWFException );

    ///
    ///             Provides a stream to read the embeddable font data obtained
    ///             from a previous call to \a embed().
    ///
    ///\return      A pointer to a data stream.  
    ///             The caller is responsible for deleting the pointer with \b DWFCORE_FREE_OBJECT.
    ///\throw       DWFException
    ///
    _DWFTK_API
    DWFInputStream* getInputStream()
        throw( DWFException );

    ///
    ///             Returns the canonical face name of the font.
    ///
    ///\return      The font name.
    ///\throw       DWFException
    ///
    _DWFTK_API
    const DWFString& getFaceName()
        throw( DWFException );

    ///
    ///             Returns the logfont face name of the font.
    ///
    ///\return      The font name.
    ///\throw       DWFException
    ///
    _DWFTK_API
    const DWFString& getLogfontName()
        throw( DWFException );


#endif

    ///
    ///             Returns the request flag with which the font was embedded.
    ///
    ///\return      The request flag.
    ///\throw       DWFException
    ///
    _DWFTK_API
    int getRequest()
        throw( DWFException );

    ///
    ///             Returns the privilege with which the font was embedded
    ///
    ///\return      The privilege enumeration.
    ///\throw       DWFException
    ///
    _DWFTK_API
    DWFFontResource::tePrivilege getPrivilege()
        throw( DWFException );

    ///
    ///             Returns the character code for the embedded font.
    ///
    ///\return      The character code enumeration.
    ///\throw       DWFException
    ///
    _DWFTK_API
    DWFFontResource::teCharacterCode getCharacterCode()
        throw( DWFException );

    ///
    ///             Loads the given font from an embedded data representation
    ///             into the font engine.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void load( bool bForceRename = true )
        throw ( DWFException );

    ///
    ///             Returns the name under which the font was loaded.
    ///
    ///\return      The (temporary) font name.
    ///\throw       DWFException
    ///
    _DWFTK_API
    const DWFString& loadedFontName() const
        throw( DWFException );

    ///
    ///             Unloads the embedded font from the font engine.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void unload()
        throw ( DWFException );

private:

    DWFEmbeddedFontImpl* _pImpl;

private:

    DWFEmbeddedFont();
    DWFEmbeddedFont( const DWFEmbeddedFont& );
    DWFEmbeddedFont& operator=( const DWFEmbeddedFont& );
};


}


#endif

