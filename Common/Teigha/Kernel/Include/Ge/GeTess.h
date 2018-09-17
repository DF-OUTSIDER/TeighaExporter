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

#ifndef __ODGITESSELATOR_H_INCLUDED__
#define __ODGITESSELATOR_H_INCLUDED__


#include "RxObject.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector2d.h"

#include "TD_PackPush.h"

#ifndef ODGT_NAMESPACE
  #define ODGT_NAMESPACE OdGeTess
  #define ODGT_PTRTYPE OdSmartPtr<Contour>
  #define ODGT_PARENT  : public OdRxObject
  #define ODGT_MEMBERS
  #define ODGT_HOLDER
  #define ODGT_HOLDER2
  #define ODGT_MEMBERS2
#endif

/** \details

    <group TD_Namespaces>
*/
namespace ODGT_NAMESPACE
{

enum EdgesType
{
  kEtUndef=0,
  kEtLeft,
  kEtRight,
  kEtParallel
};


enum BreakOptions
{
  kBrCopyResult   = 1,
  kBrDetectHoles  = 2,
  kBrNormalized   = 4
};

class Vertex;

struct Intersection
{
  Vertex* pEdgeStart;
  double u1;
  bool operator < (const Intersection& op) const { return u1 < op.u1; }
  bool operator < (double op) const { return u1 < op; }
};

typedef OdArray<Intersection, OdMemoryAllocator<Intersection> > Intersections;

class Contour;
class LocalVert;
typedef ODGT_PTRTYPE ContourPtr;

class GE_TOOLKIT_EXPORT Contour ODGT_PARENT
{
  friend class Vertex;
  ODGT_MEMBERS
protected:
  Vertex     *m_pFirstVertex;
  ContourPtr  m_pNextContour;

  const void *m_pCoords;

  unsigned    m_b2d:1;
  unsigned    m_bClockwise:1;
  unsigned    m_bFailed:1;
  double      m_tol;

  void addVertex(Vertex* vertex);
  void removeVertex(Vertex* vertex);
  void delVertex(Vertex* vertex);
  void breakContour(Vertex* vertex);
  bool normalize(double size, double tol);
  void checkClockwiseOriented(int bNested, double tol);
  static void split(Vertex* v1, Vertex* v2);
  bool isInside(const OdGePoint2d& point) const;
  static bool isIncommingR(const Vertex* v1, const Vertex* v2);
  void concatContours(Vertex* pC1vert, Contour* pContour2, Vertex* pC2vert);
  void removeEqualVertexes(const OdGeTol& tol);
  ContourPtr lastContour() const;
  void splitPolygon(Vertex* pV1, Vertex* pV2);

  void breakToConvex2(double size);
  double calcMaxCoordValue(double& size) const;
  bool splitOnCoincidentPoint(double size);
protected:
  Contour();
public:
  ~Contour();

  static ContourPtr create(const void* coords, bool b2d ODGT_HOLDER);
  static ContourPtr create(const OdGePoint2d* coords ODGT_HOLDER) { return create(coords, true ODGT_HOLDER2); }
  static ContourPtr create(const OdGePoint3d* coords ODGT_HOLDER) { return create(coords, false ODGT_HOLDER2); }
  ContourPtr insert(Contour* pCntr2Ins)
  {
    ODA_ASSERT(pCntr2Ins!=this);
    pCntr2Ins->m_pNextContour = m_pNextContour;
    return (m_pNextContour = pCntr2Ins);
  }
  Vertex* addVertex(int index, unsigned int edgeId = 0);
  ContourPtr breakToConvex(unsigned int options, double eps = -1.);
  void inverse();

  const Vertex* head() const { return m_pFirstVertex; }
        Vertex* head()       { return m_pFirstVertex; }
  bool vectorIntersects(const Vertex* v1, const Vertex* v2, bool bNested) const;
  static bool isIncomming(const Vertex* v1, const Vertex* v2);
  double squareValue() const;
  ContourPtr copy(int bNested) const;
  bool isIncommingOrOnEdgeR(const Vertex* pV1, const Vertex* pV2) const;
  const OdGePoint2d& point(int index) const
  {
    if(m_b2d)
      return reinterpret_cast<const OdGePoint2d*>(m_pCoords)[index];
    return *reinterpret_cast<const OdGePoint2d*>(reinterpret_cast<const OdGePoint3d*>(m_pCoords) + index);
  }
  ContourPtr next() const { return m_pNextContour; }

  const void* coords() const { return m_pCoords; }
  bool is2d() const { return m_b2d; }
  bool isTriangle() const;
  bool isLineSeg() const;
  bool intersections(const Vertex* vert1, const Vertex* vert2, Intersections& res, bool bNested) const;
  bool intersections_lv(const Vertex* vert1, const LocalVert* vert2, Intersections& res, bool bNested) const;
  bool failed() const { return m_bFailed; }
  double tol() const { return m_tol; }
};


class GE_TOOLKIT_EXPORT Vertex
{
  friend class Contour;
  Contour        *m_pContour;

  ODGT_MEMBERS2

protected:
  Vertex         *m_pNextVertex;
  Vertex         *m_pPrevVertex;
  int             m_nIndex;
  unsigned int    m_nEdgeId;

public:
  Vertex()
    : m_pContour(0)
    , m_pNextVertex(0)
    , m_pPrevVertex(0)
    , m_nIndex(0)
    , m_nEdgeId(0)
  {
  }
  void link(Vertex* vert2)
  {
    m_pNextVertex = vert2;
    vert2->m_pPrevVertex = this;
  }
  const Vertex* next() const { return m_pNextVertex; }
        Vertex* next()       { return m_pNextVertex; }
  const Vertex* prev() const { return m_pPrevVertex; }
        Vertex* prev()       { return m_pPrevVertex; }
           int index() const { return m_nIndex; }
  void setIndex(int nIndex) { m_nIndex = nIndex; }
  unsigned int edgeId() const { return m_nEdgeId; }
  void setEdgeId(unsigned int nEdgeId) { m_nEdgeId = nEdgeId; }

  const OdGePoint2d& point() const { return m_pContour->point(m_nIndex); }

  EdgesType edgesType() const;

  ContourPtr contour() const { return m_pContour; }
  void setContour(Contour* pContour) { m_pContour = pContour; }

};

inline bool Contour::isTriangle() const
{
  return (m_pFirstVertex && m_pFirstVertex->next()->next()==m_pFirstVertex->prev());
}

inline bool Contour::isLineSeg() const
{
  return (m_pFirstVertex && m_pFirstVertex->next()==m_pFirstVertex->prev());
}


} //namespace

#include "TD_PackPop.h"

#undef ODGT_NAMESPACE
#undef ODGT_PTRTYPE
#undef ODGT_PARENT
#undef ODGT_MEMBERS
#undef ODGT_HOLDER
#undef ODGT_HOLDER2
#undef ODGT_MEMBERS2

#endif // __ODGITESSELATOR_H_INCLUDED__
