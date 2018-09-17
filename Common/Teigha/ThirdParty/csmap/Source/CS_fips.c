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

/*lint -esym(754,csFipsCodes_::adscode)    not referenced, can't remember of what this is */

const int csFipsCount = 3360;
const int csFipsNameCount = 2041;

const struct csFipsCodes_
{
	unsigned state:7;
	unsigned county:10;
	unsigned nad83:13;
	unsigned nad27:13;
	unsigned adscode:14;
	unsigned nameIndex:12;
} csFipsCodes [] = 
{
	{   1,    0,    0,    0,    0,    19 },	/* Alabama, Alabama */
	{   1,    1,  102,  102, 3126,   101 },	/* Autauga, Alabama */
	{   1,    3,  102,  102, 3126,   109 },	/* Baldwin, Alabama */
	{   1,    5,  101,  101, 3101,   121 },	/* Barbour, Alabama */
	{   1,    7,  102,  102, 3126,   174 },	/* Bibb, Alabama */
	{   1,    9,  102,  102, 3126,   191 },	/* Blount, Alabama */
	{   1,   11,  101,  101, 3101,   252 },	/* Bullock, Alabama */
	{   1,   13,  102,  102, 3126,   262 },	/* Butler, Alabama */
	{   1,   15,  101,  101, 3101,   275 },	/* Calhoun, Alabama */
	{   1,   17,  101,  101, 3101,   329 },	/* Chambers, Alabama */
	{   1,   19,  101,  101, 3101,   351 },	/* Cherokee, Alabama */
	{   1,   21,  102,  102, 3126,   361 },	/* Chilton, Alabama */
	{   1,   23,  102,  102, 3126,   365 },	/* Choctaw, Alabama */
	{   1,   25,  102,  102, 3126,   383 },	/* Clarke, Alabama */
	{   1,   27,  101,  101, 3101,   385 },	/* Clay, Alabama */
	{   1,   29,  101,  101, 3101,   390 },	/* Cleburne, Alabama */
	{   1,   31,  101,  101, 3101,   406 },	/* Coffee, Alabama */
	{   1,   33,  102,  102, 3126,   409 },	/* Colbert, Alabama */
	{   1,   35,  102,  102, 3126,   430 },	/* Conecuh, Alabama */
	{   1,   37,  101,  101, 3101,   440 },	/* Coosa, Alabama */
	{   1,   39,  101,  101, 3101,   451 },	/* Covington, Alabama */
	{   1,   41,  101,  101, 3101,   462 },	/* Crenshaw, Alabama */
	{   1,   43,  102,  102, 3126,   473 },	/* Cullman, Alabama */
	{   1,   45,  101,  101, 3101,   484 },	/* Dale, Alabama */
	{   1,   47,  102,  102, 3126,   486 },	/* Dallas, Alabama */
	{   1,   49,  101,  101, 3101,   503 },	/* De Kalb, Alabama */
	{   1,   51,  101,  101, 3101,   590 },	/* Elmore, Alabama */
	{   1,   53,  102,  102, 3126,   600 },	/* Escambia, Alabama */
	{   1,   55,  101,  101, 3101,   604 },	/* Etowah, Alabama */
	{   1,   57,  102,  102, 3126,   622 },	/* Fayette, Alabama */
	{   1,   59,  102,  102, 3126,   646 },	/* Franklin, Alabama */
	{   1,   61,  101,  101, 3101,   680 },	/* Geneva, Alabama */
	{   1,   63,  102,  102, 3126,   732 },	/* Greene, Alabama */
	{   1,   65,  102,  102, 3126,   761 },	/* Hale, Alabama */
	{   1,   67,  101,  101, 3101,   811 },	/* Henry, Alabama */
	{   1,   69,  101,  101, 3101,   844 },	/* Houston, Alabama */
	{   1,   71,  101,  101, 3101,   893 },	/* Jackson, Alabama */
	{   1,   73,  102,  102, 3126,   901 },	/* Jefferson, Alabama */
	{   1,   75,  102,  102, 3126,  1009 },	/* Lamar, Alabama */
	{   1,   77,  102,  102, 3126,  1029 },	/* Lauderdale, Alabama */
	{   1,   79,  102,  102, 3126,  1033 },	/* Lawrence, Alabama */
	{   1,   81,  101,  101, 3101,  1040 },	/* Lee, Alabama */
	{   1,   83,  102,  102, 3126,  1059 },	/* Limestone, Alabama */
	{   1,   85,  102,  102, 3126,  1082 },	/* Lowndes, Alabama */
	{   1,   87,  101,  101, 3101,  1098 },	/* Macon, Alabama */
	{   1,   89,  101,  101, 3101,  1101 },	/* Madison, Alabama */
	{   1,   91,  102,  102, 3126,  1119 },	/* Marengo, Alabama */
	{   1,   93,  102,  102, 3126,  1125 },	/* Marion, Alabama */
	{   1,   95,  101,  101, 3101,  1129 },	/* Marshall, Alabama */
	{   1,   97,  102,  102, 3126,  1216 },	/* Mobile, Alabama */
	{   1,   99,  102,  102, 3126,  1226 },	/* Monroe, Alabama */
	{   1,  101,  101,  101, 3101,  1232 },	/* Montgomery, Alabama */
	{   1,  103,  102,  102, 3126,  1240 },	/* Morgan, Alabama */
	{   1,  105,  102,  102, 3126,  1425 },	/* Perry, Alabama */
	{   1,  107,  102,  102, 3126,  1436 },	/* Pickens, Alabama */
	{   1,  109,  101,  101, 3101,  1439 },	/* Pike, Alabama */
	{   1,  111,  101,  101, 3101,  1513 },	/* Randolph, Alabama */
	{   1,  113,  101,  101, 3101,  1583 },	/* Russell, Alabama */
	{   1,  115,  101,  101, 3101,  1709 },	/* St Clair, Alabama */
	{   1,  117,  102,  102, 3126,  1668 },	/* Shelby, Alabama */
	{   1,  119,  102,  102, 3126,  1764 },	/* Sumter, Alabama */
	{   1,  121,  101,  101, 3101,  1781 },	/* Talladega, Alabama */
	{   1,  123,  101,  101, 3101,  1783 },	/* Tallapoosa, Alabama */
	{   1,  125,  102,  102, 3126,  1852 },	/* Tuscaloosa, Alabama */
	{   1,  127,  102,  102, 3126,  1913 },	/* Walker, Alabama */
	{   1,  129,  102,  102, 3126,  1933 },	/* Washington, Alabama */
	{   1,  131,  102,  102, 3126,  1974 },	/* Wilcox, Alabama */
	{   1,  133,  102,  102, 3126,  1993 },	/* Winston, Alabama */
	{   2,    0,    0,    0,    0,    24 },	/* Alaska, Alaska */
	{   2,   13,    0,    0,    0,    29 },	/* Aleutians East, Alaska */
	{   2,   16,    0,    0,    0,    30 },	/* Aleutians West, Alaska */
	{   2,   20,    0,    0,    0,    50 },	/* Anchorage, Alaska */
	{   2,   50,    0,    0,    0,   172 },	/* Bethel, Alaska */
	{   2,   60,    0,    0,    0,   232 },	/* Bristol Bay, Alaska */
	{   2,   70,    0,    0,    0,   527 },	/* Dillingham, Alaska */
	{   2,   90,    0,    0,    0,   608 },	/* Fairbanks North Star, Alaska */
	{   2,  100,    0,    0,    0,   760 },	/* Haines, Alaska */
	{   2,  110,    0,    0,    0,   923 },	/* Juneau, Alaska */
	{   2,  122,    0,    0,    0,   943 },	/* Kenai Peninsula, Alaska */
	{   2,  130,    0,    0,    0,   955 },	/* Ketchikan Gateway, Alaska */
	{   2,  150,    0,    0,    0,   983 },	/* Kodiak Island, Alaska */
	{   2,  164,    0,    0,    0,  1007 },	/* Lake and Peninsula, Alaska */
	{   2,  170,    0,    0,    0,  1138 },	/* Matanuska-Susitna, Alaska */
	{   2,  180,    0,    0,    0,  1314 },	/* Nome, Alaska */
	{   2,  185,    0,    0,    0,  1320 },	/* North Slope, Alaska */
	{   2,  188,    0,    0,    0,  1325 },	/* Northwest Artic, Alaska */
	{   2,  201,    0,    0,    0,  1491 },	/* Prince of Wales-Outer Ketchikan, Alaska */
	{   2,  220,    0,    0,    0,  1681 },	/* Sitka, Alaska */
	{   2,  231,    0,    0,    0,  1683 },	/* Skagway-Yakutat-Angoon, Alaska */
	{   2,  240,    0,    0,    0,  1700 },	/* Southeast Fairbanks, Alaska */
	{   2,  261,    0,    0,    0,  1876 },	/* Valdez-Cordova, Alaska */
	{   2,  270,    0,    0,    0,  1905 },	/* Wade Hampton, Alaska */
	{   2,  280,    0,    0,    0,  2009 },	/* Wrangell-Petersburg, Alaska */
	{   2,  290,    0,    0,    0,  2036 },	/* Yukon-Koyukuk, Alaska */
	{   4,    0,    0,    0,    0,    72 },	/* Arizona, Arizona */
	{   4,    1,  201,  201, 3151,    62 },	/* Apache, Arizona */
	{   4,    3,  201,  201, 3151,   401 },	/* Cochise, Arizona */
	{   4,    5,  202,  202, 3176,   404 },	/* Coconino, Arizona */
	{   4,    7,  201,  201, 3151,   686 },	/* Gila, Arizona */
	{   4,    9,  201,  201, 3151,   714 },	/* Graham, Arizona */
	{   4,   11,  201,  201, 3151,   733 },	/* Greenlee, Arizona */
	{   4,   12,  203,  203, 3201,   993 },	/* La Paz, Arizona */
	{   4,   13,  202,  202, 3176,  1121 },	/* Maricopa, Arizona */
	{   4,   15,  203,  203, 3201,  1220 },	/* Mohave, Arizona */
	{   4,   17,  201,  201, 3151,  1273 },	/* Navajo, Arizona */
	{   4,   19,  202,  202, 3176,  1440 },	/* Pima, Arizona */
	{   4,   21,  202,  202, 3176,  1441 },	/* Pinal, Arizona */
	{   4,   23,  202,  202, 3176,  1627 },	/* Santa Cruz, Arizona */
	{   4,   25,  202,  202, 3176,  2025 },	/* Yavapai, Arizona */
	{   4,   27,  203,  203, 3201,  2037 },	/* Yuma, Arizona */
	{   5,    0,  302,  302, 3251,    73 },	/* Arkansas, Arkansas */
	{   5,    1,    0,    0,    0,    73 },	/* Arkansas, Arkansas */
	{   5,    3,  302,  302, 3251,    83 },	/* Ashley, Arkansas */
	{   5,    5,  301,  301, 3226,   137 },	/* Baxter, Arkansas */
	{   5,    7,  301,  301, 3226,   163 },	/* Benton, Arkansas */
	{   5,    9,  301,  301, 3226,   201 },	/* Boone, Arkansas */
	{   5,   11,  302,  302, 3251,   218 },	/* Bradley, Arkansas */
	{   5,   13,  302,  302, 3251,   275 },	/* Calhoun, Arkansas */
	{   5,   15,  301,  301, 3226,   302 },	/* Carroll, Arkansas */
	{   5,   17,  302,  302, 3251,   359 },	/* Chicot, Arkansas */
	{   5,   19,  302,  302, 3251,   382 },	/* Clark, Arkansas */
	{   5,   21,  301,  301, 3226,   385 },	/* Clay, Arkansas */
	{   5,   23,  301,  301, 3226,   390 },	/* Cleburne, Arkansas */
	{   5,   25,  302,  302, 3251,   392 },	/* Cleveland, Arkansas */
	{   5,   27,  302,  302, 3251,   421 },	/* Columbia, Arkansas */
	{   5,   29,  301,  301, 3226,   435 },	/* Conway, Arkansas */
	{   5,   31,  301,  301, 3226,   457 },	/* Craighead, Arkansas */
	{   5,   33,  301,  301, 3226,   460 },	/* Crawford, Arkansas */
	{   5,   35,  301,  301, 3226,   464 },	/* Crittenden, Arkansas */
	{   5,   37,  301,  301, 3226,   468 },	/* Cross, Arkansas */
	{   5,   39,  302,  302, 3251,   486 },	/* Dallas, Arkansas */
	{   5,   41,  302,  302, 3251,   519 },	/* Desha, Arkansas */
	{   5,   43,  302,  302, 3251,   547 },	/* Drew, Arkansas */
	{   5,   45,  301,  301, 3226,   620 },	/* Faulkner, Arkansas */
	{   5,   47,  301,  301, 3226,   646 },	/* Franklin, Arkansas */
	{   5,   49,  301,  301, 3226,   656 },	/* Fulton, Arkansas */
	{   5,   51,  302,  302, 3251,   667 },	/* Garland, Arkansas */
	{   5,   53,  302,  302, 3251,   721 },	/* Grant, Arkansas */
	{   5,   55,  301,  301, 3226,   732 },	/* Greene, Arkansas */
	{   5,   57,  302,  302, 3251,   805 },	/* Hempstead, Arkansas */
	{   5,   59,  302,  302, 3251,   841 },	/* Hot Spring, Arkansas */
	{   5,   61,  302,  302, 3251,   845 },	/* Howard, Arkansas */
	{   5,   63,  301,  301, 3226,   869 },	/* Independence, Arkansas */
	{   5,   65,  301,  301, 3226,   890 },	/* Izard, Arkansas */
	{   5,   67,  301,  301, 3226,   893 },	/* Jackson, Arkansas */
	{   5,   69,  302,  302, 3251,   901 },	/* Jefferson, Arkansas */
	{   5,   71,  301,  301, 3226,   914 },	/* Johnson, Arkansas */
	{   5,   73,  302,  302, 3251,  1002 },	/* Lafayette, Arkansas */
	{   5,   75,  301,  301, 3226,  1033 },	/* Lawrence, Arkansas */
	{   5,   77,  302,  302, 3251,  1040 },	/* Lee, Arkansas */
	{   5,   79,  302,  302, 3251,  1060 },	/* Lincoln, Arkansas */
	{   5,   81,  302,  302, 3251,  1064 },	/* Little River, Arkansas */
	{   5,   83,  301,  301, 3226,  1068 },	/* Logan, Arkansas */
	{   5,   85,  302,  302, 3251,  1071 },	/* Lonoke, Arkansas */
	{   5,   87,  301,  301, 3226,  1101 },	/* Madison, Arkansas */
	{   5,   89,  301,  301, 3226,  1125 },	/* Marion, Arkansas */
	{   5,   91,  302,  302, 3251,  1202 },	/* Miller, Arkansas */
	{   5,   93,  301,  301, 3226,  1212 },	/* Mississippi, Arkansas */
	{   5,   95,  302,  302, 3251,  1226 },	/* Monroe, Arkansas */
	{   5,   97,  302,  302, 3251,  1232 },	/* Montgomery, Arkansas */
	{   5,   99,  302,  302, 3251,  1282 },	/* Nevada, Arkansas */
	{   5,  101,  301,  301, 3226,  1297 },	/* Newton, Arkansas */
	{   5,  103,  302,  302, 3251,  1376 },	/* Ouachita, Arkansas */
	{   5,  105,  301,  301, 3226,  1425 },	/* Perry, Arkansas */
	{   5,  107,  302,  302, 3251,  1433 },	/* Phillips, Arkansas */
	{   5,  109,  302,  302, 3251,  1439 },	/* Pike, Arkansas */
	{   5,  111,  301,  301, 3226,  1459 },	/* Poinsett, Arkansas */
	{   5,  113,  302,  302, 3251,  1461 },	/* Polk, Arkansas */
	{   5,  115,  301,  301, 3226,  1465 },	/* Pope, Arkansas */
	{   5,  117,  302,  302, 3251,  1479 },	/* Prairie, Arkansas */
	{   5,  119,  302,  302, 3226,  1497 },	/* Pulaski, Arkansas */
	{   5,  121,  301,  301, 3226,  1513 },	/* Randolph, Arkansas */
	{   5,  123,  301,  301, 3226,  1711 },	/* St Francis, Arkansas */
	{   5,  125,  302,  302, 3251,  1598 },	/* Saline, Arkansas */
	{   5,  127,  301,  301, 3226,  1647 },	/* Scott, Arkansas */
	{   5,  129,  301,  301, 3226,  1651 },	/* Searcy, Arkansas */
	{   5,  131,  301,  301, 3226,  1652 },	/* Sebastian, Arkansas */
	{   5,  133,  302,  302, 3251,  1658 },	/* Sevier, Arkansas */
	{   5,  135,  301,  301, 3226,  1663 },	/* Sharp, Arkansas */
	{   5,  137,  301,  301, 3226,  1750 },	/* Stone, Arkansas */
	{   5,  139,  302,  302, 3251,  1867 },	/* Union, Arkansas */
	{   5,  141,  301,  301, 3226,  1879 },	/* Van Buren, Arkansas */
	{   5,  143,  301,  301, 3226,  1933 },	/* Washington, Arkansas */
	{   5,  145,  301,  301, 3226,  1964 },	/* White, Arkansas */
	{   5,  147,  301,  301, 3226,  2001 },	/* Woodruff, Arkansas */
	{   5,  149,  301,  301, 3226,  2027 },	/* Yell, Arkansas */
	{   6,    0,    0,    0,    0,   276 },	/* California, California */
	{   6,    1,  403,  403, 3326,    22 },	/* Alameda, California */
	{   6,    3,  402,  402, 3301,    43 },	/* Alpine, California */
	{   6,    5,  402,  402, 3301,    44 },	/* Amador, California */
	{   6,    7,  402,  402, 3301,   263 },	/* Butte, California */
	{   6,    9,  403,  403, 3326,   271 },	/* Calaveras, California */
	{   6,   11,  402,  402, 3301,   424 },	/* Colusa, California */
	{   6,   13,  403,  403, 3326,   433 },	/* Contra Costa, California */
	{   6,   15,  401,  401, 3276,   511 },	/* Del Norte, California */
	{   6,   17,  402,  402, 3301,   581 },	/* El Dorado, California */
	{   6,   19,  404,  404, 3351,   653 },	/* Fresno, California */
	{   6,   21,  402,  402, 3301,   698 },	/* Glenn, California */
	{   6,   23,  401,  401, 3276,   854 },	/* Humboldt, California */
	{   6,   25,  406,  406, 3401,   868 },	/* Imperial, California */
	{   6,   27,  404,  404, 3351,   873 },	/* Inyo, California */
	{   6,   29,  405,  405, 3376,   952 },	/* Kern, California */
	{   6,   31,  404,  404, 3351,   970 },	/* Kings, California */
	{   6,   33,  402,  402, 3301,  1006 },	/* Lake, California */
	{   6,   35,  401,  401, 3276,  1026 },	/* Lassen, California */
	{   6,   37,  405,  405, 3376,  1074 },	/* Los Angeles, California */
	{   6,   39,  403,  403, 3326,  1100 },	/* Madera, California */
	{   6,   41,  403,  403, 3326,  1123 },	/* Marin, California */
	{   6,   43,  403,  403, 3326,  1126 },	/* Mariposa, California */
	{   6,   45,  402,  402, 3301,  1182 },	/* Mendocino, California */
	{   6,   47,  403,  403, 3326,  1185 },	/* Merced, California */
	{   6,   49,  401,  401, 3276,  1218 },	/* Modoc, California */
	{   6,   51,  403,  403, 3326,  1223 },	/* Mono, California */
	{   6,   53,  404,  404, 3351,  1230 },	/* Monterey, California */
	{   6,   55,  402,  402, 3301,  1267 },	/* Napa, California */
	{   6,   57,  402,  402, 3301,  1282 },	/* Nevada, California */
	{   6,   59,  406,  406, 3401,  1361 },	/* Orange, California */
	{   6,   61,  402,  402, 3301,  1451 },	/* Placer, California */
	{   6,   63,  401,  401, 3276,  1455 },	/* Plumas, California */
	{   6,   65,  406,  406, 3401,  1550 },	/* Riverside, California */
	{   6,   67,  402,  402, 3301,  1590 },	/* Sacramento, California */
	{   6,   69,  404,  404, 3351,  1603 },	/* San Benito, California */
	{   6,   71,  405,  405, 3376,  1604 },	/* San Bernardino, California */
	{   6,   73,  406,  406, 3401,  1605 },	/* San Diego, California */
	{   6,   75,  403,  403, 3326,  1606 },	/* San Francisco, California */
	{   6,   77,  403,  403, 3326,  1609 },	/* San Joaquin, California */
	{   6,   79,  405,  405, 3376,  1612 },	/* San Luis Obispo, California */
	{   6,   81,  403,  403, 3326,  1613 },	/* San Mateo, California */
	{   6,   83,  405,  405, 3376,  1625 },	/* Santa Barbara, California */
	{   6,   85,  403,  403, 3326,  1626 },	/* Santa Clara, California */
	{   6,   87,  403,  403, 3326,  1627 },	/* Santa Cruz, California */
	{   6,   89,  401,  401, 3276,  1664 },	/* Shasta, California */
	{   6,   91,  402,  402, 3301,  1676 },	/* Sierra, California */
	{   6,   93,  401,  401, 3276,  1680 },	/* Siskiyou, California */
	{   6,   95,  402,  402, 3301,  1691 },	/* Solano, California */
	{   6,   97,  402,  402, 3301,  1694 },	/* Sonoma, California */
	{   6,   99,  403,  403, 3326,  1731 },	/* Stanislaus, California */
	{   6,  101,  402,  402, 3301,  1769 },	/* Sutter, California */
	{   6,  103,  401,  401, 3276,  1793 },	/* Tehama, California */
	{   6,  105,  401,  401, 3276,  1840 },	/* Trinity, California */
	{   6,  107,  404,  404, 3351,  1847 },	/* Tulare, California */
	{   6,  109,  403,  403, 3326,  1850 },	/* Tuolumne, California */
	{   6,  111,  405,  405, 3376,  1887 },	/* Ventura, California */
	{   6,  113,  402,  402, 3301,  2032 },	/* Yolo, California */
	{   6,  115,  402,  402, 3301,  2035 },	/* Yuba, California */
	{   8,    0,    0,    0,    0,   419 },	/* Colorado, Colorado */
	{   8,    1,  501,  501, 3451,     5 },	/* Adams, Colorado */
	{   8,    3,  503,  503, 3501,    23 },	/* Alamosa, Colorado */
	{   8,    5,  502,  502, 3476,    67 },	/* Arapahoe, Colorado */
	{   8,    7,  503,  503, 3501,    69 },	/* Archuleta, Colorado */
	{   8,    9,  503,  503, 3501,   104 },	/* Baca, Colorado */
	{   8,   11,  503,  503, 3501,   162 },	/* Bent, Colorado */
	{   8,   13,  501,  501, 3451,   207 },	/* Boulder, Colorado */
	{   8,   15,  502,  502, 3476,   328 },	/* Chaffee, Colorado */
	{   8,   17,  502,  502, 3476,   357 },	/* Cheyenne, Colorado */
	{   8,   19,  502,  502, 3476,   387 },	/* Clear Creek, Colorado */
	{   8,   21,  503,  503, 3501,   431 },	/* Conejos, Colorado */
	{   8,   23,  503,  503, 3501,   447 },	/* Costilla, Colorado */
	{   8,   25,  503,  503, 3501,   470 },	/* Crowley, Colorado */
	{   8,   27,  503,  503, 3501,   479 },	/* Custer, Colorado */
	{   8,   29,  502,  502, 3476,   513 },	/* Delta, Colorado */
	{   8,   31,  502,  502, 3476,   516 },	/* Denver, Colorado */
	{   8,   33,  503,  503, 3501,   537 },	/* Dolores, Colorado */
	{   8,   35,  502,  502, 3476,   546 },	/* Douglas, Colorado */
	{   8,   37,  502,  502, 3476,   561 },	/* Eagle, Colorado */
	{   8,   39,  502,  502, 3476,   583 },	/* Elbert, Colorado */
	{   8,   41,  502,  502, 3476,   582 },	/* El Paso, Colorado */
	{   8,   43,  502,  502, 3476,   652 },	/* Fremont, Colorado */
	{   8,   45,  502,  502, 3476,   666 },	/* Garfield, Colorado */
	{   8,   47,  501,  501, 3451,   692 },	/* Gilpin, Colorado */
	{   8,   49,  501,  501, 3451,   716 },	/* Grand, Colorado */
	{   8,   51,  502,  502, 3476,   754 },	/* Gunnison, Colorado */
	{   8,   53,  503,  503, 3501,   825 },	/* Hinsdale, Colorado */
	{   8,   55,  503,  503, 3501,   851 },	/* Huerfano, Colorado */
	{   8,   57,  501,  501, 3451,   893 },	/* Jackson, Colorado */
	{   8,   59,  502,  502, 3476,   901 },	/* Jefferson, Colorado */
	{   8,   61,  503,  503, 3501,   973 },	/* Kiowa, Colorado */
	{   8,   63,  502,  502, 3476,   974 },	/* Kit Carson, Colorado */
	{   8,   65,  502,  502, 3476,  1006 },	/* Lake, Colorado */
	{   8,   67,  503,  503, 3501,   994 },	/* La Plata, Colorado */
	{   8,   69,  501,  501, 3451,  1021 },	/* Larimer, Colorado */
	{   8,   71,  503,  503, 3501,  1023 },	/* Las Animas, Colorado */
	{   8,   73,  502,  502, 3476,  1060 },	/* Lincoln, Colorado */
	{   8,   75,  501,  501, 3451,  1068 },	/* Logan, Colorado */
	{   8,   77,  502,  502, 3476,  1190 },	/* Mesa, Colorado */
	{   8,   79,  503,  503, 3501,  1206 },	/* Mineral, Colorado */
	{   8,   81,  501,  501, 3451,  1219 },	/* Moffat, Colorado */
	{   8,   83,  503,  503, 3501,  1231 },	/* Montezuma, Colorado */
	{   8,   85,  503,  503, 3501,  1235 },	/* Montrose, Colorado */
	{   8,   87,  501,  501, 3451,  1240 },	/* Morgan, Colorado */
	{   8,   89,  503,  503, 3501,  1371 },	/* Otero, Colorado */
	{   8,   91,  503,  503, 3501,  1377 },	/* Ouray, Colorado */
	{   8,   93,  502,  502, 3476,  1395 },	/* Park, Colorado */
	{   8,   95,  501,  501, 3451,  1433 },	/* Phillips, Colorado */
	{   8,   97,  502,  502, 3476,  1446 },	/* Pitkin, Colorado */
	{   8,   99,  503,  503, 3501,  1494 },	/* Prowers, Colorado */
	{   8,  101,  503,  503, 3501,  1495 },	/* Pueblo, Colorado */
	{   8,  103,  501,  501, 3451,  1546 },	/* Rio Blanco, Colorado */
	{   8,  105,  503,  503, 3501,  1547 },	/* Rio Grande, Colorado */
	{   8,  107,  501,  501, 3451,  1578 },	/* Routt, Colorado */
	{   8,  109,  503,  503, 3501,  1593 },	/* Saguache, Colorado */
	{   8,  111,  503,  503, 3501,  1610 },	/* San Juan, Colorado */
	{   8,  113,  503,  503, 3501,  1614 },	/* San Miguel, Colorado */
	{   8,  115,  501,  501, 3451,  1653 },	/* Sedgwick, Colorado */
	{   8,  117,  502,  502, 3476,  1762 },	/* Summit, Colorado */
	{   8,  119,  502,  502, 3476,  1795 },	/* Teller, Colorado */
	{   8,  121,  501,  501, 3451,  1933 },	/* Washington, Colorado */
	{   8,  123,  501,  501, 3451,  1948 },	/* Weld, Colorado */
	{   8,  125,  501,  501, 3451,  2037 },	/* Yuma, Colorado */
	{   9,    0,    0,    0,    0,   432 },	/* Connecticut, Connecticut */
	{   9,    1,  600,  600, 3526,   611 },	/* Fairfield, Connecticut */
	{   9,    3,  600,  600, 3526,   792 },	/* Hartford, Connecticut */
	{   9,    5,  600,  600, 3526,  1063 },	/* Litchfield, Connecticut */
	{   9,    7,  600,  600, 3526,  1194 },	/* Middlesex, Connecticut */
	{   9,    9,  600,  600, 3526,  1286 },	/* New Haven, Connecticut */
	{   9,   11,  600,  600, 3526,  1289 },	/* New London, Connecticut */
	{   9,   13,  600,  600, 3526,  1821 },	/* Tolland, Connecticut */
	{   9,   15,  600,  600, 3526,  1986 },	/* Windham, Connecticut */
	{  10,    0,    0,    0,    0,   512 },	/* Delaware, Delaware */
	{  10,    1,  700,  700, 3551,   948 },	/* Kent, Delaware */
	{  10,    3,  700,  700, 3551,  1283 },	/* New Castle, Delaware */
	{  10,    5,  700,  700, 3551,  1768 },	/* Sussex, Delaware */
	{  11,    0,    0,    0,    0,   531 },	/* District of Columbia, District of Columbia */
	{  11,    1,    0,    0,    0,   531 },	/* District of Columbia, District of Columbia */
	{  12,    0,    0,    0,    0,   634 },	/* Florida, Florida */
	{  12,    1,  903,  903, 3576,    20 },	/* Alachua, Florida */
	{  12,    3,  903,  903, 3576,   107 },	/* Baker, Florida */
	{  12,    5,  903,  903, 3576,   138 },	/* Bay, Florida */
	{  12,    7,  903,  903, 3576,   217 },	/* Bradford, Florida */
	{  12,    9,  901,  901, 3601,   227 },	/* Brevard, Florida */
	{  12,   11,  901,  901, 3601,   239 },	/* Broward, Florida */
	{  12,   13,  903,  903, 3576,   275 },	/* Calhoun, Florida */
	{  12,   15,  902,  902, 3626,   337 },	/* Charlotte, Florida */
	{  12,   17,  902,  902, 3626,   375 },	/* Citrus, Florida */
	{  12,   19,  901,  901, 3601,   385 },	/* Clay, Florida */
	{  12,   21,  901,  901, 3601,   415 },	/* Collier, Florida */
	{  12,   23,  903,  903, 3576,   421 },	/* Columbia, Florida */
	{  12,   25,  901,  901, 3601,   481 },	/* Dade, Florida */
	{  12,   27,  902,  902, 3626,   504 },	/* De Soto, Florida */
	{  12,   29,  903,  903, 3576,   533 },	/* Dixie, Florida */
	{  12,   31,  901,  901, 3601,   559 },	/* Duval, Florida */
	{  12,   33,  903,  903, 3576,   600 },	/* Escambia, Florida */
	{  12,   35,  901,  901, 3601,   630 },	/* Flagler, Florida */
	{  12,   37,  903,  903, 3576,   646 },	/* Franklin, Florida */
	{  12,   39,  903,  903, 3576,   658 },	/* Gadsden, Florida */
	{  12,   41,  903,  903, 3576,   687 },	/* Gilchrist, Florida */
	{  12,   43,  901,  901, 3601,   694 },	/* Glades, Florida */
	{  12,   45,  903,  903, 3576,   753 },	/* Gulf, Florida */
	{  12,   47,  903,  903, 3576,   765 },	/* Hamilton, Florida */
	{  12,   49,  902,  902, 3626,   777 },	/* Hardee, Florida */
	{  12,   51,  901,  901, 3601,   808 },	/* Hendry, Florida */
	{  12,   53,  902,  902, 3626,   813 },	/* Hernando, Florida */
	{  12,   55,  901,  901, 3601,   820 },	/* Highlands, Florida */
	{  12,   57,  902,  902, 3626,   822 },	/* Hillsborough, Florida */
	{  12,   59,  903,  903, 3576,   831 },	/* Holmes, Florida */
	{  12,   61,  901,  901, 3601,   870 },	/* Indian River, Florida */
	{  12,   63,  903,  903, 3576,   893 },	/* Jackson, Florida */
	{  12,   65,  903,  903, 3576,   901 },	/* Jefferson, Florida */
	{  12,   67,  903,  903, 3576,  1002 },	/* Lafayette, Florida */
	{  12,   69,  901,  901, 3601,  1006 },	/* Lake, Florida */
	{  12,   71,  902,  902, 3626,  1040 },	/* Lee, Florida */
	{  12,   73,  903,  903, 3576,  1047 },	/* Leon, Florida */
	{  12,   75,  902,  902, 3626,  1050 },	/* Levy, Florida */
	{  12,   77,  903,  903, 3576,  1056 },	/* Liberty, Florida */
	{  12,   79,  903,  903, 3576,  1101 },	/* Madison, Florida */
	{  12,   81,  902,  902, 3626,  1113 },	/* Manatee, Florida */
	{  12,   83,  902,  902, 3626,  1125 },	/* Marion, Florida */
	{  12,   85,  901,  901, 3601,  1131 },	/* Martin, Florida */
	{  12,   87,  901,  901, 3601,  1226 },	/* Monroe, Florida */
	{  12,   89,  901,  901, 3601,  1270 },	/* Nassau, Florida */
	{  12,   91,  903,  903, 3576,  1346 },	/* Okaloosa, Florida */
	{  12,   93,  901,  901, 3601,  1348 },	/* Okeechobee, Florida */
	{  12,   95,  901,  901, 3601,  1361 },	/* Orange, Florida */
	{  12,   97,  901,  901, 3601,  1368 },	/* Osceola, Florida */
	{  12,   99,  901,  901, 3601,  1389 },	/* Palm Beach, Florida */
	{  12,  101,  902,  902, 3626,  1399 },	/* Pasco, Florida */
	{  12,  103,  902,  902, 3626,  1443 },	/* Pinellas, Florida */
	{  12,  105,  902,  902, 3626,  1461 },	/* Polk, Florida */
	{  12,  107,  901,  901, 3601,  1499 },	/* Putnam, Florida */
	{  12,  109,  901,  901, 3601,  1718 },	/* St Johns, Florida */
	{  12,  111,  901,  901, 3601,  1724 },	/* St Lucie, Florida */
	{  12,  113,  903,  903, 3576,  1630 },	/* Santa Rosa, Florida */
	{  12,  115,  902,  902, 3626,  1631 },	/* Sarasota, Florida */
	{  12,  117,  901,  901, 3601,  1654 },	/* Seminole, Florida */
	{  12,  119,  902,  902, 3626,  1764 },	/* Sumter, Florida */
	{  12,  121,  903,  903, 3576,  1771 },	/* Suwannee, Florida */
	{  12,  123,  903,  903, 3576,  1791 },	/* Taylor, Florida */
	{  12,  125,  903,  903, 3576,  1867 },	/* Union, Florida */
	{  12,  127,  901,  901, 3601,  1901 },	/* Volusia, Florida */
	{  12,  129,  903,  903, 3576,  1911 },	/* Wakulla, Florida */
	{  12,  131,  903,  903, 3576,  1920 },	/* Walton, Florida */
	{  12,  133,  903,  903, 3576,  1933 },	/* Washington, Florida */
	{  13,    0,    0,    0,    0,   684 },	/* Georgia, Georgia */
	{  13,    1, 1001, 1001, 3651,    64 },	/* Appling, Georgia */
	{  13,    3, 1001, 1001, 3651,    90 },	/* Atkinson, Georgia */
	{  13,    5, 1001, 1001, 3651,   105 },	/* Bacon, Georgia */
	{  13,    7, 1002, 1002, 3676,   107 },	/* Baker, Georgia */
	{  13,    9, 1001, 1001, 3651,   109 },	/* Baldwin, Georgia */
	{  13,   11, 1002, 1002, 3676,   116 },	/* Banks, Georgia */
	{  13,   13, 1002, 1002, 3676,   129 },	/* Barrow, Georgia */
	{  13,   15, 1002, 1002, 3676,   133 },	/* Bartow, Georgia */
	{  13,   17, 1002, 1002, 3676,   157 },	/* Ben Hill, Georgia */
	{  13,   19, 1002, 1002, 3676,   170 },	/* Berrien, Georgia */
	{  13,   21, 1002, 1002, 3676,   174 },	/* Bibb, Georgia */
	{  13,   23, 1002, 1002, 3676,   189 },	/* Bleckley, Georgia */
	{  13,   25, 1001, 1001, 3651,   220 },	/* Brantley, Georgia */
	{  13,   27, 1002, 1002, 3676,   237 },	/* Brooks, Georgia */
	{  13,   29, 1001, 1001, 3651,   243 },	/* Bryan, Georgia */
	{  13,   31, 1001, 1001, 3651,   251 },	/* Bulloch, Georgia */
	{  13,   33, 1001, 1001, 3651,   255 },	/* Burke, Georgia */
	{  13,   35, 1002, 1002, 3676,   264 },	/* Butts, Georgia */
	{  13,   37, 1002, 1002, 3676,   275 },	/* Calhoun, Georgia */
	{  13,   39, 1001, 1001, 3651,   284 },	/* Camden, Georgia */
	{  13,   43, 1001, 1001, 3651,   290 },	/* Candler, Georgia */
	{  13,   45, 1002, 1002, 3676,   302 },	/* Carroll, Georgia */
	{  13,   47, 1002, 1002, 3676,   317 },	/* Catoosa, Georgia */
	{  13,   49, 1001, 1001, 3651,   339 },	/* Charlton, Georgia */
	{  13,   51, 1001, 1001, 3651,   341 },	/* Chatham, Georgia */
	{  13,   53, 1002, 1002, 3676,   342 },	/* Chattahoochee, Georgia */
	{  13,   55, 1002, 1002, 3676,   343 },	/* Chattooga, Georgia */
	{  13,   57, 1002, 1002, 3676,   351 },	/* Cherokee, Georgia */
	{  13,   59, 1002, 1002, 3676,   383 },	/* Clarke, Georgia */
	{  13,   61, 1002, 1002, 3676,   385 },	/* Clay, Georgia */
	{  13,   63, 1002, 1002, 3676,   386 },	/* Clayton, Georgia */
	{  13,   65, 1001, 1001, 3651,   394 },	/* Clinch, Georgia */
	{  13,   67, 1002, 1002, 3676,   400 },	/* Cobb, Georgia */
	{  13,   69, 1001, 1001, 3651,   406 },	/* Coffee, Georgia */
	{  13,   71, 1002, 1002, 3676,   420 },	/* Colquitt, Georgia */
	{  13,   73, 1001, 1001, 3651,   421 },	/* Columbia, Georgia */
	{  13,   75, 1002, 1002, 3676,   436 },	/* Cook, Georgia */
	{  13,   77, 1002, 1002, 3676,   453 },	/* Coweta, Georgia */
	{  13,   79, 1002, 1002, 3676,   460 },	/* Crawford, Georgia */
	{  13,   81, 1002, 1002, 3676,   463 },	/* Crisp, Georgia */
	{  13,   83, 1002, 1002, 3676,   481 },	/* Dade, Georgia */
	{  13,   85, 1002, 1002, 3676,   500 },	/* Dawson, Georgia */
	{  13,   87, 1002, 1002, 3676,   508 },	/* Decatur, Georgia */
	{  13,   89, 1002, 1002, 3676,   503 },	/* De Kalb, Georgia */
	{  13,   91, 1001, 1001, 3651,   536 },	/* Dodge, Georgia */
	{  13,   93, 1002, 1002, 3676,   541 },	/* Dooly, Georgia */
	{  13,   95, 1002, 1002, 3676,   545 },	/* Dougherty, Georgia */
	{  13,   97, 1002, 1002, 3676,   546 },	/* Douglas, Georgia */
	{  13,   99, 1002, 1002, 3676,   562 },	/* Early, Georgia */
	{  13,  101, 1001, 1001, 3651,   571 },	/* Echols, Georgia */
	{  13,  103, 1001, 1001, 3651,   580 },	/* Effingham, Georgia */
	{  13,  105, 1001, 1001, 3651,   583 },	/* Elbert, Georgia */
	{  13,  107, 1001, 1001, 3651,   591 },	/* Emanuel, Georgia */
	{  13,  109, 1001, 1001, 3651,   607 },	/* Evans, Georgia */
	{  13,  111, 1002, 1002, 3676,   617 },	/* Fannin, Georgia */
	{  13,  113, 1002, 1002, 3676,   622 },	/* Fayette, Georgia */
	{  13,  115, 1002, 1002, 3676,   635 },	/* Floyd, Georgia */
	{  13,  117, 1002, 1002, 3676,   642 },	/* Forsyth, Georgia */
	{  13,  119, 1001, 1001, 3651,   646 },	/* Franklin, Georgia */
	{  13,  121, 1002, 1002, 3676,   656 },	/* Fulton, Georgia */
	{  13,  123, 1002, 1002, 3676,   691 },	/* Gilmer, Georgia */
	{  13,  125, 1001, 1001, 3651,   696 },	/* Glascock, Georgia */
	{  13,  127, 1001, 1001, 3651,   700 },	/* Glynn, Georgia */
	{  13,  129, 1002, 1002, 3676,   708 },	/* Gordon, Georgia */
	{  13,  131, 1002, 1002, 3676,   712 },	/* Grady, Georgia */
	{  13,  133, 1001, 1001, 3651,   732 },	/* Greene, Georgia */
	{  13,  135, 1002, 1002, 3676,   757 },	/* Gwinnett, Georgia */
	{  13,  137, 1002, 1002, 3676,   759 },	/* Habersham, Georgia */
	{  13,  139, 1002, 1002, 3676,   763 },	/* Hall, Georgia */
	{  13,  141, 1001, 1001, 3651,   771 },	/* Hancock, Georgia */
	{  13,  143, 1002, 1002, 3676,   776 },	/* Haralson, Georgia */
	{  13,  145, 1002, 1002, 3676,   788 },	/* Harris, Georgia */
	{  13,  147, 1001, 1001, 3651,   791 },	/* Hart, Georgia */
	{  13,  149, 1002, 1002, 3676,   803 },	/* Heard, Georgia */
	{  13,  151, 1002, 1002, 3676,   811 },	/* Henry, Georgia */
	{  13,  153, 1002, 1002, 3676,   844 },	/* Houston, Georgia */
	{  13,  155, 1002, 1002, 3676,   881 },	/* Irwin, Georgia */
	{  13,  157, 1002, 1002, 3676,   893 },	/* Jackson, Georgia */
	{  13,  159, 1002, 1002, 3676,   897 },	/* Jasper, Georgia */
	{  13,  161, 1001, 1001, 3651,   900 },	/* Jeff Davis, Georgia */
	{  13,  163, 1001, 1001, 3651,   901 },	/* Jefferson, Georgia */
	{  13,  165, 1001, 1001, 3651,   904 },	/* Jenkins, Georgia */
	{  13,  167, 1001, 1001, 3651,   914 },	/* Johnson, Georgia */
	{  13,  169, 1002, 1002, 3676,   917 },	/* Jones, Georgia */
	{  13,  171, 1002, 1002, 3676,  1009 },	/* Lamar, Georgia */
	{  13,  173, 1002, 1002, 3676,  1017 },	/* Lanier, Georgia */
	{  13,  175, 1001, 1001, 3651,  1031 },	/* Laurens, Georgia */
	{  13,  177, 1002, 1002, 3676,  1040 },	/* Lee, Georgia */
	{  13,  179, 1001, 1001, 3651,  1056 },	/* Liberty, Georgia */
	{  13,  181, 1001, 1001, 3651,  1060 },	/* Lincoln, Georgia */
	{  13,  183, 1001, 1001, 3651,  1070 },	/* Long, Georgia */
	{  13,  185, 1002, 1002, 3676,  1082 },	/* Lowndes, Georgia */
	{  13,  187, 1002, 1002, 3676,  1086 },	/* Lumpkin, Georgia */
	{  13,  189, 1001, 1001, 3651,  1156 },	/* Mc Duffie, Georgia */
	{  13,  191, 1001, 1001, 3651,  1158 },	/* Mc Intosh, Georgia */
	{  13,  193, 1002, 1002, 3676,  1098 },	/* Macon, Georgia */
	{  13,  195, 1001, 1001, 3651,  1101 },	/* Madison, Georgia */
	{  13,  197, 1002, 1002, 3676,  1125 },	/* Marion, Georgia */
	{  13,  199, 1002, 1002, 3676,  1187 },	/* Meriwether, Georgia */
	{  13,  201, 1002, 1002, 3676,  1202 },	/* Miller, Georgia */
	{  13,  205, 1002, 1002, 3676,  1215 },	/* Mitchell, Georgia */
	{  13,  207, 1002, 1002, 3676,  1226 },	/* Monroe, Georgia */
	{  13,  209, 1001, 1001, 3651,  1232 },	/* Montgomery, Georgia */
	{  13,  211, 1002, 1002, 3676,  1240 },	/* Morgan, Georgia */
	{  13,  213, 1002, 1002, 3676,  1253 },	/* Murray, Georgia */
	{  13,  215, 1002, 1002, 3676,  1255 },	/* Muscogee, Georgia */
	{  13,  217, 1002, 1002, 3676,  1297 },	/* Newton, Georgia */
	{  13,  219, 1002, 1002, 3676,  1340 },	/* Oconee, Georgia */
	{  13,  221, 1001, 1001, 3651,  1344 },	/* Oglethorpe, Georgia */
	{  13,  223, 1002, 1002, 3676,  1404 },	/* Paulding, Georgia */
	{  13,  225, 1002, 1002, 3676,  1408 },	/* Peach, Georgia */
	{  13,  227, 1002, 1002, 3676,  1436 },	/* Pickens, Georgia */
	{  13,  229, 1001, 1001, 3651,  1438 },	/* Pierce, Georgia */
	{  13,  231, 1002, 1002, 3676,  1439 },	/* Pike, Georgia */
	{  13,  233, 1002, 1002, 3676,  1461 },	/* Polk, Georgia */
	{  13,  235, 1002, 1002, 3676,  1497 },	/* Pulaski, Georgia */
	{  13,  237, 1002, 1002, 3676,  1499 },	/* Putnam, Georgia */
	{  13,  239, 1002, 1002, 3676,  1504 },	/* Quitman, Georgia */
	{  13,  241, 1002, 1002, 3676,  1505 },	/* Rabun, Georgia */
	{  13,  243, 1002, 1002, 3676,  1513 },	/* Randolph, Georgia */
	{  13,  245, 1001, 1001, 3651,  1540 },	/* Richmond, Georgia */
	{  13,  247, 1002, 1002, 3676,  1561 },	/* Rockdale, Georgia */
	{  13,  249, 1002, 1002, 3676,  1640 },	/* Schley, Georgia */
	{  13,  251, 1001, 1001, 3651,  1649 },	/* Screven, Georgia */
	{  13,  253, 1002, 1002, 3676,  1654 },	/* Seminole, Georgia */
	{  13,  255, 1002, 1002, 3676,  1701 },	/* Spalding, Georgia */
	{  13,  257, 1001, 1001, 3651,  1741 },	/* Stephens, Georgia */
	{  13,  259, 1002, 1002, 3676,  1746 },	/* Stewart, Georgia */
	{  13,  261, 1002, 1002, 3676,  1764 },	/* Sumter, Georgia */
	{  13,  263, 1002, 1002, 3676,  1779 },	/* Talbot, Georgia */
	{  13,  265, 1001, 1001, 3651,  1780 },	/* Taliaferro, Georgia */
	{  13,  267, 1001, 1001, 3651,  1790 },	/* Tattnall, Georgia */
	{  13,  269, 1002, 1002, 3676,  1791 },	/* Taylor, Georgia */
	{  13,  271, 1001, 1001, 3651,  1794 },	/* Telfair, Georgia */
	{  13,  273, 1002, 1002, 3676,  1799 },	/* Terrell, Georgia */
	{  13,  275, 1002, 1002, 3676,  1804 },	/* Thomas, Georgia */
	{  13,  277, 1002, 1002, 3676,  1807 },	/* Tift, Georgia */
	{  13,  279, 1001, 1001, 3651,  1826 },	/* Toombs, Georgia */
	{  13,  281, 1002, 1002, 3676,  1829 },	/* Towns, Georgia */
	{  13,  283, 1001, 1001, 3651,  1837 },	/* Treutlen, Georgia */
	{  13,  285, 1002, 1002, 3676,  1842 },	/* Troup, Georgia */
	{  13,  287, 1002, 1002, 3676,  1851 },	/* Turner, Georgia */
	{  13,  289, 1002, 1002, 3676,  1855 },	/* Twiggs, Georgia */
	{  13,  291, 1002, 1002, 3676,  1867 },	/* Union, Georgia */
	{  13,  293, 1002, 1002, 3676,  1869 },	/* Upson, Georgia */
	{  13,  295, 1002, 1002, 3676,  1913 },	/* Walker, Georgia */
	{  13,  297, 1002, 1002, 3676,  1920 },	/* Walton, Georgia */
	{  13,  299, 1001, 1001, 3651,  1924 },	/* Ware, Georgia */
	{  13,  301, 1001, 1001, 3651,  1925 },	/* Warren, Georgia */
	{  13,  303, 1001, 1001, 3651,  1933 },	/* Washington, Georgia */
	{  13,  305, 1001, 1001, 3651,  1942 },	/* Wayne, Georgia */
	{  13,  307, 1002, 1002, 3676,  1947 },	/* Webster, Georgia */
	{  13,  309, 1001, 1001, 3651,  1963 },	/* Wheeler, Georgia */
	{  13,  311, 1002, 1002, 3676,  1964 },	/* White, Georgia */
	{  13,  313, 1002, 1002, 3676,  1967 },	/* Whitfield, Georgia */
	{  13,  315, 1002, 1002, 3676,  1974 },	/* Wilcox, Georgia */
	{  13,  317, 1001, 1001, 3651,  1975 },	/* Wilkes, Georgia */
	{  13,  319, 1001, 1001, 3651,  1977 },	/* Wilkinson, Georgia */
	{  13,  321, 1002, 1002, 3676,  2006 },	/* Worth, Georgia */
	{  15,    0, 5101, 5101,    0,   798 },	/* Hawaii, Hawaii */
	{  15,    1, 5103, 5103,    0,   833 },	/* Honolulu, Hawaii */
	{  15,    3, 5102, 5102,    0,   926 },	/* Kalawao, Hawaii */
	{  15,    5, 5104, 5104,    0,   935 },	/* Kauai, Hawaii */
	{  15,    7, 5102, 5102,    0,  1140 },	/* Maui, Hawaii */
	{  16,    0, 1103, 1103, 3751,   866 },	/* Idaho, Idaho */
	{  16,    1, 1103, 1103, 3751,     3 },	/* Ada, Idaho */
	{  16,    3, 1103, 1103, 3751,     5 },	/* Adams, Idaho */
	{  16,    5, 1101, 1101, 3701,   118 },	/* Bannock, Idaho */
	{  16,    7, 1101, 1101, 3701,   143 },	/* Bear Lake, Idaho */
	{  16,    9, 1103, 1103, 3751,   158 },	/* Benewah, Idaho */
	{  16,   11, 1101, 1101, 3701,   181 },	/* Bingham, Idaho */
	{  16,   13, 1102, 1102, 3726,   185 },	/* Blaine, Idaho */
	{  16,   15, 1103, 1103, 3751,   193 },	/* Boise, Idaho */
	{  16,   17, 1103, 1103, 3751,   199 },	/* Bonner, Idaho */
	{  16,   19, 1101, 1101, 3701,   200 },	/* Bonneville, Idaho */
	{  16,   21, 1103, 1103, 3751,   208 },	/* Boundary, Idaho */
	{  16,   23, 1102, 1102, 3726,   263 },	/* Butte, Idaho */
	{  16,   25, 1102, 1102, 3726,   282 },	/* Camas, Idaho */
	{  16,   27, 1103, 1103, 3751,   293 },	/* Canyon, Idaho */
	{  16,   29, 1101, 1101, 3701,   297 },	/* Caribou, Idaho */
	{  16,   31, 1102, 1102, 3726,   311 },	/* Cassia, Idaho */
	{  16,   33, 1101, 1101, 3701,   382 },	/* Clark, Idaho */
	{  16,   35, 1103, 1103, 3751,   389 },	/* Clearwater, Idaho */
	{  16,   37, 1102, 1102, 3726,   479 },	/* Custer, Idaho */
	{  16,   39, 1103, 1103, 3751,   590 },	/* Elmore, Idaho */
	{  16,   41, 1101, 1101, 3701,   646 },	/* Franklin, Idaho */
	{  16,   43, 1101, 1101, 3701,   652 },	/* Fremont, Idaho */
	{  16,   45, 1103, 1103, 3751,   678 },	/* Gem, Idaho */
	{  16,   47, 1102, 1102, 3726,   707 },	/* Gooding, Idaho */
	{  16,   49,    0,    0,    0,   866 },	/* Idaho, Idaho */
	{  16,   51, 1101, 1101, 3701,   901 },	/* Jefferson, Idaho */
	{  16,   53, 1102, 1102, 3726,   907 },	/* Jerome, Idaho */
	{  16,   55, 1103, 1103, 3751,   985 },	/* Kootenai, Idaho */
	{  16,   57, 1103, 1103, 3751,  1027 },	/* Latah, Idaho */
	{  16,   59, 1102, 1102, 3726,  1044 },	/* Lemhi, Idaho */
	{  16,   61, 1103, 1103, 3751,  1051 },	/* Lewis, Idaho */
	{  16,   63, 1102, 1102, 3726,  1060 },	/* Lincoln, Idaho */
	{  16,   65, 1101, 1101, 3701,  1101 },	/* Madison, Idaho */
	{  16,   67, 1102, 1102, 3726,  1208 },	/* Minidoka, Idaho */
	{  16,   69, 1103, 1103, 3751,  1298 },	/* Nez Perce, Idaho */
	{  16,   71, 1101, 1101, 3701,  1356 },	/* Oneida, Idaho */
	{  16,   73, 1103, 1103, 3751,  1382 },	/* Owyhee, Idaho */
	{  16,   75, 1103, 1103, 3751,  1406 },	/* Payette, Idaho */
	{  16,   77, 1101, 1101, 3701,  1476 },	/* Power, Idaho */
	{  16,   79, 1103, 1103, 3751,  1674 },	/* Shoshone, Idaho */
	{  16,   81, 1101, 1101, 3701,  1801 },	/* Teton, Idaho */
	{  16,   83, 1102, 1102, 3726,  1856 },	/* Twin Falls, Idaho */
	{  16,   85, 1103, 1103, 3751,  1878 },	/* Valley, Idaho */
	{  16,   87, 1103, 1103, 3751,  1933 },	/* Washington, Idaho */
	{  16,   89,    0,    0,    0,  2030 },	/* Yellowstone National Park, Idaho */
	{  17,    0,    0,    0,    0,   867 },	/* Illinois, Illinois */
	{  17,    1, 1202, 1202, 3801,     5 },	/* Adams, Illinois */
	{  17,    3, 1202, 1202, 3801,    31 },	/* Alexander, Illinois */
	{  17,    5, 1202, 1202, 3801,   198 },	/* Bond, Illinois */
	{  17,    7, 1201, 1201, 3776,   201 },	/* Boone, Illinois */
	{  17,    9, 1202, 1202, 3801,   240 },	/* Brown, Illinois */
	{  17,   11, 1202, 1202, 3801,   254 },	/* Bureau, Illinois */
	{  17,   13, 1202, 1202, 3801,   275 },	/* Calhoun, Illinois */
	{  17,   15, 1202, 1202, 3801,   302 },	/* Carroll, Illinois */
	{  17,   17, 1202, 1202, 3801,   310 },	/* Cass, Illinois */
	{  17,   19, 1201, 1201, 3776,   330 },	/* Champaign, Illinois */
	{  17,   21, 1202, 1202, 3801,   368 },	/* Christian, Illinois */
	{  17,   23, 1201, 1201, 3776,   382 },	/* Clark, Illinois */
	{  17,   25, 1201, 1201, 3776,   385 },	/* Clay, Illinois */
	{  17,   27, 1202, 1202, 3801,   395 },	/* Clinton, Illinois */
	{  17,   29, 1201, 1201, 3776,   412 },	/* Coles, Illinois */
	{  17,   31, 1201, 1201, 3776,   436 },	/* Cook, Illinois */
	{  17,   33, 1201, 1201, 3776,   460 },	/* Crawford, Illinois */
	{  17,   35, 1201, 1201, 3776,   475 },	/* Cumberland, Illinois */
	{  17,   37, 1201, 1201, 3776,   503 },	/* De Kalb, Illinois */
	{  17,   39, 1201, 1201, 3776,   505 },	/* De Witt, Illinois */
	{  17,   41, 1201, 1201, 3776,   546 },	/* Douglas, Illinois */
	{  17,   43, 1201, 1201, 3776,   548 },	/* Du Page, Illinois */
	{  17,   45, 1201, 1201, 3776,   574 },	/* Edgar, Illinois */
	{  17,   47, 1201, 1201, 3776,   579 },	/* Edwards, Illinois */
	{  17,   49, 1201, 1201, 3776,   580 },	/* Effingham, Illinois */
	{  17,   51, 1201, 1201, 3776,   622 },	/* Fayette, Illinois */
	{  17,   53, 1201, 1201, 3776,   639 },	/* Ford, Illinois */
	{  17,   55, 1201, 1201, 3776,   646 },	/* Franklin, Illinois */
	{  17,   57, 1202, 1202, 3801,   656 },	/* Fulton, Illinois */
	{  17,   59, 1201, 1201, 3776,   662 },	/* Gallatin, Illinois */
	{  17,   61, 1202, 1202, 3801,   732 },	/* Greene, Illinois */
	{  17,   63, 1201, 1201, 3776,   744 },	/* Grundy, Illinois */
	{  17,   65, 1201, 1201, 3776,   765 },	/* Hamilton, Illinois */
	{  17,   67, 1202, 1202, 3801,   771 },	/* Hancock, Illinois */
	{  17,   69, 1201, 1201, 3776,   779 },	/* Hardin, Illinois */
	{  17,   71, 1202, 1202, 3801,   806 },	/* Henderson, Illinois */
	{  17,   73, 1202, 1202, 3801,   811 },	/* Henry, Illinois */
	{  17,   75, 1201, 1201, 3776,   880 },	/* Iroquois, Illinois */
	{  17,   77, 1202, 1202, 3801,   893 },	/* Jackson, Illinois */
	{  17,   79, 1201, 1201, 3776,   897 },	/* Jasper, Illinois */
	{  17,   81, 1201, 1201, 3776,   901 },	/* Jefferson, Illinois */
	{  17,   83, 1202, 1202, 3801,   908 },	/* Jersey, Illinois */
	{  17,   85, 1202, 1202, 3801,   913 },	/* Jo Daviess, Illinois */
	{  17,   87, 1201, 1201, 3776,   914 },	/* Johnson, Illinois */
	{  17,   89, 1201, 1201, 3776,   931 },	/* Kane, Illinois */
	{  17,   91, 1201, 1201, 3776,   932 },	/* Kankakee, Illinois */
	{  17,   93, 1201, 1201, 3776,   944 },	/* Kendall, Illinois */
	{  17,   95, 1202, 1202, 3801,   982 },	/* Knox, Illinois */
	{  17,   97, 1201, 1201, 3776,  1006 },	/* Lake, Illinois */
	{  17,   99, 1201, 1201, 3776,   996 },	/* La Salle, Illinois */
	{  17,  101, 1201, 1201, 3776,  1033 },	/* Lawrence, Illinois */
	{  17,  103, 1202, 1202, 3801,  1040 },	/* Lee, Illinois */
	{  17,  105, 1201, 1201, 3776,  1066 },	/* Livingston, Illinois */
	{  17,  107, 1202, 1202, 3801,  1068 },	/* Logan, Illinois */
	{  17,  109, 1202, 1202, 3801,  1154 },	/* Mc Donough, Illinois */
	{  17,  111, 1201, 1201, 3776,  1157 },	/* Mc Henry, Illinois */
	{  17,  113, 1201, 1201, 3776,  1161 },	/* Mc Lean, Illinois */
	{  17,  115, 1201, 1201, 3776,  1098 },	/* Macon, Illinois */
	{  17,  117, 1202, 1202, 3801,  1099 },	/* Macoupin, Illinois */
	{  17,  119, 1202, 1202, 3801,  1101 },	/* Madison, Illinois */
	{  17,  121, 1201, 1201, 3776,  1125 },	/* Marion, Illinois */
	{  17,  123, 1202, 1202, 3801,  1129 },	/* Marshall, Illinois */
	{  17,  125, 1202, 1202, 3801,  1134 },	/* Mason, Illinois */
	{  17,  127, 1201, 1201, 3776,  1135 },	/* Massac, Illinois */
	{  17,  129, 1202, 1202, 3801,  1181 },	/* Menard, Illinois */
	{  17,  131, 1202, 1202, 3801,  1186 },	/* Mercer, Illinois */
	{  17,  133, 1202, 1202, 3801,  1226 },	/* Monroe, Illinois */
	{  17,  135, 1202, 1202, 3801,  1232 },	/* Montgomery, Illinois */
	{  17,  137, 1202, 1202, 3801,  1240 },	/* Morgan, Illinois */
	{  17,  139, 1201, 1201, 3776,  1248 },	/* Moultrie, Illinois */
	{  17,  141, 1202, 1202, 3801,  1343 },	/* Ogle, Illinois */
	{  17,  143, 1202, 1202, 3801,  1421 },	/* Peoria, Illinois */
	{  17,  145, 1202, 1202, 3801,  1425 },	/* Perry, Illinois */
	{  17,  147, 1201, 1201, 3776,  1434 },	/* Piatt, Illinois */
	{  17,  149, 1202, 1202, 3801,  1439 },	/* Pike, Illinois */
	{  17,  151, 1201, 1201, 3776,  1465 },	/* Pope, Illinois */
	{  17,  153, 1202, 1202, 3801,  1497 },	/* Pulaski, Illinois */
	{  17,  155, 1202, 1202, 3801,  1499 },	/* Putnam, Illinois */
	{  17,  157, 1202, 1202, 3801,  1513 },	/* Randolph, Illinois */
	{  17,  159, 1201, 1201, 3776,  1539 },	/* Richland, Illinois */
	{  17,  161, 1202, 1202, 3801,  1558 },	/* Rock Island, Illinois */
	{  17,  163, 1202, 1202, 3801,  1709 },	/* St Clair, Illinois */
	{  17,  165, 1201, 1201, 3776,  1598 },	/* Saline, Illinois */
	{  17,  167, 1202, 1202, 3801,  1622 },	/* Sangamon, Illinois */
	{  17,  169, 1202, 1202, 3801,  1643 },	/* Schuyler, Illinois */
	{  17,  171, 1202, 1202, 3801,  1647 },	/* Scott, Illinois */
	{  17,  173, 1201, 1201, 3776,  1668 },	/* Shelby, Illinois */
	{  17,  175, 1202, 1202, 3801,  1735 },	/* Stark, Illinois */
	{  17,  177, 1202, 1202, 3801,  1742 },	/* Stephenson, Illinois */
	{  17,  179, 1202, 1202, 3801,  1792 },	/* Tazewell, Illinois */
	{  17,  181, 1202, 1202, 3801,  1867 },	/* Union, Illinois */
	{  17,  183, 1201, 1201, 3776,  1888 },	/* Vermilion, Illinois */
	{  17,  185, 1201, 1201, 3776,  1902 },	/* Wabash, Illinois */
	{  17,  187, 1202, 1202, 3801,  1925 },	/* Warren, Illinois */
	{  17,  189, 1202, 1202, 3801,  1933 },	/* Washington, Illinois */
	{  17,  191, 1201, 1201, 3776,  1942 },	/* Wayne, Illinois */
	{  17,  193, 1201, 1201, 3776,  1964 },	/* White, Illinois */
	{  17,  195, 1202, 1202, 3801,  1966 },	/* Whiteside, Illinois */
	{  17,  197, 1201, 1201, 3776,  1978 },	/* Will, Illinois */
	{  17,  199, 1201, 1201, 3776,  1983 },	/* Williamson, Illinois */
	{  17,  201, 1202, 1202, 3801,  1990 },	/* Winnebago, Illinois */
	{  17,  203, 1202, 1202, 3801,  2000 },	/* Woodford, Illinois */
	{  18,    0,    0,    0,    0,   871 },	/* Indiana, Indiana */
	{  18,    1, 1301, 1301, 3826,     5 },	/* Adams, Indiana */
	{  18,    3, 1301, 1301, 3826,    40 },	/* Allen, Indiana */
	{  18,    5, 1301, 1301, 3826,   131 },	/* Bartholomew, Indiana */
	{  18,    7, 1302, 1302, 3851,   163 },	/* Benton, Indiana */
	{  18,    9, 1301, 1301, 3826,   183 },	/* Blackford, Indiana */
	{  18,   11, 1302, 1302, 3851,   201 },	/* Boone, Indiana */
	{  18,   13, 1301, 1301, 3826,   240 },	/* Brown, Indiana */
	{  18,   15, 1302, 1302, 3851,   302 },	/* Carroll, Indiana */
	{  18,   17, 1301, 1301, 3826,   310 },	/* Cass, Indiana */
	{  18,   19, 1301, 1301, 3826,   382 },	/* Clark, Indiana */
	{  18,   21, 1302, 1302, 3851,   385 },	/* Clay, Indiana */
	{  18,   23, 1302, 1302, 3851,   395 },	/* Clinton, Indiana */
	{  18,   25, 1302, 1302, 3851,   460 },	/* Crawford, Indiana */
	{  18,   27, 1302, 1302, 3851,   496 },	/* Daviess, Indiana */
	{  18,   29, 1301, 1301, 3826,   507 },	/* Dearborn, Indiana */
	{  18,   31, 1301, 1301, 3826,   508 },	/* Decatur, Indiana */
	{  18,   33, 1301, 1301, 3826,   503 },	/* De Kalb, Indiana */
	{  18,   35, 1301, 1301, 3826,   512 },	/* Delaware, Indiana */
	{  18,   37, 1302, 1302, 3851,   549 },	/* Dubois, Indiana */
	{  18,   39, 1301, 1301, 3826,   585 },	/* Elkhart, Indiana */
	{  18,   41, 1301, 1301, 3826,   622 },	/* Fayette, Indiana */
	{  18,   43, 1301, 1301, 3826,   635 },	/* Floyd, Indiana */
	{  18,   45, 1302, 1302, 3851,   645 },	/* Fountain, Indiana */
	{  18,   47, 1301, 1301, 3826,   646 },	/* Franklin, Indiana */
	{  18,   49, 1301, 1301, 3826,   656 },	/* Fulton, Indiana */
	{  18,   51, 1302, 1302, 3851,   685 },	/* Gibson, Indiana */
	{  18,   53, 1301, 1301, 3826,   721 },	/* Grant, Indiana */
	{  18,   55, 1302, 1302, 3851,   732 },	/* Greene, Indiana */
	{  18,   57, 1301, 1301, 3826,   765 },	/* Hamilton, Indiana */
	{  18,   59, 1301, 1301, 3826,   771 },	/* Hancock, Indiana */
	{  18,   61, 1301, 1301, 3826,   789 },	/* Harrison, Indiana */
	{  18,   63, 1302, 1302, 3851,   807 },	/* Hendricks, Indiana */
	{  18,   65, 1301, 1301, 3826,   811 },	/* Henry, Indiana */
	{  18,   67, 1301, 1301, 3826,   845 },	/* Howard, Indiana */
	{  18,   69, 1301, 1301, 3826,   859 },	/* Huntington, Indiana */
	{  18,   71, 1301, 1301, 3826,   893 },	/* Jackson, Indiana */
	{  18,   73, 1302, 1302, 3851,   897 },	/* Jasper, Indiana */
	{  18,   75, 1301, 1301, 3826,   898 },	/* Jay, Indiana */
	{  18,   77, 1301, 1301, 3826,   901 },	/* Jefferson, Indiana */
	{  18,   79, 1301, 1301, 3826,   905 },	/* Jennings, Indiana */
	{  18,   81, 1301, 1301, 3826,   914 },	/* Johnson, Indiana */
	{  18,   83, 1302, 1302, 3851,   982 },	/* Knox, Indiana */
	{  18,   85, 1301, 1301, 3826,   987 },	/* Kosciusko, Indiana */
	{  18,   87, 1301, 1301, 3826,  1004 },	/* Lagrange, Indiana */
	{  18,   89, 1302, 1302, 3851,  1006 },	/* Lake, Indiana */
	{  18,   91, 1302, 1302, 3851,   995 },	/* La Porte, Indiana */
	{  18,   93, 1302, 1302, 3851,  1033 },	/* Lawrence, Indiana */
	{  18,   95, 1301, 1301, 3826,  1101 },	/* Madison, Indiana */
	{  18,   97, 1301, 1301, 3826,  1125 },	/* Marion, Indiana */
	{  18,   99, 1301, 1301, 3826,  1129 },	/* Marshall, Indiana */
	{  18,  101, 1302, 1302, 3851,  1131 },	/* Martin, Indiana */
	{  18,  103, 1301, 1301, 3826,  1192 },	/* Miami, Indiana */
	{  18,  105, 1302, 1302, 3851,  1226 },	/* Monroe, Indiana */
	{  18,  107, 1302, 1302, 3851,  1232 },	/* Montgomery, Indiana */
	{  18,  109, 1302, 1302, 3851,  1240 },	/* Morgan, Indiana */
	{  18,  111, 1302, 1302, 3851,  1297 },	/* Newton, Indiana */
	{  18,  113, 1301, 1301, 3826,  1310 },	/* Noble, Indiana */
	{  18,  115, 1301, 1301, 3826,  1345 },	/* Ohio, Indiana */
	{  18,  117, 1302, 1302, 3851,  1361 },	/* Orange, Indiana */
	{  18,  119, 1302, 1302, 3851,  1380 },	/* Owen, Indiana */
	{  18,  121, 1302, 1302, 3851,  1396 },	/* Parke, Indiana */
	{  18,  123, 1302, 1302, 3851,  1425 },	/* Perry, Indiana */
	{  18,  125, 1302, 1302, 3851,  1439 },	/* Pike, Indiana */
	{  18,  127, 1302, 1302, 3851,  1468 },	/* Porter, Indiana */
	{  18,  129, 1302, 1302, 3851,  1470 },	/* Posey, Indiana */
	{  18,  131, 1302, 1302, 3851,  1497 },	/* Pulaski, Indiana */
	{  18,  133, 1302, 1302, 3851,  1499 },	/* Putnam, Indiana */
	{  18,  135, 1301, 1301, 3826,  1513 },	/* Randolph, Indiana */
	{  18,  137, 1301, 1301, 3826,  1548 },	/* Ripley, Indiana */
	{  18,  139, 1301, 1301, 3826,  1581 },	/* Rush, Indiana */
	{  18,  141, 1301, 1301, 3826,  1719 },	/* St Joseph, Indiana */
	{  18,  143, 1301, 1301, 3826,  1647 },	/* Scott, Indiana */
	{  18,  145, 1301, 1301, 3826,  1668 },	/* Shelby, Indiana */
	{  18,  147, 1302, 1302, 3851,  1703 },	/* Spencer, Indiana */
	{  18,  149, 1302, 1302, 3851,  1736 },	/* Starke, Indiana */
	{  18,  151, 1301, 1301, 3826,  1744 },	/* Steuben, Indiana */
	{  18,  153, 1302, 1302, 3851,  1759 },	/* Sullivan, Indiana */
	{  18,  155, 1301, 1301, 3826,  1778 },	/* Switzerland, Indiana */
	{  18,  157, 1302, 1302, 3851,  1813 },	/* Tippecanoe, Indiana */
	{  18,  159, 1301, 1301, 3826,  1814 },	/* Tipton, Indiana */
	{  18,  161, 1301, 1301, 3826,  1867 },	/* Union, Indiana */
	{  18,  163, 1302, 1302, 3851,  1883 },	/* Vanderburgh, Indiana */
	{  18,  165, 1302, 1302, 3851,  1889 },	/* Vermillion, Indiana */
	{  18,  167, 1302, 1302, 3851,  1894 },	/* Vigo, Indiana */
	{  18,  169, 1301, 1301, 3826,  1902 },	/* Wabash, Indiana */
	{  18,  171, 1302, 1302, 3851,  1925 },	/* Warren, Indiana */
	{  18,  173, 1302, 1302, 3851,  1926 },	/* Warrick, Indiana */
	{  18,  175, 1301, 1301, 3826,  1933 },	/* Washington, Indiana */
	{  18,  177, 1301, 1301, 3826,  1942 },	/* Wayne, Indiana */
	{  18,  179, 1301, 1301, 3826,  1949 },	/* Wells, Indiana */
	{  18,  181, 1302, 1302, 3851,  1964 },	/* White, Indiana */
	{  18,  183, 1301, 1301, 3826,  1968 },	/* Whitley, Indiana */
	{  19,    0, 1402, 1402, 3901,   876 },	/* Iowa, Iowa */
	{  19,    1, 1402, 1402, 3901,     4 },	/* Adair, Iowa */
	{  19,    3, 1402, 1402, 3901,     5 },	/* Adams, Iowa */
	{  19,    5, 1401, 1401, 3876,    35 },	/* Allamakee, Iowa */
	{  19,    7, 1402, 1402, 3901,    63 },	/* Appanoose, Iowa */
	{  19,    9, 1402, 1402, 3901,    95 },	/* Audubon, Iowa */
	{  19,   11, 1401, 1401, 3876,   163 },	/* Benton, Iowa */
	{  19,   13, 1401, 1401, 3876,   182 },	/* Black Hawk, Iowa */
	{  19,   15, 1401, 1401, 3876,   201 },	/* Boone, Iowa */
	{  19,   17, 1401, 1401, 3876,   226 },	/* Bremer, Iowa */
	{  19,   19, 1401, 1401, 3876,   244 },	/* Buchanan, Iowa */
	{  19,   21, 1401, 1401, 3876,   247 },	/* Buena Vista, Iowa */
	{  19,   23, 1401, 1401, 3876,   262 },	/* Butler, Iowa */
	{  19,   25, 1401, 1401, 3876,   275 },	/* Calhoun, Iowa */
	{  19,   27, 1401, 1401, 3876,   302 },	/* Carroll, Iowa */
	{  19,   29, 1402, 1402, 3901,   310 },	/* Cass, Iowa */
	{  19,   31, 1402, 1402, 3901,   324 },	/* Cedar, Iowa */
	{  19,   33, 1401, 1401, 3876,   327 },	/* Cerro Gordo, Iowa */
	{  19,   35, 1401, 1401, 3876,   351 },	/* Cherokee, Iowa */
	{  19,   37, 1401, 1401, 3876,   358 },	/* Chickasaw, Iowa */
	{  19,   39, 1402, 1402, 3901,   383 },	/* Clarke, Iowa */
	{  19,   41, 1401, 1401, 3876,   385 },	/* Clay, Iowa */
	{  19,   43, 1401, 1401, 3876,   386 },	/* Clayton, Iowa */
	{  19,   45, 1402, 1402, 3901,   395 },	/* Clinton, Iowa */
	{  19,   47, 1401, 1401, 3876,   460 },	/* Crawford, Iowa */
	{  19,   49, 1402, 1402, 3901,   486 },	/* Dallas, Iowa */
	{  19,   51, 1402, 1402, 3901,   497 },	/* Davis, Iowa */
	{  19,   53, 1402, 1402, 3901,   508 },	/* Decatur, Iowa */
	{  19,   55, 1401, 1401, 3876,   512 },	/* Delaware, Iowa */
	{  19,   57, 1402, 1402, 3901,   517 },	/* Des Moines, Iowa */
	{  19,   59, 1401, 1401, 3876,   525 },	/* Dickinson, Iowa */
	{  19,   61, 1401, 1401, 3876,   550 },	/* Dubuque, Iowa */
	{  19,   63, 1401, 1401, 3876,   593 },	/* Emmet, Iowa */
	{  19,   65, 1401, 1401, 3876,   622 },	/* Fayette, Iowa */
	{  19,   67, 1401, 1401, 3876,   635 },	/* Floyd, Iowa */
	{  19,   69, 1401, 1401, 3876,   646 },	/* Franklin, Iowa */
	{  19,   71, 1402, 1402, 3901,   652 },	/* Fremont, Iowa */
	{  19,   73, 1401, 1401, 3876,   732 },	/* Greene, Iowa */
	{  19,   75, 1401, 1401, 3876,   744 },	/* Grundy, Iowa */
	{  19,   77, 1402, 1402, 3901,   756 },	/* Guthrie, Iowa */
	{  19,   79, 1401, 1401, 3876,   765 },	/* Hamilton, Iowa */
	{  19,   81, 1401, 1401, 3876,   771 },	/* Hancock, Iowa */
	{  19,   83, 1401, 1401, 3876,   779 },	/* Hardin, Iowa */
	{  19,   85, 1402, 1402, 3901,   789 },	/* Harrison, Iowa */
	{  19,   87, 1402, 1402, 3901,   811 },	/* Henry, Iowa */
	{  19,   89, 1401, 1401, 3876,   845 },	/* Howard, Iowa */
	{  19,   91, 1401, 1401, 3876,   854 },	/* Humboldt, Iowa */
	{  19,   93, 1401, 1401, 3876,   865 },	/* Ida, Iowa */
	{  19,   95,    0,    0,    0,   876 },	/* Iowa, Iowa */
	{  19,   97, 1401, 1401, 3876,   893 },	/* Jackson, Iowa */
	{  19,   99, 1402, 1402, 3901,   897 },	/* Jasper, Iowa */
	{  19,  101, 1402, 1402, 3901,   901 },	/* Jefferson, Iowa */
	{  19,  103, 1402, 1402, 3901,   914 },	/* Johnson, Iowa */
	{  19,  105, 1401, 1401, 3876,   917 },	/* Jones, Iowa */
	{  19,  107, 1402, 1402, 3901,   951 },	/* Keokuk, Iowa */
	{  19,  109, 1401, 1401, 3876,   989 },	/* Kossuth, Iowa */
	{  19,  111, 1402, 1402, 3901,  1040 },	/* Lee, Iowa */
	{  19,  113, 1401, 1401, 3876,  1061 },	/* Linn, Iowa */
	{  19,  115, 1402, 1402, 3901,  1077 },	/* Louisa, Iowa */
	{  19,  117, 1402, 1402, 3901,  1084 },	/* Lucas, Iowa */
	{  19,  119, 1401, 1401, 3876,  1095 },	/* Lyon, Iowa */
	{  19,  121, 1402, 1402, 3901,  1101 },	/* Madison, Iowa */
	{  19,  123, 1402, 1402, 3901,  1103 },	/* Mahaska, Iowa */
	{  19,  125, 1402, 1402, 3901,  1125 },	/* Marion, Iowa */
	{  19,  127, 1401, 1401, 3876,  1129 },	/* Marshall, Iowa */
	{  19,  129, 1402, 1402, 3901,  1203 },	/* Mills, Iowa */
	{  19,  131, 1401, 1401, 3876,  1215 },	/* Mitchell, Iowa */
	{  19,  133, 1401, 1401, 3876,  1224 },	/* Monona, Iowa */
	{  19,  135, 1402, 1402, 3901,  1226 },	/* Monroe, Iowa */
	{  19,  137, 1402, 1402, 3901,  1232 },	/* Montgomery, Iowa */
	{  19,  139, 1402, 1402, 3901,  1254 },	/* Muscatine, Iowa */
	{  19,  141, 1401, 1401, 3876,  1334 },	/* O'Brien, Iowa */
	{  19,  143, 1401, 1401, 3876,  1368 },	/* Osceola, Iowa */
	{  19,  145, 1402, 1402, 3901,  1387 },	/* Page, Iowa */
	{  19,  147, 1401, 1401, 3876,  1391 },	/* Palo Alto, Iowa */
	{  19,  149, 1401, 1401, 3876,  1456 },	/* Plymouth, Iowa */
	{  19,  151, 1401, 1401, 3876,  1457 },	/* Pocahontas, Iowa */
	{  19,  153, 1402, 1402, 3901,  1461 },	/* Polk, Iowa */
	{  19,  155, 1402, 1402, 3901,  1472 },	/* Pottawattamie, Iowa */
	{  19,  157, 1402, 1402, 3901,  1477 },	/* Poweshiek, Iowa */
	{  19,  159, 1402, 1402, 3901,  1544 },	/* Ringgold, Iowa */
	{  19,  161, 1401, 1401, 3876,  1589 },	/* Sac, Iowa */
	{  19,  163, 1402, 1402, 3901,  1647 },	/* Scott, Iowa */
	{  19,  165, 1402, 1402, 3901,  1668 },	/* Shelby, Iowa */
	{  19,  167, 1401, 1401, 3876,  1679 },	/* Sioux, Iowa */
	{  19,  169, 1401, 1401, 3876,  1753 },	/* Story, Iowa */
	{  19,  171, 1401, 1401, 3876,  1784 },	/* Tama, Iowa */
	{  19,  173, 1402, 1402, 3901,  1791 },	/* Taylor, Iowa */
	{  19,  175, 1402, 1402, 3901,  1867 },	/* Union, Iowa */
	{  19,  177, 1402, 1402, 3901,  1879 },	/* Van Buren, Iowa */
	{  19,  179, 1402, 1402, 3901,  1922 },	/* Wapello, Iowa */
	{  19,  181, 1402, 1402, 3901,  1925 },	/* Warren, Iowa */
	{  19,  183, 1402, 1402, 3901,  1933 },	/* Washington, Iowa */
	{  19,  185, 1402, 1402, 3901,  1942 },	/* Wayne, Iowa */
	{  19,  187, 1401, 1401, 3876,  1947 },	/* Webster, Iowa */
	{  19,  189, 1401, 1401, 3876,  1990 },	/* Winnebago, Iowa */
	{  19,  191, 1401, 1401, 3876,  1991 },	/* Winneshiek, Iowa */
	{  19,  193, 1401, 1401, 3876,  1999 },	/* Woodbury, Iowa */
	{  19,  195, 1401, 1401, 3876,  2006 },	/* Worth, Iowa */
	{  19,  197, 1401, 1401, 3876,  2010 },	/* Wright, Iowa */
	{  20,    0,    0,    0,    0,   933 },	/* Kansas, Kansas */
	{  20,    1, 1502, 1502, 3951,    40 },	/* Allen, Kansas */
	{  20,    3, 1502, 1502, 3951,    51 },	/* Anderson, Kansas */
	{  20,    5, 1501, 1501, 3926,    88 },	/* Atchison, Kansas */
	{  20,    7, 1502, 1502, 3951,   120 },	/* Barber, Kansas */
	{  20,    9, 1502, 1502, 3951,   132 },	/* Barton, Kansas */
	{  20,   11, 1502, 1502, 3951,   209 },	/* Bourbon, Kansas */
	{  20,   13, 1501, 1501, 3926,   240 },	/* Brown, Kansas */
	{  20,   15, 1502, 1502, 3951,   262 },	/* Butler, Kansas */
	{  20,   17, 1502, 1502, 3951,   340 },	/* Chase, Kansas */
	{  20,   19, 1502, 1502, 3951,   344 },	/* Chautauqua, Kansas */
	{  20,   21, 1502, 1502, 3951,   351 },	/* Cherokee, Kansas */
	{  20,   23, 1501, 1501, 3926,   357 },	/* Cheyenne, Kansas */
	{  20,   25, 1502, 1502, 3951,   382 },	/* Clark, Kansas */
	{  20,   27, 1501, 1501, 3926,   385 },	/* Clay, Kansas */
	{  20,   29, 1501, 1501, 3926,   396 },	/* Cloud, Kansas */
	{  20,   31, 1502, 1502, 3951,   407 },	/* Coffey, Kansas */
	{  20,   33, 1502, 1502, 3951,   426 },	/* Comanche, Kansas */
	{  20,   35, 1502, 1502, 3951,   454 },	/* Cowley, Kansas */
	{  20,   37, 1502, 1502, 3951,   460 },	/* Crawford, Kansas */
	{  20,   39, 1501, 1501, 3926,   508 },	/* Decatur, Kansas */
	{  20,   41, 1501, 1501, 3926,   525 },	/* Dickinson, Kansas */
	{  20,   43, 1501, 1501, 3926,   539 },	/* Doniphan, Kansas */
	{  20,   45, 1501, 1501, 3926,   546 },	/* Douglas, Kansas */
	{  20,   47, 1502, 1502, 3951,   579 },	/* Edwards, Kansas */
	{  20,   49, 1502, 1502, 3951,   584 },	/* Elk, Kansas */
	{  20,   51, 1501, 1501, 3926,   588 },	/* Ellis, Kansas */
	{  20,   53, 1501, 1501, 3926,   589 },	/* Ellsworth, Kansas */
	{  20,   55, 1502, 1502, 3951,   628 },	/* Finney, Kansas */
	{  20,   57, 1502, 1502, 3951,   639 },	/* Ford, Kansas */
	{  20,   59, 1502, 1502, 3951,   646 },	/* Franklin, Kansas */
	{  20,   61, 1501, 1501, 3926,   676 },	/* Geary, Kansas */
	{  20,   63, 1501, 1501, 3926,   711 },	/* Gove, Kansas */
	{  20,   65, 1501, 1501, 3926,   714 },	/* Graham, Kansas */
	{  20,   67, 1502, 1502, 3951,   721 },	/* Grant, Kansas */
	{  20,   69, 1502, 1502, 3951,   725 },	/* Gray, Kansas */
	{  20,   71, 1502, 1502, 3951,   728 },	/* Greeley, Kansas */
	{  20,   73, 1502, 1502, 3951,   737 },	/* Greenwood, Kansas */
	{  20,   75, 1502, 1502, 3951,   765 },	/* Hamilton, Kansas */
	{  20,   77, 1502, 1502, 3951,   787 },	/* Harper, Kansas */
	{  20,   79, 1502, 1502, 3951,   794 },	/* Harvey, Kansas */
	{  20,   81, 1502, 1502, 3951,   795 },	/* Haskell, Kansas */
	{  20,   83, 1502, 1502, 3951,   829 },	/* Hodgeman, Kansas */
	{  20,   85, 1501, 1501, 3926,   893 },	/* Jackson, Kansas */
	{  20,   87, 1501, 1501, 3926,   901 },	/* Jefferson, Kansas */
	{  20,   89, 1501, 1501, 3926,   910 },	/* Jewell, Kansas */
	{  20,   91, 1501, 1501, 3926,   914 },	/* Johnson, Kansas */
	{  20,   93, 1502, 1502, 3951,   940 },	/* Kearny, Kansas */
	{  20,   95, 1502, 1502, 3951,   968 },	/* Kingman, Kansas */
	{  20,   97, 1502, 1502, 3951,   973 },	/* Kiowa, Kansas */
	{  20,   99, 1502, 1502, 3951,   997 },	/* Labette, Kansas */
	{  20,  101, 1502, 1502, 3951,  1015 },	/* Lane, Kansas */
	{  20,  103, 1501, 1501, 3926,  1038 },	/* Leavenworth, Kansas */
	{  20,  105, 1501, 1501, 3926,  1060 },	/* Lincoln, Kansas */
	{  20,  107, 1502, 1502, 3951,  1061 },	/* Linn, Kansas */
	{  20,  109, 1501, 1501, 3926,  1068 },	/* Logan, Kansas */
	{  20,  111, 1502, 1502, 3951,  1095 },	/* Lyon, Kansas */
	{  20,  113, 1502, 1502, 3951,  1170 },	/* McPherson, Kansas */
	{  20,  115, 1502, 1502, 3951,  1125 },	/* Marion, Kansas */
	{  20,  117, 1501, 1501, 3926,  1129 },	/* Marshall, Kansas */
	{  20,  119, 1502, 1502, 3951,  1171 },	/* Meade, Kansas */
	{  20,  121, 1502, 1502, 3951,  1192 },	/* Miami, Kansas */
	{  20,  123, 1501, 1501, 3926,  1215 },	/* Mitchell, Kansas */
	{  20,  125, 1502, 1502, 3951,  1232 },	/* Montgomery, Kansas */
	{  20,  127, 1501, 1501, 3926,  1243 },	/* Morris, Kansas */
	{  20,  129, 1502, 1502, 3951,  1246 },	/* Morton, Kansas */
	{  20,  131, 1501, 1501, 3926,  1278 },	/* Nemaha, Kansas */
	{  20,  133, 1502, 1502, 3951,  1279 },	/* Neosho, Kansas */
	{  20,  135, 1502, 1502, 3951,  1281 },	/* Ness, Kansas */
	{  20,  137, 1501, 1501, 3926,  1326 },	/* Norton, Kansas */
	{  20,  139, 1502, 1502, 3951,  1366 },	/* Osage, Kansas */
	{  20,  141, 1501, 1501, 3926,  1367 },	/* Osborne, Kansas */
	{  20,  143, 1501, 1501, 3926,  1374 },	/* Ottawa, Kansas */
	{  20,  145, 1502, 1502, 3951,  1405 },	/* Pawnee, Kansas */
	{  20,  147, 1501, 1501, 3926,  1433 },	/* Phillips, Kansas */
	{  20,  149, 1501, 1501, 3926,  1471 },	/* Pottawatomie, Kansas */
	{  20,  151, 1502, 1502, 3951,  1480 },	/* Pratt, Kansas */
	{  20,  153, 1501, 1501, 3926,  1519 },	/* Rawlins, Kansas */
	{  20,  155, 1502, 1502, 3951,  1529 },	/* Reno, Kansas */
	{  20,  157, 1501, 1501, 3926,  1532 },	/* Republic, Kansas */
	{  20,  159, 1502, 1502, 3951,  1536 },	/* Rice, Kansas */
	{  20,  161, 1501, 1501, 3926,  1542 },	/* Riley, Kansas */
	{  20,  163, 1501, 1501, 3926,  1570 },	/* Rooks, Kansas */
	{  20,  165, 1502, 1502, 3951,  1581 },	/* Rush, Kansas */
	{  20,  167, 1501, 1501, 3926,  1583 },	/* Russell, Kansas */
	{  20,  169, 1501, 1501, 3926,  1598 },	/* Saline, Kansas */
	{  20,  171, 1502, 1502, 3951,  1647 },	/* Scott, Kansas */
	{  20,  173, 1502, 1502, 3951,  1653 },	/* Sedgwick, Kansas */
	{  20,  175, 1502, 1502, 3951,  1659 },	/* Seward, Kansas */
	{  20,  177, 1501, 1501, 3926,  1666 },	/* Shawnee, Kansas */
	{  20,  179, 1501, 1501, 3926,  1671 },	/* Sheridan, Kansas */
	{  20,  181, 1501, 1501, 3926,  1672 },	/* Sherman, Kansas */
	{  20,  183, 1501, 1501, 3926,  1686 },	/* Smith, Kansas */
	{  20,  185, 1502, 1502, 3951,  1730 },	/* Stafford, Kansas */
	{  20,  187, 1502, 1502, 3951,  1734 },	/* Stanton, Kansas */
	{  20,  189, 1502, 1502, 3951,  1745 },	/* Stevens, Kansas */
	{  20,  191, 1502, 1502, 3951,  1763 },	/* Sumner, Kansas */
	{  20,  193, 1501, 1501, 3926,  1804 },	/* Thomas, Kansas */
	{  20,  195, 1501, 1501, 3926,  1835 },	/* Trego, Kansas */
	{  20,  197, 1501, 1501, 3926,  1904 },	/* Wabaunsee, Kansas */
	{  20,  199, 1501, 1501, 3926,  1915 },	/* Wallace, Kansas */
	{  20,  201, 1501, 1501, 3926,  1933 },	/* Washington, Kansas */
	{  20,  203, 1502, 1502, 3951,  1971 },	/* Wichita, Kansas */
	{  20,  205, 1502, 1502, 3951,  1984 },	/* Wilson, Kansas */
	{  20,  207, 1502, 1502, 3951,  2003 },	/* Woodson, Kansas */
	{  20,  209, 1501, 1501, 3926,  2012 },	/* Wyandotte, Kansas */
	{  21,    0,    0,    0,    0,   950 },	/* Kentucky, Kentucky */
	{  21,    1, 1602, 1602, 4001,     4 },	/* Adair, Kentucky */
	{  21,    3, 1602, 1602, 4001,    40 },	/* Allen, Kentucky */
	{  21,    5, 1601, 1601, 3976,    51 },	/* Anderson, Kentucky */
	{  21,    7, 1602, 1602, 4001,   110 },	/* Ballard, Kentucky */
	{  21,    9, 1602, 1602, 4001,   127 },	/* Barren, Kentucky */
	{  21,   11, 1601, 1601, 3976,   136 },	/* Bath, Kentucky */
	{  21,   13, 1602, 1602, 4001,   154 },	/* Bell, Kentucky */
	{  21,   15, 1601, 1601, 3976,   201 },	/* Boone, Kentucky */
	{  21,   17, 1601, 1601, 3976,   209 },	/* Bourbon, Kentucky */
	{  21,   19, 1601, 1601, 3976,   214 },	/* Boyd, Kentucky */
	{  21,   21, 1602, 1602, 4001,   215 },	/* Boyle, Kentucky */
	{  21,   23, 1601, 1601, 3976,   216 },	/* Bracken, Kentucky */
	{  21,   25, 1602, 1602, 4001,   224 },	/* Breathitt, Kentucky */
	{  21,   27, 1602, 1602, 4001,   225 },	/* Breckinridge, Kentucky */
	{  21,   29, 1601, 1601, 3976,   250 },	/* Bullitt, Kentucky */
	{  21,   31, 1602, 1602, 4001,   262 },	/* Butler, Kentucky */
	{  21,   33, 1602, 1602, 4001,   273 },	/* Caldwell, Kentucky */
	{  21,   35, 1602, 1602, 4001,   279 },	/* Calloway, Kentucky */
	{  21,   37, 1601, 1601, 3976,   287 },	/* Campbell, Kentucky */
	{  21,   39, 1602, 1602, 4001,   298 },	/* Carlisle, Kentucky */
	{  21,   41, 1601, 1601, 3976,   302 },	/* Carroll, Kentucky */
	{  21,   43, 1601, 1601, 3976,   305 },	/* Carter, Kentucky */
	{  21,   45, 1602, 1602, 4001,   309 },	/* Casey, Kentucky */
	{  21,   47, 1602, 1602, 4001,   368 },	/* Christian, Kentucky */
	{  21,   49, 1601, 1601, 3976,   382 },	/* Clark, Kentucky */
	{  21,   51, 1602, 1602, 4001,   385 },	/* Clay, Kentucky */
	{  21,   53, 1602, 1602, 4001,   395 },	/* Clinton, Kentucky */
	{  21,   55, 1602, 1602, 4001,   464 },	/* Crittenden, Kentucky */
	{  21,   57, 1602, 1602, 4001,   475 },	/* Cumberland, Kentucky */
	{  21,   59, 1602, 1602, 4001,   496 },	/* Daviess, Kentucky */
	{  21,   61, 1602, 1602, 4001,   577 },	/* Edmonson, Kentucky */
	{  21,   63, 1601, 1601, 3976,   587 },	/* Elliott, Kentucky */
	{  21,   65, 1602, 1602, 4001,   603 },	/* Estill, Kentucky */
	{  21,   67, 1601, 1601, 3976,   622 },	/* Fayette, Kentucky */
	{  21,   69, 1601, 1601, 3976,   632 },	/* Fleming, Kentucky */
	{  21,   71, 1602, 1602, 4001,   635 },	/* Floyd, Kentucky */
	{  21,   73, 1601, 1601, 3976,   646 },	/* Franklin, Kentucky */
	{  21,   75, 1602, 1602, 4001,   656 },	/* Fulton, Kentucky */
	{  21,   77, 1601, 1601, 3976,   662 },	/* Gallatin, Kentucky */
	{  21,   79, 1602, 1602, 4001,   668 },	/* Garrard, Kentucky */
	{  21,   81, 1601, 1601, 3976,   721 },	/* Grant, Kentucky */
	{  21,   83, 1602, 1602, 4001,   724 },	/* Graves, Kentucky */
	{  21,   85, 1602, 1602, 4001,   727 },	/* Grayson, Kentucky */
	{  21,   87, 1602, 1602, 4001,   729 },	/* Green, Kentucky */
	{  21,   89, 1601, 1601, 3976,   735 },	/* Greenup, Kentucky */
	{  21,   91, 1602, 1602, 4001,   771 },	/* Hancock, Kentucky */
	{  21,   93, 1602, 1602, 4001,   779 },	/* Hardin, Kentucky */
	{  21,   95, 1602, 1602, 4001,   783 },	/* Harlan, Kentucky */
	{  21,   97, 1601, 1601, 3976,   789 },	/* Harrison, Kentucky */
	{  21,   99, 1602, 1602, 4001,   791 },	/* Hart, Kentucky */
	{  21,  101, 1602, 1602, 4001,   806 },	/* Henderson, Kentucky */
	{  21,  103, 1601, 1601, 3976,   811 },	/* Henry, Kentucky */
	{  21,  105, 1602, 1602, 4001,   816 },	/* Hickman, Kentucky */
	{  21,  107, 1602, 1602, 4001,   838 },	/* Hopkins, Kentucky */
	{  21,  109, 1602, 1602, 4001,   893 },	/* Jackson, Kentucky */
	{  21,  111, 1601, 1601, 3976,   901 },	/* Jefferson, Kentucky */
	{  21,  113, 1601, 1601, 3976,   909 },	/* Jessamine, Kentucky */
	{  21,  115, 1602, 1602, 4001,   914 },	/* Johnson, Kentucky */
	{  21,  117, 1601, 1601, 3976,   949 },	/* Kenton, Kentucky */
	{  21,  119, 1602, 1602, 4001,   981 },	/* Knott, Kentucky */
	{  21,  121, 1602, 1602, 4001,   982 },	/* Knox, Kentucky */
	{  21,  123, 1602, 1602, 4001,  1022 },	/* Larue, Kentucky */
	{  21,  125, 1602, 1602, 4001,  1030 },	/* Laurel, Kentucky */
	{  21,  127, 1601, 1601, 3976,  1033 },	/* Lawrence, Kentucky */
	{  21,  129, 1602, 1602, 4001,  1040 },	/* Lee, Kentucky */
	{  21,  131, 1602, 1602, 4001,  1048 },	/* Leslie, Kentucky */
	{  21,  133, 1602, 1602, 4001,  1049 },	/* Letcher, Kentucky */
	{  21,  135, 1601, 1601, 3976,  1051 },	/* Lewis, Kentucky */
	{  21,  137, 1602, 1602, 4001,  1060 },	/* Lincoln, Kentucky */
	{  21,  139, 1602, 1602, 4001,  1066 },	/* Livingston, Kentucky */
	{  21,  141, 1602, 1602, 4001,  1068 },	/* Logan, Kentucky */
	{  21,  143, 1602, 1602, 4001,  1095 },	/* Lyon, Kentucky */
	{  21,  145, 1602, 1602, 4001,  1149 },	/* Mc Cracken, Kentucky */
	{  21,  147, 1602, 1602, 4001,  1150 },	/* Mc Creary, Kentucky */
	{  21,  149, 1602, 1602, 4001,  1161 },	/* Mc Lean, Kentucky */
	{  21,  151, 1602, 1602, 4001,  1101 },	/* Madison, Kentucky */
	{  21,  153, 1602, 1602, 4001,  1102 },	/* Magoffin, Kentucky */
	{  21,  155, 1602, 1602, 4001,  1125 },	/* Marion, Kentucky */
	{  21,  157, 1602, 1602, 4001,  1129 },	/* Marshall, Kentucky */
	{  21,  159, 1602, 1602, 4001,  1131 },	/* Martin, Kentucky */
	{  21,  161, 1601, 1601, 3976,  1134 },	/* Mason, Kentucky */
	{  21,  163, 1602, 1602, 4001,  1171 },	/* Meade, Kentucky */
	{  21,  165, 1601, 1601, 3976,  1183 },	/* Menifee, Kentucky */
	{  21,  167, 1602, 1602, 4001,  1186 },	/* Mercer, Kentucky */
	{  21,  169, 1602, 1602, 4001,  1191 },	/* Metcalfe, Kentucky */
	{  21,  171, 1602, 1602, 4001,  1226 },	/* Monroe, Kentucky */
	{  21,  173, 1601, 1601, 3976,  1232 },	/* Montgomery, Kentucky */
	{  21,  175, 1601, 1601, 3976,  1240 },	/* Morgan, Kentucky */
	{  21,  177, 1602, 1602, 4001,  1251 },	/* Muhlenberg, Kentucky */
	{  21,  179, 1602, 1602, 4001,  1277 },	/* Nelson, Kentucky */
	{  21,  181, 1601, 1601, 3976,  1307 },	/* Nicholas, Kentucky */
	{  21,  183, 1602, 1602, 4001,  1345 },	/* Ohio, Kentucky */
	{  21,  185, 1601, 1601, 3976,  1353 },	/* Oldham, Kentucky */
	{  21,  187, 1601, 1601, 3976,  1380 },	/* Owen, Kentucky */
	{  21,  189, 1602, 1602, 4001,  1381 },	/* Owsley, Kentucky */
	{  21,  191, 1601, 1601, 3976,  1416 },	/* Pendleton, Kentucky */
	{  21,  193, 1602, 1602, 4001,  1425 },	/* Perry, Kentucky */
	{  21,  195, 1602, 1602, 4001,  1439 },	/* Pike, Kentucky */
	{  21,  197, 1602, 1602, 4001,  1475 },	/* Powell, Kentucky */
	{  21,  199, 1602, 1602, 4001,  1497 },	/* Pulaski, Kentucky */
	{  21,  201, 1601, 1601, 3976,  1555 },	/* Robertson, Kentucky */
	{  21,  203, 1602, 1602, 4001,  1560 },	/* Rockcastle, Kentucky */
	{  21,  205, 1601, 1601, 3976,  1579 },	/* Rowan, Kentucky */
	{  21,  207, 1602, 1602, 4001,  1583 },	/* Russell, Kentucky */
	{  21,  209, 1601, 1601, 3976,  1647 },	/* Scott, Kentucky */
	{  21,  211, 1601, 1601, 3976,  1668 },	/* Shelby, Kentucky */
	{  21,  213, 1602, 1602, 4001,  1678 },	/* Simpson, Kentucky */
	{  21,  215, 1601, 1601, 3976,  1703 },	/* Spencer, Kentucky */
	{  21,  217, 1602, 1602, 4001,  1791 },	/* Taylor, Kentucky */
	{  21,  219, 1602, 1602, 4001,  1819 },	/* Todd, Kentucky */
	{  21,  221, 1602, 1602, 4001,  1838 },	/* Trigg, Kentucky */
	{  21,  223, 1601, 1601, 3976,  1839 },	/* Trimble, Kentucky */
	{  21,  225, 1602, 1602, 4001,  1867 },	/* Union, Kentucky */
	{  21,  227, 1602, 1602, 4001,  1925 },	/* Warren, Kentucky */
	{  21,  229, 1602, 1602, 4001,  1933 },	/* Washington, Kentucky */
	{  21,  231, 1602, 1602, 4001,  1942 },	/* Wayne, Kentucky */
	{  21,  233, 1602, 1602, 4001,  1947 },	/* Webster, Kentucky */
	{  21,  235, 1602, 1602, 4001,  1968 },	/* Whitley, Kentucky */
	{  21,  237, 1602, 1602, 4001,  1997 },	/* Wolfe, Kentucky */
	{  21,  239, 1601, 1601, 3976,  2000 },	/* Woodford, Kentucky */
	{  22,    0,    0,    0,    0,  1078 },	/* Louisiana, Louisiana */
	{  22,    1, 1702, 1702, 4051,     1 },	/* Acadia, Louisiana */
	{  22,    3, 1702, 1702, 4051,    40 },	/* Allen, Louisiana */
	{  22,    5, 1702, 1702, 4051,    80 },	/* Ascension, Louisiana */
	{  22,    7, 1702, 1702, 4051,    86 },	/* Assumption, Louisiana */
	{  22,    9, 1701, 1701, 4026,   103 },	/* Avoyelles, Louisiana */
	{  22,   11, 1702, 1702, 4051,   145 },	/* Beauregard, Louisiana */
	{  22,   13, 1701, 1701, 4026,   175 },	/* Bienville, Louisiana */
	{  22,   15, 1701, 1701, 4026,   204 },	/* Bossier, Louisiana */
	{  22,   17, 1701, 1701, 4026,   269 },	/* Caddo, Louisiana */
	{  22,   19, 1702, 1702, 4051,   272 },	/* Calcasieu, Louisiana */
	{  22,   21, 1701, 1701, 4026,   273 },	/* Caldwell, Louisiana */
	{  22,   23, 1702, 1702, 4051,   285 },	/* Cameron, Louisiana */
	{  22,   25, 1701, 1701, 4026,   314 },	/* Catahoula, Louisiana */
	{  22,   27, 1701, 1701, 4026,   377 },	/* Claiborne, Louisiana */
	{  22,   29, 1701, 1701, 4026,   429 },	/* Concordia, Louisiana */
	{  22,   31, 1701, 1701, 4026,   504 },	/* De Soto, Louisiana */
	{  22,   33, 1702, 1702, 4051,   563 },	/* East Baton Rouge, Louisiana */
	{  22,   35, 1701, 1701, 4026,   564 },	/* East Carroll, Louisiana */
	{  22,   37, 1702, 1702, 4051,   565 },	/* East Feliciana, Louisiana */
	{  22,   39, 1702, 1702, 4051,   606 },	/* Evangeline, Louisiana */
	{  22,   41, 1701, 1701, 4026,   646 },	/* Franklin, Louisiana */
	{  22,   43, 1701, 1701, 4026,   721 },	/* Grant, Louisiana */
	{  22,   45, 1702, 1702, 4051,   863 },	/* Iberia, Louisiana */
	{  22,   47, 1702, 1702, 4051,   864 },	/* Iberville, Louisiana */
	{  22,   49, 1701, 1701, 4026,   893 },	/* Jackson, Louisiana */
	{  22,   51, 1702, 1702, 4051,   901 },	/* Jefferson, Louisiana */
	{  22,   53, 1702, 1702, 4051,   902 },	/* Jefferson Davis, Louisiana */
	{  22,   55, 1702, 1702, 4051,  1002 },	/* Lafayette, Louisiana */
	{  22,   57, 1702, 1702, 4051,  1003 },	/* Lafourche, Louisiana */
	{  22,   59, 1701, 1701, 4026,   996 },	/* La Salle, Louisiana */
	{  22,   61, 1701, 1701, 4026,  1060 },	/* Lincoln, Louisiana */
	{  22,   63, 1702, 1702, 4051,  1066 },	/* Livingston, Louisiana */
	{  22,   65, 1701, 1701, 4026,  1101 },	/* Madison, Louisiana */
	{  22,   67, 1701, 1701, 4026,  1239 },	/* Morehouse, Louisiana */
	{  22,   69, 1701, 1701, 4026,  1271 },	/* Natchitoches, Louisiana */
	{  22,   71, 1702, 1702, 4051,  1364 },	/* Orleans, Louisiana */
	{  22,   73, 1701, 1701, 4026,  1376 },	/* Ouachita, Louisiana */
	{  22,   75, 1702, 1702, 4051,  1452 },	/* Plaquemines, Louisiana */
	{  22,   77, 1702, 1702, 4051,  1460 },	/* Pointe Coupee, Louisiana */
	{  22,   79, 1701, 1701, 4026,  1516 },	/* Rapides, Louisiana */
	{  22,   81, 1701, 1701, 4026,  1524 },	/* Red River, Louisiana */
	{  22,   83, 1701, 1701, 4026,  1539 },	/* Richland, Louisiana */
	{  22,   85, 1701, 1701, 4026,  1588 },	/* Sabine, Louisiana */
	{  22,   87, 1702, 1702, 4051,  1707 },	/* St Bernard, Louisiana */
	{  22,   89, 1702, 1702, 4051,  1708 },	/* St Charles, Louisiana */
	{  22,   91, 1702, 1702, 4051,  1714 },	/* St Helena, Louisiana */
	{  22,   93, 1702, 1702, 4051,  1715 },	/* St James, Louisiana */
	{  22,   95, 1702, 1702, 4051,  1717 },	/* St John The Baptist, Louisiana */
	{  22,   97, 1702, 1702, 4051,  1720 },	/* St Landry, Louisiana */
	{  22,   99, 1702, 1702, 4051,  1725 },	/* St Martin, Louisiana */
	{  22,  101, 1702, 1702, 4051,  1726 },	/* St Mary, Louisiana */
	{  22,  103, 1702, 1702, 4051,  1728 },	/* St Tammany, Louisiana */
	{  22,  105, 1702, 1702, 4051,  1786 },	/* Tangipahoa, Louisiana */
	{  22,  107, 1701, 1701, 4026,  1797 },	/* Tensas, Louisiana */
	{  22,  109, 1702, 1702, 4051,  1798 },	/* Terrebonne, Louisiana */
	{  22,  111, 1701, 1701, 4026,  1867 },	/* Union, Louisiana */
	{  22,  113, 1702, 1702, 4051,  1888 },	/* Vermilion, Louisiana */
	{  22,  115, 1701, 1701, 4026,  1891 },	/* Vernon, Louisiana */
	{  22,  117, 1702, 1702, 4051,  1933 },	/* Washington, Louisiana */
	{  22,  119, 1701, 1701, 4026,  1947 },	/* Webster, Louisiana */
	{  22,  121, 1702, 1702, 4051,  1950 },	/* West Baton Rouge, Louisiana */
	{  22,  123, 1701, 1701, 4026,  1951 },	/* West Carroll, Louisiana */
	{  22,  125, 1702, 1702, 4051,  1952 },	/* West Feliciana, Louisiana */
	{  22,  127, 1701, 1701, 4026,  1989 },	/* Winn, Louisiana */
	{  23,    0,    0,    0,    0,  1106 },	/* Maine, Maine */
	{  23,    1, 1802, 1802, 4101,    54 },	/* Androscoggin, Maine */
	{  23,    3, 1801, 1801, 4076,    77 },	/* Aroostook, Maine */
	{  23,    5, 1802, 1802, 4101,   475 },	/* Cumberland, Maine */
	{  23,    7, 1802, 1802, 4101,   646 },	/* Franklin, Maine */
	{  23,    9, 1801, 1801, 4076,   771 },	/* Hancock, Maine */
	{  23,   11, 1802, 1802, 4101,   946 },	/* Kennebec, Maine */
	{  23,   13, 1801, 1801, 4076,   982 },	/* Knox, Maine */
	{  23,   15, 1802, 1802, 4101,  1060 },	/* Lincoln, Maine */
	{  23,   17, 1802, 1802, 4101,  1383 },	/* Oxford, Maine */
	{  23,   19, 1801, 1801, 4076,  1419 },	/* Penobscot, Maine */
	{  23,   21, 1801, 1801, 4076,  1445 },	/* Piscataquis, Maine */
	{  23,   23, 1802, 1802, 4101,  1591 },	/* Sagadahoc, Maine */
	{  23,   25, 1802, 1802, 4101,  1692 },	/* Somerset, Maine */
	{  23,   27, 1801, 1801, 4076,  1912 },	/* Waldo, Maine */
	{  23,   29, 1801, 1801, 4076,  1933 },	/* Washington, Maine */
	{  23,   31, 1802, 1802, 4101,  2033 },	/* York, Maine */
	{  24,    0,    0,    0,    0,  1133 },	/* Maryland, Maryland */
	{  24,    1, 1900, 1900, 4126,    37 },	/* Allegany, Maryland */
	{  24,    3, 1900, 1900, 4126,    57 },	/* Anne Arundel, Maryland */
	{  24,    5, 1900, 1900, 4126,   111 },	/* Baltimore, Maryland */
	{  24,    7, 1900, 1900, 4126,   113 },	/* Baltimore City, Maryland */
	{  24,    9, 1900, 1900, 4126,   281 },	/* Calvert, Maryland */
	{  24,   11, 1900, 1900, 4126,   301 },	/* Caroline, Maryland */
	{  24,   13, 1900, 1900, 4126,   302 },	/* Carroll, Maryland */
	{  24,   15, 1900, 1900, 4126,   323 },	/* Cecil, Maryland */
	{  24,   17, 1900, 1900, 4126,   332 },	/* Charles, Maryland */
	{  24,   19, 1900, 1900, 4126,   544 },	/* Dorchester, Maryland */
	{  24,   21, 1900, 1900, 4126,   648 },	/* Frederick, Maryland */
	{  24,   23, 1900, 1900, 4126,   669 },	/* Garrett, Maryland */
	{  24,   25, 1900, 1900, 4126,   782 },	/* Harford, Maryland */
	{  24,   27, 1900, 1900, 4126,   845 },	/* Howard, Maryland */
	{  24,   29, 1900, 1900, 4126,   948 },	/* Kent, Maryland */
	{  24,   31, 1900, 1900, 4126,  1232 },	/* Montgomery, Maryland */
	{  24,   33, 1900, 1900, 4126,  1489 },	/* Prince Georges, Maryland */
	{  24,   35, 1900, 1900, 4126,  1502 },	/* Queen Annes, Maryland */
	{  24,   37, 1900, 1900, 4126,  1727 },	/* St Marys, Maryland */
	{  24,   39, 1900, 1900, 4126,  1692 },	/* Somerset, Maryland */
	{  24,   41, 1900, 1900, 4126,  1779 },	/* Talbot, Maryland */
	{  24,   43, 1900, 1900, 4126,  1933 },	/* Washington, Maryland */
	{  24,   45, 1900, 1900, 4126,  1972 },	/* Wicomico, Maryland */
	{  24,   47, 1900, 1900, 4126,  2005 },	/* Worcester, Maryland */
	{  24,  510,    0,    0,    0,   112 },	/* Baltimore (city), Maryland */
	{  25,    0,    0,    0,    0,  1136 },	/* Massachusetts, Massachusetts */
	{  25,    1, 2001, 2001, 4151,   124 },	/* Barnstable, Massachusetts */
	{  25,    3, 2001, 2001, 4151,   168 },	/* Berkshire, Massachusetts */
	{  25,    5, 2001, 2001, 4151,   230 },	/* Bristol, Massachusetts */
	{  25,    7, 2002, 2002, 4176,   552 },	/* Dukes, Massachusetts */
	{  25,    9, 2001, 2001, 4151,   602 },	/* Essex, Massachusetts */
	{  25,   11, 2001, 2001, 4151,   646 },	/* Franklin, Massachusetts */
	{  25,   13, 2001, 2001, 4151,   767 },	/* Hampden, Massachusetts */
	{  25,   15, 2001, 2001, 4151,   768 },	/* Hampshire, Massachusetts */
	{  25,   17, 2001, 2001, 4151,  1194 },	/* Middlesex, Massachusetts */
	{  25,   19, 2002, 2002, 4176,  1266 },	/* Nantucket, Massachusetts */
	{  25,   21, 2001, 2001, 4151,  1315 },	/* Norfolk, Massachusetts */
	{  25,   23, 2001, 2001, 4151,  1456 },	/* Plymouth, Massachusetts */
	{  25,   25, 2001, 2001, 4151,  1757 },	/* Suffolk, Massachusetts */
	{  25,   27, 2001, 2001, 4151,  2005 },	/* Worcester, Massachusetts */
	{  26,    0,    0,    0,    0,  1193 },	/* Michigan, Michigan */
	{  26,    1, 2112, 2112, 6376,    27 },	/* Alcona, Michigan */
	{  26,    3, 2111, 2111, 6351,    34 },	/* Alger, Michigan */
	{  26,    5, 2113, 2113, 6401,    36 },	/* Allegan, Michigan */
	{  26,    7, 2112, 2112, 6376,    42 },	/* Alpena, Michigan */
	{  26,    9, 2112, 2112, 6376,    61 },	/* Antrim, Michigan */
	{  26,   11, 2112, 2112, 6376,    71 },	/* Arenac, Michigan */
	{  26,   13, 2111, 2111, 6351,   119 },	/* Baraga, Michigan */
	{  26,   15, 2113, 2113, 6401,   130 },	/* Barry, Michigan */
	{  26,   17, 2113, 2113, 6401,   138 },	/* Bay, Michigan */
	{  26,   19, 2112, 2112, 6376,   164 },	/* Benzie, Michigan */
	{  26,   21, 2113, 2113, 6401,   170 },	/* Berrien, Michigan */
	{  26,   23, 2113, 2113, 6401,   219 },	/* Branch, Michigan */
	{  26,   25, 2113, 2113, 6401,   275 },	/* Calhoun, Michigan */
	{  26,   27, 2113, 2113, 6401,   310 },	/* Cass, Michigan */
	{  26,   29, 2112, 2112, 6376,   336 },	/* Charlevoix, Michigan */
	{  26,   31, 2112, 2112, 6376,   347 },	/* Cheboygan, Michigan */
	{  26,   33, 2111, 2111, 6351,   362 },	/* Chippewa, Michigan */
	{  26,   35, 2112, 2112, 6376,   379 },	/* Clare, Michigan */
	{  26,   37, 2113, 2113, 6401,   395 },	/* Clinton, Michigan */
	{  26,   39, 2112, 2112, 6376,   460 },	/* Crawford, Michigan */
	{  26,   41, 2111, 2111, 6351,   513 },	/* Delta, Michigan */
	{  26,   43, 2111, 2111, 6351,   525 },	/* Dickinson, Michigan */
	{  26,   45, 2113, 2113, 6401,   568 },	/* Eaton, Michigan */
	{  26,   47, 2112, 2112, 6376,   593 },	/* Emmet, Michigan */
	{  26,   49, 2113, 2113, 6401,   679 },	/* Genesee, Michigan */
	{  26,   51, 2112, 2112, 6376,   695 },	/* Gladwin, Michigan */
	{  26,   53, 2111, 2111, 6351,   701 },	/* Gogebic, Michigan */
	{  26,   55, 2112, 2112, 6376,   719 },	/* Grand Traverse, Michigan */
	{  26,   57, 2113, 2113, 6401,   723 },	/* Gratiot, Michigan */
	{  26,   59, 2113, 2113, 6401,   823 },	/* Hillsdale, Michigan */
	{  26,   61, 2111, 2111, 6351,   843 },	/* Houghton, Michigan */
	{  26,   63, 2113, 2113, 6401,   860 },	/* Huron, Michigan */
	{  26,   65, 2113, 2113, 6401,   872 },	/* Ingham, Michigan */
	{  26,   67, 2113, 2113, 6401,   874 },	/* Ionia, Michigan */
	{  26,   69, 2112, 2112, 6376,   875 },	/* Iosco, Michigan */
	{  26,   71, 2111, 2111, 6351,   879 },	/* Iron, Michigan */
	{  26,   73, 2113, 2113, 6401,   883 },	/* Isabella, Michigan */
	{  26,   75, 2113, 2113, 6401,   893 },	/* Jackson, Michigan */
	{  26,   77, 2113, 2113, 6401,   925 },	/* Kalamazoo, Michigan */
	{  26,   79, 2112, 2112, 6376,   927 },	/* Kalkaska, Michigan */
	{  26,   81, 2113, 2113, 6401,   948 },	/* Kent, Michigan */
	{  26,   83, 2111, 2111, 6351,   957 },	/* Keweenaw, Michigan */
	{  26,   85, 2112, 2112, 6376,  1006 },	/* Lake, Michigan */
	{  26,   87, 2113, 2113, 6401,  1018 },	/* Lapeer, Michigan */
	{  26,   89, 2112, 2112, 6376,  1041 },	/* Leelanau, Michigan */
	{  26,   91, 2113, 2113, 6401,  1045 },	/* Lenawee, Michigan */
	{  26,   93, 2113, 2113, 6401,  1066 },	/* Livingston, Michigan */
	{  26,   95, 2111, 2111, 6351,  1085 },	/* Luce, Michigan */
	{  26,   97, 2111, 2111, 6351,  1096 },	/* Mackinac, Michigan */
	{  26,   99, 2113, 2113, 6401,  1097 },	/* Macomb, Michigan */
	{  26,  101, 2112, 2112, 6376,  1115 },	/* Manistee, Michigan */
	{  26,  103, 2111, 2111, 6351,  1128 },	/* Marquette, Michigan */
	{  26,  105, 2112, 2112, 6376,  1134 },	/* Mason, Michigan */
	{  26,  107, 2113, 2113, 6401,  1174 },	/* Mecosta, Michigan */
	{  26,  109, 2111, 2111, 6351,  1184 },	/* Menominee, Michigan */
	{  26,  111, 2113, 2113, 6401,  1195 },	/* Midland, Michigan */
	{  26,  113, 2112, 2112, 6376,  1211 },	/* Missaukee, Michigan */
	{  26,  115, 2113, 2113, 6401,  1226 },	/* Monroe, Michigan */
	{  26,  117, 2113, 2113, 6401,  1229 },	/* Montcalm, Michigan */
	{  26,  119, 2112, 2112, 6376,  1233 },	/* Montmorency, Michigan */
	{  26,  121, 2113, 2113, 6401,  1256 },	/* Muskegon, Michigan */
	{  26,  123, 2113, 2113, 6401,  1293 },	/* Newaygo, Michigan */
	{  26,  125, 2113, 2113, 6401,  1335 },	/* Oakland, Michigan */
	{  26,  127, 2113, 2113, 6401,  1338 },	/* Oceana, Michigan */
	{  26,  129, 2112, 2112, 6376,  1342 },	/* Ogemaw, Michigan */
	{  26,  131, 2111, 2111, 6351,  1360 },	/* Ontonagon, Michigan */
	{  26,  133, 2112, 2112, 6376,  1368 },	/* Osceola, Michigan */
	{  26,  135, 2112, 2112, 6376,  1369 },	/* Oscoda, Michigan */
	{  26,  137, 2112, 2112, 6376,  1373 },	/* Otsego, Michigan */
	{  26,  139, 2113, 2113, 6401,  1374 },	/* Ottawa, Michigan */
	{  26,  141, 2112, 2112, 6376,  1484 },	/* Presque Isle, Michigan */
	{  26,  143, 2112, 2112, 6376,  1572 },	/* Roscommon, Michigan */
	{  26,  145, 2113, 2113, 6401,  1592 },	/* Saginaw, Michigan */
	{  26,  147, 2113, 2113, 6401,  1709 },	/* St Clair, Michigan */
	{  26,  149, 2113, 2113, 6401,  1719 },	/* St Joseph, Michigan */
	{  26,  151, 2113, 2113, 6401,  1623 },	/* Sanilac, Michigan */
	{  26,  153, 2111, 2111, 6351,  1642 },	/* Schoolcraft, Michigan */
	{  26,  155, 2113, 2113, 6401,  1673 },	/* Shiawassee, Michigan */
	{  26,  157, 2113, 2113, 6401,  1854 },	/* Tuscola, Michigan */
	{  26,  159, 2113, 2113, 6401,  1879 },	/* Van Buren, Michigan */
	{  26,  161, 2113, 2113, 6401,  1936 },	/* Washtenaw, Michigan */
	{  26,  163, 2113, 2113, 6401,  1942 },	/* Wayne, Michigan */
	{  26,  165, 2112, 2112, 6376,  1959 },	/* Wexford, Michigan */
	{  27,    0,    0,    0,    0,  1210 },	/* Minnesota, Minnesota */
	{  27,    1, 2202, 2202, 4301,    18 },	/* Aitkin, Minnesota */
	{  27,    3, 2203, 2203, 4326,    58 },	/* Anoka, Minnesota */
	{  27,    5, 2202, 2202, 4301,   148 },	/* Becker, Minnesota */
	{  27,    7, 2201, 2201, 4276,   156 },	/* Beltrami, Minnesota */
	{  27,    9, 2202, 2202, 4301,   163 },	/* Benton, Minnesota */
	{  27,   11, 2203, 2203, 4326,   177 },	/* Big Stone, Minnesota */
	{  27,   13, 2203, 2203, 4326,   192 },	/* Blue Earth, Minnesota */
	{  27,   15, 2203, 2203, 4326,   240 },	/* Brown, Minnesota */
	{  27,   17, 2202, 2202, 4301,   299 },	/* Carlton, Minnesota */
	{  27,   19, 2203, 2203, 4326,   307 },	/* Carver, Minnesota */
	{  27,   21, 2202, 2202, 4301,   310 },	/* Cass, Minnesota */
	{  27,   23, 2203, 2203, 4326,   362 },	/* Chippewa, Minnesota */
	{  27,   25, 2202, 2202, 4301,   363 },	/* Chisago, Minnesota */
	{  27,   27, 2202, 2202, 4301,   385 },	/* Clay, Minnesota */
	{  27,   29, 2201, 2201, 4276,   389 },	/* Clearwater, Minnesota */
	{  27,   31, 2201, 2201, 4276,   436 },	/* Cook, Minnesota */
	{  27,   33, 2203, 2203, 4326,   450 },	/* Cottonwood, Minnesota */
	{  27,   35, 2202, 2202, 4301,   469 },	/* Crow Wing, Minnesota */
	{  27,   37, 2203, 2203, 4326,   483 },	/* Dakota, Minnesota */
	{  27,   39, 2203, 2203, 4326,   536 },	/* Dodge, Minnesota */
	{  27,   41, 2202, 2202, 4301,   546 },	/* Douglas, Minnesota */
	{  27,   43, 2203, 2203, 4326,   618 },	/* Faribault, Minnesota */
	{  27,   45, 2203, 2203, 4326,   627 },	/* Fillmore, Minnesota */
	{  27,   47, 2203, 2203, 4326,   650 },	/* Freeborn, Minnesota */
	{  27,   49, 2203, 2203, 4326,   706 },	/* Goodhue, Minnesota */
	{  27,   51, 2202, 2202, 4301,   721 },	/* Grant, Minnesota */
	{  27,   53, 2203, 2203, 4326,   809 },	/* Hennepin, Minnesota */
	{  27,   55, 2203, 2203, 4326,   844 },	/* Houston, Minnesota */
	{  27,   57, 2202, 2202, 4301,   848 },	/* Hubbard, Minnesota */
	{  27,   59, 2202, 2202, 4301,   884 },	/* Isanti, Minnesota */
	{  27,   61, 2201, 2201, 4276,   888 },	/* Itasca, Minnesota */
	{  27,   63, 2203, 2203, 4326,   893 },	/* Jackson, Minnesota */
	{  27,   65, 2202, 2202, 4301,   928 },	/* Kanabec, Minnesota */
	{  27,   67, 2203, 2203, 4326,   930 },	/* Kandiyohi, Minnesota */
	{  27,   69, 2201, 2201, 4276,   977 },	/* Kittson, Minnesota */
	{  27,   71, 2201, 2201, 4276,   984 },	/* Koochiching, Minnesota */
	{  27,   73, 2203, 2203, 4326,   998 },	/* Lac Qui Parle, Minnesota */
	{  27,   75, 2201, 2201, 4276,  1006 },	/* Lake, Minnesota */
	{  27,   77, 2201, 2201, 4276,  1008 },	/* Lake of the Woods, Minnesota */
	{  27,   79, 2203, 2203, 4326,  1035 },	/* Le Sueur, Minnesota */
	{  27,   81, 2203, 2203, 4326,  1060 },	/* Lincoln, Minnesota */
	{  27,   83, 2203, 2203, 4326,  1095 },	/* Lyon, Minnesota */
	{  27,   85, 2203, 2203, 4326,  1163 },	/* Mc Leod, Minnesota */
	{  27,   87, 2201, 2201, 4276,  1104 },	/* Mahnomen, Minnesota */
	{  27,   89, 2201, 2201, 4276,  1129 },	/* Marshall, Minnesota */
	{  27,   91, 2203, 2203, 4326,  1131 },	/* Martin, Minnesota */
	{  27,   93, 2203, 2203, 4326,  1176 },	/* Meeker, Minnesota */
	{  27,   95, 2202, 2202, 4301,  1201 },	/* Mille Lacs, Minnesota */
	{  27,   97, 2202, 2202, 4301,  1244 },	/* Morrison, Minnesota */
	{  27,   99, 2203, 2203, 4326,  1250 },	/* Mower, Minnesota */
	{  27,  101, 2203, 2203, 4326,  1253 },	/* Murray, Minnesota */
	{  27,  103, 2203, 2203, 4326,  1308 },	/* Nicollet, Minnesota */
	{  27,  105, 2203, 2203, 4326,  1311 },	/* Nobles, Minnesota */
	{  27,  107, 2201, 2201, 4276,  1317 },	/* Norman, Minnesota */
	{  27,  109, 2203, 2203, 4326,  1355 },	/* Olmsted, Minnesota */
	{  27,  111, 2202, 2202, 4301,  1375 },	/* Otter Tail, Minnesota */
	{  27,  113, 2201, 2201, 4276,  1417 },	/* Pennington, Minnesota */
	{  27,  115, 2202, 2202, 4301,  1442 },	/* Pine, Minnesota */
	{  27,  117, 2203, 2203, 4326,  1444 },	/* Pipestone, Minnesota */
	{  27,  119, 2201, 2201, 4276,  1461 },	/* Polk, Minnesota */
	{  27,  121, 2202, 2202, 4301,  1465 },	/* Pope, Minnesota */
	{  27,  123, 2203, 2203, 4326,  1511 },	/* Ramsey, Minnesota */
	{  27,  125, 2201, 2201, 4276,  1523 },	/* Red Lake, Minnesota */
	{  27,  127, 2203, 2203, 4326,  1526 },	/* Redwood, Minnesota */
	{  27,  129, 2203, 2203, 4326,  1531 },	/* Renville, Minnesota */
	{  27,  131, 2203, 2203, 4326,  1536 },	/* Rice, Minnesota */
	{  27,  133, 2203, 2203, 4326,  1557 },	/* Rock, Minnesota */
	{  27,  135, 2201, 2201, 4276,  1574 },	/* Roseau, Minnesota */
	{  27,  137, 2201, 2201, 4276,  1722 },	/* St Louis, Minnesota */
	{  27,  139, 2203, 2203, 4326,  1647 },	/* Scott, Minnesota */
	{  27,  141, 2203, 2203, 4326,  1670 },	/* Sherburne, Minnesota */
	{  27,  143, 2203, 2203, 4326,  1675 },	/* Sibley, Minnesota */
	{  27,  145, 2202, 2202, 4301,  1739 },	/* Stearns, Minnesota */
	{  27,  147, 2203, 2203, 4326,  1740 },	/* Steele, Minnesota */
	{  27,  149, 2202, 2202, 4301,  1745 },	/* Stevens, Minnesota */
	{  27,  151, 2203, 2203, 4326,  1776 },	/* Swift, Minnesota */
	{  27,  153, 2202, 2202, 4301,  1819 },	/* Todd, Minnesota */
	{  27,  155, 2202, 2202, 4301,  1832 },	/* Traverse, Minnesota */
	{  27,  157, 2203, 2203, 4326,  1903 },	/* Wabasha, Minnesota */
	{  27,  159, 2202, 2202, 4301,  1906 },	/* Wadena, Minnesota */
	{  27,  161, 2203, 2203, 4326,  1929 },	/* Waseca, Minnesota */
	{  27,  163, 2203, 2203, 4326,  1933 },	/* Washington, Minnesota */
	{  27,  165, 2203, 2203, 4326,  1938 },	/* Watonwan, Minnesota */
	{  27,  167, 2202, 2202, 4301,  1976 },	/* Wilkin, Minnesota */
	{  27,  169, 2203, 2203, 4326,  1992 },	/* Winona, Minnesota */
	{  27,  171, 2203, 2203, 4326,  2010 },	/* Wright, Minnesota */
	{  27,  173, 2203, 2203, 4326,  2028 },	/* Yellow Medicine, Minnesota */
	{  28,    0,    0,    0,    0,  1212 },	/* Mississippi, Mississippi */
	{  28,    1, 2302, 2302, 4376,     5 },	/* Adams, Mississippi */
	{  28,    3, 2301, 2301, 4351,    28 },	/* Alcorn, Mississippi */
	{  28,    5, 2302, 2302, 4376,    48 },	/* Amite, Mississippi */
	{  28,    7, 2301, 2301, 4351,    93 },	/* Attala, Mississippi */
	{  28,    9, 2301, 2301, 4351,   163 },	/* Benton, Mississippi */
	{  28,   11, 2302, 2302, 4376,   195 },	/* Bolivar, Mississippi */
	{  28,   13, 2301, 2301, 4351,   275 },	/* Calhoun, Mississippi */
	{  28,   15, 2302, 2302, 4376,   302 },	/* Carroll, Mississippi */
	{  28,   17, 2301, 2301, 4351,   358 },	/* Chickasaw, Mississippi */
	{  28,   19, 2301, 2301, 4351,   365 },	/* Choctaw, Mississippi */
	{  28,   21, 2302, 2302, 4376,   377 },	/* Claiborne, Mississippi */
	{  28,   23, 2301, 2301, 4351,   383 },	/* Clarke, Mississippi */
	{  28,   25, 2301, 2301, 4351,   385 },	/* Clay, Mississippi */
	{  28,   27, 2302, 2302, 4376,   397 },	/* Coahoma, Mississippi */
	{  28,   29, 2302, 2302, 4376,   441 },	/* Copiah, Mississippi */
	{  28,   31, 2301, 2301, 4351,   451 },	/* Covington, Mississippi */
	{  28,   33, 2302, 2302, 4376,   504 },	/* De Soto, Mississippi */
	{  28,   35, 2301, 2301, 4351,   641 },	/* Forrest, Mississippi */
	{  28,   37, 2302, 2302, 4376,   646 },	/* Franklin, Mississippi */
	{  28,   39, 2301, 2301, 4351,   682 },	/* George, Mississippi */
	{  28,   41, 2301, 2301, 4351,   732 },	/* Greene, Mississippi */
	{  28,   43, 2302, 2302, 4376,   741 },	/* Grenada, Mississippi */
	{  28,   45, 2301, 2301, 4351,   771 },	/* Hancock, Mississippi */
	{  28,   47, 2301, 2301, 4351,   789 },	/* Harrison, Mississippi */
	{  28,   49, 2302, 2302, 4376,   824 },	/* Hinds, Mississippi */
	{  28,   51, 2302, 2302, 4376,   831 },	/* Holmes, Mississippi */
	{  28,   53, 2302, 2302, 4376,   855 },	/* Humphreys, Mississippi */
	{  28,   55, 2302, 2302, 4376,   887 },	/* Issaquena, Mississippi */
	{  28,   57, 2301, 2301, 4351,   889 },	/* Itawamba, Mississippi */
	{  28,   59, 2301, 2301, 4351,   893 },	/* Jackson, Mississippi */
	{  28,   61, 2301, 2301, 4351,   897 },	/* Jasper, Mississippi */
	{  28,   63, 2302, 2302, 4376,   901 },	/* Jefferson, Mississippi */
	{  28,   65, 2302, 2302, 4376,   902 },	/* Jefferson Davis, Mississippi */
	{  28,   67, 2301, 2301, 4351,   917 },	/* Jones, Mississippi */
	{  28,   69, 2301, 2301, 4351,   942 },	/* Kemper, Mississippi */
	{  28,   71, 2301, 2301, 4351,  1002 },	/* Lafayette, Mississippi */
	{  28,   73, 2301, 2301, 4351,  1009 },	/* Lamar, Mississippi */
	{  28,   75, 2301, 2301, 4351,  1029 },	/* Lauderdale, Mississippi */
	{  28,   77, 2302, 2302, 4376,  1033 },	/* Lawrence, Mississippi */
	{  28,   79, 2301, 2301, 4351,  1037 },	/* Leake, Mississippi */
	{  28,   81, 2301, 2301, 4351,  1040 },	/* Lee, Mississippi */
	{  28,   83, 2302, 2302, 4376,  1042 },	/* Leflore, Mississippi */
	{  28,   85, 2302, 2302, 4376,  1060 },	/* Lincoln, Mississippi */
	{  28,   87, 2301, 2301, 4351,  1082 },	/* Lowndes, Mississippi */
	{  28,   89, 2302, 2302, 4376,  1101 },	/* Madison, Mississippi */
	{  28,   91, 2302, 2302, 4376,  1125 },	/* Marion, Mississippi */
	{  28,   93, 2301, 2301, 4351,  1129 },	/* Marshall, Mississippi */
	{  28,   95, 2301, 2301, 4351,  1226 },	/* Monroe, Mississippi */
	{  28,   97, 2302, 2302, 4376,  1232 },	/* Montgomery, Mississippi */
	{  28,   99, 2301, 2301, 4351,  1280 },	/* Neshoba, Mississippi */
	{  28,  101, 2301, 2301, 4351,  1297 },	/* Newton, Mississippi */
	{  28,  103, 2301, 2301, 4351,  1330 },	/* Noxubee, Mississippi */
	{  28,  105, 2301, 2301, 4351,  1352 },	/* Oktibbeha, Mississippi */
	{  28,  107, 2302, 2302, 4376,  1394 },	/* Panola, Mississippi */
	{  28,  109, 2301, 2301, 4351,  1409 },	/* Pearl River, Mississippi */
	{  28,  111, 2301, 2301, 4351,  1425 },	/* Perry, Mississippi */
	{  28,  113, 2302, 2302, 4376,  1439 },	/* Pike, Mississippi */
	{  28,  115, 2301, 2301, 4351,  1464 },	/* Pontotoc, Mississippi */
	{  28,  117, 2301, 2301, 4351,  1482 },	/* Prentiss, Mississippi */
	{  28,  119, 2302, 2302, 4376,  1504 },	/* Quitman, Mississippi */
	{  28,  121, 2302, 2302, 4376,  1514 },	/* Rankin, Mississippi */
	{  28,  123, 2301, 2301, 4351,  1647 },	/* Scott, Mississippi */
	{  28,  125, 2302, 2302, 4376,  1662 },	/* Sharkey, Mississippi */
	{  28,  127, 2302, 2302, 4376,  1678 },	/* Simpson, Mississippi */
	{  28,  129, 2301, 2301, 4351,  1686 },	/* Smith, Mississippi */
	{  28,  131, 2301, 2301, 4351,  1750 },	/* Stone, Mississippi */
	{  28,  133, 2302, 2302, 4376,  1765 },	/* Sunflower, Mississippi */
	{  28,  135, 2302, 2302, 4376,  1782 },	/* Tallahatchie, Mississippi */
	{  28,  137, 2302, 2302, 4376,  1789 },	/* Tate, Mississippi */
	{  28,  139, 2301, 2301, 4351,  1812 },	/* Tippah, Mississippi */
	{  28,  141, 2301, 2301, 4351,  1815 },	/* Tishomingo, Mississippi */
	{  28,  143, 2302, 2302, 4376,  1849 },	/* Tunica, Mississippi */
	{  28,  145, 2301, 2301, 4351,  1867 },	/* Union, Mississippi */
	{  28,  147, 2302, 2302, 4376,  1919 },	/* Walthall, Mississippi */
	{  28,  149, 2302, 2302, 4376,  1925 },	/* Warren, Mississippi */
	{  28,  151, 2302, 2302, 4376,  1933 },	/* Washington, Mississippi */
	{  28,  153, 2301, 2301, 4351,  1942 },	/* Wayne, Mississippi */
	{  28,  155, 2301, 2301, 4351,  1947 },	/* Webster, Mississippi */
	{  28,  157, 2302, 2302, 4376,  1977 },	/* Wilkinson, Mississippi */
	{  28,  159, 2301, 2301, 4351,  1993 },	/* Winston, Mississippi */
	{  28,  161, 2302, 2302, 4376,  2018 },	/* Yalobusha, Mississippi */
	{  28,  163, 2302, 2302, 4376,  2026 },	/* Yazoo, Mississippi */
	{  29,    0,    0,    0,    0,  1214 },	/* Missouri, Missouri */
	{  29,    1, 2402, 2402, 4426,     4 },	/* Adair, Missouri */
	{  29,    3, 2403, 2403, 4451,    52 },	/* Andrew, Missouri */
	{  29,    5, 2403, 2403, 4451,    88 },	/* Atchison, Missouri */
	{  29,    7, 2402, 2402, 4426,    94 },	/* Audrain, Missouri */
	{  29,    9, 2403, 2403, 4451,   130 },	/* Barry, Missouri */
	{  29,   11, 2403, 2403, 4451,   132 },	/* Barton, Missouri */
	{  29,   13, 2403, 2403, 4451,   135 },	/* Bates, Missouri */
	{  29,   15, 2402, 2402, 4426,   163 },	/* Benton, Missouri */
	{  29,   17, 2401, 2401, 4401,   196 },	/* Bollinger, Missouri */
	{  29,   19, 2402, 2402, 4426,   201 },	/* Boone, Missouri */
	{  29,   21, 2403, 2403, 4451,   244 },	/* Buchanan, Missouri */
	{  29,   23, 2401, 2401, 4401,   262 },	/* Butler, Missouri */
	{  29,   25, 2403, 2403, 4451,   273 },	/* Caldwell, Missouri */
	{  29,   27, 2402, 2402, 4426,   278 },	/* Callaway, Missouri */
	{  29,   29, 2402, 2402, 4426,   284 },	/* Camden, Missouri */
	{  29,   31, 2401, 2401, 4401,   294 },	/* Cape Girardeau, Missouri */
	{  29,   33, 2402, 2402, 4426,   302 },	/* Carroll, Missouri */
	{  29,   35, 2401, 2401, 4401,   305 },	/* Carter, Missouri */
	{  29,   37, 2403, 2403, 4451,   310 },	/* Cass, Missouri */
	{  29,   39, 2403, 2403, 4451,   324 },	/* Cedar, Missouri */
	{  29,   41, 2402, 2402, 4426,   331 },	/* Chariton, Missouri */
	{  29,   43, 2402, 2402, 4426,   368 },	/* Christian, Missouri */
	{  29,   45, 2401, 2401, 4401,   382 },	/* Clark, Missouri */
	{  29,   47, 2403, 2403, 4451,   385 },	/* Clay, Missouri */
	{  29,   49, 2403, 2403, 4451,   395 },	/* Clinton, Missouri */
	{  29,   51, 2402, 2402, 4426,   410 },	/* Cole, Missouri */
	{  29,   53, 2402, 2402, 4426,   438 },	/* Cooper, Missouri */
	{  29,   55, 2401, 2401, 4401,   460 },	/* Crawford, Missouri */
	{  29,   57, 2403, 2403, 4451,   481 },	/* Dade, Missouri */
	{  29,   59, 2402, 2402, 4426,   486 },	/* Dallas, Missouri */
	{  29,   61, 2403, 2403, 4451,   496 },	/* Daviess, Missouri */
	{  29,   63, 2403, 2403, 4451,   503 },	/* De Kalb, Missouri */
	{  29,   65, 2401, 2401, 4401,   514 },	/* Dent, Missouri */
	{  29,   67, 2402, 2402, 4426,   546 },	/* Douglas, Missouri */
	{  29,   69, 2401, 2401, 4401,   554 },	/* Dunklin, Missouri */
	{  29,   71, 2401, 2401, 4401,   646 },	/* Franklin, Missouri */
	{  29,   73, 2401, 2401, 4401,   672 },	/* Gasconade, Missouri */
	{  29,   75, 2403, 2403, 4451,   681 },	/* Gentry, Missouri */
	{  29,   77, 2402, 2402, 4426,   732 },	/* Greene, Missouri */
	{  29,   79, 2402, 2402, 4426,   744 },	/* Grundy, Missouri */
	{  29,   81, 2403, 2403, 4451,   789 },	/* Harrison, Missouri */
	{  29,   83, 2403, 2403, 4451,   811 },	/* Henry, Missouri */
	{  29,   85, 2402, 2402, 4426,   817 },	/* Hickory, Missouri */
	{  29,   87, 2403, 2403, 4451,   832 },	/* Holt, Missouri */
	{  29,   89, 2402, 2402, 4426,   845 },	/* Howard, Missouri */
	{  29,   91, 2402, 2402, 4426,   846 },	/* Howell, Missouri */
	{  29,   93, 2401, 2401, 4401,   879 },	/* Iron, Missouri */
	{  29,   95, 2403, 2403, 4451,   893 },	/* Jackson, Missouri */
	{  29,   97, 2403, 2403, 4451,   897 },	/* Jasper, Missouri */
	{  29,   99, 2401, 2401, 4401,   901 },	/* Jefferson, Missouri */
	{  29,  101, 2403, 2403, 4451,   914 },	/* Johnson, Missouri */
	{  29,  103, 2402, 2402, 4426,   982 },	/* Knox, Missouri */
	{  29,  105, 2402, 2402, 4426,  1000 },	/* Laclede, Missouri */
	{  29,  107, 2403, 2403, 4451,  1002 },	/* Lafayette, Missouri */
	{  29,  109, 2403, 2403, 4451,  1033 },	/* Lawrence, Missouri */
	{  29,  111, 2401, 2401, 4401,  1051 },	/* Lewis, Missouri */
	{  29,  113, 2401, 2401, 4401,  1060 },	/* Lincoln, Missouri */
	{  29,  115, 2402, 2402, 4426,  1061 },	/* Linn, Missouri */
	{  29,  117, 2402, 2402, 4426,  1066 },	/* Livingston, Missouri */
	{  29,  119, 2403, 2403, 4451,  1153 },	/* Mc Donald, Missouri */
	{  29,  121, 2402, 2402, 4426,  1098 },	/* Macon, Missouri */
	{  29,  123, 2401, 2401, 4401,  1101 },	/* Madison, Missouri */
	{  29,  125, 2402, 2402, 4426,  1122 },	/* Maries, Missouri */
	{  29,  127, 2401, 2401, 4401,  1125 },	/* Marion, Missouri */
	{  29,  129, 2402, 2402, 4426,  1186 },	/* Mercer, Missouri */
	{  29,  131, 2402, 2402, 4426,  1202 },	/* Miller, Missouri */
	{  29,  133, 2401, 2401, 4401,  1212 },	/* Mississippi, Missouri */
	{  29,  135, 2402, 2402, 4426,  1221 },	/* Moniteau, Missouri */
	{  29,  137, 2402, 2402, 4426,  1226 },	/* Monroe, Missouri */
	{  29,  139, 2401, 2401, 4401,  1232 },	/* Montgomery, Missouri */
	{  29,  141, 2402, 2402, 4426,  1240 },	/* Morgan, Missouri */
	{  29,  143, 2401, 2401, 4401,  1290 },	/* New Madrid, Missouri */
	{  29,  145, 2403, 2403, 4451,  1297 },	/* Newton, Missouri */
	{  29,  147, 2403, 2403, 4451,  1312 },	/* Nodaway, Missouri */
	{  29,  149, 2401, 2401, 4401,  1363 },	/* Oregon, Missouri */
	{  29,  151, 2402, 2402, 4426,  1366 },	/* Osage, Missouri */
	{  29,  153, 2402, 2402, 4426,  1384 },	/* Ozark, Missouri */
	{  29,  155, 2401, 2401, 4401,  1413 },	/* Pemiscot, Missouri */
	{  29,  157, 2401, 2401, 4401,  1425 },	/* Perry, Missouri */
	{  29,  159, 2402, 2402, 4426,  1430 },	/* Pettis, Missouri */
	{  29,  161, 2402, 2402, 4426,  1431 },	/* Phelps, Missouri */
	{  29,  163, 2401, 2401, 4401,  1439 },	/* Pike, Missouri */
	{  29,  165, 2403, 2403, 4451,  1453 },	/* Platte, Missouri */
	{  29,  167, 2402, 2402, 4426,  1461 },	/* Polk, Missouri */
	{  29,  169, 2402, 2402, 4426,  1497 },	/* Pulaski, Missouri */
	{  29,  171, 2402, 2402, 4426,  1499 },	/* Putnam, Missouri */
	{  29,  173, 2401, 2401, 4401,  1510 },	/* Ralls, Missouri */
	{  29,  175, 2402, 2402, 4426,  1513 },	/* Randolph, Missouri */
	{  29,  177, 2403, 2403, 4451,  1520 },	/* Ray, Missouri */
	{  29,  179, 2401, 2401, 4401,  1533 },	/* Reynolds, Missouri */
	{  29,  181, 2401, 2401, 4401,  1548 },	/* Ripley, Missouri */
	{  29,  183, 2401, 2401, 4401,  1708 },	/* St Charles, Missouri */
	{  29,  185, 2403, 2403, 4451,  1709 },	/* St Clair, Missouri */
	{  29,  186, 2401, 2401, 4401,  1713 },	/* St Genevieve, Missouri */
	{  29,  187, 2401, 2401, 4401,  1712 },	/* St Francois, Missouri */
	{  29,  189, 2401, 2401, 4401,  1722 },	/* St Louis, Missouri */
	{  29,  191, 2401, 2401, 4401,  1723 },	/* St Louis City, Missouri */
	{  29,  195, 2402, 2402, 4426,  1598 },	/* Saline, Missouri */
	{  29,  197, 2402, 2402, 4426,  1643 },	/* Schuyler, Missouri */
	{  29,  199, 2402, 2402, 4426,  1646 },	/* Scotland, Missouri */
	{  29,  201, 2401, 2401, 4401,  1647 },	/* Scott, Missouri */
	{  29,  203, 2401, 2401, 4401,  1661 },	/* Shannon, Missouri */
	{  29,  205, 2402, 2402, 4426,  1668 },	/* Shelby, Missouri */
	{  29,  207, 2401, 2401, 4401,  1748 },	/* Stoddard, Missouri */
	{  29,  209, 2402, 2402, 4426,  1750 },	/* Stone, Missouri */
	{  29,  211, 2402, 2402, 4426,  1759 },	/* Sullivan, Missouri */
	{  29,  213, 2402, 2402, 4426,  1785 },	/* Taney, Missouri */
	{  29,  215, 2402, 2402, 4426,  1802 },	/* Texas, Missouri */
	{  29,  217, 2403, 2403, 4451,  1891 },	/* Vernon, Missouri */
	{  29,  219, 2401, 2401, 4401,  1925 },	/* Warren, Missouri */
	{  29,  221, 2401, 2401, 4401,  1933 },	/* Washington, Missouri */
	{  29,  223, 2401, 2401, 4401,  1942 },	/* Wayne, Missouri */
	{  29,  225, 2402, 2402, 4426,  1947 },	/* Webster, Missouri */
	{  29,  227, 2403, 2403, 4451,  2006 },	/* Worth, Missouri */
	{  29,  229, 2402, 2402, 4426,  2010 },	/* Wright, Missouri */
	{  30,    0,    0,    0,    0,  1228 },	/* Montana, Montana */
	{  30,    1, 2500, 2503, 4526,   147 },	/* Beaverhead, Montana */
	{  30,    3, 2500, 2503, 4526,   176 },	/* Big Horn, Montana */
	{  30,    5, 2500, 2501, 4476,   185 },	/* Blaine, Montana */
	{  30,    7, 2500, 2503, 4526,   233 },	/* Broadwater, Montana */
	{  30,    9, 2500, 2503, 4526,   296 },	/* Carbon, Montana */
	{  30,   11, 2500, 2503, 4526,   305 },	/* Carter, Montana */
	{  30,   13, 2500, 2502, 4501,   308 },	/* Cascade, Montana */
	{  30,   15, 2500, 2501, 4476,   366 },	/* Chouteau, Montana */
	{  30,   17, 2500, 2503, 4526,   479 },	/* Custer, Montana */
	{  30,   19, 2500, 2501, 4476,   488 },	/* Daniels, Montana */
	{  30,   21, 2500, 2502, 4501,   500 },	/* Dawson, Montana */
	{  30,   23, 2500, 2503, 4526,   509 },	/* Deer Lodge, Montana */
	{  30,   25, 2500, 2503, 4526,   614 },	/* Fallon, Montana */
	{  30,   27, 2500, 2502, 4501,   625 },	/* Fergus, Montana */
	{  30,   29, 2500, 2501, 4476,   631 },	/* Flathead, Montana */
	{  30,   31, 2500, 2503, 4526,   662 },	/* Gallatin, Montana */
	{  30,   33, 2500, 2502, 4501,   666 },	/* Garfield, Montana */
	{  30,   35, 2500, 2501, 4476,   693 },	/* Glacier, Montana */
	{  30,   37, 2500, 2503, 4526,   702 },	/* Golden Valley, Montana */
	{  30,   39, 2500, 2503, 4526,   720 },	/* Granite, Montana */
	{  30,   41, 2500, 2501, 4476,   821 },	/* Hill, Montana */
	{  30,   43, 2500, 2503, 4526,   901 },	/* Jefferson, Montana */
	{  30,   45, 2500, 2502, 4501,   921 },	/* Judith Basin, Montana */
	{  30,   47, 2500, 2502, 4501,  1006 },	/* Lake, Montana */
	{  30,   49, 2500, 2502, 4501,  1052 },	/* Lewis And Clark, Montana */
	{  30,   51, 2500, 2501, 4476,  1056 },	/* Liberty, Montana */
	{  30,   53, 2500, 2501, 4476,  1060 },	/* Lincoln, Montana */
	{  30,   55, 2500, 2502, 4501,  1168 },	/* McCone, Montana */
	{  30,   57, 2500, 2503, 4526,  1101 },	/* Madison, Montana */
	{  30,   59, 2500, 2502, 4501,  1172 },	/* Meagher, Montana */
	{  30,   61, 2500, 2502, 4501,  1206 },	/* Mineral, Montana */
	{  30,   63, 2500, 2502, 4501,  1213 },	/* Missoula, Montana */
	{  30,   65, 2500, 2503, 4526,  1259 },	/* Musselshell, Montana */
	{  30,   67, 2500, 2503, 4526,  1395 },	/* Park, Montana */
	{  30,   69, 2500, 2502, 4501,  1429 },	/* Petroleum, Montana */
	{  30,   71, 2500, 2501, 4476,  1433 },	/* Phillips, Montana */
	{  30,   73, 2500, 2501, 4476,  1463 },	/* Pondera, Montana */
	{  30,   75, 2500, 2503, 4526,  1474 },	/* Powder River, Montana */
	{  30,   77, 2500, 2502, 4501,  1475 },	/* Powell, Montana */
	{  30,   79, 2500, 2502, 4501,  1479 },	/* Prairie, Montana */
	{  30,   81, 2500, 2503, 4526,  1518 },	/* Ravalli, Montana */
	{  30,   83, 2500, 2502, 4501,  1539 },	/* Richland, Montana */
	{  30,   85, 2500, 2501, 4476,  1571 },	/* Roosevelt, Montana */
	{  30,   87, 2500, 2503, 4526,  1575 },	/* Rosebud, Montana */
	{  30,   89, 2500, 2502, 4501,  1619 },	/* Sanders, Montana */
	{  30,   91, 2500, 2501, 4476,  1671 },	/* Sheridan, Montana */
	{  30,   93, 2500, 2503, 4526,  1677 },	/* Silver Bow, Montana */
	{  30,   95, 2500, 2503, 4526,  1747 },	/* Stillwater, Montana */
	{  30,   97, 2500, 2503, 4526,  1774 },	/* Sweet Grass, Montana */
	{  30,   99, 2500, 2501, 4476,  1801 },	/* Teton, Montana */
	{  30,  101, 2500, 2501, 4476,  1825 },	/* Toole, Montana */
	{  30,  103, 2500, 2503, 4526,  1834 },	/* Treasure, Montana */
	{  30,  105, 2500, 2501, 4476,  1878 },	/* Valley, Montana */
	{  30,  107, 2500, 2503, 4526,  1962 },	/* Wheatland, Montana */
	{  30,  109, 2500, 2502, 4501,  1970 },	/* Wibaux, Montana */
	{  30,  111, 2500, 2503, 4526,  2029 },	/* Yellowstone, Montana */
	{  30,  113, 2500, 2503, 4526,  2030 },	/* Yellowstone National Park, Montana */
	{  31,    0,    0,    0,    0,  1276 },	/* Nebraska, Nebraska */
	{  31,    1, 2600, 2600,    0,     5 },	/* Adams, Nebraska */
	{  31,    3, 2600, 2600,    0,    60 },	/* Antelope, Nebraska */
	{  31,    5, 2600, 2600,    0,    79 },	/* Arthur, Nebraska */
	{  31,    7, 2600, 2600,    0,   117 },	/* Banner, Nebraska */
	{  31,    9, 2600, 2600,    0,   185 },	/* Blaine, Nebraska */
	{  31,   11, 2600, 2600,    0,   201 },	/* Boone, Nebraska */
	{  31,   13, 2600, 2600,    0,   212 },	/* Box Butte, Nebraska */
	{  31,   15, 2600, 2600,    0,   214 },	/* Boyd, Nebraska */
	{  31,   17, 2600, 2600,    0,   240 },	/* Brown, Nebraska */
	{  31,   19, 2600, 2600,    0,   249 },	/* Buffalo, Nebraska */
	{  31,   21, 2600, 2600,    0,   261 },	/* Burt, Nebraska */
	{  31,   23, 2600, 2600,    0,   262 },	/* Butler, Nebraska */
	{  31,   25, 2600, 2600,    0,   310 },	/* Cass, Nebraska */
	{  31,   27, 2600, 2600,    0,   324 },	/* Cedar, Nebraska */
	{  31,   29, 2600, 2600,    0,   340 },	/* Chase, Nebraska */
	{  31,   31, 2600, 2600,    0,   352 },	/* Cherry, Nebraska */
	{  31,   33, 2600, 2600,    0,   357 },	/* Cheyenne, Nebraska */
	{  31,   35, 2600, 2600,    0,   385 },	/* Clay, Nebraska */
	{  31,   37, 2600, 2600,    0,   413 },	/* Colfax, Nebraska */
	{  31,   39, 2600, 2600,    0,   476 },	/* Cuming, Nebraska */
	{  31,   41, 2600, 2600,    0,   479 },	/* Custer, Nebraska */
	{  31,   43, 2600, 2600,    0,   483 },	/* Dakota, Nebraska */
	{  31,   45, 2600, 2600,    0,   499 },	/* Dawes, Nebraska */
	{  31,   47, 2600, 2600,    0,   500 },	/* Dawson, Nebraska */
	{  31,   49, 2600, 2600,    0,   520 },	/* Deuel, Nebraska */
	{  31,   51, 2600, 2600,    0,   534 },	/* Dixon, Nebraska */
	{  31,   53, 2600, 2600,    0,   536 },	/* Dodge, Nebraska */
	{  31,   55, 2600, 2600,    0,   546 },	/* Douglas, Nebraska */
	{  31,   57, 2600, 2600,    0,   553 },	/* Dundy, Nebraska */
	{  31,   59, 2600, 2600,    0,   627 },	/* Fillmore, Nebraska */
	{  31,   61, 2600, 2600,    0,   646 },	/* Franklin, Nebraska */
	{  31,   63, 2600, 2600,    0,   655 },	/* Frontier, Nebraska */
	{  31,   65, 2600, 2600,    0,   657 },	/* Furnas, Nebraska */
	{  31,   67, 2600, 2600,    0,   659 },	/* Gage, Nebraska */
	{  31,   69, 2600, 2600,    0,   665 },	/* Garden, Nebraska */
	{  31,   71, 2600, 2600,    0,   666 },	/* Garfield, Nebraska */
	{  31,   73, 2600, 2600,    0,   710 },	/* Gosper, Nebraska */
	{  31,   75, 2600, 2600,    0,   721 },	/* Grant, Nebraska */
	{  31,   77, 2600, 2600,    0,   728 },	/* Greeley, Nebraska */
	{  31,   79, 2600, 2600,    0,   763 },	/* Hall, Nebraska */
	{  31,   81, 2600, 2600,    0,   765 },	/* Hamilton, Nebraska */
	{  31,   83, 2600, 2600,    0,   783 },	/* Harlan, Nebraska */
	{  31,   85, 2600, 2600,    0,   800 },	/* Hayes, Nebraska */
	{  31,   87, 2600, 2600,    0,   826 },	/* Hitchcock, Nebraska */
	{  31,   89, 2600, 2600,    0,   832 },	/* Holt, Nebraska */
	{  31,   91, 2600, 2600,    0,   836 },	/* Hooker, Nebraska */
	{  31,   93, 2600, 2600,    0,   845 },	/* Howard, Nebraska */
	{  31,   95, 2600, 2600,    0,   901 },	/* Jefferson, Nebraska */
	{  31,   97, 2600, 2600,    0,   914 },	/* Johnson, Nebraska */
	{  31,   99, 2600, 2600,    0,   939 },	/* Kearney, Nebraska */
	{  31,  101, 2600, 2600,    0,   941 },	/* Keith, Nebraska */
	{  31,  103, 2600, 2600,    0,   958 },	/* Keya Paha, Nebraska */
	{  31,  105, 2600, 2600,    0,   961 },	/* Kimball, Nebraska */
	{  31,  107, 2600, 2600,    0,   982 },	/* Knox, Nebraska */
	{  31,  109, 2600, 2600,    0,  1013 },	/* Lancaster, Nebraska */
	{  31,  111, 2600, 2600,    0,  1060 },	/* Lincoln, Nebraska */
	{  31,  113, 2600, 2600,    0,  1068 },	/* Logan, Nebraska */
	{  31,  115, 2600, 2600,    0,  1079 },	/* Loup, Nebraska */
	{  31,  117, 2600, 2600,    0,  1167 },	/* Mc Pherson, Nebraska */
	{  31,  119, 2600, 2600,    0,  1101 },	/* Madison, Nebraska */
	{  31,  121, 2600, 2600,    0,  1188 },	/* Merrick, Nebraska */
	{  31,  123, 2600, 2600,    0,  1242 },	/* Morrill, Nebraska */
	{  31,  125, 2600, 2600,    0,  1264 },	/* Nance, Nebraska */
	{  31,  127, 2600, 2600,    0,  1278 },	/* Nemaha, Nebraska */
	{  31,  129, 2600, 2600,    0,  1331 },	/* Nuckolls, Nebraska */
	{  31,  131, 2600, 2600,    0,  1372 },	/* Otoe, Nebraska */
	{  31,  133, 2600, 2600,    0,  1405 },	/* Pawnee, Nebraska */
	{  31,  135, 2600, 2600,    0,  1423 },	/* Perkins, Nebraska */
	{  31,  137, 2600, 2600,    0,  1431 },	/* Phelps, Nebraska */
	{  31,  139, 2600, 2600,    0,  1438 },	/* Pierce, Nebraska */
	{  31,  141, 2600, 2600,    0,  1453 },	/* Platte, Nebraska */
	{  31,  143, 2600, 2600,    0,  1461 },	/* Polk, Nebraska */
	{  31,  145, 2600, 2600,    0,  1525 },	/* Red Willow, Nebraska */
	{  31,  147, 2600, 2600,    0,  1538 },	/* Richardson, Nebraska */
	{  31,  149, 2600, 2600,    0,  1557 },	/* Rock, Nebraska */
	{  31,  151, 2600, 2600,    0,  1598 },	/* Saline, Nebraska */
	{  31,  153, 2600, 2600,    0,  1634 },	/* Sarpy, Nebraska */
	{  31,  155, 2600, 2600,    0,  1636 },	/* Saunders, Nebraska */
	{  31,  157, 2600, 2600,    0,  1648 },	/* Scotts Bluff, Nebraska */
	{  31,  159, 2600, 2600,    0,  1659 },	/* Seward, Nebraska */
	{  31,  161, 2600, 2600,    0,  1671 },	/* Sheridan, Nebraska */
	{  31,  163, 2600, 2600,    0,  1672 },	/* Sherman, Nebraska */
	{  31,  165, 2600, 2600,    0,  1679 },	/* Sioux, Nebraska */
	{  31,  167, 2600, 2600,    0,  1734 },	/* Stanton, Nebraska */
	{  31,  169, 2600, 2600,    0,  1803 },	/* Thayer, Nebraska */
	{  31,  171, 2600, 2600,    0,  1804 },	/* Thomas, Nebraska */
	{  31,  173, 2600, 2600,    0,  1806 },	/* Thurston, Nebraska */
	{  31,  175, 2600, 2600,    0,  1878 },	/* Valley, Nebraska */
	{  31,  177, 2600, 2600,    0,  1933 },	/* Washington, Nebraska */
	{  31,  179, 2600, 2600,    0,  1942 },	/* Wayne, Nebraska */
	{  31,  181, 2600, 2600,    0,  1947 },	/* Webster, Nebraska */
	{  31,  183, 2600, 2600,    0,  1963 },	/* Wheeler, Nebraska */
	{  31,  185, 2600, 2600,    0,  2033 },	/* York, Nebraska */
	{  32,    0,    0,    0,    0,  1282 },	/* Nevada, Nevada */
	{  32,    1, 2703, 2703, 4651,   369 },	/* Churchill, Nevada */
	{  32,    3, 2701, 2701, 4601,   382 },	/* Clark, Nevada */
	{  32,    5, 2703, 2703, 4651,   546 },	/* Douglas, Nevada */
	{  32,    7, 2701, 2701, 4601,   586 },	/* Elko, Nevada */
	{  32,    9, 2703, 2703, 4651,   601 },	/* Esmeralda, Nevada */
	{  32,   11, 2701, 2701, 4601,   605 },	/* Eureka, Nevada */
	{  32,   13, 2703, 2703, 4651,   854 },	/* Humboldt, Nevada */
	{  32,   15, 2702, 2702, 4626,  1014 },	/* Lander, Nevada */
	{  32,   17, 2701, 2701, 4601,  1060 },	/* Lincoln, Nevada */
	{  32,   19, 2703, 2703, 4651,  1095 },	/* Lyon, Nevada */
	{  32,   21, 2703, 2703, 4651,  1206 },	/* Mineral, Nevada */
	{  32,   23, 2702, 2702, 4626,  1333 },	/* Nye, Nevada */
	{  32,   27, 2703, 2703, 4651,  1426 },	/* Pershing, Nevada */
	{  32,   29, 2703, 2703, 4651,  1752 },	/* Storey, Nevada */
	{  32,   31, 2703, 2703, 4651,  1935 },	/* Washoe, Nevada */
	{  32,   33, 2701, 2701, 4601,  1965 },	/* White Pine, Nevada */
	{  32,  510, 2703, 2703, 4651,   304 },	/* Carson City, Nevada */
	{  33,    0,    0,    0,    0,  1284 },	/* New Hampshire, New Hampshire */
	{  33,    1, 2800, 2800, 4676,   153 },	/* Belknap, New Hampshire */
	{  33,    3, 2800, 2800, 4676,   302 },	/* Carroll, New Hampshire */
	{  33,    5, 2800, 2800, 4676,   354 },	/* Cheshire, New Hampshire */
	{  33,    7, 2800, 2800, 4676,   439 },	/* Coos, New Hampshire */
	{  33,    9, 2800, 2800, 4676,   713 },	/* Grafton, New Hampshire */
	{  33,   11, 2800, 2800, 4676,   822 },	/* Hillsborough, New Hampshire */
	{  33,   13, 2800, 2800, 4676,  1189 },	/* Merrimack, New Hampshire */
	{  33,   15, 2800, 2800, 4676,  1562 },	/* Rockingham, New Hampshire */
	{  33,   17, 2800, 2800, 4676,  1754 },	/* Strafford, New Hampshire */
	{  33,   19, 2800, 2800, 4676,  1759 },	/* Sullivan, New Hampshire */
	{  34,    0,    0,    0,    0,  1287 },	/* New Jersey, New Jersey */
	{  34,    1, 2900, 2900, 4701,    91 },	/* Atlantic, New Jersey */
	{  34,    3, 2900, 2900, 4701,   165 },	/* Bergen, New Jersey */
	{  34,    5, 2900, 2900, 4701,   258 },	/* Burlington, New Jersey */
	{  34,    7, 2900, 2900, 4701,   284 },	/* Camden, New Jersey */
	{  34,    9, 2900, 2900, 4701,   295 },	/* Cape May, New Jersey */
	{  34,   11, 2900, 2900, 4701,   475 },	/* Cumberland, New Jersey */
	{  34,   13, 2900, 2900, 4701,   602 },	/* Essex, New Jersey */
	{  34,   15, 2900, 2900, 4701,   699 },	/* Gloucester, New Jersey */
	{  34,   17, 2900, 2900, 4701,   849 },	/* Hudson, New Jersey */
	{  34,   19, 2900, 2900, 4701,   857 },	/* Hunterdon, New Jersey */
	{  34,   21, 2900, 2900, 4701,  1186 },	/* Mercer, New Jersey */
	{  34,   23, 2900, 2900, 4701,  1194 },	/* Middlesex, New Jersey */
	{  34,   25, 2900, 2900, 4701,  1222 },	/* Monmouth, New Jersey */
	{  34,   27, 2900, 2900, 4701,  1243 },	/* Morris, New Jersey */
	{  34,   29, 2900, 2900, 4701,  1337 },	/* Ocean, New Jersey */
	{  34,   31, 2900, 2900, 4701,  1401 },	/* Passaic, New Jersey */
	{  34,   33, 2900, 2900, 4701,  1595 },	/* Salem, New Jersey */
	{  34,   35, 2900, 2900, 4701,  1692 },	/* Somerset, New Jersey */
	{  34,   37, 2900, 2900, 4701,  1768 },	/* Sussex, New Jersey */
	{  34,   39, 2900, 2900, 4701,  1867 },	/* Union, New Jersey */
	{  34,   41, 2900, 2900, 4701,  1925 },	/* Warren, New Jersey */
	{  35,    0,    0,    0,    0,  1291 },	/* New Mexico, New Mexico */
	{  35,    1, 3002, 3002, 4751,   169 },	/* Bernalillo, New Mexico */
	{  35,    3, 3003, 3003, 4776,   318 },	/* Catron, New Mexico */
	{  35,    5, 3001, 3001, 4726,   345 },	/* Chaves, New Mexico */
	{  35,    6, 3003, 3003, 4776,   372 },	/* Cibola, New Mexico */
	{  35,    7, 3001, 3001, 4726,   413 },	/* Colfax, New Mexico */
	{  35,    9, 3001, 3001, 4726,   478 },	/* Curry, New Mexico */
	{  35,   11, 3001, 3001, 4726,   502 },	/* De Baca, New Mexico */
	{  35,   13, 3002, 3002, 4751,   538 },	/* Dona Ana, New Mexico */
	{  35,   15, 3001, 3001, 4726,   573 },	/* Eddy, New Mexico */
	{  35,   17, 3003, 3003, 4776,   721 },	/* Grant, New Mexico */
	{  35,   19, 3001, 3001, 4726,   745 },	/* Guadalupe, New Mexico */
	{  35,   21, 3001, 3001, 4726,   780 },	/* Harding, New Mexico */
	{  35,   23, 3003, 3003, 4776,   818 },	/* Hidalgo, New Mexico */
	{  35,   25, 3001, 3001, 4726,  1036 },	/* Lea, New Mexico */
	{  35,   27, 3002, 3002, 4751,  1060 },	/* Lincoln, New Mexico */
	{  35,   29, 3002, 3002, 4751,  1073 },	/* Los Alamos, New Mexico */
	{  35,   31, 3003, 3003, 4776,  1087 },	/* Luna, New Mexico */
	{  35,   33, 3003, 3003, 4776,  1169 },	/* McKinley, New Mexico */
	{  35,   35, 3001, 3001, 4726,  1238 },	/* Mora, New Mexico */
	{  35,   37, 3002, 3002, 4751,  1371 },	/* Otero, New Mexico */
	{  35,   39, 3001, 3001, 4726,  1500 },	/* Quay, New Mexico */
	{  35,   41, 3002, 3002, 4751,  1545 },	/* Rio Arriba, New Mexico */
	{  35,   43, 3001, 3001, 4726,  1571 },	/* Roosevelt, New Mexico */
	{  35,   45, 3002, 3002, 4751,  1620 },	/* Sandoval, New Mexico */
	{  35,   47, 3003, 3003, 4776,  1610 },	/* San Juan, New Mexico */
	{  35,   49, 3001, 3001, 4726,  1614 },	/* San Miguel, New Mexico */
	{  35,   51, 3002, 3002, 4751,  1628 },	/* Santa Fe, New Mexico */
	{  35,   53, 3003, 3003, 4776,  1676 },	/* Sierra, New Mexico */
	{  35,   55, 3002, 3002, 4751,  1690 },	/* Socorro, New Mexico */
	{  35,   57, 3002, 3002, 4751,  1787 },	/* Taos, New Mexico */
	{  35,   59, 3002, 3002, 4751,  1827 },	/* Torrance, New Mexico */
	{  35,   61, 3001, 3001, 4726,  1867 },	/* Union, New Mexico */
	{  35,   63, 3002, 3002, 4751,  1877 },	/* Valencia, New Mexico */
	{  36,    0, 3101, 3101, 4801,  1292 },	/* New York, New York */
	{  36,    1, 3101, 3101, 4801,    25 },	/* Albany, New York */
	{  36,    3, 3103, 3103, 4851,    37 },	/* Allegany, New York */
	{  36,    5, 3101, 3101, 4801,   234 },	/* Bronx, New York */
	{  36,    7, 3101, 3101, 4801,   238 },	/* Broome, New York */
	{  36,    9, 3103, 3103, 4851,   319 },	/* Cattaraugus, New York */
	{  36,   11, 3102, 3102, 4826,   322 },	/* Cayuga, New York */
	{  36,   13, 3103, 3103, 4851,   344 },	/* Chautauqua, New York */
	{  36,   15, 3102, 3102, 4826,   349 },	/* Chemung, New York */
	{  36,   17, 3102, 3102, 4826,   350 },	/* Chenango, New York */
	{  36,   19, 3101, 3101, 4801,   395 },	/* Clinton, New York */
	{  36,   21, 3101, 3101, 4801,   421 },	/* Columbia, New York */
	{  36,   23, 3102, 3102, 4826,   444 },	/* Cortland, New York */
	{  36,   25, 3101, 3101, 4801,   512 },	/* Delaware, New York */
	{  36,   27, 3101, 3101, 4801,   558 },	/* Dutchess, New York */
	{  36,   29, 3103, 3103, 4851,   598 },	/* Erie, New York */
	{  36,   31, 3101, 3101, 4801,   602 },	/* Essex, New York */
	{  36,   33, 3101, 3101, 4801,   646 },	/* Franklin, New York */
	{  36,   35, 3101, 3101, 4801,   656 },	/* Fulton, New York */
	{  36,   37, 3103, 3103, 4851,   679 },	/* Genesee, New York */
	{  36,   39, 3101, 3101, 4801,   732 },	/* Greene, New York */
	{  36,   41, 3101, 3101, 4801,   765 },	/* Hamilton, New York */
	{  36,   43, 3101, 3101, 4801,   812 },	/* Herkimer, New York */
	{  36,   45, 3102, 3102, 4826,   901 },	/* Jefferson, New York */
	{  36,   47, 3104, 3104, 4976,   970 },	/* Kings, New York */
	{  36,   49, 3102, 3102, 4826,  1051 },	/* Lewis, New York */
	{  36,   51, 3103, 3103, 4851,  1066 },	/* Livingston, New York */
	{  36,   53, 3102, 3102, 4826,  1101 },	/* Madison, New York */
	{  36,   55, 3103, 3103, 4851,  1226 },	/* Monroe, New York */
	{  36,   57, 3101, 3101, 4801,  1232 },	/* Montgomery, New York */
	{  36,   59, 3104, 3104, 4976,  1270 },	/* Nassau, New York */
	{  36,   61,    0,    0,    0,  1292 },	/* New York, New York */
	{  36,   63, 3103, 3103, 4851,  1306 },	/* Niagara, New York */
	{  36,   65, 3102, 3102, 4826,  1356 },	/* Oneida, New York */
	{  36,   67, 3102, 3102, 4826,  1357 },	/* Onondaga, New York */
	{  36,   69, 3102, 3102, 4826,  1359 },	/* Ontario, New York */
	{  36,   71, 3101, 3101, 4801,  1361 },	/* Orange, New York */
	{  36,   73, 3103, 3103, 4851,  1364 },	/* Orleans, New York */
	{  36,   75, 3102, 3102, 4826,  1370 },	/* Oswego, New York */
	{  36,   77, 3101, 3101, 4801,  1373 },	/* Otsego, New York */
	{  36,   79, 3101, 3101, 4801,  1499 },	/* Putnam, New York */
	{  36,   81, 3104, 3104, 4976,  1503 },	/* Queens, New York */
	{  36,   83, 3101, 3101, 4801,  1530 },	/* Rensselaer, New York */
	{  36,   85, 3104, 3104, 4976,  1540 },	/* Richmond, New York */
	{  36,   87, 3101, 3101, 4801,  1563 },	/* Rockland, New York */
	{  36,   89, 3101, 3101, 4801,  1721 },	/* St Lawrence, New York */
	{  36,   91, 3101, 3101, 4801,  1632 },	/* Saratoga, New York */
	{  36,   93, 3101, 3101, 4801,  1638 },	/* Schenectady, New York */
	{  36,   95, 3101, 3101, 4801,  1641 },	/* Schoharie, New York */
	{  36,   97, 3102, 3102, 4826,  1643 },	/* Schuyler, New York */
	{  36,   99, 3102, 3102, 4826,  1655 },	/* Seneca, New York */
	{  36,  101, 3102, 3102, 4826,  1744 },	/* Steuben, New York */
	{  36,  103, 3104, 3104, 4976,  1757 },	/* Suffolk, New York */
	{  36,  105, 3101, 3101, 4801,  1759 },	/* Sullivan, New York */
	{  36,  107, 3102, 3102, 4826,  1811 },	/* Tioga, New York */
	{  36,  109, 3102, 3102, 4826,  1823 },	/* Tompkins, New York */
	{  36,  111, 3101, 3101, 4801,  1864 },	/* Ulster, New York */
	{  36,  113, 3101, 3101, 4801,  1925 },	/* Warren, New York */
	{  36,  115, 3101, 3101, 4801,  1933 },	/* Washington, New York */
	{  36,  117, 3102, 3102, 4826,  1942 },	/* Wayne, New York */
	{  36,  119, 3101, 3101, 4801,  1954 },	/* Westchester, New York */
	{  36,  121, 3103, 3103, 4851,  2013 },	/* Wyoming, New York */
	{  36,  123, 3102, 3102, 4826,  2023 },	/* Yates, New York */
	{  37,    0,    0,    0,    0,  1318 },	/* North Carolina, North Carolina */
	{  37,    1, 3200, 3200, 4901,    21 },	/* Alamance, North Carolina */
	{  37,    3, 3200, 3200, 4901,    31 },	/* Alexander, North Carolina */
	{  37,    5, 3200, 3200, 4901,    38 },	/* Alleghany, North Carolina */
	{  37,    7, 3200, 3200, 4901,    59 },	/* Anson, North Carolina */
	{  37,    9, 3200, 3200, 4901,    81 },	/* Ashe, North Carolina */
	{  37,   11, 3200, 3200, 4901,   102 },	/* Avery, North Carolina */
	{  37,   13, 3200, 3200, 4901,   144 },	/* Beaufort, North Carolina */
	{  37,   15, 3200, 3200, 4901,   171 },	/* Bertie, North Carolina */
	{  37,   17, 3200, 3200, 4901,   184 },	/* Bladen, North Carolina */
	{  37,   19, 3200, 3200, 4901,   242 },	/* Brunswick, North Carolina */
	{  37,   21, 3200, 3200, 4901,   253 },	/* Buncombe, North Carolina */
	{  37,   23, 3200, 3200, 4901,   255 },	/* Burke, North Carolina */
	{  37,   25, 3200, 3200, 4901,   265 },	/* Cabarrus, North Carolina */
	{  37,   27, 3200, 3200, 4901,   273 },	/* Caldwell, North Carolina */
	{  37,   29, 3200, 3200, 4901,   284 },	/* Camden, North Carolina */
	{  37,   31, 3200, 3200, 4901,   306 },	/* Carteret, North Carolina */
	{  37,   33, 3200, 3200, 4901,   313 },	/* Caswell, North Carolina */
	{  37,   35, 3200, 3200, 4901,   316 },	/* Catawba, North Carolina */
	{  37,   37, 3200, 3200, 4901,   341 },	/* Chatham, North Carolina */
	{  37,   39, 3200, 3200, 4901,   351 },	/* Cherokee, North Carolina */
	{  37,   41, 3200, 3200, 4901,   367 },	/* Chowan, North Carolina */
	{  37,   43, 3200, 3200, 4901,   385 },	/* Clay, North Carolina */
	{  37,   45, 3200, 3200, 4901,   392 },	/* Cleveland, North Carolina */
	{  37,   47, 3200, 3200, 4901,   423 },	/* Columbus, North Carolina */
	{  37,   49, 3200, 3200, 4901,   459 },	/* Craven, North Carolina */
	{  37,   51, 3200, 3200, 4901,   475 },	/* Cumberland, North Carolina */
	{  37,   53, 3200, 3200, 4901,   477 },	/* Currituck, North Carolina */
	{  37,   55, 3200, 3200, 4901,   490 },	/* Dare, North Carolina */
	{  37,   57, 3200, 3200, 4901,   494 },	/* Davidson, North Carolina */
	{  37,   59, 3200, 3200, 4901,   495 },	/* Davie, North Carolina */
	{  37,   61, 3200, 3200, 4901,   556 },	/* Duplin, North Carolina */
	{  37,   63, 3200, 3200, 4901,   557 },	/* Durham, North Carolina */
	{  37,   65, 3200, 3200, 4901,   575 },	/* Edgecombe, North Carolina */
	{  37,   67, 3200, 3200, 4901,   642 },	/* Forsyth, North Carolina */
	{  37,   69, 3200, 3200, 4901,   646 },	/* Franklin, North Carolina */
	{  37,   71, 3200, 3200, 4901,   673 },	/* Gaston, North Carolina */
	{  37,   73, 3200, 3200, 4901,   674 },	/* Gates, North Carolina */
	{  37,   75, 3200, 3200, 4901,   714 },	/* Graham, North Carolina */
	{  37,   77, 3200, 3200, 4901,   722 },	/* Granville, North Carolina */
	{  37,   79, 3200, 3200, 4901,   732 },	/* Greene, North Carolina */
	{  37,   81, 3200, 3200, 4901,   752 },	/* Guilford, North Carolina */
	{  37,   83, 3200, 3200, 4901,   762 },	/* Halifax, North Carolina */
	{  37,   85, 3200, 3200, 4901,   785 },	/* Harnett, North Carolina */
	{  37,   87, 3200, 3200, 4901,   802 },	/* Haywood, North Carolina */
	{  37,   89, 3200, 3200, 4901,   806 },	/* Henderson, North Carolina */
	{  37,   91, 3200, 3200, 4901,   814 },	/* Hertford, North Carolina */
	{  37,   93, 3200, 3200, 4901,   830 },	/* Hoke, North Carolina */
	{  37,   95, 3200, 3200, 4901,   862 },	/* Hyde, North Carolina */
	{  37,   97, 3200, 3200, 4901,   877 },	/* Iredell, North Carolina */
	{  37,   99, 3200, 3200, 4901,   893 },	/* Jackson, North Carolina */
	{  37,  101, 3200, 3200, 4901,   915 },	/* Johnston, North Carolina */
	{  37,  103, 3200, 3200, 4901,   917 },	/* Jones, North Carolina */
	{  37,  105, 3200, 3200, 4901,  1040 },	/* Lee, North Carolina */
	{  37,  107, 3200, 3200, 4901,  1046 },	/* Lenoir, North Carolina */
	{  37,  109, 3200, 3200, 4901,  1060 },	/* Lincoln, North Carolina */
	{  37,  111, 3200, 3200, 4901,  1155 },	/* Mc Dowell, North Carolina */
	{  37,  113, 3200, 3200, 4901,  1098 },	/* Macon, North Carolina */
	{  37,  115, 3200, 3200, 4901,  1101 },	/* Madison, North Carolina */
	{  37,  117, 3200, 3200, 4901,  1131 },	/* Martin, North Carolina */
	{  37,  119, 3200, 3200, 4901,  1173 },	/* Mecklenburg, North Carolina */
	{  37,  121, 3200, 3200, 4901,  1215 },	/* Mitchell, North Carolina */
	{  37,  123, 3200, 3200, 4901,  1232 },	/* Montgomery, North Carolina */
	{  37,  125, 3200, 3200, 4901,  1237 },	/* Moore, North Carolina */
	{  37,  127, 3200, 3200, 4901,  1269 },	/* Nash, North Carolina */
	{  37,  129, 3200, 3200, 4901,  1285 },	/* New Hanover, North Carolina */
	{  37,  131, 3200, 3200, 4901,  1321 },	/* Northampton, North Carolina */
	{  37,  133, 3200, 3200, 4901,  1358 },	/* Onslow, North Carolina */
	{  37,  135, 3200, 3200, 4901,  1361 },	/* Orange, North Carolina */
	{  37,  137, 3200, 3200, 4901,  1393 },	/* Pamlico, North Carolina */
	{  37,  139, 3200, 3200, 4901,  1400 },	/* Pasquotank, North Carolina */
	{  37,  141, 3200, 3200, 4901,  1415 },	/* Pender, North Carolina */
	{  37,  143, 3200, 3200, 4901,  1424 },	/* Perquimans, North Carolina */
	{  37,  145, 3200, 3200, 4901,  1427 },	/* Person, North Carolina */
	{  37,  147, 3200, 3200, 4901,  1447 },	/* Pitt, North Carolina */
	{  37,  149, 3200, 3200, 4901,  1461 },	/* Polk, North Carolina */
	{  37,  151, 3200, 3200, 4901,  1513 },	/* Randolph, North Carolina */
	{  37,  153, 3200, 3200, 4901,  1540 },	/* Richmond, North Carolina */
	{  37,  155, 3200, 3200, 4901,  1556 },	/* Robeson, North Carolina */
	{  37,  157, 3200, 3200, 4901,  1562 },	/* Rockingham, North Carolina */
	{  37,  159, 3200, 3200, 4901,  1579 },	/* Rowan, North Carolina */
	{  37,  161, 3200, 3200, 4901,  1584 },	/* Rutherford, North Carolina */
	{  37,  163, 3200, 3200, 4901,  1601 },	/* Sampson, North Carolina */
	{  37,  165, 3200, 3200, 4901,  1646 },	/* Scotland, North Carolina */
	{  37,  167, 3200, 3200, 4901,  1733 },	/* Stanly, North Carolina */
	{  37,  169, 3200, 3200, 4901,  1749 },	/* Stokes, North Carolina */
	{  37,  171, 3200, 3200, 4901,  1766 },	/* Surry, North Carolina */
	{  37,  173, 3200, 3200, 4901,  1772 },	/* Swain, North Carolina */
	{  37,  175, 3200, 3200, 4901,  1831 },	/* Transylvania, North Carolina */
	{  37,  177, 3200, 3200, 4901,  1858 },	/* Tyrrell, North Carolina */
	{  37,  179, 3200, 3200, 4901,  1867 },	/* Union, North Carolina */
	{  37,  181, 3200, 3200, 4901,  1882 },	/* Vance, North Carolina */
	{  37,  183, 3200, 3200, 4901,  1909 },	/* Wake, North Carolina */
	{  37,  185, 3200, 3200, 4901,  1925 },	/* Warren, North Carolina */
	{  37,  187, 3200, 3200, 4901,  1933 },	/* Washington, North Carolina */
	{  37,  189, 3200, 3200, 4901,  1937 },	/* Watauga, North Carolina */
	{  37,  191, 3200, 3200, 4901,  1942 },	/* Wayne, North Carolina */
	{  37,  193, 3200, 3200, 4901,  1975 },	/* Wilkes, North Carolina */
	{  37,  195, 3200, 3200, 4901,  1984 },	/* Wilson, North Carolina */
	{  37,  197, 3200, 3200, 4901,  2016 },	/* Yadkin, North Carolina */
	{  37,  199, 3200, 3200, 4901,  2020 },	/* Yancey, North Carolina */
	{  38,    0,    0,    0,    0,  1319 },	/* North Dakota, North Dakota */
	{  38,    1, 3302, 3302, 4951,     5 },	/* Adams, North Dakota */
	{  38,    3, 3302, 3302, 4951,   123 },	/* Barnes, North Dakota */
	{  38,    5, 3301, 3301, 4926,   161 },	/* Benson, North Dakota */
	{  38,    7, 3302, 3302, 4951,   180 },	/* Billings, North Dakota */
	{  38,    9, 3301, 3301, 4926,   206 },	/* Bottineau, North Dakota */
	{  38,   11, 3302, 3302, 4951,   211 },	/* Bowman, North Dakota */
	{  38,   13, 3301, 3301, 4926,   255 },	/* Burke, North Dakota */
	{  38,   15, 3302, 3302, 4951,   256 },	/* Burleigh, North Dakota */
	{  38,   17, 3302, 3302, 4951,   310 },	/* Cass, North Dakota */
	{  38,   19, 3301, 3301, 4926,   320 },	/* Cavalier, North Dakota */
	{  38,   21, 3302, 3302, 4951,   524 },	/* Dickey, North Dakota */
	{  38,   23, 3301, 3301, 4926,   532 },	/* Divide, North Dakota */
	{  38,   25, 3302, 3302, 4951,   555 },	/* Dunn, North Dakota */
	{  38,   27, 3301, 3301, 4926,   573 },	/* Eddy, North Dakota */
	{  38,   29, 3302, 3302, 4951,   594 },	/* Emmons, North Dakota */
	{  38,   31, 3301, 3301, 4926,   644 },	/* Foster, North Dakota */
	{  38,   33, 3302, 3302, 4951,   702 },	/* Golden Valley, North Dakota */
	{  38,   35, 3301, 3301, 4926,   717 },	/* Grand Forks, North Dakota */
	{  38,   37, 3302, 3302, 4951,   721 },	/* Grant, North Dakota */
	{  38,   39, 3301, 3301, 4926,   742 },	/* Griggs, North Dakota */
	{  38,   41, 3302, 3302, 4951,   815 },	/* Hettinger, North Dakota */
	{  38,   43, 3302, 3302, 4951,   959 },	/* Kidder, North Dakota */
	{  38,   45, 3302, 3302, 4951,   992 },	/* La Moure, North Dakota */
	{  38,   47, 3302, 3302, 4951,  1068 },	/* Logan, North Dakota */
	{  38,   49, 3301, 3301, 4926,  1157 },	/* Mc Henry, North Dakota */
	{  38,   51, 3302, 3302, 4951,  1158 },	/* Mc Intosh, North Dakota */
	{  38,   53, 3301, 3301, 4926,  1160 },	/* Mc Kenzie, North Dakota */
	{  38,   55, 3301, 3301, 4926,  1161 },	/* Mc Lean, North Dakota */
	{  38,   57, 3302, 3302, 4951,  1186 },	/* Mercer, North Dakota */
	{  38,   59, 3302, 3302, 4951,  1246 },	/* Morton, North Dakota */
	{  38,   61, 3301, 3301, 4926,  1249 },	/* Mountrail, North Dakota */
	{  38,   63, 3301, 3301, 4926,  1277 },	/* Nelson, North Dakota */
	{  38,   65, 3302, 3302, 4951,  1354 },	/* Oliver, North Dakota */
	{  38,   67, 3301, 3301, 4926,  1412 },	/* Pembina, North Dakota */
	{  38,   69, 3301, 3301, 4926,  1438 },	/* Pierce, North Dakota */
	{  38,   71, 3301, 3301, 4926,  1511 },	/* Ramsey, North Dakota */
	{  38,   73, 3302, 3302, 4951,  1515 },	/* Ransom, North Dakota */
	{  38,   75, 3301, 3301, 4926,  1531 },	/* Renville, North Dakota */
	{  38,   77, 3302, 3302, 4951,  1539 },	/* Richland, North Dakota */
	{  38,   79, 3301, 3301, 4926,  1567 },	/* Rolette, North Dakota */
	{  38,   81, 3302, 3302, 4951,  1633 },	/* Sargent, North Dakota */
	{  38,   83, 3301, 3301, 4926,  1671 },	/* Sheridan, North Dakota */
	{  38,   85, 3302, 3302, 4951,  1679 },	/* Sioux, North Dakota */
	{  38,   87, 3302, 3302, 4951,  1685 },	/* Slope, North Dakota */
	{  38,   89, 3302, 3302, 4951,  1735 },	/* Stark, North Dakota */
	{  38,   91, 3301, 3301, 4926,  1740 },	/* Steele, North Dakota */
	{  38,   93, 3302, 3302, 4951,  1755 },	/* Stutsman, North Dakota */
	{  38,   95, 3301, 3301, 4926,  1828 },	/* Towner, North Dakota */
	{  38,   97, 3301, 3301, 4926,  1830 },	/* Traill, North Dakota */
	{  38,   99, 3301, 3301, 4926,  1918 },	/* Walsh, North Dakota */
	{  38,  101, 3301, 3301, 4926,  1923 },	/* Ward, North Dakota */
	{  38,  103, 3301, 3301, 4926,  1949 },	/* Wells, North Dakota */
	{  38,  105, 3301, 3301, 4926,  1980 },	/* Williams, North Dakota */
	{  39,    0,    0,    0,    0,  1345 },	/* Ohio, Ohio */
	{  39,    1, 3402, 3402, 5001,     5 },	/* Adams, Ohio */
	{  39,    3, 3401, 3401, 4976,    40 },	/* Allen, Ohio */
	{  39,    5, 3401, 3401, 4976,    82 },	/* Ashland, Ohio */
	{  39,    7, 3401, 3401, 4976,    84 },	/* Ashtabula, Ohio */
	{  39,    9, 3402, 3402, 5001,    89 },	/* Athens, Ohio */
	{  39,   11, 3401, 3401, 4976,    96 },	/* Auglaize, Ohio */
	{  39,   13, 3402, 3402, 5001,   155 },	/* Belmont, Ohio */
	{  39,   15, 3402, 3402, 5001,   240 },	/* Brown, Ohio */
	{  39,   17, 3402, 3402, 5001,   262 },	/* Butler, Ohio */
	{  39,   19, 3401, 3401, 4976,   302 },	/* Carroll, Ohio */
	{  39,   21, 3402, 3402, 5001,   330 },	/* Champaign, Ohio */
	{  39,   23, 3402, 3402, 5001,   382 },	/* Clark, Ohio */
	{  39,   25, 3402, 3402, 5001,   391 },	/* Clermont, Ohio */
	{  39,   27, 3402, 3402, 5001,   395 },	/* Clinton, Ohio */
	{  39,   29, 3401, 3401, 4976,   422 },	/* Columbiana, Ohio */
	{  39,   31, 3401, 3401, 4976,   446 },	/* Coshocton, Ohio */
	{  39,   33, 3401, 3401, 4976,   460 },	/* Crawford, Ohio */
	{  39,   35, 3401, 3401, 4976,   480 },	/* Cuyahoga, Ohio */
	{  39,   37, 3402, 3402, 5001,   491 },	/* Darke, Ohio */
	{  39,   39, 3401, 3401, 4976,   510 },	/* Defiance, Ohio */
	{  39,   41, 3401, 3401, 4976,   512 },	/* Delaware, Ohio */
	{  39,   43, 3401, 3401, 4976,   598 },	/* Erie, Ohio */
	{  39,   45, 3402, 3402, 5001,   611 },	/* Fairfield, Ohio */
	{  39,   47, 3402, 3402, 5001,   622 },	/* Fayette, Ohio */
	{  39,   49, 3402, 3402, 5001,   646 },	/* Franklin, Ohio */
	{  39,   51, 3401, 3401, 4976,   656 },	/* Fulton, Ohio */
	{  39,   53, 3402, 3402, 5001,   663 },	/* Gallia, Ohio */
	{  39,   55, 3401, 3401, 4976,   677 },	/* Geauga, Ohio */
	{  39,   57, 3402, 3402, 5001,   732 },	/* Greene, Ohio */
	{  39,   59, 3402, 3402, 5001,   751 },	/* Guernsey, Ohio */
	{  39,   61, 3402, 3402, 5001,   765 },	/* Hamilton, Ohio */
	{  39,   63, 3401, 3401, 4976,   771 },	/* Hancock, Ohio */
	{  39,   65, 3401, 3401, 4976,   779 },	/* Hardin, Ohio */
	{  39,   67, 3401, 3401, 4976,   789 },	/* Harrison, Ohio */
	{  39,   69, 3401, 3401, 4976,   811 },	/* Henry, Ohio */
	{  39,   71, 3402, 3402, 5001,   819 },	/* Highland, Ohio */
	{  39,   73, 3402, 3402, 5001,   827 },	/* Hocking, Ohio */
	{  39,   75, 3401, 3401, 4976,   831 },	/* Holmes, Ohio */
	{  39,   77, 3401, 3401, 4976,   860 },	/* Huron, Ohio */
	{  39,   79, 3402, 3402, 5001,   893 },	/* Jackson, Ohio */
	{  39,   81, 3401, 3401, 4976,   901 },	/* Jefferson, Ohio */
	{  39,   83, 3401, 3401, 4976,   982 },	/* Knox, Ohio */
	{  39,   85, 3401, 3401, 4976,  1006 },	/* Lake, Ohio */
	{  39,   87, 3402, 3402, 5001,  1033 },	/* Lawrence, Ohio */
	{  39,   89, 3402, 3402, 5001,  1057 },	/* Licking, Ohio */
	{  39,   91, 3401, 3401, 4976,  1068 },	/* Logan, Ohio */
	{  39,   93, 3401, 3401, 4976,  1072 },	/* Lorain, Ohio */
	{  39,   95, 3401, 3401, 4976,  1084 },	/* Lucas, Ohio */
	{  39,   97, 3402, 3402, 5001,  1101 },	/* Madison, Ohio */
	{  39,   99, 3401, 3401, 4976,  1105 },	/* Mahoning, Ohio */
	{  39,  101, 3401, 3401, 4976,  1125 },	/* Marion, Ohio */
	{  39,  103, 3401, 3401, 4976,  1175 },	/* Medina, Ohio */
	{  39,  105, 3402, 3402, 5001,  1177 },	/* Meigs, Ohio */
	{  39,  107, 3401, 3401, 4976,  1186 },	/* Mercer, Ohio */
	{  39,  109, 3402, 3402, 5001,  1192 },	/* Miami, Ohio */
	{  39,  111, 3402, 3402, 5001,  1226 },	/* Monroe, Ohio */
	{  39,  113, 3402, 3402, 5001,  1232 },	/* Montgomery, Ohio */
	{  39,  115, 3402, 3402, 5001,  1240 },	/* Morgan, Ohio */
	{  39,  117, 3401, 3401, 4976,  1245 },	/* Morrow, Ohio */
	{  39,  119, 3402, 3402, 5001,  1257 },	/* Muskingum, Ohio */
	{  39,  121, 3402, 3402, 5001,  1310 },	/* Noble, Ohio */
	{  39,  123, 3401, 3401, 4976,  1374 },	/* Ottawa, Ohio */
	{  39,  125, 3401, 3401, 4976,  1404 },	/* Paulding, Ohio */
	{  39,  127, 3402, 3402, 5001,  1425 },	/* Perry, Ohio */
	{  39,  129, 3402, 3402, 5001,  1435 },	/* Pickaway, Ohio */
	{  39,  131, 3402, 3402, 5001,  1439 },	/* Pike, Ohio */
	{  39,  133, 3401, 3401, 4976,  1467 },	/* Portage, Ohio */
	{  39,  135, 3402, 3402, 5001,  1481 },	/* Preble, Ohio */
	{  39,  137, 3401, 3401, 4976,  1499 },	/* Putnam, Ohio */
	{  39,  139, 3401, 3401, 4976,  1539 },	/* Richland, Ohio */
	{  39,  141, 3402, 3402, 5001,  1576 },	/* Ross, Ohio */
	{  39,  143, 3401, 3401, 4976,  1621 },	/* Sandusky, Ohio */
	{  39,  145, 3402, 3402, 5001,  1645 },	/* Scioto, Ohio */
	{  39,  147, 3401, 3401, 4976,  1655 },	/* Seneca, Ohio */
	{  39,  149, 3401, 3401, 4976,  1668 },	/* Shelby, Ohio */
	{  39,  151, 3401, 3401, 4976,  1735 },	/* Stark, Ohio */
	{  39,  153, 3401, 3401, 4976,  1586 },	/* SUmmit, Ohio */
	{  39,  155, 3401, 3401, 4976,  1845 },	/* Trumbull, Ohio */
	{  39,  157, 3401, 3401, 4976,  1853 },	/* Tuscarawas, Ohio */
	{  39,  159, 3401, 3401, 4976,  1867 },	/* Union, Ohio */
	{  39,  161, 3401, 3401, 4976,  1880 },	/* Van Wert, Ohio */
	{  39,  163, 3402, 3402, 5001,  1897 },	/* Vinton, Ohio */
	{  39,  165, 3402, 3402, 5001,  1925 },	/* Warren, Ohio */
	{  39,  167, 3402, 3402, 5001,  1933 },	/* Washington, Ohio */
	{  39,  169, 3401, 3401, 4976,  1942 },	/* Wayne, Ohio */
	{  39,  171, 3401, 3401, 4976,  1980 },	/* Williams, Ohio */
	{  39,  173, 3401, 3401, 4976,  1998 },	/* Wood, Ohio */
	{  39,  175, 3401, 3401, 4976,  2011 },	/* Wyandot, Ohio */
	{  40,    0, 3501, 3501, 5026,  1350 },	/* Oklahoma, Oklahoma */
	{  40,    1, 3501, 3501, 5026,     4 },	/* Adair, Oklahoma */
	{  40,    3, 3501, 3501, 5026,    33 },	/* Alfalfa, Oklahoma */
	{  40,    5, 3502, 3502, 5051,    92 },	/* Atoka, Oklahoma */
	{  40,    7, 3501, 3501, 5026,   146 },	/* Beaver, Oklahoma */
	{  40,    9, 3502, 3502, 5051,   149 },	/* Beckham, Oklahoma */
	{  40,   11, 3501, 3501, 5026,   185 },	/* Blaine, Oklahoma */
	{  40,   13, 3502, 3502, 5051,   243 },	/* Bryan, Oklahoma */
	{  40,   15, 3502, 3502, 5051,   269 },	/* Caddo, Oklahoma */
	{  40,   17, 3501, 3501, 5026,   289 },	/* Canadian, Oklahoma */
	{  40,   19, 3502, 3502, 5051,   305 },	/* Carter, Oklahoma */
	{  40,   21, 3501, 3501, 5026,   351 },	/* Cherokee, Oklahoma */
	{  40,   23, 3502, 3502, 5051,   365 },	/* Choctaw, Oklahoma */
	{  40,   25, 3501, 3501, 5026,   374 },	/* Cimarron, Oklahoma */
	{  40,   27, 3502, 3502, 5051,   392 },	/* Cleveland, Oklahoma */
	{  40,   29, 3502, 3502, 5051,   398 },	/* Coal, Oklahoma */
	{  40,   31, 3502, 3502, 5051,   426 },	/* Comanche, Oklahoma */
	{  40,   33, 3502, 3502, 5051,   449 },	/* Cotton, Oklahoma */
	{  40,   35, 3501, 3501, 5026,   456 },	/* Craig, Oklahoma */
	{  40,   37, 3501, 3501, 5026,   461 },	/* Creek, Oklahoma */
	{  40,   39, 3501, 3501, 5026,   479 },	/* Custer, Oklahoma */
	{  40,   41, 3501, 3501, 5026,   512 },	/* Delaware, Oklahoma */
	{  40,   43, 3501, 3501, 5026,   521 },	/* Dewey, Oklahoma */
	{  40,   45, 3501, 3501, 5026,   588 },	/* Ellis, Oklahoma */
	{  40,   47, 3501, 3501, 5026,   666 },	/* Garfield, Oklahoma */
	{  40,   49, 3502, 3502, 5051,   670 },	/* Garvin, Oklahoma */
	{  40,   51, 3502, 3502, 5051,   712 },	/* Grady, Oklahoma */
	{  40,   53, 3501, 3501, 5026,   721 },	/* Grant, Oklahoma */
	{  40,   55, 3502, 3502, 5051,   738 },	/* Greer, Oklahoma */
	{  40,   57, 3502, 3502, 5051,   784 },	/* Harmon, Oklahoma */
	{  40,   59, 3501, 3501, 5026,   787 },	/* Harper, Oklahoma */
	{  40,   61, 3502, 3502, 5051,   795 },	/* Haskell, Oklahoma */
	{  40,   63, 3502, 3502, 5051,   852 },	/* Hughes, Oklahoma */
	{  40,   65, 3502, 3502, 5051,   893 },	/* Jackson, Oklahoma */
	{  40,   67, 3502, 3502, 5051,   901 },	/* Jefferson, Oklahoma */
	{  40,   69, 3502, 3502, 5051,   915 },	/* Johnston, Oklahoma */
	{  40,   71, 3501, 3501, 5026,   937 },	/* Kay, Oklahoma */
	{  40,   73, 3501, 3501, 5026,   967 },	/* Kingfisher, Oklahoma */
	{  40,   75, 3502, 3502, 5051,   973 },	/* Kiowa, Oklahoma */
	{  40,   77, 3502, 3502, 5051,  1028 },	/* Latimer, Oklahoma */
	{  40,   79, 3502, 3502, 5051,  1034 },	/* Le Flore, Oklahoma */
	{  40,   81, 3501, 3501, 5026,  1060 },	/* Lincoln, Oklahoma */
	{  40,   83, 3501, 3501, 5026,  1068 },	/* Logan, Oklahoma */
	{  40,   85, 3502, 3502, 5051,  1080 },	/* Love, Oklahoma */
	{  40,   87, 3502, 3502, 5051,  1146 },	/* Mc Clain, Oklahoma */
	{  40,   89, 3502, 3502, 5051,  1152 },	/* Mc Curtain, Oklahoma */
	{  40,   91, 3502, 3502, 5051,  1158 },	/* Mc Intosh, Oklahoma */
	{  40,   93, 3501, 3501, 5026,  1107 },	/* Major, Oklahoma */
	{  40,   95, 3502, 3502, 5051,  1129 },	/* Marshall, Oklahoma */
	{  40,   97, 3501, 3501, 5026,  1145 },	/* Mayes, Oklahoma */
	{  40,   99, 3502, 3502, 5051,  1253 },	/* Murray, Oklahoma */
	{  40,  101, 3501, 3501, 5026,  1258 },	/* Muskogee, Oklahoma */
	{  40,  103, 3501, 3501, 5026,  1310 },	/* Noble, Oklahoma */
	{  40,  105, 3501, 3501, 5026,  1329 },	/* Nowata, Oklahoma */
	{  40,  107, 3501, 3501, 5026,  1349 },	/* Okfuskee, Oklahoma */
	{  40,  109,    0,    0,    0,  1350 },	/* Oklahoma, Oklahoma */
	{  40,  111, 3501, 3501, 5026,  1351 },	/* Okmulgee, Oklahoma */
	{  40,  113, 3501, 3501, 5026,  1366 },	/* Osage, Oklahoma */
	{  40,  115, 3501, 3501, 5026,  1374 },	/* Ottawa, Oklahoma */
	{  40,  117, 3501, 3501, 5026,  1405 },	/* Pawnee, Oklahoma */
	{  40,  119, 3501, 3501, 5026,  1407 },	/* Payne, Oklahoma */
	{  40,  121, 3502, 3502, 5051,  1448 },	/* Pittsburg, Oklahoma */
	{  40,  123, 3502, 3502, 5051,  1464 },	/* Pontotoc, Oklahoma */
	{  40,  125, 3502, 3502, 5051,  1471 },	/* Pottawatomie, Oklahoma */
	{  40,  127, 3502, 3502, 5051,  1498 },	/* Pushmataha, Oklahoma */
	{  40,  129, 3501, 3501, 5026,  1565 },	/* Roger Mills, Oklahoma */
	{  40,  131, 3501, 3501, 5026,  1566 },	/* Rogers, Oklahoma */
	{  40,  133, 3502, 3502, 5051,  1654 },	/* Seminole, Oklahoma */
	{  40,  135, 3501, 3501, 5026,  1657 },	/* Sequoyah, Oklahoma */
	{  40,  137, 3502, 3502, 5051,  1741 },	/* Stephens, Oklahoma */
	{  40,  139, 3501, 3501, 5026,  1802 },	/* Texas, Oklahoma */
	{  40,  141, 3502, 3502, 5051,  1809 },	/* Tillman, Oklahoma */
	{  40,  143, 3501, 3501, 5026,  1848 },	/* Tulsa, Oklahoma */
	{  40,  145, 3501, 3501, 5026,  1907 },	/* Wagoner, Oklahoma */
	{  40,  147, 3501, 3501, 5026,  1933 },	/* Washington, Oklahoma */
	{  40,  149, 3502, 3502, 5051,  1934 },	/* Washita, Oklahoma */
	{  40,  151, 3501, 3501, 5026,  2002 },	/* Woods, Oklahoma */
	{  40,  153, 3501, 3501, 5026,  2004 },	/* Woodward, Oklahoma */
	{  41,    0,    0,    0,    0,  1363 },	/* Oregon, Oregon */
	{  41,    1, 3601, 3601, 5076,   107 },	/* Baker, Oregon */
	{  41,    3, 3601, 3601, 5076,   163 },	/* Benton, Oregon */
	{  41,    5, 3601, 3601, 5076,   376 },	/* Clackamas, Oregon */
	{  41,    7, 3601, 3601, 5076,   384 },	/* Clatsop, Oregon */
	{  41,    9, 3601, 3601, 5076,   421 },	/* Columbia, Oregon */
	{  41,   11, 3602, 3602, 5101,   439 },	/* Coos, Oregon */
	{  41,   13, 3602, 3602, 5101,   466 },	/* Crook, Oregon */
	{  41,   15, 3602, 3602, 5101,   478 },	/* Curry, Oregon */
	{  41,   17, 3602, 3602, 5101,   518 },	/* Deschutes, Oregon */
	{  41,   19, 3602, 3602, 5101,   546 },	/* Douglas, Oregon */
	{  41,   21, 3601, 3601, 5076,   690 },	/* Gilliam, Oregon */
	{  41,   23, 3601, 3601, 5076,   721 },	/* Grant, Oregon */
	{  41,   25, 3602, 3602, 5101,   786 },	/* Harney, Oregon */
	{  41,   27, 3601, 3601, 5076,   835 },	/* Hood River, Oregon */
	{  41,   29, 3602, 3602, 5101,   893 },	/* Jackson, Oregon */
	{  41,   31, 3601, 3601, 5076,   901 },	/* Jefferson, Oregon */
	{  41,   33, 3602, 3602, 5101,   918 },	/* Josephine, Oregon */
	{  41,   35, 3602, 3602, 5101,   978 },	/* Klamath, Oregon */
	{  41,   37, 3602, 3602, 5101,  1006 },	/* Lake, Oregon */
	{  41,   39, 3602, 3602, 5101,  1015 },	/* Lane, Oregon */
	{  41,   41, 3601, 3601, 5076,  1060 },	/* Lincoln, Oregon */
	{  41,   43, 3601, 3601, 5076,  1061 },	/* Linn, Oregon */
	{  41,   45, 3602, 3602, 5101,  1109 },	/* Malheur, Oregon */
	{  41,   47, 3601, 3601, 5076,  1125 },	/* Marion, Oregon */
	{  41,   49, 3601, 3601, 5076,  1245 },	/* Morrow, Oregon */
	{  41,   51, 3601, 3601, 5076,  1252 },	/* Multnomah, Oregon */
	{  41,   53, 3601, 3601, 5076,  1461 },	/* Polk, Oregon */
	{  41,   55, 3601, 3601, 5076,  1672 },	/* Sherman, Oregon */
	{  41,   57, 3601, 3601, 5076,  1808 },	/* Tillamook, Oregon */
	{  41,   59, 3601, 3601, 5076,  1865 },	/* Umatilla, Oregon */
	{  41,   61, 3601, 3601, 5076,  1867 },	/* Union, Oregon */
	{  41,   63, 3601, 3601, 5076,  1917 },	/* Wallowa, Oregon */
	{  41,   65, 3601, 3601, 5076,  1928 },	/* Wasco, Oregon */
	{  41,   67, 3601, 3601, 5076,  1933 },	/* Washington, Oregon */
	{  41,   69, 3601, 3601, 5076,  1963 },	/* Wheeler, Oregon */
	{  41,   71, 3601, 3601, 5076,  2019 },	/* Yamhill, Oregon */
	{  42,    0,    0,    0,    0,  1418 },	/* Pennsylvania, Pennsylvania */
	{  42,    1, 3702, 3702, 5151,     5 },	/* Adams, Pennsylvania */
	{  42,    3, 3702, 3702, 5151,    39 },	/* Allegheny, Pennsylvania */
	{  42,    5, 3702, 3702, 5151,    75 },	/* Armstrong, Pennsylvania */
	{  42,    7, 3702, 3702, 5151,   146 },	/* Beaver, Pennsylvania */
	{  42,    9, 3702, 3702, 5151,   150 },	/* Bedford, Pennsylvania */
	{  42,   11, 3702, 3702, 5151,   167 },	/* Berks, Pennsylvania */
	{  42,   13, 3702, 3702, 5151,   186 },	/* Blair, Pennsylvania */
	{  42,   15, 3701, 3701, 5126,   217 },	/* Bradford, Pennsylvania */
	{  42,   17, 3702, 3702, 5151,   246 },	/* Bucks, Pennsylvania */
	{  42,   19, 3702, 3702, 5151,   262 },	/* Butler, Pennsylvania */
	{  42,   21, 3702, 3702, 5151,   283 },	/* Cambria, Pennsylvania */
	{  42,   23, 3701, 3701, 5126,   285 },	/* Cameron, Pennsylvania */
	{  42,   25, 3701, 3701, 5126,   296 },	/* Carbon, Pennsylvania */
	{  42,   27, 3701, 3701, 5126,   326 },	/* Centre, Pennsylvania */
	{  42,   29, 3702, 3702, 5151,   355 },	/* Chester, Pennsylvania */
	{  42,   31, 3701, 3701, 5126,   381 },	/* Clarion, Pennsylvania */
	{  42,   33, 3701, 3701, 5126,   388 },	/* Clearfield, Pennsylvania */
	{  42,   35, 3701, 3701, 5126,   395 },	/* Clinton, Pennsylvania */
	{  42,   37, 3701, 3701, 5126,   421 },	/* Columbia, Pennsylvania */
	{  42,   39, 3701, 3701, 5126,   460 },	/* Crawford, Pennsylvania */
	{  42,   41, 3702, 3702, 5151,   475 },	/* Cumberland, Pennsylvania */
	{  42,   43, 3702, 3702, 5151,   493 },	/* Dauphin, Pennsylvania */
	{  42,   45, 3702, 3702, 5151,   512 },	/* Delaware, Pennsylvania */
	{  42,   47, 3701, 3701, 5126,   584 },	/* Elk, Pennsylvania */
	{  42,   49, 3701, 3701, 5126,   598 },	/* Erie, Pennsylvania */
	{  42,   51, 3702, 3702, 5151,   622 },	/* Fayette, Pennsylvania */
	{  42,   53, 3701, 3701, 5126,   640 },	/* Forest, Pennsylvania */
	{  42,   55, 3702, 3702, 5151,   646 },	/* Franklin, Pennsylvania */
	{  42,   57, 3702, 3702, 5151,   656 },	/* Fulton, Pennsylvania */
	{  42,   59, 3702, 3702, 5151,   732 },	/* Greene, Pennsylvania */
	{  42,   61, 3702, 3702, 5151,   858 },	/* Huntingdon, Pennsylvania */
	{  42,   63, 3702, 3702, 5151,   871 },	/* Indiana, Pennsylvania */
	{  42,   65, 3701, 3701, 5126,   901 },	/* Jefferson, Pennsylvania */
	{  42,   67, 3702, 3702, 5151,   924 },	/* Juniata, Pennsylvania */
	{  42,   69, 3701, 3701, 5126,   999 },	/* Lackawanna, Pennsylvania */
	{  42,   71, 3702, 3702, 5151,  1013 },	/* Lancaster, Pennsylvania */
	{  42,   73, 3702, 3702, 5151,  1033 },	/* Lawrence, Pennsylvania */
	{  42,   75, 3702, 3702, 5151,  1039 },	/* Lebanon, Pennsylvania */
	{  42,   77, 3702, 3702, 5151,  1043 },	/* Lehigh, Pennsylvania */
	{  42,   79, 3701, 3701, 5126,  1090 },	/* Luzerne, Pennsylvania */
	{  42,   81, 3701, 3701, 5126,  1091 },	/* Lycoming, Pennsylvania */
	{  42,   83, 3701, 3701, 5126,  1159 },	/* Mc Kean, Pennsylvania */
	{  42,   85, 3701, 3701, 5126,  1186 },	/* Mercer, Pennsylvania */
	{  42,   87, 3702, 3702, 5151,  1197 },	/* Mifflin, Pennsylvania */
	{  42,   89, 3701, 3701, 5126,  1226 },	/* Monroe, Pennsylvania */
	{  42,   91, 3702, 3702, 5151,  1232 },	/* Montgomery, Pennsylvania */
	{  42,   93, 3701, 3701, 5126,  1234 },	/* Montour, Pennsylvania */
	{  42,   95, 3702, 3702, 5151,  1321 },	/* Northampton, Pennsylvania */
	{  42,   97, 3701, 3701, 5126,  1324 },	/* Northumberland, Pennsylvania */
	{  42,   99, 3702, 3702, 5151,  1425 },	/* Perry, Pennsylvania */
	{  42,  101, 3702, 3702, 5151,  1432 },	/* Philadelphia, Pennsylvania */
	{  42,  103, 3701, 3701, 5126,  1439 },	/* Pike, Pennsylvania */
	{  42,  105, 3701, 3701, 5126,  1473 },	/* Potter, Pennsylvania */
	{  42,  107, 3702, 3702, 5151,  1644 },	/* Schuylkill, Pennsylvania */
	{  42,  109, 3702, 3702, 5151,  1689 },	/* Snyder, Pennsylvania */
	{  42,  111, 3702, 3702, 5151,  1692 },	/* Somerset, Pennsylvania */
	{  42,  113, 3701, 3701, 5126,  1759 },	/* Sullivan, Pennsylvania */
	{  42,  115, 3701, 3701, 5126,  1767 },	/* Susquehanna, Pennsylvania */
	{  42,  117, 3701, 3701, 5126,  1811 },	/* Tioga, Pennsylvania */
	{  42,  119, 3701, 3701, 5126,  1867 },	/* Union, Pennsylvania */
	{  42,  121, 3701, 3701, 5126,  1886 },	/* Venango, Pennsylvania */
	{  42,  123, 3701, 3701, 5126,  1925 },	/* Warren, Pennsylvania */
	{  42,  125, 3702, 3702, 5151,  1933 },	/* Washington, Pennsylvania */
	{  42,  127, 3701, 3701, 5126,  1942 },	/* Wayne, Pennsylvania */
	{  42,  129, 3702, 3702, 5151,  1956 },	/* Westmoreland, Pennsylvania */
	{  42,  131, 3701, 3701, 5126,  2013 },	/* Wyoming, Pennsylvania */
	{  42,  133, 3702, 3702, 5151,  2033 },	/* York, Pennsylvania */
	{  44,    0,    0,    0,    0,  1535 },	/* Rhode Island, Rhode Island */
	{  44,    1, 3800, 3800, 5176,   230 },	/* Bristol, Rhode Island */
	{  44,    3, 3800, 3800, 5176,   948 },	/* Kent, Rhode Island */
	{  44,    5, 3800, 3800, 5176,  1295 },	/* Newport, Rhode Island */
	{  44,    7, 3800, 3800, 5176,  1493 },	/* Providence, Rhode Island */
	{  44,    9, 3800, 3800, 5176,  1933 },	/* Washington, Rhode Island */
	{  45,    0,    0,    0,    0,  1697 },	/* South Carolina, South Carolina */
	{  45,    1, 3900, 3900,    0,     0 },	/* Abbeville, South Carolina */
	{  45,    3, 3900, 3900,    0,    12 },	/* Aiken, South Carolina */
	{  45,    5, 3900, 3900,    0,    41 },	/* Allendale, South Carolina */
	{  45,    7, 3900, 3900,    0,    51 },	/* Anderson, South Carolina */
	{  45,    9, 3900, 3900,    0,   114 },	/* Bamberg, South Carolina */
	{  45,   11, 3900, 3900,    0,   125 },	/* Barnwell, South Carolina */
	{  45,   13, 3900, 3900,    0,   144 },	/* Beaufort, South Carolina */
	{  45,   15, 3900, 3900,    0,   166 },	/* Berkeley, South Carolina */
	{  45,   17, 3900, 3900,    0,   275 },	/* Calhoun, South Carolina */
	{  45,   19, 3900, 3900,    0,   335 },	/* Charleston, South Carolina */
	{  45,   21, 3900, 3900,    0,   351 },	/* Cherokee, South Carolina */
	{  45,   23, 3900, 3900,    0,   355 },	/* Chester, South Carolina */
	{  45,   25, 3900, 3900,    0,   356 },	/* Chesterfield, South Carolina */
	{  45,   27, 3900, 3900,    0,   380 },	/* Clarendon, South Carolina */
	{  45,   29, 3900, 3900,    0,   414 },	/* Colleton, South Carolina */
	{  45,   31, 3900, 3900,    0,   492 },	/* Darlington, South Carolina */
	{  45,   33, 3900, 3900,    0,   528 },	/* Dillon, South Carolina */
	{  45,   35, 3900, 3900,    0,   544 },	/* Dorchester, South Carolina */
	{  45,   37, 3900, 3900,    0,   576 },	/* Edgefield, South Carolina */
	{  45,   39, 3900, 3900,    0,   611 },	/* Fairfield, South Carolina */
	{  45,   41, 3900, 3900,    0,   633 },	/* Florence, South Carolina */
	{  45,   43, 3900, 3900,    0,   683 },	/* Georgetown, South Carolina */
	{  45,   45, 3900, 3900,    0,   736 },	/* Greenville, South Carolina */
	{  45,   47, 3900, 3900,    0,   737 },	/* Greenwood, South Carolina */
	{  45,   49, 3900, 3900,    0,   769 },	/* Hampton, South Carolina */
	{  45,   51, 3900, 3900,    0,   840 },	/* Horry, South Carolina */
	{  45,   53, 3900, 3900,    0,   897 },	/* Jasper, South Carolina */
	{  45,   55, 3900, 3900,    0,   954 },	/* Kershaw, South Carolina */
	{  45,   57, 3900, 3900,    0,  1013 },	/* Lancaster, South Carolina */
	{  45,   59, 3900, 3900,    0,  1031 },	/* Laurens, South Carolina */
	{  45,   61, 3900, 3900,    0,  1040 },	/* Lee, South Carolina */
	{  45,   63, 3900, 3900,    0,  1053 },	/* Lexington, South Carolina */
	{  45,   65, 3900, 3900,    0,  1148 },	/* Mc Cormick, South Carolina */
	{  45,   67, 3900, 3900,    0,  1125 },	/* Marion, South Carolina */
	{  45,   69, 3900, 3900,    0,  1127 },	/* Marlboro, South Carolina */
	{  45,   71, 3900, 3900,    0,  1294 },	/* Newberry, South Carolina */
	{  45,   73, 3900, 3900,    0,  1340 },	/* Oconee, South Carolina */
	{  45,   75, 3900, 3900,    0,  1362 },	/* Orangeburg, South Carolina */
	{  45,   77, 3900, 3900,    0,  1436 },	/* Pickens, South Carolina */
	{  45,   79, 3900, 3900,    0,  1539 },	/* Richland, South Carolina */
	{  45,   81, 3900, 3900,    0,  1600 },	/* Saluda, South Carolina */
	{  45,   83, 3900, 3900,    0,  1702 },	/* Spartanburg, South Carolina */
	{  45,   85, 3900, 3900,    0,  1764 },	/* Sumter, South Carolina */
	{  45,   87, 3900, 3900,    0,  1867 },	/* Union, South Carolina */
	{  45,   89, 3900, 3900,    0,  1981 },	/* Williamsburg, South Carolina */
	{  45,   91, 3900, 3900,    0,  2033 },	/* York, South Carolina */
	{  46,    0,    0,    0,    0,  1698 },	/* South Dakota, South Dakota */
	{  46,    3, 4002, 4002, 5276,    99 },	/* Aurora, South Dakota */
	{  46,    5, 4001, 4001, 5251,   142 },	/* Beadle, South Dakota */
	{  46,    7, 4002, 4002, 5276,   159 },	/* Bennett, South Dakota */
	{  46,    9, 4002, 4002, 5276,   197 },	/* Bon Homme, South Dakota */
	{  46,   11, 4001, 4001, 5251,   236 },	/* Brookings, South Dakota */
	{  46,   13, 4001, 4001, 5251,   240 },	/* Brown, South Dakota */
	{  46,   15, 4002, 4002, 5276,   241 },	/* Brule, South Dakota */
	{  46,   17, 4002, 4002, 5276,   249 },	/* Buffalo, South Dakota */
	{  46,   19, 4001, 4001, 5251,   263 },	/* Butte, South Dakota */
	{  46,   21, 4001, 4001, 5251,   287 },	/* Campbell, South Dakota */
	{  46,   23, 4002, 4002, 5276,   334 },	/* Charles Mix, South Dakota */
	{  46,   25, 4001, 4001, 5251,   382 },	/* Clark, South Dakota */
	{  46,   27, 4002, 4002, 5276,   385 },	/* Clay, South Dakota */
	{  46,   29, 4001, 4001, 5251,   405 },	/* Codington, South Dakota */
	{  46,   31, 4001, 4001, 5251,   443 },	/* Corson, South Dakota */
	{  46,   33, 4002, 4002, 5276,   479 },	/* Custer, South Dakota */
	{  46,   35, 4002, 4002, 5276,   498 },	/* Davison, South Dakota */
	{  46,   37, 4001, 4001, 5251,   501 },	/* Day, South Dakota */
	{  46,   39, 4001, 4001, 5251,   520 },	/* Deuel, South Dakota */
	{  46,   41, 4001, 4001, 5251,   521 },	/* Dewey, South Dakota */
	{  46,   43, 4002, 4002, 5276,   546 },	/* Douglas, South Dakota */
	{  46,   45, 4001, 4001, 5251,   578 },	/* Edmunds, South Dakota */
	{  46,   47, 4002, 4002, 5276,   613 },	/* Fall River, South Dakota */
	{  46,   49, 4001, 4001, 5251,   619 },	/* Faulk, South Dakota */
	{  46,   51, 4001, 4001, 5251,   721 },	/* Grant, South Dakota */
	{  46,   53, 4002, 4002, 5276,   740 },	/* Gregory, South Dakota */
	{  46,   55, 4002, 4002, 5276,   758 },	/* Haakon, South Dakota */
	{  46,   57, 4001, 4001, 5251,   766 },	/* Hamlin, South Dakota */
	{  46,   59, 4001, 4001, 5251,   772 },	/* Hand, South Dakota */
	{  46,   61, 4002, 4002, 5276,   775 },	/* Hanson, South Dakota */
	{  46,   63, 4001, 4001, 5251,   780 },	/* Harding, South Dakota */
	{  46,   65, 4002, 4002, 5276,   852 },	/* Hughes, South Dakota */
	{  46,   67, 4002, 4002, 5276,   861 },	/* Hutchinson, South Dakota */
	{  46,   69, 4001, 4001, 5251,   862 },	/* Hyde, South Dakota */
	{  46,   71, 4002, 4002, 5276,   893 },	/* Jackson, South Dakota */
	{  46,   73, 4002, 4002, 5276,   906 },	/* Jerauld, South Dakota */
	{  46,   75, 4002, 4002, 5276,   917 },	/* Jones, South Dakota */
	{  46,   77, 4001, 4001, 5251,   971 },	/* Kingsbury, South Dakota */
	{  46,   79, 4002, 4002, 5276,  1006 },	/* Lake, South Dakota */
	{  46,   81, 4001, 4001, 5251,  1033 },	/* Lawrence, South Dakota */
	{  46,   83, 4002, 4002, 5276,  1060 },	/* Lincoln, South Dakota */
	{  46,   85, 4002, 4002, 5276,  1092 },	/* Lyman, South Dakota */
	{  46,   87, 4002, 4002, 5276,  1147 },	/* Mc Cook, South Dakota */
	{  46,   89, 4001, 4001, 5251,  1167 },	/* Mc Pherson, South Dakota */
	{  46,   91, 4001, 4001, 5251,  1129 },	/* Marshall, South Dakota */
	{  46,   93, 4001, 4001, 5251,  1171 },	/* Meade, South Dakota */
	{  46,   95, 4002, 4002, 5276,  1180 },	/* Mellette, South Dakota */
	{  46,   97, 4002, 4002, 5276,  1205 },	/* Miner, South Dakota */
	{  46,   99, 4002, 4002, 5276,  1209 },	/* Minnehaha, South Dakota */
	{  46,  101, 4002, 4002, 5276,  1236 },	/* Moody, South Dakota */
	{  46,  103, 4002, 4002, 5276,  1417 },	/* Pennington, South Dakota */
	{  46,  105, 4001, 4001, 5251,  1423 },	/* Perkins, South Dakota */
	{  46,  107, 4001, 4001, 5251,  1473 },	/* Potter, South Dakota */
	{  46,  109, 4001, 4001, 5251,  1554 },	/* Roberts, South Dakota */
	{  46,  111, 4002, 4002, 5276,  1618 },	/* Sanborn, South Dakota */
	{  46,  113, 4002, 4002, 5276,  1661 },	/* Shannon, South Dakota */
	{  46,  115, 4001, 4001, 5251,  1704 },	/* Spink, South Dakota */
	{  46,  117, 4002, 4002, 5276,  1732 },	/* Stanley, South Dakota */
	{  46,  119, 4001, 4001, 5251,  1760 },	/* Sully, South Dakota */
	{  46,  121, 4002, 4002, 5276,  1819 },	/* Todd, South Dakota */
	{  46,  123, 4002, 4002, 5276,  1841 },	/* Tripp, South Dakota */
	{  46,  125, 4002, 4002, 5276,  1851 },	/* Turner, South Dakota */
	{  46,  127, 4002, 4002, 5276,  1867 },	/* Union, South Dakota */
	{  46,  129, 4001, 4001, 5251,  1921 },	/* Walworth, South Dakota */
	{  46,  131,    0,    0,    0,  1930 },	/* Washabaugh, South Dakota */
	{  46,  135, 4002, 4002, 5276,  2021 },	/* Yankton, South Dakota */
	{  46,  137, 4001, 4001, 5251,  2040 },	/* Ziebach, South Dakota */
	{  47,    0,    0,    0,    0,  1796 },	/* Tennessee, Tennessee */
	{  47,    1, 4100, 4100, 5301,    51 },	/* Anderson, Tennessee */
	{  47,    3, 4100, 4100, 5301,   150 },	/* Bedford, Tennessee */
	{  47,    5, 4100, 4100, 5301,   163 },	/* Benton, Tennessee */
	{  47,    7, 4100, 4100, 5301,   190 },	/* Bledsoe, Tennessee */
	{  47,    9, 4100, 4100, 5301,   191 },	/* Blount, Tennessee */
	{  47,   11, 4100, 4100, 5301,   218 },	/* Bradley, Tennessee */
	{  47,   13, 4100, 4100, 5301,   287 },	/* Campbell, Tennessee */
	{  47,   15, 4100, 4100, 5301,   291 },	/* Cannon, Tennessee */
	{  47,   17, 4100, 4100, 5301,   302 },	/* Carroll, Tennessee */
	{  47,   19, 4100, 4100, 5301,   305 },	/* Carter, Tennessee */
	{  47,   21, 4100, 4100, 5301,   346 },	/* Cheatham, Tennessee */
	{  47,   23, 4100, 4100, 5301,   355 },	/* Chester, Tennessee */
	{  47,   25, 4100, 4100, 5301,   377 },	/* Claiborne, Tennessee */
	{  47,   27, 4100, 4100, 5301,   385 },	/* Clay, Tennessee */
	{  47,   29, 4100, 4100, 5301,   403 },	/* Cocke, Tennessee */
	{  47,   31, 4100, 4100, 5301,   406 },	/* Coffee, Tennessee */
	{  47,   33, 4100, 4100, 5301,   465 },	/* Crockett, Tennessee */
	{  47,   35, 4100, 4100, 5301,   475 },	/* Cumberland, Tennessee */
	{  47,   37, 4100, 4100, 5301,   494 },	/* Davidson, Tennessee */
	{  47,   39, 4100, 4100, 5301,   508 },	/* Decatur, Tennessee */
	{  47,   41, 4100, 4100, 5301,   503 },	/* De Kalb, Tennessee */
	{  47,   43, 4100, 4100, 5301,   526 },	/* Dickson, Tennessee */
	{  47,   45, 4100, 4100, 5301,   560 },	/* Dyer, Tennessee */
	{  47,   47, 4100, 4100, 5301,   622 },	/* Fayette, Tennessee */
	{  47,   49, 4100, 4100, 5301,   624 },	/* Fentress, Tennessee */
	{  47,   51, 4100, 4100, 5301,   646 },	/* Franklin, Tennessee */
	{  47,   53, 4100, 4100, 5301,   685 },	/* Gibson, Tennessee */
	{  47,   55, 4100, 4100, 5301,   688 },	/* Giles, Tennessee */
	{  47,   57, 4100, 4100, 5301,   715 },	/* Grainger, Tennessee */
	{  47,   59, 4100, 4100, 5301,   732 },	/* Greene, Tennessee */
	{  47,   61, 4100, 4100, 5301,   744 },	/* Grundy, Tennessee */
	{  47,   63, 4100, 4100, 5301,   764 },	/* Hamblen, Tennessee */
	{  47,   65, 4100, 4100, 5301,   765 },	/* Hamilton, Tennessee */
	{  47,   67, 4100, 4100, 5301,   771 },	/* Hancock, Tennessee */
	{  47,   69, 4100, 4100, 5301,   778 },	/* Hardeman, Tennessee */
	{  47,   71, 4100, 4100, 5301,   779 },	/* Hardin, Tennessee */
	{  47,   73, 4100, 4100, 5301,   799 },	/* Hawkins, Tennessee */
	{  47,   75, 4100, 4100, 5301,   802 },	/* Haywood, Tennessee */
	{  47,   77, 4100, 4100, 5301,   806 },	/* Henderson, Tennessee */
	{  47,   79, 4100, 4100, 5301,   811 },	/* Henry, Tennessee */
	{  47,   81, 4100, 4100, 5301,   816 },	/* Hickman, Tennessee */
	{  47,   83, 4100, 4100, 5301,   844 },	/* Houston, Tennessee */
	{  47,   85, 4100, 4100, 5301,   855 },	/* Humphreys, Tennessee */
	{  47,   87, 4100, 4100, 5301,   893 },	/* Jackson, Tennessee */
	{  47,   89, 4100, 4100, 5301,   901 },	/* Jefferson, Tennessee */
	{  47,   91, 4100, 4100, 5301,   914 },	/* Johnson, Tennessee */
	{  47,   93, 4100, 4100, 5301,   982 },	/* Knox, Tennessee */
	{  47,   95, 4100, 4100, 5301,  1006 },	/* Lake, Tennessee */
	{  47,   97, 4100, 4100, 5301,  1029 },	/* Lauderdale, Tennessee */
	{  47,   99, 4100, 4100, 5301,  1033 },	/* Lawrence, Tennessee */
	{  47,  101, 4100, 4100, 5301,  1051 },	/* Lewis, Tennessee */
	{  47,  103, 4100, 4100, 5301,  1060 },	/* Lincoln, Tennessee */
	{  47,  105, 4100, 4100, 5301,  1075 },	/* Loudon, Tennessee */
	{  47,  107, 4100, 4100, 5301,  1164 },	/* Mc Minn, Tennessee */
	{  47,  109, 4100, 4100, 5301,  1166 },	/* Mc Nairy, Tennessee */
	{  47,  111, 4100, 4100, 5301,  1098 },	/* Macon, Tennessee */
	{  47,  113, 4100, 4100, 5301,  1101 },	/* Madison, Tennessee */
	{  47,  115, 4100, 4100, 5301,  1125 },	/* Marion, Tennessee */
	{  47,  117, 4100, 4100, 5301,  1129 },	/* Marshall, Tennessee */
	{  47,  119, 4100, 4100, 5301,  1142 },	/* Maury, Tennessee */
	{  47,  121, 4100, 4100, 5301,  1177 },	/* Meigs, Tennessee */
	{  47,  123, 4100, 4100, 5301,  1226 },	/* Monroe, Tennessee */
	{  47,  125, 4100, 4100, 5301,  1232 },	/* Montgomery, Tennessee */
	{  47,  127, 4100, 4100, 5301,  1237 },	/* Moore, Tennessee */
	{  47,  129, 4100, 4100, 5301,  1240 },	/* Morgan, Tennessee */
	{  47,  131, 4100, 4100, 5301,  1336 },	/* Obion, Tennessee */
	{  47,  133, 4100, 4100, 5301,  1379 },	/* Overton, Tennessee */
	{  47,  135, 4100, 4100, 5301,  1425 },	/* Perry, Tennessee */
	{  47,  137, 4100, 4100, 5301,  1437 },	/* Pickett, Tennessee */
	{  47,  139, 4100, 4100, 5301,  1461 },	/* Polk, Tennessee */
	{  47,  141, 4100, 4100, 5301,  1499 },	/* Putnam, Tennessee */
	{  47,  143, 4100, 4100, 5301,  1534 },	/* Rhea, Tennessee */
	{  47,  145, 4100, 4100, 5301,  1551 },	/* Roane, Tennessee */
	{  47,  147, 4100, 4100, 5301,  1555 },	/* Robertson, Tennessee */
	{  47,  149, 4100, 4100, 5301,  1584 },	/* Rutherford, Tennessee */
	{  47,  151, 4100, 4100, 5301,  1647 },	/* Scott, Tennessee */
	{  47,  153, 4100, 4100, 5301,  1656 },	/* Sequatchie, Tennessee */
	{  47,  155, 4100, 4100, 5301,  1658 },	/* Sevier, Tennessee */
	{  47,  157, 4100, 4100, 5301,  1668 },	/* Shelby, Tennessee */
	{  47,  159, 4100, 4100, 5301,  1686 },	/* Smith, Tennessee */
	{  47,  161, 4100, 4100, 5301,  1746 },	/* Stewart, Tennessee */
	{  47,  163, 4100, 4100, 5301,  1759 },	/* Sullivan, Tennessee */
	{  47,  165, 4100, 4100, 5301,  1763 },	/* Sumner, Tennessee */
	{  47,  167, 4100, 4100, 5301,  1814 },	/* Tipton, Tennessee */
	{  47,  169, 4100, 4100, 5301,  1843 },	/* Trousdale, Tennessee */
	{  47,  171, 4100, 4100, 5301,  1866 },	/* Unicot, Tennessee */
	{  47,  173, 4100, 4100, 5301,  1867 },	/* Union, Tennessee */
	{  47,  175, 4100, 4100, 5301,  1879 },	/* Van Buren, Tennessee */
	{  47,  177, 4100, 4100, 5301,  1925 },	/* Warren, Tennessee */
	{  47,  179, 4100, 4100, 5301,  1933 },	/* Washington, Tennessee */
	{  47,  181, 4100, 4100, 5301,  1942 },	/* Wayne, Tennessee */
	{  47,  183, 4100, 4100, 5301,  1944 },	/* Weakley, Tennessee */
	{  47,  185, 4100, 4100, 5301,  1964 },	/* White, Tennessee */
	{  47,  187, 4100, 4100, 5301,  1983 },	/* Williamson, Tennessee */
	{  47,  189, 4100, 4100, 5301,  1984 },	/* Wilson, Tennessee */
	{  48,    0,    0,    0,    0,  1802 },	/* Texas, Texas */
	{  48,    1, 4203, 4203, 5372,    51 },	/* Anderson, Texas */
	{  48,    3, 4202, 4202, 5351,    53 },	/* Andrews, Texas */
	{  48,    5, 4203, 4203, 5372,    56 },	/* Angelina, Texas */
	{  48,    7, 4204, 4204, 5401,    66 },	/* Aransas, Texas */
	{  48,    9, 4202, 4202, 5351,    68 },	/* Archer, Texas */
	{  48,   11, 4201, 4201, 5326,    75 },	/* Armstrong, Texas */
	{  48,   13, 4204, 4204, 5401,    87 },	/* Atascosa, Texas */
	{  48,   15, 4204, 4204, 5401,   100 },	/* Austin, Texas */
	{  48,   17, 4202, 4202, 5351,   106 },	/* Bailey, Texas */
	{  48,   19, 4204, 4204, 5401,   115 },	/* Bandera, Texas */
	{  48,   21, 4203, 4203, 5372,   134 },	/* Bastrop, Texas */
	{  48,   23, 4202, 4202, 5351,   141 },	/* Baylor, Texas */
	{  48,   25, 4204, 4204, 5401,   152 },	/* Bee, Texas */
	{  48,   27, 4203, 4203, 5372,   154 },	/* Bell, Texas */
	{  48,   29, 4204, 4204, 5401,   173 },	/* Bexar, Texas */
	{  48,   31, 4203, 4203, 5372,   187 },	/* Blanco, Texas */
	{  48,   33, 4202, 4202, 5351,   202 },	/* Borden, Texas */
	{  48,   35, 4203, 4203, 5372,   203 },	/* Bosque, Texas */
	{  48,   37, 4202, 4202, 5351,   210 },	/* Bowie, Texas */
	{  48,   39, 4204, 4204, 5401,   222 },	/* Brazoria, Texas */
	{  48,   41, 4203, 4203, 5372,   223 },	/* Brazos, Texas */
	{  48,   43, 4204, 4204, 5401,   228 },	/* Brewster, Texas */
	{  48,   45, 4201, 4201, 5326,   229 },	/* Briscoe, Texas */
	{  48,   47, 4205, 4205, 5426,   237 },	/* Brooks, Texas */
	{  48,   49, 4203, 4203, 5372,   240 },	/* Brown, Texas */
	{  48,   51, 4203, 4203, 5372,   257 },	/* Burleson, Texas */
	{  48,   53, 4203, 4203, 5372,   259 },	/* Burnet, Texas */
	{  48,   55, 4204, 4204, 5401,   273 },	/* Caldwell, Texas */
	{  48,   57, 4204, 4204, 5401,   275 },	/* Calhoun, Texas */
	{  48,   59, 4202, 4202, 5351,   277 },	/* Callahan, Texas */
	{  48,   61, 4205, 4205, 5426,   285 },	/* Cameron, Texas */
	{  48,   63, 4202, 4202, 5351,   286 },	/* Camp, Texas */
	{  48,   65, 4201, 4201, 5326,   303 },	/* Carson, Texas */
	{  48,   67, 4202, 4202, 5351,   310 },	/* Cass, Texas */
	{  48,   69, 4201, 4201, 5326,   312 },	/* Castro, Texas */
	{  48,   71, 4204, 4204, 5401,   329 },	/* Chambers, Texas */
	{  48,   73, 4203, 4203, 5372,   351 },	/* Cherokee, Texas */
	{  48,   75, 4201, 4201, 5326,   360 },	/* Childress, Texas */
	{  48,   77, 4202, 4202, 5351,   385 },	/* Clay, Texas */
	{  48,   79, 4202, 4202, 5351,   402 },	/* Cochran, Texas */
	{  48,   81, 4203, 4203, 5372,   408 },	/* Coke, Texas */
	{  48,   83, 4203, 4203, 5372,   411 },	/* Coleman, Texas */
	{  48,   85, 4202, 4202, 5351,   416 },	/* Collin, Texas */
	{  48,   87, 4201, 4201, 5326,   417 },	/* Collingsworth, Texas */
	{  48,   89, 4204, 4204, 5401,   419 },	/* Colorado, Texas */
	{  48,   91, 4204, 4204, 5401,   425 },	/* Comal, Texas */
	{  48,   93, 4203, 4203, 5372,   426 },	/* Comanche, Texas */
	{  48,   95, 4203, 4203, 5372,   428 },	/* Concho, Texas */
	{  48,   97, 4202, 4202, 5351,   437 },	/* Cooke, Texas */
	{  48,   99, 4203, 4203, 5372,   445 },	/* Coryell, Texas */
	{  48,  101, 4202, 4202, 5351,   448 },	/* Cottle, Texas */
	{  48,  103, 4203, 4203, 5372,   458 },	/* Crane, Texas */
	{  48,  105, 4203, 4203, 5372,   465 },	/* Crockett, Texas */
	{  48,  107, 4202, 4202, 5351,   467 },	/* Crosby, Texas */
	{  48,  109, 4203, 4203, 5372,   471 },	/* Culberson, Texas */
	{  48,  111, 4201, 4201, 5326,   485 },	/* Dallam, Texas */
	{  48,  113, 4202, 4202, 5351,   486 },	/* Dallas, Texas */
	{  48,  115, 4202, 4202, 5351,   500 },	/* Dawson, Texas */
	{  48,  117, 4201, 4201, 5326,   506 },	/* Deaf Smith, Texas */
	{  48,  119, 4202, 4202, 5351,   513 },	/* Delta, Texas */
	{  48,  121, 4202, 4202, 5351,   515 },	/* Denton, Texas */
	{  48,  123, 4204, 4204, 5401,   505 },	/* De Witt, Texas */
	{  48,  125, 4202, 4202, 5351,   522 },	/* Dickens, Texas */
	{  48,  127, 4204, 4204, 5401,   529 },	/* Dimmit, Texas */
	{  48,  129, 4201, 4201, 5326,   540 },	/* Donley, Texas */
	{  48,  131, 4205, 4205, 5426,   559 },	/* Duval, Texas */
	{  48,  133, 4202, 4202, 5351,   567 },	/* Eastland, Texas */
	{  48,  135, 4203, 4203, 5372,   572 },	/* Ector, Texas */
	{  48,  137, 4204, 4204, 5401,   579 },	/* Edwards, Texas */
	{  48,  139, 4202, 4202, 5351,   588 },	/* Ellis, Texas */
	{  48,  141, 4203, 4203, 5372,   582 },	/* El Paso, Texas */
	{  48,  143, 4202, 4202, 5351,   597 },	/* Erath, Texas */
	{  48,  145, 4203, 4203, 5372,   615 },	/* Falls, Texas */
	{  48,  147, 4202, 4202, 5351,   617 },	/* Fannin, Texas */
	{  48,  149, 4204, 4204, 5401,   622 },	/* Fayette, Texas */
	{  48,  151, 4202, 4202, 5351,   629 },	/* Fisher, Texas */
	{  48,  153, 4202, 4202, 5351,   635 },	/* Floyd, Texas */
	{  48,  155, 4202, 4202, 5351,   637 },	/* Foard, Texas */
	{  48,  157, 4204, 4204, 5401,   643 },	/* Fort Bend, Texas */
	{  48,  159, 4202, 4202, 5351,   646 },	/* Franklin, Texas */
	{  48,  161, 4203, 4203, 5372,   651 },	/* Freestone, Texas */
	{  48,  163, 4204, 4204, 5401,   654 },	/* Frio, Texas */
	{  48,  165, 4202, 4202, 5351,   660 },	/* Gaines, Texas */
	{  48,  167, 4204, 4204, 5401,   664 },	/* Galveston, Texas */
	{  48,  169, 4202, 4202, 5351,   671 },	/* Garza, Texas */
	{  48,  171, 4203, 4203, 5372,   689 },	/* Gillespie, Texas */
	{  48,  173, 4203, 4203, 5372,   697 },	/* Glasscock, Texas */
	{  48,  175, 4204, 4204, 5401,   703 },	/* Goliad, Texas */
	{  48,  177, 4204, 4204, 5401,   704 },	/* Gonzales, Texas */
	{  48,  179, 4201, 4201, 5326,   725 },	/* Gray, Texas */
	{  48,  181, 4202, 4202, 5351,   727 },	/* Grayson, Texas */
	{  48,  183, 4202, 4202, 5351,   739 },	/* Gregg, Texas */
	{  48,  185, 4203, 4203, 5372,   743 },	/* Grimes, Texas */
	{  48,  187, 4204, 4204, 5401,   745 },	/* Guadalupe, Texas */
	{  48,  189, 4202, 4202, 5351,   761 },	/* Hale, Texas */
	{  48,  191, 4201, 4201, 5326,   763 },	/* Hall, Texas */
	{  48,  193, 4203, 4203, 5372,   765 },	/* Hamilton, Texas */
	{  48,  195, 4201, 4201, 5326,   774 },	/* Hansford, Texas */
	{  48,  197, 4202, 4202, 5351,   778 },	/* Hardeman, Texas */
	{  48,  199, 4203, 4203, 5372,   779 },	/* Hardin, Texas */
	{  48,  201, 4204, 4204, 5401,   788 },	/* Harris, Texas */
	{  48,  203, 4202, 4202, 5351,   789 },	/* Harrison, Texas */
	{  48,  205, 4201, 4201, 5326,   793 },	/* Hartley, Texas */
	{  48,  207, 4202, 4202, 5351,   795 },	/* Haskell, Texas */
	{  48,  209, 4204, 4204, 5401,   801 },	/* Hays, Texas */
	{  48,  211, 4201, 4201, 5326,   804 },	/* Hemphill, Texas */
	{  48,  213, 4202, 4202, 5351,   806 },	/* Henderson, Texas */
	{  48,  215, 4205, 4205, 5426,   818 },	/* Hidalgo, Texas */
	{  48,  217, 4202, 4202, 5351,   821 },	/* Hill, Texas */
	{  48,  219, 4202, 4202, 5351,   828 },	/* Hockley, Texas */
	{  48,  221, 4202, 4202, 5351,   834 },	/* Hood, Texas */
	{  48,  223, 4202, 4202, 5351,   838 },	/* Hopkins, Texas */
	{  48,  225, 4203, 4203, 5372,   844 },	/* Houston, Texas */
	{  48,  227, 4202, 4202, 5351,   845 },	/* Howard, Texas */
	{  48,  229, 4203, 4203, 5372,   850 },	/* Hudspeth, Texas */
	{  48,  231, 4202, 4202, 5351,   856 },	/* Hunt, Texas */
	{  48,  233, 4201, 4201, 5326,   861 },	/* Hutchinson, Texas */
	{  48,  235, 4203, 4203, 5372,   878 },	/* Irion, Texas */
	{  48,  237, 4202, 4202, 5351,   892 },	/* Jack, Texas */
	{  48,  239, 4204, 4204, 5401,   893 },	/* Jackson, Texas */
	{  48,  241, 4203, 4203, 5372,   897 },	/* Jasper, Texas */
	{  48,  243, 4203, 4203, 5372,   900 },	/* Jeff Davis, Texas */
	{  48,  245, 4204, 4204, 5401,   901 },	/* Jefferson, Texas */
	{  48,  247, 4205, 4205, 5426,   911 },	/* Jim Hogg, Texas */
	{  48,  249, 4205, 4205, 5426,   912 },	/* Jim Wells, Texas */
	{  48,  251, 4202, 4202, 5351,   914 },	/* Johnson, Texas */
	{  48,  253, 4202, 4202, 5351,   917 },	/* Jones, Texas */
	{  48,  255, 4204, 4204, 5401,   934 },	/* Karnes, Texas */
	{  48,  257, 4202, 4202, 5351,   936 },	/* Kaufman, Texas */
	{  48,  259, 4204, 4204, 5401,   944 },	/* Kendall, Texas */
	{  48,  261, 4205, 4205, 5426,   945 },	/* Kenedy, Texas */
	{  48,  263, 4202, 4202, 5351,   948 },	/* Kent, Texas */
	{  48,  265, 4204, 4204, 5401,   953 },	/* Kerr, Texas */
	{  48,  267, 4203, 4203, 5372,   962 },	/* Kimble, Texas */
	{  48,  269, 4202, 4202, 5351,   963 },	/* King, Texas */
	{  48,  271, 4204, 4204, 5401,   972 },	/* Kinney, Texas */
	{  48,  273, 4205, 4205, 5426,   979 },	/* Kleberg, Texas */
	{  48,  275, 4202, 4202, 5351,   982 },	/* Knox, Texas */
	{  48,  277, 4202, 4202, 5351,  1009 },	/* Lamar, Texas */
	{  48,  279, 4202, 4202, 5351,  1010 },	/* Lamb, Texas */
	{  48,  281, 4203, 4203, 5372,  1012 },	/* Lampasas, Texas */
	{  48,  283, 4204, 4204, 5401,   996 },	/* La Salle, Texas */
	{  48,  285, 4204, 4204, 5401,  1032 },	/* Lavaca, Texas */
	{  48,  287, 4203, 4203, 5372,  1040 },	/* Lee, Texas */
	{  48,  289, 4203, 4203, 5372,  1047 },	/* Leon, Texas */
	{  48,  291, 4203, 4203, 5372,  1056 },	/* Liberty, Texas */
	{  48,  293, 4203, 4203, 5372,  1059 },	/* Limestone, Texas */
	{  48,  295, 4201, 4201, 5326,  1062 },	/* Lipscomb, Texas */
	{  48,  297, 4204, 4204, 5401,  1065 },	/* Live Oak, Texas */
	{  48,  299, 4203, 4203, 5372,  1067 },	/* Llano, Texas */
	{  48,  301, 4203, 4203, 5372,  1081 },	/* Loving, Texas */
	{  48,  303, 4202, 4202, 5351,  1083 },	/* Lubbock, Texas */
	{  48,  305, 4202, 4202, 5351,  1094 },	/* Lynn, Texas */
	{  48,  307, 4203, 4203, 5372,  1151 },	/* Mc Culloch, Texas */
	{  48,  309, 4203, 4203, 5372,  1162 },	/* Mc Lennan, Texas */
	{  48,  311, 4204, 4204, 5401,  1165 },	/* Mc Mullen, Texas */
	{  48,  313, 4203, 4203, 5372,  1101 },	/* Madison, Texas */
	{  48,  315, 4202, 4202, 5351,  1125 },	/* Marion, Texas */
	{  48,  317, 4202, 4202, 5351,  1131 },	/* Martin, Texas */
	{  48,  319, 4203, 4203, 5372,  1134 },	/* Mason, Texas */
	{  48,  321, 4204, 4204, 5401,  1137 },	/* Matagorda, Texas */
	{  48,  323, 4204, 4204, 5401,  1143 },	/* Maverick, Texas */
	{  48,  325, 4204, 4204, 5401,  1175 },	/* Medina, Texas */
	{  48,  327, 4203, 4203, 5372,  1181 },	/* Menard, Texas */
	{  48,  329, 4203, 4203, 5372,  1195 },	/* Midland, Texas */
	{  48,  331, 4203, 4203, 5372,  1198 },	/* Milam, Texas */
	{  48,  333, 4203, 4203, 5372,  1203 },	/* Mills, Texas */
	{  48,  335, 4202, 4202, 5351,  1215 },	/* Mitchell, Texas */
	{  48,  337, 4202, 4202, 5351,  1227 },	/* Montague, Texas */
	{  48,  339, 4203, 4203, 5372,  1232 },	/* Montgomery, Texas */
	{  48,  341, 4201, 4201, 5326,  1237 },	/* Moore, Texas */
	{  48,  343, 4202, 4202, 5351,  1243 },	/* Morris, Texas */
	{  48,  345, 4202, 4202, 5351,  1247 },	/* Motley, Texas */
	{  48,  347, 4203, 4203, 5372,  1260 },	/* Nacogdoches, Texas */
	{  48,  349, 4202, 4202, 5351,  1274 },	/* Navarro, Texas */
	{  48,  351, 4203, 4203, 5372,  1297 },	/* Newton, Texas */
	{  48,  353, 4202, 4202, 5351,  1313 },	/* Nolan, Texas */
	{  48,  355, 4205, 4205, 5426,  1332 },	/* Nueces, Texas */
	{  48,  357, 4201, 4201, 5326,  1339 },	/* Ochiltree, Texas */
	{  48,  359, 4201, 4201, 5326,  1353 },	/* Oldham, Texas */
	{  48,  361, 4203, 4203, 5372,  1361 },	/* Orange, Texas */
	{  48,  363, 4202, 4202, 5351,  1392 },	/* Palo Pinto, Texas */
	{  48,  365, 4202, 4202, 5351,  1394 },	/* Panola, Texas */
	{  48,  367, 4202, 4202, 5351,  1397 },	/* Parker, Texas */
	{  48,  369, 4201, 4201, 5326,  1398 },	/* Parmer, Texas */
	{  48,  371, 4203, 4203, 5372,  1410 },	/* Pecos, Texas */
	{  48,  373, 4203, 4203, 5372,  1461 },	/* Polk, Texas */
	{  48,  375, 4201, 4201, 5326,  1473 },	/* Potter, Texas */
	{  48,  377, 4204, 4204, 5401,  1483 },	/* Presidio, Texas */
	{  48,  379, 4202, 4202, 5351,  1508 },	/* Rains, Texas */
	{  48,  381, 4201, 4201, 5326,  1512 },	/* Randall, Texas */
	{  48,  383, 4203, 4203, 5372,  1521 },	/* Reagan, Texas */
	{  48,  385, 4204, 4204, 5401,  1522 },	/* Real, Texas */
	{  48,  387, 4202, 4202, 5351,  1524 },	/* Red River, Texas */
	{  48,  389, 4203, 4203, 5372,  1527 },	/* Reeves, Texas */
	{  48,  391, 4204, 4204, 5401,  1528 },	/* Refugio, Texas */
	{  48,  393, 4201, 4201, 5326,  1554 },	/* Roberts, Texas */
	{  48,  395, 4203, 4203, 5372,  1555 },	/* Robertson, Texas */
	{  48,  397, 4202, 4202, 5351,  1564 },	/* Rockwall, Texas */
	{  48,  399, 4203, 4203, 5372,  1580 },	/* Runnels, Texas */
	{  48,  401, 4202, 4202, 5351,  1582 },	/* Rusk, Texas */
	{  48,  403, 4203, 4203, 5372,  1588 },	/* Sabine, Texas */
	{  48,  405, 4203, 4203, 5372,  1602 },	/* San Augustine, Texas */
	{  48,  407, 4203, 4203, 5372,  1608 },	/* San Jacinto, Texas */
	{  48,  409, 4205, 4205, 5426,  1615 },	/* San Patricio, Texas */
	{  48,  411, 4203, 4203, 5372,  1616 },	/* San Saba, Texas */
	{  48,  413, 4203, 4203, 5372,  1639 },	/* Schleicher, Texas */
	{  48,  415, 4202, 4202, 5351,  1650 },	/* Scurry, Texas */
	{  48,  417, 4202, 4202, 5351,  1660 },	/* Shackelford, Texas */
	{  48,  419, 4203, 4203, 5372,  1668 },	/* Shelby, Texas */
	{  48,  421, 4201, 4201, 5326,  1672 },	/* Sherman, Texas */
	{  48,  423, 4202, 4202, 5351,  1686 },	/* Smith, Texas */
	{  48,  425, 4202, 4202, 5351,  1693 },	/* Somervell, Texas */
	{  48,  427, 4205, 4205, 5426,  1737 },	/* Starr, Texas */
	{  48,  429, 4202, 4202, 5351,  1741 },	/* Stephens, Texas */
	{  48,  431, 4203, 4203, 5372,  1743 },	/* Sterling, Texas */
	{  48,  433, 4202, 4202, 5351,  1751 },	/* Stonewall, Texas */
	{  48,  435, 4203, 4203, 5372,  1770 },	/* Sutton, Texas */
	{  48,  437, 4201, 4201, 5326,  1777 },	/* Swisher, Texas */
	{  48,  439, 4202, 4202, 5351,  1788 },	/* Tarrant, Texas */
	{  48,  441, 4202, 4202, 5351,  1791 },	/* Taylor, Texas */
	{  48,  443, 4204, 4204, 5401,  1799 },	/* Terrell, Texas */
	{  48,  445, 4202, 4202, 5351,  1800 },	/* Terry, Texas */
	{  48,  447, 4202, 4202, 5351,  1805 },	/* Throckmorton, Texas */
	{  48,  449, 4202, 4202, 5351,  1816 },	/* Titus, Texas */
	{  48,  451, 4203, 4203, 5372,  1822 },	/* Tom Green, Texas */
	{  48,  453, 4203, 4203, 5372,  1833 },	/* Travis, Texas */
	{  48,  455, 4203, 4203, 5372,  1840 },	/* Trinity, Texas */
	{  48,  457, 4203, 4203, 5372,  1857 },	/* Tyler, Texas */
	{  48,  459, 4202, 4202, 5351,  1868 },	/* Upshur, Texas */
	{  48,  461, 4203, 4203, 5372,  1870 },	/* Upton, Texas */
	{  48,  463, 4204, 4204, 5401,  1874 },	/* Uvalde, Texas */
	{  48,  465, 4204, 4204, 5401,  1875 },	/* Val Verde, Texas */
	{  48,  467, 4202, 4202, 5351,  1881 },	/* Van Zandt, Texas */
	{  48,  469, 4204, 4204, 5401,  1892 },	/* Victoria, Texas */
	{  48,  471, 4203, 4203, 5372,  1913 },	/* Walker, Texas */
	{  48,  473, 4204, 4204, 5401,  1916 },	/* Waller, Texas */
	{  48,  475, 4203, 4203, 5372,  1923 },	/* Ward, Texas */
	{  48,  477, 4203, 4203, 5372,  1933 },	/* Washington, Texas */
	{  48,  479, 4205, 4205, 5426,  1945 },	/* Webb, Texas */
	{  48,  481, 4204, 4204, 5401,  1960 },	/* Wharton, Texas */
	{  48,  483, 4201, 4201, 5326,  1963 },	/* Wheeler, Texas */
	{  48,  485, 4202, 4202, 5351,  1971 },	/* Wichita, Texas */
	{  48,  487, 4202, 4202, 5351,  1973 },	/* Wilbarger, Texas */
	{  48,  489, 4205, 4205, 5426,  1979 },	/* Willacy, Texas */
	{  48,  491, 4203, 4203, 5372,  1983 },	/* Williamson, Texas */
	{  48,  493, 4204, 4204, 5401,  1984 },	/* Wilson, Texas */
	{  48,  495, 4203, 4203, 5372,  1988 },	/* Winkler, Texas */
	{  48,  497, 4202, 4202, 5351,  1996 },	/* Wise, Texas */
	{  48,  499, 4202, 4202, 5351,  1998 },	/* Wood, Texas */
	{  48,  501, 4202, 4202, 5351,  2031 },	/* Yoakum, Texas */
	{  48,  503, 4202, 4202, 5351,  2034 },	/* Young, Texas */
	{  48,  505, 4205, 4205, 5426,  2038 },	/* Zapata, Texas */
	{  48,  507, 4204, 4204, 5401,  2039 },	/* Zavala, Texas */
	{  49,    0, 4302, 4302, 5476,  1871 },	/* Utah, Utah */
	{  49,    1, 4303, 4303, 5501,   146 },	/* Beaver, Utah */
	{  49,    3, 4301, 4301, 5451,   213 },	/* Box Elder, Utah */
	{  49,    5, 4301, 4301, 5451,   268 },	/* Cache, Utah */
	{  49,    7, 4302, 4302, 5476,   296 },	/* Carbon, Utah */
	{  49,    9, 4301, 4301, 5451,   482 },	/* Daggett, Utah */
	{  49,   11, 4301, 4301, 5451,   497 },	/* Davis, Utah */
	{  49,   13, 4302, 4302, 5476,   551 },	/* Duchesne, Utah */
	{  49,   15, 4302, 4302, 5476,   592 },	/* Emery, Utah */
	{  49,   17, 4303, 4303, 5501,   666 },	/* Garfield, Utah */
	{  49,   19, 4302, 4302, 5476,   716 },	/* Grand, Utah */
	{  49,   21, 4303, 4303, 5501,   879 },	/* Iron, Utah */
	{  49,   23, 4302, 4302, 5476,   919 },	/* Juab, Utah */
	{  49,   25, 4303, 4303, 5501,   931 },	/* Kane, Utah */
	{  49,   27, 4302, 4302, 5476,  1200 },	/* Millard, Utah */
	{  49,   29, 4301, 4301, 5451,  1240 },	/* Morgan, Utah */
	{  49,   31, 4303, 4303, 5501,  1450 },	/* Piute, Utah */
	{  49,   33, 4301, 4301, 5451,  1537 },	/* Rich, Utah */
	{  49,   35, 4302, 4302, 5476,  1599 },	/* Salt Lake, Utah */
	{  49,   37, 4303, 4303, 5501,  1610 },	/* San Juan, Utah */
	{  49,   39, 4302, 4302, 5476,  1624 },	/* Sanpete, Utah */
	{  49,   41, 4302, 4302, 5476,  1658 },	/* Sevier, Utah */
	{  49,   43, 4301, 4301, 5451,  1762 },	/* Summit, Utah */
	{  49,   45, 4302, 4302, 5476,  1824 },	/* Tooele, Utah */
	{  49,   47, 4302, 4302, 5476,  1861 },	/* Uintah, Utah */
	{  49,   49,    0,    0,    0,  1871 },	/* Utah, Utah */
	{  49,   51, 4302, 4302, 5476,  1927 },	/* Wasatch, Utah */
	{  49,   53, 4303, 4303, 5501,  1933 },	/* Washington, Utah */
	{  49,   55, 4303, 4303, 5501,  1942 },	/* Wayne, Utah */
	{  49,   57, 4301, 4301, 5451,  1946 },	/* Weber, Utah */
	{  50,    0,    0,    0,    0,  1890 },	/* Vermont, Vermont */
	{  50,    1, 4400, 4400, 5526,     6 },	/* Addison, Vermont */
	{  50,    3, 4400, 4400, 5526,   160 },	/* Bennington, Vermont */
	{  50,    5, 4400, 4400, 5526,   274 },	/* Caledonia, Vermont */
	{  50,    7, 4400, 4400, 5526,   364 },	/* Chittenden, Vermont */
	{  50,    9, 4400, 4400, 5526,   602 },	/* Essex, Vermont */
	{  50,   11, 4400, 4400, 5526,   646 },	/* Franklin, Vermont */
	{  50,   13, 4400, 4400, 5526,   718 },	/* Grand Isle, Vermont */
	{  50,   15, 4400, 4400, 5526,  1011 },	/* Lamoille, Vermont */
	{  50,   17, 4400, 4400, 5526,  1361 },	/* Orange, Vermont */
	{  50,   19, 4400, 4400, 5526,  1364 },	/* Orleans, Vermont */
	{  50,   21, 4400, 4400, 5526,  1585 },	/* Rutland, Vermont */
	{  50,   23, 4400, 4400, 5526,  1933 },	/* Washington, Vermont */
	{  50,   25, 4400, 4400, 5526,  1986 },	/* Windham, Vermont */
	{  50,   27, 4400, 4400, 5526,  1987 },	/* Windsor, Vermont */
	{  51,    0,    0,    0,    0,  1899 },	/* Virginia, Virginia */
	{  51,    1, 4502, 4502, 5576,     2 },	/* Accomack, Virginia */
	{  51,    3, 4502, 4502, 5576,    26 },	/* Albemarle, Virginia */
	{  51,    5, 4502, 4502, 5576,    38 },	/* Alleghany, Virginia */
	{  51,    7, 4502, 4502, 5576,    45 },	/* Amelia, Virginia */
	{  51,    9, 4502, 4502, 5576,    47 },	/* Amherst, Virginia */
	{  51,   11, 4502, 4502, 5576,    65 },	/* Appomattox, Virginia */
	{  51,   13, 4501, 4501, 5551,    74 },	/* Arlington, Virginia */
	{  51,   15, 4501, 4501, 5551,    97 },	/* Augusta, Virginia */
	{  51,   17, 4501, 4501, 5551,   136 },	/* Bath, Virginia */
	{  51,   19, 4502, 4502, 5576,   150 },	/* Bedford, Virginia */
	{  51,   21, 4502, 4502, 5576,   188 },	/* Bland, Virginia */
	{  51,   23, 4502, 4502, 5576,   205 },	/* Botetourt, Virginia */
	{  51,   25, 4502, 4502, 5576,   242 },	/* Brunswick, Virginia */
	{  51,   27, 4502, 4502, 5576,   244 },	/* Buchanan, Virginia */
	{  51,   29, 4502, 4502, 5576,   245 },	/* Buckingham, Virginia */
	{  51,   31, 4502, 4502, 5576,   287 },	/* Campbell, Virginia */
	{  51,   33, 4501, 4501, 5551,   301 },	/* Caroline, Virginia */
	{  51,   35, 4502, 4502, 5576,   302 },	/* Carroll, Virginia */
	{  51,   36, 4502, 4502, 5576,   333 },	/* Charles City, Virginia */
	{  51,   37, 4502, 4502, 5576,   337 },	/* Charlotte, Virginia */
	{  51,   41, 4502, 4502, 5576,   356 },	/* Chesterfield, Virginia */
	{  51,   43, 4501, 4501, 5551,   383 },	/* Clarke, Virginia */
	{  51,   45, 4502, 4502, 5576,   456 },	/* Craig, Virginia */
	{  51,   47, 4501, 4501, 5551,   474 },	/* Culpeper, Virginia */
	{  51,   49, 4502, 4502, 5576,   475 },	/* Cumberland, Virginia */
	{  51,   51, 4502, 4502, 5576,   523 },	/* Dickenson, Virginia */
	{  51,   53, 4502, 4502, 5576,   530 },	/* Dinwiddie, Virginia */
	{  51,   57, 4502, 4502, 5576,   602 },	/* Essex, Virginia */
	{  51,   59, 4501, 4501, 5551,   609 },	/* Fairfax, Virginia */
	{  51,   61, 4501, 4501, 5551,   621 },	/* Fauquier, Virginia */
	{  51,   63, 4502, 4502, 5576,   635 },	/* Floyd, Virginia */
	{  51,   65, 4502, 4502, 5576,   636 },	/* Fluvanna, Virginia */
	{  51,   67, 4502, 4502, 5576,   646 },	/* Franklin, Virginia */
	{  51,   69, 4501, 4501, 5551,   648 },	/* Frederick, Virginia */
	{  51,   71, 4502, 4502, 5576,   688 },	/* Giles, Virginia */
	{  51,   73, 4502, 4502, 5576,   699 },	/* Gloucester, Virginia */
	{  51,   75, 4502, 4502, 5576,   705 },	/* Goochland, Virginia */
	{  51,   77, 4502, 4502, 5576,   727 },	/* Grayson, Virginia */
	{  51,   79, 4501, 4501, 5551,   732 },	/* Greene, Virginia */
	{  51,   81, 4502, 4502, 5576,   734 },	/* Greensville, Virginia */
	{  51,   83, 4502, 4502, 5576,   762 },	/* Halifax, Virginia */
	{  51,   85, 4502, 4502, 5576,   773 },	/* Hanover, Virginia */
	{  51,   87, 4502, 4502, 5576,   810 },	/* Henrico, Virginia */
	{  51,   89, 4502, 4502, 5576,   811 },	/* Henry, Virginia */
	{  51,   91, 4501, 4501, 5551,   819 },	/* Highland, Virginia */
	{  51,   93, 4502, 4502, 5576,   886 },	/* Isle of Wight, Virginia */
	{  51,   95, 4502, 4502, 5576,   895 },	/* James City, Virginia */
	{  51,   97, 4502, 4502, 5576,   966 },	/* King and Queen, Virginia */
	{  51,   99, 4501, 4501, 5551,   964 },	/* King George, Virginia */
	{  51,  101, 4502, 4502, 5576,   965 },	/* King William, Virginia */
	{  51,  103, 4502, 4502, 5576,  1013 },	/* Lancaster, Virginia */
	{  51,  105, 4502, 4502, 5576,  1040 },	/* Lee, Virginia */
	{  51,  107, 4501, 4501, 5551,  1076 },	/* Loudoun, Virginia */
	{  51,  109, 4502, 4502, 5576,  1077 },	/* Louisa, Virginia */
	{  51,  111, 4502, 4502, 5576,  1088 },	/* Lunenburg, Virginia */
	{  51,  113, 4501, 4501, 5551,  1101 },	/* Madison, Virginia */
	{  51,  115, 4502, 4502, 5576,  1139 },	/* Mathews, Virginia */
	{  51,  117, 4502, 4502, 5576,  1173 },	/* Mecklenburg, Virginia */
	{  51,  119, 4502, 4502, 5576,  1194 },	/* Middlesex, Virginia */
	{  51,  121, 4502, 4502, 5576,  1232 },	/* Montgomery, Virginia */
	{  51,  123,    0,    0,    0,  1265 },	/* Nansemond, Virginia */
	{  51,  125, 4502, 4502, 5576,  1277 },	/* Nelson, Virginia */
	{  51,  127, 4502, 4502, 5576,  1288 },	/* New Kent, Virginia */
	{  51,  129, 4502, 4502, 5576,  1315 },	/* Norfolk, Virginia */
	{  51,  131, 4502, 4502, 5576,  1321 },	/* Northampton, Virginia */
	{  51,  133, 4502, 4502, 5576,  1324 },	/* Northumberland, Virginia */
	{  51,  135, 4502, 4502, 5576,  1328 },	/* Nottoway, Virginia */
	{  51,  137, 4501, 4501, 5551,  1361 },	/* Orange, Virginia */
	{  51,  139, 4501, 4501, 5551,  1387 },	/* Page, Virginia */
	{  51,  141, 4502, 4502, 5576,  1403 },	/* Patrick, Virginia */
	{  51,  143, 4502, 4502, 5576,  1449 },	/* Pittsylvania, Virginia */
	{  51,  145, 4502, 4502, 5576,  1478 },	/* Powhatan, Virginia */
	{  51,  147, 4502, 4502, 5576,  1487 },	/* Prince Edward, Virginia */
	{  51,  149, 4502, 4502, 5576,  1488 },	/* Prince George, Virginia */
	{  51,  151,    0,    0,    0,  1492 },	/* Princess Anne, Virginia */
	{  51,  153, 4501, 4501, 5551,  1490 },	/* Prince William, Virginia */
	{  51,  155, 4502, 4502, 5576,  1497 },	/* Pulaski, Virginia */
	{  51,  157, 4501, 4501, 5551,  1517 },	/* Rappahannock, Virginia */
	{  51,  159, 4502, 4502, 5576,  1540 },	/* Richmond, Virginia */
	{  51,  161, 4502, 4502, 5576,  1552 },	/* Roanoke, Virginia */
	{  51,  163, 4502, 4502, 5576,  1559 },	/* Rockbridge, Virginia */
	{  51,  165, 4501, 4501, 5551,  1562 },	/* Rockingham, Virginia */
	{  51,  167, 4502, 4502, 5576,  1583 },	/* Russell, Virginia */
	{  51,  169, 4502, 4502, 5576,  1647 },	/* Scott, Virginia */
	{  51,  171, 4501, 4501, 5551,  1669 },	/* Shenandoah, Virginia */
	{  51,  173, 4502, 4502, 5576,  1687 },	/* Smyth, Virginia */
	{  51,  175, 4502, 4502, 5576,  1699 },	/* Southampton, Virginia */
	{  51,  177, 4501, 4501, 5551,  1706 },	/* Spotsylvania, Virginia */
	{  51,  179, 4501, 4501, 5551,  1730 },	/* Stafford, Virginia */
	{  51,  181, 4502, 4502, 5576,  1766 },	/* Surry, Virginia */
	{  51,  183, 4502, 4502, 5576,  1768 },	/* Sussex, Virginia */
	{  51,  185, 4502, 4502, 5576,  1792 },	/* Tazewell, Virginia */
	{  51,  187, 4501, 4501, 5551,  1925 },	/* Warren, Virginia */
	{  51,  191, 4502, 4502, 5576,  1933 },	/* Washington, Virginia */
	{  51,  193, 4501, 4501, 5551,  1956 },	/* Westmoreland, Virginia */
	{  51,  195, 4502, 4502, 5576,  1996 },	/* Wise, Virginia */
	{  51,  197, 4502, 4502, 5576,  2014 },	/* Wythe, Virginia */
	{  51,  199, 4502, 4502, 5576,  2033 },	/* York, Virginia */
	{  51,  510,    0,    0,    0,    32 },	/* Alexandria (city), Virginia */
	{  51,  515,    0,    0,    0,   151 },	/* Bedford (city), Virginia */
	{  51,  520,    0,    0,    0,   231 },	/* Bristol (city), Virginia */
	{  51,  530,    0,    0,    0,   248 },	/* Buena Vista (city), Virginia */
	{  51,  540,    0,    0,    0,   338 },	/* Charlottesville (city), Virginia */
	{  51,  550,    0,    0,    0,   353 },	/* Chesapeake (city), Virginia */
	{  51,  560,    0,    0,    0,   393 },	/* Clifton Forge (city), Virginia */
	{  51,  570,    0,    0,    0,   418 },	/* Colonial Heights (city), Virginia */
	{  51,  580,    0,    0,    0,   452 },	/* Covington (city), Virginia */
	{  51,  590,    0,    0,    0,   489 },	/* Danville (city), Virginia */
	{  51,  595,    0,    0,    0,   595 },	/* Emporia (city), Virginia */
	{  51,  600,    0,    0,    0,   610 },	/* Fairfax (city), Virginia */
	{  51,  610,    0,    0,    0,   616 },	/* Falls Church (city), Virginia */
	{  51,  620,    0,    0,    0,   647 },	/* Franklin (city), Virginia */
	{  51,  630,    0,    0,    0,   649 },	/* Fredericksburg (city), Virginia */
	{  51,  640,    0,    0,    0,   661 },	/* Galax (city), Virginia */
	{  51,  650,    0,    0,    0,   770 },	/* Hampton (city), Virginia */
	{  51,  660,    0,    0,    0,   790 },	/* Harrisonburg (city), Virginia */
	{  51,  670,    0,    0,    0,   837 },	/* Hopewell (city), Virginia */
	{  51,  678,    0,    0,    0,  1054 },	/* Lexington (city), Virginia */
	{  51,  680,    0,    0,    0,  1093 },	/* Lynchburg (city), Virginia */
	{  51,  683,    0,    0,    0,  1111 },	/* Manassas (city), Virginia */
	{  51,  685,    0,    0,    0,  1112 },	/* Manassas Park (city), Virginia */
	{  51,  690,    0,    0,    0,  1132 },	/* Martinsville (city), Virginia */
	{  51,  700,    0,    0,    0,  1296 },	/* Newport News (city), Virginia */
	{  51,  710,    0,    0,    0,  1316 },	/* Norfolk (city), Virginia */
	{  51,  720,    0,    0,    0,  1327 },	/* Norton (city), Virginia */
	{  51,  730,    0,    0,    0,  1428 },	/* Petersburg (city), Virginia */
	{  51,  735,    0,    0,    0,  1466 },	/* Poquoson (city), Virginia */
	{  51,  740,    0,    0,    0,  1469 },	/* Portsmouth (city), Virginia */
	{  51,  750,    0,    0,    0,  1507 },	/* Radford (city), Virginia */
	{  51,  760,    0,    0,    0,  1541 },	/* Richmond (city), Virginia */
	{  51,  770,    0,    0,    0,  1553 },	/* Roanoke (city), Virginia */
	{  51,  775,    0,    0,    0,  1596 },	/* Salem (city), Virginia */
	{  51,  780,    0,    0,    0,  1696 },	/* South Boston (city), Virginia */
	{  51,  790,    0,    0,    0,  1738 },	/* Staunton (city), Virginia */
	{  51,  800,    0,    0,    0,  1758 },	/* Suffolk (city), Virginia */
	{  51,  810,    0,    0,    0,  1900 },	/* Virginia Beach (city), Virginia */
	{  51,  820,    0,    0,    0,  1943 },	/* Waynesboro (city), Virginia */
	{  51,  830,    0,    0,    0,  1982 },	/* Williamsburg (city), Virginia */
	{  51,  840,    0,    0,    0,  1985 },	/* Winchester (city), Virginia */
	{  53,    0,    0,    0,    0,  1933 },	/* Washington, Washington */
	{  53,    1, 4602, 4602, 5626,     5 },	/* Adams, Washington */
	{  53,    3, 4602, 4602, 5626,    85 },	/* Asotin, Washington */
	{  53,    5, 4602, 4602, 5626,   163 },	/* Benton, Washington */
	{  53,    7, 4601, 4601, 5601,   348 },	/* Chelan, Washington */
	{  53,    9, 4601, 4601, 5601,   378 },	/* Clallam, Washington */
	{  53,   11, 4602, 4602, 5626,   382 },	/* Clark, Washington */
	{  53,   13, 4602, 4602, 5626,   421 },	/* Columbia, Washington */
	{  53,   15, 4602, 4602, 5626,   455 },	/* Cowlitz, Washington */
	{  53,   17, 4601, 4601, 5601,   546 },	/* Douglas, Washington */
	{  53,   19, 4601, 4601, 5601,   626 },	/* Ferry, Washington */
	{  53,   21, 4602, 4602, 5626,   646 },	/* Franklin, Washington */
	{  53,   23, 4602, 4602, 5626,   666 },	/* Garfield, Washington */
	{  53,   25, 4601, 4601, 5601,   721 },	/* Grant, Washington */
	{  53,   27, 4602, 4602, 5626,   726 },	/* Grays Harbor, Washington */
	{  53,   29, 4601, 4601, 5601,   885 },	/* Island, Washington */
	{  53,   31, 4601, 4601, 5601,   901 },	/* Jefferson, Washington */
	{  53,   33, 4601, 4601, 5601,   963 },	/* King, Washington */
	{  53,   35, 4601, 4601, 5601,   975 },	/* Kitsap, Washington */
	{  53,   37, 4602, 4602, 5626,   976 },	/* Kittitas, Washington */
	{  53,   39, 4602, 4602, 5626,   980 },	/* Klickitat, Washington */
	{  53,   41, 4602, 4602, 5626,  1051 },	/* Lewis, Washington */
	{  53,   43, 4601, 4601, 5601,  1060 },	/* Lincoln, Washington */
	{  53,   45, 4602, 4602, 5626,  1134 },	/* Mason, Washington */
	{  53,   47, 4601, 4601, 5601,  1347 },	/* Okanogan, Washington */
	{  53,   49, 4602, 4602, 5626,  1386 },	/* Pacific, Washington */
	{  53,   51, 4601, 4601, 5601,  1414 },	/* Pend Oreille, Washington */
	{  53,   53, 4602, 4602, 5626,  1438 },	/* Pierce, Washington */
	{  53,   55, 4601, 4601, 5601,  1610 },	/* San Juan, Washington */
	{  53,   57, 4601, 4601, 5601,  1682 },	/* Skagit, Washington */
	{  53,   59, 4602, 4602, 5626,  1684 },	/* Skamania, Washington */
	{  53,   61, 4601, 4601, 5601,  1688 },	/* Snohomish, Washington */
	{  53,   63, 4601, 4601, 5601,  1705 },	/* Spokane, Washington */
	{  53,   65, 4601, 4601, 5601,  1745 },	/* Stevens, Washington */
	{  53,   67, 4602, 4602, 5626,  1806 },	/* Thurston, Washington */
	{  53,   69, 4602, 4602, 5626,  1908 },	/* Wahkiakum, Washington */
	{  53,   71, 4602, 4602, 5626,  1914 },	/* Walla Walla, Washington */
	{  53,   73, 4601, 4601, 5601,  1961 },	/* Whatcom, Washington */
	{  53,   75, 4602, 4602, 5626,  1969 },	/* Whitman, Washington */
	{  53,   77, 4602, 4602, 5626,  2017 },	/* Yakima, Washington */
	{  54,    0,    0,    0,    0,  1953 },	/* West Virgina, West Virgina */
	{  54,    1, 4701, 4701, 5651,   121 },	/* Barbour, West Virgina */
	{  54,    3, 4701, 4701, 5651,   166 },	/* Berkeley, West Virgina */
	{  54,    5, 4702, 4702, 5676,   201 },	/* Boone, West Virgina */
	{  54,    7, 4702, 4702, 5676,   221 },	/* Braxton, West Virgina */
	{  54,    9, 4701, 4701, 5651,   235 },	/* Brooke, West Virgina */
	{  54,   11, 4702, 4702, 5676,   266 },	/* Cabell, West Virgina */
	{  54,   13, 4702, 4702, 5676,   275 },	/* Calhoun, West Virgina */
	{  54,   15, 4702, 4702, 5676,   385 },	/* Clay, West Virgina */
	{  54,   17, 4701, 4701, 5651,   535 },	/* Doddridge, West Virgina */
	{  54,   19, 4702, 4702, 5676,   622 },	/* Fayette, West Virgina */
	{  54,   21, 4702, 4702, 5676,   691 },	/* Gilmer, West Virgina */
	{  54,   23, 4701, 4701, 5651,   721 },	/* Grant, West Virgina */
	{  54,   25, 4702, 4702, 5676,   731 },	/* Greenbrier, West Virgina */
	{  54,   27, 4701, 4701, 5651,   768 },	/* Hampshire, West Virgina */
	{  54,   29, 4701, 4701, 5651,   771 },	/* Hancock, West Virgina */
	{  54,   31, 4701, 4701, 5651,   781 },	/* Hardy, West Virgina */
	{  54,   33, 4701, 4701, 5651,   789 },	/* Harrison, West Virgina */
	{  54,   35, 4702, 4702, 5676,   893 },	/* Jackson, West Virgina */
	{  54,   37, 4701, 4701, 5651,   901 },	/* Jefferson, West Virgina */
	{  54,   39, 4702, 4702, 5676,   929 },	/* Kanawha, West Virgina */
	{  54,   41, 4702, 4702, 5676,  1051 },	/* Lewis, West Virgina */
	{  54,   43, 4702, 4702, 5676,  1060 },	/* Lincoln, West Virgina */
	{  54,   45, 4702, 4702, 5676,  1068 },	/* Logan, West Virgina */
	{  54,   47, 4702, 4702, 5676,  1155 },	/* Mc Dowell, West Virgina */
	{  54,   49, 4701, 4701, 5651,  1125 },	/* Marion, West Virgina */
	{  54,   51, 4701, 4701, 5651,  1129 },	/* Marshall, West Virgina */
	{  54,   53, 4702, 4702, 5676,  1134 },	/* Mason, West Virgina */
	{  54,   55, 4702, 4702, 5676,  1186 },	/* Mercer, West Virgina */
	{  54,   57, 4701, 4701, 5651,  1206 },	/* Mineral, West Virgina */
	{  54,   59, 4702, 4702, 5676,  1207 },	/* Mingo, West Virgina */
	{  54,   61, 4701, 4701, 5651,  1225 },	/* Monongalia, West Virgina */
	{  54,   63, 4702, 4702, 5676,  1226 },	/* Monroe, West Virgina */
	{  54,   65, 4701, 4701, 5651,  1240 },	/* Morgan, West Virgina */
	{  54,   67, 4702, 4702, 5676,  1307 },	/* Nicholas, West Virgina */
	{  54,   69, 4701, 4701, 5651,  1345 },	/* Ohio, West Virgina */
	{  54,   71, 4702, 4702, 5676,  1416 },	/* Pendleton, West Virgina */
	{  54,   73, 4701, 4701, 5651,  1454 },	/* Pleasants, West Virgina */
	{  54,   75, 4702, 4702, 5676,  1457 },	/* Pocahontas, West Virgina */
	{  54,   77, 4701, 4701, 5651,  1485 },	/* Preston, West Virgina */
	{  54,   79, 4702, 4702, 5676,  1499 },	/* Putnam, West Virgina */
	{  54,   81, 4702, 4702, 5676,  1509 },	/* Raleigh, West Virgina */
	{  54,   83, 4702, 4702, 5676,  1513 },	/* Randolph, West Virgina */
	{  54,   85, 4701, 4701, 5651,  1549 },	/* Ritchie, West Virgina */
	{  54,   87, 4702, 4702, 5676,  1551 },	/* Roane, West Virgina */
	{  54,   89, 4702, 4702, 5676,  1761 },	/* Summers, West Virgina */
	{  54,   91, 4701, 4701, 5651,  1791 },	/* Taylor, West Virgina */
	{  54,   93, 4701, 4701, 5651,  1846 },	/* Tucker, West Virgina */
	{  54,   95, 4701, 4701, 5651,  1857 },	/* Tyler, West Virgina */
	{  54,   97, 4702, 4702, 5676,  1868 },	/* Upshur, West Virgina */
	{  54,   99, 4702, 4702, 5676,  1942 },	/* Wayne, West Virgina */
	{  54,  101, 4702, 4702, 5676,  1947 },	/* Webster, West Virgina */
	{  54,  103, 4701, 4701, 5651,  1958 },	/* Wetzel, West Virgina */
	{  54,  105, 4701, 4701, 5651,  1994 },	/* Wirt, West Virgina */
	{  54,  107, 4701, 4701, 5651,  1998 },	/* Wood, West Virgina */
	{  54,  109, 4702, 4702, 5676,  2013 },	/* Wyoming, West Virgina */
	{  55,    0,    0,    0,    0,  1995 },	/* Wisconsin, Wisconsin */
	{  55,    1, 4803, 4803, 5751,     5 },	/* Adams, Wisconsin */
	{  55,    3, 4801, 4801, 5701,    82 },	/* Ashland, Wisconsin */
	{  55,    5, 4802, 4802, 5726,   128 },	/* Barron, Wisconsin */
	{  55,    7, 4801, 4801, 5701,   140 },	/* Bayfield, Wisconsin */
	{  55,    9, 4802, 4802, 5726,   240 },	/* Brown, Wisconsin */
	{  55,   11, 4802, 4802, 5726,   249 },	/* Buffalo, Wisconsin */
	{  55,   13, 4801, 4801, 5701,   260 },	/* Burnett, Wisconsin */
	{  55,   15, 4803, 4803, 5751,   280 },	/* Calumet, Wisconsin */
	{  55,   17, 4802, 4802, 5726,   362 },	/* Chippewa, Wisconsin */
	{  55,   19, 4802, 4802, 5726,   382 },	/* Clark, Wisconsin */
	{  55,   21, 4803, 4803, 5751,   421 },	/* Columbia, Wisconsin */
	{  55,   23, 4803, 4803, 5751,   460 },	/* Crawford, Wisconsin */
	{  55,   25, 4803, 4803, 5751,   487 },	/* Dane, Wisconsin */
	{  55,   27, 4803, 4803, 5751,   536 },	/* Dodge, Wisconsin */
	{  55,   29, 4802, 4802, 5726,   542 },	/* Door, Wisconsin */
	{  55,   31, 4801, 4801, 5701,   546 },	/* Douglas, Wisconsin */
	{  55,   33, 4802, 4802, 5726,   555 },	/* Dunn, Wisconsin */
	{  55,   35, 4802, 4802, 5726,   569 },	/* Eau Claire, Wisconsin */
	{  55,   37, 4801, 4801, 5701,   633 },	/* Florence, Wisconsin */
	{  55,   39, 4803, 4803, 5751,   638 },	/* Fond Du Lac, Wisconsin */
	{  55,   41, 4801, 4801, 5701,   640 },	/* Forest, Wisconsin */
	{  55,   43, 4803, 4803, 5751,   721 },	/* Grant, Wisconsin */
	{  55,   45, 4803, 4803, 5751,   729 },	/* Green, Wisconsin */
	{  55,   47, 4803, 4803, 5751,   730 },	/* Green Lake, Wisconsin */
	{  55,   49, 4803, 4803, 5751,   876 },	/* Iowa, Wisconsin */
	{  55,   51, 4801, 4801, 5701,   879 },	/* Iron, Wisconsin */
	{  55,   53, 4802, 4802, 5726,   893 },	/* Jackson, Wisconsin */
	{  55,   55, 4803, 4803, 5751,   901 },	/* Jefferson, Wisconsin */
	{  55,   57, 4803, 4803, 5751,   923 },	/* Juneau, Wisconsin */
	{  55,   59, 4803, 4803, 5751,   947 },	/* Kenosha, Wisconsin */
	{  55,   61, 4802, 4802, 5726,   956 },	/* Kewaunee, Wisconsin */
	{  55,   63, 4803, 4803, 5751,   991 },	/* La Crosse, Wisconsin */
	{  55,   65, 4803, 4803, 5751,  1002 },	/* Lafayette, Wisconsin */
	{  55,   67, 4802, 4802, 5726,  1016 },	/* Langlade, Wisconsin */
	{  55,   69, 4802, 4802, 5726,  1060 },	/* Lincoln, Wisconsin */
	{  55,   71, 4803, 4803, 5751,  1116 },	/* Manitowoc, Wisconsin */
	{  55,   73, 4802, 4802, 5726,  1118 },	/* Marathon, Wisconsin */
	{  55,   75, 4802, 4802, 5726,  1124 },	/* Marinette, Wisconsin */
	{  55,   77, 4803, 4803, 5751,  1128 },	/* Marquette, Wisconsin */
	{  55,   78, 4802, 4802, 5726,  1184 },	/* Menominee, Wisconsin */
	{  55,   79, 4803, 4803, 5751,  1204 },	/* Milwaukee, Wisconsin */
	{  55,   81, 4803, 4803, 5751,  1226 },	/* Monroe, Wisconsin */
	{  55,   83, 4802, 4802, 5726,  1341 },	/* Oconto, Wisconsin */
	{  55,   85, 4801, 4801, 5701,  1356 },	/* Oneida, Wisconsin */
	{  55,   87, 4802, 4802, 5726,  1378 },	/* Outagamie, Wisconsin */
	{  55,   89, 4803, 4803, 5751,  1385 },	/* Ozaukee, Wisconsin */
	{  55,   91, 4802, 4802, 5726,  1422 },	/* Pepin, Wisconsin */
	{  55,   93, 4802, 4802, 5726,  1438 },	/* Pierce, Wisconsin */
	{  55,   95, 4802, 4802, 5726,  1461 },	/* Polk, Wisconsin */
	{  55,   97, 4802, 4802, 5726,  1467 },	/* Portage, Wisconsin */
	{  55,   99, 4801, 4801, 5701,  1486 },	/* Price, Wisconsin */
	{  55,  101, 4803, 4803, 5751,  1506 },	/* Racine, Wisconsin */
	{  55,  103, 4803, 4803, 5751,  1539 },	/* Richland, Wisconsin */
	{  55,  105, 4803, 4803, 5751,  1557 },	/* Rock, Wisconsin */
	{  55,  107, 4802, 4802, 5726,  1582 },	/* Rusk, Wisconsin */
	{  55,  109, 4802, 4802, 5726,  1710 },	/* St Croix, Wisconsin */
	{  55,  111, 4803, 4803, 5751,  1635 },	/* Sauk, Wisconsin */
	{  55,  113, 4801, 4801, 5701,  1637 },	/* Sawyer, Wisconsin */
	{  55,  115, 4802, 4802, 5726,  1665 },	/* Shawano, Wisconsin */
	{  55,  117, 4803, 4803, 5751,  1667 },	/* Sheboygan, Wisconsin */
	{  55,  119, 4802, 4802, 5726,  1791 },	/* Taylor, Wisconsin */
	{  55,  121, 4802, 4802, 5726,  1836 },	/* Trempealeau, Wisconsin */
	{  55,  123, 4803, 4803, 5751,  1891 },	/* Vernon, Wisconsin */
	{  55,  125, 4801, 4801, 5701,  1895 },	/* Vilas, Wisconsin */
	{  55,  127, 4803, 4803, 5751,  1921 },	/* Walworth, Wisconsin */
	{  55,  129, 4801, 4801, 5701,  1932 },	/* Washburn, Wisconsin */
	{  55,  131, 4803, 4803, 5751,  1933 },	/* Washington, Wisconsin */
	{  55,  133, 4803, 4803, 5751,  1939 },	/* Waukesha, Wisconsin */
	{  55,  135, 4802, 4802, 5726,  1940 },	/* Waupaca, Wisconsin */
	{  55,  137, 4803, 4803, 5751,  1941 },	/* Waushara, Wisconsin */
	{  55,  139, 4803, 4803, 5751,  1990 },	/* Winnebago, Wisconsin */
	{  55,  141, 4802, 4802, 5726,  1998 },	/* Wood, Wisconsin */
	{  56,    0,    0,    0,    0,  2013 },	/* Wyoming, Wyoming */
	{  56,    1, 4901, 4901, 5776,    25 },	/* Albany, Wyoming */
	{  56,    3, 4902, 4902, 5801,   176 },	/* Big Horn, Wyoming */
	{  56,    5, 4901, 4901, 5776,   287 },	/* Campbell, Wyoming */
	{  56,    7, 4902, 4902, 5801,   296 },	/* Carbon, Wyoming */
	{  56,    9, 4901, 4901, 5776,   434 },	/* Converse, Wyoming */
	{  56,   11, 4901, 4901, 5776,   466 },	/* Crook, Wyoming */
	{  56,   13, 4903, 4903, 5826,   652 },	/* Fremont, Wyoming */
	{  56,   15, 4901, 4901, 5776,   709 },	/* Goshen, Wyoming */
	{  56,   17, 4903, 4903, 5826,   842 },	/* Hot Springs, Wyoming */
	{  56,   19, 4902, 4902, 5801,   914 },	/* Johnson, Wyoming */
	{  56,   21, 4901, 4901, 5776,  1019 },	/* Laramie, Wyoming */
	{  56,   23, 4904, 4904, 5851,  1060 },	/* Lincoln, Wyoming */
	{  56,   25, 4902, 4902, 5801,  1272 },	/* Natrona, Wyoming */
	{  56,   27, 4901, 4901, 5776,  1309 },	/* Niobrara, Wyoming */
	{  56,   29, 4903, 4903, 5826,  1395 },	/* Park, Wyoming */
	{  56,   31, 4901, 4901, 5776,  1453 },	/* Platte, Wyoming */
	{  56,   33, 4902, 4902, 5801,  1671 },	/* Sheridan, Wyoming */
	{  56,   35, 4904, 4904, 5851,  1756 },	/* Sublette, Wyoming */
	{  56,   37, 4903, 4903, 5826,  1775 },	/* Sweetwater, Wyoming */
	{  56,   39, 4904, 4904, 5851,  1801 },	/* Teton, Wyoming */
	{  56,   41, 4904, 4904, 5851,  1860 },	/* Uinta, Wyoming */
	{  56,   43, 4902, 4902, 5801,  1931 },	/* Washakie, Wyoming */
	{  56,   45, 4901, 4901, 5776,  1957 },	/* Weston, Wyoming */
	{  56,   47,    0,    0,    0,  2029 },	/* Yellowstone, Wyoming */
	{  60,    0,    0,    0,    0,    46 },	/* American Samoa, American Samoa */
	{  60,   10,    0,    0,    0,   566 },	/* Eastern, American Samoa */
	{  60,   20,    0,    0,    0,  1117 },	/* Manu'a, American Samoa */
	{  60,   30,    0,    0,    0,  1573 },	/* Rose Island, American Samoa */
	{  60,   40,    0,    0,    0,  1773 },	/* Swains Island, American Samoa */
	{  60,   50,    0,    0,    0,  1955 },	/* Western, American Samoa */
	{  64,    0,    0,    0,    0,   623 },	/* Federated States of Micronesia, Federated States of Micronesia */
	{  64,    2,    0,    0,    0,   370 },	/* Chuuk, Federated States of Micronesia */
	{  64,    5,    0,    0,    0,   988 },	/* Kosrae, Federated States of Micronesia */
	{  64,   40,    0,    0,    0,  1458 },	/* Pohnpei, Federated States of Micronesia */
	{  64,   60,    0,    0,    0,  2022 },	/* Yap, Federated States of Micronesia */
	{  66,    0,    0,    0,    0,   675 },	/* Gaum, Gaum */
	{  66,   10,    0,    0,    0,   746 },	/* Guam, Gaum */
	{  68,    0,    0,    0,    0,  1130 },	/* Marshall Islands, Marshall Islands */
	{  68,    7,    0,    0,    0,    13 },	/* Ailinginae, Marshall Islands */
	{  68,   10,    0,    0,    0,    14 },	/* Ailinglaplap, Marshall Islands */
	{  68,   30,    0,    0,    0,    15 },	/* Ailuk, Marshall Islands */
	{  68,   40,    0,    0,    0,    76 },	/* Arno, Marshall Islands */
	{  68,   50,    0,    0,    0,    98 },	/* Aur, Marshall Islands */
	{  68,   60,    0,    0,    0,   178 },	/* Bikar, Marshall Islands */
	{  68,   70,    0,    0,    0,   179 },	/* Bikini, Marshall Islands */
	{  68,   73,    0,    0,    0,   194 },	/* Bokak, Marshall Islands */
	{  68,   80,    0,    0,    0,   570 },	/* Ebon, Marshall Islands */
	{  68,   90,    0,    0,    0,   596 },	/* Enewetak, Marshall Islands */
	{  68,  100,    0,    0,    0,   599 },	/* Erikub, Marshall Islands */
	{  68,  110,    0,    0,    0,   891 },	/* Jabat, Marshall Islands */
	{  68,  120,    0,    0,    0,   894 },	/* Jaluit, Marshall Islands */
	{  68,  130,    0,    0,    0,   903 },	/* Jemo, Marshall Islands */
	{  68,  140,    0,    0,    0,   960 },	/* Kili, Marshall Islands */
	{  68,  150,    0,    0,    0,   990 },	/* Kwajalein, Marshall Islands */
	{  68,  160,    0,    0,    0,  1001 },	/* Lae, Marshall Islands */
	{  68,  170,    0,    0,    0,  1055 },	/* Lib, Marshall Islands */
	{  68,  180,    0,    0,    0,  1058 },	/* Likiep, Marshall Islands */
	{  68,  190,    0,    0,    0,  1108 },	/* Majuro, Marshall Islands */
	{  68,  300,    0,    0,    0,  1110 },	/* Maloelap, Marshall Islands */
	{  68,  310,    0,    0,    0,  1178 },	/* Mejit, Marshall Islands */
	{  68,  320,    0,    0,    0,  1199 },	/* Mili, Marshall Islands */
	{  68,  330,    0,    0,    0,  1262 },	/* Namorik, Marshall Islands */
	{  68,  340,    0,    0,    0,  1263 },	/* Namu, Marshall Islands */
	{  68,  350,    0,    0,    0,  1568 },	/* Rongelap, Marshall Islands */
	{  68,  360,    0,    0,    0,  1569 },	/* Rongrik, Marshall Islands */
	{  68,  385,    0,    0,    0,  1820 },	/* Toke, Marshall Islands */
	{  68,  390,    0,    0,    0,  1862 },	/* Ujae, Marshall Islands */
	{  68,  400,    0,    0,    0,  1863 },	/* Ujelang, Marshall Islands */
	{  68,  410,    0,    0,    0,  1872 },	/* Utrik, Marshall Islands */
	{  68,  420,    0,    0,    0,  2007 },	/* Wotho, Marshall Islands */
	{  68,  430,    0,    0,    0,  2008 },	/* Wotje, Marshall Islands */
	{  69,    0,    0,    0,    0,  1323 },	/* Northern Mariana Islands, Northern Mariana Islands */
	{  69,   85,    0,    0,    0,  1322 },	/* Northern Islands, Northern Mariana Islands */
	{  69,  100,    0,    0,    0,  1577 },	/* Rota, Northern Mariana Islands */
	{  69,  110,    0,    0,    0,  1594 },	/* Saipan, Northern Mariana Islands */
	{  69,  120,    0,    0,    0,  1810 },	/* Tinian, Northern Mariana Islands */
	{  70,    0,    0,    0,    0,  1388 },	/* Palau, Palau */
	{  70,    2,    0,    0,    0,    16 },	/* Aimeliik, Palau */
	{  70,    4,    0,    0,    0,    17 },	/* Airai, Palau */
	{  70,   10,    0,    0,    0,    55 },	/* Angaur, Palau */
	{  70,   50,    0,    0,    0,   797 },	/* Hatobohei, Palau */
	{  70,  100,    0,    0,    0,   938 },	/* Kayangel, Palau */
	{  70,  150,    0,    0,    0,   986 },	/* Koror, Palau */
	{  70,  212,    0,    0,    0,  1179 },	/* Melekeok, Palau */
	{  70,  214,    0,    0,    0,  1299 },	/* Ngaraard, Palau */
	{  70,  218,    0,    0,    0,  1300 },	/* Ngarchelong, Palau */
	{  70,  222,    0,    0,    0,  1301 },	/* Ngardmau, Palau */
	{  70,  224,    0,    0,    0,  1302 },	/* Ngatpang, Palau */
	{  70,  226,    0,    0,    0,  1303 },	/* Ngchesar, Palau */
	{  70,  227,    0,    0,    0,  1304 },	/* Ngeremlengui, Palau */
	{  70,  228,    0,    0,    0,  1305 },	/* Ngiwal, Palau */
	{  70,  350,    0,    0,    0,  1411 },	/* Peleliu, Palau */
	{  70,  370,    0,    0,    0,  1695 },	/* Sonsorol, Palau */
	{  72,    0,    0,    0,    0,  1496 },	/* Puerto Rico, Puerto Rico */
	{  72,    1,    0,    0,    0,     7 },	/* Adjuntas, Puerto Rico */
	{  72,    3,    0,    0,    0,     8 },	/* Aguada, Puerto Rico */
	{  72,    5,    0,    0,    0,     9 },	/* Aguadilla, Puerto Rico */
	{  72,    7,    0,    0,    0,    10 },	/* Aguas Buenas, Puerto Rico */
	{  72,    9,    0,    0,    0,    11 },	/* Aibonito, Puerto Rico */
	{  72,   11,    0,    0,    0,    49 },	/* Anasco, Puerto Rico */
	{  72,   13,    0,    0,    0,    70 },	/* Arecibo, Puerto Rico */
	{  72,   15,    0,    0,    0,    78 },	/* Arroyo, Puerto Rico */
	{  72,   17,    0,    0,    0,   122 },	/* Barceloneta, Puerto Rico */
	{  72,   19,    0,    0,    0,   126 },	/* Barranquitas, Puerto Rico */
	{  72,   21,    0,    0,    0,   139 },	/* Bayamon, Puerto Rico */
	{  72,   23,    0,    0,    0,   267 },	/* Cabo Rojo, Puerto Rico */
	{  72,   25,    0,    0,    0,   270 },	/* Caguas, Puerto Rico */
	{  72,   27,    0,    0,    0,   288 },	/* Camuy, Puerto Rico */
	{  72,   29,    0,    0,    0,   292 },	/* Canovanas, Puerto Rico */
	{  72,   31,    0,    0,    0,   300 },	/* Carolina, Puerto Rico */
	{  72,   33,    0,    0,    0,   315 },	/* Catano, Puerto Rico */
	{  72,   35,    0,    0,    0,   321 },	/* Cayey, Puerto Rico */
	{  72,   37,    0,    0,    0,   325 },	/* Ceiba, Puerto Rico */
	{  72,   39,    0,    0,    0,   371 },	/* Ciales, Puerto Rico */
	{  72,   41,    0,    0,    0,   373 },	/* Cidra, Puerto Rico */
	{  72,   43,    0,    0,    0,   399 },	/* Coamo, Puerto Rico */
	{  72,   45,    0,    0,    0,   427 },	/* Comerio, Puerto Rico */
	{  72,   47,    0,    0,    0,   442 },	/* Corozal, Puerto Rico */
	{  72,   49,    0,    0,    0,   472 },	/* Culebra, Puerto Rico */
	{  72,   51,    0,    0,    0,   543 },	/* Dorado, Puerto Rico */
	{  72,   53,    0,    0,    0,   612 },	/* Fajardo, Puerto Rico */
	{  72,   54,    0,    0,    0,   634 },	/* Florida, Puerto Rico */
	{  72,   55,    0,    0,    0,   747 },	/* Guanica, Puerto Rico */
	{  72,   57,    0,    0,    0,   748 },	/* Guayama, Puerto Rico */
	{  72,   59,    0,    0,    0,   749 },	/* Guayanilla, Puerto Rico */
	{  72,   61,    0,    0,    0,   750 },	/* Guaynabo, Puerto Rico */
	{  72,   63,    0,    0,    0,   755 },	/* Gurabo, Puerto Rico */
	{  72,   65,    0,    0,    0,   796 },	/* Hatillo, Puerto Rico */
	{  72,   67,    0,    0,    0,   839 },	/* Hormigueros, Puerto Rico */
	{  72,   69,    0,    0,    0,   853 },	/* Humacao, Puerto Rico */
	{  72,   71,    0,    0,    0,   882 },	/* Isabela, Puerto Rico */
	{  72,   73,    0,    0,    0,   899 },	/* Jayuya, Puerto Rico */
	{  72,   75,    0,    0,    0,   920 },	/* Juana Diaz, Puerto Rico */
	{  72,   77,    0,    0,    0,   922 },	/* Juncos, Puerto Rico */
	{  72,   79,    0,    0,    0,  1005 },	/* Lajas, Puerto Rico */
	{  72,   81,    0,    0,    0,  1020 },	/* Lares, Puerto Rico */
	{  72,   83,    0,    0,    0,  1024 },	/* Las Marias, Puerto Rico */
	{  72,   85,    0,    0,    0,  1025 },	/* Las Piedras, Puerto Rico */
	{  72,   87,    0,    0,    0,  1069 },	/* Loiza, Puerto Rico */
	{  72,   89,    0,    0,    0,  1089 },	/* Luquillo, Puerto Rico */
	{  72,   91,    0,    0,    0,  1114 },	/* Manati, Puerto Rico */
	{  72,   93,    0,    0,    0,  1120 },	/* Maricao, Puerto Rico */
	{  72,   95,    0,    0,    0,  1141 },	/* Maunabo, Puerto Rico */
	{  72,   97,    0,    0,    0,  1144 },	/* Mayaguez, Puerto Rico */
	{  72,   99,    0,    0,    0,  1217 },	/* Moca, Puerto Rico */
	{  72,  101,    0,    0,    0,  1241 },	/* Morovis, Puerto Rico */
	{  72,  103,    0,    0,    0,  1261 },	/* Naguabo, Puerto Rico */
	{  72,  105,    0,    0,    0,  1268 },	/* Naranjito, Puerto Rico */
	{  72,  107,    0,    0,    0,  1365 },	/* Orocovis, Puerto Rico */
	{  72,  109,    0,    0,    0,  1402 },	/* Patillas, Puerto Rico */
	{  72,  111,    0,    0,    0,  1420 },	/* Penuelas, Puerto Rico */
	{  72,  113,    0,    0,    0,  1462 },	/* Ponce, Puerto Rico */
	{  72,  115,    0,    0,    0,  1501 },	/* Quebradillas, Puerto Rico */
	{  72,  117,    0,    0,    0,  1543 },	/* Rincon, Puerto Rico */
	{  72,  119,    0,    0,    0,  1547 },	/* Rio Grande, Puerto Rico */
	{  72,  121,    0,    0,    0,  1587 },	/* Sabana Grande, Puerto Rico */
	{  72,  123,    0,    0,    0,  1597 },	/* Salinas, Puerto Rico */
	{  72,  125,    0,    0,    0,  1607 },	/* San German, Puerto Rico */
	{  72,  127,    0,    0,    0,  1610 },	/* San Juan, Puerto Rico */
	{  72,  129,    0,    0,    0,  1611 },	/* San Lorenzo, Puerto Rico */
	{  72,  131,    0,    0,    0,  1617 },	/* San Sebastian, Puerto Rico */
	{  72,  133,    0,    0,    0,  1629 },	/* Santa Isabel, Puerto Rico */
	{  72,  135,    0,    0,    0,  1817 },	/* Toa Alta, Puerto Rico */
	{  72,  137,    0,    0,    0,  1818 },	/* Toa Baja, Puerto Rico */
	{  72,  139,    0,    0,    0,  1844 },	/* Trujillo Alto, Puerto Rico */
	{  72,  141,    0,    0,    0,  1873 },	/* Utuado, Puerto Rico */
	{  72,  143,    0,    0,    0,  1884 },	/* Vega Alta, Puerto Rico */
	{  72,  145,    0,    0,    0,  1885 },	/* Vega Baja, Puerto Rico */
	{  72,  147,    0,    0,    0,  1893 },	/* Vieques, Puerto Rico */
	{  72,  149,    0,    0,    0,  1896 },	/* Villalba, Puerto Rico */
	{  72,  151,    0,    0,    0,  2015 },	/* Yabucoa, Puerto Rico */
	{  72,  153,    0,    0,    0,  2024 },	/* Yauco, Puerto Rico */
	{  74,    0,    0,    0,    0,  1859 },	/* U.S. Minor Outlying Islands, U.S. Minor Outlying Islands */
	{  74,   50,    0,    0,    0,   108 },	/* Baker Island, U.S. Minor Outlying Islands */
	{  74,  100,    0,    0,    0,   847 },	/* Howland Island, U.S. Minor Outlying Islands */
	{  74,  150,    0,    0,    0,   896 },	/* Jarvis Island, U.S. Minor Outlying Islands */
	{  74,  200,    0,    0,    0,   916 },	/* Johnston Island, U.S. Minor Outlying Islands */
	{  74,  250,    0,    0,    0,   969 },	/* Kingman Reef, U.S. Minor Outlying Islands */
	{  74,  300,    0,    0,    0,  1196 },	/* Midway Islands, U.S. Minor Outlying Islands */
	{  74,  350,    0,    0,    0,  1275 },	/* Navassa Island, U.S. Minor Outlying Islands */
	{  74,  400,    0,    0,    0,  1390 },	/* Palmyra Atoll, U.S. Minor Outlying Islands */
	{  74,  450,    0,    0,    0,  1910 },	/* Wake Island, U.S. Minor Outlying Islands */
	{  78,    0,    0,    0,    0,  1898 },	/* Virgin Islands of the United States, Virgin Islands of the United States */
	{  78,   10,    0,    0,    0,  1710 },	/* St Croix, Virgin Islands of the United States */
	{  78,   20,    0,    0,    0,  1716 },	/* St John, Virgin Islands of the United States */
	{  78,   30,    0,    0,    0,  1729 },	/* St Thomas, Virgin Islands of the United States */
	{   0,    0,    0,    0,    0,     0 }
};

const char *csFipsNames [2042] =
{
	"Abbeville",
	"Acadia",
	"Accomack",
	"Ada",
	"Adair",
	"Adams",
	"Addison",
	"Adjuntas",
	"Aguada",
	"Aguadilla",
	"Aguas Buenas",
	"Aibonito",
	"Aiken",
	"Ailinginae",
	"Ailinglaplap",
	"Ailuk",
	"Aimeliik",
	"Airai",
	"Aitkin",
	"Alabama",
	"Alachua",
	"Alamance",
	"Alameda",
	"Alamosa",
	"Alaska",
	"Albany",
	"Albemarle",
	"Alcona",
	"Alcorn",
	"Aleutians East",
	"Aleutians West",
	"Alexander",
	"Alexandria (city)",
	"Alfalfa",
	"Alger",
	"Allamakee",
	"Allegan",
	"Allegany",
	"Alleghany",
	"Allegheny",
	"Allen",
	"Allendale",
	"Alpena",
	"Alpine",
	"Amador",
	"Amelia",
	"American Samoa",
	"Amherst",
	"Amite",
	"Anasco",
	"Anchorage",
	"Anderson",
	"Andrew",
	"Andrews",
	"Androscoggin",
	"Angaur",
	"Angelina",
	"Anne Arundel",
	"Anoka",
	"Anson",
	"Antelope",
	"Antrim",
	"Apache",
	"Appanoose",
	"Appling",
	"Appomattox",
	"Aransas",
	"Arapahoe",
	"Archer",
	"Archuleta",
	"Arecibo",
	"Arenac",
	"Arizona",
	"Arkansas",
	"Arlington",
	"Armstrong",
	"Arno",
	"Aroostook",
	"Arroyo",
	"Arthur",
	"Ascension",
	"Ashe",
	"Ashland",
	"Ashley",
	"Ashtabula",
	"Asotin",
	"Assumption",
	"Atascosa",
	"Atchison",
	"Athens",
	"Atkinson",
	"Atlantic",
	"Atoka",
	"Attala",
	"Audrain",
	"Audubon",
	"Auglaize",
	"Augusta",
	"Aur",
	"Aurora",
	"Austin",
	"Autauga",
	"Avery",
	"Avoyelles",
	"Baca",
	"Bacon",
	"Bailey",
	"Baker",
	"Baker Island",
	"Baldwin",
	"Ballard",
	"Baltimore",
	"Baltimore (city)",
	"Baltimore City",
	"Bamberg",
	"Bandera",
	"Banks",
	"Banner",
	"Bannock",
	"Baraga",
	"Barber",
	"Barbour",
	"Barceloneta",
	"Barnes",
	"Barnstable",
	"Barnwell",
	"Barranquitas",
	"Barren",
	"Barron",
	"Barrow",
	"Barry",
	"Bartholomew",
	"Barton",
	"Bartow",
	"Bastrop",
	"Bates",
	"Bath",
	"Baxter",
	"Bay",
	"Bayamon",
	"Bayfield",
	"Baylor",
	"Beadle",
	"Bear Lake",
	"Beaufort",
	"Beauregard",
	"Beaver",
	"Beaverhead",
	"Becker",
	"Beckham",
	"Bedford",
	"Bedford (city)",
	"Bee",
	"Belknap",
	"Bell",
	"Belmont",
	"Beltrami",
	"Ben Hill",
	"Benewah",
	"Bennett",
	"Bennington",
	"Benson",
	"Bent",
	"Benton",
	"Benzie",
	"Bergen",
	"Berkeley",
	"Berks",
	"Berkshire",
	"Bernalillo",
	"Berrien",
	"Bertie",
	"Bethel",
	"Bexar",
	"Bibb",
	"Bienville",
	"Big Horn",
	"Big Stone",
	"Bikar",
	"Bikini",
	"Billings",
	"Bingham",
	"Black Hawk",
	"Blackford",
	"Bladen",
	"Blaine",
	"Blair",
	"Blanco",
	"Bland",
	"Bleckley",
	"Bledsoe",
	"Blount",
	"Blue Earth",
	"Boise",
	"Bokak",
	"Bolivar",
	"Bollinger",
	"Bon Homme",
	"Bond",
	"Bonner",
	"Bonneville",
	"Boone",
	"Borden",
	"Bosque",
	"Bossier",
	"Botetourt",
	"Bottineau",
	"Boulder",
	"Boundary",
	"Bourbon",
	"Bowie",
	"Bowman",
	"Box Butte",
	"Box Elder",
	"Boyd",
	"Boyle",
	"Bracken",
	"Bradford",
	"Bradley",
	"Branch",
	"Brantley",
	"Braxton",
	"Brazoria",
	"Brazos",
	"Breathitt",
	"Breckinridge",
	"Bremer",
	"Brevard",
	"Brewster",
	"Briscoe",
	"Bristol",
	"Bristol (city)",
	"Bristol Bay",
	"Broadwater",
	"Bronx",
	"Brooke",
	"Brookings",
	"Brooks",
	"Broome",
	"Broward",
	"Brown",
	"Brule",
	"Brunswick",
	"Bryan",
	"Buchanan",
	"Buckingham",
	"Bucks",
	"Buena Vista",
	"Buena Vista (city)",
	"Buffalo",
	"Bullitt",
	"Bulloch",
	"Bullock",
	"Buncombe",
	"Bureau",
	"Burke",
	"Burleigh",
	"Burleson",
	"Burlington",
	"Burnet",
	"Burnett",
	"Burt",
	"Butler",
	"Butte",
	"Butts",
	"Cabarrus",
	"Cabell",
	"Cabo Rojo",
	"Cache",
	"Caddo",
	"Caguas",
	"Calaveras",
	"Calcasieu",
	"Caldwell",
	"Caledonia",
	"Calhoun",
	"California",
	"Callahan",
	"Callaway",
	"Calloway",
	"Calumet",
	"Calvert",
	"Camas",
	"Cambria",
	"Camden",
	"Cameron",
	"Camp",
	"Campbell",
	"Camuy",
	"Canadian",
	"Candler",
	"Cannon",
	"Canovanas",
	"Canyon",
	"Cape Girardeau",
	"Cape May",
	"Carbon",
	"Caribou",
	"Carlisle",
	"Carlton",
	"Carolina",
	"Caroline",
	"Carroll",
	"Carson",
	"Carson City",
	"Carter",
	"Carteret",
	"Carver",
	"Cascade",
	"Casey",
	"Cass",
	"Cassia",
	"Castro",
	"Caswell",
	"Catahoula",
	"Catano",
	"Catawba",
	"Catoosa",
	"Catron",
	"Cattaraugus",
	"Cavalier",
	"Cayey",
	"Cayuga",
	"Cecil",
	"Cedar",
	"Ceiba",
	"Centre",
	"Cerro Gordo",
	"Chaffee",
	"Chambers",
	"Champaign",
	"Chariton",
	"Charles",
	"Charles City",
	"Charles Mix",
	"Charleston",
	"Charlevoix",
	"Charlotte",
	"Charlottesville (city)",
	"Charlton",
	"Chase",
	"Chatham",
	"Chattahoochee",
	"Chattooga",
	"Chautauqua",
	"Chaves",
	"Cheatham",
	"Cheboygan",
	"Chelan",
	"Chemung",
	"Chenango",
	"Cherokee",
	"Cherry",
	"Chesapeake (city)",
	"Cheshire",
	"Chester",
	"Chesterfield",
	"Cheyenne",
	"Chickasaw",
	"Chicot",
	"Childress",
	"Chilton",
	"Chippewa",
	"Chisago",
	"Chittenden",
	"Choctaw",
	"Chouteau",
	"Chowan",
	"Christian",
	"Churchill",
	"Chuuk",
	"Ciales",
	"Cibola",
	"Cidra",
	"Cimarron",
	"Citrus",
	"Clackamas",
	"Claiborne",
	"Clallam",
	"Clare",
	"Clarendon",
	"Clarion",
	"Clark",
	"Clarke",
	"Clatsop",
	"Clay",
	"Clayton",
	"Clear Creek",
	"Clearfield",
	"Clearwater",
	"Cleburne",
	"Clermont",
	"Cleveland",
	"Clifton Forge (city)",
	"Clinch",
	"Clinton",
	"Cloud",
	"Coahoma",
	"Coal",
	"Coamo",
	"Cobb",
	"Cochise",
	"Cochran",
	"Cocke",
	"Coconino",
	"Codington",
	"Coffee",
	"Coffey",
	"Coke",
	"Colbert",
	"Cole",
	"Coleman",
	"Coles",
	"Colfax",
	"Colleton",
	"Collier",
	"Collin",
	"Collingsworth",
	"Colonial Heights (city)",
	"Colorado",
	"Colquitt",
	"Columbia",
	"Columbiana",
	"Columbus",
	"Colusa",
	"Comal",
	"Comanche",
	"Comerio",
	"Concho",
	"Concordia",
	"Conecuh",
	"Conejos",
	"Connecticut",
	"Contra Costa",
	"Converse",
	"Conway",
	"Cook",
	"Cooke",
	"Cooper",
	"Coos",
	"Coosa",
	"Copiah",
	"Corozal",
	"Corson",
	"Cortland",
	"Coryell",
	"Coshocton",
	"Costilla",
	"Cottle",
	"Cotton",
	"Cottonwood",
	"Covington",
	"Covington (city)",
	"Coweta",
	"Cowley",
	"Cowlitz",
	"Craig",
	"Craighead",
	"Crane",
	"Craven",
	"Crawford",
	"Creek",
	"Crenshaw",
	"Crisp",
	"Crittenden",
	"Crockett",
	"Crook",
	"Crosby",
	"Cross",
	"Crow Wing",
	"Crowley",
	"Culberson",
	"Culebra",
	"Cullman",
	"Culpeper",
	"Cumberland",
	"Cuming",
	"Currituck",
	"Curry",
	"Custer",
	"Cuyahoga",
	"Dade",
	"Daggett",
	"Dakota",
	"Dale",
	"Dallam",
	"Dallas",
	"Dane",
	"Daniels",
	"Danville (city)",
	"Dare",
	"Darke",
	"Darlington",
	"Dauphin",
	"Davidson",
	"Davie",
	"Daviess",
	"Davis",
	"Davison",
	"Dawes",
	"Dawson",
	"Day",
	"De Baca",
	"De Kalb",
	"De Soto",
	"De Witt",
	"Deaf Smith",
	"Dearborn",
	"Decatur",
	"Deer Lodge",
	"Defiance",
	"Del Norte",
	"Delaware",
	"Delta",
	"Dent",
	"Denton",
	"Denver",
	"Des Moines",
	"Deschutes",
	"Desha",
	"Deuel",
	"Dewey",
	"Dickens",
	"Dickenson",
	"Dickey",
	"Dickinson",
	"Dickson",
	"Dillingham",
	"Dillon",
	"Dimmit",
	"Dinwiddie",
	"District of Columbia",
	"Divide",
	"Dixie",
	"Dixon",
	"Doddridge",
	"Dodge",
	"Dolores",
	"Dona Ana",
	"Doniphan",
	"Donley",
	"Dooly",
	"Door",
	"Dorado",
	"Dorchester",
	"Dougherty",
	"Douglas",
	"Drew",
	"Du Page",
	"Dubois",
	"Dubuque",
	"Duchesne",
	"Dukes",
	"Dundy",
	"Dunklin",
	"Dunn",
	"Duplin",
	"Durham",
	"Dutchess",
	"Duval",
	"Dyer",
	"Eagle",
	"Early",
	"East Baton Rouge",
	"East Carroll",
	"East Feliciana",
	"Eastern",
	"Eastland",
	"Eaton",
	"Eau Claire",
	"Ebon",
	"Echols",
	"Ector",
	"Eddy",
	"Edgar",
	"Edgecombe",
	"Edgefield",
	"Edmonson",
	"Edmunds",
	"Edwards",
	"Effingham",
	"El Dorado",
	"El Paso",
	"Elbert",
	"Elk",
	"Elkhart",
	"Elko",
	"Elliott",
	"Ellis",
	"Ellsworth",
	"Elmore",
	"Emanuel",
	"Emery",
	"Emmet",
	"Emmons",
	"Emporia (city)",
	"Enewetak",
	"Erath",
	"Erie",
	"Erikub",
	"Escambia",
	"Esmeralda",
	"Essex",
	"Estill",
	"Etowah",
	"Eureka",
	"Evangeline",
	"Evans",
	"Fairbanks North Star",
	"Fairfax",
	"Fairfax (city)",
	"Fairfield",
	"Fajardo",
	"Fall River",
	"Fallon",
	"Falls",
	"Falls Church (city)",
	"Fannin",
	"Faribault",
	"Faulk",
	"Faulkner",
	"Fauquier",
	"Fayette",
	"Federated States of Micronesia",
	"Fentress",
	"Fergus",
	"Ferry",
	"Fillmore",
	"Finney",
	"Fisher",
	"Flagler",
	"Flathead",
	"Fleming",
	"Florence",
	"Florida",
	"Floyd",
	"Fluvanna",
	"Foard",
	"Fond Du Lac",
	"Ford",
	"Forest",
	"Forrest",
	"Forsyth",
	"Fort Bend",
	"Foster",
	"Fountain",
	"Franklin",
	"Franklin (city)",
	"Frederick",
	"Fredericksburg (city)",
	"Freeborn",
	"Freestone",
	"Fremont",
	"Fresno",
	"Frio",
	"Frontier",
	"Fulton",
	"Furnas",
	"Gadsden",
	"Gage",
	"Gaines",
	"Galax (city)",
	"Gallatin",
	"Gallia",
	"Galveston",
	"Garden",
	"Garfield",
	"Garland",
	"Garrard",
	"Garrett",
	"Garvin",
	"Garza",
	"Gasconade",
	"Gaston",
	"Gates",
	"Gaum",
	"Geary",
	"Geauga",
	"Gem",
	"Genesee",
	"Geneva",
	"Gentry",
	"George",
	"Georgetown",
	"Georgia",
	"Gibson",
	"Gila",
	"Gilchrist",
	"Giles",
	"Gillespie",
	"Gilliam",
	"Gilmer",
	"Gilpin",
	"Glacier",
	"Glades",
	"Gladwin",
	"Glascock",
	"Glasscock",
	"Glenn",
	"Gloucester",
	"Glynn",
	"Gogebic",
	"Golden Valley",
	"Goliad",
	"Gonzales",
	"Goochland",
	"Goodhue",
	"Gooding",
	"Gordon",
	"Goshen",
	"Gosper",
	"Gove",
	"Grady",
	"Grafton",
	"Graham",
	"Grainger",
	"Grand",
	"Grand Forks",
	"Grand Isle",
	"Grand Traverse",
	"Granite",
	"Grant",
	"Granville",
	"Gratiot",
	"Graves",
	"Gray",
	"Grays Harbor",
	"Grayson",
	"Greeley",
	"Green",
	"Green Lake",
	"Greenbrier",
	"Greene",
	"Greenlee",
	"Greensville",
	"Greenup",
	"Greenville",
	"Greenwood",
	"Greer",
	"Gregg",
	"Gregory",
	"Grenada",
	"Griggs",
	"Grimes",
	"Grundy",
	"Guadalupe",
	"Guam",
	"Guanica",
	"Guayama",
	"Guayanilla",
	"Guaynabo",
	"Guernsey",
	"Guilford",
	"Gulf",
	"Gunnison",
	"Gurabo",
	"Guthrie",
	"Gwinnett",
	"Haakon",
	"Habersham",
	"Haines",
	"Hale",
	"Halifax",
	"Hall",
	"Hamblen",
	"Hamilton",
	"Hamlin",
	"Hampden",
	"Hampshire",
	"Hampton",
	"Hampton (city)",
	"Hancock",
	"Hand",
	"Hanover",
	"Hansford",
	"Hanson",
	"Haralson",
	"Hardee",
	"Hardeman",
	"Hardin",
	"Harding",
	"Hardy",
	"Harford",
	"Harlan",
	"Harmon",
	"Harnett",
	"Harney",
	"Harper",
	"Harris",
	"Harrison",
	"Harrisonburg (city)",
	"Hart",
	"Hartford",
	"Hartley",
	"Harvey",
	"Haskell",
	"Hatillo",
	"Hatobohei",
	"Hawaii",
	"Hawkins",
	"Hayes",
	"Hays",
	"Haywood",
	"Heard",
	"Hemphill",
	"Hempstead",
	"Henderson",
	"Hendricks",
	"Hendry",
	"Hennepin",
	"Henrico",
	"Henry",
	"Herkimer",
	"Hernando",
	"Hertford",
	"Hettinger",
	"Hickman",
	"Hickory",
	"Hidalgo",
	"Highland",
	"Highlands",
	"Hill",
	"Hillsborough",
	"Hillsdale",
	"Hinds",
	"Hinsdale",
	"Hitchcock",
	"Hocking",
	"Hockley",
	"Hodgeman",
	"Hoke",
	"Holmes",
	"Holt",
	"Honolulu",
	"Hood",
	"Hood River",
	"Hooker",
	"Hopewell (city)",
	"Hopkins",
	"Hormigueros",
	"Horry",
	"Hot Spring",
	"Hot Springs",
	"Houghton",
	"Houston",
	"Howard",
	"Howell",
	"Howland Island",
	"Hubbard",
	"Hudson",
	"Hudspeth",
	"Huerfano",
	"Hughes",
	"Humacao",
	"Humboldt",
	"Humphreys",
	"Hunt",
	"Hunterdon",
	"Huntingdon",
	"Huntington",
	"Huron",
	"Hutchinson",
	"Hyde",
	"Iberia",
	"Iberville",
	"Ida",
	"Idaho",
	"Illinois",
	"Imperial",
	"Independence",
	"Indian River",
	"Indiana",
	"Ingham",
	"Inyo",
	"Ionia",
	"Iosco",
	"Iowa",
	"Iredell",
	"Irion",
	"Iron",
	"Iroquois",
	"Irwin",
	"Isabela",
	"Isabella",
	"Isanti",
	"Island",
	"Isle of Wight",
	"Issaquena",
	"Itasca",
	"Itawamba",
	"Izard",
	"Jabat",
	"Jack",
	"Jackson",
	"Jaluit",
	"James City",
	"Jarvis Island",
	"Jasper",
	"Jay",
	"Jayuya",
	"Jeff Davis",
	"Jefferson",
	"Jefferson Davis",
	"Jemo",
	"Jenkins",
	"Jennings",
	"Jerauld",
	"Jerome",
	"Jersey",
	"Jessamine",
	"Jewell",
	"Jim Hogg",
	"Jim Wells",
	"Jo Daviess",
	"Johnson",
	"Johnston",
	"Johnston Island",
	"Jones",
	"Josephine",
	"Juab",
	"Juana Diaz",
	"Judith Basin",
	"Juncos",
	"Juneau",
	"Juniata",
	"Kalamazoo",
	"Kalawao",
	"Kalkaska",
	"Kanabec",
	"Kanawha",
	"Kandiyohi",
	"Kane",
	"Kankakee",
	"Kansas",
	"Karnes",
	"Kauai",
	"Kaufman",
	"Kay",
	"Kayangel",
	"Kearney",
	"Kearny",
	"Keith",
	"Kemper",
	"Kenai Peninsula",
	"Kendall",
	"Kenedy",
	"Kennebec",
	"Kenosha",
	"Kent",
	"Kenton",
	"Kentucky",
	"Keokuk",
	"Kern",
	"Kerr",
	"Kershaw",
	"Ketchikan Gateway",
	"Kewaunee",
	"Keweenaw",
	"Keya Paha",
	"Kidder",
	"Kili",
	"Kimball",
	"Kimble",
	"King",
	"King George",
	"King William",
	"King and Queen",
	"Kingfisher",
	"Kingman",
	"Kingman Reef",
	"Kings",
	"Kingsbury",
	"Kinney",
	"Kiowa",
	"Kit Carson",
	"Kitsap",
	"Kittitas",
	"Kittson",
	"Klamath",
	"Kleberg",
	"Klickitat",
	"Knott",
	"Knox",
	"Kodiak Island",
	"Koochiching",
	"Kootenai",
	"Koror",
	"Kosciusko",
	"Kosrae",
	"Kossuth",
	"Kwajalein",
	"La Crosse",
	"La Moure",
	"La Paz",
	"La Plata",
	"La Porte",
	"La Salle",
	"Labette",
	"Lac Qui Parle",
	"Lackawanna",
	"Laclede",
	"Lae",
	"Lafayette",
	"Lafourche",
	"Lagrange",
	"Lajas",
	"Lake",
	"Lake and Peninsula",
	"Lake of the Woods",
	"Lamar",
	"Lamb",
	"Lamoille",
	"Lampasas",
	"Lancaster",
	"Lander",
	"Lane",
	"Langlade",
	"Lanier",
	"Lapeer",
	"Laramie",
	"Lares",
	"Larimer",
	"Larue",
	"Las Animas",
	"Las Marias",
	"Las Piedras",
	"Lassen",
	"Latah",
	"Latimer",
	"Lauderdale",
	"Laurel",
	"Laurens",
	"Lavaca",
	"Lawrence",
	"Le Flore",
	"Le Sueur",
	"Lea",
	"Leake",
	"Leavenworth",
	"Lebanon",
	"Lee",
	"Leelanau",
	"Leflore",
	"Lehigh",
	"Lemhi",
	"Lenawee",
	"Lenoir",
	"Leon",
	"Leslie",
	"Letcher",
	"Levy",
	"Lewis",
	"Lewis And Clark",
	"Lexington",
	"Lexington (city)",
	"Lib",
	"Liberty",
	"Licking",
	"Likiep",
	"Limestone",
	"Lincoln",
	"Linn",
	"Lipscomb",
	"Litchfield",
	"Little River",
	"Live Oak",
	"Livingston",
	"Llano",
	"Logan",
	"Loiza",
	"Long",
	"Lonoke",
	"Lorain",
	"Los Alamos",
	"Los Angeles",
	"Loudon",
	"Loudoun",
	"Louisa",
	"Louisiana",
	"Loup",
	"Love",
	"Loving",
	"Lowndes",
	"Lubbock",
	"Lucas",
	"Luce",
	"Lumpkin",
	"Luna",
	"Lunenburg",
	"Luquillo",
	"Luzerne",
	"Lycoming",
	"Lyman",
	"Lynchburg (city)",
	"Lynn",
	"Lyon",
	"Mackinac",
	"Macomb",
	"Macon",
	"Macoupin",
	"Madera",
	"Madison",
	"Magoffin",
	"Mahaska",
	"Mahnomen",
	"Mahoning",
	"Maine",
	"Major",
	"Majuro",
	"Malheur",
	"Maloelap",
	"Manassas (city)",
	"Manassas Park (city)",
	"Manatee",
	"Manati",
	"Manistee",
	"Manitowoc",
	"Manu'a",
	"Marathon",
	"Marengo",
	"Maricao",
	"Maricopa",
	"Maries",
	"Marin",
	"Marinette",
	"Marion",
	"Mariposa",
	"Marlboro",
	"Marquette",
	"Marshall",
	"Marshall Islands",
	"Martin",
	"Martinsville (city)",
	"Maryland",
	"Mason",
	"Massac",
	"Massachusetts",
	"Matagorda",
	"Matanuska-Susitna",
	"Mathews",
	"Maui",
	"Maunabo",
	"Maury",
	"Maverick",
	"Mayaguez",
	"Mayes",
	"Mc Clain",
	"Mc Cook",
	"Mc Cormick",
	"Mc Cracken",
	"Mc Creary",
	"Mc Culloch",
	"Mc Curtain",
	"Mc Donald",
	"Mc Donough",
	"Mc Dowell",
	"Mc Duffie",
	"Mc Henry",
	"Mc Intosh",
	"Mc Kean",
	"Mc Kenzie",
	"Mc Lean",
	"Mc Lennan",
	"Mc Leod",
	"Mc Minn",
	"Mc Mullen",
	"Mc Nairy",
	"Mc Pherson",
	"McCone",
	"McKinley",
	"McPherson",
	"Meade",
	"Meagher",
	"Mecklenburg",
	"Mecosta",
	"Medina",
	"Meeker",
	"Meigs",
	"Mejit",
	"Melekeok",
	"Mellette",
	"Menard",
	"Mendocino",
	"Menifee",
	"Menominee",
	"Merced",
	"Mercer",
	"Meriwether",
	"Merrick",
	"Merrimack",
	"Mesa",
	"Metcalfe",
	"Miami",
	"Michigan",
	"Middlesex",
	"Midland",
	"Midway Islands",
	"Mifflin",
	"Milam",
	"Mili",
	"Millard",
	"Mille Lacs",
	"Miller",
	"Mills",
	"Milwaukee",
	"Miner",
	"Mineral",
	"Mingo",
	"Minidoka",
	"Minnehaha",
	"Minnesota",
	"Missaukee",
	"Mississippi",
	"Missoula",
	"Missouri",
	"Mitchell",
	"Mobile",
	"Moca",
	"Modoc",
	"Moffat",
	"Mohave",
	"Moniteau",
	"Monmouth",
	"Mono",
	"Monona",
	"Monongalia",
	"Monroe",
	"Montague",
	"Montana",
	"Montcalm",
	"Monterey",
	"Montezuma",
	"Montgomery",
	"Montmorency",
	"Montour",
	"Montrose",
	"Moody",
	"Moore",
	"Mora",
	"Morehouse",
	"Morgan",
	"Morovis",
	"Morrill",
	"Morris",
	"Morrison",
	"Morrow",
	"Morton",
	"Motley",
	"Moultrie",
	"Mountrail",
	"Mower",
	"Muhlenberg",
	"Multnomah",
	"Murray",
	"Muscatine",
	"Muscogee",
	"Muskegon",
	"Muskingum",
	"Muskogee",
	"Musselshell",
	"Nacogdoches",
	"Naguabo",
	"Namorik",
	"Namu",
	"Nance",
	"Nansemond",
	"Nantucket",
	"Napa",
	"Naranjito",
	"Nash",
	"Nassau",
	"Natchitoches",
	"Natrona",
	"Navajo",
	"Navarro",
	"Navassa Island",
	"Nebraska",
	"Nelson",
	"Nemaha",
	"Neosho",
	"Neshoba",
	"Ness",
	"Nevada",
	"New Castle",
	"New Hampshire",
	"New Hanover",
	"New Haven",
	"New Jersey",
	"New Kent",
	"New London",
	"New Madrid",
	"New Mexico",
	"New York",
	"Newaygo",
	"Newberry",
	"Newport",
	"Newport News (city)",
	"Newton",
	"Nez Perce",
	"Ngaraard",
	"Ngarchelong",
	"Ngardmau",
	"Ngatpang",
	"Ngchesar",
	"Ngeremlengui",
	"Ngiwal",
	"Niagara",
	"Nicholas",
	"Nicollet",
	"Niobrara",
	"Noble",
	"Nobles",
	"Nodaway",
	"Nolan",
	"Nome",
	"Norfolk",
	"Norfolk (city)",
	"Norman",
	"North Carolina",
	"North Dakota",
	"North Slope",
	"Northampton",
	"Northern Islands",
	"Northern Mariana Islands",
	"Northumberland",
	"Northwest Artic",
	"Norton",
	"Norton (city)",
	"Nottoway",
	"Nowata",
	"Noxubee",
	"Nuckolls",
	"Nueces",
	"Nye",
	"O'Brien",
	"Oakland",
	"Obion",
	"Ocean",
	"Oceana",
	"Ochiltree",
	"Oconee",
	"Oconto",
	"Ogemaw",
	"Ogle",
	"Oglethorpe",
	"Ohio",
	"Okaloosa",
	"Okanogan",
	"Okeechobee",
	"Okfuskee",
	"Oklahoma",
	"Okmulgee",
	"Oktibbeha",
	"Oldham",
	"Oliver",
	"Olmsted",
	"Oneida",
	"Onondaga",
	"Onslow",
	"Ontario",
	"Ontonagon",
	"Orange",
	"Orangeburg",
	"Oregon",
	"Orleans",
	"Orocovis",
	"Osage",
	"Osborne",
	"Osceola",
	"Oscoda",
	"Oswego",
	"Otero",
	"Otoe",
	"Otsego",
	"Ottawa",
	"Otter Tail",
	"Ouachita",
	"Ouray",
	"Outagamie",
	"Overton",
	"Owen",
	"Owsley",
	"Owyhee",
	"Oxford",
	"Ozark",
	"Ozaukee",
	"Pacific",
	"Page",
	"Palau",
	"Palm Beach",
	"Palmyra Atoll",
	"Palo Alto",
	"Palo Pinto",
	"Pamlico",
	"Panola",
	"Park",
	"Parke",
	"Parker",
	"Parmer",
	"Pasco",
	"Pasquotank",
	"Passaic",
	"Patillas",
	"Patrick",
	"Paulding",
	"Pawnee",
	"Payette",
	"Payne",
	"Peach",
	"Pearl River",
	"Pecos",
	"Peleliu",
	"Pembina",
	"Pemiscot",
	"Pend Oreille",
	"Pender",
	"Pendleton",
	"Pennington",
	"Pennsylvania",
	"Penobscot",
	"Penuelas",
	"Peoria",
	"Pepin",
	"Perkins",
	"Perquimans",
	"Perry",
	"Pershing",
	"Person",
	"Petersburg (city)",
	"Petroleum",
	"Pettis",
	"Phelps",
	"Philadelphia",
	"Phillips",
	"Piatt",
	"Pickaway",
	"Pickens",
	"Pickett",
	"Pierce",
	"Pike",
	"Pima",
	"Pinal",
	"Pine",
	"Pinellas",
	"Pipestone",
	"Piscataquis",
	"Pitkin",
	"Pitt",
	"Pittsburg",
	"Pittsylvania",
	"Piute",
	"Placer",
	"Plaquemines",
	"Platte",
	"Pleasants",
	"Plumas",
	"Plymouth",
	"Pocahontas",
	"Pohnpei",
	"Poinsett",
	"Pointe Coupee",
	"Polk",
	"Ponce",
	"Pondera",
	"Pontotoc",
	"Pope",
	"Poquoson (city)",
	"Portage",
	"Porter",
	"Portsmouth (city)",
	"Posey",
	"Pottawatomie",
	"Pottawattamie",
	"Potter",
	"Powder River",
	"Powell",
	"Power",
	"Poweshiek",
	"Powhatan",
	"Prairie",
	"Pratt",
	"Preble",
	"Prentiss",
	"Presidio",
	"Presque Isle",
	"Preston",
	"Price",
	"Prince Edward",
	"Prince George",
	"Prince Georges",
	"Prince William",
	"Prince of Wales-Outer Ketchikan",
	"Princess Anne",
	"Providence",
	"Prowers",
	"Pueblo",
	"Puerto Rico",
	"Pulaski",
	"Pushmataha",
	"Putnam",
	"Quay",
	"Quebradillas",
	"Queen Annes",
	"Queens",
	"Quitman",
	"Rabun",
	"Racine",
	"Radford (city)",
	"Rains",
	"Raleigh",
	"Ralls",
	"Ramsey",
	"Randall",
	"Randolph",
	"Rankin",
	"Ransom",
	"Rapides",
	"Rappahannock",
	"Ravalli",
	"Rawlins",
	"Ray",
	"Reagan",
	"Real",
	"Red Lake",
	"Red River",
	"Red Willow",
	"Redwood",
	"Reeves",
	"Refugio",
	"Reno",
	"Rensselaer",
	"Renville",
	"Republic",
	"Reynolds",
	"Rhea",
	"Rhode Island",
	"Rice",
	"Rich",
	"Richardson",
	"Richland",
	"Richmond",
	"Richmond (city)",
	"Riley",
	"Rincon",
	"Ringgold",
	"Rio Arriba",
	"Rio Blanco",
	"Rio Grande",
	"Ripley",
	"Ritchie",
	"Riverside",
	"Roane",
	"Roanoke",
	"Roanoke (city)",
	"Roberts",
	"Robertson",
	"Robeson",
	"Rock",
	"Rock Island",
	"Rockbridge",
	"Rockcastle",
	"Rockdale",
	"Rockingham",
	"Rockland",
	"Rockwall",
	"Roger Mills",
	"Rogers",
	"Rolette",
	"Rongelap",
	"Rongrik",
	"Rooks",
	"Roosevelt",
	"Roscommon",
	"Rose Island",
	"Roseau",
	"Rosebud",
	"Ross",
	"Rota",
	"Routt",
	"Rowan",
	"Runnels",
	"Rush",
	"Rusk",
	"Russell",
	"Rutherford",
	"Rutland",
	"SUmmit",
	"Sabana Grande",
	"Sabine",
	"Sac",
	"Sacramento",
	"Sagadahoc",
	"Saginaw",
	"Saguache",
	"Saipan",
	"Salem",
	"Salem (city)",
	"Salinas",
	"Saline",
	"Salt Lake",
	"Saluda",
	"Sampson",
	"San Augustine",
	"San Benito",
	"San Bernardino",
	"San Diego",
	"San Francisco",
	"San German",
	"San Jacinto",
	"San Joaquin",
	"San Juan",
	"San Lorenzo",
	"San Luis Obispo",
	"San Mateo",
	"San Miguel",
	"San Patricio",
	"San Saba",
	"San Sebastian",
	"Sanborn",
	"Sanders",
	"Sandoval",
	"Sandusky",
	"Sangamon",
	"Sanilac",
	"Sanpete",
	"Santa Barbara",
	"Santa Clara",
	"Santa Cruz",
	"Santa Fe",
	"Santa Isabel",
	"Santa Rosa",
	"Sarasota",
	"Saratoga",
	"Sargent",
	"Sarpy",
	"Sauk",
	"Saunders",
	"Sawyer",
	"Schenectady",
	"Schleicher",
	"Schley",
	"Schoharie",
	"Schoolcraft",
	"Schuyler",
	"Schuylkill",
	"Scioto",
	"Scotland",
	"Scott",
	"Scotts Bluff",
	"Screven",
	"Scurry",
	"Searcy",
	"Sebastian",
	"Sedgwick",
	"Seminole",
	"Seneca",
	"Sequatchie",
	"Sequoyah",
	"Sevier",
	"Seward",
	"Shackelford",
	"Shannon",
	"Sharkey",
	"Sharp",
	"Shasta",
	"Shawano",
	"Shawnee",
	"Sheboygan",
	"Shelby",
	"Shenandoah",
	"Sherburne",
	"Sheridan",
	"Sherman",
	"Shiawassee",
	"Shoshone",
	"Sibley",
	"Sierra",
	"Silver Bow",
	"Simpson",
	"Sioux",
	"Siskiyou",
	"Sitka",
	"Skagit",
	"Skagway-Yakutat-Angoon",
	"Skamania",
	"Slope",
	"Smith",
	"Smyth",
	"Snohomish",
	"Snyder",
	"Socorro",
	"Solano",
	"Somerset",
	"Somervell",
	"Sonoma",
	"Sonsorol",
	"South Boston (city)",
	"South Carolina",
	"South Dakota",
	"Southampton",
	"Southeast Fairbanks",
	"Spalding",
	"Spartanburg",
	"Spencer",
	"Spink",
	"Spokane",
	"Spotsylvania",
	"St Bernard",
	"St Charles",
	"St Clair",
	"St Croix",
	"St Francis",
	"St Francois",
	"St Genevieve",
	"St Helena",
	"St James",
	"St John",
	"St John The Baptist",
	"St Johns",
	"St Joseph",
	"St Landry",
	"St Lawrence",
	"St Louis",
	"St Louis City",
	"St Lucie",
	"St Martin",
	"St Mary",
	"St Marys",
	"St Tammany",
	"St Thomas",
	"Stafford",
	"Stanislaus",
	"Stanley",
	"Stanly",
	"Stanton",
	"Stark",
	"Starke",
	"Starr",
	"Staunton (city)",
	"Stearns",
	"Steele",
	"Stephens",
	"Stephenson",
	"Sterling",
	"Steuben",
	"Stevens",
	"Stewart",
	"Stillwater",
	"Stoddard",
	"Stokes",
	"Stone",
	"Stonewall",
	"Storey",
	"Story",
	"Strafford",
	"Stutsman",
	"Sublette",
	"Suffolk",
	"Suffolk (city)",
	"Sullivan",
	"Sully",
	"Summers",
	"Summit",
	"Sumner",
	"Sumter",
	"Sunflower",
	"Surry",
	"Susquehanna",
	"Sussex",
	"Sutter",
	"Sutton",
	"Suwannee",
	"Swain",
	"Swains Island",
	"Sweet Grass",
	"Sweetwater",
	"Swift",
	"Swisher",
	"Switzerland",
	"Talbot",
	"Taliaferro",
	"Talladega",
	"Tallahatchie",
	"Tallapoosa",
	"Tama",
	"Taney",
	"Tangipahoa",
	"Taos",
	"Tarrant",
	"Tate",
	"Tattnall",
	"Taylor",
	"Tazewell",
	"Tehama",
	"Telfair",
	"Teller",
	"Tennessee",
	"Tensas",
	"Terrebonne",
	"Terrell",
	"Terry",
	"Teton",
	"Texas",
	"Thayer",
	"Thomas",
	"Throckmorton",
	"Thurston",
	"Tift",
	"Tillamook",
	"Tillman",
	"Tinian",
	"Tioga",
	"Tippah",
	"Tippecanoe",
	"Tipton",
	"Tishomingo",
	"Titus",
	"Toa Alta",
	"Toa Baja",
	"Todd",
	"Toke",
	"Tolland",
	"Tom Green",
	"Tompkins",
	"Tooele",
	"Toole",
	"Toombs",
	"Torrance",
	"Towner",
	"Towns",
	"Traill",
	"Transylvania",
	"Traverse",
	"Travis",
	"Treasure",
	"Trego",
	"Trempealeau",
	"Treutlen",
	"Trigg",
	"Trimble",
	"Trinity",
	"Tripp",
	"Troup",
	"Trousdale",
	"Trujillo Alto",
	"Trumbull",
	"Tucker",
	"Tulare",
	"Tulsa",
	"Tunica",
	"Tuolumne",
	"Turner",
	"Tuscaloosa",
	"Tuscarawas",
	"Tuscola",
	"Twiggs",
	"Twin Falls",
	"Tyler",
	"Tyrrell",
	"U.S. Minor Outlying Islands",
	"Uinta",
	"Uintah",
	"Ujae",
	"Ujelang",
	"Ulster",
	"Umatilla",
	"Unicot",
	"Union",
	"Upshur",
	"Upson",
	"Upton",
	"Utah",
	"Utrik",
	"Utuado",
	"Uvalde",
	"Val Verde",
	"Valdez-Cordova",
	"Valencia",
	"Valley",
	"Van Buren",
	"Van Wert",
	"Van Zandt",
	"Vance",
	"Vanderburgh",
	"Vega Alta",
	"Vega Baja",
	"Venango",
	"Ventura",
	"Vermilion",
	"Vermillion",
	"Vermont",
	"Vernon",
	"Victoria",
	"Vieques",
	"Vigo",
	"Vilas",
	"Villalba",
	"Vinton",
	"Virgin Islands of the United States",
	"Virginia",
	"Virginia Beach (city)",
	"Volusia",
	"Wabash",
	"Wabasha",
	"Wabaunsee",
	"Wade Hampton",
	"Wadena",
	"Wagoner",
	"Wahkiakum",
	"Wake",
	"Wake Island",
	"Wakulla",
	"Waldo",
	"Walker",
	"Walla Walla",
	"Wallace",
	"Waller",
	"Wallowa",
	"Walsh",
	"Walthall",
	"Walton",
	"Walworth",
	"Wapello",
	"Ward",
	"Ware",
	"Warren",
	"Warrick",
	"Wasatch",
	"Wasco",
	"Waseca",
	"Washabaugh",
	"Washakie",
	"Washburn",
	"Washington",
	"Washita",
	"Washoe",
	"Washtenaw",
	"Watauga",
	"Watonwan",
	"Waukesha",
	"Waupaca",
	"Waushara",
	"Wayne",
	"Waynesboro (city)",
	"Weakley",
	"Webb",
	"Weber",
	"Webster",
	"Weld",
	"Wells",
	"West Baton Rouge",
	"West Carroll",
	"West Feliciana",
	"West Virgina",
	"Westchester",
	"Western",
	"Westmoreland",
	"Weston",
	"Wetzel",
	"Wexford",
	"Wharton",
	"Whatcom",
	"Wheatland",
	"Wheeler",
	"White",
	"White Pine",
	"Whiteside",
	"Whitfield",
	"Whitley",
	"Whitman",
	"Wibaux",
	"Wichita",
	"Wicomico",
	"Wilbarger",
	"Wilcox",
	"Wilkes",
	"Wilkin",
	"Wilkinson",
	"Will",
	"Willacy",
	"Williams",
	"Williamsburg",
	"Williamsburg (city)",
	"Williamson",
	"Wilson",
	"Winchester (city)",
	"Windham",
	"Windsor",
	"Winkler",
	"Winn",
	"Winnebago",
	"Winneshiek",
	"Winona",
	"Winston",
	"Wirt",
	"Wisconsin",
	"Wise",
	"Wolfe",
	"Wood",
	"Woodbury",
	"Woodford",
	"Woodruff",
	"Woods",
	"Woodson",
	"Woodward",
	"Worcester",
	"Worth",
	"Wotho",
	"Wotje",
	"Wrangell-Petersburg",
	"Wright",
	"Wyandot",
	"Wyandotte",
	"Wyoming",
	"Wythe",
	"Yabucoa",
	"Yadkin",
	"Yakima",
	"Yalobusha",
	"Yamhill",
	"Yancey",
	"Yankton",
	"Yap",
	"Yates",
	"Yauco",
	"Yavapai",
	"Yazoo",
	"Yell",
	"Yellow Medicine",
	"Yellowstone",
	"Yellowstone National Park",
	"Yoakum",
	"Yolo",
	"York",
	"Young",
	"Yuba",
	"Yukon-Koyukuk",
	"Yuma",
	"Zapata",
	"Zavala",
	"Ziebach",
	0
};

/**********************************************************************
	Comparision functions used for the binary search function.
**********************************************************************/
#define BSEARCH_CAST int(*)(const void *,const void *)
static int fipsCompare (const struct csFipsCodes_ *pp,const struct csFipsCodes_ *qq)
{
	int result;

	result = pp->state - qq->state;
	if (result == 0)
	{
		result = pp->county - qq->county;
	}
	return result;
}
static int nameCompare (const char **pp,const char **qq)
{
	return CS_stricmp (*pp,*qq);
}

/**********************************************************************
**	ascii = CSfips2a (state,county);
**
**	int state;					the numeric code value of the state.
**	int county;					the numeric code value for the county.
**	char *ascii;				returns a pointer to a static memory
**								area which contains the name, null string
**								if an error.
**
**	A null string is returned in the event of an error.
**	Set the county code to zero to get the name of a state.
**
**	Call this function with a -1 for state code to have it
**	release memory resources allocated.
**
**	We use const, rather than Const, on this function as it is passed
**	as an argument to the C Run Time Library bsearch function, whose
**	prototype is defined by ANSI.
**
**	Given a state and county FIPS code, the name of the county is
**	returned.  If the county FIPS code argument is zero, this
**	function returns the name of the state identified by the
**	state argument.  NULL is returned in the event of an error.
**	Only error that is possible is that the codes are invalid.
**********************************************************************/

const char* EXP_LVL3 CSfips2a (int state,int county)
{
	const char *result;
	struct csFipsCodes_ *fipsPtr;
	struct csFipsCodes_ search;

	result = NULL;

	/* Binary search the table for a matching code. */

	search.state = (unsigned)state;
	search.county = (unsigned)county;
	fipsPtr = (struct csFipsCodes_ *)CS_bsearch (&search,csFipsCodes,csFipsCount,sizeof (struct csFipsCodes_),(BSEARCH_CAST)fipsCompare);
	if (fipsPtr != NULL) result = csFipsNames [fipsPtr->nameIndex];
	return (result);
}

/******************************************************************************

	Given an index value, returns the state name associated with that index
	value.  If the index value is invalid, null is returned.  Used to
	enumerate entries in the state table.  Note, the index argument is a
	zero based index. */

const char* EXP_LVL3 CS_stateEnum (int index)
{
	int myIndex = -1;
	const char *result = 0;
	const struct csFipsCodes_ *fipsPtr;

	for (fipsPtr = csFipsCodes;fipsPtr->state != 0;fipsPtr += 1)
	{
		if (fipsPtr->county != 0) continue;
		myIndex += 1;
		if (myIndex == index)
		{
			result = csFipsNames [fipsPtr->nameIndex];
			break;
		}
	}
	return result;
}

/******************************************************************************
	Given a state FIPS code and an index value, returns the county name
	associated with that index value.  If the index value is invalid, null is
	returned.  Used to enumerate counties in a particular state.  Note, the
	index argument is a zero based index. */

const char* EXP_LVL3 CS_countyEnum (int stateFips,int index)
{
	int myIndex = -1;
	const char *result = NULL;
	const struct csFipsCodes_ *fipsPtr;

	myIndex = -1;
	for (fipsPtr = csFipsCodes;fipsPtr->state != 0;fipsPtr += 1)
	{
		if (fipsPtr->state != (unsigned)stateFips || fipsPtr->county == 0U) continue;
		myIndex += 1;
		if (myIndex == index)
		{
			result = csFipsNames [fipsPtr->nameIndex];
			break;
		}
	}
	return result;
}

/******************************************************************************
	Given a state name, returns the fips code for the state.  Returns zero
	if the state name is invalid.  Case is ignored for the purpose of this
	function.  The full state name must match to get a result.  Zero is
	returned if the name provided is not that of a state. */

int EXP_LVL3 CS_getStateFips (Const char* stateName)
{
	int result;
	int nameIdx;
	const char **srchPtr;
	const char **namePtr;
	const struct csFipsCodes_ *fipsPtr;

	result = 0;

	/* Binary search the name table for the state name. */

	srchPtr = (const char **)&stateName;							/*lint !e50 */
	namePtr = (const char**)CS_bsearch (srchPtr,csFipsNames,csFipsNameCount,sizeof (const char*),(BSEARCH_CAST)nameCompare);
	if (namePtr != NULL)
	{

		/* Search the FIPS table for an entry that has the correct index value, and
		   also has a zero county fips code (which means its a state name). */

		nameIdx = (int)(namePtr - csFipsNames);
		for (fipsPtr = csFipsCodes;fipsPtr->state != 0;fipsPtr += 1)
		{
			if (fipsPtr->nameIndex == (unsigned)nameIdx && fipsPtr->county == 0)
			{
				result = fipsPtr->state;
				break;
			}
		}
	}
	return (result);
}

/******************************************************************************
	Given a state FIPS code, and a County name, returns the fips code for the
	county.  Returns -1 if the state fips is invalid, 0 if the county name is
	invalid.  Case is ignored for the purpose of this function.  The full
	county name must match to get a result. */

int EXP_LVL3 CS_getCountyFips (int stateFips,Const char* countyName)
{
	int result;
	int nameIdx;
	const char *lclCountyName;
	const char **srchPtr;
	const char **namePtr;
	const struct csFipsCodes_ *fipsPtr;

	result = 0;

	/* Binary search the name table for the county name.  Extra non-sense is used to
	   keep compilers and lint from complaining. */

	lclCountyName = (const char *)countyName;
	srchPtr = &lclCountyName;
	namePtr = (const char**)CS_bsearch (srchPtr,csFipsNames,csFipsNameCount,sizeof (const char*),(BSEARCH_CAST)nameCompare);
	if (namePtr != NULL)
	{
		result = -1;

		/* Search the FIPS table for an entry that has the located index value,
		   and also has a state fips code which matches the supplied state
		   FIPS code. */

		nameIdx = (int)(namePtr - csFipsNames);
		for (fipsPtr = csFipsCodes;fipsPtr->state != 0;fipsPtr += 1)
		{
			if (fipsPtr->nameIndex == (unsigned int)nameIdx && fipsPtr->state == (unsigned int)stateFips)
			{
				result = fipsPtr->county;
				break;
			}
		}
	}
	return (result);
}

/******************************************************************************
	Given a state and county FIPS codes, returns the NAD83 SPCS code for the
	county.  Returns 0 if either the state or county fips codes are invalid. */

int EXP_LVL3 CS_getCountyNad83 (int state,int county)
{
	int result;
	const struct csFipsCodes_ *fipsPtr;

	result = 0;
	for (fipsPtr = csFipsCodes;fipsPtr->state != 0;fipsPtr += 1)
	{
		if (fipsPtr->state == (unsigned)state && fipsPtr->county == (unsigned)county)
		{
			result = fipsPtr->nad83;
			break;
		}
	}
	return result;
}

/******************************************************************************
	Given a state and county FIPS codes, returns the NAD27 SPCS code for the
	county.  Returns 0 if either the state or county fips codes are invalid. */

int EXP_LVL3 CS_getCountyNad27 (int state,int county)
{
	int result;
	const struct csFipsCodes_ *fipsPtr;

	result = 0;
	for (fipsPtr = csFipsCodes;fipsPtr->state != 0;fipsPtr += 1)
	{
		if (fipsPtr->state == (unsigned)state && fipsPtr->county == (unsigned)county)
		{
			result = fipsPtr->nad27;
			break;
		}
	}
	return result;
}
