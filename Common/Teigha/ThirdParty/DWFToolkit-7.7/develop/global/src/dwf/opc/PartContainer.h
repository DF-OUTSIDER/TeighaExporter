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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/opc/PartContainer.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_OPCPARTCONTAINER_H
#define _DWFTK_OPCPARTCONTAINER_H

#include "dwfcore/Owner.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/opc/Part.h"

namespace DWFToolkit
{


///
///\ingroup     opc
///
///\class       OPCPartContainer     dwf/opc/PartContainer.h      "dwf/opc/PartContainer.h"
///\brief       A OPCPartContainer stores and manages the life time of package parts.
///\since       7.4.0
///
///             A OPCPartContainer stores and manages the life time of package parts.
///
class OPCPartContainer : public DWFOwner
                       _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    _DWFTK_API
    OPCPartContainer()
        throw();

    ///
    ///         Destructor
    ///
    _DWFTK_API
    virtual ~OPCPartContainer()
        throw();

    ///
    ///         Add the externally created part to the part container. The part container
    ///         will take ownership of the part and manage it's lifetime.
    ///
    ///\param   pPart       The new part being added to the container.
    ///\param   bOwn        Does the Container own this object.
    ///\throw   None
    ///
    _DWFTK_API
    void addPart( OPCPart* pPart, bool bOwn = false )
        throw();

    ///
    ///         Add the part to the part container, such that it is located
    ///         after pAfterPart. The part container will take ownership of the part and 
    ///         manage it's lifetime. If pAfterPart is NULL then the part will be added at
    ///         the beginning. If pAfterPart is non-null and  is not found the new part 
    ///         will not be added and the call will return false.
    ///
    ///\param   pPart       The new part being added to the container.
    ///\param   pAfterPart  The part after which the new part will be added. If this is NULL
    ///                     the new part will be added to the beginning of the list.
    ///\param   bOwn        Does the Container own this object.
    ///\return  True if the part was added to the container, false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    bool insertPart( OPCPart* pPart, OPCPart* pAfterPart, bool bOwn = false )
        throw();

    ///
    ///         Remove a part from the container. If the container is the owner of the part
    ///         by default it will be deleted.
    ///
    ///\param   pPart           The part being removed from the container.
    ///\param   bOwnerDelete    If true, the removed part will be deleted if this is the owner
    ///                         of the part. If the container is not the owner, this flag is 
    ///                         ignored.
    ///\return  True if the part was found and removed from the container, false otherwise.
    ///\throw   None
    ///
    _DWFTK_API
    bool removePart( OPCPart* pPart, bool bOwnerDelete = true )
        throw();

    _DWFTK_API
    OPCPart::tIterator* parts()
        throw();

    _DWFTK_API
    OPCPart* part( DWFString zURI )
        throw();

protected:

    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

private:

    OPCPart::tList _oParts;

};

}

#endif
