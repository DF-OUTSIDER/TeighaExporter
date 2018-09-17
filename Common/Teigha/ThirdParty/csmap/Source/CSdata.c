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
#include "cs_Legacy.h"

/*
	The following defines and declarations are used to
	access the various data files required by CS_MAP.

	NOTE:  cs_DirP MUST point to the terminating null
	character of the string contained in the cs_Dir
	array.  cs_Dir MUST end with a directory separation
	character.  If you modify the initialization of
	cs_Dir, YOU MUST ALSO MODIFY THE INITIALIZATION OF
	cs_DirP.
*/

#if _RUN_TIME < _rt_UNIXPCC
/* Rather arbitrary, an CS-MAP Open Source choice. However, there are several
   applications which require access to many of these same files. */
char cs_DirK [MAXPATH] = "C:\\ProgramData\\GeodeticData";
char cs_Dir [MAXPATH] = "";
char *cs_DirP = NULL;
char cs_UserDir [MAXPATH] = "";
char cs_DirsepC = '\\';
char cs_ExtsepC = '.';
char cs_OptchrC = '/';
char cs_EnvchrC = '%';
char cs_EnvStartC = '%';
char cs_EnvEndC = '%';
wchar_t cs_DirsepWC = L'\\';
wchar_t cs_ExtsepWC = L'.';
wchar_t cs_OptchrWC = L'/';
wchar_t cs_EnvchrWC = L'%';
wchar_t cs_EnvStartWC = L'%';
wchar_t cs_EnvEndWC = L'%';
#else
/* Rather arbitrary, an CS-MAP Open Source choice. However, there are several
   applications which require access to many of these same files. */
char cs_DirK [MAXPATH] = "/usr/etc/GeodeticData/";
char cs_Dir [MAXPATH] = "";
char *cs_DirP = NULL;
char cs_UserDir [MAXPATH] = "";
char cs_DirsepC = '/';
char cs_ExtsepC = '.';
char cs_OptchrC = '-';
char cs_EnvchrC = '$';
char cs_EnvStartC = '{';
char cs_EnvEndC = '}';
wchar_t cs_DirsepWC = L'/';
wchar_t cs_ExtsepWC = L'.';
wchar_t cs_OptchrWC = L'-';
wchar_t cs_EnvchrWC = L'$';
wchar_t cs_EnvStartWC = L'{';
wchar_t cs_EnvEndWC = L'}';
#endif
/* This is an 8 bit char on both platforms.  Trac Ticket #174. */
char cs_NameMapperName [MAXPATH] = "NameMapper.csv";

/*
	The following are used in CS_atof and CS_ftoa for the
	conversion of numbers to/from ASCII.  Modify as appropriate
	for the current locale.
*/

char cs_DecpntC = '.';
char cs_DigsepC = ',';
char cs_RatioC  = ':';
char cs_LngDir [6] = "EeWw";
char cs_LatDir [6] = "NnSs";

/*
	The following array contains the set of characters which
	are considered valid for coordinate system, datum, and
	ellipsoid key names. The test occurs in the CS_nampp function.

	Note, as of Release 8, we allow a space character. The
	name preprocessor, CS_nampp, will flag a double space as
	an error, however. Also, as of release 8, we no longer
	convert key names to upper case. However, all comparisions
	remain case Insensitive.

	As of Release 9, all alpha numeric characters are tested by
	hard code in the CS_nampp module.  This string now only
	contains the special characters (i.e. non-alphanumeric)
	which are allowed.  Note that cs_Unique need not be included
	in this string. Include a space in this string if you want to
	support spaces in key names.  CS_nampp will enforce a
	restriction which does not allow two consectuive spaces.
	If the underscore is included in this list, it will be
	considered acceptable as the first alphanumeric in a key name.
*/

Const char cs_Nmchset [32] = " _-$:.;~/";

/*
	As of release 5.0, the names of the Coordinate System, Datums,
	and Elipsoid dictionaries are maintained here. Should your
	application desire to use names other than the standard names,
	you may change them here and recompile or use the CS_csnam,
	CS_dtnam, or CS_elnam functions at runtime.

	Of course, you could just copy to these global variables, but
	the function interface provides a bit more insulation from
	future changes in the system.
*/

char cs_Csname  [cs_FNM_MAXLEN]      = cs_CS_NAME;
char cs_Dtname  [cs_FNM_MAXLEN]      = cs_DAT_NAME;
char cs_Elname  [cs_FNM_MAXLEN]      = cs_ELL_NAME;
char cs_Ctname  [cs_FNM_MAXLEN]      = cs_CT_NAME;
char cs_Gxname  [cs_FNM_MAXLEN]      = cs_GX_NAME;
char cs_Gpname  [cs_FNM_MAXLEN]      = cs_GP_NAME;

char cs_Envvar  [cs_FNM_MAXLEN]      = cs_ENV_VAR;

char cs_GeoidName   [cs_FNM_MAXLEN]  = cs_GEOID_NAME;
char cs_VertconName [cs_FNM_MAXLEN]  = cs_VERTCON_NAME;

/*
	The following array supports the enumerator for GDC file
	names.  This is used in the GDC file editor.  Adding a new
	file to this array will, essentially, add it to the
	GDC File Editor.

	This array is also used for the functions which enable
	applications to change the default names.  While
	applications can modify the global variables directly,
	this is no longer recommended.  When connvenient,
	change your application to use the new name change
	functions.

	Note, that gdcFileNone marks the end of the table.
*/
struct cs_GdcCatalogTable_ cs_GdcCatalogTable [] =
{
	{   gdcFileGeoidHeight, cs_GeoidName  ,	gdcTypeSeparation,       0UL, ".geo.bin.byn.txt.grd","Geoid96 (*.geo)|*.geo|Geoid99 (*.bin)|*.bin|Canadian (*.byn)|*.byn|UK OSGM91 (*.txt)|*.txt|EGM96 (*.grd)|*.grd||"},
	{       gdcFileVertcon, cs_VertconName,	gdcTypeVertical,         0UL, ".94"                 ,"Vertcon files (*.94)|*.94||"               },
	{          gdcFileNone, 0             ,	gdcTypeNone,             0UL, ""                    ,""                                          }
};

/*
	The following arrays are used to process default values
	for datums, ellipsoids, and units.  The value assigned,
	the null string, says that the specific feature is
	inactive.
*/
char csDtDflt [32] = "";	/* The current default datum. */
char csElDflt [32] = "";	/* The current default ellipsoid. */
char csLuDflt [32] = "";	/* The current default linear unit. */
char csAuDflt [32] = "";	/* The current default angular unit. */

/*
	The following are used to control the dictionary protection
	scheme. cs_Unique is a character which must appear in a
	user defined dictionary key name, one which the system
	distributer will never use.  Disable this feature by setting
	cs_Unique to '\0'.

	cs_Protect controls the protection of distribution coordinate
	systems and user defined systems.  Set cs_Protect to -1 to
	disable all protection. Set cs_Protect to 0 to enable
	distribution protection, but disable user coordinate system
	protection.  If cs_Protect is positive and non-zero, it is
	taken to mean that user coordinate systems are protected
	after remaining unaltered for that number of days.
*/

char cs_Unique = ':';
short cs_Protect = 0;
int cs_Safe = 1;

/*
	The following are used to communicate certain information
	between modules without overloading the calling sequence
	of the functions involved.
*/

#if _MEM_MODEL == _mm_VIRTUAL || _MEM_MODEL == _mm_FLAT
int cs_Sortbs = 128 * 1024;
#else
int cs_Sortbs = 24 * 1024;
#endif
int cs_Error = 0;
int cs_Errno = 0;
int csErrlng = 0;
int csErrlat = 0;
unsigned short cs_ErrSup = 0;
#if _RUN_TIME <= _rt_UNIXPCC
ulong32_t cs_Doserr = 0;
#endif

/* The following should be left assigned to zero.  When this variable is
   non-zero, it is assumed to point to a a valid stream, and diagnostic
   information is written to the stream.
*/
csFILE* csDiagnostic = 0;

/*
	The following points are used in the High Performance
	Interface. We define them here, so that CS_init can
	initialize them, without forcing the code of the
	High Performance Interface.
*/

struct csCscach_ *csCscachP = NULL;
int csCscachI = cs_CSCACH_MAX;
struct csDtcach_ *csDtcachP = NULL;
int csDtcachI = cs_DTCACH_MAX;

char *cs_CsKeyNames = NULL;
char *cs_DtKeyNames = NULL;
char *cs_ElKeyNames = NULL;
char *cs_CsLlEnum = NULL;
struct cs_Csgrplst_ *cs_CsGrpList = NULL;

/* Used for high level MGRS access. */
struct cs_Mgrs_ *cs_MgrsPtr = NULL;

/* Used for geocentric coordinate calculations. */
double csGeoCtrErad = 0.0;
double csGeoCtrEsq = 0.0;

/* Note: several functions assume that csErrnam is dimensioned
   at MAXPATH (i.e. a minimum value). */

char csErrnam [MAXPATH] = "<?>";
char csErrmsg [cs_ERRMSG_SIZE] = "<?>";

/* The following carries a pointer to the category list. */
struct cs_Ctdef_* cs_CtDefHead = NULL;

/**********************************************************************
	Hook function pointers.  Set to NULL to deativate.
**********************************************************************/

/*
	The following group of hook functions are called prior to actual
	fetching of the related definition from the dictionaries/tables.
	They have been implemented to support dynamically created (temporary)
	definitions.
*/
int (*CS_usrCsDefPtr)(struct cs_Csdef_ *csDef,Const char *keyName) = NULL;
int (*CS_usrDtDefPtr)(struct cs_Dtdef_ *dtDef,Const char *keyName) = NULL;
int (*CS_usrElDefPtr)(struct cs_Eldef_ *elDef,Const char *keyName) = NULL;
double (*CS_usrUnitPtr)(short type,Const char *unitName) = NULL;
/*
	The following hook functions are called after the definition has been read,
	but before any default processing.
*/
int (*cs_CsHook)(struct cs_Csdef_ *csdef,unsigned short prj_code,ulong32_t prj_flags) = NULL;
int (*cs_DtHook)(struct cs_Dtdef_ *dtdef) = NULL;

/*
	The following constants are used throughout CS_MAP.
	They are declared here, once, to reduce data space
	requirements of the package.  Variables, rather
	than literals, are used to reduce code space and
	improve performance.
*/

Const double cs_Pi       = PI;			/*  3.14159..... */
Const double cs_Mpi      = -PI;			/* -3.14159..... */
Const double cs_Two_pi   = (2.0 * PI);		/*  6.28218..... */
Const double cs_Three_pi = (3.0 * PI);		/*  9.424777.... */
Const double cs_3Pi_o_2  = (3.0 * PI / 2.0);	/*  3 pi over 2  */
Const double cs_Pi_o_2   = PI_OVR_2;		/*  PI / 2.0     */
Const double cs_Mpi_o_2  = -PI_OVR_2;		/*  -PI / 2.0    */
Const double cs_Pi_o_3   = (PI / 3.0);		/*  PI / 3.0     */
Const double cs_Pi_o_4   = PI_OVR_4;		/*  PI / 4.0     */
Const double cs_Mpi_o_4  = -PI_OVR_4;		/*  PI / 4.0     */
Const double cs_Radian   = RADIAN;		/* 57.29577..... */
Const double cs_Degree   = ONE_DEGREE;		/* 1.0 / RADIAN  */
Const double cs_Mone     =  -1.0;			/* -1.0 */
Const double cs_Zero     =   0.0;
Const double cs_Fifth    =   (1.0 / 5.0);
Const double cs_Fourth   =   (1.0 / 4.0);
Const double cs_Third    =   (1.0 / 3.0);
Const double cs_Half     =   0.5;
Const double cs_Mhalf    =  -0.5;
Const double cs_One      =   1.0;
Const double cs_Two      =   2.0;
Const double cs_Three    =   3.0;
Const double cs_Four     =   4.0;
Const double cs_Mfour    =  -4.0;
Const double cs_Five     =   5.0;
Const double cs_Six      =   6.0;
Const double cs_Seven    =   7.0;
Const double cs_Eight    =   8.0;
Const double cs_Nine     =   9.0;
Const double cs_Ten      =  10.0;
Const double cs_K15      =  15.0;
Const double cs_K30      =  30.0;
Const double cs_K45      =  45.0;
Const double cs_Km45     = -45.0;
Const double cs_K60      =  60.0;
Const double cs_K75      =  75.0;
Const double cs_K540     = 540.0;
Const double cs_K360     = 360.0;
Const double cs_K270     = 270.0;
Const double cs_K180     = 180.0;
Const double cs_K90      =  90.0;
Const double cs_K89      =  89.0;
Const double cs_Km360    = -360.0;
Const double cs_Km270    = -270.0;
Const double cs_Km180    = -180.0;
Const double cs_Km90     = -90.0;
Const double cs_Km89     = -89.0;
Const double cs_SclInf   = 9999.99;		/* The value we return for
										   a scale factor when the
										   result actually approaches
										   infinity. */

	/*  The value of cs_Huge and cs_Mhuge are carefully selected to
	    be very large, but not so large as it cannot be used.  Thus,
	    for tan (PI/2), we often use this value and then proceed to
	    square it, multiply by it, divide by it, etc.  Thus, don't make
	    this value too small, but don't make it too big either.  It's
	    possible that this value will be used in cases where floats are
	    used, so we like to keep it around 1.0E+32. */

Const double cs_Huge   = 1.0E+32;	/* Value often returned when
									   the real result would be
									   infinite. */
Const double cs_Mhuge  = -1.0E+32;	/* Negative infinity. */

	/* The following constants are used to check the validity of
	   a definitions following acquisition from disk.

	   Change them here or at run time.  Note, we allow a
	   radius as small as 1.0 to handle the test cases.  Also,
	   some folks use this stuff to map planets and the moon. So,
	   the following numbers are pretty liberal.  You may want
	   to be more conservative. */

double cs_ERadMax = 7.0E+06;
double cs_PRadMax = 7.0E+06;
double cs_ERadMin = 6.0E+06;
double cs_PRadMin = 6.0E+06;

double cs_EccentMax = 0.2;

double cs_DelMax = 50000.0;
double cs_RotMax = 50000.0;

double cs_SclMax = 2000.0;

double cs_SclRedMin = 0.5;
double cs_SclRedMax = 1.5;

double cs_ElevElMin = -25000.0;
double cs_ElevElMax =  50000.0;

	/* The following value is used to test angles for being equal
	   to a specific value, usually 0.0 or 90 degrees.  The value
	   is in radians an corresponds to .001 seconds of arc.  This
	   equates to about 30 centimeters on the earth.  See Synder,
	   page 71.  He suggests 1.E-07.  I feel that 4.85E-08 is
	   close enough to his value, works fine, and is a bit easier
	   to document.
	   
	   This value was established here in release 6.05.  The replacement
	   of the several different literal constants currently used will
	   take place over the next few releases. */
   
Const double cs_AnglTest  = 4.85E-08;
Const double cs_AnglTest1 = (1.0 - 4.85E-08);

	/* cs_ParmTest is similar to cs_AnglTest, but it:
	   1) is in units of degrees;
	   2) is used when checking user entered parameters;
	   3) is a bit larger, representing .1 seconds of arc, about
	      3 meters on the surface of the earth (at the equator).
	      
	   This is the value used to, for example, see if the two
	   standard parallels of a conic are the same.
	*/

Const double cs_ParmTest = 2.78E-05;

/*
	The following values are used to test for proximity to
	the north and south poles, and 90 degrees east and west
	of the central merdian for many projections.  Essentially,
	these values represent +- 90 degrees of latitude, less the
	cs_AnglTest value, in radians.  These constants are designed
	to be consistent with cs_AnglTest, but make checking more
	efficient.
*/

Const double cs_NPTest = ( PI_OVR_2 - 4.85E-08);	/* North Pole TEST value. */
Const double cs_SPTest = (-PI_OVR_2 + 4.85E-08);	/* South Pole TEST value. */
Const double cs_EETest = ( PI_OVR_2 - 4.85E-08);	/* East Extent TEST value. */
Const double cs_WETest = (-PI_OVR_2 + 4.85E-08);	/* West Extent TEST value. */

/*
	The following are used where appropriate to filter out noise
	in the low order bits of double precision latitude and
	longitude numbers.
*/

Const double cs_Micron  = 1.0E-06;
Const double cs_LlNoise = 1.0E-12;


/*
	The following values, all in degrees, are used to test
	coordinate system definition parameters.
*/

Const double cs_MinLng    = -180.00;
Const double cs_MaxLng    =  180.00;
Const double cs_MinLat    = -90.00;
Const double cs_MaxLat    =  90.00;
Const double cs_MinLngFz  = -179.9997222;	/* 1 second greater than -180. */
Const double cs_MaxLngFz  =  179.9997222;	/* 1 second less than 180. */
Const double cs_MinLatFz  = -89.9997222;	/* 1 second less than 90. */
Const double cs_MaxLatFz  =  89.9997222;	/* 1 second greater than -90. */
  
Const double cs_Deg2Sec = 3600.0;			/* Converts degrees to
											   arc seconds. */
Const double cs_Sec2Deg = (1.0 / 3600.0);	/* Converts arc seconds to
											   degrees. */
Const double cs_Min2Deg = (1.0 / 60.0);		/* Converts arc minutes to
											   degrees. */
Const double cs_HlfSecDeg = (0.5 / 3600.0);	/* One half second of arc
											   in degrees. */
Const double cs_Sec2Rad = (1.0 / 3600.0 / RADIAN);/* Converts arc seconds to
												   radians. */
Const double cs_Sin1Sec  = 0.00000484813681107636782;
											/* Sine of one second of arc,
											   also can be used as one
											   arc second in radians. */
Const double cs_NPSecs = (90.0 * 3600.0);	/* Latitude of the north
											   pole in arc seconds. */

Const double cs_Wgs84_aa = 6378137.000;			/* WGS-84 major radius. */
Const double cs_Wgs84_ff = 1.0 / 298.257223563;	/* WGS-84 flattening */
Const double cs_Wgs84_esq = 0.00669437999013;	/* WGS-84 eccentricity
												   squared */
Const double cs_Wgs84_1esq = 0.99330562000987;	/* WGS-84 1.0 - e_sq */

/* The following definition is used in fallback cases only.  That is, where
   extraordinary calculations are being performed.  In all "normal"
   situations, the definition fetched from the dictionary(s) is/are used. */
struct cs_Datum_ cs_Wgs84Def =
{
	"WGS84",				/* datum key name */
	"WGS84",				/* ellipsoid key name */
	6378137.000,			/* equatorial radius */
	6356752.3142,			/* polar radius */
	1.0 / 298.257223563,	/* flattening */
	0.0818191908426,		/* eccentricity */
	0.0,					/* Delta X */
	0.0,					/* Delta Y */
	0.0,					/* Delta Z */
	0.0,					/* Rot X */
	0.0,					/* Rot Y */
	0.0,					/* Rot Z */
	0.0,					/* Scale */
	cs_DTCTYP_WGS84,		/* to84_via */
	"World Geodetic System of 1984",
	"World Geodetic System of 1984"
};
#if !defined(__SUNPRO_C)
	struct cs_Datum_ *cs_Wgs84Ptr = &cs_Wgs84Def;
#endif

/*
	The following are global constants used to define the
	operation of the non-standard cartesian system
	feature of CS_MAP.

	cs_QuadMin & cs_QuadMax are used in the CS_cschk function
	to determine validity of the user supplied value.
	cs_QuadMap is used in the projection setup functions
	to convert the user supplied value to the bit map
	value expected by CS_quadF and cs_quadI.

	Note, the projection setup functions use

	trmer->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	to convert from user specified code to internal code.
*/

Const short cs_QuadMax =  4;
Const short cs_QuadMin = -4;
Const short cs_QuadMap [] =
{
		cs_QUAD_SWAP | cs_QUAD_INVY,				/* -4 */
		cs_QUAD_SWAP | cs_QUAD_INVX | cs_QUAD_INVY,	/* -3 */
		cs_QUAD_SWAP | cs_QUAD_INVX,				/* -2 */
		cs_QUAD_SWAP,								/* -1 */
		0,											/*  0 */
		0,											/*  1 */
		cs_QUAD_INVX,								/*  2 */
		cs_QUAD_INVX | cs_QUAD_INVY,				/*  3 */
		cs_QUAD_INVY								/*  4 */
};
/*
	A quad map for use with south oriented coordinate systems.
*/	
Const short cs_QuadMapSO [] =
{
		cs_QUAD_SWAP | cs_QUAD_INVX,				/* -4 */
		cs_QUAD_SWAP,								/* -3 */
		cs_QUAD_SWAP | cs_QUAD_INVY,				/* -2 */
		cs_QUAD_SWAP | cs_QUAD_INVX | cs_QUAD_INVY,	/* -1 */
		cs_QUAD_INVX | cs_QUAD_INVY,				/*  0 */
		cs_QUAD_INVX | cs_QUAD_INVY,				/*  1 */
		cs_QUAD_INVY,								/*  2 */
		0,											/*  3 */
		cs_QUAD_INVX								/*  4 */
};

/*
	The following table defines the list of currently supported
	Coordinate System groups.  It is suggested that application
	developers resist the temptation to add new groups.  If there
	are too many groups, with too few members, the whole group
	concept loses its effectiveness.  Applications developers
	may, need however, to translate the table to a different
	language.
*/

Const struct cs_Grptbl_ cs_CsGrptbl [] =
{
	{     "SPCS27","State Planes, NAD27 Based, US Foot"           ,cs_GRPTBL_ACTIVE},
	{     "SPCS83","State Planes, NAD83 Based, Meters"            ,cs_GRPTBL_ACTIVE},
	{    "SPCS83F","State Planes, NAD83 Based, US Foot"           ,cs_GRPTBL_ACTIVE},
	{    "SPCS83I","State Planes, NAD83 Based, Intn'l Foot"       ,cs_GRPTBL_ACTIVE},
	{     "SPCSHP","State Planes, HARN (HPGN) Based, Meters"      ,cs_GRPTBL_ACTIVE},
	{    "SPCSHPF","State Planes, HARN (HPGN) Based, US Foot"     ,cs_GRPTBL_ACTIVE},
	{    "SPCSHPI","State Planes, HARN (HPGN) Based, Intn'l Foot" ,cs_GRPTBL_ACTIVE},
	{   "SPNSRS07","State Planes, NSRS 2007 Based, Meters"        ,cs_GRPTBL_ACTIVE},
	{  "SPNSRS07F","State Planes, NSRS 2007 Based, US Foot"       ,cs_GRPTBL_ACTIVE},
	{  "SPNSRS07I","State Planes, NSRS 2007 Based, Intn'l Foot"   ,cs_GRPTBL_ACTIVE},
	{   "SPNSRS11","State Planes, NSRS 2011 Based, Meters"        ,cs_GRPTBL_ACTIVE},
	{  "SPNSRS11F","State Planes, NSRS 2011 Based, US Foot"       ,cs_GRPTBL_ACTIVE},
	{  "SPNSRS11I","State Planes, NSRS 2011 Based, Intn'l Foot"   ,cs_GRPTBL_ACTIVE},
	{         "LL","Latitude/Longitude"                           ,cs_GRPTBL_ACTIVE},
	{      "UTM27","UTM Zones, NAD27 Based, Meters"               ,cs_GRPTBL_ACTIVE},
	{     "UTM27F","UTM Zones, NAD27 Based, US Foot"              ,cs_GRPTBL_ACTIVE},
	{     "UTM27I","UTM Zones, NAD27 Based, Intn'l Foot"          ,cs_GRPTBL_ACTIVE},
	{      "UTM83","UTM Zones, NAD83 Based, Meters"               ,cs_GRPTBL_ACTIVE},
	{     "UTM83F","UTM Zones, NAD83 Based, US Foot"              ,cs_GRPTBL_ACTIVE},
	{     "UTM83I","UTM Zones, NAD83 Based, Intn'l Foot"          ,cs_GRPTBL_ACTIVE},
	{      "UTMHP","UTM Zones, HARN (HPGN) Based, Meters"         ,cs_GRPTBL_ACTIVE},
	{     "UTMHPF","UTM Zones, HARN (HPGN) Based, US Foot"        ,cs_GRPTBL_ACTIVE},
	{     "UTMHPI","UTM Zones, HARN (HPGN) Based, Intn'l Foot"    ,cs_GRPTBL_ACTIVE},
	{  "UTMNSRS07","UTM Zones, NSRS 2007 Based, Meters"           ,cs_GRPTBL_ACTIVE},
	{ "UTMNSRS07F","UTM Zones, NSRS 2007 Based, US Foot"          ,cs_GRPTBL_ACTIVE},
	{ "UTMNSRS07I","UTM Zones, NSRS 2007 Based, Intn'l Foot"      ,cs_GRPTBL_ACTIVE},
	{  "UTMNSRS11","UTM Zones, NSRS 2011 Based, Meters"           ,cs_GRPTBL_ACTIVE},
	{ "UTMNSRS11F","UTM Zones, NSRS 2011 Based, US Foot"          ,cs_GRPTBL_ACTIVE},
	{ "UTMNSRS11I","UTM Zones, NSRS 2011 Based, Intn'l Foot"      ,cs_GRPTBL_ACTIVE},
	{    "OTHR-US","Other US Coordinate Systems"                  ,cs_GRPTBL_ACTIVE},
	{     "CANADA","Canadian Coordinate Systems"                  ,cs_GRPTBL_ACTIVE},
	{    "OTHR-NA","Other North American Coordinate Systems"      ,cs_GRPTBL_ACTIVE},
	{       "UTMN","UTM Zones, Northern Hemisphere, Meters"       ,cs_GRPTBL_ACTIVE},
	{       "UTMS","UTM Zones, Southern Hemisphere, Meters"       ,cs_GRPTBL_ACTIVE},
	{      "WORLD","World/Continent Coordinate Systems"           ,cs_GRPTBL_ACTIVE},
	{     "EUROPE","European Coordinate Systems"                  ,cs_GRPTBL_ACTIVE},
	{      "AUSNZ","Australia/New Zealand Coordinate Systems"     ,cs_GRPTBL_ACTIVE},
	{      "SAMER","South American Coordinate Systems"            ,cs_GRPTBL_ACTIVE},
	{      "CAMER","Central American Coordinate Systems"          ,cs_GRPTBL_ACTIVE},
	{     "AFRICA","African Coordinate Systems"                   ,cs_GRPTBL_ACTIVE},
	{       "ASIA","Asian Coordinate Systems"                     ,cs_GRPTBL_ACTIVE},
	{     "RUSSIA","Russian Coordinate Systems"                   ,cs_GRPTBL_ACTIVE},
	{    "PACIFIC","Other Pacific Coordinate Systems"             ,cs_GRPTBL_ACTIVE},
	{   "ATLANTIC","Other Atlantic Coordinate Systems"            ,cs_GRPTBL_ACTIVE},
	{      "POLAR","Polar Coordinate Systems"                     ,cs_GRPTBL_ACTIVE},
	{    "EPSGPRJ","Projected defs from EPSG, area undetermined"  ,cs_GRPTBL_ACTIVE},
	{     "EPSGLL","Geographic defs from EPSG, area undetermined ",cs_GRPTBL_ACTIVE},
	{       "TEST","Test Coordinate Systems"                      ,cs_GRPTBL_ACTIVE},
	{       "USER","User Defined Coordinate Systems"              ,cs_GRPTBL_ACTIVE},
	{   "WKTSUPPT","Well Known Text Support"                      ,cs_GRPTBL_ACTIVE},
	{      "CARIB","Caribean/Gulf of Mexico and Vicinity"         ,cs_GRPTBL_ACTIVE},
	{    "MIDEAST","Mideast/Holy Land"                            ,cs_GRPTBL_ACTIVE},
	{     "INDIAN","Indian Ocean"                                 ,cs_GRPTBL_ACTIVE},
	{      "NERTH","Non geospatial (non-Earth) transformations"   ,cs_GRPTBL_ACTIVE},
	{       "NONE","No group specified in definition"             ,cs_GRPTBL_ACTIVE},
	{     "LEGACY","Obsolete/Incorrect; for legacy use only"      ,cs_GRPTBL_ACTIVE},
	{           "",""                                             ,cs_GRPTBL_END}
};
