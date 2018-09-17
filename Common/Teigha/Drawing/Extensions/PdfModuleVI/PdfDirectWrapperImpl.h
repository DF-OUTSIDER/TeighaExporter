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

#ifndef _PDFDIRECTWRAPPERIMPL_INCLUDED_
#define _PDFDIRECTWRAPPERIMPL_INCLUDED_

#include "TD_PackPush.h"
#include "PdfDirectWrapper.h"

extern "C" 
{
#include "vgpsflow.h"
#include "custom.h"
}

/** \details
  Library:  Source code provided.
  <group PdfServices_Classes>
*/
class PdfDirectWrapperImpl : public PdfDirectWrapper
{
private:
  class AutoVgPsLayerInfo
  {
    mutable VgPsLayerInfo *m_pInfo;
  public:
    AutoVgPsLayerInfo()
      : m_pInfo(0)
    {
    }

    VgPsLayerInfo &info() const
    {
      if (!m_pInfo)
      {
        m_pInfo = new VgPsLayerInfo;
        memset(&m_pInfo, sizeof(VgPsLayerInfo), 0);
      }
      return *m_pInfo;
    }

    operator const VgPsLayerInfo&() const
    {
      return info();
    }

    operator VgPsLayerInfo&()
    {
      return info();
    }

    operator VgPsLayerInfo*()
    {
      return &info();
    }

    ~AutoVgPsLayerInfo()
    {
      if (m_pInfo)
      {
        delete m_pInfo;
      }
    }
  };
  mutable AutoVgPsLayerInfo m_layerInfoCache;

  typedef std::map<OdString, bool> LevelsAtPage;
  typedef std::map<int, LevelsAtPage> LevelsInfo;
  mutable LevelsInfo m_LevelsInfo;

  virtual double getMeasureDictInfo(int nPage) const;
  const VgPsLayerInfo &getVgPsLayerInfo(int nPage) const;
  const OdGePoint2d &getPageSize(int nPage) const;
  bool checkAnnotationsOnPage(OdUInt32 nPage)  const;
  LevelsInfo::iterator fillLevelInfo(int nPage) const;

  virtual void createGeometryCache(int nPage, const OdArray<OdUnderlayLayer> &Layers) const;

  struct ConversionData
  {
    ConversionData(GeometryCache& cache, const double& scale)
      :m_cache(cache), m_scale(scale){}

    GeometryCache& m_cache;
    double m_scale;
  };

  static int saveGeometry(int type, CuTwoPoint* bbox, CU_OBJECT_ATTR* objattr,
    CU_OBJECT_DATA* objdata, CU_OBJPTR objptr, void* userdata);
  static void saveLine(CU_OBJECT_DATA*, const double& dScale, 
    GeometryCache::Polylines&);
  static void savePolyline(CU_OBJECT_DATA*, const double& dScale, 
    GeometryCache::Polylines&);
  static void saveSplines(CU_OBJECT_DATA*, const double& dScale, 
    GeometryCache::Splines&,
    GeometryCache::Polylines&);
  static void saveEllipse(CU_OBJECT_DATA*, const double& dScale, 
    GeometryCache::Ellipses&);

  void sliceImage(VgPsLayerInfo& layerInfo, const OdDbUnderlayDrawContext& contextWithPaleteFlag, const OdGeExtents2d& clipRect,
                  const OdGiViewportDraw* pVd, OdUInt32 numParts, int nPage, ODCOLORREF backgroundColor) const;

   //void makeAllLayersVisibile(int nPage) const;
   //void updateLayersVisibility(int nPage, const OdArray<OdUnderlayLayer> &layers) const;

public:
  PdfDirectWrapperImpl();
  virtual ~PdfDirectWrapperImpl();
  ODRX_DECLARE_MEMBERS(PdfDirectWrapperImpl);

  virtual OdResult loadFile(OdString path, OdString password);

  virtual OdString getItemName(int nPage) const;
  virtual OdGiRasterImagePtr getThumbnail(int nPage) const;
  virtual OdResult getMediaBox(int nPage, OdGePoint2d& min, OdGePoint2d& max) const;
  virtual void getItemExtents(int nPage, OdGePoint2d& min, OdGePoint2d& max) const;
  virtual void viewportDraw(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const;
  virtual int getItemsCount() const;
  
  virtual OdResult getExplodeGeometry(int nPage, OdGiGeometry *pGrabber,
                                      const OdArray<OdUnderlayLayer> &Layers, bool bIgnoreImages) const;

  virtual int getLevelsCount(int nPage) const;
  virtual OdString getLevelName(int nPage, int nLevel) const;
  virtual OdUInt8 isLevelEnabled(int nPage, int nLevel) const;
  virtual OdUInt8 isLevelEnabled(int nPage, const OdString &name) const;
  virtual void enableLevel(int nPage, int nLevel, OdUInt8 flag);
};
typedef OdSmartPtr<PdfDirectWrapperImpl> PdfDirectWrapperImplPtr;

#include "TD_PackPop.h"

#endif // _PDFDIRECTWRAPPERIMPL_INCLUDED_
