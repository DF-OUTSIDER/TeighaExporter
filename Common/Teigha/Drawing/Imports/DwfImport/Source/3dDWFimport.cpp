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

#if defined(_MSC_VER)
// warning C4290: C++ Exception Specification ignored
#pragma warning ( disable : 4290 )
#if _MSC_VER <= 1200
// compiler stack overflows if these warning is enabled
#pragma warning ( disable : 4786)
#endif
#endif

#include "w3dtk/BStream.h"
#include "w3dtk/BStreamFileToolkit.h"
#include "w3dtk/BOpcodeShell.h"
#include "w3dtk/BOpcodeHandler.h"
#include "w3dtk/W3DCamera.h"
#include "dwf/package/EModelSection.h"
#include "dwfcore/MIME.h"

#include "StdAfx.h"
#include "../Include/DwfImport.h"
#include "DwfImportImpl.h"
#include "ProgressMonitor.h"
#include "DbPolyFaceMesh.h"
#include "DbPolyFaceMeshVertex.h"
#include "DbFaceRecord.h"
#include "DbLine.h"
#include "DbText.h"

#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbViewTable.h"
#include "DbBlockReference.h"
#include "Ge/GeCircArc3d.h"
#include "DbCircle.h"
#include "DbArc.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "DbEllipse.h"
#include "DbSpline.h"
#include "Ge/GeKnotVector.h"
#include "DbSolid.h"
#include "DbMaterial.h"

#include "Ge/GeTess.h"
#include "Ge/GeShell.h"
#include "Gi/GiUtils.h"
#include "Gi/GiRasterWrappers.h"

#if defined(ODA_WINDOWS)
#include <float.h>

#ifndef _DN_SAVE
#define _DN_SAVE      0x00000000
#endif
#ifndef _EM_DENORMAL
#define _EM_DENORMAL  0x00080000
#endif
#ifndef _MCW_DN
#define _MCW_DN       0x03000000
#endif
#endif

// enables w3d dumping
// #define _DEBUG_DUMP

namespace TD_DWF_IMPORT {
#define DECLARE_OPCODE_CLASS(Class, ParentClass)\
class Class : public ParentClass\
{\
  DwfImporter* m_pImp;\
public:\
  Class(DwfImporter* imp) : ParentClass(), m_pImp(imp){}\
  virtual TK_Status Execute(BStreamFileToolkit & tk);\
}


#define DECLARE_OPCODE_CLASS_2(Class, ParentClass)\
class Class : public ParentClass\
{\
  DwfImporter* m_pImp;\
public:\
  Class(DwfImporter* imp, unsigned char opcode) : ParentClass(opcode), m_pImp(imp){}\
  virtual TK_Status Execute(BStreamFileToolkit & tk);\
}

class EdgeDataCollector_ // CORE-12998 CORE-10297
{
public:
  class EdgeData
  {
  public:
    OdIntArray faceIndexes; // strange but size of it > 2 for 1stModel.dwfx 
    //int idVertextStart, idVertextEnd;
    bool visible;
    EdgeData()
      : visible(true)
    //, idVertextStart(-1), idVertextEnd(-1) // undef
    {
    }
  };
protected:
  OdGePoint3dArray& m_vertices;
  OdArray<OdUInt64> m_edges;

  OdGeVector3dArray m_faceNormals;

  OdGeVector3dArray m_vertexNormals;
  OdIntArray m_numVertexFaces; // num faces at vertex
  bool m_hasAveragedVertexNormals;

  std::map<OdUInt64, EdgeData> m_mapEdgeData; // by edge indexes OdUInt64(idStart, idEnd) - where idStart < idEnd
  double m_angleToCheckEdgeVisibility;

  inline OdUInt64 getEdgeDataKey(int idStart, int idEnd)
  {
    if (idStart > idEnd)
      std::swap(idStart, idEnd);
    ODA_ASSERT_ONCE(idStart >= 0 && idEnd > idStart);
    OdUInt64 key = OdUInt64(idStart) | (OdUInt64(idEnd) << 32);
    return key;
  }
public:
  inline bool isEdgeDataCollected(int idStart, int idEnd)
  {
    OdUInt64 key = getEdgeDataKey(idStart, idEnd);
    bool bRes = (m_mapEdgeData.find(key) != m_mapEdgeData.end());
    return bRes;
  }

  inline EdgeData& getEdgeData(int idStart, int idEnd, bool bCollect = false)
  {
    OdUInt64 key = getEdgeDataKey(idStart, idEnd);
    EdgeData& data = m_mapEdgeData[key];
    //ODA_ASSERT_ONCE_X(TDWF,    (data.idVertextStart < 0 || data.idVertextStart == idStart)
    //                        && (data.idVertextEnd < 0 || data.idVertextEnd == idEnd));
    //data.idVertextStart = idStart;
    //data.idVertextEnd = idEnd;
    if (bCollect)
      m_edges.push_back(key);
    return data;
  }

  EdgeDataCollector_(OdGePoint3dArray& vertices, int faceCount = 0, double angleToCheckEdgeVisibility = (TWO_PI / 12.0))
    : m_vertices(vertices)
    , m_angleToCheckEdgeVisibility(angleToCheckEdgeVisibility)
    , m_hasAveragedVertexNormals(false)
  {
    if (faceCount)
      m_faceNormals.reserve(faceCount);

    int sz = vertices.size();
    m_vertexNormals.resize(sz);
    m_vertexNormals.setAll(OdGeVector3d());
    m_numVertexFaces.resize(sz);
    m_numVertexFaces.setAll(0);
  }

  void addFace(int num, const int* pFaceIndexes) // kOdGiCounterClockwise
  {
    m_faceNormals.append(OdGeVector3d());
    OdGeVector3d& vNormal = m_faceNormals.last();
    if (num < 3)
    {
      ODA_FAIL_ONCE_X(TDWF); // TODO for loops
      return;
    }

    if (num >= 3)
    {
      int idx0 = pFaceIndexes[0], idx1 = pFaceIndexes[1], idx2 = pFaceIndexes[2];

      ODA_ASSERT_ONCE_X(TDWF,    idx0 >= 0 && idx0 < int(m_vertices.size())
                              && idx1 >= 0 && idx1 < int(m_vertices.size()) && idx1 != idx0
                              && idx2 >= 0 && idx2 < int(m_vertices.size()) && idx2 != idx0);
      OdGeError flag;
      vNormal = (m_vertices[idx1] - m_vertices[idx0]).crossProduct(m_vertices[idx2] - m_vertices[idx0]).normalize(OdGeContext::gTol, flag);
      //DWF_4_vertex_with_hole_7130_VK 0 & others: ODA_ASSERT_ONCE_X(TDWF, flag == OdGe::kOk);
    }

    for (int idx = 0; idx < num; idx++)
    {
      int idStart = pFaceIndexes[idx],
          idEnd = pFaceIndexes[idx < (num - 1) ? (idx + 1) : 0];
      EdgeData& data = getEdgeData(idStart, idEnd, true);
      data.faceIndexes.push_back(m_faceNormals.size() - 1);
      if (vNormal.isZeroLength())
      {
        //DWF_4_vertex_with_hole_7130_VK 0 & others: // ODA_FAIL_ONCE_X(TDWF);
        data.visible = true;
        continue;
      }

      OdGeVector3d& vVertexNormal = m_vertexNormals[pFaceIndexes[idx]];
      int& numVerFaces = m_numVertexFaces[pFaceIndexes[idx]];
      if (!numVerFaces)
      {
        ODA_ASSERT_ONCE_X(TDWF, vVertexNormal.isZeroLength());
        vVertexNormal = vNormal;
        numVerFaces++;
      }
      else
      {
        ODA_ASSERT_ONCE_X(TDWF, !vVertexNormal.isZeroLength());
        if (vVertexNormal != vNormal)
        {
          m_hasAveragedVertexNormals = true;
          ODA_ASSERT_ONCE_X(TDWF, !vVertexNormal.isZeroLength());
          vVertexNormal *= numVerFaces++;
          vVertexNormal += vNormal;
          vVertexNormal /= numVerFaces;
          if (vVertexNormal.isZeroLength()) // possible via opposite normals ...
          {
            vVertexNormal = OdGeVector3d();
            numVerFaces = 0;
          }
        }
        else
          numVerFaces++;
      }

      int szFace = data.faceIndexes.size();
      if (szFace == 1)
        continue;

      data.visible = false;
      for (int idxFace = 0; idxFace < (szFace - 1); idxFace++)
      {
        ODA_ASSERT_VAR(if (szFace > 2))
          ODA_ASSERT_VAR(szFace = szFace;) // brk// strange but szFace maybe > 2 (see 1stModel.dwfx) 
        int idFace = data.faceIndexes[idxFace];
        const OdGeVector3d& vNorm = m_faceNormals[idFace];
        if (vNorm.isZeroLength())
        {
          //ODA_FAIL_ONCE_X(TDWF);
          data.visible = true;
          break;
        }
        double ang = vNormal.angleTo(vNorm);
        if (fabs(ang) > m_angleToCheckEdgeVisibility) // (TWO_PI / 12.0)
          data.visible = true;
      }
    }
  }

  void getEdgeVisibilities(OdUInt8Array& edgeVisibilities)
  {
    int sz = (int) m_edges.size();
    edgeVisibilities.resize(sz);
    if (!sz)
      return;
    bool isUseless = true;
    for (int idx = 0; idx < sz; idx++)
    {
      OdUInt64 key = m_edges[idx];
      EdgeData& data = m_mapEdgeData[key];
      if (!data.visible)
        isUseless = false;
      edgeVisibilities[idx] = data.visible ? kOdGiVisible : kOdGiInvisible;
    }
    if (isUseless)
      edgeVisibilities.clear();
  }

  void getVertexNormals(OdGeVector3dArray& vertexNormals)
  {
    for (int idx = 0, sz = m_vertexNormals.size(); idx < sz; idx++)
    {
      if (!m_numVertexFaces[idx])
        continue;
      OdGeVector3d& vVertexNormal = m_vertexNormals[idx];
      OdGeError flag; vVertexNormal.normalize(OdGeContext::gTol, flag);
    }
    vertexNormals = m_vertexNormals;
  }

  bool hasAveragedVertexNormals()
  {
    return m_hasAveragedVertexNormals;
  }
};

static OdCmEntityColor toOdCmEntityColor(const float* pRGB, float fRoundOffs = 0.5)
{
  OdCmEntityColor color;
  OdUInt8 red = OdUInt8(pRGB[0] * 255 + fRoundOffs),
          green = OdUInt8(pRGB[1] * 255 + fRoundOffs),
          blue = OdUInt8(pRGB[2] * 255 + fRoundOffs);
  color.setRGB(red, green, blue);
  return color;
}

static OdCmColor toOdCmColor(const OdCmEntityColor& clr)
{
  OdCmColor color; ODA_ASSERT_ONCE_X(TDWF, color.isByLayer() && !clr.isByLayer());
  OdUInt8 red_ = clr.red(), green_ = clr.green(), blue_ = clr.blue();
  if (red_ != 255 || green_ != 255 || blue_ != 255)
    color.setRGB(red_, green_, blue_);
  ODA_ASSERT_VAR(else)
    ODA_ASSERT(true); // brk
  return color;
}

// CORE-10565 split shell with large geometry
static OdDbSubDMeshPtr toOdDbSubDMesh(OdGeShell& geShell)
{
  OdInt32Array faceArray;

  OdDbSubDMeshPtr pSubDMesh = OdDbSubDMesh::createObject();
  OdInt32 subDLevel = 0;
  OdResult res;
  res = pSubDMesh->setSubDMesh(geShell.vertices, geShell.faces, subDLevel);
  ODA_ASSERT_ONCE_X(TDWF, res == eOk);
  //res = pSubDMesh->setVertexAt(OdInt32 nIndex, const OdGePoint3d& vertex);
  if (geShell.vertexNormals.size())
    res = pSubDMesh->setVertexNormalArray(geShell.vertexNormals);
  ODA_ASSERT_ONCE_X(TDWF, res == eOk);
  ODA_ASSERT_ONCE_X(TDWF, !geShell.faceNormals.size()); // test for unsupported data
  if (geShell.textureCoords.size())
  {
    // CORE-10297 CORE-13187
    // ACAD does not support axis mirror transformation (-Y) on in texture map
    // therefore the transformation in getTexture was move here ( or into image transformation (see DwfImporter::loadW3d))
    OdGeMatrix3d matrix;
    matrix.setCoordSystem(OdGePoint3d::kOrigin, OdGeVector3d::kXAxis, -OdGeVector3d::kYAxis, OdGeVector3d::kZAxis);
    for (OdUInt32 idx = 0, sz = geShell.textureCoords.size(); idx < sz; idx++)
      geShell.textureCoords[idx].transformBy(matrix);

    res = pSubDMesh->setVertexTextureArray(geShell.textureCoords);
  }
  ODA_ASSERT_ONCE_X(TDWF, res == eOk);
  if (geShell.faceTrueColors.size())
  {
    OdCmEntityColor color; color.setColorMethod(OdCmEntityColor::kByLayer);
    ODA_ASSERT_ONCE_X(TDWF, color.isByLayer());
    for (OdUInt32 idx = 0, sz = geShell.faceTrueColors.size(); idx < sz; idx++)
    {
      if (!idx)
      {
        color = geShell.faceTrueColors[idx];
        ODA_ASSERT_ONCE_X(TDWF, !color.isByLayer());
      }
      if (color == geShell.faceTrueColors[idx])
        continue;
      ODA_FAIL_ONCE_X(TDWF); // test
      color.setColorMethod(OdCmEntityColor::kByLayer);
      break;
    }
    if (!color.isByLayer())
    {
      OdUInt32 sz = geShell.vertices.size();
      OdCmEntityColorArray arrColor;
      arrColor.resize(sz);
      arrColor.setAll(color);
      res = pSubDMesh->setVertexColorArray(arrColor);
      ODA_ASSERT_ONCE_X(TDWF, res == eOk);
    }
  }
  // there is no way to support it & texture coordinates at once //ODA_ASSERT_ONCE_X(TDWF, !geShell.edgeVisibilities.size()); // test for unsupported data

  return pSubDMesh;
}

static OdDbPolyFaceMeshPtr toOdDbPolyFaceMesh(OdUInt32 numVertices, const OdGePoint3d* pVertexList,
                                              OdInt32 numFaces, const OdInt32* pFaceList,
                                              OdUInt32 numFaceColors, const OdCmEntityColor* pFaceColors,
                                              OdUInt32 numEdgeVisibilities, const OdUInt8* pEdgeVisibilities,
                                              EdgeDataCollector_* pCollector = NULL)
{
  ODA_ASSERT_VAR(const OdUInt32 maxVertices = 0x7FFF;) //OdInt16
  ODA_ASSERT_ONCE_X(TDWF, numVertices && numVertices <= maxVertices)

  OdDbPolyFaceMeshPtr pFaceMesh = OdDbPolyFaceMesh::createObject();

  for (OdUInt32 idxVrt = 0; idxVrt < numVertices; idxVrt++)
  {
    const OdGePoint3d& pt = pVertexList[idxVrt];
    OdDbPolyFaceMeshVertexPtr pVrt = OdDbPolyFaceMeshVertex::createObject();
    pVrt->setPosition(pt);
    pFaceMesh->appendVertex(pVrt);
  }

  for (OdInt32 idxFace = 0, offset = 0, offsetEdgeVis = 0, num = 0; 
       offset < (OdInt32) numFaces; offset += num + 1)
  {
    num = pFaceList[offset];
    ODA_ASSERT_ONCE_X(TDWF, num > 0);
    if (num < 0)
    {
      num = -num;
      continue;
    }

    OdDbFaceRecordPtr pFace = OdDbFaceRecord::createObject();
    ODA_ASSERT_ONCE_X(TDWF, !numFaceColors || idxFace < (OdInt32)numFaceColors);
    if (idxFace < (OdInt32)numFaceColors)
    {
      OdCmEntityColor entColor = pFaceColors[idxFace++];
      OdCmColor color = toOdCmColor(entColor);
      pFace->setColor(color);
    }

    int idx;
    if (   num <= 4 
        && (   (offset + num + 1) >= numFaces    // next num is missing
            || pFaceList[offset + num + 1] > 0)) // or is positive
    {
      for (idx = 0; idx < num; idx++)
      {
        // see OdDbPolyFaceMesh::subWorldDraw
        //  const OdDbFaceRecordImpl* pFaceImpl = OdDbFaceRecordImpl::getImpl(pFace.get());
        //  // Acad uses only the first vertices with not 0 index
        // ?=> ] + 1) in next line
        pFace->setVertexAt(idx, (OdInt16) pFaceList[offset + idx + 1] + 1);

        ODA_ASSERT_ONCE_X(TDWF, OdUInt32(offsetEdgeVis + num) <= numEdgeVisibilities || !pCollector);
        if (!pCollector) // || OdUInt32(offsetEdgeVis + num) > numEdgeVisibilities
          continue;

        int idStart = pFaceList[offset + idx + 1],
            idEnd = pFaceList[idx < (num - 1) ? (offset + idx + 2) : (offset + 1)];
        if (!pCollector->isEdgeDataCollected(idStart, idEnd))
        {
          ODA_FAIL_ONCE_X(TDWF);
          continue;
        }
        
        EdgeDataCollector_::EdgeData& data = pCollector->getEdgeData(idStart, idEnd);
        bool bVisible = data.visible;

        ODA_ASSERT_VAR(if (OdUInt32(offsetEdgeVis + num) <= numEdgeVisibilities))
        {
          ODA_ASSERT_VAR(bool bVisible_ = (pEdgeVisibilities[offsetEdgeVis + idx] == kOdGiVisible);)
            ODA_ASSERT_ONCE_X(TDWF, bVisible_ == data.visible);
        }

        if (bVisible)
          pFace->makeEdgeVisibleAt(idx);
        else
          pFace->makeEdgeInvisibleAt(idx);
      }
      offsetEdgeVis += num;
    }
    else // (num <= 4
    {
      offsetEdgeVis += num;

      OdGePoint3dArray local_points = OdGePoint3dArray::create(const_cast<OdGePoint3d*>(pVertexList), numVertices);
      OdGeVector3d normal = odgiFaceNormal(numVertices, pVertexList);
      if (!normal.isZeroLength())
      {
        OdGeMatrix3d w2p = OdGeMatrix3d::worldToPlane(normal);
        for (idx = 0; idx < (int) local_points.size(); idx++)
          local_points[idx].transformBy(w2p);
      }
      OdGeTess::ContourPtr pContour = OdGeTess::Contour::create(local_points.asArrayPtr());
      for (idx = 0; idx < num; idx++)
        pContour->addVertex(pFaceList[offset + idx + 1]);
      while ((offset + num + 1) < numFaces && pFaceList[offset + num + 1] < 0) // holes
      {
        offset += num + 1;
        num = - pFaceList[offset];
        idxFace++;
        ODA_ASSERT(num > 0);
        OdGeTess::ContourPtr pHole = OdGeTess::Contour::create(local_points.asArrayPtr());
        for (idx = 0; idx < num; idx++)
          pHole->addVertex(pFaceList[offset + idx + 1]);
        pContour->insert(pHole);
      }
      OdGeTess::ContourPtr pCurrContour = pContour->breakToConvex(0);
      OdInt16 faceList[3];
      while (pCurrContour.get())
      {
        OdGeTess::Vertex* pVert = pCurrContour->head();
        faceList[0] = (OdInt16)pVert->index();
        pVert = pVert->next();

        faceList[2] = (OdInt16)pVert->index();
        pVert = pVert->next();

        do
        {
          faceList[1] = faceList[2];
          faceList[2] = (OdInt16)pVert->index();
          for (idx = 0; idx < 3; idx++)
          {
            pFace->setVertexAt(idx, faceList[idx] + 1);
            if (!pCollector)
              continue;

            int idStart = faceList[idx],
                idEnd = faceList[idx < 2 ? (idx + 1) : 0];
            bool bVisible = false;
            if (pCollector->isEdgeDataCollected(idStart, idEnd))
            {
              EdgeDataCollector_::EdgeData& data = pCollector->getEdgeData(idStart, idEnd);
              bVisible = data.visible;
            }
            if (bVisible)
              pFace->makeEdgeVisibleAt(idx);
            else
              pFace->makeEdgeInvisibleAt(idx);
          } // for

          pFaceMesh->appendFaceRecord(pFace);

          pFace = OdDbFaceRecord::createObject();
          ODA_ASSERT_ONCE_X(TDWF, !numFaceColors || idxFace < (OdInt32)numFaceColors);
          if (idxFace < (OdInt32)numFaceColors)
          {
            OdCmEntityColor entColor = pFaceColors[idxFace++];
            OdCmColor color = toOdCmColor(entColor);
            pFace->setColor(color);
          }

          pVert = pVert->next();
        } while (pVert != pCurrContour->head());

        pCurrContour = pCurrContour->next();
      }
    } // (num <= 4

    pFaceMesh->appendFaceRecord(pFace);
  }
  // unsupported (also as other things) //if (geShell.vertexNormals.size()) res = ?pFaceMesh->setVertexNormalArray(geShell.vertexNormals);

  return pFaceMesh;
}

static OdArray<OdDbPolyFaceMeshPtr> toOdDbPolyFaceMeshes(const OdGeShell& geShell, EdgeDataCollector_* pCollector = NULL)
{
  OdArray<OdDbPolyFaceMeshPtr> meshes;

  OdUInt32 numVertices = geShell.vertices.size();
  const OdGePoint3d* vertexList = geShell.vertices.asArrayPtr();
  OdUInt32 faceListSize = geShell.faces.size();
  const OdInt32* faceList = geShell.faces.asArrayPtr();
  OdUInt32 numFaceColors = geShell.faceTrueColors.size();
  const OdCmEntityColor* pFaceColors = geShell.faceTrueColors.asArrayPtr();
  OdUInt32 numEdgeVisibilities = geShell.edgeVisibilities.size();
  const OdUInt8* pEdgeVisibilities = geShell.edgeVisibilities.asArrayPtr(); // kOdGiVisible or kOdGiInvisible
  const OdUInt32 nLimit = 0x7FFF; //OdInt16

  if (numVertices <= nLimit)
  {
    OdDbPolyFaceMeshPtr pMesh = toOdDbPolyFaceMesh(numVertices, vertexList, faceListSize, faceList,
                                                   numFaceColors, pFaceColors,
                                                   numEdgeVisibilities, pEdgeVisibilities, numEdgeVisibilities ? pCollector : NULL);
    meshes.push_back(pMesh);
    return meshes;
  }

  ODA_FAIL_ONCE_X(TDWF); // TODO // use code OdGiGeometrySimplifier::subdivideShellByVertexLimit for it
  return meshes;
}

DECLARE_OPCODE_CLASS(DD_TK_Shell,TK_Shell);

//#if 1
TK_Status DD_TK_Shell::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Shell::Execute(tk);

  OdGeShell geShell;

  OdUInt32 numVertices = (OdUInt32) GetPointCount(); // mp_pointcount
  geShell.vertices.reserve(numVertices);

  OdUInt32 idx;
  for (idx = 0; idx < numVertices; idx++)
  {
    OdGePoint3d pt(GetPoints()[idx * 3], GetPoints()[idx * 3 + 1], GetPoints()[idx * 3 + 2]);
    //m_pCtx->transform(pt);
    geShell.vertices.append(pt);
  }

  // #22743 problem with dataFiles/tests/bitmapTestSuite/7130/default3D.dwf
  ODA_ASSERT_ONCE_X(TDWF, m_flistlen > 0);
  if (m_flistlen <= 0)
    return res;

  OdInt32Array& destFaces = geShell.faces;
  destFaces.resize(m_flistlen);
  memcpy(destFaces.asArrayPtr(), GetFaces(), m_flistlen * sizeof(OdInt32));
  OdInt32Array facesWithoutLoops; facesWithoutLoops.reserve(m_flistlen); ODA_ASSERT_ONCE_X(TDWF, !facesWithoutLoops.size());

  EdgeDataCollector_ collector(geShell.vertices, m_flistlen);
  bool useCollector = !HasEdgeVisibilities(); // mp_evisibilities mp_evisibilitycount
  bool hasFaceLoop = false, hasLargeFace = false;

  if (GetFaceExists() || (useCollector && GetFaces()))
  {
    for (int idxFace = 0, offset = 0, num = 0; idxFace < mp_facecount; offset += num + 1)
    {
      num = GetFaces()[offset];
      if (!hasLargeFace && num > 4)
        hasLargeFace = true; // DWF_4_vertex_with_hole_7130_VK 0
      if (num < 0)
      {
        hasFaceLoop = true;
        num = -num;
        continue;
      }
      facesWithoutLoops.push_back(num);
      for (int idxCpy = 0; idxCpy < num; idxCpy++)
        facesWithoutLoops.push_back(GetFaces()[offset + idxCpy + 1]);

      if (useCollector)
        collector.addFace(num, &GetFaces()[offset + 1]);

      if (GetFaceExists())
      {
        unsigned int exist = GetFaceExists()[idxFace];
        switch (exist)
        {
        case Face_Color:
        {
          OdCmEntityColor color = toOdCmEntityColor(&GetFaceColors()[idxFace * 3]); // , 0);
          ODA_ASSERT_ONCE_X(TDWF, !geShell.faceColors.size());
          geShell.faceTrueColors.append(color);
          break;
        }
        //case Face_Index: // face color-by-index
        //case Face_Any_Color: case Face_Normal: case Face_Visibility: case Face_Pattern:
        default:
          ODA_ASSERT_ONCE_X(TDWF, exist == Face_None); // TODO
          break;
        }
      }
      idxFace++;
    } // for
  } // if (GetFaceExists() || (useCollector && GetFaces()))

  if (HasVertexNormals()) // mp_normals != 0
  {
    ODA_ASSERT_ONCE_X(TDWF, mp_normals && mp_pointcount);
    // mp_normals is float[3 * mp_pointcount]
    float* pNormals = GetVertexNormals(); // mp_normals
    geShell.vertexNormals.reserve(numVertices);
    for (idx = 0; idx < numVertices; idx++, pNormals += 3)
    {
      OdGeVector3d vNormal(pNormals[0], pNormals[1], pNormals[2]);
      geShell.vertexNormals.append(vNormal);
    }
  }
  if (HasFaceNormals()) // mp_fnormals != 0
  {
    ODA_FAIL_ONCE_X(TDWF); // test of existance
    ODA_ASSERT_ONCE_X(TDWF, mp_fnormals && mp_facecount);
    //mp_fnormals is float[3 * mp_facecount]
    float* pNormals = mp_fnormals;
    geShell.faceNormals.reserve(mp_facecount);
    for (idx = 0; idx < (OdUInt32) mp_facecount; idx++, pNormals += 3)
    {
      OdGeVector3d vNormal(pNormals[0], pNormals[1], pNormals[2]);
      geShell.faceNormals.append(vNormal);
    }
  }
  if (HasEdgeNormals()) // mp_enormals != 0
  {
    ODA_ASSERT_ONCE_X(TDWF, mp_edge_exists && mp_enormals && mp_edgecount);
    // mp_enormals is float[3 * mp_edgecount]
    ODA_FAIL_ONCE_X(TDWF); // TODO ?
  }
  if (useCollector && geShell.vertexNormals.isEmpty())
    collector.getVertexNormals(geShell.vertexNormals);

  if (HasVertexParameters()) // mp_params
  {
    ODA_ASSERT_ONCE_X(TDWF, mp_paramwidth == 3); // TODO
    if (mp_paramwidth == 3)
    {
      geShell.textureCoords.reserve(numVertices);
      //  parameters should refer to the stu triplets (again, triplets, not pairs) for the 
      //  values of the vertex parameters.These values will be copied, and the appropriate bits in mp_exists will
      //  be updated.If "width" is <= 0, this function will use the value from mp_paramwidth.
      //  \param width specifies the number of floating point values per parameter.For example, a single 2d texture
      //  will have 2, whereas two 3d textures will have 6.
      float* pTriplets = GetVertexParameters(); //mp_params[mp_paramwidth * numVertices]
      for (idx = 0; idx < numVertices; idx++, pTriplets += mp_paramwidth)
      {
        // maybe out or range 0...1 - it's standard practic
        OdGePoint3d triplet(pTriplets[0], pTriplets[1], pTriplets[2]);
        geShell.textureCoords.append(triplet);
      }
    }
  }

  // CornellBox.dwfx
  if (!useCollector && mp_vmvisibilitycount) // && HasEdgeVisibilities() // mp_evisibilities mp_evisibilitycount
  {
    ODA_ASSERT_ONCE_X(TDWF, mp_evisibilities);
    OdUInt32 numEdgeVisibilities = (OdUInt32) mp_evisibilitycount; // mp_vmvisibilitycount;
    geShell.edgeVisibilities.reserve(numEdgeVisibilities);
    bool isUseless = true;
    for (OdUInt32 idxEdge = 0; idxEdge < numEdgeVisibilities; idxEdge++)
    {
      OdUInt8 val = (mp_evisibilities[idxEdge] & Edge_Visibility) ? kOdGiVisible : kOdGiInvisible;
      if (val)
        isUseless = false;
      geShell.edgeVisibilities.append(val);
    }
    if (isUseless)
      geShell.edgeVisibilities.clear();
  }
  else if (useCollector)
    collector.getEdgeVisibilities(geShell.edgeVisibilities); // CORE-12998

  bool useOdDbSubDMesh = false;
  if (!useOdDbSubDMesh && !hasFaceLoop && geShell.textureCoords.size() && !m_pImp->checkMaterialId().isNull())
    useOdDbSubDMesh = true;
  if (   !useOdDbSubDMesh && !hasFaceLoop && geShell.vertexNormals.size()
      && (!useCollector || collector.hasAveragedVertexNormals()))
    useOdDbSubDMesh = true;
  if (!useOdDbSubDMesh)
  {
    const OdUInt32 maxVertices = 0x7FFF; //OdInt16
    OdUInt32 numVertices = geShell.vertices.size();
    ODA_ASSERT_ONCE_X(TDWF, numVertices < maxVertices || !hasFaceLoop); // TODO
    if (numVertices >= maxVertices)
      useOdDbSubDMesh = true;
  }
  if (useOdDbSubDMesh && facesWithoutLoops.size() != destFaces.size())
  {
    ODA_ASSERT_ONCE_X(TDWF, hasFaceLoop && facesWithoutLoops.size() < destFaces.size());
    destFaces = facesWithoutLoops;
  }

  if (useOdDbSubDMesh)
  {
    OdDbSubDMeshPtr pSubDMesh = toOdDbSubDMesh(geShell);
    ODA_ASSERT_ONCE_X(TDWF, pSubDMesh.get());
    if (pSubDMesh.get())
      m_pImp->add3dObject(pSubDMesh, TKO_Geo_Face);
    else
      res = TK_Error;
  }
  else
  {
    OdArray<OdDbPolyFaceMeshPtr> meshes = toOdDbPolyFaceMeshes(geShell, useCollector ? &collector : NULL);
    OdUInt32 idx = 0, sz = meshes.size();
    if (!sz)
      res = TK_Error;
    for (; idx < sz; idx++)
    {
      OdDbPolyFaceMeshPtr pFaceMesh = meshes[idx];
      ODA_ASSERT_ONCE_X(TDWF, pFaceMesh.get());
      if (pFaceMesh.get())
        m_pImp->add3dObject(pFaceMesh, TKO_Geo_Face);
      else
        res = TK_Error;
    }
  }

  return res;
}
//#else
//TK_Status DD_TK_Shell::Execute(BStreamFileToolkit & tk)
//{
//  TK_Status res = TK_Shell::Execute(tk);
//  OdDbPolyFaceMeshPtr pface = OdDbPolyFaceMesh::createObject();
//  OdGePoint3dArray points;
//  points.resize(GetPointCount());
//  int nPointCount = GetPointCount();
//  const int cnCountMax = 0x7FFF; //OdInt16
//  if (nPointCount > cnCountMax)
//  {
//    ODA_ASSERT_ONCE_X(TDWF, !"TODO for CORE-10565 Problem importing dwfx file with large geometry");
//    // see see OdGiGeometrySimplifier::subdivideShellByVertexLimit
//    nPointCount = cnCountMax;
//  }
//  for (int i = 0; i < nPointCount; ++i)
//  {
//    points[i] = OdGePoint3d(GetPoints()[i*3], GetPoints()[i*3+1], GetPoints()[i*3+2]);
//    OdDbPolyFaceMeshVertexPtr v = OdDbPolyFaceMeshVertex::createObject();
//    v->setPosition(points[i]);
//    pface->appendVertex(v);
//  }
//  for (int k = 0,j = 0; k < mp_facecount; k++ )
//  {
//    OdDbFaceRecordPtr face = OdDbFaceRecord::createObject();
//    if (GetFaceExists() && (GetFaceExists()[k] & Face_Color))
//    {
//      OdCmColor color;
//      color.setRGB(OdUInt8(GetFaceColors()[k*3]*255),OdUInt8(GetFaceColors()[k*3+1]*255),OdUInt8(GetFaceColors()[k*3+2]*255));
//      face->setColor(color);
//    }
//
//    int N = GetFaces()[j];
//    ODA_ASSERT(N > 0);
//    if (N <= 4 && ((j + N + 1) >= GetFacesLength() || GetFaces()[j + N + 1] > 0))
//    {
//      for (int i = 0; i < N; i++)
//        face->setVertexAt(i, OdInt16(GetFaces()[j + i + 1] + 1));
//    }
//    else
//    {
//      OdGePoint3dArray local_points(points);
//      OdGeVector3d normal = odgiFaceNormal(points.size(), points.asArrayPtr());
//      if (!normal.isZeroLength())
//      {
//        OdGeMatrix3d w2p = OdGeMatrix3d::worldToPlane(normal);
//        for (unsigned int i = 0; i < local_points.size(); ++i)
//          local_points[i].transformBy(w2p);
//      }
//      OdGeTess::ContourPtr pContour = OdGeTess::Contour::create(local_points.asArrayPtr());
//      for (int i = 0; i < N; i++)
//        pContour->addVertex(GetFaces()[j + i + 1]);
//      while ((j + N + 1) < GetFacesLength() && GetFaces()[j + N + 1] < 0) // holes
//      {
//        j += N + 1;
//        N = -GetFaces()[j];
//        ++k;
//        ODA_ASSERT(N > 0);
//        OdGeTess::ContourPtr pHole = OdGeTess::Contour::create(local_points.asArrayPtr());
//        for (int i = 0; i < N; i++)
//          pHole->addVertex(GetFaces()[j + i + 1]);
//        pContour->insert(pHole);
//      }
//      OdGeTess::ContourPtr pCurrContour = pContour->breakToConvex(0);
//      OdInt16 faceList[3];
//      while(pCurrContour.get())
//      {
//        OdGeTess::Vertex* pVert = pCurrContour->head();
//        faceList[0] = (OdInt16)pVert->index();
//        pVert = pVert->next();
//
//        faceList[2] = (OdInt16)pVert->index();
//        pVert = pVert->next();
//
//        do
//        {
//          faceList[1] = faceList[2];
//          faceList[2] = (OdInt16)pVert->index();
//          face->setVertexAt(0, faceList[0] + 1);
//          face->setVertexAt(1, faceList[1] + 1);
//          face->setVertexAt(2, faceList[2] + 1);
//          pface->appendFaceRecord(face);
//          face = OdDbFaceRecord::createObject();
//          if (GetFaceExists() && (GetFaceExists()[k] & Face_Color))
//          {
//            OdCmColor color;
//            color.setRGB(OdUInt8(GetFaceColors()[k*3]*255),OdUInt8(GetFaceColors()[k*3+1]*255),OdUInt8(GetFaceColors()[k*3+2]*255));
//            face->setColor(color);
//          }
//          pVert = pVert->next();
//        }
//        while(pVert!=pCurrContour->head());
//
//        pCurrContour = pCurrContour->next();
//      }
//    }
//    pface->appendFaceRecord(face);
//    j += N + 1;
//  }
//  m_pImp->add3dObject(pface, TKO_Geo_Face);
//  return res;
//}
//#endif

DECLARE_OPCODE_CLASS(DD_TK_User_Options, TK_User_Options);

TK_Status DD_TK_User_Options::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_User_Options::Execute(tk);
  tk.LogEntry(GetOptions());
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Mesh,TK_Mesh);

TK_Status DD_TK_Mesh::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Mesh::Execute(tk);
  ODA_FAIL_ONCE();
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Line,TK_Line);

TK_Status DD_TK_Line::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Line::Execute(tk);
  OdDbLinePtr line = OdDbLine::createObject();
  line->setStartPoint(OdGePoint3d(GetPoints()[0],GetPoints()[1],GetPoints()[2]));
  line->setEndPoint(OdGePoint3d(GetPoints()[3],GetPoints()[4],GetPoints()[5]));
  m_pImp->add3dObject(line, TKO_Geo_Line);
  return res;
}

static OdString getMaterialName(DwfImporter* pImp, struct DwfImporter::SegmentAttributes* pSeg)
{
  OdString sName = L"DwfImport_";
  struct DwfImporter::SegmentAttributes* pSeg_ = pSeg;
  for (; pSeg_; pSeg_ = pSeg_->_parent)
  {
    if (pSeg_->_name.isEmpty())
      continue;
    sName += (pSeg_ == pSeg ? L"Seg_" : L"Prnt_") + pSeg_->_name;
    break;
  }
  ODA_ASSERT_ONCE_X(TDWF, pSeg_); // test
  sName += OdString().format(L"__%d", pImp->_idxNextMaterial++);
  sName.replace(L'?', L'_'); sName.replace(L'/', L'_');
  return sName;
}

static OdDbMaterialPtr createDbMaterial(OdDbDictionaryPtr pMatDict, const OdString& sName,
                                        const OdGiMaterialTraitsData& dataMaterial)
{
  OdDbMaterialPtr pMaterial = OdDbMaterial::createObject();

  OdGiMaterialColor diffuseColor; OdGiMaterialMap diffuseMap;
  dataMaterial.diffuse(diffuseColor, diffuseMap);
  pMaterial->setDiffuse(diffuseColor, diffuseMap);
  //
  OdGiMaterialColor specularColor; OdGiMaterialMap specularMap; double glossFactor = 0;
  dataMaterial.specular(specularColor, specularMap, glossFactor);
  pMaterial->setSpecular(specularColor, specularMap, glossFactor);
  //
  pMaterial->setReflectivity(dataMaterial.reflectivity());
  //
  double opacity; OdGiMaterialMap opacityMap;
  dataMaterial.opacity(opacity, opacityMap);
  pMaterial->setOpacity(opacity, opacityMap);
  //
  OdGiMaterialColor ambientColor;
  dataMaterial.ambient(ambientColor);
  pMaterial->setAmbient(ambientColor);
  //
  //OdGiMaterialColor emissionColor; OdGiMaterialMap emissionMap;
  //dataMaterial.emission(emissionColor, emissionMap);
  //pMaterial->setEmission(emissionColor, emissionMap);
  //
  double refractionIndex = 0; OdGiMaterialMap refractionMap;
  dataMaterial.refraction(refractionIndex, refractionMap);
  pMaterial->setRefraction(refractionIndex, refractionMap);
  //
  OdGiMaterialMap bumpMap;
  dataMaterial.bump(bumpMap);
  pMaterial->setBump(bumpMap);

#if 1
  OdGiMaterialMap reflectionMap;
  dataMaterial.reflection(reflectionMap);
  pMaterial->setReflection(reflectionMap);
  pMaterial->setTranslucence(dataMaterial.translucence());
  pMaterial->setSelfIllumination(dataMaterial.selfIllumination());
  pMaterial->setMode(dataMaterial.mode());
  pMaterial->setChannelFlags(dataMaterial.channelFlags());
  pMaterial->setIlluminationModel(dataMaterial.illuminationModel());
  pMaterial->setColorBleedScale(dataMaterial.colorBleedScale());
  pMaterial->setIndirectBumpScale(dataMaterial.indirectBumpScale());
  pMaterial->setReflectanceScale(dataMaterial.reflectanceScale());
  pMaterial->setTransmittanceScale(dataMaterial.transmittanceScale());
  pMaterial->setTwoSided(dataMaterial.twoSided());
  pMaterial->setLuminanceMode(dataMaterial.luminanceMode());
  pMaterial->setLuminance(dataMaterial.luminance());
  //pMaterial->setShininess(dataMaterial.shininess()); // SpecularGloss
  OdGiMaterialMap normalMap; OdGiMaterialTraits::NormalMapMethod method; double strength;
  dataMaterial.normalMap(normalMap, method, strength);
  pMaterial->setNormalMap(normalMap, method, strength);
  //pMaterial->setAnonymous(dataMaterial.isAnonymous());
  pMaterial->setGlobalIllumination(dataMaterial.globalIllumination());
  pMaterial->setFinalGather(dataMaterial.finalGather());
#endif

  pMaterial->setName(sName);
  pMatDict->upgradeOpen();
  pMatDict->setAt(sName, pMaterial);
  //if (pMaterial->objectId().isNull())
  //  pMatDict->database()->addOdDbObject(pMaterial);
  ODA_ASSERT_ONCE_X(TDWF, !pMaterial->objectId().isNull());

  OdGiMaterialTraitsTaker giData;
  pMaterial->setAttributes(&giData);
  ODA_ASSERT_VAR(bool bOk = (dataMaterial == giData.getData());)
  ODA_ASSERT_ONCE_X(TDWF, bOk);

  return pMaterial;
}

OdDbObjectId DwfImporter::checkMaterialId()
{
  struct DwfImporter::SegmentAttributes* pSeg = _currentSegment;

  if (pSeg->_idMaterial.isNull() && OdGiMaterialTraitsData() == pSeg->_dataMaterial)
    return OdDbObjectId();
  
  OdDbMaterialPtr pMaterial;
  if (   pSeg->_idMaterial.isNull() && !_lastMaterialId.isNull()
      && _lastGiData.getData() == pSeg->_dataMaterial)
    pSeg->_idMaterial = _lastMaterialId;
    
  if (!pSeg->_idMaterial.isNull())
    return pSeg->_idMaterial;

  OdDbDictionaryPtr pMatDict = database()->getMaterialDictionaryId().safeOpenObject();
  if (pMatDict->numEntries())
  {
    // try to find through  all _dataMaterial in database
    for (OdDbDictionaryIteratorPtr pItr = pMatDict->newIterator(); !pItr->done(); pItr->next())
    {
      pMaterial = pItr->getObject();
      ODA_ASSERT_ONCE_X(TDWF, pMaterial.get());
      OdGiMaterialTraitsTaker giData;
      pMaterial->setAttributes(&giData);

      if (giData.getData() != pSeg->_dataMaterial)
        continue;

      pSeg->_idMaterial = _lastMaterialId = pMaterial->objectId();
      _lastGiData.setData(giData.getData());
      ODA_ASSERT_ONCE_X(TDWF, !pItr->name().find(L"DwfImport_"));
      //TODO update name by using info
      //OdString sName = pItr->name();
      //if (!sName.find(L"DwfImport_"))
      //{
      //  //if (!pSeg->_name.isEmpty())
      //  //{
      //  //  // rename
      //  //  sName += L"__" + pSeg->_name;
      //  //  pMaterial->upgradeOpen(); pMaterial->setName(sName);
      //  //  pMatDict->upgradeOpen(); pMatDict->setAt(sName, pMaterial);
      //  //}
      //}
      //else
      //{
      //  ODA_FAIL_ONCE_X(TDWF); // test
      //}
      break;
    }
  }

  if (pSeg->_idMaterial.isNull())
  {
    OdString sName = getMaterialName(this, pSeg);
    pMaterial = createDbMaterial(pMatDict, sName, pSeg->_dataMaterial);

    pSeg->_idMaterial = _lastMaterialId = pMaterial->objectId();
    pMaterial->setAttributes(&_lastGiData);
  }
  ODA_ASSERT_ONCE_X(TDWF, !pSeg->_idMaterial.isNull() && pSeg->_idMaterial == _lastMaterialId);

  return pSeg->_idMaterial;
}

static OdCmEntityColor getGenericMaterialColor(struct DwfImporter::SegmentAttributes* pSeg)
{
  ODA_ASSERT_ONCE_X(TDWF, pSeg);

  OdCmEntityColor colorGeneric; colorGeneric.setColorMethod(OdCmEntityColor::kByLayer);
  OdDbMaterialPtr pMaterial;
  if (!pSeg->_idMaterial.isNull())
    pMaterial = OdDbMaterial::cast(pSeg->_idMaterial.openObject());
  if (pMaterial.get())
  {
    OdGiMaterialColor diffuseColor; OdGiMaterialMap diffuseMap;
    pMaterial->diffuse(diffuseColor, diffuseMap);
    OdCmEntityColor color = diffuseColor.color();
    if (color.isByColor())
    {
      OdUInt8 red = color.red(),
              green = color.green(),
              blue = color.blue();
      // Generic Material Properties/Color affects 
      // the diffuse reflection as well as the transmittance of light in the material.
      colorGeneric.setRGB(red, green, blue);
      return colorGeneric;
    }
  }
  ODA_ASSERT_ONCE_X(TDWF, colorGeneric.isByLayer());
  return colorGeneric;
}

OdResult DwfImporter::add3dObject(OdDbEntity* ent, TKO_Geometry_Bits mask)
{
  OdDbBlockTableRecordPtr block = _currentSegment->_block.safeOpenObject(OdDb::kForWrite);
  _currentSegment->objects.append(block->appendOdDbEntity(ent));

  // support material proceeding
  OdDbObjectId idMaterial = checkMaterialId();
  if (!idMaterial.isNull())
    ent->setMaterial(idMaterial);

  OdCmEntityColor colorEnt;
  colorEnt.setColorMethod(OdCmEntityColor::kByLayer); ODA_ASSERT_ONCE_X(TDWF, colorEnt.isByLayer());
  if (_currentSegment->_colorGeometryMask & mask)
    colorEnt = _currentSegment->_color;
  else
    colorEnt = getGenericMaterialColor(_currentSegment);
  if (!colorEnt.isByLayer())
  {
    OdCmColor clr = toOdCmColor(colorEnt);
    if (!clr.isByLayer())
       ent->setColor(clr);
  }

  if (_currentSegment->_visibilityMask & mask)
    ent->setVisibility((_currentSegment->_visibility & mask) ? OdDb::kVisible : OdDb::kInvisible);

  return ent->transformBy(_currentSegment->_matrix);
}

// Support textures for 3D DWF
static OdGiMaterialTexturePtr getTexture(DwfImporter* pCtx, 
                                         const char* pcszName, 
                                         OdGiMaterialMap* pMap = NULL)
{
  if (!pcszName || !*pcszName || !pCtx->m_textures.size())
    return OdGiMaterialTexturePtr();
  OdString sName = pcszName;
  std::map<OdString, OdGiMaterialTexturePtr>::const_iterator pItr = pCtx->m_textures.find(sName);
  if (pItr == pCtx->m_textures.end())
  {
    int pos;
    if (sName[0] == L'\"' && (pos = sName.find(L'\"', 1)) > 0)
    {
      //pItr = pCtx->m_textures.find(sName = sName.left(++pos)); // way for ArborPress3D.dwf & Bracket3D.dwf  but with bad result
      //ODA_ASSERT_ONCE_X(TDWF, pItr != pCtx->m_textures.end());
    }
    else if (pCtx->m_textures.size() == 1)
      pItr = pCtx->m_textures.begin(); // it looks like Design Review do it for CMoren_pants_texture_wrong.dwf 
    else
    {
      ODA_FAIL_ONCE_X(TDWF); // test
    }
    if (pItr == pCtx->m_textures.end())
      return OdGiMaterialTexturePtr();
  }
  OdGiMaterialTexturePtr pTexture = pItr->second;
  // CORE-10297 CORE-13187
  // ACAD does not support axis mirror transformation (-Y) on in texture map
  // therefore the next was move toOdDbSubDMesh (or into image transformation (see DwfImporter::loadW3d))
  //if (pMap)
  //{
  //  OdGeMatrix3d& matrix = pMap->mapper().transform();
  //  if (matrix == OdGeMatrix3d::kIdentity)
  //  {
  //    matrix.setCoordSystem(OdGePoint3d::kOrigin, //OdGePoint3d(0.0, 1.0, 0.0), //OdGePoint3d(0.0, - 1.0, 0.0),  //possible TODO by Andrew
  //                          OdGeVector3d::kXAxis, -OdGeVector3d::kYAxis, OdGeVector3d::kZAxis);
  //    // or test
  //    //matrix.setToRotation(OdaPI, OdGeVector3d::kZAxis, OdGePoint3d::kOrigin); 
  //    //matrix.postMultBy(OdGeMatrix3d::rotation(OdaPI / 20, OdGeVector3d::kZAxis));
  //    //matrix.postMultBy(OdGeMatrix3d::scaling(OdGeScale3d(1.2, 1.0, 1.0)));
  //    //matrix.postMultBy(OdGeMatrix3d::translation(OdGeVector3d(1.0, 1.0, 0.0) * 0.5));
  //  }
  //}

  return pTexture;
}

DECLARE_OPCODE_CLASS(DD_TK_Color,TK_Color);

TK_Status DD_TK_Color::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Color::Execute(tk);
  struct DwfImporter::SegmentAttributes* pSeg = m_pImp->_currentSegment;
  ODA_ASSERT_ONCE_X(TDWF, pSeg->_idMaterial.isNull());

  int channels = GetChannels();
  if (channels & (1 << TKO_Channel_Diffuse))
  {
    OdGiMaterialColor diffuseColor; OdGiMaterialMap diffuseMap;
    pSeg->_dataMaterial.diffuse(diffuseColor, diffuseMap);
    OdCmEntityColor color = toOdCmEntityColor(GetDiffuse());
    diffuseColor.setColor(color);
    diffuseColor.setMethod(OdGiMaterialColor::kOverride);
    OdGiMaterialTexturePtr pTexture = getTexture(m_pImp, GetDiffuseName(), &diffuseMap);
    if (pTexture.get())
    {
      diffuseMap.setTexture(pTexture);
      if (OdGiImageFileTexture* pImageFileTexture = OdGiImageFileTexture::cast(pTexture).get())
        diffuseMap.setSourceFileName(pImageFileTexture->sourceFileName());
    }
    pSeg->_dataMaterial.setDiffuse(diffuseColor, diffuseMap);
  }
  if (channels & (1 << TKO_Channel_Specular))
  {
    OdGiMaterialColor specularColor; OdGiMaterialMap specularMap; double glossFactor = 0;
    pSeg->_dataMaterial.specular(specularColor, specularMap, glossFactor);
    OdCmEntityColor color = toOdCmEntityColor(GetSpecular()); // m_specular.m_rgb
    specularColor.setColor(color);
    specularColor.setMethod(OdGiMaterialColor::kOverride);
    OdGiMaterialTexturePtr pTexture = getTexture(m_pImp, GetSpecularName(), &specularMap); // m_specular.m_name
    if (pTexture.get())
    {
      specularMap.setTexture(pTexture);
      if (OdGiImageFileTexture* pImageFileTexture = OdGiImageFileTexture::cast(pTexture).get())
        specularMap.setSourceFileName(pImageFileTexture->sourceFileName());
    }
    pSeg->_dataMaterial.setSpecular(specularColor, specularMap, glossFactor);
  }
  if (   (channels & (1 << TKO_Channel_Mirror))
      && (GetMirror()[0] > 0. || GetMirror()[1] > 0. || GetMirror()[2] > 0.))
  {
    ODA_ASSERT_ONCE_X(TDWF,    OdEqual(GetMirror()[0], GetMirror()[1]) 
                            && OdEqual(GetMirror()[0], GetMirror()[2])); // TODO
    double refl = (double) GetMirror()[0];
    ODA_ASSERT_ONCE_X(TDWF, refl <= 1.0);
    pSeg->_dataMaterial.setReflectivity(refl);
  }
  if (   (channels & (1 << TKO_Channel_Transmission))
      && (GetTransmission()[0] > 0. || GetTransmission()[1] > 0. || GetTransmission()[2] > 0.))
  {
    double opacity; OdGiMaterialMap opacityMap;
    pSeg->_dataMaterial.opacity(opacity, opacityMap);
    ODA_ASSERT_ONCE_X(TDWF, opacity >= 0. && opacity <= 1.);
    double trn = (GetTransmission()[0] + GetTransmission()[1] + GetTransmission()[2]) / 3.0; // m_transmission.m_rgb
    ODA_ASSERT_ONCE_X(TDWF, trn <= 1.);
    OdGiMaterialTexturePtr pTexture = getTexture(m_pImp, GetTransmissionName(), &opacityMap);
    if (pTexture.get())
    {
      opacityMap.setTexture(pTexture);
      if (OdGiImageFileTexture* pImageFileTexture = OdGiImageFileTexture::cast(pTexture).get())
        opacityMap.setSourceFileName(pImageFileTexture->sourceFileName());
    }
    pSeg->_dataMaterial.setOpacity(1.0 - trn, opacityMap);
//#if 1
    OdGiMaterialColor ambientColor;
    pSeg->_dataMaterial.ambient(ambientColor);
    OdCmEntityColor color = toOdCmEntityColor(GetTransmission());
    ambientColor.setColor(color);
    ambientColor.setMethod(OdGiMaterialColor::kOverride);
    pSeg->_dataMaterial.setAmbient(ambientColor);
//#endif
  }
  if (   (channels & (1 << TKO_Channel_Emission))
      && (GetEmission()[0] > 0. || GetEmission()[1] > 0. || GetEmission()[2] > 0.)) //core-10376
  {
    OdGiMaterialColor emissionColor; OdGiMaterialMap emissionMap;
    pSeg->_dataMaterial.emission(emissionColor, emissionMap);
    OdCmEntityColor color = toOdCmEntityColor(GetEmission());
    emissionColor.setColor(color);
    emissionColor.setMethod(OdGiMaterialColor::kOverride);
    OdGiMaterialTexturePtr pTexture = getTexture(m_pImp, GetEmissionName(), &emissionMap);
    if (pTexture.get())
    {
      emissionMap.setTexture(pTexture);
      if (OdGiImageFileTexture* pImageFileTexture = OdGiImageFileTexture::cast(pTexture).get())
        emissionMap.setSourceFileName(pImageFileTexture->sourceFileName());
    }
    pSeg->_dataMaterial.setEmission(emissionColor, emissionMap);
  }
  if (channels & (1 << TKO_Channel_Gloss))
  {
    OdGiMaterialColor specularColor; OdGiMaterialMap specularMap; double glossFactor;
    pSeg->_dataMaterial.specular(specularColor, specularMap, glossFactor);
    glossFactor = GetGloss();
    pSeg->_dataMaterial.setSpecular(specularColor, specularMap, glossFactor);
  }
  if (channels & (1 << TKO_Channel_Index))
  {
    double refractionIndex; OdGiMaterialMap refractionMap;
    pSeg->_dataMaterial.refraction(refractionIndex, refractionMap);
    refractionIndex = GetIndex();
    pSeg->_dataMaterial.refraction(refractionIndex, refractionMap);
  }
  if (channels & (1 << TKO_Channel_Bump))
  {
    OdGiMaterialTexturePtr pTexture = getTexture(m_pImp, GetBumpName());
    if (pTexture.get())
    {
      ODA_FAIL_ONCE_X(TDWF); // test
      OdGiMaterialMap bumpMap;
      pSeg->_dataMaterial.bump(bumpMap);
      bumpMap.setTexture(pTexture);
      if (OdGiImageFileTexture* pImageFileTexture = OdGiImageFileTexture::cast(pTexture).get())
        bumpMap.setSourceFileName(pImageFileTexture->sourceFileName());
      pSeg->_dataMaterial.setBump(bumpMap);
    }
  }
  if (channels & (1 << TKO_Channel_Environment))
  {
    ODA_ASSERT_VAR(res = res;) // brk
    // TODO //GetEnvironmentName()
  }
  if (channels & (1 << TKO_Channel_Extended))
  {
    ODA_ASSERT_VAR(res = res;) // brk
  }

  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Color_RGB,TK_Color_RGB);

TK_Status DD_TK_Color_RGB::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Color_RGB::Execute(tk);
  m_pImp->_currentSegment->_color = toOdCmEntityColor(GetRGB()); // , 0);
  m_pImp->_currentSegment->_colorGeometryMask |= GetGeometry();
  //OdString s;
  //s.format(OD_T("%d,%d,%d"),OdUInt32(GetRGB()[0]*255), OdUInt32(GetRGB()[1]*255), OdUInt32(GetRGB()[2]*255));
  //tk.LogEntry(s);
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Text_Font,TK_Text_Font);

TK_Status DD_TK_Text_Font::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Text_Font::Execute(tk);
  m_pImp->fonts().setFontStyle(*this);
  if (GetMask() & TKO_Font_Size)
  {
    m_pImp->_currentSegment->font_size = GetSize();
  }
  return res;
}


DECLARE_OPCODE_CLASS_2(DD_TK_Text,TK_Text);

TK_Status DD_TK_Text::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Text::Execute(tk);
  OdDbTextPtr text = OdDbText::createObject();
  if (m_pImp->_currentSegment->font_size != 0)
    text->setHeight(m_pImp->_currentSegment->font_size);
  text->setPosition(OdGePoint3d(GetPosition()[0], GetPosition()[1], GetPosition()[2]));
  if (GetEncoding() == TKO_Enc_Unicode || GetEncoding() == TKO_Enc_Raw_16)
  {
    text->setTextString(OdString((const wchar_t*)GetString()));
  }
  else
  {
    text->setTextString(OdString(GetString()));
  }
  m_pImp->add3dObject(text, TKO_Geo_Text);
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Matrix,TK_Matrix);

TK_Status DD_TK_Matrix::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Matrix::Execute(tk);

  if (Opcode() == TKE_Modelling_Matrix)
  {
    OdGeMatrix3d m;
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        m(j,i) = GetMatrix()[i*4+j];
      }
    }
    m_pImp->_currentSegment->_matrix *= m;
    for (OdDbObjectIdArray::iterator it = m_pImp->_currentSegment->objects.begin(); it != m_pImp->_currentSegment->objects.end(); ++it)
    {
      OdDbEntityPtr ent = it->safeOpenObject(OdDb::kForWrite);
      ent->transformBy(m_pImp->_currentSegment->_matrix);
    }
  }
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Open_Segment,TK_Open_Segment);

TK_Status DD_TK_Open_Segment::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Open_Segment::Execute(tk);
  tk.LogEntry(GetSegment());
  OdDbBlockTablePtr bt = m_pImp->database()->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbBlockTableRecordPtr btr = OdDbBlockTableRecord::createObject();
  OdString segName(GetSegment());
  segName.replace(L'/',L'_');
  segName.replace(L'\\',L'_');
  segName.replace(L'?',L'_');
  OdDbObjectId btrId;
  if (segName.isEmpty())
    segName = L"*U";
  else // CORE-12670 DWFx import fails
  {
    btrId = bt->getAt(segName);
    ODA_ASSERT_ONCE_X(TDWF, btrId.isNull() || segName.left(15) == L"_Style Library_");
  }
  btr->setName(segName);
  if (btrId.isNull()) // CORE-12670 DWFx import fails
    btrId = bt->add(btr);
  if (GetSegment() && *GetSegment())
  {
    m_pImp->_segments[OdString(GetSegment())] = DwfImporter::SegmentAttributes(OdString(GetSegment()), m_pImp->_currentSegment);
    m_pImp->_currentSegment = &m_pImp->_segments[OdString(GetSegment())];
  }
  else
  {
    m_pImp->_segments[btr->getName()] = DwfImporter::SegmentAttributes( OdString(GetSegment()), m_pImp->_currentSegment);
    m_pImp->_currentSegment = &m_pImp->_segments[btr->getName()];
  }
  m_pImp->_currentSegment->_block = btrId;
  if (segName.left(17) != L"_Include Library_" && segName.left(15) != L"_Style Library_")
  {
    OdDbBlockTableRecordPtr ms;
    if (m_pImp->_currentSegment->_parent == 0)
    {
      ms = m_pImp->database()->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    }
    else
    {
      ms = m_pImp->_currentSegment->_parent->_block.safeOpenObject(OdDb::kForWrite);
    }
    OdDbBlockReferencePtr ref = OdDbBlockReference::createObject();
    ref->setBlockTableRecord(m_pImp->_currentSegment->_block);
    if (m_pImp->_currentSegment->_parent == 0)
    {
      OdGeMatrix3d w3DTransform; // 3D DWF is [Front] -> [Top] transformed (cf. #9714)
      w3DTransform.setCoordSystem(OdGePoint3d::kOrigin, OdGeVector3d::kXAxis, OdGeVector3d::kZAxis, -OdGeVector3d::kYAxis);
      ref->setBlockTransform(w3DTransform);
    }
    ms->appendOdDbEntity(ref);
  }
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Reopen_Segment,TK_Reopen_Segment);

TK_Status DD_TK_Reopen_Segment::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Reopen_Segment::Execute(tk);
  ODA_FAIL_ONCE();
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Close_Segment,TK_Close_Segment);

TK_Status DD_TK_Close_Segment::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Close_Segment::Execute(tk);
  if (m_pImp->_currentSegment->_anonymousSegments)
    --m_pImp->_currentSegment->_anonymousSegments;
  else
    m_pImp->_currentSegment = m_pImp->_currentSegment->_parent;
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Referenced_Segment,TK_Referenced_Segment);

void addExplodedTransformed(OdDbBlockTableRecord* block, OdDbEntity* ref, const OdGeMatrix3d& m)
{
  OdDbObjectIdArray ids;
  ref->explodeToBlock( block, &ids);
  for (unsigned i = 0; i < ids.size(); i++)
  {
    OdDbEntityPtr e(ids[i].safeOpenObject(OdDb::kForWrite));
    if (e->transformBy(m) != eOk)
    {
      addExplodedTransformed(block, e, m);
    }
  }
  ref->erase();
}

TK_Status DD_TK_Referenced_Segment::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Referenced_Segment::Execute(tk);
  tk.LogEntry(GetSegment());
  std::map<OdString,DwfImporter::SegmentAttributes>::iterator it = m_pImp->_segments.find(OdString(GetSegment()));
  if (it == m_pImp->_segments.end())
  {
    ODA_FAIL_ONCE();
  }
  else
  {
    OdDbBlockReferencePtr ref = OdDbBlockReference::createObject();
    ref->setBlockTableRecord(it->second._block);
    if (m_pImp->add3dObject(ref,TKO_Geometry_Bits(0))!=eOk)
    {
      OdDbBlockTableRecordPtr block = m_pImp->_currentSegment->_block.safeOpenObject(OdDb::kForWrite);
      addExplodedTransformed(block, ref, m_pImp->_currentSegment->_matrix);
    }
  }
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Camera,TK_Camera);

TK_Status DD_TK_Camera::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Camera::Execute(tk);
  float const * tgTemp = GetTarget();
  float const * posTemp = GetPosition();
  OdGePoint3d target(tgTemp[0], tgTemp[1], tgTemp[2]);
  OdGePoint3d position(posTemp[0], posTemp[1], posTemp[2]);
  if (Opcode()== TKE_View)
  {
    OdGeVector3d vtUpVector(GetUpVector()[0], GetUpVector()[1], GetUpVector()[2]);

    OdGeMatrix3d w3DTransform; // 3D DWF is [Front] -> [Top] transformed (cf. #9714)
    w3DTransform.setCoordSystem(OdGePoint3d::kOrigin, OdGeVector3d::kXAxis, OdGeVector3d::kZAxis, -OdGeVector3d::kYAxis);
    target.transformBy(w3DTransform);
    position.transformBy(w3DTransform); 
    vtUpVector.transformBy(w3DTransform);
    OdGeVector3d dirVector = (target - position);

    OdGeError flag;
    vtUpVector.normalize(OdGeContext::gTol, flag);
    ODA_ASSERT_ONCE_X(TDWF, flag == OdGe::kOk);
    dirVector.normalize(OdGeContext::gTol, flag);
    ODA_ASSERT_ONCE_X(TDWF, flag == OdGe::kOk);

    double withField = GetField()[0],
           heightField = GetField()[1];
    ODA_ASSERT_ONCE_X(TDWF, withField > 0.0 && heightField > 0.0);
    if (OdEqual(withField, heightField, 1e-5))
      withField *= 1.8;

    OdDbViewportTableRecordPtr vp = OdDbViewportTablePtr(m_pImp->database()->getViewportTableId().safeOpenObject())->getActiveViewportId().openObject(OdDb::kForWrite);
    OdAbstractViewPEPtr(vp)->setView(vp, target, -dirVector, vtUpVector, withField, heightField, GetProjection() == W3DCamera::ePerspective);
  }
  else
  {
    OdDbViewTablePtr pViews = m_pImp->database()->getViewTableId().safeOpenObject(OdDb::kForWrite);
    OdDbViewTableRecordPtr view;
    OdString viewName(GetView());
    if (pViews->has(viewName))
    {
      view = pViews->getAt(viewName).safeOpenObject(OdDb::kForWrite);
    }
    else
    {
      view = OdDbViewTableRecord::createObject();
      view->setName(viewName);
      pViews->add(view);
    }
    OdAbstractViewPEPtr(view)->setView(view, target, position - target,
      OdGeVector3d(GetUpVector()[0], GetUpVector()[1], GetUpVector()[2]),
      GetField()[0], GetField()[1], GetProjection() == W3DCamera::ePerspective);
  }
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_File_Info,TK_File_Info);

TK_Status DD_TK_File_Info::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_File_Info::Execute(tk);
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Instance,TK_Instance);

TK_Status DD_TK_Instance::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Instance::Execute(tk);
  ODA_FAIL_ONCE();
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Clip_Rectangle,TK_Clip_Rectangle);

TK_Status DD_TK_Clip_Rectangle::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Clip_Rectangle::Execute(tk);
  ODA_FAIL_ONCE();
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Clip_Region,TK_Clip_Region);

TK_Status DD_TK_Clip_Region::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Clip_Region::Execute(tk);
  ODA_FAIL_ONCE();
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Enumerated,TK_Enumerated);

TK_Status DD_TK_Enumerated::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Enumerated::Execute(tk);
  if (Opcode() == TKE_Text_Alignment)
  {
  }
  else
  {
    ODA_FAIL_ONCE();
  }
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Size,TK_Size);

TK_Status DD_TK_Size::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Size::Execute(tk);
  switch (Opcode())
  {
  case TKE_Line_Weight:
    break;
  case TKE_Edge_Weight:
    break;
  case TKE_Marker_Size:
    break;
  case TKE_Text_Spacing:
    break;
  }
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Circle,TK_Circle);

TK_Status DD_TK_Circle::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Circle::Execute(tk);
  OdGeCircArc3d arc;
  OdGeError err;
  arc.set(OdGePoint3d(GetStart()[0], GetStart()[1], GetStart()[2]),
    OdGePoint3d(GetMiddle()[0], GetMiddle()[1], GetMiddle()[2]),
    OdGePoint3d(GetEnd()[0], GetEnd()[1], GetEnd()[2]), err);
  if (err != OdGe::kOk)
  {
    OdDbLinePtr l = OdDbLine::createObject();
    l->setStartPoint(OdGePoint3d(GetStart()[0], GetStart()[1], GetStart()[2]));
    l->setEndPoint(OdGePoint3d(GetEnd()[0], GetEnd()[1], GetEnd()[2]));
    m_pImp->add3dObject(l, TKO_Geo_Line);
  }
  else
  {
    switch (Opcode())
    {
    case TKE_Circle:
      {
        OdDbCirclePtr c = OdDbCircle::createObject();
        c->setCenter(arc.center());
        c->setNormal(arc.normal());
        c->setRadius(arc.radius());
        m_pImp->add3dObject(c, TKO_Geo_Line);
      }
      break;
    case TKE_Circular_Arc:
      {
        OdGeVector3d ref( arc.refVec());
        ref.transformBy(OdGeMatrix3d::worldToPlane(arc.normal()));
        OdDbArcPtr a = OdDbArc::createObject();
        a->setCenter(arc.center());
        a->setNormal(arc.normal());
        a->setRadius(arc.radius());
        a->setStartAngle( OD_ATAN2(ref.y,ref.x) );
        a->setEndAngle( arc.endAng() + a->startAngle() );
        m_pImp->add3dObject(a, TKO_Geo_Line);
      }
      break;
    case TKE_Circular_Chord:
      ODA_FAIL_ONCE();
      break;
    case TKE_Circular_Wedge:
      ODA_FAIL_ONCE();
      break;
    }
  }
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Polypoint,TK_Polypoint);

TK_Status DD_TK_Polypoint::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Polypoint::Execute(tk);
  OdDb3dPolylinePtr p = OdDb3dPolyline::createObject();
  for (int i = 0; i < GetCount(); ++i)
  {
    OdDb3dPolylineVertexPtr v = OdDb3dPolylineVertex::createObject();
    v->setPosition(OdGePoint3d(GetPoints()[i*3],GetPoints()[i*3+1],GetPoints()[i*3+2]));
    p->appendVertex(v);
  }
  m_pImp->add3dObject(p, TKO_Geo_Line);
  if (Opcode() == TKE_Polygon)
  {
    if (GetCount() < 5)
    {
      OdDbSolidPtr s = OdDbSolid::createObject();
      for (int i = 0; i < GetCount(); ++i)
      {
        s->setPointAt(i,OdGePoint3d(GetPoints()[i*3],GetPoints()[i*3+1],GetPoints()[i*3+2]));
      }
      m_pImp->add3dObject(s, TKO_Geo_Face);
    }
    else
    {
      ODA_FAIL_ONCE();
    }
  }
  return res;
}

DECLARE_OPCODE_CLASS_2(DD_TK_Ellipse,TK_Ellipse);

TK_Status DD_TK_Ellipse::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Ellipse::Execute(tk);
  OdDbEllipsePtr ell = OdDbEllipse::createObject();
  OdGePoint3d center( GetCenter()[0], GetCenter()[1], GetCenter()[2]);
  OdGeVector3d major(GetMajor()[0], GetMajor()[1], GetMajor()[2]);
  OdGeVector3d minor(GetMinor()[0], GetMinor()[1], GetMinor()[2]);
  OdGeVector3d normal(major.crossProduct(minor).normal());
  double startAngle = GetLimits()[0]*Oda2PI;
  double endAngle = GetLimits()[1]*Oda2PI;
  ell->set(center, normal, major, odmin(1.0,minor.length()/major.length()), startAngle, endAngle);
  if (Opcode() != TKE_Elliptical_Arc)
  {
    m_pImp->add3dObject(ell, TKO_Geo_Line);
    OdDbHatchPtr h = OdDbHatch::createObject();
    h->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
    h->setNormal(normal);
    h->setAssociative(true);
    OdDbObjectIdArray loop;
    loop.append(ell->objectId());
    h->appendLoop(OdDbHatch::kOutermost, loop);
    m_pImp->add3dObject(h, TKO_Geo_Face);
  }
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_NURBS_Curve,TK_NURBS_Curve);

TK_Status DD_TK_NURBS_Curve::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_NURBS_Curve::Execute(tk);
  OdDbSplinePtr spline = OdDbSpline::createObject();
  OdGePoint3dArray ctl;
  ctl.resize(m_control_point_count);
  for(int i = 0; i < m_control_point_count; i++)
    ctl[i] = OdGePoint3d(m_control_points[i*3], m_control_points[i*3+1], m_control_points[i*3+2]);
  OdGeDoubleArray wts;
  if (GetOptions()& NC_HAS_WEIGHTS)
  {
    wts.resize(m_control_point_count);
    std::copy(m_weights, m_weights + wts.size(), &wts[0]);
  }
  OdGeKnotVector knots;
  knots.setLogicalLength(m_control_point_count + m_degree + 1);
  std::copy(m_knots, m_knots + knots.length(), &knots[0]);
  try 
  {
    spline->setNurbsData(GetDegree(), GetOptions()& NC_HAS_WEIGHTS, false, false, ctl, knots, wts, OdGeContext::gTol.equalPoint());
    m_pImp->add3dObject(spline, TKO_Geo_Line);
  }
  catch (const OdError&) // just skip invalid curves
  {
  }
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_Visibility,TK_Visibility);

TK_Status DD_TK_Visibility::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_Visibility::Execute(tk);
  m_pImp->_currentSegment->_visibilityMask = GetGeometry();
  m_pImp->_currentSegment->_visibility = GetValue();
  return res;
}

DECLARE_OPCODE_CLASS(DD_TK_NURBS_Surface,TK_NURBS_Surface);

TK_Status DD_TK_NURBS_Surface::Execute(BStreamFileToolkit & tk)
{
  TK_Status res = TK_NURBS_Surface::Execute(tk);
  ODA_FAIL_ONCE();
  return res;
}

DwfImporter::SegmentAttributes::SegmentAttributes(const OdChar* name, SegmentAttributes* parent)
: _parent(parent), font_size(0), _colorGeometryMask(0),_visibilityMask(0),_visibility(-1),_name(name),_anonymousSegments(0)
{
  if (parent)
  {
    *this = *parent;
    _name = name;
    _parent = parent;
    font_size = parent->font_size;
    _matrix = parent->_matrix;
    _dataMaterial = parent->_dataMaterial;
    _idMaterial = parent->_idMaterial;
    _color = parent->_color;
    _colorGeometryMask = parent->_colorGeometryMask;
    _visibilityMask = parent->_visibilityMask;
    _visibility = parent->_visibility;
  }
}


// put here for compatibility reasons (using old toolkit)
void DwfFontManager::setFontStyle(TK_Text_Font& font)
{
  WT_Font wtFont;
  if (font.GetMask() & TKO_Font_Names)
  {
    wtFont.font_name().set(font.GetNames());
  }
  if (font.GetMask() & TKO_Font_Rotation)
  {
    wtFont.rotation() = OdUInt16(font.GetRotation()/360*(2<<15));
  }
  if (font.GetMask() & TKO_Font_Slant)
  {
  }
  if (font.GetMask() & TKO_Font_Width_Scale)
  {
    wtFont.width_scale() = OdUInt16(font.GetWidthScale()*1024);
  }
  if (font.GetMask() & TKO_Font_Extra_Space)
  {
    ODA_FAIL_ONCE();
    wtFont.spacing() = OdUInt16((1 + font.GetExtraSpace()*1024));
  }
  if (font.GetMask() & TKO_Font_Underline)
  {
    ODA_FAIL_ONCE();
    wtFont.flags().set(wtFont.flags() | FLAG_UNDERSCORE);
  }
  if (font.GetMask() & TKO_Font_Outline)
  {
    ODA_FAIL_ONCE();
    wtFont.flags().set(wtFont.flags() | FLAG_OUTLINE_TEXT);
  }
  if (font.GetMask() & TKO_Font_Overline)
  {
    ODA_FAIL_ONCE();
    wtFont.flags().set(wtFont.flags() | FLAG_OVERSCORE);
  }
  setFontStyle(wtFont);
}

using namespace DWFToolkit;

static OdStreamBufPtr toOdStreamBuf(DWFInputStream* pFrom)
{
  ODA_ASSERT_ONCE_X(TDWF, pFrom && pFrom->available() > 0);
  OdStreamBufPtr pStreamBuf = OdMemoryStream::createNew();
  size_t numBytes = 0;
  char aBuffer[16384] = { 0 };
  while (pFrom->available() > 0)
  {
    numBytes = pFrom->read(aBuffer, 16384);
    ODA_ASSERT(numBytes < INT_MAX);
    pStreamBuf->putBytes(aBuffer, (OdUInt32)numBytes);
  }
  pStreamBuf->rewind();
  return pStreamBuf;
}

OdDwfImport::ImportResult DwfImporter::loadW3d(DWFEModelSection* pSection, DWFImportProgressMeter& pm, int layoutCount)
{
  if (!pSection)
  {
    ODA_FAIL_ONCE();
    return OdDwfImport::bad_file;
  }
  pSection->readDescriptor();

  //
  // get the graphics stream
  //
  DWFResourceContainer::ResourceIterator* piResources = pSection->findResourcesByRole( DWFXML::kzRole_Graphics3d );

  if ((piResources == NULL) || (piResources->valid() == false))
  {
    if (piResources)
    {
      DWFCORE_FREE_OBJECT( piResources );
    }
    ODA_FAIL_ONCE();
    return OdDwfImport::bad_file;
  }
  
  DWFGraphicResource* pW3D = dynamic_cast<DWFGraphicResource*>(piResources->get());
  DWFCORE_FREE_OBJECT( piResources );
  if (pW3D == NULL)
    return OdDwfImport::bad_file;
  if (pW3D->mime() == DWFCore::DWFMIME::kzMIMEType_W3D_S)
    return OdDwfImport::encrypted_file;

  OdRxRasterServicesPtr pRxSrv = ::odrxDynamicLinker()->loadModule(OdExRasterModuleName);
  if (pRxSrv.get())
  {
    // get textures streams (see also CORE-12892)
    // https://forums.autodesk.com/t5/dwf/reading-textures/td-p/1785569 
    piResources = pSection->findResourcesByRole(DWFXML::kzRole_Texture);
    for (; piResources && piResources->valid(); piResources->next())
    {
      DWFGraphicResource* pTxtr = dynamic_cast<DWFGraphicResource*>(piResources->get());
      if (!pTxtr)
        continue;
      DWFInputStream* pTxtrStrm = pTxtr->getInputStream();
      OdUInt32 iSizeReq = (OdUInt32)pTxtrStrm->available();
      OdString sName = (const wchar_t*)pTxtr->title();
      if (iSizeReq && !sName.isEmpty() && m_textures.find(sName) == m_textures.end())
      {
        OdStreamBufPtr pStreamBuf = toOdStreamBuf(pTxtrStrm);
#      if 0
        // useless for import into DWG
        OdGiRasterImageTexturePtr pTexture = OdGiRasterImageTexture::createObject();
        pTexture->setRasterImage(pRxSrv->loadRasterImage(pStreamBuf));
        m_textures[sName] = pTexture;
#      else
        OdString sExt = pRxSrv->mapTypeToExtension(pRxSrv->getImageFormat(pStreamBuf), NULL);
        if (sExt.isEmpty())
          sExt = L".jpg";
        else if (sExt.getAt(0) != L'.')
          sExt = L"." + sExt;

        OdGiImageFileTexturePtr pTexture = OdGiImageFileTexture::createObject();
        m_textures[sName] = pTexture;
        // way for DwfImport
        OdString sPath = database()->getFilename();
        bool bNameOnly = sPath.isEmpty();
        if (bNameOnly)
        {
          OdRxVariantValue rxPath = (OdRxVariantValue) properties()->getAt(L"DwfPath");
          ODA_ASSERT_ONCE_X(TDWF, !rxPath.isNull());
          sPath = rxPath->getString();
        }
        sPath.replace(L'\\', L'/');
        int pos;
        if ((pos = sPath.reverseFind(L'.')) > 0)
          sPath = sPath.left(pos);
        sName.remove(L'\"'); sName.remove(L'\'');
        sPath += L"_" + sName + sExt;
        
        // save
        OdStreamBufPtr pOut = ::odrxSystemServices()->createFile(sPath, Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
        pStreamBuf->rewind(); pStreamBuf->copyDataTo(pOut);
        //
        // ACAD does not support axis mirror transformation (-Y) on in texture map
        // therefore the transformation in getTexture was move here ( or into texture coordinate transformation in toOdDbSubDMesh)
        //OdGiRasterImagePtr pImage = pRxSrv->loadRasterImage(pStreamBuf);
        //OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
        //pDesc->setPixelWidth(pImage->pixelWidth());
        //pDesc->setPixelHeight(pImage->pixelHeight());
        //pDesc->setColorDepth(pImage->colorDepth());
        //pDesc->pixelFormat() = pImage->pixelFormat();
        //pImage = pImage->convert(true, 50.0, 50.0, 0.0, 0, false,
        //                         true, // flipY    (-Y)
        //                         false, pDesc);
        //pRxSrv->saveRasterImage(pImage, sPath);
        //

        if (bNameOnly && (pos = sPath.reverseFind(L'/')) > 0)
          sPath = sPath.mid(++pos);
        pTexture->setSourceFileName(sPath);
#      endif
      }
      DWFCORE_FREE_OBJECT(pTxtrStrm);
    }
    if (piResources)
    {
      DWFCORE_FREE_OBJECT(piResources);
    }
  }

  //
  // get the data stream
  //
  DWFInputStream* pW3DStream = pW3D->getInputStream();

  //
  // Create the HSF toolkit object that does the stream I/O
  //
  BStreamFileToolkit oW3DStreamParser;
#ifdef _DEBUG_DUMP
  oW3DStreamParser.SetLogFile("C:\\w3d.log");
  oW3DStreamParser.SetLogging(true);
  oW3DStreamParser.SetLoggingOptions(3);
#endif

  //oW3DStreamParser.SetOpcodeHandler (TKE_Font,                 new TK_Font);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Texture,              new TK_Texture);
  oW3DStreamParser.SetOpcodeHandler (TKE_File_Info,            new DD_TK_File_Info(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Renumber_Key_Global,  new TK_Renumber (TKE_Renumber_Key_Global));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Renumber_Key_Local,   new TK_Renumber (TKE_Renumber_Key_Local));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Dictionary,           new TK_Dictionary);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Start_User_Data,      new TK_User_Data);
  //oW3DStreamParser.SetOpcodeHandler (TKE_XML,                  new TK_XML);
  //oW3DStreamParser.SetOpcodeHandler (TKE_External_Reference,   new TK_External_Reference);
  //oW3DStreamParser.SetOpcodeHandler (TKE_URL,                  new TK_URL);
  oW3DStreamParser.SetOpcodeHandler (TKE_Open_Segment,         new DD_TK_Open_Segment(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_Close_Segment,        new DD_TK_Close_Segment(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_Reopen_Segment,       new DD_TK_Reopen_Segment(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_Include_Segment,      new DD_TK_Referenced_Segment(this, TKE_Include_Segment));
  oW3DStreamParser.SetOpcodeHandler (TKE_Style_Segment,        new DD_TK_Referenced_Segment(this, TKE_Style_Segment));
  oW3DStreamParser.SetOpcodeHandler (TKE_Repeat_Object,        new DD_TK_Instance(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Bounding,             new TK_Bounding (TKE_Bounding));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Bounding_Info,        new TK_Bounding (TKE_Bounding_Info));
  oW3DStreamParser.SetOpcodeHandler (TKE_Clip_Rectangle,       new DD_TK_Clip_Rectangle(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_Clip_Region,          new DD_TK_Clip_Region(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Callback,             new TK_Callback);
  oW3DStreamParser.SetOpcodeHandler (TKE_Camera,               new DD_TK_Camera(this,TKE_Camera));
  oW3DStreamParser.SetOpcodeHandler (TKE_View,                 new DD_TK_Camera(this,TKE_View));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Window,               new TK_Window);
  oW3DStreamParser.SetOpcodeHandler (TKE_Visibility,           new DD_TK_Visibility(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Selectability,        new TK_Selectability);
  oW3DStreamParser.SetOpcodeHandler (TKE_Color,                new DD_TK_Color(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_Color_RGB,            new DD_TK_Color_RGB(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Color_By_Value,       new TK_Color_By_Value);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Color_By_Index,       new TK_Color_By_Index (TKE_Color_By_Index));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Color_By_Index_16,    new TK_Color_By_Index (TKE_Color_By_Index_16));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Color_By_FIndex,      new TK_Color_By_FIndex);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Color_Map,            new TK_Color_Map);
  oW3DStreamParser.SetOpcodeHandler (TKE_Modelling_Matrix,     new DD_TK_Matrix(this, TKE_Modelling_Matrix));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Texture_Matrix,       new TK_Matrix (TKE_Texture_Matrix));
  oW3DStreamParser.SetOpcodeHandler (TKE_Face_Pattern,         new DD_TK_Enumerated (this, TKE_Face_Pattern));
  oW3DStreamParser.SetOpcodeHandler (TKE_Window_Pattern,       new DD_TK_Enumerated (this, TKE_Window_Pattern));
  oW3DStreamParser.SetOpcodeHandler (TKE_Marker_Symbol,        new DD_TK_Enumerated (this, TKE_Marker_Symbol));
  oW3DStreamParser.SetOpcodeHandler (TKE_Text_Alignment,       new DD_TK_Enumerated (this, TKE_Text_Alignment));
  oW3DStreamParser.SetOpcodeHandler (TKE_Window_Frame,         new DD_TK_Enumerated (this, TKE_Window_Frame));
  oW3DStreamParser.SetOpcodeHandler (TKE_Handedness,           new DD_TK_Enumerated (this, TKE_Handedness));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Edge_Pattern,         new TK_Linear_Pattern (TKE_Edge_Pattern));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Line_Pattern,         new TK_Linear_Pattern (TKE_Line_Pattern));
  oW3DStreamParser.SetOpcodeHandler (TKE_Edge_Weight,          new DD_TK_Size (this, TKE_Edge_Weight));
  oW3DStreamParser.SetOpcodeHandler (TKE_Line_Weight,          new DD_TK_Size (this, TKE_Line_Weight));
  oW3DStreamParser.SetOpcodeHandler (TKE_Marker_Size,          new DD_TK_Size (this, TKE_Marker_Size));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Rendering_Options,    new TK_Rendering_Options);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Heuristics,           new TK_Heuristics);
  oW3DStreamParser.SetOpcodeHandler (TKE_Text_Font,            new DD_TK_Text_Font(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_Text_Spacing,         new DD_TK_Size (this, TKE_Text_Spacing));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Text_Path,            new TK_Point (TKE_Text_Path));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Unicode_Options,      new TK_Unicode_Options);
  oW3DStreamParser.SetOpcodeHandler (TKE_User_Options,         new DD_TK_User_Options(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_User_Index,           new TK_User_Index);
  //oW3DStreamParser.SetOpcodeHandler (TKE_User_Value,           new TK_User_Value);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Streaming_Mode,       new TK_Streaming);
  oW3DStreamParser.SetOpcodeHandler (TKE_Circle,               new DD_TK_Circle (this, TKE_Circle));
  oW3DStreamParser.SetOpcodeHandler (TKE_Circular_Arc,         new DD_TK_Circle (this, TKE_Circular_Arc));
  oW3DStreamParser.SetOpcodeHandler (TKE_Circular_Chord,       new DD_TK_Circle (this, TKE_Circular_Chord));
  oW3DStreamParser.SetOpcodeHandler (TKE_Circular_Wedge,       new DD_TK_Circle (this, TKE_Circular_Wedge));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Cylinder,             new TK_Cylinder);
  oW3DStreamParser.SetOpcodeHandler (TKE_Ellipse,              new DD_TK_Ellipse (this, TKE_Ellipse));
  oW3DStreamParser.SetOpcodeHandler (TKE_Elliptical_Arc,       new DD_TK_Ellipse (this, TKE_Elliptical_Arc));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Grid,                 new TK_Grid);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Image,                new TK_Image);
  oW3DStreamParser.SetOpcodeHandler (TKE_Line,                 new DD_TK_Line(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_PolyCylinder,         new TK_PolyCylinder);
  oW3DStreamParser.SetOpcodeHandler (TKE_Polygon,              new DD_TK_Polypoint (this, TKE_Polygon));
  oW3DStreamParser.SetOpcodeHandler (TKE_Polyline,             new DD_TK_Polypoint (this, TKE_Polyline));
  //oW3DStreamParser.SetOpcodeHandler (TKE_PolyPolyline,         new TK_PolyPolypoint (TKE_PolyPolyline));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Marker,               new TK_Point (TKE_Marker));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Distant_Light,        new TK_Point (TKE_Distant_Light));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Local_Light,          new TK_Point (TKE_Local_Light));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Spot_Light,           new TK_Spot_Light);
  //oW3DStreamParser.SetOpcodeHandler (TKE_Area_Light,           new TK_Area_Light);
  oW3DStreamParser.SetOpcodeHandler (TKE_Shell,                new DD_TK_Shell(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_Mesh,                 new DD_TK_Mesh(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_NURBS_Curve,          new DD_TK_NURBS_Curve(this));
  oW3DStreamParser.SetOpcodeHandler (TKE_NURBS_Surface,        new DD_TK_NURBS_Surface(this));
  //oW3DStreamParser.SetOpcodeHandler (TKE_Cutting_Plane,        new TK_Cutting_Plane);
  oW3DStreamParser.SetOpcodeHandler (TKE_Text,                 new DD_TK_Text(this, TKE_Text));
  oW3DStreamParser.SetOpcodeHandler (TKE_Text_With_Encoding,   new DD_TK_Text(this, TKE_Text_With_Encoding));


#if defined(ODA_WINDOWS)
  unsigned fp_state = _control87(_DN_SAVE|_EM_DENORMAL,_MCW_DN|_EM_DENORMAL);
#endif


  DWFMonitoredInputStream pMonitor(pW3DStream, true);
  ProgressMonitor m(pW3DStream->available(), pm, 100./layoutCount);
  pMonitor.attach(&m, false);

  //
  // Attach the stream to the parser
  //
  oW3DStreamParser.OpenStream( pMonitor );


  size_t nBytesRead = 0;
  char aBuffer[16384] = {0};

  //
  // read and process the stream
  //
  while (pMonitor.available() > 0)
  {
    //
    // read from the stream ourselves, we could also use ReadBuffer()
    // but it basically just performs this same action.
    //
    nBytesRead = pMonitor.read( aBuffer, 16384 );

    //
    // use the parser to process the buffer
    //
    ODA_ASSERT(nBytesRead < INT_MAX);
    if (oW3DStreamParser.ParseBuffer(aBuffer, (int)nBytesRead, TK_Normal) == TK_Error)
      break;
  }
#if defined(ODA_WINDOWS)
  _control87(fp_state,_MCW_DN|_EM_DENORMAL);
#endif
  //
  // Done with the stream, we must delete it
  //
  oW3DStreamParser.CloseStream();
  return OdDwfImport::success;
}
}
