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

#ifndef _DBDEVICEDRIVER_INCLUDED_
#define _DBDEVICEDRIVER_INCLUDED_

#include "TD_PackPush.h"
#include "PdfProperties.h"
#include "DbRasterImage.h"

#include <memory>
#include <vector>
#include "Ge/GePoint2dArray.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeClipBoundary2d.h"
#include "PdfImport.h"

#include "core/fxcrt/retain_ptr.h"
#include "core/fxge/cfx_pathdata.h"
#include "core/fxge/ifx_renderdevicedriver.h"


class OdDbDatabase;

class OdDbDeviceDriver : public IFX_RenderDeviceDriver 
{
  class BaseCurve
  {
  public:
    BaseCurve() {}
    ~BaseCurve() {}
    void addSeg(OdGePoint2dArray points, FXPT_TYPE type)
    {
      m_PointStorage.emplace_back(std::make_pair(points, type));
      if (type != FXPT_TYPE::LineTo)
      {
        m_isPolyline = false;
      }
    }
    void setClosed(bool state)
    {
      m_isClosed = state;
    }
    bool getClosed() const
    {
      return m_isClosed;
    }
    bool isPolyline() const
    {
      return m_isPolyline;
    }

    const std::vector<std::pair<OdGePoint2dArray, FXPT_TYPE>>& getStorage() const
    {
      return m_PointStorage;
    }

    void clear()
    {
      m_PointStorage.clear();
      m_isClosed = false;
    }

  protected:
    std::vector<std::pair<OdGePoint2dArray, FXPT_TYPE>> m_PointStorage;
    bool                                                m_isClosed{ false };
    bool                                                m_isPolyline{ true };

  };
public:
  explicit OdDbDeviceDriver(const int width, const int height, const PdfPropertiesPtr& pProperties, size_t& error_counter, 
    const ByteString& layer_name, const std::map<ByteString, bool>& layers_info, bool& is_object_visible, 
    const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted, const OdString& images_path, OdPdfImport::ImportResult& error);
  ~OdDbDeviceDriver() override;

  // IFX_RenderDeviceDriver
  int GetDeviceCaps(int caps_id) const override;
  void SaveState() override {};
  void RestoreState(bool bKeepSaved) override {};
  bool SetClip_PathFill(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, int fill_mode) override;
  bool SetClip_PathStroke(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState) override;
  bool DrawPath(const CFX_PathData* pPathData, const CFX_Matrix* pObject2Device, const CFX_GraphStateData* pGraphState,
    uint32_t fill_color, uint32_t stroke_color, int fill_mode, int blend_type) override;
  bool GetClipBox(FX_RECT* pRect) override;
  bool SetDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type) override;
  bool StretchDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, int dest_left, int dest_top, int dest_width,
    int dest_height, const FX_RECT* pClipRect, uint32_t flags, int blend_type) override;
  bool StartDIBits(const RetainPtr<CFX_DIBSource>& pBitmap, int bitmap_alpha, uint32_t color, const CFX_Matrix* pMatrix, uint32_t render_flags,
    std::unique_ptr<CFX_ImageRenderer>* handle, int blend_type) override;
  bool DrawDeviceText(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device, float font_size, uint32_t color) override;
  bool DrawCosmeticLine(const CFX_PointF& ptMoveTo, const CFX_PointF& ptLineTo, uint32_t color, int blend_type);
  bool SetPixel(int x, int y, uint32_t color) override;
  bool FillRectWithBlend(const FX_RECT* pRect, uint32_t fill_color, int blend_type) override;
  bool DrawShading(const CPDF_ShadingPattern* pPattern, const CFX_Matrix* pMatrix, const FX_RECT& clip_rect,
    int alpha, bool bAlphaMode);
  bool DrawShadingAsBitmap(const RetainPtr<CFX_DIBSource>& pBitmap, uint32_t color, const FX_RECT* pSrcRect, int left, int top, int blend_type);
  bool SetBitsWithMask(const RetainPtr<CFX_DIBSource>& pBitmap, const RetainPtr<CFX_DIBSource>& pMask,
    int left, int top, int bitmap_alpha, int blend_type);
  int GetDriverType() const override;

private:
  void savePath(const std::vector<BaseCurve>& paths, const OdCmColor color, const CFX_GraphStateData* pGraphState);
  void saveSolid(const std::vector<BaseCurve>& paths, const OdCmColor color);
  OdDbObjectId getLayer(const OdChar* name);
  bool DrawDeviceTextImpl(int nChars, const FXTEXT_CHARPOS* pCharPos, CFX_Font* pFont, const CFX_Matrix* pObject2Device,
    float font_size, uint32_t color);
  OdDbRasterImagePtr createRasterImage(const RetainPtr<CFX_DIBSource>& pBitmap, const OdString& layer_name);
  bool pointInPolygon(const OdGePoint2d& point);

  int                                 m_Width;
  int                                 m_Height;
  const PdfPropertiesPtr              m_pProperties;
  OdDbBlockTableRecordPtr             m_pBTR;
  OdString                            m_PdfFileName;
  OdString                            m_ImagesPath;
  OdGeMatrix3d                        m_TransforMatrix;
  size_t&                             m_ErrorCounter;
  const ByteString&                   m_CurrentLayerName;
  const std::map<ByteString, bool>&   m_LayersInfo;
  bool&                               m_IsObjectVisible;
  OdGePoint2dArray                    m_ClipBoundary;
  bool                                m_isClipBoundaryInverted;
  OdPdfImport::ImportResult&          m_ImageError;
  static const double                 DPI;
  
};

#include "TD_PackPop.h"

#endif // _DBDEVICEDRIVER_INCLUDED_
