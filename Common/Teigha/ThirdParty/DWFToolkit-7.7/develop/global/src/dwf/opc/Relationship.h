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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/Relationship.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCRELATIONSHIP_H
#define _DWFTK_OPCRELATIONSHIP_H

///
///\file        dwf/opc/Relationship.h
///\brief       This file contains the OPCRelationship class declaration.
///

#include "dwfcore/Vector.h"
#include "dwfcore/SkipList.h"
#include "dwfcore/XML.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"

namespace DWFToolkit
{

//
// fwd declarations
//
class OPCPart;

///
///\ingroup     opc
///
///\class       OPCRelationship     dwf/opc/Relationship.h      "dwf/opc/Relationship.h"
///\brief       A OPCRelationship describes a relationship between a source and target part
///             in the OPC specification.
///\since       7.4.0
///
///             A OPCRelationship describes a relationship between a source and target part
///             in the OPC specification. The source maybe a package or a package-part. The
///             is not stored in the relationship since the source package or part owns the
///             relationship container that this relationship is a part of.
///
class OPCRelationship : public DWFXMLBuildable
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

    //
    //  Only a relationship container can create and destroy a relationship
    //
    friend class OPCRelationshipContainer;
    friend class OPCXMLElementBuilder;

public:

    ///
    ///\brief   This type defines a list of OPCRelationship pointers.
    ///
    typedef DWFOrderedVector<OPCRelationship*>              tList;
    ///
    ///\brief   This type defines a mapped collection of OPCRelationship pointers.
    ///
    typedef DWFStringKeySkipList<OPCRelationship*>          tMap;
    ///
    ///\brief   This type defines a caching iterator on a collection of OPCRelationship pointers,
    ///         that caches the pointers in its own container.
    ///
    typedef DWFCachingIterator<OPCRelationship*>            tCachingIterator;
    ///
    ///\brief   This type defines a basic iterator on a collection of OPCRelationship pointers.
    ///
    typedef DWFIterator<OPCRelationship*>                   tIterator;

public:

    typedef enum teTargetMode {
        //
        //  Used for relationships targeting a part in the same package
        //
        eInternal,
        
        //
        //  Used to describe relationships to a part in another package
        //
        eExternal

    } teTargetMode;

public:

    OPCPart* target() const 
        throw()
    {
        return _pTarget;
    }

    const DWFString& relationshipType() const
        throw()
    {
        return _zRelationshipType;
    }

    _DWFTK_API
    const DWFString& id() const
        throw();

    _DWFTK_API
    void setId( const DWFString& zId )
        throw();

    teTargetMode targetMode() const
        throw()
    {
        return _eTargetMode;
    }

    ///
    ///\copydoc DWFCore::DWFXMLBuildable::parseAttributeList(const char** ppAttributeList)
    ///
    _DWFTK_API
    virtual void parseAttributeList( const char** ppAttributeList )
        throw( DWFException );


#ifndef DWFTK_READ_ONLY

    ///
    ///\internal    This is used to serialize the contained relationships.
    ///
    ///\param       rSerializer     The object that accepts the serialization of the object.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

    _DWFTK_API
    const DWFString& targetURI()
        throw();

protected:

    //
    //      Constructor
    //
    _DWFTK_API
    OPCRelationship()
        throw();

    //
    //     Constructor
    //
    _DWFTK_API
    OPCRelationship( OPCPart* pTarget, 
                     DWFString zRelationshipType,
                     OPCRelationship::teTargetMode eTargetMode = eInternal )
        throw();

    //
    //     Destructor
    //
    _DWFTK_API
    virtual ~OPCRelationship()
        throw();

private:

    OPCPart*        _pTarget;
    DWFString       _zRelationshipType;
    teTargetMode    _eTargetMode;
    DWFString       _zId;

    //
    //  The target name is used when a relationship is read in because the corresponding
    //  target part may not have been serialized yet.
    //
    DWFString       _zTargetURI;
};

}

#endif
