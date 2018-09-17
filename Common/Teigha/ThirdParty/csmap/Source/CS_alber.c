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
**	err_cnt = CSalberQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	ununsed in this module.
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
**	equal to +180/+90.  (Exception, the south pole can be speciied
**	as the origin latitude.)
**********************************************************************/

int EXP_LVL9 CSalberQ (	Const struct cs_Csdef_ *cs_def,
						unsigned short prj_code,
						int err_list [],
						int list_sz)
{
	extern double cs_MinLng;			/* -180.0 */
	extern double cs_MinLat;			/* - 90.0 */
	extern double cs_MaxLng;			/* +180.0 */
	extern double cs_MaxLat;			/* + 90.0 */
	extern double cs_MinLatFz;			/*  -89.999 */
	extern double cs_MaxLatFz;			/* + 89.999 */
	extern double cs_ParmTest;			/* 2.78E-05 */

	int err_cnt;
	int sign1, sign2;
	
	double tmp;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Alber's Equal Area Conic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}
	if (cs_def->prj_prm1 <= cs_MinLatFz || cs_def->prj_prm1 >= cs_MaxLatFz)
	{
		/* Standard parallels can't be the pole. */
		
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
	}
	if (cs_def->prj_prm2 <= cs_MinLatFz || cs_def->prj_prm2 >= cs_MaxLatFz)
	{
		/* Standard parallels can't be the pole. */
		
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
	}
	
	if (fabs (cs_def->prj_prm1 - cs_def->prj_prm2) <= cs_ParmTest)
	{
		/* Here if the standard parallels are the same.  In this
		   case, they cannot be zero. */

		if (fabs (cs_def->prj_prm1) < cs_ParmTest ||
		    fabs (cs_def->prj_prm2) < cs_ParmTest)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
		}
	}
	else
	{
		/* The parallels may be on opposite sides of the equator,
		   but not equidistant from the equator. */
		   
		sign1 = (cs_def->prj_prm1 > 0.0) ? 0 : 1;
		sign2 = (cs_def->prj_prm2 > 0.0) ? 0 : 1;
		if (sign1 ^ sign2)
		{
			tmp = fabs (cs_def->prj_prm1) - fabs (cs_def->prj_prm2);
			if (fabs (tmp) < cs_ParmTest)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
			}
		}
	}

	/* That's it for Alber's Equal Area Conic. */
	return (err_cnt + 1);
}
/**********************************************************************
**	CSalberS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**********************************************************************/

void EXP_LVL9 CSalberS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / 57.29577.... */
	extern double cs_Radian;			/* 57.29577....       */
	extern double cs_Pi;				/* 3.14159.....       */
	extern double cs_Half;				/* 0.5                */
	extern double cs_One;				/* 1.0                */
	extern double cs_Two;				/* 2.0                */
	extern double cs_Five;				/* 5.0                */
	extern double cs_K45;				/* 45.0               */
	extern double cs_Km45;				/* -45.0              */
	extern double cs_AnglTest;			/* 0.001 arc seconds less
										   than 90 degrees in
										   radians. */
	extern double cs_MinLatFz;			/* 0.1 seconds of arc
										   short of the south
										   pole in degrees. */
	extern double cs_MaxLatFz;			/* 0.1 seconds of arc
										   short of the north
										   pole in degrees. */

	double two_n;
	double two_e;
	double term_a;
	double sin_lat0;
	double sin_lat1;
	double sin_lat2;
	double cos_lat1;
	double cos_lat2;
	double sin_sq;

	double q0;
	double q1;
	double q2;
	double m1_sq;
	double m2_sq;
	double rhoMin, rhoMax;

	double tmp1;
	double tmp2;

	struct cs_Alber_ *alber;

	alber = &csprm->proj_prms.alber;

	/* Transfer the necessary arguments to the
	   "alber" structure.  Notice, the conversion
	   from degrees to radians which is performed
	   in the process. */

	alber->ref_lat1 = csprm->csdef.prj_prm1 * cs_Degree;
	alber->ref_lat2 = csprm->csdef.prj_prm2 * cs_Degree;
	alber->org_lng = csprm->csdef.org_lng * cs_Degree;
	alber->org_lat = csprm->csdef.org_lat * cs_Degree;
	alber->k = csprm->csdef.scale;
	alber->x_off = csprm->csdef.x_off;
	alber->y_off = csprm->csdef.y_off;
	alber->ka = csprm->datum.e_rad * alber->k;
	alber->ecent = csprm->datum.ecent;
	alber->e_sq = alber->ecent * alber->ecent;
	alber->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Some calculations which are common to the sphere and
	   ellipsoid formulae. */

	sin_lat0 = sin (alber->org_lat);
	sin_lat1 = sin (alber->ref_lat1);
	sin_lat2 = sin (alber->ref_lat2);
	cos_lat1 = cos (alber->ref_lat1);
	cos_lat2 = cos (alber->ref_lat2);

	/* If the ecentricity is zero, we have a sphere.  The
	   set up is a bit different in this case. */

	if (csprm->datum.ecent == 0.0)
	{
		if (fabs (alber->ref_lat1 - alber->ref_lat2) > cs_AnglTest)
		{
			alber->n = (sin_lat1 + sin_lat2) * cs_Half;
		}
		else
		{
			alber->n = sin_lat1;
		}
		two_n = cs_Two * alber->n;
		alber->R_ovr_n = csprm->datum.e_rad * alber->k / alber->n;
		alber->C = (cos_lat1 * cos_lat1) + (two_n * sin_lat1);
		tmp1 = alber->C - (two_n * sin_lat0);
		alber->rho0 = alber->R_ovr_n * sqrt (tmp1);

		/* Compute min_rho and max_rho for range checking.  Rho_min
		   and max_rho are positive regardless of the focus pole. */

		if (alber->n >= 0.0)
		{
			alber->min_rho = alber->R_ovr_n * sqrt (alber->C - two_n);
			alber->max_rho = alber->R_ovr_n * sqrt (alber->C + two_n);
		}
		else
		{
			alber->min_rho = -alber->R_ovr_n * sqrt (alber->C + two_n);
			alber->max_rho = -alber->R_ovr_n * sqrt (alber->C - two_n);
		}
	}
	else
	{
		/* Here for the ellipsoidal calculations.  Some
		   general terms first. */

		two_e = 2.0 * alber->ecent;
		alber->one_o_2e = cs_One / two_e;
		alber->one__esq = cs_One - alber->e_sq;

		/* We do all of the variables with a zero
		   subscript first. */

		sin_sq = sin_lat0 * sin_lat0;
		term_a = cs_One - (alber->e_sq * sin_sq);

		/* Now can do q0. */

		tmp1 = alber->ecent * sin_lat0;
		tmp1 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp1 = log (tmp1) * alber->one_o_2e;
		tmp2 = sin_lat0 / term_a;
		q0 = alber->one__esq * (tmp2 - tmp1);

		/* Now subscript 1. */

		sin_sq = sin_lat1 * sin_lat1;
		term_a = cs_One - (alber->e_sq * sin_sq);

		/* Now can do q1. */

		tmp1 = alber->ecent * sin_lat1;
		tmp1 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp1 = log (tmp1) * alber->one_o_2e;
		tmp2 = sin_lat1 / term_a;
		q1 = alber->one__esq * (tmp2 - tmp1);

		/* Now m1 squared. */

		m1_sq = cos_lat1 * cos_lat1 / term_a;

		/* Now subscript 2. */

		sin_sq = sin_lat2 * sin_lat2;
		term_a = cs_One - (alber->e_sq * sin_sq);

		/* Now can do q1. */

		tmp1 = alber->ecent * sin_lat2;
		tmp1 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp1 = log (tmp1) * alber->one_o_2e;
		tmp2 = sin_lat2 / term_a;
		q2 = alber->one__esq * (tmp2 - tmp1);

		/* Now m2 squared. */

		m2_sq = cos_lat2 * cos_lat2 / term_a;

		/* Now for some of the other variables which remain
		   constant for a particular coordinate system.  Normally
		   we have two distinct reference parallels, but it is
		   possible that there is one (i.e. the two values
		   are the same). */

		if (fabs (alber->ref_lat1 - alber->ref_lat2) > cs_AnglTest)
		{
			alber->n = (m1_sq - m2_sq) / (q2 - q1);
		}
		else
		{
			alber->n = sin_lat1;
		}
		two_n = cs_Two * alber->n;

		alber->R_ovr_n = csprm->datum.e_rad * alber->k / alber->n;
		alber->C = m1_sq + (alber->n * q1);
		tmp1 = alber->C - (alber->n * q0);
		alber->rho0 = alber->R_ovr_n * sqrt (tmp1);

		/* Calculate values for min_rho and max_rho which are
		   positive regardless of the focus pole. */

		if (alber->n >= 0.0)
		{
			/* Snyder equations 3-12 & 14-12, with sin phi
			   set to +1. */

			tmp1 = (cs_One - alber->ecent) / (cs_One + alber->ecent);
			tmp1 = log (tmp1) * alber->one_o_2e;
			tmp1 = cs_One - tmp1 * alber->one__esq;
			tmp1 = alber->C - (alber->n * tmp1);
			alber->min_rho = alber->R_ovr_n * sqrt (tmp1);

			/* Snyder equations 3-12 & 14-12, with sin phi
			   set to -1. */

			tmp1 = (cs_One + alber->ecent) / (cs_One - alber->ecent);
			tmp1 = log (tmp1) * alber->one_o_2e;
			tmp1 = cs_One + tmp1 * alber->one__esq;
			tmp1 = alber->C + (alber->n * tmp1);
			alber->max_rho = alber->R_ovr_n * sqrt (tmp1);
		}
		else
		{
			/* Sine phi = -1 */

			tmp1 = (cs_One - alber->ecent) / (cs_One + alber->ecent);
			tmp1 = log (tmp1) * alber->one_o_2e;
			tmp1 = cs_One - tmp1 * alber->one__esq;
			tmp1 = alber->C - (alber->n * tmp1);
			alber->max_rho = -alber->R_ovr_n * sqrt (tmp1);

			/* Sine phi = +1 */

			tmp1 = (cs_One + alber->ecent) / (cs_One - alber->ecent);
			tmp1 = log (tmp1) * alber->one_o_2e;
			tmp1 = cs_One + tmp1 * alber->one__esq;
			tmp1 = alber->C + (alber->n * tmp1);
			alber->min_rho = -alber->R_ovr_n * sqrt (tmp1);
		}

		/* Set up the coefficients for the inverse series
		   calculation. */

		CSbtIsu (&alber->btcoefI,alber->e_sq);

		/* And the factor to convert q's into beta's. */

		tmp1 = (cs_One - alber->ecent) / (cs_One + alber->ecent);
		tmp1 = alber->one__esq * alber->one_o_2e * log (tmp1);
		alber->beta_term = cs_One - tmp1;
	}

	/* Set up theta_max, the maximum polar angle. */

	alber->theta_max = cs_Pi * fabs (alber->n);
	alber->one_mm = 0.001 * alber->k;

	/* Set up the useful range limit values.  If the definition
	   doesn't carry any, we will compute some. */

	csprm->cent_mer = alber->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* Calculate a rather liberal useful range.  Longitudes
		   are easy. */

		csprm->min_ll [LNG] = cs_Km45;
		csprm->max_ll [LNG] = cs_K45;

		/* Start with the min/max latitudes equal to the
		   standard parallels. Careful here, we don't require
		   standard parallel 1 to be north of parallel 2. */

		tmp1 = alber->ref_lat1 - alber->ref_lat2;
		if (tmp1 >= 0.0)
		{
			csprm->min_ll [LAT] = alber->ref_lat2 * cs_Radian;
			csprm->max_ll [LAT] = alber->ref_lat1 * cs_Radian;
		}
		else
		{
			csprm->min_ll [LAT] = alber->ref_lat1 * cs_Radian;
			csprm->max_ll [LAT] = alber->ref_lat2 * cs_Radian;
		}

		/* Widen the coverage as is appropriate. We'll widen by
		   10 degrees (five in each direction), unless the
		   difference between the two standard parallels is
		   more than five. */

		tmp1 = fabs (tmp1) * cs_Radian;
		if (tmp1 < cs_Five) tmp1 = cs_Five;
		csprm->min_ll [LAT] -= tmp1;
		csprm->max_ll [LAT] += tmp1;

		/* Make sure we haven't gone past either pole. */

		if (csprm->min_ll [LAT] < cs_MinLatFz)
		{
			csprm->min_ll [LAT] = cs_MinLatFz;
		}
		if (csprm->max_ll [LAT] > cs_MaxLatFz)
		{
			csprm->max_ll [LAT] = cs_MaxLatFz;
		}
	}
	else
	{
		/* Use whatever the defintion gives us, without
		   checking.  We do convert the longitudes from
		   absolute to relative. */

		csprm->min_ll [LNG] = CS_adj180 (csprm->csdef.ll_min [LNG] - csprm->cent_mer);
		csprm->min_ll [LAT] = csprm->csdef.ll_min [LAT];
		csprm->max_ll [LNG] = CS_adj180 (csprm->csdef.ll_max [LNG] - csprm->cent_mer);
		csprm->max_ll [LAT] = csprm->csdef.ll_max [LAT];
	}

	/* Similarly with the XY checking information. */

	if (csprm->csdef.xy_min [XX] == 0.0 &&
	    csprm->csdef.xy_max [XX] == 0.0)
	{
		if (alber->n >= 0.0)
		{
			/* Focus pole is north pole. */
			rhoMax = alber->R_ovr_n * sqrt (alber->C - two_n * sin (csprm->min_ll [LAT] * cs_Degree));
			rhoMin = alber->R_ovr_n * sqrt (alber->C - two_n * sin (csprm->max_ll [LAT] * cs_Degree));

			tmp1 = csprm->min_ll [LNG] * cs_Degree;
			csprm->min_xy [XX] = rhoMax * alber->n * sin (tmp1) + alber->x_off;
			tmp1 = csprm->max_ll [LNG] * cs_Degree;
			csprm->max_xy [XX] = rhoMax * alber->n * sin (tmp1) + alber->x_off;

			csprm->min_xy [YY] = alber->rho0 - rhoMax + alber->y_off;
			csprm->max_xy [YY] = alber->rho0 - rhoMin + alber->y_off;
		}
		else
		{
			/* Focus pole is south pole, curvature of the parallel
			   arcs is "up side down". */
			rhoMax = alber->R_ovr_n * sqrt (alber->C - two_n * sin (csprm->max_ll [LAT] * cs_Degree));
			rhoMin = alber->R_ovr_n * sqrt (alber->C - two_n * sin (csprm->min_ll [LAT] * cs_Degree));

			tmp1 = csprm->min_ll [LNG] * cs_Degree;
			csprm->min_xy [XX] = rhoMax * alber->n * sin (tmp1) + alber->x_off;
			tmp1 = csprm->max_ll [LNG] * cs_Degree;
			csprm->max_xy [XX] = rhoMax * alber->n * sin (tmp1) + alber->x_off;

			csprm->min_xy [YY] = alber->rho0 - rhoMin + alber->y_off;
			csprm->max_xy [YY] = alber->rho0 - rhoMax + alber->y_off;
		}
	}
	else
	{
		csprm->min_xy [XX] = csprm->csdef.xy_min [XX];
		csprm->min_xy [YY] = csprm->csdef.xy_min [YY];
		csprm->max_xy [XX] = csprm->csdef.xy_max [XX];
		csprm->max_xy [YY] = csprm->csdef.xy_max [YY];
	}

	/* That's all the calculations.  Stuff some function
	   addresses and we are done. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSalberF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSalberI;
	csprm->cs_scale = (cs_SCALE_CAST)CSalberK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSalberK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSalberH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSalberC;
	csprm->llchk    = (cs_LLCHK_CAST)CSalberL;
	csprm->xychk    = (cs_XYCHK_CAST)CSalberX;

	return;
}
/**********************************************************************
**	rtn_val = CSalberF (alber,xy,ll);
**
**	struct cs_Alber_ *alber;	structure which defines the parameters
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
**	This function calculates the Albers Equal Area Conic
**	projection and returns values in the coordinate system
**	described in the cs_Alber_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 95-98.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSalberF (Const struct cs_Alber_ *alber,double xy [2],Const double ll [2])
{
	extern double cs_Degree;		/* 1.0 / 57.295.... */
	extern double cs_Pi;			/* 3.14159... */
	extern double cs_Mpi;			/* -3.14159... */
	extern double cs_Two_pi;		/* 2 pi */
	extern double cs_Pi_o_2;		/* PI / 2 */
	extern double cs_One;			/* 1.0 */
	extern double cs_Two;			/* 2.0 */

	int rtn_val;

	double lng;		/* The given longitude, after conversion
					   to radians. */
	double lat;		/* The given latitude after conversion
					   to radians. */
	double del_lng;
	double rho;
	double theta;

	double q;
	double sin_lat;
	double sin_sq;
	double tmp1;
	double tmp2;
	double tmp3;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity
	   of the dataum in use is 0.0 exactly, we
	   shall use the spherical formulae.  There
	   is a miminal amount of stuff which is
	   common to both which we perform first.

	   Convert the latitude and longitude to radians. */

	lng = cs_Degree * ll [0];
	lat = cs_Degree * ll [1];

	/* Some calculations common to both the sphere and the
	   ellipsoid. Note, we can use any value of longitude, as
	   we can return an accurate and reversible result for
	   any supplied value.

	   Theta is essentially the azimuth of the line of longitude,
	   or the angle of the polar coordinates for the indicated
	   point. */

	del_lng = lng - alber->org_lng;
	if      (del_lng > cs_Pi  && alber->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && alber->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2piI (del_lng);
	}
	theta = del_lng * alber->n;

	/* Apply limits to the latitude. Unlike some other conics, the
	   poles are arcs with a definitive radius.  The mathemagics
	   DO NOT blow up at the poles. Nor is longitude undefined
	   at the poles. I.e. a specific X value maps to a specific
	   longitude and vice versa. */

	if (fabs (lat) > cs_Pi_o_2)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = CS_adj1pi (lat);
	}

	/* Calculate rho, the radius of the arc on which this
	   point will lie. */

	if (alber->ecent == 0.0)
	{
		/* Here for the sphere. It's not at all obvious, but
		   tmp1 is never equal to or less than zero.  Verified
		   by a test program, 6/18/96. */

		tmp1 = alber->C - (cs_Two * alber->n * sin (lat));
		rho = alber->R_ovr_n * sqrt (tmp1);
	}
	else
	{
		/* Here for the ellisoid. */

		sin_lat = sin (lat);
		sin_sq = sin_lat * sin_lat;
		tmp1 = alber->ecent * sin_lat;
		tmp1 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp1 = log (tmp1) * alber->one_o_2e;
		tmp2 = sin_lat / (cs_One - (alber->e_sq * sin_sq));
		q = alber->one__esq * (tmp2 - tmp1);

		/* Again, it is not at all obvious, but tmp3 is
		   never zero or negative. */

		tmp3 = alber->C - (alber->n * q);
		rho = alber->R_ovr_n * sqrt (tmp3);
	}

	/* Now we can convert to the desired coordinate system. */

	xy [XX] = rho * sin (theta);
	xy [YY] = alber->rho0 - (rho * cos (theta));

	if (alber->quad == 0)
	{
		xy [XX] += alber->x_off;
		xy [YY] += alber->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],alber->x_off,alber->y_off,
							  alber->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**rtn_val = CSalberI (alber,ll,xy);
**
**	struct cs_Alber_ *alber;	structure which defines the parameters
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
**	1984, pages 95-98.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**
**********************************************************************/

int EXP_LVL9 CSalberI (Const struct cs_Alber_ *alber,double ll [2],Const double xy [2])
{
	extern double cs_Radian;		/* 57.29577..... */
	extern double cs_Pi_o_2;		/* pi over 2 */
	extern double cs_Mpi_o_2;		/* -pi over 2 */
	extern double cs_Zero;			/* 0.0 */
	extern double cs_Half;			/* 0.5           */
	extern double cs_Two;			/* 2.0 */

	int rtn_val;

	double x;
	double y;
	double q;
	double lat;
	double del_lng;
	double rho;
	double theta;
	double beta;
	double tmp1;

	rtn_val = cs_CNVRT_NRML;

	/* The longitude calculation is the same for the
	   sphere and the ellipsoid.  The value of rho, rho0,
	   and theta is the same for both calcuations. */

	if (alber->quad == 0)
	{
		x = xy [XX] - alber->x_off;
		y = xy [YY] - alber->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,alber->x_off,alber->y_off,alber->quad);
	}

	tmp1 = alber->rho0 - y;		/* Taking the pole, i.e. the
								   center of the concentric
								   circles of the projection,
								   as the origin, tmp1 is the
								   Y coordinate of the point
								   being converted. It is
								   negative if the focus is
								   the south pole. */
	rho = sqrt ((x * x) + (tmp1 * tmp1));
								/* Rho is now the distance
								   from the pole to the point.
								   I.e. the radius of the
								   concentric circle. */

	if (rho > alber->max_rho)	/* In range??? */
	{
		/* Too far away from the focus pole.  Force it
		   back into range. */

		rtn_val = cs_CNVRT_RNG;
		rho = alber->max_rho;

		/* You might think we need to adjust x and y to
		   account for the change in radius.  However, they
		   only get used as arguments to atan2. */
	}
	else if (rho < alber->min_rho)
	{
		/* Rho was less than the value normally associated
		   with the focus pole.  Not big problem unless
		   both X and Y(tmp1) are zeros (i.e. rho == 0).
		   In this case, we set Y(tmp1) to a non-zero
		   value (since x == 0 the specific value doesn't
		   matter), that has the right sign.  All this is
		   to keep atan2 below happy. */

		if (rho < alber->one_mm)
		{
			tmp1 = alber->one_mm * alber->n;
			x = cs_Zero;
		}

		rtn_val = cs_CNVRT_RNG;
		rho = alber->min_rho;
	}

	/* At this point, tmp1 should never be zero, thus the
	   atan2 function will always produce a valid value
	   regardless of the value of x. */

	if (alber->n >= 0.0)
	{
		/* North pole is the focus. */

		theta = atan2 (x,tmp1);
	}
	else
	{
		/* South pole is the focus. */

		rho = -rho;
		theta = atan2 (-x,-tmp1);
	}

	/* Is theta legitimate?  Cannot be in the pie slice on the
	   back side of the central meridian. */

	if (fabs (theta) > alber->theta_max)
	{
		rtn_val = cs_CNVRT_RNG;
		theta = (theta >= 0.0) ? alber->theta_max : -alber->theta_max;
	}

	/* Now we can compute the delta longitude. The cone constant n can
	   never be zero (zero would mean a cylinder). */

	del_lng = theta / alber->n;

	/* Now we can turn to the latitude.  The following
	   terms, rho and q are used in both calcuations.  Note,
	   n is always GREATER THAN zero and LESS THAN 1. */

	tmp1 = rho / alber->R_ovr_n;
	q = (alber->C - (tmp1 * tmp1)) / alber->n;

	if (alber->ecent == 0.0)
	{
		if (fabs (q) > cs_Two)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = (q >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
		}
		else
		{
			lat = asin (q * cs_Half);
		}
	}
	else
	{
		if (fabs (q) >= alber->beta_term)
		{
			if (fabs (q) > alber->beta_term) rtn_val = cs_CNVRT_RNG;
			lat = (q >= 0.0) ? cs_Pi_o_2 : cs_Mpi_o_2;
		}
		else
		{
			beta = asin (q / alber->beta_term);
			lat = CSbtIcalPrec (&alber->btcoefI,beta);
		}
	}
	ll [LNG] = (del_lng + alber->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}
/**********************************************************************
**	gamma = CSalberC (alber,ll);
**
**	struct cs_Alber_ *alber;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Albers Equal Area
**								Conic projection.
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

double EXP_LVL9 CSalberC (Const struct cs_Alber_ *alber,Const double ll [2])
{
	extern double cs_Radian;		/* 57.2957... */
	extern double cs_Km360;			/* -360.0, the value which
									   we return if provided
									   with a bogus point. */
	int status;

	double del_xx;
	double del_yy;
	double gamma;				/* some folks call this alpha */

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
	status = CSalberF (alber,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSalberF (alber,xy2,my_ll);
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
**	kk = CSalberK (alber,ll);
**
**	struct cs_Alber_ *alber;	structure containing all parameters
**								necessary for the transformation.
**	double ll [2];				location of the point to be computed
**								is given here; longitude ([0]) and
**								latitude ([1]) in degrees.
**	double kk;					the true map scale factor at the
**								specified point.
**
**********************************************************************/

double EXP_LVL9 CSalberK (Const struct cs_Alber_ *alber,Const double ll [2])
{
	extern double cs_Degree;		/* 1.0 / 57.29577.... */
	extern double cs_One;			/* 1.0                */
	extern double cs_Two;			/* 2.0                */
	extern double cs_NPTest;		/* 0.001 arc seconds short
									   of the north pole, in
									   radians. */
	extern double cs_SclInf;		/* 9.9E+04, the value
									   we return for an
									   infinite scale */

	double kk;
	double lat;

	double q;
	double m;
	double cos_lat;
	double sin_lat;
	double sin_sq;

	double term_a;
	double tmp1;
	double tmp2;

	lat = ll [1] * cs_Degree;

	if (fabs (lat) > cs_NPTest)
	{
		/* At either pole, the scale factor is infinite.
		   That is, the single pole point is represented
		   as an arc, therefore the scale is infinite. */

		kk = cs_SclInf;
	}
	else
	{
		cos_lat = cos (lat);
		sin_lat = sin (lat);
		if (alber->ecent == 0.0)
		{
			/* Here for a spherical datum. Note, we
			   have already dealt with all cases where
			   cos_lat may be zero. */

			tmp1 = alber->C - (cs_Two * alber->n * sin_lat);
			kk = sqrt (tmp1) / cos_lat;
		}
		else
		{
			/* Here for an ellipsoidal datum. */

			sin_sq = sin_lat * sin_lat;
			term_a = cs_One - (alber->e_sq * sin_sq);
			tmp1 = alber->ecent * sin_lat;
			tmp1 = (cs_One - tmp1) / (cs_One + tmp1);
			tmp1 = log (tmp1) * alber->one_o_2e;
			tmp2 = sin_lat / term_a;
			q = alber->one__esq * (tmp2 - tmp1);
			m = cos_lat / sqrt (term_a);
			tmp1 = alber->C - (alber->n * q);

			/* m is zero only if cos_lat is zero, and we have
			   already handled all of those cases. */

			kk = sqrt (tmp1) / m;
		}
	}

	return (kk);
}
/**********************************************************************
**hh = CSalberH (alber,ll);
**
**	struct cs_Alber_ *alber;	structure containing all parameters
**								necessary for the transformation.
**	double ll [2];				location of the point to be computed
**								is given here; longitude ([0]) and
**								latitude ([1]) in degrees.
**	double hh;					the grid scale factor along a meridian
**								at the specified point.
**
**	For equal area projections, such as this one, h = 1/k.
**********************************************************************/

double EXP_LVL9 CSalberH (Const struct cs_Alber_ *alber,Const double ll [2])
{
	extern double cs_One;			/* 1.0 */

	double hh;

	hh = cs_One / CSalberK (alber,ll);

	return (hh);
}
/**********************************************************************
**	status = CSalberL (alber,cnt,pnts);
**
**	struct cs_Alber_ *alber;	the coordinate system against which the check is
**								to be performed.
**	int cnt;					the number of points in the region to be checked.
**	double pnts [][3];			the list of coordinates to be checked.
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

int EXP_LVL9 CSalberL (Const struct cs_Alber_ *alber,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;	/* 1.0 / 57.2... */
	extern double cs_Pi_o_2;	/*  PI / 2 */
	extern double cs_Mpi_o_2;	/* -PI / 2 */
	extern double cs_Pi;		/*  PI */

	int ii;
	int status;
	int sgn_lng1, sgn_lng2;

	double tmp;
	double del_lng1, del_lng2;

	/* Check all the points. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		tmp = pnts [ii][LAT] * cs_Degree;
		if (tmp < cs_Mpi_o_2 || tmp > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	if (cnt <= 1 || status != cs_CNVRT_OK)
	{
		return (status);
	}

	/* It makes little difference if we have one line, or a hundred,
	   the test is the same.  Results might not be so hot if the
	   line or region cross the back side of the central meridian.

	   NOTE: the individual points will convert fine, but graphic
	   elements which cross this boundary probably won't convert
	   as expected.

	   Set up the test for the first segment. */

	tmp = pnts [0][LNG] * cs_Degree;
	del_lng1 = CS_adj2pi (tmp - alber->org_lng);
	sgn_lng1 = (del_lng1 < 0.0);
	for (ii = 1;ii < cnt;ii++)
	{
		/* Does the line cross the back azimuth of the
		   central meridian??? */

		tmp = pnts [ii][LNG] * cs_Degree;
		del_lng2 = CS_adj2pi (tmp - alber->org_lng);
		sgn_lng2 = (del_lng2 < 0.0);
		if (sgn_lng1 != sgn_lng2)
		{
			/* Crosses the central meridian or the back
			   azimuth.  Which one? */

			tmp = fabs (del_lng1) + fabs (del_lng2);
			if (tmp > cs_Pi)
			{
				/* Opps!!! We have a problem. */

				status = cs_CNVRT_DOMN;
				break;
			}
		}

		/* Set up for the next segment. */

		del_lng1 = del_lng2;
		sgn_lng1 = sgn_lng2;
	}
	return (status);
}
/**********************************************************************
**	status = CSalber (alber,cnt,pnts);
**
**	struct cs_Alber_ *alber;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSalberL to check lat/long
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
/*
	This function converts the cartesian coordinate to
	polar form.
*/
static int CSalberXP (Const struct cs_Alber_ *alber,Const double xy [2],double *theta)
{
	double x, y;
	double rho;
	double tmp;

	/* Convert to polar coordinates. */

	if (alber->quad == 0)
	{
		x = xy [XX] - alber->x_off;
		y = xy [YY] - alber->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,alber->x_off,alber->y_off,alber->quad);
	}
	tmp = alber->rho0 - y;
	rho = sqrt (x * x + (tmp * tmp));

	/* If rho is greater than the somewhat arbitrary value which
	   CSalberS has calculated as the pole opposite to the focus,
	   we consider it bogus. */

	if (rho < alber->min_rho || rho > alber->max_rho)
	{
		return (cs_CNVRT_DOMN);
	}

	/* If we met the test above, at least one of the two
	   variables used below must be non-zero. Therefore, the
	   atan2 function will be able to return a valid result
	   without dying. */

	if (alber->n > 0.0) *theta = atan2 (x,tmp);
	else		    *theta = atan2 (-x,-tmp);

	/* If the theta value is in the cutout region, we have a
	   problem. */

	return ((fabs (*theta) > alber->theta_max) ? cs_CNVRT_DOMN : cs_CNVRT_OK);
}
int EXP_LVL9 CSalberX (Const struct cs_Alber_ *alber,int cnt,Const double pnts [][3])
{
	extern double cs_Pi;
	extern double cs_Zero;

	int ii;
	int status;
	int sgn_theta1 = 0;
	int sgn_theta2;

	double tmp;
	double theta1 = cs_Zero;
	double theta2;

	status = cs_CNVRT_OK;			/* until we know different */
	for (ii = 0;ii < cnt;ii++)
	{
		/* Check the current point. */

		status = CSalberXP (alber,pnts [0],&theta2);
		if (status != cs_CNVRT_OK) break;
		if (ii == 0)
		{
			/* This is the first point, set up subsequent
			   segment checks. */

			theta1 = theta2;
			sgn_theta1 = (theta1 < 0.0);
		}
		else
		{
			/* We have a problem if any segment crosses the
			   back azimuth of the central meridian. */

			sgn_theta2 = (theta2 < 0.0);
			if (sgn_theta1 != sgn_theta2)			/*lint !e644 */
			{
				/* Segment crosses the central meridian
				   or the back azimuth thereof. Which is
				   it? */

				tmp = fabs (theta1) + fabs (theta2);		/*lint !e644 */
				if (tmp > cs_Pi)
				{
					/* Opps!!! Crosses the back
					   azimuth. */

					status = cs_CNVRT_DOMN;
					break;
				}
			}

			/* Set up for the next segment. */

			theta1 = theta2;
			sgn_theta1 = sgn_theta2;
		}
	}
	return (status);
}

