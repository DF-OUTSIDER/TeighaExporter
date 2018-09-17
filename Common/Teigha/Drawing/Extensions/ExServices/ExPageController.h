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

#ifndef _EX_DBPAGECONTROLLER_H_
#define _EX_DBPAGECONTROLLER_H_

#include "TD_PackPush.h"
#include "DbPageController.h"
#include "OdBinaryData.h"
#include "OdStreamBuf.h"
#include "Int64Array.h"
#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
  <group ExServices_Classes>  
  
  This PageController class implements unloading of objects in partially opened database.

  Library: Source code provided.
*/
class ExUnloadController : public OdDbPageController
{
public:
  ExUnloadController();

  int pagingType() const;
  OdStreamBufPtr read(Key key);
  bool write(Key& key, OdStreamBuf* pStreamBuf);
  void setDatabase(OdDbDatabase* pDb);
  OdDbDatabase* database();

private:
  OdDbDatabase* m_pDb;
};

typedef OdInt64Array Offsets;

/** \details
   <group ExServices_Classes> 

   This class implements unloading and paging to external storage.

   Library: Source code provided. 

   \remarks
   This class contains a very simple implementation of paging, with consecutive writing in a file.
*/
class ExPageController : public ExUnloadController
{
public:
  ExPageController();
  ~ExPageController();

  int pagingType() const;
  OdStreamBufPtr read(Key key);
  bool write(Key& key, OdStreamBuf* pStreamBuf);
  void setDatabase(OdDbDatabase* pDb);

private:
  std::map<OdUInt32, Offsets> m_mFreeOffsets;
  OdStreamBufPtr pPagingFile;
  OdBinaryData  m_buffTmp;

  OdInt64 getOffsetInPageFile(OdUInt32 length);
};
#include "TD_PackPop.h"

#endif // _EX_DBPAGECONTROLLER_H_
