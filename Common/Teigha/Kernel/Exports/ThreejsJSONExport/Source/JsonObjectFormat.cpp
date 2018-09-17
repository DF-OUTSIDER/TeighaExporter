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
#include "JsonObjectFormat.h"

/////////////////////////////
class OdGLES2JsonNestingLevel
{
  OdGLES2JsonServer *m_pJson;
  const char *m_pText;
  const OdGLES2JsonServer::JsonType &m_pDel;
public:
  OdGLES2JsonNestingLevel(OdGLES2JsonServer *pJson, const char *pText, const OdGLES2JsonServer::JsonType &pDel)
    : m_pJson(pJson)
    , m_pText(pText)
    , m_pDel(pDel)
  {
    pJson->IncreaseNesting(pText, pDel);
  }
  ~OdGLES2JsonNestingLevel()
  {
    m_pJson->DecreaseNesting(m_pText, m_pDel);
  }
};

#define JSON_LEVEL(nLevel, pText, pDel) \
  OdGLES2JsonNestingLevel _jsonNsL##nLevel(pJson, pText, pDel)
#define JSON_TYPE_LEVEL0(pText) \
  JSON_LEVEL(0, pText, OdGLES2JsonServer::kType)
#define JSON_TYPE_LEVEL(nLevel, pText) \
  JSON_LEVEL(nLevel, pText, OdGLES2JsonServer::kType)

#define JSON_ARR_LEVEL0(pText) \
  JSON_LEVEL(0, pText, OdGLES2JsonServer::kArray)
#define JSON_ARR_LEVEL(nLevel, pText) \
  JSON_LEVEL(nLevel, pText, OdGLES2JsonServer::kArray)

#define JSON_DROP_COMMA() \
  pJson->DropChars(NULL, ",");

#define JSON_DROP_UUID(id)\
  OdString uuid;\
  uuid.format(L"%u", id);\
  pJson->DropString("uuid", uuid);
////////////////////////////////

#define INVERT_RGB(r,g,b) (((OdUInt8)(b)|((OdUInt16)((OdUInt8)(g))<<8))|(((OdUInt32)(OdUInt8)(r))<<16))

//JsonMaterial
OdUInt32 JsonMaterialColor::GetInvertColor() const
{
  return INVERT_RGB(ODGETRED(m_color), ODGETGREEN(m_color), ODGETBLUE(m_color));
}

//JsonObjectData
//

struct JsonCameraObjectData : public JsonObjectData
{
  JsonCameraObjectData()
    : m_fov(0)
    , m_aspect(0)
    , m_near_plan(0)
    , m_far_plan(0)
    , m_left(0)
    , m_right(0)
    , m_top(0)
    , m_bottom(0)
  {}

  JsonCameraObjectData(const char *name, const JsonObjectData::kObjectType &type, const OdGeMatrix3d &matrix)
    : JsonObjectData(name, type, matrix)
    , m_fov(0)
    , m_aspect(0)
    , m_near_plan(0)
    , m_far_plan(0)
    , m_left(0)
    , m_right(0)
    , m_top(0)
    , m_bottom(0)
  {}

  virtual ~JsonCameraObjectData()
  {
  }

  void SetCameraFov(const float &fov) { m_fov = fov; }
  void SetCameraAspectRatio(const float &aspect_ratio) { m_aspect = aspect_ratio; }
  void SetCameraNearPlan(const float &near_plan) { m_near_plan = near_plan; }
  void SetCameraFarPlan(const float &far_plan) { m_far_plan = far_plan; }
  void SetCameraLeft(const float &left) { m_left = left; }
  void SetCameraRight(const float &right) { m_right = right; }
  void SetCameraTop(const float &top) { m_top = top; }
  void SetCameraBottom(const float &bottom) { m_bottom = bottom; }

  const float &GetCameraFov() const { return m_fov; }
  const float &GetCameraAspectRatio() const { return m_aspect; }
  const float &GetCameraNearPlan() const { return m_near_plan; }
  const float &GetCameraFarPlan() const { return m_far_plan; }
  const float &GetCameraLeft() const { return m_left; }
  const float &GetCameraRight() const { return m_right; }
  const float &GetCameraTop() const { return m_top; }
  const float &GetCameraBottom() const { return m_bottom; }

private:
  float m_fov;// Field of view
  float m_aspect;// Aspect ratio
  float m_near_plan;// Near plane
  float m_far_plan;// Far plane

  float m_left;
  float m_right;
  float m_top;
  float m_bottom;
};

struct JsonMetafileObjectData : public JsonObjectData
{
  JsonMetafileObjectData()
    : m_geometry(0)
    , m_material(0)
  {}

  JsonMetafileObjectData(const char *name, const JsonObjectData::kObjectType &type, const OdGeMatrix3d &matrix)
    : JsonObjectData(name, type, matrix)
    , m_geometry(0)
    , m_material(0)
  {}

  const OdUInt64 &GetGeometry() const { return m_geometry; }
  const OdUInt64 &GetMaterial() const { return m_material; }

  void SetGeometry(const OdUInt64& geometry) { m_geometry = geometry; }
  void SetMaterial(const OdUInt64& material) { m_material = material; }

private:
  OdUInt64 m_geometry;
  OdUInt64 m_material;
};

struct JsonLightObjectData : public JsonObjectData
{
  JsonLightObjectData()
    : m_color(0)
  {}

  JsonLightObjectData(const char *name, const JsonObjectData::kObjectType &type, const OdGeMatrix3d &matrix)
    : JsonObjectData(name, type, matrix)
    , m_color(0)
  {}

  const ODCOLORREF &GetColor() const { return m_color; } 

  void SetColor(const ODCOLORREF &color) { m_color = color; }

  OdUInt32 GetInvertColor() const { return INVERT_RGB(ODGETRED(m_color), ODGETGREEN(m_color), ODGETBLUE(m_color)); }

private:
  ODCOLORREF m_color;
};

void JsonObjectData::AddObject(const OdUInt64 &id)
{
  m_children.push_back(id);
}

//JsonObjectFormat

void JsonObjectFormat::SetMeataData(const float &version, const char *type, const char *generator)
{
  m_metadata.version = version;
  m_metadata.type = type;
  m_metadata.generator = generator;
}

struct pred
{
  pred(const OdGeMatrix3d &matrix, OdBool is_matrix_for_child, JsonObjectFormat *obj)
    : mat(matrix)
    , matrix_for_child(is_matrix_for_child)
    , jobj(obj)
  {
  }
  void operator()(const OdUInt64 &obj_id)
  {
    jobj->SetObjectMatrix(obj_id, mat, matrix_for_child);
  }
protected:
  const OdGeMatrix3d &mat;
  OdBool matrix_for_child;
  JsonObjectFormat *jobj;
};

void JsonObjectFormat::SetObjectMatrix(const OdUInt64 &obj_id, const OdGeMatrix3d &matrix, OdBool matrix_for_child)
{
  JsonObjectData *const pobj = GetObjectChild(obj_id);
  if (pobj != NULL)
  {
    pobj->SetMatrix(matrix);
    if (matrix_for_child && pobj->GetChildrenCount() > 0)
    {
      pred fe_pred(matrix, matrix_for_child, this);

      pobj->ForEachChildren<pred>(&fe_pred);
    }
  }
}

void JsonObjectFormat::SetLensWeightByIndx(const OdUInt8Array &arr)
{
  if (m_camera_options != NULL)
  {
    for (OdUInt16 n = 0; n < arr.size(); n++)
    {
      std::map<OdInt16, OdUInt64>::iterator it = m_camera_options->m_lens_weight_indexs.find(n);
      if (it != m_camera_options->m_lens_weight_indexs.end())
      {
        JsonMaterials *const m = GetMaterial(it->second);
        if (m != NULL)
          m->SetWidth(arr.getPtr()[n]);
      }
    }
  }
}

void JsonObjectFormat::SetCameraMetafileMode()
{
  if (m_camera_options != NULL)
  {
    if (m_camera_options->m_mode == CameraViewOptions::default_camera_mode)
      m_camera_options->m_mode = CameraViewOptions::unique_camera_mode;
    else if (m_camera_options->m_mode == CameraViewOptions::unique_camera_mode)
    {
      delete m_camera_options;
      m_camera_options = NULL;
    }
  }
}

void JsonObjectFormat::SetCameraMetafileMatrix(const OdGeMatrix3d &matrix)
{
  if (m_camera_options != NULL)
    m_camera_options->m_camera_metafile_matrix = matrix;
}

void JsonObjectFormat::SetCameraMetafileMatrixToObject(const OdUInt64 &id)
{
  if (m_camera_options != NULL)
    SetObjectMatrix(id, m_camera_options->m_camera_metafile_matrix, true);
}

OdUInt64 JsonObjectFormat::GetNewObjectUUID() const
{
  static OdUInt64 uuid = 1;
  if (m_objects.find(++uuid) == m_objects.end())
    return uuid;
  else
    return GetNewObjectUUID();
}

OdUInt64 JsonObjectFormat::GetNewMaterialUUID() const
{
  static OdUInt64 uuid = 1;
  if (m_materials.find(++uuid) == m_materials.end())
    return uuid;
  else
    return GetNewMaterialUUID();
}

OdUInt64 JsonObjectFormat::GetNewGeometryUUID() const
{
  static OdUInt64 uuid = 1;
  if (m_geometries.find(++uuid) == m_geometries.end())
    return uuid;
  else
    return GetNewGeometryUUID();
}

const OdGeMatrix3d *const JsonObjectFormat::GetMatrix() const
{
  const JsonObjectData *const obj = GetObjectChild(m_scene_uuid);
  if (obj != NULL)
    return &(obj->GetMatrix());
  else
    return NULL;
}

JsonObjectData *const JsonObjectFormat::GetObjectChild(const OdUInt64 &id)
{
  std::map<OdUInt64, JsonObjectDataPtr>::iterator obj = m_objects.find(id);
  if (obj != m_objects.end())
    return obj->second.get();
  else
    return NULL;
}

const JsonObjectData *const JsonObjectFormat::GetObjectChild(const OdUInt64 &id) const
{
  std::map<OdUInt64, JsonObjectDataPtr>::const_iterator obj = m_objects.find(id);
  if (obj != m_objects.end())
    return obj->second.get();
  else
    return NULL;
}

JsonGeometries *const JsonObjectFormat::GetGeomertry(const OdUInt64& id)
{
  std::map<OdUInt64, JsonGeometries>::iterator g = m_geometries.find(id);
  if (g != m_geometries.end())
    return &(g->second);
  else
    return NULL;
}

JsonMaterials *const JsonObjectFormat::GetMaterial(const OdUInt64& id)
{
  std::map<OdUInt64, JsonMaterials>::iterator m = m_materials.find(id);
  if (m != m_materials.end())
    return &(m->second);
  else
    return NULL;
}

struct recalc_for_each_pred
{
  recalc_for_each_pred(JsonObjectFormat *const pj, const double &c)
    : pJobj(pj)
    , coef(c)
  {}
  void operator()(const std::pair<double, OdUInt64> &lens)
  {
    JsonMaterials *const m = pJobj->GetMaterial(lens.second);
    if (m != NULL)
      m->SetWidth(lens.first * coef);
  }

private:
  JsonObjectFormat *const pJobj;
  const double &coef;
};

void JsonObjectFormat::RecalcLensWeight(const double &coef)
{
  if (m_camera_options != NULL)
  {
    recalc_for_each_pred recalc_pred(this, coef);

    std::for_each(m_camera_options->m_lens_weight.begin(), m_camera_options->m_lens_weight.end(), recalc_pred);
  }
}

void JsonObjectFormat::AddMaterial(const OdUInt64 &uuid, const JsonMaterials::kMaterialType &type, const float &opacity)
{
  m_materials.insert(std::pair<OdUInt64, JsonMaterials>(uuid, JsonMaterials(type, opacity)));
}

OdBool JsonObjectFormat::AddMaterialColor(const OdUInt64& id, const JsonMaterialColor::kColorType &type, const ODCOLORREF &color)
{
  JsonMaterials *const m = GetMaterial(id);
  if (m != NULL)
  {
    m->AddMaterialColor(JsonMaterialColor(type, color));
    return true;
  }
  else
    return false;
}

OdBool JsonObjectFormat::AddMaterialWidth(const OdUInt64& id, float w)
{
  JsonMaterials *const m = GetMaterial(id);
  if (m != NULL)
  {
    m->SetWidth(w);
    return true;
  }
  else
    return false;
}

OdUInt64 JsonObjectFormat::CopyMaterial(const OdUInt64& id, const JsonMaterials::kMaterialType &new_type)
{
  OdUInt64 res = 0;
  JsonMaterials *const mat = GetMaterial(id);
  if (mat != NULL)
  {
    std::pair<std::map<OdUInt64, JsonMaterials>::iterator, bool> m = m_materials.insert(std::pair<OdUInt64, JsonMaterials>(GetNewMaterialUUID(), JsonMaterials(*mat, new_type)));
    if (m.second)
      res = m.first->first;
  }

  return res;
}

void JsonObjectFormat::AddMaterialLenWeightIndx(const OdUInt64 &mat_id, const OdInt16 &lens_ind)
{
  if (m_camera_options != NULL)
    m_camera_options->m_lens_weight_indexs.insert(std::pair<OdInt16, OdUInt64>(lens_ind, mat_id));
}

void JsonObjectFormat::AddMaterialLenWeight(const OdUInt64 &mat_id, const double &len_weight)
{
  if (m_camera_options != NULL)
    m_camera_options->m_lens_weight.push_back(std::pair<double, OdUInt64>(len_weight, mat_id));
}

void JsonObjectFormat::AddGeometries(const OdUInt64 &uuid, const JsonGeometries::kGeometriesType &type)
{
  m_geometries.insert(std::pair<OdUInt64, JsonGeometries>(uuid, JsonGeometries(type)));
}

JsonObjectDataPtr JsonObjectFormat::MakeObjectData(const JsonObjectData::kObjectType &type, const char *name, const OdGeMatrix3d &matrix)
{
  switch (type)
  {
  case JsonObjectData::Scene:
  case JsonObjectData::Object3D:
    return JsonObjectDataPtr(new JsonObjectData(name, type, matrix));
  case JsonObjectData::Points:
  case JsonObjectData::Line:
  case JsonObjectData::Mesh:
    return JsonObjectDataPtr(new JsonMetafileObjectData(name, type, matrix));
  case JsonObjectData::PerspectiveCamera:
  case JsonObjectData::OrthographicCamera:
    return JsonObjectDataPtr(new JsonCameraObjectData(name, type, matrix));
  case JsonObjectData::AmbientLight:
  case JsonObjectData::PointLight:
  case JsonObjectData::DirectionalLight:
  case JsonObjectData::SpotLight:
    return JsonObjectDataPtr(new JsonLightObjectData(name, type, matrix));
  }

  return JsonObjectDataPtr();
}

JsonObjectData *const JsonObjectFormat::AddObject(const OdUInt64 &root_id, const OdUInt64 &id, const char *name, const JsonObjectData::kObjectType &type)
{
  JsonObjectData *const root_obj = GetObjectChild(root_id);
  if (root_obj != NULL)
  {
    JsonObjectDataPtr jobj = MakeObjectData(type, name, root_obj->GetMatrix());
    std::pair<std::map<OdUInt64, JsonObjectDataPtr>::iterator, bool> ins_it = m_objects.insert(std::pair<OdUInt64, JsonObjectDataPtr>(id, jobj));
    if (ins_it.second)
      root_obj->AddObject(id);
    return ins_it.first->second.get();
  }
  else
    return NULL;
}

JsonObjectData *const JsonObjectFormat::AddObjectToScene(const OdUInt64 &id, const char *name, const JsonObjectData::kObjectType &type)
{
  return AddObject(m_scene_uuid, id, name, type);
}

OdBool JsonObjectFormat::AddMetafileObject(const OdUInt64 &root_id, const char *name, const JsonObjectData::kObjectType &type, const OdUInt64 &geometry_id, const OdUInt64 &material_id, OdBool visible)
{
  const OdUInt64 &id = GetNewObjectUUID();
  JsonObjectData *const obj = AddObject(root_id, id, name, type);

  if (obj)
  {
    JsonMetafileObjectData *met_obj = dynamic_cast<JsonMetafileObjectData*>(obj);
    if (met_obj != NULL)
    {
      met_obj->SetGeometry(geometry_id);
      met_obj->SetMaterial(material_id);
      met_obj->SetVisible(visible);
      return true;
    }
  }

  return false;
}

OdBool JsonObjectFormat::AddLayer(const OdUInt64 &layer_id, const char *name)
{
  return AddObjectToScene(layer_id, name) ? true : false;
}

OdBool JsonObjectFormat::AddObjectByLayer(const OdUInt64 &layer_id, const OdUInt64 &id)
{
  JsonObjectData *const obj = layer_id ? AddObject(layer_id, id) : AddObjectToScene(id);
  return obj == NULL ? false : true;
}

//add light
OdBool JsonObjectFormat::AddAmbientLight(const OdUInt64 &root_id, const ODCOLORREF &color)
{
  const OdUInt64 &light_uuid = GetNewObjectUUID();
  JsonObjectData *const obj = AddObject(root_id, light_uuid, "AmbientLight", JsonObjectData::AmbientLight);
  if (obj)
  {
    JsonLightObjectData *light_obj = dynamic_cast<JsonLightObjectData*>(obj);
    if (light_obj != NULL)
    {
      light_obj->SetColor(color);
      return true;
    }
  }

  return false;
}

OdBool JsonObjectFormat::AddDirectionLight(const OdUInt64 &root_id, const ODCOLORREF &color, const OdGeVector3d &direction)
{
  const OdUInt64 &dl_uuid = GetNewObjectUUID();
  JsonObjectData *const obj = AddObject(root_id, dl_uuid, "DirectionalLight", JsonObjectData::DirectionalLight);
  if (obj)
  {
    JsonLightObjectData *light_obj = dynamic_cast<JsonLightObjectData*>(obj);
    if (light_obj != NULL)
    {
      light_obj->SetColor(color);
      JsonObjectData *const root_obj = GetObjectChild(root_id);
      if (root_obj != NULL)
        light_obj->SetMatrix(root_obj->GetMatrix() * direction);
      return true;
    }
  }

  return false;
}

OdBool JsonObjectFormat::AddPointLight(const OdUInt64 &root_id, const ODCOLORREF &color, const OdGePoint3d &position)
{
  const OdUInt64 &pl_uuid = GetNewObjectUUID();
  JsonObjectData *const obj = AddObject(root_id, pl_uuid, "PointLight", JsonObjectData::PointLight);
  if (obj)
  {
    JsonLightObjectData *light_obj = dynamic_cast<JsonLightObjectData*>(obj);
    if (light_obj != NULL)
    {
      light_obj->SetColor(color);
      light_obj->SetMatrix(OdGeMatrix3d().setCoordSystem(position, OdGeVector3d::kXAxis, OdGeVector3d::kYAxis, OdGeVector3d::kZAxis));
      return true;
    }
  }

  return false;
}

OdBool JsonObjectFormat::AddSpotLight(const OdUInt64 &root_id, const ODCOLORREF &color, const OdGePoint3d &position, const OdGeVector3d &direction)
{
  const OdUInt64 &pl_uuid = GetNewObjectUUID();
  JsonObjectData *const obj = AddObject(root_id, pl_uuid, "SpotLight", JsonObjectData::SpotLight);
  if (obj)
  {
    JsonLightObjectData *light_obj = dynamic_cast<JsonLightObjectData*>(obj);
    if (light_obj != NULL)
    {
      light_obj->SetColor(color);
      OdGeVector3d xAxis = direction.crossProduct(OdGeVector3d::kYAxis);
      OdGeVector3d yAxis = direction.crossProduct(xAxis);
      light_obj->SetMatrix(OdGeMatrix3d().setCoordSystem(position, xAxis, yAxis, direction));
      return true;
    }
  }

  return false;
}

//add camera
OdBool JsonObjectFormat::AddPerspectiveCamera(const OdUInt64 &id, const float &aspect_ratio, const OdGeMatrix3d &matr, const float &far_plan, const float &near_plan)
{
  JsonObjectData *const obj = AddObjectToScene(id, "camera", JsonObjectData::PerspectiveCamera);
  if (obj)
  {
    JsonCameraObjectData *camera_obj = dynamic_cast<JsonCameraObjectData*>(obj);
    if (camera_obj != NULL)
    {
      camera_obj->SetCameraAspectRatio(aspect_ratio);
      camera_obj->SetMatrix(matr);
      camera_obj->SetCameraFarPlan(far_plan);
      camera_obj->SetCameraNearPlan(near_plan);
      return true;
    }
  }

  return false;
}

OdBool JsonObjectFormat::AddOrthographicCamera(const OdUInt64 &id, const double &width, const double &height, const OdGeMatrix3d &matr, const float &far_plan, const float &near_plan)
{
  JsonObjectData *const obj = AddObjectToScene(id, "camera", JsonObjectData::OrthographicCamera);
  if (obj)
  {
    JsonCameraObjectData *camera_obj = dynamic_cast<JsonCameraObjectData*>(obj);
    if (camera_obj != NULL)
    {
      camera_obj->SetCameraLeft(width / -2);
      camera_obj->SetCameraRight(width / 2);
      camera_obj->SetCameraBottom(height / -2);
      camera_obj->SetCameraTop(height / 2);
      camera_obj->SetMatrix(matr);
      camera_obj->SetCameraFarPlan(far_plan);
      camera_obj->SetCameraNearPlan(near_plan);
      return true;
    }
  }

  return false;
}
////

/////////////
//methods of drop data to JsonServer
void JsonObjectFormat::DropMetadata() const
{
  JSON_TYPE_LEVEL(nLevel, "metadata");
  pJson->DropFloat("version", m_metadata.version);
  JSON_DROP_COMMA() 
  pJson->DropString("type", m_metadata.type);
  JSON_DROP_COMMA()
  pJson->DropString("generator", m_metadata.generator);
}

struct mat_for_each_pred
{
  mat_for_each_pred(OdGLES2JsonServer* j)
    : pJson(j)
  {}
  void operator ()(const JsonMaterialColor &mat_col)
  {
    const OdUInt32 &col = mat_col.GetInvertColor();
    switch (mat_col.GetType())
    {
      case JsonMaterialColor::main_color:
        JSON_DROP_COMMA()
        pJson->DropUInt32("color", col);
      break;
      case JsonMaterialColor::ambient_color:
        JSON_DROP_COMMA()
        pJson->DropUInt32("ambient", col);
      break;
      case JsonMaterialColor::emessive_color:
        JSON_DROP_COMMA()
        pJson->DropUInt32("emessive", col);
      break;
      case JsonMaterialColor::specular_color:
        JSON_DROP_COMMA()
        pJson->DropUInt32("specular", col);
      break;
    }
  }
private:
  OdGLES2JsonServer* pJson;
};

void JsonObjectFormat::DropMaterials() const
{
  JSON_ARR_LEVEL(nLevel, "materials");
  ++nLevel;
  std::map<OdUInt64, JsonMaterials>::const_iterator pMat = m_materials.begin();

  while (pMat != m_materials.end())
  {
    const JsonMaterials &cur_mat = pMat->second;
    if (cur_mat.GetType() == JsonMaterials::default_mat_type)
    {
      ++pMat;
      continue;
    }

    if (pMat != m_materials.begin())
      JSON_DROP_COMMA()
    JSON_TYPE_LEVEL(nLevel, "");
    ++nLevel;

    JSON_DROP_UUID(pMat->first)

    mat_for_each_pred mat_col(pJson);

    switch (cur_mat.GetType())
    {
      case JsonMaterials::point_mat_type:
      {
        JSON_DROP_COMMA()
        pJson->DropString("type", "PointsMaterial");
        cur_mat.ForEachColor<mat_for_each_pred>(&mat_col);
        JSON_DROP_COMMA()
        pJson->DropFloat("size", cur_mat.GetWidth());
        JSON_DROP_COMMA()
        pJson->DropBoolean("sizeAttenuation", false);
      }
      break;
      case JsonMaterials::line_basic_mat_type:
      {
        JSON_DROP_COMMA()
        pJson->DropString("type", "LineBasicMaterial");
        cur_mat.ForEachColor<mat_for_each_pred>(&mat_col);
        JSON_DROP_COMMA()
        pJson->DropFloat("linewidth", cur_mat.GetWidth());
      }
      break;
      case JsonMaterials::mesh_basic_mat_type:
      {
        JSON_DROP_COMMA()
        pJson->DropString("type", "MeshPhongMaterial");
        JSON_DROP_COMMA()
        pJson->DropFloat("opacity", cur_mat.GetOpacity());
        cur_mat.ForEachColor<mat_for_each_pred>(&mat_col);
      }
      break;
    }

    --nLevel;
    ++pMat;
  }

  --nLevel;
}
struct geomerty_for_each_pred
{
  geomerty_for_each_pred(OdGLES2JsonServer *json, OdUInt32 *l)
    : pJson(json)
    , is_first(true)
    , nLevel(l)
  {}

  void operator()(const JsonGeometriesDataPtr &geom_attr)
  {
    if (is_first)
      is_first = false;
    else
      JSON_DROP_COMMA()

    const char *type = "";
    switch (geom_attr->GetDataType())
    {
      case JsonGeometriesDataBase::normal:
        type = "normals";
        break;
      case JsonGeometriesDataBase::vertec:
        type = "vertices";
        break;
      case JsonGeometriesDataBase::face:
        type = "faces";
        break;
      case JsonGeometriesDataBase::color:
        type = "colors";
        break;
    }

    OdUInt32 level = (*nLevel);
    JSON_TYPE_LEVEL(level, type);
    ++(*nLevel);

    if (dynamic_cast<const JsonGeometriesData<float>* >(geom_attr.get()) != NULL)
    {
      const OdUInt32 &nData = (geom_attr.get())->GetSize() / sizeof(float);
      pJson->DropUInt32("size", nData / 3);
      JSON_DROP_COMMA()
      pJson->DropString("type", "Float32Array");
      JSON_DROP_COMMA()
      pJson->DropFloats("array", nData, dynamic_cast<const JsonGeometriesData<float>* >(geom_attr.get())->GetArray());
    }
    else if (dynamic_cast<const JsonGeometriesData<OdUInt32>* >(geom_attr.get()) != NULL)
    {
      const OdUInt32 &nData = (geom_attr.get())->GetSize() / sizeof(OdUInt32);
      pJson->DropUInt32("size", nData);
      JSON_DROP_COMMA()
      pJson->DropString("type", "UInt32Array");
      JSON_DROP_COMMA()
      pJson->DropUInts("array", nData, dynamic_cast<const JsonGeometriesData<OdUInt32>* >(geom_attr.get())->GetArray());
    }
    else if (dynamic_cast<const JsonGeometriesData<OdUInt16>* >(geom_attr.get()) != NULL)
    {
      const OdUInt32 &nData = (geom_attr.get())->GetSize() / sizeof(OdUInt16);
      pJson->DropUInt32("size", nData);
      JSON_DROP_COMMA()
      pJson->DropString("type", "UInt16Array");
      JSON_DROP_COMMA()
      pJson->DropInts("array", nData, dynamic_cast<const JsonGeometriesData<OdUInt16>* >(geom_attr.get())->GetArray());
    }

    --(*nLevel);
  }

private:
  OdGLES2JsonServer* pJson;
  OdUInt32 *nLevel;
  OdBool is_first;
};

struct geometry2_for_each_pred
{
  geometry2_for_each_pred(OdGLES2JsonServer *json, OdBool *f, OdBool *v)
    : pJson(json)
    , is_first(true)
    , withFaces(f)
    , withVertices(v)
  {}

  void operator()(const JsonGeometriesDataPtr &geom_attr)
  {
    if (is_first)
      is_first = false;
    else
      JSON_DROP_COMMA()

    const char *type = NULL;
    switch (geom_attr->GetDataType())
    {
      case JsonGeometriesDataBase::normal:
        type = "normals";
        break;
      case JsonGeometriesDataBase::vertec:
        type = "vertices";
        *withVertices = true;
        break;
      case JsonGeometriesDataBase::face:
        type = "faces";
        *withFaces = true;
        break;
      case JsonGeometriesDataBase::color:
        type = "colors";
        break;
    }

    if (dynamic_cast<const JsonGeometriesData<float>* >(geom_attr.get()) != NULL)
    {
      const OdUInt32 &nData = (geom_attr.get())->GetSize() / sizeof(float);
      pJson->DropFloats(type, nData, dynamic_cast<const JsonGeometriesData<float>* >(geom_attr.get())->GetArray());
    }
    else if (dynamic_cast<const JsonGeometriesData<OdUInt32>* >(geom_attr.get()) != NULL)
    {
      const OdUInt32 &nData = (geom_attr.get())->GetSize() / sizeof(OdUInt32);
      pJson->DropUInts(type, nData, dynamic_cast<const JsonGeometriesData<OdUInt32>* >(geom_attr.get())->GetArray());
    }
  }

private:
  OdGLES2JsonServer* pJson;
  OdBool is_first;
  OdBool *withFaces;
  OdBool *withVertices;
};

void JsonObjectFormat::DropGeometries() const
{
  JSON_ARR_LEVEL(nLevel, "geometries");
  ++nLevel;
  std::map<OdUInt64, JsonGeometries>::const_iterator pGeom = m_geometries.begin();

  while (pGeom != m_geometries.end())
  {
    if (pGeom != m_geometries.begin())
      JSON_DROP_COMMA()
    JSON_TYPE_LEVEL(nLevel, "");
    ++nLevel;

    JSON_DROP_UUID(pGeom->first)
    JSON_DROP_COMMA()

    const JsonGeometries &cur_geom = pGeom->second;

    if (cur_geom.GetType() == JsonGeometries::BufferGeometry)
    {
      pJson->DropString("type", "BufferGeometry");
      JSON_DROP_COMMA()
      JSON_TYPE_LEVEL(nLevel, "data");
      ++nLevel;

      geomerty_for_each_pred geometry_pred(pJson, &nLevel);

      cur_geom.ForEachAttr<geomerty_for_each_pred>(&geometry_pred);

      --nLevel;
    }
    else if (cur_geom.GetType() == JsonGeometries::Geometry)
    {
      pJson->DropString("type", "Geometry");
      JSON_DROP_COMMA()
      JSON_TYPE_LEVEL(nLevel, "data");
      ++nLevel;

      OdBool withFaces = false;
      OdBool withVertices = false;

      geometry2_for_each_pred geometry_pred(pJson, &withFaces, &withVertices);

      cur_geom.ForEachAttr<geometry2_for_each_pred>(&geometry_pred);

      if (!withFaces)
      {
        if (cur_geom.GetAttributeCount())
          JSON_DROP_COMMA()
        JSON_ARR_LEVEL(nLevel, "faces");
      }
      if (!withVertices)
      {
        JSON_DROP_COMMA()
        JSON_ARR_LEVEL(nLevel, "vertices");
      }

      --nLevel;
    }

    --nLevel;
    ++pGeom;
  }

  --nLevel;
}

void JsonObjectFormat::DropObjectData(const JsonObjectData *obj) const
{
  if (obj != NULL)
  {
    OdString uuid;

    switch (obj->GetType())
    {
      case JsonObjectData::Scene:
      {
        JSON_DROP_COMMA()
        pJson->DropString("name", obj->GetName());
        JSON_DROP_COMMA()
        pJson->DropString("type", "Scene");
        JSON_DROP_COMMA()
        pJson->DropUInt32("background", INVERT_RGB(ODGETRED(m_background_color), ODGETGREEN(m_background_color), ODGETBLUE(m_background_color)));
        if (obj->GetChildrenCount() > 0)
        {
          JSON_DROP_COMMA()
          DropObjectChildren(obj);
        }
        break;
      }
      case JsonObjectData::Object3D:
      {
        JSON_DROP_COMMA()
        pJson->DropString("name", obj->GetName());
        JSON_DROP_COMMA()
        pJson->DropString("type", "Object3D");
        JSON_DROP_COMMA()
        pJson->DropMatrix("matrix", obj->GetMatrix());
        if (obj->GetChildrenCount() > 0)
        {
          JSON_DROP_COMMA()
          DropObjectChildren(obj);
        }
        break;
      }
      case JsonObjectData::Points:
      {
        const JsonMetafileObjectData *p_met_obj = dynamic_cast<const JsonMetafileObjectData*>(obj);
        if (p_met_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_met_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "Points");
          if (p_met_obj->GetGeometry())
          {
            JSON_DROP_COMMA()
            uuid.format(L"%u", p_met_obj->GetGeometry());
            pJson->DropString("geometry", uuid);
          }

          if (p_met_obj->GetMaterial())
          {
            JSON_DROP_COMMA()
            uuid.format(L"%u", p_met_obj->GetMaterial());
            pJson->DropString("material", uuid);
          }

          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_met_obj->GetMatrix());
        }
        break;
      }
      case JsonObjectData::Mesh:
      {
        const JsonMetafileObjectData *p_met_obj = dynamic_cast<const JsonMetafileObjectData*>(obj);
        if (p_met_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_met_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "Mesh");
          if (p_met_obj->GetGeometry())
          {
            JSON_DROP_COMMA()
            uuid.format(L"%u", p_met_obj->GetGeometry());
            pJson->DropString("geometry", uuid);
          }

          if (p_met_obj->GetMaterial())
          {
            JSON_DROP_COMMA()
            uuid.format(L"%u", p_met_obj->GetMaterial());
            pJson->DropString("material", uuid);
          }
          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_met_obj->GetMatrix());

          if (!p_met_obj->isVisible())
          {
            JSON_DROP_COMMA()
            pJson->DropBoolean("visible", p_met_obj->isVisible());
          }
        }
        break;
      }
      case JsonObjectData::Line:
      {
        const JsonMetafileObjectData *p_met_obj = dynamic_cast<const JsonMetafileObjectData*>(obj);
        if (p_met_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_met_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "LineSegments");
          if (p_met_obj->GetGeometry())
          {
            JSON_DROP_COMMA()
            uuid.format(L"%u", p_met_obj->GetGeometry());
            pJson->DropString("geometry", uuid);
          }

          if (p_met_obj->GetMaterial())
          {
            JSON_DROP_COMMA()
            uuid.format(L"%u", p_met_obj->GetMaterial());
            pJson->DropString("material", uuid);
          }
          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_met_obj->GetMatrix());
          JSON_DROP_COMMA()
          pJson->DropBoolean("visible", p_met_obj->isVisible());
        }
        break;
      }
      case JsonObjectData::PerspectiveCamera:
      {
        const JsonCameraObjectData *p_cam_obj = dynamic_cast<const JsonCameraObjectData*>(obj);
        if (p_cam_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_cam_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "PerspectiveCamera");
          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_cam_obj->GetMatrix());
          JSON_DROP_COMMA()
          pJson->DropFloat("fov", p_cam_obj->GetCameraFov());
          JSON_DROP_COMMA()
          pJson->DropFloat("aspect", p_cam_obj->GetCameraAspectRatio());
          JSON_DROP_COMMA()
          pJson->DropFloat("near", p_cam_obj->GetCameraNearPlan());
          JSON_DROP_COMMA()
          pJson->DropFloat("far", p_cam_obj->GetCameraFarPlan());

          if (p_cam_obj->GetChildrenCount() > 0)
          {
            JSON_DROP_COMMA()
            DropObjectChildren(p_cam_obj);
          }
        }
        break;
      }
      case JsonObjectData::OrthographicCamera:
      {
        const JsonCameraObjectData *p_cam_obj = dynamic_cast<const JsonCameraObjectData*>(obj);
        if (p_cam_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_cam_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "OrthographicCamera");
          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_cam_obj->GetMatrix());
          JSON_DROP_COMMA()
          pJson->DropFloat("left", p_cam_obj->GetCameraLeft());
          JSON_DROP_COMMA()
          pJson->DropFloat("right", p_cam_obj->GetCameraRight());
          JSON_DROP_COMMA()
          pJson->DropFloat("top", p_cam_obj->GetCameraTop());
          JSON_DROP_COMMA()
          pJson->DropFloat("bottom", p_cam_obj->GetCameraBottom());
          JSON_DROP_COMMA()
          pJson->DropFloat("near", p_cam_obj->GetCameraNearPlan());
          JSON_DROP_COMMA()
          pJson->DropFloat("far", p_cam_obj->GetCameraFarPlan());

          if (p_cam_obj->GetChildrenCount() > 0)
          {
            JSON_DROP_COMMA()
            DropObjectChildren(p_cam_obj);
          }
        }
        break;
      }
      case JsonObjectData::AmbientLight: 
      {
        const JsonLightObjectData *p_light_obj = dynamic_cast<const JsonLightObjectData*>(obj);
        if (p_light_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_light_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "AmbientLight");
          JSON_DROP_COMMA()
          pJson->DropUInt32("color", p_light_obj->GetInvertColor());
        }
        break;
      }
      case JsonObjectData::PointLight:
      {
        const JsonLightObjectData *p_light_obj = dynamic_cast<const JsonLightObjectData*>(obj);
        if (p_light_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_light_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "PointLight");
          JSON_DROP_COMMA()
          pJson->DropUInt32("color", p_light_obj->GetInvertColor());
          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_light_obj->GetMatrix());
        }
        break;
      }
      case JsonObjectData::DirectionalLight:
      {
        const JsonLightObjectData *p_light_obj = dynamic_cast<const JsonLightObjectData*>(obj);
        if (p_light_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_light_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "DirectionalLight");
          JSON_DROP_COMMA()
          pJson->DropUInt32("color", p_light_obj->GetInvertColor());
          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_light_obj->GetMatrix());
        }
        break;
      }
      case JsonObjectData::SpotLight:
      {
        const JsonLightObjectData *p_light_obj = dynamic_cast<const JsonLightObjectData*>(obj);
        if (p_light_obj != NULL)
        {
          JSON_DROP_COMMA()
          pJson->DropString("name", p_light_obj->GetName());
          JSON_DROP_COMMA()
          pJson->DropString("type", "SpotLight");
          JSON_DROP_COMMA()
          pJson->DropUInt32("color", p_light_obj->GetInvertColor());
          JSON_DROP_COMMA()
          pJson->DropMatrix("matrix", p_light_obj->GetMatrix());
        }
        break;
      }
    }
  }
}

struct drop_for_each_pred
{
    drop_for_each_pred(const JsonObjectFormat *const jof, OdGLES2JsonServer * const j, OdUInt32 *l)
      : obj_format(jof)
      , pJson(j)
      , nLevel(l)
      , is_first(true)
    {}
    void operator()(const OdUInt64 &children_id)
    {
      const JsonObjectData *data = obj_format->GetObjectChild(children_id);
      if (data != NULL)
      {
        if (is_first)
          is_first = false;
        else
          JSON_DROP_COMMA()
        OdUInt32 l = *nLevel;
        JSON_TYPE_LEVEL(l, "");
        ++(*nLevel);
        JSON_DROP_UUID(children_id)
        obj_format->DropObjectData(data);
        --(*nLevel);
      }
    }
  private:
    const JsonObjectFormat *const obj_format;
    OdGLES2JsonServer* const pJson;
    OdUInt32 *nLevel;
    OdBool is_first;
};

void JsonObjectFormat::DropObjectChildren(const JsonObjectData *obj) const
{
  JSON_ARR_LEVEL(nLevel, "children");
  ++nLevel;

  drop_for_each_pred drop_children(this, pJson, &nLevel);

  obj->ForEachChildren<drop_for_each_pred>(&drop_children);

  --nLevel;
}

void JsonObjectFormat::DropObject() const
{
  JSON_TYPE_LEVEL0("");
  DropMetadata();
  JSON_DROP_COMMA()
  DropMaterials();
  JSON_DROP_COMMA()
  DropGeometries();
  JSON_DROP_COMMA()

  JSON_TYPE_LEVEL(nLevel, "object");
  ++nLevel;
  const JsonObjectData *const obj = GetObjectChild(m_scene_uuid);
  if (obj != NULL)
  {
    JSON_DROP_UUID(m_scene_uuid)
    DropObjectData(obj);
  }
}
