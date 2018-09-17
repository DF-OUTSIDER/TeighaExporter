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


#ifndef _SpBBox_h_Included_
#define _SpBBox_h_Included_

#include "Si/SiSpatialIndex.h"
#include "Ge/GeBoundBlock3d.h"

/** \details

    <group TD_Namespaces>
*/
namespace OdSi
{
  /** \details
      This class implements 3D Spatial Index bounding volumes.

      Corresponding C++ library: SpatialIndex
  */
  class BBox : public OdGeBoundBlock3d, public OdSiShape
  {
  public:
    enum ExtentsType
    {
      k2dExtents  = 0,
      k3dExtents  = 1
    };

    /** \details
      Explodes the specified extents box or bounding block into an array of points.
	  
      \param extents [in]  3D extents.
      \param points [out] Receives the array[8] of points.
      
      \returns
      Returns the ExtentsType of the specified exents.
      
      \remarks
      explodeExtent() returns one of the following:
      
      <table>
      Name          Value
      k2dExtents    0
      k3dExtents    1
      </table>
      
      The points array will start at extents.minPoint, and proceed clockwise about the 
      extents. It will then repeat this sequence at the top of the extents.
    */
    static ExtentsType explodeExtent(OdGePoint3d* points, const OdGeExtents3d& extents)
    {
       points[0] =    extents.minPoint();
       points[1].set( extents.minPoint().x, extents.maxPoint().y, extents.minPoint().z );
       points[2].set( extents.maxPoint().x, extents.maxPoint().y, extents.minPoint().z );
       points[3].set( extents.maxPoint().x, extents.minPoint().y, extents.minPoint().z );

       if(points->z == extents.maxPoint().z)
         return k2dExtents;

       points[4].set( extents.minPoint().x, extents.minPoint().y, extents.maxPoint().z );
       points[5].set( extents.minPoint().x, extents.maxPoint().y, extents.maxPoint().z );
       points[6] =    extents.maxPoint();
       points[7].set( extents.maxPoint().x, extents.minPoint().y, extents.maxPoint().z );

       return k3dExtents;
    }

	/** \details
      Explodes the specified extents box or bounding block into an array of points.
	  
      \param extents [in]  BoundBlock object.
      \param points [out] Receives the array[8] of points.
      
      \returns
      Returns the ExtentsType of the specified exents.
      
      \remarks
      explodeExtent() returns one of the following:
      
      <table>
      Name          Value
      k2dExtents    0
      k3dExtents    1
      </table>
      
      The points array will start at extents.minPoint, and proceed clockwise about the 
      extents. It will then repeat this sequence at the top of the extents.
    */
    static ExtentsType explodeExtent(OdGePoint3d* points, const OdGeBoundBlock3d& extents)
    {
      OdGeVector3d side1, side2, side3;
      extents.get(points[0], side1, side2, side3);

      points[1] = points[0] + side1;
      points[2] = points[0] + side1 + side2;
      points[3] = points[0] + side2;
      
      if(OdZero(side3.length()))
        return k2dExtents;
      
      points[4] = points[0] + side3;
      points[5] = points[1] + side3;
      points[6] = points[2] + side3;
      points[7] = points[3] + side3;
      
      return k3dExtents;
    }

    virtual bool contains( const OdGeExtents3d& extents, bool planar = false, const OdGeTol& tol = OdGeContext::gTol ) const
    {
      ODA_ASSERT( planar == false );
      
      if ( !OdGeBoundBlock3d::contains( extents.minPoint(), tol ) )
        return false;
      if ( !OdGeBoundBlock3d::contains( extents.maxPoint(), tol ) )
        return false;
      if ( !OdGeBoundBlock3d::contains( OdGePoint3d( extents.minPoint().x, 
        extents.maxPoint().y, extents.minPoint().z ), tol ) )
        return false;
      if ( !OdGeBoundBlock3d::contains( OdGePoint3d( extents.maxPoint().x, 
        extents.minPoint().y, extents.minPoint().z ), tol ) )
        return false;
      if(OdEqual(extents.maxPoint().z, extents.minPoint().z, tol.equalPoint()))
        return true;
      if ( !OdGeBoundBlock3d::contains( OdGePoint3d( extents.minPoint().x, 
        extents.maxPoint().y, extents.maxPoint().z ), tol ) )
        return false;
      if ( !OdGeBoundBlock3d::contains( OdGePoint3d( extents.maxPoint().x, 
        extents.minPoint().y, extents.maxPoint().z ), tol ) )
        return false;

      return true;
    }
    
    virtual bool intersects( const OdGeExtents3d& extents, bool planar = false, const OdGeTol& tol = OdGeContext::gTol) const
    {
      ODA_ASSERT( planar == false );
      if (isBox())
      {
        OdGePoint3d minPt, maxPt;
        getMinMaxPoints(minPt, maxPt);
        return !OdGeExtents3d(minPt, maxPt).isDisjoint(extents, tol);
      }
      else
        return !OdGeBoundBlock3d( extents.minPoint(), extents.maxPoint() ).isDisjoint( *this, tol );
    }

    virtual OdSiShape* clone() const
    {
      return new BBox(*this);
    }

    virtual void transform(const OdGeMatrix3d& mtx)
    {
      setToBox(false);
      transformBy(mtx);
      setToBoxOrtho();
    }

  /** \param p1 [in]  First point of a coordinate-aligned block.
    \param p2 [in]  Second point of a coordinate-aligned block.
    \param base [in]  Base of parallelepiped bounding block.
    \param side1 [in]  First side of parallelepiped bounding block.
    \param side2 [in]  Second side of parallelepiped bounding block.
    \param side3 [in]  Third side of parallelepiped bounding block.
    \param source [in]  Object to be cloned.

    \remarks
    The default constructor constructs a parallelepiped reduced to the coordinate origin.
    
    A parallelepiped is a parallelogram extruded in an arbitrary direction. 
  */
    BBox() {}
    BBox( const BBox& source ) : OdGeBoundBlock3d( source ) {}
    BBox( const OdGePoint3d& base, const OdGeVector3d& side1,
      const OdGeVector3d& side2, const OdGeVector3d& side3 ) 
      : OdGeBoundBlock3d( base, side1, side2, side3 )
    {
      setToBoxOrtho(side1, side2, side3);
    }
    BBox( const OdGePoint3d& p1, const OdGePoint3d& p2 )
      : OdGeBoundBlock3d( p1, p2 ){}
  };
}

#endif
