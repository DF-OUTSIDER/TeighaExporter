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

#include "HoopsExport.h"
#include "HoopsExportImpl.h"
#include "Gs/GsBaseVectorizer.h"
#include "DynamicLinker.h"
#include "RxRasterServices.h"

#ifdef _TOOLKIT_IN_DLL_
#ifdef ODA_WINDOWS
  #ifdef _DEBUG
    #define HOOPS_DLL_NAME OD_T("hoops2141_vc11d.dll")
  #else
    #define HOOPS_DLL_NAME OD_T("hoops2141_vc11.dll")
  #endif
#else 
  #define HOOPS_DLL_NAME OD_T("hoops2141")
#endif
#endif

OdUInt32 HOOPSEXPORT_DLL exportHoops( HoopsExportParams &params )
{
  OdUInt32 result = exOk;

#ifdef _TOOLKIT_IN_DLL_
#ifdef ODA_WINDOWS
  HMODULE hmHoops = ::LoadLibrary( HOOPS_DLL_NAME );
  if( hmHoops != NULL  )
#endif
#endif
  {
    try
    {
      HoopsResultEx res;
      CHoopsExportImpl hoopsExport;

      odgsInitialize();

      res = hoopsExport.init( params );

      if ( res == exOk )
      {
        res = hoopsExport.run();

        if ( res == exOk )
        {
          res = hoopsExport.exportFile();
        }
      }

      odgsUninitialize();

      result = res;
#ifdef _TOOLKIT_IN_DLL_
#ifdef ODA_WINDOWS
      ::FreeLibrary( hmHoops );
#endif
#endif
    }
    catch(const OdError &ex)
    {
      result = exOdError | (ex.code() & 0xFFFF);

#ifdef _TOOLKIT_IN_DLL_
#ifdef ODA_WINDOWS
      if( hmHoops )
        ::FreeLibrary( hmHoops );
#endif
#endif
    }
    catch(...)
    {
      result = exInternalError;

#ifdef _TOOLKIT_IN_DLL_
#ifdef ODA_WINDOWS
      if( hmHoops )
        ::FreeLibrary( hmHoops );
#endif
#endif
    }
  }
#ifdef _TOOLKIT_IN_DLL_
#ifdef ODA_WINDOWS
  else
    result = exInternalError;
#endif
#endif

  return result;
}

OdString HOOPSEXPORT_DLL exportHoopsErrorCode(OdUInt32 errorCode)
{
  switch (errorCode)
  {
    #define HOOPS_ERROR_MESSAGE_DEFS(a, b) case a: return b;
    #include "HoopsResultExCodes.h"
  };

  switch (errorCode)
  {
    #define HOOPS_ERROR_MESSAGE_DEFS(a, b) case a: return b;
    #include "HoopsResultExCodes.h"
  };

  if (errorCode & 0x20000)
  {
    errorCode &= ~0x20000;
    OdError dderror( (OdResult)errorCode );

    OdString str = dderror.description();
    if (!str.isEmpty())
      return str;
  }

  return OD_T("Undefined error.");
}
