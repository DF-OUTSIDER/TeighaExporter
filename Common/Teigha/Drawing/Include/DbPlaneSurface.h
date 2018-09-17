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


#ifndef _DbPlaneSurface_h_Included_
#define _DbPlaneSurface_h_Included_

#include "TD_PackPush.h"
#include "DbSurface.h"

class OdDbRegion;

/** \details
    This class implements a three-dimensional planar surface entity.
    
    \sa
    TD_Db
   
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbPlaneSurface : public OdDbSurface
{
public:
  /** \details
  Creates a plane surface.
  */
  OdDbPlaneSurface ();
  ODDB_DECLARE_MEMBERS(OdDbPlaneSurface);

  /** \details
  Creates the plane surface from a specified region.
  
  \param pRegion [in]  A pointer to a specified region that the plane surface must be created from.
  
  \returns Returns eOk if the plane surface was successfully created or an appropriate error otherwise.
  */
  virtual OdResult createFromRegion( OdDbRegion *pRegion );

  // OdDbObject methods

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual bool isDependent() const { return false; }

  virtual OdResult subExplode(OdRxObjectPtrArray& entitySet) const;

  /** \details
  Returns the plane that contains this entity.

  \param plane      [out] Receives the plane that contains this entity.
  \param planarity  [out] Receives the planarity of this entity.

  \returns Returns eOk if successful or an appropriate error code if not.

  \remarks 
  Output parameters planarity and plane are returned as described below:

  <table>
  planarity Value Description plane
  kPlanar   1     Planar      Entity plane
  </table>
  */
  virtual OdResult getPlane(OdGePlane& plane, OdDb::Planarity& planarity) const;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbPlaneSurface object pointers.
*/  
typedef OdSmartPtr<OdDbPlaneSurface> OdDbPlaneSurfacePtr;

#include "TD_PackPop.h"
#endif //_DbPlaneSurface_h_Included_
