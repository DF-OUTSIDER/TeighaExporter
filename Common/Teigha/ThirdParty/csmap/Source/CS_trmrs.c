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

int EXP_LVL9 CStrmrsQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
{
	extern double cs_One;			/*    1.0 */
	extern double cs_Mone;			/*    1.0 */
	extern double cs_K60;			/*   60.0 */
	extern double cs_MinLng;		/* -180.0 */
	extern double cs_MinLat;		/* - 90.0 */
	extern double cs_MaxLng;		/* +180.0 */
	extern double cs_MaxLat;		/* + 90.0 */
	extern double cs_SclRedMin;		/*    0.75 */
	extern double cs_SclRedMax;		/*    1.1 */

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	if (prj_code != cs_PRJCOD_UTM)
	{
		/* Basic Transverse Mercator specific stuff. */
	
		if (cs_def->prj_prm1 <= cs_MinLng || cs_def->prj_prm1 > cs_MaxLng)
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
	}
	else
	{
		/* Here for the UTM projection. */

		if (cs_def->prj_prm1 < cs_One || cs_def->prj_prm1 > cs_K60)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_UTMZON;
		}
		if (cs_def->prj_prm2 < cs_Mone || cs_def->prj_prm2 > cs_One)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_CSQ_HMISPHR;
		}
	}

	/* That's it for Transverse Mercator. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CStrmrtS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate
**								system parameters.
**
**	Normal conventions and CS-MAP standards have NOT
**	been followed in codng this function so that the
**	variable nomenclature could adhere as closely
**	as possible to that used in the reference.
**********************************************************************/

void EXP_LVL9 CStrmrsS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_EETest;			/* .001 seconds of arc
										   short of 90 degrees
										   in radians; used to
										   test for excessive
										   eastern longitude. */

	struct cs_Trmrs_ *trmrs;

	double tmp1;
	double e_2;
	double e_4;
	double e_6;
	double e1_2;
	double e1_3;
	double e1_4;
	double zone_wd;

	double test_ll [3];
	double test_xy [3];

	trmrs = &csprm->proj_prms.trmrs;

	trmrs->cent_lng = csprm->csdef.prj_prm1 * cs_Degree;
	trmrs->org_lat = csprm->csdef.org_lat * cs_Degree;
	trmrs->x_off = csprm->csdef.x_off;
	trmrs->y_off = csprm->csdef.y_off;
	trmrs->k0 = csprm->csdef.scl_red;
	trmrs->k = csprm->csdef.scale * csprm->csdef.scl_red;
	trmrs->ecent = csprm->datum.ecent;
	trmrs->e_sq = trmrs->ecent * trmrs->ecent;
	trmrs->e_rad = csprm->datum.e_rad;
	trmrs->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	if (trmrs->ecent == 0.0)
	{
		/* Here for a sphere. */

		trmrs->Rk = trmrs->e_rad * trmrs->k;
		trmrs->Rk_ovr_2 = trmrs->Rk * 0.5;
	}
	else
	{
		/* Here for an ellipsoid. */

		trmrs->eprim_sq = trmrs->e_sq / (1.0 - trmrs->e_sq);

		/* The coefficients for the M calculations. */

		e_2 = trmrs->e_sq;
		e_4 = e_2 * e_2;
		e_6 = e_4 * e_2;

		tmp1 = 1.0;
		tmp1 -= e_2 * (1.0 / 4.0);
		tmp1 -= (3.0 / 64.0) * e_4;
		trmrs->Mcoef0 = tmp1 - (5.0 / 256.0) * e_6;
		tmp1 = (3.0 / 8.0) * e_2;
		tmp1 += (3.0 / 32.0) * e_4;
		trmrs->Mcoef2 = tmp1 + (45.0 / 1024.0) * e_6;
		tmp1 = (15.0 / 256.0) * e_4;
		trmrs->Mcoef4 = tmp1 + (45.0 / 1024.0) * e_6;
		trmrs->Mcoef6 = (35.0 / 3072.0) * e_6;

		/* Now we compute M0, the M associated with
		   the origin latiude. */

		tmp1 = trmrs->Mcoef0 * trmrs->org_lat;
		tmp1 -= trmrs->Mcoef2 * sin (2.0 * trmrs->org_lat);
		tmp1 += trmrs->Mcoef4 * sin (4.0 * trmrs->org_lat);
		tmp1 -= trmrs->Mcoef6 * sin (6.0 * trmrs->org_lat);
		trmrs->M0 = trmrs->e_rad * tmp1;

		/* The mu coefficients for the phi1 calculation
		   on the inverse. */

		tmp1 = sqrt (1.0 - trmrs->e_sq);
		trmrs->e1 = (1.0 - tmp1) / (1.0 + tmp1);
		e1_2 = trmrs->e1 * trmrs->e1;
		e1_3 = trmrs->e1 * e1_2;
		e1_4 = e1_2 * e1_2;
		trmrs->mucoef2 = trmrs->e1 * (3.0 / 2.0) - e1_3 * (27.0 / 32.0);
		trmrs->mucoef4 = e1_2 * (21.0 / 16.0) - e1_4 * (55.0 / 32.0);
		trmrs->mucoef6 = e1_3 * (151.0 / 96.0);
		trmrs->mucoef8 = e1_4 * (1097.0 / 512.0);
	}

	/* Need to compute the maximum value of X which can be
	   supported mathematically.  This is, essentially, the
	   value generated by a del_lng of a skosh less than
	   90 degrees at the equator.  Since most all the setup
	   is complete, we can use the CStrmrsF function to
	   compute this for us. */

	test_ll [LNG] = CS_adj2pi (trmrs->cent_lng + cs_EETest) * cs_Radian;
	test_ll [LAT] = cs_Zero;
	CStrmrsF (trmrs,test_ll,test_ll);				/*lint !e534   ignoring return value */
	trmrs->xx_max = fabs (test_ll [XX]);

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = trmrs->cent_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* We're to calculate the useful range.  We'll assume a
		   5 degree width, increased by an amount related to
		   scale reduction.  We set the latitude range to that
		   customarily used in the UTM system.

		   There are some coordinate systems where the scale
		   reduction factor is greater than 1; strange as it
		   might seem. */

		tmp1 = trmrs->k0;
		if (tmp1 > cs_One) tmp1 = cs_One;
		zone_wd = cs_Three + cs_Two * acos (tmp1) * cs_Radian;
		csprm->min_ll [LNG] = -zone_wd;
		csprm->max_ll [LNG] =  zone_wd;

		csprm->min_ll [LAT] = -84.0;
		csprm->max_ll [LAT] =  84.0;
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
		   The setup is virtually complete, so we can use CStrmrsF
		   to calculate some values, if necessary. The curved
		   nature of the lat/long lines means we cannot just
		   convert the lat/long min/max.

		   Need to compute the min/max without the false origin
		   first, it will be added back in by the quadMM
		   processor. */

		test_ll [LNG] = CS_adj180 (csprm->cent_mer + csprm->max_ll [LNG]);
		test_ll [LAT] = trmrs->org_lat * cs_Radian;
		CStrmrsF (trmrs,test_xy,test_ll);				/*lint !e534   ignoring return value */
		csprm->max_xy [XX] = test_xy [XX] - trmrs->x_off;
		csprm->min_xy [XX] = -csprm->max_xy [XX];

		/* Origin latitude is not always the equator, need to
		   do min and max separately. */

		test_ll [LNG] = csprm->cent_mer;
		test_ll [LAT] = -84.0;
		CStrmrsF (trmrs,test_xy,test_ll);				/*lint !e534   ignoring return value */
		csprm->min_xy [YY] = test_xy [YY] - trmrs->y_off;
		test_ll [LAT] = 84.0;
		CStrmrsF (trmrs,test_xy,test_ll);				/*lint !e534   ignoring return value */
		csprm->max_xy [YY] = test_xy [YY] - trmrs->y_off;

		/* Apply quad processing, e.g. a left handed coordinate
		   system. */

		CS_quadMM (csprm->min_xy,csprm->max_xy,trmrs->x_off,
											   trmrs->y_off,
											   trmrs->quad);
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

	/* That's all the calculations we can do at this time.
	   Set up the internal function calls.  Note, since the
	   Transverse Mercator is a conformal projection, the
	   h and k scale factors are the same.  Therefore, we
	   set all three scale function pointers to the same
	   function. */

	csprm->ll2cs = (cs_LL2CS_CAST)CStrmrsF;
	csprm->cs2ll = (cs_CS2LL_CAST)CStrmrsI;
	csprm->cs_scale = (cs_SCALE_CAST)CStrmrsK;
	csprm->cs_sclk = (cs_SCALK_CAST)CStrmrsK;
	csprm->cs_sclh = (cs_SCALH_CAST)CStrmrsK;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CStrmrsC;
	csprm->llchk    = (cs_LLCHK_CAST)CStrmrsL;
	csprm->xychk    = (cs_XYCHK_CAST)CStrmrsX;

	return;
}

int EXP_LVL9 CStrmrsF (Const struct cs_Trmrs_ *trmrs,double xy [2],Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi;				/* 3.14159... */
	extern double cs_Pi_o_2;			/* Pi over two */
	extern double cs_Mpi_o_2;			/* minus Pi over two */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_AnglTest;			/* .001 seconds of arc in
										   radians. */
	extern double cs_AnglTest1;			/* 1.0 minus .001 seconds of
										   arc radians. */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the north pole,
										   in radians. */
	extern double cs_SPTest;			/* .001 seconds of arc
										   short of the south pole,
										   in radians. */
	extern double cs_EETest;			/* .001 seconds of arc
										   short of 90 degrees,
										   in radians. */
	extern double cs_WETest;			/* .001 seconds of arc
										   short of -90 degrees,
										   in radians. */
	extern double cs_Huge;				/* An approximation of
										   infinity, 1.0E+37,
										   but not so large as
										   we can't use it in
										   calculations. */
	extern double cs_Mhuge;				/* Negative infinity. */

	int rtn_val;

	double lng;
	double lat;
	double del_lng;

	double A;
	double A_2;
	double A_3;
	double A_4;
	double A_5;
	double A_6;
	double B;
	double C;
	double C_2;
	double M;
	double N;
	double T;
	double T_2;

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
		if (lat >= cs_NPTest)
		{
			/* North Pole */

			sin_lat = cs_One;
			cos_lat = cs_Zero;
			tan_lat = cs_Huge;
			lat = cs_Pi_o_2;
		}
		else if (lat <= cs_SPTest)
		{
			/* South Pole */

			sin_lat = cs_Mone;
			cos_lat = cs_Zero;
			tan_lat = cs_Mhuge;
			lat = cs_Mpi_o_2;
		}
		else
		{
			sin_lat = sin (lat);
			cos_lat = cos (lat);
			tan_lat = sin_lat / cos_lat;
		}
	}
	else
	{
		/* Otherwise.  Since we have handled the poles,
		   cos (lat) should never be zero here. */

		sin_lat = sin (lat);
		cos_lat = cos (lat);
		tan_lat = sin_lat / cos_lat;
	}

	/* That's the latitude, deal with the longitude now. */

	del_lng = lng - trmrs->cent_lng;

	/* The following statement deals with the issue of the -180 crack.
	   For example, if the central meridian is -179, and the provided
	   longitude is +179, we want a del_lng of -2. */

	if (fabs (del_lng) > cs_Pi) del_lng = CS_adj2pi (del_lng);

	/* We can't allow a del_lng value to get too close to
	   PI over 2 (i.e. 90 degrees) if the cosine of the latitude
	   is one (i.e. the equator).  Otherwise, all longitude
	   values can be handled.  Note, that we allow delta longitude
	   values greater than 90, it's the specific value of 90
	   that causes trouble.  When delta longitude exceeds 90,
	   the projection just wraps back on itself, thus providing
	   the "rational" results which we must return. */

	if (del_lng > cs_EETest || del_lng < cs_WETest)
	{
		rtn_val = cs_CNVRT_RNG;
		if (fabs (fabs (del_lng) - cs_Pi_o_2) < cs_AnglTest &&
		    fabs (cos_lat) > cs_AnglTest1)
		{
			/* Opps!!! We're in never never land. */

			if (del_lng < 0.0)
			{
				del_lng = cs_Mpi_o_2 + cs_AnglTest;
			}
			else
			{
				del_lng = cs_Pi_o_2 - cs_AnglTest;
			}
		}
	}

	if (trmrs->ecent == 0.0)
	{
		/* Here for a sphere. */

		B = cos_lat * sin (del_lng);
		tmp1 = (1.0 + B) / (1.0 - B);
		xy [0] = trmrs->Rk_ovr_2 * log (tmp1) + trmrs->x_off;

		tmp1 = tan_lat / cos (del_lng);
		xy [1] = trmrs->Rk * (atan (tmp1) - trmrs->org_lat) + trmrs->y_off;
	}
	else
	{
		/* Here for an ellipsoid. */

		A = cos_lat * (del_lng);
		A_2 = A * A;
		A_3 = A_2 * A;
		A_4 = A_2 * A_2;
		A_5 = A_3 * A_2;
		A_6 = A_3 * A_3;

		C = trmrs->eprim_sq * cos_lat * cos_lat;
		C_2 = C * C;

		tmp1 = trmrs->e_sq * sin_lat * sin_lat;
		N = trmrs->e_rad / sqrt (1.0 - tmp1);

		T = tan_lat * tan_lat;
		T_2 = T * T;

		M = trmrs->Mcoef0 * lat;
		M -= trmrs->Mcoef2 * sin (2.0 * lat);
		M += trmrs->Mcoef4 * sin (4.0 * lat);
		M -= trmrs->Mcoef6 * sin (6.0 * lat);
		M *= trmrs->e_rad;

		/* Now for the x value. */

		tmp1 = A;
		tmp1 += (1.0 - T + C) * A_3 * (1.0 / 6.0);
		tmp2 = 5.0 - 18.0 * T + T_2 + 72.0 * C - 58.0 * trmrs->eprim_sq;
		tmp1 += tmp2 * A_5 * (1.0 / 120.0);
		xy [0] = trmrs->k * N * tmp1;

		/* Now the y. */

		tmp1 = A_2 * (1.0 / 2.0);
		tmp1 += (5.0 - T + 9.0 * C + 4.0 * C_2) * A_4 * (1.0 / 24.0);
		tmp2 = 61.0 - 58.0 * T + T_2 + 600.0 * C - 330.0 * trmrs->eprim_sq;
		tmp1 += tmp2 * A_6 * (1.0 / 720.0);
		tmp1 *= N * tan_lat;
		xy [1] = trmrs->k * (M - trmrs->M0 + tmp1);
	}

	/* Apply the false easting and northing, perhaps a quad adjustment. */

	if (trmrs->quad == 0)
	{
		xy [XX] += trmrs->x_off;
		xy [YY] += trmrs->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],trmrs->x_off,trmrs->y_off,trmrs->quad);
	}
	return (rtn_val);
}

int EXP_LVL9 CStrmrsI (Const struct cs_Trmrs_ *trmrs,double ll [2],Const double xy [2])
{
	extern double cs_Radian;		/* 57.29577... */

	int rtn_val;

	double x;
	double y;

	double C;
	double C_2;
	double D;
	double D_2;
	double D_3;
	double D_4;
	double D_5;
	double D_6;
	double M;
	double N;
	double R;
	double T;
	double T_2;

	double phi1;
	double mu;
	double sin_phi1;
	double cos_phi1;
	double tan_phi1;

	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	if (trmrs->quad == 0)
	{
		x = xy [XX] - trmrs->x_off;
		y = xy [YY] - trmrs->y_off;
	}
	else
	{
		CS_quadI (&x,&y,xy,trmrs->x_off,trmrs->y_off,trmrs->quad);
	}

	/* Deal with the limiting xx case.  Xx_max is the value which
	   the forward function returns for a latitude of zero and
	   a delta longitude of +-90.0 degrees.  This is the limit
	   of the map.  In any case, it is about 10 times the
	   scaled radius of the earth (Rk).

	   Since latitude is determined primarily by the sin (yy/R),
	   it wraps around automatically.  Therefore, there is no
	   mathematical limit on Y. */

	if (fabs (x) > trmrs->xx_max)
	{
		rtn_val = cs_CNVRT_RNG;
		x = (x >= 0.0) ? trmrs->xx_max : -trmrs->xx_max;
	}

	if (trmrs->ecent == 0.0)
	{
		/* Here for a sphere. */

		D = y / trmrs->Rk + trmrs->org_lat;
		tmp1 = x / trmrs->Rk;

		tmp2 = sin (D) / cosh (tmp1);
		ll [1] = asin (tmp2) * cs_Radian;

		tmp1 = sinh (tmp1) / cos (D);
		ll [0] = (trmrs->cent_lng + atan (tmp1)) * cs_Radian;
	}
	else
	{
		/* Here for an ellipsoid. */

		M = trmrs->M0 + (y / trmrs->k);
		mu = M / (trmrs->e_rad * trmrs->Mcoef0);
		phi1 = mu;
		phi1 += trmrs->mucoef2 * sin (2.0 * mu);
		phi1 += trmrs->mucoef4 * sin (4.0 * mu);
		phi1 += trmrs->mucoef6 * sin (6.0 * mu);
		phi1 += trmrs->mucoef8 * sin (8.0 * mu);
		sin_phi1 = sin (phi1);
		cos_phi1 = cos (phi1);
		tan_phi1 = tan (phi1);

		C = trmrs->eprim_sq * cos_phi1 * cos_phi1;
		C_2 = C * C;

		T = tan_phi1 * tan_phi1;
		T_2 = T * T;

		tmp1 = 1.0 - trmrs->e_sq * sin_phi1 * sin_phi1;
		tmp2 = sqrt (tmp1);
		N = trmrs->e_rad / tmp2;
		R = trmrs->e_rad * (1.0 - trmrs->e_sq) / (tmp1 * tmp2);

		D = x / (N * trmrs->k);
		D_2 = D * D;
		D_3 = D_2 * D;
		D_4 = D_2 * D_2;
		D_5 = D_3 * D_2;
		D_6 = D_3 * D_3;

		/* Now we can compute a longitude. */

		tmp1 = D;
		tmp1 -= (1.0 + 2.0*T + C) * D_3 * (1.0 / 6.0);
		tmp2 = 5.0 - 2.0*C + 28.0*T - 3.0*C_2;
		tmp2 += 8.0*trmrs->eprim_sq + 24.0*T_2;
		tmp1 += tmp2 * D_5 * (1.0 / 120.0);
		ll [0] = (trmrs->cent_lng + tmp1 / cos_phi1) * cs_Radian;

		/* Now for a latitude. */

		tmp1 = D_2 * (1.0 / 2.0);
		tmp2 = 5.0 + 3.0*T + 10.0*C - 4.0*C_2 - 9.0*trmrs->eprim_sq;
		tmp1 -= tmp2 * D_4 * (1.0 / 24.0);
		tmp2 = 61.0 + 90.0*T + 298.0*C + 45.0*T_2;
		tmp2 -= 252.0 * trmrs->eprim_sq + 3.0*C_2;
		tmp1 += tmp2 * D_6 * (1.0 / 720.0);
		tmp2 = N * tan_phi1 / R;
		ll [1] = (phi1 - (tmp2 * tmp1)) * cs_Radian;
	}
	return (rtn_val);
}
double EXP_LVL9 CStrmrsK (Const struct cs_Trmrs_ *trmrs,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi_o_2;			/* Pi over two */
	extern double cs_Mpi_o_2;			/* Minus Pi over two */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_Half;				/* 0.5 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Four;				/* 4.0 */
	extern double cs_Five;				/* 5.0 */
	extern double cs_NPTest;			/* .001 seconds of arc
										   short of the North Pole,
										   in radians. */
	extern double cs_SPTest;			/* .001 seconds of arc
										   short of the South Pole,
										   in radians. */

	double k;
	double lng;
	double lat;
	double del_lng;

	double A;
	double A_2;
	double A_4;
	double A_6;
	double B;
	double C;
	double C_2;
	double T;
	double T_2;

	double cos_lat;
	double tan_lat;

	double tmp1;
	double tmp2;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;

	if (lat > cs_NPTest)
	{
		lat = cs_Pi_o_2;
		cos_lat = cs_Zero;
	}
	else if (lat < cs_SPTest)
	{
		lat = cs_Mpi_o_2;
		cos_lat = cs_Zero;
	}
	else
	{
		cos_lat = cos (lat);
	}
	del_lng = CS_adj2pi (lng - trmrs->cent_lng);

	if (trmrs->ecent == 0.0)
	{
		/* Here for a sphere. */

		B = cos_lat * sin (del_lng);
		tmp1 = cs_One - B * B;
		k = trmrs->k0 / sqrt (tmp1);
	}
	else
	{
		/* Here for an ellipsoid. */

		A = cos_lat * (del_lng);
		A_2 = A * A;
		A_4 = A_2 * A_2;
		A_6 = A_4 * A_2;

		C = trmrs->eprim_sq * cos_lat * cos_lat;
		C_2 = C * C;

		tan_lat = tan (lat);
		T = tan_lat * tan_lat;
		T_2 = T * T;

		/* Now for the k value. */

		tmp1 = cs_One;
		tmp1 += (cs_One + C) * A_2 * cs_Half;
		tmp2 = cs_Five - cs_Four*T + 42.0*C + 13.0*C_2 - 28.0*trmrs->eprim_sq;
		tmp1 += tmp2 * A_4 * (cs_One / 24.0);
		tmp2 = 61.0 - 148.0*T + 16.0*T_2;
		tmp1 += tmp2 * A_6 * (cs_One / 720.0);
		k = trmrs->k0 * tmp1;
	}
	return (k);
}

double EXP_LVL9 CStrmrsC (Const struct cs_Trmrs_ *trmrs,Const double ll [2])
{
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Three;				/* 3.0 */
	extern double cs_K15;				/* 15.0 */
	extern double cs_Huge;				/* An approximation of
										   infinity which is small
										   enough to calculate with
										   without producing
										   FP faults. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole
										   in radians. */
	extern double cs_SPTest;			/* 0.001 seconds of arc
										   short of the south pole
										   in radians. */
	extern double cs_EETest;			/* 0.001 seconds of arc
										   short of +90.00, in
										   radians. */
	extern double cs_WETest;			/* 0.001 seconds of arc
										   short of -90.00, in
										   radians. */

	double alpha;
	double lng;
	double lat;

	double sin_lat;
	double cos_lat;
	double cos_lat2;
	double tan_lat;
	double tan_lat2;
	double del_lng;
	double del_lng2;

	double tmp1;
	double tmp2;

	lng = ll [LNG] * cs_Degree;
	lat = ll [LAT] * cs_Degree;
	del_lng = CS_adj2pi (lng - trmrs->cent_lng);
	if (del_lng < cs_WETest) del_lng = cs_WETest;
	if (del_lng > cs_EETest) del_lng = cs_EETest;
	del_lng2 = del_lng * del_lng;

	if (lat > cs_NPTest)
	{
		tan_lat = cs_Huge;
		cos_lat = cs_Zero;
		sin_lat = cs_One;
	}
	else if (lat < cs_SPTest)
	{
		tan_lat = -cs_Huge;
		cos_lat = cs_Zero;
		sin_lat = -cs_One;
	}
	else
	{
		sin_lat = sin (lat);
		cos_lat = cos (lat);
		tan_lat = tan (lat);
	}
	cos_lat2 = cos_lat * cos_lat;
	tan_lat2 = tan_lat * tan_lat;

	if (trmrs->ecent == 0.0)
	{
		/* Here for a sphere. */

		tmp1 = del_lng2 * cos_lat2 / cs_Three;
		tmp2 = (del_lng2 * del_lng2) * (cos_lat2 * cos_lat2) *
				(cs_Two - tan_lat2) / cs_K15;
		alpha = del_lng * sin_lat * (cs_One + tmp1 + tmp2);
	}
	else
	{
		tmp1 = cs_One + (cs_Three * trmrs->eprim_sq * cos_lat2);
		tmp1 = del_lng2 * cos_lat2 * tmp1 / cs_Three;
		tmp2 = (del_lng2 * del_lng2) * (cos_lat2 * cos_lat2) *
				(cs_Two - tan_lat2) / cs_K15;
		alpha = del_lng * sin_lat * (cs_One + tmp1 + tmp2);
	}
	return (alpha * cs_Radian);
}


/**********************************************************************
	The following function is static, not visible outside
	this source module, so the name may not adhere to the
	normal naming conventtion.

	The function verifies that the point supplied to it
	is indeed within the geographic domain of the coordinate
	system.
**********************************************************************/

static int CStrmrsLP (Const struct cs_Trmrs_ *trmrs,Const double ll [3])
{
	extern double cs_Degree;		/* 1.0 / 57.2... */
	extern double cs_Pi_o_2;		/* PI / 2.0 */
	extern double cs_AnglTest;		/* 0.001 seconds of arc in
									   radians */
	double del_lng;

	double my_lng, my_lat;

	/* There is one case which we must check for; 90 degrees from
	   the central meridian on the equator.  That's it. */

	my_lng =       ll [LNG] * cs_Degree;
	my_lat = fabs (ll [LAT] * cs_Degree);

	if (my_lat > cs_Pi_o_2) return (cs_CNVRT_DOMN);

	del_lng = fabs (CS_adj2pi (my_lng - trmrs->cent_lng));
	if (my_lat < cs_AnglTest &&
	    fabs (del_lng - cs_Pi_o_2) < cs_AnglTest)
	{
		return (cs_CNVRT_DOMN);
	}

	return (cs_CNVRT_OK);
}

int EXP_LVL9 CStrmrsL (Const struct cs_Trmrs_ *trmrs,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;
	extern double cs_EETest;			/* .001 seconds of arc less
										   than PI/2 in radians. */
	extern double cs_WETest;		/* -cs_EETest */

	int ii;
	int status;

	double tmp;
	double del_lng;

	/* Check all the points. */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
	{
		status = CStrmrsLP (trmrs,pnts [ii]);
	}
	if (cnt <= 1 || status != cs_CNVRT_OK) return (status);

	/* If cnt is 2, we have a line which we must check. */

	if (cnt == 2)
	{
		/* WHAT WE SHOULD BE DOING:
		   We should be calculating the angular distance
		   from the great circle defined by the the two
		   points to each of the two singularity points.
		   If either distance is less than cs_AnglTest,
		   we have a problem.

		   WHAT WE DO NOW:
		   If the longitude of either point exceeds the
		   central meridian +- pi/2, we have a problem. */

		for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
		{
			tmp = pnts [ii][LNG] * cs_Degree;
			del_lng = CS_adj2pi (tmp - trmrs->cent_lng);
			if (del_lng > cs_EETest || del_lng < cs_WETest)
			{
				status = cs_CNVRT_DOMN;
			}
		}
	}
	else if (cnt == 3)
	{
		/* Can't handle a three point list, the region must
		   be closed. */

		CS_erpt (cs_RGN_PNTCNT);
		return (cs_CNVRT_ERR);
	}
	else
	{
		/* WHAT WE SHOULD BE DOING:
		   The great circle between the two singularity points
		   is a 180 degree segment of the equator.  We should
		   1) use the technique described for the line case
		      to see if any segment of the region actually
		      goes through either of the singularity points.
		   2) count the intersections of all segments in the
		      boundary with the equator segment between the
		      two singularity points.  If the count is even
		      (or zero) we are OK.  Otherwise, we have a problem.

		   WHAT WE DO NOW:
		   We simply see if any of the points has a longitude
		   which puts it outside of the range of central
		   meridian +- pi/2. */

		for (ii = 0;ii < cnt && status == cs_CNVRT_OK;ii++)
		{
			tmp = pnts [ii][LNG] * cs_Degree;
			del_lng = CS_adj2pi (tmp - trmrs->cent_lng);
			if (del_lng > cs_EETest || del_lng < cs_WETest)
			{
				status = cs_CNVRT_DOMN;
			}
		}
	}
	return (status);
}

int EXP_LVL9 CStrmrsX (Const struct cs_Trmrs_ *trmrs,int cnt,Const double pnts [][3])
{
	int ii;
	int rtn_val;

	double test_val;
	double dummy;

	rtn_val = cs_CNVRT_OK;

	/* All Y values are within the domain of the function.  However,
	   in the cartesian frame of reference, an absolute value of
	   X greater than xx_max is outside of the domain. */

	for (ii = 0;ii < cnt && rtn_val == cs_CNVRT_OK;ii++)
	{
		if (trmrs->quad == 0)
		{
			test_val = pnts [ii][XX] - trmrs->x_off;
		}
		else
		{
			CS_quadI (&test_val,&dummy,pnts [ii],trmrs->x_off,
												 trmrs->y_off,
												 trmrs->quad);
		}

		if (fabs (test_val) > trmrs->xx_max)
		{
			rtn_val = cs_CNVRT_DOMN;
		}
	}
	return (rtn_val);
}
