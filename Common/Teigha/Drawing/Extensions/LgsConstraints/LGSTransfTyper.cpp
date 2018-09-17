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
#include "OdaCommon.h"

#include "LGSTransfTyper.h"

LGSTransfTyper::LGSTransfTyper():m_matr(),
                                     m_type(TransTypeNo)
{
  
}

LGSTransfTyper::LGSTransfTyper(const OdGeVector3d& move):m_matr()
{
  if ( !move.isZeroLength() )
  {
    m_matr.setTranslation(move);
    m_type = TransTypeMove;
    m_move = move;
  }
  else
    m_type = TransTypeNo;
}

LGSTransfTyper::LGSTransfTyper(const OdGePoint3d& center, const double& ang):m_matr()
{
    m_matr.setToRotation(ang, OdGeVector3d::kZAxis, center);
    m_type = TransTypeRotate;

    m_center = center;
    m_ang = ang;
}

bool LGSTransfTyper::isFitTransform(const OdGePoint3d& src, const OdGePoint3d& dst)
{
  OdGePoint3d coord = src;
  if ( m_type != TransTypeNo )
    coord.transformBy(m_matr);

  return coord == dst;
}

bool LGSTransfTyper::isSimpleTypeTransformation() const
{
  return m_type == TransTypeMove || m_type == TransTypeRotate;
}

LGSTransfTyper::TransformationType LGSTransfTyper::getType() const
{
  return m_type;
}

void LGSTransfTyper::setType(LGSTransfTyper::TransformationType type)
{
  m_type = type;
}

bool LGSTransfTyper::getMoveParam(OdGeVector3d& move)
{
  if ( m_type != TransTypeMove )
    return false;

  move = m_move;
  return true;
}

bool LGSTransfTyper::getRotateParam(OdGePoint3d& center, double& ang)
{
  if ( m_type != TransTypeRotate )
    return false;

  center = m_center;
  ang = m_ang;

  return true;
}
