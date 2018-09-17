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


#ifndef _DWFTK_XPSFixedDocumentResourceExtractor_H
#define _DWFTK_XPSFixedDocumentResourceExtractor_H

#include "dwfcore/XML.h"
#include "dwfcore/Vector.h"
#include "dwf/Toolkit.h"

namespace DWFCore
{
//
// fwd decl
//
class DWFXMLParser;
class DWFBufferOutputStream;
class DWFZipFileDescriptor;
}

namespace DWFToolkit
{

///
///\ingroup     dwfx
///
///\class       XPSFixedDocumentResourceExtractor   dwf/dwfx/reader/FixedPageResourceExtractor.h     "dwf/dwfx/reader/FixedPageResourceExtractor.h"
///\brief       This class implements extraction of DWF resources contained within a Fixed Page part
///\since       7.4.0
///
///

class XPSFixedDocumentResourceExtractor : public DWFCore::DWFXMLCallback
                             _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:
    _DWFTK_API
    XPSFixedDocumentResourceExtractor( DWFInputStream* pInputStream, bool bOwnStream )
        throw( DWFException );
    
    _DWFTK_API
    virtual ~XPSFixedDocumentResourceExtractor()
        throw();

    _DWFTK_API
    void parse()
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartElement()
    ///
    _DWFTK_API
    void notifyStartElement( const char*   zName,
                             const char**  ppAttributeList )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndElement()
    ///
    _DWFTK_API
    void notifyEndElement( const char* zName )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyStartNamespace()
    ///
    _DWFTK_API
    void notifyStartNamespace( const char* zPrefix,
                               const char* zURI )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyEndNamespace()
    ///
    _DWFTK_API
    void notifyEndNamespace( const char* zPrefix )
        throw();

    ///
    ///\copydoc DWFCore::DWFXMLCallback::notifyCharacterData()
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

	///
    ///\internal    Access the list of document URIs. This is used during when reading
    ///             in the DWFDocumentSequence.
    ///
    ///\return      An iterator that must be deleted using \b DWFCORE_FREE_OBJECT.
    ///             The iterator maybe NULL.
    ///\throw       None
    ///
    _DWFTK_API
	DWFOrderedVector<DWFString>::ConstIterator* documentURIs() const
        throw()
    {
        return _oDocumentURIs.constIterator();
    }


private:
    DWFInputStream*             _pFixedPagePartStream;
    bool                        _bOwnStream;
    DWFCore::DWFXMLParser*      _pParser;

	DWFOrderedVector<DWFString>		_oDocumentURIs;
};

}

#endif
