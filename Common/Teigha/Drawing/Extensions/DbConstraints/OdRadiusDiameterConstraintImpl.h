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

#ifndef OdRadiusDiameterConstraintImpl_INCLUDED
#define OdRadiusDiameterConstraintImpl_INCLUDED

#include "OdExplicitConstr.h"
#include "OdExplicitConstraintImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of a radius or diameter constraint node 
  in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdRadiusDiameterConstraintImpl : public OdExplicitConstraintImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdRadiusDiameterConstraintImpl();
  OdRadiusDiameterConstraintImpl(OdRadiusDiameterConstraint::RadiusDiameterConstrType type);

  /** \details
    Returns the radius type of the radius constraint.
  */
  OdRadiusDiameterConstraint::RadiusDiameterConstrType constrType() const;

  /** \details
    Sets the radius type for the radius constraint.
  */
  OdResult setConstrType(OdRadiusDiameterConstraint::RadiusDiameterConstrType type);

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
protected:
  /*!DOM*/
  OdRadiusDiameterConstraint::RadiusDiameterConstrType m_type;
};

#include "TD_PackPop.h"

#endif
