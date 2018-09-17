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
// 2Support.cpp : for keeping some Auxiliary methods.
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "2dSupport.h"

///////////////////////////////////////////////////////////////////////////////
// Auxiliary methods
///////////////////////////////////////////////////////////////////////////////
namespace TD_2D_EXPORT {
char* getUnitNameString(OdUInt32 idUnit)
{
  static const char* const Units[] = 
  {
    "", /* 0	Unspecified (No units) */
    "Inches",
    "Feet",
    "Miles",
    "Millimeters",
    "Centimeters",
    "Meters",
    "Kilometers",
    "Microinches",
    "Mils",
    "Yards",
    "Angstroms",
    "Nanometers",
    "Microns",
    "Decimeters",
    "Decameters",
    "Hectometers",
    "Gigameters",
    "Astronomical Units",
    "Light Years",
    "Parsecs"
  };

  if (idUnit > 20)
  {
    idUnit = 0;
  }

  return (char*)Units[idUnit];
}


//**************************************************************************************
void calculateExtents( OdGePoint3d& minExtent, 
                       OdGePoint3d& maxExtent, 
                       OdGePoint3dArray& array
                      /*, const OdGeMatrix3d& matrix*/
                     )
{
  int nSize = array.size();
/*
  for (int k = 0; k < nSize; k ++)
  {
    array[k].transformBy(matrix);
  }
*/
  minExtent = array[0];
  maxExtent = array[0];

  // min & max
  for (int i = 1; i < nSize; i++)
  {
    if (minExtent.x > array[i].x)
    {
      minExtent.x = array[i].x;
    }
    if (minExtent.y > array[i].y)
    {
      minExtent.y = array[i].y;
    }
    if (maxExtent.x < array[i].x)
    {
      maxExtent.x = array[i].x;
    }
    if (maxExtent.y < array[i].y)
    {
      maxExtent.y = array[i].y;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Geometry methods
///////////////////////////////////////////////////////////////////////////////

//**************************************************************************************
// IsPolygonRectIntersection2d()
//
//  Returns true if at least one of polygon's segments intersects rectangle 
//**************************************************************************************
bool IsPolygonRectIntersection2d(OdUInt32 nPoints, 
                                 const OdGePoint2d* pPoints, 
                                 const OdGePoint2d& ptMin, 
                                 const OdGePoint2d& ptMax)
{
  // Loop polygon per segment. 
	//  If any two edges (polygon's & rectangele's) are intersected
  //  then the polygon and rectangle are intersected.
	for (OdUInt32 i = 0, j = nPoints - 1; i < nPoints; j = i++)
  {
    if (!isSegmentOutOfRect( OdGeLineSeg2d(pPoints[i], pPoints[j]), ptMin, ptMax ))
    {
      return true;
    }
  }
	return false;
}


//**************************************************************************************
//	PolygonOutOfRect2d()
//
//	  Returns true if the polygon is completely out of the rectangle.
//**************************************************************************************
bool PolygonOutOfRect2d(OdUInt32 nPoints, 
                        const OdGePoint2d* pPoints, 
                        const OdGePoint2d& ptMin, 
                        const OdGePoint2d& ptMax)
{
	// If any two edges (polygon's & rectangele's) are intersected
  //  then the polygon is inside rectangle completely or partly.
	if (IsPolygonRectIntersection2d(nPoints, pPoints, ptMin, ptMax))
	{
		return false;
	}

	// In case the polygon & the rectangle aren't intersected
	//  if one point of the first one is inside the second one 
  //  then the whole polygon is inside in rectangle
	return (!isPtInRect(pPoints[0], ptMin, ptMax));
}


//**************************************************************************************
//
// isOutOfRect()
//
//  Returns true if segment is completely out of the rectangle
//**************************************************************************************
bool isSegmentOutOfRect(const OdGeLineSeg2d& lineSeg, 
                        const OdGePoint2d& ptMin, const OdGePoint2d& ptMax)
{
  // Tolerance used in calculations. Should not take it less then 1.0e-8 - 
  // there will be some problems. It seems that problems is due to big
  // numeric error (because there are a lot of matrix operations)
  OdGeTol tolerance(1.0e-6);

  OdGePoint2d ptInt;
  return 
    
    (    lineSeg.intersectWith( OdGeLineSeg2d(OdGePoint2d(ptMin.x, ptMin.y), 
                                OdGePoint2d(ptMax.x, ptMin.y)), ptInt, tolerance)
      || lineSeg.intersectWith( OdGeLineSeg2d(OdGePoint2d(ptMax.x, ptMin.y), 
                                OdGePoint2d(ptMax.x, ptMax.y)), ptInt, tolerance)
      || lineSeg.intersectWith( OdGeLineSeg2d(OdGePoint2d(ptMax.x, ptMax.y), 
                                OdGePoint2d(ptMin.x, ptMax.y)), ptInt, tolerance)
      || lineSeg.intersectWith( OdGeLineSeg2d(OdGePoint2d(ptMin.x, ptMax.y), 
                                OdGePoint2d(ptMin.x, ptMin.y)), ptInt, tolerance)
	      // The segment does not intersect any edge of rectangle.
	      //  In this case if one point of the segment is inside the rectangle 
        //  then the whole segment is inside one
      || isPtInRect(lineSeg.startPoint(), ptMin, ptMax)
     )
      ? false
      : true;
}



//**************************************************************************************
//
// isInRect()
//
// Returns true if the point is inside the rectangle
//**************************************************************************************
bool isPtInRect(const OdGePoint2d& pt, const OdGePoint2d& ptMin, const OdGePoint2d& ptMax)
{
  return (   pt.x >= ptMin.x && pt.y >= ptMin.y
          && pt.x <= ptMax.x && pt.y <= ptMax.y
         ) 
          ? true
          : false;
}
}
