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




#ifndef OD_GE_LIB_VERSION
#define OD_GE_LIB_VERSION /*!DOM*/

#include "GeExport.h"
#include "TD_PackPush.h"

#define IMAGE_MAJOR_VER 2           /*!DOM*/
#define IMAGE_MINOR_VER 0           /*!DOM*/
#define IMAGE_CORRECTIVE_VER 0      /*!DOM*/
#define IMAGE_INTERNAL_VER 0        /*!DOM*/

/** \details
    This class provides management of GeLib versions.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeLibVersion
{
public:
  /** \param major [in]  Major version.
    \param minor [in]  Minor version.
    \param corrective [in]  Corrective version.
    \param schema [in]  Schema version.
  */
  OdGeLibVersion ();
  OdGeLibVersion (
    const OdGeLibVersion& source);
  OdGeLibVersion (
    unsigned char major, 
    unsigned char minor,
    unsigned char corrective,
    unsigned char schema);

  /** \details
    Returns the major version of GeLib.
  */
  unsigned char majorVersion () const; 

  /** \details
    Returns the minor version of GeLib.
  */
  unsigned char minorVersion () const; 

  /** \details
    Returns the corrective version of GeLib.
  */
  unsigned char correctiveVersion () const;
   
  /** \details
    Returns the schema version of GeLib.
  */
  unsigned char schemaVersion () const;

  /** \details
    Sets the major version of GeLib.

    \param major [in]  Major version.
  */
  OdGeLibVersion& setMajorVersion (
    unsigned char major); 

  /** \details
    Sets the minor version of GeLib.

    \param minor [in]  Minor version.
  */
  OdGeLibVersion& setMinorVersion (
    unsigned char minor); 

  /** \details
    Sets the corrective version of GeLib.

    \param corrective [in]  Corrective version.
  */
  OdGeLibVersion& setCorrectiveVersion (
    unsigned char corrective); 

  /** \details
    Sets the schema version of GeLib.

    \param schema [in]  Schema version.
  */
  OdGeLibVersion& setSchemaVersion (
    unsigned char schema);

  bool operator == (
    const OdGeLibVersion& libVersion) const; 

  bool operator != (
    const OdGeLibVersion& libVersion) const; 

  bool operator < (
    const OdGeLibVersion& libVersion) const;

  bool operator <= (
    const OdGeLibVersion& libVersion) const; 

  bool operator > (
    const OdGeLibVersion& libVersion) const; 

  bool operator >= (
    const OdGeLibVersion& libVersion) const;

  GE_STATIC_EXPORT static const OdGeLibVersion kRelease0_95; // GeLib release 0.

  GE_STATIC_EXPORT static const OdGeLibVersion kReleaseSed; // GeLib 14.0 release. 

  GE_STATIC_EXPORT static const OdGeLibVersion kReleaseTah; // GeLib 15.0 release.

private:
  unsigned char mVersion[10];
};

#include "TD_PackPop.h"

#endif // OD_GE_LIB_VERSION

