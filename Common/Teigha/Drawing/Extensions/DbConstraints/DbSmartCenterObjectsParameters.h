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

#ifndef DbCenterObjectsParameters_INCLUDED_
#define DbCenterObjectsParameters_INCLUDED_ /*!DOM*/

#include "DbAssocArrayParameters.h"
#include "DbAssocArrayCommonParameters.h"
#include "DbImpSmartCenterObjectsParameters.h"

#include "TD_PackPush.h"

/** \details
This class represents an array of common parameters.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT ODRX_ABSTRACT OdDbSmartCenterCommonParameters : public OdDbAssocArrayParameters
{
public:
  ODRX_DECLARE_MEMBERS(OdDbSmartCenterCommonParameters);

  static double MAX_EXTENSIONS()
  {
      return 1e+60;
  }
  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbSmartCenterCommonParameters();

  /** \details
  Returns the item locator.
  */
  virtual OdDbItemLocator getCanonicalForm(const OdDbItemLocator& locator) const;
  OdResult setOwner(class OdDbAssocActionBody* pOwner);

  OdResult setCenterDepId(const OdDbObjectId input,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

  OdDbImpSmartCenterCommonParameters* impSmartCenterParameters();
protected:
  OdDbSmartCenterCommonParameters(class OdDbImpSmartCenterCommonParameters* pImpl);
};

typedef OdSmartPtr<OdDbSmartCenterCommonParameters> OdDbSmartCenterCommonParametersPtr;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** \details
This class represents an array of center mark parameters.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbSmartCenterMarkParameters : public OdDbSmartCenterCommonParameters
{
public:
//  ODRX_DECLARE_MEMBERS(OdDbSmartCenterMarkParameters);
//  ODRX_DECLARE_MEMBERS_GENERIC(OdRxClass, ClassName)
//public:                                                                                              

  /** Casts the specified pointer to an ClassName SmartPointer. **/                     
static OdSmartPtr<OdDbSmartCenterMarkParameters> cast(const OdRxObject* pObj)                                          
{                                                
if (pObj)                                                                                        
  return OdSmartPtr<OdDbSmartCenterMarkParameters>(((OdDbSmartCenterMarkParameters*)pObj->queryX(OdDbSmartCenterMarkParameters::desc())), kOdRxObjAttach);

return (OdDbSmartCenterMarkParameters*)0;                                                                            
}                                                                                                  

static OdRxClass* g_pDesc;                                                                         

/** Returns the static ClassType description object associated with this object. **/  
/** This function is for use only when the class type of this object is known.          **/  
/** If the class type of this object is unknown, use instead isA().                           **/  
static OdRxClass* desc();                                                                   
/** Returns the ClassType description instance associated with this object.      **/  
/** This function is for use only when the class type of this object is unknown.        **/  
/** If the class type of this object is known, use instead desc().                            **/  
virtual OdRxClass* isA() const;   
/** Returns the Protocol Extension object for this object.        **/               
/** Return null if there is no Protocol Extension object is found.    **/           
virtual OdRxObject* queryX(const OdRxClass* protocolClass) const;                   
/** Creates a new instance of this object type.                   **/               
/** Returns a SmartPointer to the new instance.                       **/           
static OdRxObjectPtr pseudoConstructor();                                           
/** Creates a new instance of this object type.                   **/               
/** Returns a SmartPointer to the new instance.                       **/           
static OdSmartPtr<OdDbSmartCenterMarkParameters> createObject()
{ 
  if (!desc()) 
    throw OdError(eNotInitializedYet); 
  return desc()->create(); 
}        
/** Registers ClassName with Teigha.                           **/                
static void rxInit();                                                             
static void rxInit(AppNameChangeFuncPtr appNameChangeFunc);                       
/* Unregisters ClassName with Teigha.                          **/                
static void rxUninit();

  /** \details
  Declares the grip modes.
  */
  //enum GripModes
  //{
  //  kStretchGrip = 1 << 0,
  //  kRowCountGrip = 1 << 1,
  //  kRowSpacingGrip = 1 << 2,
  //  kUniformRowSpacingGrip = 1 << 3,
  //  kColumnCountGrip = 1 << 4,
  //  kColumnSpacingGrip = 1 << 5,
  //  kUniformColSpacingGrip = 1 << 6,
  //  kRowColumnCountGrip = 1 << 7,
  //  kRowColumnSpacingGrip = 1 << 8,
  //  kColumnDirectionGrip = 1 << 9,
  //  kRowDirectionGrip = 1 << 10,
  //  kLevelCountGrip = 1 << 11,
  //  kLevelSpacingGrip = 1 << 12,
  //  kUniformLevelSpacingGrip = 1 << 13,
  //  kRowAxisAngleGrip = 1 << 14,
  //};

  /** \details
  Constructor creates an instance of this class.
  */
    OdDbSmartCenterMarkParameters
    (
      double HorizontalStartExtension = MAX_EXTENSIONS(),
      double HorizontalEndExtension = MAX_EXTENSIONS(),
      double VerticalStartExtension = MAX_EXTENSIONS(),
      double VerticalEndExtension = MAX_EXTENSIONS(),
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
  virtual ~OdDbSmartCenterMarkParameters();

  virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items) const;
  virtual OdResult getItemPosition(const OdDbItemLocator &, OdGePoint3d &, OdGeMatrix3d &) const;

  OdResult setCachedCircleRadius(const double,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);
  
  protected:
  /*!DOM*/
    OdDbSmartCenterMarkParameters(class OdDbImpSmartCenterMarkParameters* pImpl); // disabled Copy constructor
};

/** \details
The typified smart pointer for the array of center mark parameters. This template class
is specialization of the OdSmartPtr class for this class.

\sa
<link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbSmartCenterMarkParameters> OdDbSmartCenterMarkParametersPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents an array of center line parameters.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbSmartCenterLineParameters : public OdDbSmartCenterCommonParameters
{
public:
  /** Casts the specified pointer to an ClassName SmartPointer. **/
  static OdSmartPtr<OdDbSmartCenterLineParameters> cast(const OdRxObject* pObj)
  {
    if (pObj)
      return OdSmartPtr<OdDbSmartCenterLineParameters>(((OdDbSmartCenterLineParameters*)pObj->queryX(OdDbSmartCenterLineParameters::desc())), kOdRxObjAttach);

    return (OdDbSmartCenterLineParameters*)0;
  }

  static OdRxClass* g_pDesc;

  /** Returns the static ClassType description object associated with this object. **/
  /** This function is for use only when the class type of this object is known.          **/
  /** If the class type of this object is unknown, use instead isA().                           **/
  static OdRxClass* desc();
  /** Returns the ClassType description instance associated with this object.      **/
  /** This function is for use only when the class type of this object is unknown.        **/
  /** If the class type of this object is known, use instead desc().                            **/
  virtual OdRxClass* isA() const;
  /** Returns the Protocol Extension object for this object.        **/
  /** Return null if there is no Protocol Extension object is found.    **/
  virtual OdRxObject* queryX(const OdRxClass* protocolClass) const;
  /** Creates a new instance of this object type.                   **/
  /** Returns a SmartPointer to the new instance.                       **/
  static OdRxObjectPtr pseudoConstructor();
  /** Creates a new instance of this object type.                   **/
  /** Returns a SmartPointer to the new instance.                       **/
  static OdSmartPtr<OdDbSmartCenterLineParameters> createObject()
  {
    if (!desc())
      throw OdError(eNotInitializedYet);
    return desc()->create();
  }
  /** Registers ClassName with Teigha.                           **/
  static void rxInit();
  static void rxInit(AppNameChangeFuncPtr appNameChangeFunc);
  /* Unregisters ClassName with Teigha.                          **/
  static void rxUninit();

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbSmartCenterLineParameters
    (
      double StartExtLength = MAX_EXTENSIONS(),
      double EndExtLength = MAX_EXTENSIONS(),
      double StartOvershoot = 3.5,
      double EndOvershoot = 3.5,
      int EdgeRefDirectionEnding = 1,
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
  virtual ~OdDbSmartCenterLineParameters();

  virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items) const;
  virtual OdResult getItemPosition(const OdDbItemLocator &, OdGePoint3d &, OdGeMatrix3d &) const;

  double startOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  double endOvershoot(
    OdString& expression, // = OdString::kEmpty
    OdString& evaluatorId // = OdString::kEmpty
    ) const;

  OdResult edgeRefDirEndingArray(OdArray<int> &);// , OdArrayMemCopyReallocator<int> > &);

  OdResult setEdgeRefDirEndingArray(const OdArray<int> & inputArr,
    const OdString& expression, // = OdString::kEmpty
    const OdString& evaluatorId, // = OdString::kEmpty
    OdString& pErrorMessage);

protected:
  OdDbSmartCenterLineParameters(class OdDbImpSmartCenterLineParameters* pImpl);
};

typedef OdSmartPtr<OdDbSmartCenterLineParameters> OdDbSmartCenterLineParametersPtr;

#include "TD_PackPop.h"

#endif

