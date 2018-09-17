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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/publisher/impl/DWFPackagePublisher.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//

#ifndef _DWFTK_DWF_PACKAGE_PUBLISHER_H
#define _DWFTK_DWF_PACKAGE_PUBLISHER_H


///
///\file        dwf/publisher/impl/DWFPackagePublisher.h
///\brief       This file contains the DWFPackagePublisher class declaration.
///


#ifndef DWFTK_READ_ONLY

#include "dwfcore/File.h"

using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/package/Section.h"
#include "dwf/package/writer/PackageWriter.h"
#include "dwf/publisher/Image.h"
#include "dwf/publisher/Publisher.h"
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
///\class       DWFPackagePublisher   dwf/publisher/impl/DWFPackagePublisher.h   "dwf/publisher/impl/DWFPackagePublisher.h"
///\brief       Base class for publisher implementations that produce DWF files
///\since       7.0.1
///
///             Due to the composite nature of the the visitors, even this class can be used to extract
///             additional data to other output pipes during the creation of the DWF package.
///
///\todo        Implement a full version for composite DWF packages.
///
class DWFPackagePublisher : public DWFPublisher
                          , public DWFEmbeddedFontVisitor
                          _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{

public:

    ///
    ///\brief   The name of the default model view (see DWFModel::createView).
    ///
    static const wchar_t* const kz_NamedView_Default;
    ///
    ///\brief   The name of the DWFProperty that captures the handedness heuristic (see DWFModel::getPolygonHandedness)
    ///
    static const wchar_t* const kz_PropName_PolygonHandedness;
    ///
    ///\brief   The name of the DWFProperty that captures the lighting preference (see DWFModel::useModelLighting)
    ///
    static const wchar_t* const kz_PropName_UseDefaultLighting;
    ///
    ///\brief   The name of the DWFProperty that captures the feature edge preference (see DWFModel::useSilhouetteEdges)
    ///
    static const wchar_t* const kz_PropName_UseSilhouetteEdges;
    ///
    ///\brief   The name of the DWFProperty that captures the edge color preference (see DWFModel::getEdgeColor)
    ///
    static const wchar_t* const kz_PropName_EdgeColor;
    ///
    ///\brief   The name of the DWFProperty that captures the initial display mode (see DWFModel::getDisplayMode)
    ///
    static const wchar_t* const kz_PropName_DisplayMode;

    ///
    ///\brief   The name of the DWFPropertyContainer that bundles the default (initial) model view.
    ///
    static const wchar_t* const kz_PropSetName_DefaultView;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera projection.
    ///
    static const wchar_t* const kz_PropName_CameraProjection;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera position.
    ///
    static const wchar_t* const kz_PropName_CameraPosition;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera target.
    ///
    static const wchar_t* const kz_PropName_CameraTarget;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera up vector.
    ///
    static const wchar_t* const kz_PropName_CameraUpVector;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera field.
    ///
    static const wchar_t* const kz_PropName_CameraField;

    ///
    ///\brief   The name of the DWFProperty that decribes the default state (show/hide) of compass.
    ///
    static const wchar_t* const kz_PropName_ViewCubeShowCompass;
    ///
    ///\brief   The name of the DWFProperty that decribes the default angle of north.
    ///
    static const wchar_t* const kz_PropName_ViewCubeAngleOfNorth;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera projection.
    ///
    static const wchar_t* const kz_PropName_ViewCubeHomeCameraProjection;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera position.
    ///
    static const wchar_t* const kz_PropName_ViewCubeHomeCameraPosition;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera target.
    ///
    static const wchar_t* const kz_PropName_ViewCubeHomeCameraTarget;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera up vector.
    ///
    static const wchar_t* const kz_PropName_ViewCubeHomeCameraUpVector;
    ///
    ///\brief   The name of the DWFProperty that decribes the default camera field.
    ///
    static const wchar_t* const kz_PropName_ViewCubeHomeCameraField;
	///
    ///\brief   The name of the DWFPropertyContainer that set the front view by client.
    ///
	static const wchar_t* const kz_PropName_FrontView;


	static const wchar_t* const kz_PropName_FrontViewCameraProjection;
	static const wchar_t* const kz_PropName_FrontViewCameraPosition;
	static const wchar_t* const kz_PropName_FrontViewCameraTarget;
	static const wchar_t* const kz_PropName_FrontViewCameraUpVector;
	static const wchar_t* const kz_PropName_FrontViewCameraField;

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
    ///\throw   DWFException
    ///
    _DWFTK_API
    DWFPackagePublisher( const DWFString& zSourceProductVendor     = "",
                         const DWFString& zSourceProductName       = "",
                         const DWFString& zSourceProductVersion    = "",
                         const DWFString& zPublisherProductVendor  = "",
                         const DWFString& zPublisherProductVersion = "" )
        throw( DWFException );

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    virtual ~DWFPackagePublisher()
        throw();

    ///
    ///         Set the version to publish to. The default is the current DWF file format version.
    ///         For backwards compatibility one may use the version previous version numbers.
    ///
    ///\param   eVersion    This defines the type of metadata that will be published with the package.
    ///\throw   A DWFUnexpectedException is thrown if the version is not recognized by the publisher.
    ///
    _DWFTK_API
    void setVersion( DWFPublisher::teMetadataVersion eVersion )
        throw( DWFException );

    ///
    ///         Get the version set for publishing.
    ///
    ///\return  The metadata version to which the publisher will publish
    ///\throw   None.
    ///
    DWFPublisher::teMetadataVersion getVersion() const
        throw()
    {
        return _eMetaDataVersion;
    }

    ///
    ///         Compose all of the publishable section content and graphics data
    ///         provided to this object and publish a DWF package file.
    ///
    ///\throw   DWFException
    ///
    _DWFTK_API
    void publish()
        throw( DWFException );

    ///
    ///\copydoc DWFPublisher::getPublishedObjectVisitor()
    ///
    DWFPublishedObject::Visitor* getPublishedObjectVisitor()
        throw( DWFException )
    {
        return _pPublishedObjectVisitor;
    }

    ///
    ///\copydoc DWFPublisher::getPropertyVisitor()
    ///
    DWFPropertyVisitor* getPropertyVisitor()
        throw( DWFException )
    {
        return this->_pPropertyVisitor;
    }

    ///
    ///\copydoc DWFPublisher::getEmbeddedFontVisitor()
    ///
    DWFEmbeddedFontVisitor* getEmbeddedFontVisitor()
        throw( DWFException )
    {
        return this;
    }

    ///
    ///         Returns a visitor that can process a resource.
    ///
    ///\return  A resource visitor.  This pointer must not be deleted by the caller.
    ///\throw   DWFException
    ///
    DWFResourceVisitor* getResourceVisitor()
        throw( DWFException )
    {
        return _pResourceVisitor;
    }

    ///
    ///         Returns the content library to use for metadata publishing.
    ///
    ///\return  Pointer to the primary content of a package.  This pointer must not be
    ///         deleted by the caller.
    ///\throw   DWFException
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    DWFContent* getContent()
        throw( DWFException );

    ///
    ///         If a content manager is created outside the publisher, this method is
    ///         used to attach it to the publisher.
    ///
    ///\param   pContentManager     The content manager to attach.
    ///\param   bTakeOwnership      The ownership of the content manager will be assumed
    ///                             by classes internal to the publisher.
    ///\throw   None
    ///
    ///\since   7.2.0
    ///
    _DWFTK_API
    void attachContentManager( DWFContentManager* pContentManager, bool bTakeOwnership = true )
        throw();

    ///
    ///         This returns the global content presentations container.
    ///         Note the caller should not delete the container.
    ///
    ///\return  Pointer to the content presentations container
    ///\throw   DWFException
    ///
    ///\since   7.3
    ///
    void addPresentation(DWFContentPresentation* pPresentation)
        throw( DWFException )
    {
        return getPackageWriter()->addPresentation(pPresentation);
    }

    ///
    ///         Sets the visitor that can process a resource.
    ///
    ///\param   pVisitor    The resource visitor. This pointer will not
    ///                     be deleted by the publisher.
    ///\throw   None
    ///
    void setResourceVisitor( DWFResourceVisitor* pVisitor )
        throw()
    {
        _pResourceVisitor = pVisitor;
    }

    ///
    ///         Disables the default content navigation presentation.
    ///
    ///\throw   None
    ///
    void disableContentNavigationPresentation()
        throw()
    {
        _bCreateContentNavigationPresentation = false;
    }

    ///
    ///         Sets the label for the content navigation presentation.
    ///
    ///\throw   None
    ///
    void setContentNavigationPresentationLabel( const DWFString& zLabel)
        throw()
    {
        _zContentNavigationPresentationLabel = zLabel;
    }

    ///
    ///         Sets the label for the views presentation.
    ///
    ///\throw   None
    ///
    void setViewsPresentationLabel( const DWFString& zLabel)
        throw()
    {
        _zContentViewsPresentationLabel = zLabel;
    }

    ///
    ///         This passes the call onto the published object visitor owned
    ///         by the DWFPackage publisher. The base publisher's visitor is
    ///         also called to ensure that composite visitors get a chance to
    ///         visit the objects.
    ///
    _DWFTK_API
    void visitPublishedObject( DWFPublishedObject& rObject )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertyVisitor::visitProperty
    ///
    _DWFTK_API
    void visitProperty( DWFProperty& rProperty )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertyVisitor::visitPropertyContainer
    ///
    _DWFTK_API
    void visitPropertyContainer( DWFPropertyContainer& rPropertyContainer )
        throw( DWFException );

    ///
    ///\copydoc DWFEmbeddedFontVisitor::visitEmbeddedFont
    ///
    _DWFTK_API
    void visitEmbeddedFont( const DWFEmbeddedFont& rFont )
        throw( DWFException );

    ///
    ///\copydoc DWFPublisher::preprocess
    ///
    _DWFTK_API
    void preprocess( DWFPublishable* pContent )
        throw( DWFException ) = 0;

    ///
    ///\copydoc DWFPublisher::postprocess
    ///
    _DWFTK_API
    virtual void postprocess( DWFPublishable* pContent )
        throw( DWFException );

protected:
    
    virtual DWFPackageWriter* getPackageWriter( )
        throw( DWFException ) = 0;

	virtual DWFGraphicResource* getGraphicResource( )
		throw( DWFException ) = 0;

	virtual void preprocessSection( DWFPublishableSection* pSection )
        throw( DWFException );

    virtual void postprocessSection( DWFPublishableSection* pSection )
        throw( DWFException );

    virtual void preprocessModel( DWFModel* pW3DModel )
        throw( DWFException );

    virtual void postprocessModel( DWFModel* pW3DModel )
        throw( DWFException );

    virtual void preprocessPlot( DWFPlot* pW2DPlot )
        throw( DWFException );

    virtual void postprocessPlot( DWFPlot* pW2DPlot )
        throw( DWFException );

    virtual void preprocessData( DWFData* pData )
        throw( DWFException );

    virtual void postprocessData( DWFData* pData )
        throw( DWFException );

	DWFSection*                         _pCurrentSection;
	DWFResource*                        _pCurrentGraphicResource;
	size_t		                        _nSections;

private:

	

    void _createDefaultModelNavigationPresentation( DWFModel* pW3DModel,
                                                    DWFSortedVector<DWFInstance*>& oExcluded )
        throw( DWFException );

    void _createDefaultViewsPresentation( DWFModel* pW3DModel )
        throw( DWFException );

private:

	DWFString                           _zSourceProductVendor;
    DWFString                           _zSourceProductName;
    DWFString                           _zSourceProductVersion;
    DWFString                           _zPublisherProductVendor;
    DWFString                           _zPublisherProductVersion;
    DWFEmbeddedFont::tList              _oEmbeddedFonts;
    DWFResourceVisitor*                 _pResourceVisitor;
    

    //
    //  In 7.2.0 the published object visitors are accessed via these pointers.
    //  This replaces the DWFPackagePublisher inheriting from the classes as done
    //  in 7.1.
    //  They are initialized with the appropriate visitor depending on whether
    //  we generate metadata for DWF toolkit 7.2 and higher or metadata compatible
    //  with version 7.1
    //
    DWFPublishedObject::Visitor*        _pPublishedObjectVisitor;
    DWFPropertyVisitor*                 _pPropertyVisitor;

    DWFPublishedContentElementListener* _pPublishedElementListener;

    //
    //  The version determines whether to use the old object definition model using DWFDefinedObjects
    //  or the new object definition model using the DWF content and content elements.
    //
    DWFPublisher::teMetadataVersion     _eMetaDataVersion;

    bool                                _bCreateContentNavigationPresentation;
    DWFString                           _zContentNavigationPresentationLabel;
    DWFString                           _zContentViewsPresentationLabel;

private:

    DWFPackagePublisher( const DWFPackagePublisher& );
    DWFPackagePublisher& operator=( const DWFPackagePublisher& );
};

    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_PolygonHandedness =    L"_PolygonHandedness";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_UseDefaultLighting =   L"_UseDefaultLighting";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_UseSilhouetteEdges =   L"_UseSilhouetteEdges";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_EdgeColor =            L"_EdgeColor";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_DisplayMode =          L"_DisplayMode";

_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_NamedView_Default =             L"Initial";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropSetName_DefaultView =       L"_DefaultView";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_CameraProjection =     L"_CameraProjection";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_CameraPosition =       L"_CameraPosition";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_CameraTarget =         L"_CameraTarget";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_CameraUpVector =       L"_CameraUpVector";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_CameraField =          L"_CameraField";

_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_ViewCubeShowCompass =            L"_ViewCubeShowCompass";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_ViewCubeAngleOfNorth =           L"_ViewCubeAngleOfNorth";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_ViewCubeHomeCameraProjection =   L"_ViewCubeHomeCameraProjection";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_ViewCubeHomeCameraPosition =     L"_ViewCubeHomeCameraPosition";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_ViewCubeHomeCameraTarget =       L"_ViewCubeHomeCameraTarget";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_ViewCubeHomeCameraUpVector =     L"_ViewCubeHomeCameraUpVector";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_ViewCubeHomeCameraField =        L"_ViewCubeHomeCameraField";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_FrontView               =		L"_FrontView";

// front view property
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_FrontViewCameraProjection =   L"_FrontViewCameraProjection";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_FrontViewCameraPosition =     L"_FrontViewCameraPosition";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_FrontViewCameraTarget =       L"_FrontViewCameraTarget";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_FrontViewCameraUpVector =     L"_FrontViewCameraUpVector";
_declspec(selectany) const wchar_t* const DWFPackagePublisher::kz_PropName_FrontViewCameraField =        L"_FrontViewCameraField";
//DNT_End

#endif
#endif

}



#endif
#endif

