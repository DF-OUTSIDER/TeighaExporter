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

#include "PdfExportCommon.h"
#include "PrcContextForPdfExport.h"
#include "Gi/GiBaseVectorizer.h"

class PdfExportExtCalc : public OdGiExtCalc
{
public:
  PdfExportExtCalc();
  virtual ~PdfExportExtCalc();

  void draw(const OdGiDrawable* pDrawable);
  OdDbStub* viewportObjectId() const;
  void setViewportObjectId(OdDbStub* id);
  OdGiRegenType regenType() const;
  void setRegenType(OdGiRegenType regenType);

private:
  OdDbStub* m_pViewportObjectId;
  OdGiRegenType m_regenType;
};

PdfExportExtCalc::PdfExportExtCalc()
  : OdGiExtCalc()
  , m_pViewportObjectId(NULL)
  , m_regenType(eOdGiRegenTypeInvalid)
{
}

PdfExportExtCalc::~PdfExportExtCalc()
{
}

void PdfExportExtCalc::draw(const OdGiDrawable* pDrawable)
{
  m_BBoxSet = false;

  SETBIT_0(m_flags, OdGiBaseVectorizer::kSuppressViewportDraw);
  OdGiBaseVectorizer::doDraw(pDrawable->setAttributes(this), pDrawable);

  //pDrawable->worldDraw(this);
  m_BBoxSet = false;
}

OdDbStub* PdfExportExtCalc::viewportObjectId() const
{
  return m_pViewportObjectId;
}

void PdfExportExtCalc::setViewportObjectId(OdDbStub* id)
{
  m_pViewportObjectId = id;
}

OdGiRegenType PdfExportExtCalc::regenType() const
{
  return m_regenType;
}

void PdfExportExtCalc::setRegenType(OdGiRegenType regenType)
{
  m_regenType = regenType;
}

void OdPrcContextForPdfExport::getExtents(
  const OdGiDrawable * pDrawable,
  OdDbStub * viewportObjectId,
  OdGiContext * pGiContext,
  const OdGeMatrix3d & cameraView,
  const OdGeMatrix3d & modelToWorld,
  OdGeExtents3d & extents) const
{
  getExtentsWithParams(pDrawable, viewportObjectId, pGiContext, cameraView, modelToWorld, extents, kOdGiForExtents);
}

void OdPrcContextForPdfExport::getExtentsWithParams(
  const OdGiDrawable * pDrawable,
  OdDbStub * viewportObjectId,
  OdGiContext * pGiContext,
  const OdGeMatrix3d & cameraView,
  const OdGeMatrix3d & modelToWorld,
  OdGeExtents3d & extents,
  OdGiRegenType regenType) const
{
  // calculate visible extents
  OdStaticRxObject<PdfExportExtCalc> extentsCalc;
  extentsCalc.setRegenType(regenType);
  extentsCalc.pushModelTransform(cameraView.inverse());
  extentsCalc.pushModelTransform(modelToWorld);
  extentsCalc.setContext(pGiContext);
  extentsCalc.setViewportObjectId(viewportObjectId);
  extentsCalc.draw(pDrawable);
  extentsCalc.getExtents(extents);
}
