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


#ifndef _DbSweepOptions_h_Included_
#define _DbSweepOptions_h_Included_

#include "TD_PackPush.h"
#include "DbEntity.h"


class OdDbSweepOptionsImpl;

/** \details
Class for setting options used for creating swept surfaces and swept solids (by calling OdDbSweptSurface::createSweptSurface() or OdDb3dSolid::createSweptSolid() methods).
Sweep options are used to control the shape of the resulting surface or solid object.

<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSweepOptions
{
public:
  /** \details 
  Creates an instance of sweep options. 
  */
  OdDbSweepOptions ();
  
  /** \details 
  Destroys the instance of sweep options. 
  */  
  ~OdDbSweepOptions ();
  
  /** \details 
  Creates a copy of another instance of swept entity options (copy constructor).
  
  \param src [in] An instance of swept entity options to be copied.
  */
  OdDbSweepOptions ( const OdDbSweepOptions& src );
  
  /** \details 
  Assigning operator for swept entity options.
  
  \param src [in] An instance of swept entity options to be assigned to this instance.
  */
  OdDbSweepOptions& operator = ( const OdDbSweepOptions& src );

  /** \details
  Alignment options enumerator for swept entities.
  */
  enum AlignOption 
  {
    kNoAlignment                = 0, // 
    kAlignSweepEntityToPath     = 1, // 
    kTranslateSweepEntityToPath = 2, // 
    kTranslatePathToSweepEntity = 3 // 
  };

  /** \details
  Miter options enumerator for swept entities.
  */
  enum MiterOption 
  {
    kDefaultMiter = 0,
    kOldMiter     = 1,
    kNewMiter     = 2,
    kCrimpMiter   = 3,
    kBendMiter    = 4
  };

  /** \details
  Returns the draft angle (in radians).
  The draft angle is the angle by which the profile will taper as it is swept. 
  The default value is equal to 0.0. 
  */
  double  draftAngle () const;

  /** \details
  Sets the draft angle.
  
  The draft angle is the angle by which the profile will taper as it is swept. 
  The default value is equal to 0.0. 

  \param ang [in] A new draft angle (in radians).
  */
  void  setDraftAngle ( double ang );

  /** \details
  Returns the start draft distance.
  The default value is equal to 0.0. 
  */
  double  startDraftDist () const;

  /** \details
  Sets the start draft distance.
	The default value is equal to 0.0. 
  
  \param val [in] A new value of the start draft distance.
  */
  void  setStartDraftDist ( double val );

  /** \details
  Returns the end draft distance. 
  The default value is equal to 0.0. 
  */
  double  endDraftDist () const;

  /** \details
  Sets the start draft distance.
  The default value is equal to 0.0. 	  
  
  \param val [in] A new value of the end draft distance.
  */
  void  setEndDraftDist ( double val );

  /** \details
  Returns the twist angle in radians.
  Twist angle is the angle by which the profile will be twisted as it is swept.
  The default value is equal to 0.0. 
  */
  double  twistAngle () const;

  /** \details
  Sets the twist angle.
  
  Twist angle is the angle by which the profile will be twisted as it is swept.
  The default value is equal to 0.0. 	  
  
  \param ang [in] A new twist angle (in radians).
  */
  void  setTwistAngle ( double ang );

  /** \details
  Returns the scale factor.
  */
  double  scaleFactor () const;

  /** \details
  Sets the scale factor.
	  
	\param val [in] A new value of the scale factor.
  */
  void  setScaleFactor ( double val );

  /** \details
  Returns the align angle.
  */
  double  alignAngle () const;

  /** \details
  Sets the align angle.
	  
  \param ang [in] A new value of the align angle.
  */
  void  setAlignAngle ( double ang );

  /** \details
  Returns the align option.
  */
  OdDbSweepOptions::AlignOption  align () const;

  /** \details
  Sets the align option.
	  
  \param val [in] A new value for the align option.
  */
  void  setAlign ( OdDbSweepOptions::AlignOption val );

  /** \details
  Returns the miter option.
  */
  OdDbSweepOptions::MiterOption  miterOption () const;

  /** \details
  Sets the miter option.
	  
  \param val [in]  A new value for the miter option.
  */
  void  setMiterOption ( OdDbSweepOptions::MiterOption val );

  /** \details
  Returns the align to start option.

  If the option value is equal to true, alignment to the start of the curve is applied.
  If the option value is equal to false, alignment to the end of the curve is applied.
  */
  bool  alignStart () const;

  /** \details
  Sets the align to start option.

  If the option value is equal to true, alignment to the start of the curve is applied.
	If the option value is equal to false, alignment to the end of the curve is applied.
  
  \param val [in] A new value of the align to start option.
  */
  void  setAlignStart ( bool val );

  /** \details
  Returns the base point for alignment.
  */
  OdGePoint3d  basePoint () const;

  /** \details
  Sets the base point for alignment.
	  
  \param pnt [in] A new base point for alignment.
  */
  void  setBasePoint ( const OdGePoint3d& pnt );

  /** \details
  Returns the bank flag.
  */
  bool  bank () const;

  /** \details
  Sets the bank flag.
	  
  \param val [in]  A new value for the bank flag.
  */
  void  setBank ( bool val );

  /** \details
  Returns the check self-intersections flag.
  If this flag is disabled, a self-intersecting surface may be created.
  */
  bool  checkIntersections () const;

  /** \details
  Sets the check self-intersections flag.
  
	If this flag is disabled, a self-intersecting surface may be created.  
  
  \param val [in]  A new value of the check self-intersections flag.
  */
  void  setCheckIntersections ( bool val );

  /** \details
  Returns the twist vector.
  */
  OdGeVector3d  twistRefVec () const;

  /** \details
  Sets the twist vector.
	  
  \param vec [in]  A new twist vector.
  */
  void  setTwistRefVec ( const OdGeVector3d& vec );

  /** \details
  Returns the transformation associated with this entity.
	  
  \param mat [out] A passed-in parameter for returning the transformation matrix.
  */
  bool  getSweepEntityTransform ( OdGeMatrix3d& mat )const;

  /** \details
  Sets the transformation associated with this entity.
	  
  \param mat [in] A new transformation to be associated with the entity.
  */
  void  setSweepEntityTransform ( const OdGeMatrix3d& mat ); 
  
  /** \details
  Sets the coordinate system transformation matrix to be applied to the sweep entity, using the sweep entity geometry for calculating the transformation matrix. 

  \param sweepEntities        [out] Sweep entity geometry to be used for calculation of the sweep entity transformation matrix.
  \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.
  
  \returns Returns eOk if transformation was successfully done.
  */
  OdResult  setSweepEntityTransform ( OdDbEntityPtrArray& sweepEntities,  bool displayErrorMessages = false );

  /** \details
  Returns the transformation associated with the start and end of the path curve.
  
  \param mat [in] A passed-in parameter for returning the transformation matrix of the path curve.
	  
  \returns Returns true if the transformation matrix was successfully returned, otherwise returns false.  
  */
  bool  getPathEntityTransform ( OdGeMatrix3d& mat )const;

  /** \details
  Sets the transformation associated with the start and end of the path curve.
	  
  \param mat [in] A new transformation matrix to be applied.
  */
  void  setPathEntityTransform ( const OdGeMatrix3d& mat ); 

  /** \details
  Sets the transformation associated with the start and end of the path curve.

  \param pPathEnt             [in] A pointer to the new transformation matrix to be applied.
  \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.
  
  \remarks
  Before calling the method, setSweepEntityTranform() method must be called.
  */
  OdResult  setPathEntityTransform ( OdDbEntity *pPathEnt,  bool displayErrorMessages = false );

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
  OdResult  checkSweepCurve ( OdDbEntity *pSweepEnt,
    OdDb::Planarity& planarity, OdGePoint3d& pnt, OdGeVector3d& vec,
    bool& closed, double& approxArcLen, bool displayErrorMessages = false );

  /** \details
  Determines if the passed-in path curve is valid.

  \param pPathEnt             [in] A pointer to the path curve.
  \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.
  
  \returns Returns eOk if curve is valid or eInvalidInput otherwise. 
  */
  OdResult  checkPathCurve ( OdDbEntity *pPathEnt,    bool displayErrorMessages = false );

private:
  friend class OdDbSweepOptionsImpl;
  OdDbSweepOptionsImpl *m_pImpl;
};

#include "TD_PackPop.h"
#endif //_DbSweepOptions_h_Included_
