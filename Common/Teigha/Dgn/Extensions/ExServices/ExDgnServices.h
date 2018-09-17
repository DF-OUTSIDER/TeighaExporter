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
#ifndef _EX_DGNSERVICES_H_
#define _EX_DGNSERVICES_H_


//#include "OdaCommon.h"
//#include "RxSystemServices.h"
#include "ExSystemServices.h"

/** *********************************************************************
*/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* ExSystemServices and ExHostAppServices                               */ 
/** *********************************************************************
*/
// TODO: Redesign ExSystemServices - may be template : ExSystemServices<TBaseClass>.
// ExSystemServices is inherited from OdDgSystemServices
// and can not be used without Teigha for .dwg files database,
// but OdDgSystemServices has no methods - only constructor/destructor.
//

/** \details
  <group Dg_Extension_Classes>

  This class implements the root run-time service object for dgn application.
*/
class OdExDgnSystemServices : public ExSystemServices
{
public:
  OdExDgnSystemServices() {}
};

#ifdef TG_SystemServices
class OdExDgnSystemServices : public ExSystemServices
{
  static OdRxClass* g_pDesc;                                                                         
public:                                                                                                                                                                                                         \
  /** \details
    Returns the OdRxClass description instance associated with this object.        
    \note This function is for use only when the class type of this object is unknown.  
    If the class type of this object is known, use instead desc().                            
  */
    virtual OdRxClass* isA() const;                                                                    
public:
  OdExDgnSystemServices(){}

  OdStreamBufPtr createFile( const OdString& filename,                            
                             Oda::FileAccessMode accessMode = Oda::kFileRead,  
                             Oda::FileShareMode shareMode = Oda::kShareDenyNo, 
                             Oda::FileCreationDisposition creationDisposition = Oda::kOpenExisting );

  bool accessFile( const OdString& filename, int accessMode );

  OdInt64 getFileCTime( const OdString& /*filename*/ )
  { 
    return 0; 
  }
  OdInt64 getFileMTime( const OdString& /*filename*/ )
  { 
    return 0; 
  }
  OdInt64 getFileSize( const OdString& /*filename*/ )
  { 
    return 0; 
  }

  OdString formatMessage( unsigned int formatId, va_list* argList = 0 );

  OdCodePageId systemCodePage() const
  { 
    return CP_UNDEFINED; 
  }
};
#endif
#endif //_EX_DGNSERVICES_H_
