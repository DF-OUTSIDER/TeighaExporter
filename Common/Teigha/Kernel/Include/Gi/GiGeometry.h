/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#ifndef __ODGIGEOMETRY_H__
#define __ODGIGEOMETRY_H__

#include "TD_PackPush.h"
#include "RxObject.h"

#include "Ge/GePoint3d.h"
#include "Ge/GeMatrix3d.h"

#include "Gi/GiRasterImage.h"
#include "Gi/GiMaterial.h"

class OdDbStub;
class OdCmEntityColor;
class OdGeMatrix3d;
class OdGeVector3d;
class OdGePoint2d;
class OdGeCurve2d;
class OdGiPathNode;
class OdGiMapper;
class OdCmTransparency;
class OdGiImageBGRA32;

typedef OdArray<OdGeCurve2d*, OdMemoryAllocator<OdGeCurve2d*> > OdGiEdge2dArray;

/** \details
    This class represents arrays of edge attributes for meshes and shells.
    
    \remarks
    EdgeData objects are optional; they can be optionally passed 
    to OdGiWorldGeometry and OdGiViewportGeometry when using their 
	mesh and shell methods. 
    
    \remarks 
    Setting an 'improper' size of any array will cause
    unpredictable or fatal results.

    \sa
    TD_Gi

    <group OdGi_Classes> 
*/
class OdGiEdgeData
{
  const OdUInt16*         m_pColors;
  const OdCmEntityColor*  m_pTrueColors;
        OdDbStub**        m_pLayerIds;
        OdDbStub**        m_pLinetypeIds;
  const OdGsMarker*       m_pSelectionMarkers;
  const OdUInt8*          m_pVisibilities;
public:
  OdGiEdgeData()
    : m_pColors(NULL)
    , m_pTrueColors(NULL)
    , m_pLayerIds(NULL)
    , m_pLinetypeIds(NULL)
    , m_pSelectionMarkers(NULL)
    , m_pVisibilities(NULL)
  {}

  /** \details
    Sets the edge colors to be used by this object.

    \param colors [in]  Array of color indices.
        
    \remarks
    You cannot call both setColors() and setTrueColors().
    
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setColors(
    const OdUInt16 *colors)                    { m_pColors = colors; }

  /** \details
    Sets the edge colors to be used by this object.

    \param colors [in]  Array of OdCmEntityColor objects.
        
    \remarks
    You cannot call both setColors() and setTrueColors().

    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setTrueColors(
    const OdCmEntityColor *colors)         { m_pTrueColors = colors; }

  /** \details
    Sets the edge Layer Table Records to be used by this object.

    \param layerIds [in]  Array of LayerTableRecord Object IDs.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setLayers(
    OdDbStub* *layerIds)                       { m_pLayerIds = layerIds; }

  /** \details
    Sets the edge Linetype Table Records to be used by this object.

    \param linetypeIds [in]  Array of LinetypeTableRecord Object IDs.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setLinetypes(
    OdDbStub* *linetypeIds)                 { m_pLinetypeIds = linetypeIds; }

  /** \details
    Sets the edge graphics system selection markers to be used by this object.

    \param selectionMarkers [in]  Array of graphics system selection markers.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setSelectionMarkers(
    const OdGsMarker* selectionMarkers) { m_pSelectionMarkers = selectionMarkers; }

  /** \details
    Sets the edge visibilities to be used by this object.

    \param visibilities [in]  Array of visibility values.

    \remarks
    Each visibility must be one of the following:
     
    <table>
    Name              Value   Description
    kOdGiInvisible    0       Invisible 
    kOdGiVisible      1       Visible 
    kOdGiSilhouette   2       Silhouette edge 
    </table>
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setVisibility(
    const OdUInt8* visibilities)           { m_pVisibilities = visibilities; }
  
  /** \details
    Returns the array of edge colors used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdUInt16* colors() const           { return m_pColors; }

  /** \details
    Returns the array of edge colors used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdCmEntityColor* trueColors() const       { return m_pTrueColors; }

  /** \details
    Returns the array of edge LayerTableRecord Object IDs used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  OdDbStub** layerIds() const         { return m_pLayerIds; }

  /** \details
    Returns the array of edge LinetypeTableRecord Object IDs used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  OdDbStub** linetypeIds() const      { return m_pLinetypeIds; }

  /** \details
    Returns the array of edge graphics system selection markers used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGsMarker* selectionMarkers() const { return m_pSelectionMarkers; }

  /** \details
    Returns the array of edge visibilities used by this object.

    \remarks
    Each visibility must be one of the following:
     
    <table>
    Name              Value   Description
    kOdGiInvisible    0       Invisible 
    kOdGiVisible      1       Visible 
    kOdGiSilhouette   2       Silhouette edge 
    </table>

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdUInt8* visibility() const       { return m_pVisibilities; }
};


/** \details
    This class contains arrays of face attributes for meshes and shells.

    \sa
    TD_Gi

    <group OdGi_Classes> 
*/
class OdGiFaceData
{
  const OdUInt16*         m_pColors;
  const OdCmEntityColor*  m_pTrueColors;
        OdDbStub**        m_pLayerIds;
  const OdGsMarker*       m_pSelectionMarkers;
  const OdUInt8*          m_pVisibilities;
  const OdGeVector3d*     m_pNormals;
        OdDbStub**        m_pMaterialIds;
  const OdGiMapper*       m_pMappers;
  const OdCmTransparency* m_pTransparency;
  const OdGePoint2d*      m_pFillOrigins;
  const OdGeVector2d*     m_pFillDirections;
public:
  OdGiFaceData()
    : m_pColors(NULL)
    , m_pTrueColors(NULL)
    , m_pLayerIds(NULL)
    , m_pSelectionMarkers(NULL)
    , m_pVisibilities(NULL)
    , m_pNormals(NULL)
    , m_pMaterialIds(NULL)
    , m_pMappers(NULL)
    , m_pTransparency(NULL)
    , m_pFillOrigins(NULL)
    , m_pFillDirections(NULL)
  {}

  /** \details
    Sets the face colors to be used by this object.

    \param colors [in]  Array of color indices.
        
    \remarks
    You cannot call both setColors() and setTrueColors().

    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setColors(
    const OdUInt16 *colors)                    { m_pColors = colors; }

  /** \details
    Sets the face colors to be used by this object.

    \param colors [in]  Array of OdCmEntityColor objects.
        
    \remarks
    You cannot call both setColors() and setTrueColors().

    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setTrueColors(
    const OdCmEntityColor *colors)         { m_pTrueColors = colors; }

  /** \details
    Sets the face Layer Table Records to be used by this object.

    \param layerIds [in]  Array of LayerTableRecord Object IDs.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setLayers(
    OdDbStub* *layerIds)                       { m_pLayerIds = layerIds; }

  /** \details
    Sets the face graphics system selection markers to be used by this object.

    \param selectionMarkers [in]  Array of graphics system selection markers.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setSelectionMarkers(
    const OdGsMarker* selectionMarkers) { m_pSelectionMarkers = selectionMarkers; }

  /** \details
    Sets the face normal vectors to be used by this object.

    \param normals [in]  Array of normal vectors.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setNormals(
    const OdGeVector3d* normals)              { m_pNormals = normals; }

  /** \details
    Sets the face visibilities to be used by this object.

    \param visibilities [in]  Array of visibility values.

    \remarks
    Each visibility must be one of the following:
     
    <table>
    Name              Value   Description
    kOdGiInvisible    0       Invisible 
    kOdGiVisible      1       Visible 
    </table>
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setVisibility(
    const OdUInt8* visibilities)           { m_pVisibilities = visibilities; }

  /** \details
    Sets the face material object IDs to be used by this object.

    \param materialIds [in]  Array of material object IDs.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setMaterials(
    OdDbStub* *materialIds)                { m_pMaterialIds = materialIds; }

  /** \details
    Sets the face material mappers to be used by this object.

    \param mappers [in]  Array of material mappers.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setMappers(
    const OdGiMapper* mappers)             { m_pMappers = mappers; }

  /** \details
    Sets the face transparency to be used by this object.

    \param transparencies [in]  Array of OdCmTransparency objects.
        
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setTransparency(
    const OdCmTransparency *transparencies) { m_pTransparency = transparencies; }

  /** \details
    Sets the face fill origin points to be used by this object. BIM-specific

    \param fillOrigins [in]  Array of fill origin points.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setFillOrigins(
    const OdGePoint2d* fillOrigins)              { m_pFillOrigins = fillOrigins; }

  /** \details
    Sets the face fill direction vectors to be used by this object. BIM-specific

    \param fillDirections [in]  Array of fill direction vectors.
        
    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setFillDirections(
    const OdGeVector2d* fillDirections)              { m_pFillDirections = fillDirections; }

  /** \details
    Returns the array of face colors used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdUInt16* colors() const           { return m_pColors; }

  /** \details
    Returns the array of face colors used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdCmEntityColor* trueColors() const       { return m_pTrueColors; }

  /** \details
    Returns the array of face LayerTableRecord Object IDs used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  OdDbStub** layerIds() const         { return m_pLayerIds; }

  /** \details
    Returns the array of face graphics system selection markers used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGsMarker* selectionMarkers() const { return m_pSelectionMarkers; }

  /** \details
    Returns the array of face normal vectors used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGeVector3d* normals() const          { return m_pNormals; }

  /** \details
    Returns the array of edge visibilities used by this object.

    \remarks
    Each visibility must be one of the following:
     
    <table>
    Name              Value   Description
    kOdGiInvisible    0       Invisible 
    kOdGiVisible      1       Visible 
    </table>

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdUInt8* visibility() const       { return m_pVisibilities; }

  /** \details
    Returns the array of face material object IDs used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  OdDbStub** materials() const            { return m_pMaterialIds; }

  /** \details
    Returns the array of face material mappers used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGiMapper* mappers() const       { return m_pMappers; }

  /** \details
    Returns the array of face transparency used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdCmTransparency* transparency() const { return m_pTransparency; }
  
  /** \details
    Returns the array of fill origin points used by this object. BIM-specific

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGePoint2d* fillOrigins() const          { return m_pFillOrigins; }

  /** \details
    Returns the array of fill direction vectors used by this object. BIM-specific

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGeVector2d* fillDirections() const          { return m_pFillDirections; }
};

typedef enum
{
  kOdGiCounterClockwise = -1,
  kOdGiNoOrientation    = 0,
  kOdGiClockwise        = 1
} OdGiOrientationType;


/** \details
    This class contains arrays of vertex attributes for meshes and shells.
    
    \remarks
    VertexData objects are optional; they can be optionally passed 
    to OdGiWorldGeometry and OdGiViewportGeometry when using their 
	mesh and shell methods.
	
	 Setting an 'improper' size of any array will cause unpredictable or fatal results.

    \sa
    TD_Gi

    <group OdGi_Classes> 
*/
class OdGiVertexData
{
  const OdGeVector3d*    m_pNormals;
  OdGiOrientationType    m_orientationFlag;
  const OdCmEntityColor* m_pTrueColors;
  const OdGePoint3d*     m_pMappingCoords;
public:
  OdGiVertexData()
    : m_pNormals(NULL)
    , m_orientationFlag(kOdGiNoOrientation)
    , m_pTrueColors(NULL)
    , m_pMappingCoords(NULL)
  {}

  enum MapChannel { kAllChannels = 0 };

  /** \details
    Sets the vertex normal vectors to be used by this object.

    \param normals [in]  Array of normal vectors.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setNormals(
    const OdGeVector3d* normals)              { m_pNormals = normals; }

  /** \details
    Sets the orientation type to be used by this object.

    \param orientationFlag [in]  Orientation type.

    \remarks
    The orientation type defines the positive direction of the normal at the vertices.

    orientationFlag must be one of the following:

    <table>
    Name                      Value 
    kOdGiCounterClockwise     -1
    kOdGiNoOrientation        0
    kOdGiClockwise            1
    </table>
  */
  void setOrientationFlag(
    const OdGiOrientationType orientationType) { m_orientationFlag = orientationType; }

  /** \details
    Sets the vertex colors to be used by this object.

    \param colors [in]  Array of OdCmEntityColor objects.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setTrueColors(
    const OdCmEntityColor *colors)         { m_pTrueColors = colors; }

  /** \details
    Sets the mapping coordinates to be used by this object.

    \param channel [in]  Specifies mapping channel.
    \param coords [in]  Array of OdGePoint3d objects.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. An incorrect number of elements will have unpredictable or fatal consequences.
  */
  void setMappingCoords(
    MapChannel channel,
    const OdGePoint3d *coords)             { ODA_ASSERT(channel == kAllChannels); m_pMappingCoords = coords; }

  /** \details
    Returns the array of vertex normal vectors used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGeVector3d* normals() const         { return m_pNormals; }

  /** \details
    Returns the direction of the normal at the vertices that is used by this object.

    \remarks
    orientationFlag must be one of the following:

    <table>
    Name                      Value 
    kOdGiCounterClockwise     -1
    kOdGiNoOrientation        0
    kOdGiClockwise            1
    </table>
  */
  OdGiOrientationType orientationFlag() const { return m_orientationFlag; }

  /** \details
    Returns the array of vertex colors used by this object.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdCmEntityColor* trueColors() const   { return m_pTrueColors; }

  /** \details
    Returns the array of mapping coordinates used by this object.

    \param channel [in]  Specifies mapping channel.

    \remarks
    This function does not make a copy of the array, which should have the same number of elements as the 
    mesh or shell with which it is used. Writing beyond the array bounds will have unpredictable or fatal consequences.
  */
  const OdGePoint3d* mappingCoords(MapChannel channel) const { ODA_ASSERT(channel == kAllChannels); return m_pMappingCoords; }
};

class OdGiDrawable;
struct OdGiClipBoundary;
class OdGiAbstractClipBoundary;

class OdDbPolyline;
class OdPolyPolygon3d;
class OdGeNurbCurve3d;
class OdGiTextStyle;
class OdDbBody;
class OdGeEllipArc3d;
class OdGiPolyline;

/** \details
    Arc types.
*/
typedef enum
{
  kOdGiArcSimple = 0,   // Unfilled.
  kOdGiArcSector = 1,   // Filled area bounded by the arc and its center.
  kOdGiArcChord  = 2    // Filled area bounded by the arc and its end points
} OdGiArcType;

/** \details
    This class defines functions that allow entities to vectorize themselves.

    \sa
    TD_Gi

    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiGeometry : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiGeometry);

  /** \details
    Returns the model-to-world coordinate transform matrix for the entity being vectorized.
    
    \remarks
    This is the inverse of the matrix returned by getWorldToModelTransform(). 

    If an entity is in one or more blocks, this matrix can be used to
    determine the WCS coordinates of the entity.  
  */
  virtual OdGeMatrix3d getModelToWorldTransform() const = 0;

  /** \details
    Returns the world-to-model coordinate transform matrix for the entity being vectorized.
    
    \remarks
    This is the inverse of the matrix returned by getModelToWorldTransform(). 
  */
  virtual OdGeMatrix3d getWorldToModelTransform() const = 0;
  
  /** \details
    Specifies a model transformation matrix for the current transformation stack.  
    
    \remarks
    When a vector is specified, the transformation is created using the arbitrary axis algorithm 
    on the vector. 
 
    The specified transformation is concatenated to the current model transformation 
	(which is initially the identity transform). The resulting model transformation 
	is subsequently applied to all geometry introduced into this vectorization context, 
	until popModelTransform() is called.
    
    \param normal [in]  Normal vector.    
  */
  virtual void pushModelTransform(
    const OdGeVector3d& normal) = 0;

  /** \details
    Specifies a model transformation matrix for the current transformation stack.  
    
    \remarks
    The specified transformation is concatenated to the current model transformation 
	(which is initially the identity transform). The resulting model transformation 
	is subsequently applied to all geometry introduced into this vectorization context, 
	until popModelTransform() is called.
    
    \param xfm [in]  Transformation matrix.
    
  */
  virtual void pushModelTransform(
    const OdGeMatrix3d& xfm) = 0;

  /** \details
    Removes the top transformation off the current transformation stack.
  */
  virtual void popModelTransform() = 0;
  
  /** \details
    Introduces a circle to this vectorization context.  
    
    \remarks
    The circle is not filled and takes on the current color.
    
    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident.

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal.
  */
  virtual void circle(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal) = 0;
  
  /** \details
    Introduces a circle to this vectorization context.

    \param firstPoint [in]  First point of a 3-point circle.
    \param secondPoint [in]  Second point of a 3-point circle.
    \param thirdPoint [in]  Third point of a 3-point circle.
  */
  virtual void circle(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint) = 0;
  
/** \details
    Introduces a circular arc to this vectorization context.  
    
    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Start of the arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    
    \remarks
    The arc takes on the current color.      

    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident.
    
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>
    
    \remarks
    All angles are expressed in radians.
*/
  virtual void circularArc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple) = 0;
  
  /** \details
    Introduces a circular arc to this vectorization context.
    
    \param firstPoint [in]  First point of a 3-point arc.
    \param secondPoint [in]  Second point of a 3-point arc.
    \param thirdPoint [in]  Third point of a 3-point arc.
    \param arcType [in]  Arc type.
  */
  virtual void circularArc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple) = 0;

  /** \details
    Introduces a polyline to this vectorization context.  
    
    \remarks
    The polyline is unfilled and takes on the current color and thickness. Use polygon() to render filled areas.
    
    The polyline is rendered as a series of lines connecting the first point
    in vertexList to the second, the second to the third, etc.
    All points must be coplanar.

    Use polygon() to render closed areas.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param baseSubEntMarker [in]  Not used.
  */
  virtual void polyline(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    OdGsMarker baseSubEntMarker = -1) = 0;
  
  /** \details
    Introduces a polygon to this vectorization context.  
    
    \remarks
    The polygon is filled and takes on the current color. Use polyline() to render unfilled areas.

    The polygon is rendered as a series of lines connecting the first point
    in vertexList to the second, the second to the third, etc.
    All points must be coplanar.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
  */
  virtual void polygon(
    OdInt32 numVertices, 
    const OdGePoint3d* vertexList) = 0;

  /** \details
  Introduces a polygon (with a normal) to this vectorization context.

  \remarks
  The polygon is filled and takes on the current color. Use polyline() to render unfilled areas.

  The polygon is rendered as a series of lines connecting the first point
  in vertexList to the second, the second to the third, etc.
  All points must be coplanar.

  \param numVertices [in]  Number of vertices.
  \param vertexList [in]  Array of vertices.
  \param pNormal [in]  Pointer to the normal vector.
  */
  virtual void polygon(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal);

  /** \details
    Introduces a lightweight polyline into this vectorization context.
    
    \remarks
    The polyline may contain varying segment widths, straight segments
    and arc segments.    

    The polyline takes on the current color.
    
    All points must be coplanar.

    \param polyline [in]  Polyline.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed (0 indicates all segments).
  */
  virtual void pline(
    const OdGiPolyline& polyline, 
    OdUInt32 fromIndex = 0, 
    OdUInt32 numSegs = 0) = 0;

  /** \details
    Introduces a mesh into this vectorization context.  
    
    \param numRows [in]  Number of rows.
    \param numColumns [in]  Number of columns.
    \param vertexList [in]  Array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 

    \remarks
    A mesh is a surface defined by a grid of vertices, and corresponds to a Polygon Mesh.
    By default, a mesh takes on the current color. Color, linetype, and
    and other properties can be controlled supplying the appropriate
    data for the pEdgeData, pFaceData, and pVertexData arguments.

    \remarks
    * vertexList must have numRows x numColumns elements.
    * The arrays in *pEdgeData must be NULL or contain (numRows - 1) x numColumns + numRows x (numColumns -1) elements.
    * The arrays in *pFaceData must be NULL or contain (numRows - 1) x (numColumns - 1) elements.
    * The arrays in *pVertexData must be NULL or contain numRows x numColumns elements. 
  */
  virtual void mesh(
    OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0) = 0;
  
  /** \details
    Introduces a shell into this vectorization context.  
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
    \param faceListSize [in]  Number of entries in facesList.
    \param faceList [in]  Array of integers defining faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data.
    
    \remarks
    A shell is a set of faces that can contain holes, and corresponds to a Polyface mesh.

    By default, a mesh takes on the current color. Color, linetype, and
    and other properties can be controlled supplying the appropriate
    data for the pEdgeData, pFaceData, and pVertexData arguments.
    
    \remarks
    * The arrays in *pEdgeData must be NULL or contain the same number of elements as there are edges.
    * The arrays in *pFaceData must be NULL or contain the same number of elements as there are faces.
    * The arrays in *pVertexData must be NULL or contain numVertices elements. 
    
    \sa
    Faces
  */
  virtual void shell(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0) = 0;
  
  /** \details
    Introduces text into this vectorization context.
    
    \param position [in]  Position of the text string.
    \param normal [in]  Normal vector of the text.
    \param direction [in]  Baseline direction of the text.
    \param height [in]  Height of the text.
    \param width [in]  Width factor of the text.
    \param oblique [in]  Oblique angle.
    \param msg [in]  Text string.

    \remarks
    The text takes on the current color.
 
    If numBytes is not specified, msg must be null terminated.
    
    \remarks
    All angles are expressed in radians.

    As currently implemented, this function ignores width and oblique.
    They will be fully implemented in a future release.
  */
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg) = 0;
  
  /** \details
    Introduces text into this vectorization context.
    
    \param position [in]  Position of the text string.
    \param normal [in]  Normal vector of the text.
    \param direction [in]  Baseline direction of the text.
    \param msg [in]  Text string.
    \param length [in]  Number of characters in msg (not including the optional null character).
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to the TextStyle for the text.
  */
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw, 
    const OdGiTextStyle* pTextStyle) = 0;
  
  /** \details
    Introduces an Xline into this vectorization context.  
      
    \remarks
    Xlines are infinite lines passing through two points.
 
    The xline takes on the current color.
    
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
  */
  virtual void xline(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint) = 0;

  /** \details
    Introduces a Ray into this vectorization context.  
    
    \remarks
    A Ray is a semi-infinite line that starts at the basePoint, and passes 
    through the throughPoint.

    The ray takes on the current color.
 
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point
  */
  virtual void ray(
    const OdGePoint3d& basePoint, 
    const OdGePoint3d& throughPoint) = 0;
  
  /** \details
    Introduces a NURBS curve into this vectorization context.
      
    \remarks
    The curve takes on the current color.
    
    \param nurbsCurve [in]  NURBS curve data.
  */
  virtual void nurbs(
    const OdGeNurbCurve3d& nurbsCurve) = 0;

  /** \details
    Introduces an elliptical arc into this vectorization context.
        
    \param ellipArc [in]  Elliptical arc.
    \param endPointsOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
    \param arcType [in]  Arc type.
        
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     3         Filled area bounded by the arc and its end points
    </table>
  */
  virtual void ellipArc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple) = 0;

  /** \details
    Introduces the specified object into this vectorization context.
    
    \param pDrawable [in]  Pointer to the Drawable object.

    \remarks
    Implementations of this method are expected to do the following:
    * Call OdGiDrawable::subSetAttributes to set attribute information for the object.
    * Call subWorldDraw on the drawable object  to vectorize it into this context.
    * If subWorldDraw returns false, call subViewportDraw each viewport.
  */
  virtual void draw(
    const OdGiDrawable* pDrawable) = 0;

  /** \details
    Specifies a clip boundary for the current clip stack.

    \param pBoundary [in]  Pointer to the boundary.

    \remarks
    Subsequent objects are clipped until popClipBoundary() is called.
  */
  virtual void pushClipBoundary(
    OdGiClipBoundary* pBoundary) = 0;

  /** \details
    Removes the top clip boundary from the clip stack.
  */
  virtual void popClipBoundary() = 0;

  /** \details
    Introduces a line into this vectorization context.  

    \param points [in]  Array of WCS start and end points.

	  \remarks
    The current model transformation is not applied to the line.
  */
  virtual void worldLine(
    const OdGePoint3d points[2]) = 0;

  /** \details
    Inroduces a image in BGRA format into this vectorization context.
    
    \param img [in]  Image in BGRA format.
    \param origin [in]  Image position in WCS.
    \param uVec [in]  Image width and X-axis orientation.
    \param vVec [in]  Image height and Y-axis orientation.
    \param trpMode [in]  Image transparency processing mode.
  */
  virtual void image(
    const OdGiImageBGRA32& img,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit) = 0;

  /** \details
    Inroduces a boundary for hatch filling.
    
    \param edges [in]  Array of 2d curves which represents filling boundary.
  */
  virtual void edge(
    const OdGiEdge2dArray& edges) = 0;

  /** \details
    Returns currently processing GiDrawable objects graph.
  */
  virtual const OdGiPathNode* currentGiPath() const;

  /** \details
    Specifies a clip boundary with additional clipping information for the current clip stack.

    \param pBoundary [in]  Pointer to the boundary.
    \param pClipInfo [in]  Additional clipping information.

    \remarks
    Subsequent objects are clipped until popClipBoundary() is called.
  */
  virtual void pushClipBoundary(
    OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);

  /** \details
    Introduces an array of points to this vectorization context.

    \param numPoints [in]  Number of point in array.
    \param vertexList [in]  Array of points.
    \param pColors [in]  Array of colors.
    \param pTransparency [in]  Array of transparencies.
    \param pNormals [in]  Optional array of normal vectors.
    \param pSubEntMarkers [in]  Optional array of subentity markers.
    \param nPointSize [in]  Size of points in pixels.

    \remarks
    The points takes on the current color if pColors array doesn't specified.

    Lengths of pColors, pTransparency, pNormals and pSubEntMarkers arrays if them set must be equal to numPoints.
  */
  virtual void polypoint(
    OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency,
    const OdGeVector3d* pNormals = NULL,
    const OdGsMarker* pSubEntMarkers = NULL,
    OdInt32 nPointSize = 0);
  
  /** \details
    Introduces an array of points to this vectorization context.

    \param numPoints [in]  Number of point in array.
    \param vertexList [in]  Array of points.
    \param pColors [in]  Array of colors.
    \param pNormals [in]  Optional array of normal vectors.
    \param pSubEntMarkers [in]  Optional array of subentity markers.

    \remarks
    The points takes on the current color if pColors array doesn't specified.

    Lengths of pColors, pNormals and pSubEntMarkers arrays if them set must be equal to numPoints.
  */
    ODRX_SEALED_VIRTUAL void polypoint(
    OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdGeVector3d* pNormals = NULL,
    const OdGsMarker* pSubEntMarkers = NULL) ODRX_SEALED;
  
  /** \details
    Introduces an array of points to this vectorization context.

    \param numPoints [in]  Number of point in array.
    \param vertexList [in]  Array of points.
    \param pNormals [in]  Optional array of normal vectors.
    \param pSubEntMarkers [in]  Optional array of subentity markers.

    \remarks
    The points takes on the current color if pColors array doesn't specified.

    Lengths of pNormals and pSubEntMarkers arrays if them set must be equal to numPoints.
  */
    ODRX_SEALED_VIRTUAL void polypoint(
    OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormals = NULL,
    const OdGsMarker* pSubEntMarkers = NULL) ODRX_SEALED;

  /** \details
    Introduces a row of points to this vectorization context.

    \param numPoints [in]  Number of points to draw.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset and direction between next points.

    \remarks
    The points takes on the current color.
  */
  virtual void rowOfDots(
    OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);
};

inline const OdGiPathNode* OdGiGeometry::currentGiPath() const
{
  return 0;
}

inline void OdGiGeometry::pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* /*pClipInfo*/)
{
  // Some implementations could not support additional clipping styles, so redirect to old style clipping method by default.
  pushClipBoundary(pBoundary);
}

inline void OdGiGeometry::polygon(OdInt32 numVertices, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal)
{
  polygon(numVertices, vertexList);
}

inline void OdGiGeometry::polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* /*pColors*/,
                                    const OdCmTransparency* /*pTransparency*/, const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers,
                                    OdInt32 /*nPointSize*/)
{
  OdGePoint3d tmpVerts[2];
  for (OdInt32 nPoint = 0; nPoint < numPoints; nPoint++)
  {
    const OdGeVector3d *pNormal = (pNormals) ? (pNormals + nPoint) : NULL;
    const OdGsMarker baseSubEntMarker = (pSubEntMarkers) ? pSubEntMarkers[nPoint] : -1;
    tmpVerts[1] = tmpVerts[0] = vertexList[nPoint];
    polyline(2, tmpVerts, pNormal, baseSubEntMarker);
  }
}

inline void OdGiGeometry::polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors,
                                    const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers)
{
  polypoint(numPoints, vertexList, pColors, NULL, pNormals, pSubEntMarkers);
}

inline void OdGiGeometry::polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList,
                                    const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers)
{
  polypoint(numPoints, vertexList, NULL, pNormals, pSubEntMarkers);
}

inline void OdGiGeometry::rowOfDots(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint)
{
  OdGePoint3d tmpVerts[2];
  for (OdInt32 nPoint = 0; nPoint < numPoints; nPoint++)
  {
    tmpVerts[1] = tmpVerts[0] = startPoint + dirToNextPoint * double(nPoint);
    polyline(2, tmpVerts);
  }
}

/** \details
This class is used to ensure OdGiGeometry::pushClipBoundary()/OdGiGeometry::popClipBoundary() 
consistency (exception safety).

\sa
TD_Gi

<group OdGi_Classes> 
*/
class OdGiClipBoundarySaver
{
  bool m_bSave;
  OdGiGeometry& m_geom;
public:
  OdGiClipBoundarySaver(OdGiGeometry& geom, bool bSave) : m_bSave(bSave), m_geom(geom){}
  void setSave(bool bSave){m_bSave = bSave;}
  ~OdGiClipBoundarySaver()
  {
    if (m_bSave)
      m_geom.popClipBoundary();
  }
};

/** \details
This class is used to ensure OdGiGeometry::pushModelTransform()/OdGiGeometry::popModelTransform() 
consistency (exception safety).

\sa
TD_Gi

<group OdGi_Classes> 
*/
class OdGiModelTransformSaver
{
  OdGiGeometry& m_geom;
public:
  OdGiModelTransformSaver(OdGiGeometry& geom, const OdGeMatrix3d& xMat) : m_geom(geom)
  {
    m_geom.pushModelTransform(xMat);
  }
  ~OdGiModelTransformSaver()
  {
    m_geom.popModelTransform();
  }
};

/* #11298 info: Matrix identity check was removed from OdGiModelTransformSaver and place into separate OdGiModelTransformSaverOpt class.
                Identity check is also time consuming operation and it is only makes things worse if matrices become identity seldom in
                some case. Following statistics on 10 typical drawing was collected on 18.10.2012:

                     Source File                                     : Line     Identity=true      Identity=false     % of identity=true
..\..\..\..\..\TD\Source\Gs\GsBlockReferenceNode.cpp                 : 1263     3202               60838              5%
..\..\..\..\..\TD\Source\Gs\GsBlockReferenceNodeImpl.cpp             : 707      2                  64364              0.003%
..\..\..\..\..\TD\Source\Gs\GsBlockReferenceNodeImpl.cpp             : 868      3                  62                 4.6%
..\..\..\..\..\TD\Source\database\Entities\DbBlockReference.cpp      : 1101     25                 64905              0.038%
..\..\..\..\..\TD\Source\database\Entities\DbDimension.cpp           : 1307     12546              16                 99.87%
..\..\..\..\..\TD\Source\database\Entities\DbHatchRender.cpp         : 714      38566              0                  100%
..\..\..\..\..\TD\Source\database\Entities\DbMInsertBlock.cpp        : 322      1603               30457              5%
..\..\..\..\..\TD\Source\database\Entities\DbModelerGeometryImpl.cpp : 1410     8                  0                  100%
..\..\..\..\..\TD\Source\database\Entities\DbModelerGeometryImpl.cpp : 1545     6                  0                  100%
..\..\..\..\..\TD\Source\database\Entities\DbPoint.cpp               : 264      0                  208                0%
..\..\..\..\TD\Source\database\GiDefaultContext.cpp                  : 454      4405               0                  100%
..\..\..\..\TD\Source\database\GiDefaultContext.cpp                  : 557      124                23626              0.522%

All places where percents of identity=true < 10% will use OdGiModelTransformSaver. Places where Identity=true very frequent will use
OdGiModelTransformSaverOpt. Unchecked places isn't principle since them are called very seldom.                                          */

/** \details
This class is used to ensure OdGiGeometry::pushModelTransform()/OdGiGeometry::popModelTransform() 
consistency (exception safety) with additional optimization check of matrix identity.

\sa
TD_Gi

<group OdGi_Classes> 
*/
class OdGiModelTransformSaverOpt
{
  OdGiGeometry& m_geom;
  bool          m_bIdentity;
public:
  OdGiModelTransformSaverOpt(OdGiGeometry& geom, const OdGeMatrix3d& xMat) : m_geom(geom)
  {
    // @@@TODO: check, probably simple memcmp will be more effective here. memcmp is very assembler-optimized in modern crt's. Identity
    //          matrix could be inexact only after some multiplications and etc. But statistics must be collected first for isEqual check
    //          and for memcmp, if it will be faster and precisely enough isEqualTo check with tolerance can be exchanged by memcmp.
    m_bIdentity = xMat.isEqualTo(OdGeMatrix3d::kIdentity);
    if (!m_bIdentity)
      m_geom.pushModelTransform(xMat);
  }
  ~OdGiModelTransformSaverOpt()
  {
    if (!m_bIdentity)
      m_geom.popModelTransform();
  }
};

#include "TD_PackPop.h"

#endif
