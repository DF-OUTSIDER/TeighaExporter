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



#ifndef __ODGISHELLFACEWITHHOLESITERATOR__
#define __ODGISHELLFACEWITHHOLESITERATOR__

#include "TD_PackPush.h"

/** \details
    
    <group OdGi_Classes> 
*/
class OdGiShellFaceWithHolesIterator
{
  const OdGePoint3d* _points;
  const OdInt32* _faceList;
  OdInt32 _loopSize;
public:
  OdInt32Array loopCounts;

  OdGiShellFaceWithHolesIterator()
    : _points(points)
    , _faceList(faceList)
    , _loopSize(0)
  {
  }

  OdGiShellFaceWithHolesIterator(const OdGePoint3d* points, const OdInt32* faceList)
    : _points(points)
    , _faceList(faceList)
    , _loopSize(0)
  {
    loopCounts.push_back(_loopSize = *faceList);
  }

  const OdGePoint3d& operator *() const
  {
    return _points[*_faceList];
  }
  const OdGePoint3d* operator ->() const
  {
    return _points + *_faceList;
  }
  OdGiShellFaceIterator& operator ++()
  {
    if(_loopSize--)
      loopCounts.push_back(_loopSize = *faceList);
    ++_faceList;
    return *this;
  }
  OdGiShellFaceIterator operator ++(int)
  {
    OdGiShellFaceIterator res(*this);
    ++(*this);
    return res;
  }
};

#include "TD_PackPop.h"

#endif // __ODGISHELLFACEWITHHOLESITERATOR__
