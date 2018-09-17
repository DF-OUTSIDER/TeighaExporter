//
//  Copyright (c) 2007 by Autodesk, Inc.
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


#ifndef _DWFTK_DWFXFixedPageResourceExtractor_H
#define _DWFTK_DWFXFixedPageResourceExtractor_H

#include "dwfcore/XML.h"
#include "dwf/Toolkit.h"

namespace DWFCore
{
//
// fwd decl
//
class DWFXMLParser;
class DWFBufferOutputStream;
}

namespace DWFToolkit
{

///
///\ingroup     dwfx
///
///\class       DWFXFixedPageResourceExtractor   dwf/dwfx/reader/FixedPageResourceExtractor.h     "dwf/dwfx/reader/FixedPageResourceExtractor.h"
///\brief       This class implements extraction of DWF resources contained within a Fixed Page part
///\since       7.4.0
///
///

class DWFXFixedPageResourceExtractor : public DWFCore::DWFXMLCallback
                             _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:
    _DWFTK_API
    DWFXFixedPageResourceExtractor( DWFInputStream* pFixedPagePartStream, bool bOwnStream )
        throw( DWFException );
    
    _DWFTK_API
    virtual ~DWFXFixedPageResourceExtractor()
        throw();

    _DWFTK_API
    void parse()
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement(const char*   zName, const char**  ppAttributeList)
    ///
    _DWFTK_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement(const char* zName)
    ///
    _DWFTK_API
    void notifyEndElement( const char* zName )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartNamespace(const char* zPrefix, const char* zURI)
    ///
    _DWFTK_API
    void notifyStartNamespace( const char* zPrefix,
                               const char* zURI )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace(const char* zPrefix)
    ///
    _DWFTK_API
    void notifyEndNamespace( const char* zPrefix )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData(const char* zCData, int nLength)
    ///
    _DWFTK_API
    void notifyCharacterData( const char* zCData, 
                              int nLength ) 
        throw();

    ///
    ///         Returns the input stream for the resource specified by its locator
    ///
    ///\param   zLocator    The locator for the resource
    ///
    ///\return  A stream for the requested resource. The caller must delete this pointer
    ///         with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFInputStream* getResourceStream( const DWFString& zLocator )
        throw( DWFException );

private:
    DWFInputStream*             _pFixedPagePartStream;
    bool                        _bOwnStream;
    DWFCore::DWFXMLParser*      _pParser;
    DWFXMLSerializer*           _pResourceSerializer;
    DWFBufferOutputStream*      _pCurrentResourceOutputStream;
    int                         _nCanvasElementDepth;
    int                         _nResourceCanvasElementDepth;
    DWFUUID                     _oUUID;
    DWFString                   _zLocator;

    typedef _DWFTK_STD_MAP(DWFString, DWFBufferOutputStream*)  _tResourceLocatorToStream;
    _tResourceLocatorToStream                                  _oResourceStreamMap;  
};

}

#endif
