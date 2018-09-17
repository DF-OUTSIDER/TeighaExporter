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
/*lint -esym(534,CSbpcncF) ignoring return value */

/**********************************************************************
**	err_cnt = CSbpcncQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSbpcncQ (	Const struct cs_Csdef_ *cs_def,	unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MinLatFz;		/* - 89.999 */
	extern double cs_MaxLatFz;		/* + 89.999 */
	extern double cs_MaxLngFz;		/* + 179.999 */
	extern double cs_ParmTest;		/* .1 seconds of arc in degrees */

	int err_cnt;

	double tmp;
	double pole_cc;

	double ll0 [2];
	double ll1 [2];

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the BiPoalr specific stuff. */
	
	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_POLLNG;
	}
	if (cs_def->prj_prm2 <= cs_MinLatFz || cs_def->prj_prm2 >= cs_MaxLatFz)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_POLLAT;
	}
	
	if (cs_def->prj_prm3 <= cs_MinLng || cs_def->prj_prm3 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_POLLNG;
	}

	if (cs_def->prj_prm5 == 0.0)
	{
		if (cs_def->prj_prm4 <= cs_MinLatFz || cs_def->prj_prm4 >= cs_MaxLatFz)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_POLLAT;
		}

		/* Calculate the angular distance between the two poles.
		   If zero, or close to it, we have a big problem. */

		ll0 [LNG] = cs_def->prj_prm1;
		ll0 [LAT] = cs_def->prj_prm2;
		ll1 [LNG] = cs_def->prj_prm3;
		ll1 [LAT] = cs_def->prj_prm4;
		pole_cc = CSccsphrD (ll0,ll1);
		if (pole_cc < cs_ParmTest)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_POLDUP;
		}
	}
	else
	{
		if (cs_def->prj_prm5 < 0.0 || cs_def->prj_prm5 >= cs_MaxLngFz)
		{
			pole_cc = cs_MaxLngFz;
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_POLDD;
		}
		else
		{
			pole_cc = cs_def->prj_prm5;
		}
	}

	/* The standard parallel distances must not be:
	   1) less than or equal to zero;
	   2) greater than or equal to 90;
	   2) greater than pole_cc;
	   3) equal to each other.
	   4) Their sum must equal pole_cc
	   Also, the first standard parallel distance must be less
	   than the second.  Not very difficult, but a lot of work.
	   Let's get to it.
	*/

	if (cs_def->prj_prm6 < cs_ParmTest ||
	    cs_def->prj_prm7 < cs_ParmTest)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_PLLZERO;
	}
	if (cs_def->prj_prm6 > cs_MaxLatFz ||
	    cs_def->prj_prm7 > cs_MaxLatFz)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_PLL90;
	}
	tmp = cs_def->prj_prm7 - cs_def->prj_prm6;
	if (fabs (tmp) <= cs_ParmTest)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_PLLEQU;
	}
	if (tmp < 0.0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_PLLREV;
	}
	if (cs_def->prj_prm6 > pole_cc ||
	    cs_def->prj_prm7 > pole_cc)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_PLLLRG;
	}
	tmp = (cs_def->prj_prm6 + cs_def->prj_prm7) - pole_cc;
	if (fabs (tmp) > cs_ParmTest)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_PLLED;
	}

	/* That's it for the Bipolar Oblique Conformal Conic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSbpcncS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSbpcncS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Half;				/* 0.5               */
	extern double cs_One;				/* 1.0               */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_K90;				/* 90.0 */
	extern double cs_Km90;				/* -90.0 */

	struct cs_Bpcnc_ *bpcnc;

	double sin_dd, cos_dd;
	double sin_zc, cos_zc;
	double sin_Az_AB, cos_Az_AB;
	double cos_lat_c;

	double tmp1, tmp2, tmp3, tmp4, tmp5;

	double test_ll [3];
	double test_xy [3];

	bpcnc = &csprm->proj_prms.bpcnc;

	/* Transfer the necessary arguments to the
	   "bpcnc" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	bpcnc->lng_A = csprm->csdef.prj_prm1 * cs_Degree;
	bpcnc->lat_A = csprm->csdef.prj_prm2 * cs_Degree;
	bpcnc->lng_B = csprm->csdef.prj_prm3 * cs_Degree;
	bpcnc->lat_B = csprm->csdef.prj_prm4 * cs_Degree;
	bpcnc->pole_dd = csprm->csdef.prj_prm5 * cs_Degree;
	bpcnc->sp_1 = csprm->csdef.prj_prm6 * cs_Degree;
	bpcnc->sp_2 = csprm->csdef.prj_prm7 * cs_Degree;
	bpcnc->x_off = csprm->csdef.x_off;
	bpcnc->y_off = csprm->csdef.y_off;
	bpcnc->ka = csprm->datum.e_rad * csprm->csdef.scale;
	bpcnc->one_cm = 0.01 * csprm->csdef.scale;
	bpcnc->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Check for the test cases where the sphere radius is one. */

	if (csprm->datum.e_rad == cs_One) bpcnc->one_cm = 2.0E-09;		/*lint !e777 */

	/* This projection is only defined for the sphere.
	   Compute the required constant values from the data
	   provided by the user. */

	bpcnc->sin_lat_A = sin (bpcnc->lat_A);
	bpcnc->cos_lat_A = cos (bpcnc->lat_A);
	bpcnc->sin_lat_B = sin (bpcnc->lat_B);
	bpcnc->cos_lat_B = cos (bpcnc->lat_B);
	if (bpcnc->pole_dd <= cs_Zero)
	{
		/* Here to compute pole_dd from the actual
		   pole positions provided.  Note, we assume
		   that this distance will not be close to
		   zero.  See Synder, page 30. */

		tmp1 = cos (bpcnc->lng_B - bpcnc->lng_A);
		cos_dd = (bpcnc->sin_lat_A * bpcnc->sin_lat_B) -
		         (bpcnc->cos_lat_B * bpcnc->cos_lat_A * tmp1);
		bpcnc->pole_dd = acos (cos_dd);
		sin_dd = sin (bpcnc->pole_dd);
	}
	else
	{
		/* Otherwise, we caluclate the longitude of
		   pole B from the angular length provided. */

		sin_dd = sin (bpcnc->pole_dd);
		cos_dd = cos (bpcnc->pole_dd);

		tmp1 = bpcnc->cos_lat_A * bpcnc->cos_lat_B;
		tmp1 = (cos_dd - bpcnc->sin_lat_A * bpcnc->sin_lat_B) / tmp1;
		bpcnc->lng_B = bpcnc->lng_A + acos (tmp1);
	}

	/* N is the cone constant, it's the same for both cones. */

	if (fabs (bpcnc->sp_1 - bpcnc->sp_2) > 1.0E-06)
	{
		tmp1 = tan (bpcnc->sp_1 * cs_Half);
		tmp2 = tan (bpcnc->sp_2 * cs_Half);
		tmp3 = log (tmp1) - log (tmp2);
		tmp4 = sin (bpcnc->sp_1);
		tmp5 = sin (bpcnc->sp_2);
		bpcnc->nn = (log (tmp4) - log (tmp5)) / tmp3;
		bpcnc->T = pow (tmp1,bpcnc->nn) + pow (tmp2,bpcnc->nn);
	}
	else
	{
		bpcnc->nn = sin (bpcnc->sp_1);
		bpcnc->T = cs_One;
	}

	tmp1 = tan (bpcnc->sp_1 * cs_Half);
	tmp1 = bpcnc->nn * pow (tmp1,bpcnc->nn);
	bpcnc->F0 = bpcnc->ka * sin (bpcnc->sp_1) / tmp1;

	tmp1 = bpcnc->pole_dd * cs_Half;
	tmp2 = bpcnc->ka * sin (tmp1);
	tmp3 = pow (tan (tmp1 * cs_Half),bpcnc->nn);
	tmp3 *= bpcnc->F0 * bpcnc->nn;
	bpcnc->k0 = cs_Two / (cs_One + (tmp3 / tmp2));

	bpcnc->F = bpcnc->k0 * bpcnc->F0;

	tmp1 = cos (bpcnc->lng_B - bpcnc->lng_A);
	tmp2 = bpcnc->cos_lat_A * bpcnc->sin_lat_B - bpcnc->sin_lat_A * bpcnc->cos_lat_B * tmp1;
	bpcnc->Az_AB = acos (tmp2 / sin_dd);
	sin_Az_AB = sin (bpcnc->Az_AB);
	cos_Az_AB = cos (bpcnc->Az_AB);

	tmp2 = bpcnc->cos_lat_B * bpcnc->sin_lat_A - bpcnc->sin_lat_B * bpcnc->cos_lat_A * tmp1;
	bpcnc->Az_BA = acos (tmp2 / sin_dd);
	bpcnc->Az_BAc = CS_adj2pi (bpcnc->Az_BA - cs_Pi);

	bpcnc->rho_c = cs_Half * bpcnc->F * bpcnc->T;

	tmp1 = pow (bpcnc->T * cs_Half,cs_One / bpcnc->nn);
	bpcnc->z_c = cs_Two * atan (tmp1);
	cos_zc = cos (bpcnc->z_c);
	sin_zc = sin (bpcnc->z_c);

	tmp1 = bpcnc->sin_lat_A * cos_zc + bpcnc->cos_lat_A * sin_zc * cos_Az_AB;
	bpcnc->lat_c = asin (tmp1);
	cos_lat_c = cos (bpcnc->lat_c);

	tmp1 = sin_zc * sin_Az_AB / cos_lat_c;
	bpcnc->lng_c = asin (tmp1) + bpcnc->lng_A;

	tmp1 = bpcnc->cos_lat_A * sin_Az_AB / cos_lat_c;
	bpcnc->Az_c = asin (tmp1);
	bpcnc->sin_Az_c = sin (bpcnc->Az_c);
	bpcnc->cos_Az_c = cos (bpcnc->Az_c);

	tmp1 = tan (cs_Half * bpcnc->pole_dd);
	bpcnc->max_xx = bpcnc->F * pow (tmp1,bpcnc->nn);
	bpcnc->Az_max = cs_Pi * bpcnc->nn;

	/* Set up the coordinate checking information.  This is difficult
	   for this projection, which is not only oblique, but bipolar
	   oblique.  We are helped, graciously, by the fact that the
	   projection can handle just about any lat/long, and just
	   about any X and Y as a point. */

	csprm->cent_mer = bpcnc->lng_c * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  This range actually includes some
		   values outside the useful range, but which are
		   computable nevertheless.  We need to do this, otherwise
		   the min/max rectangle would exclude values in the
		   usable range, and trhat we need to avoid. */

		csprm->min_ll [LNG] = cs_Km90;
		csprm->max_ll [LNG] = cs_K90;
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
		   The setup is virtually complete, so we can use CSbpcncF
		   to calculate some values as necessary.  In this
		   projection, the actualk min/max X's and Y's are for a
		   skew rectangle.  So, the best we can do here is provide
		   a basic guess. */

		test_ll [LNG] = CS_adj180 (csprm->min_ll [LNG] + csprm->cent_mer);
		test_ll [LAT] = cs_Zero;
		CSbpcncF (bpcnc,test_xy,test_ll);
		csprm->min_xy [XX] = test_xy [XX] - bpcnc->x_off;
		csprm->max_xy [XX] = -csprm->min_xy [XX];

		test_ll [LNG] = csprm->cent_mer;
		test_ll [LAT] = cs_K90;
		CSbpcncF (bpcnc,test_xy,test_ll);
		csprm->min_xy [YY] = test_xy [YY] - bpcnc->y_off;
		test_ll [LAT] = cs_Km90;
		CSbpcncF (bpcnc,test_xy,test_ll);
		csprm->max_xy [YY] = test_xy [YY] - bpcnc->y_off;

		CS_quadMM (csprm->min_xy,csprm->max_xy,bpcnc->x_off,
						       bpcnc->y_off,
						       bpcnc->quad);
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
	   addresses and we are done.  Since this projection
	   is conformal; the k and h scale factors are the
	   same. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSbpcncF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSbpcncI;
	csprm->cs_scale = (cs_SCALE_CAST)CSbpcncK;
	csprm->cs_sclk  = (cs_SCALE_CAST)CSbpcncK;
	csprm->cs_sclh  = (cs_SCALE_CAST)CSbpcncK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSbpcncC;
	csprm->llchk    = (cs_LLCHK_CAST)CSbpcncL;
	csprm->xychk    = (cs_XYCHK_CAST)CSbpcncX;

	return;
}

/**********************************************************************
**	rtn_val = CSbpcncF (bpcnc,xy,ll);
**
**	struct cs_Bpcnc_ *bpcnc;	structure which defines the parameters
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
**	This function calculates the Bipolar Oblique Conformal
**	Conic projection and returns values in the coordinate
**	system described in the cs_Bpcnc_ structure.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 116-121.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**
**	The Lambert Conformal Conic, which is the basis of this
**	projection, can handle just about any latitude and longitude.
**	By having two cones, we can handle any latitude and longitude.
**********************************************************************/

int EXP_LVL9 CSbpcncF (	Const struct cs_Bpcnc_ *bpcnc,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 3.14159 / 180.0 */
	extern double cs_Pi;				/* 3.14159 */
	extern double cs_Mpi;				/* 3.14159 */
	extern double cs_Two_pi;			/* 6.28... */
	extern double cs_Pi_o_2;			/* 3.14159 / 2.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_AnglTest;			/* 0.001 arc seconds, in
										   radians. */
	int rtn_val;
	int south;

	double xx;
	double yy;
	double lng;
	double lat;

	double del;
	double zz;
	double Az;
	double Az_term;
	double alpha;
	double rho;
	double cos_del;
	double sin_lat;
	double cos_lat;

	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = CS_adj1pi (lat);
	}

	sin_lat = sin (lat);
	cos_lat = cos (lat);

	del = bpcnc->lng_B - lng;
	if      (del > cs_Pi  && bpcnc->lng_B < 0.0) del -= cs_Two_pi;
	else if (del < cs_Mpi && bpcnc->lng_B > 0.0) del += cs_Two_pi;
	if (fabs (del) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del = CS_adj2pi (del);
	}
	cos_del = cos (del);

	/* Compute Az, the azimuth of the point from (at this time)
	   pole B.  If we are actually at pole B, the following would
	   produce an atan2 call with zeros for both arguments.  Many
	   atan2 functions don't like that. We rely on the fact that
	   for small angles, the angle (in radians) and the sine are
	   essentially the same. */

	tmp1 = bpcnc->cos_lat_B * tan (lat) - bpcnc->sin_lat_B * cos_del;
	tmp2 = sin (del);
	if (fabs (tmp1) > cs_AnglTest || fabs (tmp2) > cs_AnglTest)
	{
		Az = atan2 (tmp2,tmp1);
	}
	else
	{
		/* At pole B, we use the azimuth from B to A. This forces
		   use of the B cone's equations. */

		Az = bpcnc->Az_BA;
	}

	if (Az < bpcnc->Az_BA && Az > bpcnc->Az_BAc)
	{
		/* Here for the northern cone, i.e. the cone whose
		   tip is above pole B. zz is the angular distance
		   from the pole to the point in question. */

		south = FALSE;

		tmp1 = bpcnc->sin_lat_B * sin_lat;
		tmp2 = bpcnc->cos_lat_B * cos_lat * cos_del;
		zz = acos (tmp1 + tmp2);

		/* Az_term here is the convergence angle, relative
		   to pole B.  If this is greater than 180 degrees, we
		   are outside the appropriate range of the projection.
		   I.e, we're on the back side of the cone. */

		tmp1 = bpcnc->Az_BA - Az;
		if (fabs (tmp1) > cs_Pi)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp1 = CS_adj2pi (tmp1);
		}
		Az_term = bpcnc->nn * tmp1;
	}
	else
	{
		/* Here for the northern cone, i.e. the cone whose
		   tip is above pole A. */

		south = TRUE;

		del = lng - bpcnc->lng_A;
		if      (del > cs_Pi && bpcnc->lng_A < 0.0) del -= cs_Two_pi;
		else if (del < cs_Pi && bpcnc->lng_A > 0.0) del += cs_Two_pi;
		if (fabs (del) > cs_Pi)
		{
			rtn_val = cs_CNVRT_RNG;
			del = CS_adj2pi (del);
		}
		cos_del = cos (del);

		tmp1 = bpcnc->sin_lat_A * sin_lat;
		tmp2 = bpcnc->cos_lat_A * cos_lat * cos_del;
		zz = acos (tmp1 + tmp2);

		/* To prevent atan2 (0,0) */

		if (fabs (zz) > cs_AnglTest)
		{
			tmp1 = bpcnc->cos_lat_A * tan (lat) - bpcnc->sin_lat_A * cos_del;
			Az = atan2 (sin (del),tmp1);
		}
		else
		{
			Az = bpcnc->Az_AB;
		}
		tmp1 = bpcnc->Az_AB - Az;
		if (fabs (tmp1) > cs_Pi)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp1 = CS_adj2pi (tmp1);
		}
		Az_term = bpcnc->nn * tmp1;
	}

	/* Given appropriate calculation of zz and Az_term for the
	   proper pole, the following is common to both poles. What's
	   happening here is the handling of the discontinuity
	   between the two cones. T is zero only if prj_prm6 and
	   prj_prm7 are both zeros, a situation disallowed by
	   CSbpcncQ. */

	tmp1 = pow (tan (cs_Half * zz),bpcnc->nn);
	if (zz <= bpcnc->pole_dd)
	{
		tmp2 = tan (cs_Half * (bpcnc->pole_dd - zz));
		tmp2 = pow (tmp2,bpcnc->nn);
		alpha = acos ((tmp1 + tmp2) / bpcnc->T);
	}
	else
	{
		/* Turn off the discontinuity correction,
		   the point is beyond the opposite pole.
		   We also consider this to be outside of
		   the domain of the projection. */

		tmp2 = pow (tan (cs_Half * bpcnc->pole_dd),bpcnc->nn);
		alpha = acos (tmp2 / bpcnc->T);
		rtn_val = cs_CNVRT_RNG;
	}

	rho = bpcnc->F * tmp1;
	if (fabs (Az_term) < alpha)
	{
		/* Alpha is a small value, and we never get here unless
		   Az_term is equally as small.  Therefore, the cosine
		   terms have no chance of being zero.

		   Note, the test above determines if the point in
		   question is in the discontinuity zone, a small
		   band which straddles the great circle between
		   the two poles. */

		if (south)
		{
			rho = rho / cos (alpha + Az_term);
		}
		else
		{
			rho = rho / cos (alpha - Az_term);
		}
	}

	xx = rho * sin (Az_term);
	if (south)
	{
		yy = -rho * cos (Az_term) + bpcnc->rho_c;
	}
	else
	{
		yy = rho * cos (Az_term) - bpcnc->rho_c;
	}

	/* Rectify the X and Y to a Y == True North system. */

	xy [XX] = -xx * bpcnc->cos_Az_c - yy * bpcnc->sin_Az_c;
	xy [YY] = -yy * bpcnc->cos_Az_c + xx * bpcnc->sin_Az_c;

	if (bpcnc->quad == 0)
	{
		xy [XX] += bpcnc->x_off;
		xy [YY] += bpcnc->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],bpcnc->x_off,bpcnc->y_off,
							  bpcnc->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSbpcncI (bpcnc,ll,xy);
**
**	struct cs_Bpcnc_ *bpcnc;	structure which defines the parameters
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
**	The ll and xy arguments of this function may point
**	to the same array with no adverse affects.
**
**	All the formulae used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 116-121.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSbpcncI (	Const struct cs_Bpcnc_ *bpcnc,double ll [2],Const double xy [2])
{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Radian;			/* 57.29577    */
	extern double cs_Pi;				/* 3.14159 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0         */
	extern double cs_Two;				/* 2.0         */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_Huge;				/* An approximation of
										   infinity, not so large
										   as to preclude using the
										   value in calculations. */

	int rtn_val;
	int first;

	double xx;
	double yy;

	double lng;
	double lat;

	double xx1;
	double yy1;

	double Az;
	double zz;
	double rho;
	double prv_rho;
	double base_rho;
	double yy_p;
	double Az_ref;
	double alpha;
	double cos_Az;
	double sin_Az;
	double sin_zz;
	double cos_zz;

	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	if (bpcnc->quad == 0)
	{
		xx1 = xy [XX] - bpcnc->x_off;
		yy1 = xy [YY] - bpcnc->y_off;
	}
	else
	{
		CS_quadI (&xx1,&yy1,xy,bpcnc->x_off,bpcnc->y_off,bpcnc->quad);
	}

	xx = -xx1 * bpcnc->cos_Az_c + yy1 * bpcnc->sin_Az_c;
	yy = -xx1 * bpcnc->sin_Az_c - yy1 * bpcnc->cos_Az_c;

	/* At this point, xx and yy are coordinates of the the cartesian
	   system with origin on the great circle connecting the two
	   oblique poles, and halfway between them.  The positive Y axis
	   extends from the origin to oblique pole A.  In the normal use
	   of this projection, the right handed cartesian system causes
	   the positive X axis to extend from the origin into the
	   northern hemisphere. */

	if (xx >= 0.0)
	{
		/* Coordinate mapped using pole B cone. */

		yy_p = bpcnc->rho_c + yy;
		Az_ref = bpcnc->Az_BA;
	}
	else
	{
		/* Coordinate mapped using pole A cone. */

		yy_p = bpcnc->rho_c - yy;
		Az_ref = bpcnc->Az_AB;
	}

	/* Rho is the radius of the circular arc centered at the
	   appropriate oblique pole and passing through the point.
	   Az is the azimuth, relative to the great circle
	   connecting the oblique poles, of the point. */

	rho = base_rho = sqrt (xx * xx + yy_p * yy_p);
	Az = atan2 (xx,yy_p);
	if (fabs (Az) > bpcnc->Az_max)
	{
		/* Opps!!! X and Y are in the pie slice on the
		   back side of the cone. */

		rtn_val = cs_CNVRT_RNG;
		Az = (Az >= 0.0) ? bpcnc->Az_max : -bpcnc->Az_max;
	}

	/* The following iteration may be confusing.  It is actually
	   only used if the point falls in the small region where the
	   two cones intersect and the resulting glitch is fixed
	   by linear interpolation.  The obscure code is dictated
	   by performance and the desire to eliminate duplicate
	   code.

	   Read zz as the angular distance of the point from the
	   appropriate pole which was, essentially, selected above. */

	first = TRUE;
	for (;;)
	{
		/* F can be zero only if prj_prm6 is zero, which is
		   disallowed by CSBpcncQ. */

		zz = cs_Two * atan (pow (rho / bpcnc->F,cs_One / bpcnc->nn));
		if (zz > bpcnc->pole_dd)
		{
			/* The distance to the XY point from the approriate
			   pole exceeds the distance to the opposite pole.
			   Thus, we are technically out of range. */

			rtn_val = cs_CNVRT_RNG;
			zz = bpcnc->pole_dd;
			break;
		}
		if (zz < 0.0)
		{
			/* This should never happen, but just in case. */

			rtn_val = cs_CNVRT_RNG;
			zz = cs_Zero;
			break;
		}
		tmp1 = tan (cs_Half * (bpcnc->pole_dd - zz));
		tmp2 = pow (tmp1,bpcnc->nn);
		tmp1 = pow (tan (cs_Half * zz),bpcnc->nn);

		/* T is zero only if prj_prm6 and prj_prm7 are both
		   zero, which is disallowed by CSbpcncQ. */

		alpha = acos ((tmp1 + tmp2) / bpcnc->T);
		if (first && fabs (Az) >= alpha)
		{
			/* Break if Az indicates that we are not in
			   the overlap zone and this is the first time
			   through the iteration; the normal case
			   (98% of the time). */

			break;
		}

		/* The remainder of this block is the stuff which
		   needs to be iterated in order to find the
		   appropriate value of rho if we are in the linear
		   interpolation zone between the two cones. */

		first = FALSE;
		prv_rho = rho;
		if (xx >= 0.0)
		{
			rho = base_rho * cos (alpha - Az);
		}
		else
		{
			rho = base_rho * cos (alpha + Az);
		}			
		if (fabs (prv_rho - rho) < bpcnc->one_cm)
		{
		    	break;
		}
	}
	Az = Az_ref - Az / bpcnc->nn;

	sin_Az = sin (Az);
	cos_Az = cos (Az);
	sin_zz = sin (zz);
	cos_zz = cos (zz);

	if (xx < 0.0)
	{
		tmp1 = bpcnc->sin_lat_A * cos_zz +
		       bpcnc->cos_lat_A * sin_zz * cos_Az;
		lat = asin (tmp1);

		/* Zz should never be zero, but just in case. */

		if (zz > cs_AnglTest)
		{
			tmp1 = (bpcnc->cos_lat_A / tan (zz)) -
			       (bpcnc->sin_lat_A * cos_Az);
		}
		else
		{
			tmp1 = cs_Huge;
		}

		/* Both sin_Az and tmp1 can never be zero at the
		   same time. */

		tmp2 = atan2 (sin_Az,tmp1);
		if (fabs (tmp2) > cs_Pi)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp2 = CS_adj2pi (tmp2);
		}
		lng = tmp2 + bpcnc->lng_A;
	}
	else
	{
		tmp1 = bpcnc->sin_lat_B * cos_zz +
		       bpcnc->cos_lat_B * sin_zz * cos_Az;
		lat = asin (tmp1);

		if (zz > cs_AnglTest)
		{
			tmp1 = (bpcnc->cos_lat_B / tan (zz)) -
			       (bpcnc->sin_lat_B * cos_Az);
		}
		else
		{
			tmp1 = cs_Huge;
		}
		tmp2 = atan2 (sin_Az,tmp1);
		if (fabs (tmp2) > cs_Pi)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp2 = CS_adj2pi (tmp2);
		}
		lng = bpcnc->lng_B - tmp2;
	}

	if (fabs (lat) > cs_NPTest && rtn_val == cs_CNVRT_NRML)
	{
	    	rtn_val = cs_CNVRT_INDF;
	}

	ll [LNG] = lng * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CSbpcncC (bpcnc,ll);
**
**	struct cs_Bpcnc_ *bpcnc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the BiPolar Oblique Conic
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

double EXP_LVL9 CSbpcncC (Const struct cs_Bpcnc_ *bpcnc,Const double ll [2])
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
	   the mathemagics of the specific projection involved. */
		   
	my_ll [LAT] -= 0.00005;
	status = CSbpcncF (bpcnc,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSbpcncF (bpcnc,xy2,my_ll);
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
**	k = CSbpcncK (bpcnc,ll);
**
**	struct cs_Bpcnc_ *bpcnc;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Bipolar Oblique
**								Conformal Conic projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the grid scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double k;					returns the grid scale factor of the
**								projected coordinate system at that
**								point.
**
**	Synder does give formulas for the scale, but these are
**	inaccurate when the angular distance from the appropriate
**	pole exceeds 90 degrees.  Therefore, we use the empirical
**	technique.
**********************************************************************/

double EXP_LVL9 CSbpcncK (Const struct cs_Bpcnc_ *bpcnc,Const double ll [2])
{
	extern double cs_Degree;		/* pi / 180.0 */
	extern double cs_Mone;			/* -1.0 */
	extern double cs_SclInf;		/* 9.9E+04, the value we return
									   for an infinite scale
									   factor. */
	extern double cs_HlfSecDeg;		/* One half arc second in
									   degrees. */
	extern double cs_Sin1Sec;		/* Sine of one second of arc. */

	int status;

	double kk;
	double ll_dd;
	double xy_dd;
	double del_xx, del_yy;

	double xy1 [2];
	double xy2 [2];
	double ll1 [2];

	/* Establish two points along the parallel which are
	   about 1 second (about 30 meters) apart from each
	   other, with the point in question in the middle.
	   Then convert each point to the equivalent grid
	   coordinates. */

	ll1 [LNG] = ll [LNG] - cs_HlfSecDeg;
	ll1 [LAT] = ll [LAT];
	status = CSbpcncF (bpcnc,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll1 [LNG] = ll [LNG] + cs_HlfSecDeg;
	status = CSbpcncF (bpcnc,xy2,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Calculate the geodetic distance between the two lat/long
	   points. We know that the angular distance is one arc
	   second, and that this distance is along a parallel on
	   a sphere.  Thus, this calculation becomes trivial. */

	ll_dd = cs_Sin1Sec * bpcnc->ka * cos (ll [LAT] * cs_Degree);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the parallel
	   scale factor at the indicated point.  I don't think
	   ll_dd can be zero since we know that ll1 id not the
	   same as ll0.  However, just to be safe: */

	if (fabs (ll_dd) > bpcnc->one_cm)
	{
		kk = xy_dd / ll_dd;
	}
	else
	{
		kk = cs_SclInf;
	}
	return (kk);
}

/**********************************************************************
**	status = CSbpcncL (bpcnc,cnt,pnts);
**
**	struct cs_Bpcnc_ *bpcnc;	the coordinate system against which the
**								check is to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
**	double pnts [][3];			the list of coordinates to be checked.
**	int status;					returns cs_CNVRT_OK if the point, line,
**								or region is completely within the
**								domain of the coordinate system.
**								Otherwise, cs_CNVRT_DOMN is returned.
**
**	This function simply checks the mathematical viability of
**	a coordinate conversion.  It has nothing to do with the
**	useful limits of the coordinate system.
**
**	This function expects that the input lat/longs are normalized
**	if appropriate.
**********************************************************************/

int EXP_LVL9 CSbpcncL (Const struct cs_Bpcnc_ *bpcnc,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	double test_xy [3];

	/* Check each point. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSbpcncF (bpcnc,test_xy,pnts [ii]);
		if (status == cs_CNVRT_RNG)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSbpcncX (bpcnc,cnt,pnts);
**
**	struct cs_Bpcnc_ *bpcnc;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points
**								which define a point, line, or region
**								to be checked.
**	int status;					returns the status of the check; see
**								remarks below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSbpcncL to check lat/long
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

int EXP_LVL9 CSbpcncX (Const struct cs_Bpcnc_ *bpcnc,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double xx,yy;
	double xx1,yy1;

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic skew
	   rectangle which defines the useful range of this
	   projection.  Since this entire region is continuous,
	   this single check is also valid for all lines and
	   regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		/* Convert the X and Y to the skew coordinates.
		   Skew coordinates have an origin on the great
		   circle between the poles, half way between the
		   poles.  The positive Y axis extends towards
		   pole A. */

		if (bpcnc->quad == 0)
		{
			xx1 = pnts [ii][XX] - bpcnc->x_off;
			yy1 = pnts [ii][YY] - bpcnc->y_off;
		}
		else
		{
			CS_quadI (&xx1,&yy1,pnts [ii],bpcnc->x_off,
						      bpcnc->y_off,
						      bpcnc->quad);
		}

		xx = -xx1 * bpcnc->cos_Az_c + yy1 * bpcnc->sin_Az_c;
		yy = -xx1 * bpcnc->sin_Az_c - yy1 * bpcnc->cos_Az_c;
		if (fabs (yy) > bpcnc->rho_c ||
		    fabs (xx) > bpcnc->max_xx)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
