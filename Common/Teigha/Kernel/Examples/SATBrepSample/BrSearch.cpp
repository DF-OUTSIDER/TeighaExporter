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

// searches.cpp -- using the OdBr interface to search for vertex and edge
//                 adjacencies

#include <iostream>

#include "BrSearch.h"
// for edge search and adjacencies
//#include "OdaCommon.h"
#include "Br/BrBrepEdgeTraverser.h"
#include "Br/BrEdgeLoopTraverser.h"
#include "Br/BrLoopEdgeTraverser.h"
#include "Br/BrLoop.h"

// for face search and adjacencies

#include "Br/BrBrepFaceTraverser.h"
#include "Ge/GeCone.h"
#include "Ge/GeCylinder.h"
#include "Ge/GeNurbSurface.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GePlane.h"
#include "Ge/GeSphere.h"
#include "Ge/GeTorus.h"

#include "Ge/GeEllipCone.h"  // AE 03.09.2003 
#include "Ge/GeEllipCylinder.h"  
#include "Ge/GeExternalBoundedSurface.h"  

// for vertex search and adjacencies
#include "Br/BrBrepVertexTraverser.h"
#include "Br/BrVertexEdgeTraverser.h"
#include "Br/BrVertexLoopTraverser.h"

// for geometry queries
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeLineSeg3d.h"


void dumpPoint(const OdGePoint3d& p);
void dumpVector(const OdGeVector3d& w);
void reportEdgeAdjacencies(OdBrEdge edg);
void reportFaceGeometry(OdBrFace f);

/************************************************************************/
/* Dump the specified cone                                              */
/************************************************************************/
void dumpCone(OdGeCone* theCone)
{
  std::cout << "Cone apex: ";
  dumpPoint(theCone->apex());

  std::cout << "\n\tCone axis: ";
  dumpVector(theCone->axisOfSymmetry());

  std::cout << "\n\tCone base center: ";
  dumpPoint(theCone->baseCenter());

  std::cout << "\n\tCone base radius: " << theCone->baseRadius();
  std::cout << "\n\tCone half angle: " << theCone->halfAngle() << "\n\n";
}

/************************************************************************/
/* Dump the specified elliptical cone                                   */
/************************************************************************/
void dumpEllipCone(OdGeEllipCone* theEllipCone)
{
  std::cout << "Elliptical cone apex: ";
  dumpPoint(theEllipCone->apex());

  std::cout << "\n\tElliptical cone axis: ";
  dumpVector(theEllipCone->axisOfSymmetry());

  std::cout << "\n\tElliptical cone base center: ";
  dumpPoint(theEllipCone->baseCenter());

  std::cout << "\n\tElliptical cone minor radius: " << theEllipCone->minorRadius();
  std::cout << "\n\tElliptical cone major radius: " << theEllipCone->majorRadius();
  std::cout << "\n\tElliptical cone half angle: " << theEllipCone->halfAngle() << "\n\n";
}

/************************************************************************/
/* Dump the specified cylinder                                          */
/************************************************************************/
void dumpCylinder(OdGeCylinder* theCyl)
{
  OdGePoint3d B = theCyl->origin();
  OdGeVector3d w = theCyl->axisOfSymmetry();
  double r = theCyl->radius();
  std::cout << "Cylinder base: ";
  dumpPoint(B);
  std::cout << "\n\tCylinder axis: ";
  dumpVector(w);
  std::cout << "\n\tCylinder radius: " << r << "\n\n";
}
/************************************************************************/
/* Dump the specified elliptical cylinder                               */
/************************************************************************/
void dumpEllipCylinder(OdGeEllipCylinder* theCyl)
{
  OdGePoint3d B = theCyl->origin();
  OdGeVector3d w = theCyl->axisOfSymmetry();
  double r = theCyl->minorRadius();
  double R = theCyl->majorRadius();
  std::cout << "Elliptical cylinder base: ";
  dumpPoint(B);
  std::cout << "\n\tElliptical cylinder axis: ";
  dumpVector(w);
  std::cout << "\n\tElliptical cylinder major radius: " << R;
  std::cout << "\n\tElliptical cylinder minor radius: " << r << "\n\n";
}

/************************************************************************/
/* Dump the specified ellipse                                           */
/************************************************************************/
void dumpEllipse(OdGeEllipArc3d* ell)
{
  if (ell == NULL)
  {
    std::cerr << "NULL ellipse!\n";
    return;
  }
  std::cout << "(C,u,v,R,r) = (";
  OdGePoint3d C = ell->center();
  std::cout << "(" << C.x << ", " << C.y << ", " << C.z << "), ";
  OdGeVector3d u = ell->majorAxis();
  OdGeVector3d v = ell->minorAxis();
  std::cout << "(" << u.x << ", " << u.y << ", " << u.z << "), ";
  std::cout << "(" << v.x << ", " << v.y << ", " << v.z << "), ";
  std::cout << ell->majorRadius() << ", " << ell->minorRadius() << '\n';
}

/************************************************************************/
/* Dump the specified line                                              */
/************************************************************************/
void dumpLine(OdGeLineSeg3d* lin)
{
  if (lin == NULL)
  {
    std::cerr << "NULL line!\n";
    return;
  }
  OdGePoint3d base = lin->pointOnLine();
  OdGeVector3d dir = lin->direction();
  std::cout << "Line base point: ";
  dumpPoint(base);
  std::cout << "\n\tLine direction: ";
  dumpVector(dir);
  std::cout << "\n\n";
}

/************************************************************************/
/* Dump the specified nurb curve                                        */
/************************************************************************/
void dumpNurbCurve(OdGeNurbCurve3d* nc)
{
  if (nc == NULL)
  {
    std::cerr << "NULL nurbs curve!\n";
    return;
  }
  int          degree;
  bool        rational, periodic;
  OdGeKnotVector    knots;
  OdGeDoubleArray    wts;
  OdGePoint3dArray  cPts;
  nc->getDefinitionData(degree,rational,periodic,knots,cPts,wts);
  std::cout << "Degree = " << degree << ", rational = " << rational
       << ", periodic = " << periodic << '\n'
       << "\t#knots = " << knots.length() << "\n\tknots: (";
  for (int i=0 ; i<knots.length() ; i++)
  {
    std::cout << knots[i];
    if (i != (knots.length()-1))
      std::cout << ", ";
  }
  std::cout << ")\n\t#cpts = " << cPts.size() << "\n\tControl points & weights: (";
  for (unsigned int j=0 ; j<cPts.size() ; j++)
  {
    dumpPoint(cPts[j]);
    std::cout << ", " << (wts.isEmpty() ? 1. : wts[j]) << ')';
    if (j != cPts.size()-1)
      std::cout << ", ";
  }
  std::cout << ")\n\n";
}

/************************************************************************/
/* Dump the specified nurb surface                                      */
/************************************************************************/
void dumpNurbSurface(OdGeNurbSurface* theNurb)
{
  int          degreeU, degreeV, propsU, propsV, nCptsU, nCptsV;
  OdGePoint3dArray  cPts;
  OdGeDoubleArray    weights;
  OdGeKnotVector    uKnots, vKnots;

  theNurb->getDefinition(degreeU, degreeV, propsU, propsV,
    nCptsU, nCptsV, cPts, weights, uKnots, vKnots);

  std::cout << "degreeU = " << degreeU << ", degreeV = " << degreeV
       << ", propsU = " << propsU << ", propsV = " << propsV
       << ", nCptsU = " << nCptsU << ", nCptsV = " << nCptsV << "\n\t";

  std::cout << "uKnots: (";
  int i = 0;
  for ( ; i<uKnots.length() ; i++)
  {
    std::cout << uKnots[i];
    if (i != uKnots.length()-1)
      std::cout << ", ";
  }
  std::cout << ")\n\t";

  std::cout << "vKnots: (";
  for (i=0 ; i<vKnots.length() ; i++)
  {
    std::cout << vKnots[i];
    if (i != vKnots.length()-1)
      std::cout << ", ";
  }
  std::cout << ")\n";

  std::cout << "Control points & weights:\n\t(";
  for (i=0 ; i < (int)cPts.size() ; i++)
  {
    std::cout << '(';
    dumpPoint(cPts[i]);
    std::cout << ", " << (weights.isEmpty() ? 1. : weights[i]) << ')';
    if (i != (int)cPts.size()-1)
      std::cout << ", ";
  }
  std::cout << ")\n\n";
}

/************************************************************************/
/* Dump the specified plane                                             */
/************************************************************************/
void dumpPlane(OdGePlane* thePlane)
{
  OdGePoint3d B = thePlane->pointOnPlane();
  OdGeVector3d n = thePlane->normal();
  std::cout << "Plane base: ";
  dumpPoint(B);
  std::cout << "\n\tPlane normal: ";
  dumpVector(n);
  std::cout << "\n\n";
}

/************************************************************************/
/* Dump the specified point                                              */
/************************************************************************/
void dumpPoint(const OdGePoint3d& p)
{
  std::cout << '(' << p.x << " , " << p.y << " , " << p.z << ')';
}

/************************************************************************/
/* Dump the specified sphere                                            */
/************************************************************************/
void dumpSphere(OdGeSphere* theSphere)
{
  OdGePoint3d C = theSphere->center();
  double r = theSphere->radius();
  std::cout << "Sphere center: ";
  dumpPoint(C);
  std::cout << "\n\tSphere radius: " << r << "\n\n";
}

/************************************************************************/
/* Dump the specified torus                                             */
/************************************************************************/
void dumpTorus(OdGeTorus* theTorus)
{
  OdGePoint3d C = theTorus->center();
  OdGeVector3d w = theTorus->axisOfSymmetry();
  double R = theTorus->majorRadius();
  double r = theTorus->minorRadius();
  std::cout << "Torus center: ";
  dumpPoint(C);
  std::cout << "\n\tTorus axis: ";
  dumpVector(w);
  std::cout << "\n\tTorus major radius: " << R
       << ", Torus minor radius: " << r << "\n\n";
}

/************************************************************************/
/* Dump the specified vector                                            */
/************************************************************************/
void dumpVector(const OdGeVector3d& w)
{
  std::cout << '(' << w.x << " , " << w.y << " , " << w.z << ')';
}

/************************************************************************/
/* Return the edge with the specified number                            */
/************************************************************************/
void edgeSearch(OdBrBrep& aBrep, int edgeNum)
{
  std::cout << "Searching for edge " << edgeNum << std::endl;

  int num = 0;
  OdBrBrepEdgeTraverser  bet;
  bet.setBrep(aBrep);
  while (!bet.done())
  {
    OdBrEdge cur = bet.getEdge();
    if (num == edgeNum)
    {
      reportEdgeAdjacencies(cur);
      return;
    }
    bet.next(); ++num;
  }

  std::cout << "Edge not found.\n";
}

/************************************************************************/
/* Return the face with the specified number                            */
/************************************************************************/
void faceSearch(OdBrBrep& aBrep, int faceNum)
{
  std::cout << "Searching for face " << faceNum << std::endl;

  int num = 0;
  OdBrBrepFaceTraverser  bft;
  bft.setBrep(aBrep);
  while (!bft.done())
  {
    OdBrFace cur = bft.getFace();
    if (num == faceNum)
    {
      reportFaceGeometry(cur);
      return;
    }
    bft.next(); ++num;
  }

  std::cout << "Face not found.\n";
}
/************************************************************************/
/*  Output the number of adjacent edges, and curve type of the          */
/*  specified                                                           */
/************************************************************************/
void reportEdgeAdjacencies(OdBrEdge edg)
{
  OdBrEdgeLoopTraverser  elt;
  elt.setEdge(edg);
  std::cout << "Number of adjacent loops :\n";
  OdUInt32 num = 0;
  while (!elt.done())
  {
    OdBrLoop curLoop = elt.getLoop();
    elt.next(); ++num;
  }

  std::cout << "\t " << num << '\n';

  OdGeCurve3d* theCurve = edg.getCurve();
  if (!theCurve)
    std::cerr << "Could not get curve!\n";
  
  OdGe::EntityId type = edg.getCurveType();
  switch (type)
  {
    case OdGe::kEllipArc3d:
      std::cout << "Edge lies on an ellipse:\n\t";
      dumpEllipse(static_cast<OdGeEllipArc3d*>(theCurve));
      break;
    case OdGe::kNurbCurve3d:
      std::cout << "Edge lies on a nurb curve:\n\t";
      dumpNurbCurve(static_cast<OdGeNurbCurve3d*>(theCurve));
      break;
    case OdGe::kLineSeg3d:
      std::cout << "Edge lies on a straight line:\n\t";
      dumpLine(static_cast<OdGeLineSeg3d*>(theCurve));
      break;
    default:
      std::cout << "Unknown curve type: " << type << '\n';
  }

  std::cout << "Returned as a NURBS curve:\n\t";
  OdGeNurbCurve3d  nc;
  if (edg.getCurveAsNurb(nc))
    dumpNurbCurve(&nc);
  else
    std::cout << "Could not retrieve the curve as a Nurb curve!\n";

  delete theCurve;
}

/************************************************************************/
/* Output the face geometry of the specified face                       */
/************************************************************************/
void reportFaceGeometry(OdBrFace f)
{
  if (f.isNull())
  {
    std::cerr << "No face!\n";
    return;
  }
  OdGeExternalBoundedSurface* pExtSurface = (OdGeExternalBoundedSurface*)f.getSurface();
  if (!pExtSurface)
    std::cerr << "Could not get surface!\n";

  OdGeSurface *theSurface = 0;
  pExtSurface->getBaseSurface(theSurface);
  OdGe::EntityId surfType = theSurface->type();

  OdGe::EntityId type;
  OdBrErrorStatus err = f.getSurfaceType(type);
  if (err != odbrOK && type == surfType)
    std::cerr << "Could not get surface type!\n";
  switch (type)
  {
    case OdGe::kCone:
      std::cout << "Face lies on a cone:\n\t";
      dumpCone(static_cast<OdGeCone*>(theSurface));
      break;
    case OdGe::kEllipCone:
      std::cout << "Face lies on a elliptical cone:\n\t";
      dumpEllipCone(static_cast<OdGeEllipCone*>(theSurface));
      break;
    case OdGe::kCylinder:
      std::cout << "Face lies on a cylinder:\n\t";
      dumpCylinder(static_cast<OdGeCylinder*>(theSurface));
      break;
    case OdGe::kEllipCylinder:
      std::cout << "Face lies on a elliptical cylinder:\n\t";
      dumpEllipCylinder(static_cast<OdGeEllipCylinder*>(theSurface));
    case OdGe::kNurbSurface:
      std::cout << "Face lies on a NURBS surface:\n\t";
      dumpNurbSurface(static_cast<OdGeNurbSurface*>(theSurface));
      break;
    case OdGe::kPlane:
      std::cout << "Face lies on a plane:\n\t";
      dumpPlane(static_cast<OdGePlane*>(theSurface));
      break;
    case OdGe::kSphere:
      std::cout << "Face lies on a sphere:\n\t";
      dumpSphere(static_cast<OdGeSphere*>(theSurface));
      break;
    case OdGe::kTorus:
      std::cout << "Face lies on a torus:\n\t";
      dumpTorus(static_cast<OdGeTorus*>(theSurface));
      break;
    default:
      std::cout << "Unknown surface type: " << type << '\n';
  }

  delete theSurface;
  delete pExtSurface;

  std::cout << "Returned as a NURBS surface:\n\t";
  OdGeNurbSurface  ns;
  err = f.getSurfaceAsNurb(ns);
  if (err == odbrOK)
    dumpNurbSurface(&ns);
  else
    std::cout << "Could not retrieve the surface as a Nurb surface!\n";
}

/************************************************************************/
/* Output the number of adjacent loops and edges for the specified      */
/* Vertex                                                               */
/************************************************************************/
void reportVertexAdjacencies(OdBrVertex vtx)
{
  OdBrVertexLoopTraverser  vlt;
  vlt.setVertex(vtx);
  std::cout << "Number of adjacent loops:\n";
  OdUInt32 num = 0;
  while (!vlt.done())
  {
    OdBrLoop curLoop = vlt.getLoop();
    vlt.next(); ++num;
  }
  std::cout << "\t" << num << '\n';

  OdBrVertexEdgeTraverser  vet;
  vet.setVertex(vtx);
  std::cout << "Number of adjacent edges:\n";
  num = 0;
  while (!vet.done())
  {
    OdBrEdge  curEdge = vet.getEdge();
    vet.next(); ++num;
  }
   std::cout << "\t" << num << '\n';
}

/************************************************************************/
/* Return the vertex with the specified number                          */
/************************************************************************/
void vertexSearch(OdBrBrep& aBrep, int vertexNum)
{
  std::cout << "Searching for vertex " << vertexNum << std::endl;

  OdBrBrepVertexTraverser  bvt;
  bvt.setBrep(aBrep);
  int num = 0;
  while (!bvt.done())
  {
    OdBrVertex  cur = bvt.getVertex();
    if (num == vertexNum)
    {
      reportVertexAdjacencies(cur);
      return;
    }
    bvt.next(); ++num;
  }

  std::cout << "Vertex not found.\n";
}
