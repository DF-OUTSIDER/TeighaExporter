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

#ifndef dDbAssocArrayPathParameters_INCLUDED_
#define dDbAssocArrayPathParameters_INCLUDED_ /*!DOM*/

#include "DbAssocArrayCommonParameters.h"

#include "TD_PackPush.h"

/** \details
  This class represents an array of path parameters.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocArrayPathParameters : public OdDbAssocArrayCommonParameters
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocArrayPathParameters);

  /** \details
    Declares the grip modes.
  */
  enum GripModes
  {
    kStretchGrip             = 1 << 0,
    kRowCountGrip            = 1 << 1,
    kRowSpacingGrip          = 1 << 2,
    kUniformRowSpacingGrip   = 1 << 3,
    kLevelCountGrip          = 1 << 4,
    kLevelSpacingGrip        = 1 << 5,
    kUniformLevelSpacingGrip = 1 << 6,
    kItemCountGrip           = 1 << 7,
    kItemSpacingGrip         = 1 << 8, 
    kUniformItemSpacingGrip  = 1 << 9
  };

  /** \details
    Declares the methods.
  */
  enum Method
  {
    kDivide,
    kMeasure
  };

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocArrayPathParameters(double itemSpacing = 1,
                               double rowSpacing = 1,
                               double levelSpacing = 1,
                               int itemCount = 1,
                               int rowCount = 1,
                               int levelCount = 1,
                               double rowElevation = 0);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocArrayPathParameters();

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
  int itemCount(OdString & expression) const;

  /** \details
    Returns the item spacing.
  */
  double itemSpacing(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the item count.
  */
  double itemSpacing() const;
  
  /** \details
    Returns the item count.
  */
  double itemSpacing(OdString& expression) const;

  /** \details
    Returns the edge reference.
  */
  OdDbEdgeRef path() const;

  /** \details
    Returns the method identifier.
  */
  Method method() const;

  /** \details
    Checks the alignment of items.
  */
  bool alignItems() const;

  /** \details
    Returns the start offset.
  */
  double startOffset(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the start offset.
  */
  double startOffset() const;

  /** \details
    Returns the end offset.
  */
  double endOffset(OdString& expression, OdString& evaluatorId) const;
  
  /** \details
    Returns the end offset.
  */
  double endOffset() const;
  
  /** \details
    Sets the item count to a given value.
  */
  OdResult setItemCount(int nItems, 
                        const OdString& expression = OdString::kEmpty, 
                        const OdString& evaluatorId = OdString::kEmpty,
                        OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty));

  /** \details
    Sets the item spacing to a given value.
  */
  OdResult setItemSpacing(double offset, 
                          const OdString& expression = OdString::kEmpty,
                          const OdString& evaluatorId = OdString::kEmpty,
                          OdString& pErrorMessage = const_cast<OdString&>(OdString::kEmpty) );

  /** \details
    Sets the edge path.
  */
  OdResult setPath(const OdDbEdgeRef& edgeRef);

  /** \details
    Sets the method.
  */
  OdResult setMethod(Method method);

  /** \details
    Sets the alignment of items.
  */
  OdResult setAlignItems(bool bAlignItems);

  /** \details
    Sets the start offset to a given value.
  */
  OdResult setStartOffset(double startOffset,
                          const OdString& expression = OdString::kEmpty,
                          const OdString& evaluatorId = OdString::kEmpty,
                          OdString* pErrorMessage = NULL);

  /** \details
    Sets the end offset to a given value.
  */
  OdResult setEndOffset(double endOffset, 
                        const OdString& expression = OdString::kEmpty,
                        const OdString& evaluatorId = OdString::kEmpty,
                        OdString* pErrorMessage = NULL);

  /** \details
    Sets the edge direction.
  */
  OdResult setPathDirection(bool bAlongParams);

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
  
  /** \details
    Gets the item position.
  */
  OdResult getItemPosition(const OdDbItemLocator& locator, 
                           OdGePoint3d& position) const;

protected:
  /*!DOM*/
  OdDbAssocArrayPathParameters(class OdDbImpAssocArrayPathParameters* pImpl); // disabled Copy constructor
};

/** \details
  The typified smart pointer for the array of path parameters. This template class 
  is specialization of the OdSmartPtr class for the OdDbAssocArrayPathParameters class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocArrayPathParameters> OdDbAssocArrayPathParametersPtr;

#include "TD_PackPop.h"

#endif
