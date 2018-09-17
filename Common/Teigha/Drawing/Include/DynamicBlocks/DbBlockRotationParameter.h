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

#ifndef _DbBlockRotationParameter_h_Included_
#define _DbBlockRotationParameter_h_Included_

#include "DbBlock2PtParameter.h"
#include "DbBlockParamValueSet.h"

class OdDbBlockRotationGrip;
typedef OdSmartPtr<OdDbBlockRotationGrip> OdDbBlockRotationGripPtr;


/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockRotationParameter : public OdDbBlock2PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockRotationParameter);
  OdDbBlockRotationParameter();
  OdString angleDescription() const;
  void setAngleDescription(const OdString &);
  OdString angleName() const;
  void setAngleName(const OdString &);
  double baseAngle() const;
  OdGePoint3d baseAnglePoint() const;
  double definitionBaseAngle() const;
  OdGePoint3d definitionBaseAnglePoint() const;
  OdDbBlockRotationGripPtr getAssociatedRotationGrip( OdDb::OpenMode );
  double offset() const;
  void setDefinitionBaseAnglePoint( const OdGePoint3d& );
  void setOffset(double);
  void setUpdatedAngle(double);
  void setValueSet( const OdDbBlockParamValueSet&);
  const OdDbBlockParamValueSet& valueSet() const;
};

typedef OdSmartPtr<OdDbBlockRotationParameter> OdDbBlockRotationParameterPtr;

#endif // _DbBlockRotationParameter_h_Included_
