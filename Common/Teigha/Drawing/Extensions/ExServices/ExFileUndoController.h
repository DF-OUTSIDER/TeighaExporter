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

#ifndef _EX_DBFILEUNDOCONTROLLER_H_
#define _EX_DBFILEUNDOCONTROLLER_H_

#include "TD_PackPush.h"
#include "DbUndoController.h"
#include "UInt8Array.h"
#include "OdList.h"

/** \details
This class implements platform-independent UndoController objects.

<group ExServices_Classes> Library: Source provided. 
*/
class ExFileUndoController : public OdDbUndoController
{
public:
  struct UndoRecord
  {
    OdUInt32 options;
    OdUInt64 blockSize;
  };
private:
  OdList<UndoRecord> m_records;
  OdStreamBufPtr     m_pStorage;
protected:
  ExFileUndoController();

public:
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

  void setStorage(OdStreamBufPtr pStorage);
};

/** \details
This template class is a specialization of the OdSmartPtr class for ExFileUndoController object pointers.
*/
typedef OdSmartPtr<ExFileUndoController> ExFileUndoControllerPtr;

#include "TD_PackPop.h"

#endif // _EX_DBFILEUNDOCONTROLLER_H_
