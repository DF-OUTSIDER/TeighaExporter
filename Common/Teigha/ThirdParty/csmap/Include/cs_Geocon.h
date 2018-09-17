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
**                                                                           **
**    US GEOCON (2012) Data Files  (e.g. dela?.b, delo?.b & dev.b files)     **
**                  (also geoid99.bin & g2012a??.bin files)                  **
**                                                                           **
**	This is the CS-MAP rendition of the US National Geodetic Survey's grid   **
**	system for converting from NAD83/HARN to NAD83/2011.  Thus, to get from, **
**	say, NAD27, to NAD83/2011 one needs to:                                  **
**		1> use NADCON to convert from NAD27 to NAD83, then                   **
**		2> Use HARN to convert from NAD83 to HARN, then                      **
**		3> Use GEOCON with the appropriate grid files to convert from        **
**		   HARN to NAD83/2011                                                **
**                                                                           **
**	As of this writing, we are assuming NAD83/2012 is just another           **
**	name for NAD83/2011. We are not at all sure exactly how to distinguish   **
**	between a 2007 and a 2011 grid data file.  Hopefully, we'll be able to   **
**	figure this all out when we have some code running which will enable to  **
**	examine the results using different grid data files.                     **
**                                                                           **
**	We have used a 5 character naming system since the origin of CS-MAP      **
**  in 1987 (when function names were limited to 8 characters (seven in      **
**	some compiler/linker products).  Thus, we have chosen to use the         **
**	following names as appropriate in the Geodetic Transformation naming     **
**	convention: "geocn", "cs_Geocn_".                                        **
**                                                                           **
**	Note that the grid file formats and calculation algorithms are the same  **
**	as that used in the NGS's Geoid 1999 grid effort.  Thus, we have most    **
**	all of the code necessary to implement this new grid file based          ** 
**	transformation.  Question as I write this comment: Do we isolate the     **
**	Geoid99 code and rename it so that it takes on the more generic          **
**	function which has mow made itself evident?  Think so, especially since  **
**	data files in this same format are used for the vertical portion of the  **
**	base geodetic transformation, and also for using the quality of          **
**	conversion data files associated with this geodetic transformation.      **
**                                                                           **
**	Another reason for isolating/renaming/refactoring is that our new        **
**	Geodetic Transformation paradigm requires several new functions which    **
**	do not exist in the original Geoid 1999 implementation.                  **
**                                                                           **
**	So, the end result of these deliberations is that much of what you       **
**	find in this header file and its corresponding implementation file(s)    **
**	is a cut/paste/replace of the code previously existing in cs_map.h and   **
**	CS_geoid99.c with struct and function names such as csGeoid99GridFile_   **
**	and CSnewGeoid99GridFile.  The new names will adhere to the naming       **
**	system used in the rewrite of Geodetic Transformations initiated by      **
**	RFC 2.                                                                   **
**                                                                           **
**	The actual, "official", NGS code for doing this transformation           **
**	is written in (drum roll please) FORTRAN.  For integrity purposes, the   **
**	actual FORTRAN code is included as a huge comment at the tail end of     **
**	implementation file associated with this header file.                    ** 
**                                                                           **
**	The data files for this geodetic conversion, as it applies to
**	NAD832007 and(?) NAD83/2011 come in groups of 6.  Our implementation     **
**  relies pretty heavily on this nameing convention in order to get the     **
**	data files properly paired with each other.  Since the coverage and      **
**	grid cell size are the same in each of a paired set, a comparison of     **
**	these values is done at construction time to insure that the files are   **
**	being properly paired with each other.                                   **     
**                                                                           **
**	dela.b		--> conterminous 48 states, latitude shift                   **
**	delo.b		--> conterminous 48 states, longitude shift                  **
**	dev.b		--> conterminous 48 states, vertical shift                   **		
**	dsla.b		--> conterminous 48 states, latitude shift quality estimate  **
**	dslo.b		--> conterminous 48 states, longitude shift quailty estimate **
**	dsv.b		--> conterminous 48 states, vertical shift quality estimate  **
**                                                                           **
**	delaa.b		--> Alaska, latitude shift                                   **
**	deloa.b		--> Alaska, longitude shift                                  **
**	deva.b		--> Alaska, vertical shift                                   **
**	dslaa.b		--> Alaska, latitude shift quality estimate                  **
**	dsloa.b		--> Alaska, longitude shift quailty estimate                 **
**	dsva.b		--> Alaska, vertical shift quality estimate                  **
**                                                                           **
**	delap.b		--> Puerto Rico/Virgin Is, latitude shift                    **
**	delop.b		--> Puerto Rico/Virgin Is, longitude shift                   **
**	devp.b		--> Puerto Rico/Virgin Is, vertical shift                    **
**	dslap.b		--> Puerto Rico/Virgin Is, latitude shift quality estimate   **
**	dslop.b		--> Puerto Rico/Virgin Is, longitude shift quailty estimate  **
**	dsvp.b		--> Puerto Rico/Virgin Is, vertical shift quality estimate   **
**                                                                           **
**	Presumably, in the future there may be an 'h' series for Hawaii, but     **
**	no such data files have been found for Hawaii.                           **
**                                                                           **
**	Finally, there are files with names such as infoh.txt, infoha.txt, and   **
**	infohp.txt.  These files contain the locations of holes in the           **
**	related grid files.  Currently, we do not use them at all, although      **
**	you might see some "future use provisions" for such.                     **
**                                                                           **
*******************************************************************************
******************************************************************************/

/*	United States 2012 Grid File Object  (GEOCON)

These file types are used for several different purposes.  While
not crucial, we like to identify each of the grid structures as to
the purpose of the file it represents.

Applications in which this file format is used:

	1> NAD83/HARN to NAD83/2012 geodetic cconversions,
	   both horizontal and vertical  (ext = .b)
	2> Geoid 2012 Height calculation (ext = .bin)
	3> Geoid 2009 Height calculation (ext = .bin)
	4> Geoid 2003 Height calculation (ext = .bin)
	5> Geoid 1999 Height calculation (ext = .bin)

Note sure at the current time if we'll be able to distinguish a 2007
grid file from a 2007 grid file.  Thus, this distinction may disappear.
*/

/* The following very arbitrary value is returned by functions
   which typically return a grid shift value extracted from a
   GEOCON grid file when an error is detected. This condition
   is quite unimaginable, so a huge number which will not
   go unnoticed in reaults has been chosen. */
#define csGEOCON_SHIFT_ERROR -9.9E+12;

enum csGeocnFileType {	geocnTypeNoneYet = 0,
						geocnTypeLatShift,
						geocnTypeLngShift,
						geocnTypeHgtShift,
						geocnTypeGeoidHgt,
						geocnTypeUnknown = 999
					  };

enum csGeocnEdgeEffects {	geocnEdgeNone = 0,
							geocnEdgeSouthwest,
							geocnEdgeSouth,
							geocnEdgeSoutheast,
							geocnEdgeEast,
							geocnEdgeNortheast,
							geocnEdgeNorth,
							geocnEdgeNorthwest,
							geocnEdgeWest,
							geocnEdgeUnknown
					    };

/* Geocon Grid Cell Object -->  Used to hold the definition of a grid cell.
   A Geocon frid cell is similar in function to the grid cell (with an
   unqualified name) which is used in the NADCON and NTv2 datum shift
   calculations. However, a Geocon grid cell carries nine data values
   in a array of nine doubles. The interpolation is therefore, a bit more
   mathematically rigourous that the standard bi-linear interpolation used
   in the NADCON and NTv2 environments.

   The grid cell is identified by the grid indices (zero based in our case)
   of the central data element. The data extraction algorithm always extracts
   a cell such that the point being converted is closer to the central
   point than any of the others.  Thus, when determining if we can use
   a specific grid cell, we will use the integral indicies rather than
   a range of coverage type calculation.

   The array of doubles is used as follows:
   
		IDX            [0]   [1]   [2]

		                    NORTH

		[2]     W      nw     n    ne      E
		[1]     E      w     ctr    e      A
		[0]     S      sw     s    se      S
		  	    T                          T 
				            SOUTH

   Note that the first record in the data files is the southernmost record,
   amd that the values in each record start at the western edge and run to
   the eastern edge (i.e. left to right, west to east).

*/
struct csGeocnGridCell_
{
	short isValid;				/* TRUE if this cell accurately represents data
								   data extracted from the associated grid data
								   file; otherwise FALSE. */
	short cellAge;				/* A cell is aged (i.e. the age element incremented
								   by one) for each conversion which takes place.
								   A cell's age is set to zero whenever the cell is
								   actually used to performa a conversion.  This
								   is intended to support a possible future
								   performance enhancement of a grid cell cahce.
								   This is not currently implemented.*/
	enum csGeocnEdgeEffects edge;
								/* The appropriate edge effect calculation in
								   force for this grid cell. */
	long32_t lngIdx;			/* longitude grid file index value associated
								   with cellData [0][0] element of the grid
								   cell. */
	long32_t latIdx;			/* latitude grid file index value associated
								   with cellData [0][0] element of the grid
								   cell. */
	double swLng;				/* Longitude, in degrees, of the southwest
								   point of the southwest value of the 3x3 grid
								   cell matrix. */
	double swLat;				/* Latitude, in degrees, of the southwest
								   point of the southwest value of the 3x3 grid
								   cell matrix. */
	double deltaLng;			/* dimensions of cell, in degrees; for
								   Geocon files this is often 0.0166666666666667,
								   a rather ugly number for us to deal with. */
	double deltaLat;			/* dimensions of cell, in degrees; for
								   Geocon files this is often 0.0166666666666667,
								   a rather ugly number for us to deal with. */

	double cellData [3][3];		/* For performance purposes, cells carry data
								   values pre-converted to doubles. */

	char sourceId [32];			/* File name of the source of the grid cell data. */
};

/* The following represents the data contained in the header of
   a GEOCON grid data file.  Note, to avoid compiler packing
   issues, we do not read this structure directly from disk;
   each element is read individually and stored in a structure
   of this form simply for covenience.
   
   Note that the latitude and longitude values in this structure
   are assumed to be NAD83/HARN based.  Further, we assume that
   for geoid height calculations, the datum to which the geographic
   coordinates are based are considered immaterial, but probably
   the datum of the indicated geoid.
   
   Note, that the grid carried in one of these files, given the
   information extracted from the header, is essentially as follows:
   
	   float cs_GeocnGridFileData [latCount][lngCount];
   
   where element [0][0] is the grid shift value for a point
   precisely at latitude = latMin and longitude = lngMin.

   Horizontal datum shift values are floats, in	arc-seconds * 100,000.00 (~ 0.3 mm)
   Verical datum shift values are floats in centimeters.
   Geoid height data elements are in meters.
*/
struct cs_GeoconFileHdr_
{
	double latMin;			/* Southernmost latitude covered in the file,
							   in degrees. */
	double lngMin;			/* Westernmost longitude covered in the file,
							   0 thru 360, positive east from Greenwich. */
	double latDelta;		/* Latitudinal grid cell size, in degrees. */
	double lngDelta;		/* Longitudinal grid cell size, in degrees. */
	long32_t latCount;		/* Latitudinal size of the grid, i.e. number of
							   rows datum shift records in the grid file. */
	long32_t lngCount;		/* Longitudinal size of the grid, i.e. the number
							   of datum shift values in each record of the
							   grid file. */
	long32_t iKind;			/* Generally, a value of +1 means no swapping
							   required, i.e. the file is in byte order of
							   the host platform already.  Any other value
							   probably means swapping is required on
							   the current host machine. */
	long32_t headerSize;	/* Actual size of the header in the file
							   from which it was extracted; not necessarily
							   the sizeof this structure for several reasons.*/
};
#define cs_BSWP_GeocnHdr "ddddlll"

/******************************************************************************
*******************************************************************************
**          cs_GeoconFile_  -->  A Single GEOCON file Object                 **
**                                                                           **
**	While two such files are required to perform a datum shift (e.g. HARN    **
**	to NAD83/2007), a single file is used for other purposes such as geoid   **
**	height calculations.  Thus, is it convenient to have an object which can **
**	be used independently to access a single file of this type.              **
**                                                                           **
**	While not really used in any way, we do our best to classify the file    **
**	type and purpose to aid in debugging. The need for byte swappping is     **
**	determined by a static data constant which indicates the byte order of   **
**	the processor we are running on, and examination of the data in the      **
**	file header.  I.e. the first two elements in the header are 8 byte       **
**	doubles which are a valid latitude and a valid ongitude values.  Clearly **
**	we swap if necessary to get values which meet this criteria.			 **
**                                                                           **
**	Files are now heavily buffered.  In fact, in some cases the	entire file  **
**	resides in memory after the first access.  In such cases, the file       **
**	stream will be closed.  Note, the typical GEOCON Grid Data file is more  **
**	than 25 megabytes.  Since two are required for a horizontal datum shift, **
**  buffering the entire file(s) would normally consume 50 megabytes of      **
**	memory.  We won't do that unless it is really necessary for performance  **
**  purposes.                                                                **
**                                                                           **
**	Please note that the grid cell size on GEOCON files is often set to      **
**	rather painful values.  I.e. the most popular of the several files uses  **
**	a geoid cell size of 0.0166666667 degrees (one minute of arc).  This is  **
**	is a rather unfriendly value to work with in a digital computer and      **
**	produces all sorts of strange bugs when working on the edge of a grid    **
**	cell or the entire grid itself.  Watch out for these problems.  Make     **
**	sure that, when using these types of values, you get extended precision  **
**	rather than just what appears in the files.  To accomplish this, you are **
**	likely to find that min/max type calculations are done using integers    **
**	carrying seconds of arc to insure maximum precison is obtained.          **
**                                                                           **
*******************************************************************************
******************************************************************************/

/* The following is what a US GEOCON Grid file, looks like in memory,
   regardless of its function.
*/

struct cs_GeoconFile_
{
	enum csGeocnFileType type;			/* Type of file. */

	double latMin;						/* Latitude of the the southwest corner
										   of the grid in decimal degrees. */
	double lngMin;						/* Longitude of the the southwest corner
										   of the grid in decimal degrees.
										   This value rangs from 0.0 to +360.0. */
	double latMax;						/* Latitude of the northeast corner
										   of the grid in decimal degrees. */
	double lngMax;						/* Longitude of the the northeast corner
										   of the grid in decimal degrees.
										   This value rangs from 0.0 to +360.0. */

	double deltaLng;					/* size of a grid cell in degrees */
	double deltaLat;					/* size of a grid cell in degrees */
	double density;						/* smaller of the two deltas, just
										   in case they ever are different. */

	long32_t elementCount;				/* elements per record */
	long32_t recordCount;				/* number of records in the file */
	long32_t elementSize;				/* size of the internal elements,
										   i.e. sizeof (float). */
	long32_t headerSize;				/* size of the file header, in
										   bytes. */
	long32_t recordSize;				/* size in bytes of a record for
										   I/O and buffer management
										   purposes. This includes the 32 bit
										   record size word that FORTRAN likes
										   to put on the fron and end of records
										   in an unformatted (i.e. binary) data
										   file. */
	long32_t fileSize;				/* size of complete file on disk,
									   stream will be closed if
									   bufferSize == fileSize */
	long32_t bufferSize;			/* size of buffer allocated */
	long32_t bufferBeginPosition;	/* file position of 1st byte in buffer,
									   -1 says nothing in buffer */
	long32_t bufferEndPosition;		/* file position of last byte in buffer,
									   -2 says nothing in buffer */
	csFILE* strm;					/* file is not opened until required,
									   may be closed if entire contents
									   have been buffered. */
	void *dataBuffer;				/* not allocated until required, i.e.
									   file is actually opened. */
	short swapBytes;				/* True (+1) indicates that byte swapping
									   is necessary in the current
									   environment. */
	char filePath [MAXPATH];		/* Full path name to the file. */
	char fileName [32];				/* Last 31 characters of the actual
									   file name (sans ext); used to generate
									   point data ID's. */
	struct csGeocnGridCell_ gridCell;
									/* Last used grid cell. */
};

/* The following structure combines three GEOCON Grid file structures into a
   single object capable of doing a 2D/3D datum shift, i.e. a horizontal datum
   shift or a horizontal and vertical datum shift.

   Since this is a grid file interpolation method based on just another grid
   file format, the function protocols are designed to fit within the design
   of the existing grid file.
*/

struct cs_Geocn_
{
	short verticalAvailable;	/* Redundant? Equivalent to hgtShift != NULL */

	double minLng;				/*    0.0 <= minLng <  360.0 */
	double minLat;				/*  -90.0 <  minLat <=  90.0 */
	double maxLng;				/* minLng <  maxLng <= 360.0 */
	double maxLat;				/*  -90.0 <  minLat <=  90.0 */
	double density;				/* smaller of the two cell dimensions in degrees,
								   usually 0.0166666667 for these types of files. */

	/* The two dimensional inverse reqires iteration.  Thus, the following
	   values are used to deal with the remote possibility that the interation
	   doesn't converge.
	   
	      *	If, after maxIterations, the delta value remains >= errorValue,
			the transformation is reported as a fatal error.
		  * If, after maxIterations, the delta value is less than errorValue
		    but >= convergeValue, the transformation is treated as a failure
		    to converge.
		  * The iteration loop is considered successful and complete when the
			delta value is lkess than cnvrgValue.
		  *	The loop never iterates more than maxIterations.
	*/
	double errorValue;
	double cnvrgValue;
	short maxIterations;

	struct cs_GeoconFile_* lngShift;
	struct cs_GeoconFile_* latShift;
	struct cs_GeoconFile_* hgtShift;	/* Could be null if vertical option
										   is not activated. */
};

struct cs_GeoconFile_* CSnewGeoconFile (Const char* filePath,long32_t bufferSize,
															 ulong32_t flags,
															 double density,
															 enum csGeocnFileType fileType);
int CSpathsGeoconFile (char *pathBufferLng,char *pathBufferLat,char *pathBufferHgt,
															   Const char *pathBuffer);
int CSheaderGeoconFile (struct cs_GeoconFileHdr_ *thisPtr,csFILE *fstr);
int CSchkHdrGeoconFile (struct cs_GeoconFileHdr_ *thisPtr);

void CSinitGeoconFile (struct cs_GeoconFile_ *thisPtr);
void CSreleaseGeoconFile (struct cs_GeoconFile_* thisPtr);
void CSdeleteGeoconFile (struct cs_GeoconFile_* thisPtr);
double CStestGeoconFile (struct cs_GeoconFile_* thisPtr,Const double *sourceLL);
int CScalcGeoconFile (struct cs_GeoconFile_* thisPtr,double* result,Const double* sourceLL);
int CSextractGeoconGridFile (struct csGeocnGridCell_ *gridCellPtr,struct cs_GeoconFile_* thisPtr,
																  long32_t eleNbr,
																  long32_t recNbr);
int CSreadGeoconGridFile (struct cs_GeoconFile_* thisPtr,long32_t recNbr);
double CSvalueGeoconGridFile (struct cs_GeoconFile_* thisPtr,long32_t eleNbr,
															 long32_t recNbr);
int CSedgeGeocnGridCell (struct csGeocnGridCell_ *cellPtr,struct cs_GeoconFile_* filePtr,
														  long32_t eleNbr,
														  long32_t recNbr,
														  enum csGeocnEdgeEffects edge);
void CSinitGeocnGridCell (struct csGeocnGridCell_* thisPtr);
double CScalcGeocnGridCell (struct csGeocnGridCell_* thisPtr,Const double *sourceLL);
const char *CSsourceGeocnGridCell (struct csGeocnGridCell_* thisPtr);
double EXP_LVL9 CSgeocnQterp (struct csGeocnGridCell_* thisPtr,double sourceDelta [2]);
double EXP_LVL9 CSgeocnQterpOne (double delta,double rowData [3]);
