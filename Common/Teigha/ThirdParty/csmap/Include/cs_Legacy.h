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
	The following define the codes used to distinguish the different types
	of general geodetic reference system transformations.
*/

#define cs_DTCTYP_NONE    0
#define cs_DTCTYP_MOLO    1
#define cs_DTCTYP_MREG    2
#define cs_DTCTYP_BURS    3
#define cs_DTCTYP_NAD27   4
#define cs_DTCTYP_NAD83   5
#define cs_DTCTYP_WGS84   6
#define cs_DTCTYP_WGS72   7
#define cs_DTCTYP_HPGN    8
#define cs_DTCTYP_7PARM   9
#define cs_DTCTYP_AGD66   10
#define cs_DTCTYP_3PARM   11
#define cs_DTCTYP_6PARM   12
#define cs_DTCTYP_4PARM   13
#define cs_DTCTYP_AGD84   14
#define cs_DTCTYP_NZGD49  15
#define cs_DTCTYP_ATS77   16
#define cs_DTCTYP_GDA94   17
#define cs_DTCTYP_NZGD2K  18
#define cs_DTCTYP_CSRS    19
#define cs_DTCTYP_TOKYO   20
#define cs_DTCTYP_RGF93   21
#define cs_DTCTYP_ED50    22
#define cs_DTCTYP_DHDN    23
#define cs_DTCTYP_ETRF89  24
#define cs_DTCTYP_GEOCTR  25
#define cs_DTCTYP_CHENYX  26

/*
	A datum transformation may require several different transformations.
	Each individual transformation is defined by one of the following
	structures.  This structure, essentially, defines the type of the
	transformation, and carries the parameters for the transformation
	in the form of a union of each of the parameter requirements.

	The supported transformation types are defined as an enumerator.
*/

enum cs_DtcXformType
{
	dtcTypNone = 0,
	dtcTypMolodensky,						/*    1 */
	dtcTypDMAMulReg,						/*    2 */
	dtcTypBursaWolf,						/*    3 */
	dtcTypNad27ToNad83,						/*    4 */
	dtcTypNad83ToWgs84,						/*    5 */
	dtcTypWgs72ToWgs84,						/*    6 */
	dtcTypGda94ToWgs84,						/*    7 */
	dtcTypNzgd2KToWgs84,					/*    8 */
	dtcTypNad83ToHarn,						/*    9 */
	dtcTypAgd66ToGda94,						/*   10 */
	dtcTypAgd84ToGda94,						/*   11 */
	dtcTypNzgd49ToNzgd2K,					/*   12 */
	dtcTypAts77ToCsrs,						/*   13 */
	dtcTypNad83ToCsrs,						/*   14 */
	dtcTypSevenParm,						/*   15 */
	dtcTypGeoCtr,							/*   16 */
	dtcTypSixParm,							/*   17 */
	dtcTypFourParm,							/*   18 */
	dtcTypTokyoToJgd2k,						/*   19 */
	dtcTypNtfToRgf93,						/*   20 */
	dtcTypNad27ToCsrs,						/*   21 */
	dtcTypEd50ToEtrf89,						/*   22 */
	dtcTypDhdnToEtrf89,						/*   23 */
	dtcTypEtrf89ToWgs84,					/*   24 */
	dtcTypNad27ToAts77,						/*   25 */
	dtcTypThreeParm,						/*   26 */
	dtcTypCh1903ToPlus,						/*   27 */
	dtcTypChPlusToChtrs95,					/*   28 */
	dtcTypChtrs95ToEtrf89,  				/*   29 */

	/* Inverse of the above */
	dtcTypStartOfInverses = 1000,			/* 1000 */
	dtcTypMolodenskyInv,					/* 1001 */
	dtcTypDMAMulRegInv,						/* 1002 */
	dtcTypBursaWolfInv,						/* 1003 */
	dtcTypNad83ToNad27,						/* 1004 */
	dtcTypWgs84ToNad83,						/* 1005 */
	dtcTypWgs84ToWgs72,						/* 1006 */
	dtcTypWgs84ToGda94,						/* 1007 */
	dtcTypWgs84ToNzgd2K,					/* 1008 */
	dtcTypHarnToNad83,						/* 1009 */
	dtcTypGda94ToAgd66,						/* 1010 */
	dtcTypGda94ToAgd84,						/* 1011 */
	dtcTypNzgd2KToNzgd49,					/* 1012 */
	dtcTypCsrsToAts77,						/* 1013 */
	dtcTypCsrsToNad83,						/* 1014 */
	dtcTypSevenParmInv,						/* 1015 */
	dtcTypGeoCtrInv,						/* 1016 */
	dtcTypSixParmInv,						/* 1017 */
	dtcTypFourParmInv,						/* 1018 */
	dtcTypJgd2kToTokyo,						/* 1019 */
	dtcTypRgf93ToNtf,						/* 1020 */
	dtcTypCsrsToNad27,						/* 1021 */
	dtcTypEtrf89ToEd50,						/* 1022 */
	dtcTypEtrf89ToDhdn,						/* 1023 */
	dtcTypWgs84ToEtrf89,					/* 1024 */
	dtcTypAts77ToNad27,						/* 1025 */
	dtcTypThreeParmInv,						/* 1026 */
	dtcTypEtrf89ToChtrs95,					/* 1027 */
	dtcTypChtrs95ToChPlus,					/* 1028 */
	dtcTypPlusToCh1903, 					/* 1029 */

	/* A programming convenience */
	dtcTypSkip = 9999						/* 9999 */
};

/*
	Multiple regression file format.  This structure is, essentially, the
	format of a .MRT file.  MRT files can only be written by the
	dictionary compiler.  MRT files will soon be replaced with an
	expanded datum dictionary format.
*/

#define cs_DMAMREG_MAXCOEF 150
struct csDmaMReg_
{
	cs_magic_t magic;		/* Used to identify the file in which
							   these structures are stored. */
	long32_t mr_size;			/* Actual number of bytes required
							   for the complete definition.  Not
							   all 150 coefficients are written
							   to a data file.  If you get this
							   many bytes in the read, you are
							   OK. */
	unsigned char max_uu;   /* Maximum power of the uu term in all
							   three expansions. */
	unsigned char max_vv;
	unsigned char lat_idx;  /* Index into coefs for the first
							   latitude coeficient. */
	unsigned char hgt_idx;
	ulong32_t fill01;		/* Force alignment on an 8
							   byte boundary for all
							   compilers. */
	ulong32_t lng_map [4];
							/* A bit map of the coefficients which
							   are present (i.e. non-zero) for the
							   longitude calculation. */
	ulong32_t lat_map [4];
	ulong32_t hgt_map [4];
	double kk;				/* The normalizing scale factor for the
							   multiple regression. */
	double uu_off;          /* The longitude offset. */
	double vv_off;          /* The latitude offset. */
	double coefs [cs_DMAMREG_MAXCOEF];
							/* The coefficients for the calculations.
							   This structure allocates space for the
							   maximum we have encountered so far.
							   The format of the file which we
							   have devised supports having much
							   less than 150 coefficients.  All
							   150 are never used. */
};
#define cs_BSWP_DMAMREG "ll4c4l4l4lddd150d"

/*
	The following variation on the DmaMReg theme includes a provision
	for using an alternative fallback transformation when the data
	being converted is outside the range of the multiple regression.
*/
struct cs_DmaMReg_
{
	struct csDmaMReg_ mReg;
	enum cs_DtcXformType fallback;
	union
	{
		struct cs_Molo_* molo;
		struct cs_Parm6_* parm6;
		struct cs_Parm7_* parm7;
	} fallbackXfrm;
};

/*
	The following is a generic transformation object.  That is, encapsulates
	all variations (i.e. any type) of a single datum transformation into a
	single obejct.
*/

struct cs_DtcXform_
{
	enum cs_DtcXformType xfrmType;
	short flag3D;						/* Kludge: set true when and if Vertcon
										   or other auxilliary 3D database was
										   opened for this conversion.  Thus, 2D
									       conversions can ocurr without error
										   if auxilliary data is not available. */
	union
	{
		struct cs_Molo_* molo;
		struct cs_Bursa_* bursa;
		struct cs_Parm7_* parm7;
		struct cs_Geoctr_* geoctr;
		struct cs_Parm6_* parm6;
		struct cs_Parm4_* parm4;
		struct cs_DmaMReg_* mreg;
		struct cs_Parm3_* parm3;
	} parms;
	char sourceId [48];
};
void		EXP_LVL9	CSdtcXformFree (struct cs_DtcXform_ *xfrmPtr);

/*
	The header information on a US Grid file has the following format.
	The size of the header record is always equal to the size of a
	record in the file.  This is the stuff that is in the disk file!
	Note, that since a lot of this stuff is binary, byte swapping
	may be required.
*/
struct csGridHdrUS_
{
	char ident [56];        /* NADCON's identification.  We don't
							   use it. */
	char pgm [8];           /* Don't use it. */
	long32_t ele_cnt;           /* Number of data elements in each record.
							   NADCON calls it a column count.  This
							   value does not include the record number
							   element which is the first item in
							   each record.  It is used to calculate
							   record size and the longitude extent of
							   the rectangular region of coverage. */
	long32_t rec_cnt;           /* The number of records in the file.
							   Does not include the header record
							   defined by this structure.  Notice, that
							   the header record is the same length
							   as the others, but only the first
							   n bytes, as defined by this structure,
							   are actually used.  This value also
							   defines the latitude extent of
							   the rectangular region of coverage. */
	long32_t z_cnt;             /* Don't know what this is, we don't use
							   it.  When, and if, NADCON ever uses it,
							   we'll know what it is. */
	float min_lng;          /* The longitude associated with the
							   first record in the file. */
	float del_lng;          /* The distance, in degrees of longitude,
							   between the grid cells. */
	float min_lat;          /* The latitude associated with the first
							   data element in each record. */
	float del_lat;          /* The distance, in degrees of latitude,
							   between the grid cells. */
	float angle;            /* Don't know what this is.  When NADCON
							   ever gets around to using it, we'll
							   know then. */
};
#define cs_BSWP_GridFileHdrUS "56c8clllfffff"

/******************************************************************************
	Canadian National Transformation, Version 1

	The following structure defines the format of the Canadian	National
	Transformation, Version 1, database file header.  We use it to access the
	descriptive information supplied with the data file.

	Records in this file are 16 bytes long, i.e. two doubles.  The header
	information occupies the first 12 records.

	In this structure, the only information that is of interest
	to us is the maximum and minimum lat/longs and the the two
	grid values.  We ignore all of the rest of the stuff.

	NOTE: The Canadian algorithm is all based on longitudes
	being positive.  This is the reverse of the US algorithm.
*/
struct csGridFileCa1Hdr_
{
	char hdr_hdr [8];       /* Carries the word HEADER with two
							   blanks. */
	short hdr_cnt;          /* The number of 16 bytes records
							   devoted to the header.  Usually 12,
							   but who knows. */
	short fill_01;			/* Fill out to next boundary of 8,
							   using shorts since the SUN likes
							   to start character arrays on an
							   8 byte boundary. */
	short fill_02;			/* Ditto */
	short fill_03;			/* Ditto */
	char min_lat_lbl [8];   /* Carries "S LAT   " as a label for the
							   double which follows. */
	double min_lat;         /* The minimum latitude of the region
							   covered by the data file. */
	char max_lat_lbl [8];   /* Carries "N LAT   " as a label for the
							   double which follows. */
	double max_lat;         /* The maximum latitude of the region
							   covered by the data file. */
	char min_lng_lbl [8];   /* Carries "E LONG  " as a label for the
							   double which follows. */
	double min_lng;         /* The minimum longitude of the region
							   covered by the data file. */
	char max_lng_lbl [8];   /* Carries "W LONG  " as a label for the
							   double which follows. */
	double max_lng;         /* The maximum longitude of the region
							   covered by the data file. */
	char ngrid_lbl [8];     /* Carries the string "N GRID  " */
	double del_lat;         /* The latitude, in seconds, between
							   the grid cells in the data file. */
	char wgrid_lbl [8];     /* Carries the string "W GRID  " */
	double del_lng;         /* The longitude, in seconds, between
							   the grid cells in the data file. */
	char type_lbl [8];      /* Carries the string "TYPE    " */
	char type_val [8];      /* Carries the string "SECONDS " */
	char from_lbl [8];      /* Carries the string "FROM    " */
	char from_val [8];      /* Carries the string "NAD27   " */
	char to_lbl [8];        /* Carries the string "TO      " */
	char to_val [8];        /* Carries the string "NAD83   " */
	double fel_equ;         /* I believe this value is the equatorial
							   radius for the NAD27 ellipsoid. */
	double fel_plr;         /* I believe this value is the polar
							   radius for the NAD27 ellipsoid. */
	double tel_equ;         /* I believe this value is the equatorial
							   radius for the NAD83 ellipsoid. */
	double tel_plr;         /* I believe this value is the polar
							   radius for the NAD83 ellipsoid. */
};
#define cs_BSWP_GridFileCa1Hdr "8cs3s8cd8cd8cd8cd8cd8cd8c8c8c8c8c8cdddd"

