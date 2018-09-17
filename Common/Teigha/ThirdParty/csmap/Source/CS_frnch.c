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
 *       from this software without specific p0rior written permission.
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

/*lint -esym(613,err_list)  possible use of a null pointer; but not really. */

int CSfrnchQ (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz)
{
	extern char cs_DirsepC;

	int err_cnt;
	size_t rdCnt;

	char *cp;
	csFILE* strm;

	char chrBuffer [8];
	char pathBuffer [MAXPATH];

	cp = fileParms->fileName;
	if (*cp == '.' && *(cp + 1) == cs_DirsepC)
	{
		CS_stncp (pathBuffer,dictDir,sizeof (pathBuffer));
		CS_stncat (pathBuffer,cp,MAXPATH);
	}
	else
	{
		CS_stncp (pathBuffer,cp,MAXPATH);
	}

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Verify that the file exists and that the format appears to be correct. */
	strm = CS_fopen (pathBuffer,_STRM_TXTRD);
	if (strm != NULL)
	{
		rdCnt = CS_fread (chrBuffer,1,sizeof (chrBuffer),strm);
		CS_fclose (strm);
		strm = NULL;

		if (rdCnt != sizeof (chrBuffer) || CS_strnicmp (chrBuffer," GR3D",5))
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FORMAT;
		}
	}
	else
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FILE;
	}
	return (err_cnt + 1);
}
int CSfrnchS  (struct cs_GridFile_ *gridFile)
{
	int status;

	struct cs_Frnch_ *frnchPtr;

	status = csGRIDI_ST_SYSTEM;
	frnchPtr = CSnewFrnch (gridFile->filePath,gridFile->bufferSize,gridFile->flags,gridFile->density);
	if (frnchPtr != NULL)
	{
		/* OK, it initialized OK. */
		frnchPtr->cnvrgValue = gridFile->cnvrgValue;
		frnchPtr->errorValue = gridFile->errorValue;
		frnchPtr->maxIterations = gridFile->maxIterations;
		gridFile->fileObject.FrenchRgf = frnchPtr;

		gridFile->test = (cs_TEST_CAST)CSfrnchT;
		gridFile->frwrd2D = (cs_FRWRD2D_CAST)CSfrnchF2;
		gridFile->frwrd3D = (cs_FRWRD3D_CAST)CSfrnchF3;
		gridFile->invrs2D = (cs_INVRS2D_CAST)CSfrnchI2;
		gridFile->invrs3D = (cs_INVRS3D_CAST)CSfrnchI3;
		gridFile->inRange = (cs_INRANGE_CAST)CSfrnchL;
		gridFile->release = (cs_RELEASE_CAST)CSfrnchR;
		gridFile->destroy = (cs_DESTROY_CAST)CSfrnchD;
		
		status = csGRIDI_ST_OK;
	}
	return status;
}
double CSfrnchT (struct cs_Frnch_ *frnch,double *ll_src,short direction)
{
	double density;

	/* For this fiule format, we don't care about the direction. */
	density = CStestFrnch (frnch,ll_src);
	return density;
}
int CSfrnchF2 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;

	int status;

	double myRgf [3];
	double myNtf [3];

	myRgf [LNG] = ll_src [LNG];
	myRgf [LAT] = ll_src [LAT];
	myRgf [HGT] = cs_Zero;
	status = CScalcRgfToNtf (frnch,myNtf,myRgf);
	if (status >= csGRIDI_ST_OK)
	{
		ll_trg [LNG] = myNtf [LNG];
		ll_trg [LAT] = myNtf [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	else
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	return status;
}
int CSfrnchF3 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src)
{
	int status;

	double myNtf [3];

	status = CScalcRgfToNtf (frnch,myNtf,ll_src);
	if (status >= csGRIDI_ST_OK)
	{
		ll_trg [LNG] = myNtf [LNG];
		ll_trg [LAT] = myNtf [LAT];
		ll_trg [HGT] = myNtf [HGT];
	}
	else
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	return status;
}
int CSfrnchI2 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;

	int status;

	double myNtf [3];
	double myRgf [3];

	myNtf [LNG] = ll_src [LNG];
	myNtf [LAT] = ll_src [LAT];
	myNtf [HGT] = cs_Zero;
	status = CScalcNtfToRgf (frnch,myRgf,myNtf);
	if (status >= csGRIDI_ST_OK)
	{
		ll_trg [LNG] = myRgf [LNG];
		ll_trg [LAT] = myRgf [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	else
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	return status;
}
int CSfrnchI3 (struct cs_Frnch_ *frnch,double *ll_trg,Const double *ll_src)
{
	int status;

	double myNtf [3];
	double myRgf [3];

	myNtf [LNG] = ll_src [LNG];
	myNtf [LAT] = ll_src [LAT];
	myNtf [HGT] = ll_src [HGT];
	status = CScalcNtfToRgf (frnch,myRgf,myNtf);
	if (status >= csGRIDI_ST_OK)
	{
		ll_trg [LNG] = myRgf [LNG];
		ll_trg [LAT] = myRgf [LAT];
		ll_trg [HGT] = myRgf [HGT];
	}
	else
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	return status;
}
int CSfrnchL  (struct cs_Frnch_ *frnch,int cnt,Const double pnts [][3])
{
	short ok;
	int index;
	double density;

	ok = TRUE;
	for (index = 0;index < cnt;index += 1)
	{
		/* Note: CSfrnchT ignores the direction parameter.  The assumption
		   always forward. */
		density = CSfrnchT (frnch,pnts [index],cs_DTCDIR_FWD);
		if (fabs (density) > 1.0E-08)		/* i.e. != 0.0 */
		{
			ok = FALSE;
			break;
		}
	}
	return (ok != TRUE) ? cs_CNVRT_USFL : cs_CNVRT_OK;
}
int CSfrnchR  (struct cs_Frnch_ *frnch)
{
	return 0;
}
int CSfrnchD  (struct cs_Frnch_ *frnch)
{
	CSdeleteFrnch (frnch);
	return 0;
}
struct cs_Frnch_* CSnewFrnch (Const char *filePath,long32_t bufferSize,ulong32_t flags,double density)
{
	int status;

	struct cs_Frnch_* frnchPtr;
	
	/* Allocate the initial chunk of memory for this thing. */
	status = 0;
	frnchPtr = CS_malc (sizeof (struct cs_Frnch_));
	if (frnchPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	else
	{
		status = CSinitFrnch (frnchPtr,filePath,bufferSize,flags,density);
	}
	if (status != 0)
	{
		goto error;
	}
	return frnchPtr;
error:
	if (frnchPtr != NULL)
	{
		CS_free (frnchPtr);
		frnchPtr = NULL;
	}
	return NULL;
}

void CSinitializeFrnchObj (struct cs_Frnch_ *thisPtr)
{
	extern double cs_Zero;

	CSinitCoverage (&thisPtr->coverage);
	thisPtr->lngCount = 0L;
	thisPtr->latCount = 0L;
	thisPtr->deltaLng = cs_Zero;
	thisPtr->deltaLat = cs_Zero;
	thisPtr->rgf93ERad = cs_Zero;
	thisPtr->rgf93ESq = cs_Zero;
	thisPtr->ntfERad = cs_Zero;
	thisPtr->ntfESq = cs_Zero;
	thisPtr->filePath [0] = '\0';
	thisPtr->fileName [0] = '\0';
	thisPtr->deltaX = NULL;
	thisPtr->deltaY = NULL;
	thisPtr->deltaZ = NULL;
	thisPtr->crcX = 0U;
	thisPtr->crcY = 0U;
	thisPtr->crcZ = 0U;
	return;
}
int CSinitFrnch (struct cs_Frnch_* thisPtr,Const char *filePath,long32_t bufferSize,ulong32_t flags,double density)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];
	extern double cs_Zero;

	int status;
	int hdrFlag = 0;

	long32_t dblFrmt;
	long32_t lngIdx, latIdx;
	unsigned arrayIdx;
	unsigned tknCount;
	size_t malcSize;

	char *cpV;
	csFILE *fstrm;
	struct cs_Eldef_ *elPtr;
	double lng, lat;
	double deltaX, deltaY, deltaZ;

	double sw [3], ne [2];
	char *ptrs [20];

	char lineBuffer [256];

	/* Get the object we are constructing into a known state. */
	CSinitializeFrnchObj (thisPtr);

	/* Prepare for an error. */
	fstrm = NULL;
	elPtr = NULL;
	status = -1;

	/* Keep lint happy! */
	sw [0] = sw [1] = ne [0] = ne [1] = cs_Zero;

	/* Capture the full path to the file and the file name. */
	CS_stncp (thisPtr->filePath,filePath,sizeof (thisPtr->filePath));
	cpV = strrchr (thisPtr->filePath,cs_DirsepC);
	if (cpV != NULL)
	{
		cpV += 1;
	}
	else
	{
		cpV = thisPtr->filePath;
	}
	CS_stncp (thisPtr->fileName,cpV,sizeof (thisPtr->fileName));
	cpV = strrchr (thisPtr->fileName,cs_ExtsepC);
	if (cpV != NULL) *cpV = '\0';

	/* OK, open the file, and extra the parameters.  There are two types of
	   files.  We handle either kind (since we can do it all in this module).
	   Get the first line and determine which type of file we have. */
	fstrm = CS_fopen (filePath,_STRM_TXTRD);
	if (fstrm == NULL)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}

	/* If a buffersize has been given, we use it. */
	if (bufferSize > 0L)
	{
		setvbuf (fstrm,NULL,_IOFBF,(size_t)bufferSize);
	}

	/* Parse the file content. */
	if (CS_fgets (lineBuffer,sizeof (lineBuffer),fstrm) == NULL)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	tknCount = CS_spaceParse (lineBuffer,ptrs,20);

	/* Decide which file type we have. */
	if (!CS_stricmp (ptrs [0],"GR3D"))
	{
		/* Here for the verbose format. */
		while (CS_fgets (lineBuffer,sizeof (lineBuffer),fstrm) != NULL)
		{
			tknCount = CS_spaceParse (lineBuffer,ptrs,20);
			if (!CS_stricmp (ptrs [0],"GR3D3")) break;
			if (!CS_stricmp (ptrs [0],"GR3D1") && tknCount == 7)
			{
				hdrFlag = 1;
				/* Convert the parsed tokens in a locale independent manner.
				   That is, we need to avoid use of functions like strtod
				   as they are locale dependent. */
				dblFrmt  = CSatof (&sw[0],ptrs [1],'.',',',':');
				dblFrmt |= CSatof (&ne[0],ptrs [2],'.',',',':');
				dblFrmt |= CSatof (&sw[1],ptrs [3],'.',',',':');
				dblFrmt |= CSatof (&ne[1],ptrs [4],'.',',',':');
				dblFrmt |= CSatof (&thisPtr->deltaLng,ptrs [5],'.',',',':');
				dblFrmt |= CSatof (&thisPtr->deltaLat,ptrs [6],'.',',',':');
				if (dblFrmt < 0)
				{
					/* This should never happen with a good data file. */
					CS_erpt (cs_INV_FILE);
					goto error;
				}
			}
		}
		if (hdrFlag == 0)
		{
			/* If we didn't see the expected header record, we have
			   a big problem. */
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_INV_FILE);
			goto error;
		}

		/* Capture the coverage area. */
		thisPtr->coverage.southWest [LNG] = sw [0];
		thisPtr->coverage.southWest [LAT] = sw [1];
		thisPtr->coverage.northEast [LNG] = ne [0];
		thisPtr->coverage.northEast [LAT] = ne [1];

		/* Set the density.  Normally it would be the smaller of deltaLng or
		   deltaLat.  These are usually the same for this file type.
		   Alternatively, a density may have been specified by the user in the
		   dictionary file.  In that case, we use that value. */
		if (density > 1.0E-12)			/* like to avoid == on doubles */
		{
			thisPtr->coverage.density = density;
		}
		else if (thisPtr->deltaLat >= thisPtr->deltaLng)
		{
			thisPtr->coverage.density = thisPtr->deltaLat;
		}
		else
		{
			thisPtr->coverage.density = thisPtr->deltaLng;
		}

		/* Compute the size of the grid.  We need to add the extra 0.1 to
		   account for round off errors on certain compilers/platforms. */
		thisPtr->lngCount = (long32_t)(((ne [0] - sw [0]) / thisPtr->deltaLng) + 0.1) + 1;
		thisPtr->latCount = (long32_t)(((ne [1] - sw [1]) / thisPtr->deltaLat) + 0.1) + 1;

		/* Now we can allocate the arrays. */
		malcSize = (size_t)(thisPtr->lngCount * thisPtr->latCount) * sizeof (long32_t);
		thisPtr->deltaX = CS_malc (malcSize);
		if (thisPtr->deltaX == NULL)
		{
			CS_erpt (cs_NO_MEM);
			goto error;
		}
		thisPtr->deltaY = CS_malc (malcSize);
		if (thisPtr->deltaY == NULL)
		{
			CS_erpt (cs_NO_MEM);
			goto error;
		}
		thisPtr->deltaZ = CS_malc (malcSize);
		if (thisPtr->deltaZ == NULL)
		{
			CS_erpt (cs_NO_MEM);
			goto error;
		}

		/* Initialize the delta arrays to zero.  Perhaps we should use memset
		   here for performance reasons.  Not much to be gained, however, since
		   we have to parse through 2MB of text anyway. */
		for (lngIdx = 0;lngIdx < thisPtr->lngCount;lngIdx += 1)
		{
			for (latIdx = 0;latIdx < thisPtr->latCount;latIdx += 1)
			{
				arrayIdx = (unsigned)((latIdx * thisPtr->lngCount) + lngIdx);
				*(thisPtr->deltaX + arrayIdx) = 0L;
				*(thisPtr->deltaY + arrayIdx) = 0L;
				*(thisPtr->deltaZ + arrayIdx) = 0L;
			}
		}

		/* Fill in the ellipsoid numbers.  Pretty hoeky, but it works.
		   Unfortunately, the specific ellipsoids involved are impled by the
		   data file, but the numbers are not included in the format.  Thus,
		   for now, this grid file format is usful only for the specific French geography for which it was developed. */
		elPtr = CS_eldef ("GRS1980");
		if (elPtr == NULL) goto error;
		thisPtr->rgf93ERad = elPtr->e_rad;
		thisPtr->rgf93ESq = elPtr->ecent * elPtr->ecent;
		CS_free (elPtr);
		elPtr = NULL;

		elPtr = CS_eldef ("CLRK-IGN");
		if (elPtr == NULL) goto error;
		thisPtr->ntfERad = elPtr->e_rad;
		thisPtr->ntfESq = elPtr->ecent * elPtr->ecent;
		CS_free (elPtr);
		elPtr = NULL;

		/* Process the rest of the file. */
		while (CS_fgets (lineBuffer,sizeof (lineBuffer),fstrm) != NULL)
		{
			tknCount = CS_spaceParse (lineBuffer,ptrs,20);
			if (tknCount == 8)
			{
				/* Convert the data to numeric form in a locale independent
				   manner. */
				dblFrmt  = CSatof (&lng,ptrs [1],'.',',',':');
				dblFrmt |= CSatof (&lat,ptrs [2],'.',',',':');
				dblFrmt |= CSatof (&deltaX,ptrs [3],'.',',',':');
				dblFrmt |= CSatof (&deltaY,ptrs [4],'.',',',':');
				dblFrmt |= CSatof (&deltaZ,ptrs [5],'.',',',':');
				if (dblFrmt < 0)
				{
					/* This should never happen with a good data file. */
					CS_erpt (cs_INV_FILE);
					goto error;
				}

				/* Determine the location of the node in the grid. */
				lngIdx = (long32_t)(((lng - sw [0]) / thisPtr->deltaLng) + 1.0E-10);
				latIdx = (long32_t)(((lat - sw [1]) / thisPtr->deltaLat) + 1.0E-10);
				if (lngIdx < 0 || lngIdx >= thisPtr->lngCount ||
					latIdx < 0 || latIdx >= thisPtr->latCount)
				{
					/* This should never happen with a good data file. */
					CS_erpt (cs_INV_FILE);
					goto error;
				}
				
				/* Adjust for possible round down; required for Linux (or is it
				   the gcc runtime library), perhaps others as well. */
				deltaX += (deltaX < 0.0) ? -0.0002 : 0.0002;
				deltaY += (deltaY < 0.0) ? -0.0002 : 0.0002;
				deltaZ += (deltaZ < 0.0) ? -0.0002 : 0.0002;

				/* Stuff extracted values in the local arrays. */
				arrayIdx = (unsigned)((latIdx * thisPtr->lngCount) + lngIdx);
				*(thisPtr->deltaX + arrayIdx) = (long32_t)(deltaX * 1000.0);
				*(thisPtr->deltaY + arrayIdx) = (long32_t)(deltaY * 1000.0);
				*(thisPtr->deltaZ + arrayIdx) = (long32_t)(deltaZ * 1000.0);
			}
		}
	}
	else
	{
		/* We don't support the abbreviated file.  The abbreviated file is
		   simply a binary version of the text file.  Thus, we avoid all byte
		   swapping issues.  There is little to be gained by using the
		   abbreviated file. */
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* For testing purposes, we generate the check-sum of the three memory
	   arrays allocated above.  The CScheckRgf93ToNtf function verifies that the
	   memory in these arrays has not changed since they were allocated.
	   
	   This was implemented in response to a bug report which suggested that the
	   in memory arrays may have been corrupted by some code elsehwere in the
	   library.  This check has never produced a failure and this code is a
	   good candidate for removal. */
	thisPtr->crcX = CS_crc16 (0X0101,(unsigned char *)thisPtr->deltaX,(int)malcSize);
	thisPtr->crcY = CS_crc16 (0X0202,(unsigned char *)thisPtr->deltaY,(int)malcSize);
	thisPtr->crcZ = CS_crc16 (0X0404,(unsigned char *)thisPtr->deltaZ,(int)malcSize);

	/* OK, we're outa here. */
	CS_fclose (fstrm);
	fstrm = NULL;
	status = csGRIDI_ST_OK;
	return status;

error:
	if (elPtr != NULL)
	{
		CS_free (elPtr);
		elPtr = NULL;
	}
	if (fstrm != NULL)
	{
		CS_fclose (fstrm);
		fstrm = NULL;
	}
	return csGRIDI_ST_SYSTEM;
}
/* Release allocated resources without losing existence information.
	object can still be used. */
void CSreleaseFrnch (struct cs_Frnch_* thisPtr)
{
}
void CSdeleteFrnch (struct cs_Frnch_* thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->deltaX != NULL)
		{
			CS_free (thisPtr->deltaX);
			thisPtr->deltaX = NULL;
		}
		if (thisPtr->deltaY != NULL)
		{
			CS_free (thisPtr->deltaY);
			thisPtr->deltaY = NULL;
		}
		if (thisPtr->deltaZ != NULL)
		{
			CS_free (thisPtr->deltaZ);
			thisPtr->deltaZ = NULL;
		}
		CS_free (thisPtr);
	}
}
/* Returns the name of the specific grid file which would be used if the
   provided point was to be converted. */
Const char* CSpathFrnch (Const struct cs_Frnch_* thisPtr)
{
	return (Const char *)thisPtr->filePath;
}
double CStestFrnch (struct cs_Frnch_* thisPtr,Const double location [2])
{
	double density;

	density = CStestCoverage (&thisPtr->coverage,location);
	return density;
}

/* Given a RGF93 lat/long in degreess relative to Greenwich, this function
   extracts the appropaite delta X, Y, and Z from the grid file.  Returns +1
   if the provided lat/long are not covered by the grid file, in which case,
   the average geocentric translation values are returned.  These default
   values are hard coded. */
int CScalcRgf2NtfDeltas (struct cs_Frnch_* thisPtr,double *deltaX,double *deltaY,double *deltaZ,Const double ll_rgf93 [3])
{
	short westEdg, northEdg, eastEdg, southEdg;

	int rtnVal;

	unsigned swIdx, nwIdx, neIdx, seIdx;

	long32_t lngIdx, latIdx;

	double density;
	double cellLng, cellLat;
	double x1, x2, x3, x4;
	double y1, y2, y3, y4;										/*lint !e578 */
	double z1, z2, z3, z4;

	double sw [3];

	rtnVal = 0;

	/* In the case of a +1 return, we return the standard values. */
	*deltaX = -168.0;
	*deltaY =  -60.0;
	*deltaZ =  320.0;

	/* Check and make sure we have proper coverage. */
	density = CStestCoverage (&thisPtr->coverage,ll_rgf93);
	if (density == 0.0) return csGRIDI_ST_COVERAGE;

	/* Here if we have coverage.  Calculate the indices to the appropriate cell. */
	lngIdx = (long32_t)((ll_rgf93 [0] - thisPtr->coverage.southWest [0]) / thisPtr->deltaLng);
	latIdx = (long32_t)((ll_rgf93 [1] - thisPtr->coverage.southWest [1]) / thisPtr->deltaLat);

	/* The coverage test should have caught any coordinate which would cause
	   us a problem.  But to be extra safe: */
	westEdg  = (short)(lngIdx < 0);
	northEdg = (short)(latIdx >= thisPtr->latCount);
	eastEdg  = (short)(lngIdx >= thisPtr->lngCount);
	southEdg = (short)(latIdx < 0);
	if (westEdg || northEdg || eastEdg || southEdg) return 1;

	/* We calculate the geographic coordinates of the southwest corner of the
	   appropriate grid cell. */
	sw [0] = thisPtr->coverage.southWest [0] + (lngIdx * thisPtr->deltaLng);
	sw [1] = thisPtr->coverage.southWest [1] + (latIdx * thisPtr->deltaLat);

	/* Now, deltaLng and deltaLat are the normalized distances into the
	   grid cell of the provided point. */
	cellLng = (ll_rgf93 [0] - sw [0]) / thisPtr->deltaLng;
	cellLat = (ll_rgf93 [1] - sw [1]) / thisPtr->deltaLat;

	/* Compute the indices into the delta arrays.  Due to the above checking,
	   these indices should be safe to use for memory access. */
	swIdx = (unsigned)(( latIdx      * thisPtr->lngCount) + lngIdx);
	nwIdx = (unsigned)(((latIdx + 1) * thisPtr->lngCount) + lngIdx);
	neIdx = (unsigned)(((latIdx + 1) * thisPtr->lngCount) + lngIdx + 1);
	seIdx = (unsigned)(( latIdx      * thisPtr->lngCount) + lngIdx + 1);

	/* Now we can do the bilinear calculation.  The nomenclature here jives
	   with the reference document. */
	x1 = (double)(*(thisPtr->deltaX + swIdx)) * 0.001;
	x2 = (double)(*(thisPtr->deltaX + nwIdx)) * 0.001;
	x3 = (double)(*(thisPtr->deltaX + seIdx)) * 0.001;
	x4 = (double)(*(thisPtr->deltaX + neIdx)) * 0.001;

	y1 = (double)(*(thisPtr->deltaY + swIdx)) * 0.001;
	y2 = (double)(*(thisPtr->deltaY + nwIdx)) * 0.001;
	y3 = (double)(*(thisPtr->deltaY + seIdx)) * 0.001;
	y4 = (double)(*(thisPtr->deltaY + neIdx)) * 0.001;

	z1 = (double)(*(thisPtr->deltaZ + swIdx)) * 0.001;
	z2 = (double)(*(thisPtr->deltaZ + nwIdx)) * 0.001;
	z3 = (double)(*(thisPtr->deltaZ + seIdx)) * 0.001;
	z4 = (double)(*(thisPtr->deltaZ + neIdx)) * 0.001;

	*deltaX = x1 + (x3 - x1) * cellLng + (x2 - x1) * cellLat +
				   (x1 - x2 - x3 + x4) * cellLng * cellLat;
	*deltaY = y1 + (y3 - y1) * cellLng + (y2 - y1) * cellLat +
				   (y1 - y2 - y3 + y4) * cellLng * cellLat;
	*deltaZ = z1 + (z3 - z1) * cellLng + (z2 - z1) * cellLat +
				   (z1 - z2 - z3 + z4) * cellLng * cellLat;

	/* If we are still here, the return value is zero. */
	return rtnVal;
}

int CScalcRgfToNtf (struct cs_Frnch_* thisPtr,double* llNtf,Const double *llRgf93)
{
	int rtnVal, xyzSt;

	double deltaX, deltaY, deltaZ;

	double xyz [3];

	/* Until we know different */
	rtnVal = 0;
	llNtf [0] = llRgf93 [0];
	llNtf [1] = llRgf93 [1];
	llNtf [2] = llRgf93 [2];

	/* Determine the delta shift values. */
	rtnVal = CScalcRgf2NtfDeltas (thisPtr,&deltaX,&deltaY,&deltaZ,llRgf93);

	/* TO DO: The CScalcNtfToRfg93Deltas function will return the average
	   geocentric translation values for France whenever presented a geographic
	   coordinate outside the coverage of the grid data file.  These values are
	   hardcoded.  These values should, perhaps, be obtained from the fallback
	   or some means other than hardcoded.  Since the ellipsoids in use are also
	   harcoded (as there are no ellipsoid numbers in the data file format) this
	   is probably not a big deal. */

	/* A status value of +1 says that the given point is not covered by the grid file.
	   However, in this case, CScalcNtf2RfgDeltas returns delta X, Y, & Z set
	   to average values.  Thus, we do the following regardless of the value
	   of rtnVal. */
	if (rtnVal >= csGRIDI_ST_OK)
	{
		/* OK, we have the appropriate delta values.  We need to do a three parameter
		   transformation using these values.  Before we can do this, we need to convert
		   the source geographic coordinates to geocentric form. */
		CS_llhToXyz (xyz,llRgf93,thisPtr->rgf93ERad,thisPtr->rgf93ESq);
		xyz [0] -= deltaX;
		xyz [1] -= deltaY;
		xyz [2] -= deltaZ;
		xyzSt = CS_xyzToLlh (llNtf,xyz,thisPtr->ntfERad,thisPtr->ntfESq);
		if (xyzSt != csGRIDI_ST_OK)
		{
			CS_erpt ( cs_XYZ_ITR);
			rtnVal = csGRIDI_ST_COVERAGE;
		}
	}
	return rtnVal;
}
#if !defined (__SKIP__)
/* Since the following code has been tested, we leave it in here.  Easier to
   delete than it is to create.
   
   It is important to note that while this algorithm will produce the exact
   (i.e. within .1 millimeters) inverse, it does not take the height of the
   NTF source point into consideration.  Thus, while the horizontal inverse
   is rather precise, the effect of the height on this calculation has not
   been evaluated as yet. */
int CScalcNtfToRgf (struct cs_Frnch_* thisPtr,double* llRgf,Const double* llNtf)
{
	extern double cs_Zero;

	int ii;
	int lngOk;
	int latOk;
	int rtnVal;

	double guess [3];
	double newLl [3];
	double epsilon [3];

	epsilon [0] = epsilon [1] = epsilon [2] = cs_Zero;	/* keep lint happy */

	/* Assume everything goes OK until we know different. */
	rtnVal = csGRIDI_ST_OK;

	/* First, we copy the source lat/longs to the local array.
	   This is the default result which the user may want in
	   the event of an error.  Note, we assume such has been
	   done below, even if there has not been an error. */
	llRgf [LNG] = guess [LNG] = llNtf [LNG];
	llRgf [LAT] = guess [LAT] = llNtf [LAT];
	llRgf [HGT] = guess [HGT] = llNtf [HGT];

	/* Start a loop which will iterate as many as maxIteration times. */
	for (ii = 0;ii < thisPtr->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		rtnVal = CScalcRgfToNtf (thisPtr,newLl,guess);
		if (rtnVal != csGRIDI_ST_OK)
		{
			/* Oopps!! It appears this iterative inverse function has wandered
			   across the grid file coverage boundary. */
			break;
		}

		/* See how far we are off. */
		epsilon [LNG] = llNtf [LNG] - newLl [LNG];
		epsilon [LAT] = llNtf [LAT] - newLl [LAT];

		/* If our guess at the longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > thisPtr->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > thisPtr->cnvrgValue)
		{
			latOk = FALSE;
			guess [LAT] += epsilon [LAT];
		}

		/* If our current guess produces a newLl that is within
		   samllValue of srcLl, we are done. */
		if (lngOk && latOk) break;
	}

	if (rtnVal == csGRIDI_ST_OK)
	{
		/* If we didn't resolve in maxIteration tries, we issue a warning
		   message.  Usually, three or four iterations does the trick. */
		if (ii >= thisPtr->maxIterations)
		{
			rtnVal = csGRIDI_ST_COVERAGE;
			CS_erpt (cs_RGF93_ICNT);

			/* Issue a warning if we're close, a fatal if we are still way off.
			   In any case, we return the last computed value.  We could have
			   gotten very fancy with this stuff, but it would have had serious
			   affects on the performance.  So we just check epsilon here as
			   we know we have an error and this doesn't happen very often. */
			if (fabs (epsilon [LNG]) > thisPtr->errorValue ||
				fabs (epsilon [LAT]) > thisPtr->errorValue)
			{
				rtnVal = csGRIDI_ST_SYSTEM;
			}
		}
	}

	/* Return the resuls if everything converged. */
	if (rtnVal == csGRIDI_ST_OK)
	{
		llRgf [LNG] = guess [LNG];
		llRgf [LAT] = guess [LAT];
	}
	else
	{
		llRgf [LNG] = llNtf [LNG];
		llRgf [LAT] = llNtf [LAT];
	}
	return rtnVal;
}
#else
/* The following calculation is the calculation as described in the
   source document from the IGN.  It produces values which are within
   1.5 millimeters of the original  coordinate.  So while the above is
   more accurate (accurate to 0.1 millimeters), this is the algorithm
   used as it is believed to be the officially sanctioned technique. */
int CScalcNtfToRgf (struct cs_Frnch_* thisPtr,double* llRgf,Const double* llNtf)
{
	int rtnVal = 0;

	double deltaX, deltaY, deltaZ;

	double xyz [3];

	/* We start with the basic, default, average, values. */
	deltaX = -168.0;
	deltaY =  -60.0;
	deltaZ =  320.0;

	/* The grid file is indexed by RGF93 lat/longs, even though the delta values
	   in the grid file are the values which convert from NTF to RGF93.  We only
	   have ntf lat/longs.  So, first thing we do is to do an approximate
	   calculation, converting the provided NTF lat/longs to RGF93 lat/longs using
	   standard, average, delta X, Y, & Z values.  Note that in the event
	   of an error below, these approximate results are allowed to remain in
	   the target array. */
	CS_llhToXyz (xyz,llNtf,thisPtr->ntfERad,thisPtr->ntfESq);
	xyz [0] += deltaX;
	xyz [1] += deltaY;
	xyz [2] += deltaZ;
	rtnVal = CS_xyzToLlh (llRgf,xyz,thisPtr->rgf93ERad,thisPtr->rgf93ESq);
	if (rtnVal != 0)
	{
		CS_erpt ( cs_XYZ_ITR);
		rtnVal = csGRIDI_ST_COVERAGE;
	}
	else
	{
		/* Now we use this approximate RGF93 lat/long to access the grid file to get
		   the Delta X, Y, & Z.  Obviously, not perfectly exact, but very close. */
		rtnVal = CScalcRgf2NtfDeltas (thisPtr,&deltaX,&deltaY,&deltaZ,llRgf);

		/* A status value of +1 says that the given point is not covered by the grid file.
		   In this case, since we have already calculated an approximation, we are done. */
		if (rtnVal == 0)
		{
			CS_llhToXyz (xyz,llNtf,thisPtr->ntfERad,thisPtr->ntfESq);
			xyz [0] += deltaX;
			xyz [1] += deltaY;
			xyz [2] += deltaZ;
			rtnVal = CS_xyzToLlh (llRgf,xyz,thisPtr->rgf93ERad,thisPtr->rgf93ESq);
			if (rtnVal != 0)
			{
				CS_erpt ( cs_XYZ_ITR);
				rtnVal = csGRIDI_ST_COVERAGE;
			}
		}
	}
	return rtnVal;
}
#endif
