/*
 * Copyright (c) 2013, Autodesk, Inc.
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

/*lint -esym(613,err_list)  possible use of a null pointer; but not really. */

long32_t csGeocnBufrSize = 434176;			/* This buffer size ensures that for
											   any typical Geocon file, dsla.b
											   for example, the buffer will
											   contain sufficient data to cover
											   a complete dgree of latitude. */

int CSgeocnQ  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz)
{
	extern char cs_DirsepC;
	extern int cs_Errno;

	int status;
	int err_cnt;

	char *cp;

	char pathBuffer    [MAXPATH];
	char pathBufferLng [MAXPATH];
	char pathBufferLat [MAXPATH];
	char pathBufferHgt [MAXPATH];

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

	/* We will end up returning (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	status = CSpathsGeoconFile (pathBufferLng,pathBufferLat,pathBufferHgt,pathBuffer);
	if (status < 0)
	{
		/* CSpathsGeoconFile returns zero if the format of pathBuffer
		   yields paths to all three files, the files exist, and are
		   readable. */
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_Errno;
	}

	/* At this point, we could open each of the files to verify the
	   correct format.  In the normal course of events, this does not
	   provide much benefit given the expense of opening, and then
	   immediately closing, each of the three files.  In the typical
	   scenario, we'll be doing that anyway as this Q function is usually
	   called immediately before the setup function is called. */

	return (err_cnt + 1);
}

/* Given a string which defines a geocon file set (typically three files) per
   the conventions established in CSpathsGeoconFile, this module constructs
   a complete transformation object be the CS-MAP csGridFile envirnment. */
int CSgeocnS (struct cs_GridFile_ *gridFile)
{
	int status;

	struct cs_Geocn_ *geocnPtr = NULL;

	char pathBuffer    [MAXPATH];
	char pathBufferLng [MAXPATH];
	char pathBufferLat [MAXPATH];
	char pathBufferHgt [MAXPATH];

	geocnPtr = (struct cs_Geocn_ *)CS_malc (sizeof (struct cs_Geocn_));
	if (geocnPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	geocnPtr->lngShift = NULL;
	geocnPtr->latShift = NULL;
	geocnPtr->hgtShift = NULL;

	CS_stncp (pathBuffer,gridFile->filePath,sizeof (pathBuffer));
	status = CSpathsGeoconFile (pathBufferLng,pathBufferLat,pathBufferHgt,pathBuffer);
	if (status != 0)
	{
		/* CSpathsGeoconFile will have ready reported the nature of the
		   error (i.e. the first one encounterd) to CS_erpt ().  We have
		   no need to do that again. */
		goto error;
	}
	
	/* Do the 'delo' file (longitude) */
	geocnPtr->lngShift = CSnewGeoconFile (pathBufferLng,gridFile->bufferSize,
														gridFile->flags,
														gridFile->density,
														geocnTypeLngShift);
	if (geocnPtr->lngShift == NULL) goto error;

	/* Do the 'dela' file (latitude) */
	geocnPtr->latShift = CSnewGeoconFile (pathBufferLat,gridFile->bufferSize,
														gridFile->flags,
														gridFile->density,
														geocnTypeLatShift);
	if (geocnPtr->latShift == NULL) goto error;

	/* Do the 'dev' file (vertical, e.g. height) */
	geocnPtr->hgtShift = CSnewGeoconFile (pathBufferHgt,gridFile->bufferSize,
														gridFile->flags,
														gridFile->density,
														geocnTypeHgtShift);
	if (geocnPtr->hgtShift == NULL) goto error;

	/* Verify that the three files are consistent. */
	if (!CS_cmpDbls (geocnPtr->lngShift->latMin,geocnPtr->latShift->latMin) ||
		!CS_cmpDbls (geocnPtr->lngShift->lngMin,geocnPtr->latShift->lngMin) ||
		!CS_cmpDbls (geocnPtr->lngShift->deltaLng,geocnPtr->latShift->deltaLng) ||
		!CS_cmpDbls (geocnPtr->lngShift->deltaLat,geocnPtr->latShift->deltaLat) ||
		geocnPtr->lngShift->elementCount != geocnPtr->latShift->elementCount ||
		geocnPtr->lngShift->recordCount  != geocnPtr->latShift->recordCount)
	{
		CS_erpt (cs_NADCON_CONS);
		goto error;
	}
	if (!CS_cmpDbls (geocnPtr->lngShift->latMin,geocnPtr->hgtShift->latMin) ||
		!CS_cmpDbls (geocnPtr->lngShift->lngMin,geocnPtr->hgtShift->lngMin) ||
		!CS_cmpDbls (geocnPtr->lngShift->deltaLng,geocnPtr->hgtShift->deltaLng) ||
		!CS_cmpDbls (geocnPtr->lngShift->deltaLat,geocnPtr->hgtShift->deltaLat) ||
		geocnPtr->lngShift->elementCount != geocnPtr->hgtShift->elementCount ||
		geocnPtr->lngShift->recordCount  != geocnPtr->hgtShift->recordCount)
	{
		CS_erpt (cs_NADCON_CONS);
		goto error;
	}

	/* OK.  If we are still here all is well.  Return the results of our
	   setup in the provided cs_GridFile_ structure. */
	geocnPtr->cnvrgValue = gridFile->cnvrgValue;
	geocnPtr->errorValue = gridFile->errorValue;
	geocnPtr->maxIterations = gridFile->maxIterations;
	gridFile->fileObject.Geocon = geocnPtr;

	gridFile->test = (cs_TEST_CAST)CSgeocnT;
	gridFile->frwrd2D = (cs_FRWRD2D_CAST)CSgeocnF2;
	gridFile->frwrd3D = (cs_FRWRD3D_CAST)CSgeocnF3;
	gridFile->invrs2D = (cs_INVRS2D_CAST)CSgeocnI2;
	gridFile->invrs3D = (cs_INVRS3D_CAST)CSgeocnI3;
	gridFile->inRange = (cs_INRANGE_CAST)CSgeocnL;
	gridFile->release = (cs_RELEASE_CAST)CSgeocnR;
	gridFile->destroy = (cs_DESTROY_CAST)CSgeocnD;
	return 0;

error:
	if (geocnPtr != NULL)
	{
		CSgeocnD (geocnPtr);		/*lint !e534  ignoring return value */
		geocnPtr = NULL;
	}
	return -1;
}
/* The following is a bit different fron other transformations. The longitude
   coverage values in the grid file structure range from zero to 360.  The LL
   input values will range from -180 to +180.  This is nice, in a way, as
   it does away with the Alaska kludge we had to use for NADCON.  Thus, the
   longitudes for Alaska start at about 166.0 and extend through 232.0 (232 
   being the equivalent of 128 degrees west longitude). */
double CSgeocnT (struct cs_Geocn_ *geocn,double *ll_src,short direction)
{
	extern double cs_Zero;
	extern double cs_K360;

	double density;
	double lclSrcLl [3];

	lclSrcLl [LNG] = ll_src [LNG];
	lclSrcLl [LAT] = ll_src [LAT];
	if (lclSrcLl [LNG] < cs_Zero)
	{
		lclSrcLl [LNG] += cs_K360;
	}

	/* We established at setup time that the coverages of all three
	   files are the same.  Thus, we only have to test a single file
	   in this function. */
	density = CStestGeoconFile (geocn->lngShift,lclSrcLl);
	return density;
}
int CSgeocnF2 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;
	extern double cs_K180;
	extern double cs_K360;
	extern double cs_Sec2Deg;

	int status;

	double deltaLng;
	double deltaLat;
	
	double lclSrcLl [3];

	lclSrcLl [LNG] = ll_src [LNG];
	lclSrcLl [LAT] = ll_src [LAT];
	ll_trg [HGT]   = ll_src [HGT];
	
	/* Internally, CS-MAP assumes longitudes are in the -180 to +180 range,
	   where negative values indicate west longitude. This is a pretty 
	   standard convention.  However, Alaska presents a real problem
	   when using this convention as portions of Alaska are west of
	   -180.0 longitude.
	   
	   In the Geocon environment, this issue is addressed by assuming that
	   longitude is in the range of 0.0 thru +360.0.  We need to account
	   for this different convention before doing any serious calculations. */
	if (lclSrcLl [LNG] < 0.0)
	{
		lclSrcLl [LNG] += cs_K360;
	}

	deltaLng = deltaLat = cs_Zero;
	status = CScalcGeoconFile (geocn->lngShift,&deltaLng,lclSrcLl);
	if (status == csGRIDI_ST_OK)
	{
		status = CScalcGeoconFile (geocn->latShift,&deltaLat,lclSrcLl);
	}

	/* Status at this point is either 0 meaning there is coverage and the
	   calculation is good, or -1 indicating the data file does not provide
	   coverage.  The -1 condition should only happen in normal circumstances
	   in the case where this function is being called by the iterative
	   inverse function. */
	if (status != csGRIDI_ST_OK)
	{
		deltaLng = cs_Zero;
		deltaLat = cs_Zero;
	}
	ll_trg [LNG] = lclSrcLl [LNG] + deltaLng * (cs_Sec2Deg / 100000.0);
	ll_trg [LAT] = lclSrcLl [LAT] + deltaLat * (cs_Sec2Deg / 100000.0);

	/* Return to normal CS-MAP longitude/ */
	if (ll_trg [LNG] > cs_K180)
	{
		ll_trg [LNG] -= cs_K360;
	}
	return status;
}
int CSgeocnF3 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;
	extern double cs_K180;
	extern double cs_K360;
	extern double cs_Sec2Deg;

	int status;

	double deltaLng;
	double deltaLat;
	double deltaHgt;

	double lclSrcLl [3];

	lclSrcLl [LNG] = ll_src [LNG];
	lclSrcLl [LAT] = ll_src [LAT];
	lclSrcLl [HGT] = ll_src [HGT];

	/* Geocon uses longitude in the range from 0.0 to +360.0.  This does away
	   with the Alaska problem that NADCON experiences. */
	if (lclSrcLl [LNG] < cs_Zero)
	{
		lclSrcLl [LNG] += cs_K360;
	}

	deltaLng = deltaLat = deltaHgt = cs_Zero;
	status = CScalcGeoconFile (geocn->lngShift,&deltaLng,lclSrcLl);
	if (status == csGRIDI_ST_OK)
	{
		status = CScalcGeoconFile (geocn->latShift,&deltaLat,lclSrcLl);
	}
	if (status == csGRIDI_ST_OK && geocn->hgtShift != NULL)
	{
		status = CScalcGeoconFile (geocn->hgtShift,&deltaHgt,lclSrcLl);
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
		deltaHgt = cs_Zero;
	}

	ll_trg [LNG] = lclSrcLl [LNG] - deltaLng * (cs_Sec2Deg / 100000.0);
	ll_trg [LAT] = lclSrcLl [LAT] + deltaLat * (cs_Sec2Deg / 100000.0);
	ll_trg [HGT] = lclSrcLl [HGT] + deltaHgt * ( 1.0 / 100.0);		/* grid data in centimeters */

	/* Return yto normal CS-MAP longitude convention. */
	if (ll_trg [LNG] > cs_K180)
	{
		ll_trg [LNG] -= cs_K360;
	}

	return status;
}
int CSgeocnI2 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;

	short lng_ok;
	short lat_ok;

	int ii;
	int status;

	double guess [3];
	double epsilon [2];
	double newResult [3];

	status = -1;			/* Defensive, until we know differently. */
	epsilon [0] = epsilon [1] = cs_Zero;		/* kep lint happy */

	guess [LNG] = ll_src [LNG];
	guess [LAT] = ll_src [LAT];

	/* Start a loop which will iterate up to maxIteration times. */
	for (ii = 1;ii < geocn->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lng_ok = lat_ok = TRUE;

		/* Compute the NAD83 lat/long for our current guess. */
		status = CSgeocnF2 (geocn,newResult,guess);
		if (status != csGRIDI_ST_OK)
		{
			/* A negative status here is possible if this iterative solution
			   has wandered outside of the coverage of the grid data file. */
			break;
		}

		/* See how far we are off. */
		epsilon [LNG] = -CS_deltaLongitude (ll_src [LNG],newResult [LNG]);
		epsilon [LAT] = ll_src [LAT] - newResult [LAT];

		/* If our guess at the longitude is off by more than cnvrgValue, we
		   adjust our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > geocn->cnvrgValue)
		{
			lng_ok = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		if (fabs (epsilon [LAT]) > geocn->cnvrgValue)
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
		   message. */
		if (ii >= geocn->maxIterations)
		{
			status = csGRIDI_ST_COVERAGE;
			CS_erpt (cs_NADCON_ICNT);
			if (fabs (epsilon [LNG]) > geocn->errorValue ||
				fabs (epsilon [LAT]) > geocn->errorValue)
			{
				/* If the iteration appeared to diverge instead of converge,
				   we consider it a fatal error. */
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
int CSgeocnI3 (struct cs_Geocn_ *geocn,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;
	extern double cs_K360;

	int status;
	double deltaHgt;
	
	double lclSrcLl [3];

	/* We use the source latitude and longitude to determine the elevation
	   shift. It is assumed that any change in the vertical shift due to the
	   very slight changes in the lat/long position are incredibly small and
	   an order of magnitude less than the actual accuracy of the data being
	   used.  Therefore, we assume we can use the unshifted geographic
	   coordinates to determine the height shift.  That's what geocon.f does.

	   As always, we need to adjust longitude as all Geocon data files are
	   based on longitude ranging from 0.0 to +360.0;

	   NOTE:  This relies on the fact that currently, CSgeocnI2 does not
	   modify in any way the elevation portion of the target point.  We
	   could add some defensive code here for that, but this is a performance
	   sensitive function. */

	lclSrcLl [LNG] = ll_src [LNG];
	lclSrcLl [LAT] = ll_src [LAT];
	lclSrcLl [HGT] = ll_src [HGT];
	if (lclSrcLl [LNG] < cs_Zero)
	{
		lclSrcLl [LNG] += cs_K360;
	}

	deltaHgt = cs_Zero;
	if (geocn->hgtShift != NULL)
	{
		status = CScalcGeoconFile (geocn->hgtShift,&deltaHgt,lclSrcLl);
	}

	/* Use the 2D inverse to move the horizontal portion of the coordinate.
	   Be careful to make sure the longitude adjustment does not get done
	   twice. */
	status = CSgeocnI2 (geocn,ll_trg,ll_src);
	if (status == csGRIDI_ST_OK)
	{
		ll_trg [2] = lclSrcLl [2] - deltaHgt * ( 1.0 / 100.0);
	}
	return status;
}
int CSgeocnL  (struct cs_Geocn_ *geocn,int cnt,Const double pnts [][3])
{
	short ok;
	int index;
	double density;

	ok = TRUE;
	for (index = 0;index < cnt;index += 1)
	{
		/* Note: CSgeocnT ignores the direction parameter.  The assumption
		   always forward.  We also assume that the density of all grid files
		   is positive, for performance purposes. */
		density = CSgeocnT (geocn,pnts [index],cs_DTCDIR_FWD);
		if (density < 1.0E-08)		/* i.e. == 0.0 */
		{
			ok = FALSE;
		}
	}
	return (ok != TRUE) ? cs_CNVRT_USFL : cs_CNVRT_OK;
}
/* CSgeocnR -> Geocon Release
	This function releases resources such as memory buffers and
	file streams.  However, leaves the control structure in
	place.  Thus, the coverage of a specific Geocon file set
	is maintained in memory. */
int CSgeocnR  (struct cs_Geocn_ *geocn)
{
	/* CSreleaseGeoconFile checks for a NULL pointer before it does anything. */
	CSreleaseGeoconFile (geocn->lngShift);
	CSreleaseGeoconFile (geocn->latShift);
	CSreleaseGeoconFile (geocn->hgtShift);
	return 0;
}
/* CSgeocnD -> Geocon Destructor
	Releases all resources and deletes all memory control structures. */
int CSgeocnD  (struct cs_Geocn_ *geocn)
{
	if (geocn != NULL)
	{
		/* CSdeleteGeoconFile checks for a null pointer before it does anything. */ 
		CSdeleteGeoconFile (geocn->lngShift);
		geocn->lngShift = NULL;

		CSdeleteGeoconFile (geocn->latShift);
		geocn->latShift = NULL;

		CSdeleteGeoconFile (geocn->hgtShift);
		geocn->hgtShift = NULL;

		CS_free (geocn);
	}
	return 0;
}
/******************************************************************************
*******************************************************************************
**          cs_GeoconFile_  -->  A Single GEOCON file Object                 **
**                                                                           **
**                                                                           **
*******************************************************************************
******************************************************************************/
struct cs_GeoconFile_* CSnewGeoconFile (Const char* filePath,long32_t bufferSize,
															 ulong32_t flags,
															 double density,
															 enum csGeocnFileType fileType)
{
	extern char cs_DirsepC;
	extern char cs_ExtsepC;
	extern char csErrnam [];

	int swapped;
	long32_t lngTmp;

	const char *cpK;
	char *cp1;
	csFILE *fstr;
	struct cs_GeoconFile_* thisPtr;
	struct cs_GeoconFileHdr_ geoconFileHdr;

	/* Prepare for an error. */
	thisPtr = NULL;
	fstr = NULL;

	/* Malloc and initialize */
	thisPtr = CS_malc (sizeof (struct cs_GeoconFile_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	/* Set default values for all members. */
	CSinitGeoconFile (thisPtr);

	/* Adjust the buffer size to that requested; otherwise default value if no
	   specific request was made. No request is the typical, so the default
	   buffersize has a huge affect on the performance of all the Geocon
	   stuff. */
	thisPtr->bufferSize = bufferSize;
	if (thisPtr->bufferSize <= 0) thisPtr->bufferSize = csGeocnBufrSize;

	/* Save file path/name. This is sometimes used in reporting which specific
	   grid data file was used for a specific calculation.  Thus, we just keep
	   it small.  The full path is used for error reporting, so we want all
	   possible information in that element. */
	CS_stncp (thisPtr->filePath,filePath,sizeof (thisPtr->filePath));
	cpK = strrchr (filePath,cs_DirsepC);
	if (cpK != NULL) cpK += 1;
	else             cpK = filePath;
	CS_stncp (thisPtr->fileName,cpK,sizeof (thisPtr->fileName));
	cp1 = strrchr (thisPtr->fileName,cs_ExtsepC);
	if (cp1 != NULL) *cp1 = '\0';

	/* Set the file type.  For Geocon files, the type is basically a
	   function of who is constructing this object.  Therefore, the
	   actual usage type is a parameter to this function. */
	thisPtr->type = fileType;

	/* Get the file information header. */
	fstr = CS_fopen (thisPtr->filePath,_STRM_BINRD);
	if (fstr == NULL)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;
	}

	/* Read the header information.  This function also determines
	   if the data file is in big or little endian byte order. */
	swapped = CSheaderGeoconFile (&geoconFileHdr,fstr);
	if (swapped < 0)
	{
		CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);
		CS_erpt (cs_DTC_FILE);
		goto error;		
	}
	thisPtr->swapBytes = (swapped != 0);

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

	/* This constructor doesn't need the file any more.  It is quite likely
	   that we may never need to access this file.  We may only need to
	   know the geographic coverage of the file. */
	CS_fclose (fstr);
	fstr = NULL;

	/* The extra stuff here is required as conversions of floats to doubles
	   does not always provide precise results.  To get the precise results we
	   require, we assume that the value (which is in degrees) is an intergal
	   number of seconds.  In this way, we escape the inaccuracies encountered
	   when trying to calculations with numbers like 0.016666666666666666... 
	   which are very popular in the Geocon data files.*/
	thisPtr->headerSize = geoconFileHdr.headerSize;
	thisPtr->elementCount = geoconFileHdr.lngCount;
	thisPtr->recordCount = geoconFileHdr.latCount;

	/* The FORTRAN nature of the program which produces the official binary
	   files likes to put a long32_t element on the front and back of each
	   record.  Thus, reecordSize as we need to use it is not exactly what
	   you would normally expect. */
	thisPtr->recordSize = geoconFileHdr.lngCount * sizeof (float) + 
						  sizeof (long32_t) + 
						  sizeof (long32_t);

	/* We play the following games to insure accuracy in the conversions
	   of the delta values which are often ugly numbers from binary
	   number point of view. */
	thisPtr->latMin = geoconFileHdr.latMin;
	lngTmp = (long32_t)(((double)geoconFileHdr.latDelta * 3600.0) + 0.4);
	thisPtr->deltaLng = ((double)lngTmp / 3600.0);
	lngTmp *= (thisPtr->recordCount - 1);
	thisPtr->latMax = thisPtr->latMin + ((double)lngTmp / 3600.0);

	thisPtr->lngMin = geoconFileHdr.lngMin;
	lngTmp = (long32_t)(((double)geoconFileHdr.lngDelta * 3600.0) + 0.4);
	thisPtr->deltaLat = ((double)lngTmp / 3600.0);
	lngTmp *= (thisPtr->elementCount - 1);
	thisPtr->lngMax = thisPtr->lngMin + ((double)lngTmp / 3600.0);

	/* We use the smaller of the two possible cell sizes as the density
	   value. */
	thisPtr->density = thisPtr->deltaLat;
	if (thisPtr->deltaLng < thisPtr->deltaLat)
	{
		thisPtr->density = thisPtr->deltaLng;
	}

	/* Verify the integrity of the file. */
	lngTmp = (thisPtr->recordCount) * thisPtr->recordSize + thisPtr->headerSize;
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
		/* Requested buffer size is the whole file. */
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
			/* We require a minimum buffer size of 3 records.  The interpolation algorithm
			   below uses a 3 x 3 grid cell interpolation technique. */
			thisPtr->bufferSize = 3 * thisPtr->recordSize;
		}
	}

	/* We should be ready to rock and roll.  Note that the initialization
	   function should have initialized everything else in the structure to
	   the desired initial condition which is:
		1> file not open
		2> no buffer allocated
		3> nothing in the buffer
	*/
	return (thisPtr);
error:
	if (fstr!= NULL)
	{
		CS_fclose (fstr);
		fstr = NULL;
	}
	/* CSdeleteGeoconFile does not try to delete a null pointer. */
	CSdeleteGeoconFile (thisPtr);
	return NULL;
}
/******************************************************************************

	The following function deals with the issue of three files which have
	different names, yet are closely associated with each other.  It
	essentially defines the convention used to reference all three related
	files with a single file/path specification; such specification which is
	given in the Geodetic Transformation Dictionary.

	For this geodetic transformation (GEOCON) we expect file names in the form
	of:
		 dela.b,  delo.b,  dev.b		// Conterminous 48 states
		delaa.b, deloa.b, deva.b		// Alaska
		delap.b, delop.b, devp.b		// Puerto Rico & Virgin Islands

	We expect a file name specification in the form of (example):
		./USA/GEOCON/del?.b

	We simply replace the ? with 'a', and 'o'; and the 'l?' sequence with a 'v'
	in order to get the complete path names to the three files to be used in
	the datum shift calculation.  Note, we substitue uppercase letters if the
	'l' in the 'l?' sequence is an uppcase 'L'.

	Thus, users can specify all three files with one specification, and
	they have a lot of control over what that specification looks like and
	exactly where the files are located.  Also, the names of the files can
	remain exactly as originally named by the NGS (United States National
	Geodetic Service).

	Three pointers for the threee results are provided.  The file is checked
	for only if the corresponding pointer is non-null.  Thus, this function
	can be used to check each file independently or, more likely, to check
	for the two horizontal files and the vertical file separately.
*/
int CSpathsGeoconFile (char *pathBufferLng,char *pathBufferLat,char *pathBufferHgt,
															   Const char *pathBuffer)
{
	extern int cs_Errno;
	extern char csErrnam [MAXPATH];

	char cc1;
	char ccL;
	int fstat;
	int status;

	char *cp;

	char lclBuffer    [MAXPATH];
	char lclBufferHgt [MAXPATH];

	/* Prepare for an error in this module. */
	if (pathBufferLng != NULL)
	{
		*pathBufferLng = '\0';
	}
	if (pathBufferLat != NULL)
	{
		*pathBufferLat = '\0';
	}
	if (pathBufferHgt != NULL)
	{
		*pathBufferHgt = '\0';
	}
	cs_Errno = 0;
	status = 0;

	/* Locate the '?' which is the key to our algorithm. */
	cp = strrchr (pathBuffer,'?');
	
	/* Verify that the '?' is not the first, or last character in the
	   string. */
	if (cp != NULL)
	{
		if (cp == pathBuffer || *(cp + 1) == '\0')
		{
			cp = NULL;
		}
	}
	
	/* Verify that a lower or upper case 'L' preceeds the '?' character. */
	ccL = 'l';						/* keep the compiler happy */
	if (cp != NULL)
	{
		ccL = *(cp - 1);
		if (ccL != 'L' && ccL != 'l')
		{
			cp = NULL;
		}
	}
	if (cp == NULL)
	{
		/* Here if there was no '?' or if it not preceeded by an 'l' or an 'L';
		   i.e. an improper specification in the dictionary file. */
		CS_stncp (csErrnam,pathBuffer,MAXPATH);
		CS_erpt (cs_DTQ_FLSPEC);
		goto error;
	}

	/* Construct what we believe to be the longitude file of the three file
	   set. */
	if (pathBufferLng != NULL)
	{
		/* Get a local copy of the provided geocon path specification where we
		   can freely work with it.*/
		CS_stncp (lclBuffer,pathBuffer,sizeof (lclBuffer));

		/* Locate the '?' which is the key our algorithm. */
		cp = strrchr (lclBuffer,'?');
		if (cp == NULL)
		{
			/* Defensive programming and to keep lint happy. */
			CS_stncp (csErrnam,"CS_geocn.c:1",sizeof (csErrnam));
			CS_erpt (cs_ISER);
			goto error;
		}

		/* Check for the existence and readability of the indicated longitude
		   file. */
		*cp = (ccL == 'L') ? 'O' : 'o';
		fstat = CS_access (lclBuffer,4);
		if (fstat != 0)
		{
			CS_stncp (csErrnam,lclBuffer,MAXPATH);
			CS_erpt (cs_DTQ_FILE);
			status = cs_Errno;
			goto error;
		}
		else
		{
			CS_stncp (pathBufferLng,lclBuffer,MAXPATH);
			status = 0;
		}
	}

	/* Now for the Latitude file.  Essentially the same stuff as above,
	   sans comments. */
	if (pathBufferLat != NULL)
	{
		CS_stncp (lclBuffer,pathBuffer,sizeof (lclBuffer));
		cp = strrchr (lclBuffer,'?');
		if (cp == NULL)
		{
			/* Defensive programming and to keep lint happy. */
			CS_stncp (csErrnam,"CS_geocn.c:2",sizeof (csErrnam));
			CS_erpt (cs_ISER);
			goto error;
		}

		*cp = (ccL) == 'L' ? 'A' : 'a';
		fstat = CS_access (lclBuffer,4);
		if (fstat != 0)
		{
			CS_stncp (csErrnam,lclBuffer,MAXPATH);
			CS_erpt (cs_DTQ_FILE);
			status = cs_Errno;
			goto error;
		}
		else
		{
			CS_stncp (pathBufferLat,lclBuffer,MAXPATH);
			status = 0;
		}
	}

	/* Now for the Vertical file.  Essentially the same stuff as above,
	   But in this case, we need to remove a character from the proposed
	   path.  That is, we need to replace the "l?" sequence with a single
	   character: 'v'. */
	if (pathBufferHgt != NULL)
	{
		CS_stncp (lclBufferHgt,pathBuffer,sizeof (lclBufferHgt));
		cp = strrchr (lclBufferHgt,'?');
		if (cp == NULL)
		{
			/* Defensive programming and to keep lint happy. */
			CS_stncp (csErrnam,"CS_geocn.c:3",sizeof (csErrnam));
			CS_erpt (cs_ISER);
			goto error;
		}
		cc1 = (ccL == 'L') ? 'V' : 'v';
		*(cp - 1) = '%';
		*cp = 'c';
		sprintf (lclBuffer,lclBufferHgt,cc1);
		fstat = CS_access (lclBuffer,4);
		if (fstat != 0)
		{
			/* The file does not exist, or is not readable. */
			CS_stncp (csErrnam,lclBuffer,MAXPATH);
			CS_erpt (cs_DTQ_FILE);
			status = cs_Errno;
			goto error;
		}
		else
		{
			/* The vertical file exists and is readable. */
			CS_stncp (pathBufferHgt,lclBuffer,MAXPATH);
			status = 0;
		}
	}
	return status;
error:
	return cs_Errno;
}
/* The following reads a GEOCON file header, one element at a time
   so as to avoid structure packing issues between different compilers. */
int CSheaderGeoconFile (struct cs_GeoconFileHdr_ *thisPtr,csFILE *fstr)
{
	extern double cs_Zero;
	extern char csErrnam [MAXPATH];

	int status;
	int swapped;
	size_t readCount;
	
	union
	{
		long32_t tmpInt;
		double tmpDbl;
	} readBuffer;

	status = 0;
	swapped = FALSE;

	/* Initialize the header to something recognizable as initialized,
	   but not valid. */
	thisPtr->latMin   = cs_Zero;
	thisPtr->lngMin   = cs_Zero;
	thisPtr->latDelta = cs_Zero;
	thisPtr->lngDelta = cs_Zero;
	thisPtr->latCount = 0L;
	thisPtr->lngCount = 0L;
	thisPtr->iKind    = 0L;
	thisPtr->headerSize = 0L;

	/* Read the header from the file, one element at a time. Doing this one
	   element at a time eliminates structure packing problems with the
	   different compilers/platforms.

	   The code is rather laborious, but easy to follow and easy to debug. 

	   Note that the official binary files are written by a FORTRAN program
	   using "unformatted" format.  FORTRAN adds strange stuff to the binary
	   image.  I can't explain why, but just so you know the extraneous
	   stuff in here is indeed necessary.

	   The first four bytes of the binary image carry the size of the
	   header in bytes.  The 4 byte recordSize does not include the
	   recordSize data element itself. */
	readCount = CS_fread (&readBuffer,sizeof (long32_t),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	/* The values for checked the byte order of the record size element were
	   chosen pretty arbitrarily.  The expected value as of this writing is
	   44. */
	if (readBuffer.tmpInt <= 24 || readBuffer.tmpInt > 128)
	{
		swapped = TRUE;
		CSbswap ((void*)&readBuffer,"l");
	}
	thisPtr->headerSize = readBuffer.tmpInt + sizeof (long32_t);
	/* The FORTRAN output mechanism appears to add a record size element at the
	   end of each record also.  So . . .  */
	thisPtr->headerSize += sizeof (long32_t);

	readCount = CS_fread (&readBuffer,sizeof (double),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	if (swapped)
	{
		CSbswap ((void*)&readBuffer,"d");		/*lint !e534  ignoring return value */
	}
	thisPtr->latMin = readBuffer.tmpDbl;

	readCount = CS_fread (&readBuffer,sizeof (double),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	if (swapped)
	{
		CSbswap ((void*)&readBuffer,"d");
	}
	thisPtr->lngMin = readBuffer.tmpDbl;

	readCount = CS_fread (&readBuffer,sizeof (double),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	if (swapped)
	{
		CSbswap ((void*)&readBuffer,"d");
	}
	thisPtr->latDelta = readBuffer.tmpDbl;

	readCount = CS_fread (&readBuffer,sizeof (double),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	if (swapped)
	{
		CSbswap ((void*)&readBuffer,"d");
	}
	thisPtr->lngDelta = readBuffer.tmpDbl;

	readCount = CS_fread (&readBuffer,sizeof (long32_t),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	if (swapped)
	{
		CSbswap ((void*)&readBuffer,"l");
	}
	thisPtr->latCount = readBuffer.tmpInt;

	readCount = CS_fread (&readBuffer,sizeof (long32_t),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	if (swapped)
	{
		CSbswap ((void*)&readBuffer,"l");
	}
	thisPtr->lngCount = readBuffer.tmpInt;

	readCount = CS_fread (&readBuffer,sizeof (long32_t),1,fstr);
	if (CS_ferror (fstr)) {CS_erpt (cs_IOERR); goto error; }
	if (readCount != 1)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}
	if (swapped)
	{
		CSbswap ((void*)&readBuffer,"l");
	}
	thisPtr->iKind = readBuffer.tmpInt;

	/* A basic check for all of this is that iKind should be +1L. */
	if (thisPtr->iKind != 1L)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}

	/* Check the header for general validity. */
	status = CSchkHdrGeoconFile (thisPtr);
	if (status < 0)
	{
		CS_erpt (cs_DTQ_FORMAT);
		goto error;
	}

	/* Rewind the file, so when we return it in a known state. */
	CS_fseek (fstr,0L,SEEK_SET);

	/* Return the swapped flag so we know how to handle the grid cell data in
	   the file. */
	return swapped;

error:
	/* At this point,we only have the stream, no file name.  We do the
	   best we can for an error message.  Hopefully, the calling module
	   will provide the actual name of the file. */
	CS_fseek (fstr,0L,SEEK_SET);
	CS_stncp (csErrnam,"GEOCON file",sizeof (csErrnam));
	return -1;
}
int CSchkHdrGeoconFile (struct cs_GeoconFileHdr_ *thisPtr)
{
	extern double cs_One;
	extern double cs_K360;
	extern double cs_MinLat;
	extern double cs_MaxLat;
	extern double cs_MinLngFz;
	extern double cs_Sec2Deg;

	/* Here to check the validity of a GEOCON file header.  We assume
	   it is valid until we know different. */
	if (CS_isnan (thisPtr->latMin) || thisPtr->latMin < cs_MinLat
								   || thisPtr->latMin > cs_MaxLat)
	{
		return -1;
	}
	if (CS_isnan (thisPtr->lngMin) || thisPtr->lngMin < cs_MinLngFz
								   || thisPtr->lngMin > cs_K360)
	{
		return -1;
	}
	if (CS_isnan (thisPtr->latDelta) || thisPtr->latDelta < cs_Sec2Deg
									 || thisPtr->latDelta > cs_One)
	{
		return -1;
	}
	if (CS_isnan (thisPtr->lngDelta) || thisPtr->lngDelta < cs_Sec2Deg
									 || thisPtr->lngDelta > cs_One)
	{
		return -1;
	}
	/* These test values here are quite arbitrary. */
	if (thisPtr->latCount < 16L || thisPtr->latCount > 100000)
	{
		return -1;
	}
	/* These test values here are quite arbitrary. */
	if (thisPtr->lngCount < 16L || thisPtr->lngCount > 100000)
	{
		return -1;
	}
	return 0;
}
void CSinitGeoconFile (struct cs_GeoconFile_ *thisPtr)
{
	extern double cs_Zero;

	thisPtr->type                = geocnTypeNoneYet;
	thisPtr->latMin              = cs_Zero;
	thisPtr->lngMin              = cs_Zero;
	thisPtr->latMax              = cs_Zero;
	thisPtr->lngMax              = cs_Zero;
	thisPtr->deltaLng            = cs_Zero;
	thisPtr->deltaLat            = cs_Zero;
	thisPtr->density             = cs_Zero;
	thisPtr->elementCount        = 0L;
	thisPtr->recordCount         = 0L;
	thisPtr->elementSize         = sizeof (float);
	thisPtr->headerSize          = (4 * sizeof (double) + 3 * sizeof (long32_t) + 2 * sizeof (long32_t));
	thisPtr->recordSize          = 0L;
	thisPtr->fileSize            = 0L;
	thisPtr->bufferSize          = 0L;
	thisPtr->bufferBeginPosition = -1L;
	thisPtr->bufferEndPosition   = -2L;
	thisPtr->strm                = NULL;
	thisPtr->dataBuffer          = NULL;
	thisPtr->swapBytes           = FALSE;
	thisPtr->filePath [0]        = '\0';
	thisPtr->fileName [0]        = '\0';
	CSinitGeocnGridCell (&thisPtr->gridCell);
}
void CSreleaseGeoconFile (struct cs_GeoconFile_* thisPtr)
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

		/* Initialize the grid cell so it doesn't match anything.
		CSinitGridCell (&thisPtr->currentCell); */

		/* Close the file (if open). */
		if (thisPtr->strm != NULL)
		{
			CS_fclose (thisPtr->strm);
			thisPtr->strm = NULL;
		}
	}
	return;
}
void CSdeleteGeoconFile (struct cs_GeoconFile_* thisPtr)
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
			thisPtr->bufferBeginPosition = -1L;
			thisPtr->bufferEndPosition = -2L;
		}
		CS_free (thisPtr);
	}
	return;
}
double CStestGeoconFile (struct cs_GeoconFile_* thisPtr,Const double *sourceLL)
{
	extern double cs_Zero;
	extern double cs_K360;

	double density;

	double lclLng;
	double lclLat;
	
	density = 0.0;

	lclLat = sourceLL [LAT];
	if (lclLat >= thisPtr->latMin && lclLat <= thisPtr->latMax)
	{
		lclLng = sourceLL [LNG];
		if (lclLng < cs_Zero)
		{
			lclLng += cs_K360;
		}
		if (lclLng >= thisPtr->lngMin && lclLng <= thisPtr->lngMax)
		{
			density = thisPtr->density;
		}
	}
	return density;
}
int CScalcGeoconFile (struct cs_GeoconFile_* thisPtr,double *result,Const double* sourceLL)
{
	extern double cs_Half;
	extern double cs_LlNoise;			/* 1.0E-12 */
	extern double cs_Huge;

	int status;

	long32_t eleNbr;
	long32_t recNbr;

	enum csGeocnEdgeEffects edge;

	double cellLngDelta;
	double cellLatDelta;

	struct csGeocnGridCell_ myEdgeGridCell;

	/* Prepare for an error.  In the case of an error, we return an obnoxious
	   value which will clearly indicate something is wrong.  We do this in
	   case the calling module fails to properly detect or handle the error
	   condition which we may have to return. */
	*result = cs_Huge;

	/* Until we know differently, we assume the normal case of the data point
	   being in the middle of the grid somewhere. */
	edge = geocnEdgeNone;
	status = csGRIDI_ST_OK;

	/* Compute the basic indices to the cell in the data file. Use of cs_LlNoise
	   is used to force a 'real' result, such as 0.99999999999998, to be a hard
	   1.0.  These values are the equivalent of the ix  and iy values in geocon.f

	   Note, at this point, we expect the GEOCON longitude adjustment to have
	   applied already.
	   
	   Also note that this calculation will yield the indices of the specific
	   grid value which is the southwest corner of the the 3x3 grid cell
	   containing the query point.  We will adjust below for the case where
	   the query point is closer to the north and/or east edges of that 3x3
	   grid cell. */
	eleNbr = (long32_t)((sourceLL [LNG] - thisPtr->lngMin + cs_LlNoise) / thisPtr->deltaLng);
	recNbr = (long32_t)((sourceLL [LAT] - thisPtr->latMin + cs_LlNoise) / thisPtr->deltaLat);

	if (eleNbr >= (thisPtr->elementCount - 1) || recNbr >= (thisPtr->recordCount - 1) ||
		eleNbr <=  0                          || recNbr <=  0)
	{
		/* Getting here should be very very rare, if ever.  So what we do here
		   can be rather inefficient without affecting normal performance.
		   Getting here means either an error of some sort, or that we are on
		   the edge of the grid and special handling is required.  Either way,
		   robustness is required much more than performance.

		   We deal with the possibility of an error condition first; keeping
		   in mind that performance in this block of code is not crucial. */
		if (eleNbr < 0 || eleNbr > thisPtr->elementCount ||
			recNbr < 0 || recNbr > thisPtr->recordCount)
		{
			/* This is definitely not supposed to happen.  This is a 'private'
			   function and should only be called after the source coordinate
			   has been verified to be within the coverage of this grid file.
			   
			   Well, at least that was the original idea. Upon testing, we
			   find that the inverse function can at times meander across the
			   edges of the grid file.  This was found to occur in modules
			   where the code is manipulating coordinate coverage extrema
			   for some obscure reason. So, try to handle this situation by
			   simply returning a warning status.  The CSgeocnI2 function
			   deals with this properly.
			   
			   Under normal circumstances (i.e. data in the normal range
			   for geodetic applications) this code should never be executed.
			   This is a good place to put a break point during testing. */
			CS_erpt  (cs_DTC_RNG_W);
			status = csGRIDI_ST_COVERAGE;
		}
		else
		{
			status = csGRIDI_ST_OK;
			/* OK, we have an edge issue; i.e the query point is within
			   deltaLng or deltaLat of an edge or corner of the grid. At
			   this point, we simply determine the type of edge and adjust
			   the grid file data indices so as to get the correct data
			   into the grid cell.
			   
			   As it turns out, what we really end up doing is constructing a
			   special grid cell specific to the particular edge condition
			   we have encountered. So, it is the value of the "edge" variable
			   which is really crucial here. */
			if (recNbr < 1)
			{
				recNbr = 1;
				if (eleNbr < 1)
				{
					/* Southwest corner of the grid.  The center point of
					   the 3x3 cell is at indices [1,1]. */
					eleNbr = 1;
					edge = geocnEdgeSouthwest;
				}
				else if (eleNbr >= (thisPtr->elementCount - 1))
				{
					eleNbr = thisPtr->elementCount - 2;
					edge = geocnEdgeSoutheast;
				}
				else
				{
					edge = geocnEdgeSouth;
				}
			}
			else if (recNbr >= (thisPtr->recordCount - 1))
			{
				recNbr = thisPtr->recordCount - 2;
				if (eleNbr < 1)
				{
					eleNbr = 1;
					edge = geocnEdgeNorthwest;
				}
				else if (eleNbr >= (thisPtr->elementCount - 1))
				{
					eleNbr = thisPtr->elementCount - 2;
					edge = geocnEdgeNortheast;
				}
				else
				{
					edge = geocnEdgeNorth;
				}
			}
			else
			{
				/* Here if we are not on either the southern or northern
				   boundary of the grid.  POssible on the east or west
				   boundary, but not any of the four corners. */
				if (eleNbr < 1)
				{
					eleNbr = 1;
					edge = geocnEdgeWest;
				}
				else if (eleNbr > (thisPtr->elementCount -1))
				{
					eleNbr = thisPtr->elementCount - 2;
					edge = geocnEdgeEast;
				}
				else
				{
					/* We should never get here.  Perhaps we shoul report this
					   as an internal software error. */
					edge = geocnEdgeNone;
				}
			}
		}
	}

	/* There are four ways to go from here.
		1> Status indicates no coverage, we're done.
		2> We are on the edge of the grid, special processing is required and
		   a special grid cell object needs to be generated and used to
		   calculate the result.  Very very rare.
		3> We have a query point within the grid, there is a valid grid cell,
		   AND the query point is covered by the grid cell.  We simply use
		   the existing grid cell.
		4> We have a query point within the grid, but there is no valid grid
		   cell _OR_ the query point is not covered by the grid cell.  We need
		   to interrogate the grid file and build a new grid cell which covers
		   the provided query point.
	*/
	if (status == csGRIDI_ST_OK)
	{
		if (edge == geocnEdgeNone)
		{
			/* This is the normal case, the query point is in the middle of the
			   grid as defined by the grid file.
			   
			   At this point, eleNbr and recNbr are a guess as to the grid
			   indices of the center point of the 3x3 grid cell containing the
			   query point.  This will be true about half of the time.  It will
			   be false in those cases where the query point is closer to the
			   north and/or eastern edge of the 2x2 grid cell of which the
			   current central point is the southwestern point thereof.

			   Compute the delta value into the 3x3 cell, and adjust the
			   indices so that the indices refer to the point to which this
			   query point isclosest.  Clearly, in a regular grid like this,
			   a delta value greater than or equal to 0.5 indicates that the
			   point is the closest. Our reference point is always the
			   southwest corner of the grid file, so these 'cell delta values'
			   will always be positive because of our assumption at this point
			   that the provided query point is always within the coverage of
			   the underlying grid file.
			   
			   So, calculate the distance of the query point from the currently
			   selected grid cell normalized to the the size of the grid cell. */
			cellLngDelta = ((sourceLL [LNG] - thisPtr->lngMin) - (thisPtr->deltaLng * (double)eleNbr)) / thisPtr->deltaLng;
			cellLatDelta = ((sourceLL [LAT] - thisPtr->latMin) - (thisPtr->deltaLat * (double)recNbr)) / thisPtr->deltaLat;
			
			/* Things get a bit strange here.  However, we are duplicating the
			   geocon algorithm as written by the National Geodetic Survey (US)
			   in FORTRAN as strange as it might appear to the ordinary
			   computer geek such as myself. That is, it is important that we
			   reproduce as close as possible the numbers produced by the
			   geocon.exe program regardless of how strange the algorithm
			   might appear of us geeks. So:

			   If the query point is in the western/southern half of the
			   currently "selected" grid cell, which will end up being be the
			   southwestern most cell of the 3x3 matrix, we adjust our
			   "selected" grid cell so that the "selected" grid cell will be
			   the grid cell which is wets, and/or south, of the currently
			   selected grid cell. */
			if (cellLngDelta < cs_Half && eleNbr > 0)
			{
				eleNbr -= 1;		/* geocon.f does the equivalent of cellLngDelta += 1.0; here,
									   We don't need this as this value is recalculated in the
									   actual grid cell code. */
			}
			if (cellLatDelta < cs_Half && recNbr > 0)
			{
				recNbr -= 1;		/* geocon.f does the equivalent of cellLatDelta += 1.0; here,
									   We don't need this as this value is recalculated in the
									   actual grid cell code. */
			}

			/* At this point, eleNbr is the equivalent of the geocon.f ix value,
			   and recNbr is the equivalent to the iy value; essentially index
			   values into the grid file of the southwestern cell the 3x3 grid
			   cell matrix which is appropriate for the given query point.  Our
			   indices are zero based, of course, while those of geocon.f are 
			   one based.

			   If we have a valid grid cell, and the index values which were used
			   to build it are the same as what we have calculated ;ast time, we
			   simply reuse the existing grid cell.  This condition is often met
			   as the grid cells are one minute square, which means that a grid
			   cell, on average, will cover about 1.8 million square meters
			   (about 6 square miles) */
			if (thisPtr->gridCell.isValid &&
				thisPtr->gridCell.lngIdx == eleNbr &&
				thisPtr->gridCell.latIdx == recNbr)
			{
				*result = CScalcGeocnGridCell (&thisPtr->gridCell,sourceLL);
			}
			else
			{
				/* We need to build a new grid cell, actually a 3x3 matrix of grid cell
				   values, given the currently "selected" individual grid cell. */
				status = CSextractGeoconGridFile (&thisPtr->gridCell,thisPtr,eleNbr,recNbr);
				if (status == csGRIDI_ST_OK && thisPtr->gridCell.isValid)	/* redundant */
				{
					*result = CScalcGeocnGridCell (&thisPtr->gridCell,sourceLL);
				}
			}
		}
		else
		{
			/* Construct a dummy grid cell for the specifc edge condition
			   encountered.  These are so rare, it's not worth caching these
			   things. */
			status = CSedgeGeocnGridCell (&myEdgeGridCell,thisPtr,eleNbr,recNbr,edge);
			if (status == csGRIDI_ST_OK)
			{
				*result = CScalcGeocnGridCell (&myEdgeGridCell,sourceLL);
			}
		}
	}
	return status;
}
/* Please note that the algorithm used below is an attempt to duplicate the
   algorithm used in the geocon.f FROTRAN program.  It looks strange to me,
   and I therefore assume it will look strange to any one else who is actually
   reading this.  Note the that the specific cell value indexed by the
   eleNbr and recNbr arguments to this function are the indices of the
   southwest cell of the desired 3x3 cell value matrix which we will base
   our interpolations on. */
int CSextractGeoconGridFile (struct csGeocnGridCell_ *cellPtr,struct cs_GeoconFile_* thisPtr,
															  long32_t eleNbr,
															  long32_t recNbr)
{
	int status;

	long32_t fpos;

	char *chrPtr;
	char* chrPtrSw;
 
	union
	{
		char unswappedChars [sizeof (float) * 3];
		float floatValues[3];
	} rowUnion;

	status = csGRIDI_ST_SYSTEM;			/* Until we know differently */
	cellPtr->isValid = FALSE;			/* Until we know differently */
	
	/* Get the required data into the buffer.  CSreadGeoconGridFile is smart
	   enough to know that the 'recNbr' argument indicates the 'southernmost'
	   record of a three (minimum) record set. */
	status = CSreadGeoconGridFile (thisPtr,recNbr);
	if (status == csGRIDI_ST_OK)
	{
		/* Extract from the buffer the nine floats which we need.  We compute
		   the file position of the central data element of the 3x3 cell. */
		fpos = thisPtr->headerSize +			/* file header */
			  (recNbr * thisPtr->recordSize) +	/* skip to desired record */
			  sizeof (long32_t) +				/* record size word of target record */
			  (eleNbr * thisPtr->elementSize);	/* position within the record */

		/* Compute the address in the buffer of the southwest cell.
		   Unfortunately, this code now must get very specific to the data
		   type of the elements in the data file. */
		chrPtrSw = (char *)(thisPtr->dataBuffer) + (fpos - thisPtr->bufferBeginPosition);

		/* Do the three elements of the southernmost row.  We extract into a
		   temporary buffer so that we can do any byte swapping necessary
		   before we ask the host processor to treat these values as floats
		   and/or doubles.  We do this in order to avoid the posibility of
		   getting some sort of NaN exception on various processors or in
		   various run time environments.*/
		chrPtr = chrPtrSw;						/* For utmost readability */
		memcpy (rowUnion.unswappedChars,chrPtr,sizeof (rowUnion.unswappedChars));
		if (thisPtr->swapBytes)
		{
			CS_bswap (rowUnion.unswappedChars,"fff");
		}
		cellPtr->cellData [0][0] = (double)(rowUnion.floatValues [0]);
		cellPtr->cellData [0][1] = (double)(rowUnion.floatValues [1]);
		cellPtr->cellData [0][2] = (double)(rowUnion.floatValues [2]);

		/* Move north by one record, and then extract three data values. */
		chrPtr = chrPtrSw + thisPtr->recordSize;	/* For utmost readability */
		memcpy (rowUnion.unswappedChars,chrPtr,sizeof (rowUnion.unswappedChars));
		if (thisPtr->swapBytes)
		{
			CS_bswap (rowUnion.unswappedChars,"fff");
		}
		cellPtr->cellData [1][0] = (double)(rowUnion.floatValues [0]);
		cellPtr->cellData [1][1] = (double)(rowUnion.floatValues [1]);
		cellPtr->cellData [1][2] = (double)(rowUnion.floatValues [2]);

		/* Move north one record one more time to complete the 3x3 grid cell
		   value matrix. */
		chrPtr = chrPtrSw + thisPtr->recordSize + thisPtr->recordSize;	/* For utmost readability */
		memcpy (rowUnion.unswappedChars,chrPtr,sizeof (rowUnion.unswappedChars));
		if (thisPtr->swapBytes)
		{
			CS_bswap (rowUnion.unswappedChars,"fff");
		}
		cellPtr->cellData [2][0] = (double)(rowUnion.floatValues [0]);
		cellPtr->cellData [2][1] = (double)(rowUnion.floatValues [1]);
		cellPtr->cellData [2][2] = (double)(rowUnion.floatValues [2]);

		/*  Finish this off this 3x3 grid cell set by adding the proper
		    identifcation to the cell. */
		cellPtr->isValid = TRUE;
		cellPtr->cellAge  = 0;
		cellPtr->edge     = geocnEdgeNone;
		cellPtr->lngIdx   = eleNbr;
		cellPtr->latIdx   = recNbr;
		/* This is crucial: the lat/long of the southwest corner of the
		   southwestern grid cell.  Remember, at this point all longitudes
		   are positice (i.e. 0 <==> 360). Nevertheless, the shift is always in
		   a positive direction, i.e. always northwest; so we always add
		   regardless of the geographic location. */
		cellPtr->swLng    = thisPtr->lngMin + (thisPtr->deltaLng * (double)(eleNbr));
		cellPtr->swLat    = thisPtr->latMin + (thisPtr->deltaLat * (double)(recNbr));
		cellPtr->deltaLng = thisPtr->deltaLng;
		cellPtr->deltaLat = thisPtr->deltaLat;
		CS_stncp (cellPtr->sourceId,thisPtr->fileName,sizeof (cellPtr->sourceId));

		/* Mark it valid and set OK return value. */
		status = csGRIDI_ST_OK;
	}
	else
	{
		/* Disable the current grid cell to indicate that it is invalid. */
		CSinitGeocnGridCell (cellPtr);

		/* Release the resources allocated to this object.  The next call to this
		   object would then require a refresh. */
		CSreleaseGeoconFile (thisPtr);

		/* Negative return indicates a system error of sorts. */
		status = csGRIDI_ST_SYSTEM;
	}
	return status;
}
/* The following function reads a minimum of three records into the file buffer
   such that record 'recNbr' is in the buffer as well as the two records which
   immediately follow it.  That is, the indicated record and the two records
   "north" of the indicated record.  If the buffer is large enough to
   accomodate more than three records, this module will read additional records
   which preceed, and additional records which follow, the 'recNbr' record
   in a (hopefully) rational manner.  The idea is read in enough geography
   on the first shot which is likely to cover the entire area of a normal
   GIS project.  A default buffer size of 30 records, implying a region of
   coverage of .5 x .5 degrees, is about 900 square miles.  That should do
   it for your typical GIS project.
*/
int CSreadGeoconGridFile (struct cs_GeoconFile_* thisPtr,long32_t recNbr)
{
	extern char csErrnam [MAXPATH];

	int status;
	size_t readCount;
	size_t checkCount;

	long32_t lngTmp;
	long32_t checkSeek;
	long32_t fposBegin;
	long32_t fposEnd;

	status = csGRIDI_ST_SYSTEM;			/* Until we know differently */

#ifdef _DEBUG
	/* We are not supposed to have to deal with edge conditions in this
	   function. They are dealt with elsewhere.  If the following test ever
	   fails, please look elswhere for a problem. */
	if (recNbr < 1L || recNbr >= thisPtr->recordCount)
	{
		CS_stncp (csErrnam,"CS_geocn.c:4",sizeof (csErrnam));
		CS_erpt (cs_ISER);
		goto error;
	}
#endif

	/* Compute the beginning and ending positions of the minimum amount of data
	   required, and adjust to record boundaries.  We do I/O on a record
	   basis; and we always require a minimum of three records (even though the
	   records for these types of files can be very large, like 15KB).
	   Note that fposEnd is actually the position in the file of the first
	   byte following the data to be read. */
	fposBegin = recNbr * thisPtr->recordSize + thisPtr->headerSize;
	fposEnd = fposBegin + (3 * thisPtr->recordSize);

#ifdef _DEBUG
	/* Verify that the specified buffer size is sufficient for the
	   minimum amount of data which needs to be read. */
	readCount = fposEnd - fposBegin;
	if (readCount > (size_t)thisPtr->bufferSize)
	{
		/* Oops!!! A srew up in CSnewGeoconFile. */
		CS_stncp (csErrnam,"CS_geocn.c:5",sizeof (csErrnam));
		CS_erpt (cs_ISER);
		goto error;
	}
#endif

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

		/* Make sure the rest of this stuff knows the buffer is empty.  These
		   values will fail to match any specific file position. */
		thisPtr->bufferBeginPosition = -1L;
		thisPtr->bufferEndPosition = -2L;
	}

	/* See if the stuff we need is in the buffer.  Careful here, all of the
	   intended range must be in the buffer, not just a portion of it. */
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

		/* Compute the starting position of the actual read. First we see if we
		   can read in the entire file into the buffer. */
		readCount = (size_t)(thisPtr->recordCount * thisPtr->recordSize);
		if ((size_t)thisPtr->bufferSize >= readCount)
		{
			/* The buffer is big enough to hold the entire file,
			   so that's what we do. */
			thisPtr->bufferBeginPosition = thisPtr->headerSize;
			thisPtr->bufferEndPosition = thisPtr->bufferBeginPosition + (long32_t)readCount;
		}
		else
		{
			/* Typical case, the buffer size is significantly smaller than
			   the file size.  We want to read as much of the data file as
			   our allowed buffer size can handle.  However, we always read
			   an intergal nbumber of complete records to preserve our sanity
			   and lend som robustness to this code. */
			thisPtr->bufferBeginPosition = fposBegin;
			thisPtr->bufferEndPosition = fposEnd;
			readCount = (size_t)(thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition);

			/* In this section, lngTmp is the number of additional records
			   which can fit in the buffer. */
			lngTmp = (thisPtr->bufferSize - (long32_t)readCount) / thisPtr->recordSize;
			if (lngTmp > 2L)
			{
				/* Move the beginning of the read up by one half of the
				   amount of extra space in the buffer; but never past the
				   beginning of the first data record in the file. */
				lngTmp = lngTmp / 2;
				thisPtr->bufferBeginPosition -= thisPtr->recordSize * lngTmp;
				thisPtr->bufferEndPosition += thisPtr->recordSize * lngTmp;
				if (thisPtr->bufferBeginPosition < thisPtr->headerSize)
				{
					thisPtr->bufferBeginPosition = thisPtr->headerSize;
				}
				if (thisPtr->bufferEndPosition > thisPtr->fileSize)
				{
					thisPtr->bufferBeginPosition = thisPtr->fileSize;
				}
				readCount = (size_t)(thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition);
			}

			/* In the rare cases where we are working at or very near the
			   latitude minimum or the latitude maximum, there may still be
			   some room in the buffer for additional records. */
			lngTmp = (thisPtr->bufferSize - (long32_t)readCount) / thisPtr->recordSize;
			while (lngTmp > 0L)
			{
				if (thisPtr->bufferBeginPosition > thisPtr->headerSize)
				{
					thisPtr->bufferBeginPosition -= thisPtr->recordSize;
					lngTmp -= 1;
				}
				if (lngTmp > 0L && thisPtr->bufferEndPosition < thisPtr->fileSize)
				{
					thisPtr->bufferEndPosition += thisPtr->recordSize;
					lngTmp -= 1;
				}
				readCount = (size_t)(thisPtr->bufferEndPosition - thisPtr->bufferBeginPosition);
			}
			
			/* Defensive programming */
			if (readCount != (size_t)thisPtr->bufferSize)
			{
				/* Oops!!! All that stuff above has a bug in it. */
				CS_stncp (csErrnam,"CS_geocn::6",MAXPATH);
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
		checkCount = (size_t)CS_fread (thisPtr->dataBuffer,1,(size_t)readCount,thisPtr->strm);
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

		/* Since the GEOCON data records tend to be so large, and our design
		   is such that we read in a lot of data that will never actually be
		   used, we do not do any byte swapping until we know that specific
		   data values will be used.  We could try to read only those values
		   which are required, but that would be very complex (and therefore
		   error prone), end up reading the same data multiple times, and
		   probably be slower than the appropach we have adopted here.
		   
		   Performance is best sought in the grid cell cache which will,
		   essentially, tailor itself to the specific data set being
		   converted after a limited number of actual conversions. */

		/* OK, this has all completed successfuly as expected.  We can set the
		   appropriate return status now. */
		status = csGRIDI_ST_OK;
	}
	else
	{
		/* All of the necessary data is indeed in the buffer.  We can return
		   an OK status value. */
		status = csGRIDI_ST_OK;
	}
	return status;

error:
	/* The intended design objective for this whole module is that there
	   should be no normal conidition which this function can fail.  Thus,
	   if it does indeed fail, we do a 'release' of all resources such that
	   a retry will start with a clean slate. */
	CSreleaseGeoconFile (thisPtr);
	status = csGRIDI_ST_SYSTEM;
	return status;
}
/* The following function returns in the form of a double the specific value
   indicated by the eleNbr and recNbr arguments.  Very useful in handling
   specifc cases (like edge processing) where high performance is not a
   consideration. */
double CSvalueGeoconGridFile (struct cs_GeoconFile_* thisPtr,long32_t eleNbr,
															 long32_t recNbr)
{
	int status;
	
	long32_t fpos;
	char *chrPtr;
	
	double rtnValue;

	union
	{
		char charData [sizeof (float)];
		float floatData;
	} valueUnion;

	status = CSreadGeoconGridFile (thisPtr,recNbr);
	if (status == csGRIDI_ST_OK)
	{
		/* fpos here is the position in the "file" of the requested element.
		   Take note that there exists in the buffer a 4 byte record length
		   element on the front (and on the end) of each record in the buffer.
		   These values are included in the recordSize element, but the value
		   on the front needs to be considered when determining where the
		   desired element is within the record. */
		fpos = thisPtr->headerSize +				/* where grid data actually begins */
			   (recNbr * thisPtr->recordSize) +		/* move to the desired record */
			   sizeof (long32_t) +					/* deal with the "recodSize" word on the front of every record */
			   (eleNbr * thisPtr->elementSize);		/* Move to the specific element in the record */

		/* fpos gives us the position of the desired element in the actual data
		   file.  Where is it in the data buffer? */
		chrPtr = (char *)thisPtr->dataBuffer + (fpos - thisPtr->bufferBeginPosition);

		/* Get any necessary byte swappiong done before we ask the processor to
		   consider the data value as a float. */
		memcpy (valueUnion.charData,chrPtr,sizeof (float));
		if (thisPtr->swapBytes)
		{
			CS_bswap (valueUnion.charData,"f");
		}
		rtnValue = (double)valueUnion.floatData;
	}
	else
	{
		rtnValue = csGEOCON_SHIFT_ERROR
	}
	return rtnValue;
}
/* The following function populates the provided cell structure as is
   appropriate given the specific edge and position on that edge indicated
   by the parameter values. */
int CSedgeGeocnGridCell (struct csGeocnGridCell_ *cellPtr,struct cs_GeoconFile_* filePtr,
														  long32_t eleNbr,
														  long32_t recNbr,
														  enum csGeocnEdgeEffects edge)
{
	extern char csErrnam [MAXPATH];

	int status;

	long32_t lastElement;
	long32_t lastRecord;

	/* Until we know different. */
	status = csGRIDI_ST_SYSTEM;
	cellPtr->isValid = FALSE;

	if (edge == geocnEdgeNone || edge == geocnEdgeUnknown)
	{
		/* Oops!!! Edge conditions are supposed to be determined elswhere, and
		   this function called only after a successful determination of the
		   edge complexity.  If this test fails, an improper call to this
		   function has been made. */
		CS_stncp (csErrnam,"CS_geocn.c:7",sizeof (csErrnam));
		CS_erpt (cs_ISER);
		goto error;
	}

	/* Edge processing is a very rare situation, thus we do not concern
	   ourselves with performance in this function.

	   We will need some data from the grid data file to peroperly build the
	   edge grid cell. CSreadGeoconGridFile will give us what we need, and then
	   some. But, we don't care about performance in this function; so getting
	   more data than we need is just fine. */

	/* Initialize the grid cell object to a known 'empty' state. */
	CSinitGeocnGridCell (cellPtr);

	/* The following local variables help keep the code clean and readable. */
	lastElement = filePtr->elementCount - 1;
	lastRecord = filePtr->recordCount - 1;

	/* Populate as is appropriate for the specific edge condition we have
	   been given. */
	switch (edge) {
	default:
	case geocnEdgeNone:
	case geocnEdgeUnknown:
		status = csGRIDI_ST_SYSTEM;
		cellPtr->isValid = FALSE;		/* Yeah! Redundant. */
		break;

	case geocnEdgeSouthwest:
		/* Hard corner, i.e. first data element in the grid file. */
		cellPtr->cellData [0][0] = CSvalueGeoconGridFile (filePtr,0L,0L); 
		break;

	case geocnEdgeSoutheast:
		/* Hard corner, i.e. first data record, last data element. */
		cellPtr->cellData [0][2] = CSvalueGeoconGridFile (filePtr,lastElement,0L); 
		break;

	case geocnEdgeNorthwest:
		/* Hard corner, i.e. last data record, first data element. */
		cellPtr->cellData [2][0] = CSvalueGeoconGridFile (filePtr,0L,lastRecord); 
		break;

	case geocnEdgeNortheast:
		/* Hard corner, i.e. last data record, last data element. */
		cellPtr->cellData [2][2] = CSvalueGeoconGridFile (filePtr,lastElement,lastRecord); 
		break;

	case geocnEdgeSouth:
		/* Along the south edge, we need three values of which the specific
		   value referenced is the westernmost element. */
		cellPtr->cellData [0][0] = CSvalueGeoconGridFile (filePtr,eleNbr,0L); 
		cellPtr->cellData [0][1] = CSvalueGeoconGridFile (filePtr,(eleNbr + 1L),0L); 
		cellPtr->cellData [0][2] = CSvalueGeoconGridFile (filePtr,(eleNbr + 2L),0L); 
		break;

	case geocnEdgeEast:
		/* Along the east edge, we need three values of which the specific
		   value referenced is the southernmost element. */
		cellPtr->cellData [0][2] = CSvalueGeoconGridFile (filePtr,lastElement,recNbr); 
		cellPtr->cellData [1][2] = CSvalueGeoconGridFile (filePtr,lastElement,(recNbr + 1L)); 
		cellPtr->cellData [2][2] = CSvalueGeoconGridFile (filePtr,lastElement,(recNbr + 2L)); 
		break;
	
	case geocnEdgeNorth:
		/* Along the north edge, we need three values of which the specific
		   value referenced is the westernmost element. */
		cellPtr->cellData [2][0] = CSvalueGeoconGridFile (filePtr,eleNbr,lastRecord); 
		cellPtr->cellData [2][1] = CSvalueGeoconGridFile (filePtr,(eleNbr + 1L),lastRecord); 
		cellPtr->cellData [2][2] = CSvalueGeoconGridFile (filePtr,(eleNbr + 2L),lastRecord); 
		break;

	case geocnEdgeWest:
		/* Along the west edge, we need three values of which the specific
		   value referenced is the southernmost element. */
		cellPtr->cellData [0][0] = CSvalueGeoconGridFile (filePtr,0L,recNbr); 
		cellPtr->cellData [1][0] = CSvalueGeoconGridFile (filePtr,0L,(recNbr + 1L)); 
		cellPtr->cellData [2][0] = CSvalueGeoconGridFile (filePtr,0L,(recNbr + 2L)); 
		break;
	}

	/* Finish up the creation of the grid cell. */
	cellPtr->isValid  = TRUE;
	cellPtr->cellAge  = 0;
	cellPtr->edge     = edge;
	cellPtr->lngIdx   = eleNbr;
	cellPtr->latIdx   = recNbr;
	cellPtr->swLng    = filePtr->lngMin + (filePtr->deltaLng * (double)(eleNbr));
	cellPtr->swLat    = filePtr->latMin + (filePtr->deltaLat * (double)(recNbr));
	cellPtr->deltaLng = filePtr->deltaLng;
	cellPtr->deltaLat = filePtr->deltaLat;
	CS_stncp (cellPtr->sourceId,filePtr->fileName,sizeof (cellPtr->sourceId));

	status = csGRIDI_ST_OK;
	return status;

error:
	CSinitGeocnGridCell (cellPtr);
	status = csGRIDI_ST_SYSTEM;
	CSreleaseGeoconFile (filePtr);	
	return status;
}
//short CSageGeoconGridCells (struct cs_GeoconFile_* thisPtr)
//{
//	short oldest;
//	short oldestAge;
//	int idndex;
//
//	struct csGeocnGridCell* cellPtr;
//
//	oldest = 0;
//	oldestAge = -1;
//	for (index = 0;index < csGeocnCellCount;index += 1)
//	{
//		cellPtr = thisPtr->cellBuffer [index];
//		if (cellPtr != NULL)
//		{
//			if (cellPtr->cellAge > oldestAge)
//			{
//				oldestAge = cellPtr->cellAge;
//				oldest = index;
//			}
//			if (cellPtr->cellAge < MAXSHORT)
//			{
//				cellPtr->cellAge += 1;
//			}
//		}
//	}
//	return oldest;
//}
void CSinitGeocnGridCell (struct csGeocnGridCell_* thisPtr)
{
	extern double cs_Zero;

	thisPtr->isValid         = FALSE;
	thisPtr->cellAge         = 16384;			/* rather arbitrary very old age */
	thisPtr->edge            = geocnEdgeUnknown;
	thisPtr->lngIdx          = -1;
	thisPtr->latIdx          = -1;
	thisPtr->swLng           = cs_Zero;
	thisPtr->swLat           = cs_Zero;
	thisPtr->deltaLng        = cs_Zero;
	thisPtr->deltaLat        = cs_Zero;
	thisPtr->cellData [0][0] = cs_Zero;
	thisPtr->cellData [0][1] = cs_Zero;
	thisPtr->cellData [0][2] = cs_Zero;
	thisPtr->cellData [1][0] = cs_Zero;
	thisPtr->cellData [1][1] = cs_Zero;
	thisPtr->cellData [1][2] = cs_Zero;
	thisPtr->cellData [2][0] = cs_Zero;
	thisPtr->cellData [2][1] = cs_Zero;
	thisPtr->cellData [2][2] = cs_Zero;
	thisPtr->sourceId [0]    = '\0';
}
/* The following function assumes that the provided longitude has already
   been adjusted to the range convention of GEOCON data files. */
/*lint -esym(752,cs_Half,cs_Huge)   used only on debug builds */
double CScalcGeocnGridCell (struct csGeocnGridCell_* thisPtr,Const double *sourceLL)
{
	extern double cs_Half;
	extern double cs_Huge;
	extern char csErrnam [MAXPATH];

	double rtnValue;

	double sourceDelta [2];
	double rowData [3];

	/* source Delta is actually the distance of the query point from the the
	   implied location of the center point of the grid cell, normalized for
	   the size of the grid in both directions.  Note that the swLng and
	   swLat values are expected to be the lat/long of the southwest corner
	   of the southwest cell of the 3x3 grid cell matrix. 

	   sourceDelta is then the distance from the above described southwest
	   point to the query point in units of the grid cell width,  Grid cell
	   width in this case (i.e. geocon data files o 2012) is always minutes
	   of arc. */
	sourceDelta [LNG] = (sourceLL [LNG] - thisPtr->swLng) / thisPtr->deltaLng;
	sourceDelta [LAT] = (sourceLL [LAT] - thisPtr->swLat) / thisPtr->deltaLat;

#ifdef _DEBUG
	/* The absolute value of these values should be greater or equal to 0.5
	   and less than 1.5 as the grid cell selection algorithm attempts to
	   enhance the effectivness of this algorithm by "moving" the grid.
	   See lines 690 thru 698 in the geocon.f code (circa August 2012). */
	if ((sourceDelta [LNG] <= cs_Half || sourceDelta [LNG] > 1.5) ||
		(sourceDelta [LAT] <= cs_Half || sourceDelta [LAT] > 1.5))
	{
		CS_stncp (csErrnam,"CS_geocn.c:8",sizeof (csErrnam));
		CS_erpt (cs_ISER);
		rtnValue = cs_Huge;
		return rtnValue;
	}
#endif

	switch (thisPtr->edge) {
	
	default:
	case geocnEdgeUnknown:
		CS_stncp (csErrnam,"CS_geocn:9",sizeof (csErrnam));
		CS_erpt (cs_ISER);
		rtnValue = csGEOCON_SHIFT_ERROR;
		break;

	case geocnEdgeNone:
		/* This is the normal case.  Our design has (hopefully) made this case
		   perform very well. */
		rowData [0] = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [0]);
		rowData [1] = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [1]);
		rowData [2] = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [2]);
		rtnValue = CSgeocnQterpOne (sourceDelta [1],rowData);
		break;
	
	case geocnEdgeSouthwest:
		/* The corners are easy due to the way edge cells are setup. */
		rtnValue = thisPtr->cellData [0][0];
		break;
	case geocnEdgeSoutheast:
		/* The corners are easy due to the way edge cells are setup. */
		rtnValue = thisPtr->cellData [0][2];
		break;
	case geocnEdgeNortheast:
		/* The corners are easy due to the way edge cells are setup. */
		rtnValue = thisPtr->cellData [2][2];
		break;
	case geocnEdgeNorthwest:
		/* The corners are easy due to the way edge cells are setup. */
		rtnValue = thisPtr->cellData [2][0];
		break;

	case geocnEdgeSouth:
		/* Interpolate bwetween the three values in center row. */
		rtnValue = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [0]);
		break;
	case geocnEdgeNorth:
		/* Interpolate bwetween the three values in center row. */
		rtnValue = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [2]);
		break;
		
	case geocnEdgeEast:
		/* Interpolate between the values in the center column. */
		rowData [0] = thisPtr->cellData [0][2];
		rowData [1] = thisPtr->cellData [1][2];
		rowData [2] = thisPtr->cellData [2][2];
		rtnValue = CSgeocnQterpOne (sourceDelta [1],rowData);
		break;
	case geocnEdgeWest:
		/* Interpolate between the values in the center column. */
		rowData [0] = thisPtr->cellData [0][0];
		rowData [1] = thisPtr->cellData [1][0];
		rowData [2] = thisPtr->cellData [2][0];
		rtnValue = CSgeocnQterpOne (sourceDelta [1],rowData);
		break;
	}
	return rtnValue;
}
/*lint +esym(752,cs_Half,cs_Huge) */
const char *CSsourceGeocnGridCell (struct csGeocnGridCell_* thisPtr)
{
	const char* rtnValue = NULL;

	if (thisPtr->isValid)
	{
		rtnValue = thisPtr->sourceId;
	}
	return rtnValue;
}
/* Since we added edge p[rocessing, we don't use this function anymore,
   but its good, tested, code. */
double EXP_LVL9 CSgeocnQterp (struct csGeocnGridCell_* thisPtr,double sourceDelta [2])
{
	double result;
	double rowData [3];

	rowData [0] = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [0]);
	rowData [1] = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [1]);
	rowData [2] = CSgeocnQterpOne (sourceDelta [0],thisPtr->cellData [2]);
	result = CSgeocnQterpOne (sourceDelta [1],rowData);
	return result;
}
double EXP_LVL9 CSgeocnQterpOne (double delta,double rowData [3])
{
	extern double cs_One;
	extern double cs_Half;

	double tmp;
	double df0;			/* first delta */
	double df1;			/* second delta */
	double d2f0;		/* second order delta */
	double result;

	df0  = rowData [1] - rowData [0];
	df1  = rowData [2] - rowData [1];
	d2f0 = df1  - df0;
	tmp = cs_Half * delta * (delta - cs_One);
	result = rowData [0] + delta * df0 + tmp * d2f0;
	return result;
}
