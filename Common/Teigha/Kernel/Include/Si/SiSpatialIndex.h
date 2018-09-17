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

#ifndef _SpatialIndex_h_Included_
#define _SpatialIndex_h_Included_

#include "OdPlatformSettings.h"

#ifdef SPATIALINDEX_DLL_EXPORTS
  #define ODSI_API          OD_TOOLKIT_EXPORT
  #define ODSI_API_STATIC   OD_STATIC_EXPORT
#else
  #define ODSI_API          OD_TOOLKIT_IMPORT
  #define ODSI_API_STATIC   OD_STATIC_IMPORT
#endif

//////////////////////////////////////////////////////////////////////////

#include "RxObject.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeExtents2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents Spatial Index Shape objects.
    Corresponding C++ library: SpatialIndex
    <group OdSi_Classes>
*/
struct ODSI_API OdSiShape
{
  /** \details
    Returns true if and only if this Shape object contains the specfied extents.

    \param extents [in]  Any 3D extents box.
    \param planar [in]  If and only if true, a planar computation is made.
  */
  virtual bool contains( const OdGeExtents3d& extents, bool planar, const OdGeTol& tol = OdGeContext::gTol ) const = 0;
  /** \details
    Returns true if and only if this Shape object intersects the specfied extents.

    \param extents [in]  Any 3D extents box.
    \param planar [in]  If and only if true, a planar computation is made.
  */
  virtual bool intersects( const OdGeExtents3d& extents, bool planar, const OdGeTol& tol = OdGeContext::gTol ) const = 0;

  virtual ~OdSiShape() {};
  virtual OdSiShape* clone() const { return 0; }
  virtual void transform(const OdGeMatrix3d&) {};

  ODSI_API_STATIC static const OdSiShape& kOverallSpace;
  ODSI_API_STATIC static const OdSiShape& kNoSpace;
};
/** \details
    This class represents Spatial Index Entity objects.
    Corresponding C++ library: SpatialIndex

    <group OdSi_Classes>
*/
struct OdSiEntity
{
  /** \details
    Returns the extents of this Entity object.

    \param extents [out]  Receives the extents.
    \returns
    Returns true if and only if this Entity object has extents.
  */
  virtual bool extents(OdGeExtents3d& extents) const = 0;
};

/** \details
    This class represents Spatial Index Visitor Objects
    Corresponding C++ library: SpatialIndex
    <group OdSi_Classes>
*/
struct OdSiVisitor
{
  /** 
    \param entity [in] Pointer to any OdSiEntity object.
    completelyInside [in] unknown.
  */
  virtual void visit( OdSiEntity* entity, bool completelyInside ) = 0;
};

class ODSI_API OdSiSpatialIndex;
/** \details
    This template class is a specialization of the OdSmartPtr class for OdSiSpatialIndex object pointers.
*/
typedef OdSmartPtr<OdSiSpatialIndex> OdSiSpatialIndexPtr;

/** \details
    This class represents SpatialIndex objects.
    Corresponding C++ library: SpatialIndex
    <group OdSi_Classes>
*/
class ODSI_API OdSiSpatialIndex : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdSiSpatialIndex);

  /** \details
    Spatial Index creation flags.
  */
  enum CreationFlags
  {
    kSiNoFlags       = 0,

    kSiPlanar        = (1 << 0), // Create 2D Spatial Index.
    kSiModifyMtAware = (1 << 1), // Protect insert/remove/clear by mutex object.
    kSiAccessMtAware = (1 << 2), // Protect query/extents/tolerance by mutex object.

    kSiFullMtAware   = (kSiModifyMtAware | kSiAccessMtAware)
  };

  /** \details
    Creates a SpatialIndex object with the specified parameters.
    \param flags [in]  Specifies set of flags for Spatial Index creation.
    \param maxDepth [in]  Maximum depth of the Spatial Index.
    \param maxCount [in]  Maximum number of nodes in the SpatialIndex.
    \param eps [in]  Relative tolerance (equalVector).

    \returns
    Returns a SmartPointer to the new SpatialIndex object.
    \remarks
    maxDepth specifies the maximum number of times the tree-structured SpatialIndex can branch.
  */
  static OdSiSpatialIndexPtr createObject( OdUInt32 flags, unsigned int initialNumEntity, unsigned int maxDepth = 30, unsigned int maxCount = 20, double eps = 1e-10 );

  /** \details
    Inserts the specified SiEntity object into the this SpatialIndex object.
    \param entity [in]  Pointer to the OdSiEntity to insert.
  */
  virtual void insert( OdSiEntity* entity ) = 0;
  /** \details
    Removes the specified SiEntity object into the this SpatialIndex object.
    \param entity [in]  Pointer to the OdSiEntity to remove.
    \returns
    Returns true if and only if the specified entity was in this SpatialIndex object.
  */
  virtual bool remove( OdSiEntity* entity ) = 0;

  /** \details
    \param shape [in]  Any OdSiShape object.
    \param visitor [in]  Any OdSiVisitor object.
  */
  virtual void query( const OdSiShape& shape, OdSiVisitor& visitor ) const = 0;
  /** \details
    Clears this SpatialIndex object.
  */
  virtual void clear() = 0;
  /** \details
    Sets the maximum depth of this SpatialIndex object.
    \param maxDepth [in]  Maximum depth.
    \remarks
    maxDepth specifies the maximum number of times the tree-structured SpatialIndex can branch.
  */
  virtual void setMaxTreeDepth( unsigned char maxDepth ) = 0;
  /** \details
    Sets the maxiumum number of nodes in this SpatialIndex object.
    \param maxCount [in]  Maximum number of nodes.
  */
  virtual void setMaxNodeSize( unsigned char maxCount ) = 0;
  /** \details
    Returns the extents of this SpatialIndex object.

    \param extents [out]  Receives the extents.
    \returns
    Returns true if and only if this SpatialIndex object has extents.
  */
  virtual bool extents(OdGeExtents3d& extents) const = 0;

  /** \details
    Returns the maximum depth of this SpatialIndex object.
    \remarks
    maxDepth specifies the maximum number of times the tree-structured SpatialIndex can branch.
  */
  virtual unsigned maxTreeDepth() const = 0;

  /** \details
    Returns the maxiumum number of nodes in this SpatialIndex object.
  */
  virtual unsigned maxNodeSize() const = 0;

  /** \details
    Returns the tolerance of this SpatialIndex object.

    \returns
    Returns tolerance where equalPoint value is scaled by extents of SpatialIndex.
  */
  virtual const OdGeTol& tolerance() const = 0;

  virtual void setTolerance(const OdGeTol& tol) = 0;
};

namespace OdSi
{
bool ODSI_API properExtents(const OdGeExtents3d& ext);
}

#include "TD_PackPop.h"

#endif
