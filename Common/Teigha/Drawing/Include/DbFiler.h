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

#ifndef FILER_INCLUDED
#define FILER_INCLUDED

#include <float.h>
#include "RxObject.h"
#include "OdStreamBuf.h"
#include "DbObjectId.h"

class OdString;
class OdGeScale3d;
class OdBinaryData;
class OdDbFilerController;
class OdDbDatabase;
class OdResBuf;
typedef OdSmartPtr<OdResBuf> OdResBufPtr;
class OdDbObjectId;
class OdGePoint2d;
class OdGePoint3d;
class OdGeVector2d;
class OdGeVector3d;
class OdDbAuditInfo;

#include "TD_PackPush.h"

/** \details
    This class is the abstract base class for classes that are used for file I/O operations
    utilizing the .dwg and .dxf file formats.
    
    Corresponding C++ library: TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbFiler : public OdRxObject
{
public:
  OdDbFiler() {}

  ODRX_DECLARE_MEMBERS(OdDbFiler);

  /** \details
    Returns the current status of this Filer object.    

    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual OdResult filerStatus() const;

  /** \details
    Resets the current status of this Filer object to eOk.    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual void resetFilerStatus();

  enum FilerType
  {
    kFileFiler         = 0, // .dwg and .dxf files
    kCopyFiler         = 1,
    kUndoFiler         = 2,  
    kBagFiler          = 3,
    kIdXlateFiler      = 4,
    kPageFiler         = 5,
    kDeepCloneFiler    = 6,
    kIdFiler           = 7,
    kPurgeFiler        = 8,
    kWblockCloneFiler  = 9
  };

  /** \details
    Returns the type of this Filer object.
  */
  virtual FilerType filerType() const = 0;

  /** \details
    Returns the database with which this Filer object is associated.  
  */
  virtual OdDbDatabase* database() const;

  /** \details
    Returns the version of this the file which this Filer object is associated.
    
    \remarks
    The maintenance release version is returned if and only if pMaintReleaseVer != 0. 

    \param pMaintReleaseVer [in]  Pointer to the MaintReleaseVer to receive the maintenance release version.
  */
  virtual OdDb::DwgVersion dwgVersion(
    OdDb::MaintReleaseVer* pMaintReleaseVer = 0) const;

  /** \details
    Returns the AuditInfo with which this Filer object is associated.  
  */
  OdDbAuditInfo * getAuditInfo() const;

  /** \details
    Sets the FilerController with which this Filer object is associated.
    
    \param pFilerController [in]  Pointer to the FilerController.
  */
  virtual void setController(
    OdDbFilerController * pFilerController);

  /** \details
    Returns the FilerController with which this Filer object is associated.
  */
  virtual OdDbFilerController* controller() const;
};

///////////////////////////////////////////////////////////////////////
/** \details
  <group Other_Classes>

  This class defines a function that returns the data type using 
  the specified group code (DXF code). 

  \sa
  <link db_rb_verify.html, Determining the Data Type by Group Code>  
*/
class TOOLKIT_EXPORT OdDxfCode
{
public:
  /** \details
    Defines codes for data types.
  */
  typedef enum
  {
    Unknown         = 0,
    Name            = 1,
    String          = 2,
    Bool            = 3,
    Integer8        = 4,
    Integer16       = 5,
    Integer32       = 6,
    Double          = 7,
    Angle           = 8,
    Point           = 9,
    BinaryChunk     = 10,
    LayerName       = 11,
    Handle          = 12,
    ObjectId        = 13,
    SoftPointerId   = 14,
    HardPointerId   = 15,
    SoftOwnershipId = 16,
    HardOwnershipId = 17,
    Integer64       = 18
  } Type;

  /** \details
    Returns the data type for the specified group code (DXF code).

    \sa
    <link db_rb_verify.html, Determining the Data Type by Group Code>

    \sa
    <link db_rb_type.html, Getting and Setting Tagged Data>
  */
  static Type _getType( int code );
};

/** \details
    This class is the abstract base class for classes that are used for file I/O operations
    with DXF formats.
    
    Corresponding C++ library: TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbDxfFiler : public OdDbFiler
{
public:
  ODRX_DECLARE_MEMBERS(OdDbDxfFiler);
  OdDbDxfFiler() {}

  /** \details
    Moves the I/O pointer to the specified location in this Filer object.
    
    \param offset [in]  Number of bytes.
    \param seekType [in]  Seek type.
    
    \remarks
    seekType must be one of the following:
    
    <table>
    Name                    Value       Description.
    OdDb::kSeekFromStart    SEEK_SET    Start of file.
    OdDb::kSeekFromCurrent  SEEK_CUR    Current position of I/O pointer.
    OdDb::kSeekFromEnd      SEEK_END    End of file.
    </table>

    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual void seek(
    OdInt64 offset, 
    OdDb::FilerSeekType seekType);
  
  /** \details
    Returns the current I/O pointer position of this Filer object.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual OdUInt64 tell() const;

  /** \details
    Returns the number of decimal places for DxfOut filing with this Filer object.
    
    \remarks
    Returns kDfltPrec for DxfIn filing.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual int precision() const;

  /** \details
    Sets the number of decimal places DxfOut filing with this Filer object.
    
    \param precision [in]  Decimal places.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual void setPrecision(
    int decimalDigits);

  /** \details
    Number of decimal digits written to ASCII DXF files.
  */
  enum 
  { 
    kDfltPrec = -1, 
    kMaxPrec = DBL_DIG + 1 
  };

  /** \details
    Indicates the start of xdata in this Filer object.
  */
  virtual void writeXDataStart();
  
  /** \details
    Returns true if and only if default values are normally written 
    to the file associated with this Filer object.
  */
    virtual bool includesDefaultValues() const;

  /** \details
    Returns true if and only if this Filer object is at the end of an object's data.
    
    \remarks
    This will be true when the filter is at any of the following:

    * A subclass data marker.
    * The start of an object's xdata.
    * The start of embedded object's data.
    * The start of another object's data.
    * The end of a file.

    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual bool atEOF();
  
  /** \details
    Returns true if and only if this Filer object is at the end of an object's data.
    
    \remarks
    This will be true when the filter is at any of the following:
    
    * The start of another object's data.
    * The end of a file.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual bool atEndOfObject();
  
  /** \details
    Returns true if and only if this Filer object is at the start of an object's xdata.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual bool atExtendedData();

  /** \details
    Returns true if and only if this Filer object is at a subclass data marker with the specified subClassName.

    \param subClassName [in]  Subclass name.
    
    \remarks
    If true, the Filer advances the file pointer to the next item.
     
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual bool atSubclassData(
    const OdString& subClassName);

  /** \details
    Returns true if and only if this Filer object is at start of an embedded object.
    
    \remarks
    If true, the Filer advances the file pointer to the next item.
     
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual bool atEmbeddedObjectStart();

  /** \details
    Reads the next item of this Filer object, and returns its group code.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual int nextItem();
  
  /** \details
    Reads the next item of this Filer object, and returns a SmartPointer to its ResBuf.
  */
  virtual OdResBufPtr nextRb();
  
  /** \details
    Writes the specified ResBuf object to this Filer object.
    \param pRb [in]  Pointer to the ResBuf object.
  */
  virtual void writeRb(
    const OdResBuf* pRb);

  /** \details
    Resets this Filer object to where it was before the last item was read.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
    virtual void pushBackItem();
  
  /** \details
    Returns the string value of the current item of this Filer object.
    \param value [out]  Receives the value.
  */
  OdString rdString();
  virtual void rdString(
    OdString &value) = 0;

  /** \details
    Returns the bool value of the current item of this Filer object.
  */
  virtual bool rdBool() = 0;

  /** \details
    Returns the Int8 value of the current item of this Filer object.
  */
  virtual OdInt8 rdInt8() = 0;

  /** \details
    Returns the Int16 value of the current item of this Filer object.
  */
  virtual OdInt16 rdInt16() = 0;

  /** \details
    Returns the Int32 value of the current item of this Filer object.
  */
  virtual OdInt32 rdInt32() = 0;
  
  /** \details
  Returns the Int64 value of the current item of this Filer object.
  */
  virtual OdInt64 rdInt64() = 0;

  /** \details
    Returns the UInt8 value of the current item of this Filer object.
  */
  virtual OdUInt8 rdUInt8() = 0;
  
  /** \details
    Returns the UInt16 value of the current item of this Filer object.
  */
  virtual OdUInt16 rdUInt16() = 0;
  
  /** \details
    Returns the UInt32 value of the current item of this Filer object.
  */
  virtual OdUInt32 rdUInt32() = 0;
  
  /** \details
  Returns the UInt64 value of the current item of this Filer object.
  */
  virtual OdUInt64 rdUInt64() = 0;

  /** \details
    Returns the handle value of the current item of this Filer object.
  */
  virtual OdDbHandle rdHandle() = 0;
  
  /** \details
    Returns the Object ID value of the current item of this Filer object.
  */
  virtual OdDbObjectId rdObjectId() = 0;

  /** \details
  Returns the angle value of the current item of this Filer object.
  */
  virtual double rdAngle() = 0;

  /** \details
    Returns the double value of the current item of this Filer object.
  */
  virtual double rdDouble() = 0;
 
  /** \details
    Returns the 2D point value of the current item of this Filer object.
    \param value [out]  Receives the value.
  */
  virtual void rdPoint2d(
    OdGePoint2d& value) = 0;
    
  /** \details
    Returns the 3D point value of the current item of this Filer object.
    \param value [out]  Receives the value.
  */
  virtual void rdPoint3d(
    OdGePoint3d& value) = 0;

  /** \details
    Returns the 2D vector value of the current item of this Filer object.
    \param value [out]  Receives the value.
  */
  virtual void rdVector2d(
    OdGeVector2d& value) = 0;

  /** \details
    Returns the 3D vector value of the current item of this Filer object.
    \param value [out]  Receives the value.
  */
  virtual void rdVector3d(
    OdGeVector3d& value) = 0;
  
  /** \details
    Returns the scale value of the current item of this Filer object.
    \param value [out]  Receives the value.
  */
  virtual void rdScale3d(
    OdGeScale3d& value) = 0;
    
  /** \details
    Returns the BinaryData value of the current item of this Filer object.
    
    \param value [out]  Receives the value.
  */
  virtual void rdBinaryChunk(
    OdBinaryData& value) = 0;

  /** \details
    Copies the current item from the specified Filer object to this one.
    \param pSource [in]  Pointer to the source Filer.
  */
  virtual void copyItem(
    OdDbDxfFiler * pSource);

  /** \details
    Writes the specified name value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
  virtual void wrName(
    int groupCode,
    const OdString& value) = 0;
    
  /** \details
    Writes the specified string value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrString(
    int groupCode, 
    const OdString& value) = 0;
    
  /** \details
    Writes the specified optional string value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != NULL) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    void wrStringOpt(
    int groupCode, 
    const OdString& value);

  /** \details
    Writes the specified subclass data marker to this Filer object.
      
    \param value [in]  Value.
  */
    void wrSubclassMarker(
    const OdString &value);

  /** \details
    Writes the embedded object start marker to this Filer object.
  */
    void wrEmbeddedObjectStart();

  /** \details
    Writes the specified bool value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrBool(
    int groupCode, 
    bool value) = 0;
    
  /** \details
    Writes the specified optional bool value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrBoolOpt(
    int groupCode, 
    bool value, 
    bool defaultValue);

  /** \details
    Writes the specified Int8 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrInt8(
    int groupCode, 
    OdInt8 value) = 0;
    
  /** \details
    Writes the specified optional Int8 value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrInt8Opt(
    int groupCode, 
    OdInt8 value, 
    OdInt8 defaultValue);

  /** \details
    Writes the specified UInt8 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrUInt8(
    int groupCode, 
    OdUInt8 value) = 0;
  /** \details
    Writes the specified optional UInt8 value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrUInt8Opt(
    int groupCode, 
    OdUInt8 value, 
    OdUInt8 defaultValue);

  /** \details
    Writes the specified Int16 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrInt16(
    int groupCode, 
    OdInt16 value) = 0;
    
  /** \details
    Writes the specified optional Int16 value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrInt16Opt(
    int groupCode, 
    OdInt16 value, 
    OdInt16 defaultValue);

  /** \details
    Writes the specified UInt16 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrUInt16(
    int groupCode, 
    OdUInt16 value) = 0;
    
  /** \details
    Writes the specified optional UInt16 value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrUInt16Opt(
    int groupCode, 
    OdUInt16 value, 
    OdUInt16 defaultValue);

  /** \details
    Writes the specified Int32 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrInt32(
    int groupCode, 
    OdInt32 value) = 0;
    
  /** \details
    Writes the specified optional Int32 value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrInt32Opt(
    int groupCode, 
    OdInt32 value, 
    OdInt32 defaultValue);

  /** \details
    Writes the specified UInt32 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrUInt32(
    int groupCode, 
    OdUInt32 value) = 0;
    
  /** \details
    Writes the specified optional UInt32 value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrUInt32Opt(
    int groupCode, 
    OdUInt32 value, 
    OdUInt32 defaultValue);

  /** \details
    Writes the specified Int64 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrInt64(
    int groupCode, 
    OdInt64 value) = 0;

  /** \details
    Writes the specified optional Int64 value to this Filer object.

    \remarks
    value is written if and only if
    (value != default) || (includesDefaultValues() == true)

    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrInt64Opt(
    int groupCode, 
    OdInt64 value, 
    OdInt64 defaultValue);

  /** \details
    Writes the specified UInt64 value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrUInt64(
    int groupCode, 
    OdUInt64 value) = 0;

  /** \details
    Writes the specified optional UInt64 value to this Filer object.

    \remarks
    value is written if and only if
    (value != default) || (includesDefaultValues() == true)

    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrUInt64Opt(
    int groupCode, 
    OdUInt64 value, 
    OdUInt64 defaultValue);

  /** \details
    Writes the specified bool value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrHandle(
    int groupCode, 
    OdDbHandle value) = 0;

  /** \details
    Writes the specified Object ID value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */
    virtual void wrObjectId(
    int groupCode, 
    OdDbObjectId value) = 0;
    
  /** \details
    Writes the specified optional Object ID value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != NULL) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
  */
  void wrObjectIdOpt(
    int groupCode, 
    OdDbObjectId value);

  /** \details
    Writes the specified angle value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param precision [in]  Decimal places.
  */
    virtual void wrAngle(
    int groupCode, 
    double value, 
    int precision = kDfltPrec) = 0;
    
  /** \details
    Writes the specified optional angle value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
    \param precision [in]  Decimal places.
  */
  void wrAngleOpt(
    int groupCode, 
    double value, 
    double defaultValue = 0., 
    int precision = kDfltPrec);

  /** \details
    Writes the specified double value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param precision [in]  Decimal places.
  */
    virtual void wrDouble(
    int groupCode, 
    double value, 
    int precision = kDfltPrec) = 0;
    
  /** \details
    Writes the specified optional double value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
    \param precision [in]  Decimal places.
  */
  void wrDoubleOpt(
    int groupCode, 
    double value, 
    double defaultValue = 0., 
    int precision = kDfltPrec);

  /** \details
    Writes the specified 2D point value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param precision [in]  Decimal places.
  */
    virtual void wrPoint2d(
    int groupCode, 
    const OdGePoint2d& value, 
    int precision = kDfltPrec) = 0;
    
  /** \details
    Writes the specified optional 2D point value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
    \param precision [in]  Decimal places.
  */
  void wrPoint2dOpt(
    int groupCode, 
    const OdGePoint2d& value, 
    const OdGePoint2d& defaultValue, 
    int precision = kDfltPrec);

  /** \details
    Writes the specified 3D point value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param precision [in]  Decimal places.
  */
    virtual void wrPoint3d(
    int groupCode, 
    const OdGePoint3d& value, 
    int precision = kDfltPrec) = 0;
    
  /** \details
    Writes the specified optional 3D point value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
    \param precision [in]  Decimal places.
  */
  void wrPoint3dOpt(
    int groupCode, 
    const OdGePoint3d& value, 
    const OdGePoint3d& defaultValue, 
    int precision = kDfltPrec);

  /** \details
    Writes the specified 2D vector value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param precision [in]  Decimal places.
  */
    virtual void wrVector2d(
    int groupCode, 
    const OdGeVector2d& value, 
    int precision = kDfltPrec) = 0;
    
  /** \details
    Writes the specified optional 2D vector value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
    \param precision [in]  Decimal places.
  */
  void wrVector2dOpt(
    int groupCode, 
    const OdGeVector2d& value, 
    const OdGeVector2d& defaultValue, 
    int precision = kDfltPrec);

  /** \details
    Writes the specified 3D vector value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param precision [in]  Decimal places.
  */
    virtual void wrVector3d(
    int groupCode, 
    const OdGeVector3d& value, 
    int precision = kDfltPrec) = 0;
    
  /** \details
    Writes the specified optional 3D vector value to this Filer object.
    
    \remarks
    value is written if and only if
              (value != default) || (includesDefaultValues() == true)
     
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param defaultValue [in]  Default value.
    \param precision [in]  Decimal places.
  */
  void wrVector3dOpt(
    int groupCode, 
    const OdGeVector3d& value, 
    const OdGeVector3d& defaultValue, 
    int precision = kDfltPrec);

  /** \details
    Writes the specified 2D scale  value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
    \param precision [in]  Decimal places.
  */
    virtual void wrScale3d(
    int groupCode, 
    const OdGeScale3d& value, 
    int precision = kDfltPrec) = 0;

  /** \details
    Writes the specified BinaryChunk value to this Filer object.
    \param groupCode [in]  Group code.
    \param numBytes [in]  Number of bytes.
    \param buffer [in]  Array of bytes.
  */
    virtual void wrBinaryChunk(
    int groupCode, 
    const OdUInt8* buffer, 
    OdUInt32 numBytes) = 0;
  
  /** \details
    Writes the specified BinaryChunk value to this Filer object.
    \param groupCode [in]  Group code.
    \param value [in]  Value.
  */   
  void wrBinaryChunk(
    int groupCode, 
    const OdBinaryData& value);
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDxfFiler object pointers.
*/
typedef OdSmartPtr<OdDbDxfFiler> OdDbDxfFilerPtr;

/** \details
    This class implements OdError objects with Result codes of eDwgObjectImproperlyRead.
    
    Corresponding C++ library: TD_Db
    <group Error_Classes> 
*/
class TOOLKIT_EXPORT OdError_DwgObjectImproperlyRead : public OdError
{
public:
  OdError_DwgObjectImproperlyRead() : OdError(eDwgObjectImproperlyRead){}
};

/** \details
    This class is the abstract base class for classes that are used for file I/O operations
    with the .dwg file format.
    
    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbDwgFiler : public OdDbFiler
{
public:
  OdDbDwgFiler() {}

  ODRX_DECLARE_MEMBERS(OdDbDwgFiler);

  /** \details
    Moves the I/O pointer to the specified location in this Filer object.
    
    \param offset [in]  Number of bytes.
    \param seekType [in]  Seek type.
    
    \remarks
    seekType must be one of the following:
    
    <table>
    Name                    Value       Description.
    OdDb::kSeekFromStart    SEEK_SET    Start of file.
    OdDb::kSeekFromCurrent  SEEK_CUR    Current position of I/O pointer.
    OdDb::kSeekFromEnd      SEEK_END    End of file.
    </table>
  */
  virtual void seek(
    OdInt64 offset, 
    OdDb::FilerSeekType seekType) = 0;

  /** \details
    Returns the current I/O pointer position of this Filer object.
  */
  virtual OdUInt64 tell() const = 0;

  /** \details
    Returns the bool value of the current item of this Filer object.
  */
  virtual bool rdBool() = 0;

  /** \details
    Returns the string value of the current item of this Filer object.

  */
  virtual OdString rdString() = 0;

  
  /** \details
    Returns the specified number of bytes from this Filer object.
    \param buffer [out]  Receives the bytes.
    \param numBytes [in]  Number of bytes.
  */
  virtual void rdBytes(
    void* buffer, 
    OdUInt32 numBytes) = 0;


  /** \details
    Returns the Int8 value of the current item of this Filer object.
  */
  virtual OdInt8 rdInt8() = 0;
  
  /** \details
    Returns the UInt8 value of the current item of this Filer object.
  */
  virtual OdUInt8 rdUInt8() = 0;

  /** \details
    Returns the Int16 value of the current item of this Filer object.
  */
  virtual OdInt16 rdInt16() = 0;

  /** \details
    Returns the Int32 value of the current item of this Filer object.
  */
  virtual OdInt32 rdInt32() = 0;

  /** \details
    Returns the Int64 value of the current item of this Filer object.
  */
  virtual OdInt64 rdInt64() = 0;

  /** \details
    Returns the memory address value of the current item of this Filer object.
  */
  virtual void* rdAddress();

  /** \details
    Returns the double value of the current item of this Filer object.
  */
  virtual double rdDouble() = 0;

  /** \details
    Returns the handle value of the current item of this Filer object.
  */
  virtual OdDbHandle rdDbHandle() = 0;

  /** \details
    Returns the SoftOwnershipId value of the current item of this Filer object.
  */
  virtual OdDbObjectId rdSoftOwnershipId() = 0;

  /** \details
    Returns the HardOwnershipId value of the current item of this Filer object.
  */
  virtual OdDbObjectId rdHardOwnershipId() = 0;

  /** \details
    Returns the HardPointerId value of the current item of this Filer object.
  */
  virtual OdDbObjectId rdHardPointerId() = 0;

  /** \details
    Returns the SoftPointerId value of the current item of this Filer object.
  */
  virtual OdDbObjectId rdSoftPointerId() = 0;

  /** \details
    Returns the 2D point value of the current item of this Filer object.
  */
  virtual OdGePoint2d rdPoint2d() = 0;
    
  /** \details
    Returns the 3D point value of the current item of this Filer object.
  */
  virtual OdGePoint3d rdPoint3d() = 0;

  /** \details
    Returns the 2D vector value of the current item of this Filer object.
  */
  virtual OdGeVector2d rdVector2d() = 0;

  /** \details
    Returns the 3D vector value of the current item of this Filer object.
  */
  virtual OdGeVector3d rdVector3d() = 0;
  
  /** \details
    Returns the scale value of the current item of this Filer object.
  */
  virtual OdGeScale3d rdScale3d() = 0;

  /** \details
    Returns the thickness value of the current item of this Filer object.
  */
  virtual double rdThickness();

  /** \details
    Returns the extrusion value of the current item of this Filer object.
  */
  virtual OdGeVector3d rdExtrusion();


  /** \details
    Writes the specified bool value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrBool(
    bool value) = 0;

  /** \details
    Writes the specified string value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrString(
    const OdString &value) = 0;
    
  /** \details
    Writes the specified number of bytes to this Filer object.
    \param buffer [in]  array of bytes.
    \param numBytes [in]  Number of bytes.
  */
  virtual void wrBytes(
    const void* buffer, 
    OdUInt32 numBytes) = 0;

  /** \details
    Writes the specified Int8 value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrInt8(
    OdInt8 value) = 0;
    
  /** \details
    Writes the specified UInt8 value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrUInt8(
    OdUInt8 value) = 0;

  /** \details
    Writes the specified Int16 value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrInt16(
    OdInt16 value) = 0;
    
  /** \details
    Writes the specified Int32 value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrInt32(
    OdInt32 value) = 0;

  /** \details
    Writes the specified Int64 value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrInt64(
    OdInt64 value) = 0;

  /** \details
    Writes the specified memory address value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrAddress(
    const void* value);
    
  /** \details
    Writes the specified double value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrDouble(double value) = 0;

  /** \details
    Writes the specified handle value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrDbHandle(
    const OdDbHandle& value) = 0;

  /** \details
    Writes the specified SoftOwnershipId value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrSoftOwnershipId(
    const OdDbObjectId& value) = 0;
    
  /** \details
    Writes the specified HardOwnershipId value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrHardOwnershipId(
    const OdDbObjectId& value) = 0;

  /** \details
    Writes the specified SoftPointerId value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrSoftPointerId(
    const OdDbObjectId& value) = 0;

  /** \details
    Writes the specified HardPointerId value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrHardPointerId(
    const OdDbObjectId& value) = 0;

  /** \details
    Writes the specified 2D point value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrPoint2d(
    const OdGePoint2d& value) = 0;

  /** \details
    Writes the specified 3D point value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrPoint3d(
    const OdGePoint3d& value) = 0;
  
  /** \details
    Writes the specified 2D vector value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrVector2d(
    const OdGeVector2d& value) = 0;
  
  /** \details
    Writes the specified 3D vector value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrVector3d(
    const OdGeVector3d& value) = 0;
  
  /** \details
    Writes the specified 3D scale value to this Filer object.
    \param value [in]  Value.
  */
    virtual void wrScale3d(
    const OdGeScale3d& value) = 0;

  /** \details
    Writes the specified thickness value to this Filer object.
    \param value [in]  Value.
  */
  virtual void wrThickness(double value);

  /** \details
    Writes the specified extrusion value to this Filer object.
    \param value [in]  Value.
  */
  virtual void wrExtrusion(const OdGeVector3d& value);

  /** \details
    Returns true if and only if this Filer object wishes to see
    all object references.
    
    \remarks
    The default implementation of this function always returns true. 
  */
  virtual bool usesReferences() const;
  
  /*!DOM*/
  /** \details
  */
  virtual void addReference(OdDbObjectId id, OdDb::ReferenceType rt);

  /** \details
  Returns true if and only if this filer object is used to store/read revision control format.

  \remarks
  The default implementation of this function always returns false.
  */
  virtual bool isPersistentMode() const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbDwgFiler object pointers.
*/
typedef OdSmartPtr<OdDbDwgFiler> OdDbDwgFilerPtr;


/** \details
    This class is the empty implementation of a base class 
    for classes that are used as ID Filers.
    
    Corresponding C++ library: TD_Db
    
    <group Other_Classes>
*/
class TOOLKIT_EXPORT OdIdFiler : public OdDbDwgFiler
{
public:
  OdIdFiler() : m_pController(NULL) {}

  void seek(
    OdInt64 offset, 
    OdDb::FilerSeekType seekType);
  
  OdUInt64 tell() const;
  
  OdResult filerStatus() const;
  
  void resetFilerStatus();
  
  FilerType filerType() const;
  
  OdDb::DwgVersion dwgVersion(
    OdDb::MaintReleaseVer* pMaintReleaseVer = 0) const;
    
  bool rdBool();
  
  OdString rdString();
  
  void rdBytes(
    void* buffer, 
    OdUInt32 numBytes);

  OdInt8 rdInt8();
  OdUInt8 rdUInt8();
  OdInt16 rdInt16();
  OdInt32 rdInt32();
  OdInt64 rdInt64();
  double rdDouble();
  OdDbHandle rdDbHandle();

  OdDbObjectId rdSoftOwnershipId();
  OdDbObjectId rdHardOwnershipId();
  OdDbObjectId rdHardPointerId();
  OdDbObjectId rdSoftPointerId();

  OdGePoint2d rdPoint2d();
  OdGePoint3d rdPoint3d();
  OdGeVector3d rdVector3d();
  OdGeVector2d rdVector2d();
  OdGeScale3d rdScale3d();

  void wrBool(
    bool value);

  void wrString(
    const OdString &value);
    
  void wrBytes(
    const void* buffer, 
    OdUInt32 numBytes);

  void wrInt8(
    OdInt8 value);
  void wrUInt8(
    OdUInt8 value);
  void wrInt16(
    OdInt16 value);
  void wrInt32(
    OdInt32 value);
  void wrInt64(
    OdInt64 value);
  void wrDouble(
    double value);
  void wrDbHandle(
    const OdDbHandle& value);

  void wrPoint2d(
    const OdGePoint2d& value);
  void wrPoint3d(
    const OdGePoint3d& value);
  void wrVector2d(
    const OdGeVector2d& value);
  void wrVector3d(
    const OdGeVector3d& value);
  void wrScale3d(
    const OdGeScale3d& value);

  void setController(
    OdDbFilerController* pFilerController) { m_pController = pFilerController; }
  OdDbFilerController* controller() const { return m_pController; }
private:
  OdDbFilerController* m_pController;
};

#include "TD_PackPop.h"

#endif  // FILER_INCLUDED

