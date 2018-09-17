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
**           ATS77 to NAD27 Datum Shift Object (aka TRANSFORM)               **
**                                                                           **
*******************************************************************************
******************************************************************************/

/* This object enables multiple grid shift files to convert from ATS77 to
   NAD27 and vice versa.  There are six files, two for each province.  One
   file provides for the forward (NAD27 --> ATS77) conversion, and the other
   implements the inverse.

   This object is, essentially, an implementation of the TRANSFORM program
   used in the Maritime provinces for many years now.
*/

enum cs_Ats77Prov_ {	ats77PrvNone = 0,
						ats77PrvNB,
						ats77PrvNS,
						ats77PrvPE
				   };
enum cs_Ats77Dir_  {	ats77DirNone = 0,
						ats77DirToAts77,
						ats77DirToNad27
				   };
struct cs_Ats77_
{
	/* Most of this comes from the header of the data file.  Note that the
	   the numbers in the file are in little endian byte order.  Since this
	   complies with the CS-MAP standard, we can simply run all binary
	   values extracted through CS_bswap. */
	struct csGridCoverage_ coverage;
	enum cs_Ats77Prov_ province;			/* There are two files for each province. */
	enum cs_Ats77Dir_ direction;			/* There is a forward and an inverse for each province. */	
	long32_t iMax;
	long32_t jMax;
	long32_t nf;
	long32_t polynomialDegree;
	long32_t controlStations;
	long32_t coefficientCount;
	double oprue;
	double rui2;						/* don't really know what this is;
									       but it's pretty important. */
	double localOrigin [2];
	double dataScale [2];
	double UO;
	double VO;
	double SU;
	double SV;
	double *coeffs;					/* An array of the complex coefficients which
									   are read from the file.  The number is
									   variable, depending upon the dgree of the
									   polynomial, and the number of coefficients. */
	struct cs_Cmplx_ ccc [10];
	
	double errorValue;
	double cnvrgValue;

	short maxIterations;

	char filePath [MAXPATH];		/* Complete path to the data file. */
	char fileName [32];				/* File name of the data file. */
};

struct cs_Ats77_ *CSnewAts77 (Const char *filePath,ulong32_t flags,double density);
void CSreleaseAts77 (struct cs_Ats77_ *thisPtr);
void CSdeleteAts77 (struct cs_Ats77_ *thisPtr);
enum cs_Ats77Dir_ CSdirectionAts77 (Const struct cs_Ats77_ *thisPtr);
double CStestAts77 (Const struct cs_Ats77_ *thisPtr,enum cs_Ats77Dir_ direction,Const double point [2]);
int CScalcAts77 (Const struct cs_Ats77_ *thisPtr,double ll_out [2],Const double ll_in [2]);
Const char *CSsourceAts77 (Const struct cs_Ats77_ *thisPtr);
double CS_Ats77TestFunction (void);

