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

#ifndef _COLLADA_EXPORT_VIEW_INCLUDED_
#define _COLLADA_EXPORT_VIEW_INCLUDED_
#pragma once
#include "OdaCommon.h"
#include "OdError.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiDrawable.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiLightTraitsData.h"
#include "Gi/GiUtils.h"
#include "Gs/GsBaseVectorizer.h"
#include "Gs/GsBaseMaterialView.h"
#include "RxObjectImpl.h"
#include "RxDynamicModule.h"
#include "COLLADASWEffectProfile.h"
#include "LightExporter.h"


/** \details
  <group OdExport_Classes> 
*/
namespace TD_COLLADA_EXPORT
{
  static OdUInt64 iColladaEntCounter = 0;

  /** \details
    This class implements the material data for Collada.
  */
  struct MaterialData
  {
    COLLADASW::Color ambientColor;
    COLLADASW::Color diffuseColor;
    COLLADASW::Color specularColor;
    double           dAmbientColorFactor;
    double           dDiffuseColorFactor;
    double           dSpecularColorFactor;
    bool             bDiffuseHasTexture;
    bool             bSpecularHasTexture;
    bool             bAmbientChannelEnabled;
    bool             bDiffuseChannelEnabled;
    bool             bSpecularChannelEnabled;
    OdString         sDiffuseFileSource;
    OdString         sSpecularFileSource;
    // Specular
    float glossFactor;
    // Opacity
    bool             bOpacityChannelEnabled;
    double           dOpacityPercentage;
    bool             bOpacityHasTexture;
    OdString         sOpacityFileSource;
    // Reflection
    bool             bReflectionChannelEnabled;
    double           dReflectionPercentage;
    bool             bReflectionHasTexture;
    OdString         sReflectionFileSource;
    // Bump
    bool             bBumpChannelEnabled;
    double           dBumpPercentage;
    bool             bBumpHasTexture;
    OdString         sBumpFileSource;
    // Emission
    double           dEmissionPercentage;    
    // Refraction
    bool             bRefractionChannelEnabled;
    double           dRefractionIndex;
    // Translucence
    double           dTranslucence;


    bool operator == ( MaterialData& val ) const
    {
      return (bAmbientChannelEnabled ? (ambientColor == val.ambientColor && OdEqual(dAmbientColorFactor, val.dAmbientColorFactor) && bAmbientChannelEnabled == val.bAmbientChannelEnabled):true)&&
       (bDiffuseChannelEnabled ? (diffuseColor == val.diffuseColor && OdEqual(dDiffuseColorFactor ,val.dDiffuseColorFactor) && bDiffuseChannelEnabled == val.bDiffuseChannelEnabled &&
       (bDiffuseHasTexture ? (bDiffuseHasTexture == val.bDiffuseHasTexture && sDiffuseFileSource == val.sDiffuseFileSource):true)):true) &&
       (bSpecularChannelEnabled ? (specularColor == val.specularColor && OdEqual(dSpecularColorFactor, val.dSpecularColorFactor) && bSpecularChannelEnabled == val.bSpecularChannelEnabled &&
       (bSpecularHasTexture ? (bSpecularHasTexture == val.bSpecularHasTexture && sSpecularFileSource == val.sSpecularFileSource):true)):true) &&
       (bOpacityChannelEnabled ? (OdEqual(dOpacityPercentage ,val.dOpacityPercentage) && bOpacityChannelEnabled == val.bOpacityChannelEnabled &&
       (bOpacityHasTexture ? (bOpacityHasTexture == val.bOpacityHasTexture && sOpacityFileSource == val.sOpacityFileSource):true)):true) &&
       (bReflectionChannelEnabled ? (OdEqual(dReflectionPercentage, val.dReflectionPercentage) && bReflectionChannelEnabled == val.bReflectionChannelEnabled &&
       (bReflectionHasTexture ? (bReflectionHasTexture == val.bReflectionHasTexture && sReflectionFileSource == val.sReflectionFileSource):true)):true) &&
       (bBumpChannelEnabled ? (OdEqual(dBumpPercentage, val.dBumpPercentage) && bBumpChannelEnabled == val.bBumpChannelEnabled &&
       (bBumpHasTexture ? (bBumpHasTexture == val.bBumpHasTexture && sBumpFileSource == val.sBumpFileSource):true)):true) &&
       (bRefractionChannelEnabled ? (bRefractionChannelEnabled == val.bRefractionChannelEnabled && OdEqual(dRefractionIndex, val.dRefractionIndex)):true) &&
       (OdEqual(glossFactor, val.glossFactor) && OdEqual(dEmissionPercentage, val.dEmissionPercentage) && OdEqual(dTranslucence, val.dTranslucence));
    }


    MaterialData() : dAmbientColorFactor(0), dDiffuseColorFactor(0), dSpecularColorFactor(0),
                     bDiffuseHasTexture(false), bSpecularHasTexture(false), bAmbientChannelEnabled(false),
                     bDiffuseChannelEnabled(false), bSpecularChannelEnabled(false), bOpacityChannelEnabled(false),
                     dOpacityPercentage(0), bOpacityHasTexture(false), bReflectionChannelEnabled(false),
                     dReflectionPercentage(0), bReflectionHasTexture(false), bBumpChannelEnabled(false),
                     dBumpPercentage(0), bBumpHasTexture(false), dEmissionPercentage(0),
                     bRefractionChannelEnabled(false), dRefractionIndex(0), dTranslucence(0) {}
  };

  enum Flags
  {
    kRasterImage    = 1,
    kRasterImageMat = 2,
    kAddEntity      = 4
  };

  /** \details
    This structure implements the material mapper for Collada.
  */
  struct ColladaMaterialMapper
  {
    OdGePoint2dArray   m_ptTextureCoordArr;
    OdUInt32Array      m_indTextureCoordArr;
  };

  /** \details
    This structure stores the ent data for Collada.
  */
  struct ColladaEntData
  {
    OdGePoint3dArray      m_ptArr;
    OdUInt32Array         m_indPtsArr;
    OdGeVector3dArray     m_normVtxArr;
    OdUInt32Array         m_indVtxNormArr;
    ColladaMaterialMapper m_pDiffuseMaterialMapperArr;
    OdUInt64              m_iId;
    unsigned int          m_iEntMaterial;

    ColladaEntData() : m_iId(0), m_iEntMaterial(0) {}
  };

  /** \details
    This structure stores the material data for Collada.
  */
  struct ColladaMatrialData
  {
    OdArray<MaterialData>           m_matDataArr;
    OdUInt64Array                   m_matIdArr;
  };

  typedef OdArray<ColladaEntData> ColladaEntDataArray;
  typedef OdArray<ColladaEntDataArray> EntityDataArray;

  /** \details
    This class implements the stub vectorize device for Collada.
  */
  class StubVectorizeDevice : public OdGsBaseVectorizeDevice
  {
  protected:
    ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);
  public:
    StubVectorizeDevice() { }
    ~StubVectorizeDevice() { }
  };  


  static OdCmEntityColor fixByACI(const ODCOLORREF *ids, const OdCmEntityColor &color)
  {
    if (color.isByACI() || color.isByDgnIndex())
    {
      return OdCmEntityColor(ODGETRED(ids[color.colorIndex()]), ODGETGREEN(ids[color.colorIndex()]), ODGETBLUE(ids[color.colorIndex()]));
    }
    else if (!color.isByColor())
    {
      return OdCmEntityColor(0, 0, 0);
    }
    return color;
  }

  /** \details
    This class implements the Collada output.
  */
  class OdColladaOut : public OdGsBaseMaterialView, 
                       public OdGiGeometrySimplifier
  {
    OdGePoint2dArray      m_texCoordsArray;

    EntityDataArray*      m_pEntityDataArr;
    ColladaMatrialData*   m_pColladaMaterialData;
    LightExporter*        m_pLightExp;

    ColladaEntDataArray*  m_pCurrentColladaEntDataArr;
    ColladaEntData*       m_pCurrentColladaEntData;
    unsigned int          m_iCurMaterial;
    OdUInt32              m_flags;
    unsigned int          m_iLightCounter;

    unsigned int          iTmpIndex;
    OdInt32               m_iPtArrSize;

    void addColladaEntData();

  protected:
    OdGiMapperItemPtr setMapper(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v, const OdGiRasterImage* pImage);

    void triangleOut( const OdInt32* p3Vertices, const OdGeVector3d* pNormal);

    void fillCache(OdUInt64 matId, const MaterialData& matData, unsigned int iStartFind);
    void fillMaterialCache(MaterialData& mData, const OdGiMaterialTraitsData &materialData);
    OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub* materialId, const OdGiMaterialTraitsData & materialData);

    void polylineOut(OdInt32 , const OdGePoint3d* );

    void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);
    void circle(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint);

    void circularArc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector,
                     double sweepAngle, OdGiArcType arcType);

    void circularArc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint, OdGiArcType arcType);

    void polyline(OdInt32 numVertices, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal, OdGsMarker baseSubEntMarker);

    void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList);

    void pline(const OdGiPolyline& polyline, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);

    void shape(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
               int shapeNumber, const OdGiTextStyle* pTextStyle);

    void text(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
              double height, double width, double oblique, const OdString& msg);

    void text(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
              const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle);

    void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
    void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint );

    void nurbs(const OdGeNurbCurve3d& nurbsCurve);

    void ellipArc(const OdGeEllipArc3d& ellipArc, const OdGePoint3d* endPointsOverrides, OdGiArcType arcType);

    void mesh(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList, const OdGiEdgeData* pEdgeData,
              const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);

    void meshProc(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList, const OdGiEdgeData* pEdgeData = 0,
                  const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0);

    void worldLine(const OdGePoint3d points[2]);

    void edge(const OdGiEdge2dArray& edges);

    void shell( OdInt32 numVerts, const OdGePoint3d* pVertexList,
                OdInt32 faceListSize, const OdInt32* pFaceList, const OdGiEdgeData* pEdgeData,
                const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);

    void shellProc( OdInt32 numVertices, const OdGePoint3d* vertexList,
                    OdInt32 faceListSize, const OdInt32* faceList, const OdGiEdgeData* pEdgeData,
                    const OdGiFaceData* pFaceData, const OdGiVertexData* pVertexData);

    void shellFaceOut(OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal);

    void meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal);

    void rasterImageDc( const OdGePoint3d& origin,
                        const OdGeVector3d& u,
                        const OdGeVector3d& v,
                        const OdGiRasterImage* pImage,
                        const OdGePoint2d* uvBoundary,
                        OdUInt32 numBoundPts,
                        bool transparency = false,
                        double brightness = 50.0,
                        double contrast = 50.0,
                        double fade = 0.0);

    void image(const OdGiImageBGRA32& img,
               const OdGePoint3d& origin,
               const OdGeVector3d& uVec,
               const OdGeVector3d& vVec,
               OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit);

    void processMaterialNode(OdDbStub *materialId, OdGsMaterialNode *pNode);

    void initTexture( const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                      const OdGiRasterImage* pImage, bool transparency, double brightness, double contrast, double fade);

    void uninitTexture();

    void addColladaEntity();
  public:
    OdColladaOut();

    StubVectorizeDevice* device();

    bool doDraw(OdUInt32 i, const OdGiDrawable* pDrawable);
    void init(EntityDataArray* pEntityDataArr, ColladaMatrialData* pColladaMaterialData, LightExporter* pLightExp);
    void beginViewVectorization();
    void endViewVectorization();

    void setMode(OdGsView::RenderMode mode);
  };
}
#endif //_COLLADA_EXPORT_VIEW_INCLUDED_

