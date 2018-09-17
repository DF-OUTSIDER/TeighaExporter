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


#ifndef _DWFTK_SECTION_BUILDER_H
#define _DWFTK_SECTION_BUILDER_H


///
///\file        dwf/package/SectionBuilder.h
///\brief       This file contains the DWFSectionBuilder class declaration.
///

#include "dwfcore/String.h"
using namespace DWFCore;

#include "dwf/package/Section.h"


namespace DWFToolkit
{


///
///\ingroup     dwfpackage
///
///\class       DWFSectionBuilder   dwf/package/SectionBuilder.h     "dwf/package/SectionBuilder.h"
///\brief       This class is used to create new strongly typed section objects.
///\since       7.0.1
///
///             DWF is an extensible format; and while the toolkit knows how to process
///             those object matching the format specification, it also allows for
///             customization.  Using this class, custom section creation can be
///             integrated into the general package reading process.
///
class DWFSectionBuilder     _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFSectionBuilder()
        throw();

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFSectionBuilder()
        throw();

    ///
    ///         Adds a new factory to the builder.
    ///
    ///         If the factory provided is of the same type as one already
    ///         in the builder, the existing factory will be replaced and
    ///         returned.  The factory is keyed by DWFSection::Factory::type().
    ///
    ///\param   pFactory    A pointer to the new factory (must not be NULL).
    ///                     If this object will not be explicitly removed
    ///                     from the builder, it will be deleted by the builder
    ///                     using the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\return  A pointer to the previous factory for the same section type (may be NULL).
    ///         The caller is responsible for freeing this pointer.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFSection::Factory* addFactory( DWFSection::Factory* pFactory )
        throw( DWFException );

    ///
    ///         Returns the factory for the specified type.
    ///
    ///\param   zType   Identifies the section factory.
    ///\return  A pointer to the factory that was removed from this builder (may be NULL).
    ///         The caller is responsible for freeing this pointer.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFSection::Factory* removeFactory( const DWFString& zType )
        throw( DWFException );

    ///
    ///         Allocates a new section object based on the attribute data
    ///         parsed from the document.
    ///
    ///         The section is created using a registered section factory
    ///         or the default implementation provided by DWFSection::Factory::buildSection()
    ///
    ///\param   ppAttributeList     An array of null-terminated atribute name, value string pairs.
    ///                             The array itself is zero-terminated.
    ///\param   pPackageReader      A package reader to bind to the section from which resource
    ///                             content will be extracted.
    ///\return  A pointer to the new section. Caller must delete this object with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFSection* buildSection( const char**      ppAttributeList,
                              DWFPackageReader* pPackageReader )
        throw( DWFException );

    ///
    ///         Allocates a new section object.
    ///
    ///         The section is created using a registered section factory
    ///         or the default implementation provided by DWFSection::Factory::buildSection()
    ///
    ///\param   zType               The section type identifier.
    ///\param   zName               A string that uniquely identifies the section in the DWF package.
    ///\param   zTitle              The descriptive and display friendly title text.
    ///\param   pPackageReader      A package reader to bind to the section from which resource
    ///                             content will be extracted.
    ///\return  A pointer to the new section. Caller must delete this object with the \b DWFCORE_FREE_OBJECT macro.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFSection* buildSection( const DWFString&  zType,
                              const DWFString&  zName,
                              const DWFString&  zTitle,
                              DWFPackageReader* pPackageReader )
        throw( DWFException );

private:

    DWFSection::Factory::tMap   _oFactories;
    static DWFSection::Factory  _koDefaultFactory;

private:

    DWFSectionBuilder( const DWFSectionBuilder& );
    DWFSectionBuilder& operator=( const DWFSectionBuilder& );
};


}


#endif
