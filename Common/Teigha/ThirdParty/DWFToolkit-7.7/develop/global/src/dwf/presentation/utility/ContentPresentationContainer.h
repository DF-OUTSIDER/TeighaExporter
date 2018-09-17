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

//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/presentation/utility/ContentPresentationContainer.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $

#ifndef _DWFTK_CONTENT_PRESENTATION_CONTAINER_H
#define _DWFTK_CONTENT_PRESENTATION_CONTAINER_H


///
///\file        dwf/presentation/utility/ContentPresentationContainer.h
///\brief       This file contains the DWFContentPresentationContainer class declaration.
///

#include "dwfcore/Owner.h"
#include "dwfcore/STL.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/presentation/ContentPresentation.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFContentPresentationContainer   dwf/package/utility/ContentPresentationContainer.h     "dwf/package/utility/ContentPresentationContainer.h"
///\brief       An collection utility for DWFContentPresentation objects.
///\since       7.2
///
///             This class is a content presentation owner.
///
class DWFContentPresentationContainer : protected DWFOwner
                                       _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFContentPresentationContainer()
        throw();

    ///
    ///         Destructor
    ///
    ///         This method will delete all of the container presentations
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFContentPresentationContainer()
        throw();

    ///
    ///         Inserts a presentation to the container.
    ///
    ///\param   pPresentation       The presentation to add to the container (must
    ///                             not be NULL). This  pointer is now owned by the
    ///                             container and will be released with the \b
    ///                             DWFCORE_FREE_OBJECT macro.
    ///\param   bOwnPresentation    If \e true, the container will claim ownership of the presentation
    ///                             and delete it using the \b DWFCORE_FREE_OBJECT macro upon
    ///                             destruction of this container if it is still owned.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addPresentation( DWFContentPresentation* pPresentation, bool bOwnPresentation = true )
        throw( DWFException );

    ///
    ///         Locates all presentations in the container
    ///
    ///\return  A pointer to an iterator for enumerating the presentations (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Presentations returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentation::tList::Iterator* getPresentations( )
        throw();

    ///
    ///         Locates all presentations in the container
    ///
    ///\return  A pointer to a const iterator for enumerating the presentations (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Presentations returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    _DWFTK_API
    virtual DWFContentPresentation::tList::ConstIterator* getPresentations( ) const
        throw();

    ///
    ///         Clears the presentation from the container.
    ///
    ///\param   pPresentation   The presentation object to remove from this container.
    ///\param   bDeleteIfOwned  If \e true and this container currently owns the presentation,
    ///                         it will be deleted; otherwise, the presentation must be 
    ///                         tracked by the caller who must either explicitly delete it
    ///                         using the \b DWFCORE_FREE_OBJECT macro or recognize that
    ///                         another object has claimed ownership of it.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void removePresentation( DWFContentPresentation* pPresentation,
                                     bool                    bDeleteIfOwned )
        throw( DWFException );

    ///
    ///         Returns the number of presentations in the container.
    ///
    ///\return  The presentation count.
    ///\throw   None
    ///
    size_t presentationCount() const
        throw()
    {
        return _oPresentations.size();
    }


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

    ///
    ///\copydoc DWFCore::DWFOwner::notifyOwnerChanged()
    ///
    _DWFTK_API
    virtual void notifyOwnerChanged( DWFOwnable& rOwnable )
        throw( DWFException );

    ///
    ///\copydoc DWFCore::DWFOwner::notifyOwnableDeletion()
    ///
    _DWFTK_API
    virtual void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

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

        void is( DWFContentPresentationContainer* pContainer )
        {
            _pContainer = pContainer;
        }

        //
        //
        //
        virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
            throw( DWFException );

    private:

        DWFContentPresentationContainer* _pContainer;
    };

private:

    friend class _Serializer;
    _Serializer _oSerializer;

#endif


protected:

    ///
    ///\brief       The main collection of presentations
    ///
    DWFContentPresentation::tList      _oPresentations;

	typedef DWFStringKeySkipList<size_t>     IDToPostionMap;
	IDToPostionMap						  posMap;

private:

    DWFContentPresentationContainer( const DWFContentPresentationContainer& );
    DWFContentPresentationContainer& operator=( const DWFContentPresentationContainer& );
};

}

#endif
