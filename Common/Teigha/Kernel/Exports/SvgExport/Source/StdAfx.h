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

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__64E3FF02_C5C2_4A39_A513_699E387BD989__INCLUDED_)
#define AFX_STDAFX_H__64E3FF02_C5C2_4A39_A513_699E387BD989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OdaCommon.h"
#include "OdString.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "RxDispatchImpl.h"
#include "SmartPtr.h"
//#include "OdArray.h"
#include "Gs/GsBaseVectorizer.h"
#include "Ge/GePoint3d.h"
#include "ColorMapping.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GePoint2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "RxVariantValue.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiShellFaceIterator.h"
#include "RxRasterServices.h"
#include "Gi/GiPerspectivePreprocessor.h"

#define STL_USING_MAP
#define STL_USING_SET
#define STL_USING_VECTOR
#include "OdaSTL.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__64E3FF02_C5C2_4A39_A513_699E387BD989__INCLUDED_)
