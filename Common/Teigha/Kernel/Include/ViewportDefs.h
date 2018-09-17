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


#ifndef __OD_VIEWPORT_DEFS__
#define __OD_VIEWPORT_DEFS__

/** \details

    <group TD_Namespaces>
*/
namespace OdDb
{
  enum OrthographicView
  {
    kNonOrthoView = 0,
    kTopView      = 1,
    kBottomView   = 2,
    kFrontView    = 3,
    kBackView     = 4,
    kLeftView     = 5,
    kRightView    = 6
  };

  enum RenderMode
  {
    k2DOptimized                 = 0,
    kWireframe                   = 1,
    kHiddenLine                  = 2,
    kFlatShaded                  = 3,
    kGouraudShaded               = 4,
    kFlatShadedWithWireframe     = 5,
    kGouraudShadedWithWireframe  = 6,
    kMaxRenderMode
  };
}

#endif // __OD_VIEWPORT_DEFS__
