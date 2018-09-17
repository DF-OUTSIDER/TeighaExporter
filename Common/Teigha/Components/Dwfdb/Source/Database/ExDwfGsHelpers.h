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

#ifndef EX_DWF_GS_HELPERS_H_
#define EX_DWF_GS_HELPERS_H_

#include "TD_PackPush.h"

#include "ExGsHelpers.h"

#include "Gi/GiConveyorNode.h"
#include "Gi/GiConveyorEntryPoint.h"
#include "Gi/GiBaseVectorizer.h"

class OdExDwfExtendedLineWeightStyleClient : public OdExExtendedLineWeightStyleClient
{
  OdGiViewportGeometry &m_geometry;
  OdGiViewportDraw* m_vd;
public:
  OdExDwfExtendedLineWeightStyleClient(const OdGeVector3d &viewNormal, OdGiViewportDraw* vd)
    : OdExExtendedLineWeightStyleClient(viewNormal, vd->subEntityTraits().lineWeight())
    , m_geometry(vd->geometry())
    , m_vd(vd)
  {
    setCapStyle(OdPs::kLesRound);
    setJoinStyle(OdPs::kLjsBevel);
  }
  virtual bool elsPolygonOut(const OdGePoint3d *pPoints, OdUInt32 nPoints)
  {
    m_geometry.polygon(nPoints, pPoints);
    return true;
  }
  virtual bool elsCircleOut(const OdGePoint3d &pPoint, const OdGeVector3d & normal, double radius)
  {
    m_geometry.circle(pPoint, radius, normal);
    return true;
  }
};

// Flags   Flag bits reserved for the WHIP! data generator’s use. 
// Currently the AutoCAD DWF ePlot driver uses this field to set one or more of the following flags: 

#define FLAG_VERTICAL        0x0001      // TVERT
#define FLAG_MIRROR_X        0x0002      // TXMIR
#define FLAG_MIRROR_Y        0x0004      // TYMIR
#define FLAG_UNDERSCORE      0x0008      // TUNDER
#define FLAG_OVERSCORE       0x0010      // TOVER
#define FLAG_MTEXT_START     0x0020      // TMSTART
#define FLAG_MTEXT_END       0x0040      // TMEND
#define FLAG_MTEXT           0x0080      // TMTEXT
#define FLAG_GREEK_TEXT      0x1000      // Whip and GDI font engine only
#define FLAG_PATH_TEXT       0x2000      // Whip and GDI font engine only
#define FLAG_OUTLINE_TEXT    0x4000      // Outline Text Only

template <class TBase>
class DwfDbDrawTextForContourExplode : public OdGiBaseVectorizer
                                     , public TBase //OdGiContextForDwfDatabase
{
public:
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

  DwfDbDrawTextForContourExplode()
  {
    OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts|kDrawLayerOff|kDrawLayerFrozen);
    setContext(this); 
    m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
  }
  void attachOutputNode(OdGiConveyorNode *pNode)
  {
    pNode->input().addSourceNode(output());
  }
  OdGiRegenType regenType() const
  {
    return kOdGiForExplode;
  }
  void draw(const OdGiDrawable* pDrawable)
  {
    pDrawable->setAttributes(this);
    pDrawable->worldDraw(this);
  }
  const OdGiSubEntityTraitsData& effectiveTraits() const
  {
    return m_entityTraitsData;
  }
  void affectTraits(const OdGiSubEntityTraitsData* , OdGiSubEntityTraitsData& ) const
  {
  }
};

#include "TD_PackPop.h"

#endif // EX_DWF_GS_HELPERS_H_
