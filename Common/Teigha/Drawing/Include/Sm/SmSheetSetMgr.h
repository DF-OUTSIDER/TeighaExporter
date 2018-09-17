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




#ifndef _SmSheetSetMgr_h_Included_
#define _SmSheetSetMgr_h_Included_

#include <RxObject.h>
#include <OdString.h>
#include "Sm/SmEvents.h"
#include "Sm/SmExport.h"
#include "Sm/SmSheetSet.h"
#include "Sm/SmSheet.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  typedef OdSmEnum<OdSmDatabase> OdSmEnumDatabase;
  typedef OdSmartPtr<OdSmEnumDatabase>OdSmEnumDatabasePtr;

  /** \details

       
  */
  struct SMDLL_EXPORT OdSmSheetSetMgr : public OdRxObject
  {
    ODRX_DECLARE_MEMBERS( OdSmSheetSetMgr );
    virtual OdSmDatabase* createDatabase( const OdString& filename, const OdString& templatefilename, bool bAlwaysCreate = true ) = 0;
    virtual OdSmDatabase* openDatabase( const OdString& filename, bool bFailIfAlreadyOpen = false, OdStreamBuf* file = 0 ) = 0;
    virtual OdSmDatabase* findOpenDatabase( const OdString& filename ) = 0;
    virtual void closeAll() = 0;
    virtual void close( OdSmDatabase* db ) = 0;
    virtual long subscribe( OdSmEvents * eventHandler ) = 0;
    virtual void unSubscribe( long cookie ) = 0;
    virtual OdSmSheetSet* getParentSheetSet( const OdString& dwg, const OdString& layout ) = 0;
    virtual OdSmSheet* getSheetFromLayout( const OdString& filename, const OdString& handle ) = 0;
    virtual OdSmEnumDatabasePtr getDatabaseEnumerator() const = 0;
  };
  
  typedef OdSmartPtr<OdSmSheetSetMgr> OdSmSheetSetMgrPtr;
}

#endif //_SmSheetSetMgr_h_Included_
