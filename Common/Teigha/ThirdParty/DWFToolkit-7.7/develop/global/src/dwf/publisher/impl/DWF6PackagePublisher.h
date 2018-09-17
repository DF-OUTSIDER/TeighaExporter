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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/impl/DWF6PackagePublisher.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_DWF6_PACKAGE_PUBLISHER_H
#define _DWFTK_DWF6_PACKAGE_PUBLISHER_H


///
///\file        dwf/publisher/impl/DWF6PackagePublisher.h
///\brief       This file contains the DWF6PackagePublisher class declaration.
///


#ifndef DWFTK_READ_ONLY

#include "dwfcore/File.h"

using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Section.h"
#include "dwf/package/writer/DWF6PackageWriter.h"
#include "dwf/publisher/Image.h"
#include "dwf/publisher/impl/DWFPackagePublisher.h"
#include "dwf/publisher/model/Model.h"
#include "dwf/publisher/plot/Plot.h"
#include "dwf/publisher/data/Data.h"
#include "dwf/publisher/impl/DWFResourceVisitor.h"

namespace DWFToolkit
{

//
//  fwd declaration
//
class DWFContent;
class DWFPublishedContentElementListener;

///
///\ingroup         dwfpublish
///
///\class       DWF6PackagePublisher   dwf/publisher/impl/DWF6PackagePublisher.h   "dwf/publisher/impl/DWF6PackagePublisher.h"
///\brief       A publisher implementation for producing DWF package files.
///\since       7.4.0
///
///             This is a reference implementation of the DWFPublisher base class for creating DWF packages.
///
///             Due to the composite nature of the the visitors, even this class can be used to extract
///             additional data to other output pipes during the creation of the DWF package.
///
///\todo        Implement a full version for composite DWF packages.
///
class DWF6PackagePublisher : public DWFPackagePublisher
                           _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///         Constructor
    ///
    ///         This documentation has been copied from DWFPackageWriter::write():
    ///
    ///         This method exposes the standard DWF package properties directly.
    ///         It is highly recommended that at DWF publishers set these correctly with as
    ///         much detail as possible.
    ///
    ///         Setting standard DWF package properties:
    ///         \code
    ///         Autodesk DWF Writer is a printer driver DLL that is used to generate DWF packages
    ///         through the Windows printing system, it is a component that another application
    ///         would use (indirectly) to publish the DWF.  The following is an example of how
    ///         it might populate these properties when printing from Microsoft Word:
    ///
    ///         zSourceProductVendor    = L"Microsoft Corporation"          <--- from Word 2003
    ///         zSourceProductName      = L"Microsoft Office 2003"          <--- from Word 2003
    ///         zSourceProductVersion   = L"11.0.5604"                      <--- from Word 2003
    ///         zDWFProductVendor       = L"Autodesk, Inc."                 <--- from DWF Writer 2
    ///         zDWFProductVersion      = L"2.0.4.0"                        <--- from DWF Writer 2
    ///         \endcode
    ///
    ///\param   rDWFFile                    The destination DWF package file name.
    ///\param   zPassword                   An optional password to protect the DWF package.
    ///\param   zSourceProductVendor        A standard property detailing the vendor of the software
    ///                                     that the produced the original content and used the DWF product
    ///                                     in order to produce the DWF package.
    ///\param   zSourceProductName          A standard property detailing the name of the software
    ///                                     that the produced the original content and used the DWF product
    ///                                     in order to produce the DWF package.
    ///\param   zSourceProductVersion       A standard property detailing the version of the software
    ///                                     that the produced the original content and used the DWF product
    ///                                     in order to produce the DWF package.
    ///\param   zPublisherProductVendor     A standard property that details the vendor of the software component
    ///                                     that was used by the source product to publish the DWF package.
    ///\param   zPublisherProductVersion    A standard property that details the version of the software component
    ///                                     that was used by the source product to publish the DWF package.
    ///\param   bNoPasswordSalting          If password salting is enabled it prevents the usage of zip password recovery tools
    ///                                     to determine the password from the DWF package.
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWF6PackagePublisher( const DWFFile&   rDWFFile,
                          const DWFString& zPassword                = "",
                          const DWFString& zSourceProductVendor     = "",
                          const DWFString& zSourceProductName       = "",
                          const DWFString& zSourceProductVersion    = "",
                          const DWFString& zPublisherProductVendor  = "",
                          const DWFString& zPublisherProductVersion = "",
                          bool             bNoPasswordSalting       = true )
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWF6PackagePublisher()
        throw();

    ///
    ///         The will salt any provided password to prevent usage of zip password recovery tools. Note: this will
    ///         make the DWF package non-PKZIP compliant.
    ///
    ///\param   bEnable                 Set this to true to enable salting.
    ///\throw   None
    ///
    _DWFTK_API
    void enablePasswordSalting( bool bEnable )
        throw();

    ///
    ///         Adds a request that a signature be generated when the DWF package is created.
    ///         This pointer will be deleted with the \b DWFCORE_FREE_OBJECT macro.
    ///
    ///\param   pSignatureRequest       A class describing the signature that is being requested.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void addSignatureRequest( DWFSignatureRequest* pSignatureRequest )
        throw( DWFException );

protected:

    virtual DWFPackageWriter* getPackageWriter( )
        throw( DWFException )
    {
        return &_oPackageWriter;
    }
	
	virtual DWFGraphicResource* getGraphicResource()
		throw( DWFException )
	{
		return _pGraphicResource;
	}

	virtual void postprocess( DWFPublishable* pContent )
        throw( DWFException );
	
	virtual void preprocess( DWFPublishable* pContent )
		throw( DWFException );

    virtual void postprocessPlot( DWFPlot* pW2DPlot )
        throw( DWFException );

	virtual void postprocessModel( DWFModel* pW3DModel )
        throw( DWFException );

private:

    DWF6PackageWriter                   _oPackageWriter;
	DWFGraphicResource*					_pGraphicResource;

private:

    DWF6PackagePublisher( const DWF6PackagePublisher& );
    DWF6PackagePublisher& operator=( const DWF6PackagePublisher& );
};

}



#endif
#endif

