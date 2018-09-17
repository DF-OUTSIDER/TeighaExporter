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




#ifndef _SmDatabase_h_Included
#define _SmDatabase_h_Included

#include "Sm/SmComponent.h"
#include "Sm/SmCustomProperty.h"
#include "Sm/SmEvents.h"
#include "Sm/SmSheetSet.h"
#include "Sm/SmEnum.h"
#include "Sm/SmPersist.h"
#include "Sm/SmObjectId.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  typedef OdSmEnum<OdSmPersist> OdSmEnumPersist;
  typedef OdSmartPtr<OdSmEnumPersist> OdSmEnumPersistPtr;

  /** \details

       
  */
  struct SMDLL_EXPORT OdSmDatabase : OdSmComponent
  {
    ODRX_DECLARE_MEMBERS( OdSmDatabase );
    virtual void loadFromFile( const OdString& filename ) = 0;
    virtual void loadFromStream(OdStreamBuf* file) = 0;
    virtual void save(OdStreamBuf* file = 0) = 0;
    virtual OdString fileName() const = 0;
    virtual void setFileName ( const OdString& newVal ) = 0;
    virtual OdString templateDstFileName() const = 0;
    virtual OdSmSheetSet* sheetSet() = 0;
    virtual const OdSmSheetSet* sheetSet() const = 0;
    virtual OdSmEnumPersistPtr enumerator() const = 0;
    virtual long subscribe( OdSmEvents * eventHandler ) = 0;
    virtual void unSubscribe( long cookie ) = 0;
    virtual void notifyRegisteredEventHandlers( OdSmEvent event, OdSmPersist * comp ) const = 0;
    virtual OdSmPersist* findObject( const OdString& hand ) const = 0;
    virtual void findAcDbObjectReferences(const OdString& filename, const OdString& dbHandle, std::vector<OdSmPersist*>& ppObjects ) = 0;
    virtual void updateInMemoryDwgHints() = 0;
    virtual bool isTemporary() const = 0;
    virtual void setIsTemporary() = 0;
    virtual OdSmObjectIdPtr registerNewObject(const OdString& hand, OdSmPersist* pObject ) = 0;
    virtual OdString version() const = 0;
    virtual OdInt32 getRevision() const = 0;
    virtual OdString getFingerPrint() const = 0;
  };

  typedef OdSmartPtr<OdSmDatabase> OdSmDatabasePtr;
}


#endif
