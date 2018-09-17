/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


#ifndef _DbBlendOptions_h_Included_
#define _DbBlendOptions_h_Included_

#include "TD_PackPush.h"
#include "DbEntity.h"
#include "ModelerGeometryCreator.h"

class OdDbBlendOptionsImpl;

/** \details
Class for setting options used for creating blend surfaces and lofted solids (by calling OdDbLoftedSurface::createBlendSurface() or OdDb3dSolid::createBlendSolid() methods).
Loft and blend options are used to control the shape of the resulting surface or solid object.

<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbBlendOptions
{
public:
  /** \details
  Creates an OdDbBlendOptions object.
  */
  OdDbBlendOptions();

  /** \details
  Copy constructor for the OdDbBlendOptions class.
  */
  //OdDbBlendOptions(const OdDbBlendOptions& src);

  /** \details
  Destroys the OdDbBlendOptions object.
  */
  ~OdDbBlendOptions();

  /** \details
  Assigns to the OdDbBlendOptions object another object (assignment operator).

  \param rhs [in] Another OdDbBlendOptions object to assign.
  */
  OdDbBlendOptions& operator = (const OdDbBlendOptions& rhs);

  /// <summary>
  ///   <para>The drive mode property defines which section(s) "drive" the creation of the planes </para>
  ///   <para>containing the planar guides. Any of the two input sections can be selected for </para>
  ///   <para>that role, or both of them can be used at the same time (the default value). </para>
  ///   <para>The implementation uses the driving sections in conjunction with the coplanar point </para>
  ///   <para>and direction (if supplied).</para>
  /// </summary>
  ///
  enum DriveModeType {
    kDriveModeFirst = 0,
    kDriveModeSecond = 1,
    kDriveModeBoth = 2
  };

  /// <summary>
  ///   <para>The simplify option allows the application to replace the output ASM procedural </para>
  ///   <para>blend spline surface with an analytic surface, but only if the analytic surface </para>
  ///   <para>is an exact match. This option is enabled by default.</para>
  /// </summary>
  /// <returns>Return true or false. The default value is true. </returns>
  ///
  bool simplify() const;

  /// <summary>
  ///   <para>The simplify option allows the application to replace the output ASM procedural </para>
  ///   <para>blend spline surface with an analytic surface, but only if the analytic surface </para>
  ///   <para>is an exact match. This option is enabled by default.</para>
  /// </summary>
  /// <param name="val"> Input specifying true or false </param>
  /// <returns> Return Acad::eOk if successful. </returns>
  void setSimplify(bool simplify);
  /// <summary>
  ///   <para>The solid option allows the implementation to stitch the output surface to the </para>
  ///   <para>input section curves in order to produce a solid body. This function is reserved </para>
  ///   <para>for future use.  Currently, blend surface always output an surface.  It cannot </para>
  ///   <para>output solid.</para>
  /// </summary>
  /// <returns>Return true or false.</returns>
  ///
  bool  solid() const;

  /// <summary>
  ///   <para>The solid option allows the implementation to stitch the output surface to the </para>
  ///   <para>input section curves in order to produce a solid body. This function is reserved </para>
  ///   <para>for future use.  Currently, blend surface always output an surface.  It cannot </para>
  ///   <para>output solid.</para>
  /// </summary>
  /// <param name="val">Input specifying true or false</param>
  /// <returns>Return Acad::eOk if successful.</returns>
  ///
  OdResult  setSolid(bool val);
  /// <summary>
  ///   <para>The quality option controls how many planar curves (aka planar guides) blending </para>
  ///   <para>between the input sections are made by the implementation to constrain the </para>
  ///   <para>output surface. More planar guides will result in a blend surface that has a </para>
  ///   <para>better shape; however, the representation of the surface becomes more complex </para>
  ///   <para>which results in larger storage requirements and larger evaluation times. The </para>
  ///   <para>default value is 5. The value of 0 has a special meaning: In this case the </para>
  ///   <para>output surface will coincide with the standard loft output for the same input </para>
  ///   <para>sections and might have a significantly different shape from the surface </para>
  ///   <para>produced for a quality value >=1.</para>
  /// </summary>
  /// <returns>Return an unsigned integer</returns>
  ///
  OdUInt32  quality() const;

  /// <summary>
  ///   <para>The quality option controls how many planar curves (aka planar guides) blending </para>
  ///   <para>between the input sections are made by the implementation to constrain the </para>
  ///   <para>output surface. More planar guides will result in a blend surface that has a </para>
  ///   <para>better shape; however, the representation of the surface becomes more complex </para>
  ///   <para>which results in larger storage requirements and larger evaluation times. The </para>
  ///   <para>default value is 5. The value of 0 has a special meaning: In this case the </para>
  ///   <para>output surface will coincide with the standard loft output for the same input </para>
  ///   <para>sections and might have a significantly different shape from the surface </para>
  ///   <para>produced for a quality value >=1.</para>
  /// </summary>
  /// <param name="val">Input specifying a non-negative integer value for quality</param>
  /// <returns>Return Acad::eOk if successful</returns>
  ///
  OdResult  setQuality(OdUInt32 val);

  /// <summary>
  ///   <para>The coplanar point is an optional property that can be supplied to constrain the </para>
  ///   <para>set of planes containing the planar guides made by the implementation. If set, </para>
  ///   <para>all guide planes will contain it. By default no coplanar point is set. This </para>
  ///   <para>option is useful in conjunction with the coplanar direction option to define an </para>
  ///   <para>axis contained by all guide planes. An example where such an axis is useful is a </para>
  ///   <para>blend operation between involving a cone: The customer application might prefer </para>
  ///   <para>that all guides planes to contain the cone axis.</para>
  /// </summary>
  /// <returns>Return an AcGePoint3d point</returns>
  ///
  OdGePoint3d  coplanarPoint() const;

  /// <summary>
  ///   <para>The coplanar point is an optional property that can be supplied to constrain the </para>
  ///   <para>set of planes containing the planar guides made by the implementation. If set, </para>
  ///   <para>all guide planes will contain it. By default no coplanar point is set. This </para>
  ///   <para>option is useful in conjunction with the coplanar direction option to define an </para>
  ///   <para>axis contained by all guide planes. An example where such an axis is useful is a </para>
  ///   <para>blend operation between involving a cone: The customer application might prefer </para>
  ///   <para>that all guides planes to contain the cone axis.</para>
  /// </summary>
  /// <param name="pPt">Input specifying an AcGePoint3d point</param>
  /// <returns>Return Acad::eOk if successful</returns>
  ///
  OdResult  setCoplanarPoint(const OdGePoint3d pPt);
  /// <summary>
  ///   <para>The coplanar direction is an optional property that can be supplied to constrain </para>
  ///   <para>the set of planes containing the planar guides made by the implementation. If </para>
  ///   <para>set, all guide planes will contain it (i.e. are parallel to it). By default no </para>
  ///   <para>coplanar direction is set.</para>
  /// </summary>
  /// <returns>Return an AcGeVector3d vector</returns>
  ///
  OdGeVector3d  coplanarDirection() const;

  /// <summary>
  ///   <para>The coplanar direction is an optional property that can be supplied to constrain </para>
  ///   <para>the set of planes containing the planar guides made by the implementation. If </para>
  ///   <para>set, all guide planes will contain it (i.e. are parallel to it). By default no </para>
  ///   <para>coplanar direction is set.</para>
  /// </summary>
  /// <param name="pDir">Input specifying AcGeVector3d vector for coplanar direction</param>
  /// <returns>Return Acad::eOk if successful</returns>
  ///
  OdResult  setCoplanarDirection(const OdGeVector3d pDir);

  /// <summary>
  ///   <para>The drive mode property defines which section(s) "drive" the creation of the </para>
  ///   <para>planes containing the planar guides. Any of the two input sections can be </para>
  ///   <para>selected for that role, or both of them can be used at the same time (the </para>
  ///   <para>default value). The implementation uses the driving sections in conjunction with </para>
  ///   <para>the coplanar point and direction (if supplied).</para>
  /// </summary>
  /// <returns>Return 0,1 or 2 representing kDriveModeFirst, kDriveModeSecond or DriveModeBoth respectively.</returns>
  ///
  OdDbBlendOptions::DriveModeType  driveMode() const;

  /// <summary>
  ///   <para>The drive mode property defines which section(s) "drive" the creation of the </para>
  ///   <para>planes containing the planar guides. Any of the two input sections can be </para>
  ///   <para>selected for that role, or both of them can be used at the same time (the </para>
  ///   <para>default value). The implementation uses the driving sections in conjunction with </para>
  ///   <para>the coplanar point and direction (if supplied).</para>
  /// </summary>
  /// <param name="val">Input integer of value 0, 1, or 2 representing kDriveModeFirst, kDriveModeSecond or DriveModeBoth respectively.</param>
  /// <returns>Return Acad::eOk if successful</returns>
  ///
  OdResult  setDriveMode(OdDbBlendOptions::DriveModeType val);


private:
  friend class OdDbBlendOptionsImpl;
  OdDbBlendOptionsImpl *m_pImpl;
};

#include "TD_PackPop.h"
#endif // _DbBlendOptions_h_Included_
