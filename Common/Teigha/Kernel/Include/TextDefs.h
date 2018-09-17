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


#ifndef __OD_TEXT_DEFS__
#define __OD_TEXT_DEFS__

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  enum TextHorzMode
  {
    kTextLeft   = 0,
    kTextCenter = 1,
    kTextRight  = 2,
    kTextAlign  = 3,
    kTextMid    = 4,
    kTextFit    = 5
  };

  enum TextVertMode
  {
    kTextBase    = 0,
    kTextBottom  = 1,
    kTextVertMid = 2,
    kTextTop     = 3
  };

  enum LineSpacingStyle
  {
    kAtLeast = 1, // Larger characters on lines will increase line spacing.
    kExactly = 2  // Line spacing is constant.
  };

}

#endif // __OD_TEXT_DEFS__
