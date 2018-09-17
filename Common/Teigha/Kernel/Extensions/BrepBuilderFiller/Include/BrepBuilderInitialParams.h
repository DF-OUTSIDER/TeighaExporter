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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////


#ifndef _BREPBUILDERFILLERPARAMS_INCLUDED_
#define _BREPBUILDERFILLERPARAMS_INCLUDED_

/** \details
	
    Corresponding C++ library: TD_BrepBuilderFiller
    <group Extension_Classes>
*/
class OdBrepBuilderFillerParams
{
  bool m_skipNullSurface;
  bool m_skipCodge2dCurve;

public:
  OdBrepBuilderFillerParams()
    : m_skipNullSurface(false)
    , m_skipCodge2dCurve(false)
  {}

  /** \details
  If true face without surface will not generate error.
  All topology of such face will be skiped.
  */
  void setSkipNullSurface(bool skipNullSurface)
  {
    m_skipNullSurface = skipNullSurface;
  }
  bool isSkipNullSurface() const
  {
    return m_skipNullSurface;
  }

  /** \details
  If true coedge parametric curves will be ignored.
  */
  void setSkipCoedge2dCurve(bool skipCoedge2dCurve)
  {
    m_skipCodge2dCurve = skipCoedge2dCurve;
  }
  bool isSkipCoedge2dCurve() const
  {
    return m_skipCodge2dCurve;
  }
};

#endif //_BREPBUILDERFILLERPARAMS_INCLUDED_
