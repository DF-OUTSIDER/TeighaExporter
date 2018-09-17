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


#ifndef _SpExtents_h_Included_
#define _SpExtents_h_Included_

#if _MSC_VER >= 1000
#pragma once
#endif

#include "Si/SiSpatialIndex.h"

/** \details

    <group TD_Namespaces>
*/
namespace OdSi
{

  /** \details
      This class implements 3D Spatial Index Extents objects.

      Corresponding C++ library: SpatialIndex
  */
  class ODSI_API Extent3d : public OdGeExtents3d, public OdSiShape
  {
  public:
    virtual bool contains( const OdGeExtents3d& extents, bool planar = false, const OdGeTol& tol = OdGeContext::gTol) const
    {
      if ( planar ) 
      {
        ODA_ASSERT ( extents.minPoint().z == 0 && extents.maxPoint().z == 0 );
      }
      return OdGeExtents3d::contains( extents, tol );
    }

	/** \details
      Returns true if and only if this Shape object intersects the specfied extents. 

      \param extents  [in]   Any 3D extents box.  
      \param planar  [in]  If and only if true, a planar computation is made. 
      \param tol  [in]  Tolerance. 
	  
	  \remarks
      intersects() returns one of the following.
      
      <table>
      Name    Value    Description
      true    ..       The specified extents intersect this Extents object.
      false   ..       The specified extents do not intersect this Extents object.
      </table>
    */
    virtual bool intersects( const OdGeExtents3d& extents, bool planar = false, const OdGeTol& tol = OdGeContext::gTol) const
    {
      if ( planar == false )
        return !isDisjoint( extents, tol );
      else
      {
        return !( extents.minPoint().x > (maxPoint().x + tol.equalPoint())
          || extents.minPoint().y > (maxPoint().y + tol.equalPoint())
          || minPoint().x > (extents.maxPoint().x + tol.equalPoint()) 
          || minPoint().y > (extents.maxPoint().y + tol.equalPoint()));
      }
    }

    enum IntersectResult 
    { 
      left    = 0,     // Coordinate is to the *left* of the *extents*.
      upon    = 1,     // Coordinate is *upon* the *extents*.
      right   = 2      // Coordinate is to the *right* of the *extents*.
    };
    
    
#ifndef SWIG
    /** \details
      Returns the intersection with this Extents object.

	  \param coordinate [in]  Coordinate to test.
      \param dimension [in]  X, Y, or Z.
      \param tol  [in]  Tolerance. 
      
      \remarks
      intersects() returns one of the following.
      
      <table>
      Name    Value    Description
      left    0        coordinate is to the left of this Extents object.
      upon    1        coordinate is upon this Extents object.
      right   2        coordinate is to the right of this Extents object.
      </table>
    */
    IntersectResult intersects( double coordinate, int dimension, double tol = OdGeContext::gTol.equalPoint() ) const
    {
      if ( m_min[dimension] > (coordinate + tol) ) return right;
      if ( m_max[dimension] < (coordinate - tol) ) return left;
      return upon;
    }
#endif
    /** \details
      Halves the specified dimension of this Extents object.
      \param dimension [in]  X, Y, or Z.
      \param moveRight [in]  True to move the right -hand edge, false for the left -hand.
    */
    void makeHalf( int dimension, bool moveRight )
    {
      if ( moveRight )
        m_max[dimension] = ( m_min[dimension] + m_max[dimension]) / 2;
      else 
        m_min[dimension] = ( m_min[dimension] + m_max[dimension]) / 2;
    }

    /** \details
      Doubles the specified dimension of this Extents object.
      \param dimension [in]  X, Y, or Z.
      \param moveRight [in]  True to move the right -hand edge, false for the left -hand.
    */
    void makeDouble( int dimension, bool moveRight )
    {
      if ( moveRight )
        m_max[dimension] += ( m_max[dimension] - m_min[dimension] );
      else 
        m_min[dimension] -= ( m_max[dimension] - m_min[dimension] );
    }
    /** \details
      Expands this Extents object to a square or cube of its largest dimension.
      \param planar [in]  True for 2D extents, false for 3D.
    */
    void makeCube( bool planar )
    {
      if ( m_min.isEqualTo( m_max ) ) return;
      double ext = odmax( m_max.x - m_min.x, m_max.y - m_min.y );
      if ( !planar ) ext = odmax( m_max.z - m_min.z, ext );
      m_max.x = m_min.x + ext;
      m_max.y = m_min.y + ext;
      if ( !planar ) m_max.z = m_min.z + ext;
    }
    
    Extent3d() {}
    /** \param min [in]  Minimum point.
      \param max [in]  Maximum point.
    */
    Extent3d( const OdGePoint3d& min, const OdGePoint3d& max )
      : OdGeExtents3d( min, max ) {}
    Extent3d( const OdGeExtents3d& source ) : OdGeExtents3d( source ) {}
    Extent3d( const OdGeExtents2d& source ) 
      : OdGeExtents3d( OdGePoint3d( source.minPoint().x, source.minPoint().y, 0 ), 
      OdGePoint3d( source.maxPoint().x, source.maxPoint().y, 0 )) {}
  };
}
#endif
