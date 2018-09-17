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


#ifndef __IPSHELPER_H__
#define __IPSHELPER_H__

#include "PSExport.h"
#include "OdStreamBuf.h"

class PS_EXPORT IPSHelper
{
public:
  /** \details
      Loads the specified Parasolid file. 
      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
      \param typeVer [out]  Receives the type and version of the Parasolid file.
      \param standardSaveFlag [in]  Controls the saving and restoring of use count data in the save file.
      \returns
      Returns true if and only if successful
  */
  virtual bool in ( OdStreamBuf *pStreamBuf ) = 0;
  /** \details
      Saves the specified Parasolid file. 
      \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.
      \param typeVer [in]  Type and version of the Parasolid file.
      \param standardSaveFlag [in]  Controls the saving and restoring of use count data in the save file.
      \returns
      Returns true if and only if successful
  */

  virtual void *getIBr( ) = 0;

  virtual ~IPSHelper();
};

PS_EXPORT IPSHelper* createPSHelper();

PS_EXPORT void deletePSHelper(IPSHelper* pIC);

#endif // __IPSHELPER_H__
