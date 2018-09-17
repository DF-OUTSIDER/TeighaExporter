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

/**********************************************************************
**	err_cnt = CSplycnQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSplycnQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the American Polyconic specific stuff. */

	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}

	/* That's it for American Polyconic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CStrmrtS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate system
**								parameters.
**
**	Normal conventions and CS-MAP standards have NOT
**	been followed in codng this function so that the
**	variable nomenclature could adhere as closely
**	as possible to that used in the reference.
**********************************************************************/

void EXP_LVL9 CSplycnS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/* 57.27.... */
	extern double cs_Degree;			/* 1.0 / 57.27....  */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Ten;				/* 10.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc
										   in radians. */
	extern double cs_Pi_o_2;			/* PI / 2 */

	struct cs_Plycn_ *plycn;

	double sin_lat0;
	double cos_lat0;

	double test_ll [3];
	double test_xy [3];

	plycn = &csprm->proj_prms.plycn;

	plycn->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
	plycn->org_lat = csprm->csdef.org_lat * cs_Degree;
	plycn->x_off = csprm->csdef.x_off;
	plycn->y_off = csprm->csdef.y_off;
	plycn->k = csprm->csdef.scale;
	plycn->ecent = csprm->datum.ecent;
	plycn->e_sq = plycn->ecent * plycn->ecent;
	plycn->e_rad = csprm->datum.e_rad;
	plycn->e_rad_sq = plycn->e_rad * plycn->e_rad;
	plycn->Rk = plycn->e_rad * plycn->k;
	plycn->Rk_ovr_2 = plycn->Rk * cs_Half;
	plycn->Rk_sq = plycn->Rk * plycn->Rk;
	plycn->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* The following value approximates, in cartesian coordinate
	   system units, 0.001 seconds of arc.  If map scale is 1.0,
	   and unit is meter, usually works out to be about 30
	   centimeters. */

	plycn->small_xy = cs_AnglTest * plycn->Rk;

	if (plycn->ecent == 0.0)
	{
		/* Here for a sphere. */

		plycn->Rlat0 = -plycn->Rk * plycn->org_lat;
		plycn->max_rho = plycn->Rk * cs_Pi_o_2;
	}
	else
	{
		/* Here for a ellipsoid. Set up the
		   forward and inverses for M. */

		CSmmFsu (&plycn->mmcofF,plycn->Rk,plycn->e_sq);
		CSmpFsu (&plycn->mpcofF,plycn->e_sq);

		/* Now we compute M0, the M associated with
		   the origin latiude. */

		sin_lat0 = sin (plycn->org_lat);
		cos_lat0 = cos (plycn->org_lat);
		plycn->M0 = CSmmFcal (&plycn->mmcofF,plycn->org_lat,sin_lat0,cos_lat0);

		/* The y coordinate for the equator is -M0. */

		plycn->Rlat0 = -plycn->M0;
		plycn->one_e_sq = cs_One - plycn->e_sq;

		/* Max_rho is the diatance from the equator to the
		   north pole on the ellipsoid. */

		plycn->max_rho = CSmmFcal (&plycn->mmcofF,cs_Pi_o_2,cs_One,cs_Zero);
	}
	plycn->max_rho_sq = plycn->max_rho * plycn->max_rho;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = plycn->cent_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* We're to calculate a useful range.  We don't have to
		   much information to work with.  We'll assume that the
		   map is a relatively small scale map. */
		csprm->min_ll [LNG] = -cs_Ten;
		csprm->max_ll [LNG] =  cs_Ten;
		if (plycn->org_lat >= 0.0)
		{
			csprm->min_ll [LAT] = plycn->org_lat * cs_Radian;
			csprm->max_ll [LAT] = csprm->min_ll [LAT] + cs_Ten;
		}
		else
		{
			csprm->max_ll [LAT] = plycn->org_lat * cs_Radian;
			csprm->min_ll [LAT] = csprm->max_ll [LAT] - cs_Ten;
		}
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
		   The setup is virtually complete, so we can use CStrmerF
		   to calculate some values. */
		test_ll [LNG] = CS_adj180 (csprm->min_ll [LNG] + csprm->cent_mer);
		test_ll [LAT] = csprm->min_ll [LAT];
		if (fabs (csprm->max_ll [LAT]) < fabs (csprm->min_ll [LAT]))
		{
			test_ll [LAT] = csprm->max_ll [LAT];
		}
		CSplycnF (plycn,test_xy,test_ll);				/*lint !e534 */
		csprm->min_xy [XX] = test_xy [XX] - plycn->x_off;
		csprm->max_xy [XX] = -csprm->min_xy [XX];

		test_ll [LNG] = csprm->cent_mer;
		test_ll [LAT] = csprm->min_ll [LAT];
		CSplycnF (plycn,test_xy,test_ll);						/*lint !e534 */
		csprm->min_xy [YY] = test_xy [YY] - plycn->y_off;

		test_ll [LAT] = csprm->max_ll [LAT];
		CSplycnF (plycn,test_xy,test_ll);						/*lint !e534 */
		csprm->max_xy [YY] = test_xy [YY] - plycn->y_off;

		CS_quadMM (csprm->min_xy,csprm->max_xy,plycn->x_off,
											   plycn->y_off,
											   plycn->quad);
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

	/* That's all the calculations we can do at this time. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSplycnF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSplycnI;
	csprm->cs_scale = (cs_SCALE_CAST)CSplycnH;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSplycnK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSplycnH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSplycnC;
	csprm->llchk    = (cs_LLCHK_CAST)CSplycnL;
	csprm->xychk    = (cs_XYCHK_CAST)CSplycnX;

	return;
}

/**********************************************************************
**	rtn_val = CSplycnF (plycn,xy,ll);
**
**	struct cs_Plycn_ *plycn;	structure containing all parameters necessary
**								for the transformation.
**	double xy [2];				converted values are returned here, x ([0])
**								and y ([1]).
**	double ll [2];				the longitude ([0]) and latitude ([1]) values
**								to be converted, in degrees.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and the xy arrays may share the same memory
**	with no adverse affects.
**********************************************************************/

int EXP_LVL9 CSplycnF (Const struct cs_Plycn_ *plycn,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Pi;				/* 3.14159.... */
	extern double cs_Mpi;				/* -3.14159.... */
	extern double cs_Two_pi;			/* 2 Pi  */
	extern double cs_Pi_o_2;			/* 1.570795.... */
	extern double cs_Huge;				/* very large number, but not so large that
										   we can't compute with it. */
	extern double cs_AnglTest;			/* 0.001 arc seconds, in
										   radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_EETest;			/* 0.001 seconds of arc
										   short of 90 degrees,
										   in radians. */
	extern double cs_WETest;			/* 0.001 seconds of arc
										   short of -90 degrees,
										   in radians. */

	int rtn_val;

	double lng;
	double lat;
	double sin_lat;
	double cos_lat;
	double del_lng;
	double cot_lat;
	double E;
	double M;
	double N;
	double sin_E;
	double one_cos_E;
	double N_cot_lat;

	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	lng = ll [0] * cs_Degree;
	lat = ll [1] * cs_Degree;

	if (fabs (lat) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}

	/* Compute, and normalize del_lng.  A del_lng greater than
	   cs_Pi_o_2 is a problem, we can't do the inverse. */

	del_lng = lng - plycn->cent_lng;
	if      (del_lng > cs_Pi  && plycn->cent_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && plycn->cent_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) >= cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj1pi (del_lng);
		if (del_lng > cs_EETest) del_lng = cs_EETest;
		if (del_lng < cs_WETest) del_lng = cs_WETest;
	}

	/* The equator is a special case.  To keep things from
	   blowing up, we will assume that anything within
	   approximately 30 centimeters of the equator is actually
	   on the the equator.  Remember, lat is in radians at this
	   point. */

	if (fabs (lat) <= cs_AnglTest)
	{
		lat = cs_Zero;
		cot_lat = cs_Huge;
	}
	else
	{
		cot_lat = cs_One / tan (lat);
	}

	/* OK, now for the rest of the calculations common to both the
	   sphere and the ellipsoid. */

	sin_lat = sin (lat);
	E = del_lng * sin_lat;
	sin_E = sin (E);
	one_cos_E = cs_One - cos (E);

	if (plycn->ecent == 0.0)
	{
		/* Here for a spherical ellipsoid.  The equator
		   is a special case. */

		if (fabs (lat) <= cs_AnglTest)
		{
			/* If the point is on the equator.  This
			   is easy. */

			xy [XX] = plycn->Rk * del_lng;
			xy [YY] = -plycn->Rk * plycn->org_lat;
		}
		else
		{
			/* We're not on the equator, so the following
			   should compute without blowing up. */

			xy [XX] = plycn->Rk * cot_lat * sin_E;			/*lint !e644 */
			tmp1 = cot_lat * one_cos_E;
			xy [YY] = plycn->Rk * (lat - plycn->org_lat + tmp1);
		}
	}
	else
	{
		/* Here for an ellipsoid.  First, we check for
		   a point on the equator for which cot (lat)
		   is not defined. */

		if (lat == 0.0)
		{
			xy [XX] = plycn->Rk * del_lng;
			xy [YY] = -plycn->M0;
		}
		else
		{
			/* Here for a point which is not on the equator,
			   i.e. cot (lat) is defined.  First we must
			   compute M.  M is the north/south distance
			   from the equator to the point on the
			   ellipsoid. */

			cos_lat = cos (lat);
			M = CSmmFcal (&plycn->mmcofF,lat,sin_lat,cos_lat);

			/* Now we compute N.  N is the cone constant which,
			   in the polyconic, varies with the latitude. */

			tmp1 = cs_One - (plycn->e_sq * sin_lat * sin_lat);
			N = plycn->Rk / sqrt (tmp1);

			/* Compute N * cot_lat.  We need this for both X
			   and Y. */

			N_cot_lat = N * cot_lat;

			xy [XX] = N_cot_lat * sin_E;
			xy [YY] = M - plycn->M0 + (N_cot_lat * one_cos_E);
		}
	}

	/* Add the false origin. */

	if (plycn->quad == 0)
	{
		xy [XX] += plycn->x_off;
		xy [YY] += plycn->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],plycn->x_off,plycn->y_off,
							  plycn->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSplycnI (plycn,ll,xy);
**
**	struct cs_Plycn_ *plycn;	structure which defines the parameters
**								in effect for the transformation.
**	double ll [2];				the longitude ([0]) and latitude ([1]),
**								in degrees, are returned here.
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
**	U. S. Geological Survey", pages 128-130.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSplycnI (Const struct cs_Plycn_ *plycn,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.29577..... */
	extern double cs_Pi;				/*  Pi = 3.14159... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole
										   in radians. */
	extern double cs_AnglTest;			/* 0.001 seconds of arc
										   in radians. */

	int itr_cnt;
	int rtn_val;

	double x;
	double y;

	double A;
	double B;
	double C;
	double M;
	double Mp;
	double Ma;
	double Ma_sq;
	double lat;
	double rho;
	double rho_sq;
	double del_lng;
	double del_lat;
	double last_lat;
	double tan_lat;
	double sin_lat;
	double cos_lat;
	double sin_2lat;
	double tmp1;
	double tmp2;
	double tmp3;
	double tmp4;

	rtn_val = cs_CNVRT_NRML;

	/* Remove the false easting and northing from the provided
	   coordinates. */

	if (plycn->quad == 0)
	{
		x = xy [XX] - plycn->x_off;
		y = xy [YY] - plycn->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,plycn->x_off,plycn->y_off,plycn->quad);
	}

	/* Are we in range? We attempt to calculate rho, the cartesian
	   distance from org_lng and the equator to the given point.
	   Tmp1 is y relative to the equator. */

	tmp1 = y - plycn->Rlat0;
	rho_sq = x * x + tmp1 * tmp1;
	if (rho_sq > plycn->max_rho_sq)
	{
		/* We are out of range. Replace X and Y with values
		   which are in range, and will produce a somewhat
		   rational result. */

		rtn_val = cs_CNVRT_RNG;
		rho = sqrt (rho_sq);
		x *= plycn->max_rho / rho;
		y = plycn->Rlat0 + tmp1 * plycn->max_rho / rho;
	}

	/* If the y coordinate provided is very close to the equator,
	   we put it on the equator and skip the mathemagics which
	   would have a hard time. */

	if (fabs (y - plycn->Rlat0) < plycn->small_xy)
	{
		/* We're at the equator, this code works for both the
		   ellipsoid and the sphere. */

		del_lng = x / plycn->Rk;
		lat = cs_Zero;
	}
	else if (plycn->ecent == 0.0)
	{
		/* Here for the sphere. */

		A = plycn->org_lat + (y / plycn->Rk);
		B = ((x * x) / plycn->Rk_sq) + (A * A);

		/* The following doesn't converge if fabs (del_lng)
		   is greater than 90 degrees.  Of course, we don't
		   know what del_lng is yet?  Therefore, some extraneous
		   stuff has to be included. */

		lat = A;
		itr_cnt = 10;
		do
		{
			last_lat = lat;
			tan_lat = tan (lat);
			tmp1 = (lat * tan_lat) + cs_One;
			tmp2 = ((lat * lat) + B) * tan_lat;
			tmp3 = (A * tmp1) - lat - cs_Half * tmp2;
			tmp4 = ((lat - A) / tan_lat) - cs_One;
			lat -= tmp3 / tmp4;
			del_lat = lat - last_lat;
		} while (fabs (del_lat) > cs_AnglTest && --itr_cnt >= 0);

		/* If the thing didn't converge in 10 iterations, we assume
		   we are out of the acceptable range. */

		if (fabs (del_lat) > cs_AnglTest)
		{
			rtn_val = cs_CNVRT_RNG;

			/* Our initial estimate of latitude is probably
			   better than where the non-converging series
			   put us. */

			lat = A;
		}

		if (fabs (lat) >= cs_NPTest)
		{
			/* We're at a pole, any longitude will do. */

			del_lng = cs_Zero;
			if (rtn_val == cs_CNVRT_NRML)
			{
				rtn_val = cs_CNVRT_INDF;
			}
		}
		else
		{
			tan_lat = tan (lat);
			tmp1 = x * tan_lat / plycn->Rk;

			/* Keep asin from blowing up.  tmp1 should never
			   really be greater than one, but noise in the
			   low order bits sometimes causes this.  Thus,
			   limiting at +- 1.0 is fine.  Note, since this
			   is rare, and since we assume fabs is very
			   fast, we code as follows: */

			if (fabs (tmp1) >= cs_One)
			{
				tmp1 = (tmp1 > 0.0) ? cs_One : cs_Mone;
			}
			del_lng = asin (tmp1) / sin (lat);
		}
	}
	else
	{
		/* Here for the ellipsoid. */

		A = (plycn->M0 + y) / plycn->Rk;
		B = ((x * x) / plycn->Rk_sq) + (A * A);

		lat = A;
		itr_cnt = 10;
		do
		{
			/* Save the last latitude value to see
			   when we have converged. */

			last_lat = lat;

			/* Compute the value of C. */

			sin_lat = sin (lat);
			cos_lat = cos (lat);
			tan_lat = sin_lat / cos_lat;
			sin_2lat = sin (lat + lat);

			tmp1 = cs_One - plycn->e_sq * sin_lat * sin_lat;
			C = sqrt (tmp1) * tan_lat;

			/* Compute the value of M based on the
			   current latitude estimate. */

			M  = CSmmFcal (&plycn->mmcofF,lat,sin_lat,cos_lat);
			Mp = CSmpFcal (&plycn->mpcofF,lat);

			/* Compute the value of Ma. */

			Ma = M / plycn->Rk;
			Ma_sq = Ma * Ma;

			/* Compute the new guestimate for the
			   latitude. */

			tmp1 = A * (C * Ma + cs_One);
			tmp1 -= Ma;
			tmp1 -= cs_Half * C * (Ma_sq + B);

			tmp2 = Ma_sq + B - (cs_Two * A * Ma);
			tmp3 = (plycn->e_sq * sin_2lat * tmp2) / (cs_Four * C);
			tmp2 = (C * Mp) - (cs_Two / sin_2lat);
			tmp3 += (A - Ma) * tmp2;
			tmp3 -= Mp;
			lat -= tmp1 / tmp3;
			del_lat = lat - last_lat;
		} while (fabs (del_lat) > cs_AnglTest && --itr_cnt >= 0);

		/* Again, if the iteration failed to converge, we assume
		   that we are out of range. */

		if (fabs (del_lat) > cs_AnglTest)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = A;
		}

		if (fabs (lat) >= cs_NPTest)
		{
			/* At the poles, longitude is (mathematically)
			   undefined and any longitude will do. */

			if (rtn_val == cs_CNVRT_NRML)
			{
				rtn_val = cs_CNVRT_INDF;
			}
			del_lng = cs_Zero;
		}
		else
		{
			tan_lat = tan (lat);
			sin_lat = sin (lat);
			tmp1 = cs_One - plycn->e_sq * sin_lat * sin_lat;
			C = sqrt (tmp1) * tan_lat;
			tmp1 = x * C / plycn->Rk;

			/* Keep asin from blowing up.  tmp1 should never
			   really be greater than one, but noise in the
			   low order bits sometimes causes this.  Thus,
			   limiting at +- 1.0 is fine.  Note, since this
			   is rare, and since we assume fabs is very
			   fast, we code as follows: */

			if (fabs (tmp1) >= cs_One)
			{
				tmp1 = (tmp1 > 0.0) ? cs_One : cs_Mone;
			}
			del_lng = asin (tmp1) / sin_lat;
		}
	}

	/* Convert to degrees. */

	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}
	ll [LNG] = (del_lng + plycn->cent_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSplycnC (plycn,ll);
**
**	struct cs_Plycn_ *plycn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the American Polyconic
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

double EXP_LVL9 CSplycnC (Const struct cs_Plycn_ *plycn,Const double ll [2])
{
	extern double cs_Radian;			/* 57.2957... */
	extern double cs_Km360;				/* -360.0, the value which
										   we return if provided
										   with a bogus point. */
	int status;

	double del_xx;
	double del_yy;
	double gamma;			/* some folks call this alpha */

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
	status = CSplycnF (plycn,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSplycnF (plycn,xy2,my_ll);
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
**	k = CSplycnK (plycn,ll);
**
**	struct cs_Plycn_ *plycn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Polyconic projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double k;					returns the grid scale factor along a
**								parallel of the projected coordinate system
**								at the specified point.
**
**	I wish the were all this easy.
**********************************************************************/

double EXP_LVL9 CSplycnK (Const struct cs_Plycn_ *plycn,Const double ll [2])
{
	extern double cs_One;

	/* The grid scale factor along a parallel is always
	   1.0 at any point in the coordinate system. */

	return (cs_One);
}								/*lint !e715 */

/**********************************************************************
**	h = CSplycnH (plycn,ll);
**
**	struct cs_Plycn_ *plycn;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Polyconic projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double h;					returns the gris scale factor along a
**								meridian of the coordinate system at the
**								specified point.
**********************************************************************/

double EXP_LVL9 CSplycnH (Const struct cs_Plycn_ *plycn,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north
										   pole, in radians. */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */

	double lng;
	double lat;
	double h;

	double del_lng;
	double cos_lat;
	double sin_lat;
	double tan_lat;
	double sec_lat;
	double D;
	double cos_D;
	double sin_E;
	double E;
	double cos_E;
	double Mp;

	double tmp1;
	double tmp2;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;

	if (fabs (lat) > cs_NPTest)
	{
		/* At the poles. */

		return (cs_One);
	}

	del_lng = CS_adj2pi (lng - plycn->cent_lng);

	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* We have already handle the case where cos_lat is zero. */

	sec_lat = cs_One / cos_lat;
	E = del_lng * sin_lat;
	sin_E = sin (E);
	cos_E = cos (E);

	if (plycn->ecent == 0.0)
	{
		/* Here for the sphere. */

		if (fabs (lat) >= cs_AnglTest)
		{
			/* Secant squared == cos_E??? Only at the equator;
			   sin_lat == 0??? ONly at the equator. */

			tmp1 = (E - sin_E) / (sec_lat * sec_lat - cos_E);
			D = atan (tmp1);
			h = (cs_One - cos_lat * cos_lat * cos_E) /
				     (sin_lat * sin_lat * cos (D));
		}
		else
		{
			h = cs_One + (del_lng * del_lng * cs_Half);
		}
	}
	else
	{
		/* Here for the ellipsoid. */

		if (fabs (lat) < cs_AnglTest)
		{
			/* At the equator. */

			Mp = CSmpFcal (&plycn->mpcofF,cs_Zero);
			h = (Mp + (del_lng * del_lng * cs_Half)) / plycn->one_e_sq;
		}
		else
		{
			/* Elsewhere, sin_lat != 0. Since we've already
			   filtered out the poles, cos_lat != 0 also. */

			tan_lat = sin_lat / cos_lat;
			tmp1 = plycn->e_sq * sin_lat * sin_lat;
			tmp2 = tmp1 / (cs_One - tmp1);
			tmp2 = (sec_lat * sec_lat) - cos_E - tmp2;
			D = atan ((E - sin_E) / tmp2);
			cos_D = cos (D);
			tmp2 = sin (cs_Half * E);
			tmp2 = cs_Two * ((cs_One - tmp1) * (tmp2 * tmp2));
			tmp2 /= (tan_lat * tan_lat);
			h = (plycn->one_e_sq + tmp2) / (plycn->one_e_sq * cos_D);
		}
	}

	return (h);
}

/**********************************************************************
**	status = CSplycnL (plycn,cnt,pnts);
**
**	struct cs_Plycn_ *plycn;	the coordinate system against which the check is
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

int EXP_LVL9 CSplycnL (Const struct cs_Plycn_ *plycn,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi_o_2;		/* PI over 2 */

	int ii;
	int status;

	double lat;
	double del_lng;

	/* Check all the points.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		del_lng = (pnts [ii][LNG] * cs_Degree) - plycn->cent_lng;
		if (fabs (del_lng) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSplycnX (plycn,cnt,pnts);
**
**	struct cs_Plycn_ *plycn;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSplycnL to check lat/long
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

int EXP_LVL9 CSplycnX (Const struct cs_Plycn_ *plycn,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double my_xx;
	double my_yy;
	double rho_sq;

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic range. */

	for (ii = 0;ii < cnt;ii++)
	{
		if (plycn->quad == 0)
		{
			my_xx = pnts [ii][XX] - plycn->x_off;
			my_yy = pnts [ii][YY] - plycn->y_off;
		}
		else
		{
			CS_quadI (&my_xx,&my_yy,pnts [ii],plycn->x_off,
											  plycn->y_off,
											  plycn->quad);
		}
		my_yy -= plycn->Rlat0;
		rho_sq = my_xx * my_xx + my_yy * my_yy;
		if (rho_sq > plycn->max_rho_sq)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
