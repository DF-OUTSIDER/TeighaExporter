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
#ifndef __DG_ATTRIBUTELINKAGE_H__
#define __DG_ATTRIBUTELINKAGE_H__

#include "TD_PackPush.h"

#include "DgExport.h"
#include "RxObject.h"
#include "OdBinaryData.h"
#include "Ge/GeQuaternion.h"
#include "DgElementId.h"
#include "DoubleArray.h"
#include "DgDimension.h"
#include "DgTextExtendedProperties.h"
#include "DgTable.h"
#include "DgTextStyleTableRecord.h"
#include "UInt32Array.h"
#include "DgOle2Frame.h"
#include "DgGradientFill.h"

class OdDgAttributeLinkageHeader;

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgAttributeLinkage : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgAttributeLinkage);

  //if some linkage has an id from the list above, it is a linkage of some predefine type and could be operated using an appropriate interface
  enum PrimaryIds
  {
    kDMRS       = 0x0000,   // It's a fictive value for DMRS linkages because only non-DMRS linkages could have any primaryId
    kDependency = 0x56D0,   // Dependency Linkage
    kString     = 0x56D2,   // String Linkage
    kThickness  = 0x56D4,   // Thickness Linkage
    kTextStyle  = 0x80D4,   // Text Style Linkage
    kHatch      = 0x7D28,   // Pattern Linkage
    kDWGData    = 0x56E4,   // .dwg file Xdata Linkage
    kByteArray  = 0x5751,   // Byte Array Linkage
    kFillStyle  = 0x0041,
    kOLE        = 0xB01E,
    kBREP       = 0x522E,
    kLStyleMod  = 0x79F9,   // Line Style Modification Linkage

    kFRAMME     = 0x0020,   // DB Linkage - FRAMME tag data signature
    kBSI        = 0x0F81,   // DB Linkage - secondary id link (BSI radix 50)
    kXBASE      = 0x1971,   // DB Linkage - XBase (DBase)
    kINFORMIX   = 0x3948,   // DB Linkage - Informix
    kINGRES     = 0x3A77,   // DB Linkage - INGRES
    kSYBASE     = 0x4F58,   // DB Linkage - Sybase
    kODBC       = 0x5E62,   // DB Linkage - ODBC
    kOLEDB      = 0x5800,   // DB Linkage - OLEDB
    kORACLE     = 0x6091,   // DB Linkage - Oracle
    kRIS        = 0x71FB,   // DB Linkage - RIS

    kFilterMember = 0x56DD, // Level filter member linkage
    kDimension    = 0x56DE, // Dimension & Dimension Style Extension Linkage
    kSheetModel   = 0x56ED, // Sheet model page data linkage
    kSheetScale   = 0x56EE, // Sheet model scale data linkage
    kSheetModelEx = 0x5701, // Sheet model extension data linkage
    kExternalMaterial = 0x4f5a, // External material linkage
    kReferenceAttachmentPath = 0x56F1, // Reference attachment path linkage for level tables
    kAssocRegion  = 0x56D8, // Associative region linkage
    kFeature      = 0x5229, // Feature solid linkage
    kSeedPoints   = 0x56DA, // Seed points linkage
    kDoubleArray  = 0x56D5, // Double array linkage
    kNoteLinkage  = 0x562E, // Note linkage
    kBitMaskLinkage = 0x56D3, // Bit mask linkage
    kTextIndentation = 0x5810, // Text indentation linkage
    kLevelMask       = 0x5710, // Level mask linkage
    kBoundaryClip    = 0x56DC, // Boundary clip linkage
    kXData           = 0x56E4, // X-Data linkage
    kDimTextStyle    = 0x5801, // Linkage to store text style data for dimension text
    kInfiniteLine    = 0x56E9, // Infinite Line linkage (XLine)
    kTextAnnotation  = 0x56F3  // Text annotation linkage
  };

  //returns the primaryId that determines a type of a linkage;
  //Note: the primaryId is zero for DMRS linkages
  virtual OdUInt16 getPrimaryId() const = 0;

  //an implementation should fill this array with its own data,
  //but exclude the linkage header and the primary Id
  virtual void getData(OdBinaryData& data) const = 0;

  virtual OdDgAttributeLinkageHeader getLinkageHeader( bool bRecalculateSize = false ) const = 0;
  virtual void setLinkageHeader( const OdDgAttributeLinkageHeader& hdr ) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgAttributeLinkage object pointers.
*/
typedef OdSmartPtr<OdDgAttributeLinkage> OdDgAttributeLinkagePtr;

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgAttributeLinkageHeader
{
  public:
    OdDgAttributeLinkageHeader();
    OdDgAttributeLinkageHeader( OdDgAttributeLinkage::PrimaryIds uPrimaryId, OdUInt32 uDataSize );
    virtual ~OdDgAttributeLinkageHeader();

    virtual void set( OdDgAttributeLinkage::PrimaryIds uPrimaryId, OdUInt32 uDataSize );

    virtual OdDgAttributeLinkage::PrimaryIds getPrimaryId() const;
    virtual void setPrimaryId( OdDgAttributeLinkage::PrimaryIds uPrimaryId );

    virtual OdUInt32 getLinkageDataSize() const;
    virtual void     setLinkageDataSize( OdUInt32 uSize );

    virtual bool getUserDataFlag() const;

    virtual bool getRemoteFlag() const;

    virtual bool getModifiedFlag() const;
    virtual void setModifiedFlag( bool bSet );

    virtual bool getInfoLinkageFlag() const;
    virtual void setInfoLinkageFlag( bool bSet );

    virtual OdUInt32 getLinkageHeaderValue() const;
    virtual void     setLinkageHeaderValue( OdUInt32 uValue );

    static OdUInt32 getRawDataSize();

  private:

    void      setUserDataFlag( bool bSet );
    void      setRemoteFlag  ( bool bSet );
    OdUInt16  getSizeInWords() const;
    OdUInt16  getSizeShift()   const;
    void      setSizeInWords( OdUInt16 uSize );
    void      setSizeShift( OdUInt16 uShift );

  private:

    OdUInt32 m_uHeaderData;
};


/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgStringLinkage object pointers.
*/
class OdDgStringLinkage;
typedef OdSmartPtr<OdDgStringLinkage> OdDgStringLinkagePtr;


/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgStringLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgStringLinkage);

  static OdDgStringLinkagePtr createObject( OdUInt16 stringId );

  virtual OdUInt32 getStringId() const = 0;
  virtual void setStringId(OdUInt32 val) = 0;

  virtual OdString getString() const = 0;
  virtual void setString(OdString val) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgReferenceAttachPathLinkage object pointers.
*/
class OdDgReferenceAttachPathLinkage;
typedef OdSmartPtr<OdDgReferenceAttachPathLinkage> OdDgReferenceAttachPathLinkagePtr;


/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgReferenceAttachPathLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgReferenceAttachPathLinkage);

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual OdUInt32 getPathLength() const = 0;

  virtual OdUInt64 getPathItem( OdUInt32 nItem ) const = 0;
  virtual void     setPathItem( OdUInt64 uPathElementId, OdUInt32 nItem ) = 0;
  virtual void     deletePathItem( OdUInt32 nItem ) = 0;
  virtual void     addPathItem( OdUInt64 uPathElementId ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgBoundaryClipLinkage object pointers.
*/
class OdDgBoundaryClipLinkage;
typedef OdSmartPtr<OdDgBoundaryClipLinkage> OdDgBoundaryClipLinkagePtr;


/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBoundaryClipLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgBoundaryClipLinkage);

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual OdGeMatrix2d getTransformMatrix() const = 0;
  virtual void         setTransformMatrix( const OdGeMatrix2d& matTransform ) = 0;

  virtual OdGePoint3d  getOrigin() const = 0;
  virtual void         setOrigin( const OdGePoint3d& ptOrigin ) = 0;

  virtual OdUInt32 getPointCount() const = 0;

  virtual OdGePoint2d getPoint( OdUInt32 uIndex ) const = 0;
  virtual void        setPoint( const OdGePoint2d& ptPoint, OdUInt32 uIndex ) = 0;
  virtual void        deletePoint( OdUInt32 uIndex ) = 0;
  virtual void        addPoint( const OdGePoint2d& ptPoint ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgBitMaskLinkage object pointers.
*/

class OdDgBitMaskLinkage;
typedef OdSmartPtr<OdDgBitMaskLinkage> OdDgBitMaskLinkagePtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBitMaskLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgBitMaskLinkage);

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual OdUInt32 getBitMaskId() const = 0;
  virtual void     setBitMaskId( OdUInt32 uLinkageType ) = 0;

  virtual OdUInt32 getBitCount() const = 0;
  virtual void     setBitCount( OdUInt32 uBitCount ) = 0;

  virtual OdBinaryData getBitMaskData() const = 0;
  virtual void         setBitMaskData( const OdBinaryData& binData ) = 0;

};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTextWhiteSpaceLinkage object pointers.
*/

class OdDgTextWhiteSpaceLinkage;
typedef OdSmartPtr<OdDgTextWhiteSpaceLinkage> OdDgTextWhiteSpaceLinkagePtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgTextWhiteSpaceLinkage : public OdDgAttributeLinkage
{
public:

  enum OdDgSpecialSymbolType
  {
    kTab            = 0,
    kParagraphBreak = 1,
    kNewLineBreak   = 2
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgTextWhiteSpaceLinkage);

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual OdUInt32 getSymbolCount() const = 0;

  virtual OdDgSpecialSymbolType getSymbol( OdUInt32 nEntry ) const = 0;
  virtual void     setSymbol( OdDgSpecialSymbolType uValue, OdUInt32 nEntry ) = 0;
  virtual void     deleteSymbol( OdUInt32 nEntry ) = 0;
  virtual void     addSymbol( OdDgSpecialSymbolType uValue ) = 0;

  virtual OdString createString() const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTextIndentationLinkage object pointers.
*/

class OdDgTextIndentationLinkage;
typedef OdSmartPtr<OdDgTextIndentationLinkage> OdDgTextIndentationLinkagePtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgTextIndentationLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTextIndentationLinkage);

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual double getFirstLineIndentation() const = 0;
  virtual void   setFirstLineIndentation( double dIndentation ) = 0;

  virtual double getParagraphIndentation() const = 0;
  virtual void   setParagraphIndentation( double dIndentation ) = 0;

  virtual OdUInt32 getTabCount() const = 0;
  virtual double   getTabValue( OdUInt32 uIndex ) const = 0;
  virtual void     setTabValue( OdUInt32 uIndex, double dValue ) = 0;
  virtual void     addTabValue( double dValue ) = 0;
  virtual void     deleteTabValue( OdUInt32 uIndex ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgOleLinkage object pointers.
*/
class OdDgOleLinkage;
typedef OdSmartPtr<OdDgOleLinkage> OdDgOleLinkagePtr;


/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgOleLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgOleLinkage);

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual OdDgOle2Frame::OdDgDrawAspect getDrawAspect() const = 0;
  virtual void   setDrawAspect( OdDgOle2Frame::OdDgDrawAspect uAspect ) = 0;

  virtual OdDgOle2Frame::OdDgOleObjectType getOleType() const = 0;
  virtual void   setOleType( OdDgOle2Frame::OdDgOleObjectType uType ) = 0;

  virtual OdUInt32 getXSize() const = 0;
  virtual void     setXSize( OdUInt32 uSize ) = 0;

  virtual OdUInt32 getYSize() const = 0;
  virtual void     setYSize( OdUInt32 uSize ) = 0;

  virtual bool     getAspectRatioFlag() const = 0;
  virtual void     setAspectRatioFlag( bool bSet) = 0;

  virtual bool     getRotateWithViewFlag() const = 0;
  virtual void     setRotateWithViewFlag( bool bSet) = 0;

  virtual bool     getViewProjectionModeFlag() const = 0;
  virtual void     setViewProjectionModeFlag( bool bSet) = 0;

  virtual bool     getTransparentBackgroundFlag() const = 0;
  virtual void     setTransparentBackgroundFlag( bool bSet) = 0;

  virtual bool     getCanBeLinkedAndEmbeddedFlag() const = 0;
  virtual void     setCanBeLinkedAndEmbeddedFlag( bool bSet) = 0;

  virtual bool     getCanBePictureFlag() const = 0;
  virtual void     setCanBePictureFlag( bool bSet) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgThicknessLinkage object pointers.
*/
class OdDgThicknessLinkage;
typedef OdSmartPtr<OdDgThicknessLinkage> OdDgThicknessLinkagePtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgThicknessLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgThicknessLinkage);

  virtual double getThickness() const = 0;
  virtual void   setThickness(const double dThickness) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDoubleArrayLinkage object pointers.
*/
class OdDgDoubleArrayLinkage;
typedef OdSmartPtr<OdDgDoubleArrayLinkage> OdDgDoubleArrayLinkagePtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDoubleArrayLinkage : public OdDgAttributeLinkage
{
public:
  enum OdDgDoubleArrayLinkageArrayId
  {
    kXRefClipBoundaryRotation  = 0x04,
    kXRefLightingAndSaturation = 0x09,
    kXRefHSV                   = 0x0B,
    kXRefTransparency          = 0x0e
  };
public:
  ODRX_DECLARE_MEMBERS(OdDgDoubleArrayLinkage);

  virtual void     readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;
  virtual OdUInt32 getArrayId() const = 0;
  virtual void     setArrayId(OdUInt32 val) = 0;

  virtual OdUInt32 getItemCount() const = 0;
  virtual double   getItem( OdUInt32 nItem ) const = 0;
  virtual void     setItem( OdUInt32 nItem, double dValue ) = 0;
  virtual void     addItem( double dValue ) = 0;
  virtual void     deleteItem( OdUInt32 nItem ) = 0;
};


/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDependencyLinkage object pointers.
*/
class OdDgDependencyLinkage;
typedef OdSmartPtr<OdDgDependencyLinkage> OdDgDependencyLinkagePtr;


/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDependencyLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDependencyLinkage);

  enum CopyOptions
  {
    kNoCopy = 0, // no copy, copy will not contain the dependency linkage
    kCopy   = 1, // remap roots to the copies of those roots, and if the roots aren't being copied, the uncopied root elementIDs are set to INVALID_ELEMENTID.
    kCopyEx = 2  // same logic is applied even if roots are from another file.
  };

  enum RootDataType
  {
    kElementId      = 0,
    kElementId_V    = 1,
    kAssocPoint     = 2,
    kAssocPoint_I   = 3,
    kFarElementId   = 4,
    kFarElementId_V = 5,
    kPath_V         = 6
  };

  virtual OdUInt16 getAppId() const = 0;
  virtual void setAppId( OdUInt16 appId ) = 0;

  virtual OdUInt16 getAppValue() const = 0;
  virtual void setAppValue( OdUInt16 appvalue ) = 0;

  virtual bool getDeleteRootsFlag() const = 0;
  virtual bool getDontNeedAppForDeleteFlag() const = 0;
  virtual bool getRequireApplicationFlag() const = 0;
  virtual bool getDisabledFlag() const = 0;

  virtual CopyOptions getCopyOptions() const = 0;

  virtual void setDeleteRootsFlag( bool bSet ) = 0;
  virtual void setDontNeedAppForDeleteFlag( bool bSet ) = 0;
  virtual void setRequireApplicationFlag( bool bSet ) = 0;
  virtual void setDisabledFlag( bool bSet ) = 0;

  virtual void setCopyOptions( CopyOptions value ) = 0;

  virtual RootDataType getRootDataType() const = 0;

};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgDepLinkageElementId object pointers.
*/
class OdDgDepLinkageElementId;
typedef OdSmartPtr<OdDgDepLinkageElementId> OdDgDepLinkageElementIdPtr;


/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDepLinkageElementId : public OdDgDependencyLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDepLinkageElementId);

  virtual OdUInt32 getCount() const = 0;
  virtual OdUInt64 getAt( OdUInt32 index ) const = 0;
  virtual void setAt( OdUInt32 index, OdUInt64 uHandle ) = 0;
  virtual void deleteAt( OdUInt32 index ) = 0;
  virtual void add( OdUInt64 id ) = 0;
};

//---------------------------------------------------------------------

struct OdDgElementIdVData
{
  OdUInt64 m_uId;
  double   m_dValue;
};

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDepLinkageElementIdV : public OdDgDependencyLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDepLinkageElementIdV);

  virtual OdUInt32 getCount() const = 0;
  virtual OdDgElementIdVData getAt( OdUInt32 index ) const = 0;
  virtual void setAt( OdUInt32 index, const OdDgElementIdVData& data ) = 0;
  virtual void deleteAt( OdUInt32 index ) = 0;
  virtual void add( OdDgElementIdVData data ) = 0;
};

typedef OdSmartPtr<OdDgDepLinkageElementIdV> OdDgDepLinkageElementIdVPtr;

//---------------------------------------------------------------------
/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgAssocPointRoot : public OdRxObject
{
public:

  enum OdDgAssocPointRootType
  {
    kUnknownAssociation = 0,
    kLinearAssociation = 1,
    kIntersectAssociation  = 2,
    kArcAssociation = 3,
    kMultilineAssociation = 4,
    kBSplineCurveAssociation = 5,
    kProjectionAssociation = 6,
    kOriginAssociation = 7,
    kIntersect2Association  = 8,
    kMeshVertexAssociation = 10,
    kMeshEdgeAssociation = 11,
    kBSplineSurfaceAssociation = 13
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgAssocPointRoot);

  OdDgAssocPointRoot(){};
  virtual ~OdDgAssocPointRoot(){};

  virtual OdDgAssocPointRootType getType() const = 0;

  virtual OdUInt64 getElementId() const = 0;
  virtual void     setElementId( OdUInt64 uId ) = 0;

  virtual OdUInt64 getRefAttachmentId() const = 0;
  virtual void     setRefAttachmentId( OdUInt64 uId ) = 0;
};

typedef OdSmartPtr<OdDgAssocPointRoot> OdDgAssocPointRootPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgUnknownAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgUnknownAssociation);

  OdDgUnknownAssociation();
  virtual ~OdDgUnknownAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdBinaryData getData() const;
  void setData( OdBinaryData data );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

private:

  OdBinaryData m_data;
  OdUInt64     m_elementId;
  OdUInt64     m_refAttachmentId;
};

typedef OdSmartPtr<OdDgUnknownAssociation> OdDgUnknownAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgLinearAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgLinearAssociation);

  OdDgLinearAssociation();
  virtual ~OdDgLinearAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt16 getVertexNumber() const;
  void     setVertexNumber( OdUInt16 uVertex );

  OdUInt16 getNumerator() const;
  void     setNumerator( OdUInt16 uNum );

  OdUInt16 getDivisor() const;
  void     setDivisor( OdUInt16 uDiv );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  OdUInt16 getNVertices() const;
  void     setNVertices( OdUInt16 nVertices );

private:

  OdUInt16 m_uVertex;
  OdUInt16 m_uNumerator;
  OdUInt16 m_uDivisor;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  OdUInt16 m_nVertices;
};

typedef OdSmartPtr<OdDgLinearAssociation> OdDgLinearAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgIntersectAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgIntersectAssociation);

  OdDgIntersectAssociation();
  virtual ~OdDgIntersectAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt8  getIndex() const;
  void     setIndex( OdUInt8 uIndex );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getElement2Id() const;
  void     setElement2Id( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  OdUInt64 getRefAttachment2Id() const;
  void     setRefAttachment2Id( OdUInt64 uId );

private:

  OdUInt8  m_uIndex;
  OdUInt64 m_element1Id;
  OdUInt64 m_uRefAttach1Id;
  OdUInt64 m_element2Id;
  OdUInt64 m_uRefAttach2Id;
};

typedef OdSmartPtr<OdDgIntersectAssociation> OdDgIntersectAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgArcAssociation : public OdDgAssocPointRoot
{
public:

  enum OdDgArcAssociationKeyPoint
  {
    kAnglePoint  = 0,
    kCenterPoint = 1,
    kStartPoint  = 2,
    kEndPoint    = 3
  };

public:

  ODRX_DECLARE_MEMBERS(OdDgArcAssociation);

  OdDgArcAssociation();
  virtual ~OdDgArcAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdDgArcAssociationKeyPoint getKeyPoint() const;
  void     setKeyPoint( OdDgArcAssociationKeyPoint keyPoint );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getAngle() const;
  void     setAngle( double dAngle );

private:

  OdDgArcAssociationKeyPoint m_uKeyPoint;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dAngle;
};

typedef OdSmartPtr<OdDgArcAssociation> OdDgArcAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMultilineAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgMultilineAssociation);

  OdDgMultilineAssociation();
  virtual ~OdDgMultilineAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt16 getVertexNumber() const;
  void     setVertexNumber( OdUInt16 uVertex );

  OdUInt8  getLineNumber() const;
  void     setLineNumber( OdUInt8 uLineNum );

  bool     getJointFlag() const;
  void     setJointFlag( bool bSet );

  bool     getProjectFlag() const;
  void     setProjectFlag( bool bSet );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getOffset() const;
  void     setOffset( double dOffset );

  OdUInt16 getNVertices() const;
  void     setNVertices( OdUInt16 nVertices );

private:

  OdUInt16 m_uVertex;
  OdUInt16 m_uFlags;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dOffset;
  OdUInt16 m_nVertices;
};

typedef OdSmartPtr<OdDgMultilineAssociation> OdDgMultilineAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineCurveAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgBSplineCurveAssociation);

  OdDgBSplineCurveAssociation();
  virtual ~OdDgBSplineCurveAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getParam() const;
  void     setParam( double dParam );

private:

  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dParam;
};

typedef OdSmartPtr<OdDgBSplineCurveAssociation> OdDgBSplineCurveAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgProjectionAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgProjectionAssociation);

  OdDgProjectionAssociation();
  virtual ~OdDgProjectionAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt16 getVertexNumber() const;
  void     setVertexNumber( OdUInt16 uVertex );

  OdUInt16 getNVertices() const;
  void     setNVertices( OdUInt16 nVertices );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getFraction() const;
  void     setFraction( double dFraction );

private:

  OdUInt16 m_uVertex;
  OdUInt16 m_nVertices;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dFraction;
};

typedef OdSmartPtr<OdDgProjectionAssociation> OdDgProjectionAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgOriginAssociation : public OdDgAssocPointRoot
{
public:

  enum OdDgOriginAssocOption
  {
    kInsertionPoint = 0,
    kUpperLeftPoint = 1
  };
public:

  ODRX_DECLARE_MEMBERS(OdDgOriginAssociation);

  OdDgOriginAssociation();
  virtual ~OdDgOriginAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdDgOriginAssocOption getTextOriginOption() const;
  void     setTextOriginOption( OdDgOriginAssocOption uOption );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

private:

  OdDgOriginAssocOption m_uOption;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
};

typedef OdSmartPtr<OdDgOriginAssociation> OdDgOriginAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgIntersect2Association : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgIntersect2Association);

  OdDgIntersect2Association();
  virtual ~OdDgIntersect2Association();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt8  getIndex() const;
  void     setIndex( OdUInt8 uIndex );

  OdUInt8  getNSeg1() const;
  void     setNSeg1( OdUInt8 uNSeg );

  OdUInt8  getNSeg2() const;
  void     setNSeg2( OdUInt8 uNSeg );

  OdUInt16 getSegment1() const;
  void     setSegment1( OdUInt8 uSeg );

  OdUInt16 getSegment2() const;
  void     setSegment2( OdUInt8 uSeg );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getElement2Id() const;
  void     setElement2Id( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  OdUInt64 getRefAttachment2Id() const;
  void     setRefAttachment2Id( OdUInt64 uId );

private:

  OdUInt8  m_uIndex;
  OdUInt8  m_uNSeg1;
  OdUInt8  m_uNSeg2;
  OdUInt16 m_uSeg1;
  OdUInt16 m_uSeg2;
  OdUInt64 m_element1Id;
  OdUInt64 m_uRefAttach1Id;
  OdUInt64 m_element2Id;
  OdUInt64 m_uRefAttach2Id;
};

typedef OdSmartPtr<OdDgIntersect2Association> OdDgIntersect2AssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshVertexAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgMeshVertexAssociation);

  OdDgMeshVertexAssociation();
  virtual ~OdDgMeshVertexAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt32 getVertexIndex() const;
  void     setVertexIndex( OdUInt32 uVertex );

  OdUInt32 getNVertices() const;
  void     setNVertices( OdUInt32 nVertices );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

private:

  OdUInt32 m_uVertex;
  OdUInt32 m_nVertices;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
};

typedef OdSmartPtr<OdDgMeshVertexAssociation> OdDgMeshVertexAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgMeshEdgeAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgMeshEdgeAssociation);

  OdDgMeshEdgeAssociation();
  virtual ~OdDgMeshEdgeAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt32 getEdgeIndex() const;
  void     setEdgeIndex( OdUInt32 uEdge );

  OdUInt32 getNEdges() const;
  void     setNEdges( OdUInt32 nEdges );

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getUParam() const;
  void     setUParam( double dParam );

private:

  OdUInt32 m_uEdge;
  OdUInt32 m_nEdges;
  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dParam;
};

typedef OdSmartPtr<OdDgMeshEdgeAssociation> OdDgMeshEdgeAssociationPtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBSplineSurfaceAssociation : public OdDgAssocPointRoot
{
public:

  ODRX_DECLARE_MEMBERS(OdDgBSplineSurfaceAssociation);

  OdDgBSplineSurfaceAssociation();
  virtual ~OdDgBSplineSurfaceAssociation();

  virtual OdDgAssocPointRoot::OdDgAssocPointRootType getType() const;

  OdUInt64 getElementId() const;
  void     setElementId( OdUInt64 uId );

  OdUInt64 getRefAttachmentId() const;
  void     setRefAttachmentId( OdUInt64 uId );

  double   getUParam() const;
  void     setUParam( double dParam );

  double   getVParam() const;
  void     setVParam( double dParam );

private:

  OdUInt64 m_elementId;
  OdUInt64 m_uRefAttachId;
  double   m_dUParam;
  double   m_dVParam;
};

typedef OdSmartPtr<OdDgBSplineSurfaceAssociation> OdDgBSplineSurfaceAssociationPtr;

//---------------------------------------------------------------------

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDepLinkageAssocPoint : public OdDgDependencyLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDepLinkageAssocPoint);

  virtual OdUInt32 getCount() const = 0;
  virtual OdDgAssocPointRootPtr getAt( OdUInt32 index ) const = 0;
  virtual void setAt( OdUInt32 index, const OdDgAssocPointRootPtr pData ) = 0;
  virtual void deleteAt( OdUInt32 index ) = 0;
  virtual void add( const OdDgAssocPointRootPtr pData ) = 0;
};

typedef OdSmartPtr<OdDgDepLinkageAssocPoint> OdDgDepLinkageAssocPointPtr;

//---------------------------------------------------------------------

struct OdDgAssocPointIData
{
  OdDgAssocPointRootPtr m_assocPointData;
  OdInt32               m_iParam1;
  OdInt32               m_iParam2;
};

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDepLinkageAssocPointI : public OdDgDependencyLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDepLinkageAssocPointI);

  virtual OdUInt32 getCount() const = 0;
  virtual OdDgAssocPointIData getAt( OdUInt32 index ) const = 0;
  virtual void add( const OdDgAssocPointIData& data ) = 0;
  virtual void setAt( OdUInt32 index, const OdDgAssocPointIData data ) = 0;
  virtual void deleteAt( OdUInt32 index ) = 0;
};

typedef OdSmartPtr<OdDgDepLinkageAssocPointI> OdDgDepLinkageAssocPointIPtr;

//---------------------------------------------------------------------

struct OdDgFarElementIdData
{
  OdUInt64  m_elementId;
  OdUInt64  m_referenceAttachId;
};
/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDepLinkageFarElementId : public OdDgDependencyLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDepLinkageFarElementId);

  virtual OdUInt32 getCount() const = 0;
  virtual OdDgFarElementIdData getAt( OdUInt32 index ) const = 0;
  virtual void setAt( OdUInt32 index, const OdDgFarElementIdData& data ) = 0;
  virtual void deleteAt( OdUInt32 index ) = 0;
  virtual void add( const OdDgFarElementIdData& data ) = 0;
};

typedef OdSmartPtr<OdDgDepLinkageFarElementId> OdDgDepLinkageFarElementIdPtr;

//---------------------------------------------------------------------

struct OdDgFarElementIdVData
{
  OdUInt64  m_elementId;
  double    m_dParam;
  OdUInt64  m_referenceAttachId;
};

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDepLinkageFarElementIdV : public OdDgDependencyLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDepLinkageFarElementIdV);

  virtual OdUInt32 getCount() const = 0;
  virtual OdDgFarElementIdVData getAt( OdUInt32 index ) const = 0;
  virtual void setAt( OdUInt32 index, const OdDgFarElementIdVData& data ) = 0;
  virtual void deleteAt( OdUInt32 index ) = 0;
  virtual void add( const OdDgFarElementIdVData& data ) = 0;
};

typedef OdSmartPtr<OdDgDepLinkageFarElementIdV> OdDgDepLinkageFarElementIdVPtr;

//---------------------------------------------------------------------

struct OdDgDependencyPathData
{
  double            m_dParam;
  OdArray<OdUInt64> m_referenceAttachPath;
};

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgDepLinkagePath : public OdDgDependencyLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgDepLinkagePath);

  virtual OdUInt32 getCount() const = 0;
  virtual OdDgDependencyPathData getAt( OdUInt32 index ) const = 0;
  virtual void setAt( OdUInt32 index, const OdDgDependencyPathData& data ) = 0;
  virtual void deleteAt( OdUInt32 index ) = 0;
  virtual void add( const OdDgDependencyPathData& data ) = 0;
};

typedef OdSmartPtr<OdDgDepLinkagePath> OdDgDepLinkagePathPtr;

//---------------------------------------------------------------------

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgBrepLinkage object pointers.
*/
class OdDgBrepLinkage;
typedef OdSmartPtr<OdDgBrepLinkage> OdDgBrepLinkagePtr;


/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgBrepLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgBrepLinkage);

  static OdDgBrepLinkagePtr createObject(OdUInt16 f1, OdUInt16 f2, double dScale);

  virtual OdUInt16 getFlag1() const = 0;
  virtual OdUInt16 getFlag2() const = 0;
  virtual double getScale() const = 0;
  virtual void setFlag1(OdUInt16 val) = 0;
  virtual void setFlag2(OdUInt16 val) = 0;
  virtual void setScale(double dScale) = 0;


};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgProxyLinkage object pointers.
*/
class OdDgProxyLinkage;
typedef OdSmartPtr<OdDgProxyLinkage> OdDgProxyLinkagePtr;

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgProxyLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgProxyLinkage);

  static OdDgProxyLinkagePtr createObject(OdUInt16 appId);

  virtual void setData(const OdBinaryData& data) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgNoteLinkage object pointers.
*/
class OdDgNoteLinkage;
typedef OdSmartPtr<OdDgNoteLinkage> OdDgNoteLinkagePtr;

/** \details

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgNoteLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgNoteLinkage);

  virtual bool setData(const OdBinaryData& data) = 0;
  virtual bool readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;
};

/** \details
   Corresponding C++ library: TG_Db
   <group OdDg_Classes>
*/
class TG_EXPORT OdDgSeedPointsLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgSeedPointsLinkage );

  virtual OdUInt32 getPointsCount() const = 0;
  virtual void     readData( const OdUInt8* pData, OdUInt32 dataSize ) = 0;

  virtual OdGePoint3d getPointAt( OdUInt32 uIndex ) const = 0;
  virtual void        setPointAt( OdUInt32 uIndex, const OdGePoint3d& ptPoint ) = 0;
  virtual void        addPoint( const OdGePoint3d& ptPoint ) = 0;
  virtual void        deletePoint( OdUInt32 uIndex ) = 0;
};

typedef OdSmartPtr<OdDgSeedPointsLinkage> OdDgSeedPointsLinkagePtr;

/** \details

Common descriptor for all types of pattern attribute linkages

\remarks
Patterns of different types have the fields:
1. Obligatory fields (like the space for the linear patterns). These fields are active always.
2. Optional fields (like the tolerance). These fields are not active before their value is set or their 'use' flag is set 
   (setTolerance() & setUseToleranceFlag() for the tolerance field). 

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgPatternLinkage;
typedef OdSmartPtr<OdDgPatternLinkage> OdDgPatternLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgPatternLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPatternLinkage );

  //
  // Type declarations
  //

  //types of patterns
  enum PatternType
  {
    kLinearPattern,
    kCrossPattern,
    kSymbolPattern,
    kDWGPattern,
    kProxyPattern
  };

  //
  // Manipulations with sub-types
  //

  virtual PatternType getType() const = 0;
  static OdDgPatternLinkagePtr createObject( PatternType type );

  //
  // Common features
  //

  // tolerance determine the minimal perimeter of hatched areas
  virtual double getTolerance() const = 0;
  virtual void setTolerance( double tolerance ) = 0;

  virtual bool getUseToleranceFlag() const = 0;
  virtual void setUseToleranceFlag( bool value ) = 0;

  // common rotation
  virtual OdGeQuaternion getRotation() const = 0;
  virtual void setRotation( OdGeQuaternion const& rotation ) = 0;

  //these methods should be used if the pattern is own by a 2d element
  void getRotation2d( OdGeMatrix2d& matrix ) const;
  void setRotation2d( const OdGeMatrix2d& matrix );

  virtual bool getUseRotationFlag() const = 0;
  virtual void setUseRotationFlag( bool value ) = 0;

  //used for multiline, determines the low and high lines to set the hatch between
  virtual OdUInt32 getLowMultilineIndex() const = 0;
  virtual void setLowMultilineIndex( OdUInt32 index ) = 0;
  virtual OdUInt32 getHighMultilineIndex() const = 0;
  virtual void setHighMultilineIndex( OdUInt32 index ) = 0;

  virtual bool getUseMultilineIndicesFlag() const = 0;
  virtual void setUseMultilineIndicesFlag( bool value ) = 0;

  //it determines the base for all hatches and allows to set its offset
  virtual void getOffset( OdGePoint3d& offset ) const = 0;
  virtual void setOffset( const OdGePoint3d& offset ) = 0;

  virtual bool getUseOffsetFlag() const = 0;
  virtual void setUseOffsetFlag( bool value ) = 0;

  // flags
  virtual bool getSnappableFlag() const = 0;
  virtual void setSnappableFlag( bool value ) = 0;
};

/** \details

Linear pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgLinearPatternLinkage;
typedef OdSmartPtr<OdDgLinearPatternLinkage> OdDgLinearPatternLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgLinearPatternLinkage : public OdDgPatternLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPatternLinkage );

  // geometry
  virtual double getSpace() const = 0;
  virtual void setSpace( double space ) = 0;

  virtual double getAngle() const = 0;
  virtual void setAngle( double angle ) = 0;

  // appearance
  virtual OdUInt32 getLineStyleEntryId() const = 0;
  virtual void setLineStyleEntryId( OdUInt32 id ) = 0;

  virtual bool getUseLineStyleFlag() const = 0;
  virtual void setUseLineStyleFlag( bool value ) = 0;

  virtual OdUInt32 getLineWeight() const = 0;
  virtual void setLineWeight( OdUInt32 weight ) = 0;

  virtual bool getUseLineWeightFlag() const = 0;
  virtual void setUseLineWeightFlag( bool value ) = 0;

  virtual OdUInt32 getLineColorIndex() const = 0;
  virtual void setLineColorIndex( OdUInt32 colorIndex ) = 0;

  virtual bool getUseLineColorFlag() const = 0;
  virtual void setUseLineColorFlag( bool value ) = 0;
};

/** \details

Cross pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgCrossPatternLinkage;
typedef OdSmartPtr<OdDgCrossPatternLinkage> OdDgCrossPatternLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgCrossPatternLinkage : public OdDgPatternLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPatternLinkage );

  // geometry
  virtual double getSpace1() const = 0;
  virtual void setSpace1( double space ) = 0;

  virtual double getAngle1() const = 0;
  virtual void setAngle1( double angle ) = 0;

  virtual double getSpace2() const = 0;
  virtual void setSpace2( double space ) = 0;

  virtual double getAngle2() const = 0;
  virtual void setAngle2( double angle ) = 0;

  // appearance
  virtual OdUInt32 getLineStyleEntryId() const = 0;
  virtual void setLineStyleEntryId( OdUInt32 id ) = 0;

  virtual bool getUseLineStyleFlag() const = 0;
  virtual void setUseLineStyleFlag( bool value ) = 0;

  virtual OdUInt32 getLineWeight() const = 0;
  virtual void setLineWeight( OdUInt32 weight ) = 0;

  virtual bool getUseLineWeightFlag() const = 0;
  virtual void setUseLineWeightFlag( bool value ) = 0;

  virtual OdUInt32 getLineColorIndex() const = 0;
  virtual void setLineColorIndex( OdUInt32 colorIndex ) = 0;

  virtual bool getUseLineColorFlag() const = 0;
  virtual void setUseLineColorFlag( bool value ) = 0;
};


/** \details

Pattern with a symbol (shared cells are used here)

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgSymbolPatternLinkage;
typedef OdSmartPtr<OdDgSymbolPatternLinkage> OdDgSymbolPatternLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgSymbolPatternLinkage : public OdDgPatternLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPatternLinkage );

  // geometry
  virtual double getSpace1() const = 0;
  virtual void setSpace1( double space ) = 0;

  virtual double getAngle1() const = 0;
  virtual void setAngle1( double angle ) = 0;

  virtual double getSpace2() const = 0;
  virtual void setSpace2( double space ) = 0;

  virtual double getAngle2() const = 0;
  virtual void setAngle2( double angle ) = 0;

  virtual double getScale() const = 0;
  virtual void setScale( double scale ) = 0;

  // flags
  virtual bool getTrueScaleFlag() const = 0;
  virtual void setTrueScaleFlag( bool value ) = 0;

  virtual OdUInt32 getColorIndex() const = 0;
  virtual void setColorIndex( OdUInt32 colorIndex ) = 0;

  virtual bool getUseColorFlag() const = 0;
  virtual void setUseColorFlag( bool value ) = 0;

  virtual OdUInt32 getLineStyleEntryId() const = 0;
  virtual void setLineStyleEntryId( OdUInt32 id ) = 0;

  virtual bool getUseLineStyleFlag() const = 0;
  virtual void setUseLineStyleFlag( bool value ) = 0;

  virtual OdUInt32 getLineWeight() const = 0;
  virtual void setLineWeight( OdUInt32 weight ) = 0;

  virtual bool getUseLineWeightFlag() const = 0;
  virtual void setUseLineWeightFlag( bool value ) = 0;
};



class OdDgProxyPatternLinkage;
typedef OdSmartPtr<OdDgProxyPatternLinkage> OdDgProxyPatternLinkagePtr;

/** \details

Proxy pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgProxyPatternLinkage: public OdDgPatternLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgPatternLinkage );
  // tolerance determine the minimal perimeter of hatched areas
    virtual double getSpace1() const = 0;
    virtual void setSpace1( double ) = 0;
    virtual double getAngle1() const = 0;
    virtual void setAngle1( double ) = 0;
    virtual double getSpace2() const = 0;
    virtual void setSpace2( double ) = 0;
    virtual double getAngle2() const = 0;
    virtual void setAngle2( double ) = 0;
    virtual double getScale() const = 0;
    virtual void setScale( double ) = 0;
    virtual double getTolerance() const = 0;
    virtual void setTolerance( double ) = 0;
    //virtual void setLineStyle( OdUInt32 );
    virtual OdUInt32 getLineWeight() const = 0;
    virtual void setLineWeight( OdUInt32 ) = 0;
    //virtual void setLineColor( OdUInt32 );
    virtual OdGeQuaternion getRotation( ) const = 0;
    virtual void setRotation( OdGeQuaternion const& ) = 0;
    virtual void getOffset(OdGePoint3d& ) const = 0;
    virtual void setOffset( const OdGePoint3d& ) = 0;
    virtual OdUInt32 getMLineLowIndex() const = 0;
    virtual void setMLineLowIndex( OdUInt32 ) = 0;
    virtual OdUInt16 getHoleStyle() const = 0;
    virtual void setHoleStyle( OdUInt16 ) = 0;
    virtual OdUInt32 getMLineHighIndex() const = 0;
    virtual void setMLineHighIndex( OdUInt32 ) = 0;
    //virtual void setHoleStyle( OdUInt16 );
    //virtual void setDWGHatchDef( hatch );
    virtual double getPixelSize() const = 0;
    virtual void setPixelSize( double ) = 0;
    virtual OdUInt16 getIslandStyle() const = 0;
    virtual void setIslandStyle( OdUInt16 ) = 0;
    virtual bool getTrueScale() const = 0;
    virtual void setTrueScale(bool) = 0;
    //virtual OdGePoint3d getOrigin() const = 0;
    //virtual void setOrigin(const OdGePoint3d&) = 0;
};

/** \details

DWG-style pattern

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDWGPatternLinkage;
typedef OdSmartPtr<OdDgDWGPatternLinkage> OdDgDWGPatternLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDWGPatternLinkage : public OdDgPatternLinkage
{

public:
  ODRX_DECLARE_MEMBERS( OdDgPatternLinkage );

  //data for a single line of the .dwg file hatch
  struct TG_EXPORT DWGHatchLine
  {
    DWGHatchLine();
    bool operator ==(const DWGHatchLine& l) const
    {
      return m_angle == l.m_angle && m_throughPoint == l.m_throughPoint && m_offset == l.m_offset && m_dashes == l.m_dashes;
    }
    double m_angle;
    OdGePoint2d m_throughPoint;
    OdGePoint2d m_offset;
    OdDoubleArray m_dashes;
  };

  //complete data for the .dwg file hatch
  typedef OdArray< DWGHatchLine > DWGHatch;

  //geometry
  virtual DWGHatch const& getHatch() const = 0;
  virtual void setHatch( DWGHatch const& hatch ) = 0;
};

/** \details

The linkage to represent some similar DB linkages

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDBLinkage;
typedef OdSmartPtr<OdDgDBLinkage> OdDgDBLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDBLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDBLinkage );

  enum DBType
  {
    kBSI,
    kFRAMME,
    kInformix,
    kIngres,
    kODBC,
    kOLEDB,
    kOracle,
    kRIS,
    kSybase,
    kXbase
  };

  static OdDgDBLinkagePtr createObject( DBType type );

  //type of DB
  virtual DBType getDBType() const = 0;

  //the Id of the linked table that is stored within MSCATALOG table
  virtual OdUInt16 getTableEntityId() const = 0;
  virtual void setTableEntityId( OdUInt16 entityId ) = 0; 

  //the mslink from the linked table
  virtual OdUInt32 getMSLink() const = 0;
  virtual void setMSLink( OdUInt32 msLink ) = 0;

};

/** \details
    Byte Array Linkage.

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgByteArrayLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgByteArrayLinkage);

  virtual OdUInt16 getArrayId() const = 0;
  virtual void setArrayId(OdUInt16 val) = 0;

  virtual void getArrayData(OdBinaryData& data) const = 0;
  virtual void setArrayData(const OdBinaryData& data) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgByteArrayLinkage object pointers.
*/
typedef OdSmartPtr<OdDgByteArrayLinkage> OdDgByteArrayLinkagePtr;

/** \details
Level Filter Member Linkage.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgFilterMemberLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgFilterMemberLinkage);

  virtual OdUInt32 getMemberId() const = 0;
  virtual void     setMemberId( OdUInt32 uId )  = 0;

  virtual OdDgLevelFilterTable::OdDgFilterMemberType getMemberType() const = 0;
  virtual void     setMemberType( OdDgLevelFilterTable::OdDgFilterMemberType uType ) = 0;

  virtual OdString getNameString() const = 0;
  virtual void     setNameString( const OdString& strName ) = 0;

  virtual OdString getExpressionString() const = 0;
  virtual void     setExpressionString( const OdString& strExpression ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgFilterMemberLinkage object pointers.
*/
typedef OdSmartPtr<OdDgFilterMemberLinkage> OdDgFilterMemberLinkagePtr;


class OdDgFillColorLinkage;
typedef OdSmartPtr<OdDgFillColorLinkage> OdDgFillColorLinkagePtr;

/** \details

This linkage defines a fill color for closed graphics objects

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgFillColorLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgFillColorLinkage );

  //the color index
  //see OdDgColorTable for details about indices & RGB color
  virtual OdUInt32 getColorIndex() const = 0;
  virtual void setColorIndex( OdUInt32 colorIndex ) = 0;

};

class OdDgGradientFillLinkage;
typedef OdSmartPtr<OdDgGradientFillLinkage> OdDgGradientFillLinkagePtr;

/** \details

This linkage defines a gradient fill for closed graphics objects

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class TG_EXPORT OdDgGradientFillLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgGradientFillLinkage );

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual double getAngle() const = 0;
  virtual void   setAngle( double dAngle ) = 0;
  virtual double getWhiteIntensity() const = 0;
  virtual void   setWhiteIntensity( double dIntencity ) = 0;
  virtual OdDgGradientFill::OdDgGradientType getGradientType() const = 0;
  virtual void   setGradientType( OdDgGradientFill::OdDgGradientType iType ) = 0;
  virtual bool   getInvertFlag() const = 0;
  virtual void   setInvertFlag( bool bSet ) = 0;
  virtual bool   getOutlineFlag() const = 0;
  virtual void   setOutlineFlag( bool bSet ) = 0;

  virtual OdUInt16        getKeyCount() const = 0;
  virtual OdDgGradientFill::OdDgGradientKey getKey( OdUInt16 uKeyNumber ) = 0;
  virtual bool            setKeyColor( OdUInt16 uKeyNumber, ODCOLORREF clrKey ) = 0;
  virtual bool            addKey( const OdDgGradientFill::OdDgGradientKey& key ) = 0;
  virtual bool            deleteKey( OdUInt16 uKeyNumber ) = 0;

  virtual OdDgGradientFill getGradientFill() const  = 0;
  virtual void             setGradientFill( const OdDgGradientFill& gradFill ) = 0;
};

class OdDgInternalMaterialLinkage;
typedef OdSmartPtr<OdDgInternalMaterialLinkage> OdDgInternalMaterialLinkagePtr;

/** \details

This linkage defines a fill color for closed graphics objects

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgInternalMaterialLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgInternalMaterialLinkage );

  //the id of material table record
  virtual OdUInt64 getMaterialTableRecordId() const = 0;
  virtual void setMaterialTableRecordId( OdUInt64 idMaterialTableRecord ) = 0;

  virtual OdDg::OdDgMaterialType getMaterialType() const = 0;
  virtual void setMaterialType( OdDg::OdDgMaterialType uType ) = 0;
};

class OdDgTransparencyLinkage;
typedef OdSmartPtr<OdDgTransparencyLinkage> OdDgTransparencyLinkagePtr;

/** \details

This linkage defines a fill color for closed graphics objects

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgTransparencyLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgTransparencyLinkage );

  //the transparency
  virtual double getTransparency() const = 0;
  virtual void setTransparency( double dTransparency ) = 0;

};

/** \details
The DMRS linkage (old-style DB linkage)
\remarks
It is non-DMRS linkage, so it does not have primaryId, getPrimaryId() returns 0

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDMRSLinkage;
typedef OdSmartPtr<OdDgDMRSLinkage> OdDgDMRSLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDMRSLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDMRSLinkage );

  //Id of a table within MSCATALOG table
  virtual OdUInt16 getTableId() const = 0;
  virtual void setTableId( OdUInt16 tableId ) = 0;

  //MSLink
  virtual OdUInt32 getMSLink() const = 0;
  virtual void setMSLink( OdUInt32 MSLink ) = 0;

  //typedef
  virtual OdUInt8 getType() const = 0;
  virtual void setType( OdUInt8 type ) = 0;
  
  // Some files have DMRS of old style with reversed fields.
  // If so, this method should be called before any read/create operations on DMRS linkages
  static void setReversedMode( bool setReversed = true );
  static bool getReversedMode();

};

/** \details

Common descriptor for all types of dimension attribute linkages

\remarks
Dimension data of different types have the purpose:
0. Overall subtype (is used to store properties that affect to entire dimension element). 
   These fields are not active before their value is set or their 'use' flag is set.
1. Segment subtype (is used to store properties that can be controlled for a particular
   dimension segment ). These fields are not active before their value is set or their 'use' flag is set.
2. Point subtype (is used to store properties that can be controlled for a particular dimension point). 
   These fields are not active before their value is set or their 'use' flag is set.
3. Segment Flags subtype (is used to store simple flags related to a particular dimension segment ).
   These fields are active always.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionLinkage;
typedef OdSmartPtr<OdDgDimensionLinkage> OdDgDimensionLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  //
  // Type declarations
  //

  //types of patterns
  enum DimensionSubType
  {
    kOverall = 0,
    kSegment = 1,
    kPoint   = 2,
    kSegmentFlags = 3,
    kRefValue = 4,
    kTextItem = 5,
    kDimensionInfo = 6
  };

  //
  // Manipulations with sub-types
  //

  virtual DimensionSubType getType() const = 0;
  static OdDgDimensionLinkagePtr createObject( DimensionSubType type );
};

/** \details

Dimension extension overall linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionOverallLinkage;
typedef OdSmartPtr<OdDgDimensionOverallLinkage> OdDgDimensionOverallLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionOverallLinkage : public OdDgDimensionLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  virtual bool getUseRefScaleFlag() const = 0;
  virtual void setUseRefScaleFlag( bool bSetFlag ) = 0;
  virtual bool getUseAngleQuadrantFlag() const = 0;
  virtual void setUseAngleQuadrantFlag( bool bSetFlag ) = 0;
  virtual bool getUseSlantAngleFlag() const = 0;
  virtual void setUseSlantAngleFlag( bool bSetFlag ) = 0;

  virtual double getRefScale() const = 0;
  virtual void   setRefScale( double dRefScale ) = 0;
  virtual OdUInt16 getAngleQuadrant() const = 0;
  virtual void   setAngleQuadrant( OdUInt16 iAngleQuadrant ) = 0;
  virtual double getSlantAngle() const = 0;
  virtual void   setSlantAngle( double dSlantAngle ) = 0;
  virtual OdUInt32 getFlags() const = 0;

  virtual bool   getUseAnnotationScaleValueFlag() const = 0;
  virtual void   setUseAnnotationScaleValueFlag( bool bSetFlag ) = 0;
  virtual double getAnnotationScaleValue() const = 0;
  virtual void   setAnnotationScaleValue( double dRefScale ) = 0;

  virtual void        copy( const OdDgDimensionOverallLinkagePtr pSource ) = 0;
  virtual void        copy( const OdDgDimensionOverallLinkage& source ) = 0;
};

/** \details

Dimension extension segment linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionSegmentLinkage;
typedef OdSmartPtr<OdDgDimensionSegmentLinkage> OdDgDimensionSegmentLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionSegmentLinkage : public OdDgDimensionLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  virtual bool getUseTextRotationFlag() const = 0;
  virtual void setUseTextRotationFlag( bool bSetFlag ) = 0;
  virtual bool getUseTextJustFlag() const = 0;
  virtual void setUseTextJustFlag( bool bSetFlag ) = 0;

  virtual double getTextRotation() const = 0;
  virtual void   setTextRotation( double dTextRotation ) = 0;
  virtual OdUInt32 getTextJust() const = 0;
  virtual void   setTextJust( OdUInt32 iTextJust ) = 0;
  virtual OdUInt32 getFlags() const = 0;
};

/** \details

Dimension extension point linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionPointLinkage;
typedef OdSmartPtr<OdDgDimensionPointLinkage> OdDgDimensionPointLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionPointLinkage : public OdDgDimensionLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  virtual bool getUseWitColorFlag() const = 0;
  virtual void setUseWitColorFlag( bool bSetFlag ) = 0;
  virtual bool getUseWitWeightFlag() const = 0;
  virtual void setUseWitWeightFlag( bool bSetFlag ) = 0;
  virtual bool getUseWitStyleFlag() const = 0;
  virtual void setUseWitStyleFlag( bool bSetFlag ) = 0;
  virtual bool getUseWitExtendFlag() const = 0;
  virtual void setUseWitExtendFlag( bool bSetFlag ) = 0;

  virtual OdUInt32 getWitnessColor() const = 0;
  virtual void   setWitnessColor( OdUInt32 lColor ) = 0;
  virtual OdUInt32 getWitnessWeight() const = 0;
  virtual void   setWitnessWeight( OdUInt32 lWeight ) = 0;
  virtual OdUInt32 getWitnessStyle() const = 0;
  virtual void   setWitnessStyle( OdUInt32 lStyle ) = 0;
  virtual double getWitnessExtension() const = 0;
  virtual void   setWitnessExtension( double dExtension ) = 0;
  virtual OdUInt32 getFlags() const = 0;
};

/** \details

Dimension extension ref value linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionRefValueLinkage;
typedef OdSmartPtr<OdDgDimensionRefValueLinkage> OdDgDimensionRefValueLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionRefValueLinkage : public OdDgDimensionLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  virtual double getRefValue() const = 0;
  virtual void   setRefValue( double dValue ) = 0;
};

/** \details

Dimension extension segment flags linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionSegmentFlagsLinkage;
typedef OdSmartPtr<OdDgDimensionSegmentFlagsLinkage> OdDgDimensionSegmentFlagsLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionSegmentFlagsLinkage : public OdDgDimensionLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  virtual bool getUseUnderlineTextFlag() const = 0;
  virtual void setUseUnderlineTextFlag( bool bSetFlag ) = 0;
  virtual bool getUseSuppressLeftDimLineFlag() const = 0;
  virtual void setUseSuppressLeftDimLineFlag( bool bSetFlag ) = 0;
  virtual bool getUseSuppressRightDimLineFlag() const = 0;
  virtual void setUseSuppressRightDimLineFlag( bool bSetFlag ) = 0;
  virtual bool getUsePrimaryReferenceFlag() const = 0;
  virtual void setUsePrimaryReferenceFlag( bool bSetFlag ) = 0;
  virtual bool getUseSecondaryReferenceFlag() const = 0;
  virtual void setUseSecondaryReferenceFlag( bool bSetFlag ) = 0;
  virtual OdUInt32 getFlags() const = 0;
};

/** \details

Dimension information linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionInfoLinkage;
typedef OdSmartPtr<OdDgDimensionInfoLinkage> OdDgDimensionInfoLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionInfoLinkage : public OdDgDimensionLinkage
{
public:

  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  virtual bool getUsePrimaryToleranceAccuracy() const = 0;
  virtual bool getUseSecondaryToleranceAccuracy() const = 0;
  virtual bool getUseDatumValue() const = 0;
  virtual bool getUseDecrimentInReverceDirection() const = 0;
  virtual bool getUseFreeLocationText() const = 0;
  virtual bool getUseRetainFractionalAccuracy() const = 0;
  virtual bool getUseStackedFractionScale() const = 0;
  virtual bool getUseTextLocation() const = 0;
  virtual bool getUseAnnotationScale() const = 0;
  virtual bool getUseMinLeaderTermLength() const = 0;
  virtual bool getUseInlineLeaderLengthValue() const = 0;
  virtual bool getUseFitOptionsFlag() const = 0;
  virtual bool getUseSuppressUnfitTerm() const = 0;
  virtual bool getUseInlineLeaderLength() const = 0;
  virtual bool getUseTextAboveOptimalFit() const = 0;
  virtual bool getUseNarrowFontOptimalFit() const = 0;
  virtual bool getUseNoteLeaderLength() const = 0;
  virtual bool getUseNoteLeftMargin() const = 0;
  virtual bool getUseNoteLowerMargin() const = 0;
  virtual bool getUseNoteFrameScale() const = 0;
  virtual bool getUseInlineTextLift() const = 0;
  virtual bool getUseNoteSplineFit() const = 0;
  virtual bool getPrimaryRetainFractionalAccuracy() const = 0;
  virtual bool getSecondaryRetainFractionalAccuracy() const = 0;
  virtual bool getPrimaryAltFormatRetainFractionalAccuracy() const = 0;
  virtual bool getSecondaryAltFormatRetainFractionalAccuracy() const = 0;
  virtual bool getPrimaryTolerRetainFractionalAccuracy() const = 0;
  virtual bool getSecondaryTolerRetainFractionalAccuracy() const = 0;

  // RAV: Change name and description when information will be found.
  virtual bool getUseUnknownUInt16() const = 0;
  virtual void setUseUnknownUInt16( bool bSet ) = 0;
  virtual OdUInt16 getUnknownUInt16() const = 0;
  virtual void setUnknownUInt16( OdUInt16 dValue) = 0;
  //

  virtual void setUsePrimaryToleranceAccuracy( bool bSetFlag ) = 0;
  virtual void setUseSecondaryToleranceAccuracy( bool bSetFlag ) = 0;
  virtual void setUseDatumValue( bool bSetFlag ) = 0;
  virtual void setUseDecrimentInReverceDirection( bool bSetFlag ) = 0;
  virtual void setUseFreeLocationText( bool bSetFlag ) = 0;
  virtual void setUseRetainFractionalAccuracy( bool bSetFlag ) = 0;
  virtual void setUseStackedFractionScale( bool bSetFlag ) = 0;
  virtual void setUseTextLocation( bool bSetFlag ) = 0;
  virtual void setUseAnnotationScale( bool bSetFlag ) = 0;
  virtual void setUseMinLeaderTermLength( bool bSetFlag ) = 0;
  virtual void setUseInlineLeaderLengthValue( bool bSetFlag ) = 0;
  virtual void setUseFitOptions( bool bSetFlag ) = 0;
  virtual void setUseSuppressUnfitTerm( bool bSetFlag ) = 0;
  virtual void setUseInlineLeaderLength( bool bSetFlag ) = 0;
  virtual void setUseTextAboveOptimalFit( bool bSetFlag ) = 0;
  virtual void setUseNarrowFontOptimalFit( bool bSetFlag ) = 0;
  virtual void setUseNoteLeaderLength( bool bSetFlag ) = 0;
  virtual void setUseNoteLeftMargin( bool bSetFlag ) = 0;
  virtual void setUseNoteLowerMargin( bool bSetFlag ) = 0;
  virtual void setUseNoteFrameScale( bool bSetFlag ) = 0;
  virtual void setUseInlineTextLift( bool bSetFlag ) = 0;
  virtual void setUseNoteSplineFit( bool bSetFlag ) = 0;
  virtual void setPrimaryRetainFractionalAccuracy( bool bSetFlag ) = 0;
  virtual void setSecondaryRetainFractionalAccuracy( bool bSetFlag ) = 0;
  virtual void setPrimaryAltFormatRetainFractionalAccuracy( bool bSetFlag ) = 0;
  virtual void setSecondaryAltFormatRetainFractionalAccuracy( bool bSetFlag ) = 0;
  virtual void setPrimaryTolerRetainFractionalAccuracy( bool bSetFlag ) = 0;
  virtual void setSecondaryTolerRetainFractionalAccuracy( bool bSetFlag ) = 0;

  virtual OdDgDimTextFormat::Accuracy getPrimaryToleranceAccuracy() const = 0;
  virtual void   setPrimaryToleranceAccuracy( const OdDgDimTextFormat::Accuracy iAccuracy ) = 0;
  virtual OdDgDimTextFormat::Accuracy getSecondaryToleranceAccuracy() const = 0;
  virtual void   setSecondaryToleranceAccuracy( const OdDgDimTextFormat::Accuracy iAccuracy ) = 0;
  virtual double getStackedFractionScale() const = 0;
  virtual void setStackedFractionScale( const double dScale ) = 0;
  virtual OdDgDimTextInfo::TextLocation getTextLocation() const = 0;
  virtual void setTextLocation( const OdDgDimTextInfo::TextLocation iLocation ) = 0;
  virtual double getAnnotationScale() const = 0;
  virtual void setAnnotationScale( const double dScale ) = 0;
  virtual double getDatumValue() const = 0;
  virtual void  setDatumValue( const double dValue ) = 0;
  virtual double getInlineLeaderLength() const = 0;
  virtual void setInlineLeaderLength( const double dLength ) = 0;
  virtual OdDgDimTextInfo::FitOptions getFitOptions() const = 0;
  virtual void setFitOptions( const OdDgDimTextInfo::FitOptions iFitOptions ) = 0;
  virtual OdDgDimLabelLine::LabelLineDimensionMode  getLabelLineDimensionMode() const = 0;
  virtual void setLabelLineDimensionMode( const OdDgDimLabelLine::LabelLineDimensionMode iDimMode ) = 0;
  virtual OdUInt32 getFlags() const = 0;
  virtual double getNoteLeaderLength() const = 0;
  virtual void  setNoteLeaderLength( const double dLength ) = 0;
  virtual double getNoteLeftMargin() const = 0;
  virtual void  setNoteLeftMargin( const double dMargin ) = 0;
  virtual double getNoteLowerMargin() const = 0;
  virtual void  setNoteLowerMargin( const double dMargin ) = 0;
  virtual double getNoteFrameScale() const = 0;
  virtual void  setNoteFrameScale( const double dScale ) = 0;
  virtual double getInlineTextLift() const = 0;
  virtual void  setInlineTextLift( const double dMargin ) = 0;
  virtual OdDgDimTool::TerminatorType getNoteTerminator() const = 0;
  virtual void setNoteTerminator( const OdDgDimTool::TerminatorType iType ) = 0;
  virtual OdDgDimTextInfo::TextRotation getNoteTextRotation() const = 0;
  virtual void setNoteTextRotation( const OdDgDimTextInfo::TextRotation iRotation ) = 0;

  // For linkage data read/write
  virtual OdInt16 getRetainFractionalAccuracyValue() const = 0;
  virtual OdInt32 getTextModeValue() const = 0;
  virtual OdInt32 getFitOptionsValue() const = 0;
  virtual void    setRetainFractionalAccuracyValue( OdInt16 iValue ) = 0;
  virtual void    setTextModeValue( OdInt32 iValue ) = 0;
  virtual void    setFitOptionsValue( OdInt32 iValue ) = 0;
  virtual bool    isEmptyLinkage()const = 0;
  //virtual bool    isDatumValueUsed() const = 0;
  virtual void    copy( const OdDgDimensionInfoLinkagePtr pSource ) = 0;
  virtual void    copy( const OdDgDimensionInfoLinkage& source ) = 0;

  // Only for dimension style
  virtual bool    getUseTextCenterRightJustification() const = 0;     
  virtual void    setUseTextCenterRightJustification( bool bSetFlag ) = 0;
  //
  virtual bool    getUsePrimaryRoundOff() const = 0;
  virtual bool    getUseSecondaryRoundOff() const = 0;
  virtual bool    getUseAutoLeaderMode() const = 0;
  virtual void    setUsePrimaryRoundOff( bool bSetFlag ) = 0;
  virtual void    setUseSecondaryRoundOff( bool bSetFlag ) = 0;
  virtual void    setUseAutoLeaderMode( bool bSetFlag ) = 0;
  virtual double  getPrimaryRoundOff() const = 0;
  virtual void    setPrimaryRoundOff( const double dOffset ) = 0;
  virtual double  getSecondaryRoundOff() const = 0;
  virtual void    setSecondaryRoundOff( const double dOffset ) = 0;
  virtual OdDgDimNote::NoteFrameType  getNoteFrameType() const = 0;
  virtual void    setNoteFrameType( const OdDgDimNote::NoteFrameType iFrameType ) = 0;
  virtual bool    getEnableNoteFrameScale() const = 0;
  virtual void    setEnableNoteFrameScale( bool bSetFlag ) = 0;
  virtual OdDgDimNote::NoteAttachmentDirection  getNoteHorizontalAttachmentLeaderDirection() const = 0;
  virtual void    setNoteHorizontalAttachmentLeaderDirection( const OdDgDimNote::NoteAttachmentDirection iDirection ) = 0;
  virtual OdDgDimNote::NoteEditAbout  getNoteEditAbout() const = 0;
  virtual void    setNoteEditAbout( const OdDgDimNote::NoteEditAbout iEditAbout ) = 0;
  virtual OdDgDimNote::NoteAttachmentAlignment  getNoteLeftAttachmentAlignment() const = 0;
  virtual void    setNoteLeftAttachmentAlignment( const OdDgDimNote::NoteAttachmentAlignment iAlignment ) = 0;
  virtual OdDgDimNote::NoteAttachmentAlignment  getNoteRightAttachmentAlignment() const = 0;
  virtual void    setNoteRightAttachmentAlignment( const OdDgDimNote::NoteAttachmentAlignment iAlignment ) = 0;
  virtual bool    getUseExtendDimLineUnderText() const = 0;
  virtual void    setUseExtendDimLineUnderText( bool bSetFlag ) = 0;
};

/** \details

Dimension text item linkage for multiline dimension text

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgDimensionTextItemLinkage;
typedef OdSmartPtr<OdDgDimensionTextItemLinkage> OdDgDimensionTextItemLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionTextItemLinkage : public OdDgDimensionLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionLinkage );

  virtual OdUInt32    getDimensionSegmentNumber() const = 0;
  virtual void        setDimensionSegmentNumber( const OdUInt32 iNumber ) = 0;
  virtual OdUInt32    getExtensionDataFlag() const = 0;
  virtual void        setExtensionDataFlag( const OdUInt32 iFlag ) = 0;
  virtual OdUInt32    getFontEntryId() const = 0;
  virtual void        setFontEntryId( const OdUInt32 uFontId ) = 0;
  virtual OdGePoint2d getOriginOffset() const = 0;
  virtual void        setOriginOffset(const OdGePoint2d ptOffset) = 0;
  virtual double      getLength() const = 0;
  virtual void        setLength( const double dLength ) = 0;
  virtual double      getHeight() const = 0;
  virtual void        setHeight( const double dHeight ) = 0;
  virtual double      getWidthFactor() const = 0;
  virtual void        setWidthFactor( const double dWidthFactor ) = 0;
  virtual double      getHeightFactor() const = 0;
  virtual void        setHeightFactor( const double dHeightFactor ) = 0;
  virtual OdString    getTextString() const = 0;
  virtual void        setTextString( const OdString strText ) = 0;
  virtual OdDg::TextJustification  getJustification() const = 0;
  virtual void        setJustification( const OdDg::TextJustification iJust ) = 0;
  virtual void        readTextProperties( OdStreamBuf* pStream ) = 0;
  virtual void        writeTextProperties( OdStreamBuf* pStream ) const = 0;
  virtual OdBinaryData getExtraData() const = 0;
  virtual void         setExtraData( const OdBinaryData& binData ) = 0;

#undef TEXT_FLAGS_DEF
#define TEXT_FLAGS_DEF(name)              \
  virtual bool get##name##Flag() const = 0;   \
  virtual void set##name##Flag(bool val) = 0;

  TEXT_FLAGS_DEF( InterCharSpacing )
  TEXT_FLAGS_DEF( FixedWidthSpacing )
  TEXT_FLAGS_DEF( Underline )
  TEXT_FLAGS_DEF( Slant )
  TEXT_FLAGS_DEF( Vertical )
  TEXT_FLAGS_DEF( RightToLeft )
  TEXT_FLAGS_DEF( ReverseMline )
  TEXT_FLAGS_DEF( Kerning )
  TEXT_FLAGS_DEF( Offset )
  TEXT_FLAGS_DEF( Codepage )
  TEXT_FLAGS_DEF( ShxBigFont )
  TEXT_FLAGS_DEF( BgColor )
  TEXT_FLAGS_DEF( Subscript )
  TEXT_FLAGS_DEF( Superscript )
  TEXT_FLAGS_DEF( ExText )
  TEXT_FLAGS_DEF( TextStyle )
  TEXT_FLAGS_DEF( Overline )
  TEXT_FLAGS_DEF( Bold )
  TEXT_FLAGS_DEF( UnderlineStyle )
  TEXT_FLAGS_DEF( OverlineStyle )
  TEXT_FLAGS_DEF( StyleOverrides )
  TEXT_FLAGS_DEF( WordWrapTextNode )
  TEXT_FLAGS_DEF( FullJustification )
  TEXT_FLAGS_DEF( Color )
  TEXT_FLAGS_DEF( AcadInterCharSpacing )
  TEXT_FLAGS_DEF( Backwards )
  TEXT_FLAGS_DEF( Upsidedown )
  TEXT_FLAGS_DEF( AcadFittedText )

  virtual OdDgTextExtendedProperties::LineSpacingType getLineSpacingType() const = 0;
  virtual void        setLineSpacingType( OdDgTextExtendedProperties::LineSpacingType uType ) = 0;
  virtual OdUInt8     getStackedFractionAlign() const = 0;
  virtual void        setStackedFractionAlign( OdUInt8 var ) = 0; 
  virtual OdUInt8     getStackedFractionSection() const = 0;
  virtual void        setStackedFractionSection( OdUInt8 var ) = 0; 
  virtual OdUInt8     getStackedFractionType() const = 0;
  virtual void        setStackedFractionType( OdUInt8 var ) = 0; 
  virtual double      getSlant() const = 0;
  virtual void        setSlant( double var ) = 0; 
  virtual double      getUnderlineSpacing() const = 0;
  virtual void        setUnderlineSpacing( double var ) = 0; 
  virtual OdGePoint2d getLineOffset() const = 0;
  virtual void        setLineOffset( const OdGePoint2d& var ) = 0; 
  virtual OdUInt32    getCodePage() const  = 0;
  virtual void        setCodePage( OdUInt32 var ) = 0; 
  virtual OdUInt32    getShxBigFontEntryId() const = 0;
  virtual void        setShxBigFontEntryId( OdUInt32 var ) = 0; 
  virtual OdUInt32    getBackgroundLineStyleEntryId() const = 0;
  virtual OdUInt32    getBackgroundLineWeight() const = 0;
  virtual OdUInt32    getBackgroundColorIndex() const = 0;
  virtual OdUInt32    getBackgroundFillColorIndex() const = 0;
  virtual OdGePoint2d getBackgroundBorder() const = 0;
  virtual double      getOverlineSpacing() const = 0;
  virtual void        setOverlineSpacing( double var ) = 0; 
  virtual OdUInt32    getTextStyleEntryId() const = 0;
  virtual void        setTextStyleEntryId( OdUInt32 var ) = 0; 
  virtual OdUInt32    getUnderlineColorIndex() const = 0;
  virtual OdUInt32    getUnderlineLineStyleEntryId() const = 0;
  virtual OdUInt32    getUnderlineLineWeight() const = 0;
  virtual OdUInt32    getOverlineColorIndex() const = 0;
  virtual OdUInt32    getOverlineLineStyleEntryId() const = 0;
  virtual OdUInt32    getOverlineLineWeight() const = 0;
  virtual double      getTextnodeWordWrapLength() const = 0;
  virtual void        setTextnodeWordWrapLength( double var ) = 0; 
  // the number of CR which should be added after the text element
  virtual OdUInt8     getCrCount() const = 0;
  virtual void        setCrCount( OdUInt8 var ) = 0; 
  virtual OdUInt32    getColorIndex() const = 0;
  virtual void        setColorIndex( OdUInt32 color ) = 0;

  //
  // OverrideFlags
  //
  //
#undef TEXTSTYLE_OVERRIDE_FLAGS_DEF
#define TEXTSTYLE_OVERRIDE_FLAGS_DEF(name)            \
  virtual bool get##name##OverrideFlag() const = 0;   \
  virtual void set##name##OverrideFlag(bool val) = 0;

  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FontNumber         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(ShxBigFont         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Width              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Height             )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Slant              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineSpacing        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineCharSpacing    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineOffset    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineOffset     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Justification      )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(NodeJustification  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineLength         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Direction          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Underline          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Overline           )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Italics            )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Bold               )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Superscript        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Subscript          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FixedSpacing       )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Background         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundStyle    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundWeight   )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundColor    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundFillColor)
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(BackgroundBorder   )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineStyle     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineWeight    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineColor     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineStyle      )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineWeight     )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineColor      )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineOffset         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Fractions          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(OverlineStyleFlag  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(UnderlineStyleFlag )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Color              )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(WidthFactor        )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(ColorFlag          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(FullJustification  )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(LineSpacingType    )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Backwards          )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(Upsidedown         )
  TEXTSTYLE_OVERRIDE_FLAGS_DEF(AcadInterCharSpacing)

  virtual void setOverlineStyle(OdUInt32 dOverlineColorIndex, OdUInt32 dOverlineLineStyleEntryId, OdUInt32 dOverlineLineWeight) = 0;
  virtual void setUnderlineStyle(OdUInt32 dUnderlineColorIndex, OdUInt32 dUnderlineLineStyleEntryId, OdUInt32 dUnderlineLineWeight) = 0;
  virtual void setBackground( OdUInt32 backgroundStyle,
    OdUInt32 backgroundWeight,
    OdUInt32 backgroundColor,
    OdUInt32 backgroundFillColor,
    OdGePoint2d const& backgroundBorder) = 0;
  virtual void getSpacing( double& var, OdDgTextExtendedProperties::Spacing& flag ) const = 0;
  virtual void setSpacing( double var, OdDgTextExtendedProperties::Spacing flag ) = 0;
  virtual void getOverrideFlags( OdUInt32& uLowFlag, OdUInt32& uHiFlag ) const = 0;

  virtual void        copy( const OdDgDimensionTextItemLinkagePtr pSource ) = 0;
  virtual void        copy( const OdDgDimensionTextItemLinkage& source ) = 0;
};

/** \details

Dimension text style data linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class OdDgDimensionTextStyleDataLinkage;
typedef OdSmartPtr<OdDgDimensionTextStyleDataLinkage> OdDgDimensionTextStyleDataLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDimensionTextStyleDataLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDimensionTextStyleDataLinkage );

  virtual bool readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;
  virtual OdDgTextStyleTableRecordPtr createTextStyle( OdDgDatabase* pDb ) const = 0;
  virtual void initFromTextStyle( const OdDgTextStyleTableRecordPtr& pTextStyle, bool bSetForDimension = true ) = 0;
  virtual bool hasData() const = 0;
};

/** \details

Display style linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class OdDgDisplayStyleLinkage;
typedef OdSmartPtr<OdDgDisplayStyleLinkage> OdDgDisplayStyleLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgDisplayStyleLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS( OdDgDisplayStyleLinkage );

  virtual bool readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;
  virtual OdUInt32 getDisplayStyleEntryId() const = 0;
  virtual void     setDisplayStyleEntryId( OdUInt32 uEntryId ) = 0;
};

/** \details

Sheet model linkage

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/

class OdDgSheetModelLinkage;
typedef OdSmartPtr<OdDgSheetModelLinkage> OdDgSheetModelLinkagePtr;

/** \details

Corresponding C++ library: DG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgSheetModelLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSheetModelLinkage);
  virtual void readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;
  virtual bool hasData() const = 0;

  virtual bool   getDrawBorderFlag() const = 0;
  virtual void   setDrawBorderFlag( bool bSet ) = 0;
  virtual bool   getDrawMarginsFlag() const = 0;
  virtual void   setDrawMarginsFlag( bool bSet ) = 0;
  virtual double getSheetXOffset() const = 0;
  virtual void   setSheetXOffset( double dXOffset ) = 0;
  virtual double getSheetYOffset() const = 0;
  virtual void   setSheetYOffset( double dYOffset ) = 0;
  virtual double getSheetRotationAngle() const = 0;
  virtual void   setSheetRotationAngle( double dAngle ) = 0;
  virtual double getSheetWidth() const = 0;
  virtual void   setSheetWidth( double dWidth ) = 0;
  virtual double getSheetHeight() const = 0;
  virtual void   setSheetHeight( double dHeight ) = 0;
  virtual double getSheetTopMargin() const = 0;
  virtual void   setSheetTopMargin( double dMargin ) = 0;
  virtual double getSheetLeftMargin() const = 0;
  virtual void   setSheetLeftMargin( double dMargin ) = 0;
  virtual double getSheetBottomMargin() const = 0;
  virtual void   setSheetBottomMargin( double dMargin ) = 0;
  virtual double getSheetRightMargin() const = 0;
  virtual void   setSheetRightMargin( double dMargin ) = 0;
  virtual double getSheetSizeScaleFactor() const = 0;
  virtual void   setSheetSizeScaleFactor( double dScale ) = 0;
  virtual OdDgModel::UnitDescription getSheetUnits() const = 0;
  virtual void   setSheetUnits( const OdDgModel::UnitDescription units ) = 0;
};

class OdDgSheetAnnotationScaleLinkage;
typedef OdSmartPtr<OdDgSheetAnnotationScaleLinkage> OdDgSheetAnnotationScaleLinkagePtr;

/** \details

Corresponding C++ library: DG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgSheetAnnotationScaleLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSheetAnnotationScaleLinkage);
  virtual void readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;
  virtual bool hasData() const = 0;

  virtual double getAnnotationScale() const = 0;
  virtual void   setAnnotationScale( double dScale ) = 0;
};

class OdDgSheetModelExtensionLinkage;
typedef OdSmartPtr<OdDgSheetModelExtensionLinkage> OdDgSheetModelExtensionLinkagePtr;

/** \details

Corresponding C++ library: DG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgSheetModelExtensionLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgSheetModelExtensionLinkage);
  virtual void readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;
  virtual bool hasData() const = 0;

  virtual OdUInt32 getSheetNumber() const = 0;
  virtual void     setSheetNumber( OdUInt32 iNumber ) = 0;
};

/** \details
Line Style Modification Linkage.

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class OdDgLineStyleModificationLinkage;
typedef OdSmartPtr<OdDgLineStyleModificationLinkage> OdDgLineStyleModificationLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgLineStyleModificationLinkage : public OdDgAttributeLinkage
{
public:

  enum OdDgLsMultilineDataType
  {
    kLsMultilineTypeLine     = 0,
    kLsMultilineTypeStartCap = 1,
    kLsMultilineTypeEndCap   = 2,
    kLsMultilineTypeJoints   = 3
  };

  enum OdDgLsModWidthMode
  {
    kLsModNoWidth       = 0,
    kLsModConstantWidth = 1,
    kLsModTaperedWidth  = 2
  };

  enum OdDgLsModShiftMode
  {
    kLsModNoShift       = 0,
    kLsModDistance      = 1,
    kLsModFraction      = 2,
    kLsModCentered      = 3
  };

  enum OdDgLsModCornerMode
  {
    kLsModFromLineStyle     = 0,
    kLsModBreakAtCorners    = 1,
    kLsModRunThroughCorners = 2
  };

public:
  ODRX_DECLARE_MEMBERS(OdDgLineStyleModificationLinkage);
  virtual void readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual bool getUseLineStyleScaleFlag() const = 0;
  virtual void setUseLineStyleScaleFlag( bool bSet ) = 0;

  virtual bool getUseLineStyleDashScaleFlag() const = 0;
  virtual void setUseLineStyleDashScaleFlag( bool bSet ) = 0;

  virtual bool getUseLineStyleGapScaleFlag() const = 0;
  virtual void setUseLineStyleGapScaleFlag( bool bSet ) = 0;

  virtual bool getUseLineStyleTrueWidthFlag() const = 0;
  virtual void setUseLineStyleTrueWidthFlag( bool bSet ) = 0;

  virtual double   getLineStyleScale() const = 0;
  virtual void     setLineStyleScale( double dScale ) = 0;

  virtual double   getLineStyleDashScale() const = 0;
  virtual void     setLineStyleDashScale( double dScale ) = 0;

  virtual double   getLineStyleGapScale() const = 0;
  virtual void     setLineStyleGapScale( double dScale ) = 0;

  virtual double   getLineStyleWidth() const = 0;
  virtual void     setLineStyleWidth( double dWidth ) = 0;

  virtual double   getLineStyleEndWidth() const = 0;
  virtual void     setLineStyleEndWidth( double dEndWidth ) = 0;

  virtual double   getLineStyleShift() const = 0;
  virtual void     setLineStyleShift( double dShift ) = 0;

  virtual double   getLineStyleFractionPhase() const = 0;
  virtual void     setLineStyleFractionPhase( double dPhase ) = 0;

  virtual OdDgLsModWidthMode getWidthMode() const = 0;
  virtual void     setWidthMode( OdDgLsModWidthMode iMode ) = 0;

  virtual OdDgLsModShiftMode getShiftMode() const = 0;
  virtual void     setShiftMode( OdDgLsModShiftMode iMode ) = 0;

  virtual OdDgLsModCornerMode getCornerMode() const = 0;
  virtual void     setCornerMode( OdDgLsModCornerMode iMode ) = 0;

  virtual bool     getUseLineStyleMultilineDataFlag() const = 0;
  virtual void     setUseLineStyleMultilineDataFlag( bool bSet ) = 0;

  virtual OdDgLsMultilineDataType getMultilineDataType() const = 0;
  virtual void     setMultilineDataType( OdDgLsMultilineDataType iDataType ) = 0;

  virtual bool     getMultilineProfileIndexes( OdUInt32Array& arrIndexes ) = 0;
  virtual void     setMultilineProfileIndexes( const OdUInt32Array& arrIndexes ) = 0;
};


/** \details

Corresponding C++ library: DG_Db

<group OdDg_Classes>
*/
class OdDgMaterialLinkage;
typedef OdSmartPtr<OdDgMaterialLinkage> OdDgMaterialLinkagePtr;

/** \details
   
  Corresponding C++ library: TG_Db 

  <group OdDg_Classes>
*/
class TG_EXPORT OdDgMaterialLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgMaterialLinkage);
  virtual void readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual OdString getMaterialName() const = 0;
  virtual void     setMaterialName( OdString strName ) = 0;
  virtual OdUInt16 getPaletteIndex() const = 0;
  virtual void     setPaletteIndex( OdUInt16 iIndex ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgInfiniteLineLinkage object pointers.
*/
class OdDgInfiniteLineLinkage;
typedef OdSmartPtr<OdDgInfiniteLineLinkage> OdDgInfiniteLineLinkagePtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgInfiniteLineLinkage : public OdDgAttributeLinkage
{
public:
  enum InfiniteLineType
  {
    kFromStartPoint = 1, 
    kFromEndPoint   = 2,
    kInfiniteLine = 3
  };
public:
  ODRX_DECLARE_MEMBERS(OdDgInfiniteLineLinkage);

  virtual InfiniteLineType getInfiniteLineType() const = 0;
  virtual void setInfiniteLineType( InfiniteLineType type ) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgTextAnnotationLinkage object pointers.
*/
class OdDgTextAnnotationLinkage;
typedef OdSmartPtr<OdDgTextAnnotationLinkage> OdDgTextAnnotationLinkagePtr;

/** \details

Corresponding C++ library: TG_Db

<group OdDg_Classes>
*/
class TG_EXPORT OdDgTextAnnotationLinkage : public OdDgAttributeLinkage
{
public:
  ODRX_DECLARE_MEMBERS(OdDgTextAnnotationLinkage);

  virtual void   readData(const OdUInt8* pData, OdUInt32 dataSize) = 0;

  virtual double getAnnotationScale() const = 0;
  virtual void   setAnnotationScale( double dScale ) = 0;
};

/** \details
    Factory method.
*/
typedef OdDgAttributeLinkagePtr (*OdDgLinkageFactoryFunc)(const OdUInt8* pData, OdUInt32 dataSize);


/** \details
    Sets (registers) the Linkage handler class.

    \param handlerId [in]  Handler ID (type) of XAttribute.
    \param pXAttributeClass [in]  Handler class

    Returns pointer to previous XAttribute handler class.
*/
//TG_EXPORT OdRxClass* oddgSetLinkageHandler(OdUInt16 primaryId, OdRxClass* pXAttributeClass);
TG_EXPORT OdDgLinkageFactoryFunc oddgSetLinkageHandler(OdUInt16 primaryId, OdDgLinkageFactoryFunc pFactoryFunc);

#include "TD_PackPop.h"

#endif // __DG_ATTRIBUTELINKAGE_H__
