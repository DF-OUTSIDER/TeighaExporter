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

#ifndef _EX_DBUNDOCONTROLLER_H_
#define _EX_DBUNDOCONTROLLER_H_

#include "TD_PackPush.h"
#include "DbUndoController.h"
#include "UInt8Array.h"
#include "OdList.h"
/** \details
  This class implements platform-independent UndoController objects.
  
  <group ExServices_Classes> Library: Source provided. 
*/
class ExUndoController : public OdDbUndoController
{
  OdList<OdUInt8Array>  m_records;
  OdUInt32              m_nMemoryUsed;

  OdUInt32              m_nMaxSteps;
  OdUInt32              m_nMaxMemory;
protected:
  ExUndoController();

  /** \details
    Frees the first record of this UndoController object.
  */
  void freeFrontRecord();
  
  /** \details
    Frees the last record of this UndoController object.
  */
  void freeBackRecord();
  
  /** \details
    Returns the memory size (in bytes) of the first record of this UndoController object.
  */
  OdUInt32 frontRecordMemory() const;

  /** \details
    Returns the memory size (in bytes) of the last record of this UndoController object.
  */
  OdUInt32 backRecordMemory() const;

  /** \details
    Returns the memory size (in bytes) required for a record with the specified data size.
    \param dataSize [in]  Data size (in bytes).
  */
  static OdUInt32 recordMemory(OdUInt32 dataSize);
  
  /** \details
    Frees extra memory from this UndoController object.
    \remarks
    Frees front records until the memory size and number of records of the 
    UndoControler list is less that that specified by the limits.
  */
  void freeExtra();
  
  /** \details
     Adds a record of the specifed size to the end of this UndoController object. 
     \remarks
     Returns true if and only if successful. 
  */
  bool pushRecord(OdUInt32 nSizeOfRecToAppend);
public:
  /** \details
    Sets the limits of this UndoController object.
    \param maxSteps [in]  Maximum number of records.
    \param maxMemory [in]  Maximum memory in bytes.
  */
  void setLimits(OdUInt32 maxSteps, OdUInt32 maxMemory);

  /** \details
    Adds the specified number of bytes from the specified StreamBuf object
    to the end of this UndoController object.
    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
    \param numBytes [in]  Number of bytes to be read.   
  */
  void pushData(OdStreamBuf* pStreamBuf, OdUInt32 numBytes, OdUInt32 opt);

  /** \details
    Returns true if and only if this UndoController object is not empty.
  */
  bool hasData() const;
  
  /** \details
    Writes the record at the end of this UndoController object to the specified
    StreamBuf object.
    \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.
    
  */
   OdUInt32 popData(OdStreamBuf* pStreamBuf);

  OdRxIteratorPtr newRecordStackIterator() const;
  
  void clearData();
};
#include "TD_PackPop.h"

#endif // _EX_DBUNDOCONTROLLER_H_
