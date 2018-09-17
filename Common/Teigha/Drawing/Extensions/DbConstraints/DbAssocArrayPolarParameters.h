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

#ifndef DbAssocArrayPolarParameters_INCLUDED_
#define DbAssocArrayPolarParameters_INCLUDED_ /*!DOM*/

#include "DbAssocArrayCommonParameters.h"

#include "TD_PackPush.h"

/** \details
  This class represents an array of polar parameters.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocArrayPolarParameters : public OdDbAssocArrayCommonParameters
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocArrayPolarParameters);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocArrayPolarParameters(double angle = OdaToRadian(60), 
                                double rowSpacing = 200,
                                double levelSpacing = 200,
                                int itemCount = 6,
                                int rowCount = 1, 
                                int levelCount = 1,
                                double rowElevation = 0);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocArrayPolarParameters(void);

  /** \details
    Declares the grip modes.
  */
  enum GripModes
  {
    kCenterPointGrip         = 1 << 0, 
    kStretchRadiusGrip       = 1 << 1,
    kRowCountGrip            = 1 << 2,
    kRowSpacingGrip          = 1 << 3,
    kUniformRowSpacingGrip   = 1 << 4,
    kLevelCountGrip          = 1 << 5,
    kLevelSpacingGrip        = 1 << 6,
    kUniformLevelSpacingGrip = 1 << 7,
    kItemCountGrip           = 1 << 8,
    kAngleBetweenItemsGrip   = 1 << 9, 
    kFillAngleGrip           = 1 << 10
  };

  /** \details
    Declares the directions.
  */
  enum Direction
  {
    kClockwise,
    kCounterClockwise,
  };

  /** \details
    Returns the item count.
  */
  int itemCount(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the item count.
  */
  int itemCount() const;
  
  /** \details
    Returns the item count.
  */
  int itemCount(OdString& expression) const;

  /** \details
    Returns the angle between items.
  */
  double angleBetweenItems(OdString& expression, OdString& evaluatorId) const;
  double angleBetweenItems() const;
  double angleBetweenItems(OdString& expression) const;

  /** \details
    Fills all items to the angle.
  */
  double fillAngle(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the angle between items.
  */
  double fillAngle() const;
  
  /** \details
    Returns the angle between items.
  */
  double fillAngle(OdString& expression) const;

  /** \details
    Returns the start angle.
  */
  double startAngle(OdString& expression, OdString& evaluatorId) const;

  /** \details
    Returns the direction.
  */
  Direction direction() const;

  /** \details
    Checks the rotate of items.
  */
  bool rotateItems() const;

  /** \details
    Returns the radius.
  */
  double radius(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the radius.
  */
  double radius() const;
  
  /** \details
    Returns the radius.
  */
  double radius(OdString& expression) const;

  /** \details
    Sets the item count to a given value.
  */
  OdResult setItemCount(int nItems, 
                        const OdString& expression = OdString::kEmpty, 
                        const OdString& evaluatorId = OdString::kEmpty, 
                        OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty) );

  /** \details
    Sets the angle between items to a given value.
  */
  OdResult setAngleBetweenItems(double angle, const OdString& expression = OdString::kEmpty, 
      const OdString& evaluatorId = OdString::kEmpty, OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the fill angle for items to a given value.
  */
  OdResult setFillAngle(double fillAngle, const OdString& expression = OdString::kEmpty, 
      const OdString& evaluatorId = OdString::kEmpty, OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the start angle to a given value.
  */
  OdResult setStartAngle(double angle, const OdString& expression = OdString::kEmpty, 
      const OdString& evaluatorId = OdString::kEmpty, OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Switches the direction between clockwise and counterclockwise.
  */
  OdResult setDirection(Direction direction);

  /** \details
    Switches the rotate of items.
  */
  OdResult setRotateItems(bool bRotateItems);

  /** \details
    Sets the radius to a given value.
  */
  OdResult setRadius(double radius, const OdString& expression = OdString::kEmpty, 
      const OdString& evaluatorId = OdString::kEmpty, OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Gets an array of items.
  */
  virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items) const;

  /** \details
    Gets the item position.
  */
  virtual OdResult getItemPosition(const OdDbItemLocator& locator, 
      OdGePoint3d& position, OdGeMatrix3d& xform) const;

protected:
  /*!DOM*/
  OdDbAssocArrayPolarParameters(class OdDbImpAssocArrayPolarParameters* pImpl); // disabled Copy constructor
};

/** \details
  The typified smart pointer for the array of polar parameters. This template class 
  is specialization of the OdSmartPtr class for the OdDbAssocArrayPolarParameters class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocArrayPolarParameters> OdDbAssocArrayPolarParametersPtr;

#include "TD_PackPop.h"

#endif
