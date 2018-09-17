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
// Temporary experimental clipping

#include "OdaCommon.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "OdVector.h"
#include "OdFileBuf.h"

#include "TrVisRenderClient.h"
#include "TrRndLocalViewer.h"
#include "TrGL2LocalRenditionDefaultClient.h"
#include "TrGL2LocalTransient.h"
#include "TrVisMetafileBuilder.h"
#include "AppClient.h"
#include "ExtBar.h"

// Core/Source
#include "Source/Gi/ExClip/ExClipSpace.h"

class ExClipTransient : public OdTrGL2LocalTransient
{
  public:
    ExClipTransient() {}

    enum TransientType
    {
      kPolygonTransient = 0,
      kMarkerTransient,
      kCurveTransient
    };
    virtual TransientType transientType() const = 0;

    virtual void save(OdStreamBuf *pStream) const = 0;
    virtual void load(OdStreamBuf *pStream) = 0;

    typedef OdSmartPtr<ExClipTransient> Ptr;
};

class ExClipPolygonTransient : public ExClipTransient
{
  public:
    enum ContourType
    {
      kPolygonContour = 0,
      kPolygonHole,
      kClipContour,
      kClipHole,
      kClippedContour,
      kClippedHole
    };
  protected:
    ContourType m_ctrType;
    OdGePoint3dArray m_dataPoints;
    bool m_bClosed;
  public:
    ExClipPolygonTransient()
      : m_ctrType(kPolygonContour)
      , m_bClosed(false)
    {}

    virtual TransientType transientType() const { return kPolygonTransient; }

    virtual void save(OdStreamBuf *pStream) const
    {
      pStream->putBytes(&m_ctrType, sizeof(ContourType));
      const OdUInt32 nPoints = m_dataPoints.size();
      pStream->putBytes(&nPoints, sizeof(OdUInt32));
      pStream->putBytes(m_dataPoints.getPtr(), nPoints * sizeof(OdGePoint3d));
      pStream->putBytes(&m_bClosed, sizeof(bool));
    }
    virtual void load(OdStreamBuf *pStream)
    {
      pStream->getBytes(&m_ctrType, sizeof(ContourType));
      OdUInt32 nPoints = 0;
      pStream->getBytes(&nPoints, sizeof(OdUInt32));
      m_dataPoints.resize(nPoints);
      pStream->getBytes(m_dataPoints.asArrayPtr(), nPoints * sizeof(OdGePoint3d));
      pStream->getBytes(&m_bClosed, sizeof(bool));
    }

    void setContourType(ContourType type) { m_ctrType = type; }
    ContourType contourType() const { return m_ctrType; }

    void close(bool bRemoveLast = false)
    {
      if (bRemoveLast)
        m_dataPoints.removeLast();
      m_bClosed = true;
      invalidate();
    }
    void open()
    {
      m_bClosed = false;
      invalidate();
    }

    OdUInt32 numberOfPoints() const { return m_dataPoints.size(); }
    const OdGePoint3d &pointAt(OdUInt32 nPt) const { return m_dataPoints.getPtr()[nPt]; }
    const OdGePoint3d *points() const { return m_dataPoints.getPtr(); }
    OdGePoint3dArray pointsArray() const
    {
      OdGePoint3dArray arry;
      arry.insert(arry.end(), points(), points() + numberOfPoints());
      if (m_bClosed)
        arry.push_back(pointAt(0));
      return arry;
    }

    ODCOLORREF color() const
    {
      switch (m_ctrType)
      {
        case kPolygonContour:
          return ODRGBA(0, 0, 255, 255);
        case kPolygonHole:
          return ODRGBA(0, 0, 128, 255);
        case kClipContour:
          return ODRGBA(255, 0, 0, 255);
        case kClipHole:
          return ODRGBA(128, 0, 0, 255);
        case kClippedContour:
          return ODRGBA(0, 255, 0, 128);
        case kClippedHole:
          return ODRGBA(0, 128, 0, 128);
      }
      return ODRGBA(255, 255, 255, 255);
    }

    virtual void draw(OdTrVisMetafileBuilderPtr mfBuilder, const OdTrGL2LocalRenderer* pRenderer)
    {
      OdTrVisGeometryDraw *geom = mfBuilder->wcsGeometry();
      OdTrVisColorRGBA cl;
      cl.setCRef( color() );
      mfBuilder->setColor(cl);
      geom->polyline(m_dataPoints.size(), m_dataPoints.getPtr(), OdTrVisGeometryDraw::kLineStrip);
      if (m_bClosed)
      {
        const OdGePoint3d clPt[2] = { m_dataPoints.first(), m_dataPoints.last() };
        geom->polyline(2, clPt, OdTrVisGeometryDraw::kLineStrip);
      }
    }

    void addPoint(const OdGePoint3d &pt = OdGePoint3d::kOrigin) { m_dataPoints.push_back(pt); }
    void resetPoint(const OdGePoint3d &pt)
    {
      m_dataPoints.last() = pt;
      invalidate();
    }

    static ExClipPolygonTransient *cast(ExClipTransient *pTrans) { return static_cast<ExClipPolygonTransient*>(pTrans); }
    static const ExClipPolygonTransient *cast(const ExClipTransient *pTrans) { return static_cast<const ExClipPolygonTransient*>(pTrans); }
    static ExClipTransient::Ptr create(ContourType type, const OdGePoint3d &pt = OdGePoint3d::kOrigin)
    {
      OdSmartPtr<ExClipPolygonTransient> pPoly = OdRxObjectImpl<ExClipPolygonTransient>::createObject();
      pPoly->setContourType( type );
      pPoly->addPoint(pt); pPoly->addPoint(pt);
      return pPoly;
    }
};

class ExClipCurveTransient : public ExClipPolygonTransient
{
  protected:
    OdGeCurve3dPtr m_pCurve;
  public:
    ExClipCurveTransient()
    {}

    virtual TransientType transientType() const { return kCurveTransient; }

    virtual void save(OdStreamBuf *pStream) const
    {
      ExClipPolygonTransient::save(pStream);
      const OdGe::EntityId type = m_pCurve->type();
      pStream->putBytes(&type, sizeof(OdGe::EntityId));
      switch (type)
      {
        case OdGe::kCircArc3d:
          {
            const OdGeCircArc3d *pArc = static_cast<const OdGeCircArc3d*>(m_pCurve.get());
            { OdGePoint3d center = pArc->center(); pStream->putBytes(&center, sizeof(OdGePoint3d)); }
            { OdGeVector3d normal = pArc->normal(); pStream->putBytes(&normal, sizeof(OdGeVector3d)); }
            { double radius = pArc->radius(); pStream->putBytes(&radius, sizeof(double)); }
            { OdGeVector3d refVec = pArc->refVec(); pStream->putBytes(&refVec, sizeof(OdGeVector3d)); }
            { double startAngl = pArc->startAng(); pStream->putBytes(&startAngl, sizeof(double)); }
            { double endAngl = pArc->endAng(); pStream->putBytes(&endAngl, sizeof(double)); }
          }
        break;
        case OdGe::kEllipArc3d:
          {
            const OdGeEllipArc3d *pArc = static_cast<const OdGeEllipArc3d*>(m_pCurve.get());
            { OdGePoint3d center = pArc->center(); pStream->putBytes(&center, sizeof(OdGePoint3d)); }
            { OdGeVector3d majorAxs = pArc->majorAxis(); pStream->putBytes(&majorAxs, sizeof(OdGeVector3d)); }
            { double majorRdius = pArc->majorRadius(); pStream->putBytes(&majorRdius, sizeof(double)); }
            { OdGeVector3d minorAxs = pArc->minorAxis(); pStream->putBytes(&minorAxs, sizeof(OdGeVector3d)); }
            { double minorRdius = pArc->minorRadius(); pStream->putBytes(&minorRdius, sizeof(double)); }
            { double startAngl = pArc->startAng(); pStream->putBytes(&startAngl, sizeof(double)); }
            { double endAngl = pArc->endAng(); pStream->putBytes(&endAngl, sizeof(double)); }
          }
        break;
      }
    }
    virtual void load(OdStreamBuf *pStream)
    {
      ExClipPolygonTransient::load(pStream);
      OdGe::EntityId type = m_pCurve->type();
      pStream->getBytes(&type, sizeof(OdGe::EntityId));
      switch (type)
      {
        case OdGe::kCircArc3d:
          {
            OdGePoint3d center; OdGeVector3d normal; double radius; OdGeVector3d refVec; double startAngl, endAngl;
            pStream->getBytes(&center, sizeof(OdGePoint3d));
            pStream->getBytes(&normal, sizeof(OdGeVector3d));
            pStream->getBytes(&radius, sizeof(double));
            pStream->getBytes(&refVec, sizeof(OdGeVector3d));
            pStream->getBytes(&startAngl, sizeof(double));
            pStream->getBytes(&endAngl, sizeof(double));
            m_pCurve = new OdGeCircArc3d(center, normal, refVec, radius, startAngl, endAngl);
          }
        break;
        case OdGe::kEllipArc3d:
          {
            OdGePoint3d center; OdGeVector3d majorAxs; double majorRdius; OdGeVector3d minorAxs; double minorRdius; double startAngl, endAngl;
            pStream->getBytes(&center, sizeof(OdGePoint3d));
            pStream->getBytes(&majorAxs, sizeof(OdGeVector3d));
            pStream->getBytes(&majorRdius, sizeof(double));
            pStream->getBytes(&minorAxs, sizeof(OdGeVector3d));
            pStream->getBytes(&minorRdius, sizeof(double));
            pStream->getBytes(&startAngl, sizeof(double));
            pStream->getBytes(&endAngl, sizeof(double));
            m_pCurve = new OdGeEllipArc3d(center, majorAxs, minorAxs, majorRdius, minorRdius, startAngl, endAngl);;
          }
        break;
      }
    }

    virtual void draw(OdTrVisMetafileBuilderPtr mfBuilder, const OdTrGL2LocalRenderer* pRenderer)
    {
      OdGeInterval ival;
      m_pCurve->getInterval(ival);
      m_pCurve->getSamplePoints(ival.lowerBound(), ival.upperBound(), 0.0, m_dataPoints);
      OdTrVisGeometryDraw* geom = mfBuilder->wcsGeometry();
      OdTrVisColorRGBA cl;
      cl.setCRef( color() );
      mfBuilder->setColor(cl);
      geom->polyline(m_dataPoints.size(), m_dataPoints.getPtr(), OdTrVisGeometryDraw::kLineStrip);
    }

    void setCurve(OdGeCurve3d *pCurve) { m_pCurve = pCurve; }
    const OdGeCurve3d *curve() const { return m_pCurve; }

    static ExClipCurveTransient *cast(ExClipTransient *pTrans) { return static_cast<ExClipCurveTransient*>(pTrans); }
    static const ExClipCurveTransient *cast(const ExClipTransient *pTrans) { return static_cast<const ExClipCurveTransient*>(pTrans); }
    static ExClipTransient::Ptr create(ContourType type, OdGeCurve3d *pCurve)
    {
      OdSmartPtr<ExClipCurveTransient> pCurv = OdRxObjectImpl<ExClipCurveTransient>::createObject();
      pCurv->setContourType(type);
      pCurv->setCurve(pCurve);
      return pCurv;
    }
};

class ExClipMarkerTransient : public ExClipTransient
{
  public:
    enum MarkType
    {
      kStartVertex = 0,
      kStopVertex,
      kFromInfinityVertex,
      kToInfinityVertex
    };
  protected:
    enum MarkShape
    {
      kTriangleShape,
      kRectShape,
      kCircleShape
    };
  protected:
    MarkType m_markType;
    OdGePoint3d m_origin;
  public:
    ExClipMarkerTransient()
      : m_markType(kStartVertex)
    {
      setAutoRegen(true);
    }

    virtual TransientType transientType() const { return kMarkerTransient; }

    virtual void save(OdStreamBuf *pStream) const
    {
      pStream->putBytes(&m_markType, sizeof(MarkType));
      pStream->putBytes(&m_origin, sizeof(OdGePoint3d));
    }
    virtual void load(OdStreamBuf *pStream)
    {
      pStream->getBytes(&m_markType, sizeof(MarkType));
      pStream->getBytes(&m_origin, sizeof(OdGePoint3d));
    }

    void setMarkType(MarkType type) { m_markType = type; }
    MarkType markType() const { return m_markType; }

    void setOrigin(const OdGePoint3d &origin) { m_origin = origin; }
    const OdGePoint3d &origin() const { return m_origin; }

    double radius() const { return 4.5; }

    ODCOLORREF color() const
    {
      switch (m_markType)
      {
        case kStartVertex:
          return ODRGBA(255, 255, 0, 255);
        case kStopVertex:
          return ODRGBA(128, 128, 0, 255);
        case kFromInfinityVertex:
          return ODRGBA(255, 0, 255, 255);
        case kToInfinityVertex:
          return ODRGBA(128, 0, 128, 255);
      }
      return ODRGBA(255, 255, 255, 255);
    }
    MarkShape markShape() const
    {
      switch (m_markType)
      {
        case kStartVertex: case kStopVertex:
          return kCircleShape;
        case kFromInfinityVertex: case kToInfinityVertex:
          return kTriangleShape;
      }
      return kRectShape;
    }

    virtual void draw(OdTrVisMetafileBuilderPtr mfBuilder, const OdTrGL2LocalRenderer* pRenderer)
    {
      OdTrVisGeometryDraw* geom = mfBuilder->wcsGeometry();
      OdTrVisColorRGBA cl;
      cl.setCRef( color() );
      mfBuilder->setColor(cl);

      OdGeVector3d unitVec = OdGeVector3d::kYAxis;
      unitVec.transformBy(pRenderer->coordinateSpaceTransform( mfBuilder->viewportId(), mfBuilder->overlayId(), OdTrGL2LocalRenderer::kScreenSpace, OdTrGL2LocalRenderer::kEyeSpace));
      double unitsPerPixel = unitVec.length();

      const double dRad = radius() * unitsPerPixel;
      switch (markShape())
      {
        case kTriangleShape:
        {
          const OdGePoint3d pts[4] =
          {
            m_origin + OdGeVector3d::kYAxis * dRad,
            m_origin - OdGeVector3d::kYAxis * dRad - OdGeVector3d::kXAxis * dRad,
            m_origin - OdGeVector3d::kYAxis * dRad + OdGeVector3d::kXAxis * dRad,
            m_origin + OdGeVector3d::kYAxis * dRad
          };
          geom->polyline(4, pts, OdTrVisGeometryDraw::kLineStrip );
        }
        break;
        case kRectShape:
        {
          const OdGePoint3d pts[5] =
          {
            m_origin - OdGeVector3d::kYAxis * dRad - OdGeVector3d::kXAxis * dRad,
            m_origin + OdGeVector3d::kYAxis * dRad - OdGeVector3d::kXAxis * dRad,
            m_origin + OdGeVector3d::kYAxis * dRad + OdGeVector3d::kXAxis * dRad,
            m_origin - OdGeVector3d::kYAxis * dRad + OdGeVector3d::kXAxis * dRad,
            m_origin - OdGeVector3d::kYAxis * dRad - OdGeVector3d::kXAxis * dRad
          };
          geom->polyline(5, pts, OdTrVisGeometryDraw::kLineStrip);
        }
        break;
        case kCircleShape:
        {
          int numSegs = (int)(radius() * 2);
          if (numSegs < 3) numSegs = 3; else if (numSegs > 100) numSegs = 100;
          geom->arc(m_origin, dRad, OdGeVector3d::kZAxis, (OdUInt32)numSegs);
        }
        break;
      }
    }

    static ExClipMarkerTransient *cast(ExClipTransient *pTrans) { return static_cast<ExClipMarkerTransient*>(pTrans); }
    static const ExClipMarkerTransient *cast(const ExClipTransient *pTrans) { return static_cast<const ExClipMarkerTransient*>(pTrans); }
    static ExClipTransient::Ptr create(MarkType type, const OdGePoint3d &pt = OdGePoint3d::kOrigin)
    {
      OdSmartPtr<ExClipMarkerTransient> pMark = OdRxObjectImpl<ExClipMarkerTransient>::createObject();
      pMark->setMarkType(type);
      pMark->setOrigin(pt);
      return pMark;
    }
};

static const OdUInt32 ExCmdElem_DrawingMode    = 100;
static const OdUInt32 ExCmdElem_PolygonContour = 101;
static const OdUInt32 ExCmdElem_PolygonHole    = 102;
static const OdUInt32 ExCmdElem_ClipContour    = 103;
static const OdUInt32 ExCmdElem_ClipHole       = 104;
static const OdUInt32 ExCmdElem_3dViewDesc     = 105;
static const OdUInt32 ExCmdElem_3dViewTop      = 106;
static const OdUInt32 ExCmdElem_3dViewFront    = 107;
static const OdUInt32 ExCmdElem_3dViewLeft     = 108;
static const OdUInt32 ExCmdElem_3dViewBack     = 109;
static const OdUInt32 ExCmdElem_3dViewRight    = 110;
static const OdUInt32 ExCmdElem_3dViewBottom   = 111;
static const OdUInt32 ExCmdElem_CreateCircles  = 112;
static const OdUInt32 ExCmdElem_CreateEllipses = 113;
static const OdUInt32 ExCmdElem_ClipAsPlanes   = 114;
static const OdUInt32 ExCmdElem_LinesClip      = 115;
static const OdUInt32 ExCmdElem_RaysClip       = 116;
static const OdUInt32 ExCmdElem_SegmentsClip   = 117;
static const OdUInt32 ExCmdElem_CurveClip      = 118;
static const OdUInt32 ExCmdElem_PolygonClip    = 119;
static const OdUInt32 ExCmdElem_ClearPolygon   = 120;
static const OdUInt32 ExCmdElem_ClearClip      = 121;
static const OdUInt32 ExCmdElem_ClearResult    = 122;
static const OdUInt32 ExCmdElem_Save           = 123;
static const OdUInt32 ExCmdElem_ClearAll       = 124;

static ExClipPolygonTransient::ContourType exCmdElemToContourType(OdUInt32 cmdElem)
{
  switch (cmdElem)
  {
    case ExCmdElem_PolygonContour: return ExClipPolygonTransient::kPolygonContour;
    case ExCmdElem_PolygonHole:    return ExClipPolygonTransient::kPolygonHole;
    case ExCmdElem_ClipContour:    return ExClipPolygonTransient::kClipContour;
    case ExCmdElem_ClipHole:       return ExClipPolygonTransient::kClipHole;
  }
  return ExClipPolygonTransient::kPolygonContour;
}
static OdUInt32 contourTypeToExCmdElem(ExClipPolygonTransient::ContourType type)
{
  switch (type)
  {
    case ExClipPolygonTransient::kPolygonContour: return ExCmdElem_PolygonContour;
    case ExClipPolygonTransient::kPolygonHole:    return ExCmdElem_PolygonHole;
    case ExClipPolygonTransient::kClipContour:    return ExCmdElem_ClipContour;
    case ExClipPolygonTransient::kClipHole:       return ExCmdElem_ClipHole;
  }
  return ExCmdElem_PolygonContour;
}

static struct ExClipRenderClient : public AppClient
{
  AppHandle                           m_appHandle;
  OdTrGL2LocalViewerPtr               ViewRenderer;
  OdVector<ExClipTransient::Ptr>      Transients;
  ExClipTransient::Ptr                EditTransient;
  bool                                m_bPanning;
  bool                                m_bRotating;
  ExClipPolygonTransient::ContourType m_ContourType;

  AppHandle appHandle() const { return m_appHandle; }

  OdTrGL2LocalViewerPtr createRenderer() { return OdTrGL2LocalViewerPtr().create(OdTrGL2LocalRenditionDefaultClient::createObject()); }
  OdTrGL2LocalViewer *viewer() { return ViewRenderer.get(); }
  OdTrGL2LocalRenderer *renderer() { return viewer()->renderer(); }
  OdTrVisRendition *rendition() { return renderer()->rendition(); }

  struct RenderClientImpl : public OdTrVisRenderClientPropImpl
  {
    ExClipRenderClient *m_pAppClient;
    OdAnsiString m_lastWarn;

    RenderClientImpl() : m_pAppClient(NULL)
    {
      setProperty(OD_T("DoubleBufferEnabled"), OdRxVariantValue(true));
      setProperty(OD_T("UseSceneGraph"), OdRxVariantValue(true));
    }

    virtual bool usePartialUpdate() const
    { // This property isn't currently configurable through properties dictionary.
      return true;
    }

    // Output window size
    virtual int outputWindowWidth() const
    {
      OdGsDCRect clientRect;
      m_pAppClient->getSize(m_pAppClient->appHandle(), clientRect);
      return clientRect.m_max.x - clientRect.m_min.x;
    }
    virtual int outputWindowHeight() const
    {
      OdGsDCRect clientRect;
      m_pAppClient->getSize(m_pAppClient->appHandle(), clientRect);
      return clientRect.m_min.y - clientRect.m_max.y;
    }

    // Rendering process handlers
    virtual void emitError(const char *pError)
    {
      m_pAppClient->displayError(m_pAppClient->appHandle(), pError);
      throw pError;
    }

    virtual void emitWarning(const char *pWarn)
    {
      if (pWarn != m_lastWarn)
      {
        m_pAppClient->displayWarning(m_pAppClient->appHandle(), pWarn);
        m_lastWarn = pWarn;
      }
    }
  } ViewRenderClient;

  void setAppHandle(AppHandle appHandle)
  {
    m_appHandle = appHandle;
    ViewRenderClient.setProperty(OD_T("WindowHWND"), OdRxVariantValue((OdIntPtr)appHandle));
  }

  ExClipRenderClient()
    : m_appHandle(NULL)
    , m_bPanning(false)
    , m_ContourType(ExClipPolygonTransient::kPolygonContour)
  {
    AppClient::registerAppClient(this);
    ViewRenderClient.m_pAppClient = this;
  }
  ~ExClipRenderClient()
  {
    AppClient::unregisterAppClient(this);
  }

  virtual const char *appClientName() const { return "ExClip"; }
  virtual bool isUtilitaryAppClient() const { return true; }

  void init()
  {
    clearAllTransients();
    ViewRenderer = createRenderer();
    // Initialize viewport
    OdTrVisViewportDef vpDef;
    vpDef.setDefault(); vpDef.m_bVisible = true;
    rendition()->onViewportAdded(100);
    rendition()->onViewportModified(100, vpDef);
    // Resize rendering area
    OdGsDCRect rArea;
    getSize(appHandle(), rArea);
    viewer()->resize(rArea);
  }

  void clearTransients(ExClipTransient::TransientType type, ExClipPolygonTransient::ContourType subType = (ExClipPolygonTransient::ContourType)(-1))
  {
    const bool bTypeAll    =    type == (ExClipTransient::TransientType)(-1);
    const bool bSubTypeAll = subType == (ExClipPolygonTransient::ContourType)(-1);
    const bool bType2      =    type == ExClipTransient::kPolygonTransient;
    OdUInt32 nTransient = 0;
    while (nTransient < Transients.size())
    {
      if (bTypeAll || ((Transients.getPtr()[nTransient]->transientType() == type) || (bType2 && (Transients.getPtr()[nTransient]->transientType() == ExClipTransient::kCurveTransient))))
      {
        if (bSubTypeAll || (ExClipPolygonTransient::cast(Transients[nTransient])->contourType() == subType))
        {
          renderer()->transientManager()->detachTransient(Transients[nTransient]);
          Transients.removeAt(nTransient), --nTransient;
        }
      }
      nTransient++;
    }
  }

  void clearEditTransient()
  {
    if (!EditTransient.isNull())
    {
      renderer()->transientManager()->detachTransient(EditTransient);
      EditTransient.release();
    }
  }

  void clearAllTransients()
  {
    clearTransients((ExClipTransient::TransientType)(-1));
    clearEditTransient();
  }

  virtual void onCreate(AppHandle appHandle)
  {
    setAppHandle(appHandle);
    init();
    // Fill Extensions Bar
    ExtBar &extBar = getExtBar(appHandle);
    ExtBar::GroupElement *pGroup = extBar.createGroup(ExCmdElem_DrawingMode - 1);
    ExtBar::TextElement::cast(pGroup)->setText(L"Drawing Mode");
    ExtBar::RadioElement *pRGroup = extBar.createRadioGroup(ExCmdElem_DrawingMode, pGroup);
    extBar.createRadioButton(ExCmdElem_PolygonContour, pRGroup)->setText(L"Polygon Contour");
    extBar.createRadioButton(ExCmdElem_PolygonHole, pRGroup)->setText(L"Polygon Hole");
    extBar.createRadioButton(ExCmdElem_ClipContour, pRGroup)->setText(L"Clip Contour");
    extBar.createRadioButton(ExCmdElem_ClipHole, pRGroup)->setText(L"Clip Hole");
    pRGroup->setCheckedElement(::contourTypeToExCmdElem(m_ContourType));
    extBar.createText(ExCmdElem_3dViewDesc, NULL, ExtBar::k2d8)->setText(L"3dView:");
    extBar.createButton(ExCmdElem_3dViewTop, NULL, ExtBar::k1d8)->setText(L"Tp");
    extBar.createButton(ExCmdElem_3dViewFront, NULL, ExtBar::k1d8)->setText(L"Fr");
    extBar.createButton(ExCmdElem_3dViewLeft, NULL, ExtBar::k1d8)->setText(L"Lt");
    extBar.createButton(ExCmdElem_3dViewBack, NULL, ExtBar::k1d8)->setText(L"Bk");
    extBar.createButton(ExCmdElem_3dViewRight, NULL, ExtBar::k1d8)->setText(L"Rt");
    extBar.createButton(ExCmdElem_3dViewBottom, NULL, ExtBar::k1d8)->setText(L"Bt");
    extBar.createButton(ExCmdElem_CreateCircles)->setText(L"Create Circles");
    extBar.createButton(ExCmdElem_CreateEllipses)->setText(L"Create Ellipses");
    extBar.createCheckBox(ExCmdElem_ClipAsPlanes)->setText(L"Clip as Planes");
    extBar.createButton(ExCmdElem_LinesClip)->setText(L"Clip Lines");
    extBar.createButton(ExCmdElem_RaysClip)->setText(L"Clip Rays");
    extBar.createButton(ExCmdElem_SegmentsClip)->setText(L"Clip Line Segments");
    extBar.createButton(ExCmdElem_CurveClip)->setText(L"Clip Curves");
    extBar.createButton(ExCmdElem_PolygonClip)->setText(L"Clip Polygons");
    extBar.createPlaceholder();
    extBar.createButton(ExCmdElem_ClearPolygon)->setText(L"Clear Polygon");
    extBar.createButton(ExCmdElem_ClearClip)->setText(L"Clear Clipping");
    extBar.createButton(ExCmdElem_ClearResult)->setText(L"Clear Result");
    extBar.createPlaceholder();
    extBar.createButton(ExCmdElem_Save)->setText(L"Save File");
    extBar.createButton(ExCmdElem_ClearAll)->setText(L"Clear All");
    // Complete Extensions Bar
    updateExtBar(appHandle);
  }
  virtual void onDestroy(AppHandle /*appHandle*/)
  {
    clearAllTransients();
    ViewRenderer.release();
  }

  virtual const OdChar *supportFormats(AppHandle /*appHandle*/, OdString &defExt)
  {
    defExt = L"ect";
    return L"ECT Files (*.ect)\0*.ect\0"
           L"All Files (*.*)\0*.*\0\0";
  }
  virtual void onSave()
  {
    OdString defExt, fileName = getExtBar(appHandle()).getSaveFileName(supportFormats(appHandle(), defExt), defExt);
    if (!fileName.isEmpty())
    {
      OdWrFileBuf wrFile(fileName);
      const char hdr[4] = { 'e', 'c', 't', '0' };
      wrFile.putBytes(hdr, sizeof(hdr));
      const OdUInt32 nObjs = Transients.size();
      wrFile.putBytes(&nObjs, sizeof(OdUInt32));
      for (OdUInt32 nObj = 0; nObj < nObjs; nObj++)
      {
        const ExClipTransient::TransientType type = Transients[nObj]->transientType();
        wrFile.putBytes(&type, sizeof(ExClipTransient::TransientType));
        Transients[nObj]->save(&wrFile);
      }
    }
  }
  virtual void onOpen(AppHandle appHandle, const OdString &fileName)
  {
    init();
    OdRdFileBuf rdFile(fileName);
    char hdr[4] = { 0 };
    rdFile.getBytes(hdr, sizeof(hdr));
    if ((hdr[0] != 'e') || (hdr[1] != 'c') || (hdr[2] != 't') || (hdr[3] != '0'))
      return;
    OdUInt32 nObjs = 0;
    rdFile.getBytes(&nObjs, sizeof(OdUInt32));
    for (OdUInt32 nObj = 0; nObj < nObjs; nObj++)
    {
      ExClipTransient::TransientType type = (ExClipTransient::TransientType)0;
      rdFile.getBytes(&type, sizeof(ExClipTransient::TransientType));
      ExClipTransient::Ptr pTransient;
      switch (type)
      {
        case ExClipTransient::kPolygonTransient: pTransient = OdRxObjectImpl<ExClipPolygonTransient>::createObject(); break;
        case ExClipTransient::kMarkerTransient:  pTransient = OdRxObjectImpl<ExClipMarkerTransient>::createObject(); break;
        case ExClipTransient::kCurveTransient:   pTransient = OdRxObjectImpl<ExClipCurveTransient>::createObject(); break;
      }
      pTransient->load(&rdFile);
      renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), pTransient.get());
      Transients.push_back(pTransient);
    }
  }

  virtual void onDraw(AppHandle /*appHandle*/)
  {
    try
    {
      viewer()->update(&ViewRenderClient);
    }
    catch (const char*)
    {
      close(appHandle());
    }
  }
  virtual void onSize(AppHandle /*appHandle*/, const OdGsDCRect &size)
  {
    viewer()->resize(size);
  }

  virtual void onLMouseButtonDown(AppHandle /*appHandle*/, int /*x*/, int /*y*/, bool /*bDoubleClick*/)
  {
  }
  virtual void onLMouseButtonUp(AppHandle /*appHandle*/, int x, int y)
  {
    OdGePoint3d pt((double)x, (double)y, 0.0);
    pt.transformBy(renderer()->coordinateSpaceTransform(viewer()->activeViewportId(), kTrVisMainOverlayId, OdTrGL2LocalRenderer::kScreenSpace, OdTrGL2LocalRenderer::kWorldSpace));
    if (EditTransient.isNull())
    {
      EditTransient = ExClipPolygonTransient::create(m_ContourType, pt);
      renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), EditTransient.get());
    }
    else
      ExClipPolygonTransient::cast(EditTransient)->addPoint(pt);
  }

  virtual void onRMouseButtonDown(AppHandle /*appHandle*/, int /*x*/, int /*y*/, bool /*bDoubleClick*/)
  {
  }
  virtual void onRMouseButtonUp(AppHandle appHandle, int x, int y)
  {
    if (!EditTransient.isNull())
    {
      if (ExClipPolygonTransient::cast(EditTransient)->numberOfPoints() > 3)
      {
        ExClipPolygonTransient::cast(EditTransient)->close(true);
        Transients.push_back(EditTransient);
        EditTransient.release();
      }
      else
        clearEditTransient();
      redraw(appHandle);
    }
  }

  virtual void onMMouseButtonDown(AppHandle /*appHandle*/, int /*x*/, int /*y*/, bool bDoubleClick)
  {
    if (!bDoubleClick)
    {
      capture(appHandle());
      m_bPanning = true;
    }
    else
    {
      capture(appHandle());
      m_bPanning = false;
      m_bRotating = true;
    }
  }
  virtual void onMMouseButtonUp(AppHandle /*appHandle*/, int /*x*/, int /*y*/)
  {
    if (m_bPanning)
    {
      releaseCapture(appHandle());
      m_bPanning = false;
    }
    else if (m_bRotating)
    {
      releaseCapture(appHandle());
      m_bPanning = false;
    }
  }

  virtual void onMouseMove(AppHandle /*appHandle*/, int dx, int dy)
  {
    if (m_bPanning)
    {
      viewer()->pan(dx, dy);
      redraw(appHandle());
    }
    else if (m_bRotating)
    {
      bool bShiftPressed = GETBIT(getKeyStates(appHandle()), kShiftKey);
      if (!bShiftPressed)
        viewer()->rotate(dy * -0.025, dx * -0.025, 0.0);
      else
        viewer()->rotate(0.0, 0.0, (dx + dy) * -0.025);
      redraw(appHandle());
    }
  }
  virtual void onNcdMouseMove(AppHandle appHandle, int dx, int dy)
  {
    if (!EditTransient.isNull())
    {
      int x, y; getLastPoint(appHandle, x, y);
      OdGePoint3d pt((double)(x + dx), (double)(y + dy), 0.0);
      pt.transformBy(renderer()->coordinateSpaceTransform(viewer()->activeViewportId(), kTrVisMainOverlayId, OdTrGL2LocalRenderer::kScreenSpace, OdTrGL2LocalRenderer::kWorldSpace));
      ExClipPolygonTransient::cast(EditTransient)->resetPoint(pt);
      redraw(appHandle);
    }
  }

  virtual void onMouseWheel(AppHandle /*appHandle*/, float delta, int x, int y)
  {
    if (!isCaptured(appHandle()))
    {
      viewer()->zoom(x, y, (delta <= 0.0) ? (1.0f / 0.9f) : 0.9f);
      redraw(appHandle());
    }
  }

  virtual void onExtBar(AppHandle appHandle, ExtBar::Element *pElem)
  {
    switch (pElem->elementId())
    {
      case ExCmdElem_DrawingMode:
        m_ContourType = ::exCmdElemToContourType(ExtBar::RadioElement::cast(pElem)->checkedElement());
      break;
      case ExCmdElem_3dViewTop: case ExCmdElem_3dViewFront: case ExCmdElem_3dViewLeft:
      case ExCmdElem_3dViewBack: case ExCmdElem_3dViewRight: case ExCmdElem_3dViewBottom:
        {
          OdTrVisViewportDef *pVp = renderer()->getViewportDef(viewer()->activeViewportId());
          OdTrVisViewParamsDef &viewParams = pVp->m_viewParams;
          switch (pElem->elementId())
          {
            case ExCmdElem_3dViewTop:    viewParams.m_viewBasis[2] =  OdGeVector3d::kZAxis; viewParams.m_viewBasis[1] =  OdGeVector3d::kYAxis; break;
            case ExCmdElem_3dViewFront:  viewParams.m_viewBasis[2] =  OdGeVector3d::kYAxis; viewParams.m_viewBasis[1] =  OdGeVector3d::kZAxis; break;
            case ExCmdElem_3dViewLeft:   viewParams.m_viewBasis[2] =  OdGeVector3d::kXAxis; viewParams.m_viewBasis[1] =  OdGeVector3d::kZAxis; break;
            case ExCmdElem_3dViewBack:   viewParams.m_viewBasis[2] = -OdGeVector3d::kYAxis; viewParams.m_viewBasis[1] = -OdGeVector3d::kZAxis; break;
            case ExCmdElem_3dViewRight:  viewParams.m_viewBasis[2] = -OdGeVector3d::kXAxis; viewParams.m_viewBasis[1] = -OdGeVector3d::kZAxis; break;
            case ExCmdElem_3dViewBottom: viewParams.m_viewBasis[2] = -OdGeVector3d::kZAxis; viewParams.m_viewBasis[1] = -OdGeVector3d::kYAxis; break;
          }
          viewParams.m_viewPosition = OdGePoint3d::kOrigin;
          viewParams.m_viewTarget = viewParams.m_viewPosition + viewParams.m_viewBasis[2];
          viewer()->markViewportModified(viewer()->activeViewportId(), OdTrVisViewportDef::kViewportModOrientation);
        }
      break;
      case ExCmdElem_CreateCircles:
      case ExCmdElem_CreateEllipses:
        {
          for (OdUInt32 nTrans = 0; nTrans < Transients.size(); nTrans++)
          {
            const ExClipTransient *pTrans = Transients.getPtr()[nTrans];
            if (pTrans->transientType() == ExClipTransient::kPolygonTransient)
            {
              const ExClipPolygonTransient *pPoly = ExClipPolygonTransient::cast(pTrans);
              if ((pPoly->contourType() == ExClipPolygonTransient::kPolygonContour) ||
                  (pPoly->contourType() == ExClipPolygonTransient::kPolygonHole))
              {
                OdGeCurve3d *pCurve = NULL;
                const bool bMakeEllipse = pElem->elementId() == ExCmdElem_CreateEllipses;
                const bool bMakeArc = pPoly->contourType() == ExClipPolygonTransient::kPolygonHole;
                if (!bMakeEllipse)
                {
                  if (!bMakeArc)
                    pCurve = new OdGeCircArc3d(pPoly->pointAt(1), OdGeVector3d::kZAxis, pPoly->pointAt(0).distanceTo(pPoly->pointAt(1)));
                  else
                    pCurve = new OdGeCircArc3d(pPoly->pointAt(0), pPoly->pointAt(1), pPoly->pointAt(2));
                }
                else
                {
                  OdGeVector3d major = pPoly->pointAt(1) - pPoly->pointAt(0);
                  OdGeVector3d cross = OdGeVector3d::kZAxis.crossProduct(major);
                  if (!bMakeArc)
                    pCurve = new OdGeEllipArc3d(pPoly->pointAt(0), major.normal(), cross.normal(), major.length(), major.length() * 0.5);
                  else
                    pCurve = new OdGeEllipArc3d(pPoly->pointAt(0), major.normal(), cross.normal(), major.length(), major.length() * 0.5, OdaPI * 0.25, Oda2PI * 0.76);
                }
                ExClipTransient::Ptr curveTrans = ExClipCurveTransient::create(pPoly->contourType(), pCurve);
                renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), curveTrans.get());
                Transients.push_back(curveTrans);
              }
            }
          }
        }
      break;
      case ExCmdElem_LinesClip:
      case ExCmdElem_RaysClip:
      case ExCmdElem_SegmentsClip:
      case ExCmdElem_CurveClip:
      case ExCmdElem_PolygonClip:
        {
          ExClip::ClipSpace clipSpace;
          // Prepare clipping shape
          ExClip::ClipPoly *pClipPoly = NULL;
          OdVector<ExClip::ClipPlane*, OdMemoryAllocator<ExClip::ClipPlane*> > planes;
          OdUInt32 nTrans;
          for (nTrans = 0; nTrans < Transients.size(); nTrans++)
          {
            const ExClipTransient *pTrans = Transients.getPtr()[nTrans];
            if (pTrans->transientType() == ExClipTransient::kPolygonTransient)
            {
              const ExClipPolygonTransient *pPoly = ExClipPolygonTransient::cast(pTrans);
              if ((pPoly->contourType() == ExClipPolygonTransient::kClipContour) ||
                  (pPoly->contourType() == ExClipPolygonTransient::kClipHole))
              {
                ExClip::Polygon3d poly(pPoly->numberOfPoints(), pPoly->points());
                if (!ExtBar::CheckElement::cast(getExtBar(appHandle).getElement(ExCmdElem_ClipAsPlanes))->isChecked())
                {
                  if (!pClipPoly)
                  {
                    pClipPoly = clipSpace.createClipPoly();
                    clipSpace.pushShape(pClipPoly);
                  }
                  clipSpace.appendPolygon(pClipPoly, poly, (pPoly->contourType() == ExClipPolygonTransient::kClipContour) ? ExClip::ClipPoly::kClipContour : ExClip::ClipPoly::kClipHole);
                }
                else
                {
                  OdUInt32 nVert1 = 0, nVert2 = 1;
                  while (nVert1 < poly.numPoints())
                  {
                    OdGeVector3d dir = (poly.pointAt(nVert2) - poly.pointAt(nVert1)).normal();
                    OdGeVector3d perp = poly.normal().crossProduct(dir);
                    if (pPoly->contourType() == ExClipPolygonTransient::kClipHole)
                      perp.negate();
                    ExClip::ClipPlane *pClipPlane = clipSpace.createClipPlane();
                    clipSpace.setClipPlane(pClipPlane, ExClip::Plane(poly.pointAt(nVert1), perp));
                    clipSpace.pushShape(pClipPlane); planes.push_back(pClipPlane);
                    nVert1++; nVert2++; if (nVert2 == poly.numPoints()) nVert2 = 0;
                  }
                }
              }
            }
          }
          // Do clipping
          ExClip::PolygonChainChain polyChain;
          for (nTrans = 0; nTrans < Transients.size(); nTrans++)
          {
            const ExClipTransient *pTrans = Transients.getPtr()[nTrans];
            if (pElem->elementId() != ExCmdElem_CurveClip)
            {
              if (pTrans->transientType() == ExClipTransient::kPolygonTransient)
              {
                const ExClipPolygonTransient *pPoly = ExClipPolygonTransient::cast(pTrans);
                if ((pPoly->contourType() == ExClipPolygonTransient::kPolygonContour) ||
                    (pPoly->contourType() == ExClipPolygonTransient::kPolygonHole))
                {
                  if (!(pElem->elementId() == ExCmdElem_PolygonClip))
                  {
                    ExClip::ClipIntervalChain ivals;
                    OdGePoint3dArray points = pPoly->pointsArray();
                    switch (pElem->elementId())
                    {
                      case ExCmdElem_LinesClip:
                      case ExCmdElem_RaysClip:
                        {
                          for (OdUInt32 nSeg = 0; nSeg < points.size() - 1; nSeg++)
                          {
                            ivals.clear();
                            const OdGePoint3d ptOrigin = points[nSeg];
                            const OdGeVector3d ptDirection = points[nSeg + 1] - ptOrigin;
                            if ((pElem->elementId() == ExCmdElem_LinesClip) ?
                                clipSpace.clipLine(ptOrigin, ptDirection, ivals) :
                                clipSpace.clipRay(ptOrigin, ptDirection, ivals))
                            {
                              ExClip::ClipInterval *pIval = ivals.first();
                              while (pIval)
                              {
                                if (pIval->isBoundedFrom())
                                { // In
                                  OdGePoint3d pt; clipSpace.pointAtParam(ptOrigin, ptDirection, pIval->paramFrom(), pt);
                                  ExClipTransient::Ptr markTrans = ExClipMarkerTransient::create(
                                    (pIval->isBoundedTo()) ? ExClipMarkerTransient::kStartVertex : ExClipMarkerTransient::kToInfinityVertex, pt);
                                  renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), markTrans.get());
                                  Transients.push_back(markTrans);
                                }
                                if (pIval->isBoundedTo())
                                { // Out
                                  OdGePoint3d pt; clipSpace.pointAtParam(ptOrigin, ptDirection, pIval->paramTo(), pt);
                                  ExClipTransient::Ptr markTrans = ExClipMarkerTransient::create(
                                    (pIval->isBoundedFrom()) ? ExClipMarkerTransient::kStopVertex : ExClipMarkerTransient::kFromInfinityVertex, pt);
                                  renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), markTrans.get());
                                  Transients.push_back(markTrans);
                                }
                                pIval = ivals.next(pIval);
                              }
                            }
                          }
                        }
                      break;
                      case ExCmdElem_SegmentsClip:
                        if (clipSpace.clipLineSegment(points.size() - 1, points.getPtr(), ivals))
                        {
                          ExClip::ClipInterval *pIval = ivals.first();
                          while (pIval)
                          {
                            OdGePoint3d p1, p2;
                            clipSpace.pointAtParam(points.size() - 1, points.getPtr(), pIval->paramFrom(), p1);
                            clipSpace.pointAtParam(points.size() - 1, points.getPtr(), pIval->paramTo()  , p2);
                            { // Seg
                              ExClipPolygonTransient::Ptr pTrans = ExClipPolygonTransient::create(ExClipPolygonTransient::kClippedContour, p1);
                              for (int nSeg = pIval->paramFrom()->m_nSeg + 1; nSeg <= pIval->paramTo()->m_nSeg; nSeg++)
                              {
                                ExClipPolygonTransient::cast(pTrans)->resetPoint(points[nSeg]);
                                ExClipPolygonTransient::cast(pTrans)->addPoint(points[nSeg]);
                              }
                              ExClipPolygonTransient::cast(pTrans)->resetPoint(p2);
                              renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), pTrans.get());
                              Transients.push_back(pTrans);
                            }
                            { // In
                              // Make transient
                              ExClipTransient::Ptr markTrans = ExClipMarkerTransient::create(ExClipMarkerTransient::kStartVertex, p1);
                              renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), markTrans.get());
                              Transients.push_back(markTrans);
                            }
                            { // Out
                              // Make transient
                              ExClipTransient::Ptr markTrans = ExClipMarkerTransient::create(ExClipMarkerTransient::kStopVertex, p2);
                              renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), markTrans.get());
                              Transients.push_back(markTrans);
                            }
                            pIval = ivals.next(pIval);
                          }
                        }
                      break;
                    }
                  }
                  else
                  { // Polygon clip chain
                    polyChain.append(clipSpace.newPolygonChain());
                    for (OdUInt32 nPt = 0; nPt < pPoly->numberOfPoints(); nPt++)
                      polyChain.last()->append(clipSpace.newClipPoint()->set(pPoly->pointAt(nPt)));
                    polyChain.last()->setHole(pPoly->contourType() == ExClipPolygonTransient::kPolygonHole);
                  }
                }
              }
            }
            else
            { // Curve clip
              if (pTrans->transientType() == ExClipTransient::kCurveTransient)
              {
                const ExClipCurveTransient *pCrv = ExClipCurveTransient::cast(pTrans);
                if ((pCrv->contourType() == ExClipPolygonTransient::kPolygonContour) ||
                    (pCrv->contourType() == ExClipPolygonTransient::kPolygonHole))
                {
                  ExClip::ClipIntervalChain ivals;
                  if (clipSpace.clipCurve(pCrv->curve(), ivals))
                  {
                    ExClip::ClipInterval *pIval = ivals.first();
                    while (pIval)
                    {
                      OdGeCurve3d *pCurve = clipSpace.curveAtInterval(pCrv->curve(), *pIval);
                      { // Sub-curve
                        ExClipTransient::Ptr curveTrans = ExClipCurveTransient::create(ExClipCurveTransient::kClippedContour, pCurve);
                        renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), curveTrans.get());
                        Transients.push_back(curveTrans);
                      }
                      { // In
                        ExClipTransient::Ptr markTrans = ExClipMarkerTransient::create(ExClipMarkerTransient::kStartVertex,
                          pCrv->curve()->evalPoint(pIval->paramFrom()->m_fParam));
                        renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), markTrans.get());
                        Transients.push_back(markTrans);
                      }
                      { // Out
                        ExClipTransient::Ptr markTrans = ExClipMarkerTransient::create(ExClipMarkerTransient::kStopVertex,
                          pCrv->curve()->evalPoint(pIval->paramTo()->m_fParam));
                        renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), markTrans.get());
                        Transients.push_back(markTrans);
                      }
                      pIval = ivals.next(pIval);
                    }
                  }
                }
              }
            }
          }
          if (!polyChain.empty())
          { // Check that AABB algorithm works
            OdGeExtents3d exts = polyChain.first()->extents();
            int needClip = clipSpace.checkAABBClip(exts);
            // Do polygon clipping
            if ((needClip == -1) && clipSpace.clipPolygon(polyChain))
            {
              ExClip::PolygonChain *pPoly = polyChain.first();
              while (pPoly)
              {
                ExClip::ClipPoint *pPt = pPoly->first();
                ExClipPolygonTransient::Ptr pTrans = ExClipPolygonTransient::create(
                  (!pPoly->isHole()) ? ExClipPolygonTransient::kClippedContour : ExClipPolygonTransient::kClippedHole, *pPt);
                pPt = pPoly->next(pPt);
                while (pPt)
                {
                  ExClipPolygonTransient::cast(pTrans)->resetPoint(*pPt);
                  ExClipPolygonTransient::cast(pTrans)->addPoint(*pPt);
                  pPt = pPoly->next(pPt);
                }
                ExClipPolygonTransient::cast(pTrans)->close();
                renderer()->transientManager()->attachTransient(viewer()->activeViewportId(), pTrans.get());
                Transients.push_back(pTrans);
                pPoly = polyChain.next(pPoly);
              }
            }
          }
          if (pClipPoly)
            clipSpace.popShape(pClipPoly, true);
          if (!planes.isEmpty())
          {
            for (OdUInt32 nPln = 0; nPln < planes.size(); nPln++)
              clipSpace.popShape(planes[nPln], true);
          }
        }
      break;
      case ExCmdElem_ClearPolygon:
        clearTransients(ExClipTransient::kPolygonTransient, ExClipPolygonTransient::kPolygonContour);
        clearTransients(ExClipTransient::kPolygonTransient, ExClipPolygonTransient::kPolygonHole);
      break;
      case ExCmdElem_ClearClip:
        clearTransients(ExClipTransient::kPolygonTransient, ExClipPolygonTransient::kClipContour);
        clearTransients(ExClipTransient::kPolygonTransient, ExClipPolygonTransient::kClipHole);
      break;
      case ExCmdElem_ClearResult:
        clearTransients(ExClipTransient::kPolygonTransient, ExClipPolygonTransient::kClippedContour);
        clearTransients(ExClipTransient::kPolygonTransient, ExClipPolygonTransient::kClippedHole);
        clearTransients(ExClipTransient::kMarkerTransient);
      break;
      case ExCmdElem_Save:
        onSave();
      break;
      case ExCmdElem_ClearAll:
        clearTransients((ExClipTransient::TransientType)(-1));
      break;
    }
    redraw(appHandle);
  }
} _ExClipAppClient;

//
