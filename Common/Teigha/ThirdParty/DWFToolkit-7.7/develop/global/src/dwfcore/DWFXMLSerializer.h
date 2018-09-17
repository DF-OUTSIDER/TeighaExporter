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


#ifndef _DWFTK_DWFXMLSERIALIZER_H
#define _DWFTK_DWFXMLSERIALIZER_H


///
///\file        dwfcore/DWFXMLSerializer.h
///\brief       This file contains the DWFXMLSerializer class declaration.
///



#include "dwfcore/STL.h"
#include "dwfcore/UUID.h"
#include "dwfcore/InputStream.h"
#include "dwfcore/BufferOutputStream.h"
#include "dwfcore/XMLEncodingOutputStream.h"
#include "dwfcore/UTF8EncodingOutputStream.h"
using namespace DWFCore;

//#include "dwf/Toolkit.h"
//#include "dwf/package/XML.h"

    //
    //
    //
#ifndef DWFCORE_XMLSERIALIZER_WRITE_BUFFER_BYTES
#define DWFCORE_XMLSERIALIZER_WRITE_BUFFER_BYTES      16384
#endif

    //
    //
    //
#ifndef DWFCORE_XMLSERIALIZER_ENCODING_BUFFER_BYTES
#define DWFCORE_XMLSERIALIZER_ENCODING_BUFFER_BYTES   4096
#endif


namespace DWFCore
{

///
///\class       DWFXMLSerializer   dwfcore/DWFXMLSerializer.h
///\brief       An object that facilitates the publishing of toolkit data objects into XML.
///\since       7.4
///
class DWFXMLSerializer : public DWFCoreMemory
{

public:

    ///
    ///\brief   Named enumerations for use during the serialization.
    ///
    typedef enum
    {
        eElementOpen = 0x00010000

    } teFlags;

public:

    ///
    ///         Constructor
    ///
    ///\param   rUUID       A unique identifier generator that is available
    ///                     to consumers of the serializer.  
    ///\throw   None
    ///
    _DWFCORE_API
    DWFXMLSerializer( DWFUUID& rUUID )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFCORE_API
    virtual ~DWFXMLSerializer()
        throw();

    ///
    ///         Binds a new output stream to the serializer.
    ///
    ///         All subsequent output will be written to \a rStream.
    ///
    ///\param   rStream     The XML destination stream.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void attach( DWFOutputStream& rStream )
        throw( DWFException );

    ///
    ///         Unbinds an output stream from the serializer.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void detach()
        throw( DWFException );

    ///
    ///         Helper utility that provides a unique but sequential UUID
    ///         that can be used by client code for object IDs, etc.
    ///
    ///         Sequential IDs are preferred as the compress better resulting
    ///         in smaller DWF package files.
    ///
    ///\param       bSquash     If \e true, the 128-bit uuid will be Base64 encoded
    ///                         into a short form string representation, otherwise the 
    ///                         standard form string representation will be returned.
    ///                         The encoding used for the short form string will be
    ///                         non RFC 1521 compliant as detailed in \a DWFString::EncodeBase64().
    ///\throw   None
    ///
    _DWFCORE_API
    const DWFString& nextUUID( bool bSquash )
        throw();

    ///
    ///         Emits the xml header
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void emitXMLHeader()
        throw( DWFException );
    ///
    ///         Opens a new element tag in the XML data stream.
    ///
    ///\param   zName           The name of the element.
    ///\param   zNamespace      An optional namespace to add to the element.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void startElement( const DWFString& zName, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Inserts a new string attribute into an open element.
    ///
    ///\param   zName           The name of the attribute.
    ///\param   zValue          The value of the attribute.
    ///\param   zNamespace      An optional namespace to add to the attribute.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void addAttribute( const DWFString& zName, const DWFString& zValue, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Inserts a new integer attribute into an open element.
    ///
    ///\param   zName           The name of the attribute.
    ///\param   nValue          The value of the attribute.
    ///\param   zNamespace      An optional namespace to add to the attribute.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    void addAttribute( const DWFString& zName, int nValue, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Inserts new integer attributes into an open element.
    ///
    ///\param   zName           The name of the attribute.
    ///\param   anValues        A pointer to an array of integer attribute values.
    ///\param   nCount          The number of attribute values.
    ///\param   zNamespace      An optional namespace to add to the attribute.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    void addAttribute( const DWFString& zName, const int* anValues, size_t nCount, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Inserts a new float attribute into an open element.
    ///
    ///\param   zName           The name of the attribute.
    ///\param   fValue          The value of the attribute.
    ///\param   zNamespace      An optional namespace to add to the attribute.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    void addAttribute( const DWFString& zName, const float& fValue, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Inserts new float attributes into an open element.
    ///
    ///\param   zName           The name of the attribute.
    ///\param   afValues        A pointer to an array of float attribute values.
    ///\param   nCount          The number of attribute values.
    ///\param   zNamespace      An optional namespace to add to the attribute.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    void addAttribute( const DWFString& zName, const float* anValues, size_t nCount, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Inserts a new double attribute into an open element.
    ///
    ///\param   zName           The name of the attribute.
    ///\param   dValue          The value of the attribute.
    ///\param   zNamespace      An optional namespace to add to the attribute.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    void addAttribute( const DWFString& zName, const double& dValue, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Inserts new double attributes into an open element.
    ///
    ///\param   zName           The name of the attribute.
    ///\param   anValues        A pointer to an array of double attribute values.
    ///\param   nCount          The number of attribute values.
    ///\param   zNamespace      An optional namespace to add to the attribute.
    ///\throw   DWFException
    ///\since   7.2
    ///
    _DWFCORE_API
    void addAttribute( const DWFString& zName, const double* anValues, size_t nCount, const DWFString& zNamespace = /*NOXLATE*/L"" )
        throw( DWFException );

    ///
    ///         Insert a block of CDATA to the XML data stream.
    ///
    ///\param   zCData          The CDATA text to add.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void addCData( const DWFString& zCData )
        throw( DWFException );

    ///
    ///         Inserts the contents of the given stream directly into the current XML output. It is
    ///         assumed that the contents of the stream are valid XML and are encoded in the correct
    ///         unicode format (i.e. UTF-8).
    ///
    ///\param   pInputStream    An input stream holding the data to be inserted.
    ///\param   bCloseCurrentElement Forces the current element (header) to be closed before serializing the given stream.
    ///\throw   DWFException
    ///\since   7.4.0
    ///
    ///\warning if bCloseCurrentElement is false, then it is required that the inserted data contain *only* attribute data,
    ///         with the attribute name and appropriate ="" wrappers around the content, and must not contain any xml elements,
    ///         cdata, etc.  Breaking this rule will most certainly produce mal-formed XML.
    ///
    _DWFCORE_API
    void insertXMLStream( DWFInputStream* pInputStream, bool bCloseCurrentElement=true )
        throw( DWFException );

    ///
    ///         Closes an open element in the XML data stream.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    void endElement()
        throw( DWFException );

protected:

    class XMLOutputStream 
        : public DWFBufferOutputStream
    {
    public:

        XMLOutputStream( DWFOutputStream& rOutputStream )
            throw();

        virtual ~XMLOutputStream()
            throw();

        //
        // use this method for publishing the xml data
        // it will be encoded correctly
        //
        size_t writeXML( const void*   pBuffer,
                         size_t        nBytesToWrite )
            throw( DWFException );

        //
        // stream interface - called back into for
        // writing to buffer
        //
        size_t write( const void*   pBuffer,
                      size_t        nBytesToWrite )
            throw( DWFException );

        //
        // toggles XML encoding - certain characters "<", ">", etc
        // shouldn't be XML encoded as they are written to the stream
        //
        void encode( bool bXML = true )
        {
            _bXML = bXML;
        }

    private:
        bool                        _bXML;
        DWFXMLEncodingOutputStream  _oXMLStream;
        DWFUTF8EncodingOutputStream _oUTF8Stream;
    };

protected:

    XMLOutputStream*        _pStream;

private:

    DWFUUID&                _rUUID;

    bool                            _bLastElementOpen;
    const DWFString*                _pLastElement;

    // TODO:  Do something like this in core
    //_DWFTK_STD_VECTOR(DWFString)    _oDepthChart;
    std::vector<DWFString>          _oDepthChart;

    wchar_t*                        _zBuffer;
    size_t                          _nBufferSize;

private:

    //
    // Unimplemented methods
    //

    DWFXMLSerializer( const DWFXMLSerializer& );
    DWFXMLSerializer& operator=( const DWFXMLSerializer& );
};

}

#endif

