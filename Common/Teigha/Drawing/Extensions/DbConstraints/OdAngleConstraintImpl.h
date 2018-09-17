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

#ifndef OdAngleConstraintImpl_INCLUDED
#define OdAngleConstraintImpl_INCLUDED

#include "OdExplicitConstr.h"
#include "OdExplicitConstraintImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of the angle constraint node. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdAngleConstraintImpl : public OdExplicitConstraintImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdAngleConstraintImpl();
  OdAngleConstraintImpl(OdAngleConstraint::SectorType type);

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdAngleConstraintImpl();

  /** \details
    Returns the angle sector type of the angle constraint.
  */
  OdAngleConstraint::SectorType sectorType() const;

  /** \details
    Sets the angle sector type for the angle constraint.
  */
  OdResult setSectorType(OdAngleConstraint::SectorType type);

  /** \details
    Returns the current angle factor of the angle constraint.
  */
  static double angleMultiplier();

  /** \details
    Sets the current angle factor of the angle constraint. The value is OdaPI/180 by default.
  */
  static void setAngleMultiplier(double multiplier);

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

  /** \details
    Updates the dependencies using the transformation matrix.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd);
protected:
  /*!DOM*/
  static double ANGLE_MULTIPLIER;
  /*!DOM*/
  OdAngleConstraint::SectorType m_secType;
};

#include "TD_PackPop.h"

#endif
