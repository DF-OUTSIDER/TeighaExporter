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


#ifndef _ODDBUNDOCONTROLLER_INCLUDED_
#define _ODDBUNDOCONTROLLER_INCLUDED_

#include "RxObject.h"
#include "RxIterator.h"
#include "DbRootExport.h"

class OdStreamBuf;

/** \details
  <group OdDb_Classes>
*/
class DBROOT_EXPORT ODRX_ABSTRACT OdDbUndoControllerRecord : public OdRxObject
{
protected:
  OdDbUndoControllerRecord();
public:
  ODRX_DECLARE_MEMBERS(OdDbUndoControllerRecord);

  virtual OdUInt32 options() const = 0;
};

typedef OdSmartPtr<OdDbUndoControllerRecord> OdDbUndoControllerRecordPtr;


/** \details
    This class defines the interface for Undo Controller objects for OdDbDatabase instances.
    \remarks
    The implementer manages resources for storing undo and redo information.
    <group OdDb_Classes>
*/
class DBROOT_EXPORT ODRX_ABSTRACT OdDbUndoController : public OdRxObject
{
public:
  enum
  {
    kDefault    = 0,
    kBlockBegin = 1,
    kBlockEnd   = 2,
    kMarker     = 3
  };

  ODRX_DECLARE_MEMBERS(OdDbUndoController);
  /** \details
    Saves undo information in some manner.
    \param pStream [in]  stream that contains single undo step data.
    \param nSize [in]  size of undo information (starting from current stream position).
    \remarks
    Implementer should save nSize bytes from specified stream.
  */
  virtual void pushData(OdStreamBuf* pStream, OdUInt32 nSize, OdUInt32 opt = kDefault) = 0;

  /** \details
    Returns true if undo information is stored inside this object
    and can be taken back via call popData.
  */
  virtual bool hasData() const = 0;

  /** \details
    Gives back most recent single undo step data.
    \param pStream [out]  stream
    \remarks
    Implementer should write back single undo step data
    saved in this object via the most recent call pushData into specified stream
    and then free internal resources used for storing undo step.
  */
  virtual OdUInt32 popData(OdStreamBuf* pStream) = 0;

  virtual OdRxIteratorPtr newRecordStackIterator() const = 0;

  virtual void clearData() = 0;
};

/** \details
 This template class is a specialization of the OdSmartPtr class for OdDbUndoController object pointers.
*/
typedef OdSmartPtr<OdDbUndoController> OdDbUndoControllerPtr;


#endif // _ODDBUNDOCONTROLLER_INCLUDED_



