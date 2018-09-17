/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
#ifndef __DG_VARIABLES_H__
#define __DG_VARIABLES_H__

#include "TD_PackPush.h"
#include "DgItemTypeLibrary.h"
#include "DgModel.h"

#define STL_USING_MAP
#include "OdaSTL.h"

//------------------------------------------------------------------------------------

class OdDgVariableValueVariant;

//------------------------------------------------------------------------------------

class OdDgExpressionItem;
typedef OdSmartPtr<OdDgExpressionItem> OdDgExpressionItemPtr;

class TG_EXPORT OdDgExpressionItem : public OdRxObject
{
public:
  enum OdDgExpressionItemType
  {
    kUnknown       = 0,
    kConstant      = 1,
    kVariable      = 2,
    kAdd           = 3,
    kSubtract      = 4,
    kMultiply      = 5,
    kDivide        = 6,
    kExponent      = 7,
    kGroup         = 8,
    kNegative      = 9,
    kFunction      = 10,
    kSpecialConst  = 11,
    kCondition     = 12,
    kAnd           = 13,
    kOr            = 14,
    kNot           = 15,
    kEqualOrNot    = 16,
    kCompare       = 17,
    kModulo        = 18,
    kVariableId    = 255
  };
public:
  ODRX_DECLARE_MEMBERS(OdDgExpressionItem);

  virtual OdDgExpressionItemType getType() const = 0;

  virtual OdBinaryData getItemData() const = 0;
  virtual void setItemData( const OdBinaryData& binData ) = 0;

  virtual OdUInt32 getOperandCount() const = 0;
  virtual OdDgExpressionItemPtr getOperand( OdUInt32 uIndex ) const = 0;
  virtual void     setOperand( OdUInt32 uIndex, const OdDgExpressionItemPtr& pOperand ) = 0;

  virtual OdString getAsString() const = 0;
  virtual bool     calculate( std::map<OdDgElementId, OdDgVariableValueVariant> mapVariableId_Value, OdDgVariableValueVariant& retVal ) = 0;

  static OdDgExpressionItemPtr createObject( OdDgExpressionItemType uType );
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgProxyExpressionItem : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgProxyExpressionItem );
  virtual void setType( const OdDgExpressionItemType& uType ) = 0;

  virtual void addOperand( const OdDgExpressionItemPtr& pOperand ) = 0;
  virtual void deleteOperand( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgProxyExpressionItem> OdDgProxyExpressionItemPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgExpressionConstValueVariant
{
public:

  enum OdDgExpressionConstValueVariantType
  {
    kNumeric  = 5,
    kDistance = 6,
    kAngled   = 7,
    kInt32    = 8,
    kArea     = 0x1e
  };
public:
  OdDgExpressionConstValueVariant();
  OdDgExpressionConstValueVariant( OdInt32 iValue );
  OdDgExpressionConstValueVariant( const OdDgExpressionConstValueVariantType& uValueType, double dValue );
  virtual ~OdDgExpressionConstValueVariant();

  OdDgExpressionConstValueVariantType getDataType() const;

  OdInt32 getInt32Value() const;
  void    setInt32Value( OdInt32 iValue );

  double  getDoubleValue() const;
  void    setDoubleValue( const OdDgExpressionConstValueVariantType& uValueType, double dValue );

private:
  OdDgExpressionConstValueVariantType m_uType;
  OdInt32                             m_iValue;
  double                              m_dValue;
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgConstantValue : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgConstantValue );
  virtual OdDgExpressionConstValueVariant getValue() const = 0;
  virtual void                  setValue( const OdDgExpressionConstValueVariant& value ) = 0;
};

typedef OdSmartPtr<OdDgConstantValue> OdDgConstantValuePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgVariableValue : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgVariableValue );
  virtual OdString getVariableName() const = 0;
  virtual void     setVariableName( const OdString& strVarName ) = 0;
};

typedef OdSmartPtr<OdDgVariableValue> OdDgVariableValuePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgVariableIdValue : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgVariableIdValue );
  virtual OdDgElementId getVariableId() const = 0;
  virtual void     setVariableId( const OdDgElementId& idVariable ) = 0;
};

typedef OdSmartPtr<OdDgVariableIdValue> OdDgVariableIdValuePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgArithmeticOperator : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgArithmeticOperator );
  virtual void     setType( const OdDgExpressionItemType& uType ) = 0;
};

typedef OdSmartPtr<OdDgArithmeticOperator> OdDgArithmeticOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgGroupOperator : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgGroupOperator );
};

typedef OdSmartPtr<OdDgGroupOperator> OdDgGroupOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgNegativeOperator : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgNegativeOperator );
};

typedef OdSmartPtr<OdDgNegativeOperator> OdDgNegativeOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgConditionOperator : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgConditionOperator );

  virtual OdDgExpressionItemPtr getCondition() const = 0;
  virtual void                  setCondition( const OdDgExpressionItemPtr& pExpr ) = 0;

  virtual OdDgExpressionItemPtr getPositiveCase() const = 0;
  virtual void                  setPositiveCase( const OdDgExpressionItemPtr& pExpr ) = 0;

  virtual OdDgExpressionItemPtr getNegativeCase() const = 0;
  virtual void                  setNegativeCase( const OdDgExpressionItemPtr& pExpr ) = 0;
};

typedef OdSmartPtr<OdDgConditionOperator> OdDgConditionOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgCompareOperator : public OdDgExpressionItem
{
public:
  enum OdDgCompareOperatorType
  {
    kGreater        = 0x15,
    kLess           = 0x16,
    kGreaterOrEqual = 0x17,
    kLessOrEqual    = 0x18
  };
public:
  ODRX_DECLARE_MEMBERS( OdDgCompareOperator );

  virtual OdDgCompareOperatorType getOperatorType() const = 0;
  virtual void                    setOperatorType( const OdDgCompareOperatorType& uType ) = 0;
};

typedef OdSmartPtr<OdDgCompareOperator> OdDgCompareOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgFunctionOperator : public OdDgExpressionItem
{
public:
  enum OdDgFunctionOperatorType
  {
    kSqrt       = 0,
    kLimit      = 1,
    kSin        = 2,
    kMax        = 3,
    kMin        = 4,
    kInteger    = 5,
    kNumeric    = 6,
    kDistance   = 7,
    kAngle      = 8,
    kAbs        = 9,
    kACos       = 10,
    kASin       = 11,
    kATan       = 12,
    kCos        = 13,
    kTan        = 14,
    kFloor      = 15,
    kCeil       = 16,
    kLog        = 17,
    kLog10      = 18,
    kTrunc      = 19,
    kRound      = 20,
    kOdd        = 21,
    kSign       = 22,
    kDegrees    = 23,
    kArea       = 24
  };
public:
  ODRX_DECLARE_MEMBERS( OdDgFunctionOperator );

  virtual OdDgFunctionOperatorType getFunctionType() const = 0;
  virtual void                     setFunctionType( const OdDgFunctionOperatorType& uType ) = 0;
  virtual void addOperand( const OdDgExpressionItemPtr& pOperand ) = 0;
  virtual void deleteOperand( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgCompareOperator> OdDgCompareOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgLogicalOperator : public OdDgExpressionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgLogicalOperator );
  virtual void     setType( const OdDgExpressionItemType& uType ) = 0;
};

typedef OdSmartPtr<OdDgLogicalOperator> OdDgLogicalOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgEqualOrNotOperator : public OdDgExpressionItem
{
public:
  enum OdDgEqualOrNotOperatorType
  {
    kEqual          = 0x19,
    kNotEqual       = 0x1a
  };
public:
  ODRX_DECLARE_MEMBERS( OdDgEqualOrNotOperator );

  virtual OdDgEqualOrNotOperatorType getOperatorType() const = 0;
  virtual void                       setOperatorType( const OdDgEqualOrNotOperatorType& uType ) = 0;
};

typedef OdSmartPtr<OdDgEqualOrNotOperator> OdDgEqualOrNotOperatorPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgSpecialConstant : public OdDgExpressionItem
{
public:
  enum OdDgSpecialConstantType
  {
    kPi        = 0,
    kE         = 1,
    kSqrt2     = 2,
    kTrue      = 3,
    kFalse     = 4,
    kSqrt1_2   = 5,
    kLn2       = 6,
    kLn10      = 7,
    kLog2E     = 8,
    kLog10E    = 9
  };
public:
  ODRX_DECLARE_MEMBERS( OdDgSpecialConstant );

  virtual OdDgSpecialConstantType getConstantType() const = 0;
  virtual void                    setConstantType( const OdDgSpecialConstantType& uType ) = 0;
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgExpressionVariable : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgExpressionVariable);

  virtual OdBinaryData getData() const = 0;
  virtual void setData( const OdBinaryData& binData ) = 0;

  virtual OdString getResultVariableName() const = 0;
  virtual void     setResultVariableName( const OdString& strName ) = 0;

  virtual OdString getExpressionString() const = 0;
  virtual OdDgExpressionItemPtr getExpression() const = 0;
  virtual void setExpression( OdDgExpressionItemPtr& pExpression ) = 0;

  virtual bool getUsedVariables( const OdDgElementId& idVariableList, OdDgElementIdArray& arrUsedVariables ) const = 0;
  virtual bool calculate( std::map<OdDgElementId, OdDgVariableValueVariant>& mapVariableId_Value, OdDgVariableValueVariant& retVal ) = 0;
  virtual void removeVariable( const OdDgElementId& idVariable, const OdDgVariableValueVariant& variableValue ) = 0;
};

typedef OdSmartPtr<OdDgExpressionVariable> OdDgExpressionVariablePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgVariable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgVariable);
  public:
    enum OdDgVariableType
    {
      kProxy       = 0,
      kDistance    = 1,
      kAngle       = 2,
      kNumber      = 3,
      kInteger     = 4,
      kBoolean     = 5,
      kArea        = 6
    };

    enum OdDgVariableScope
    {
      kInstance    = 0,
      kDefinition  = 1
    };

  public:

    virtual OdString getName() const;
    virtual void     setName( const OdString& strName );

    virtual OdString getDisplayName() const;
    virtual void     setDisplayName( const OdString& strDisplayName );

    virtual OdString getUnitsDescription() const;
    virtual void     setUnitsDescription( const OdString& strUnits );

    virtual OdDgVariableType getType() const;
    virtual void     setType( const OdDgVariableType& uType );

    virtual OdDgVariableScope getScope() const;
    virtual void     setScope( const OdDgVariableScope& uScope );

    virtual bool     getReadOnlyFlag() const;
    virtual void     setReadOnlyFlag( bool bSet );

    virtual bool     getVisibleFlag() const;
    virtual void     setVisibleFlag( bool bSet );

    virtual bool     getExpressionBasedFlag() const;

    virtual OdDgExpressionVariablePtr getExpression() const;
    virtual bool                      setExpression( OdDgExpressionVariablePtr& pExpression );

    virtual OdDgVariableValueVariant  getDefaultValue() const;
    virtual void                      setDefaultValue( const OdDgVariableValueVariant& value );

    // if type of value was not recognized.
    virtual OdString getProxyTypeName() const;
    virtual OdString getProxyValue() const;

    virtual OdResult subOpen(OdDg::OpenMode mode);
    virtual OdResult subClose();
    virtual OdResult subErase( bool erasing );
    virtual OdRxObjectPtr clone() const;
};

typedef OdSmartPtr< OdDgVariable > OdDgVariablePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgItemTypeBasedVariable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgItemTypeBasedVariable);
public:
  enum OdDgItemTypeBasedVariableUnits
  {
    kMasterUnits         = 0,
    // Area
    kAcre                = 1,
    kCentimetreSquared   = 2,
    kFootFoot            = 3,
    kFootMile            = 4,
    kFootSquared         = 5,
    kHectare             = 6,
    kInchFoot            = 7,
    kInchMetre           = 8,
    kInchMile            = 9,
    kInchSquared         = 10,
    kKilometreSquared    = 11,
    kMeterKilometre      = 12,
    kMetreMetre          = 13,
    kMetreSquared        = 14,
    kMileSquared         = 15,
    kMillimetreKilometre = 16,
    kMillimetreMetre     = 17,
    kMillimetreMile      = 18,
    kMillimetreSquared   = 19,
    kYardSquared         = 20,
    // Angle
    kDegree              = 21,
    kRadian              = 22,
    kAngleMinute         = 23,
    kAngleQuadrant       = 24,
    kAngleSecond         = 25,
    kRevolution          = 26,
    kGradian             = 27,
    // Distance
    kMetre               = 28,
    kMicroInch           = 29,
    kMicroMetre          = 30,
    kCentimetre          = 31,
    kDecimetre           = 32,
    kFoot                = 33,
    kInch                = 34,
    kKilometre           = 35,
    kMilliFoot           = 36,
    kMile                = 37,
    kMilliInch           = 38,
    kMilliMetre          = 39,
    kYard                = 40
  };
public:

  virtual OdString getName() const;
  virtual OdDgVariable::OdDgVariableType getType() const;
  virtual OdDgItemTypeBasedVariableUnits getUnits() const;

  virtual OdDgVariable::OdDgVariableScope getScope() const;
  virtual void     setScope( const OdDgVariable::OdDgVariableScope& uScope );

  virtual bool     getVisibleFlag() const;
  virtual void     setVisibleFlag( bool bSet );

  virtual bool     getExpressionBasedFlag() const;

  virtual OdDgExpressionVariablePtr getExpression() const;
  virtual bool                      setExpression( OdDgExpressionVariablePtr& pExpression );

  virtual OdDgVariableValueVariant getDefaultValue() const;
  virtual void                     setDefaultValue( const OdDgVariableValueVariant& value );

  static  OdDgVariableValueVariant convertValueToMasterUnits( const OdDgVariableValueVariant& value, const OdDgItemTypeBasedVariableUnits& uUnits, const OdDgModel::UnitDescription& masterUnits );
  static  OdDgVariableValueVariant convertValueToItemBasedUnits( const OdDgVariableValueVariant& value, const OdDgItemTypeBasedVariableUnits& uUnits, const OdDgModel::UnitDescription& masterUnits );

  virtual OdResult subOpen(OdDg::OpenMode mode);
  virtual OdResult subClose();
  virtual OdRxObjectPtr clone() const;
};

typedef OdSmartPtr< OdDgItemTypeBasedVariable > OdDgItemTypeBasedVariablePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgVariableValuesSet : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgVariableValuesSet);
public:

  virtual OdString getName() const;
  virtual void     setName( const OdString& strName );

  virtual OdString getDescription() const;
  virtual void     setDescription( const OdString& strDescription );

  virtual OdDgItemTypeInstance getItemTypeInstance() const;
  virtual bool                 setItemTypeInstance( const OdDgItemTypeInstance& newInstance );

  virtual OdUInt32      getVariablesCount() const;
  virtual OdDgElementId getVariableId( OdUInt32 uIndex ) const;

  virtual OdDgVariableValueVariant getVariableValue( const OdDgElementId& idVar ) const;
  virtual OdDgVariableValueVariant getVariableValue( const OdString& strVarName ) const;
  virtual void setVariableValue( const OdDgElementId& idVar, const OdDgVariableValueVariant& value );
  virtual void setVariableValue( const OdString& strVarName, const OdDgVariableValueVariant& value );

  virtual OdResult subOpen(OdDg::OpenMode mode);
  virtual OdResult subClose();
  virtual OdRxObjectPtr clone() const;
};

typedef OdSmartPtr< OdDgVariableValuesSet > OdDgVariableValuesSetPtr;

//-------------------------------------------------------------------

class TG_EXPORT OdDgVariableValueVariant
{
public:
  OdDgVariableValueVariant();
  OdDgVariableValueVariant( OdDgVariable::OdDgVariableType uType, double dValue );
  OdDgVariableValueVariant( OdInt32 iValue );
  OdDgVariableValueVariant( bool    bValue );
  ~OdDgVariableValueVariant();

  OdDgVariable::OdDgVariableType getType() const;

  double getDoubleValue() const;
  void   setDoubleValue( OdDgVariable::OdDgVariableType uType, double dValue );

  OdInt32 getIntegerValue() const;
  void    setIntegerValue( OdInt32 iValue );

  bool    getBooleanValue() const;
  void    setBooleanValue( bool bValue );

  void    setValueAsProxy();

private:
  OdDgVariable::OdDgVariableType           m_uType;
  double                                   m_dValue;
  OdInt32                                  m_iValue;
  bool                                     m_bValue;
};

//-------------------------------------------------------------------

// type of Extended control element, subtype Local Variable List

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/

//-----------------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgVariablesList : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL( OdDgVariablesList );

public:
  // Local Variables
  virtual OdDgElementId getLocalVariable( const OdString& strVariableName ) const;
  virtual void          addLocalVariable( const OdDgVariablePtr& varData );
  virtual OdDgElementIteratorPtr createLocalVariableIterator(bool atBeginning = true, bool skipDeleted = true) const;

  // Item Type Based Variables
  virtual OdDgElementId getItemTypeLibraryId() const;
  virtual OdString      getItemTypeName() const;
  virtual void          setItemTypeLibrary( const OdDgElementId& idItemType, const OdString& strItemTypeName );

  virtual OdDgItemTypeInstance getItemTypeDefaults() const;
  virtual bool                 setItemTypeDefaults( const OdDgItemTypeInstance& newDefaults );

  virtual OdDgElementId          getItemTypeBasedVariable( const OdString& strVariableName ) const;
  virtual OdDgElementIteratorPtr createItemTypeBasedVariableIterator(bool atBeginning = true, bool skipDeleted = true) const;

  virtual OdDgVariableValueVariant getValueByName( const OdString& strVariableName ) const;
  virtual OdDgElementId            getVariableByName( const OdString& strVariableName ) const;

  // Variation Sets
  virtual OdDgElementId getVariableValuesSet( const OdString& strVariableValuesSetName ) const;
  virtual void          addVariableValuesSet( const OdString& strVariableValuesSetName );
  virtual OdDgElementIteratorPtr createVariableValuesSetIterator(bool atBeginning = true, bool skipDeleted = true) const;

  virtual OdDgModel::UnitDescription getMasterUnits() const;
  virtual OdRxObjectPtr clone() const;
};

typedef OdSmartPtr< OdDgVariablesList > OdDgVariablesListPtr;

#include "TD_PackPop.h"

#endif // __DG_VARIABLES_H__
