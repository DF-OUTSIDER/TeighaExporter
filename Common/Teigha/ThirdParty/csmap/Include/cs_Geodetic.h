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

/*
	The following are the error control codes which are	used to control how
	the datum conversion routines deal with errors.  There is one set for
	datums, i.e. dealing with unsupported datum changes, and one set for a
	data block which is not found, i.e. required data is not present on the
	system.
*/

#define cs_DTCFLG_DAT_F   0		/* Fatal */
#define cs_DTCFLG_DAT_W   1		/* Warning */
#define cs_DTCFLG_DAT_W1  2		/* Warn Once */
#define cs_DTCFLG_DAT_I   3		/* Ignore */

#define cs_DTCFLG_BLK_F   0		/* Fatal */
#define cs_DTCFLG_BLK_W   1		/* Warning */
#define cs_DTCFLG_BLK_1   2		/* Warn, once per block */
#define cs_DTCFLG_BLK_I   3		/* Ignore */
#define cs_DTCFLG_BLK_10  4		/* Warn up to 10 blocks, then ignore. */

/* Status values used internally in the grid file interpolation system.
   Basically the same values as those used everywhere else in CS-MAP. */
#define csGRIDI_ST_OK 0
#define csGRIDI_ST_COVERAGE  1
#define csGRIDI_ST_FALLBACK  2
#define csGRIDI_ST_SYSTEM   -1

/*
	The following define the extensions of the grid interpolation data
	files.  We used to have different cases for different environments. The
	whole thing is now case insensitive.
*/

#define cs_NADCON_LOS  "LOS"
#define cs_NADCON_LAS  "LAS"
#define cs_HPGN_LOS    "LOS"
#define cs_HPGN_LAS    "LAS"
#define cs_HPGN_TAG    "HPGN"
#define cs_GEOID96_TAG "GEO"
#define cs_VERTCON_TAG "94"
#define cs_CANNT_DAC   "DAC"
#define cs_CANNT_GSB   "GSB"
#define cs_MULREG_EXT  "MRT"

/* The following transformation "methods" are supported by the
   Geodetic Transformation facility.  The methods are grouped by
   a method class; the method class distinguishing the type of
   parameters which are used by the method (i.e. which member
   of the parameter union of the cs_GeodeticTransform_ structure
   applies to that method). */
#define cs_DTCPRMTYP_MASK       0xF000
#define cs_DTCPRMTYP_STANDALONE 0x1000
#define cs_DTCPRMTYP_GEOCTR     0x2000
#define cs_DTCPRMTYP_GRIDINTP   0x3000
#define cs_DTCPRMTYP_MULRG      0x4000
#define cs_DTCPRMTYP_PWRSRS     0x5000

/* Methods of the GRIDFILE parameter type are qualified by
   the file format of the grid file.  These are the currently
   supported grid file formats. */
#define cs_DTCFRMT_NONE     0x00
#define cs_DTCFRMT_CNTv1    0x01
#define cs_DTCFRMT_CNTv2    0x02
#define cs_DTCFRMT_NADCN    0x03
#define cs_DTCFRMT_FRNCH    0x04
#define cs_DTCFRMT_JAPAN    0x05
#define cs_DTCFRMT_ATS77    0x06
#define cs_DTCFRMT_OST97    0x07
#define cs_DTCFRMT_OST02    0x08
#define cs_DTCFRMT_GEOCN    0x09

/* Standalone methods are (i.e. noparameters): */
#define cs_DTCMTH_NULLX       (cs_DTCPRMTYP_STANDALONE + 0x0001)
#define cs_DTCMTH_WGS72       (cs_DTCPRMTYP_STANDALONE + 0x0002)

/* Geocentric Methods Are: */ 
#define cs_DTCMTH_3PARM       (cs_DTCPRMTYP_GEOCTR + 0x0001)
#define cs_DTCMTH_MOLOD       (cs_DTCPRMTYP_GEOCTR + 0x0002)	/* not really geocentric, but close */
#define cs_DTCMTH_AMOLO       (cs_DTCPRMTYP_GEOCTR + 0x0003)	/* not really geocentric, but close */
#define cs_DTCMTH_GEOCT       (cs_DTCPRMTYP_GEOCTR + 0x0004)
#define cs_DTCMTH_4PARM       (cs_DTCPRMTYP_GEOCTR + 0x0005)
#define cs_DTCMTH_6PARM       (cs_DTCPRMTYP_GEOCTR + 0x0006)
#define cs_DTCMTH_BURSA       (cs_DTCPRMTYP_GEOCTR + 0x0007)
#define cs_DTCMTH_FRAME       (cs_DTCPRMTYP_GEOCTR + 0x0008)
#define cs_DTCMTH_7PARM       (cs_DTCPRMTYP_GEOCTR + 0x0009)
#define cs_DTCMTH_BDKAS       (cs_DTCPRMTYP_GEOCTR + 0x000A)

/* Multiple Regression methods are: */
#define cs_DTCMTH_MULRG       (cs_DTCPRMTYP_PWRSRS + 0x0001)
#define cs_DTCMTH_PLYNM       (cs_DTCPRMTYP_PWRSRS + 0x0002)

/* Grid File interpolation methods are: */
#define cs_DTCMTH_GFILE       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_NONE)
#define cs_DTCMTH_CNTv1       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_CNTv1)
#define cs_DTCMTH_CNTv2       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_CNTv2)
#define cs_DTCMTH_NADCN       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_NADCN)
#define cs_DTCMTH_FRNCH       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_FRNCH)
#define cs_DTCMTH_JAPAN       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_JAPAN)
#define cs_DTCMTH_ATS77       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_ATS77)
#define cs_DTCMTH_OST97       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_OST97)
#define cs_DTCMTH_OST02       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_OST02)
#define cs_DTCMTH_GEOCN       (cs_DTCPRMTYP_GRIDINTP + cs_DTCFRMT_GEOCN)

/* For programming convenience: */
#define cs_DTCMTH_NONE        (0x0000)
#define cs_DTCMTH_SKIP        (0x7FFE)
#define cs_DTCMTH_END         (0x7FFF)

#define cs_DTCDIR_NONE        0
#define cs_DTCDIR_FWD         1
#define cs_DTCDIR_INV         2
#define cs_DTCDIR_ERR         999

#define cs_DTCBRG_NONE        0
#define cs_DTCBRG_BUILDING    1
#define cs_DTCBRG_COMPLETE    2
#define cs_DTCBRG_NOTUNIQUE   998
#define cs_DTCBRG_ERROR       999

#define cs_PATHDIR_FWD 0
#define cs_PATHDIR_INV 1

#define cs_GXIDX_NOXFRM    -1
#define cs_GXIDX_DUPXFRM   -2
#define cs_GXIDX_ERROR     -3

/* The following are essentially bits used to add specific properties to
   specific transformations and/or their variations.  Curently, we have
   only one such property: that is being reentrant.  Quite likely to be
   more before this code is sent to code heaven. */
  
#define cs_XFRMFLG_NONE    0x0000UL
#define cs_XFRMFLG_RNTRNT  0x0001UL				/* (1UL << 0) */


#define cs_FRMTFLG_NONE    0x0000UL
#define cs_FRMTFLG_RNTRNT  0x0001UL				/* (1UL << 0) */

#define cs_XFRMFLGS_NULLX  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_WGS72  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_MOLOD  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_AMOLO  cs_XFRMFLG_NONE			/* Not iumplemented, don't know yet */
#define cs_XFRMFLGS_GEOCT  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_3PARM  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_4PARM  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_6PARM  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_BURSA  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_FRAME  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_7PARM  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_BDKAS  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_MULRG  cs_XFRMFLG_RNTRNT
#define cs_XFRMFLGS_PLYNM  cs_XFRMFLG_NONE			/* Not implemented, don't know yet */
#define cs_XFRMFLGS_GFILE  cs_XFRMFLG_NONE			/* Depends upon the file format. */

#define cs_FRMTFLGS_CNTv1  cs_FRMTFLG_NONE			/* Obsolete, probably always be non-reentrant. */
#define cs_FRMTFLGS_CNTv2  cs_FRMTFLG_NONE			/* Need to examine buffer use */
#define cs_FRMTFLGS_NADCN  cs_FRMTFLG_RNTRNT		/* Need to remove the local buffer */
#define cs_FRMTFLGS_FRNCH  cs_FRMTFLG_RNTRNT		/* Data grids are memory resident upon construction */
#define cs_FRMTFLGS_JAPAN  cs_FRMTFLG_RNTRNT		/* Data grids are memory resident upon construction */
#define cs_FRMTFLGS_ATS77  cs_FRMTFLG_NONE			/* Don't really know about this one. */
#define cs_FRMTFLGS_OST97  cs_FRMTFLG_NONE			/* Not implemented yet as a geodetic transformation, which it should be. */
#define cs_FRMTFLGS_OST02  cs_FRMTFLG_NONE			/* Not implemented yet as a geodetic transformation, which it should be. */
#define cs_FRMTFLGS_GEOCN  cs_FRMTFLG_NONE			/* Files are typically 28MB in binary form; unlikely to be made reentrant. */

/*
	The following defines define possible values of the bit mapped
	argument to the CS_gxchk function.  These options make it
	possible to use this function in a variety of different
	ways.  The Coordinate System compiler environment, where the
	datums and ellipsoids need to be checked against the dictionaries
	being generated (not necessarily the currently active one) is the
	primary reason for all of this.
*/

#define cs_GXCHK_DATUM	1	/* Turns on checking of the datum key
							   name against the currently active
							   datum dictionary. */
#define cs_GXCHK_REPORT 2	/* Instructs CS_gxchk to report all
							   errors to CS_erpt. */

/*
	The following defines define possible values of the bit mapped
	argument to the CS_gpchk function.  These options make it
	possible to use this function in a variety of different
	ways.  The Coordinate System compiler environment, where the
	geodetic transforms and geodetic paths need to be checked against
	the dictionaries being generated (not necessarily the currently
	active ones) is the	primary reason for all of this.
*/

#define cs_GPCHK_DATUM	1	/* Turns on checking of the source and target
							   datum key names against the currently active
							   datum dictionary. */
#define cs_GPCHK_XFORM	2	/* Turns on checking of each transformation key
							   name against the currently active geodetic
							   transformation dictionary. */
#define cs_GPCHK_REPORT 4	/* Instructs CS_gpchk to report all
							   errors to CS_erpt. */


#define csGRIDI1_FILEMAX 50
#define csGRIDI1_FLNMSZ  238

/* TODO I don't think we really need this. */

enum csDtCvtType {	dtcTypeNone = 0,
					dtcTypeUS,
					dtcTypeHarn,
					dtcTypeCanadian1,
					dtcTypeCanadian2,
					dtcTypeAustralian,
					dtcTypeGeoid96,
					dtcTypeUSVertcon,
					dtcTypeJapanese,
					dtcTypeFrench
				 };

/*
	An enumerator which enumerates all different grid interpolation file 
	formats.  Generally, we have a set of functions which handles each of
	the differing formats.  This enumerator is used only for horizontal
	geodetic transformations (although some of these may include the vertical).
	Some of these formats are used for other purposes, such as vertical
	datum shift and geoid height.  That does not concern us here.*/
enum csGridFormat {	gridFrmtNone = 0,
					gridFrmtNTv1,
					gridFrmtNTv2,
					gridFrmtNadcn,
					gridFrmtFrnch,
					gridFrmtJapan,
					gridFrmtAts77,
					gridFrmtOst97,
					gridFrmtOst02,
					gridFrmtGeocn,
					gridFrmtUnknown = 9999
				 };

/* Coverage Object

   Carries the region of coverage for a grid file, a grid cell, whatever.
   Note that the density of the source is also required if this object
   is to be used to select one object over an another.

	This coverage object and associated functions assume that east longitude
	is positive.  The Canadian NTv2 stuff assumes that west lonigtude is
	positive, so for those types of data files, there is another coverage
	object. 
*/
struct csGridCoverage_
{
	/* A cell is marked as invalid by a southWest point northeast of the
	   northeast point; i.e. a coverage which will not match anything. */
	double southWest [2];
	double northEast [2];
	double density;				/* size of grid cell in seconds
								   used to select one file over
								   another in cases of overlap */
};
void CSinitCoverage (struct csGridCoverage_* thisPtr);
double CStestCoverage (struct csGridCoverage_* thisPtr,Const double point [2]);
void CSsetCoverage (struct csGridCoverage_* thisPtr,Const double* swLL,Const double* neLL);

/* Grid Cell Object -->  Used to hold the definition of a grid cell.  As of
   this writing, all grid techniques use the same concept, same algorithm, as
   defined for this object.  Where these cells come from varies, but once
   extracted from the appropriate data file, the cells are the same.

   This particular object is what we refer to as a normal grid cell.  We call
   it normal as these cells are generated from file formats where east
   longitude is positive; thus implying that the reference corner for all
   interpolations is the southwest corner.

   Another variation of this concept appears below and it is refered to as a
   Canadian grid cell.  In the case of a Canadian grid cell, west longitude is
   considered positive and implies that the reference corner for grid
   interpolation is the southeast corner of the grid cell. */
struct csGridCell_
{
	struct csGridCoverage_ coverage;	/* cell coverage in this case */
	double deltaLng;					/* dimensions of cell, degrees */
	double deltaLat;
	/* For performance, the AA, BB, CC, and DD values are precalculated
	   after cell data has been fetched from the data file. */
	double currentAA;
	double currentBB;
	double currentCC;
	double currentDD;
	char sourceId [32];					/* File name, and possibly
										   sub-grid, of the source of
										   the grid cell data. */
};
void CSinitGridCell (struct csGridCell_* thisPtr);
double CScalcGridCell (struct csGridCell_* thisPtr,Const double *sourceLL);
const char *CSsourceGridCell (struct csGridCell_* thisPtr);




/*
	The following is used to determine if byte swapping is
	at all necessary.  The character array is intialized to
	a sequence of specific characters.  The actual value
	of the resulting long is tested to see if the current
	machine is a little indian or big indian machine.

	NOTE:  The code as distributed by CS-MAP assumes
	that all binary data files are in little indian format.
	If you are running on a big indian system, and you want
	to use data files in big indian format, simply write
	your own stub version of void CS_bswap (char *rec,char *frmt)
	which does nothing, and force link it rather than the
	CS_bswap which resides in the CS_MAP library.
*/

union cs_Bswap_
{
	/* I should have, perhaps, used sizeof long here instead
	   of 4.  However, if longs aren't 4 long, none of this
	   byte swap stuff will work anyway. */

	char cccc [4];
	long32_t llll;
};

/******************************************************************************
*******************************************************************************
**                                                                           **
**                Analytical Geodetic   Method   Structures                  **
**                                                                           **
*******************************************************************************
******************************************************************************/

/*
	The following structures carry definitions of datums
	in various forms, suitable for the various transformation in
	use.  Soon to be merged into a union in the cs_DtcXform_
	structure, sort of like cs_Csprm_ structure above.

	We stick to a concept which has worked well in the past and,
	hopefully will continue to do so in the future.  That is, a
	particular instance of one of these structures defines a datum.
	Any such instance is suitable for use with either the forward
	or the inverse algorithm.  Should it ever become necessary,
	a given structure will be augmented to carry redundant
	information to support both the forward and the inverse in
	a single structure.

	Note, that forward implies from the source datum to the
	target datum as specified at setup time.  While WGS84
	is the target datum 99.99% of the time, the algorithms
	and/or structures do not require this.

	In the general case, however, the source datum will always be
	the datum in question.  The target datum will be WGS84.
	Initializations will almost always be set up this way, and the
	inverse code will see to it that the inverse is properly
	calculated.

	Thus, the resulting initialization will work for 2D/3D and
	forward/inverse transformations.
*/

/* Null Transformation

This transformation does very little, silently.  It is also very fast :>)

Such a transformation is used often: e.g. GDA94 <--> WGS84.  Traditionally
this is accomplished using a Geocentric Translation transformation with
zero geocentric translation parameters.  Thus, to emulate this we need to
simply convert from the source ellipsoid to the target ellipsoid.
*/
struct csNullx_
{
	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* WGS72 ala DMA 

Could be done by a simple Bursa Wolf, but we've always supported
this as a sepatate transformation, so I think we need to continue
to do so.  This should be simple as there are no parameters.
 */
struct csWgs72_
{
	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

struct csMolod_
{
	double srcERad;					/* Major axis radius of the source datum
									   ellipsoid. */
	double srcPRad;					/* Minor axis radius of the source datum
									   ellipsoid. */
	double srcESqr;					/* Major eccentricity squared of the source
									   datum ellipsoid. */
	double srcFlat;					/* Flattening of the source ellipsoid. */
	double trgERad;					/* Major axis radius of the source datum
									   ellipsoid. */
	double trgPRad;					/* Minor axis radius of the source datum
									   ellipsoid. */
	double trgESqr;					/* Major eccentricity squared of the source
									   datum ellipsoid. */
	double trgFlat;					/* Flattening of the target ellipsoid. */

	double deltaX;					/* X component on the vector defining the
									   geocenter of the local geodetic reference
									   system and WGS84. */
	double deltaY;					/* Y component on the vector defining the
									   geocenter of the local geodetic reference
									   system and WGS84. */
	double deltaZ;					/* Z component on the vector defining the
									   geocenter of the local geodetic reference
									   system and WGS84. */
	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Abridged Molodensky */
struct csAmolo_
{
	double delta_X;					/* X component on the vector defining the
									   geocenter of the local geodetic reference
									   system and WGS84. */
	double delta_Y;					/* Y component on the vector defining the
									   geocenter of the local geodetic reference
									   system and WGS84. */
	double delta_Z;					/* Z component on the vector defining the
									   geocenter of the local geodetic reference
									   system and WGS84. */
	double srcERad;					/* Major axis radius of the source datum
									   ellipsoid. */
	double srcPRad;					/* Minor axis radius of the source datum
									   ellipsoid. */
	double srcESqr;					/* Major eccentricity squared of the source
									   datum ellipsoid. */
	double srcFlat;					/* Flattening of the source ellipsoid. */
	double trgERad;					/* Major axis radius of the source datum
									   ellipsoid. */
	double trgPRad;					/* Minor axis radius of the source datum
									   ellipsoid. */
	double trgESqr;					/* Major eccentricity squared of the source
									   datum ellipsoid. */
	double trgFlat;					/* Flattening of the target ellipsoid. */

	char srcKeyName [cs_KEYNM_DEF];	/* Useful for debugging and error messages,
									   otherwise not used. */
	char trgKeyName [cs_KEYNM_DEF];	/* Useful for debugging and error messages,
									   otherwise not used. */
};

/* Geocentric Translation */
struct csGeoct_
{
	double srcERad;		/* Equatorial radius of the ellipsoid on
						   which the source datum is based. */
	double srcESqr;		/* Eccentricity squared of the elllipsoid
						   on which the source datum is based. */
	double trgERad;		/* Equatorial radius of the ellipsoid on
						   which the target datum is based. */
	double trgESqr;		/* Eccentricity squared of the elllipsoid
						   on which the target datum is based. */
	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Three Paramater (aka Geocentric Translation)

		DEPRECATED  ---  LEGACY  USE  ONLY
		DEPRECATED  ---  LEGACY  USE  ONLY
		DEPRECATED  ---  LEGACY  USE  ONLY
		DEPRECATED  ---  LEGACY  USE  ONLY
		DEPRECATED  ---  LEGACY  USE  ONLY
		DEPRECATED  ---  LEGACY  USE  ONLY
*/
struct csParm3_
{
	double srcERad;		/* Equatorial radius of the ellipsoid on
						   which the source datum is based. */
	double srcESqr;		/* Eccentricity squared of the elllipsoid
						   on which the source datum is based. */
	double trgERad;		/* Equatorial radius of the ellipsoid on
						   which the target datum is based. */
	double trgESqr;		/* Eccentricity squared of the elllipsoid
						   on which the target datum is based. */
	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Four Parameter (A Mentor Software invention) */
struct csParm4_
{
	double srcERad;					/* Equatorial radius of the ellipsoid on
									   which the source datum is based. */
	double srcESqr;					/* Eccentricity squared of the elllipsoid
									   on which the source datum is based. */
	double trgERad;					/* Equatorial radius of the ellipsoid on
									   which the target datum is based. */
	double trgESqr;					/* Eccentricity squared of the elllipsoid
									   on which the target datum is based. */
	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;
	double scale;		/* Scale of the transformation. */

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Six Parameter */
struct csParm6_
{
	double srcERad;		/* Equatorial radius of the ellipsoid on
						   which the source datum is based. */
	double srcESqr;		/* Eccentricity squared of the elllipsoid
						   on which the source datum is based. */
	double trgERad;		/* Equatorial radius of the ellipsoid on
						   which the target datum is based. */
	double trgESqr;		/* Eccentricity squared of the elllipsoid
						   on which the target datum is based. */
	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;
	/* The following represent a 3D rotation matrix.  This is a
	   rigorous implementation of the transformation matrix as
	   opposed to the Bursa/Wolfe which uses the sin(x) == x
	   approximation. */
	double rt11;
	double rt12;
	double rt13;
	double rt21;
	double rt22;
	double rt23;
	double rt31;
	double rt32;
	double rt33;

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Bursa Wolf (Position Vector variety) */
struct csBursa_
{
	double srcERad;		/* Equatorial radius of the ellipsoid on
						   which the source datum is based. */
	double srcESqr;		/* Eccentricity squared of the elllipsoid
						   on which the source datum is based. */
	double trgERad;		/* Equatorial radius of the ellipsoid on
						   which the target datum is based. */
	double trgESqr;		/* Eccentricity squared of the elllipsoid
						   on which the target datum is based. */
	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;

	double scale;		/* Scale of the transformation. */
	double rotX;		/* X rotation angle in radians. */
	double rotY;		/* Y rotation angle in radians. */
	double rotZ;		/* Z rotation angle in radians. */

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Coordinate Frame (Coordinate Frame variation of Bursa Wolf) */
struct csFrame_
{
	double srcERad;		/* Equatorial radius of the ellipsoid on
						   which the source datum is based. */
	double srcESqr;		/* Eccentricity squared of the elllipsoid
						   on which the source datum is based. */
	double trgERad;		/* Equatorial radius of the ellipsoid on
						   which the target datum is based. */
	double trgESqr;		/* Eccentricity squared of the elllipsoid
						   on which the target datum is based. */
	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;

	double scale;		/* Scale of the transformation. */
	double rotX;		/* X rotation angle in radians. */
	double rotY;		/* Y rotation angle in radians. */
	double rotZ;		/* Z rotation angle in radians. */

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Molodensky Badekas */ 
struct csBdkas_
{
	double srcERad;		/* Equatorial radius of the ellipsoid on
						   which the source datum is based. */
	double srcESqr;		/* Eccentricity squared of the elllipsoid
						   on which the source datum is based. */
	double trgERad;		/* Equatorial radius of the ellipsoid on
						   which the target datum is based. */
	double trgESqr;		/* Eccentricity squared of the elllipsoid
						   on which the target datum is based. */

	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;

	double scale;		/* Scale of the transformation. */
	double rotX;		/* X rotation angle in radians. */
	double rotY;		/* Y rotation angle in radians. */
	double rotZ;		/* Z rotation angle in radians. */

	/* Local system translation parameters. */
	double xlateX;     /*  X coordinate of the point about which the coordinate reference system is
						   rotated; given in the source cartesian coordinate reference system */
	double xlateY;     /* Y coordinate ... */
	double xlateZ;     /* Z coordinate ... */

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Rigorous Seven Parameter Transformation */ 
struct csParm7_
{
	double srcERad;		/* Equatorial radius of the ellipsoid on
						   which the source datum is based. */
	double srcESqr;		/* Eccentricity squared of the elllipsoid
						   on which the source datum is based. */
	double trgERad;		/* Equatorial radius of the ellipsoid on
						   which the target datum is based. */
	double trgESqr;		/* Eccentricity squared of the elllipsoid
						   on which the target datum is based. */
	/* Geocentric translation parameters. */
	double deltaX;
	double deltaY;
	double deltaZ;
	double scale;		/* Scale of the transformation. */

	/* The following represent a 3D rotation matrix.  This is a
	   rigorous implementation of the transformation matrix as
	   opposed to the Bursa/Wolf which uses the sin(x) == x
	   approximation. */
	double rt11;
	double rt12;
	double rt13;
	double rt21;
	double rt22;
	double rt23;
	double rt31;
	double rt32;
	double rt33;

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

/* Multiple Regression file format (of the DMA variety)

This struct contains the definition of a power series transformation
which is used to perform CS-MAP's original implementation of the
Defnese Mapping Agency's (now NIMA) variation of the multiple
regression transformation.  It is also used to implement any one of
the several EPSG General Polynomial transformations.

EPSG has a different method for all Polynomial transformations of
different orders.  We only have one implementation which is order
independent. The order of the transformation is, essentially,
determined at run time by an examination of the provided coefficients.

The DMA and EPSG versions differ only in that the EPSG version
supports a different set of offset values for use in the unnormalization
proicess of the results.  The DMA version does not support any offset
values for the results.*/
/* TODO!!!  Needs a flalback provision here. */
#define cs_MULRG_MAXDEG 13
#define cs_MULRG_MAXCOEF 105	 /* ((MAXDEG + 1) * (MAXDEG + 2) / 2) */
struct csMulrg_
{
	short max_uu;
	short max_vv;			/* Maximum power of the uu term in all
							   three expansions. */
	ulong32_t fill01;		/* Force alignment on an 8
							   byte boundary for all
							   compilers. */
	ulong32_t phiMap [4];
	ulong32_t lambdaMap [4];
	ulong32_t heightMap [4];/* A bit map of the coefficients which
							   are present (i.e. non-zero) for the
							   longitude calculation. */
	double normalizationScale;
							/* The normalizing scale factor for the
							   multiple regression. */
	double uu_off;          /* The longitude offset. */
	double vv_off;          /* The latitude offset. */
	double validation;		/* Definition of useful range in terms of
							   normalized coordinates. */
	double testPhi;
	double testLambda;		/* The latitude and longitude of the test
							   case associated with this definition. */
	double deltaPhi;
	double deltaLambda;
	double deltaHeight;		/* The results of using this transformation on
							   the test case provided above. */
	double phiCoefs [cs_MULRG_MAXCOEF];
	double lambdaCoefs [cs_MULRG_MAXCOEF];
	double heightCoefs [cs_MULRG_MAXCOEF];

	double errorValue;
	double cnvrgValue;
	short maxIterations;

	short fallback;
	union
	{
		struct csMolod_ molod;
		struct csParm6_ parm6;
		struct csParm7_ parm7;
	} fallbackXfrm;
};

#define cs_PLYNM_MAXDEG 13
#define cs_PLYNM_MAXCOEF 105	 /* ((MAXDEG + 1) * (MAXDEG + 2) / 2) */

/* Grid File Interpolation  -- This object ius actually a host for
   one or more grid file interpolation objects.  The design is such that
   more than a single file can be referenced and each file has a direction
   specification.  This enables defining a geodetic transformation such as
   NAD27 to NAD83 to include both the US and Canada.  This can be accomplished
   by including the US NADCON files and the Canadian National Transformation
   file in the definition.
   
   This introduces the problem of overlap.  Which file is to be used in the case
   where the point to be converted is in a region of overlap between the two
   files.  This is resolved by simply using the files in the order listed in the
   definition.  Thus, there can be a transformation named NAD83/US which gives
   priority to the US NADCON files (i.e. the NADCON files are listed first) and
   another named NAD83/CA which gives priority to the Canadian National
   Transformation file (i.e. the Canadian NTv2 file is listed first).

             ---- FYI ----
   Typically, there is only one file.  In some cases there may be three or
   four.  In order to support legacy operation of the now deprecated
   Nad83ToHarn.gdc geodtic data catalog file, we need to support 48 of these
   things concurrently, acting as a single datum (i.e. HARM/HPGN/NAD83-91 for
   all 49 states as a single datum).
*/

struct cs_GridFile_;
struct csGridi_
{
	double errorValue;
	double cnvrgValue;
	short maxIterations;
	short userDirection;
	short useBest;
	short fallbackDir;
	struct cs_GxXform_ *fallback;

	short fileCount;			/* Number of files */
	struct cs_GridFile_* gridFiles [csGRIDI1_FILEMAX];
};

/******************************************************************************
*******************************************************************************
**                                                                           **
**                Grid File Format Specific Structures                       **
**                                                                           **
*******************************************************************************
******************************************************************************/

/******************************************************************************
*******************************************************************************
**   Canadian National Transformation, Version 2 Grid Interpolation File     **
*******************************************************************************
******************************************************************************/
#include "cs_NTv2.h"

/******************************************************************************
*******************************************************************************
**   Canadian National Transformation, Version 1 Grid Interpolation File     **
*******************************************************************************
******************************************************************************/
#include "cs_NTv1.h"

/******************************************************************************
*******************************************************************************
**      US National Geodetic Service "NADCON" File (i.e. .las / .los )       **
*******************************************************************************
******************************************************************************/
#include "cs_Nadcon.h"

/******************************************************************************
*******************************************************************************
**             French Text Format Geodetic Interpolation File                **
*******************************************************************************
******************************************************************************/
#include "cs_Frnch.h"

/******************************************************************************
*******************************************************************************
**          ATS77 to NAD27 Datum Shift Object (aka TRANSFORM)                **
**                                                                           **
**   NAD27 <--> ATS77 Transformation for the Maritime Provinces of Canada    **
*******************************************************************************
******************************************************************************/
#include "cs_Ats77.h"

/******************************************************************************
*******************************************************************************
**             Japanese .PAR Grid Interpolation Data File                    **
**                                                                           **
**     Tokyo Datum to Japanese Geodetic Datum of 2000 Transformation         **
*******************************************************************************
******************************************************************************/
#include "cs_Japan.h"

/******************************************************************************
*******************************************************************************
**             GEOCON Horizontal Datum and Geod Height Grid Files            **
**                                                                           **
**     Used for NAD83(2007), NAD83(2011), and Geoid99, Geoid03,              **
**     and Geoid12 geoid height grid data files.                             **
**                                                                           **
**     Note that the NAD83(2007) and NAD83(2011) implementations include     **
**     a vertical datum shift file inaddition to the two horizontal grid     **
**     data files.                                                           **
**                                                                           **
*******************************************************************************
******************************************************************************/
#include "cs_Geocon.h"

/******************************************************************************
*******************************************************************************
**                                                                           **
**           General  Grid   Interpolation   File   Object                   **
**                                                                           **
*******************************************************************************
******************************************************************************/

struct cs_GridFile_
{
	short direction;
	enum csGridFormat format;
	char filePath [MAXPATH];

	double density;
	double errorValue;
	double cnvrgValue;

	long32_t bufferSize;
	ulong32_t flags;

	short maxIterations;

	double (*test)(void *gridFile,double *ll_src,short direction);
	int (*frwrd2D)(void *gridFile,double *ll_src,double *ll_trg);
	int (*frwrd3D)(void *gridFile,double *ll_src,double *ll_trg);
	int (*invrs2D)(void *gridFile,double *ll_src,double *ll_trg);
	int (*invrs3D)(void *gridFile,double *ll_src,double *ll_trg);
	int (*inRange)(void *gridFile,int cnt,Const double pnts [][3]);
	int (*release)(void *gridFile);
	int (*destroy)(void *gridFile);

	union
	{
		void* genericPtr;
		struct cs_NTv1_*  NTv1;
		struct cs_NTv2_*  NTv2;
		struct cs_Nadcn_* Nadcon;
		struct cs_Japan_* JapanPar;
		struct cs_Frnch_* FrenchRgf;
		struct cs_Ats77_* Ats77Xfrm;
		struct cs_Ost97_* Ostn97;
		struct cs_Ost02_* Ostn02;
		struct cs_Geocn_* Geocon;
	} fileObject;
};

/******************************************************************************
*******************************************************************************
**                                                                           **
**                General   Geodetic   Data   Structures                     **
**                                                                           **
*******************************************************************************
******************************************************************************/

/* cs_GeodeticTransformation_

This structure is essentially the binary form of a geodeticf transformation
definition.  A Geodetic Transformation Dictionary is a collection of these
things sorted in order by name.

Essentially, the CS_dtcsu function (DaTum Conversion Set Up) turns this
thing into a cs_GxXform which enables the actual mathemagics of the
desired operation.  Probably should have been called cs_Gxdef_ */

struct csGeodeticXfromParmsFile_
{
	unsigned char fileFormat;
	unsigned char direction;
	char fileName [csGRIDI1_FLNMSZ];	
};											/* 240 */

struct cs_GeodeticTransform_
{
	char xfrmName [64];
	char srcDatum [cs_KEYNM_DEF];
	char trgDatum [cs_KEYNM_DEF];
	char group [24];
	char description [128];
	char source [64];			/* 280 */
	short methodCode;
	short epsgCode;
	short epsgVariation;
	short inverseSupported;
	short maxIterations;
	short protect;
	short fill01;
	short fill02;				/* 296 */
	long32_t fill03;
	long32_t fill04;			/* 304 */
	double cnvrgValue;
	double errorValue;
	double accuracy;
	double rangeMinLng;
	double rangeMaxLng;
	double rangeMinLat;
	double rangeMaxLat;			/* 360 */
	double fill05;
	double fill06;
	double fill07;				/* 384 */
	/* The active member of the following union is a function of
	   the method type. */
	union csGeodeticXformParameters
	{
		struct csGeodeticXformParmsGeocentric
		{
			double deltaX;
			double deltaY;
			double deltaZ;
			double rotateX;
			double rotateY;
			double rotateZ;
			double scale;
			
			/* For molodenskii badekas: (http://info.ogp.org.uk/geodesy/guides/docs/G7-2.pdf; page 111 et seq.)
			   The point about which the coordinate reference farame is rottated, given in the source Cartesian coordinate reference system. */
			double translateX;
			double translateY;
			double translateZ; /* Complete: 10 * 8 */

			char fill [11844]; /* 4096 - (364 + 10 * 8) = 3652 */
		} geocentricParameters;

		/* An EPSG power series is generaly a 2D thing; but we 
		   provide for a 3D transformation just in case.
		   SHould be renamed to csPolynominal_ */
		struct csGeodeticXformParmsPwrSeries_
		{
			unsigned long bitMapA [4];
			unsigned long bitMapB [4];
			unsigned long bitMapC [4];
			double validation;
			double testX;
			double testY;
			double deltaX;
			double deltaY;
			double deltaZ;
			double srcOffsetX;
			double srcOffsetY;
			double srcOffsetZ;
			double trgOffsetX;
			double trgOffsetY;
			double trgOffsetZ;
			double normalizationScale;	/* 48 + 11 * 8 = 136 */
			double coeffA [cs_PLYNM_MAXCOEF];			/* max order is 13 */
			double coeffB [cs_PLYNM_MAXCOEF];			/* max order is 13 */
			double coeffC [cs_PLYNM_MAXCOEF];			/* max order is 13 */
			char fill [8292];
		} pwrSeriesParameters;			/* 364 + 152 + 2520 + 1060 = 4096 */
		struct csGeodeticXformParmsDmaMulReg_
		{
			//unsigned long phiBitMap [4];
			//unsigned long lambdaBitMap [4];
			//unsigned long heightBitMap [4];		/* 16 * 3 = 48 */
			double phiOffset;
			double lambdaOffset;
			double normalizationScale;	/* 9 * 8 = 72 + 48 = 120 */
			double validation;
			double testPhi;
			double testLambda;
			double deltaPhi;
			double deltaLambda;
			double deltaHeight;
			double coeffPhi [cs_MULRG_MAXCOEF];		/* max order is 10 */
			double coeffLambda [cs_MULRG_MAXCOEF];	/* max order is 10 */
			double coeffHeight [cs_MULRG_MAXCOEF];	/* max order is 10 */
			char fill [9284];
		} dmaMulRegParameters;			/*  364 + 120 + 2520 + 1092  = 4096  */
		struct csGeodeticXformParmsGridFiles_
		{
			short fileReferenceCount;
			struct csGeodeticXfromParmsFile_ fileNames [csGRIDI1_FILEMAX];
			char fallback [64];
			char fill01 [222];
		} fileParameters;				/* 240 * 50 + 2 + 64 = 12066 */
		struct csGeodeticXfromParmsSize_
		{
			char unionSize [12288];
		} sizeDetermination;
	} parameters;
};
#define cs_BSWP_GXDEF_BASE   "64c24c24c12c128c64cssssssddd"
#define cs_BSWP_GXDEF_GEOCTR "dddddddddd3676c"
#define cs_BSWP_GXDEF_PWRSRS "lllddddddddddd105d105d105d1136c"
#define cs_BSWP_GXDEF_MULREG "lllddddddddd105d105d105d1152c"
#define cs_BSWP_GXDEF_FILPRM "s3648c64c42c"

struct cs_GxIndex_
{
	char xfrmName [cs_KEYNM_DEF1];
	char srcDatum [cs_KEYNM_DEF];
	char trgDatum [cs_KEYNM_DEF];
	double accuracy;
	short inverseSupported;
	short methodCode;
};

struct cs_PivotDatumTbl_
{
	char datumName [cs_KEYNM_DEF];
	short fill01;
};

/* A geodetic path consists of an ordered list of up to 8 geodetic
  transformations.  Associated with each geodetic transformation is
  a direction (Forward or Inverse).
  
  A geodetic path, which a user can examine, modify, create, and/or
  delete, servers two purposes:
  1> defines a series of transformations which may be necessary to
     accomplish the transformation of geodetic coordinates from one
     datum to another.  For example, conversion of NAD27 to HARN is
     (to be technically correct) a conversion from NAD27 to NAD83
     (via NADCON) and then from NAD83 to HARN using the NADCON with
     the HPGN grid files.  To be technically correct, WGS84 does not
     enter the picture at all.
  2> Enables user to choose a specific variant when the traditional
     means of selecting a datum shift transformation fails to produce
     a unique result.  Thus, it is quite likely to have geodetic path
     definitions with a single geodetic transformation entry.
*/

/* This first structure simply  provides the means by which we associate
   a direction with a geodetic transformation name. */
struct cs_GeodeticPathElement_
{
	char geodeticXformName [64];
	double fill01;
	short direction;			/* cs_DTCDIR_FWD or cs_DTCDIR_INV */
	short fill02;
	short fill03;
	short fill04;
};								/* 80 */

/* To simplify code and meet tight development schedules, it has been
   decided to limit the length of a geodetic path to 8 transformations.
   In the current CS-MAP emulation of paths (a hard coded array in the
   CS_datum.c module) the largest number of transformatiosn was 4.  In
   EPSG (7.05), the maximum number of concatenated operations is three,
   even though a lonigtude translation (i.e. dealing with a prime meridian
   other than Greenwich) is usuallyone of the three.
   
   So, 8 looks like a pretty safe limit. */
#define csPATH_MAXXFRM 8

/* This beast is written to disk, thus we must be able to swap it
   whenever read or written from/to disk.  Also, we will need to linearly
   searching this dictionary quite often, looking at the source datum and
   then the target datum.  As the dictionary is maintained in sorted order
   by path name, we need to linearly search quite frequently.  Thus this
   structure is carefully constructed to be exactly 1024 bytes long for
   highly efficient disk I/O. */
struct cs_GeodeticPath_
{
	char pathName [64];				/* name assigned to this path
									   definition */
	char srcDatum [cs_KEYNM_DEF];	/* key name of the source datum for
									   this path */
	char trgDatum [cs_KEYNM_DEF];	/* key name of the target datum for
									   this path */
	double accuracy;				/* the largest accuracy value of all
									   transformations in the path, thus an
									   indication of the accuracy of the entire
									   path */
	double fill01;
	double fill02;
	short protect;					/* Protection scheme value, zero indicates 
									   a distribution definition whichis 100%
									   protected. */
	short reversible;				/* set to true if all elements in the path
									   are reversible which is the normal case */
	short elementCount;				/* Number of transformations in this definition.
									   Can be just one; usually two or three;
									   csPATHELE_BASE max */
	short epsgCode;					/* EPSG coordinate operation code of the
									   concatenated coordinate operation if there
									   is one and it is known; otherwise zero */
	short variant;					/* EPSG variant (if known) of the concatenated
									   coordinate operation which this path represents;
									   otherwise zero. */
	short fill03;
	short fill04;
	short fill05;
	short fill06;
	short fill07;
	short fill08;
	short fill09;
	char description [128];		/* detailed description of this definition. */
	char source [64];			/* the source of information for the definition
								   of this path. */
	char group [24];			/* Geographic group, is "LEGACY" in the case
								   deprecation. */
	char fill10 [8];			/* 360 */
	struct cs_GeodeticPathElement_ geodeticPathElements [csPATH_MAXXFRM];
};
#define cs_BSWP_GPDEF "64c24c24cdddssssssssssss128c64c24c8c64cdssss64cdssss64cdssss64cdssss64cdssss64cdssss64cdssss64cdssss" 

/* DESIGN NOTE:  this object represents a geodetic transformation which has
   been fully expanded and implemented.  Any and all extraneous information
   in the Geodetic Transformation definition should end up here. */
struct cs_GxXform_
{
	struct cs_GeodeticTransform_ gxDef;		/* a copy of the original definition. */

	/* Thie following stuff is common to (most) all datum transformation
	   types. */
	struct cs_Datum_ srcDatum;				/* supplies the source ellipsoid */			
	struct cs_Datum_ trgDatum;				/* supplies the target ellipsoid */	
	struct csGridCoverage_ xfrmCoverage;	/* Useful extent of the transformation */

	short methodCode;
	short isNullXfrm;
	short maxItr;
	short epsgVar;
	short inverseSupported;
	short maxIterations;
	short protect;
	short userDirection;
	unsigned long epsgNbr;
	double cnvrgValue;
	double errorValue;
	double accuracy;

	int (*frwrd2D)(void *gxXform,double *ll_src,double *ll_trg);
	int (*frwrd3D)(void *gxXform,double *ll_src,double *ll_trg);
	int (*invrs2D)(void *gxXform,double *ll_src,double *ll_trg);
	int (*invrs3D)(void *gxXform,double *ll_src,double *ll_trg);
	int (*inRange)(void *gxXform,int cnt,Const double pnts [][3]);
	int (*isNull) (void *gxXform);
	int (*release)(void *gxXform);
	int (*destroy)(void *gxXform);

	/* The following union carries information specific to the
	   various transformation types. */
	union
	{
		struct csNullx_   nullx;		/* Null transformation */
		struct csWgs72_   wgs72;		/* WGS72 -> WGS84 ala DMA TR-8350.2-B */
		struct csMolod_   molod;		/* Molodensky */
//		struct csAmolo_   amolo;		/* Abridged Molodensky */
		struct csGeoct_   geoct;		/* Geocentric Translation */
		struct csParm4_   parm4;		/* Four Parameter (a Mentor invention) */
		struct csParm6_   parm6;		/* Six Parameter Transformation */
		struct csBursa_   bursa;		/* Bursa Wolf, position vector variation */ 
		struct csFrame_   frame;		/* Coordinate Frame Rotation variation of Bursa Wolf */ 
		struct csParm7_   parm7;		/* Seven Parameter Transformation */
		struct csBdkas_   bdkas;		/* Molodensky Badekas */
		struct csMulrg_   mulrg;		/* DMA svariation of Multiple Regression Transformation */
//		struct csPlynm_   plynm;		/* Generalized polynominal transformation, up to 13th order */
		struct csGridi_   gridi;		/* Grid file interpolation aggregate.  Contains a
//										   transformation each of one or more grid files of
//										   varying format. */
		/* Legacy stuff */
		struct csParm3_   parm3;		/* Obsolete, used for legacy purposes only. */
	} xforms;

	/* The following is common to all transformation types, but are
	   not involved with the actual mathemagics of the process. */
	char xfrmName [64];
	char group [24];
	char description [128];
	char source [64];
};

struct cs_XfrmTab_
{
	char key_nm [64];
	int (EXP_LVL9 * initialize)(struct cs_GxXform_ *gxXform);
	int (EXP_LVL9 * isNull)    (void* gxXformParms);
	int (EXP_LVL9 * check)     (struct cs_GeodeticTransform_ *xfrmDef,
								unsigned short prj_code,
								int err_list [],
								int list_sz);
	unsigned short methodCode;
	ulong32_t methodFlags;
	ulong32_t epsgMethodCode;
	char descr [128];
};

struct cs_GridFormatTab_
{
	char key_nm [64];
	int (EXP_LVL9 * initialize)(struct cs_GridFile_* gxFile);
	int (EXP_LVL9 * check)     (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,
																			int err_list [],
																			int list_sz);
	unsigned short formatCode;
	ulong32_t formatFlags;
	ulong32_t epsgMethodCode;
	char descr [128];
};

struct csDtmBridgeXfrm_
{
	struct cs_GxIndex_ *xfrmPtr;
	short direction;	
};

/* A csBridge_ object is used to automatically construct a path from a source
   datum to a target datum from the Geodetic Transformation dictionary.
   
   We start out with just the source and target datum names, and the index
   values set to -1.  The -1 indicates that there is no coreesponding entry
   in the bridge.
*/

struct csDtmBridge_
{
	short srcIndex;						/* pointer to the last transformation
										   added to the bridge from the source
										   end.  -1 means none as yet. */
	char srcDatumName [cs_KEYNM_DEF];	/* The name of the datum at which the
										   bridge starts. */
	struct csDtmBridgeXfrm_ bridgeXfrms [csPATH_MAXXFRM];
										/* An ordered array of geodetic transformation
										   index entries defining (when complete) the
										   Path from the start of the bridge to the end
										   of the bridge.  Important to have the
										   direction of each transformation in the
										   path. */
	char trgDatumName [cs_KEYNM_DEF];	/* The name of the datum at which the
										   bridge ends (i.e. completes). */
	short trgIndex;						/* pointer to the last transformation
										   added to the bridge from the traget
										   end.  csPath_MAXXFRM (or greater)
										   means none as yet. */

};

/* DESIGN NOTE: this object represents a geodetic path which has been fully
   expanded and implemented.  Any and all extraneous information in the
   Geodetic Path definition should end up here. */
struct cs_Dtcprm_
{
	char srcKeyName [24];		/* Key name of the source geodetic
								   reference system (datum).  For error
								   reporting purposes. */
	char trgKeyName [24];		/* Key name of the target geodetic
								   reference system (datum).  For
								   error reporting purposes. */
	char pathName [64];			/* name assigned to this path
								   definition */
	char description [128];		/* detailed description of this definition. */
	char source [64];			/* the source of information for the definition
								   of this path. */
	char group [24];			/* Geographic group, is "LEGACY" in the case
								   deprecation. */
	short block_err;			/* Carries the block error reporting code:
									cs_DTCFLG_BLK_F,
									cs_DTCFLG_BLK_W,
									cs_DTCFLG_BLK_1,
									cs_DTCFLG_BLK_I
								*/
	short rptCount;				/* Used to suppress multiple reports of
								   the same problem. */
	short listCount;			/* number of entries used in the lat/long list. */
	short errLngLat [10][2];	/* An error of longitude and latitude values which
								   produced block errors.  Can be used to limit
								   warning messages produced by errant conversions. */
	short xfrmCount;			/* Number of xforms */
	struct cs_GxXform_ *xforms [csPATH_MAXXFRM];
								/* An array of transformation types and pointers
								   to the parameters required by the various
								   transformation techniques which are required to
								   get from the source to the target system.
								   Transformation is complete when a transformation
								   type of dtcTypNone is encountered.  A null
								   transformation is achieved if the first
								   transformation has a type of dtcTypNone. */
};

/*
	The following casts are used to eliminate warnings from
	ANSI compilers.  I don't understand why they are necessary,
`	but it appears that they are.  Perhaps this is because
	we use a union in the cs_Csprm_ structure and pass around
	pointers to specific elements in the union.

	Note, the defines do not include the surrounding
	parenthesis so they look something like a cast in
	the code.
*/
#if _RUN_TIME >= _rt_UNIXPCC
#	define cs_TEST_CAST double(*)(void *,double *,short)
#	define cs_FRWRD2D_CAST int(*)(void *,double *,double *)
#	define cs_FRWRD3D_CAST int(*)(void *,double *,double *)
#	define cs_INVRS2D_CAST int(*)(void *,double *,double *)
#	define cs_INVRS3D_CAST int(*)(void *,double *,double *)
#	define cs_INRANGE_CAST int(*)(void *,int,const double [][3])
#	define cs_ISNULL_CAST  int(*)(void *)
#	define cs_RELEASE_CAST int(*)(void *)
#	define cs_DESTROY_CAST int(*)(void *)
#else
#	define cs_TEST_CAST double(*)(Const void *,double *,short)
#	define cs_FRWRD2D_CAST int(*)(Const void *,double *,Const double *)
#	define cs_FRWRD3D_CAST int(*)(Const void *,double *,Const double *)
#	define cs_INVRS2D_CAST int(*)(Const void *,double *,Const double *)
#	define cs_INVRS3D_CAST int(*)(Const void *,double *,Const double *)
#	define cs_INRANGE_CAST int(*)(Const void *,int,Const double [][3])
#	define cs_ISNULL_CAST  int(*)(Const void *)
#	define cs_RELEASE_CAST int(*)(Const void *)
#	define cs_DESTROY_CAST int(*)(Const void *)
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum csGridFormat EXP_LVL9 CSgridFileFormatCvt (unsigned flagValue);
struct cs_GxIndex_* EXP_LVL3 CS_getGxIndexPtr (void);
unsigned EXP_LVL3 CS_getGxIndexCount ();
Const struct cs_GxIndex_* EXP_LVL3 CS_getGxIndexEntry (unsigned index);
void EXP_LVL3 CS_releaseGxIndex (void);
int EXP_LVL3 CS_locateGxByName (Const char* xfrmName);
int EXP_LVL3 CS_locateGxByDatum (unsigned startAt,Const char* srcDtmName,Const char* trgDtmName);
int EXP_LVL3 CS_locateGxFromDatum (int* direction,Const char* srcDtmName);
int EXP_LVL3 CS_locateGxToDatum (int* direction,Const char* trgDtmName);
int EXP_LVL3 CS_locateGxByDatum2 (int* direction,Const char* srcDtmName,Const char* trgDtmName);
void EXP_LVL9 CSgenerateGxIndex (void);

struct cs_GxXform_* EXP_LVL1 CS_gxloc (Const char* gxDefName,short userDirection);
struct cs_GxXform_* EXP_LVL1 CS_gxloc1 (Const struct cs_GeodeticTransform_ *gxXform,short userDirection);
struct cs_GxXform_*	EXP_LVL3 CS_gxlocDtm (Const struct cs_Datum_ *src_dt,Const struct cs_Datum_ *dst_dt);

int EXP_LVL1 CS_gxFrwrd3D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3]);
int EXP_LVL1 CS_gxFrwrd2D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3]);
int EXP_LVL1 CS_gxInvrs3D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3]);
int EXP_LVL1 CS_gxInvrs2D (struct cs_GxXform_ *xform,double trgLl [3],Const double srcLl [3]);
int EXP_LVL1 CS_gxIsNull  (struct cs_GxXform_ *xform);
void EXP_LVL1 CS_gxDisable (struct cs_GxXform_ *xform);

int CSdtcsuPhaseOne   (struct csDtmBridge_* bridgePtr,struct cs_Dtcprm_ *dtcPtr);
int CSdtcsuPhaseTwo   (struct csDtmBridge_* bridgePtr,struct cs_Dtcprm_ *dtcPtr);
int CSdtcsuPhaseThree (struct csDtmBridge_* bridgePtr,struct cs_Dtcprm_ *dtcPtr);
int CSdtcsuPhaseFour  (struct csDtmBridge_* bridgePtr,struct cs_Dtcprm_ *dtcPtr);

struct csDtmBridge_* CSnewDtmBridge (Const struct cs_Datum_* src_dt,
									 Const struct cs_Datum_* dst_dt);
Const char* CSdtmBridgeGetSourceDtm (struct csDtmBridge_* thisPtr);
Const char* CSdtmBridgeGetTargetDtm (struct csDtmBridge_* thisPtr);

int CSdtmBridgeIsComplete (struct csDtmBridge_* thisPtr);
int CSdtmBridgeIsFull (Const struct csDtmBridge_* thisPtr);
int CSdtmBridgeAddSrcPath (struct csDtmBridge_* thisPtr,Const struct cs_GeodeticPath_* pathPtr,
													    short direction);
int CSdtmBridgeAddSrcTransformation (struct csDtmBridge_* thisPtr,Const struct cs_GxIndex_* xfrmPtr,
																  short direction);
int CSdtmBridgeAddTrgPath (struct csDtmBridge_* thisPtr,Const struct cs_GeodeticPath_* pathPtr,
													    short direction);
int CSdtmBridgeAddTrgTransformation (struct csDtmBridge_* thisPtr,Const struct cs_GxIndex_* xfrmPtr,
																  short direction);

int			EXP_LVL9	  CSnullxD  (struct csNullx_ *nullx);
int			EXP_LVL9	  CSnullxF2 (struct csNullx_ *nullx,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnullxF3 (struct csNullx_ *nullx,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnullxI2 (struct csNullx_ *nullx,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnullxI3 (struct csNullx_ *nullx,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnullxL  (struct csNullx_ *nullx,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSnullxN  (struct csNullx_ *nullx);
int			EXP_LVL9	  CSnullxQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSnullxR  (struct csNullx_ *nullx);
int			EXP_LVL9	  CSnullxS  (struct cs_GxXform_ *nullx);

int			EXP_LVL9	  CSwgs72D  (struct csWgs72_ *wgs72);
int			EXP_LVL9	  CSwgs72F2 (struct csWgs72_ *wgs72,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSwgs72F3 (struct csWgs72_ *wgs72,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSwgs72I2 (struct csWgs72_ *wgs72,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSwgs72I3 (struct csWgs72_ *wgs72,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSwgs72L  (struct csWgs72_ *wgs72,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSwgs72N  (struct csWgs72_ *wgs72);
int			EXP_LVL9	  CSwgs72Q  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSwgs72R  (struct csWgs72_ *wgs72);
int			EXP_LVL9	  CSwgs72S  (struct cs_GxXform_ *wgs72);

int			EXP_LVL9	  CSmolodD  (struct csMolod_ *molod);
int			EXP_LVL9	  CSmolodF2 (struct csMolod_ *molod,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmolodF3 (struct csMolod_ *molod,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmolodI2 (struct csMolod_ *molod,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmolodI3 (struct csMolod_ *molod,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmolodL  (struct csMolod_ *molod,int cnt,Const double pnts [][3]);
int 		EXP_LVL9	  CSmolodN  (struct csMolod_ *molod);
int			EXP_LVL9	  CSmolodQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSmolodR  (struct csMolod_ *molod);
int 		EXP_LVL9	  CSmolodS  (struct cs_GxXform_ *molod);
int			EXP_LVL9	  CSmolodSf (struct csMolod_* molod,Const struct cs_Datum_ *srcDatum,
															Const struct cs_Datum_ *trgDatum);

//int			EXP_LVL9	  CSamoloD  (struct csAmolo_ *amolo);
//int			EXP_LVL9	  CSamoloF2 (struct csAmolo_ *amolo,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSamoloF3 (struct csAmolo_ *amolo,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSamoloI2 (struct csAmolo_ *amolo,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSamoloI3 (struct csAmolo_ *amolo,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSamoloL  (struct csAmolo_ *amolo,int cnt,Const double pnts [][3]);
//int			EXP_LVL9	  CSamoloN  (struct csAmolo_ *amolo);
//int			EXP_LVL9	  CSamoloQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
//int			EXP_LVL9	  CSamoloR  (struct csAmolo_ *amolo);
//int			EXP_LVL9	  CSamoloS  (struct cs_GxXform_ *amolo);

int			EXP_LVL9	  CSgeoctD  (struct csGeoct_ *geoct);
int			EXP_LVL9	  CSgeoctF2 (struct csGeoct_ *geoct,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeoctF3 (struct csGeoct_ *geoct,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeoctI2 (struct csGeoct_ *geoct,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeoctI3 (struct csGeoct_ *geoct,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeoctL  (struct csGeoct_ *geoct,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSgeoctN  (struct csGeoct_ *geoct);
int			EXP_LVL9	  CSgeoctQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSgeoctR  (struct csGeoct_ *geoct);
int			EXP_LVL9	  CSgeoctS  (struct cs_GxXform_ *geoct);

int			EXP_LVL9	  CSparm4D  (struct csParm4_ *parm4);
int			EXP_LVL9	  CSparm4F2 (struct csParm4_ *parm4,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm4F3 (struct csParm4_ *parm4,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm4I2 (struct csParm4_ *parm4,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm4I3 (struct csParm4_ *parm4,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm4L  (struct csParm4_ *parm4,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSparm4N  (struct csParm4_ *parm4);
int			EXP_LVL9	  CSparm4Q  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSparm4R  (struct csParm4_ *parm4);
int			EXP_LVL9	  CSparm4S  (struct cs_GxXform_ *parm4);

int			EXP_LVL9	  CSparm6D  (struct csParm6_ *parm6);
int			EXP_LVL9	  CSparm6F2 (struct csParm6_ *parm6,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm6F3 (struct csParm6_ *parm6,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm6I2 (struct csParm6_ *parm6,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm6I3 (struct csParm6_ *parm6,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm6L  (struct csParm6_ *parm6,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSparm6N  (struct csParm6_ *parm6);
int			EXP_LVL9	  CSparm6Q  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSparm6R  (struct csParm6_ *parm6);
int			EXP_LVL9	  CSparm6S  (struct cs_GxXform_ *parm6);

int			EXP_LVL9	  CSbursaD  (struct csBursa_ *bursa);
int			EXP_LVL9	  CSbursaF2 (struct csBursa_ *bursa,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbursaF3 (struct csBursa_ *bursa,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbursaI2 (struct csBursa_ *bursa,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbursaI3 (struct csBursa_ *bursa,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbursaL  (struct csBursa_ *bursa,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSbursaN  (struct csBursa_ *gxXfrm);
int			EXP_LVL9	  CSbursaQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSbursaR  (struct csBursa_ *bursa);
int			EXP_LVL9	  CSbursaS  (struct cs_GxXform_ *gxXfrm);

int			EXP_LVL9	  CSframeD  (struct csFrame_ *frame);
int			EXP_LVL9	  CSframeF2 (struct csFrame_ *frame,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSframeF3 (struct csFrame_ *frame,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSframeI2 (struct csFrame_ *frame,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSframeI3 (struct csFrame_ *frame,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSframeL  (struct csFrame_ *frame,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSframeN  (struct csFrame_ *frame);
int			EXP_LVL9	  CSframeQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSframeR  (struct csFrame_ *frame);
int			EXP_LVL9	  CSframeS  (struct cs_GxXform_ *frame);

int			EXP_LVL9	  CSparm7D  (struct csParm7_ *parm7);
int			EXP_LVL9	  CSparm7F2 (struct csParm7_ *parm7,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm7F3 (struct csParm7_ *parm7,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm7I2 (struct csParm7_ *parm7,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm7I3 (struct csParm7_ *parm7,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm7L  (struct csParm7_ *parm7,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSparm7N  (struct csParm7_ *parm7);
int			EXP_LVL9	  CSparm7Q  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSparm7R  (struct csParm7_ *parm7);
int			EXP_LVL9	  CSparm7S  (struct cs_GxXform_ *parm7);
int			EXP_LVL9	  CSparm7Sf (struct csParm7_* parm7,Const struct cs_Datum_ *srcDatum,
															Const struct cs_Datum_* trgDatum);

int			EXP_LVL9	  CSbdkasD  (struct csBdkas_ *bdkas);
int			EXP_LVL9	  CSbdkasF2 (struct csBdkas_ *bdkas,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbdkasF3 (struct csBdkas_ *bdkas,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbdkasI2 (struct csBdkas_ *bdkas,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbdkasI3 (struct csBdkas_ *bdkas,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSbdkasL  (struct csBdkas_ *bdkas,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSbdkasN  (struct csBdkas_ *bdkas);
int			EXP_LVL9	  CSbdkasQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSbdkasR  (struct csBdkas_ *bdkas);
int			EXP_LVL9	  CSbdkasS  (struct cs_GxXform_ *bdkas);

int			EXP_LVL9	  CSmulrgD  (struct csMulrg_ *mulrg);
int			EXP_LVL9	  CSmulrgF2 (struct csMulrg_ *mulrg,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmulrgF3 (struct csMulrg_ *mulrg,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmulrgI2 (struct csMulrg_ *mulrg,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmulrgI3 (struct csMulrg_ *mulrg,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSmulrgL  (struct csMulrg_ *mulrg,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSmulrgN  (struct csMulrg_ *mulrg);
int			EXP_LVL9	  CSmulrgQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSmulrgR  (struct csMulrg_ *mulrg);
int			EXP_LVL9	  CSmulrgS  (struct cs_GxXform_ *mulrg);

//int			EXP_LVL9	  CSplynmD  (struct csPlynm_ *plynm);
//int			EXP_LVL9	  CSplynmF2 (struct csPlynm_ *plynm,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSplynmF3 (struct csPlynm_ *plynm,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSplynmI2 (struct csPlynm_ *plynm,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSplynmI3 (struct csPlynm_ *plynm,double *ll_trg,Const double *ll_src);
//int			EXP_LVL9	  CSplynmL  (struct csPlynm_ *plynm,int cnt,Const double pnts [][3]);
//int			EXP_LVL9	  CSplynmN  (struct csPlynm_ *plynm);
//int			EXP_LVL9	  CSplynmQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
//int			EXP_LVL9	  CSplynmR  (struct csPlynm_ *plynm);
//int			EXP_LVL9	  CSplynmS  (struct cs_GxXform_ *plynm);

int			EXP_LVL9	  CSgridiD  (struct csGridi_ *gridi);
int			EXP_LVL9	  CSgridiF2 (struct csGridi_ *gridi,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgridiF3 (struct csGridi_ *gridi,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgridiI2 (struct csGridi_ *gridi,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgridiI3 (struct csGridi_ *gridi,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgridiL  (struct csGridi_ *gridi,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSgridiN  (struct csGridi_ *gridi);
int			EXP_LVL9	  CSgridiQ  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSgridiR  (struct csGridi_ *gridi);
int			EXP_LVL9	  CSgridiS  (struct cs_GxXform_ *gridi);
int			EXP_LVL9	  CSgridiT  (struct csGridi_ *gridi,double* ll_src,short direction);

/********   DEPRECATED,  LEGACY USE ONLY  ******************/
int			EXP_LVL9	  CSparm3D  (struct csParm3_ *parm3);
int			EXP_LVL9	  CSparm3F2 (struct csParm3_ *parm3,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm3F3 (struct csParm3_ *parm3,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm3I2 (struct csParm3_ *parm3,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm3I3 (struct csParm3_ *parm3,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSparm3L  (struct csParm3_ *parm3,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSparm3N  (struct csParm3_ *parm3);
int			EXP_LVL9	  CSparm3Q  (struct cs_GeodeticTransform_ *gxDef,unsigned short prj_code,int err_list [],int list_sz);
int			EXP_LVL9	  CSparm3R  (struct csParm3_ *parm3);
int			EXP_LVL9	  CSparm3S  (struct cs_GxXform_ *parm3);
/********   END DEPRECATED SYSTEMS  ******************/

int			EXP_LVL9	  CScntv1D  (struct cs_NTv1_ *cntv1);
int			EXP_LVL9	  CScntv1F2 (struct cs_NTv1_ *cntv1,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv1F3 (struct cs_NTv1_ *cntv1,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv1I2 (struct cs_NTv1_ *cntv1,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv1I3 (struct cs_NTv1_ *cntv1,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv1L  (struct cs_NTv1_ *cntv1,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CScntv1Q  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CScntv1R  (struct cs_NTv1_ *cntv1);
int			EXP_LVL9	  CScntv1S  (struct cs_GridFile_ *cntv1);
double		EXP_LVL9	  CScntv1T  (struct cs_NTv1_ *cntv1,double *ll_src,short direction);

int			EXP_LVL9	  CScntv2D  (struct cs_NTv2_ *cntv2);
int			EXP_LVL9	  CScntv2F2 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv2F3 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv2I2 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv2I3 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CScntv2L  (struct cs_NTv2_ *cntv2,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CScntv2Q  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CScntv2R  (struct cs_NTv2_ *cntv2);
int			EXP_LVL9	  CScntv2S  (struct cs_GridFile_ *cntv2);
double		EXP_LVL9	  CScntv2T  (struct cs_NTv2_ *cntv2,double ll_src [2],short direction);

int			EXP_LVL9	  CSnadcnD  (struct cs_Nadcn_ *nadcn);
int			EXP_LVL9	  CSnadcnF2 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnadcnF3 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnadcnI2 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnadcnI3 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSnadcnL  (struct cs_Nadcn_ *nadcn,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSnadcnQ  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CSnadcnR  (struct cs_Nadcn_ *nadcn);
int			EXP_LVL9	  CSnadcnS  (struct cs_GridFile_ *nadcn);
double		EXP_LVL9	  CSnadcnT  (struct cs_Nadcn_ *nadcn,double *ll_src,short direction);

int			EXP_LVL9	  CSfrnchD  (struct cs_Frnch_ *frnch);
int			EXP_LVL9	  CSfrnchF2 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSfrnchF3 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSfrnchI2 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSfrnchI3 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSfrnchL  (struct cs_Frnch_ *frnch,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSfrnchQ  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CSfrnchR  (struct cs_Frnch_ *frnch);
int			EXP_LVL9	  CSfrnchS  (struct cs_GridFile_ *frnch);
double		EXP_LVL9	  CSfrnchT  (struct cs_Frnch_ *frnch,double *ll_src,short direction);

int			EXP_LVL9	  CSjapanD  (struct cs_Japan_ *japan);
int			EXP_LVL9	  CSjapanF2 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSjapanF3 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSjapanI2 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSjapanI3 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSjapanL  (struct cs_Japan_ *japan,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSjapanQ  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CSjapanR  (struct cs_Japan_ *japan);
int			EXP_LVL9	  CSjapanS  (struct cs_GridFile_ *japan);
double		EXP_LVL9	  CSjapanT  (struct cs_Japan_ *japan,double *ll_src,short direction);

int			EXP_LVL9	  CSats77D  (struct cs_Ats77_ *ats77);
int			EXP_LVL9	  CSats77F2 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSats77F3 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSats77I2 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSats77I3 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSats77L  (struct cs_Ats77_ *ats77,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSats77Q  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CSats77R  (struct cs_Ats77_ *ats77);
int			EXP_LVL9	  CSats77S  (struct cs_GridFile_ *ats77);
double		EXP_LVL9	  CSats77T  (struct cs_Ats77_ *ats77,double *ll_src,short direction);

int			EXP_LVL9	  CSost97D  (struct cs_Ost97_ *ost97);
int			EXP_LVL9	  CSost97F2 (struct cs_Ost97_ *ost97,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost97F3 (struct cs_Ost97_ *ost97,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost97I2 (struct cs_Ost97_ *ost97,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost97I3 (struct cs_Ost97_ *ost97,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost97L  (struct cs_Ost97_ *ost97,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSost97Q  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CSost97R  (struct cs_Ost97_ *ost97);
int			EXP_LVL9	  CSost97S  (struct cs_GridFile_ *ost97);
double		EXP_LVL9	  CSost97T  (struct cs_Ost97_ *ost97,double *ll_src,short direction);

int			EXP_LVL9	  CSost02D  (struct cs_Ost02_ *ost02);
int			EXP_LVL9	  CSost02F2 (struct cs_Ost02_ *ost02,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost02F3 (struct cs_Ost02_ *ost02,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost02I2 (struct cs_Ost02_ *ost02,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost02I3 (struct cs_Ost02_ *ost02,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSost02L  (struct cs_Ost02_ *ost02,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSost02Q  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CSost02R  (struct cs_Ost02_ *ost02);
int			EXP_LVL9	  CSost02S  (struct cs_GridFile_ *ost02);
double		EXP_LVL9	  CSost02T  (struct cs_Ost02_ *ost02,double *ll_src,short direction);

int			EXP_LVL9	  CSgeocnD  (struct cs_Geocn_ *geocn);
int			EXP_LVL9	  CSgeocnF2 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeocnF3 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeocnI2 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeocnI3 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src);
int			EXP_LVL9	  CSgeocnL  (struct cs_Geocn_ *geocn,int cnt,Const double pnts [][3]);
int			EXP_LVL9	  CSgeocnQ  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz);
int			EXP_LVL9	  CSgeocnR  (struct cs_Geocn_ *geocn);
int			EXP_LVL9	  CSgeocnS  (struct cs_GridFile_ *geocn);
double		EXP_LVL9	  CSgeocnT  (struct cs_Geocn_ *geocn,double *ll_src,short direction);

#ifdef __cplusplus
}
#endif
