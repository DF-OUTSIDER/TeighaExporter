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

/**********************************************************************
**	angl_out = CS_adj2pi (angl_in);
**
**	double angl_in;		the angle to be adjusted, in degrees.
**	double angl_out;	the adjusted angle, in degrees.
**
**
**	Designed expressly for adjusting angles for the infamous
**	-180 crack.  Algortithm uses repeated additions and/or
**	subtractions.  Fast for the normal case, but if the input
**	is way out of line, it could be a long haul.
**
**	NOTE: the value -180 is returned as +180.
**********************************************************************/

double EXP_LVL5 CS_adj180 (double az_in)
{
	extern double cs_K180;			/*  180.0 */
	extern double cs_Km180;			/* -180.0 */
	extern double cs_K360;			/*  360.0 */

	if (fabs (az_in) >= cs_K180)
	{
		if (az_in != cs_K180)			/*lint !e777 */
		{
			az_in = fmod (az_in,cs_K360);
			if (az_in <= cs_Km180) az_in += cs_K360;
			if (az_in >  cs_K180)  az_in -= cs_K360;
		}
	}
	return (az_in);
}

/**********************************************************************
**	angl_out = CS_adj90 (angl_in);
**
**	double angl_in;		the angle to be adjusted, in degrees.
**	double angl_out;	the adjusted angle, in degrees.
**
**	Normalizes an angle, in degrees, to be greater than -90, and less
**	than or equal to 90.
**
**	NOTE: the value -90 is returned as +90.
**********************************************************************/

double EXP_LVL5 CS_adj90 (double az_in)
{
	extern double cs_K90;			/*  90.0 */
	extern double cs_Km90;			/* -90.0 */
	extern double cs_K180;			/*  180.0 */

	if (fabs (az_in) >= cs_K90)
	{
		if (az_in != cs_K90)		/*lint !e777 */
		{
			az_in = fmod (az_in,cs_K180);
			if (az_in <= cs_Km90) az_in += cs_K180;
			if (az_in >  cs_K90)  az_in -= cs_K180;
		}
	}
	return (az_in);
}

/**********************************************************************
**	angl_out = CS_adj270 (angl_in);
**
**	double angl_in;		the angle to be adjusted, in degrees.
**	double angl_out;	the adjusted angle, in degrees.
**
**	Normalizes an angle, in degrees, to be greater than -270, and less
**	than or equal to 270.
**********************************************************************/

double EXP_LVL5 CS_adj270 (double az_in)
{
	extern double cs_K270;			/*  270.0 */
	extern double cs_Km270;			/* -270.0 */
	extern double cs_K360;			/*  540.0 */

	if (fabs (az_in) >= cs_K270)
	{
		if (az_in != cs_K270)		/*lint !e777 */
		{
			az_in = fmod (az_in,cs_K360);
			if (az_in <= cs_Km270) az_in += cs_K360;
			if (az_in >  cs_K270)  az_in -= cs_K360;
		}
	}
	return (az_in);
}
/**********************************************************************
**
**	az_out = CS_adj1pi (az_in);
**
**	double az_in;		the angle to be adjusted, in radians.
**	double az_out;		the adjusted angle, in radians.
**
**	Normalizes an angle to be greater than -pi/2 and less than or equal
**	to +pi/2.
**
**********************************************************************/

double EXP_LVL5 CS_adj1pi (double az_in)
{
	extern double cs_Pi;
	extern double cs_Mpi_o_2;
	extern double cs_Pi_o_2;

	if (fabs (az_in) > cs_Pi_o_2)
	{
		az_in = fmod (az_in,cs_Pi);
		if (az_in <= cs_Mpi_o_2) az_in += cs_Pi;
		if (az_in >  cs_Pi_o_2)  az_in -= cs_Pi;
	}
	return (az_in);
}

/**********************************************************************
**	az_out = CS_adj2pi  (az_in);
**	az_out = CS_adj2piI (az_in);
**
**	double az_in;		the angle to be adjusted, in radians.
**	double az_out;		the adjusted angle, in radians.
**
**	The difference between the two functions is the return value.
**	Read CS_adj2piI and Adjust to 2 PI INCLUSIVE.
**
**	CS_adj2pi   ==>	-pi <  az_out <= +pi
**	CS_adj2piI  ==>	-pi <= az_out <= +pi
**
**	That is, under no circumstances will CS_adj2pi return a value
**	of -pi.  CS_adj2piI will return -pi when appropriate.
**********************************************************************/

double EXP_LVL5 CS_adj2pi (double az_in)
{
	extern double cs_Pi;
	extern double cs_Mpi;
	extern double cs_Two_pi;

	if (fabs (az_in) >= cs_Pi)
	{
		if (az_in != cs_Pi)			/*lint !e777 */
		{
			az_in = fmod (az_in,cs_Two_pi);
			if (az_in <= cs_Mpi) az_in += cs_Two_pi;
			if (az_in > cs_Pi)   az_in -= cs_Two_pi;
		}
	}
	return (az_in);
}

double EXP_LVL5 CS_adj2piI (double az_in)
{
	extern double cs_Pi;
	extern double cs_Mpi;
	extern double cs_Two_pi;

	if (fabs (az_in) > cs_Pi)
	{
		az_in = fmod (az_in,cs_Two_pi);
		if (az_in > cs_Pi)  az_in -= cs_Two_pi;
		if (az_in < cs_Mpi) az_in += cs_Two_pi;
	}
	return (az_in);
}
/**********************************************************************
**	status = CS_adjll (ll);
**
**	double ll [2];		the latitude and longitude to be adjusted,
**						in DEGREES.
**	int status;			return TRUE if an adjustment was required,
**						else returns FALSE.
**
**	The longitude of the result is always between -270 and
**	+270 inclusive.  This enables CS-MAP to support a map
**	which has overlap at the eastern and western edges.  Of course,
**	the ability to to that depends upon the projection involved.
**	However, the standard enforced by this module is general,
**	and therefore, needs to support the most general case.
**
**	Longitudes outside of the range described above are normalized
**	to be: -180 < longitude <= +180.
**
**	Latitudes are normalized to be in the range of -90 to +90
**	inclusive.  Normalization is used to bring out of range numbers
**	into range. This normalization does not affect the longitude in
**	any way.
**
**	THIS FUNCTION REQUIRES THE LAT/LONG TO BE IN DEGREES.
**********************************************************************/

int EXP_LVL5 CS_adjll (double ll [2])
{
	extern double cs_Two;				/*    2.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K270;				/*  27.0 */
	extern double cs_K360;				/*  360.0 */

	int status = FALSE;

	/* To keep performance up, we check to see if we have to
	   do anything.  99.99% of the time, we don't have to do
	   anything.  Thus, we are better off with a simple check
	   before doing and heavy duty arithmatic. */

	if (fabs (ll [LAT]) > cs_K90)
	{
		status = TRUE;

		/* The latitude needs some adjustment.  We desire a
		   reflection type of normalization: i.e. 91 degrees
		   converts to 89.  Fmod handles any number, rather
		   quickly. */

		ll [LAT] = fmod (ll [LAT],cs_K180);
		if (ll [LAT] > cs_K90)
		{
			ll [LAT] += cs_Two * (cs_K90 - ll [LAT]);
		}
		else if (ll [LAT] < cs_Km90)
		{
			ll [LAT] += cs_Two * (cs_Km90 - ll [LAT]);
		}
		status = TRUE;
	}

	/* Check the longitude. */

	if (fabs (ll [LNG]) > cs_K270)
	{
		status = TRUE;

		/* The longitude needs adjustment.  Since its outside
		   of our rather liberal range, we normalize to
		   +- 180 which is the normal range.  Fmod will not
		   wrap around zero as we desire, so we use it to
		   normalize to 360, then the standard technique
		   to wrap around normalize. */

		ll [LNG] = fmod (ll [LNG],cs_K360);
		while (ll [LNG] <= cs_Km180)
		{
			ll [LNG] += cs_K360;
		}
		while (ll [LNG] > cs_K180)
		{
			ll [LNG] -= cs_K360;
		}
	}
	return (status);
}
/**********************************************************************
**	alpha = CS_azsphr (ll0,ll1);
**
**	double ll0 [2];		the point at which the azimuth angle is based,
**						in degrees, ll0 [0] = lng, ll0 [1] = lat.
**	double ll1 [2];		the point to which the azimuth is computed,
**						in degrees, ll1 [0] = lng, ll1 [1] = lat.
**	double alpha;		azimuth, in degrees east of north, from ll0
**						to ll1.
**
**	Computes the azimuth, in degrees east of north, from ll0 to
**	ll1 on a sphere.  Note, we don't have to know the size of
**	the sphere for this calculation.
**********************************************************************/

double EXP_LVL1 CS_azsphr (Const double ll0 [2],Const double ll1 [2])
{
	extern double cs_Degree;		/* PI / 180.0 */
	extern double cs_Radian;		/*  180.0 / PI */

	double az;
	double lng0, lng1;
	double lat0, lat1;
	double del_lng;
	double sin_del_lng, cos_del_lng;
	double sin_lat0, sin_lat1;
	double cos_lat0, cos_lat1;

	double tmp1;
	double tmp2;

	lng0 = ll0 [0] * cs_Degree;
	lat0 = ll0 [1] * cs_Degree;
	lng1 = ll1 [0] * cs_Degree;
	lat1 = ll1 [1] * cs_Degree;

	del_lng = CS_adj2pi (lng1 - lng0);
	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);

	sin_lat1 = sin (lat1);
	sin_lat0 = sin (lat0);
	cos_lat0 = cos (lat0);
	cos_lat1 = cos (lat1);

	tmp1 = cos_lat1 * sin_del_lng;
	tmp2 = cos_lat0 * sin_lat1;
	tmp2 -= sin_lat0 * cos_lat1 * cos_del_lng;
	if (fabs (tmp2) > 1.0E-08)
	{
		az = atan2 (tmp1,tmp2);
	}
	else
	{
		/* The latitudes are the same and delta longitude
		   is either zero or 180 (i.e. cos (del_lng) == 1). */

		az = del_lng;
	}
	return (az * cs_Radian);
}
/**********************************************************************
**	cc = CSccsphrD (ll0,ll1);
**	cc = CSccsphrR (ll0,ll1);
**
**	double ll0 [2];		the first point, ll0 [0] = lng, ll0 [1] = lat.
**	double ll1 [2];		the second point.
**	double cc;			the angular distance between the two points is
**						returned.
**
**	FOR CSccsphrD, the units are degrees;
**	FOR CSccsphrR, the units are radians.
**
**	Note, we don't have to know the size of the sphere for this
**	calculation.
**
**	There are simpler, faster, formulas which are not quite as
**	precise as the one used here.  The formula used here retains
**	its accuracy for results close to zero and results close to
**	180 degrees.
**
**	See Synder, Page 30.  We use ll0 for lamba0 and phi0, we
**	use ll1 for lamba and phi.
**********************************************************************/

double EXP_LVL5 CSccsphrR (Const double ll0 [2],Const double ll1 [2])
{
	extern double cs_Zero;			/* 0.0 */
	extern double cs_Half;			/* 0.5 */
	extern double cs_Two;			/* 2.0 */

	double cc;

	double sin_del_lng;
	double sin_del_lat;
	double cos_lat0;
	double cos_lat1;
	double tmp3;

	sin_del_lng = sin (CS_adj2pi (ll1 [LNG] - ll0 [LNG]) * cs_Half);
	sin_del_lat = sin (CS_adj1pi (ll1 [LAT] - ll0 [LAT]) * cs_Half);
	cos_lat0 = cos (CS_adj1pi (ll0 [LAT]));
	cos_lat1 = cos (CS_adj1pi (ll1 [LAT]));
	tmp3 = (sin_del_lat * sin_del_lat) +
		   (cos_lat0 * cos_lat1) * (sin_del_lng * sin_del_lng);
	if (tmp3 > 0.0)
	{
		cc = cs_Two * asin (sqrt (tmp3));
	}
	else
	{
		cc = cs_Zero;
	}
	return (cc);
}


double EXP_LVL5 CSccsphrD (Const double ll0 [2],Const double ll1 [2])
{
	extern double cs_Degree;		/* PI / 180.0 */
	extern double cs_Radian;		/*  180.0 / PI */

	double my_ll0 [2];
	double my_ll1 [2];

	my_ll0 [LNG] = ll0 [LNG] * cs_Degree;
	my_ll0 [LAT] = ll0 [LAT] * cs_Degree;
	my_ll1 [LNG] = ll1 [LNG] * cs_Degree;
	my_ll1 [LAT] = ll1 [LAT] * cs_Degree;

	return (CSccsphrR (my_ll0,my_ll1) * cs_Radian);
}

/**********************************************************************
**	status = CS_llazdd (e_rad,e_sq,ll_from,az,dist,ll_to);
**
**	double e_rad;		equatorial radius of the ellipsoid which
**						is to be used.
**	double e_sq;		ellipsoid eccentricity, squared.
**	double ll_from [2];	latitude and longitude of the initial station,
**						in degrees.
**	double az;			the azimuth of the leg, in degrees east of north.
**	double dist;		the length of the leg in same units as e_rad.
**	double ll_to [2];	latitude and longitude of the resulting station
**						is returned here.
**	int status;			returns zero for success; returns -1 if the
**						Newton/Rhapson iteration fails to converge.
**
**	The initial station and azimuth values are used only in
**	their trigonometric form (i.e. sine and cosine of these
**	values).  Therefore, even absurdly bogus values for these
**	inputs should not cause a problem. An absurd value for the
**	dist argument may, however, cause problems.
**
**	The following was extracted from the FORTRAN code from which
**	this 'C' rendition was extracted:
**
**	SOLUTION OF THE GEODETIC DIRECT PROBLEM AFTER T.VINCENTY
**	MODIFIED RAINSFORD'S METHOD WITH HELMERT'S ELLIPTICAL TERMS
**	EFFECTIVE IN ANY AZIMUTH AND AT ANY DISTANCE SHORT OF ANTIPODAL
**
**	This implies that this function is not likely to work if the
**	dist argument is anti-podal. Since there is no way to
**	easily calculate the anti-podal distance, we suggest that
**	users apply a large, but less than pi times the polar
**	radius as a limit.  We do not apply such a test in this
**	function.
**********************************************************************/

int EXP_LVL1 CS_azddll (double e_rad,
			double e_sq,
			Const double ll_from [2],
			double az,
			double dist,
			double ll_to [2])

{
	extern double cs_Pi;			/* 3.14159.... */
	extern double cs_Degree;		/* 1.0 / 57.29577... */
	extern double cs_Radian;		/* 57.29577... */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_Fourth;		/* 0.25*/
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_Three;			/* 3.0 */
	extern double cs_Four;			/* 4.0 */

	int status;
	int itr_cnt;

	double rr, rr2;
	double sf, cf;
	double sy, cy;
	double tu, cu, su;
	double sa, cz;
	double c2a;
	double xx, yy;
	double cc, dd, ee;
	double tmp;
	double faz, baz;
	double flat;

	double from [2];

	status = 0;

	/* Convert to radians. */

	from [LNG] = CS_adj2pi (ll_from [LNG] * cs_Degree);
	from [LAT] = CS_adj1pi (ll_from [LAT] * cs_Degree);
	faz = az * cs_Degree;

	/* CS-MAP is driven by eccentricity, this guy works off
	   flattening */

	flat = cs_One - sqrt (cs_One - e_sq);

	/* The following is a conversion from FORTRAN of the
	   Subroutine named DIRCT1 in the FOWARD program. I
	   really have no idea as to what is going on here,
	   ither then there is what appears to be a Newton
	   Rhapson iteration.

	   We have followed the variable nomenclature used in
	   the FORWARD program with two exceptions:

	   1) we have used lower case,
	   2) we have doubled up all single letter variable names.
	*/

	rr = cs_One - flat;
	rr2 = rr * rr;
	tu = rr * sin (from [LAT]) / cos (from [LAT]);
	sf = sin (faz);
	cf = cos (faz);
	if (fabs (tu) >= 1.0E-13 || fabs (cf) >= 1.0E-13)
	{
		baz = cs_Two * atan2 (tu,cf);
	}
	else
	{
		baz = cs_Zero;
	}
	cu = cs_One / sqrt (cs_One + tu * tu);
	su = tu * cu;
	sa = cu * sf;
	c2a = cs_One - sa * sa;

	tmp = c2a * (cs_One / rr2 - cs_One) + cs_One;
	xx = sqrt (tmp) + cs_One;
	xx = (xx - cs_Two) / xx;

	cc = cs_One - xx;
	cc = (cs_Fourth * xx * xx + cs_One) / cc;

	dd = (0.375 * xx * xx - cs_One) * xx;
	tu = dist /  rr / e_rad / cc;
	yy = tu;

	itr_cnt = 20;
	do
	{
		sy = sin (yy);
		cy = cos (yy);
		cz = cos (baz + yy);
		ee = cz * cz * cs_Two - cs_One;
		cc = yy;
		xx = ee * cy;
		yy = ee + ee - cs_One;
		tmp = cs_Four * sy * sy - cs_Three;
		tmp *= yy * cz * dd * (1.0 / 6.0);
		tmp += xx;
		tmp *= dd * cs_Fourth;
		tmp -= cz;
		tmp *= sy * dd;
		yy = tmp + tu;
	} while (fabs (yy - cc) > 0.5E-13 && itr_cnt-- > 0);
	if (itr_cnt <= 0) status = -1;

	baz = (cu * cy * cf) - (su * sy);
	cc = rr * sqrt ((sa * sa) + (baz * baz));
	dd = (su * cy) + (cu * sy * cf);
	ll_to [LAT] = atan2 (dd,cc);

	cc = (cu * cy) - (su * sy * cf);
	xx = atan2 (sy * sf,cc);
	tmp = (cs_Four - cs_Three * c2a) * flat + cs_Four;
	cc = tmp * c2a * flat / 16.0;
	tmp = (ee * cy * cc) + cz;
	tmp *= sy * cc;
	dd = (tmp + yy) * sa;
	tmp = (cs_One - cc) * dd * flat;
	ll_to [LNG] = from [LNG] + xx - tmp;
	baz = atan2 (sa,baz) + cs_Pi;

	/* Convert back to degrees. */

	ll_to [LNG] = CS_adj2pi (ll_to [LNG]) * cs_Radian;
	ll_to [LAT] *= cs_Radian;

	return (status);
}

/**********************************************************************
**	az = CS_llazdd (e_rad,e_sq,ll_from,ll_to,dist);
**
**	double e_rad;		equatorial radius of the ellipsoid which
**						is to be used.
**	double e_sq;		ellipsoid eccentricity, squared.
**	double ll_from [2];	latitude and longitude of the from point
**						in degrees.
**	double ll_to [2];	latitude and longitude of the to point in
**						degrees.
**	double dist;		returns the geodetic distance bewteen the
**						two points here, in same units as the radius
**						is given, usually meters.
**	double az;			returns the geodetic azimuth, in degrees
**						east of north, of the to point relative to the
**						to point.
**
**	The algorithms used here essentially are based on a sphere
**	using the Gaussian Curvative as the radius.  This algorithm
**	appears to be more precise than the more sophisticated
**	algorithms for small arcs (i.e. less than 10 kilometers).
**	The choices made in the design of this algorithm were based
**	on the "small arc" ideal.
**********************************************************************/

static double CSllazdd (double e_rad,double e_sq,double from [2],double to [2],double *dist);

double EXP_LVL1 CS_llazdd (	double e_rad,
							double e_sq,
							Const double ll_from [2],
							Const double ll_to [2],
							double *dist)

{
	extern double cs_Pi;			/* 3.14159.... */
	extern double cs_Mpi;			/* -3.14159.... */
	extern double cs_Pi_o_2;		/* 1.570796.... */
	extern double cs_Degree;		/* 1.0 / 57.29577... */
	extern double cs_Radian;		/* 57.29577... */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_Half;			/* 0.5 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_AnglTest;		/* 0.001 arc seconds, in radians */
	extern double cs_NPTest;		/* 0.001 seconds of arc short
									   of the north pole, in radians. */
	extern double cs_SPTest;		/* 0.001 seconds of arc short of
									   the south pole, in radians. */

	double az;
	double mm, mp;
	double del_lng;
	double del_lat;

	double cc;				/* Angular distance between the
							   two points on a sphere, in
							   radians of course. */
	double sin_co2;			/* sine of one half of the angular
							   distance, i.e. c over 2 */
	double tmp1;
	double tmp2;

	double from [2];
	double to [2];

	struct cs_MmcofF_ mm_cof;

	/* Convert the degrees we have been provided with to
	   radians for our computations. */

	from [LNG] = CS_adj2pi (ll_from [LNG] * cs_Degree);
	from [LAT] = CS_adj1pi (ll_from [LAT] * cs_Degree);

	to [LNG] = CS_adj2pi (ll_to [LNG] * cs_Degree);
	to [LAT] = CS_adj1pi (ll_to [LAT] * cs_Degree);

	del_lng = CS_adj2pi (to [LNG] - from [LNG]);
	del_lat = CS_adj1pi (to [LAT] - from [LAT]);

	/* Deal with the zero length arc. */

	if (fabs (del_lng) <= cs_AnglTest && fabs (del_lat) <= cs_AnglTest)
	{
		*dist = cs_Zero;
		return (cs_Zero);
	}

	/* Deal with the antipodal case.  Another situation which can
	   cause floating point exceptions. */

	if (fabs (del_lng) > (cs_Pi - cs_AnglTest))
	{ 
		/* Here only when it makes sense to check the latitude
		   for the anti-posal case.  This is not trivial. */

		tmp1 = CS_adj1pi (from [LAT]);
		tmp2 = CS_adj1pi (to [LAT]);
		if (fabs (tmp1 + tmp2) < cs_AnglTest)
		{
			/* Antipodal. The distance is tricky. */

			if (e_sq == 0.0)
			{
				/* Sphere. */

				*dist = cs_Pi * e_rad;
			}
			else
			{
				/* Ellipsoid. */

				CSmmFsu (&mm_cof,e_rad,e_sq);
				*dist = cs_Two * CSmmFcal (&mm_cof,cs_Pi_o_2,
								   cs_One,
								   cs_Zero);
			}

			/* The shortest distance is over a pole.  We could
			   choose either pole. By always using the north pole,
			   the returned results are the same for any antipodal
			   points. */

			return (cs_Zero);
		}
	}

	/* Now, we deal with the case where one of the two points is at
	   a pole.  This is a bit easier now that we know that only one
	   points can be at a pole. */

	if (fabs (from [LAT]) > cs_NPTest || fabs (to   [LAT]) < cs_SPTest)
	{
		/* One of the two points is a pole.  Compute the distance
		   to the pole from the equator for this ellipsoid. */

		if (e_sq == 0.0)
		{
			mp = cs_Pi_o_2 * e_rad;
		}
		else
		{
			CSmmFsu (&mm_cof,e_rad,e_sq);
			mp = CSmmFcal (&mm_cof,cs_Pi_o_2,cs_One,cs_Zero);
		}

		if (fabs (to [LAT]) > cs_NPTest)
		{
			/* The TO point is a pole.  The forward azimuth
			   is zero if its the north pole, -180 if its the
			   south pole. */

			if (to [LAT] > 0.0) az = cs_Zero;
			else		    az = cs_Mpi;

			/* Compute the distance of the from point
			   from the equator. */

			if (e_sq == 0.0)
			{
				mm = cs_Pi_o_2 * e_rad * sin (from [LAT]);
			}
			else
			{
				mm = CSmmFcal (&mm_cof,from [LAT],
						       sin (from [LAT]),
						       cos (from [LAT]));		/*lint !e645 */
			}
		}
		else
		{
			/* The FROM point is a pole.  The azimuth is -180
			   if its the north pole, zero if it is the south
			   pole. */

			if (from [LAT] > 0.0) az = cs_Mpi;
			else		      az = cs_Zero;

			/* Distance of the TO point from the equator. */

			if (e_sq == 0.0)
			{
				mm = cs_Pi_o_2 * e_rad * sin (to [LAT]);
			}
			else
			{
				mm = CSmmFcal (&mm_cof,to [LAT],
						       sin (to [LAT]),
						       cos (to [LAT]));
			}
		}

		/* Compute the distance. */

		if ((to [LAT] * from [LAT]) > 0.0) *dist = mp - fabs (mm);
		else				   *dist = mp + fabs (mm);

		return (az * cs_Radian);
	}

	/* Now, we calculate the angular length of the arc.  If the arc
	   length exceeds a specific value, we use the geodetic function,
	   CSllazdd to compute the necessary information.  Otherwise,
	   we use the spherical trigonometry given here. */

	tmp1 = sin (del_lng * cs_Half);
	tmp1 = cos (from [LAT]) * cos (to [LAT]) * tmp1 * tmp1;
	tmp2 = sin (del_lat * cs_Half);
	tmp2 *= tmp2;
	sin_co2 = sqrt (tmp2 + tmp1);

	if (e_sq != 0.0)
	{
		return (CSllazdd (e_rad,e_sq,from,to,dist));
	}

	cc = cs_Two * asin (sin_co2);
	*dist = cc * e_rad;
	az = asin (sin (del_lng) * cos (to [LAT]) / sin (cc));
	return (az * cs_Radian);
}

/**********************************************************************
	Duplicates the code available from the NGS known as INVERSE.
	I don't really have a clue about what goes on here, only
	that it duplicates the functionality of the FORTRAN source
	code available in the above mentioned NGS distribution.
	Any comments you see are just some obvious observations.

	To quote the FORTRAN source code:

		SOLUTION OF THE GEODETIC INVERSE PROBLEM AFTER
		T. VINCENTY MODIFIED RAINSFORD'S METHOD WITH
		HELMERT'S ELLIPTICAL TERMS

		EFFECTIVE IN ANY AZIMUTH AND AT ANY DISTANCE
		SHORT OF ANTIPODAL.

		STANDPOINT/FOREPOINT MUST NOT BE THE GEOGRAPHIC
		POLE

	This algorithm used here is supposed to work effectively
	regardless of the points unless either is a pole, and
	the distance is less than a half revolution.

	The rather ugly variable names used are the same as
	used in the reference with the exception that all single
	character variable names used in the reference have been
	doubled up here.

	The code below assumes the preprocessing performed by CS_llazdd
	above, specifically: no zero length arcs, neither point is a
	pole, anti-podal arcs are removed, and the lat/long arguments
	are in radians.
***********************************************************************/

static double CSllazdd (	double e_rad,
							double e_sq,
							double from [2],
							double to [2],
							double *dist)
{
	extern double cs_Radian;		/* 180 / pi */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */
	extern double cs_Three;			/* 3.0 */
	extern double cs_Four;			/* 4.0 */
	extern double cs_Six;			/* 6.0 */

	int itr_brk;

	double del_lng;
	double rr;
	double tu1, tu2;
	double cu1, cu2;
	double su1;
	double ss, sa, sx, sy;
	double cx, cy, cz;
	double cc, dd, ee;
	double xx, yy;
	double c2a;
	double faz, baz;
	double flat;
	double tmp1;

	/* To keep compilers and lint happy. */
	sy = cy = yy = c2a = cz = ee = cs_Zero;
	
	/* We use the flattening of the ellipsoid several times. */
	flat = cs_One - sqrt (cs_One - e_sq);
	rr = cs_One - flat;

	tu1 = rr * sin (from [LAT]) / cos (from [LAT]); 
	tu2 = rr * sin (to [LAT]) / cos (to [LAT]); 

	cu1 = cs_One / sqrt (cs_One + tu1 * tu1);
	su1 = cu1 * tu1;
	cu2 = cs_One / sqrt (cs_One + tu2 * tu2);

	ss = cu1 * cu2;
	baz = ss * tu2;
	faz = baz * tu1;

	/* We now have a rough approximation of the forward
	   azimuth, faz, the back azimuth, baz, and the
	   geodetic distance between the two points.  These
	   are just rough approximations.  We enter a
	   iterative loop which will recompute these
	   values until the difference between successive
	   approximations of the azimuth is less than delta. */

	del_lng = CS_adj2pi (to [LNG] - from [LNG]);
	xx = del_lng;
	itr_brk = 16;
	do
	{
		sx = sin (xx);
		cx = cos (xx);
		tu1 = cu2 * sx;
		tu2 = baz - (su1 * cu2 * cx);
		if (--itr_brk <= 0) break;
		sy = sqrt (tu1 * tu1 + tu2 * tu2);
		cy = ss * cx + faz;
		yy = atan2 (sy,cy);
		sa = ss * sx / sy;
		c2a = -sa * sa + cs_One;
		cz = faz + faz;
		if (c2a > 0.0)
		{
			cz = -cz / c2a + cy;
		}
		ee = (cs_Two * cz * cz) - cs_One;
		cc = c2a * flat * ((-cs_Three * c2a + cs_Four) * flat + cs_Four) / 16.0;
		dd = xx;
		xx = ((ee * cy * cc + cz) * sy * cc + yy) * sa;
		xx = (cs_One - cc) * xx * flat + del_lng;
	} while (fabs (dd - xx) > 0.5E-13);
	faz = atan2 (tu1,tu2);

	/* In the following, 1.0 / rr / rr is eqivalent to
	   1.0 / (rr * rr) */

	xx = sqrt ((cs_One / rr / rr - cs_One) * c2a + cs_One) + cs_One;
	xx = (xx - cs_Two) / xx;

	cc = cs_One - xx;
	cc = (((xx * xx) / cs_Four) + cs_One) / cc;

	dd = ((0.375 * xx * xx) - cs_One) * xx;

	xx = ee * cy;

	ss = cs_One - ee - ee;

	tmp1 = (sy * sy * cs_Four) - cs_Three;
	tmp1 = (tmp1 * ss * cz * dd) / cs_Six;
	tmp1 = (tmp1 - xx) * dd / cs_Four;
	tmp1 = (tmp1 + cz) * sy * dd + yy;

	ss = tmp1 * cc * e_rad * rr;

	*dist = ss;
	return (faz * cs_Radian);
}

/**********************************************************************
**	CSllnrml (oll,ll,ll1,ll2);
**
**	double oll [2];				the longitude and latitude of the first point
**								on the line, usually the origin of the coordinate
**								system, IN DEGREES.
**	double ll [2];				the longitude and latitude of the second point
**								on the line, usually point being tested, IN DEGREES.
**	double ll1 [2];				returns longitude and latitude, in degrees, of a
**								point 0.5 seconds away from ll, normal to line from
**								oll to ll.
**	double ll2 [2];				returns longitude and latitude, in degrees, of a
**								point 0.5 seconds away from ll, in opposite direction
**								of point returned in ll1.
**
**	If ll is the same as oll, or if both have a latitude of
**	either pole, there is no arc defined and the concept of
**	an arc normal to it is ambiguous.  In this case, this
**	function returns an arc of constant latitude.
**********************************************************************/

/*
		define cc as one half of of a second of arc.

		cc = (0.5 / 3600.0) * RADIAN;

		we then use the sin and cosine of this value.

		sin_cc = sin (cc);
		cos_cc = cos (cc);
*/

static double sin_cc = 0.00000242406840554530;
static double cos_cc = 0.99999999999706194618;

void EXP_LVL7 CSllnrml (Const double oll [2],Const double ll  [2],double ll1 [2],double ll2 [2])

{
	extern double cs_Zero;
	extern double cs_One;
	extern double cs_Radian;
	extern double cs_Degree;
	extern double cs_Pi_o_2;
	extern double cs_NPTest;

	double Az;
	double lng;
	double sin_Az;
	double cos_Az;
	double del_lng;
	double sin_lat;
	double cos_lat;
	double cos_org_lat;
	double sin_org_lat;
	double sin_del_lng;
	double cos_del_lng;

	double tmp1;
	double tmp2;
	double tmp3;
	double tmp4;
	double tmp5;
	double tmp6;

	lng = ll [0] * cs_Degree;
	tmp1 = oll [1] * cs_Degree;
	sin_org_lat = sin (tmp1);
	cos_org_lat = cos (tmp1);

	if (fabs (tmp1) < cs_NPTest)
	{
		del_lng = (ll [0] - oll [0]) * cs_Degree;
		sin_del_lng = sin (del_lng);
		cos_del_lng = cos (del_lng);
	}
	else
	{
		del_lng = cs_Zero;
		sin_del_lng = cs_Zero;
		cos_del_lng = cs_One;
	}

	tmp1 = ll [1] * cs_Degree;
	sin_lat = sin (tmp1);
	cos_lat = cos (tmp1);

	/* Compute the azimuth (assume a sphere) of the line from
	   the origin to the point in question. */

	tmp1 = cos_lat * sin_del_lng;
	tmp2 = (cos_org_lat * sin_lat) - (sin_org_lat * cos_lat * cos_del_lng);
	if (tmp2 < 1.0E-06)
	{
		Az = cs_Zero;
	}
	else
	{
		Az = atan2 (tmp1,tmp2);
	}

	/* Add 90 degrees, i.e. the azimuth of the normal line. */

	Az += cs_Pi_o_2;
	Az = CS_adj2pi (Az);

	/* Get the sine and cosine of the azimuth. */

	cos_Az = cos (Az);
	sin_Az = sin (Az);

	/* Compute two new lat/longs, using the appropriate signs
	   of sin_Az and cos_Az to place the new line on either
	   side of the given point. */

	tmp1 = sin_lat * cos_cc;
	tmp2 = cos_lat * cos_cc;
	tmp3 = sin_cc * sin_Az;
	tmp4 = sin_cc * cos_Az;
	tmp5 = cos_lat * tmp4;
	tmp6 = sin_lat * tmp4;

	ll1 [0] = lng + atan2 (tmp3,(tmp2 - tmp6));
	ll1 [0] = CS_adj2pi (ll1 [0]);
	ll1 [1] = asin (tmp1 + tmp5);

	ll2 [0] = lng + atan2 (-tmp3,(tmp2 + tmp6));
	ll2 [0] = CS_adj2pi (ll2 [0]);
	ll2 [1] = asin (tmp1 - tmp5);

	/* Adjust the longitudes to be in correct quads. */

	ll1 [0] *= cs_Radian;
	ll1 [1] *= cs_Radian;
	ll2 [0] *= cs_Radian;
	ll2 [1] *= cs_Radian;

	CS_adjll (ll1);
	CS_adjll (ll2);

	/* That's it. */

	return;
}

/**********************************************************************
**	double CS_deltaLongitude (double baseMeridian, double relativeLongtiude);
**
**	baseLongitude;				the reference longitude to which the
**								returned delta longitude is referenced
**								to. IN DEGREES.
**	relativeLongitude;			the longitude to be converted to a delta
**								referenced to the baseMeridian.  IN DEGREES.
**	RETURNS:					the difference between the the relativeLongtiude
**								and the baseMeridian, where a negative value
**								indicates that the relative longitude value
**								indicates a meridian west of the provided
**								base meridian.
**
** No assumptions are made concerning the the meridian and longitude
** values provided.  Both are normalizaed to +/- 180 before any
** calculations are performed.
**
** Both longitude values are expected to be referenced to the same
** prime meridian.  It makes no difference what that prime meridian
** is, just as long as both are referenced to the same prime meridian.
**
**********************************************************************/

/*
baseMeridian,  relativeLongitude, deltaLongitude,  Case            
         0.0,              180.0,         +180.0, special case 1
       180.0,                0.0,         -180.0, special case 1
      -179.0,             +179.0,         +358.0, special case 2
      +179.0,             -179.0,         -358.0, special case 3
*/

/*lint -esym(550,myBase,myRelative)    not accessed, but nice to have when debugging */
double EXP_LVL3 CS_deltaLongitude (double baseMeridian,double relativeLongitude)
{
	extern double cs_K180;
	extern double cs_Km180;
	extern double cs_K360;
	
	double myBase;
	double myRelative;
	double deltaLongitude;
	
	myBase = CS_adj180 (baseMeridian);
	myRelative = CS_adj180 (relativeLongitude);
	deltaLongitude = relativeLongitude - baseMeridian;

	/* For 99.99% of the cases, we're done.  The whole purpose of having
	   this function is the 0.01% of the time where the relative longitude
	   is antiPodal or the simple calculation crosses over the +/- 180
	   degree crack.  Thus we have three special cases: */
	if (fabs (fabs (deltaLongitude) - cs_K180) < 1.0E-10)
	{
		/* 1.0E-10 ~ 0.01 millimeters at the equator of the earth. */
		/* Special case 1: the relative longitude is antiPodal to the
		   base meridian. */
		deltaLongitude = cs_K180;
	}
	else if (deltaLongitude > cs_K180)
	{
		/* Special case 2: crossing the +/- 180 degree crack going west */ 
		deltaLongitude -= cs_K360;
	}
	else if (deltaLongitude < cs_Km180)
	{
		/* Special case 3: crossing the =/- 180 degree crack going east. */
		deltaLongitude += cs_K360;
	}
	return deltaLongitude;
}
/*lint +esym(550,myBase,myRelative) */

