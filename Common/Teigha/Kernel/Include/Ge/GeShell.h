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

#ifndef _ODGESHELL_INCLUDED_
#define _ODGESHELL_INCLUDED_ /*!DOM*/

#include "TD_PackPush.h"

#include "Ge/GePoint3dArray.h"
#include "Ge/GeVector3dArray.h"
#include "DbStubPtrArray.h"
#include "Int32Array.h"
#include "UInt16Array.h"
#include "UInt8Array.h"
#include "CmEntityColorArray.h"
#include "CmTransparencyArray.h"
#include "GsMarkerArray.h"
#include "Gi/GiGeometry.h"

/** \details
    This class represents shell primitives.
    
    \remarks
    A shell is a set of faces, defined by a list of shell vertices. Shells can have regular faces and holes and disjoint regions. 
    Shell objects contain vertex data, edge data and face data. Vertex data is defined by one array that is the list of face vertices. 
    Edge data contains arrays of edge properties such as edge color, linetype, visibility etc. 
    Face data is defined by an array of shell faces and arrays of face properties such as face color, normals, transparency, material etc.
     
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>
*/
class OdGeShell
{
protected:
  int                   m_nFaceStartIndex;
public:
  OdGeShell() : m_nFaceStartIndex(-1) {}

  /** \details
    An array of vertices that are used to construct faces of the shell.
  */
  OdGePoint3dArray      vertices;

  /** \details
    An array of integer number that defines a list of shell faces.
    In a face list a particular face is defined be a set of number. The first number contains the number of vertices of the face and its sign indicates if the face is a hole or a regular face: if the face is a hole then its vertices number must be negative.
    A hole face must be coplanar and entirely in another face (not touching edges).
    For example, the face array {3,0,2,4,-3,1,0,5} represents two faces. The first is a regular face that consists of three vertices: vertex 0, vertex 2, and vertex 4. The second is the hole face having three vertices: vertex 1, vertex 0, and vertex 5.
  */
  OdInt32Array          faces;

  /** \details
    An array of edges colors.
  */
  OdUInt16Array         edgeColors;

  /** \details
    An array of edges true colors.
  */
  OdCmEntityColorArray  edgeTrueColors;

  /** \details
    An array of edges layers.
  */
  OdDbStubPtrArray edgeLayers;

  /** \details
    An array of edges linetypes.
  */
  OdDbStubPtrArray edgeLinetypes;

  /** \details
    An array of edges selection markers.
  */
  OdGsMarkerArray       edgeSelectionMarkers;

  /** \details
    An array of edges visibilities.
  */
  OdUInt8Array          edgeVisibilities;

  /** \details
    An array of faces colors.
  */
  OdUInt16Array         faceColors;

  /** \details
    An array of faces true colors.
  */
  OdCmEntityColorArray  faceTrueColors;

  /** \details
    An array of faces layers.
  */
  OdDbStubPtrArray faceLayers;

  /** \details
    An array of faces normal vectors.
  */
  OdGeVector3dArray     faceNormals;

  /** \details
    An array of faces selection markers.
  */
  OdGsMarkerArray       faceSelectionMarkers;

  /** \details
    An array of faces visibilities.
  */
  OdUInt8Array          faceVisibilities;

  /** \details
    An array of faces materials.
  */
  OdDbStubPtrArray faceMaterials;

  /** \details
    An array of faces mappers.
  */
  OdArray<OdGiMapper>   faceMappers;

  /** \details
    An array of faces transparencies.
  */
  OdCmTransparencyArray faceTransparencies;

  /** \details
    An array of texture coordinates.
  */
  OdGePoint3dArray textureCoords;

  /** \details
    An array of vertex normal vectors.
  */
  OdGeVector3dArray vertexNormals;

  /** \details
    Adds a vertex to the vertex array of the shell.

    \param vertex [in]  The vertex to be inserted.
  */
  void addVertex(const OdGePoint3d& vertex)
  {
    vertices.push_back(vertex);
  }

  /** \details
    Ends face assembling. Writes the number of face vertices that have been added by the addFaceVertex() method.
  */
  void endFace()
  {
    if(m_nFaceStartIndex>=0)
    {
      faces[m_nFaceStartIndex] *= (faces.size()-m_nFaceStartIndex-1);
      m_nFaceStartIndex = -1;
    }
  }

  /** \details
    Starts face assembling. Sets the face index at the start position of a new face to be added and reserves place for the number of face vertices adding -1 or 1 values: -1 if a hole face is expected and 1 for a regular face.

    \param bHole [in]  Indicates if a hole or regular face is expected. It's false by default.
  */
  void startFace(bool bHole = false)
  {
    ODA_ASSERT(m_nFaceStartIndex==-1); // endFace() wasn't called.
    m_nFaceStartIndex = faces.size();
    faces.push_back(bHole ? -1 : 1);
  }

  /** \details
    Adds a face vertex with a specified index to the face array. 

    \param nVertexIndex [in]  The index from the shell vertices array.
  */
  void addFaceVertex(int nVertexIndex)
  {
    edgeVisibilities.push_back(OdUInt8(nVertexIndex > 0));
    faces.push_back(Od_abs(nVertexIndex)-1);
  }

  /** \details
    Draws the shell using the specified vectorization context.

    \param geometry [in]  The vectorization context.
  */
  void draw(OdGiGeometry& geometry)
  {
    ODA_ASSERT(m_nFaceStartIndex<0 || faces[m_nFaceStartIndex]>2); // endFace() wasn't called.

    OdGiEdgeData edgeData;
    if (!edgeColors.isEmpty()) 
      edgeData.setColors(edgeColors.getPtr());
    if (!edgeTrueColors.isEmpty()) 
      edgeData.setTrueColors(edgeTrueColors.getPtr());
    if (!edgeLayers.isEmpty())
      edgeData.setLayers((OdDbStub**)edgeLayers.getPtr());
    if (!edgeLinetypes.isEmpty()) 
      edgeData.setLinetypes((OdDbStub**)edgeLinetypes.getPtr());
    if (!edgeSelectionMarkers.isEmpty()) 
      edgeData.setSelectionMarkers(edgeSelectionMarkers.getPtr());
    if (!edgeVisibilities.isEmpty()) 
      edgeData.setVisibility(edgeVisibilities.getPtr());

    OdGiFaceData faceData;
    if (!faceColors.isEmpty())
      faceData.setColors(faceColors.getPtr());
    if (!faceTrueColors.isEmpty()) 
      faceData.setTrueColors(faceTrueColors.getPtr());
    if (!faceLayers.isEmpty()) 
      faceData.setLayers((OdDbStub**)faceLayers.getPtr());
    if (!faceNormals.isEmpty()) 
      faceData.setNormals(faceNormals.getPtr());
    if (!faceSelectionMarkers.isEmpty()) 
      faceData.setSelectionMarkers(faceSelectionMarkers.getPtr());
    if (!faceVisibilities.isEmpty()) 
      faceData.setVisibility(faceVisibilities.getPtr());
    if (!faceMaterials.isEmpty()) 
      faceData.setMaterials((OdDbStub**)faceMaterials.getPtr());
    if (!faceMappers.isEmpty()) 
      faceData.setMappers(faceMappers.getPtr());
    if (!faceTransparencies.isEmpty()) 
      faceData.setTransparency(faceTransparencies.getPtr());

    if (!textureCoords.isEmpty() || !vertexNormals.isEmpty())
    {
      OdGiVertexData pVertexData;
      if (!textureCoords.isEmpty())
        pVertexData.setMappingCoords(OdGiVertexData::kAllChannels, textureCoords.asArrayPtr());
      if (!vertexNormals.isEmpty())
        pVertexData.setNormals(vertexNormals.getPtr());

      geometry.shell(vertices.size(), vertices.getPtr(), faces.size(), faces.getPtr(), &edgeData, &faceData, &pVertexData);
    }
    else
      geometry.shell(vertices.size(), vertices.getPtr(), faces.size(), faces.getPtr(), &edgeData, &faceData);
  }
};

#include "TD_PackPop.h"


#endif // _ODGESHELL_INCLUDED_
