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

#ifndef ODDGNLS_DBLSDEFINITION_H
#define ODDGNLS_DBLSDEFINITION_H

#include "DbLSObject.h"

#include "TD_PackPush.h"

enum MSLSUnits
{
  kMSLSMasterUnits = 0, // Master Units
  kMSLSUORS        = 1, // Design UORs
  kMSLSDeviceUnits = 2  // Device Units
};

/** \details
    Dgn Line Style Definition.
    \sa
    TD_Db
    <group OdDbLS_Classes> 
*/
class ODDGNLS_EXPORT OdDbLSDefinition : public OdDbLSObject
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbLSDefinition);

    OdDbLSDefinition();

    /** \details
      Returns initial component for Line Style.
    */
    OdDbObjectId component() const;
    /** \details
      Sets initial component for Line Style.

      \param compId [in]  Identifier of component.
    */
    void setComponent(const OdDbObjectId &compId);

    /** \details
      Returns component unique identifier.

      \remarks
      16 bytes of data.
    */
    const OdUInt8 *componentUID() const;

    /** \details
      Returns true if Line Style marked as continuous.
    */
    bool isContinuous() const;
    /** \details
      Marks Line Style as continuous.

      \param bSet [in]  New value to set.
    */
    void setIsContinuous(bool bSet);

    /** \details
      Returns true if Line Style comes from RSC file, elsewhere it is embedded in Design file.
    */
    bool isElement() const;
    /** \details
      Sets flag which indicate that this Line Style comes from RSC file.

      \param bSet [in]  New value to set.
    */
    void setIsElement(bool bSet);

    /** \details
      Returns true if Line Style marked as physical.

      \remarks
      If true, prevents the Global Line Style Scale and the Annotation Scale factors from being applied.
    */
    bool isPhysical() const;
    /** \details
      Marks this Line Style as physical.

      \param bSet [in]  New value to set.
    */
    void setIsPhysical(bool bSet);

    /** \details
      Returns true if Line Style marked to scale inside shared cells independently.
    */
    bool isSCScaleIndependent() const;
    /** \details
      Sets shared cell scale independent flag to this Line Style.

      \param bSet [in]  New value to set.
    */
    void setIsSCScaleIndependent(bool bSet);

    /** \details
      Returns true if Line Style marked as snappable.

      \remarks
      If true, allows you to snap to the individual components of a line style.
      If false, allows you to snap to the underlying line of a line style.
    */
    bool isSnappable() const;
    /** \details
      Sets snappable flag for this Line Style.

      \param bSet [in]  New value to set.
    */
    void setIsSnappable(bool bSet);

    /** \details
      Returns name of this Line Style.
    */
    OdString name() const;
    /** \details
      Sets name for this Line Style.

      \param str [in]  Name to set.
    */
    void setName(const OdString &str);

    /** \details
      Returns the style number for this Line Style.
    */
    OdInt32 styleNumber() const;
    /** \details
      Sets style number for this Line Style.

      \param nStyle [in]  Style number to set.
      \remarks
      Typically it is null.
    */
    void setStyleNumber(OdInt32 nStyle);

    /** \details
      Returns the Line Style scale stored in definition.

      \remarks
      Typically it is null.
    */
    double unitDef() const;
    /** \details
      Sets the Line Style scale stored in definition.

      \param fUnit [in]  New scale factor to set.
      \remarks
      It will be used as multiplier for unitScale if will be set as non-zero.
    */
    void setUnitDef(double fUnit);

    /** \details
      Returns unit scale for this Line Style.

      \remarks
      Typically it is 1.0.
    */
    double unitScale() const;
    /** \details
      Sets the unit scale factor for this Line Style.

      \param fUnit [in]  New scale factor to set.
    */
    void setUnitScale(double fUnit);

    /** \details
      Returns type of units used for this Line Style.

      \remarks
      Typically it is set to kMSLSMasterUnits.
    */
    MSLSUnits unitsType() const;
    /** \details
      Sets type of units used for this Line Style.

      \param type [in]  New type to set.
    */
    void setUnitsType(MSLSUnits type);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbLSDefinition object pointers.
*/
typedef OdSmartPtr<OdDbLSDefinition> OdDbLSDefinitionPtr;

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSDEFINITION_H
