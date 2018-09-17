//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
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
#ifndef _DWFCORE_XML_H
#define _DWFCORE_XML_H

#include "dwfcore/BufferInputStream.h"
#include "dwfcore/String.h"
#include "dwfcore/STL.h"
#include "dwfcore/DWFXMLSerializer.h"

namespace DWFCore
{

///
///
///\interface   DWFXMLCallback    dwfcore/XML.h
///\brief       This class integrates with the Expat XML parser.
///\since       7.4
///
///             The Expat XML parser uses a static callback function API,
///             which doesn't lend itself well to multi-threaded applications
///             directly.  Classes interested in participating in XML parsing
///             must implement this interface and set themselves as the \a UserData
///             pointer on the parser.
///
class DWFXMLCallback
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFXMLCallback()
        throw()
    {
        if (_pFilter && _bOwnFilter)
        {
            DWFCORE_FREE_OBJECT( _pFilter );
        }
    }

    ///
    ///         This method is notified by the static \a XML_StartElementHandler
    ///         Expat parser callback function.
    ///
    ///         Attribute list processing example:
    ///         \code
    ///          size_t iAttrib = 0;
    ///
    ///          for(; ppAttributeList[iAttrib]; iAttrib += 2)
    ///          {
    ///             //
    ///             // dump attribute 
    ///             //
    ///             cout << "Attribute Name: " << ppAttributeList[iAttrib] << " ";
    ///             cout << "Attribute Value: " << ppAttributeList[iAttrib+1] << endl;
    ///         }
    ///         \endcode
    ///
    ///\param   zName               The name of the element (includes any namespace prefix)
    ///\param   ppAttributeList     An array of null-terminated atribute name, value string pairs.
    ///                             The array itself is zero-terminated.
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void notifyStartElement( const char*    zName,
                                     const char**   ppAttributeList )
        throw() = 0;

    ///
    ///         This method is notified by the static \a XML_EndElementHandler
    ///         Expat parser callback function.
    ///
    ///\param   zName               The name of the element (includes any namespace prefix)
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void notifyEndElement( const char*      zName )
        throw() = 0;

    ///
    ///         This method is notified by the static \a XML_StartNamespaceDeclHandler
    ///         Expat parser callback function.
    ///
    ///         This callback is not currently used.
    ///
    ///\param   zPrefix             The namespace prefix.
    ///\param   zURI                The namespace URI (may be NULL).
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void notifyStartNamespace( const char*  zPrefix,
                                       const char*  zURI )
        throw() = 0;

    ///
    ///         This method is notified by the static \a XML_EndNamespaceDeclHandler
    ///         Expat parser callback function.
    ///
    ///         This callback is not currently used.
    ///
    ///\param   zPrefix             The namespace prefix.
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void notifyEndNamespace( const char*    zPrefix )
        throw() = 0;

    ///
    ///         This method is notified by the static \a XML_CharacterDataHandler
    ///         Expat parser callback function.
    ///
    ///         It is possible that this callback will be invoked a number of times
    ///         for a single CDATA block; there is no requirement that \a zCData
    ///         contain all the text in the block.
    ///
    ///\todo    Notifications for \a XML_StartCdataSectionHandler and \a XML_EndCdataSectionHandler
    ///         static callbacks are currently not implemented and should be implemented.
    ///
    ///\param   zCData              A \b non-null-terminated string of CDATA.
    ///\param   nLength             The number of characters in \a zCData.
    ///\throw   None
    ///
    _DWFCORE_API
    virtual void notifyCharacterData( const char*   zCData, 
                                      int           nLength ) 
        throw() = 0;

    ///
    ///         This method attaches a stream that can be should be used
    ///         by the parser to filter the source XML byte stream.
    ///
    ///         There may be cases when the original XML data must be examined
    ///         or even modified prior to its hitting the Expat parser.
    ///         In these cases, use another stream to filter the incoming
    ///         bytes before providing them to the parser.
    ///
    ///\param   pFilter     The filtering stream.  If ownership is to be transferred
    ///                     to this object, \a pFilter must be allocated with the
    ///                     \b DWFCORE_ALLOC_OBJECT macro.
    ///\param   bOwnFilter  If \e true, this object will delete \a pFilter using the
    ///                     \b DWFCORE_FREE_OBJECT object; if \e false, the caller
    ///                     is reponsible for managing the lifetime of \a pFilter.
    ///\throw   None
    ///
    void setStreamFilter( DWFBufferInputStream*     pFilter,
                          bool                      bOwnFilter )
        throw()
    {
        if (_pFilter && _bOwnFilter)
        {
            DWFCORE_FREE_OBJECT( _pFilter );
        }

        _pFilter = pFilter;
        _bOwnFilter = bOwnFilter;
    }

    ///
    ///         Returns the XML source stream filter, if one exists.
    ///
    ///\return  A pointer to the filter stream (may be NULL) - must not be deleted by the caller.
    ///\throw   None
    ///
    DWFBufferInputStream* getStreamFilter() const
        throw()
    {
        return _pFilter;
    }

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFXMLCallback()
        throw()
        : _nElementDepth( 0 )
        , _nNamespaceDepth( 0 )
        , _pFilter( NULL )
        , _bOwnFilter( false )
    {;}

protected:

    unsigned int _nElementDepth;
    unsigned int _nNamespaceDepth;

private:

    DWFBufferInputStream*   _pFilter;
    bool                    _bOwnFilter;
};


///
///\interface   DWFXMLBuildable    dwfcore/XML.h     "dwfcore/XML.h"
///\brief       This interface is used to indicate those classes that can be
///             dynamically constructed during the document parsing process.
///\since       7.4.0
///
class DWFXMLBuildable
{

public:

    ///
    ///\brief   This defines a structure to return unresolved attributes back to the caller
    ///         when trying to parse the attributes of an XML buildable.
    ///
    typedef struct tUnresolved
    {
        ///
        ///\brief   The derived class can use this as needed to define what unresolved
        ///         data is being returned based based on the name of the attribute.
        ///
        unsigned int    nType;
        ///
        ///\brief   The attribute value.
        ///
        DWFString       zValue;

        tUnresolved( unsigned int nUnresolveType, const DWFString& zUnresolvedValue )
            : nType( nUnresolveType )
            , zValue( zUnresolvedValue )
        {;}
    } tUnresolved;

    ///
    ///\brief   This defines a list of the tResolvedAttributes.
    ///
    typedef std::vector<tUnresolved>  tUnresolvedList;

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFXMLBuildable()
        throw()
    {;}

    ///
    ///         Receives and processes an attribute list from the parser;
    ///         using it to initialize class members.
    ///
    ///         Example:
    ///         \code
    ///          size_t iAttrib = 0;
    ///
    ///          for(; ppAttributeList[iAttrib]; iAttrib += 2)
    ///          {
    ///             //
    ///             // dump attribute 
    ///             //
    ///             cout << "Attribute Name: " << ppAttributeList[iAttrib] << " ";
    ///             cout << "Attribute Value: " << ppAttributeList[iAttrib+1] << endl;
    ///         }
    ///         \endcode
    ///
    ///\param   ppAttributeList     An array of null-terminated atribute name, value string pairs.
    ///                             The array itself is zero-terminated.
    ///\throw   DWFException
    ///
    _DWFCORE_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException ) = 0;

    ///
    ///         Receives and processes an attribute list from the parser;
    ///         using it to initialize class members.
    ///
    ///\param   ppAttributeList     An array of null-terminated atribute name, value string pairs.
    ///                             The array itself is zero-terminated.
    ///\param   rUnresolvedList     Unresolved attributes can be returned to the parser/caller 
    ///                             via this structure.
    ///\throw   DWFException
    ///
    virtual void parseAttributeList( const char** ppAttributeList, 
                                     tUnresolvedList& rUnresolvedList )
        throw( DWFException )
    {;}



protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFXMLBuildable()
        throw()
    {;}
};


///
///\class       DWFXMLNamespaceBase    dwfcore/XML.h     "dwfcore/XML.h"
///\brief       Encapsulates a namespace used to extend elements and/or attributes 
///             added to a DWF XML document.
///\since       7.4
///
class DWFXMLNamespaceBase
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFXMLNamespaceBase()
        throw()
    {;}

    ///
    ///         Constructor
    ///
    ///\param   zNamespace      The namespace prefix.  This parameter is required.
    ///                         This must be a non-DWF specified namespace
    ///\param   zXMLNS          The namespace reference (should include version).  This parameter is required.
    ///
    ///\throw   DWFException
    ///
    _DWFCORE_API
    DWFXMLNamespaceBase( const DWFString& zNamespace, const DWFString& zXMLNS )
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFXMLNamespaceBase()
        throw()
    {;}

    ///
    ///         Returns the namespace prefix.
    ///
    virtual const DWFString& prefix() const
        throw()
    {
        return _zPrefix;
    }

    ///
    ///         Returns the namespace reference uri.
    ///
    virtual const DWFString& xmlns() const
        throw()
    {
        return _zXMLNS;
    }

protected:

    DWFString _zPrefix;
    DWFString _zXMLNS;

};


///
///\class       DWFXMLSerializableBase    dwfcore/XML.h     "dwfcore/XML.h"
///\brief       This interface is used to indicate those classes that can be
///             written as XML into a stream.
///\since       7.4
///
class DWFXMLSerializableBase
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFXMLSerializableBase()
        throw()
    {;}

    ///
    ///         Writes an XML representation of the object into a stream.
    ///
    ///\param   rSerializer     The object that accepts the serialization of the object.
    ///\param   nFlags          An implementation-defined set of options for altering
    ///                         the serialization process of this object.
    ///
    _DWFCORE_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException ) = 0;

    ///
    ///         This method can be used to determine the namespace to use
    ///         during serialization.  
    ///
    ///\param   nFlags          These options should match those passed to \a serializeXML().
    ///\throw   None
    ///
    _DWFCORE_API
    virtual DWFString namespaceXML( unsigned int nFlags ) const
        throw() = 0;

    ///
    ///         Assigns an optional string that can be used as the
    ///         namespace prefix when none has been explicitly
    ///         defined; DWFXML::kzNamespace_DWF is used by default.
    ///
    ///\param   zDefaultNamespace   The namespace prefix.
    ///\throw   DWFException
    ///
    
    virtual void setDefaultNamespace( const DWFString& zDefaultNamespace )
        throw( DWFException )
    {
        _zDefaultNamespace.assign( zDefaultNamespace );
    }

    ///
    ///         May be used to add a namespace to a DWF XML document
    ///         in order to extend/add elements and/or attributes.
    ///
    ///\note    The default implementation of this method will throw a DWFNotImplementedException.
    ///         This method is only supported by some of the DWF XML document classes.
    ///
    ///\param   zNamespace  The namespace (prefix) <b>excluding</b> the colon (:).
    ///                     This parameter is required and may not be any of the
    ///                     known DWF namespaces.
    ///\param   zXMLNS      The namespace URI with version.  This parameter is required.
    ///
    ///\return  A reference to the namespace object.  This must be used in order to extend
    ///         any known element.
    ///
    ///\throw   DWFException
    ///
    
    virtual const DWFXMLNamespaceBase& addNamespace( const DWFString& /*zNamespace*/, 
                                                     const DWFString& /*zXMLNS*/ )
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"This serializable entity does not support additional namespaces." );
    }

protected:

    ///
    ///         Constructor
    ///
    ///\param   zDefaultNamespace   An optional string that can be used as the
    ///                             namespace prefix when none has been explicitly
    ///                             defined; DWFXML::kzNamespace_DWF is used by default.
    ///\throw   None
    ///
    _DWFCORE_API
    DWFXMLSerializableBase( const DWFString& zDefaultNamespace = /*NOXLATE*/L"" )
        throw();

protected:

    DWFString   _zDefaultNamespace;
};

}

#endif

