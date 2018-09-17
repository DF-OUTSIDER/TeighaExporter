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

#ifndef _DWFTK_SEGMENT_H
#define _DWFTK_SEGMENT_H


///
///\file        dwf/publisher/model/Segment.h
///\brief       This file contains the DWFSegment class declaration.
///

#ifndef DWFTK_READ_ONLY


#include "dwfcore/String.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/w3dtk/BStream.h"
#include "dwf/w3dtk/BOpcodeShell.h"
#include "dwf/w3dtk/BOpcodeHandler.h"
#include "dwf/publisher/PublishedObject.h"
#include "dwf/publisher/model/AttributeHandlerBuilder.h"
#include "dwf/publisher/model/GeometryHandlerBuilder.h"
#include "dwf/publisher/model/FeatureHandlerBuilder.h"
#include "dwf/publisher/model/SegmentHandlerBuilder.h"



namespace DWFToolkit
{

//
// fwd decl
//
class DWFStyleSegment;
class DWFIncludeSegment;


///
///\ingroup         dwfpublish3d
///
///\class           DWFSegment     dwf/publisher/model/Segment.h    "dwf/publisher/model/Segment.h"
///\brief           Represents a unit in the scene to which geometry and/or attributes can be applied.
///\since           7.0.1
///
class DWFSegment : public DWFAttributeHandlerBuilder
                 , public DWFGeometryHandlerBuilder
                 , public DWFFeatureHandlerBuilder
                 , public DWFPropertyContainer
                 _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{


public:

    ///
    ///\brief       Indicates the segment represents some object of interest
    ///             that may have metadata and scope outside of the 3d graphic stream itself.
    ///
    static const char* const kz_User_Options_Node;
    ///
    ///\brief       Indicates the segment contains explicitly defined feature geometry.
    ///
    static const char* const kz_User_Options_Edges;  

    ///
    ///\brief       Indicates the segment represents some object of interest
    ///             that may have metadata and scope outside of the 3d graphic stream itself;
    ///             and additionally contains explicitly defined feature edge geometry.
    ///
    static const char* const kz_User_Options_Hidden;
    static const char* const kz_User_Options_Transparent;

    static const char* const kz_User_Options_Hidden_Node;
    static const char* const kz_User_Options_Transparent_Node;
    
    static const char* const kz_User_Options_Node_and_Edges;
    static const char* const kz_User_Options_Hidden_Node_and_Edges;
    static const char* const kz_User_Options_Transparent_Node_and_Edges;

    ///
    ///\brief       Defines the data type used for segment keys.
    ///
    typedef DWFPublishedObject::tKey        tKey;

    //
    // 
    //
    typedef enum tePublishedState
    {
        eDefault,

        eHidden,
        eTransparent

    } tePublishedState;

public:

    ///
    ///             Constructor
    ///
    ///\param       rSegmentBuilder     An interface for acquiring the segment-specific op-code handlers.
    ///\param       rGeometryBuilder    An interface for acquiring the geometry-specific op-code handlers.
    ///\param       rFeatureBuilder     An interface for acquiring the feature-specific op-code handlers.
    ///\param       rAttributeBuilder   An interface for acquiring the attribute-specific op-code handlers.
    ///\param       rObjectFactory      An interface for building published objects.
    ///\param       pPublishedParent    An optional object defining a structural relationship between
    ///                                 itself and the published object that will be created for this segment.
    ///\param       bUseOldObjectModel  If true, rules corresponding to the old object definition model will
    ///                                 be applied to test for validity during the model creation process.
    ///\throw       None
    ///
    _DWFTK_API
    DWFSegment( DWFSegmentHandlerBuilder&     rSegmentBuilder,
                DWFGeometryHandlerBuilder&    rGeometryBuilder,
                DWFFeatureHandlerBuilder&     rFeatureBuilder,
                DWFAttributeHandlerBuilder&   rAttributeBuilder,
                DWFPublishedObject::Factory&  rObjectFactory,
                DWFPublishedObject*           pPublishedParent = NULL,
                bool                          bUseOldObjectModel = false )
        throw();

    ///
    ///             Copy Constructor
    ///
    ///\param       rSegment    The segment to clone.
    ///\throw       DWFException
    ///
    _DWFTK_API
    DWFSegment( const DWFSegment& rSegment )
        throw( DWFException );

    ///
    ///             Assignment Operator
    ///
    ///\param       rSegment    The segment to clone.
    ///\throw       DWFException
    ///
    _DWFTK_API
    DWFSegment& operator=( const DWFSegment& rSegment )
        throw( DWFException );

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFTK_API
    virtual ~DWFSegment()
        throw();

    ///
    ///             Returns a unique identifier for the segment.
    ///
    ///\return      The segment key.  
    ///\throw       None
    ///
    _DWFTK_API
    tKey key() const
        throw();

    ///
    ///             Opens the segment for use.
    ///
    ///             The usage model for segments is:
    ///             -# Create or obtain a segment object.
    ///             -# Open the segment.
    ///             -# Add geometry, attributes, sub-segments, etc.
    ///             -# Close the segment.
    ///             -# Repeat. (The same segment object can be reused.)
    ///
    ///\param       zName               An optional name for the segment.
    ///                                 If a segment is named it will be marked as a node
    ///                                 and captured in the package metadata as a defined object and/or instance.
    ///
    ///\param       bPublishedEdges     Set this flag to \e true if feature edge geometry
    ///                                 will be added to this segment.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void open( const DWFString* zName = NULL,
                       bool             bPublishedEdges = false )
        throw( DWFException );

    ///
    ///             Opens the segment for use.
    ///
    ///             The usage model for segments is:
    ///             -# Create or obtain a segment object.
    ///             -# Open the segment.
    ///             -# Add geometry, attributes, sub-segments, etc.
    ///             -# Close the segment.
    ///             -# Repeat. (The same segment object can be reused.)
    ///
    ///\param       zName               An optional name for the segment.
    ///                                 If a segment is named it will be marked as a node
    ///                                 and captured in the package metadata as a defined object and/or instance.
    ///
    ///\param       bPublishedEdges     Set this flag to \e true if feature edge geometry
    ///                                 will be added to this segment.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void open( const DWFString& zName,
                       bool             bPublishedEdges = false )
        throw( DWFException );

    ///
    ///             Closes the segment completely and serializes any remaining
    ///             data and op-codes into the W3D stream.
    ///
    ///             This segment object will be completely cleaned out and
    ///             is available for reuse after this method returns.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void close()
        throw( DWFException );

    ///
    ///             Creates a subsegment in the scene.
    ///         
    ///             Despite this method name, the subsegment object
    ///             must still be explicitly opened as it follows the 
    ///             general workflow detailed in \a open().
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    DWFSegment openSegment()
        throw( DWFException );

    ///
    ///             Applies a style (via a styling segment).
    ///
    ///             Styling segments are bundles of attributes that can be
    ///             applied as a whole in the scene to affect visualization.
    ///
    ///\param       rSegment    The style to apply.
    ///\throw       DWFException
    ///
    _DWFTK_API
    void style( DWFStyleSegment& rSegment )
        throw( DWFException );

    ///
    ///             Capture a reference to a segment from the include library.
    ///
    ///             Use this method for reusing geometry and instancing objects and composites.
    ///
    ///             Since all segments in the include library currently must be named,
    ///             including such a segment will create parent-child relationship in the
    ///             meta-structure if this segment is <b>un</b>named and the metadata is set to
    ///             be generated. See DWFIncludeSegment and \ref dwfpublish3d page for more details.
    ///
    ///\param       rSegment                The segment to include.
    ///\param       bGenerateMetadata       If true, the associated metadata, e.g., objects and entities will
    ///                                     be generated. Otherwise only the geometry will be included.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void include( DWFIncludeSegment& rSegment,
                          bool bGenerateMetadata = true )
        throw( DWFException );

    ///
    ///             Capture a reference to a segment from the include library.
    ///
    ///             Use this method for reusing geometry and instancing objects and composites.
    ///
    ///             Since all segments in the include library currently must be named,
    ///             including such a segment will create parent-child relationship in the
    ///             meta-structure if this segment is <b>un</b>named. See DWFIncludeSegment
    ///             and \ref dwfpublish3d page for more details.
    ///
    ///\param       rSegment                The segment to include.
    ///\param       rInstanceName           Changes the name of the relationship in the meta-structure
    ///                                     (from the name of the include segment.)
    ///\throw       DWFException
    ///
    _DWFTK_API
    void include( DWFIncludeSegment& rSegment,
                  const DWFString& rInstanceName )
        throw( DWFException );

    ///
    ///             Capture a reference to a segment from the include library.
    ///
    ///             Use this method for reusing geometry and instancing objects and composites.
    ///
    ///             Since all segments in the include library currently must be named,
    ///             including such a segment will create parent-child relationship in the
    ///             meta-structure if this segment is <b>un</b>named and the metadata is set to
    ///             be generated. See DWFIncludeSegment and \ref dwfpublish3d page for more details.
    ///
    ///\param       nSegmentKey             The key of the segment to include.
    ///\param       bGenerateMetadata       If true, the associated metadata, e.g., objects and entities will
    ///                                     be generated. Otherwise only the geometry will be included.
    ///\throw       DWFException
    ///
    _DWFTK_API
    virtual void include( tKey nSegmentKey,
                          bool bGenerateMetadata = true )
        throw( DWFException );

    ///
    ///             Capture a reference to a segment from the include library.
    ///
    ///             Use this method for reusing geometry and instancing objects and composites.
    ///
    ///             Since all segments in the include library currently must be named,
    ///             including such a segment will create parent-child relationship in the
    ///             meta-structure if this segment is <b>un</b>named. See DWFIncludeSegment
    ///             and \ref dwfpublish3d page for more details.
    ///
    ///\param       nSegmentKey             The key of the segment to include.
    ///\param       rInstanceName           Changes the name of the relationship in the meta-structure
    ///                                     (from the name of the include segment.)
    ///\throw       DWFException
    ///
    _DWFTK_API
    void include( tKey             nSegmentKey,
                  const DWFString& rInstanceName )
        throw( DWFException );

    _DWFTK_API
    void hideFromDefaultModel()
        throw( DWFException );

    ///
    ///             Sets the visibility for this segment
    ///
    ///             This sets the visibility for this segmnt and all of its subsegments
    ///             unless overriden by a subsegment.
    ///
    ///             All segments are visible by default
    ///
    ///\param       bVisible      The visibility of this segment
    ///\throw       DWFException
    ///
    ///\since       7.2.0
    ///
    _DWFTK_API
    void setVisibility( bool bVisible )
        throw( DWFException );

    ///
    ///             When generating an object for this segment, set the object's entity to the one provided,
    ///             instead of generating one. 
    ///             This call is useful if you directly create an entity directly in the content, or if
    ///             you want to assign an entity generated in an earlier model, to the current segment.
    ///
    ///             \b Note: Any of the following scenarios will throw an exception.
    ///             1) This is called after include has be called on this segment.
    ///             2) This is called after setEntity has already been called on this segment.
    ///             3) This is called after setObject has been called on this segment.
    ///             4) include is called after calling this ( the exception is thrown by include ).
    ///             5) setObject is called after calling this ( the exception is thrown by setObject ).
    ///
    ///             Essentially the calls include, setObject and setEntity are mutually exclusive.
    ///
    ///\param   pEntity         The DWFEntity which will be associated with this segment.
    ///\param   pzInstanceName  An optional name for the generated content object. Maybe NULL.
    ///\throw   DWFException
    ///
    ///\since       7.2.0
    ///
    _DWFTK_API
    virtual void setContentEntity( DWFEntity* pEntity, 
                                   const DWFString* pzInstanceName = NULL )
        throw( DWFException );

    ///
    ///             Instead of generating an object for this segment, set the provided object as the content
    ///             object associated with the segment.
    ///             This call is useful if you directly create an object in the content, or if you want to
    ///             assign an object generated in an earlier model to the current segment. This is very 
    ///             important if you want to have an object model that spans multiple graphical renditions
    ///             of the model.
    /// 
    ///             Settting this on a named segment will cause the name of the segment to be ignored. It will
    ///             replaced by the object name. 
    ///
    ///             \b Note: Any of the following scenarios will throw an exception:
    ///             1) If the parent segment of this segment has an associated object that is not the 
    ///                parent of the provided object.
    ///             2) This is called after include has be called on this segment.
    ///             3) This is called after setEntity has been called on this segment.
    ///             4) This is called after setObject has been already called on this segment.
    ///             5) include is called after calling this ( the exception is thrown by include ).
    ///             6) setEntity is called after calling this ( the exception is thrown by setEntity ).
    ///
    ///             Essentially the calls include, setObject and setEntity are mutually exclusive.
    ///
    ///\param       pObject         The DWFObject which will be associated with this segment.
    ///\throw       DWFException
    ///
    ///\since       7.2.0
    ///
    _DWFTK_API
    virtual void setContentObject( DWFObject* pObject )
        throw( DWFException );

    _DWFTK_API
    virtual void setUsingOldObjectModel( bool bUse = true )
        throw();

public:

    ///
    ///\copydoc DWFPropertyContainer::findProperty
    ///
    _DWFTK_API
    virtual const DWFProperty* const findProperty( const DWFString& zName,
                                                   const DWFString& zCategory = /*NOXLATE*/L"" ) const
        throw();

    ///
    ///\copydoc DWFPropertyContainer::getProperties
    ///
    _DWFTK_API
    virtual DWFProperty::tMap::Iterator* getProperties( const DWFString& zCategory = /*NOXLATE*/L"" )
        throw();

    ///
    ///         Inserts a property to the container.
    ///
    ///\param   pProperty               The property to add to the container (must not be NULL).
    ///
    ///\throw   DWFException
    ///
   _DWFTK_API
    virtual void addProperty( DWFProperty* pProperty )
        throw( DWFException );

    ///
    ///         Inserts a property to the container.
    ///
    ///\param   rProperty           The property to add to the container.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual void addProperty( const DWFProperty& rProperty )
        throw( DWFException );

    ///
    ///         Insert a property into the container, by directly specifying the 
    ///         property elements. The property will be owned.
    ///
    ///\param   zName       Identifies the property within its \a zCategory.
    ///\param   zValue      The data associated with the property.
    ///\param   zCategory   An optional grouping of properties.
    ///\param   zType       An optional type defining the data.
    ///\param   zUnits      An optional type defining the data.
    ///\return  A pointer to the property created.
    ///\throw   DWFException
    ///
    _DWFTK_API
    virtual DWFProperty* addProperty( const DWFString& zName,
                                      const DWFString& zValue,
                                      const DWFString& zCategory = /*NOXLATE*/L"",
                                      const DWFString& zType = /*NOXLATE*/L"",
                                      const DWFString& zUnits = /*NOXLATE*/L"" )
          throw( DWFException );

    ///
    ///\copydoc DWFPropertyContainer::addPropertyContainer
    ///
    _DWFTK_API
    virtual void addPropertyContainer( DWFPropertyContainer* pContainer )
        throw( DWFException );
    
    ///
    ///\copydoc DWFPropertyContainer::referencePropertyContainer
    ///
    _DWFTK_API
    virtual void referencePropertyContainer( const DWFPropertyContainer& rContainer )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertyContainer::removeOwnedPropertyContainers
    ///
    _DWFTK_API
    virtual void removeOwnedPropertyContainers( DWFPropertyContainer::tList& rContainerList,
                                                bool                         bMakeReferences = false )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertyContainer::removeReferencedPropertyContainers
    ///
    _DWFTK_API
    virtual void removeReferencedPropertyContainers( DWFPropertyContainer::tList& rContainerList )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertyContainer::removeAllPropertyContainers
    ///
    _DWFTK_API
    virtual void removeAllPropertyContainers( DWFPropertyContainer::tList& rContainerList )
        throw( DWFException );

    ///
    ///\copydoc DWFPropertyContainer::copyProperties
    ///
    _DWFTK_API
    virtual void copyProperties( DWFPropertyContainer& rContainer,
                                 bool                  bRemoveOwnership = false,
                                 bool                  bMakeReferences = false)
        throw( DWFException );

public:

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getAreaLightHandler()
    ///
    _DWFTK_API
    TK_Area_Light& getAreaLightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getCircleHandler()
    ///
    _DWFTK_API
    TK_Circle& getCircleHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getCircularArcHandler
    ///
    _DWFTK_API
    TK_Circle& getCircularArcHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getCircularChordHandler
    ///
    _DWFTK_API
    TK_Circle& getCircularChordHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getCircularWedgeHandler
    ///
    _DWFTK_API
    TK_Circle& getCircularWedgeHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getClipRectangleHandler
    ///
    _DWFTK_API
    TK_Clip_Rectangle& getClipRectangleHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getClipRegionHandler
    ///
    _DWFTK_API
    TK_Clip_Region& getClipRegionHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getCuttingPlaneHandler
    ///
    _DWFTK_API
    TK_Cutting_Plane& getCuttingPlaneHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getCylinderHandler
    ///
    _DWFTK_API
    TK_Cylinder& getCylinderHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getDistantLightHandler
    ///
    _DWFTK_API
    TK_Point& getDistantLightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getEllipseHandler
    ///
    _DWFTK_API
    TK_Ellipse& getEllipseHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getEllipticalArcHandler
    ///
    _DWFTK_API
    TK_Ellipse& getEllipticalArcHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getGlyphHandler
    ///
    _DWFTK_API
    TK_Glyph_Definition& getGlyphDefinitionHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getGridHandler
    ///
    _DWFTK_API
    TK_Grid& getGridHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getImageHandler
    ///
    _DWFTK_API
    TK_Image& getImageHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getLineHandler
    ///
    _DWFTK_API
    TK_Line& getLineHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getLocalLightHandler
    ///
    _DWFTK_API
    TK_Point& getLocalLightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getMarkerHandler
    ///
    _DWFTK_API
    TK_Point& getMarkerHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getMeshHandler
    ///
    _DWFTK_API
    TK_Mesh& getMeshHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getNURBSCurveHandler
    ///
    _DWFTK_API
    TK_NURBS_Curve& getNURBSCurveHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getNURBSSurfaceHandler
    ///
    _DWFTK_API
    TK_NURBS_Surface& getNURBSSurfaceHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getPolyCylinderHandler
    ///
    _DWFTK_API
    TK_PolyCylinder& getPolyCylinderHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getPolygonHandler
    ///
    _DWFTK_API
    TK_Polypoint& getPolygonHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getPolylineHandler
    ///
    _DWFTK_API
    TK_Polypoint& getPolylineHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getShellHandler
    ///
    _DWFTK_API
    TK_Shell& getShellHandler( bool bTriStripsOnly = false,
                               bool bDisableOptimization = false )
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getSphereHandler
    ///
    _DWFTK_API
    TK_Sphere& getSphereHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getSpotLightHandler
    ///
    _DWFTK_API
    TK_Spot_Light& getSpotLightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getTextHandler
    ///
    _DWFTK_API
    TK_Text& getTextHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFGeometryHandlerBuilder::getTextWithEncodingHandler
    ///
    _DWFTK_API
    TK_Text& getTextWithEncodingHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFFeatureHandlerBuilder::getTextureHandler
    ///
    _DWFTK_API
    TK_Texture& getTextureHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFFeatureHandlerBuilder::getTextureMatrixHandler
    ///
    _DWFTK_API
    TK_Matrix& getTextureMatrixHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getCameraHandler
    ///
    _DWFTK_API
    TK_Camera& getCameraHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getColorHandler
    ///
    _DWFTK_API
    TK_Color& getColorHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getColorMapHandler
    ///
    _DWFTK_API
    TK_Color_Map& getColorMapHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getColorRGBHandler
    ///
    _DWFTK_API
    TK_Color_RGB& getColorRGBHandler()
        throw( DWFException );

    _DWFTK_API
    TK_Named& getEdgePatternHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getEdgeWeightHandler
    ///
    _DWFTK_API
    TK_Size& getEdgeWeightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getFacePatternHandler
    ///
    _DWFTK_API
    TK_Enumerated& getFacePatternHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getHandednessHandler
    ///
    _DWFTK_API
    TK_Enumerated& getHandednessHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getHeuristicsHandler
    ///
    _DWFTK_API
    TK_Heuristics& getHeuristicsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getLinePatternHandler
    ///
    _DWFTK_API
    TK_Named& getLinePatternHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getLineStyleHandler
    ///
    _DWFTK_API
    TK_Line_Style& getLineStyleHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getLineWeightHandler
    ///
    _DWFTK_API
    TK_Size& getLineWeightHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getMarkerSizeHandler
    ///
    _DWFTK_API
    TK_Size& getMarkerSizeHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getMarkerSymbolHandler
    ///
    _DWFTK_API
    TK_Enumerated& getMarkerSymbolHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getModellingMatrixHandler
    ///
    _DWFTK_API
    TK_Matrix& getModellingMatrixHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getRenderingOptionsHandler
    ///
    _DWFTK_API
    TK_Rendering_Options& getRenderingOptionsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getSelectabilityHandler
    ///
    _DWFTK_API
    TK_Selectability& getSelectabilityHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextAlignmentHandler
    ///
    _DWFTK_API
    TK_Enumerated& getTextAlignmentHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextFontHandler
    ///
    _DWFTK_API
    TK_Text_Font& getTextFontHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextPathHandler
    ///
    _DWFTK_API
    TK_Point& getTextPathHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getTextSpacingHandler
    ///
    _DWFTK_API
    TK_Size& getTextSpacingHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getUserOptionsHandler
    ///
    _DWFTK_API
    TK_User_Options& getUserOptionsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getUnicodeOptionsHandler
    ///
    _DWFTK_API
    TK_Unicode_Options& getUnicodeOptionsHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getVisibilityHandler
    ///
    _DWFTK_API
    TK_Visibility& getVisibilityHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getWindowHandler
    ///
    _DWFTK_API
    TK_Window& getWindowHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getWindowHandler
    ///
    _DWFTK_API
    TK_Enumerated& getWindowFrameHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::getWindowHandler
    ///
    _DWFTK_API
    TK_Enumerated& getWindowPatternHandler()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::openLocalLightAttributes
    ///
    _DWFTK_API
    void openLocalLightAttributes()
        throw( DWFException );

    ///
    ///\copydoc DWFAttributeHandlerBuilder::closeLocalLightAttributes
    ///
    _DWFTK_API
    void closeLocalLightAttributes()
        throw( DWFException );

private:

    //
    //  When including a segment, use this to serialize the segment into the 
    //  hoops stream. This is called from the include menthods.
    //
    _DWFTK_API
    void _serializeIncludeSegment( DWFPublishedObject* pIncludeObject )
        throw( DWFException );

    //
    //  When including a segment, use this to update the associated published
    //  objects to ensure the reference information is gathered by the published
    //  object for metadata generation later.
    //
    _DWFTK_API
    void _updatePublishedObjects( DWFPublishedObject* pPubObject,
                                  const DWFString* pInstanceName )
        throw( DWFException );

private:

    //
    //  Use this enum to set the flag signifying which call was made on the segment
    //  This needs to be declared before the _testContentIncludeIsAllowed method
    //
    typedef enum _teContentIncludeCalls
    {
        eNoContentIncludeCalls  = 0,
        eIncludeCall            = 1,
        eSetEntityCall          = 2,
        eSetObjectCall          = 3

    } _teContentIncludeCalls;

private:

    //
    //  This tests if any of the three mutually exclusive methods have been 
    //  called already, and if so throws an appropriate exception. Otherwise
    //  it sets the flag for subsequent tests.
    //
    _DWFTK_API
    void _testContentIncludeIsAllowed( _teContentIncludeCalls eCallType )
        throw( DWFException );

protected:

    DWFSegmentHandlerBuilder&       _rSegmentBuilder;
    DWFGeometryHandlerBuilder&      _rGeometryBuilder;
    DWFFeatureHandlerBuilder&       _rFeatureBuilder;
    DWFAttributeHandlerBuilder&     _rAttributeBuilder;
    DWFPublishedObject::Factory&    _rObjectFactory;
    
    DWFPublishedObject*             _pPublishedObject;
    DWFPublishedObject*             _pPublishedParent;

    DWFString*                      _pLibrary;


private:

    bool                            _bOpen;

    bool                            _bUsingOldObjectModel;

    //
    //  This is a bit mask the points out which of the three methods,
    //  include, setEntity and setObject have been applied on the
    //  segment. Since the calls are mutually exclusive, if any one
    //  the is called, this flag should become non-zero, indicating
    //  that further calls are not allowed.
    //
    _teContentIncludeCalls          _eContentIncludeFlag;

    DWFPublishedObject::tKey        _nKey;
    static unsigned int             _knNextKey;

private:

    //
    // Not Implemented
    //

    DWFSegment();
};



    //
    // required for win32 dll external linkage
    //
#ifdef  _DWFCORE_WIN32_SYSTEM
#ifndef DWFTK_STATIC

//DNT_Start
_declspec(selectany) const char* const DWFSegment::kz_User_Options_Node                         = "node";
_declspec(selectany) const char* const DWFSegment::kz_User_Options_Edges                        = "edges";
_declspec(selectany) const char* const DWFSegment::kz_User_Options_Hidden                       = "hidden";
_declspec(selectany) const char* const DWFSegment::kz_User_Options_Transparent                  = "transparent";

_declspec(selectany) const char* const DWFSegment::kz_User_Options_Hidden_Node                  = "node,hidden";
_declspec(selectany) const char* const DWFSegment::kz_User_Options_Transparent_Node             = "node,transparent";

_declspec(selectany) const char* const DWFSegment::kz_User_Options_Node_and_Edges               = "node,edges";
_declspec(selectany) const char* const DWFSegment::kz_User_Options_Hidden_Node_and_Edges        = "node,edges,hidden";
_declspec(selectany) const char* const DWFSegment::kz_User_Options_Transparent_Node_and_Edges   = "node,edges,transparent";
//DNT_End

#endif
#endif


}


#endif  
#endif

