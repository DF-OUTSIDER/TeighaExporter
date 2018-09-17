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

/* This code applies to a single transformation file.  There exist six files
   that we know of; two files for each of the three provinces.
   
   The provinces are New Brunswick, Nova Scotia, and Prince Edward Island. For
   each province there exists a file for transforming from NAD27 to ATS77 and
   a separate file for transforming from ATS77 to NAD27.
   
   As to which is the forward, and which is the inverse, is a matter of choice.
   We adopt the convention most ghenerally observed: converting from the older
   datum to the newer datum is considered the forward direction.  Thus, the
   data files which convert from NAD27 to ATS77 will be considered to be the
   forward files, and those that convert from ATS77 to NAD27 will be
   considered the inverse files.

   Note that in the Maritime Provinces, there is no such thing as NAD83.  There
   is no conversion from ATS77 or NAD27 to NAD83.  What you can do is convert
   from NAD27 to ATS77 and then to CSRS (CSRS is called NAD83 in the Maritime
   Provinces).  If for some reason you really really need to convert Maritime
   Province data to "NAD83", what you do is convert ATS77 to NAD27 and then
   convert NAD27 to NAD83 using the NTv2_0.gsb data file.
   
   All that having been said, note that the cs_Ats77_ object respresents a
   single file.  For each such file, an iterative inverse is implemented.
   So, there is no real need for having both files.  However, if both files are
   available and listed in the Geodetic Transformation definition, we need a
   means which will cause the inverse file to be used when present.  This
   "means", whatevere it turns out to be caould also be used to provide a
   forward if only the inverse file is present.  So, things get a little
   screwy here. */

/*lint -esym(613,err_list)   possible use of bull pointer */
int CSats77Q  (struct csGeodeticXfromParmsFile_* fileParms,Const char* dictDir,int err_list [],int list_sz)
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
	strm = CS_fopen (pathBuffer,_STRM_BINRD);
	if (strm != NULL)
	{
		rdCnt = CS_fread (chrBuffer,1,sizeof (chrBuffer),strm);
		CS_fclose (strm);
		strm = NULL;

		if (rdCnt != sizeof (chrBuffer) ||
			chrBuffer [0] != 'T' ||
			chrBuffer [1] != '\0')
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
int CSats77S  (struct cs_GridFile_ *gridFile)
{
	int status;

	struct cs_Ats77_ *ats77Ptr;

	ats77Ptr = CSnewAts77 (gridFile->filePath,gridFile->flags,gridFile->density);
	if (ats77Ptr != NULL)
	{
		/* OK, it initialized OK. */
		ats77Ptr->cnvrgValue = gridFile->cnvrgValue;
		ats77Ptr->errorValue = gridFile->errorValue;
		ats77Ptr->maxIterations = gridFile->maxIterations;
		gridFile->fileObject.Ats77Xfrm = ats77Ptr;

		gridFile->test = (cs_TEST_CAST)CSats77T;
		gridFile->frwrd2D = (cs_FRWRD2D_CAST)CSats77F2;
		gridFile->frwrd3D = (cs_FRWRD3D_CAST)CSats77F3;
		gridFile->invrs2D = (cs_INVRS2D_CAST)CSats77I2;
		gridFile->invrs3D = (cs_INVRS3D_CAST)CSats77I3;
		gridFile->inRange = (cs_INRANGE_CAST)CSats77L;
		gridFile->release = (cs_RELEASE_CAST)CSats77R;
		gridFile->destroy = (cs_DESTROY_CAST)CSats77D;

		status = csGRIDI_ST_OK;
	}
	else
	{
		status = csGRIDI_ST_SYSTEM;
	}
	return status;
}
double CSats77T (struct cs_Ats77_ *ats77,double *ll_src,short direction)
{
	double density;
	
	density = 0.0;
	density = CStestCoverage (&ats77->coverage,ll_src);
	return density;
}
int CSats77F2 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src)
{
	int status;
	double ll_lcl [2];

	ll_lcl [LNG] = ll_src [LNG]; 
	ll_lcl [LAT] = ll_src [LAT]; 

	status = CScalcAts77 (ats77,ll_lcl,ll_lcl);

	if (status >= csGRIDI_ST_OK)
	{
		ll_trg [LNG] = ll_lcl [LNG]; 
		ll_trg [LAT] = ll_lcl [LAT]; 
	}
	else
	{
		ll_trg [LNG] = ll_src [LNG]; 
		ll_trg [LAT] = ll_src [LAT]; 
	}
	ll_trg [HGT] = ll_src [HGT];

	return status;
}
int CSats77F3 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	lcl_ll [LNG] = ll_src [LNG]; 
	lcl_ll [LAT] = ll_src [LAT]; 
	lcl_ll [HGT] = cs_Zero;

	status = CSats77F2 (ats77,lcl_ll,lcl_ll);
	if (status == csGRIDI_ST_OK)
	{
		ll_trg [LNG] = lcl_ll [LNG];
		ll_trg [LAT] = lcl_ll [LAT];
		ll_trg [HGT] = lcl_ll [HGT];
	}
	else
	{
		ll_trg [LNG] = ll_src [LNG];
		ll_trg [LAT] = ll_src [LAT];
		ll_trg [HGT] = ll_src [HGT];
	}
	return status;
}
int CSats77I2 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src)
{
	int status;

	CS_erpt (cs_ATS77_INV);
	status = csGRIDI_ST_SYSTEM;
	return status;
}
int CSats77I3 (struct cs_Ats77_ *ats77,double *ll_trg,Const double *ll_src)
{
	int status;

	CS_erpt (cs_ATS77_INV);
	status = csGRIDI_ST_SYSTEM;
	return status;
}
int CSats77L  (struct cs_Ats77_ *ats77,int cnt,Const double pnts [][3])
{
	return 0;
}
int CSats77R  (struct cs_Ats77_ *ats77)
{
	CSreleaseAts77 (ats77);
	return 0;
}
int CSats77D  (struct cs_Ats77_ *ats77)
{
	CSdeleteAts77 (ats77);
	return 0;
}

struct cs_Ats77_ *CSnewAts77 (Const char *filePath,ulong32_t flags,double density)
{
	extern char csErrnam [];
	extern char cs_DirsepC;
	extern char cs_ExtsepC;

	extern double cs_Huge;
	extern double cs_Mhuge;
	extern double cs_Zero;
	extern double cs_One;

	int idx;
	size_t rdCnt;

	long32_t np;
	long32_t n4;
	long32_t n5;
	long32_t tmpLong;

	double tmpDbl;
	double c1;
	double c2;

	char *vCp;
	const char *cp;
	csFILE *strm = NULL;
	struct cs_Ats77_ *thisPtr = NULL;

	/* Allocate and initialize the object. */
	thisPtr = CS_malc (sizeof (struct cs_Ats77_));
	if (thisPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	thisPtr->coeffs = NULL;

	/* Initialize the structure; mostly just to be safe. */
	thisPtr->coverage.southWest [LNG] = cs_Huge;
	thisPtr->coverage.southWest [LAT] = cs_Huge;
	thisPtr->coverage.northEast [LNG] = cs_Mhuge;
	thisPtr->coverage.northEast [LAT] = cs_Mhuge;
	thisPtr->coverage.density = cs_Zero;
	thisPtr->province = ats77PrvNone;
	thisPtr->direction = ats77DirNone;
	thisPtr->iMax = 0L;
	thisPtr->jMax = 0L;
	thisPtr->polynomialDegree = 0L;
	thisPtr->controlStations  = 0L;
	thisPtr->rui2 = 0.0;
	thisPtr->localOrigin [0] = cs_Zero;
	thisPtr->localOrigin [0] = cs_Zero;
	thisPtr->localOrigin [1] = cs_Zero;
	thisPtr->dataScale [0] = cs_One;
	thisPtr->dataScale [1] = cs_One;
	thisPtr->UO = cs_Zero;
	thisPtr->UO = cs_Zero;
	thisPtr->SU = cs_Zero;
	thisPtr->SV = cs_Zero;
	thisPtr->filePath [0] = '\0';
	thisPtr->fileName [0] = '\0';
	for (idx = 0;idx < 10;idx++)
	{
		CS_iicrt (&thisPtr->ccc [idx],cs_Zero,cs_Zero);
	}

	/* Save the file path we will be reading (hopefully).  Also, extract
	   the name only portion for error reporting purposes. */
	CS_stncp (thisPtr->filePath,filePath,sizeof (thisPtr->filePath));
	cp = strrchr (filePath,cs_DirsepC);
	if (cp != NULL)
	{
		CS_stncp (thisPtr->fileName,(cp + 1),sizeof (thisPtr->fileName));
		vCp = strrchr (thisPtr->fileName,cs_ExtsepC);
		if (vCp != NULL) *vCp = '\0';
	}

	/* Doing this after every read is rather boring.  We'll do it
	   here once and get it done with. */
	CS_stncp (csErrnam,thisPtr->filePath,MAXPATH);

	/* OK, open up the file and read in the important stuff.  Note, we leave
	   all this stuff in memeory, so we don't keep the file open. */
	strm = CS_fopen (thisPtr->filePath,_STRM_BINRD);
	if (strm == NULL)
	{
		CS_erpt (cs_DTC_FILE);
		goto error;
	}

	/* The third and fourth letters indicate the province. */
	if (!CS_strnicmp (&thisPtr->fileName [2],"nb",2))
	{
		/* NB for New Brunswick. */
		thisPtr->province = ats77PrvNB;
		thisPtr->iMax = 76086L;
	}
	else if (!CS_strnicmp (&thisPtr->fileName [2],"ns",2))
	{
		/* NS for Nova Scotia. */
		thisPtr->province = ats77PrvNS;
		thisPtr->iMax = 86141;
	}
	else if (!CS_strnicmp (&thisPtr->fileName [2],"pe",2))
	{
		/* PE for Prince Edward island. */
		thisPtr->province = ats77PrvPE;
		thisPtr->iMax = 18856L;
	}
	else
	{
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Read in the various header stuff, do header calculations, and
	   remember to do the byte swapping as is appropriate.
	   
	   The first thing in the file is junk.  Perhaps something to do
	   with how FORTRAN I/O works. */
	rdCnt = CS_fread (&tmpLong,sizeof (tmpLong),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}

	/* Polynomial degree. */
	rdCnt = CS_fread (&tmpLong,sizeof (tmpLong),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpLong,"l");
	thisPtr->polynomialDegree = tmpLong;
	thisPtr->nf = thisPtr->polynomialDegree + 1;

	/* Control Stations. */
	rdCnt = CS_fread (&tmpLong,sizeof (tmpLong),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpLong,"l");
	np = thisPtr->controlStations = tmpLong;

	/* Direction indicator. */
	rdCnt = CS_fread (&tmpLong,sizeof (tmpLong),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpLong,"l");
	if (tmpLong == 1L)	thisPtr->direction = ats77DirToAts77;
	else if (tmpLong == 2L) thisPtr->direction = ats77DirToNad27;
	else
	{
		CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* RUI, whatever that is. */
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->rui2 = tmpDbl * tmpDbl;
	thisPtr->oprue = cs_One + 0.250;

	/* Local Origin. */
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->localOrigin [1] = tmpDbl;
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->localOrigin [0] = tmpDbl;

	/* UO & VO, whatever they are. */
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->UO = tmpDbl;
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->VO = tmpDbl;

	/* Data Scale. */
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->dataScale [1] = tmpDbl;
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->dataScale [0] = tmpDbl;

	/* SU & SV whatever they are. */
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->SU = tmpDbl;
	rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpDbl,"d");
	thisPtr->SV = tmpDbl;

	/* Skip FORTRAN structure packing stuff. */
	rdCnt = CS_fread (&tmpLong,sizeof (tmpLong),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpLong,"l");
	rdCnt = CS_fread (&tmpLong,sizeof (tmpLong),1,strm);
	if (rdCnt != 1)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	CS_bswap (&tmpLong,"l");

	/* Determine the size of the complex coefficient array, allocate it,
	   and then read in the data.  While the first several thousand of
	   these things are actual complec number pairs, the last thousand
	   or so are straight doubles.  So, we use the brute force approach. */
	n4 = (5 * np) + 1;
	n5 = n4 + (2 * thisPtr->nf);
	thisPtr->jMax = 5 * (np + thisPtr->nf) + (thisPtr->nf * thisPtr->nf);
	thisPtr->coeffs = CS_malc ((unsigned long)n5 * sizeof (double));
	if (thisPtr->coeffs == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	for (idx = 0;idx < n5;idx += 1)
	{
		rdCnt = CS_fread (&tmpDbl,sizeof (tmpDbl),1,strm);
		if (rdCnt != 1)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
		CS_bswap (&tmpDbl,"d");
		*(thisPtr->coeffs + idx) = tmpDbl;
	}

	/* We don't need the file anymore. */
	CS_fclose (strm);
	strm = NULL;

	/* Need to calculate the base coefficients. */
	for (idx = 0;idx < thisPtr->nf;idx += 1)
	{
		c1 = thisPtr->coeffs [(n4 - 1) + (2 * idx)];
		c2 = thisPtr->coeffs [(n4 - 1) + (2 * idx) + 1];
		CS_iicrt (&thisPtr->ccc [idx],c1,c2);
	}

	/* Now we can compute the coverage of this object. */
	thisPtr->coverage.southWest [LNG] = -thisPtr->localOrigin [0] - (thisPtr->oprue / thisPtr->dataScale [0]);
	thisPtr->coverage.southWest [LAT] =  thisPtr->localOrigin [1] - (thisPtr->oprue / thisPtr->dataScale [1]);
	thisPtr->coverage.northEast [LNG] = -thisPtr->localOrigin [0] + (thisPtr->oprue / thisPtr->dataScale [0]);
	thisPtr->coverage.northEast [LAT] =  thisPtr->localOrigin [1] + (thisPtr->oprue / thisPtr->dataScale [1]);
	thisPtr->coverage.density = 0.1;
	if (density > 0.0) thisPtr->coverage.density = density;

	/* That's that. */
	return thisPtr;
error:
	if (strm != NULL)
	{
		CS_fclose (strm);
		strm = NULL;
	}
	CSdeleteAts77 (thisPtr);
	return NULL;
}
void CSdeleteAts77 (struct cs_Ats77_ *thisPtr)
{
	if (thisPtr != NULL)
	{
		if (thisPtr->coeffs != NULL)
		{
			CS_free (thisPtr->coeffs);
			thisPtr->coeffs = NULL;
		}
		CS_free (thisPtr);
	}
	return;
}
void CSreleaseAts77 (struct cs_Ats77_ *thisPtr)
{
	/* For this type of object, release does nothing. */
	return;
}
enum cs_Ats77Dir_ CSdirectionAts77 (Const struct cs_Ats77_ *thisPtr)
{
	return thisPtr->direction;
}
double CStestAts77 (Const struct cs_Ats77_ *thisPtr,enum cs_Ats77Dir_ direction,Const double point [2])
{
	double returnValue = 0.0;
	if (CSdirectionAts77 (thisPtr) == direction)
	{
		if (point [LNG] >= thisPtr->coverage.southWest [LNG] &&
			point [LAT] >= thisPtr->coverage.southWest [LAT] &&
			point [LNG] <  thisPtr->coverage.northEast [LNG] &&
			point [LAT] <  thisPtr->coverage.northEast [LAT])
		{
			returnValue = thisPtr->coverage.density;
		}
	}
	return returnValue;
}
/*
	Calculate the offset for a given geographic position.  The direction is built
	into the object.
*/
int CScalcAts77 (Const struct cs_Ats77_ *thisPtr,double ll_out [2],Const double ll_in [2])
{
	extern double cs_Zero;
	extern double cs_One;

	static const double clrk66Rad = 6378206.400;
	static const double clrk66Esq = 0.00676865799729122;

	static const double ats77Rad = 6378135.000;
	static const double ats77Esq = 0.00669438497508474;

	static const double ats77DeltaX = -15.0;
	static const double ats77DeltaY = 165.0;
	static const double ats77DeltaZ = 175.0;

	int st;
	int idx;
	int status;
	int idxNp1;
	int idxNp3;
	int idxNp5;

	double dd;
	double gu;
	double gv;
	double fu;
	double fv;
	double qq;
	double qw;
	double llDd;
	double expdd;

	double deltaLng;
	double deltaLat;

	double llh [3];
	double xyz [3];

	struct cs_Cmplx_ cz;
	struct cs_Cmplx_ cf;
	struct cs_Cmplx_ zz;
	struct cs_Cmplx_ czz;
	struct cs_Cmplx_ cTmp;
	struct cs_Cmplx_ cTmp1;

	status = csGRIDI_ST_OK;

	/* There are 5 sets of doubles in the A array (among other things).
	   The following are set up as the 'C' index to the first element
	   in each group.  Note, that the first two groups are actually
	   complex numbers, two doubles per index.  The last group
	   is a single set of doubles.  We use this scheme to preserve
	   our sanity as we convert some complicated indexing from
	   FORTRAN where the first element is index 1, and do loops start
	   index 1. */
	idxNp1 = 0;
	idxNp3 = 2 * thisPtr->controlStations;
	idxNp5 = 4 * thisPtr->controlStations;

	/* Convert to the correcdt ellipsoid using a Three Parameter Transformation. */
	llh [0] = ll_in [0];
	llh [1] = ll_in [1];
	llh [2] = cs_Zero;
	if (thisPtr->direction == ats77DirToAts77)
	{
		CS_llhToXyz (xyz,llh,clrk66Rad,clrk66Esq);
		xyz [0] += ats77DeltaX;
		xyz [1] += ats77DeltaY;
		xyz [2] += ats77DeltaZ;
		st = CS_xyzToLlh (llh,xyz,ats77Rad,ats77Esq);
		if (st != 0) status = csGRIDI_ST_COVERAGE;
	}
	else if (thisPtr->direction == ats77DirToNad27)
	{
		CS_llhToXyz (xyz,llh,ats77Rad,ats77Esq);
		xyz [0] -= ats77DeltaX;
		xyz [1] -= ats77DeltaY;
		xyz [2] -= ats77DeltaZ;
		st = CS_xyzToLlh (llh,xyz,clrk66Rad,clrk66Esq);
		if (st != 0) status = csGRIDI_ST_COVERAGE;
	}

	if (status == csGRIDI_ST_OK)
	{
		/* The rest of this stuff expects longitude to be positive west. */
		llh [0] = -llh [0];

		/* Now for the hard stuff.  We apply the complex coefficients.  We
		   work with the results of above, i.e. llh. */
		deltaLng = thisPtr->dataScale [0] * (llh [0] - thisPtr->localOrigin [0]);
		deltaLat = thisPtr->dataScale [1] * (llh [1] - thisPtr->localOrigin [1]);
		llDd = sqrt (deltaLng * deltaLng + deltaLat * deltaLat);
		if (llDd <= thisPtr->oprue)
		{
			/* Appears to be in range.  Compute the result of applying the base
			   polynomial to the input values. */
			CS_iicrt (&cz,deltaLat,deltaLng);
			CS_iicpy (&thisPtr->ccc [0],&cf);
			CS_iicrt (&czz,cs_One,cs_Zero);
			for (idx = 1;idx < thisPtr->nf;idx += 1)
			{
				CS_iimul (&czz,&cz,&czz);
				CS_iimul (&czz,&thisPtr->ccc [idx],&cTmp);
				CS_iiadd (&cf,&cTmp,&cf);
			}
			qq = cs_Zero;
			CS_iicrt (&czz,cs_Zero,cs_Zero);
			if (thisPtr->rui2 != 0.0)
			{
				for (idx = 0;idx < thisPtr->controlStations;idx += 1)
				{
					CS_iicrt (&zz,thisPtr->coeffs [idxNp1 + (2 * idx)],thisPtr->coeffs [idxNp1 + (2 * idx) + 1]);
					CS_iisub (&cz,&zz,&cTmp);
					dd = (cTmp.real * cTmp.real + cTmp.img * cTmp.img) / thisPtr->rui2;
					expdd = cs_Zero;
					if (dd < 172.00) expdd = exp (-dd);
					qw = thisPtr->coeffs [idxNp5 + idx] * expdd;
					if (fabs (qq + qw) >= 1.0E-75)
					{
						CS_iicrt (&cTmp,thisPtr->coeffs [idxNp3 + (2 * idx)],thisPtr->coeffs [idxNp3 + (2 * idx) + 1]);
						CS_iikmul (&cTmp,qw,&cTmp);
						CS_iikmul (&czz,qq,&cTmp1);
						CS_iiadd (&cTmp1,&cTmp,&cTmp);
						CS_iikmul (&cTmp,(1.0 / (qq + qw)),&czz);
					}
					qq += qw;
				}
				fu = cf.real  / thisPtr->SU + thisPtr->UO;
				fv = cf.img   / thisPtr->SV + thisPtr->VO;
				gu = czz.real / thisPtr->SU;
				gv = czz.img  / thisPtr->SV;

				ll_out [0] = -((llh [0] * 3600.00) - fv - gv) / 3600.00;
				ll_out [1] =  ((llh [1] * 3600.00) - fu - gu) / 3600.00;
			}
		}
	}
	else
	{
		ll_out [0] = ll_in [0];
		ll_out [1] = ll_in [1];
	}
	return status;
}
Const char *CSsourceAts77 (Const struct cs_Ats77_ *thisPtr)
{
	return thisPtr->fileName;
}

double CS_Ats77TestFunction ()
{
	extern char cs_Dir [];
	extern char *cs_DirP;

	/* These test values were obtained from execution of the TRANSFORM
	   program itself.  Obviously, the results were transcribed by
	   the authors of CS-MAP.  Thus, there may be an error in the
	   transcription. */
	static const double ll_in [7][2] =
	{
		{-59.8827587  ,46.12309425 },
		{-64.015471706,45.023685822},
		{-60.993470508,45.336660964},
		{-63.940173583,45.888912733},
		{-60.477884683,46.8888983  },
		{-66.132080328,43.951812075},
		{-60.534750144,47.026871825}
	};
	static const double ll_test [7][2] =
	{
		{-59.883498633,46.123061128},
		{-64.016066744,45.023638556},
		{-60.994167014,45.336616058},
		{-63.940773869,45.888867769},
		{-60.478607592,46.888862417},
		{-66.132609239,43.951761983},
		{-60.535471706,47.535471706}
	};

	int idx;
	int stat;

	struct cs_Ats77_ *thisPtr;

	double deltaX;
	double deltaY;
	double errorSum = 0.0;
	
	double ll_out [2];

	/* We only test one province, in one direction, but the code is
	   virtually the same. */
	CS_stcpy (cs_DirP,"TRNS7727.dat");
	thisPtr = CSnewAts77 (cs_Dir,0,0.1);
	if (thisPtr != NULL)
	{
		for (idx = 0;idx < 7;idx += 1)
		{
			stat = CScalcAts77 (thisPtr,ll_out,ll_in [idx]);
			if (stat == 0)
			{
				deltaX = ll_out [0] - ll_test [idx][0];
				deltaY = ll_out [1] - ll_test [idx][1];
				errorSum += sqrt (deltaX * deltaX + deltaY * deltaY); 
			}
			else
			{
				CS_erpt (cs_ISER);
			}
		}
	}
	CSdeleteAts77 (thisPtr);

	return (errorSum / 7.0);
}
