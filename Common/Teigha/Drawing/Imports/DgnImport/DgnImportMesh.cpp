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

#include <OdaCommon.h>
#include "DgnImportMesh.h"
#include "DgnImportCommon.h"

#include <DgMesh.h>
#include <DbPolyFaceMesh.h>
#include <DbPolyFaceMeshVertex.h>
#include <DbFaceRecord.h>
#include <DbPolygonMesh.h>

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

void OdDgMeshFaceLoopsImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgMeshFaceLoops* mesh = (OdDgMeshFaceLoops*)e;

  // Check mesh face loop
  bool bImportThroughDraw = false;

  OdUInt32 nFaces = mesh->getFacesNumber();

  for(OdUInt32 k = 0; k < nFaces; ++k )
  {
    OdDgMeshFaceLoops::VerticesArray vertices;
    mesh->getFace(k, vertices);

    if( vertices.size() > 4)
    {
      bImportThroughDraw = true;
      break;
    }
  }

  if( bImportThroughDraw )
  {
    OdDgnImportPE::subImportElement( e, owner );
    return;
  }

  // Create polyface mesh

  OdDbPolyFaceMeshPtr pface = OdDbPolyFaceMesh::createObject();
  pface->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(pface, owner->objectId());
  owner->appendOdDbEntity(pface);
  OdGePoint3dArray points;
  int M = mesh->getPointsNumber();
  points.resize(M);
  for (int i = 0; i < M; ++i)
  {
    points[i] = mesh->getPoint(i);
    OdDbPolyFaceMeshVertexPtr v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(points[i]);
    pface->appendVertex(v);
  }
  int N = mesh->getFacesNumber();
  for (int i = 0; i < N; ++i)
  {
    OdDgMeshFaceLoops::VerticesArray vertices;
    mesh->getFace(i, vertices);
    OdDbFaceRecordPtr face = OdDbFaceRecord::createObject();
    if (vertices.size() < 5)
    {
      for (unsigned int j = 0; j < vertices.size(); ++j)
      {
        face->setVertexAt(j, vertices[j].m_pointIndex + 1);
        if (!vertices[j].m_impliesVisibleEdge)
          face->makeEdgeInvisibleAt(j);
      }
    }
    else
    {
      ODA_FAIL_ONCE();
    }
    pface->appendFaceRecord(face);
  }
  copyEntityProperties(mesh, pface);
}

//---------------------------------------------------------------------------------------------------

void OdDgMeshTriangleListImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgMeshTriangleList* mesh = (OdDgMeshTriangleList*)e;
  OdDbPolyFaceMeshPtr pface = OdDbPolyFaceMesh::createObject();
  pface->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(pface, owner->objectId());
  owner->appendOdDbEntity(pface);
  OdUInt32 N = mesh->getTrianglesNumber();
  for (OdUInt32 i = 0; i < N; ++i)
  {
    OdDgMeshTriangleList::Triangle triangle;
    mesh->getTriangle(i, triangle);
    OdDbPolyFaceMeshVertexPtr v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(triangle.m_points[0]);
    pface->appendVertex(v);
    v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(triangle.m_points[1]);
    pface->appendVertex(v);
    v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(triangle.m_points[2]);
    pface->appendVertex(v);
  }
  for (OdUInt32 i = 0; i < N; ++i)
  {
    OdDbFaceRecordPtr face = OdDbFaceRecord::createObject();
    face->setVertexAt(0, i * 3 + 1);
    face->setVertexAt(1, i * 3 + 2);
    face->setVertexAt(2, i * 3 + 3);
    pface->appendFaceRecord(face);
  }
  copyEntityProperties(mesh, pface);
}

//---------------------------------------------------------------------------------------------------

void OdDgMeshQuadListImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgMeshQuadList* mesh = (OdDgMeshQuadList*)e;
  OdDbPolyFaceMeshPtr pface = OdDbPolyFaceMesh::createObject();
  pface->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(pface, owner->objectId());
  owner->appendOdDbEntity(pface);
  OdUInt32 N = mesh->getQuadsNumber();
  for (OdUInt32 i = 0; i < N; ++i)
  {
    OdDgMeshQuadList::Quad quad;
    mesh->getQuad(i, quad);
    OdDbPolyFaceMeshVertexPtr v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(quad.m_points[0]);
    pface->appendVertex(v);
    v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(quad.m_points[1]);
    pface->appendVertex(v);
    v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(quad.m_points[2]);
    pface->appendVertex(v);
    v = OdDbPolyFaceMeshVertex::createObject();
    v->setPosition(quad.m_points[3]);
    pface->appendVertex(v);
  }
  for (OdUInt32 i = 0; i < N; ++i)
  {
    OdDbFaceRecordPtr face = OdDbFaceRecord::createObject();
    face->setVertexAt(0, i * 4 + 1);
    face->setVertexAt(1, i * 4 + 2);
    face->setVertexAt(2, i * 4 + 3);
    face->setVertexAt(3, i * 4 + 4);
    pface->appendFaceRecord(face);
  }
  copyEntityProperties(mesh, pface);
}

//---------------------------------------------------------------------------------------------------

void OdDgMeshTriangleGridImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgMeshTriangleGrid* mesh = (OdDgMeshTriangleGrid*)e;
  OdDbPolyFaceMeshPtr pface = OdDbPolyFaceMesh::createObject();
  pface->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(pface, owner->objectId());
  owner->appendOdDbEntity(pface);
  OdUInt32 x, y, xSize = mesh->getColumnsNumber(), ySize = mesh->getRowsNumber();

  for( x = 0; x < xSize; x++ )
  {
    for( y = 0; y < ySize; y++ )
    {
      OdDbPolyFaceMeshVertexPtr v = OdDbPolyFaceMeshVertex::createObject();
      v->setPosition(mesh->getPoint(y, x));
      pface->appendVertex(v);
    }
  }

  for( x = 0; x < xSize-1; x++ )
  {
    for( y = 0; y < ySize-1; y++ )
    {
      OdDbFaceRecordPtr face = OdDbFaceRecord::createObject();
      face->setVertexAt(0, x * ySize + y + 1);
      face->setVertexAt(1, (x + 1) * ySize + y + 1);
      face->setVertexAt(2, (x + 1) * ySize + y + 2);
      pface->appendFaceRecord(face);
      face = OdDbFaceRecord::createObject();
      face->setVertexAt(0, x * ySize + y + 1);
      face->setVertexAt(1, (x + 1) * ySize + y + 2);
      face->setVertexAt(2, x * ySize + y + 2);
      pface->appendFaceRecord(face);
    }
  }

  copyEntityProperties(mesh, pface);
}

//---------------------------------------------------------------------------------------------------

void OdDgMeshQuadGridImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgMeshQuadGrid* mesh = (OdDgMeshQuadGrid*)e;
  OdDbPolygonMeshPtr pface = OdDbPolygonMesh::createObject();
  pface->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(pface, owner->objectId());
  owner->appendOdDbEntity(pface);
  OdUInt32 N = mesh->getRowsNumber();
  OdUInt32 M = mesh->getColumnsNumber();
  pface->setMSize(M);
  pface->setNSize(N);
  for (OdUInt32 j = 0; j < M; ++j)
  {
    for (OdUInt32 i = 0; i < N; ++i)
    {
      OdDbPolygonMeshVertexPtr v = OdDbPolygonMeshVertex::createObject();
      v->setPosition(mesh->getPoint(i, j));
      pface->appendVertex(v);
    }
  }

  copyEntityProperties(mesh, pface);
}

//---------------------------------------------------------------------------------------------------

}
