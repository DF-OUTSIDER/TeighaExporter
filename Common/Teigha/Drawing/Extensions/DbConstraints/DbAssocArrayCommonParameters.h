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

#ifndef DbAssocArrayCommonParameters_INCLUDED_
#define DbAssocArrayCommonParameters_INCLUDED_ /*!DOM*/

#include "DbAssocArrayParameters.h"
//#include "DbAssocActionBody.h"

#include "TD_PackPush.h"

/** \details
  This class represents an array of common parameters.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT ODRX_ABSTRACT OdDbAssocArrayCommonParameters : public OdDbAssocArrayParameters
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocArrayCommonParameters);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocArrayCommonParameters();

  /** \details
    Returns the level count.
  */
  int levelCount(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the level count.
  */
  int levelCount() const;
  
  /** \details
    Returns the level count.
  */
  int levelCount(OdString& expression) const;

  /** \details
    Returns the level spacing.
  */
  double levelSpacing(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the level spacing.
  */
  double levelSpacing() const;
  
  /** \details
    Returns the level spacing.
  */
  double levelSpacing(OdString& expression) const;

  /** \details
    Returns the row count.
  */
  int rowCount(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the row count.
  */
  int rowCount() const;
  
  /** \details
    Returns the row count.
  */
  int rowCount(OdString& expression) const;

  /** \details
    Returns the row elevation.
  */
  double rowElevation(OdString& expression, OdString& evaluatorId ) const;
  
  /** \details
    Returns the row elevation.
  */
  double rowElevation() const;
  
  /** \details
    Returns the row elevation.
  */
  double rowElevation(OdString& expression) const;

  /** \details
    Returns the row spacing.
  */
  double rowSpacing(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the row spacing.
  */
  double rowSpacing() const;
  
  /** \details
    Returns the row spacing.
  */
  double rowSpacing(OdString& expression) const;

  /** \details
    Sets the level count to a given value.
  */
  OdResult setLevelCount(int nLevels, 
                         const OdString& expression = OdString::kEmpty, 
                         const OdString& evaluatorId = OdString::kEmpty,
                         OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the offset of the level spacing to a given value.
  */
  OdResult setLevelSpacing(double offset, 
                           const OdString& expression = OdString::kEmpty, 
                           const OdString& evaluatorId = OdString::kEmpty,
                           OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the row count to a given value.
  */
  OdResult setRowCount(int nRows, 
                       const OdString& expression = OdString::kEmpty, 
                       const OdString& evaluatorId = OdString::kEmpty,
                       OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the row elevation to a given value.
  */
  OdResult setRowElevation(double elevation,
                           const OdString& expression = OdString::kEmpty, 
                           const OdString& evaluatorId = OdString::kEmpty,
                           OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the offset of the row spacing to a given value.
  */
  OdResult setRowSpacing(double offset,
                         const OdString& expression = OdString::kEmpty, 
                         const OdString& evaluatorId = OdString::kEmpty,
                         OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Gets the base point and normal of the base plane.
  */
  OdResult getBasePlane(OdDbVertexRef& basePoint, 
                        OdGeVector3d& normal, 
                        OdDbFaceRef* pFace = NULL) const;

  /** \details
    Sets the base point and normal of the base plane.
  */
  OdResult setBasePlane(const OdDbVertexRef& basePoint, 
                        const OdGeVector3d& normal, 
                        const OdDbFaceRef* pFace = NULL);

  /** \details
    Returns the item locator.
  */
  virtual OdDbItemLocator getCanonicalForm(const OdDbItemLocator& locator) const;

protected:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocArrayCommonParameters(class OdDbImpAssocArrayCommonParameters* pImpl);
};


/** \details
  The typified smart pointer for the array of common parameters. This template class 
  is specialization of the OdSmartPtr class for the OdDbAssocArrayCommonParameters class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocArrayCommonParameters> OdDbAssocArrayCommonParametersPtr;

#include "TD_PackPop.h"

#endif
