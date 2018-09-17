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
#ifndef __DG_ELEMENTTEMPLATE_H__
#define __DG_ELEMENTTEMPLATE_H__

#include "DgApplicationData.h"
#include "DgElementIterator.h"

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTemplateColorValue
{
  public:

    enum OdDgTemplateColorType
    {
      kColorByLevel = 0,
      kColorByCell  = 1,
      kColorIndex   = 2,
      kColorRGB     = 3,
      kColorBook    = 4  // RGB color must be initialized too
    };

  public:

    OdDgTemplateColorValue();

    OdDgTemplateColorType getColorType() const;
    void setColorType( OdDgTemplateColorType uType );

    // for color by index

    OdUInt32 getColorIndex() const;
    void     setColorIndex( OdUInt32 uIndex );

    // for rgb color and color book

    ODCOLORREF getColorRGB() const;
    void       setColorRGB( ODCOLORREF clrRgbColor );

    // for color books

    OdUInt32 getColorBookIndex() const;
    void     setColorBookIndex( OdUInt32 uIndex );

    OdUInt32 getColorIndexInBook() const;
    void     setColorIndexInBook( OdUInt32 uIndex );

    OdString getBookAndColorName() const;
    void     setBookAndColorName( OdString strBookAndColorName );

    OdBinaryData getExtendedColorData() const;
    void         setExtendedColorData( const OdBinaryData& binData );

  private:

    OdUInt16                m_uFlags;
    OdDgTemplateColorType   m_uColorType;
    OdUInt32                m_uColorIndex;
    ODCOLORREF              m_uColorRGB;
    OdUInt32                m_uColorBookIndex;
    OdUInt32                m_uColorIndexInBook;
    OdString                m_strBookAndColorName;
    OdBinaryData            m_extColorData;
};

//--------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTemplateValueVariant
{
  public:

    enum OdDgTemplateVariantDataType
    {
      kNone    = 0,
      kBoolean = 1,
      kByte    = 2,
      kShort   = 3,
      kDouble  = 4,
      kPoint3d = 5,
      kColor   = 6,
      kString  = 7
    };

  public:

    OdDgTemplateValueVariant();

    OdDgTemplateVariantDataType getDataType() const;
    void setDataType( OdDgTemplateVariantDataType uType );

    bool     getBoolValue() const;
    void     setBoolValue( bool bVal );

    OdUInt8  getByteValue() const;
    void     setByteValue( OdUInt8 uVal );

    OdUInt16 getShortValue() const;
    void     setShortValue( OdUInt16 uVal );

    double   getDoubleValue() const;
    void     setDoubleValue( double dVal );

    OdGePoint3d   getPoint3dValue() const;
    void          setPoint3dValue( const OdGePoint3d& ptVal );

    OdString   getStringValue() const;
    void       setStringValue( const OdString& strVal );

    OdDgTemplateColorValue   getColorValue() const;
    void       setColorValue( const OdDgTemplateColorValue& clrVal );

  private:

    OdDgTemplateVariantDataType m_uType;
    bool                        m_bBoolVal;
    OdUInt8                     m_uByteVal;
    OdUInt16                    m_uShortVal;
    double                      m_dDoubleVal;
    OdGePoint3d                 m_ptPoint3dVal;
    OdString                    m_strVal;
    OdDgTemplateColorValue      m_clrColorVal;
};

//--------------------------------------------------------------

class OdDgTemplateItem;
typedef OdSharedPtr<OdDgTemplateItem> OdDgTemplateItemPtr;

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgTemplateItem
{
  public:

    OdDgTemplateItem();
    ~OdDgTemplateItem();

    OdUInt32            getPathLength() const;
    OdString            getPathItem( OdUInt32 uIndex ) const;
    void                setPathItem( OdUInt32 uIndex, const OdString& strPathItem );
    void                addPathItem( const OdString& strPathItem );
    void                deletePathItem( OdUInt32 uIndex );

    OdUInt32                 getValueCount() const;
    OdDgTemplateValueVariant getValue( OdUInt32 uIndex ) const;
    void                     setValue( OdUInt32 uIndex, const OdDgTemplateValueVariant& valData );
    void                     addValue( const OdDgTemplateValueVariant& valData );
    void                     deleteValue( OdUInt32 uIndex );

  private:

    OdArray<OdDgTemplateValueVariant> m_arrValues;
    OdArray<OdString>                 m_strPath;
};

//--------------------------------------------------------------

class OdDgElementTemplate;
typedef OdSmartPtr<OdDgElementTemplate> OdDgElementTemplatePtr;

//--------------------------------------------------------------

/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgElementTemplate : public OdDgApplicationData
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgElementTemplate);

public:

  enum
  {
    kSignature = 0x583B
  };

public:

  virtual OdString getTemplateName() const;
  virtual void     setTemplateName( const OdString& strName );

  virtual OdUInt32 getTemplateOrder() const;
  virtual void     setTemplateOrder( OdUInt32 uOrder );

  virtual OdUInt64 getParentId() const;
  virtual void     setParentId( OdUInt64 uId );

  virtual bool     isTemplateGroup() const;
  virtual void     setTemplateGroup( bool bSet );

  // For element templates

  virtual OdUInt32 getItemCount() const;

  virtual OdDgTemplateItem getTemplateItem( OdUInt32 uIndex );
  virtual void setTemplateItem( OdUInt32 uIndex, const OdDgTemplateItem& item );
  virtual void addTemplateItem( const OdDgTemplateItem& item );
  virtual void deleteTemplateItem( OdUInt32 uIndex );

  // For element template groups

  virtual void addElementTemplate( OdDgElement* pTemplate );
  virtual OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;
  virtual OdDgElementId getAt( const OdString& strTemplate );
};

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgElementTemplateTable : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgElementTemplateTable);
public:

  void add( OdDgElementPtr pTemplate);

  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  OdDgElementId getAt( const OdString& strTemplate );
};

typedef OdSmartPtr<OdDgElementTemplateTable> OdDgElementTemplateTablePtr;

#endif // __DG_ELEMENTTEMPLATE_H__
