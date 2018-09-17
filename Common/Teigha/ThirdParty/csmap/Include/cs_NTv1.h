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

/******************************************************************************
*******************************************************************************
**             Canadian National Transformation, Version 1                   **
*******************************************************************************
******************************************************************************/

/* The structure immediately following emulates the format of data in disk
   files.  Thus, structure packing becomes critical.  We establish the packing
   we need based on the specific compilers we know something about. */
#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a1
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members (1)
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack (1)
#elif _RUN_TIME == _rt_HPUX
#	pragma pack 1
#elif _RUN_TIME == _rt_AIX
#	pragma options align=packed
#elif _RUN_TIME >= _rt_UNIXPCC
#	pragma pack (1)
#else
	/* I don't know what would work for WATCOM.  You may need to
	   add your own stuff here depeneding on the compiler.  You need
	   to enforce a packing factor of 1; i.e. the compiler MUST not
	   add any packing to the structure for alignment. */
#endif

/* The following structure defines the format of the Canadian	National
   Transformation, Version 1, database file header.  We use it to access the
   descriptive information supplied with the data file.

   Records in this file are 16 bytes long, i.e. two doubles.  The header
   information occupies the first 12 records.

   In this structure, the only information that is of interest to us is the
   maximum and minimum lat/longs and the the two grid density values.  We
   ignore all of the rest of the stuff.

   NOTE: The Canadian algorithm is all based on longitudes being positive.
   This is the reverse of the US algorithm.
*/
struct csNTv1Hdr_
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
#define cs_BSWP_NTv1Hdr "8cs3s8cd8cd8cd8cd8cd8cd8c8c8c8c8c8cdddd"

/* End of structures of the NTv1 format which appear on disk.  We now
   re-establish the normal packing for in memory structures only. */
#if   _RUN_TIME == _rt_BORLAND16 || _RUN_TIME == _rt_BORLAND32
#	pragma option -a.
#elif _RUN_TIME == _rt_METAWARE
#	pragma Align_members ()
#elif _RUN_TIME == _rt_MSC16 || _RUN_TIME == _rt_MSVC16 || _RUN_TIME == _rt_MSDOTNET || \
														   _RUN_TIME == _rt_MSWIN64 ||  \
														   _RUN_TIME == _rt_MSVC32
#	pragma pack ()
#elif _RUN_TIME == _rt_HPUX
#	pragma pack
#elif _RUN_TIME == _rt_AIX
#   pragma options align=reset
#elif _RUN_TIME >= _rt_UNIXPCC
#   pragma pack ()
#else
		/* I don't know what would work for WATCOM. */
#endif

/* The following structure represents the im memory object used to implement
   the Canadian National Transformation, Version 1.  This is obsolete.  You
   will not be able to obtain a copy of the version 1 data file from any of
   the normal sources.
   
   The version 1 data file is a simple, one grid, data file with some header
   information on the front.  As is the case with version 2, west longitude
   is considered positive which makes this gird different from most others.
   
   The software which produced this file was written in FORTRAN.  As such,
   the producing application produced a file with 16 byte records.  The
   typical data element is a 16 byte record with consists of two IEEE
   doubles. */
struct cs_NTv1_
{
	struct csGridCoverage_ coverage;
	long32_t headerCount;				/* number of 16 byte header records */
	long32_t elementCount;				/* number of data elements in a record */
	long32_t recordCount;				/* number of longitude rows in the file. */
	long32_t recordSize;				/* size of a longitude row */
	double deltaLng;					/* grid cell size in degrees */
	double deltaLat;					/* grid cell size in degrees */
	long32_t fileSize;					/* size of complete file on disk,
										   stream will be closed if
										   bufferSize == fileSize */
	csFILE* strm;						/* file is not opened until required,
										   may be closed if entire contents
										   have been buffered. */
	long32_t bufferSize;				/* size of buffer allocated */
	long32_t bufferBeginPosition;		/* file position of 1st byte in buffer,
										   -1 says nothing in buffer */
	long32_t bufferEndPosition;			/* file position of last byte in buffer,
										   -1 says nothing in buffer */
	void *dataBuffer;					/* not allocated until required, i.e.
										   file is actually opened. */
	short CellIsValid;					/* +1 indicates that the longitude and
										   latitude cells are valid. */
	struct csNTv2GridCell_ longitudeCell;
										/* Last cell used, the longitude data. */
	struct csNTv2GridCell_ latitudeCell;
										/* Last cell used, the longitude data. */
	char filePath [MAXPATH];			/* Full path name to the file. */
	char FileName [16];					/* Last 15 characters of the actual
										   file name, used to generate
										   point data ID's. */
};
