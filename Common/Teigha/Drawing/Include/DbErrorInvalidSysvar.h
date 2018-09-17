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


#ifndef _ODDBERRORINVALIDSYSVAR_INCLUDED_
#define _ODDBERRORINVALIDSYSVAR_INCLUDED_

/** \details
    This class implements OdError objects with Result codes of eInvalidIndex,
    and the names, lower limits and upper limits of the variables associated with the errors.
    
    Corresponding C++ library: TD_Db
    <group Error_Classes> 
*/
class TOOLKIT_EXPORT OdError_InvalidSysvarValue : public OdError
{
public:
  /** \param name [in]  Name of Sysvar.
    \param limmin [in]  Lower limit for Sysvar.
    \param limmax [in]  Upper limit for Sysvar.
  */
  OdError_InvalidSysvarValue(const OdString& name);
  OdError_InvalidSysvarValue(const OdString& name, int limmin, int limmax);
  OdError_InvalidSysvarValue(const OdString& name, double limmin, double limmax);
#if OD_SIZEOF_LONG == 4
  OdError_InvalidSysvarValue(const OdString& name, OdInt32 limmin, OdInt32 limmax);
#endif
  OdResBufPtr limmin() const;
  /** \details
    Returns the upper limit for the Sysvar associated with this OdError object.
  */
  OdResBufPtr limmax() const;
  /** \details
    Returns the name of the Sysvar associated with this OdError object.
  */
  OdString name() const;
};

/** \details
    This class implements OdError objects with Result codes of eUnknownSysvar,
    a the names  of the variables associated with the errors.
    
    Corresponding C++ library: TD_Db
    <group Error_Classes> 
*/
class TOOLKIT_EXPORT OdError_UnknownSysvar : public OdError
{
public:
  /** \param name [in]  Name of Sysvar.
  */
  OdError_UnknownSysvar(const OdString& name);
  /** \details
    Returns the name of the Sysvar associated with this OdError object.
  */
  OdString name() const;
};

/** \details
    This class implements OdError objects with Result codes of eInvalidInput,
    a the names of the variables associated with the errors.
    
    Corresponding C++ library: TD_Db
    <group Error_Classes> 
*/
class TOOLKIT_EXPORT OdError_ModelSpaceSysvar : public OdError
{
public:
  /** \param name [in]  Name of Sysvar.
  */
  OdError_ModelSpaceSysvar(const OdString& name);
  /** \details
    Returns the name of the Sysvar associated with this OdError object.
  */
  OdString name() const;
};

#endif
