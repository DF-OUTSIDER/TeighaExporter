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
#ifndef __DG_DIMOPTION_H__
#define __DG_DIMOPTION_H__

#include "TD_PackPush.h"

#include "DgElement.h"
#include "DgCommonStructs.h"
#include "OdString.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeQuaternion.h"
#include "SharedPtr.h"
#include "DgDimDefs.h"
#include "DgModel.h"

//class OdDgDimOptionImpl;

//-------------------------------------------------------------------
// Dimension Option Blocks
//-------------------------------------------------------------------

//class OdDgDimOptionTolerance; +
//class OdDgDimOptionTerminators; +
//class OdDgDimOptionPrefixSymbol; +
//class OdDgDimOptionSuffixSymbol; +
//class OdDgDimOptionDiameterSymbol; +
//class OdDgDimOptionPrefixSuffix; +
//class OdDgDimOptionPrimaryUnits; +
//class OdDgDimOptionSecondaryUnits; +
//class OdDgDimOptionTerminatorSymbology; +
//class OdDgDimOptionView; +
//class OdDgDimOptionAlternateFormatForPrimaryUnits; +
//class OdDgDimOptionOffset; 
//class OdDgDimOptionAlternateFormatForSecondaryUnits; +
//class OdDgDimOptionAlternatePrefixSeparator; +
//class OdDgDimOptionAlternateSuffixSeparator; +
//class OdDgDimOptionProxyCell; +

//-------------------------------------------------------------------
// OdDgDimOption
//-------------------------------------------------------------------

class OdDgDimOption;
typedef OdSmartPtr<OdDgDimOption> OdDgDimOptionPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOption : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDimOption);

  //
  // Type declarations
  //
  enum Type
  {
      kNone                       = 0x0
    , kTolerance                  = 0x01      // checked OK, implemented
    , kTerminators                = 0x02
    , kPrefixSymbol               = 0x03
    , kSuffixSymbol               = 0x04
  //        DIMOPT_OVERALL_PREFIX_CELL          = 0x05,
  //        DIMOPT_OVERALL_SUFFIX_CELL          = 0x06,
    , kDiameterSymbol             = 0x07
    , kPrefixSuffix               = 0x08
    , kPrimaryUnits               = 0x09      // checked OK, implemented
    , kSecondaryUnits             = 0x0a
    , kTerminatorSymbology        = 0x0b
    , kView                       = 0x0c
    , kAlternatePrimaryUnits      = 0x0d
    , kOffset                     = 0x0e
    , kAlternateSecondaryUnits    = 0x0f
    , kAlternatePrefixSymbol      = 0x10
    , kAlternateSuffixSymbol      = 0x11
    , kProxyCell                  = 0x12
  };

  //
  // Manipulations with sub-types
  //
  virtual Type getType() const = 0;
  static OdDgDimOptionPtr createObject( Type type );

//protected:
//  OdDgDimOptionImpl* m_pImpl;
//  friend class OdDgDimOptionImpl;
};

//-------------------------------------------------------------------
// OdDgDimOptionView
// Type = 12
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionView : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionView );

  virtual OdGeQuaternion  getQuaternion() const = 0;
  virtual void            setQuaternion( const OdGeQuaternion quat) = 0;
  virtual OdGeMatrix2d    getRotationMatrix() const = 0;
  virtual void            setRotationMatrix( const OdGeMatrix2d mat) = 0;
};

typedef OdSmartPtr<OdDgDimOptionView> OdDgDimOptionViewPtr;

//-------------------------------------------------------------------
// OdDgDimOptionTerminatorSymbology
//  Type = 11
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionTerminatorSymbology : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionTerminatorSymbology );

  virtual bool getStyleFlag() const = 0;
  virtual void setStyleFlag( bool bSetFlag ) = 0;

  virtual bool getWeightFlag() const = 0;
  virtual void setWeightFlag( bool bSetFlag ) = 0;

  virtual bool getColorFlag() const = 0;
  virtual void setColorFlag( bool bSetFlag ) = 0;

  virtual OdInt32  getStyle() const = 0;
  virtual void setStyle( OdInt32 iStyle ) = 0;

  virtual OdUInt32 getWeight() const = 0;
  virtual void setWeight( OdUInt32 iWeight ) = 0;

  virtual OdUInt32 getColor() const = 0;
  virtual void setColor( OdUInt32 iColor ) = 0;
};

typedef OdSmartPtr<OdDgDimOptionTerminatorSymbology> OdDgDimOptionTerminatorSymbologyPtr;

//-------------------------------------------------------------------
// OdDgDimOptionTolerance
//  Type = 1
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionTolerance : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionTolerance );

  virtual OdUInt32 getFontEntryId() const = 0;
  virtual void     setFontEntryId( OdUInt32 val ) = 0;

  virtual OdUInt16 getStackAlign() const = 0;
  virtual void     setStackAlign( OdUInt16 val ) = 0;

  virtual bool     getShowSignForZeroFlag() const = 0;
  virtual void     setShowSignForZeroFlag( bool val ) = 0;

  virtual bool     getStackEqualFlag() const = 0;
  virtual void     setStackEqualFlag( bool val ) = 0;

  virtual OdUInt16 getToleranceFlags() const = 0;
  virtual void     setToleranceFlags( OdUInt16 val ) = 0;

// these are set in drawing units when created from a percentage of OdDgDimTextInfo::getHeight()

  virtual double   getToleranceTextWidth() const = 0;
  virtual void     setToleranceTextWidth( double val ) = 0;

  virtual double   getToleranceTextHeight() const = 0;
  virtual void     setToleranceTextHeight( double val ) = 0;

  virtual double   getToleranceLower() const = 0;
  virtual void     setToleranceLower( double val ) = 0;

  virtual double   getToleranceUpper() const = 0;
  virtual void     setToleranceUpper( double val ) = 0;

  virtual double   getToleranceHorizSep() const = 0;
  virtual void     setToleranceHorizSep( double val ) = 0;

  virtual double   getToleranceVertSep() const = 0;
  virtual void     setToleranceVertSep( double val ) = 0;

  virtual OdChar   getTolerancePlusMinusSymbol() const = 0;
  virtual void     setTolerancePlusMinusSymbol( OdChar val ) = 0;

  virtual OdChar   getTolerancePrefixSymbol() const = 0;
  virtual void     setTolerancePrefixSymbol( OdChar val ) = 0;

  virtual OdChar   getToleranceSuffixSymbol() const = 0;
  virtual void     setToleranceSuffixSymbol( OdChar val ) = 0;
};

typedef OdSmartPtr<OdDgDimOptionTolerance> OdDgDimOptionTolerancePtr;

//-------------------------------------------------------------------
// OdDgDimOptionUnits
//  Type = 9,10
//-------------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionUnits : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionUnits );

  virtual bool        getIndependentUnitsFlag() const = 0;
  virtual void        setIndependentUnitsFlag( bool bSet ) = 0;

  virtual void        getMasterUnit( OdDgModel::UnitDescription& unitStruct ) const = 0;
  virtual void        setMasterUnit( OdDgModel::UnitDescription unitStruct ) = 0;

  virtual void        getSubUnit( OdDgModel::UnitDescription&  unitStruct) const = 0;
  virtual void        setSubUnit( OdDgModel::UnitDescription unitStruct ) = 0;

  virtual double      getUORPerMaster() const = 0;
  virtual void        setUORPerMaster( double dVal) = 0;
};
typedef OdSmartPtr<OdDgDimOptionUnits> OdDgDimOptionUnitsPtr;

//-------------------------------------------------------------------
// OdDgDimOptionPrimaryUnits
//  Type = 9
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionPrimaryUnits : public OdDgDimOptionUnits
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionPrimaryUnits );
};

typedef OdSmartPtr<OdDgDimOptionPrimaryUnits> OdDgDimOptionPrimaryUnitsPtr;

//-------------------------------------------------------------------
// OdDgDimOptionSecondaryUnits
// Type = 10
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionSecondaryUnits : public OdDgDimOptionUnits
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionSecondaryUnits );
};

typedef OdSmartPtr<OdDgDimOptionSecondaryUnits> OdDgDimOptionSecondaryUnitsPtr;

//-------------------------------------------------------------------
// OdDgDimOptionPrefixSuffix
// Type = 8
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionPrefixSuffix : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionPrefixSuffix );

  virtual OdChar      getMainPrefix()  const = 0;
  virtual OdChar      getMainSuffix()  const = 0;
  virtual OdChar      getUpperPrefix() const = 0;
  virtual OdChar      getUpperSuffix() const = 0;
  virtual OdChar      getLowerPrefix() const = 0;
  virtual OdChar      getLowerSuffix() const = 0;
  virtual void        setMainPrefix(  OdChar chVal ) = 0;
  virtual void        setMainSuffix(  OdChar chVal ) = 0;
  virtual void        setUpperPrefix( OdChar chVal ) = 0;
  virtual void        setUpperSuffix( OdChar chVal ) = 0;
  virtual void        setLowerPrefix( OdChar chVal ) = 0;
  virtual void        setLowerSuffix( OdChar chVal ) = 0;
};

typedef OdSmartPtr<OdDgDimOptionPrefixSuffix> OdDimOptionPrefixSuffixPtr;

//-------------------------------------------------------------------
// OdDgDimOptionAltFormat
// Type = 13,15
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionAltFormat : public OdDgDimOption
{
public:

  enum ComparisonOperator
  {
    kMore,
    kLess,
    kMoreOrEqual,
    kLessOrEqual
  };

public:

  ODRX_DECLARE_MEMBERS( OdDgDimOptionAltFormat );

  virtual OdUInt16 getAccuracy() const = 0;
  virtual void     setAccuracy( OdUInt16 val ) = 0;

  virtual bool     getSubUnits() const = 0;
  virtual void     setSubUnits( bool val ) = 0;

  virtual bool     getLabel() const = 0;
  virtual void     setLabel( bool val ) = 0;

  virtual bool     getDelimiter() const = 0;
  virtual void     setDelimiter( bool val ) = 0;

  virtual bool     getNoMasterUnits() const = 0;
  virtual void     setNoMasterUnits( bool val ) = 0;

  virtual bool     getEqualToThreshold() const = 0;
  virtual void     setEqualToThreshold( bool val ) = 0;

  virtual bool     getAllowZeroMasterUnits() const = 0;
  virtual void     setAllowZeroMasterUnits( bool val ) = 0;

  virtual bool     getAllowZeroSubUnits() const = 0;
  virtual void     setAllowZeroSubUnits( bool val ) = 0;

  virtual bool     getMoreThanThreshold() const = 0;
  virtual void     setMoreThanThreshold( bool val ) = 0;

  virtual double   getThreshold() const = 0;
  virtual void     setThreshold( double val ) = 0;
};

typedef OdSmartPtr<OdDgDimOptionAltFormat> OdDgDimOptionAltFormatPtr;

//-------------------------------------------------------------------
// OdDgDimOptionPrimaryAltFormat
//  Type = 13
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionPrimaryAltFormat : public OdDgDimOptionAltFormat
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionPrimaryAltFormat );
};

typedef OdSmartPtr<OdDgDimOptionPrimaryAltFormat> OdDgDimOptionPrimaryAltFormatPtr;

//-------------------------------------------------------------------
// OdDgDimOptionSecondaryAltFormat
//  Type = 13
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionSecondaryAltFormat : public OdDgDimOptionAltFormat
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionSecondaryAltFormat );
};

typedef OdSmartPtr<OdDgDimOptionSecondaryAltFormat> OdDgDimOptionSecondaryAltFormatPtr;

//-------------------------------------------------------------------
// OdDgDimOptionTerminators
// Type = 2
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionTerminators : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionTerminators );

  enum TerminatorStyle
  {
    kTermDefault,
    kTermSymbol,
    kTermCell,
    kTermScaledCell
  };

  virtual TerminatorStyle getArrowTermStyle() const = 0;
  virtual TerminatorStyle getStrokeTermStyle() const = 0;
  virtual TerminatorStyle getOriginTermStyle() const = 0;
  virtual TerminatorStyle getDotTermStyle() const = 0;
  virtual void            setArrowTermStyle( TerminatorStyle iStyle ) = 0;
  virtual void            setStrokeTermStyle( TerminatorStyle iStyle ) = 0;
  virtual void            setOriginTermStyle( TerminatorStyle iStyle ) = 0;
  virtual void            setDotTermStyle( TerminatorStyle iStyle ) = 0;
  virtual double          getSharedCellScale() const = 0;
  virtual void            setSharedCellScale( double dScale ) = 0;
  virtual OdUInt32        getArrowFontID() const = 0;
  virtual OdChar          getArrowSymbol() const = 0;
  virtual OdUInt64        getArrowCellID() const = 0;
  virtual void            setArrowFontID( OdUInt32 uiFont ) = 0;
  virtual void            setArrowSymbol( OdChar chSymbol ) = 0;
  virtual void            setArrowCellID( OdUInt64 iId ) = 0;
  virtual OdUInt32        getStrokeFontID() const = 0;
  virtual OdChar          getStrokeSymbol() const = 0;
  virtual OdUInt64        getStrokeCellID() const = 0;
  virtual void            setStrokeFontID( OdUInt32 uiFont ) = 0;
  virtual void            setStrokeSymbol( OdChar chSymbol ) = 0;
  virtual void            setStrokeCellID( OdUInt64 iId ) = 0;
  virtual OdUInt32        getOriginFontID() const = 0;
  virtual OdChar          getOriginSymbol() const = 0;
  virtual OdUInt64        getOriginCellID() const = 0;
  virtual void            setOriginFontID( OdUInt32 uiFont ) = 0;
  virtual void            setOriginSymbol( OdChar chSymbol ) = 0;
  virtual void            setOriginCellID( OdUInt64 iId ) = 0;
  virtual OdUInt32        getDotFontID() const = 0;
  virtual OdChar          getDotSymbol() const = 0;
  virtual OdUInt64        getDotCellID() const = 0;
  virtual void            setDotFontID( OdUInt32 uiFont ) = 0;
  virtual void            setDotSymbol( OdChar chSymbol ) = 0;
  virtual void            setDotCellID( OdUInt64 iId ) = 0;
};

typedef OdSmartPtr<OdDgDimOptionTerminators> OdDgDimOptionTerminatorsPtr;

//-------------------------------------------------------------------
// OdDgDimOptionSymbol
// Type = 3,4,7,16,17
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionSymbol : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionSymbol );

  virtual OdUInt32    getSymbolFont()  const = 0;
  virtual void        setSymbolFont( OdUInt32 iFont ) = 0;
  virtual OdChar      getSymbolChar()  const = 0;
  virtual void        setSymbolChar( OdChar chCode ) = 0;
};

typedef OdSmartPtr<OdDgDimOptionSymbol> OdDgDimOptionSymbolPtr;

//-------------------------------------------------------------------
// OdDgDimOptionPrefixSymbol
// Type = 3
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionPrefixSymbol : public OdDgDimOptionSymbol
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionPrefixSymbol );
};

typedef OdSmartPtr<OdDgDimOptionPrefixSymbol> OdDgDimOptionPrefixSymbolPtr;

//-------------------------------------------------------------------
// OdDgDimOptionSuffixSymbol
// Type = 4
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionSuffixSymbol : public OdDgDimOptionSymbol
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionSuffixSymbol );
};

typedef OdSmartPtr<OdDgDimOptionSuffixSymbol> OdDgDimOptionSuffixSymbolPtr;

//-------------------------------------------------------------------
// OdDgDimOptionDiameterSymbol
// Type = 7
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionDiameterSymbol : public OdDgDimOptionSymbol
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionDiameterSymbol );
};

typedef OdSmartPtr<OdDgDimOptionDiameterSymbol> OdDgDimOptionDiameterSymbolPtr;

//-------------------------------------------------------------------
// OdDgDimOptionAltPrefixSeparator
// Type = 16
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionAltPrefixSeparator : public OdDgDimOptionSymbol
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionAltPrefixSeparator );
};

typedef OdSmartPtr<OdDgDimOptionAltPrefixSeparator> OdDgDimOptionAltPrefixSeparatorPtr;

//-------------------------------------------------------------------
// OdDgDimOptionAltSuffixSeparator
// Type = 17
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionAltSuffixSeparator : public OdDgDimOptionSymbol
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionAltSuffixSeparator );
};

typedef OdSmartPtr<OdDgDimOptionAltSuffixSeparator> OdDgDimOptionAltSuffixSeparatorPtr;

//-------------------------------------------------------------------
// OdDgDimOptionOffset
// Type = 14
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionOffset : public OdDgDimOption
{
public:
  enum ChainType
  {
    kNone = 0
    ,kLine = 1
    ,kArc  = 2
    ,kBSpline = 3
  };

  enum LeaderAlignment
  {
    kAutoAlignment  = 0
    ,kLeftAlignment  = 1
    ,kRightAlignment = 2
  };

public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionOffset );

  virtual OdDgDimTool::TerminatorType getTerminator()  const = 0;
  virtual ChainType   getChainType()   const = 0;
  virtual LeaderAlignment getAlignment()   const = 0;
  virtual bool        getElbowFlag()   const = 0;
  virtual bool        getNoDockOnDimLineFlag() const = 0;
  virtual void        setTerminator( OdDgDimTool::TerminatorType iTerm ) = 0;
  virtual void        setChainType( ChainType iChainType ) = 0;
  virtual void        setAlignment( LeaderAlignment iAlignment ) = 0;
  virtual void        setElbowFlag( bool bSet) = 0;
  virtual void        setNoDockOnDimLineFlag( bool bSet) = 0;
};

typedef OdSmartPtr<OdDgDimOptionOffset> OdDgDimOptionOffsetPtr;

//-------------------------------------------------------------------
// OdDgOptionProxyCell
// Type = 18
//-------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDimOptionProxyCell : public OdDgDimOption
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimOptionProxyCell );

  virtual OdGePoint3d  getOrigin() const = 0;
  virtual OdGeMatrix3d getRotScale() const = 0;
  virtual OdUInt16     getCheckSum() const = 0;
  virtual void         setOrigin( OdGePoint3d ptOrigin ) = 0;
  virtual void         setRotScale(OdGeMatrix3d matRotScale ) = 0;
  virtual void         setCheckSum( OdUInt16 uCheckSum ) = 0;
};
typedef OdSmartPtr<OdDgDimOptionProxyCell> OdDgDimOptionProxyCellPtr;


//class TG_EXPORT OdDgDimOptionProxy : public OdDgDimOption
//{
//public:
//  ODRX_DECLARE_MEMBERS( OdDgDimOptionProxy );
//
//  virtual OdUInt32 getRawSize() const = 0;
//  virtual void getRawData( OdBinaryData& data ) const  = 0;
//};
//typedef OdSmartPtr<OdDgDimOptionProxy> OdDgDimOptionProxyPtr;

#include "TD_PackPop.h"

#endif // __DG_DIMOPTION_H__
