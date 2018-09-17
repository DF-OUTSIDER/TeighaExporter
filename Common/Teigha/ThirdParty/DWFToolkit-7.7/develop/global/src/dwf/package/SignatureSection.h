//
//  Copyright (c) 1996-2005 by Autodesk, Inc.
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


#ifndef _DWFTK_SIGNATURE_SECTION_H
#define _DWFTK_SIGNATURE_SECTION_H

///
///\file        dwf/package/SignatureSection.h
///\brief       This file contains the DWFSignatureSection class declaration.
///


#include "dwfcore/STL.h"
#include "dwfcore/SkipList.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Section.h"
#include "dwf/package/reader/SignatureSectionDescriptorReader.h"
#include "dwf/package/writer/PackageWriter.h"
#include "dwf/package/Signature.h"

namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFSignatureSection   dwf/package/SignatureSection.h     "dwf/package/SignatureSection.h"
///\brief       A Signature section encapsulates the ownership of Digital Signatures in the DWF package.
///\since       7.3
///
///             The type constant for these object is \b _DWF_FORMAT_SIGNATURE_TYPE_STRING
///             (defined in dwf/Version.h)
///
class DWFSignatureSection
            : public DWFSection
            , public DWFSignatureSectionDescriptorReader
            _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   This type defines a list of DWFSignatureSection pointers.
    ///
    typedef _DWFTK_STD_VECTOR(DWFSignatureSection*)                                      tList;
    ///
    ///\brief   This type defines a mapped collection of DWFSignatureSection pointers.
    ///
    typedef DWFWCharKeySkipList<DWFSignatureSection*>                                    tMap;
    ///
    ///\brief   This type defines a multi-value mapped collection of DWFSignatureSection pointers.
    ///
    typedef std::multimap<const wchar_t*, DWFSignatureSection*, tDWFWCharCompareLess>    tMultiMap;

public:

    ///
    ///\class   Factory     dwf/package/SignatureSection.h     "dwf/package/SignatureSection.h"
    ///\brief   Class factory for DWFSignatureSection objects.
    ///\since   7.2
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
    DWFSignatureSection( const DWFString&  zName,
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
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFSignatureSection( const DWFString& zTitle,
                    const DWFString& zObjectID,
                    double           nPlotOrder,
                    const DWFSource& rSource )
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSignatureSection()
        throw();

    ///
    ///         Returns the interface associated with a data section.
    ///
    ///         This interface will be defined with the following constant data:
    ///
    ///         name:   DWFInterface::kzSignature_Name
    ///         href:   DWFInterface::kzSignature_HRef
    ///         id:     DWFInterface::kzSignature_ID
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
    ///         As a specialized section class, this class knows how to parse EPlot
    ///         section descriptor documents and build a data model from them.  It
    ///         derives this functionality by implementing the DWFSignatureSectionDescriptorReader
    ///         interface.  There are two ways to affect the default parsing process.
    ///         The first of which is to provide a non-NULL interface pointer to this method
    ///         via the \a pSectionDescriptorReader parameter.  This will bypass this object
    ///         entirely and process the descriptor using the reader provided.  Alternatively,
    ///         this object can still be used to read the descriptor document but every provider
    ///         callback can be intercepted using a filter.  A filter is just another implementation
    ///         of the DWFSignatureSectionDescriptorReader set on this object before this call is made
    ///         using the \a DWFSectionDescriptorReader::setFilter() method.  When a filter is applied,
    ///         the reader will provide data to the filter interface first and then, depending on
    ///         how the filter responds, pass the data into the default (this object) provider callback.
    ///
    ///\param   pSectionDescriptorReader    An optional interface pointer to an object that
    ///                                     knows how to read and parse EPlot section descriptor documents.
    ///
    ///\return  A reference to the EPlot descriptor document resource.
    ///\throw   DWFException
    ///
    _DWFTK_API
    const DWFResource& readDescriptor( DWFSectionDescriptorReader* pSectionDescriptorReader = NULL ) const
        throw( DWFException );

    //
    // TODO: Doc
    //
    _DWFTK_API
    DWFSignature* getSignature( DWFResource* pResource, 
                                DWFSignatureReader* pReaderFilter )
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
    ///\copydoc DWFSignatureSectionDescriptorReader::provideName()
    ///
    _DWFTK_API
    const char* provideName( const char* zName )
        throw();

    ///
    ///\copydoc DWFSignatureSectionDescriptorReader::provideObjectID()
    ///
    _DWFTK_API
    const char* provideObjectID( const char* zObjectID )
        throw();

    ///
    ///\copydoc DWFSignatureSectionDescriptorReader::provideVersion()
    ///
    _DWFTK_API
    double provideVersion( double nVersion )
        throw();

    ///
    ///\copydoc DWFSignatureSectionDescriptorReader::provideProperty()
    ///
    _DWFTK_API
    DWFProperty* provideProperty( DWFProperty* pProperty )
        throw();

    ///
    ///\copydoc DWFSignatureSectionDescriptorReader::provideResource()
    ///
    _DWFTK_API
    DWFResource* provideResource( DWFResource* pResource )
        throw();

    ///
    ///\copydoc DWFSignatureSectionDescriptorReader::provideSignatureResource()
    ///
    _DWFTK_API
    DWFSignatureResource* provideSignatureResource( DWFSignatureResource* pResource )
        throw();

private:

    DWFSignatureSection( const DWFSignatureSection& );
    DWFSignatureSection& operator=( const DWFSignatureSection& );
};

}

#endif
