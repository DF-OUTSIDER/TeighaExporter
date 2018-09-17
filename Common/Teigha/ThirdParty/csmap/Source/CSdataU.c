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

#include "cs_map.h"

struct cs_Unittab_ cs_Unittab [] =
{
	/* The following entries define the relationshil between units
	   of length.  The factor is the value which one multiplies a
	   value by to obtain meters.  To convert meters to one of the
	   units listed, divide the meter value by the constant given. */

	{cs_UTYP_LEN,         "Meter",             "Meters",     "M", cs_USYS_Metric,         1.00000000000000000000,    9001  },
	{cs_UTYP_LEN,          "Foot",    "U.S.Survey Feet",    "FT", cs_USYS_English,        0.30480060960121920243,    9003  },
	{cs_UTYP_LEN,          "Inch",  "U.S.Survey Inches",    "IN", cs_USYS_English,        0.02540005080010160020,       0  },
	{cs_UTYP_LEN,         "IFoot",              "IFeet",   "IFT", cs_USYS_English,        0.30480000000000000000,    9002  },
	{cs_UTYP_LEN,    "ClarkeFoot",         "ClarkeFeet",    "CF", cs_USYS_English,        0.3047972651151,           9005  },
	{cs_UTYP_LEN,     "SearsFoot",          "SearsFeet",    "SF", cs_USYS_English,        0.30479947153867624624,    9041  },
	{cs_UTYP_LEN, "GoldCoastFoot",      "GoldCoastFeet",   "GCF", cs_USYS_English,        0.30479971018150881758,    9094  },
	{cs_UTYP_LEN,         "IInch",            "IInches",   "IIN", cs_USYS_English,        0.02540000000000000000,       0  },
	{cs_UTYP_LEN,     "MicroInch",        "MicroInches",    "uI", cs_USYS_English,        0.00000002540000000000000,    0  },
	{cs_UTYP_LEN,           "Mil",               "Mils",    "ML", cs_USYS_English,        0.00002540000000000,          0  },
	{cs_UTYP_LEN,    "Centimeter",        "Centimeters",    "CM", cs_USYS_Metric,        0.01000000000000000000,        0  },
	{cs_UTYP_LEN,     "Kilometer",         "Kilometers",    "KM", cs_USYS_Metric,     1000.00000000000000000000,     9036  },
	{cs_UTYP_LEN,          "Yard",   "U.S.Survey Yards",    "YD", cs_USYS_English,        0.91440182880365760731,       0  },
	{cs_UTYP_LEN,     "SearsYard",         "SearsYards",    "SY", cs_USYS_English,        0.914398414616029,         9040  },
	{cs_UTYP_LEN,    "IndianYard",        "IndianYards",   "InY", cs_USYS_English,        0.91439853074444079983,    9084  },
	{cs_UTYP_LEN,    "IndianYd37",        "IndianYds37",   "InY", cs_USYS_English,        0.91439523,                9085  },
	{cs_UTYP_LEN,    "IndianYd62",        "IndianYds62",   "InY", cs_USYS_English,        0.9143988,                 9086  },
	{cs_UTYP_LEN,    "IndianYd75",        "IndianYds75",   "InY", cs_USYS_English,        0.9143985,                 9087  },
	{cs_UTYP_LEN,    "IndianFoot",         "IndianFeet",  "InFt", cs_USYS_English,        0.30479951,                9080  },
	{cs_UTYP_LEN,    "IndianFt37",         "IndianFt37","InFt37", cs_USYS_English,        0.30479841,                9081  },
	{cs_UTYP_LEN,    "IndianFt62",         "IndianFt62","InFt62", cs_USYS_English,        0.3047996,                 9082  },
	{cs_UTYP_LEN,    "IndianFt75",         "IndianFt75","InFt75", cs_USYS_English,        0.3047995,                 9083  },
	{cs_UTYP_LEN,          "Mile",   "U.S.Survey Miles",    "MI", cs_USYS_English,     1609.34721869443738887477,       0  },
	{cs_UTYP_LEN,         "IYard",             "IYards",   "IYD", cs_USYS_English,        0.91440000000000000000,    9096  },
	{cs_UTYP_LEN,         "IMile",             "IMiles",   "IMI", cs_USYS_English,     1609.34400000000000000000,    9093  },
	{cs_UTYP_LEN,          "Knot",              "Knots",    "KT", cs_USYS_English,     1852.00000000000000000000,    9030  },
	{cs_UTYP_LEN,         "NautM",             "NautMs",    "NM", cs_USYS_English,     1852.00000000000000000000,    9030  },
	{cs_UTYP_LEN,        "Lat-66",            "Lat-66s",   "L66", cs_USYS_None,      110943.316488932731        ,       0  },
	{cs_UTYP_LEN,        "Lat-83",            "Lat-83s",   "L83", cs_USYS_None,      110946.25736872234125      ,       0  },
	{cs_UTYP_LEN,     "Decimeter",         "Decimeters",    "DM", cs_USYS_Metric,         0.10000000000000000000,       0  },
	{cs_UTYP_LEN,    "Millimeter",        "Millimeters",    "MM", cs_USYS_Metric,         0.00100000000000000000,       0  },
	{cs_UTYP_LEN,     "Dekameter",         "Dekameters",    "DK", cs_USYS_Metric,        10.00000000000000000000,       0  },
    {cs_UTYP_LEN,     "Decameter",         "Decameters",   "DAM", cs_USYS_Metric,        10.00000000000000000000,       0  },
	{cs_UTYP_LEN,    "Hectometer",        "Hectometers",    "HM", cs_USYS_Metric,       100.00000000000000000000,       0  },
	{cs_UTYP_LEN,   "GermanMeter",       "GermanMeters",    "GM", cs_USYS_Metric,         1.0000135965,              9031  },
	{cs_UTYP_LEN,        "CaGrid",            "CaGrids",    "CG", cs_USYS_English,        0.999738,                     0  },
	{cs_UTYP_LEN,   "ClarkeChain",       "ClarkeChains",    "CC", cs_USYS_English,       20.1166194976,              9038  },
	{cs_UTYP_LEN,   "GunterChain",       "GunterChains",    "GC", cs_USYS_English,       20.11684023368047,          9033  },
	{cs_UTYP_LEN,   "BenoitChain",       "BenoitChains",    "BC", cs_USYS_English,       20.116782494375872,         9062  },
	{cs_UTYP_LEN,    "SearsChain",        "SearsChains",    "SC", cs_USYS_English,       20.11676512155,             9042  },
	{cs_UTYP_LEN,    "ClarkeLink",        "ClarkeLinks",    "CL", cs_USYS_English,         .201166194976,            9039  },
	{cs_UTYP_LEN,    "GunterLink",        "GunterLinks",    "GL", cs_USYS_English,         .2011684023368047,        9034  },
	{cs_UTYP_LEN,    "BenoitLink",        "BenoitLinks",    "BL", cs_USYS_English,         .20116782494375872,       9063  },
	{cs_UTYP_LEN,     "SearsLink",         "SearsLinks",    "SL", cs_USYS_English,         .2011676512155,           9043  },
	{cs_UTYP_LEN,           "Rod",               "Rods",    "RD", cs_USYS_English,        5.02921005842012,             0  },
	{cs_UTYP_LEN,    "IntnlChain",        "IntnlChains",  "IChn", cs_USYS_English,       20.1168,                    9097  },
	{cs_UTYP_LEN,     "IntnlLink",         "IntnlLinks",  "ILnk", cs_USYS_English,         .201168,                  9098  },
	{cs_UTYP_LEN,         "Perch",            "Perches",    "PE", cs_USYS_English,        5.02921005842012,             0  },
	{cs_UTYP_LEN,          "Pole",              "Poles",    "PO", cs_USYS_English,        5.02921005842012,             0  },
	{cs_UTYP_LEN,       "Furlong",           "Furlongs",    "FU", cs_USYS_English,      201.1684023368046,              0  },
	{cs_UTYP_LEN,          "Rood",              "Roods",    "RO", cs_USYS_English,        3.778266898,                  0  },
	{cs_UTYP_LEN,      "CapeFoot",           "CapeFeet",    "CA", cs_USYS_English,        0.3047972615,                 0  },
	{cs_UTYP_LEN,       "Brealey",           "Brealeys",    "BR", cs_USYS_English,      375.00000000000000000000,       0  },
	{cs_UTYP_LEN,       "ModAmFt",          "ModAmFeet","MdAmFt", cs_USYS_English,        0.304812252984505969011938,   0  },
	{cs_UTYP_LEN,        "Fathom",            "Fathoms",  "Fthm", cs_USYS_English,        1.8288,                       0  },
	{cs_UTYP_LEN,      "NautM-UK",          "NautMs-UK", "NM/UK", cs_USYS_English,     1853.184,                        0  },
	{cs_UTYP_LEN,  "50kilometers",       "50Kilometers",  "50Km", cs_USYS_English,    50000.0,                          0  },
	{cs_UTYP_LEN, "150kilometers",      "150Kilometers", "150Km", cs_USYS_English,   150000.0,                          0  },
	{cs_UTYP_LEN,   "BrFootTrunc",        "BrFeetTrunc",  "BrFt", cs_USYS_English,        0.304799333333333333333,   9300  },
	{cs_UTYP_LEN,  "BrChainTrunc",       "BrCHainTrunc", "BrChn", cs_USYS_English,       20.116756,                  9301  },
	{cs_UTYP_LEN,   "BrLinkTrunc",        "BrLinkTrunc", "BrLnk", cs_USYS_English,        0.20116756,                9302  },

	/* The following entries define the relationship between units of
	   angular measure.  One multiplies a value by the given factor
	   to obtain a value in degrees.  To convert from degrees to
	   one of the units given, one divides the degree value by the
	   given factor. */

	{cs_UTYP_ANG,       "Degree",            "Degrees",    "DG", cs_USYS_None,           1.00000000000000000000,    9102  },
	{cs_UTYP_ANG,         "Grad",              "Grads",    "GR", cs_USYS_None,           0.90000000000000000000,    9105  },
	{cs_UTYP_ANG,        "Grade",             "Grades",    "GR", cs_USYS_None,           0.90000000000000000000,    9105  },
	{cs_UTYP_ANG,      "MapInfo",           "MapInfos",    "MA", cs_USYS_None,           0.00000100000000000000,       0  },
	{cs_UTYP_ANG,          "Mil",               "Mils",   "MIL", cs_USYS_None,           0.05625000000000000000,    9114  },
	{cs_UTYP_ANG,       "Minute",            "Minutes",    "MN", cs_USYS_None,           0.01666666666666666666,    9103  },
	{cs_UTYP_ANG,       "Radian",            "Radians",    "RA", cs_USYS_None,          57.2957795130823208772 ,    9101  },
	{cs_UTYP_ANG,       "Second",            "Seconds",    "SE", cs_USYS_None,           0.00027777777777777777,    9104  },
	{cs_UTYP_ANG,      "Decisec",           "Decisecs",    "DS", cs_USYS_None,           0.00002777777777777777,       0  },
	{cs_UTYP_ANG,     "Centisec",          "Centisecs",    "CS", cs_USYS_None,           0.00000277777777777777,       0  },
	{cs_UTYP_ANG,     "Millisec",          "Millisecs",    "MS", cs_USYS_None,           0.00000027777777777777,       0  },

	/* The following are essentially bogus unit definitions which are marked,
	   by virtue of the type, off (i.e. disabled).  These entries are replaced
	   by user defined entries.  There is no practical limit to the number
	   of slots can be added. */

	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },
	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },
	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },
	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },
	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },
	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },
	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },
	{cs_UTYP_OFF,      " <user>",            " <user>",    " z", cs_USYS_None,           0.00000000000000000000,       0  },

	/* The following entry terminates the table. */

	{cs_UTYP_END,           " z",                 " z",    " z", cs_USYS_None,          0.00000000000000000000,        0  }
};
