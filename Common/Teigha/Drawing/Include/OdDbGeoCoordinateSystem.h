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

#ifndef _OD_DB_GEOCOORDINATESYSTEM_
#define _OD_DB_GEOCOORDINATESYSTEM_

#include "TD_PackPush.h"
#include "Ge/GeExtents2d.h"
#include "DbDatabase.h"
#include "Ge/GePoint3d.h"

class OdDbGeoCoordinateSystemCategory;
class OdDbGeoCoordinateSystem;
class OdDbGeoCoordinateSystemTransformer;
typedef OdSmartPtr<OdDbGeoCoordinateSystemCategory> OdDbGeoCoordinateSystemCategoryPtr;
typedef OdSmartPtr<OdDbGeoCoordinateSystem> OdDbGeoCoordinateSystemPtr;
typedef OdSmartPtr<OdDbGeoCoordinateSystemTransformer> OdDbGeoCoordinateSystemTransformerPtr;

struct OdDbGeoDatum
{
  OdString id;
  OdString desc;
};

struct OdDbGeoEllipsoid
{
  OdString id;
  OdString desc;
  double   polarRadius; 
  double   eccentricity;
};

struct OdDbGeoProjectionParameter
{
  OdString name;
  double   value;
};
/** \details
  This abstract class represents a category which holds string identifiers referring to 
  a group of coordinate reference systems.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoCoordinateSystemCategory : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemCategory);
  
  /** \details
    Default constructor.
  */
  OdDbGeoCoordinateSystemCategory();
  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoCoordinateSystemCategory();

  /** \details
    Returns the category's ID.
    
    \param categoryId [in/out] Category ID.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getId(OdString& categoryId) const = 0;
  
  /** \details
    Returns the number of coordinate systems that belong to this category.
    
    \param num [in/out] Number of coordinate systems.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getNumOfCoordinateSystem(int& num) const = 0;
  
  /** \details
    Returns the coordinate system at the specified index.
    
    \param index [in] Index.
    \param pCoordSys [in/out] Pointer to an OdDbGeoCoordinateSystem object.
        
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getCoordinateSystemAt(int index, OdDbGeoCoordinateSystemPtr& pCoordSys) const = 0;
  
  /** \details
    Creates and returns all coordinate system categories.
    
    \param allCategories [in/out] Array of pointers to returned categories.
        
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  static  OdResult createAll(OdArray<OdDbGeoCoordinateSystemCategoryPtr>& allCategories);
};

/** \details
  This abstract class represents a coordinate reference system (CRS) definition.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoCoordinateSystem : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystem);

  /** \details
    This enumeration indicates the type of a coordinate reference system.
  */
  enum Type
  {
    kTypeUnknown = 0,     // The coordinate system is not set.
    kTypeArbitrary = 1,   // The coordinate system is arbitrary.
    kTypeGeographic = 2,  // The coordinate system is geographic.
    kTypeProjected = 3    // The coordinate system is projected.
  };

  /** \details
    This enumeration represents the coordinate reference system projection method types.
  */
  enum ProjectionCode
  {
    kProjectionCodeUnknown     = 0,     // Unknown projection code.
    kProjectionCodeAlber       = 4,     // Albers equal-area conic projection.
    kProjectionCodeAzede       = 59,    // Azimuthal equidistant projection (elevated ellipsoid).
    kProjectionCodeAzmea       = 11,    // Lambert azimuthal equal-area projection.
    kProjectionCodeAzmed       = 7,     // Lambert azimuthal equidistant projection.
    kProjectionCodeBipolar     = 31,    // Bipolar oblique conformal conic projection.
    kProjectionCodeBonne       = 24,    // Bonne Pseudo Conical projection.
    kProjectionCodeCassini     = 22,    // Cassini projection.
    kProjectionCodeEckert4     = 25,    // Eckert pseudocylindrical projection (number IV).
    kProjectionCodeEckert6     = 26,    // Eckert pseudocylindrical projection (number VI).
    kProjectionCodeEdcnc       = 12,    // Equidistant conic projection (conic projection).
    kProjectionCodeEdcyl       = 20,    // Equidistant cylindrical projection (spherical only).
    kProjectionCodeGaussK      = 46,    // Gauss-Kruger: Transverse Mercator projection without scale reduction parameter.
    kProjectionCodeGnomonic    = 19,    // Gnomonic projection.
    kProjectionCodeGoode       = 28,    // Goode homolosine projection.
    kProjectionCodeHom1uv      = 1281,  // Unrectified Hotine Oblique Mercator Projection. Single point form.
    kProjectionCodeHom1xy      = 1282,  // Rectified Hotine Oblique Mercator Projection. Single poin form.
    kProjectionCodeHom2uv      = 1283,  // Unrectified Hotine Oblique Mercator Projection. Two point form.
    kProjectionCodeHom2xy      = 1284,  // Rectified Hotine Oblique Mercator Projection. Two point form.
    kProjectionCodeKrovak      = 47,    // Original Krovak projection. 
    kProjectionCodeKrvk95      = 51,    // Krovak projection includes 1995 adjustment.
    kProjectionCodeLL          = 1,     // Produces (processes) latitude and longitude.  
    kProjectionCodeLm1sp       = 36,    // Single standard parallel variation of the Lambert Conformal Conic projection.
    kProjectionCodeLm2sp       = 37,    // Double standard parallel variation of the Lambert Conformal Conic projection.
    kProjectionCodeLmblg       = 38,    // Belgian variation of the Lambert Conformal Conic projection.
    kProjectionCodeLmtan       = 8,     // Lambert Tangential Conformal Conic projection.
    kProjectionCodeMiller      = 13,    // Miller cylindrical projection.
    kProjectionCodeMndotl      = 41,    // Minnesota DOT variation of the Lambert Conformal Conic projection. 
    kProjectionCodeMndott      = 42,    // Minnesota DOT variation of the Transverse Mercator projection.
    kProjectionCodeModpc       = 10,    // Lallemand IMW Modified Polyconic projection.
    kProjectionCodeMollweid    = 27,    // Mollweide projection.
    kProjectionCodeMrcat       = 6,     // Mercator Cylindrical projection. 
    kProjectionCodeMrcatK      = 49,    // Standard Mercator projection with a scale reduction factor instead of a standard parallel.
    kProjectionCodeMstero      = 15,    // Modified Stereographic projection.
    kProjectionCodeNeacyl      = 29,    // Normal aspect. Equal-area Cylindrical projection.
    kProjectionCodeNerth       = 55,    // Non-Earth projection.  
    kProjectionCodeNzealand    = 16,    // New Zealand National Grid.
    kProjectionCodeOblqM       = 5,     // Oblique Mercator projection.
    kProjectionCodeObqcyl      = 56,    // Oblique Cylindrical projection. 
    kProjectionCodeOrtho       = 18,    // Orthographic projection.
    kProjectionCodeOstn02      = 60,    // Transverse Mercator projection with the OSTN02 grid shift tacked on.
    kProjectionCodeOstn97      = 58,    // Transverse Mercator projection with the OSTN97 grid shift tacked on.
    kProjectionCodeOstro       = 34,    // Oblique Stereographic projection.
    kProjectionCodePlycn       = 9,     // Hassler American Polyconic projection.
    kProjectionCodePstro       = 33,    // Polar Stereographic projection.
    kProjectionCodePstrosl     = 53,    // Polar Stereographic projection with standard latitude.
    kProjectionCodeRskew       = 1285,  // Hotine Oblique Mercator (variant B) projection.
    kProjectionCodeRskewc      = 1286,  // Rectified Skew Orthomorphic projection, false origin and azimuth at center.
    kProjectionCodeRskewo      = 1287,  // Rectified Skew Orthomorphic projection.
    kProjectionCodeRobinson    = 23,    // Robinson Cylindrical projection.
    kProjectionCodeSinus       = 17,    // Sinusoidal projection.
    kProjectionCodeSotrm       = 43,    // Transverse Mercator south oriented projection.
    kProjectionCodeSstro       = 35,    // Synder's Oblique Stereographic projection.
    kProjectionCodeSwiss       = 32,    // Swiss projection.
    kProjectionCodeSys34       = 57,    // Combination of Transverse Mercator projection and a polynomial expansion used in Denmark.
    kProjectionCodeSys34_99    = 61,    // Combination of Transverse Mercator projection and a polynomial expansion used in Denmark. Polynomials are of the 1999 vintage.
    kProjectionCodeTeacyl      = 30,    // Transverse aspect. Equal-area Cylindrical projection.
    kProjectionCodeTm          = 3,     // Gauss-Kruger or Transverse Mercator Projection.
    kProjectionCodeTrmeraf     = 54,    // Transverse Mercator projection with affine post-processor.  
    kProjectionCodeTrmrkrg     = 62,    // Transverse Mercator projection using Kruger formulation.  
    kProjectionCodeTrmrs       = 45,    // Transverse Mercator projection per J. P. Snyder.
    kProjectionCodeVdgrntn     = 21,    // Van der Grinten projection.
    kProjectionCodeWccsl       = 39,    // Wisconsin County Coordinate System variation; Lambert Conformal Conic projection.
    kProjectionCodeWccst       = 40,    // Wisconsin County Coordinate System variation; Transverse Mercator projection.
    kProjectionCodeUtm         = 44,    // The UTM direct variation of the Transverse Mercator projection.
    kProjectionCodeWinkl       = 63,    // Winkel Tripel projection.
    kProjectionCodeNrthsrt     = 64,    // North with scale and rotation projection. 
    kProjectionCodeLmbrtaf     = 65,    // Lambert Conformal Conic projection with affine post-processor.
    kProjectionCodeSys34_01    = 66,    // Combination of Transverse Mercator projection and polynomial expansion used in Denmark.
    kProjectionCodeEdcylE      = 67,    // Equidistant Cylindrical projection, Ellipsoidal or Spherical.
    kProjectionCodePlateCarree = 68,    // Plate Carré projection.
    kProjectionCodePvMercator  = 69,    // Popular Visualisation Pseudo Mercator projection (Google Earth).
    kProjectionCodeLmMich      = 70,    // Lambert Conic Conformal (2SP Michigan) projection.
  };
  /** \details
    This enumeration represents the linear or angular unit.
  */
  enum Unit
  {
    kUnitUnknown = 0,
    kUnitMeter = 1,
    kUnitFoot = 2,
    kUnitInch = 3,
    kUnitIFoot = 4,
    kUnitClarkeFoot = 5,
    kUnitIInch = 6,
    kUnitCentimeter = 7,
    kUnitKilometer = 8,
    kUnitYard = 9,
    kUnitSearsYard = 10,
    kUnitMile = 11,
    kUnitIYard = 12,
    kUnitIMile = 13,
    kUnitKnot = 14,
    kUnitNautM = 15,
    kUnitLat66 = 16,
    kUnitLat83 = 17,
    kUnitDecimeter = 18,
    kUnitMillimeter = 19,
    kUnitDekameter = 20,
    kUnitHectometer = 21,
    kUnitGermanMeter = 22,
    kUnitCaGrid = 23,
    kUnitClarkeChain = 24,
    kUnitGunterChain = 25,
    kUnitBenoitChain = 26,
    kUnitSearsChain = 27,
    kUnitClarkeLink = 28,
    kUnitGunterLink = 29,
    kUnitBenoitLink = 30,
    kUnitSearsLink = 31,
    kUnitRod = 32,
    kUnitPerch = 33,
    kUnitPole = 34,
    kUnitFurlong = 35,
    kUnitRood = 36,
    kUnitCapeFoot = 37,
    kUnitBrealey = 38,
    kUnitSearsFoot = 39,
    kUnitGoldCoastFoot = 40,
    kUnitMicroInch = 41,
    kUnitIndianYard = 42,
    kUnitIndianFoot = 43,
    kUnitIndianFt37 = 44,
    kUnitIndianFt62 = 45,
    kUnitIndianFt75 = 46,
    kUnitIndianYd37 = 47,
    kUnitDecameter = 48,
    kUnitInternationalChain = 49,
    kUnitInternationalLink = 50,
    kUnitBrFootTrunc = 51,
    kUnitBrChainTrunc = 52,
    kUnitBrLinkTrunc = 53,
    kUnitDegree = 1001,
    kUnitGrad = 1002,
    kUnitGrade = 1003,
    kUnitMapInfo = 1004,
    kUnitMil = 1005,
    kUnitMinute = 1006,
    kUnitRadian = 1007,
    kUnitSecond = 1008,
    kUnitDecisec = 1009,
    kUnitCentisec = 1010,
    kUnitMillisec = 1011,
  };
  /** \details
    Default constructor.
  */
  OdDbGeoCoordinateSystem();
  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoCoordinateSystem();
  /** \details
    Returns the coordinate reference system's identifier string.
    
    \param coordSysId [out] Coordinate reference system's identifier.
    
    \returns Returns eOK if successful, or an appropriate error code if not. 
  */
  virtual OdResult getId(OdString& coordSysId) const = 0;
  /** \details
    Returns this CRS's EPSG code.
    
    \param epsgCode [out] EPSG code.
    
    \returns Returns eOK if successful, or an appropriate error code if not. 
  */
  virtual OdResult getEpsgCode(int& epsgCode) const = 0;
  /** \details
    Returns this CRS's type.
    
    \param type [out] CRS's type.
    
    \returns Returns eOK if successful, or an appropriate error code if not. 
  */
  virtual OdResult getType(OdDbGeoCoordinateSystem::Type& type) const = 0;
  /** \details
    Returns the description of this CRS.
    
    \param coordSysDesc [out] CRS's description.
    
    \returns Returns eOK if successful, or an appropriate error code if not. 
  */
  virtual OdResult getDescription(OdString& coordSysDesc) const = 0;
  /** \details
    Returns the unit of the coordinate system axis of this CRS.
    
    \param unit [out] Unit value.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
    
    \remarks
    All axis contains the same unit.
  */
  virtual OdResult getUnit(OdDb::UnitsValue& unit) const = 0;
  /** \details
    Returns the unit of the coordinate system axis of this CRS.
    
    \param unit [out] Unit value.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
    
    \remarks
    Each axis contains the same unit.
  */
  virtual OdResult getUnit(OdDbGeoCoordinateSystem::Unit& unit) const = 0;
  /** \details
    Returns the factor that is used for multiplying the length units of this CRS 
    for calculating the equivalent in meters.
    
    \param unitScale [out] Scale factor.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getUnitScale(double& unitScale) const = 0;
  /** \details
    Returns the projection method of this CRS.
    
    \param prjCode [out] Projection method.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getProjectionCode(OdDbGeoCoordinateSystem::ProjectionCode& prjCode) const = 0;
  /** \details
    Returns the projection parameters of this CRS.
    
    \param prjParams [out] Projection parameters.
    \param includeSpecialParams [in] Boolean flag that indicates if the special projection parameters should be counted.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getProjectionParameters(OdArray<OdDbGeoProjectionParameter>& prjParams,
    bool includeSpecialParams) const = 0;
  /** \details
    Returns the datum of this CRS.
    
    \param datum [in/out] Datum value.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getDatum(OdDbGeoDatum& datum) const = 0;
  /** \details
    Returns the ellipsoid related to this CRS.
    
    \param ellipsoid [out] Ellipsoid.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
    
    \remarks
    If the CRS contains an ellipsoid, it will not be geodetic and the datum reference will not have a value.
  */
  virtual OdResult getEllipsoid(OdDbGeoEllipsoid& ellipsoid) const = 0;
  /** \details
    Returns the projection method parameter offset easting of this CRS.
    
    \param offsetVec [in/out] Offset vector.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getOffset(OdGeVector2d& offsetVec) const = 0;
  /** \details
    Returns the cartesian extents of this CRS.
    
    \param exts [out] Extents.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getCartesianExtents(OdGeExtents2d& exts) const = 0;
  /** \details
    Returns the geodetic extents of this CRS, the range of longitude and latitude.
    
    \param exts [out] Extents.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getGeodeticExtents(OdGeExtents2d& exts) const = 0;
  /** \details
    Returns the string in XML format that represents this CRS.
    
    \param strXml [out] CRS's representation in XML format.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getXmlRepresentation(OdString& strXml) const = 0;
  /** \details
    Returns the string in WKT format that represents this CRS.
    
    \param strWkt [out] CRS's representation in WKT format.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getWktRepresentation(OdString& strWkt) const = 0;
  /** \details
    Creates and returns a new OdDbGeoCoordinateSystem object from the current virtual catalog.
    
    \param coordSysIdOrFullDef [in] ID, WKT or XML representation of the CRS definition to load.
    \param pCoordSys [out] New CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  static OdResult create(const OdString& coordSysIdOrFullDef, 
    OdDbGeoCoordinateSystemPtr& pCoordSys);
  /** \details
    Creates and returns a new OdDbGeoCoordinateSystem object for every existing, 
    persistent CRS definition from the current virtual catalog when the passed in 
    geodetic point is within the geodetic extents of the CRS.
    
    \param geoPt [in] Geodetic point.
    \param allCoordSys [out] Array of pointers to the new CRS objects.
    
    \returns Returns eOK if successful, or an appropriate error code if not.

    \remarks
    This method is not supported yet.
  */
  static OdResult createAll(const OdGePoint3d& geoPt, OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys);
  /** \details
    Creates and returns a new OdDbGeoCoordinateSystem object for every existing, 
    persistent CRS definition from the specified catalog.
    
    \param allCoordSys [out] Array of pointers to the new CRS objects.
    \param pCategory [in] Category.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
    
    \remarks
    If pCategory is NULL, all existing and persistent CRS definitions are returned.
  */
  static OdResult createAll(OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys,
    const OdDbGeoCoordinateSystemCategory* pCategory = NULL);
};

/** \details
  This abstract class represents a transformer object which is used to transform points from the source 
  CRS to the target CRS.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoCoordinateSystemTransformer : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemTransformer);
  /** \details
    Default constructor.
  */
  OdDbGeoCoordinateSystemTransformer();
  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoCoordinateSystemTransformer();
  /** \details
    Returns the source CRS's ID.
    
    \param sourceCoordSysId [out] Source CRS's ID.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getSourceCoordinateSystemId(OdString& sourceCoordSysId) const = 0;
  /** \details
    Returns the target CRS's ID.
    
    \param targetCoordSysId [out] Target CRS's ID.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult getTargetCoordinateSystemId(OdString& targetCoordSysId) const = 0;
  /** \details
    Transforms the point from the source CRS to the target CRS.
    
    \param pointIn [in] Point in the source CRS.
    \param pointOut [out] Point in the target CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult transformPoint(const OdGePoint3d& pointIn, OdGePoint3d& pointOut) const = 0;
  /** \details
    Transforms the array of points from the source CRS to the target CRS.
    
    \param pointsIn [in] Point array in the source CRS.
    \param pointsOut [out] Point array in the target CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult transformPoints(const OdGePoint3dArray& pointsIn, OdGePoint3dArray& pointsOut) const = 0;
  /** \details
    Transforms the point from the source CRS to the target CRS.
    
    \param sourceCoordSysId [in] Source CRS's ID.
    \param targetCoordSysId [in] Target CRS's ID.
    \param pointIn [in] Point in the source CRS.
    \param pointOut [out] Point in the target CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  static OdResult transformPoint(const OdString& sourceCoordSysId,
    const OdString& targetCoordSysId,
    const OdGePoint3d& pointIn,
    OdGePoint3d& pointOut);

  /** \details
    Transforms the array of points from the source CRS to the target CRS.
    
    \param sourceCoordSysId [in] Source CRS's ID.
    \param targetCoordSysId [in] Target CRS's ID.
    \param pointsIn [in] Point array in the source CRS.
    \param pointsOut [out] Point array in the target CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  static OdResult transformPoints(const OdString& sourceCoordSysId,
    const OdString& targetCoordSysId,
    const OdGePoint3dArray& pointsIn,
    OdGePoint3dArray& pointsOut);
  /** \details
    Creates a new OdDbGeoCoordinateSystemTransformer object from the source CRS and target CRS.
    
    \param sourceCoordSysId [in] Source CRS's ID.
    \param targetCoordSysId [in] Target CRS's ID.
    \param pCoordSysTransformer [out] New OdDbGeoCoordinateSystemTransformer object.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  static OdResult create(const OdString& sourceCoordSysId, 
    const OdString& targetCoordSysId,
    OdDbGeoCoordinateSystemTransformerPtr& pCoordSysTransformer);
};

#include "TD_PackPop.h"

#endif //_OD_DB_GEOCOORDINATESYSTEM_
