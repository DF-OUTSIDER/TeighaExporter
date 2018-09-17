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

#ifndef _DWFTK_GEOMETRY_HANDLER_BUILDER_H
#define _DWFTK_GEOMETRY_HANDLER_BUILDER_H

///
///\file        dwf/publisher/model/GeometryHandlerBuilder.h
///\brief       This file contains the DWFGeometryHandlerBuilder class declaration.
///


#include "dwfcore/Exception.h"
#include "dwf/w3dtk/BOpcodeShell.h"
#include "dwf/w3dtk/BOpcodeHandler.h"


namespace DWFToolkit
{

///
///\ingroup         dwfpublish3d
///
///\class           DWFGeometryHandlerBuilder     dwf/publisher/model/GeometryHandlerBuilder.h    "dwf/publisher/model/GeometryHandlerBuilder.h"
///\brief           Base implementation for a W3D scene geometry op-code handler provider.
///\since           7.0.1
///
///                 This class should be specialized by classes that can provide W3D op-code handlers for scene geometry.
///
class DWFGeometryHandlerBuilder _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE
{

public:

    ///
    ///             Constructor
    ///
    ///\throw       None
    ///
    DWFGeometryHandlerBuilder()
        throw()
    {;}

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    virtual ~DWFGeometryHandlerBuilder()
        throw()
    {;}

    ///
    ///             Copy Constructor
    ///
    ///\throw       None
    ///
    DWFGeometryHandlerBuilder( const DWFGeometryHandlerBuilder& )
        throw()
    {;}

    ///
    ///             Assignment Operator
    ///
    ///\throw       None
    ///
    DWFGeometryHandlerBuilder& operator=( const DWFGeometryHandlerBuilder& )
        throw()
    {
        return *this;
    }

public:

    ///
    ///\ingroup     w3dtkhlight
    ///
    ///             Returns a handler for area light geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Area_Light& getAreaLightHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for circle geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Circle& getCircleHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for circular arc geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Circle& getCircularArcHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for circular chord geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Circle& getCircularChordHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for circular wedge geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Circle& getCircularWedgeHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for clip rectangle geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Clip_Rectangle& getClipRectangleHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for clip region geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Clip_Region& getClipRegionHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for cutting plane geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Cutting_Plane& getCuttingPlaneHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for cylinder geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///\since       7.2
    ///
    virtual TK_Cylinder& getCylinderHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhlight
    ///
    ///             Returns a handler for distant light geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Point& getDistantLightHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for ellipse geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Ellipse& getEllipseHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for elliptical arc geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Ellipse& getEllipticalArcHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for glyph definition
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Glyph_Definition& getGlyphDefinitionHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for grid geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///\since       7.2
    ///
    virtual TK_Grid& getGridHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhimage
    ///
    ///             Returns a handler for image geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Image& getImageHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for line geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Line& getLineHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhlight
    ///
    ///             Returns a handler for local light geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Point& getLocalLightHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for marker geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///\since       7.2
    ///
    virtual TK_Point& getMarkerHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for mesh geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///\since       7.2
    ///
    virtual TK_Mesh& getMeshHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for NURBS curve geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_NURBS_Curve& getNURBSCurveHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for NURBS surface geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_NURBS_Surface& getNURBSSurfaceHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for polycylinder geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_PolyCylinder& getPolyCylinderHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for polygon geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Polypoint& getPolygonHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for polyline geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Polypoint& getPolylineHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for shell geometry.
    ///
    ///\param       bTriStripsOnly          The shell is assumed to contain triangles only and may not contain holes, 
    ///                                     and the "faces" are actually the way that vertices are connected into triangle strips. 
    ///                                     Start with the length of the strip, followed by the vertex indices to be connected. 
    ///                                     The first 3 vertices form a triangle, and every additional vertex is 
    ///                                     combined with the two previous ones to  define one additional triangle. 
    ///                                     Exactly as with OpenGL's GL_TRIANGLE_STRIP primitive, 
    ///                                     the orientation of every even triangle is reversed, beginning with the second.
    ///                                     <b>IMPORTANT: Setting this flag to true when your data is not in tristrips will produce garbage results.</b>
    ///
    ///\param       bDisableOptimization    Disable vertex and face optimization algorithm normally applied before stream serialization.
    ///                                     This optimization is on by default, it's tolerance parameters may be tuned
    ///                                     by calling \a optimize() (this method can also be used to disable the process).
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Shell& getShellHandler( bool /*bTriStripsOnly = false*/,
                                       bool /*bDisableOptimization = false*/ )
    throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///             Returns a handler for sphere geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///\since       7.2
    ///
    virtual TK_Sphere& getSphereHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhlight
    ///
    ///             Returns a handler for spot light geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Spot_Light& getSpotLightHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhtextf
    ///
    ///             Returns a handler for text geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Text& getTextHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }

    ///
    ///\ingroup     w3dtkhtextf
    ///
    ///             Returns a handler for encoded text geometry.
    ///
    ///\return      A reference to an op-code handler.
    ///\throw       DWFException
    ///
    virtual TK_Text& getTextWithEncodingHandler()
        throw( DWFException )
    {
        _DWFCORE_THROW( DWFNotImplementedException, /*NOXLATE*/L"Handler not available in this builder" );
    }
};


}


#endif
