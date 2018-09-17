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

#ifndef _PDF2PRCEXPORTPARAMS_INCLUDED_
#define _PDF2PRCEXPORTPARAMS_INCLUDED_

#include "PdfExportDef.h"
#include "Ge/GeMatrix3d.h"
#include "RxSystemServices.h"
#include "RxDictionary.h"
#include "PdfExportParams.h"
#include "Gi/GiSubEntityTraitsData.h"

/** \details
    Auxiliary class for export from PRC to PDF format.
    <group OdExport_Classes>
*/
struct PDF2PRCExportParams
{
  OdRxDictionaryPtr m_extraOptions; // reserve
  OdUInt32 m_compressionLevel;
  OdGeMatrix3d m_bodyTransformationMatr;
  double m_dDeviation; // deviation for export as mesh mode
  bool m_exportAsBrep; // if set to false then will export as Mesh
  bool m_doWriteToStream; // do export context contents to stream
  OdUInt32 m_viewportIdx; // index of viewport (prc file) in pdf document where entity will be exported
  OdUInt32 m_viewIdx;
  const OdGiSubEntityTraitsData * m_pTraitsData;
  const OdGiSubEntityTraitsData * m_pByBlockTraitsData;
  PDF2PRCExportParams()
    : m_compressionLevel(0)
    , m_bodyTransformationMatr()
    , m_dDeviation(0.01)
    , m_exportAsBrep(true)
    , m_doWriteToStream(true)
    , m_viewportIdx(0)
    , m_viewIdx(0)
    , m_pTraitsData(NULL)
    , m_pByBlockTraitsData(NULL)
  {
    m_extraOptions = odrxCreateRxDictionary();
  }

  void setPRCCompression(PDF3D_ENUMS::PRCCompressionLevel compressionLevel, bool bCompressBrep, bool bCompressTessellation)
  {
    if ((!bCompressBrep) && (!bCompressTessellation))
    {
      SETBIT_0(m_compressionLevel, 0x00000004);
      SETBIT_0(m_compressionLevel, 0x00000008);
      return;
    }

    m_compressionLevel = compressionLevel;
    SETBIT(m_compressionLevel, 0x00000004, bCompressBrep);
    SETBIT(m_compressionLevel, 0x00000008, bCompressTessellation);
  }

  bool hasPrcBrepCompression(PDF3D_ENUMS::PRCCompressionLevel &compressionLev) const
  {
    if ((m_compressionLevel & 0x00000004) != 0)
    {
      // apply mask
      compressionLev = (PDF3D_ENUMS::PRCCompressionLevel)(m_compressionLevel & 0x00000003);
      return true;
    }
    return false;
  }

  bool hasPrcTessellationCompression() const { return ((m_compressionLevel & 0x00000008) != 0); }
};

#endif
