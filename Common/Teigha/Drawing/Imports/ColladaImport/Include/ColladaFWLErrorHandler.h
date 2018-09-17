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

#ifndef __COLLADA_FWLERRORHANDLER_H__
#define __COLLADA_FWLERRORHANDLER_H__

#include "COLLADASaxFWLIErrorHandler.h"


/** \details
  <group OdImport_Classes> 
*/
namespace TD_COLLADA_IMPORT
{
  /** \details
    This class implements the FWL error handler.
  */
  class FWLErrorHandler : public COLLADASaxFWL::IErrorHandler	
  {
  private:
	
  public:
    /** Constructor. 
    */
    FWLErrorHandler();
    /** Destructor. 
    */
    virtual ~FWLErrorHandler();

    /** If this method returns true, the loader stops parsing immediately. If severity is nor CRITICAL
	and this method returns true, the loader continues loading.
    */
    bool virtual handleError(const COLLADASaxFWL::IError* error);

  private:
    /** Disable default copy ctor. 
    */
    FWLErrorHandler( const FWLErrorHandler& pre );
    /** Disable default assignment operator. 
    */
    const FWLErrorHandler& operator= ( const FWLErrorHandler& pre );
  };
}

#endif // __COLLADA_FWLERRORHANDLER_H__
