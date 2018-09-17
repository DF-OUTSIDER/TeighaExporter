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


#ifndef _DWFTK_FONT_RESOURCE_H
#define _DWFTK_FONT_RESOURCE_H

///
///\file        dwf/package/FontResource.h
///\brief       This file contains the DWFFontResource class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/Resource.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFFontResource   dwf/package/FontResource.h     "dwf/package/FontResource.h"
///\brief       This class represents an embedded font.
///\since       7.0.1
///
///             Embedded fonts are encrypted data derived from TrueType font tables.
///             They are published, when licensing permits, to deliver the original
///             font to a viewer's system for optimal visualization.
///
class DWFFontResource : public DWFResource
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   Privileges controlling the font usage.
    ///
    typedef enum
    {
        ePreviewPrint = 1,
        eEditable     = 2,
        eInstallable  = 3,
        eNoEmbedding  = 4

    } tePrivilege;

    ///
    ///\brief   Describes the font data.
    ///
    typedef enum
    {
        eUnicode    = 1,
        eSymbol     = 2,
        eGlyphIndex = 3
    } teCharacterCode;

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read resource content
    ///         data from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to resource content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFFontResource( DWFPackageReader*  pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read resource content
    ///         data from the DWF package.
    ///
    ///\param   nRequest        ???
    ///\param   ePrivilege      An enumeration describing the font license.
    ///\param   eCharacterCode  An enumeration descibring the font data.
    ///\param   zCanonicalName  The descriptive name of the font.
    ///\param   zLogfontName    Legacy name of the font.
    ///\throw   None
    ///
    _DWFTK_API
    DWFFontResource( int                nRequest,
                     tePrivilege        ePrivilege,
                     teCharacterCode    eCharacterCode,
                     const DWFString&   zCanonicalName,
                     const DWFString&   zLogfontName)
throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFFontResource()
        throw();

    ///
    ///         Returns the font request.
    ///
    ///\return  An integer code indicating the request.
    ///\throw   None
    ///
    int request() const
        throw()
    {
        return _nRequest;
    }

    ///
    ///         Returns the licensing privilege.
    ///
    ///\return  An enumeration indicating the privilege.
    ///\throw   None
    ///
    tePrivilege privilege() const
        throw()
    {
        return _ePrivilege;
    }

    ///
    ///         Returns the font character code.
    ///
    ///\return  An enumeration indicating the type of data in the font.
    ///\throw   None
    ///
    teCharacterCode characterCode() const
        throw()
    {
        return _eCharacterCode;
    }

    ///
    ///         Returns the legacy font name.
    ///
    ///\return  The legacy font name.
    ///\throw   None
    ///
    const DWFString& logfontName() const
        throw()
    {
        return _zLogfontName;
    }

    ///
    ///         Returns the descriptive font name.
    ///
    ///\return  The descriptive font name.
    ///\throw   None
    ///
    const DWFString& canonicalName() const
        throw()
    {
        return _zCanonicalName;
    }

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList()
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif


private:
        
    int             _nRequest;
    tePrivilege     _ePrivilege;
    teCharacterCode _eCharacterCode;
    DWFString       _zLogfontName;
    DWFString       _zCanonicalName;

private:

    DWFFontResource( const DWFFontResource& );
    DWFFontResource& operator=( const DWFFontResource& );
};


}

#endif
