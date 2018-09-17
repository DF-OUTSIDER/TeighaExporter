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

#ifndef DbAssocArrayRectangularParameters_INCLUDED_
#define DbAssocArrayRectangularParameters_INCLUDED_ /*!DOM*/

#include "DbAssocArrayCommonParameters.h"

#include "TD_PackPush.h"

/** \details
  This class represents an array of rectangular parameters.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocArrayRectangularParameters : public OdDbAssocArrayCommonParameters
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocArrayRectangularParameters);

  /** \details
    Declares the grip modes.
  */
  enum GripModes
  {
      kStretchGrip             = 1 << 0,
      kRowCountGrip            = 1 << 1,
      kRowSpacingGrip          = 1 << 2,
      kUniformRowSpacingGrip   = 1 << 3,
      kColumnCountGrip         = 1 << 4,
      kColumnSpacingGrip       = 1 << 5,
      kUniformColSpacingGrip   = 1 << 6,
      kRowColumnCountGrip      = 1 << 7,
      kRowColumnSpacingGrip    = 1 << 8,
      kColumnDirectionGrip     = 1 << 9,
      kRowDirectionGrip        = 1 << 10,
      kLevelCountGrip          = 1 << 11,
      kLevelSpacingGrip        = 1 << 12,
      kUniformLevelSpacingGrip = 1 << 13,
      kRowAxisAngleGrip        = 1 << 14,
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocArrayRectangularParameters(double columnSpacing = 1.,
                                      double rowSpacing = 1.,
                                      double levelSpacing = 1.,
                                      int columnCount = 1,
                                      int rowCount = 1,
                                      int levelCount = 1,
                                      double rowElevation = 0.,
                                      double axesAngle = 90.);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocArrayRectangularParameters();

  /** \details
    Returns the column count.
  */
  int columnCount(OdString& expression, 
                  OdString& evaluatorId) const;
  
  /** \details
    Returns the column count.
  */
  int columnCount() const;
  
  /** \details
    Returns the column count.
  */
  int columnCount(OdString& expression) const;

  /** \details
    Returns the column spacing.
  */
  double columnSpacing(OdString& expression, 
                       OdString& evaluatorId) const;
  
  /** \details
    Returns the column spacing.
  */
  double columnSpacing() const;
  
  /** \details
    Returns the column spacing.
  */
  double columnSpacing(OdString& expression) const;
  
  /** \details
    Returns the axes angle.
  */
  double axesAngle(OdString& expression, 
                   OdString& evaluatorId) const;
  
  /** \details
    Returns the axes angle.
  */
  double axesAngle() const;
  
  /** \details
    Returns the axes angle.
  */
  double axesAngle(OdString& expression) const;

  /** \details
    Returns the axis direction.
  */
  OdGeVector3d axisDirection(OdGeVector3d* pYAxis = NULL) const;

  /** \details
    Sets the column count to a given value.
  */
  OdResult setColumnCount(int nColumns, 
                          const OdString& expression = OdString::kEmpty,
                          const OdString& evaluatorId = OdString::kEmpty,
                          OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the column spacing to a given value.
  */
  OdResult setColumnSpacing(double offset, 
                            const OdString& expression = OdString::kEmpty,
                            const OdString& evaluatorId = OdString::kEmpty,
                            OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the axes angle to a given value.
  */
  OdResult setAxesAngle(double axesAngle, 
                        const OdString& expression = OdString::kEmpty,
                        const OdString& evaluatorId = OdString::kEmpty,
                        OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the axis direction to a given value.
  */
  OdResult setAxisDirection(const OdGeVector3d& xAxis);

  /** \details
    Gets an array of items.
  */
  virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items) const;

  /** \details
    Gets the item position.
  */
  virtual OdResult getItemPosition(const OdDbItemLocator& locator,
                                   OdGePoint3d& position,
                                   OdGeMatrix3d& xform) const;
protected:
  /*!DOM*/
  OdDbAssocArrayRectangularParameters(class OdDbImpAssocArrayRectangularParameters* pImpl); // disabled Copy constructor
};

/** \details
  The typified smart pointer for the array of rectangular parameters. This template class 
  is specialization of the OdSmartPtr class for the OdDbAssocArrayRectangularParameters class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocArrayRectangularParameters> OdDbAssocArrayRectangularParametersPtr;

#include "TD_PackPop.h"

#endif
