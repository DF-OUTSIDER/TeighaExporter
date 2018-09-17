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


#ifndef _OD_MODELERGEOMETRY_INCLUDED_
#define _OD_MODELERGEOMETRY_INCLUDED_

#define STL_USING_MAP
#include "OdaSTL.h"
#include "RxObject.h"
#include "ModelerDefs.h"
#include "DbEntity.h"
#include "Db3dSolid.h"
#include "DbRegion.h"
#include "DbSubDMesh.h"
#include "DbNurbSurface.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "MaterialResolver.h"

class OdStreamBuf;
class OdBrBrep;
class OdBrEntity;
class OdGiCommonDraw;
class OdGiViewportDraw;
class OdDbCurve;
class OdDbRevolveOptions;
class OdDbSweepOptions;
class OdDbLoftOptions;
class OdDbBlendOptions;
class OdDbSection;
class SectArgs;
class OdBrMesh2dFilter;
class OdBrMesh2d;
namespace GeMesh { class OdGeTrMesh; }
class wrTriangulationParams;

#include "TD_PackPush.h"

/** \details
    This class defines the interface for classes that
    provide conversion services for ACIS SAT and SAB data.

    \remarks
    This class is used to convert ACIS data between various
    versions of SAT and SAB.  
    
    For example, if a version 2004 .dwg file is saved as an R15 .dxf file, 
    the SAB data in the 2004 file must be converted to version 700 SAT,
    so as to be be valid within the R15 .dxf file.

    Corresponding C++ library: ModelerGeometry
    <group Other_Classes>
*/
class DBENT_EXPORT OdModelerGeometry : public OdRxObject
{
protected:
  OdModelerGeometry() {}

public:  
  ODRX_DECLARE_MEMBERS(OdModelerGeometry);

  // in/out functionality

  /** \details
      Reads the ACIS data from the specified StreamBuf object into this ModelerGeometry object.

      \remarks
      The implementation of this function is responsible for determining
      the type (SAT/SAB) and version of the data read.

      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data is to be read.   
      \param typeVer [out]  Receives the type and version of the ACIS file.
      \param standardSaveFlag [in]  Controls the saving and restoring of used count data in the saved file.
      \param bEnableACISAudit [in]  true - do additional checks of acis data.
      
      \returns
      Returns eOk if successful, or an appropriate error code if not.
      \remarks
      pStreamIn may be NULL.
     
      \sa
      AfTypeVerEnum
  */
  virtual OdResult in(OdStreamBuf* pStreamBuf, AfTypeVer *typeVer = 0, bool standardSaveFlag = true) = 0;
  

  /** \details
      Writes the ACIS data from this ModelerGeometry object to the specified StreamBuf object.

      \remarks
      This function is responsible for converting
      the contained ACIS data to the specified type and format. If afVer == kAfTypeVerAny, 
      then any type or version may be written.

      \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data is to be written.   
      \param typeVer [in]  Type and version of the ACIS file.
      \param standardSaveFlag [in]  Controls the saving and restoring of used count data in the saved file.
      \param bEnableACISAudit [in]  true - do additional checks of acis data.

      \returns
      Returns eOk if successful, or an appropriate error code if not.

      \sa
      AfTypeVerEnum
  */
  virtual OdResult out(OdStreamBuf* pStreamBuf, AfTypeVer typeVer, bool standardSaveFlag = true) const = 0;

  /** \details
      Populates the specified BrBrep object with the ACIS BREP data of this ModelerGeometry object.
      \param brBrep [out]  Receives the BREP data. 
      \returns
      Returns true if and only if successful.
      
  */
  virtual bool brep(OdBrBrep& brBrep) const = 0;

  virtual OdResult getEdgeSubentityGeometry(OdArray<OdUInt32> indexesOfEdgeCalculation, OdGeCurve3d*& pEdgeCurve) = 0;

  virtual OdResult setSubentPath(OdBrEntity & /*pInpEnt*/, OdDbFullSubentPath& /*subpath*/) { return eNotImplemented; }

  virtual OdResult generateMesh(const OdBrMesh2dFilter& filter, OdBrMesh2d &mesh2d) { return eNotImplemented; }

  /** \details
    Sets the Facetres for this ModelerGeometry object.
    \param facetRes [in]  Facet resolution.
  */
  virtual bool setFACETRES(double facetRes) = 0;

  struct NumIsolines
  {
    OdUInt32 numU, numV;
  };
  /** \details
    Creates a viewport-independent geometric representation of this ModelerGeometry object.
    
    \returns
    Returns true if and only if the geometric representation can be generated in a
    viewport-independent manner. 

    \remarks
    A return value of false indicates that viewportDraw() must be called for this object.

    The 3D GS will call this function at least once, but may cache subsequent display updates.

    \param pWd [in]  Pointer to the OdGiCommonDraw interface.
    \param geomType [in]  Geometry type.
    \param pNumIsolines [in]  Number of isolines in U and V directions.
    
    \remarks
    geomType must be a combination of one or more of the following:
    
    <table>
    Name            Value
    kNothing        0   
    kIsolines       1
    kEdges          2
    kShells         4
    kOrderedEdges   8
    kSurfIsolines   16
    </table>
  */
  virtual bool worldDraw(OdGiCommonDraw* pWd, OdUInt32 geomType, const NumIsolines *pNumIsolines = 0) = 0;
  
  /** \details
    Creates a viewport-dependent silhouette representation of this ModelerGeometry object.

    \remarks
    Causes this ModelerGeometry object to describe its geometry to the specified OdGiViewportDraw object. 

    This function is called once per viewport.

    \remarks
    It is possible to have totally different images in each viewport; a schematic
    in one viewport, a layout in another.
    
    \param pVd [in]  Pointer to the OdGiViewportDraw interface.
  */
  virtual bool drawSilhouettes(OdGiViewportDraw* pVd) = 0;
  virtual OdResult getCachedSilhouettes(OdGeCurve3dPtrArray* cachedSilhouettes) = 0;

  /** \details
    Explodes this ModelerGeometry object into a set of OdDbEntity objects.  

    \param entitySet [in/out] Receives an array of pointers to the new entities.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.

    \remarks
    Entities resulting from the explosion are appended to the specified array.
    
    The newly created entities are not database residents.
  */
  virtual bool explode(OdDbEntityPtrArray& entitySet) const = 0;
  
  /** \details
    Returns the transformation matrix for this ModelerGeometry object.
    \param xfm [out]  Receives the transformation matrix.
  */
  virtual bool getTransformation(OdGeMatrix3d& xfm) = 0;
  
  /** \details
    Applies the specified 3D transformation matrix to this ModelerGeometry object.

    \param xfm [in]  3D transformation matrix.
  */
  virtual void transformBy( const OdGeMatrix3d& xfm ) = 0;
  
  /** \details
    Creates a WCS aligned solid box centered about the world origin. 
    
    \param xLen [in]  Dimension along the WCS X-axis.
    \param yLen [in]  Dimension along the WCS Y-axis. 
    \param zLen [in]  Dimension along the WCS Z-axis. 

    \returns
    Returns eOk if successful, or an appropriate error code if not. 
    
    \remarks
    The following constraints apply:
    
    * xLen >= 1e-6.
    * yLen >= 1e-6.
    * zLen >= 1e-6.
  */
  virtual void createBox( double xLen, double yLen, double zLen ) = 0;
  /** \details
    Creates a WCS aligned frustum centered about the world origin. 
    
    \param height [in]  Dimension along the WCS Z-axis.
    \param xRadius [in]  Base radius along the WCS X-axis. 
    \param yRadius [in]  Base radius along the WCS Y-axis.
    \param topXRadius [in]  Top radius along the WCS X-axis. 

    \remarks
    The function can generate a frustum that is any one of the following:
    
    * A circular cylinder
    * An elliptical cylinder
    * A circular cone
    * An elliptical cone
    * A truncated circular cone
    * A truncated elliptical cone
    
    \remarks
    The following constraints apply:
    
    * height >= 1e-6.
    * xRadius >= 1e-6.
    * yRadius >= 1e-6.
    * topXRadius >= 1e-6. 
  */
  virtual void createFrustum( double height, double xRadius, double yRadius, double topXRadius ) = 0;
  /** \details
    Creates a sphere centered about the world origin. 
    
    \param radius [in]  Radius.

    \remarks
    The following constraints apply:
    
    * radius >= 1e-6.

  */
  virtual void createSphere( double radius ) = 0;
  /** \details
    Creates a WCS aligned torus centered about the world origin
    and with the Z-axis as its axis of rotation. 
    
    \param majorRadius [in]  Radius of the torus
    \param minorRadius [in]  Radius of the tube.

    \remarks
    The following constraints apply:
    
    * minorRadius >= 1e-6.
    * |majorRadius| >= 1e-6
    * if majorRadius < 0, then minorRadius > |majorRadius| + 1e-6
  */
  virtual void createTorus( double majorRadius, double minorRadius ) = 0;

  /** \details
    Creates a WCS aligned wedge centered about the world origin. 
    
    \param xLen [in]  Dimension along the WCS X-axis.
    \param yLen [in]  Dimension along the WCS Y-axis. 
    \param zLen [in]  Dimension along the WCS Z-axis. 

    \remarks
    The inclined face is defined by the points 
    
              ( xLen/2, -yLen/2, -zLen/2)
              (-xLen/2, -yLen/2,  zLen/2)  
              ( xLen/2,  yLen/2, -zLen/2)
              (-xLen/2, +yLen/2,  zLen/2)  
    
    \remarks
    The following constraints apply:
    
    * xLen >= 1e-6.
    * yLen >= 1e-6.
    * zLen >= 1e-6.
  */
  virtual void createWedge( double xLen, double yLen, double zLen ) = 0;

  /** \details
    Creates a solid by extruding the specified region.
   
    \param pRegion [in]  Pointer to the region to be extruded.
    \param height [in]  Height of the extrusion.
    \param taperAngle [in]  Taper angle.

    \remarks
    The extrusion direction is along the normal of the region. Height may be negative.     


    \remarks
    All angles are expressed in radians.
     
    The following constraints apply:
    
    * |height| >= 1e-6.
    * |taperAngle| < OdaPI2 - 1e-6.
  */
  virtual OdResult extrude(const OdDbRegion* pRegion, double height, double taperAngle, bool isSolid = true) = 0;

  /** \details
    Creates a solid by revolving the specified region.
   
    \param pRegion [in]  Pointer to the region to be extruded.
    \param axisPoint [in]  Point on the axis of revolution.
    \param axisDir [in]  Vector defining the axis of revolution.
    \param angleOfRevolution [in]  Angle of revolution.
    
    \remarks
    The axis of revolution is projected onto the plane of the region, parallel to its normal.

    \remarks
    All angles are expressed in radians.
    
    The following constraints apply:
    
    * The projected axis of revolution cannot intersect the region. 
    * angleOfRevolution >= 1e-6
  */
  virtual OdResult revolve(const OdDbRegion* pRegion, const OdGePoint3d& axisPoint, 
    const OdGeVector3d& axisDir, double angleOfRevolution, bool isSolid = true) = 0;

  /** \details
    Clears the color attributes of this ModelerGeometry object.
  */
  virtual void ClearColorAttributes() = 0;

  /** \details
  Clears the material attributes of this ModelerGeometry object.
  */
  virtual void ClearMaterialAttributes() = 0;

  /** \details
  Clears the material mapper attributes of this ModelerGeometry object.
  */
  virtual void ClearMaterialMapperAttributes() = 0;

  /** \details
    Describes a material state.
  */
  enum MaterialState
  {
    kNoMaterials = 0, 
    kHasMaterials = 1,
    kUnknown = 2
  };
  virtual MaterialState hasMaterials() const {return kUnknown; }

  /** \details
    Returns true if and only if TrueColor attributes are attached to the faces and/or
    edges of this ModelerGeometry object.
  */
  virtual bool hasTrueColorAttributes() const = 0;

  /** \details
    Returns the plane of this ModelerGeometry object.

    \param regionPlane [out]  Receives the region plane.
    \returns
    Returns eOk if successful, or an appropriate error code if (bodyType() != kRegion).
    
  */
  virtual OdResult getPlane(OdGePlane& regionPlane) const = 0;

  /** \details
    Sets the mm/unit for this ModelerGeometry object.
    \param mmPerUnit [in]  mm/unit. 
    \returns
    Returns true if and only if successful.
  */
  virtual bool setMMPerUnit(double  mmPerUnit) = 0;
  /** \details
    Returns the mm/unit for this ModelerGeometry object.
    \param mmPerUnit [out]  mm/unit.
    \returns
    Returns true if and only if successful.
  */
  virtual bool getMMPerUnit(double &mmPerUnit) const = 0;
  
  /** \details
    Geometry type.
  */
  enum geomType
  {
    kUndefined = 0,
    kBody      = 1,
    kSolid     = 2,
    kRegion    = 3,
    kSurface   = 4
  };

  /** \details
    Returns the body type of this ModelerGeometry object:
    \remarks
    bodyType returns one of the following:
    
    <table>
    Name         Value
    kUndefined   0
    kBody        1
    kSolid       2
    kRegion      3
    </table>
  */
  virtual geomType bodyType() const = 0;

  
  /** \details
    Performs a Boolean operation between this ModelerGeometry object and another ModelerGeometry object. 

    \param operation [in] Type of Boolean operation.
    \param otherRegion [out] Pointer to the other ModelerGeometry object which also will contain the result of the Boolean operation.

    \returns Returns eOk if the Boolean operation is successful or an appropriate error code if not. 
    If ModelerGeometry is null, eInvalidInput is returned.

    \remarks 
    Possible operation types are: 
      * kBoolUnite - Unites two ModelerGeometry objects into one.
      * kBoolIntersect - Returns the intersection of two ModelerGeometry objects.
      * kBoolSubtract - Subtracts the second ModelerGeometry object from the first one.
  */
  virtual OdResult booleanOper(OdDb::BoolOperType operation, OdSmartPtr<OdModelerGeometry> otherRegion) = 0;
  
  /** \details
    Performs a Boolean operation between this ModelerGeometry object and another ModelerGeometry object. 

    \param operation [in] Type of Boolean operation.	
    \param pModelerPar [in] Pointer to the other ModelerGeometry object which also will contain the result of the Boolean operation.
    \param intersectionEntities [out] Array of intersection entities.
    
    \returns Returns eOk if Boolean operation is successful or an appropriate error code if not. 
    If ModelerGeometry is null, eInvalidInput is returned.

    \remarks 
    Possible operation types are: 
      * kBoolUnite - Unites two ModelerGeometry objects into one.
      * kBoolIntersect - Returns the intersection of two ModelerGeometry objects.
      * kBoolSubtract - Subtracts the second ModelerGeometry object from the first one.
  */
  virtual OdResult booleanOper(OdDb::BoolOperType operation, const OdSmartPtr<OdModelerGeometry> pModelerPar, OdDbEntityPtrArray& intersectionEntities) = 0;
  
  /** \details
    Returns the area of this ModelerGeometry object. 
    \param regionArea [out] Reference for returning the area value.
    \returns Returns eOk if successful or an appropriate error code in the other case. 
    Area value is returned in square drawing units.
  */
  virtual OdResult getArea(double& regionArea) const = 0;
  
  /** \details
    Returns the perimeter of this ModelerGeometry object. 
    \param dRegionPerimeter [out] Reference to a double value to store the perimeter.
    
    \returns Returns eOk if the perimeter was successfully returned to the calling 
    subroutine or an appropriate error code in the other case.
  */
  virtual OdResult getPerimeter(double&) const = 0;

  /** \details
    Calculates the area properties of this ModelerGeometry object. 
  
    The method validates the origin, xAxis, and yAxis parameters to ensure that:
* axes are perpendicular to each other.
* axes and the origin lie in the same plane as the region.

  \param origin        [in]  Origin of the coordinate system to use for evaluation (WCS). 
  \param xAxis         [in]  X-axis of the coordinate system to use for evaluation (WCS).
  \param yAxis         [in]  Y-axis of the coordinate system to use for evaluation (WCS).
  \param perimeter     [out] Reference for returning the value of the ModelerGeometry object's perimeter.
  \param area          [out] Reference for returning the value of the ModelerGeometry object's area.
  \param centroid      [out] Reference for returning the centroid of the ModelerGeometry object.
  \param momInertia    [out] Static array for returning moments of inertia for the ModelerGeometry object.
  \param prodInertia   [out] Reference for returning the ModelerGeometry object's product of inertia.
  \param prinMoments   [out] Static array for returning principal moments for the ModelerGeometry object. 
  \param prinAxes      [out] Static array for returning principle axes of the ModelerGeometry object. 
  \param radiiGyration [out] Static array for returning radii of gyration of the ModelerGeometry object. 
  \param extentsLow    [out] Reference for returning the ModelerGeometry object's minimum extents point. 
  \param extentsHigh   [out] Reference for returning the ModelerGeometry object's maximum extents point.

  \returns Returns eOk if successful, or an appropriate error code otherwise.

  \remarks 
  Returned values are measured using the values of origin, xAxis, and yAxis parameters, 
  and are represented in WCS coordinates.
  */
  virtual OdResult getAreaProp( const OdGePoint3d& origin, const OdGeVector3d& xAxis,
    const OdGeVector3d& yAxis, double& perimeter,
    double& area, OdGePoint2d& centroid,
    double momInertia[2], double& prodInertia,
    double prinMoments[2], OdGeVector2d prinAxes[2],
    double radiiGyration[2], OdGePoint2d& extentsLow,
    OdGePoint2d& extentsHigh ) const = 0;
  
  /** \details
    Clears the contents of this ModelerGeometry object.
    
    \returns Returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult clear( ) = 0;
  
  /** \details
    Creates a ModelerGeometry object by extruding a region along a path curve. 

    \param region [in] Pointer to a region object for extruding from.
    \param path   [in] Pointer to a curve object to extrude along. Must be one of the following: 
* Line
* OdDbArc
* OdDbCircle
* Ellipse
* OdDbSpline
* OdDb2dPolyline
* non-spline fit OdDb3dPolyline

    \param taperAngle [in] Extrusion taper angle in radians. Default value is 0.0. 
    Value must be between 0.5 pi and -0.5 pi. 
    If the path represented by the OdDbCurve object is closed, the taper angle parameter is ignored.
    \param isSolid [in] Boolean flag that indicates that the ModelerGeometry object is a solid.
    
    \returns Returns eOk if extruding operation is successful or an appropriate error code if not.

    \remarks
    Entity path must not have self-intersections, and it is recommended that the path not have high curvature areas. 
    The following constraints are applied: |taper Angle| >= 1e-6
  */
  virtual OdResult extrudeAlongPath(const OdDbRegion* region, const OdDbCurve* path, double taperAngle = 0.0, bool isSolid = true) = 0;
  
  /** \details
    Intersects the given entity with this ModelerGeometry object and imprints their intersection graph 
    onto the ModelerGeometry object. 

    \param pEntity [in] A pointer to an entity to be imprinted.

    \returns Returns eOk if imprinting entity onto the ModelerGeometry object is successful or an appropriate error code if not. 
  
    \remarks
    The given entity must be one of the following:
* Curve
* OdDb3dSolid
* Body
* Region
* Trace
* OdDbSolid
* Face

    If the ModelerGeometry object is null, or if the given entity is not one of the entities listed 
    above, eInvalidInput is returned.
    If as a result of intersection a closed loop of new edges is created, a new face will be made. 
    If an open loop of edges is made, it can be added to an existing loop as a spur. 
    Or it can be added as a slit in the face. 
  */
  virtual OdResult imprintEntity(const OdDbEntity *pEntity) = 0;
  
  /** \details
    Checks whether the solid interferes with another solid object.

    \param otherSolid         [in] A pointer to another solid object for checking interference.
    \param createNewSolid     [in] A flag determining where a new solid object containing the interference must be created.
    \param solidsInterfere    [out] A reference to a boolean variable where the result of the interference checking will be stored and returned.
    \param commonVolumeSolid  [out] A reference to a pointer to a new solid containing the interference of the origin solid and otherSolid. 

    \returns Returns eOk if interference checking is successful or an appropriate error code in other case.

    \remarks
    The caller of this function is responsible for the memory used by the object pointed to by commonVolumeSolid (resulting from intersection of this solid and otherSolid) when the function finishes. 
    If commonVolumeSolid is added to a database, the database takes responsibility for the memory pointed to by commonVolumeSolid. 
    If commonVolumeSolid is not added to a database, the caller must delete memory when it is no longer needed to avoid a memory leak.
  */
  virtual OdResult checkInterference(const OdDb3dSolid* otherSolid,
    bool createNewSolid, 
    bool& solidsInterfere,
    OdDb3dSolidPtr &commonVolumeSolid)
    const = 0;
  
  /** \details
    Returns volume properties of the ModelerGeometry object.

    \param volume         [out] Reference for returning the volume of the ModelerGeometry object.
    \param centroid       [out] Reference to an OdGePoint3d object for returning the point, representing the centroid of the ModelerGeometry object.
    \param momInertia     [out] A static array for returning X, Y, Z moments of inertia of the ModelerGeometry object.
    \param prodInertia    [out] A static array for returning X, Y, Z products of inertia of the ModelerGeometry object. 
    \param prinMoments    [out] A static array for returning X, Y, Z principal moments of the ModelerGeometry object.
    \param prinAxes       [out] A static array for returning an OdGeVector3d object, determining principal axes of the ModelerGeometry object.
    \param radiiGyration  [out] A static array for returning X, Y, Z radii of gyration of the ModelerGeometry object.
    \param extents        [out] A reference to an OdGeExtents3d object for returning the bounding box of the ModelerGeometry object.

    \returns Returns the result of getting volume properties of the ModelerGeometry object: 
    eOk if successful or an appropriate error code in other case. All properties are evaluated in the WCS.
  */
  virtual OdResult getMassProp(double& volume,
    OdGePoint3d& centroid,
    double momInertia[3],
    double prodInertia[3],
    double prinMoments[3],
    OdGeVector3d prinAxes[3], 
    double radiiGyration[3],
    OdGeExtents3d& extents) const = 0;
  
  /** \details
    Creates a DbRegion entity as a result of the intersection between the specified plane and the ModelerGeometry object. 

    \param plane          [in] Reference to an OdGePlane object to use as the section cutting plane.
    \param sectionRegion  [out] Reference to a new region created as the result of the intersection between the specified plane and the ModelerGeometry object.
    \param pSectionCurves [out] Pointer to an array of section curves.
 
    \returns Returns the result of intersecting the ModelerGeometry object with a 
    plane: eOk if successful or an appropriate error code in other case. 
    Value eOk is also returned when a plane and the ModelerGeometry object do not 
    intersect each other or if the intersection region is not a valid region, that is, 
    it intersects at a point, along a bounding face of the ModelerGeometry object and so on. 
  */
  virtual OdResult getSection(const OdGePlane& plane, OdDbRegionPtr &sectionRegion, OdDbEntityPtrArray* pSectionCurves = NULL) const = 0;
  
  /** \details
    Slices the ModelerGeometry object with a plane and transforms the native ModelerGeometry object
    to be the portion remaining on the positive side of the normal of the slicing plane.

    \param plane         [in] Reference to a slicing plane.
    \param getNegHalfToo [in] Flag determining whether the other (negative) part of the native ModelerGeometry object should be created as a separate ModelerGeometry object (when true).
    \param negHalfSolid  [out] Reference to a pointer to the created ModelerGeometry object representing the negative half of the native ModelerGeometry object. If getNegHalfToo is false, NULL value is returned.

    \returns Returns eOk if the native ModelerGeometry object was successfully sliced or an appropriate error code in other case. 
    If the slicing plane does not intersect the ModelerGeometry object, the ModelerGeometry object is left unchanged, negHalfSolid is set to NULL, and eOk is returned. 

    \remarks
    The caller of this function is responsible for the memory used by the object pointed to by negHalfSolid when the function finishes. 
    If negHalfSolid is added to a database, the database takes responsibility for the memory. 
    If negHalfSolid is not added to a database, the caller must delete memory when it is no longer needed to avoid a memory leak.
  */
  virtual OdResult getSlice(const OdGePlane& plane, bool getNegHalfToo, OdDb3dSolidPtr &negHalfSolid) = 0;
  
  /** \details
    Slices the ModelerGeometry object with a surface and transforms the native ModelerGeometry object to be the portion remaining on the positive side of the slicing surface normal.

    \param pSurface       [in] Pointer to a slicing surface.
    \param bGetNegHalfToo [in] Flag determining whether the other (negative) part of the native ModelerGeometry object should be created as a separate ModelerGeometry object (when true).
    \param pNegHalfSolid  [out] Reference to a pointer to the created ModelerGeometry object representing the negative half of the native ModelerGeometry object. If bGetNegHalfToo is false, NULL value is returned.

    \returns Returns eOk if the native ModelerGeometry object was successfully sliced or an appropriate error code in other case. 
    If pSurface pointer is NULL or the ModelerGeometry object is empty, returns eInvalidInput.
    
    \remarks
    The native ModelerGeometry object will be transformed to a resulting body which is located in the negative side of the surface normal.	
    
    The caller of this function is responsible for the memory used by the object pointed to by pNegHalfSolid when the function finishes. 
    If pNegHalfSolid is added to a database, the database takes responsibility for the memory. 
    If pNegHalfSolid is not added to a database, the caller must delete memory when it is no longer needed to avoid a memory leak.
  */
  virtual OdResult getSlice(OdDbSurface* pSurface, bool bGetNegHalfToo, OdDb3dSolidPtr& pNegHalfSolid) = 0;
 
  /** \details
    Removes all edges and faces that are not needed for supporting the topology of the ModelerGeometry object. 

    \returns Returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult cleanBody() = 0;
  
  /** \details
    Offsets all faces by the specified distance. 

    \param offsetDistance) [in] A distance for offset.

    \returns Returns eOk if all faces are successfully offset, or an appropriate error code in other case. 
    Returns eInvalidInput if the offsetDistance is less than -maxSide/2, 
    where maxSide is the size of the greater side of the ModelerGeometry object's bounding box. 
    Also, eInvalidInput is returned when the absolute value of offsetDistance is less than 1e-6.
    
    \remarks
    If a face has surfaces that cannot be offset, it is removed. Therefore, the offset operation 
    can cause the ModelerGeometry object to become invalid. 
  */
  virtual OdResult offsetBody(double offsetDistance) = 0;
  //virtual OdResult offsetBody(OdDbEntity* pOffsetEnt, double offsetDistance) = 0;
  /** \details
    Separates the ModelerGeometry object into an array of solids representing the additional disjoint volumes. 
    This ModelerGeometry object is reduced to a solid with one volume. 

    \param newSolids [out] Reference to an array of pointers to OdDb3dSolid objects representing the additional disjoint volumes. 

    \returns Returns eOk if separating was successful or an appropriate error code in other case.

    \remarks
    The calling application is responsible for the resulting entities (either appending them to a database or deleting them when they are no longer needed). 
    If the new solids from the output array are not appended to the database, there will be a memory leak. 
  */
  virtual OdResult separateBody(OdDb3dSolidPtrArray &newSolids) = 0;
  /** \details
    Creates an object by sweeping a planar curve, region, or planar surface in the direction of the given vector using a distance equal to the length of the vector.

    \param pSweepEnt    [in] A pointer to the swept entity.
    \param directionVec [in] Vector determining the direction and length of extruding.
    \param sweepOptions [in] A set of sweeping options.
    \param isSolid [in] Boolean flag that indicates that this ModelerGeometry object is solid.
    
    \returns Returns eOk if extruding was successful or an appropriate error code in other case.
    
    \remarks
    Sweeping parameters (for example, angle) are set through the sweepOptions object reference. 
  */
  virtual OdResult createExtrudedObject ( OdDbEntity* pSweepEnt,
                                          const OdGeVector3d& directionVec,
                                          OdDbSweepOptions& sweepOptions,
                                          bool isSolid = true,
                                          bool bHistoryEnabled = false) = 0;
  /** \details
    Creates a lofted object from the given curves.

    \param crossSectionCurves [in] Array of curve entities to be used as cross sections for the lofting operation.
    \param guideCurves [in] Array of guide curves.
    \param pPathCurve [in] Path curve.
    \param loftOptions [in] Options for lofting.
    \param isSolid [in] Boolean flag.

    \returns Returns eOk if successful or an appropriate error code in the other case.

    \remarks
    Lofting parameters can be passed with the loftOptions parameter. 
  */
  virtual OdResult createLoftedObject (
    OdDbEntityPtrArray& crossSectionCurves,
    OdDbEntityPtrArray& guideCurves,
    OdDbEntity* pPathCurve, 
    OdDbLoftOptions& loftOptions,
    bool isSolid = true) = 0;
  
  /** \details
  Creates a blend object from the given curves.

  \param crossSectionCurves [in] Array of curve entities to be used as cross sections for the lofting operation.
  \param guideCurves [in] Array of guide curves.
  \param pPathCurve [in] Path curve.
  \param loftOptions [in] Options for lofting.
  \param isSolid [in] Boolean flag.

  \returns Returns eOk if successful or an appropriate error code in the other case.

  \remarks
  Lofting parameters can be passed with the loftOptions parameter.
  */
  virtual OdResult createBlendObject(
    OdDbEntityPtrArray& blendedEdges, 
    OdDbBlendOptions& blendOptions,
    bool isSolid = true) = 0;
  /** \details
  Creates a lofted object from the given curves by coedges.

  \param crossSectionCurves [in] Array of curve entities to be used as cross sections for the lofting operation.
  \param guideCurves [in] Array of guide curves.
  \param pPathCurve [in] Path curve.
  \param loftOptions [in] Options for lofting.
  \param isSolid [in] Boolean flag.

  \returns Returns eOk if successful or an appropriate error code in the other case.

  \remarks
  Lofting parameters can be passed with the loftOptions parameter.
  */
  //virtual OdResult createLoftedObjectByCoedges(
  //  OdDbEntityPtrArray& crossSectionCurves,
  //  OdDbEntityPtrArray& guideCurves,
  //  OdDbEntity* pPathCurve,
  //  OdDbLoftOptions& loftOptions,
  //  bool isSolid = true) = 0;

  /** \details
    Creates a pyramid. World origin is centered about the base of the pyramid and the pyramid is positioned at half of the specified height above the world origin.

    \param height     [in] Value of the pyramid's height.
    \param sides      [in] Quantity of sides for the pyramid.
    \param radius     [in] Radius for circle inscribed into the bottom polygon.
    \param topRadius  [in] Radius for circle inscribed into the top polygon. 

    \returns Returns eOk if successful or an appropriate error code in the other case. 
    
    \remarks 
    The pyramid's base is located in the X-Y plane of the WCS. The pyramid's direction (from the base to the top) coincides with the positive direction of the Z-axis.
    When radius and topRadius values are equal, a prism is created. 

    The following constraints are applied:
* |height| >= 1e-6;
* |radius| >= 1e-6;
* |topRadius| >= 1e-6;
* 3 <= sides <= 64.
  */
  virtual OdResult createPyramid(double height, int sides, 
    double radius, double topRadius = 0.0 ) = 0;

  /** \details
    Creates an object by revolving a specified curve, region, or planar surface around the specified axis of revolution.

    \param pRevEnt        [in] Pointer to the planar curve, region, or planar surface that must be revolved. 
    \param axisPnt        [in] Point on the axis of revolution.
    \param axisDir        [in] Direction of the revolution axis.
    \param revAngle       [in] Revolve angle in radians.
    \param startAngle     [in] Start angle of rotation. If 0, rotation will start from the current position of pRevEnt.
    \param revolveOptions [in] Revolve options.
    \param isSolid        [in] Boolean flag.

    \returns Returns eOk if successful or an appropriate error code in the other case. 
    
    \remarks
    Axis of revolution is defined by a point (axisPnt) and vector (axisDir). 
    pRevEnt will be revolved according to the start and revolve angles counter-clockwise about the axisDir vector. 
    
    Options for revolving can be specified with the revolveOptions object reference.
  */
  virtual OdResult createRevolvedObject ( OdDbEntity* pRevEnt,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    double revAngle, double startAngle,
    OdDbRevolveOptions& revolveOptions,
    bool isSolid = true,
    bool bHistoryEnabled = false) = 0;

  /** \details
    Creates a swept object by sweeping a face along the specified path.

    \param pSweepEnt    [in] Pointer to the face entity for sweeping.
    \param pPathEnt     [in] Pointer to the curve entity that determines the path to sweep along.
    \param sweepOptions [in] Sweep options.
    \param isSolid      [in] Boolean flag.

    \returns Returns eOk if successful or an appropriate error code in the other case.
    
    \remarks
    To set optional sweep parameters (for example, draft angle) use the sweepOptions parameter. 
    The default value for the alignment option of OdDbSweepOptions (returned by align() method) is kNoAlignment. 
    Set it to kAlignSweepEntityToPath before calling this method.
  */
  virtual OdResult createSweptObject ( OdDbEntity* pSweepEnt,
    OdDbEntity* pPathEnt,
    OdDbSweepOptions& sweepOptions,
    bool isSolid = true,
    bool bHistoryEnabled = false) = 0;

  // Loft options utility functions
  /** \details
    Checks whether cross section curves are valid for the lofting operation. 

    \param crossSectionCurves   [in]  A list of cross-section curves.
    \param allOpen              [out] A passed-in parameter indicating whether all of the cross-section curves are open.
    \param allClosed            [out] A passed-in parameter indicating whether all of the cross-section curves are closed.
    \param allPlanar            [out] A passed-in parameter indicating whether all of the cross-section curves are planar.
    \param displayErrorMessages [in]  Indicates whether error messages are displayed. Default value is false.

    \returns Returns eOk if curves are valid or eInvalidInput otherwise.
  */
  virtual OdResult checkCrossSectionCurves (
    OdDbEntityPtrArray& crossSectionCurves,
    bool& allOpen, bool& allClosed, bool& allPlanar,
    bool displayErrorMessages = false ) = 0;
  
  /** \details
    Checks whether guide curves are valid for the lofting operation.

    \param guideCurves          [in] A list of guide curves.
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns Returns eOk if guide curves are valid or eInvalidInput otherwise.
  */
  virtual OdResult checkGuideCurves ( OdDbEntityPtrArray& guideCurves,
    bool displayErrorMessages = false ) = 0;
  
  /** \details
    Checks whether a path curve is valid for the lofting operation.

    \param pPathCurve           [in] A pointer to the path curve (can be NULL).
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns Returns eOk if a path curve is valid or eInvalidInput otherwise.
  */
  virtual OdResult checkPathCurve ( OdDbEntity *pPathCurve,
    bool displayErrorMessages = false ) = 0;

  // Sweep options utility functions
  /** \details
    Determines if the specified sweep entity is valid and returns the planarity of the sweep entity.

    \param pSweepEnt            [in]  A pointer to the curve, region, or planar surface to be swept.
    \param planarity            [out] A passed-in parameter for returning the planarity flag of the entity.
    \param pnt                  [out] A passed-in parameter for returning the point on the sweep entity if planarity is kPlanar, or a point on the line if planarity is kLinear.
    \param vec                  [out] A passed-in parameter for returning the normal vector of the entity if planarity is kPlanar, or the line direction if planarity is kLinear.
    \param closed               [out] A passed-in parameter for returning the closed flag determining whether an entity to be swept is a closed entity.
    \param approxArcLen         [out] A passed-in parameter for returning the approximate arc length.
    \param displayErrorMessages [in]  Indicates whether error messages are displayed. Default value is false.

    \returns If kPlanar is returned, then pnt and vec will specify the normal plane
    of the entity. If kLinear is returned, then pnt and vec will specify
    a defining point and direction of the sweep entity.
  */
  virtual OdResult  checkSweepCurve ( OdDbEntity *pSweepEnt,
    OdDb::Planarity& planarity, OdGePoint3d& pnt, OdGeVector3d& vec,
    bool& closed, double& approxArcLen, bool displayErrorMessages = false ) = 0;

  virtual OdResult  checkSweepPathCurve ( OdDbEntity *pPathEnt,    bool displayErrorMessages = false ) = 0;

  // Revolve options utility functions
  /** \details
    Checks whether a specified entity is valid for the revolve operation.

    \param pRevEnt              [in]  A pointer to the entity (curve or region) to be revolved.
    \param axisPnt              [in]  A point on the axis of revolution.
    \param axisDir              [in]  A vector determining the direction of the revolution axis.
    \param closed               [out] A passed-in parameter for returning the indicator determining whether the revolved entity is closed.
    \param endPointsOnAxis      [out] A passed-in parameter for returning the indicator determining whether the revolved entity is open and whether its endpoints lie on the axis of revolution.
    \param planar               [out] A passed-in parameter for returning the indicator determining whether the revolved entity is planar.
    \param displayErrorMessages [in]  Indicates whether error messages are displayed. Default value is false.

    \returns Returns eOk if a specified entity is valid for revolving, or eInvalidInput otherwise.
  */
  virtual OdResult checkRevolveCurve(
    OdDbEntity* pRevEnt,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    bool& closed,
    bool& endPointsOnAxis,
    bool& planar,
    bool displayErrorMessages = false) = 0;

  /** \details
    Creates an extruded surface when given a planar entity with thickness, 
    or creates a plane surface when given a simple, closed planar entity.

    \param pFromEntity [in]  Pointer to either a planar entity with thickness, a simple, closed planar entity, or an OdDbSolid entity 
    pointed to by pFromEntity. 

    \returns
    Returns eOk if successful, or an appropriate error code if not. 
  */
  virtual OdResult createFrom ( const OdDbEntity* pFromEntity ) = 0;

  /** \details
    Creates region (OdDbRegion class) entities for each face of the ModelerGeometry object. 

    \param regions [out]  Reference to an array of the OdDbRegion entities created from the face of the ModelerGeometry object.
  
    \returns Returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult convertToRegion ( OdDbEntityPtrArray& regions ) = 0;

  /** \details
    Creates a 3D solid object by thickening the surface by a specified amount. 
    
    \param thickness  [in] Amount to thicken the surface.
    \param bBothSides [in] Flag indicating whether the thickening should be applied to both sides of the surface.
    \param pSolid     [out] Reference to an OdDb3dSolid object pointer created during the thickening. If thickening fails, this parameter is NULL.
    
    \returns Returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult thicken(double thickness, bool bBothSides, OdDb3dSolidPtr& pSolid) const = 0;
  /** \details
    This method returns eOk if successful.

    \param ent [in]  An entity with which this entity is intersected.
    \param intType [in]  Type of intersection.
    \param points [out]  An array of points in which the intersection points should be appended.
    \param thisGsMarker [in]  GS marker of the subentity of this entity which is involved in the intersection.
    \param otherGsMarker [in]  GS marker of the subentity of the entity pointed to by pEnt which is involved in the intersection.
  */
  virtual OdResult intersectWith( const OdDbEntity* ent, OdDb::Intersect intType, 
    OdGePoint3dArray& points, OdGsMarker thisGsMarker, OdGsMarker otherGsMarker ) const = 0;

  /** \details
    This method returns eOk if successful.

    \param ent [in]  An entity with which this entity is intersected.
    \param intType [in]  Type of intersection.
    \param projPlane [in]  A projection plane for the intersection of the two entities.
    \param points [out]  An array of points in which the intersection points should be appended.
    \param thisGsMarker [in]  GS marker of the subentity of this entity which is involved in the intersection.
    \param otherGsMarker [in]  GS marker of the subentity of the entity pointed to by pEnt which is involved in the intersection.
  */
  virtual OdResult intersectWith( const OdDbEntity* ent, OdDb::Intersect intType, const OdGePlane& projPlane, 
    OdGePoint3dArray& points, OdGsMarker thisGsMarker, OdGsMarker otherGsMarker ) const = 0;

  virtual OdResult copySubEntity(const OdDbSubentId &subentId, OdDbEntityPtr &newEntity) const = 0;
  /** \details
    Tapers specified faces about a specified base point and draft vector for a specified draft angle.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be tapered.
    \param basePoint     [in] Reference to the base point of the draft plane.
    \param draftVector   [in] Reference to the draft direction vector.
    \param draftAngle    [in] Draft angle in radians.

    \returns Returns eOk if successful or an appropriate error code in the other case.

    \remarks
    The base point and draft vector define a draft plane about which faces are tapered. 
    If a face is lying in the draft plane, it will not be modified. 
  */
  virtual OdResult taperFaces(const OdArray<OdDbSubentId *> &faceSubentIds, const OdGePoint3d &basePoint, 
                              const OdGeVector3d &draftVector, double draftAngle) = 0;
  /** \details
    Removes the specified faces from this ModelerGeometry object.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be removed.

    \returns Returns eOk if successful or an appropriate error code in the other case.
    
    \remarks
    After removing the specified faces, the adjacent faces will enlarge to fill the gaps. 
  */
  virtual OdResult removeFaces(const OdArray<OdDbSubentId *> &faceSubentIds) = 0;

  /** \details
    Offsets the specified faces of this ModelerGeometry object for the specified distance.

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be offset.
    \param offsetDistance [in] Offset distance.

    \returns Returns eOk if successful or an appropriate error code in the other case.
    
    \remarks
    The following constraints are applied:
* number of specified faces (in faceSubentIds parameter) must be more than zero
* offsetDistance >= minus half of the max side of the ModelerGeometry object's bounding box
* |offsetDistance| >= 1e-6
  */
  virtual OdResult offsetFaces(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance) = 0;

  /** \details
    Transforms this ModelerGeometry object into a thin-walled ModelerGeometry object shell by offsetting faces. 

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be removed from the shell.
    \param offsetDistance [in] Offset distance for each face.

    \returns Returns eOk if successful or an appropriate error code in the other case.

    \remarks 
    Offset can have an outside or inside direction related to the ModelerGeometry object, depending on the sign of the offsetDistance parameter:
* positive value means outside direction
* negative value means inside direction
  */
  virtual OdResult shellBody(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance) = 0;

  /** \details
    Transforms the specified faces of a ModelerGeometry object by applying the transformation 
    matrix for rotation and (or) moving the faces. 

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be transformed.
    \param matrix         [in] Transformation matrix to be applied to the specified faces.

    \returns Returns eOk if successful or an appropriate error code in other case.	
  */
  virtual OdResult transformFaces(const OdArray<OdDbSubentId *> &faceSubentIds, const OdGeMatrix3d &matrix) = 0;

  /** \details
    This method returns eOk if successful.

    \param type [in]  A subentity type specified by the OdDb::SubentType enumeration.
    \param gsMark [in]  GS marker representing a specific subentity.
    \param pickPoint [in]  WCS coordinates of the point picked during the selection of the entity.
    \param viewXform [in]  A transformation matrix to transform from WCS to DCS.
    \param subentPaths [out]  An array of full subentity path objects identifying the subentities generated from the GS marker.
    \param entAndInsertStack [in]  An array of object IDs that are the nested containers of the subentity.
  */
  virtual OdResult getSubentPathsAtGsMarker(OdDb::SubentType type,
                                            OdGsMarker gsMark, 
                                            const OdGePoint3d& pickPoint,
                                            const OdGeMatrix3d& viewXform, 
                                            OdDbFullSubentPathArray& subentPaths, 
                                            const OdDbObjectIdArray* entAndInsertStack = 0) const = 0;
  /** \details
    Returns an array of GS markers. 

    \param subPath [in]  A full subentity path object identifying the subentity.
    \param gsMarkers [out]   An array with GSMarkers for the subentities.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath, 
                                            OdGsMarkerArray& gsMarkers) const = 0;
  /** \details
    For internal use only.
    
    \param ent [in] ACIS object pointer. 
  */
  virtual OdDbSubentId internalSubentId(void* ent) const = 0;

  /** \details
    For internal use only.
    
    \param id [out] Subentity ID in the ACIS object. 
  */
  virtual void* internalSubentPtr(const OdDbSubentId& id) const = 0;

  virtual bool getNurbCurvesCache(OdGeCurve3dPtrArray &)  const { return false; }

  // for internal use only
  virtual OdResult setBody(const void * /*pBody*/) { return eNotImplemented; }
  virtual void * body( ) const { return 0; }
  
  /** \details
    Sets the color of the specified face or edge subentity. 

    \param subentId [in] ID of the subentity (face or edge) to be colored.
    \param color    [in] Color value to set.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult setSubentColor(const OdDbSubentId &subentId, const OdCmColor &color) = 0;

  /** \details
    Returns the color of the specified subentity.

    \param subentId [in] ID of the subentity.
    \param color    [in] A reference to an OdCmColor object to store the current color of the specified subentity.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getSubentColor(const OdDbSubentId &subentId, OdCmColor &color) const = 0;
  
  /** \details
    Sets a material for the specified subentity.

    \param subentId [in] Subentity ID. 
    \param matId    [in] Object ID for the material.

    \returns Returns eOk if successful or an appropriate error code in the other case.
    
    \remarks
    To remove the currently assigned material, set matId parameter value to NULL.    
  */
  virtual OdResult setSubentMaterial(const OdDbSubentId &subentId, OdDbObjectId matId) = 0;

  /** \details
    Returns the material of the specified subentity.

    \param subentId [in] Subentity ID.
    \param matId    [in] Reference to an OdDbObjectId for returning the material object ID.
    
    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getSubentMaterial(const OdDbSubentId &subentId, OdUInt64& matId) const = 0;

  virtual OdResult setSubentMaterialMapper( const OdDbSubentId &subentId, 
                                            OdGeMatrix3d &mx, 
                                            OdUInt8 &projection, 
                                            OdUInt8 &tiling, 
                                            OdUInt8 &autoTransform) = 0;
  virtual OdResult getSubentMaterialMapper( const OdDbSubentId &subentId,
                                            OdGeMatrix3d &mx, 
                                            OdUInt8 &projection, 
                                            OdUInt8 &tiling, 
                                            OdUInt8 &autoTransform) const = 0;
  /** \details
    Creates chamfers at the edges of a ModelerGeometry object.

    \param edgeSubentIds    [in] Array of edge object IDs. The chamfer will be applied at the specified edges.
    \param baseFaceSubentId [in] Base face object IDs. Chamfer will be applied on the specified face.
    \param baseDist         [in] Chamfer distance for the base face.
    \param otherDist        [in] Chamfer distance for the other faces.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult chamferEdges(const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdDbSubentId& baseFaceSubentId, 
                                double baseDist, 
                                double otherDist) = 0;
  /** \details
    Creates rounded fillets on the edges of a ModelerGeometry object.

    \param edgeSubentIds [in] Array of edges' object IDs. Fillet will be applied on the specified edges.
    \param radius        [in] Array of fillet radius values for the corresponding edges.
    \param startSetback  [in] Array of start setback values for the corresponding edges.
    \param endSetback    [in] Array of end setback values for the corresponding edges.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult filletEdges( const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdGeDoubleArray& radius, 
                                const OdGeDoubleArray& startSetback, 
                                const OdGeDoubleArray& endSetback) = 0;

  /** \details
    Intersects the specified plane with this ModelerGeometry object and creates the intersection in a resulting array. 
 
    \param sectionPlane   [in]  Pointer to a plane object used for intersection (section cutting plane).
    \param sectionObjects [out] A reference to an entity array for representing resulting sections.
    
    \returns 
    Returns eOk if the intersection operation is successfully performed, or an appropriate error code otherwise. 
  */
  virtual OdResult createSectionObjects(const OdGePlane& sectionPlane, OdDbEntityPtrArray& sectionObjects) const = 0;

  /** \details
    Creates new entities that represent the result of interference between this ModelerGeometry object and a specified entity. 
    Created entities are appended to the specified array.

    \param interferenceObjects [out]  Output array of interference entities.
    \param pEntity             [in]   Pointer to an entity the object must interfere with. pEntity can point to the following entity types: OdDbSurface, OdDb3dSolid, OdDbRegion, or OdDbBody.
    \param flags               [in]   Interference flags; equal to kDefault (0x00).

    \returns Returns eOk if interference entities are successfully created or an appropriate error code otherwise. 
    For example, if interference entities are not created, eNoIntersection error code is returned.
    eInvalidInput is returned if an unsupported entity or NULL pointer is provided as a value of pEntity.  
  */
  virtual OdResult createInterferenceObjects( OdDbEntityPtrArray& interferenceObjects,
                                              OdDbEntityPtr pEntity, 
                                              unsigned int flags ) const = 0;
  /** \details
    Slices this ModelerGeometry object with the specified plane and sets the object to be the portion on the positive normal side plane. 
    
    \param slicePlane      [in]   Reference to a plane object to be used for slicing the ModelerGeometry object.
    \param pNegHalfSurface [out]  Pointer to a new surface representing the negative side of the object that is sliced by the specified plane.
    \param pNewSurface     [out]  Pointer to a new surface representing the positive half of the object that is sliced by the specified plane.
    \param bNotModifyItself [in]  Boolean flag.
    
    \returns Returns eOk if the slicing operation is successfully performed or an appropriate error code otherwise. 
  */
  virtual OdResult sliceByPlane(const OdGePlane& slicePlane, OdDbSurfacePtr& pNegHalfSurface, OdDbSurfacePtr& pNewSurface, bool bNotModifyItself) = 0;
  
  /** \details
    Slices this ModelerGeometry object with the specified surface. 

    \param pSlicingSurface [in]  Pointer to the surface to be used to slice this object. 
    \param pNegHalfSurface [out] Pointer to a new surface object representing the other (negative) side of the sliced surface.
    \param pNewSurface     [out] Pointer to a new surface object representing the positive side of the sliced surface.
    \param bNotModifyItself [in] Boolean flag.
    
    \returns Returns eOk if the slicing operation is successfully performed or an appropriate error code otherwise. 
  */
  virtual OdResult sliceBySurface(const OdDbSurfacePtr pSlicingSurface, OdDbSurfacePtr& pNegHalfSurface, OdDbSurfacePtr& pNewSurface, bool bNotModifyItself) = 0;

  /** \details
    Extrudes faces of the ModelerGeometry object with specified distance and taper angle.

    \param faceSubentIds  [in] Face's subentity ID in the drawing object.
    \param height         [in] Distance of extrusion.
    \param taper          [in] Taper angle.
  
    \returns Returns eOk if successful or an appropriate error code in the other case.
    
    \remarks  
    The distance of extrusion is determined by the height parameter and the taper angle (the taper parameter). 
  
    The extrusion direction can be along or opposite the normal of each extruded face depending on the height value: 
* If height is positive, direction is along the normal (extruding faces outward from the drawing object).
* If height is negative, direction is opposite the normal (extruding faces into the drawing object).
  
    If the absolute value of taper is less than 1e-6, taper angle is assumed to be equal to 0.0.

    If taper value does not equal 0.0, each face should have edges that join together smoothly (i.e. tangents at the points of connection must be equal). 
  
    The following constraints are applied:
* number of specified faces (in faceSubentIds parameter) must be more than zero
* |height| >= 1e-6
* |taper| >= (PI/2) - 1e-6
  */
  virtual OdResult extrudeFaces(const OdArray<OdDbSubentId *> &faceSubentIds, double height, double taper) = 0;
  
  /** \details
    Extrudes specified faces along the specified path determined by a curve entity. 

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be extruded.
    \param path          [in] Pointer to a curve object to extrude along (extrusion path). Can not be NULL.

    \returns Returns eOk if successful or an appropriate error code in the other case.

    Faces for extrusion must be planar.
    
    Curve entity must be one of the following:
* OdDbLine
* OdDbArc
* OdDbEllipse
* OdDbSpline
* OdDb2dPolyline
* Non-spline fit OdDb3dPolyline

    Extrusion path must not have self-intersections. Also it is recommended that the path doesn't have high curvature areas. 
    If one of the end points of the path is not located on the region's plane, the path will move the point on the plane. 
  */
  virtual OdResult extrudeFacesAlongPath(const OdArray<OdDbSubentId *> &faceSubentIds, const OdDbCurve* path) = 0;
  
  virtual OdResult ChangeFacesDoubleSidedParam(bool isDoubleSided) = 0;

  /** \details
    Converts this ModelerGeometry object to nurb surfaces.
    
    \param nurbSurfaceArray [in] Array of generated nurb surfaces.
    
    \returns Returns eOk if converting operation was successfully finished or an appropriate error code otherwise. 
  */
  virtual OdResult convertToNurbSurface(OdDbNurbSurfaceArray& nurbSurfaceArray) = 0;

  virtual OdResult get( int& iUDegree, int& iVDegree, bool& bRational, int& iUNumControlPoints, int& iVNumControlPoints,
                        OdGePoint3dArray& ctrlPtsArr, OdGeDoubleArray& weights, 
                        OdGeKnotVector& uKnots, OdGeKnotVector& vKnots) const = 0;

  virtual OdResult set (int iUDegree, int iVDegree, bool bRational, int iUNumControlPoints, int iVNumControlPoints,
                        const OdGePoint3dArray& ctrlPtsArr, const OdGeDoubleArray& weights, 
                        const OdGeKnotVector& uKnots, const OdGeKnotVector& vKnots) = 0;
  /** \details
    Returns the quantity of control points in the u direction.

    \param iCount [out] A passed-in parameter for returning the quantity of control points in the u direction.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfControlPointsInU(int& iCount) const = 0;
  
  /** \details
    Returns the quantity of control points in the v direction.

    \param iCount [out] A passed-in parameter for returning the quantity of control points in the v direction.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfControlPointsInV(int& iCount) const = 0;

  /** \details
    Returns the quantity of knots in the u direction.

    \param iCount [out] A passed-in parameter for returning the quantity of knots in the u direction.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfKnotsInU(int& iCount) const = 0;
  
  /** \details
    Returns the quantity of knots in the v direction.

    \param iCount [out] A passed-in parameter for returning the quantity of knots in the v direction.

  \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfKnotsInV(int& iCount) const = 0;

  /** \details
    Returns the knot vector in the u direction.

    \param knots [out] A passed-in parameter for returning the knot vector in the u direction.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getUKnots(OdGeKnotVector& knots) const = 0;

  /** \details
    Returns the knot vector in the v direction.

    \param knots [out] A passed-in parameter for returning the knot vector in the v direction.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getVKnots(OdGeKnotVector& knots) const = 0;

  /** \details
    Returns the degree of the ModelerGeometry object in the u direction.

    \param iDegree [out] A passed-in parameter for returning the degree in the u direction.

    \returns Returns eOk if successful. 
  */
  virtual OdResult getDegreeInU(int& iDegree) const = 0;
  
  /** \details
    Returns the degree of the ModelerGeometry object in the v direction.

    \param iDegree [out] A passed-in parameter for returning the degree in the v direction.

    \returns Returns eOk if successful.
  */
  virtual OdResult getDegreeInV(int& iDegree) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is closed in the u direction.

    \param bIsClosed [out] A passed-in parameter for returning the indicator of whether the ModelerGeometry object is closed in the u direction. 
    bIsClosed is equal to true if the ModelerGeometry object is closed in the u direction, otherwise it is equal to false.

    \returns Returns eOk if successful.
  */
  virtual OdResult isClosedInU(bool& bIsClosed) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is closed in the v direction.

    \param bIsClosed [out] A passed-in parameter for returning the indicator of whether the ModelerGeometry object is closed in the v direction. 
    bIsClosed is equal to true if the ModelerGeometry object is closed in the v direction, otherwise it is equal to false.

    \returns Returns eOk if successful.
  */
  virtual OdResult isClosedInV(bool& bIsClosed) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is periodic in the u direction.

    \param bIsPeriodic [out] A passed-in parameter for returning the indicator of whether the ModelerGeometry object is periodic in the u direction. 
    bIsPeriodic is equal to true if the ModelerGeometry object is periodic in the u direction, otherwise it is equal to false.

    \returns Returns eOk if successful.
  */
  virtual OdResult isPeriodicInU(bool& bIsPeriodic) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is periodic in the v direction.

    \param bIsPeriodic [out] A passed-in parameter for returning the indicator of whether the ModelerGeometry object is periodic in the v direction. 
    bIsPeriodic is equal to true if the ModelerGeometry object is periodic in the v direction, otherwise it is equal to false.

    \returns Returns eOk if successful.
  */
  virtual OdResult isPeriodicInV(bool& bIsPeriodic) const = 0;

  /** \details
    Returns the period value in the u direction for the ModelerGeometry object.
    Returned value is valid only if the ModelerGeometry object is periodic in the u direction.

    \param dPeriod [out] A passed-in parameter for returning the period value in the u direction.

    \returns Returns eOk if successful. 
  */
  virtual OdResult getPeriodInU(double& dPeriod) const = 0;
  /** \details
    Returns the period value in the v direction for the ModelerGeometry object.
    Returned value is valid only if the ModelerGeometry object is periodic in the v direction.

    \param dPeriod [out] A passed-in parameter for returning the period value in the v direction.

    \returns Returns eOk if successful.
  */
  virtual OdResult getPeriodInV(double& dPeriod) const = 0;

  /** \details
    Evaluates the position on the ModelerGeometry object at the specified parameter value (u, v).
  
    \param dU   [in]  u direction parameter. 
    \param dV   [in]  v direction parameter.
    \param pos  [out] A passed-in parameter for returning the evaluated position on the ModelerGeometry object.
  
    \returns Returns eOk if evaluation is successfully done. 
    If the u or v value is out of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, OdGePoint3d& pos) const = 0;

  /** \details
    Evaluates the position and first derivatives on the ModelerGeometry object at the specified parameter value (u, v).

    \param dU     [in]  u direction parameter. 
    \param dV     [in]  v direction parameter.
    \param pos    [out] A passed-in parameter for returning the evaluated position on the ModelerGeometry object.
    \param uDeriv [out] A passed-in parameter for returning the first derivative with respect to u.
    \param vDeriv [out] A passed-in parameter for returning the first derivative with respect to v.

    \returns 
    Returns eOk if evaluation is successfully done. 
    If the u or v value is out of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, OdGePoint3d& pos, OdGeVector3d& uDeriv, OdGeVector3d& vDeriv) const = 0;

  /** \details
    Evaluates the position and first and second derivatives on the ModelerGeometry object at the specified parameter value (u, v).

    \param dU       [in]  u direction parameter. 
    \param dV       [in]  v direction parameter.
    \param pos      [out] A passed-in parameter for returning the evaluated position on the surface.
    \param uDeriv   [out] A passed-in parameter for returning the first derivative with respect to u.
    \param vDeriv   [out] A passed-in parameter for returning the first derivative with respect to v.
    \param uuDeriv  [out] A passed-in parameter for returning the second derivative with respect to u twice.
    \param uvDeriv  [out] A passed-in parameter for returning the second derivative with respect to u and v. 
    \param vvDeriv  [out] A passed-in parameter for returning the second derivative with respect to v twice. 

    \returns Returns eOk if evaluation is successfully done. 
    If the u or v value is out of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, OdGePoint3d& pos, OdGeVector3d& uDeriv, OdGeVector3d& vDeriv,
                            OdGeVector3d& uuDeriv, OdGeVector3d& uvDeriv, OdGeVector3d& vvDeriv) const = 0;

  /** \details
    Evaluates the position and an arbitrary number of derivatives of the ModelerGeometry object at the specified parameter value (u, v).

    \param dU           [in]  u direction parameter. 
    \param dV           [in]  v direction parameter.
    \param iDerivDegree [in]  The degree of derivatives to evaluate.
    \param point        [out] A passed-in parameter for returning the evaluated position on the ModelerGeometry object.
    \param derivatives  [out] A passed-in parameter for returning derivatives array of vectors, in the order of uDeriv, vDeriv, uuDeriv, uvDeriv, vvDeriv.

    \returns Returns eOk if evaluation is successfully done. 
    If the u or v value is out of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, int iDerivDegree, OdGePoint3d& point, OdGeVector3dArray& derivatives) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is rational.
    If the ModelerGeometry object is rational, the method returns true, otherwise the method returns false.

    \param bIsRational [out] A passed-in parameter for returning the indicator of whether the ModelerGeometry object is rational.

    \returns Returns eOk if successful.
  */
  virtual OdResult isRational(bool& bIsRational) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is a planar surface. If the surface is planar, also returns the plane information and normal.
    If the surface is planar, the method returns true via the bIsPlanar parameter, otherwise the method returns false.

    \param bIsPlanar    [out] A passed-in parameter for returning the indicator of whether the surface is planar.
    \param ptOnSurface  [out] A passed-in parameter for returning the point on the surface (if the surface is planar).
    \param normal       [out] A passed-in parameter for returning the plane normal (if the surface is planar).

    \returns Returns eOk if successful. If the surface doesn't contain any NURBS information, returns eFail. 
  */
  virtual OdResult isPlanar(bool& bIsPlanar, OdGePoint3d& ptOnSurface, OdGeVector3d& normal) const = 0;

  /** \details
    Determines whether a specified point is on the ModelerGeometry object.
    If the specified point is on the ModelerGeometry object, the method returns true via bOnSurface, otherwise the method returns false.

    \param point      [in] A point.
    \param bOnSurface [out] A passed-in parameter for returning the indicator of whether the specified point is on the surface.

    \returns Returns eOk if successful.
  */
  virtual OdResult isPointOnSurface(const OdGePoint3d& point, bool& bOnSurface) const = 0;

  /** \details
    Returns the normal vector at the specified parameter location ([u, v]).

    \param dU     [in]  u parameter.
    \param dV     [in]  v parameter.
    \param normal [out] A passed-in parameter for returning the normal vector.

    \returns Returns eOk if successful. If the dU or dV values are invalid, returns eOutOfRange. 
  */
  virtual OdResult getNormal(double dU, double dV, OdGeVector3d& normal) const = 0;

  /** \details
    Returns the simple patches in the u direction.

    \param span [out] A passed-in parameter for returning the number of simple patches in the u direction.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getNumberOfSpansInU(int& iSpan) const = 0;
  
  /** \details
    Returns the simple patches in the v direction.

    \param span [out] A passed-in parameter for returning the number of simple patches in the v direction.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getNumberOfSpansInV(int& iSpan) const = 0;

  /** \details
    Returns the specified number of isolines created in the v direction. 

    \param dNumberSegments [in] Number of isoline segments.
    \param lineSegments [out] Array of isoline segments.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getIsolineAtU(double dNumberSegments, OdDbCurvePtrArray& lineSegments) const = 0;

  /** \details
    Returns the specified number of isolines created in the u direction. 

    \param dNumberSegments [in] Number of isoline segments.
    \param lineSegments [out] Array of isoline segments.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getIsolineAtV(double dNumberSegments, OdDbCurvePtrArray& lineSegments) const = 0;

  /** \details
    Inserts a knot at the specified u or v parameter.
    The knot insertion doesn't modify the shape of the ModelerGeometry object. 

    \param dVal [in] New knot.
    \param iUorV [in] u or v parameter.
  
    \returns Returns eOk if successful or an appropriate error code in the other case.

  */
  virtual OdResult InsertKnot(double dVal, int iUorV) = 0;

  /** \details
    Inserts a row of control points at the given u knot parameter.

    \param dU [in] u parameter.
    \param vCtrlPts  [in] An array of control points to be added.
    \param vWeights  [in] Array of weight values to add if the surface is rational.

    \returns Returns eOk if successful. 
    If the dU parameter is out of range, returns eOutOfRange.
  */
  virtual OdResult InsertControlPointsAtU(double dU, const OdGePoint3dArray& vCtrlPts, const OdGeDoubleArray& vWeights) = 0;

  /** \details
    Inserts a row of control points at the given v knot parameter.

    \param dU [in] v parameter.
    \param vCtrlPts  [in] An array of control points to be added.
    \param vWeights  [in] Array of weight values to add if the surface is rational.

    \returns Returns eOk if successful. 
    If the dU parameter is out of range, returns eOutOfRange.
  */
  virtual OdResult InsertControlPointsAtV(double dV, const OdGePoint3dArray& uCtrlPts, const OdGeDoubleArray& uWeights) = 0;

  /** \details
    Removes a row of control points at the specified position in the control points array.
    The method modifies the shape of the ModelerGeometry object.

    \param iUDegree [in] An index of the control point to remove from the array.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult RemoveControlPointsAtU(int iUDegree) = 0;
  
  /** \details
    Removes a row of control points at the specified position in the control points array.
    The method modifies the shape of the ModelerGeometry object.

    \param iVDegree [in] An index of the control point to remove from the array.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult RemoveControlPointsAtV(int iVDegree) = 0;
  
  /** \details
    Rebuilds this ModelerGeometry object with a specified new degree and quantity of control points in the u and v directions.
    This operation modifies the shape of the ModelerGeometry object.

    \param iUDegree     [in] The new degree value in the u direction.
    \param iVDegree     [in] The new degree value in the v direction.
    \param iNumUCtrlPts [in] The new quantity of control points in the u direction.
    \param iNumVCtrlPts [in] The new quantity of control points in the v direction.
    \param bRestore     [in] Restore flag.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult rebuild(int iUDegree, int iVDegree, int iNumUCtrlPts, int iNumVCtrlPts, bool bRestore) = 0;

  /** \details
    Adjusts the location and tangent of a point on this ModelerGeometry object with the specified parameter ([u, v]). 
    The local control points are adjusted accordingly. 
    If the pointer uDeriv and vDeriv are equal to NULL, then only the location of the point is adjusted.

    \param dU     [in] u parameter.
    \param dV     [in] v parameter.
    \param point  [in] A new location of the point on the surface.
    \param uDeriv [in] The tangent vector in the u direction.
    \param vDeriv [in] The tangent vector in the v direction.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult modifyPositionAndTangent(double dU, double dV, const OdGePoint3d& point, const OdGeVector3d* uDeriv, const OdGeVector3d* vDeriv) = 0;

  /** \details
    Returns the u and v parameter for a point on this ModelerGeometry object.

    \param point [in] A point on the ModelerGeometry object.
    \param dU    [in] A passed-in parameter for returning the value of the u parameter.
    \param dV    [in] A passed-in parameter for returning the value of the v parameter.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getParameterOfPoint(const OdGePoint3d& point, double& dU, double& dV) const = 0;

  /** \details
    Returns the control points and the quantity of control points in both u and v directions.

    \param iUCount  [out] The quantity of control points in the u direction.
    \param iVCount  [out] The quantity of control points in the v direction.
    \param points   [out] Array of control points (in WCS coordinates).

    \returns Returns eOk if control points were successfully returned, or an appropriate error code in the other case.

    \remarks
    The control points will be allocated in the passed-in OdGePoint3d array. 
    The column indexes are for the v direction, and row indexes are for the u direction. 
    For example, if the surface has n control points in the u direction and m control points in the v direction, 
    the array looks like [0,0], [0,1], ...[0,vCount-1], ..., [uCount-1,0], [uCount-1,1], ...[uCount-1,vCount-1]. 
  */
  virtual OdResult getControlPoints(int& iUCount, int& iVCount, OdGePoint3dArray& points) const = 0;

  /** \details
    Sets the control points. 

    \param iUCount [in] The new value of the quantity of control points in the u direction.
    \param iVCount [in] The new value of the quantity of control points in the v direction.
    \param points  [in] New array of control points (in WCS coordinates).

    \returns Returns eOk if control points were successfully set or an appropriate error code in the other case. 

    \remarks
    The new quantity of control points in the u and v direction must be equal to the current values. 
    New control points are allocated in an AcGePoint3d array. 
    The column indexes are for the v direction, and row indexes are for the u direction. 
    For example, if the surface has n control points in the u direction and m control points in the v direction, the array looks like [0,0], [0,1], ...[0,vCount-1], ..., [uCount-1,0], [uCount-1,1], ...[uCount-1,vCount-1].
  */
  virtual OdResult setControlPoints(int iUCount, int iVCount, const OdGePoint3dArray& points) = 0;

  virtual OdResult getControlPointAndWeight(int iUIndex, int iVIndex, OdGePoint3d& point, double& weight, bool& bIsRational) const = 0;

  virtual OdResult setControlPointAndWeight(int iUIndex, int iVIndex, const OdGePoint3d& point, double weight) = 0;

  /** \details
    Sets the new color for subentities. 

    \param color [in] New color value.

    \returns Returns eOk if the new color value was successfully set or an appropriate error code in the other case. 
  */
  virtual OdResult setColorToSubents(OdCmColor const& color) = 0;
  
  /** \details
    Sets the material. 

    \param materialId [in] New material.

    \returns Returns eOk if the new material value was successfully set or an appropriate error code in the other case. 
  */
  virtual OdResult setMaterialToSubents(OdDbObjectId materialId) = 0;

  virtual OdResult setMaterialResolver(const OdMaterialResolver *pResolver) = 0;

  virtual OdResult setMaterialMapperToSubents(OdGeMatrix3d &mx, OdUInt8 &projection,
                                              OdUInt8 &tiling, OdUInt8 &autoTransform) = 0;

  /** \details 
    For internal use only.
  */ 
  virtual OdResult generateSectionGeometry(const OdDbSection*      pSection,
                                           OdDbEntityPtrArray&     sourceEntArr,
                                           OdArray<OdDbEntityPtr>& intBoundaryEnts,
                                           OdArray<OdDbEntityPtr>& intFillEnts,
                                           OdArray<OdDbEntityPtr>& backgroundEnts,
                                           OdArray<OdDbEntityPtr>& foregroundEnts,
                                           OdArray<OdDbEntityPtr>& curveTangencyEnts,
                                           bool                    bIsSetProperties) = 0;
  /** \details
    Trims the surface with specified cutting curves. 
    When curves are used, the actual trimming boundary is formed by projecting 
    the curves onto the surface, so the additional information of project direction needs to be provided.

    \param toolIds            [in] Reference to an array of the cutting entity's IDs. Bodies of specified entities are used to trim the specified surface.
    \param toolCurveIds       [in] Reference to an array of the cutting curve's IDs. The specified surface will be trimmed by projecting curves to it.
    \param projVectors        [in] Reference to an array of projection direction for each cutting curve.
    \param pickPoint          [in] Pick point that specifies which area of the specified surface must be trimmed.
    \param viewVector         [in] Vector representing the view direction for creating a ray starting from the pick point.
    \param bAutoExtend        [in] If this option is set, then when a tool body consists of a single face with analytic geometry, the underlying geometry will be extended as much as possible to make sure the surface is trimmed. The tool body supplied will not be modified. 
    \param bAssociativeEnabled [in] Specifies whether the surface trimming operation should be associative.  
    
    \returns Returns eOk if trimming operation was successfully finished or an appropriate error code otherwise. 
  */
  virtual OdResult trimSurface( const OdDbObjectIdArray& toolIds,
                                const OdDbObjectIdArray& toolCurveIds, 
                                const OdGeVector3dArray& projVectors,
                                const OdGePoint3d&       pickPoint, 
                                const OdGeVector3d&      viewVector,
                                bool                     bAutoExtend,
                                bool                     bAssociativeEnabled) = 0;

  virtual OdResult projectOnToEntity( const OdDbEntity*   pEntityToProject,
                                      const OdGeVector3d& projectionDirection,
                                      OdDbEntityPtrArray& projectedEntities ) const = 0;

  /** \details
    Creates a solid by trimming and (or) extending the specified surfaces and 3D solids, forming an enclosed volume.

    \param limitingBodies [in] Reference to the entity array (representing surface and solids) that are used to create a solid.
    \params limitingFlags [in] Reserved for internal use.  

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult createSculptedSolid(OdDbEntityPtrArray& limitingBodies,
                                       const OdGeIntArray& limitingFlags) = 0;

  /** \details 
    Creates a ray which starts from a specified point and has a specified direction.

    \param rayBasePoint [in]  A base point to start a ray from.
    \param rayDir      [in]  A ray direction.
    \param rayRadius   [in]  A ray radius. It defines the tolerance used for intersection checking.
    \param subEntIds   [out] A reference to subentities array to return. Returned subentities can be one of the following: faces, edges, or vertices.
    \param parameters   [out] An array of parameters to return. Parameters shows the intersection between a ray and subentities.

    \returns Returns eOk if a ray was successfully created or an appropriate error code in the other case.
  */
  virtual OdResult rayTest( const OdGePoint3d&      rayBasePoint, 
                            const OdGeVector3d&     rayDir,
                            double                  rayRadius, 
                            OdArray<OdDbSubentId> & subEntIds, 
                            OdGeDoubleArray&        parameters) const = 0;

  /** \details 
    Creates a tangent surface between two edges of a surface with constant radius and trimming or no trimming of the original surfaces.

    \param surf1    [out] Pointer to a first source surface.
    \param pickPt1  [in]  3D point specifying the side on the first source surface that will be kept after filleting.
    \param surf2    [out] Pointer to a second source surface.
    \param pickPt2  [in] 3D point specifying the side on the second source surface that will be kept after filleting.
    \param dRadius  [in] Radius for fillet operation (positive value).
    \param trimMode [in] Flag that specifies the OdDb::FilletTrimMode value. 
    \param projDir  [in] 3D vector specifying the projection (view) direction for pick points.

    \returns Returns eOk if successful or an appropriate error code in the other case.
    
    \remarks     
    The trimMode can be one of the following:
* kTrimNone
* kTrimFirst
* kTrimSecond
* kTrimBoth
  */
  virtual OdResult createFilletSurface (OdDbSurfacePtr&      surf1,
                                        const OdGePoint3d&   pickPt1,
                                        OdDbSurfacePtr&      surf2,
                                        const OdGePoint3d&   pickPt2,
                                        double               dRadius,
                                        OdDb::FilletTrimMode trimMode,
                                        const OdGeVector3d&  projDir) = 0;

  /** \details 
    Extends the edges of this ModelerGeometry object.

    \param edgesId [out] Array of full sub-entity paths of the edges that need to be extended.
    \param dExtDist [in] Distance at which the ModelerGeometry object should be expanded.
    \param extOption [in] Extension option, if edges need to be extended or stretched.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult extendEdges( OdDbFullSubentPathArray& edgesId,
                                double                   dExtDist, 
                                EdgeExtensionType        extOption ) = 0;

  virtual OdResult getObjectMesh( const OdDbFaceterSettings *faceter,
                                  OdGePoint3dArray& vertexArray, 
                                  OdInt32Array& faceArray,
                                  OdGiFaceData*& faceData ) = 0;
                                  
                                  
  /** \details 
    Returns the bounding box.

    \param box [out] 3D bounding boxes as minimum and maximum 3D points.

    \returns Returns eOk if successful or an appropriate error code in the other case.
  */                                  
  virtual OdResult getBoundingBox(OdGeExtents3d &box) = 0;

  /** \details 
    For internal use only.
  */ 
  virtual OdResult generateSectionGeometry( SectArgs& sectArgs, OdDbEntity *pEnt, bool* bHasForeground ) = 0;

  virtual OdResult createCachedCurves(OdGeCurve3dPtrArray& pCurves) = 0;
  virtual OdResult restoreAttributes(OdModelerGeometry* oldmodeler) = 0;
  virtual OdResult addSubentitySpecialSettings(const OdDbSubentId subentId, OdArray<OdUInt32>& retArray, OdUInt32 idx) = 0;

  virtual OdResult convertTo(const OdGePoint3dArray &arrVertexes, const OdInt32Array &arrEdges, const OdInt32Array &arrFaces, OdGiFaceData &fd,
    OdInt32Array &arrFacesColors, OdInt32Array &arrFacesMaterials, OdDbEntity* pEntity) = 0;

  virtual bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) = 0;

  virtual OdResult auditAcisData(OdDbAuditInfo* pInfo, const OdRxObject* pObj) { return eNotImplemented; }

  virtual void enableAcisAudit(bool bEnable) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdModelerGeometry object pointers.
*/
typedef OdSmartPtr<OdModelerGeometry> OdModelerGeometryPtr;

#include "TD_PackPop.h"

#endif // _OD_MODELERGEOMETRY_INCLUDED_
