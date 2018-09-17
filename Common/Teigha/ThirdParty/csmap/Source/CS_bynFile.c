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

/*lint -esym(715,flags) */			/* parameter not referenced */
/*lint -e826 */						/* we use char pointers to do calculations,
									   then cast the result to a short to access
									   the data. */

/*??*/ /*Probably should rewrite the calculations to do the calculations using
		 a short pointer.  HOwever, this code has worked for a long time, so
		 its probably best not to mess with it until there is another reason
		 to do so. */

#include "cs_map.h"

/*
	This file contains the implementation of the BynFile object.
	BynFile's are very similar to Geoid99 type files and are
	also used for data sets other than geoid heights.

	Anywho, the implementation, use, and purpose are different
	enough to warrant a separate set of implementation code.
*/

extern char csErrnam [MAXPATH];

short CSswapShort (short source,int swapEm);
long32_t  CSswapLong  (long32_t source,int swapEm);
double EXP_LVL9 CSbynGridFileQterp (double sourceDelta [2],double array [9]);
double EXP_LVL9 CSbynGridFileQterp1 (double delta,double f0,double f1,double f2);

/*****************************************************************************
	'Private' support function
*/
void CSinitBynGridFile (struct csBynGridFile_ *thisPtr)
{
	CSinitCoverage (&thisPtr->coverage);
	thisPtr->elementCount = 0;
	thisPtr->recordCount = 0;
	thisPtr->recordSize = 0;
	thisPtr->deltaLng = 0.0;
	thisPtr->deltaLat = 0.0;
	thisPtr->dataFactor = 1.0;
	thisPtr->fileSize = 0L;
	thisPtr->strm = NULL;
	thisPtr->bufferSize = 0L;
	thisPtr->bufferBeginPosition = 0;
	thisPtr->bufferEndPosition = 0;
	thisPtr->dataBuffer = NULL;
	thisPtr->fileType = 0;
	thisPtr->elementSize = 0;
	thisPtr->byteOrder = 1;
	thisPtr->swapFlag = 0;
	thisPtr->filePath [0] = '\0';
	thisPtr->fileName [0] = '\0';
}

/*****************************************************************************
	Constructor
*/
struct csBynGridFile_* CSnewBynGridFile (Const char *path,long32_t bufferSize,ulong32_t flags,double density)
{
	extern double cs_Sec2Deg;
	extern double cs_One;

	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];

	size_t readCount;
	long32_t lngTmp;
	char *cp1, *cp2;
	csFILE *fstr;
	struct csBynGridFile_* thisPtr;
	char cTemp [MAXPATH];
	struct csBynGridFileHdr_ bynFileHdr;

	/* Prepare for an error. */
	thisPtr = NULL;
	fstr = NULL;

	/* Malloc and initialize */
	thisPtr = CS_malc (sizeof (struct csBynGridFile_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	CSinitBynGridFile (thisPtr);

	/* Set default values for all members. */
	thisPtr->bufferSize = bufferSize;
	if (thisPtr->bufferSize < 0) thisPtr->bufferSize = 0;

	/* Save file path/name. */
	CS_stncp (thisPtr->filePath,path,sizeof (thisPtr->filePath));
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
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}
	cp1 += 1;
	cp2 = strchr (cp1,cs_ExtsepC);
	if (cp2 == NULL)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}
	*cp2++ = '\0';
	CS_stncp (thisPtr->fileName,cp1,sizeof (thisPtr->fileName));

	/* The thing should have a .bin extension to be processed by us. */
	if (CS_stricmp (cp2,"byn"))
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Get the file information header. */
	fstr = CS_fopen (thisPtr->filePath,_STRM_BINRD);
	if (fstr == NULL)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}
	readCount = CS_fread (&bynFileHdr,1,sizeof (bynFileHdr),fstr);
	if (readCount != sizeof (bynFileHdr))
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}
	if (CS_ferror (fstr))
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_IOERR);
		goto error;
	}

	/* Determine the size of the file. */
	if (CS_fseek (fstr,0L,SEEK_END))
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_IOERR);
		goto error;
	}
	thisPtr->fileSize = (long32_t)CS_ftell (fstr);
	if (thisPtr->fileSize < 0L)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_IOERR);
		goto error;
	}

	/* This constructor doesn't need the file any more. */
	CS_fclose (fstr);
	fstr = NULL;

	/* Swap the bytes if necessary.  Note that this implementation
	   assumes that the file is in little endian order. */
	CS_bswap (&bynFileHdr,cs_BSWP_BynFileHDR);

	/* Extract the important stuff from the header. */
	thisPtr->coverage.southWest [LNG] = (double)bynFileHdr.westBoundary  * cs_Sec2Deg;
	thisPtr->coverage.southWest [LAT] = (double)bynFileHdr.southBoundary * cs_Sec2Deg;
	thisPtr->coverage.northEast [LNG] = (double)bynFileHdr.eastBoundary * cs_Sec2Deg;
	thisPtr->coverage.northEast [LAT] = (double)bynFileHdr.northBoundary * cs_Sec2Deg;
	thisPtr->deltaLng = (double)bynFileHdr.ewSpacing * cs_Sec2Deg;
	thisPtr->deltaLat = (double)bynFileHdr.nsSpacing * cs_Sec2Deg;
	thisPtr->coverage.density = (thisPtr->deltaLng < thisPtr->deltaLat) ? thisPtr->deltaLng : thisPtr->deltaLat;
	if (density != 0.0) thisPtr->coverage.density = density;
	thisPtr->elementCount = ((bynFileHdr.eastBoundary - bynFileHdr.westBoundary) / bynFileHdr.ewSpacing) + 1L;
	thisPtr->recordCount = ((bynFileHdr.northBoundary - bynFileHdr.southBoundary) / bynFileHdr.nsSpacing) + 1L;
	thisPtr->recordSize = thisPtr->elementCount * bynFileHdr.sizeOfData;

	/* Verify the integrity of the file. */
	lngTmp = thisPtr->recordCount * thisPtr->recordSize + sizeof (bynFileHdr);
	if (lngTmp != thisPtr->fileSize)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Now that we know recordSize, we can adjust the bufferSize for maximum
	   efficiency. */
	if (thisPtr->bufferSize > thisPtr->fileSize)
	{
		thisPtr->bufferSize = thisPtr->fileSize;
	}
	else
	{
		if (thisPtr->bufferSize > (3 * thisPtr->recordSize))
		{
			/* Maximum efficiency is obtained with a buffer size whch is
			   a multiple of the record size. */
			thisPtr->bufferSize = (thisPtr->bufferSize / thisPtr->recordSize) * thisPtr->recordSize;
		}
		else
		{
			/* We require a minimum buffer size of 3 records. */
			thisPtr->bufferSize = 3 * thisPtr->recordSize;
		}
	}

	/* Finish up with some simple stuff.  We inverse factor as
	   multiples are faster than divides, and we will probably
	   be doing a lot of them. */
	thisPtr->dataFactor = cs_One / bynFileHdr.factor;
	thisPtr->fileType = bynFileHdr.dataType;
	thisPtr->elementSize = bynFileHdr.sizeOfData;
	thisPtr->byteOrder = bynFileHdr.byteOrder;
	thisPtr->swapFlag = (short)((thisPtr->byteOrder == 0) ^ CS_isBigEndian ());

	return (thisPtr);

error:
	CSdeleteBynGridFile (thisPtr);
	return NULL;
}
/*****************************************************************************
	Destructor
*/
void CSdeleteBynGridFile (struct csBynGridFile_* thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->strm != NULL) CS_fclose (thisPtr->strm);
		if (thisPtr->dataBuffer != NULL)	CS_free (thisPtr->dataBuffer);
		CS_free (thisPtr);
	}
}
/*****************************************************************************
	Release resources
*/
void CSreleaseBynGridFile (struct csBynGridFile_* thisPtr)
{
	if (thisPtr != NULL)
	{
		/* Close the file and release the buffer.  Leave the file size,
		   buffer size, coverage information, and other stuff intact.
		   The only purpose here is to release allocated resources. */

		/* Free and invalidate the buffer, if there is one. */
		if (thisPtr->dataBuffer != NULL)
		{
			CS_free (thisPtr->dataBuffer);
		}
		thisPtr->dataBuffer = NULL;
		thisPtr->bufferBeginPosition = -1L;
		thisPtr->bufferEndPosition = -2L;

		/* Close the file (if open). */
		if (thisPtr->strm != NULL)
		{
			CS_fclose (thisPtr->strm);
		}
		thisPtr->strm = NULL;
	}
	return;
}
/*****************************************************************************
	Coverage Test
*/
double CStestBynGridFile (struct csBynGridFile_* thisPtr,Const double *sourceLL)
{
	extern double cs_Zero;

	int st;
	double geoidHgt;
	double density;

	density = CStestCoverage (&(thisPtr->coverage),sourceLL);
	if (density > 0.0)
	{
		/* Need to do this extra check, which can be painful, as there are
		   regions in the .byn file which carry values which indicate
		   no coverage.  Note, if we get a minus 1 back from the calc
		   function, we still return the current density.  This will
		   tend to force a calc function call be the user, and the proper
		   error reporting performed at that time. */
		st = CScalcBynGridFile (thisPtr,&geoidHgt,sourceLL);
		density = (st > 0) ? cs_Zero : thisPtr->coverage.density;
	}
	return density;
}

/*****************************************************************************
	This function would be a private function in C++.
	This function sets the currentCell member to the cell which covers the
	provided geographic coordinate.  Returns 0 on success or a negative value
	for a system error of some sort (i.e. file I/O).  Per the design of this
	object, this function is not to be called unless the provided source
	coordinate is within the coverage of the object.
*/
int CScalcBynGridFile (struct csBynGridFile_* thisPtr,double* result,Const double* sourceLL)
{
	extern double cs_One;

	int idx;
	int rtnVal;
	int eleNbr;
	int recNbr;
	int readCount;
	int checkCount;

	long32_t lngTmp;
	long32_t checkSeek;
	long32_t fpos;
	long32_t fposBegin;
	long32_t fposEnd;

	char* chrPtr;
	short *shrtPtr;
	long32_t *lngPtr;

	double centerLL [2];
	double deltaLL [2];

	/* The arrays carry the 3x3 array as follows:
		         North

		W       6  7  8      E
		e       3  4  5      a
		s       0  1  2      s
		t                    t 
			     South          */

	short shrtArray [9];
	long32_t lngArray [9];
	double dblArray [9];

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
	   CStestBynGridFile function to select the proper csBynGridFile object. */
	if (sourceLL [LNG] < thisPtr->coverage.southWest [LNG] || sourceLL [LNG] > thisPtr->coverage.northEast [LNG] ||
	    sourceLL [LAT] < thisPtr->coverage.southWest [LAT] || sourceLL [LAT] > thisPtr->coverage.northEast [LAT])
	{
		CS_stncp (csErrnam,"CSbynGridFile:1",MAXPATH);
		CS_erpt  (cs_ISER);
		goto error;
	}
	   
	/* Compute the basic indices to the cell in the data file.  We're supposed to come up
	   with a reference to the closest point.  Since we know the source coordinate is within
	   range, dealing with the edges is rather easy.  Note, this file goes from north to south,
	   unlike most all other files of this type.  Thus, a record number of 1 means the northern
	   edge of the data. */
	eleNbr = (long32_t)(((sourceLL [LNG] - thisPtr->coverage.southWest [LNG]) / thisPtr->deltaLng) + 0.5);
	recNbr = (long32_t)(((thisPtr->coverage.northEast [LAT] - sourceLL [LAT]) / thisPtr->deltaLat) + 0.5);

	/* Determine the if an edge effect applies. */
	if (recNbr < 1)
	{
		recNbr = 1;
		if (eleNbr < 1)
		{
			eleNbr = 1;
			edge = edgeNorthwest;
		}
		else if (eleNbr >= (thisPtr->elementCount - 1))
		{
			eleNbr = thisPtr->elementCount - 2;
			edge = edgeNortheast;
		}
		else
		{
			edge = edgeNorth;
		}
	}
	else if (recNbr >= (thisPtr->recordCount - 1))
	{
		if (eleNbr < 1)
		{
			eleNbr = 1;
			edge = edgeSouthwest;
		}
		else if (eleNbr >= (thisPtr->elementCount - 1))
		{
			eleNbr = thisPtr->elementCount - 2;
			edge = edgeSoutheast;
		}
		else
		{
			edge = edgeSouth;
		}
	}
	else
	{
		if (eleNbr < 1)
		{
			eleNbr = 1;
			edge = edgeWest;
		}
		else if (eleNbr > (thisPtr->elementCount -1))
		{
			eleNbr = thisPtr->elementCount - 2;
			edge = edgeEast;
		}
		else
		{
			edge = edgeNone;
		}
	}

	/* Compute the minimal region of the file which we need to read. */
	fposBegin = sizeof (struct csBynGridFileHdr_) + (recNbr - 1) * thisPtr->recordSize;
	fposEnd = fposBegin + (thisPtr->recordSize * 3);
	if (fposEnd > thisPtr->fileSize) fposEnd = thisPtr->fileSize;

	/* Do we have a buffer?  Could have been released.  Maybe this is the
	   first access. */
	if (thisPtr->dataBuffer == NULL)
	{
		thisPtr->dataBuffer = CS_malc ((size_t)thisPtr->bufferSize);
		if (thisPtr->dataBuffer == NULL)
		{
			CS_erpt (cs_NO_MEM);
			goto error;
		}

		/* Make sure the rest of this stuff knows the buffer is empty.  These values
		   will fail to match any specific file position. */
		thisPtr->bufferBeginPosition = -1L;
		thisPtr->bufferEndPosition = -2L;
	}

	/* See if the stuff we want is in the buffer.  Careful here, all of the intended
	   range must be in the buffer, not just a portion of it. */
	if (fposBegin < thisPtr->bufferBeginPosition || fposBegin > thisPtr->bufferEndPosition ||
		fposEnd   < thisPtr->bufferBeginPosition || fposEnd   > thisPtr->bufferEndPosition)
	{
		/* The data we need is not there; we need to read it in.  Is the file open? */
		if (thisPtr->strm == NULL)
		{
			thisPtr->strm = CS_fopen (thisPtr->filePath,_STRM_BINRD);
			if (thisPtr->strm == NULL)
			{
				CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
				CS_erpt (cs_DTC_FILE);
				goto error;
			}
			/* We do our own buffering, turn stream buffering off. */
			setvbuf (thisPtr->strm,NULL,_IONBF,0);
		}

		/* Compute the starting position of the actual read. */
		if (thisPtr->bufferSize >= thisPtr->fileSize)
		{
			thisPtr->bufferBeginPosition = 0L;
			thisPtr->bufferEndPosition = thisPtr->fileSize;
			readCount = thisPtr->fileSize;
		}
		else
		{
			/* We need to do a partial read, the normal case.  Initialize for the
			   minimal case computed above, then expand as is possible.  Note, we
			   get here only when a read has to be done. */
			thisPtr->bufferBeginPosition = fposBegin;
			thisPtr->bufferEndPosition = fposEnd;
			readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;

			/* In this section, lngTmp is the number of additional records
			   which can fit in the buffer. */
			lngTmp = (thisPtr->bufferSize - readCount) / thisPtr->recordSize;
			if (lngTmp > 3L)
			{
				/* Move the beginning of the read up by one half of the
				   amount of extra space in the buffer; but never past the
				   beginning of record number 1. */
				lngTmp = lngTmp / 2;
				thisPtr->bufferBeginPosition -= thisPtr->recordSize * lngTmp;
				if (thisPtr->bufferBeginPosition < sizeof (struct csBynGridFileHdr_))	/*lint !e574 */
				{
					thisPtr->bufferBeginPosition = sizeof (struct csBynGridFileHdr_);
				}
				readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;
			}

			lngTmp = (thisPtr->bufferSize - readCount) / thisPtr->recordSize;
			if (lngTmp > 3L)
			{
				/* Move the end of the read back by the amount of extra
				   space in the buffer, but never past the end of the file. */
				thisPtr->bufferEndPosition += thisPtr->recordSize * lngTmp;
				if (thisPtr->bufferEndPosition > thisPtr->fileSize)
				{
					thisPtr->bufferEndPosition = thisPtr->fileSize;
				}
				readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;
			}

			lngTmp = (thisPtr->bufferSize - readCount) / thisPtr->recordSize;
			if (lngTmp > 0L)
			{
				/* In case the expanded end of read exceeded the end of the
				   file, we can move the beginning of the read up some more,
				   However, never more than the beginning of the first
				   data record. */
				thisPtr->bufferBeginPosition -= thisPtr->recordSize * lngTmp;
				if (thisPtr->bufferBeginPosition < sizeof (struct csBynGridFileHdr_))	/*lint !e574 */
				{
					thisPtr->bufferBeginPosition = sizeof (struct csBynGridFileHdr_);
				}
				readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;
			}

			/* Defensive programming. */
			if (readCount != thisPtr->bufferSize)
			{
				CS_stncp (csErrnam,"CSbynGridFIle:2",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
		}

		/* OK, read in the data. */
		checkSeek = (long32_t)CS_fseek (thisPtr->strm,(long)thisPtr->bufferBeginPosition,SEEK_SET);
		if (checkSeek < 0L)
		{
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_IOERR);
			goto error;
		}
		checkCount = (long32_t)CS_fread (thisPtr->dataBuffer,1,(size_t)readCount,thisPtr->strm);
		if (checkCount != readCount)
		{
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_INV_FILE);
			goto error;
		}
		if (CS_ferror (thisPtr->strm))
		{
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_IOERR);
			goto error;
		}
		/* Note, we swap the individual cells as we need them.  Perhaps we should
		   swap the whole buffer now, but we don't.  We do each data item as
		   it is required. */

		/* We have the desired data in the buffer.  If we read in the whole file,
		   we close the stream now.  No need to have the file descriptor open. */
		if (thisPtr->bufferSize == thisPtr->fileSize)
		{
			CS_fclose (thisPtr->strm);
			thisPtr->strm = NULL;
		}
	}

	/* Compute the delta into the grid cell we will contruct below.  Note, we are using adjusted
	   recNbr and eleNbr variables here.  Center is the southwest corner of the cell in the
	   center of the minigrid we will deal with. */
	centerLL [LNG] = thisPtr->coverage.southWest [LNG] + thisPtr->deltaLng * (double)eleNbr;
	centerLL [LAT] = thisPtr->coverage.northEast [LAT] - thisPtr->deltaLat * (double)recNbr;
	deltaLL [LNG] = ((sourceLL [LNG] - centerLL [LNG]) / thisPtr->deltaLng) + cs_One;
	deltaLL [LAT] = ((sourceLL [LAT] - centerLL [LAT]) / thisPtr->deltaLat) + cs_One;

	/* OK, the necessary stuff should be in the buffer.  We do what is necessary to
	   populate the array.  Notice, we populate the array in a way that the edge
	   effects come out correct using a standard algorithm below.  Actually, I
	   think all this edge stuff is superfluous, as the cells onthe edges are
	   probaly marked as "999" anyway.  (9999 means no data available.) */
	if (thisPtr->elementSize == 2)
	{
		switch (edge) {
		case edgeNone:
			fpos = sizeof (struct csBynGridFileHdr_) + (recNbr * thisPtr->recordSize) + (eleNbr * thisPtr->elementSize);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr -= thisPtr->recordSize;
			shrtPtr = (short *)(chrPtr);
			shrtArray [6] = CSswapShort (*(shrtPtr - 1),thisPtr->swapFlag);
			shrtArray [7] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [8] = CSswapShort (*(shrtPtr + 1),thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);
			shrtArray [3] = CSswapShort (*(shrtPtr - 1),thisPtr->swapFlag);
			shrtArray [4] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [5] = CSswapShort (*(shrtPtr + 1),thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr += thisPtr->recordSize;
			shrtPtr = (short *)(chrPtr);
			shrtArray [0] = CSswapShort (*(shrtPtr - 1),thisPtr->swapFlag);
			shrtArray [1] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [2] = CSswapShort (*(shrtPtr + 1),thisPtr->swapFlag);
			break;

		case edgeSouthwest:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->recordCount - 1) * thisPtr->recordSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);

			shrtArray [6] = shrtArray [7] = shrtArray [8] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [3] = shrtArray [4] = shrtArray [5] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [0] = shrtArray [1] = shrtArray [2] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeSouth:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->recordCount - 1) * thisPtr->recordSize + eleNbr * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);

			shrtArray [6] = shrtArray [3] = shrtArray [0] = CSswapShort (*(shrtPtr - 1),thisPtr->swapFlag);
			shrtArray [7] = shrtArray [4] = shrtArray [1] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [8] = shrtArray [5] = shrtArray [2] = CSswapShort (*(shrtPtr + 1),thisPtr->swapFlag);

			deltaLL [LAT] = cs_One;
			break;

		case edgeSoutheast:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->recordCount - 1) * thisPtr->recordSize + (thisPtr->elementCount - 1) * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);

			shrtArray [6] = shrtArray [7] = shrtArray [8] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [3] = shrtArray [4] = shrtArray [5] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [0] = shrtArray [1] = shrtArray [2] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeEast:
			fpos = sizeof (struct csBynGridFileHdr_) + (recNbr * thisPtr->recordSize) + (thisPtr->elementCount - 1) * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr -= thisPtr->recordSize;
			shrtPtr = (short *)(chrPtr);

			shrtArray [6] = shrtArray [7] = shrtArray [8] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);
			shrtArray [3] = shrtArray [4] = shrtArray [5] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr += thisPtr->recordSize;
			shrtPtr = (short *)(chrPtr);
			shrtArray [0] = shrtArray [1] = shrtArray [2] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			deltaLL [LNG] = cs_One;
			break;

		case edgeNortheast:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->elementCount - 1) * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);

			shrtArray [6] = shrtArray [7] = shrtArray [8] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [3] = shrtArray [4] = shrtArray [5] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [0] = shrtArray [1] = shrtArray [2] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeNorth:
			fpos = sizeof (struct csBynGridFileHdr_) + eleNbr * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);

			shrtArray [6] = shrtArray [3] = shrtArray [0] = CSswapShort (*(shrtPtr - 1),thisPtr->swapFlag);
			shrtArray [7] = shrtArray [4] = shrtArray [1] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [8] = shrtArray [5] = shrtArray [2] = CSswapShort (*(shrtPtr + 1),thisPtr->swapFlag);

			deltaLL [LAT] = cs_One;
			break;

		case edgeNorthwest:
			fpos = sizeof (struct csBynGridFileHdr_);
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);

			shrtArray [6] = shrtArray [7] = shrtArray [8] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [3] = shrtArray [4] = shrtArray [5] = CSswapShort (*shrtPtr,thisPtr->swapFlag);
			shrtArray [0] = shrtArray [1] = shrtArray [2] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeWest:
			fpos = sizeof (struct csBynGridFileHdr_) + recNbr * thisPtr->recordSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr -= thisPtr->recordSize;
			shrtPtr = (short *)(chrPtr);
			shrtArray [6] = shrtArray [7] = shrtArray [8] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			shrtPtr = (short *)(chrPtr);
			shrtArray [3] = shrtArray [4] = shrtArray [5] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr += thisPtr->recordSize;
			shrtPtr = (short *)(chrPtr);
			shrtArray [0] = shrtArray [1] = shrtArray [2] = CSswapShort (*shrtPtr,thisPtr->swapFlag);

			deltaLL [LNG] = cs_One;
			break;
		}
		
		/* Transfer to dblArray. */
		for (idx = 0;idx < 9;idx += 1)
		{
			if (shrtArray [idx] == 32767) dblArray [idx] = 9999.0;		/*lint !e644 */
			else
			{
				dblArray [idx] = shrtArray [idx] * thisPtr->dataFactor;
			}
		}
	}
	else if (thisPtr->elementSize == 4)
	{
		switch (edge) {
		case edgeNone:
			fpos = sizeof (struct csBynGridFileHdr_) + (recNbr * thisPtr->recordSize) + (eleNbr * thisPtr->elementSize);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr -= thisPtr->recordSize;
			lngPtr = (long32_t *)(chrPtr);
			lngArray [6] = CSswapLong (*(lngPtr - 1),thisPtr->swapFlag);
			lngArray [7] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [8] = CSswapLong (*(lngPtr + 1),thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);
			lngArray [3] = CSswapLong (*(lngPtr - 1),thisPtr->swapFlag);
			lngArray [4] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [5] = CSswapLong (*(lngPtr + 1),thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr += thisPtr->recordSize;
			lngPtr = (long32_t *)(chrPtr);
			lngArray [0] = CSswapLong (*(lngPtr - 1),thisPtr->swapFlag);
			lngArray [1] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [2] = CSswapLong (*(lngPtr + 1),thisPtr->swapFlag);
			break;

		case edgeSouthwest:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->recordCount - 1) * thisPtr->recordSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);

			lngArray [6] = lngArray [7] = lngArray [8] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [3] = lngArray [4] = lngArray [5] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [0] = lngArray [1] = lngArray [2] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeSouth:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->recordCount - 1) * thisPtr->recordSize + eleNbr * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);

			lngArray [6] = lngArray [3] = lngArray [0] = CSswapLong (*(lngPtr - 1),thisPtr->swapFlag);
			lngArray [7] = lngArray [4] = lngArray [1] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [8] = lngArray [5] = lngArray [2] = CSswapLong (*(lngPtr + 1),thisPtr->swapFlag);

			deltaLL [LAT] = cs_One;
			break;

		case edgeSoutheast:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->recordCount - 1) * thisPtr->recordSize + (thisPtr->elementCount - 1) * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);

			lngArray [6] = lngArray [7] = lngArray [8] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [3] = lngArray [4] = lngArray [5] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [0] = lngArray [1] = lngArray [2] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeEast:
			fpos = sizeof (struct csBynGridFileHdr_) + recNbr * thisPtr->recordSize + (thisPtr->elementCount - 1) * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr -= thisPtr->recordSize;
			lngPtr = (long32_t *)(chrPtr);

			lngArray [6] = lngArray [7] = lngArray [8] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);
			lngArray [3] = lngArray [4] = lngArray [5] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr += thisPtr->recordSize;
			lngPtr = (long32_t *)(chrPtr);
			lngArray [0] = lngArray [1] = lngArray [2] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			deltaLL [LNG] = cs_One;
			break;

		case edgeNortheast:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->elementCount - 1) * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);

			lngArray [6] = lngArray [7] = lngArray [8] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [3] = lngArray [4] = lngArray [5] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [0] = lngArray [1] = lngArray [2] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeNorth:
			fpos = sizeof (struct csBynGridFileHdr_) + (thisPtr->elementCount - 1) * thisPtr->elementSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);

			lngArray [6] = lngArray [3] = lngArray [0] = CSswapLong (*(lngPtr - 1),thisPtr->swapFlag);
			lngArray [7] = lngArray [4] = lngArray [1] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [8] = lngArray [5] = lngArray [2] = CSswapLong (*(lngPtr + 1),thisPtr->swapFlag);

			deltaLL [LAT] = cs_One;
			break;

		case edgeNorthwest:
			fpos = sizeof (struct csBynGridFileHdr_);
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);

			lngArray [6] = lngArray [7] = lngArray [8] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [3] = lngArray [4] = lngArray [5] = CSswapLong (*lngPtr,thisPtr->swapFlag);
			lngArray [0] = lngArray [1] = lngArray [2] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			deltaLL [LNG] = deltaLL [LAT] = cs_One;
			break;

		case edgeWest:
			fpos = sizeof (struct csBynGridFileHdr_) + recNbr * thisPtr->recordSize;
			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr -= thisPtr->recordSize;
			lngPtr = (long32_t *)(chrPtr);
			lngArray [6] = lngArray [7] = lngArray [8] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			lngPtr = (long32_t *)(chrPtr);
			lngArray [3] = lngArray [4] = lngArray [5] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
			chrPtr += thisPtr->recordSize;
			lngPtr = (long32_t *)(chrPtr);
			lngArray [0] = lngArray [1] = lngArray [2] = CSswapLong (*lngPtr,thisPtr->swapFlag);

			deltaLL [LNG] = cs_One;
			break;
		}

		/* Transfer to dblArray. */
		for (idx = 0;idx < 9;idx += 1)
		{
			dblArray [idx] = lngArray [idx] * thisPtr->dataFactor;	/*lint !e644 */	
		}
	}
	else
	{
		CS_stncp (csErrnam,"CSbynGridFile:3",MAXPATH);
		CS_erpt (cs_ISER);
		goto error;
	}

	/* Do the calculations. */
	rtnVal = 0;
	for (idx = 0;idx < 9;idx += 1)
	{
		if (dblArray [idx] >= 9999.0) rtnVal = 1;
	}
	if (rtnVal == 0)
	{
		*result = CSbynGridFileQterp (deltaLL,dblArray);
	}
	else
	{
		*result = 9999.0;
	}
	return rtnVal;

error:
	/* Release the resources allocated to this object.  The next call to this
	   object would then require a refresh. */
	CSreleaseBynGridFile (thisPtr);

	/* Negative return indicates a system error of sorts. */
	return -1;
}
double EXP_LVL9 CSbynGridFileQterp (double sourceDelta [2],double array [9])
{
	double result;
	double f0, f1, f2;

	f0 = CSgeoidQterp1 (sourceDelta [0],array [0],array [1],array [2]);
	f1 = CSgeoidQterp1 (sourceDelta [0],array [3],array [4],array [5]);
	f2 = CSgeoidQterp1 (sourceDelta [0],array [6],array [7],array [8]);
	result = CSbynGridFileQterp1 (sourceDelta [1],f0,f1,f2);
	return result;
}
double EXP_LVL9 CSbynGridFileQterp1 (double delta,double f0,double f1,double f2)
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
short CSswapShort (short source,int swapEm)
{
	union
	{
		unsigned char bytes [2];
		short result;
	} value;
	cs_Register unsigned char *ptr;

	if (swapEm == 0) return source;

	ptr = (unsigned char *)(&source);
	value.bytes [1] = *ptr++;
	value.bytes [0] = *ptr;
	return value.result;
}
long32_t CSswapLong (long32_t source,int swapEm)
{
	union
	{
		unsigned char bytes [4];
		long32_t result;
	} value;
	cs_Register unsigned char *ptr;

	if (swapEm == 0) return source;

	ptr = (unsigned char *)(&source);
	value.bytes [3] = *ptr++;
	value.bytes [2] = *ptr++;
	value.bytes [1] = *ptr++;
	value.bytes [0] = *ptr;
	return value.result;
}
