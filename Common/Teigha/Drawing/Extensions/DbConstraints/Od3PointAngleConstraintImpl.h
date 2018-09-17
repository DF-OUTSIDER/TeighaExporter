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

#ifndef Od3PointAngleConstraintImpl_INCLUDED
#define Od3PointAngleConstraintImpl_INCLUDED

#include "OdAngleConstraintImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of an angle constraint node measured between 
  three constrained points in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT Od3PointAngleConstraintImpl : public OdAngleConstraintImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  Od3PointAngleConstraintImpl();
  Od3PointAngleConstraintImpl(OdAngleConstraint::SectorType type);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~Od3PointAngleConstraintImpl();

  /** \details
    Updates the dependencies using the transformation matrix.
  */
  virtual void updateDimDependency(const OdGeMatrix3d& matrToWrd);
};

#include "TD_PackPop.h"

#endif
