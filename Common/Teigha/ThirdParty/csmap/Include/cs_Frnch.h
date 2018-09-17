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
**           French Text Format Geodetic Interpolation File                  **
**                                                                           **
*******************************************************************************
******************************************************************************/

/* This object is used to convert geographic coordinates from RGF93 to NTF.
   Note, this is the opposite of most other datum shifts.  Most techniques
   convert from the old to the new.  This one converts from the new to the
   old.  Of course, an inverse is provided, but the code will look strange
   as it is different from all the others in this manner.

   The following indicates the form in which the initial header information
   in the binary file is stored in memory.
 */
struct cs_Frnch_
{
	struct csGridCoverage_ coverage;	/* Carries the converage of the
										   internal grid cells. */
	long32_t lngCount;					/* number of nodes (not cells) in the
										   east/west direction. */
	long32_t latCount;					/* number of nodes (not cells) in the
										   north/south direction. */
	double deltaLng;
	double deltaLat;
	double rgf93ERad;					/* Radius of the rgf93 ellipsoid (GRS80). */
	double rgf93ESq;					/* Eccentricity squared, RGF93 ellipsoid. */
	double ntfERad;						/* Radius of the NTF ellipsoid (CLRK 1880). */
	double ntfESq;						/* Eccentricity squared, NTF ellipsoid. */
	char filePath [MAXPATH];			/* We keep this for logging and error
										   reporting. */
	char fileName [32];					/* For logging and error reporting. */
	long32_t *deltaX;					/* an array of longs is malloc'ed and
										   the pointer is stored here.  Note
										   that the array is a lngCount x latCount
										   two dimensional array of longs.*/ 
	long32_t *deltaY;
	long32_t *deltaZ;
										/* For testing purposes only.  These three
										   elements carry the crc16 check code for
										   the three memory arrays which carry the
										   transformation grid.  The idea here is
										   to verify that CS-MAP is not hosing up
										   the memory in these "heap" arrays. */
	unsigned short crcX;
	unsigned short crcY;
	unsigned short crcZ;

	double errorValue;
	double cnvrgValue;
	short maxIterations;
};

struct cs_Frnch_* CSnewFrnch (Const char *filePath,long32_t bufferSize,ulong32_t flags,double density);
void CSinitializeFrnchObj (struct cs_Frnch_ *thisPtr);
int CSinitFrnch (struct cs_Frnch_* thisPtr,Const char *filePath,long32_t bufferSize,ulong32_t flags,double density);
void CSreleaseFrnch (struct cs_Frnch_* thisPtr);
void CSdeleteFrnch (struct cs_Frnch_* thisPtr);
Const char* CSpathFrnch (Const struct cs_Frnch_* thisPtr);
double CStestFrnch (struct cs_Frnch_* thisPtr,Const double location [2]);
int CScalcRgf2NtfDeltas (struct cs_Frnch_* thisPtr,double* deltaX,double* deltaY,double* deltaZ,Const double* ll_rgf93);
int CScalcRgfToNtf (struct cs_Frnch_* thisPtr,double* llNtf,Const double* llRgf93);
int CScalcNtfToRgf (struct cs_Frnch_* thisPtr,double* llRgf93,Const double* llNtf);



