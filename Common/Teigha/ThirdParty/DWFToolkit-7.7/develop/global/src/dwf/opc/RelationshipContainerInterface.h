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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/RelationshipContainerInterface.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCRELATIONSHIPCONTAINERINTERFACE_H
#define _DWFTK_OPCRELATIONSHIPCONTAINERINTERFACE_H

#include "dwf/opc/Relationship.h"

namespace DWFToolkit
{

class OPCRelationshipContainerInterface
{

public:

    ///
    ///         Create and add a new relationship from the container to the specified part.
    ///
    ///\param   pTargetPart         The target of the new relationship.
    ///\param   zRelationshipType   The type to be assigned to the relationship.
    ///\param   eTargetMode         The mode of the relationship - internal or external.
    ///\return  Pointer to the new relationship
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual OPCRelationship* addRelationship( OPCPart* pTargetPart, 
                                              const DWFString& zRelationshipType, 
                                              OPCRelationship::teTargetMode eTargetMode = OPCRelationship::eInternal )
        throw( DWFException ) = 0;

    ///
    ///         Delete a specific relationship
    ///
    ///\param   pRelationship       Pointer to the relationship to be removed.
    ///\throw   None
    ///
    _DWFTK_API
    virtual bool deleteRelationship( OPCRelationship* pRelationship )
        throw() = 0;

    ///
    ///         Get the total number of relationships from this item.
    ///
    ///\return  The number of relationships from this item.
    ///\throw   None.
    ///
    _DWFTK_API
    virtual size_t relationshipCount() const
        throw() = 0;

    ///
    ///         Get all the relationships from this item
    ///
    ///\return  Pointer to an iterator over all relationships. It may be NULL. It must be
    ///         deleted using \b DWFCORE_FREE_OBJECT.
    ///\throw   None.
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationships()
        throw() = 0;

    ///
    ///         Get all the relationships from this item to a specific target.
    ///
    ///\param   zTargetURI      Relationships to the target with this URI will be returned.
    ///\return  Pointer to an iterator over the relationships. It may be NULL. It must be
    ///         deleted using \b DWFCORE_FREE_OBJECT.
    ///\throw   None.
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByTarget( const DWFString& zTargetURI ) const
        throw() = 0;

    ///
    ///         Get all the relationships from this item to a specific target.
    ///
    ///\param   pTargetPart     Relationships to this target will be returned.
    ///\return  Pointer to an iterator over the relationships. It may be NULL. It must be
    ///         deleted using \b DWFCORE_FREE_OBJECT.
    ///\throw   None.
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByTarget( OPCPart* pTargetPart ) const
        throw() = 0;

    ///
    ///         Get all the relationships of a given type from this item.
    ///
    ///\param   zType           The type of relationships to be iterated over.
    ///\return  Pointer to an iterator over the relationships. It may be NULL. It must be
    ///         deleted using \b DWFCORE_FREE_OBJECT.
    ///\throw   None.
    ///
    _DWFTK_API
    virtual OPCRelationship::tIterator* relationshipsByType( const DWFString& zType ) const
        throw() = 0;

    ///
    ///         Remove all relationships from this item to the specified target.
    ///
    ///\param   pTargetPart     Relationships to this target willl be removed.
    ///\throw   None.
    ///
    _DWFTK_API
    virtual void deleteRelationshipsByTarget( OPCPart* pTargetPart )
        throw( DWFException ) = 0;

    ///
    ///         Get the input stream that will provide the serialized bytes of 
    ///         all relationships in the container.
    ///
    ///\return  Pointer to an input stream. It must be deleted by the caller using 
    ///         \b DWFCORE_FREE_OBJECT.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFInputStream* getInputStream()
        throw( DWFException );

protected:

    OPCRelationshipContainerInterface()
        throw()
    {;}

    virtual ~OPCRelationshipContainerInterface()
        throw()
    {;}

};

}

#endif
