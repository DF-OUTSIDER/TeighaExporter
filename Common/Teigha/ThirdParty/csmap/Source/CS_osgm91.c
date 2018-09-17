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
	This file contains the implementation of the Osgm91 object.

	Osgm91 is based on a text file with some 225,000 records, i.e.
	lines of text.  Each line contains the geoid separation as a
	number of millimeters.  The records are to be looked at as
	a grid of 450 (east/west) by 500 high (north/south), which
	have a delta of 2 kilometers.  The major complication here
	is that the cells are accessed using OSTN97 cartesian
	coordinates.

	Thus, to fit into the general scheme of Geoid separation,
	where geographic coordinates are used to produce a geoid
	separation value, the geographic coordinates first need to
	be converted to OSTN97 cartesian coordinates.  This process
	represents a majority of the code provided here.
*/

struct cs_Osgm91_ *CSnewOsgm91 (const char *filePath,long32_t bufferSize,ulong32_t flags,double density)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;

	extern double cs_Half;
	extern double cs_One;
	extern double cs_Two;			/* 2.0 */
	extern double cs_Degree;

	int st;

	char *cp;
	struct cs_Osgm91_ *__This = NULL;
	struct cs_Eldef_ *elPtr;

	/* Allocate and initialize the object. */
	__This = CS_malc (sizeof (struct cs_Osgm91_));
	if (__This == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	/* Populate the structure.  Most of this stuff is fixed, there
	   is no header on the fron of this file. */
	__This->coverage.southWest [LNG] = -6.1718700684;
	__This->coverage.southWest [LAT] = 49.8244421956;
	__This->coverage.northEast [LNG] =  8.2740772358;
	__This->coverage.northEast [LAT] = 58.4576498217;
	__This->coverage.density = (density > 0.0) ? density : 0.018018;
	__This->elementCount = 450;
	__This->recordCount = 500;
	__This->elementSize = sizeof (float);
	__This->recordSize = __This->elementCount * __This->elementSize;

	__This->strm = NULL;
	__This->dataBuffer = NULL;
	__This->bufferSize = 4 * __This->recordSize;
	if (bufferSize > __This->bufferSize) __This->bufferSize = bufferSize;

	__This->bufferBeginPosition = -1L;
	__This->bufferEndPosition = -2L;

	CS_stncp (__This->filePath,filePath,sizeof (__This->filePath));
	cp = strrchr (filePath,cs_DirsepC);							 /*lint !e605*/
	if (cp != NULL)
	{
		CS_stncp (__This->fileName,(cp + 1),sizeof (__This->fileName));
		cp = strrchr (__This->fileName,cs_ExtsepC);
		if (cp != NULL) *cp = '\0';
	}
	
	/* Create a binary file if one does not exist already. */
	st = CSmkBinaryOsgm91 (__This);
	if (st != 0) goto error;

	/* Initialize the Transverse Mercator portion.  We need this to
	   convert latitude and longitude to OSGB36 coodinates before
	   throwing them at the Grid file. */
	elPtr = CS_eldef ("GRS1980");
	__This->osgb36Trmer.cent_lng = -cs_Two * cs_Degree;
	__This->osgb36Trmer.org_lat = 49.0 * cs_Degree;
	__This->osgb36Trmer.k = 0.9996012717;
	__This->osgb36Trmer.k0 = 0.9996012717;
	__This->osgb36Trmer.x_off = 400000.0;
	__This->osgb36Trmer.y_off = -100000.0;
	__This->osgb36Trmer.ecent = elPtr->ecent;
	__This->osgb36Trmer.e_sq = elPtr->ecent * elPtr->ecent;
	__This->osgb36Trmer.e_rad = elPtr->e_rad;
	__This->osgb36Trmer.Rk = elPtr->e_rad * __This->osgb36Trmer.k;
	__This->osgb36Trmer.Rk_ovr_2 = __This->osgb36Trmer.Rk * cs_Half;
	__This->osgb36Trmer.var_K = cs_One;
	__This->osgb36Trmer.eprim_sq = __This->osgb36Trmer.e_sq / (cs_One - __This->osgb36Trmer.e_sq);
	__This->osgb36Trmer.xx_max = 1500000.0;
	CSmmFsu (&__This->osgb36Trmer.mmcofF,__This->osgb36Trmer.e_rad,__This->osgb36Trmer.e_sq);
	CSmmIsu (&__This->osgb36Trmer.mmcofI,elPtr->e_rad,__This->osgb36Trmer.e_sq);
	__This->osgb36Trmer.M0 = CSmmFcal (&__This->osgb36Trmer.mmcofF,__This->osgb36Trmer.org_lat,
																   sin (__This->osgb36Trmer.org_lat),
																   cos (__This->osgb36Trmer.org_lat));
	__This->osgb36Trmer.quad = 0;
	CS_free (elPtr);

	/* That's that. */
	return __This;
error:
	CSdeleteOsgm91 (__This);
	return NULL;
}																 /*lint !e715*/ /*flags*/
void CSdeleteOsgm91 (struct cs_Osgm91_ *__This)
{
	if (__This != NULL)
	{
		CSreleaseOsgm91 (__This);
		CS_free (__This);
	}
	return;
}
void CSreleaseOsgm91 (struct cs_Osgm91_ *__This)
{
	if (__This != NULL)
	{
		/* Close the file if opened. */
		if (__This->strm != NULL)
		{
			CS_fclose (__This->strm);
			__This->strm = NULL;
		}

		/* Delete the buffer is allocated. */
		if (__This->dataBuffer != NULL)
		{
			CS_free (__This->dataBuffer);
			__This->dataBuffer = NULL;
		}
		__This->bufferBeginPosition = -1L;
		__This->bufferEndPosition = -2L;
	}
	return;
}
double CStestOsgm91 (struct cs_Osgm91_ *__This,const double etrs89 [2])
{
	/* Returns zero if not covered, file density if covered. */
	return CStestCoverage (&(__This->coverage),etrs89);
}
/* Note, the following function is also called by the CSinverseOsgm91 function. */
int CScalcOsgm91 (struct cs_Osgm91_ *__This,double *geoidHgt,const double etrs89 [2])
{
	extern double cs_Mhuge;
	extern char csErrnam [];
	extern double cs_Zero;

	long32_t readCount;
	long32_t checkCount;

	long32_t lngTmp;
	long32_t checkSeek;
	long32_t recNbr, eleNbr;
	long32_t recFirst, recLast;
	long32_t fposBegin, fposEnd;

	char *chrPtr;
	float *fltPtr;

	double tt, uu;

	double xy [3];

	float southWest, southEast;
	float northWest, northEast;

	/* First, we compute the cartesian equivalent of the provided geographic
	   coordinates.  Essentially, we apply the OS British National Grid
	   Transformation, using the GRS1980 ellipsoid, to produce the necessary
	   cartesian coordinates. */
	CStrmerF (&__This->osgb36Trmer,xy,etrs89);				/*lint !e534  ignoring return value */

	/* Given the ETRS89 OSGB coordinates, return the translation values necessary to
	   produce official OSGB36 coordinates.  Returns zero on normal completion, +1 if
	   ETRS89 coordinates are out of range, -1 for any other error (i.e. call
	   CS_errmsg for a textual description of the error). */;

	/* Compute the indices of the grid cells involved. */
	recNbr = (long32_t)(xy [1] / 2000.0);
	eleNbr = (long32_t)(xy [0] / 2000.0) - 50;

	/* Return now if out of range. */
	if (recNbr < 0 || recNbr > __This->recordCount || eleNbr < 0 || __This->elementCount > 700)
	{
		*geoidHgt = cs_Zero;
		return 1;
	}

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

	/* Is the necessary data in the buffer already? */
	fposBegin = recNbr * __This->recordSize + eleNbr * __This->elementSize;
	fposEnd   = fposBegin + __This->recordSize + __This->elementSize;
	if (fposBegin < __This->bufferBeginPosition || fposBegin > __This->bufferEndPosition ||
		fposEnd   < __This->bufferBeginPosition || fposEnd   > __This->bufferEndPosition)
	{
		/* The data we need is not in the buffer.  If the file has been released,
		   open it again now. */
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

		/* Fill the buffer with data, including the specific data that we need.
		   When we go to the "well", we always fetch a minimum of two "records"
		   of water. */
		recFirst = recNbr;
		recLast = recFirst + 1;
		lngTmp = __This->bufferSize / __This->recordSize;
		if (lngTmp > 3)
		{
			recFirst = recNbr - (lngTmp / 2);
			recLast = recFirst + (lngTmp - 1);
			while (recFirst < 0)
			{
				recFirst += 1;
				recLast += 1;
			}
		}
		while (recLast > 500)
		{
			recFirst -= 1;
			recLast -= 1;
		}
		readCount = (recLast - recFirst + 1) * __This->recordSize;
		if (readCount != __This->bufferSize)
		{
			CS_stncp (csErrnam,"CS_osgm91:1",MAXPATH);
			CS_erpt (cs_ISER);
			goto error;
		}
		__This->bufferBeginPosition = recFirst * __This->recordSize;
		__This->bufferEndPosition = __This->bufferBeginPosition + readCount;

		/* OK, read in the data. */
		checkSeek = CS_fseek (__This->strm,__This->bufferBeginPosition,SEEK_SET);
		if (checkSeek < 0L)
		{
			CS_stncp (csErrnam,__This->filePath,MAXPATH);
			CS_erpt (cs_IOERR);
			goto error;
		}
		checkCount = (long32_t)CS_fread (__This->dataBuffer,1,(size_t)readCount,__This->strm);
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
		/* We assume that the binary file is manufactured on the system which is
		   using it.  Therefore, byte swapping is not an issue. */
	}

	/* Extract the grid cell. */
	chrPtr = (char *)__This->dataBuffer + (fposBegin - __This->bufferBeginPosition);
	fltPtr = (float *)(chrPtr);									 /*lint !e826*/
	southWest = *fltPtr;
	southEast = *(fltPtr + 1);
	chrPtr += __This->recordSize;
	fltPtr = (float *)(chrPtr);									 /*lint !e826*/
	northWest = *fltPtr;
	northEast = *(fltPtr + 1);

	/* Swap the bytes as necessary. */
	CS_bswap (&southWest,"f");
	CS_bswap (&southEast,"f");
	CS_bswap (&northWest,"f");
	CS_bswap (&northEast,"f");

	/* Perform the bi-linear calculation. */
	tt = fmod (xy [0],2000.0) / 2000.0;
	uu = fmod (xy [1],2000.0) / 2000.0;

	*geoidHgt = southWest +
				tt * (southEast - southWest) +
				uu * (northWest - southWest) +
				tt * uu * (southWest - southEast - northWest + northEast);

	/* We're done. */
	return 0;

error:
	CSreleaseOsgm91 (__This);

	/* We do the following to make sure that no application uses an error
	   without some one noticing it. */
	*geoidHgt = cs_Mhuge;
	return -1;
}
/* This function creates a binary version of the Osgm91 file if it doesn't
   exist, or if the date on the binary file is older than that of the
   text file.  Note, that this function uses the path name in the
   provide object, and modifies it to point to the binary file which
   is created.
   
   This function is required as records in the ASCII text file are not
   of fixed length, and there are a million of them.  The binary file
   enables random access to the data file for decent performance without
   eating up 1.6MB of RAM. */
int CSmkBinaryOsgm91 (struct cs_Osgm91_ *__This)
{
	extern char cs_ExtsepC;
	extern char csErrnam [];

	int st;
	cs_Time_ aTime, bTime;
	
	char *cp;
	csFILE *aStrm, *bStrm;

	float floatBufr;
	char lineBufr [128];
	char binaryPath [MAXPATH];

	CS_stncp (binaryPath,__This->filePath,sizeof (binaryPath));
	cp = strrchr (binaryPath,cs_ExtsepC);
	if (cp == NULL) 
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}
	CS_stcpy ((cp + 1),"_gm");

	aTime = CS_fileModTime (__This->filePath);
	if (aTime == 0)
	{
		CS_stncp (csErrnam,__This->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}

	bTime = CS_fileModTime (binaryPath);
	if (bTime == 0 || bTime <= aTime)
	{
		/* Here to create a, possibly new, binary version of the
		   OSGB97.txt file.  We write a file which has two floats
		   for each line of text that we read. */
		aStrm = CS_fopen (__This->filePath,_STRM_TXTRD);
		if (aStrm == NULL)
		{
			CS_stncp (csErrnam,__This->filePath,MAXPATH);
			CS_erpt (cs_DTC_FILE);
			goto error;
		}
		/* The mode of the following open will truncate any existing file, and
		   create a new file if necessary. */
		bStrm = CS_fopen (binaryPath,_STRM_BINWR);
		if (bStrm == NULL)
		{
			CS_stncp (csErrnam,__This->filePath,MAXPATH);
			CS_erpt (cs_FL_OPEN);
			goto error;
		}

		/* If we're still here, we can copy the file, converting it to
		   binary form as we do. */
		while (CS_fgets (lineBufr,sizeof (lineBufr),aStrm) != NULL)
		{
			floatBufr = (float)(atof (lineBufr) / 1000.0);
			CS_fwrite (&floatBufr,sizeof (floatBufr),1,bStrm);
		}
		CS_fclose (aStrm);
		st = CS_fclose (bStrm);
		if (st != 0)
		{
			CS_erpt (cs_INV_FILE);
			goto error;
		}
	}

	/* If all that was done successfully, we change the name of
	   the file and return success. */
	CS_stncp (__This->filePath,binaryPath,sizeof (__This->filePath));
	return 0;
error:
	return -1;
}
#ifdef _DEBUG
double CSdebugOsgm91 (struct cs_Osgm91_ *__This)
{
	int st;
	double calcHgt;
	double knownHgt;
	double etrs89 [2];

	etrs89 [0] =  1.71753230899;
	etrs89 [1] = 52.65725668745;
	st = CScalcOsgm91 (__This,&calcHgt,etrs89);
	knownHgt = 44.273306;
	return fabs (calcHgt - knownHgt);
}
#endif

