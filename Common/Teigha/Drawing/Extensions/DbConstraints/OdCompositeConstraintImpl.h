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

#ifndef OdCompositeConstraintImpl_INCLUDED_
#define OdCompositeConstraintImpl_INCLUDED_/*!DOM*/

#include "DbConstraintsExports.h"
#include "OdGeomConstraint.h"
#include "OdGeomConstraintImpl.h"

#include "TD_PackPush.h"

/** \details
  This class represents custom implementation of a composite constraint node in 
  the owning constraint group. This class is base for kind of composite constraint.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdCompositeConstraintImpl : public OdGeomConstraintImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdCompositeConstraintImpl();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdCompositeConstraintImpl();

  /** \details
    Returns an array of pointers to the geometry constraint objects that are owned for the
    composite constraint.
  */
  OdResult getOwnedConstraints(OdArray<OdGeomConstraint*>& apSubConstraints) const;

  /** \details
    Adds a geometry constraint objects to the composite constraint.
  */
  OdResult addOwnedConstraint(const OdGeomConstraint* pCnstr);

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

protected:
  /*!DOM*/
  OdGroupNodeIdArray m_ownedConstraints;
};

#include "TD_PackPop.h"

#endif

