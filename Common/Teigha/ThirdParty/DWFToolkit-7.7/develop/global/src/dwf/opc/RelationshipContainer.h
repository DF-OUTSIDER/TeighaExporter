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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/RelationshipContainer.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCRELATIONSHIPCONTAINER_H
#define _DWFTK_OPCRELATIONSHIPCONTAINER_H


#include "dwf/opc/Relationship.h"
#include "dwf/opc/RelationshipContainerInterface.h"
#include "dwf/opc/reader/RelationshipContainerReader.h"


namespace DWFToolkit
{

///
///\ingroup     opc
///
///\class       DWFXRelationshipContainer     dwf/dwfx/RelationshipContainer.h      "dwf/dwfx/RelationshipContainer.h"
///\brief       A DWFXRelationshipContainer stores and manages the life time of relationships associated with 
///             a package part
///\since       7.4.0
///
///             A DWFXRelationshipContainer stores and manages the life time of relationships associated with 
///             a package part. Only a RelationshipContainer can create or destroy a DWFXRelationship. Any
///             object that have relationships to target parts must store and manage them via such a container.
///
class OPCRelationshipContainer : public OPCRelationshipContainerInterface
                               , public OPCRelationshipContainerReader
                               _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    _DWFTK_API
    OPCRelationshipContainer()
        throw();

    _DWFTK_API
    virtual ~OPCRelationshipContainer()
        throw();

    _DWFTK_API
    virtual OPCRelationship* addRelationship( OPCPart* pTargetPart, 
                                              const DWFString& zRelationshipType, 
                                              OPCRelationship::teTargetMode eTargetMode )
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
    virtual OPCRelationship::tIterator* relationshipsByTarget( OPCPart* pPart ) const
        throw();

    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByType( const DWFString& zType ) const
        throw();

    _DWFTK_API
    virtual void deleteRelationshipsByTarget( OPCPart* pPart )
        throw( DWFException );

    _DWFTK_API
    DWFInputStream* getInputStream()
        throw( DWFException );

    _DWFTK_API
    virtual void loadRelationships(DWFInputStream* pRelsInputStream)
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeRelationship( DWFXMLSerializer& rSerializer )
        throw();

#endif

    _DWFTK_API
    virtual OPCRelationship* provideRelationship( OPCRelationship* pRelationship )
        throw();

protected:

    //
    // Buffer to which the XML is written for serialization of this content.
    //
    char*           _pBuffer;

private:

    OPCRelationship::tList _oRelationships;

};

}

#endif
