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

//:> OdPolyPolygon3d.h: interface for the OdPolyPolygon3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OdPolyPolygon3d_H__0D83A6B9_F93A_4801_AB9B_47B6648AE23B__INCLUDED_)
#define AFX_OdPolyPolygon3d_H__0D83A6B9_F93A_4801_AB9B_47B6648AE23B__INCLUDED_

#include "TD_PackPush.h"

#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning ( disable : 4512 )  // assignment operator could not be generated
#endif

#include "RxObject.h"
#include "OdArray.h"
#include "Ge/GePoint3d.h"
#include "Int32Array.h"

/** \details
    This class implements multi-polygon objects.
    
    \remarks
    These objects are typically used for TrueType font outlines.
    
    <group Other_Classes>
*/
class OdPolyPolygon3d
{
	OdGePoint3dArray m_Points; // Points defining the polygons.
	OdInt32Array     m_Counts; // Counts of points in each polygon.

public:
  OdPolyPolygon3d() {}
	virtual ~OdPolyPolygon3d() {}

  /** \details
    Adds a new 3D polygon to this PolyPolygon object.
  */
	void newContour() { m_Counts.append(0); }

  /** \details
    Appends a point to the last polygon of this PolyPolygon object.
    \param point [in]  New point.
  */
  void addToCurrentContour(const OdGePoint3d& point) { m_Points.append(point); m_Counts[m_Counts.size() - 1]++; }

  void removeLastContour() 
  { 
    if ( m_Counts.size() )
    {
      for ( int i = 0; i < m_Counts[m_Counts.size()-1]; i++)
      {
        m_Points.removeLast(); 
      }
      m_Counts.removeLast();
    }
  } 

  /** \details
    Sets the GrowLength for the m_Points array.
    \remarks
    The GrowLength for the m_Counts array is set to 1/10 this value.
  */
    void setGrowLength(OdInt32 growLength) 
  { m_Points.setGrowLength(growLength); m_Counts.setGrowLength(growLength/10 > 0 ? growLength/10 : 1); }

  /*!DOM*/
  void setGrowLenght(OdInt32 growLength) { setGrowLength(growLength); }
    
  /** \details
    Returns the points defining the polygons.
  */
	OdGePoint3dArray& points() { return m_Points; }
	/** \details
    Returns the points defining the polygons.
  */
	const OdGePoint3dArray& points() const { return m_Points; }
    /** \details
        Returns the counts of points defining the polygons.
    */
	OdInt32Array& counts() { return m_Counts; }
	/** \details
        Returns the counts of points defining the polygons.
    */
	const OdInt32Array& counts() const { return m_Counts; }
};

#if defined(_MSC_VER)
#pragma warning (pop)
#endif

#include "TD_PackPop.h"

#endif // !defined(AFX_OdPolyPolygon3d_H__0D83A6B9_F93A_4801_AB9B_47B6648AE23B__INCLUDED_)

