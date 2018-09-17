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

// topDownWalk.c++ -- Using the OdBr API to walk the hierarchical BRep model

#include <iostream>
#include <iomanip>

// traversers used to go "straight top-down" through the topological forms
//#include "OdaCommon.h"
#include "Br/BrBrepComplexTraverser.h"
#include "Br/BrComplexShellTraverser.h"
#include "Br/BrShellFaceTraverser.h"
#include "Br/BrFaceLoopTraverser.h"
#include "Br/BrLoopVertexTraverser.h"
#include "Br/BrEdgeLoopTraverser.h"
#include "Br/BrLoopEdgeTraverser.h"

// Other traversers
#include "Br/BrBrepShellTraverser.h"
#include "Br/BrBrepEdgeTraverser.h"
#include "Br/BrBrepFaceTraverser.h"
#include "Br/BrBrepVertexTraverser.h"

// Miscellaneous includes:

#include "BrTraverse.h"


void processBrep(const OdBrBrep& aBrep);
void processComplex(const OdBrComplex& comp);
void processLoopToEdge(const OdBrLoop& l);
void processLoopToVertex(const OdBrLoop& l);
void processShell(const OdBrShell& aShell);
void processVertex(const OdBrVertex& vtx);

bool  doLoopToCoEdge = false;

const int idWidth = 4;
const int floatPrecision = 2;
const int floatWidth = 5;

/************************************************************************/
/* Return the specified entity type as a string                         */
/************************************************************************/
const char * entityIdString(OdGe::EntityId type)
{
  switch (type)
  {
    case OdGe::kEllipArc3d:
      return "EllipArc3d";
    case OdGe::kNurbCurve3d:
      return "NurbCurve3d";
    case OdGe::kLineSeg3d:
      return "LineSeg3d";
    case OdGe::kCone:
      return "GeCone";
    case OdGe::kEllipCone:
      return "EllipCone";
    case OdGe::kCylinder:
      return "Cylinder";
    case OdGe::kEllipCylinder:
      return "EllipCylinder";
    case OdGe::kNurbSurface:
      return "NurbSurface";
    case OdGe::kPlane:
      return "Plane";
    case OdGe::kSphere:
      return "Sphere";
    case OdGe::kTorus:
      return "Torus";
    default:
      break;
  };

  return "Unknown";
}
/************************************************************************/
/* Returns the edge specified by the edge number                        */
/* Returns true if and only if the specified edge exists                */
/************************************************************************/
bool findEdge(const OdBrBrep& aBrep, int edgeNum, OdBrEdge& edge)
{
  OdBrBrepEdgeTraverser bet;
  bet.setBrep(aBrep);
  int num = 0;
  while (!bet.done())
  {
    edge = bet.getEdge();
    if (num == edgeNum)
      return true;
    bet.next(); ++num;
  }
  edge = OdBrEdge();
  return false;
}
/************************************************************************/
/* Output the complete Brep                                             */
/************************************************************************/
void processBrep(const OdBrBrep& aBrep)
{
  OdBrBrepComplexTraverser  bct;
  bct.setBrep(aBrep);
  int complexNum = 0;
  std::cout << "Start at which Complex (0 to start with first): ";
  int  firstID = -1;
  std::cin >> firstID;
  /**********************************************************************/
  /* Find the starting complex                                          */
  /**********************************************************************/
  if (firstID >= 0)
  {
    bool foundIT = false;
    while (!bct.done())
    {
      OdBrComplex comp = bct.getComplex();
      if (complexNum == firstID)
      {
        bct.setComplex(comp);
        foundIT = true;
        break;
      }
      bct.next(); ++complexNum;
    }
    if (!foundIT)
    {
      std::cout << "complex " << firstID << " is not found.\n";
      bct.restart();
    }
  }


  /**********************************************************************/
  /* Output the complexes                                               */
  /**********************************************************************/
  complexNum = 0;
  while (!bct.done())
  {
    OdBrComplex cur = bct.getComplex();
    std::cout << "\tComplex " << std::setw(idWidth) << complexNum << '\n';

    processComplex(cur);

    complexNum++;
    bct.next();
  }
}
/************************************************************************/
/* Output just the edges in the specified Brep                          */
/************************************************************************/
void processBrepToEdgeOnly(const OdBrBrep& aBrep)
{
  OdBrBrepEdgeTraverser  bet;
  bet.setBrep(aBrep);
  int edgeNum = 0;
  std::cout << "Start at which Edge (0 to start with first): ";
  int  firstID;
  std::cin >> firstID;
  OdBrEdge edg;

  /**********************************************************************/
  /* Find the starting edge                                             */
  /**********************************************************************/
  if (firstID >= 0)
  {
    if (findEdge(aBrep, firstID, edg))
      bet.setEdge(edg);
    else
      std::cout << firstID << " is not found.\n";
  }

  std::cout.precision(floatPrecision);
  /**********************************************************************/
  /* Output the edges                                                   */
  /**********************************************************************/
  edgeNum = 0;
  while (!bet.done())
  {
    OdBrEdge cur = bet.getEdge();
    std::cout << "\tEdge " << std::setw(idWidth) << edgeNum;

    OdBrVertex v1, v2;
    cur.getVertex1(v1);
    cur.getVertex2(v2);

    OdGe::EntityId  eID =  cur.getCurveType();
    std::cout << " on curve type " << entityIdString(eID) << '\n';

    edgeNum++;
    bet.next();
  }
}
/************************************************************************/
/* Output just the faces in the specified Brep                          */
/************************************************************************/
void processBrepToFaceOnly(const OdBrBrep& aBrep)
{
  OdBrBrepFaceTraverser  bft;
  bft.setBrep(aBrep);
  int faceNum = 0;

  std::cout << "Start at which Face (satfile ID; 0 to start with first): ";
  int  firstID;
  std::cin >> firstID;
  /**********************************************************************/
  /* Find the starting face                                             */
  /**********************************************************************/
  if (firstID >= 0)
  {
    bool foundIT = false;
    while (!bft.done())
    {
      OdBrFace face = bft.getFace();
      if (faceNum == firstID)
      {
        bft.setFace(face);
        foundIT = true;
        break;
      }
      bft.next(); ++faceNum;
    }
    if (!foundIT)
    {
      std::cout << firstID << " is not found.\n";
      bft.restart();
    }
  }
  std::cout.precision(floatPrecision);
  /**********************************************************************/
  /* Output the faces                                                   */
  /**********************************************************************/
  faceNum = 0;
  while (!bft.done())
  {
    OdBrFace cur = bft.getFace();
    std::cout << "\tFace " << std::setw(idWidth) << faceNum;

    OdGe::EntityId  eID;
    cur.getSurfaceType(eID);
    std::cout << " on surface type " << entityIdString(eID) << '\n';

    faceNum++;
    bft.next();
  }
}
/************************************************************************/
/* Output just the shells in the specified Brep                         */
/************************************************************************/
void processBrepToShellOnly(const OdBrBrep& aBrep)
{
  OdBrBrepShellTraverser  bst;
  bst.setBrep(aBrep);
  int shellNum = 0;
  std::cout << "Start at which Shell (satfile ID; 0 to start with first): ";
  int  firstID;
  std::cin >> firstID;
  /**********************************************************************/
  /* Find the starting shell                                            */
  /**********************************************************************/
  if (firstID >= 0)
  {
    bool foundIT = false;
    while (!bst.done())
    {
      OdBrShell shell = bst.getShell();
      if (shellNum == firstID)
      {
        bst.setShell(shell);
        foundIT = true;
        break;
      }
      bst.next(); ++shellNum;
    }
    if (!foundIT)
    {
      std::cout << firstID << " is not found.\n";
      bst.restart();
    }
  }

  /**********************************************************************/
  /* Output the shells                                                  */
  /**********************************************************************/
  shellNum = 0;
  while (!bst.done())
  {
    OdBrShell cur = bst.getShell();
    std::cout << "\tShell " << std::setw(idWidth) << shellNum << '\n';

    bst.next(); ++shellNum;
  }
}
/************************************************************************/
/* Output just vertices in the specified Brep                           */
/************************************************************************/
void processBrepToVertexOnly(const OdBrBrep& aBrep)
{
  OdBrBrepVertexTraverser  bvt;
  bvt.setBrep(aBrep);
  int vertexNum = 0;
  std::cout << "Start at which Vertex (0 to start with first): ";
  int  firstID;
  std::cin >> firstID;
  /**********************************************************************/
  /* Find the starting vertex                                           */
  /**********************************************************************/
  if (firstID >= 0)
  {
    bool foundIT = false;
    while (!bvt.done())
    {
      OdBrVertex vtx = bvt.getVertex();
      if (vertexNum == firstID)
      {
        bvt.setVertex(vtx);
        foundIT = true;
        break;
      }
      bvt.next(); ++vertexNum;
    }
    if (!foundIT)
    {
      std::cout << firstID << " is not found.\n";
      bvt.restart();
    }
  }
  /**********************************************************************/
  /* Output the vertices                                                */
  /**********************************************************************/
  vertexNum = 0;
  while (!bvt.done())
  {
    OdBrVertex cur = bvt.getVertex();
    std::cout << "\tVertex " << std::setw(idWidth) << vertexNum;
    OdGePoint3d pnt = cur.getPoint();
    std::cout << " at (" << pnt.x << ", " << pnt.y << ", " << pnt.z << ')' << '\n';

    vertexNum++;
    bvt.next();
  }
}
/************************************************************************/
/* Output the specified loop edge                                       */
/************************************************************************/
void processLoopEdge(const OdBrLoopEdgeTraverser& elt)
{
}

/************************************************************************/
/* Output the specified edge loop                                       */
/************************************************************************/
void processEdgeLoop(const OdBrLoopEdgeTraverser& elt)
{
}

/************************************************************************/
/* Output the shells in the specified complex                           */
/************************************************************************/
void processComplex(const OdBrComplex& aComp)
{
  OdBrComplexShellTraverser  cst;
  cst.setComplex(aComp);
  int shellNum = 0;
  while (!cst.done())
  {
    OdBrShell cur = cst.getShell();
    std::cout << "\t\tShell: " << std::setw(idWidth) << shellNum;
    std::cout << '\n';

    processShell(cur);

    ++shellNum;
    cst.next();
  }
}

/************************************************************************/
/* Output the loops for the specified face                              */
/************************************************************************/
void processFace(const OdBrFace& f)
{
  OdBrFaceLoopTraverser  flt;
  flt.setFace(f);
  int loopNum = 0;
  while (!flt.done())
  {
    OdBrLoop cur = flt.getLoop();
    std::cout << "\t\t\t\tLoop: " << std::setw(idWidth) << loopNum << '\n';

    if (doLoopToCoEdge)
      processLoopToEdge(cur);
    else
      processLoopToVertex(cur);

    loopNum++;
    flt.next();
  }
}

/************************************************************************/
/* Output the edges in the specified loop                               */
/************************************************************************/
void processLoopToEdge(const OdBrLoop& l)
{
  OdBrLoopEdgeTraverser  lct;
  OdBrErrorStatus err = lct.setLoop(l);
  if (err == odbrDegenerateTopology)
  {
    std::cout << "\t\t\t\t singularities(need OdBrLoopVertexTraverser)\n";
    return;
  }
  
  int edgeNum = 0;
  while (!lct.done())
  {
    OdBrEdge cur = lct.getEdge();
    std::cout << "\t\t\t\t\tEdge: " << std::setw(idWidth) << edgeNum << '\n';

    processLoopEdge(lct);

    ++edgeNum;
    lct.next();
  }
}
/************************************************************************/
/* Output the vertices in the specified loop                            */
/************************************************************************/
void processLoopToVertex(const OdBrLoop& l)
{
  OdBrLoopVertexTraverser  lvt;
  lvt.setLoop(l);
  int vertexNum = 0;
  while (!lvt.done())
  {
    OdBrVertex cur = lvt.getVertex();
    std::cout << "\t\t\t\t\tVertex: " << std::setw(idWidth) << vertexNum;
    processVertex(cur);

    ++vertexNum;
    lvt.next();
  }
}

/************************************************************************/
/* Output the faces in the specified shell                              */
/************************************************************************/
void processShell(const OdBrShell& aShell)
{
  OdBrShellFaceTraverser  sft;
  sft.setShell(aShell);
  int faceNum = 0;
  while (!sft.done())
  {
    OdBrFace cur = sft.getFace();
    std::cout << "\t\t\tFace: " << std::setw(idWidth) << faceNum << '\n';

    processFace(cur);

    faceNum++;
    sft.next();
  }
}
/************************************************************************/
/* Output the specified vertex                                          */
/************************************************************************/
void processVertex(const OdBrVertex& vtx)
{
  std::cout << /* coords and then */ '\n';
}
/************************************************************************/
/* Traverse the Brep as specified by menu()                             */
/*                                                                      */
/*  1 ==> Complete top-down traversal of Brep, ending with loop->edge   */
/*  2 ==> Complete top-down traversal of Brep, ending with loop->vertex */
/*  3 ==> Brep-shell traverser only                                     */
/*  4 ==> Brep-face traverser only                                      */
/*  5 ==> Brep-edge traverser only                                      */
/*  6 ==> Brep-vertex traverser only                                    */
/************************************************************************/
void traverseBrep(OdBrBrep&  aBrep, int how)
{
  std::cout << "\nBrep " << '\n';

  if ( (how == 1) || (how == 2) )
  {
    doLoopToCoEdge = (how == 1);
    processBrep(aBrep);
  }
  else if (how == 3)
    processBrepToShellOnly(aBrep);
  else if (how == 4)
    processBrepToFaceOnly(aBrep);
  else if (how == 5)
    processBrepToEdgeOnly(aBrep);
  else if (how == 6)
    processBrepToVertexOnly(aBrep);

}
