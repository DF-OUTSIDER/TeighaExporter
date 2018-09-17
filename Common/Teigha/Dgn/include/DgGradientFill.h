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

#ifndef __DG_GRADIENTFILL_H__
#define __DG_GRADIENTFILL_H__


#include "TD_PackPush.h"


/** \details
    This class implements specifics of dgn gradient fill.

	Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/

class TG_EXPORT OdDgGradientFill
{
public:
  enum OdDgGradientType
  {
    kLinear        = 1,
    kCurved        = 2,
    kCylindrical   = 3,
    kSpherical     = 4,
    kHemispherical = 5
  };

  struct OdDgGradientKey
  {
    double         dKeyPosition;
    ODCOLORREF     clrKeyColor;
  };

public:
  OdDgGradientFill();
  virtual ~OdDgGradientFill();

  double getAngle() const;
  void   setAngle( double dAngle );
  double getWhiteIntensity() const;
  void   setWhiteIntensity( double dIntencity );
  double getShift() const;
  void   setShift( double dShift );
  OdDgGradientType getGradientType() const;
  void   setGradientType( OdDgGradientType iType );
  bool   getInvertFlag() const;
  void   setInvertFlag( bool bSet );
  bool   getOutlineFlag() const;
  void   setOutlineFlag( bool bSet );

  OdUInt16        getKeyCount() const;
  OdDgGradientKey getKey( OdUInt16 uKeyNumber ) const;
  bool            setKeyColor( OdUInt16 uKeyNumber, ODCOLORREF clrKey );
  bool            addKey( const OdDgGradientKey& key );
  bool            deleteKey( OdUInt16 uKeyNumber );
  void            setKeys( const OdArray<OdDgGradientKey>& arrKeys );

  bool   operator == (const OdDgGradientFill& gradFill) const;

private:

  double   m_dAngle;
  double   m_dWhiteIntensity;
  double   m_dShift;
  bool     m_bInvertFlag;
  bool     m_bOutlineFlag;
  OdDgGradientType m_uType;
  OdArray<OdDgGradientKey> m_keyArr;
};


#include "TD_PackPop.h"

#endif // __DG_GRADIENTFILL_H__
