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

#ifndef __OD_GI_SHM_DATA_STORAGE__
#define __OD_GI_SHM_DATA_STORAGE__

#include "Gi/GiGeometry.h"
#include "Ge/GeVector3dArray.h"
#include "DbStubPtrArray.h"
#include "CmEntityColorArray.h"
#include "CmTransparencyArray.h"
#include "UInt8Array.h"
#include "UInt32Array.h"
#include "Int32Array.h"
#include "UInt16Array.h"
#include "GsMarkerArray.h"

class OdGiEdgeDataStorage : public OdGiEdgeData
{
  protected:
    OdUInt16Array m_colors;
    OdCmEntityColorArray m_trueColors;
    OdDbStubPtrArray m_layers;
    OdDbStubPtrArray m_linetypes;
    OdGsMarkerArray m_selMarkers;
    OdUInt8Array m_visibilities;
  public:
    OdGiEdgeDataStorage() : OdGiEdgeData() { }

    const OdUInt16Array &colorsArray() const { return m_colors; }
    OdUInt16Array &colorsArray() { return m_colors; }
    void setColorsArray() { setColors(m_colors.getPtr()); }
    OdUInt16 *resizeColorsArray(OdUInt32 nSize, bool bSetPtr = true) { m_colors.resize(nSize); if (bSetPtr) setColorsArray(); return m_colors.asArrayPtr(); }

    const OdCmEntityColorArray &trueColorsArray() const { return m_trueColors; }
    OdCmEntityColorArray &trueColorsArray() { return m_trueColors; }
    void setTrueColorsArray() { setTrueColors(m_trueColors.getPtr()); }
    OdCmEntityColor *resizeTrueColorsArray(OdUInt32 nSize, bool bSetPtr = true) { m_trueColors.resize(nSize); if (bSetPtr) setTrueColorsArray(); return m_trueColors.asArrayPtr(); }

    const OdDbStubPtrArray &layerIdsArray() const { return m_layers; }
    OdDbStubPtrArray &layerIdsArray() { return m_layers; }
    void setLayersArray() { setLayers(const_cast<OdDbStub**>(m_layers.getPtr())); }
    OdDbStub **resizeLayerIdsArray(OdUInt32 nSize, bool bSetPtr = true) { m_layers.resize(nSize); if (bSetPtr) setLayersArray(); return m_layers.asArrayPtr(); }

    const OdDbStubPtrArray &linetypeIdsArray() const { return m_linetypes; }
    OdDbStubPtrArray &linetypeIdsArray() { return m_linetypes; }
    void setLinetypesArray() { setLinetypes(const_cast<OdDbStub**>(m_linetypes.getPtr())); }
    OdDbStub **resizeLinetypeIdsArray(OdUInt32 nSize, bool bSetPtr = true) { m_linetypes.resize(nSize); if (bSetPtr) setLinetypesArray(); return m_linetypes.asArrayPtr(); }

    const OdGsMarkerArray &selectionMarkersArray() const { return m_selMarkers; }
    OdGsMarkerArray &selectionMarkersArray() { return m_selMarkers; }
    void setSelectionMarkersArray() { setSelectionMarkers(m_selMarkers.getPtr()); }
    OdGsMarker *resizeSelectionMarkersArray(OdUInt32 nSize, bool bSetPtr = true) { m_selMarkers.resize(nSize); if (bSetPtr) setSelectionMarkersArray(); return m_selMarkers.asArrayPtr(); }

    const OdUInt8Array &visibilityArray() const { return m_visibilities; }
    OdUInt8Array &visibilityArray() { return m_visibilities; }
    void setVisibilityArray() { setVisibility(m_visibilities.getPtr()); }
    OdUInt8 *resizeVisibilityArray(OdUInt32 nSize, bool bSetPtr = true) { m_visibilities.resize(nSize); if (bSetPtr) setVisibilityArray(); return m_visibilities.asArrayPtr(); }

    void cloneData(const OdGiEdgeData *pData) { *(OdGiEdgeData*)this = *pData; }
    void resetPointers()
    {
      setColors(NULL);
      setTrueColors(NULL);
      setLayers(NULL);
      setLinetypes(NULL);
      setSelectionMarkers(NULL);
      setVisibility(NULL);
    }
    void setPointersArray(const OdGiEdgeData *pEdgeData = NULL)
    {
      if (!pEdgeData || pEdgeData->colors()) setColorsArray();
      if (!pEdgeData || pEdgeData->trueColors()) setTrueColorsArray();
      if (!pEdgeData || pEdgeData->layerIds()) setLayersArray();
      if (!pEdgeData || pEdgeData->linetypeIds()) setLinetypesArray();
      if (!pEdgeData || pEdgeData->selectionMarkers()) setSelectionMarkersArray();
      if (!pEdgeData || pEdgeData->visibility()) setVisibilityArray();
    }
    void clearArrays(const OdGiEdgeData *pEdgeData = NULL)
    {
      if (!pEdgeData || pEdgeData->colors()) m_colors.clear();
      if (!pEdgeData || pEdgeData->trueColors()) m_trueColors.clear();
      if (!pEdgeData || pEdgeData->layerIds()) m_layers.clear();
      if (!pEdgeData || pEdgeData->linetypeIds()) m_linetypes.clear();
      if (!pEdgeData || pEdgeData->selectionMarkers()) m_selMarkers.clear();
      if (!pEdgeData || pEdgeData->visibility()) m_visibilities.clear();
    }
    void deleteArrays(const OdGiEdgeData *pEdgeData = NULL)
    {
      if (!pEdgeData || pEdgeData->colors()) m_colors.setPhysicalLength(0);
      if (!pEdgeData || pEdgeData->trueColors()) m_trueColors.setPhysicalLength(0);
      if (!pEdgeData || pEdgeData->layerIds()) m_layers.setPhysicalLength(0);
      if (!pEdgeData || pEdgeData->linetypeIds()) m_linetypes.setPhysicalLength(0);
      if (!pEdgeData || pEdgeData->selectionMarkers()) m_selMarkers.setPhysicalLength(0);
      if (!pEdgeData || pEdgeData->visibility()) m_visibilities.setPhysicalLength(0);
    }
    void reserveArrays(const OdGiEdgeData *pEdgeData, OdUInt32 nReserve)
    {
      if (!pEdgeData || pEdgeData->colors()) m_colors.reserve(nReserve);
      if (!pEdgeData || pEdgeData->trueColors()) m_trueColors.reserve(nReserve);
      if (!pEdgeData || pEdgeData->layerIds()) m_layers.reserve(nReserve);
      if (!pEdgeData || pEdgeData->linetypeIds()) m_linetypes.reserve(nReserve);
      if (!pEdgeData || pEdgeData->selectionMarkers()) m_selMarkers.reserve(nReserve);
      if (!pEdgeData || pEdgeData->visibility()) m_visibilities.reserve(nReserve);
    }
    void copyFrom(const OdGiEdgeData *pEdgeData, OdUInt32 nEdge)
    {
      if (pEdgeData && pEdgeData->colors()) m_colors.push_back(pEdgeData->colors()[nEdge]);
      if (pEdgeData && pEdgeData->trueColors()) m_trueColors.push_back(pEdgeData->trueColors()[nEdge]);
      if (pEdgeData && pEdgeData->layerIds()) m_layers.push_back(pEdgeData->layerIds()[nEdge]);
      if (pEdgeData && pEdgeData->linetypeIds()) m_linetypes.push_back(pEdgeData->linetypeIds()[nEdge]);
      if (pEdgeData && pEdgeData->selectionMarkers()) m_selMarkers.push_back(pEdgeData->selectionMarkers()[nEdge]);
      if (pEdgeData && pEdgeData->visibility()) m_visibilities.push_back(pEdgeData->visibility()[nEdge]);
    }
};

class OdGiFaceDataStorage : public OdGiFaceData
{
  public:
    typedef OdArray<OdGiMapper, OdMemoryAllocator<OdGiMapper> > OdGiMapperArray;
  protected:
    OdUInt16Array m_colors;
    OdCmEntityColorArray m_trueColors;
    OdDbStubPtrArray m_layers;
    OdGsMarkerArray m_markers;
    OdGeVector3dArray m_normals;
    OdUInt8Array m_visibilities;
    OdDbStubPtrArray m_materials;
    OdGiMapperArray m_mappers;
    OdCmTransparencyArray m_transparencies;
  public:
    OdGiFaceDataStorage() : OdGiFaceData() { }

    const OdUInt16Array &colorsArray() const { return m_colors; }
    OdUInt16Array &colorsArray() { return m_colors; }
    void setColorsArray() { setColors(m_colors.getPtr()); }
    OdUInt16 *resizeColorsArray(OdUInt32 nSize, bool bSetPtr = true) { m_colors.resize(nSize); if (bSetPtr) setColorsArray(); return m_colors.asArrayPtr(); }

    const OdCmEntityColorArray &trueColorsArray() const { return m_trueColors; }
    OdCmEntityColorArray &trueColorsArray() { return m_trueColors; }
    void setTrueColorsArray() { setTrueColors(m_trueColors.getPtr()); }
    OdCmEntityColor *resizeTrueColorsArray(OdUInt32 nSize, bool bSetPtr = true) { m_trueColors.resize(nSize); if (bSetPtr) setTrueColorsArray(); return m_trueColors.asArrayPtr(); }

    const OdDbStubPtrArray &layerIdsArray() const { return m_layers; }
    OdDbStubPtrArray &layerIdsArray() { return m_layers; }
    void setLayersArray() { setLayers(const_cast<OdDbStub**>(m_layers.getPtr())); }
    OdDbStub **resizeLayerIdsArray(OdUInt32 nSize, bool bSetPtr = true) { m_layers.resize(nSize); if (bSetPtr) setLayersArray(); return m_layers.asArrayPtr(); }

    const OdGsMarkerArray &selectionMarkersArray() const { return m_markers; }
    OdGsMarkerArray &selectionMarkersArray() { return m_markers; }
    void setSelectionMarkersArray() { setSelectionMarkers(m_markers.getPtr()); }
    OdGsMarker *resizeSelectionMarkersArray(OdUInt32 nSize, bool bSetPtr = true) { m_markers.resize(nSize); if (bSetPtr) setSelectionMarkersArray(); return m_markers.asArrayPtr(); }

    const OdGeVector3dArray &normalsArray() const { return m_normals; }
    OdGeVector3dArray &normalsArray() { return m_normals; }
    void setNormalsArray() { setNormals(m_normals.getPtr()); }
    OdGeVector3d *resizeNormalsArray(OdUInt32 nSize, bool bSetPtr = true) { m_normals.resize(nSize); if (bSetPtr) setNormalsArray(); return m_normals.asArrayPtr(); }

    const OdUInt8Array &visibilityArray() const { return m_visibilities; }
    OdUInt8Array &visibilityArray() { return m_visibilities; }
    void setVisibilityArray() { setVisibility(m_visibilities.getPtr()); }
    OdUInt8 *resizeVisibilityArray(OdUInt32 nSize, bool bSetPtr = true) { m_visibilities.resize(nSize); if (bSetPtr) setVisibilityArray(); return m_visibilities.asArrayPtr(); }

    const OdDbStubPtrArray &matkerialsArray() const { return m_materials; }
    OdDbStubPtrArray &materialsArray() { return m_materials; }
    void setMaterialsArray() { setMaterials(const_cast<OdDbStub**>(m_materials.getPtr())); }
    OdDbStub **resizeMaterialsArray(OdUInt32 nSize, bool bSetPtr = true) { m_materials.resize(nSize); if (bSetPtr) setMaterialsArray(); return m_materials.asArrayPtr(); }

    const OdGiMapperArray &mappersArray() const { return m_mappers; }
    OdGiMapperArray &mappersArray() { return m_mappers; }
    void setMappersArray() { setMappers(m_mappers.getPtr()); }
    OdGiMapper *resizeMappersArray(OdUInt32 nSize, bool bSetPtr = true) { m_mappers.resize(nSize); if (bSetPtr) setMappersArray(); return m_mappers.asArrayPtr(); }

    const OdCmTransparencyArray &transparencyArray() const { return m_transparencies; }
    OdCmTransparencyArray &transparencyArray() { return m_transparencies; }
    void setTransparencyArray() { setTransparency(m_transparencies.getPtr()); }
    OdCmTransparency *resizeTransparencyArray(OdUInt32 nSize, bool bSetPtr = true) { m_transparencies.resize(nSize); if (bSetPtr) setTransparencyArray(); return m_transparencies.asArrayPtr(); }

    void cloneData(const OdGiFaceData *pData) { *(OdGiFaceData*)this = *pData; }
    void resetPointers()
    {
      setColors(NULL);
      setTrueColors(NULL);
      setLayers(NULL);
      setSelectionMarkers(NULL);
      setNormals(NULL);
      setVisibility(NULL);
      setMaterials(NULL);
      setMappers(NULL);
      setTransparency(NULL);
    }
    void setPointersArray(const OdGiFaceData *pFaceData = NULL)
    {
      if (!pFaceData || pFaceData->colors()) setColorsArray();
      if (!pFaceData || pFaceData->trueColors()) setTrueColorsArray();
      if (!pFaceData || pFaceData->layerIds()) setLayersArray();
      if (!pFaceData || pFaceData->selectionMarkers()) setSelectionMarkersArray();
      if (!pFaceData || pFaceData->normals()) setNormalsArray();
      if (!pFaceData || pFaceData->visibility()) setVisibilityArray();
      if (!pFaceData || pFaceData->materials()) setMaterialsArray();
      if (!pFaceData || pFaceData->mappers()) setMappersArray();
      if (!pFaceData || pFaceData->transparency()) setTransparencyArray();
    }
    void clearArrays(const OdGiFaceData *pFaceData = NULL)
    {
      if (!pFaceData || pFaceData->colors()) m_colors.clear();
      if (!pFaceData || pFaceData->trueColors()) m_trueColors.clear();
      if (!pFaceData || pFaceData->layerIds()) m_layers.clear();
      if (!pFaceData || pFaceData->selectionMarkers()) m_markers.clear();
      if (!pFaceData || pFaceData->normals()) m_normals.clear();
      if (!pFaceData || pFaceData->visibility()) m_visibilities.clear();
      if (!pFaceData || pFaceData->materials()) m_materials.clear();
      if (!pFaceData || pFaceData->mappers()) m_mappers.clear();
      if (!pFaceData || pFaceData->transparency()) m_transparencies.clear();
    }
    void deleteArrays(const OdGiFaceData *pFaceData = NULL)
    {
      if (!pFaceData || pFaceData->colors()) m_colors.setPhysicalLength(0);
      if (!pFaceData || pFaceData->trueColors()) m_trueColors.setPhysicalLength(0);
      if (!pFaceData || pFaceData->layerIds()) m_layers.setPhysicalLength(0);
      if (!pFaceData || pFaceData->selectionMarkers()) m_markers.setPhysicalLength(0);
      if (!pFaceData || pFaceData->normals()) m_normals.setPhysicalLength(0);
      if (!pFaceData || pFaceData->visibility()) m_visibilities.setPhysicalLength(0);
      if (!pFaceData || pFaceData->materials()) m_materials.setPhysicalLength(0);
      if (!pFaceData || pFaceData->mappers()) m_mappers.setPhysicalLength(0);
      if (!pFaceData || pFaceData->transparency()) m_transparencies.setPhysicalLength(0);
    }
    void reserveArrays(const OdGiFaceData *pFaceData, OdUInt32 nReserve)
    {
      if (!pFaceData || pFaceData->colors()) m_colors.reserve(nReserve);
      if (!pFaceData || pFaceData->trueColors()) m_trueColors.reserve(nReserve);
      if (!pFaceData || pFaceData->layerIds()) m_layers.reserve(nReserve);
      if (!pFaceData || pFaceData->selectionMarkers()) m_markers.reserve(nReserve);
      if (!pFaceData || pFaceData->normals()) m_normals.reserve(nReserve);
      if (!pFaceData || pFaceData->visibility()) m_visibilities.reserve(nReserve);
      if (!pFaceData || pFaceData->materials()) m_materials.reserve(nReserve);
      if (!pFaceData || pFaceData->mappers()) m_mappers.reserve(nReserve);
      if (!pFaceData || pFaceData->transparency()) m_transparencies.reserve(nReserve);
    }
    void copyFrom(const OdGiFaceData *pFaceData, OdUInt32 nFace)
    {
      if (pFaceData && pFaceData->colors()) m_colors.push_back(pFaceData->colors()[nFace]);
      if (pFaceData && pFaceData->trueColors()) m_trueColors.push_back(pFaceData->trueColors()[nFace]);
      if (pFaceData && pFaceData->layerIds()) m_layers.push_back(pFaceData->layerIds()[nFace]);
      if (pFaceData && pFaceData->selectionMarkers()) m_markers.push_back(pFaceData->selectionMarkers()[nFace]);
      if (pFaceData && pFaceData->normals()) m_normals.push_back(pFaceData->normals()[nFace]);
      if (pFaceData && pFaceData->visibility()) m_visibilities.push_back(pFaceData->visibility()[nFace]);
      if (pFaceData && pFaceData->materials()) m_materials.push_back(pFaceData->materials()[nFace]);
      if (pFaceData && pFaceData->mappers()) m_mappers.push_back(pFaceData->mappers()[nFace]);
      if (pFaceData && pFaceData->transparency()) m_transparencies.push_back(pFaceData->transparency()[nFace]);
    }
};

class OdGiVertexDataStorage : public OdGiVertexData
{
  protected:
    OdGeVector3dArray m_normals;
    OdCmEntityColorArray m_trueColors;
    OdGePoint3dArray m_texCoords;
  public:
    OdGiVertexDataStorage() : OdGiVertexData() { }

    const OdGeVector3dArray &normalsArray() const { return m_normals; }
    OdGeVector3dArray &normalsArray() { return m_normals; }
    void setNormalsArray() { setNormals(m_normals.getPtr()); }
    OdGeVector3d *resizeNormalsArray(OdUInt32 nSize, bool bSetPtr = true) { m_normals.resize(nSize); if (bSetPtr) setNormalsArray(); return m_normals.asArrayPtr(); }

    const OdCmEntityColorArray &trueColorsArray() const { return m_trueColors; }
    OdCmEntityColorArray &trueColorsArray() { return m_trueColors; }
    void setTrueColorsArray() { setTrueColors(m_trueColors.getPtr()); }
    OdCmEntityColor *resizeTrueColorsArray(OdUInt32 nSize, bool bSetPtr = true) { m_trueColors.resize(nSize); if (bSetPtr) setTrueColorsArray(); return m_trueColors.asArrayPtr(); }

    const OdGePoint3dArray &mappingCoordsArray(MapChannel /*channel*/) const { return m_texCoords; }
    OdGePoint3dArray &mappingCoordsArray(MapChannel /*channel*/) { return m_texCoords; }
    void setMappingCoordsArray(MapChannel channel) { setMappingCoords(channel, m_texCoords.getPtr()); }
    OdGePoint3d *resizeMappingCoordsArray(MapChannel channel, OdUInt32 nSize, bool bSetPtr = true) { m_texCoords.resize(nSize); if (bSetPtr) setMappingCoordsArray(channel); return m_texCoords.asArrayPtr(); }

    void cloneData(const OdGiVertexData *pData) { *(OdGiVertexData*)this = *pData; }
    void resetPointers()
    {
      setNormals(NULL);
      setTrueColors(NULL);
      setMappingCoords(OdGiVertexData::kAllChannels, NULL);
    }
    void setPointersArray(const OdGiVertexData *pVertexData = NULL)
    {
      if (!pVertexData || pVertexData->normals()) setNormalsArray();
      if (!pVertexData || pVertexData->trueColors()) setTrueColorsArray();
      if (!pVertexData || pVertexData->mappingCoords(OdGiVertexData::kAllChannels)) setMappingCoordsArray(OdGiVertexData::kAllChannels);
    }
    void clearArrays(const OdGiVertexData *pVertexData = NULL)
    {
      if (!pVertexData || pVertexData->normals()) m_normals.clear();
      if (!pVertexData || pVertexData->trueColors()) m_trueColors.clear();
      if (!pVertexData || pVertexData->mappingCoords(OdGiVertexData::kAllChannels)) m_texCoords.clear();
    }
    void deleteArrays(const OdGiVertexData *pVertexData = NULL)
    {
      if (!pVertexData || pVertexData->normals()) m_normals.setPhysicalLength(0);
      if (!pVertexData || pVertexData->trueColors()) m_trueColors.setPhysicalLength(0);
      if (!pVertexData || pVertexData->mappingCoords(OdGiVertexData::kAllChannels)) m_texCoords.setPhysicalLength(0);
    }
    void reserveArrays(const OdGiVertexData *pVertexData, OdUInt32 nReserve)
    {
      if (!pVertexData || pVertexData->normals()) m_normals.reserve(nReserve);
      if (!pVertexData || pVertexData->trueColors()) m_trueColors.reserve(nReserve);
      if (!pVertexData || pVertexData->mappingCoords(OdGiVertexData::kAllChannels)) m_texCoords.reserve(nReserve);
    }
    void copyFrom(const OdGiVertexData *pVertexData, OdUInt32 nVertex)
    {
      if (pVertexData && pVertexData->normals()) m_normals.push_back(pVertexData->normals()[nVertex]);
      if (pVertexData && pVertexData->trueColors()) m_trueColors.push_back(pVertexData->trueColors()[nVertex]);
      if (pVertexData && pVertexData->mappingCoords(OdGiVertexData::kAllChannels))
        m_texCoords.push_back(pVertexData->mappingCoords(OdGiVertexData::kAllChannels)[nVertex]);
    }
};

class OdGiShmDataStorage
{
  protected:
    OdGePoint3dArray m_vertexList;
    const OdGePoint3d *m_pVertexList;
    OdInt32Array m_faceList;
    const OdInt32 *m_pFaceList;
    OdGiEdgeDataStorage m_edgeData;
    const OdGiEdgeData *m_pEdgeData;
    OdGiFaceDataStorage m_faceData;
    const OdGiFaceData *m_pFaceData;
    OdGiVertexDataStorage m_vertexData;
    const OdGiVertexData *m_pVertexData;
  public:
    OdGiShmDataStorage()
      : m_pVertexList(NULL)
      , m_pFaceList(NULL)
      , m_pEdgeData(NULL)
      , m_pFaceData(NULL)
      , m_pVertexData(NULL)
    { }

    void setVertexList(const OdGePoint3d *pVertexList) { m_pVertexList = pVertexList; }
    const OdGePoint3d *vertexList() const { return m_pVertexList; }
    const OdGePoint3dArray &vertexListArray() const { return m_vertexList; }
    OdGePoint3dArray &vertexListArray() { return m_vertexList; }
    void setVertexListArray() { setVertexList(m_vertexList.getPtr()); }
    OdGePoint3d *resizeVertexListArray(OdUInt32 nSize, bool bSetPtr = true) { m_vertexList.resize(nSize); if (bSetPtr) setVertexListArray(); return m_vertexList.asArrayPtr(); }

    void setFaceList(const OdInt32 *pFaceList) { m_pFaceList = pFaceList; }
    const OdInt32 *faceList() const { return m_pFaceList; }
    const OdInt32Array &faceListArray() const { return m_faceList; }
    OdInt32Array &faceListArray() { return m_faceList; }
    void setFaceListArray() { setFaceList(m_faceList.getPtr()); }
    OdInt32 *resizeFaceListArray(OdUInt32 nSize, bool bSetPtr = true) { m_faceList.resize(nSize); if (bSetPtr) setFaceListArray(); return m_faceList.asArrayPtr(); }

    void setEdgeData(const OdGiEdgeData *pEdgeData) { m_pEdgeData = pEdgeData; }
    const OdGiEdgeData *edgeData() const { return m_pEdgeData; }
    const OdGiEdgeDataStorage &edgeDataStorage() const { return m_edgeData; }
    OdGiEdgeDataStorage &edgeDataStorage() { return m_edgeData; }
    void setEdgeDataStorage() { setEdgeData(&m_edgeData); }
    void resetEdgeData() { m_pEdgeData = NULL; }
    void cloneEdgeData(const OdGiEdgeData *pEdgeData) { if (pEdgeData) { m_edgeData.cloneData(pEdgeData); setEdgeDataStorage(); } else resetEdgeData(); }

    void setFaceData(const OdGiFaceData *pFaceData) { m_pFaceData = pFaceData; }
    const OdGiFaceData *faceData() const { return m_pFaceData; }
    const OdGiFaceDataStorage &faceDataStorage() const { return m_faceData; }
    OdGiFaceDataStorage &faceDataStorage() { return m_faceData; }
    void setFaceDataStorage() { setFaceData(&m_faceData); }
    void resetFaceData() { m_pFaceData = NULL; }
    void cloneFaceData(const OdGiFaceData *pFaceData) { if (pFaceData) { m_faceData.cloneData(pFaceData); setFaceDataStorage(); } else resetFaceData(); }

    void setVertexData(const OdGiVertexData *pVertexData) { m_pVertexData = pVertexData; }
    const OdGiVertexData *vertexData() const { return m_pVertexData; }
    const OdGiVertexDataStorage &vertexDataStorage() const { return m_vertexData; }
    OdGiVertexDataStorage &vertexDataStorage() { return m_vertexData; }
    void setVertexDataStorage() { setVertexData(&m_vertexData); }
    void resetVertexData() { m_pVertexData = NULL; }
    void cloneVertexData(const OdGiVertexData *pVertexData) { if (pVertexData) { m_vertexData.cloneData(pVertexData); setVertexDataStorage(); } else resetVertexData(); }

    void resetPointers()
    {
      setVertexList(NULL);
      setFaceList(NULL);
      resetEdgeData();
      resetFaceData();
      resetVertexData();
      m_edgeData.resetPointers();
      m_faceData.resetPointers();
      m_vertexData.resetPointers();
    }
    void setPointersArray(const OdGiFaceData *pFaceData = NULL, const OdGiEdgeData *pEdgeData = NULL, const OdGiVertexData *pVertexData = NULL)
    {
      setVertexListArray();
      setFaceListArray();
      if (pEdgeData) setEdgeDataStorage();
      if (pFaceData) setFaceDataStorage();
      if (pVertexData) setVertexDataStorage();
      m_edgeData.setPointersArray(pEdgeData);
      m_faceData.setPointersArray(pFaceData);
      m_vertexData.setPointersArray(pVertexData);
    }
    void clearArrays(const OdGiFaceData *pFaceData = NULL, const OdGiEdgeData *pEdgeData = NULL, const OdGiVertexData *pVertexData = NULL)
    {
      m_vertexList.clear();
      m_faceList.clear();
      m_edgeData.clearArrays(pEdgeData);
      m_faceData.clearArrays(pFaceData);
      m_vertexData.clearArrays(pVertexData);
    }
    void deleteArrays(const OdGiFaceData *pFaceData = NULL, const OdGiEdgeData *pEdgeData = NULL, const OdGiVertexData *pVertexData = NULL)
    {
      m_vertexList.setPhysicalLength(0);
      m_faceList.setPhysicalLength(0);
      m_edgeData.deleteArrays(pEdgeData);
      m_faceData.deleteArrays(pFaceData);
      m_vertexData.deleteArrays(pVertexData);
    }
};

#endif // __OD_GI_SHM_DATA_STORAGE__
