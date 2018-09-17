//
//  Copyright (c) 2006 by Autodesk, Inc.
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
//  $Header: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/dwfx/FixedPage.h#1 $
//  $DateTime: 2011/02/14 01:16:30 $
//  $Author: caos $
//  $Change: 197964 $
//  $Revision: #1 $
//
//

#ifndef _DWFTK_DWFXFIXEDPAGE_H
#define _DWFTK_DWFXFIXEDPAGE_H

#include "dwf/xps/FixedPage.h"
#include "dwf/dwfx/ResourcePart.h"
#include "dwf/package/GraphicResource.h"

namespace DWFToolkit
{

//
// fwd declaration
//
class DWFSection;

class DWFXFixedPage : public XPSFixedPage
{

public:

    _DWFTK_API
    DWFXFixedPage( DWFSection* pSection )
        throw(DWFException);

    _DWFTK_API
    virtual ~DWFXFixedPage()
        throw();

    _DWFTK_API
    DWFSection* section() const
        throw()
    {
        return _pSection;
    }

    ///
    ///         Add a graphics resource to the fixed page. If the resource is not compatible 
    ///         with a fixed page, i.e., its MIME-type is not \a DWFMIME::kzMIMEType_FIXEDPAGEXML, 
    ///         and if its role is not one of \a DWFXML::kzRole_Graphics2d, 
    ///         \a DWFXML::kzRole_Graphics2dOverlay or \a DWFXML::kzRole_Graphics2dMarkup, then
    ///         it will not be added. Instead an exception will be thrown.
    ///
    ///\param   pResource   The resource to add.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void addGraphicResource( DWFGraphicResource* pResource )
        throw( DWFException );

    ///
    ///         Remove the provided graphicresource from the fixed page. If the resource is not
    ///         referenced by the fixed page this will throw an exception. This does not delete
    ///         the resource.
    ///
    ///\param   pResource   The resource to remove. 
    ///\throw   DWFException
    ///
    _DWFTK_API
    void removeGraphicResource( DWFGraphicResource* pResource )
        throw( DWFException );

    ///
    ///         Add a resource part referencing a raster overlay or raster markup resource, to the 
    ///         fixed  page. This will add an imagebrush element to the fixed page referencing the part.
    ///         If the resource in the part is not compatible with a fixed page, i.e., its role is not
    ///         \a DWFXML::kzRole_RasterOverlay or \a DWFXML::kzRole_RasterMarkup, and its MIME-type
    ///         is not one of PNG, JPG or TIFF, then it will throw an exception.
    ///
    ///\param   pPart       The part to add.
    ///\throw   DWFException
    ///
    _DWFTK_API
    void addRasterPart( DWFXResourcePart* pPart )
        throw( DWFException );

    ///
    ///         Remove the provided resource part from the fixed page. If the part is not
    ///         referenced by the fixed page this will throw an exception. This does not delete
    ///         the part.
    ///
    ///\param   pResource   The resource to remove. 
    ///\throw   DWFException
    ///
    _DWFTK_API
    void removeRasterPart( DWFXResourcePart* pPart )
        throw( DWFException );

    ///
    ///         Add a required-resource relationship from the fixed page to the resource part
    ///
    ///\param   pPart       The part to add.
    ///\throw   None
    ///
    _DWFTK_API
    void addRequiredResourcePart( DWFXResourcePart* pPart )
        throw( DWFException );

    _DWFTK_API
    void removeRequiredResourcePart( DWFXResourcePart* pPart )
        throw( DWFException );

    ///
    ///         Sets a resource part referencing a thumbnail for
    ///         the fixed page. This will add a thumbnail 
    ///         relationship from the fixed page to this part.
    ///
    ///\param   pPart       The part to add.
    ///\throw   None
    ///
    _DWFTK_API
    void setThumbnailResourcePart( DWFXResourcePart* pPart )
        throw( DWFException );

    _DWFTK_API
    void removeThumbnailResourcePart()
        throw( DWFException );

    _DWFTK_API
    void updateImplicitRelationships()
        throw();

    ///
    ///         Get the graphic resources with mime-type kzMIMEType_FIXEDPAGEXML, 
    ///         used to draw the 2D vector graphics.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None.
    ///
    _DWFTK_API
    DWFGraphicResource::tIterator* graphicResources() const
        throw();

    ///
    ///         Get the parts with image resources used for raster overlays
    ///         and raster markups.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None.
    ///
    _DWFTK_API
    DWFXResourcePart::tIterator* rasterParts() const
        throw();

    ///
    ///         Get the all resources directly reference by the fixed page, namely, the 2D
    ///         vector graphics, or referenced via resource parts for raster overlays and markups.
    ///
    ///\return  A pointer to an iterator that must be deleted by the caller with the \b DWFCORE_FREE_OBJECT macro.
    ///         This pointer may be NULL.
    ///\throw   None.
    ///
    _DWFTK_API
    DWFGraphicResource::tIterator* allResources() const
        throw();
    

#ifndef DWFTK_READ_ONLY

    _DWFTK_API
    virtual void serializeXML( DWFXMLSerializer& rSerializer )
        throw( DWFException );

#endif

private:

    //
    //  Typedef for a map of roles to a list of graphic resources
    //
    typedef _DWFTK_STD_MAP(DWFString, DWFGraphicResource::tList*)   _tRoleResourceListMap;
    //
    //  Typedef for a map of roles to a list of resource parts (for image resources)
    //
    typedef _DWFTK_STD_MAP(DWFString, DWFXResourcePart::tList*)     _tRolePartListMap;
    //
    //  Structure to pair up a resource list map part list map
    //
    typedef struct _tRoleMapPair
    {
        _tRoleResourceListMap   _oResources;
        _tRolePartListMap       _oParts;

        _tRoleMapPair()
            : _oResources()
            , _oParts()
        {
        };
    } _tRoleMapPair;
    //
    //  Typedef for a map from zOrder to a pair of the role maps
    //
    typedef _DWFTK_STD_MAP(int, _tRoleMapPair*)                      _tZOrderMap;

private:

#ifndef DWFTK_READ_ONLY

    //
    //  This is called to serialize the resources in the resource list map,
    //  that have the given role.
    //
    void _serializeGraphicResources( _tRoleResourceListMap& rResources,
                                     const DWFString& zRole, 
                                     DWFXMLSerializer& rSerializer )
        throw();

    //
    //  This is called to serialize the image brushes corresponding to the 
    //  raster image resources in the resource part map, that have the 
    //  given role.
    //
    void _serializeResourceParts( _tRolePartListMap& rParts,
                                  const DWFString& zRole, 
                                  DWFXMLSerializer& rSerializer )
        throw( DWFException );

    void _serializeToCanvas( DWFGraphicResource* pResource, 
                             DWFXMLSerializer& rSerializer )
        throw( DWFException );

    void _serializeToImageBrush( DWFXResourcePart* pPart, 
                                 DWFXMLSerializer& rSerializer )
        throw( DWFException );

    //
    //  This calculates the transform to apply in the XPS FixedPage from the 
    //  transform on the image resource, and also the corresponding viewport
    //  and viewbox to use in the serialization process.
    //
    void _getImagePositionInfo( DWFImageResource* pImage,
                                double* adViewbox, 
                                double* adViewport,
                                double* adXPSTransform )
        throw( DWFException );

    //
    //  This takes the logical to DWF-paper transform from the graphics resource
    //  and converts to a logical to XPS-paper transform.
    void _getGraphicsTransform( DWFGraphicResource* pResource,
                                double* adXPSTransform )
        throw( DWFException );

#endif

private:

    DWFSection*                 _pSection;

    _tRoleResourceListMap       _oRoleToResourceListMap;
    _tRolePartListMap           _oRoleToPartListMap;
    
    _tZOrderMap                 _oZOrderMap;

    DWFXResourcePart*           _pThumbnailResourcePart;

private:

    //
    // Not implemented
    //
    DWFXFixedPage( const DWFXFixedPage& );
    DWFXFixedPage& operator=( const DWFXFixedPage& );

};

}

#endif

