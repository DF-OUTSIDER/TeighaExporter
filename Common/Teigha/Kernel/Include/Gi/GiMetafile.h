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




#ifndef _ODGIMETAFILE_H_
#define _ODGIMETAFILE_H_

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    Represents a metafile within the Teigha vectorization framework.

    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiMetafile : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiMetafile);

  /** \details
      Returns the size of the metafile data within this object.
  */
  virtual OdUInt32 dataSize() const = 0;

  /** \details
      Returns the metafile data from this object, either in the older Windows format
      or in the new enhanced format.

        \param pBytes [out]  Receives the metafile data (caller must ensure that enough memory has been allocated).
  */
  virtual void bitsData(OdUInt8* pBytes) const = 0;

  /** \details
    Returns the metafile data from this object, either in the older Windows format
    or in the new enhanced format.

    \remarks
    If the OdUInt8* bitsData() version of this function returns NULL, 
    a direct pointer to the data could not be 
    returned, and the bitsData(OdUInt8* pBytes) version of this function
    should be called instead.
  */
  virtual const OdUInt8* bitsData() const { return 0; }
};

typedef OdSmartPtr<OdGiMetafile> OdGiMetafilePtr;

#include "TD_PackPop.h"

#endif //#ifndef _ODGIMETAFILE_H_
