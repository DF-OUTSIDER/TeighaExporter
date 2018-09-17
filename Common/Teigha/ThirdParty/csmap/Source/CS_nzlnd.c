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
**	err_cnt = CSnzlndQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	currently unused.
**	int err_list [];			an array of integers in which error codes are
**								returned if not NULL.
**	int list_sz;t				he size of the array pointed to be err_list,
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

int EXP_LVL9 CSnzlndQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */
	extern double cs_SclRedMin;		/* 0.75 */
	extern double cs_SclRedMax;		/* 1.10 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */

	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the New Zealand National Grid specific stuff. */
	
	if (cs_def->org_lng <= cs_MinLng || cs_def->org_lng > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	
	if (cs_def->org_lat <= cs_MinLat || cs_def->org_lat >= cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}
	
	if (cs_def->scl_red < cs_SclRedMin || cs_def->scl_red > cs_SclRedMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_SCLRED;
	}

	/* That's it for New Zealand National Grid. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	CSnzlndS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure which defines the parameters
**								in effect for the transformation.
**********************************************************************/

void EXP_LVL9 CSnzlndS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/* 57.29....  */
	extern double cs_Eight;				/* 1.0 / 8.0 */
	extern double cs_K15;				/* 15.0 */

	struct cs_Nzlnd_ *nzlnd;

	nzlnd = &csprm->proj_prms.nzlnd;

	/* Transfer the necessary arguments to the "nzlnd" structure.
	   Notice, the conversion from degrees to radians which is
	   performed in the process. */

	/* NOTE: in this system, we work with latitudes in
	   degrees, (actually, seconds times 1.0E-05).
	   Longitudes are in radians.  However, since there
	   is not a single trigonometric function call
	   in the whole thing, we will use degrees in this
	   setup function. */

	nzlnd->org_lng = csprm->csdef.org_lng;
	nzlnd->org_lat = csprm->csdef.org_lat;
	nzlnd->k = csprm->csdef.scale * csprm->csdef.scl_red;
	nzlnd->x_off = csprm->csdef.x_off;
	nzlnd->y_off = csprm->csdef.y_off;
	nzlnd->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* The ellipsoid is the International by default.  We
	   don't even use the eccentricity.  We keep these
	   around for the empirical scale and convergence
	   calculations. */

	nzlnd->ecent = csprm->datum.ecent;
	nzlnd->e_rad = csprm->datum.e_rad;
	nzlnd->e_sq = nzlnd->ecent * nzlnd->ecent;
	nzlnd->ka = csprm->datum.e_rad * nzlnd->k;

	/* Set up a value in the units of the cartesian system
	   which represents one tenth of a millimeter. */

	nzlnd->one_mm = 0.0001 * nzlnd->k;

	/* The delta latitude normalization constant. */

	nzlnd->lat_kk = (3600.0 * 1.0E-05);

	/* The series which have been developed are hard coded
	   for the International ellipsoid.  Therefore there
	   is only one set of equations.

	   Stuff the series coefficients here.  This enables us
	   to use our complex Horner expansion code. */

	nzlnd->A_ary [0] =  0.6399175073;
	nzlnd->A_ary [1] = -0.1358797613;
	nzlnd->A_ary [2] =  0.063294409;
	nzlnd->A_ary [3] = -0.02526853;
	nzlnd->A_ary [4] =  0.0117879;
	nzlnd->A_ary [5] = -0.0055161;
	nzlnd->A_ary [6] =  0.0026906;
	nzlnd->A_ary [7] = -0.001333;
	nzlnd->A_ary [8] =  0.00067;
	nzlnd->A_ary [9] = -0.00034;

	nzlnd->B_ary [0].real =  0.0;		nzlnd->B_ary [0].img  =  0.0;
	nzlnd->B_ary [1].real =  0.7557853228;	nzlnd->B_ary [1].img  =  0.0;
	nzlnd->B_ary [2].real =  0.249204646;	nzlnd->B_ary [2].img  =  0.003371507;
	nzlnd->B_ary [3].real = -0.001541739;	nzlnd->B_ary [3].img  =  0.041058560;
	nzlnd->B_ary [4].real = -0.10162907;	nzlnd->B_ary [4].img  =  0.01727609;
	nzlnd->B_ary [5].real = -0.26623489;	nzlnd->B_ary [5].img  = -0.36249218;
	nzlnd->B_ary [6].real = -0.6870983;	nzlnd->B_ary [6].img  = -1.1651967;

	nzlnd->C_ary [0].real =  0.0;		nzlnd->C_ary [0].img  =  0.0;
	nzlnd->C_ary [1].real =  1.3231270439;	nzlnd->C_ary [1].img  =  0.0;
	nzlnd->C_ary [2].real = -0.577245789;	nzlnd->C_ary [2].img  = -0.007809598;
	nzlnd->C_ary [3].real =  0.508307513;	nzlnd->C_ary [3].img  = -0.112208952;
	nzlnd->C_ary [4].real = -0.15094762;	nzlnd->C_ary [4].img  =  0.18200602;
	nzlnd->C_ary [5].real =  1.01418179;	nzlnd->C_ary [5].img  =  1.64497696;
	nzlnd->C_ary [6].real =  1.9660549;	nzlnd->C_ary [6].img  =  2.5127645;

	nzlnd->D_ary [0] =  1.5627014243;
	nzlnd->D_ary [1] =  0.5185406398;
	nzlnd->D_ary [2] = -0.03333098;
	nzlnd->D_ary [3] = -0.1052906;
	nzlnd->D_ary [4] = -0.0368594;
	nzlnd->D_ary [5] =  0.007317;
	nzlnd->D_ary [6] =  0.01220;
	nzlnd->D_ary [7] =  0.00394;
	nzlnd->D_ary [8] = -0.0013;

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = nzlnd->org_lng;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
		csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range.  We'll establish some pretty liberal
		   values. */

		csprm->min_ll [LNG] = -cs_K15;
		csprm->max_ll [LNG] = cs_K15;
		csprm->min_ll [LAT] = nzlnd->org_lat * cs_Radian - cs_K15;
		csprm->max_ll [LAT] = nzlnd->org_lat * cs_Radian + cs_K15;
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
		   The setup is virtually complete, so we can use CSnzlndF
		   to calculate some values as necessary. Unfortuneately
		   it is the rare case where we can just convert the
		   lat/long min/max. */

		csprm->min_xy [XX] = -cs_Eight * nzlnd->ka;
		csprm->max_xy [XX] =  cs_Eight * nzlnd->ka;
		csprm->min_xy [YY] = -cs_Eight * nzlnd->ka;
		csprm->max_xy [YY] =  cs_Eight * nzlnd->ka;

		CS_quadMM (csprm->min_xy,csprm->max_xy,nzlnd->x_off,
											   nzlnd->y_off,
											   nzlnd->quad);
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

	csprm->ll2cs    = (cs_LL2CS_CAST)CSnzlndF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CSnzlndI;
	csprm->cs_scale = (cs_SCALE_CAST)CSnzlndK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CSnzlndK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CSnzlndK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CSnzlndC;
	csprm->llchk    = (cs_LLCHK_CAST)CSnzlndL;
	csprm->xychk    = (cs_XYCHK_CAST)CSnzlndX;

	return;
}

/**********************************************************************
**	rtn_val = CSnzlndF (nzlnd,xy,ll);
**
**	struct cs_Nzlnd_ *nzlnd;	structure which defines the parameters
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
**	This function calculates the New Zealand Grid System
**	forward equations and returns values in the coordinate system
**	described by the cs_Nzlnd_ structure.  This system is based
**	on a series expansion which is, in turn, based on the
**	International Ellipsoid.  What you get if a different
**	ellipsoid is given is undetermined.
**
**	All the formulas used here were extracted from formulas
**	provided by serveral sources, all of which are in agreement. 
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSnzlndF (Const struct cs_Nzlnd_ *nzlnd,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/*  1.0 / RADIAN  */
	extern double cs_Zero;				/*  0.0 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Eight;				/*  8.0 */
	extern double cs_K180;				/*  180.0 */
	extern double cs_Km180;				/* -180.0 */
	extern double cs_K360;				/*  360.0 */

	int ii;
	int rtn_val;

	double del_lng;
	double del_lat;
	double del_psi;
	struct cs_Cmplx_ theta;

	rtn_val = cs_CNVRT_NRML;

	/* Note, the following look different than the sources as
	   the sources assume south latitude to be positive.  Our
	   convention is that south latitude is negative. */

	del_lng = (ll [LNG] - nzlnd->org_lng);
	if      (del_lng > cs_K180  && nzlnd->org_lng < 0.0) del_lng -= cs_K360;
	else if (del_lng < cs_Km180 && nzlnd->org_lng > 0.0) del_lng += cs_K360;
	if (fabs (del_lng) > cs_Eight)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = (del_lng > 0.0) ? cs_Eight : -cs_Eight;
	}
	del_lng *= cs_Degree;

	del_lat = (ll [LAT] - nzlnd->org_lat);
	if (fabs (del_lat) > cs_Eight)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lat = (del_lat > 0.0) ? cs_Eight : -cs_Eight;
	}
	del_lat *= nzlnd->lat_kk;

	del_psi = cs_Zero;
	for (ii = 9;ii >= 0;ii--)
	{
		del_psi = (del_psi + nzlnd->A_ary [ii]) * del_lat;
	}

	if (fabs (del_lng) > cs_One)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = (del_lng > 0.0) ? cs_One : cs_Mone;
	}
	if (fabs (del_psi) > cs_One)
	{
		rtn_val = cs_CNVRT_RNG;
		del_psi = (del_psi > 0.0) ? cs_One : cs_Mone;
	}

	theta.real = del_psi;
	theta.img  = del_lng;
	CS_iisrs (&theta,nzlnd->B_ary,6,&theta);

	xy [XX] = (theta.img  * nzlnd->ka);
	xy [YY] = (theta.real * nzlnd->ka);

	if (nzlnd->quad == 0)
	{
		xy [XX] += nzlnd->x_off;
		xy [YY] += nzlnd->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],nzlnd->x_off,nzlnd->y_off,
												  nzlnd->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CSnzlndI (nzlnd,ll,xy);
**
**	struct cs_Nzlnd_ *nzlnd;	structure which defines the parameters
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
**	Formulas and constants used here were taken from several
**	unattributed sources, all of which were in agreement.
**
**	In the arrays of doubles, x and longitude values always
**	occur first.  Negative values indicate values West or
**	South of the origin.
**********************************************************************/

int EXP_LVL9 CSnzlndI (Const struct cs_Nzlnd_ *nzlnd,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/*  57.29577..... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_K90;				/* 90.0 */
	
	int ii;
	int rtn_val;

	double xx;
	double yy;
	double rho;
	double lat;
	double del_psi,del_lat;

	struct cs_Cmplx_ zz;
	struct cs_Cmplx_ theta;
	struct cs_Cmplx_ theta1;
	struct cs_Cmplx_ theta2;

	rtn_val = cs_CNVRT_NRML;

	if (nzlnd->quad == 0)
	{
		xx = xy [XX] - nzlnd->x_off;
		yy = xy [YY] - nzlnd->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,nzlnd->x_off,nzlnd->y_off,nzlnd->quad);
	}
	xx /= nzlnd->ka;
	yy /= nzlnd->ka;

	if (fabs (xx) > cs_One || fabs (yy) > cs_One)
	{
		rtn_val = cs_CNVRT_RNG;
		rho = sqrt (xx * xx + yy * yy);

		xx /= rho;
		yy /= rho;
	}

	zz.real = yy;
	zz.img  = xx;
	CS_iisrs (&zz,nzlnd->C_ary,6,&theta);

	for (ii = 0;ii < 2;ii++)
	{
		CS_iisrs1 (&theta,nzlnd->B_ary,6,&theta1);
		CS_iisrs0 (&theta,nzlnd->B_ary,6,&theta2);
		CS_iiadd  (&zz,&theta2,&theta2);
		CS_iidiv  (&theta2,&theta1,&theta);
	}

	del_psi = theta.real;
	del_lat = cs_Zero;
	for (ii = 8;ii >= 0;ii--)
	{
		del_lat = (del_lat + nzlnd->D_ary [ii]) * del_psi;
	}

	ll [LNG] = CS_adj180 (theta.img * cs_Radian) + nzlnd->org_lng;
	lat = CS_adj90  (del_lat / nzlnd->lat_kk) + nzlnd->org_lat;
	if (fabs (lat) > cs_K90)
	{
		rtn_val = cs_CNVRT_RNG;
		lat = CS_adj90 (lat);
	}
	ll [LAT] = lat;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CSnzlndC (nzlnd,ll);
**
**	struct cs_Nzlnd_ *nzlnd;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the New Zealand National
**								Grid coordinate system.
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

double EXP_LVL9 CSnzlndC (Const struct cs_Nzlnd_ *nzlnd,Const double ll [2])
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
	status = CSnzlndF (nzlnd,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CSnzlndF (nzlnd,xy2,my_ll);
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
**	kk = CSnzlndK (nzlnd,ll);
**
**	struct cs_Nzlnd_ *nzlnd;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the New Zealand
**								National Grid system.
**	double ll [2];				the longitude ([0]) and the latitude ([0])
**								of the point at which the true scale of
**								the coordinate system is to be computed.
**								Values are in degrees.
**	double kk;					returns the actual scale factor, along a
**								parallel, of the projected coordinate system
**								at the specified point.
**
**	We have not as yet located any specific information on
**	analytical formulas for determining the K scale factor
**	for this projection.  Therefore, we simply sample the
**	projection at two points which are close to each other
**	and return the ratio of geodetic and grid distance.
**********************************************************************/

double EXP_LVL9 CSnzlndK (Const struct cs_Nzlnd_ *nzlnd,Const double ll [2])
{
	extern double cs_Mone;			/* -1.0, the value we return
									   when presented with bogus
									   lat/long coordinates. */
	int status;

	double kk;
	double ll_dd;
	double xy_dd;
	double del_xx, del_yy;

	double xy1 [2];
	double xy2 [2];
	double ll1 [2];
	double ll2 [2];

	/* Establish two points along the parallel which are
	   about .5 seconds (about 15 meters) apart from each
	   other, with the point in question in the middle.
	   Then convert each point to the equivalent grid
	   coordinates. */

	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] - (0.5 / 3600.0);
	status = CSnzlndF (nzlnd,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + (0.5 / 3600.0);
	status = CSnzlndF (nzlnd,xy2,ll2);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	(void)CS_llazdd (nzlnd->ka,nzlnd->e_sq,ll1,ll2,&ll_dd);

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the parallel
	   scale factor at the indicated point. */

	kk = xy_dd / ll_dd;
	return (kk);
}

/**********************************************************************
**	status = CSnzlndL (nzlnd,cnt,pnts);
**
**	struct cs_Nzlnd_ *nzlnd;	the coordinate system against which the check is
**			to be performed.
**	int cnt;					the number of points in the region to be
**			checked.
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

int EXP_LVL9 CSnzlndL (Const struct cs_Nzlnd_ *nzlnd,int cnt,Const double pnts [][3])
{
	int ii;
	int status;
	int rtn_val;

	double test_xy [3];

	/* Check all the points.  */

	rtn_val = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		status = CSnzlndF (nzlnd,test_xy,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}

/**********************************************************************
**	status = CSnzlndX (nzlnd,cnt,pnts);
**
**	struct cs_Nzlnd_ *nzlnd;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CSnzlndL to check lat/long
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
**
**********************************************************************/

int EXP_LVL9 CSnzlndX (Const struct cs_Nzlnd_ *nzlnd,int cnt,Const double pnts [][3])
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
		status = CSnzlndI (nzlnd,test_ll,pnts [ii]);
		if (status != cs_CNVRT_NRML)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
