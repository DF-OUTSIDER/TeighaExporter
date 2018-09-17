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




#ifndef _SmFiler_h_Included_
#define _SmFiler_h_Included_

#include "RxObject.h"
#define STL_USING_STRING
#include "OdaSTL.h"
#include "Sm/SmObjectId.h"
#include <OdString.h>
#include <RxVariant.h>
#include <OdStreamBuf.h>

#ifndef GUID_DEFINED
struct GUID;
#endif

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  struct OdSmDatabase;
  struct OdSmObjectReference;

  /** \details

       
  */
  struct OdSmFiler : public OdRxObject 
  {
    virtual bool init( OdStreamBuf* file, OdSmDatabase *pDb, bool bForWrite ) = 0;
    virtual void close() = 0;
    virtual OdSmDatabase* getDatabase() = 0;
    // virtual void PushCurrentWriteObject( OdSmPersist * pObj ) = 0;
    // virtual void PopCurrentWriteObject(long bSuccess ) = 0;
    virtual void pushCurrentReadObject( const OdSmPersist * pObj ) = 0;
    virtual void popCurrentReadObject( bool bSuccess ) = 0;
    virtual const OdSmPersist* getCurrentObject() const = 0;
    // virtual void GetCurrentObjectHeaderInfo( /*[out]*/ SAFEARRAY * * names, /*[out]*/ SAFEARRAY * * values ) = 0;
    // virtual long GetRevisionNumber() = 0;
    virtual OdSmPersist* readObject() = 0;
    virtual bool readProperty( OdString& ppPropname, OdVariant& pVal ) = 0;
    virtual bool readRawData( OdStreamBuf* pStreamBuf ) = 0;
    virtual bool readReference( OdSmObjectReference* ) = 0;

    virtual void writeObject( const OdSmPersist* pObj ) = 0;
    virtual void writeProperty(const OdString& pPropname, const OdVariant& pVal ) = 0;
    virtual void writeRawData( OdStreamBuf* pStreamBuf ) = 0;
    virtual bool readGuid( GUID& ) = 0;
    virtual bool readInt( long& ) = 0;
    virtual bool readDouble( double& ) = 0;
    virtual bool readString( OdString& ) = 0;
    // virtual void ReadBytes( /*[out]*/ unsigned char * * p, /*[in,out]*/ long * count ) = 0;
    virtual void writeGuid( const GUID& id ) = 0;
    virtual void writeInt( long pInt ) = 0;
    virtual void writeDouble( double pDbl ) = 0;
    virtual void writeString(const OdString& s ) = 0;
    // virtual void WriteBytes( unsigned char * p, long count ) = 0;
    virtual void writeReference( const OdSmObjectReference* ) = 0;
  };
}

#endif //_SmFiler_h_Included_
