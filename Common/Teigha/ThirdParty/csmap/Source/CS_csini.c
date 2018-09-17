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
/*lint -esym(534,CScsiniF) ignoring return value */

/**********************************************************************
**	err_cnt = CScsiniQ (cs_def,prj_code,err_list,list_sz);
**
**	struct cs_Csdef_ *cs_def;	the coordinate system to be checked.
**	unsigned short prj_code;	currently unused.
**	int err_list [];			an array of integers in which error
**								codes are returned if not NULL.
**	int list_sz;				the size of the array pointed to be
**								err_list, may be zero.
**	int err_cnt;				returns the number of errors detected.
**
**	Set err_list to NULL, and/or list_sz to zero, to simply get
**	a yea or nay status of the definition (i.e. err_cnt != 0).
**
**	All lat/longs in definitions must be referenced to Greennwich,
**	and in the range of greater than -180/-90, and less than or
**	equal to +180/+90.
**********************************************************************/

int EXP_LVL9 CScsiniQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the Cassini specific stuff. */
	
	if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLNG;
	}
	
	if (cs_def->org_lat < cs_MinLat || cs_def->org_lat > cs_MaxLat)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_ORGLAT;
	}
	
	/* That's it for Cassini. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CScsiniS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate
**								system parameters.
**
**	Normal conventions and CS_MAP standards have NOT been followed
**	in codng this function so that the variable nomenclature could
**	adhere as closely as possible to that used in the reference.
**********************************************************************/

void EXP_LVL9 CScsiniS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577.. */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_Mpi_o_2;			/* -Pi over 2 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_K75;				/* 75.0 */

	struct cs_Csini_ *csini;

	double tmp1;
	double sin_org_lat;
	double cos_org_lat;

	double test_xy [3];
	double test_ll [3];

	csini = &csprm->proj_prms.csini;

	csini->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
	csini->org_lat = csprm->csdef.org_lat * cs_Degree;
	csini->x_off = csprm->csdef.x_off;
	csini->y_off = csprm->csdef.y_off;
	csini->k = csprm->csdef.scale;
	csini->ecent = csprm->datum.ecent;
	csini->e_sq = csini->ecent * csini->ecent;
	csini->e_rad = csprm->datum.e_rad;
	csini->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Probably could add a scale reduction factor to the
	   following term. */

	csini->ka = csini->e_rad * csini->k;
	csini->max_xx = csini->ka * cs_Pi_o_2;
	csini->max_yy = csini->ka * (cs_Pi_o_2 - csini->org_lat);
	csini->min_yy = csini->ka * (cs_Mpi_o_2 - csini->org_lat);

	/* Nothing special required for the sphere. */

	if (csini->ecent != 0.0)
	{
		/* Here for a ellipsoid datum.  We compute
		   what we can once only. */

		tmp1 = cs_One - csini->e_sq;
		csini->C_term = csini->e_sq / tmp1;
		csini->R_term = csini->ka * tmp1;
		csini->s_term = cs_Two * csini->ka * csini->ka * tmp1;

		/* CSmuFsu and CSmuIsu do the rest. */

		CSmmFsu (&csini->mmcofF,csini->ka,csini->e_sq);
		CSmmIsu (&csini->mmcofI,csini->ka,csini->e_sq);

		/* Now we compute M0, the M associated with
		   the origin latiude. */

		sin_org_lat = sin (csini->org_lat);
		cos_org_lat = cos (csini->org_lat);
		csini->M0 = CSmmFcal (&csini->mmcofF,csini->org_lat,
										     sin_org_lat,
										     cos_org_lat);

		csini->max_yy = CSmmFcal (&csini->mmcofF,cs_Pi_o_2,
												 cs_One,
												 cs_Zero);
		csini->max_yy -= csini->M0;
		csini->min_yy = CSmmFcal (&csini->mmcofF,cs_Mpi_o_2,
												 cs_One,
												 cs_Zero);
		csini->min_yy -= csini->M0;
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = csini->cent_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* User hasn't specified any values.  We're to establish
		   the useful range. */

		csprm->min_ll [LNG] = -cs_Five;
		csprm->max_ll [LNG] = cs_Five;
		csprm->min_ll [LAT] = -cs_K75;
		csprm->max_ll [LAT] = cs_K75;
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
		   The setup is virtually complete, so we can use CScsiniF
		   to calculate some values as necessary. Unfortuneately
		   it is the rare case where we can just convert the
		   lat/long min/max. */

		test_ll [LNG] = CS_adj180 (csprm->min_ll [LNG] + csprm->cent_mer);
		test_ll [LAT] = csini->org_lat * cs_Radian;
		CScsiniF (csini,test_xy,test_ll);
		csprm->min_xy [XX] = test_xy [XX] - csini->x_off;
		csprm->max_xy [XX] = -csprm->min_xy [XX];

		test_ll [LNG] = csini->cent_lng * cs_Radian;
		test_ll [LAT] = cs_K75;
		CScsiniF (csini,test_xy,test_ll);
		csprm->max_xy [YY] = test_xy [YY] - csini->y_off;

		test_ll [LAT] = -cs_K75;
		CScsiniF (csini,test_xy,test_ll);
		csprm->min_xy [YY] = test_xy [YY] - csini->y_off;

		CS_quadMM (csprm->min_xy,csprm->max_xy,csini->x_off,
						       csini->y_off,
						       csini->quad);
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

	/* That's all the calculations we can do at this time. Set
	   up the internal function calls.  Note, the Cassini is
	   not a conformal projection. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CScsiniF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CScsiniI;
	csprm->cs_scale = (cs_SCALE_CAST)CScsiniH;
	csprm->cs_sclk  = (cs_SCALK_CAST)CScsiniK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CScsiniH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CScsiniC;
	csprm->llchk    = (cs_LLCHK_CAST)CScsiniL;
	csprm->xychk    = (cs_XYCHK_CAST)CScsiniX;

	return;
}

/**********************************************************************
**	rtn_val = CScsiniF (csini,xy,ll);
**
**	struct cs_Csini_ *csini;	structure containing all parameters
**								necessary for the transformation.
**	double xy [2];				converted values are returned here,
**								x ([0]) and y ([1]).
**	double ll [2];				the longitude ([0]) and latitude ([1])
**								values to be converted, in degrees.
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The ll and the xy arrays may share the same memory
**	with no adverse affects.
**********************************************************************/

int EXP_LVL9 CScsiniF (Const struct cs_Csini_ *csini,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_Six;				/* 6.0 */
	extern double cs_Eight;				/* 8.0 */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_EETest;			/* 0.001 arc seconds short
										   of 90 degrees, in
										   readians. */
	extern double cs_WETest;			/* 0.001 arc seconds short
										   of -90 degrees, in
										   readians. */

	int rtn_val;

	double lng;
	double lat;
	double del_lng;

	double A;
	double A_2;
	double A_3;
	double A_4;
	double A_5;
	double B;
	double C;
	double M;
	double N;
	double T;

	double sin_lat;
	double cos_lat;
	double tan_lat;

	double tmp1;
	double tmp2;

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

	del_lng = lng - csini->cent_lng;

	/* The following statement is required to handle the case where
	   the central meridian is -179, and the provided longitude is
	   +179, for example. */

	if (fabs (del_lng) > cs_Pi) del_lng = CS_adj2pi (del_lng);

	/* Del_lng of 90 degrees is a problem for this
	   projection. */

	if (fabs (del_lng) > cs_EETest)
	{
		rtn_val = cs_CNVRT_RNG;
		del_lng = (del_lng >= 0.0) ? cs_EETest : cs_WETest;
	}

	cos_lat = cos (lat);
	tan_lat = tan (lat);

	if (csini->ecent == 0.0)
	{
		/* Here for a sphere. */

		B = cos_lat * sin (del_lng);
		xy [XX] = csini->ka * asin (B);

		/* We've dealt with a del_lng of 90 degree above. */

		tmp1 = tan_lat / cos (del_lng);
		xy [YY] = csini->ka * (atan (tmp1) - csini->org_lat);
	}
	else
	{
		/* Here for an ellipsoid. */

		A = cos_lat * del_lng;
		A_2 = A * A;
		A_3 = A_2 * A;
		A_4 = A_2 * A_2;
		A_5 = A_3 * A_2;

		C = csini->C_term * cos_lat * cos_lat;

		sin_lat = sin (lat);
		tmp1 = csini->e_sq * sin_lat * sin_lat;
		N = csini->ka / sqrt (cs_One - tmp1);

		T = tan_lat * tan_lat;

		M = CSmmFcal (&csini->mmcofF,lat,sin_lat,cos_lat);

		/* Now for the x value. */

		tmp1 = A - (T * A_3 * (1.0 / 6.0));
		tmp2 = cs_Eight - T + cs_Eight * C;
		tmp1 = tmp1 + (tmp2 * T * A_5 * (1.0 / 120.0));
		xy [XX] = N * tmp1;

		/* Now the y. */

		tmp1 = cs_Five - T + (cs_Six * C);
		tmp1 = tmp1 * A_4 * (1.0 / 24.0);
		tmp1 += A_2 * cs_Half;
		xy [YY] = M - csini->M0 + (N * tan_lat * tmp1);
	}

	if (csini->quad == 0)
	{
		xy [XX] += csini->x_off;
		xy [YY] += csini->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],csini->x_off,csini->y_off,
							  csini->quad);
	}

	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CScsiniI (csini,ll,xy);
**
**	struct cs_Csini_ *csini;	structure detailing the Cassini coordinate
**								system in use.
**	double ll [2];				converted results returned here,
**								longitude ([0]) and latitude ([1]), in
**								degrees.
**	double xy [2];				coordinates to be converted are obtained
**								from here, x ([0]) and y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The xy and ll arrays may share the same memory.
**********************************************************************/

int EXP_LVL9 CScsiniI (Const struct cs_Csini_ *csini,double ll [2],Const double xy [2])
{
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Pi_o_2;			/* pi over 2 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Third;				/* 0.3333... */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_AnglTest;			/* 0.001 arc seconds, in
										   radians. */
	extern double cs_Huge;				/* an approximation of
										   infinity, not so large
										   that it can't be used in
										   normal calculations
										   without problems. */
	int rtn_val;

	double x;
	double y;

	double lat;
	double del_lng;

	double D;
	double D_2;
	double D_3;
	double D_4;
	double D_5;
	double N1;
	double R1;
	double T1;

	double phi1;
	double sin_phi1;
	double cos_phi1;
	double tan_phi1;

	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	if (csini->quad == 0)
	{
		x = xy [XX] - csini->x_off;
		y = xy [YY] - csini->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,csini->x_off,csini->y_off,csini->quad);
	}

	if (fabs (x) > csini->max_xx)
	{
		rtn_val = cs_CNVRT_RNG;
		x = (x >= 0.0) ? csini->max_xx : -csini->max_xx;
	}
	if (y < csini->min_yy || y > csini->max_yy)
	{
		rtn_val = cs_CNVRT_RNG;
	}

	if (csini->ecent == 0.0)
	{
		/* Here for a spherical datum.  Cos D is zero at the
		   poles.  Tan (tmp1) is also zero when x is zero. */

		D = (y / csini->ka) + csini->org_lat;
		tmp1 = x / csini->ka;

		tmp2 = sin (D) * cos (tmp1);
		lat = asin (tmp2);

		if (fabs (tmp1) > cs_AnglTest)
		{
			/* This blows up when both arguments are zero,
			   which will happen only at the poles. At the
			   poles, all values of longitudes are
			   equivalent. */

			del_lng = atan2 (tan (tmp1),cos (D));
		}
		else
		{
			if (rtn_val == cs_CNVRT_NRML)
			{
				rtn_val = cs_CNVRT_INDF;
			}
			del_lng = cs_Zero;
		}
	}
	else
	{
		/* Here for an ellipsoid. */

		phi1 = CSmmIcal (&csini->mmcofI,csini->M0 + y);

		sin_phi1 = sin (phi1);
		cos_phi1 = cos (phi1);

		if (fabs (cos_phi1) > cs_AnglTest)
		{
			/* Blows up at the poles, but not really out the
			   useful range of this projection. */

			tan_phi1 = sin_phi1/cos_phi1;
		}
		else
		{
			tan_phi1 = cs_Huge;
		}
		T1 = tan_phi1 * tan_phi1;

		tmp1 = cs_One - (csini->e_sq * (sin_phi1 * sin_phi1));
		tmp2 = sqrt (tmp1);
		N1 = csini->ka / tmp2;
		R1 = csini->R_term / (tmp1 * tmp2);

		D = x / N1;
		D_5 = D * (D_4 = D * (D_3 = D * (D_2 = D * D)));

		/* Compute the latitude. */

		tmp1 = (cs_Half * D_2);
		tmp1 -= (cs_One + cs_Three * T1) * D_4 * (1.0 / 24.0);
		tmp1 *= N1 * tan_phi1 / R1;
		lat = phi1 - tmp1;

		/* Now we can compute a longitude. */

		if (fabs (cos_phi1) > cs_AnglTest)
		{
			tmp1 = D - (T1 * (cs_Third * D_3));
			tmp1 += (cs_One + cs_Three * T1) * T1 * D_5 * (1.0 / 15.0);
			del_lng = tmp1 / cos_phi1;	/* Blows up at the poles. */
		}
		else
		{
			/* Any longitude, like cent_lng, will do just
			   fine. */

			if (rtn_val == cs_CNVRT_NRML)
			{
				rtn_val = cs_CNVRT_INDF;
			}
			del_lng = cs_Zero;
		}
	}

	if (fabs (del_lng) > cs_Pi)
	{
		del_lng = CS_adj2pi (del_lng);
		rtn_val = cs_CNVRT_RNG;
	}
	if (fabs (lat) > cs_Pi_o_2)
	{
		lat = CS_adj1pi (lat);
		rtn_val = cs_CNVRT_RNG;
	}

	ll [LNG] = (del_lng + csini->cent_lng) * cs_Radian;
	ll [LAT] = lat * cs_Radian;
	return (rtn_val);
}

/**********************************************************************
**	gamma = CScsiniC (csini,ll);
**
**	struct cs_Csini_ *csini;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Cassini
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

double EXP_LVL9 CScsiniC (Const struct cs_Csini_ *csini,Const double ll [2])
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
	status = CScsiniF (csini,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CScsiniF (csini,xy2,my_ll);
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
**	k = CScsiniK (csini,ll);
**
**	struct m_Csini_ *csini;		structure containing all parameters
**								necessary for the transformation.
**	double ll [2];				location of the point to be computed
**								is given here; longitude ([0]) and
**								latitude ([1]) in degrees.
**	double k;					the grid scale factor at the specified
**								point.  See Remarks below.
**
**	Normally, the K scale factor is the grid scale factor along a
**	parallel at the point in question.  For this projection, what
**	we return is the grid scale factor along a line which
**	perpendicular to the central meridian at the point given.
**	This is because this is a particular feature of this projection.
**	(It may also be because the result is rather easy to calculate,
**	see below.)
**********************************************************************/

double EXP_LVL9 CScsiniK (Const struct cs_Csini_ *csini,Const double ll [2])
{
	extern double cs_One;		/* 1.0 */

	return (cs_One);
}									/*lint !e715 */

/**********************************************************************
**	k = CScsiniH (csini,ll);
**
**	struct m_Csini_ *csini;		structure containing all parameters
**								necessary for the transformation.
**	double ll [2];				location of the point to be computed is
**								given here; longitude ([0]) and
**								latitude ([1]) in degrees.
**	double k;					the grid scale factor at the specified
**								point.
**
**	Normally, the H scale factor is the grid scale factor along
**	a meridian at the point indicated.  For this projection, the
**	H scale factor is the grid scale factor along a line parallel
**	to the central meridian.
**********************************************************************/

double EXP_LVL9 CScsiniH (Const struct cs_Csini_ *csini,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_One;				/* 1.0 */
	extern double cs_Eight;				/* 8.0 */
	extern double cs_AnglTest;			/* 0.001 arc seconds, in
										   radians. */
	extern double cs_Huge;				/* AN approximation of
										   infinity, not so large
										   that it can't be used in
										   calculations without
										   problems. */
	extern double cs_SclInf;			/* 9.9E+04, the value we
										   return for an infinite
										   scale factor. */

	double h;
	double xx;
	double lng;
	double lat;
	double del_lng;

	double sin_lat;
	double cos_lat;
	double tan_lat;

	double N, C, T, B;
	double A, A_3, A_5;

	double tmp1;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;
	del_lng = CS_adj2pi (lng - csini->cent_lng);
	cos_lat = cos (lat);

	if (csini->ecent == 0.0)
	{
		/* Here for a spherical.  Blows up at the equator
		   when del_lng is 90 degrees. */

		B = cos_lat * sin (del_lng);
		tmp1 = cs_One - (B * B);
		if (tmp1 > cs_AnglTest)
		{
			h = cs_One / sqrt (tmp1);
		}
		else
		{
			h = cs_SclInf;
		}
	}
	else
	{
		/* Here for an ellipsoid.  We need to compute the X
		   location of the given point first.  Note, this
		   can bust if we're not careful. */

		A = cos_lat * (del_lng);
		A_5 = A * A * (A_3 = A * A * A);

		C = csini->C_term * cos_lat * cos_lat;

		sin_lat = sin (lat);

		/* Blows up at the poles. */

		if (fabs (cos_lat) > cs_AnglTest)
		{
			tan_lat = sin_lat / cos_lat;
			T = tan_lat * tan_lat;
		}
		else
		{
			T = cs_Huge;
		}

		tmp1 = cs_One - (csini->e_sq * sin_lat * sin_lat);
		N = csini->ka / sqrt (tmp1);

		/* Now for the X value. */

		xx = A - (T * A_3 * (1.0 / 6.0));
		xx -= (cs_Eight - T + cs_Eight * C) * T * A_5 * (1.0 / 120.0);
		xx *= N;

		/* Now for H.  We use Synder's equation 13-9, with Az set
		   to zero, thus cosine squared Az is 1.0. */

		h = cs_One + ((xx * tmp1) * (xx * tmp1)) / csini->s_term;
	}
	return (h);
}

/**********************************************************************
**	status = CScsiniL (csini,cnt,pnts);
**
**	struct cs_Csini_ *csini;	the coordinate system against which the
**								check is to be performed.
**	int cnt;					the number of points in the region to be
**								checked.
**	double pnts [][3];			the list of coordinates to be checked.
**	int status;					returns cs_CNVRT_OK if the point, line,
**								or region is completely within the
**								domain of the coordinate system.
**								Otherwise, cs_CNVRT_DOMN
**								is returned.
**
**	This function simply checks the mathematical viability of
**	a coordinate conversion.  It has nothing to do with the
**	useful limits of the coordinate system.
**
**	This function expects that the input lat/longs are normalized
**	if appropriate.
**********************************************************************/

int EXP_LVL9 CScsiniL (Const struct cs_Csini_ *csini,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi_o_2;		/* PI over 2 */
	extern double cs_EETest;		/* 0.001 arc seconds short of
									   90 degrees, in radians. */

	int ii;
	int status;

	double lat;
	double del_lng;

	/* Check all the points. The area of the domain is continuous,
	   so there is no need for special checking on lines or
	   regions. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
		del_lng = pnts [ii][LNG] * cs_Degree - csini->cent_lng;
		if (fabs (del_lng) > cs_EETest)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CScsiniX (csini,cnt,pnts);
**
**	struct cs_Csini_ *csini;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be
**								checked.
**	int status;					returns the status of the check; see
**								remarks below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CScsiniL to check lat/long
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

int EXP_LVL9 CScsiniX (Const struct cs_Csini_ *csini,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double xx, yy;

	rtn_val = cs_CNVRT_OK;

	/* Check that all X's and Y's are within the basic
	   range.  No special checks are required for lines and/or
	   regions. */

	for (ii = 0;ii < cnt;ii++)
	{
		if (csini->quad == 0)
		{
			xx = pnts [ii][XX] - csini->x_off;
			yy = pnts [ii][YY] - csini->y_off;
		}
		else
		{
			CS_quadI (&xx,&yy,pnts [ii],csini->x_off,
						    csini->y_off,
						    csini->quad);
		}
		if (fabs (xx) > csini->max_xx ||
		    fabs (yy) > csini->max_yy)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
