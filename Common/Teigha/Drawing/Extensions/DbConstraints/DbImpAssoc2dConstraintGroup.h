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

#ifndef DbImpAssoc2dConstraintGroup_INCLUDED_
#define DbImpAssoc2dConstraintGroup_INCLUDED_ /*!DOM*/


#include "DbConstraintsExports.h"
#include "DbImpAssocAction.h"
#include "OdConstraintGroupNode.h"
#include "OdConstrainedGeometry.h"
#include "OdGeomConstraint.h"
#include "OdConstraintGroupNodeImpl.h"
#include "OdExplicitConstr.h"

#include "Ge/GePlane.h"
#define STL_USING_MAP
#include "OdaSTL.h"

class OdDbAssocGeomDependency;

#include "TD_PackPush.h"


/** \details
  This class represents the custom implementation of the planar constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssoc2dConstraintGroup : public OdDbImpAssocAction
{
public:
  /** \details
    Gets an implementation of the planar constraint group node.
  */
  static OdConstraintGroupNodeImpl* getConstraintGroupNodeImpl(OdConstraintGroupNode *node)
  {
    return node->m_pImpl;
  }
  static OdConstraintGroupNodeImpl* getConstraintGroupNodeImpl(const OdConstraintGroupNode *node)
  {
    return const_cast<OdConstraintGroupNodeImpl*>(node->m_pImpl);
  }

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssoc2dConstraintGroup();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssoc2dConstraintGroup();

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
  OdResult addConstrainedGeometry(const OdDbFullSubentPath& subentPath, OdConstrainedGeometry*& pConsGeom, OdDbAssoc2dConstraintGroup *pGroup);

  /** \details
    Removes and erases an existing constrain geometry from this planar constraint group.
  */
  OdResult deleteConstrainedGeometry(const OdDbObjectId& geomDependencyId);
  OdResult deleteConstrainedGeometry(const OdDbObjectIdArray& aDestDepToRemove);


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
                                  OdConstrainedImplicitPoint::ImplicitPointType* pPtType = NULL,
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
  OdResult addGeometricalConstraint(OdDbObjectId actionId,
                                    OdGeomConstraint::GeomConstraintType type, 
                                    const OdDbFullSubentPathArray& aPaths,
                                    OdGeomConstraint** ppNewConstraint  = NULL);

  OdResult addGeometricalConstraint(OdDbObjectId actionId,
                                    OdGeomConstraint::GeomConstraintType type,
                                    const OdArray<OdConstrainedGeometry*>& apConsGeoms,
                                    OdGeomConstraint** ppNewConstraint  = NULL);
  /** \details
    Checks whether the planar constraint group is empty.
  */
  bool isEmpty() const;

  /** \details
    Creates and adds a distance constraint to this planar constraint group 
    using two given constrain geometry objects.
  */
  OdResult addDistanceConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                 OdConstrainedGeometry* pConsGeom1,
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
  OdResult addAngleConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                              OdConstrainedLine* pConsLine1,
                              OdConstrainedLine* pConsLine2,
                              OdAngleConstraint::SectorType sectorType,
                              OdDbObjectId valueDependencyId,
                              OdDbObjectId dimDependencyId, // May be null
                              OdAngleConstraint** ppNewAngConstraint = NULL);

  /** \details
    Creates and adds an three-points angle constraint to this planar constraint group 
    using three given constrain points.
  */
  OdResult add3PointAngleConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                    OdConstrainedPoint* pConsPoint1,
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
  OdResult addRadiusDiameterConstraint(OdDbAssoc2dConstraintGroup *pGroup,
                                       OdConstrainedGeometry* pConsGeom,
                                       OdRadiusDiameterConstraint::RadiusDiameterConstrType type,
                                       OdDbObjectId valueDependencyId,
                                       OdDbObjectId dimDependencyId, // May be null
                                       OdRadiusDiameterConstraint** ppNewRadDiaConstraint = NULL);

  /** \details
    Deletes the nodes of the constraint group.
  */
  OdResult deleteNodes(OdArray<OdGeomConstraint*> &iaConstraints, OdArray<OdConstrainedGeometry*> &iaGeometries);

  /** \details
    Deletes the constraint geometries.
  */
  OdResult deleteGeometries(OdArray<OdConstrainedGeometry*> &ioaGeometries);

  /** \details
    Deletes the constraint help parameters.
  */
  OdResult deleteHelpParameters(OdArray<OdHelpParameter*> &ioaHelpParameters, OdConstraintGroupNodeId id);

  /** \details
    Deletes the constraints.
  */
  OdResult deleteConstraints(OdArray<OdGeomConstraint*> &iaConstraints,
                             OdArray<OdConstrainedGeometry*> &ioaGeometries);

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
#endif

  /** \details
    Evaluates this planar constraint group.
  */
  virtual void evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback);

  /** \details
     This is the notification call of the corresponding postProcessAfterDeepClone() method 
     of the parent class that owns this action.
  */
  virtual OdResult postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap);

  /** \details
     This is the notification call of the corresponding postProcessAfterDeepCloneCancel() method 
     of the parent class that owns this action.
  */
  virtual OdResult postProcessAfterDeepCloneCancel(OdDbAssocAction *pAction, OdDbIdMapping& idMap);

  /** \details
    Moves the constraint geometry.
  */
  OdResult moveConstrainedGeometry(const OdDbFullSubentPath& subentPath, OdConstrainedGeometry*& pConsGeom,
                                   OdDbAssocGeomDependency * pExistingDep,
                                   OdDbAssoc2dConstraintGroup *pGroup);

  /** \details
     This is the notification call of the corresponding method 
     of the parent class that owns this action.
  */
  void onAddConstrainedGeometry(const OdDbFullSubentPath& subentPath, OdConstrainedGeometry* pConsGeom);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Reads the .dwg file data of this object for 2010 version. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInNodes2010(OdDbDwgFiler* pFiler, OdDbObjectId objectId, OdUInt32 ver);

  /** \details
    Reads the .dwg file data of this object for 2013 version. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInNodes2013(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Writes the .dwg file data of this object for 2010 version. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dwgOutNodes2010(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Writes the .dwg file data of this object for 2013 version. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dwgOutNodes2013(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Reads the .dxf file data of this object for 2010 version. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dxfInNodes2010(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Reads the .dxf file data of this object for 2013 version. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dxfInNodes2013(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Writes the .dxf file data of this object for 2010 version. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dxfOutNodes2010(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Writes the .dxf file data of this object for 2013 version. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dxfOutNodes2013(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  void composeForLoad( OdDbObject* pObj, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo );

  //remove in full version
  OdResult updateDatabaseObjects(OdDbAssoc2dConstraintGroup *pGroup);
  void audit(OdDbAssoc2dConstraintGroup* pGroup, OdDbAuditInfo* pAuditInfo);

protected:
  /*!DOM*/
  OdGePlane               m_plane;
  /*!DOM*/
  OdConstraintGroupNodeId m_seedId;
  /*!DOM*/
  typedef std::map<OdConstraintGroupNodeId, OdConstraintGroupNodePtr> OdConstraintGroupNodeMap;
  /*!DOM*/
  OdConstraintGroupNodeMap m_mapNodes;
  /*!DOM*/
  bool          m_Unk_Bool;
  /*!DOM*/
  OdDbObjectId  m_Unk_ObjId;
private:
  /*!DOM*/
  void postReadWork();
  /*!DOM*/
  bool isConstraintOfThisTypeExist(const OdArray<OdGeomConstraint*> aConstr,
                                   const OdRxClass* pObjType,
                                   OdGeomConstraint** ppConstraint);
  /*!DOM*/
  OdResult getConstraintGroupNodeObjects(OdArray<OdConstraintGroupNodePtr>& aResObj, OdRxClass* pObjType);
  /*!DOM*/
  bool findDatumLine(OdConstrainedDatumLine** ppDatumLine, const OdGeVector3d& dir);
  /*!DOM*/
  OdResult tieNode(OdConstraintGroupNodePtr node, OdDbObjectId actionId);
  /*!DOM*/
  OdResult removeNode(OdConstraintGroupNode* pNode);
  /*!DOM*/
  void tieGeomsAndCnstr3(OdGeomConstraint *pCnstr,
                         OdConstrainedGeometry* pGeomF,
                         OdConstrainedGeometry* pGeomS,
                         OdConstrainedGeometry* pGeomT = NULL);
  /*!DOM*/
  void tieDimAndCnstr(OdExplicitConstraint *pCnstr,
                      OdDbObjectId valueDependencyId,
                      OdDbObjectId dimDependencyId,
                      OdDbAssoc2dConstraintGroup *pGroup);
  /*!DOM*/
  void tieGeomsAndCnstr(OdConstrainedGeometry* pGeomF, OdConstrainedGeometry* pGeomS, OdGeomConstraint *pCnstr);
  /*!DOM*/
  void tieGeomsAndCnstrWithHeplPrm(OdConstrainedGeometry* pGeomF, OdConstrainedGeometry* pGeomS,
                                   OdGeomConstraint *pCnstr, OdHelpParameter *pHelpPrmF, OdHelpParameter *pHelpPrmS);

  /*!DOM*/
  OdGeomConstraint* addHorizontalConstraint(OdConstrainedGeometry* pGeomF,
                                            OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addHorizontalConstraint(OdConstrainedGeometry* pGeomF,
                                            OdConstrainedGeometry* pGeomS,
                                            OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addVerticalConstraint(OdConstrainedGeometry* pGeomF,
                                          OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addVerticalConstraint(OdConstrainedGeometry* pGeomF,
                                          OdConstrainedGeometry* pGeomS,
                                          OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addParallelConstraint(OdConstrainedGeometry* pGeomF,
                                          OdConstrainedGeometry* pGeomS,
                                          OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addPerpendicularConstraint(OdConstrainedGeometry* pGeomF,
                                               OdConstrainedGeometry* pGeomS,
                                               OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addNormalConstraint(OdConstrainedGeometry* pGeomF,
                                        OdConstrainedGeometry* pGeomS,
                                        OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addColinearConstraint(OdConstrainedGeometry* pGeomF,
                                          OdConstrainedGeometry* pGeomS,
                                          OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addCoincidentConstraint(OdConstrainedGeometry* pGeomF,
                                          OdConstrainedGeometry* pGeomS,
                                          OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addConcentricConstraint(OdConstrainedGeometry* pGeomF,
                                            OdConstrainedGeometry* pGeomS,
                                            OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addTangentConstraint(OdConstrainedGeometry* pGeomF,
                                         OdConstrainedGeometry* pGeomS,
                                         OdHelpParameter* pHelpParamF,
                                         OdHelpParameter* pHelpParamS,
                                         OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addEqualRadiusConstraint(OdConstrainedGeometry* pGeomF,
                                             OdConstrainedGeometry* pGeomS,
                                             OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addEqualLengthConstraint(OdConstrainedGeometry* pGeomF,
                                             OdConstrainedGeometry* pGeomS,
                                             OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addSymmetricConstraint(OdConstrainedGeometry* pGeomF,
                                           OdConstrainedGeometry* pGeomS,
                                           OdConstrainedGeometry* pGeomLine,
                                           OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addFixConstraint(OdConstrainedGeometry* pGeom, OdDbObjectId actionId);

  /*!DOM*/
  OdGeomConstraint* addG2SmoothConstraint(OdConstrainedGeometry* pGeomF,
                                          OdConstrainedGeometry* pGeomS,
                                          OdDbObjectId actionId);

  //need for composite constraints

  /*!DOM*/
  OdGeomConstraint* addEqualCurvatureConstraint(OdConstrainedGeometry* pGeomF,
                                                OdConstrainedGeometry* pGeomS,
                                                OdHelpParameter* pHelpParamF,
                                                OdHelpParameter* pHelpParamS,
                                                OdDbObjectId actionId);

  /*!DOM*/
  OdHelpParameter* createHelpParameter(OdConstrainedImplicitPoint *pPoint, OdDbObjectId actionId);

  //common code for vert/horz cnstr

  /*!DOM*/
  OdGeomConstraint* addVertHorzConstraint(OdConstrainedGeometry* pGeomF,
                                          OdDbObjectId actionId,
                                          const OdRxClass *desc,
                                          const OdGeVector3d dir,
                                          const OdString name);

  /*!DOM*/
  OdGeomConstraint* addVertHorzConstraint(OdConstrainedGeometry* pGeomF,
                                          OdConstrainedGeometry* pGeomS,
                                          OdDbObjectId actionId,
                                          const OdRxClass *desc,
                                          const OdGeVector3d dir,
                                          const OdString name);

  /*!DOM*/
  OdConstrainedGeometry* addConstrainedGeometry(OdDbAssocGeomDependencyPtr pGeomDep);
  /*!DOM*/
  OdConstrainedGeometry* addPoint(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addCircle(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addArc(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addBoundedLine(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addRay(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addXLine(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addEllipse(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addEllipseArc(OdDbObjectId geomDepId);
  /*!DOM*/
  OdConstrainedGeometry* addSpline(OdDbObjectId geomDepId, const OdGeNurbCurve3d& spline);

  /*!DOM*/
  OdConstrainedImplicitPoint* addImplicitPoint(OdConstrainedCurve *pCurve, OdConstrainedImplicitPoint::ImplicitPointType type, int index = -1);

  /*!DOM*/
  virtual void doPostEvaluateJob(OdDbAssocAction *pAction);
};

#include "TD_PackPop.h"

#endif

