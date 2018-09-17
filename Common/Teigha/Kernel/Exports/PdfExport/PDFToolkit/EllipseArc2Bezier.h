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


///////////////////////////////////////////////////////////////////////////////
//
// PdfExport.h - Open Design Pdf Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _ELLIPSEARC2BEZIER_
#define _ELLIPSEARC2BEZIER_

#include "Ge/GeCurve2d.h"

namespace TD_PDF
{
namespace AUX
{

typedef OdGePoint2d bzControlPoint;

struct bzCurveControlPoints
{
  bzControlPoint P0;
  bzControlPoint P1;
  bzControlPoint P2;
  bzControlPoint P3;
};

typedef OdArray<bzCurveControlPoints> bzBezierCurves;


// input :
// arc - can be OdGeEllipArc2d, OdGeCircArc2d
//
// output :
// pCurves - array of 4 x Besier curve control points
//
// return :
// 0 - ok
// 1 - arc type is not valid (not OdGeEllipArc2d or OdGeCircArc2d)
// 2 - zero sweep angle

int bzEllipseArc2Bezier(const OdGeCurve2d &arc, bzBezierCurves &pCurves);

}
}

#endif // _ELLIPSEARC2BEZIER_

