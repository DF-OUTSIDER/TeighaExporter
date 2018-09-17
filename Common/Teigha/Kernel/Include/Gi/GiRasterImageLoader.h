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

#ifndef __ODGIRASTERIMAGELOADER_H__
#define __ODGIRASTERIMAGELOADER_H__

#include "GiExport.h"
#include "RxObject.h"
#include "GiRasterImage.h"
#include "DbBaseHostAppServices.h"

#include "TD_PackPush.h"

class OdGiContext;

/** \details
    Protocol extension for OdGiRasterImageLoader.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiRasterImageLoaderPE : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiRasterImageLoaderPE);

    OdGiRasterImageLoaderPE();
    /** \details
      Loads OdGiRasterImage from file.

      \param fileName [in] Input file name.
      \param pDb [in] Database.
      \param hint [in] FindFileHint value.
    */
    virtual OdGiRasterImagePtr loadRasterImage(const OdString &fileName, OdDbBaseDatabase* pDb, OdDbBaseHostAppServices::FindFileHint hint = OdDbBaseHostAppServices::kDefault) = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdGiRasterImageLoaderPE object pointers.
*/
typedef OdSmartPtr<OdGiRasterImageLoaderPE> OdGiRasterImageLoaderPEPtr;

/** \details
    Loads OdGiRasterImage from a file.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiRasterImageLoader : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiRasterImageLoader);

    OdGiRasterImageLoader();
    /** \details
      Loads OdGiRasterImage from a file.

      \param fileName [in] Input file name.
      \param giCtx [in] Input vectorization process data.
      \param hint [in] FindFileHint value.
    */
    virtual OdGiRasterImagePtr loadRasterImage(const OdString &fileName, const OdGiContext &giCtx, OdDbBaseHostAppServices::FindFileHint hint = OdDbBaseHostAppServices::kDefault);
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdGiRasterImageLoader object pointers.
*/
typedef OdSmartPtr<OdGiRasterImageLoader> OdGiRasterImageLoaderPtr;

#include "TD_PackPop.h"

#endif // __ODGIRASTERIMAGELOADER_H__
