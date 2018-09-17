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


#ifndef _DbLoftOptions_h_Included_
#define _DbLoftOptions_h_Included_

#include "TD_PackPush.h"
#include "DbEntity.h"
#include "ModelerGeometryCreator.h"

class OdDbLoftOptionsImpl;

/** \details
  Class for setting options used for creating lofted surfaces and lofted solids (by calling OdDbLoftedSurface::createLoftedSurface() or OdDb3dSolid::createLoftedSolid() methods). 
  Loft options are used to control the shape of the resulting surface or solid object. 

  <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbLoftOptions
{
public:
  /** \details 
    Creates an OdDbLoftOptions object. 
  */
  OdDbLoftOptions();
  
  /** \details 
    Copy constructor for the OdDbLoftOptions class.
  */
  OdDbLoftOptions(const OdDbLoftOptions& src);
  
  /** \details 
    Destroys the OdDbLoftOptions object. 
  */
  ~OdDbLoftOptions();

  /** \details 
    Assigns to the OdDbLoftOptions object another object (assignment operator). 
  
    \param rhs [in] Another OdDbLoftOptions object to assign.
  */
  OdDbLoftOptions& operator = (const OdDbLoftOptions& rhs);

  /** \details
    Enumeration for specifying plane normal lofting regimes.
  */
  enum NormalOption 
  {
    /** Plane normal lofting is off: surface direction is calculated at each cross-section. */
    kNoNormal       = 0, 
    /** Direction of the surface at the first cross-section will be the same as the plane normal of the cross-section.*/
    kFirstNormal    = 1, 
    /** Direction of the surface at the last cross-section will be the same as the plane normal of the cross-section.*/
    kLastNormal     = 2, 
    /** Direction of the surface at the first and last cross-sections will be the same as the plane normal of the cross-section.*/
    kEndsNormal     = 3, 
    /** Direction of the surface at each cross-section will be the same as the plane normal of the cross-section.*/
    kAllNormal      = 4, 
    /** Direction of the surface at each cross-section will be the same as the plane normal of the cross-section.*/
    kUseDraftAngles = 5 
  };

  /** \details
    Returns the start draft angle in radians. 
    This is the surface take-off direction at the first cross section. 
    The default value is 0; no draft angle is applied and the optimum angle is calculated at the first cross section.
  */
  double draftStart() const;

  /** \details
    Sets the start draft angle. 
    This is the surface take-off direction at the first cross section. 
    The default value is 0; no draft angle is applied and the optimum angle is calculated at the first cross section.
    
    \param angle [in]  A new value of the start draft angle (radians).
  */
  void setDraftStart(double angle);

  /** \details
    Returns the end draft angle in radians. 
    This is the surface take-off direction at the last cross section. 
    The default value is 0; no draft angle is applied and the optimum angle is calculated at the last cross section.
  */
  double draftEnd() const;

  /** \details
    Sets the end draft angle. 
    This is the surface take-off direction at the last cross section. 
    The default value is 0; no draft angle is applied and the optimum angle is calculated at the last cross section.
  
    \param angle [in]  A new value of the end draft angle (radians).
  */
  void setDraftEnd(double angle);

  /** \details
    Returns the start draft magnitude.
    The value returned by the method determines the magnitude of the surface tangent vector at the first cross-section. 
    The default value is 0; the optimum magnitude is calculated at the last cross section.
  */
  double draftStartMag() const;

  /** \details
    Sets the start draft magnitude.
    The value set by the method determines the magnitude of the surface tangent vector at the first cross-section. 
    The default value is 0; the optimum magnitude is calculated at the last cross section.
    
    \param startMag [in]  A new value of the start draft magnitude.
  */
  void setDraftStartMag(double startMag);

  /** \details
    Returns the end draft magnitude.
    
    The value returned by the method determines the magnitude of the surface tangent vector at the last cross-section. 
    The default value is 0; the optimum magnitude is calculated at the last cross section.
  */
  double draftEndMag() const;

  /** \details
    Sets the end draft magnitude.
    
    The value set by the method determines the magnitude of the surface tangent vector at the last cross-section. 
    The default value is 0; the optimum magnitude is calculated at the last cross section.
    
    \param startMag [in]  A new value of the end draft magnitude.
  */
  void setDraftEndMag(double val);

  /** \details
    Returns the flag determining whether arc length parameterization is applied. 
  */
  bool arcLengthParam() const;

  /** \details
    Sets the flag determining whether arc length parameterization is applied.
    
    \param alParam [in]  A new value of the arc length parameterizaton flag.
  */
  void setArcLengthParam(bool alParam);

  /** \details
    Returns the flag determining whether the noTwist option is applied.
    THe option noTwist can be used to minimize the amount of twist in the surface when the cross-sections are all closed. 
    The default value is true; the start point of each closed cross-section is aligned with the other cross-sections to minimize twist.
  */
  bool noTwist() const;

  /** \details
    Sets the flag determining whether the noTwist option is applied.
    The option noTwist can be used due to minimize the amount of twist in the surface when the cross-sections are all closed. 
    The default value is true; the start point of each closed cross-section is aligned with the other cross-sections to minimize twist.
    
    \param noTwist [in]  A new value of noTwist flag.
  */
  void setNoTwist(bool noTwist);

  /** \details
    Returns the value of the direction alignment flag.
    
    THe direction alignment flag is used to orient each of the cross sections so that the normal vector of each cross section points towards the next cross section. 
    The default value is true. 
    If the cross sections are not all oriented in the same direction, then this flag should be used to avoid creating a twisted or self-intersecting surface.
  */
  bool alignDirection() const;

  /** \details
    Sets the value of the direction alignment flag.
    
    The direction alignment flag is used to orient each of the cross sections so that the normal vector of each cross section points towards the next cross section. 
    The default value is true. 
    If the cross sections are not all oriented in the same direction, then this flag should be used to avoid creating a twisted or self-intersecting surface.
    
    \param alignDir [in]  A new value of the direction alignment flag.
  */
  void setAlignDirection(bool alignDir);

  /** \details
    Returns the value of the simplify flag. 
    If the simplify flag is set to true, simple surface types (such as plane, cylinder, cone, sphere and torus) are created whenever all of the cross sections lie on such a surface. 
    If the simplify flag is set to false, a general surface type will be created. 
    The default value is true. 
  */
  bool simplify() const;

  /** \details
    Sets the value of the simplify flag. 
  
    If the simplify flag is set to true, simple surface types (such as plane, cylinder, cone, sphere and torus) are created whenever all of the cross sections lie on such a surface. 
    If the simplify flag is set to false, a general surface type will be created. 
    The default value is true. 
    
    \param simplify [in]  A new value of the simplify flag.
  */
  void setSimplify(bool simplify);

  /** \details
    Returns the value of the closed flag. 
    
    If the closed flag is set to true, the created surface starts and ends at the first cross section (i.e. the surface is closed). 
    If all of the cross sections are closed, the resulting surface takes a torus-like shape.
    If the closed flag is set to false, the surface is opened or capped with a planar surface at the first and last cross sections. 
    If the flag is set, then there must be at least three cross sections. 
    
    The default value is false. 
  */
  bool closed() const;

  /** \details
    Sets the value of the closed flag. 
    
    If the closed flag is set to true, the created surface starts and ends at the first cross section (i.e. the surface is closed). 
    If all of the cross sections are closed, the resulting surface takes a torus-like shape.
    If the closed flag is set to false, the surface is opened or capped with a planar surface at the first and last cross sections. 
    If the flag is set, then there must be at least three cross sections. 
    
    The default value is false.   
    
    \param closed [in]  A new value of the closed flag.
  */
  void setClosed(bool closed);

  /** \details
    Returns the value of the ruled flag.
    
    If the flag is set to true, a ruled surface will be created between each pair of cross sections, which leads to a coarser surface.  
    If the flag is set to false, then a single smooth surface is created. 
    The default value is false.
  */
  bool ruled() const;

  /** \details
    Sets the value of the ruled flag.
    
    If the flag is set to true, a ruled surface will be created between each pair of cross sections, which leads to a coarser surface.  
    If the flag is set to false, then a single smooth surface is created. 
    The default value is false.
    
    \param ruled [in]  A new value of the ruled flag.
  */
  void setRuled(bool ruled);

  /** \details
    Returns the virtual guide option. 
  */
  bool virtualGuide() const;

  /** \details
    Sets the virtual guide flag. 
    
    \param virtGuide [in]  A new value of the virtual guide flag.
  */
  void setVirtualGuide(bool virtGuide);

  /** \details
    Returns the plane normal flag. 
    
    The default value of the flag is kNoNormal. 
  */
  OdDbLoftOptions::NormalOption normal() const;

  /** \details
    Sets the plane normal flag.
    The default value of the flag is kNoNormal. 
    
    \param option [in]  A new value of the plane normal flag.
  */
  void setNormal(OdDbLoftOptions::NormalOption option);

  /** \details
    Sets loft options from the current values of system variables.
    
    \param pDb [in]  A reference to a database object that is used to get access to system variable values.
    
    \returns Returns eOk if options were successfully set or an appropriate error otherwise.
  */
  OdResult setOptionsFromSysvars(const OdDbDatabase& pDb);

  /** \details
    Sets the system variables values from loft options values.
    
    \param pDb [out]  A reference to a database object that is used to change system variables values.
    
    \returns Returns eOk if system variables were successfully set or an appropriate error otherwise.
  */
  OdResult setSysvarsFromOptions(OdDbDatabase& pDb);

  /** \details
    Validates the loft options values.

    \param displayErrorMessages [in]  Indicates whether error messages are displayed. Default value is false.
    
    \returns Returns eOk if the options values are all valid. 
  */
  OdResult checkOptions(bool displayErrorMessages = false);

  /** \details
    Checks whether cross section curves and path curves are valid for the lofting operation.
  
    \param crossSectionCurves   [in]  A list of cross-section curves.
    \param guideCurves          [in]  A list of guide curves.
    \param pPathCurve           [in]  A pointer to a path curve (can be NULL).
    \param allOpen              [out] A passed-in parameter indicating whether all of the cross-section curves are open.
    \param allClosed            [out] A passed-in parameter indicating whether all of the cross-section curves are closed.
    \param allPlanar            [out] A passed-in parameter indicating whether all of the cross-section curves are planar.
    \param displayErrorMessages [in]  Indicates whether error messages are displayed. Default value is false.
  
    \returns Returns eOk if curves are valid or eInvalidInput otherwise.
  
    \remarks 
    The method is called by createLoftedSurface() and createLoftedSolid() methods, so it is not necessary to call the method separately.
    The method combines calls to check CrossSectionCurves(), checkGuideCurves(), and checkPathCurve() into a single function.
  */
  OdResult checkLoftCurves(
    OdDbEntityPtrArray& crossSectionCurves,
    OdDbEntityPtrArray& guideCurves,
    OdDbEntity* pPathCurve,
    bool& allOpen, 
    bool& allClosed, 
    bool& allPlanar,
    bool displayErrorMessages = false);

  /** \details
    Checks whether cross section curves are valid for the lofting operation. 
   
    \param crossSectionCurves   [in]  A list of cross-section curves.
    \param allOpen              [out] A passed-in parameter indicating whether all of the cross-section curves are open.
    \param allClosed            [out] A passed-in parameter indicating whether all of the cross-section curves are closed.
    \param allPlanar            [out] A passed-in parameter indicating whether all of the cross-section curves are planar.
    \param displayErrorMessages [in]  Indicates whether error messages are displayed. Default value is false.
    
    \returns Returns eOk if curves are valid or eInvalidInput otherwise.
    
    \remarks 
    The method is called by createLoftedSurface() and createLoftedSolid() methods, so it is not necessary to call the method separately().
  */
  OdResult checkCrossSectionCurves(
    OdDbEntityPtrArray& crossSectionCurves,
    bool& allOpen, 
    bool& allClosed, 
    bool& allPlanar,
    bool displayErrorMessages = false);

  /** \details
    Checks whether guide curves are valid for the lofting operation.
    

    \param guideCurves          [in] A list of guide curves.
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.
    
    \returns Returns eOk if guide curves are valid or eInvalidInput otherwise.
    
    \remarks 
    The method is called by createLoftedSurface() and createLoftedSolid() methods, so it is not necessary to call the method separately().
  */
  OdResult checkGuideCurves(OdDbEntityPtrArray& guideCurves,
    bool displayErrorMessages = false);

  /** \details
    Checks whether a path curve is valid for the lofting operation.

    \param pPathCurve           [in] A pointer to the path curve (can be NULL).
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.
    
    \returns Returns eOk if a path curve is valid or eInvalidInput otherwise.
    
    \remarks     
    The method is called by createLoftedSurface() and createLoftedSolid() methods, so it is not necessary to call the method separately().
  */
  OdResult checkPathCurve(OdDbEntity *pPathCurve,
    bool displayErrorMessages = false);


private:
  friend class OdDbLoftOptionsImpl;
  OdDbLoftOptionsImpl *m_pImpl;
};

#include "TD_PackPop.h"
#endif // _DbLoftOptions_h_Included_
