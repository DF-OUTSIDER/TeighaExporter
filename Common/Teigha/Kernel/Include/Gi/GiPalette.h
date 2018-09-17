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

#ifndef __ODGIPALETTE_H__
#define __ODGIPALETTE_H__

#include "TD_PackPush.h"

#include "GiExport.h"
#include "OdArray.h"
#include "StaticRxObject.h"

/** \details
    This class represents RGB color components as set of 32bit integers.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
struct OdGiIntRGB
{
  OdInt32 r, g, b;

  OdGiIntRGB(OdInt32 _r, OdInt32 _g, OdInt32 _b)
    : r(_r), g(_g), b(_b)
  { }
  OdGiIntRGB() { }
  OdGiIntRGB(ODCOLORREF cref)
  {
    setColor(cref);
  }

  /** \details
      Returns red color component.
  */
  OdInt32 red() const { return r; }
  /** \details
      Returns green color component.
  */
  OdInt32 green() const { return g; }
  /** \details
      Returns blue color component.
  */
  OdInt32 blue() const { return b; }

  /** \details
      Resets red color component.
      \param _r [in]  New value for red color component.
  */
  void setRed(OdInt32 _r) { r = _r; }
  /** \details
      Resets green color component.
      \param _g [in]  New value for green color component.
  */
  void setGreen(OdInt32 _g) { g = _g; }
  /** \details
      Resets blue color component.
      \param _b [in]  New value for blue color component.
  */
  void setBlue(OdInt32 _b) { b = _b; }
  /** \details
      Resets red, green and blue color components together.
      \param _r [in]  New value for red color component.
      \param _g [in]  New value for green color component.
      \param _b [in]  New value for blue color component.
  */
  void setRGB(OdInt32 _r, OdInt32 _g, OdInt32 _b) { r = _r; g = _g; b = _b; }

  /** \details
      Resets all color components from ODCOLORREF.
      \param cref [in]  Input color.
  */
  void setColor(ODCOLORREF cref)
  {
    setRGB((OdInt32)ODGETRED(cref), (OdInt32)ODGETGREEN(cref), (OdInt32)ODGETBLUE(cref));
  }
  /** \details
      Returns color as ODCOLORREF.
  */
  ODCOLORREF color() const
  {
    return ODRGB(r, g, b);
  }
};

/** \details
    This class represents RGB color cube.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiColorCube : public OdStaticRxObject<OdRxObject>
{
  protected:
    OdInt32    m_nBaseOffset;
    OdGiIntRGB m_nGridDivs;
    float      m_fIntensity;
    OdInt32    m_nGridSize;
    OdGiIntRGB m_nOffsets;
    OdGiIntRGB m_nDims;
  protected:
    virtual bool isDynamic() const { return false; }
  public:
    OdGiColorCube(OdGiIntRGB nGridDivs = OdGiIntRGB(2, 2, 2), float fIntensity = 1.0f, OdInt32 nBaseOffset = 0)
      : m_nBaseOffset(nBaseOffset)
      , m_nGridDivs(nGridDivs)
      , m_fIntensity(fIntensity)
    {
      validate();
    }
    static OdSmartPtr<OdGiColorCube> createDynamic(OdGiIntRGB nGridDivs = OdGiIntRGB(2, 2, 2), float fIntensity = 1.0f, OdInt32 nBaseOffset = 0);

    virtual OdRxObjectPtr clone() const;
    OdSmartPtr<OdGiColorCube> cloneIfNeed() const { if (!isDynamic()) return clone(); return this; }

    /** \details
        Returns base offset for color entries.
    */
    OdInt32 baseOffset() const { return m_nBaseOffset; }
    /** \details
        Returns color cube grid divisions count.
    */
    const OdGiIntRGB &gridDivisions() const { return m_nGridDivs; }
    /** \details
        Returns color cube intensity.
    */
    float intensity() const { return m_fIntensity; }
    /** \details
        Returns total number of color cube grid knots.
    */
    OdInt32 gridSize() const { return m_nGridSize; }
    /** \details
        Returns offsets of color components.
    */
    const OdGiIntRGB &offsets() const { return m_nOffsets; }
    /** \details
        Returns dimensions of color cube components.
    */
    const OdGiIntRGB &dimensions() const { return m_nDims; }

    /** \details
        Returns color for specified color cube knot.
        \param nColor [in]  Index of color cube knot.
    */
    ODCOLORREF color(OdInt32 nColor) const;

    /** \details
        Compute closest color cube knot index.
        \param cref [in]  Input color.
    */
    OdInt32 closestMatch(ODCOLORREF cref) const;

    struct DtMatchResult
    {
      OdInt32 m_fitColors[4];
      OdInt32 m_pattern[4];
    };

    /** \details
        Compute set of closest color cube knot indexes.
        \param cref [in]  Input color.
        \param results [out]  Set of output colors and pattern offsets.
        \returns
        Returns number of fit colors.
    */
    OdInt32 ditheredMatch(ODCOLORREF cref, DtMatchResult &results) const;
  protected:
    void validate();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiColorCube object pointers.
*/
typedef OdSmartPtr<OdGiColorCube> OdGiColorCubePtr;

/** \details
    This class represents shades of gray ramp.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiGrayRamp : public OdStaticRxObject<OdRxObject>
{
  protected:
    OdInt32 m_nBaseOffset;
    OdInt32 m_nGridDivs;
    float m_fIntensity;
    OdInt32 m_nDim;
  protected:
    virtual bool isDynamic() const { return false; }
  public:
    OdGiGrayRamp(OdInt32 nGridDivs = 2, float fIntensity = 1.0f, OdInt32 nBaseOffset = 0)
      : m_nBaseOffset(nBaseOffset)
      , m_nGridDivs(nGridDivs)
      , m_fIntensity(fIntensity)
    {
      validate();
    }
    static OdSmartPtr<OdGiGrayRamp> createDynamic(OdInt32 nGridDivs = 2, float fIntensity = 1.0f, OdInt32 nBaseOffset = 0);

    virtual OdRxObjectPtr clone() const;
    OdSmartPtr<OdGiGrayRamp> cloneIfNeed() const { if (!isDynamic()) return clone(); return this; }

    /** \details
        Returns base offset for color entries.
    */
    OdInt32 baseOffset() const { return m_nBaseOffset; }
    /** \details
        Returns gray ramp divisions count.
    */
    OdInt32 gridDivisions() const { return m_nGridDivs; }
    /** \details
        Returns gray ramp intensity.
    */
    float intensity() const { return m_fIntensity; }
    /** \details
        Returns total number of gray ramp subdivisions.
    */
    OdInt32 dimension() const { return m_nDim; }

    /** \details
        Returns color for specified gray ramp subdivision.
        \param nColor [in]  Index of gray ramp subdivision.
    */
    ODCOLORREF color(OdInt32 nColor) const;

    /** \details
        Compute closest gray ramp subdivision index.
        \param cref [in]  Input color.
    */
    OdInt32 closestMatch(ODCOLORREF cref) const;
  protected:
    void validate();
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiGrayRamp object pointers.
*/
typedef OdSmartPtr<OdGiGrayRamp> OdGiGrayRampPtr;

/** \details
    This class represents 256 colors palette.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiPalette : public OdStaticRxObject<OdRxObject>
{
  protected:
    ODCOLORREF       m_colors[256];
    OdUInt64         m_activities[4];
    OdGiColorCubePtr m_pColorCube;
    OdGiGrayRampPtr  m_pGrayRamp;
  public:
    struct Addressation
    {
      OdInt32 m_nSrcFrom;
      OdInt32 m_nDstTo;
      OdInt32 m_nNumColors;
      Addressation(OdInt32 nFrom = 0, OdInt32 nTo = 0, OdInt32 nNum = 256)
        : m_nSrcFrom(nFrom), m_nDstTo(nTo), m_nNumColors(nNum)
      {}
      bool validate() const { return ((m_nSrcFrom + m_nNumColors) <= 256) && ((m_nDstTo + m_nNumColors) <= 256); }
      bool fullRange() const { return (m_nSrcFrom == 0) && (m_nDstTo == 0) && (m_nNumColors == 256); }
    };
  protected:
    virtual bool isDynamic() const { return false; }
  public:
    OdGiPalette()
    {
      initPalette();
    }
    static OdSmartPtr<OdGiPalette> createDynamic();

    virtual OdRxObjectPtr clone() const;
    OdSmartPtr<OdGiPalette> cloneIfNeed() const { if (!isDynamic()) return clone(); return this; }

    /** \details
        Returns palette color.
        \param nColor [in]  Color index.
    */
    ODCOLORREF color(OdInt32 nColor) const
    {
      return m_colors[nColor];
    }
    /** \details
        Returns palette entry activity flag.
        \param nColor [in]  Color index.
    */
    bool entryActivity(OdInt32 nColor) const
    {
      return GETBIT(m_activities[nColor >> 6], 1ULL << (OdUInt64(nColor) & 63));
    }

    /** \details
        Sets palette color.
        \param nColor [in]  Color index.
        \param color [in]  Input color.
    */
    void setColor(OdInt32 nColor, ODCOLORREF color)
    {
      m_colors[nColor] = color;
    }
    /** \details
        Sets palette entry activity flag.
        \param nColor [in]  Color index.
        \param entryHelper [in]  New palette entry helper value.
    */
    void setEntryActivity(OdInt32 nColor, bool bActivity)
    {
      SETBIT(m_activities[nColor >> 6], 1ULL << (OdUInt64(nColor) & 63), bActivity);
    }

    /** \details
        Returns palette colors as array pointer.
    */
    const ODCOLORREF *asArray() const
    {
      return m_colors;
    }

    /** \details
        Returns color cube linked with this palette.
    */
    const OdGiColorCube *colorCube() const
    {
      return m_pColorCube.get();
    }
    /** \details
        Link color cube with this palette.
    */
    void setColorCube(const OdGiColorCube *pColorCube)
    {
      resetColorCube(pColorCube);
    }
    /** \details
        Reset linkage of color cube with this palette.
    */
    void resetColorCube()
    {
      resetColorCube(NULL);
    }

    /** \details
        Returns gray ramp linked with this palette.
    */
    const OdGiGrayRamp *grayRamp() const
    {
      return m_pGrayRamp.get();
    }
    /** \details
        Link gray ramp with this palette.
    */
    void setGrayRamp(const OdGiGrayRamp *pGrayRamp)
    {
      resetGrayRamp(pGrayRamp);
    }
    /** \details
        Reset linkage of gray ramp with this palette.
    */
    void resetGrayRamp()
    {
      resetGrayRamp(NULL);
    }

    /** \details
        Setup palette for specified color cube.
        \param pColorCube [in]  Input color cube.
        \param bForceUpdate [in]  Update palette color even if it is already set.
    */
    bool install(const OdGiColorCube *pColorCube, bool bForceUpdate = false);
    /** \details
        Setup palette for specified gray ramp.
        \param pGrayRamp [in]  Input gray ramp.
        \param bForceUpdate [in]  Update palette color even if it is already set.
    */
    bool install(const OdGiGrayRamp *pGrayRamp, bool bForceUpdate = false);
    /** \details
        Setup palette as a clone of another palette.
        \param pPalette [in]  Input palette.
        \param address [in]  Palette insertion offsets.
    */
    bool install(const OdGiPalette *pPalette, bool bForceUpdate = false, const Addressation &address = Addressation());
    /** \details
        Setup palette from specified palette array.
        \param pColors [in]  Array of palette colors.
        \param bForceUpdate [in]  Update palette color even if it is already set.
        \param address [in]  Palette insertion offsets.
    */
    bool install(ODCOLORREF const *const pColors, bool bForceUpdate = false, const Addressation &address = Addressation());

    /** \details
        Compute closest palette color index.
        \param cref [in]  Input color.
        \param bThroughPal [in]  Skip usage of linked color cube and gray ramp in calculations.
    */
    OdInt32 closestMatch(ODCOLORREF cref, bool bThroughPal = false) const;

    /** \details
        Find start index of first available free block in palette.
        \param blockSize [in]  Size of free block for search.
    */
    OdInt32 firstAvailableBlock(OdInt32 blockSize) const;

    /** \details
        Check does this palette is equal with another one.
        \param pPalette [in]  Palette for check.
        \param address [in]  Palette insertion offsets.
    */
    bool isEqualTo(const OdGiPalette *pPalette, const Addressation &address = Addressation()) const;
    /** \details
        Remove equal sub-palette from current palette.
        \param pPalette [in]  Palette for remove.
        \param address [in]  Palette insertion offsets.
    */
    bool remove(const OdGiPalette *pPalette, bool bForceUpdate = false, const Addressation &address = Addressation());

    /** \details
        Clear palette.
    */
    void clear();
  protected:
    void initPalette();

    void resetColorCube(const OdGiColorCube *pColorCube);
    void resetGrayRamp(const OdGiGrayRamp *pGrayRamp);

    void setColorImpl(OdInt32 nColor, ODCOLORREF color)
    {
      m_colors[nColor] = color;
      setEntryActivity(nColor, true);
    }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGiPalette object pointers.
*/
typedef OdSmartPtr<OdGiPalette> OdGiPalettePtr;

// Forward declaration
class OdGiSubEntityTraits;

/** \details
    Helper class to simplify lineweight override setting.
  
    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiPaletteOverrideHelper
{
  protected:
    OdGiSubEntityTraits *m_pTraits;
  public:
    OdGiPaletteOverrideHelper() : m_pTraits(NULL) {}
    OdGiPaletteOverrideHelper(OdGiSubEntityTraits &pTraits, const OdGiPalette &pOverride) : m_pTraits(NULL)
    { setPaletteOverride(pTraits, pOverride); }
    ~OdGiPaletteOverrideHelper();
    void setPaletteOverride(OdGiSubEntityTraits &pTraits, const OdGiPalette &pOverride);
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIPALETTE_H__
