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
**	err_cnt = CSlmbrtQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	int err_list [];			an array of integers in which error codes are
**								returned if not NULL.
**	unsigned short prj_code;	projection code, used to identify specific
**								variations of the projection.
**	int list_sz;				the size of the array pointed to be err_list,
**								may be zero.
**	int err_cnt;				returns the number of errors detected.
**
**	Set err_list to NULL, and/or list_sz to zero, to simply get
**	a yea or nay status of the definition (i.e. err_cnt != 0).
**
**	All lat/longs in definitions must be referenced to Greenwich,
**	and in the range of greater than -180/-90, and less than or
**	equal to +180/+90.
**
**	The Two Standard Parallel case has two standard parallels,
**	but no scale reduction factor. Niethe standard parallel can
**	be a pole. The two can be the same, but not if they are the
**	equator. The two can straddle the equator, but may not be
**	equidistant from it.
**********************************************************************/

int EXP_LVL9 CSlmbrtQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;			/* -180.0 */
	extern double cs_MinLat;			/* - 90.0 */
	extern double cs_MaxLng;			/* +180.0 */
	extern double cs_MaxLat;			/* + 90.0 */
	extern double cs_MinLatFz;			/*  -89.999 */
	extern double cs_MaxLatFz;			/* + 89.999 */
	extern double cs_SclRedMin;			/* 0.75 */
	extern double cs_SclRedMax;			/* 1.10 */
	extern double cs_ParmTest;			/* 2.78E-05 */

	int err_cnt;
	int sign1, sign2;
	
	double tmp;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Lambert Conformal Conic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}
	if (prj_code != cs_PRJCOD_LM1SP)
	{
		/* All 4 variations other than the single standard parallel are
		   checked here. */

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
		if (prj_code == cs_PRJCOD_LMMICH)
		{
			/* Need to check one extra parameter here. */
			if (cs_def->prj_prm3 < 0.9990 || cs_def->prj_prm3 > 1.0001)
			{
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_STDPLL;
			}
		}
	}
	else
	{
		/* The single parallel case is checked here. */

		if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ELPSCL;
		}
	}

	/* That's it for Lambert Conformal Conic. */

	return (err_cnt + 1);
}

/**********************************************************************
**	CSlmbrtS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	We support three variations of this projection. All
**	differences are limited to this setup function. The
**	three variations are:
**
**	Two Standard Parallels: This is what we have traditionally
**		supported here. Thus, this variation inherits the
**		original name and code value.
**	Single Standard Parallel: This is numerically equivalent
**		to the Lambert Tangential. However, we add it
**		as a variation here as this is what many
**		Europeans expect.
**	Belgian Variation: A minor variation to the Two Standard
**		parallel case. This is implemented by the
**		Belgian variable in the structure. It is set to
**		zero for the normal projection, and 29.2985
**		seconds of arc for this variation.
**
**	Note, we have a big problem if either standard parallel
**	is at either pole, or the origin latiude is at either
**	pole.  These are definition problems, we don't attempt
**	to deal with them here.  Use CS_cschk and/or CSlmbrtQ
**	before calling this function.
**********************************************************************/

#if defined (_MSC_VER) && (_MSC_VER >= 1100)
#pragma optimize ("g",off)
#endif

void EXP_LVL9 CSlmbrtS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577.....     */
	extern double cs_Pi;				/* 3.14159           */
	extern double cs_Pi_o_2;			/* pi over 2         */
	extern double cs_Mpi_o_2;			/* -pi over 2        */
	extern double cs_Pi_o_4;			/* pi over 4         */
	extern double cs_Zero;				/* 0.0               */
	extern double cs_Half;				/* 0.5               */
	extern double cs_One;				/* 1.0               */
	extern double cs_Ten;				/* 5.0               */
	extern double cs_Sec2Deg;			/* 1.0 / 3600.0      */
	extern double cs_AnglTest;			/* 0.001 seconds of arc
										   in radians. */
	extern double cs_NPTest;			/* 90 less .001 seconds
										   of arc in radians */
	extern double cs_SPTest;			/* -90 plus .001 seconds
										   of arc in radians */
	extern double cs_MinLatFz;			/* 0.1 arc seconds short
										   of the south pole, in
										   degrees. */
	extern double cs_MaxLatFz;			/* 0.1 arc seconds short
										   of the morth pole, in
										   degrees. */

	struct cs_Lmbrt_ *lmbrt;

	double t1, t2;
	double m1, m2;
	double ref_lat1, ref_lat2;
	double inf_lat;

	double flat;
	double p_rad;
	double height;
	double rhoMin, rhoMax;

	double tmp1;
	double tmp2;
	double tmp3;
	double tmp4;
	double tmp5;
	double tmp6;

	lmbrt = &csprm->proj_prms.lmbrt;

	/* The following is the same for all variations. */
	lmbrt->belgium = cs_Zero;		/* until we know different */
	lmbrt->ellipsoidK = cs_One;		/* until we know different. */
	lmbrt->org_lng = csprm->csdef.org_lng * cs_Degree;
	lmbrt->org_lat = csprm->csdef.org_lat * cs_Degree;
	lmbrt->x_off = csprm->csdef.x_off;
	lmbrt->y_off = csprm->csdef.y_off;
	lmbrt->k0 = cs_One;
	lmbrt->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Initialize the affine post processor stuff to a benign state. */
	lmbrt->affineA0    = cs_Zero;
	lmbrt->affineA1    = cs_One;
	lmbrt->affineA2    = cs_Zero;;
	lmbrt->affineB0    = cs_Zero;
	lmbrt->affineB1    = cs_One;
	lmbrt->affineB2    = cs_Zero;
	lmbrt->affineK     = cs_One;
	lmbrt->affineGamma = cs_Zero;
	lmbrt->affineDenom = cs_One;

	/* See which variation we have. Note that the two parallel form
	   can be defaulted to a single parallel form by setting the
	   two standard parallels to the same value. We cannot just
	   arbitraily switch to the single parallel form, as there
	   is the issue of the origin latitude. */
	switch (csprm->prj_code) {

	case cs_PRJCOD_LM1SP:
		/* Single parallel, we have a scale reduction factor.
		   Setting the reference parallels to the origin
		   latitude means we have common code for the
		   bulk of this module. */
		ref_lat1 = ref_lat2 = lmbrt->org_lat;
		lmbrt->k0 = csprm->csdef.scl_red;
		lmbrt->k = csprm->csdef.scale * csprm->csdef.scl_red;
		lmbrt->e_rad = csprm->datum.e_rad;
		lmbrt->ecent = csprm->datum.ecent;
		break;

	case cs_PRJCOD_LMBLG:
		/* Belgian version requires an extra constant. */
		lmbrt->belgium = 29.2985 * cs_Sec2Deg * cs_Degree;
		/* No break, by design. */

	default:
	case cs_PRJCOD_LM2SP:
		/* Two reference parallels, no scale reduction. */
		ref_lat1 = csprm->csdef.prj_prm1 * cs_Degree;
		ref_lat2 = csprm->csdef.prj_prm2 * cs_Degree;
		lmbrt->k = csprm->csdef.scale;
		lmbrt->e_rad = csprm->datum.e_rad;
		lmbrt->ecent = csprm->datum.ecent;
		break;

	case cs_PRJCOD_WCCSL:
		/* Same as a two parallel system, but we twiddle the
		   size and shape of the ellipsoid. */
		ref_lat1 = csprm->csdef.prj_prm1 * cs_Degree;
		ref_lat2 = csprm->csdef.prj_prm2 * cs_Degree;
		lmbrt->k = csprm->csdef.scale;
		height = csprm->csdef.prj_prm4 * csprm->csdef.unit_scl;
		height += csprm->csdef.prj_prm3;
		lmbrt->e_rad = csprm->datum.e_rad + height;
		p_rad = csprm->datum.p_rad + height;
		flat = cs_One - p_rad / lmbrt->e_rad;
		lmbrt->ecent = sqrt ((flat + flat) - (flat * flat));
		break;

	case cs_PRJCOD_MNDOTL:
		/* Same as a two parallel system, but we twiddle the
		   size of the ellipsoid (shape remains the same). */
		ref_lat1 = csprm->csdef.prj_prm1 * cs_Degree;
		ref_lat2 = csprm->csdef.prj_prm2 * cs_Degree;
		lmbrt->k = csprm->csdef.scale;
		height = csprm->csdef.prj_prm3 * csprm->csdef.unit_scl;
		lmbrt->e_rad = csprm->datum.e_rad + height;
		lmbrt->ecent = csprm->datum.ecent;
		break;

	case cs_PRJCOD_LMBRTAF:
		/* Same as a two parallel system, but we capture the
		   affine parameters as well. */
		ref_lat1 = csprm->csdef.prj_prm1 * cs_Degree;
		ref_lat2 = csprm->csdef.prj_prm2 * cs_Degree;
		lmbrt->k = csprm->csdef.scale;
		lmbrt->e_rad = csprm->datum.e_rad;
		lmbrt->ecent = csprm->datum.ecent;
		lmbrt->affineA0    = csprm->csdef.prj_prm3;
		lmbrt->affineB0    = csprm->csdef.prj_prm4;
		lmbrt->affineA1    = csprm->csdef.prj_prm5;
		lmbrt->affineA2    = csprm->csdef.prj_prm6;
		lmbrt->affineB1    = csprm->csdef.prj_prm7;
		lmbrt->affineB2    = csprm->csdef.prj_prm8;
		break;

	case cs_PRJCOD_LMMICH:
		/* Two reference parallels, no scale reduction. */
		ref_lat1 = csprm->csdef.prj_prm1 * cs_Degree;
		ref_lat2 = csprm->csdef.prj_prm2 * cs_Degree;
		lmbrt->k = csprm->csdef.scale;
		lmbrt->e_rad = csprm->datum.e_rad;
		lmbrt->ecent = csprm->datum.ecent;
		lmbrt->ellipsoidK = csprm->csdef.prj_prm3;
		break;
	}

	/* A few special calculations if there is an Affine post-process. */
	if (csprm->prj_code == cs_PRJCOD_LMBRTAF)
	{
		lmbrt->affineK = sqrt (lmbrt->affineA1 * lmbrt->affineA1 + lmbrt->affineB1 * lmbrt->affineB1);
		if (lmbrt->affineB2 != 0.0 || lmbrt->affineA2 != 0.0)
		{
			lmbrt->affineGamma = -atan2 (lmbrt->affineA2,lmbrt->affineB2);
		}
		else
		{
			lmbrt->affineGamma = cs_Zero;
		}
		lmbrt->affineDenom = lmbrt->affineA1 * lmbrt->affineB2 - lmbrt->affineA2 * lmbrt->affineB1;
		lmbrt->quad |= csLMBRT_AFFINE;
	}

	/* Now that that's all done, the remainder of this is all the
	   same regardless of which variation is in use. */
	lmbrt->e_sq = lmbrt->ecent * lmbrt->ecent;
	lmbrt->e_ovr_2 = lmbrt->ecent * cs_Half;

	/* If the ecentricity is zero, we have a sphere.  The
	   set up is a bit different in this case. */
	if (lmbrt->ecent == 0.0)
	{
		tmp1 = tan (cs_Pi_o_4 + (ref_lat1 * cs_Half));
		tmp3 = cos (ref_lat1);
		if (fabs (ref_lat1 - ref_lat2) <= cs_AnglTest)
		{
			/* A single standard parallel. */
			lmbrt->n = sin (ref_lat1);
		}
		else
		{
			/* Two standard parallels. */
			tmp2 = tan (cs_Pi_o_4 + (ref_lat2 * cs_Half));
			tmp4 = cos (ref_lat2);
			lmbrt->n = log (tmp3 / tmp4) / log (tmp2 / tmp1);
		}
		lmbrt->k_tlat = lmbrt->k * tmp3 * pow (tmp1,lmbrt->n);
		lmbrt->F = tmp3 * pow (tmp1,lmbrt->n) / lmbrt->n;
		lmbrt->aF = lmbrt->F * lmbrt->e_rad * lmbrt->k;
		tmp1 = tan (cs_Pi_o_4 + (lmbrt->org_lat * cs_Half));
		lmbrt->rho0 = lmbrt->aF / pow (tmp1,lmbrt->n);

		inf_lat = (lmbrt->n > 0.0) ? cs_SPTest : cs_NPTest;
		tmp1 = tan (cs_Pi_o_4 + (inf_lat * cs_Half));
		lmbrt->infinity = fabs (lmbrt->aF) / pow (tmp1,lmbrt->n);
	}
	else
	{
		/* Here for the ellipsoidal calculations. */
		tmp1 = tan (cs_Pi_o_4 - (lmbrt->org_lat * cs_Half));
		tmp2 = lmbrt->ecent * sin (lmbrt->org_lat);
		lmbrt->t0 = tmp1 / pow ((cs_One - tmp2)/(cs_One + tmp2),lmbrt->e_ovr_2);

		tmp1 = tan (cs_Pi_o_4 - (ref_lat1 * cs_Half));
		tmp3 = sin (ref_lat1);
		tmp2 = lmbrt->ecent * tmp3;
		t1 = tmp1 / pow ((cs_One - tmp2)/(cs_One + tmp2),lmbrt->e_ovr_2);

		tmp4 = lmbrt->e_sq * tmp3 * tmp3;
		m1 = cos (ref_lat1) / sqrt (cs_One - tmp4);

		if (fabs (ref_lat1 - ref_lat2) <= cs_AnglTest)
		{
			/* There is only one standard parallel. */
			lmbrt->n = sin (ref_lat1);
		}
		else
		{
			/* There are two standard parallels. */
			tmp1 = tan (cs_Pi_o_4 - (ref_lat2 * cs_Half));
			tmp3 = sin (ref_lat2);
			tmp2 = lmbrt->ecent * tmp3;
			t2 = tmp1 / pow ((cs_One - tmp2)/(cs_One + tmp2),lmbrt->e_ovr_2);

			tmp4 = lmbrt->e_sq * tmp3 * tmp3;
			m2 = cos (ref_lat2) / sqrt (cs_One - tmp4);

			tmp5 = log (m1) - log (m2);
			tmp6 = log (t1) - log (t2);
			lmbrt->n = tmp5 / tmp6;
		}

		/* The rest of this is all the same, regardless of the variation. */
		lmbrt->F = m1 / (lmbrt->n * pow (t1,lmbrt->n));
		lmbrt->aF = lmbrt->e_rad * lmbrt->k * lmbrt->F;
		if (csprm->prj_code == cs_PRJCOD_LMMICH)
		{
			/* The Michigan variation uses an elevated ellipsoid.  I think
			   we can deal with this here in the setup, leaving the actual
			   forward and inverse calculations unchanged. */
			lmbrt->aF *= lmbrt->ellipsoidK;
		}
		lmbrt->rho0 = lmbrt->aF * pow (lmbrt->t0,lmbrt->n);

		/* Now we compute the coefficients for the conformal latitude
		   inverse power series.  This series is used in the inverse
		   calculation and is faster than the iterative loop. */
		CSchiIsu (&lmbrt->chicofI,lmbrt->e_sq);

		/* Compute the value of rho which we will use for infinity for
		   this system.  This is the value which corresponds to the very
		   small angle which we use to see if a latitude is the pole. */
		inf_lat = (lmbrt->n > 0.0) ? cs_SPTest : cs_NPTest;
		tmp1 = tan (cs_Pi_o_4 - (inf_lat * cs_Half));
		tmp3 = sin (cs_NPTest);
		tmp2 = lmbrt->ecent * tmp3;
		tmp4 = tmp1 / pow ((cs_One - tmp2)/(cs_One + tmp2),lmbrt->e_ovr_2);
		lmbrt->infinity = fabs (lmbrt->aF) * pow (tmp4,lmbrt->n);
	}

	/* Finish it off. */
	if (lmbrt->n > 0.0) lmbrt->pi_o2n = cs_Pi_o_2;
	else				lmbrt->pi_o2n = cs_Mpi_o_2;
	lmbrt->one_ovr_n = cs_One / lmbrt->n;
	lmbrt->theta_max = cs_Pi * fabs (lmbrt->n);
	lmbrt->pole_test = lmbrt->aF * cs_AnglTest;

	/* Set up the useful range limit values.  If the definition
	   doesn't carry any, we will compute some. */
	csprm->cent_mer = lmbrt->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* Calculate a rather conservative useful range.  Longitudes
		   are easy. */
		csprm->min_ll [LNG] = -cs_Ten;
		csprm->max_ll [LNG] =  cs_Ten;

		/* Start with the min/max latitudes equal to the
		   standard parallels. Careful here, we don't require
		   standard parallel 1 to be north of parallel 2, and
		   they may be set to the same value.  The two parallels
		   may be on opposite sides of the equator. */
		tmp1 = ref_lat1 - ref_lat2;
		if (fabs (tmp1) < cs_AnglTest) tmp1 = cs_Zero;
		if (tmp1 >= 0.0)
		{
			csprm->min_ll [LAT] = ref_lat2 * cs_Radian;
			csprm->max_ll [LAT] = ref_lat1 * cs_Radian;
		}
		else
		{
			csprm->min_ll [LAT] = ref_lat1 * cs_Radian;
			csprm->max_ll [LAT] = ref_lat2 * cs_Radian;
		}

		/* Widen the coverage as is appropriate.  We assume that
		   the standard parallels were set to be 1/6th of the
		   extent in from the northern and southern edges of the
		   extent of the map. */
		tmp1 = fabs (tmp1) * cs_Radian;
		tmp1 = (tmp1 == 0.0) ? cs_Half : tmp1 * cs_Half;
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
		if (lmbrt->n >= 0.0)
		{
			/* Focus pole is north pole. */
			tmp1 = tan (cs_Pi_o_4 + csprm->min_ll [LAT] * cs_Half * cs_Degree);
			rhoMax = lmbrt->aF / pow (tmp1,lmbrt->n);
			tmp1 = tan (cs_Pi_o_4 + csprm->max_ll [LAT] * cs_Half * cs_Degree);
			rhoMin = lmbrt->aF / pow (tmp1,lmbrt->n);

			tmp1 = csprm->min_ll [LNG] * cs_Degree;
			csprm->min_xy [XX] = rhoMax * lmbrt->n * sin (tmp1) + lmbrt->x_off;
			tmp1 = csprm->max_ll [LNG] * cs_Degree;
			csprm->max_xy [XX] = rhoMax * lmbrt->n * sin (tmp1) + lmbrt->x_off;

			csprm->min_xy [YY] = lmbrt->rho0 - rhoMax + lmbrt->y_off;
			csprm->max_xy [YY] = lmbrt->rho0 - rhoMin + lmbrt->y_off;
		}
		else
		{
			/* Focus pole is south pole, curvature of the parallel
			   arcs is "up side down". */
			tmp1 = tan (cs_Pi_o_4 + csprm->max_ll [LAT] * cs_Half * cs_Degree);
			rhoMax = lmbrt->aF / pow (tmp1,lmbrt->n);
			tmp1 = tan (cs_Pi_o_4 + csprm->min_ll [LAT] * cs_Half * cs_Degree);
			rhoMin = lmbrt->aF / pow (tmp1,lmbrt->n);

			tmp1 = csprm->min_ll [LNG] * cs_Degree;
			csprm->min_xy [XX] = rhoMax * lmbrt->n * sin (tmp1) + lmbrt->x_off;
			tmp1 = csprm->max_ll [LNG] * cs_Degree;
			csprm->max_xy [XX] = rhoMax * lmbrt->n * sin (tmp1) + lmbrt->x_off;

			csprm->min_xy [YY] = lmbrt->rho0 - rhoMin + lmbrt->y_off;
			csprm->max_xy [YY] = lmbrt->rho0 - rhoMax + lmbrt->y_off;
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
	   addresses and we are done.  Since this projection
	   is conformal, the k and h scale factors are the
	   same. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSlmbrtF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSlmbrtI;
	csprm->cs_scale = (cs_SCALE_CAST)CSlmbrtK;
	csprm->cs_sclk  = (cs_SCALE_CAST)CSlmbrtK;
	csprm->cs_sclh  = (cs_SCALE_CAST)CSlmbrtK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSlmbrtC;
	csprm->llchk    = (cs_LLCHK_CAST)CSlmbrtL;
	csprm->xychk = (cs_XYCHK_CAST)CSlmbrtX;

	return;
}

/**********************************************************************
**	rnt_val = CSlmbrtF (lmbrt,xy,ll);
**
**	struct cs_Lmbrt_ *lmbrt;	structure which defines the parameters
**								in effect for the transformation.
**	double xy [2];				the results are returned here, x ([0])
**								and y ([1]).
**	double ll [2];				the longitude ([0]) and latitude ([1]),
**								in degrees, to be converted.
**	int rtn_val;				returns cs_CNVRT_NRML as for all values
**								execpt the pole opposite the focus.  In
**								this case, cs_CNVRT_RNG is returned.
**
**	The ll and xy arguments of this function may point
**	to the same array with no adverse affects.
**
**	This function calculates the Lambert Conformal Conic
**	projection and returns values in the coordinate system
**	described in the cs_Lmbrt_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 101-109.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSlmbrtF (Const struct cs_Lmbrt_ *lmbrt,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Mpi;				/* -3.14159... */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_Pi_o_2;			/* pi over 2 */
	extern double cs_Pi_o_4;			/* pi over 4 */
	extern double cs_Zero;				/* 0.0               */
	extern double cs_One;				/* 1.0               */
	extern double cs_Half;				/* 0.5 */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the north pole
										   in radians */
	extern double cs_SPTest;			/* .001 seconds of arc
										   short of the south pole
										   in radians */

	int rtn_val;

	double lng;				/* The given longitude, after conversion
							   to radians. */
	double lat;				/* The given latitude after conversion
							   to radians. */
	double rho;
	double theta;
	double del_lng;
	double t;
	double tmp1;		/* (tan (PI/4 + lat/2)) */
	double tmp2;		/* sin (lat) */
	double tmp3;		/* ecentricity * sin (lat) */
	
	double affineXY [2];

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and
	   one for the ellipsoid.  If the ecentricity
	   of the dataum in use is 0.0 exactly, we
	   shall use the spherical formulae.  There
	   is a miminal amount of stuff which is
	   common to both which we perform first.

	   Convert the latitude and longitude to radians. */

	lng = cs_Degree * ll [LNG];
	lat = cs_Degree * ll [LAT];

	/* Some calculations common to both the sphere and the
	   ellipsoid.  lmbrt->n is the cone constant, and will be
	   greater than zero and less than one as long as we have
	   a cone and not a cylinder or a flat plane.  (It is
	   negative if the focus pole is the south pole.) */

	del_lng = lng - lmbrt->org_lng;
	if (fabs (del_lng) > cs_Pi)
	{
		/* Rarely get here, so we can spend some time analyzing the
		   situation. What we're attempting to do here is isolate
		   the case where the central meridian is at -179 and the
		   provided longitude is +179. */

		if      (del_lng > cs_Pi  && lmbrt->org_lng < 0.0) del_lng -= cs_Two_pi;
		else if (del_lng < cs_Mpi && lmbrt->org_lng > 0.0) del_lng += cs_Two_pi;
		if (fabs (del_lng) > cs_Pi) rtn_val = cs_CNVRT_RNG;

		del_lng = CS_adj2piI (del_lng);
	}
	theta = del_lng * lmbrt->n;

	/* A latitude equal to either pole causes problems.
	   Rho, the radius of the arc used to depict the parallel
	   is zero for the focus pole, and infinity for the
	   opposite pole.  In either case, the mathemagics below
	   have problems.  The result is the same, however, for
	   both the sphere and the ellipsoid in these cases. */

	if (fabs (lat) >= cs_NPTest)
	{
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}
	if (lat >= cs_NPTest)
	{
		if (lmbrt->n > 0.0)
		{
			 rho = cs_Zero;
			 if (rtn_val == cs_CNVRT_NRML) rtn_val = cs_CNVRT_INDF;
		}
		else
		{
			rho = lmbrt->infinity;
			rtn_val = cs_CNVRT_RNG;
		}
	}
	else if (lat <= cs_SPTest)
	{
		if (lmbrt->n < 0.0)
		{
			rho = cs_Zero;
			if (rtn_val == cs_CNVRT_NRML) rtn_val = cs_CNVRT_INDF;
		}
		else
		{
			rho = lmbrt->infinity;
			rtn_val = cs_CNVRT_RNG;
		}
	}
	else if (lmbrt->ecent == 0.0)
	{
		/* Here for the sphere.  Since we have filtered the
		   latitude, tmp1 will never be zero or negative. */

		tmp1 = tan (cs_Pi_o_4 + (lat * cs_Half));

		/* tmp1 can be zero only at the north pole.  We've
		   dealt with that already.  For a southern focus,
		   aF is negative, yielding a negative rho. */

		rho = lmbrt->aF / pow (tmp1,lmbrt->n);
	}
	else
	{
		/* Here for the ellisoid. tmp1 and tmp3 are
		   non-zero as long as we're not at a pole, which
		   we have already dealt with. */

		tmp1 = tan (cs_Pi_o_4 - (lat * cs_Half));
		tmp2 = sin (lat) * lmbrt->ecent;
		tmp3 = (cs_One - tmp2) / (cs_One + tmp2);
		t = tmp1 / pow (tmp3,lmbrt->e_ovr_2);
		rho = lmbrt->aF * pow (t,lmbrt->n);
	}

	/* Now we can convert to the desired coordinate system. */

	xy [XX] = rho * sin (theta - lmbrt->belgium);
	xy [YY] = lmbrt->rho0 - (rho * cos (theta - lmbrt->belgium));

	if (lmbrt->quad == 0)
	{
		xy [XX] += lmbrt->x_off;
		xy [YY] += lmbrt->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],lmbrt->x_off,lmbrt->y_off,lmbrt->quad);
	}

	if ((lmbrt->quad & csLMBRT_AFFINE) != 0)
	{
		affineXY [XX] = xy [XX];
		affineXY [YY] = xy [YY];
		xy [XX] = lmbrt->affineA0 + lmbrt->affineA1 * affineXY [XX] +
									lmbrt->affineA2 * affineXY [YY];
		xy [YY] = lmbrt->affineB0 + lmbrt->affineB1 * affineXY [XX] +
									lmbrt->affineB2 * affineXY [YY];
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSlmbrtI (lmbrt,ll,xy);
**
**	struct cs_Lmbrt_ *lmbrt;	structure which defines the parameters
**								in effect for the transformation.
**	double ll [2];				the longitude ([0]) and latitude ([1]),
**								in degrees, are returnrd here.
**	double xy [2];				the coordinates to be converted, x ([0])
**								and y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if the input values
**								are within mathematically acceptable limits,
**								else returns cs_CNVRT_RNG.
**
**	The ll and xy arguments of this function may point
**	to the same array with no adverse affects.
**
**	All the formulae used here were extracted from the
**	USGS publication "Map Projections Used by the
**	U. S. Geological Survey", Second Edition, reprinted
**	1984, pages 101-109.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSlmbrtI (Const struct cs_Lmbrt_ *lmbrt,double ll [2],Const double xy [2])
{
	extern double cs_Pi_o_2;			/* pi over two */
	extern double cs_Radian;			/* 57.29577    */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Two;				/* 2.0         */

	int rtn_val;

	double x;
	double y;
	double t;
	double lat;
	double del_lng;
	double chi;
	double rho;
	double theta;

	double tmp1;
	double myXY [2];

	rtn_val = cs_CNVRT_NRML;

	/* The longitude calculation is the same for the
	   sphere and the ellipsoid.  The value of rho
	   and theta is the same for both calcuations.

	   Rho0 is the radius of the arc which represents the
	   origin latitude.  Rho is the radius of the arc which
	   represents the latitude of the point we are working
	   with.  Be careful: at the focus pole rho will be zero.

	   rho is the radius of the circular arc upon which this
	   point resides.  If greater than lmbrt->infinity, the
	   value is unrealistic. */

	if (lmbrt->quad == 0)
	{
		x = xy [XX] - lmbrt->x_off;
		y = xy [YY] - lmbrt->y_off;
	}
	else
	{
		if ((lmbrt->quad & csLMBRT_AFFINE) != 0)
		{
			myXY [XX]  = lmbrt->affineB2 * (xy [XX] - lmbrt->affineA0) - lmbrt->affineA2 * (xy [YY] - lmbrt->affineB0);
			myXY [XX] /= lmbrt->affineDenom;

			myXY [YY]  = lmbrt->affineA1 * (xy [YY] - lmbrt->affineB0) - lmbrt->affineB1 * (xy [XX] - lmbrt->affineA0);
			myXY [YY] /= lmbrt->affineDenom;
		}
		else
		{
			myXY [XX] = xy [XX];
			myXY [YY] = xy [YY];
		}
		CS_quadI (&x,&y,myXY,lmbrt->x_off,lmbrt->y_off,lmbrt->quad);
	}

	tmp1 = lmbrt->rho0 - y;
	rho = sqrt (x * x + (tmp1 * tmp1));

	if (rho > lmbrt->infinity) rtn_val = cs_CNVRT_RNG;

	if (lmbrt->n < 0.0) rho = -rho;

	/* Theta is essentially the convergence angle at the point.
	   If the point is a pole, it is undefined, and we will
	   choose zero.  This also produces a longitude of org_lng
	   at the pole. */

	if (fabs (rho) > lmbrt->pole_test)
	{
		/* XY coordinate not at the focus pole. */

		if (lmbrt->n > 0.0)
		{
			/* Focus at the north pole. */

			theta = atan2 (x,tmp1);
		}
		else
		{
			/* Focus at the south pole. */

			theta = atan2 (-x,-tmp1);
		}
	}
	else
	{
		/* XY is at the focus pole. */

		theta = cs_Zero;
		rtn_val = cs_CNVRT_INDF;
	}

	/* Make sure that theta is within range. */

	if (fabs (theta) > lmbrt->theta_max)
	{
		rtn_val = cs_CNVRT_RNG;
		theta = (theta > 0.0) ? lmbrt->theta_max : -lmbrt->theta_max;
	}

	/* The cone constant (n) can't be zero if we really have a
	   cone (as opposed to a cylinder or a flat plane).  We rely
	   on CS_cschk and CSlmbrtQ to filter these problems out.
	   Note for the focus pole, theta is zero for all values
	   if longitude and we get the origin longitude as the
	   result. */

	del_lng = (theta + lmbrt->belgium) / lmbrt->n;

	/* Now we can turn to the latitude.  The sphere
	   and the ellipsoid require different calculations. */
	   
	if (lmbrt->ecent == 0.0)
	{
		/* Here for the sphere. */

		if (fabs (rho) > lmbrt->pole_test)
		{
			/* Not at the focus pole, normal calculation. */

			tmp1 = pow (lmbrt->aF / rho,lmbrt->one_ovr_n);
		}
		else
		{
			/* At the focus pole, set tmp1 to latitude for
			   the pole of the system. */

			tmp1 = lmbrt->pi_o2n;
		}
		lat = (cs_Two * atan (tmp1) - cs_Pi_o_2);
	}
	else
	{
		/* Here for the ellisoid. */

		if (fabs (rho) > lmbrt->pole_test)
		{
			/* Not at the focus pole, normal calculation. */

			t = pow (rho / lmbrt->aF,lmbrt->one_ovr_n);
			chi = cs_Pi_o_2 - cs_Two * atan (t);
		}
		else
		{
			/* At the focus pole.  Set t to zero to get
			   the latitude of the focus pole. */

			chi = lmbrt->pi_o2n;
		}
		lat = CSchiIcal (&lmbrt->chicofI,chi);
	}

	ll [LNG] = (del_lng + lmbrt->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	theta = CSlmbrtK (lmbrt,ll);
**
**	struct cs_Lmbrt_ *lmbrt;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the convergence of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double theta;				returns the convergence angle of the
**								projected coordinate system at that
**								point, in degrees, east of north.
**
**	I wish they were all this simple, but I'd be out of a job.
**********************************************************************/

double EXP_LVL9 CSlmbrtC (Const struct cs_Lmbrt_ *lmbrt,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577... */

	double lng;
	double theta;

	lng = ll [LNG] * cs_Degree;
	theta = lmbrt->n * CS_adj2pi (lng - lmbrt->org_lng) * cs_Radian;

	/* If we have an affine post process, we need to factor that in.
	   I just know I'm going to get the sign wrong on this.  */
	if ((lmbrt->quad & csLMBRT_AFFINE) != 0) theta -= lmbrt->affineGamma;

	return (theta);
}

/**********************************************************************
**	k = CSlmbrtK (lmbrt,ll);
**
**	struct cs_Lmbrt_ *lmbrt;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Lambert projection.
**	double ll [2];				the longitude ([0]) and the latitude ([1])
**								of the point at which the grid scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double k;					returns the grid scale factor of the
**								projected coordinate system at that
**								point.
**
**	The cs_Lmbrt_ structure must be setup properly,
**	CSlmbrtS can do this for you.
**********************************************************************/

double EXP_LVL9 CSlmbrtK (Const struct cs_Lmbrt_ *lmbrt,Const double ll [2])
{
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Pi_o_4;			/* Pi over 4 */
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the north pole
										   in radians. */
	extern double cs_SPTest;			/* .001 seconds of arc
										   short of the south pole
										   in radians. */
	extern double cs_SclInf;			/* The standard value we
										   return for an infinite
										   scale factor: 9.9E+04 */

	double lat;
	double kk;

	double tan_lat;
	double cos_lat;
	double sin_lat;
	double esin_lat;
	double t;
	double t_n;
	double m;

	double tmp1;
	double tmp2;
	double tmp3;

	lat = ll [LAT] * cs_Degree;

	cos_lat = cos (lat);
	tan_lat = tan (cs_Pi_o_4 - (lat * cs_Half));

	/* If we are at either pole, the mathemagics below have a
	   problem.  The resulting scale factor is either zero
	   or infinity depending upon the signs of the numbers. */

	if (lat >= cs_NPTest)
	{
		kk = (lmbrt->n >= 0.0) ? cs_Zero : cs_SclInf;
	}
	else if (lat <= cs_SPTest)
	{
		kk = (lmbrt->n <= 0.0) ? cs_Zero : cs_SclInf;
	}
	else if (lmbrt->ecent == 0.0)
	{
		/* Here for the sphere. */

		tmp1 = tan (cs_Pi_o_4 + (lat * cs_Half));
		tmp2 = cos_lat * pow (tmp1,lmbrt->n);
		kk = lmbrt->k_tlat / tmp2;

		/* If we have an affine post process, we need to factor that in. */
		if ((lmbrt->quad & csTRMER_AFFINE) != 0) kk *= lmbrt->affineK;
	}
	else
	{
		/* Here for the ellipsoid. */

		sin_lat = sin (lat);
		esin_lat = lmbrt->ecent * sin_lat;
		tmp1 = (cs_One - esin_lat) / (cs_One + esin_lat);
		tmp2 = pow (tmp1,lmbrt->e_ovr_2);
		t = tan_lat / tmp2;
		t_n = pow (t,lmbrt->n);

		tmp3 = lmbrt->e_sq * sin_lat * sin_lat;
		tmp3 = sqrt (cs_One - tmp3);
		m = cos_lat / tmp3;

		kk = (lmbrt->n * lmbrt->k0 * lmbrt->F * t_n * lmbrt->ellipsoidK) / m ;

		/* If we have an affine post process, we need to factor that in. */
		if ((lmbrt->quad & csTRMER_AFFINE) != 0) kk *= lmbrt->affineK;
	}
	return (kk);
}

/**********************************************************************
**	status = CSlmbrtL (lmbrt,cnt,pnts);
**
**	struct cs_Lmbrt_ *lmbrt;	the coordinate system against which the check is
**								to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
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

int EXP_LVL9 CSlmbrtL (Const struct cs_Lmbrt_ *lmbrt,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.2... */
	extern double cs_Pi_o_2;		/*  PI / 2 */
	extern double cs_Mpi_o_2;		/* -PI / 2 */
	extern double cs_Pi;			/*  PI */
	extern double cs_NPTest;		/* .001 seconds of arc short of
									   the north pole, in radians. */
	extern double cs_SPTest;		/* .001 seconds of arc short of
									    the south pole, in radians. */

	int ii;
	int status;
	int sgn_lng1, sgn_lng2;

	double tmp;
	double s_limit, n_limit;
	double del_lng1, del_lng2;

	/* Check all the points.  The sign of n tells which pole is
	   the focus pole. */

	if (lmbrt->n >= 0.0)
	{
		s_limit = cs_SPTest;
		n_limit = cs_Pi_o_2;
	}
	else
	{
		s_limit = cs_Mpi_o_2;
		n_limit = cs_NPTest;
	}

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		tmp = pnts [ii][LAT] * cs_Degree;
		if (tmp < s_limit || tmp > n_limit)
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
	del_lng1 = CS_adj2pi (tmp - lmbrt->org_lng);
	sgn_lng1 = (del_lng1 < 0.0);
	for (ii = 1;ii < cnt;ii++)
	{
		/* Does the line cross the back azimuth of the
		   central meridian??? */

		tmp = pnts [ii][LNG] * cs_Degree;
		del_lng2 = CS_adj2pi (tmp - lmbrt->org_lng);
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
**	status = CSlmbrt (lmbrt,cnt,pnts);
**
**	struct cs_Lmbrt_ *lmbrt;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSlmbrtL to check lat/long
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
	This function converts the cartesian coordinate to polar form.
*/

static int CSlmbrtXP (	Const struct cs_Lmbrt_ *lmbrt,Const double xy [2],double *theta)
{
	extern double cs_Zero;					/* 0.0 */

	double x, y;
	double rho;
	double tmp;

	/* Convert to polar coordinates. */

	if (lmbrt->quad == 0)
	{
		x = xy [XX] - lmbrt->x_off;
		y = xy [YY] - lmbrt->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,lmbrt->x_off,lmbrt->y_off,lmbrt->quad);
	}
	tmp = lmbrt->rho0 - y;
	rho = sqrt (x * x + (tmp * tmp));

	/* If rho is greater than the somewhat arbitrary value which
	   CSlmbrtS has calculated as the pole opposite to the focus,
	   we consider it bogus. */

	if (rho > lmbrt->infinity) return (cs_CNVRT_DOMN);

	/* Now for the angle portion of the polar coordinate, we
	   call it theta; it's also the convergence angle. */

	if (rho > lmbrt->pole_test)
	{
		/* At least one of the inputs to atan2 is not zero,
		   its safe. */

		if (lmbrt->n > 0.0) *theta = atan2 (x,tmp);
		else		    *theta = atan2 (-x,-tmp);
	}
	else
	{
		/* At the focus pole. */

		*theta = cs_Zero;
	}

	/* If the theta value is in the cutout region, we have a
	   problem. */

	return ((fabs (*theta) > lmbrt->theta_max) ? cs_CNVRT_DOMN : cs_CNVRT_OK);
}

int EXP_LVL9 CSlmbrtX (Const struct cs_Lmbrt_ *lmbrt,int cnt,Const double pnts [][3])
{
	extern double cs_Pi;

	int ii;
	int status = cs_CNVRT_NRML;

	double tmp;
	double theta1 = 0.0;
	double theta2 = 0.0;
	short sgn_theta1 = 0;
	short sgn_theta2 = 0;

	for (ii = 0;ii < cnt;ii++)
	{
		/* Check the current point. */

		status = CSlmbrtXP (lmbrt,pnts [ii],&theta2);
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

				tmp = fabs (theta1) + fabs (theta2);	/*lint !e644 */
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
