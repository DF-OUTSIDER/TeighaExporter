/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autodesk, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Autodesk, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Autodesk, Inc. OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Now comes May 21, 2014 
// The following list, and the order of their listing, has been optimized for
// the use of pre-compiled headers.  Some of these files are unreferenced in
// this module, a small price paid for the efficiency affored by pre-compiled
// headers.
/*lint -e766 */		/* Disable PC-Lint's warning of unreferenced headers */

#include "cs_map.h"
#include "cs_NameMapper.hpp"
//  cs_NameMapper.hpp includes cs_CsvFileSupport.hpp
//  cs_NameMapper.hpp includes csNameMapperSupport.hpp
#include "cs_WktObject.hpp"
#include "cs_wkt.h"

////////////////////////////////////////////////////////////////////////////////
// This array associates the WKT type enumerator with the identifying text
// string.  The boolean indicates if the type includes a name as the value
// item.  Note that the implementation assumes a one-to-one correspondence
// between the enumerator and this table.
TrcWktEleTypeMap KrcWktEleTypeMap [] =
{
	{         "<None>", rcWktNone,          false                },
	{      "AUTHORITY", rcWktAuthority,     true                 },
	{           "AXIS", rcWktAxis,          true                 },
	{       "COMPD_CS", rcWktCompDCS,       true                 },
	{      "CONCAT_MT", rcWktConcatMt,      false                },
	{          "DATUM", rcWktDatum,         true                 },
	{      "FITTED_CS", rcWktFittedCS,      true                 },
	{         "GEOCCS", rcWktGeoCCS,        true                 },
	{         "GEOGCS", rcWktGeogCS,        true                 },
	{       "GEOGTRAN", rcWktGeogTran,      true                 },
	{     "INVERSE_MT", rcWktInverseMt,     false                },
	{       "LOCAL_CS", rcWktLocalCS,       true                 },
	{         "METHOD", rcWktMethod,        true                 },
	{      "PARAMETER", rcWktParameter,     true                 },
	{       "PARAM_MT", rcWktParamMt,       true                 },
	{ "PASSTHROUGH_MT", rcWktPassThroughMt, false                },
	{         "PRIMEM", rcWktPrimeM,        true                 },
	{         "PROJCS", rcWktProjCS,        true                 },
	{     "PROJECTION", rcWktProjection,    true                 },
	{       "SPHEROID", rcWktSpheroid,      true                 },
	{        "TOWGS84", rcWktToWgs84,       false                },
	{           "UNIT", rcWktUnit,          true                 },
	{     "VERT_DATUM", rcWktVertDatum,     true                 },
	{        "VERT_CS", rcWktVertCS,        true                 },
	{    "LOCAL_DATUM", rcWktLocalDatum,    true                 },
	{      "ELLIPSOID", rcWktSpheroid,      true                 },		// A synonym for SPHEROID.
	{      "<Unknown>", rcWktUnknown,       false                }
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// This array associates a text string with the axis enumerator.  This is not
// really used.  It was developed, and appears to be correct.  It is preserved
// only in case it may be useful in the future.
TrcWktAxisValueMap KrcWktAxisValueMap [] =
{
	{         "<None>", rcWktAxisNone                           },
	{          "NORTH", rcWktAxisNorth                          },
	{          "SOUTH", rcWktAxisSouth                          },
	{           "EAST", rcWktAxisEast                           },
	{           "WEST", rcWktAxisWest                           },
	{             "UP", rcWktAxisUp                             },
	{           "DOWN", rcWktAxisDown                           },
	{          "OTHER", rcWktAxisOther                          },
	{        "Unknown", rcWktAxisUnknown                        }
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following is a table of acronyms which is the first step in reducing
// the size of a key name.  Many of these are widely used.  Some were
// invented for the purpose of this table.  This table is used to reduce
// all key name types.
extern "C" const struct csKeyNmRed_ csAcronyms [] =
{
	{                      "Ancienne Triangulation Francaise",  "ATF"      },
	{                            "Australian Antarctic Datum",  "AAD"      },
	{                             "Australian Geodetic Datum",  "AGD"      },
	{                            "Average Terrestrial System",  "ATS"      },
	{                 "European Terrestrial Reference System",  "ETRF"     },
	{                         "Geocentric Datum of Australia",  "GDA"      },
	{                             "Geodetic Reference System",  "GRS"      },
	{                               "Japanese Geodetic Datum",  "JDA"      },
	{                        "Militar-Geographische Institut",  "MGI"      },
	{                        "High Accuracy Regional Network",  "HARN"     },
	{                     "Canadian Spatial Reference System",  "CSRS"     },
	{                            "New Zealand Geodetic Datum",  "NZGD"     },
	{                                  "North American Datum",  "NAD"      },
	{                      "Nouvelle Triangulation Francaise",  "NTF"      },
	{                      "Posiciones Geodesicas Argentinas",  "PGA"      },
	{                      "Provisional South American Datum",  "PSAD"     },
	{                                  "Qatar National Datum",  "QND"      },
	{                              "Red Geodesica Venezolana",  "RGV"      },
	{                            "Reseau Geodesique Francais",  "RGF"      },
	{                                 "Reseau National Belge",  "RGB"      },
	{"Sistema de Referencia Geocentrico para America del Sur",  "GRS-SA"   },
	{                                  "South American Datum",  "SAD"      },
	{                     "Swiss Terrestrial Reference Frame",  "STRF"     },
	{                                 "World Geodetic System",  "WGS"      },
	{                       "Yemen National Geodetic Network",  "Yeman"    },
	{                                   "British West Indies",  "BWI"      },
	{                                   "Tunisia Mining Grid",  "TMG"      },
	{                                                      "",   ""        }
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following table is used to reduce some special cases with regard to
// ellipsoid names.
extern "C" const struct csKeyNmRed_ csElReduce [] =
{
	{      "Spheroid",  ""         },
	{   "Adjustment)",  "Adj)"     },
	{   "Definition)",  "Def)"     },
	{              "",   ""        }
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following table is used to reduce some special cases with regard to
// datum names.  Essentially, we try to eliminate redundant information.  For
// example, "Azores Central Islands" is reduced to "Azores Central" with the
// justification that with regard to the Azores, the term "Islands" is
// redundant.  Similarly, this table applies only to datum names.  Therefore,
// the term datum within a datum name is considered to be redundant.
extern "C" const struct csKeyNmRed_ csDtReduce [] =
{
	{                             " To WGS 1984",  ""                       },
	{                   "Azores Central Islands",  "Azores Central"         },
	{                "Azores Occidental Islands",  "Azores Occidental"      },
	{                  "Azores Oriental Islands",  "Azores Oriental"        },
	{              "Deutsches Hauptdreiecksnetz",  "Hauptdreiecksnetz"      },
	{                  "EPSG example Platform Y",  "EPSG exmpl Y"           },
	{                           "European Datum",  "European"               },
	{                    "European Libyan Datum",  "European/Libyan"        },
	{          "Greek Geodetic Reference System",  "Greek GRS"              },
	{"Jednotne Trigonometricke Site Katastralni",  "Katastralni"            },
	{         "Kartasto Koordinaati Jarjestelma",  "Jarjestelma"            },
	{                "National Geodetic Network",  "National Geodetic Net"  },
	{                              "Nord Sahara",  "N Sahara"               },
	{                 "Not specified (based on ",  "Ellp: "                 },
	{                              " ellipsoid)",  ""                       },
	{                   "Rikets koordinatsystem",  "Rikets"                 },
	{                              "(Stockholm)",  "(Stckhlm)"              },
	{          "Yemen National Geodetic Network",  "Yeman"                  },
	{                                         "",   ""                      }
};
//newPage//
///////////////////////////////////////////////////////////////////////////////
// The following table is used to reduce some special cases with regard to
// datum names.  Essentially, we try to eliminate redundant information.  For
// example, "Azores Central Islands" is reduced to "Azores Central" with the
// justification that with regard to the Azores, the term "Islands" is
// redundant.  Similarly, this table applies only to datum names.  Therefore,
// the term datum within a datum name is considered to be redundant.
//
// As of this writing (October 1, 2003) this list is rather incomplete.
// However, the basic functionality associated with it has been tested.
//
extern "C" const struct csKeyNmRed_ csCsReduce [] =
{
	{                                " / ",  "."          },
	{                             " zone ",  "-"          },
	{                           "National",  "Ntnl"       },
	{                       "Gauss-Kruger",  "GK"         },
	{                     "Nord de Guerre",  "NdG"        },
	{                           "3-degree",  "3d"         },
	{                     "American Samoa",  "Samoa"      },
	{                     "Azores Central",  "AzoresCtrl" },
	{                  "Azores Occidental",  "AzoresOccd" },
	{                      "Azores Orient",  "AzoresOrnt" },
	{                     "Campo Inchause",  "Campo"      },
	{                 "NAD 1927 CGQ77 MTM",  "MTM77"      },
	{                              " 1987",  "87"         },
	{                              " 1962",  "62"         },
	{                              " 1958",  "58"         },
	{                              " 1957",  "57"         },
	{                              " 1943",  "43"         },
	{                              " 1950",  "50"         },
	{                              " 1960",  "60"         },
	{                              " 1940",  "40"         },
	{                              " 1948",  "48"         },
	{                              " 1939",  "39"         },
	{                              " 1954",  "54"         },
	{                              " 1987",  "97"         },
	{                              " 1987",  "97"         },
	{                              " 1987",  "97"         },
	{                              " 1987",  "97"         },
	{                              " 1905",  "05"         },
	{                              " 1907",  "07"         },
	{                          "(Jakarta)",  "(Jkta)"     },
	{                         "(Brussels)",  "(Brsls)"    },
	{                                   "",  ""           }
};
//newPage//
// The following table is used to convert EPSG Operation Codes to CS-MAP
// projection codes.  The cs_PRJCOD_END value is used to indicate an EPSG
// operation code which does not map to a supported CS-MAP projection.
//
// The folowing produces a warning about the extern in the following
// definition.  However, you can't declare it with "C" linkage without
// the extern.  Since this table is only used in one 'C' module, it
// probably should be moved to that module to make the warning
// go away.
extern "C" struct cs_EpsgOpMthMap_ cs_EpsgOpMthMap [] =			//lint !e509   extern used with definition
{
	{ 10101, 10233,  cs_PRJCOD_TRMER},
	{ 10301, 10408,  cs_PRJCOD_LM2SP},
	{ 10420, 10420,  cs_PRJCOD_ALBER},
	{ 10431, 10630,  cs_PRJCOD_LM2SP},
	{ 10700, 10902,  cs_PRJCOD_TRMER},
	{ 10903, 10903,  cs_PRJCOD_LM2SP},
	{ 10931, 10932,  cs_PRJCOD_TRMER},
	{ 10933, 10933,  cs_PRJCOD_LM2SP},
	{ 10934, 10934,  cs_PRJCOD_ALBER},
	{ 11001, 11332,  cs_PRJCOD_TRMER},
	{ 11401, 11733,  cs_PRJCOD_LM2SP},
	{ 11801, 11854,  cs_PRJCOD_TRMER},
	{ 11900, 12032,  cs_PRJCOD_LM2SP},
	{ 12101, 12103,  cs_PRJCOD_TRMER},
	{ 12111, 12143,  cs_PRJCOD_LM2SP},
	{ 12150, 12150,  cs_PRJCOD_SWISS},
	{ 12201, 12233,  cs_PRJCOD_LM2SP},
	{ 12301, 12433,  cs_PRJCOD_TRMER},
	{ 12501, 12630,  cs_PRJCOD_LM2SP},
	{ 12701, 13103,  cs_PRJCOD_TRMER},
	{ 13104, 13104,  cs_PRJCOD_LM2SP},
	{ 13131, 13133,  cs_PRJCOD_TRMER},
	{ 13134, 13732,  cs_PRJCOD_LM2SP},
	{ 13800, 13830,  cs_PRJCOD_TRMER},
	{ 13901, 14253,  cs_PRJCOD_LM2SP},
	{ 14254, 14254,  cs_PRJCOD_ALBER},
	{ 14301, 14333,  cs_PRJCOD_LM2SP},
	{ 14400, 14430,  cs_PRJCOD_TRMER},
	{ 14501, 14803,  cs_PRJCOD_LM2SP},
	{ 14811, 14811,  cs_PRJCOD_TRMER},
	{ 14831, 14833,  cs_PRJCOD_LM2SP},
	{ 14841, 14938,  cs_PRJCOD_TRMER},
	{ 15001, 15001,  cs_PRJCOD_SWISS},
	{ 15002, 15009,  cs_PRJCOD_TRMER},
	{ 15010, 15010,  cs_PRJCOD_LM2SP},
	{ 15020, 15021,  cs_PRJCOD_ALBER},
	{ 15031, 15031,  cs_PRJCOD_SWISS},
	{ 15032, 15039,  cs_PRJCOD_TRMER},
	{ 15040, 15040,  cs_PRJCOD_LM2SP},
	{ 15101, 15138,  cs_PRJCOD_TRMER},
	{ 15201, 15299,  cs_PRJCOD_LM2SP},
	{ 15300, 15301,  cs_PRJCOD_LM1SP},
	{ 15302, 15303,  cs_PRJCOD_LM2SP},
	{ 15304, 15306,  cs_PRJCOD_TRMER},
	{ 15307, 15316,  cs_PRJCOD_LM2SP},
	{ 15317, 15319,  cs_PRJCOD_TRMER},
	{ 15320, 15320,  cs_PRJCOD_LM2SP},
	{ 15321, 15327,  cs_PRJCOD_TRMER},
	{ 15328, 15335,  cs_PRJCOD_LM2SP},
	{ 15336, 15337,  cs_PRJCOD_TRMER},
	{ 15338, 15338,  cs_PRJCOD_LM2SP},
	{ 15339, 15344,  cs_PRJCOD_TRMER},
	{ 15345, 15371,  cs_PRJCOD_LM2SP},
	{ 15372, 15373,  cs_PRJCOD_TRMER},
	{ 15374, 15375,  cs_PRJCOD_LM2SP},
	{ 15376, 15376,  cs_PRJCOD_LM1SP},
	{ 15377, 15380,  cs_PRJCOD_LM2SP},
	{ 15381, 15384,  cs_PRJCOD_TRMER},
	{ 15385, 15386,  cs_PRJCOD_LM2SP},
	{ 15387, 15390,  cs_PRJCOD_TRMER},
	{ 15391, 15395,  cs_PRJCOD_LM2SP},
	{ 15397, 15398,  cs_PRJCOD_ALBER},
	{ 15399, 15399,    cs_PRJCOD_END},
	{ 15400, 15400,    cs_PRJCOD_END},
	{ 15914, 15917,  cs_PRJCOD_TRMER},
	{ 16000, 16000,    cs_PRJCOD_END},
	{ 16001, 16060,  cs_PRJCOD_TRMER},
	{ 16061, 16061,  cs_PRJCOD_PSTRO},
	{ 16065, 16099,  cs_PRJCOD_TRMER},
	{ 16100, 16100,    cs_PRJCOD_END},
	{ 16101, 16160,  cs_PRJCOD_TRMER},
	{ 16161, 16161,  cs_PRJCOD_PSTRO},
	{ 16170, 17054,  cs_PRJCOD_TRMER},
	{ 17204, 17274,  cs_PRJCOD_LM2SP},
	{ 17275, 17293,cs_PRJCOD_PSTROSL},
	{ 17294, 17294,  cs_PRJCOD_LM2SP},
	{ 17295, 17300,  cs_PRJCOD_AZMEA},
	{ 17321, 17358,  cs_PRJCOD_TRMER},
	{ 17359, 17362,  cs_PRJCOD_LM2SP},
	{ 17363, 17363,  cs_PRJCOD_TRMER},
	{ 17364, 17364,  cs_PRJCOD_LM2SP},
	{ 17365, 17365,  cs_PRJCOD_ALBER},
	{ 17401, 17401,  cs_PRJCOD_LM2SP},
	{ 17402, 17458,  cs_PRJCOD_TRMER},
	{ 17515, 17625,  cs_PRJCOD_SOTRM},
	{ 17700, 18009,  cs_PRJCOD_TRMER},
	{ 18011, 18022,  cs_PRJCOD_LM1SP},
	{ 18031, 18059,  cs_PRJCOD_TRMER},
	{ 18061, 18062,  cs_PRJCOD_LM1SP},
	{ 18071, 18074,  cs_PRJCOD_TRMER},
	{ 18081, 18084,  cs_PRJCOD_LM1SP},
	{ 18085, 18085,  cs_PRJCOD_LM2SP},
	{ 18086, 18117,  cs_PRJCOD_LM1SP},
	{ 18121, 18122,  cs_PRJCOD_TRMER},
	{ 18131, 18135,  cs_PRJCOD_LM1SP},
	{ 18141, 18180,  cs_PRJCOD_TRMER},
	{ 18181, 18182,  cs_PRJCOD_LM1SP},
	{ 18183, 18199,  cs_PRJCOD_TRMER},
	{ 18201, 18201,  cs_PRJCOD_CSINI},
	{ 18202, 18202,  cs_PRJCOD_TRMER},
	{ 18203, 18203,  cs_PRJCOD_CSINI},
	{ 18204, 18205,  cs_PRJCOD_TRMER},
	{ 18211, 18212,  cs_PRJCOD_LM1SP},
	{ 18221, 18228,  cs_PRJCOD_TRMER},
	{ 18231, 18238,  cs_PRJCOD_LM1SP},
	{ 18240, 18253,  cs_PRJCOD_TRMER},
	{ 18260, 18263,  cs_PRJCOD_LM1SP},
	{ 18275, 18278,  cs_PRJCOD_TRMER},
	{ 18280, 18284,  cs_PRJCOD_OSTRO},
	{ 18285, 18285,  cs_PRJCOD_TRMER},
	{ 18286, 18286,  cs_PRJCOD_OSTRO},
	{ 18300, 18417,  cs_PRJCOD_TRMER},
	{ 18421, 18437,    cs_PRJCOD_END},
	{ 18441, 18452,  cs_PRJCOD_TRMER},
	{ 19854, 19854,  cs_PRJCOD_LM2SP},
	{ 19855, 19855, cs_PRJCOD_MRCATK},
	{ 19856, 19856,  cs_PRJCOD_TRMER},
	{ 19857, 19857,  cs_PRJCOD_LM2SP},
	{ 19858, 19858,  cs_PRJCOD_ALBER},
	{ 19859, 19859,  cs_PRJCOD_TRMER},
	{ 19860, 19860,  cs_PRJCOD_LM1SP},
	{ 19861, 19861,    cs_PRJCOD_END},
	{ 19862, 19863,  cs_PRJCOD_LM2SP},
	{ 19864, 19864,  cs_PRJCOD_TRMER},
	{ 19865, 19866, cs_PRJCOD_PSTROSL},
	{ 19867, 19868,  cs_PRJCOD_AZMEA},
	{ 19870, 19870,    cs_PRJCOD_END},
	{ 19871, 19872,  cs_PRJCOD_SWISS},
	{ 19873, 19875,  cs_PRJCOD_LM2SP},
	{ 19876, 19876,  cs_PRJCOD_TRMER},
	{ 19877, 19877,    cs_PRJCOD_END},
	{ 19879, 19879,  cs_PRJCOD_CSINI},
	{ 19880, 19882,  cs_PRJCOD_TRMER},
	{ 19883, 19883, cs_PRJCOD_MRCATK},
	{ 19884, 19884,  cs_PRJCOD_MRCAT},
	{ 19885, 19893,  cs_PRJCOD_CSINI},
	{ 19894, 19894, cs_PRJCOD_HOM1XY},
	{ 19895, 19895,  cs_PRJCOD_SWISS},
	{ 19896, 19896,  cs_PRJCOD_CSINI},
	{ 19897, 19897,  cs_PRJCOD_LM2SP},
	{ 19898, 19898, cs_PRJCOD_MRCATK},
	{ 19899, 19899,  cs_PRJCOD_LM1SP},
	{ 19900, 19900,  cs_PRJCOD_TRMER},
	{ 19901, 19901,  cs_PRJCOD_LM2SP},
	{ 19902, 19902,  cs_PRJCOD_LMBLG},
	{ 19903, 19903,  cs_PRJCOD_LM1SP},
	{ 19904, 19904,  cs_PRJCOD_TRMER},
	{ 19905, 19905, cs_PRJCOD_MRCATK},
	{ 19906, 19906,  cs_PRJCOD_LM1SP},
	{ 19907, 19908,  cs_PRJCOD_TRMER},
	{ 19909, 19910,  cs_PRJCOD_LM1SP},
	{ 19911, 19911, cs_PRJCOD_RSKEWO},
	{ 19913, 19914,  cs_PRJCOD_OSTRO},
	{ 19915, 19915,  cs_PRJCOD_LM1SP},
	{ 19916, 19916,  cs_PRJCOD_TRMER},
	{ 19917, 19917,  cs_PRJCOD_NZLND},
	{ 19919, 19919,  cs_PRJCOD_TRMER},
	{ 19920, 19920,  cs_PRJCOD_CSINI},
	{ 19921, 19921,  cs_PRJCOD_LM1SP},
	{ 19922, 19923, cs_PRJCOD_RSKEWO},
	{ 19924, 19925,  cs_PRJCOD_CSINI},
	{ 19926, 19927,  cs_PRJCOD_OSTRO},
	{ 19928, 19930,  cs_PRJCOD_TRMER},
	{ 19931, 19931, cs_PRJCOD_RSKEWO},
	{ 19933, 19933,  cs_PRJCOD_OSTRO},
	{ 19934, 19934,  cs_PRJCOD_TRMER},
	{ 19935, 19935,  cs_PRJCOD_SWISS},
	{ 19936, 19936,  cs_PRJCOD_TRMER},
	{ 19937, 19937,    cs_PRJCOD_END},
	{ 19938, 19938,  cs_PRJCOD_LM2SP},
	{ 19939, 19939,  cs_PRJCOD_TRMER},
	{ 19940, 19940,    cs_PRJCOD_END},
	{ 19941, 19941,  cs_PRJCOD_PLYCN},
	{ 19942, 19943,  cs_PRJCOD_TRMER},
	{ 19944, 19944,  cs_PRJCOD_LM2SP},
	{ 19945, 19946,  cs_PRJCOD_OSTRO},
	{ 19947, 19947,  cs_PRJCOD_LM2SP},
	{ 19948, 19948,  cs_PRJCOD_LM1SP},
	{ 19949, 19949,  cs_PRJCOD_OSTRO},
	{ 19950, 19951, cs_PRJCOD_RSKEWO},
	{ 19952, 19952, cs_PRJCOD_KROVAK},
	{ 19953, 19953,  cs_PRJCOD_CSINI},
	{ 19954, 19955,  cs_PRJCOD_TRMER},
	{ 19956, 19958, cs_PRJCOD_RSKEWO},
	{ 19959, 19959,  cs_PRJCOD_TRMER},
	{ 19960, 19960,  cs_PRJCOD_OSTRO},
	{ 19961, 19961,  cs_PRJCOD_LM2SP},
	{ 19962, 19964,  cs_PRJCOD_TRMER},
	{ 19965, 19965,  cs_PRJCOD_AZMEA},
	{ 19966, 19967,  cs_PRJCOD_TRMER},
	{ 19968, 19968,  cs_PRJCOD_EDCYLE},
	{ 19969, 19974,  cs_PRJCOD_TRMER},
	{ 19975, 19975,  cs_PRJCOD_CSINI},
	{ 19976, 19977,  cs_PRJCOD_LM2SP},
	{ 19978, 19978,  cs_PRJCOD_TRMER},
	{ 19979, 19979,    cs_PRJCOD_END},
	{ 19981, 19981,  cs_PRJCOD_LM2SP},
	{ 19982, 19982,  cs_PRJCOD_TRMER},
	{ 19983, 19983,    cs_PRJCOD_END},
	{ 19984, 19984,  cs_PRJCOD_ALBER},
	{ 19985, 19985,  cs_PRJCOD_LM2SP},
	{ 19986, 19986,  cs_PRJCOD_AZMEA},
	{ 19987, 19988,    cs_PRJCOD_END},
	{ 19989, 19989,  cs_PRJCOD_LM2SP},
	{ 19990, 19991,  cs_PRJCOD_TRMER},
	{ 19992, 19993,cs_PRJCOD_PSTROSL},
	{ 19994, 19994,  cs_PRJCOD_LM2SP},
	{ 19995, 19995,  cs_PRJCOD_TRMER},
	{ 19996, 19996,  cs_PRJCOD_CSINI},
	{     0,     0,    cs_PRJCOD_END}
};
