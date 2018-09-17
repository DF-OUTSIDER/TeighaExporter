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


#ifndef _DWFTK_GLOBAL_SECTION_H
#define _DWFTK_GLOBAL_SECTION_H



///
///\file        dwf/package/GlobalSection.h
///\brief       This file contains the DWFGlobalSection class declaration.
///



#include "dwfcore/STL.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Version.h"
#include "dwf/Toolkit.h"
#include "dwf/package/Section.h"
#include "dwf/package/reader/GlobalSectionDescriptorReader.h"


namespace DWFToolkit
{


///
///\ingroup     dwfpackage
///
///\class       DWFGlobalSection   dwf/package/GlobalSection.h     "dwf/package/GlobalSection.h"
///\brief       This class contains the base functionality for all global sections in a DWF package.
///\since       7.0.1
///
///             A global section is a special construct in that the data contained
///             herein applies to all sections in the DWF package of the same type.
///             Global sections also vary from standard sections (see DWFSection) in that
///             their descriptors often contain object definitions.  These data are generally
///             small in number and, as noted, are applicable to all object defintions defined
///             in sections whose type match the global section.  
///
///             Global sections also hold bookmarks (see DWFBookmark.)  A bookmark is a
///             hyperlink reference that can apply across sections.  Defined globally,
///             bookmarks can provide a navigation option across a subsection of packaged
///             sections (per type.)
///
class DWFGlobalSection : public DWFSection,
                         public DWFGlobalSectionDescriptorReader
                         _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFGlobalSection pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFGlobalSection*)                                    tList;
    ///
    ///\brief   This type defines a mapped collection of DWFGlobalSection pointers.
    ///
    typedef DWFWCharKeySkipList<DWFGlobalSection*>                                  tMap;
    ///
    ///\brief   This type defines a multi-value mapped collection of DWFGlobalSection pointers.
    ///
    typedef std::multimap<const wchar_t*, DWFGlobalSection*, tDWFWCharCompareLess>  tMultiMap;

public:

    ///
    ///\class   Factory     dwf/package/GlobalSection.h     "dwf/package/GlobalSection.h"
    ///\brief   Class factory for DWFSection objects.
    ///\since       7.0.1
    ///
    ///         Global section factories are used by the DWFSectionBuilder.
    ///         Any custom global sections that require creation by the DWFPackageReader
    ///         must provide a section factory.
    ///
    class Factory : public DWFSection::Factory
                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\param   zType   The global section type.
        ///\throw   None
        ///
        _DWFTK_API
        Factory( const DWFString& zType )
            throw();

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~Factory()
            throw();

        ///
        ///         Create a new section.
        ///
        ///         This creation method is generally only used by the 
        ///         parsing process when the DWFPackageReader associated with 
        ///         (and providing read access to) the DWF package file is available.
        ///         The subsequent binding makes it possible to read section content
        ///         data from the DWF package.
        ///         
        ///\param   zName           A string that uniquely identifies the section in the DWF package.
        ///\param   zTitle          The descriptive and display friendly title text.
        ///\param   pPackageReader  A package reader to bind to the section.
        ///\throw   DWFException
        ///
        _DWFTK_API
        virtual DWFSection* build( const DWFString&     zName,
                                   const DWFString&     zTitle,
                                   DWFPackageReader*    pPackageReader )
            throw( DWFException );

        ///
        ///         Create a new section.
        ///
        ///         This creation method is generally only used by the 
        ///         parsing process when the DWFPackageReader associated with 
        ///         (and providing read access to) the DWF package file is available.
        ///         The subsequent binding makes it possible to read section content
        ///         data from the DWF package.
        ///         
        ///\param   zType           The section type identifier.
        ///\param   zName           A string that uniquely identifies the section in the DWF package.
        ///\param   zTitle          The descriptive and display friendly title text.
        ///\param   pPackageReader  A package reader to bind to the section.
        ///\throw   DWFException
        ///
         _DWFTK_API
        virtual DWFSection* build( const DWFString&     zType,
                                   const DWFString&     zName,
                                   const DWFString&     zTitle,
                                   DWFPackageReader*    pPackageReader )
            throw( DWFException );
    };

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read global section content
    ///         data from the DWF package.
    ///
    ///\param   zType           The global section type identifier.
    ///\param   zName           A string that uniquely identifies the global section in the DWF package.
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   pPackageReader  Provides access to global section content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFGlobalSection( const DWFString&  zType,
                      const DWFString&  zName,
                      const DWFString&  zTitle,
                      DWFPackageReader* pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally used by applications and publishers for
    ///         creating new global section objects.
    ///
    ///\param   zType           The global section type identifier.
    ///\param   zName           A string that uniquely identifies the global section in the DWF package.
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   nVersion        Identifies the version of the global section type and/or descriptor.
    ///\param   rSource         Describes the original source of the data in the global section (a drawing file, for example.)
    ///\throw   None
    ///
    _DWFTK_API
    DWFGlobalSection( const DWFString& zType,
                      const DWFString& zName,
                      const DWFString& zTitle,
                      double           nVersion,
                      const DWFSource& rSource )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFGlobalSection()
        throw();

    ///
    ///         Returns the root bookmark.
    ///
    ///         All bookmarks are rooted in an unnamed root bookmark.
    ///         This method provides access to this composite object,
    ///         if it exists.
    ///
    ///         Bookmarks Example:
    ///         \code
    ///         void dump_bookmarks( const DWFBookmark* pRoot )
    ///         {
    ///             static size_t l = 0;
    ///             size_t i;
    ///         
    ///             for (i=0;i<l;i++)
    ///             {
    ///                 wcout << "  ";
    ///             }
    ///         
    ///             if (pRoot->name())
    ///             {
    ///                 wcout << (const wchar_t*)(pRoot->name()) << endl;
    ///             }
    ///         
    ///             const DWFBookmark::tList& rChildren = pRoot->getChildBookmarks();
    ///         
    ///             for (i=0; i < rChildren.size(); i++)
    ///             {
    ///                 l++;
    ///                 dump_bookmarks( rChildren[i] );
    ///                 l--;
    ///             }
    ///         }
    ///
    ///         ...
    ///
    ///         const DWFBookmark* pRoot = pGlobal->bookmark();
    ///         if (pRoot)
    ///         {
    ///             wcout << L"\tDumping bookmarks..." << endl;
    ///             dump_bookmarks( pRoot );
    ///         }
    ///         \endcode
    ///
    ///\throw   None
    ///
    const DWFBookmark* const bookmark() const
        throw()
    {
        return _pRootBookmark;
    }

    ///
    ///\copydoc DWFSection::readDescriptor(
    ///
    _DWFTK_API
    const DWFResource& readDescriptor( DWFSectionDescriptorReader* pSectionDescriptorReader = NULL ) const
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideName()
    ///
    _DWFTK_API
    const char* provideName( const char* zName )
        throw();

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideVersion()
    ///
    _DWFTK_API
    double provideVersion( double nVersion )
        throw();

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideProperty()
    ///
    _DWFTK_API
    DWFProperty* provideProperty( DWFProperty* pProperty )
        throw();

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideResource()
    ///
    _DWFTK_API
    DWFResource* provideResource( DWFResource* pResource )
        throw();

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideFontResource()
    ///
    _DWFTK_API
    DWFFontResource* provideFontResource( DWFFontResource* pResource )
        throw();

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideGraphicResource()
    ///
    _DWFTK_API
    DWFGraphicResource* provideGraphicResource( DWFGraphicResource* pResource )
        throw();

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideImageResource()
    ///
    _DWFTK_API
    DWFImageResource* provideImageResource( DWFImageResource* pResource )
        throw();

    ///
    ///\copydoc DWFGlobalSectionDescriptorReader::provideBookmark()
    ///
    _DWFTK_API
    DWFBookmark* provideBookmark( DWFBookmark* pBookmark )
        throw();

private:

    DWFBookmark*    _pRootBookmark;

private:

    DWFGlobalSection( const DWFGlobalSection& );
    DWFGlobalSection& operator=( const DWFGlobalSection& );
};

///
///\ingroup     dwfpackage
///
///\class       DWFEPlotGlobalSection   dwf/package/GlobalSection.h     "dwf/package/GlobalSection.h"
///\brief       This class contains the base functionality for all global EPlot sections in a DWF package.
///\since       7.0.1
///
class DWFEPlotGlobalSection : public DWFGlobalSection
                              _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\class   Factory     dwf/package/GlobalSection.h     "dwf/package/GlobalSection.h"
    ///\brief   Class factory for DWFEPlotGlobalSection objects.
    ///\since       7.0.1
    ///
    ///         Global section factories are used by the DWFSectionBuilder.
    ///         Any custom global sections that require creation by the DWFPackageReader
    ///         must provide a section factory.
    ///
    class Factory : public DWFGlobalSection::Factory
                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        Factory()
            throw();

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~Factory()
            throw();

        ///
        ///\copydoc DWFGlobalSection::Factory::build(const DWFString& zName,const DWFString& zTitle,DWFPackageReader* pPackageReader)
        ///
        _DWFTK_API
        virtual DWFSection* build( const DWFString&     zName,
                                   const DWFString&     zTitle,
                                   DWFPackageReader*    pPackageReader )
            throw( DWFException );
    };

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read global EPlot section content
    ///         data from the DWF package.
    ///
    ///\param   zName           A string that uniquely identifies the global EPlot section in the DWF package.
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   pPackageReader  Provides access to global EPlot section content in the DWF package.
    ///\throw   None
    /// 
    DWFEPlotGlobalSection( const DWFString&  zName,
                           const DWFString&  zTitle,
                           DWFPackageReader* pPackageReader )
        throw()
        : DWFGlobalSection( _DWF_FORMAT_EPLOT_GLOBAL_TYPE_STRING, zName, zTitle, pPackageReader )
    {;}

    ///
    ///         Constructor
    ///
    ///         This constructor is generally used by applications and publishers for
    ///         creating new global section objects.
    ///
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   rSource         Describes the original source of the data in the global EPlot section (a drawing file, for example.)
    ///\throw   None
    ///
    DWFEPlotGlobalSection( const DWFString& zTitle = /*NOXLATE*/L"",
                           const DWFSource& rSource = DWFSource(/*NOXLATE*/L"",/*NOXLATE*/L"",/*NOXLATE*/L"") )
        throw()
        : DWFGlobalSection( _DWF_FORMAT_EPLOT_GLOBAL_TYPE_STRING,
                            _DWF_FORMAT_EPLOT_GLOBAL_TYPE_STRING,
                             zTitle,
                            _DWF_FORMAT_EPLOT_VERSION_CURRENT_FLOAT,
                             rSource )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFEPlotGlobalSection()
        throw()
    {;}

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

private:

    DWFEPlotGlobalSection( const DWFEPlotGlobalSection& );
    DWFEPlotGlobalSection& operator=( const DWFEPlotGlobalSection& );
};


///
///\ingroup     dwfpackage
///
///\class       DWFEModelGlobalSection   dwf/package/GlobalSection.h     "dwf/package/GlobalSection.h"
///\brief       This class contains the base functionality for all global EPlot sections in a DWF package.
///\since       7.0.1
///
class DWFEModelGlobalSection : public DWFGlobalSection
                               _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\class   Factory     dwf/package/GlobalSection.h     "dwf/package/GlobalSection.h"
    ///\brief   Class factory for DWFEModelGlobalSection objects.
    ///\since       7.0.1
    ///
    ///         Global section factories are used by the DWFSectionBuilder.
    ///         Any custom global sections that require creation by the DWFPackageReader
    ///         must provide a section factory.
    ///
    class Factory : public DWFGlobalSection::Factory
                    _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
    {

    public:

        ///
        ///         Constructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        Factory()
            throw();

        ///
        ///         Destructor
        ///
        ///\throw   None
        ///
        _DWFTK_API
        virtual ~Factory()
            throw();

        ///
        ///\copydoc DWFGlobalSection::Factory::build(const DWFString& zName,const DWFString& zTitle,DWFPackageReader* pPackageReader)
        ///
        _DWFTK_API
        virtual DWFSection* build( const DWFString&     zName,
                                   const DWFString&     zTitle,
                                   DWFPackageReader*    pPackageReader )
            throw( DWFException );
    };

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read global EModel section content
    ///         data from the DWF package.
    ///
    ///\param   zName           A string that uniquely identifies the global EModel section in the DWF package.
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   pPackageReader  Provides access to global EModel section content in the DWF package.
    ///\throw   None
    /// 
    DWFEModelGlobalSection( const DWFString&  zName,
                            const DWFString&  zTitle,
                            DWFPackageReader* pPackageReader )
        throw()
        : DWFGlobalSection( _DWF_FORMAT_EMODEL_GLOBAL_TYPE_STRING, zName, zTitle, pPackageReader )
    {;}

    ///
    ///         Constructor
    ///
    ///         This constructor is generally used by applications and publishers for
    ///         creating new global section objects.
    ///
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   rSource         Describes the original source of the data in the global EModel section (a drawing file, for example.)
    ///\throw   None
    ///
    DWFEModelGlobalSection( const DWFString& zTitle = /*NOXLATE*/L"",
                            const DWFSource& rSource = DWFSource(/*NOXLATE*/L"",/*NOXLATE*/L"",/*NOXLATE*/L"") )
        throw()
        : DWFGlobalSection( _DWF_FORMAT_EMODEL_GLOBAL_TYPE_STRING,
                            _DWF_FORMAT_EMODEL_GLOBAL_TYPE_STRING,
                             zTitle,
                            _DWF_FORMAT_EMODEL_VERSION_CURRENT_FLOAT,
                             rSource )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFEModelGlobalSection()
        throw()
    {;}

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

private:

    DWFEModelGlobalSection( const DWFEModelGlobalSection& );
    DWFEModelGlobalSection& operator=( const DWFEModelGlobalSection& );
};

}

#endif
