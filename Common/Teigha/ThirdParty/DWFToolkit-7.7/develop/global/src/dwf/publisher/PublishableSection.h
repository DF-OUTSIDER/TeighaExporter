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

#ifndef _DWFTK_PUBLISHABLESECTION_H
#define _DWFTK_PUBLISHABLESECTION_H


///
///\file        dwf/publisher/PublishableSection.h
///\brief       This file contains the DWFPublishableSection class declaration.
///

#ifndef DWFTK_READ_ONLY



#include "dwfcore/STL.h"
#include "dwfcore/UUID.h"
#include "dwfcore/Iterator.h"
#include "dwfcore/Exception.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/publisher/Publishable.h"
#include "dwf/publisher/PublishableResource.h"
#include "dwf/publisher/EmbeddedFont.h"
#include "dwf/package/utility/PropertyContainer.h"
#include "dwf/package/utility/ResourceContainer.h"
#include "dwf/package/CoordinateSystem.h"


namespace DWFToolkit
{

//
// fwd decl
//
class DWFPublisher;



///
///\ingroup         dwfpublish
///
///\interface   DWFPublishableSection   dwf/publisher/PublishableSection.h    "dwf/publisher/PublishableSection.h"
///\brief       Base class for sections that can be used with the DWFPublisher.
///\since       7.0.1
///
class DWFPublishableSection : public DWFPublishable
                            , public DWFResourceContainer
                            , public DWFPropertyContainer
                            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\class       ResourceIterator   dwf/publisher/model/Model.h    "dwf/publisher/model/Model.h"
    ///\brief       An iterator for retrieving resources in the model.
    ///\since       7.0.1
    ///
        class ResourceIterator : public DWFIterator<DWFPublishableResource*>
                                 _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\param   rVector     The collection over which to iterate.
        ///\throw   None
        ///
        ResourceIterator( DWFPublishableResource::tList& rVector )
            throw()
            : _rVector( rVector )
            , _iCurrent( 0 )
        {;}

        ///
        ///         Copy Constructor
        ///
        ///\param   i           The iterator to clone.
        ///\throw   None
        ///
        ResourceIterator( const ResourceIterator& i )
            throw()
            : _rVector( i._rVector )
            , _iCurrent( i._iCurrent )
        {;}

        ///
        ///         Assignment Operator
        ///
        ///\param   i           The iterator to clone.
        ///\throw   None
        ///
        ResourceIterator& operator=( const ResourceIterator& i )
            throw()
        {
            _iCurrent = i._iCurrent;

            size_t n = 0;
            for (; n < i._rVector.size(); n++)
            {
                _rVector.push_back( i._rVector[n] );
            }

            return *this;
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~ResourceIterator()
            throw()
        {;}

        ///
        ///\copydoc DWFCore::DWFIterator::reset
        ///
        void reset()
            throw()
        {
            _iCurrent = 0;
        }

        ///
        ///\copydoc DWFCore::DWFIterator::valid
        ///
        bool valid()
            throw()
        {
            return (_iCurrent < _rVector.size());
        }

        ///
        ///\copydoc DWFCore::DWFIterator::next
        ///
        bool next()
            throw()
        {
            if (valid())
            {
                _iCurrent++;
                return valid();
            }
            else
            {
                return false;
            }
        }

        ///
        ///\copydoc DWFCore::DWFIterator::get
        ///
        DWFPublishableResource*& get()
            throw( DWFException )
        {
            if (valid())
            {
                return _rVector[_iCurrent];
            }
            else
            {
                _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
            }
        }

    private:

        DWFPublishableResource::tList& _rVector;
        size_t  _iCurrent;
    };

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFPublishableSection()
        throw()
    {;}

    ///
    ///         Write the section structure to \a rPublisher.
    ///
    ///\param   rPublisher      The publisher that captures the output of the section.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void publish( DWFPublisher& rPublisher )
        throw( DWFException ) = 0;

    ///
    ///         Request the provided font be embedded in the section.
    /// 
    ///\param   pFont       The font data object to embed.  This object will assume ownership of \a pFont 
    ///                     and will delete it with \b DWFCORE_FREE_OBJECT.
    ///\throw   DWFException
    ///
    virtual void embedFont( DWFEmbeddedFont* /*pFont*/ )
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Font embedding not supported by this object" );
    }

    ///
    ///         Request that the resource be added to the section.
    ///
    ///         Use this method for adding thumbnails, previews, etc.
    ///
    ///\param   pResource   The resource to add. This object will assume ownership of \a pResource 
    ///                     and will delete it with \b DWFCORE_FREE_OBJECT.
    ///\throw   DWFException
    ///
    virtual void addResource( DWFPublishableResource* /*pResource*/ )
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Additional resources are not supported by this object" );
    }

    ///
    ///         Returns the list of resources added to this section.
    ///
    ///\return  A pointer to an iterator than will provide the resources.
    ///         This pointer must be deleted by the caller with \b DWFCORE_FREE_OBJECT
    ///         but the resource pointers themselves should not be.
    ///\throw   DWFException
    ///
    virtual DWFIterator<DWFPublishableResource*>* getResources()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Additional resources are not supported by this object" );
    }

    ///
    ///             Returns the type of the section.
    ///
    ///\return      Corresponds to the section type.
    ///\throw       None
    ///
    virtual const DWFString& getType() const
        throw()
    {
        return _zType;
    }

    ///
    ///             Returns the title of the section.
    ///
    ///\return      Corresponds to the section title.
    ///\throw       None
    ///
    virtual const DWFString& getTitle() const
        throw()
    {
        return _zTitle;
    }


    ///
    ///             Returns the version of this section type/descriptor.
    ///
    ///\return      The version of this section type/descriptor.
    ///\throw       None
    ///
    _DWFTK_API
    virtual double getVersion() const
        throw() = 0;

    ///
    ///             Returns the name of the source of the section.
    ///
    ///\return      Describes the source (application, component, etc.) of the section data.
    ///\throw       None
    ///
    virtual const DWFString& getSource() const
        throw()
    {
        return _zSource;
    }

    ///
    ///             Returns an identifier of the source of the section.
    ///
    ///\return      Uniquely identifies the source (document, file, database, etc.) of the section data.
    ///\throw       None
    ///
    virtual const DWFString& getSourceID() const
        throw()
    {
        return _zSourceID;
    }

    ///
    ///             Returns the href of the source of the section.
    ///
    ///\return      An href location of the source (document, file, database, etc.) of the section data.
    ///\throw       None
    ///
    virtual const DWFString& getSourceHRef() const
        throw()
    {
        return _zSourceHRef;
    }

    ///
    ///         Sets the section label.
    ///
    ///\param   zLabel   A string containing the label.
    ///\throw   None
    ///\since   7.3.0
    ///
    virtual void setLabel( const DWFString& zLabel )
        throw()
    {
        _zLabel = zLabel;
    }

    ///
    ///             Returns the section label.
    ///
    ///\return      A string containing a label for this section
    ///\throw       None
    ///\since       7.2.0
    ///
    virtual const DWFString& getLabel() const
        throw()
    {
        return _zLabel;
    }

    ///
    ///         Gets the initial URI.
    ///
    ///\return  A string containing the URI to be applied after the section is loaded.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual const DWFString& initialURI()
        throw()
    {
        return _zInitialURI;
    }

    ///
    ///         Sets the initial URI.
    ///
    ///\param   zInitialURI       A string containing the URI to be applied after the section is loaded.
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual void setInitialURI( const DWFString& zInitialURI )
        throw()
    {
        _zInitialURI = zInitialURI;
    }

    ///
    ///         Get the section href that uniquely identifies the published section in the package. When
    ///         setting URIs into this section, using this href in the URI.
    ///
    ///\return  The section href
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual DWFString href()
        throw()
    {
        if (_zName.chars() == 0)
        {
            DWFUUID uuid;
            _zName = uuid.next( false );
        }

        DWFString zHREF( _zType );
        zHREF.append( /*NOXLATE*/L"_" );
        zHREF.append( _zName );
        
        return zHREF;
    }

    ///
    ///         Get the section name, i.e. the second portion of the section href. This is set once
    ///         a call to href() has been made. This is used by the publisher to ensure the published
    ///         section has the correct href.
    ///
    ///\return  The section name
    ///\throw   None
    ///\since   7.2.0
    ///
    virtual const DWFString& name()
        throw()
    {
        return _zName;
    }

    ///
    ///         Adds a coordinate system to be used with this publishable section
    ///
    ///\param   pCS     The coordinate system to be added to this publishable section (must
    ///                 not be NULL). This  pointer is now owned by the
    ///                 this resource and will be released with the \b
    ///                 DWFCORE_FREE_OBJECT macro.
    ///\throw   None
    ///
    void addCoordinateSystem(DWFCoordinateSystem* pCS)
        throw()
    {
        _oCSs.insert( pCS );
    }

    ///
    ///         Locates all coordinate systems in this publishable section
    ///
    ///\return  A pointer to an iterator for enumerating the coordinate systems (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Coordinate systems returned from the iterator are owned by the publishable section 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    virtual DWFCoordinateSystem::tList::Iterator* getCoordinateSystems( )
        throw()
    {
        return _oCSs.iterator();
    }

    ///
    ///         Clears the coordinate system from the publishable section.
    ///
    ///\param   pCS             The coordinate system object to remove from this container.
    ///\param   bDelete         If \e true the coordinate system object will be deleted; otherwise, the object must be 
    ///                         deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    virtual void removeCoordinateSystem( DWFCoordinateSystem* pCS,
                             bool                             bDelete )
        throw( DWFException )
    {
        _oCSs.erase(pCS);
        
        if (bDelete)
        {
            DWFCORE_FREE_OBJECT(pCS);
        }
    }

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFPublishableSection( const DWFString& zSectionType,
                           const DWFString& zSectionTitle = "",
                           const DWFString& zSectionSource = "",
                           const DWFString& zSectionSourceID = "",
                           const DWFString& zSectionSourceHRef = "",
                           const DWFString& zSectionLabel = "")
                           : _zType(zSectionType)
                           , _zTitle(zSectionTitle)
                           , _zSource(zSectionSource)
                           , _zSourceID(zSectionSourceID)
                           , _zSourceHRef(zSectionSourceHRef)
                           , _zLabel(zSectionLabel)
                           , _zInitialURI()
                           , _oCSs()
                           , _zName()                           
    {
        DWFUUID uuid;
        _zName = uuid.next( false );
    }

private:

    DWFString                       _zType;
    DWFString                       _zTitle;
    DWFString                       _zSource;
    DWFString                       _zSourceID;
    DWFString                       _zSourceHRef;
    DWFString                       _zLabel;
    DWFString                       _zInitialURI;
    DWFCoordinateSystem::tList      _oCSs;

    //
    // This defines the second part of the HREF - need this separately 
    // for setting the name of the final published section.
    //
    DWFString               _zName;

};

}

#endif  

#endif

