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

/*lint -esym(613,err_list)  possible use of null pointer; but not really */

long32_t csNadconBufrSize = 16384;			/* This buffer size ensures that any
											   HARN/HPGN grid file is read into
											   memory and stored in the internal
											   buffer. */

int CSnadcnQ  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz)
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

	/* Verify that both files exist and that the format appears to be correct.
	   We'll need a copy of the path which we can modify. */
	cp = pathBuffer + strlen (pathBuffer) - 2;
	if (*cp != '?')
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FLSPEC;
	}
	else
	{
		*cp = 'o';
		strm = CS_fopen (pathBuffer,_STRM_BINRD);
		if (strm != NULL)
		{
			rdCnt = CS_fread (chrBuffer,1,sizeof (chrBuffer),strm);
			CS_fclose (strm);
			strm = NULL;

			if (rdCnt != sizeof (chrBuffer) || CS_strnicmp (chrBuffer,"NADCON",6))
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FORMAT;
			}
		}
		else
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FILE;
		}
		*cp = 'a';
		strm = CS_fopen (pathBuffer,_STRM_BINRD);
		if (strm != NULL)
		{
			rdCnt = CS_fread (chrBuffer,1,sizeof (chrBuffer),strm);
			CS_fclose (strm);
			strm = NULL;

			if (rdCnt != sizeof (chrBuffer) || CS_strnicmp (chrBuffer,"NADCON",6))
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FORMAT;
			}
		}
		else
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FILE;
		}
	}
	return (err_cnt + 1);
}
int CSnadcnS (struct cs_GridFile_ *gridFile)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;

	int status;

	char *cpFront;
	char *cpBack;
	struct cs_Nadcn_ *nadcnPtr = NULL;
	char ctemp [MAXPATH];

	nadcnPtr = (struct cs_Nadcn_ *)CS_malc (sizeof (struct cs_Nadcn_));
	if (nadcnPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	nadcnPtr->lngShift = NULL;
	nadcnPtr->latShift = NULL;

	/* Extract the path to the file to a place we can modify as we prepare to
	   open two different files. */
	CS_stncp (ctemp,gridFile->filePath,sizeof (ctemp));
	cpBack = strrchr (ctemp,cs_DirsepC);
	if (cpBack == NULL)
	{
		cpBack = ctemp;
	}
	else
	{
		cpBack += 1;
	}
	cpFront = CS_stristr (cpBack,cs_HPGN_TAG);
	if (cpFront != NULL)
	{
		nadcnPtr->type = nadconHARNShift;
	}
	else
	{
		nadcnPtr->type = nadconNAD27Shift;
	}

	cpFront = strrchr (ctemp,cs_DirsepC);
	if (cpFront == NULL) cpFront = ctemp;
	else cpFront += 1;

	/* Do the .LOS file */
	cpBack = strrchr (cpFront,cs_ExtsepC);
	if (cpBack == NULL) cpBack = cpFront + strlen (cpFront);
	*cpBack++ = cs_ExtsepC;
	*cpBack = '\0';
	strcat (ctemp,cs_NADCON_LOS);
	nadcnPtr->lngShift = CSnewNadconFile (ctemp,gridFile->bufferSize,
											   gridFile->flags,
											   gridFile->density);
	if (nadcnPtr->lngShift == NULL) goto error;

	/* Do the .LAS file */
	cpBack = strrchr (cpFront,cs_ExtsepC);
	if (cpBack == NULL) cpBack = cpFront + strlen (cpFront);
	*cpBack++ = cs_ExtsepC;
	*cpBack = '\0';
	strcat (ctemp,cs_NADCON_LAS);
	nadcnPtr->latShift = CSnewNadconFile (ctemp,gridFile->bufferSize,
											   gridFile->flags,
											   gridFile->density);
	if (nadcnPtr->latShift == NULL) goto error;

	/* Verify that the two files are consistent. */
	if (!CS_cmpDbls (nadcnPtr->lngShift->coverage.southWest [0],nadcnPtr->latShift->coverage.southWest [0]) ||
		!CS_cmpDbls (nadcnPtr->lngShift->coverage.southWest [1],nadcnPtr->latShift->coverage.southWest [1]) ||
		!CS_cmpDbls (nadcnPtr->lngShift->coverage.northEast [0],nadcnPtr->latShift->coverage.northEast [0]) ||
		!CS_cmpDbls (nadcnPtr->lngShift->coverage.northEast [1],nadcnPtr->latShift->coverage.northEast [1]) ||
		nadcnPtr->lngShift->elementCount != nadcnPtr->latShift->elementCount ||
		nadcnPtr->lngShift->recordCount  != nadcnPtr->latShift->recordCount ||
		!CS_cmpDbls (nadcnPtr->lngShift->deltaLng,nadcnPtr->latShift->deltaLng) ||
		!CS_cmpDbls (nadcnPtr->lngShift->deltaLat,nadcnPtr->latShift->deltaLat)
	   )
	{
		CS_erpt (cs_NADCON_CONS);
		goto error;
	}

	/* Finally, in case a 3D call is made, and the collection of data files
	   includes the 48 states (i.e. conus.las and conus,los), we active the
	   Vertcon feature.  WOuld like to rewrite this Vertcon stuff, but time
	   for that wasn't in (by mistake, not purpose) the project schedule. */
	status = CSvrtconInit ();
	if (status != 0)
	{
		goto error;
	}
	else
	{
		nadcnPtr->usingVertcon = TRUE;
	}

	/* OK.  If we are still here all is well.  Return the results of our
	   setup in the provided cs_GridFile_ structure. */
	nadcnPtr->cnvrgValue = gridFile->cnvrgValue;
	nadcnPtr->errorValue = gridFile->errorValue;
	nadcnPtr->maxIterations = gridFile->maxIterations;
	gridFile->fileObject.Nadcon = nadcnPtr;

	gridFile->test = (cs_TEST_CAST)CSnadcnT;
	gridFile->frwrd2D = (cs_FRWRD2D_CAST)CSnadcnF2;
	gridFile->frwrd3D = (cs_FRWRD3D_CAST)CSnadcnF3;
	gridFile->invrs2D = (cs_INVRS2D_CAST)CSnadcnI2;
	gridFile->invrs3D = (cs_INVRS3D_CAST)CSnadcnI3;
	gridFile->inRange = (cs_INRANGE_CAST)CSnadcnL;
	gridFile->release = (cs_RELEASE_CAST)CSnadcnR;
	gridFile->destroy = (cs_DESTROY_CAST)CSnadcnD;
	return 0;

error:
	if (nadcnPtr != NULL)
	{
		if (nadcnPtr->lngShift != NULL) CSdeleteNadconFile (nadcnPtr->lngShift);
		if (nadcnPtr->latShift != NULL) CSdeleteNadconFile (nadcnPtr->latShift);
		CS_free (nadcnPtr);
	}
	return -1;
}
double CSnadcnT (struct cs_Nadcn_ *nadcn,double *ll_src,short direction)
{
	extern double cs_K360;

	double density;
	
	double lclSrcLl [3];

	/* The Alaska kludge again. */
	lclSrcLl [LNG] = ll_src [LNG];
	lclSrcLl [LAT] = ll_src [LAT];
	if (lclSrcLl [LNG] >= 166.0)
	{
		lclSrcLl [LNG] -= cs_K360;
	}

	/* For this file format, we don't care about the direction. */
	density = CStestNadconFile (nadcn->lngShift,lclSrcLl);
	return density;
}
int CSnadcnF2 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;
	extern double cs_Km180;
	extern double cs_K360;
	extern double cs_Sec2Deg;

	int status;
	int flag180;

	double deltaLng;
	double deltaLat;
	
	double lclSrcLl [3];

	flag180 = FALSE;
	lclSrcLl [LNG] = ll_src [LNG];
	lclSrcLl [LAT] = ll_src [LAT];
	lclSrcLl [HGT] = ll_src [HGT];
	
	/* Within the NADCON data files for Alaska, the coverage is given as
	   -194.0 to -128.0.  Internally, CS-MAP normalizes longitude to the
	   +/- 180 range.  Thus, if we see a longitude which is greater than
	   or equal to +166.0, we assume it is a longitude for Alaskan geography
	   and make the change here.  This code is particular to the NADCON
	   file format, and the 'NA' in NADCON refers to North America.  So
	   this rather simple test is considered sufficient. */
	if (lclSrcLl [LNG] >= 166.0)
	{
		flag180 = TRUE;
		lclSrcLl [LNG] -= cs_K360;
	}

	deltaLng = deltaLat = cs_Zero;
	status = CScalcNadconFile (nadcn->lngShift,&deltaLng,lclSrcLl);
	if (status == csGRIDI_ST_OK)
	{
		status = CScalcNadconFile (nadcn->latShift,&deltaLat,lclSrcLl);
	}
	
	/* Status at this point is either 0 for there is coverage and the
	   calculation is good, or -1 indicating the data file does not provide
	   coverage.  The -1 condition should only happen in normal circumstances
	   in the case where this function is being called by the iterative
	   inverse function. */
	if (status != csGRIDI_ST_OK)
	{
		deltaLng = cs_Zero;
		deltaLat = cs_Zero;
	}
	ll_trg [LNG] = lclSrcLl [LNG] - deltaLng * cs_Sec2Deg;
	ll_trg [LAT] = lclSrcLl [LAT] + deltaLat * cs_Sec2Deg;
	ll_trg [HGT] = lclSrcLl [HGT];

	/* Undo the Alaska kludge. */
	if (flag180 && (ll_trg [LNG] < cs_Km180))
	{
		ll_trg [LNG] += cs_K360;
	}
	return status;
}
int CSnadcnF3 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;
	extern double cs_Km180;
	extern double cs_K360;
	extern double cs_Sec2Deg;

	int status;
	int flag180;
	int vcStatus;

	double deltaLng;
	double deltaLat;
	double deltaHgt;

	double lclSrcLl [3];

	flag180 = FALSE;
	lclSrcLl [LNG] = ll_src [LNG];
	lclSrcLl [LAT] = ll_src [LAT];
	lclSrcLl [HGT] = ll_src [HGT];
	
	/* Within the NADCON data files for Alaska, the coverage is given as
	   -194.0 to -128.0.  Internally, CS-MAP normalizes longitude to the
	   +/- 180 range.  Thus, if we see a longitude which is greater than
	   or equal to +166.0, we assume it is a longitude for Alaskan geography
	   and make the change here.  This code is particular to the NADCON
	   file format, and the 'NA' in NADCON refers to North America.  So
	   this rather simple test is considered sufficient. */
	if (lclSrcLl [LNG] >= 166.0)
	{
		flag180 = TRUE;
		lclSrcLl [LNG] -= cs_K360;
	}

	deltaLng = deltaLat = deltaHgt = cs_Zero;
	status = CScalcNadconFile (nadcn->lngShift,&deltaLng,lclSrcLl);
	if (status == csGRIDI_ST_OK)
	{
		status = CScalcNadconFile (nadcn->latShift,&deltaLat,lclSrcLl);
	}

	/* Status at this point is either 0 for there is coverage and the
	   calculation is good, or -1 indicating the data file does not provide
	   coverage.  The -1 condition should only happen in normal circumstances
	   in the case where this function is being called by the iterative
	   inverse function. */
	if (status != csGRIDI_ST_OK)
	{
		deltaLng = cs_Zero;
		deltaLat = cs_Zero;
	}

	ll_trg [LNG] = lclSrcLl [LNG] - deltaLng * cs_Sec2Deg;
	ll_trg [LAT] = lclSrcLl [LAT] + deltaLat * cs_Sec2Deg;
	deltaHgt = cs_Zero;
	if (nadcn->type == nadconNAD27Shift && status >= csGRIDI_ST_OK)
	{
		vcStatus = CSvrtcon29To88 (&deltaHgt,ll_trg);
		if (vcStatus != 0)
		{
			deltaHgt = cs_Zero;
		}
	}
	ll_trg [HGT] = lclSrcLl [HGT] + deltaHgt;

	/* Undo the Alaska kludge. */
	if (flag180 && (ll_trg [LNG] < cs_Km180))
	{
		ll_trg [LNG] += cs_K360;
	}

	return status;
}
int CSnadcnI2 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src)
{
	extern double cs_LlNoise;

	short lng_ok;
	short lat_ok;

	int ii;
	int status;

	double guess [3];
	double epsilon [2];
	double newResult [3];

	status = -1;			/* Defensive, until we know differently. */

	epsilon [0] = epsilon [1] = nadcn->errorValue + cs_LlNoise;

	guess [LNG] = ll_src [LNG];
	guess [LAT] = ll_src [LAT];

	/* Start a loop which will iterate up to 10 times. The Canadians and
	   the Aussies max out at 4.  We would duplicate theirs, but since
	   this is an inverse, we'll do a little better than they do. */
	for (ii = 1;ii < nadcn->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lng_ok = lat_ok = TRUE;

		/* Compute the NAD83 lat/long for our current guess. */
		status = CSnadcnF2 (nadcn,newResult,guess);
		if (status != csGRIDI_ST_OK)
		{
			/* A negative status here is possible if this iterative solution
			   has wandered outside of the coverage of the grid data file. */
			break;
		}

		/* See how far we are off. */
		epsilon [LNG] = -CS_deltaLongitude (ll_src [LNG],newResult [LNG]);
		epsilon [LAT] = ll_src [LAT] - newResult [LAT];

		/* If our guess at the longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > nadcn->cnvrgValue)
		{
			lng_ok = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		if (fabs (epsilon [LAT]) > nadcn->cnvrgValue)
		{
			lat_ok = FALSE;
			guess [LAT] += epsilon [LAT];
		}

		/* If our current guess produces a newResult that is within
		   small of original, we are done. */
		if (lng_ok && lat_ok) break;
	}

	if (status == 0)
	{
		/* Here if the iterative algorithm completed normally.
	
		   If we didn't resolve in maxIteration tries, we issue a warning
		   message.  Casual reading of the NADCON code would lead one to
		   believe that they do five iterations, but four is all they really
		   do.  Since this is an inverse, and our clients expect it to produce
		   what we started with, we do maxIterations iterations, instead of the
		   four that GRIDINT does.  Thus, there is room for a slight discrepancy
		   between the two programs. */
		if (ii >= nadcn->maxIterations)
		{
			status = csGRIDI_ST_COVERAGE;
			CS_erpt (cs_NADCON_ICNT);
			if (fabs (epsilon [LNG]) > nadcn->errorValue ||
				fabs (epsilon [LAT]) > nadcn->errorValue)
			{
				status = csGRIDI_ST_SYSTEM;			/* fatal */
			}
		}
	}
	if (status == csGRIDI_ST_OK)
	{
		ll_trg [LNG] = guess [LNG];
		ll_trg [LAT] = guess [LAT];
	}
	else
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
	}
	return status;
}
int CSnadcnI3 (struct cs_Nadcn_ *nadcn,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;

	int status;
	int statusV;
	double deltaHgt;

	status = statusV = csGRIDI_ST_SYSTEM;		/* until we know otherwise */

	/* We use the NAD83 (i.e. ll_src in this case) to determine the elevation
	   shift.  Cine ll_src and ll_trg are often pointers to the same array, we
	   get the elevation shift calculation done while the NAD83 lat/longs are
	   still available.
	   
	   NOTE:  This relies on the fact thatat currently, CAnadcnI2 does not
	   modify in any weay the elevation portion of the target point.  We
	   would add some defensive code here for that, but this is a performance
	   sensitive function. */
	deltaHgt = cs_Zero;
	if (nadcn->type == nadconNAD27Shift)
	{
		/* Here we rely on the fact that CSvrtcon29To88 sets deltaHgt to zero
		   if the return status is not zero.  We would do some defensive
		   programming to avoid this reliance, but this module is performance
		   sensitive. */
		statusV = CSvrtcon29To88 (&deltaHgt,ll_src);
	}

	/* Use the 2D inverse to move the horizontal potion of the coordinate.
	   In the case of the NADCON grid file interpolation method, we have
	   no choice in the matter. */
	status = CSnadcnI2 (nadcn,ll_trg,ll_src);
	if (status == csGRIDI_ST_OK)
	{
		ll_trg [2] = ll_src [2] - deltaHgt;
	}

	/* Accumulate the status values. */
	if (status == 0 && statusV != 0)
	{
		status = csGRIDI_ST_COVERAGE;
	}
	return status;
}
int CSnadcnL  (struct cs_Nadcn_ *nadcn,int cnt,Const double pnts [][3])
{
	short ok;
	int index;
	double density;

	ok = TRUE;
	for (index = 0;index < cnt;index += 1)
	{
		/* Note: CSnadcnT ignores the direction parameter.  The assumption
		   always forward. */
		density = CSnadcnT (nadcn,pnts [index],cs_DTCDIR_FWD);
		if (fabs (density) > 1.0E-08)		/* i.e. != 0.0 */
		{
			ok = FALSE;
		}
	}
	return (ok != TRUE) ? cs_CNVRT_USFL : cs_CNVRT_OK;
}
int CSnadcnR  (struct cs_Nadcn_ *nadcn)
{
	CSreleaseNadconFile (nadcn->lngShift);
	CSreleaseNadconFile (nadcn->latShift);
	return 0;
}
int CSnadcnD  (struct cs_Nadcn_ *nadcn)
{
	if (nadcn->usingVertcon)
	{
		CSvrtconCls ();
		nadcn->usingVertcon = FALSE;
	}
	CSdeleteNadconFile (nadcn->lngShift);
	CSdeleteNadconFile (nadcn->latShift);
	CS_free (nadcn);
	return 0;
}
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
long csNadconFileBufrSize = 10240;
struct cs_NadconFile_* CSnewNadconFile (Const char* filePath,long32_t bufferSize,
															 ulong32_t flags,
															 double density)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];

	int hpgn;
	size_t readCount;
	long32_t lngTmp;

	const char *cpK;
	char *cp1, *cp2;
	csFILE *fstr;
	struct cs_NadconFile_* thisPtr;
	struct csNadconFileHdr_ nadconFileHdr;

	/* Prepare for an error. */
	thisPtr = NULL;
	fstr = NULL;

	/* Malloc and initialize */
	thisPtr = CS_malc (sizeof (struct cs_NadconFile_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	CSinitNadconFile (thisPtr);

	/* Set default values for all members. */
	thisPtr->bufferSize = bufferSize;
	if (thisPtr->bufferSize <= 0) thisPtr->bufferSize = csNadconFileBufrSize;

	/* Save file path/name. */
	CS_stncp (thisPtr->filePath,filePath,sizeof (thisPtr->filePath));
	cpK = strrchr (filePath,cs_DirsepC);
	if (cpK != NULL) cpK += 1;
	else             cpK = filePath;
	CS_stncp (thisPtr->fileName,cpK,sizeof (thisPtr->fileName));
	cp1 = strrchr (thisPtr->fileName,cs_ExtsepC);
	if (cp1 != NULL) *cp1 = '\0';

	/* Set up the type of file.  Get cp1 to point at the file name, and cp2 to
	   point at the extension.  We consider it an error if we are not given a
	   full path name.  Note, we care not about the format of the drive
	   specification (i.e. Windows or Linux).  But there must be at least one
	   directory and there must be an extension. */
	cp1 = strrchr (thisPtr->filePath,cs_DirsepC);
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
	cp2 += 1;

	/* Is it an HPGN file? */
	hpgn = !CS_stristr (cp1,cs_HPGN_TAG);
	if (!CS_stricmp (cp2,cs_NADCON_LAS))
	{
		if (hpgn) thisPtr->type = nadconTypeHarnLAS;
		else      thisPtr->type = nadconTypeNad27LAS;
	}
	else if (!CS_stricmp (cp2,cs_NADCON_LOS))
	{
		if (hpgn) thisPtr->type = nadconTypeHarnLOS;
		else      thisPtr->type = nadconTypeNad27LOS;
	}
	else if (!CS_stricmp (cp2,cs_GEOID96_TAG))
	{
		thisPtr->type = nadconTypeGeoid96;
	}
	else if (!CS_stricmp (cp2,cs_VERTCON_TAG))
	{
		thisPtr->type = nadconTypeVertCon;
	}
	else
	{
		thisPtr->type = nadconTypeUnknown;
	}

	/* Get the file information header. */
	fstr = CS_fopen (thisPtr->filePath,_STRM_BINRD);
	if (fstr == NULL)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}
	readCount = CS_fread (&nadconFileHdr,1,sizeof (nadconFileHdr),fstr);
	if (readCount != sizeof (nadconFileHdr))
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
	thisPtr->fileSize = CS_ftell (fstr);
	if (thisPtr->fileSize < 0L)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_IOERR);
		goto error;
	}

	/* This constructor doesn't need the file any more. */
	CS_fclose (fstr);
	fstr = NULL;

	/* Swap the bytes if necessary. */
	CS_bswap (&nadconFileHdr,cs_BSWP_NadconFileHdr);

	/* The extra stuff here is required as conversions of floats to doubles
	   does not always provide precise results.  To get the precise results we
	   require, we assume that the value (which is in degrees) is an intergal
	   number of seconds.

	   You can reproduce NADCON results precisely by replacing this stuff with
	   the following:

			thisPtr->deltaLng = nadconFileHdr.del_lng;
			thisPtr->deltaLat = nadconFileHdr.del_lat;

		However, you'll run into problems in the inverse, noticeable
		in Hawaii and Puerto Rico where the grid density is an awkward
		number, but especially on the island of St. George.  Note that
		the compiler that I'm using as I write, converts a "float" of
		0.0166667 (1 minute) to 0.0166699 something. */
	lngTmp = (long32_t)(((double)nadconFileHdr.del_lng * 3600.0) + 0.4);
	thisPtr->deltaLng = ((double)lngTmp / 3600.0);
	lngTmp = (long32_t)(((double)nadconFileHdr.del_lat * 3600.0) + 0.4);
	thisPtr->deltaLat = ((double)lngTmp / 3600.0);

	/* Now we can do the rest of this stuff. */
	thisPtr->coverage.southWest [LNG] = nadconFileHdr.min_lng;
	thisPtr->coverage.southWest [LAT] = nadconFileHdr.min_lat;
	thisPtr->coverage.northEast [LNG] = nadconFileHdr.min_lng + (thisPtr->deltaLng * (nadconFileHdr.ele_cnt - 1));
	thisPtr->coverage.northEast [LAT] = nadconFileHdr.min_lat + (thisPtr->deltaLat * (nadconFileHdr.rec_cnt - 1));
	thisPtr->coverage.density = (thisPtr->deltaLng < thisPtr->deltaLat) ? thisPtr->deltaLng : thisPtr->deltaLat;
	if (density != 0.0) thisPtr->coverage.density = density;
	thisPtr->elementCount = nadconFileHdr.ele_cnt;
	thisPtr->recordCount = nadconFileHdr.rec_cnt;
	thisPtr->recordSize = nadconFileHdr.ele_cnt * (int)sizeof (float) + (int)sizeof (long32_t);

	/* Verify the integrity of the file. */
	lngTmp = (thisPtr->recordCount + 1) * thisPtr->recordSize;
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
		if (thisPtr->bufferSize > (2 * thisPtr->recordSize))
		{
			/* Maximum efficiency is obtained with a buffer size whch is
			   a multiple of the record size. */
			thisPtr->bufferSize = (thisPtr->bufferSize / thisPtr->recordSize) * thisPtr->recordSize;
		}
		else
		{
			/* We require a minimum buffer size of 2 records. */
			thisPtr->bufferSize = 2 * thisPtr->recordSize;
		}
	}
	return (thisPtr);
error:
	if (fstr!= NULL)
	{
		CS_fclose (fstr);
		fstr = NULL;
	}
	/* CSdeleteNadconFile will not try to delete a null pointer. */
	CSdeleteNadconFile (thisPtr);
	return NULL;
}
void CSinitNadconFile (struct cs_NadconFile_ *thisPtr)
{
	thisPtr->type = nadconTypeNoneYet;
	CSinitCoverage (&thisPtr->coverage);
	thisPtr->elementCount = 0;
	thisPtr->recordCount = 0;
	thisPtr->recordSize = 0;
	thisPtr->deltaLng = 0.0;
	thisPtr->deltaLat = 0.0;
	thisPtr->fileSize = 0L;
	thisPtr->strm = NULL;
	thisPtr->bufferSize = 0L;
	thisPtr->bufferBeginPosition = 0L;
	thisPtr->bufferEndPosition = 0L;
	thisPtr->dataBuffer = NULL;
	thisPtr->cellIsValid = FALSE;
	CSinitGridCell (&thisPtr->currentCell);
	thisPtr->filePath [0] = '\0';
	thisPtr->fileName [0] = '\0';
}
void CSreleaseNadconFile (struct cs_NadconFile_* thisPtr)
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
			thisPtr->dataBuffer = NULL;
		}
		thisPtr->bufferBeginPosition = -1L;
		thisPtr->bufferEndPosition = -2L;

		/* Initialize the grid cell so it doesn't match anything. */
		CSinitGridCell (&thisPtr->currentCell);

		/* Close the file (if open). */
		if (thisPtr->strm != NULL)
		{
			CS_fclose (thisPtr->strm);
			thisPtr->strm = NULL;
		}
	}
	return;
}
void CSdeleteNadconFile (struct cs_NadconFile_* thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->strm != NULL) CS_fclose (thisPtr->strm);
		if (thisPtr->dataBuffer != NULL)
		{
			CS_free (thisPtr->dataBuffer);
			thisPtr->bufferBeginPosition = -1L;
			thisPtr->bufferEndPosition = -2L;
		}
		CS_free (thisPtr);
	}
}
double CStestNadconFile (struct cs_NadconFile_* thisPtr,Const double *sourceLL)
{
	double density;

	/* Returns zero if not covered, file density if covered. */
	density =  CStestCoverage (&(thisPtr->coverage),sourceLL);
	return density;
}
/*lint -e826  inappropriate cast: 'fltPtr = (float *)(chrPtr)' which we do a lot in this function. */
int CSextractNadconFile (struct cs_NadconFile_* thisPtr,Const double* sourceLL)
{
	extern double cs_LlNoise;			/* 1.0E-12 */
	extern char csErrnam [MAXPATH];

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
	float* fltPtr;

	float south [2];
	float north [2];

	if (thisPtr->cellIsValid)
	{
		if (sourceLL [LNG] >= thisPtr->currentCell.coverage.southWest [LNG] &&
			sourceLL [LAT] >= thisPtr->currentCell.coverage.southWest [LAT] &&
			sourceLL [LNG] <  thisPtr->currentCell.coverage.northEast [LNG] &&
			sourceLL [LAT] <  thisPtr->currentCell.coverage.northEast [LAT])
		{
			return csGRIDI_ST_OK;
		}
	}

	/* Compute the basic indices to the cell in the data file. Use of cs_LlNoise
	   is to force a result, such as, 0.99999999999998 to be a 1.0. */
	eleNbr = (long32_t)((sourceLL [LNG] - thisPtr->coverage.southWest [LNG] + cs_LlNoise) / thisPtr->deltaLng);
	recNbr = (long32_t)((sourceLL [LAT] - thisPtr->coverage.southWest [LAT] + cs_LlNoise) / thisPtr->deltaLat);
	if (eleNbr >= (thisPtr->elementCount - 1 ) || recNbr >= (thisPtr->recordCount - 1))
	{
		/* This is not supposed to happen.  This is a "private" function and
		   is to be called only when it is known that the provided coordinate is
		   within the coverage of the file.
		   
		   That was when originally designed.  It turns out that when
		   converting a point very close the coverage boundary, the iteratiive
		   inverse function can easily generate a point which is outside the
		   converage of the file.  Not by much, mind you, but enough to cause
		   this function to fail.  The iterative inverse function needs to be
		   informed about this.  So return a +1 status.  Note, that we haven't
		   done anything yet that requires undoing, so we can simply return. */
		CS_erpt  (cs_DTC_RNG_W);
		return csGRIDI_ST_COVERAGE;
	}

	/* Until we know differently. */
	thisPtr->cellIsValid = FALSE;

	/* Compute the latitude and longitude of the southwest corner of the grid cell. */
	thisPtr->currentCell.coverage.southWest [LNG] = thisPtr->coverage.southWest [LNG] + thisPtr->deltaLng * (double)eleNbr;
	thisPtr->currentCell.coverage.southWest [LAT] = thisPtr->coverage.southWest [LAT] + thisPtr->deltaLat * (double)recNbr;

	/* The northeast corner of the grid cell. */
	thisPtr->currentCell.coverage.northEast [LNG] = thisPtr->currentCell.coverage.southWest [LNG] + thisPtr->deltaLng;
	thisPtr->currentCell.coverage.northEast [LAT] = thisPtr->currentCell.coverage.southWest [LAT] + thisPtr->deltaLat;

	/* Transfer the grid cell size. */
	thisPtr->currentCell.deltaLng = thisPtr->deltaLng;
	thisPtr->currentCell.deltaLat = thisPtr->deltaLat;

	/* Compute the position in the file of the data of interest.  Note, a
	   header occupies the first record.  Therefore, the +1 on the record
	   number.  Each record has a 32 bit record number as the first element,
	   thus the +1 on the element number. */
	fpos = (recNbr + 1) * thisPtr->recordSize + (eleNbr + 1) * (int)sizeof (float);

	/* Adjust the beginning and the end to record boundaries.  We do I/O on
	   a record number basis; and we always require a minimum of two records. */
	fposBegin = (fpos / thisPtr->recordSize) * thisPtr->recordSize;
	fposEnd = fposBegin + thisPtr->recordSize + thisPtr->recordSize;

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
			thisPtr->bufferBeginPosition = fposBegin;
			thisPtr->bufferEndPosition = fposEnd;
			readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;

			/* In this section, lngTmp is the number of additional records
			   which can fit in the buffer. */
			lngTmp = (thisPtr->bufferSize - readCount) / thisPtr->recordSize;
			if (lngTmp > 2L)
			{
				/* Move the beginning of the read up by one half of the
				   amount of extra space in the buffer; but never past the
				   beginning of record number 1. */
				lngTmp = lngTmp / 2;
				thisPtr->bufferBeginPosition -= thisPtr->recordSize * lngTmp;
				if (thisPtr->bufferBeginPosition < thisPtr->recordSize)
				{
					thisPtr->bufferBeginPosition = thisPtr->recordSize;
				}
				readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;
			}

			lngTmp = (thisPtr->bufferSize - readCount) / thisPtr->recordSize;
			if (lngTmp > 2L)
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
				   file, we can move the beginning of the read up some more.
				   However, never more than the beginning of the first
				   data record. */
				thisPtr->bufferBeginPosition -= thisPtr->recordSize * lngTmp;
				if (thisPtr->bufferBeginPosition < thisPtr->recordSize)
				{
					thisPtr->bufferBeginPosition = thisPtr->recordSize;
				}
				readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;
			}

			lngTmp = (thisPtr->bufferSize - readCount) / thisPtr->recordSize;
			if (lngTmp > 0L)
			{
				/* In case the expanded beginning of read exceeded the beginning
				   of the file, we can move the end of the read back some more.
				   However, never more than the end of the file data record. */
				thisPtr->bufferEndPosition += thisPtr->recordSize * lngTmp;
				if (thisPtr->bufferEndPosition > thisPtr->fileSize)
				{
					thisPtr->bufferEndPosition = thisPtr->fileSize;
				}
				readCount = thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition;
			}

			/* Defensive programming.  If all this is working the way it was
			   intended, the read count and the buffer size whould be the
			   same. */
			if (readCount != thisPtr->bufferSize)
			{
				CS_stncp (csErrnam,"CS_nadcn::2",MAXPATH);
				CS_erpt (cs_ISER);
				goto error;
			}
		}

		/* OK, read in the data. */
		checkSeek = CS_fseek (thisPtr->strm,thisPtr->bufferBeginPosition,SEEK_SET);
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

		/* We have the desired data in the buffer.  Extract what we need from it.
		   If we read in the whole file, we close the stream now.  No need to
		   have the file descriptor open. */
		if (thisPtr->bufferSize == thisPtr->fileSize)
		{
			CS_fclose (thisPtr->strm);
			thisPtr->strm = NULL;
		}
	}

	/* Extract from the buffer the four floats which we need. */
	chrPtr = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);
	fltPtr = (float *)(chrPtr);
	south [0] = *fltPtr;
	south [1] = *(fltPtr + 1);
	chrPtr += thisPtr->recordSize;
	fltPtr = (float *)(chrPtr);
	north [0] = *fltPtr;
	north [1] = *(fltPtr + 1);

	/* Swap bytes as appropriate. */
	CS_bswap (south,"ff");
	CS_bswap (north,"ff");

	/* Do the calculations.  We do these here once and save the results in the
	   current cell.  Note, an ANSI 'C' compiler should do the calculation as
	   a double. */
	thisPtr->currentCell.currentAA = (double)(south [0]);
	thisPtr->currentCell.currentBB = (double)(south [1] - south [0]);
	thisPtr->currentCell.currentCC = (double)(north [0] - south [0]);
	thisPtr->currentCell.currentDD = (double)(north [1] - south [1] - north [0] + south [0]);
	strncpy (thisPtr->currentCell.sourceId,thisPtr->fileName,sizeof (thisPtr->currentCell.sourceId));

	/* If we get here, the current cell should be valid. */
	thisPtr->cellIsValid = TRUE;

	return csGRIDI_ST_OK;

error:
	/* Disable the current grid cell to indicate that it is invalid. */
	thisPtr->cellIsValid = FALSE;				/* redundant; defensive coding. */
	thisPtr->currentCell.coverage.southWest [LNG] = 1.0;
	thisPtr->currentCell.coverage.southWest [LAT] = 1.0;
	thisPtr->currentCell.coverage.northEast [LNG] = -1.0;
	thisPtr->currentCell.coverage.northEast [LAT] = -1.0;
	thisPtr->currentCell.sourceId [0] = '\0';

	/* Release the resources allocated to this object.  The next call to this
	   object would then require a refresh. */
	CSreleaseNadconFile (thisPtr);

	/* Negative return indicates a system error of sorts. */
	return csGRIDI_ST_SYSTEM;
}
/*lint +e826 */
int CScalcNadconFile (struct cs_NadconFile_* thisPtr,double* result,Const double* sourceLL)
{
	extern double cs_Mhuge;

	int cellStatus;

	/* Returns the grid data value obtained from the grid file after
	   appropriate bilinear interpolation.  Note, this value means different
	   things in different applications.  Its calculation is the same for all
	   applications. */
	cellStatus = CSextractNadconFile (thisPtr,sourceLL);
	if (cellStatus == 0)
	{
		*result = CScalcGridCell (&thisPtr->currentCell,sourceLL);
	}
	else
	{
		*result = cs_Mhuge;
	}
	return cellStatus;
}
