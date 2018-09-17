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

#include "OdaCommon.h"
#include "GsBitmapVectorizer.h"
#include "OdRound.h"
#include "RxObjectImpl.h"
#include "Gi/GiRasterWrappers.h"
#include "Ge/GeScale3d.h"

#define OD_OPAQUE(x) ((x) | 0xFF000000)
#define OD_TRANSPARENT(x, y) ((x) | ((ODCOLORREF)(y) << 24))

// OdGsTriangleClipper implementation

#define OD_GSTR_SAMESIGNS(a, b)	\
  (((long)((unsigned long)a ^ (unsigned long)b)) >= 0)

class OdGsTriangleClipper
{
  public:
    OdGsTriangleClipper()
    {
    }
    ~OdGsTriangleClipper()
    {
    }

    // Divide triangle by specified line segment (returns two intersection points and
    // edges numbers if intersection found)
    bool subdivideTriangle(int x1, int y1, int x2, int y2, int x3, int y3, // Input triangle
                           int xl1, int yl1, int xl2, int yl2, // Input line segment
                           int &xpt1, int &ypt1, int &xpt2, int &ypt2, // Output intersection points
                           int &e1, int &e2) // Output edge flags
    {
      long opt_x, opt_y;
      int rval = linesIntersect(long(x1), long(y1), long(x2), long(y2), long(xl1), long(yl1), long(xl2), long(yl2), opt_x, opt_y), ptn = 0;
      if (rval == 1)
        return false;
      if (rval == 2)
      {
        ptn = 1;
        xpt1 = (int)opt_x;
        ypt1 = (int)opt_y;
        e1 = 0;
      }
      rval = linesIntersect(long(x2), long(y2), long(x3), long(y3), long(xl1), long(yl1), long(xl2), long(yl2), opt_x, opt_y);
      if (rval == 1)
        return false;
      if (rval == 2)
      {
        if (ptn == 1)
        {
          xpt2 = (int)opt_x;
          ypt2 = (int)opt_y;
          e2 = 1;
          return true;
        }
        else
        {
          ptn = 1;
          xpt1 = (int)opt_x;
          ypt1 = (int)opt_y;
          e1 = 1;
        }
      }
      rval = linesIntersect(long(x3), long(y3), long(x1), long(y1), long(xl1), long(yl1), long(xl2), long(yl2), opt_x, opt_y);
      if (rval == 1)
        return false;
      if (rval == 2)
      {
        if (ptn == 1)
        {
          xpt2 = (int)opt_x;
          ypt2 = (int)opt_y;
          e2 = 2;
          return true;
        }
      }
      return false;
    }
  protected:
    // Optimized lines intersection routine
    int linesIntersect(long x1, long y1, long x2, long y2, // First line segment
                       long x3, long y3, long x4, long y4, // Second line segment
                       long &x, long &y) // Output point of intersection
    {
      if (x3 == x4)
      {
        // Horizontal xline
        long ymin, ymax, xmin, xmax;
        if (x1 < x2)
        {
          xmin = x1; xmax = x2; ymin = y1; ymax = y2;
        }
        else
        {
          xmin = x2; xmax = x1; ymin = y2; ymax = y1;
        }
        if (x3 >= xmin && x4 <= xmax)
        {
          // Inside segment
          if (x1 == x3 && x2 == x3)
            return 1; // Collinear
          x = x3;
          if (xmax != xmin)
          {
            y = long(double(x3 - xmin) / (xmax - xmin) * (ymax - ymin)) + ymin;
          }
          else
          {
            y = ymin;
          }
          return 2;
        }
        return 0;
      }
      else if (y3 == y4)
      {
        // Vertical xline
        long ymin, ymax, xmin, xmax;
        if (y1 < y2)
        {
          xmin = x1; xmax = x2; ymin = y1; ymax = y2;
        }
        else
        {
          xmin = x2; xmax = x1; ymin = y2; ymax = y1;
        }
        if (y3 >= ymin && y4 <= ymax)
        {
          // Inside segment
          if (y1 == y3 && y2 == y3)
            return 1; // Collinear
          y = y3;
          if (ymax != ymin)
          {
            x = long(double(y3 - ymin) / (ymax - ymin) * (xmax - xmin)) + xmin;
          }
          else
          {
            x = xmin;
          }
          return 2;
        }
        return 0;
      }
      else
      {
        long a1, a2, b1, b2, c1, c2; // Coefficients of line eqns.
        long r1, r2, r3, r4; // 'Sign' values
        long denom, offset, num; // Intermediate values
        // Compute a1, b1, c1, where line joining points 1 and 2
        // is "a1 x + b1 y + c1 = 0".
        a1 = y2 - y1;
        b1 = x1 - x2;
        c1 = x2 * y1 - x1 * y2;
        // Compute r3 and r4.
        r3 = a1 * x3 + b1 * y3 + c1;
        r4 = a1 * x4 + b1 * y4 + c1;
        // Check signs of r3 and r4.  If both point 3 and point 4 lie on
        // same side of line 1, the line segments do not intersect.
        if (r3 != 0 && r4 != 0 && OD_GSTR_SAMESIGNS(r3, r4))
          return 0; // No intersection
        // Compute a2, b2, c2
        a2 = y4 - y3;
        b2 = x3 - x4;
        c2 = x4 * y3 - x3 * y4;
        // Compute r1 and r2
        r1 = a2 * x1 + b2 * y1 + c2;
        r2 = a2 * x2 + b2 * y2 + c2;
        // Check signs of r1 and r2. If both point 1 and point 2 lie
        // on same side of second line segment, the line segments do
        // not intersect.
        if (r1 != 0 && r2 != 0 && OD_GSTR_SAMESIGNS(r1, r2))
          return 0; // No intersection
        // Line segments intersect: compute intersection point. 
        denom = a1 * b2 - a2 * b1;
        if (denom == 0)
          return 1; // Line segments are collinear
        offset = (denom < 0) ? (-denom / 2) : (denom / 2);
        // The denom / 2 is to get rounding instead of truncating.  It
        // is added or subtracted to the numerator, depending upon the
        // sign of the numerator.
        num = b1 * c2 - b2 * c1;
        x = ((num < 0) ? (num - offset) : (num + offset)) / denom;
        num = a2 * c1 - a1 * c2;
        y = ((num < 0) ? (num - offset) : (num + offset)) / denom;
        return 2; // Line segments intersects
      }
    }

    // Compute triangle extents
    void triangleExtents(int x1, int y1, int x2, int y2, int x3, int y3,
                         int &xmin, int &ymin, int &xmax, int &ymax, int offset = 0)
    {
      xmin = odmin(x1, odmin(x2, x3));
      ymin = odmin(y1, odmin(y2, y3));
      xmax = odmax(x1, odmax(x2, x3));
      ymax = odmax(y1, odmax(y2, y3));
      xmin -= offset; ymin -= offset;
      ymax += offset; ymax += offset;
    }
};

// end of OdGsTriangleClipper implementation

// OdGsTriangleRenderer implementation

// Vertex clipping types
#define OD_GSTR_CLIPLEFT   1
#define OD_GSTR_CLIPTOP    2
#define OD_GSTR_CLIPRIGHT  4
#define OD_GSTR_CLIPBOTTOM 8

// Optimized vertices sorting
#define OD_GSTR_SORTVERTICES \
  if (pp1->y > pp2->y) { temp = pp1; pp1 = pp2; pp2 = temp; } \
  if (pp2->y > pp3->y) { temp = pp2; pp2 = pp3; pp3 = temp; } \
  if (pp1->y > pp2->y) { temp = pp1; pp1 = pp2; pp2 = temp; }

// Fixed arithmetic inversion
#define OD_GSTR_INVERSE(a) (((a) < 1) ? (0) : (65536 / (a)))

// Fixed arithmetic multiply
inline int OD_GSTR_FIXMUL(int a, int b)
{
#if defined(_MSC_VER) && _M_IX86 >= 300 && !defined(_WIN32_WCE)
  // Intel386-486 platform with Microsoft compiler
  __asm
  {
    xor edx, edx
    mov eax, a
    mov ebx, b
    imul ebx
    shrd eax, edx, 16
    mov a, eax
  }
  return (a);
#elif defined(_WIN32) && defined(_ULONGLONG_) // Defined in windows header
  LONGLONG ll = Int32x32To64(a, b);
  ll = Int64ShraMod32(ll, 16);
  return (int)ll;
//#elif defined(_WIN32)
//  ::MulDiv(a, b, 65536); // Returns little bit incorrect results as result of rounding
#else
  // Unoptimized C++
  //return (int)(double(a) * double(b) * (1.0 / 65536.0));
  // Doubles sometimes return little bit incorrect results as result of rounding
  // More fast version without division and using build-in types
  return (int)((OdInt64(a) * b) >> 16);
  // We haven't currently multiplier operation into "Int64.h", but if this is necessary it have very simple
  // implementation: http://hackersdelight.org/HDcode.htm
#endif
}

// Increase performance for PPC, using fixed arithmetic, but decrease textures rendering quality.
#ifdef _WIN32_WCE
#define OD_GSTR_USEFIXEDTEX
#endif

// Texture addressing modes

static void textureAddressing_Clamp(int &u, int &v, int texWidth, int texHeight)
{
  if (u < 0)
    u = 0;
  else if (u >= texWidth)
    u = int(texWidth) - 1;
  if (v < 0)
    v = 0;
  else if (v >= texHeight)
    v = int(texHeight) - 1;
}

static void textureAddressing_Wrap(int &u, int &v, int texWidth, int texHeight)
{
  if (u < 0)
    u = texWidth + (u + (-u) / texWidth * texWidth);
  if (v < 0)
    v = texHeight + (v + (-v) / texHeight * texHeight);
  u %= texWidth;
  v %= texHeight;
}

// Texture blitting modes

static void textureBlit_Nearest_1bpp(const OdUInt8 *pTexScanLines, const ODCOLORREF *pTexPalette,
                                     OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  OdUInt8 nColorIndex = ((pTexScanLines + texScanLineLength * v)[u / 8] >> (0x07 - (u % 8))) & 0x01;
  *(bgr++) = ODGETBLUE(pTexPalette[nColorIndex]);
  *(bgr++) = ODGETGREEN(pTexPalette[nColorIndex]);
  *bgr     = ODGETRED(pTexPalette[nColorIndex]);
}

static void textureBlit_Nearest_1bpp_Transparent(const OdUInt8 *pTexScanLines, const ODCOLORREF *pTexPalette,
                                                 OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  OdUInt8 nColorIndex = ((pTexScanLines + texScanLineLength * v)[u / 8] >> (0x07 - (u % 8))) & 0x01;
  if (ODGETALPHA(pTexPalette[nColorIndex]))
  {
    *(bgr++) = ODGETBLUE(pTexPalette[nColorIndex]);
    *(bgr++) = ODGETGREEN(pTexPalette[nColorIndex]);
    *bgr     = ODGETRED(pTexPalette[nColorIndex]);
  }
}

static void textureBlit_Nearest_4bpp(const OdUInt8 *pTexScanLines, const ODCOLORREF *pTexPalette,
                                     OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  OdUInt8 nColorIndex = ((u % 2) & 0x01) ?  ((pTexScanLines + texScanLineLength * v)[u / 2] & 0x0F) :
                                           (((pTexScanLines + texScanLineLength * v)[u / 2] & 0xF0) >> 4);
  *(bgr++) = ODGETBLUE(pTexPalette[nColorIndex]);
  *(bgr++) = ODGETGREEN(pTexPalette[nColorIndex]);
  *bgr     = ODGETRED(pTexPalette[nColorIndex]);
}

static void textureBlit_Nearest_8bpp(const OdUInt8 *pTexScanLines, const ODCOLORREF *pTexPalette,
                                     OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  OdUInt8 nColorIndex = (pTexScanLines + texScanLineLength * v)[u];
  *(bgr++) = ODGETBLUE(pTexPalette[nColorIndex]);
  *(bgr++) = ODGETGREEN(pTexPalette[nColorIndex]);
  *bgr     = ODGETRED(pTexPalette[nColorIndex]);
}

static void textureBlit_Nearest_24bpp(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                      OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const OdUInt8 *texBGR = pTexScanLines + texScanLineLength * v + u * 3;
  *(bgr++) = *(texBGR++);
  *(bgr++) = *(texBGR++);
  *bgr     = *texBGR;
}

static void textureBlit_Nearest_24bpp_RGB(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                          OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const OdUInt8 *texRGB = pTexScanLines + texScanLineLength * v + u * 3;
  *(bgr++) = texRGB[2];
  *(bgr++) = texRGB[1];
  *bgr     = *texRGB;
}

static void textureBlit_Nearest_32bpp(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                      OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  *(bgr++) = ODGETRED(*texBGRA);
  *(bgr++) = ODGETGREEN(*texBGRA);
  *bgr     = ODGETBLUE(*texBGRA);
}

static void textureBlit_Nearest_32bpp_TransparentDef(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                                     OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  if (ODGETALPHA(*texBGRA))
  {
    *(bgr++) = ODGETRED(*texBGRA);
    *(bgr++) = ODGETGREEN(*texBGRA);
    *bgr     = ODGETBLUE(*texBGRA);
  }
}

static void textureBlit_Nearest_32bpp_Transparent1Bit(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                                      OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  if (ODGETALPHA(*texBGRA) == 255)
  {
    *(bgr++) = ODGETRED(*texBGRA);
    *(bgr++) = ODGETGREEN(*texBGRA);
    *bgr     = ODGETBLUE(*texBGRA);
  }
}

static void textureBlit_Nearest_32bpp_Transparent8Bit(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                                      OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  if (ODGETALPHA(*texBGRA))
  {
    OdMergeRGBAlpha(bgr[0], bgr[1], bgr[2], ODGETRED(*texBGRA), ODGETGREEN(*texBGRA), ODGETBLUE(*texBGRA), ODGETALPHA(*texBGRA), bgr[0], bgr[1], bgr[2]);
  }
}

static void textureBlit_Nearest_32bpp_RGB(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                          OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  *(bgr++) = ODGETBLUE(*texBGRA);
  *(bgr++) = ODGETGREEN(*texBGRA);
  *bgr     = ODGETRED(*texBGRA);
}

static void textureBlit_Nearest_32bpp_RGB_TransparentDef(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                                         OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  if (ODGETALPHA(*texBGRA))
  {
    *(bgr++) = ODGETBLUE(*texBGRA);
    *(bgr++) = ODGETGREEN(*texBGRA);
    *bgr     = ODGETRED(*texBGRA);
  }
}

static void textureBlit_Nearest_32bpp_RGB_Transparent1Bit(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                                          OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  if (ODGETALPHA(*texBGRA) == 255)
  {
    *(bgr++) = ODGETBLUE(*texBGRA);
    *(bgr++) = ODGETGREEN(*texBGRA);
    *bgr     = ODGETRED(*texBGRA);
  }
}

static void textureBlit_Nearest_32bpp_RGB_Transparent8Bit(const OdUInt8 *pTexScanLines, const ODCOLORREF * /*pTexPalette*/,
                                                          OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr)
{
  const ODCOLORREF *texBGRA = reinterpret_cast<const ODCOLORREF*>(pTexScanLines + texScanLineLength * v + u * 4);
  if (ODGETALPHA(*texBGRA))
  {
    OdMergeRGBAlpha(bgr[0], bgr[1], bgr[2], ODGETBLUE(*texBGRA), ODGETGREEN(*texBGRA), ODGETRED(*texBGRA), ODGETALPHA(*texBGRA), bgr[0], bgr[1], bgr[2]);
  }
}

// OdGsTriangleRenderer

class OdGsTriangleRenderer
  : protected OdGsTriangleClipper
{
  protected:
    // Renderable flat vertex
    struct rVertexFlat
    {
      int x, y;
    };
    // Renderable flat edge
    struct rEdgeFlat
    {
      int x, dx;
    };
    // Precis renderable flat edge
    struct rEdgeFlatPrec
    {
      double x, dx;
    };
    // Renderable gouraud vertex
    struct rVertexGouraud
    {
      int x, y;
      int r, g, b;
    };
    // Renderable gouraud edge
    struct rEdgeGouraud
    {
      int x, dx;
      int r, dr, g, dg, b, db;
    };
    // Precis renderable gouraud edge
    struct rEdgeGouraudPrec
    {
      double x, dx;
      double r, dr, g, dg, b, db;
    };
    // Renderable textured vertex
    struct rVertexTextured
    {
      int x, y;
#ifdef OD_GSTR_USEFIXEDTEX
      int u, v;
#else
      double s, t;
#endif
    };
    // Renderable textured edge
    struct rEdgeTextured
    {
      int x, dx;
#ifdef OD_GSTR_USEFIXEDTEX
      int u, du, v, dv;
#else
      double s, ds, t, dt;
#endif
    };
    // Precis renderable textured vertex
    struct rVertexTexturedPrec
    {
      int x, y;
      double s, t;
    };
    // Precis renderable textured edge
    struct rEdgeTexturedPrec
    {
      double x, dx;
      double s, ds, t, dt;
    };
    // Information of triangle subdivision
    struct rSubdivideInfo
    {
      int x1, y1;
      int x2, y2;
      int e1, e2;
      OdUInt32 side;
    };
  protected:
    OdUInt32 m_width, m_height, m_scanLineLength;
    OdGsDCRect m_clipRegion;
    OdUInt8 *m_pScanLines; // BGR array
    // NRC viewport clip
    long m_nrcOffsetX, m_nrcOffsetY;
    OdUInt32 m_nrcWidth, m_nrcHeight;
    OdUInt8 *m_pnrcClip;
    // Only for textured triangles
    OdUInt32 m_texWidth, m_texHeight;
    OdUInt32 m_texScanLineLength;
    const OdUInt8 *m_pTexScanLines;
    const ODCOLORREF *m_pTexPalette;
    // Additional settings
    void (*m_fpTextureAddressing)(int &u, int &v, int texWidth, int texHeight);
    void (*m_fpTextureBlit)(const OdUInt8 *pTexScanLines, const ODCOLORREF *pTexPalette, OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr);
  public:
    OdGsTriangleRenderer(OdUInt32 width, OdUInt32 height, OdUInt8 *pScanLines, OdUInt32 scanLineLength, const OdGsDCRect &clipRegion,
                         long nrcOffsetX, long nrcOffsetY, OdUInt32 nrcWidth, OdUInt32 nrcHeight, OdUInt8 *pnrcBuf)
      : m_width(width)
      , m_height(height)
      , m_scanLineLength(scanLineLength)
      , m_clipRegion(clipRegion)
      , m_pScanLines(pScanLines)
      , m_nrcOffsetX(nrcOffsetX)
      , m_nrcOffsetY(nrcOffsetY)
      , m_nrcWidth(nrcWidth)
      , m_nrcHeight(nrcHeight)
      , m_pnrcClip(pnrcBuf)
      , m_texWidth(0)
      , m_texHeight(0)
      , m_texScanLineLength(0)
      , m_pTexScanLines(NULL)
      , m_pTexPalette(NULL)
      , m_fpTextureAddressing(textureAddressing_Clamp)
      , m_fpTextureBlit(textureBlit_Nearest_24bpp)
    {
      prepareClipRegion();
    }
    OdGsTriangleRenderer(OdUInt32 width, OdUInt32 height, OdUInt8 *pScanLines, OdUInt32 scanLineLength, const OdGsDCRect &clipRegion,
                         long nrcOffsetX, long nrcOffsetY, OdUInt32 nrcWidth, OdUInt32 nrcHeight, OdUInt8 *pnrcBuf,
                         OdUInt32 texWidth, OdUInt32 texHeight, const OdUInt8 *pTexScanLines, OdUInt32 texScanLineLength, const ODCOLORREF *pTexPalette)
      : m_width(width)
      , m_height(height)
      , m_scanLineLength(scanLineLength)
      , m_clipRegion(clipRegion)
      , m_pScanLines(pScanLines)
      , m_nrcOffsetX(nrcOffsetX)
      , m_nrcOffsetY(nrcOffsetY)
      , m_nrcWidth(nrcWidth)
      , m_nrcHeight(nrcHeight)
      , m_pnrcClip(pnrcBuf)
      , m_texWidth(texWidth)
      , m_texHeight(texHeight)
      , m_texScanLineLength(texScanLineLength)
      , m_pTexScanLines(pTexScanLines)
      , m_pTexPalette(pTexPalette)
      , m_fpTextureAddressing(textureAddressing_Clamp)
      , m_fpTextureBlit(textureBlit_Nearest_24bpp)
    {
      prepareClipRegion();
    }
    ~OdGsTriangleRenderer()
    {
    }

    void setTexture(OdUInt32 texWidth, OdUInt32 texHeight, OdUInt8 *pTexScanLines, OdUInt32 texScanLineLength, const ODCOLORREF *pTexPalette)
    {
      m_texWidth = texWidth;
      m_texHeight = texHeight;
      m_pTexScanLines = pTexScanLines;
      m_texScanLineLength = texScanLineLength;
      m_pTexPalette = pTexPalette;
    }

    OdGsTriangleRenderer &setTextureAddressingFunc(void (*fpTextureAddressing)(int &u, int &v, int texWidth, int texHeight))
    {
      m_fpTextureAddressing = fpTextureAddressing;
      return *this;
    }
    OdGsTriangleRenderer &setTextureBlittingFunc(void (*fpTextureBlit)(const OdUInt8 *pTexScanLines, const ODCOLORREF *pTexPalette, OdUInt32 texScanLineLength, int u, int v, OdUInt8 *bgr))
    {
      m_fpTextureBlit = fpTextureBlit;
      return *this;
    }

    void prepareClipRegion()
    {
      long temp = m_clipRegion.m_max.y;
      m_clipRegion.m_max.y = (m_height - 1) - m_clipRegion.m_min.y;
      m_clipRegion.m_min.y = (m_height - 1) - temp;
      m_clipRegion.m_max.x++;
      m_clipRegion.m_max.y++;
    }

    // Return region-triangle clip state.
    OdUInt32 regionTriangleClipState(int x1, int y1, int x2, int y2, int x3, int y3, OdUInt32 *clipTest = NULL)
    {
      OdUInt32 clipFlags[3] = { 0, 0, 0 };
      if (x1 < m_clipRegion.m_min.x)
        clipFlags[0] |= OD_GSTR_CLIPLEFT;
      else if (x1 >= m_clipRegion.m_max.x)
        clipFlags[0] |= OD_GSTR_CLIPRIGHT;
      if (y1 < m_clipRegion.m_min.y)
        clipFlags[0] |= OD_GSTR_CLIPTOP;
      else if (y1 > m_clipRegion.m_max.y)
        clipFlags[0] |= OD_GSTR_CLIPBOTTOM;
      if (x2 < m_clipRegion.m_min.x)
        clipFlags[1] |= OD_GSTR_CLIPLEFT;
      else if (x2 >= m_clipRegion.m_max.x)
        clipFlags[1] |= OD_GSTR_CLIPRIGHT;
      if (y2 < m_clipRegion.m_min.y)
        clipFlags[1] |= OD_GSTR_CLIPTOP;
      else if (y2 > m_clipRegion.m_max.y)
        clipFlags[1] |= OD_GSTR_CLIPBOTTOM;
      if (x3 < m_clipRegion.m_min.x)
        clipFlags[2] |= OD_GSTR_CLIPLEFT;
      else if (x3 >= m_clipRegion.m_max.x)
        clipFlags[2] |= OD_GSTR_CLIPRIGHT;
      if (y3 < m_clipRegion.m_min.y)
        clipFlags[2] |= OD_GSTR_CLIPTOP;
      else if (y3 > m_clipRegion.m_max.y)
        clipFlags[2] |= OD_GSTR_CLIPBOTTOM;
      if (clipTest != 0)
      {
        *clipTest = ((clipFlags[0] & clipFlags[1]) & clipFlags[2]);
      }
      return ((clipFlags[0] | clipFlags[1]) | clipFlags[2]);
    }

    // Test region-triangle clipping. Return true if triangle totally clipped by region.
    bool regionTriangleClipTest(int x1, int y1, int x2, int y2, int x3, int y3)
    {
      OdUInt32 clipTest;
      regionTriangleClipState(x1, y1, x2, y2, x3, y3, &clipTest);
      return clipTest != 0;
    }

    // Render flat triangle
    void renderTriangleFlat(int x1, int y1, int x2, int y2, int x3, int y3, ODCOLORREF color)
    {
      rVertexFlat triVertex[3]; OdUInt8 r, g, b, a;

      //if (regionTriangleClipTest(x1, y1, x2, y2, x3, y3))
      if (clipTriangleFlat(x1, y1, x2, y2, x3, y3, color))
      {
        return;
      }

      triVertex[0].x = x1;
      triVertex[0].y = y1;
      triVertex[1].x = x2;
      triVertex[1].y = y2;
      triVertex[2].x = x3;
      triVertex[2].y = y3;

      r = ODGETRED(color);
      g = ODGETGREEN(color);
      b = ODGETBLUE(color);
      a = ODGETALPHA(color);

      rVertexFlat *pp1 = triVertex, *pp2 = triVertex + 1, *pp3 = triVertex + 2, *temp;
      OD_GSTR_SORTVERTICES

      // Render triangle
      rEdgeFlat eshort, elong;
      int y, h;
      // Long side
      if (edgeTriangleFlat(&elong, pp1, pp3) <= 0)
      {
        if (pp1->y == pp3->y)
        {
          blitFlatLine(pp1->y, pp1->x, pp3->x, r, g, b, a);
        }
        return;
      }
      // Start y
      y = odmax(pp1->y, m_clipRegion.m_min.y);
      // Top short side
      h = edgeTriangleFlat(&eshort, pp1, pp2);
      if (h > 0)
      {
        if (eshort.dx < elong.dx)
          blitTriangleFlat(&eshort, &elong, y, h, r, g, b, a);
        else
          blitTriangleFlat(&elong, &eshort, y, h, r, g, b, a);
        y += h;
      }
      else if (pp1->y == pp2->y)
      {
        blitFlatLine(y, pp1->x, pp2->x, r, g, b, a);
      }
      // Bottom short side
      h = edgeTriangleFlat(&eshort, pp2, pp3);
      if (h > 0)
      {
        if (eshort.x < elong.x)
          blitTriangleFlat(&eshort, &elong, y, h, r, g, b, a);
        else
          blitTriangleFlat(&elong, &eshort, y, h, r, g, b, a);
      }
      else if (pp2->y == pp3->y)
      {
        blitFlatLine(y, pp2->x, pp3->x, r, g, b, a);
      }
    }
    // Render precis flat triangle
    void renderTriangleFlatPrec(int x1, int y1, int x2, int y2, int x3, int y3, ODCOLORREF color)
    {
      rVertexFlat triVertex[3]; OdUInt8 r, g, b, a;

      if (regionTriangleClipTest(x1, y1, x2, y2, x3, y3))
      {
        return;
      }

      triVertex[0].x = x1;
      triVertex[0].y = y1;
      triVertex[1].x = x2;
      triVertex[1].y = y2;
      triVertex[2].x = x3;
      triVertex[2].y = y3;

      r = ODGETRED(color);
      g = ODGETGREEN(color);
      b = ODGETBLUE(color);
      a = ODGETALPHA(color);

      rVertexFlat *pp1 = triVertex, *pp2 = triVertex + 1, *pp3 = triVertex + 2, *temp;
      OD_GSTR_SORTVERTICES
      // @@@TODO: looks for comments in edgeTriangleFlatPrec/blitTriangleFlatPrec for improvements of precision
      //          mode. 1) input as doubles can be useful for precision mode.
      // Render triangle
      rEdgeFlatPrec eshort, elong;
      int y, h;
      // Long side
      if (edgeTriangleFlatPrec(&elong, pp1, pp3) <= 0)
      {
        if (pp1->y == pp3->y)
        {
          blitFlatLine(pp1->y, pp1->x, pp3->x, r, g, b, a);
        }
        return;
      }
      // Start y
      y = odmax(pp1->y, m_clipRegion.m_min.y);
      // Top short side
      h = edgeTriangleFlatPrec(&eshort, pp1, pp2);
      if (h > 0)
      {
        if (eshort.dx < elong.dx)
          blitTriangleFlatPrec(&eshort, &elong, y, h, r, g, b, a);
        else
          blitTriangleFlatPrec(&elong, &eshort, y, h, r, g, b, a);
        y += h;
      }
      else if (pp1->y == pp2->y)
      {
        blitFlatLine(y, pp1->x, pp2->x, r, g, b, a);
      }
      // Bottom short side
      h = edgeTriangleFlatPrec(&eshort, pp2, pp3);
      if (h > 0)
      {
        if (eshort.x < elong.x)
          blitTriangleFlatPrec(&eshort, &elong, y, h, r, g, b, a);
        else
          blitTriangleFlatPrec(&elong, &eshort, y, h, r, g, b, a);
      }
      else if (pp2->y == pp3->y)
      {
        blitFlatLine(y, pp2->x, pp3->x, r, g, b, a);
      }
    }
    // Render gouraud triangle
    void renderTriangleGouraud(int x1, int y1, ODCOLORREF color1,
                               int x2, int y2, ODCOLORREF color2,
                               int x3, int y3, ODCOLORREF color3, OdUInt8 alpha)
    {
      rVertexGouraud triVertex[3];

      //if (regionTriangleClipTest(x1, y1, x2, y2, x3, y3))
      if (clipTriangleGouraud(x1, y1, color1, x2, y2, color2, x3, y3, color3, alpha))
      {
        return;
      }

      triVertex[0].x = x1;
      triVertex[0].y = y1;
      triVertex[0].r = ODGETRED(color1);
      triVertex[0].g = ODGETGREEN(color1);
      triVertex[0].b = ODGETBLUE(color1);
      triVertex[1].x = x2;
      triVertex[1].y = y2;
      triVertex[1].r = ODGETRED(color2);
      triVertex[1].g = ODGETGREEN(color2);
      triVertex[1].b = ODGETBLUE(color2);
      triVertex[2].x = x3;
      triVertex[2].y = y3;
      triVertex[2].r = ODGETRED(color3);
      triVertex[2].g = ODGETGREEN(color3);
      triVertex[2].b = ODGETBLUE(color3);

      rVertexGouraud *pp1 = triVertex, *pp2 = triVertex + 1, *pp3 = triVertex + 2, *temp;
      OD_GSTR_SORTVERTICES

      // Render triangle
      rEdgeGouraud eshort, elong;
      int y, h;
      // Long side
      if (edgeTriangleGouraud(&elong, pp1, pp3) <= 0)
      {
        if (pp1->y == pp3->y)
        {
          blitGouraudLine(pp1->y, pp1->x, pp3->x, pp1->r, pp1->g, pp1->b, pp3->r, pp3->g, pp3->b, alpha);
        }
        return;
      }
      // Start y
      y = odmax(pp1->y, m_clipRegion.m_min.y);
      // Top short side
      h = edgeTriangleGouraud(&eshort, pp1, pp2);
      if (h > 0)
      {
        if (eshort.dx < elong.dx)
          blitTriangleGouraud(&eshort, &elong, y, h, alpha);
        else
          blitTriangleGouraud(&elong, &eshort, y, h, alpha);
        y += h;
      }
      else if (pp1->y == pp2->y)
      {
        blitGouraudLine(pp1->y, pp1->x, pp2->x, pp1->r, pp1->g, pp1->b, pp2->r, pp2->g, pp2->b, alpha);
      }
      // Bottom short side
      h = edgeTriangleGouraud(&eshort, pp2, pp3);
      if (h > 0)
      {
        if (eshort.x < elong.x)
          blitTriangleGouraud(&eshort, &elong, y, h, alpha);
        else
          blitTriangleGouraud(&elong, &eshort, y, h, alpha);
      }
      else if (pp2->y == pp3->y)
      {
        blitGouraudLine(pp2->y, pp2->x, pp3->x, pp2->r, pp2->g, pp2->b, pp3->r, pp3->g, pp3->b, alpha);
      }
    }
    // Render precis gouraud triangle
    void renderTriangleGouraudPrec(int x1, int y1, ODCOLORREF color1,
                                   int x2, int y2, ODCOLORREF color2,
                                   int x3, int y3, ODCOLORREF color3, OdUInt8 alpha)
    {
      rVertexGouraud triVertex[3];

      if (regionTriangleClipTest(x1, y1, x2, y2, x3, y3))
      {
        return;
      }

      triVertex[0].x = x1;
      triVertex[0].y = y1;
      triVertex[0].r = ODGETRED(color1);
      triVertex[0].g = ODGETGREEN(color1);
      triVertex[0].b = ODGETBLUE(color1);
      triVertex[1].x = x2;
      triVertex[1].y = y2;
      triVertex[1].r = ODGETRED(color2);
      triVertex[1].g = ODGETGREEN(color2);
      triVertex[1].b = ODGETBLUE(color2);
      triVertex[2].x = x3;
      triVertex[2].y = y3;
      triVertex[2].r = ODGETRED(color3);
      triVertex[2].g = ODGETGREEN(color3);
      triVertex[2].b = ODGETBLUE(color3);

      rVertexGouraud *pp1 = triVertex, *pp2 = triVertex + 1, *pp3 = triVertex + 2, *temp;
      OD_GSTR_SORTVERTICES

      // Render triangle
      rEdgeGouraudPrec eshort, elong;
      int y, h;
      // Long side
      if (edgeTriangleGouraudPrec(&elong, pp1, pp3) <= 0)
      {
        if (pp1->y == pp3->y)
        {
          blitGouraudLine(pp1->y, pp1->x, pp3->x, pp1->r, pp1->g, pp1->b, pp3->r, pp3->g, pp3->b, alpha);
        }
        return;
      }
      // Start y
      y = odmax(pp1->y, m_clipRegion.m_min.y);
      // Top short side
      h = edgeTriangleGouraudPrec(&eshort, pp1, pp2);
      if (h > 0)
      {
        if (eshort.dx < elong.dx)
          blitTriangleGouraudPrec(&eshort, &elong, y, h, alpha);
        else
          blitTriangleGouraudPrec(&elong, &eshort, y, h, alpha);
        y += h;
      }
      else if (pp1->y == pp2->y)
      {
        blitGouraudLine(pp1->y, pp1->x, pp2->x, pp1->r, pp1->g, pp1->b, pp2->r, pp2->g, pp2->b, alpha);
      }
      // Bottom short side
      h = edgeTriangleGouraudPrec(&eshort, pp2, pp3);
      if (h > 0)
      {
        if (eshort.x < elong.x)
          blitTriangleGouraudPrec(&eshort, &elong, y, h, alpha);
        else
          blitTriangleGouraudPrec(&elong, &eshort, y, h, alpha);
      }
      else if (pp2->y == pp3->y)
      {
        blitGouraudLine(pp2->y, pp2->x, pp3->x, pp2->r, pp2->g, pp2->b, pp3->r, pp3->g, pp3->b, alpha);
      }
    }
    // Render textured triangle
    void renderTriangleTextured(int x1, int y1, double s1, double t1,
                                int x2, int y2, double s2, double t2,
                                int x3, int y3, double s3, double t3, OdUInt8 alpha)
    {
      rVertexTextured triVertex[3];

      //if (regionTriangleClipTest(x1, y1, x2, y2, x3, y3))
      if (clipTriangleTextured(x1, y1, s1, t1, x2, y2, s2, t2, x3, y3, s3, t3, alpha))
      {
        return;
      }

      ODA_ASSERT(m_texWidth > 0 && m_texHeight > 0 && m_texScanLineLength > 0 && m_pTexScanLines != 0);

      triVertex[0].x = x1;
      triVertex[0].y = y1;
#ifdef OD_GSTR_USEFIXEDTEX
      triVertex[0].u = int(OdRound(s1 * m_texWidth));
      triVertex[0].v = int(OdRound(t1 * m_texHeight));
#else
      triVertex[0].s = s1 * m_texWidth;
      triVertex[0].t = t1 * m_texHeight;
#endif
      triVertex[1].x = x2;
      triVertex[1].y = y2;
#ifdef OD_GSTR_USEFIXEDTEX
      triVertex[1].u = int(OdRound(s2 * m_texWidth));
      triVertex[1].v = int(OdRound(t2 * m_texHeight));
#else
      triVertex[1].s = s2 * m_texWidth;
      triVertex[1].t = t2 * m_texHeight;
#endif
      triVertex[2].x = x3;
      triVertex[2].y = y3;
#ifdef OD_GSTR_USEFIXEDTEX
      triVertex[2].u = int(OdRound(s3 * m_texWidth));
      triVertex[2].v = int(OdRound(t3 * m_texHeight));
#else
      triVertex[2].s = s3 * m_texWidth;
      triVertex[2].t = t3 * m_texHeight;
#endif
      
      rVertexTextured *pp1 = triVertex, *pp2 = triVertex + 1, *pp3 = triVertex + 2, *temp;
      OD_GSTR_SORTVERTICES

      // Render triangle
      rEdgeTextured eshort, elong;
      int y, h;
      // Long side
      if (edgeTriangleTextured(&elong, pp1, pp3) <= 0)
      {
        if (pp1->y == pp3->y)
        {
#ifdef OD_GSTR_USEFIXEDTEX
          blitTexturedLine(pp1->y, pp1->x, pp3->x, pp1->u, pp1->v, pp3->u, pp3->v, alpha);
#else
          blitTexturedLine(pp1->y, pp1->x, pp3->x, pp1->s, pp1->t, pp3->s, pp3->t, alpha);
#endif
        }
        return;
      }
      // Start y
      y = odmax(pp1->y, m_clipRegion.m_min.y);
      // Top short side
      h = edgeTriangleTextured(&eshort, pp1, pp2);
      if (h > 0)
      {
        if (eshort.dx < elong.dx)
          blitTriangleTextured(&eshort, &elong, y, h, alpha);
        else
          blitTriangleTextured(&elong, &eshort, y, h, alpha);
        y += h;
      }
      else if (pp1->y == pp2->y)
      {
#ifdef OD_GSTR_USEFIXEDTEX
        blitTexturedLine(pp1->y, pp1->x, pp2->x, pp1->u, pp1->v, pp2->u, pp2->v, alpha);
#else
        blitTexturedLine(pp1->y, pp1->x, pp2->x, pp1->s, pp1->t, pp2->s, pp2->t, alpha);
#endif
      }
      // Bottom short side
      h = edgeTriangleTextured(&eshort, pp2, pp3);
      if (h > 0)
      {
        if (eshort.x < elong.x)
          blitTriangleTextured(&eshort, &elong, y, h, alpha);
        else
          blitTriangleTextured(&elong, &eshort, y, h, alpha);
      }
      else if (pp2->y == pp3->y)
      {
#ifdef OD_GSTR_USEFIXEDTEX
        blitTexturedLine(pp2->y, pp2->x, pp3->x, pp2->u, pp2->v, pp3->u, pp3->v, alpha);
#else
        blitTexturedLine(pp2->y, pp2->x, pp3->x, pp2->s, pp2->t, pp3->s, pp3->t, alpha);
#endif
      }
    }
    // Precis render textured triangle
    void renderTriangleTexturedPrec(int x1, int y1, double s1, double t1,
                                    int x2, int y2, double s2, double t2,
                                    int x3, int y3, double s3, double t3, OdUInt8 alpha)
    {
      rVertexTexturedPrec triVertex[3];

      if (regionTriangleClipTest(x1, y1, x2, y2, x3, y3))
      {
        return;
      }

      ODA_ASSERT(m_texWidth > 0 && m_texHeight > 0 && m_texScanLineLength > 0 && m_pTexScanLines != 0);

      triVertex[0].x = x1;
      triVertex[0].y = y1;
      triVertex[0].s = s1 * m_texWidth;
      triVertex[0].t = t1 * m_texHeight;
      triVertex[1].x = x2;
      triVertex[1].y = y2;
      triVertex[1].s = s2 * m_texWidth;
      triVertex[1].t = t2 * m_texHeight;
      triVertex[2].x = x3;
      triVertex[2].y = y3;
      triVertex[2].s = s3 * m_texWidth;
      triVertex[2].t = t3 * m_texHeight;
      
      rVertexTexturedPrec *pp1 = triVertex, *pp2 = triVertex + 1, *pp3 = triVertex + 2, *temp;
      OD_GSTR_SORTVERTICES

      // Render triangle
      rEdgeTexturedPrec eshort, elong;
      int y, h;
      // Long side
      if (edgeTriangleTexturedPrec(&elong, pp1, pp3) <= 0)
      {
        if (pp1->y == pp3->y)
        {
          blitTexturedLine(pp1->y, pp1->x, pp3->x, pp1->s, pp1->t, pp3->s, pp3->t, alpha);
        }
        return;
      }
      // Start y
      y = odmax(pp1->y, m_clipRegion.m_min.y);
      // Top short side
      h = edgeTriangleTexturedPrec(&eshort, pp1, pp2);
      if (h > 0)
      {
        if (eshort.dx < elong.dx)
          blitTriangleTexturedPrec(&eshort, &elong, y, h, alpha);
        else
          blitTriangleTexturedPrec(&elong, &eshort, y, h, alpha);
        y += h;
      }
      else if (pp1->y == pp2->y)
      {
        blitTexturedLine(pp1->y, pp1->x, pp2->x, pp1->s, pp1->t, pp2->s, pp2->t, alpha);
      }
      // Bottom short side
      h = edgeTriangleTexturedPrec(&eshort, pp2, pp3);
      if (h > 0)
      {
        if (eshort.x < elong.x)
          blitTriangleTexturedPrec(&eshort, &elong, y, h, alpha);
        else
          blitTriangleTexturedPrec(&elong, &eshort, y, h, alpha);
      }
      else if (pp2->y == pp3->y)
      {
        blitTexturedLine(pp2->y, pp2->x, pp3->x, pp2->s, pp2->t, pp3->s, pp3->t, alpha);
      }
    }

  protected:
    // Clip flat triangle
    bool clipTriangleFlat(int x1, int y1, int x2, int y2, int x3, int y3, ODCOLORREF color)
    {
      OdUInt32 clipTest, clipState;
      clipState = regionTriangleClipState(x1, y1, x2, y2, x3, y3, &clipTest);
      if (clipTest != 0)
        return true; // Triangle totally clipped by region
      if (clipState == 0)
        return false; // Triangle totally inside region
      int xmin, ymin, xmax, ymax;
      triangleExtents(x1, y1, x2, y2, x3, y3, xmin, ymin, xmax, ymax, 1);
      if ((clipState & OD_GSTR_CLIPLEFT) == OD_GSTR_CLIPLEFT)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, m_clipRegion.m_min.x, ymin, m_clipRegion.m_min.x, ymax,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPLEFT;
          dropSubdividedTriangleFlat(x1, y1, x2, y2, x3, y3, sdInfo, color);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPTOP) == OD_GSTR_CLIPTOP)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, xmin, m_clipRegion.m_min.y, xmax, m_clipRegion.m_min.y,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPTOP;
          dropSubdividedTriangleFlat(x1, y1, x2, y2, x3, y3, sdInfo, color);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPRIGHT) == OD_GSTR_CLIPRIGHT)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, m_clipRegion.m_max.x - 1, ymin, m_clipRegion.m_max.x - 1, ymax,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPRIGHT;
          dropSubdividedTriangleFlat(x1, y1, x2, y2, x3, y3, sdInfo, color);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPBOTTOM) == OD_GSTR_CLIPBOTTOM)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, xmin, m_clipRegion.m_max.y, xmax, m_clipRegion.m_max.y,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPBOTTOM;
          dropSubdividedTriangleFlat(x1, y1, x2, y2, x3, y3, sdInfo, color);
          return true;
        }
      }
      return false;
    }
    // Subdivide flat triangle
    void dropSubdividedTriangleFlat(int x1, int y1, int x2, int y2, int x3, int y3, const rSubdivideInfo &sdInfo,
                                    ODCOLORREF color)
    {
      int eLock = 0;
      if (sdInfo.e1 != 1 && sdInfo.e2 != 1)
        eLock = 1;
      else if (sdInfo.e1 != 2 && sdInfo.e2 != 2)
        eLock = 2;
      int vertCut = (eLock == 0) ? 2 : ((eLock == 1) ? 0 : 1);

      int *vertCutX = (vertCut == 0) ? (&x1) : ((vertCut == 1) ? (&x2) : (&x3));
      int *vertCutY = (vertCut == 0) ? (&y1) : ((vertCut == 1) ? (&y2) : (&y3));

      int cvXInc = 0, cvYInc = 0, cqXInc = 0, cqYInc = 0;
      switch (sdInfo.side)
      {
        case OD_GSTR_CLIPLEFT:
          if (*vertCutX < m_clipRegion.m_min.x)
            cvXInc = -1;
          else
            cqXInc = -1;
        break;
        case OD_GSTR_CLIPTOP:
          if (*vertCutY < m_clipRegion.m_min.y)
            cvYInc = -1;
          else
            cqYInc = -1;
        break;
        case OD_GSTR_CLIPRIGHT:
          if (*vertCutX >= m_clipRegion.m_max.x)
            cvXInc = 1;
          else
            cqXInc = 1;
        break;
        case OD_GSTR_CLIPBOTTOM:
          if (*vertCutY > m_clipRegion.m_max.y)
            cvYInc = 1;
          else
            cqYInc = 1;
        break;
      };

      // Cutted triangle
      renderTriangleFlat(sdInfo.x1 + cvXInc, sdInfo.y1 + cvYInc, *vertCutX, *vertCutY, sdInfo.x2 + cvXInc, sdInfo.y2 + cvYInc, color);
      // Cutted quadrilateral
      switch (eLock)
      {
        case 0:
          renderTriangleFlat(x1, y1, x2, y2, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, color);
          renderTriangleFlat(x1, y1, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, color);
        break;
        case 1:
          renderTriangleFlat(x2, y2, x3, y3, sdInfo.x2, sdInfo.y2, color);
          renderTriangleFlat(x2, y2, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, color);
        break;
        case 2:
          renderTriangleFlat(x3, y3, x1, y1, sdInfo.x1, sdInfo.y1, color);
          renderTriangleFlat(x3, y3, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, color);
      }
    }
    // Process flat triangle edge
    int edgeTriangleFlat(rEdgeFlat *e, rVertexFlat *v1, rVertexFlat *v2)
    {
      int dy, h, adv_y = 0;
      dy = h = v2->y - v1->y;
      if (v1->y < m_clipRegion.m_min.y)
      {
        if (v2->y <= m_clipRegion.m_min.y)
          return 0;
        adv_y = m_clipRegion.m_min.y - v1->y;
        h -= adv_y;
      }
      if (v2->y > m_clipRegion.m_max.y)
      {
        if (v1->y >= m_clipRegion.m_max.y)
          return 0;
        h -= v2->y - m_clipRegion.m_max.y;
      }
      dy = OD_GSTR_INVERSE(dy);
      e->x = v1->x << 16;
      e->dx = (v2->x - v1->x) * dy;
      if (adv_y)
      {
        e->x += e->dx * adv_y;
      }
      return h;
    }
    // Draw half of flat triangle
    void blitTriangleFlat(rEdgeFlat *e1, rEdgeFlat *e2, int y, int h, OdUInt8 r, OdUInt8 g, OdUInt8 b, OdUInt8 a) {
      int x1, x2, dx, count;
      OdUInt8 *stack, *sstack;
      OdUInt8 *drawer = m_pScanLines;
      OdUInt8 *stencil = m_pnrcClip;
      drawer += (m_height - 1 - y) * m_scanLineLength;
      if (stencil) stencil += (m_height - 1 - y - m_nrcOffsetY) * m_nrcWidth;
      while (h > 0)
      {
        x1 = e1->x >> 16; x2 = e2->x >> 16;
        x2++;
        dx = x2 - x1;
        // Degraded triangles rendering fix
        if (dx == 1)
        {
          if (e2->dx < 0)
          {
            if (dx < -(e2->dx >> 16))
            {
              if (((e1->x + e1->dx) >> 16) == (e2->x + e2->dx) >> 16)
                x1 = ((e1->x + e1->dx) >> 16) + 1;
            }
          }
          else
          {
            if (dx < (e2->dx >> 16))
            {
              if (((e1->x + e1->dx) >> 16) == (e2->x + e2->dx) >> 16)
                x2 = (e2->x + e2->dx) >> 16;
            }
          }
        }
        //
        if (x2 > m_clipRegion.m_max.x)
        {
          x2 = m_clipRegion.m_max.x;
        }
        if (x1 < m_clipRegion.m_min.x)
        {
          x1 = m_clipRegion.m_min.x;
        }
        if (x1 >= x2)
        {
          goto NEXT_SPAN;
        }
        if (!stencil)
        {
          stack = drawer;
          drawer += x1 * 3;
          count = x2 - x1;
          if (a == 255)
          {
            while (count > 0)
            {
              *(drawer++) = b;
              *(drawer++) = g;
              *(drawer++) = r;
              count--;
            }
          }
          else if (a != 0)
          {
            while (count > 0)
            {
              OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], r, g, b, a, drawer[2], drawer[1], drawer[0]);
              drawer += 3;
              count--;
            }
          }
          drawer = stack;
        }
        else
        {
          stack = drawer;
          sstack = stencil;
          drawer += x1 * 3;
          stencil += x1 - m_nrcOffsetX;
          count = x2 - x1;
          if (a == 255)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                *(drawer++) = b;
                *(drawer++) = g;
                *(drawer++) = r;
              }
              else
              {
                drawer += 3;
              }
              stencil++;
              count--;
            }
          }
          else if (a != 0)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], r, g, b, a, drawer[2], drawer[1], drawer[0]);
              }
              drawer += 3;
              stencil++;
              count--;
            }
          }
          drawer = stack;
          stencil = sstack;
        }
NEXT_SPAN:
        e1->x += e1->dx;
        e2->x += e2->dx;
        drawer -= m_scanLineLength;
        if (stencil) stencil -= m_nrcWidth;
        h--;
      }
    }
    // Blit degrade line
    void blitFlatLine(int y, int x1, int x2, OdUInt8 r, OdUInt8 g, OdUInt8 b, OdUInt8 a)
    {
      if (y < m_clipRegion.m_min.y || y >= m_clipRegion.m_max.y)
        return;
      if (x1 > x2)
        x1 ^= x2 ^= x1 ^= x2;
      rEdgeFlat e1, e2;
      e1.x = x1 << 16;
      e2.x = x2 << 16;
      e1.dx = e2.dx = 0;
      blitTriangleFlat(&e1, &e2, y, 1, r, g, b, a);
    }
    // Process precis flat triangle edge
    int edgeTriangleFlatPrec(rEdgeFlatPrec *e, rVertexFlat *v1, rVertexFlat *v2)
    {
      int dy, h, adv_y = 0;
      double fdy;
      dy = h = v2->y - v1->y;
      if (v1->y < m_clipRegion.m_min.y)
      {
        if (v2->y <= m_clipRegion.m_min.y)
          return 0;
        adv_y = m_clipRegion.m_min.y - v1->y;
        h -= adv_y;
      }
      if (v2->y > m_clipRegion.m_max.y)
      {
        if (v1->y >= m_clipRegion.m_max.y)
          return 0;
        h -= v2->y - m_clipRegion.m_max.y;
      }
      fdy = (dy) ? (1.0 / dy) : 0.0;
      e->x = (double)v1->x;
      e->dx = (v2->x - v1->x) * fdy;
      if (adv_y)
      {
        e->x += e->dx * adv_y;
      }
      return h;
    }
    // Draw half of flat triangle
    void blitTriangleFlatPrec(rEdgeFlatPrec *e1, rEdgeFlatPrec *e2, int y, int h, OdUInt8 r, OdUInt8 g, OdUInt8 b, OdUInt8 a) {
      int x1, x2, dx, count;
      OdUInt8 *stack, *sstack;
      OdUInt8 *drawer = m_pScanLines;
      OdUInt8 *stencil = m_pnrcClip;
      drawer += (m_height - 1 - y) * m_scanLineLength;
      if (stencil) stencil += (m_height - 1 - y - m_nrcOffsetY) * m_nrcWidth;
      while (h > 0)
      {
        x1 = (int)OdRound(e1->x); x2 = (int)OdRound(e2->x);
        // @@@TODO: 2) this is should be computed from doubles input. Increment only if length between points >= 0.5.
        x2++;
        dx = x2 - x1;
        if (x2 > m_clipRegion.m_max.x)
        {
          x2 = m_clipRegion.m_max.x;
        }
        if (x1 < m_clipRegion.m_min.x)
        {
          x1 = m_clipRegion.m_min.x;
        }
        if (x1 >= x2)
        {
          goto NEXT_SPAN;
        }
        if (!stencil)
        {
          stack = drawer;
          drawer += x1 * 3;
          count = x2 - x1;
          if (a == 255)
          {
            while (count > 0)
            {
              *(drawer++) = b;
              *(drawer++) = g;
              *(drawer++) = r;
              count--;
            }
          }
          else if (a != 0)
          {
            while (count > 0)
            {
              OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], r, g, b, a, drawer[2], drawer[1], drawer[0]);
              drawer += 3;
              count--;
            }
          }
          drawer = stack;
        }
        else
        {
          stack = drawer;
          sstack = stencil;
          drawer += x1 * 3;
          stencil += x1 - m_nrcOffsetX;
          count = x2 - x1;
          if (a == 255)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                *(drawer++) = b;
                *(drawer++) = g;
                *(drawer++) = r;
              }
              else
              {
                drawer += 3;
              }
              stencil++;
              count--;
            }
          }
          else if (a != 0)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], r, g, b, a, drawer[2], drawer[1], drawer[0]);
              }
              drawer += 3;
              stencil++;
              count--;
            }
          }
          drawer = stack;
          stencil = sstack;
        }
NEXT_SPAN:
        e1->x += e1->dx;
        e2->x += e2->dx;
        drawer -= m_scanLineLength;
        if (stencil) stencil -= m_nrcWidth;
        h--;
      }
    }
    // Clip gouraud triangle
    bool clipTriangleGouraud(int x1, int y1, ODCOLORREF color1,
                             int x2, int y2, ODCOLORREF color2,
                             int x3, int y3, ODCOLORREF color3, OdUInt8 alpha)
    {
      OdUInt32 clipTest, clipState;
      clipState = regionTriangleClipState(x1, y1, x2, y2, x3, y3, &clipTest);
      if (clipTest != 0)
        return true; // Triangle totally clipped by region
      if (clipState == 0)
        return false; // Triangle totally inside region
      int xmin, ymin, xmax, ymax;
      triangleExtents(x1, y1, x2, y2, x3, y3, xmin, ymin, xmax, ymax, 1);
      if ((clipState & OD_GSTR_CLIPLEFT) == OD_GSTR_CLIPLEFT)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, m_clipRegion.m_min.x, ymin, m_clipRegion.m_min.x, ymax,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPLEFT;
          dropSubdividedTriangleGouraud(x1, y1, color1, x2, y2, color2, x3, y3, color3, alpha, sdInfo);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPTOP) == OD_GSTR_CLIPTOP)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, xmin, m_clipRegion.m_min.y, xmax, m_clipRegion.m_min.y,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPTOP;
          dropSubdividedTriangleGouraud(x1, y1, color1, x2, y2, color2, x3, y3, color3, alpha, sdInfo);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPRIGHT) == OD_GSTR_CLIPRIGHT)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, m_clipRegion.m_max.x - 1, ymin, m_clipRegion.m_max.x - 1, ymax,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPRIGHT;
          dropSubdividedTriangleGouraud(x1, y1, color1, x2, y2, color2, x3, y3, color3, alpha, sdInfo);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPBOTTOM) == OD_GSTR_CLIPBOTTOM)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, xmin, m_clipRegion.m_max.y, xmax, m_clipRegion.m_max.y,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPBOTTOM;
          dropSubdividedTriangleGouraud(x1, y1, color1, x2, y2, color2, x3, y3, color3, alpha, sdInfo);
          return true;
        }
      }
      return false;
    }
    ODCOLORREF interpolateGouraudColor(int x1, int y1, ODCOLORREF color1, int x2, int y2, ODCOLORREF color2,
                                       int x, int y)
    {
      int dx = x2 - x1, dy = y2 - y1;
      double l = sqrt(double(dx * dx) + double(dy * dy));
      if (OdZero(l))
        return color1;
      dx = x - x1, dy = y - y1;
      double t = sqrt(double(dx * dx) + double(dy * dy)) / l;
      return ODRGB(ODGETRED(color1) + OdInt32((ODGETRED(color2) - ODGETRED(color1)) * t),
                   ODGETGREEN(color1) + OdInt32((ODGETGREEN(color2) - ODGETGREEN(color1)) * t),
                   ODGETBLUE(color1) + OdInt32((ODGETBLUE(color2) - ODGETBLUE(color1)) * t));
    }
    // Subdivide gouraud triangle
    void dropSubdividedTriangleGouraud(int x1, int y1, ODCOLORREF color1, int x2, int y2, ODCOLORREF color2,
                                       int x3, int y3, ODCOLORREF color3, OdUInt8 alpha, const rSubdivideInfo &sdInfo)
    {
      int eLock = 0;
      if (sdInfo.e1 != 1 && sdInfo.e2 != 1)
        eLock = 1;
      else if (sdInfo.e1 != 2 && sdInfo.e2 != 2)
        eLock = 2;
      int vertCut = (eLock == 0) ? 2 : ((eLock == 1) ? 0 : 1);

      int *vertCutX = (vertCut == 0) ? (&x1) : ((vertCut == 1) ? (&x2) : (&x3));
      int *vertCutY = (vertCut == 0) ? (&y1) : ((vertCut == 1) ? (&y2) : (&y3));
      ODCOLORREF *vertCutC = (vertCut == 0) ? (&color1) : ((vertCut == 1) ? (&color2) : (&color3));

      ODCOLORREF colorSD1 = 0, colorSD2 = 0;
      switch (vertCut)
      {
        case 0:
          colorSD1 = interpolateGouraudColor(*vertCutX, *vertCutY, *vertCutC, x2, y2, color2, sdInfo.x1, sdInfo.y1);
          colorSD2 = interpolateGouraudColor(*vertCutX, *vertCutY, *vertCutC, x3, y3, color3, sdInfo.x2, sdInfo.y2);
        break;
        case 1:
          colorSD1 = interpolateGouraudColor(*vertCutX, *vertCutY, *vertCutC, x1, y1, color1, sdInfo.x1, sdInfo.y1);
          colorSD2 = interpolateGouraudColor(*vertCutX, *vertCutY, *vertCutC, x3, y3, color3, sdInfo.x2, sdInfo.y2);
        break;
        case 2:
          colorSD1 = interpolateGouraudColor(*vertCutX, *vertCutY, *vertCutC, x2, y2, color2, sdInfo.x1, sdInfo.y1);
          colorSD2 = interpolateGouraudColor(*vertCutX, *vertCutY, *vertCutC, x1, y1, color1, sdInfo.x2, sdInfo.y2);
      };

      int cvXInc = 0, cvYInc = 0, cqXInc = 0, cqYInc = 0;
      switch (sdInfo.side)
      {
        case OD_GSTR_CLIPLEFT:
          if (*vertCutX < m_clipRegion.m_min.x)
            cvXInc = -1;
          else
            cqXInc = -1;
        break;
        case OD_GSTR_CLIPTOP:
          if (*vertCutY < m_clipRegion.m_min.y)
            cvYInc = -1;
          else
            cqYInc = -1;
        break;
        case OD_GSTR_CLIPRIGHT:
          if (*vertCutX >= m_clipRegion.m_max.x)
            cvXInc = 1;
          else
            cqXInc = 1;
        break;
        case OD_GSTR_CLIPBOTTOM:
          if (*vertCutY > m_clipRegion.m_max.y)
            cvYInc = 1;
          else
            cqYInc = 1;
        break;
      };

      // Cutted triangle
      renderTriangleGouraud(sdInfo.x1 + cvXInc, sdInfo.y1 + cvYInc, colorSD1,
                            *vertCutX, *vertCutY, *vertCutC,
                            sdInfo.x2 + cvXInc, sdInfo.y2 + cvYInc, colorSD2, alpha);
      // Cutted quadrilateral
      switch (eLock)
      {
        case 0:
          renderTriangleGouraud(x1, y1, color1, x2, y2, color2, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, colorSD1, alpha);
          renderTriangleGouraud(x1, y1, color1, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, colorSD1, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, colorSD2, alpha);
        break;
        case 1:
          renderTriangleGouraud(x2, y2, color2, x3, y3, color3, sdInfo.x2, sdInfo.y2, colorSD2, alpha);
          renderTriangleGouraud(x2, y2, color2, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, colorSD2, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, colorSD1, alpha);
        break;
        case 2:
          renderTriangleGouraud(x3, y3, color3, x1, y1, color1, sdInfo.x1, sdInfo.y1, colorSD1, alpha);
          renderTriangleGouraud(x3, y3, color3, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, colorSD1, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, colorSD2, alpha);
      }
    }
    // Process gouraud triangle edge
    int edgeTriangleGouraud(rEdgeGouraud *e, rVertexGouraud *v1, rVertexGouraud *v2)
    {
      int dy, h, adv_y = 0;
      dy = h = v2->y - v1->y;
      if (v1->y < m_clipRegion.m_min.y)
      {
        if (v2->y <= m_clipRegion.m_min.y)
          return 0;
        adv_y = m_clipRegion.m_min.y - v1->y;
        h -= adv_y;
      }
      if (v2->y > m_clipRegion.m_max.y)
      {
        if (v1->y >= m_clipRegion.m_max.y)
          return 0;
        h -= v2->y - m_clipRegion.m_max.y;
      }
      dy = OD_GSTR_INVERSE(dy);
      e->x = v1->x << 16;
      e->r = v1->r << 16;
      e->g = v1->g << 16;
      e->b = v1->b << 16;
      e->dx = (v2->x - v1->x) * dy;
      e->dr = (v2->r - v1->r) * dy;
      e->dg = (v2->g - v1->g) * dy;
      e->db = (v2->b - v1->b) * dy;
      if (adv_y)
      {
        e->x += e->dx * adv_y;
        e->r += e->dr * adv_y;
        e->g += e->dg * adv_y;
        e->b += e->db * adv_y;
      }
      return h;
    }
    // Draw half of gouraud triangle
    void blitTriangleGouraud(rEdgeGouraud *e1, rEdgeGouraud *e2, int y, int h, OdUInt8 alpha) {
      int x1, x2, dx, count, x_adv;
      int r, dr, g, dg, b, db;
      OdUInt8 *stack, *sstack;
      OdUInt8 *drawer = m_pScanLines;
      OdUInt8 *stencil = m_pnrcClip;
      drawer += (m_height - 1 - y) * m_scanLineLength;
      if (stencil) stencil += (m_height - 1 - y - m_nrcOffsetY) * m_nrcWidth;
      while (h > 0)
      {
        x1 = e1->x >> 16; x2 = e2->x >> 16;
        x2++;
        dx = x2 - x1;
        if (x2 > m_clipRegion.m_max.x)
        {
          x2 = m_clipRegion.m_max.x;
        }
        if (x1 < m_clipRegion.m_min.x)
        {
          x_adv = m_clipRegion.m_min.x - x1;
          x1 = m_clipRegion.m_min.x;
        }
        else
        {
          x_adv = 0;
        }
        if (x1 >= x2)
        {
          goto NEXT_SPAN;
        }
        dx = OD_GSTR_INVERSE(dx);
        dr = OD_GSTR_FIXMUL(e2->r - e1->r, dx);
        dg = OD_GSTR_FIXMUL(e2->g - e1->g, dx);
        db = OD_GSTR_FIXMUL(e2->b - e1->b, dx);
        r = e1->r; g = e1->g; b = e1->b;
        if (x_adv)
        {
          r += dr * x_adv;
          g += dg * x_adv;
          b += db * x_adv;
        }
        if (!stencil)
        {
          stack = drawer;
          drawer += x1 * 3;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
              *(drawer++) = OdUInt8(b >> 16);
              *(drawer++) = OdUInt8(g >> 16);
              *(drawer++) = OdUInt8(r >> 16);
              count--;
              r += dr; g += dg; b += db;
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
              OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], OdUInt8(r >> 16), OdUInt8(g >> 16), OdUInt8(b >> 16), alpha, drawer[2], drawer[1], drawer[0]);
              drawer += 3;
              count--;
              r += dr; g += dg; b += db;
            }
          }
          drawer = stack;
        }
        else
        {
          stack = drawer;
          sstack = stencil;
          drawer += x1 * 3;
          stencil += x1 - m_nrcOffsetX;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                *(drawer++) = OdUInt8(b >> 16);
                *(drawer++) = OdUInt8(g >> 16);
                *(drawer++) = OdUInt8(r >> 16);
              }
              else
              {
                drawer += 3;
              }
              stencil++;
              count--;
              r += dr; g += dg; b += db;
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], OdUInt8(r >> 16), OdUInt8(g >> 16), OdUInt8(b >> 16), alpha, drawer[2], drawer[1], drawer[0]);
              }
              drawer += 3;
              stencil++;
              count--;
              r += dr; g += dg; b += db;
            }
          }
          drawer = stack;
          stencil = sstack;
        }
NEXT_SPAN:
        e1->x += e1->dx;
        e1->r += e1->dr;
        e1->g += e1->dg;
        e1->b += e1->db;
        e2->x += e2->dx;
        e2->r += e2->dr;
        e2->g += e2->dg;
        e2->b += e2->db;
        drawer -= m_scanLineLength;
        if (stencil) stencil -= m_nrcWidth;
        h--;
      }
    }
    // Blit degrade line
    void blitGouraudLine(int y, int x1, int x2, int r1, int g1, int b1, int r2, int g2, int b2, OdUInt8 alpha)
    {
      if (y < m_clipRegion.m_min.y || y >= m_clipRegion.m_max.y)
        return;
      if (x1 > x2)
      {
        x1 ^= x2 ^= x1 ^= x2;
        r1 ^= r2 ^= r1 ^= r2;
        g1 ^= g2 ^= g1 ^= g2;
        b1 ^= b2 ^= b1 ^= b2;
      }
      rEdgeGouraud e1, e2;
      e1.x = x1 << 16;
      e1.r = r1 << 16;
      e1.g = g1 << 16;
      e1.b = b1 << 16;
      e2.x = x2 << 16;
      e2.r = r2 << 16;
      e2.g = g2 << 16;
      e2.b = b2 << 16;
      e1.dx = e2.dx = e1.dr = e2.dr = e1.dg = e2.dg = e1.db = e2.db = 0;
      blitTriangleGouraud(&e1, &e2, y, 1, alpha);
    }
    // Process precis gouraud triangle edge
    int edgeTriangleGouraudPrec(rEdgeGouraudPrec *e, rVertexGouraud *v1, rVertexGouraud *v2)
    {
      int dy, h, adv_y = 0;
      double fdy;
      dy = h = v2->y - v1->y;
      if (v1->y < m_clipRegion.m_min.y)
      {
        if (v2->y <= m_clipRegion.m_min.y)
          return 0;
        adv_y = m_clipRegion.m_min.y - v1->y;
        h -= adv_y;
      }
      if (v2->y > m_clipRegion.m_max.y)
      {
        if (v1->y >= m_clipRegion.m_max.y)
          return 0;
        h -= v2->y - m_clipRegion.m_max.y;
      }
      fdy = (dy) ? (1.0 / dy) : 0.0;
      e->x = (double)v1->x;
      e->r = (double)v1->r;
      e->g = (double)v1->g;
      e->b = (double)v1->b;
      e->dx = (v2->x - v1->x) * fdy;
      e->dr = (v2->r - v1->r) * fdy;
      e->dg = (v2->g - v1->g) * fdy;
      e->db = (v2->b - v1->b) * fdy;
      if (adv_y)
      {
        e->x += e->dx * adv_y;
        e->r += e->dr * adv_y;
        e->g += e->dg * adv_y;
        e->b += e->db * adv_y;
      }
      return h;
    }
    // Draw half of precis gouraud triangle
    void blitTriangleGouraudPrec(rEdgeGouraudPrec *e1, rEdgeGouraudPrec *e2, int y, int h, OdUInt8 alpha) {
      int x1, x2, dx, count, x_adv;
      double fdx;
      double r, dr, g, dg, b, db;
      OdUInt8 *stack, *sstack;
      OdUInt8 *drawer = m_pScanLines;
      OdUInt8 *stencil = m_pnrcClip;
      drawer += (m_height - 1 - y) * m_scanLineLength;
      if (stencil) stencil += (m_height - 1 - y - m_nrcOffsetY) * m_nrcWidth;
      while (h > 0)
      {
        x1 = (int)OdRound(e1->x); x2 = (int)OdRound(e2->x);
        x2++;
        dx = x2 - x1;
        if (x2 > m_clipRegion.m_max.x)
        {
          x2 = m_clipRegion.m_max.x;
        }
        if (x1 < m_clipRegion.m_min.x)
        {
          x_adv = m_clipRegion.m_min.x - x1;
          x1 = m_clipRegion.m_min.x;
        }
        else
        {
          x_adv = 0;
        }
        if (x1 >= x2)
        {
          goto NEXT_SPAN;
        }
        fdx = (dx) ? (1.0 / dx) : 0.0;
        dr = (e2->r - e1->r) * fdx;
        dg = (e2->g - e1->g) * fdx;
        db = (e2->b - e1->b) * fdx;
        r = e1->r; g = e1->g; b = e1->b;
        if (x_adv)
        {
          r += dr * x_adv;
          g += dg * x_adv;
          b += db * x_adv;
        }
        if (!stencil)
        {
          stack = drawer;
          drawer += x1 * 3;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
              *(drawer++) = OdUInt8(b);
              *(drawer++) = OdUInt8(g);
              *(drawer++) = OdUInt8(r);
              count--;
              r += dr; g += dg; b += db;
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
              OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], OdUInt8(r), OdUInt8(g), OdUInt8(b), alpha, drawer[2], drawer[1], drawer[0]);
              drawer += 3;
              count--;
              r += dr; g += dg; b += db;
            }
          }
          drawer = stack;
        }
        else
        {
          stack = drawer;
          sstack = stencil;
          drawer += x1 * 3;
          stencil += x1 - m_nrcOffsetX;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                *(drawer++) = OdUInt8(b);
                *(drawer++) = OdUInt8(g);
                *(drawer++) = OdUInt8(r);
              }
              else
              {
                drawer += 3;
              }
              stencil++;
              count--;
              r += dr; g += dg; b += db;
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                OdMergeRGBAlpha(drawer[2], drawer[1], drawer[0], OdUInt8(r), OdUInt8(g), OdUInt8(b), alpha, drawer[2], drawer[1], drawer[0]);
              }
              drawer += 3;
              stencil++;
              count--;
              r += dr; g += dg; b += db;
            }
          }
          drawer = stack;
          stencil = sstack;
        }
NEXT_SPAN:
        e1->x += e1->dx;
        e1->r += e1->dr;
        e1->g += e1->dg;
        e1->b += e1->db;
        e2->x += e2->dx;
        e2->r += e2->dr;
        e2->g += e2->dg;
        e2->b += e2->db;
        drawer -= m_scanLineLength;
        if (stencil) stencil -= m_nrcWidth;
        h--;
      }
    }
    // Clip textured triangle
    bool clipTriangleTextured(int x1, int y1, double s1, double t1,
                              int x2, int y2, double s2, double t2,
                              int x3, int y3, double s3, double t3, OdUInt8 alpha)
    {
      OdUInt32 clipTest, clipState;
      clipState = regionTriangleClipState(x1, y1, x2, y2, x3, y3, &clipTest);
      if (clipTest != 0)
        return true; // Triangle totally clipped by region
      if (clipState == 0)
        return false; // Triangle totally inside region
      int xmin, ymin, xmax, ymax;
      triangleExtents(x1, y1, x2, y2, x3, y3, xmin, ymin, xmax, ymax, 1);
      if ((clipState & OD_GSTR_CLIPLEFT) == OD_GSTR_CLIPLEFT)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, m_clipRegion.m_min.x, ymin, m_clipRegion.m_min.x, ymax,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPLEFT;
          dropSubdividedTriangleTextured(x1, y1, s1, t1, x2, y2, s2, t2, x3, y3, s3, t3, alpha, sdInfo);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPTOP) == OD_GSTR_CLIPTOP)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, xmin, m_clipRegion.m_min.y, xmax, m_clipRegion.m_min.y,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPTOP;
          dropSubdividedTriangleTextured(x1, y1, s1, t1, x2, y2, s2, t2, x3, y3, s3, t3, alpha, sdInfo);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPRIGHT) == OD_GSTR_CLIPRIGHT)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, m_clipRegion.m_max.x - 1, ymin, m_clipRegion.m_max.x - 1, ymax,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPRIGHT;
          dropSubdividedTriangleTextured(x1, y1, s1, t1, x2, y2, s2, t2, x3, y3, s3, t3, alpha, sdInfo);
          return true;
        }
      }
      if ((clipState & OD_GSTR_CLIPBOTTOM) == OD_GSTR_CLIPBOTTOM)
      {
        rSubdivideInfo sdInfo;
        if (subdivideTriangle(x1, y1, x2, y2, x3, y3, xmin, m_clipRegion.m_max.y, xmax, m_clipRegion.m_max.y,
          sdInfo.x1, sdInfo.y1, sdInfo.x2, sdInfo.y2, sdInfo.e1, sdInfo.e2))
        {
          sdInfo.side = OD_GSTR_CLIPBOTTOM;
          dropSubdividedTriangleTextured(x1, y1, s1, t1, x2, y2, s2, t2, x3, y3, s3, t3, alpha, sdInfo);
          return true;
        }
      }
      return false;
    }
    void interpolateTextureCoord(int x1, int y1, double s1, double t1, int x2, int y2, double s2, double t2,
                                 int x, int y, double &so, double &to)
    {
      int dx = x2 - x1, dy = y2 - y1;
      double l = sqrt(double(dx * dx) + double(dy * dy));
      if (OdZero(l))
      {
        so = s1;
        to = t1;
        return;
      }
      dx = x - x1, dy = y - y1;
      double t = sqrt(double(dx * dx) + double(dy * dy)) / l;
      so = s1 + (s2 - s1) * t;
      to = t1 + (t2 - t1) * t;
    }
    // Subdivide gouraud triangle
    void dropSubdividedTriangleTextured(int x1, int y1, double s1, double t1, int x2, int y2, double s2, double t2,
                                        int x3, int y3, double s3, double t3, OdUInt8 alpha, const rSubdivideInfo &sdInfo)
    {
      int eLock = 0;
      if (sdInfo.e1 != 1 && sdInfo.e2 != 1)
        eLock = 1;
      else if (sdInfo.e1 != 2 && sdInfo.e2 != 2)
        eLock = 2;
      int vertCut = (eLock == 0) ? 2 : ((eLock == 1) ? 0 : 1);

      int *vertCutX = (vertCut == 0) ? (&x1) : ((vertCut == 1) ? (&x2) : (&x3));
      int *vertCutY = (vertCut == 0) ? (&y1) : ((vertCut == 1) ? (&y2) : (&y3));
      double *vertCutS = (vertCut == 0) ? (&s1) : ((vertCut == 1) ? (&s2) : (&s3));
      double *vertCutT = (vertCut == 0) ? (&t1) : ((vertCut == 1) ? (&t2) : (&t3));

      double sSD1 = 0.0, tSD1 = 0.0, sSD2 = 0.0, tSD2 = 0.0;
      switch (vertCut)
      {
        case 0:
          interpolateTextureCoord(*vertCutX, *vertCutY, *vertCutS, *vertCutT, x2, y2, s2, t2, sdInfo.x1, sdInfo.y1, sSD1, tSD1);
          interpolateTextureCoord(*vertCutX, *vertCutY, *vertCutS, *vertCutT, x3, y3, s3, t3, sdInfo.x2, sdInfo.y2, sSD2, tSD2);
        break;
        case 1:
          interpolateTextureCoord(*vertCutX, *vertCutY, *vertCutS, *vertCutT, x1, y1, s1, t1, sdInfo.x1, sdInfo.y1, sSD1, tSD1);
          interpolateTextureCoord(*vertCutX, *vertCutY, *vertCutS, *vertCutT, x3, y3, s3, t3, sdInfo.x2, sdInfo.y2, sSD2, tSD2);
        break;
        case 2:
          interpolateTextureCoord(*vertCutX, *vertCutY, *vertCutS, *vertCutT, x2, y2, s2, t2, sdInfo.x1, sdInfo.y1, sSD1, tSD1);
          interpolateTextureCoord(*vertCutX, *vertCutY, *vertCutS, *vertCutT, x1, y1, s1, t1, sdInfo.x2, sdInfo.y2, sSD2, tSD2);
      };

      int cvXInc = 0, cvYInc = 0, cqXInc = 0, cqYInc = 0;
      switch (sdInfo.side)
      {
        case OD_GSTR_CLIPLEFT:
          if (*vertCutX < m_clipRegion.m_min.x)
            cvXInc = -1;
          else
            cqXInc = -1;
        break;
        case OD_GSTR_CLIPTOP:
          if (*vertCutY < m_clipRegion.m_min.y)
            cvYInc = -1;
          else
            cqYInc = -1;
        break;
        case OD_GSTR_CLIPRIGHT:
          if (*vertCutX >= m_clipRegion.m_max.x)
            cvXInc = 1;
          else
            cqXInc = 1;
        break;
        case OD_GSTR_CLIPBOTTOM:
          if (*vertCutY > m_clipRegion.m_max.y)
            cvYInc = 1;
          else
            cqYInc = 1;
        break;
      };

      // Cutted triangle
      renderTriangleTextured(sdInfo.x1 + cvXInc, sdInfo.y1 + cvYInc, sSD1, tSD1,
                             *vertCutX, *vertCutY, *vertCutS, *vertCutT,
                             sdInfo.x2 + cvXInc, sdInfo.y2 + cvYInc, sSD2, tSD2, alpha);
      // Cutted quadrilateral
      switch (eLock)
      {
        case 0:
          renderTriangleTextured(x1, y1, s1, t1, x2, y2, s2, t2, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, sSD1, tSD1, alpha);
          renderTriangleTextured(x1, y1, s1, t1, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, sSD1, tSD1, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, sSD2, tSD2, alpha);
        break;
        case 1:
          renderTriangleTextured(x2, y2, s2, t2, x3, y3, s3, t3, sdInfo.x2, sdInfo.y2, sSD2, tSD2, alpha);
          renderTriangleTextured(x2, y2, s2, t2, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, sSD2, tSD2, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, sSD1, tSD1, alpha);
        break;
        case 2:
          renderTriangleTextured(x3, y3, s3, t3, x1, y1, s1, t1, sdInfo.x1, sdInfo.y1, sSD1, tSD1, alpha);
          renderTriangleTextured(x3, y3, s3, t3, sdInfo.x1 + cqXInc, sdInfo.y1 + cqYInc, sSD1, tSD1, sdInfo.x2 + cqXInc, sdInfo.y2 + cqYInc, sSD2, tSD2, alpha);
      }
    }
    // Process textured triangle edge
    int edgeTriangleTextured(rEdgeTextured *e, rVertexTextured *v1, rVertexTextured *v2)
    {
      int dy, h, adv_y = 0;
      dy = h = v2->y - v1->y;
      if (v1->y < m_clipRegion.m_min.y)
      {
        if (v2->y <= m_clipRegion.m_min.y)
          return 0;
        adv_y = m_clipRegion.m_min.y - v1->y;
        h -= adv_y;
      }
      if (v2->y > m_clipRegion.m_max.y)
      {
        if (v1->y >= m_clipRegion.m_max.y)
          return 0;
        h -= v2->y - m_clipRegion.m_max.y;
      }
      dy = OD_GSTR_INVERSE(dy);
      e->x = v1->x << 16;
#ifdef OD_GSTR_USEFIXEDTEX
      e->u = v1->u << 16;
      e->v = v1->v << 16;
#else
      e->s = v1->s;
      e->t = v1->t;
#endif
      e->dx = (v2->x - v1->x) * dy;
#ifdef OD_GSTR_USEFIXEDTEX
      e->du = (v2->u - v1->u) * dy;
      e->dv = (v2->v - v1->v) * dy;
#else
      if (v2->y - v1->y != 0)
      {
        e->ds = (v2->s - v1->s) / (v2->y - v1->y);
        e->dt = (v2->t - v1->t) / (v2->y - v1->y);
      }
      else
      {
        e->ds = e->dt = 0.0;
      }
#endif
      if (adv_y)
      {
        e->x += e->dx * adv_y;
#ifdef OD_GSTR_USEFIXEDTEX
        e->u += e->du * adv_y;
        e->v += e->dv * adv_y;
#else
        e->s += e->ds * adv_y;
        e->t += e->dt * adv_y;
#endif
      }
      return h;
    }
    // Draw half of textured triangle
    void blitTriangleTextured(rEdgeTextured *e1, rEdgeTextured *e2, int y, int h, OdUInt8 alpha) {
      int x1, x2, dx, count, x_adv;
#ifdef OD_GSTR_USEFIXEDTEX
      int u, du, v, dv;
#else
      double s, ds, t, dt;
#endif
      OdUInt8 *stack, *sstack;
      OdUInt8 *drawer = m_pScanLines;
      OdUInt8 *stencil = m_pnrcClip;
      drawer += (m_height - 1 - y) * m_scanLineLength;
      if (stencil) stencil += (m_height - 1 - y - m_nrcOffsetY) * m_nrcWidth;
      while (h > 0)
      {
        x1 = e1->x >> 16; x2 = e2->x >> 16;
        x2++;
        dx = x2 - x1;
        if (x2 > m_clipRegion.m_max.x)
        {
          x2 = m_clipRegion.m_max.x;
        }
        if (x1 < m_clipRegion.m_min.x)
        {
          x_adv = m_clipRegion.m_min.x - x1;
          x1 = m_clipRegion.m_min.x;
        }
        else
        {
          x_adv = 0;
        }
        if (x1 >= x2)
        {
          goto NEXT_SPAN;
        }
#ifndef OD_GSTR_USEFIXEDTEX
        ds = (e2->s - e1->s) / dx;
        dt = (e2->t - e1->t) / dx;
#endif
        dx = OD_GSTR_INVERSE(dx);
#ifdef OD_GSTR_USEFIXEDTEX
        du = OD_GSTR_FIXMUL(e2->u - e1->u, dx);
        dv = OD_GSTR_FIXMUL(e2->v - e1->v, dx);
        u = e1->u; v = e1->v;
#else
        s = e1->s; t = e1->t;
#endif
        if (x_adv)
        {
#ifdef OD_GSTR_USEFIXEDTEX
          u += du * x_adv;
          v += dv * x_adv;
#else
          s += ds * x_adv;
          t += dt * x_adv;
#endif
        }
        if (!stencil)
        {
          stack = drawer;
          drawer += x1 * 3;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
#ifdef OD_GSTR_USEFIXEDTEX
              getTexturePixel(u >> 16, v >> 16, drawer);
#else
              getTexturePixel((int)OdRound(s), (int)OdRound(t), drawer);
#endif
              count--;
              drawer += 3;
#ifdef OD_GSTR_USEFIXEDTEX
              u += du; v += dv;
#else
              s += ds; t += dt;
#endif
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
#ifdef OD_GSTR_USEFIXEDTEX
              getTexturePixelAlpha(u >> 16, v >> 16, drawer, alpha);
#else
              getTexturePixelAlpha((int)OdRound(s), (int)OdRound(t), drawer, alpha);
#endif
              count--;
              drawer += 3;
#ifdef OD_GSTR_USEFIXEDTEX
              u += du; v += dv;
#else
              s += ds; t += dt;
#endif
            }
          }
          drawer = stack;
        }
        else
        {
          stack = drawer;
          sstack = stencil;
          drawer += x1 * 3;
          stencil += x1 - m_nrcOffsetX;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
              if (*stencil)
              {
#ifdef OD_GSTR_USEFIXEDTEX
                getTexturePixel(u >> 16, v >> 16, drawer);
#else
                getTexturePixel((int)OdRound(s), (int)OdRound(t), drawer);
#endif
              }
              stencil++;
              count--;
              drawer += 3;
#ifdef OD_GSTR_USEFIXEDTEX
              u += du; v += dv;
#else
              s += ds; t += dt;
#endif
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
              if (*stencil)
              {
#ifdef OD_GSTR_USEFIXEDTEX
                getTexturePixelAlpha(u >> 16, v >> 16, drawer, alpha);
#else
                getTexturePixelAlpha((int)OdRound(s), (int)OdRound(t), drawer, alpha);
#endif
              }
              stencil++;
              count--;
              drawer += 3;
#ifdef OD_GSTR_USEFIXEDTEX
              u += du; v += dv;
#else
              s += ds; t += dt;
#endif
            }
          }
          drawer = stack;
          stencil = sstack;
        }
NEXT_SPAN:
        e1->x += e1->dx;
#ifdef OD_GSTR_USEFIXEDTEX
        e1->u += e1->du;
        e1->v += e1->dv;
#else
        e1->s += e1->ds;
        e1->t += e1->dt;
#endif
        e2->x += e2->dx;
#ifdef OD_GSTR_USEFIXEDTEX
        e2->u += e2->du;
        e2->v += e2->dv;
#else
        e2->s += e2->ds;
        e2->t += e2->dt;
#endif
        drawer -= m_scanLineLength;
        if (stencil) stencil -= m_nrcWidth;
        h--;
      }
    }
    // Blit degrade line
#ifdef OD_GSTR_USEFIXEDTEX
    void blitTexturedLine(int y, int x1, int x2, int u1, int v1, int u2, int v2, OdUInt8 alpha)
    {
      if (y < m_clipRegion.m_min.y || y >= m_clipRegion.m_max.y)
        return;
      if (x1 > x2)
      {
        x1 ^= x2 ^= x1 ^= x2;
        u1 ^= u2 ^= u1 ^= u2;
        v1 ^= v2 ^= v1 ^= v2;
      }
      rEdgeTextured e1, e2;
      e1.x = x1 << 16;
      e1.u = u1 << 16;
      e1.v = v1 << 16;
      e2.x = x2 << 16;
      e2.u = u2 << 16;
      e2.v = v2 << 16;
      e1.dx = e2.dx = 0;
      e1.du = e2.dv = e1.dv = e2.du = 0;
      blitTriangleTextured(&e1, &e2, y, 1, alpha);
    }
#endif
    void blitTexturedLine(int y, int x1, int x2, double s1, double t1, double s2, double t2, OdUInt8 alpha)
    {
      if (y < m_clipRegion.m_min.y || y >= m_clipRegion.m_max.y)
        return;
      if (x1 > x2)
      {
        x1 ^= x2 ^= x1 ^= x2;
        double tmp;
        tmp = s1; s1 = s2; s2 = tmp;
        tmp = t1; t1 = t2; t2 = tmp;
      }
      rEdgeTextured e1, e2;
      e1.x = x1 << 16;
#ifdef OD_GSTR_USEFIXEDTEX
      e1.u = int(OdRound(s1));
      e1.v = int(OdRound(t1));
#else
      e1.s = s1;
      e1.t = t1;
#endif
      e2.x = x2 << 16;
#ifdef OD_GSTR_USEFIXEDTEX
      e2.u = int(OdRound(s2));
      e2.v = int(OdRound(t2));
#else
      e2.s = s2;
      e2.t = t2;
#endif
      e1.dx = e2.dx = 0;
#ifdef OD_GSTR_USEFIXEDTEX
      e1.du = e2.dv = e1.dv = e2.du = 0;
#else
      e1.ds = e1.dt = e2.ds = e2.dt = 0.0;
#endif
      blitTriangleTextured(&e1, &e2, y, 1, alpha);
    }
    // Process textured triangle edge
    int edgeTriangleTexturedPrec(rEdgeTexturedPrec *e, rVertexTexturedPrec *v1, rVertexTexturedPrec *v2)
    {
      int dy, h, adv_y = 0;
      double fdy;
      dy = h = v2->y - v1->y;
      if (v1->y < m_clipRegion.m_min.y)
      {
        if (v2->y <= m_clipRegion.m_min.y)
          return 0;
        adv_y = m_clipRegion.m_min.y - v1->y;
        h -= adv_y;
      }
      if (v2->y > m_clipRegion.m_max.y)
      {
        if (v1->y >= m_clipRegion.m_max.y)
          return 0;
        h -= v2->y - m_clipRegion.m_max.y;
      }
      fdy = (dy) ? (1.0 / dy) : 0.0;
      e->x = (double)v1->x;
      e->s = v1->s;
      e->t = v1->t;
      e->dx = (v2->x - v1->x) * fdy;
      e->ds = (v2->s - v1->s) * fdy;
      e->dt = (v2->t - v1->t) * fdy;
      if (adv_y)
      {
        e->x += e->dx * adv_y;
        e->s += e->ds * adv_y;
        e->t += e->dt * adv_y;
      }
      return h;
    }
    // Draw half of textured triangle
    void blitTriangleTexturedPrec(rEdgeTexturedPrec *e1, rEdgeTexturedPrec *e2, int y, int h, OdUInt8 alpha) {
      int x1, x2, dx, count, x_adv;
      double fdx;
      double s, ds, t, dt;
      OdUInt8 *stack, *sstack;
      OdUInt8 *drawer = m_pScanLines;
      OdUInt8 *stencil = m_pnrcClip;
      drawer += (m_height - 1 - y) * m_scanLineLength;
      if (stencil) stencil += (m_height - 1 - y - m_nrcOffsetY) * m_nrcWidth;
      while (h > 0)
      {
        x1 = (int)OdRound(e1->x); x2 = (int)OdRound(e2->x);
        x2++;
        dx = x2 - x1;
        if (x2 > m_clipRegion.m_max.x)
        {
          x2 = m_clipRegion.m_max.x;
        }
        if (x1 < m_clipRegion.m_min.x)
        {
          x_adv = m_clipRegion.m_min.x - x1;
          x1 = m_clipRegion.m_min.x;
        }
        else
        {
          x_adv = 0;
        }
        if (x1 >= x2)
        {
          goto NEXT_SPAN;
        }
        fdx = (dx) ? (1.0 / dx) : 0.0;
        ds = (e2->s - e1->s) * fdx;
        dt = (e2->t - e1->t) * fdx;
        s = e1->s; t = e1->t;
        if (x_adv)
        {
          s += ds * x_adv;
          t += dt * x_adv;
        }
        if (!stencil)
        {
          stack = drawer;
          drawer += x1 * 3;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
              getTexturePixel((int)OdRound(s), (int)OdRound(t), drawer);
              count--;
              drawer += 3;
              s += ds; t += dt;
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
              getTexturePixelAlpha((int)OdRound(s), (int)OdRound(t), drawer, alpha);
              count--;
              drawer += 3;
              s += ds; t += dt;
            }
          }
          drawer = stack;
        }
        else
        {
          stack = drawer;
          sstack = stencil;
          drawer += x1 * 3;
          stencil += x1 - m_nrcOffsetX;
          count = x2 - x1;
          if (alpha == 255)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                getTexturePixel((int)OdRound(s), (int)OdRound(t), drawer);
              }
              stencil++;
              count--;
              drawer += 3;
              s += ds; t += dt;
            }
          }
          else if (alpha != 0)
          {
            while (count > 0)
            {
              if (*stencil)
              {
                getTexturePixelAlpha((int)OdRound(s), (int)OdRound(t), drawer, alpha);
              }
              stencil++;
              count--;
              drawer += 3;
              s += ds; t += dt;
            }
          }
          drawer = stack;
          stencil = sstack;
        }
NEXT_SPAN:
        e1->x += e1->dx;
        e1->s += e1->ds;
        e1->t += e1->dt;
        e2->x += e2->dx;
        e2->s += e2->ds;
        e2->t += e2->dt;
        drawer -= m_scanLineLength;
        if (stencil) stencil -= m_nrcWidth;
        h--;
      }
    }

    // Get pixel color from texture
    void getTexturePixel(int u, int v, OdUInt8 *bgr)
    {
      // Texture addressing (wrap, clamp, mirror and etc.)
      m_fpTextureAddressing(u, v, (int)m_texWidth, (int)m_texHeight);
      // Texture interpolation (nearest, bilinear, trilinear and etc.)
      m_fpTextureBlit(m_pTexScanLines, m_pTexPalette, m_texScanLineLength, u, v, bgr);
    }

    // Get pixel color from texture with alpha merge
    void getTexturePixelAlpha(int u, int v, OdUInt8 *bgr, OdUInt8 alpha)
    {
      // Texture addressing (wrap, clamp, mirror and etc.)
      m_fpTextureAddressing(u, v, (int)m_texWidth, (int)m_texHeight);
      // Texture interpolation (nearest, bilinear, trilinear and etc.)
      OdUInt8 preReq[3] = { bgr[0], bgr[1], bgr[2] };
      m_fpTextureBlit(m_pTexScanLines, m_pTexPalette, m_texScanLineLength, u, v, preReq);
      OdMergeRGBAlpha(bgr[0], bgr[1], bgr[2], preReq[0], preReq[1], preReq[2], alpha, bgr[0], bgr[1], bgr[2]);
    }
};

// end of OdGsTriangleRenderer implementation

// OdGsBitmapVectorizeDevice

OdGsBitmapVectorizeDevice::OdGsBitmapVectorizeDevice()
: m_width(0)
, m_height(0)
, m_scanLineLength(0)
, m_pScanLines(0)
, m_bEnableHLR(false)
, m_bPrecisionMode(false)
, m_rasterWidth(0), m_rasterHeight(0), m_rasterScanLineLength(0)
, m_pRasterScanLines(NULL)
, m_pRasterPalette(NULL)
, m_rasterBitDepth(kRIBD24Bpp)
, m_bRasterTransparency(false), m_bRasterRGB(false)
, m_raster32TranspMode(kRI32TTDef)
, m_nrcOffsetX(0), m_nrcOffsetY(0)
, m_nrcWidth(0), m_nrcHeight(0)
, m_pnrcClip(NULL)
{
}

OdGsBitmapVectorizeDevice::~OdGsBitmapVectorizeDevice()
{
}

OdColorConverterCallback *OdGsBitmapVectorizeDevice::getColorConverter()
{
  if (!properties().isNull() && properties()->has(OD_T("ColorConverter")))
    return OdColorConverterCallbackPtr(properties()->getAt(OD_T("ColorConverter")));
  return NULL;
}

void OdGsBitmapVectorizeDevice::update(OdGsDCRect* pUpdatedRect)
{
  // AMark : #5312 fix
  if (!isValid())
  {
    initMetrics();
    // Force invalidation of all views
    invalidate();
  }
  //
  OdGsBaseVectorizeDevice::update(pUpdatedRect);
}

OdGsModelPtr OdGsBitmapVectorizeDevice::createModel()
{
  return OdGsModelPtr();
}

inline void swap(int& a, int& b)
{
  int c = a;
  a = b;
  b = c;
}
inline void swap(long& a, long& b)
{
  long c = a;
  a = b;
  b = c;
}
inline void swap(double& a, double& b)
{
  double c = a;
  a = b;
  b = c;
}

void OdGsBitmapVectorizeDevice::setClipRegion(const OdGsDCRect& rect) // set clip region
{
  m_clipRegion = rect;
  m_clipRegion.m_max.y = m_height - m_clipRegion.m_max.y;
  m_clipRegion.m_min.y = m_height - m_clipRegion.m_min.y;
  if(m_clipRegion.m_min.x > m_clipRegion.m_max.x)
    swap(m_clipRegion.m_min.x, m_clipRegion.m_max.x);
  if(m_clipRegion.m_min.y > m_clipRegion.m_max.y)
    swap(m_clipRegion.m_min.y, m_clipRegion.m_max.y);
  if(m_clipRegion.m_min.x < 0)
    m_clipRegion.m_min.x = 0;
  if(m_clipRegion.m_min.y < 0)
    m_clipRegion.m_min.y = 0;
  if(m_clipRegion.m_max.x > (int)m_width-1)
    m_clipRegion.m_max.x = m_width-1;
  if(m_clipRegion.m_max.y > (int)m_height-1)
    m_clipRegion.m_max.y = m_height-1;
}

template <class Type>
inline bool linterp_add(Type &y1, Type y2, Type x1, Type x2, Type minVal)
{ y1 += (y2-y1)*(minVal-x1)/(x2-x1); return true; }
template <> // #CORE-12621 : Specialization for integers
inline bool linterp_add<int>(int &y1, int y2, int x1, int x2, int minVal)
{ OdInt64 _y1 = OdInt64(y1) + ((OdInt64(y2) - OdInt64(y1)) * (OdInt64(minVal) - OdInt64(x1)) / (OdInt64(x2) - OdInt64(x1)));
  if ((_y1 < INT_MIN) || (_y1 > INT_MAX)) return false; // Better to skip additional calculation due to overflow
  y1 = (int)_y1; return true; }

template <class Type>
inline bool clipMin(Type& x1, Type& y1, Type x2, Type y2, int minX)
{
  if(x1 < (Type)minX)
  {
    if(x2 >= (Type)minX)
    {
      if (!linterp_add<Type>(y1, y2, x1, x2, (Type)minX))
        return false;
      x1  = (Type)minX;
    }
    else
      return false;
  }
  return true;
}
template <class Type>
inline bool clipMax(Type& x1, Type& y1, Type x2, Type y2, int maxX)
{
  if(x1 > (Type)maxX)
  {
    if(x2 <= (Type)maxX)
    {
      if (!linterp_add<Type>(y1, y2, x1, x2, (Type)maxX))
        return false;
      x1  = (Type)maxX;
    }
    else
      return false;
  }
  return true;
}

void OdGsBitmapVectorizeDevice::draw_line(int x1, int y1, int x2, int y2, ODCOLORREF col)
{
  y1 = m_height-y1;
  y2 = m_height-y2;

  // clipping
  if(!clipMin(x1, y1, x2, y2, m_clipRegion.m_min.x))
    return ;
  if(!clipMax(x1, y1, x2, y2, m_clipRegion.m_max.x))
    return ;

  if(!clipMin(x2, y2, x1, y1, m_clipRegion.m_min.x))
    return ;
  if(!clipMax(x2, y2, x1, y1, m_clipRegion.m_max.x))
    return ;

  if(!clipMin(y1, x1, y2, x2, m_clipRegion.m_min.y))
    return ;
  if(!clipMax(y1, x1, y2, x2, m_clipRegion.m_max.y))
    return ;

  if(!clipMin(y2, x2, y1, x1, m_clipRegion.m_min.y))
    return ;
  if(!clipMax(y2, x2, y1, x1, m_clipRegion.m_max.y))
    return ;

  // #CORE-12621 : check final results correctness
  //ODA_ASSERT(OdGsDCRect(odmin(x1, x2), odmax(x2, x1), odmin(y1, y2), odmax(y2, y1)).within(m_clipRegion));

  OdUInt8 red(ODGETRED(col)), green(ODGETGREEN(col)), blue(ODGETBLUE(col)), alpha(ODGETALPHA(col));

  int dx=Od_abs(x2-x1);
  int dy=Od_abs(y2-y1);
  int sx=x2 >= x1 ? 3:-3;
  int sy=y2 >= y1 ? m_scanLineLength:-(int)m_scanLineLength;
  int d,d1,d2,i;
  OdUInt8* pCurPtr = getPixPtr(x1, y1);
  if (!m_pnrcClip)
  {
    if(dy<=dx)
    {
      d =(dy << 1)-dx;
      d1=dy << 1;
      d2=(dy - dx) << 1;
      if (alpha == 255)
      {
        putPixel(pCurPtr, red, green, blue);
        for(pCurPtr+=sx,i=1;i<=dx;i++,pCurPtr+=sx)
        {
          if (d>0)
          {
            d+=d2;
            pCurPtr += sy;
          }
          else 
            d+=d1;
          putPixel(pCurPtr, red, green, blue);
        }
      }
      else if (alpha > 0)
      {
        putPixel(pCurPtr, red, green, blue, alpha);
        for(pCurPtr+=sx,i=1;i<=dx;i++,pCurPtr+=sx)
        {
          if (d>0)
          {
            d+=d2;
            pCurPtr += sy;
          }
          else 
            d+=d1;
          putPixel(pCurPtr, red, green, blue, alpha);
        }
      }
    }
    else
    {
      d=(dx << 1) - dy;
      d1=dx << 1;
      d2=(dx - dy) << 1;
      if (alpha == 255)
      {
        putPixel(pCurPtr, red, green, blue);
        for(pCurPtr+=sy,i=1;i<=dy;i++,pCurPtr+=sy)
        {
          if(d>0)
          {
            d+=d2; 
            pCurPtr+=sx;
          }
          else 
            d+=d1;
          putPixel(pCurPtr, red, green, blue);
        }
      }
      else if (alpha > 0)
      {
        putPixel(pCurPtr, red, green, blue, alpha);
        for(pCurPtr+=sy,i=1;i<=dy;i++,pCurPtr+=sy)
        {
          if(d>0)
          {
            d+=d2; 
            pCurPtr+=sx;
          }
          else 
            d+=d1;
          putPixel(pCurPtr, red, green, blue, alpha);
        }
      }
    }
  }
  else
  {
    int ssx=x2 >= x1 ? 1:-1;
    int ssy=y2 >= y1 ? m_nrcWidth:-(int)m_nrcWidth;
    OdUInt8* pStencil = m_pnrcClip + (y1 - m_nrcOffsetY) * m_nrcWidth + (x1 - m_nrcOffsetX);
    if(dy<=dx)
    {
      d =(dy << 1)-dx;
      d1=dy << 1;
      d2=(dy - dx) << 1;
      if (alpha == 255)
      {
        if (*pStencil)
          putPixel(pCurPtr, red, green, blue);
        for(pCurPtr+=sx,pStencil+=ssx,i=1;i<=dx;i++,pCurPtr+=sx,pStencil+=ssx)
        {
          if (d>0)
          {
            d+=d2;
            pCurPtr += sy;
            pStencil += ssy;
          }
          else 
            d+=d1;
          if (*pStencil)
            putPixel(pCurPtr, red, green, blue);
        }
      }
      else if (alpha > 0)
      {
        if (*pStencil)
          putPixel(pCurPtr, red, green, blue, alpha);
        for(pCurPtr+=sx,pStencil+=ssx,i=1;i<=dx;i++,pCurPtr+=sx,pStencil+=ssx)
        {
          if (d>0)
          {
            d+=d2;
            pCurPtr += sy;
            pStencil += ssy;
          }
          else 
            d+=d1;
          if (*pStencil)
            putPixel(pCurPtr, red, green, blue, alpha);
        }
      }
    }
    else
    {
      d=(dx << 1) - dy;
      d1=dx << 1;
      d2=(dx - dy) << 1;
      if (alpha == 255)
      {
        if (*pStencil)
          putPixel(pCurPtr, red, green, blue);
        for(pCurPtr+=sy,pStencil+=ssy,i=1;i<=dy;i++,pCurPtr+=sy,pStencil+=ssy)
        {
          if(d>0)
          {
            d+=d2; 
            pCurPtr+=sx;
            pStencil+=ssx;
          }
          else 
            d+=d1;
          if (*pStencil)
            putPixel(pCurPtr, red, green, blue);
        }
      }
      else if (alpha > 0)
      {
        if (*pStencil)
          putPixel(pCurPtr, red, green, blue, alpha);
        for(pCurPtr+=sy,pStencil+=ssy,i=1;i<=dy;i++,pCurPtr+=sy,pStencil+=ssy)
        {
          if(d>0)
          {
            d+=d2; 
            pCurPtr+=sx;
            pStencil+=ssx;
          }
          else 
            d+=d1;
          if (*pStencil)
            putPixel(pCurPtr, red, green, blue, alpha);
        }
      }
    }
  }
}

void OdGsBitmapVectorizeDevice::draw_lineAntiAlias(double x1, double y1, double x2, double y2, ODCOLORREF col)
{
  y1 = (double)m_height - y1;
  y2 = (double)m_height - y2;

  // clipping
  if (!clipMin(x1, y1, x2, y2, m_clipRegion.m_min.x))
    return;
  if (!clipMax(x1, y1, x2, y2, m_clipRegion.m_max.x))
    return;

  if (!clipMin(x2, y2, x1, y1, m_clipRegion.m_min.x))
    return;
  if (!clipMax(x2, y2, x1, y1, m_clipRegion.m_max.x))
    return;

  if (!clipMin(y1, x1, y2, x2, m_clipRegion.m_min.y))
    return;
  if (!clipMax(y1, x1, y2, x2, m_clipRegion.m_max.y))
    return;

  if (!clipMin(y2, x2, y1, x1, m_clipRegion.m_min.y))
    return;
  if (!clipMax(y2, x2, y1, x1, m_clipRegion.m_max.y))
    return;

  OdUInt8 red(ODGETRED(col)), green(ODGETGREEN(col)), blue(ODGETBLUE(col)), alpha(ODGETALPHA(col));

#define ipart_(X) ((int)(X))
#define round_(X) ((int)(((double)(X)) + 0.5))
#define fpart_(X) (((double)(X)) - (double)ipart_(X))
#define rfpart_(X) (1.0 - fpart_(X))

  double dx = x2 - x1, dy = y2 - y1;
  if (fabs(dx) > fabs(dy))
  {
    if (x2 < x1)
    {
      swap(x1, x2);
      swap(y1, y2);
    }
    double gradient = dy / dx;
    int xpxl1 = round_(x1);
    double xend = (double)xpxl1;
    double yend = y1 + gradient * (xend - x1);
    double xgap = rfpart_(x1 + 0.5);
    int ypxl1 = ipart_(yend);
    putPixelAlias(xpxl1, ypxl1    , red, green, blue, alpha, rfpart_(yend) * xgap);
    putPixelAlias(xpxl1, ypxl1 + 1, red, green, blue, alpha,  fpart_(yend) * xgap);
    double intery = yend + gradient;

    int xpxl2 = round_(x2);
    xend = (double)xpxl2;
    yend = y2 + gradient * (xend - x2);
    xgap = fpart_(x2 + 0.5);
    int ypxl2 = ipart_(yend);
    putPixelAlias(xpxl2, ypxl2    , red, green, blue, alpha, rfpart_(yend) * xgap);
    putPixelAlias(xpxl2, ypxl2 + 1, red, green, blue, alpha,  fpart_(yend) * xgap);

    for(int x = xpxl1 + 1; x <= (xpxl2 - 1); x++)
    {
      putPixelAlias(x, ipart_(intery)    , red, green, blue, alpha, rfpart_(intery));
      putPixelAlias(x, ipart_(intery) + 1, red, green, blue, alpha,  fpart_(intery));
      intery += gradient;
    }
  }
  else
  {
    if (y2 < y1)
    {
      swap(x1, x2);
      swap(y1, y2);
    }
    double gradient = dx / dy;
    int ypxl1 = round_(y1);
    double yend = (double)ypxl1;
    double xend = x1 + gradient * (yend - y1);
    double ygap = rfpart_(y1 + 0.5);
    int xpxl1 = ipart_(xend);
    putPixelAlias(xpxl1, ypxl1    , red, green, blue, alpha, rfpart_(xend) * ygap);
    putPixelAlias(xpxl1 + 1, ypxl1, red, green, blue, alpha,  fpart_(xend) * ygap);
    double interx = xend + gradient;

    int ypxl2 = round_(y2);
    yend = (double)ypxl2;
    xend = x2 + gradient * (yend - y2);
    ygap = fpart_(y2 + 0.5);
    int xpxl2 = ipart_(xend);
    putPixelAlias(xpxl2, ypxl2    , red, green, blue, alpha, rfpart_(xend) * ygap);
    putPixelAlias(xpxl2 + 1, ypxl2, red, green, blue, alpha,  fpart_(xend) * ygap);

    for(int y = ypxl1 + 1; y <= (ypxl2 - 1); y++)
    {
      putPixelAlias(ipart_(interx), y    , red, green, blue, alpha, rfpart_(interx));
      putPixelAlias(ipart_(interx) + 1, y, red, green, blue, alpha,  fpart_(interx));
      interx += gradient;
    }
  }

#undef ipart_
#undef round_
#undef fpart_
#undef rfpart_
}

void OdGsBitmapVectorizeDevice::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, ODCOLORREF col)
{
#if 1
  // Solid color triangle
  if (!m_bPrecisionMode)
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip).renderTriangleFlat(x1, y1, x2, y2, x3, y3, col);
  else
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip).renderTriangleFlatPrec(x1, y1, x2, y2, x3, y3, col);
#else
  y1 = m_height-y1;
  y2 = m_height-y2;
  y3 = m_height-y3;

  // must be y1 = min(y), y3 = max(y)
  if(y2 < y1)
  {
    swap(x1, x2);
    swap(y1, y2);
  }
  if(y3 < y1)
  {
    swap(x3, x1);
    swap(y3, y1);
  }
  if(y3 < y2)
  {
    swap(x3, x2);
    swap(y3, y2);
  }
  if(y1 == y3)
  {
    int minX(x1), maxX(x2);
    if(minX > x2)
      minX = x2;
    if(minX > x3)
      minX = x3;
    if(maxX < x2)
      maxX = x2;
    if(maxX < x3)
      maxX = x3;
    draw_horizontalLine(y1, minX, maxX, col);
  }
  else
  {
    for(int sy = y1; sy <= y3; sy++)
    {
      int xx1, xx2;
      xx1 = x1 + (sy - y1) * (x3 - x1) / (y3 - y1);
      if (sy < y2)
        xx2 = x1 + (sy - y1) * (x2 - x1) / (y2 - y1);
      else 
      {
        if(y3 == y2)
          xx2 = x2;
        else
          xx2 = x2 + (sy - y2) * (x3 - x2) / (y3 - y2);
      }
      draw_horizontalLine(sy, xx1, xx2, col);
    }
  }
#endif
}

void OdGsBitmapVectorizeDevice::draw_gouraud_triangle(int x1, int y1, ODCOLORREF color1,
                                                      int x2, int y2, ODCOLORREF color2,
                                                      int x3, int y3, ODCOLORREF color3, OdUInt8 alpha)
{
  if (!m_bPrecisionMode)
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip).renderTriangleGouraud(x1, y1, color1, x2, y2, color2, x3, y3, color3, alpha);
  else
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip).renderTriangleGouraudPrec(x1, y1, color1, x2, y2, color2, x3, y3, color3, alpha);
}

OdGsBitmapVectorizeDevice::blitFunc OdGsBitmapVectorizeDevice::chooseBlitFuncFor() const
{
  switch (m_rasterBitDepth)
  {
    case kRIBDMono:
    return (m_bRasterTransparency) ? textureBlit_Nearest_1bpp_Transparent : textureBlit_Nearest_1bpp;
    case kRIBD16Clr:
    return textureBlit_Nearest_4bpp;
    case kRIBD256Clr:
    return textureBlit_Nearest_8bpp;
    case kRIBD24Bpp:
    return (m_bRasterRGB) ? textureBlit_Nearest_24bpp_RGB : textureBlit_Nearest_24bpp;
    case kRIBD32Bpp:
      if (m_bRasterRGB)
      {
        if (m_bRasterTransparency)
        {
          switch (m_raster32TranspMode)
          {
            case kRI32TT1: return textureBlit_Nearest_32bpp_RGB_Transparent1Bit;
            case kRI32TT8: return textureBlit_Nearest_32bpp_RGB_Transparent8Bit;
            default: return textureBlit_Nearest_32bpp_RGB_TransparentDef;
          }
        }
        else
        {
          return textureBlit_Nearest_32bpp_RGB;
        }
      }
      else
      {
        if (m_bRasterTransparency)
        {
          switch (m_raster32TranspMode)
          {
            case kRI32TT1: return textureBlit_Nearest_32bpp_Transparent1Bit;
            case kRI32TT8: return textureBlit_Nearest_32bpp_Transparent8Bit;
            default: return textureBlit_Nearest_32bpp_TransparentDef;
          }
        }
        else
        {
          return textureBlit_Nearest_32bpp;
        }
      }
    break;
  }
  return NULL;
}

void OdGsBitmapVectorizeDevice::draw_textured_triangle(int x1, int y1, const OdGePoint3d &pt1,
                                                       int x2, int y2, const OdGePoint3d &pt2,
                                                       int x3, int y3, const OdGePoint3d &pt3, OdUInt8 alpha)
{
  ODA_ASSERT(m_pRasterScanLines != NULL);

  OdGePoint3d pt[3] = { pt1, pt2, pt3 };
  for (int i = 0; i < 3; i++)
    pt[i].transformBy(m_rasterTransform);

  if (!m_bPrecisionMode)
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip,
      m_rasterWidth, m_rasterHeight, m_pRasterScanLines, m_rasterScanLineLength, m_pRasterPalette)
      .setTextureBlittingFunc(chooseBlitFuncFor())
      .renderTriangleTextured(x1, y1, pt[0].x, pt[0].y, x2, y2, pt[1].x, pt[1].y, x3, y3, pt[2].x, pt[2].y, alpha);
  else
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip,
      m_rasterWidth, m_rasterHeight, m_pRasterScanLines, m_rasterScanLineLength, m_pRasterPalette)
      .setTextureBlittingFunc(chooseBlitFuncFor())
      .renderTriangleTexturedPrec(x1, y1, pt[0].x, pt[0].y, x2, y2, pt[1].x, pt[1].y, x3, y3, pt[2].x, pt[2].y, alpha);
}

void OdGsBitmapVectorizeDevice::draw_textured_triangle(int x1, int y1, const OdGePoint3d &pt1,
                                                       int x2, int y2, const OdGePoint3d &pt2,
                                                       int x3, int y3, const OdGePoint3d &pt3,
                                                       bool wrap, bool notransform, OdUInt8 alpha)
{
  ODA_ASSERT(m_pRasterScanLines != NULL);

  OdGePoint3d pt[3] = { pt1, pt2, pt3 };
  if (!notransform)
  {
    for (int i = 0; i < 3; i++)
      pt[i].transformBy(m_rasterTransform);
  }

  if (!m_bPrecisionMode)
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip,
      m_rasterWidth, m_rasterHeight, m_pRasterScanLines, m_rasterScanLineLength, m_pRasterPalette)
      .setTextureAddressingFunc((wrap) ? textureAddressing_Wrap : textureAddressing_Clamp)
      .setTextureBlittingFunc(chooseBlitFuncFor())
      .renderTriangleTextured(x1, y1, pt[0].x, pt[0].y, x2, y2, pt[1].x, pt[1].y, x3, y3, pt[2].x, pt[2].y, alpha);
  else
    OdGsTriangleRenderer(m_width, m_height, m_pScanLines, m_scanLineLength, m_clipRegion,
      m_nrcOffsetX, m_nrcOffsetY, m_nrcWidth, m_nrcHeight, m_pnrcClip,
      m_rasterWidth, m_rasterHeight, m_pRasterScanLines, m_rasterScanLineLength, m_pRasterPalette)
      .setTextureAddressingFunc((wrap) ? textureAddressing_Wrap : textureAddressing_Clamp)
      .setTextureBlittingFunc(chooseBlitFuncFor())
      .renderTriangleTexturedPrec(x1, y1, pt[0].x, pt[0].y, x2, y2, pt[1].x, pt[1].y, x3, y3, pt[2].x, pt[2].y, alpha);
}

void OdGsBitmapVectorizeDevice::draw_clip_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
  y1 = m_height-y1;
  y2 = m_height-y2;
  y3 = m_height-y3;

  // must be y1 = min(y), y3 = max(y)
  if(y2 < y1)
  {
    swap(x1, x2);
    swap(y1, y2);
  }
  if(y3 < y1)
  {
    swap(x3, x1);
    swap(y3, y1);
  }
  if(y3 < y2)
  {
    swap(x3, x2);
    swap(y3, y2);
  }
  if(y1 == y3)
  {
    int minX(x1), maxX(x2);
    if(minX > x2)
      minX = x2;
    if(minX > x3)
      minX = x3;
    if(maxX < x2)
      maxX = x2;
    if(maxX < x3)
      maxX = x3;
    draw_horizontalLineClip(y1, minX, maxX);
  }
  else
  {
    for(int sy = y1; sy <= y3; sy++)
    {
      int xx1, xx2;
      xx1 = x1 + (sy - y1) * (x3 - x1) / (y3 - y1);
      if (sy < y2)
        xx2 = x1 + (sy - y1) * (x2 - x1) / (y2 - y1);
      else 
      {
        if(y3 == y2)
          xx2 = x2;
        else
          xx2 = x2 + (sy - y2) * (x3 - x2) / (y3 - y2);
      }
      draw_horizontalLineClip(sy, xx1, xx2);
    }
  }
}

void OdGsBitmapVectorizeDevice::draw_filled_circle(int x, int y, double radius, ODCOLORREF color)
{
  y = m_height - y;
  OdInt32 iDmtr = (OdInt32)OdRound(radius * 2);
  OdInt32 dmtrLow = iDmtr / 2;
  OdInt32 dmtrHigh = dmtrLow;//iDmtr - dmtrLow;
  if (iDmtr < 2)
  {
    draw_point(x, y, color);
  }
  else
  {
    int xFrom = x - dmtrLow;
    int xTo = x + dmtrHigh;
    int yFrom = y - dmtrLow;
    int yTo = y + dmtrHigh;
    int i;
    if (xFrom > m_clipRegion.m_max.x || xTo < m_clipRegion.m_min.x ||
        yFrom > m_clipRegion.m_max.y || yTo < m_clipRegion.m_min.y)
      return;
    // Low part
    for (i = yFrom; i <= y; i++)
    {
       OdInt32 inRad = (OdInt32)OdRound(sqrt(double(dmtrLow * dmtrLow) - double((i - y) * (i - y))) * 2);
       OdInt32 inRadLow = inRad / 2;
       OdInt32 inRadHigh = inRad - inRadLow;
       if (inRad > 0)
         draw_horizontalLine(i - 1, x - inRadLow, x + inRadHigh, color);
    }
    // High part
    for (i = y + 1; i <= yTo; i++)
    {
       OdInt32 inRad = (OdInt32)OdRound(sqrt(double(dmtrHigh * dmtrHigh) - double((i - y) * (i - y))) * 2);
       OdInt32 inRadLow = inRad / 2;
       OdInt32 inRadHigh = inRad - inRadLow;
       if (inRad > 0)
         draw_horizontalLine(i - 1, x - inRadLow, x + inRadHigh, color);
    }
  }
}

void OdGsBitmapVectorizeDevice::draw_lwLine(int x1, int y1, int x2, int y2, int lw, ODCOLORREF color)
{
  OdGeVector2d vDir((double)(x2 - x1), (double)(y2 - y1));
  if (!OdZero(vDir.lengthSqrd()))
  {
    double dblLw = double(lw) * 0.5;
    OdGeVector2d vPerp = vDir.perpVector().normal();
    OdGeVector2d vPerpMax = vPerp * dblLw;
    draw_triangle((int)OdRound(double(x1) + vPerpMax.x),
                  (int)OdRound(double(y1) + vPerpMax.y),
                  (int)OdRound(double(x2) + vPerpMax.x),
                  (int)OdRound(double(y2) + vPerpMax.y),
                  (int)OdRound(double(x2) - vPerpMax.x),
                  (int)OdRound(double(y2) - vPerpMax.y),
                  color);
    draw_triangle((int)OdRound(double(x1) + vPerpMax.x),
                  (int)OdRound(double(y1) + vPerpMax.y),
                  (int)OdRound(double(x2) - vPerpMax.x),
                  (int)OdRound(double(y2) - vPerpMax.y),
                  (int)OdRound(double(x1) - vPerpMax.x),
                  (int)OdRound(double(y1) - vPerpMax.y),
                  color);
    draw_filled_circle(x1, y1, double(lw) * 0.5, color);
  }
  draw_filled_circle(x2, y2, double(lw) * 0.5, color);
}

void OdGsBitmapVectorizeDevice::initMetrics()
{
  m_clipRegion.m_max.x = m_width  = width();
  m_clipRegion.m_max.y = m_height = height();
  m_clipRegion.m_max.x --;
  m_clipRegion.m_max.y --;
  m_clipRegion.m_min.x = m_clipRegion.m_min.y = 0;
  m_scanLineLength = scanLineSize();
  m_scanLines.resize(m_scanLineLength * m_height);
  m_pScanLines = m_scanLines.asArrayPtr();
  ODCOLORREF bgColor = getBackgroundColor();
  { OdColorConverterCallback *pColorConverter = getColorConverter();
    if (pColorConverter && pColorConverter->convertBackgroundColors())
      bgColor = pColorConverter->convert(bgColor); }
  OdUInt8 bgRed(ODGETRED(bgColor)), bgGreen(ODGETGREEN(bgColor)), bgBlue(ODGETBLUE(bgColor));
  OdUInt8* pCurLn = m_pScanLines;
  for(OdUInt32 i = 0; i < m_height; i ++)
  {
    OdUInt8* pCurPix = pCurLn;
    for(OdUInt32 j = 0; j < m_width; j ++)
    {
      *(pCurPix++) = bgBlue;
      *(pCurPix++) = bgGreen;
      *(pCurPix++) = bgRed;
    }
    pCurLn += m_scanLineLength;
  }
}

  // implementation of OdGiRasterImage interface
OdUInt32 OdGsBitmapVectorizeDevice::pixelWidth() const
{
  return m_width;
}

OdUInt32 OdGsBitmapVectorizeDevice::pixelHeight() const
{
  return m_height;
}

OdUInt32 OdGsBitmapVectorizeDevice::colorDepth() const
{
  return 24;
}

OdUInt32 OdGsBitmapVectorizeDevice::numColors() const
{
  return 0;
}

ODCOLORREF OdGsBitmapVectorizeDevice::color(OdUInt32 ) const
{
  return ODRGB(0,0,0);
}

OdUInt32 OdGsBitmapVectorizeDevice::paletteDataSize() const
{
  return 0;
}

void OdGsBitmapVectorizeDevice::paletteData(OdUInt8* ) const
{
}

void OdGsBitmapVectorizeDevice::scanLines(OdUInt8* pBytes, OdUInt32 index, OdUInt32 numLines) const
{
  ::memcpy(pBytes, m_scanLines.asArrayPtr() + index*m_scanLineLength, numLines*m_scanLineLength);
}

const OdUInt8* OdGsBitmapVectorizeDevice::scanLines() const
{
  return m_scanLines.asArrayPtr();
}

OdGiRasterImage::PixelFormatInfo OdGsBitmapVectorizeDevice::pixelFormat() const
{
  OdGiRasterImage::PixelFormatInfo res;
  res.setBGRA(); // ? May be must be BGR?
  return res;
}

OdUInt32 OdGsBitmapVectorizeDevice::scanLinesAlignment() const
{
  return 4;
}

bool OdGsBitmapVectorizeDevice::isRasterImageProcessible(const OdGiRasterImage *pImage) const
{
  switch (pImage->colorDepth())
  {
    case 1:
    case 4:
    case 8:
    case 24:
    case 32:
    return true;
  }
  return false;
}

void OdGsBitmapVectorizeDevice::setRasterImage(const OdGiRasterImage *pImage, bool bTransparent)
{
  m_rasterWidth = pImage->pixelWidth(); m_rasterHeight = pImage->pixelHeight();
  m_rasterScanLineLength = pImage->scanLineSize();
  m_bRasterTransparency = m_bRasterRGB = false;
  m_raster32TranspMode = kRI32TTDef;
  switch (pImage->colorDepth())
  {
    case 1: m_rasterBitDepth = kRIBDMono; m_bRasterTransparency = bTransparent && (pImage->transparentColor() != -1); break;
    case 4: m_rasterBitDepth = kRIBD16Clr; break;
    case 8: m_rasterBitDepth = kRIBD256Clr; break;
    case 24: m_rasterBitDepth = kRIBD24Bpp; m_bRasterRGB = (pImage->pixelFormat().isRGB() || pImage->pixelFormat().isRGBA()); break;
    case 32: m_rasterBitDepth = kRIBD32Bpp; m_bRasterRGB = (pImage->pixelFormat().isRGB() || pImage->pixelFormat().isRGBA());
             m_bRasterTransparency = bTransparent && pImage->transparencyMode() != OdGiRasterImage::kTransparencyOff;
             if (m_bRasterTransparency)
             {
               switch (pImage->transparencyMode())
               {
                case OdGiRasterImage::kTransparency1Bit: m_raster32TranspMode = kRI32TT1; break;
                case OdGiRasterImage::kTransparency8Bit: m_raster32TranspMode = kRI32TT8; break;
                default: break;
               }
             } break;
    default: throw OdError(eNotApplicable);
  };
  m_pRasterImagePtr = pImage;
  if (m_rasterBitDepth <= kRIBD256Clr)
  {
    OdUInt32 nClrs = pImage->numColors();
    m_rasterPalette.resize(sizeof(ODCOLORREF) * nClrs);
    ODCOLORREF *clRefs = (ODCOLORREF*)m_rasterPalette.asArrayPtr();
    for (OdUInt32 nClr = 0; nClr < nClrs; nClr++)
      clRefs[nClr] = OD_OPAQUE(pImage->color(nClr));
    if (m_bRasterTransparency)
      clRefs[pImage->transparentColor()] &= 0x00FFFFFF;
    m_pRasterPalette = clRefs;
  }
  m_pRasterScanLines = pImage->scanLines();
  if (!m_pRasterScanLines)
  {
    m_rasterScanLines.resize(m_rasterHeight * m_rasterScanLineLength);
    pImage->scanLines(m_rasterScanLines.asArrayPtr(), 0, m_rasterHeight);
    m_pRasterScanLines = m_rasterScanLines.asArrayPtr();
  }
}

void OdGsBitmapVectorizeDevice::setRasterImage(const OdGiRasterImage *pImage, bool bTransparent,
                                               const OdGePoint3d &rasterOrigin, const OdGeVector3d &rasterU, const OdGeVector3d &rasterV)
{
  setRasterImage(pImage, bTransparent);

  OdGeMatrix3d world2uvInvU;
  OdGeVector3d normal = rasterU.crossProduct(rasterV);
  world2uvInvU.setCoordSystem(rasterOrigin, rasterU, rasterV, normal);
  world2uvInvU.invert();
  OdGeMatrix3d uv2uvNorm;
  uv2uvNorm.setToScaling(OdGeScale3d(1.0 / m_rasterWidth, 1.0 / m_rasterHeight, 1.0));
  m_rasterTransform = uv2uvNorm * world2uvInvU;
}

void OdGsBitmapVectorizeDevice::clearRasterImage()
{
  m_pRasterPalette = NULL;
  m_rasterPalette.clear();
  m_pRasterScanLines = NULL;
  m_rasterScanLines.clear();
  m_pRasterImagePtr.release();
}

void OdGsBitmapVectorizeDevice::setNRCClipping(long offsetX, long offsetY, OdUInt32 nrcWidth, OdUInt32 nrcHeight, OdUInt8 *pBuffer)
{
  m_nrcOffsetX = offsetX; m_nrcOffsetY = offsetY;
  m_nrcWidth = nrcWidth; m_nrcHeight = nrcHeight;
  m_pnrcClip = pBuffer;
}

void OdGsBitmapVectorizeDevice::resetNRCClipping()
{
  m_pnrcClip = NULL;
}

// OdGsBitmapVectorizeView

OdGsBitmapVectorizeView::OdGsBitmapVectorizeView()
  : m_nNRCWidth(0)
  , m_nNRCHeight(0)
  , m_nNRCOffsetX(0)
  , m_nNRCOffsetY(0)
  , m_bClipStage(false)
  , m_bLwdPoints(true)
  , m_defLesStyle(OdPs::kLesRound)
  , m_defLjsStyle(OdPs::kLjsRound)
  , m_bDefLwdStyleDef(true)
  , m_bLinesAntiAlias(false)
  , m_bDrawTransparency(true)
  , m_bLwdOverride(false)
  , m_pColorConverter(NULL)
{
  OdGeDoubleArray devs(5, 1);
  devs.insert(devs.end(), 5, .5);
  OdGiGeometrySimplifier::setDeviation(devs);

  m_pZClip = OdGiOrthoClipper::createObject();
  m_pZClip->input().addSourceNode(output());

  m_pPerspPrep = OdGiPerspectivePreprocessor::createObject();
  m_pPerspPrep->setDeviation(&m_pModelToEyeProc->eyeDeviation());
  m_pPerspPrep->input().addSourceNode(m_pZClip->output());

  m_pPerspXform = OdGiXform::createObject();
  m_pPerspXform->input().addSourceNode(m_pPerspPrep->output());

  m_pHLR = OdGiHLRemover::createObject();
  m_pHLR->setDeviation(devs);
  m_pHLR->output().setDestGeometry(*this);
  m_pHLR->input().addSourceNode(m_pPerspXform->output());
}

OdGsBitmapVectorizeView::~OdGsBitmapVectorizeView()
{
}

void OdGsBitmapVectorizeView::setMode(OdGsView::RenderMode mode) // NB: same with ExGDIVectorizeView::setMode :(
{
  if (((mode == kHiddenLine) && (!enabledSoftwareHLR())) || (mode > kHiddenLine))
    mode = kWireframe;
  OdGsBaseVectorizeView::setMode(mode);
}

void OdGsBitmapVectorizeView::beginViewVectorization()
{
  OdGsBaseVectorizeViewDef::beginViewVectorization();

  if (isOutputSuppressed())
    return;

  // initialize lineweight settings
  updateLineweightOverride(OdGiLineweightOverride());
  // initialize anti-aliasing settings
  m_bLinesAntiAlias = GETBIT(giContext().antiAliasingMode(), 1) && (view().mode() == k2DOptimized);
  // initialize transparency
  m_bDrawTransparency = view().isPlotTransparency() || !giContext().isPlotGeneration();
  // initialize optional color converter
  m_pColorConverter = device()->getColorConverter();
  // setting draw context to simplifier
  OdGiGeometrySimplifier::setDrawContext(OdGsBaseVectorizeViewDef::drawContext());
  m_pZClip->setDrawContext(OdGsBaseVectorizeViewDef::drawContext());
  m_pPerspPrep->setDrawContext(OdGsBaseVectorizeViewDef::drawContext());
  m_pHLR->setDrawContext(OdGsBaseVectorizeViewDef::drawContext());
  // setting conveyor transforms
  bool bFrontClip = isFrontClipped(), bBackClip = isBackClipped();
  double fFrontClip = frontClip(), fBackClip = backClip();
  // enable perspective transform
  if (view().isPerspective())
  {
    if (bFrontClip)
      fFrontClip = odmin(fFrontClip, focalLength() * 0.9);
    else
      fFrontClip = focalLength() * 0.9;
    bFrontClip = true;
    m_pPerspPrep->enable(true);  
  }
  else
    m_pPerspPrep->enable(false);  
  // setup z-clipping
  m_pZClip->set(0, NULL, bBackClip, fBackClip, bFrontClip, fFrontClip);
  m_pZClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
  // setup geometry transforms
  OdGeMatrix3d xEyeToDc = eyeToScreenMatrix();
  if (bFrontClip || bBackClip)
  {
    setEyeToOutputTransform(OdGeMatrix3d::kIdentity);
    m_pPerspXform->setTransform(xEyeToDc);
  }
  else
  {
    setEyeToOutputTransform(xEyeToDc);
    m_pPerspXform->setTransform(OdGeMatrix3d::kIdentity);
  }
  // setup hidden lines remover
  m_pHLR->enable((renderMode() == kHiddenLine) && enabledSoftwareHLR());
  m_pHLR->invertZ(true);
}

void OdGsBitmapVectorizeView::endViewVectorization()
{
  if (!isOutputSuppressed())
  {
    if((renderMode() == kHiddenLine) && enabledSoftwareHLR())
      m_pHLR->process(); // calculate the result of hidden line removing
    m_bLinesAntiAlias = false;
  }

  OdGsBaseVectorizeViewDef::endViewVectorization();
}

// AMark : #6939 : fix contour self-intersections
#include "GsNrcGpcConverter.cpp"

void OdGsBitmapVectorizeView::getViewportClipRect(OdGsDCRect &pRect)
{
  OdGePoint2d lowerLeft, upperRight;
  OdGsDCRect vRect2;
  pRect = device()->outputRect();
  getViewport(lowerLeft, upperRight);
  getViewport(vRect2);
  long nWidth = Od_abs(pRect.m_max.x - pRect.m_min.x);
  long nHeight = Od_abs(pRect.m_min.y - pRect.m_max.y);
  bool bNegatedY = pRect.m_max.y < pRect.m_min.y;
  pRect.m_min.x = OdRoundToLong(lowerLeft.x * nWidth);
  if (bNegatedY)
    pRect.m_min.y = nHeight - OdRoundToLong(lowerLeft.y * nHeight);
  else
    pRect.m_min.y = OdRoundToLong(lowerLeft.y * nHeight);
  pRect.m_max.x = OdRoundToLong(upperRight.x * nWidth);
  if (bNegatedY)
    pRect.m_max.y = nHeight - OdRoundToLong(upperRight.y * nHeight);
  else
    pRect.m_max.y = OdRoundToLong(upperRight.y * nHeight);
  if (pRect.m_min.x > pRect.m_max.x)
    std::swap(pRect.m_min.x, pRect.m_max.x);
  if (pRect.m_min.y > pRect.m_max.y)
    std::swap(pRect.m_min.y, pRect.m_max.y);
  if (vRect2.m_min.x > vRect2.m_max.x)
    std::swap(vRect2.m_min.x, vRect2.m_max.x);
  if (vRect2.m_min.y > vRect2.m_max.y)
    std::swap(vRect2.m_min.y, vRect2.m_max.y);
  pRect.intersectWith(vRect2, false);
  std::swap(pRect.m_min.y, pRect.m_max.y);
}

void OdGsBitmapVectorizeView::loadViewport()
{
  if (device()->numViews() > 1)
  {
    OdGsDCRect rcRect;
    getViewportClipRect(rcRect);
    device()->setClipRegion(rcRect);
    // initialize NRC viewport clipping
    if (isNonRectClipped())
    {
      rcRect = device()->clipRegion();
      long intOffsetX = rcRect.m_min.x, intOffsetY = rcRect.m_min.y;
      OdUInt32 intWidth = 0, intHeight = 0;
      if (rcRect.m_max.x > rcRect.m_min.x && rcRect.m_max.y > rcRect.m_min.y)
      {
        intWidth = OdUInt32(rcRect.m_max.x - rcRect.m_min.x);
        intHeight = OdUInt32(rcRect.m_max.y - rcRect.m_min.y);
      }
      if (intWidth == 0 || intHeight == 0)
      {
        m_pNRCScanlines.setPhysicalLength(0);
      }
      else
      {
        intWidth++; intHeight++;
        if (m_nNRCWidth != intWidth || m_nNRCHeight != intHeight)
        {
          m_pNRCScanlines.resize(intWidth * intHeight);
          m_nNRCWidth = intWidth; m_nNRCHeight = intHeight;
        }
        m_nNRCOffsetX = intOffsetX; m_nNRCOffsetY = intOffsetY;
      }
    }
  }
  if (!m_pNRCScanlines.empty())
  {
    // render clipping contour
    ::memset(m_pNRCScanlines.asArrayPtr(), 0, m_pNRCScanlines.size() * sizeof(OdUInt8));
    device()->setNRCClipping(m_nNRCOffsetX, m_nNRCOffsetY, m_nNRCWidth, m_nNRCHeight, m_pNRCScanlines.asArrayPtr());
    OdGePoint2dArray clipPoints;
    OdIntArray ptCounts;
    viewportClipRegion(ptCounts, clipPoints);
    gpc_polygon poly = OdGsNrcGpcConverter::getNrcClipAsGpcPolygon(ptCounts.size(), ptCounts.getPtr(), clipPoints);
    OdGsNrcGpcConverter::getGpcPolygonAsNrcClip(&poly, ptCounts, clipPoints);
    ::gpc_free_polygon(&poly);
    int numVerts;
    OdGePoint3dArray newVerts(clipPoints.size());
    OdGiFillType prevType = subEntityTraits().fillType();
    OdGsView::RenderMode prevMode = OdGiGeometrySimplifier::m_renderMode;
    OdGiGeometrySimplifier::m_renderMode = k2DOptimized;
    if (prevType != kOdGiFillAlways)
      subEntityTraits().setFillType(kOdGiFillAlways);
    m_bClipStage = true;
    bool bApplyMerge = ptCounts.size() > 1;
    for (int j = 0, i = 0; j < (int)ptCounts.size(); j++)
    {
      numVerts = ptCounts[j];
      newVerts.resize(numVerts);
      int i0 = i;
      for (; i < i0 + numVerts; i++)
        newVerts[i - i0].set(clipPoints[i].x, clipPoints[i].y, 1.0);//.transformBy(pixels2DxCoords);
      m_curColor = 0;
      polygonOut(numVerts, newVerts.asArrayPtr());
      if (bApplyMerge)
      {
        OdUInt8 *pMergeBuf = m_pNRCScanlines.asArrayPtr();
        OdUInt32 size = m_pNRCScanlines.size();
        while (size)
        {
          if (*pMergeBuf == 3)
            *pMergeBuf = 0;
          else if (*pMergeBuf == 2)
            *pMergeBuf = 1;
          pMergeBuf++;
          size--;
        }
      }
    }
    m_bClipStage = false;
    if (prevType != kOdGiFillAlways)
      subEntityTraits().setFillType(prevType);
    OdGiGeometrySimplifier::m_renderMode = prevMode;
    //OdUInt8 *pNR = m_pNRCScanlines.asArrayPtr();
    //for (OdUInt32 i = 0; i < m_pNRCScanlines.size(); i++, pNR++)
    //  *pNR = (OdUInt8)(i % 2);
  }
  else
  {
    device()->resetNRCClipping();
  }
}

void OdGsBitmapVectorizeView::drawViewportFrame()
{
  OdSaveState<bool> _laaState(m_bLinesAntiAlias, false);
  if (enabledSoftwareHLR() && (mode() == kHiddenLine) && m_pHLR->enabled())
  {
    m_pHLR->process();
    m_pHLR->enable(false);
  }
  device()->resetNRCClipping();
  device()->setClipRegion(device()->outputRect());
  OdGsBaseVectorizeViewDef::drawViewportFrame();
}

void OdGsBitmapVectorizeView::onTraitsModified()
{
  OdGsBaseVectorizeViewDef::onTraitsModified();
  if (isOutputSuppressed())
    return;
  if (!isSupportPlotStyles())
  {
    const OdGiSubEntityTraitsData &entTraits = effectiveTraits();
    const OdCmEntityColor &color = entTraits.trueColor();
    const OdCmTransparency &trans = entTraits.transparency();
    if (color.isByACI() || color.isByDgnIndex())
      m_curColor = OD_TRANSPARENT(paletteColor(color.colorIndex()), (m_bDrawTransparency) ? trans.alpha() : 0xFF);
    else
      m_curColor = OD_TRANSPARENT(ODTOCOLORREF(color), (m_bDrawTransparency) ? trans.alpha() : 0xFF);
    if (!m_bLwdOverride)
      m_curLineweight = lineweightToPixels(entTraits.lineWeight());
    m_curLesStyle = m_defLesStyle; m_curLjsStyle = m_defLjsStyle; m_bCurLwdStyleDef = m_bDefLwdStyleDef;
  }
  else
  {
    const OdPsPlotStyleData& plotStyle = effectivePlotStyle();
    const OdCmEntityColor &color = plotStyle.color();
    if (color.isByACI() || color.isByDgnIndex())
      m_curColor = OD_TRANSPARENT(paletteColor(color.colorIndex()), (m_bDrawTransparency) ? effectiveTraits().transparency().alpha() : 0xFF);
    else
      m_curColor = OD_TRANSPARENT(ODTOCOLORREF(color), (m_bDrawTransparency) ? effectiveTraits().transparency().alpha() : 0xFF);
    if (!m_bLwdOverride)
      m_curLineweight = (int)OdTruncateToLong(lineweightToPixels(plotStyle.lineweight()));
    m_curLesStyle = plotStyle.endStyle(); m_curLjsStyle = plotStyle.joinStyle();
    if (m_curLesStyle == OdPs::kLesUseObject) m_curLesStyle = m_defLesStyle;
    if (m_curLjsStyle == OdPs::kLjsUseObject) m_curLjsStyle = m_defLjsStyle;
    m_bCurLwdStyleDef = (m_curLesStyle == OdPs::kLesRound) && (m_curLjsStyle == OdPs::kLjsRound);
  }
  if (m_pColorConverter)
  {
    if (m_pColorConverter->convertBackgroundColors() || !GETBIT(effectiveTraits().drawFlags(), OdGiSubEntityTraits::kDrawNoPlotstyle))
      m_curColor = m_pColorConverter->convert(m_curColor);
  }
}

void OdGsBitmapVectorizeView::updateLineweightOverride(const OdGiLineweightOverride &lwdOverride)
{
  OdGiBaseVectorizerImpl::updateLineweightOverride(lwdOverride);
  if (lwdOverride.hasStyleOverride())
  {
    m_defLesStyle = lwdOverride.endStyleOverride();
    m_defLjsStyle = lwdOverride.joinStyleOverride();
  }
  else
  {
    m_bLwdPoints = giContext().lineWeightConfiguration(OdGiContext::kPointLineWeight) != 0;
    m_defLesStyle = (OdPs::LineEndStyle)giContext().lineWeightConfiguration(OdGiContext::kLineCapStyle);
    m_defLjsStyle = (OdPs::LineJoinStyle)giContext().lineWeightConfiguration(OdGiContext::kLineJoinStyle);
  }
  m_bDefLwdStyleDef = (m_defLesStyle == OdPs::kLesRound) && (m_defLjsStyle == OdPs::kLjsRound);
}

void OdGsBitmapVectorizeView::polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                                            const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                                            const OdGeVector3d* pNormals, const OdGeVector3d* pExtrusions,
                                            const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize)
{
  class VVLwdOverride
  {
    bool m_bOverride;
    bool m_bLwdOverride, *m_pLwdOverride;
    bool m_bLwdPoints, *m_pLwdPoints;
    int m_curLineweight, *m_pCurLineweight;
    public:
      VVLwdOverride(bool bOverride, int nNewLineweight, bool *pLwdOverride, bool *pLwdPoints, int *pCurLineweight)
        : m_bOverride(bOverride)
        , m_bLwdOverride(*pLwdOverride), m_pLwdOverride(pLwdOverride)
        , m_bLwdPoints(*pLwdPoints), m_pLwdPoints(pLwdPoints)
        , m_curLineweight(*pCurLineweight), m_pCurLineweight(pCurLineweight)
      {
        if (m_bOverride)
        {
          *m_pLwdOverride = true;
          *m_pLwdPoints = true;
          *m_pCurLineweight = nNewLineweight;
        }
      }
      ~VVLwdOverride()
      {
        if (m_bOverride)
        {
          *m_pLwdOverride = m_bLwdOverride;
          *m_pLwdPoints = m_bLwdPoints;
          *m_pCurLineweight = m_curLineweight;
        }
      }
  } _vvOr(nPointSize >= 1, (int)nPointSize, &m_bLwdOverride, &m_bLwdPoints, &m_curLineweight);
  OdGiGeometrySimplifier::polypointProc(numPoints, vertexList, pColors, pTransparency, pNormals, pExtrusions, pSubEntMarkers, nPointSize);
}

// graphics data
// required by simplifier
void OdGsBitmapVectorizeView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  if( nPoints == 2 && pPoints[0].isEqualTo(pPoints[1]) )
    nPoints = 1;

  if (nPoints > 1)
  {
    if (m_curLineweight < 2 || m_bCurLwdStyleDef)
    {
      if (!m_bLinesAntiAlias || m_curLineweight >= 2)
      {
        int oldx((int)OdRound(pPoints[0].x)), oldy((int)OdRound(pPoints[0].y)), newx, newy;
        for (OdInt32 i = 0; i < nPoints - 1; i++)
        {
          newx = (int)OdRound(pPoints[i+1].x);
          newy = (int)OdRound(pPoints[i+1].y);
          if (m_curLineweight < 2)
            device()->draw_line(oldx, oldy, newx, newy, m_curColor);
          else
            device()->draw_lwLine(oldx, oldy, newx, newy, m_curLineweight, m_curColor);
          oldx = newx;
          oldy = newy;
        }
      }
      else
      {
        for (OdInt32 i = 0; i < nPoints - 1; i++)
          device()->draw_lineAntiAlias(pPoints[i].x, pPoints[i].y, pPoints[i + 1].x, pPoints[i + 1].y, m_curColor);
      }
    }
    else
    {
      // Run through LineWeight generator
      class OdExBitmapExtendedLineWeightStyleClient : public OdExExtendedLineWeightStyleClient
      {
        OdGsBitmapVectorizeDevice *pOutputDevice;
        ODCOLORREF curColor;
        public:
          OdExBitmapExtendedLineWeightStyleClient(double lwd, OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle, OdGsBitmapVectorizeDevice *pDevice, ODCOLORREF color)
            : OdExExtendedLineWeightStyleClient(OdGeVector3d::kZAxis, lwd, capStyle, joinStyle), pOutputDevice(pDevice), curColor(color)
          {
          }
          virtual bool elsPolygonOut(const OdGePoint3d *pPoints, OdUInt32 nPoints)
          {
            int polyPoints[4][2];
            for (OdUInt32 n = 0; n < nPoints; n++)
            {
              polyPoints[n][0] = (int)OdRound(pPoints[n].x);
              polyPoints[n][1] = (int)OdRound(pPoints[n].y);
            }
            pOutputDevice->draw_triangle(polyPoints[0][0], polyPoints[0][1], polyPoints[1][0], polyPoints[1][1],
                                         polyPoints[2][0], polyPoints[2][1], curColor);
            if (nPoints == 4)
              pOutputDevice->draw_triangle(polyPoints[0][0], polyPoints[0][1], polyPoints[2][0], polyPoints[2][1],
                                           polyPoints[3][0], polyPoints[3][1], curColor);
            return true;
          }
          virtual bool elsCircleOut(const OdGePoint3d &pPoint, const OdGeVector3d & /*normal*/, double radius)
          {
            pOutputDevice->draw_filled_circle((int)OdRound(pPoint.x), (int)OdRound(pPoint.y), radius, curColor);
            return true;
          }
      } _exlwd_renderer((double)m_curLineweight, m_curLesStyle, m_curLjsStyle, device(), m_curColor);
      _exlwd_renderer.process(pPoints, (OdUInt32)nPoints);
    }
  }
  else if (nPoints == 1)
  {
    if (!m_bLwdPoints || m_curLineweight < 2)
      device()->draw_point((int)OdRound(pPoints[0].x), (int)OdRound(pPoints[0].y), m_curColor);
    else
      device()->draw_filled_circle((int)OdRound(pPoints[0].x), (int)OdRound(pPoints[0].y), double(m_curLineweight) * 0.5, m_curColor);
  }
}

void OdGsBitmapVectorizeView::triangleOut( const OdInt32* p3Vertices, const OdGeVector3d* /*pNormal*/ )
{
#if 0
  device()->draw_triangle(
    (int)OdRound(vertexDataList()[p3Vertices[0]].x), 
    (int)OdRound(vertexDataList()[p3Vertices[0]].y), 
    (int)OdRound(vertexDataList()[p3Vertices[1]].x), 
    (int)OdRound(vertexDataList()[p3Vertices[1]].y), 
    (int)OdRound(vertexDataList()[p3Vertices[2]].x), 
    (int)OdRound(vertexDataList()[p3Vertices[2]].y), 
    m_curColor);
#else
  if (m_bClipStage)
  {
    device()->draw_clip_triangle(
      (int)OdRound(vertexDataList()[p3Vertices[0]].x),
      (int)OdRound(vertexDataList()[p3Vertices[0]].y),
      (int)OdRound(vertexDataList()[p3Vertices[1]].x),
      (int)OdRound(vertexDataList()[p3Vertices[1]].y),
      (int)OdRound(vertexDataList()[p3Vertices[2]].x),
      (int)OdRound(vertexDataList()[p3Vertices[2]].y));
  }
  else if (vertexData() && vertexData()->trueColors())
  {
    // Gouraud triangle
    device()->draw_gouraud_triangle(
      (int)OdRound(vertexDataList()[p3Vertices[0]].x),
      (int)OdRound(vertexDataList()[p3Vertices[0]].y),
      OD_OPAQUE(ODTOCOLORREF(vertexData()->trueColors()[p3Vertices[0]])),
      (int)OdRound(vertexDataList()[p3Vertices[1]].x),
      (int)OdRound(vertexDataList()[p3Vertices[1]].y),
      OD_OPAQUE(ODTOCOLORREF(vertexData()->trueColors()[p3Vertices[1]])),
      (int)OdRound(vertexDataList()[p3Vertices[2]].x),
      (int)OdRound(vertexDataList()[p3Vertices[2]].y),
      OD_OPAQUE(ODTOCOLORREF(vertexData()->trueColors()[p3Vertices[2]])), ODGETALPHA(m_curColor));
  }
  else
  {
    if (!device()->hasRasterImage())
    {
      // Flat triangle
      device()->draw_triangle(
        (int)OdRound(vertexDataList()[p3Vertices[0]].x),
        (int)OdRound(vertexDataList()[p3Vertices[0]].y),
        (int)OdRound(vertexDataList()[p3Vertices[1]].x),
        (int)OdRound(vertexDataList()[p3Vertices[1]].y),
        (int)OdRound(vertexDataList()[p3Vertices[2]].x),
        (int)OdRound(vertexDataList()[p3Vertices[2]].y),
        m_curColor);
    }
    else
    {
      // Textured triangle
      device()->draw_textured_triangle(
        (int)OdRound(vertexDataList()[p3Vertices[0]].x),
        (int)OdRound(vertexDataList()[p3Vertices[0]].y),
        vertexDataList()[p3Vertices[0]],
        (int)OdRound(vertexDataList()[p3Vertices[1]].x),
        (int)OdRound(vertexDataList()[p3Vertices[1]].y),
        vertexDataList()[p3Vertices[1]],
        (int)OdRound(vertexDataList()[p3Vertices[2]].x),
        (int)OdRound(vertexDataList()[p3Vertices[2]].y),
        vertexDataList()[p3Vertices[2]], ODGETALPHA(m_curColor));
    }
  }
#endif
}

void OdGsBitmapVectorizeView::initTexture(const OdGePoint3d& origin,
                                          const OdGeVector3d& u,
                                          const OdGeVector3d& v,
                                          const OdGiRasterImage* pImage,
                                          bool  transparency,
                                          double brightness,
                                          double contrast,
                                          double fade)
{
  ODA_ASSERT(pImage != 0);
  bool bTransparentBitonal = false;
  bool bTransparent32 = false;
  OdGiRasterImagePtr pBitonalImg;
  if (pImage->colorDepth() == 1)
  {
    if (transparency)
      bTransparentBitonal = true;
    OdSmartPtr<OdGiBitonalRasterTransformer> pBitRast = OdRxObjectImpl<OdGiBitonalRasterTransformer>::createObject();
    const OdGiSubEntityTraitsData &effTraits = effectiveTraits();
    if (bTransparentBitonal || !GETBIT(effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill))
    {
      pBitRast->setOriginal(pImage, m_curColor, device()->getPalette()[0], bTransparentBitonal);
    }
    else
    {
      // Teigha(R) for .dgn files specific
      ODCOLORREF secColor;
      if(effTraits.secondaryTrueColor().isByACI() || effTraits.secondaryTrueColor().isByDgnIndex())
        secColor = OD_OPAQUE(paletteColor(effTraits.secondaryTrueColor().colorIndex()));
      else
        secColor = OD_OPAQUE(ODTOCOLORREF(effTraits.secondaryTrueColor()));
      pBitRast->setOriginal(pImage, m_curColor, secColor, bTransparentBitonal);
    }
    pBitonalImg = pBitRast;
    pImage = pBitonalImg;
  }
  else if (pImage->colorDepth() == 32 && transparency)
  {
    bTransparent32 = true;
  }
  if (device()->isRasterImageProcessible(pImage) &&
      OdEqual(brightness, 50.0) && OdEqual(contrast, 50.0) && OdZero(fade))
  {
    device()->setRasterImage(pImage, bTransparentBitonal || bTransparent32, origin, u, v);
  }
  else
  {
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
    pDesc->setPixelWidth(pImage->pixelWidth());
    pDesc->setPixelHeight(pImage->pixelHeight());
    if (!bTransparentBitonal && !bTransparent32)
    {
      pDesc->setColorDepth(24);
      pDesc->pixelFormat().setBGR();
    }
    else
    {
      pDesc->setColorDepth(32);
      pDesc->pixelFormat().setBGRA();
    }
    OdGiRasterImagePtr pCopyImg;
    pCopyImg = pImage->convert(true, brightness, contrast, fade, device()->getPalette()[0], false, false, false, pDesc);
    device()->setRasterImage(pCopyImg, bTransparentBitonal || bTransparent32, origin, u, v);
  }
}

void OdGsBitmapVectorizeView::uninitTexture()
{
  device()->clearRasterImage();
}

void OdGsBitmapVectorizeView::generateMeshFaces(OdInt32 rows, OdInt32 columns, const OdGiFaceData* pFaceData)
{
  OdGiGeometrySimplifier::generateMeshFaces(rows, columns, pFaceData);

  // #11681
  if ((renderMode() == OdGsView::k2DOptimized) && !(vertexData() && vertexData()->trueColors()))
  {
    // edge properties are taken from faces
    generateMeshWires(rows, columns, 0, pFaceData);
  }
}

void OdGsBitmapVectorizeView::display(bool update)
{
  // Render background
  updateViewportProperties(OdGsProperties::kDeviceBackground);
  if (mode() != OdGsView::k2DOptimized)
  {
    OdGeMatrix3d *prevXformMat = NULL;
    if (view().isPerspective() || isBackClipped() || isFrontClipped())
    {
      m_pZClip->input().removeSourceNode(output());
      output().setDestGeometry(*this);
      prevXformMat = new OdGeMatrix3d(eyeToOutputTransform());
      setEyeToOutputTransform(eyeToScreenMatrix());
    }
    OdGiDrawFlagsHelper saveDrawFlag(subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
    displayViewportProperties(this, OdGsProperties::kDeviceBackground);
    if (prevXformMat)
    {
      m_pZClip->input().addSourceNode(output());
      setEyeToOutputTransform(*prevXformMat);
      delete prevXformMat;
    }
  }
  //
  OdGsBaseVectorizeViewDef::display(update);
}

void OdGsBitmapVectorizeView::directRenderOutputPoint(const OdGePoint3d &point, const OdCmEntityColor &color)
{
  device()->draw_point((int)OdRound(point.x), (int)OdRound(point.y), OD_OPAQUE(ODTOCOLORREF(color)));
}

void OdGsBitmapVectorizeView::directRenderOutputLineFlat(const OdGePoint3d &pt1, const OdGePoint3d &pt2,
                                                         const OdCmEntityColor &color)
{
  device()->draw_line((int)OdRound(pt1.x), (int)OdRound(pt1.y), (int)OdRound(pt2.x), (int)OdRound(pt2.y), OD_OPAQUE(ODTOCOLORREF(color)));
}

void OdGsBitmapVectorizeView::directRenderOutputTriangleFlat(const OdGePoint3d &pt1, const OdGePoint3d &pt2, const OdGePoint3d &pt3,
                                                             const OdCmEntityColor &color)
{
  device()->draw_triangle((int)OdRound(pt1.x), (int)OdRound(pt1.y), (int)OdRound(pt2.x), (int)OdRound(pt2.y), (int)OdRound(pt3.x), (int)OdRound(pt3.y), OD_OPAQUE(ODTOCOLORREF(color)));
}

void OdGsBitmapVectorizeView::directRenderOutputTriangleGouraud(const OdGePoint3d &pt1, const OdGePoint3d &pt2, const OdGePoint3d &pt3,
                                                                const OdCmEntityColor &clr1, const OdCmEntityColor &clr2, const OdCmEntityColor &clr3)
{
  device()->draw_gouraud_triangle((int)OdRound(pt1.x), (int)OdRound(pt1.y), OD_OPAQUE(ODTOCOLORREF(clr1)),
                                  (int)OdRound(pt2.x), (int)OdRound(pt2.y), OD_OPAQUE(ODTOCOLORREF(clr2)),
                                  (int)OdRound(pt3.x), (int)OdRound(pt3.y), OD_OPAQUE(ODTOCOLORREF(clr3)));
}

void OdGsBitmapVectorizeView::directRenderOutputPolygoneFlat(OdUInt32 nPts, const OdGePoint3d *pPts,
                                                             const OdCmEntityColor &color)
{
  for (OdUInt32 i = 2; i < nPts; i++)
  {
    directRenderOutputTriangleFlat(pPts[0], pPts[i - 1], pPts[i], color);
  }
}

void OdGsBitmapVectorizeView::directRenderOutputPolygoneGouraud(OdUInt32 nPts, const OdGePoint3d *pPts,
                                                                const OdCmEntityColor *pClrs)
{
  for (OdUInt32 i = 2; i < nPts; i++)
  {
    directRenderOutputTriangleGouraud(pPts[0], pPts[i - 1], pPts[i], pClrs[0], pClrs[i - 1], pClrs[i]);
  }
}

void OdGsBitmapVectorizeView::directRenderOutputImage(const OdGePoint3d *pRect, const OdGiRasterImage *pImage,
                                                      const OdGsPropertiesDirectRenderOutput::DirectRenderImageParams &driParams)
{
  ODA_ASSERT(pImage != 0 && pRect != 0);
  const OdGsPropertiesDirectRenderOutput::DirectRenderImageUV driDefaultUVs[4] =
  {
    { 0.0, 0.0 },
    { 1.0, 0.0 },
    { 1.0, 1.0 },
    { 0.0, 1.0 }
  };
  const OdGsPropertiesDirectRenderOutput::DirectRenderImageUV *driUVs = (driParams.uvCoords) ? driParams.uvCoords : driDefaultUVs;
  // Load image
  if (device()->isRasterImageProcessible(pImage))
    device()->setRasterImage(pImage);
  else
  {
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
    pDesc->setPixelWidth(pImage->pixelWidth());
    pDesc->setPixelHeight(pImage->pixelHeight());
    pDesc->setColorDepth(24);
    pDesc->pixelFormat().setBGR();
    OdGiRasterImagePtr pCopyImg;
    pCopyImg = pImage->convert(true, 50.0, 50.0, 0.0, 0, false, false, false, pDesc);
    device()->setRasterImage(pCopyImg);
  }
  // Render image
  device()->draw_textured_triangle((int)OdRound(pRect[0].x), (int)OdRound(pRect[0].y), OdGePoint3d(driUVs[0].u, driUVs[0].v, 0.0),
                                   (int)OdRound(pRect[1].x), (int)OdRound(pRect[1].y), OdGePoint3d(driUVs[1].u, driUVs[1].v, 0.0),
                                   (int)OdRound(pRect[2].x), (int)OdRound(pRect[2].y), OdGePoint3d(driUVs[2].u, driUVs[2].v, 0.0),
                                   true, true);
  device()->draw_textured_triangle((int)OdRound(pRect[0].x), (int)OdRound(pRect[0].y), OdGePoint3d(driUVs[0].u, driUVs[0].v, 0.0),
                                   (int)OdRound(pRect[2].x), (int)OdRound(pRect[2].y), OdGePoint3d(driUVs[2].u, driUVs[2].v, 0.0),
                                   (int)OdRound(pRect[3].x), (int)OdRound(pRect[3].y), OdGePoint3d(driUVs[3].u, driUVs[3].v, 0.0),
                                   true, true);
  // Close image
  uninitTexture();
}

OdUInt32 OdGsBitmapVectorizeView::directRenderOutputFlags() const
{
  return OdGsPropertiesDirectRenderOutput::DirectRender_Point |
         OdGsPropertiesDirectRenderOutput::DirectRender_LineFlat |
         OdGsPropertiesDirectRenderOutput::DirectRender_TriangleFlat |
         OdGsPropertiesDirectRenderOutput::DirectRender_TriangleGouraud |
         OdGsPropertiesDirectRenderOutput::DirectRender_PolygoneFlat |
         OdGsPropertiesDirectRenderOutput::DirectRender_PolygoneGouraud |
         OdGsPropertiesDirectRenderOutput::DirectRender_Image;
}

// Snapshot support

void OdGsBitmapVectorizeView::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  device()->getSnapShot(pImage, region, this);
}

void OdGsBitmapVectorizeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  getSnapShot(pImage, region, NULL);
}

#include "SnapshotSupport.h"

void OdGsBitmapVectorizeDevice::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region, OdGsView *pView)
{
  struct SnapshotCallback : public OdGiSnapshotImageCallback
  {
    OdGiRasterImagePtr snapshotImageParams(OdGsBaseVectorizeDevice &runDevice) const
    {
      return &static_cast<OdGsBitmapVectorizeDevice&>(runDevice);
    }
    OdGiRasterImagePtr snapshotRegion(OdGsBaseVectorizeDevice &runDevice, const OdGsDCRect &area, bool bCrop) const
    {
      OdGsBitmapVectorizeDevice &device = static_cast<OdGsBitmapVectorizeDevice&>(runDevice);
      if (!device.width() || !device.height())
        return OdGiRasterImagePtr();
      OdGiRasterImagePtr pCrop = &device;
      if (bCrop)
        pCrop = device.crop(area.m_min.x, area.m_min.y, area.m_max.x - area.m_min.x, area.m_max.y - area.m_min.y);
      return pCrop;
    }
  } _cbk;
  OdGiSnapshotImageImpl::getSnapshot(*this, &_cbk, pImage, region, pView);
}

//
