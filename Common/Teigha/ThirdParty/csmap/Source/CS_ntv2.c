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

/*				* * * * R E M A R K S * * * *

	This object encapsulates the functionality of a Canadian National
	Transformation, Version 2, grid data file.  Several other nations have
	adopted this format for use in their geodetic projects, but there are some
	minor variations.  These variations are supported by this code, the code
	senses the difference automatically.
*/

#include "cs_map.h"

/*lint -esym(613,err_list)  possible use of null pointer; but not really */

long32_t csNTv2BufrSz = 32768L;

int CScntv2Q (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz)
{
	extern char cs_DirsepC;

	int err_cnt;
	size_t rdCnt;

	char *cp;
	csFILE* strm;

	char chrBuffer [16];
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
	strm = CS_fopen (pathBuffer,_STRM_BINRD);
	if (strm != NULL)
	{
		rdCnt = CS_fread (chrBuffer,1,sizeof (chrBuffer),strm);
		CS_fclose (strm);
		strm = NULL;

		if (rdCnt != sizeof (chrBuffer) || CS_strnicmp (chrBuffer,"NUM_OREC",8))
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
int CScntv2S (struct cs_GridFile_ *gridFile)
{
	int status;

	struct cs_NTv2_ *ntv2Ptr;

	/* There is no need to allocate memory, it already exists in the
	   fileObject union in the provided cs_GridFile object.  We do need to
	   initialize this area of memory as a valid cs_NTv2_ object, however. */
	ntv2Ptr = CSnewNTv2 (gridFile->filePath,gridFile->bufferSize,gridFile->flags,gridFile->density);
	if (ntv2Ptr != NULL)
	{
		/* OK, it initialized OK. */
		ntv2Ptr->cnvrgValue = gridFile->cnvrgValue;
		ntv2Ptr->errorValue = gridFile->errorValue;
		ntv2Ptr->maxIterations = gridFile->maxIterations;
		gridFile->fileObject.NTv2 = ntv2Ptr;
		
		gridFile->test = (cs_TEST_CAST)CScntv2T;
		gridFile->frwrd2D = (cs_FRWRD2D_CAST)CScntv2F2;
		gridFile->frwrd3D = (cs_FRWRD3D_CAST)CScntv2F3;
		gridFile->invrs2D = (cs_INVRS2D_CAST)CScntv2I2;
		gridFile->invrs3D = (cs_INVRS3D_CAST)CScntv2I3;
		gridFile->inRange = (cs_INRANGE_CAST)CScntv2L;
		gridFile->release = (cs_RELEASE_CAST)CScntv2R;
		gridFile->destroy = (cs_DESTROY_CAST)CScntv2D;

		status = 0;
	}
	else
	{
		status = -1;
	}
	return status;
}
/* Returns a hard zero in the case of no coverage. */
double CScntv2T (struct cs_NTv2_ *cntv2,double ll_src [2],short direction)
{
	double density;			/* actually, grid cell size */

	/* For this file format, we don't care about the direction. */
	
	density = CStestNTv2 (cntv2,ll_src);
	return density;
}
int CScntv2F2 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src)
{
	extern double cs_Sec2Deg;

	int status;

	double deltaLL [2];
	
	ll_trg [LNG] = ll_src [LNG];
	ll_trg [LAT] = ll_src [LAT];
	
	/* CScalcNTv2 returns the datum shift, in seconds of arc.  The shift is
	   set to zero in the event of an error. */
	status = CScalcNTv2 (cntv2,deltaLL,ll_src);
	if (status == 0)
	{
		ll_trg [LNG] -= deltaLL [LNG] * cs_Sec2Deg;
		ll_trg [LAT] += deltaLL [LAT] * cs_Sec2Deg;
	}
	return status;
}
int CScntv2F3 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	ll_trg [LNG] = lcl_ll [LNG] = ll_src [LNG];
	ll_trg [LAT] = lcl_ll [LAT] = ll_src [LAT];
	ll_trg [HGT] = ll_src [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CScntv2F2 (cntv2,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		ll_trg [LNG] = lcl_ll [LNG];
		ll_trg [LAT] = lcl_ll [LAT];
	}
	return status;
}
/* The NTv2 format specifies the shift from one datum to another datum.  The
   only way to go the other way is the iterative technique used below.  As
   long as the transformation is continuous with out any sharp deviations,
   this technique works fine.  You can run into problem when close to an
   edge in the coverage, and the iterative technique can wander outside the
   coverage of the file, and thus fail to converge. */
int CScntv2I2 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src)
{
	extern double cs_LlNoise;

	short lng_ok;
	short lat_ok;
	int ii;
	int status;

	double guess [3];
	double epsilon [2];
	double newResult [3];

	epsilon [0] = epsilon [1] = cntv2->errorValue + cs_LlNoise;

	guess [LNG] = ll_src [LNG];
	guess [LAT] = ll_src [LAT];
	guess [HGT] = ll_src [HGT];

	status = csGRIDI_ST_SYSTEM;			/* until we know different */

	/* Start a loop which will iterate up to 10 times. The Canadians and
	   the Aussies max out at 4.  We would duplicate theirs, but since
	   this is an inverse, we'll do a little better than they do. */
	for (ii = 1;ii < cntv2->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lng_ok = lat_ok = TRUE;

		/* Compute the NAD83 lat/long for our current guess. */
		status = CScntv2F2 (cntv2,newResult,guess);
		if (status != csGRIDI_ST_OK)
		{
			/* In this context, this error can occur when inverting points
			   close to the coverage boundary of the file.  If we detect a
			   single instance of wandering across the boundary, we simply
			   return a coverage status and the overall grid file
			   interpolation system will use the fallback if one was
			   specified.
			   
			   If we don't do as written above, the iterative technique fails
			   to converge and we have a situation where a system error may
			   be generated and the results may not be reproducable. */
			break;
		}

		/* See how far we are off. */
		epsilon [LNG] = ll_src [LNG] - newResult [LNG];
		epsilon [LAT] = ll_src [LAT] - newResult [LAT];

		/* If our guess at the longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > cntv2->cnvrgValue)
		{
			lng_ok = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		if (fabs (epsilon [LAT]) > cntv2->cnvrgValue)
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
		/* If we didn't resolve in maxIteration tries, we issue a warning
		   message.  Casual reading of the NADCON code would lead one to
		   believe that they do five iterations, but four is all they really
		   do.  Since this is an inverse, and our clients expect it to produce
		   what we started with, we do maxIterations iterations, insteadt of the
		   four that GRIDINT does.  Thus, there is room for a slight discrepancy
		   between the two programs. */
		if (ii >= cntv2->maxIterations)
		{
			status = csGRIDI_ST_COVERAGE;
			CS_erpt (cs_NADCON_ICNT);
			
			if (fabs (epsilon [LNG]) > cntv2->errorValue ||
				fabs (epsilon [LAT]) > cntv2->errorValue)
			{
				/* Here if either residual is greater than that specified in
				   the transformation definition.  This rarely, if ever,
				   happens in normal usage.  Should it really happen, the
				   problem is indeed serious. */
				status = csGRIDI_ST_SYSTEM;			/* fatal */
			}
		}
	}

	/* Adjust the ll_27 value to the computed value, now that we
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
	return 0;
}
int CScntv2I3 (struct cs_NTv2_ *cntv2,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	ll_trg [LNG] = lcl_ll [LNG] = ll_src [LNG];
	ll_trg [LAT] = lcl_ll [LAT] = ll_src [LAT];
	ll_trg [HGT] = ll_src [HGT];

	lcl_ll [HGT] = cs_Zero;
	status = CScntv2I2 (cntv2,lcl_ll,lcl_ll);
	if (status >= 0)
	{
		ll_trg [LNG] = lcl_ll [LNG];
		ll_trg [LAT] = lcl_ll [LAT];
	}
	return status;
}
int CScntv2L  (struct cs_NTv2_ *cntv2,int cnt,Const double pnts [][3])
{
	int idx;
	int status;
	double density;

	status = cs_CNVRT_OK;
	for (idx = 0;idx < cnt;idx += 1)
	{
		density = CStestNTv2 (cntv2,pnts [idx]);
		if (fabs (density) > 1.0E-08)
		{
			status = cs_CNVRT_USFL;
			break;
		}
	}
	return status;
}
int CScntv2R  (struct cs_NTv2_ *cntv2)
{
	CSreleaseNTv2 (cntv2);
	return 0;
}
int CScntv2D  (struct cs_NTv2_ *cntv2)
{
	CSdeleteNTv2 (cntv2);
	return 0;
}

/******************************************************************************
	Constructor.  Can't have an object of this type without a data file.  Thus
	this constructor requires a data file and returns NULL in the event of a
	failure.  Currently, there is no use of the flags value.
*/
struct cs_NTv2_* CSnewNTv2 (Const char *filePath,long32_t bufferSize,ulong32_t flags,double density)
{
	int st;

	struct cs_NTv2_* thisPtr;

	/* Allocate and initialize the basic structure. */
	thisPtr = (struct cs_NTv2_ *)CS_malc (sizeof (struct cs_NTv2_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
	}
	else
	{
		st = CSinitNTv2 (thisPtr,filePath,bufferSize,flags,density);
		if (st != 0)
		{
			CSdeleteNTv2 (thisPtr);
			thisPtr = NULL;
		}
	}
	return thisPtr;
}

int CSinitNTv2 (struct cs_NTv2_* thisPtr,Const char *filePath,long32_t bufferSize,
															  ulong32_t flags,
															  double density)
{
	extern double cs_Sec2Deg;
	extern double cs_K360;
	extern char cs_DirsepC;
	extern char csErrnam [];

	short idx;
	short parIdx;

	int overlap;
	int seekStat;

	size_t readCnt;
	size_t readCntRq;
	size_t malcCnt;
	long32_t skipAmount;

	char *cp;
	struct csNTv2SubGrid_* subPtr;
	struct csNTv2SubGrid_* kidPtr;
	struct csNTv2SubGrid_* parPtr;

	union csNtv2Hdrs_ fileHdr;
	struct csNTv2SubHdr_ fileSubHdr;

	char ctemp [MAXPATH];

	csFILE* stream = NULL;

	/* Try to prevent a likely crash. */
	if (thisPtr == NULL)
	{
		CS_stncp (csErrnam,"CS_ntv2::1",MAXPATH);
		CS_erpt (cs_ISER);
		return -1;
	}

	/* In the event of an error; this eliminates duplicating this many many
	   times. */
	CS_stncp (csErrnam,filePath,MAXPATH);

	/* Initialize the structure to harmless values. */
	thisPtr->SubGridDir = NULL;
	thisPtr->fileImage = NULL;
	thisPtr->fileImageSize = 0;
	thisPtr->HdrRecCnt = 0;
	thisPtr->SubCount = 0;
	thisPtr->RecSize = 16;
	thisPtr->CellIsValid = FALSE;
	thisPtr->SubOverlap = (short)((flags & 0x01) != 0);
	thisPtr->IntType = csNTv2TypeNone;
	thisPtr->BufferSize = bufferSize;
	thisPtr->sourceId [0] = '\0';
	if (thisPtr->BufferSize <= 0) thisPtr->BufferSize = csNTv2BufrSz;
	if (thisPtr->BufferSize <= 4096) thisPtr->BufferSize = 4096;
	CSinitNTv2GridCell (&thisPtr->longitudeCell);
	CSinitNTv2GridCell (&thisPtr->latitudeCell);

	/* Deal with the file path. */
	CS_stncp (thisPtr->FilePath,filePath,sizeof (thisPtr->FilePath));

	/* Extract and save last 15 characters of the data file name. */
	cp = strrchr (thisPtr->FilePath,cs_DirsepC);
	if (cp == NULL) cp = thisPtr->FilePath;
	CS_stncp (ctemp,cp,sizeof (ctemp));
	cp = strrchr (ctemp,'.');
	if (cp != NULL) *cp = '\0';
	cp = ctemp;
	if (strlen (ctemp) > 15)
	{
		cp = ctemp + strlen (ctemp) - 15;
	}
	CS_stncp (thisPtr->FileName,cp,sizeof (thisPtr->FileName));

	/* Open the file. */
	stream = CS_fopen (thisPtr->FilePath,_STRM_BINRD);
	if (stream == NULL)
	{
		CS_erpt (cs_DTC_FILE);
		goto error;
	}
	setvbuf (stream,NULL,_IOFBF,(size_t)thisPtr->BufferSize);

	/* We've got a file.  Read the header. */
	readCnt = CS_fread (&fileHdr,1,sizeof (fileHdr),stream);
	if (CS_ferror (stream))
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	if (readCnt != sizeof (fileHdr))
	{
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Verify that this is the kind of file we know how to deal with. */
	if (strncmp (fileHdr.Canadian.titl01,"NUM_OREC",8))
	{
		/* Opps!!! Not a CaNTv2 file. */
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Determine the type/source of the file. */
	if (fileHdr.Canadian.titl02 [0] == 'N' &&
	    fileHdr.Canadian.titl02 [1] == 'U')
	{
		/* It appears that the file is Canadian. */
		thisPtr->IntType = csNTv2TypeCanada;
		skipAmount = sizeof (struct csNTv2HdrCa_);
		CS_bswap (&fileHdr.Canadian,cs_BSWP_NTv2HdrCa);
	}
	else if (fileHdr.Australian.titl02 [0] == 'N' &&
			 fileHdr.Australian.titl02 [1] == 'U')
	{
		/* It appears to be a legacy Australian file. */
		thisPtr->IntType = csNTv2TypeAustralia;
		skipAmount = sizeof (struct csNTv2HdrAu_);
		CS_bswap (&fileHdr.Australian,cs_BSWP_NTv2HdrAu);
	}
	else
	{
		/* Opps!!! Don't know what kind of file it is. */
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Reposition the input file as is appropriate due to the
	   type of file.  A little hoeky, but it should be portable. */
	seekStat = CS_fseek (stream,skipAmount,SEEK_SET);
	if (seekStat != 0)
	{
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Extract the valuable stuff. */
	if (thisPtr->IntType == csNTv2TypeCanada)
	{
		thisPtr->HdrRecCnt = fileHdr.Canadian.num_orec;
		thisPtr->SubCount = fileHdr.Canadian.num_file;
		thisPtr->SubHdrRecCnt = fileHdr.Canadian.num_srec;
	}
	else
	{
		thisPtr->HdrRecCnt = fileHdr.Australian.num_orec;
		thisPtr->SubCount = fileHdr.Australian.num_file;
		thisPtr->SubHdrRecCnt = fileHdr.Australian.num_srec;
	}
	/* The rest of the header is pretty much useless. */

	/* Now, we deal with the sub-directories.  THese are very
	   important. */
	malcCnt = sizeof (struct csNTv2SubHdr_) * (ulong32_t)thisPtr->SubCount;
	thisPtr->SubGridDir = (struct csNTv2SubGrid_ *)CS_malc (malcCnt);
	if (thisPtr->SubGridDir == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	/* Initialize (i.e. construct) each of the sub-grid items we just
	   allocated. */
	for (idx = 0;idx < thisPtr->SubCount;idx += 1)
	{
		subPtr = &thisPtr->SubGridDir [idx];

		/* Initialize to a boundary which will not match anything. */
		subPtr->SouthWest [LNG] =  cs_K360;
		subPtr->SouthWest [LAT] =  cs_K360;
		subPtr->NorthEast [LNG] = -cs_K360;
		subPtr->NorthEast [LAT] = -cs_K360;

		/* Remember, these values as extracted from the file itself are
		   WEST positive. */
		subPtr->SeReference [LNG] =  cs_K360;
		subPtr->SeReference [LAT] =  cs_K360;
		subPtr->NwReference [LNG] = -cs_K360;
		subPtr->NwReference [LAT] = -cs_K360;

		subPtr->DeltaLng  = 0.0;
		subPtr->DeltaLat  = 0.0;
		subPtr->Density   = 0.0;
		subPtr->FirstRecord = -1;
		subPtr->GridRecCnt = 0;
		subPtr->ParentIndex = -1;
		subPtr->ChildIndex = -1;
		subPtr->RowCount = 0;
		subPtr->ElementCount = 0;
		subPtr->RowSize = 0;
		subPtr->Cacheable = FALSE;
		subPtr->Name [0] = '\0';
		subPtr->Parent [0] = '\0';
	}

	/* Prepare for accumulating the east positive extrema for
	   optimum coverage evaluations. */
	thisPtr->swExtents [LNG] =  cs_K360;
	thisPtr->swExtents [LAT] =  cs_K360;
	thisPtr->neExtents [LNG] = -cs_K360;
	thisPtr->neExtents [LAT] = -cs_K360;

	/* Once for each sub-grid in the file; read in the header.  At this point,
	   we just read them in.  Later on, we peruse the array and figure out
	   who the mamas and the papas are. */
	for (idx = 0;idx < thisPtr->SubCount;idx += 1)
	{
		/* Kludge to handle the variation in format.  Doing this right
		   would require duplication of a whole bunch of code. So . . . */
		readCntRq = sizeof (fileSubHdr);
		if (thisPtr->IntType == csNTv2TypeAustralia) readCntRq -= 4;

		readCnt = CS_fread (&fileSubHdr,1,readCntRq,stream);
		if (CS_ferror (stream))
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
		if (readCnt != readCntRq)
		{
			CS_erpt (cs_INV_FILE);
			goto error;
		}
		if (strncmp (fileSubHdr.titl01,"SUB_NAME",8))
		{
			CS_erpt (cs_INV_FILE);
			goto error;
		}
		if (thisPtr->IntType == csNTv2TypeCanada)
		{
			CS_bswap (&fileSubHdr,cs_BSWP_NTv2SubHdrCA);
		}
		else
		{
			CS_bswap (&fileSubHdr,cs_BSWP_NTv2SubHdrAU);
		}

		/* Collect the useful stuff. */		
		subPtr = &thisPtr->SubGridDir [idx];

		/* Data for each sub-grid immediately follows the sub-grid header. */
		subPtr->FirstRecord = CS_ftell (stream);

		/* These boundaries are rational east positive boundaries. */
		subPtr->SouthWest [LNG] = -fileSubHdr.w_long * cs_Sec2Deg;
		subPtr->SouthWest [LAT] =  fileSubHdr.s_lat  * cs_Sec2Deg;
		subPtr->NorthEast [LNG] = -fileSubHdr.e_long * cs_Sec2Deg;
		subPtr->NorthEast [LAT] =  fileSubHdr.n_lat  * cs_Sec2Deg;

		/* These boundaries are the screwy west positive ones used in the
		   NTv2 format. */
		subPtr->SeReference [LNG] = fileSubHdr.e_long * cs_Sec2Deg;
		subPtr->SeReference [LAT] = fileSubHdr.s_lat  * cs_Sec2Deg;
		subPtr->NwReference [LNG] = fileSubHdr.w_long * cs_Sec2Deg;
		subPtr->NwReference [LAT] = fileSubHdr.n_lat  * cs_Sec2Deg;

		/* The remainder of this is pretty rational. */
		subPtr->DeltaLng  = fileSubHdr.long_inc * cs_Sec2Deg;
		subPtr->DeltaLat  = fileSubHdr.lat_inc  * cs_Sec2Deg;

		/* We do not use Density in the calculations.  It is only used to
		   select one sub-grid over another in the case of overlap.  Yes,
		   I know.  The sub-grids at the same level are not suppoded to
		   overlap; but they do.  Call it job security for you an me. */
		subPtr->Density = (subPtr->DeltaLat < subPtr->DeltaLng) ? subPtr->DeltaLat : subPtr->DeltaLng;

		/* If the user has specified a default density value, we use it. */
		if (density != 0.0)
		{
			subPtr->Density = density;
		}

		/* Save the name for reporting purposes. */
		CS_stncp (subPtr->Name,fileSubHdr.sub_name,9);
		CS_stncp (subPtr->Parent,fileSubHdr.parent,9);

		subPtr->GridRecCnt = fileSubHdr.gs_count;
		/* WEST Positive, dummy.  The extra .01 is to eliminate possible fuzz
		   in the double portion of the calculations. */
		subPtr->RowCount = (unsigned short)(((subPtr->NwReference [LAT] - subPtr->SeReference [LAT]) / subPtr->DeltaLat) + 1.01);
		subPtr->ElementCount = (unsigned short)(((subPtr->NwReference [LNG] - subPtr->SeReference [LNG]) / subPtr->DeltaLng) + 1.01);
		subPtr->RowSize = (unsigned short)(subPtr->ElementCount * thisPtr->RecSize);

		/* Certain sub grids are not cacheable.  In the Canadian file, the region
		   which is not cacheable is rather small.  We use the csCaNTv2KludgeTable
		   to handle it.  The one Austrailian sub-grid we've seen is screwed up,
		   so we disable cacheing (at least for now), for all Australian files.
		   Australian, in this context, means file in the old Australian format,
		   not necessarily data files covering Australian geography.
		   
		   In the case of the Spanish variation, parent grids overlap, and
		   therefore none of the sub-grids are cacheable.

		   NOTE: with the advent of RFC-2, the grid cache system is essentially
		   deactivated.  In the Canadian NTv2 file, there were two subgrids 
		   which did not adhere to the original standard and thus were not
		   cacheable.  As there is no cache anymore, the Cacheable element has
		   no use and should be removed from the structure.  For now, the value
		   is simply forced to FALSE in all cases.*/
		subPtr->Cacheable = FALSE;

		/* Skip over the data records in the file. */
		skipAmount = subPtr->GridRecCnt * thisPtr->RecSize;
		seekStat = CS_fseek (stream,skipAmount,SEEK_CUR);
		if (seekStat != 0)
		{
			CS_erpt (cs_INV_FILE);
			goto error;
		}

		/* Accumulate the "whole file" coverage boundaries in East Positive
		   form. */
		if (subPtr->SouthWest [LNG] < thisPtr->swExtents [LNG])
		{
			thisPtr->swExtents [LNG] = subPtr->SouthWest [LNG];
		}
		if (subPtr->SouthWest [LAT] < thisPtr->swExtents [LAT])
		{
			thisPtr->swExtents [LAT] = subPtr->SouthWest [LAT];
		}

		if (subPtr->NorthEast [LNG] > thisPtr->neExtents [LNG])
		{
			thisPtr->neExtents [LNG] = subPtr->NorthEast [LNG];
		}
		if (subPtr->NorthEast [LAT] > thisPtr->neExtents [LAT])
		{
			thisPtr->neExtents [LAT] = subPtr->NorthEast [LAT];
		}
	}

	/* Now we figure out who the mammas and the pappas are.  Note, all we have
	   to work with are parent names.  Therefore, we have to work bassackwards.

	   End result of all of this, is that each child needs to have the index
	   of its parent; and each sub-grid that has a child needs to be so marked.
	   Of course, all of this is of no value for files which do not adhere to
	   the original NTv2 standard.  Most notably, the Spanish file does not
	   adhere.  */
	for (idx =  0;idx < thisPtr->SubCount;idx += 1)
	{
		kidPtr = &thisPtr->SubGridDir [idx];
		if (CS_stricmp (kidPtr->Parent,"NONE    "))
		{
			/* Its a child, find the parent. */
			for (parIdx = 0;parIdx < thisPtr->SubCount;parIdx += 1)
			{
				parPtr = &thisPtr->SubGridDir [parIdx];
				if (!CS_stricmp (kidPtr->Parent,parPtr->Name))
				{
					/* Save the index of the parent. */
					kidPtr->ParentIndex = parIdx;

					/* Mark the parent as having a child, if not already so marked. */
					if (parPtr->ChildIndex == -1 || parPtr->ChildIndex > idx)
					{
						parPtr->ChildIndex = idx;
					}
				}
			}
		} 
	}

	/* To accomodate the Spanish (and perhaps others in the future), we check
	   the parent grids in the list of sub-grids for overlap.  If overlap 
	   exists, we turn on the SubOverlap flag.  Of course, if this flag is
	   already on, we have nothing to do.  If we did indeed turn on the
	   SubOverlap flag, we need to cruise through all the sub-grids and set the
	   Cacheable flag to false to assure that no data from this file makes it
	   to the grid cell cache. */
	if (thisPtr->SubOverlap == 0)
	{
		for (parIdx = 0;parIdx < thisPtr->SubCount && thisPtr->SubOverlap == 0;parIdx += 1)
		{
			parPtr = &thisPtr->SubGridDir [parIdx];
			/* Top level grids only, we know the children overlap. */
			if (parPtr->ParentIndex >= 0) continue;

			overlap = FALSE;
			for (idx = 0;idx < thisPtr->SubCount;idx += 1)
			{
				if (idx == parIdx) continue;
				subPtr = &thisPtr->SubGridDir [idx];
				if (subPtr->ParentIndex >= 0) continue;
				
				/* See if subPtr overlaps with parPtr. */
				overlap  = subPtr->SeReference [LNG] > parPtr->SeReference [LNG] &&
						   subPtr->SeReference [LAT] > parPtr->SeReference [LAT] &&
						   subPtr->SeReference [LNG] < parPtr->NwReference [LNG] &&
						   subPtr->SeReference [LAT] < parPtr->NwReference [LAT];
				overlap |= subPtr->NwReference [LNG] > parPtr->SeReference [LNG] &&
						   subPtr->NwReference [LAT] > parPtr->SeReference [LAT] &&
						   subPtr->NwReference [LNG] < parPtr->NwReference [LNG] &&
						   subPtr->NwReference [LAT] < parPtr->NwReference [LAT];		/*lint !e514  usual use of a boolean; looks just fine */
				if (overlap)
				{
					thisPtr->SubOverlap = TRUE;		/* for testing ease */
				}
			}
		}
	
		if (thisPtr->SubOverlap != 0)
		{
			for (idx = 0;idx < thisPtr->SubCount;idx += 1)
			{
				subPtr = &thisPtr->SubGridDir [idx];
				subPtr->Cacheable = FALSE;
			}
		}
	}

	/* OK, we should be ready to rock and roll.  We close the Stream until
	   we actually need it.  Often, we get constructed just so there is a
	   record of the coverage afforded by the file. */
	if (stream != NULL)			/*lint !e774  boolean always evalutaes to true */
	{
		CS_fclose (stream);
		stream = NULL;
	}
	csErrnam [0] = '\0';
	return 0;

error:
	if (stream != NULL)
	{
		CS_fclose (stream);
		stream = NULL;
	}
	if (thisPtr->SubGridDir != NULL)
	{
		CS_free (thisPtr->SubGridDir);
		thisPtr->SubGridDir = NULL;
	}
	thisPtr->HdrRecCnt = 0;
	thisPtr->SubCount = 0;
	thisPtr->RecSize = 16;
	thisPtr->CellIsValid = FALSE;
	thisPtr->SubOverlap = (short)((flags & 0x01) != 0);
	thisPtr->IntType = csNTv2TypeNone;
	thisPtr->BufferSize = bufferSize;
	thisPtr->sourceId [0] = '\0';
	if (thisPtr->BufferSize <= 0) thisPtr->BufferSize = csNTv2BufrSz;
	if (thisPtr->BufferSize <= 4096) thisPtr->BufferSize = 4096;
	CSinitNTv2GridCell (&thisPtr->longitudeCell);
	CSinitNTv2GridCell (&thisPtr->latitudeCell);
	return -1;
}

/* Destructor */
void CSdeleteNTv2 (struct cs_NTv2_* thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->fileImage != NULL) CS_free (thisPtr->fileImage);
		if (thisPtr->SubGridDir != NULL) CS_free (thisPtr->SubGridDir);
		CS_free (thisPtr);
	}
}

/* Release allocated resources without losing existence information.
	object can still be used. */
void CSreleaseNTv2 (struct cs_NTv2_* thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->fileImage != NULL)
		{
			CS_free(thisPtr->fileImage); thisPtr->fileImage = NULL;
		}
	}
}

/* Returns the name of the specific grid file which would be used if the
	provided point was to be converted. */
Const char* CSpathNTv2 (Const struct cs_NTv2_* thisPtr)
{
	return (Const char *)thisPtr->FilePath;
}

/* Given a geographic location, locate the sub-grid (if any) to use for the
   conversion of that location. */
struct csNTv2SubGrid_* CSlocateSubNTv2 (struct cs_NTv2_* thisPtr,Const double source [2])
{
	extern char csErrnam [MAXPATH];

	short idx;
	short parIdx;
	
	struct csNTv2SubGrid_ *subPtr;
	struct csNTv2SubGrid_ *cvtPtr;

	double bestCellSize;

	double wpLL [2];

	/* In case of an error.  This saves duplication of this many many times. */
	CS_stncp (csErrnam,thisPtr->FilePath,MAXPATH);

	/* Remember, source is East Positive.  All NTv2 files are West Positive. */
	wpLL [LNG] = -source [LNG];
	wpLL [LAT] =  source [LAT];

	/* Locate the appropriate sub-grid.  If there is none, than there is no
	   coverage.  There are two algorithms:  the original one and one invented
	   to cater to the Spaniards (and maybe some others in the future).
	   
	   In the original alghorithm, we search through the top level of parent
	   grids looking for coverage.  The top level parents are those which have
	   no parent.  If none is found, there is no coverage.  If we locate a parent
	   which provides coverage, we examine all children of that parent looking
	   for a sub-grid; and so on.
	   
	   In the Spanish algorithm, we search all grids, and choose the grid which
	   produces the smallest cell size.  This is necessary as the grids are
	   allowed to overlap in the Spanish variation. */
	cvtPtr = NULL;			/* NULL says no coverage (yet). */
	if (thisPtr->SubOverlap == 0)
	{
		/* Here if no overlap of the parent grids was detected when the
		   cs_NTv2_ object was constructed.  We use the Canadian algorithm
		   as it can be significantly faster.

		   The Canadian algorithm. We loop, only considering those
		   sub-grids whose parent index match parIdx.  On the first
		   iteration, parIdx is set so that all parent grids will
		   be examined. */
		parIdx = -1;
		for (idx = 0;idx < thisPtr->SubCount;idx += 1)
		{
			subPtr = &thisPtr->SubGridDir [idx];

			/* The following verifies that the current sub is a child of
			   the located parent.  Also, causes children to be skipped
			   until such time as we have found a parent. */
			if (subPtr->ParentIndex != parIdx) continue;

			/* Does this sub grid cover the point we are to convert?
			   Remember, we're dealing with WEST POSITIVE longitude.
			   the SeReference & NwReference values are west positive.
			   Think of this being a transformation that applies to
			   Russia, instead of Canada. */
			if (wpLL [LNG] >= subPtr->SeReference [LNG] &&
				wpLL [LAT] >= subPtr->SeReference [LAT] &&
				wpLL [LNG] <= subPtr->NwReference [LNG] &&
				wpLL [LAT] <= subPtr->NwReference [LAT])
			{
				/* If this is a sub grid and on the northern or western
				   boundary, we do not consider it a match. */
				if (subPtr->ParentIndex >= 0 &&
					(wpLL [LNG] >= subPtr->NwReference [LNG] ||
					 wpLL [LAT] >= subPtr->NwReference [LAT])
				   )
				{
					continue;
				}
				
				/* We have a match. */
				cvtPtr = subPtr;

				/* See if this grid has one or more children. */
				if (cvtPtr->ChildIndex < 0)
				{
					/* This one has no children; use cvtPtr. */
					break;
				}

				/* This guy has children. We need to see if any of these
				   children cover the point we are converting.  Need a minus
				   one here as the loop code is going to bump idx. */
				parIdx = idx;
				idx = cvtPtr->ChildIndex - 1;
			}
		}
	}			/*lint !e850   loop variable (idx) is modified within the loop body  (PC_Lint lin nbr is off by one) */
	else
	{
		/* The Spanish variation.  We search all subgrids looking for
		   coverages.  As the sub-grids are allowed to overlap, we must
		   search them all, and we select the one which produces the
		   smallest cell size as the "appropriate" one. */
		bestCellSize = 1.0E+100;
		for (idx = 0;idx < thisPtr->SubCount;idx += 1)
		{
			subPtr = &thisPtr->SubGridDir [idx];

			/* Does this sub grid cover the point we are to convert?
			Remember, we're dealing with WEST POSITIVE longitude.
			the SeReference & NwReference values are west positive.
			Think of this being a transformation that applies to Russia,
			instead of Canada. */
			if (wpLL [LNG] >= subPtr->SeReference [LNG] &&
				wpLL [LAT] >= subPtr->SeReference [LAT] &&
				wpLL [LNG] <= subPtr->NwReference [LNG] &&
				wpLL [LAT] <= subPtr->NwReference [LAT])
			{
				/* Yes it does.  Getthe cell size and see if it is batter
				   than what we have found so far. */
				if (subPtr->Density < bestCellSize)
				{
					cvtPtr = subPtr;
					bestCellSize = subPtr->Density;
				}
			}
		}
		/* cvtPtr should still be null if no coverage was found, we rely on this. */
	}
	return cvtPtr;
}

/* Interpolation Calculator
	The comment below is stale now that we store the entire grid file in memory,
	but is interesting nonetheless as it documents unusual properties of NTv2
	grids.

	====

	Due to a bust in the file format, we do not buffer up grid cells and stuff.
	There are a couple of sub-grids which overlap other grids in such a way that
	buffering can cause errors.  So, at least until (if ever) the data file is
	corrected, we do no buffering of the grid cells.

	Also, this file format is being adopted by others, such as the Australians.
	We don't know what they are going to do.  So to be safe, NO BUFFERING OF
	GRID CELLS.

	Also, due to the sub-grid nature of the data file, we do not buffer the
	data file in any special way; we simply use normal stream buffering.  We
	do, however, use a normal stream buffer of the size specified in the
	main object.
*/
/*lint -esym(550,readCnt)   not accessed, but very helpful when debugging */
int CScalcNTv2 (struct cs_NTv2_* thisPtr,double deltaLL [2],Const double source [2])
{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_LlNoise;			/* 1.0E-12 */
	extern char csErrnam [MAXPATH];

	short onLimit;
	unsigned short eleNbr, rowNbr;

	int rtnValue;
	int swapping;

	csFILE* stream = NULL;
	size_t readCnt;
	long32_t filePosition;
	struct csNTv2SubGrid_ *cvtPtr;

	double wpLL [2];
	double seCell [2];
	double nwCell [2];

	struct TcsCaNTv2Data southEast;
	struct TcsCaNTv2Data southWest;
	struct TcsCaNTv2Data northEast;
	struct TcsCaNTv2Data northWest;

	/* Until we know differently. */
	rtnValue = csGRIDI_ST_SYSTEM;
	thisPtr->CellIsValid = FALSE;

	/* In case of an error.  This saves duplication of this many many times. */
	CS_stncp (csErrnam,thisPtr->FilePath,MAXPATH);

	/* Remember, source is East Positive.  All NTv2 files are West Positive. */

	/* Locate the appropriate sub-grid.  If there is none, than there is no
	   coverage.  There are two algorithms:  the original one and one invented
	   to cater to the Spaniards (and maybe some others in the future).
	   
	   In the original algorithm, we search through the top level of parent
	   grids looking for coverage.  The top level parents are those which have
	   no parent.  If none is found, there is no coverage.  If we locate a parent
	   which provides coverage, we examine all children of that parent looking
	   for a sub-grid; and so on.
	   
	   In the Spanish algorithm, we search all grids, and choose the grid which
	   produces the smallest cell size.  This is necessary as the grids are
	   allowed to overlap in the Spanish variation. */
	cvtPtr = CSlocateSubNTv2 (thisPtr,source);
	
	/* OK, if cvtPtr is not NULL, its a pointer to the appropriate sub grid
	   for this conversion. */
	if (cvtPtr != NULL)
	{
		/* NTv2 files consider west longitude to be positive. */
		wpLL [LNG] = -source [LNG];
		wpLL [LAT] =  source [LAT];

		/* Determine the status of onLimit.  This indicates if the point to be
		   converted actually resides on the northern or western edge of the
		   grid cell. */
		onLimit = 0;
		if (fabs (wpLL [LAT] - cvtPtr->NwReference [LAT]) <= cs_LlNoise) onLimit |= 1;
		if (fabs (wpLL [LNG] - cvtPtr->NwReference [LNG]) <= cs_LlNoise) onLimit |= 2;

		if (thisPtr->fileImage == NULL)
		{
			stream = CS_fopen (thisPtr->FilePath,_STRM_BINRD);
			if (stream == NULL)
			{
				CS_stncp (csErrnam,thisPtr->FilePath,MAXPATH);
				CS_erpt (cs_DTC_FILE);
				goto error;
			}
			setvbuf (stream,NULL,_IOFBF,(size_t)thisPtr->BufferSize);

			// Determine the size of the file.
			if (CS_fseek (stream,0L,SEEK_END))
			{
				CS_stncp (csErrnam,thisPtr->FilePath,MAXPATH);
				CS_erpt (cs_IOERR);
				goto error;
			}
			thisPtr->fileImageSize = CS_ftell (stream);
			if (thisPtr->fileImageSize < 0L)
			{
				CS_stncp (csErrnam,thisPtr->FilePath,MAXPATH);
				CS_erpt (cs_IOERR);
				goto error;
			}
			if (CS_fseek (stream, 0L, SEEK_SET))
			{
				CS_stncp (csErrnam,thisPtr->FilePath,MAXPATH);
				CS_erpt (cs_IOERR);
				goto error;
			}

			// Prepare memory
			thisPtr->fileImage = (char*)CS_malc((size_t)thisPtr->fileImageSize);
			if (thisPtr->fileImage == NULL)
			{
				CS_erpt (cs_NO_MEM);
				goto error;
			}

			// Copy everything into the memory
			readCnt = CS_fread(thisPtr->fileImage,1,(unsigned int)thisPtr->fileImageSize,stream);
			if (CS_ferror(stream))
			{
				CS_erpt (cs_IOERR);
				goto error;
			}

			CS_fclose (stream); stream = NULL;
		}

		/* Compute onLimit for this point and the selected sub-grid regardless
		   of how we got here.  This should now only occur at the extreme edges
		   of the entire file coverage. */
		onLimit = 0;
		if (fabs (wpLL [LAT] - cvtPtr->NwReference [LAT]) <= cs_LlNoise) onLimit |= 1;
		if (fabs (wpLL [LNG] - cvtPtr->NwReference [LNG]) <= cs_LlNoise) onLimit |= 2;

		/* Compute the elements required for the file access.  This is common to
		   all cases of "onLimit". */
		eleNbr = (unsigned short)(((wpLL [LNG] - cvtPtr->SeReference [LNG]) / cvtPtr->DeltaLng) + cs_LlNoise);
		rowNbr = (unsigned short)(((wpLL [LAT] - cvtPtr->SeReference [LAT]) / cvtPtr->DeltaLat) + cs_LlNoise);

		/* Compute the boundaries of the specific cell we dealing with, assuming
		   onLimit is zero (which is the case 99.999% of the time). */
		seCell [LNG] = cvtPtr->SeReference [LNG] + cvtPtr->DeltaLng * (double)eleNbr;
		seCell [LAT] = cvtPtr->SeReference [LAT] + cvtPtr->DeltaLat * (double)rowNbr;
		nwCell [LNG] = seCell [LNG] + cvtPtr->DeltaLng;
		nwCell [LAT] = seCell [LAT] + cvtPtr->DeltaLng;

		/* Build the extent portions of the grid cells. */
		thisPtr->longitudeCell.seCorner [LNG] = seCell [LNG];
		thisPtr->longitudeCell.seCorner [LAT] = seCell [LAT];
		thisPtr->longitudeCell.nwCorner [LNG] = nwCell [LNG];
		thisPtr->longitudeCell.nwCorner [LAT] = nwCell [LAT];
		thisPtr->longitudeCell.deltaLng = cvtPtr->DeltaLng;
		thisPtr->longitudeCell.deltaLat = cvtPtr->DeltaLat;
		thisPtr->longitudeCell.density = cvtPtr->Density;

		thisPtr->latitudeCell.seCorner [LNG] = seCell [LNG];
		thisPtr->latitudeCell.seCorner [LAT] = seCell [LAT];
		thisPtr->latitudeCell.nwCorner [LNG] = nwCell [LNG];
		thisPtr->latitudeCell.nwCorner [LAT] = nwCell [LAT];
		thisPtr->latitudeCell.deltaLng = cvtPtr->DeltaLng;
		thisPtr->latitudeCell.deltaLat = cvtPtr->DeltaLat;
		thisPtr->latitudeCell.density = cvtPtr->Density;

		/* We could reduce the code complexity here by getting smart with the
		   onLimit thing.  However, this gets very tricky.  My excuse here is
		   that what is code below emulates the way the Canadians did it in
		   FORTRAN as best we can do in C. */
		if (onLimit == 0)
		{
			/* The normal case, probably about 99.9999 percent of the time.
			   Read the data into my record buffer. */
			filePosition = cvtPtr->FirstRecord + rowNbr * cvtPtr->RowSize + eleNbr * thisPtr->RecSize;

			if ((long32_t)(filePosition + sizeof(southEast) + sizeof(southWest)) > thisPtr->fileImageSize)
			{
				CS_erpt (cs_INV_FILE);
				goto error;
			}

			memcpy(&southEast, thisPtr->fileImage + filePosition, sizeof(southEast));
			/* Read southwest shifts. */
			memcpy(&southWest,
				   thisPtr->fileImage + filePosition + sizeof(southEast),
				   sizeof(southWest));

			/* Read northeast shifts. */
			filePosition += cvtPtr->RowSize;

			if ((long32_t)(filePosition + sizeof(northEast) + sizeof(northWest)) > thisPtr->fileImageSize)
			{
				CS_erpt (cs_INV_FILE);
				goto error;
			}

			memcpy(&northEast, thisPtr->fileImage + filePosition, sizeof(northEast));

			/* Read northwest shifts. */
			memcpy(&northWest,
				   thisPtr->fileImage + filePosition + sizeof(northEast),
				   sizeof(northWest));

			/* Swap as necessary. */
			swapping = CS_bswap (&southEast,cs_BSWP_NTv2Data);
			if (swapping)
			{
				CS_bswap (&southWest,cs_BSWP_NTv2Data);
				CS_bswap (&northEast,cs_BSWP_NTv2Data);
				CS_bswap (&northWest,cs_BSWP_NTv2Data);
			}

			/* Build the grid cell AA, BB, CC, and DD values. */
			thisPtr->longitudeCell.currentAA = southEast.del_lng;
			thisPtr->longitudeCell.currentBB = southWest.del_lng - southEast.del_lng;
			thisPtr->longitudeCell.currentCC = northEast.del_lng - southEast.del_lng;
			thisPtr->longitudeCell.currentDD = northWest.del_lng - southWest.del_lng - northEast.del_lng + southEast.del_lng;

			thisPtr->latitudeCell.currentAA = southEast.del_lat;
			thisPtr->latitudeCell.currentBB = southWest.del_lat - southEast.del_lat;
			thisPtr->latitudeCell.currentCC = northEast.del_lat - southEast.del_lat;
			thisPtr->latitudeCell.currentDD = northWest.del_lat - southWest.del_lat - northEast.del_lat + southEast.del_lat;
		}
		else if (onLimit == 1)
		{
			/* Point is on the extreme northern edge of the sub-grid.  This occurs
			   ocassionally.  In this case, the "northern" boundary of the grid cell
			   doesn't exist, and we must manufacture such.  This is called a
			   virtual cell in the Canadian documentation.  */
			filePosition = cvtPtr->FirstRecord + rowNbr * cvtPtr->RowSize + eleNbr * thisPtr->RecSize;

			if ((long32_t)(filePosition + sizeof(southEast) + sizeof(southWest)) > thisPtr->fileImageSize)
			{
				CS_erpt (cs_INV_FILE);
				goto error;
			}

			memcpy(&southEast, thisPtr->fileImage + filePosition, sizeof(southEast));
			/* Read southwest shifts. */
			memcpy(&southWest,
				   thisPtr->fileImage + filePosition + sizeof(southEast),
				   sizeof(southWest));

			/* Swap as necessary. */
			swapping = CS_bswap (&southEast,cs_BSWP_NTv2Data);
			if (swapping)
			{
				CS_bswap (&southWest,cs_BSWP_NTv2Data);
			}

			/* Do not attempt to read the northern boundary, it ain't there.
			   Compute the AA, BB, CC, DD values. */
			thisPtr->longitudeCell.currentAA = southEast.del_lng;
			thisPtr->longitudeCell.currentBB = southWest.del_lng - southEast.del_lng;
			thisPtr->longitudeCell.currentCC = cs_Zero;
			thisPtr->longitudeCell.currentDD = cs_Zero;

			thisPtr->latitudeCell.currentAA = southEast.del_lat;
			thisPtr->latitudeCell.currentBB = southWest.del_lat - southEast.del_lat;
			thisPtr->latitudeCell.currentCC = cs_Zero;
			thisPtr->latitudeCell.currentDD = cs_Zero;
			
			/* Adjust the grid cell boundaries to indicate that the northern
			   limits are the same as the southern limits.  I.e. a grid cell
			   that has zero height. */
			thisPtr->longitudeCell.nwCorner [LAT] = thisPtr->longitudeCell.seCorner [LAT] + cs_LlNoise;
			thisPtr->latitudeCell.nwCorner [LAT]  = thisPtr->latitudeCell.seCorner [LAT] + cs_LlNoise;
		}
		else if (onLimit == 2)
		{
			/* Point is on the extreme western edge of the sub-grid. */
			filePosition = cvtPtr->FirstRecord + rowNbr * cvtPtr->RowSize + eleNbr * thisPtr->RecSize;

			if ((long32_t)(filePosition + sizeof(southEast)) > thisPtr->fileImageSize)
			{
				CS_erpt (cs_INV_FILE);
				goto error;
			}

			memcpy(&southEast, thisPtr->fileImage + filePosition, sizeof(southEast));

			/* Don't read the south west, it ain't there. */

			filePosition += cvtPtr->RowSize;

			if ((long32_t)(filePosition + sizeof(northEast)) > thisPtr->fileImageSize)
			{
				CS_erpt (cs_INV_FILE);
				goto error;
			}

			memcpy(&northEast, thisPtr->fileImage + filePosition, sizeof(northEast));
			
			/* Don't read the northwest, it ain't there. */
			swapping = CS_bswap (&southEast,cs_BSWP_NTv2Data);
			if (swapping)
			{
				CS_bswap (&northEast,cs_BSWP_NTv2Data);
			}

			thisPtr->longitudeCell.currentAA = southEast.del_lng;
			thisPtr->longitudeCell.currentBB = cs_Zero;
			thisPtr->longitudeCell.currentCC = northEast.del_lng - southEast.del_lng;
			thisPtr->longitudeCell.currentDD = cs_Zero;

			thisPtr->latitudeCell.currentAA = southEast.del_lat;
			thisPtr->latitudeCell.currentBB = cs_Zero;
			thisPtr->latitudeCell.currentCC = northEast.del_lat - southEast.del_lat;
			thisPtr->latitudeCell.currentDD = cs_Zero;

			/* Adjust the grid cell boundaries to indicate that the eastern
			   limits are the same as the western limits.  I.e. a grid cell
			   that has zero width. */
			thisPtr->longitudeCell.nwCorner [LNG] = thisPtr->longitudeCell.seCorner [LNG] + cs_LlNoise;
			thisPtr->latitudeCell.nwCorner [LNG]  = thisPtr->latitudeCell.seCorner [LNG] + cs_LlNoise;
		}
		else  /* onLimit == 3 */
		{
			/* Point is actually the northwestern corner of the sub-grid. */
			filePosition = cvtPtr->FirstRecord + rowNbr * cvtPtr->RowSize + eleNbr * thisPtr->RecSize;

			if ((long32_t)(filePosition + sizeof(southEast)) > thisPtr->fileImageSize)
			{
				CS_erpt (cs_INV_FILE);
				goto error;
			}

			memcpy(&southEast, thisPtr->fileImage + filePosition, sizeof(southEast));

			/* Don't read anything else.  There's nothing there. */
			CS_bswap (&southEast,cs_BSWP_NTv2Data);

			/* Compute the AA, BB, CC, DD values. */
			thisPtr->longitudeCell.currentAA = southEast.del_lng;
			thisPtr->longitudeCell.currentBB = cs_Zero;
			thisPtr->longitudeCell.currentCC = cs_Zero;
			thisPtr->longitudeCell.currentDD = cs_Zero;

			thisPtr->latitudeCell.currentAA = southEast.del_lat;
			thisPtr->latitudeCell.currentBB = cs_Zero;
			thisPtr->latitudeCell.currentCC = cs_Zero;
			thisPtr->latitudeCell.currentDD = cs_Zero;

			/* Adjust the grid cell boundaries to indicate that the northeastern
			   limits are the same as the southwestern limits.  I.e. a grid cell
			   that has zero width and zero height. */
			thisPtr->longitudeCell.nwCorner [LNG] = thisPtr->longitudeCell.seCorner [LNG] + cs_LlNoise;
			thisPtr->latitudeCell.nwCorner [LNG]  = thisPtr->latitudeCell.seCorner [LNG] + cs_LlNoise;
			thisPtr->longitudeCell.nwCorner [LAT] = thisPtr->longitudeCell.seCorner [LAT] + cs_LlNoise;
			thisPtr->latitudeCell.nwCorner [LAT]  = thisPtr->latitudeCell.seCorner [LAT] + cs_LlNoise;
		}

		/* The cells are now valid, maybe.  We now work around a bust in the
		   Canadian NTV2_0.gsb grid data file. */
		thisPtr->CellIsValid = TRUE;

		/* Perform the interpolation calculation. */
		deltaLL [LNG] = CScalcNTv2GridCell (&thisPtr->longitudeCell,source);
		deltaLL [LAT] = CScalcNTv2GridCell (&thisPtr->latitudeCell,source);
		rtnValue = csGRIDI_ST_OK;
	}
	else
	{
		/* We didn't find a sub-grid.  The return value is +1 to indicate no
		   coverage. */
		deltaLL [LNG] = cs_Zero;
		deltaLL [LAT] = cs_Zero;
		rtnValue = csGRIDI_ST_COVERAGE;
	}
	csErrnam [0] = '\0';
	return rtnValue;
error:
	if (stream != NULL)
	{
		CS_fclose (stream); stream = NULL;
	}
	return csGRIDI_ST_SYSTEM;
}
/*lint +esym(550,readCnt) */

/* Test function, used to determine if this object covers the provided point.
   If so, the "grid density" of the conversion is returned as a means of
   selecting one grid object over another. */
double CStestNTv2 (Const struct cs_NTv2_* thisPtr,Const double location [2])
{
	extern double cs_Zero;

	double density = cs_Zero;

	struct csNTv2SubGrid_* cvtPtr;

	/* Note, the whole file extents which we use here carry East Positive
	   longitude; expressly to help make this function perfrom well.

	   The strange way this is coded is that testing strongly indicated that
	   the ">=" operator applied to doubles is much more CPU time consuming
	   that a simple '<' operator.  Thus, the more complex boolean expression
	   than one might ordinarily expect. */
	if (!(location [LAT] < thisPtr->swExtents [LAT] ||
		  location [LAT] > thisPtr->neExtents [LAT] ||
		  location [LNG] < thisPtr->swExtents [LNG] ||
		  location [LNG] > thisPtr->neExtents [LNG]))
	{
		/* Locate the sub-grid which covers the provided location, if any. */
		cvtPtr = CSlocateSubNTv2 (thisPtr,location);
		if (cvtPtr != NULL)
		{
			density = cvtPtr->Density;
		}
	}
	/* Return the grid density of the sub-grid covering this point. If no
	   coverage, return zero. */
	return density;
}
/*
	The following returns a pointer to the file which would be used to
	do the calculation.  At this point, the file is already selected.
	What we do here is to add the name of the sub-grid to the name
	of the file.
*/
Const char *CSsourceNTv2 (struct cs_NTv2_* thisPtr,Const double llSource [2])
{
	char *cp1;
	Const char *cp;
	struct csNTv2SubGrid_ *cvtPtr;

	cp = NULL;		/* Until we know different. */

	/* Locate the sub-grid which covers the provided location, if any. */
	cvtPtr = CSlocateSubNTv2 (thisPtr,llSource);

	/* Return the file name and the sub-grid name of the sub-grid
	   which covers this point. */
	if (cvtPtr != NULL)
	{
		/* This file does cover the point, and we have located the
		   sub-grid. */
		cp1 = CS_stncp (thisPtr->sourceId,thisPtr->FileName,16);
		*cp1++ = ':';
		*cp1++ = ':';
		CS_stncp (cp1,cvtPtr->Name,16);
		cp = thisPtr->sourceId;
	}
	return cp;
}

void CSinitNTv2GridCell (struct csNTv2GridCell_* thisPtr)
{
	extern double cs_Zero;					/* 0.0 */
	extern double cs_Mone;					/* -1.0 */

	thisPtr->seCorner [LNG] = cs_Zero;
	thisPtr->seCorner [LAT] = cs_Zero;
	thisPtr->nwCorner [LNG] = cs_Mone;
	thisPtr->nwCorner [LNG] = cs_Mone;
	thisPtr->deltaLng = cs_Zero;
	thisPtr->deltaLat = cs_Zero;
	thisPtr->density = cs_Zero;
	
	thisPtr->currentAA = cs_Zero;
	thisPtr->currentBB = cs_Zero;
	thisPtr->currentCC = cs_Zero;
	thisPtr->currentDD = cs_Zero;
	thisPtr->sourceId [0] = '\0';
}
double CScalcNTv2GridCell (Const struct csNTv2GridCell_* thisPtr,Const double sourceLL [2])
{
	double myLng, myLat;
	double gridDeltaLng;
	double gridDeltaLat;
	double returnValue;

	/* This function performs the interpolation of the grid cell, and returns
	   the result.  The algorithm used is the bilinear grid interpolation
	   algorithm used in  most all grid interpolation techniques.

	   The NTv2 format uses positive west longitudes.  Thus the reference
	   corner of the grid cell is the southeast corner of the grid cell. */
	myLng = -sourceLL [LNG];
	myLat =  sourceLL [LAT];
	gridDeltaLng  = (myLng - thisPtr->seCorner [LNG]) / thisPtr->deltaLng;
	gridDeltaLat  = (myLat - thisPtr->seCorner [LAT]) / thisPtr->deltaLat;
	returnValue = thisPtr->currentAA + 
				  thisPtr->currentBB * gridDeltaLng +
				  thisPtr->currentCC * gridDeltaLat +
				  thisPtr->currentDD * gridDeltaLng * gridDeltaLat;

	/* Note return value for datum shift grid files is usually in seconds.
	   However, this varies from application to application.  The units of
	   value calculated by this function is determined by the file format;
	   i.e. the units of the data values given for the cell corners.  For the
	   NTv2 format, these values are always (so far, at least) in seconds of
	   arc. */
	return returnValue;	
}
