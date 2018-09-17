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
// GLES2 device definitions

#ifndef ODTRVISDEVICEDEFINITIONS
#define ODTRVISDEVICEDEFINITIONS

#include "TD_PackPush.h"

#include "Ge/GePoint3d.h"
#include "Gs/Gs.h"

#ifndef OD_FORCEINLINE
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#define OD_FORCEINLINE __forceinline
#elif defined(__GNUC__) && (((__GNUC__ == 4) && (__GNUC_MINOR__ >= 4)) || (__GNUC__ > 4))
#define OD_FORCEINLINE inline __attribute__((always_inline))
#elif defined(__APPLE__) && !defined(__GNUC__)
#define OD_FORCEINLINE inline __attribute__((always_inline))
#else
#define OD_FORCEINLINE inline
#endif
#endif // OD_FORCEINLINE

#if (__cplusplus > 199711L) || (_MSC_VER > 1700)
#define OD_CPP0X_SUPPORTED
#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define OD_CPP0X_PARTIAL_SUPPORT
#endif
#endif

#ifndef OD_CONSTEXPR
#if defined(OD_CPP0X_SUPPORTED) && !defined(OD_CPP0X_PARTIAL_SUPPORT)
#define OD_CONSTEXPR_FUNC constexpr
#define OD_CONSTEXPR constexpr
#else
#define OD_CONSTEXPR_FUNC OD_FORCEINLINE
#define OD_CONSTEXPR
#endif
#endif // OD_CONSTEXPR

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes> 
*/
struct OdTrVisSyncFlags
{
  OdUInt32 m_syncFlags;
  OdTrVisSyncFlags() : m_syncFlags(0) { }
  void setSyncFlag(OdUInt32 flag) { SETBIT_1(m_syncFlags, flag); }
  bool isSyncFlagSet(OdUInt32 flag) const { return GETBIT(m_syncFlags, flag); }
  void resetSyncFlag(OdUInt32 flag) { SETBIT_0(m_syncFlags, flag); }
  void resetSyncFlags() { m_syncFlags = 0; }
  void setSyncAll() { m_syncFlags = 0xFFFFFFFF; }
  bool isSynchronized() const { return m_syncFlags == 0; }
};

// GsMarker

typedef OdUInt64 OdTrVisGsMarker;
typedef OdArray<OdTrVisGsMarker, OdMemoryAllocator<OdTrVisGsMarker> > OdTrVisGsMarkerArray;
const OdTrVisGsMarker kTrVisNullGsMarker = OdTrVisGsMarker(kNullSubentIndex);
inline bool isValidMarker(OdTrVisGsMarker marker)
{ return marker != kTrVisNullGsMarker; }

// Internal ID's

typedef OdUInt64 OdTrVisId;

typedef OdTrVisId OdTrVisViewportId;

typedef OdTrVisId OdTrVisMetafileId;
typedef OdUInt32  OdTrVisArrayId;

typedef OdTrVisId OdTrVisTextureId;
typedef OdTrVisId OdTrVisMaterialId;

typedef OdTrVisId OdTrVisProgramId;

typedef OdTrVisId OdTrVisDisplayId;

typedef OdTrVisId OdTrVisHlBranchId;

typedef OdTrVisId OdTrVisVisualStyleId;

typedef OdTrVisId OdTrVisLayerId;

typedef OdTrVisId OdTrVisOverlayId;

#define OdTrVisPtrToId(ptr) ((OdTrVisId)((OdIntPtr)ptr))
#define OdTrVisIdToPtr(ptrType, id) ((ptrType*)(OdIntPtr)(id))

const OdTrVisId kTrVisNegativeId = OdTrVisId(-1);
OD_FORCEINLINE bool isValidTrVisId(OdTrVisId id)
{
  return id != kTrVisNegativeId;
}

const OdTrVisId kTrVisNullId = OdTrVisId(0);
OD_FORCEINLINE bool isNullTrVisId(OdTrVisId id)
{
  return id != kTrVisNullId;
}

const OdTrVisId kTrVisMainOverlayId = kTrVisNullId;
OD_FORCEINLINE bool isMainTrVisOverlayId(OdTrVisId id)
{
  return id != kTrVisMainOverlayId;
}

const OdTrVisArrayId kTrVisNegativeArrayId = OdTrVisArrayId(-1);
OD_FORCEINLINE bool isValidTrVisArrayId(OdTrVisArrayId id)
{
  return id != kTrVisNegativeArrayId;
}

typedef OdArray<float, OdMemoryAllocator<float> > OdFloatArray;

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
struct OdTrVisColorRGBA
{
  OdUInt8 r, g, b, a;
  bool operator ==(const OdTrVisColorRGBA &sec) const
  { return (r == sec.r) && (g == sec.g) && (b == sec.b) && (a == sec.a); }
  bool operator !=(const OdTrVisColorRGBA &sec) const
  { return (r != sec.r) || (g != sec.g) || (b != sec.b) || (a != sec.a); }
  OdTrVisColorRGBA &set(OdUInt8 _r, OdUInt8 _g, OdUInt8 _b, OdUInt8 _a) {
    r = _r; g = _g; b = _b; a = _a; return *this; }
  OdTrVisColorRGBA &set_r(OdUInt8 _r) { r = _r; return *this; }
  OdTrVisColorRGBA &set_g(OdUInt8 _g) { g = _g; return *this; }
  OdTrVisColorRGBA &set_b(OdUInt8 _b) { b = _b; return *this; }
  OdTrVisColorRGBA &set_a(OdUInt8 _a) { a = _a; return *this; }
  OdTrVisColorRGBA &setCRef(ODCOLORREF color) { r = ODGETRED(color);  g = ODGETGREEN(color);
                                                b = ODGETBLUE(color); a = ODGETALPHA(color); return *this; }
  void setFlt(float _r, float _g, float _b, float _a) { r = (OdUInt8)(_r * 255); g = (OdUInt8)(_g * 255);
                                                        b = (OdUInt8)(_b * 255); a = (OdUInt8)(_a * 255); }
  void asFlt(float &_r, float &_g, float &_b, float &_a) const { _r = float(r) / 255; _g = float(g) / 255; _b = float(b) / 255; _a = float(a) / 255; }
  ODCOLORREF asCRef() const { return ODRGBA(r, g, b, a); }
  const OdUInt8 *asArray() const { return &r; }
  // Color representations
  enum ColorRep { kRGBA = 0, kBGRA, kARGB, kABGR, kRGBX, kBGRX, kXRGB, kXBGR };
  void setRep(const OdTrVisColorRGBA &clr, ColorRep rep, OdUInt8 alpha = 255)
  { switch (rep) {
      case kRGBA: *this = clr;                                break; case kBGRA: r = clr.b; g = clr.g; b = clr.r; a = clr.a; break;
      case kARGB: r = clr.g; g = clr.b; b = clr.a; a = clr.r; break; case kABGR: r = clr.a; g = clr.b; b = clr.g; a = clr.r; break;
      case kRGBX: r = clr.r; g = clr.g; b = clr.b; a = alpha; break; case kBGRX: r = clr.b; g = clr.g; b = clr.r; a = alpha; break;
      case kXRGB: r = clr.g; g = clr.b; b = clr.a; a = alpha; break; case kXBGR: r = clr.a; g = clr.b; b = clr.g; a = alpha; break;
  } }
  void getRep(OdTrVisColorRGBA &clr, ColorRep rep, OdUInt8 alpha = 255) const
  { switch (rep) {
      case kRGBA: clr = *this;                                        break; case kBGRA: clr.r = b;     clr.g = g; clr.b = r; clr.a = a;     break;
      case kARGB: clr.r = a;     clr.g = r; clr.b = g; clr.a = b;     break; case kABGR: clr.r = a;     clr.g = b; clr.b = g; clr.a = r;     break;
      case kRGBX: clr.r = r;     clr.g = g; clr.b = b; clr.a = alpha; break; case kBGRX: clr.r = b;     clr.g = g; clr.b = r; clr.a = alpha; break;
      case kXRGB: clr.r = alpha; clr.g = r; clr.b = g; clr.a = b;     break; case kXBGR: clr.r = alpha; clr.g = b; clr.b = g; clr.a = r;     break;
  } }
  OdTrVisColorRGBA getRep(ColorRep rep, OdUInt8 alpha = 255) const { OdTrVisColorRGBA getter; getRep(getter, rep, alpha); return getter; }
};

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
struct OdTrVisMatrix
{
  float entry[4][4];
};

typedef OdArray<OdTrVisMatrix, OdMemoryAllocator<OdTrVisMatrix> > OdTrVisMatrixArray;

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
struct OdTrVisSectionPlane
{
  float entry[3][3];
  OdTrVisSectionPlane()
  {
    entry[0][0] = entry[0][1] = entry[0][2] = 0.0f;
    entry[1][0] = entry[1][1] = 0.0f; entry[1][2] = 1.0f;
    entry[2][0] = entry[2][1] = entry[2][2] = 0.0f;
  }
  OdTrVisSectionPlane(OdGePoint3d position, OdGeVector3d normal)
  {
    set(position, normal);
  }
  void set(OdGePoint3d position, OdGeVector3d normal)
  {
    entry[0][0] = (float)position.x; entry[0][1] = (float)position.y; entry[0][2] = (float)position.z;
    entry[1][0] = (float)normal.x; entry[1][1] = (float)normal.y; entry[1][2] = (float)normal.z;
  }
  OdGePoint3d position() const { return OdGePoint3d(entry[0][0], entry[0][1], entry[0][2]); }
  OdGeVector3d normal() const { return OdGeVector3d(entry[1][0], entry[1][1], entry[1][2]); }
  void transformBy(const OdGeMatrix3d &xfm) { set(xfm * position(), xfm * normal()); }
};

typedef OdArray<OdTrVisSectionPlane, OdMemoryAllocator<OdTrVisSectionPlane> > OdTrVisSectionPlaneArray;

#define OD_TRVIS_OPAQUE(color)          ((color) | 0xFF000000)
#define OD_TRVIS_TRANSPARENT(color)     ((color) & 0x00FFFFFF)
#define OD_TRVIS_SETALPHA(color, alpha) (OD_TRVIS_TRANSPARENT(color) | (ODCOLORREF(alpha) << 24))

// FP comparison functions

OD_FORCEINLINE bool OdTrVisFPEqual(float a, float b, float tol = 1.e-8f)
{
  const float dif = a - b;
  return (dif < tol) && (dif > -tol);
}
OD_FORCEINLINE bool OdTrVisFPZero(float a, float tol = 1.e-8f)
{
  return !(a > tol) && !(a < -tol);
}

// Clamping

template <typename Type>
OD_FORCEINLINE Type &OdTrVisClamp(Type &val, Type valMin, Type valMax)
{ if (val < valMin) val = valMin; else if (val > valMax) val = valMax;
  return val; }
template <typename Type>
OD_FORCEINLINE const Type &OdTrVisClamp(const Type &val, Type valMin, Type valMax)
{ if (val < valMin) val = valMin; else if (val > valMax) val = valMax;
  return val; }

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
class OdTrVisDescStub : public OdRxClass
{
  public:
    OdRxObjectPtr create() const { return OdRxObjectPtr(); }
    const OdString appName() const { return OdString::kEmpty; }
    AppNameChangeFuncPtr appNameCallbackPtr() const { return NULL; }
    const OdString dxfName() const { return OdString::kEmpty; }
    const OdString name() const { return OdString::kEmpty; }
    OdDb::DwgVersion getClassVersion(OdDb::MaintReleaseVer* = 0) const { return OdDb::kDHL_CURRENT; }
    OdUInt32 proxyFlags() const { return 0; }
    OdUInt32 customFlags() const { return 0; }
    OdRxClass* myParent() const { return OdRxClass::desc(); }
    OdRxModule* module() const { return NULL; }
    void setConstructor(OdPseudoConstructorType) { }
	  OdRxOverruleIterator* getOverrule(OverrulingType t) const { return 0; }
	  void setOverrule(OdRxOverruleIterator* pIt, OverrulingType t) {}
};

#include "TD_PackPop.h"

#endif // ODTRVISDEVICEDEFINITIONS
