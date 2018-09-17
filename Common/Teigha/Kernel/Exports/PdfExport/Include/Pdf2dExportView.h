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
//
//
//////////////////////////////////////////////////////////////////////

#ifndef _PDF_2D_EXPORT_VIEW_INCLUDED_
#define _PDF_2D_EXPORT_VIEW_INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000

#include "2dExportDevice.h"
#include "RxObjectImpl.h"
#include "OdPlatform.h"
#include "Gi/GiPlotGenerator.h"
#include "PdfExportClipper.h"
#include "Pdf2dExportDevice.h"

using namespace TD_PDF;

//////////////////////////////////////////////////////////////////////

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF_2D_EXPORT {

/** \details
  This class implements the two-dimensional export view.
*/
class PDF2dExportView : public Od2dExportView
{
  OdString                m_SearchableText; //collector for text annotation for searchable text

  OdGeMatrix3d            m_WorldToCameraMatrix; // matrix of transformation from world to camera coordinates

  OdGiPlotGeneratorPtr    m_pPlotstyler; // Plot style generator.
  mutable OdGiDrawablePtr m_pSolidHatch; // Smart pointer to /OdDbHatch/ class instance implementing solid hatch mode.

  mutable bool            m_bPRCMode;
  mutable bool            m_bExtentsMode; // Determines if the view supports extents mode.
  mutable bool            m_bHatchToBitmapMode; // Determines if the view can be hatched to bitmap mode (*true*) or can not (*false*).
  bool                    m_bAdditionalClip;
  bool                    m_bDarkenGSSet;

  OdGsDCRect              m_screenRect;

protected:
/** \details
  Add hyperlink if exists
*/
  void processHyperlinks(const OdGiDrawable* pDrawable, const OdString &layerName = OdString());

  /** \details
  Add text annotation if necessary
  */
  void processSearchableText(const OdGiDrawable* pDrawable, const OdString &layerName = OdString());

  void processPRC(const OdGiDrawable* pDrawable);

/** \details
    Pushes a clip to the view port.
*/
  void pushViewportClip();
  
/** \details
    Pops a clip from the view port.
*/
  void popViewportClip();

  bool isFrozenLayer(const OdString &layerName) const;

/** \details
    Views extended specified block.

    \remarks
	Returns true if view successfully done or false in the other case.

	\param extents [in]  A reference to the OdGeBoundBlock3d class instance that must be viewed.
*/
    bool viewExtents(OdGeBoundBlock3d& extents) const;
public:

  // Constructor. 

  PDF2dExportView() 
    : m_bExtentsMode(false)
    , m_bHatchToBitmapMode(false)
    , m_bPRCMode(false)
    , m_bAdditionalClip(false)
    , m_SearchableText("")
    , m_bDarkenGSSet(false)
  {
    setDrawContextFlags(OdGiConveyorContext::kPlineAllowArcProc, true);
  }

/** \details
    Traits modifying handler for the export view.

    \remarks
    Process modified traits with specified data for the view.
*/
  void onTraitsModified();

  // PseudoConstructor. 

  /** \details
    Creating the view.

    \remarks
    Returns an instance of the OdGsViewPtr class.
  */
  static OdGsViewPtr createObject()
  {
    return OdRxObjectImpl<PDF2dExportView, OdGsView>::createObject();
  }

/** \details
    Returns the pointer to the export device associated with the export view.

    \remarks
    Returns a pointer to PDF2dExportDevice object.
*/
   PDF2dExportDevice* device()
  { 
    return (PDF2dExportDevice*)Od2dExportView::device();
  }

/** \details
    Returns plot generator for the export view.

    \remarks
    Returns a reference to an instance of the OdGiPlotGenerator class.
*/
   OdGiPlotGenerator& plotGenerator();

  // --------  Od2dExportView  --------
/** \details
    Starts the vectorization of the export view.
*/
  virtual void beginViewVectorization();
  virtual void update();
  virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);

  virtual void loadViewport();
  
/** \details
    Ends the vectorization of the export view.
*/
  virtual void endViewVectorization();

  virtual void shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);

  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal,
    const OdGeVector3d& direction,
    const OdChar* msg,
    OdInt32 length,
    bool raw,
    const OdGiTextStyle* pTextStyle);

//   virtual void rasterImageDc(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
//     const OdGiRasterImage* pImage, const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts,
//     bool transparency = false, double brightness = 50.0, double contrast = 50.0, double fade = 0.0);

  virtual void setText(const OdGePoint3d& position,
                          const OdGeVector3d& u,      // normal
                          const OdGeVector3d& v,      // direction
                          const OdChar* msg, OdInt32 length, bool raw,
                          const OdGiTextStyle* pTextStyle,
                          bool  bBox,
                          const Od2dFont* pFont,
                          const OdArray<OdUInt16> *pUnicode);

  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool bDcAligned = true,
    bool bAllowClipping = false);

/** \details
    Returns if the geometry can be processed.
	
	\remarks
	This call is  to order a preliminary loop for viewing & processing of current settings without geometry processing.
*/
    bool isProcessGeom() { return true; }

/** \details
    Returns the deviation of the specified type for the specified point.
	
    \remarks
	Returns true if view successfully done or false in the other case.

	\param First argument [in] Deviation type (an instance of the OdGiDeviationType class).	
	\param Second argument [in] Point for which deviation is needed to be returned (an instance of the OdGePoint3d class).	
*/
  //double deviation(const OdGiDeviationType, const OdGePoint3d&) const;
  double calcScaledDeviation(const OdGiDeviationType type, const OdGePoint3d& pt) const;

  friend class PDF2dExportDevice;

};  // end PDF2dExportView 
}
#endif // #ifndef _PDF_2D_EXPORT_VIEW_INCLUDED_
