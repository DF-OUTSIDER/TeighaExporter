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
/*lint -esym(534,fgets)   ignoring return value */
int CSjapanQ  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;

	int err_cnt;
	int foundMeshCode;

	char *cp;
	csFILE* strm;

	char meshCode [] = "Meshcode";
	char lineBuffer [256];
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
		foundMeshCode = 0;
		while (CS_fgets (lineBuffer,sizeof (lineBuffer),strm))
		{
			if (CS_stristr(lineBuffer, meshCode))
			{
				/* Line contains "MeshCode", so we are satisfied.
				   Probably only want lines that start with MeshCode, but we
				   don't have a spec and the original implementation was similarly
				   loose.  If we tighten things up we'll also have to consider lines
				   too long to fit in lineBuffer.*/
				foundMeshCode = 1;
				break;
			}
		}

		CS_fclose (strm);
		strm = NULL;

		if (!foundMeshCode)
		{
			/* The phrase "meshcode" was not found, so we assume this is not a ".par"
			   file. */
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FORMAT;
		}
	}
	else
	{
		//the txt file didn't exist - so, check whether at least the binary file does
		cp = strrchr (pathBuffer,cs_ExtsepC);
		if (cp == NULL) 
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FILE;
		}
		else
		{
			CS_stcpy ((cp + 1),"_par");
			if (0 != CS_access(pathBuffer, 4))
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FILE;
			}
		}
	}

	return (err_cnt + 1);
}
/*lint +esym(534,fgets) */

int CSjapanS  (struct cs_GridFile_ *gridFile)
{
	int status;
	struct cs_Japan_ *japanPtr;

	japanPtr = CSnewJgd2kGridFile (gridFile->filePath,gridFile->bufferSize,gridFile->flags,
																		   gridFile->density);
	if (japanPtr != NULL)
	{
		/* OK, it initialized OK. */
		japanPtr->cnvrgValue = gridFile->cnvrgValue;
		japanPtr->errorValue = gridFile->errorValue;
		japanPtr->maxIterations = gridFile->maxIterations;
		gridFile->fileObject.JapanPar = japanPtr;

		gridFile->test = (cs_TEST_CAST)CSjapanT;
		gridFile->frwrd2D = (cs_FRWRD2D_CAST)CSjapanF2;
		gridFile->frwrd3D = (cs_FRWRD3D_CAST)CSjapanF3;
		gridFile->invrs2D = (cs_INVRS2D_CAST)CSjapanI2;
		gridFile->invrs3D = (cs_INVRS3D_CAST)CSjapanI3;
		gridFile->inRange = (cs_INRANGE_CAST)CSjapanL;
		gridFile->release = (cs_RELEASE_CAST)CSjapanR;
		gridFile->destroy = (cs_DESTROY_CAST)CSjapanD;

		status = csGRIDI_ST_OK;
	}
	else
	{
		status = csGRIDI_ST_SYSTEM;
	}
	return status;
}
double CSjapanT (struct cs_Japan_ *japan,double *ll_src,short direction)
{
	int status;
	double density;

	/* The coverage object is simply a rectangular region that enclosed
	   all covered data and does __NOT__ mean that coverage exists for all
	   real estate within the region.  We use this first as determining
	   if coverage really exists for this format is a non-trivial process. */
	density = CStestCoverage (&japan->coverage,ll_src);
	if (density != 0.0)
	{
		/* This is an expensive way to determine coverage.  However, it is
		   assumed that if your testing for coverage and it succeeds, the
		   host application will proceed to perform the transformation
		   calculation which will require all the information produced
		   and saved by the CXSextractJgd2kGridFile function.
		   
		   In the rather common case where ll_src is in the same grid cell
		   as the last point transformed, CSextactJgd2kFridFile determines
		   this quickly and instantly knows that coverage exists. */		   
		status = CSextractJgd2kGridFile (japan,ll_src);
		if (status == 0)
		{
			density = japan->ewDelta;
			if (japan->nsDelta > density) density = japan->nsDelta;
		}
	}
	return density;
}
int CSjapanF2 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src)
{
	int status;

	double lcl_ll [3];

	/* The current environment, this function should never be called
	   unless the point has produce a successful return from the CSjapanT
	   function.  Therefore we skip all bounds checking and get right to
	   the calculation.
	   
	   This file format only provides a two dimensional transformation. */
	
	status = CScalcJgd2kGridFile (japan,lcl_ll,ll_src);
	if (status == csGRIDI_ST_OK)
	{
		ll_trg [LNG] = lcl_ll [LNG];
		ll_trg [LAT] = lcl_ll [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	else if (ll_trg != ll_src)
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	return status;
}
int CSjapanF3 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src)
{
	int status;

	double lcl_ll [3];
	
	status = CScalcJgd2kGridFile (japan,lcl_ll,ll_src);
	if (status == csGRIDI_ST_OK)
	{
		ll_trg [LNG] = lcl_ll [LNG];
		ll_trg [LAT] = lcl_ll [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	else if (ll_trg != ll_src)
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	return status;
}
int CSjapanI2 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;

	short lng_ok;
	short lat_ok;
	int ii;
	int status;

	double guess [3];
	double epsilon [2];
	double newResult [3];

	status = csGRIDI_ST_OK;
	epsilon [0] = epsilon [1] = cs_Zero;		/* keep lint happy */

	guess [LNG] = ll_src [LNG];
	guess [LAT] = ll_src [LAT];
	guess [HGT] = ll_src [HGT];

	/* Start a loop which will iterate up to 10 times. The Canadians and
	   the Aussies max out at 4.  We would duplicate theirs, but since
	   this is an inverse, we'll do a little better than they do. */
	for (ii = 1;ii < japan->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lng_ok = lat_ok = TRUE;

		/* Compute the NAD83 lat/long for our current guess. */
		status = CSjapanF2 (japan,newResult,guess);
		if (status != csGRIDI_ST_OK)
		{
			/* It appears that this iterative inverse function has wandered
			   outside the coverage of the grid data file.  Especially likely
			   for this file format as the coverage is not necessarily
			   rectangular, and can have holes in the middle of it. */
			break;
		}

		/* See how far we are off. */
		epsilon [LNG] = ll_src [LNG] - newResult [LNG];
		epsilon [LAT] = ll_src [LAT] - newResult [LAT];

		/* If our guess at the longitude is off by more than 'small', we adjust
		 our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > japan->cnvrgValue)
		{
			lng_ok = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		if (fabs (epsilon [LAT]) > japan->cnvrgValue)
		{
			lat_ok = FALSE;
			guess [LAT] += epsilon [LAT];
		}

		/* If our current guess produces a newResult that is within
		   small of original, we are done. */
		if (lng_ok && lat_ok) break;
	}

	if (status == csGRIDI_ST_OK)
	{
		if (ii >= japan->maxIterations)
		{
			status = csGRIDI_ST_COVERAGE;
			CS_erpt (cs_TOKYO_ICNT);
			if (fabs (epsilon [LNG]) > japan->errorValue ||
				fabs (epsilon [LAT]) > japan->errorValue)
			{
				status = csGRIDI_ST_SYSTEM;
			}
		}
	}

	/* Adjust the Tokyo value to the computed value, now that we
	   know that it should be correct. */
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
int CSjapanI3 (struct cs_Japan_ *japan,double *ll_trg,Const double *ll_src)
{
	int status;

	double lcl_ll [3];
	
	status = CSjapanI2 (japan,lcl_ll,ll_src);
	if (status == csGRIDI_ST_OK)
	{
		ll_trg [LNG] = lcl_ll [LNG];
		ll_trg [LAT] = lcl_ll [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	else if (status > csGRIDI_ST_OK)
	{
		if (ll_trg != ll_src)
		{
			ll_trg [LNG] = ll_src [LNG];
			ll_trg [LAT] = ll_src [LAT];
			ll_trg [HGT] = ll_src [HGT];
		}
	}
	return status;
}
/* This can be an very expensive function to call for this file format. */
int CSjapanL  (struct cs_Japan_ *japan,int cnt,Const double pnts [][3])
{
	int idx;
	int status;
	double density;

	status = cs_CNVRT_OK;
	for (idx = 0;idx < cnt;idx += 1)
	{
		density = CSjapanT (japan,pnts [idx],cs_DTCDIR_FWD);
		if (fabs (density) < 1.0E-08)	/* i.e. == 0.0 */
		{
			status = cs_CNVRT_USFL;
			break;
		}
	}
	return status;
}
int CSjapanR  (struct cs_Japan_ *japan)
{
	CSreleaseJgd2kGridFile (japan);
	return 0;
}
int CSjapanD  (struct cs_Japan_ *japan)
{
	CSdeleteJgd2kGridFile (japan);
	return 0;
}
struct cs_Japan_* CSnewJgd2kGridFile (Const char *path,long32_t bufferSize,ulong32_t flags,double density)
{
	extern double cs_Sec2Deg;		/* 1.0 / 3600.0 */
	extern char cs_DirsepC;
	extern char csErrnam [];

	int st;
	size_t rdCnt;

	char *cp;
	csFILE *bStrm = NULL;
	struct cs_Japan_ *thisPtr = NULL;

	char lineBufr [MAXPATH];

	struct csJgd2kGridRecord_ minRec;
	struct csJgd2kGridRecord_ maxRec;

	/* Allocate the object structure. */
	thisPtr = CS_malc (sizeof (struct cs_Japan_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	/* Initialize. */
	CSinitCoverage (&thisPtr->coverage);
	thisPtr->ewDelta = ( 45.0 / 3600.0);
	thisPtr->nsDelta = ( 30.0 / 3600.0);
	thisPtr->coverage.density = thisPtr->ewDelta;
	//if (thisPtr->nsDelta > thisPtr->ewDelta) 
	//{
	//	thisPtr->coverage->density = thisPtr->nsDelta;
	//}

	thisPtr->strm = NULL;
	thisPtr->bufferSize = 64 * sizeof (struct csJgd2kGridRecord_);
	if (bufferSize > 0L) thisPtr->bufferSize = bufferSize;
	thisPtr->dataBuffer = NULL;
	CSinitGridCell (&thisPtr->lngCell);
	CSinitGridCell (&thisPtr->latCell);
	thisPtr->lngCell.deltaLng = thisPtr->ewDelta;
	thisPtr->lngCell.deltaLat = thisPtr->nsDelta;
	thisPtr->latCell.deltaLng = thisPtr->ewDelta;
	thisPtr->latCell.deltaLat = thisPtr->nsDelta;
	thisPtr->filePath [0] = '\0';
	thisPtr->fileName [0] = '\0';

	/* Capture the file name, etc. */
	CS_stncp (thisPtr->filePath,path,sizeof (thisPtr->filePath));
	CS_stncp (lineBufr,path,sizeof (thisPtr->filePath));
	cp = strrchr (lineBufr,cs_DirsepC);
	if (cp != NULL) CS_stncp (thisPtr->fileName,(cp + 1),sizeof (thisPtr->fileName));
	else CS_stncp (thisPtr->fileName,lineBufr,sizeof (thisPtr->fileName));

	/* Save the name for error message, audit trail purposes. */
	CS_stncp (thisPtr->lngCell.sourceId,thisPtr->fileName,sizeof (thisPtr->lngCell.sourceId));
	CS_stncp (thisPtr->latCell.sourceId,thisPtr->fileName,sizeof (thisPtr->latCell.sourceId));

	/* Make a binary file which we can use.  Note this verifies the existence
	   of the file. */
	st = CSmakeBinaryJgd2kFile (thisPtr);
	if (st != 0)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}

	/* If we're still here, we have a binary image of the named .par file.
	   We open it and extract the first two records which should be the
	   min and max of the coverage of the file. */
	bStrm = CS_fopen (thisPtr->filePath,_STRM_BINRD);
	if (bStrm == NULL)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_FL_OPEN);
		goto error;
	}
	rdCnt = CS_fread (&minRec,sizeof (minRec),1,bStrm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	rdCnt = CS_fread (&maxRec,sizeof (maxRec),1,bStrm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_fclose (bStrm);
	bStrm = NULL;

	if (minRec.meshCode != 1L || maxRec.meshCode != 2L)
	{
		CS_stncp (csErrnam,"CS_japan::1",MAXPATH);
		CS_erpt (cs_ISER);
		goto error;
	}

	/* Capture the min/max of the the file coverage in the coverage object. */
	thisPtr->coverage.southWest [LNG] = (double)(minRec.deltaLng) * cs_Sec2Deg;
	thisPtr->coverage.southWest [LAT] = (double)(minRec.deltaLat) * cs_Sec2Deg;
	thisPtr->coverage.northEast [LNG] = (double)(maxRec.deltaLng) * cs_Sec2Deg;
	thisPtr->coverage.northEast [LAT] = (double)(maxRec.deltaLat) * cs_Sec2Deg;

	/* We leave the binary file closed until such time as the host application
	   requests a conversion which actually requires access to the file.  It is
	   not uncommon for geodetic transformations to list grid interpolation
	   files in their definition which are rarely used. */
	return thisPtr;
error:
	if (bStrm != NULL)
	{
		CS_fclose (bStrm);
		bStrm = NULL;
	}
	CSdeleteJgd2kGridFile (thisPtr);
	return NULL;
}
/* The equivalent of a destructor follows. */
void CSdeleteJgd2kGridFile (struct cs_Japan_ *thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->strm != NULL)
		{
			CS_fclose (thisPtr->strm);
			thisPtr->strm = NULL;
		}
		if (thisPtr->dataBuffer != NULL)
		{
			CS_free (thisPtr->dataBuffer);
			thisPtr->dataBuffer = NULL;
		}
		CS_free (thisPtr);
	}
	return;
}
/* The following releases resources, but does not close the object.  The
   object, defining the existence of the file and its coverage remain in
   memory.  Resources necessary to complete a transformation calculation
   are re-allocated only when this object is required to actually perform
   such calculation. */
void CSreleaseJgd2kGridFile (struct cs_Japan_ *thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->strm != NULL)
		{
			CS_fclose (thisPtr->strm);
			thisPtr->strm = NULL;
		}
		if (thisPtr->dataBuffer != NULL)
		{
			CS_free (thisPtr->dataBuffer);
			thisPtr->dataBuffer = NULL;
		}
	}
	return;
}
/* Given a lat/long, we extract the grid cell which covers the point. */
int CSextractJgd2kGridFile (struct cs_Japan_ *thisPtr,Const double* sourceLL)
{
	extern char csErrnam [];
	extern double cs_Sec2Deg;

	int flag;
	size_t rdCnt;

	long32_t startFP;
	ulong32_t meshCode;
	ulong32_t iLng, iLat;

	double density;
	double swLL [2], seLL [2], neLL [2], nwLL [2];

	struct csJgd2kGridRecord_ srchKey;
	struct csJgd2kGridRecord_ swRec, seRec, neRec, nwRec;

	/* If the cuirrent grid cell objects saved within this object cover
	   the provided point, there is no need to repeat the bulk of this
	   function.  Once a point is converted, it is quite liekly that the
	   sunsequent point is in the same grid cell, so this check usually
	   saves mucho processing time. */
	if (CStestCoverage (&thisPtr->lngCell.coverage,sourceLL) != 0.0)
	{
		/* The longitude cell covers this point.  The latitude cell always
		   covers the exact same cell, no need to waste time checking it.
		   
		   Since we already have the appropriate grid cell available, we're
		   done.  */
		return csGRIDI_ST_OK;
	}

	/* Ok, the provided point is not in the same cell as the last point.  We
	   have lots of work to do; but only if the point provided is within the
	   rectangular region of coverage of the file.  While this check is usually
	   performed prior to calling this function, we repeat it again here as
	   it precludes a lot of overflow and bounds checking below.  Thus, the
	   price of the this additional check is fully paid for. */
	density = CStestCoverage (&thisPtr->coverage,sourceLL);
	if (density == 0.0)
	{
		/* Out of range.  This check eliminates the need for lots of error
		   checking below */
		return csGRIDI_ST_COVERAGE;
	}

	/* Open the binary image file if is isn't open already. */
	if (thisPtr->strm == NULL)
	{
		/* Open the binary file, since it isn't open already. */
		thisPtr->strm = CS_fopen (thisPtr->filePath,_STRM_BINRD);
		if (thisPtr->strm == NULL)
		{
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_DTC_FILE);
			return csGRIDI_ST_SYSTEM;
		} 
	}

	/* Note, we don't use a buffer (anymore, we used to).  If bufferSize
	   is not zero, we instuct the OS to use a buffer of the indicated size. */
	if (thisPtr->bufferSize > 128L /*&& thisPtr->dataBuffer == NULL*/)
	{
		setvbuf (thisPtr->strm,NULL,_IOFBF,(size_t)thisPtr->bufferSize);
	}

	/* The first two records in the binary image are the min/max of the file
	   coverage.  Thus, we instruct the binary search function to use the
	   thrid record as the front end of the binary search using the startFP
	   variable. */
	startFP = sizeof (struct csJgd2kGridRecord_) + sizeof (struct csJgd2kGridRecord_);

	/* Locate and read the four corners.  This is quite brutal, but
	   this is the only general way to determine coverage is to see
	   if all four corners of the cell exist.  This is a very specific
	   feature of the Japanese file.  This means that the data file does
	   not need to contain thousands of bogus values covering the
	   Pacific Ocean.  Unlike many other nations, a rectangular coverage
	   region does not work well for Japan.

	   The rather strange nature of this code is the result of our
	   desire to keep all information about the nature of the 'mesh'
	   in the two functions defined above.
	   
	   The sequence which appears immediately below produces the mesh code
	   of the southwest corner of the desired grid cell. */
	meshCode = CSjpnLlToMeshCode (sourceLL);
	if (meshCode == 0UL)
	{
		return csGRIDI_ST_SYSTEM;
	}
	CSjpnMeshCodeToLl (swLL,meshCode);
	iLng = (ulong32_t)CS_degToSec (swLL [0]);
	iLat = (ulong32_t)CS_degToSec (swLL [1]);

	srchKey.meshCode = meshCode;
	flag = CS_bins (thisPtr->strm,startFP,-1L,sizeof (struct csJgd2kGridRecord_),&srchKey,(CMPFUNC_CAST)CScompareJgd2kGridRecord);
	if (flag < 0) return -1;
	if (!flag) return 1;			/* mesh code does not exist in the file, thus no coverage for this point */
	rdCnt = CS_fread (&swRec,1,sizeof (swRec),thisPtr->strm);
	if (rdCnt != sizeof (swRec))
	{
		CS_erpt (cs_IOERR);
		return csGRIDI_ST_SYSTEM;
	}

	/* Now for the southeast corner of the cell. */
	iLng += 45;
	seLL [0] = (double)iLng * cs_Sec2Deg;
	seLL [1] = (double)iLat * cs_Sec2Deg;
	meshCode = CSjpnLlToMeshCode (seLL);
	if (meshCode == 0UL) return -1;
	srchKey.meshCode = meshCode;
	flag = CS_bins (thisPtr->strm,0L,-1L,sizeof (struct csJgd2kGridRecord_),&srchKey,(CMPFUNC_CAST)CScompareJgd2kGridRecord);
	if (flag < 0) return -1;
	if (!flag) return 1;			/* mesh code does not exist in the file, thus no coverage for this point */
	rdCnt = CS_fread (&seRec,1,sizeof (seRec),thisPtr->strm);
	if (rdCnt != sizeof (seRec))
	{
		CS_erpt (cs_IOERR);
		return csGRIDI_ST_SYSTEM;
	}

	/* The northeast corner of the grid cell. */
	iLat += 30;
	neLL [0] = (double)iLng * cs_Sec2Deg;
	neLL [1] = (double)iLat * cs_Sec2Deg;
	meshCode = CSjpnLlToMeshCode (neLL);
	if (meshCode == 0UL) return -1;
	srchKey.meshCode = meshCode;
	flag = CS_bins (thisPtr->strm,0L,-1L,sizeof (struct csJgd2kGridRecord_),&srchKey,(CMPFUNC_CAST)CScompareJgd2kGridRecord);
	if (flag < 0) return -1;
	if (!flag) return 1;			/* mesh code does not exist in the file, thus no coverage for this point */
	rdCnt = CS_fread (&neRec,1,sizeof (neRec),thisPtr->strm);
	if (rdCnt != sizeof (neRec))
	{
		CS_erpt (cs_IOERR);
		return csGRIDI_ST_SYSTEM;
	}

	/* Finally, the northwest corner of the grid cell. */
	iLng -= 45;
	nwLL [0] = (double)iLng * cs_Sec2Deg;
	nwLL [1] = (double)iLat * cs_Sec2Deg;
	meshCode = CSjpnLlToMeshCode (nwLL);
	if (meshCode == 0UL) return -1;
	srchKey.meshCode = meshCode;
	flag = CS_bins (thisPtr->strm,0L,-1L,sizeof (struct csJgd2kGridRecord_),&srchKey,(CMPFUNC_CAST)CScompareJgd2kGridRecord);
	if (flag < 0) return -1;
	if (!flag) return 1;			/* mesh code does not exist in the file, thus no coverage for this point */
	rdCnt = CS_fread (&nwRec,1,sizeof (nwRec),thisPtr->strm);
	if (rdCnt != sizeof (nwRec))
	{
		CS_erpt (cs_IOERR);
		return csGRIDI_ST_SYSTEM;
	}

	/* If we're still here, we have all four corners and finally, at last,
	   know that we have coverage for the provided point. */

	/* Populate the two cell structures involved.  Note that these cells have their
	   own rather general coverage structures.  Thus, on the next transformation
	   involving this object, we can quickly determine if the new point is in the same
	   cell as the last point (quite likely), and all of the above can be skipped.  */
	CSsetCoverage (&thisPtr->lngCell.coverage,swLL,neLL);
	thisPtr->lngCell.coverage.density = thisPtr->ewDelta;
	thisPtr->lngCell.currentAA = (double)swRec.deltaLng / 100000.0;
	thisPtr->lngCell.currentBB = (double)(seRec.deltaLng - swRec.deltaLng) / 100000.0;
	thisPtr->lngCell.currentCC = (double)(nwRec.deltaLng - swRec.deltaLng) / 100000.0;
	thisPtr->lngCell.currentDD = (double)(swRec.deltaLng - seRec.deltaLng - nwRec.deltaLng + neRec.deltaLng) / 100000.0;

	CSsetCoverage (&thisPtr->latCell.coverage,swLL,neLL);
	thisPtr->latCell.coverage.density = thisPtr->nsDelta;
	thisPtr->latCell.currentAA = (double)swRec.deltaLat / 100000.0;
	thisPtr->latCell.currentBB = (double)(seRec.deltaLat - swRec.deltaLat) / 100000.0;
	thisPtr->latCell.currentCC = (double)(nwRec.deltaLat - swRec.deltaLat) / 100000.0;
	thisPtr->latCell.currentDD = (double)(swRec.deltaLat - seRec.deltaLat - nwRec.deltaLat + neRec.deltaLat) / 100000.0;

	/* Identify the source of data for the grid cell. */
	CS_stncp (thisPtr->lngCell.sourceId,thisPtr->fileName,sizeof (thisPtr->lngCell.sourceId));
	CS_stncp (thisPtr->latCell.sourceId,thisPtr->fileName,sizeof (thisPtr->lngCell.sourceId));

	/* We're done, some one else does the actual calculation. */
	return csGRIDI_ST_OK;
}
/* Transform the provided point.  Again, if the provided point is outside the
   rectagular bounding box for this object, this is a very expensive way to
   find out that there is no coverage for the point. */
int CScalcJgd2kGridFile (struct cs_Japan_* thisPtr,double targetLL [2],Const double* sourceLL)
{
	extern double cs_Sec2Deg;				/* 1.0 / 3600.0 */

	double deltaLng;
	double deltaLat;

	int cellStatus;

	cellStatus = CSextractJgd2kGridFile (thisPtr,sourceLL);
	if (cellStatus == 0)
	{
		/* We have a two complete cells, we can perform the calculation. */
		deltaLng = CScalcGridCell (&thisPtr->lngCell,sourceLL);
		deltaLat = CScalcGridCell (&thisPtr->latCell,sourceLL);

		/* Apply the datum dhift values. */
		targetLL [LNG] = sourceLL [LNG] + deltaLng * cs_Sec2Deg;
		targetLL [LAT] = sourceLL [LAT] + deltaLat * cs_Sec2Deg;
	}
	else
	{
		/* We have no coverage, we return the original point unmodified.
		   The status value indicates that there was no coverage. */
		targetLL [LNG] = sourceLL [LNG];
		targetLL [LAT] = sourceLL [LAT];
	}
	return cellStatus;
}
/* This function creates a binary version of the Jgd2k file if it doesn't
   exist, or if the date on the binary file is older than or equal to
   that of the primary text file.  Note, that this function uses the path name
   in the provided object, and modifies it to point to the binary file which
   is then created if necessary.

   This function is implemented as records in the ASCII text file may not be
   of fixed length, there can be a million of them, and there is no guarantee
   that the records in the text file will be properly sorted.  The binary
   file enables random access to the data file for decent performance without
   eating up 12MB of RAM. */
int CSmakeBinaryJgd2kFile (struct cs_Japan_* thisPtr)
{
	extern char cs_ExtsepC;
	extern char csErrnam [];
	extern double cs_Zero;

	int st;
	int foundMeshcode;
	cs_Time_ aTime, bTime;

	size_t wrCnt;

	char *cp1, *cp2;
	csFILE *aStrm, *bStrm;

	long32_t tempL;
	long32_t dblFrmt;
	ulong32_t meshCode;

	double lngTmp, latTmp;
	double mesh_ll [2];

	char lineBufr [128];
	char binaryPath [MAXPATH];

	struct csJgd2kGridRecord_ gridRec;
	struct csJgd2kGridRecord_ minRec;
	struct csJgd2kGridRecord_ maxRec;

	/* We will write two records to the file with bogus mesh codes.  These
	   records will carry the minimum (i.e. southwest) longitude and latitude
	   and the maximum (i.e. northeast) longitude and latitude of the data in
	   the file.  This is an attempt to make the file format something which
	   does not inherently imply applicability to Japanese geography.
	   
	   The bogus mesh codes were devised such that the min/max will be the
	   first two records in the file, thus easily accessible.  For these two
	   special records, the deltaLng and DeltaLat fields carry absolute 
	   longitude and latitude in seconds. */
	minRec.meshCode = 1UL;
	minRec.deltaLat = (long32_t)( 90 * 3600);
	minRec.deltaLng = (long32_t)(180 * 3600);
	maxRec.meshCode = 2UL;
	maxRec.deltaLat = (long32_t)( -90 * 3600);
	maxRec.deltaLng = (long32_t)(-180 * 3600);

	/* Prepare for an error of some sort. */
	aStrm = bStrm = NULL;
	lngTmp = latTmp = cs_Zero;

	/* Manufacture the name of the binary file. */
	CS_stncp (binaryPath,thisPtr->filePath,sizeof (binaryPath));
	cp1 = strrchr (binaryPath,cs_ExtsepC);
	if (cp1 == NULL) 
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}
	CS_stcpy ((cp1 + 1),"_par");

	bTime = CS_fileModTime (binaryPath);
	/* Build a new binary file only if it doesn't exist yet. */
	if (0 == bTime)
	{
		/* Determine the last modification time for the two files.  Zero time value
		   means the file does not exist. */
		aTime = CS_fileModTime (thisPtr->filePath);
		if (aTime == 0)
		{
			/* The indicated source file does not exist, not uncommon among the
			   error conditions possible. */
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_DTC_FILE);
			goto error;
		}

		/* Here to create a, possibly new, binary version of the Jgd2k file,
		   typically named "TKY2JGD.par".  To follow the general design
		   theme, we assume there can be several files, and there is no fixed
		   name for any of the files.  At this particular point, of course,
		   we're only dealing with one file, the file whose name was provided.
		   
		   We write one csJgd2kGridRecord structure	for each line of text that
		   we read.   Of course, we need to skip the first two lines (usually)
		   in the file as they are of the mets-data variety. */
		aStrm = CS_fopen (thisPtr->filePath,_STRM_TXTRD);
		if (aStrm == NULL)
		{
			/* This could happen if the file exists, but for some reason we
			   are denied permission to read the file.*/
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_DTC_FILE);
			goto error;
		}

		/* The mode of the following open will truncate any existing file, and
		   create a new binary file if necessary. */
		bStrm = CS_fopen (binaryPath,_STRM_BINWR);
		if (bStrm == NULL)
		{
			CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
			CS_erpt (cs_FL_OPEN);
			goto error;
		}

		/* If we're still here, we can copy the file, converting it to binary
		   form as we do.  While doing so, we'll convert each mesh code to
		   latitude and longitude form and accumulate an estimate of the
		   coverage of the file. */
		foundMeshcode = 0;
		while (CS_fgets (lineBufr,sizeof (lineBufr),aStrm) != NULL)
		{
			// Skip lines till we've advanced past the header
			if (!foundMeshcode)
			{
				if (CS_stristr(lineBufr, "meshcode"))
				{
					foundMeshcode = 1;
				}
				continue;
			}

			/* Parse the information is in a record. */
			meshCode = CS_strtoul (lineBufr,&cp1,10);

			/* This method of filtering header lines was not sufficient for the
			   JGD2011 .par file.  Hopefully there will be no data after the header
			   (skipped above) that will trigger this filtering. */
			if (meshCode == 0 || meshCode == 0xffffffffUL) continue;

			/* Accumulate a bounding box (or minimum and maximum as us old
			   farts used to call it) so we have a rough estimate of the
			   coverage of the file. */
			CSjpnMeshCodeToLl (mesh_ll,meshCode);			
			if (mesh_ll [LNG] < -180.0 || mesh_ll [LNG] > 180.0 ||
				mesh_ll [LAT] <  -90.0 || mesh_ll [LAT] > 90.0)
			{
				/* Bogus mesh code.  We could consider this to be meta-data
				   that slipped through the code immediately above, or that
				   the file is invalid.  For now, we consider it to be the
				   latter:  The file is not a validly formatted data file. */
				CS_erpt (cs_INV_FILE);
				goto error;
			}
			tempL = (long32_t)(mesh_ll [LNG] * 3600 + 0.01);
			if (tempL < minRec.deltaLng) minRec.deltaLng = tempL; 
			if (tempL > maxRec.deltaLng) maxRec.deltaLng = tempL; 
			tempL = (long32_t)(mesh_ll [LAT] * 3600 + 0.01);
			if (tempL < minRec.deltaLat) minRec.deltaLat = tempL; 
			if (tempL > maxRec.deltaLat) maxRec.deltaLat = tempL; 

			/* Separate the two data values.  We avoid strtod as it is
			   locale dependent and the data file is not. */
			while (*cp1 == ' ') cp1++;
			cp2 = strchr (cp1,' ');
			if (cp2 == NULL)
			{
				goto error;
			}
			*cp2++ = '\0';
			while (*cp2 == ' ') cp2++;

			dblFrmt = CSatof (&latTmp,cp1,'.',',',':');
			if (dblFrmt >= 0)
			{
				dblFrmt = CSatof (&lngTmp,cp2,'.',',',':');
			}
			if (dblFrmt < 0)
			{
				CS_erpt (cs_INV_FILE);
				goto error;
			}

			/* Build a new record in the array. */
			gridRec.meshCode = meshCode;
			
			/* NTO 30 Oct 2008, Trac # 13
			   The following was necessiatated by Linux.  It appears that on the
			   Linux platform (or some of them, anyway), the rounding of doubles
			   (especially negative ones) is different than what you will see on
			   WIN32 platforms.  Which one is correct is of no concern to me,
			   only that they are different. */
			latTmp *= 100000.0;
			latTmp += (latTmp >= 0.0) ? 0.1 : -0.1;
			lngTmp *= 100000.0;
			lngTmp += (lngTmp >= 0.0) ? 0.1 : -0.1;
			/* End Linux fix, Trac #13 */

			gridRec.deltaLat = (long32_t)(latTmp);
			gridRec.deltaLng = (long32_t)(lngTmp);
			wrCnt = CS_fwrite (&gridRec,sizeof (gridRec),1,bStrm);
			if (wrCnt != 1)
			{
				CS_erpt (cs_IOERR);
				goto error;
			}
		}

		/* If we're still here, we write minRec and maxRec to the file so we
		   have a record of the coverage of the file. */
		wrCnt =	CS_fwrite (&minRec,sizeof (minRec),1,bStrm);
		if (wrCnt != 1)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
		wrCnt = CS_fwrite (&maxRec,sizeof (maxRec),1,bStrm);
		if (wrCnt != 1)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}

		/* Close the file streams.  We're going to open the binary file back
		   up immediately, but have found this redundant close and open to
		   produce more reliable operation cross platforms. */
		CS_fclose (aStrm);
		aStrm = NULL;
		st = CS_fclose (bStrm);
		bStrm = NULL;

		/* OK, we need to sort the file by mesh codes so we can use a binary
		   search algorithm on it.  This should also cause the min/max records
		   to end up being the first two records in the file. This is very
		   convenient.

		   Note that the position of any one record in the file with respect
		   to any other record in the file is immaterial in this file format.
		   Thus, we can reorder the file any way we want to.  The original
		   code was written in the Windows 95 era, so the current scheme may
		   look rather dumb.  Nevertheless, it works and I have no motivation
		   to fix it. */
		bStrm = CS_fopen (binaryPath,_STRM_BINUP);
		if (bStrm == NULL)
		{
			CS_stncp (csErrnam,binaryPath,MAXPATH);
			CS_erpt (cs_FL_OPEN);
			goto error;
		}
		st = CS_ips (bStrm,sizeof (gridRec),0L,(CMPFUNC_CAST)CScompareJgd2kGridRecord);
		CS_fclose (bStrm);
		bStrm = NULL;
		if (st < 0) goto error;
		if (st == 1) st = csGRIDI_ST_OK;	/* CS_ips returns 1 for success, for historical reasons. */

		/* OK, now we're really done. */
	}

	/* If all that was done successfully, we change the name of
	   the file and return success. */
	CS_stncp (thisPtr->filePath,binaryPath,sizeof (thisPtr->filePath));
	return csGRIDI_ST_OK;
error:
	if (aStrm != NULL)
	{
		CS_fclose (aStrm);
		aStrm = NULL;
	}
	if (bStrm != NULL)
	{
		CS_fclose (bStrm);
		bStrm = NULL;
	}
	return csGRIDI_ST_SYSTEM;
}
/*
	The following function returns the mesh code appropriate for the given
	geographic coordinates provided.  Note, it is expected that the calling
	module deals with the rounding to the appropriate grid point.

	This function returns a zero to indicate an error.  Zero is not, under
	any circumstances, a valid mesh code.
*/
ulong32_t EXP_LVL9 CSjpnLlToMeshCode (const double ll [2])
{
	extern char csErrnam [];

	ulong32_t mesh;
	ulong32_t iLat, iLng;

	/* Verify that we are in the proper range.  This prevents many nasty
	   situations such as overflow. */
	if (ll [0] < 120.0 || ll [0] > 160.0 || ll [1] <  20.0 || ll [1] >  60.0)
	{
		CS_stncp (csErrnam,"CS_jpnMeshCode:1",MAXPATH);
		CS_erpt (cs_ISER);
		return 0;
	}

	/* Compute the mesh numbers for each value. */
	iLat = ((ulong32_t)CS_degToSec (ll [1])) / 30;
	iLng = ((ulong32_t)CS_degToSec (ll [0] - 100.0)) / 45;

	/* We do the last two digits; these range from 0 thru 9.  Each increment in
	   longitude represents 45 seconds of longitude.  Each increment in
	   latitude represents 30 seconds of latitude. */
	mesh  =  iLng % 10;
	mesh += (iLat % 10) * 10;

	/* Next two digits, range from 0 thru 7.  Each increment of longitude represents
	   7.5 minutes (450 seconds).  Each increment of latitude represents 5 minutes
	   (300 seconds). */
	mesh += ((iLng / 10) % 8) * 100;
	mesh += ((iLat / 10) % 8) * 1000;

	/* Now for the last set of four digits.  Each increment of latitude represents
	   40 minutes (2400 sceonds) and each increment of longitude repsents 12 degree
	   (3600 seconds) of longitude.  Note that we biased the longitude by 100 degrees
	   above. */
	mesh += ((iLng / 80) % 100) *   10000;
	mesh += ((iLat / 80) % 100) * 1000000;
	return mesh;
}
/* The inverse of the above. */
void EXP_LVL9 CSjpnMeshCodeToLl (double ll [2],ulong32_t meshCode)
{
	extern double cs_Sec2Deg;		/* 1.0 / 3600.00 */

	ulong32_t iLat, iLng;		/* in seconds, lng - 100. */

	iLng = ((meshCode /        1) % 10) * 45;
	iLat = ((meshCode /       10) % 10) * 30;

	iLng += ((meshCode /     100) % 10) * 450;
	iLat += ((meshCode /    1000) % 10) * 300;

	iLng += ((meshCode /   10000) % 100) * 3600;
	iLat += ((meshCode / 1000000) % 100) * 2400;

	ll [0] = (double)iLng * cs_Sec2Deg + 100.0;
	ll [1] = (double)iLat * cs_Sec2Deg;

	return;
}
int CScompareJgd2kGridRecord (const struct csJgd2kGridRecord_ *elem1,const struct csJgd2kGridRecord_ *elem2)
{
	int rtnVal = 0;

	if (elem1->meshCode < elem2->meshCode) rtnVal = -1;
	else if (elem1->meshCode > elem2->meshCode) rtnVal = 1;
	return rtnVal;
}
