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
#ifndef __DG_FILER_H__
#define __DG_FILER_H__

#include "RxObject.h"
#include "Ge/GeQuaternion.h"
#include "DgExport.h"
#include "DgElementId.h"

class OdDgDatabase;
class OdDgFilerController;

#include "TD_PackPush.h"

/** \details
    This class is the abstract base class for classes that are used for file I/O operations
    utilizing DGN formats.

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgFiler : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgFiler);

  OdDgFiler() {}

  enum FilerType
  {
    kFileFiler         = 0
  , kCopyFiler         = 1
  , kUndoFiler         = 2
  , kIdXlateFiler      = 4
  , kDeepCloneFiler    = 6
  , kWblockCloneFiler  = 9
  , k2dTo3dFiler       = 10
  , k3dTo2dFiler       = 11
  };

  /** \details
    Returns the type of this Filer object.
  */
  virtual FilerType filerType() const = 0;

  /** \details
    Returns the database with which this Filer object is associated.  
  */
  virtual OdDgDatabase* database() const = 0;

  /** \details
    Sets the FilerController with which this Filer object is associated.
    
    \param pFilerController [in]  Pointer to the FilerController.
  */
  virtual void setController(
    OdDgFilerController* pFilerController);

  /** \details
    Returns the FilerController with which this Filer object is associated.
  */
  virtual OdDgFilerController* getController() const;

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
    OdInt32 offset,
    OdDb::FilerSeekType whence) = 0;

  /** \details
    Returns the current I/O pointer position of this Filer object.
    
    \remarks
    The default implementation of this function does nothing but
    throw an exception.
  */
  virtual OdUInt32 tell() const = 0;

  virtual OdUInt32 length() const = 0;

  virtual void rdBytes(void* buffer, OdUInt32 nLen) = 0;
  virtual OdInt8 rdInt8() = 0;
  virtual OdUInt8 rdUInt8() = 0;
  virtual OdInt16 rdInt16() = 0;
  virtual OdInt32 rdInt32() = 0;
  virtual OdInt64 rdInt64() = 0;
  virtual double rdDouble() = 0;

  virtual OdDbHandle rdDbHandle() = 0;
  virtual OdDgElementId rdSoftOwnershipId() = 0;
  virtual OdDgElementId rdHardOwnershipId() = 0;
  virtual OdDgElementId rdHardPointerId() = 0;
  virtual OdDgElementId rdSoftPointerId() = 0;

  virtual OdGePoint2d rdPoint2d() = 0;
  virtual OdGePoint3d rdPoint3d() = 0;
  virtual OdGeVector2d rdVector2d() = 0;
  virtual OdGeVector3d rdVector3d() = 0;
  virtual OdGeQuaternion rdQuaternion() = 0;
  virtual OdString rdUnicodeString(OdUInt32 length) = 0;
  virtual OdAnsiString rdAnsiString(OdInt32 length) = 0;

  virtual void wrBytes(const void* buffer, OdUInt32 nLen) = 0;
  virtual void wrInt8(OdInt8 val) = 0;
  virtual void wrUInt8(OdUInt8 val) = 0;
  virtual void wrInt16(OdInt16 val) = 0;
  virtual void wrInt32(OdInt32 val) = 0;
  virtual void wrInt64(OdInt64 val) = 0;
  virtual void wrDouble(double val) = 0;

  virtual void wrDbHandle(const OdDbHandle& value) = 0;
  virtual void wrSoftOwnershipId(const OdDgElementId& value) = 0;
  virtual void wrHardOwnershipId(const OdDgElementId& value) = 0;
  virtual void wrSoftPointerId(const OdDgElementId& value) = 0;
  virtual void wrHardPointerId(const OdDgElementId& value) = 0;

  virtual void wrPoint2d(const OdGePoint2d& pt) = 0;
  virtual void wrPoint3d(const OdGePoint3d& pt) = 0;
  virtual void wrVector2d(const OdGeVector2d& vec) = 0;
  virtual void wrVector3d(const OdGeVector3d& vec) = 0;
  virtual void wrQuaternion(const OdGeQuaternion& rotation) = 0;
  virtual void wrUnicodeString(const OdString& str) = 0;
  virtual void wrAnsiString(const OdAnsiString& str) = 0;

  virtual void* rdAddress();
  virtual void  wrAddress(const void* value);

  virtual float  rdFloat() = 0;
  virtual void wrFloat(float val) = 0;
};

typedef OdSmartPtr<OdDgFiler> OdDgFilerPtr;


///////////////////////////////////////////////////////////////////////////////
// Predefined names
#define AC_STRING(name, val) \
extern TG_EXPORT_GLOBAL const OdConstString name;
#include "DgStdStrings.h"
#undef AC_STRING


#include "TD_PackPop.h"

#endif  // __DG_FILER_H__
