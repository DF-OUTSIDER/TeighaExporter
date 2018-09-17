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



#ifndef _INC_DDBRBREP_3F82B32C036B_INCLUDED
#define _INC_DDBRBREP_3F82B32C036B_INCLUDED

#include "Br/BrEntity.h"

#include "TD_PackPush.h"

class OdGeMatrix3d;
/** \details
    This class is the interface class for the BREP portions of OdDbDatabase objects.
    
    \remarks
    This class provides the top-level interface for traversing the BREP structure
    of the 3d data contained in an OdDb3dSolid (or other 3d) entity, 
    
    Corresponding C++ library: TD_Br
    <group OdBr_Classes>
*/
class ODBR_TOOLKIT_EXPORT OdBrBrep : public OdBrEntity
{
public:

  /** \details
    Sets the 3d data for this BREP object from the specified 3d file.
    \remarks
    See the sample file, OdBrEx.cpp in the Examples/OdBrEx folder
    for sample usage.
    \param pFile [in]  Pointer to the 3d file object.
  */
  void set(const void* pFile);

  /** \details
    Returns true and only if this BREP object contains valid 3d data..
  */
  bool isValid() const;

  /** \details
    Returns the transformation matrix for this BREP object.
    \param xfm [out]  Receives the transformation matrix.
  */
  bool getTransformation( OdGeMatrix3d& xfm ) const;

  OdBrBrep();
};

#include "TD_PackPop.h"

#endif /* _INC_DDBRBREP_3F82B32C036B_INCLUDED */

