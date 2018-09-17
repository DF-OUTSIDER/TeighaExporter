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



#ifndef OD_GEQUATERNION_H
#define OD_GEQUATERNION_H /*!DOM*/

#include "Ge/GeExport.h"
#include "Ge/GeMatrix3d.h"


#include "TD_PackPush.h"

/** \details
    This class represents a rotation quaternion in 3D space.

    \remarks
    OdGeQuaternion may be viewed as an array[4] of doubles.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes>
*/
class GE_TOOLKIT_EXPORT OdGeQuaternion
{
public:

  /** Quaternions are used to specify the rotations necessary for 3d planar orientation.
    It stores 4 Double variables w, x, y, and z.



    \remarks
    When called with no arguments, constructs a zero quaternion.
  */
  OdGeQuaternion ()
    : w( 1. ), x( 0. ), y( 0. ), z( 0. )
  {
  }
  OdGeQuaternion (double ww, double xx, double yy, double zz)
    : w(ww), x(xx), y(yy), z(zz)
  {
  }

  OdGeQuaternion& set (double ww, double xx, double yy, double zz)
  {
    w = ww;
    x = xx;
    y = yy;
    z = zz;
    return *this;
  }

  OdGeQuaternion& set( const OdGeMatrix3d& matrix )
  {
      double trace = matrix[0][0] + matrix[1][1] + matrix[2][2];
      if ( trace > 0.0 )
      {
          w = sqrt(1.0 + trace) / 2.0;

          double w4 = (4.0 * w);
          x = (matrix[1][2] - matrix[2][1]) / w4;
          y = (matrix[2][0] - matrix[0][2]) / w4;
          z = (matrix[0][1] - matrix[1][0]) / w4;
      }
      else
      {
          OdUInt32 i = 0;
          if (matrix[1][1] > matrix[0][0]) i = 1;
          if (matrix[2][2] > matrix[i][i]) i = 2;
          OdUInt32 j = (2 == i ? 0 : i + 1);
          OdUInt32 k = (2 == j ? 0 : j + 1);

          double s = sqrt((matrix[i][i] - (matrix[j][j] + matrix[k][k])) + 1.0);
          double q[ 4 ];

          q[i] = s * 0.5;
          if (s != 0.0) s = 0.5/s;

          q[3] = (matrix[j][k] - matrix[k][j]) * s;
          q[j] = (matrix[i][j] + matrix[j][i]) * s;
          q[k] = (matrix[i][k] + matrix[k][i]) * s;

          x = q[0];
          y = q[1];
          z = q[2];
          w = q[3];
      }
      return( *this );
  }

  OdGeMatrix3d getMatrix() const
  {
    OdGeMatrix3d matrix;

    matrix( 0, 0 ) = w * w + x * x - y * y - z * z;
    matrix( 1, 0 ) = 2. * ( x * y - w * z );
    matrix( 2, 0 ) = 2. * ( w * y + x * z );

    matrix( 0, 1 ) = 2. * ( w * z + x * y );
    matrix( 1, 1 ) = w * w - x * x + y * y - z * z;
    matrix( 2, 1 ) = 2. * ( y * z - w * x );

    matrix( 0, 2 ) = 2. * ( x * z - w * y );
    matrix( 1, 2 ) = 2. * ( w * x + y * z );
    matrix( 2, 2 ) = w * w - x * x - y * y + z * z;

    return matrix;
  }

  OdGePoint3d rotate( OdGePoint3d& sourcePoint ) const
  {
    OdGeMatrix3d matrix = getMatrix();

    return sourcePoint.transformBy( matrix );
  }

  OdGeVector3d rotate( OdGeVector3d& vector ) const
  {
    OdGeMatrix3d matrix = getMatrix();

    return vector.transformBy( matrix );
  }

  /** \details
    Returns true if and only if quat is identical to this quaternion,
    within the specified tolerance.

    \param quat [in]  Any quaternion.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo(
    const OdGeQuaternion& quat,
    const OdGeTol& tol = OdGeContext::gTol) const
  {
    return fabs(x - quat.x) <= tol.equalVector() &&
           fabs(y - quat.y) <= tol.equalVector() &&
           fabs(z - quat.z) <= tol.equalVector() &&
           fabs(w - quat.w) <= tol.equalVector();
  }

  bool operator == (const OdGeQuaternion& quat) const
  {
    return isEqualTo(quat);
  }
  bool operator != (const OdGeQuaternion& quat) const
  {
    return !isEqualTo(quat);
  }

  double w;
  double x;
  double y;
  double z;

  GE_STATIC_EXPORT static const OdGeQuaternion kOrigin;

};

#include "TD_PackPop.h"


#endif // OD_GEQUATERNION_H
