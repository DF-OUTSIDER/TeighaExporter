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
**	err_cnt = CSsstroQ (cs_def,prj_code,err_list,list_sz);
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
**	All lat/longs in definitions must be referenced to Greenwich,
**	and in the range of greater than -180/-90, and less than or
**	equal to +180/+90.
**********************************************************************/

int EXP_LVL9 CSsstroQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLatFz;		/* - 89.999 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLatFz;		/* + 89.999 */
	extern double cs_SclRedMin;		/* 0.75 */
	extern double cs_SclRedMax;		/* 1.10 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check Synder's Stereographic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	
	if (cs_def->org_lat < cs_MinLatFz || cs_def->org_lat > cs_MaxLatFz)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_USEPLR;
	}

	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 >= cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_AZMTH;
	}

	/* Scale reduction. */

	if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
	}

	/* That's it for Synder's Stereographic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSsstroS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**
**	Use the Polar Stereographic for polar aspects.
**********************************************************************/

void EXP_LVL9 CSsstroS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Radian;			/*   57.29....  */
	extern double cs_Pi_o_4;			/*  PI / 4.0     */
	extern double cs_Pi_o_2;			/*  PI / 2.0     */
	extern double cs_Zero;				/*    0.0 */
	extern double cs_Half;				/*    0.5 */
	extern double cs_One;				/*    1.0 */
	extern double cs_Two;				/*    2.0 */
	extern double cs_Three;				/*    3.0 */
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

	struct cs_Sstro_ *sstro;

	double chi1;
	double tmp1;
	double tmp2;
	double tmp3;

	sstro = &csprm->proj_prms.sstro;

	/* Transfer the necessary arguments to the "sstro" structure. */

	sstro->org_lng = csprm->csdef.org_lng * cs_Degree;
	sstro->org_lat = csprm->csdef.org_lat * cs_Degree;
	sstro->k = csprm->csdef.scale * csprm->csdef.scl_red;
	sstro->x_off = csprm->csdef.x_off;
	sstro->y_off = csprm->csdef.y_off;
	sstro->ecent = csprm->datum.ecent;
	sstro->e_rad = csprm->datum.e_rad;
	sstro->ka = csprm->datum.e_rad * sstro->k;
	sstro->two_ka = sstro->ka * cs_Two;
	sstro->cos_org_lat = cos (sstro->org_lat);
	sstro->sin_org_lat = sin (sstro->org_lat);
	sstro->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	sstro->Az = csprm->csdef.prj_prm1 * cs_Degree;
	sstro->sin_Az = sin (sstro->Az);
	sstro->cos_Az = cos (sstro->Az);

	sstro->aspect = cs_STERO_OBLIQUE;
	if (fabs (sstro->org_lat) < cs_AnglTest)
	{
		sstro->aspect = cs_STERO_EQUATOR;
		sstro->sin_org_lat = cs_Zero;
		sstro->cos_org_lat = cs_One;
		sstro->org_lat = cs_Zero;
	}

	/* Set up a value in the units of the cartesian system
	   which represents one millimeter. Also, the epsilon that
	   the inverse of the International version uses to terminate
	   the iterative algorithm. */

	sstro->one_mm = 0.001 * sstro->k;
	/* The following is to adjust for the test case from Snyder where the
	   unit sphere is used. */
	if (sstro->e_rad <= cs_Three) sstro->one_mm = 2.0E-10;

	/* If the ecentricity is zero, we have a sphere.  The
	   set up is a bit different in this case. */

	if (sstro->ecent == 0.0)
	{
		/* Not much we can do here. */

		sstro->two_k0 = cs_Two * csprm->csdef.scl_red;
	}
	else
	{
		/* Here for the ellipsoidal calculations. */

		sstro->e_o_2 = csprm->datum.ecent * cs_Half;
		sstro->e_sq = sstro->ecent * sstro->ecent;

		tmp1 = sstro->ecent * sstro->sin_org_lat;
		tmp2 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp2 = pow (tmp2,sstro->e_o_2);
		tmp3 = cs_Pi_o_4 + (sstro->org_lat * cs_Half);
		tmp2 = tan (tmp3) * tmp2;
		chi1 = cs_Two * atan (tmp2) - cs_Pi_o_2;
		sstro->sin_chi1 = sin (chi1);
		sstro->cos_chi1 = cos (chi1);

		tmp1 *= tmp1;
		sstro->m1 = sstro->cos_org_lat / sqrt (cs_One - tmp1);
		sstro->two_kam1 = sstro->two_ka * sstro->m1;

		CSchiIsu (&sstro->chicofI,sstro->e_sq);
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = sstro->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to
		   establish the useful range.  We'll establish some
		   pretty liberal values. */

		if (sstro->aspect == cs_STERO_OBLIQUE)
		{
			csprm->min_ll [LNG] = cs_Km90;
			csprm->max_ll [LNG] = cs_K90;
			csprm->min_ll [LAT] = sstro->org_lat * cs_Radian - cs_K89;
			csprm->max_ll [LAT] = sstro->org_lat * cs_Radian + cs_K89;
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
			csprm->min_ll [LNG] = cs_Km90;
			csprm->max_ll [LNG] = cs_K90;
			csprm->min_ll [LAT] = cs_Km89;
			csprm->max_ll [LAT] = cs_K89;
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

		csprm->min_xy [XX] = -sstro->ka;
		csprm->max_xy [XX] =  sstro->ka;
		csprm->min_xy [YY] = -sstro->ka;
		csprm->max_xy [YY] =  sstro->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,sstro->x_off,
											   sstro->y_off,
											   sstro->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSsstroF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSsstroI;
	csprm->cs_scale = (cs_SCALE_CAST)CSsstroK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSsstroK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSsstroK;	/* Conformal */
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSsstroC;
	csprm->llchk    = (cs_LLCHK_CAST)CSsstroL;
	csprm->xychk    = (cs_XYCHK_CAST)CSsstroX;

	return;
}

/**********************************************************************
**	rtn_val = CSsstroF (sstro,xy,ll);
**
**	struct cs_Sstro_ *sstro;	structure which defines the parameters
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
**	This function calculates Synder's Sterographic
**	projection and returns values in the coordinate system
**	described by the cs_Sstro_ structure.  If the ecentricity
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

int EXP_LVL9 CSsstroF (Const struct cs_Sstro_ *sstro,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Mpi;				/* -3.14159... */
	extern double cs_Two_pi;			/* 2 Pi */
	extern double cs_Pi_o_2;			/* PI over 2 */
	extern double cs_Pi_o_4;			/* PI over 4 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double lng;			/* The given longitude, after conversion
						   to radians. */
	double lat;			/* The given latitude after conversion
						   to radians. */
	double x;
	double y;

	double del_lng;
	double sin_del_lng;
	double cos_del_lng;
	double sin_lat;
	double cos_lat;
	double sin_chi;
	double cos_chi;
	double tmp_chi;

	double A;
	double kR;
	double chi;
	double tmp1;
	double tmp2;
	double tmp3;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and one for the
	   ellipsoid.  If the ecentricity of the ellipsoid in use is
	   0.0 exactly, we shall use the spherical formulae. */

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
	del_lng = lng - sstro->org_lng;

	if      (del_lng > cs_Pi  && sstro->org_lng > 0.0) del_lng += cs_Two_pi;
	else if (del_lng < cs_Mpi && sstro->org_lng < 0.0) del_lng -= cs_Two_pi;
	if (del_lng > cs_Pi || del_lng <= cs_Mpi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}
	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* See if we have a sphere or an ellipsoid. */

	if (sstro->ecent == 0.0)
	{
		/* Here for the sphere. */

		switch (sstro->aspect) {

		default:
		case cs_STERO_OBLIQUE:

			tmp1 = cos_lat * cos_del_lng;
			tmp2 = cs_One + (sstro->sin_org_lat * sin_lat) +
				       (sstro->cos_org_lat * tmp1);
			if (tmp2 < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp2 = cs_AnglTest;
			}
			kR = cs_Two * sstro->ka / tmp2;

			/* X and Y */

			x = kR * cos_lat * sin_del_lng;
			y = kR * ((sstro->cos_org_lat * sin_lat) - (sstro->sin_org_lat * tmp1));
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
			kR = cs_Two * sstro->ka / tmp1;

			x = kR * cos_lat * sin_del_lng;
			y = kR * sin_lat;
			break;
		}
	}
	else
	{
		/* Here for an ellipsoid model of the earth. */

		tmp1 = sstro->ecent * sin_lat;
		tmp2 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp2 = pow (tmp2,sstro->e_o_2);
		tmp3 = lat * cs_Half;
		tmp_chi = tan (cs_Pi_o_4 + tmp3) * tmp2;

		switch (sstro->aspect) {

		default:		
		case cs_STERO_OBLIQUE:

			/* This is Synder's rendition of the oblique. 

			   Read (tmp1 + tmp2) as 1 - cos (c) where c is
			   the angular distance from the origin to the
			   point being converted.  When the point is
			   antipodal to the origin, (tmp1 + tmp2) will be
			   zero, hence tmp3 will be zero. */

			chi = cs_Two * atan (tmp_chi) - cs_Pi_o_2;
			sin_chi = sin (chi);
			cos_chi = cos (chi);

			tmp1 = sstro->cos_chi1 * cos_chi * cos_del_lng;
			tmp2 = cs_One + sstro->sin_chi1 * sin_chi;
			tmp3 = sstro->cos_chi1 * (tmp2 + tmp1);
			if (tmp3 < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp3 = cs_AnglTest;
			}
			A = sstro->two_kam1 / tmp3;

			x = A * cos_chi * sin_del_lng;
			tmp1 = sstro->cos_chi1 * sin_chi;
			tmp2 = sstro->sin_chi1 * cos_chi * cos_del_lng;
			y = A * (tmp1 - tmp2);
			break;

		case cs_STERO_EQUATOR:

			chi = cs_Two * atan (tmp_chi) - cs_Pi_o_2;
			sin_chi = sin (chi);
			cos_chi = cos (chi);

			tmp1 = cs_One + (cos_chi * cos_del_lng);
			if (tmp1 < cs_AnglTest)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp1 = cs_AnglTest;
			}
			A = sstro->two_ka / tmp1;

			x = A * cos_chi * sin_del_lng;
			y = A * sin_chi;
			break;
		}
	}
	
	/* Add the rotation, adjust for the selected quadrant,
	   and, finally, add flase easting and false northing. */

	if (sstro->Az != 0.0)
	{
		xy [XX] = sstro->cos_Az * x - sstro->sin_Az * y;
		xy [YY] = sstro->cos_Az * y + sstro->sin_Az * x;
	}
	else
	{
		xy [XX] = x;
		xy [YY] = y;
	}

	/* Finally, handle the users desired cartesian system. */

	if (sstro->quad == 0)
	{
		xy [XX] += sstro->x_off;
		xy [YY] += sstro->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],sstro->x_off,sstro->y_off,
							  sstro->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSsstroI (sstro,ll,xy);
**
**	struct cs_Sstro_ *sstro;	structure which defines the parameters
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
**	USGS publication "Map Projections - A Working Manual",
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 154-163.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSsstroI (Const struct cs_Sstro_ *sstro,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Two;				/* 2.0 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double x;
	double y;

	double c;
	double ce;
	double chi;
	double rho;
	double sin_c;
	double cos_c;
	double sin_ce;
	double cos_ce;

	double tmp1;
	double tmp2;

	double my_xy [2];

	rtn_val = cs_CNVRT_NRML;

	/* Adjust for a non-standard quadrant. */

	if (sstro->quad == 0)
	{
		x = xy [XX] - sstro->x_off;
		y = xy [YY] - sstro->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,sstro->x_off,sstro->y_off,sstro->quad);
	}

	/* Adjust for a Y axis which is not true north. */

	if (sstro->Az != 0.0)
	{
		my_xy [XX] = x;
		my_xy [YY] = y;
		x = sstro->cos_Az * my_xy [XX] + sstro->sin_Az * my_xy [YY];
		y = sstro->cos_Az * my_xy [YY] - sstro->sin_Az * my_xy [XX];
	}

	/* If x and y are now both zero, or very close to it,
	   we must just set the result to the origin of the
	   projection.  We don't want to divide by rho if it's
	   zero. */

	rho = sqrt (x * x + y * y);
	if (rho <= sstro->one_mm)
	{
		/* The coordinate is essentially zero.  Return
		   the origin of the coordinate system. */

		ll [LNG] = sstro->org_lng * cs_Radian;
		ll [LAT] = sstro->org_lat * cs_Radian;
		return (rtn_val);
	}

	/* Now we can convert back to lat/longs. */

	if (sstro->ecent == 0.0)
	{
		/* Here for the sphere.  Note, we have already
		   filtered out cases where rho is zero (or very
		   close to zero).

		   Note, rho can approach infinity, although considering
		   values greater than two_ka out of the domain would
		   be generally acceptable. Read c as the angular distance
		   from the origin.

		   Note that this code is duplicated in the spherical
		   form of the Oblique Stereographic. Undesirable, but
		   the only alternative is a lot of spaghetti code. */

		c = cs_Two * atan (rho / sstro->two_ka);
		sin_c = sin (c);
		cos_c = cos (c);

		switch (sstro->aspect) {

		default:
		case cs_STERO_OBLIQUE:

			/* c is the angluar distance from the origin to
			   the point.  If rho is not zero (we dealt with
			   that above) we assume that c cannot be zero. */

			tmp1 = x * sin_c;
			tmp2 = (rho * sstro->cos_org_lat * cos_c) -
			       (y * sstro->sin_org_lat * sin_c);
			ll [LNG] = sstro->org_lng + atan2 (tmp1,tmp2);

			tmp1 = cos_c * sstro->sin_org_lat;
			tmp2 = y * sin_c * sstro->cos_org_lat / rho;
			ll [LAT] = asin (tmp1 + tmp2);
			break;

		case cs_STERO_EQUATOR:

			/* Same as above, but cos_org_lat = 1.0 and
			   sin_org_lat = 0.0; should go a bit faster. */

			tmp1 = x * sin_c;
			tmp2 = rho * cos_c;
			ll [LNG] = sstro->org_lng + atan2 (tmp1,tmp2);

			tmp1 = y * sin_c / rho;
			ll [LAT] = asin (tmp1);
			break;
		}
	}
	else
	{
		/* Here for the ellisoid. Not much is saved by having
		   special cases for the equatorial aspect here. */

		tmp1 = rho * sstro->cos_chi1;
		ce = cs_Two * atan2 (tmp1,sstro->two_kam1);
		sin_ce = sin (ce);
		cos_ce = cos (ce);

		tmp1 = x * sin_ce;
		tmp2 = rho * sstro->cos_chi1 * cos_ce;
		tmp2 -= y * sstro->sin_chi1 * sin_ce;
		ll [LNG] = sstro->org_lng + atan2 (tmp1,tmp2);

		tmp1 = cos_ce * sstro->sin_chi1;
		tmp2 = (y * sin_ce * sstro->cos_chi1) / rho;
		chi = asin (tmp1 + tmp2);
		ll [LAT] = CSchiIcal (&sstro->chicofI,chi);
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
**	gamma = CSsstroC (sstro,ll);
**
**	struct cs_Sstro_ *sstro;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming Synder's Stereographic
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

double EXP_LVL9 CSsstroC (Const struct cs_Sstro_ *sstro,Const double ll [2])
{
	extern double cs_Radian;			/* 57.2957... */
	extern double cs_Km360;				/* -360.0, the value which
										   we return if provided
										   with a bogus point. */
	int status;

	double del_xx;
	double del_yy;
	double gamma;				/* some folks call this alpha */

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
	status = CSsstroF (sstro,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSsstroF (sstro,xy2,my_ll);
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
	if ((fabs (del_xx) + fabs (del_yy)) >= sstro->one_mm)
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
**	kk = CSsstroK (sstro,ll);
**
**	struct cs_Sstro_ *sstro;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Azimuthal Equal
**								Area projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	Synder's Sterographic projection is conformal, therefore, the
**	grid scale factor returned by this function is valid in
**	all directions.  Thus, there is no CSsstroH function.
**********************************************************************/

double EXP_LVL9 CSsstroK (Const struct cs_Sstro_ *sstro,Const double ll [2])
{
	extern double cs_Pi_o_2;			/*  PI / 2.0 */
	extern double cs_Pi_o_4;			/*  PI / 4.0 */
	extern double cs_Degree;			/*  1.0 / 57.29577... */
	extern double cs_Half;				/*  0.5 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Two;				/*  2.0 */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for infinite
										   scale. */
	extern double cs_AnglTest;			/* 0.001 seconds of arc, in
										   radians. */

	double A;
	double m;
	double kk;
	double lng;
	double lat;
	double tmp1;
	double tmp2;
	double chi;
	double del_lng;
	double sin_lat;
	double cos_lat;
	double tmp_chi;
	double sin_chi;
	double cos_chi;
	double esin_lat;
	double cos_del_lng;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		return (cs_Mone);
	}
	sin_lat = sin (lat);
	cos_lat = cos (lat);
	del_lng = CS_adj2pi (lng - sstro->org_lng);
	cos_del_lng = cos (del_lng);

	if (sstro->ecent == 0.0)
	{
		/* Here for the spherical case. */

		if (sstro->aspect == cs_STERO_EQUATOR)
		{
			/* tmp1 will be zero if the point is antipodal to
			   the origin. */

			tmp1 = cs_One + (cos_lat * cos_del_lng);
			if (tmp1 > cs_AnglTest)
			{
				kk = sstro->two_k0 / tmp1;
			}
			else
			{
				kk = cs_SclInf;
			}
		}
		else
		{
			/* tmp1 will be zero if the point is antipodal to
			   the origin. */

			tmp1 = sstro->cos_org_lat * cos_lat * cos_del_lng;
			tmp2 = sstro->sin_org_lat * sin_lat;
			tmp1 = cs_One + tmp2 + tmp1;
			if (tmp1 > cs_AnglTest)
			{
				kk = sstro->two_k0 / tmp1;
			}
			else
			{
				kk = cs_SclInf;
			}
		}
	}
	else
	{
		/* Here for the ellipsoid case. */

		esin_lat = sstro->ecent * sin_lat;
		tmp1 = (cs_One - esin_lat) / (cs_One + esin_lat);
		tmp1 = pow (tmp1,sstro->e_o_2);
		tmp2 = lat * cs_Half;
		tmp_chi = tan (cs_Pi_o_4 + tmp2) * tmp1;
		chi = cs_Two * atan (tmp_chi) - cs_Pi_o_2;

		/* Careful, m is zero at the poles. */

		m = cos_lat / sqrt (cs_One - esin_lat * esin_lat);

		cos_chi = cos (chi);
		sin_chi = sin (chi);
		tmp1 = sstro->cos_chi1 * cos_chi * cos_del_lng;
		tmp2 = cs_One + sstro->sin_chi1 * sin_chi;
		tmp2 = sstro->cos_chi1 * (tmp2 + tmp1);

		/* Tmp2 will be zero if the point is
		   antipodal to the origin. */

		if (tmp2 < cs_AnglTest) tmp2 = cs_AnglTest;
		A = sstro->two_kam1 / tmp2;

		/* As lat approaches either pole, m and cos_chi
		   both approach zero.  Thus, assume that the
		   effect of both terms cancel each other out
		   and we arrive at k = A/e_rad. */

		if (m > cs_AnglTest)
		{
			kk = (A * cos_chi) / (sstro->e_rad * m);
		}
		else
		{
			kk = A / sstro->e_rad;
		}
	}
	return (kk);
}

/**********************************************************************
**	status = CSsstroL (sstro,cnt,pnts);
**
**	struct cs_Sstro_ *sstro;	the coordinate system against which the check is
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

int EXP_LVL9 CSsstroL (Const struct cs_Sstro_ *sstro,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_xy [3];

	/* Check all the points.  */

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSsstroF (sstro,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	status = CSsstroX (sstro,cnt,pnts);
**
**	struct cs_Sstro_ *sstro;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSsstroL to check lat/long
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

int EXP_LVL9 CSsstroX (Const struct cs_Sstro_ *sstro,int cnt,Const double pnts [][3])
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
		status = CSsstroI (sstro,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
