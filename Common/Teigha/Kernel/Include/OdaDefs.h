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




#ifndef _ODA_DEFS_
#define _ODA_DEFS_

#ifndef _ODA_COMMON_INCLUDED_
#error  "Each cpp module MUST include OdaCommon.h as first header included"
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>


/** \details

    <group TD_Namespaces>
*/
namespace OdDb
{
  enum DwgVersion
  {
    kDHL_MC0_0   = 0,
    kDHL_AC1_2   = 1,
    kDHL_AC1_40  = 2,
    kDHL_AC1_50  = 3,
    kDHL_AC2_20  = 4,
    kDHL_AC2_10  = 5,
    kDHL_AC2_21  = 6,
    kDHL_AC2_22  = 7,
    kDHL_1001    = 8,
    kDHL_1002    = 9,             // Release 2.5
    vAC025       = kDHL_1002,     // Release 2.5
    kDHL_1003    = 10,            // Release 2.6
    vAC026       = kDHL_1003,     // Release 2.6
    kDHL_1004    = 11,            // Release 9
    vAC09        = kDHL_1004,     // Release 9
    kDHL_1005    = 12,
    kDHL_1006    = 13,            // Release 10
    vAC10        = kDHL_1006,     // Release 10
    kDHL_1007    = 14,
    kDHL_1008    = 15,
    kDHL_1009    = 16,             // R11 and R12
    vAC12        = kDHL_1009,      // R11 and R12
    kDHL_1010    = 17,
    kDHL_1011    = 18,
    kDHL_1012    = 19,             // R13
    vAC13        = kDHL_1012,      // R13
    kDHL_1013    = 20,             // R14 beta
    vAC14beta    = kDHL_1013,      // R14 beta
    kDHL_1014    = 21,             // R14 release
    vAC14        = kDHL_1014,      // R14 release
    kDHL_1500    = 22,             // R15 (2000) beta
    kDHL_1015    = 23,             // R15 (2000) release
    vAC15        = kDHL_1015,      // R15 (2000) release
    kDHL_1800a   = 24,             // R18 (2004) beta
    kDHL_1800    = 25,             // R18 (2004) release
    vAC18        = kDHL_1800,      // R18 (2004) release
    kDHL_2100a   = 26,             // R21 (2007) beta
    kDHL_1021    = 27,             // R21 (2007) release
    vAC21        = kDHL_1021,      // R21 (2007) release
    kDHL_2400a   = 28,             // R24 (2010) beta
    kDHL_1024    = 29,             // R24 (2010) release
    vAC24        = kDHL_1024,      // R24 (2010) release
    kDHL_2700a   = 30,             // R27 (2013) beta
    kDHL_1027    = 31,             // R27 (2013) release
    vAC27        = kDHL_1027,      // R27 (2013) release

    kDHL_3200a   = 32,             // R32 (2018) beta
    kDHL_1032    = 33,             // R32 (2018) release
    vAC32        = kDHL_1032,      // R32 (2018) release

    /*kDHL_PRECURR = vAC24,
    kDHL_CURRENT = vAC27,*/

    kDHL_PRECURR = vAC27,
    kDHL_CURRENT = vAC32,
    kDHL_Unknown = 32766,
    kDHL_Max     = 32767
  };

  enum MaintReleaseVer
  {
    kMRelease0      = 0,
    kMRelease1      = 1,
    kMRelease2      = 2,
    kMRelease3      = 3,
    kMRelease4      = 4,
    kMRelease5      = 5,
    kMRelease6      = 6,
    kMRelease7      = 7,
    kMRelease8      = 8,
    kMRelease9      = 9,
    kMRelease10     = 10,
    kMRelease11     = 11,
    kMRelease12     = 12,
    kMRelease13     = 13,
    kMRelease14     = 14,
    kMRelease15     = 15,
    kMRelease16     = 16,
    kMRelease17     = 17,
    kMRelease18     = 18,
    kMRelease19     = 19,
    kMRelease20     = 20,
    kMRelease21     = 21,
    kMRelease22     = 22,
    kMRelease23     = 23,
    kMRelease24     = 24,
    kMRelease25     = 25,
    kMRelease26     = 26,
    kMRelease27     = 27,
    kMRelease28     = 28,
    kMRelease29     = 29,
    kMRelease30     = 30,
    kMRelease31     = 31,
    kMRelease32     = 32,
    kMRelease33     = 33,
    kMRelease34     = 34,
    kMRelease35     = 35,
    kMRelease36     = 36,
    kMRelease37     = 37,
    kMRelease38     = 38,
    kMRelease39     = 39,
    kMRelease40     = 40,
    kMRelease41     = 41,
    kMRelease42     = 42,
    kMRelease43     = 43,
    kMRelease44     = 44,
    kMRelease45     = 45,
    kMRelease46     = 46,
    kMRelease47     = 47,
    kMRelease48     = 48,
    kMRelease49     = 49,
    kMRelease50     = 50,
    kMRelease51     = 51,
    kMRelease52     = 52,
    kMRelease53     = 53,
    kMRelease54     = 54,
    kMRelease55     = 55,
    kMRelease56     = 56,
    kMRelease57     = 57,
    kMRelease58     = 58,
    kMRelease59     = 59,
    kMRelease60     = 60,
    kMRelease61     = 61,
    kMRelease62     = 62,
    kMRelease63     = 63,
    kMRelease64     = 64,
    kMRelease65     = 65,
    kMRelease66     = 66,
    kMRelease67     = 67,
    kMRelease68     = 68,
    kMRelease69     = 69,
    kMRelease70     = 70,
    kMRelease71     = 71,
    kMRelease72     = 72,
    kMRelease73     = 73,
    kMRelease74     = 74,
    kMRelease75     = 75,
    kMRelease76     = 76,
    kMRelease77     = 77,
    kMRelease78     = 78,
    kMRelease79     = 79,
    kMRelease80     = 80,
    kMRelease81     = 81,
    kMRelease82     = 82,
    kMRelease83     = 83,
    kMRelease84     = 84,
    kMRelease85     = 85,
    kMRelease86     = 86,
    kMRelease87     = 87,
    kMRelease88     = 88,
    kMRelease89     = 89,
    kMRelease90     = 90,
    kMRelease91     = 91,
    kMRelease92     = 92,
    kMRelease93     = 93,
    kMRelease94     = 94,
    kMRelease95     = 95,
    kMRelease96     = 96,
    kMRelease97     = 97,
    kMRelease98     = 98,
    kMRelease99     = 99,
    kMRelease100    = 100,
    kMRelease101    = 101,
    kMRelease102    = 102,
    kMRelease103    = 103,
    kMRelease104    = 104,
    kMRelease105    = 105,
    kMRelease106    = 106,
    kMRelease107    = 107,
    kMRelease108    = 108,
    kMRelease109    = 109,
    kMRelease110    = 110,
    kMRelease111    = 111,
    kMRelease112    = 112,
    kMRelease113    = 113,
    kMRelease114    = 114,
    kMRelease115    = 115,
    kMRelease116    = 116,
    kMRelease117    = 117,
    kMRelease118    = 118,
    kMRelease119    = 119,
    kMRelease120    = 120,
    kMRelease121    = 121,
    kMRelease122    = 122,
    kMRelease123    = 123,
    kMRelease124    = 124,                             
    kMRelease125    = 125,
    kMRelease126    = 126,
    kMRelease127    = 127,
    kMRelease128    = 128,
    kMRelease129    = 129,
    kMRelease130    = 130,
    kMRelease131    = 131,
    kMRelease132    = 132,
    kMRelease133    = 133,
    kMRelease134    = 134,
    kMRelease135    = 135,
    kMRelease136    = 136,
    kMRelease137    = 137,
    kMRelease138    = 138,
    kMRelease139    = 139,
    kMRelease140    = 140,
    kMRelease141    = 141,
    kMRelease142    = 142,
    kMRelease143    = 143,
    kMRelease144    = 144,
    kMRelease145    = 145,
    kMRelease146    = 146,
    kMRelease147    = 147,
    kMRelease148    = 148,
    kMRelease149    = 149,
    kMRelease150    = 150,
    kMRelease151    = 151,
    kMRelease152    = 152,
    kMRelease153    = 153,
    kMRelease154    = 154,
    kMRelease155    = 155,
    kMRelease156    = 156,
    kMRelease157    = 157,
    kMRelease158    = 158,
    kMRelease159    = 159,
    kMRelease160    = 160,
    kMRelease161    = 161,
    kMRelease162    = 162,
    kMRelease163    = 163,
    kMRelease164    = 164,
    kMRelease165    = 165,
    kMRelease166    = 166,
    kMRelease167    = 167,
    kMRelease168    = 168,
    kMRelease169    = 169,
    kMRelease170    = 170,
    kMRelease171    = 171,
    kMRelease172    = 172,
    kMRelease173    = 173,
    kMRelease174    = 174,
    kMRelease175    = 175,
    kMRelease176    = 176,
    kMRelease177    = 177,
    kMRelease178    = 178,
    kMRelease179    = 179,
    kMRelease180    = 180,
    kMRelease181    = 181,
    kMRelease182    = 182,
    kMRelease183    = 183,
    kMRelease184    = 184,
    kMRelease185    = 185,
    kMRelease186    = 186,
    kMRelease187    = 187,
    kMRelease188    = 188,
    kMRelease189    = 189,
    kMRelease190    = 190,
    kMRelease191    = 191,
    kMRelease192    = 192,
    kMRelease193    = 193,
    kMRelease194    = 194,
    kMRelease195    = 195,
    kMRelease196    = 196,
    kMRelease197    = 197,
    kMRelease198    = 198,
    kMRelease199    = 199,
    kMRelease200    = 200,
    kMRelease201    = 201,
    kMRelease202    = 202,
    kMRelease203    = 203,
    kMRelease204    = 204,
    kMRelease205    = 205,
    kMRelease206    = 206,
    kMRelease207    = 207,
    kMRelease208    = 208,
    kMRelease209    = 209,
    kMRelease210    = 210,
    kMRelease211    = 211,
    kMRelease212    = 212,
    kMRelease213    = 213,
    kMRelease214    = 214,
    kMRelease215    = 215,
    kMRelease216    = 216,
    kMRelease217    = 217,
    kMRelease218    = 218,
    kMRelease219    = 219,
    kMRelease220    = 220,
    kMRelease221    = 221,
    kMRelease222    = 222,
    kMRelease223    = 223,
    kMRelease224    = 224,
    kMRelease225    = 225,
    kMRelease226    = 226,
    kMRelease227    = 227,
    kMRelease228    = 228,
    kMRelease229    = 229,
    kMRelease230    = 230,
    kMRelease231    = 231,
    kMRelease232    = 232,
    kMRelease233    = 233,
    kMRelease234    = 234,
    kMRelease235    = 235,
    kMRelease236    = 236,
    kMRelease237    = 237,
    kMRelease238    = 238,
    kMRelease239    = 239,
    kMRelease240    = 240,
    kMRelease241    = 241,
    kMRelease242    = 242,
    kMRelease243    = 243,
    kMRelease244    = 244,
    kMRelease245    = 245,
    kMRelease246    = 246,
    kMRelease247    = 247,
    kMRelease248    = 248,
    kMRelease249    = 249,
    kMRelease250    = 250,
    kMRelease251    = 251,
    kMRelease252    = 252,
    kMRelease253    = 253,
    kMRelease254    = 254,
    kMRelease255    = 255,

    // kMRelease57 - R27 (2016) beta
    // kMRelease105 -R27 (2016) release

    // kMRelease4   -R32 (2018) release

    kMReleaseFirstValid1500 = kMRelease41,

    //kMReleaseCurrent = kMRelease105,
    kMReleaseCurrent = kMRelease4,

    kMReleaseUnknown = 126,
    kMReleaseMax     = 127,
    kMRelease2010Max = 255
  };

  // lineweights are in 100ths of a millimeter
  enum LineWeight
  {
    kLnWt000          =   0,
    kLnWt005          =   5,
    kLnWt009          =   9,
    kLnWt013          =  13,
    kLnWt015          =  15,
    kLnWt018          =  18,
    kLnWt020          =  20,
    kLnWt025          =  25,
    kLnWt030          =  30,
    kLnWt035          =  35,
    kLnWt040          =  40,
    kLnWt050          =  50,
    kLnWt053          =  53,
    kLnWt060          =  60,
    kLnWt070          =  70,
    kLnWt080          =  80,
    kLnWt090          =  90,
    kLnWt100          = 100,
    kLnWt106          = 106,
    kLnWt120          = 120,
    kLnWt140          = 140,
    kLnWt158          = 158,
    kLnWt200          = 200,
    kLnWt211          = 211,
    kLnWtByLayer      =  -1,
    kLnWtByBlock      =  -2,
    kLnWtByLwDefault  =  -3
  };

  enum PlotStyleNameType
  {
    kPlotStyleNameByLayer       = 0,
    kPlotStyleNameByBlock       = 1,
    kPlotStyleNameIsDictDefault = 2,
    kPlotStyleNameById          = 3
  };

// ODA_MT_DB_BEGIN
  enum MultiThreadedMode
  {
    kSTMode       =  0,
    kMTRendering  =  1,
    kMTLoading    =  2
  };
// ODA_MT_DB_END

  /** \details
    Represents the measurement systems.
  */
  enum MeasurementValue
  {
    kEnglish = 0,  //!< English system.
    kMetric  = 1   //!< Metric system.
  };

// This enum is used in OdUtilAdds / Units formatter
  enum UnitsValue
  {
    kUnitsUndefined    = 0,
    kUnitsInches       = 1,
    kUnitsFeet         = 2,
    kUnitsMiles        = 3,
    kUnitsMillimeters  = 4,
    kUnitsCentimeters  = 5,
    kUnitsMeters       = 6,
    kUnitsKilometers   = 7,
    kUnitsMicroinches  = 8,
    kUnitsMils         = 9,
    kUnitsYards        = 10,
    kUnitsAngstroms    = 11,
    kUnitsNanometers   = 12,
    kUnitsMicrons      = 13,
    kUnitsDecimeters   = 14,
    kUnitsDekameters   = 15,
    kUnitsHectometers  = 16,
    kUnitsGigameters   = 17,
    kUnitsAstronomical = 18,
    kUnitsLightYears   = 19,
    kUnitsParsecs      = 20,
    kUnitsUSSurveyFeet  = 21,
    kUnitsUSSurveyInch  = 22,
    kUnitsUSSurveyYard  = 23,
    kUnitsUSSurveyMile  = 24,
    kUnitsMax           = kUnitsUSSurveyMile
  };

  enum Intersect
  {
    kOnBothOperands    = 0,
    kExtendThis        = 1,
    kExtendArg         = 2,
    kExtendBoth        = 3
  };

  enum OsnapMode
  {
    kOsModeEnd      = 1,  // Endpoint
    kOsModeMid      = 2,  // Midpoint
    kOsModeCen      = 3,  // Center
    kOsModeNode     = 4,  // Node
    kOsModeQuad     = 5,  // Quadrant
    kOsModeIntersec = 6,  // Intersection
    kOsModeIns      = 7,  // Insertion point
    kOsModePerp     = 8,  // Perpendicular
    kOsModeTan      = 9,  // Tangent
    kOsModeNear     = 10, // Nearest
    kOsModeApint    = 11, // Apparent intersection
    kOsModePar      = 12, // Parallel
    kOsModeStart    = 13  // Startpoint
  };

  // Do not append any enums here !
}

#define SETBIT(flags, bit, value) ((value) ? (flags |= (bit)) : (flags &= ~(bit)))
#define GETBIT(flags, bit) (((flags) & (bit)) ? true : false)

#define SETBIT_0(flags, bit) (flags &= ~(bit))
#define SETBIT_1(flags, bit) (flags |=  (bit))

inline bool OdPositive(double x, double tol = 1.e-10)
{
  return (x > tol);
}

inline bool OdNegative(double x, double tol = 1.e-10)
{
  return (x < -tol);
}

inline bool OdZero(double x, double tol = 1.e-10)
{
  return !OdPositive(x, tol) && !OdNegative(x, tol);
}

inline bool OdNonZero(double x, double tol = 1.e-10)
{
  return OdPositive(x, tol) || OdNegative(x, tol);
}

inline bool OdEqual(double x, double y, double tol = 1.e-10)
{
  return OdZero(x - y, tol);
}

inline bool OdLess(double x, double y, double tol = 1.e-10)
{
  return x < y - tol;
}

inline bool OdLessOrEqual(double x, double y, double tol = 1.e-10)
{
  return x <= y + tol;
}

inline bool OdGreater(double x, double y, double tol = 1.e-10)
{
  return x > y + tol;
}

inline bool OdGreaterOrEqual(double x, double y, double tol = 1.e-10)
{
  return x >= y - tol;
}

inline double OdSign(double x)
{
  return (x < 0.0 ? -1.0 : (x > 0.0 ? 1.0 : 0.0));
}

inline int OdSign(int x)
{
  return (x < 0 ? -1 : (x > 0 ? 1 : 0));
}

inline int OdCmpDouble(double x, double y, double tol = 1.e-10)
{
  if (OdEqual(x,y,tol))
    return 0;
  if (OdPositive(x - y, tol))
    return 1;
  else
    return -1;
}

inline double safeDivide( double a, double b)
{
  if (a == 0.)
    return 0.;
  
  if (b != 0.)
  {
    // Binary exponent of 1.e+300 (value which can be safely represented
    // by IEEE double. Note that even if on some platforms double can represent
    // larger values they are saved to a .dwg file as IEEE where
    // DBL_MAX = 1.7976931348623158e+308 and DBL_MAX_EXP = 1024
    static const int maxBinExp = 997;
    int nExpA, nExpB;
    //double dMantissa = 
    frexp(a, &nExpA);
    //dMantissa = 
    frexp(b, &nExpB);
    if ((nExpA - nExpB) < maxBinExp)
      return a / b;
  }

  bool bNeg = (a < 0.);
  if (b < 0.)
    bNeg = !bNeg;

  return bNeg ? -1.e+300 : 1.e+300;
}



#endif // _ODA_DEFS_

