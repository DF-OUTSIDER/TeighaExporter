//
//  Copyright (c) 2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/Part.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCPART_H
#define _DWFTK_OPCPART_H

#include "dwfcore/Owner.h"
#include "dwfcore/InputStream.h"
using namespace DWFCore;

#include "dwf/opc/RelationshipContainerInterface.h"

namespace DWFToolkit
{

//
//  fwd declarations
//
class OPCRelationshipContainer;

///
///\ingroup     opc
///
///\class       OPCPart     dwf/opc/Part.h      "dwf/opc/Part.h"
///\brief       A OPCPart corresponds to a part in the OPC specification.
///\since       7.4.0
///
///             TODO
///             The OPCPart class ...
///
class OPCPart : public OPCRelationshipContainerInterface
              , public DWFOwnable
              _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of OPCPart pointers.
    ///
    typedef DWFOrderedVector<OPCPart*>              tList;
    ///
    ///\brief   This type defines a mapped collection of OPCPart pointers.
    ///
    typedef DWFStringKeySkipList<OPCPart*>          tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of OPCPart pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<OPCPart*>            tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of OPCPart pointers.
    ///
    typedef DWFIterator<OPCPart*>                   tIterator;

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    OPCPart()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~OPCPart()
        throw();

    ///
    ///         Set the path of the part - this does not include the name of the
    ///         part itself. For example if the full URI is /dwf/doc/1/part.xml
    ///         this should be used to set /dwf/doc/1
    ///
    ///\param   zPath       The path to the part.
    ///\throw   None
    ///
    virtual void setPath( const DWFString& zPath )
        throw()
    {
        _zPath = zPath;
    }

    ///
    ///         Set the name of the part - this does not include any path
    ///         information. For example if the full URI is /dwf/doc/1/part.xml
    ///         this should be used to set part.xml. Note: currently nothing
    ///         prevents one from setting the part name as a/b.ext. The only
    ///         constraint is that it does not start with ".." or "/".
    ///
    ///\param   zName       The name of the part.
    ///\throw   DWFExcetion
    ///
    _DWFTK_API
    virtual void setName( const DWFString& zName )
        throw( DWFException );

    ///
    ///         Get the path of to the part
    ///
    ///\return  The path of the part.
    ///\throw   None
    ///
    virtual const DWFString& path() const
        throw()
    {
        return _zPath;
    }

    ///
    ///         Get the name of to the part
    ///
    ///\return  The name of the part.
    ///\throw   None
    ///
    virtual const DWFString& name() const
        throw()
    {
        return _zName;
    }

    ///
    ///         Get the URI of the part.
    ///
    ///\return  The URI of the part.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFString uri() const
        throw( DWFException );

    ///
    ///         Get the URI of the relationship corresponding to this part. It is 
    ///         usually computed from the URI. 
    ///         If the part URI is /path/PartURI, then the relationship URI should
    ///         normally be /path/_rels/PartURI.rels
    ///
    ///\return  The URI for the relationship part corresponding to the this part.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFString relationshipUri() const
        throw( DWFException );

    _DWFTK_API
    virtual OPCRelationship* addRelationship( OPCPart* pTargetPart, 
                                              const DWFString& zRelationshipType, 
                                              OPCRelationship::teTargetMode eTargetMode = OPCRelationship::eInternal )
        throw( DWFException );

    _DWFTK_API
    virtual bool deleteRelationship( OPCRelationship* pRelationship )
        throw();

    _DWFTK_API
    virtual size_t relationshipCount() const
        throw();

    _DWFTK_API
    virtual OPCRelationship::tIterator* relationships()
        throw();

    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByTarget( const DWFString& zTargetURI ) const
        throw();

    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByTarget( OPCPart* ) const
        throw();

    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByType( const DWFString& zType ) const
        throw();

    _DWFTK_API
    virtual void deleteRelationshipsByTarget( OPCPart* pPart )
        throw( DWFException );

    _DWFTK_API
    virtual DWFInputStream* getInputStream()
        throw( DWFException );

    _DWFTK_API
    virtual DWFInputStream* getRelationshipsInputStream()
        throw( DWFException );

    _DWFTK_API
    virtual void loadRelationships(DWFInputStream* pRelsInputStream)
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void setInputStream( DWFInputStream* pInputStream, 
                                 size_t nBytes,
                                 bool bOwnStream = true )
        throw( DWFException );

#endif

protected:

    DWFString           _zPath;
    DWFString           _zName;

    DWFInputStream*     _pInputStream;
    size_t              _nStreamBytes;
    bool                _bOwnStream;

private:

    OPCRelationshipContainer*   _pRelsContainer;
    
private:

    //
    // Not implemented
    //
    OPCPart( const OPCPart& );
    OPCPart& operator=( const OPCPart& );

};

}

#endif
