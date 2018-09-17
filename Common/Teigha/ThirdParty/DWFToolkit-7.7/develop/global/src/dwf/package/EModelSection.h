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


#ifndef _DWFTK_EMODEL_SECTION_H
#define _DWFTK_EMODEL_SECTION_H


///
///\file        dwf/package/EModelSection.h
///\brief       This file contains the DWFEModelSection class declaration.
///

#include "dwfcore/STL.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Section.h"
#include "dwf/package/reader/EModelSectionDescriptorReader.h"
#include "dwf/package/writer/PackageWriter.h"


namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFEModelSection   dwf/package/EModelSection.h     "dwf/package/EModelSection.h"
///\brief       An EModel section encapsulates a 3D data model in the DWF package.
///\since       7.0.1
///
///             The type constant for these object is \b _DWF_FORMAT_EMODEL_TYPE_STRING
///             (defined in dwf/Version.h)
///
///\note        <b><i>Units</i></b> should always be defined on an EModel section.  This information
///             allows applications processing, viewing, measuring, etc. this section
///             to correctly interpret the graphics data.  3D models will be translated
///             into unit space using the matrix obtained from DWFGraphicResource::transform().
///             The graphics transform and the units information are \b BOTH used, always.
///             3D model data is \b never assumed to be in any default unit space.
///             3D models with no unit information will be unitless; and any end-user processing
///             will require assignment of unit information at that time.
///
class DWFEModelSection : public DWFSection
                       , public DWFEModelSectionDescriptorReader
                       _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFEModelSection pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFEModelSection*)                                    tList;
    ///
    ///\brief   This type defines a mapped collection of DWFEModelSection pointers.
    ///
    typedef DWFWCharKeySkipList<DWFEModelSection*>                                  tMap;
    ///
    ///\brief   This type defines a multi-value mapped collection of DWFEModelSection pointers.
    ///
    typedef std::multimap<const wchar_t*, DWFEModelSection*, tDWFWCharCompareLess>  tMultiMap;

public:

    ///
    ///\class   Factory     dwf/package/EModelSection.h     "dwf/package/EModelSection.h"
    ///\brief   Class factory for DWFEModelSection objects.
    ///\since       7.0.1
    ///
    class Factory : public DWFSection::Factory
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
    };

public:

    ///
    ///         Constructor
    ///
    ///         This constructor is generally only used by the 
    ///         parsing process when the DWFPackageReader associated with 
    ///         (and providing read access to) the DWF package file is available.
    ///         The subsequent binding makes it possible to read section content
    ///         data from the DWF package.
    ///
    ///\param   zName           A string that uniquely identifies the section in the DWF package.
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   pPackageReader  Provides access to section content in the DWF package.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEModelSection( const DWFString&  zName,
                      const DWFString&  zTitle,
                      DWFPackageReader* pPackageReader )
        throw();

    ///
    ///         Constructor
    ///
    ///         This constructor is generally used by applications and publishers for
    ///         creating new section objects.
    ///
    ///\param   zTitle          The descriptive and display friendly title text.
    ///\param   zObjectID       Uniquely identifies the section.
    ///\param   nPlotOrder      Indicates the initial position in the package to which the section was/will be published.
    ///\param   rSource         Describes the original source of the data in the section (a drawing file, for example.)
    ///\param   pUnits          Defines the units (for measure, etc.) that applications consuming this section
    ///                         should use for all graphics data within.  A copy will be made of this object, the caller remains
    ///                         responsible for deleting this pointer.
    ///\throw   None
    ///
    _DWFTK_API
    DWFEModelSection( const DWFString& zTitle,
                      const DWFString& zObjectID,
                      double           nPlotOrder,
                      const DWFSource& rSource,
                      const DWFUnits*  pUnits )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFEModelSection()
        throw();

    ///
    ///         Returns the units into which the 3D graphics will be transformed.
    ///
    ///\return  A pointer to the units object (may be NULL)
    ///\throw   None
    ///
    DWFUnits* units() const
        throw()
    {
        return _pUnits;
    }

    ///
    ///         Returns the interface associated with an EModel section.
    ///
    ///         This interface will be defined with the following constant data:
    ///
    ///         name:   DWFInterface::kzEModel_Name
    ///         href:   DWFInterface::kzEModel_HRef
    ///         id:     DWFInterface::kzEModel_ID
    ///         
    ///\return  A pointer to a new interface object.  This object will be allocated
    ///         using the \b DWFCORE_ALLOC_OBJECT macro and must be deleted by the
    ///         caller using the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFInterface* buildInterface()
        throw( DWFException );

    ///
    ///         Locates the one descriptor resource in the section
    ///         and parses it accordingly with the reader provided
    ///         or the default reader implementation for this section by this object.
    ///
    ///         As a specialized section class, this class knows how to parse EModel
    ///         section descriptor documents and build a data model from them.  It
    ///         derives this functionality by implementing the DWFEModelSectionDescriptorReader
    ///         interface.  There are two ways to affect the default parsing process.
    ///         The first of which is to provide a non-NULL interface pointer to this method
    ///         via the \a pSectionDescriptorReader parameter.  This will bypass this object
    ///         entirely and process the descriptor using the reader provided.  Alternatively,
    ///         this object can still be used to read the descriptor document but every provider
    ///         callback can be intercepted using a filter.  A filter is just another implementation
    ///         of the DWFEModelSectionDescriptorReader set on this object before this call is made
    ///         using the \a DWFSectionDescriptorReader::setFilter() method.  When a filter is applied,
    ///         the reader will provide data to the filter interface first and then, depending on
    ///         how the filter responds, pass the data into the default (this object) provider callback.
    ///
    ///\param   pSectionDescriptorReader    An optional interface pointer to an object that
    ///                                     knows how to read and parse EModel section descriptor documents.
    ///
    ///\return  A reference to the EModel descriptor document resource.
    ///\throw   DWFException
    ///
    _DWFTK_API
    const DWFResource& readDescriptor( DWFSectionDescriptorReader* pSectionDescriptorReader = NULL ) const
        throw( DWFException );

#ifndef DWFTK_READ_ONLY

    ///
    ///\copydoc DWFXMLSerializable::serializeXML()
    ///
    _DWFTK_API
    void serializeXML( DWFXMLSerializer& rSerializer, unsigned int nFlags )
        throw( DWFException );

#endif

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideName()
    ///
    _DWFTK_API
    const char* provideName( const char* zName )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideVersion()
    ///
    _DWFTK_API
    double provideVersion( double nVersion )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::providePlotOrder()
    ///
    _DWFTK_API
    double providePlotOrder( double nPlotOrder )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideUnits()
    ///
    _DWFTK_API
    DWFUnits* provideUnits( DWFUnits* pUnits )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideProperty()
    ///
    _DWFTK_API
    DWFProperty* provideProperty( DWFProperty* pProperty )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideResource()
    ///
    _DWFTK_API
    DWFResource* provideResource( DWFResource* pResource )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideFontResource()
    ///
    _DWFTK_API
    DWFFontResource* provideFontResource( DWFFontResource* pResource )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideGraphicResource()
    ///
    _DWFTK_API
    DWFGraphicResource* provideGraphicResource( DWFGraphicResource* pResource )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideImageResource()
    ///
    _DWFTK_API
    DWFImageResource* provideImageResource( DWFImageResource* pResource )
        throw();

    ///
    ///\copydoc DWFEModelSectionDescriptorReader::provideContentPresentationResource()
    ///
    _DWFTK_API
    DWFContentPresentationResource* provideContentPresentationResource( DWFContentPresentationResource* pResource )
        throw();

private:

    DWFUnits* _pUnits;

private:

    DWFEModelSection( const DWFEModelSection& );
    DWFEModelSection& operator=( const DWFEModelSection& );
};

}

#endif
