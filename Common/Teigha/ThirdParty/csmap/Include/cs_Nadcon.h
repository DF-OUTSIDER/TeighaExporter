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
**          US NADCON Data Files  (i.e. a .las and/or .los file)             **
**                                                                           **
*******************************************************************************
******************************************************************************/

/*	United States Grid File Object

These file types are used for several different purposes.  While
not crucial, we like to identify each of the grid structures as to
the purpose of the file it represents.

Applications in which this file format is used:
	1> NADCON 27 to 83 and vice versa (two files, *.LAS & *.LOS )
	2> HARN, aka HPGN.  (again two files, *.LAS & *.LOS)
	3> VERTCON	(single file, *.94)
	4> GEOID96  (single file, *.GEO)
*/
enum csNadconFileType {	nadconTypeNoneYet = 0,
						nadconTypeNad27LOS,
						nadconTypeNad27LAS,
						nadconTypeHarnLAS,
						nadconTypeHarnLOS,
						nadconTypeVertCon,
						nadconTypeGeoid96,
						nadconTypeUnknown = 999
					  };

/* The structures immediately following emulate the format of data in disk
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

/* US NADCON .LAS File and /LOS File
The following structure is used to represent a grid data file of the
NADCON format.  FOr horizontal applications, two are required to
complete a grid.  The file with the .LAS extension is used for the
latitude shifts, and the file with the .LOS extension is used for the
the longitude shifts.  Thus, for horizontal applications, a pair of
these structures is compbined into a single structure which represents
the complete geodetic transformation method.

The header information on a US Grid file has the following format.
The size of the header record is always equal to the size of a
record in the file.  Since a record in the file consists of an
entire row of grid points (west to east), the size of the header
is not fixed.  Note, that since a lot of this stuff is binary, byte
swapping may be required. */
struct csNadconFileHdr_
{
	char ident [56];        /* NADCON's identification.  We don't
							   use it. */
	char pgm [8];           /* Don't use it. */
	long32_t ele_cnt;       /* Number of data elements in each record.
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
#define cs_BSWP_NadconFileHdr "56c8clllfffff"

/* End of structures of the NTv2 format which appear on disk.  We now
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

/******************************************************************************
*******************************************************************************
**          cs_NadconFile_  -->  A Single NADCON file Object                 **
**                                                                           **
** While two such files are required to perform a datum shift (e.g. NAD27    **
** to NAD83), a single file is used for other purposes such as VERTCON       **
** vertical datum shifts and GEOID96 geoid heights.  Thus, is it convenient  **
** to have an object which can be used independently to access a single file **
** of this type.                                                             **
**                                                                           **
*******************************************************************************
******************************************************************************/

/* The following is what a US NADCON Grid file, regardless of its function,
   looks like in memory. */
struct cs_NadconFile_
{
	enum csNadconFileType type;			/* Type of file. */
	struct csGridCoverage_ coverage;
	long32_t elementCount;
	long32_t recordCount;
	long32_t recordSize;
	double deltaLng;
	double deltaLat;
	/* Files are now heavily buffered.  In fact, in most cases the
	   entire file resides in memory after the first access.  In such
	   cases, the file stream will be closed. */
	long32_t fileSize;				/* size of complete file on disk,
									   stream will be closed if
									   bufferSize == fileSize */
	csFILE* strm;					/* file is not opened until required,
									   may be closed if entire contents
									   have been buffered. */
	long32_t bufferSize;			/* size of buffer allocated */
	long32_t bufferBeginPosition;	/* file position of 1st byte in buffer,
									   -1 says nothing in buffer */
	long32_t bufferEndPosition;		/* file position of last byte in buffer,
									   -1 says nothing in buffer */
	void *dataBuffer;				/* not allocated until required, i.e.
									   file is actually opened. */
	short cellIsValid;				/* +1 indicates that the currentCell
									   element is valid. */
	struct csGridCell_ currentCell;	/* Last cell used. Upon construction,
									   the coverage element of this
									   structure is set so that it
									   won't match anything, thus
									   disabling it.  This additional
									   buffer does wonders for performance. */
	char filePath [MAXPATH];		/* Full path name to the file. */
	char fileName [16];				/* Last 15 characters of the actual
									   file name, used to generate
									   point data ID's. */
};

/* The following structure combines two of the NADCON Grid file
   structures for horizontal applications. */
enum csNadcnShiftType	{	nadconNone = 0,
							nadconNAD27Shift,
							nadconHARNShift,
							nadconUnknownShift = 999
						};
struct cs_Nadcn_
{
	enum csNadcnShiftType type;

	short usingVertcon;

	double errorValue;
	double cnvrgValue;
	short maxIterations;

	struct cs_NadconFile_* lngShift;
	struct cs_NadconFile_* latShift;
	
};

struct cs_NadconFile_* CSnewNadconFile (Const char* filePath,long32_t bufferSize,
															 ulong32_t flags,
															 double density);
void CSinitNadconFile (struct cs_NadconFile_ *thisPtr);
void CSreleaseNadconFile (struct cs_NadconFile_* thisPtr);
void CSdeleteNadconFile (struct cs_NadconFile_* thisPtr);
double CStestNadconFile (struct cs_NadconFile_* thisPtr,Const double *sourceLL);
int CSextractNadconFile (struct cs_NadconFile_* thisPtr,Const double* sourceLL);
int CScalcNadconFile (struct cs_NadconFile_* thisPtr,double* result,Const double* sourceLL);
