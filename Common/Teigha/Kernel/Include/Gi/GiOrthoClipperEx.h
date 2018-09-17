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

#ifndef __ODGIORTOCLIPPEREX_H__
#define __ODGIORTOCLIPPEREX_H__

#include "Gi/GiOrthoClipper.h"
#include "Gi/GiAbstractClipBoundary.h"

#include "TD_PackPush.h"

class OdGeBoundBlock3d;
class OdStreamBuf;

/** \details

    This class implements a geometry clipper, used for clipping objects.
    {group:OdGi_Classes}
*/
class ODGI_EXPORT OdGiOrthoClipperEx : public OdGiOrthoClipper
{
public:
  ODRX_DECLARE_MEMBERS(OdGiOrthoClipperEx);

  // Initial clipper interface support

  TD_USING(OdGiOrthoClipper::set);
  TD_USING(OdGiOrthoClipper::get);

  // Extend initial interface by inverted clipping boundary support

  /** \details
    Creates a clipping stage.

    \param bInverted [in]  Specifies the Inverted (if true) or the Normal (if false) type of polygonal clipping boundary.
    \param nPoints [in]  Count of points.
    \param points [in]  Points defining sides of clipping prism perpendicular to XY.
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.

    \remarks
    If nPoints is 0 - XY clip is disabled. If nPoints is 2 - defines a rectangle clipping boundary.
  */
  virtual void set(bool bInverted,
                   OdUInt32 nPoints,          // points counter. 0 - disables XY clip; 2 - defines rectangle.
                   const OdGePoint2d* points, // points defining sides of clipping prism perpendicular to XY
                   bool bClipLowerZ = false, double dLowerZ = 0.0,
                   bool bClipUpperZ = false, double dUpperZ = 0.0) = 0;

  /** \details
    Creates a clipping stage.

    \param bInverted [in]  Specifies the Inverted (if true) or the Normal (if false) type of polygonal clipping boundary.
    \param points [in]  Array of 2D points defining sides of clipping prism perpendicular to XY.
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane. 
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.
  */
  virtual void set(bool bInverted,
                   const OdGePoint2dArray& points, // points defining sides of clipping prism perpendicular to XY
                   bool bClipLowerZ = false, double dLowerZ = 0.0,
                   bool bClipUpperZ = false, double dUpperZ = 0.0) = 0;

  /** \details
    Returns a clipping stage.

    \param bInverted [out]  Specifies the Inverted (if true) or the Normal (if false) type of polygonal clipping boundary.
    \param points [out]  Array of 2D points defining sides of clipping prism perpendicular to XY.
    \param bClipLowerZ [out]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [out]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [out]  Flag which indicates that there is an upper clipping plane. 
    \param dUpperZ [out]  Z-coordinate of the upper clipping plane.
  */
  virtual void get(bool &bInverted, OdGePoint2dArray& points,
                   bool& bClipLowerZ, double& dLowerZ,
                   bool& bClipUpperZ, double& dUpperZ) const = 0;

  // Extend initial interface by extended clipping boundary support

  /** \details
    Creates a clipping stage with extended or complex clipping boundary.

    \param bPreprocess [in]  Specifies the Complex (if true) or the Extended (if false) type of polygonal clipping boundary.
    \param nCounts [in]  Count of polygons which define the clipping boundary.
    \param counts [in]  Array of points count of each polygon.
    \param nPoints [in]  Count of points.
    \param points [in]  Points defining sides of clipping prism perpendicular to XY.
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane.
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.
  */
  virtual void set(bool bPreprocess, OdUInt32 nCounts, const int *counts,
                   OdUInt32 nPoints, const OdGePoint2d *points,
                   bool bClipLowerZ = false, double dLowerZ = 0.0,
                   bool bClipUpperZ = false, double dUpperZ = 0.0) = 0;

  /** \details
    Creates a clipping stage with extended or complex clipping boundary.

    \param bPreprocess [in]  Specifies the Complex (if true) or the Extended (if false) type of polygonal clipping boundary.
    \param counts [in]  Array of points count of each polygon.
    \param points [in]  Array of 2D Points defining polygons.
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane.
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.
  */
  virtual void set(bool bPreprocess, const OdIntArray& counts, const OdGePoint2dArray& points,
                   bool bClipLowerZ = false, double dLowerZ = 0.0,
                   bool bClipUpperZ = false, double dUpperZ = 0.0) = 0;

  /** \details
    Returns a clipping stage.

    \param counts [out]  Array of points count of each polygon.
    \param points [out]  Array of 2D points defining polygons.
    \param bClipLowerZ [out]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [out]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [out]  Flag which indicates that there is an upper clipping plane.
    \param dUpperZ [out]  Z-coordinate of the upper clipping plane.
  */
  virtual void get(OdIntArray& counts, OdGePoint2dArray& points,
                   bool& bClipLowerZ, double& dLowerZ,
                   bool& bClipUpperZ, double& dUpperZ) const = 0;

  // Stubs for old extended clipping boundary interface

  /** \details
    Creates a clipping stage with extended clipping boundary.

    \param nCounts [in]  Count of polygons which define the clipping boundary.
    \param counts [in]  Array of points count of each polygon.
    \param nPoints [in]  Count of points.
    \param points [in]  Points defining sides of clipping prism perpendicular to XY.
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane. 
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.
  */
  void set(OdUInt32 nCounts, const int *counts,
           OdUInt32 nPoints, const OdGePoint2d *points,
           bool bClipLowerZ = false, double dLowerZ = 0.0,
           bool bClipUpperZ = false, double dUpperZ = 0.0)
  { set(false, nCounts, counts, nPoints, points, bClipLowerZ, dLowerZ, bClipUpperZ, dUpperZ); }

  /** \details
    Creates a clipping stage with extended clipping boundary.

    \param counts [in]  Array of points count of each polygon.
    \param points [in]  Array of 2D points defining polygons.
    \param bClipLowerZ [in]  Flag which indicates that there is a lower clipping plane.
    \param dLowerZ  [in]  Z-coordinate of the lower clipping plane.
    \param bClipUpperZ [in]  Flag which indicates that there is an upper clipping plane.
    \param dUpperZ [in]  Z-coordinate of the upper clipping plane.
  */
  void set(const OdIntArray& counts, const OdGePoint2dArray& points,
           bool bClipLowerZ = false, double dLowerZ = 0.0,
           bool bClipUpperZ = false, double dUpperZ = 0.0)
  { set(false, counts, points, bClipLowerZ, dLowerZ, bClipUpperZ, dUpperZ); }

  // Some classification functionality applicable for single (top) clipping stage

  /** \details
    Returns true if the top clipping stage is empty or returns false in the other case.
  */
  virtual bool isEmpty() const = 0;

  /** \details
    Returns true if the top clipping stage is inverted or returns false in the other case.
  */
  virtual bool isInverted() const = 0;

  /** \details
    Returns true if the top clipping stage is extended or returns false in the other case.
  */
  virtual bool isExtended() const = 0;

  /** \details
    Returns true if there is only one clipping stage or returns false if there is not.
  */
  virtual bool isSingleClipStage() const = 0;

  /** \details
    Returns true if the set of clipping stages is empty or returns false in the other case.
  */
  virtual bool isEmptyClipSet() const = 0;

  // Support direct setting of clipping boundaries

  /** \details
    Creates a clipping boundary.

    \param pBoundary [in]  Clipping boundary.
    \param pClipInfo [in]  Additional clipping boundary information. 
    \param pXform [in]  Optional boundary transform matrix.
  */
  virtual void set(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo = NULL, const OdGeMatrix3d *pXform = NULL) = 0;

  /** \details
    Returns the type of clipping boundary.

    \sa
    <link OdGiAbstractClipBoundary::BoundaryType, BoundaryType enumeration.>
  */
  virtual OdGiAbstractClipBoundary::BoundaryType getType() const = 0;

  // Configuration

  /** \details
    Enables an analytic curves clipping.

    \param bEnable [in]  Boolean flag which indicates that analytic curves clipping is enabled (true value)
    or disabled (false value).
  */
  virtual void enableAnalyticCurvesClipping(bool bEnable) = 0;

  /** \details
    Returns true if the analytic curves clipping is enabled or returns false in the other case.
  */
  virtual bool isAnalyticCurvesClippingEnabled() const = 0;

  /** \details
    Enables clipping.
  */
  virtual void enable() = 0;

  /** \details
    Disables clipping.
  */
  virtual void disable() = 0;

  /** \details
    Returns true if clipping is enabled or returns false in the other case.
  */
  virtual bool enabled() const = 0;

  /** \details
    Represents a memory limit (in bytes) used by clipping engine during clipping a set of polygons.
  */
  enum ClipLimit
  {
    /**Memory limit (in bytes) which limits internal clipping structures number.*/
    kPolyClipLimit = 0, // Memory limit (in bytes) which limits internal clipping structures number
    /**Count of limits available at the current moment.*/
    kNumClipLimits
  };

  /** \details
    Sets a memory limit (in bytes) used by clipping engine during clipping a set of polygons.

    \param clipLimit [in]  Memory limit (in bytes) which limits internal clipping structures number..
    \param nLimit [in]  Count of limits.
  */
  virtual void setLimit(ClipLimit clipLimit, OdUInt32 nLimit) = 0;

  /** \details
    Returns a memory limit (in bytes) used by clipping engine during clipping a set of polygons.

    \param clipLimit [in]  Memory limit (in bytes) which limits internal clipping structures number.
  */
  virtual OdUInt32 getLimit(ClipLimit clipLimit) const = 0;

  // Support mutiple clip stages

  /** \details
    Tolerance override specification.
  */
  struct TolOverride
  {
    /**Tolerance override value.*/
    double m_tolOverride;
    /** \details
      How to treat tolerance override.
    */
    enum OverrideType
    {
      /**Tolerance override disabled.*/
      kNoOverride = 0,
      /**Absolute tolerance override which reset internally calculated tolerance completely.*/
      kAbsolute,
      /**Tolerance override is a multiplier for internally calculated tolerance.*/
      kMultiplier,
      /**Tolerance override is a addition to internally calculated tolerance.*/
      kAddition
    } m_overrideType;

    TolOverride() : m_tolOverride(1.e-10), m_overrideType(kNoOverride) {}
  };

  /** \details
    Polygonal clipping boundary tolerance override.
  */
  struct PolyTolOverride
  {
    /**World space clipping tolerance override.*/
    TolOverride m_clipTolOverride;
    /**Local boundary space clipping tolerance override.*/
    TolOverride m_localTolOverride;
    /**Sections generation tolerance override.*/
    TolOverride m_sectionTolOverride; // reserved
    /**Clipping boundary preprocessing tolerance.*/
    TolOverride m_boundaryTolOverride; // reserved
  };

  /** \details
    Describes a clipping plane.
  */
  struct ClipPlane
  {
    /**Plane's origin point.*/
    OdGePoint3d m_origin;
    /**Plane's normal vector.*/
    OdGeVector3d m_normal;
    /** \details
      Describes a type of clipping plane.
    */
    enum MarkType
    {
      /**Plane is not marked.*/
      kNoMark = 0,
      /**Lower plane.*/
      kMarkLower,
      /**Upper plane.*/
      kMarkUpper
    } m_markType;
    /**Optional plane clipping tolerance override.*/
    TolOverride m_clipTolOverride;
    /**Optional plane sections generation tolerance override.*/
    TolOverride m_sectionTolOverride;

    ClipPlane() : m_markType(kNoMark) {}
  };
  /** \details
    Defines the type of array of clipping planes.
  */
  typedef OdArray<ClipPlane, OdMemoryAllocator<ClipPlane> > ClipPlanesArray;

  /** \details
    Set of flags which can be passed for separate clipping stage during construction.
  */ 
  enum ClipStageFlags
  {
    /**Default clipping stage flags state.*/
    kCSNoFlags         = 0,
    /**Construct clipping stage in disabled state.*/
    kCSDisabled        = (1 << 0),
    /**Specifies the Inverted (if true) or the Normal (if false) type of polygonal clipping boundary.*/
    kCSInverted        = (1 << 1),
    /**Mark that the clipping stage can generate section geometry.*/
    kCSEnableSections  = (1 << 2),
    /**Enable passing geometry through clipping stage without clipping if sections output disabled.*/
    kCSPassNonSections = (1 << 3)
  };

  /** \details
    Represents a classification of clipping contours in a clipping boundary.
  */ 
  enum CountsClassify
  {
    /**Classification is not used.*/
    kCCDontClassify = 0,      // Avoid classification
    /**Classification by contours inclusion.*/
    kCCClassifiedByInclusion, // Classify by polygon-polygon inclusion
    /**Classification by positive/negative counts number.*/
    kCCClassifiedByInteger,   // Classify by positive/negative counts number
    /**Classification by clockwise/counterclockwise ordering.*/
    kCCClassifiedByOrder,     // Classify by CW/CCW ordering
    /**Classification by positive/negative normal.*/
    kCCClassifiedByNormal,    // Classify by positive/negative normal
    /**Classification by Xor operation (intersection).*/
    kCCClassifiedByXor,       // Preprocess by Xor operation (intersection)
    /**Classification by set of Union/Subtract operations.*/
    kCCClassifiedBySignedXor  // Preprocess by set of Union/Subtract operations
  };

  /** \details
    Creates a clipping boundary.

    \param pBoundary [in]  Clipping boundary.
    \param pClipInfo [in]  Additional clipping boundary information. 
    \param pXform [in]  Optional boundary transform matrix.
  */ 
  virtual void pushClipStage(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo = NULL, const OdGeMatrix3d *pXform = NULL) = 0;

  /** \details
    Creates a clipping boundary from infinite planes defined in a space.

    \param numPlanes [in]  Count of planes.
    \param pPlanes [in]  Planes for defining a clipping boundary. 
    \param pXform [in]  Optional boundary transform matrix.
    \param stageFlags [in]  Set of clipping stage flags (look at ClipStageFlags enumeration).
  */
  virtual void pushClipStage(OdUInt32 numPlanes, const ClipPlane *pPlanes, const OdGeMatrix3d *pXform = NULL, OdUInt32 stageFlags = kCSNoFlags) = 0;

  /** \details
    Creates a clipping boundary.

    \param nCounts [in]  Count of polygons which define the clipping boundary.
    \param counts [in]  Array of points count of each polygon.
    \param nPoints [in]  Count of vertices which define the clipping boundary.
    \param points [in]  Array of points defining a clipping boundary.
    \param stageFlags [in]  Set of clipping stage flags (look at ClipStageFlags enumeration).
    \param countsClass [in]  Clipping contours classification mode. 
    \param pXform [in]  Optional boundary transform matrix.
    \param numPlanes [in]  Count of planes.
    \param pPlanes [in]  Planes for defining a clipping boundary.
    \param pPolyTol [in]  Optional polygon tolerance overrides.
  */ 
  virtual void pushClipStage(OdUInt32 nCounts, const int *counts,
                             OdUInt32 nPoints, const OdGePoint2d *points,
                             OdUInt32 stageFlags = kCSNoFlags, CountsClassify countsClass = kCCDontClassify, const OdGeMatrix3d *pXform = NULL,
                             OdUInt32 numPlanes = 0, const ClipPlane *pPlanes = NULL, PolyTolOverride *pPolyTol = NULL) = 0;

  /** \details
    Creates a clipping boundary.

    \param nCounts [in]  Count of polygons which define the clipping boundary.
    \param counts [in]  Array of points count of each polygon.
    \param nPoints [in]  Count of vertices which define the clipping boundary.
    \param points [in]  Array of points defining a clipping boundary.
    \param pNormal [in]  Normal.
    \param stageFlags [in]  Set of clipping stage flags (look at ClipStageFlags enumeration).
    \param countsClass [in]  Clipping contours classification mode. 
    \param pXform [in]  Optional boundary transform matrix.
    \param numPlanes [in]  Count of planes.
    \param pPlanes [in]  Planes for defining a clipping boundary.
    \param pPolyTol [in]  Optional polygon tolerance overrides.
  */ 
  virtual void pushClipStage(OdUInt32 nCounts, const int *counts,
                             OdUInt32 nPoints, const OdGePoint3d *points, const OdGeVector3d *pNormal = NULL,
                             OdUInt32 stageFlags = kCSNoFlags, CountsClassify countsClass = kCCDontClassify, const OdGeMatrix3d *pXform = NULL,
                             OdUInt32 numPlanes = 0, const ClipPlane *pPlanes = NULL, PolyTolOverride *pPolyTol = NULL) = 0;

  /** \details
    Removes a clipping stage.

    \returns
    Returns true if the clipping stage was successfully removed or returns false in the other case.
  */ 
  virtual bool popClipStage() = 0;

  /** \details
    Clears all clipping stages.

    \param bClearCache [in]  Flag which indicates that caches should be cleared.
  */ 
  virtual void clearClipping(bool bClearCache = false) = 0;

  /** \details
    Clears all temporarily allocated arrays and caches.
  */   
  virtual void clearTemporaryArrays() = 0;

  /** \details
    Returns the count of clipping stages.
  */ 
  virtual OdUInt32 numClipStages() const = 0;

  /** \details
    Classifies the clipping stage.

    \param nStage [in]  Clipping stage's index.
    \param pNPlanes [in/out]  Count of planes in the clipping stage.
    \param pNPolys [in/out]  Count of polygons in the clipping stage.
    \param bFirstPolyInverted [in/out]  Flag which indicates that the first polygon is inverted.
  */
  virtual void classifyClipStage(OdUInt32 nStage, OdUInt32 *pNPlanes, OdUInt32 *pNPolys = NULL, bool *bFirstPolyInverted = NULL) const = 0;

  /** \details
    Returns true if sectioning enabled for specified clipping stage.
    \param nStage [in]  Clipping stage's index.
  */
  virtual bool isClipStageSectionable(OdUInt32 nStage) const = 0;

  /** \details
    Returns the clipping stage.

    \param nStage [in]  Clipping stage's index.
    \param counts [out]  Array of points count of each polygon in the clipping stage.
    \param points [out]  Array of 2D points defining a clipping boundary.
    \param pNormal [in/out]  Normal.

    \returns
    Returns true if the clipping stage is not empty or returns false in the other case.
  */
  virtual bool getClipStage(OdUInt32 nStage, OdIntArray& counts, OdGePoint2dArray& points, OdGeVector3d *pNormal = NULL) const = 0;

  /** \details
    Returns the clipping stage.

    \param nStage [in]  Clipping stage's index.
    \param counts [out]  Array of points count of each polygon in the clipping stage.
    \param points [out]  Array of 3D points defining a clipping boundary.
    \param pNormal [in/out]  Normal.

    \returns
    Returns true if the clipping stage is not empty or returns false in the other case.
  */
  virtual bool getClipStage(OdUInt32 nStage, OdIntArray& counts, OdGePoint3dArray& points, OdGeVector3d *pNormal = NULL) const = 0;

  /** \details
    Returns the clipping stage when the clipping boundary defined by infinite planes.

    \param nStage [in]  Clipping stage's index.
    \param planes [out]  Array of infinite planes in the clipping stage.

    \returns
    Returns true if the clipping stage is not empty or returns false in the other case.
  */
  virtual bool getClipStage(OdUInt32 nStage, ClipPlanesArray& planes) const = 0;

  /** \details
    Enables or disables specified clipping stage.

    \param nStage [in]  Clipping stage's index.
    \param bEnable [in]  New clipping stage state.
  */
  virtual void enableClipStage(OdUInt32 nStage, bool bEnable) = 0;

  /** \details
    Checks that clipping stage enabled or disabled.

    \param nStage [in]  Clipping stage's index.
  */
  virtual bool isClipStageEnabled(OdUInt32 nStage) const = 0;

  // Interface compatibility with OdGiOrthoPrismIntersector

  /** \details
    Returns the clipping status:
    <table>
    Returned value    Description
    0                 Clipping does not occur.
    8                 The object clipped partially or completely at least once.
    16                The object completely clipped at least once.
    </table>
  */
  virtual OdUInt32 clipStatus() const = 0;

  /** \details
    Clears the clipping status:
  */
  virtual void clearClipStatus() = 0;

  // Some helpful checking routines

  /** \details
    Represents a visibility status of clipped objects.
  */ 
  enum VisibilityStatus
  {
    /**Invisible.*/
    kStatusInvisible = -1,
    /**Clipped.*/
    kStatusClipped   =  0,
    /**Visible.*/
    kStatusVisible   =  1
  };

  /** \details
    Returns the visible status of the 3D point.

    \param pt [in]  3D point.
  */
  virtual VisibilityStatus checkPointVisibility(const OdGePoint3d &pt) const = 0;

  /** \details
    Returns the sphere's visible status.

    \param origin [in]  Sphere's origin point.
    \param radius [in]  Sphere's radius.
  */
  virtual VisibilityStatus checkSphereVisibility(const OdGePoint3d &origin, double radius) const = 0;

  /** \details
    Returns the visible status of extents.

    \param extents [in]  Extents.
  */
  virtual VisibilityStatus checkExtentsVisibility(const OdGeExtents3d &extents) const = 0;

  /** \details
    Returns the visible status of the bounding block.

    \param bb [in]  Bounding block.
  */
  virtual VisibilityStatus checkBoundsVisibility(const OdGeBoundBlock3d &bb) const = 0;

  // Clipping engine debugging

  /** \details
    Enables clipping engine debugging.

    \param pStream [out]  Stream to which the debugging information is to be written.
  */
  virtual void enableLogging(OdStreamBuf *pStream) = 0;

  /** \details
    Disable clipping engine debugging.
  */
  virtual void disableLogging() = 0;

  /** \details
    Returns boolean value true if the clipping engine debugging enabled. If 
    the clipping engine debugging disabled returns false.
  */
  virtual bool isLoggingEnabled() const = 0;

  // Support sections

  /** \details
    Output for optional section geometry.
  */
  virtual OdGiConveyorOutput& sectionOutput() = 0;

  /** \details
    Sends accumulated geometry sections into conveyor output.

    \param bFlushClosed [in]  Send accumulated closed sections as shell geometry primitive.
    \param bFlushOpened [in]  Send accumulated opened sections as polyline geometry primitive.
    \param bReleaseData [in]  Delete accumulated sections data after flushing.
  */
  virtual void flushSection(bool bFlushClosed = true, bool bFlushOpened = false,
                            bool bReleaseData = true) = 0;
};

/** \details
  This template class is specialization of the OdSmartPtr class for the OdGiOrthoClipperEx class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdGiOrthoClipperEx> OdGiOrthoClipperExPtr;

// Utility classes

/** \details

    This class implements a section geometry interface connector for clipper.
    {group:OdGi_Classes}
*/
class ODGI_EXPORT OdGiSectionGeometryClipConnector : public OdGiConveyorNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSectionGeometryClipConnector);

  /** \details
    Sets section geometry output interface.
    \param pOutput [in]  Section geometry output interface.
  */
  virtual void setSectionGeometryOutput(OdGiSectionGeometryOutput *pOutput) = 0;

  /** \details
    Returns section geometry output interface.
  */
  virtual OdGiSectionGeometryOutput *getSectionGeometryOutput() const = 0;

  /** \details
    Checks that section geometry output was set.
  */
  virtual bool hasSectionGeometryOutput() const = 0;

  /** \details 
    Sets the draw context object (to access to traits, etc).
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawCtx) = 0;

  /** \details
    Enables skipping of section geometry output.
    \param bSkip [in]  New state for section geometry output skipping mode.
    \remarks In case if skipping enabled this conveyor node will simply redirect section geometry to output geometry.
  */
  virtual void setSectionGeometryOutputSkip(bool bSkip) = 0;

  /** \details
    Check that section geometry output skipping mode enabled.
  */
  virtual bool isSectionGeometryOutputSkipping() const = 0;

  /** \details
    Provides ability to disable output for this conveyor node.
    \param bDisable [in]  New state for conveyor node output disable.
    \remarks In case if this conveyor node is in disabled state it will work as OdGiEmptyGeometry.
  */
  virtual void disableGeometryOutput(bool bDisable) = 0;

  /** \details
    Returns current state of conveyor node output disable mode.
  */
  virtual bool isGeometryOutputDisabled() const = 0;

  /** \details
    Returns true if closed sections output enabled.
  */
  virtual bool isClosedSectionsOutputEnabled() const = 0;

  /** \details
    Returns true if opened sections output enabled.
  */
  virtual bool isOpenedSectionsOutputEnabled() const = 0;
};

/** \details
  This template class is specialization of the OdSmartPtr class for the OdGiSectionGeometryClipConnector class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdGiSectionGeometryClipConnector> OdGiSectionGeometryClipConnectorPtr;

#include "TD_PackPop.h"

#endif //#ifndef __ODGIORTOCLIPPEREX_H__
