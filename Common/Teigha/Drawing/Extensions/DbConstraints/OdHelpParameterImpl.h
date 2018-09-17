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

#ifndef OdHelpParameterImpl_INCLUDED_
#define OdHelpParameterImpl_INCLUDED_ /*!DOM*/

#include "OdConstraintGroupNodeImpl.h"
#include "OdConstrainedGeometry.h"
#include "OdGeomConstraint.h"

#include "TD_PackPush.h"

/** \details
  This class represent custom implementation of a help parameter node in the owning constraint group.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdHelpParameterImpl : public OdConstraintGroupNodeImpl
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdHelpParameterImpl();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdHelpParameterImpl();

  /** \details
    Returns a pointer to the geomtry constraint node to which this help parameter is connected. 
    This method does not take in account of equal parameter applied to this help parameter.
  */
  OdResult getConnectedGeometry(OdConstrainedGeometry*& pGeometry) const;

  /** \details
    Returns a pointer to geometry constrain object to which the help parameter is connected.
  */
  OdResult getConnectedConstraint(OdGeomConstraint*& pConstraint) const;

  /** \details
    Returns the array of pointers to equal help parameter constraint object to which the constraint is connected.
  */
  OdResult getConnectedEqualparamConstraints(OdArray<OdEqualHelpParameterConstraint*>& apEqualParamConstrs) const;

  /** \details
    Returns the value assigned to the help parameter (larger than or equal to 0.0).
  */
  double getValue() const;

  /** \details
    Sets the value for the help parameter (larger than or equal to 0.0).
  */
  void setValue(const double &val);

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
  double m_value;
  /*!DOM*/
  bool m_UnkBool;  //maybe possibility of changing
};

#include "TD_PackPop.h"

#endif
