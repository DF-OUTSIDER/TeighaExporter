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

/******************************************************************************
*******************************************************************************
**     General  (east longitude is positive) Coverage Object                 **
*******************************************************************************
******************************************************************************/

void CSinitCoverage (struct csGridCoverage_* thisPtr)
{
	extern double cs_Zero;
	extern double cs_K90;
	extern double cs_Km90;
	extern double cs_K180;
	extern double cs_Km180;

	/* Set the coverage to something that won't match anything. */
	thisPtr->southWest [LNG] = cs_K180;
	thisPtr->southWest [LAT] = cs_K90;
	thisPtr->northEast [LNG] = cs_Km180;
	thisPtr->northEast [LAT] = cs_Km90;
	thisPtr->density = cs_Zero;
}
double CStestCoverage (struct csGridCoverage_* thisPtr,Const double point [2])
{
	extern double cs_Zero;
	extern double cs_LlNoise;			/* 1.0E-12 */

	double neLng;
	double neLat;
	double returnValue;

	/* Return value is the density value used to select a specific source in
	   the case of overlap which is rare, but possible.  A zero return value
	   indicates that the current coverage window does not include the
	   provided location.

	   The coverage is inclusive on the south and west, exclusive on the
	   north and east.  This is standard for all grid interpolation files
	   except the Canadian National Transformation (version 1 and 2).  The
	   north and east edges of the typical grid file are not part of the
	   coverage of such a file. */
	neLng  = thisPtr->northEast [LNG];
	neLng += (neLng >= 0.0) ? -cs_LlNoise : cs_LlNoise;
	neLat  = thisPtr->northEast [LAT];
	neLat += (neLat >= 0.0) ? -cs_LlNoise : cs_LlNoise;
	
	returnValue = cs_Zero;
	if (point [LNG] >= thisPtr->southWest [LNG] &&
		point [LAT] >= thisPtr->southWest [LAT] &&
		point [LNG] <  neLng &&
		point [LAT] <  neLat)
	{
		returnValue = thisPtr->density;
	}
	return returnValue;
}
void CSsetCoverage (struct csGridCoverage_* thisPtr,Const double* swLL,Const double* neLL)
{
	extern double cs_Zero;

	thisPtr->southWest [LNG] = swLL [LNG];
	thisPtr->southWest [LAT] = swLL [LAT];
	thisPtr->northEast [LNG] = neLL [LNG];
	thisPtr->northEast [LAT] = neLL [LAT];
	thisPtr->density = cs_Zero;
}


/******************************************************************************
*******************************************************************************
**                General  Grid  Cell  Implementation                        **
*******************************************************************************
******************************************************************************/
/*
	Again, this is an implementation of a grid cell for typical grid
	interpolation.  A separate variation exists for the Canadian National
	Transformation (verrsiona 1 and 2).
*/
void CSinitGridCell (struct csGridCell_* thisPtr)
{
	extern double cs_Zero;

	CSinitCoverage (&thisPtr->coverage);
	thisPtr->deltaLng = cs_Zero;
	thisPtr->deltaLat = cs_Zero;
	thisPtr->currentAA = cs_Zero;
	thisPtr->currentBB = cs_Zero;
	thisPtr->currentCC = cs_Zero;
	thisPtr->currentDD = cs_Zero;
	thisPtr->sourceId [0] = '\0';
}
double CScalcGridCell (struct csGridCell_* thisPtr,Const double *sourceLL)
{
	/* This function performs the interpolation of the grid cell, and
	   returns the result.  Type of result depends upon the application.
	   Calculation algorithm is the same for most all applications. */

	double gridDeltaLng;
	double gridDeltaLat;
	double returnValue;

	gridDeltaLng  = (sourceLL [LNG] - thisPtr->coverage.southWest [LNG]) / thisPtr->deltaLng;
	gridDeltaLat  = (sourceLL [LAT] - thisPtr->coverage.southWest [LAT]) / thisPtr->deltaLat;
	returnValue = thisPtr->currentAA + 
				  thisPtr->currentBB * gridDeltaLng +
				  thisPtr->currentCC * gridDeltaLat +
				  thisPtr->currentDD * gridDeltaLng * gridDeltaLat;

	/* Note return value for datum shift grid files is usually in seconds.
	   However, this varies from application to application. */
	return returnValue;	
}
const char *CSsourceGridCell (struct csGridCell_* thisPtr)
{
	return thisPtr->sourceId;
}
