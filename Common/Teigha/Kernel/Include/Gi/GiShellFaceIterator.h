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



#ifndef __ODGISHELLFACEITERATOR__
#define __ODGISHELLFACEITERATOR__

#include "TD_PackPush.h"


/** \details
    
    <group OdGi_Classes> 
*/
class OdGiShellFaceIterator
{
  const OdGePoint3d* _points;
  const OdInt32* _faceList;
public:
  inline OdGiShellFaceIterator()
    : _points(0)
    , _faceList(0)
  {
  }

  inline OdGiShellFaceIterator(const OdGePoint3d* points, const OdInt32* faceList)
  {
    set(points, faceList);
  }

  inline void set(const OdGePoint3d* points, const OdInt32* faceList)
  {
    _points = points;
    _faceList = faceList;
  }

  inline const OdInt32* faceListPos() { return _faceList; }

  inline const OdGePoint3d& operator *() const
  {
    return _points[*_faceList];
  }
  inline operator const OdGePoint3d*() const
  {
    return _points + (*_faceList);
  }
  inline const OdGePoint3d* operator ->() const
  {
    return _points + *_faceList;
  }
  inline OdGiShellFaceIterator& operator ++()
  {
    ++_faceList;
    return *this;
  }
  inline OdGiShellFaceIterator operator ++(int)
  {
    OdGiShellFaceIterator res(*this);
    ++_faceList;
    return res;
  }
  inline OdGiShellFaceIterator& operator --()
  {
    --_faceList;
    return *this;
  }
  inline OdGiShellFaceIterator operator --(int)
  {
    OdGiShellFaceIterator res(*this);
    --_faceList;
    return res;
  }
  inline OdGiShellFaceIterator& operator += (const int n)
  {
    _faceList+=n;
    return *this;
  }
  inline OdGiShellFaceIterator& operator -= (const int n)
  {
    _faceList-=n;
    return *this;
  }
  inline OdGiShellFaceIterator operator + (const int n) const
  {
    OdGiShellFaceIterator res(*this);
    res+=n;
    return res;
  }
  inline OdGiShellFaceIterator operator - (const int n) const
  {
    OdGiShellFaceIterator res(*this);
    res-=n;
    return res;
  }
  inline bool operator < (const OdGiShellFaceIterator& op) const
  {
    return _faceList < op._faceList;
  }
  inline bool operator < (const OdInt32* op) const
  {
    return _faceList < op;
  }
};

#include "TD_PackPop.h"

#endif // __ODGISHELLFACEITERATOR__
