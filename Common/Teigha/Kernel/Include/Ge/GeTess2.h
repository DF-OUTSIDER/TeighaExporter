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

#ifndef __ODGITESSELATOR_2_H_INCLUDED__
#define __ODGITESSELATOR_2_H_INCLUDED__

#include "TD_PackPush.h"

#include "OdList.h"
#include "OdArray.h"

#define ODGT_NAMESPACE OdGeTess2
#define ODGT_PTRTYPE Contour*
#define ODGT_PARENT

#define ODGT_MEMBERS2 \
public:\
static void *create(OdUInt32 num)\
{\
  return new Vertex[num];\
}

#define ODGT_MEMBERS \
friend class OptimizedHolder;\
 MemoryHolder *m_pHolder;\
public:\
static void *create(OdUInt32 num)\
{\
  return new Contour[num];\
}

#define ODGT_HOLDER , MemoryHolder &pHolder
#define ODGT_HOLDER2 , pHolder

namespace ODGT_NAMESPACE
{
  class Vertex;
  class Contour;

  class GE_TOOLKIT_EXPORT MemoryHolder
  {
  public:
    virtual Vertex *newVertex() = 0;
    virtual void deleteVertex(Vertex *) = 0;

    virtual Contour *newContour() = 0;
    virtual void deleteContour(Contour *) = 0;

    virtual ~MemoryHolder() {}
  };
};


#include "GeTess.h"

#define HOLDER_(A, B) struct A\
{\
  bool         m_First;\
  B*           m_FirstPage;\
  OdList< B* > m_Buf;\
  OdUInt32     m_pageSize;\
  OdUInt32     m_pos;\
\
  A(OdUInt32 pageSize)\
    : m_First(true)\
    , m_FirstPage(0)\
    , m_pageSize(pageSize)\
    , m_pos(0)\
  {\
    m_FirstPage = (B*)B::create(m_pageSize);\
    /*m_Buf.resize(1, (B*)B::create(m_pageSize) );*/\
  }\
\
  void freeMem()\
  {\
    delete []m_FirstPage; m_FirstPage = 0;\
    OdList< B* >::iterator pIt = m_Buf.begin();\
    OdList< B* >::iterator pItEnd = m_Buf.end();\
\
    while(pIt != pItEnd)\
    {\
      delete[] *pIt;\
      ++pIt;\
    }\
  }\
\
  ~A()\
  {\
    freeMem();\
  }\
\
  B* newOne()\
  {\
    if (m_pos == m_pageSize)\
    {\
      m_First = false;\
      m_Buf.push_front( (B*)B::create(m_pageSize) );\
      m_pos = 0;\
    }\
    if (m_First)\
      return &m_FirstPage[m_pos++];\
    return &(*m_Buf.begin())[m_pos++];\
  }\
};

/** \details

    <group TD_Namespaces>
*/
namespace OdGeTess2
{
  class GE_TOOLKIT_EXPORT OptimizedHolder : public MemoryHolder
  {
    HOLDER_(Holder_Contour, Contour)
    HOLDER_(Holder_Vertex, Vertex)

    Holder_Vertex  m_Vxs;
    Holder_Contour m_Ctrs;

  public:
    OptimizedHolder(OdUInt32 nPreAllocVertexes, OdUInt32 nPreAllocContours);

    virtual Vertex *newVertex();
    virtual void deleteVertex(Vertex * pVx);

    virtual Contour *newContour();
    virtual void deleteContour(Contour *);

    ~OptimizedHolder();
  };
};

#undef ODGT_NAMESPACE
#undef ODGT_PTRTYPE
#undef ODGT_PARENT
#undef ODGT_MEMBERS
#undef ODGT_HOLDER
#undef ODGT_HOLDER2

#include "TD_PackPop.h"

#endif // __ODGITESSELATOR_2_H_INCLUDED__
