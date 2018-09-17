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

#ifndef OdDistanceConstraintImpl_INCLUDED
#define OdDistanceConstraintImpl_INCLUDED

#include "OdExplicitConstr.h"
#include "OdExplicitConstraintImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of a distance constraint node applied between 
  two constrained geometries in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDistanceConstraintImpl : public OdExplicitConstraintImpl
{
public:
  /** \details
    Constructors create an instance of this class.
  */
  OdDistanceConstraintImpl();
  OdDistanceConstraintImpl(const OdGeVector3d& direction);
  OdDistanceConstraintImpl(const OdConstraintGroupNodeId consLineId,
      OdDistanceConstraint::DirectionType type = OdDistanceConstraint::kPerpendicularToLine);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDistanceConstraintImpl();

  /** \details
    Sets the fixed direction and direction type to kFixedDirection.
  */
  OdResult setDirection(const OdGeVector3d& direction);

  /** \details
    Sets the direction relative to the constrained line and direction type to kPerpendicularToLine.
  */
  OdResult setDirection(const OdConstraintGroupNodeId consLineId,
                        OdDistanceConstraint::DirectionType type = OdDistanceConstraint::kPerpendicularToLine);

  /** \details
    Returns the direction type of the distance constraint.
  */
  OdDistanceConstraint::DirectionType directionType() const;

  /** \details
    Returns the fixed direction when the direction type is set to kFixedDirection.
  */
  OdGeVector3d direction()           const;

  /** \details
    Returns an ID of the constrained line when the direction type is set to kPerpendicularToLine or kParallelToLine.
  */
  OdConstraintGroupNodeId constrainedLineId() const;

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** \details
    Updates the dependencies using the transformation matrix.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd);

  /*!DOM*/
  void fillCurrentStateObj();

protected:
  /*!DOM*/
  OdDistanceConstraint::DirectionType m_dirType;
  /*!DOM*/
  OdGeVector3d                        m_dirVect;
  /*!DOM*/
  OdConstraintGroupNodeId             m_lineId;
  /*!DOM*/
  OdGePoint3d m_oldPointF;
  /*!DOM*/
  OdGePoint3d m_oldPointS;
  /*!DOM*/
  OdGeLineSeg3d m_oldLineF;
  /*!DOM*/
  OdGeLineSeg3d m_oldLineS;
};

#include "TD_PackPop.h"

#endif
