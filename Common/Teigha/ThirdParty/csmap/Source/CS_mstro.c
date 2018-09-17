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

/*lint -esym(613,err_list)  possible use of null pointerl but not really */

/**********************************************************************
**	err_cnt = CSmstroQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CSmstroQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_One;			/* 1.0 */
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */
	extern double cs_SclRedMin;		/* 0.75 */
	extern double cs_SclRedMax;		/* 1.10 */

	int ms_cnt;
	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Modified Sterographic specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}
	
	if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
	}

	/* There may be as many as 24 coeficients in the prj_prms section
	   of the cs_def structure.  However, it is not really possible
	   to determine whether they are valid or not.  The best we can
	   do is see if the absolute value of any coeficient is greater
	   than 1.0.  This would be an error for this implementation.

	   Looks like prj_prms should have been an array after all. */
													/*lint -e514 */
	ms_cnt  = (fabs (cs_def->prj_prm1) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm2) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm3) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm4) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm5) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm6) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm7) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm8) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm9) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm10) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm11) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm12) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm13) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm14) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm15) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm16) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm17) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm18) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm19) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm20) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm21) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm22) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm23) > cs_One);
	ms_cnt += (fabs (cs_def->prj_prm24) > cs_One);
														/*lint +e514 */
	if (ms_cnt > 0)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_MSCOEF;
	}

	/* That's it for Modified Sterographic. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSmstroS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSmstroS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/*    1.0 / RADIAN  */
	extern double cs_Radian;			/*   57.29....  */
	extern double cs_Pi_o_4;			/*   PI / 4.0     */
	extern double cs_Pi_o_2;			/*   PI / 2.0     */
	extern double cs_Zero;				/*   0.0 */
	extern double cs_Half;				/*   0.5 */
	extern double cs_One;				/*   1.0 */
	extern double cs_Two;				/*   2.0 */
	extern double cs_Three;				/*   3.0 */
	extern double cs_K90;				/*  90.0 */
	extern double cs_Km90;				/* -90.0 */
	extern double cs_K45;				/*  45.0 */

	short order;

	struct cs_Mstro_ *mstro;
	double *pr, *pi;

	double chi1;

	double tmp1, tmp2, tmp3;

	mstro = &csprm->proj_prms.mstro;

	/* Transfer the necessary arguments to the "mstro" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */

	mstro->org_lng = csprm->csdef.org_lng * cs_Degree;
	mstro->org_lat = csprm->csdef.org_lat * cs_Degree;
	mstro->k = csprm->csdef.scale * csprm->csdef.scl_red;
	mstro->x_off = csprm->csdef.x_off;
	mstro->y_off = csprm->csdef.y_off;
	mstro->ecent = csprm->datum.ecent;
	mstro->e_rad = csprm->datum.e_rad;
	mstro->ka = csprm->datum.e_rad * mstro->k;
	mstro->cos_org_lat = cos (mstro->org_lat);
	mstro->sin_org_lat = sin (mstro->org_lat);
	mstro->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Set up a value in the units of the cartesian system
	   which represents one millimeter. */

   	mstro->one_mm = 0.001 * mstro->k;
	if (mstro->e_rad <= cs_Three) mstro->one_mm = 2.0E-10;

	if (mstro->ecent != 0.0)
	{
		mstro->e_o_2 = csprm->datum.ecent * cs_Half;
		mstro->e_sq = mstro->ecent * mstro->ecent;

		/* Here for the ellipsoidal calculations. */

		tmp1 = mstro->ecent * mstro->sin_org_lat;
		tmp2 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp2 = pow (tmp2,mstro->e_o_2);
		tmp3 = cs_Pi_o_4 + (mstro->org_lat * cs_Half);
		tmp2 = tan (tmp3) * tmp2;
		chi1 = cs_Two * atan (tmp2) - cs_Pi_o_2;
		mstro->sin_chi1 = sin (chi1);
		mstro->cos_chi1 = cos (chi1);

		CSchiIsu (&mstro->chicofI,mstro->e_sq);
	}

	/* Determine the order of the least squares power series,
	   and populate the internal array from the parameters
	   provided.

	   Some of this is rather ugly.  However, the projection
	   parameters are not an array.  Maybe in the next life. */

	mstro->order = 0;
	for (order = 12;order > 0;order -= 1)
	{
		switch (order) {
		case 12:
			pr = &csprm->csdef.prj_prm23;
			pi = &csprm->csdef.prj_prm24;
			break;
		case 11:
			pr = &csprm->csdef.prj_prm21;
			pi = &csprm->csdef.prj_prm22;
			break;
		case 10:
			pr = &csprm->csdef.prj_prm19;
			pi = &csprm->csdef.prj_prm20;
			break;
		case  9:
			pr = &csprm->csdef.prj_prm17;
			pi = &csprm->csdef.prj_prm18;
			break;
		case  8:
			pr = &csprm->csdef.prj_prm15;
			pi = &csprm->csdef.prj_prm16;
			break;
		case  7:
			pr = &csprm->csdef.prj_prm13;
			pi = &csprm->csdef.prj_prm14;
			break;
		case  6:
			pr = &csprm->csdef.prj_prm11;
			pi = &csprm->csdef.prj_prm12;
			break;
		case  5:
			pr = &csprm->csdef.prj_prm9;
			pi = &csprm->csdef.prj_prm10;
			break;
		case  4:
			pr = &csprm->csdef.prj_prm7;
			pi = &csprm->csdef.prj_prm8;
			break;
		case  3:
			pr = &csprm->csdef.prj_prm5;
			pi = &csprm->csdef.prj_prm6;
			break;
		case  2:
			pr = &csprm->csdef.prj_prm3;
			pi = &csprm->csdef.prj_prm4;
			break;
		default:
		case  1:
			pr = &csprm->csdef.prj_prm1;
			pi = &csprm->csdef.prj_prm2;
			break;
		}

		if (mstro->order != 0)
		{
			mstro->ABary [order].real = *pr;
			mstro->ABary [order].img  = *pi;
		}
		else if (*pr != 0.0 && *pi != 0.0)
		{
			mstro->order = order;
			mstro->ABary [order].real = *pr;
			mstro->ABary [order].img  = *pi;
		}
		else
		{
			mstro->ABary [order].real = cs_Zero;
			mstro->ABary [order].img  = cs_Zero;
		}
	}
	mstro->ABary [0].real = cs_Zero;
	mstro->ABary [0].img  = cs_Zero;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = mstro->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */

		csprm->min_ll [LNG] = cs_Km90;
		csprm->max_ll [LNG] = cs_K90;
		tmp1 = mstro->org_lat * cs_Radian;
		csprm->min_ll [LAT] = tmp1 - cs_K45;
		csprm->max_ll [LAT] = tmp1 + cs_K45;
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

		csprm->min_xy [XX] = -(cs_Pi_o_2 * mstro->ka);
		csprm->max_xy [XX] =  (cs_Pi_o_2 * mstro->ka);
		csprm->min_xy [YY] = -(cs_Pi_o_2 * mstro->ka);
		csprm->max_xy [YY] =  (cs_Pi_o_2 * mstro->ka);

		CS_quadMM (csprm->min_xy,csprm->max_xy,mstro->x_off,
											   mstro->y_off,
											   mstro->quad);
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
	   addresses and we are done.  Note, this is a conformal
	   projection, there is no H function. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CSmstroF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSmstroI;
	csprm->cs_scale = (cs_SCALE_CAST)CSmstroK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSmstroK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSmstroK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSmstroC;
	csprm->llchk    = (cs_LLCHK_CAST)CSmstroL;
	csprm->xychk    = (cs_XYCHK_CAST)CSmstroX;

	return;
}

/**********************************************************************
**	rtn_val = CSmstroF (mstro,xy,ll);
**
**	struct cs_Mstro_ *mstro;	structure which defines the parameters
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
**	This function calculates the Modified Sterographic
**	projection and returns values in the coordinate system
**	described by the cs_Mstro_ structure.  If the ecentricity
**	of the datum is zero, the formulas for the earth as
**	a sphere are used.
**
**	All the formulas used here were extracted from the
**	USGS publication "Map Projections - A Working Manual"
**	U. S. Geological Survey Professional Paper 1395, by
**	John P. Synder, pages 203-211.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSmstroF (Const struct cs_Mstro_ *mstro,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / RADIAN  */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Pi;				/* 3.14159.... */
	extern double cs_Mpi;				/* -3.14159.... */
	extern double cs_Two_pi;			/* 2 PI */
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
	double chi;			/* Isometric form of the latitude. */

	double x_prime, y_prime;
	double k_prime;

	double del_lng;
	double sin_del_lng, cos_del_lng;
	double sin_lat, cos_lat;
	double sin_chi, cos_chi;
	double ss;

	double tmp_chi;
	double tmp1;
	double tmp2;
	double tmp3;

	struct cs_Cmplx_ cmplx_xy;

	rtn_val = cs_CNVRT_NRML;

	/* There are two formulae, one for the sphere and one for
	   the ellipsoid.  If the ecentricity of the datum in use
	   is 0.0 exactly, we shall use the spherical formulae.
	   There is a certain amount of stuff which is common to
	   both which we perform first. */

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
	del_lng = lng - mstro->org_lng;
	if      (del_lng > cs_Pi  && mstro->org_lng < 0.0) del_lng -= cs_Two_pi;
	else if (del_lng < cs_Mpi && mstro->org_lng > 0.0) del_lng += cs_Two_pi;
	if (fabs (del_lng) > cs_Pi)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = CS_adj2pi (del_lng);
	}

	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* See if the ellipsod is a sphere or an ellipsoid. */

	if (mstro->ecent == 0.0)
	{
		/* Here for the sphere.  First, we need k prime. */

		tmp1 = cos_lat * cos_del_lng;
		tmp2 = cs_One + (mstro->sin_org_lat * sin_lat);
		tmp2 += mstro->cos_org_lat * tmp1;
		if (tmp2 < cs_AnglTest)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp2 = cs_AnglTest;
		}
		k_prime = cs_Two / tmp2;

		/* X' and Y' */

		x_prime = k_prime * cos_lat * sin_del_lng;
		y_prime = k_prime * ((mstro->cos_org_lat * sin_lat) -
							 (mstro->sin_org_lat * tmp1));
	}
	else
	{
		/* Here for the ellipsoid.  First we need the isometric
		   equivalent of the geographic latitude. */

		tmp1 = mstro->ecent * sin_lat;
		tmp2 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp2 = pow (tmp2,mstro->e_o_2);
		tmp3 = lat * cs_Half;
		tmp_chi = tan (cs_Pi_o_4 + tmp3) * tmp2;
		chi = cs_Two * atan (tmp_chi) - cs_Pi_o_2;

		sin_chi = sin (chi);
		cos_chi = cos (chi);

		tmp1 = cos_chi * cos_del_lng;
		tmp2 = cs_One + (mstro->sin_chi1 * sin_chi) +
						(mstro->cos_chi1 * tmp1);
		if (tmp2 < cs_AnglTest)
		{
			rtn_val = cs_CNVRT_RNG;
			tmp2 = cs_AnglTest;
		}
		ss   = cs_Two / tmp2;

		x_prime = ss * cos_chi * sin_del_lng;
		tmp2    = (mstro->cos_chi1 * sin_chi) -
				  (mstro->sin_chi1 * tmp1);
		y_prime = ss * tmp2;
	}

	/* Once x_prime and y_prime have been determined, the remainder
	   is the same for sphere and ellipsoid.  Of course, the
	   coefficients are different between shpere and ellipsoid.
	   This could be a problem. */

	cmplx_xy.real = x_prime;
	cmplx_xy.img  = y_prime;
	CS_iisrs (&cmplx_xy,mstro->ABary,mstro->order,&cmplx_xy);
	xy [XX] = mstro->ka * cmplx_xy.real;
	xy [YY] = mstro->ka * cmplx_xy.img;

	if (mstro->quad == 0)
	{
		xy [XX] += mstro->x_off;
		xy [YY] += mstro->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],mstro->x_off,mstro->y_off,
												  mstro->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSmstroI (mstro,ll,xy);
**
**	struct cs_Mstro_ *mstro;	structure which defines the parameters
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
**	John P. Synder, pages 203-211.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSmstroI (Const struct cs_Mstro_ *mstro,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_AnglTest;			/* 4.85E-08 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;
	int iter_cnt;

	double x,x_prime;
	double y,y_prime;

	double c;
	double chi;
	double rho;
	double lat;
	double sin_c;
	double cos_c;
	double xy_lmit;
	double abs_xy;
	double del_lng;

	double tmp1;
	double tmp2;

	struct cs_Cmplx_ cmplx_xy;
	struct cs_Cmplx_ xy_prime;
	struct cs_Cmplx_ xy_1st;
	struct cs_Cmplx_ ff_xy;

	rtn_val = cs_CNVRT_NRML;

	if (mstro->quad == 0)
	{
		x = xy [XX] - mstro->x_off;
		y = xy [YY] - mstro->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,mstro->x_off,mstro->y_off,mstro->quad);
	}

	/* If x and y are now both zero, or very close to it,
	   we must just set the result to the origin of the
	   projection.  We don't want to divide by rho if it's
	   zero. */

	rho = sqrt (x * x + y * y);
	if (rho < mstro->one_mm)
	{
		/* The coordinate is essentially zero.  Return
		   the origin of the coordinate system. */

		ll [LNG] = mstro->org_lng * cs_Radian;
		ll [LAT] = mstro->org_lat * cs_Radian;
		return (rtn_val);
	}
	if (rho > mstro->ka)
	{
		/* Opps!!! XY is outside of the bounding circle.
		   Bring back to the edge of the circle. */

		rtn_val = cs_CNVRT_RNG;
		tmp1 = mstro->ka / rho;
		x *= tmp1;
		y *= tmp1;
	}

	/* We need to compute x_prime and y_prime.  This is the
	   same for sphere and ellipsoid.  First we get some good
	   approximations. */

	cmplx_xy.real = xy_prime.real = x / mstro->ka;
	cmplx_xy.img  = xy_prime.img  = y / mstro->ka;

	/* Now we iterate until the change is less than one_mm.
	   Since we are dealing with normalized coordinates here,
	   we need to similarly adjust the value used to test
	   for convergence.  We choose the larget value which
	   will provide 12 digits of precision.  This usually
	   requires about three iterations of the following
	   Newton/Rhapson iteration. */

	iter_cnt = 0;
	xy_lmit = 1.0E-12;
	for (;;)
	{
		iter_cnt += 1;

		CS_iisrs (&xy_prime,mstro->ABary,mstro->order,&ff_xy);
		CS_iisub (&ff_xy,&cmplx_xy,&ff_xy);

		abs_xy = CS_iiabs (&ff_xy);
		if (abs_xy < xy_lmit) break;

		/* Eliminate the possibility of an infinite loop. */

		if (iter_cnt > 6) break;

		CS_iisrs1 (&xy_prime,mstro->ABary,mstro->order,&xy_1st);
		CS_iidiv  (&ff_xy,&xy_1st,&ff_xy);
		CS_iisub (&xy_prime,&ff_xy,&xy_prime);
	}

	/* xy_prime now contains the values which will produce x and y
	   using the forward formula.  Now we can convert back to
	   lat/longs. */

	x_prime = xy_prime.real;
	y_prime = xy_prime.img;

	/* Could this rho be zero? */

	rho = sqrt (x_prime * x_prime + y_prime * y_prime);

	del_lng = cs_Zero;
	if (rho < cs_AnglTest)
	{
		lat = mstro->org_lng;
	}
	else
	{
		c = cs_Two * atan (rho * cs_Half);
		sin_c = sin (c);
		cos_c = cos (c);

		if (mstro->ecent == 0.0)
		{
			/* Here for the sphere.  Note, we have already
			   filtered out cases where rho is zero (or very
			   close to zero). */

			tmp1 = x_prime * sin_c;
			tmp2 = (rho * mstro->cos_org_lat * cos_c) -
				   (y_prime * mstro->sin_org_lat * sin_c);

			/* It is, remotely, possible for both tmp1 and tmp2
			   to be zero. */

			if (fabs (x_prime) > mstro->one_mm)
			{
				del_lng = atan2 (tmp1,tmp2);
			}

			tmp1 = cos_c * mstro->sin_org_lat;
			tmp2 = y_prime * sin_c * mstro->cos_org_lat / rho;
			lat = asin (tmp1 + tmp2);
		}
		else
		{
			/* Here for the ellisoid. */

			tmp1 = x_prime * sin_c;
			tmp2 = rho * mstro->cos_chi1 * cos_c;
			tmp2 -= y_prime * mstro->sin_chi1 * sin_c;

			/* There is a specific singularity point, for
			   apecific coordinate systems only, in which
			   both tmp1 and tmp2 can be zero at the same
			   time. */

			if (fabs (x) > mstro->one_mm)
			{
				del_lng = atan2 (tmp1,tmp2);
			}

			tmp1 = cos_c * mstro->sin_chi1;
			tmp2 = (y_prime * sin_c * mstro->cos_chi1) / rho;
			chi = asin (tmp1 + tmp2);
			lat = CSchiIcal (&mstro->chicofI,chi);
		}
	}
	if (fabs (lat) > cs_NPTest && rtn_val == cs_CNVRT_NRML)
	{
		rtn_val = cs_CNVRT_INDF;
	}

	/* Convert the results to degrees. */

	ll [LNG] = (del_lng + mstro->org_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CSmstroC (mstro,ll);
**
**	struct cs_Mstro_ *mstro;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Modified Sterographic
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

double EXP_LVL9 CSmstroC (Const struct cs_Mstro_ *mstro,Const double ll [2])
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
	status = CSmstroF (mstro,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSmstroF (mstro,xy2,my_ll);
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
**	kk = CSmstroK (mstro,ll);
**
**	struct cs_Mstro_ *mstro;	structure which carries all parameters
**								in effect for the coordinate system being used,
**								assuming the Modified Sterographic projection.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the grid scale of the coordinate
**								system at the specified point.
**
**	The Modified Sterographic projection is conformal, therefore,
**	the grid scale factor returned by this function is valid in
**	all directions.  Thus, there is no CSmstroH function.
**********************************************************************/

double EXP_LVL9 CSmstroK (Const struct cs_Mstro_ *mstro,Const double ll [2])
{
	extern double cs_Pi_o_2;			/* PI / 2.0 */
	extern double cs_Pi_o_4;			/* PI / 4.0 */
	extern double cs_Degree;			/*   1.0 / 57.29577... */
	extern double cs_Half;				/*   0.5 */
	extern double cs_One;				/*   1.0 */
	extern double cs_Mone;				/*  -1.0 */
	extern double cs_Two;				/*   2.0 */
	extern double cs_AnglTest;			/* 0.001 seconds of arc,
										   in radians. */

	double kk;
	double lng;
	double lat;

	double x_prime, y_prime;
	double k_prime;
	double mm, ss;
	double chi;

	double del_lng;
	double sin_lat, cos_lat;
	double sin_chi, cos_chi;
	double cos_del_lng, sin_del_lng;

	double tmp_chi;
	double tmp1, tmp2, tmp3;

	struct cs_Cmplx_ xy_prime;

	/* This is common stuff. */

	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_Pi_o_2)
	{
		return (cs_Mone);
	}

	lng = ll [LNG] * cs_Degree;
	del_lng = CS_adj2pi (lng - mstro->org_lng);

	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);
	sin_lat = sin (lat);
	cos_lat = cos (lat);

	/* See if the ellipsod is a sphere or an ellipsoid. */

	if (mstro->ecent == 0.0)
	{
		/* Here for the sphere. */

		tmp1 = cos_lat * cos_del_lng;
		tmp2 = cs_One + (mstro->sin_org_lat * sin_lat);
		tmp2 += mstro->cos_org_lat * tmp1;
		if (tmp2 < cs_AnglTest)
		{
			return (cs_Mone);
		}
		k_prime = cs_Two / tmp2;

		/* X' and Y' */

		x_prime = k_prime * cos_lat * sin_del_lng;
		y_prime = k_prime * ((mstro->cos_org_lat * sin_lat) -
							 (mstro->sin_org_lat * tmp1));
	}
	else
	{
		/* Here for the ellipsoid.  First we need the isometric
		   equivalent of the geographic latitude. */

		tmp1 = mstro->ecent * sin_lat;
		tmp2 = (cs_One - tmp1) / (cs_One + tmp1);
		tmp2 = pow (tmp2,mstro->e_o_2);
		tmp3 = lat * cs_Half;
		tmp_chi = tan (cs_Pi_o_4 + tmp3) * tmp2;
		chi = cs_Two * atan (tmp_chi) - cs_Pi_o_2;

		sin_chi = sin (chi);
		cos_chi = cos (chi);

		tmp1 = cs_One - (mstro->e_sq * sin_lat * sin_lat);
		mm = cos_lat / sqrt (tmp1);

		tmp1 = cos_chi * cos_del_lng;
		tmp2 = cs_One + (mstro->sin_chi1 * sin_chi) +
						(mstro->cos_chi1 * tmp1);
		if (tmp2 < cs_AnglTest)
		{
			return (cs_Mone);
		}
		ss   = cs_Two / tmp2;

		/* Opps!!! mm is zero at the poles; so is cos_chi.
		   I assume that these guys will cancel each other. */

		if (mm > cs_AnglTest)
		{
			k_prime = ss * cos_chi / mm;
		}
		else
		{
			k_prime = ss;
		}

		x_prime = ss * cos_chi * sin_del_lng;
		tmp2    = (mstro->cos_chi1 * sin_chi) -
				  (mstro->sin_chi1 * tmp1);
		y_prime = ss * tmp2;
	}

	if (fabs (x_prime) > cs_One || fabs (y_prime) > cs_One)
	{
		kk = cs_Mone;
	}
	else
	{
		xy_prime.real = x_prime;
		xy_prime.img  = y_prime;
		CS_iisrs1 (&xy_prime,mstro->ABary,mstro->order,&xy_prime);
		kk = k_prime * CS_iiabs (&xy_prime);
	}
	return (kk);
}

/**********************************************************************
**	status = CSmstroL (mstro,cnt,pnts);
**
**	struct cs_Mstro_ *mstro;	the coordinate system against which the check is
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

int EXP_LVL9 CSmstroL (Const struct cs_Mstro_ *mstro,int cnt,Const double pnts [][3])
{
	int ii;
	int status;

	double test_xy [3];

	/* Check all the points.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSmstroF (mstro,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CSmstroX (mstro,cnt,pnts);
**
**	struct cs_Mstro_ *mstro;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSmstroL to check lat/long
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

int EXP_LVL9 CSmstroX (Const struct cs_Mstro_ *mstro,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_ll [3];

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic
	   range.  No special checks are required for lines and/or
	   regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		status = CSmstroI (mstro,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
