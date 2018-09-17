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

#ifndef DbAssoc2dConstraintGroup_INCLUDED_
#define DbAssoc2dConstraintGroup_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocAction.h"
#include "OdConstraintGroupNode.h"
#include "OdConstrainedGeometry.h"
#include "OdGeomConstraint.h"
#include "OdExplicitConstr.h"

class OdDbAssocGeomDependency;
class OdAutoConstrainEvaluationCallback;

#include "TD_PackPush.h"

/** \details
  This class represents a set of geometry constraint relations between subentities which
  must lie in a same plane.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssoc2dConstraintGroup : public OdDbAssocAction
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssoc2dConstraintGroup);

  /** \details
    Declares the solution status.
  */
  enum SolutionStatus
  {
    kWellDefined = 0,   // All geometries are fully constrained. The freedom degree of the model is 0.
    kUnderConstrained,  // Some or all geometries are under-constrained. The freedom degree of the model is more than 0.
    kOverConstrained,   // Some or all geometries are over-constrained. It is because duplicate or redundant constraints have been added.
    kInconsistent,      // Some or all geometries are inconsistent. It is because invalid constraints have been added.
    kNotEvaluated,      // Some or all geometries are not evaluated. It means new constrained geometries have been added since the last evaluation.
    kNotAvailable,      // Fatal errors, not able to get the solution status.
    kRejectedByClient,  // Evaluation produced a result that may be considered valid.
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssoc2dConstraintGroup();

  void audit(OdDbAuditInfo* pAuditInfo) ODRX_OVERRIDE;

  /** \details
    Sets the work plane to associate it with this planar constraint group.
  */
  OdResult setWorkPlane(const OdGePlane plane);

  /** \details
    Returns the work plane associated with this planar constraint group.
  */
  const OdGePlane getWorkPlane() const;

#if 0
  /** \details
    Applies the transformation matrix for this planar constraint group.
  */
  OdResult transformActionBy(const OdGeMatrix3d& transform); 

  /** \details
    Returns the remaining degree of freedom of this planar constraint group.
  */
  int getDOF() const;

  /** \details
    Returns the solution status of this planar constraint group.
  */
  SolutionStatus solutionStatus(bool bAlsoCheckForConstraints = true) const;

  /** \details
    Returns the evaluation status of a given constrain geometry.
  */
  SolutionStatus geometryStatus(const OdConstrainedGeometry* pConsGeom) const;

  /** \details
    Returns the evaluation status of a given constraint.
  */
  SolutionStatus constraintStatus(const OdGeomConstraint* pConstraint) const;
#endif
  /** \details
    Adds a constrain geometry to this planar constraint group.
  */
  OdResult addConstrainedGeometry(const OdDbFullSubentPath& subentPath, OdConstrainedGeometry*& pConsGeom);

  /** \details
    Removes and erases an existing constrain geometry from this planar constraint group.
  */
  OdResult deleteConstrainedGeometry(const OdDbObjectId& geomDependencyId);


  /** \details
    Gets a pointer to the corresponding internally held the constrain geometry.
  */
  OdResult getConstrainedGeometry(const OdDbFullSubentPath& fullSubentPath,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  bool bCreateArcLineMid = false);

  /** \details
    Gets a pointer to the corresponding internally held the constrain geometry.
  */
  OdResult getConstrainedGeometry(const OdDbAssocGeomDependency* pGeomDependency,
                                  OdConstrainedGeometry*&   pConsGeom,
                                  OdConstrainedImplicitPoint::ImplicitPointType* pPtType = 0,
                                  int defPtIndex = -1,
                                  bool bCreateArcLineMid = false);

  /** \details
    Gets all constrain geometry objects of this planar constraint group, including the implicit points 
    associated with corresponding constrain curves.
  */
  OdResult getConstrainedGeometries(OdArray<OdConstrainedGeometry*>& aConstGeom) const;

  /** \details
    Creates and adds a geometrical constraint to this planar constraint group.
  */
  OdResult addGeometricalConstraint(OdGeomConstraint::GeomConstraintType type, 
                                    const OdDbFullSubentPathArray& aPaths,
                                    OdGeomConstraint** ppNewConstraint  = NULL);

  
  /** \details
    Creates and adds a geometrical constraint to this planar constraint group.
  */
  OdResult addGeometricalConstraint(OdGeomConstraint::GeomConstraintType type,
                                    const OdArray<OdConstrainedGeometry*>& apConsGeoms,
                                    OdGeomConstraint** ppNewConstraint  = NULL);

  /** \details
    Creates and adds a distance constraint to this planar constraint group 
    using two given constrain geometry objects.
  */
  OdResult addDistanceConstraint(OdConstrainedGeometry* pConsGeom1,
                                 OdConstrainedGeometry* pConsGeom2,
                                 OdDistanceConstraint::DirectionType directionType,
                                 OdDbObjectId valueDependencyId,
                                 OdDbObjectId dimDependencyId, // May be null
                                 const OdGeVector3d* pFixedDirection = NULL,
                                 const OdConstrainedLine* pDirectionLine = NULL,
                                 OdDistanceConstraint** ppNewDisConstraint = NULL);

  /** \details
    Creates and adds an angle constraint to this planar constraint group 
    using two given constrain lines.
  */
  OdResult addAngleConstraint(OdConstrainedLine* pConsLine1,
                              OdConstrainedLine* pConsLine2,
                              OdAngleConstraint::SectorType sectorType,
                              OdDbObjectId valueDependencyId,
                              OdDbObjectId dimDependencyId, // May be null
                              OdAngleConstraint** ppNewAngConstraint = NULL);

  /** \details
    Creates and adds an three-points angle constraint to this planar constraint group 
    using three given constrain points.
  */
  OdResult add3PointAngleConstraint(OdConstrainedPoint* pConsPoint1,
                                    OdConstrainedPoint* pConsPoint2,
                                    OdConstrainedPoint* pConsPoint3,
                                    OdAngleConstraint::SectorType sectorType,
                                    OdDbObjectId valueDependencyId,
                                    OdDbObjectId dimDependencyId, // May be null
                                    Od3PointAngleConstraint** ppNewAngConstraint = NULL);

  /** \details
    Creates and adds an radius or diameter constraint to this planar constraint group 
    using a given constrain geometry.
  */
  OdResult addRadiusDiameterConstraint(OdConstrainedGeometry* pConsGeom,
                                       OdRadiusDiameterConstraint::RadiusDiameterConstrType type,
                                       OdDbObjectId valueDependencyId,
                                       OdDbObjectId dimDependencyId, // May be null
                                       OdRadiusDiameterConstraint** ppNewRadDiaConstraint = NULL);

  /** \details
    Deletes an existing geometry constrain object held by this planar constraint group.
  */
  OdResult deleteConstraint(OdGeomConstraint* pGeomConst);

  /** \details
    Returns all geometry constrain objects in this planar constraint group, 
    including topological and dimensional constraints.
  */
  OdResult getConstraints(OdArray<OdGeomConstraint*>& apConstraints) const;
#if 0
  /** \details
    Defines the constraints which can be added to an under-constrained model.
  */
  OdResult autoConstrain(const OdDbFullSubentPathArray& aPaths,
                         const OdGeTol& tol,
                         OdAutoConstrainEvaluationCallback* pCallback);
#endif
  /** \details
    Gets all associated geometry dependency which are connected by the planar constraint group.
  */
  OdResult getAllConnectedGeomDependencies(const OdDbObjectIdArray& srcGeomDependencyIds,
                                           OdDbObjectIdArray& allConnectedGeomDependencyIds) const;

  /** \details
    Returns a pointer to this planar constraint group.
  */
  OdConstraintGroupNode* getGroupNodePtr(const OdConstraintGroupNodeId nodeId) const;
#if 0
  /** \details
    Regenerate the dimension system.
  */
  void regenDimensionSystem();

  /** \details
     This is the notification call of the corresponding method of the parent class 
     which owns this action to inform the constraint group that the dependent entity 
     given by the geometry dependency has been mirrored.
  */
  OdResult geometryMirrored(OdDbAssocGeomDependency* pGeomDependency);
  
  /** \details
    Adds the callback to this planar constraint group.
  */
  static void addGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback);

  /** \details
    Removes the callback from this planar constraint group.
  */
  static void removeGlobalCallback(class OdDbAssoc2dConstraintCallback* pCallback);

  /** \details
    Returns the callback of this planar constraint group.
  */
  static class OdDbAssoc2dConstraintCallback* globalCallback();
#endif

};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssoc2dConstraintGroup class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssoc2dConstraintGroup> OdDbAssoc2dConstraintGroupPtr;

/** \details
  This class implements the callback for the planar constraint group.
  Client code can register callback(s) derived from this class that are then called 
  when events are triggered within the planar constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssoc2dConstraintCallback
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssoc2dConstraintCallback() {}

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssoc2dConstraintCallback() {}

  /** \details
    Allows client code to inhibit erasing a dimension constraint when it is not satisfied 
    and would be normally erased during a relax evaluation.
  */
  virtual bool canBeRelaxed(const class OdExplicitConstraint* /*pConstraint*/)
  {
     return false;
  }

  /** \details
    For internal use.
  */
  virtual void constraintDeactivated(const OdExplicitConstraint * /*pConstraint*/,
                                     bool /* bDeactivated */ = true) {}
};

/** \details
  This class implements the callback for the planar constraint group to provide 
  constraint priorities and priority override for auto-constraint evaluation.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdAutoConstrainEvaluationCallback
{
  /*!DOM*/
  friend class OdDbImpAssoc2dConstraintGroup;
  /*!DOM*/
  friend class OdAutoConstraintEvalManager;
public:
  /** \details
    This is the notification call when the system searches a possible constraint between 
    a given set of constrain geometries.
  */
  virtual void getConstraintPriorityOverride(OdGeomConstraint::GeomConstraintType /*type*/, 
        const OdArray<const OdConstrainedGeometry*>& /*geometries*/, int& /*priority*/) const
  {
      return; //!DOM No override.
  }

  /** \details
    Defines an order of the constraint types.
  */
  virtual void getAutoConstrainPriority(OdArray<OdGeomConstraint::GeomConstraintType>& constraintList) const
  {
      constraintList.setLogicalLength(0);
      for(int type = (int)OdGeomConstraint::kHorizontal;
          type < (int)OdGeomConstraint::kEqualRadius; ++type)
      {
          constraintList.append((OdGeomConstraint::GeomConstraintType)type);
      }
  }

  /** \details
    Checks whether the evaluation is cancelled. If the method returns True the auto-constraint 
    evaluation from DCM is cancelled.
  */
  virtual bool isEvaluationCancelled() { return false; }

  /** \details
    Gets a list of constraint geometries found by auto-constraint evaluation and 
    returns the number of constraints.
  */
  int getTotalConstraints(OdArray<OdGeomConstraint*>* pConstraints) const;

private:  
  /*!DOM*/
  int getAutoConstraintMask() const;

  /*!DOM*/
  void validate(void* pData);

  /*!DOM*/
  bool isValidConstraintType(OdGeomConstraint::GeomConstraintType type,
        const OdGeomConstraint* pConstraint) const;

  /*!DOM*/
  void constraintAdded(OdGeomConstraint* pNewCons);

private:
  /*!DOM*/
  OdArray<OdGeomConstraint*> mConstraints;
};

#include "TD_PackPop.h"

#endif

