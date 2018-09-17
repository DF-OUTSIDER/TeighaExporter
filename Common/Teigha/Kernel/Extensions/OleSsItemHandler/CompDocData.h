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
//
// CompDocData.h
//

#if !defined(OD_OLE_COMP_DOC_H_)
#define OD_OLE_COMP_DOC_H_

#define OD_PBRUSH_CLSID \
 { \
   0x0003000a, 0x0000, 0x0000, \
   { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } \
 }

# define OD_COMP_DOC_COMP_OBJ_DATA \
  { \
    0x01, 0x00, 0xFE, 0xFF, 0x03, 0x0A, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, /* unknown12 */ \
    0x0A, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, /* clsid */ \
    0x07, 0x00, 0x00, 0x00, 0x50, 0x42, 0x72, 0x75, 0x73, 0x68, 0x00, /* len + "PBrush" */ \
    0x07, 0x00, 0x00, 0x00, 0x50, 0x42, 0x72, 0x75, 0x73, 0x68, 0x00, /* len + "PBrush" */ \
    0x07, 0x00, 0x00, 0x00, 0x50, 0x42, 0x72, 0x75, 0x73, 0x68, 0x00, /* len + "PBrush" */ \
    0xF4, 0x39, 0xB2, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /* unknown16 */ \
  }

# define OD_COMP_DOC_OLE_DATA \
  { \
    0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00 \
  }

#endif // OD_OLE_COMP_DOC_H_
