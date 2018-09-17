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

#ifndef _DWFTK_DATA_H
#define _DWFTK_DATA_H


///
///\file        dwf/publisher/data/Data.h
///\brief       This file contains the DWFData class declaration.
///

#ifndef DWFTK_READ_ONLY

#include "dwfcore/STL.h"
#include "dwfcore/TempFile.h"
#include "dwfcore/OutputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/publisher/Publisher.h"

namespace DWFToolkit
{
///
///\ingroup         dwfpublish
///
///\class       DWFData   dwf/publisher/data/Data.h    "dwf/publisher/data/Data.h"
///\brief       A publishable object that represents a single Data DWF section.
///\since       7.2
///

class DWFData : public DWFPublishableSection
                _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:

    ///
    ///             Constructor
    ///
    ///             Use this constructor to manage the graphics stream manually.
    ///
    ///\param       zDataTitle      Corresponds to the EData section title.
    ///\param       zDataSource     Describes the source (application, component, etc.) of the plot data.
    ///\param       zDataSourceID   Uniquely identifies the source (document, file, database, etc.) of the plot data.
    ///\param       zDataSourceHRef A locator for more information about (or the actual location of) the original document.
    ///\throw       None
    ///
    _DWFTK_API
    DWFData( const DWFString& zDataTitle = "",
             const DWFString& zDataSource = "",
             const DWFString& zDataSourceID = "",
             const DWFString& zDataSourceHRef = "" )
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFTK_API
    ~DWFData()
        throw();

    ///
    ///             Prepares the data section for first use.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void open()
        throw( DWFException );


    ///
    ///             Completes the plot.
    ///
    ///             This method should be paired with \a open() to scope the
    ///             section's usage.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void close()
        throw( DWFException );

    ///
    ///             Use the publisher to extract the plot structure.
    ///
    ///\param       rPublisher      The publisher.
    ///\throw       DWFException
    ///
    _DWFTK_API
    void publish( DWFPublisher& rPublisher )
        throw( DWFException );

        //
        //
        // Publishable Interface
        //
        //

    ///
    ///\copydoc DWFPublishableSection::getVersion
    ///
    virtual double getVersion() const
        throw()
    {
        return _DWF_FORMAT_DATA_VERSION_CURRENT_FLOAT;
    }

    ///
    ///\copydoc DWFPublishableSection::getMIMEType
    ///
    const DWFString& getMIMEType()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"Data sections do not have a MIME type" );
    }

    ///
    ///\copydoc DWFPublishableSection::getInputStream
    ///
    DWFInputStream* getInputStream()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFUnexpectedException, /*NOXLATE*/L"Data sections do not return an input stream" );
    }

    ///
    ///\copydoc DWFPublishableSection::embedFont
    ///
    _DWFTK_API
    void embedFont( DWFEmbeddedFont* pFont )
        throw( DWFException );

    ///
    ///\copydoc DWFPublishableSection::addResource
    ///
    _DWFTK_API
    void addResource( DWFPublishableResource* pResource )
        throw( DWFException );

    ///
    ///\copydoc DWFPublishableSection::getResources
    ///
    _DWFTK_API
    DWFIterator<DWFPublishableResource*>* getResources()
        throw( DWFException );

private:
    bool                    _bOpen;

    DWFEmbeddedFont::tList          _oEmbeddedFonts;
    DWFPublishableResource::tList   _oResources;

private:

    //
    // Not Implemented
    //

    DWFData( const DWFData& );
    DWFData& operator=( const DWFData& );
};

}

#endif

#endif
