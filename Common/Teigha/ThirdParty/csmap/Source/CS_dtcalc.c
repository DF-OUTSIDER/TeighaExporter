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

/*		       * * * * R E M A R K S * * * *

	Both forward and inverse calculations are coded for each transformation
	technique.  An initialize function is also included.  Each of these
	functions initialize a parameter structure given a cs_Datum_ structure
	for the source datum and the target datum.  These initialize functions
	do, essentially, what a constructor would do in the C++ environment.

	Be careful, and note that the initialization functions assume two
	definitions which use the same type of transformations.  This is done
	to support the very small number (one as I write) of datum trasnformations
	which bypass WGS84 and go direcrtly from one datum to another.  Note the
	the DMA Mreg initialize function has a target datum of WGS84 hard coded
	into it.
	
	Generalized calculation of geocentric cartesian coordinates,
	including the inverse, are also included.
*/
/*lint -e788 */		/* enumeration value is not included in a switch */

#include "cs_map.h"

/******************************************************************************
	CS_llhToXyz (xyz [3],llh [3],e_rad,e_sq);

	double xyz [3];				the computed results are returned here.  Units
								are the same as the equatorial radius. Results
								are relative to the center of the earth.
	double llh [3];				the longitude, latitude, and ellipsoid height
								of the point to be converted.  Height must be
								in same units as the equatorial radius.
	double e_rad;				equatorial radius of the ellipsoid/shpere.
								Units ar eusually meters, but this is not
								required.
	double e_sq;				eccentricity squared of the ellipsoid.  Use
								zero for a sphere.

	The resulting coordinates are sometimes referred to as Helmert coordinates.
*/
void EXP_LVL5 CS_llhToXyz (double xyz [3],Const double llh [3],double e_rad,double e_sq)
{
	extern double cs_One;				/* 1.0 */
	extern double cs_Degree;			/* 0.017.... */

	double lng, lat, hhh;
	double vv;
	double cos_lat, sin_lat;

	lng = llh [LNG] * cs_Degree;
	lat = llh [LAT] * cs_Degree;
	hhh = llh [HGT];

	/* Compute the Helmert coordinates of the point. */
	cos_lat = cos (lat);
	sin_lat = sin (lat);
	vv = e_rad / sqrt (cs_One - (e_sq * sin_lat * sin_lat));
	xyz [XX] = (vv + hhh) * cos_lat * cos (lng);
	xyz [YY] = (vv + hhh) * cos_lat * sin (lng);
	xyz [ZZ] = (vv * (cs_One - e_sq) + hhh) * sin_lat;

	return;
}

/******************************************************************************
	status = CS_llhToXyz (xyz [3],llh [3],e_rad,e_sq);

	double llh [3];				the longitude, latitude, and ellipsoid height
								of the point are returned here.  Ellipsoid
								height is in the same units as the equatorial
								radius.
	double xyz [3];				the point to be converted.  Units must be the
								same as the equatorial radius provided.
	double e_rad;				equatorial radius of the ellipsoid/shpere.
								Units of this parameter and xyz must be the
								same.
	double e_sq;				eccentricity squared of the ellipsoid.  Use
								zero for a sphere.
	int status;					returns zero on success, -1 on failure. Only
								possible cause of failure is the failure of
								iterative solution to converge.  This is not
								possible if the arguments are reasonable.

	The source coordinates are sometimes referred to as Helmert coordinates.
*/
int EXP_LVL5 CS_xyzToLlh (double llh [3],Const double xyz [3],double e_rad,double e_sq)
{
	extern double cs_One;				/* 1.0 */
	extern double cs_Radian;			/* 57.29... */

	int ii;
	int status;
	static int maxIteration = 9;

	double sinLat;
	double lat, prevLat;
	double vv,denom;
	static double smallLat = 5.0E-12;

	status = 0;

	/* Longitude is easy. */
	llh [LNG] = atan2 (xyz [YY],xyz [XX]) * cs_Radian;

	/* Latitude is a bit more difficult. An iterative process is required.
	   We need a basic constant and an approximate latitude to start with.
	   We start with the latitude if the ellipsoid was a sphere.
	   
	   This initial guess is important for performance reasons.  I.e. the
	   closer we can get to the real thing, the fewer iterations of the
	   loop below are required. */
	denom = sqrt (xyz [XX] * xyz [XX] + xyz [YY] * xyz [YY]);
	lat = atan (xyz [ZZ] / denom);

	/* Iterate to the final solution. */
	for (ii = 0;ii < maxIteration;ii++)
	{
		prevLat = lat;
		sinLat = sin (lat);
		vv = e_rad / sqrt (cs_One - (e_sq * sinLat * sinLat));
		lat = (xyz [ZZ] + (e_sq * vv * sinLat)) / denom;
		lat = atan (lat);
		if (fabs (lat - prevLat) < smallLat) break;
	}
	if (ii >= maxIteration)
	{
		CS_erpt (cs_XYZ_ITR);
		status = -1;
	}
	llh [LAT] = lat * cs_Radian;

	/* Now for the new elevation above the ellipsoid.  There are
	   two different formulas.  It was suggested by the source
	   of all of this that the second be used at high latitudes.
	   They didn't say what they consider high.  We will assume
	   a radian value of one is high.  I believe we just need
	   to stay away from the indeterminant cases where the
	   sine and cosine go to zero since we are dividing by
	   them here (i.e. secant and cosecant). */
	sinLat = sin (lat);
	vv = e_rad / sqrt (cs_One - (e_sq * sinLat * sinLat));
	if (fabs (lat) <= cs_One)
	{
		llh [HGT] = (denom / cos (lat)) - vv;
	}
	else
	{
		llh [HGT] = (xyz [ZZ] / sinLat) - ((cs_One - e_sq) * vv);
	}

	/* We return the calculated value even if the returned status is not
	   zero.  Much of the rest of this module relies on this behavior. */
	return status;
}
