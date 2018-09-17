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

// PDFDirect module wrapper (used for isolate same typenames).

#ifndef _PDFDIRECTWRAPPER_INCLUDED_
#define _PDFDIRECTWRAPPER_INCLUDED_

#include "RxObject.h"
#include "Gi/GiRasterImage.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeNurbCurve3d.h"

#include "DbEntity.h"
#include "DbUnderlayHost.h"
#include "PdfUnderlayItemPE.h"
#include "StaticRxObject.h"

//#define SOME_TRACE
#ifdef SOME_TRACE
void odPrintConsoleString(const wchar_t* fmt, ...);
#endif

#define STL_USING_MAP
#include "OdaSTL.h"

class OdGePoint2d;
class OdGeMatrix3d;
class OdGiWorldDraw;
class OdGiViewportDraw;
class OdGiGeometry;
struct OdDbUnderlayDrawContext;

/** \details
  Library:  Source code provided.
  <group PdfServices_Classes>
*/
class PdfDirectWrapper : public OdRxObject
{
  static OdStaticRxObject<OdDbPdfUnderlayItemPE> m_PdfItemPE;
protected:
  mutable OdString         m_path;
  OdString                 m_password;
  int                      m_CountOfPages;
  mutable OdGePoint2dArray m_Extents;
  mutable OdArray<bool>    m_Annotations;
  mutable int              m_layerInfoCachePage;

  struct ImageCache
  {
    ImageCache() : m_dpi(0.), m_nPage(-1), m_biWidth(1), m_biHeight(1), m_backgroundColor(0) {};
    ImageCache(const ImageCache& cache) { operator = (cache); }
    ImageCache& operator = (const ImageCache& cache)
    {
      m_pImage = cache.m_pImage;
      m_ext = cache.m_ext;
      m_dpi = cache.m_dpi;

      m_nPage = cache.m_nPage;
      m_biWidth = cache.m_biWidth;
      m_biHeight = cache.m_biHeight;
      m_backgroundColor = cache.m_backgroundColor;

      m_drawCtx = cache.m_drawCtx;
      return *this;
    }
    static bool isUContextEqual(const OdDbUnderlayDrawContext& drawCtx1, const OdDbUnderlayDrawContext& drawCtx2);

    OdGiRasterImagePtr m_pImage;
    // The part of PDF which correspond to m_pImage.
    // It is bigger or equal visible part of PDF.
    OdGeExtents2d m_ext;     // in underlayCS [0-1]
    double m_dpi;

    int m_nPage;
    int m_biWidth;           // Specifies the width of the bitmap, in pixels
    int m_biHeight;
    ODCOLORREF m_backgroundColor;

    OdDbUnderlayDrawContext m_drawCtx;
  };
  class ImageCacheHolder : public OdDbObjectReactor
  {
    typedef std::map<OdDbObjectId, ImageCache > OdImageCacheMap;
    OdArray<OdImageCacheMap> m_cachedPdfInfoArray;
  public:
    ImageCacheHolder() {};
    virtual ~ImageCacheHolder() {};

    OdMutex  m_lock;

    virtual void erased(const OdDbObject* pDbObj, bool pErasing = true);
    virtual void goodbye(const OdDbObject* pObject);
    ImageCache& getPdfInfo(OdUInt32 vpId, const OdDbObjectId& objId);// const;
    void clear();
  };
  mutable OdStaticRxObject<ImageCacheHolder> m_cash;

  struct GeometryCache
  {
    GeometryCache() :m_nPage(-1) {}

    typedef std::pair<OdGePoint3dArray, OdGeExtents2d> Polyline;
    typedef OdArray<Polyline> Polylines;
    Polylines m_polylines;

    Polylines m_splines2polylines;

    typedef std::pair<OdGeNurbCurve3d, OdGeExtents2d> Spline;
    typedef OdArray<Spline> Splines;
    Splines m_splines;

    typedef std::pair<OdGeEllipArc3d, OdGeExtents2d> Ellipse;
    typedef OdArray<Ellipse> Ellipses;
    Ellipses m_ellipses;

    int m_nPage;

    void clear(int nPage = -1)
    {
      m_nPage = nPage;
      m_splines.clear();
      m_polylines.clear();
      m_ellipses.clear();
      m_splines2polylines.clear();
    }

    ~GeometryCache()
    {
      clear();
    }
  };
  mutable GeometryCache m_geometryCache;
  mutable double m_Deviation;//deviation for splines in geometry cache

  OdGiRasterImagePtr convertBmpToRaster(const BITMAPINFOHEADER &bmi, const unsigned char *imagedata) const;
  void method1(double uScale, double vScale, OdGiViewportDraw* pVd, OdGeExtents2d &resUnCS/*in WCS*/, bool &bFlag) const;
  OdGeExtents2d extendVisiblePart(const OdGeExtents2d& visiblePartUnCS) const;

  virtual double getMeasureDictInfo(int nPage) const = 0;
  virtual void createGeometryCache(int nPage, const OdArray<OdUnderlayLayer> &Layers) const = 0;

  OdResult optimizeSplines(OdGiViewportDraw* pVd) const;

public:
  ODRX_DECLARE_MEMBERS(PdfDirectWrapper);
  static OdSmartPtr<PdfDirectWrapper> createObjectImpl();
  static void rxInitWrapper();
  static void rxUninitWrapper();

  virtual OdResult loadFile(OdString path, OdString password) = 0;
  
  virtual OdGiRasterImagePtr getThumbnail(int nPage) const = 0;
  virtual int getItemsCount() const = 0;
  virtual OdString getItemName(int nPage) const = 0;
  virtual void getItemExtents(int nPage, OdGePoint2d& min, OdGePoint2d& max) const = 0;
  virtual OdResult getMediaBox(int nPage, OdGePoint2d& min, OdGePoint2d& max) const = 0;
  virtual const OdGePoint2d &getPageSize(int nPage) const = 0;

  virtual void viewportDraw(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const = 0;
  virtual bool worldDraw(int nPage, OdGiWorldDraw* pVd, const OdDbUnderlayDrawContext& context) const
  {
    return false; // Go to viewportDraw
  };

  virtual int getLevelsCount(int nPage) const = 0;
  virtual OdString getLevelName(int nPage, int nLevel) const = 0;
  virtual OdUInt8 isLevelEnabled(int nPage, int nLevel) const = 0;
  virtual void enableLevel(int nPage, int nLevel, OdUInt8 flag) = 0;

  virtual OdResult getOsnapPoints(int nPage,
                                  const OdGeMatrix3d& modelToWorld,
                                  OdDb::OsnapMode osnapMode,
                                  OdGsMarker gsSelectionMark,
                                  const OdGePoint3d& pickPoint,
                                  const OdGePoint3d& lastPoint,
                                  const OdGeMatrix3d& viewXform,
                                  OdGePoint3dArray& snapPoints) const
  {
    return eNotImplementedYet;
  };

  virtual OdResult getSnapGeometry(OdDbDatabase *pDb, int nPage,
    OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &Layers,
    const OdGePoint3d* pickPtMc) const;
  virtual OdResult drawGeometryForSelection(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const;
  virtual OdResult getExplodeGeometry(int nPage, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &Layers, bool bIgnoreImages) const = 0;
};

typedef OdSmartPtr<PdfDirectWrapper> PdfDirectWrapperPtr;

#endif // _PDFDIRECTWRAPPER_INCLUDED_
