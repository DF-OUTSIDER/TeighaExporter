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

#ifndef ODDGNLS_DBLSSYMBOLCOMPONENT_H
#define ODDGNLS_DBLSSYMBOLCOMPONENT_H

#include "DbLSComponent.h"

#include "TD_PackPush.h"

/** \details
    Dgn Line Style Symbol Component.
    \sa
    TD_Db
    <group OdDbLS_Classes> 
*/
class ODDGNLS_EXPORT OdDbLSSymbolComponent : public OdDbLSComponent
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbLSSymbolComponent);

    OdDbLSSymbolComponent();

    /** \details
      Returns block table record identifier associated with this symbol component.
    */
    OdDbObjectId blockTableRecord() const;
    /** \details
      Sets block table record identifier associated with this symbol component.

      \param blockId [in] Block table record identifier to set.
    */
    void setBlockTableRecord(const OdDbObjectId &blockId);

    /** \details
      Compute geometry extents for this symbol component.

      \param extents [out]  Output geometry extents.
      \returns
      Returns error code if computation failed.
    */
    OdResult getGeomExtents(OdGeExtents3d& extents) const;

    /** \details
      Returns true if unit scale is present.
    */
    bool hasUnitScale() const;
    /** \details
      Reset flag which indicates that unit scale is present.

      \param bSet [in]  New flag to set.
    */
    void setHasUnitScale(bool bSet);

    /** \details
      Returns true if symbol was marked as 3D.
    */
    bool Is3d() const;
    /** \details
      Marks symbol as 3D.

      \param bSet [in]  New value to set.
    */
    void setIs3d(bool bSet);

    /** \details
      Returns unit scale stored in Design.

      \remarks
      For backward conversion.
    */
    double storedUnitScale() const;
    /** \details
      Sets stored unit scale.

      \param fScale [in]  Unit scale to set.
    */
    void setStoredUnitScale(double fScale);

    /** \details
      Returns symbol unit scale.
    */
    double unitScale() const;
    /** \details
      Sets symbol unit scale.

      \param fScale [in]  Unit scale to set.
    */
    void setUnitScale(double fScale);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbLSSymbolComponent object pointers.
*/
typedef OdSmartPtr<OdDbLSSymbolComponent> OdDbLSSymbolComponentPtr;

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSSYMBOLCOMPONENT_H
