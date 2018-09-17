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

/*lint -esym(613,err_list) */
/*lint -esym(715,prj_code) */

/**********************************************************************
**	err_cnt = CSvdgrnQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	currently unused.
**	int err_list [];			an array of integers in which error codes are
**								returned if not NULL.
**	int list_sz;				the size of the array pointed to be err_list,
**								may be zero.
**	int err_cnt;				returns the number of errors detected.
**
**	Set err_list to NULL, and/or list_sz to zero, to simply get
**	a yea or nay status of the definition (i.e. err_cnt != 0).
**
**	All lat/longs in definitions must be referenced to Greennwich,
**	and in the range of greater than -180/-90, and less than or
**	equal to +180/+90.
**********************************************************************/

int EXP_LVL9 CSvdgrnQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Van Der Grinten specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}

	/* That's it for Van Der Grinten. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSvdgrnS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	This projection is supported for the sphere only.  We use
**	the equatorial radius of the related ellipsoid for the
**	radius of the sphere.
**********************************************************************/

void EXP_LVL9 CSvdgrnS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Radian;			/* 57.2..... */
	extern double cs_Pi;				/* 3.14159..... */
	extern double cs_Mpi;				/* -3.14159..... */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */

	struct cs_Vdgrn_ *vdgrn;

	vdgrn = &csprm->proj_prms.vdgrn;

	/* Transfer the necessary arguments to the "vdgrn" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */

	vdgrn->org_lng = csprm->csdef.org_lng * cs_Degree;
	vdgrn->k = csprm->csdef.scale;
	vdgrn->x_off = csprm->csdef.x_off;
	vdgrn->y_off = csprm->csdef.y_off;
	vdgrn->e_rad = csprm->datum.e_rad;
	vdgrn->ka = csprm->datum.e_rad * vdgrn->k;
	vdgrn->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	vdgrn->piR = vdgrn->ka * cs_Pi;
	vdgrn->two_ovr_pi = cs_Two / cs_Pi;
	vdgrn->one_mm = 0.001 * vdgrn->k;
	if (vdgrn->e_rad <= cs_Three) vdgrn->one_mm = 2.0E-10;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = vdgrn->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */

		csprm->min_ll [LNG] = cs_Km180;
		csprm->max_ll [LNG] = cs_K180;
		csprm->min_ll [LAT] = cs_Km90;
		csprm->max_ll [LAT] = cs_K90;
	}
	else
	{
		/* The definition includes a useful range specification.
		   We use these values without checking.  We expect the
		   user to give us absolute values, and we convert
		   to values relative to the central meridian. */

		csprm->min_ll [LNG] = CS_adj180 (csprm->csdef.ll_min [LNG] - csprm->cent_mer);
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = CS_adj180 (csprm->csdef.ll_max [LNG] - csprm->cent_mer);
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* Similarly with the X's and Y's.  If the coordinate system
	   definition carries some values, we use them.  If not, we
	   calculate some appropriate values. */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
	    csprm->csdef.xy_max [XX] == 0.0)
	{
		/* No specification in the coordinate system definition.
		   The setup is virtually complete, so we can use CSgnricF
		   to calculate some values as necessary. Unfortuneately
		   it is the rare case where we can just convert the
		   lat/long min/max. */

		csprm->min_xy [XX] = cs_Mpi * vdgrn->ka;
		csprm->max_xy [XX] = cs_Pi  * vdgrn->ka;
		csprm->min_xy [YY] = cs_Mpi * vdgrn->ka;
		csprm->max_xy [YY] = cs_Pi  * vdgrn->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,vdgrn->x_off,
											   vdgrn->y_off,
											   vdgrn->quad);
	}
	else
	{
		/* Use what ever the user has given us.  No adjustment
		   necessary.  Note: we don't check anything. */

		csprm->min_xy [XX] = csprm->csdef.xy_min [XX];
		csprm->min_xy [YY] = csprm->csdef.xy_min [YY];
		csprm->max_xy [XX] = csprm->csdef.xy_max [XX];
		csprm->max_xy [YY] = csprm->csdef.xy_max [YY];
	}
	/* That's all the calculations.  Stuff some function
	   addresses and we are done. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSvdgrnF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSvdgrnI;
	csprm->cs_scale = (cs_SCALE_CAST)CSvdgrnK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSvdgrnK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSvdgrnH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSvdgrnC;
	csprm->llchk    = (cs_LLCHK_CAST)CSvdgrnL;
	csprm->xychk    = (cs_XYCHK_CAST)CSvdgrnX;

	return;
}

/**********************************************************************
**	rtn_val =  CSvdgrnF (vdgrn,xy,ll);
**
**	struct cs_Vdgrn_ *vdgrn;	structure which defines the parameters
**								in effect for the transformation.
**	double xy [2];				the results are returned here, x ([0])
**								and y ([1]).
**	double ll [2];				the longitude ([0]) and latitude ([1]),
**								in degrees, to be converted.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and xy arguments of this function may point
**	to the same array with no adverse affects.
**
**	This function calculates the Van Der Grinten projection
**	and returns values in the coordinate system described
**	by the cs_Vdgrn_ structure.  For this projection
**	only the spherical earth is supported.  We use the
**	equatorial radius of the ellipsoid provided as the radius
**	of the sphere.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections - A Working Manual"
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 239-242.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSvdgrnF (Const struct cs_Vdgrn_ *vdgrn,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Pi;				/* 3.14159..... */
	extern double cs_Two_pi;			/* 2 pi */
	extern double cs_Mpi;				/* -3.14159..... */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */

	int rtn_val;

	double lng;				/* The given longitude, after conversion
							   to radians. */
	double lat;				/* The given latitude after conversion
							   to radians. */
	double del_lng;
	double abs_lat;
	double xx;
	double yy;

	double theta;
	double sin_theta, cos_theta;

	double A, A_sq;
	double P, P_sq;
	double G;
	double Q;
	double tmp1, tmp2, tmp3;

	rtn_val = cs_CNVRT_NRML;

	/* For this projection, we only support the spherical form,
	   therefore there is only one set of equations. */

	lng = ll [LNG] * cs_Degree;
	del_lng = lng - vdgrn->org_lng;
	if      (del_lng > cs_Pi  && vdgrn->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && vdgrn->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2piI (del_lng);
	}

	lat = ll [LAT] * cs_Degree;
	abs_lat = fabs (lat);
	if (abs_lat > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (abs_lat > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
			abs_lat = fabs (lat);
		}
	}

	if (abs_lat > cs_NPTest)
	{
		/* The sign of yy gets reversed below, we can simply
		   use cs_Pi here. */

		xx = cs_Zero;
		yy = cs_Pi * vdgrn->ka;
	}
	else if (abs_lat <= cs_AnglTest)
	{
		/* This is the calculation for the equator.

		   The sign of X gets reversed below, we need to use
		   the absolute value here. */

		xx = vdgrn->ka * fabs (del_lng);
		yy = cs_Zero;
	}
	else if (fabs (del_lng) < cs_AnglTest)
	{
		/* We're on the central meridian, BUT NOT on the
		   equator, and not at either pole. Npte, sign of
		   result is dealt with below. */

		xx = cs_Zero;
		theta = asin (abs_lat / cs_Pi_o_2);
		yy = cs_Pi * vdgrn->ka * tan (theta * cs_Half);
	}
	else
	{
		/* OK, we're not at either pole, or the equator.  The
		   rest of this should be pretty safe. */

		sin_theta = fabs (lat) * vdgrn->two_ovr_pi;
		cos_theta = sqrt (cs_One - sin_theta * sin_theta);

		tmp1 = (cs_Pi / del_lng) - (del_lng / cs_Pi);
		A = cs_Half * fabs (tmp1);
		A_sq = A * A;

		G = cos_theta / (sin_theta + cos_theta - cs_One);

		P = G * ((cs_Two / sin_theta) - cs_One);
		P_sq = P * P;

		Q = A_sq + G;

		tmp1 = P_sq + A_sq;
		tmp2 = G - P_sq;

		tmp3 = tmp1 * ((G * G) - P_sq);
		tmp3 = (A_sq * tmp2 * tmp2) - tmp3;
		if (tmp3 < 0.0) tmp3 = cs_Zero;
		tmp3 = (A * tmp2) + sqrt (tmp3);
		xx = vdgrn->piR * tmp3 / tmp1;

		tmp3 = ((A_sq + cs_One) * tmp1) - (Q * Q);
		if (tmp3 < 0.0) tmp3 = cs_Zero;
		tmp3 = (P * Q) - (A * sqrt (tmp3));
		yy = vdgrn->piR * tmp3 / tmp1;
	}

	/* Apply the appropriate sign and false origin. */

	xy [XX] = (del_lng >= 0.0) ? xx : -xx;
	xy [YY] = (lat     >= 0.0) ? yy : -yy;

	if (vdgrn->quad == 0)
	{
		xy [XX] += vdgrn->x_off;
		xy [YY] += vdgrn->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],vdgrn->x_off,vdgrn->y_off,vdgrn->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSvdgrnI (vdgrn,ll,xy);
**
**	struct cs_Vdgrn_ *vdgrn;	structure which defines the parameters
**								in effect for the transformation.
**	double ll [2];				the longitude ([0]) and latitude ([1]),
**								in degrees, are returnrd here.
**	double xy [2];				the coordinates to be converted, x ([0])
**								and y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and xy arguments of this function may point to the same
**	array with no adverse affects.
**
**	All the formulae used here were extracted from the
**	USGS publication "Map Projections - A Working Manual",
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 239-242.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**
**	For this projection, only the spherical form is supported.
**********************************************************************/

int EXP_LVL9 CSvdgrnI (Const struct cs_Vdgrn_ *vdgrn,double ll [2],Const double xy [2])
{
	/* We undefine the following as these identifiers are used
	   as variables in order to match the notation used in Synder. */

	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Pi;				/* 3.14159....  */
	extern double cs_Mpi;				/*-3.14159....  */
	extern double cs_Pi_o_3;			/* PI / 3.0 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Third;				/* 0.33333..... */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_Nine;				/* 9.0 */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_Mpi_o_2;			/* -Pi over 2 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double x;
	double y;

	double lat;
	double del_lng;

	double xx, xx2;
	double yy, yy2, two_yy2;
	double xx2_yy2, xx2_yy2_sq;
	double c1;
	double c2, c2_sq, c2_cube;
	double c3, c3_sq, c3_cube;
	double dd;
	double a1;
	double m1;
	double theta1;

	double tmp;

	rtn_val = cs_CNVRT_NRML;

	if (vdgrn->quad == 0)
	{
		x = xy [0] - vdgrn->x_off;
		y = xy [1] - vdgrn->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,vdgrn->x_off,vdgrn->y_off,vdgrn->quad);
	}

	/* Deal with bogus x and y coordinates.  Should do this without
	   the square root for performance purposes. */

	tmp = sqrt (x * x + y * y);
	if (tmp > vdgrn->piR)
	{
		rtn_val = cs_CNVRT_RNG;
		x *= vdgrn->piR / tmp;
		y *= vdgrn->piR / tmp;
	}

	/* Set up all repeated variations of X and Y. */

	xx = x / vdgrn->piR;
	yy = y / vdgrn->piR;
	xx2 = xx * xx;
	yy2 = yy * yy;
	two_yy2 = yy2 + yy2;
	xx2_yy2 = xx2 + yy2;
	xx2_yy2_sq = xx2_yy2 * xx2_yy2;

	/* Compute the internal constants. */

	c1 = -(fabs (yy) * (cs_One + xx2_yy2));

	c2 = c1 - two_yy2 + xx2;
	c2_sq = c2 * c2;
	c2_cube = c2_sq * c2;

	c3 = cs_One - (cs_Two * c1) + two_yy2 + xx2_yy2_sq;
	c3_sq = c3 * c3;
	c3_cube = c3_sq * c3;

	/* Compute the latitude.  The ((3 * dd) / (a1 * m1))
	   term goes to infinity when Y is zero.  Since Y == 0 means
	   the equator for this projection, we punt and simply
	   set lat = 0.0 in this case. */

	if (fabs (y) <= vdgrn->one_mm)
	{
		lat = cs_Zero;
	}
	else
	{
		tmp = (cs_Two * c2_cube / c3_cube) - (cs_Nine * c1 * c2 / c3_sq);
		dd = (yy2 / c3) + (tmp / 27.0);
		a1 = (c1 - (c2_sq / (cs_Three * c3))) / c3;
		m1 = cs_Two * sqrt ((-a1) / cs_Three);
		tmp = (cs_Three * dd) / (a1 * m1);

		/* tmp, at this point, should never be greater than
		   one.  However, noise in the lower bits makes the
		   following necessary. */

		if (fabs (tmp) >= cs_One)
		{
			tmp = (tmp >= 0.0) ? cs_One : cs_Mone;
		}
		theta1 = cs_Third * acos (tmp);
		tmp = cos (theta1 + cs_Pi_o_3);
		lat = cs_Pi * ((-m1 * tmp) - (c2 / (cs_Three * c3)));
		if (y < 0) lat = -lat;
	}

	/* If xx is zero, the following falls apart, but longitude
	   is equal to the origin longitude. */

	if (fabs (x) < vdgrn->one_mm)
	{
		del_lng = cs_Zero;
	}
	else
	{
		tmp = cs_One + cs_Two * (xx2 - yy2) + xx2_yy2_sq;
		tmp = xx2_yy2 - cs_One + sqrt (tmp);
		/* Following divide by 2X causes the blow up. */
		del_lng = ((cs_Pi * tmp) / (cs_Two * xx));
	}

	/* Convert the results to degrees. */

	if (fabs (del_lng) > cs_Pi)
	{
		del_lng = (del_lng > 0.0) ? cs_Pi : cs_Mpi;
		rtn_val = cs_CNVRT_RNG;
	}
	if (fabs (lat) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = (lat > 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
		}
	}
	ll [LNG] = (del_lng + vdgrn->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSvdgrnC (vdgrn,ll);
**
**	struct cs_Vdgrn_ *vdgrn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Van der Grinten
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double gamma;				returns the computed convergence angle for
**								the coordinate system as the specified
**								location in degrees east of north.
**
**	Returns cs_Km360 as an error indication; caused by a point
**	outside the domain of the projection.
**
**	We have not as yet found or deduced an analytical equation
**	for the convergence angle for this projection.  We calculate
**	it empicially. The convergence angle is defined as the
**	arctangent of the partial derivative of Y with respect to
**	latitude (read delta Y when the latitude changes a skosh)
**	divied by the partial derivative of X with repsect to
**	latitude (i.e. delta X).  See Synder/Bugayevskiy, page 16.
**********************************************************************/

double EXP_LVL9 CSvdgrnC (Const struct cs_Vdgrn_ *vdgrn,Const double ll [2])
{
	extern double cs_Radian;			/* 57.2957... */
	extern double cs_Km360;				/* -360.0, the value which
										   we return if provided
										   with a bogus point. */
	int status;

	double del_xx;
	double del_yy;
	double gamma;					/* some folks call this alpha */

	double xy1 [3];
	double xy2 [3];
	double my_ll [3];

	my_ll [LNG] = ll [LNG];
	my_ll [LAT] = ll [LAT];

	/* Compute the cartesian coordinates of the end points of a
	   linear line segment whose end points are on the same meridian,
	   but separated by a small amount of latitude.  The degree of
	   latitude separation is rather arbitrary.  Technically, the
	   smaller the better, but if its too small, we end up with
	   noise problems in the trig functions.  0.00005 gives us a
	   line of about 10 meters on the surface of the earth.  We
	   use literal constants as this may need to be adjusted to
	   the mathemagics of the specific projection involved.

	   Should be zero on the central meridian and the equator. */

	my_ll [LAT] -= 0.00005;
	status = CSvdgrnF (vdgrn,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;					/* 2 * 0.00005 */
	status = CSvdgrnF (vdgrn,xy2,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	
	/* Some atan2's (not all) don't like it when both arguments are
	   zero. Normally, it is safe to assume that del_yy is never
	   zero given the above. However, testing has shown that when
	   provided with unrealistic locations, both del_xx and del_yy
	   can be zero (exactly on the pole where the pole is
	   the origin). */

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	if ((fabs (del_xx) + fabs (del_yy)) > 0.0)
	{
		gamma = -atan2 (del_xx,del_yy) * cs_Radian;
	}
	else
	{
		gamma = cs_Km360;
	}
	return (gamma);
}

/**********************************************************************
**	kk = CSvdgrnK (vdgrn,ll);
**
**	struct cs_Vdgrn_ *vdgrn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Van Der Grinten
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale along a parallel of the
**								coordinate system at the specified point.
**
**	This projection is not defined, as far as we know at the
**	current time, for the ellipsoid.  We simple use the
**	equatorial radius of the supplied ellipsoid as the radius
**	of the sphere.
**
**	While formulas for this quantity are known to exist, we have
**	located them as yet.  Therefore, we calculate this quantity
**	using an empirical method.
**********************************************************************/

double EXP_LVL9 CSvdgrnK (Const struct cs_Vdgrn_ *vdgrn,Const double ll [2])
{
	extern double cs_Degree;			/* 1/RADIAN */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_HlfSecDeg;			/* half of a second, in
										   degrees */
	extern double cs_SclInf;			/* 9.9E+04, the value
										   returned for an
										   infinite scale factor. */
	extern double cs_Sin1Sec;			/* Sine of 1 second of arc */

	int status;

	double kk;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

	double del_xx, del_yy;
	double dd_xy, dd_ll;

	/* We compute the grid scale factor empirically.  There are
	   formulas, but they are so complex, that this is just as
	   fast.

	   Algorithm here is very simple:

	   1) convert the lat/long to X and Y.
	   2) add a skosh to the longitude and compute a second X and Y.
	   3) calculate the distance between the two XY's.
	   4) compute the angular distance between the two lat/longs.
	   5) convert the angular distance to linear using the radius.
	   6) compare the two distances. */

	ll0 [LNG] = ll [LNG] - cs_HlfSecDeg;
	ll0 [LAT] = ll [LAT];
	ll1 [LNG] = ll [LNG] + cs_HlfSecDeg;
	ll1 [LAT] = ll [LAT];

	/* Establish two points, pretty close together. */

	status = CSvdgrnF (vdgrn,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CSvdgrnF (vdgrn,xy2,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Get the grid distance. */

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	dd_xy = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Compute the angular distance.  We know that delta longitude
	   is one arc second, and delta latitude is zero.  We also
	   know that the sine of very small angles, like one 
	   arc second, is the same as the angle.  Working
	   these facts into spherical trigonometry yields the
	   following simple formula for the geographic distance
	   on a sphere for an arc second.  cs_Sin1Sec is the sine
	   of one arc second, or one arc second. See Synder, page 30. */

	dd_ll = vdgrn->ka * cs_Sin1Sec * cos (ll0 [1] * cs_Degree);

	/* The ratio of the two distances is the grid scale factor. */

	if (dd_ll > 0.0) kk = dd_xy / dd_ll;
	else		 kk = cs_SclInf;
	return (kk);
}

/**********************************************************************
**	hh = CSvdgrnH (vdgrn,ll);
**
**	struct cs_Vdgrn_ *vdgrn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Van Der Grinten			Cylindrical Projection.
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double hh;					returns the grid scale along a meridian of the
**								coordinate system at the specified point.
**
**	This projection is not defined, as far as we know at the
**	current time, for the ellipsoid.  We simple use the
**	equatorial radius of the supplied ellipsoid as the radius
**	of the sphere.
**********************************************************************/

double EXP_LVL9 CSvdgrnH (Const struct cs_Vdgrn_ *vdgrn,Const double ll [2])
{
	extern double cs_Mone;				/* -1.0 */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   sacle. */

	int status;

	double hh;

	double del_xx, del_yy;
	double dd_xy, dd_ll;

	double xy1 [2];
	double xy2 [2];
	double ll0 [2];
	double ll1 [2];

	/* We compute the grid scale factor empirically.  There are
	   formulas, but they are so complex, that this is just as
	   fast.  We'll program the formulas after a trip to the
	   library.

	   Algorithm here is rather simple:

	   1) convert the lat/long to X and Y.
	   2) add a skosh to the longitude and compute a second X and Y.
	   3) calculate the distance between the two XY's.
	   4) compute the angular distance between the two lat/longs.
	   5) convert the angular distance to linear using the radius.
	   6) compare the two distances. */

	ll0 [LNG] = ll [LNG];
	ll0 [LAT] = ll [LAT] - 0.00005;
	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] + 0.00005;

	/* Establish two points, pretty close together. */

	status = CSvdgrnF (vdgrn,xy1,ll0);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}
	status = CSvdgrnF (vdgrn,xy2,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Get the grid distance. */

	del_xx = xy2 [XX] - xy1 [XX];
	del_yy = xy2 [YY] - xy1 [YY];
	dd_xy = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Compute the angular distance.  Keep in mind that delta
	   longitude is zero for this case, delta latitude is fixed
	   at one arc second, and the sine of a small angle like
	   one arc second is the same as the angle (in radians). */

	dd_ll = vdgrn->ka * 1.74532925E-06;
	if (dd_ll > vdgrn->one_mm)
	{
		hh = dd_xy / dd_ll;
	}
	else
	{
		hh = cs_SclInf;
	}
	return (hh);
}

/**********************************************************************
**	status = CSvdgrnL (vdgrn,cnt,pnts);
**
**	struct cs_Vdgrn_ *vdgrn;	the coordinate system against which the check is
**								to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
**	double pnts [][2];			the list of coordinates to be checked.
**	int status;					returns cs_CNVRT_OK if the point, line, or region
**								is completely within the domain of the
**								coordinate system.  Otherwise, cs_CNVRT_DOMN
**								is returned.
**
**	This function simply checks the mathematical viability of
**	a coordinate conversion.  It has nothing to do with the
**	useful limits of the coordinate system.
**
**	This function expects that the input lat/longs are normalized
**	if appropriate.
**********************************************************************/

int EXP_LVL9 CSvdgrnL (Const struct cs_Vdgrn_ *vdgrn,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi_o_2;		/* PI over 2 */

	int ii;
	int status;

	double lat;

	/* Check all the points.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}											/*lint !e715 */

/**********************************************************************
**	status = CSvdgrnX (vdgrn,cnt,pnts);
**
**	struct cs_Vdgrn_ *vdgrn;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSvdgrnL to check lat/long
**	values.
**
**	What gets checked depends upon the cnt argument.  A value
**	of zero (or less than that) checks nothing successfully.
**	A value of 1 indicates that a single point is to be checked.
**	a value of 2 indicates that a line is to be checked.  The
**	entire line must reside within the domain of the coordinate
**	system to pass.
**
**	A value of 4 or more for cnt indicates that a region is to
**	be checked.  The pnts array must contain the indicated
**	number of points which defines a region.  The last point
**	in the array must duplicate the first point.  The resulting
**	polygon must be simply connected (no bow ties).  The region
**	passes if the entire region is within the domain of the
**	coordinate system.
**
**	The return value refers to either the point, or the line
**	segment, provided. Return values are as follows:
**
**	cs_CNVRT_OK	the point, entire line segment, or entire region,
**			is within the catresian domain of the coordinate
**			system.
**	cs_CNVRT_DOMN	the point, all or a portion of the line segment,
**			or all or a portion of the region, is not within
**			the cartesian domain of the coordinate system.
**********************************************************************/

int EXP_LVL9 CSvdgrnX (Const struct cs_Vdgrn_ *vdgrn,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic range.
	   No special checks are required for lines and/or regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSvdgrnI (vdgrn,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
