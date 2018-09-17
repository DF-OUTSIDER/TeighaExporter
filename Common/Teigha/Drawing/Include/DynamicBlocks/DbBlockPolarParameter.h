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

#ifndef _DbBlockPolarParameter_h_Included_
#define _DbBlockPolarParameter_h_Included_

#include "DbBlock2PtParameter.h"
#include "DbBlockParamValueSet.h"

class OdDbBlockPolarGrip;
typedef OdSmartPtr<OdDbBlockPolarGrip>OdDbBlockPolarGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPolarParameter : public OdDbBlock2PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockPolarParameter);
  OdDbBlockPolarParameter();
  //OdString angleDescription() const;
  //OdString angleName() const;
  //const OdDbBlockParamValueSet& angleValueSet() const;
  //double baseAngle() const;
  //OdGePoint3d baseAnglePoint() const;
  //OdString distanceDescription() const;
  //OdString distanceName() const;
  //const OdDbBlockParamValueSet& distanceValueSet() const;
  //OdDbBlockPolarGripPtr getAssociatedPolarGrip( OdDbBlockParameter::ParameterComponent, OdDb::OpenMode );
  //double offset();
  //void setAngleDescription(const OdString&);
  //void setAngleName(const OdString&);
  //void setAngleValueSet(class OdDbBlockParamValueSet const &);
  //void setDistanceDescription( const OdString&);
  //void setDistanceName( const OdString&);
  //void setDistanceValueSet(const OdDbBlockParamValueSet &);
  //void setOffset(double);
  //void setUpdatedAngle(double);
  //void setUpdatedDistance(double);
};

typedef OdSmartPtr<OdDbBlockPolarParameter> OdDbBlockPolarParameterPtr;

#endif //_DbBlockPolarParameter_h_Included_
