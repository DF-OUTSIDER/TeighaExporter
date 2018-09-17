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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/utility/PropertyReferenceContainer.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_PROPERTY_REFERENCE_CONTAINER_H
#define _DWFTK_PROPERTY_REFERENCE_CONTAINER_H


///
///\file        dwf/presentation/utility/PropertyReferenceContainer.h
///\brief       This file contains the DWFPropertyReferenceContainer class declaration.
///

#include "dwfcore/STL.h"

#include "dwf/Toolkit.h"
#include "dwf/presentation/PropertyReference.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFPropertyReferenceContainer   dwf/package/utility/PropertyReferenceContainer.h     "dwf/package/utility/PropertyReferenceContainer.h"
///\brief       An collection utility for DWFPropertyReference objects.
///\since       7.2
///
///             This class is a property reference owner.
///
class DWFPropertyReferenceContainer _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Constructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFPropertyReferenceContainer()
        throw();

    ///
    ///         Destructor
    ///
    ///         This method will delete all of the property references
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPropertyReferenceContainer()
        throw();

    ///
    ///         Inserts a property reference to the container.
    ///
    ///\param   pPropertyReference The property reference to add to the container
    ///                            (must not be NULL). This  pointer is now owned
    ///                            by the container and will be released with the
    ///                            \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addPropertyReference( DWFPropertyReference* pPropertyReference )
        throw( DWFException );

    ///
    ///         Locates all property references in the container
    ///
    ///\return  A pointer to an iterator for enumerating the property references (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Property references returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFPropertyReference::tList::Iterator* getPropertyReferences()
        throw();

    ///
    ///         Clears the property reference from the container.
    ///
    ///\param   pPropertyReference      The property reference object to remove
    ///                                 from this container.
    ///\param   bDelete                 If \e true the property reference will be
    ///                                 deleted; otherwise, the property reference
    ///                                 must be deleted by the caller with the
    ///                                 \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void removePropertyReference( DWFPropertyReference* pPropertyReference,
                                          bool                  bDelete )
        throw( DWFException );


#ifndef DWFTK_READ_ONLY

    ///
    ///         Returns an interface with which the container can be
    ///         serialized into an XML document.
    ///
    ///\return  A reference to a serialization inteface.
    ///\throw   None
    ///
    DWFXMLSerializable& getSerializable() const
        throw()
    {
        return (DWFXMLSerializable&)_oSerializer;
    }
#endif


#ifndef DWFTK_READ_ONLY

private:

    //
    // in order to avoid bizarre diamond patterns and
    // excessively overload base implementations,
    // all container classes must define and implementation this
    // internal [PRIVATE] class utility for serializing themselves into XML.
    //
    class _Serializer : public DWFXMLSerializable
                        _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        _Serializer()
            throw()
            : DWFXMLSerializable()
            , _pContainer( NULL )
        {;}

        virtual ~_Serializer()
            throw()
        {;}

        void is( DWFPropertyReferenceContainer* pContainer )
        {
            _pContainer = pContainer;
        }

        //
        //
        //
        virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

    private:

        DWFPropertyReferenceContainer* _pContainer;
    };

private:

    _Serializer _oSerializer;

#endif


protected:
#if defined(__BCPLUSPLUS__)
    public:
#endif

    ///
    ///\brief       The main collection of property references
    ///
    DWFPropertyReference::tList      _oPropertyReferences;

private:

    DWFPropertyReferenceContainer( const DWFPropertyReferenceContainer& );
    DWFPropertyReferenceContainer& operator=( const DWFPropertyReferenceContainer& );
};

}

#endif
