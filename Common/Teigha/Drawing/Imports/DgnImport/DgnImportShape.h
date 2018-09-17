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

#ifndef _DGN_IMPORT_SHAPE_INCLUDED_ 
#define _DGN_IMPORT_SHAPE_INCLUDED_

#include "DgnImportPE.h"
#include "DgnImportCommon.h"
#include "DgnImportPatterns.h"
#include <OdaCommon.h>
#include "DbRegion.h"
#include "DgShape.h"
#include <DbFace.h>
#include <DgAttributeLinkage.h>

namespace TD_DGN_IMPORT 
{

//---------------------------------------------------------------------------------------------------

template <class T> OdGePoint3d getShapeVertex(T*, OdUInt32 uIndex){return OdGePoint3d::kOrigin;}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getShapeVertex<OdDgShape2d>(OdDgShape2d* shape2d, OdUInt32 uIndex)
{
  OdGePoint2d ptVertex = shape2d->getVertexAt(uIndex);

  return OdGePoint3d( ptVertex.x, ptVertex.y, 0 );
}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getShapeVertex<OdDgShape3d>(OdDgShape3d* shape3d, OdUInt32 uIndex)
{
  return shape3d->getVertexAt(uIndex);
}

//---------------------------------------------------------------------------------------------------

template <class T> struct OdDgShapeImportPE : OdDgnImportPE
{
  void subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner) ODRX_OVERRIDE
  {
    T* pShape = static_cast<T*>(e);

    OdUInt8 uShapeImportMode = OdDgnImportContext::getShape2dImportMode();

    if( pShape->get3dFormatFlag() )
    {
      uShapeImportMode = OdDgnImportContext::getShape3dImportMode();
    }

    bool bImportAsPolylineMode = OdDgnImportContext::getImportCurvesOnlyFlag();

    if( (uShapeImportMode == 2) && !bImportAsPolylineMode )
    {
      OdDgnImportPE::subImportElement( e, owner );
      return;
    }

    OdGePoint3dArray arrVertices;

    for( OdUInt32 j = 0; j < pShape->getVerticesCount(); j++ )
    {
      arrVertices.push_back( getShapeVertex(pShape,j) );
    }

    if( arrVertices[0] == arrVertices[arrVertices.size()-1] )
    {
      arrVertices.removeLast();
    }

    if( arrVertices.size() < 3 )
    {
      return;
    }

    bool     bUseFillColor   = false;
    OdUInt32 uFillColorIndex = 0;

    OdRxObjectPtrArray arrFillLinkages;

    pShape->getLinkages( OdDgAttributeLinkage::kFillStyle, arrFillLinkages );

    for( OdUInt32 i = 0; i < arrFillLinkages.size(); i++ )
    {
      if( arrFillLinkages[i]->isKindOf(OdDgFillColorLinkage::desc() ) )
      {
        bUseFillColor = true;
        OdDgFillColorLinkagePtr pFillColor = arrFillLinkages[i];
        uFillColorIndex = pFillColor->getColorIndex();
        break;
      }
    }

    OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

    if( !pActiveView.isNull() && !pActiveView->getShowPatternsFlag() )
    {
      bUseFillColor = false;
    }

    OdDbObjectId idCreatedObject;

    bool bImportAsPolyline = OdDgnImportContext::getImportCurvesOnlyFlag();

    if( (uShapeImportMode == 0) || bImportAsPolylineMode ) // create polyline
    {
      bImportAsPolyline = true;
    }

    if( bUseFillColor || bImportAsPolyline ) // Create polyline and solid hatch
    {
      OdDbPolylinePtr pPolyline;
      OdDbHatchPtr    pHatch;

      if( createVertexPolyline( arrVertices, pPolyline, pShape->getThickness() ) )
      {
        if( bUseFillColor )
        {
          pHatch = OdDbHatch::createObject();
          owner->appendOdDbEntity( pHatch );

          owner->appendOdDbEntity( pPolyline );

          OdDbObjectIdArray arrHatchItems;
          arrHatchItems.push_back( pPolyline->id() );

          pHatch->appendLoop( OdDbHatch::kExternal, arrHatchItems );
          pHatch->setPattern( OdDbHatch::kPreDefined, L"SOLID" );
          pHatch->setAssociative(true);
          OdDbObjectIdArray arrIds;
          arrIds.push_back(pPolyline->objectId());
          pHatch->setAssocObjIdsAt(0,arrIds);

          idCreatedObject = pPolyline->objectId();
        }
        else
        {
          owner->appendOdDbEntity( pPolyline );
          idCreatedObject = pPolyline->objectId();
        }
      }

      if( !pPolyline.isNull() )
      {
        OdDgnImportPathToDwgObject dwgPath;
        dwgPath.m_idPath.objectIds().push_back( pPolyline->objectId() );
        dwgPath.m_bExists = true;
        OdDgnImportContext::addObjectPath( pShape->elementId(), dwgPath );

        copyEntityProperties( pShape, pPolyline);
      }

      if( !pHatch.isNull() )
      {
        copyEntityProperties( pShape, pHatch);
        setDwgColorByDgnIndex( pShape->database(), pHatch, uFillColorIndex );
      }
    }
    else // Create Face or Region
    {
      if( arrVertices.size() < 5 && ( getLineStyleEntryId(pShape) == 0) ) // create face
      {
        OdDbFacePtr pFace = OdDbFace::createObject();

        pFace->setVertexAt(0, arrVertices[0] );
        pFace->setVertexAt(1, arrVertices[1] );
        pFace->setVertexAt(2, arrVertices[2] );

        if( arrVertices.size() > 3 )
        {
          pFace->setVertexAt(3, arrVertices[3] );
        }
        else
        {
          pFace->setVertexAt(3, arrVertices[2] );
        }

        owner->appendOdDbEntity( pFace );

        copyEntityProperties( pShape, pFace );

        idCreatedObject = pFace->objectId();

        OdDgnImportPathToDwgObject dwgPath;
        dwgPath.m_idPath.objectIds().push_back( idCreatedObject );
        dwgPath.m_bExists = true;
        OdDgnImportContext::addObjectPath( pShape->elementId(), dwgPath );
      }
      else
      {
        OdDbPolylinePtr pPolyline;

        if( createVertexPolyline( arrVertices, pPolyline, pShape->getThickness() ) )
        {
          OdRxObjectPtrArray arrPolylines;
          arrPolylines.push_back( (OdRxObjectPtr)(pPolyline) );

          OdRxObjectPtrArray arrRegions;

          if( OdDbRegion::createFromCurves(arrPolylines,arrRegions) == eOk )
          {
            OdDbRegionPtr pRegion = arrRegions[0];

            owner->appendOdDbEntity( pRegion );

            copyEntityProperties( pShape, pRegion );

            idCreatedObject = pRegion->objectId();

            OdDgnImportPathToDwgObject dwgPath;
            dwgPath.m_idPath.objectIds().push_back( idCreatedObject );
            dwgPath.m_bExists = true;
            OdDgnImportContext::addObjectPath( pShape->elementId(), dwgPath );
          }
          else
          {        
            OdDgnImportPE::subImportElement( pShape, owner );
            return;
          }
        }
        else
        {
          OdDgnImportPE::subImportElement( pShape, owner );
          return;
        }
      }
    }

    OdDbObjectIdArray arrIds;

    if( !idCreatedObject.isNull() )
    {
      arrIds.push_back( idCreatedObject );
    }

    if( !bImportAsPolylineMode )
    {
      importPatterns( pShape, arrVertices, arrIds, true, owner, true, idCreatedObject );
    }
  }

};

//---------------------------------------------------------------------------------------------------

}
#endif // _DGN_IMPORT_SHAPE_INCLUDED_
