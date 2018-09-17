//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/package/Manifest.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//


#ifndef _DWFTK_MANIFEST_H
#define _DWFTK_MANIFEST_H


///
///\file        dwf/package/Manifest.h
///\brief       This file contains the DWFManifest class declaration.
///


#include "dwf/Toolkit.h"
#include "dwf/package/Section.h"
#include "dwf/package/GlobalSection.h"
#include "dwf/package/writer/PackageWriter.h"
#include "dwf/package/reader/PackageManifestReader.h"
#include "dwf/package/utility/PropertyContainer.h"
#include "dwf/presentation/PackageContentPresentations.h"

#include "dwfcore/Owner.h"
#include "dwfcore/Iterator.h"
using namespace DWFCore;


namespace DWFToolkit
{

//
// fwd declarations
//
class DWFContentManager;


///
///\ingroup     dwfpackage
///
///\class       DWFManifest   dwf/package/Manifest.h     "dwf/package/Manifest.h"
///\brief       This class represents a fully parsed DWF package manifest document.
///\since       7.0.1
///
class DWFManifest : public DWFManifestReader

#ifndef DWFTK_READ_ONLY
                  , public DWFXMLSerializable
#endif
                  , public DWFPropertyContainer
                  _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines the iterator for enumerating manifest interfaces.
    ///
    typedef DWFWCharKeySkipList<DWFInterface*>::Iterator    tInterfaceIterator;

    ///
    ///\class   SectionIterator     dwf/package/Manifest.h     "dwf/package/Manifest.h"
    ///\brief   This class defines the iterator for enumerating manifest sections.
    ///\since   7.0.1
    ///
    ///         This iterator implementation works over STL lists and maps.
    ///
    class SectionIterator : public DWFIterator<DWFSection*>
                            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\param   iBegin  Points to the first section.
        ///\param   iEnd    Points to the end of the section list.
        ///\throw   None
        ///
        SectionIterator( DWFSection::tList::iterator iBegin,
                         DWFSection::tList::iterator iEnd )
            throw()
            : _bList( true )
            , _ilBegin( iBegin )
            , _ilEnd( iEnd )
            , _ilCurrent( iBegin )
        {;}

        ///
        ///         Constructor
        ///
        ///\param   iBegin  Points to the first section.
        ///\param   iEnd    Points to the end of the section map.
        ///\throw   None
        ///
        SectionIterator( std :: multimap<const wchar_t*, DWFSection*, tDWFWCharCompareLess>::iterator iBegin,
                         std :: multimap<const wchar_t*, DWFSection*, tDWFWCharCompareLess>::iterator iEnd )
            throw()
            : _bList( false )
            , _imBegin( iBegin )
            , _imEnd( iEnd )
            , _imCurrent( iBegin )
        {;}

        ///
        ///         Copy Constructor
        ///
        ///\param   i       The iterator to clone.
        ///\throw   None
        ///
        SectionIterator( const SectionIterator& i )
            throw()
            : _bList( i._bList )
            , _ilBegin( i._ilBegin )
            , _ilEnd( i._ilEnd )
            , _ilCurrent( i._ilCurrent )
            , _imBegin( i._imBegin )
            , _imEnd( i._imEnd )
            , _imCurrent( i._imCurrent )
        {;}

        ///
        ///         Assignment Operator
        ///
        ///\param   i       The iterator to clone.
        ///\throw   None
        ///
        SectionIterator& operator=( const SectionIterator& i )
            throw()
        {
            _ilBegin = i._ilBegin;
            _ilEnd = i._ilEnd;
            _ilCurrent = i._ilCurrent;

            _imBegin = i._imBegin;
            _imEnd = i._imEnd;
            _imCurrent = i._imCurrent;

            _bList = i._bList;

            return *this;
        }

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        virtual ~SectionIterator()
            throw()
        {;}

        ///
        ///\copydoc DWFCore::DWFIterator::reset()
        ///
        void reset()
            throw()
        {
            if (_bList)
            {
                _ilCurrent = _ilBegin;
            }
            else
            {
                _imCurrent = _imBegin;
            }
        }

        ///
        ///\copydoc DWFCore::DWFIterator::valid()
        ///
        bool valid()
            throw()
        {
            return (_bList ? (_ilCurrent != _ilEnd) : (_imCurrent != _imEnd));
        }

        ///
        ///\copydoc DWFCore::DWFIterator::next()
        ///
        bool next()
            throw()
        {
            if (valid())
            {
                if (_bList)
                {
                    _ilCurrent++;
                }
                else
                {
                    _imCurrent++;
                }

                return valid();
            }
            else
            {
                return false;
            }
        }

        ///
        ///\copydoc DWFCore::DWFIterator::get()
        ///
        DWFSection*& get()
            throw( DWFException )
        {
            if (valid())
            {
                return (_bList ? *_ilCurrent : _imCurrent->second);
            }
            else
            {
                _DWFCORE_THROW( DWFDoesNotExistException, /*NOXLATE*/L"No more elements" );
            }
        }

    private:

        bool _bList;

        DWFSection::tList::iterator _ilBegin;
        DWFSection::tList::iterator _ilEnd;
        DWFSection::tList::iterator _ilCurrent;

        DWFSection::tMultiMap::iterator _imBegin;
        DWFSection::tMultiMap::iterator _imEnd;
        DWFSection::tMultiMap::iterator _imCurrent;
    };

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read content
    ///         from the DWF package.
    ///
    ///\param   pPackageReader  Provides access to section content in the DWF package.
    ///\throw   None
    ///    
    _DWFTK_API
    DWFManifest( DWFPackageReader* pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally used by applications and publishers for
    ///         creating a package manifest.
    ///
    ///\param   zObjectID       Uniquely identifies the manifest.
    ///\throw   None
    ///
    _DWFTK_API
    DWFManifest( const DWFString& zObjectID )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFManifest()
        throw();

    ///
    ///         Returns version of the manifest document schema.
    ///
    ///\return  The version number.
    ///\throw   None
    ///
    double version() const
        throw()
    {
        return _nVersion;
    }

    ///
    ///         Returns the unique identifer of the manifest document.
    ///
    ///\return  The manifest object ID.
    ///\throw   None
    ///
    const DWFString& objectID() const
        throw()
    {
        return _zObjectID;
    }

    ///
    ///         Returns an iterator for listing each unique section interface in the manifest.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None
    ///
    _DWFTK_API
    tInterfaceIterator* getInterfaces()
        throw();

    ///
    ///         Locates a section in the manifest by name.
    ///
    ///\param   zName   The unique name of the section in the package.
    ///\return  A pointer to the section, if it exists, NULL otherwise.
    ///         The caller must not delete this pointer without explicitly
    ///         claiming ownership of it using the DWFCore::DWFOwnable interface
    ///         exposed by DWFSection.
    ///\throw   None
    ///
    _DWFTK_API
    DWFSection* findSectionByName( const DWFString& zName )
        throw();

    ///
    ///         Locates all sections in the manifest that share a type.
    ///
    ///\param   zType   The type of sections to locate.
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///         The caller must not delete any section pointers obtained from the iterator without explicitly
    ///         claiming ownership of it using the DWFCore::DWFOwnable interface exposed by DWFSection.
    ///\throw   None
    ///
    _DWFTK_API
    SectionIterator* findSectionsByType( const DWFString& zType )
        throw();

    ///
    ///         Locates all non-global sections in the package.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///         The caller must not delete any section pointers obtained from the iterator without explicitly
    ///         claiming ownership of it using the DWFCore::DWFOwnable interface exposed by DWFSection.
    ///\throw   None
    ///
    _DWFTK_API
    SectionIterator* getSections()
        throw();

    ///
    ///         Locates all global sections in the package.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///         The caller must not delete any section pointers obtained from the iterator without explicitly
    ///         claiming ownership of it using the DWFCore::DWFOwnable interface exposed by DWFSection.
    ///\throw   None
    ///
    _DWFTK_API
    SectionIterator* getGlobalSections()
        throw();

    ///
    ///         Get the content manager from the manifest. The caller should not delete the content
    ///         manager unless it was attached to the manifest using \a attachContentManager with
    ///         \a bTakeOwnership set to false.
    ///
    ///\return  A pointer the to content manager that the manifest points to.
    ///\throw   None
    ///
    DWFContentManager* getContentManager() const
        throw()
    {
        return _pContentManager;
    }

    ///
    ///         Gets the package level content presentations.
    ///
    ///\return  A pointer to an iterator for enumerating the presentations (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Presentations returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    ///
    DWFContentPresentation::tList::Iterator* getContentPresentations()
        throw()
    {
        DWFContentPresentation::tList::Iterator* piPresentations = NULL;
        if (_pPackageContentPresentations)
        {
            piPresentations = _pPackageContentPresentations->getPresentations();
        }

        return piPresentations;
    }

    ///
    ///         Gets the package level content presentations.
    ///
    ///\return  A pointer to a const iterator for enumerating the presentations (may be NULL).  
    ///         This pointer must be deleted by the caller with the \b DWFCORE_FREE_OBJECT
    ///         macro.
    ///         Presentations returned from the iterator are owned by the container 
    ///         and must not be deleted by the caller.
    ///\throw   None
    ///
    ///
    DWFContentPresentation::tList::ConstIterator* getContentPresentations() const
        throw()
    {
        DWFContentPresentation::tList::ConstIterator* piPresentations = NULL;
        if (_pPackageContentPresentations)
        {
            piPresentations = ((const DWFContentPresentationContainer*)(_pPackageContentPresentations))->getPresentations();
        }

        return piPresentations;
    }

    ///
    ///         This attaches the content manager to the manifest.
    ///         The manifest will take ownership of the manager, only if requested to.
    ///
    ///\param   pContentManager The content manager to attach.
    ///\param   bTakeOwnership  Set this to true if the manifest should take ownership.
    ///\param   bDeletePrevious Set this to true if a previously attached content manager should be deleted.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void attachContentManager( DWFContentManager* pContentManager, bool bTakeOwnership = true, bool bDeletePrevious = true )
        throw( DWFException );

    ///
    ///\copydoc DWFManifestReader::provideVersion()
    ///
    _DWFTK_API
    double provideVersion( double nVersion )
        throw();

    ///
    ///\copydoc DWFManifestReader::provideObjectID()
    ///
    _DWFTK_API
    const char* provideObjectID( const char* zObjectID )
        throw();

    ///
    ///\copydoc DWFManifestReader::provideDependency()
    ///
    _DWFTK_API
    DWFDependency* provideDependency( DWFDependency* pDependency )
        throw();

    ///
    ///\copydoc DWFManifestReader::provideInterface()
    ///
    _DWFTK_API
    DWFInterface* provideInterface( DWFInterface* pInterface )
        throw();

    ///
    ///\copydoc DWFManifestReader::provideProperty()
    ///
    _DWFTK_API
    DWFProperty* provideProperty( DWFProperty* pProperty )
        throw();

    ///
    ///\copydoc DWFManifestReader::provideSection()
    ///
    _DWFTK_API
    DWFSection* provideSection( DWFSection* pSection )
        throw();

    ///
    ///         Set the content manager pointed to by the manifest. 
    ///
    ///         If the owner of the content manager is null, the manifest will take
    ///         ownership. If it already has a owner, the manifest maybe explicitly
    ///         granted the ownership by calling the manager's \a own() method.
    ///         To revoke ownership (to prevent it's deletion with the manifest) call
    ///         the manager's \a disown() method.
    ///
    ///\param   pContent    The new content manager. 
    ///\throw   None.
    ///
    _DWFTK_API
    DWFContent* provideContent( DWFContent* pContent )
        throw( DWFException );

    ///
    ///\copydoc DWFManifestReader::provideContentPresentations()
    ///
    _DWFTK_API
    DWFPackageContentPresentations* provideContentPresentations( DWFPackageContentPresentations* pPackageContentPresentations )
        throw();

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

protected:

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

private:

    double                              _nVersion;
    DWFString                           _zObjectID;
    DWFSection::tList                   _oSectionsInOrder;
    DWFSection::tList                   _oGlobalSectionsInOrder;
    DWFSection::tMap                    _oSectionsByName;
    DWFSection::tMultiMap               _oSectionsByType;
    DWFWCharKeySkipList<DWFInterface*>  _oInterfaces;

    DWFContentManager*                  _pContentManager;
    DWFPackageContentPresentations*     _pPackageContentPresentations;
};

}

#endif
