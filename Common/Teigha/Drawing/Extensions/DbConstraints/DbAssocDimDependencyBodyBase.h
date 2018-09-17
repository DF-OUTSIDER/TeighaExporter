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

#ifndef DbAssocDimDependencyBodyBase_INCLUDED_
#define DbAssocDimDependencyBodyBase_INCLUDED_ /*!DOM*/

#include "OdString.h"
#include "OdArray.h"
#include "DbEntity.h"
#include "OdConstrainedGeometry.h"
#include "DbAssocDependencyBody.h"

#include "Ge/GePoint3d.h"
#include "Ge/GeCurve3d.h"

#include "TD_PackPush.h"

/** \details
  This class declares the subentity geometry and its types.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbSubentGeometry
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbSubentGeometry() : mSubentType(OdDb::kNullSubentType), mpCurve(NULL) {}

  OdDbSubentGeometry(const OdGePoint3d& pnt) 
      : mSubentType(OdDb::kVertexSubentType), mPoint(pnt), mpCurve(NULL) {}

  OdDbSubentGeometry(OdGeCurve3d* pCurve)
    : mSubentType(OdDb::kEdgeSubentType), mpCurve(pCurve) {}

  /** \details
    Returns the subentity type.
  */
  OdDb::SubentType type() const { return mSubentType; }

  /** \details
    Returns three-dimensional point if the subentity type is set to OdDb::kVertexSubentType.
  */
  OdGePoint3d point() const { return mPoint; }

  /** \details
    Returns the subentity curve if the subentity type is set to OdDb::kEdgeSubentType.
  */
  OdGeCurve3d* curve() const { return mpCurve; }

private:
  /*!DOM*/
  OdDb::SubentType mSubentType;
  /*!DOM*/
  OdGePoint3d      mPoint;
  /*!DOM*/
  OdGeCurve3d*     mpCurve;
};


/** \details
  This is the abstract base class that represents the dimensional constraint.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocDimDependencyBodyBase : public OdDbAssocDependencyBody
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocDimDependencyBodyBase);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocDimDependencyBodyBase();

  //explicit OdDbAssocDimDependencyBodyBase(OdDbAssocCreateImpObject createImpObject = kOdDbAssocCreateImpObject);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocDimDependencyBodyBase();

  /** \details
    Declares the pure method that gets the entity text and 
    must be overridden in derived classes.
  */
  virtual OdString getEntityTextOverride() const = 0;

  /** \details
    Declares the pure method that sets the entity text and 
    must be overridden in derived classes.
  */
  virtual OdResult setEntityTextOverride(const OdString& newText) = 0;

  /** \details
    Declares the pure method that gets the entity measurement and 
    must be overridden in derived classes.
  */
  virtual double getEntityMeasurementOverride() const = 0;

  /** \details
    Declares the pure method that checks whether the entity attachment is changed and 
    must be overridden in derived classes.
  */
  virtual bool isEntityAttachmentChangedOverride() const = 0;

  /** \details
    Updates the entity position, size, orientation and must be overridden in derived classes.
  */
  virtual OdResult updateDependentOnObjectOverride();

  /** \details
    Returns the dimensional constraint node.
  */
  class OdExplicitConstraint* constraint() const;

  /** \details
    Returns an object ID of the associated variable.
  */
  OdDbObjectId variable() const; // of OdDbAssocVariable

  /** \details
    Gets an array of constrained geometries.
  */
  OdResult getConstrainedGeoms(OdArray<OdConstrainedGeometry*>& geoms) const;

  /** \details
    Gets an array of constrained geometries.
  */
  OdResult getConstrainedGeoms(OdArray<OdDbFullSubentPath>& geoms) const;

  /** \details
    Gets an array of constrained geometries.
  */
  OdResult getConstrainedGeoms(OdArray<OdDbSubentGeometry>& geoms,
                                        OdGeVector3d&  distanceDirection) const;

  /** \details
    Gets the variable name and expression of the dimensional constraint node.
  */
  OdResult getVariableNameAndExpression(OdString& name, OdString& expression, OdString& value) const;

  /** \details
    Gets the entity name and expression of the dimensional constraint node.
  */
  OdResult getEntityNameAndExpression(OdString& name, OdString& expression) const;

  /** \details
    Sets the variable name and expression for the dimensional constraint node.
  */
  OdResult setVariableNameAndExpression(const OdString& name, const OdString& expression);

  /** \details
    Sets the entity name and expression for the dimensional constraint node.
  */
  OdResult setEntityNameAndExpression(const OdString& name, const OdString& expression, const OdString& value);

  /** \details
    Sets the entity and variable name together with expression for the dimensional constraint node.
  */
  OdResult setNameAndExpression(const OdString& name, const OdString& expression); // Both variable and entity

  /** \details
    Checks whether the given textual string can be used as an entity text for variable name and expression, 
    otherwise either the variable name, or expression may be empty.
  */
  OdResult validateEntityText(const OdString& entityTextToValidate, 
                                    OdString& errorMessage) const;

  /** \details
    Composes the entity text from the variable name or expression.
  */
  OdString composeEntityText(int requiredNameFormat = -1) const;

  /** \details
    Measures the current dimensional constraint node based on positions and sizes of its geometries 
    and sets the measured value for the corresponding associated variable.
  */
  OdResult setVariableValueToMeasuredValue() const;

  /** \details
    Deactivates the dimensional constraint node.
  */
  OdResult deactivateConstraint() const;

  /** \details
    Reactivates the dimensional constraint node.
  */
  OdResult reactivateConstraint() const;

  bool isConstraintActive() const; // Has DCM d_node or r_node

  /** \details
    Measures and returns measured value for current dimensional constraint node based on the 
    positions and sizes of its geometries.
  */
  OdResult getMeasuredValue(double& measurement) const;

  /** \details
    Switches the status to be referenced only.
  */
  OdResult setIsReferenceOnly(bool yesNo);

  /** \details
    Gets the name and expression from entity text.
  */
  static OdResult 
  getNameAndExpressionFromEntityText(const OdString& entityText,
                                     bool            useMeasurementIfNoText,
                                     double          measurement,
                                     bool            isAngular,
                                     OdString&       name, 
                                     OdString&       expression);

  /** \details
    Gets the format which is currently used for representing the entity name and expression.
  */
  int getCurrentlyUsedEntityNameFormat() const;

  /** \details
    This method is called when the dependent object is dragged.
  */
  void dragStatus(const OdDb::DragStat status);

  /** \details
    Gets the associated dimensional dependency body from the dependent-on entity.
  */
  static OdResult getFromEntity(const OdDbObjectId& entityId, OdDbObjectId& dimDepBodyId);

  /** \details
    Updates the constraint geometry when dimension points are moved.
  */
  OdResult entityAttachmentPointMoved(const OdArray<OdDbSubentGeometry>& newAttachedGeometries,
                                               double measurement = 0.0);

  /** \details
    Checks whether this method is overridden from the associated dependency body.
  */
  virtual OdResult isRelevantChangeOverride(bool& isRelevChange) const;

  /** \details
    Changes the name and expression in the entity text to set them to the name and expression of 
    the corresponding variable.
  */
  virtual void evaluateOverride();

  /** \details
    Changes the name and expression of the variable to set them to the same name and expression 
    in the entity text.
  */
  virtual void modifiedOverride(const OdDbObject* pDbObj); 

  /** \details
    Erases the corresponding variable when the dependent-on object is erased.
  */
  virtual void erasedOverride(const OdDbObject* pDbObj, bool isErasing);

  /** \details
    Erases the dependent-on entity.
  */
  virtual OdResult subErase(bool erasing);

  /** \details
    Sets the format to the current precision.
  */
  static OdString formatToCurrentPrecision(const OdString& expression, bool isAngular);

  /** \details
    Under normal occurs an associated dimension object is erased if associated dimension dependency body is erased.
    Returns whether the behavior is suppressed or not.
  */
  static bool setEraseDimensionIfDependencyIsErased(bool yesNo);

  /** \details
    Under normal occurs an associated dimension object is erased if associated dimension dependency body is erased.
    Returns whether the behavior is suppressed or not.
  */
  static bool getEraseDimensionIfDependencyIsErased();

  /** \details
    This class implements the notification ignorer.

    Library: Source code provided.
    <group Contraint_Classes> 
  */
  class DBCONSTRAINTS_EXPORT NotificationIgnorer
  {
  public:
    /** \details
      Constructor creates an instance of this class.
    */
    NotificationIgnorer();

    /** \details
      Destructor destroys an instance of this class.
    */
    ~NotificationIgnorer();

    /** \details
      Returns the notification ignored status.
    */
    static bool isIgnoringNotifications();

  private:
    /*!DOM*/
    const bool mPrevIsIgnoringNotifications;
    /*!DOM*/
    static bool smIsIgnoringNotifications;

  };

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dxfOutFields(OdDbDxfFiler* pFiler) const;

  private:
    /*!DOM*/
    OdString m_text;
    /*!DOM*/
    static bool sbEraseDimensionIfDependencyIsErased;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocDimDependencyBodyBase class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocDimDependencyBodyBase> OdDbAssocDimDependencyBodyBasePtr;


/** \details
  Declares the dimensional constraint types
*/
enum OdDbAssocConstraintType
{
    kNoneAssocConstraintType = 0,            // None Associate Constraint 
    kDistanceAssocConstraintType,            // Distance Associate Constraint 
    kHorizontalDistanceAssocConstraintType,  // Horizontal Associate Constraint 
    kVerticalDistanceAssocConstraintType,    // Vertical Associate Constraint 
    kAngle0AssocConstraintType,              // Parallel Counter-Clockwise Associate Constraint 
    kAngle1AssocConstraintType,              // Anti-Parallel Clockwise Associate Constraint
    kAngle2AssocConstraintType,              // Parallel Clockwise Associate Constraint
    kAngle3AssocConstraintType,              // Anti-Parallel Counter-Clockwise Associate Constraint
    kRadiusAssocConstraintType,              // Radius Associate Constraint
    kDiameterAssocConstraintType,            // Diameter Associate Constraint
};

#include "TD_PackPop.h"

#endif

