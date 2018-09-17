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
**	err_cnt = CSostroQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSostroQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLatFz;		/* - 89.9999 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLatFz;		/* + 89.9999 */
	extern double cs_SclRedMin;		/* 0.75 */
	extern double cs_SclRedMax;		/* 1.10 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Oblique Sterographic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	
	if (cs_def->org_lat < cs_MinLatFz || cs_def->org_lat > cs_MaxLatFz)
	{
		/* Tell them to use the Polar Sterographic. */
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_USEPLR;
	}

	/* Scale reduction. */

	if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
	}

	/* That's it for the Oblique Sterographic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSostroS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSostroS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Radian;			/*   57.29....  */
	extern double cs_Pi_o_4;			/*  PI / 4.0     */
	extern double cs_Zero;				/*    0.0 */
	extern double cs_Half;				/*    0.5 */
	extern double cs_One;				/*    1.0 */
	extern double cs_Two;				/*    2.0 */
	extern double cs_K90;				/*   90.0 */
	extern double cs_Km90;				/*  -90.0 */
	extern double cs_K89;				/*   89.0 */
	extern double cs_Km89;				/*  -89.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc in
										   radians. */
	extern double cs_MinLatFz;			/* 0.1 seconds of arc short
										   of the south pole in
										   degrees. */
	extern double cs_MaxLatFz;			/* 0.1 seconds of arc short
										   of the north pole in
										   degrees. */

	struct cs_Ostro_ *ostro;

	double tmp1;
	double tmp2;
	double tmp3;

	ostro = &csprm->proj_prms.ostro;

	/* Transfer the necessary arguments to the "ostro" structure. */

	ostro->org_lng = csprm->csdef.org_lng * cs_Degree;
	ostro->org_lat = csprm->csdef.org_lat * cs_Degree;
	ostro->k = csprm->csdef.scale * csprm->csdef.scl_red;
	ostro->x_off = csprm->csdef.x_off;
	ostro->y_off = csprm->csdef.y_off;
	ostro->ecent = csprm->datum.ecent;
	ostro->e_rad = csprm->datum.e_rad;
	ostro->ka = csprm->datum.e_rad * ostro->k;
	ostro->two_ka = ostro->ka * cs_Two;
	ostro->cos_org_lat = cos (ostro->org_lat);
	ostro->sin_org_lat = sin (ostro->org_lat);
	ostro->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* See if this is an equatorial aspect. */

	if (fabs (ostro->org_lat) < cs_AnglTest)
	{
		ostro->aspect = cs_STERO_EQUATOR;
		ostro->sin_org_lat = cs_Zero;
		ostro->cos_org_lat = cs_One;
		ostro->org_lat = cs_Zero;
	}
	else
	{
		ostro->aspect = cs_STERO_OBLIQUE;
	}

	/* Set up a value in the units of the cartesian system
	   which represents one millimeter. Also, the epsilon that
	   the inverse of the International version uses to terminate
	   the iterative algorithm. */

	ostro->one_mm = 0.001 * csprm->csdef.scale;
	ostro->cnvrg = cs_AnglTest;

	/* If the ecentricity is zero, we have a sphere. */

	if (ostro->ecent == 0.0)
	{
		/* Not much we can here. */

		ostro->two_k0 = cs_Two * csprm->csdef.scl_red;
	}
	else
	{
		/* Here for the ellipsoidal calculations. */

		ostro->e_o_2 = csprm->datum.ecent * cs_Half;
		ostro->e_sq = ostro->ecent * ostro->ecent;
		ostro->one_esq = cs_One - ostro->e_sq;

		tmp1 = ostro->ecent * ostro->sin_org_lat; /* used several places */
		tmp2 = cs_One - tmp1 * tmp1;
		tmp3 = sqrt (tmp2);

		ostro->nu0 = ostro->e_rad / tmp3;
		ostro->rho0 = ostro->e_rad * ostro->one_esq / (tmp2 * tmp3);

		ostro->ka = sqrt (ostro->nu0 * ostro->rho0) * ostro->k;
		ostro->two_ka = cs_Two * ostro->ka;

		/* Conformal longitude conversion factor. */

		tmp3 = ostro->cos_org_lat * ostro->cos_org_lat;
		tmp3 = (ostro->e_sq * tmp3 * tmp3) / ostro->one_esq;
		/* If org_lat is proper, and since in this CASE is never
		   a pole, tmp3 can never be <= 0.0 */
		ostro->nn = sqrt (cs_One + tmp3);
		
		/* Conformal latitude of the origin. */

		if (ostro->aspect == cs_STERO_OBLIQUE)
		{
			double S1, S2;
			double tmp4, w1, w2;

			S1 = (cs_One + ostro->sin_org_lat) / (cs_One - ostro->sin_org_lat);
			S2 = (cs_One - tmp1) / (cs_One + tmp1);
			tmp3 = S1 * pow (S2,ostro->ecent);
			w1 = pow (tmp3,ostro->nn);
			ostro->sin_chi0 = (w1 - cs_One) / (w1 + cs_One);
			ostro->cos_chi0 = sqrt (cs_One - ostro->sin_chi0 * ostro->sin_chi0);

			/* Conformal latitude integration factor. */

			tmp3 = cs_One - ostro->sin_chi0;
			tmp4 = (ostro->nn - ostro->sin_org_lat) * (cs_One + ostro->sin_chi0);
			ostro->cc = (ostro->nn + ostro->sin_org_lat) * (tmp3 / tmp4);

			/* Again with the sin_chi0 stuff. */

			w2 = ostro->cc * w1;
			ostro->sin_chi0 = (w2 - cs_One) / (w2 + cs_One);
			ostro->cos_chi0 = sqrt (cs_One - ostro->sin_chi0 * ostro->sin_chi0);
			ostro->chi0 = asin (ostro->sin_chi0);

			/* Terms used only in the non-polar inverse. As we are handling
			   only the oblique case here, floating point exceptions are
			   not possible. */

			ostro->gg = ostro->two_ka * tan (cs_Pi_o_4 - cs_Half * ostro->chi0);
			ostro->hh = cs_Two * ostro->two_ka * tan (ostro->chi0) + ostro->gg;
		}
		else
		{
			ostro->chi0 = cs_Zero;
			ostro->sin_chi0 = cs_Zero;
			ostro->cos_chi0 = cs_One;
			ostro->nn = sqrt ((cs_One + ostro->e_sq) / ostro->one_esq);
			ostro->cc = cs_One;
			ostro->gg = ostro->two_ka;
			ostro->hh = ostro->gg;
		}
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = ostro->org_lng * cs_Radian;

	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to
		   establish the useful range.  We'll establish some
		   pretty liberal values. */

		switch (ostro->aspect) {

		default:
		case cs_STERO_OBLIQUE:

			csprm->min_ll [LNG] = cs_Km90;
			csprm->max_ll [LNG] = cs_K90;
			csprm->min_ll [LAT] = ostro->org_lat * cs_Radian - cs_K90;
			csprm->max_ll [LAT] = ostro->org_lat * cs_Radian + cs_K90;
			if (csprm->min_ll [LAT] < cs_MinLatFz)
			{
				csprm->min_ll [LAT] = cs_MinLatFz;
			}
			if (csprm->max_ll [LAT] > cs_MaxLatFz)
			{
				csprm->max_ll [LAT] = cs_MaxLatFz;
			}
			break;

		case cs_STERO_EQUATOR:

			csprm->min_ll [LNG] = cs_Km90;
			csprm->max_ll [LNG] = cs_K90;
			csprm->min_ll [LAT] = cs_Km89;
			csprm->max_ll [LAT] = cs_K89;
			break;
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
		/* No specification in the coordinate system definition. */

		csprm->min_xy [XX] = -ostro->ka;
		csprm->max_xy [XX] =  ostro->ka;
		csprm->min_xy [YY] = -ostro->ka;
		csprm->max_xy [YY] =  ostro->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,ostro->x_off,
											   ostro->y_off,
											   ostro->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSostroF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSostroI;
	csprm->cs_scale = (cs_SCALE_CAST)CSostroK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSostroK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSostroK;	/* Conformal */
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSostroC;
	csprm->llchk    = (cs_LLCHK_CAST)CSostroL;
	csprm->xychk    = (cs_XYCHK_CAST)CSostroX;

	return;
}

/**********************************************************************
**	rtn_val = CSostroF (ostro,xy,ll);
**
**	struct cs_Ostro_ *ostro;	structure which defines the parameters
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
**	This function calculates the Oblique Sterographic
**	projection and returns values in the coordinate system
**	described by the cs_Ostro_ structure.  If the ecentricity
**	of the ellipsoid is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections - A Working Manual"
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 154-163.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSostroF (Const struct cs_Ostro_ *ostro,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Mpi;				/* -3.14159... */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_Pi_o_2;			/* PI over 2 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double lng;				/* The given longitude, after conversion
							   to radians. */
	double lat;				/* The given latitude after conversion
							   to radians. */
	double x;
	double y;

	double del_lng, delLambda;
	double sin_del_lng, sinDelLambda;
	double cos_del_lng, cosDelLambda;
	double sin_lat;
	double cos_lat;
	double sin_chi;
	double cos_chi;

	double B;
	double kR;
	double Sa, Sb;
	double ww;
	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and one for the
	   ellipsoid.  If the ecentricity of the ellipsoid in use is 0.0
	   exactly, we shall use the spherical formulae. */

	lat = cs_Degree * ll [LAT];
	if (fabs (lat) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}

	lng = cs_Degree * ll [LNG];
	
	del_lng = lng - ostro->org_lng;
	if      (del_lng > cs_Pi  && ostro->org_lng > 0.0) del_lng += cs_Two_pi;
	else if (del_lng < cs_Mpi && ostro->org_lng < 0.0) del_lng -= cs_Two_pi;
	if (del_lng > cs_Pi || del_lng <= cs_Mpi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}
	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* See if the ellipsoid is a sphere. */

	if (ostro->ecent == 0.0)
	{
		/* Here for the sphere. Note this code is duplicated in the
		   Synder version. While duplicate code is undesireable, the
		   alternative is an ugly bunch of spaghetti code. */

		switch (ostro->aspect) {

		default:
		case cs_STERO_OBLIQUE:

			tmp1 = cos_lat * cos_del_lng;
			tmp2 = cs_One + (ostro->sin_org_lat * sin_lat) +
				        (ostro->cos_org_lat * tmp1);
			if (tmp2 < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp2 = cs_AnglTest;
			}
			kR = cs_Two * ostro->ka / tmp2;

			/* X and Y */

			x = kR * cos_lat * sin_del_lng;
			y = kR * ((ostro->cos_org_lat * sin_lat) -
				      (ostro->sin_org_lat * tmp1));
			break;

		case cs_STERO_EQUATOR:

			/* This is a faster version for the equatorial
			   aspect. */

			tmp1 = cs_One + (cos_lat * cos_del_lng);
			if (tmp1 < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp1 = cs_AnglTest;
			}
			kR = cs_Two * ostro->ka / tmp1;

			x = kR * cos_lat * sin_del_lng;
			y = kR * sin_lat;
			break;
		}
	}
	else
	{
		/* Here for an ellipsoid model of the earth.

		   First, we get the conformal longtiude (i.e. delta
		   longitude). */

		delLambda = ostro->nn * del_lng;
		sinDelLambda = sin (delLambda);
		cosDelLambda = cos (delLambda);

		/* Now the conformal latitude. We can't use the conformal
		   series here (i.e. CSchiFcal) as these conformal
		   latitudes are the result of a differential equation
		   solution based on different initial conditions than
		   that used to generate the series. Therefore, the
		   results of this calculation and the series will be
		   slightly different. */

		if (fabs (lat) > cs_NPTest)
		{
			/* At the poles, conformal latitude is the same
			   as the geographic. If we don't do this, we'll
			   end up with a deivide by zero below. */

			if (lat >= 0.0)
			{
				sin_chi = cs_One;
				cos_chi = cs_Zero;
			}
			else
			{
				sin_chi = cs_Mone;
				cos_chi = cs_Zero;
			}
		}
		else
		{
			tmp1 = ostro->ecent * sin_lat;
			Sa = (cs_One + sin_lat) / (cs_One - sin_lat);
			Sb = (cs_One - tmp1) / (cs_One + tmp1);
			tmp1 = Sa * pow (Sb,ostro->ecent);
			ww = ostro->cc * pow (tmp1,ostro->nn);
			sin_chi = (ww - cs_One) / (ww + cs_One);
			cos_chi = sqrt (cs_One - sin_chi * sin_chi);
		}

		switch (ostro->aspect) {

		default:		
		case cs_STERO_OBLIQUE:

			/* The usual, also the general, case. */

			B = cos_chi * ostro->cos_chi0 * cosDelLambda;
			B += sin_chi * ostro->sin_chi0;
			B += cs_One;

			x =  ostro->two_ka * cos_chi * sinDelLambda / B;
			y =  sin_chi * ostro->cos_chi0 - cos_chi * ostro->sin_chi0 * cosDelLambda;
			y *= ostro->two_ka / B;
			break;

		case cs_STERO_EQUATOR:

			/* A slightly faster version, sin_chi0 = 0.0, and
			   cos_chi0 = 1.0 */

			B = cs_One + cos_chi * cos (delLambda);

			x = ostro->two_ka * cos_chi * sinDelLambda / B;
			y = ostro->two_ka * sin_chi / B;
			break;
		}
	}

	/* Finally, handle the users desired cartesian system. */

	if (ostro->quad == 0)
	{
		xy [XX] = x + ostro->x_off;
		xy [YY] = y + ostro->y_off;
	}
	else
	{
		CS_quadF (xy,x,y,ostro->x_off,ostro->y_off,ostro->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSostroI (ostro,ll,xy);
**
**	struct cs_Ostro_ *ostro;	structure which defines the parameters
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
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSostroI (Const struct cs_Ostro_ *ostro,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Pi_o_2;			/* PI over 2.0 */
	extern double cs_Pi_o_4;			/* PI over 4.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double x;
	double y;

	double c;
	double ii, jj;
	double lat, last_lat;
	double chi;
	double rho;
	double sin_c;
	double cos_c;
	double del_lng;
	double psi,psi_I;
	double sin_chi;

	double tmp1;
	double tmp2;
	double tmp3;

	rtn_val = cs_CNVRT_NRML;

	/* Adjust for a non-standard quadrant. */

	if (ostro->quad == 0)
	{
		x = xy [XX] - ostro->x_off;
		y = xy [YY] - ostro->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,ostro->x_off,ostro->y_off,ostro->quad);
	}

	/* If x and y are now both zero, or very close to it,
	   we must just set the result to the origin of the
	   projection.  We don't want to divide by rho if it's
	   zero. */

	rho = sqrt (x * x + y * y);
	if (rho <= ostro->one_mm)
	{
		/* The coordinate is essentially zero.  Return the origin of
		   the coordinate system. */

		ll [LNG] = ostro->org_lng * cs_Radian;
		ll [LAT] = ostro->org_lat * cs_Radian;
		return (rtn_val);
	}

	/* Now we can convert back to lat/longs. */

	if (ostro->ecent == 0.0)
	{
		/* Here for the sphere.  Note, we have already filtered out cases
		   where rho is zero (or very close to zero).

		   Note, rho can approach infinity, although considering
		   values greater than two_ka out of the domain would
		   be generally acceptable. Read c as the angular distance
		   from the origin.

		   This code is duplicated in the SYnder version of the
		   Sterographic. Undesireable, but the alternative is bunch
		   of spaghetti code. */

		c = cs_Two * atan (rho / ostro->two_ka);
		sin_c = sin (c);
		cos_c = cos (c);

		switch (ostro->aspect) {

		default:
		case cs_STERO_OBLIQUE:

			/* c is the angluar distance from the origin to
			   the point.  If rho is not zero (we dealt with
			   that above) we assume that c cannot be zero. */

			tmp1 = x * sin_c;
			tmp2 = (rho * ostro->cos_org_lat * cos_c) -
			       (y * ostro->sin_org_lat * sin_c);
			ll [LNG] = ostro->org_lng + atan2 (tmp1,tmp2);

			tmp1 = cos_c * ostro->sin_org_lat;
			tmp2 = y * sin_c * ostro->cos_org_lat / rho;
			ll [LAT] = asin (tmp1 + tmp2);
			break;

		case cs_STERO_EQUATOR:

			/* Same as above, but cos_org_lat = 1.0 and
			   sin_org_lat = 0.0; should go a bit faster. */

			tmp1 = x * sin_c;
			tmp2 = rho * cos_c;
			ll [LNG] = ostro->org_lng + atan2 (tmp1,tmp2);

			tmp1 = y * sin_c / rho;
			ll [LAT] = asin (tmp1);
			break;
		}
	}
	else
	{
		/* Here for the ellisoid. Nothing is saved by having
		   special cases for the equatorial aspect here. So,
		   there is just one coding for the general case.

		   We reluctantly use ii and jj here as doubles.
		   This is, however, the notation used in the
		   source document. Therefore, we use that notation. */

		ii = atan2 (x,ostro->hh + y);
		jj = atan2 (x,ostro->gg - y) - ii;
		del_lng = (jj + cs_Two * ii) / ostro->nn;

		tmp1 = y - x * tan (cs_Half * jj);
		chi = ostro->chi0 + cs_Two * atan2 (tmp1,ostro->two_ka);
		sin_chi = sin (chi);
		
		tmp1 = cs_One + sin_chi;
		tmp2 = ostro->cc * (cs_One - sin_chi);
		psi = cs_Half * log (tmp1 / tmp2) / ostro->nn;
		
		lat = cs_Two * atan (exp (psi)) - cs_Pi_o_2;
		do
		{
			last_lat = lat;
			tmp1 = ostro->ecent * sin (last_lat);
			tmp2 = tan (cs_Half * last_lat + cs_Pi_o_4);
			tmp3 = (cs_One - tmp1) / (cs_One + tmp1);
			tmp3 = pow (tmp3,ostro->e_o_2);
			psi_I = log (tmp2 * tmp3);
			tmp2 = (cs_One - tmp1 * tmp1) / ostro->one_esq;
			lat = last_lat - (psi_I - psi) * cos (last_lat) * tmp2;
		} while (fabs (lat - last_lat) >= ostro->cnvrg);

		ll [LNG] = ostro->org_lng + del_lng;
		ll [LAT] = lat;
	}
	if (fabs (ll [LAT]) > cs_NPTest && rtn_val == cs_CNVRT_NRML)	/*lint !e774 */
	{
		rtn_val = cs_CNVRT_INDF;
	}
	ll [LNG] *= cs_Radian;
	ll [LAT] *= cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSostroC (ostro,ll);
**
**	struct cs_Ostro_ *ostro;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Oblique Stereographic
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

double EXP_LVL9 CSostroC (Const struct cs_Ostro_ *ostro,Const double ll [2])
{
	extern double cs_Radian;			/* 57.2957... */
	extern double cs_Km360;				/* -360.0, the value which
										   we return if provided
										   with a bogus point. */
	int status;

	double del_xx;
	double del_yy;
	double gamma;		/* some folks call this alpha */

	double xy1 [3];
	double xy2 [3];
	double my_ll [3];

	/* We have no analytical formulas for any of the aspects/variations
	   supported. Therefore, we can handle all cases with the simple
	   empirical algorithm below. */

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
	status = CSostroF (ostro,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSostroF (ostro,xy2,my_ll);
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
	if ((fabs (del_xx) + fabs (del_yy)) >= ostro->one_mm)
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
**	kk = CSostroK (ostro,ll);
**
**	struct cs_Stero_ *ostro;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Oblique Sterographic
**								projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	The Oblique Sterographic projection is conformal, therefore, the
**	grid scale factor returned by this function is valid in
**	all directions.  Thus, there is no CSostroH function.
**
**	Analytical formula have not, as yet, neem located. Thus, an
**	empirical approach is used here.
**********************************************************************/

double EXP_LVL9 CSostroK (Const struct cs_Ostro_ *ostro,Const double ll [2])
{
	extern double cs_SclInf;		/* 9.9E+04, the value which we
									   return for an infinite
									   scale factor. */
	extern double cs_Mone;			/* -1.0 */
	extern double cs_AnglTest;		/* 0.001 arc seconds, in radians. */
	extern double cs_HlfSecDeg;		/* 0.5 seconds of arc, in degrees. */

	int status;

	double kk;
	double xy_dd, ll_dd;
	double del_xx, del_yy;

	double ll1 [2];
	double ll2 [2];
	double xy1 [2];
	double xy2 [2];

	/* Establish two points along the parallel which
	   are about 1 second (about 30 meters) apart from
	   each other, with the point in question in the
	   middle.  Then convert each point to the equivalent
	   grid coordinates. */

	ll1 [LNG] = ll [LNG] - cs_HlfSecDeg;
	ll1 [LAT] = ll [LAT];
	status = CSostroF (ostro,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll2 [LNG] = ll [LNG] + cs_HlfSecDeg;
	ll2 [LAT] = ll [LAT];
	status = CSostroF (ostro,xy2,ll2);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);
	if (xy_dd == 0.0)
	{
		/* This can happen when very close to either pole.
		   Since we only get here on an oblique or equatorial,
		   we return minus one.  What we should do is switch
		   and do an H calculation here. This should be the
		   same as what the K would be since the projection
		   is conformal. */

		return (cs_Mone);
	}

	/* Calculate the geodetic distance between the two
	   lat/long points. We know we're on an ellipsoide, else
	   we wouldn't be here. */

	CS_llazdd (ostro->ka,ostro->e_sq,ll1,ll2,&ll_dd);

	/* Return the ration of the two distances as the grid
	   scale factor at the point. */

	if (ll_dd > cs_AnglTest)
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
**	status = CSostroL (ostro,cnt,pnts);
**
**	struct cs_Ostro_ *ostro;	the coordinate system against which the check is
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

int EXP_LVL9 CSostroL (Const struct cs_Ostro_ *ostro,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_xy [3];

	/* Check all the points.  */

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSostroF (ostro,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	status = CSostroX (ostro,cnt,pnts);
**
**	struct cs_Ostro_ *ostro;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see remarks
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSostroL to check lat/long
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

int EXP_LVL9 CSostroX (Const struct cs_Ostro_ *ostro,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* The domain is contiguous, there are no singularities
	   within the basic range of the projection. If all points
	   are within the domain, then all lines and regions will
	   be within the domain. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSostroI (ostro,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

