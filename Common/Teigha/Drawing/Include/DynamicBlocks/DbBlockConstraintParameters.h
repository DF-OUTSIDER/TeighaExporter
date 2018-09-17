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

#ifndef _DbBlockConstraintParameter_h_Included_
#define _DbBlockConstraintParameter_h_Included_

#include "DbBlock2PtParameter.h"
#include "DbBlockParamValueSet.h"

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockConstraintParameter : public OdDbBlock2PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockConstraintParameter);
  OdDbObjectId dependency();
  void setDependency(OdDbObjectId id);
};

typedef OdSmartPtr<OdDbBlockConstraintParameter> OdDbBlockConstraintParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockLinearConstraintParameter : public OdDbBlockConstraintParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockLinearConstraintParameter);
  OdDbBlockLinearConstraintParameter();
  OdString description() const;
  void setDescription(const OdString&);
  double value() const;
  void setValue(double v);
  const OdDbBlockParamValueSet& valueSet() const;
  void setValueSet(const OdDbBlockParamValueSet&);
};

typedef OdSmartPtr<OdDbBlockLinearConstraintParameter> OdDbBlockLinearConstraintParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockHorizontalConstraintParameter : public OdDbBlockLinearConstraintParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockHorizontalConstraintParameter);
  OdDbBlockHorizontalConstraintParameter();
};

typedef OdSmartPtr<OdDbBlockHorizontalConstraintParameter> OdDbBlockHorizontalConstraintParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockVerticalConstraintParameter : public OdDbBlockLinearConstraintParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockVerticalConstraintParameter);
  OdDbBlockVerticalConstraintParameter();
};

typedef OdSmartPtr<OdDbBlockVerticalConstraintParameter> OdDbBlockVerticalConstraintParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockAlignedConstraintParameter : public OdDbBlockLinearConstraintParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockAlignedConstraintParameter);
  OdDbBlockAlignedConstraintParameter();
};

typedef OdSmartPtr<OdDbBlockAlignedConstraintParameter> OdDbBlockAlignedConstraintParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockAngularConstraintParameter : public OdDbBlockConstraintParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockAngularConstraintParameter);
  OdDbBlockAngularConstraintParameter();
};

typedef OdSmartPtr<OdDbBlockAngularConstraintParameter> OdDbBlockAngularConstraintParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockDiametricConstraintParameter : public OdDbBlockConstraintParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockDiametricConstraintParameter);
  OdDbBlockDiametricConstraintParameter();
};

typedef OdSmartPtr<OdDbBlockDiametricConstraintParameter> OdDbBlockDiametricConstraintParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockRadialConstraintParameter : public OdDbBlockConstraintParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockRadialConstraintParameter);
  OdDbBlockRadialConstraintParameter();
};

typedef OdSmartPtr<OdDbBlockRadialConstraintParameter> OdDbBlockRadialConstraintParameterPtr;

#endif //_DbBlockConstraintParameter_h_Included_
