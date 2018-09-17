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

#include "Br/BrBrep.h"

#include "BrSearch.h"
#include "BrTraverse.h"
#include "Br/BrMesh2dControl.h"
#include "Br/BrMesh2dFilter.h"
#include "Br/BrMesh2d.h"
#include "Br/BrMesh2dElement2dTraverser.h"
#include "Br/BrElement2d.h"
#include "Br/BrElement2dNodeTraverser.h"
#include "Br/BrNode.h"
#include "Ge/GePoint3d.h"

#ifndef SATBREPSAMPLEMODE
  #include "DbBody.h"
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


#define BROK_CHECK(a, b) if (a != odbrOK) {STD(cout) << b; return;}
#define EOK_CHECK(a, b) if (a != eOk) {STD(cout) << b; return;}


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

/************************************************************************/
/* Interactive Menu                                                     */
/************************************************************************/
int menu()
{
  STD(cout) << "\n\n"
  << "   1 ==> Complete top-down traversal of Brep, ending with loop->edge\n"
  << "   2 ==> Complete top-down traversal of Brep, ending with loop->vertex\n"
  << "   3 ==> Brep-shell traverser only\n"
  << "   4 ==> Brep-face traverser only\n"
  << "   5 ==> Brep-edge traverser only\n"
  << "   6 ==> Brep-vertex traverser only\n"
  << "   7 ==> Query for a face number, report surface geometry\n"
  << "   8 ==> Query for an edge number, \n"
  << "         report adjacent Edges, Loops, and curve geometry\n"
  << "   9 ==> Query for an SAT file vertex number;\n"
  << "         report adjacent Edges and Loops\n"
#ifndef SATBREPSAMPLEMODE
  << "  10 ==> OdBrMesh\n"
#endif
  << "   0  ==> exit\n"
  << "\n Choice: ";

  int choice;
  STD(cin) >> choice;
  return choice;
}
/************************************************************************/
/* Process the specified by menu()                                      */
/************************************************************************/
void processOption(OdBrBrep& br, OdRxObject *pEnt, int testToRun)
{
  if (testToRun < 7)
  {
    /********************************************************************/
    /* Traverse the Brep                                                */
    /********************************************************************/
    traverseBrep(br, testToRun);  
  }
  else if (testToRun == 7)
  {
    /********************************************************************/
    /*  Query for a face number, report surface geometry                */
    /********************************************************************/
    int faceSeqNum;
    STD(cout) << "Enter a face number (negative to quit): ";
    STD(cin) >> faceSeqNum;
    while (faceSeqNum >= 0)
    {
      faceSearch(br, faceSeqNum);
      STD(cout).flush();
      STD(cout) << "Enter a face number (negative to quit): ";
      STD(cin) >> faceSeqNum;
    }
  }
  else if (testToRun == 8)
  {
    /********************************************************************/
    /*  Query for an edge number, report adjacent Edges, Loops, & curves*/
    /********************************************************************/
    int edgeSeqNum;
    STD(cout) << "Enter an edge number (negative to quit): ";
    STD(cin) >> edgeSeqNum;
    while (edgeSeqNum >= 0)
    {
      edgeSearch(br,edgeSeqNum);
      STD(cout) << "Enter an edge number (negative to quit): ";
      STD(cin) >> edgeSeqNum;
    }
  }
  else if (testToRun == 9)
  {
    /********************************************************************/
    /*  Query for an vertex number, report adjacent edges and loops     */
    /********************************************************************/
    int vertexSeqNum;
    STD(cout) << "Enter a vertex number (negative to quit): ";
    STD(cin) >> vertexSeqNum;
    while (vertexSeqNum >= 0)
    {
      vertexSearch(br,vertexSeqNum);
      STD(cout) << "Enter a vertex number (negative to quit): ";
      STD(cin) >> vertexSeqNum;
    }
  }
#ifndef SATBREPSAMPLEMODE
  else if (testToRun == 10)
  {
    /********************************************************************/
    /*  Query for a mesh                                                */
    /********************************************************************/
    OdBrMesh2dControl control = OdBrMesh2dControl::OdBrMesh2dControlDefault;
    OdBrMesh2dFilter filter;
    BROK_CHECK(filter.set(br, control), "Filter initialization failed.\n");
    
    OdBrMesh2d mesh2d;
    EOK_CHECK(OdDbBody::generateMesh((OdDbEntity*)pEnt, filter, mesh2d), "Mesh generation failed.\n");
    
    OdBrMesh2dElement2dTraverser mesh2elem;
    BROK_CHECK(mesh2elem.setMesh(mesh2d), "Mesh2dElement initialization failed.\n");

    STD(cout) << "Mesh:";
    for(;!mesh2elem.done();mesh2elem.next())
    {
      STD(cout) << "  Next element:\n";
      OdBrElement2d element2d;
      BROK_CHECK(mesh2elem.getElement(element2d), "Mesh2dElement.getElement failed.\n");

      OdBrElement2dNodeTraverser entr;
      BROK_CHECK(entr.setElement(mesh2elem), "Element2dNode initialization failed.\n");

      for(;!entr.done();entr.next())
      {
        STD(cout) << "    Next node:";

        OdBrNode node;
        BROK_CHECK(entr.getNode(node), "Element2dNode.getNode failed.\n");

        OdGePoint3d pnt;
        BROK_CHECK(node.getPoint(pnt), "Node.getPoint failed.\n");
        STD(cout) << " (" << pnt.x << ", " << pnt.y << ", " << pnt.z << ')' << '\n';
      }
    }
  }
#endif
}


