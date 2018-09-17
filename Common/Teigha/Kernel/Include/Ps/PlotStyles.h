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



// RxPlotStyle.h -- interfaces for setting up plotstyles support for rendering

#ifndef __OD_RX_PLOT_STYLE__
#define __OD_RX_PLOT_STYLE__


#include "RxObject.h"
#include "CmColorBase.h"
#include "RxModule.h"
#include "Ge/GeDoubleArray.h"
#include "StringArray.h"

#define STL_USING_MAP
#define STL_USING_ALGORITHM
#include "OdaSTL.h"

class OdStreamBuf;

#include "TD_PackPush.h"

/** \details

    <group TD_Namespaces>
*/
namespace OdPs
{
  enum FillStyle
  {
    kFsSolid            = 64,
    kFsCheckerboard     = 65,
    kFsCrosshatch       = 66,
    kFsDiamonds         = 67,
    kFsHorizontalBars   = 68,
    kFsSlantLeft        = 69,
    kFsSlantRight       = 70,
    kFsSquareDots       = 71,
    kFsVerticalBars     = 72,
    kFsUseObject        = 73,

    kFsLast             = kFsUseObject
  };

  enum LineEndStyle
  {
    kLesButt          = 0,
    kLesSquare        = 1,
    kLesRound         = 2,
    kLesDiamond       = 3,
    kLesUseObject     = 4
  };
  
  enum LineJoinStyle
  {
    kLjsMiter         = 0,
    kLjsBevel         = 1,
    kLjsRound         = 2,
    kLjsDiamond       = 3,
    kLjsUseObject     = 5
  };
  
  // plotstyle's linetype
  
  enum LineType
  {
    kLtpSolid                            = 0,
    kLtpDashed                           = 1,
    kLtpDotted                           = 2,
    kLtpDashDot                          = 3,
    kLtpShortDash                        = 4,
    kLtpMediumDash                       = 5,
    kLtpLongDash                         = 6,
    kLtpShortDashX2                      = 7,
    kLtpMediumDashX2                     = 8,
    kLtpLongDashX2                       = 9,
    kLtpMediumLongDash                   = 10,
    kLtpMediumDashShortDashShortDash     = 11,
    kLtpLongDashShortDash                = 12,
    kLtpLongDashDotDot                   = 13,
    kLtpLongDashDot                      = 14,
    kLtpMediumDashDotShortDashDot        = 15,
    kLtpSparseDot                        = 16,
    kLtpISODash                          = 17,
    kLtpISODashSpace                     = 18,
    kLtpISOLongDashDot                   = 19,
    kLtpISOLongDashDoubleDot             = 20,
    kLtpISOLongDashTripleDot             = 21,
    kLtpISODot                           = 22,
    kLtpISOLongDashShortDash             = 23,
    kLtpISOLongDashDoubleShortDash       = 24,
    kLtpISODashDot                       = 25,
    kLtpISODoubleDashDot                 = 26,
    kLtpISODashDoubleDot                 = 27,
    kLtpISODoubleDashDoubleDot           = 28,
    kLtpISODashTripleDot                 = 29,
    kLtpISODoubleDashTripleDot           = 30,
    kLtpUseObject                        = 31,

    kLtpLast = kLtpUseObject
  };

}

/** \details
    This class represents PlotStyle data for a single color (or object color) in OdPsPlotStyle objects.
 
    Corresponding C++ library: PlotStyleSvcs
    <group Other_Classes> 
*/
class OdPsPlotStyleData
{
public:
  enum ColorPolicy
  {
    kDither       = 1,
    kGrayScale    = 2,
    kNotUseObject = 4
  };

protected:
  OdCmEntityColor       m_color; 
  OdInt16               m_colorPolicy;
  bool                  m_bAdaptiveLinetype; // not in color policy
  OdInt16               m_screening;
  OdInt16               m_physPenNum;
  OdInt16               m_virtPenNum;
  OdInt16               m_psLt;
  OdInt16               m_fillStyle;
  OdInt16               m_lineEndStyle;
  OdInt16               m_lineJoinStyle;
  double                m_dLinePatSz;
  double                m_lineweight;

public:

  OdPsPlotStyleData()
    : m_color(255, 255, 255)
    , m_colorPolicy(0)
    , m_bAdaptiveLinetype(false)
    , m_screening(100)
    , m_physPenNum(-1)
    , m_virtPenNum(-1)
    , m_psLt(OdInt16(OdPs::kLtpUseObject))
    , m_fillStyle(OdInt16(OdPs::kFsUseObject))
    , m_lineEndStyle(OdInt16(OdPs::kLesUseObject))
    , m_lineJoinStyle(OdInt16(OdPs::kLjsUseObject))
    , m_dLinePatSz(0.0)
    , m_lineweight(-1)
  {}

  FIRSTDLL_EXPORT FIRSTDLL_EXPORT_STATIC static const OdPsPlotStyleData kDefault;

   // m_color(255, 255, 255) indicates "UseEntity: in PlotStyles

  /** \details
    Equality operator.
  */
  bool operator ==(const OdPsPlotStyleData& other) const
  {
    return (
        m_color             == other.m_color             &&
        m_colorPolicy       == other.m_colorPolicy       &&
        m_bAdaptiveLinetype == other.m_bAdaptiveLinetype &&
        m_screening         == other.m_screening         &&
        m_physPenNum        == other.m_physPenNum        &&
        m_virtPenNum        == other.m_virtPenNum        &&
        m_psLt              == other.m_psLt              &&
        m_fillStyle         == other.m_fillStyle         &&
        m_lineEndStyle      == other.m_lineEndStyle      &&
        m_lineJoinStyle     == other.m_lineJoinStyle     &&
        m_dLinePatSz        == other.m_dLinePatSz        &&
        m_lineweight        == other.m_lineweight        
      );
  }

  /** \details
    Inequality operator.
  */
  bool operator !=(const OdPsPlotStyleData& other) const { return !operator ==(other); }

  /** \details
    Returns the Color for this PlotStyleData object.
  */
  const OdCmEntityColor&  color() const { return m_color; }

  OdInt16 colorPolicy() const { return m_colorPolicy; }
  /** \details
    Returns the Physical Pen Number for this PlotStyleData object.
  */
  OdInt16                 physicalPenNumber() const { return m_physPenNum; }
  /** \details
    Returns the Virtual Pen Number for this PlotStyleData object.
  */
  OdInt16                 virtualPenNumber() const { return m_virtPenNum; }
  
  /** \details
    Returns the Screening for this PlotStyleData object.
    
    \returns
    Returns a value in the range [0.0 .. 100.0].
    * 0 indicates white.
    * 100 indicates colors at their full intensities.
  */
  int                     screening() const { return m_screening; }

  /** \details
    Returns the Line Pattern Size for this PlotStyleData object.
  */
  double                  linePatternSize() const { return m_dLinePatSz; }
  /** \details
    Returns the Linetype for this PlotStyleData object.
  */
  OdPs::LineType          linetype() const { return OdPs::LineType(m_psLt); }

  /** \details
    Returns true if an only if the Linetype is adaptive for this PlotStyleData object.
  */
  bool                    isAdaptiveLinetype() const { return m_bAdaptiveLinetype; }
  /** \details
    Returns true if an only if Grayscale is on for this PlotStyleData object.
  */
  bool                    isGrayScaleOn() const { return GETBIT(m_colorPolicy, kGrayScale); }
  /** \details
    Returns true if an only if Dither is on for this PlotStyleData object.
  */
  bool                    isDitherOn() const { return GETBIT(m_colorPolicy, kDither); }
  /** \details
    Returns the Lineweight for this PlotStyleData object.
  */
  double                  lineweight() const { return m_lineweight; }
  /** \details
    Returns the Fill Style for this PlotStyleData object.
  */
  OdPs::FillStyle         fillStyle() const { return OdPs::FillStyle(m_fillStyle); }
  /** \details
    Returns the Line End Style for this PlotStyleData object.
  */
  OdPs::LineEndStyle      endStyle()  const { return OdPs::LineEndStyle(m_lineEndStyle); }
  /** \details
    Returns the Line Join Style for this PlotStyleData object.
  */
  OdPs::LineJoinStyle     joinStyle() const { return OdPs::LineJoinStyle(m_lineJoinStyle); }

  /** \details
    Sets the Color for this PlotStyleData object.
    \param color [in]  Color.
  */
  void setColor(const OdCmEntityColor& color) 
  { 
    m_color = color;
    SETBIT(m_colorPolicy, kNotUseObject, !(color.red() == 255 && color.green() == 255 && color.blue() == 255));
    // #CORE-11537 : turn off kNotUseObject bit if kGrayScale bit is set
    if (isGrayScaleOn() && GETBIT(m_colorPolicy, kNotUseObject))
      SETBIT_0(m_colorPolicy, kNotUseObject);
  }
  
  void setColorPolicy(OdInt16 colorPolicy) { m_colorPolicy = colorPolicy; }

  /** \details
    Sets the Physical Pen Number for this PlotStyleData object.
    \param physicalPenNumber [in]  Physical Pen Number.
  */
  void setPhysicalPenNumber(OdInt16 physicalPenNumber) { m_physPenNum = physicalPenNumber; }
  /** \details
    Sets the Virtual Pen Number for this PlotStyleData object.
    \param virtualPenNumber [in]  Virtual Pen Number.
  */
  void setVirtualPenNumber(OdInt16 virtualPenNumber) { m_virtPenNum = virtualPenNumber; }
  /** \details
    Sets the Screening for this PlotStyleData object.
    
    \param screening [in]  Screening.
    
    \remarks
    screening must be in range [0.0 .. 100.0].
    * 0 indicates white.
    * 100 indicates the colors at their full intensities.
  */
  void setScreening(int screening) { m_screening = OdInt16(screening); }
  /** \details
    Sets the Line Pattern Size for this PlotStyleData object.
    \param linePatternSize [in]  Line Pattern Size.
    
  */
  void setLinePatternSize(double linePatternSize) { m_dLinePatSz = linePatternSize; }
  /** \details
    Sets the Linetype for this PlotStyleData object.
    \param linetype [in]  Linetype.
  */
  void setLinetype(OdPs::LineType linetype) { m_psLt = OdInt16(linetype); }
  
  /** \details
    Controls if the Linetype is adaptive for this PlotStyleData object.
    \param adaptiveLinetype [in]  Controls Adaptive Linetype.
  */
  void setAdaptiveLinetype(bool adaptiveLinetype) { m_bAdaptiveLinetype = adaptiveLinetype; }
  /** \details
    Controls if Grayscale is on for this PlotStyleData object.
    \param grayScaleOn [in]  Controls Grayscale On.
  */
  void setGrayScaleOn(bool grayScaleOn)
  {
    if (grayScaleOn)
    {
      // #CORE-11537 : turn off kNotUseObject bit 
      SETBIT(m_colorPolicy, kNotUseObject, false);
    }
    else
    {
      // #CORE-11537 : turn on kNotUseObject bit if m_color is not white
      SETBIT(m_colorPolicy, kNotUseObject, !(m_color.red() == 255 && m_color.green() == 255 && m_color.blue() == 255));
    }
    SETBIT(m_colorPolicy, kGrayScale, grayScaleOn);
  }
  /** \details
    Controls if Dither is on for this PlotStyleData object.
    \param ditherOn [in]  Controls Dither On.
  */
  void setDitherOn(bool ditherOn) { SETBIT(m_colorPolicy, kDither, ditherOn); }
  /** \details
    Sets the Lineweight for this PlotStyleData object.
    \param lineweight [in]  Lineweight.
  */
  void setLineweight(double lineweight) { m_lineweight = lineweight; }
  /** \details
    Sets the Fill Style for this PlotStyleData object.
    \param fillStyle [in]  Fill Style.
  */
  void setFillStyle(OdPs::FillStyle fillStyle) { m_fillStyle = OdInt16(fillStyle); }
  /** \details
    Sets the LineEndStyle for this PlotStyleData object.
    \param LineEndStyle [in]  LineEndStyle.
  */
  void setEndStyle(OdPs::LineEndStyle lineEndStyle) { m_lineEndStyle = OdInt16(lineEndStyle); }
  /** \details
    Sets the Line Join Style for this PlotStyleData object.
    
    \param lineJoinStyle [in] Line Join Style.
  */
  void setJoinStyle(OdPs::LineJoinStyle lineJoinStyle) { m_lineJoinStyle = OdInt16(lineJoinStyle); }
};

/** \details
    This class represents PlotStyle objects.
 
    Corresponding C++ library: PlotStyleSvcs

    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdPsPlotStyle : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdPsPlotStyle);

  /** \details
    Returns the name of this PlotStyle object.
  */
  virtual OdString  name() const = 0;

  /** \details
    Returns the description of this PlotStyle object.
  */
  virtual OdString  description() const = 0;
  /** \details
    Returns the PlotStyleData for this PlotStyle object.
    \param data [out]  Receives the data.
  */
  virtual void      getData(OdPsPlotStyleData& data) const = 0;
  /** \details
    Returns the localizedName of this PlotStyle object.
  */
  virtual OdString  localizedName() const = 0;

  virtual void      setData(const OdPsPlotStyleData& data) = 0;

  virtual void      setDescription(const OdString& desc) = 0;

  virtual void      setName(const OdString& name) = 0;

  virtual void      setLocalizedName(const OdString& name) = 0;

};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdPsPlotStyle object pointers.
*/
typedef OdSmartPtr<OdPsPlotStyle> OdPsPlotStylePtr;

namespace OdPs
{

  // predicate
  class OdPsStrLess : public std::binary_function<OdString, OdString, bool>
  {
  public:
    bool operator()(const OdString& x, const OdString& y) const
    {
      return x.iCompare(y) < 0;
    }
  };

} // OdPs

typedef std::map<const OdChar*, OdPsPlotStylePtr, OdPs::OdPsStrLess> OdPsPlotStylesMap;
typedef OdArray<OdPsPlotStylePtr> OdPsPlotStylesArray;

/** \details
    This class implements Plot Style Tables.

    Corresponding C++ library: PlotStyleSvcs

    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdPsPlotStyleTable : public OdRxObject
{
public:  
  ODRX_DECLARE_MEMBERS(OdPsPlotStyleTable);

  virtual OdRxObjectPtr clone() const = 0;

  virtual void copyFrom(const OdRxObject* pSource) = 0;

  /** \details
    Returns true if and only if the global scale factor is to be applied to non-ISO linetypes
    for this PlotStyleTable object.
  */
  virtual bool      isApplyScaleFactor() const = 0;

  /** \details
    Returns the description of this PlotStyleTable object.
  */
  virtual OdString  description() const = 0;

  /** \details
    Returns the global scale factor to be applied to non-ISO linetypes for this PlotStyleTable object.
  */
  virtual double    scaleFactor() const = 0;

  /** \details
    Returns true if and only if Custom Lineweight Units are to be displayed for this PlotStyleTable object.
  */
  virtual bool      isDisplayCustomLineweightUnits() const = 0;

  /** \details
    Returns the lineweight for the specified entry in this PlotStyleTable object.
    \param index [in]  PlotStyle index.
  */
  virtual double    getLineweightAt(OdUInt32 index) const = 0;

  /** \details
    Returns the specified PlotStyle in this PlotStyleTable object.
    \param name [in]  Plot Style name.
  */
  virtual OdPsPlotStylePtr plotStyleAt(const OdString& name) const = 0;

  /** 
  */
  /** \details
    Returns the specified PlotStyle in this PlotStyleTable object.
    \param index [in]  PlotStyle index.
  */
  virtual OdPsPlotStylePtr plotStyleAt(int index) const = 0;

  /** \details
    Returns the logical length of this PlotStyle
  */
  virtual unsigned int plotStyleSize() const = 0;

  virtual unsigned int lineweightSize() const = 0;

  virtual bool isAciTableAvailable() const = 0;

  virtual OdPsPlotStylePtr addNewPlotStyle(const OdString& styleName) = 0;

  virtual OdPsPlotStylePtr delPlotStyle(OdPsPlotStylePtr pPsPtr) = 0;

  virtual void setDescription(const OdString& desc) = 0;

  virtual void setScaleFactor(double scFac) = 0;

  virtual void setApplyScaleFactor(bool flag) = 0;

  virtual void setDisplayCustomLineweightUnits(bool flag) = 0;

  virtual void setLineweightAt(double value, OdUInt32 index) = 0;

  virtual void setLineweights(const OdGeDoubleArray& lineweights) = 0;

  // Methods added in 2.7.0

  virtual void setAciTableAvailable(bool flag) = 0;

  virtual void setPlotStylesIndexed(const OdPsPlotStylesArray plotStylesIndexed) = 0;

  virtual void setPlotStyles(const OdPsPlotStylesMap plotStyles) = 0;

  virtual void addPlotStyle(OdPsPlotStyle* pPs) = 0;

  virtual void setOrdering(const OdStringArray& order) = 0;

  virtual OdPsPlotStylesMap plotStyles() const = 0;

  virtual OdPsPlotStylesArray plotStylesIndexed() const = 0;
};
/** \details
    This template class is a specialization of the OdPsPlotStyleTable class for OdAveScene object pointers.
*/
typedef OdSmartPtr<OdPsPlotStyleTable> OdPsPlotStyleTablePtr;

/** \details
    This class is the base class for classes that provide platform-dependent loading of PlotStyle (CTB/STB) files
    for Teigha.
    Corresponding C++ library: PlotStyleSvcs
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdPsPlotStyleServices : public OdRxModule
{
public:
  ODRX_DECLARE_MEMBERS(OdPsPlotStyleServices);

  /** \details
    Loads a plot style table.

    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
    \returns
    Returns a SmartPointer to the newly loaded PlotStyleTable object.   
  */
  virtual OdPsPlotStyleTablePtr createPlotStyleTable() = 0;
  virtual OdPsPlotStyleTablePtr loadPlotStyleTable(OdStreamBuf* pStreamBuf) = 0;
  virtual void                  savePlotStyleTable(OdStreamBuf* pBuf, OdPsPlotStyleTablePtr pPSTab) = 0;

};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdPsPlotStyleServices object pointers.
*/
typedef OdSmartPtr<OdPsPlotStyleServices> OdPsPlotStyleServicesPtr;

#include "TD_PackPop.h"

#endif // __OD_RX_PLOT_STYLE__
