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

#ifndef OdDbImpCenterObjectsParameters_INCLUDED_
#define OdDbImpCenterObjectsParameters_INCLUDED_ /*!DOM*/
#include "map"
#include "DbConstraintsExports.h"
#include "DbEntity.h"
#include "DbFiler.h"

#include "Ge/GePoint3d.h"
#include "Ge/GeMatrix3d.h"
#include "DbAssocArrayItem.h"
#include "DbImpAssocActionBody.h"
#include "DbImpAssocArrayParameters.h"

#include "TD_PackPush.h"
/** \details
This class represents the custom implementation of an array of common parameters.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpSmartCenterCommonParameters : public OdDbImpAssocArrayParameters
{
public:
  /** \details
  Constructor creates an instance of this class.
  */
  OdDbImpSmartCenterCommonParameters();

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpSmartCenterCommonParameters();

  OdResult setCenterDepId(const OdDbObjectId input,
    const OdString& expression,
    const OdString& evaluatorId,
    OdString& pErrorMessage);

  OdResult setSubentLineType(const OdString,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setSubentLineWeight(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setSubentLinetypeScale(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setCachedAssociateStatus(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setAssociationToBeBroken(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

  OdDbObjectId centerDepId(
    const OdDbDatabase* pDb,
    OdString& expression,
    OdString& evaluatorId
    ) const;

  OdString subentLineType(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double subentLineWeight(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double subentLinetypeScale(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double cachedAssociateStatus(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double associationToBeBroken(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  virtual OdDbItemLocator getCanonicalForm(const OdDbItemLocator& locator) const;
  /** \details
  Sets the owner for this array of parameters.
  */
  OdResult setOwner(class OdDbAssocActionBody* pOwner);
protected:

  /*!DOM*/
  friend class OdDbSmartCenterCommonParameters;
  friend class OdDbSmartCenterMarkParameters;
  friend class OdDbSmartCenterLineParameters;
  friend class OdDbImpAssocArrayActionBody;
  friend class OdDbImpSmartCenterActionBody;

  virtual OdResult GetItemByLocator(const OdDbItemLocator &locator, OdDbAssocArrayItemPtr &pItem) const;
  
  class OdDbAssocActionBody* m_pOwner;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

/** \details
This class represents the custom implementation of an array of rectangular parameters.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbImpSmartCenterMarkParameters : public OdDbImpSmartCenterCommonParameters
{
public:
  /** \details
  Constructor creates an instance of this class.
  */

  OdDbImpSmartCenterMarkParameters(
    double HorizontalStartExtension = 1e+60,
    double HorizontalEndExtension = 1e+60,
    double VerticalStartExtension = 1e+60,
    double VerticalEndExtension = 1e+60,
    double HorizontalStartOvershoot = 3.5,
    double HorizontalEndOvershoot = 3.5,
    double VerticalStartOvershoot = 3.5,
    double VerticalEndOvershoot = 3.5,
    OdString CrossHairLengthExpression = L"0.1x",
    OdString CrossHairGapExpression = L"0.05x",
    OdDbObjectId CenterDepId = NULL,
    OdDb::Visibility extensionLinesVisibility = OdDb::kVisible,
    //eLeftLine_visibility,
    //eRightLine_visibility,
    //eBottomLine_visibility,
    //eTopLine_visibility,
    double CachedCircleRadius = 1,
    OdString SubentLineType = OdString::kEmpty,
    double SubentLineWeight = -1,
    double SubentLinetypeScale = 1,
    double CachedAssociateStatus = 1
    );

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpSmartCenterMarkParameters();

  OdResult setHorizontalStartExtension(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setHorizontalEndExtension(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setVerticalStartExtension(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setVerticalEndExtension(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setHorizontalStartOvershoot(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setHorizontalEndOvershoot(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setVerticalStartOvershoot(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setVerticalEndOvershoot(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setCrossHairLengthExpression(const OdString,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setCrossHairGapExpression(const OdString,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setextensionLinesVisibility(const OdDb::Visibility,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setCachedCircleRadius(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  OdResult setCachedRotation(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  ////////////////////////////////
  double horizontalStartExtension(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double horizontalEndExtension(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId
    ) const;
  double verticalStartExtension( 
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId
    ) const;
  double verticalEndExtension(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double horizontalStartOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double horizontalEndOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double verticalStartOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double verticalEndOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  OdString crossHairLengthExpression(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  OdString crossHairGapExpression(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  OdDb::Visibility extensionLinesVisibility(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double cachedCircleRadius(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  double cachedRotation(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;
  //////////////////////////////////
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;
  /** \details
  Gets an array of items.
  */
  virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items);
  virtual OdGeMatrix3d CalculateDefaultMatrix(const OdDbItemLocator &locator);

protected:
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DBCONSTRAINTS_EXPORT OdDbImpSmartCenterLineParameters : public OdDbImpSmartCenterCommonParameters
{
public:
  /** \details
  Constructor creates an instance of this class.
  */

  OdDbImpSmartCenterLineParameters(
    double StartExtLength = 1e+60,
    double EndExtLength = 1e+60,
    double StartOvershoot = 3.5,
    double EndOvershoot = 3.5,
    int EdgeRefDirectionEnding = 1,
    int EdgeRefDirectionEnding2 = 0,
    OdDbObjectId CenterDepId = NULL,
    double CachedCenterLineLength = 0,
    double CachedCenterLineDirectionX = 0,
    double CachedCenterLineDirectionY = 0,
    double CachedCenterLineDirectionZ = 0,
    OdString SubentLineType = OdString::kEmpty,
    double SubentLineWeight = -1,
    double SubentLinetypeScale = 1,
    double CachedAssociateStatus = 1
    );

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpSmartCenterLineParameters();

  OdResult edgeRefDirEndingArray(OdArray<int> & inputArray);

  OdResult setEdgeRefDirEndingArray(const OdArray<int> &,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

  OdResult setStartExtension(const double,
        const OdString& expression, // = OdString::kEmpty
        const OdString& evaluatorId, // = OdString::kEmpty
        OdString& pErrorMessage);

  OdResult setEndExtension(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

  OdResult setStartOvershoot(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

  OdResult setEndOvershoot(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

  OdResult setEdgeRefDirEnding(const int, const int,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

  OdResult setCachedCenterLineLength(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  
  OdResult setCachedCenterLineDirection(OdGeVector3d const &,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  ///////////////
  double startExtension(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  double endExtension(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  double startOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  double endOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  int edgeRefDirEnding(
    int,
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  double cachedCenterLineLength(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  OdGeVector3d cachedCenterLineDirection(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items);
  virtual OdGeMatrix3d CalculateDefaultMatrix(const OdDbItemLocator &locator);
};

#include "TD_PackPop.h"

#endif

