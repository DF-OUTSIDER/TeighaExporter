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
// Volumetric spatial index query shape. For perspective mode projections.

#ifndef _SpVolume_h_Included_
#define _SpVolume_h_Included_

#include "TD_PackPush.h"

#include "Si/SiSpatialIndex.h"
#include "Ge/GePlane.h"

namespace OdSi
{

class Volume : public OdSiShape
{
  protected:
    // Subset of OdGePlane functionality. Optimized allocation, transforms and accessibility.
    class PlaneImpl
    {
      OdGePoint3d  m_origin;
      OdGeVector3d m_normal;
      double       m_dist;
      public:
        PlaneImpl() : m_dist(0.0) { }
        PlaneImpl(const PlaneImpl &pl2) : m_origin(pl2.m_origin), m_normal(pl2.m_normal), m_dist(pl2.m_dist) { }
        PlaneImpl &operator =(const PlaneImpl &pl2)
        {
          m_origin = pl2.m_origin; m_normal = pl2.m_normal; m_dist = pl2.m_dist;
          return *this;
        }

        void set(const OdGePoint3d &origin, const OdGeVector3d &normal)
        {
          m_origin = origin;
          m_normal = normal;
          m_dist = -m_normal.dotProduct(m_origin.asVector());
        }
        void transformBy(const OdGeMatrix3d &xfm)
        {
          m_origin.transformBy(xfm);
          m_normal.transformBy(xfm);
          m_normal.normalize();
          m_dist = -m_normal.dotProduct(m_origin.asVector());
        }
        void getCoefficients(double& a, double& b, double& c, double& d) const
        {
          a = m_normal.x; b = m_normal.y; c = m_normal.z; d = m_dist;
        }
    };
    OdUInt32 m_nPlanes;
    //OdGePlane m_plane[6];
    PlaneImpl m_plane[6];
    bool m_planeValid[6];
  public:
    Volume() : m_nPlanes(0) {}
    Volume( const Volume& source ) : m_nPlanes(source.m_nPlanes)
    {
      for (OdUInt32 npl = 0; npl < m_nPlanes; npl++)
      {
        if (source.m_planeValid[npl])
          m_plane[npl] = source.m_plane[npl];
        m_planeValid[npl] = source.m_planeValid[npl];
      }
    }
    Volume( const OdGePoint3d& origin, const OdGeVector3d& zAxis, const OdGeVector3d& yAxis, const OdGeVector3d& xAxis,
            double xFov, double yFov, bool xFovAsAspect = false, bool yFovAsAspect = false,
            bool bNearPlane = false, double fNearPlane = 0.01,
            bool bFarPlane  = false, double fFarPlane  = 1.0 );

    // Si interface
    virtual bool contains( const OdGeExtents3d& extents, bool planar = false, const OdGeTol& tol = OdGeContext::gTol ) const;
    
    virtual bool intersects( const OdGeExtents3d& extents, bool planar = false, const OdGeTol& tol = OdGeContext::gTol ) const;

    virtual OdSiShape* clone() const
    {
      return new Volume(*this);
    }

    virtual void transform(const OdGeMatrix3d& mtx)
    {
      for (OdUInt32 npl = 0; npl < m_nPlanes; npl++)
      {
        if (m_planeValid[npl])
          m_plane[npl].transformBy(mtx);
      }
    }

    // misc
    static double fovToPlane(double fov, double len)
    {
      return 2.0 * (len * tan(fov * 0.5));
    }
    static double fovFromPlane(double plane, double len)
    {
      return atan(plane / len * .5) * 2.0;
    }
  protected:
    static bool planeFromTri(const OdGePoint3d &A, const OdGePoint3d &B, const OdGePoint3d &C, /*OdGePlane*/ PlaneImpl &pl)
    {
      OdGeVector3d u = B - A;
      OdGeVector3d v = C - A;
      if (!u.isZeroLength(1e-100) && !v.isZeroLength(1e-100))
      {
        u.normalize(1e-100);
        v.normalize(1e-100);
      }
      else
      {
        return false;
      }
      pl.set(A, u.crossProduct(v).normal());
      return true;
    }
    static bool aabbInsidePlane(const OdGePoint3d &min, const OdGePoint3d &max, const /*OdGePlane*/ PlaneImpl &plane, const OdGeTol &tol)
    {
      double pt[3];
      double pl[4];
      plane.getCoefficients(pl[0], pl[1], pl[2], pl[3]);
      pt[0] = (pl[0] > 0.0) ? max.x : min.x;
      pt[1] = (pl[1] > 0.0) ? max.y : min.y;
      pt[2] = (pl[2] > 0.0) ? max.z : min.z;
      return pl[0] * pt[0] + pl[1] * pt[1] + pl[2] * pt[2] + pl[3] >= /*0.0*/ -tol.equalVector();
    }
};

inline
Volume::Volume( const OdGePoint3d& origin, const OdGeVector3d& zAxis, const OdGeVector3d& yAxis, const OdGeVector3d& xAxis,
                double xFov, double yFov, bool xFovAsAspect, bool yFovAsAspect,
                bool bNearPlane, double fNearPlane,
                bool bFarPlane, double fFarPlane )
{
  // Aspect is width/height, so x is y*aspect, y is x/aspect
  ODA_ASSERT(!(xFovAsAspect && yFovAsAspect));
  double nearPlane_ = odmax(fNearPlane, 1.0); // near plane is required for plane calculation
  double planeWidth = 1., planeHeight = 1.;
  if (!xFovAsAspect)
    planeWidth = fovToPlane(xFov, nearPlane_);
  if (!yFovAsAspect)
    planeHeight = fovToPlane(yFov, nearPlane_);
  if (xFovAsAspect)
    planeWidth = xFov * planeHeight;
  if (yFovAsAspect)
    planeHeight = planeWidth / yFov;
  OdGeVector3d basisX(xAxis.normal()), basisY(yAxis.normal()), basisZ(zAxis.normal());
  //basisX = basisZ.crossProduct(basisY).normal(); // from input, for support isotropy
  OdGeVector3d deltaX = basisX * (planeWidth * 0.5);
  OdGeVector3d deltaY = basisY * (planeHeight * 0.5);
  OdGePoint3d nearPlane = origin + basisZ * nearPlane_;
  OdGePoint3d planeQuad[4] =
  {
    nearPlane - deltaX - deltaY,
    nearPlane + deltaX - deltaY,
    nearPlane + deltaX + deltaY,
    nearPlane - deltaX + deltaY
  };
  m_planeValid[0] = planeFromTri(planeQuad[0], origin, planeQuad[1], m_plane[0]);
  m_planeValid[1] = planeFromTri(planeQuad[1], origin, planeQuad[2], m_plane[1]);
  m_planeValid[2] = planeFromTri(planeQuad[2], origin, planeQuad[3], m_plane[2]);
  m_planeValid[3] = planeFromTri(planeQuad[3], origin, planeQuad[0], m_plane[3]);
  if (bNearPlane || bFarPlane)
  {
    m_planeValid[4] = bNearPlane;
    if (bNearPlane)
      m_plane[4].set(origin + basisZ * fNearPlane, basisZ);
    if (bFarPlane)
    {
      m_planeValid[5] = true;
      m_plane[5].set(origin + basisZ * fFarPlane, -basisZ);
      m_nPlanes = 6;
    }
    else
      m_nPlanes = 5;
  }
  else
    m_nPlanes = 4;
}

inline
bool Volume::contains( const OdGeExtents3d& extents, bool planar, const OdGeTol& tol ) const
{
  ODA_ASSERT( planar == false );
  OdUInt32 i;
  for (i = 0; i < m_nPlanes; i++)
  {
    if (!m_planeValid[i])
      continue;
    if (!aabbInsidePlane(extents.minPoint(), extents.maxPoint(), m_plane[i], tol))
      return false; // entirely outside
  }
  for (i = 0; i < m_nPlanes; i++)
  {
    if (!m_planeValid[i])
      continue;
    if (!aabbInsidePlane(extents.maxPoint(), extents.minPoint(), m_plane[i], tol))
      return false; // intersects
  }
  return true; // entirely inside
}

inline
bool Volume::intersects( const OdGeExtents3d& extents, bool planar, const OdGeTol& tol ) const
{
  ODA_ASSERT( planar == false );
  for (OdUInt32 i = 0; i < m_nPlanes; i++)
  {
    if (!m_planeValid[i])
      continue;
    if (!aabbInsidePlane(extents.minPoint(), extents.maxPoint(), m_plane[i], tol))
      return false;
  }
  return true;
}

} // OdSi

#include "TD_PackPop.h"

#endif // _SpVolume_h_Included_
