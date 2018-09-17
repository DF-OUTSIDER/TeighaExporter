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



#ifndef _INC_DDBRENTITY_3F819553013C_INCLUDED
#define _INC_DDBRENTITY_3F819553013C_INCLUDED


#include "OdaCommon.h"
#include "Br/BrExport.h"
#include "Br/BrEnums.h"

#include "TD_PackPush.h"
#include "SharedPtr.h"
#include "DbBaseSubentId.h"
#include "../Ge/Ge.h"

class OdBrBrep;
class OdBrHit;
class OdGePoint3d;
class OdGeLinearEnt3d;


/** \details
  <group OdBr_Classes>

  This class is the interface base class for BREP topology objects.

  Corresponding C++ library: TD_Br
*/
class ODBR_TOOLKIT_EXPORT OdBrEntity
{
public:

  /** \details
    Returns true if and only if there is no ACIS model associated with this Entity object. 
  */
  bool isNull() const;

  /** \details
    Returns true if and only if the specified Entity object is equivalent to this Entity object.
    
    \remarks
    Two OdBrEntity objects are equivalent if and only if they non-null, and have the
    same subentity path.
    
    \param pOtherEntity [in]  Pointer to any OdBrEntity object.
  */
  bool isEqualTo(
    const OdBrEntity* pOtherEntity) const;
  
  /** \details
    Returns the owner of this Entity object.
  */
  bool getBrep(OdBrBrep& brep) const;

  /** \details
    Validates the element.
  */
  bool checkEntity() const;

  /** \details
  Returns the FullSubentPath of this Entity object.
  see OdDbBody::setSubentPath also.
  */
  OdBrErrorStatus getSubentPath(OdDbBaseFullSubentPath& subPath) const;

  virtual ~OdBrEntity();

  // for internal usage only. OdDbBody::setSubentPath should be used instead.
  OdBrErrorStatus setSubentPath(OdBrBrep &brep, OdDbBaseFullSubentPath& subpath);

  OdBrErrorStatus getPointContainment(const OdGePoint3d&      point,
                                      OdGe::PointContainment& containment,
                                      OdBrEntity*&            brEntContainer ) const;

  OdBrErrorStatus getLineContainment( const OdGeLinearEnt3d& line,
                                      const OdUInt32&        iNumHitsWanted,
                                      OdUInt32&              iNumHitsFound,
                                      OdBrHit*&              brHit ) const;


/*
  void* getEntity() const;
  OdBrErrorStatus setEntity(void* pData, OdDbBaseFullSubentPath& subPath = *(OdDbBaseFullSubentPath*)NULL);

  // Geometric bounds (i.e., model space bounding box)
  OdBrErrorStatus getBoundBlock(OdGeBoundBlock3d& block) const;
*/

  // Validation
  OdBrErrorStatus setValidationLevel(const BrValidationLevel& level = *(BrValidationLevel*)NULL);
  OdBrErrorStatus getValidationLevel(BrValidationLevel& level) const;

  //BrEntityFlags
  OdUInt32 getFlags() const;

  OdBrErrorStatus getGsMarker(OdGsMarker& marker) const;
/*
  bool brepChanged() const;

  // Queries & Initialisers
  OdBrErrorStatus set(const BrValidationLevel& path);
  OdBrErrorStatus get(BrValidationLevel& path) const;
  OdBrErrorStatus set(OdBrBrepData* data);
  OdBrErrorStatus get(OdBrBrepData*& data) const;

  // Geometric properties
  OdBrErrorStatus getMassProps(OdBrMassProps& massProps,
    const double& dDensity     = *(double*)NULL,
    const double& dTolRequired = *(double*)NULL,
    double&       dTolAchieved = *(double*)NULL) const;
  OdBrErrorStatus getVolume(double& dVolume,
    const double& dTolRequired = *(double*)NULL,
    double&       dTolAchieved = *(double*)NULL) const;
  OdBrErrorStatus getSurfaceArea(double& dArea,
    const double& dTolRequired = *(double*)NULL,
    double&       dTolAchieved = *(double*)NULL) const;
  OdBrErrorStatus getPerimeterLength(double& dLength,
    const double& dTolRequired = *(double*)NULL,
    double&       dTolAchieved = *(double*)NULL) const;
*/

protected:
  void *m_pImp;
  OdSharedPtr<OdDbStubPtrArray> m_pFSubentPath;
  bool m_bIsValidate; //The enum has only 2 values.

  OdBrEntity();

  friend class OdBrEntityInternals;
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRENTITY_3F819553013C_INCLUDED */

