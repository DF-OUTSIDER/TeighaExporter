/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
// JsonMetafileConverter

#ifndef JSON_METAFILE_CONVERTER
#define JSON_METAFILE_CONVERTER

#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"
#include "RxObject.h"
#include "OdHeap.h"
#include "OdArray.h"

class OdTrVisFlatMetafileContainer;
class JsonObjectFormat;

/** \details
  <group Other_Classes>
*/
class JsonMetafileConverter
{
public:
  enum kGeomMarkerType
  {
    gm_default = 0,
    gm_2dFacets = 1,
    gm_2dFacetsNoFill = 2,
    gm_3dFacets = 3,
    gm_3dFacetsNoFill = 4,
    gm_2dFacetEdges = 6,
    gm_3dFacetEdges = 7,
    gm_Isolines = 8,
  };

  enum kFacesFormat
  {
    triangle = 0x00000000,
    vertex_normal = 0x00000020,
    vertex_color = 0x00000080
  };

  enum kGeometryType
  {
    geometry = 0,
    buffer_geometry
  };

  enum kGeomArrayType
  {
    vertex_arr = 0,
    colors_arr = 1,
    texture_coord_arr = 2,
    normals_arr = 3,
    secondary_normal_arr = 4
  };

  struct line_weight
  {
    enum kLineWeightType
    {
      ind_attach_viewport = 0,
      float_val = 1,
      i16_val = 2,
      i16_val_at_coord = 3,
    };

    line_weight()
      : type(float_val)
      , double_val(1)
    {}

    void SetType(OdUInt8 line_weight_type, const OdInt16 &val)
    {
      type = (kLineWeightType)line_weight_type;
      double_val = 0;
      int_val = val;
    }
    void SetType(OdUInt8 line_weight_type, const double &val)
    {
       type = (kLineWeightType)line_weight_type;
       double_val = val;
    }

    const kLineWeightType &GetType() const { return type; }
    const OdInt16 &GetIntVal() const { return int_val; }
    const double &GetDoubleVal() const { return double_val; }

  private:
    kLineWeightType type;
    union
    {
      OdInt16  int_val;
      double   double_val;
    };
  };

  JsonMetafileConverter()
    : m_rgb(0)
    , m_material_id(0)
    , m_faces_form(0)
    , m_geom_marker_type(gm_default)
    , m_geometry_arrays()
    , m_objs()
    , m_weight()
  {}
  ~JsonMetafileConverter() {}

  void SetRGB(const ODCOLORREF &rgb) { m_rgb = rgb; }
  void SetMaterial(const OdUInt64 &material_id) { m_material_id = material_id; }
  void SetGeomMark(OdUInt8 geometry_marker) { m_geom_marker_type = (kGeomMarkerType)geometry_marker; }
  template <typename T>
  void SetLineWeight(OdUInt8 line_weight_type, const T &val) { m_weight.SetType(line_weight_type, val); }

  void AndToFacesForm(const OdUInt32 &mask) { m_faces_form &= mask; }
  void OrToFacesForm(const OdUInt32 &mask) { m_faces_form |= mask; }

  void AddGeometryArrayInd(const OdUInt32 &ind, OdUInt8 type) { m_geometry_arrays.insert(std::pair<OdUInt32, OdUInt8>(ind, type)); }
  void DeleteGeometryArrayByType(OdUInt8 type);

  void AddPoint(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type = geometry, const OdUInt32 &ind = 0);
  void AddPoint(const float *pFloats);
  void AddLine(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type =  geometry, const OdUInt32 &ind = 0);
  void AddLine(const float *pFloats);
  void AddLineStrip(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type = geometry, const OdUInt32 &ind = 0);
  void AddMesh(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type = geometry, const OdUInt32 &ind = 0);

  void AddObjectsByRootId(JsonObjectFormat *json_obj, const OdTrVisFlatMetafileContainer *pMetafile, const OdUInt64 &root_id, const OdUInt64 &layer_id);

private:
  ODCOLORREF m_rgb;
  OdUInt64 m_material_id;
  OdUInt32 m_faces_form;
  kGeomMarkerType m_geom_marker_type;
  std::map<OdUInt32, OdUInt8> m_geometry_arrays;
  std::vector<OdRxObjectPtr> m_objs;
  line_weight m_weight;
};

#endif // JSON_METAFILE_CONVERTER
