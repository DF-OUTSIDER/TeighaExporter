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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/impl/DWFXPackagePublisher.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_DWFX_PACKAGE_PUBLISHER_H
#define _DWFTK_DWFX_PACKAGE_PUBLISHER_H


///
///\file        dwf/publisher/impl/DWFXPackagePublisher.h
///\brief       This file contains the DWFXPackagePublisher class declaration.
///


#ifndef DWFTK_READ_ONLY

#include "dwfcore/File.h"

using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Section.h"
#include "dwf/package/writer/DWFXPackageWriter.h"
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
///\class       DWFXPackagePublisher   dwf/publisher/impl/DWFXPackagePublisher.h   "dwf/publisher/impl/DWFXPackagePublisher.h"
///\brief       A publisher implementation for producing DWFX package files.
///\since       7.4.0
///
///             This is a reference implementation of the DWFPublisher base class for creating DWFX packages.
///
///             Due to the composite nature of the the visitors, even this class can be used to extract
///             additional data to other output pipes during the creation of the DWF package.
///
///\todo        Implement a full version for composite DWF packages.
///
class DWFXPackagePublisher : public DWFPackagePublisher
                           _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:
	
    ///
    ///\brief   These enumerations are used to specify whether proxy
    ///         content, if specified, is added to the package.
    ///         If \a eAlways is specified, proxy content will always be added.
    ///         If \a eIfNoPageOutput is specified, proxy content will only be added if no other FixedPage content would be generated.
    ///         If \a eIfOnlyPartialPageOutput is specified, proxy content only be added if at least one section exists which provides no FixedPage content.
    ///
    typedef enum teProxyGraphicsRulesType
    {
        eAlways                     ,
        eIfNoPageOutput             ,
        eIfOnlyPartialPageOutput
    } teProxyGraphicsRulesType;

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
	///\param	bNoPasswordSalting			If false, the provided password will be salted to prevent password recovery.
	///
	///
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFXPackagePublisher( const DWFFile&   rDWFFile,
						  const DWFString& zPassword                = "",
                          const DWFString& zSourceProductVendor     = "",
                          const DWFString& zSourceProductName       = "",
                          const DWFString& zSourceProductVersion    = "",
                          const DWFString& zPublisherProductVendor  = "",
                          const DWFString& zPublisherProductVersion = "", 
						  bool  bNoPasswordSalting = true  )
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFXPackagePublisher()
        throw();

	///
    ///         This is used to pass proxy graphics content to the
    ///         package publisher for serialization. 
    ///         The provided section should have resources that can
    ///         be serialized such that valid XPS graphics are produced.
    ///
    ///\param   pProxyPlot		The plot containing the proxy content resources.
    ///\param   eRules          Values of type teProxyGraphicsRulesType, specifying in which cases the proxy content should be used.
    ///\param   bAsFirstPage    A boolean specifying whether the proxy graphics should be the first page (true), or the last page (false).
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addProxyGraphicsSection( DWFPlot* pProxyPlot, teProxyGraphicsRulesType eRules=eAlways, bool bAsFirstPage=true )
        throw( DWFException );

	///
    ///         This is used to add default proxy page to the package from a source proxy page XPS file.
    ///         The provided proxy page XPS file should be with a full path name.
    ///
    ///\param   zProxyPageXPSPath		A specified culture proxy page XPS file.
    ///\throw   DWFException
    ///
	_DWFTK_API
    virtual void addProxyPage( const DWFString& zProxyPageXPSPath )
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

    DWFXPackageWriter                   _oPackageWriter;
	DWFGraphicResource*					_pGraphicResource;

private:

    DWFXPackagePublisher( const DWFXPackagePublisher& );
    DWFXPackagePublisher& operator=( const DWFXPackagePublisher& );
	
};

}



#endif
#endif

