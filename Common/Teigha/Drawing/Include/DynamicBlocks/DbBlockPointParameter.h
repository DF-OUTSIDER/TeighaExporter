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

#ifndef _DbBlockPointParameter_h_Included_
#define _DbBlockPointParameter_h_Included_

#include "DbBlock1PtParameter.h"

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPointParameter : public OdDbBlock1PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockPointParameter);
  OdDbBlockPointParameter();
  OdString positionName() const;
  void setPositionName(const OdString&);
  OdString positionDescription() const;
  void setPositionDescription(const OdString&);
  OdGePoint3d definitionLabelPoint() const;
  void setDefinitionLabelPoint( const OdGePoint3d& );
  //double definitionX() const;
  //double definitionY() const;
  //void setDefinitionX(double);
  //void setDefinitionY(double);
  //double updatedX();
  //double updatedY();
  //void setUpdatedX(double);
  //void setUpdatedY(double);
  //double X();
  //double Y();
  //void setX(double);
  //void setY(double);
  //OdDbBlockXYGripPtr getAssociatedXYGrip( OdDb::OpenMode );
};

typedef OdSmartPtr<OdDbBlockPointParameter> OdDbBlockPointParameterPtr;

#endif // _DbBlockPointParameter_h_Included_
