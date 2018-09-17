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
#ifndef __DG_XATTRIBUTE_H__
#define __DG_XATTRIBUTE_H__

#include "TD_PackPush.h"

#include "DgExport.h"
#include "RxObject.h"
#include "OdBinaryData.h"
#include "DgCmColor.h"
#include "DgMaterialTableRecord.h"
#include "DgColorBook.h"
#include "DgLinkSet.h"
#include "DgTextExtendedProperties.h"
#include "DgTerrain.h"
#include "DgTerrainCtrl.h"
#include "DgTextStyleTableRecord.h"
#include "OdTimeStamp.h"
#include "DgTableElement.h"
#include "Ge/GeKnotVector.h"
#include "DgTable.h"
#include "DgMesh.h"
#include "DgAttributeLinkage.h"
#include "DgVariables.h"

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgXAttribute : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgXAttribute);

  virtual OdUInt32 getHandlerId() const = 0;
  virtual OdUInt32 getXAttrId() const = 0;
  virtual void setXAttrId(OdUInt32 xAttrId) = 0;
  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgXAttribute> OdDgXAttributePtr;


/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgProxyXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgProxyXAttribute);

  virtual void setHandlerId(OdUInt32 handlerId) = 0;
  virtual void setData(const OdBinaryData& data) = 0;
  virtual void getData(OdBinaryData& data) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgProxyXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgProxyXAttribute> OdDgProxyXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTextFieldXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x57050000 };

public:
  ODRX_DECLARE_MEMBERS(OdDgTextFieldXAttribute);

  virtual void setData(const OdBinaryData& data) = 0;
  virtual void getData(OdBinaryData& data) const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgTextFieldXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTextFieldXAttribute> OdDgTextFieldXAttributePtr;


/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgXmlXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgXmlXAttribute);

  OdDgXmlXAttribute();

  virtual OdUInt32 getXAttrId() const;
  virtual void setXAttrId(OdUInt32 xAttrId);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0);
  virtual void writeData(OdBinaryData& data) const;

  virtual OdString getXmlString() const;
  virtual bool setXmlString(const OdString& xml);

protected:
  OdString m_xml;
  OdUInt32 m_uXAttrId;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgXmlXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgXmlXAttribute> OdDgXmlXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgZippedXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgZippedXAttribute);

  OdDgZippedXAttribute();

  virtual OdUInt32 getXAttrId() const;
  virtual void setXAttrId(OdUInt32 xAttrId);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0);
  virtual void writeData(OdBinaryData& data) const;

  virtual void getUnzippedData( OdBinaryData& unzippedData ) const;
  virtual void setUnzippedData(const OdBinaryData& unzippedData);

protected:
  OdBinaryData m_unzippedData;
  OdUInt32     m_uXAttrId;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgZipCompressedXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgZippedXAttribute> OdDgZippedXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleTypeFilterXAttribute : public OdDgZippedXAttribute
{
public:
  enum OdDgFilterApplyToType
  {
    kAllElementTypes               = 0,
    kSelectedElementTypes          = 1,
    kAllExceptSelectedElementTypes = 2
  };

  enum OdDgFilterElementType
  {
    kBSplineSurface           = 0,
    kBSplineCurve             = 1,
    kCellHeader               = 2,
    kSpiralCurve              = 3,
    kAssociativeRegion        = 4,
    kDimension                = 5,
    kSmartSolidAndSurface     = 6,
    kSharedCell               = 7,
    kNote                     = 8,
    kMultiline                = 9,
    kGroupedHole              = 10,
    kTag                      = 11,
    kLineString               = 12,
    kRasterImage              = 13,
    kTextNode                 = 14,
    kRasterAttachment         = 15,
    kComplexString            = 16,
    kMesh                     = 17,
    kEllipse                  = 18,
    kExtendedElement          = 19,
    kText                     = 20,
    kSavedView                = 21,
    kSolid                    = 22,
    kSectionClip              = 23,
    kCone                     = 24,
    kSectionCallout           = 25,
    kFeatureSolid             = 26,
    kDetailCallout2dView      = 27,
    kDigitalSignature         = 28,
    kElevationCallout         = 29,
    kLine                     = 30,
    kTitleText                = 31,
    kCurve                    = 32,
    kSurface                  = 33,
    kPointString              = 34,
    kMarkupContentAreaHeader  = 35,
    kOleObject                = 36,
    kAnnotationCell           = 37,
    kShape                    = 38,
    kComplexShape             = 39,
    kDrawingTitle             = 40,
    kDrawingTitleUnderlined   = 41,
    kSheetAnnotationClip      = 42,
    kPlanCallout              = 43,
    kSectionCalloutOrthogonal = 44,
    kInteriorElevationCallout = 45,
    kDetailCallout            = 46,
    kArc                      = 47
  };
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleTypeFilterXAttribute);

  enum { kType = 0x58EA1000 };

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0)  = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt16 getFlags() const = 0;
  virtual void     setFlags( OdUInt16 uFlags ) = 0;

  virtual OdDgFilterApplyToType getApplyToMode() const = 0;
  virtual void     setApplyToMode( const OdDgFilterApplyToType& uMode ) = 0;

  virtual OdUInt32 getSelectedElementTypeCount() const = 0;
  virtual OdDgFilterElementType getSelectedElementType( OdUInt32 uIndex ) const = 0;
  virtual void setSelectedElementType( OdUInt32 uIndex, const OdDgFilterElementType& uType ) = 0;
  virtual void addSelectedElementType( const OdDgFilterElementType& uType ) = 0;
  virtual void deleteSelectedElementType( OdUInt32 uIndex ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDisplayStyleTypeFilterXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgDisplayStyleTypeFilterXAttribute> OdDgDisplayStyleTypeFilterXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleLegendXAttribute : public OdDgZippedXAttribute
{
public:

  struct OdDgLegendItem
  {
    double     m_dItemPosition;
    ODCOLORREF m_uItemColor;
    bool       m_bVisibleItem;

    OdDgLegendItem( double dPos, ODCOLORREF clrColor, bool bVis )
    {
      m_dItemPosition = dPos;
      m_uItemColor    = clrColor;
      m_bVisibleItem  = bVis;
    }

    OdDgLegendItem()
    {
      m_dItemPosition = 0.0;
      m_uItemColor    = ODRGB(0,0,0);
      m_bVisibleItem  = true;
    }
  };

  enum OdDgLegendCoordinateAxis
  {
    kXAxis = 0,
    kYAxis = 1,
    kZAxis = 2
  };

  enum OdDgSlopeDisplayMode
  {
    kAngle   = 0,
    kPercent = 1
  };

  enum OdDgLegendColorScheme
  {
    kBlueRed     = 0,
    kRedBlue     = 1,
    kMonochrome  = 2,
    kTopographic  = 3,
    kSeaMountain = 4,
    kCustom      = -1
  };

  enum OdDgLegendEdgeOverride
  {
    kNone         = 0,
    kAllVisible   = 1,
    kAllInvisible = 2
  };

  enum OdDgLegendValuesCalcMethod
  {
    kCount = 0,
    kStep  = 1
  };

  enum OdDgSteppedDisplayMethod
  {
    kOff          = 0,
    kAccurate     = 1,
    kIsolines     = 2,
    kFast         = 3,
    kFastIsolines = 4
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleLegendXAttribute);

  enum { kType = 0x58EA1001 };

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0)  = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdDgLegendCoordinateAxis getCoordinateAxis() const = 0;
  virtual void setCoordinateAxis( const OdDgLegendCoordinateAxis& uAxis ) = 0;

  virtual OdDgSlopeDisplayMode getSlopeDisplayMode() const = 0;
  virtual void setSlopeDisplayMode( const OdDgSlopeDisplayMode& uMode ) = 0;

  virtual OdDgLegendColorScheme getLegendColorScheme() const = 0;
  virtual void setLegendColorScheme( const OdDgLegendColorScheme& uScheme ) = 0;

  virtual bool getUseFixedDisplayMinFlag() const = 0;
  virtual void setUseFixedDisplayMinFlag( bool bSet ) = 0;

  virtual bool getUseFixedDisplayMaxFlag() const = 0;
  virtual void setUseFixedDisplayMaxFlag( bool bSet ) = 0;

  virtual OdDgLegendEdgeOverride getEdgeOverride() const = 0;
  virtual void setEdgeOverride( const OdDgLegendEdgeOverride& uEdgeOverride ) = 0;

  virtual OdDgLegendValuesCalcMethod getValuesCalcMethod() const = 0;
  virtual void setValuesCalcMethod( const OdDgLegendValuesCalcMethod& uMethod ) = 0;

  virtual bool getDisplayLegendFlag() const = 0;
  virtual void setDisplayLegendFlag( bool bSet ) = 0;
  
  virtual OdDgSteppedDisplayMethod getSteppedDisplayMethod() const = 0;
  virtual void setSteppedDisplayMethod( const OdDgSteppedDisplayMethod& uMethod ) = 0;

  virtual bool getTransparentMarginsFlag() const = 0;
  virtual void setTransparentMarginsFlag( bool bSet ) = 0;

  virtual bool getDescendingLegendOrderFlag() const = 0;
  virtual void setDescendingLegendOrderFlag( bool bSet ) = 0;

  virtual double getFixedDisplayMinValue() const = 0;
  virtual void   setFixedDisplayMinValue( double dValue ) = 0;

  virtual double getFixedDisplayMaxValue() const = 0;
  virtual void   setFixedDisplayMaxValue( double dValue ) = 0;

  virtual double getLegendStepValue() const = 0;
  virtual void   setLegendStepValue( double dValue ) = 0;

  virtual ODCOLORREF getMarginColor() const = 0;
  virtual void   setMarginColor( const ODCOLORREF& uColor ) = 0;

  virtual OdUInt16 getLegendTransparency() const = 0;
  virtual void     setLegendTransparency( OdUInt16 uValue ) = 0;

  virtual OdUInt32 getColorSchemeItemCount() const = 0;
  virtual OdDgLegendItem getColorSchemeItem( OdUInt32 uIndex ) const = 0;
  virtual void     setColorSchemeItem( OdUInt32 uIndex, const OdDgLegendItem& schemeItem) = 0;
  virtual void     addColorSchemeItem( const OdDgLegendItem& schemeItem) = 0;
  virtual void     deleteColorSchemeItem(OdUInt32 uIndex) = 0;

  virtual OdUInt32 getLegendItemCount() const = 0;
  virtual OdDgLegendItem getLegendItem( OdUInt32 uIndex ) const = 0;
  virtual void     setLegendItem( OdUInt32 uIndex, const OdDgLegendItem& schemeItem) = 0;
  virtual void     addLegendItem( const OdDgLegendItem& schemeItem) = 0;
  virtual void     deleteLegendItem(OdUInt32 uIndex) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDisplayStyleLegendXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgDisplayStyleLegendXAttribute> OdDgDisplayStyleLegendXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleAzimuthAltitudeXAttribute : public OdDgZippedXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleAzimuthAltitudeXAttribute);

  enum { kType = 0x58EA1003 };

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0)  = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual double getAzimuth() const = 0;
  virtual void   setAzimuth( double dAzimuth ) = 0;

  virtual double getAltitude() const = 0;
  virtual void   setAltitude( double dAltitude ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDisplayStyleExt3XAttribute object pointers.
*/
typedef OdSmartPtr<OdDgDisplayStyleAzimuthAltitudeXAttribute> OdDgDisplayStyleAzimuthAltitudeXAttributePtr;


/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/

class OdDgXMLProperty;

//-------------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgXMLNode
{
public:
  OdDgXMLNode();
  OdDgXMLNode( const OdString& strNodeName );

  OdString getName() const;
  void     setName( const OdString& strName );

  OdUInt32               getPropertiesCount() const;
  const OdDgXMLProperty& getProperty( OdUInt32 uPropIndex ) const;
  bool                   getProperty( const OdString& strPropName, OdDgXMLProperty& propValue ) const;
  bool                   getProperty( const OdString& strPropName, OdDgXMLProperty& propValue, bool bRemoveIt = false );
  bool                   setProperty( OdUInt32 uPropIndex, const OdDgXMLProperty& propValue );
  bool                   setProperty( const OdDgXMLProperty& propValue );
  void                   addProperty( const OdDgXMLProperty& propValue, OdInt32 insertBefore = -1 );
  bool                   deleteProperty( OdUInt32 uPropIndex );
  bool                   deleteProperty( const OdString& strPropName );
  bool                   hasProperty( const OdString& strPropName ) const;
  void                   clear();

  OdDgXMLProperty        getProperty( const OdString& strPropName, const OdString& strDefaultValue ) const;
  OdDgXMLProperty        getProperty( const OdString& strPropName, const OdString& strDefaultValue, bool bRemoveIt = false );
  void                   setProperty( const OdDgXMLProperty& propValue, const OdString& strDefaultValue );
  void                   mergeXMLNode( const OdDgXMLProperty& propValue );

private:

  static void mergeNodes( const OdDgXMLNode& sourceNode, OdDgXMLNode& destinationNode );

private:
  OdArray<OdDgXMLProperty> m_arrProperties;
  OdString                 m_strName;
};

//-------------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgXMLProperty
{
public:
  OdDgXMLProperty();
  OdDgXMLProperty( const OdString& strName );
  OdDgXMLProperty( const OdString& strName, const OdString& strValue, bool bXmlText = false );
  OdDgXMLProperty( const OdString& strName, double dValue, OdUInt32 uPrecision = 6, bool bXmlText = false ); // 1e-6
  OdDgXMLProperty( const OdString& strName, OdInt32 iValue, bool bXmlText = false );
  OdDgXMLProperty( const OdString& strName, OdUInt32 uValue, bool bXmlText = false );
  OdDgXMLProperty( const OdString& strName, bool bValue, const OdString& strTrue = L"true", const OdString& strFalse = L"false", bool bXmlText = false );
  OdDgXMLProperty( const OdString& strName, const OdGePoint2d& pt2dValue, bool bXmlText = false );
  OdDgXMLProperty( const OdString& strName, const OdGePoint3d& pt3dValue, bool bXmlText = false );
  OdDgXMLProperty( const OdDgXMLNode& xmlNode );

  OdString     getName()   const;
  OdString     asString()  const;
  double       asDouble()  const;
  OdInt32      asInt32()   const;
  OdUInt32     asUInt32()  const;
  bool         asBool( const OdString& strTrue = L"true", const OdString& strFalse = L"false", bool bCaseDependent = false ) const;
  OdGePoint2d  asPoint2d() const;
  OdGePoint3d  asPoint3d() const;
  bool         isEmpty()   const;
  void         clear();

  bool         isXMLNode() const;
  bool         isXMLText() const;
  const        OdDgXMLNode& asXMLNode() const;
  OdDgXMLNode  asXMLNode();

  void         setName( const OdString& strName );
  void         setString( const OdString& strValue);
  void         setXMLText( const OdString& strXMLText );
  void         setDouble( double dValue, OdUInt32 uPrecision = 6); // 1e-6
  void         setInt32( OdInt32 iValue );
  void         setUInt32( OdUInt32 uValue );
  void         setBool( bool bValue, const OdString& strTrue = L"true", const OdString& strFalse = L"false" );
  void         setPoint2d( const OdGePoint2d& pt2dValue, OdUInt32 uPrecision = 6 ); // 1e-6
  void         setPoint3d( const OdGePoint3d& pt3dValue, OdUInt32 uPrecision = 6 ); // 1e-6
  void         setXMLNode( const OdDgXMLNode& xmlNode );

private:
  OdString      m_strName;
  OdString      m_strValue;
  OdDgXMLNode   m_xmlNodeValue;
  bool          m_bXMLNode;
  bool          m_bXMLText;
};

//-------------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgProcessedXmlXAttribute : public OdDgXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgProcessedXmlXAttribute);

  OdDgProcessedXmlXAttribute();

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0);
  virtual void writeData(OdBinaryData& data) const;

  const OdDgXMLNode& getRootNode() const;
  void               setRootNode( const OdDgXMLNode& xmlRootNode );
protected:
  OdDgXMLNode m_xmlNode;
  bool        m_bModified;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgProcessedXmlXAttribute object pointers.
*/

typedef OdSmartPtr<OdDgProcessedXmlXAttribute> OdDgProcessedXmlXAttributePtr;

//-------------------------------------------------------------------------------------------------------

class TG_EXPORT OdDgStringXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgStringXAttribute);

  OdDgStringXAttribute();

  virtual OdUInt32 getXAttrId() const;
  virtual void setXAttrId(OdUInt32 uAttrId);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0);
  virtual void writeData(OdBinaryData& data) const;

  virtual const OdString& getString() const;
  virtual void  setString( const OdString& strData );

protected:
  OdUInt32 m_uXAttrId;
  OdString m_strData;
  OdUInt32 m_uCompressionMethod;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgStringXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgStringXAttribute> OdDgStringXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemLibNameXAttribute : public OdDgStringXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemLibNameXAttribute);

  enum { kType = 0x56D2003D };
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgItemLibNameXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgItemLibNameXAttribute> OdDgItemLibNameXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemLibVersionXAttribute : public OdDgStringXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemLibVersionXAttribute);

  enum { kType = 0x56D20050 };
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgItemLibVersionXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgItemLibVersionXAttribute> OdDgItemLibVersionXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgItemLibProviderXAttribute : public OdDgStringXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgItemLibProviderXAttribute);

  enum { kType = 0x56D20054 };
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgItemLibProviderXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgItemLibProviderXAttribute> OdDgItemLibProviderXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgMatProjectionXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x56DF0002 };

public:

  ODRX_DECLARE_MEMBERS(OdDgMatProjectionXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdDgMaterialMap::MappingType getMappingType() const = 0;
  virtual void setMappingType( OdDgMaterialMap::MappingType uType ) = 0;

  virtual bool getCappedFlag() const = 0;
  virtual void setCappedFlag( bool bSet ) = 0;

  virtual OdGePoint3d getProjectionOffset() const = 0;
  virtual void setProjectionOffset( const OdGePoint3d& ptOffset ) = 0;

  virtual OdGeScale3d getProjectionScale() const = 0;
  virtual void setProjectionScale( const OdGeScale3d& sclScale ) = 0;

  virtual OdGeVector3d getProjectionAngles() const = 0;
  virtual void setProjectionAngles( const OdGeVector3d& vrAngles ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgMatProjectionXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgMatProjectionXAttribute> OdDgMatProjectionXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgEnvironmentMapXAttribute : public OdDgXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgEnvironmentMapXAttribute);

  enum { kType = 0x58750001 };
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgEnvironmentMapXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgEnvironmentMapXAttribute> OdDgEnvironmentMapXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgLuxologySetupXAttribute : public OdDgXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgLuxologySetupXAttribute);

  enum { kType = 0x58750000 };
};

// Use OdDgDisplayStyleExtXAttribute instead of OdDgDisplayStyleXAttribute
// Saved to keep binary compatibility with V. 4.2.0. 

class TG_EXPORT OdDgDisplayStyleXAttribute : public OdDgXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleXAttribute);

  enum { kType = 0x58660000 };
};

typedef OdSmartPtr<OdDgDisplayStyleXAttribute> OdDgDisplayStyleXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleExtXAttribute : public OdDgProcessedXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleExtXAttribute);

  enum { kType = 0x58660000 };
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDisplayStyleExtXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgDisplayStyleExtXAttribute> OdDgDisplayStyleExtXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgPrintStyleXAttribute : public OdDgProcessedXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgPrintStyleXAttribute);

  enum { kType = 0x571A0001 };
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgPrintStyleXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgPrintStyleXAttribute> OdDgPrintStyleXAttributePtr;


/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgBECXMLVariant
{
public:

  struct OdDgBECXMLVariantColor
  {
    OdUInt16     m_uDataFlags;
    ODCOLORREF   m_varRGB;
    OdUInt32     m_varColorIndex;
    OdUInt8      m_varColorFlags;     // 1 - colorByLevel, 2 - colorByCell
    OdString     m_varColorBookName;
    OdUInt32     m_varColorBookIndex;
    OdUInt32     m_varColorIndexInBook;
    OdBinaryData m_varExtColorData;
    OdDgBECXMLVariantColor()
    {
      m_varRGB              = 0;
      m_uDataFlags          = 0;
      m_varColorIndex       = 0;
      m_varColorFlags       = 0;
      m_varColorBookName    = OdString::kEmpty;
      m_varColorBookIndex   = 0;
      m_varColorIndexInBook = 0;
      m_varExtColorData.empty();
    }
  };

public:

  enum OdDgBECXMLVariantColorDataFlags
  {
    kColorRGB         = 0x0001,
    kColorIndex       = 0x0002,
    kColorFlags       = 0x0004,
    kColorBookName    = 0x0008,
    kColorBookIndex   = 0x0010,
    kColorIndexInBook = 0x0020,
    kColorExtData     = 0x0040
  };

  enum OdDgBECXMLVariantFlags
  {
    kBool             = 0x00000001,
    kByte             = 0x00000002,
    kByteOrBool       = 0x00000004,
    kShort            = 0x00000008,
    kInt32            = 0x00000010,
    kInt64            = 0x00000020,
    kDouble           = 0x00000040,
    kString           = 0x00000080,
    kColorData        = 0x00000100,
    kPoint3d          = 0x00000200,
    kDateAndTime      = 0x00000400,
    kStringArray      = 0x00000800,
    kBoolArray        = 0x00001000,
    kByteArray        = 0x00002000,
    kShortArray       = 0x00004000,
    kInt32Array       = 0x00008000,
    kInt64Array       = 0x00010000,
    kDoubleArray      = 0x00020000,
    kDateTimeArray    = 0x00040000,
    kPoint3dArray     = 0x00080000,
    kColorDataArray   = 0x00100000
  };

public:

  OdDgBECXMLVariant()
  {
    m_uFlags              = 0;
    m_varBool             = false;
    m_varByte             = 0;
    m_varShort            = 0;
    m_varInt32            = 0;
    m_varInt64            = 0;
    m_varDouble           = 0.0;
    m_varString           = OdString::kEmpty;
  }

  virtual ~OdDgBECXMLVariant(){};

public:

  OdUInt32                        m_uFlags;

  bool                            m_varBool;
  OdInt8                          m_varByte;
  OdInt16                         m_varShort;
  OdInt32                         m_varInt32;
  OdInt64                         m_varInt64;
  double                          m_varDouble;
  OdString                        m_varString;
  OdUInt32                        m_varStringIndex;
  OdDgBECXMLVariantColor          m_varColorData;
  OdGePoint3d                     m_varPoint3d;
  OdTimeStamp                     m_varDateTime;
  OdArray<OdString>               m_varStringArray;
  OdArray<bool>                   m_varBoolArray;
  OdArray<OdInt8>                 m_varByteArray;
  OdArray<OdInt16>                m_varShortArray;
  OdArray<OdInt32>                m_varInt32Array;
  OdArray<OdInt64>                m_varInt64Array;
  OdArray<double>                 m_varDoubleArray;
  OdArray<OdTimeStamp>            m_varDateTimeArray;
  OdArray<OdGePoint3d>            m_varPoint3dArray;
  OdArray<OdDgBECXMLVariantColor> m_varColorDataArray;
};

//----------------------------------------------------------

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgBinXMLItem;
typedef OdSharedPtr<OdDgBinXMLItem> OdDgBinXMLItemPtr;

//----------------------------------------------------------

class TG_EXPORT OdDgBinXMLItem
{
  public:
    virtual ~OdDgBinXMLItem(){};
    virtual OdString          getName() const = 0;
    virtual void              setName( const OdString& strName ) = 0;
    virtual bool              isXmlNode() const = 0;
    virtual OdDgBinXMLItemPtr copy() const = 0;
};

//----------------------------------------------------------

class TG_EXPORT OdDgBinXMLValue : public OdDgBinXMLItem
{
public:
  OdDgBinXMLValue();
  OdDgBinXMLValue( const OdString& strName, const OdDgBECXMLVariant& value );
  virtual ~OdDgBinXMLValue();

  virtual OdString getName() const;
  virtual void     setName( const OdString& strName );

  virtual OdDgBECXMLVariant getValue() const;
  virtual void     setValue( const OdDgBECXMLVariant& value );

  virtual bool     isXmlNode() const;
  virtual OdDgBinXMLItemPtr copy() const;
private:

  OdString          m_strName;
  OdDgBECXMLVariant m_value;
};

//----------------------------------------------------------

typedef OdSharedPtr<OdDgBinXMLValue> OdDgBinXMLValuePtr;

//----------------------------------------------------------

class TG_EXPORT OdDgBinXMLNode : public OdDgBinXMLItem
{
public:
  OdDgBinXMLNode();
  OdDgBinXMLNode( const OdString& strName );
  virtual ~OdDgBinXMLNode();

  virtual OdString getName() const;
  virtual void     setName( const OdString& strName );

  virtual OdUInt32 getChildCount() const;
  virtual OdDgBinXMLItemPtr getChildItem( OdUInt32 uIndex ) const;
  virtual void setChildItem( OdUInt32 uIndex, const OdDgBinXMLItemPtr& pItem );
  virtual void setChildItem( OdUInt32 uIndex, const OdDgBinXMLValue& xmlValue );
  virtual void setChildItem( OdUInt32 uIndex, const OdDgBinXMLNode& xmlNode );
  virtual void addChildItem( const OdDgBinXMLItemPtr& pItem );
  virtual void addChildItem( const OdDgBinXMLValue& xmlValue );
  virtual void addChildItem( const OdDgBinXMLNode& xmlNode );
  virtual void deleteChildItem( OdUInt32 uIndex );

  virtual bool     isXmlNode() const;
  virtual OdDgBinXMLItemPtr copy() const;
private:

  OdString                   m_strName;
  OdArray<OdDgBinXMLItemPtr> m_arrChilds;
};

//----------------------------------------------------------

typedef OdSharedPtr<OdDgBinXMLNode> OdDgBinXMLNodePtr;

//----------------------------------------------------------

class TG_EXPORT OdDgBinXMLXAttribute : public OdDgXAttribute
{
public:

	enum { kType = 0x56FF0001 };

public:

	ODRX_DECLARE_MEMBERS(OdDgBinXMLXAttribute);

	virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
	virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdString getTypeString() const = 0;
  virtual void setTypeString( const OdString& strType ) = 0;

  virtual const OdDgBinXMLNode* getRootNode() const = 0;
  virtual void  setRootNode( const OdDgBinXMLNodePtr& pRootNode ) = 0;
  virtual void  setRootNode( const OdDgBinXMLNode& rootNode ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgBinXMLXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgBinXMLXAttribute> OdDgBinXMLXAttributePtr;

/** \details

Corresponding C++ library: DG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgTemplateHeaderXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x583B0001 };

  enum OdDgTemplateHeaderType
  {
    kTemplate      = 0,
    kTemplateGroup = 1
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgTemplateHeaderXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32 getTemplateOrder() const = 0;
  virtual void     setTemplateOrder( OdUInt32 uOrder ) = 0;

  virtual OdDgTemplateHeaderType getTemplateType() const = 0;
  virtual void setTemplateType( OdDgTemplateHeaderType uType ) = 0;

  virtual OdString getTemplateName() const = 0;
  virtual void     setTemplateName( const OdString& strName ) = 0;

  virtual OdUInt64 getParentId() const = 0;
  virtual void     setParentId( OdUInt64 uId ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTemplateHeaderXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTemplateHeaderXAttribute> OdDgTemplateHeaderXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTemplateIdXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x583B0004 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTemplateIdXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt64 getTemplateId() const = 0;
  virtual void     setTemplateId( OdUInt64 uId ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTemplateIdXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTemplateIdXAttribute> OdDgTemplateIdXAttributePtr;

//----------------------------------------------------------

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgItemTypeXAttribute : public OdDgXAttribute
{
public:

	enum { kType = 0x56FF0000 };

public:

	ODRX_DECLARE_MEMBERS(OdDgItemTypeXAttribute);

	virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
	virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt64 getItemLibraryId() const = 0;
  virtual void     setItemLibraryId( OdUInt64 uId ) = 0;

  virtual OdString getItemLibraryName() const = 0;
  virtual void     setItemLibraryName( const OdString& strName ) = 0;

  virtual OdString getItemName() const = 0;
  virtual void setItemName( const OdString& strName ) = 0;

  virtual const OdDgBinXMLNode* getRootNode() const = 0;
  virtual void  setRootNode( const OdDgBinXMLNodePtr& pRootNode ) = 0;
  virtual void  setRootNode( const OdDgBinXMLNode& rootNode ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgItemTypeXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgItemTypeXAttribute> OdDgItemTypeXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgMaterialXAttribute : public OdDgXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMaterialXAttribute);

  enum { kType = 0x57080000 };
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgMaterialXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgMaterialXAttribute> OdDgMaterialXAttributePtr;


/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTrueColorXAttribute : public OdDgXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTrueColorXAttribute);

  enum { kType = 0x57090000 };

  virtual void getTrueColors(OdArray<OdDgCmColor>& trueColors) const = 0;
  virtual void setTrueColors(const OdArray<OdDgCmColor>& trueColors) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgTrueColorXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTrueColorXAttribute> OdDgTrueColorXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgMaterialTableExtXAttribute : public OdDgProcessedXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMaterialTableExtXAttribute);

  enum { kType = 0x570A0000 };

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdString getMaterialTableName() const = 0;
  virtual void setMaterialTableName(const OdString& strName ) = 0;

  virtual OdString getMaterialTableDescription() const = 0;
  virtual void setMaterialTableDescription(const OdString& strDescription ) = 0;

  virtual OdUInt32 getPaletteCount() const = 0;
  virtual OdString getPaletteName( OdUInt32 uIndex ) const = 0;
  virtual void     setPaletteName( OdUInt32 uIndex, const OdString& strName ) = 0;
  virtual void     addPaletteName( const OdString& strName ) = 0;
  virtual void     deletePaletteName( OdUInt32 uIndex ) = 0;

  virtual OdUInt32 getRenderingParamCount() const = 0;
  virtual OdString getRenderingParam( OdUInt32 uIndex ) const = 0;
  virtual void     setRenderingParam( OdUInt32 uIndex, const OdString& strParam ) = 0;
  virtual void     addRenderingParam( const OdString& strParam ) = 0;
  virtual void     deleteRenderingParam( OdUInt32 uIndex ) = 0;

  virtual OdUInt32 getAssignmentCount() const = 0;
  virtual OdDgMaterialAssignment getAssignment( OdUInt32 uIndex ) const = 0;
  virtual void     setAssignment( OdUInt32 uIndex, const OdDgMaterialAssignment& assignment ) = 0;
  virtual void     addAssignment( const OdDgMaterialAssignment& assignment ) = 0;
  virtual void     deleteAssignment( OdUInt32 uIndex ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgTrueColorXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgMaterialTableExtXAttribute> OdDgMaterialTableExtXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgAnnotationCellXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAnnotationCellXAttribute);

  enum { kType = 0x57020000 };

  virtual double getAnnotationScale() const = 0;
  virtual void setAnnotationScale( double dNewScale ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgAnnotationCellXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgAnnotationCellXAttribute> OdDgAnnotationCellXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgClipVolumeSettingsXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgClipVolumeSettingsXAttribute);

  enum { kType = 0x58620000 };

  virtual bool     getForwardDisplayFlag() const = 0;
  virtual void     setForwardDisplayFlag( bool bSet ) = 0;
  virtual bool     getForwardSnapFlag() const = 0;
  virtual void     setForwardSnapFlag( bool bSet ) = 0;
  virtual bool     getForwardLocateFlag() const = 0;
  virtual void     setForwardLocateFlag( bool bSet ) = 0;
  virtual OdUInt32 getForwardDisplayStyleEntryId() const = 0;
  virtual void     setForwardDisplayStyleEntryId( OdUInt32 uEntryId ) = 0;

  virtual bool     getBackDisplayFlag() const = 0;
  virtual void     setBackDisplayFlag( bool bSet ) = 0;
  virtual bool     getBackSnapFlag() const = 0;
  virtual void     setBackSnapFlag( bool bSet ) = 0;
  virtual bool     getBackLocateFlag() const = 0;
  virtual void     setBackLocateFlag( bool bSet ) = 0;
  virtual OdUInt32 getBackDisplayStyleEntryId() const = 0;
  virtual void     setBackDisplayStyleEntryId( OdUInt32 uEntryId ) = 0;

  virtual bool     getCutDisplayFlag() const = 0;
  virtual void     setCutDisplayFlag( bool bSet ) = 0;
  virtual bool     getCutSnapFlag() const = 0;
  virtual void     setCutSnapFlag( bool bSet ) = 0;
  virtual bool     getCutLocateFlag() const = 0;
  virtual void     setCutLocateFlag( bool bSet ) = 0;
  virtual OdUInt32 getCutDisplayStyleEntryId() const = 0;
  virtual void     setCutDisplayStyleEntryId( OdUInt32 uEntryId ) = 0;

  virtual bool     getOutsideDisplayFlag() const = 0;
  virtual void     setOutsideDisplayFlag( bool bSet ) = 0;
  virtual bool     getOutsideSnapFlag() const = 0;
  virtual void     setOutsideSnapFlag( bool bSet ) = 0;
  virtual bool     getOutsideLocateFlag() const = 0;
  virtual void     setOutsideLocateFlag( bool bSet ) = 0;
  virtual OdUInt32 getOutsideDisplayStyleEntryId() const = 0;
  virtual void     setOutsideDisplayStyleEntryId( OdUInt32 uEntryId ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgClipVolumeSettingsXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgClipVolumeSettingsXAttribute> OdDgClipVolumeSettingsXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleNumberXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDisplayStyleNumberXAttribute);

  enum { kType = 0x586A0000 };

  virtual OdUInt32 getDisplayStyleEntryId() const = 0;
  virtual void setDisplayStyleEntryId( OdUInt32 uEntryId ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDisplayStyleNumberXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgDisplayStyleNumberXAttribute> OdDgDisplayStyleNumberXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgRasterClipXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x570B0000 };

public:

  struct OdDgRasterClipElementData
  {
    OdUInt32        uContourNum;
    OdDgElementPtr  pContourElement;
    bool            bContourIsMask;
    OdUInt32        uContourFlags;
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgRasterClipXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32 getClipElementCount() const = 0;
  virtual OdDgRasterClipElementData getClipElementData( OdUInt32 uIndex ) = 0;
  virtual void setClipElementData( OdUInt32 uIndex, const OdDgRasterClipElementData& clipData ) = 0;
  virtual void addClipElementData( const OdDgRasterClipElementData& clipData ) = 0;
  virtual void addClipElementData( const OdGePoint3dArray& arrPoints, bool bMaskElement ) = 0;
  virtual void addClipElementData( const OdGePoint2dArray& arrPoints, bool bMaskElement ) = 0;
  virtual void deleteClipElementData( OdUInt32 uCountourNum ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgMatProjectionXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgRasterClipXAttribute> OdDgRasterClipXAttributePtr;



/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDgnLinkNodeXAttribute : public OdDgXmlXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDgnLinkNodeXAttribute);

  enum { kType = 0x56F50000 };

  virtual OdDgLinkNodePtr getLinkNode() = 0;
  virtual void setLinkNode( const OdDgLinkNodePtr& pLinkNode ) = 0;
  virtual OdUInt32 getParentAttrId() const = 0;
  virtual void setParentAttrId( OdUInt32 uId ) = 0;
  virtual bool isFolderLink() const = 0;
  virtual OdUInt32 getChildCount() const = 0;
  virtual OdUInt32 getChildAttrId( OdUInt32 uIndex ) const = 0;
  virtual void     setChildAttrId( OdUInt32 uIndex, OdUInt32 uAttrId ) = 0;
  virtual void     addChildAttrId( OdUInt32 uAttrId ) = 0;
  virtual void     deleteChildAttrId( OdUInt32 uIndex ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDgnLinkNodeXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgDgnLinkNodeXAttribute> OdDgDgnLinkNodeXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgColorBookXAttribute : public OdDgXAttribute
{
public:
  ODRX_DECLARE_MEMBERS(OdDgColorBookXAttribute);

  virtual OdUInt32          getItemCount() const = 0;
  virtual OdDgColorBookItem getItem( OdUInt32 uIndex ) const = 0;
  virtual void setItem( OdUInt32 uIndex, const OdDgColorBookItem& item ) = 0;
  virtual void addItem( const OdDgColorBookItem& item ) = 0;
  virtual void deleteItem( OdUInt32 uIndex ) = 0;

  enum { kType = 0x57040000 };
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgColorBookXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgColorBookXAttribute> OdDgColorBookXAttributePtr;

/** \details

    Corresponding C++ library: DG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgDetailingSymbolXAttribute : public OdDgXAttribute
{
public:
  enum OdDgDetailingXAttributeType
  {
    kSymbolSymbologyFlags  = 1, // X Attribute Id = 1
    kSymbolProperties = 2  // X Attribute Id = 4
  };

  enum OdDgDetailingSymbolType
  {
    kUndefined       = 0,
    kSelectionMarker = 1,
    kTitleText       = 2,
    kArrowMarker     = 3,
    kDetailMarker    = 4
  };

  enum { kType = 0x57000000 };

public:
  ODRX_DECLARE_MEMBERS(OdDgDetailingSymbolXAttribute);

  virtual OdDgDetailingXAttributeType getXAttributeType() const = 0;
  virtual void setXAttributeType( OdDgDetailingXAttributeType uType ) = 0;

  // For symbol overrides x attribute

  virtual bool getColorWasDefinedFlag()      const = 0;
  virtual bool getLineTypeWasDefinedFlag()   const = 0;
  virtual bool getLineWeightWasDefinedFlag() const = 0;
  virtual bool getLevelWasDefinedFlag()      const = 0;
  virtual bool getBubbleSizeFlag()           const = 0;

  virtual void setColorWasDefinedFlag( bool bSet )      = 0;
  virtual void setLineTypeWasDefinedFlag( bool bSet )   = 0;
  virtual void setLineWeightWasDefinedFlag( bool bSet ) = 0;
  virtual void setLevelWasDefinedFlag( bool bSet )      = 0;
  virtual void setBubbleSizeFlag( bool bSet )           = 0;

  virtual double getBubbleSize() const = 0;
  virtual void setBubbleSize( double dSize ) = 0;

  // For symbol properties x attribute

  virtual OdDgDetailingSymbolType getSymbolType() const = 0;
  virtual void setSymbolType( OdDgDetailingSymbolType uType ) = 0;

  virtual bool getAnnotationScaleFlag() const = 0;
  virtual void setAnnotationScaleFlag( bool bSet ) = 0;

  virtual bool getFlipArrowMarkerFlag() const = 0;
  virtual void setFlipArrowMarkerFlag( bool bSet ) = 0;

  virtual OdUInt64 getTableRecordId() const = 0;
  virtual void setTableRecordId( OdUInt64 uId ) = 0;

  virtual double getAnnotationScale() const = 0;
  virtual void   setAnnotationScale( double dScale ) = 0;

  virtual double getDetailingSymbolSize() const = 0;
  virtual void   setDetailingSymbolSize( double dSize ) = 0;

  virtual double getDetailSymbolArea() const = 0;
  virtual void   setDetailSymbolArea( double dSize ) = 0;

  virtual OdUInt16 getTextPositionFlags() const = 0;
  virtual void     setTextPositionFlags(  OdUInt16 uFlags ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgDetailingSymbolXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgDetailingSymbolXAttribute> OdDgDetailingSymbolXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgSectionClipDataXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x006D0000 };

public:

  ODRX_DECLARE_MEMBERS(OdDgSectionClipDataXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual bool getPerspectiveUpFlag() const = 0;
  virtual void setPerspectiveUpFlag( bool bSet ) = 0;

  virtual bool getLeftCropFlag() const = 0;
  virtual void setLeftCropFlag( bool bSet ) = 0;

  virtual bool getRightCropFlag() const = 0;
  virtual void setRightCropFlag( bool bSet ) = 0;

  virtual bool getTopCropFlag() const = 0;
  virtual void setTopCropFlag( bool bSet ) = 0;

  virtual bool getBottomCropFlag() const = 0;
  virtual void setBottomCropFlag( bool bSet ) = 0;

  virtual bool getFrontCropFlag() const = 0;
  virtual void setFrontCropFlag( bool bSet ) = 0;

  virtual bool getBackCropFlag() const = 0;
  virtual void setBackCropFlag( bool bSet ) = 0;

  virtual double getFrontDepth() const = 0;
  virtual void setFrontDepth( double dDepth ) = 0;

  virtual double getBackDepth() const = 0;
  virtual void setBackDepth( double dDepth ) = 0;

  virtual double getTopHeight() const = 0;
  virtual void setTopHeight( double dHeight ) = 0;

  virtual double getBottomHeight() const = 0;
  virtual void setBottomHeight( double dHeight ) = 0;

  virtual OdGeMatrix3d getRotation() const = 0;
  virtual void setRotation( const OdGeMatrix3d& matRotation ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgSectionClipDataXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgSectionClipDataXAttribute> OdDgSectionClipDataXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgSectionClipDataExtXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x006D0001 };

public:

  ODRX_DECLARE_MEMBERS(OdDgSectionClipDataExtXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdGePoint3d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint3d& ptOrigin ) = 0;

  virtual OdGePoint3d getDirection() const = 0;
  virtual void setDirection( const OdGePoint3d& ptDirection ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgSectionClipDataExtXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgSectionClipDataExtXAttribute> OdDgSectionClipDataExtXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainVertexCoordinatesXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC012E };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainVertexCoordinatesXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32    getVerticesCount() const = 0;
  virtual OdGePoint3d getVertex( OdUInt32 uVertexNumber ) = 0;
  virtual void        setVertex( OdUInt32 uVertexNumber, const OdGePoint3d& ptVertex ) = 0;
  virtual void        addVertex( const OdGePoint3d& ptVertex ) = 0;
  virtual void        deleteVertex( OdUInt32 uVertexNumber ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainVertexCoordinatesXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainVertexCoordinatesXAttribute> OdDgTerrainVertexCoordinatesXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainTransformationXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC014E };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainTransformationXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdGeMatrix3d getTransformation() = 0;
  virtual void         setTransformation( const OdGeMatrix3d& matTransform ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainTransformationXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainTransformationXAttribute> OdDgTerrainTransformationXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/

class TG_EXPORT OdDgTerrainGraphicsSettings
{

public:
  OdDgTerrainGraphicsSettings();
  virtual ~OdDgTerrainGraphicsSettings();

  OdUInt32  getLevelEntryId() const;
  void      setLevelEntryId( OdUInt32 uLevelId );

  OdUInt32  getColorIndex() const;
  void      setColorIndex( OdUInt32 uColorIndex );

  OdUInt32  getLineStyleEntryId() const;
  void      setLineStyleEntryId( OdUInt32 uLineStyleId );

  OdUInt32  getLineWeight() const;
  void      setLineWeight( OdUInt32 uLineWeight );

  double    getTransparency() const;
  void      setTransparency( double dTransparency );

  bool      getOnFlag() const;
  void      setOnFlag( bool bOn );

private:

  OdUInt32               m_uLevelId;
  OdUInt32               m_uColorIndex;
  OdUInt32               m_uLineStyle;
  OdUInt32               m_uLineWeight;
  double                 m_dTransparency;
  bool                   m_bOn;
};

typedef OdSharedPtr<OdDgTerrainGraphicsSettings> OdDgTerrainGraphicsSettingsPtr;

//----------------------------------------------------------------------------

class TG_EXPORT OdDgTerrainSourceFeatureSettings : public OdDgTerrainGraphicsSettings
{
public:

  enum OdDgSourceFeatureType
  {
    kBreakLineFeature = 0,
    kHoleFeature      = 1,
    kIslandFeature    = 2,
    kVoidFeature      = 3,
    kBoundaryFeature  = 4,
    kContourFeature   = 5
  };

public:
  OdDgTerrainSourceFeatureSettings();
  virtual ~OdDgTerrainSourceFeatureSettings();

  OdDgSourceFeatureType getFeatureType() const;
  void setFeatureType( const OdDgSourceFeatureType& uType );

private:

  OdDgSourceFeatureType m_uType;
};

//----------------------------------------------------------------------------

class TG_EXPORT OdDgTerrainTrianglesSettings : public OdDgTerrainGraphicsSettings
{
public:
  OdDgTerrainTrianglesSettings();
  virtual ~OdDgTerrainTrianglesSettings();

  OdUInt32 getMaterialId() const;
  void     setMaterialId( OdUInt32 uEntryId );

private:
  OdUInt32 m_uMaterialId;
};

//----------------------------------------------------------------------------

class TG_EXPORT OdDgTerrainVertexFeatureSettings : public OdDgTerrainGraphicsSettings
{
public:

  enum OdDgTerrainVertexFeatureType
  {
    kSourceVertex   = 0,
    kTriangleVertex = 1,
    kLowPoints      = 2,
    kHighPoints     = 3,
    kFlowArrows     = 4
  };

public:
  OdDgTerrainVertexFeatureSettings();
  virtual ~OdDgTerrainVertexFeatureSettings();

  OdDgTerrainVertexFeatureType getFeatureType() const;
  void setFeatureType( const OdDgTerrainVertexFeatureType& uType );

  bool      getDisplayTextFlag() const;
  void      setDisplayTextFlag( bool bOn );

  double    getCellXScale() const;
  void      setCellXScale( double dScale );

  double    getCellYScale() const;
  void      setCellYScale( double dScale );

  double    getCellZScale() const;
  void      setCellZScale( double dScale );

  OdDgTerrainOptions::OdDgTerrainPointDisplayType getPointDisplayType() const;
  void      setPointDisplayType( OdDgTerrainOptions::OdDgTerrainPointDisplayType uDisplayType );

  OdChar    getCharSymbol() const;
  void      setCharSymbol( const OdChar& chSymbol );

  OdUInt32  getTextStyleEntryId() const;
  void      setTextStyleEntryId( OdUInt32 uTextStyleId );

  OdString  getCellName() const;
  void      setCellName( const OdString& strCellName );

  OdString  getPrefix() const;
  void      setPrefix( const OdString& strPrefix );

  OdString  getSuffix() const;
  void      setSuffix( const OdString& strSuffix );

  double    getMinValue() const;
  void      setMinValue( double dMinValue );

private:

  OdDgTerrainVertexFeatureType  m_uType;
  bool                   m_bDisplayText;
  double                 m_dScaleX;
  double                 m_dScaleY;
  double                 m_dScaleZ;
  OdChar                 m_chSymbol;
  OdString               m_strCellName;
  OdDgTerrainOptions::OdDgTerrainPointDisplayType m_uPointType;
  OdString               m_strPrefix;
  OdString               m_strSuffix;
  double                 m_dMinValue;
  OdUInt32               m_uTextStyle;
};

//----------------------------------------------------------------------------

class TG_EXPORT OdDgTerrainContourFeatureSettings : public OdDgTerrainGraphicsSettings
{
public:

  enum OdDgTerrainContourFeatureType
  {
    kMinorContour   = 0,
    kMajorContour   = 1
  };

public:

  OdDgTerrainContourFeatureSettings();
  virtual ~OdDgTerrainContourFeatureSettings();

  OdDgTerrainContourFeatureType getFeatureType() const;
  void setFeatureType( const OdDgTerrainContourFeatureType& uType );

  double getInterval() const;
  void setInterval( double dInterval );

  OdDgTerrainOptions::OdDgTerrainContourSmoothingType getSmoothingType() const;
  void setSmoothingType( const OdDgTerrainOptions::OdDgTerrainContourSmoothingType& uSmoothingType );

  OdUInt32 getSmoothingFactor() const; // minimum value is 1, maximum value is 10
  void setSmoothingFactor( OdUInt32 uSmoothingFactor );

  bool getDisplayTextFlag() const;
  void setDisplayTextFlag( bool bSet );

  double getTextInterval() const;
  void setTextInterval( double dInterval );

  double getUORPerWorkingUnitsScale() const;
  void setUORPerWorkingUnitsScale( double dScale );

  OdUInt32 getTextStyleEntryId() const;
  void setTextStyleEntryId( OdUInt32 uTextStyleId );

  bool getIgnoreMaxSlopesFlag() const;
  void setIgnoreMaxSlopesFlag( bool bSet );

  double getMaxSlopeValue() const;
  void setMaxSlopeValue( double dValue );

  OdUInt32  getDepressionColorIndex() const;
  void      setDepressionColorIndex( OdUInt32 uColorIndex );

  OdUInt32  getDepressionLineStyleEntryId() const;
  void      setDepressionLineStyleEntryId( OdUInt32 uLineStyleId );

  OdUInt32  getDepressionLineWeight() const;
  void      setDepressionLineWeight( OdUInt32 uLineWeight );

  OdUInt32  getContourLabelPrecision() const;
  void      setContourLabelPrecision( OdUInt32 uPecision );

private:

  OdDgTerrainContourFeatureType   m_uType;
  double                          m_dInterval;
  OdDgTerrainOptions::OdDgTerrainContourSmoothingType m_uSmoothing;
  OdUInt32                        m_uSmoothingFactor;
  bool                            m_bDisplayText;
  double                          m_dTextInterval;
  double                          m_dUORsToMasterUnits;
  OdUInt32                        m_uTextStyleEntryId;
  bool                            m_bIgnoreMaxSlopes;
  double                          m_dMaxSlopeValue;
  OdUInt32                        m_uDepressionColorIndex;
  OdUInt32                        m_uDepressionLineStyle;
  OdUInt32                        m_uDepressionLineWeight;
  OdUInt32                        m_uContourLabelPrecision;
};

//----------------------------------------------------------------------------

class TG_EXPORT OdDgTerrainGraphicsSettingsXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC0136 };

  enum OdDgTerrainGraphicsType
  {
    kNoSettings              = 0,
    kTriangleSettings        = 1,
    kVertexSettings          = 2,
    kContourSettings         = 3,
    kSourceFeatureSettings   = 4
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainGraphicsSettingsXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdDgTerrainGraphicsType getSettingsType() const = 0;
  virtual void                    setSettingsType( OdDgTerrainGraphicsType uType ) = 0;

  virtual OdDgTerrainGraphicsSettingsPtr getGraphicsFeatureSettings() = 0;

};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainGraphicsSettingsXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainGraphicsSettingsXAttribute> OdDgTerrainGraphicsSettingsXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainDisplayStyleXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC0001 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainDisplayStyleXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32                getDisplayStyleEntryId() const = 0;
  virtual void                    setDisplayStyleEntryId( OdUInt32 uDisplayStyleId ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainDisplayStyleXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainDisplayStyleXAttribute> OdDgTerrainDisplayStyleXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTextStyleDataXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58010001 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTextStyleDataXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32 getTextStyleIndex() const = 0;
  virtual void     setTextStyleIndex( OdUInt32 uTextStyleIndex ) = 0;

  virtual OdUInt32 getFontId() const = 0;
  virtual void     setFontId( OdUInt32 uFontId ) = 0;

  virtual OdUInt32 getBigFontId() const = 0;
  virtual void     setBigFontId( OdUInt32 uBigFontId ) = 0;

  virtual double   getTextWidth() const = 0;
  virtual void     setTextWidth( double dTextWidth ) = 0;

  virtual double   getTextHeight() const = 0;
  virtual void     setTextHeight( double dTextHeight ) = 0;

  virtual double   getSlantAngle() const = 0;
  virtual void     setSlantAngle( double dSlantAngle ) = 0;

  virtual double   getLineSpacing() const = 0;
  virtual void     setLineSpacing( double dLineSpacing ) = 0;

  virtual double   getIntercharSpacing() const = 0;
  virtual void     setIntercharSpacing( double dIntercharSpacing ) = 0;

  virtual double   getUnderlineOffset() const = 0;
  virtual void     setUnderlineOffset( double dOffset ) = 0;

  virtual double   getOverlineOffset() const = 0;
  virtual void     setOverlineOffset( double dOffset ) = 0;

  virtual OdUInt32 getCodePage() const = 0;
  virtual void     setCodePage( OdUInt32 uCodePage ) = 0;

  virtual OdGePoint2d getLineOffset() const = 0;
  virtual void        setLineOffset( const OdGePoint2d& ptOffset ) = 0;

  virtual OdDg::TextJustification getTextNodeJustification() const = 0;
  virtual void setTextNodeJustification( const OdDg::TextJustification& uJustification ) = 0;

  virtual OdDg::TextJustification getTextJustification() const = 0;
  virtual void setTextJustification( const OdDg::TextJustification& uJustification ) = 0;

  virtual bool getMarginJustificationFlag() const = 0;
  virtual void setMarginJustificationFlag( bool bSet ) = 0;

  virtual OdUInt16 getLineLength() const = 0;
  virtual void     setLineLength( OdUInt16 uLength ) = 0;

  virtual bool getJapaneseFlag() const = 0;
  virtual void setJapaneseFlag( bool bSet ) = 0;

  virtual bool getVerticalFlag() const = 0;
  virtual void setVerticalFlag( bool bSet ) = 0;

  virtual bool getRightToLeftFlag() const = 0;
  virtual void setRightToLeftFlag( bool bSet ) = 0;

  virtual bool getBackwardsFlag() const = 0;
  virtual void setBackwardsFlag( bool bSet ) = 0;

  virtual bool getUpsideDownFlag() const = 0;
  virtual void setUpsideDownFlag( bool bSet ) = 0;

  virtual OdUInt32    getBackgroundLineStyleEntryId() const = 0;
  virtual void        setBackgroundLineStyleEntryId( OdUInt32 uLineStyleId ) = 0;

  virtual OdUInt32    getBackgroundLineWeight() const = 0;
  virtual void        setBackgroundLineWeight( OdUInt32 uLineWeight ) = 0;

  virtual OdUInt32    getBackgroundColorIndex() const = 0;
  virtual void        setBackgroundColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdUInt32    getBackgroundFillColorIndex() const = 0;
  virtual void        setBackgroundFillColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdGePoint2d getBackgroundBorder() const = 0;
  virtual void        setBackgroundBorder( const OdGePoint2d& ptBorder ) = 0;

  virtual OdUInt32    getUnderlineColorIndex() const = 0;
  virtual void        setUnderlineColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdUInt32    getUnderlineLineStyleEntryId() const = 0;
  virtual void        setUnderlineLineStyleEntryId( OdUInt32 uLineStyleId ) = 0;

  virtual OdUInt32    getUnderlineLineWeight() const = 0;
  virtual void        setUnderlineLineWeight( OdUInt32 uLineWeight ) = 0;

  virtual OdUInt32    getOverlineColorIndex() const = 0;
  virtual void        setOverlineColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdUInt32    getOverlineLineStyleEntryId() const = 0;
  virtual void        setOverlineLineStyleEntryId( OdUInt32 uLineStyleId ) = 0;

  virtual OdUInt32    getOverlineLineWeight() const = 0;
  virtual void        setOverlineLineWeight( OdUInt32 uLineWeight ) = 0;

  virtual OdUInt32    getTextColorIndex() const = 0;
  virtual void        setTextColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual bool        getUnderlineFlag() const = 0;
  virtual void        setUnderlineFlag( bool bSet ) = 0;

  virtual bool        getOverlineFlag() const = 0;
  virtual void        setOverlineFlag( bool bSet ) = 0;

  virtual bool        getItalicFlag() const = 0;
  virtual void        setItalicFlag( bool bSet ) = 0;

  virtual bool        getBoldFlag() const = 0;
  virtual void        setBoldFlag( bool bSet ) = 0;

  virtual bool        getSuperScriptFlag() const = 0;
  virtual void        setSuperScriptFlag( bool bSet ) = 0;

  virtual bool        getSubScriptFlag() const = 0;
  virtual void        setSubScriptFlag( bool bSet ) = 0;

  virtual bool        getBackgroundFlag() const = 0;
  virtual void        setBackgroundFlag( bool bSet ) = 0;

  virtual bool        getOverlineStyleFlag() const = 0;
  virtual void        setOverlineStyleFlag( bool bSet ) = 0;

  virtual bool        getUnderlineStyleFlag() const = 0;
  virtual void        setUnderlineStyleFlag( bool bSet ) = 0;

  virtual bool        getFixedSpacingFlag() const = 0;
  virtual void        setFixedSpacingFlag( bool bSet ) = 0;

  virtual bool        getFractionFlag() const = 0;
  virtual void        setFractionFlag( bool bSet ) = 0;

  virtual bool        getTextColorFlag() const = 0;
  virtual void        setTextColorFlag( bool bSet ) = 0;

  virtual bool        getACadIntercharSpacingFlag() const = 0;
  virtual void        setACadIntercharSpacingFlag( bool bSet ) = 0;

  virtual bool        getFullJustificationFlag() const = 0;
  virtual void        setFullJustificationFlag( bool bSet ) = 0;

  virtual OdDgTextExtendedProperties::LineSpacingType getLineSpacingType() const = 0;
  virtual void        setLineSpacingType( const OdDgTextExtendedProperties::LineSpacingType& uType ) = 0;

  virtual void        initFromTextStyle( const OdDgTextStyleTableRecordPtr& pTextStyle ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTextStyleDataXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTextStyleDataXAttribute> OdDgTextStyleDataXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainControlReferenceXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC0064 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainControlReferenceXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt64 getTerrainControlElementId() const = 0;
  virtual void     setTerrainControlElementId( OdUInt64 uElementId ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainControlReferenceXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainControlReferenceXAttribute> OdDgTerrainControlReferenceXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgExtendedElementTypeXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x56EC0000 };

  enum OdDgExtendedElementType
  {
    kSectionClipElement             = 0x006D0000,
    kXRefViewsElement               = 0x006B0002,
    kDisplayStyleElement            = 0x006F0000,
    kDetailingSymbolElement         = 0x57000001,
    kLuxSetupElement                = 0x58740000,
    kEnvironmentMapTableElement     = 0x58740001,
    kExtendedElementWithGrCache     = 0x587E0000,
    kTerrainCtrlElement             = 0x58EC0015,
    kTerrainGraphElement            = 0x58EC000B,
    kTerrainGraphElementCE          = 0x58D3000C,
    kTableElement                   = 0x592D0000,
    kItemLibrary                    = 0xEC340000,
    kTablePrototype                 = 0xEC3A0000,
    kVariables                      = 0x58E40005,
    kConstraints2d                  = 0x56DA001E
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgExtendedElementTypeXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdDgExtendedElementType getElementType() const = 0;
  virtual void                    setElementType( const OdDgExtendedElementType& uType ) = 0;
  virtual OdUInt32                getFlags() const = 0;
  virtual void                    setFlags( OdUInt32 uFlags ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgExtendedElementTypeXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgExtendedElementTypeXAttribute> OdDgExtendedElementTypeXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainInfoXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC012C };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainInfoXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32 getNumberOfPoints() const = 0;
  virtual void     setNumberOfPoints( OdUInt32 nPoints ) = 0;

  virtual OdUInt32 getNumberOfEdges() const = 0;
  virtual void     setNumberOfEdges( OdUInt32 nEdges ) = 0;

  virtual OdUInt32 getNumberOfTriangles() const = 0;
  virtual void     setNumberOfTriangles( OdUInt32 nTriangles ) = 0;

  virtual OdUInt32 getNumberOfFeatures() const = 0;
  virtual void     setNumberOfFeatures( OdUInt32 nFeatures ) = 0;

  virtual OdUInt32 getNumberOfEdgeRecords() const = 0;
  virtual void     setNumberOfEdgeRecords( OdUInt32 nEdgeRecords ) = 0;

  virtual OdUInt32 getNumberOfFeatureTopologyRecords() const = 0;
  virtual void     setNumberOfFeatureTopologyRecords( OdUInt32 nFeatureTopologyRecords ) = 0;

  virtual OdUInt32 getNumberOfFeatureLinkages() const = 0;
  virtual void     setNumberOfFeatureLinkages( OdUInt32 nLinkages ) = 0;

  virtual OdUInt32 getNumberOfVertexTopoLinkages() const = 0;
  virtual void     setNumberOfVertexTopoLinkages( OdUInt32 nLinkages ) = 0;

  virtual OdUInt32 getNumberOfVertexLinkages() const = 0;
  virtual void     setNumberOfVertexLinkages( OdUInt32 nLinkages ) = 0;

  virtual OdUInt32 getNumberOfEdgeLinkages() const = 0;
  virtual void     setNumberOfEdgeLinkages( OdUInt32 nLinkages ) = 0;

  virtual OdUInt32 getNumberOfFeatureTopoLinkages() const = 0;
  virtual void     setNumberOfFeatureTopoLinkages( OdUInt32 nLinkages ) = 0;

  virtual OdUInt32 getNumberOfUnknownLinkages() const = 0;
  virtual void     setNumberOfUnknownLinkages( OdUInt32 nLinkages ) = 0;

  virtual OdUInt32 getFirstBoundaryIndex() const = 0;
  virtual void     setFirstBoundaryIndex( OdUInt32 uFirstBoundaryIndex ) = 0;

  virtual OdDgTerrainControlElement::OdDgTerrainEdgeMethod getEdgeMethod() const = 0;
  virtual void     setEdgeMethod( OdDgTerrainControlElement::OdDgTerrainEdgeMethod uEdgeMethod ) = 0;

  virtual OdUInt32 getNumberOfNativeTrianglesAndFeatures() const = 0;
  virtual void     setNumberOfNativeTrianglesAndFeatures( OdUInt32 nTriAndFeatures ) = 0;

  virtual OdUInt32 getUnknownValue1() const = 0;
  virtual void     setUnknownValue1( OdUInt32 uValue ) = 0;

  virtual OdUInt32 getUnknownValue2() const = 0;
  virtual void     setUnknownValue2( OdUInt32 uValue ) = 0;

  virtual OdUInt32 getUnknownValue3() const = 0;
  virtual void     setUnknownValue3( OdUInt32 uValue ) = 0;

  virtual OdUInt32 getUnknownValue4() const = 0;
  virtual void     setUnknownValue4( OdUInt32 uValue ) = 0;

  virtual OdUInt32 getUnknownFlags1() const = 0;
  virtual void     setUnknownFlags1( OdUInt32 uFlagsValue ) = 0;

  virtual OdUInt32 getUnknownFlags2() const = 0;
  virtual void     setUnknownFlags2( OdUInt32 uFlagsValue ) = 0;

  virtual OdUInt32 getUnknownFlags3() const = 0;
  virtual void     setUnknownFlags3( OdUInt32 uFlagsValue ) = 0;

  virtual OdUInt32 getUnknownFlags4() const = 0;
  virtual void     setUnknownFlags4( OdUInt32 uFlagsValue ) = 0;

  virtual OdUInt32 getUnknownFlags5() const = 0;
  virtual void     setUnknownFlags5( OdUInt32 uFlagsValue ) = 0;

  virtual OdUInt32 getMaxPointsOr1000() const = 0;
  virtual void     setMaxPointsOr1000( OdUInt32 uValue ) = 0;

  virtual OdUInt32 getMaxPointsOr100() const = 0;
  virtual void     setMaxPointsOr100( OdUInt32 uValue ) = 0;

  virtual double   getMaxTriangleLength() const = 0;
  virtual void     setMaxTriangleLength( double dLength ) = 0;

  virtual OdGeExtents3d getModelExtents() const = 0;
  virtual void     setModelExtents( const OdGeExtents3d& extModel ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainInfoXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainInfoXAttribute> OdDgTerrainInfoXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/

struct OdDgTerrainFeatureDescriptionItem
{
public:
  OdDgTerrainFeature::OdDgTerrainFeatureType m_uFeatureType;
  OdUInt32 m_nPoints;
  OdUInt32 m_uFeatureNumber;
  OdUInt32 m_uVertexIndex;
  OdUInt32 m_uUnknownValue1;
  OdUInt32 m_uUnknownValue2;
  bool     m_bCorruptedFeature;
};

class TG_EXPORT OdDgTerrainFeatureDescriptionXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC012D };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainFeatureDescriptionXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32    getFeatureDescriptionCount() const = 0;
  virtual OdDgTerrainFeatureDescriptionItem getFeatureDescription( OdUInt32 uFeatureIndex ) = 0;
  virtual void        setFeatureDescription( OdUInt32 uFeatureIndex, const OdDgTerrainFeatureDescriptionItem& featureItem ) = 0;
  virtual void        addFeatureDescription( const OdDgTerrainFeatureDescriptionItem& featureItem ) = 0;
  virtual void        deleteFeatureDescription( OdUInt32 uFeatureIndex ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainFeatureDescriptionXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainFeatureDescriptionXAttribute> OdDgTerrainFeatureDescriptionXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/

struct OdDgTerrainFeatureTopologyItem
{
  OdUInt32 m_uItemIndex;
  OdInt32  m_uEndVertexIndex;
  OdUInt32 m_uFeatureNumber;
  OdInt32  m_uStartVertexItemIndex;
  OdUInt32 m_uUnknownIndex;
};

class TG_EXPORT OdDgTerrainFeatureTopologyXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC0131 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainFeatureTopologyXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32    getFeatureTopologyItemCount() const = 0;
  virtual OdDgTerrainFeatureTopologyItem getFeatureTopologyItem( OdUInt32 uItemIndex ) = 0;
  virtual void        setFeatureTopologyItem( OdUInt32 uItemIndex, const OdDgTerrainFeatureTopologyItem& featureItem ) = 0;
  virtual void        addFeatureTopologyItem( const OdDgTerrainFeatureTopologyItem& featureItem ) = 0;
  virtual void        deleteFeatureTopologyItem( OdUInt32 uItemIndex ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainFeatureTopologyXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainFeatureTopologyXAttribute> OdDgTerrainFeatureTopologyXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainStringXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x587A0000 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainStringXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdString getString() = 0;
  virtual void     setString( const OdString& strValue ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainStringXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainStringXAttribute> OdDgTerrainStringXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgTerrainCorruptedFeatureXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC0140 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainCorruptedFeatureXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32    getVerticesCount() const = 0;
  virtual OdGePoint3d getVertex( OdUInt32 uVertexIndex ) = 0;
  virtual void        setVertex( OdUInt32 uVertexIndex, const OdGePoint3d& ptVertex ) = 0;
  virtual void        addVertex( const OdGePoint3d& ptVertex ) = 0;
  virtual void        deleteVertex( OdUInt32 uVertexIndex ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainCorruptedFeatureXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainCorruptedFeatureXAttribute> OdDgTerrainCorruptedFeatureXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/

struct OdDgTerrainVertexTopologyItem
{
  OdUInt32 m_iFlags;
  OdInt32 m_iNextBoundaryIndex;
  OdInt32 m_iTriangleStartIndex;
  OdInt32 m_iUnknownIndex2;
  OdInt32 m_iUnknownIndex3;
  OdInt32 m_iFeaturePtIndex;
};

class TG_EXPORT OdDgTerrainVertexTopologyXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC012F };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainVertexTopologyXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32    getVerticesCount() const = 0;
  virtual OdDgTerrainVertexTopologyItem getVertexTopology( OdUInt32 uVertexNumber ) = 0;
  virtual void        setVertexTopology( OdUInt32 uVertexNumber, const OdDgTerrainVertexTopologyItem& topologyItem ) = 0;
  virtual void        addVertexTopology( const OdDgTerrainVertexTopologyItem& topologyItem ) = 0;
  virtual void        deleteVertexTopology( OdUInt32 uVertexNumber ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainVertexTopologyXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainVertexTopologyXAttribute> OdDgTerrainVertexTopologyXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/

struct OdDgTerrainEdgeOneDirectionRecord
{
  OdUInt32 m_uRecordIndex;
  OdUInt32 m_uVertexIndex;
  OdInt32  m_iNextTriangleEdgeIndex;
};

struct OdDgTerrainEdgeItem
{
  OdDgTerrainEdgeOneDirectionRecord m_startToEnd;
  OdDgTerrainEdgeOneDirectionRecord m_endToStart;
};

class TG_EXPORT OdDgTerrainEdgesXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x58EC0130 };

public:

  ODRX_DECLARE_MEMBERS(OdDgTerrainEdgesXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt32    getEdgeCount() const = 0;
  virtual OdDgTerrainEdgeItem getEdge( OdUInt32 uEdgeNumber ) = 0;
  virtual void        setEdge( OdUInt32 uEdgeNumber, const OdDgTerrainEdgeItem& edgeItem ) = 0;
  virtual void        addEdge( const OdDgTerrainEdgeItem& edgeItem ) = 0;
  virtual void        deleteEdge( OdUInt32 uEdgeNumber ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTerrainEdgesXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTerrainEdgesXAttribute> OdDgTerrainEdgesXAttributePtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/

class OdDgGeometryCacheAction;
typedef OdSmartPtr<OdDgGeometryCacheAction> OdDgGeometryCacheActionPtr;

class TG_EXPORT OdDgGeometryCacheAction : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgGeometryCacheAction);

  enum OdDgGeometryCacheActionType
  {
      kUnknown                    = 0x00
    , kPolyline3d                 = 0x01
    , kPolyline2d                 = 0x02
    , kArc3d                      = 0x05
    , kArc2d                      = 0x06
    , kEllipse3d                  = 0x07
    , kEllipse2d                  = 0x08
    , kPolygon3d                  = 0x09
    , kPolygon2d                  = 0x0A
    , kCone                       = 0x0B
    , kTorus                      = 0x0C
    , kPushTransform              = 0x0D
    , kPopTransform               = 0x0E
    , kBeginSolid                 = 0x10
    , kBeginExtrusionSolid        = 0x11
    , kBeginRevolutionSolid       = 0x12
    , kEndSolid                   = 0x13
    , kBeginComplexString         = 0x14
    , kBeginComplexShapeOrGrHole  = 0x15
    , kEndComplex                 = 0x16
    , kMarkHole                   = 0x17
    , kBrepData                   = 0x18
    , kMesh                       = 0x19
    , kBSplineCurve3d             = 0x1A
    , kBSplineSurface             = 0x1B
    , kSetTrainsData              = 0x1C
    , kSlabSolid                  = 0x1E
    , kSphere                     = 0x1F 
    , kOriginAndRotation          = 0x22
    , kModifiedProfileBegin       = 0x23
    , kModifiedProfileEnd         = 0x24
    , kText                       = 0x25
    , kByLevelOrGradientData      = 0x28 
    , kExtraDataSectionBegin      = 0x29 // Used to store level data and for gradient.
    , kExtraDataSectionEnd        = 0x2c
    , kHatchPattern               = 0x31
  };

  virtual OdDgGeometryCacheActionType getType() const = 0;

  virtual OdBinaryData getProxyData() const = 0;
  virtual bool         setProxyData( const OdBinaryData& binData ) = 0;

  virtual bool         isCorrupted() const = 0; // initialized only proxy data if true.

  virtual void         scale( double dScale ) = 0;
  virtual OdUInt32     getDataSize() const = 0;
  virtual void         transformBy( const OdGeMatrix3d& matTransform ) = 0;

  virtual bool         getModificationFlag() const = 0;
  virtual void         setModificationFlag( bool bSet ) = 0;

  virtual OdDgElementId getAssignedElementId() const = 0;
  virtual void          setAssignedElementId( const OdDgElementId& idAttachedElm ) = 0;

  static OdDgGeometryCacheActionPtr createObject( OdDgGeometryCacheActionType type );
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgProxyGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgProxyGeometryCacheAction );

  virtual void setType( OdDgGeometryCacheActionType uType ) = 0;
};

typedef OdSmartPtr<OdDgProxyGeometryCacheAction> OdDgProxyGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPolyline3dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPolyline3dGeometryCacheAction );

  virtual OdUInt32 getVertexCount() const = 0;
  virtual OdGePoint3d getVertex( OdUInt32 uIndex ) = 0;
  virtual void setVertex( OdUInt32 uIndex, const OdGePoint3d& ptVertex ) = 0;
  virtual void addVertex( const OdGePoint3d& ptVertex ) = 0;
  virtual void deleteVertex( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgPolyline3dGeometryCacheAction> OdDgPolyline3dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgArc3dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgArc3dGeometryCacheAction );

  virtual OdGePoint3d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual OdGeMatrix3d getRotation() const = 0;
  virtual void setRotation( const OdGeMatrix3d& matRotation ) = 0;
  virtual double getPrimaryRadius() const = 0;
  virtual void setPrimaryRadius( double dRadius ) = 0;
  virtual double getSecondaryRadius() const = 0;
  virtual void setSecondaryRadius( double dRadius ) = 0;
  virtual double getStartAngle() const = 0;
  virtual void setStartAngle( double dAngle ) = 0;
  virtual double getSweepAngle() const = 0;
  virtual void setSweepAngle( double dAngle ) = 0;
};

typedef OdSmartPtr<OdDgArc3dGeometryCacheAction> OdDgArc3dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgArc2dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgArc2dGeometryCacheAction );

  virtual OdGePoint2d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint2d& ptOrigin ) = 0;
  virtual double getRotationAngle() const = 0;
  virtual void setRotationAngle( double dAngle ) = 0;
  virtual double getPrimaryRadius() const = 0;
  virtual void setPrimaryRadius( double dRadius ) = 0;
  virtual double getSecondaryRadius() const = 0;
  virtual void setSecondaryRadius( double dRadius ) = 0;
  virtual double getStartAngle() const = 0;
  virtual void setStartAngle( double dAngle ) = 0;
  virtual double getSweepAngle() const = 0;
  virtual void setSweepAngle( double dAngle ) = 0;
  virtual double getPriority() const = 0;
  virtual void setPriority( double dPriority ) = 0;
};

typedef OdSmartPtr<OdDgArc2dGeometryCacheAction> OdDgArc2dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgEllipse3dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgEllipse3dGeometryCacheAction );

  virtual OdGePoint3d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual OdGeQuaternion getRotation() const = 0;
  virtual void setRotation( const OdGeQuaternion& quatRotation ) = 0;
  virtual double getPrimaryRadius() const = 0;
  virtual void setPrimaryRadius( double dRadius ) = 0;
  virtual double getSecondaryRadius() const = 0;
  virtual void setSecondaryRadius( double dRadius ) = 0;
};

typedef OdSmartPtr<OdDgEllipse3dGeometryCacheAction> OdDgEllipse3dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgEllipse2dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgEllipse2dGeometryCacheAction );

  virtual OdGePoint2d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint2d& ptOrigin ) = 0;
  virtual double getRotationAngle() const = 0;
  virtual void setRotationAngle( double dAngle ) = 0;
  virtual double getPrimaryRadius() const = 0;
  virtual void setPrimaryRadius( double dRadius ) = 0;
  virtual double getSecondaryRadius() const = 0;
  virtual void setSecondaryRadius( double dRadius ) = 0;
  virtual double getPriority() const = 0;
  virtual void setPriority( double dPriority ) = 0;
};

typedef OdSmartPtr<OdDgEllipse2dGeometryCacheAction> OdDgEllipse2dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBrepGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBrepGeometryCacheAction );

  virtual OdGeMatrix3d getTransformationMatrix() const = 0;
  virtual void setTransformationMatrix( const OdGeMatrix3d& matTransform ) = 0;
  virtual OdBinaryData getBrepData() const = 0;
  virtual void setBrepData( const OdBinaryData& binData ) = 0;
  virtual bool isAcisBrep() const = 0;
  virtual void setAcisBrepFlag( bool bSet ) = 0;
  virtual double       getBrepScale() const = 0;
  virtual void         setBrepScale( double dScale ) = 0;
};

typedef OdSmartPtr<OdDgBrepGeometryCacheAction> OdDgBrepGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBSplineSurfaceGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBSplineSurfaceGeometryCacheAction );

  virtual bool         getFirstBoundaryIsHoleFlag() const = 0;
  virtual void         setFirstBoundaryIsHoleFlag( bool bSet ) = 0;
  virtual OdUInt32     getRulesU() const = 0;
  virtual void         setRulesU( OdUInt32 nRules ) = 0;
  virtual OdUInt32     getRulesV() const = 0;
  virtual void         setRulesV( OdUInt32 nRules ) = 0;

  virtual OdUInt32     getBoundariesCount() const = 0;
  virtual void         addBoundary(const OdGePoint2dArray& arrBoundaryPts ) = 0;
  virtual void         getBoundary( OdUInt32 uIndex, OdGePoint2dArray& arrBoundaryPts ) const = 0;
  virtual void         setBoundary( OdUInt32 uIndex, const OdGePoint2dArray& arrBoundaryPts ) = 0;
  virtual void         deleteBoundary( OdUInt32 uIndex ) = 0;

  virtual OdResult get( OdUInt8& uOrderU, OdUInt8& uOrderV, bool& bRational, bool& bClosedInU, bool& bClosedInV,
    OdInt32& iUNumControlPoints, OdInt32& iVNumControlPoints, OdGePoint3dArray& ctrlPtsArr, 
    OdGeDoubleArray& weights, OdGeKnotVector& uKnots, OdGeKnotVector& vKnots) const = 0;

  virtual OdResult set (OdUInt8 uOrderU, OdUInt8 uOrderV, bool bRational, bool bClosedInU, bool bClosedInV,
    OdInt32 iUNumControlPoints, OdInt32 iVNumControlPoints, const OdGePoint3dArray& ctrlPtsArr, 
    const OdGeDoubleArray& weights, const OdGeKnotVector& uKnots, const OdGeKnotVector& vKnots) = 0; 
};

typedef OdSmartPtr<OdDgBSplineSurfaceGeometryCacheAction> OdDgBSplineSurfaceGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgConeGeometryCacheAction : public OdDgGeometryCacheAction // Cone
{
public:
  ODRX_DECLARE_MEMBERS( OdDgConeGeometryCacheAction );

  virtual OdGeVector3d getRefVector() const = 0;                    
  virtual void setRefVector( const OdGeVector3d& vrRef ) = 0;       
  virtual OdGeVector3d getYDirectionVector() const = 0;             
  virtual void setYDirectionVector( const OdGeVector3d& vrDir ) = 0; 
  virtual OdGePoint3d getStartOrigin() const = 0;
  virtual void setStartOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual OdGePoint3d getEndOrigin() const = 0;
  virtual void setEndOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual double getStartRadius() const = 0;
  virtual void setStartRadius( double dRadius ) = 0;
  virtual double getEndRadius() const = 0;
  virtual void setEndRadius( double dRadius ) = 0;
  virtual bool getSolidFlag() const = 0;
  virtual void setSolidFlag( bool bSolid ) = 0;
};

typedef OdSmartPtr<OdDgConeGeometryCacheAction> OdDgConeGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgGeometryCacheMeshFace
{
  public:
    OdDgGeometryCacheMeshFace();
    virtual ~OdDgGeometryCacheMeshFace();

    // Cache actions doesn't use colorIndexes to set different colors of mesh edges, only RGB colors

    OdUInt32 getPointCount() const;
    OdDgMeshFaceLoops::FacePoint getPoint( OdUInt32 uIndex ) const;
    void setPoint( OdUInt32 uIndex, const OdDgMeshFaceLoops::FacePoint& point );
    void addPoint( const OdDgMeshFaceLoops::FacePoint& point );
    void deletePoint( OdUInt32 uIndex );

  private:
    OdArray<OdDgMeshFaceLoops::FacePoint> m_arrPoints;
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgMeshGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgMeshGeometryCacheAction );

  virtual bool getUseDoubleColorsFlag() const = 0;
  virtual bool getUseNormalsFlag() const = 0;
  virtual bool getUseTextureCoordinatesFlag() const = 0;
  virtual void setUseDoubleColorsFlag( bool bSet ) = 0;
  virtual void setUseNormalsFlag( bool bSet ) = 0;
  virtual void setUseTextureCoordinatesFlag( bool bSet ) = 0;

  virtual OdUInt32 getFaceCount() const = 0;
  virtual OdDgGeometryCacheMeshFace getFace( OdUInt32 uIndex ) const = 0;
  virtual void setFace( OdUInt32 uIndex, const OdDgGeometryCacheMeshFace& meshFace ) = 0;
  virtual void addFace( const OdDgGeometryCacheMeshFace& meshFace ) = 0;
  virtual void deleteFace( OdUInt32 uIndex ) = 0;

  virtual OdUInt32 getVertexCount() const = 0;
  virtual OdGePoint3d getVertex( OdUInt32 uIndex ) const = 0;
  virtual void setVertex( OdUInt32 uIndex, const OdGePoint3d& ptVertex ) = 0;
  virtual void addVertex( const OdGePoint3d& ptVertex ) = 0;
  virtual void deleteVertex( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgMeshGeometryCacheAction> OdDgMeshGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPolyline2dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPolyline2dGeometryCacheAction );

  virtual double getPriority() const = 0;
  virtual void   setPriority( double dPriority ) = 0;

  virtual OdUInt32 getVertexCount() const = 0;
  virtual OdGePoint2d getVertex( OdUInt32 uIndex ) = 0;
  virtual void setVertex( OdUInt32 uIndex, const OdGePoint2d& ptVertex ) = 0;
  virtual void addVertex( const OdGePoint2d& ptVertex ) = 0;
  virtual void deleteVertex( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgPolyline2dGeometryCacheAction> OdDgPolyline2dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPolygon3dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPolygon3dGeometryCacheAction );

  virtual OdUInt32 getVertexCount() const = 0;
  virtual OdGePoint3d getVertex( OdUInt32 uIndex ) = 0;
  virtual void setVertex( OdUInt32 uIndex, const OdGePoint3d& ptVertex ) = 0;
  virtual void addVertex( const OdGePoint3d& ptVertex ) = 0;
  virtual void deleteVertex( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgPolygon3dGeometryCacheAction> OdDgPolygon3dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPolygon2dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPolygon2dGeometryCacheAction );

  virtual double getPriority() const = 0;
  virtual void   setPriority( double dPriority ) = 0;

  virtual OdUInt32 getVertexCount() const = 0;
  virtual OdGePoint2d getVertex( OdUInt32 uIndex ) = 0;
  virtual void setVertex( OdUInt32 uIndex, const OdGePoint2d& ptVertex ) = 0;
  virtual void addVertex( const OdGePoint2d& ptVertex ) = 0;
  virtual void deleteVertex( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgPolygon2dGeometryCacheAction> OdDgPolygon2dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPushTransformGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPushTransformGeometryCacheAction );

  virtual OdGeMatrix3d getTransformMatrix() const = 0;
  virtual void setTransformMatrix( const OdGeMatrix3d& matTransform ) = 0;
};

typedef OdSmartPtr<OdDgPushTransformGeometryCacheAction> OdDgPushTransformGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPopTransformGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPopTransformGeometryCacheAction );
};

typedef OdSmartPtr<OdDgPopTransformGeometryCacheAction> OdDgPopTransformGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgModifiedProfileBeginGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgModifiedProfileBeginGeometryCacheAction );
};

typedef OdSmartPtr<OdDgModifiedProfileBeginGeometryCacheAction> OdDgModifiedProfileBeginGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgModifiedProfileEndGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgModifiedProfileEndGeometryCacheAction );
};

typedef OdSmartPtr<OdDgModifiedProfileEndGeometryCacheAction> OdDgModifiedProfileEndGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgExtraDataSectionBeginGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgExtraDataSectionBeginGeometryCacheAction );

  virtual OdUInt32     getSectionBlockSize() const = 0;
  virtual void         setSectionBlockSize( OdUInt32 uBlockSizeInBytes ) = 0;

  virtual bool         isLevelIndexData() const = 0;
  virtual OdUInt32     getLevelIndex() const = 0;
  virtual void         setLevelIndex( OdUInt32 uLevelIndex ) = 0;

  virtual bool         isExtraProxyData() const = 0;
  virtual OdUInt16     getExtraProxyDataType() const = 0;
  virtual void         setExtraProxyDataType( OdUInt16 uType ) = 0;
  virtual OdBinaryData getExtraProxyData() const = 0;
  virtual void         setExtraProxyData( const OdBinaryData& proxyData ) = 0;
};

typedef OdSmartPtr<OdDgExtraDataSectionBeginGeometryCacheAction> OdDgExtraDataSectionBeginGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgExtraDataSectionEndGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgExtraDataSectionEndGeometryCacheAction );
};

typedef OdSmartPtr<OdDgExtraDataSectionEndGeometryCacheAction> OdDgExtraDataSectionEndGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBeginSolidGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBeginSolidGeometryCacheAction );

  virtual bool isSolid() const = 0;
  virtual void setSolidFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgBeginSolidGeometryCacheAction> OdDgBeginSolidGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgEndSolidGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgEndSolidGeometryCacheAction );
};

typedef OdSmartPtr<OdDgEndSolidGeometryCacheAction> OdDgEndSolidGeometryCacheActionPtr;

//------------------------------------------------------------------------------------
// All hatch pattern data except geometry items must be in UORs.

class TG_EXPORT OdDgHatchPatternGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgHatchPatternGeometryCacheAction );

  virtual bool getUseSpace1Flag() const = 0;
  virtual void setUseSpace1Flag( bool bSet ) = 0;
  virtual bool getUseAngle1Flag() const = 0;
  virtual void setUseAngle1Flag( bool bSet ) = 0;
  virtual bool getUseSpace2Flag() const = 0;
  virtual void setUseSpace2Flag( bool bSet ) = 0;
  virtual bool getUseAngle2Flag() const = 0;
  virtual void setUseAngle2Flag( bool bSet ) = 0;
  virtual bool getUseScaleFlag() const = 0;
  virtual void setUseScaleFlag( bool bSet ) = 0;
  virtual bool getCellFlag() const = 0;
  virtual void setCellFlag( bool bSet ) = 0;
  virtual bool getUseToleranceFlag() const = 0;
  virtual void setUseToleranceFlag( bool bSet ) = 0;
  virtual bool getUseLineStyleFlag() const = 0;
  virtual void setUseLineStyleFlag( bool bSet ) = 0;
  virtual bool getUseLineWeightFlag() const = 0;
  virtual void setUseLineWeightFlag( bool bSet ) = 0;
  virtual bool getUseLineColorFlag() const = 0;
  virtual void setUseLineColorFlag( bool bSet ) = 0;
  virtual bool getSnappableFlag() const = 0;
  virtual void setSnappableFlag( bool bSet ) = 0;
  virtual bool getUseRotationFlag() const = 0;
  virtual void setUseRotationFlag( bool bSet ) = 0;
  virtual bool getUseOffsetFlag() const = 0;
  virtual void setUseOffsetFlag( bool bSet ) = 0;
  virtual bool getUseHoleStyleFlag() const = 0;
  virtual void setUseHoleStyleFlag( bool bSet ) = 0;
  virtual bool getUseDWGHatchDefFlag() const = 0;
  virtual void setUseDWGHatchDefFlag( bool bSet ) = 0;
  virtual bool getUseMLineFlag() const = 0;
  virtual void setUseMLineFlag( bool bSet ) = 0;
  virtual bool getUseOriginFlag() const = 0;
  virtual void setUseOriginFlag( bool bSet ) = 0;
  virtual bool getUsePixelSizeFlag() const = 0;
  virtual void setUsePixelSizeFlag( bool bSet ) = 0;
  virtual bool getUseIslandStyleFlag() const = 0;
  virtual void setUseIslandStyleFlag( bool bSet ) = 0;
  virtual bool getTrueScaleFlag() const = 0;
  virtual void setTrueScaleFlag( bool bSet ) = 0;
  virtual bool getRawDWGLoopsFlag() const = 0;
  virtual void setRawDWGLoopsFlag( bool bSet ) = 0;

  virtual OdGePoint3d getSeedPoint() const = 0;
  virtual void setSeedPoint( const OdGePoint3d& ptSeed ) = 0;
  virtual OdGePoint3d getMinExtPoint() const = 0;
  virtual void setMinExtPoint( const OdGePoint3d& ptExt ) = 0;
  virtual OdGePoint3d getMaxExtPoint() const = 0;
  virtual void setMaxExtPoint( const OdGePoint3d& ptExt ) = 0;

  virtual double getSpace1() const = 0;
  virtual void   setSpace1( double dSpace ) = 0;
  virtual double getAngle1() const = 0;
  virtual void   setAngle1( double dAngle ) = 0;
  virtual double getSpace2() const = 0;
  virtual void   setSpace2( double dSpace ) = 0;
  virtual double getAngle2() const = 0;
  virtual void   setAngle2( double dAngle ) = 0;
  virtual double getScale() const = 0;
  virtual void   setScale( double dScale ) = 0;
  virtual double getTolerance() const = 0;
  virtual void   setTolerance( double dTolerance ) = 0;
  virtual OdUInt32 getLineStyle() const = 0;
  virtual void   setLineStyle( OdUInt32 uLineStyle ) = 0;
  virtual OdUInt32 getLineWeight() const = 0;
  virtual void   setLineWeight( OdUInt32 uLineWeight ) = 0;
  virtual OdUInt32 getLineColor() const = 0;
  virtual void   setLineColor( OdUInt32 uLineColor ) = 0;
  virtual OdGeQuaternion getRotation() const = 0;
  virtual void   setRotation( const OdGeQuaternion& quatRotation ) = 0;
  virtual OdGePoint3d getOffset() const = 0;
  virtual void setOffset( const OdGePoint3d& ptOffset ) = 0;
  virtual OdUInt32 getMLineLowIndex() const = 0;
  virtual void   setMLineLowIndex( OdUInt32 uIndex ) = 0;
  virtual OdUInt32 getMLineHighIndex() const = 0;
  virtual void   setMLineHighIndex( OdUInt32 uIndex ) = 0;
  virtual OdUInt16 getHoleStyle() const = 0;
  virtual void   setHoleStyle( OdUInt16 uStyle ) = 0;
  virtual OdUInt16 getIslandStyle() const = 0;
  virtual void   setIslandStyle( OdUInt16 uStyle ) = 0;
  virtual double getPixelSize() const = 0;
  virtual void   setPixelSize( double dPixelSize ) = 0;

  virtual OdUInt32 getDwgHatchLineCount() const = 0;
  virtual OdDgDWGPatternLinkage::DWGHatchLine getDwgHatchLine( OdUInt32 uIndex ) const = 0;
  virtual void     setDwgHatchLine( OdUInt32 uIndex, const OdDgDWGPatternLinkage::DWGHatchLine& hatchLine ) = 0;
  virtual void     addDwgHatchLine( const OdDgDWGPatternLinkage::DWGHatchLine& hatchLine ) = 0;
  virtual void     deleteDwgHatchLine( OdUInt32 uIndex ) = 0;

  virtual OdUInt32 getGeometryItemsCount() const = 0;
  virtual OdDgGeometryCacheActionPtr getGeometryItem( OdUInt32 uIndex ) const = 0;
  virtual void     setGeometryItem( OdUInt32 uIndex, const OdDgGeometryCacheActionPtr& pItem ) = 0;
  virtual void     addGeometryItem( const OdDgGeometryCacheActionPtr& pItem ) = 0;
  virtual void     deleteGeometryItem( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgHatchPatternGeometryCacheAction> OdDgHatchPatternGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBeginExtrusionSolidGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBeginExtrusionSolidGeometryCacheAction );

  virtual OdGeVector3d getExtrusionVector() const = 0;
  virtual void setExtrusionVector( const OdGeVector3d& vrExtrusion ) = 0;

  virtual bool isSolid() const = 0;
  virtual void setSolidFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgBeginExtrusionSolidGeometryCacheAction> OdDgBeginExtrusionSolidGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBeginRevolutionSolidGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBeginRevolutionSolidGeometryCacheAction );

  virtual OdGeVector3d getRotPlaneXAxis() const = 0;
  virtual void         setRotPlaneXAxis( const OdGeVector3d& vrAxis ) = 0;
  virtual OdGeVector3d getRotPlaneYAxis() const = 0;
  virtual void         setRotPlaneYAxis( const OdGeVector3d& vrAxis ) = 0;
  virtual OdGePoint3d  getOrigin() const = 0;
  virtual void         setOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual double       getScaleX() const = 0;
  virtual void         setScaleX( double dScale ) = 0;
  virtual double       getScaleY() const = 0;
  virtual void         setScaleY( double dScale ) = 0;
  virtual OdGeVector3d getNormalOfSecondBoundary() const = 0;
  virtual void         setNormalOfSecondBoundary( const OdGeVector3d& vrNormal ) = 0;
  virtual double       getSweepAngle() const = 0;
  virtual void         setSweepAngle( double dAngle ) = 0;
  virtual bool         getSolidFlag() const = 0;
  virtual void         setSolidFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgBeginRevolutionSolidGeometryCacheAction> OdDgBeginRevolutionSolidGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgSlabSolidGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgSlabSolidGeometryCacheAction );

  virtual OdGeVector3d getXAxis() const = 0;
  virtual void         setXAxis( const OdGeVector3d& vrXAxis ) = 0;
  virtual OdGeVector3d getYAxis() const = 0;
  virtual void         setYAxis( const OdGeVector3d& vrYAxis ) = 0;
  virtual OdGePoint3d  getStartOrigin() const = 0;
  virtual void         setStartOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual OdGePoint3d  getEndOrigin() const = 0;
  virtual void         setEndOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual double       getStartLength() const = 0;
  virtual void         setStartLength( double dLength ) = 0;
  virtual double       getStartWidth() const = 0;
  virtual void         setStartWidth( double dWidth ) = 0;
  virtual double       getEndLength() const = 0;
  virtual void         setEndLength( double dLength ) = 0;
  virtual double       getEndWidth() const = 0;
  virtual void         setEndWidth( double dWidth ) = 0;
  virtual bool         getSolidFlag() const = 0;
  virtual void         setSolidFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgSlabSolidGeometryCacheAction> OdDgSlabSolidGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgSphereGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgSphereGeometryCacheAction );

  virtual OdGeVector3d getXAxis() const = 0;
  virtual void         setXAxis( const OdGeVector3d& vrRef ) = 0;
  virtual OdGeVector3d getYAxis() const = 0;
  virtual void         setYAxis( const OdGeVector3d& vrYRef ) = 0;
  virtual OdGePoint3d  getOrigin() const = 0;
  virtual void         setOrigin( const OdGePoint3d& ptOrigin ) = 0;
  virtual double       getPrimaryRadius() const = 0;
  virtual void         setPrimaryRadius( double dPrimaryRadius ) = 0;
  virtual double       getSecondaryRadius() const = 0;
  virtual void         setSecondaryRadius( double dSecondaryRadius ) = 0;
  virtual double       getStartAngle() const = 0;
  virtual void         setStartAngle( double dAngle ) = 0;
  virtual double       getSweepAngle() const = 0;
  virtual void         setSweepAngle( double dAngle ) = 0;
};

typedef OdSmartPtr<OdDgSphereGeometryCacheAction> OdDgSphereGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTorusGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTorusGeometryCacheAction );

  virtual OdGeVector3d getXAxis() const = 0;
  virtual void         setXAxis( const OdGeVector3d& vrRef ) = 0;

  virtual OdGeVector3d getYAxis() const = 0;
  virtual void         setYAxis( const OdGeVector3d& vrYRef ) = 0;

  virtual OdGePoint3d  getOrigin() const = 0;
  virtual void         setOrigin( const OdGePoint3d& ptOrigin ) = 0;

  virtual double       getPrimaryRadius() const = 0;
  virtual void         setPrimaryRadius( double dPrimaryRadius ) = 0;

  virtual double       getSecondaryRadius() const = 0;
  virtual void         setSecondaryRadius( double dSecondaryRadius ) = 0;

  virtual double       getSweepAngle() const = 0;
  virtual void         setSweepAngle( double dAngle ) = 0;

  virtual bool         getSolidFlag() const = 0;
  virtual void         setSolidFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgTorusGeometryCacheAction> OdDgTorusGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBeginComplexGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBeginComplexGeometryCacheAction );

  virtual OdUInt8 getActionSubtype() const = 0;
  virtual void    setActionSubtype( OdUInt8 uSubType ) = 0;
};

typedef OdSmartPtr<OdDgBeginComplexGeometryCacheAction> OdDgBeginComplexGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBeginComplexStringGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBeginComplexStringGeometryCacheAction );
};

typedef OdSmartPtr<OdDgBeginComplexStringGeometryCacheAction> OdDgBeginComplexStringGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgEndComplexGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgEndComplexGeometryCacheAction );
};

typedef OdSmartPtr<OdDgEndComplexGeometryCacheAction> OdDgEndComplexGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgMarkHoleGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgMarkHoleGeometryCacheAction );
};

typedef OdSmartPtr<OdDgMarkHoleGeometryCacheAction> OdDgMarkHoleGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgSetTraitsDataGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgSetTraitsDataGeometryCacheAction );

  virtual bool getColorFlag() const = 0;
  virtual void setColorFlag( bool bSet ) = 0;
  virtual bool getFillColorFlag() const = 0;
  virtual void setFillColorFlag( bool bSet ) = 0;
  virtual bool getLineWeightFlag() const = 0;
  virtual void setLineWeightFlag( bool bSet ) = 0;
  virtual bool getFillModeFlag() const = 0;
  virtual void setFillModeFlag( bool bSet ) = 0;
  virtual bool getLineTypeFlag() const = 0;
  virtual void setLineTypeFlag( bool bSet ) = 0;
  virtual bool getTransparencyFlag() const = 0;
  virtual void setTransparencyFlag( bool bSet ) = 0;
  virtual bool getMaterialIdFlag() const = 0;
  virtual void setMaterialIdFlag( bool bSet ) = 0;
  virtual bool getAlternativeRGBColorFlag() const = 0;
  virtual void setAlternativeRGBColorFlag( bool bSet ) = 0;
  virtual bool getSubitemIndexFlag() const = 0;
  virtual void setSubitemIndexFlag( bool bSet ) = 0; 
  virtual bool getLineStyleModFlag() const = 0;
  virtual void setLineStyleModFlag( bool bSet ) = 0;

  virtual OdUInt32 getColorIndex() const = 0;
  virtual void     setColorIndex( OdUInt32 uColorIndex ) = 0;
  virtual OdUInt32 getFillColorIndex() const = 0;
  virtual void     setFillColorIndex( OdUInt32 uColorIndex ) = 0;
  virtual OdUInt32 getLineWeight() const = 0;
  virtual void     setLineWeight( OdUInt32 uLineWeight ) = 0;
  /** \details
    Operate with fill mode.
    uFillMode == 0:      no fill into Wireframe, use traits color as fill color.
    uFillMode == 1 or 2: use fill into Wireframe, use traits fill color as fill color.
    uFillMode == 3:      no fill and edges into Wireframe, use traits fill color as fill color.
    uFillMode >  4:      no fill into Wireframe, use traits fill color as fill color.
  */
  virtual OdUInt32 getFillMode() const = 0;
  virtual void     setFillMode( OdUInt32 uFillMode ) = 0;
  virtual OdUInt32 getLineType() const = 0;
  virtual void     setLineType( OdUInt32 uLineType ) = 0;
  virtual double   getTransparency() const = 0;
  virtual void     setTransparency( double dTransparency ) = 0;
  virtual OdUInt64 getMateralId() const = 0;
  virtual void     setMaterialId( OdUInt64 uMaterialId ) = 0;
  virtual ODCOLORREF getAlternativeRGBColor() const = 0;
  virtual void     setAlternativeRGBColor( const ODCOLORREF& uRGBColor ) = 0;
  virtual OdUInt32 getSubitemIndex() const = 0;
  virtual void     setSubitemIndex( OdUInt32 uSubitemIndex ) = 0;
  virtual OdGiDgLinetypeModifiers getLineStyleModifiers() const = 0;
  virtual void     setLineStyleModifiers( const OdGiDgLinetypeModifiers& lsMod ) = 0;
  virtual double   getLineStyleScale() const = 0;
  virtual void     setLineStyleScale( double dScale ) = 0;
};

typedef OdSmartPtr<OdDgSetTraitsDataGeometryCacheAction> OdDgSetTraitsDataGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgOriginAndRotationGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgOriginAndRotationGeometryCacheAction );

  virtual OdGePoint3d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint3d& ptOrigin ) = 0;

  virtual OdGeQuaternion getRotation() const = 0;
  virtual void setRotation( const OdGeQuaternion& quatRotation ) = 0;

  virtual bool getRotationFlag() const = 0;
  virtual void setRotationFlag( bool bSet ) = 0;

  virtual OdGeMatrix3d getTransformationMatrix() const = 0;
  virtual void setTransformationMatrix( const OdGeMatrix3d& matTransform ) = 0;

  virtual bool getTransformationMatrixFlag() const = 0;
  virtual void setTransformationMatrixRotationFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgOriginAndRotationGeometryCacheAction> OdDgOriginAndRotationGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgBSplineCurve3dGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgBSplineCurve3dGeometryCacheAction );

  virtual bool getRationalFlag() const = 0;
  virtual void setRationalFlag( bool bSet ) = 0;

  virtual bool getClosedFlag() const = 0;
  virtual void setClosedFlag( bool bSet ) = 0;

  virtual OdUInt32 getOrder() const = 0;
  virtual void setOrder( OdUInt32 uOrder ) = 0;

  virtual OdGePoint3dArray getControlPoints() const = 0;
  virtual void setControlPoints( const OdGePoint3dArray& arrCtrlPts ) = 0;

  virtual OdGeKnotVector getKnots() const = 0;
  virtual void setKnots( const OdGeKnotVector& vrKnots ) = 0;

  virtual OdDoubleArray getWeights() const = 0;
  virtual void setWeights( const OdDoubleArray& arrWeights ) = 0;
};

typedef OdSmartPtr<OdDgBSplineCurve3dGeometryCacheAction> OdDgBSplineCurve3dGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgByLevelOrGradientDataGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgByLevelOrGradientDataGeometryCacheAction );

  virtual bool getUseGradientDataFlag() const = 0;
  virtual void setUseGradientDataFlag( bool bSet ) = 0;

  virtual bool getUseByLevelDataFlag() const = 0;
  virtual void setUseByLevelDataFlag( bool bSet ) = 0;

  virtual bool getColorByLevelFlag() const = 0;
  virtual void setColorByLevelFlag( bool bSet ) = 0;

  virtual bool getFillColorByLevelFlag() const = 0;
  virtual void setFillColorByLevelFlag( bool bSet ) = 0;

  virtual bool getLineTypeByLevelFlag() const = 0;
  virtual void setLineTypeByLevelFlag( bool bSet ) = 0;

  virtual bool getLineWeightByLevelFlag() const = 0;
  virtual void setLineWeightByLevelFlag( bool bSet ) = 0;

  virtual OdDgGradientFill getGradientData() const = 0;
  virtual void             setGradientData( const OdDgGradientFill& gradData ) = 0;

  virtual bool getEndOfUseGradientDataFlag() const = 0;
  virtual void setEndOfUseGradientDataFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgByLevelOrGradientDataGeometryCacheAction> OdDgByLevelOrGradientDataGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTextGeometryCacheAction : public OdDgGeometryCacheAction
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTextGeometryCacheAction );

  virtual OdString getText() const = 0;
  virtual void setText( const OdString& strText ) = 0;

  virtual OdGePoint3d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint3d& ptOrigin ) = 0;

  virtual OdGeQuaternion getRotation() const = 0;
  virtual void setRotation( const OdGeQuaternion& quatRotation ) = 0;

  virtual double getWidthMultiplier() const = 0;
  virtual void   setWidthMultiplier( double dWidth ) = 0;

  virtual double getHeightMultiplier() const = 0;
  virtual void   setHeightMultiplier( double dHeight ) = 0;

  virtual OdUInt32 getTextColor() const = 0;
  virtual void   setTextColor( OdUInt32 uColor ) = 0;

  virtual OdUInt32 getFont() const = 0;
  virtual void   setFont( OdUInt32 uFont ) = 0;

  virtual OdUInt32 getFontFlags() const = 0;
  virtual void   setFontFlags( OdUInt32 uFlags ) = 0;

  virtual bool   getBigFontFlag() const      = 0;
  virtual void   setBigFontFlag( bool bSet ) = 0;
  virtual bool   getIntercharSpacingFlag() const      = 0;
  virtual void   setIntercharSpacingFlag( bool bSet ) = 0;
  virtual bool   getSlantAngleFlag() const      = 0;
  virtual void   setSlantAngleFlag( bool bSet ) = 0;
  virtual bool   getLineWeightFlag() const      = 0;
  virtual void   setLineWeightFlag( bool bSet ) = 0;
  virtual bool   getUnderlineColorFlag() const      = 0;
  virtual void   setUnderlineColorFlag( bool bSet ) = 0;
  virtual bool   getUnderlineLineTypeFlag() const      = 0;
  virtual void   setUnderlineLineTypeFlag( bool bSet ) = 0;
  virtual bool   getUnderlineLineWeightFlag() const      = 0;
  virtual void   setUnderlineLineWeightFlag( bool bSet ) = 0;
  virtual bool   getUnderlineOffsetFlag() const      = 0;
  virtual void   setUnderlineOffsetFlag( bool bSet ) = 0;
  virtual bool   getOverlineColorFlag() const      = 0;
  virtual void   setOverlineColorFlag( bool bSet ) = 0;
  virtual bool   getOverlineLineTypeFlag() const      = 0;
  virtual void   setOverlineLineTypeFlag( bool bSet ) = 0;
  virtual bool   getOverlineLineWeightFlag() const      = 0;
  virtual void   setOverlineLineWeightFlag( bool bSet ) = 0;
  virtual bool   getOverlineOffsetFlag() const      = 0;
  virtual void   setOverlineOffsetFlag( bool bSet ) = 0;
  virtual bool   getBackgroundColorFlag() const      = 0;
  virtual void   setBackgroundColorFlag( bool bSet ) = 0;
  virtual bool   getBackgroundFillColorFlag() const      = 0;
  virtual void   setBackgroundFillColorFlag( bool bSet ) = 0;
  virtual bool   getBackgroundLineTypeFlag() const      = 0;
  virtual void   setBackgroundLineTypeFlag( bool bSet ) = 0;
  virtual bool   getBackgroundLineWeightFlag() const      = 0;
  virtual void   setBackgroundLineWeightFlag( bool bSet ) = 0;
  virtual bool   getBackgroundOffsetFlag() const      = 0;
  virtual void   setBackgroundOffsetFlag( bool bSet ) = 0;
  virtual bool   getOffsetFlag() const      = 0;
  virtual void   setOffsetFlag( bool bSet ) = 0;
  virtual bool   getFixedSpacingTypeFlag() const      = 0;
  virtual void   setFixedSpacingTypeFlag( bool bSet ) = 0;

  virtual OdUInt32   getBigFont() const      = 0;
  virtual void   setBigFont( OdUInt32 uFont ) = 0;
  virtual double   getIntercharSpacing() const      = 0;
  virtual void   setIntercharSpacing( double dSpacing ) = 0;
  virtual double   getSlantAngle() const      = 0;
  virtual void   setSlantAngle( double dAngle) = 0;
  virtual OdUInt32   getLineWeight() const      = 0;
  virtual void   setLineWeight( OdUInt32 uLineWeight ) = 0;
  virtual OdUInt32   getUnderlineColor() const      = 0;
  virtual void   setUnderlineColor( OdUInt32 uColor ) = 0;
  virtual OdUInt32   getUnderlineLineType() const      = 0;
  virtual void   setUnderlineLineType( OdUInt32 uLineType ) = 0;
  virtual OdUInt32   getUnderlineLineWeight() const      = 0;
  virtual void   setUnderlineLineWeight( OdUInt32 uLineWeight ) = 0;
  virtual double   getUnderlineOffset() const      = 0;
  virtual void   setUnderlineOffset( double dOffset ) = 0;
  virtual OdUInt32   getOverlineColor() const      = 0;
  virtual void   setOverlineColor( OdUInt32 uColor ) = 0;
  virtual OdUInt32   getOverlineLineType() const      = 0;
  virtual void   setOverlineLineType( OdUInt32 uLineType ) = 0;
  virtual OdUInt32   getOverlineLineWeight() const      = 0;
  virtual void   setOverlineLineWeight( OdUInt32 uLineWeight ) = 0;
  virtual double   getOverlineOffset() const      = 0;
  virtual void   setOverlineOffset( double dOffset ) = 0;
  virtual OdUInt32   getBackgroundColor() const      = 0;
  virtual void   setBackgroundColor( OdUInt32 uColor ) = 0;
  virtual OdUInt32   getBackgroundFillColor() const      = 0;
  virtual void   setBackgroundFillColor( OdUInt32 uColor ) = 0;
  virtual OdUInt32   getBackgroundLineType() const      = 0;
  virtual void   setBackgroundLineType( OdUInt32 uLineType ) = 0;
  virtual OdUInt32   getBackgroundLineWeight() const      = 0;
  virtual void   setBackgroundLineWeight( OdUInt32 uLineWeight ) = 0;
  virtual OdGePoint2d   getBackgroundOffset() const      = 0;
  virtual void   setBackgroundOffset( const OdGePoint2d& ptOffset ) = 0;
  virtual OdGePoint2d   getOffset() const      = 0;
  virtual void   setOffset( const OdGePoint2d& ptOffset ) = 0;
  virtual bool   getFixedSpacingType() const      = 0;
  virtual void   setFixedSpacingType( bool bSet ) = 0;

  virtual bool   getBackgroundFlag() const      = 0;
  virtual void   setBackgroundFlag( bool bSet ) = 0;
  virtual bool   getUnderlineFlag() const      = 0;
  virtual void   setUnderlineFlag( bool bSet ) = 0;
  virtual bool   getUnderlineStyleFlag() const      = 0;
  virtual void   setUnderlineStyleFlag( bool bSet ) = 0;
  virtual bool   getOverlineFlag() const      = 0;
  virtual void   setOverlineFlag( bool bSet ) = 0;
  virtual bool   getOverlineStyleFlag() const      = 0;
  virtual void   setOverlineStyleFlag( bool bSet ) = 0;
  virtual bool   getBoldFlag() const      = 0;
  virtual void   setBoldFlag( bool bSet ) = 0;
  virtual bool   getItalicFlag() const      = 0;
  virtual void   setItalicFlag( bool bSet ) = 0;
  virtual bool   getUseIntercharSpacingFlag() const      = 0;
  virtual void   setUseIntercharSpacingFlag( bool bSet ) = 0;
  virtual bool   getSubscriptFlag() const      = 0;
  virtual void   setSubscriptFlag( bool bSet ) = 0;
  virtual bool   getSuperscriptFlag() const      = 0;
  virtual void   setSuperscriptFlag( bool bSet ) = 0;
  virtual bool   getVerticalFlag() const      = 0;
  virtual void   setVerticalFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgTextGeometryCacheAction> OdDgTextGeometryCacheActionPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgGeometryCacheXAttribute : public OdDgXAttribute
{
public:

  enum { kType = 0x57180000 };

public:

  ODRX_DECLARE_MEMBERS(OdDgGeometryCacheXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  // Read only mode at the moment
/** \details
    Bad name of function. The function contains several flags of element rendering.
      0x0100 - Change fill mode of element with render mode. If the flag is not set, then 
    polygon primitive without fill will be drawn as polyline for smooth modes.
  */
  virtual OdUInt16 getTypeValue() const = 0;

  virtual OdUInt32 getActionCount() const = 0;
  virtual OdDgGeometryCacheActionPtr getAction( OdUInt32 uIndex ) = 0;
  virtual void setAction( OdUInt32 uIndex, const OdDgGeometryCacheActionPtr& pAction ) = 0;
  virtual void addAction( const OdDgGeometryCacheActionPtr& pAction ) = 0;
  virtual void deleteAction( OdUInt32 uIndex ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgGeometryCacheXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgGeometryCacheXAttribute> OdDgGeometryCacheXAttributePtr;

//------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/

class OdDgSchemaDescriptionItem;
typedef OdSmartPtr<OdDgSchemaDescriptionItem> OdDgSchemaDescriptionItemPtr;

class TG_EXPORT OdDgSchemaDescriptionItem : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSchemaDescriptionItem);

  enum OdDgSchemaDescriptionItemType
  {
      kUnknown                    = 0x00
    , kTableSchema                = 0x01
    , kCellParamSchema            = 0x02
  };
  enum Type { /*kType = 0xEC5C0000*/    // "Type" as enum name added for swig
#if defined(OD_SWIGCSHARP)
    kType          = (uint)0xEC5C0000
#elif defined(OD_SWIGJAVA)
    kType          = (long)0xEC5C0000
#else
    kType          = (OdInt32)0xEC5C0000
#endif
       };

  virtual OdDgSchemaDescriptionItemType getType() const = 0;

  virtual void readData( const OdBinaryData& binData ) = 0;
  virtual void writeData( OdBinaryData& binData ) const = 0;

  static OdDgSchemaDescriptionItemPtr createObject( OdDgSchemaDescriptionItemType type );
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgProxySchemaDescriptionItem : public OdDgSchemaDescriptionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgProxySchemaDescriptionItem );

  virtual OdUInt32 getTypeValue() const = 0;
  virtual void     setTypeValue( OdUInt32 uType ) = 0;

  virtual OdBinaryData getProxyData() const = 0;
  virtual void         setProxyData( const OdBinaryData& binData ) = 0;
};

typedef OdSmartPtr<OdDgProxySchemaDescriptionItem> OdDgProxySchemaDescriptionItemPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTableSchemaDescriptionItem : public OdDgSchemaDescriptionItem
{
public:
  enum OdDgTableSchemaDescriptionType
  {
    kSchemaItem       = 1,
    kSchemaFlags      = 2,
    kSchemaGroup      = 4
  };

  enum OdDgTableSchemaDescriptionItemType
  {
    kGroup            = 0,
    kExpressionDesc   = 1,
    kBoolean          = 2,
    kDateTime         = 3,
    kDouble           = 4,
    kUInt32           = 5,
    kPoint3d          = 8,
    kString           = 9
  };

public:
  ODRX_DECLARE_MEMBERS( OdDgTableSchemaDescriptionItem );

  virtual OdUInt32 getParentItemIndex() const = 0;
  virtual void     setParentItemIndex( OdUInt32 uItemIndex ) = 0;

  virtual OdDgTableSchemaDescriptionType getSchemaType() const = 0;
  virtual void     setSchemaType( const OdDgTableSchemaDescriptionType& uDataType ) = 0;

  // For item description

  virtual OdDgTableSchemaDescriptionItemType getItemDataType() const = 0;
  virtual void     setItemDataType( const OdDgTableSchemaDescriptionItemType& uDataType ) = 0;

  virtual OdUInt32 getItemDataOffset() const = 0;
  virtual void     setItemDataOffset( OdUInt32 uDataOffset ) = 0;

  virtual OdUInt64 getItemBitwiseAndData() const = 0;
  virtual void     setItemBitwiseAndData( OdUInt64 uBitwiseAnd ) = 0;

  virtual OdString getItemName() const = 0;
  virtual void     setItemName( const OdString& strName ) = 0;

  virtual bool     getProxyDataFlag() const = 0;
  virtual void     setProxyDataFlag( bool bSet ) = 0;

  virtual OdBinaryData getProxyData() const = 0;
  virtual void setProxyData( const OdBinaryData& binData ) = 0;

  virtual bool     getItemTypeDataFlag() const = 0;
  virtual void     setItemTypeDataFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgTableSchemaDescriptionItem> OdDgTableSchemaDescriptionItemPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgCellParamSchemaDescriptionItem : public OdDgSchemaDescriptionItem
{
public:
  ODRX_DECLARE_MEMBERS( OdDgCellParamSchemaDescriptionItem );

  virtual OdUInt32 getTypeValue() const = 0;
  virtual void     setTypeValue( OdUInt32 uTypeValue ) = 0;

  virtual bool     getReadOnlyFlag() const = 0;
  virtual void     setReadOnlyFlag( bool bSet ) = 0;

  virtual bool     getVisibleFlag() const = 0;
  virtual void     setVisibleFlag( bool bSet ) = 0;

  virtual bool     getDefinitionScopeFlag() const = 0; // false - Instance, true - Definition
  virtual void     setDefinitionScopeFlag( bool bSet ) = 0;

  virtual bool     getExpressionFlag() const = 0;
  virtual void     setExpressionFlag( bool bSet ) = 0;

  virtual OdString getVariableName() const = 0;
  virtual void     setVariableName( const OdString& strName ) = 0;

  virtual OdString getVariableValue() const = 0;
  virtual void     setVariableValue( const OdString& strValue ) = 0;

  virtual OdString getVariableDisplayName() const = 0;
  virtual void     setVariableDisplayName( const OdString& strDisplayName ) = 0;

  virtual OdString getVariableTypeDescription() const = 0;
  virtual void     setVariableTypeDescription( const OdString& strTypeDescription ) = 0;

  virtual OdString getUnitsDescription() const = 0;
  virtual void     setUnitsTypeDescription( const OdString& strUnitsDescription ) = 0;
};

typedef OdSmartPtr<OdDgCellParamSchemaDescriptionItem> OdDgCellParamSchemaDescriptionItemPtr;

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgSchemaDescriptionXAttribute : public OdDgXAttribute
{
public:

  enum Type { /*kType = 0xEC5C0000*/    // "Type" as enum name added for swig
#if defined(OD_SWIGCSHARP)
    kType          = (uint)0xEC5C0000
#elif defined(OD_SWIGJAVA)
    kType          = (long)0xEC5C0000
#else
    kType          = (OdInt32)0xEC5C0000
#endif
       };


public:

  ODRX_DECLARE_MEMBERS(OdDgSchemaDescriptionXAttribute);

  virtual OdUInt16 getPrimaryIndex() const = 0;
  virtual void     setPrimaryIndex( OdUInt16 uPrimaryIndex ) = 0;

  virtual OdUInt16 getSecondaryIndex() const = 0;
  virtual void     setSecondaryIndex( OdUInt16 uSecondaryIndex ) = 0;

  virtual OdUInt32 getVersion() const = 0;
  virtual void     setVersion( OdUInt32 uVersion ) = 0;

  virtual OdUInt32 getSubVersion() const = 0;
  virtual void     setSubVersion( OdUInt32 uSubVersion ) = 0;

  virtual OdUInt32 getDataFlags() const = 0;
  virtual void     setDataFlags( OdUInt32 uDataFlags ) = 0;

  virtual OdDgSchemaDescriptionItem::OdDgSchemaDescriptionItemType getSchemaType() const = 0;
  virtual OdDgSchemaDescriptionItemPtr getSchemaItem() const = 0;
  virtual void setSchemaItem( const OdDgSchemaDescriptionItemPtr pItem ) = 0;

  // X-attr override

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgSchemaDescriptionXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgSchemaDescriptionXAttribute> OdDgSchemaDescriptionXAttributePtr;

//------------------------------------------------------------------------------------

/** \details

  Corresponding C++ library: DG_Db

  <group OdDg_Classes>
*/
class OdDgTableElementSchema;
typedef OdSmartPtr<OdDgTableElementSchema> OdDgTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTableElementSchema : public OdRxObject
{
public:

  enum OdDgTableElementSchemaType
  {
      kTableData                  = 0x00
    , kCellData                   = 0x01
    , kColumnData                 = 0x02
    , kEdgeRun                    = 0x03
    , kFill                       = 0x04
    , kMergeData                  = 0x05
    , kRowData                    = 0x06
    , kSymbology                  = 0x07
    , kPrototypeDescription       = 0x08
    , kSchemaPrototype            = 0x09
    , kHeaderPrototype            = 0x0A
    , kSnippedText                = 0x0B
    , kPCMCell                    = 0x0C
    , kPCMCellDef                 = 0x0D
    , kPrototypeBasedSchema       = kPCMCellDef + 1
    , kProxySchema                = kPrototypeBasedSchema + 1
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgTableElementSchema);

  virtual OdDgTableElementSchemaType getType() const = 0;

  virtual OdUInt32 getVersion() const = 0;
  virtual void     setVersion( OdUInt32 uVersion ) = 0;

  virtual OdUInt32 getSubVersion() const = 0;
  virtual void     setSubVersion( OdUInt32 uSubVersion ) = 0;

  virtual OdBinaryData getData() const = 0;
  virtual void         setData( const OdBinaryData& binData ) = 0;

  static OdDgTableElementSchemaPtr createObject( OdDgTableElementSchemaType type, const OdBinaryData& binData );
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgItemGroupVariable
{
public: 
  OdDgItemGroupVariable();
  ~OdDgItemGroupVariable();

  OdUInt32 getDefaultFlags() const;
  void     setDefaultFlags( OdUInt32 uFlags );

  OdUInt32Array  getGroupItemIdArray() const;
  void           setGroupItemIdArray( const OdUInt32Array& arrIds );

private:
  OdUInt32          m_uFlags;
  OdUInt32Array     m_arrXAttrId;
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgExpressionGroupVariable
{
public: 
  OdDgExpressionGroupVariable();
  ~OdDgExpressionGroupVariable();

  OdUInt32 getDefaultFlags() const;
  void     setDefaultFlags( OdUInt32 uFlags );

  OdUInt32                  getExpressionCount() const;
  OdDgExpressionVariablePtr getExpression( OdUInt32 uIndex );
  OdDgExpressionVariablePtr getExpression( const OdString& strExpressionName );
  void                      setExpression( OdUInt32 uIndex, const OdDgExpressionVariablePtr& pExpression );
  void                      setExpression( const OdDgExpressionVariablePtr& pExpression );
  void                      addExpression( const OdDgExpressionVariablePtr& pExpression );
  void                      deleteExpression( OdUInt32 uIndex );

private:
  OdUInt32                            m_uFlags;
  OdArray<OdDgExpressionVariablePtr>  m_arrExpressionPtr;
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgSchemaItemVariant
{
public:
  enum OdDgSchemaItemVariantType
  {
    kUnknownVar         = 0,
    kStringVar          = 1,
    kDoubleVar          = 2,
    kInt32Var           = 3,
    kBoolVar            = 4,
    kPoint3dVar         = 5,
    kDateTimeVar        = 6,
    kItemGroupVar       = 7,
    kExpressionGroupVar = 8,
    kStringArrayVar     = 9,
    kDoubleArrayVar     = 10,
    kInt32ArrayVar      = 11,
    kBoolArrayVar       = 12,
    kPoint3dArrayVar    = 13,
    kDateTimeArrayVar   = 14,
    kEmptyVar           = 15,
  };

public:
  OdDgSchemaItemVariant();
  OdDgSchemaItemVariant( const OdString& strName );
  OdDgSchemaItemVariant( const OdString& strName, const OdBinaryData& unknownValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdString& strValue );
  OdDgSchemaItemVariant( const OdString& strName, double dValue );
  OdDgSchemaItemVariant( const OdString& strName, OdInt32 iValue );
  OdDgSchemaItemVariant( const OdString& strName, bool bValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdGePoint3d& ptValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdTimeStamp& dateTime );
  OdDgSchemaItemVariant( const OdString& strName, const OdDgItemGroupVariable& itemGroupValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdDgExpressionGroupVariable& expressionGroupValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdArray<OdString>& strArrValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdArray<double>& dArrValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdArray<OdInt32>& iArrValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdArray<bool>& bArrValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdArray<OdGePoint3d>& ptArrValue );
  OdDgSchemaItemVariant( const OdString& strName, const OdArray<OdTimeStamp>& arrDateTime );

  OdDgSchemaItemVariantType getType() const;

  OdString getName() const;
  void     setName( const OdString& strName );

  OdString getStringValue() const;
  void     setStringValue( const OdString& strValue );

  double   getDoubleValue() const;
  void     setDoubleValue( double dValue );

  OdInt32  getInt32Value() const;
  void     setInt32Value( OdInt32 iValue );

  bool     getBoolValue() const;
  void     setBoolValue( bool bValue );

  void     setEmptyValue();

  OdGePoint3d getPoint3dValue() const;
  void        setPoint3dValue( const OdGePoint3d& ptValue );

  OdTimeStamp getDateTimeValue() const;
  void        setDateTimeValue( const OdTimeStamp& dateTimeValue );

  OdArray<OdString> getStringArrayValue() const;
  void        setStringArrayValue( const OdArray<OdString>& arrValue );

  OdArray<double> getDoubleArrayValue() const;
  void        setDoubleArrayValue( const OdArray<double>& arrValue );

  OdArray<OdInt32> getInt32ArrayValue() const;
  void        setInt32ArrayValue( const OdArray<OdInt32>& arrValue );

  OdArray<bool> getBoolArrayValue() const;
  void        setBoolArrayValue( const OdArray<bool>& arrValue );

  OdArray<OdGePoint3d> getPoint3dArrayValue() const;
  void        setPoint3dArrayValue( const OdArray<OdGePoint3d>& arrValue );

  OdArray<OdTimeStamp> getDateTimeArrayValue() const;
  void        setDateTimeArrayValue( const OdArray<OdTimeStamp>& arrValue );

  OdDgItemGroupVariable getItemGroupValue() const;
  void        setItemGroupValue( const OdDgItemGroupVariable& itemGroupValue );

  OdDgExpressionGroupVariable getExpressionGroupValue() const;
  void        setExpressionGroupValue( const OdDgExpressionGroupVariable& expressionGroupValue );

  OdBinaryData getUnknownValue() const;
  void         setUnknownValue( const OdBinaryData& binValue );

private:

  OdString                    m_strName;
  OdDgSchemaItemVariantType   m_uType;
  OdString                    m_strValue;
  double                      m_dValue;
  OdInt32                     m_int32Value;
  bool                        m_bValue;
  OdGePoint3d                 m_pt3dValue;
  OdTimeStamp                 m_stampValue;
  OdArray<OdString>           m_arrStrValue;
  OdArray<double>             m_arrDoubleValue;
  OdArray<OdInt32>            m_arrInt32Value;
  OdArray<bool>               m_arrBoolValue;
  OdArray<OdGePoint3d>        m_arrPt3dValue;
  OdArray<OdTimeStamp>        m_arrStampValue;
  OdDgItemGroupVariable       m_itemGroupValue;
  OdDgExpressionGroupVariable m_expressionGroupValue;
  OdBinaryData                m_binUnknownValue;
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPrototypeBasedTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPrototypeBasedTableElementSchema );

  virtual OdUInt32 getPrototypeIndex() const = 0;
  virtual void     setPrototypeIndex( OdUInt32 uIndex ) = 0;

  virtual OdUInt32 getSchemaIndex() const = 0;
  virtual void     setSchemaIndex( OdUInt32 uIndex ) = 0;

  virtual bool     loadFromPrototype( const OdDgTablePrototypeElementPtr& pPrototype ) = 0;

  virtual OdBinaryData getPrototypeHeader() const = 0;
  virtual void         setPrototypeHeader( const OdBinaryData& headerData ) = 0;

  virtual OdUInt32 getPrototypeFlags() const = 0;
  virtual void     setPrototypeFlags( OdUInt32 uFalgs ) = 0;

  virtual OdUInt32 getSchemaItemCount() const = 0;
  virtual OdDgSchemaItemVariant getSchemaItem( OdUInt32 uIndex ) const = 0;
  virtual void     setSchemaItem( OdUInt32 uIndex, const OdDgSchemaItemVariant& item ) = 0;
  virtual void     addSchemaItem( const OdDgSchemaItemVariant& item ) = 0;
  virtual void     deleteSchemaItem( OdUInt32 uIndex ) = 0;
  virtual OdDgSchemaItemVariant getSchemaItem( const OdString& strItemName ) const = 0;
  virtual void     setSchemaItem( const OdDgSchemaItemVariant& item ) = 0;
};

typedef OdSmartPtr<OdDgPrototypeBasedTableElementSchema> OdDgPrototypeBasedTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgProxyTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgProxyTableElementSchema );

  virtual void setType( const OdDgTableElementSchema::OdDgTableElementSchemaType& uType ) = 0;
};

typedef OdSmartPtr<OdDgProxyTableElementSchema> OdDgProxyTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTableDataTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTableDataTableElementSchema );

  virtual bool getAnnotationScaleFlag() const = 0;
  virtual void setAnnotationScaleFlag( bool bSet ) = 0;

  virtual double getAnnotationScale() const = 0;
  virtual void setAnnotationScale( double dScale ) = 0;

  virtual OdUInt32 getRowCount() const = 0;
  virtual void setRowCount( OdUInt32 nRows ) = 0;

  virtual OdUInt32 getColumnCount() const = 0;
  virtual void setColumnCount( OdUInt32 nColumns ) = 0;

  virtual OdUInt32 getTableBodyTextStyle() const = 0;
  virtual void setTableBodyTextStyle( OdUInt32 uTextStyleEntryId ) = 0;

  virtual OdGePoint3d getOrigin() const = 0;
  virtual void setOrigin( const OdGePoint3d& ptOrigin ) = 0;

  virtual OdGeMatrix3d getTransformation() const = 0;
  virtual void setTransformation( const OdGeMatrix3d& matTransform ) = 0;

  virtual OdUInt32 getTitleRowCount() const = 0;
  virtual void setTitleRowCount( OdUInt32 nRows ) = 0;

  virtual OdUInt32 getHeaderRowCount() const = 0;
  virtual void setHeaderRowCount( OdUInt32 nRows ) = 0;

  virtual OdUInt32 getFooterRowCount() const = 0;
  virtual void setFooterRowCount( OdUInt32 nRows ) = 0;

  virtual OdUInt32 getFirstColumnCount() const = 0;
  virtual void setFirstColumnCount( OdUInt32 nColumns ) = 0;

  virtual OdUInt32 getLastColumnCount() const = 0;
  virtual void setLastColumnCount( OdUInt32 nColumns ) = 0;

  virtual OdDgTableElement::OdDgTableElementBreakType getBreakType() const = 0;
  virtual void setBreakType( const OdDgTableElement::OdDgTableElementBreakType& uBreakType ) = 0;

  virtual OdDgTableElement::OdDgTableElementSubTablePosition getSubTablePosition() const = 0;
  virtual void setSubTablePosition( const OdDgTableElement::OdDgTableElementSubTablePosition& uPosition ) = 0;

  virtual double getBreakHeight() const = 0;
  virtual void setBreakHeight( double dHeight ) = 0;

  virtual double getSubTableSpacing() const = 0;
  virtual void setSubTableSpacing( double dSubTableSpacing ) = 0;

  virtual bool getRepeatHeadersFlag() const = 0;
  virtual void setRepeatHeadersFlag( bool bSet ) = 0;

  virtual bool getRepeatFootersFlag() const = 0;
  virtual void setRepeatFootersFlag( bool bSet ) = 0;

  virtual double getDefaultCellWidth() const = 0;
  virtual void setDefaultCellWidth( double dWidth ) = 0;

  virtual double getDefaultCellHeight() const = 0;
  virtual void setDefaultCellHeight( double dHeight ) = 0;

  virtual double getCellTopMargin() const = 0;
  virtual void setCellTopMargin( double dMargin ) = 0;

  virtual double getCellBottomMargin() const = 0;
  virtual void setCellBottomMargin( double dMargin ) = 0;

  virtual double getCellLeftMargin() const = 0;
  virtual void setCellLeftMargin( double dMargin ) = 0;

  virtual double getCellRightMargin() const = 0;
  virtual void setCellRightMargin( double dMargin ) = 0;

  virtual bool getOddFillStyleFlag() const = 0;
  virtual void setOddFillStyleFlag( bool bSet ) = 0;

  virtual bool getEvenFillStyleFlag() const = 0;
  virtual void setEvenFillStyleFlag( bool bSet ) = 0;

  virtual OdUInt32 getOddFillStyle() const = 0;
  virtual void setOddFillStyle( OdUInt32 uFillStyleId ) = 0;

  virtual OdUInt32 getEvenFillStyle() const = 0;
  virtual void setEvenFillStyle( OdUInt32 uFillStyleId ) = 0;

  virtual OdUInt32 getTitleRowsTextStyle() const = 0;
  virtual void setTitleRowsTextStyle( OdUInt32 uTextStyleEntryId ) = 0;

  virtual OdUInt32 getHeaderRowsTextStyle() const = 0;
  virtual void setHeaderRowsTextStyle( OdUInt32 uTextStyleEntryId ) = 0;

  virtual OdUInt32 getFooterRowsTextStyle() const = 0;
  virtual void setFooterRowsTextStyle( OdUInt32 uTextStyleEntryId ) = 0;

  virtual OdUInt32 getFirstColumnsTextStyle() const = 0;
  virtual void setFirstColumnsTextStyle( OdUInt32 uTextStyleEntryId ) = 0;

  virtual OdUInt32 getLastColumnsTextStyle() const = 0;
  virtual void setLastColumnsTextStyle( OdUInt32 uTextStyleEntryId ) = 0;

  virtual double getDefaultTextSize() const = 0;
  virtual void setDefaultTextSize( double dTextSize ) = 0;

  virtual OdUInt32 getCellAlignment() const = 0;
  virtual void setCellAlignment( OdUInt32 uCellAlignment ) = 0;

  virtual OdUInt32 getDataSourceProviderId() const = 0;
  virtual void setDataSourceProviderId( OdUInt32 uProviderId ) = 0;

  virtual double getBodyTextHeight() const = 0;
  virtual void setBodyTextHeight( double dTextHeight ) = 0;

  virtual double getTitleTextHeight() const = 0;
  virtual void setTitleTextHeight( double dTextHeight ) = 0;

  virtual double getHeaderTextHeight() const = 0;
  virtual void setHeaderTextHeight( double dTextHeight ) = 0;

  virtual double getFooterTextHeight() const = 0;
  virtual void setFooterTextHeight( double dTextHeight ) = 0;

  virtual double getFirstColumnTextHeight() const = 0;
  virtual void setFirstColumnTextHeight( double dTextHeight ) = 0;

  virtual double getLastColumnTextHeight() const = 0;
  virtual void setLastColumnTextHeight( double dTextHeight ) = 0;

  virtual OdUInt32 getDefaultTextSymbology() const = 0;
  virtual void setDefaultTextSymbology( OdUInt32 uSymbologyIndex ) = 0;
};

typedef OdSmartPtr<OdDgTableDataTableElementSchema> OdDgTableDataTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgMergeDataTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgMergeDataTableElementSchema );

  virtual OdUInt32 getFirstRow() const = 0;
  virtual void     setFirstRow( OdUInt32 uRow ) = 0;

  virtual OdUInt32 getFirstColumn() const = 0;
  virtual void     setFirstColumn( OdUInt32 uColumn ) = 0;

  virtual OdUInt32 getRowsCount() const = 0;
  virtual void     setRowsCount( OdUInt32 uRowsCount ) = 0;

  virtual OdUInt32 getColumnsCount() const = 0;
  virtual void     setColumnsCount( OdUInt32 uColumnsCount ) = 0;
};

typedef OdSmartPtr<OdDgMergeDataTableElementSchema> OdDgMergeDataTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgFillTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgFillTableElementSchema );

  virtual OdUInt32 getEntryId() const = 0;
  virtual void     setEntryId( OdUInt32 uEntryId ) = 0;

  virtual OdUInt32 getFillColorIndex() const = 0;
  virtual void     setFillColorIndex( OdUInt32 uFillColorIndex ) = 0;
};

typedef OdSmartPtr<OdDgFillTableElementSchema> OdDgFillTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgSymbologyTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgSymbologyTableElementSchema );

  virtual OdUInt32 getEntryId() const = 0;
  virtual void     setEntryId( OdUInt32 uEntryId ) = 0;

  virtual OdUInt32 getColorIndex() const = 0;
  virtual void     setColorIndex( OdUInt32 uColorIndex ) = 0;

  virtual OdUInt32 getLineWeight() const = 0;
  virtual void     setLineWeight( OdUInt32 uLineWeight ) = 0;

  virtual OdUInt32 getLineType() const = 0;
  virtual void     setLineType( OdUInt32 uLineType ) = 0;

  virtual bool     getVisibilityFlag() const = 0;
  virtual void     setVisibilityFlag( bool bSet ) = 0;

  virtual bool     getDefaultColorFlag() const = 0;
  virtual void     setDefaultColorFlag( bool bSet ) = 0;

  virtual bool     getDefaultLineWeightFlag() const = 0;
  virtual void     setDefaultLineWeightFlag( bool bSet ) = 0;

  virtual bool     getDefaultLineTypeFlag() const = 0;
  virtual void     setDefaultLineTypeFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgSymbologyTableElementSchema> OdDgSymbologyTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgEdgeRunTableElementSchema : public OdDgTableElementSchema
{
public:
  enum OdDgEdgeRunSide
  {
    kTopEdge    = 0,
    kLeftEdge   = 1,
    kBottomEdge = 2,
    kRightEdge  = 3
  };
public:
  ODRX_DECLARE_MEMBERS( OdDgEdgeRunTableElementSchema );

  virtual OdDgEdgeRunSide getCellSide() const = 0;
  virtual void     setCellSide( const OdDgEdgeRunSide& uSide ) = 0;

  virtual OdUInt32 getFirstRow() const = 0;
  virtual void     setFirstRow( OdUInt32 uRow ) = 0;

  virtual OdUInt32 getFirstColumn() const = 0;
  virtual void     setFirstColumn( OdUInt32 uColumn ) = 0;

  virtual OdUInt32 getLength() const = 0;
  virtual void     setLength( OdUInt32 nColumnsOrRows ) = 0;

  virtual OdUInt32 getSymbologyIndex() const = 0;
  virtual void     setSymbologyIndex( OdUInt32 uSymbologyIndex ) = 0;

  virtual bool     getDefaultSymbologyFlag() const = 0;
  virtual void     setDefaultSymbologyFlag( bool bSet ) = 0;

  virtual bool     getFirstRowOrColumnFlag() const = 0;
  virtual void     setFirstRowOrColumnFlag( bool bSet ) = 0;
};

typedef OdSmartPtr<OdDgEdgeRunTableElementSchema> OdDgEdgeRunTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgRowTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgRowTableElementSchema );

  virtual OdUInt32 getRowIndex() const = 0;
  virtual void     setRowIndex( OdUInt32 uIndex ) = 0;

  virtual bool     getHeightLockFlag() const = 0;
  virtual void     setHeightLockFlag( bool bSet ) = 0;

  virtual double   getHeight() const = 0;
  virtual void     setHeight( double dHeight ) = 0;
};

typedef OdSmartPtr<OdDgRowTableElementSchema> OdDgRowTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgColumnTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgColumnTableElementSchema );

  virtual OdUInt32 getColumnIndex() const = 0;
  virtual void     setColumnIndex( OdUInt32 uIndex ) = 0;

  virtual bool     getWidthLockFlag() const = 0;
  virtual void     setWidthLockFlag( bool bSet ) = 0;

  virtual double   getWidth() const = 0;
  virtual void     setWidth( double dWidth ) = 0;
};

typedef OdSmartPtr<OdDgColumnTableElementSchema> OdDgColumnTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgCellTableElementSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgCellTableElementSchema );

  virtual OdUInt32 getRowIndex() const = 0;
  virtual void     setRowIndex( OdUInt32 uRowIndex ) = 0;

  virtual OdUInt32 getColumnIndex() const = 0;
  virtual void     setColumnIndex( OdUInt32 uColumnIndex ) = 0;

  virtual OdUInt32 getFillIndex() const = 0;
  virtual void     setFillIndex( OdUInt32 uFillIndex ) = 0;

  virtual OdDgTableCellElement::OdDgTableCellElementTextAlignment getTextAlignment() const = 0;
  virtual void setTextAlignment( const OdDgTableCellElement::OdDgTableCellElementTextAlignment& uAlignment ) = 0;

  virtual OdDgTableCellElement::OdDgTableCellElementTextOrientation getTextOrientation() const = 0;
  virtual void setTextOrientation( const OdDgTableCellElement::OdDgTableCellElementTextOrientation& uOrientation ) = 0;

  virtual bool     getFillFlag() const = 0;
  virtual void     setFillFlag( bool bSet ) = 0;

  virtual double   getTopMargin() const = 0;
  virtual bool     getDefaultTopMarginFlag() const = 0;
  virtual void     setTopMargin( double dMargin, bool bSetAsDefault = false ) = 0;

  virtual double   getBottomMargin() const = 0;
  virtual bool     getDefaultBottomMarginFlag() const = 0;
  virtual void     setBottomMargin( double dMargin, bool bSetAsDefault = false ) = 0;

  virtual double   getLeftMargin() const = 0;
  virtual bool     getDefaultLeftMarginFlag() const = 0;
  virtual void     setLeftMargin( double dMargin, bool bSetAsDefault = false ) = 0;

  virtual double   getRightMargin() const = 0;
  virtual bool     getDefaultRightMarginFlag() const = 0;
  virtual void     setRightMargin( double dMargin, bool bSetAsDefault = false ) = 0;

  virtual bool     hasTextData() const = 0;
  virtual void     setShowTextData( bool bSet ) = 0;

  virtual OdDgXMLNode getTextData() const = 0;
  virtual void setTextData( const OdDgXMLNode& xmlText ) = 0;
};

typedef OdSmartPtr<OdDgCellTableElementSchema> OdDgCellTableElementSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTablePrototypeDescriptionSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTablePrototypeDescriptionSchema );

  virtual OdBinaryData getPrototypeHeader() const = 0;
  virtual void         setPrototypeHeader( const OdBinaryData& headerData ) = 0;

  virtual OdUInt32 getPrototypeFlags() const = 0;
  virtual void     setPrototypeFlags( OdUInt32 uFalgs ) = 0;

  virtual OdUInt32 getPrototypeId() const = 0;
  virtual void     setPrototypeId( OdUInt32 uEntryId ) = 0;

  virtual OdString getPrototypeName() const = 0;
  virtual void     setPrototypeName( const OdString& strName ) = 0;
};

typedef OdSmartPtr<OdDgTablePrototypeDescriptionSchema> OdDgTablePrototypeDescriptionSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTablePrototypeHeaderSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTablePrototypeHeaderSchema );

  virtual OdBinaryData getPrototypeHeader() const = 0;
  virtual void         setPrototypeHeader( const OdBinaryData& headerData ) = 0;
};

typedef OdSmartPtr<OdDgTablePrototypeHeaderSchema> OdDgTablePrototypeHeaderSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTablePrototypeItemSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTablePrototypeItemSchema );

  virtual OdBinaryData getPrototypeHeader() const = 0;
  virtual void         setPrototypeHeader( const OdBinaryData& headerData ) = 0;

  virtual OdUInt32 getPrototypeFlags() const = 0;
  virtual void     setPrototypeFlags( OdUInt32 uFalgs ) = 0;

  virtual OdUInt32 getSchemaId() const = 0;
  virtual void     setSchemaId( OdUInt32 uEntryId ) = 0;

  virtual OdString getSchemaName() const = 0;
  virtual void     setSchemaName( const OdString& strName ) = 0;

  virtual OdUInt64 getDefaultSchemaFlags() const = 0;
  virtual void     setDefaultSchemaFlags( OdUInt64 uFalgs ) = 0;

  virtual OdUInt32 getSchemaItemCount() const = 0;
  virtual OdUInt32 getSchemaItemXAttributeIndex( OdUInt32 uIndex ) const = 0;
  virtual void     setSchemaItemXAttributeIndex( OdUInt32 uIndex, OdUInt32 uXAttrIndex ) = 0;
  virtual void     addSchemaItemXAttributeIndex( OdUInt32 uXAttrIndex ) = 0;
  virtual void     deleteSchemaItemXAttributeIndex( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgTablePrototypeItemSchema> OdDgTablePrototypeItemSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTableSnippedTextSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTableSnippedTextSchema );

  virtual OdBinaryData getHeader() const = 0;
  virtual void         setHeader( const OdBinaryData& headerData ) = 0;

  virtual OdUInt32 getFlags() const = 0;
  virtual void     setFlags( OdUInt32 uFalgs ) = 0;

  virtual OdString getName() const = 0;
  virtual void     setName( const OdString& strName ) = 0;

  virtual OdString getText() const = 0;
  virtual void     setText( const OdString& strText ) = 0;
};

typedef OdSmartPtr<OdDgTableSnippedTextSchema> OdDgTableSnippedTextSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPCMCellSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPCMCellSchema );

  virtual OdGeVector3d getXRotation() const = 0;
  virtual void         setXRotation( const OdGeVector3d& vrRotation ) = 0;

  virtual OdGeVector3d getYRotation() const = 0;
  virtual void         setYRotation( const OdGeVector3d& vrRotation ) = 0;

  virtual OdGeVector3d getZRotation() const = 0;
  virtual void         setZRotation( const OdGeVector3d& vrRotation ) = 0;

  virtual OdGePoint3d  getOrigin() const = 0;
  virtual void         setOrigin( const OdGePoint3d& ptOrigin ) = 0;

  virtual double       getScale() const = 0;
  virtual void         setScale( double dScale ) = 0;

  virtual OdString     getVarSetName() const = 0;
  virtual void         setVarSetName( const OdString& strName ) = 0;
};

typedef OdSmartPtr<OdDgPCMCellSchema> OdDgPCMCellSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgPCMCellDefSchema : public OdDgTableElementSchema
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPCMCellDefSchema );

  virtual OdUInt32     getModelIndex() const = 0;
  virtual void         setModelIndex( OdUInt32 uModelIndex ) = 0;

  virtual OdString     getCellName() const = 0;
  virtual void         setCellName( const OdString& strName ) = 0;
};

typedef OdSmartPtr<OdDgPCMCellDefSchema> OdDgPCMCellDefSchemaPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgTableElementSchemaXAttribute : public OdDgXAttribute
{
public:

  enum Type { /*kType = 0xECDA0000*/    // "Type" as enum name added for swig
#if defined(OD_SWIGCSHARP)
    kType          = (uint)0xECDA0000
#elif defined(OD_SWIGJAVA)
    kType          = (long)0xECDA0000
#else
    kType          = (OdInt32)0xECDA0000
#endif
       };

public:

  ODRX_DECLARE_MEMBERS(OdDgTableElementSchemaXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdUInt16 getTableType() const = 0;
  virtual void     setTableType( OdUInt16 uType ) = 0;

  virtual OdDgTableElementSchemaPtr getSchema() const = 0;
  virtual void setSchema( const OdDgTableElementSchemaPtr& pSchema ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTableElementSchemaXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgTableElementSchemaXAttribute> OdDgTableElementSchemaXAttributePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgSetVariableAsPropertyXAttribute : public OdDgXAttribute
{
public:

  enum Type { /*kType = 0x58E4000A*/    // "Type" as enum name added for swig
#if defined(OD_SWIGCSHARP)
    kType          = (uint)0x58E4000A
#elif defined(OD_SWIGJAVA)
    kType          = (long)0x58E4000A
#else
    kType          = (OdInt32)0x58E4000A
#endif
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgSetVariableAsPropertyXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  // Set property or override local variable.
  virtual bool     getOverrideVariableFlag() const = 0;
  virtual void     setOverrideVariableFlag( bool bSet ) = 0;

  virtual OdUInt32 getVariableDescriptionElementId() const = 0;
  virtual void     setVariableDescriptionElementId( OdUInt32 uElementId ) = 0;

  virtual OdString getIdentificationString() const = 0;
  virtual void     setIdentificationString( const OdString& strValue ) = 0;

  virtual OdUInt32 getVariablePropertyPairCount() const = 0;
  virtual bool     getVariablePropertyPair( OdUInt32 uIndex, OdString& strVariable, OdString& strProperty ) const = 0;
  virtual void     setVariableToProperty( OdString& strVariable, OdString& strProperty ) = 0;
  virtual OdString getVariableName( OdString& strProperty ) const = 0;
  virtual void     deleteVariablePropertyPair( OdUInt32 uIndex ) = 0;
  virtual void     deleteVariablePropertyPair( OdString& strProperty ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgSetVariableAsPropertyXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgSetVariableAsPropertyXAttribute> OdDgSetVariableAsPropertyXAttributePtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgConstraintRepPoint
{
public:
  enum OdDgConstraintRepTopologyType
  {
    kUnknownTopology                = 0x00,
    kParametricSolidTopology        = 0x01,
    kSolidCapTopology               = 0x0E,
    kSolidLateralTopology           = 0x0F,
    kParametricSolidShortTopology   = 0x14,
    kCurveBaseTopology              = 0x15
  };

  enum OdDgConstraintRepPointSnapType
  {
    kFullElementOrSegment           = 0x00,
    kStartOfSegment                 = 0x01,
    kEndOfSegment                   = 0x02,
    kOriginPoint                    = 0x03,
    kEllipsePrimaryAxis             = 0x0B
  };

public:
  OdDgConstraintRepPoint();
  virtual ~OdDgConstraintRepPoint();

  virtual OdUInt32 getEelementIdMapIndex() const;
  virtual void     setEelementIdMapIndex( OdUInt32 uIndex );

  virtual OdUInt32 getSegmentIndex() const;
  virtual void     setSegmentIndex( OdUInt32 uIndex );

  virtual OdDgConstraintRepPoint::OdDgConstraintRepPointSnapType getSnapType() const;
  virtual void     setSnapType( const OdDgConstraintRepPoint::OdDgConstraintRepPointSnapType& uSnapType );

  virtual OdUInt32 getNumberOfSegments() const;
  virtual void     setNumberOfSegments( OdUInt32 nSegments );

  virtual bool     getSkipTopLevelComplexElementFlag() const;
  virtual void     setSkipComplexElementFlag( bool bSkip );

  virtual OdUInt16 getTopologyId() const;
  virtual void     setTopologyId( OdUInt16 uTopologyId );

  virtual OdUInt16 getTopologyFlags() const;
  virtual void     setTopologyFlags( OdUInt16 uFlags );

  virtual OdDgConstraintRepPoint::OdDgConstraintRepTopologyType getType() const;
  virtual void     setType( OdDgConstraintRepPoint::OdDgConstraintRepTopologyType uType );

  virtual OdUInt32 getItemsCount() const;
  virtual OdUInt32 getItem( OdUInt32 uIndex ) const;
  virtual void     setItem( OdUInt32 uIndex, OdUInt32 uValue );
  virtual void     addItem( OdUInt32 uValue );
  virtual void     deleteItem( OdUInt32 uIndex );

  virtual OdBinaryData  getData() const;
  virtual void          setData( OdBinaryData& binData );

private:
  OdUInt32 m_uElementIdIndex;
  OdUInt32 m_uSegmentIndex;
  OdUInt32 m_nSegments;
  bool     m_bSkipTopLevelCplxElm;
  OdDgConstraintRepPoint::OdDgConstraintRepPointSnapType m_uSnapType;
  OdUInt16 m_uTopologyId;
  OdUInt16 m_uTopologyFlags;
  OdDgConstraintRepPoint::OdDgConstraintRepTopologyType m_uTopologyType;
  OdArray<OdUInt32> m_arrTopologyItems;
};

//------------------------------------------------------------------------------------

class OdDgConstraintRepresentation;
typedef OdSmartPtr<OdDgConstraintRepresentation> OdDgConstraintRepresentationPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgConstraintRepresentation : public OdRxObject
{
public:

  enum OdDgConstraintFamily
  {
    k2dConstraints          = 0,
    kDimensionalConstraints = 1
  };

  enum OdDgConstraintType
  {
    kUnknownConstraint               = 0,
    kDistanceConstraint              = 0x80,
    kAngleConstraint                 = 0x81,
    kConcentricConstraint            = 0x82,
    kRadiusConstraint                = 0x83,
    kParallelConstraint              = 0x84,
    kPerpendicularConstraint         = 0x85,
    kCoincidentConstraint            = 0x86,
    kTangentConstraint               = 0x87,
    kEllipArcPrimaryAxisConstraint   = 0x8A,
    kEllipArcSecondaryAxisConstraint = 0x8B,
    kEqualDistanceConstraint         = 0x8D,
    kAreaConstraint                  = 0xE4,
    kPerimeterConstraint             = 0xE5,
    kFix2dConstraint                 = 0xE6,
    kEqualConstraint                 = 0xE9,
    kDimensionPlaneConstraint        = 0xEA
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgConstraintRepresentation);

  virtual OdDgConstraintRepresentation::OdDgConstraintFamily getFamily() const = 0;

  virtual OdDgConstraintRepresentation::OdDgConstraintType getType() const = 0;
  virtual void                                             setType( const OdDgConstraintRepresentation::OdDgConstraintType& uType ) = 0;

  virtual OdUInt32               getPointCount() const = 0;
  virtual OdDgConstraintRepPoint getPoint( OdUInt32 uIndex ) const = 0;
  virtual void setPoint( OdUInt32 uIndex, const OdDgConstraintRepPoint& ptConstraint ) = 0;
  virtual void addPoint( const OdDgConstraintRepPoint& ptConstraint ) = 0;
  virtual void deletePoint( OdUInt32 uIndex ) = 0;

  static OdDgConstraintRepresentationPtr createObject( OdDgConstraintFamily uFamily );
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDg2dConstraintRepresentation : public OdDgConstraintRepresentation
{
public:
  ODRX_DECLARE_MEMBERS(OdDg2dConstraintRepresentation);
};

typedef OdSmartPtr<OdDg2dConstraintRepresentation> OdDg2dConstraintRepresentationPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgDimensionalConstraintRepresentation : public OdDgConstraintRepresentation
{
public:
  enum OdDgDimensionalConstraintRepAngleQuadrant
  {
    kQuadrant0 = 0,
    kQuadrant1 = 1,
    kQuadrant2 = 2,
    kQuadrant3 = 3
  };

  enum OdDgDimensionalConstraintYOffsetAlignment
  {
    kTrue      = 0,
    kPositiveY = 1,
    kNegativeY = 2,
    kPositiveX = 3,
    kNegativeX = 4
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgDimensionalConstraintRepresentation);

  virtual double       getValue() const = 0;
  virtual void         setValue( double dValue ) = 0;

  virtual OdString     getVariableName() const = 0;
  virtual void         setVariableName( const OdString& strName ) = 0;

  virtual double       getXOffsetValue() const = 0;
  virtual void         setXOffsetValue( double dValue ) = 0;

  virtual double       getYOffsetValue() const = 0;
  virtual void         setYOffsetValue( double dValue ) = 0;

  virtual double       getParamValue() const = 0;
  virtual void         setParamValue( double dValue ) = 0;

  virtual OdDgDimensionalConstraintRepAngleQuadrant getAngleQuadrant() const = 0;
  virtual void         setAngleQuadrant( const OdDgDimensionalConstraintRepAngleQuadrant& uQuadrant ) = 0;

  virtual bool         getClockwiseAngleFlag() const = 0;
  virtual void         setClockwiseAngleFlag( bool bSet ) = 0;

  virtual OdDgDimensionalConstraintYOffsetAlignment getAlignment() const = 0;
  virtual void         setAlignment( const OdDgDimensionalConstraintYOffsetAlignment& uAlignment ) = 0;

  virtual OdUInt8      getSegment1Status() const = 0;
  virtual void         setSegment1Status( OdUInt8 uStatus ) = 0;

  virtual OdUInt8      getSegment2Status() const = 0;
  virtual void         setSegment2Status( OdUInt8 uStatus ) = 0;

  virtual OdUInt32     getDirectionFlags() const = 0;
  virtual void         setDirectionFlags( OdUInt32 uFlags ) = 0;

  virtual OdUInt32     getUnknownValue1() const = 0;
  virtual void         setUnknownValue1( OdUInt32 uValue ) = 0;

  virtual double       getUnknownValue2() const = 0;
  virtual void         setUnknownValue2( double dValue ) = 0;
};

typedef OdSmartPtr<OdDgDimensionalConstraintRepresentation> OdDgDimensionalConstraintRepresentationPtr;

//------------------------------------------------------------------------------------

class OdDgConstraintsDataXAttr;
typedef OdSmartPtr<OdDgConstraintsDataXAttr> OdDgConstraintsDataXAttrPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgConstraintsDataXAttr : public OdRxObject
{
public:

  enum OdDgConstraintsXAttributeType
  {
    k2dConstraints      = 0,
    kDimConstraints     = 1,
    kVersionConstraints = 2,
    kIdMapConstraints   = 3,
    kElmVisibilityConstraints   = 4
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgConstraintsDataXAttr);

  virtual OdDgConstraintsDataXAttr::OdDgConstraintsXAttributeType getType() const = 0;

  virtual OdBinaryData getData() const = 0;
  virtual void         setData( const OdBinaryData& binData ) = 0;

  static OdDgConstraintsDataXAttrPtr createObject( OdDgConstraintsXAttributeType type, const OdBinaryData& binData );
};

//------------------------------------------------------------------------------------

class TG_EXPORT OdDg2dConstraintsDataXAttr : public OdDgConstraintsDataXAttr
{
public:
  ODRX_DECLARE_MEMBERS( OdDg2dConstraintsDataXAttr );

  virtual bool         isCorrupted() const = 0;

  virtual OdUInt32                        getConstraintsCount() = 0;
  virtual OdDgConstraintRepresentationPtr getConstraint( OdUInt32 uIndex ) const = 0;
  virtual bool                            setConstraint( OdUInt32 uIndex, const OdDgConstraintRepresentationPtr& pConstraint ) = 0;
  virtual bool                            addConstraint(  const OdDgConstraintRepresentationPtr& pConstraint ) = 0;
  virtual void                            deleteConstraint( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDg2dConstraintsDataXAttr> OdDg2dConstraintsDataXAttrPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgDimConstraintsDataXAttr : public OdDgConstraintsDataXAttr
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimConstraintsDataXAttr );

  virtual bool         isCorrupted() const = 0;

  virtual OdUInt32                        getConstraintsCount() = 0;
  virtual OdDgConstraintRepresentationPtr getConstraint( OdUInt32 uIndex ) const = 0;
  virtual bool                            setConstraint( OdUInt32 uIndex, const OdDgConstraintRepresentationPtr& pConstraint ) = 0;
  virtual bool                            addConstraint(  const OdDgConstraintRepresentationPtr& pConstraint ) = 0;
  virtual void                            deleteConstraint( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgDimConstraintsDataXAttr> OdDgDimConstraintsDataXAttrPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgVersionConstraintsDataXAttr : public OdDgConstraintsDataXAttr
{
public:
  ODRX_DECLARE_MEMBERS( OdDgVersionConstraintsDataXAttr );

  virtual OdUInt16 getVersion() const = 0;
  virtual void setVersion( OdUInt16 uVersion ) = 0;

  virtual OdUInt16 getSubVersion() const = 0;
  virtual void setSubVersion( OdUInt16 uSubVersion ) = 0;
};

typedef OdSmartPtr<OdDgVersionConstraintsDataXAttr> OdDgVersionConstraintsDataXAttrPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgIdMapConstraintsDataXAttr : public OdDgConstraintsDataXAttr
{
public:
  ODRX_DECLARE_MEMBERS( OdDgIdMapConstraintsDataXAttr );

  virtual OdUInt32 getMapItemCount() const = 0;
  virtual bool     getItem( OdUInt32 uIndex, OdUInt32& uMapIndex, OdUInt64& uElementId ) const = 0;
  virtual void     addItem( OdUInt32 uMapIndex, OdUInt64 uElementId ) = 0;
  virtual void     deleteItem( OdUInt32 uMapIndex ) = 0;
  virtual OdUInt64 getElementIdByMapIndex( OdUInt32 uMapIndex ) const = 0;
  virtual OdUInt32 getMapIndexByElementId( OdUInt64 uElementId ) const = 0;
};

typedef OdSmartPtr<OdDgIdMapConstraintsDataXAttr> OdDgIdMapConstraintsDataXAttrPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgElmVisibilityConstraintsDataXAttr : public OdDgConstraintsDataXAttr
{
public:
  ODRX_DECLARE_MEMBERS( OdDgElmVisibilityConstraintsDataXAttr );

  virtual OdUInt32 getVisibilityCount() const = 0;
  virtual bool     getVisibility( OdUInt32 uIndex, OdUInt32& uMapIndex, bool& bVisible ) const = 0;
  virtual void     addVisibility( OdUInt32 uMapIndex, bool bVisible ) = 0;
  virtual void     deleteVisibility( OdUInt32 uMapIndex ) = 0;
  virtual bool     getVisibilityByMapIndex( OdUInt32 uMapIndex ) const = 0;
};

typedef OdSmartPtr<OdDgElmVisibilityConstraintsDataXAttr> OdDgElmVisibilityConstraintsDataXAttrPtr;

//------------------------------------------------------------------------------------

class TG_EXPORT OdDgConstraintsXAttribute : public OdDgXAttribute
{
public:

  enum Type { /*kType = 0x56DA0001*/    // "Type" as enum name added for swig
#if defined(OD_SWIGCSHARP)
    kType          = (uint)0x56DA0001
#elif defined(OD_SWIGJAVA)
    kType          = (long)0x56DA0001
#else
    kType          = (OdInt32)0x56DA0001
#endif
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgConstraintsXAttribute);

  virtual void readData(const OdBinaryData& data, OdDgDatabase* pDb = 0) = 0;
  virtual void writeData(OdBinaryData& data) const = 0;

  virtual OdDgConstraintsDataXAttrPtr   getXAttrData() const = 0;
  virtual void                          setXAttrData( const OdDgConstraintsDataXAttrPtr& pData ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgSetVariableAsPropertyXAttribute object pointers.
*/
typedef OdSmartPtr<OdDgConstraintsXAttribute> OdDgConstraintsXAttributePtr;

/** \details
    Sets (registers) the XAttribute handler class.

    \param handlerId [in]  Handler ID (type) of XAttribute.
    \param pXAttributeClass [in]  Handler class

    Returns pointer to previous XAttribute handler class.
*/
TG_EXPORT OdRxClass* oddgSetXAttributeHandler(OdUInt32 handlerId, OdRxClass* pXAttributeClass);

#include "TD_PackPop.h"

#endif // __DG_XATTRIBUTE_H__
