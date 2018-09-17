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

#ifndef EX_H_GSHELPERS
#define EX_H_GSHELPERS

#include "TD_PackPush.h"

#include "Gi/GiPathNode.h"
#include "Gi/GiBaseVectorizer.h"
#include "Gs/Gs.h"
#include "Gs/GsViewImpl.h"
// for OdExExtendedLineWeightStyleClient
#include "Ps/PlotStyles.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GePoint3d.h"

// Vectorizer helpers

inline
bool isNestedPersistent(const OdGiPathNode *pCurPath)
{
  while (pCurPath) // skip transients
  {
    if (pCurPath->persistentDrawableId())
      break;
    pCurPath = pCurPath->parent();
  }
  return pCurPath && pCurPath->parent() && pCurPath->parent()->parent();
}

inline
bool renderModeSupportsPlotStyles(OdGsView::RenderMode rm)
{
  return (rm >= OdGsView::k2DOptimized) && (rm <= OdGsView::kWireframe);
}

inline
bool odExDiscardBackfaces(const OdGiBaseVectorizer &vectorizer, bool bDevOverride)
{
  const OdUInt32 drawFlags = vectorizer.drawFlags();
  if (GETBIT(drawFlags, OdGiSubEntityTraits::kDrawBackfaces))
    return false;
  if (GETBIT(drawFlags, OdGiSubEntityTraits::kDrawFrontfacesOnly))
    return true;
  return bDevOverride;
}

// Colors merging

inline
ODCOLORREF OdMergeRGBAlpha(ODCOLORREF dst, ODCOLORREF src, OdUInt8 alpha)
{
  int r, g, b;
  r = (int)ODGETRED(dst);
  r -= ((r - ODGETRED(src))   * (int)alpha) >> 8;
  g = (int)ODGETGREEN(dst);
  g -= ((g - ODGETGREEN(src)) * (int)alpha) >> 8;
  b = (int)ODGETBLUE(dst);
  b -= ((b - ODGETBLUE(src))  * (int)alpha) >> 8;
  return ODRGB(r, g, b);
}

inline
void OdMergeRGBAlpha(OdUInt8 dr, OdUInt8 dg, OdUInt8 db,
                     OdUInt8 sr, OdUInt8 sg, OdUInt8 sb, OdUInt8 alpha,
                     OdUInt8 &_or, OdUInt8 &og, OdUInt8 &ob)
{
  int r, g, b;
  r = (int)dr;
  r -= ((r - sr) * (int)alpha) >> 8;
  g = (int)dg;
  g -= ((g - sg) * (int)alpha) >> 8;
  b = (int)db;
  b -= ((b - sb) * (int)alpha) >> 8;
  _or = (OdUInt8)r;
  og = (OdUInt8)g;
  ob = (OdUInt8)b;
}

// Color analysis

inline
bool isLightColor(ODCOLORREF color)
{
  return (ODGETRED(color) >= 128 || ODGETGREEN(color) >= 128 || ODGETBLUE(color) >= 128);
}

inline
ODCOLORREF makeForegroundColor(ODCOLORREF bkgndColor)
{
  if (isLightColor(bkgndColor))
    return ODRGBA(0, 0, 0, 255);
  return ODRGBA(255, 255, 255, 255);
}

// Hex helpers

inline
OdUInt32 ExGsMakeHex(OdUInt64 uVal, char *pOut, bool bLowerCase = false, bool bLeadingZeroes = false)
{ // Max 16 chars
  const char *pIn = pOut;
#ifdef ODA_BIGENDIAN
  const OdUInt8 *pBufIn = (OdUInt8*)&uVal;
#else
  const OdUInt8 *pBufIn = ((OdUInt8*)&uVal) + 7;
#endif
  bool bFirstLead = bLeadingZeroes;
  for (int nRun = 0; nRun < 8; nRun++)
  {
#ifdef ODA_BIGENDIAN
    OdUInt8 symIn[2] = { pBufIn[nRun] >> 4, pBufIn[nRun] & 0x0F };
#else
    OdUInt8 symIn[2] = { (OdUInt8)(pBufIn[-nRun] >> 4), (OdUInt8)(pBufIn[-nRun] & 0x0F) };
#endif
    for (int nNib = 0; nNib < 2; nNib++)
    {
      if ((symIn[nNib] != 0) || bFirstLead)
      {
        bFirstLead = true;
        if (symIn[nNib] < 10)
          *pOut = '0' + symIn[nNib];
        else
        {
          if (bLowerCase)
            *pOut = 'a' + (symIn[nNib] - 10);
          else
            *pOut = 'A' + (symIn[nNib] - 10);
        }
        pOut++;
      }
    }
  }
  //*pOut = 0;
  return OdUInt32(pOut - pIn);
}

// HSL color space

/** \details
  <group ExRender_Classes> 
*/
template <typename FPType = float>
class OdExHSL
{
  public:
    FPType m_h, m_s, m_l;
  protected:
    void toRGBSub(FPType& c, const FPType& temp1, const FPType& temp2, const FPType& temp3) const
    {
      if ((temp3 * 6) < 1.0f) c = temp2 + (temp1 - temp2) * 6 * temp3;
      else if ((temp3 * 2) < 1.0f) c = temp1;
      else if ((temp3 * 3) < 2.0f) c = temp2 + (temp1 - temp2)*(.66666f - temp3) * 6;
      else c = temp2;
    }
  public:
    explicit OdExHSL(FPType h = 0.0f, FPType s = 0.0f, FPType l = 0.0f) : m_h(h), m_s(s), m_l(l) {}
    // Setters/getters
    void asUInts(OdUInt32 &h, OdUInt32 &s, OdUInt32 &l)
    { h = OdUInt32(m_h * 360); s = OdUInt32(m_s * 100); l = OdUInt32(m_l * 100); }
    void fromUInts(OdUInt32 h, OdUInt32 s, OdUInt32 l)
    { m_h = FPType(h) / 360; m_s = FPType(s) / 100; m_l = FPType(l) / 100; }
    FPType hue() const { return m_h; } void setHue(FPType h) { m_h = h; }
    FPType saturation() const { return m_s; } void setSaturation(FPType s) { m_s = s; }
    FPType lightness() const { return m_l; } void setLightness(FPType l) { m_l = l; }
    // RGB -> HSL
    void fromRGB(FPType r, FPType g, FPType b)
    {
      FPType min_color = 0.0f, max_color = 0.0f;
      if ((r >= g) && (r >= b)) max_color = r;
      if ((g >= r) && (g >= b)) max_color = g;
      if ((b >= r) && (b >= g)) max_color = b;
      if ((r <= g) && (r <= b)) min_color = r;
      if ((g <= r) && (g <= b)) min_color = g;
      if ((b <= r) && (b <= g)) min_color = b;
      m_l = (max_color + min_color) / 2; m_s = m_h = 0.0f;
      if (max_color != min_color)
      {
        if (m_l < 0.5f) m_s = (max_color - min_color) / (max_color + min_color);
        else m_s = (max_color - min_color) / (2.0f - max_color - min_color);
        if (max_color == r) m_h = (g - b) / (max_color - min_color);
        if (max_color == g) m_h = 2 + (b - r) / (max_color - min_color);
        if (max_color == b) m_h = 4 + (r - g) / (max_color - min_color);
      }
      if (m_h < 0.0f) m_h += 6; m_h /= 6;
    }
    void fromRGB(ODCOLORREF color)
    { fromRGB(FPType(ODGETRED(color)) / 255, FPType(ODGETGREEN(color)) / 255, FPType(ODGETBLUE(color)) / 255); }
    // HSL -> RGB
    void toRGB(FPType &r, FPType &g, FPType &b) const
    {
      if (m_s == 0.0f)
        r = g = b = m_l;
      else
      {
        FPType temp1 = 0.0f;
        if (m_l < 0.5f) temp1 = m_l * (m_s + 1);
        else temp1 = m_l + m_s - (m_l * m_s);
        FPType temp2 = 2.0f * m_l - temp1;
        FPType temp3 = m_h + 0.33333f; // red
        if (temp3 > 1.0f) temp3 -= 1;
        toRGBSub(r, temp1, temp2, temp3);
        temp3 = m_h; // green
        toRGBSub(g, temp1, temp2, temp3);
        temp3 = m_h - 0.33333f; // blue
        if (temp3 < 0.0f) temp3 += 1;
        toRGBSub(b, temp1, temp2, temp3);
      }
    }
    ODCOLORREF toRGB() const
    {
      FPType typ_r, typ_g, typ_b;
      toRGB(typ_r, typ_g, typ_b);
      return ODRGB(typ_r * 255, typ_g * 255, typ_b * 255);
    }
};

// OdGsViewImpl accessors

/** \details
  <group ExRender_Classes> 
*/
class GsViewImplHelper
{
  public:
    static void getExtents(const OdGsViewImpl& view, OdGsOverlayId nOverlay, OdGeExtents3d& ext) { view.getExtents(nOverlay, ext); }
};

// PlotStyle lineweights

/** \details
  <group ExRender_Classes> 
*/
class OdExExtendedLineWeightStyleClient
{
  protected:
    OdGeVector3d m_viewNormal;
    double m_lwd, m_lwd2;
    OdPs::LineEndStyle m_capStyle;
    OdPs::LineJoinStyle m_joinStyle;
  private:
    OdGePoint3d m_buffer[4];
    OdUInt32 m_indexes[4];
  public:
    explicit OdExExtendedLineWeightStyleClient(const OdGeVector3d &viewNormal = OdGeVector3d::kZAxis,
                                               double lwd = 1.0,
                                               OdPs::LineEndStyle capStyle = OdPs::kLesRound,
                                               OdPs::LineJoinStyle joinStyle = OdPs::kLjsRound)
      : m_viewNormal(viewNormal)
      , m_lwd(lwd)
      , m_lwd2(lwd / 2)
      , m_capStyle(capStyle)
      , m_joinStyle(joinStyle)
    {
    }

    void setViewNormal(const OdGeVector3d &viewNormal) { m_viewNormal = viewNormal; }
    const OdGeVector3d &viewNormal() const { return m_viewNormal; }

    void setLineWeight(double lwd) { m_lwd = lwd; m_lwd2 = lwd / 2; }
    double lineWeight() const { return m_lwd; }

    void setCapStyle(OdPs::LineEndStyle capStyle) { m_capStyle = capStyle; }
    OdPs::LineEndStyle capStyle() const { return m_capStyle; }

    void setJoinStyle(OdPs::LineJoinStyle joinStyle) { m_joinStyle = joinStyle; }
    OdPs::LineJoinStyle joinStyle() const { return m_joinStyle; }

    OdUInt32 polygonVertexIndex(OdUInt32 nPoint) const { return m_indexes[nPoint]; }
    OdUInt32 circleVertexIndex() const { return *m_indexes; }

    bool process(const OdGePoint3d *pPoints, OdUInt32 nPoints)
    {
      if (nPoints < 2 || pPoints == NULL) return false;
      bool bClosed = (nPoints > 3) && pPoints->isEqualTo(pPoints[nPoints - 1]);
      // Start cap
      if (!bClosed)
      {
        switch (m_capStyle)
        {
          case OdPs::kLesButt:
            // Does nothing
          break;
          case OdPs::kLesSquare:
            {
              OdGeVector3d dir;
              if (!computeInputVector(pPoints, nPoints, dir))
                dir = m_viewNormal.perpVector();
              OdGeVector3d perp = m_viewNormal.crossProduct(dir) * m_lwd2;
              dir *= m_lwd2;
              m_buffer[0] = *pPoints + dir + perp; m_indexes[0] = 0;
              m_buffer[1] = *pPoints + perp;       m_indexes[1] = 0;
              m_buffer[2] = *pPoints - perp;       m_indexes[2] = 0;
              m_buffer[3] = *pPoints + dir - perp; m_indexes[3] = 0;
              if (!elsPolygonOut(m_buffer, 4)) return false;
            }
          break;
          case OdPs::kLesRound:
            *m_indexes = 0; // @@@TODO: optimize using arcs.
            if (!elsCircleOut(*pPoints, m_viewNormal, m_lwd2)) return false;
          break;
          case OdPs::kLesDiamond:
            {
              OdGeVector3d dir;
              if (!computeInputVector(pPoints, nPoints, dir))
                dir = m_viewNormal.perpVector();
              OdGeVector3d perp = m_viewNormal.crossProduct(dir) * m_lwd2;
              m_buffer[0] = *pPoints + dir * m_lwd2; m_indexes[0] = 0;
              m_buffer[1] = *pPoints + perp;         m_indexes[1] = 0;
              m_buffer[2] = *pPoints - perp;         m_indexes[2] = 0;
              if (!elsPolygonOut(m_buffer, 3)) return false;
            }
          break;
          default: break;
        }
      }
      // lines&joints
      OdGeVector3d prevDir, prevPerp; bool bPrevDir = false;
      if (bClosed)
      {
        OdGeVector3d dir;
        if (computeOutputVector(pPoints, nPoints, dir))
        {
          prevDir = dir;
          prevPerp = m_viewNormal.crossProduct(dir);
          bPrevDir = true;
        }
      }
      for (OdUInt32 i = 1; i < nPoints; i++)
      {
        OdGeVector3d dir;
        if (computeDirection(pPoints + i, dir))
        {
          // joint
          if (((i > 1) || bClosed) && bPrevDir)
          {
            switch (m_joinStyle)
            {
              case OdPs::kLjsMiter:
              case OdPs::kLjsBevel:
              case OdPs::kLjsDiamond:
                {
                  OdGeVector3d perp = m_viewNormal.crossProduct(dir);
                  OdGeVector2d perpPrc(prevDir.dotProduct(perp), prevPerp.dotProduct(perp));
                  double angleTo = OdGeVector2d::kYAxis.angleToCCW(perpPrc);
                  if (OdNonZero(angleTo))
                  {
                    double sideMult = 1.0;
                    if (!OdNegative(angleTo))
                    {
                      sideMult = -1.0;
                      angleTo -= Oda2PI;
                    }
                    m_buffer[0] = pPoints[i - 1];                                  m_indexes[0] = i - 1;
                    m_buffer[1] = pPoints[i - 1] + prevPerp * (sideMult * m_lwd2); m_indexes[1] = i - 1;
                    if (m_joinStyle != OdPs::kLjsBevel)
                    {
                      OdGeVector3d vecTo = prevPerp * m_lwd2;
                      vecTo.rotateBy(angleTo * 0.5, m_viewNormal);
                      if (m_joinStyle == OdPs::kLjsMiter)
                      {
                        double divr = fabs(dir.crossProduct(vecTo / m_lwd2).length());
                        if (OdNonZero(divr))
                          vecTo /= divr;
                      }
                      m_buffer[2] = pPoints[i - 1] + vecTo;                      m_indexes[2] = i - 1;
                      m_buffer[3] = pPoints[i - 1] + perp * (sideMult * m_lwd2); m_indexes[3] = i - 1;
                      if (!elsPolygonOut(m_buffer, 4)) return false;
                    }
                    else
                    {
                      m_buffer[2] = pPoints[i - 1] + perp * (sideMult * m_lwd2); m_indexes[2] = i - 1;
                      if (!elsPolygonOut(m_buffer, 3)) return false;
                    }
                  }
                }
              break;
              case OdPs::kLjsRound:
                *m_indexes = i - 1; // @@@TODO: optimize using arcs.
                if (!elsCircleOut(pPoints[i - 1], m_viewNormal, m_lwd2)) return false;
              break;
              default: break;
            }
          }
          // line
          OdGeVector3d perp = m_viewNormal.crossProduct(dir);
          OdGeVector3d perp2(perp * m_lwd2);
          m_buffer[0] = pPoints[i - 1] + perp2; m_indexes[0] = i - 1;
          m_buffer[1] = pPoints[i] + perp2;     m_indexes[1] = i;
          m_buffer[2] = pPoints[i] - perp2;     m_indexes[2] = i;
          m_buffer[3] = pPoints[i - 1] - perp2; m_indexes[3] = i - 1;
          if (!elsPolygonOut(m_buffer, 4)) return false;
          prevDir = dir; prevPerp = perp; bPrevDir = true;
        }
      }
      // End cap
      if (!bClosed)
      {
        switch (m_capStyle)
        {
          case OdPs::kLesButt:
            // Does nothing
          break;
          case OdPs::kLesSquare:
            {
              OdGeVector3d dir;
              if (!computeOutputVector(pPoints, nPoints, dir))
                dir = m_viewNormal.perpVector();
              OdGeVector3d perp = m_viewNormal.crossProduct(dir) * m_lwd2;
              m_buffer[0] = pPoints[nPoints - 1] + dir * m_lwd2 + perp; m_indexes[0] = nPoints - 1;
              m_buffer[1] = pPoints[nPoints - 1] + perp;                m_indexes[1] = nPoints - 1;
              m_buffer[2] = pPoints[nPoints - 1] - perp;                m_indexes[2] = nPoints - 1;
              m_buffer[3] = pPoints[nPoints - 1] + dir * m_lwd2 - perp; m_indexes[3] = nPoints - 1;
              if (!elsPolygonOut(m_buffer, 4)) return false;
            }
          break;
          case OdPs::kLesRound:
            *m_indexes = nPoints - 1; // @@@TODO: optimize using arcs.
            if (!elsCircleOut(pPoints[nPoints - 1], m_viewNormal, m_lwd2)) return false;
          break;
          case OdPs::kLesDiamond:
            {
              OdGeVector3d dir;
              if (!computeOutputVector(pPoints, nPoints, dir))
                dir = m_viewNormal.perpVector();
              OdGeVector3d perp = m_viewNormal.crossProduct(dir) * m_lwd2;
              m_buffer[0] = pPoints[nPoints - 1] + dir * m_lwd2; m_indexes[0] = nPoints - 1;
              m_buffer[1] = pPoints[nPoints - 1] + perp;         m_indexes[1] = nPoints - 1;
              m_buffer[2] = pPoints[nPoints - 1] - perp;         m_indexes[2] = nPoints - 1;
              if (!elsPolygonOut(m_buffer, 3)) return false;
            }
          break;
          default: break;
        }
      }
      return true;
    }

    // Override for convex polygon output (return false for abort processing)
    virtual bool elsPolygonOut(const OdGePoint3d *pPoints, OdUInt32 nPoints) = 0;
    // Override for filled circle output (return false for abort processing)
    virtual bool elsCircleOut(const OdGePoint3d &pPoint, const OdGeVector3d &normal, double radius) = 0;
  protected:
    bool normalize(OdGeVector3d &norm, bool op = true) const
    {
      if (op)
        norm -= m_viewNormal * m_viewNormal.dotProduct(norm); // ortho projected
      OdGe::ErrorCondition errCond;
      norm.normalize(OdGeContext::gTol, errCond);
      return (errCond == OdGe::kOk);
    }
    bool computeInputVector(const OdGePoint3d *pPoints, OdUInt32 nPoints, OdGeVector3d &dir) const
    {
      for (OdUInt32 i = 1; i < nPoints; i++)
      {
        dir = pPoints[i - 1] - pPoints[i];
        if (normalize(dir))
          return true;
      }
      return false;
    }
    bool computeOutputVector(const OdGePoint3d *pPoints, OdUInt32 nPoints, OdGeVector3d &dir) const
    {
      pPoints += nPoints - 1;
      for (OdUInt32 i = 1; i < nPoints; i++, pPoints--)
      {
        dir = pPoints[0] - pPoints[-1];
        if (normalize(dir))
          return true;
      }
      return false;
    }
    bool computeDirection(const OdGePoint3d *pPoints, OdGeVector3d &dir) const
    {
      dir = pPoints[0] - pPoints[-1];
      return normalize(dir);
    }
};

#include "TD_PackPop.h"

#endif // EX_H_GSHELPERS
