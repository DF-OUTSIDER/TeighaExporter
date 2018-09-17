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

/*
	This file contains the implementation of the Geoi99 object.
	Geoid99 is a slight variation of the Geoid 96 thing, which
	is itself a slight variation of the standard US Geodetic
	Grid thing (i.e. NADCON).

	ANywho, the implementation, use, and purpose are different
	enough to warrant a separate set of implementation code.
*/

extern char csErrnam [MAXPATH];

/*****************************************************************************
	'Private' support function
*/
void CSinitGeoid99 (struct csGeoid99GridFile_ *__This)
{
	CSinitCoverage (&__This->coverage);
	__This->elementCount = 0;
	__This->recordCount = 0;
	__This->recordSize = 0;
	__This->deltaLng = 0.0;
	__This->deltaLat = 0.0;
	__This->strm = NULL;
	__This->bufferSize = 0L;
	__This->bufferBeginPosition = 0;
	__This->bufferEndPosition = 0;
	__This->dataBuffer = NULL;
	__This->iKind = 1L;
	__This->filePath [0] = '\0';
	__This->fileName [0] = '\0';
}

/*****************************************************************************
	Constructor
*/
struct csGeoid99GridFile_* CSnewGeoid99GridFile (Const char *path,long32_t bufferSize,ulong32_t flags,double density)
{
	extern double cs_K360;
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];

	size_t readCount;
	long lngTmp;
	double lngMin, lngMax;
	char *cp1, *cp2;
	csFILE *fstr;
	struct csGeoid99GridFile_* __This;
	char cTemp [MAXPATH];
	struct csGeoid99Hdr_ geoid99Hdr;

	/* Prepare for an error. */
	__This = NULL;
	fstr = NULL;

	/* Malloc and initialize */
	__This = CS_malc (sizeof (struct csGeoid99GridFile_));
	if (__This == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	CSinitGeoid99 (__This);

	/* Set default values for all members. */
	__This->bufferSize = bufferSize;
	if (__This->bufferSize < 0) __This->bufferSize = 0;

	/* Save file path/name. */
	CS_stncp (__This->filePath,path,sizeof (__This->filePath));
	CS_stncp (cTemp,path,sizeof (cTemp));

	/* Set up the type of file.  Get cp1 to point at the file
	   name, and cp2 to point at the extension.  We consider it
	   an error if we are not given a full path name.  Note,
	   we care not about the format of the drive specification.
	   But there must be at least one directory and there must
	   be an extension. */
	cp1 = strrchr (cTemp,cs_DirsepC);
	if (cp1 == NULL)
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}
	cp1 += 1;
	cp2 = strchr (cp1,cs_ExtsepC);
	if (cp2 == NULL)
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}
	*cp2++ = '\0';
	CS_stncp (__This->fileName,cp1,sizeof (__This->fileName));

	/* The thing should have a .bin extension to be processed by us. */
	if (CS_stricmp (cp2,"bin"))
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Get the file information header. */
	fstr = CS_fopen (__This->filePath,_STRM_BINRD);
	if (fstr == NULL)
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}
	readCount = CS_fread (&geoid99Hdr,1,sizeof (geoid99Hdr),fstr);
	if (readCount != sizeof (geoid99Hdr))
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}
	if (CS_ferror (fstr))
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_IOERR);
		goto error;
	}

	/* Determine the size of the file. */
	if (CS_fseek (fstr,0L,SEEK_END))
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_IOERR);
		goto error;
	}
	__This->fileSize = CS_ftell (fstr);
	if (__This->fileSize < 0L)
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_IOERR);
		goto error;
	}

	/* This constructor doesn't need the file any more. */
	CS_fclose (fstr);
	fstr = NULL;

	/* Swap the bytes if necessary.  In all the data files I've seen, iKind is 32
	   bit 1.  If it is a one in the structure, than the file is in the same byte
	   order as the machine we are running on, and swapping is unnecessary.  If
	   iKind is not a one, we assume that is because of byte order, and we do a
	   swap.  Note, however, that we preserve iKind as it was before swapping so
	   that we know whether to swap or not in the other functions in this code
	   module. */

	__This->iKind = geoid99Hdr.iKind;
	if (geoid99Hdr.iKind != 1L)
	{
		/* CSbswap is a variation on CS_bswap.  CSbswap always swaps. */
		CSbswap (&geoid99Hdr,cs_BSWP_Geoid99Hdr);
	}

	/* At this point, geoid99Hdr.iKind should be a one, or either the file
	   is corrupted, or we don't understand the file format properly. */

	if (geoid99Hdr.iKind != 1L)
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Extract the important stuff from the header.  Note that the longitude in the header
	   is a zero thru 360 value, proceeding east from Greenwich.  This makes a lot of sense
	   since Alaska crosses the 190 degree crack.  However, it is inconsistent with
	   all the other stuff.  So, we do the following kludge, which is consistent with the
	   NADCON data files. */
	lngMin = geoid99Hdr.lngMin;
	lngMax = lngMin + (geoid99Hdr.lngDelta * (geoid99Hdr.lngCount - 1));
	if (lngMin >= 180.0 || lngMax > 180.0)
	{
		__This->coverage.southWest [LNG] = geoid99Hdr.lngMin - cs_K360;
	}
	else
	{
		__This->coverage.southWest [LNG] = geoid99Hdr.lngMin;
	}
	__This->coverage.southWest [LAT] = geoid99Hdr.latMin;
	__This->deltaLng = geoid99Hdr.lngDelta;
	__This->deltaLat = geoid99Hdr.latDelta;
	__This->coverage.northEast [LNG] = __This->coverage.southWest [LNG] + (geoid99Hdr.lngDelta * (geoid99Hdr.lngCount - 1));
	__This->coverage.northEast [LAT] = __This->coverage.southWest [LAT] + (geoid99Hdr.latDelta * (geoid99Hdr.latCount - 1));
	__This->coverage.density = (__This->deltaLng < __This->deltaLat) ? __This->deltaLng : __This->deltaLat;
	if (density != 0.0) __This->coverage.density = density;
	__This->elementCount = geoid99Hdr.lngCount;
	__This->recordCount = geoid99Hdr.latCount;
	__This->recordSize = geoid99Hdr.lngCount * (int)sizeof (float);

	/* Verify the integrity of the file. */
	lngTmp = __This->recordCount * __This->recordSize + sizeof (geoid99Hdr);
	if (lngTmp != __This->fileSize)
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Now that we know recordSize, we can adjust the bufferSize for maximum
	   efficiency. */
	if (__This->bufferSize > __This->fileSize)
	{
		__This->bufferSize = __This->fileSize;
	}
	else
	{
		if (__This->bufferSize > (3 * __This->recordSize))
		{
			/* Maximum efficiency is obtained with a buffer size whch is
			   a multiple of the record size. */
			__This->bufferSize = (__This->bufferSize / __This->recordSize) * __This->recordSize;
		}
		else
		{
			/* We require a minimum buffer size of 3 records. */
			__This->bufferSize = 3 * __This->recordSize;
		}
	}

	return (__This);
error:
	CSdeleteGeoid99GridFile (__This);
	return NULL;
}

/*****************************************************************************
	Destructor
*/
void CSdeleteGeoid99GridFile (struct csGeoid99GridFile_* __This)
{
	if (__This != NULL)
	{
		if (__This->strm != NULL) CS_fclose (__This->strm);
		if (__This->dataBuffer != NULL)	CS_free (__This->dataBuffer);
		CS_free (__This);
	}
}

/*****************************************************************************
	Release resources
*/
void CSreleaseGeoid99GridFile (struct csGeoid99GridFile_* __This)
{
	if (__This != NULL)
	{
		/* Close the file and release the buffer.  Leave the file size,
		   buffer size, coverage information, and other stuff intact.
		   The only purpose here is to release allocated resources. */

		/* Free and invalidate the buffer, if there is one. */
		if (__This->dataBuffer != NULL)
		{
			CS_free (__This->dataBuffer);
		}
		__This->dataBuffer = NULL;
		__This->bufferBeginPosition = -1L;
		__This->bufferEndPosition = -2L;

		/* Close the file (if open). */
		if (__This->strm != NULL)
		{
			CS_fclose (__This->strm);
		}
		__This->strm = NULL;
	}
	return;
}

/*****************************************************************************
	Coverage Test
*/
double CStestGeoid99GridFile (struct csGeoid99GridFile_* __This,Const double *sourceLL)
{
	/* Returns zero if not covered, file density if covered. */
	return CStestCoverage (&(__This->coverage),sourceLL);
}

/*****************************************************************************
	This function would be a private function in C++.
	This function sets the currentCell member to the cell which covers the
	provided geographic coordinate.  Returns 0 on success or a negative value
	for a system error of some sort (i.e. file I/O).  Per the design of this
	object, this function is not to be called unless the provided source
	coordinate is within the coverage of the object.
*/
/*lint -e826  disable message on fltPtr = (float *)(chrPtr) used extensively here */
int CScalcGeoid99GridFile (struct csGeoid99GridFile_* __This,double* result,Const double* sourceLL)
{
	extern double cs_One;

	int eleNbr;
	int recNbr;
	int readCount;
	int checkCount;

	long lngTmp;
	long checkSeek;
	long fpos;
	long fposBegin;
	long fposEnd;
	long swapCount;

	char* chrPtr;
	float* fltPtr;

	double centerLL [2];
	double deltaLL [2];
	
	char swapSpec [16];

	/* Float array carries the 3x3 array as follows:
		         North

		W       6  7  8      E
		e       3  4  5      a
		s       0  1  2      s
		t                    t 
			     South          */
	float array [9];

	enum edgeEffects {	edgeNone = 0,
						edgeSouthwest,
						edgeSouth,
						edgeSoutheast,
						edgeEast,
						edgeNortheast,
						edgeNorth,
						edgeNorthwest,
						edgeWest
					 } edge;
	edge = edgeNone;

	/* We are not supposed to get here unless the sourceLL is within the coverage of the
	   file object.  We make sure of that now.  This makes life much easier below.  Use the
	   CStestGeoid99GridFile function to select the proper csGeoid99GridFile object. */
	if (sourceLL [LNG] < __This->coverage.southWest [LNG] || sourceLL [LNG] > __This->coverage.northEast [LNG] ||
	    sourceLL [LAT] < __This->coverage.southWest [LAT] || sourceLL [LAT] > __This->coverage.northEast [LAT])
	{
		CS_stncp (csErrnam,"CS_geoid99:1",MAXPATH);
		CS_erpt  (cs_ISER);
		goto error;
	}
	   
	/* Compute the basic indices to the cell in the data file.  We're supposed to come up
	   with a reference to the closest point.  Since we know the source coordinate is within
	   range, dealing with the edges is rather easy. */
	eleNbr = (long)(((sourceLL [LNG] - __This->coverage.southWest [LNG]) / __This->deltaLng) + 0.5);
	recNbr = (long)(((sourceLL [LAT] - __This->coverage.southWest [LAT]) / __This->deltaLat) + 0.5);

	/* Determine the if an edge effect applies. */
	if (recNbr < 1)
	{
		recNbr = 1;
		if (eleNbr < 1)
		{
			eleNbr = 1;
			edge = edgeSouthwest;
		}
		else if (eleNbr >= (__This->elementCount - 1))
		{
			eleNbr = __This->elementCount - 2;
			edge = edgeSoutheast;
		}
		else
		{
			edge = edgeSouth;
		}
	}
	else if (recNbr >= (__This->recordCount - 1))
	{
		if (eleNbr < 1)
		{
			eleNbr = 1;
			edge = edgeNorthwest;
		}
		else if (eleNbr >= (__This->elementCount - 1))
		{
			eleNbr = __This->elementCount - 2;
			edge = edgeNortheast;
		}
		else
		{
			edge = edgeNorth;
		}
	}
	else
	{
		if (eleNbr < 1)
		{
			eleNbr = 1;
			edge = edgeWest;
		}
		else if (eleNbr > (__This->elementCount -1))
		{
			eleNbr = __This->elementCount - 2;
			edge = edgeEast;
		}
		else
		{
			edge = edgeNone;
		}
	}

	/* Compute the minimal region of the file which we need to read. */
	fposBegin = sizeof (struct csGeoid99Hdr_) + (recNbr - 1) * __This->recordSize;
	fposEnd = fposBegin + (__This->recordSize * 3);
	if (fposEnd > __This->fileSize) fposEnd = __This->fileSize;
	
	/* Do we have a buffer?  Could have been released.  Maybe this is the
	   first access. */
	if (__This->dataBuffer == NULL)
	{
		__This->dataBuffer = CS_malc ((size_t)__This->bufferSize);
		if (__This->dataBuffer == NULL)
		{
			CS_erpt (cs_NO_MEM);
			goto error;
		}

		/* Make sure the rest of this stuff knows the buffer is empty.  These values
		   will fail to match any specific file position. */
		__This->bufferBeginPosition = -1L;
		__This->bufferEndPosition = -2L;
	}

	/* See if the stuff we want is in the buffer.  Careful here, all of the intended
	   range must be in the buffer, not just a portion of it. */
	if (fposBegin < __This->bufferBeginPosition || fposBegin > __This->bufferEndPosition ||
		fposEnd   < __This->bufferBeginPosition || fposEnd   > __This->bufferEndPosition)
	{
		/* The data we need is not there; we need to read it in.  Is the file open? */
		if (__This->strm == NULL)
		{
			__This->strm = CS_fopen (__This->filePath,_STRM_BINRD);
			if (__This->strm == NULL)
			{
				CS_stncp (csErrnam,__This->filePath,MAXPATH);
				CS_erpt (cs_DTC_FILE);
				goto error;
			}
			/* We do our own buffering, turn stream buffering off. */
			setvbuf (__This->strm,NULL,_IONBF,0);
		}

		/* Compute the starting position of the actual read. */
		if (__This->bufferSize >= __This->fileSize)
		{
			__This->bufferBeginPosition = 0L;
			__This->bufferEndPosition = __This->fileSize;
			readCount = __This->fileSize;
		}
		else
		{
			/* We need to doa partial read, the normal case.  Initialize for the
			   minimal case computed above, then expand as is possible.  Note, we
			   get here only when a read has to be done. */
			__This->bufferBeginPosition = fposBegin;
			__This->bufferEndPosition = fposEnd;
			readCount = __This->bufferEndPosition - __This->bufferBeginPosition;

			/* In this section, lngTmp is the number of additional records
			   which can fit in the buffer. */
			lngTmp = (__This->bufferSize - readCount) / __This->recordSize;
			if (lngTmp > 3L)
			{
				/* Move the beginning of the read up by one half of the
				   amount of extra space in the buffer; but never past the
				   beginning of record number 1. */
				lngTmp = lngTmp / 2;
				__This->bufferBeginPosition -= __This->recordSize * lngTmp;
				if (__This->bufferBeginPosition < sizeof (struct csGeoid99Hdr_))	/*lint !e574 */
				{
					__This->bufferBeginPosition = sizeof (struct csGeoid99Hdr_);
				}
				readCount = __This->bufferEndPosition - __This->bufferBeginPosition;
			}

			lngTmp = (__This->bufferSize - readCount) / __This->recordSize;
			if (lngTmp > 3L)
			{
				/* Move the end of the read back by the amount of extra
				   space in the buffer, but never past the end of the file. */
				__This->bufferEndPosition += __This->recordSize * lngTmp;
				if (__This->bufferEndPosition > __This->fileSize)
				{
					__This->bufferEndPosition = __This->fileSize;
				}
				readCount = __This->bufferEndPosition - __This->bufferBeginPosition;
			}

			lngTmp = (__This->bufferSize - readCount) / __This->recordSize;
			if (lngTmp > 0L)
			{
				/* In case the expanded end of read exceeded the end of the
				   file, we can move the beginning of the read up some more,
				   However, never more than the beginning of the first
				   data record. */
				__This->bufferBeginPosition -= __This->recordSize * lngTmp;
				if (__This->bufferBeginPosition < sizeof (struct csGeoid99Hdr_))	/*lint !e574 */
				{
					__This->bufferBeginPosition = sizeof (struct csGeoid99Hdr_);
				}
				readCount = __This->bufferEndPosition - __This->bufferBeginPosition;
			}

			/* Defensive programming. */
			if (readCount != __This->bufferSize)
			{
				CS_stncp (csErrnam,"CS_geoid99:2",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
		}

		/* OK, read in the data. */
		checkSeek = CS_fseek (__This->strm,__This->bufferBeginPosition,SEEK_SET);
		if (checkSeek < 0L)
		{
			CS_stncp (csErrnam,__This->filePath,MAXPATH);
			CS_erpt (cs_IOERR);
			goto error;
		}
		checkCount = (long)CS_fread (__This->dataBuffer,1,(size_t)readCount,__This->strm);
		if (checkCount != readCount)
		{
			CS_stncp (csErrnam,__This->filePath,MAXPATH);
			CS_erpt (cs_INV_FILE);
			goto error;
		}
		if (CS_ferror (__This->strm))
		{
			CS_stncp (csErrnam,__This->filePath,MAXPATH);
			CS_erpt (cs_IOERR);
			goto error;
		}

		/* Under certain circumstances, it will be rather inefficient to swap bytes here.
		   However, we swap here to preclude the chance of swapping the data twice, and
		   the chance that unswapped floats appear to be Nan's or something ugly like
		   that.  So, this may be somewhat inefficient, but it is the safest place
		   to put the swap.  In general, Geoid99 files are in little endian order, and
		   Geoid 2003 files are in Big endian order.  Thus, by using the appropriate
		   files, you may be able to skip the whole issue of byte swapping here
		   altogether. */

		if (__This->iKind != 1L)
		{
			/* Here if some swapping has to be done.  Need to exercise some care.
			   If the whole file was read in, we have a header on the front which
			   we don't want to swap.  Otherwise, we can treat the buffer as an
			   array of floats, which is rather simple.
			   
			   In the code below, we use lngTmp as the number of bytes on the
			   front of the buffer which we must omit from the swapping process. */

			lngTmp = (long)sizeof (struct csGeoid99Hdr_) - __This->bufferBeginPosition;
			if (lngTmp < 0) lngTmp = 0L;

			swapCount = (__This->bufferEndPosition - (__This->bufferBeginPosition + lngTmp)) / (long32_t)sizeof (float);
			sprintf (swapSpec,"%ldf",swapCount);
			chrPtr = (char *)(__This->dataBuffer) + lngTmp;
			CSbswap (chrPtr,swapSpec);
		}

		/* We have the desired data in the buffer.  If we read in the whole file,
		   we close the stream now.  No need to have the file descriptor open. */
		if (__This->bufferSize == __This->fileSize)
		{
			CS_fclose (__This->strm);
			__This->strm = NULL;
		}
	}

	/* Compute the delta into the grid cell we will contruct below.  Note, we are using adjusted
	   recNbr and eleNbr variables here. */
	centerLL [LNG] = __This->coverage.southWest [LNG] + __This->deltaLng * (double)eleNbr;
	centerLL [LAT] = __This->coverage.southWest [LAT] + __This->deltaLat * (double)recNbr;
	deltaLL [LNG] = ((sourceLL [LNG] - centerLL [LNG]) / __This->deltaLng) + cs_One;
	deltaLL [LAT] = ((sourceLL [LAT] - centerLL [LAT]) / __This->deltaLat) + cs_One;

	/* OK, the necessary stuff should be in the buffer.  We do what is necessary to
	   populate the array.  Notice, we populate the array in a way that the edge
	   effects come out correct using a standard algorithm below. */
	switch (edge) {
	case edgeNone:
		fpos = sizeof (struct csGeoid99Hdr_) + (recNbr * __This->recordSize) + (eleNbr * sizeof (float));

		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		chrPtr += __This->recordSize;
		fltPtr = (float *)(chrPtr);
		array [6] = *(fltPtr - 1);
		array [7] = *fltPtr;
		array [8] = *(fltPtr + 1);

		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);
		array [3] = *(fltPtr - 1);
		array [4] = *fltPtr;
		array [5] = *(fltPtr + 1);

		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		chrPtr -= __This->recordSize;
		fltPtr = (float *)(chrPtr);
		array [0] = *(fltPtr - 1);
		array [1] = *fltPtr;
		array [2] = *(fltPtr + 1);
		break;

	case edgeSouthwest:
		fpos = sizeof (struct csGeoid99Hdr_);
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);

		array [6] = array [7] = array [8] = *fltPtr;
		array [3] = array [4] = array [5] = *fltPtr;
		array [0] = array [1] = array [2] = *fltPtr;

		deltaLL [LNG] = deltaLL [LAT] = cs_One;
		break;

	case edgeSouth:
		fpos = sizeof (struct csGeoid99Hdr_) + eleNbr * sizeof (float);
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);

		array [6] = array [3] = array [0] = *(fltPtr - 1);
		array [7] = array [4] = array [1] = *fltPtr;
		array [8] = array [5] = array [2] = *(fltPtr + 1);

		deltaLL [LAT] = cs_One;
		break;

	case edgeSoutheast:
		fpos = sizeof (struct csGeoid99Hdr_) + (__This->elementCount - 1) * sizeof (float);
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);

		array [6] = array [7] = array [8] = *fltPtr;
		array [3] = array [4] = array [5] = *fltPtr;
		array [0] = array [1] = array [2] = *fltPtr;

		deltaLL [LNG] = deltaLL [LAT] = cs_One;
		break;

	case edgeEast:
		fpos = sizeof (struct csGeoid99Hdr_) + recNbr * __This->recordSize + (__This->elementCount - 1) * sizeof (float);
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		chrPtr += __This->recordSize;
		fltPtr = (float *)(chrPtr);

		array [6] = array [7] = array [8] = *fltPtr;

		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);
		array [3] = array [4] = array [5] = *fltPtr;

		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		chrPtr -= __This->recordSize;
		fltPtr = (float *)(chrPtr);
		array [0] = array [1] = array [2] = *fltPtr;

		deltaLL [LNG] = cs_One;
		break;

	case edgeNortheast:
		fpos = sizeof (struct csGeoid99Hdr_) + (__This->recordCount - 1) * __This->recordSize + (__This->elementCount - 1) * sizeof (float);
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);

		array [6] = array [7] = array [8] = *fltPtr;
		array [3] = array [4] = array [5] = *fltPtr;
		array [0] = array [1] = array [2] = *fltPtr;

		deltaLL [LNG] = deltaLL [LAT] = cs_One;
		break;

	case edgeNorth:
		fpos = sizeof (struct csGeoid99Hdr_) + (__This->recordCount - 1) * __This->recordSize + eleNbr * sizeof (float);
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);

		array [6] = array [3] = array [0] = *(fltPtr - 1);
		array [7] = array [4] = array [1] = *fltPtr;
		array [8] = array [5] = array [2] = *(fltPtr + 1);

		deltaLL [LAT] = cs_One;
		break;

	case edgeNorthwest:
		fpos = sizeof (struct csGeoid99Hdr_) + (__This->recordCount - 1) * __This->recordSize;
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);

		array [6] = array [7] = array [8] = *fltPtr;
		array [3] = array [4] = array [5] = *fltPtr;
		array [0] = array [1] = array [2] = *fltPtr;

		deltaLL [LNG] = deltaLL [LAT] = cs_One;
		break;

	case edgeWest:
		fpos = sizeof (struct csGeoid99Hdr_) + recNbr * __This->recordSize;
		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		chrPtr += __This->recordSize;
		fltPtr = (float *)(chrPtr);
		array [6] = array [7] = array [8] = *fltPtr;

		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		fltPtr = (float *)(chrPtr);
		array [3] = array [4] = array [5] = *fltPtr;

		chrPtr = (char *)(__This->dataBuffer) + (fpos - __This->bufferBeginPosition);
		chrPtr -= __This->recordSize;
		fltPtr = (float *)(chrPtr);
		array [0] = array [1] = array [2] = *fltPtr;

		deltaLL [LNG] = cs_One;
		break;
	}

	/* Do the calculations. */
	*result = CSgeoidQterp (deltaLL,array);
	return 0;

error:
	/* Release the resources allocated to this object.  The next call to this
	   object would then require a refresh. */
	CSreleaseGeoid99GridFile (__This);

	/* Negative return indicates a system error of sorts. */
	return -1;
}

double EXP_LVL9 CSgeoidQterp (double sourceDelta [2],float array [9])
{
	double result;
	double f0, f1, f2;

	f0 = CSgeoidQterp1 (sourceDelta [0],(double)array [0],(double)array [1],(double)array [2]);
	f1 = CSgeoidQterp1 (sourceDelta [0],(double)array [3],(double)array [4],(double)array [5]);
	f2 = CSgeoidQterp1 (sourceDelta [0],(double)array [6],(double)array [7],(double)array [8]);
	result = CSgeoidQterp1 (sourceDelta [1],f0,f1,f2);
	return result;
}
double EXP_LVL9 CSgeoidQterp1 (double delta,double f0,double f1,double f2)
{
	extern double cs_One;
	extern double cs_Half;

	double tmp;
	double df0;			/* first delta */
	double df1;			/* second delta */
	double d2f0;		/* second order delta */
	double result;

	df0  = f1 - f0;
	df1  = f2 - f1;
	d2f0 = df1  - df0;
	tmp = cs_Half * delta * (delta - cs_One);
	result = f0 + delta * df0 + tmp * d2f0;
	return result;
}
/*lint +e826 */
