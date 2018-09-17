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

// Mapper item.

#ifndef __ODGIMAPPERITEM_H__
#define __ODGIMAPPERITEM_H__

#include "GiExport.h"
#include "GiMaterial.h"
#include "GiMaterialTraitsData.h"
#include "GiGeometry.h"

#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"

#include "RxObject.h"

#include "TD_PackPush.h"

/** \details
    Material mapper item entry for separate texture channel.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMapperItemEntry : public OdRxObject
{
  public:
    /** \details
      Input mapper triangle type.
    */
    struct MapInputTriangle {
      OdGePoint3d inPt[3]; // Mapping xyz
    };
    /** \details
      Output mapper vertices texture coordinates.
    */
    struct MapOutputCoords {
      OdGePoint2d outCoord[3]; // Mapped uv
    };
  public:
    ODRX_DECLARE_MEMBERS(OdGiMapperItemEntry);

    /** \details
      Return mapper (return result mapper combination for pMapper and pInheritMapper).
      \sa
      setMapper
    */
    virtual const OdGiMapper &mapper() const = 0;
    /** \details
      Set mapper combination.
      \param pMapper [in]  Entity mapper.
      \param pInheritMapper [in]  Material (default) mapper.
    */
    virtual void setMapper(const OdGiMapper &pMapper, const OdGiMapper &pInheritMapper) = 0;
    /** \details
      Set mapper combination and object/model matrix.
      \param pMapper [in]  Entity mapper.
      \param pInheritMapper [in]  Material (default) mapper.
      \param tm [in]  Model or object matrix.
    */
    virtual void setMapper(const OdGiMapper &pMapper, const OdGiMapper &pInheritMapper, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for material (entity mapper not specified).
      \param pMaterialMapper [in]  Material mapper.
    */
    virtual void setMapper(const OdGiMapper &pMaterialMapper) = 0;
    /** \details
      Set mapper for material and object/model matrix.
      \param pMaterialMapper [in]  Material mapper.
      \param tm [in]  Model or object matrix.
    */
    virtual void setMapper(const OdGiMapper &pMaterialMapper, const OdGeMatrix3d &tm) = 0;

    /** \details
      Return input vertices transformation matrix.
    */
    virtual const OdGeMatrix3d &inputTransform() const = 0;
    /** \details
      Set input vertices transformation matrix.
    */
    virtual void setInputTransform(const OdGeMatrix3d &tm) = 0;

    /** \details
      Return input vertices device transformation matrix.

      \remarks
      This is a device-dependant optional transformation matrix.
    */
    virtual const OdGeMatrix3d &deviceTransform() const = 0;
    /** \details
      Set input vertices device transformation matrix.

      \remarks
      This is a device-dependant optional transformation matrix.
    */
    virtual void setDeviceTransform(const OdGeMatrix3d &dtm) = 0;

    /** \details
      Return model transformation matrix (used only for kModel mappers).
    */
    virtual const OdGeMatrix3d &modelTransform() const = 0;
    /** \details
      Set model transformation matrix (used only for kModel mappers).
    */
    virtual void setModelTransform(const OdGeMatrix3d &mtm) = 0;

    /** \details
      Return object transformation matrix (used only for kObject mappers).
    */
    virtual const OdGeMatrix3d &objectTransform() const = 0;
    /** \details
      Set object transformation matrix (used only for kObject mappers).
    */
    virtual void setObjectTransform(const OdGeMatrix3d &otm) = 0;

    /** \details
      Return output texture coordinates transformation matrix.
    */
    virtual const OdGeMatrix3d &outputTransform() const = 0;
    /** \details
      Set output texture coordinates transformation matrix.
    */
    virtual void setOutputTransform(const OdGeMatrix3d &tm) = 0;

    /** \details
      Recompute transformation matrices.
      \remarks
      Must be called after mapper or model transforms set.
      \sa
      setMapper, setModelTransform
    */
    virtual void recomputeTransformations() = 0;

    /** \details
      Calculate texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const MapInputTriangle &trg, MapOutputCoords &uv) const = 0;
    /** \details
      Calculate texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const OdGePoint3d *trg, OdGePoint2d *uv) const = 0;
    /** \details
      Calculate texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const double *trg, double *uv) const = 0;
    /** \details
      Calculate texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param normal [in]  Input triangle normal (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const MapInputTriangle &trg, const OdGeVector3d &normal, MapOutputCoords &uv) const = 0;
    /** \details
      Calculate texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param normal [in]  Input triangle normal (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const OdGePoint3d *trg, const OdGeVector3d &normal, OdGePoint2d *uv) const = 0;
    /** \details
      Calculate texture coordinates for input triangle.
      \param trg [in]  Input triangle coordinates (X, Y, Z).
      \param normal [in]  Input triangle normal (X, Y, Z).
      \param uv [out]  Output texture coordinates (U, V).
    */
    virtual void mapCoords(const double *trg, const double *normal, double *uv) const = 0;

    // Shell mapping extensions for 3.4

    enum MappingIteratorType
    {
      kShellMapIt = 0, // Shell-style iterator
      kTriListMapIt, // Triangles list { { 0, 1, 2 }, { 1, 2, 3 } }
      kPolyIt // Simple list of indices (probably shell/mesh face, polygon or unstructured data)
      // kTriListMapIt makes subdividing of vertices without indices on triangles. Set kShellMapIt or
      // kPolyIt if input data doesn't stored as triangles list.
    };

    /** \details
      Calculate texture coordinates for input shellmeshpolygon vertices.
      \param nVerts [in]  Count of input vertices.
      \param pVerts [in]  Array of input vertices (should be equal with vertices count).
      \param pUV [out]  Output array of texture coordinates (should be equal with vertices count).
      \param nList [in]  Count of indices in indices array (include face sizes for shell-style arrays).
      \param pIds [in]  Vertex indices array.
      \param mapIt [in]  Type of indices iterator.
      \param pNormal [in]  Optional normal (if already precalculated and doesn't specified in face/vertex data).
      \param pFaceNormals [in]  Optional face normals array (have greater priority than optional normal).
      \param pVertNormals [in]  Optional vertex normals array (have greater priority than optional face normals).
      \param oType [in]  Used for calculation of normal if doesn't specified.
    */
    virtual void mapCoords(OdUInt32 nVerts, const OdGePoint3d *pVerts, OdGePoint2d *pUV,
                           OdUInt32 nList = 0, const OdInt32 *pIds = NULL, MappingIteratorType mapIt = kTriListMapIt,
                           const OdGeVector3d *pNormal = NULL, const OdGeVector3d *pFaceNormals = NULL,
                           const OdGeVector3d *pVertNormals = NULL, OdGiOrientationType oType = kOdGiNoOrientation) = 0;

    // Predefined coordinates extensions

    /** \details
      Recalculate predefined texture coordinates using current material transformation.
      \param pInUVW [in]  Input array of texture coordinates.
      \param pOutUV [out]  Output array of transformed texture coordinates.
      \param nPoints [in]  Count of input and output texture coordinates.
    */
    virtual void mapPredefinedCoords(const OdGePoint3d *pInUVW, OdGePoint2d *pOutUV, OdUInt32 nPoints) const = 0;

    //

    /** \details
      Return true if entity mapper is specified.
    */
    virtual bool isEntityMapper() const = 0;
    /** \details
      Return true if mapper needs object matrix.
    */
    virtual bool isObjectMatrixNeed() const = 0;
    /** \details
      Return true if mapper needs model matrix.
    */
    virtual bool isModelMatrixNeed() const = 0;
    /** \details
      Return true if mapper requires object or vertex transform.
    */
    virtual bool isDependsFromObjectMatrix() const = 0;
    /** \details
      Returns true if mapper requires vertex transform.
    */
    virtual bool isVertexTransformRequired() const = 0;

    /** \details
      Sets vertex transform from points array.
    */
    virtual void setVertexTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Sets vertex transform from object extents.
    */
    virtual void setVertexTransform(const OdGeExtents3d &exts) = 0;
    /** \details
      Sets vertex transform from points array.
    */
    virtual void setObjectTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Sets object transform from extents.
    */
    virtual void setObjectTransform(const OdGeExtents3d &exts) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMapperItemEntry object pointers.
	
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMapperItemEntry> OdGiMapperItemEntryPtr;

/** \details
    Container item for material data storage.
    OdGiMapperItem used for real-time devices, such as DirectX or OpenGL.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMapperItem : public OdRxObject
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMapperItem);

    /** \details
      Set mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() const = 0;
    /** \details
      Return mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() = 0;

    /** \details
      Set mapper for all texture channels.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Set mapper for all texture channels.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Set mapper for all texture channels.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Set mapper for all texture channels.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm, const OdDbStub *pMaterial = NULL) = 0;

    /** \details
      Set model transformation for all texture channels which needs model transformation.
      \param mtm [in]  Model transformation matrix.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setModelTransform(const OdGeMatrix3d &mtm, bool recomputeTransforms = true) = 0;

    /** \details
      Set object transformation for all texture channels which needs object transformation.
      \param otm [in]  Object transformation matrix.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(const OdGeMatrix3d &otm, bool recomputeTransforms = true) = 0;
    /** \details
      Set object transformation for all texture channels which needs object transformation.
      \param nCount [in]  Number of points into pPoints array.
      \param pPoints [in]  Array of points.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(OdInt32 nCount, const OdGePoint3d *pPoints, bool recomputeTransforms = true) = 0;
    /** \details
      Set object transformation for all texture channels which needs object transformation.
      \param exts [in]  Object extents.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix set.
    */
    virtual void setObjectTransform(const OdGeExtents3d &exts, bool recomputeTransforms = true) = 0;

    /** \details
      Set device-dependant transformation for all texture channels.
      \param dtm [in]  Device transformation matrix.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix set.
    */
    virtual void setDeviceTransform(const OdGeMatrix3d &dtm, bool recomputeTransforms = true) = 0;

    /** \details
      Test whether the current material is valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial) = 0;
    /** \details
      Test whether the current material and matrix are valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Test whether the current mapper and material are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial) = 0;
    /** \details
      Test whether the current mapper, material, and matrix are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Test whether the current object/model matrix is valid for use with previously set up mapper data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual bool isLastProcValid(const OdGeMatrix3d &tm) = 0;

    /** \details
      Return true if entity mapper is specified.
    */
    virtual bool isEntityMapper() const = 0;
    /** \details
      Return true if mapper needs object matrix.
    */
    virtual bool isObjectMatrixNeed() const = 0;
    /** \details
      Return true if mapper needs model matrix.
    */
    virtual bool isModelMatrixNeed() const = 0;
    /** \details
      Return true if mapper requires object or vertex transform.
    */
    virtual bool isDependsFromObjectMatrix() const = 0;
    /** \details
      Returns true if mapper requires vertex transform.
    */
    virtual bool isVertexTransformRequired() const = 0;

    /** \details
      Sets vertex transform from points array for channels which needs vertex transformation.
    */
    virtual void setVertexTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Sets vertex transform from object extents for channels which needs vertex transformation.
    */
    virtual void setVertexTransform(const OdGeExtents3d &exts) = 0;

    /** \details
      Sets input tranform for all channels.
      \param tm [in]  Input transformation matrix.
      \param bVertexDependantOnly [in]  Set to channels which require vertex tranform only.
    */
    virtual void setInputTransform(const OdGeMatrix3d &tm, bool bVertexDependantOnly = true) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMapperItem object pointers.

	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMapperItem> OdGiMapperItemPtr;

/** \details
    Container item for material data storage.
    OdGiMapperRenderItem used for rendering devices.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiMapperRenderItem : public OdGiMapperItem
{
  public:
    ODRX_DECLARE_MEMBERS(OdGiMapperRenderItem);

    /** \details
      Set mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for diffuse texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for diffuse texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setDiffuseMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() const = 0;
    /** \details
      Return mapper for diffuse texture channel.
    */
    virtual OdGiMapperItemEntryPtr diffuseMapper() = 0;

    /** \details
      Set mapper for specular texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setSpecularMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for specular texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setSpecularMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for specular texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setSpecularMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for specular texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setSpecularMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for specular texture channel.
    */
    virtual OdGiMapperItemEntryPtr specularMapper() const = 0;
    /** \details
      Return mapper for specular texture channel.
    */
    virtual OdGiMapperItemEntryPtr specularMapper() = 0;

    /** \details
      Set mapper for reflection texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setReflectionMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for reflection texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setReflectionMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for reflection texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setReflectionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for reflection texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setReflectionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr reflectionMapper() const = 0;
    /** \details
      Return mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr reflectionMapper() = 0;

    /** \details
      Set mapper for opacity texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setOpacityMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for opacity texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setOpacityMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for opacity texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setOpacityMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for opacity texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setOpacityMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for opacity texture channel.
    */
    virtual OdGiMapperItemEntryPtr opacityMapper() const = 0;
    /** \details
      Return mapper for opacity texture channel.
    */
    virtual OdGiMapperItemEntryPtr opacityMapper() = 0;

    /** \details
      Set mapper for bump texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setBumpMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for bump texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setBumpMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for bump texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setBumpMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for bump texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setBumpMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for bump texture channel.
    */
    virtual OdGiMapperItemEntryPtr bumpMapper() const = 0;
    /** \details
      Return mapper for bump texture channel.
    */
    virtual OdGiMapperItemEntryPtr bumpMapper() = 0;

    /** \details
      Set mapper for reflection texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setRefractionMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for refraction texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setRefractionMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for reflection texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setRefractionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for refraction texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setRefractionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr refractionMapper() const = 0;
    /** \details
      Return mapper for reflection texture channel.
    */
    virtual OdGiMapperItemEntryPtr refractionMapper() = 0;

    /** \details
      Set mapper for normal map texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setNormalMapMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for normal map texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setNormalMapMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for normal map texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setNormalMapMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for normal map texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setNormalMapMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for normal map texture channel.
    */
    virtual OdGiMapperItemEntryPtr normalMapMapper() const = 0;
    /** \details
      Return mapper for normal map texture channel.
    */
    virtual OdGiMapperItemEntryPtr normalMapMapper() = 0;

    /** \details
      Set mapper for emission texture channel.
      \param traitsData [in]  Input traits data.
    */
    virtual void setEmissionMapper(const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for emission texture channel.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setEmissionMapper(const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Set mapper for emission texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
    */
    virtual void setEmissionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData) = 0;
    /** \details
      Set mapper for emission texture channel.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual void setEmissionMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdGeMatrix3d &tm) = 0;
    /** \details
      Return mapper for emission texture channel.
    */
    virtual OdGiMapperItemEntryPtr emissionMapper() const = 0;
    /** \details
      Return mapper for emission texture channel.
    */
    virtual OdGiMapperItemEntryPtr emissionMapper() = 0;

    /** \details
      Set mapper for all texture channels.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;
    /** \details
      Set mapper for all texture channels.
      \param pMapper [in]  Entity mapper.
      \param traitsData [in]  Input traits data.
      \param pMaterial [in]  Material ID for input traits data.
    */
    virtual void setMapper(const OdGiMapper *pMapper, const OdGiMaterialTraitsData &traitsData, const OdDbStub *pMaterial = NULL) = 0;

    /** \details
      Set model transformation for all texture channels which needs model transformation.
      \param mtm [in]  Model transformation matrix.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setModelTransform(const OdGeMatrix3d &mtm, bool recomputeTransforms = true) = 0;

    /** \details
      Set object transformation for all texture channels which needs object transformation.
      \param otm [in]  Object transformation matrix.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(const OdGeMatrix3d &otm, bool recomputeTransforms = true) = 0;
    /** \details
      Set object transformation for all texture channels which needs object transformation.
      \param nCount [in]  Number of points into pPoints array.
      \param pPoints [in]  Array of points.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(OdInt32 nCount, const OdGePoint3d *pPoints, bool recomputeTransforms = true) = 0;
    /** \details
      Set object transformation for all texture channels which needs object transformation.
      \param exts [in]  Object extents.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setObjectTransform(const OdGeExtents3d &exts, bool recomputeTransforms = true) = 0;

    /** \details
      Set device-dependant transformation for all texture channels.
      \param dtm [in]  Device transformation matrix.
      \param recomputeTransforms [in]  Call recomputeTransformations() for all texture channels after matrix is set.
    */
    virtual void setDeviceTransform(const OdGeMatrix3d &dtm, bool recomputeTransforms = true) = 0;

    /** \details
      Test whether the current material is valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial) = 0;
    /** \details
      Test whether the current material and matrix are valid for use with previously set up mapper data.
      \param pMaterial [in]  Material ID.
      \param tm [in]  object/model transformation matrix.
    */
    virtual bool isLastProcValid(const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Test whether the current mapper and material are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial) = 0;
    /** \details
      Test whether the current mapper, material, and matrix are valid for use with previously set up mapper data.
      \param pMapper [in]  Entity mapper.
      \param pMaterial [in]  Material ID.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual bool isLastProcValid(const OdGiMapper *pMapper, const OdDbStub *pMaterial, const OdGeMatrix3d &tm) = 0;
    /** \details
      Test whether the current object/model matrix is valid for use with previously set up mapper data.
      \param tm [in]  Object/model transformation matrix.
    */
    virtual bool isLastProcValid(const OdGeMatrix3d &tm) = 0;

    /** \details
      Return true if entity mapper is specified for all channels.
    */
    virtual bool isEntityMapper() const = 0;
    /** \details
      Return true if at least one mapper needs object matrix.
    */
    virtual bool isObjectMatrixNeed() const = 0;
    /** \details
      Return true if at least one mapper needs model matrix.
    */
    virtual bool isModelMatrixNeed() const = 0;
    /** \details
      Return true if mapper requires object or vertex transform.
    */
    virtual bool isDependsFromObjectMatrix() const = 0;
    /** \details
      Returns true if mapper requires vertex transform.
    */
    virtual bool isVertexTransformRequired() const = 0;

    /** \details
      Set vertex transform from points array for channels which needs vertex transformation.
    */
    virtual void setVertexTransform(OdInt32 nCount, const OdGePoint3d *pPoints) = 0;
    /** \details
      Set vertex transform from object extents for channels which needs vertex transformation.
    */
    virtual void setVertexTransform(const OdGeExtents3d &exts) = 0;

    /** \details
      Sets input tranform for all channels.
      \param tm [in]  Input transformation matrix.
      \param bVertexDependantOnly [in]  Set to channels which require vertex tranform only.
    */
    virtual void setInputTransform(const OdGeMatrix3d &tm, bool bVertexDependantOnly = true) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiMapperRenderItem object pointers.
	
	<group Type Definitions>
*/
typedef OdSmartPtr<OdGiMapperRenderItem> OdGiMapperRenderItemPtr;

#include "TD_PackPop.h"

#endif // __ODGIMAPPERITEM_H__
