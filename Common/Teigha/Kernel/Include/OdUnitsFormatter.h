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

#ifndef _OD_UNITSFORMATTER_H_INCLUDED_
#define _OD_UNITSFORMATTER_H_INCLUDED_

#include "OdPlatform.h"
#include "RxObject.h"
#include "OdString.h"
#include "OdDToStr.h"
#include "SharedPtr.h"

class OdCmColorBase;
class OdGePoint3d;

/** \details
    This class defines the interface for a Units Formatter.

    <group Other_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdUnitsFormatter : public OdRxObject
{ 
public:
  ODRX_DECLARE_MEMBERS(OdUnitsFormatter);

  enum LUnits
  {
    kScientific    = 1,
    kDecimal       = 2,
    kEngineering   = 3, // feet and decimal inches
    kArchitectural = 4, // feet and fractional inches
    kFractional    = 5,
    kWindowsDesktop = 6
  };

  enum DimzinValues
  {
    kSuppressesZeroFeetAndPreciselyZeroInches = 0,
    kIncludesZeroFeetAndPreciselyZeroInches   = 1,
    kIncludesZeroFeetAndSuppressesZeroInches  = 2,
    kIncludesZeroInchesAndSuppressesZeroFeet  = 3,
    // for dimentions
    kSuppressesLeadingZeros                   = 4,
    kSuppressesTrailingZeros                  = 8,
    kSuppressesBothLeadingAndTrailingZeros    = 12
  };

  /** \details
    Returns the specified color value or OdCmColorBase object as a string.
    \param value [in]  Value to format.
  */
  virtual OdString formatCmColor(const OdCmColorBase& value) const = 0;

  /** \details
    Returns the specified string as an wrapped OdCmColorBase object.
    \param string [in]  String to unformat.
  */
  virtual OdSharedPtr<OdCmColorBase> unformatCmColor(const OdString& string) const = 0;

  /** \details
    Returns the specified distance value as a string.
    \param value [in]  Value to format.
    \remarks
    The returned value depends on the DIMZIN, LUNITS, LUPREC, and UNITMODE system variables.
  */
  virtual OdString formatLinear(double value) const = 0;

  /** \details
    Returns the specified string as a distance value.
    \param string [in]  String to unformat.
    \remarks
    The returned value depends on the UNITMODE system variable.
  */
  virtual double unformatLinear(const OdString& string) const = 0;

  /** \details
    Returns the specified angle value as a string.
    \param value [in]  Value to format.
    \remarks
    The returned value depends on the ANGBASE, AUNITS, AUPREC, DIMZIN, and UNITMODE system variables.
    \remarks
    All angles are expressed in radians.
  */
  virtual OdString formatAngle(double value) const = 0;

  /** \details
    Returns the specified string as an angle value.
    \param string [in]  String to unformat.
    \remarks
    The returned value depends on the ANGBASE system variable.
    \remarks
    All angles are expressed in radians.
  */
  virtual double unformatAngle(const OdString& string) const = 0;

  static bool isZeroFeetSuppressed(int dimzin);
  static bool isZeroInchesSuppressed(int dimzin);

  /** \details
    Returns the specified distance value as a string.
    \param value [in]  Value to format.
    \param lUnits [in]  Length units. 
    \param precision [in]  Number of decimal places.
    \param dimzin [in]  A combination of ZeroSuppressionFlags.
    \param unitMode [in]  UNITMODE value.
    \param decsep [in]  Decimal separator.
    \param thsep [in]  Thousands separator.
    \remarks
    * A zero value for decsep specifies the default seoarator is to be used.
    * A zero value for thsep specifies that no separator is to be used.
  */
  static OdString formatL(double value, LUnits lUnits, int precision, int dimzin, int unitMode, const OdChar* decsep = 0, const OdChar* thsep = 0);
  static OdString formatArea(double value, LUnits lUnits, int precision, int dimzin, int unitMode, const OdChar* decsep = 0, const OdChar* thsep = 0);

  /** \details
    Returns the specified length string as a length value.
    \param string [in]  String to unformat.
  */
  static double unformatL(const OdString& string);

  enum AUnits
  {
    kDegrees    = 0,
    kDegMinSec  = 1,
    kGrads      = 2,
    kRadians    = 3,
    kSurveyors  = 4
  };


  /** \details
    Returns the specified angular value as a string.
    \param value [in]  Value to format.
    \param aUnits [in]  Angle units. 
    \param precision [in]  Number of decimal places.
    \param dimzin [in]  A combination of ZeroSuppressionFlags.
    \param unitMode [in]  UNITMODE value.
    \param decsep [in]  Decimal separator.
    \remarks
    * A zero value for decsep specifies the default separator is to be used.
    \remarks
    All angles are expressed as radians.
  */
  static OdString formatA(double value, AUnits aUnits, int precision, int dimzin, int unitMode, const OdChar* decsep = 0);

  /** \details
    Returns the specified string as an angular value.
    \param string [in]  String to unformat.
    \remarks
    All angles are expressed as radians.
  */
  static double unformatA(const OdString& string);
  static double unformatA_unnorm(const OdString& string, bool refuseDots = false);

private:
  static OdString _formatL(double value, bool isItArea, LUnits lUnits, int precision, int dimzin, int unitMode, const OdChar* decsep = 0, const OdChar* thsep = 0);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbUnitsFormatter object pointers.
*/
typedef OdSmartPtr<OdUnitsFormatter> OdUnitsFormatterPtr;


/** \details
    This class defines the interface for a Units Formatter.

    <group Other_Classes>
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdUnitsFormatterTool
{ 
public:
  OdUnitsFormatterTool()  {}
  ~OdUnitsFormatterTool() {}

  static OdString formatDecimal(double value, int precision, int dimzin, const OdChar* decsep, const OdChar* thsep = 0);
  static double angle(const OdChar*& buf, bool refuseDots = false);
  static int toInt(const OdString& sValue, int nMinValid = INT_MIN, int nMaxValid = INT_MAX);
  static bool isZero(double v, int precision);
  static OdString formatArchitectural(bool isNegative, int feet, int entier, int numerator, int denominator, int dimzin, int mode);
  static bool negative(const OdChar*& buf);
  static double integer(const OdChar*& buf);
  static void fraction(double value, int& entier, int& numerator, int& denominator, int precision)
  {
    denominator = 1 << precision;
    value = double(floor(value * denominator + 0.5)) / double(denominator);
    entier = int(floor(value));
    numerator = int((value - entier) * double(denominator));
    while((numerator % 2) == 0 && (numerator != 0))
    {
      numerator   /= 2;
      denominator /= 2;
    }
  }

  static double base_denominator(int prec, double base)
  {
    double denom = 1.;
    for(int i = 0; i < prec; ++i)
      denom *= base;
    return denom;
  }

  static double linear_denominator(int prec)
  {
    return base_denominator(prec, 10.);
  }

  static bool digit(OdChar c)
  {
    return (c >= '0' && c <= '9');
  }

  static double denominator(int prec)
  {
    double denom = 1.;
    switch(prec)
    {
    default:
    {
      for(int i = prec; i >= 5; i--)
        denom *= 10.;
    }
      //case 8: // 19d58'58.8889"
      //  denom *= 10;
      //case 7: // 19d58'58.889"
      //  denom *= 10;
      //case 6: // 19d58'58.89"
      //  denom *= 10;
      //case 5: // 19d58'58.9"
      //  denom *= 10;
    case 4: // 19d58'59"
    case 3:
      denom *= 60;
    case 2: // 19d59'
    case 1:
      denom *= 60;
    case 0: // 20d
      break;
    }
    return denom;
  }

  static void decomp(double v, int& degs, int& mins, double& secs, int prec);

  static OdString format(int degs, int mins, double secs, int prec)
  {
      OdString res;
      switch (prec)
      {
      case 0:
        res.format(OD_T("%dd"), degs);
        break;
      case 1:
      case 2:
        res.format(OD_T("%dd%d'"), degs, mins);
        break;
      case 3:
        prec = 4;
        // no break
      default:
        prec -= 4;
        OdString s = odDToStr(secs, 'f', prec);
        res.format(OD_T("%dd%d'%ls\""), degs, mins, s.c_str());
        break;
      }

    return res;
  }

  static OdString next(OdString& list, const OdChar* delim = OD_T(","))
  {
    OdString res = list.spanExcluding(delim);
    if(res.getLength() != list.getLength())
    {
      list = list.mid(res.getLength()+1);
    }
    else
    {
      list.empty();
    }
    return res;
  }

  static int countOccurences(const OdString& string, OdChar delim = ',')
  {
    int nOccurences = 0;
    int nDigits = 0;
    int nSpec = 0;
    const OdChar *pBuffer = string.c_str();
    while (*pBuffer)
    {
      if (*pBuffer >= '0' && *pBuffer <= '9')
        nDigits++;
      else if (*pBuffer == '+' || *pBuffer == '-')
      {
        nSpec++;
        if (nDigits > 0)
          break;
        }
      else if (*pBuffer == delim && nDigits > 0 && nSpec <= 1)
      {
        nDigits = nSpec = 0;
        nOccurences++;
      }
      else
        break;
      pBuffer++;
    } 
    return nOccurences;
  }
};

#endif //#ifndef _OD_UNITSFORMATTER_H_INCLUDED_
