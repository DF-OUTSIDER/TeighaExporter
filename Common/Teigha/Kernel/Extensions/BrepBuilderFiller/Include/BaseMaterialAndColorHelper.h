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


#ifndef _BASEMATERIALHELPER_INCLUDED_
#define _BASEMATERIALHELPER_INCLUDED_

#include "BrepBuilderFillerDef.h"
#include "IMaterialAndColorHelper.h"

#include "MaterialResolver.h"
#include "CmColorBase.h"
#include "Gi/GiMaterial.h"
#include "DbBaseDatabase.h"

class OdGiContext;


/** \details
  Helper structure for resolve materials.
  <group Extension_Classes>
*/
class BREPBUILDERFILLER_DLL OdBaseMaterialAndColorHelper : public OdIMaterialAndColorHelper
{
public:
  explicit OdBaseMaterialAndColorHelper(
    OdDbBaseDatabasePtr pSourceDb,
    OdDbBaseDatabasePtr pDestinationDb,
    const OdMaterialResolver* pMaterialResolver = NULL,
    OdDbStub* pDefaultMaterial = NULL);
  ~OdBaseMaterialAndColorHelper();
  
  /** \details
  Sets the material resolver for getting source material.
  */
  void setSourceMaterialResolver(const OdMaterialResolver* pSourceMaterialResolver)
  {
    m_pSourceMaterialResolver = pSourceMaterialResolver;
  }

  /** \details
  Sets the default source material.
  This material will be used if B-Rep face have not any material.
  */
  void setSourceEntityMaterial(OdDbStub* pSourceEntityMaterial)
  {
    m_pSourceEntityMaterial = pSourceEntityMaterial;
  }

  /** \details
  Sets the default source material mapping.
  This material mapping will be used if B-Rep face have not any material mapping.
  */
  void setSourceEntityMaterialMapping(const OdGiMapper& materialMapper)
  {
    m_hasSourceEntityMaterialMapping = true;
    m_sourceEntityMaterialMapping = materialMapper;
  }
  /** \details
  Resets the default source material mapping.
  Nothing will be used if B-Rep face have not any material mapping.
  */
  void resetSourceEntityMapping()
  {
    m_hasSourceEntityMaterialMapping = false;
    m_sourceEntityMaterialMapping = OdGiMapper();
  }

  /** \details
  Sets the default color of source faces and edges.
  This color will be used if B-Rep face or edge have not any color.
  */
  void setSourceEntityColor(OdCmEntityColor defaultColor)
  {
    setSourceFaceColor(defaultColor);
    setSourceEdgeColor(defaultColor);
  }
  /** \details
  Resets the default color of source faces and edges.
  Nothing will be used if B-Rep face or edge have not any color.
  */
  void resetSourceEntityColor()
  {
    resetSourceFaceColor();
    resetSourceEdgeColor();
  }
  /** \details
  Sets the default color of source faces.
  This color will be used if B-Rep face have not any color.
  */
  void setSourceFaceColor(OdCmEntityColor defaultColor)
  {
    m_hasSourceEntityFaceColor = true;
    m_sourceEntityFaceColor = defaultColor;
  }
  /** \details
  Resets the default color of source faces.
  Nothing will be used if B-Rep face have not any color.
  */
  void resetSourceFaceColor()
  {
    m_hasSourceEntityFaceColor = false;
    m_sourceEntityFaceColor = OdCmEntityColor::kNone;
  }
  /** \details
  Sets the default color of source edges.
  This color will be used if B-Rep edge have not any color.
  */
  void setSourceEdgeColor(OdCmEntityColor defaultColor)
  {
    m_hasSourceEntityEdgeColor = true;
    m_sourceEntityEdgeColor = defaultColor;
  }
  /** \details
  Resets the default color of source edges.
  Nothing will be used if B-Rep edge have not any color.
  */
  void resetSourceEdgeColor()
  {
    m_hasSourceEntityEdgeColor = false;
    m_sourceEntityEdgeColor = OdCmEntityColor::kNone;
  }

  /** \details
  This method get and convert face visuals information for BrepBuilder.
  Also it should return same material for similar visual information.
  \param face [in] object for getting source visual information.
  \param faceMaterial [out] face material for BrepBuilder.
  \param faceMaterialMapping [out] face material mapping for BrepBuilder. Should be ignoredd if applyFaceMaterialMapping is false.
  \param applyFaceMaterialMapping [out] flag signals to use faceMaterialMapping.
  \param faceColor [out] face color for BrepBuilder. Should be ignored if applyFaceColor is false.
  \param applyFaceColor [out] flag signals to use faceColor.
  */
  OdResult getFaceVisualInfo(const OdBrFace& face,
    OdDbStub*& faceMaterial,
    OdGiMapper& faceMaterialMapping, bool& applyFaceMaterialMapping,
    OdCmEntityColor& faceColor, bool& applyFaceColor) ODRX_OVERRIDE;

  /** \details
  This method get and convert edge visuals information for BrepBuilder.
  \param edge [in] object for getting source visual information.
  \param edgeColor [out] edge color for BrepBuilder. Should be ignored if applyEdgeColor is false.
  \param applyEdgeColor [out] flag signals to use edgeColor.
  */
  OdResult getEdgeVisualInfo(const OdBrEdge& edge, OdCmEntityColor& edgeColor, bool& applyEdgeColor) ODRX_OVERRIDE;
protected:
  /** \details
  Return color specific to destinion database.
  This method should be redefined in subclasses.
  Default implementation return eNotImplemented.
  \param sourceDbColor [in] color from source B-Rep.
  \param destinationDbColor [out] color for BrepBuilder.
  */
  virtual OdResult convertColor(
    const OdCmEntityColor& sourceDbColor,
    OdCmEntityColor& destinationDbColor
  );

  /** \details
  Return source face material id.
  */
  OdResult getSourceMaterialId(const OdBrFace& face, OdDbStub*& material) const;
  /** \details
  Return source face material mapping.
  */
  bool getSourceMaterialMapper(const OdBrFace& face, OdGiMapper& materialMapper) const;
  /** \details
  Return source face color.
  */
  bool getSourceFaceColor(const OdBrFace& face, OdCmEntityColor& faceColor) const;
  /** \details
  Return source edge color.
  */
  bool getSourceEdgeColor(const OdBrEdge& edge, OdCmEntityColor& edgeColor) const;

  OdDbBaseDatabasePtr m_pSourceDb;
  OdDbBaseDatabasePtr m_pDestinationDb;

  const OdMaterialResolver* m_pSourceMaterialResolver;
  OdGiDefaultContextPtr m_pSourceGiContext;
  OdDbStub* m_pSourceEntityMaterial;
  bool m_hasSourceEntityMaterialMapping;
  OdGiMapper m_sourceEntityMaterialMapping;
  bool m_hasSourceEntityFaceColor;
  OdCmEntityColor m_sourceEntityFaceColor;
  bool m_hasSourceEntityEdgeColor;
  OdCmEntityColor m_sourceEntityEdgeColor;


  /** \details
  Helper structure for cache materials.
  */
  struct MaterialDataStore
  {
    OdDbStub* sourceMaterial;
    OdGiMapper sourceMapper;
    OdCmEntityColor sourceColor;

    OdDbStub* materialId;

    MaterialDataStore()
      : sourceMaterial(NULL)
      , sourceMapper(OdGiMapper::kIdentity)
      , sourceColor(OdCmEntityColor::kNone)
      , materialId(NULL)
    {}

    bool operator == (const MaterialDataStore& other) const
    {
      return sourceMaterial == other.sourceMaterial
        && sourceMapper == other.sourceMapper
        && sourceColor == other.sourceColor;
    }
  };

  typedef OdArray<MaterialDataStore> MaterialCache;
  MaterialCache m_materialCache;

  virtual OdDbStub * findMaterialInUserCache(const MaterialDataStore & matData);
  virtual void addMaterialToUserCache(const MaterialDataStore & matData);
};

#endif //_BASEMATERIALHELPER_INCLUDED_
