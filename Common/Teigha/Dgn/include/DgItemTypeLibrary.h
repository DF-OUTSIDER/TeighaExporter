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
#ifndef __DG_ITEMTYPELIBRARY_H__
#define __DG_ITEMTYPELIBRARY_H__


#include "TD_PackPush.h"

#include "DgElement.h"
#include "OdBinaryData.h"
#include "DgTagSetDefinition.h"
#include "OdTimeStamp.h"
#include "DgGraphicsElement.h"

//------------------------------------------------------

OdString TG_EXPORT convertItemDisplayLabelToName( const OdString& strName );
OdString TG_EXPORT convertItemNameToDisplayLabel( const OdString& strDispalyLabel );

//------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/

//------------------------------------------------------

class OdDgItemTypeProperty;
typedef OdSmartPtr<OdDgItemTypeProperty> OdDgItemTypePropertyPtr;

//------------------------------------------------------

class OdDgItemTypePropertyUnits;
class OdDgItemTypePropertyValue;

//------------------------------------------------------

class TG_EXPORT OdDgItemTypeProperty : public OdRxObject
{
public:
  enum OdDgItemTypePropertyType
  {
    kCustom   = 0,
    kString   = 1,
    kDouble   = 2,
    kInteger  = 3,
    kBoolean  = 4,
    kDateTime = 5,
    kPoint3d  = 6
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgItemTypeProperty);

  virtual OdUInt32             getEntryId() const = 0;

  virtual OdDgItemTypePropertyType getType() const = 0;
  virtual void                     setType( const OdDgItemTypePropertyType& uType ) = 0;

  virtual OdString             getTypeName() const = 0;
  virtual void                 setTypeName( const OdString& strTypeName ) = 0;

  virtual OdString             getName() const = 0;
  virtual bool                 setName( const OdString& strName ) = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual bool                 getReadOnlyFlag() const = 0;
  virtual void                 setReadOnlyFlag( bool bSet ) = 0;

  virtual bool                 getArrayFlag() const = 0;
  virtual void                 setArrayFlag( bool bSet ) = 0;

  virtual bool                 getUsePropertyTypeFlag() const = 0;
  virtual void                 setUsePropertyTypeFlag( bool bSet ) = 0;

  virtual bool                 hasDefaultValue() const = 0;
  virtual void                 clearDefaultValue() = 0;

  virtual OdDgItemTypePropertyValue getDefaultValue() const = 0;
  virtual void                 setDefaultValue( const OdDgItemTypePropertyValue& defaultValue ) = 0;

  virtual OdDgItemTypePropertyUnits getUnits() const = 0;
  virtual void                 setUnits( const OdDgItemTypePropertyUnits& unitsValue ) = 0;

  virtual OdDgItemTypePropertyPtr createClone() const = 0;
};

//------------------------------------------------------

class TG_EXPORT OdDgItemTypePropertyUnits
{
public:
  enum OdDgItemTypePropertyUnitsType
  {
    kUndefined           = 0,
    kCustomUnits         = 1,
    kWorkingUnits        = 2,
    kSquareWorkingUnits  = 3,
    kCubicWorkingUnits   = 4,
    kAngularWorkingUnits = 5
  };

public:
  OdDgItemTypePropertyUnits();
  virtual ~OdDgItemTypePropertyUnits();

  OdString getUnitsName() const;
  void     setUnitsName( const OdString& strUnitsName );

  OdDgItemTypePropertyUnitsType getUnitsType() const;
  void setUnitsType( const OdDgItemTypePropertyUnitsType& uType );

private:
  OdDgItemTypePropertyUnitsType m_uType;
  OdString                      m_strUnitsName;
};

//------------------------------------------------------

class TG_EXPORT OdDgItemTypePropertyValue
{
public:
  OdDgItemTypePropertyValue();
  virtual ~OdDgItemTypePropertyValue();

  virtual OdDgItemTypeProperty::OdDgItemTypePropertyType getPropertyType() const;

  virtual OdString getStringValue() const;
  virtual void     setStringValue( const OdString& strValue );

  virtual double   getDoubleValue() const;
  virtual void     setDoubleValue( double dValue );

  virtual OdInt32  getIntegerValue() const;
  virtual void     setIntegerValue( OdInt32 iValue );

  virtual bool     getBooleanValue() const;
  virtual void     setBooleanValue( bool bValue );

  virtual OdTimeStamp getDateTimeValue() const;
  virtual void     setDateTimeValue( const OdTimeStamp& timeValue );

  virtual OdGePoint3d getPointValue() const;
  virtual void     setPointValue( const OdGePoint3d& ptValue );

private:

  OdDgItemTypeProperty::OdDgItemTypePropertyType m_valueType;
  OdString    m_strValue;
  double      m_dValue;
  OdInt32     m_iValue;
  bool        m_bValue;
  OdTimeStamp m_dateTimeValue;
  OdGePoint3d m_ptValue;
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/

//------------------------------------------------------

class OdDgItemType;
typedef OdSmartPtr<OdDgItemType> OdDgItemTypePtr;

//------------------------------------------------------

class TG_EXPORT OdDgItemType : public OdRxObject
{
public:
  enum OdDgItemTypeCategory
  {
    kItemType        = 0,
    kPropertyType    = 1,
    kCustomAttribute = 2
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgItemType);

  virtual OdUInt32             getEntryId() const = 0;

  virtual OdDgItemTypeCategory getCategory() const = 0;
  virtual void                 setCategory( const OdDgItemTypeCategory& uCategory ) = 0;

  virtual OdString             getName() const = 0;
  virtual bool                 setName( const OdString& strName ) = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual bool                 getUseNameForElementFlag() const = 0;
  virtual void                 setUseNameForElementFlag( bool bSet ) = 0;

  virtual OdUInt32             getPropertyCount() const = 0;
  virtual OdDgItemTypePropertyPtr getProperty( OdUInt32 uIndex ) const = 0;
  virtual OdDgItemTypePropertyPtr getProperty( const OdString& strPropertyName ) const = 0;
  virtual void setProperty( OdUInt32 uIndex, const OdDgItemTypePropertyPtr& pProp ) = 0;
  virtual void setProperty( const OdDgItemTypePropertyPtr& pProp ) = 0;
  virtual void addProperty( const OdDgItemTypePropertyPtr& pProp ) = 0;
  virtual void deleteProperty( OdUInt32 uIndex ) = 0;
  virtual void deleteProperty(  const OdString& strPropertyName ) = 0;

  virtual OdDgItemTypePtr      createClone() const = 0;
};

//------------------------------------------------------

class TG_EXPORT OdDgItemTypeLibrary : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgItemTypeLibrary);

public:
  virtual OdString getName() const;
  virtual bool     setName(const OdString& name);

  virtual OdString getDescription() const;
  virtual void setDescription( const OdString& strDescription );

  virtual OdString getDisplayLabel() const;
  virtual void setDisplayLabel( const OdString& strDisplayLabel );

  virtual OdUInt32 getItemTypeCount() const;
  virtual OdDgItemTypePtr getItemType( OdUInt32 uIndex ) const;
  virtual OdDgItemTypePtr getItemType( const OdString& strItemName ) const;
  virtual void setItemType( OdUInt32 uIndex, const OdDgItemTypePtr& pItem );
  virtual void setItemType( const OdDgItemTypePtr& pItem );
  virtual void addItemType( const OdDgItemTypePtr& pItem );
  virtual void deleteItemType( OdUInt32 uIndex );
  virtual void deleteItemType(  const OdString& strItemName );

// If item type is corrupted.
  virtual bool getIsProxyFlag() const;
  virtual void setIsProxyFlag( bool bSet );

  virtual OdUInt32 getBinarySize() const;
  virtual void     getBinaryData( OdBinaryData& data ) const;
  virtual void     setBinaryData( const OdBinaryData& data );
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgItemTypeLibrary> OdDgItemTypeLibraryPtr;

//------------------------------------------------------

class TG_EXPORT OdDgXMLSchemaContainer : public OdDgItemTypeLibrary
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgXMLSchemaContainer);
public:
  virtual bool setName(const OdString& name);
  virtual void setDescription( const OdString& strDescription );
  virtual void setDisplayLabel( const OdString& strDisplayLabel );

  virtual OdDgItemTypePtr getItemType( OdUInt32 uIndex ) const;
  virtual OdDgItemTypePtr getItemType( const OdString& strItemName ) const;
  virtual void setItemType( OdUInt32 uIndex, const OdDgItemTypePtr& pItem );
  virtual void setItemType( const OdDgItemTypePtr& pItem );
  virtual void addItemType( const OdDgItemTypePtr& pItem );
  virtual void deleteItemType( OdUInt32 uIndex );
  virtual void deleteItemType(  const OdString& strItemName );

  virtual void setIsProxyFlag( bool bSet );
  virtual void setBinaryData( const OdBinaryData& data );

  virtual OdString getXmlString() const;
  virtual void     setXmlString( const OdString& strData );
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgXMLSchemaContainer> OdDgXMLSchemaContainerPtr;

//------------------------------------------------------

class OdDgXMLProperty;

//------------------------------------------------------

class OdDgCustomItemTypeProperty;
typedef OdSmartPtr<OdDgCustomItemTypeProperty> OdDgCustomItemTypePropertyPtr;

//------------------------------------------------------

class TG_EXPORT OdDgCustomItemTypeProperty : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCustomItemTypeProperty);

  virtual OdDgItemTypeProperty::OdDgItemTypePropertyType getType() const = 0;
  virtual void                 setType( const OdDgItemTypeProperty::OdDgItemTypePropertyType& uType ) = 0;

  virtual OdString             getTypeName() const = 0;
  virtual void                 setTypeName( const OdString& strTypeName ) = 0;

  virtual OdString             getName() const = 0;
  virtual void                 setName( const OdString& strName ) = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual bool                 getReadOnlyFlag() const = 0;
  virtual void                 setReadOnlyFlag( bool bSet ) = 0;

  virtual bool                 getArrayFlag() const = 0;
  virtual void                 setArrayFlag( bool bSet ) = 0;

  virtual OdUInt32 getCustomAttributePropertiesCount() const = 0;
  virtual OdDgXMLProperty getCustomAttributeProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setCustomAttributeProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addCustomAttributeProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteCustomAttributeProperty( OdUInt32 uIndex ) = 0;

  virtual OdUInt32 getExtraPropertiesCount() const = 0;
  virtual OdDgXMLProperty getExtraProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setExtraProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addExtraProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteExtraProperty( OdUInt32 uIndex ) = 0;

  virtual OdDgCustomItemTypePropertyPtr createClone() const = 0;
};

//------------------------------------------------------

class OdDgCustomItemType;
typedef OdSmartPtr<OdDgCustomItemType> OdDgCustomItemTypePtr;

//------------------------------------------------------

class TG_EXPORT OdDgCustomItemType : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgCustomItemType);

  virtual OdString             getName() const = 0;
  virtual void                 setName( const OdString& strName ) = 0;

  virtual bool                 getIsCustomAttributeClassFlag() const = 0;
  virtual void                 setIsCustomAttributeClassFlag( bool bSet ) = 0;

  virtual bool                 getIsDomainClassFlag() const = 0;
  virtual void                 setIsDomainClassFlag( bool bSet ) = 0;

  virtual bool                 getIsStructFlag() const = 0;
  virtual void                 setIsStructFlag( bool bSet ) = 0;

  virtual OdString             getDisplayLabel() const = 0;
  virtual void                 setDisplayLabel( const OdString& strDisplayLabel ) = 0;

  virtual OdString             getDescription() const = 0;
  virtual void                 setDescription( const OdString& strDescription ) = 0;

  virtual OdUInt32 getCustomAttributePropertiesCount() const = 0;
  virtual OdDgXMLProperty getCustomAttributeProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setCustomAttributeProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addCustomAttributeProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteCustomAttributeProperty( OdUInt32 uIndex ) = 0;

  virtual OdUInt32             getPropertyCount() const = 0;
  virtual OdDgCustomItemTypePropertyPtr getProperty( OdUInt32 uIndex ) const = 0;
  virtual OdDgCustomItemTypePropertyPtr getProperty( const OdString& strPropertyName ) const = 0;
  virtual void setProperty( OdUInt32 uIndex, const OdDgCustomItemTypePropertyPtr& pProp ) = 0;
  virtual void setProperty( const OdDgCustomItemTypePropertyPtr& pProp ) = 0;
  virtual void addProperty( const OdDgCustomItemTypePropertyPtr& pProp ) = 0;
  virtual void deleteProperty( OdUInt32 uIndex ) = 0;
  virtual void deleteProperty(  const OdString& strPropertyName ) = 0;

  virtual OdUInt32 getExtraPropertiesCount() const = 0;
  virtual OdDgXMLProperty getExtraProperty( OdUInt32 uIndex ) const = 0;
  virtual void     setExtraProperty( OdUInt32 uIndex, const OdDgXMLProperty& property ) = 0;
  virtual void     addExtraProperty( const OdDgXMLProperty& property ) = 0;
  virtual void     deleteExtraProperty( OdUInt32 uIndex ) = 0;

  virtual OdDgCustomItemTypePtr      createClone() const = 0;
};

//------------------------------------------------------

struct OdDgItemTypeLibraryReference
{
  OdString m_strName;
  OdString m_strPrefix;
  OdString m_strVersion;
};

//------------------------------------------------------

class TG_EXPORT OdDgXMLCustomSchemaContainer : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgXMLCustomSchemaContainer);

public:
  virtual OdString getName() const;
  virtual void     setName(const OdString& strName);

  virtual OdString getNameSpacePrefix() const;
  virtual void     setNameSpacePrefix(const OdString& strNameSpacePrefix);

  virtual OdString getVersion() const;
  virtual void     setVersion(const OdString& strVersion);

  virtual OdString getDisplayName() const;
  virtual void     setDisplayName(const OdString& strDisplayName);

  virtual OdString getDescription() const;
  virtual void     setDescription(const OdString& strDescription);

  virtual OdArray<OdDgItemTypeLibraryReference> getXMLReferences() const;
  virtual void setXMLReferences( const OdArray<OdDgItemTypeLibraryReference>& arrRefs );

  virtual OdUInt32 getCustomItemCount() const;

  virtual OdDgCustomItemTypePtr getCustomItem( OdUInt32 uIndex ) const;
  virtual OdDgCustomItemTypePtr getCustomItem( const OdString& strItemName ) const;
  virtual void setCustomItem( OdUInt32 uIndex, const OdDgCustomItemTypePtr& pItem );
  virtual void setCustomItem( const OdDgCustomItemTypePtr& pItem );
  virtual void addCustomItem( const OdDgCustomItemTypePtr& pItem );
  virtual void deleteCustomItem( OdUInt32 uIndex );
  virtual void deleteCustomItem(  const OdString& strItemName );

  virtual OdUInt32 getCustomAttributePropertiesCount() const;
  virtual OdDgXMLProperty getCustomAttributeProperty( OdUInt32 uIndex ) const;
  virtual void     setCustomAttributeProperty( OdUInt32 uIndex, const OdDgXMLProperty& property );
  virtual void     addCustomAttributeProperty( const OdDgXMLProperty& property );
  virtual void     deleteCustomAttributeProperty( OdUInt32 uIndex );

  virtual OdUInt32 getExtraPropertiesCount() const;
  virtual OdDgXMLProperty getExtraProperty( OdUInt32 uIndex ) const;
  virtual void     setExtraProperty( OdUInt32 uIndex, const OdDgXMLProperty& property );
  virtual void     addExtraProperty( const OdDgXMLProperty& property );
  virtual void     deleteExtraProperty( OdUInt32 uIndex );
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgXMLCustomSchemaContainer> OdDgXMLCustomSchemaContainerPtr;

//------------------------------------------------------

class OdDgItemTypePropertyInstance;

//------------------------------------------------------

typedef OdArray<OdDgItemTypePropertyInstance> OdDgItemTypeComplexPropertyInstance;

//------------------------------------------------------

class TG_EXPORT OdDgItemTypeComplexPropertyArrayInstance
{
public:
  OdDgItemTypeComplexPropertyArrayInstance();
  virtual ~OdDgItemTypeComplexPropertyArrayInstance();

  OdString getPropertyTypeName() const;
  void     setPropertyTypeName( const OdString& strTypeName );

  OdUInt32 getComplexPropertyCount() const;
  OdDgItemTypeComplexPropertyInstance getComplexProperty( OdUInt32 uIndex ) const;
  void     setComplexProperty( OdUInt32 uIndex, const OdDgItemTypeComplexPropertyInstance& complexProperty );
  void     addComplexProperty( const OdDgItemTypeComplexPropertyInstance& complexProperty );
  void     deleteComplexProperty( OdUInt32 uIndex );

private:
  OdString m_strPropertyTypeName;
  OdArray<OdDgItemTypeComplexPropertyInstance> m_propArray;
};

//------------------------------------------------------

class TG_EXPORT OdDgItemTypePropertyInstanceValue
{
public:
  enum OdDgItemTypePropertyInstanceValueType
  {
    kUndefined            = 0,
    kBool                 = 1,
    kBoolArray            = 2,
    kByte                 = 3,
    kByteArray            = 4,
    kShort                = 5,
    kShortArray           = 6,
    kInt32                = 7,
    kInt32Array           = 8,
    kInt64                = 9,
    kInt64Array           = 10,
    kDouble               = 11,
    kDoubleArray          = 12,
    kString               = 13,
    kStringArray          = 14,
    kPoint3d              = 15,
    kPoint3dArray         = 16,
    kDateAndTime          = 17,
    kDateTimeArray        = 18,
    kComplexProperty      = 19,
    kComplexPropertyArray = 20
  };

public:
  OdDgItemTypePropertyInstanceValue();
  OdDgItemTypePropertyInstanceValue( bool bValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<bool>& arrBoolValue );
  OdDgItemTypePropertyInstanceValue( OdInt8 iValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<OdInt8>& arrByteValue );
  OdDgItemTypePropertyInstanceValue( OdInt16 iValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<OdInt16>& arrShortValue );
  OdDgItemTypePropertyInstanceValue( OdInt32 iValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<OdInt32>& arrIntValue );
  OdDgItemTypePropertyInstanceValue( OdInt64 iValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<OdInt64>& arrInt64Value );
  OdDgItemTypePropertyInstanceValue( double dValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<double>& arrDoubleValue );
  OdDgItemTypePropertyInstanceValue( const OdString& strValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<OdString>& arrStrValue );
  OdDgItemTypePropertyInstanceValue( const OdGePoint3d& ptValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<OdGePoint3d>& arrPt3dValue );
  OdDgItemTypePropertyInstanceValue( const OdTimeStamp& timeValue );
  OdDgItemTypePropertyInstanceValue( const OdArray<OdTimeStamp>& arrDateTimeValue );
  OdDgItemTypePropertyInstanceValue( const OdDgItemTypeComplexPropertyInstance& compelxProperty );
  OdDgItemTypePropertyInstanceValue( const OdDgItemTypeComplexPropertyArrayInstance& arrComplexProperty );

  virtual ~OdDgItemTypePropertyInstanceValue();

  void setByDefault( const OdDgItemTypePropertyPtr& pProperty );

  OdDgItemTypePropertyInstanceValueType getType() const;

  bool getBoolValue() const;
  void setBoolValue( bool bValue );

  OdArray<bool> getBoolArrayValue() const;
  void setBoolArrayValue( const OdArray<bool>& arrBoolValue );

  OdInt8 getByteValue() const;
  void   setByteValue( OdInt8 iValue );

  OdArray<OdInt8> getByteArrayValue() const;
  void setByteArrayValue( const OdArray<OdInt8>& arrByteValue );

  OdInt16 getShortValue() const;
  void   setShortValue( OdInt16 iValue );

  OdArray<OdInt16> getShortArrayValue() const;
  void setShortArrayValue( const OdArray<OdInt16>& arrShortValue );

  OdInt32 getIntValue() const;
  void   setIntValue( OdInt32 iValue );

  OdArray<OdInt32> getIntArrayValue() const;
  void setIntArrayValue( const OdArray<OdInt32>& arrIntValue );

  OdInt64 getInt64Value() const;
  void   setInt64Value( OdInt64 iValue );

  OdArray<OdInt64> getInt64ArrayValue() const;
  void setInt64ArrayValue( const OdArray<OdInt64>& arrInt64Value );

  double getDoubleValue() const;
  void   setDoubleValue( double dValue );

  OdArray<double> getDoubleArrayValue() const;
  void setDoubleArrayValue( const OdArray<double>& arrDoubleValue );

  OdString getStringValue() const;
  void   setStringValue( const OdString& strValue );

  OdArray<OdString> getStringArrayValue() const;
  void setStringArrayValue( const OdArray<OdString>& arrStringValue );

  OdGePoint3d getPoint3dValue() const;
  void   setPoint3dValue( const OdGePoint3d& ptValue );

  OdArray<OdGePoint3d> getPoint3dArrayValue() const;
  void setPoint3dArrayValue( const OdArray<OdGePoint3d>& arrPoint3dValue );

  OdTimeStamp getDateTimeValue() const;
  void   setDateTimeValue( const OdTimeStamp& timeValue );

  OdArray<OdTimeStamp> getDateTimeArrayValue() const;
  void setDateTimeArrayValue( const OdArray<OdTimeStamp>& arrDateTimeValue );

  OdDgItemTypeComplexPropertyInstance getComplexPropertyValue() const;
  void setComplexPropertyValue( const OdDgItemTypeComplexPropertyInstance& complexProperty );

  OdDgItemTypeComplexPropertyArrayInstance getComplexPropertyArrayValue() const;
  void setComplexPropertyArrayValue( const OdDgItemTypeComplexPropertyArrayInstance& arrComplexProperty );

private:

  OdDgItemTypePropertyInstanceValueType     m_uType;
  bool                                      m_bValue;
  OdArray<bool>                             m_arrBoolValue;
  OdInt8                                    m_iInt8Value;
  OdArray<OdInt8>                           m_arrInt8Value;
  OdInt16                                   m_iInt16Value;
  OdArray<OdInt16>                          m_arrInt16Value;
  OdInt32                                   m_iInt32Value;
  OdArray<OdInt32>                          m_arrInt32Value;
  OdInt64                                   m_iInt64Value;
  OdArray<OdInt64>                          m_arrInt64Value;
  double                                    m_dValue;
  OdArray<double>                           m_arrDoubleValue;
  OdString                                  m_strValue;
  OdArray<OdString>                         m_arrStringValue;
  OdGePoint3d                               m_ptValue;
  OdArray<OdGePoint3d>                      m_arrPoint3dValue;
  OdTimeStamp                               m_timeValue;
  OdArray<OdTimeStamp>                      m_arrDateTimeValue;
  OdDgItemTypeComplexPropertyInstance       m_complexProperty;
  OdDgItemTypeComplexPropertyArrayInstance  m_arrComplexProperty;
};

//------------------------------------------------------

class TG_EXPORT OdDgItemTypePropertyInstance
{
  public:
    OdDgItemTypePropertyInstance();
    OdDgItemTypePropertyInstance( const OdString& strName, const OdDgItemTypePropertyInstanceValue& value );
    virtual ~OdDgItemTypePropertyInstance();

    OdString getName() const;
    void     setName( const OdString& strName );

    OdDgItemTypePropertyInstanceValue getValue() const;
    void setValue( const OdDgItemTypePropertyInstanceValue& value );

  private:
    OdString m_strName;
    OdDgItemTypePropertyInstanceValue m_value;
};

//------------------------------------------------------

class TG_EXPORT OdDgItemTypeInstance
{
  public:
    OdDgItemTypeInstance();
    virtual ~OdDgItemTypeInstance();

    OdUInt64 getItemTypeLibraryId() const;
    void     setItemTypeLibraryId( OdUInt64 uId );

    OdString getItemTypeLibraryName() const;
    void     setItemTypeLibraryName( const OdString& strName );

    OdString getItemTypeName() const;
    void     setItemTypeName( const OdString& strName );

    OdUInt32 getPropertyCount() const;
    OdDgItemTypePropertyInstance getProperty( OdUInt32 uIndex ) const;
    bool getProperty( const OdString& strName, OdDgItemTypePropertyInstance& retVal ) const;
    void setProperty( OdUInt32 uIndex, const OdDgItemTypePropertyInstance& prop );
    void setProperty( const OdDgItemTypePropertyInstance& prop );
    void addProperty( const OdDgItemTypePropertyInstance& prop );
    void deleteProperty( OdUInt32 uIndex );
    void deleteProperty( const OdString& strName );

  private:
    OdUInt64 m_uItemTypeLibraryId;
    OdString m_strItemTypeLibraryName;
    OdString m_strItemName;
    OdArray<OdDgItemTypePropertyInstance> m_arrProperties;
};

//------------------------------------------------------

/** \details

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgItemTypesGraphicsElementPE : public OdRxObject
{
  ODRX_DECLARE_MEMBERS(OdDgItemTypesGraphicsElementPE);
  long        m_references;     //used by each object separately
public:

  OdDgItemTypesGraphicsElementPE();
  ~OdDgItemTypesGraphicsElementPE();

  //OdRxObject overridden
  virtual void addRef();
  virtual void release();
  virtual long numRefs() const;

  virtual OdUInt32 getItemTypeInstanceCount(const OdDgGraphicsElement* obj);
  virtual OdDgItemTypeInstance getItemTypeInstance(const OdDgGraphicsElement* obj, OdUInt32 uIndex);
  virtual void setItemTypeInstance(OdDgGraphicsElement* obj, OdUInt32 uIndex, const OdDgItemTypeInstance& itemType);
  virtual void addItemTypeInstance(OdDgGraphicsElement* obj, const OdDgItemTypeInstance& itemType);
  virtual void deleteItemTypeInstance(OdDgGraphicsElement* obj, OdUInt32 uIndex);
};

//------------------------------------------------------

typedef OdSmartPtr<OdDgItemTypesGraphicsElementPE> OdDgItemTypesGraphicsElementPEPtr;

//------------------------------------------------------

#include "TD_PackPop.h"

#endif // __DG_ITEMTYPELIBRARY_H__
