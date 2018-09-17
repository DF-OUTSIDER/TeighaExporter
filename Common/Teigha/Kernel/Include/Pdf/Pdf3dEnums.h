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

#if _MSC_VER > 1000         // remove "PCH warning: cannot find a suitable header stop location"
#pragma once
#endif // _MSC_VER > 1000


#ifndef _PDF3DENUMS_INCLUDED_
#define _PDF3DENUMS_INCLUDED_

/** \details
    <group TD_Namespaces>
*/
namespace PDF3D_ENUMS
{

  /** \details
  PRC objects compression level.
  */
  enum PRCCompressionLevel
  {
    kA3DLooseCompression   = 0, // Compression with tolerance set to 0.001 mm
    kA3DMeddiumCompression,     // Compression with tolerance set to 0.01 mm
    kA3DHighCompression         // Compression with tolerance set to 0.1 mm
  };

  /** \details
  Enumeration representing the PRC rendering modes.
  */
  enum PRCRenderingMode
  {
    kDefaultMode = 0, //not applied
    kSolid,
    kSolidWireframe,
    kSolidOutline,
    kBoundingBox,
    kTransparent,
    kTransparentWireframe,
    kTransparentBoundingBox,
    kTransparentBoundingBoxOutline,
    kIllustration,
    kShadedIllustration,
    kWireframe,
    kShadedWireframe,
    kHiddenWireframe,
    kVertices,
    kShadedVertices
  };

}

#endif //_PDF3DENUMS_INCLUDED_
