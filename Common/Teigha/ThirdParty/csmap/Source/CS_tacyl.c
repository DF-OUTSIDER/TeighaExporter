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
**	err_cnt = CStacylQ (cs_def,prj_code,err_list,list_sz);
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

int EXP_LVL9 CStacylQ (	Const struct cs_Csdef_ *cs_def,unsigned short prj_code,int err_list [],int list_sz)
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

	/* Check the Transverse Aspect of the Equal Area Cylindrical
	   projection specific stuff. */
	
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

	/* That's it for Transverse Aspect of the Equal Area Cylindrical
	   projection. */
	
	return (err_cnt + 1);
}

/**********************************************************************
**	(void) CStacylS (csprm);
**
**	struct cs_Csprm_ *csprm;	structure containing all coordinate
**								system parameters.
**********************************************************************/

void EXP_LVL9 CStacylS (struct cs_Csprm_ *csprm)
{
	extern short cs_QuadMin;			/* -4 */
	extern short cs_QuadMap [];
	extern double cs_Radian;			/* 57.29577... */
	extern double cs_Degree;			/* 1.0 / 57.29577... */
	extern double cs_Pi_o_2;			/* Pi over 2 */
	extern double cs_Zero;				/* 0.0 */
	extern double cs_One;				/* 1.0 */
	extern double cs_Two;				/* 2.0 */
	extern double cs_Ten;				/* 2.0 */

	struct cs_Tacyl_ *tacyl;

	double tmp;

	double sin_org_lat;
	double cos_org_lat;

	double test_ll [3];
	double test_xy [3];

	tacyl = &csprm->proj_prms.tacyl;

	tacyl->org_lng = csprm->csdef.org_lng * cs_Degree;
	tacyl->org_lat = csprm->csdef.org_lat * cs_Degree;
	tacyl->h0 = csprm->csdef.scl_red;
	tacyl->x_off = csprm->csdef.x_off;
	tacyl->y_off = csprm->csdef.y_off;
	tacyl->e_rad = csprm->datum.e_rad;
	tacyl->ecent = csprm->datum.ecent;
	tacyl->k = csprm->csdef.scale * tacyl->h0;
	tacyl->quad = cs_QuadMap [csprm->csdef.quad - cs_QuadMin];

	/* Note that due to the authalic nature of this projection,
	   the ka factor does not include the scale reduction
	   factor as is common for conformal projections. */

	tacyl->ka = csprm->csdef.scale * tacyl->e_rad;
	tacyl->ka_o_h0 = tacyl->ka / tacyl->h0;
	tacyl->kah0    = tacyl->ka * tacyl->h0;
	tacyl->h0_o_ka = tacyl->h0 / tacyl->ka;

	if (tacyl->ecent == 0.0)
	{
		tacyl->max_xx = tacyl->ka_o_h0;
		tacyl->max_yy = tacyl->kah0;
	}
	else
	{
		/* For the ellipsoid. */

		tacyl->ecent = csprm->datum.ecent;
		tacyl->e_sq = tacyl->ecent * tacyl->ecent;
		tacyl->one_m_esq = cs_One - tacyl->e_sq;

		tacyl->one_o_2e = cs_One / (cs_Two * tacyl->ecent);

		tmp = (cs_One - tacyl->ecent) / (cs_One + tacyl->ecent);
		tacyl->qp = cs_One - (tacyl->one_m_esq * tacyl->one_o_2e) * log (tmp);

		CSbtIsu (&tacyl->btcofI,tacyl->e_sq);
		CSmmFsu (&tacyl->mmcofF,tacyl->ka,tacyl->e_sq);
		CSmmIsu (&tacyl->mmcofI,tacyl->ka,tacyl->e_sq);

		sin_org_lat = sin (tacyl->org_lat);
		cos_org_lat = cos (tacyl->org_lat);
		tacyl->M0 = CSmmFcal (&tacyl->mmcofF,tacyl->org_lat,sin_org_lat,cos_org_lat);

		tacyl->max_xx = tacyl->ka_o_h0 / tacyl->one_m_esq;
		tacyl->max_yy = tacyl->h0 * CSmmFcal (&tacyl->mmcofF,cs_Pi_o_2,cs_One,cs_Zero);
	}

	/* Set up the coordinate checking information.  If the user has
	   specified a useful range, we use it without checking it.
	   Otherwise, we compute what I, the programmer, consider to
	   be the useful range of the projection.  Note, values are in
	   degrees and longitude values are relative to the central
	   meridian. */

	csprm->cent_mer = tacyl->org_lng * cs_Radian;
	if (csprm->csdef.ll_min [LNG] == 0.0 &&
	    csprm->csdef.ll_max [LNG] == 0.0)
	{
		/* We're to calculate the useful range.  We'll assume a
		   20 degree width, increased by an amount related to
		   scale reduction.  We set the latitude range to that
		   customarily used in the UTM system. */

		tmp = cs_Two * acos (tacyl->h0) * cs_Radian + cs_Ten;
		csprm->min_ll [LNG] = -tmp;
		csprm->max_ll [LNG] =  tmp;

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
		   The setup is virtually complete, so we can use CStacylF
		   to calculate some values, if necessary. The curved
		   nature of the lat/long lines means we cannot just
		   convert the lat/long min/max. */

		test_ll [LNG] = CS_adj180 (csprm->cent_mer + csprm->min_ll [LNG]);
		test_ll [LAT] = tacyl->org_lat * cs_Radian;
		CStacylF (tacyl,test_xy,test_ll);					/*lint !e534 */
		csprm->min_xy [XX] = test_xy [XX] - tacyl->x_off;
		csprm->max_xy [XX] = -csprm->min_xy [XX];

		test_ll [LNG] = csprm->cent_mer;
		test_ll [LAT] = -84.0;
		CStacylF (tacyl,test_xy,test_ll);					/*lint !e534 */
		csprm->min_xy [YY] = test_xy [YY] - tacyl->y_off;
		test_ll [LAT] = 84.0;
		CStacylF (tacyl,test_xy,test_ll);					/*lint !e534 */
		csprm->max_xy [YY] = test_xy [YY] - tacyl->y_off;

		CS_quadMM (csprm->min_xy,csprm->max_xy,tacyl->x_off,
											   tacyl->y_off,
											   tacyl->quad);
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
	   The projection is conformal, so ha nd k scale
	   factors are the same. */

	csprm->ll2cs    = (cs_LL2CS_CAST)CStacylF;
	csprm->cs2ll    = (cs_CS2LL_CAST)CStacylI;
	csprm->cs_scale = (cs_SCALE_CAST)CStacylK;
	csprm->cs_sclk  = (cs_SCALK_CAST)CStacylK;
	csprm->cs_sclh  = (cs_SCALH_CAST)CStacylH;
	csprm->cs_cnvrg = (cs_CNVRG_CAST)CStacylC;
	csprm->llchk    = (cs_LLCHK_CAST)CStacylL;
	csprm->xychk    = (cs_XYCHK_CAST)CStacylX;

	return;
}

/**********************************************************************
**	rtn_val = CStacylF (tacyl,xy,ll);
**
**	struct cs_Tacyl_ *tacyl;	structure containing all parameters necessary
**								for the transformation.
**	double xy [2];				converted values are returned here, X ([0])
**								and Y ([1]).
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

int EXP_LVL9 CStacylF (Const struct cs_Tacyl_ *tacyl,double xy [2],Const double ll [2])
{
	extern double cs_Zero;				/*  0.0 */
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Degree;			/*  1.0 / 57.29577... */
	extern double cs_Pi;				/*  3.14159... */
	extern double cs_Pi_o_2;			/*  PI/2 */
	extern double cs_AnglTest;			/* 0.001 arc seconds, in
										   radians. */
	extern double cs_NPTest;			/* 0.001 seconds of arc
										   short of the north pole,
										   in radians. */

	int rtn_val;

	double lng;
	double lat;
	double del_lng;

	double sin_lat;
	double esin_lat;

	double sin_beta;
	double beta, beta_c;
	double lat_c, sin_lat_c, cos_lat_c;
	double sin_del_lng;
	double cos_del_lng;
	double qq;
	double Mc;
	double tmp1;
	double tmp2;

	rtn_val = cs_CNVRT_NRML;

	/* The following is required by both the sperical and
	   ellipsoidal forms. */

	lng = ll [LNG] * cs_Degree;
	del_lng = lng - tacyl->org_lng;

	/* The following statement is used to deal with the 180 degree crack. */

	if (fabs (del_lng) > cs_Pi) del_lng = CS_adj2pi (del_lng);

	sin_del_lng = sin (del_lng);
	cos_del_lng = cos (del_lng);


	lat = ll [LAT] * cs_Degree;
	if (fabs (lat) > cs_NPTest)
	{
		rtn_val = cs_CNVRT_INDF;
		if (fabs (lat) > cs_Pi_o_2)
		{
			rtn_val = cs_CNVRT_RNG;
			lat = CS_adj1pi (lat);
		}
	}

	if (tacyl->ecent == 0.0)
	{
		/* Here for the sphere. */
		
		xy [XX] = tacyl->ka_o_h0 * cos (lat) * sin_del_lng;
		if (fabs (cos_del_lng) > cs_AnglTest)
		{
			tmp1 = atan (tan (lat) / cos_del_lng);
		}
		else
		{
			/* We are 90 degrees or so away from the
			   central meridian.  There is a singularity
			   at del_lng == 90 and lat == 0.0

			   However, there is a very rational result
			   for this singularity point. */

			rtn_val = cs_CNVRT_RNG;
			if      (lat > cs_AnglTest) tmp1 = cs_Pi_o_2;
			else if (lat < cs_AnglTest) tmp1 = -cs_Pi_o_2;
			else			    tmp1 = cs_Zero;
		}
		xy [YY] = tacyl->kah0 * (tmp1 - tacyl->org_lat);
	}
	else
	{
		/* Here for the ellipsoid.  If we are at 90 degrees
		   of longitude from the central meridian, the normal
		   equations are indeterminate.  However, the result
		   is well defined. */

		sin_lat = sin (lat);
		esin_lat = tacyl->ecent * sin_lat;
		tmp1 = (cs_One - esin_lat) / (cs_One + esin_lat);
		tmp1 = tacyl->one_o_2e * log (tmp1);
		tmp2 = sin_lat / (cs_One - esin_lat * esin_lat);
		qq = tacyl->one_m_esq * (tmp2 - tmp1);

		sin_beta = qq / tacyl->qp;
		if (fabs (sin_beta) >= cs_One)
		{
			sin_beta = (sin_beta >= 0.0) ? cs_One : cs_Mone;
		}
		beta = asin (sin_beta);
		if (fabs (cos_del_lng) > cs_AnglTest)
		{
			beta_c = atan (tan (beta) / cos_del_lng);
		}
		else
		{
			rtn_val = cs_CNVRT_RNG;
			if      (beta > cs_AnglTest) beta_c = cs_Pi_o_2;
			else if (beta < cs_AnglTest) beta_c = -cs_Pi_o_2;
			else			     beta_c = cs_Zero;
		}

		lat_c = CSbtIcalPrec (&tacyl->btcofI,beta_c);
		sin_lat_c = sin (lat_c);
		cos_lat_c = cos (lat_c);

		Mc = CSmmFcal (&tacyl->mmcofF,lat_c,sin_lat_c,cos_lat_c);
		xy [YY] = tacyl->h0 * (Mc - tacyl->M0);

		if (fabs (cos_del_lng) > cs_AnglTest)
		{
			tmp1 = cs_One - tacyl->e_sq * sin_lat_c * sin_lat_c;
			tmp1 = cos (beta_c) * sqrt (tmp1);
			tmp2 = cos (beta) * cos_lat_c * sin_del_lng;
			xy [XX] = tacyl->ka_o_h0 * (tmp2 / tmp1);
		}
		else
		{
			if (sin_del_lng >= 0.0) xy [XX] = tacyl->ka_o_h0;
			else			xy [XX] = -tacyl->ka_o_h0;
		}
	}

	if (tacyl->quad == 0)
	{
		xy [XX] += tacyl->x_off;
		xy [YY] += tacyl->y_off;
	}
	else
	{
		CS_quadF (xy,xy [XX],xy [YY],tacyl->x_off,tacyl->y_off,tacyl->quad);
	}
	return (rtn_val);
}

/**********************************************************************
**	rtn_val = CStacylI (tacyl,ll,xy);
**
**	struct cs_Tacyl_ *tacyl;	structure detailing the Transverse Equal Area
**								Cylindrical coordinate system in use.
**	double ll [2];				converted results returned here, longitude ([0])
**								and latitude ([1]), in degrees.
**	double xy [2];				coordinates to be converted are obtained from
**								this array, X ([0]) and Y ([1]).
**	int rtn_val;				returns cs_CNVRT_NRML if result is normal;
**								cs_CNVRT_RNG if value to be converted is
**								outside of mathematical scope of the
**								projection.
**
**	The xy and ll arrays may share the same memory.
**********************************************************************/

int EXP_LVL9 CStacylI (Const struct cs_Tacyl_ *tacyl,double ll [2],Const double xy [2])
{
	extern double cs_Zero;
	extern double cs_One;				/*  1.0 */
	extern double cs_Mone;				/* -1.0 */
	extern double cs_Radian;			/*  57.29577... */
	extern double cs_Pi;				/*  3.14159... */
	extern double cs_Pi_o_2;			/*  PI / 2.0 */
	extern double cs_AnglTest;			/* 0.001 arc seconds, in
										   radians. */

	int rtn_val;

	double xx;
	double yy;
	double del_lng;
	double lat;

	double qq;
	double Mc;
	double DD;
	double beta;
	double sin_DD;
	double cos_DD;
	double latc;
	double sin_lat;
	double sin_latc;
	double cos_latc;
	double esin_latc;
	double betac;
	double sin_betac;
	double cos_betac;
	double betap;
	double cos_betap;

	double tmp1;
	double tmp2;
	double tmp3;
	double tmp4;

	rtn_val = cs_CNVRT_NRML;

	/* Remove the false origin. */

	if (tacyl->quad == 0)
	{
		xx = xy [XX] - tacyl->x_off;
		yy = xy [YY] - tacyl->y_off;
	}
	else
	{
		CS_quadI (&xx,&yy,xy,tacyl->x_off,tacyl->y_off,tacyl->quad);
	}

	lat = cs_Zero;
	del_lng = cs_Zero;
	if (tacyl->ecent == 0.0)
	{
		/* Here for the sphere.  There are two special
		   cases to deal with.  If X is out of range, we
		   will end up taking the square root of a
		   negative number.  In this case, assuming
		   we are not at a pole (cos_DD != 0.0), we
		   set the longitude to be equal to org_lng
		   +- _PI/2 taking the sign of X.

		   If X is in range, but we are at either pole,
		   there is no longitude and we can pick any
		   value, like org_lng. */

		DD = yy / tacyl->kah0 + tacyl->org_lat;
		sin_DD = sin (DD);
		cos_DD = cos (DD);

		if (fabs (cos_DD) < cs_AnglTest)
		{
			/* Opps!!! We're at a pole.  Can't divide
			   by cos_DD.  Pick the right pole and
			   leave longitude ar org_lng. */

			rtn_val = cs_CNVRT_INDF;
			if (sin_DD > 0.0) lat = cs_Pi_o_2;
			else			  lat =  cs_Pi_o_2;
		}
		else
		{
			/* Not at either pole.  Make sure we don't
			   take the square root of a negative number. */

			tmp1 = xx * tacyl->h0_o_ka;
			tmp2 = cs_One - tmp1 * tmp1;

			if (tmp2 < cs_AnglTest)
			{
				if (tmp2 < 0.0) rtn_val = cs_CNVRT_RNG;
				tmp2 = cs_Zero;
			}
			else
			{
				tmp2 = sqrt (tmp2);
			}
			sin_lat = tmp2 * sin_DD;
			if (fabs (sin_lat) >= cs_One)
			{
				sin_lat = (sin_lat >= 0.0) ? cs_One : cs_Mone;
			}
			lat = asin (sin_lat);
			tmp2 *= cos_DD;

			/* tmp2 may be zero here, but tmp1 and tmp2
			   cannot both be zero.  Therefore, atan2 should
			   work OK in all cases. */

			del_lng = atan2 (tmp1,tmp2);
		}
	}
	else
	{
		/* Here for the ellipsoid.  The standard equations
		   are indeterminate at the poles.  At the current
		   time, the handling of |X| values greater than
		   R/h0 produces different results between the
		   spherical and ellipsoid cases. X isn't supposed
		   to get bigger than this, so it might not make
		   any difference to anyone. */

		Mc = tacyl->M0 + yy / tacyl->h0;
		latc = CSmmIcal (&tacyl->mmcofI,Mc);
		sin_latc = sin (latc);
		cos_latc = cos (latc);

		if (fabs (cos_latc) > cs_AnglTest)
		{
			/* Not at a pole. */

			esin_latc = tacyl->ecent * sin_latc;
			tmp1 = (cs_One - esin_latc) / (cs_One + esin_latc);
			tmp1 = tacyl->one_o_2e * log (tmp1);
			tmp2 = cs_One - esin_latc * esin_latc;
			qq = tacyl->one_m_esq * (sin_latc / tmp2 - tmp1);

			/* Seems a bit inefficient, but we need the
			   normalizing affect of the asin function
			   here.  sin_betac should never be more than
			   one, but noise in the low order bits during
			   the calculations require that we check to
			   prevent asin from generating a floating
			   point exception. */

			sin_betac = qq / tacyl->qp;
			if (fabs (sin_betac) >= cs_One)
			{
				sin_betac = (sin_betac >= 0.0) ? cs_One : cs_Mone;
			}
			betac = asin (sin_betac);
			sin_betac = sin (betac);
			cos_betac = cos (betac);

			/* Note what the following does when |X| is
			   greater than R/h0: Betap, and hence beta,
			   simply wraps back on themselves to computable
			   values.  I.e. an X value of R/h0+p is treated
			   the same as the value R/h0-p. */

			tmp3 = cos_betac * sqrt (tmp2) / cos_latc;
			tmp4 = xx * tacyl->h0_o_ka * tmp3;
			if (fabs (tmp4) > cs_One)
			{
				rtn_val = cs_CNVRT_RNG;
				tmp4 = (tmp4 >= 0.0) ? cs_One : cs_Mone;
			}
			betap = -asin (tmp4);
			cos_betap = cos (betap);
			beta = asin (cos_betap * sin_betac);

			/* We don't get here if we are at a pole.
			   Therefore, tan (betap) should be OK,
			   and cos_betac should never be zero.
			   Since cos_betac is always positive,
			   we can use atan just as well as atan2. */

			del_lng = -atan (tan (betap) / cos_betac);
			lat = CSbtIcalPrec (&tacyl->btcofI,beta);
		}
		else
		{
			/* At a pole, any longitude will do.  We'll
			   use the origin longitude by leaving lng
			   set to zero. */

			rtn_val = cs_CNVRT_INDF;
			if (latc > 0.0) lat = cs_Pi_o_2;
			else		lat = cs_Pi_o_2;
		}
	}

	if (fabs (del_lng) > cs_Pi)
	{
		del_lng = CS_adj2pi (del_lng);
		rtn_val = cs_CNVRT_RNG;
	}
	ll [LNG] = (del_lng + tacyl->org_lng) * cs_Radian;
	if (fabs (lat) > cs_Pi_o_2) rtn_val = cs_CNVRT_RNG;
	ll [LAT] = CS_adj1pi (lat) * cs_Radian;

	return (rtn_val);
}

/**********************************************************************
**	gamma = CStacylC (tacyl,ll);
**
**	struct cs_Tacyl_ *tacyl;	structure which carries all parameters
**								in effect for the coordinate system
**								being used, assuming the Transverse Equal Area
**								Cylindrical projection.
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

double EXP_LVL9 CStacylC (Const struct cs_Tacyl_ *tacyl,Const double ll [2])
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
	   the mathemagics of the specific projection involved.

	   Should be zero on the central meridian. */

	my_ll [LAT] -= 0.00005;
	status = CStacylF (tacyl,xy1,my_ll);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Km360);
	}
	my_ll [LAT] += 0.0001;		/* 2 * 0.00005 */
	status = CStacylF (tacyl,xy2,my_ll);
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
**	kk = CStacylK (tacyl,ll);
**
**	struct cs_Tacyl_ *tacyl;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				location of the point to be computed is given
**								here; longitude ([0]) and latitude ([1]) in
**								degrees.
**	double kk;					the true map scale factor at the specified point.
**								Should be compared with the tacyl->k.
**
**	Normal conventions and CS-MAP standards have NOT
**	been followed in codng this function so that the
**	variable nomenclature could adhere as closely
**	as possible to that used in the reference.
**********************************************************************/

double EXP_LVL9 CStacylK (Const struct cs_Tacyl_ *tacyl,Const double ll [2])
{
	extern double cs_Degree;		/* pi / 180.0 */
	extern double cs_Sin1Sec;		/* Sine of one arc second. */
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

	ll1 [LNG] = ll [LNG] - cs_HlfSecDeg;;
	ll1 [LAT] = ll [LAT];
	status = CStacylF (tacyl,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll2 [LNG] = ll [LNG] + cs_HlfSecDeg;
	ll2 [LAT] = ll [LAT];
	status = CStacylF (tacyl,xy2,ll2);
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
		/* This can happen when very close to the singularity
		   point for this projection. */

		return (cs_Mone);
	}

	if (tacyl->ecent == 0.0)
	{
		/* Here for a sphere.  cs_Sin1Sec is the sine of one
		   arc second; which is essentially the same as
		   one arc second in radians. */

		ll_dd = cs_Sin1Sec * tacyl->ka * cos (ll [LAT] * cs_Degree);
	}
	else
	{
		/* Calculate the geodetic distance between the two
		   lat/long points. */

		CS_llazdd (tacyl->ka,tacyl->e_sq,ll1,ll2,&ll_dd);
	}

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
**	kk = CStacylH (tacyl,ll);
**
**	struct cs_Tacyl_ *tacyl;	structure containing all parameters necessary
**								for the transformation.
**	double ll [2];				location of the point to be computed is given
**								here; longitude ([0]) and latitude ([1]) in
**								degrees.
**	double kk;					the true map scale factor at the specified point.
**********************************************************************/

double EXP_LVL9 CStacylH (Const struct cs_Tacyl_ *tacyl,Const double ll [2])
{
	extern double cs_HlfSecDeg;		/* One half second of arc in
									   degrees. */
	extern double cs_Sin1Sec;		/* Sine of one second of arc, used
									   here as one second of arc in
									   radians. */
	extern double cs_Mone;			/* -1, the value we return when
									   provided with a bogus point. */
	extern double cs_SclInf;		/* 9.9E+04, the value returned
									   to indicate an infinite
									   scale factor */
	int status;

	double hh;
	double ll_dd;
	double xy_dd;
	double del_xx, del_yy;

	double xy1 [2];
	double xy2 [2];
	double ll1 [2];
	double ll2 [2];

	/* Establish two points along the meridian which are
	   about 1 second (about 30 meters) apart from each
	   other, with the point in question in the middle.
	   Then convert each point to the equivalent grid
	   coordinates. */

	ll1 [LNG] = ll [LNG];
	ll1 [LAT] = ll [LAT] - cs_HlfSecDeg;
	status = CStacylF (tacyl,xy1,ll1);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	ll2 [LNG] = ll [LNG];
	ll2 [LAT] = ll [LAT] + cs_HlfSecDeg;
	status = CStacylF (tacyl,xy2,ll2);
	if (status != cs_CNVRT_NRML)
	{
		return (cs_Mone);
	}

	/* Calculate the geodetic distance between the two lat/long
	   points.  Note, we provide the geodetic calculator with
	   the scaled radius of the earth so that the distance
	   it computes will be in the same units as the X and
	   Y coordinates. */

	if (tacyl->ecent == 0.0)
	{
		ll_dd = cs_Sin1Sec * tacyl->ka;
	}
	else
	{
		CS_llazdd (tacyl->ka,tacyl->e_sq,ll1,ll2,&ll_dd);
	}

	/* Calculate the grid distance between the two points. */

	del_xx = xy1 [XX] - xy2 [XX];
	del_yy = xy1 [YY] - xy2 [YY];
	xy_dd = sqrt (del_xx * del_xx + del_yy * del_yy);

	/* Return the ratio of the two distances as the meridian
	   scale factor at the indicated point. ll_dd should never
	   be zero, but just in case. */

	if (ll_dd > 0.0)
	{
		hh = xy_dd / ll_dd;
	}
	else
	{
		hh = cs_SclInf;
	}
	return (hh);
}

/**********************************************************************
**	status = CStacylL (tacyl,cnt,pnts);
**
**	struct cs_Tacyl_ *tacyl;	the coordinate system against which the check is
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

int EXP_LVL9 CStacylL (Const struct cs_Tacyl_ *tacyl,int cnt,Const double pnts [][3])
{
	extern double cs_Degree;		/* 1.0 / 57.29...... */
	extern double cs_Pi_o_2;		/* PI over 2 */
	extern double cs_EETest;		/* 0.001 seconds of arc short
									   of 90 degrees, in radians. */

	int ii;
	int status;

	double lat;
	double del_lng;

	/* Check all the points.  */

	status = cs_CNVRT_OK;
	for (ii = 0;ii < cnt;ii++)
	{
		lat = pnts [ii][LAT] * cs_Degree;
		if (fabs (lat) > cs_Pi_o_2)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
		del_lng = (pnts [ii][LNG] - tacyl->org_lng) * cs_Degree;
		if (del_lng > cs_EETest)
		{
			status = cs_CNVRT_DOMN;
			break;
		}
	}
	return (status);
}

/**********************************************************************
**	status = CStacylX (tacyl,cnt,pnts);
**
**	struct cs_Tacyl_ *tacyl;	coordinate system definition
**	int cnt;					number of points in the pnts array.
**	double pnts [][3];			an array of three dimensional points which
**								define a point, line, or region to be checked.
**	int status;					returns the status of the check; see REMARKS
**								below.
**
**	The values provided in the pnts array are required to be
**	cartesian coordinates.  Use CStacylL to check lat/long
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

int EXP_LVL9 CStacylX (Const struct cs_Tacyl_ *tacyl,int cnt,Const double pnts [][3])
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
		if (tacyl->quad == 0)
		{
			xx = pnts [ii][XX] - tacyl->x_off;
			yy = pnts [ii][YY] - tacyl->y_off;
		}
		else
		{
			CS_quadI (&xx,&yy,pnts [ii],tacyl->x_off,tacyl->y_off,tacyl->quad);
		}
		if (fabs (xx) > tacyl->max_xx || fabs (yy) > tacyl->max_yy)
		{
			rtn_val = cs_CNVRT_DOMN;
			break;
		}
	}
	return (rtn_val);
}
