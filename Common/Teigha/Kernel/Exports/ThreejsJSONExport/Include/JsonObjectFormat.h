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
// JsonObjectFormat interface for GLES2(?) JSON device

#ifndef JSON_OBJECT_FORMAT
#define JSON_OBJECT_FORMAT

#include "OdString.h"
#define STL_USING_LIST
#define STL_USING_ALGORITHM
#include "OdaSTL.h"
#include "JsonServer.h"

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
struct JsonMetadata
{
  JsonMetadata()
    : version(0)
    , type("")
    , generator("")
  {}

  float version;
  OdString type;
  OdString generator;
};

//JsonGeometries

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
struct JsonGeometriesDataBase : public OdRxObject
{
  enum kDataType
  {
    normal = 0,
    vertec,
    uvs,
    face,
    color,
    index
  };

  enum kBufferCopyType
  {
    copy_buffer = 0,
    move_buffer
  };

  JsonGeometriesDataBase() {}
  JsonGeometriesDataBase(const OdUInt32 &size, const kDataType& data_type)
    : m_size(size)
    , m_data_type(data_type)
  {}
  virtual ~JsonGeometriesDataBase() {}

  void addRef() { }
  void release() { }

  const kDataType &GetDataType() const { return m_data_type; }
  const OdUInt32 &GetSize() const { return m_size; }

  ODRX_USING_HEAP_OPERATORS(OdRxObject);

protected:
  OdUInt32 m_size;
  kDataType m_data_type;
};
typedef OdSmartPtr<JsonGeometriesDataBase> JsonGeometriesDataPtr;

//JsonGeometries

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
template <typename T>
struct JsonGeometriesData : JsonGeometriesDataBase
{
  JsonGeometriesData()
    : data_array(NULL)
  {}
  JsonGeometriesData(const OdUInt32 &size, const void* source_array, const kDataType& data_type, const kBufferCopyType& copy_buffer_type)
    : JsonGeometriesDataBase(size, data_type)
    , data_array(NULL)
  {
    if (copy_buffer_type == JsonGeometriesDataBase::move_buffer)
      data_array = (T*)source_array;
    else
    {
      try
      {
        OdUInt32 nData = size / sizeof(T);
        data_array = new T[nData];
        memcpy((void*)data_array, source_array, size);
      }
      catch (...)
      {
        if (data_array != NULL)
          delete data_array;
        throw;
      }
    }
  }

  virtual ~JsonGeometriesData()
  {
    delete[] data_array;
  }

  const T *GetArray() const { return data_array; }

protected:
  T* data_array;
};

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
struct JsonGeometries
{
  enum kGeometriesType
  {
    Geometry = 0,
    BufferGeometry
  };

  JsonGeometries()
    : m_type(Geometry)
    , m_data_attribute()
  {}
  JsonGeometries(const kGeometriesType &type)
    : m_type(type)
    , m_data_attribute()
  {}

  const kGeometriesType &GetType() const { return m_type; }
  OdUInt32 GetAttributeCount() const { return (OdUInt32)m_data_attribute.size(); }

  template <typename T>
  void AddData(const OdUInt32 &size, const void* data, const JsonGeometriesDataBase::kDataType &data_type, const JsonGeometriesDataBase::kBufferCopyType &copy_type)
  {
    m_data_attribute.push_back(JsonGeometriesDataPtr(new JsonGeometriesData<T>(size, data, data_type, copy_type))); 
  }

  template <typename T>
  void ForEachAttr(T *obj) const
  {
    std::for_each(m_data_attribute.begin(), m_data_attribute.end(), *obj);
  }

protected:
  kGeometriesType m_type;
  std::list<JsonGeometriesDataPtr> m_data_attribute;
};

//JsonMaterials

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
struct JsonMaterialColor 
{
  enum kColorType
  {
    main_color = 0,
    ambient_color,
    emessive_color,
    specular_color
  };

  JsonMaterialColor()
    : m_type(main_color)
    , m_color(0)
  {}
  JsonMaterialColor(const kColorType &type, const ODCOLORREF &color)
    : m_type(type)
    , m_color(color)
  {}

  const kColorType &GetType() const { return m_type; }
  const ODCOLORREF &GetColor() const { return m_color; }
  OdUInt32 GetInvertColor() const;

protected:
  kColorType m_type;
  ODCOLORREF m_color;
};

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
struct JsonMaterials
{
  enum kMaterialType
  {
    default_mat_type = 0,
    point_mat_type,
    line_basic_mat_type,
    mesh_basic_mat_type
  };

  JsonMaterials()
    : m_type(default_mat_type)
    , m_opacity(1)
    , m_width(1)
    , m_colors()
  {}
  JsonMaterials(const JsonMaterials::kMaterialType &type, const float &opacity)
    : m_type(type)
    , m_opacity(opacity)
    , m_width(1)
    , m_colors()
  {}
  JsonMaterials(const JsonMaterials &material, const JsonMaterials::kMaterialType &type)
    : m_type(type)
    , m_opacity(material.m_opacity)
    , m_width(material.m_width)
    , m_colors()
  {
    if (m_type == JsonMaterials::mesh_basic_mat_type)
      m_colors = material.m_colors;
    else if (m_type == JsonMaterials::line_basic_mat_type ||
      m_type == JsonMaterials::point_mat_type)
    {
      const JsonMaterialColor *col = material.m_colors.begin();
      while (col != material.m_colors.end())
      {
        if (col->GetType() == JsonMaterialColor::main_color)
          m_colors.push_back(*col);
        ++col;
      }
    }
  }

  void SetWidth(const float &width) { m_width = width; }

  const kMaterialType &GetType() const { return m_type; }
  const float &GetWidth() const { return m_width; }
  const float &GetOpacity() const { return m_opacity; }

  void AddMaterialColor(const JsonMaterialColor &material_color) { m_colors.push_back(material_color); }

  template <typename T>
  void ForEachColor(T *obj) const
  {
    std::for_each(m_colors.begin(), m_colors.end(), *obj);
  }

protected:
  kMaterialType m_type;
  float m_opacity;
  float m_width;
  OdArray<JsonMaterialColor> m_colors;
};

//JsonObject

/** \details
  <group !!RECORDS_tkernel_apiref>
*/
struct JsonObjectData : public OdRxObject
{
  enum kObjectType
  {
    Scene = 0,
    Object3D,
    Points,
    Line,
    Mesh,
    PerspectiveCamera,
    OrthographicCamera,
    AmbientLight,
    PointLight,
    DirectionalLight,
    SpotLight
  };

  JsonObjectData()
    : m_name("")
    , m_type(Object3D)
    , m_matrix()
    , m_visible(true)
  {}
  JsonObjectData(const char *name, const JsonObjectData::kObjectType &type, const OdGeMatrix3d &matrix = OdGeMatrix3d())
    : m_name(name)
    , m_type(type)
    , m_matrix(matrix)
    , m_visible(true)
  {}
  virtual ~JsonObjectData() {}

  OdUInt32 GetChildrenCount() const { return (OdUInt32)m_children.size(); }
  const OdGeMatrix3d &GetMatrix() const { return m_matrix; }
  const OdString &GetName() const { return m_name; }
  const kObjectType &GetType() const { return m_type; }

  void SetMatrix(const OdGeMatrix3d &matrix) { m_matrix = matrix; }
  void SetType(const JsonObjectData::kObjectType &type) { m_type = type; }
  void SetVisible(OdBool visible) { m_visible = visible; }

  void AddObject(const OdUInt64 &id);

  template <typename T>
  void ForEachChildren(T *obj) const
  {
    std::for_each(m_children.begin(), m_children.end(), (*obj));
  }

  OdBool isVisible() const { return m_visible; }

  void addRef() { }
  void release() { }

  ODRX_USING_HEAP_OPERATORS(OdRxObject);

protected:
  OdString m_name;
  kObjectType m_type;
  OdGeMatrix3d m_matrix;
  std::list<OdUInt64> m_children;
  OdBool m_visible;
};
typedef OdSmartPtr<JsonObjectData> JsonObjectDataPtr;

struct drop_for_each_pred;
//

/** \details
  <group Other_Classes>
*/
class JsonObjectFormat : public OdRxObject
{
  struct CameraViewOptions
  {
    enum kCameraMetafileMode
    {
      default_camera_mode = 0,
      unique_camera_mode
    };

    CameraViewOptions()
      : m_mode(default_camera_mode)
      , m_camera_metafile_matrix()
      , m_lens_weight_indexs()
      , m_lens_weight()
    {}
    ~CameraViewOptions() {}

    kCameraMetafileMode m_mode;
    OdGeMatrix3d m_camera_metafile_matrix;
    std::map<OdInt16, OdUInt64> m_lens_weight_indexs;
    std::list<std::pair<double, OdUInt64> > m_lens_weight;
  };

public:
  JsonObjectFormat(OdGLES2JsonServer* json)
    : m_metadata()
    , m_geometries()
    , m_materials()
    , m_objects()
    , m_scene_uuid(GetNewObjectUUID())
    , m_camera_options(NULL)
    , m_background_color(0)
    , nLevel(1)
    , pJson(json)
  {
    try
    {
      m_camera_options = new CameraViewOptions();
      std::pair<std::map<OdUInt64, JsonObjectDataPtr>::iterator, bool> ins_it = m_objects.insert(std::pair<OdUInt64, JsonObjectDataPtr>(m_scene_uuid, new JsonObjectData("global_scene", JsonObjectData::Scene)));
      if (ins_it.second)
        ins_it.first->second->SetMatrix(OdGeMatrix3d().setCoordSystem(OdGePoint3d(0,0,0), OdGeVector3d::kXAxis, OdGeVector3d::kYAxis, OdGeVector3d::kZAxis));
    }
    catch(...)
    {
      if (m_camera_options != NULL)
        delete m_camera_options;
    }
  }
  virtual ~JsonObjectFormat() 
  {
    if (m_camera_options != NULL)
      delete m_camera_options;
  }

  void addRef() { }
  void release() { }

  ODRX_USING_HEAP_OPERATORS(OdRxObject);

  void SetJsonServer(OdGLES2JsonServer *json) { pJson = json; }
  void SetBackColor(const OdUInt32 &backgound_color) { m_background_color = backgound_color; }
  void SetMeataData(const float &version, const char *type, const char *generator);
  void SetObjectMatrix(const OdUInt64 &obj_id, const OdGeMatrix3d &matrix, OdBool matrix_for_child = false);
  void SetLensWeightByIndx(const OdUInt8Array &arr);
  void SetCameraMetafileMode();
  void SetCameraMetafileMatrix(const OdGeMatrix3d &matrix);
  void SetCameraMetafileMatrixToObject(const OdUInt64 &id);

  OdUInt64 GetNewObjectUUID() const;
  OdUInt64 GetNewMaterialUUID() const;
  OdUInt64 GetNewGeometryUUID() const;
  const OdUInt32 &GetBackColor() const { return m_background_color; }
  const OdGeMatrix3d *const GetMatrix() const;
  const OdUInt64 &GetSceneID() const { return m_scene_uuid; }
  JsonObjectData *const GetObjectChild(const OdUInt64 &id);
  const JsonObjectData *const GetObjectChild(const OdUInt64 &id) const;
  JsonGeometries *const GetGeomertry(const OdUInt64 &id);
  JsonMaterials *const GetMaterial(const OdUInt64 &id);

  void RecalcLensWeight(const double &coef);

  void AddMaterial(const OdUInt64 &uuid, const JsonMaterials::kMaterialType &type, const float &opacity = 1.0);
  OdBool AddMaterialColor(const OdUInt64& id, const JsonMaterialColor::kColorType &type, const ODCOLORREF &color);
  OdBool AddMaterialWidth(const OdUInt64& id, float w);
  OdUInt64 CopyMaterial(const OdUInt64& id, const JsonMaterials::kMaterialType &new_type);

  void AddMaterialLenWeightIndx(const OdUInt64 &mat_id, const OdInt16 &lens_ind);
  void AddMaterialLenWeight(const OdUInt64 &mat_id, const double &len_weight);

  void AddGeometries(const OdUInt64 &uuid, const JsonGeometries::kGeometriesType &type);
  template <typename T>
  OdBool AddGeometriesData(const OdUInt64& uuid, OdUInt32 size, const void* data, const JsonGeometriesDataBase::kDataType &data_type, const JsonGeometriesDataBase::kBufferCopyType &copy_type = JsonGeometriesDataBase::copy_buffer)
  {
    JsonGeometries *const g = GetGeomertry(uuid);
    if (g != NULL)
    {
      g->AddData<T>(size, data, data_type, copy_type);
      return true;
    }
    else
      return false;
  }
  friend struct drop_for_each_pred;
protected:
  JsonObjectDataPtr MakeObjectData(const JsonObjectData::kObjectType &type, const char *name, const OdGeMatrix3d &matrix);
  JsonObjectData *const AddObject(const OdUInt64 &root_id, const OdUInt64 &id, const char *name = "", const JsonObjectData::kObjectType &type = JsonObjectData::Object3D);
  JsonObjectData *const AddObjectToScene(const OdUInt64 &id, const char *name = "", const JsonObjectData::kObjectType &type = JsonObjectData::Object3D);
public:
  OdBool AddMetafileObject(const OdUInt64 &root_id, const char *name, const JsonObjectData::kObjectType &type, const OdUInt64 &geometry_id, const OdUInt64 &material_id, OdBool visible);
  OdBool AddLayer(const OdUInt64 &layer_id, const char *name);
  OdBool AddObjectByLayer(const OdUInt64 &layer_id, const OdUInt64 &id);

//add light
  OdBool AddAmbientLight(const OdUInt64 &root_id, const ODCOLORREF &color);
  OdBool AddDirectionLight(const OdUInt64 &root_id, const ODCOLORREF &color, const OdGeVector3d &direction);
  OdBool AddPointLight(const OdUInt64 &root_id, const ODCOLORREF &color, const OdGePoint3d &position);
  OdBool AddSpotLight(const OdUInt64 &root_id, const ODCOLORREF &color, const OdGePoint3d &position, const OdGeVector3d &direction);

//add camera
  OdBool AddPerspectiveCamera(const OdUInt64 &id, const float &aspect_ratio, const OdGeMatrix3d &matr, const float &far_plan, const float &near_plan);
  OdBool AddOrthographicCamera(const OdUInt64 &id, const double &width, const double &height, const OdGeMatrix3d &matr, const float &far_plan, const float &near_plan);
////

/////////////
//methods of drop data to JsonServer
  void DropObject() const;
protected:
  void DropMetadata() const;
  void DropMaterials() const;
  void DropGeometries() const;
  void DropObjectData(const JsonObjectData *obj) const;
  void DropObjectChildren(const JsonObjectData *obj) const;

private:
  JsonMetadata m_metadata;
  std::map<OdUInt64, JsonGeometries> m_geometries;
  std::map<OdUInt64, JsonMaterials> m_materials;
  std::map<OdUInt64, JsonObjectDataPtr> m_objects;
  OdUInt64 m_scene_uuid;
  CameraViewOptions *m_camera_options;
  OdUInt32 m_background_color;
  mutable OdUInt32 nLevel;
  OdGLES2JsonServer *pJson;
};

#endif // JSON_OBJECT_FORMAT
