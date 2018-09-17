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
#include "DgnImportRasterAttach.h"
#include "DgnImportCommon.h"

#include <DgRasterFrame.h>
#include <DgRasterAttach.h>
#include <DbRasterImage.h>
#include <DbRasterImageDef.h>
#include <DbDictionary.h>
#include <Gi/GiRasterWrappers.h>
#include "DgnImportContext.h"
#include "DgRaster.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

bool OdDgnRasterAttachmentImportPE::allowImportElement(OdDgElement* e)
{
  bool bRet = true;

  if( !OdDgnImportContext::isImportInvisible() )
  {
    OdDgRasterAttachmentHeader* pRaster = (OdDgRasterAttachmentHeader*)e;

    OdDgElementId idRasterFrame = pRaster->getRasterFrameId();

    if( !idRasterFrame.isNull() )
    {
      OdDgRasterFramePtr pRasterFrame = idRasterFrame.openObject(OdDg::kForRead);

      if( !pRasterFrame.isNull() )
      {
        bRet = OdDgnImportPE::allowImportElement( pRasterFrame.get());
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgnRasterAttachmentImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgRasterAttachmentHeader* pRaster = (OdDgRasterAttachmentHeader*)e;

  OdDbDatabase* pDb = owner->database();

  OdDbObjectId      imageDictId    = OdDbRasterImageDef::createImageDictionary(pDb);
  OdDbDictionaryPtr pImageDict     = imageDictId.safeOpenObject(OdDb::kForWrite);

  OdDbRasterImageDefPtr pImageDef  = OdDbRasterImageDef::createObject();

  OdString strFileName = pRaster->getFilePath();

  if( strFileName.isEmpty() )
  {
    strFileName = pRaster->getFilename();
  }

  if( (strFileName.getLength() > 7) && (strFileName.left(7).makeUpper() == L"FILE://") )
  {
    strFileName = strFileName.right( strFileName.getLength() - 7 );
  }

  OdString strDictNameBase = strFileName;

  strDictNameBase.replace(L'\\',L'/');

  if( strDictNameBase.reverseFind('/') != -1 )
  {
    strDictNameBase = strDictNameBase.right( strDictNameBase.getLength() - strDictNameBase.reverseFind('/') - 1 );
  }

  if( strDictNameBase.getLength() > 4 && strDictNameBase[strDictNameBase.getLength()-4] == L'.' )
  {
    strDictNameBase = strDictNameBase.left(strDictNameBase.getLength() - 4);
  }

  OdString strDictName = strDictNameBase;
  OdUInt32 uCount = 0;

  while( !pImageDict->getAt(strDictName).isNull() )
  {
    strDictName.format(L"_%d", uCount );

    strDictName = strDictNameBase + strDictName;

    uCount++;
  }

  OdDbObjectId imageDefId = pImageDict->setAt(strDictName, pImageDef);

  OdUInt32 uWidth  = 0;
  OdUInt32 uHeight = 0;

  pRaster->getImageSize( uWidth, uHeight );

  OdGePoint3d  ptImgOrigin;
  OdGeVector3d uVector,vVector;

  pRaster->getOrientation( ptImgOrigin, uVector, vVector);

  if( (uWidth == 0) && (uHeight == 0) )
  {
    OdGePoint2d ptRes = pRaster->getExtent();

    double dXScale = uVector.length();
    double dYScale = vVector.length();

    if( OdZero( dXScale ))
    {
      dXScale = 1.0;
    }

    if( OdZero( dYScale ))
    {
      dYScale = 1.0;
    }

    uWidth  = (OdUInt32)(OdRoundToLong(ptRes.x / dXScale));
    uHeight = (OdUInt32)(OdRoundToLong(ptRes.y / dYScale));
  }

  pImageDef->setSourceFileName(strFileName);

  OdGiRasterImage* pDgnImageBuf = pRaster->getImage();

  bool bUnloadImage = false;

  if( pDgnImageBuf )
  {
    pImageDef->setImage( pDgnImageBuf, false );
  }
  else if( pImageDef->load() != eOk )
  {
    bUnloadImage = true;
    pImageDef->setImage(OdGiRasterImageDesc::createObject(uWidth, uHeight, OdGiRasterImage::kInch));
  }

  OdDbRasterImagePtr pImage = OdDbRasterImage::createObject();
  pImage->setDatabaseDefaults(pDb);
  owner->appendOdDbEntity(pImage);

  pImage->setImageDefId(imageDefId);
  pImage->setOrientation( ptImgOrigin, uVector*uWidth, vVector*uHeight);
  pImage->setDisplayOpt(OdDbRasterImage::kShow, true);
  pImage->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);

  OdDgElementId idRasterFrame = pRaster->getRasterFrameId();

  if( !idRasterFrame.isNull() )
  {
    OdDgRasterFramePtr pRasterFrame = idRasterFrame.openObject(OdDg::kForRead);

    if( !pRasterFrame.isNull() )
    {
      pImage->setBrightness( (pRasterFrame->getBrightness() + 1.0 )*50.0  );
      pImage->setContrast( (pRasterFrame->getContrast() + 1.0) * 50.0 );

      copyEntityProperties( pRasterFrame.get(), pImage );
    }
  }

  if( pRaster->getClippingFlag() && (pRaster->getBoundaryClipNumber() > 0) )
  {
    OdGePoint2dArray borderArray;
    OdGePoint2dArray currentArray;
    OdUInt16 elementNum = 0;
    OdUInt16 vertexNum = 0;

    gpc_polygon raster;

    raster.hole = NULL;
    raster.num_contours = 1;

    raster.contour = ::gpc_alloc<gpc_vertex_list>(1);
    raster.contour[0].num_vertices = 4;
    raster.contour[0].vertex = ::gpc_alloc<gpc_vertex>(4);
    raster.contour[0].vertex[0].x = 0;
    raster.contour[0].vertex[0].y = 0;
    raster.contour[0].vertex[1].x = uWidth;
    raster.contour[0].vertex[1].y = 0;
    raster.contour[0].vertex[2].x = uWidth;
    raster.contour[0].vertex[2].y = uHeight;
    raster.contour[0].vertex[3].x = 0;
    raster.contour[0].vertex[3].y = uHeight;

    // boundary elements start
    elementNum = pRaster->getBoundaryClipNumber();

    gpc_polygon border;
    gpc_polygon result;

    border.hole = NULL;
    border.contour = NULL;
    border.num_contours = 0;
    result.hole = NULL;
    result.contour = NULL;
    result.num_contours = 0;

    for( OdUInt32 i = 0; i < elementNum; i++ )
    {
      pRaster->getBoundaryClipVertices(i, currentArray);
      vertexNum = currentArray.size();

      gpc_polygon element;
      element.num_contours = 1;
      element.hole = NULL;
      element.contour = ::gpc_alloc<gpc_vertex_list>(1);
      element.contour[0].vertex = ::gpc_alloc<gpc_vertex>((int)vertexNum);
      element.contour[0].num_vertices = (int)vertexNum;

      for( OdUInt32 j = 0; j < vertexNum; j++ )
      {
        element.contour[0].vertex[j].x = currentArray[j].x;
        element.contour[0].vertex[j].y = uHeight - currentArray[j].y;
      }

      gpc_free_polygon(&border);
      ::gpc_polygon_clip(GPC_INT, &raster, &element, &border);
      gpc_free_polygon(&result);
      ::gpc_polygon_clip(GPC_INT, &border, &element, &result);
      gpc_free_polygon(&element);     
    }

    if( border.num_contours > 0 )
    {
      vertexNum = border.contour[0].num_vertices;
      for (unsigned int i = 0; i < vertexNum; i++)
      {
        borderArray.append(OdGePoint2d(border.contour[0].vertex[i].x - 0.5, border.contour[0].vertex[i].y - 0.5));
      }
      // go to the first element of the border - that should give a proper closed polygon
      borderArray.append(OdGePoint2d(border.contour[0].vertex[0].x - 0.5, border.contour[0].vertex[0].y - 0.5));
    }

    if( borderArray.size() > 3 )
    {
      pImage->setClipBoundary( borderArray );
      pImage->setDisplayOpt( OdDbRasterImage::kClip, true );
    }

    gpc_free_polygon(&raster);
    gpc_free_polygon(&border);
    gpc_free_polygon(&result);
  }

  if( bUnloadImage && !pImageDef.isNull() )
  {
    pImageDef->setImage( NULL, false );
  }
}

//---------------------------------------------------------------------------------------------------

void OdDgRasterFrameImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgRasterFrame* pDgRaster = (OdDgRasterFrame*)e;
  OdDgElementId idRasterAttach = pDgRaster->getRasterReferenceId();
  if (idRasterAttach.isNull())
    return;
  OdDgRasterAttachmentHeaderPtr pRaster = idRasterAttach.openObject();
  if (pRaster.isNull())
    return;
}

//---------------------------------------------------------------------------------------------------

void OdDgRasterHeader2dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgnImportPE::subImportElement( e, owner );
}

//---------------------------------------------------------------------------------------------------

void OdDgRasterHeader3dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgnImportPE::subImportElement( e, owner );
}

//---------------------------------------------------------------------------------------------------

}
