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

#ifndef _DbBlock2PtParameter_h_Included
#define _DbBlock2PtParameter_h_Included

#include "DbBlockParameter.h"
class OdDbBlockAction;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlock2PtParameter : public OdDbBlockParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlock2PtParameter);
  enum ParameterBaseLocation { start = 0, mid = 1 };
  OdGePoint3d basePoint() const;
  OdGePoint3d endPoint() const;
  double distance() const;
  double angle() const;
  ParameterBaseLocation baseLocation() const;
  OdGePoint3d definitionBasePoint() const;
  OdGePoint3d definitionEndPoint() const;
  double definitionDistance() const;
  double definitionAngle() const;
  OdGePoint3d updatedBasePoint() const;
  OdGePoint3d updatedEndPoint() const;
  double updatedDistance() const;
  double updatedAngle() const;
  OdGeVector3d normal() const;
  OdDbBlockGripPtr getAssociatedGrip( OdDbBlockParameter::ParameterComponent, OdDb::OpenMode );
  virtual void graphEvalEnd( bool );
  //bool parameterComponentDrivesAction( OdDbBlockParameter::ParameterComponent, const OdDbBlockAction* );
  bool parameterComponentHasAction( OdDbBlockParameter::ParameterComponent, const OdDbBlockAction*);
  void setBaseLocation(ParameterBaseLocation);
  void setBasePoint( const OdGePoint3d& );
  void setEndPoint(const OdGePoint3d &);
  void setDefinitionBasePoint( const OdGePoint3d& );
  void setDefinitionEndPoint( const OdGePoint3d&);
  void setUpdatedBasePoint(const OdGePoint3d &);
  void setUpdatedEndPoint(const OdGePoint3d &);
};

typedef OdSmartPtr<OdDbBlock2PtParameter>OdDbBlock2PtParameterPtr;

#endif //_DbBlock2PtParameter_h_Included
