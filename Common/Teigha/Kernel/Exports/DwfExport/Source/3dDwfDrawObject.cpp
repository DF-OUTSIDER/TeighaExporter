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



//////////////////////////////////////////////////////////////////////
// 3dDwfDrawObject.cpp: implementation of the Od3dDwfDevice, Od3dDwfView classes.
//////////////////////////////////////////////////////////////////////


#include "OdaCommon.h"

// to avoid STL warnings (if not to include STL here, it will include itself from whip tk)
#define STL_USING_VECTOR
#define STL_USING_LIST
#define STL_USING_MAP
#include "OdaSTL.h"

#ifdef _MSC_VER
#pragma warning(push)
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#pragma warning(disable: 4290)
#endif // _MSC_VER

#include "3dDwfDrawObject.h"
#include "3dDwfExportImpl.h"
// DWF Toolkit 7
//
#include "dwf/publisher/model/Segment.h" 
#include "dwf/package/utility/PropertyContainer.h" 

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#include "RxObjectImpl.h"
#include "Ge/GeCircArc3d.h"
#include "ColorMapping.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeLineSeg3d.h"
#include "RxVariantValue.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve3d.h"

#include "Ge/GeExtents3d.h"

using namespace TD_DWF_EXPORT;

//static DWFToolkit::DWFSegment* g_pSubpartForShell;

//================
/*
    // SegmentHandlerBuilder Interface

    TK_Close_Segment& getCloseSegmentHandler()
    TK_Open_Segment& getOpenSegmentHandler()
    TK_Referenced_Segment& getIncludeSegmentHandler()
    TK_Referenced_Segment& getStyleSegmentHandler()

    // GeometryHandlerBuilder Interface

    TK_Area_Light& getAreaLightHandler()
    TK_Circle& getCircleHandler()
    TK_Circle& getCircularArcHandler()
    TK_Circle& getCircularChordHandler()
    TK_Circle& getCircularWedgeHandler()
    TK_Cutting_Plane& getCuttingPlaneHandler()
    TK_Point& getDistantLightHandler()
    TK_Ellipse& getEllipseHandler()
    TK_Ellipse& getEllipticalArcHandler()
    TK_Image& getImageHandler()
    TK_Line& getLineHandler()
    TK_Point& getLocalLightHandler()
    TK_NURBS_Curve& getNURBSCurveHandler()
    TK_NURBS_Surface& getNURBSSurfaceHandler()
    TK_PolyCylinder& getPolyCylinderHandler()
    TK_Polypoint& getPolygonHandler()
    TK_Polypoint& getPolylineHandler()
    TK_Shell& getShellHandler( bool bTriStripsOnly = false,
                               bool bDisableOptimization = false )
    TK_Spot_Light& getSpotLightHandler()
    TK_Text& getTextHandler()
    TK_Text& getTextWithEncodingHandler()

    // FeatureHandlerBuilder Interface

    TK_Texture& getTextureHandler()
    TK_Matrix& getTextureMatrixHandler()

    // AttributeHandlerBuilder Interface

    TK_Color& getColorHandler()
    TK_Color_Map& getColorMapHandler()
    TK_Color_RGB& getColorRGBHandler()
    TK_Linear_Pattern& getEdgePatternHandler()
    TK_Size& getEdgeWeightHandler()
    TK_Enumerated& getFacePatternHandler()
    TK_Enumerated& getHandednessHandler()
    TK_Heuristics& getHeuristicsHandler()
    TK_Linear_Pattern& getLinePatternHandler()
    TK_Size& getLineWeightHandler()
    TK_Size& getMarkerSizeHandler()
    TK_Enumerated& getMarkerSymbolHandler()
    TK_Matrix& getModellingMatrixHandler()
    TK_Rendering_Options& getRenderingOptionsHandler()
    TK_Selectability& getSelectabilityHandler()
    TK_Enumerated& getTextAlignmentHandler()
    TK_Text_Font& getTextFontHandler()
    TK_Point& getTextPathHandler()
    TK_Size& getTextSpacingHandler()
    TK_User_Options& getUserOptionsHandler()
    TK_Unicode_Options& getUnicodeOptionsHandler()
    TK_Visibility& getVisibilityHandler()
    TK_Camera& getCameraHandler()
    TK_Camera& getViewHandler()
*/
//================

//////////////////////////////////////////////////////////////////////
//  Od3dDwfDevice
//////////////////////////////////////////////////////////////////////
namespace TD_DWF_EXPORT {
Od3dDwfDevice::Od3dDwfDevice() 
{
  ::odgsInitialize();
}

Od3dDwfDevice::~Od3dDwfDevice()
{
  ::odgsUninitialize();
}


//====================================================================
//
void Od3dDwfDevice::update(OdGsDCRect* pUpdatedRect)
{
  OdGsBaseVectorizeDevice::update(pUpdatedRect);
}


OdGsDevicePtr Od3dDwfDevice::createObject()
{
  OdGsDevicePtr pRes = OdRxObjectImpl<Od3dDwfDevice, OdGsDevice>::createObject();
  return pRes;
}


OdGsViewPtr Od3dDwfDevice::createView(const OdGsClientViewInfo* pViewInfo, bool bEnableLayVisPerView)
{
  OdGsViewPtr pView = Od3dDwfView::createObject();
  Od3dDwfView* pMyView = static_cast<Od3dDwfView*>(pView.get());

  pMyView->init(this, pViewInfo, bEnableLayVisPerView);

//  pMyView->screenOutput().setDestGeometry(*pMyView);

  return pMyView;
}

void Od3dDwfDevice::setContext(C3dDwfExportImpl* dwf)
{
  m_3dDwf = dwf;
}

OdDbBaseDatabase* Od3dDwfDevice::getDb()
{
  return getExportImpl()->getDB();
}


//////////////////////////////////////////////////////////////////////
//  Od3dDwfView
//////////////////////////////////////////////////////////////////////

Od3dDwfView::Od3dDwfView()
    : m_pCurrentRootSubpart(NULL)
    , m_bWriteToTheSameSegment(false)
    , m_TKOGeometry(0)
{
}

void Od3dDwfView::update()
{
  setMode(OdGsView::kGouraudShaded);
  m_pCurrentRootSubpart = device()->getExportImpl()->getCurrentSegment();
  OdGsBaseVectorizeView::update();
}


//====================================================================

void Od3dDwfView::setCurrTraits()
{
  const OdGiSubEntityTraitsData& currTraits = effectiveTraits();
  setCurrTraits( currTraits );
}


void Od3dDwfView::setCurrTraits(const OdGiSubEntityTraitsData& currTraits)
{
  // color
  if(currTraits.trueColor().isByColor())
  {
    set_Color(ODTOCOLORREF(currTraits.trueColor()));
  }
  else
  {
    set_ColorIndex(currTraits.color());
  }
}

//====================================================================
static float getRatio( int value)
{
  if ( !(value >= 0 && value < 256) )
  {
    ODA_ASSERT( false );
  }
  return ( (float)value/255 );
}
void Od3dDwfView::set_Color(ODCOLORREF rgb)
{
  TK_Color_RGB* pColorRGB = device()->getExportImpl()->getTKColorRGB();

  pColorRGB->SetGeometry ( getTKOGeometry() );
  pColorRGB->SetRGB( getRatio( ODGETRED(rgb) ), getRatio( ODGETGREEN(rgb) ), getRatio( ODGETBLUE(rgb) ) );
  pColorRGB->serialize();

}

void Od3dDwfView::set_ColorIndex(OdUInt16 colorIndex)
{
  set_Color(device()->getColor(colorIndex));
}


//====================================================================

bool Od3dDwfView::regenAbort() const
{
  // return true here to abort the rendering process
  return false;  
}


//====================================================================

void Od3dDwfView::beginViewVectorization()
{
  OdGsBaseVectorizer::beginViewVectorization();

  // setting destination geometry recipient (in this device it coincides with view)
  output().setDestGeometry(*this);
  OdGeMatrix3d w3DTransform; // 3D DWF is [Front] -> [Top] transformed (cf. #9714)
  w3DTransform.setCoordSystem(OdGePoint3d::kOrigin, OdGeVector3d::kXAxis, -OdGeVector3d::kZAxis, OdGeVector3d::kYAxis);
  setEyeToOutputTransform(w3DTransform*getEyeToWorldTransform());

  // setting draw context to simplifier
  OdGiGeometrySimplifier::setDrawContext(OdGsBaseVectorizer::drawContext());

}

void Od3dDwfView::endViewVectorization()
{
  OdGsBaseVectorizer::endViewVectorization();
  OdGiGeometrySimplifier::setDrawContext(0);
}


//====================================================================
// Geometry processing
//====================================================================

//====================================================================

void Od3dDwfView::polylineProc(OdInt32 nPoints, 
                              const OdGePoint3d* pVertexList,
                              const OdGeVector3d* pNormal,
                              const OdGeVector3d* pExtrusion,
                              OdGsMarker lBaseSubEntMarker)
{
  if (m_bWriteToTheSameSegment)
  {
    polylineProcEx( nPoints, pVertexList, pNormal, pExtrusion, lBaseSubEntMarker );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Polyline");

    polylineProcEx( nPoints, pVertexList, pNormal, pExtrusion, lBaseSubEntMarker );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::polylineProcEx(OdInt32 nPoints, 
                              const OdGePoint3d* pVertexList,
                              const OdGeVector3d* /*pNormal*/,
                              const OdGeVector3d* pExtrusion,
                              OdGsMarker /*lBaseSubEntMarker*/)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  if ( pExtrusion && !pExtrusion->isZeroLength() )
  {
    OdGiGeometrySimplifier::polylineProc(nPoints, pVertexList, NULL, pExtrusion);
  }
  else
  {
    setTKOGeometry( TKO_Geo_Line ); 
    setCurrTraits();

    TK_Line& rLine = getRootSubpart()->getLineHandler();

    for (OdInt32 j = 1; j < nPoints; j++)
    {
      rLine.SetPoints( pVertexList[j - 1].x,
                       pVertexList[j - 1].y,
                       pVertexList[j - 1].z,
                       pVertexList[j].x,    
                       pVertexList[j].y,    
                       pVertexList[j].z );  

      rLine.serialize();
    }

    // here could be a set of object properties
    //
    /*
    DWFToolkit::DWFProperty oProperty( L"", L"" );
    oProperty.setName( L"zzzzz" );
    oProperty.setValue( L"xxxx" );
    oProperty.setCategory( L"ccccc" );
    oSubpart.addProperty( oProperty );
    */
  }

  WriteToTheSameSegment( bPreviousState );
}


//====================================================================
void Od3dDwfView::polygonProc(OdInt32 nPoints,
                               const OdGePoint3d* pVertexList,
                               const OdGeVector3d* pNormal,
                               const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    polygonProcEx( nPoints, pVertexList, pNormal, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Polygon");

    polygonProcEx( nPoints, pVertexList, pNormal, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::polygonProcEx(OdInt32 nPoints,
                               const OdGePoint3d* pVertexList,
                               const OdGeVector3d* pNormal,
                               const OdGeVector3d* pExtrusion)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  if ( pExtrusion && !pExtrusion->isZeroLength() )
  {
    OdGiGeometrySimplifier::polygonProc(nPoints, pVertexList, NULL, pExtrusion);
    return;
  }

  if (effectiveTraits().fillType() != kOdGiFillNever)
  {
    OdInt32* pFaceList = new OdInt32[nPoints + 1];
    pFaceList[0] = nPoints;
    for (OdInt32 j = 0; j < nPoints; j++)
    {
      pFaceList[j + 1] = j;
    }

    {
      setShell( nPoints, pVertexList, nPoints + 1/*1*/, pFaceList, NULL, NULL, NULL, pNormal );
    }

    delete [] pFaceList;
  }
  else
  {
    // Draw closed polyline
    OdGePoint3dArray points( nPoints + 1 );
    points.insert( points.end(), pVertexList, pVertexList + nPoints );
    points.push_back( pVertexList[0] );
    polylineProc( ++nPoints, points.getPtr(), pNormal );
  }

  WriteToTheSameSegment( bPreviousState );
}

//====================================================================
//
inline bool setFaceTraits(OdGiSubEntityTraits& traits, const OdGiFaceData* pFaceData, int faceIndex)
{
  if (!pFaceData)
  {
    return true;
  }

  if (pFaceData->visibility() && pFaceData->visibility()[faceIndex] == kOdGiInvisible)
  {
    return false;
  }

  if (pFaceData->colors())
  {
    traits.setColor( pFaceData->colors()[faceIndex] );
  }

  if (pFaceData->layerIds())
  {
    traits.setLayer( pFaceData->layerIds()[faceIndex] );
  }

  return true;
}

inline bool setEdgeTraits(OdGiSubEntityTraits& traits, const OdGiEdgeData* pEdgeData, int edgeIndex)
{
  if (!pEdgeData)
  {
    return true;
  }

  if (pEdgeData->visibility() && pEdgeData->visibility()[edgeIndex] == kOdGiInvisible)
  {
    return false;
  }
  if (pEdgeData->colors())
  {
    traits.setColor( pEdgeData->colors()[edgeIndex] );
  }

  if(pEdgeData->layerIds())
  {
    traits.setLayer( pEdgeData->layerIds()[edgeIndex] );
  }

  if(pEdgeData->linetypeIds())
  {
    traits.setLineType( pEdgeData->linetypeIds()[edgeIndex] );
  }

  return true;
}

//====================================================================

//====================================================================
void Od3dDwfView::meshProc(OdInt32 rows,
            OdInt32 columns,
            const OdGePoint3d* pVertexList,
            const OdGiEdgeData* pEdgeData,
            const OdGiFaceData* pFaceData,
            const OdGiVertexData* pVertexData)
{
  if (m_bWriteToTheSameSegment)
  {
    meshProcEx( rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Mesh");

    meshProcEx( rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::meshProcEx(
            OdInt32 rows,
            OdInt32 columns,
            const OdGePoint3d* pVertexList,
            const OdGiEdgeData* pEdgeData,
            const OdGiFaceData* pFaceData,
            const OdGiVertexData* /*pVertexData*/)
{
  bool bPreviousState = WriteToTheSameSegment( true );

/*OdGiGeometrySimplifier::meshProc(rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);
  WriteToTheSameSegment( bPreviousState );
return;
*/

  {
    OdGiSubEntityTraits& traits = subEntityTraits();

    OdGsView::RenderMode renderMode = mode();

    bool bFillOn = (( renderMode == OdGsView::k2DOptimized 
                      || renderMode>OdGsView::kHiddenLine) 
                    && effectiveTraits().fillType() == kOdGiFillAlways);

    bool bFillOff = !bFillOn 
                    || renderMode > OdGsView::kGouraudShaded;
  
    OdInt32 row, col, nVertex, nEdge, nFaceIndex;
    OdInt32 nrows = rows, ncols = columns;

    --nrows;
    --ncols;

    if (bFillOn)
    {
      OdGePoint3d ptsPolygon[4];

      for (row= 0; row < nrows; ++row)
      {
        for(col= 0; col < ncols; ++col)
        {
          nFaceIndex = row * (columns - 1) + col;

          if (::setFaceTraits( traits, pFaceData, nFaceIndex++ ) && effectiveTraits().visibility())
          {
            setCurrTraits();

            nVertex = row * columns + col;
            ptsPolygon[0] = pVertexList[ nVertex ];
            ptsPolygon[3] = pVertexList[ nVertex + 1 ];

            nVertex += columns;

            ptsPolygon[1] = pVertexList[ nVertex ];
            ptsPolygon[2] = pVertexList[ nVertex + 1 ];

            polygonProc( 4, ptsPolygon );
          }
        }
      }
    }

    if (bFillOff)
    {
      OdGePoint3d ptsLine[2];

      for (row = 0; row < nrows; ++row)
      {
        for (col = 0; col < ncols; ++col)
        {
          nVertex = row * columns       + col;
          nEdge   = row * (columns - 1) + col;

          if (::setEdgeTraits( traits, pEdgeData, nEdge ) && effectiveTraits().visibility())
          {
            setCurrTraits();

            ptsLine[0] = pVertexList[nVertex];
            ptsLine[1] = pVertexList[nVertex + 1];
            polylineProc( 2, ptsLine );
          }
        
          nEdge   = (columns - 1) * rows + row + col * (rows - 1);

          if (::setEdgeTraits( traits, pEdgeData, nEdge ) && effectiveTraits().visibility())
          {
            setCurrTraits();

            ptsLine[0] = pVertexList[nVertex];
            ptsLine[1] = pVertexList[nVertex + columns];
            polylineProc( 2, ptsLine );
          }
        }
      }

      for (row = 0; row < nrows; ++row)
      {
        nEdge   = ncols * rows + ncols * nrows + row;

        if (::setEdgeTraits( traits, pEdgeData, nEdge ) && effectiveTraits().visibility())
        {
          setCurrTraits();

          nVertex = row * columns + ncols;

          ptsLine[0] = pVertexList[nVertex];
          ptsLine[1] = pVertexList[nVertex + columns];
          polylineProc( 2, ptsLine );
        }
      }

      for (col = 0; col < ncols; ++col)
      {
        nEdge   = row * (columns - 1) + col;

        if (::setEdgeTraits( traits, pEdgeData, nEdge ) && effectiveTraits().visibility())
        {
          setCurrTraits();

          nVertex = row * columns + col;

          ptsLine[0] = pVertexList[nVertex];
          ptsLine[1] = pVertexList[nVertex + 1];
          polylineProc( 2, ptsLine );
        }
      }
    }
  }

  WriteToTheSameSegment( bPreviousState );
}


//====================================================================

void Od3dDwfView::setShell(
            OdInt32 nbVertex,
            const OdGePoint3d* pVertexList,
            OdInt32 faceListSize,
            const OdInt32* pFaceList,
            const OdGiEdgeData* /*pEdgeData*/,
            const OdGiFaceData* pFaceData,
            const OdGiVertexData* /*pVertexData*/,
            const OdGeVector3d* pNormal
            )

{
  // Set Fill on/ off for faces

  // this isn't gradient shell
/*
  OdGsView::RenderMode renderMode = mode();
  bool bFillOn = (( renderMode == OdGsView::k2DOptimized 
                    || renderMode > OdGsView::kHiddenLine ) 
                  && effectiveTraits().fillType() == kOdGiFillAlways );

  bool bFillOff = !bFillOn 
                  || renderMode > OdGsView::kGouraudShaded;

  if (!bFillOn && bFillOff)
  {
  const OdInt32* pFaceListEnd = pFaceList + faceListSize;
  // const OdInt32* pFaceListStart = pFaceList;
  OdInt32 nFaceSize  = 0;
  OdInt32 nEdgeIndex = 0;
  OdInt32 nFaceIndex = 0;

  OdGePoint3d ptsLine[2];

  OdGiSubEntityTraits& traits = subEntityTraits();

  while(pFaceList < pFaceListEnd)
  {
    if (::setFaceTraits( traits, pFaceData, nFaceIndex++ ) && effectiveTraits().visibility())
    {
      setCurrTraits();

      const OdInt32* pSaveFaceList = pFaceList;

      if(bFillOn)
      {
        ODA_ASSERT(*pFaceList > 0);

        nFaceSize = Od_abs((int)*pFaceList);
        ++pFaceList;

        OdArray<OdGePoint3d>  dcLoops( nbVertex );
      
        while (nFaceSize--)
        {
          dcLoops.append( pVertexList[*pFaceList] );
          ++pFaceList;
        }
        if (pFaceList < pFaceListEnd && *pFaceList < 0) // hole
        {
          OdIntArray counts;
          counts.append( dcLoops.size() );

          do
          {
            nFaceSize = Od_abs((int)*pFaceList);
            ++pFaceList;
            counts.append(nFaceSize);

            while (nFaceSize--)
            {
              dcLoops.append( pVertexList[*pFaceList] );
              ++pFaceList;
            }
          }
          while (pFaceList < pFaceListEnd && *pFaceList < 0);

          int iAllCounts = 0;
          for ( int i = 0, nCounts = counts.size(); i < nCounts; i++)
          {
            int nSize = counts[i];
            polygonProc( nSize, dcLoops.getPtr() + iAllCounts );  // to output the polygon
            iAllCounts += nSize;
          }
        }
        else
        {
          polygonProc( dcLoops.size(), dcLoops.getPtr() );        // to output the polygon
        }
      }

      if (bFillOff)
      {
        pFaceList = pSaveFaceList;
        
        const OdGePoint3d* pPoint;
        do
        {
          nFaceSize = Od_abs((int)*pFaceList);
          ++pFaceList;
          
          OdInt32 nFirstPoint = *pFaceList;
          ++pFaceList;
          
          pPoint = pVertexList + nFirstPoint;
          ptsLine[0] = *pPoint;                                   // to set the start point of Line

          while (--nFaceSize)
          {
            pPoint = pVertexList + *pFaceList;
            if (::setEdgeTraits(traits, pEdgeData, nEdgeIndex++) && effectiveTraits().visibility())
            {
              setCurrTraits();

              ptsLine[1] = *pPoint;                               // to set the end point of Line
              polylineProc( 2, ptsLine );                         // to output the polyline
              ptsLine[0] = ptsLine[1];                            // to reset the start point of Line
            }
            else
            {
              ptsLine[0] = *pPoint;                               // to set the start point of Line
            }
            ++pFaceList;
          }
          if (::setEdgeTraits(traits, pEdgeData, nEdgeIndex++) && effectiveTraits().visibility())
          {
            setCurrTraits();

            pPoint = pVertexList + nFirstPoint;
            ptsLine[1] = *pPoint;                                 // to set the end point of Line
            polylineProc( 2, ptsLine );                           // to output the polyline
            ptsLine[0] = ptsLine[1];                              // to reset the start point of Line
          }
        }
        while (pFaceList < pFaceListEnd && *pFaceList < 0);
      }
    }
    else // skip face
    {
      do
      {
        nFaceSize = Od_abs((int)*pFaceList);
        ++pFaceList;
        pFaceList += nFaceSize;

        if (nFaceSize > 2)
        {
          nEdgeIndex += nFaceSize;
        }
        else
        {
          ++nEdgeIndex;
        }
      }
      while (pFaceList < pFaceListEnd && *pFaceList < 0);
    }
  }
  }
  else
*/
  {
  OdGeExtents3d shellExts;
  float* points = new float[nbVertex * 3];
  for (OdInt32 j = 0; j < nbVertex; j++)
  {
    shellExts.addPoint( pVertexList[j] );

    points[j*3]     = pVertexList[j].x;
    points[j*3 + 1] = pVertexList[j].y;
    points[j*3 + 2] = pVertexList[j].z;
  }

  float bbox[6];
  bbox[0] = shellExts.minPoint().x;
  bbox[1] = shellExts.minPoint().y;
  bbox[2] = shellExts.minPoint().z;
  bbox[3] = shellExts.maxPoint().x;
  bbox[4] = shellExts.maxPoint().y;
  bbox[5] = shellExts.maxPoint().z;

  setTKOGeometry( TKO_Geo_Face ); 
  setCurrTraits();

  TK_Shell& rShell = getRootSubpart()->getShellHandler(false, true);

  rShell.SetBounding( bbox );
  rShell.SetPoints( nbVertex, points );                   // set the vertex list for the shell
  rShell.SetFaces( faceListSize, (const int*)pFaceList ); // set the face list for the shell

  if ( (pFaceData && pFaceData->normals()) || pNormal )
  {
    float* normals = new float[nbVertex * 3];
    if ( pFaceData && pFaceData->normals() )
    {
      const OdGeVector3d* pNoramls = pFaceData->normals();
      OdInt32 iEntries = 0;
      for ( OdInt32 i = 0, nFaces = 0; i < faceListSize; /*nFaces++, i++ */)
      {
        iEntries = pFaceList[i];                  // get the number of entries in current face
        i++;
        int nFaceEntries = Od_abs((int)iEntries);
        if (iEntries > 0)
        {
          nFaces++;     // counts the number of faces not holes for getting normal correctly
                        // Note: The hole should be ignored calculating the faces number.
                        //       It differs from the number of entries in pFaceList.
                        //       Mention about it was made in ObjectARX Reference AcGiGeometry::shell() function.
        }

        // Any holes in a face must immediately follow the definition of the containing face.
        for (int k = 0; k < nFaceEntries; k++)
        {
          int j = pFaceList[i + k];
          const OdGeVector3d vNoraml = pNoramls[nFaces - 1];
          normals[j*3]     = vNoraml.x;
          normals[j*3 + 1] = vNoraml.y;
          normals[j*3 + 2] = vNoraml.z;
        }
        i += nFaceEntries;                        // go to the last item of current face
      }
    }
    else if (pNormal)
    {
      for (OdInt32 j = 0; j < nbVertex; j++)
      {
        normals[j*3]     = pNormal->x;
        normals[j*3 + 1] = pNormal->y;
        normals[j*3 + 2] = pNormal->z;
      }
    }
    rShell.SetVertexNormals( normals );

    delete [] normals;
  }

  rShell.serialize();

  delete [] points;
  }
}


void Od3dDwfView::shellProc(
            OdInt32 nbVertex,
            const OdGePoint3d* pVertexList,
            OdInt32 faceListSize,
            const OdInt32* pFaceList,
            const OdGiEdgeData* pEdgeData,
            const OdGiFaceData* pFaceData,
            const OdGiVertexData* pVertexData)
{
  if (m_bWriteToTheSameSegment)
  {
    shellProcEx( nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Shell");

    shellProcEx( nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::shellProcEx(
            OdInt32 nbVertex,
            const OdGePoint3d* pVertexList,
            OdInt32 faceListSize,
            const OdInt32* pFaceList,
            const OdGiEdgeData* pEdgeData,
            const OdGiFaceData* pFaceData,
            const OdGiVertexData* pVertexData)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  {
    const OdCmEntityColor* pColors = NULL;
    if(pVertexData)
    {
      pColors = pVertexData->trueColors();
    }

    if (pColors != NULL)
    {
        //device()->dc_gouraud_polytriangle( pVertexList, faceListSize, pFaceList, pColors );
/*
  // it is shell with gradient fill
  for(int i = 0; i < faceListSize;)
  {
#ifdef _DEBUG
    OdInt32 numVerts = 
#endif
    pFaceList[i++];
    ODA_ASSERT(numVerts == 3);
    WT_Logical_Point vP[3];
    WT_RGBA32        rgba[3];
    for(OdInt32 k = 0; k < 3; k ++)
    {
      OdGePoint3d pt(pVertexList[pFaceList[i]]);
      OdCmEntityColor col(pColors[pFaceList[i]]);
      i ++;

      // converting to...
      RoundPoint(vP[k], OdGePoint2d(pt.x, pt.y));
      rgba[k] = WT_RGBA32(col.red(), col.green(), col.blue(), 255);
    }
    WT_Gouraud_Polytriangle gpt(3, vP, rgba, WD_True);
    dc_gouraud_polytriangle(gpt);
  }
*/
    }
    else
    {
      //g_pSubpartForShell = &oSubpart;
      setShell( nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData );
    }
  }

  WriteToTheSameSegment( bPreviousState );
}


//====================================================================

void Od3dDwfView::shapeProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            int shapeNo, 
            const OdGiTextStyle* pStyle,
            const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    shapeProcEx( position, u, v, shapeNo, pStyle, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Shape");

    shapeProcEx( position, u, v, shapeNo, pStyle, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::shapeProcEx(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            int shapeNo, 
            const OdGiTextStyle* pStyle,
            const OdGeVector3d* pExtrusion)
{
  bool bPreviousState = WriteToTheSameSegment( true );

    OdGiGeometrySimplifier::shapeProc( position, u, v, shapeNo, pStyle, pExtrusion );

  WriteToTheSameSegment( bPreviousState );
}


//====================================================================
void Od3dDwfView::nurbsProc(
            const OdGeNurbCurve3d& nurbs)
{
  if (m_bWriteToTheSameSegment)
  {
    nurbsProcEx( nurbs );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Nurbs");

    nurbsProcEx( nurbs );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::nurbsProcEx(
            const OdGeNurbCurve3d& nurbs)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  {
    setTKOGeometry( TKO_Geo_Line ); 
    setCurrTraits();

    TK_NURBS_Curve& rNurbs = getRootSubpart()->getNURBSCurveHandler();

    int options = 0;
/*
03603 #define NC_HAS_WEIGHTS 0x01     
03604 #define NC_HAS_KNOTS   0x02     
03605 #define NC_HAS_START   0x04     
03606 #define NC_HAS_END     0x08
*/
    int               degree;
    bool              rational;
    bool              periodic;
    OdGeKnotVector    vKnots;
    OdGePoint3dArray  controlPoints;
    OdGeDoubleArray   weights;

    nurbs.getDefinitionData( degree, rational, periodic, vKnots, controlPoints, weights );

    int control_count = controlPoints.size();
    float* pPoints = new float[control_count * 3];
    const OdGePoint3d* pPointsArray = controlPoints.getPtr();
    for (int i = 0; i < control_count; i++ )
    {
      pPoints[i*3]     = pPointsArray[i].x;
      pPoints[i*3 + 1] = pPointsArray[i].y;
      pPoints[i*3 + 2] = pPointsArray[i].z;
    }

    float* pWeights = NULL;             // If spline is not rational, weights array will be empty.
    if (rational)
    {
      options |= NC_HAS_WEIGHTS;
      int weights_count = weights.size();
      pWeights = new float[weights_count];

      const double* pWeightsArray = weights.getPtr();
      for (int i = 0; i < weights_count; i++ )
      {
        pWeights[i] = pWeightsArray[i];
      }
    }

    float fStart = 0;
    float fEnd = 1;
    //double knotTol = vKnots.tolerance();
    OdUInt32 numKnots = vKnots.logicalLength();
    float* pKnots = NULL;
    if (numKnots)
    {
      options |= NC_HAS_KNOTS;
      pKnots = new float[numKnots];
      double * pVKnot = vKnots.asArrayPtr();
      for (OdUInt32 i = 0; i < numKnots; i++ )
      {
        pKnots[i] = *pVKnot++;
      }

    
      options |= NC_HAS_START | NC_HAS_END;
      fStart = ((nurbs.startParam() - pKnots[0])/(pKnots[numKnots - 1] - pKnots[0]));
      fEnd = ((nurbs.endParam() - pKnots[0])/(pKnots[numKnots - 1] - pKnots[0]));
      if (fStart < 0 || fStart > 1)
      {
        fStart = 0;
      }
      if (fEnd >1 || fEnd < fStart)
      {
        fEnd = 1;
      }
    }

    rNurbs.SetCurve(degree, control_count, pPoints, pWeights, pKnots, fStart, fEnd);
    rNurbs.SetOptions( options );

    rNurbs.serialize();

    delete [] pPoints;
    if (pWeights)
    {
      delete [] pWeights;
    }
    if (pKnots)
    {
      delete [] pKnots;
    }
  }

  WriteToTheSameSegment( bPreviousState );
}


//====================================================================
void Od3dDwfView::textProc(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool raw, 
            const OdGiTextStyle* pStyle,
            const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    textProcEx( position, u, v, msg, length, raw, pStyle, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Text");

    textProcEx( position, u, v, msg, length, raw, pStyle, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::textProcEx(
            const OdGePoint3d& position,
            const OdGeVector3d& u, 
            const OdGeVector3d& v,
            const OdChar* msg, 
            OdInt32 length, 
            bool raw, 
            const OdGiTextStyle* pStyle,
            const OdGeVector3d* pExtrusion)
{
  if (length == 0)  
  {
    return;               // Text must be not empty
  }

  double dRotation = OdGeVector3d::kXAxis.angleTo( u, OdGeVector3d::kZAxis);
  // rotated text modifies modeling matrix
  bool bPreviousState = WriteToTheSameSegment( OdZero(dRotation) );
  {
    if (!pStyle->isTtfFont())
    {
      // for SHX font
      //
      ODA_ASSERT(pStyle->isShxFont());

      OdGiGeometrySimplifier::textProc( position, u, v, msg, length, raw, pStyle, pExtrusion );
    }
    else
    {
      // for True Type Font only

      setTKOGeometry( TKO_Geo_Text );
      setCurrTraits();

      // Prepare Font
      //
      int nMask = 0;
      TK_Text_Font& rFont = getRootSubpart()->getTextFontHandler();
      OdString fontName = pStyle->ttfdescriptor().typeface();
      if ( fontName.isEmpty() )
        fontName = pStyle->ttfdescriptor().fileName();
      rFont.SetNames( (const char*)fontName );

      nMask |= TKO_Font_Names;              // the name of the font (i.e. which font to use)

      if (pStyle->isUnderlined())       
      {
        nMask |= TKO_Font_Underline;        // add an underline to the font
      }
      if (pStyle->isOverlined())
      {
        nMask |= TKO_Font_Overline;         // add an overline to the font
      }

      float fWidthScale = pStyle->xScale();
      if ( !OdZero(fWidthScale - 1) )
      {
        rFont.SetWidthScale( fWidthScale );
        nMask |= TKO_Font_Width_Scale;      // scale factor for width
      }
                                            // The desired rotation of the individual characters within the font
      float fSlant = OdaPI2 - u.angleTo(v, OdGeVector3d::kZAxis);
      if ( !OdZero(fSlant) )
      {
        rFont.SetSlant( fSlant );
        nMask |= TKO_Font_Slant;            // character slant
      }

      // to get height
      OdGeVector2d yDir = (const OdGeVector2d&)v;

      // Note: It is necessary for height to take into account as rotation as oblique
      //       If an oblique angle is equal to 0 than the height can be taken simply 
      //       as vector length without taking into account a rotation.
      //       Otherwise to transform the vector to the 1st quadrant and take the Y value.
      double dY = yDir.length();
      if (!OdZero( fSlant ))
      {
        if (!OdZero(dRotation))
        {
          yDir.rotateBy( dRotation );
        }
        dY = yDir.y;
      }
      float fHeight = dY;
      rFont.SetSize( fHeight );
      nMask |= TKO_Font_Size;               // the size at which to draw characters

      int iTKO_Font_Size_Units = TKO_Font_Size_Object; //TKO_Font_Size_Screen;
      rFont.SetSizeUnits( iTKO_Font_Size_Units );

      rFont.SetTransforms( TKO_Font_Transform_Full );
         
      nMask |= TKO_Font_Transforms;

      rFont.SetMask( nMask );
      rFont.SetValue( nMask );

      rFont.serialize();

      if ( !OdZero(dRotation) )
      {
        TK_Matrix& m = getRootSubpart()->getModellingMatrixHandler();
        OdGeMatrix3d gm;
        gm.setToRotation(dRotation, OdGeVector3d::kZAxis, position);
        float data[16];
        for (int i = 0; i < 4; i++)
        {
          for (int j = 0; j < 4; j++)
          {
            data[i*4+j] = gm(j,i);
          }
        }
        m.SetMatrix(data);
        m.serialize();
      }

      // Text
      //
      TK_Enumerated& rEnumerated = getRootSubpart()->getTextAlignmentHandler();
      rEnumerated.SetIndex( TKO_Text_Alignment_Lower_Left );
      rEnumerated.serialize();

      //TK_Text& rText = oSubpart.getTextHandler();
      TK_Text& rText = getRootSubpart()->getTextWithEncodingHandler();

      rText.SetEncoding( TKO_Enc_ISO_Latin_One/*TKO_Enc_Unicode*/ );     // todo: look at 'enum TKO_Text_Encodings'
      rText.SetPosition( position.x, position.y, position.z );
      rText.SetString( OdW2US(msg) );
      rText.serialize();
    }
  }

  WriteToTheSameSegment( bPreviousState );
}

//====================================================================
void Od3dDwfView::setCircleArc(const OdGePoint3d& point1, const OdGePoint3d& point2, const OdGePoint3d& point3)
{
  setTKOGeometry( TKO_Geo_Line ); 
  setCurrTraits();

//  const OdGePoint3d* pPoints = pointArray.getPtr();

  //TK_Circle& rCircle = getRootSubpart()->getCircleHandler();
  TK_Circle& rCircleArc = getRootSubpart()->getCircularArcHandler();

  rCircleArc.SetStart( point1.x, point1.y, point1.z );
  rCircleArc.SetMiddle( point2.x, point2.y, point2.z );
  rCircleArc.SetEnd( point3.x, point3.y, point3.z );

  rCircleArc.serialize();
}


void Od3dDwfView::circleProc(const OdGePoint3d& center,
                             double radius,
                             const OdGeVector3d& normal,
                             const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    circleProcEx( center, radius, normal, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Circle");

    circleProcEx( center, radius, normal, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::circleProcEx(const OdGePoint3d& center,
                             double radius,
                             const OdGeVector3d& normal,
                             const OdGeVector3d* pExtrusion)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  if ( pExtrusion && !pExtrusion->isZeroLength() )
  {
    OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
  }
  else if (OdZero(radius))
  {
    // to transform to point at this case
    OdGePoint3d points[2];
    points[1] = points[0] = center;

    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;

    polylineProc(2, points, normals, pExtrusion, -1);
  }
  else
  {
    OdGeCircArc3d circle( center, normal, radius );
    OdGePoint3dArray pointArray;
    circle.getSamplePoints( 5, pointArray );

    const OdGePoint3d* pPoints = pointArray.getPtr();

    setCircleArc(pPoints[0], pPoints[1], pPoints[2]);
    setCircleArc(pPoints[2], pPoints[3], pPoints[0]);
  }

  WriteToTheSameSegment( bPreviousState );
}

//====================================================================
//
void Od3dDwfView::circleProc(const OdGePoint3d& ptStart,
                              const OdGePoint3d& pt2,
                              const OdGePoint3d& ptEnd,
                              const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    circleProcEx( ptStart, pt2, ptEnd, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Circle");

    circleProcEx( ptStart, pt2, ptEnd, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::circleProcEx(const OdGePoint3d& ptStart,
                              const OdGePoint3d& pt2,
                              const OdGePoint3d& ptEnd,
                              const OdGeVector3d* pExtrusion)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(ptStart, pt2, ptEnd, error);

  if (error != OdGe::kOk)
  {
    OdGiGeometrySimplifier::circleProc(ptStart, pt2, ptEnd, pExtrusion);
  }
  else
  {
    circleProc(circArc.center(), circArc.radius(), circArc.normal(), pExtrusion);
  }

  WriteToTheSameSegment( bPreviousState );
}

//====================================================================
//
void Od3dDwfView::circularArcProc(const OdGePoint3d& center,
                                   double radius,
                                   const OdGeVector3d& normal,
                                   const OdGeVector3d& startVector,
                                   double sweepAngle,
                                   OdGiArcType arcType,
                                   const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    circularArcProcEx( center, radius, normal, startVector, sweepAngle, arcType, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"CircularArc");

    circularArcProcEx( center, radius, normal, startVector, sweepAngle, arcType, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::circularArcProcEx(const OdGePoint3d& center,
                                   double radius,
                                   const OdGeVector3d& normal,
                                   const OdGeVector3d& startVector,
                                   double sweepAngle,
                                   OdGiArcType arcType,
                                   const OdGeVector3d* pExtrusion)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  if ( pExtrusion && !pExtrusion->isZeroLength() )
  {
    OdGiGeometrySimplifier::circularArcProc( center, radius, normal, startVector, sweepAngle, arcType, pExtrusion );
  }
  else if (OdZero(radius))
  {
    // to transform to point at this case
    OdGePoint3d points[2];
    points[1] = points[0] = center;

    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;

    polylineProc(2, points, normals, pExtrusion, -1);
  }
  else
  {
    OdGeCircArc3d circArc;
    circArc.set(center, normal, startVector, radius, 0.0, sweepAngle);

    OdGePoint3dArray pointArray;
    circArc.getSamplePoints( 3, pointArray );

    const OdGePoint3d* pPoints = pointArray.getPtr();

    setCircleArc(pPoints[0], pPoints[1], pPoints[2]);
  }

  WriteToTheSameSegment( bPreviousState );
}


//====================================================================
//
void Od3dDwfView::circularArcProc(const OdGePoint3d& start,
                                   const OdGePoint3d& point,
                                   const OdGePoint3d& end,
                                   OdGiArcType arcType,
                                   const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    circularArcProcEx( start, point, end, arcType, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"CircularArc");

    circularArcProcEx( start, point, end, arcType, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::circularArcProcEx(const OdGePoint3d& start,
                                   const OdGePoint3d& point,
                                   const OdGePoint3d& end,
                                   OdGiArcType arcType,
                                   const OdGeVector3d* pExtrusion)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(start, point, end, error);

  if (error != OdGe::kOk)
  {
    OdGiGeometrySimplifier::circularArcProc(start, point, end, arcType, pExtrusion);
  }
  else if ( pExtrusion && !pExtrusion->isZeroLength() )
  {
    OdGeVector3d startVec(circArc.refVec().rotateBy(circArc.startAng(), circArc.normal()));
    OdGiGeometrySimplifier::circularArcProc( circArc.center(), circArc.radius(), circArc.normal(), 
                                             startVec, circArc.endAng() - circArc.startAng(), 
                                             arcType, pExtrusion );
  }
  else if (OdZero(circArc.radius()))
  {
    // to transform to point at this case
    OdGePoint3d points[2];
    points[1] = points[0] = circArc.center();

    OdGeVector3d normals[2];
    normals[1] = normals[0] = circArc.normal();

    polylineProc(2, points, normals, pExtrusion, -1);
  }
  else
  {
    setCircleArc(start, point, end);
  }

  WriteToTheSameSegment( bPreviousState );
}


//====================================================================
//
void Od3dDwfView::ellipArcProc(const OdGeEllipArc3d& arc,
                               const OdGePoint3d* pEndPointsOverrides,
                               OdGiArcType arcType,
                               const OdGeVector3d* pExtrusion)
{
  if (m_bWriteToTheSameSegment)
  {
    ellipArcProcEx( arc, pEndPointsOverrides, arcType, pExtrusion );
  }
  else
  {
    DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
    DWFToolkit::DWFSegment* pRootSubpart = setRootSubpart( &oSubpart );

    oSubpart.open(L"Ellipse");

    ellipArcProcEx( arc, pEndPointsOverrides, arcType, pExtrusion );

    oSubpart.close();
    setRootSubpart( pRootSubpart );
  }
}

void Od3dDwfView::ellipArcProcEx(const OdGeEllipArc3d& arc,
                               const OdGePoint3d* pEndPointsOverrides,
                               OdGiArcType arcType,
                               const OdGeVector3d* pExtrusion)
{
  bool bPreviousState = WriteToTheSameSegment( true );

  if ( pExtrusion && !pExtrusion->isZeroLength() )
  {
    OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
  }
  else
  {
    OdGeEllipArc3d ellipArc(arc);   // for transformation

    OdGe::ErrorCondition ret;
    ret = OdGe::kOk;
    OdGeVector3d normal = ellipArc.normal();
    normal.normalize( OdGeContext::gTol, ret );

    double dRadius = (ellipArc.majorRadius() > ellipArc.minorRadius()) 
                        ? ellipArc.majorRadius() 
                        : ellipArc.minorRadius();

    if (OdZero(dRadius))
    {
      // to transform to point at this case
      OdGePoint3d points[2];
      points[1] = points[0] = ellipArc.center();

      OdGeVector3d normals[2];
      if (ret == OdGe::kOk)
      {
        normals[1] = normals[0] = normal;
      }

      polylineProc(2, points, (ret == OdGe::kOk) ? normals : NULL, pExtrusion, -1);
    }
    else if (ret == OdGe::kOk)
    {
      // Because ACAD dosn't support TK_Ellipse yet correctly

      //setTKOGeometry( TKO_Geo_Line ); 
      //setCurrTraits();

      if (ellipArc.isClosed())
      {
        OdGePoint3dArray pointArray;
        OdInt32 nPoints = 122;
        ellipArc.getSamplePoints( nPoints + 1, pointArray );
        const OdGePoint3d* pPoints = pointArray.getPtr();

        OdInt32* pFaceList = new OdInt32[nPoints + 1];
        pFaceList[0] = nPoints;
        for (OdInt32 j = 0; j < nPoints; j++)
        {
          pFaceList[j + 1] = j;
        }
        setShell( nPoints, pPoints, nPoints + 1, pFaceList, NULL, NULL, NULL, &normal );
        delete [] pFaceList;

        /*
        TK_Ellipse& rEllipse = getRootSubpart()->getEllipseHandler();

        OdGePoint3d point;

        point = ellipArc.center();
        rEllipse.SetCenter( point.x, point.y, point.z );

        OdGeVector3d vector;
        vector = ellipArc.majorAxis();
        rEllipse.SetMajor( vector.x, vector.y, vector.z );

        vector = ellipArc.minorAxis();
        rEllipse.SetMinor( vector.x, vector.y, vector.z );

        rEllipse.serialize();
        */
      }
      else
      {
        OdGiGeometrySimplifier::ellipArcProc( ellipArc, pEndPointsOverrides, arcType, pExtrusion );

        /*
        TK_Ellipse& rEllipseArc = getRootSubpart()->getEllipticalArcHandler();

        OdGePoint3d point;

        point = ellipArc.center();
        rEllipseArc.SetCenter( point.x, point.y, point.z );

        OdGeVector3d vector;
        vector = ellipArc.majorAxis();
        rEllipseArc.SetMajor( vector.x, vector.y, vector.z );

        vector = ellipArc.minorAxis();
        rEllipseArc.SetMinor( vector.x, vector.y, vector.z );

        rEllipseArc.SetLimits( ellipArc.startAng(), ellipArc.endAng() );

        rEllipseArc.serialize();
        */
      }
    }
    else
    {
      OdGiGeometrySimplifier::ellipArcProc( ellipArc,
                                            pEndPointsOverrides,
                                            arcType,
                                            pExtrusion );
    }
  }

  WriteToTheSameSegment( bPreviousState );
}

//====================================================================
//
void Od3dDwfView::xlineProc(const OdGePoint3d& /*pt1*/, const OdGePoint3d& /*pt2*/)
{
  // ACAD doesn't support it for W3D
/*
  DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
  oSubpart.open(L"XLine");

  OdGePoint3d ptsLine[2];

  ptsLine[0] = pt1;
  ptsLine[1] = pt2;
  polylineProc( 2, ptsLine );
*/
}

//====================================================================
//
void Od3dDwfView::rayProc(const OdGePoint3d& /*pt1*/, const OdGePoint3d& /*pt2*/)
{
  // ACAD doesn't support it for W3D
/*
  DWFToolkit::DWFSegment oSubpart = getRootSubpart()->openSegment();
  oSubpart.open(L"Ray");

  OdGePoint3d ptsLine[2];

  ptsLine[0] = pt1;
  ptsLine[1] = pt2;
  polylineProc( 2, ptsLine );
*/
}
}
