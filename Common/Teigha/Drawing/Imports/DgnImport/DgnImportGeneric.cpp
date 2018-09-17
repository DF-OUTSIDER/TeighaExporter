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

#include <OdaCommon.h>
#include "DgnImportPE.h"
#include "DgnImportImpl.h"
#include "DgnImportContext.h"
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include "Gi/GiBaseVectorizer.h"
#include "OdList.h"
#include "StaticRxObject.h"
#include "GiContextForDbDatabase.h"
#include "Gi/GiGeometrySimplifier.h"
//#include "OdTextIterator.h"
#include "DbPolyline.h"
#include "DbDatabase.h"
#include "DbLine.h"
#include "DbSolid.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "DbLayerTableRecord.h"
#include "DbText.h"
#include "DbSymUtl.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"
#include "Db2dPolyline.h"
#include "Db2dVertex.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "DbBlockReference.h"
#include "DbSolid.h"
#include "DbPolyFaceMesh.h"
#include "DbPolyFaceMeshVertex.h"
#include "DbFaceRecord.h"
#include "DbPolygonMesh.h"
#include "DbPolygonMeshVertex.h"
#include "DbXline.h"
#include "DbRay.h"
#include "DbHatch.h"
#include "Gi/GiPolyline.h"
#include "Gi/GiUtils.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeLineSeg2d.h"
#include "OdDToStr.h"
#include "DbFace.h"
#include "CmTransparencyArray.h"
#include "DgGraphicsElement.h"
#include "CmEntityColorArray.h"
#include "DbMText.h"
#include "DbBlockTableRecord.h"
#include "DbLinetypeTable.h"
#include "DgGiContext.h"
#include "DbMPolygon.h"
#include "DgCone.h"
#include "Dg3DObject.h"
#include "DgBRepEntityPE.h"
#include "DgnImportCommon.h"
#include "DgDimension.h"
#include "DgShape.h"
#include "DgComplexShape.h"
#include "DgEllipse.h"
#include "DgBSplineCurve.h"
#include "DgFontTableRecord.h"
#include "DgCurve.h"
#include <DbOle2Frame.h>
#include <OleItemHandler.h>

using namespace TD_DGN_IMPORT;

ODRX_NO_CONS_DEFINE_MEMBERS(OdDgnImportPE, OdRxObject);

class OdDbPolyFaceMesh;
class OdDbEntity;
typedef OdSmartPtr<OdDbEntity> OdDbEntityPtr;

namespace TD_DGN_IMPORT {
class OdTGDrawObjectForExplode : 
  public OdGiBaseVectorizer, 
  public OdGiContextForDgDatabase, 
  public OdGiGeometrySimplifier
{
  OdDbDatabasePtr _dbDatabase;
public:
  OdTGDrawObjectForExplode();
  OdResult explode(const OdDgGraphicsElement* pEnt, OdDgDatabase* pDb, OdDbDatabase* db, OdRxObjectPtrArray& entitySet);
  OdResult drawDimensionSegment(const OdDgGraphicsElement* pEnt, OdUInt32 uSegmentNum, OdDgDatabase* pDb, OdDbDatabase* db, OdRxObjectPtrArray& entitySet);
  virtual void worldDrawEntity(const OdDgGraphicsElement* pEnt);
  OdDbDatabase *getDbDatabase() { return _dbDatabase.get(); }
  void setSaveClippedOnlyFlag( bool bSet );

  // OdGiBaseVectorizer overrides
  //
  OdGiRegenType regenType() const {return m_uRegenType;}
  virtual void draw(const OdGiDrawable* pDrawable);

  virtual void polyline(OdInt32 nbPoints,
    const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = NULL,
    OdGsMarker lBaseSubEntMarker = -1);
  virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
  virtual void pline(const OdGiPolyline& lwBuf, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);
  virtual void xline(const OdGePoint3d&,  const OdGePoint3d&);
  virtual void ray(const OdGePoint3d&, const OdGePoint3d&);
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg);
  // Explodes the text() primitive as a single OdDbMText entity
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw, 
    const OdGiTextStyle* pTextStyle);

  // OdGiGeometrySimplifier overrides
  //
  virtual void circleProc(
    const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0);

  virtual void circleProc(
    const OdGePoint3d&, const OdGePoint3d&, const OdGePoint3d&, const OdGeVector3d* pExtrusion = 0);

  virtual void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  virtual void circularArcProc(
    const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  virtual void meshProc(OdInt32 rows,
    OdInt32 columns,
    const OdGePoint3d* pVertexList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL);

  virtual void shellProc(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL);

  virtual void rasterImageProc(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  virtual void polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
  virtual void facetOut(const OdInt32* p3Vertices, const OdInt32* pEdgeIndices, const OdGeVector3d* pNormal);

  // Acad does not use clip boundary for explode (CR 3238)
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary);
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  virtual void popClipBoundary();

  double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;
  void   setDeviation( double dDeviation );

  TD_USING(OdGiBaseVectorizer::text);
  TD_USING(OdGiGeometrySimplifier::polylineOut);

protected:
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);
  void addEntity(OdDbEntityPtr pEnt, bool needTransform = false);

  //virtual void addTransformedCopy(OdDbEntityPtr pEnt);

  void getEntityArray(OdRxObjectPtrArray& entitySet) const;

  virtual OdDbObjectId getStyleForDbText(const OdGiTextStyle& textStyle);
  OdDbObjectId createStyleForDbText(const OdGiTextStyle& textStyle);
  // Explodes the text() primitive as a set of simple OdDbText entities
  // Used for MText explode and saving to R12 format.
  void textToDbText(
    const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction,
    const OdChar* pMsg,
    OdInt32 length,
    bool raw,
    const OdGiTextStyle* pStyle);
private:
  OdDbEntityPtr makeCircle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);
  OdDbEntityPtr makePolyline(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal);
  OdDbEntityPtr makePolygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
  OdDbEntityPtr makeMPolygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList, OdCmEntityColor borderColor);
  OdList<OdDbObjectPtr> m_entityList;
  const OdGiEdgeData* m_pEdgeData;
  OdDbPolyFaceMesh*   m_pPolyFaceMesh;
  OdUInt32Array       m_contourPairStart;
  OdUInt32Array       m_contourPairEnd;
  bool                m_bClipEnabled;
  bool                m_bSaveClippedOnly;
  bool                m_bClipOn;
  bool                m_bForceFillPolygon;
  OdGiRegenType       m_uRegenType;
  double              m_dDeviation;

  TD_USING(OdGiGeometrySimplifier::triangleOut);

  static bool isElementVisible( const OdDgGraphicsElement* pDgnElement, const OdDgView* pDgnView = NULL );

  void setCurrTraitsTo(OdDbEntity* pEnt);

  void prepareToDraw(const OdGiDrawable* pDrawable);
  void restoreAfterDraw();

private:
  bool                m_bStoredForceFillPolygon;
  OdGiRegenType       m_uStoredRegenType;
};

//----------------------------------------------------------------------------------------

void IgnoredElementImportPE::subImportElement(OdDgElement*, OdDbBlockTableRecord*)
{

}

//----------------------------------------------------------------------------------------

bool OdDgnImportPE::allowImportElement(OdDgElement* e)
{
  bool bRet = true;

  if( !OdDgnImportContext::isImportInvisible() )
  {
    if( e->isKindOf(OdDgGraphicsElement::desc() ) )
    {
      OdDgGraphicsElement* pGrElm = (OdDgGraphicsElement*)(e);

      if( pGrElm->getInvisibleFlag() )
      {
        bRet = false;
      }

      if( bRet )
      {
        OdDgElementId idLevel = pGrElm->getLevelId();
        idLevel.convertToRedirectedId();

        if( !idLevel.isNull() )
        {
          OdDgLevelTableRecordPtr pElmLevel = idLevel.openObject(OdDg::kForRead);

          if( !pElmLevel.isNull() )
          {
            bRet = pElmLevel->getIsDisplayedFlag() && !pElmLevel->getIsFrozenFlag();
          }
        }
      }

      if( bRet && OdDgnImportContext::isLevelMask() )
      {
        bRet = OdDgnImportContext::checkLevelMask( pGrElm->getLevelEntryId() );
      }

      OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

      if( bRet && !pActiveView.isNull() )
      {
        if( !pActiveView->getShowConstructionFlag() && (pGrElm->getClass() == OdDgGraphicsElement::kClassConstruction) )
        {
          bRet = false;
        }

        if( !pActiveView->getShowDimensionsFlag() && (pGrElm->getElementType() == OdDgElement::kTypeDimension) )
        {
          bRet = false;
        }

        if( !pActiveView->getShowTagsFlag() && (pGrElm->getElementType() == OdDgElement::kTypeTagElement) )
        {
          bRet = false;
        }

        if( pActiveView->getHideTextsFlag() && 
            ((pGrElm->getElementType() == OdDgElement::kTypeText) || (pGrElm->getElementType() == OdDgElement::kTypeTextNode))
          )
        {
          bRet = false;
        }
      }
    }
  }

  return bRet;
}

//----------------------------------------------------------------------------------------

void OdDgnImportPE::importElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  if( allowImportElement(e) )
  {
    subImportElement( e, owner );
  }
}

//----------------------------------------------------------------------------------------

void OdDgnImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  if (e->isKindOf(OdDgGraphicsElement::desc()))
  {
    OdDgGraphicsElementPtr ge(e);
    OdStaticRxObject<OdTGDrawObjectForExplode> drawObject;

    TD_DGN_IMPORT::DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();
    OdRxVariantValue tmpVar = (OdRxVariantValue)pImporter->properties()->getAt("ImportViewIndex");
    OdInt8 uImportViewNumber = tmpVar->getInt8();

    OdDgViewPtr pDgView = OdDgnImportContext::getActiveView();

    if( pDgView.isNull() )
    {
      OdDgViewGroupPtr viewGroup = OdDgViewGroup::cast(e->database()->getActiveViewGroupId().openObject());
      if (!viewGroup.isNull())
      {
        OdDgElementIteratorPtr iterator = viewGroup->createIterator();

        if( uImportViewNumber >= 0 && uImportViewNumber < 8 )
        {
          for(; !iterator->done(); iterator->step() )
          {
            pDgView = OdDgView::cast(iterator->item().openObject( OdDg::kForWrite ));

            if( pDgView->getIndex() == uImportViewNumber )
            {
              break;
            }

            pDgView = 0;
          }
        }
        else
        {
          if( !iterator->done() )
            pDgView = OdDgView::cast(iterator->item().openObject( OdDg::kForWrite ));
        }
      }
      if (pDgView.isNull())
        pDgView = OdDgView::createObject(); // Create view with default parameters
    }

    bool bDrawPatterns = pDgView->getShowPatternsFlag();

    pDgView->setShowPatternsFlag( !OdDgnImportContext::getSkipPatternDrawFlag() );

    drawObject.setView(pDgView);

    try {
      OdRxObjectPtrArray entitySet;
      drawObject.explode(ge, e->database(), owner->database(), entitySet);
      for (unsigned int i = 0; i < entitySet.size(); ++i)
      {
        OdDbEntityPtr e = entitySet[i];
        owner->appendOdDbEntity(e);
      }
    } catch (...) {
      ODA_FAIL_ONCE();
    }

    pDgView->setShowPatternsFlag( bDrawPatterns );
  }
  else
  {
    ODA_TRACE1("Unknown DWG element %ls", e->isA()->name().c_str());
  }
}

//----------------------------------------------------------

void importSymbolPatternOnly(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  if( e->isKindOf(OdDgGraphicsElement::desc()) )
  {
    OdDgGraphicsElementPtr ge(e);
    OdStaticRxObject<OdTGDrawObjectForExplode> drawObject;
    drawObject.setSaveClippedOnlyFlag( true );

    TD_DGN_IMPORT::DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();
    OdRxVariantValue tmpVar = (OdRxVariantValue)pImporter->properties()->getAt("ImportViewIndex");
    OdInt8 uImportViewNumber = tmpVar->getInt8();

    OdDgViewPtr pDgView = OdDgnImportContext::getActiveView();

    if( pDgView.isNull() )
    {
      OdDgViewGroupPtr viewGroup = OdDgViewGroup::cast(e->database()->getActiveViewGroupId().openObject());

      if (!viewGroup.isNull())
      {
        OdDgElementIteratorPtr iterator = viewGroup->createIterator();

        if( uImportViewNumber >= 0 && uImportViewNumber < 8 )
        {
          for(; !iterator->done(); iterator->step() )
          {
            pDgView = OdDgView::cast(iterator->item().openObject( OdDg::kForWrite ));

            if( pDgView->getIndex() == uImportViewNumber )
            {
              break;
            }

            pDgView = 0;
          }
        }
        else
        {
          if( !iterator->done() )
            pDgView = OdDgView::cast(iterator->item().openObject( OdDg::kForWrite ));
        }
      }

      if (pDgView.isNull())
        pDgView = OdDgView::createObject(); // Create view with default parameters
    }

    bool bDrawPattern = false;

    if( !pDgView.isNull() )
    {
      bDrawPattern = pDgView->getShowPatternsFlag();
      pDgView->setShowPatternsFlag(true);
    }

    drawObject.setView(pDgView);

    try {
      OdRxObjectPtrArray entitySet;
      drawObject.explode(ge, e->database(), owner->database(), entitySet);
      for (unsigned int i = 0; i < entitySet.size(); ++i)
      {
        OdDbEntityPtr e = entitySet[i];
        owner->appendOdDbEntity(e);
      }
    } catch (...) {
      ODA_FAIL_ONCE();
    }

    if( !pDgView.isNull() )
    {
      pDgView->setShowPatternsFlag( bDrawPattern );
    }
  }
  else
  {
    ODA_TRACE1("Unknown DWG element %ls", e->isA()->name().c_str());
  }
}

//----------------------------------------------------------

void importDimensionSegment(OdDgElement* e, OdUInt32 uSegmentNum, const OdGeVector3d& vrDimNormal, OdDbBlockTableRecord* owner)
{
  OdDgGraphicsElementPtr ge(e);
  OdStaticRxObject<OdTGDrawObjectForExplode> drawObject;

  TD_DGN_IMPORT::DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();
  OdRxVariantValue tmpVar = (OdRxVariantValue)pImporter->properties()->getAt("ImportViewIndex");
  OdInt32 uImportViewNumber = tmpVar->getInt8();

  OdDgViewPtr pDgView = OdDgnImportContext::getActiveView();

  if( pDgView.isNull() )
  {
    OdDgViewGroupPtr viewGroup = OdDgViewGroup::cast(e->database()->getActiveViewGroupId().openObject());

    if (!viewGroup.isNull())
    {
      OdDgElementIteratorPtr iterator = viewGroup->createIterator();

      if( uImportViewNumber >= 0 && uImportViewNumber < 8 )
      {
        for(; !iterator->done(); iterator->step() )
        {
          pDgView = OdDgView::cast(iterator->item().openObject( OdDg::kForWrite ));

          if( pDgView->getIndex() == uImportViewNumber )
          {
            break;
          }

          pDgView = 0;
        }
      }
      else
      {
        if( !iterator->done() )
          pDgView = OdDgView::cast(iterator->item().openObject( OdDg::kForWrite ));
      }
    }
    if (pDgView.isNull())
      pDgView = OdDgView::createObject(); // Create view with default parameters
  }

  drawObject.setView(pDgView);

  try 
  {
    OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(vrDimNormal));

    OdRxObjectPtrArray entitySet;
    drawObject.drawDimensionSegment(ge, uSegmentNum, e->database(), owner->database(), entitySet);

    for (unsigned int i = 0; i < entitySet.size(); ++i)
    {
      OdDbEntityPtr e = entitySet[i];
      owner->appendOdDbEntity(e);
      e->transformBy( mWorld2Plane );
    }
  } 
  catch (...) 
  {
    ODA_FAIL_ONCE();
  }
}


//----------------------------------------------------------
//
// OdTGDrawObjectForExplode
//
//----------------------------------------------------------
OdTGDrawObjectForExplode::OdTGDrawObjectForExplode() 
  : m_pEdgeData(0) 
{ 
  OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts|kDrawLayerOff|kDrawLayerFrozen);
  setContext(this); 
  OdGiGeometrySimplifier::setDrawContext(OdGiBaseVectorizer::drawContext());
  m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
  output().setDestGeometry(*this);
  m_bClipEnabled = false;
  m_bClipOn = false;
  m_bForceFillPolygon = false;
  m_uRegenType = kOdGiStandardDisplay;
  m_bSaveClippedOnly = false;
  m_dDeviation = 0.0;
}

double OdTGDrawObjectForExplode::deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const
{
  return m_dDeviation;
}

void   OdTGDrawObjectForExplode::setDeviation( double dDeviation )
{
  m_dDeviation = dDeviation;
}

void OdTGDrawObjectForExplode::prepareToDraw(const OdGiDrawable* pDrawable)
{
  m_bStoredForceFillPolygon = m_bForceFillPolygon;
  m_uStoredRegenType        = m_uRegenType;

  if( pDrawable->isKindOf(OdDgCone::desc()) )
  {
    m_bForceFillPolygon = true;
  }

  if( pDrawable->isKindOf(OdDgSolid::desc()) ||
      pDrawable->isKindOf(OdDgSurface::desc())
    )
  {
    m_uRegenType = kOdGiHideOrShadeCommand;
  }
  else if( pDrawable->isKindOf(OdDgShape3d::desc()) )
  {
    if( OdDgnImportContext::getShape3dImportMode() == 2 )
    {
      m_uRegenType = kOdGiHideOrShadeCommand;
    }
  }
  else if( pDrawable->isKindOf(OdDgShape2d::desc()) )
  {
    if( OdDgnImportContext::getShape2dImportMode() == 2 )
    {
      m_uRegenType = kOdGiHideOrShadeCommand;
    }
  }
  else if( pDrawable->isKindOf(OdDgComplexShape::desc()) )
  {
    OdDgGraphicsElementPtr pComplexShape = pDrawable;

    if( !pComplexShape.isNull() && pComplexShape->get3dFormatFlag() )
    {
      if( OdDgnImportContext::getShape3dImportMode() == 2 )
      {
        m_uRegenType = kOdGiHideOrShadeCommand;
      }
    }
    else
    {
      if( OdDgnImportContext::getShape2dImportMode() == 2 )
      {
        m_uRegenType = kOdGiHideOrShadeCommand;
      }
    }
  }
  else if( pDrawable->isKindOf(OdDgCellHeader2d::desc()) )
  {
    OdDgCellHeader2dPtr pCell2d = pDrawable;

    if( !pCell2d.isNull() && pCell2d->isPolygonWithHoles() )
    {
      m_uRegenType = kOdGiHideOrShadeCommand;
    }
  }
  else if( pDrawable->isKindOf(OdDgCellHeader3d::desc()) )
  {
    OdDgCellHeader3dPtr pCell3d = pDrawable;

    if( !pCell3d.isNull() && pCell3d->isPolygonWithHoles() )
    {
      m_uRegenType = kOdGiHideOrShadeCommand;
    }
    else if( !pCell3d.isNull() )
    {
      OdDgBRepEntityPE::BRepEntityType brepType = OdDgBRepEntityPEPtr(pCell3d)->type(pCell3d);

      if( brepType != OdDgBRepEntityPE::eNotBRepEntity )
      {
        m_uRegenType = kOdGiHideOrShadeCommand;
      }
    }
  }

  // Set deviation

  if( m_uRegenType == kOdGiHideOrShadeCommand )
  {
    OdGeExtents3d  extElement;
    pDrawable->getGeomExtents( extElement );

    if( extElement.isValidExtents() )
    {
      setDeviation( extElement.maxPoint().distanceTo( extElement.minPoint()) / 2000 );
    }
  }
}

void OdTGDrawObjectForExplode::restoreAfterDraw()
{
  m_bForceFillPolygon = m_bStoredForceFillPolygon;
  m_uRegenType        = m_uStoredRegenType;
  setDeviation(0.0);
}

void OdTGDrawObjectForExplode::draw(const OdGiDrawable* pDrawable)
{
  prepareToDraw( pDrawable );

  OdGiBaseVectorizer::draw( pDrawable );

  restoreAfterDraw();
}

bool OdTGDrawObjectForExplode::isElementVisible( const OdDgGraphicsElement* pDgnElement, const OdDgView* /*pDgnView*/ )
{
  bool bRet = true;

  if( pDgnElement )
  {
    bRet = !pDgnElement->getInvisibleFlag();
  }

  return bRet;
}

void OdTGDrawObjectForExplode::setSaveClippedOnlyFlag( bool bSet )
{
  m_bSaveClippedOnly = bSet;
}

OdResult OdTGDrawObjectForExplode::drawDimensionSegment(const OdDgGraphicsElement* pEnt, 
                                                          OdUInt32 uSegmentNum, 
                                                            OdDgDatabase* pDb, 
                                                              OdDbDatabase* db, 
                                                                OdRxObjectPtrArray& entitySet)
{
  prepareToDraw( pEnt );

  OdUInt32 oldArraySize = entitySet.size();
  setDatabase(pDb);
  _dbDatabase = db;

  OdGiFillType oldFillType = fillType();

  m_nDrawableAttributes = setAttributes(pEnt); // use virtual method of OdGiBaseVectorizer

  OdDgDimensionPtr pDim = pEnt;

  OdDgDimensionExportPEPtr pDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

  if( !pDimPE.isNull() )
  {
    pDimPE->drawDimensionSegment( pDim, this, uSegmentNum );
  }

  getEntityArray(entitySet);

  setFillType( oldFillType );

  restoreAfterDraw();

  return (entitySet.size() > oldArraySize) ? eOk : eCannotExplodeEntity;
}

OdResult OdTGDrawObjectForExplode::explode(const OdDgGraphicsElement* pEnt, OdDgDatabase* pDb, OdDbDatabase* db, OdRxObjectPtrArray& entitySet)
{
  prepareToDraw( pEnt );

  OdUInt32 oldArraySize = entitySet.size();
  setDatabase(pDb);
  _dbDatabase = db;

  OdGiFillType oldFillType = fillType();

  m_nDrawableAttributes = setAttributes(pEnt); // use virtual method of OdGiBaseVectorizer

  if( m_uRegenType == kOdGiHideOrShadeCommand )
  {
    setFillType( kOdGiFillAlways );
  }

  worldDrawEntity(pEnt);
  getEntityArray(entitySet);

  setFillType( oldFillType );

  restoreAfterDraw();

  return (entitySet.size() > oldArraySize) ? eOk : eCannotExplodeEntity;
}

void OdTGDrawObjectForExplode::worldDrawEntity(const OdDgGraphicsElement* pEnt)
{
  if ( !pEnt->worldDraw(this) )
    pEnt->viewportDraw(this);
}

void OdTGDrawObjectForExplode::addEntity(OdDbEntityPtr pEnt, bool needTransform)
{
  if( m_bSaveClippedOnly && !m_bClipOn )
  {
    return;
  }

  if (pEnt.isNull())
    return;
  if (needTransform)
  {
    OdGeMatrix3d matr = getModelToWorldTransform();
    if (matr != OdGeMatrix3d::kIdentity)
    {
      if (pEnt->transformBy(matr) != eOk)
      {
        OdDbEntityPtr pOldEnt(pEnt.get());
        if (pOldEnt->getTransformedCopy(matr, pEnt) != eOk)
        {
          return;
        }
      }
    }
  }
  setCurrTraitsTo(pEnt);
  m_entityList.push_back(pEnt);
}

void OdTGDrawObjectForExplode::pushClipBoundary(OdGiClipBoundary* pBoundary )
{
  OdGiBaseVectorizer::pushClipBoundary( pBoundary );

  m_bClipEnabled = true;
  m_bClipOn = true;
}

void OdTGDrawObjectForExplode::pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pAbsBoundary )
{
  OdGiBaseVectorizer::pushClipBoundary( pBoundary, pAbsBoundary );

  m_bClipEnabled = true;
  m_bClipOn = true;
}

void OdTGDrawObjectForExplode::popClipBoundary()
{
  OdGiBaseVectorizer::popClipBoundary();

  m_bClipEnabled = false;
  m_bClipOn = false;
}

// From DgnImportElements.cpp
OdDb::LineWeight odDgnImportLineweightMapping(OdUInt32 mappingVal);
void odDgnImportSetLSModXData(OdDbEntity *pEntity, const OdGiDgLinetypeModifiers &lsMod, double lsScale);

OdCmColor convertColor(OdCmEntityColor ec, OdDgDatabase* db)
{
  OdCmColor c;

  if (ec.colorMethod() == OdCmEntityColor::kByDgnIndex)
  {
    OdCmColor cc;

    OdUInt32 color = ec.colorIndex();
    ODCOLORREF clr = OdDgColorTable::lookupRGB(db, color);

    if( color < 256 && !OdDgnImportContext::getConvertColorIndexToTrueColorFlag() )
    {
      OdDgnImportColorIndexDescription dwgColor;

      bool bSetTrueColor = true;

      if( OdDgnImportContext::getDwgColor( color, dwgColor ) )
      {
        if( !dwgColor.m_bUseTrueColor )
        {
          bSetTrueColor = false;
          c.setColorMethod(OdCmEntityColor::kByACI);
          c.setColorIndex( dwgColor.m_uDwgColorIndex );
        }
      }

      if( bSetTrueColor )
      {
        c.setRGB(ODGETRED(clr), ODGETGREEN(clr), ODGETBLUE(clr));
      }
    }
    else
    {
      c.setRGB(ODGETRED(clr), ODGETGREEN(clr), ODGETBLUE(clr));
    }
  }
  else
    c.setColor(ec.color());

  return c;
}

void OdTGDrawObjectForExplode::setCurrTraitsTo(OdDbEntity* pEnt)
{
  // ! traits are set using 'doSubentities = false' because shell faces for example
  // may have different colors
  OdDbObjectId layerId = OdDgnImportContext::getObjectId(OdDgElementId(layer()));
  if (   layerId.isNull()
      || OdDbLayerTableRecord::cast(layerId.openObject()).isNull()) // is used in audit checking also
    layerId = getDbDatabase()->getLayerZeroId();
  pEnt->setLayer(layerId, false);
  pEnt->setColor(convertColor(trueColor(), (OdDgDatabase*)database()), false);
  {
    OdDgLineStyleTableRecordPtr pLSRec = OdDgLineStyleTableRecord::cast(OdDgElementId(lineType()).openObject());
    OdUInt32 entryId = 0;
    if (!pLSRec.isNull())
      entryId = pLSRec->getEntryId();
    if (entryId < 8 && entryId >= 0)
    {
      // internal
      if (entryId == 0)
        pEnt->setLinetype(getDbDatabase()->getLinetypeContinuousId());
      else
        pEnt->setLinetype(OdDbLinetypeTable::cast(getDbDatabase()->getLinetypeTableId().openObject())->getAt(OdString().format(OD_T("DGN%u"), entryId)));
    }
    else if (entryId == OdDg::kLineStyleByLevel)
      pEnt->setLinetype(getDbDatabase()->getLinetypeByLayerId());
    else if (entryId == OdDg::kLineStyleByCell)
      pEnt->setLinetype(getDbDatabase()->getLinetypeByBlockId());
    else
      pEnt->setLinetype(getDbDatabase()->getOdDbObjectId(OdDgElementId(lineType()).getHandle()), false);
  }
  pEnt->setLinetypeScale(lineTypeScale(), false);
  if (lineStyleModifiers() && (lineStyleModifiers()->m_uFlags != 0))
    odDgnImportSetLSModXData(pEnt, *lineStyleModifiers(), lineTypeScale());
  pEnt->setLineWeight(odDgnImportLineweightMapping((OdUInt32)lineWeight()), false);
  pEnt->setPlotStyleName(plotStyleNameType(), getDbDatabase()->getOdDbObjectId(OdDgElementId(plotStyleNameId()).getHandle()), false);
  // #6702
  if( material() )
  {
    OdDbObjectId materialId = getDwgMaterialByDgnMaterial(OdDgElementId(material()), getDbDatabase() );

    if( !materialId.isNull() )
    {
      pEnt->setMaterial(materialId, false);
    }
  }

  pEnt->setVisibility( (m_nDrawableAttributes & OdGiDrawable::kDrawableIsInvisible) ? OdDb::kInvisible : OdDb::kVisible );

  if( OdDgnImportContext::isLevelMask() && (pEnt->visibility() == OdDb::kVisible) )
  {
    OdDgElementId idLevel = OdDgElementId(layer());

    if( !idLevel.isNull() )
    {
      OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);

      if( !pLevel.isNull() )
      {
        pEnt->setVisibility( OdDgnImportContext::checkLevelMask( pLevel->getEntryId() ) ? OdDb::kVisible : OdDb::kInvisible );
      }
    }
  }
}

OdDbEntityPtr OdTGDrawObjectForExplode::makeCircle(const OdGePoint3d& center,
                                                   double radius,
                                                   const OdGeVector3d& normal)
{
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setCenter(center);
  pCircle->setRadius(radius);
  pCircle->setNormal(normal);
  pCircle->setThickness(thickness());
  return pCircle;
}

void OdTGDrawObjectForExplode::circleProc(const OdGePoint3d& center,
															double radius, const OdGeVector3d& normal,
                              const OdGeVector3d* pExtrusion)
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
  }
  else
  {
    addEntity(makeCircle(center, radius, normal));
  }
}


void OdTGDrawObjectForExplode::circleProc(const OdGePoint3d& p1,
															const OdGePoint3d& p2, const OdGePoint3d& p3,
                              const OdGeVector3d* pExtrusion)
{
  if( m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circleProc( p1, p2, p3, pExtrusion );
  }
  else
  {
    OdGeCircArc3d arc(p1,p2,p3);
    addEntity(makeCircle(arc.center(), arc.radius(), arc.normal()));
  }
}


void OdTGDrawObjectForExplode::circularArcProc(const OdGePoint3d& center,
																				double radius,
																				const OdGeVector3d& normal,
																				const OdGeVector3d& startVector,
																				double sweepAngle,
																				OdGiArcType arcType,
                                        const OdGeVector3d* pExtrusion)
{
  if( pExtrusion || m_bClipEnabled )
  {
    OdGiGeometrySimplifier::circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
    return;
  }

  if( OdZero( fabs(sweepAngle) - Oda2PI) )
  {
    addEntity(makeCircle(center, radius, normal));
    return;
  }

  OdDbArcPtr pArc = OdDbArc::createObject();

  OdGeMatrix3d matrNormal;
  matrNormal.setToPlaneToWorld(normal);

  pArc->setCenter(center);
  pArc->setRadius(radius);
  pArc->setNormal(normal);
  pArc->setThickness(thickness());

  OdGeVector3d axisX = OdGeVector3d::kXAxis;
  axisX.transformBy(matrNormal);

  double angl = axisX.angleTo(startVector, normal);

  if (sweepAngle > 0)
  {
    pArc->setStartAngle(angl);
    pArc->setEndAngle(angl + sweepAngle);
  }
  else
  {
    pArc->setEndAngle(angl);
    pArc->setStartAngle(angl + sweepAngle);
  }

  addEntity(pArc);

  if (arcType == kOdGiArcSimple)
    return;

  OdDbHatchPtr h = OdDbHatch::createObject();
  h->setNormal(normal);
  h->setPattern(OdDbHatch::kPreDefined, L"SOLID");
  EdgeArray edges;

  OdGeMatrix3d projectMatrix = OdGeMatrix3d::worldToPlane(normal);
  OdGePoint3d p1, p2, p3;
  pArc->getStartPoint(p1);
  pArc->getEndPoint(p3);
  double param1, param2;
  pArc->getStartParam(param1);
  pArc->getEndParam(param2);
  pArc->getPointAtParam((param1 + param2)/2 , p2);
  p1.transformBy(projectMatrix);
  p2.transformBy(projectMatrix);
  p3.transformBy(projectMatrix);
  edges.push_back(new OdGeCircArc2d(OdGePoint2d(p1.x, p1.y), OdGePoint2d(p2.x, p2.y), OdGePoint2d(p3.x, p3.y)));
  switch(arcType)
  {
  case kOdGiArcChord:
    edges.push_back(new OdGeLineSeg2d(OdGePoint2d(p3.x, p3.y), OdGePoint2d(p1.x, p1.y)));
    break;
  case kOdGiArcSector:
    {
      OdGePoint3d c(center);
      c.transformBy(projectMatrix);
      edges.push_back(new OdGeLineSeg2d(OdGePoint2d(p3.x, p3.y), OdGePoint2d(c.x, c.y)));
      edges.push_back(new OdGeLineSeg2d(OdGePoint2d(c.x, c.y), OdGePoint2d(p1.x, p1.y)));
    }
    break;
  case kOdGiArcSimple:
    break;
  }
  h->appendLoop(OdDbHatch::kOutermost, edges);
  addEntity(h);
}

void OdTGDrawObjectForExplode::circularArcProc(const OdGePoint3d& start,
                                   const OdGePoint3d& point,
                                   const OdGePoint3d& end,
                                   OdGiArcType arcType,
                                   const OdGeVector3d* pExtrusion)
{
  OdGeCircArc3d arc(start, point, end);
  circularArcProc(arc.center(), arc.radius(), arc.normal(), start - arc.center(),
    arc.endAng() - arc.startAng(), arcType, pExtrusion);
}

OdDbEntityPtr OdTGDrawObjectForExplode::makePolyline(OdInt32 nbPoints,
                                                     const OdGePoint3d* pVertexList,
                                                     const OdGeVector3d* /*pNormal*/)
{
  if (nbPoints < 2)
  {
    ODA_FAIL_ONCE();
    return OdDbEntityPtr();
  }

  OdDbCurvePtr pObj;
  if (nbPoints == 2)
  {
    OdDbLinePtr pLine = pObj = OdDbLine::createObject();
    pLine->setStartPoint(pVertexList[0]);
    pLine->setEndPoint(pVertexList[1]);
    pLine->setThickness(thickness());
  }
  else
  {
    bool bClosed = (pVertexList[0] == pVertexList[nbPoints-1]);
    if (bClosed)
    {
      --nbPoints;
    }
    OdGeVector3d vNormal;
    if (geCalculateNormal(pVertexList, nbPoints, &vNormal) == OdGe::kOk)
    {
      OdGeMatrix3d matrix;

      if( vNormal.isParallelTo(OdGeVector3d::kZAxis) )
      {
        vNormal = OdGeVector3d::kZAxis;
      }
      else
      {
        matrix.setToWorldToPlane(vNormal);
      }

      OdDbPolylinePtr pLine = OdDbPolyline::createObject();
      pObj = pLine;
      pLine->setThickness(thickness());

      pLine->setNormal(vNormal);
      OdGePoint3d pnt = matrix*pVertexList[0];
      pLine->setElevation(pnt.z);
      matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));

      for (OdInt32 f = 0; f < nbPoints; ++f)
      {
        OdGePoint3d ptPos(matrix * pVertexList[f]);
        ODA_ASSERT_ONCE(OdZero(ptPos.z / 2., 1E-9));
        pLine->addVertexAt(f, ptPos.convert2d());
      }
      if (bClosed)
        pLine->setClosed(true);
    }
    else
    { // 3d
      OdDb3dPolylinePtr pLine = OdDb3dPolyline::createObject();
      pObj = pLine;
      ODA_ASSERT(OdZero(thickness()));

      for(OdInt32 f=0; f<nbPoints; ++f)
      {
        OdDb3dPolylineVertexPtr pVert = OdDb3dPolylineVertex::createObject();
        pVert->setPosition(pVertexList[f]);
        setCurrTraitsTo(pVert);
        pLine->appendVertex(pVert);
      }
      if (bClosed)
      {
        pLine->makeClosed();
      }
    }
  }
  return pObj;
}

void OdTGDrawObjectForExplode::polyline(OdInt32 nbPoints,
																        const OdGePoint3d* pVertexList,
																        const OdGeVector3d* pNormal,
																        OdGsMarker lBaseSubEntMarker)
{
  if (nbPoints < 2)
    return;

  if( m_bClipEnabled )
  {
    m_bClipEnabled = false;

    OdGiBaseVectorizer::polyline( nbPoints, pVertexList, pNormal, lBaseSubEntMarker );

    m_bClipEnabled = true;
  }
  else
  {
    addEntity(makePolyline(nbPoints, pVertexList, pNormal), true);
  }
}

void OdTGDrawObjectForExplode::polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  addEntity(makePolyline(nbPoints, pVertexList, 0));
}

template <class EntityType> OdDbEntityPtr makeHatchOrPolygon(
  OdInt32 nbPoints, const OdGePoint3d* pVertexList,
  OdGePoint2dArray& verts,OdGeDoubleArray& bulges)
{
  if (nbPoints < 3)
    return OdDbEntityPtr();
  OdGeVector3d vNormal;
  if (geCalculateNormal(pVertexList, nbPoints, &vNormal) == OdGe::kOk)
  {
    vNormal = odgiFaceNormal(nbPoints, pVertexList);
    OdGeMatrix3d matrix;
    if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
    {
      vNormal = OdGeVector3d::kZAxis;
    }
    else
    {
      matrix.setToWorldToPlane(vNormal);
    }
    OdSmartPtr<EntityType> pHatch = EntityType::createObject();
    pHatch->setNormal(vNormal);
    OdGePoint3d pnt = matrix * pVertexList[0];
    pHatch->setElevation(pnt.z);
    matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));
    for (OdInt32 f = 0; f < nbPoints; ++f)
    {
      OdGePoint3d ptPos(matrix * pVertexList[f]);
      ODA_ASSERT(OdZero(ptPos.z / 2.));
      verts.append(ptPos.convert2d());
      bulges.append(0.0);
    }
    pHatch->setPattern(OdDbHatch::kPreDefined, L"SOLID");
    return pHatch;
  }
  else
  {
    ODA_FAIL_ONCE();
    return OdDbEntityPtr();
  }

}

OdDbEntityPtr OdTGDrawObjectForExplode::makeMPolygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList, OdCmEntityColor borderColor)
{
  OdGePoint2dArray verts;
  OdGeDoubleArray bulges;
  OdDbEntityPtr pEnt = makeHatchOrPolygon<OdDbMPolygon>(nbPoints, pVertexList, verts, bulges);
  if (!pEnt.isNull())
  {
    OdDbMPolygonPtr pHatch = pEnt;
    pHatch->appendMPolygonLoop(verts, bulges);
    pHatch->setPatternColor(convertColor(borderColor, (OdDgDatabase*)database()));
  }
  return pEnt;
}

OdDbEntityPtr OdTGDrawObjectForExplode::makePolygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  if (nbPoints < 3)
    return OdDbEntityPtr();
  else if (nbPoints == 3 || nbPoints == 4)
  {
    OdDbSolidPtr pSolid = OdDbSolid::createObject();
    OdGeVector3d normal;
    if(geCalculateNormal(pVertexList, nbPoints, &normal)==OdGe::kOk)
    {
      normal = odgiFaceNormal(nbPoints, pVertexList); // AMark : #5910 : geCalculateNormal don't take vertices order into account,
                                                      //   so need recalculate for correct normal orientation (CW, CCW)
      pSolid->setNormal(normal);
    }
    pSolid->setPointAt(0, *pVertexList++);
    pSolid->setPointAt(1, *pVertexList++);
    pSolid->setPointAt(3, *pVertexList);
    if(nbPoints==4)
    {
      ++pVertexList;
    }
    pSolid->setPointAt(2, *pVertexList);
    return pSolid;
  }
  else
  {
    OdGePoint2dArray verts;
    OdGeDoubleArray bulges;
    OdDbEntityPtr pEnt = makeHatchOrPolygon<OdDbHatch>(nbPoints, pVertexList, verts, bulges);
    if (!pEnt.isNull())
    {
      OdDbHatchPtr pHatch = pEnt;
      pHatch->appendLoop(OdDbHatch::kOutermost|OdDbHatch::kPolyline, verts, bulges);
      return pHatch;
    }
    else
    {
      ODA_FAIL_ONCE();
      return OdDbEntityPtr();
    }
  }
}

void OdTGDrawObjectForExplode::polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  if( m_bClipEnabled )
  {
    m_bClipEnabled = false;

    OdGiBaseVectorizer::polygon( nbPoints, pVertexList );

    m_bClipEnabled = true;
  }
  else if (!OdGiGeometrySimplifier::fillMode() && !m_bForceFillPolygon)
  {
    addEntity(makePolyline(nbPoints, pVertexList, 0), true);
  }
  else
  {
    if (drawFlags() & OdGiSubEntityTraits::kDrawContourFill)
      addEntity(makeMPolygon(nbPoints, pVertexList, secondaryTrueColor()), true);
    else
      addEntity(makePolygon(nbPoints, pVertexList), true);
  }
}

void OdTGDrawObjectForExplode::facetOut(const OdInt32* pFaceList,
                                        const OdInt32* pEdgeIndices,
                                        const OdGeVector3d* /*pNormal*/)
{
    OdDbFaceRecordPtr pFace = OdDbFaceRecord::createObject();
    setCurrTraitsTo(pFace);
    const OdUInt8* vis = 0;
    if(m_pEdgeData)
      vis = m_pEdgeData->visibility();
    int n = *pFaceList++;
    for(int i=0; i < n; ++i)
    {
      pFace->setVertexAt(i, OdInt16(pFaceList[i] + 1));
      OdInt32 nEdgeIndex = pEdgeIndices[i];

      if( nEdgeIndex >= 0 && vis && vis[nEdgeIndex] == kOdGiInvisible )
      {
        pFace->makeEdgeInvisibleAt(i);
      }
      else
      {
        bool bAdditionalEdge = true;

        OdUInt32 uStartEdge = pFaceList[i];
        OdUInt32 uEndEdge = ( i == (n-1) ) ? pFaceList[0] : pFaceList[i+1];

        for( OdUInt32 k = 0; k < m_contourPairStart.size(); k++ )
        {
          if( ((uStartEdge == m_contourPairStart[k]) && (uEndEdge == m_contourPairEnd[k])) ||
            ((uStartEdge == m_contourPairEnd[k]) && (uEndEdge == m_contourPairStart[k]))
            )
          {
            bAdditionalEdge = false;
            break;
          }
        }

        if( bAdditionalEdge )
        {
          pFace->makeEdgeInvisibleAt(i);
        }
      }
    }
    m_pPolyFaceMesh->appendFaceRecord(pFace);
}

void OdTGDrawObjectForExplode::shellProc(OdInt32 nbVertex,
                                         const OdGePoint3d* pVertexList,
                                         OdInt32 faceListSize,
                                         const OdInt32* pFaceList,
                                         const OdGiEdgeData* pEdgeData,
                                         const OdGiFaceData* pFaceData,
                                         const OdGiVertexData* pVertexData)
{
  if (nbVertex <= 4 && faceListSize <= 5
    && pFaceData == 0
    && (pEdgeData == 0 ||
          (pEdgeData->colors() == NULL &&
           pEdgeData->trueColors() == NULL &&
           pEdgeData->layerIds() == NULL
           )
        )
     )
  {
    OdDbFacePtr pFace = OdDbFace::createObject();
    setCurrTraitsTo(pFace);
    int i;
    int nVerts = *pFaceList++;
    for (i = 0; i < nVerts; i++)
    {
      pFace->setVertexAt((OdUInt16)i, pVertexList[*pFaceList++]);
    }
    if (nVerts == 3)
    {
      --pFaceList;
      pFace->setVertexAt(3, pVertexList[*pFaceList]);
    }
    if (pEdgeData)
    {
      const OdUInt8 * pVisibilities = pEdgeData->visibility();
      if (pVisibilities)
      {
        for (i = 0; i < nVerts; i++)
        {
          if (pVisibilities[i] == kOdGiInvisible)
            pFace->makeEdgeInvisibleAt((OdUInt16)i);
        }
      }
    }
    addEntity(pFace);
  }
  else if(nbVertex < 0x7FFF)
  {
      OdDbPolyFaceMeshPtr pPolyFaceMesh = m_pPolyFaceMesh = OdDbPolyFaceMesh::createObject();
      OdDbPolyFaceMeshVertexPtr pVertex;
      for(int i = 0; i<nbVertex; ++i)
      {
        pVertex = OdDbPolyFaceMeshVertex::createObject();
        setCurrTraitsTo(pVertex);
        pVertex->setPosition(pVertexList[i]);
        pPolyFaceMesh->appendVertex(pVertex);
      }

      for( OdInt32 k = 0; k < faceListSize; k++ )
      {
        OdUInt32 nPointsInContour = Od_abs(pFaceList[k]);

        k++;

        for( OdUInt32 l = 0; l < nPointsInContour-1; l++, k++ )
        {
          m_contourPairStart.push_back(pFaceList[k]);
          m_contourPairEnd.push_back(pFaceList[k+1]);
        }

        m_contourPairStart.push_back(pFaceList[k - nPointsInContour+1]);
        m_contourPairEnd.push_back(pFaceList[k]);

        if( OdZero( pVertexList[pFaceList[k]].distanceTo(pVertexList[pFaceList[k - nPointsInContour+1]])) )
        {
          m_contourPairStart.push_back(pFaceList[k - nPointsInContour+1]);
          m_contourPairEnd.push_back(pFaceList[k - 1]);
        }
      }

      setVertexData(nbVertex, pVertexList, pVertexData);
      m_pEdgeData = pEdgeData;
      generateShellFacets(faceListSize, pFaceList, pFaceData, 4);

      m_contourPairStart.clear();
      m_contourPairEnd.clear();
      m_pEdgeData = 0;

      if( pPolyFaceMesh->numFaces() > 0 )
        addEntity(pPolyFaceMesh);

      ODA_ASSERT_ONCE((pPolyFaceMesh->numFaces()==0) == (pPolyFaceMesh->numVertices()==0));
  }
  else
  {
    OdInt32 startPosFc(0);
    OdInt32 endPosFc(0);
    OdInt32 startPosFcDt(0);
    OdInt32 endPosFcDt(0);
    OdInt32 startPosEdgeDt(0);
    OdInt32 endPosEdgeDt(0);
    OdInt32 sumVtx(0);
    OdInt32 curNumVtx(0);
    OdInt32 curAtt = pFaceList[startPosFc];
    OdInt32 attempts = curAtt;

    /*should not be any hole*/
    for(OdInt32 i = 0; i < faceListSize; i++)
    {
      if (0 <= curAtt)
      {
        if (0 < attempts)
        {
          attempts--;
        }
        else
        {
          if (i + 1 < faceListSize)
          {
            curAtt = pFaceList[i + 1];
            attempts = curAtt;
          }
        }
      }
      else
      {
        return;
      }
    }

    curNumVtx = pFaceList[startPosFc];
    while (endPosFc < faceListSize)
    {
      if ((sumVtx + curNumVtx < 0x7FFF) &&
          (endPosFc + curNumVtx + 1 < faceListSize))
      {
        endPosFcDt++;
        sumVtx += curNumVtx;
        endPosFc += (curNumVtx + 1);
        curNumVtx = pFaceList[endPosFc];
      }
      else
      {
        if (endPosFc + curNumVtx + 1 == faceListSize)
        {
          endPosFcDt++;
          sumVtx += curNumVtx;
          endPosFc += (curNumVtx + 1);
        }

        OdInt32 j(0);
        OdGePoint3dArray pCurVertexList;
        OdInt32Array pMapVertexList;
        OdInt32Array pCurFaceList;
        OdGiVertexData pCurVertexData;
        OdGeVector3dArray pCurVerNormals;
        OdArray<OdCmEntityColor>pCurVerTrueColors;
        OdGiFaceData pCurFaceData;
        OdGiEdgeData pCurEdgeData;
        pMapVertexList.resize(nbVertex, -1);
        curAtt = pFaceList[startPosFc];
        attempts = curAtt;
        for (OdInt32 i = startPosFc; i < endPosFc; i++)
        {
          if (attempts == curAtt)
          {
            pCurFaceList.push_back(pFaceList[i]);
            attempts--;
          }
          else if (0 < attempts)
          {
            endPosEdgeDt++;
            if (pMapVertexList[pFaceList[i]] < 0)
            {
              pCurVertexList.push_back(pVertexList[pFaceList[i]]);
              pMapVertexList[pFaceList[i]] = j++;
              if (pVertexData)
              {
                if (pVertexData->normals())
                {
                  const OdGeVector3d* pNormals = pVertexData->normals();
                  pCurVerNormals.append(pNormals[pFaceList[i]]);
                }

                if (pVertexData->trueColors())
                {
                  const OdCmEntityColor*  pTrueColors = pVertexData->trueColors();
                  pCurVerTrueColors.append(pTrueColors[pFaceList[i]]);
                }
                pCurVertexData.setOrientationFlag(pVertexData->orientationFlag());
              }
            }

            pCurFaceList.push_back(pMapVertexList[pFaceList[i]]);
            attempts--;
          }
          else
          {
            endPosEdgeDt++;
            if (pMapVertexList[pFaceList[i]] < 0)
            {
              pCurVertexList.push_back(pVertexList[pFaceList[i]]);
              pMapVertexList[pFaceList[i]] = j++;

              if (pVertexData)
              {
                if (pVertexData->normals())
                {
                  const OdGeVector3d* pNormals = pVertexData->normals();
                  pCurVerNormals.append(pNormals[pFaceList[i]]);
                }

                if (pVertexData->trueColors())
                {
                  const OdCmEntityColor*  pTrueColors = pVertexData->trueColors();
                  pCurVerTrueColors.append(pTrueColors[pFaceList[i]]);
                }
                pCurVertexData.setOrientationFlag(pVertexData->orientationFlag());
              }
            }

            pCurFaceList.push_back(pMapVertexList[pFaceList[i]]);
            if (i + 1 < endPosFc)
            {
              curAtt = pFaceList[i + 1];
              attempts = curAtt;
            }
          }
        }

        pCurVertexData.setNormals(pCurVerNormals.asArrayPtr());
        pCurVertexData.setTrueColors(pCurVerTrueColors.asArrayPtr());

        OdUInt16Array pCurFcColors;
        OdCmEntityColorArray pCurFcTrueColor;
        OdDbStubPtrArray pCurFcLayerIds;
        OdGsMarkerArray pCurFcSelectionMarkers;
        OdUInt8Array pCurFcVisibilities;
        OdGeVector3dArray pCurFcNormals;
        OdDbStubPtrArray pCurFcMaterialIds;
        OdArray<OdGiMapper>pCurFcMappers;
        OdCmTransparencyArray pCurFcTransparency;
        if (pFaceData)
        {
#ifdef _DEBUG
          OdInt32 sumFace(0);
          curAtt = pCurFaceList[0];
          attempts = curAtt;
          for (OdUInt32 i = 0;  i < pCurFaceList.size(); i++)
          {
            if (attempts == curAtt)
            {
              sumFace++;
              attempts--;
            }
            else if (0 < attempts)
            {
              attempts--;
            }
            else
            {
              if (i + 1 < pCurFaceList.size())
              {
                curAtt = pCurFaceList[i + 1];
                attempts = curAtt;
              }
            }
          }
          ODA_ASSERT(sumFace == endPosFcDt - startPosFcDt);
#endif
          if (pFaceData->colors())
          {
            const OdUInt16* pColors = pFaceData->colors();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcColors.append(pColors[i]);
            }
            pCurFaceData.setColors(pCurFcColors.asArrayPtr());
          }

          if (pFaceData->trueColors())
          {
            const OdCmEntityColor* pTrueColor = pFaceData->trueColors();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcTrueColor.append(pTrueColor[i]);
            }
            pCurFaceData.setTrueColors(pCurFcTrueColor.asArrayPtr());
          }

          if (pFaceData->layerIds())
          {
            OdDbStub** pLayerIds = pFaceData->layerIds();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcLayerIds.append(pLayerIds[i]);
            }
            pCurFaceData.setLayers(pCurFcLayerIds.asArrayPtr());
          }

          if (pFaceData->selectionMarkers())
          {
            const OdGsMarker* pSelectionMarkers = pFaceData->selectionMarkers();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcSelectionMarkers.append(pSelectionMarkers[i]);
            }
            pCurFaceData.setSelectionMarkers(pCurFcSelectionMarkers.asArrayPtr());
          }

          if (pFaceData->visibility())
          {
            const OdUInt8* pVisibilities = pFaceData->visibility();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcVisibilities.append(pVisibilities[i]);
            }
            pCurFaceData.setVisibility(pCurFcVisibilities.asArrayPtr());
          }

          if (pFaceData->normals())
          {
            const OdGeVector3d* pNormals = pFaceData->normals();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcNormals.append(pNormals[i]);
            }
            pCurFaceData.setNormals(pCurFcNormals.asArrayPtr());
          }

          if (pFaceData->materials())
          {
            OdDbStub** pMaterialIds = pFaceData->materials();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcMaterialIds.append(pMaterialIds[i]);
            }
            pCurFaceData.setMaterials(pCurFcMaterialIds.asArrayPtr());
          }

          if (pFaceData->mappers())
          {
            const OdGiMapper* pMappers = pFaceData->mappers();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcMappers.append(pMappers[i]);
            }
            pCurFaceData.setMappers(pCurFcMappers.asArrayPtr());
          }

          if (pFaceData->transparency())
          {
            const OdCmTransparency* pTransparency = pFaceData->transparency();
            for (OdInt32 i = startPosFcDt; i < endPosFcDt; i++)
            {
              pCurFcTransparency.append(pTransparency[i]);
            }
            pCurFaceData.setTransparency(pCurFcTransparency.asArrayPtr());
          }
        }

        OdUInt16Array pCurEdColors;
        OdCmEntityColorArray pCurEdTrueColors;
        OdDbStubPtrArray pCurEdLayerIds;
        OdDbStubPtrArray pCurEdLinetypeIds;
        OdGsMarkerArray pCurEdSelectionMarkers;
        OdUInt8Array pCurEdVisibilities;
        if (pEdgeData)
        {

#ifdef _DEBUG
          OdInt32 sumEdge(0);
          curAtt = pCurFaceList[0];
          attempts = curAtt;
          for (OdUInt32 i = 0;  i < pCurFaceList.size(); i++)
          {
            if (attempts == curAtt)
            {
              attempts--;
            }
            else if (0 < attempts)
            {
              sumEdge++;
              attempts--;
            }
            else
            {
              sumEdge++;
              if (i + 1 < pCurFaceList.size())
              {
                curAtt = pCurFaceList[i + 1];
                attempts = curAtt;
              }
            }
          }
          ODA_ASSERT(sumEdge == endPosEdgeDt - startPosEdgeDt);
#endif
          if (pEdgeData->colors())
          {
            const OdUInt16* pColors = pEdgeData->colors();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdColors.append(pColors[i]);
            }
            pCurEdgeData.setColors(pCurEdColors.asArrayPtr());
          }

          if (pEdgeData->trueColors())
          {
            const OdCmEntityColor* pTrueColors = pEdgeData->trueColors();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdTrueColors.append(pTrueColors[i]);
            }
            pCurEdgeData.setTrueColors(pCurEdTrueColors.asArrayPtr());
          }

          if (pEdgeData->layerIds())
          {
            OdDbStub** pLayerIds = pEdgeData->layerIds();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdLayerIds.append(pLayerIds[i]);
            }
            pCurEdgeData.setLayers(pCurEdLayerIds.asArrayPtr());
          }


          if (pEdgeData->linetypeIds())
          {
            OdDbStub** pLinetypeIds = pEdgeData->linetypeIds();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdLinetypeIds.append(pLinetypeIds[i]);
            }
            pCurEdgeData.setLinetypes(pCurEdLinetypeIds.asArrayPtr());
          }

          if (pEdgeData->selectionMarkers())
          {
            const OdGsMarker* pSelectionMarkers = pEdgeData->selectionMarkers();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdSelectionMarkers.append(pSelectionMarkers[i]);
            }
            pCurEdgeData.setSelectionMarkers(pCurEdSelectionMarkers.asArrayPtr());
          }

          if (pEdgeData->visibility())
          {
            const OdUInt8* pVisibilities = pEdgeData->visibility();
            for (OdInt32 i = startPosEdgeDt; i < endPosEdgeDt; i++)
            {
              pCurEdVisibilities.append(pVisibilities[i]);
            }
            pCurEdgeData.setVisibility(pCurEdVisibilities.asArrayPtr());
          }
        }

#ifdef _DEBUG
        int maxVXIndx = pCurVertexList.size();
        for (OdUInt32 ii = 0; ii < pCurFaceList.size();)
        {
          for (int f = 1; f <= pCurFaceList[ii]; ++f)
          {
            ODA_ASSERT(pCurFaceList[ii+f] < maxVXIndx);
          }
          ii += pCurFaceList[ii] + 1;
        }
#endif

        OdTGDrawObjectForExplode::shellProc(pCurVertexList.size(),
                                            pCurVertexList.asArrayPtr(),
                                            pCurFaceList.size(),
                                            pCurFaceList.asArrayPtr(),
                                            &pCurEdgeData,
                                            &pCurFaceData,
                                            &pCurVertexData);
        sumVtx = 0;
        startPosFc = endPosFc;
        startPosFcDt = endPosFcDt;
        startPosEdgeDt = endPosEdgeDt;
      }
    }
  }
}

void OdTGDrawObjectForExplode::meshProc(OdInt32 rows, OdInt32 columns,
														const OdGePoint3d* pVertexList, const OdGiEdgeData* pEdgeData,
														const OdGiFaceData* pFaceData, const OdGiVertexData* )
{
  if(rows < 0x7FFF && columns < 0x7FFF)
  {
#define M rows
#define N columns

    bool bEdgeInfoPresent, bFaceInfoPresent;
    bEdgeInfoPresent = (pEdgeData!=0 && (
      pEdgeData->colors()!=0 ||
      pEdgeData->trueColors()!=0 ||
      pEdgeData->layerIds()!=0 ||
      pEdgeData->linetypeIds()!=0 ||
      pEdgeData->visibility()!=0));

    bFaceInfoPresent = (pFaceData!=0 && (
      pFaceData->colors()!=0 ||
      pFaceData->trueColors()!=0 ||
      pFaceData->layerIds()!=0 ||
      pFaceData->visibility()!=0 ||
      pFaceData->materials()!=0 ||
      pFaceData->mappers()!=0 ||
      pFaceData->transparency()!=0));

    int i,j,k;
    if(!bFaceInfoPresent)
    {
      OdDbPolygonMeshPtr pMesh = OdDbPolygonMesh::createObject();
      setCurrTraitsTo(pMesh);
      OdDbPolygonMeshVertexPtr pVertex;
      pMesh->setNSize(OdInt16(N));
      pMesh->setMSize(OdInt16(M));
      for (i=0, k=0; i<N; i++)
      {
        for (j=0; j<M; j++)
        {
          pVertex = OdDbPolygonMeshVertex::createObject();
          setCurrTraitsTo(pVertex);
          pVertex->setPosition(pVertexList[k++]);
          pMesh->appendVertex(pVertex);
        }
      }
      addEntity(pMesh);
    }
    else
    {
      OdDbPolyFaceMeshPtr pMesh = OdDbPolyFaceMesh::createObject();
      setCurrTraitsTo(pMesh);
      OdDbPolyFaceMeshVertexPtr pVertex;
      for (i=0, k=0; i<N; i++)
      {
        for (j=0; j<M; j++)
        {
          pVertex = OdDbPolyFaceMeshVertex::createObject();
          setCurrTraitsTo(pVertex);
          pVertex->setPosition(pVertexList[k++]);
          pMesh->appendVertex(pVertex);
        }
      }
      OdDbFaceRecordPtr pFace;
      for (i=1, k=0; i<M; i++)
      {
        for (j=1; j<N; j++, k++)
        {
          pFace = OdDbFaceRecord::createObject();
          pFace->setVertexAt(0, OdInt16(N * (i-1) + j-1 + 1));
          pFace->setVertexAt(1, OdInt16(N * (i-1) + j   + 1));
          pFace->setVertexAt(2, OdInt16(N *  i    + j   + 1));
          pFace->setVertexAt(3, OdInt16(N *  i    + j-1 + 1));
          if(pFaceData->visibility())
          {
            pFace->setVisibility(pFaceData->visibility()[k]==kOdGiVisible ? OdDb::kVisible : OdDb::kInvisible);
          }
          if(pFaceData->colors())
          {
            pFace->setColorIndex(pFaceData->colors()[k], false);
          }
          if(pFaceData->trueColors())
          {
            OdCmColor cmColor;
            cmColor.setColor(pFaceData->trueColors()[k].color());
            pFace->setColor(cmColor, false);
          }
          if(pFaceData->materials())
          {
            pFace->setMaterial(pFaceData->materials()[k], false);
          }
          if(pFaceData->mappers())
          {
            pFace->setMaterialMapper(pFaceData->mappers() + k, false);
          }
          if(pFaceData->transparency())
          {
            pFace->setTransparency(pFaceData->transparency()[k], false);
          }
          pMesh->appendFaceRecord(pFace);
        }
      }
      addEntity(pMesh);
    }
  }
  else
  {
    ODA_FAIL();
    // should be split
  }
}

OdDbObjectId OdTGDrawObjectForExplode::getStyleForDbText(const OdGiTextStyle& giStyle)
{
  OdDbDatabase* pDb = getDbDatabase();
  return OdDgnImportContext::getStyleForDbText( pDb, giStyle );
}

//OdDbObjectId OdTGDrawObjectForExplode::createStyleForDbText(const OdGiTextStyle& textStyle)
//{
//  ODA_ASSERT(getDatabase());
//  return getDatabase()->getTEXTSTYLE();
//}

OdDbObjectId OdTGDrawObjectForExplode::createStyleForDbText(const OdGiTextStyle& textStyle)
{
  ODA_ASSERT(getDatabase());
  OdDbDatabase* pDb = getDbDatabase();

  return OdDgnImportContext::createStyleForDbText( pDb, (OdDgDatabase*)database(), textStyle );
}

void OdTGDrawObjectForExplode::xline(const OdGePoint3d& first, const OdGePoint3d& second)
{
  OdDbXlinePtr pXLine = OdDbXline::createObject();
  pXLine->setBasePoint(first);
  pXLine->setUnitDir(second-first);
  addEntity(pXLine, true);
}

void OdTGDrawObjectForExplode::ray(const OdGePoint3d& first, const OdGePoint3d& second)
{
  OdDbRayPtr pRay = OdDbRay::createObject();
  pRay->setBasePoint(first);
  pRay->setUnitDir(second-first);
  addEntity(pRay, true);
}

void OdTGDrawObjectForExplode::pline(const OdGiPolyline& lwBuf, OdUInt32 fromIndex, OdUInt32 numSegs)
{
  OdDbPolylinePtr pPlineSrc = OdDbPolyline::cast(lwBuf.getDbPolyline());
  if (!pPlineSrc.isNull() && fromIndex==0 && numSegs==0)
  {
    OdDbPolylinePtr pPline = OdDbPolyline::createObject();
    pPline->copyFrom(pPlineSrc);
    addEntity(pPline, true);
  }
  else
  {
    OdGiBaseVectorizer::pline(lwBuf, fromIndex, numSegs);
  }
}

void OdTGDrawObjectForExplode::text(const OdGePoint3d& position,
                                    const OdGeVector3d& normal,
                                    const OdGeVector3d& direction,
                                    double height,
                                    double width,
                                    double oblique,
                                    const OdString& smsg)
{
  OdString msg = smsg;
  msg.trimRight();
  if(msg.isEmpty())
  {
    return;
  }

  OdDbTextPtr pText = OdDbText::createObject();
  OdDbDatabase* pDb = getDbDatabase();
  if (pDb)
  {
    pText->setTextStyle(pDb->getTextStyleStandardId());
  }
  pText->setNormal(normal);
  pText->setPosition(position);
  pText->setRotation(OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal));
  pText->setThickness(thickness());
  pText->setHeight(height);
  pText->setWidthFactor(width);
  pText->setOblique(oblique);
  pText->setTextString(msg);
  addEntity(pText, true);
}

void OdTGDrawObjectForExplode::text(const OdGePoint3d& position,
                                    const OdGeVector3d& normal,
                                    const OdGeVector3d& direction,
                                    const OdChar* pMsg,
                                    OdInt32 length,
                                    bool raw,
                                    const OdGiTextStyle* pStyle)
{
  OdString msg;
  if (length==-1)
  {
    msg = pMsg;
  }
  else
  {
    msg = OdString(pMsg, length);
  }
  msg.trimRight();
  if (msg.isEmpty())
  {
    return;
  }

  OdGiTextStyle defTextStyle;
  if (!pStyle)
  {
    pStyle = &defTextStyle;
  }

  OdDbDatabase* pDb = getDbDatabase();
  if (pStyle->isBackward() || pStyle->isUpsideDown() || pStyle->isVertical())
  {
    OdDbTextPtr pText = OdDbText::createObject();
    OdDbObjectId tsId = getStyleForDbText(*pStyle);
    if (tsId.isNull() && pDb)
    {
      tsId = pDb->getTEXTSTYLE();
    }
    pText->setTextStyle(tsId);
    pText->setNormal(normal);
    pText->setPosition(position);
    pText->setRotation(OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal));
    pText->setThickness(thickness());
    pText->setHeight(pStyle->textSize());
    pText->setWidthFactor(pStyle->xScale());
    pText->setOblique(pStyle->obliquingAngle());
    pText->mirrorInX(pStyle->isBackward());
    pText->mirrorInY(pStyle->isUpsideDown());
    if(raw)
    {
      msg.replace(L"%%", L"%%%%");
    }
    if(pStyle->isUnderlined())
    {
      msg = L"%%U" + msg;
    }
    if(pStyle->isOverlined())
    {
      msg = L"%%O" + msg;
    }
    pText->setTextString(msg);
    addEntity(pText, true);
  }
  else
  {
    OdDbMTextPtr pText = OdDbMText::createObject();
    pText->setDatabaseDefaults(pDb);
    OdGePoint3d mtextLocation(position);
    double rotAng = OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal);
    pText->setNormal(normal);
    pText->setAttachment(OdDbMText::kBottomLeft);
    pText->setLocation(mtextLocation);
    pText->setRotation(rotAng);
    pText->setTextHeight(pStyle->textSize());

    OdString font;

    OdFont* pFont = pStyle->getFont();

    if( pFont && pFont->isShxFont() )
    {
      OdString shxfont = pStyle->ttfdescriptor().fileName();

      OdDgFontTablePtr pFontTable = getDatabase()->getFontTable(OdDg::kForRead);

      if( !pFontTable.isNull() )
      {
        OdDgFontTableRecordPtr pFontRec = pFontTable->getFont( shxfont );

        if( !pFontRec.isNull() && pFontRec->getType() == kFontTypeRsc && OdDgnImportContext::getDgnImporter() )
        {
          OdDgnImportContext::getDgnImporter()->createShxFontFromRscFont( shxfont );
        }
      }

      if( !shxfont.isEmpty() )
      {
        if( (shxfont.find(L'.') == -1) )
        {
          shxfont += L".shx";
        }

        font.format(L"\\F%ls;", shxfont.c_str());
      }
    }
    else if( pFont )
    {
      OdTtfDescriptor descr;
      pFont->getDescriptor(descr);

      OdString fontName = descr.typeface();

      if (fontName.isEmpty())
      {
        fontName = descr.fileName();
      }

      font.format(L"\\f%ls|b%d|i%d|c%d|p%d;"
        , fontName.c_str()
        , descr.isBold() ? 1 : 0
        , descr.isItalic() ? 1 : 0
        , descr.charSet()
        , descr.pitchAndFamily());
    }

    OdString obliq;
    if (pStyle->obliquingAngle() != 0.)
    {
      obliq.format(L"\\Q%ls;", odDToStr(pStyle->obliquingAngle(), 'f', 10).c_str());
    }
    OdString opt;
    opt.format(L"%ls\\W%ls;\\T%ls;%ls\\%lc\\%lc"
      , font.c_str()
      , odDToStr(pStyle->xScale(), 'f', 10).c_str()
      , odDToStr(pStyle->trackingPercent(), 'f', 10).c_str()
      , obliq.c_str()
      , pStyle->isOverlined() ? L'O' : L'o'
      , pStyle->isUnderlined() ? L'L' : L'l');

    if ( msg.getLength() == 1 )
    {    // Single control symbols. There is no the same ACAD behavior, but there is no sense single control symbol in content.
      if ( msg[0] == '{' || msg[0] == '}' || msg[0] == '\\' ) 
      {
        msg = L"\\" + msg;
      }
    }
    pText->setContents(opt + msg);

    addEntity(pText, true);
  }
}

void OdTGDrawObjectForExplode::textToDbText(const OdGePoint3d& position,
                                            const OdGeVector3d& normal,
                                            const OdGeVector3d& direction,
                                            const OdChar* pMsg,
                                            OdInt32 length,
                                            bool raw,
                                            const OdGiTextStyle* pStyle)
{
  OdString msg;
  if (length==-1)
  {
    msg = pMsg;
  }
  else
  {
    msg = OdString(pMsg, length);
  }
  msg.trimRight();
  if (msg.isEmpty())
  {
    return;
  }

  OdDbTextPtr pText = OdDbText::createObject();
  OdDbObjectId tsId = getStyleForDbText(*pStyle);
  if (tsId.isNull() && getDbDatabase())
  {
    tsId = getDbDatabase()->getTEXTSTYLE();
  }
  pText->setTextStyle(tsId);
  pText->setNormal(normal);
  pText->setPosition(position);
  pText->setRotation(OdGeMatrix3d::planeToWorld(normal).getCsXAxis().angleTo(direction, normal));
  pText->setThickness(thickness());

  pText->setHeight(pStyle->textSize());
  pText->setWidthFactor(pStyle->xScale());
  pText->setOblique(pStyle->obliquingAngle());
  pText->mirrorInX(pStyle->isBackward());
  pText->mirrorInY(pStyle->isUpsideDown());
  if(raw)
  {
    msg.replace(OD_T("%%"),OD_T("%%%%"));
  }
  if(pStyle->isUnderlined())
  {
    msg = OD_T("%%U") + msg;
  }
  if(pStyle->isOverlined())
  {
    msg = OD_T("%%O") + msg;
  }
  pText->setTextString(msg);
  addEntity(pText, true);
}
/*
void OdTGDrawObjectForExplode::addTransformedCopy(OdDbEntityPtr pEnt)
{
  m_entityList.push_back(pEnt);
}

void OdTGDrawObjectForExplode::draw(const OdGiDrawable* pDrawable)
{
  OdDbEntityPtr pEnt = OdDbEntity::cast(pDrawable);
  if (pEnt.get())
  {
    OdDbEntityPtr pTransEnt;

    OdResult res = pEnt->getTransformedCopy(getModelToWorldTransform(), pTransEnt);
    if (res == eOk)
    {
      addTransformedCopy(pTransEnt);
    }
    else
    {
      if (res == eExplodeBeforeTransform)
      {
        OdRxObjectPtrArray explodedEntities;
        pEnt->explode(explodedEntities);
        unsigned int i;
        for (i = 0; i < explodedEntities.size(); i++)
        {
          OdDbEntityPtr pExEnt = OdDbEntity::cast(explodedEntities[i]);
          if (!pExEnt.isNull())
          {
            draw(pExEnt);
          }
        }
      }
    }
  }
  else
  {
    pDrawable->worldDraw(this);
  }
}
*/

void OdTGDrawObjectForExplode::getEntityArray(OdRxObjectPtrArray& entitySet) const
{
  int nOldSize = entitySet.size();
  entitySet.resize((unsigned int)(nOldSize + m_entityList.size()));
  OdList<OdDbObjectPtr>::const_iterator ppEnt = m_entityList.begin(), ppEnd = m_entityList.end();
  OdRxObjectPtrArray::iterator ppEntDest = entitySet.begin() + nOldSize;
  while(ppEnt != ppEnd)
  {
    *ppEntDest = *ppEnt;
    ++ppEnt;
    ++ppEntDest;
  }
}

void OdTGDrawObjectForExplode::rasterImageProc(const OdGePoint3d& origin,
                                                 const OdGeVector3d& u,
                                                   const OdGeVector3d& v,
                                                     const OdGiRasterImage* pImage, 
                                                       const OdGePoint2d* uvBoundary, 
                                                         OdUInt32 numBoundPts,
                                                           bool transparency,
                                                             double brightness,
                                                               double contrast,
                                                                 double fade )
{
  if( !pImage )
    return;

  OdDbDatabase* pDb = getDbDatabase();

  if( !pDb )
    return;

  OdDbOle2FramePtr pOleFrame = OdDbOle2Frame::createObject();
  pOleFrame->setDatabaseDefaults(pDb);
  OdOleItemHandler* pItem = pOleFrame->getItemHandler();

  if (!pItem->embedRaster(pImage, pDb))
    return;

  pOleFrame->setOutputQuality(OdDbOle2Frame::kHighGraphics);

  try
  {
    OdRectangle3d curRect;
    curRect.lowLeft = origin + v*pImage->pixelHeight();
    curRect.lowRight = origin + u*pImage->pixelWidth() + v*pImage->pixelHeight();
    curRect.upLeft = origin;
    curRect.upRight = origin + u*pImage->pixelWidth();
    pOleFrame->setPosition( curRect );

    addEntity(pOleFrame, true);
  }
  catch(...)
  {
    ODA_FAIL_ONCE();
  }
}

}
