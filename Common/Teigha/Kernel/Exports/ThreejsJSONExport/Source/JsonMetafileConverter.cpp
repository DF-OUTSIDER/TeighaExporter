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

#include "OdaCommon.h"
#include "JsonMetafileConverter.h"
#include "Tr/TrVisMetafileStream.h"
#include "JsonObjectFormat.h"

#define COORDS_COUNT_AT_POINT 3
#define COORDS_COUNT_AT_RGBA_COLOR 4

class JsonInterjacentObject : public OdRxObject
{
public:
  enum kJsonObjType
  {
    point_obj_type = 0,
    line_obj_type = 1,
    line_strip_obj_type = 3,
    mesh_obj_type = 4,
    vline_obj_type = 10,
    vpoint_obj_type = 11
  };

  struct JsonInterjacentMat
  {
    JsonInterjacentMat()
      : m_rgb(0)
      , m_mat_id(0)
      , m_copy_by_material_id(0)
    {}

    JsonInterjacentMat(const ODCOLORREF &rgb, const OdUInt64 &copy_by_material_id)
      : m_rgb(rgb)
      , m_mat_id(0)
      , m_copy_by_material_id(copy_by_material_id)
    {}

    JsonInterjacentMat(const JsonInterjacentMat &o)
      : m_rgb(o.m_rgb)
      , m_mat_id(o.m_mat_id)
      , m_copy_by_material_id(o.m_copy_by_material_id)
    {}

    const ODCOLORREF &GetRGB() const { return m_rgb; }
    const OdUInt64 &GetMaterialID() const { return m_mat_id; }
    const OdUInt64 &GetCopyByMaterialID() const { return m_copy_by_material_id; }

    void SetMaterialID(const OdUInt64 &material_id) { m_mat_id = material_id; }

  private:
    ODCOLORREF m_rgb;
    OdUInt64 m_mat_id;
    OdUInt64 m_copy_by_material_id;
  };

  ODRX_USING_HEAP_OPERATORS(OdRxObject);

  //default constructor
  JsonInterjacentObject()
    : m_geometry(0, JsonMetafileConverter::geometry)
    , m_material()
    , m_type(point_obj_type)
    , m_geometry_marker(JsonMetafileConverter::gm_default)
  {}

  JsonInterjacentObject(const ODCOLORREF &color, const kJsonObjType &object_type, const OdUInt64 &material_id, const JsonMetafileConverter::kGeomMarkerType &geometry_marker, const JsonMetafileConverter::kGeometryType &geometry_type)
    : m_geometry(0, geometry_type)
    , m_material(color, material_id)
    , m_type(object_type)
    , m_geometry_marker(geometry_marker)
  {}

  JsonInterjacentObject(const JsonInterjacentObject &o)
    : m_geometry(o.m_geometry)
    , m_material(o.m_material)
    , m_type(o.m_type)
    , m_geometry_marker(o.m_geometry_marker)
  {}

  virtual ~JsonInterjacentObject() {}

  void addRef() { }
  void release() { }

  const OdUInt64 &GetMaterialID() const { return m_material.GetMaterialID(); }
  const ODCOLORREF &GetMaterialRGB() const { return m_material.GetRGB(); }
  const OdUInt64 &GetCopyByMaterialID() const { return m_material.GetCopyByMaterialID(); }
  const OdUInt64 &GetGeometryID() const { return m_geometry.first; }
  const JsonMetafileConverter::kGeometryType &GetGeometryType() const { return m_geometry.second; }
  const kJsonObjType &GetType() const { return m_type; }
  const JsonMetafileConverter::kGeomMarkerType &GetGeomMarkerType() const { return m_geometry_marker; }

  JsonInterjacentMat *const GetMaterial() { return &m_material; }

  void SetMaterialID(const OdUInt64 &id) { m_material.SetMaterialID(id); }
  void SetGeometryID(const OdUInt64 &id) { m_geometry.first = id; }

  void CalculateMaterialLineWeight(JsonObjectFormat *json_obj);

protected:
  std::pair<OdUInt64, JsonMetafileConverter::kGeometryType> m_geometry;
  JsonInterjacentMat m_material;
  kJsonObjType m_type;
  JsonMetafileConverter::kGeomMarkerType m_geometry_marker;
};

class JsonInterjacentMetafileObject : public JsonInterjacentObject
{
public:
  JsonInterjacentMetafileObject()
    : JsonInterjacentObject()
    , m_weight()
  {}
  JsonInterjacentMetafileObject(const ODCOLORREF &color, const kJsonObjType &object_typet, const OdUInt64 &material_id, const JsonMetafileConverter::kGeomMarkerType &geometry_marker, const JsonMetafileConverter::kGeometryType &geometry, const JsonMetafileConverter::line_weight &weight)
    : JsonInterjacentObject(color, object_typet, material_id, geometry_marker, geometry)
    , m_weight(weight)
  {}
  JsonInterjacentMetafileObject(const JsonInterjacentMetafileObject &o)
    : JsonInterjacentObject(o)
    , m_weight(o.m_weight)
  {}

  virtual ~JsonInterjacentMetafileObject() {}

  void RecalcMaterialLineWeight(const OdUInt64 &material_id, JsonObjectFormat *json_obj)
  {
    if (m_weight.GetType() == JsonMetafileConverter::line_weight::i16_val ||
        m_weight.GetType() == JsonMetafileConverter::line_weight::i16_val_at_coord)
      json_obj->AddMaterialWidth(material_id, m_weight.GetIntVal());
    else if (m_weight.GetType() == JsonMetafileConverter::line_weight::float_val)
      json_obj->AddMaterialLenWeight(material_id, m_weight.GetDoubleVal());
    else if (m_weight.GetType() == JsonMetafileConverter::line_weight::ind_attach_viewport)
      json_obj->AddMaterialLenWeightIndx(material_id, m_weight.GetIntVal());
  }

protected:
  JsonMetafileConverter::line_weight m_weight;
};

class VObject : public JsonInterjacentMetafileObject
{
public:
  enum kVertexCount
  {
    point_vertex_count = 3,
    line_vertex_count = 6
  };

  VObject()
    : JsonInterjacentMetafileObject()
    , m_vertex(NULL)
  {}

  VObject(const ODCOLORREF &color, const kJsonObjType &object_type, const OdUInt64 &material_id, const float* vertex, const JsonMetafileConverter::kGeometryType &geometry_type, const JsonMetafileConverter::line_weight &weight)
    : JsonInterjacentMetafileObject(color, object_type, material_id, JsonMetafileConverter::gm_default, geometry_type, weight)
    , m_vertex(NULL)
  {
    try
    {
      if (m_type == JsonInterjacentObject::vline_obj_type)
      {
        m_vertex = new float[line_vertex_count];
        memcpy((void*)m_vertex, (const void*)vertex, line_vertex_count * sizeof(float));
      }
      else if (m_type == JsonInterjacentObject::vpoint_obj_type)
      {
        m_vertex = new float[point_vertex_count];
        memcpy((void*)m_vertex, (const void*)vertex, point_vertex_count * sizeof(float));
      }
    }
    catch (...)
    {
      if (m_vertex != NULL)
        delete[] m_vertex;
      throw;
    }
  }

  VObject(const VObject& o)
    : JsonInterjacentMetafileObject(o)
    , m_vertex(NULL)
  {
    m_vertex = o.m_vertex;
    const_cast<VObject&>(o).m_vertex = NULL;
  }

  virtual ~VObject()
  {
    if (m_vertex != NULL)
      delete[] m_vertex;
  }

  void AddVertex(JsonObjectFormat *json_obj)
  {
    if (m_type == JsonInterjacentObject::vline_obj_type)
      json_obj->AddGeometriesData<float>(m_geometry.first, sizeof(float) * VObject::line_vertex_count, (void*)m_vertex, JsonGeometriesDataBase::vertec, JsonGeometriesDataBase::move_buffer);
    else if (m_type == JsonInterjacentObject::vpoint_obj_type)
      json_obj->AddGeometriesData<float>(m_geometry.first, sizeof(float) * VObject::point_vertex_count, (void*)m_vertex, JsonGeometriesDataBase::vertec, JsonGeometriesDataBase::move_buffer);
    m_vertex = NULL;
  }

protected:
  float *m_vertex;
};

class DataArrayObject : public JsonInterjacentMetafileObject
{
public:
  DataArrayObject()
    : JsonInterjacentMetafileObject()
    , m_first_index(0)
    , m_count(0)
    , m_array()
    , m_ind_of_index_array(0)
  {}
  
  DataArrayObject(const ODCOLORREF &color, const kJsonObjType &object_type, std::map<OdUInt32, OdUInt8> *geometry_array, const OdInt32 &first_index, const OdInt32 &count, const OdUInt64 &material_id, const JsonMetafileConverter::kGeomMarkerType &geometry_marker, const JsonMetafileConverter::kGeometryType &geometry_type, const OdUInt32 &ind_of_index_array, const JsonMetafileConverter::line_weight &weight)
    : JsonInterjacentMetafileObject(color, object_type, material_id, geometry_marker, geometry_type, weight)
    , m_first_index(first_index)
    , m_count(count)
    , m_array()
    , m_ind_of_index_array(ind_of_index_array)
  {
    std::map<OdUInt32, OdUInt8>::iterator beg = geometry_array->begin();
    while (beg != geometry_array->end())
    {
      m_array.push_back((JsonMetafileConverter::kGeomArrayType)beg->first);
      ++beg;
    }
  }

  virtual ~DataArrayObject() {}

  void AddVertex(JsonObjectFormat *obj, const float *pData, const OdTrVisArrayWrapper &index_array)
  {
    float *vertex = NULL;

    try
    {
      vertex = new float[m_count * COORDS_COUNT_AT_POINT];
      const OdUInt16 *pIndData = (const OdUInt16*)index_array.m_pData;
      OdUInt32 ind = 0;

      if (m_count <= index_array.m_uSize / sizeof(OdUInt16))
      {
        for (OdInt32 i = m_first_index; i < m_count; ++i)
        {
          memcpy(vertex + ind, pData + pIndData[i] * COORDS_COUNT_AT_POINT, COORDS_COUNT_AT_POINT * sizeof(float));
          ind += COORDS_COUNT_AT_POINT;
        }
      }

      obj->AddGeometriesData<float>(m_geometry.first, m_count * sizeof(float) * COORDS_COUNT_AT_POINT, (void*)(vertex), JsonGeometriesDataBase::vertec, JsonGeometriesDataBase::move_buffer);
      vertex = NULL;
    }
    catch (...)
    {
      if (vertex == NULL)
        delete[]vertex;
      throw;
    }
  }

  void AddNormal(JsonObjectFormat *obj, const float *pData, const OdTrVisArrayWrapper &index_array)
  {
    float *normal = NULL;

    try
    {
      normal = new float[m_count * COORDS_COUNT_AT_POINT];
      const OdUInt16 *pIndData = (const OdUInt16*)index_array.m_pData;
      OdUInt32 ind = 0;

      if (m_count <= index_array.m_uSize / sizeof(OdUInt16))
      {
        for (OdInt32 i = m_first_index; i < m_count; ++i)
        {
          memcpy(normal + ind, pData + pIndData[i] * COORDS_COUNT_AT_POINT, COORDS_COUNT_AT_POINT * sizeof(float));
          ind += COORDS_COUNT_AT_POINT;
        }
      }

      obj->AddGeometriesData<float>(m_geometry.first, m_count * sizeof(float) * COORDS_COUNT_AT_POINT, (void*)(normal), JsonGeometriesDataBase::normal, JsonGeometriesDataBase::move_buffer);
      normal = NULL;
    }
    catch (...)
    {
      if (normal == NULL)
        delete[]normal;
      throw;
    }
  }

  void ParseGeometryArrays(JsonObjectFormat *json_obj, const OdTrVisFlatMetafileContainer *pMetafile);

  const OdInt32 &GetFirstCoordIndex() const { return m_first_index; }
  const OdInt32 &GetCoordsCount() const { return m_count; }
  const OdUInt32 &GetIndOfIndexArray() const { return m_ind_of_index_array; }

protected:
  OdInt32 m_first_index;
  OdInt32 m_count;
  OdArray<JsonMetafileConverter::kGeomArrayType> m_array;
  OdUInt32 m_ind_of_index_array;//index of index array
};

class FaceObject : public DataArrayObject
{
public:
  FaceObject()
    : DataArrayObject()
    , m_face_form(0)
  {}
  
  FaceObject(const ODCOLORREF &color, const kJsonObjType &object_type, std::map<OdUInt32, OdUInt8> *geometry_array, const OdInt32 &first_index, const OdInt32 &count, const OdUInt64 &material_id, const JsonMetafileConverter::kGeomMarkerType &geometry_marker, const OdUInt32 &face_form, const JsonMetafileConverter::kGeometryType &geometry_type, const OdUInt32 &ind_of_index_array)
    : DataArrayObject(color, object_type, geometry_array, first_index, count, material_id, geometry_marker, geometry_type, ind_of_index_array, JsonMetafileConverter::line_weight())
    , m_face_form(face_form)
  {}

  virtual ~FaceObject() {}

  void AddFaces(JsonObjectFormat *obj)
  {
    OdInt32 *faces = NULL;
    try
    {
      OdUInt32 step = COORDS_COUNT_AT_POINT;
      if (m_face_form & JsonMetafileConverter::vertex_normal)
        step += COORDS_COUNT_AT_POINT;
      if (m_face_form & JsonMetafileConverter::vertex_color)
        step += COORDS_COUNT_AT_POINT;
      OdUInt32 size = (m_count * (step + 1)) / COORDS_COUNT_AT_POINT;
      faces = new OdInt32[size];

      OdInt32 vert_coord = 0;
      OdInt32 norm_coord = 0;
      OdInt32 color_coord = 0;

      OdUInt32 i = 0;
      while (i < size)
      {
        faces[i++] = m_face_form;

        for (OdUInt32 j = 0; j < 3 && i < size; ++j)
          faces[i++] = vert_coord++;

        if (m_face_form & JsonMetafileConverter::vertex_normal)
        {
          for (OdUInt32 j = 0; j < 3 && i < size; ++j)
            faces[i++] = norm_coord++;
        }

        if (m_face_form & JsonMetafileConverter::vertex_color)
        {
          for (OdUInt32 j = 0; j < 3 && i < size; ++j)
            faces[i++] = color_coord++;
        }
      }

      obj->AddGeometriesData<OdUInt32>(m_geometry.first, size * sizeof(OdUInt32), (void*)faces, JsonGeometriesDataBase::face, JsonGeometriesDataBase::move_buffer);
      faces = NULL;
    }
    catch (...)
    {
      if (faces != NULL)
        delete[] faces;
      throw;
    }
  }

  void AddColors(JsonObjectFormat *obj, const float *pData)
  {
    float *color = NULL;

    try
    {
      color = new float[m_count * COORDS_COUNT_AT_POINT];
      OdInt32 step = 0;
      while (step < m_count)
      {
        memcpy((void*)(color + step * COORDS_COUNT_AT_POINT), (const void*)(pData + (step + m_first_index) * COORDS_COUNT_AT_POINT), COORDS_COUNT_AT_POINT * sizeof(float));
        ++step;
      }

      obj->AddGeometriesData<float>(m_geometry.first, m_count * sizeof(float) * COORDS_COUNT_AT_POINT, (void*)color, JsonGeometriesDataBase::color, JsonGeometriesDataBase::move_buffer);
      color = NULL;
    }
    catch (...)
    {
      if (color != NULL)
        delete[] color;
      throw;
    }
  }

  void AddColors(JsonObjectFormat *obj, const float *pData, const OdTrVisArrayWrapper &index_array)
  {
    float *color = NULL;

    try
    {
      color = new float[m_count * COORDS_COUNT_AT_POINT];

      const OdUInt16 *pIndData = (const OdUInt16*)index_array.m_pData;
      OdUInt32 ind = 0;

      if (m_count <= index_array.m_uSize / sizeof(OdUInt16))
      {
        for (OdInt32 i = m_first_index; i < m_count; ++i)
        {
          memcpy(color + ind, pData + pIndData[i] * COORDS_COUNT_AT_RGBA_COLOR, COORDS_COUNT_AT_POINT * sizeof(float));
          ind += COORDS_COUNT_AT_POINT;
        }
      }

      obj->AddGeometriesData<float>(m_geometry.first, m_count * sizeof(float) * COORDS_COUNT_AT_POINT, (void*)color, JsonGeometriesDataBase::color, JsonGeometriesDataBase::move_buffer);
      color = NULL;
    }
    catch (...)
    {
      if (color != NULL)
        delete[] color;
      throw;
    }
  }

  const OdUInt32 &GetFaceForm() const { return m_face_form; }
protected:
  OdUInt32  m_face_form;
};

void JsonInterjacentObject::CalculateMaterialLineWeight(JsonObjectFormat *json_obj)
{
  if (m_type == JsonInterjacentObject::line_strip_obj_type)
    json_obj->AddMaterialWidth(m_material.GetMaterialID(), 20);
  else if (m_type == JsonInterjacentObject::line_obj_type ||
    m_type == JsonInterjacentObject::point_obj_type)
  {
    DataArrayObject * p_obj = dynamic_cast<DataArrayObject*> (this);
    if (p_obj != NULL)
      p_obj->RecalcMaterialLineWeight(m_material.GetMaterialID(), json_obj);
  }
  else if (m_type == JsonInterjacentObject::vline_obj_type ||
    m_type == JsonInterjacentObject::vpoint_obj_type)
  {
    VObject * p_obj = dynamic_cast<VObject*> (this);
    if (p_obj != NULL)
      p_obj->RecalcMaterialLineWeight(m_material.GetMaterialID(), json_obj);
  }
}

void DataArrayObject::ParseGeometryArrays(JsonObjectFormat *json_obj, const OdTrVisFlatMetafileContainer *pMetafile)
{
  const JsonMetafileConverter::kGeomArrayType * arr_el = m_array.begin();
  while (arr_el != m_array.end())
  {
    const OdTrVisArrayWrapper &array = pMetafile->m_ArrayElements.getPtr()[(*arr_el)];
    switch (array.m_type)
    {
      case OdTrVisArrayWrapper::Type_Vertex:
      {
        const float *pData = (const float*)array.m_pData;
        if (m_geometry.second == JsonMetafileConverter::geometry)
          json_obj->AddGeometriesData<float>(m_geometry.first, m_count * sizeof(float) * COORDS_COUNT_AT_POINT, (void*)(pData + m_first_index * COORDS_COUNT_AT_POINT), JsonGeometriesDataBase::vertec);
        else if (m_geometry.second == JsonMetafileConverter::buffer_geometry)
        {
          const OdTrVisArrayWrapper &index_array = pMetafile->m_ArrayElements.getPtr()[m_ind_of_index_array];
          if (index_array.m_type == OdTrVisArrayWrapper::Type_Index && m_count > 0)
            AddVertex(json_obj, pData, index_array);
        }
        break;
      }
      case OdTrVisArrayWrapper::Type_Normal:
      {
        FaceObject * p_obj_face = dynamic_cast<FaceObject*> (this);

        if (p_obj_face != NULL)
        {
          const float *pData = (const float*)array.m_pData;
          if (p_obj_face->GetFaceForm() & JsonMetafileConverter::vertex_normal)
          {
            if (m_geometry.second == JsonMetafileConverter::geometry)
              json_obj->AddGeometriesData<float>(m_geometry.first, m_count * sizeof(float) * COORDS_COUNT_AT_POINT, (void*)(pData + m_first_index * COORDS_COUNT_AT_POINT), JsonGeometriesDataBase::normal);
            else if (m_geometry.second == JsonMetafileConverter::buffer_geometry)
            {
              const OdTrVisArrayWrapper &index_array = pMetafile->m_ArrayElements.getPtr()[m_ind_of_index_array];
              if (index_array.m_type == OdTrVisArrayWrapper::Type_Index && m_count > 0)
                AddNormal(json_obj, pData, index_array);
            }
          }
        }
        break;
      }
      case OdTrVisArrayWrapper::Type_Color:
      {
        FaceObject * p_obj_face = dynamic_cast<FaceObject*> (this);
        if (p_obj_face != NULL)
        {
          const float *pData = (const float*)array.m_pData;
          if (p_obj_face->GetFaceForm() & JsonMetafileConverter::vertex_color)
          {
            if (m_geometry.second == JsonMetafileConverter::geometry)
              p_obj_face->AddColors(json_obj, pData);
            else if (m_geometry.second == JsonMetafileConverter::buffer_geometry)
            {
              const OdTrVisArrayWrapper &index_array = pMetafile->m_ArrayElements.getPtr()[m_ind_of_index_array];
              if (index_array.m_type == OdTrVisArrayWrapper::Type_Index && m_count > 0)
                p_obj_face->AddColors(json_obj, pData, index_array);
            }
          }
        }
        break;
      }
      case OdTrVisArrayWrapper::Type_TexCoord:
        break;
      case OdTrVisArrayWrapper::Type_Index:
        break;
      case OdTrVisArrayWrapper::Type_Marker:
        break;
    }
    ++arr_el;
  }
}

////////////////////
//JsonMetafileConverter

void JsonMetafileConverter::DeleteGeometryArrayByType(OdUInt8 type)
{
  std::map<OdUInt32, OdUInt8>::iterator beg = m_geometry_arrays.begin();
  while (beg != m_geometry_arrays.end())
  {
    if (beg->second == type)
    {
      m_geometry_arrays.erase(beg);
      beg = m_geometry_arrays.begin();

      if (beg == m_geometry_arrays.end())
        break;
    }
    ++beg;
  }
}

void JsonMetafileConverter::AddPoint(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type, const OdUInt32 &ind)
{
  m_objs.push_back(OdRxObjectPtr(new DataArrayObject(m_rgb, JsonInterjacentObject::point_obj_type, &m_geometry_arrays, first, count, m_material_id, m_geom_marker_type, geom_type, ind, m_weight)));
}
void JsonMetafileConverter::AddPoint(const float *pFloats)
{
  m_objs.push_back(OdRxObjectPtr(new VObject(m_rgb, JsonInterjacentObject::vpoint_obj_type, m_material_id, pFloats, JsonMetafileConverter::geometry, m_weight)));
}
void JsonMetafileConverter::AddLine(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type, const OdUInt32 &ind)
{
  m_objs.push_back(OdRxObjectPtr(new DataArrayObject(m_rgb, JsonInterjacentObject::line_obj_type, &m_geometry_arrays, first, count, m_material_id, m_geom_marker_type, geom_type, ind, m_weight)));
}
void JsonMetafileConverter::AddLine(const float *pFloats)
{
  m_objs.push_back(OdRxObjectPtr(new VObject(m_rgb, JsonInterjacentObject::vline_obj_type, m_material_id, pFloats, JsonMetafileConverter::geometry, m_weight)));
}
void JsonMetafileConverter::AddLineStrip(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type, const OdUInt32 &ind)
{
  m_objs.push_back(OdRxObjectPtr(new DataArrayObject(m_rgb, JsonInterjacentObject::line_strip_obj_type, &m_geometry_arrays, first, count, m_material_id, m_geom_marker_type, geom_type, ind, m_weight)));
}
void JsonMetafileConverter::AddMesh(const OdInt32 &first, const OdInt32 &count, const kGeometryType &geom_type, const OdUInt32 &ind)
{
  m_objs.push_back(OdRxObjectPtr(new FaceObject(m_rgb, JsonInterjacentObject::mesh_obj_type, &m_geometry_arrays, first, count, m_material_id, m_geom_marker_type, m_faces_form, geom_type, ind)));
}

void JsonMetafileConverter::AddObjectsByRootId(JsonObjectFormat *json_obj, const OdTrVisFlatMetafileContainer *pMetafile, const OdUInt64 &root_id, const OdUInt64 &layer_id)
{
  json_obj->AddObjectByLayer(layer_id, root_id);

  std::map<JsonInterjacentObject::kJsonObjType, JsonInterjacentObject::JsonInterjacentMat*> cur_type_materials;

  std::vector<OdRxObjectPtr>::iterator ptr = m_objs.begin();
  while (ptr != m_objs.end())
  {
    JsonInterjacentObject*const p_json_obj = dynamic_cast<JsonInterjacentObject*> (ptr->get());
    if (p_json_obj != NULL)
    {
      //object material
      std::map<JsonInterjacentObject::kJsonObjType, JsonInterjacentObject::JsonInterjacentMat*>::iterator old_mat = cur_type_materials.find(p_json_obj->GetType());
      if (old_mat != cur_type_materials.end() &&
        old_mat->second != NULL &&
        old_mat->second->GetCopyByMaterialID() == p_json_obj->GetCopyByMaterialID() &&
        old_mat->second->GetRGB() == p_json_obj->GetMaterialRGB())
        p_json_obj->SetMaterialID(old_mat->second->GetMaterialID());
      else
      {
        JsonMaterials::kMaterialType mat_type = JsonMaterials::default_mat_type;
        switch (p_json_obj->GetType())
        {
          case JsonInterjacentObject::vpoint_obj_type:
          case JsonInterjacentObject::point_obj_type:
            mat_type = JsonMaterials::point_mat_type;
          break;
          case JsonInterjacentObject::line_obj_type:
          case JsonInterjacentObject::vline_obj_type:
          case JsonInterjacentObject::line_strip_obj_type:
            mat_type = JsonMaterials::line_basic_mat_type;
          break;
          case JsonInterjacentObject::mesh_obj_type:
            mat_type = JsonMaterials::mesh_basic_mat_type;
          break;
        }

        if (p_json_obj->GetCopyByMaterialID() != 0)
          p_json_obj->SetMaterialID(json_obj->CopyMaterial(p_json_obj->GetCopyByMaterialID(), mat_type));
        else
        {
          p_json_obj->SetMaterialID(json_obj->GetNewMaterialUUID());
          json_obj->AddMaterial(p_json_obj->GetMaterialID(), mat_type);

          p_json_obj->CalculateMaterialLineWeight(json_obj);
        }

        if (old_mat == cur_type_materials.end())
          cur_type_materials.insert(std::pair<JsonInterjacentObject::kJsonObjType, JsonInterjacentObject::JsonInterjacentMat*>(p_json_obj->GetType(), p_json_obj->GetMaterial()));
        else
          old_mat->second = p_json_obj->GetMaterial();

        json_obj->AddMaterialColor(p_json_obj->GetMaterialID(), JsonMaterialColor::main_color, m_rgb);
      }

      //object geometries
      p_json_obj->SetGeometryID(json_obj->GetNewGeometryUUID());
      json_obj->AddGeometries(p_json_obj->GetGeometryID(), JsonGeometries::Geometry);

      if (p_json_obj->GetType() == JsonInterjacentObject::vline_obj_type)
      {
        VObject * p_obj = dynamic_cast<VObject*> (p_json_obj);
        if (p_obj != NULL)
          p_obj->AddVertex(json_obj);
      }
      else if (p_json_obj->GetType() == JsonInterjacentObject::vpoint_obj_type)
      {
        VObject * p_obj = dynamic_cast<VObject*> (p_json_obj);
        if (p_obj != NULL)
          p_obj->AddVertex(json_obj);
      }
      else
      {
        //geometries arrays
        DataArrayObject * p_obj = dynamic_cast<DataArrayObject*> (p_json_obj);
        if (p_obj != NULL)
        {
          p_obj->ParseGeometryArrays(json_obj, pMetafile);

          if (p_obj->GetType() == JsonInterjacentObject::mesh_obj_type)
          {
            FaceObject * p_f_obj = dynamic_cast<FaceObject*> (p_obj);
            if (p_f_obj != NULL)
              p_f_obj->AddFaces(json_obj);
          }
        }
      }

      //objects child
      OdBool obj_visible = true;
      if (p_json_obj->GetGeomMarkerType() == JsonMetafileConverter::gm_3dFacetEdges || p_json_obj->GetGeomMarkerType() == JsonMetafileConverter::gm_2dFacetEdges || p_json_obj->GetGeomMarkerType() == JsonMetafileConverter::gm_2dFacetsNoFill)
        obj_visible = false;
      const char *name = "";
      JsonObjectData::kObjectType obj_type = JsonObjectData::Object3D;
      switch (p_json_obj->GetType())
      {
        case JsonInterjacentObject::vline_obj_type:
          name = "vline";
          obj_type = JsonObjectData::Line;
          break;
        case JsonInterjacentObject::line_obj_type:
          if (p_json_obj->GetGeomMarkerType() == JsonMetafileConverter::gm_3dFacetEdges || p_json_obj->GetGeomMarkerType() == JsonMetafileConverter::gm_2dFacetEdges)
            name = "edge_line";
          else
            name = "line";
          obj_type = JsonObjectData::Line;
          break;
        case JsonInterjacentObject::line_strip_obj_type:
          name = "line_strip";
          obj_type = JsonObjectData::Line;
          break;
        case JsonInterjacentObject::vpoint_obj_type:
          name = "vpoint";
          obj_type = JsonObjectData::Points;
        case JsonInterjacentObject::point_obj_type:
          name = "points";
          obj_type = JsonObjectData::Points;
          break;
        case JsonInterjacentObject::mesh_obj_type:
          name = "mesh";
          obj_type = JsonObjectData::Mesh;
          break;
      }

      json_obj->AddMetafileObject(root_id, name, obj_type, p_json_obj->GetGeometryID(), p_json_obj->GetMaterialID(), obj_visible);
    }

    ++ptr;
  }
}
